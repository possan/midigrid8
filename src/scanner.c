#include <avr/io.h>

#include "scanner.h"
#include "ads1015.h"
#include "midistate.h"

uint8_t scanmux = 0;
uint8_t scanchannel = 0;
uint8_t scanstate = 0;

uint16_t scancounter = 0;

void scanner_init() {
}

void scanner_tick() {
	if (scanstate == 0) {
		// set up mux
		inputmux_setinput(scanmux);

		scancounter = 3; // needs time to get ADC up to speed
		scanchannel = 0;
		scanstate = 1;

	} else if (scanstate == 1) {

		// wait a bit

		scancounter --;
		if (scancounter < 1) {
			scanstate = 2;
		}

	} else if (scanstate == 2) {

		ads1015_prepare(0, scanchannel);

		scanstate = 3;
		scancounter = 3;

	} else if (scanstate == 3) {

		// wait a bit before reading back result (short)

		scancounter --;
		if (scancounter < 1) {
			scanstate = 4;
		}

	} else if (scanstate == 4) {

		// read results back from i2c

		int16_t value = ads1015_read(0);
		if (value != -1) {
			midistate_report(scanmux * 3 + scanchannel, value);
		}

		scanstate = 5;

	} else if (scanstate == 5) {
		scanchannel ++;
		if (scanchannel < 3) {
			// read next channel in same mux
			scanstate = 2;
		} else {
			scanmux ++;
			scanmux %= 16;
			scanstate = 6;
		}

	} else if (scanstate == 6) {

		scanstate = 0;
	}
}
