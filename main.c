/*
 * Lab1.cpp
 *
 * Created: 23/01/2025 19:14:18
 * Author : luisd
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "display.h"

// Variable de 8 bits para almacenar las banderas de los botones
volatile uint8_t banderas = 0;
volatile uint8_t contador1 = 0;
volatile uint8_t contador2 = 0;

// Función de antirrebote
void debounce() {
	_delay_ms(50);  // Pequeño retardo para evitar rebotes
}

void setup() {
	
	// Configurar pines de LEDs como salida (PORTB)
	DDRB = 0xFF;  // Configurar PORTB como salida
	DDRC = 0b00010001;  // Configurar PORTC como salida (excepto pines PC1, PC2, PC3)
	DDRD = 0xFF; // Configurar PORTD como salida
	
	PORTC |= (1 << PC1) | (1 << PC2) | (1 << PC3); // Pull-ups activadas

	UCSR0B &= ~((1 << RXEN0) | (1 << TXEN0));  // Deshabilita RX y TX

	// Asegurarse de que los pines están a nivel bajo
	PORTD &= ~((1 << PD0) | (1 << PD1));  // Establecer ambos pines en 0

	// Configurar interrupciones de cambio de pin (Pin Change Interrupts)
	PCICR |= (1 << PCIE1);   // Habilitar interrupciones en PORTC (PCINT8-14)
	PCMSK1 |= (1 << PCINT9) | (1 << PCINT10) | (1 << PCINT11);  // Habilitar interrupción en PC1, PC2, PC3
	
	//CLKPR = (1<<CLKPCE);  // Habilita la modificación del prescaler
	//CLKPR = (1<<CLKPS3);  // Ajusta el prescaler a 8 MHz (por ejemplo, dividiendo la frecuencia de 16 MHz entre 2)

	sei();  // Habilitar interrupciones globales
}

int main(void) {
	setup();  // Configurar el sistema
	


	while (1) {
		if (banderas & (1 << 0)) {  // Si el bit 0 está activo (botón PC1 presionado)
			PORTD=0;
			contador1 = 0;  // Reiniciar
			contador2 = 0;  // Reiniciar
			for (int8_t  i=5; i>=0; i--){
				ShowDisp(i);  // Muestra el número en el display
				_delay_ms(1000);    // Espera 1 segundo
			}
			banderas &= ~(1 << 0);  // Limpiar el bit 0
			banderas |= (1 << 1); // Activar bit 1 (Permite inicio de carrera)
		}
		if (banderas & (1 << 2)) {  // Si el bit 3 está activo (botón PC2 presionado)
			contador1++;
			if (contador1 > 15) {
				contador1 = 15;  // Dejar en 15
				contador2 = 0;  // Reiniciar después de 15
				PORTD = (PORTD & 0xF0) | (contador2 & 0x0F);
				banderas &= ~(1 << 1);  // Limpiar el bit 1
				ShowDisp(1);  // Muestra el número en el display
			}
			// Actualizar solo los primeros 4 bits del puerto D (PD0-PD3)
			PORTD = (PORTD & 0x0F) | ((contador1 & 0x0F) << 4);
			banderas &= ~(1 << 2);  // Limpiar el bit 2
		}
		if (banderas & (1 << 3)) {  // Si el bit 2 está activo (botón PC3 presionado)
			contador2++;
			if (contador2 > 15) {
				contador1 = 0;  // Reiniciar después de 15
				contador2 = 15;  // Dejar en 15
				PORTD = (PORTD & 0x0F) | ((contador1 & 0x0F) << 4);
				banderas &= ~(1 << 1);  // Limpiar el bit 1
				ShowDisp(2);  // Muestra el número en el display
			}
			// Actualizar solo los últimos 4 bits del puerto D (PD0-PD3)
			PORTD = (PORTD & 0xF0) | (contador2 & 0x0F);
			banderas &= ~(1 << 3);  // Limpiar el bit 3
		}
	}
}

// Rutina de interrupción para botones en PCINT8-PCINT14 (PORTC)
ISR(PCINT1_vect) {
	debounce();  // Aplicar antirrebote

	// Verificar el estado de los botones después del rebote
	if (!(PINC & (1 << PC1))) {
		banderas &= ~(1 << 1);  // Limpiar el bit 1
		banderas |= (1 << 0);  // Activar el bit 0 si se presiona el botón en PC1
	}

	if (banderas & (1 << 1)){
		if (!(PINC & (1 << PC2))) {
			if (!(banderas & (1 << 4))){
				banderas |= (1 << 2);  // Activar el bit 2 si se presiona el botón en PC2
				banderas |= (1 << 4);  // Activar el bit 4 si se presiona el botón en PC2
			}
		} else {
			banderas &= ~(1 << 4);  // Limpiar el bit 4
		}

		if (!(PINC & (1 << PC3))) {
			if (!(banderas & (1 << 5))){
				banderas |= (1 << 3);  // Activar el bit 3 si se presiona el botón en PC3
				banderas |= (1 << 5);  // Activar el bit 5 si se presiona el botón en PC3
			}
		} else {
			banderas &= ~(1 << 5);  // Limpiar el bit 5
		}
	}
}