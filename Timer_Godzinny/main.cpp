/*
 * Timer_Godzinny.cpp
 *
 * Created: 11.12.2023 19:09:32
 * Author : Kuba
 */ 

#define  F_CPU 4000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "Sev_Seg.h"
#include "Encoder.h"
#include "Ext_Out.h"

#define BUZZER_PIN PIN1_bm
#define RESET_BUTTON PIN2_bm

volatile uint8_t buzzer_flag = 0;
volatile uint32_t milliseconds = 0;
volatile uint8_t countdown_started = 0;  // Dodajemy now� zmienn�

void Start_Buzzer(){
	if(buzzer_flag){
		PORTC.OUTSET =BUZZER_PIN;
		_delay_ms(200);
		PORTC.OUTCLR =BUZZER_PIN;
		_delay_ms(100);
		PORTC.OUTSET =BUZZER_PIN;
		_delay_ms(500);
		PORTC.OUTCLR =BUZZER_PIN;
		_delay_ms(200);
		buzzer_flag=0;
	}
}
void DisplayTime(uint32_t hours, uint32_t minutes)
{
	uint32_t time = hours * 100 + minutes;  // Konwertuj czas na format HHMM
	DisplayNumber(time);  // Wy�wietl czas
}
void DisplayRemainTime(uint32_t hours, uint32_t minutes, uint32_t milliseconds)
{
	uint32_t total_seconds = (hours * 3600) + (minutes * 60);
	uint32_t elapsed_seconds = milliseconds / 1000;
	uint32_t remaining_seconds = total_seconds - elapsed_seconds;

	uint32_t display_minutes = remaining_seconds / 60;
	uint32_t display_hours = display_minutes / 60;
	display_minutes = display_minutes % 60;  // Oblicz pozosta�e minuty po odj�ciu godzin

	DisplayTime(display_hours, display_minutes);  // Wy�wietl pozosta�y czas
}
ISR(TCA1_OVF_vect)
{
	if(select_mode == 2)
	{
		countdown_started = 1;  // Rozpocz�li�my odliczanie
		on_off_flag = 0;
		milliseconds++;
		if(milliseconds % 1000 == 0)  // Co sekund�
		{
			uint32_t remaining_seconds = (hours * 3600) - (milliseconds / 1000);
			uint32_t display_minutes = remaining_seconds / 60;
			uint32_t display_hours = display_minutes / 60;
			display_minutes = display_minutes % 60;  // Oblicz pozosta�e minuty po odj�ciu godzin
			DisplayTime(display_hours, display_minutes);  // Wy�wietl pozosta�y czas
		}
		if(milliseconds >= ((uint32_t)(hours * 60 + minutes) * 60 * 1000))  // Po okre�lonej liczbie godzin
		{
			on_off_flag = 1;
			buzzer_flag = 1;
			milliseconds = 0;
			select_mode = 0;
			hours = 0;
			minutes =0;
			countdown_started = 0;
		}
	}

	TCA1.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;  // Wyczy�� flag� przerwania
}

ISR(PORTB_PORT_vect){
	if(!(PORTB.IN & RESET_BUTTON))  // Je�li przycisk Pb2 jest naci�ni�ty
	{
				countdown_started = 0;  // Zatrzymujemy odliczanie
		select_mode = 0;  // Zatrzymaj odliczanie
		hours = 0;  // Zresetuj godziny
		minutes = 0;  // Zresetuj minuty
		milliseconds = 0;  // Zresetuj milisekundy
		countdown_active = 0;  // Zatrzymujemy odliczanie
		DisplayTime(hours, minutes);  // Wy�wietl zresetowany czas
		on_off_flag =1;
	}
	PORTB.INTFLAGS=RESET_BUTTON;
}


ISR(PORTA_PORT_vect)
{
	if(!(PORTA.IN & BUTTON_PIN))  // Je�li przycisk jest naci�ni�ty
	{
		if(!countdown_started) {  // Je�li odliczanie nie zosta�o jeszcze rozpocz�te
			if(select_mode < 2) {
				select_mode++;
				encoderValue = 0;
				} else {
				select_mode = 0;
			}
		}
	}

	// Obs�uga enkodera
	if(!countdown_started) {  // Je�li odliczanie nie zosta�o jeszcze rozpocz�te
		readEncoder();
	}

	PORTA.INTFLAGS = BUTTON_PIN | ENCODER_A_PIN | ENCODER_B_PIN;  // Wyczy�� flagi przerwania
}
void Timer_2_Init(){
	// Konfiguracja Timer/Counter Type A (TCA)
	TCA1.SINGLE.CTRLA = TCA_SINGLE_ENABLE_bm;         // W��cz timer
	TCA1.SINGLE.CTRLA |= TCA_SINGLE_CLKSEL_DIV16_gc;  // Ustaw prescaler na DIV16
	TCA1.SINGLE.PER = (F_CPU / 16 / 1000) - 1;        // Ustaw okres na 1 ms
	TCA1.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;          // W��cz przerwanie od przepelnienia
}
void Periphals_Init(){
	PORTC.DIRSET = BUZZER_PIN;
	PORTB.DIRCLR = RESET_BUTTON;
	
	PORTB.PIN2CTRL = PORT_ISC_FALLING_gc |PORT_PULLUPEN_bm;
	

}


int main(void) {
	Timer_init();
	Disp_init();
	Timer_2_Init();
	Periphals_Init();
	encoder_init();
	IN_OUT_Init();
	sei();
on_off_flag =1;
	while (1) {
		ON_OFF();
		if(select_mode < 2)
		{
			if(select_mode == 0) {
				DisplayTime(hours, minutes);  // Wy�wietl ustawione godziny
				} else if(select_mode == 1) {
				DisplayTime(hours, minutes);  // Wy�wietl ustawione minuty
			}
			Start_Buzzer();
			} else if(select_mode == 2) {
			DisplayRemainTime(hours, minutes, milliseconds);  // Wy�wietl pozosta�y czas
		}
	}
}
