/*
 * Encoder.cpp
 *
 * Created: 11.12.2023 18:03:03
 *  Author: Kuba
 */ 
#ifndef ENCODER_H
#define ENCODER_H

#include <avr/io.h>

#define ENCODER_A_PIN PIN4_bm
#define ENCODER_B_PIN PIN5_bm
#define BUTTON_PIN PIN3_bm
#define STEPS_PER_ROTATION 5  // Liczba kroków na obrót
extern volatile uint8_t countdown_active;
extern volatile int16_t encoderValue;
extern volatile uint8_t lastEncoded;
extern volatile int8_t stepCount;
extern volatile uint16_t hours;  // Deklaracja zmiennej 'hours'
extern volatile uint16_t minutes;  // Deklaracja zmiennej 'minutes'
extern volatile uint8_t start_countdown;  // Deklaracja zmiennej 'start_countdown'
extern volatile uint8_t select_mode;  // Deklaracja zmiennej 'select_mode'
void readEncoder();
void encoder_init();

#endif // ENCODER_H