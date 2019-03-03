#ifndef _USBMIDI_H_
#define _USBMIDI_H_

void usbmidi_init();
void usbmidi_tick();
int usbmidi_sendcontrolchange(unsigned char channel, unsigned char controller, unsigned char value);

#endif
