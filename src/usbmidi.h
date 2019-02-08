#ifndef _USBMIDI_H_
#define _USBMIDI_H_

void usbmidi_init();
void usbmidi_tick();
void usbmidi_sendcontrolchange(int channel, int controller, int value);

#endif
