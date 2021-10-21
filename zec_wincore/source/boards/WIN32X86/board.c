/* board.c - for WINDOWS
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: board.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <WINDOWS/Os_WINDOWS_WIN32.h>
/* Undefine SetEvent(). See http://msdn.microsoft.com/en-us/library/windows/desktop/ms686211%28v=vs.85%29.aspx . */
#undef SetEvent

#include <board.h>
#include <Os.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#define BOARD_NUMBER_OF_RANDOM_COLORS 6

board_consoleBgColor_t BOARD_consoleBgColor = BRDBGCLR_RANDOM;

/** Sets the new background color of the (active) console.
 *
 * \param color		The background color to be used in the (active) console from now on.
 */
static void BOARD_SetConsoleBgColor(board_consoleBgColor_t color)
{
	static int randomColorsIndex = 0;
	static int randomColors[BOARD_NUMBER_OF_RANDOM_COLORS] =
	{
			/*BRDCLR_RED | BRDCLR_GREEN |*/ BRDBGCLR_BLUE,
			/*BRDCLR_RED |*/ BRDBGCLR_GREEN /*| BRDCLR_BLUE*/,
			/*BRDCLR_RED |*/ BRDBGCLR_GREEN | BRDBGCLR_BLUE,
			BRDBGCLR_RED /*| BRDCLR_GREEN | BRDCLR_BLUE*/,
			BRDBGCLR_RED /*| BRDCLR_GREEN*/ | BRDBGCLR_BLUE,
			BRDBGCLR_RED | BRDBGCLR_GREEN /*| BRDCLR_BLUE*/
	};

	/* ATTENTION!
	 * In the following block we use Windows services: SetConsoleTextAttribute() and
	 * GetStdHandle(). Hence we must notify AUTOSAR OS about this by surrounding the
	 * block with OS_WINDOWSGoingToUseWindowsService() and OS_WINDOWSFinishedUsingWindowsService().
	 */
	{
		os_intstatus_t is = OS_WINDOWSGoingToUseWindowsService();
		{
			int c = color;
			if (color == BRDBGCLR_RANDOM)
			{
				c = randomColors[randomColorsIndex];
				randomColorsIndex = (randomColorsIndex + 1) % BOARD_NUMBER_OF_RANDOM_COLORS;
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
		}
		OS_WINDOWSFinishedUsingWindowsService(is);
	}
}

/** \brief Restores the console's background color.
 *
 * This function is registered to run at application exit.
 * It sets the foreground color to white and the background color to black.
 *
 * \attention We must not use any AUTOSAR OS services directly nor indirectly,
 * because we're in the course of application termination. Violations might
 * cause abnormal program terminations.
 */
static void BOARD_RestoreConsoleBgColor(void)
{
	/* Use Windows service directly, because we're shutting down and AUTOSAR
	 * OS services are no longer available/usable.
	 */
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BRDBGCLR_BLACK);
}

void LEDS_INIT(void)
{
	os_intstatus_t is = OS_WINDOWSGoingToUseWindowsService();
	{
		printf("OS multi-core demo for Windows.\n"
				"Copyright Elektrobit Automotive GmbH\n"
				"\n"
				"You can change the color of the ASCII LEDs of the counter by pressing\n"
				"'r' for red, 'b' for blue, 'g' for green and 'c' for crazy. Any other key\n"
				"makes the background black again.\n"
				"\n"
				"\n");

		if (atexit(BOARD_RestoreConsoleBgColor) != 0)
		{
			printf("Warning: The foreground and background colors of the console are not restored\n"
					"at application exit.\n\n");
		}

		BOARD_SetConsoleBgColor(BRDBGCLR_BLACK);
		{
			os_size_t i;
			for (i = 0; i < CHAR_BIT; ++i)
			{
				printf(".");
			}
		}
		fflush(stdout);
	}
	OS_WINDOWSFinishedUsingWindowsService(is);
}

void LEDS_SET(os_uint_t val)
{
	static unsigned int previousVal = 0;

	if (previousVal != (val & 0xF))
	{
		/* ATTENTION!
		 * We use a Windows service in the following block: printf() and fflush().
		 * The AUTOSAR OS must be notified about this to adapt its scheduling.
		 * Failing to do so might lead to deadlocks.
		 */
		os_intstatus_t is = OS_WINDOWSGoingToUseWindowsService();
		{
			printf("\r");

			{
				unsigned int mask = 1 << (CHAR_BIT - 1);
				for (; mask != 0; mask >>= 1)
				{
					if ((mask & val) != 0)
					{
						BOARD_SetConsoleBgColor(BOARD_consoleBgColor);
						printf("O");
					}
					else
					{
						BOARD_SetConsoleBgColor(BRDBGCLR_BLACK);
						printf(".");
					}
					fflush(stdout);
				}
			}
		}
		OS_WINDOWSFinishedUsingWindowsService(is);

		previousVal = (val & 0xF);
	}
}

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
