#ifndef _MIDISTATE_H_
#define _MIDISTATE_H_

void midistate_init();
void midistate_report(int controller, int value);
void midistate_flush();

#endif
