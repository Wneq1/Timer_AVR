/*
 * Ext_Out.cpp
 *
 * Created: 11.12.2023 18:34:46
 *  Author: Kuba
 */ 
#include "Ext_Out.h"

volatile uint8_t on_off_flag;

void IN_OUT_Init(){
		PORTB.DIRSET = LED_PIN;                                   // Ustaw pin diody (PB3) jako wyjœcie
		PORTB.OUTSET = LED_PIN;                                   // Wy³¹cz diodê LED
}

void ON_OFF(){
	switch(on_off_flag){
	case 0:
	PORTB.OUTCLR = LED_PIN; 
	break;
	case 1:
	PORTB.OUTSET = LED_PIN; 	
	break;
	}
}
