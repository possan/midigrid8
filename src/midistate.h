#ifndef _MIDISTATE_H_
#define _MIDISTATE_H_

void midistate_init();

int midistate_getbank();
void midistate_setbank(int bank);

void midistate_report(int physicalid, int16_t value);

void midistate_tick();
void midistate_flush();

#endif
