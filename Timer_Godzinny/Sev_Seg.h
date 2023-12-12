/*
 * Sev_Seg1.h
 *
 * Created: 08.12.2023 20:27:57
 *  Author: Kuba
 */ 
#ifndef _SEGMENT_DISPLAY_H_
#define _SEGMENT_DISPLAY_H_
#define  F_CPU 4000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

// Definitions of segments, ports, pins, and segment codes go here...

// Function prototypes
void Timer_init(void);
void Disp_init(void);
void DisplayNumber(uint16_t value);
void ClearDisplay(void);
#endif // _SEGMENT_DISPLAY_H_
