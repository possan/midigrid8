#include <avr/io.h>
#include <stdbool.h>
#include <string.h>

#include "midistate.h"
#include "usbmidi.h"

#define MAX_CONTROLLERS 16

int16_t knobstate[MAX_CONTROLLERS] = { 9999, };
unsigned char knobhistory[MAX_CONTROLLERS] = { 0, };
unsigned char knobdirty[MAX_CONTROLLERS] = { 0 };

void midistate_init() {}

void midistate_report(int idx, int rawvalue) {
	int16_t newvalue = (int16_t)rawvalue - 130;
	if (newvalue < 0) {
		newvalue = 0;
	}

	newvalue = (newvalue * 12) / 100;
	if (newvalue > 127) {
		newvalue = 127;
	}

	if (knobhistory[idx] != 9999) {
		newvalue = (newvalue + knobhistory[idx]) >> 1;
	}

	knobhistory[idx] = newvalue;

	int16_t oldvalue = knobstate[idx];

	if (newvalue == oldvalue) {
		// Don't send if there is no change.
		return;
	}

	// Remember new value
	knobstate[idx] = newvalue;

	if (oldvalue == 9999) {
		// Don't send initial value, just remember it.
		return;
	}

	knobdirty[idx] = 1;
}

void midistate_flush() {
    for(int i=0; i<MAX_CONTROLLERS; i++) {
        if (knobdirty[i]) {
            knobdirty[i] = 0;
            usbmidi_sendcontrolchange(1, i, knobstate[i]);
        }
    }
}
