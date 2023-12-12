/*
 * Ext_Out.cpp
 *
 * Created: 11.12.2023 18:35:03
 *  Author: Kuba
 */ 
#ifndef _Ext_Out_H_
#define _Ext_Out_H_

#define  F_CPU 4000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

extern volatile uint8_t on_off_flag;

#define LED_PIN PIN3_bm


void IN_OUT_Init();
void ON_OFF();
#endif