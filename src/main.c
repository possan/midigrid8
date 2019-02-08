#include <LUFA/Platform/Platform.h>

#include "inputmux.h"
#include "ads1015.h"
#include "midistate.h"
#include "scanner.h"
#include "usbmidi.h"

int main(void)
{
	midistate_init();
	ads1015_init();
	inputmux_init();
	scanner_init();
	usbmidi_init();

	while(true)
	{
		scanner_tick();
		usbmidi_tick();
	}
}
