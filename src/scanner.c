#include <avr/io.h>

#include "scanner.h"
#include "ads1015.h"
#include "midistate.h"

unsigned char scanmux = 0;
unsigned char scanchannel = 0;
unsigned char scanstate = 0;

unsigned int scancounter = 0;

void scanner_init() {}

void scanner_tick() {
	// inputmux_setinput(rand() % 2);

	if (scanstate == 0) {
		// set up mux
		inputmux_setinput(scanmux);

		scancounter = 100; // needs time to get ADC up to speed
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
		// scancounter = 20000;
		scancounter = 50;

	} else if (scanstate == 3) {

		// wait a bit before reading back result (short)

		scancounter --;
		if (scancounter < 1) {
			scanstate = 4;
		}

	} else if (scanstate == 4) {

		// read results back from i2c

		uint16_t value = ads1015_read(0);
		midistate_report(scanmux * 4 + scanchannel, value);

		scanstate = 5;

	} else if (scanstate == 5) {

		scanchannel ++;

		if (scanchannel < 4) {
			// read next channel
			scanstate = 2;
		} else {
			scanmux ++;
			if (scanmux > 3) {
				scanmux = 0;
			}
			scanstate = 6;
		}

	} else if (scanstate == 6) {
		midistate_flush();

		scanstate = 0;
	}
}
