#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>

#include "Descriptors.h"

uint16_t lastbankbuttonstate = 0;
uint16_t lastbankdebouncetimer = 0;
uint16_t lastvalue = 0;
uint16_t lastvaluetimer = 0;

void bankui_init() {
    // Button is on pin 5
	DDRC &= ~(1 << 6); // Pin 5 - PC6

    // Leds are on pin 6..9
	DDRD |= (1 << 7); // Pin 6 - PD7
	DDRE |= (1 << 6); // Pin 7 - PE6
	DDRB |= (1 << 4); // Pin 8 - PB4
	DDRB |= (1 << 5); // Pin 9 - PB5

    lastbankdebouncetimer = 0;
    lastvaluetimer = 0;
}

void bankui_setleds(int a, int b, int c, int d) {
    if (a) {
        PORTD |= (1 << 7);
    } else {
        PORTD &= ~(1 << 7);
    }

    if (b) {
        PORTE |= (1 << 6);
    } else {
        PORTE &= ~(1 << 6);
    }

    if (c) {
        PORTB |= (1 << 4);
    } else {
        PORTB &= ~(1 << 4);
    }

    if (d) {
        PORTB |= (1 << 5);
    } else {
        PORTB &= ~(1 << 5);
    }
}

void bankui_flash1() {
    bankui_setleds(1,0,0,0);
	_delay_ms(100);
    bankui_setleds(1,1,0,0);
	_delay_ms(100);
    bankui_setleds(1,1,1,0);
	_delay_ms(100);
    bankui_setleds(1,1,1,1);
	_delay_ms(100);
}

void bankui_flash2() {
	// _delay_ms(1000);
    bankui_setleds(0,1,1,1);
	_delay_ms(100);
    bankui_setleds(0,0,1,1);
	_delay_ms(100);
    bankui_setleds(0,0,0,1);
	_delay_ms(100);
    bankui_setleds(0,0,0,0);
	_delay_ms(100);
}

void bankui_reportvalue(int value) {
    if (value != lastvalue) {
        lastvalue = value;
        lastvaluetimer = 4000;
    }
}

void bankui_tick() {
    // scan button

    uint16_t bankbuttonstate = (PINC & (1 << 6)) == (1 << 6);
    if (bankbuttonstate != lastbankbuttonstate) {
        lastbankbuttonstate = bankbuttonstate;
        if (!bankbuttonstate && lastbankdebouncetimer > 100) {
            // bank button is released and debounced, change bank.
            midistate_setbank((midistate_getbank() + 1) % 4);
            lastvaluetimer = 0;
        }
        lastbankdebouncetimer = 0;
    }

    // update leds

    if (lastvaluetimer > 1000) {
        // show knob value
        lastvaluetimer --;
        int v = (lastvalue >> 2) % 4;
        bankui_setleds(v == 0, v == 1, v == 2, v == 3);
    } else if (lastvaluetimer > 1) {
        // a short blank period before showing bank again
        lastvaluetimer --;
        bankui_setleds(0, 0, 0, 0);
    } else {
        uint16_t v = midistate_getbank();
        bankui_setleds(v == 0, v == 1, v == 2, v == 3);
    }

    if (lastbankdebouncetimer < 64000) {
        lastbankdebouncetimer ++;
    }
}
