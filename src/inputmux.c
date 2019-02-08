#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>

#include "Descriptors.h"

// #include <LUFA/Drivers/Board/LEDs.h>
// #include <LUFA/Drivers/Board/Joystick.h>
// #include <LUFA/Drivers/Board/Buttons.h>
// #include <LUFA/Drivers/Peripheral/TWI.h>

// if ((scanaddress & 3) == 0) {
// 	PORTB |= (1 << 1); // Addr 0
// } else {
// 	PORTB &= ~(1 << 1); // Addr 0
// }

// if ((scanaddress & 3) == 1) {
// 	PORTB |= (1 << 3); // Addr 1
// } else {
// 	PORTB &= ~(1 << 3); // Addr 1
// }

// if ((scanaddress & 3) == 2) {
// 	PORTB |= (1 << 2); // Addr 2
// } else {
// 	PORTB &= ~(1 << 2); // Addr 3
// }



// MUX pins, 4,   5,   6,   7
// MUX ports PD4, PC6, PD7, PE6

void inputmux_init() {

	// DDRD |= (1 << 4);
	// DDRC |= (1 << 6);
	// DDRD |= (1 << 7);
	// DDRE |= (1 << 6);

	DDRB |= (1 << 1); // Addr 0
	DDRB |= (1 << 3); // Addr 1
	DDRB |= (1 << 2); // Addr 2

}

void inputmux_setinput(int input) {
	// if ((input & 1) == 1) {
	// 	PORTD |= (1 << 4);
	// } else {
	// 	PORTD &= ~(1 << 4);
	// }

	// if (input & 2 == 2) {
	// 	PORTC |= (1 << 6);
	// } else {
	// 	PORTC &= ~(1 << 6);
	// }

	// if (input & 4 == 4) {
	// 	PORTD |= (1 << 7);
	// } else {
	// 	PORTD &= ~(1 << 7);
	// }

	// if (input & 8 == 8) {
	// 	PORTE |= (1 << 6);
	// } else {
	// 	PORTE &= ~(1 << 6);
	// }

	if ((input & 1) == 1) {
		PORTB |= (1 << 1); // Addr 0
	} else {
		PORTB &= ~(1 << 1); // Addr 0
	}

	if ((input & 2) == 2) {
		PORTB |= (1 << 3); // Addr 1
	} else {
		PORTB &= ~(1 << 3); // Addr 1
	}

	if ((input & 4) == 4) {
		PORTB |= (1 << 2); // Addr 2
	} else {
		PORTB &= ~(1 << 2); // Addr 3
	}

}
