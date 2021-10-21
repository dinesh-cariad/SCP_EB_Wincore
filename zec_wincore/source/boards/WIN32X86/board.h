/* board.h - for WIN32X86
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: board.h 25264 2016-11-28 08:23:48Z olme8414 $
*/

#ifndef BOARD_H
#define BOARD_H

#include <Os.h>
#include <Platform_Types.h>

/** \name Console Character Attributes
 *
 * The following macros were copied from wincon.h to avoid its inclusion
 * into this header file. These macros must match the corresponding ones
 * defined by Microsoft. Note, the prefix 'BOARD_' were added to avoid
 * name clashes.
 *
 * \see http://msdn.microsoft.com/en-us/library/windows/desktop/ms682088%28v=vs.85%29.aspx#_win32_character_attributes
 */
/** @{ */
#define BOARD_FOREGROUND_BLUE		1
#define BOARD_FOREGROUND_GREEN		2
#define BOARD_FOREGROUND_RED		4
#define BOARD_FOREGROUND_INTENSITY	8
#define BOARD_BACKGROUND_BLUE		16
#define BOARD_BACKGROUND_GREEN		32
#define BOARD_BACKGROUND_RED		64
#define BOARD_BACKGROUND_INTENSITY	128
/** @} */

/** \brief Makes the console's background color black. */
#define BOARD_FOREGROUND_WHITE (BOARD_FOREGROUND_BLUE | BOARD_FOREGROUND_GREEN | BOARD_FOREGROUND_RED)

/** \brief Enumerates the available background colors for consoles. */
#define BRDBGCLR_BLACK   BOARD_FOREGROUND_WHITE
#define BRDBGCLR_GREEN   (BOARD_BACKGROUND_INTENSITY | BOARD_BACKGROUND_GREEN | BOARD_FOREGROUND_WHITE)
#define BRDBGCLR_RED     (BOARD_BACKGROUND_INTENSITY | BOARD_BACKGROUND_RED | BOARD_FOREGROUND_WHITE)
#define BRDBGCLR_BLUE    (BOARD_BACKGROUND_INTENSITY | BOARD_BACKGROUND_BLUE | BOARD_FOREGROUND_WHITE)
#define BRDBGCLR_RANDOM  (BOARD_BACKGROUND_INTENSITY | BOARD_BACKGROUND_BLUE | BOARD_FOREGROUND_INTENSITY)

typedef uint8 board_consoleBgColor_t;

/** \brief The shared memory location to communicate the background color from the
 * Windows to the AUTOSAR world.
 *
 * The key reader thread uses this shared memory location to tell AUTOSAR runnables which
 * background color to use when refreshing the ASCII LEDs in the console.
 *
 * To avoid any communication errors between the Windows and AUTOSAR world one should
 * always follow this pattern. Use a Windows thread, which only calls
 * OS_WINDOWSTriggerInterrupt() and a shared memory location, which is read by a task
 * or ISR to transport data from Windows into the AUTOSAR world.
 */
extern board_consoleBgColor_t BOARD_consoleBgColor;

/** \brief Initializes the ASCII LED display. */
extern void LEDS_INIT(void);

/** \brief Displays the value \a x.
 *
 * \remarks The color of the ASCII LEDs is determined by BOARD_consoleBgColor.
 *
 * \param x		The counter value to be displayed. Only a change in the lower
 *				four bits lead to an update of the display.
 */
extern void LEDS_SET(os_uint_t val);

/** \name Platform time conversion functions.
 *
 * OS_BoardNsToTicks() converts nanoseconds (nS) to ticks. This macro must
 * compute entirely in the preprocessor, because it is used to initialize time
 * tables.
 *
 * OS_BoardTicksToNs() is the reverse computation.
 *
 * According to http://msdn.microsoft.com/en-us/library/windows/desktop/dd757627%28v=vs.85%29.aspx
 * and http://msdn.microsoft.com/en-us/library/windows/desktop/dd757625%28v=vs.85%29.aspx it
 * is assumed that Windows offers at most a time resolution of 1 millisecond. Hence, this is
 * taken as the base resolution. On certain platforms though it can be the case that this
 * resolution is not offered by Microsoft Windows. Use the "--help" command-line argument to find
 * out more about the timing capabilities of your platform and update the following conversion
 * macros accordingly, if your platform offers a higher timer resolution. Usually, one millisecond
 * is good enough for most use cases.
 *
 * One has to pay attention to overflows, because the timer API usually uses 32-bit types and
 * the conversion macros use the factor 1e6 internally. Hence, one can cover a range of
 * (int)(2**32 / 1e6) = 4294 milliseconds.
 */
/** @{ */
#define OS_BoardNsToTicks(nS)	((nS) / 1000000)
#define OS_BoardTicksToNs(tik)	((tik) * 1000000)
/** @} */

#include <WINDOWS/Os_WINDOWS_timer_config.h>
#include <WINDOWS/Os_WINDOWS_cpu.h>

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
