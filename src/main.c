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

	uint8_t data = 0x00;

	// Infinite loop
	while (1)
	{
		while (SendToDisplay(data) == 0x00) {};
		while (SendToDisplay(((uint16_t)data) | (1 << DISP_STD_LEFT_CTRLR)) == 0x00) {};

		data ++;
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
