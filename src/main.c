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

	DisplaySetStartLine(10);

	SendToDisplayWithWait(0x00 | BV(DISP_STD_LEFT_CTRLR));
	SendToDisplayWithWait(0x00);

	// Infinite loop
	while (1)
	{
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
