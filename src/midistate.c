#include <avr/io.h>
#include <stdbool.h>
#include <string.h>

#include "midistate.h"
#include "usbmidi.h"

#define MAX_PHYSICAL_CONTROLLERS 48
#define HISTORY_VALUES 5

// uint16_t knoblastread[MAX_PHYSICAL_CONTROLLERS] = { 0, };
uint16_t knoblastvalue[MAX_PHYSICAL_CONTROLLERS] = { 9999, };
uint8_t knobreadcount[MAX_PHYSICAL_CONTROLLERS] = { 0, };
// uint16_t knobhistory[MAX_PHYSICAL_CONTROLLERS] = { 0, };
uint16_t knobhistory2[MAX_PHYSICAL_CONTROLLERS * HISTORY_VALUES] = { 9999, };
uint8_t knobdirty[MAX_PHYSICAL_CONTROLLERS] = { 0, };

int current_bank = 0;
int tick_counter = 0;

void midistate_init() {
	current_bank = 0;

	for(int i=0; i<MAX_PHYSICAL_CONTROLLERS; i++) {
		knoblastvalue[i] = 9999;
		knobdirty[i] = 0;
		knobreadcount[i] = 0;
	}

	for(int i=0; i<HISTORY_VALUES * MAX_PHYSICAL_CONTROLLERS; i++) {
		knobhistory2[i] = 0;
	}
}

int midistate_getbank() {
	return current_bank;
}

void midistate_setbank(int bank) {
	current_bank = bank;
}

void midistate_report(int physicalid, int16_t rawvalue) {
	// rawvalue = 12bit ~= 0 - 4095 ~ 0-4.096v, 0-3v = ~0-3000?

	rawvalue -= 5;

	// clamp down.
	if (rawvalue < 0) {
		rawvalue = 0;
	}

	uint16_t newvalue = (uint16_t)rawvalue;
	newvalue *= 5;
	newvalue /= 60;
	// newvalue *= 8;

	// don't clamp values up because of rounding issues.
	// knoblastread[physicalid] = newvalue;

	int o = physicalid * HISTORY_VALUES;
	// for(int i=HISTORY_VALUES - 1; i >= 0; i--) {
	// }
	// knobhistory2[o + 9] = knobhistory2[o + 8];
	// knobhistory2[o + 8] = knobhistory2[o + 7];
	// knobhistory2[o + 7] = knobhistory2[o + 6];
	// knobhistory2[o + 6] = knobhistory2[o + 5];
	// knobhistory2[o + 5] = knobhistory2[o + 4];
	knobhistory2[o + 4] = knobhistory2[o + 3];
	knobhistory2[o + 3] = knobhistory2[o + 2];
	knobhistory2[o + 2] = knobhistory2[o + 1];
	knobhistory2[o + 1] = knobhistory2[o + 0];
	knobhistory2[o + 0] = newvalue;

	if (knobreadcount[physicalid] < 100) {
		knobreadcount[physicalid] ++;
	}
}

void midistate_tick() {
	tick_counter ++;
	if (tick_counter < 10) {
		return;
	}

	tick_counter = 0;

	for(int i=0; i<MAX_PHYSICAL_CONTROLLERS; i++) {

		if (knobreadcount[i] < 10) {
			continue;
		}

		int o = i * HISTORY_VALUES;

		int newvalue = 0;
		newvalue += knobhistory2[o + 0];
		newvalue += knobhistory2[o + 1];
		newvalue += knobhistory2[o + 2];
		newvalue += knobhistory2[o + 3];
		newvalue += knobhistory2[o + 4];
		newvalue /= 5;

		if (newvalue < 0) {
			newvalue = 0;
		}

		if (newvalue > 127) {
			newvalue = 127;
		}

		uint16_t oldvalue = knoblastvalue[i];

		if (newvalue == oldvalue) {
			// Don't send if there is no change.
			continue;
		}

		// Remember new value
		knoblastvalue[i] = newvalue;

		if (oldvalue == 9999) {
			// Don't send initial value, just remember it.
			continue;
		}

		bankui_reportvalue(newvalue);

		knobdirty[i] = 1;
	}
}

void midistate_flush() {
    for(int i=0; i<MAX_PHYSICAL_CONTROLLERS; i++) {
        if (knobdirty[i]) {
			knobdirty[i] = 0;
			int channel = 1 + (current_bank >> 1);
			int controller = i + ((current_bank % 2) * 48);
			int value = knoblastvalue[i];
            int ok = usbmidi_sendcontrolchange(channel, controller, value);
			if (ok) {
				// return;
			}
        }
    }
}
