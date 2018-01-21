#include "main.h"


// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"




int main(int argc, char* argv[])
{
	// Hardware initialization
	SwitchClockToHSE();
	SwitchClockToPLL();
//	ActivateLSEClock();

	InitializeDisplay();
	DisplayOnOff(0xFF);

	PushFramebuffer();

	uint8_t data = 0x00U;

	// Infinite loop
	while (1)
	{
		memset(Framebuffer, data, 1024);
		PushFramebuffer();
		data ++;

		uint32_t counter = 0;
		while (counter < 1000000)
		{
			counter ++;
		}
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
