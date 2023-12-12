/*
 * Encoder.cpp
 *
 * Created: 11.12.2023 18:02:52
 *  Author: Kuba
 */ 


#include "encoder.h"
volatile uint8_t countdown_active;
volatile int16_t encoderValue;
volatile uint8_t lastEncoded;
volatile int8_t stepCount;
volatile uint16_t hours = 0;
volatile uint16_t minutes = 0;
volatile uint8_t start_countdown;
volatile uint8_t select_mode = 0;  // 0 - wybór godzin, 1 - wybór minut, 2 - rozpoczêcie odliczania

void readEncoder() {
	if(!countdown_active) { 
    if(select_mode != 2) {  // Dodajemy ten warunek
        uint8_t MSB = (PORTA.IN & ENCODER_A_PIN) != 0; //MSB = most significant bit
        uint8_t LSB = (PORTA.IN & ENCODER_B_PIN) != 0; //LSB = least significant bit

        uint8_t encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
        uint8_t sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

        if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) stepCount++;
        if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) stepCount--;

        if(stepCount >= STEPS_PER_ROTATION) {
            encoderValue++;
            stepCount -= STEPS_PER_ROTATION;
        } else if(stepCount <= -STEPS_PER_ROTATION) {
            encoderValue--;
            stepCount += STEPS_PER_ROTATION;
        }
        // Ogranicz wartoœæ encoderValue do zakresu 0-23 dla godzin i 0-59 dla minut
        if(encoderValue < 0) {
            encoderValue = 0;
        } else if(select_mode == 0 && encoderValue > 23) {
            encoderValue = 23;
        } else if(select_mode == 1 && encoderValue > 59) {
            encoderValue = 59;
        }
        lastEncoded = encoded; //store this value for next time
        if(select_mode == 0) {
            hours = encoderValue;
        } else if(select_mode == 1) {
            minutes = encoderValue;
        }
		}
}
		}

void encoder_init() {
	PORTA.DIRCLR = ENCODER_A_PIN | ENCODER_B_PIN | BUTTON_PIN;
	PORTA.PIN4CTRL = PORT_PULLUPEN_bm;
	PORTA.PIN5CTRL = PORT_PULLUPEN_bm;
	PORTA.PIN3CTRL = PORT_ISC_FALLING_gc | PORT_PULLUPEN_bm;
	PORTA.PIN4CTRL |= PORT_ISC_BOTHEDGES_gc;
	PORTA.PIN5CTRL |= PORT_ISC_BOTHEDGES_gc;
}