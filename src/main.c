#include <LUFA/Platform/Platform.h>

#include "inputmux.h"
#include "ads1015.h"
#include "midistate.h"
#include "scanner.h"
#include "usbmidi.h"

int main(void)
{
	bankui_init();

	bankui_flash1();

	midistate_init();
	ads1015_init();
	inputmux_init();
	scanner_init();

	midistate_flush();

	bankui_flash2();

	usbmidi_init();

	while(true)
	{
		scanner_tick();
		midistate_tick();
		midistate_flush();
		bankui_tick();

		_delay_us(10);

		usbmidi_tick();

	}
}
