/*
 * display.c
 *
 * Created: 23/01/2025 20:11:49
 *  Author: luisd
 */ 
// display.c

#include <avr/io.h>
#include "display.h"

void ShowDisp(uint8_t n){
	switch(n){
		case 1: PORTC = 0b00011110; PORTB = 0b00000010; break;
		case 2: PORTC = 0b00011110; PORTB = 0b00111100; break;
		case 3: PORTC = 0b00011110; PORTB = 0b00110110; break;
		case 4: PORTC = 0b00011111; PORTB = 0b00010010; break;
		case 5: PORTC = 0b00001111; PORTB = 0b00110110; break;
		case 6: PORTC = 0b00001111; PORTB = 0b00011110; break;
		case 7: PORTC = 0b00011110; PORTB = 0b00100010; break;
		case 8: PORTC = 0b00011111; PORTB = 0b00111110; break;
		case 9: PORTC = 0b00011111; PORTB = 0b00110010; break;
		case 0: PORTC = 0b00011111; PORTB = 0b00101110; break;
		//default: PORTC = 0b00011111; PORTB =  0b00101110; break;
	}
	
}
