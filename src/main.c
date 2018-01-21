#include "main.h"


// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

void DrawNextPosition(void)
{
	// Clearing old
	SetActiveColor(DISPLAY_PIXEL_OFF_COLOR);
	DrawPixel(Y, X);

	// Computing new position
	SetActiveColor(DISPLAY_PIXEL_ON_COLOR);

	Y = Y + YSpeed;
	X = X + XSpeed;

	if ((Y <= 1) || (Y >= DISPLAY_Y_SIZE - 1))
	{
		YSpeed *= -1;
		Y = Y + 2 * YSpeed;
	}

	if ((X <= 1) || (X >= DISPLAY_X_SIZE - 1))
	{
		XSpeed *= -1;
		X = X + 2 * XSpeed;
	}

	DrawPixel(Y, X);

	PushFramebuffer();
}


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

	// Draw borders
	for (uint8_t x = 0; x < DISPLAY_X_SIZE; x++)
	{
		DrawPixel(0, x);
		DrawPixel(DISPLAY_Y_SIZE - 1, x);
	}

	for (uint8_t y = 0; y < DISPLAY_X_SIZE; y++)
	{
		DrawPixel(y, 0);
		DrawPixel(y, DISPLAY_X_SIZE - 1);
	}

	PushFramebuffer();

	X = 64;
	Y = 32;

	XSpeed = 0.3;
	YSpeed = 0.4;

	// Infinite loop
	while (1)
	{
		DrawNextPosition();

		uint32_t counter = 500000;
		while (counter > 0)
		{
			counter --;
		}
	}
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
