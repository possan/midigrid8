#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>

#include <LUFA/Drivers/Board/LEDs.h>
#include <LUFA/Drivers/Board/Joystick.h>
#include <LUFA/Drivers/Board/Buttons.h>
#include <LUFA/Drivers/USB/USB.h>
#include <LUFA/Platform/Platform.h>

#include "descriptors.h"
#include "usbmidi.h"

USB_ClassInfo_MIDI_Device_t Keyboard_MIDI_Interface =
{
	.Config =
	{
		.StreamingInterfaceNumber = INTERFACE_ID_AudioStream,
		.DataINEndpoint =
		{
			.Address = MIDI_STREAM_IN_EPADDR,
			.Size = MIDI_STREAM_EPSIZE,
			.Banks = 1,
		},
		.DataOUTEndpoint =
		{
			.Address = MIDI_STREAM_OUT_EPADDR,
			.Size = MIDI_STREAM_EPSIZE,
			.Banks = 1,
		},
	},
};

void EVENT_USB_Device_Connect(void) {}

void EVENT_USB_Device_Disconnect(void) {}

void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;
	ConfigSuccess &= MIDI_Device_ConfigureEndpoints(&Keyboard_MIDI_Interface);
}

void EVENT_USB_Device_ControlRequest(void)
{
	MIDI_Device_ProcessControlRequest(&Keyboard_MIDI_Interface);
}




// int needs_flush = 0;

void usbmidi_init()
{
	MCUSR &= ~(1 << WDRF);
	wdt_disable();
	clock_prescale_set(clock_div_1);
	USB_Init();
	GlobalInterruptEnable();
}

void usbmidi_tick()
{
	// if (needs_flush) {
	// 	int ret = MIDI_Device_Flush(&Keyboard_MIDI_Interface);
	// 	if (ret == ENDPOINT_READYWAIT_NoError) {
	// 		needs_flush = 0;
	// 	}
	// }

	MIDI_EventPacket_t ReceivedMIDIEvent;
	while (MIDI_Device_ReceiveEventPacket(&Keyboard_MIDI_Interface, &ReceivedMIDIEvent))
	{
		// if ((ReceivedMIDIEvent.Event == MIDI_EVENT(0, MIDI_COMMAND_NOTE_ON)) && (ReceivedMIDIEvent.Data3 > 0))
		//   LEDs_SetAllLEDs(ReceivedMIDIEvent.Data2 > 64 ? LEDS_LED1 : LEDS_LED2);
		// else
		//   LEDs_SetAllLEDs(LEDS_NO_LEDS);
	}

	MIDI_Device_USBTask(&Keyboard_MIDI_Interface);

	USB_USBTask();
}

int usbmidi_sendcontrolchange(unsigned char channel, unsigned char controller, unsigned char value)
{
	uint8_t MIDICommand = MIDI_COMMAND_CONTROL_CHANGE;
	uint8_t Channel = MIDI_CHANNEL(channel);

	MIDI_EventPacket_t MIDIEvent = (MIDI_EventPacket_t) {
		.Event       = MIDI_EVENT(0, MIDICommand),
		.Data1       = MIDICommand | Channel,
		.Data2       = controller,
		.Data3       = value,
	};

	int ret = MIDI_Device_SendEventPacket(&Keyboard_MIDI_Interface, &MIDIEvent);

	if (ret == ENDPOINT_RWSTREAM_NoError) {
		// needs_flush = 1;
		return 1;
	}

	return 0;
}
