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

	ClearFramebuffer();
	PushFramebuffer();

	SetActiveColor(DISPLAY_PIXEL_ON_COLOR);

	// Infinite loop
	while (1)
	{
		PushFramebuffer();

		for (uint16_t y = 0; y < DISPLAY_Y_SIZE; y++)
		{
			for (uint16_t x = 0; x < DISPLAY_X_SIZE; x++)
			{
				DrawPixel(y, x);

				PushFramebuffer();
			}
		}

		SetActiveColor(DISPLAY_PIXEL_OFF_COLOR);
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
