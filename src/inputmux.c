#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>

// MUX pins, 4,   5,   6,   7
// MUX ports PD4, PC6, PD7, PE6

void inputmux_init() {
	DDRB |= (1 << 1); // Addr 0
	DDRB |= (1 << 3); // Addr 1
	DDRB |= (1 << 2); // Addr 2
	// DDRB |= (1 << 2); // Addr 3
}

void inputmux_setinput(int input) {
	if ((input & 1) == 1) {
		PORTB |= (1 << 1);
	} else {
		PORTB &= ~(1 << 1);
	}

	if ((input & 2) == 2) {
		PORTB |= (1 << 3);
	} else {
		PORTB &= ~(1 << 3);
	}

	if ((input & 4) == 4) {
		PORTB |= (1 << 2);
	} else {
		PORTB &= ~(1 << 2);
	}

	// if ((input & 4) == 4) {
	// 	PORTB |= (1 << 2);
	// } else {
	// 	PORTB &= ~(1 << 2);
	// }
}
