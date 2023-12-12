/*
 * Sev_Seg.c
 *
 * Created: 08.12.2023 20:26:23
 *  Author: Kuba
 */ 
// Definicje segmentów
#include "Sev_Seg.h"

#define SegA    PIN5_bm
#define SegB    PIN0_bm
#define SegC    PIN2_bm
#define SegD    PIN3_bm
#define SegE    PIN6_bm
#define SegF    PIN7_bm
#define SegG    PIN1_bm
#define SegDot  PIN4_bm

// Definicje portu dla segmentów
#define PORT_SEGMENTS  PORTD

// Definicje portu dla sterowania cyframi
#define PORT_CTRL  PORTE

// Definicje pinów dla sterowania poszczególnymi cyframi
#define SegOne   PIN2_bm
#define SegTwo   PIN1_bm
#define SegThree PIN0_bm
#define SegFour  PIN3_bm

// Definicje kodów segmentów
//               0bFEA.DCGB
#define SEG_0    0b11101101 //                                        AAAA
#define SEG_1    0b00000101 //                                      F      B
#define SEG_2    0b01101011 //                                      F      B
#define SEG_3    0b00101111 //                                        GGGG
#define SEG_4    0b10000111 //                                      E      C
#define SEG_5    0b10101110 //                                      E      C
#define SEG_6    0b11101110 //                                        DDDD      DP
#define SEG_7    0b00100101
#define SEG_8    0b11101111
#define SEG_9    0b10101111
#define SEG_A    0b11100111
#define SEG_B    0b11001110
#define SEG_C    0b11101000
#define SEG_D    0b01001111
#define SEG_E    0b11101010
#define SEG_F    0b11100010

// Zmienna przechowuj¹ca liczbê do wyœwietlenia
volatile uint16_t cnt = 0;

// Tablica kodów segmentów
char seg_code[] = {
	SEG_0, SEG_1, SEG_2, SEG_3, SEG_4, SEG_5, SEG_6, SEG_7,
	SEG_8, SEG_9, SEG_A, SEG_B, SEG_C, SEG_D, SEG_E, SEG_F
};

// Funkcja do obs³ugi przerwania od timera
ISR(TCA0_OVF_vect) {
	static uint8_t digit_count = 0;
	uint8_t temp;

	PORT_CTRL.OUTCLR = SegOne | SegTwo | SegThree | SegFour;

	switch (digit_count) {
		case 0:
		temp = cnt / 1000;
		PORT_CTRL.OUTSET = SegOne;
		PORT_SEGMENTS.OUT = seg_code[temp];
		break;
		case 1:
		temp = (cnt / 100) % 10;
		PORT_CTRL.OUTSET = SegTwo;
		PORT_SEGMENTS.OUT = seg_code[temp];
		break;
		case 2:
		temp = (cnt / 10) % 10;
		PORT_CTRL.OUTSET = SegThree;
		PORT_SEGMENTS.OUT = seg_code[temp];
		break;
		case 3:
		temp = cnt % 10;
		PORT_CTRL.OUTSET = SegFour;
		PORT_SEGMENTS.OUT = seg_code[temp];
		break;
	}

	digit_count = (digit_count + 1) % 4;
	TCA0.SINGLE.INTFLAGS |= TCA_SINGLE_OVF_bm;
}

// Funkcja inicjalizuj¹ca timer
void Timer_init() {
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	TCA0.SINGLE.PER = 18;  // Przerwanie co 1000 cykli zegara
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1024_gc;
	TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

// Funkcja inicjalizujaca wyswietlacze
void Disp_init(){
	PORT_SEGMENTS.DIRSET = SegA | SegB | SegC | SegD | SegE | SegF | SegG | SegDot; // Data lines
	PORT_CTRL.DIRSET = SegOne | SegTwo | SegThree | SegFour; // Control lines
}

// Funkcja do wyœwietlania liczby na wyœwietlaczu
void DisplayNumber(uint16_t value) {
	cnt = value;
}
void ClearDisplay() {
	PORT_SEGMENTS.OUTCLR = SegA | SegB | SegC | SegD | SegE | SegF | SegG | SegDot; // Wy³¹cz wszystkie segmenty
	PORT_CTRL.OUTCLR = SegOne | SegTwo | SegThree | SegFour; // Wy³¹cz wszystkie cyfry
}
