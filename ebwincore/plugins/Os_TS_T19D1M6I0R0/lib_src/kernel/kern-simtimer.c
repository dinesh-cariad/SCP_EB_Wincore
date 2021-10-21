/* kern-simtimer.c - Hardware timer simulator for testing
 *
 * This file implements a driver for a simulated hardware timer.
 * The simulator implements two (or more) free-running timers in software, Each timer has
 * two (or more) compare registers that can be programmed to generate an "interrupt"
 * whenever the timer passes the value in the compare register.
 *
 * The difference between this timer and a real hardware timer is that the timer does not
 * automatically advance. It is advanced by calling the OS_AdvanceTimer() function from a
 * (trusted) task, ISR etc. Thus the timer can be controlled precisely by the test program,
 * thus permitting fine-grained control over the "timing" and therefore accurate testing
 * of the software that uses it.
 *
 * In addition to the 4 standard init/read/start/stop functions provided by this driver,
 * two additional functions are provided:
 *
 *  - OS_SimTimerSetup(tmr, maks, isr) sets up the timer.
 *  - OS_SimTimerAdvance(tmr, incr) advances the timer by incr ticks.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-simtimer.c 27799 2017-11-08 08:46:09Z mist9353 $
*/

#include <Os_api_simtimer.h>
#include <Os_kernel.h>
#include <Os_simtimer.h>
#include <Os_tool.h>

/* These are the timers. Each timer has a current value, two (or more) compare registers and
 * two control registers.
*/
typedef struct os_simtimer_s os_simtimer_t;
typedef struct os_simtimercmp_s os_simtimercmp_t;

struct os_simtimercmp_s
{
	os_timervalue_t	compare;
	os_isrid_t		isr;
	os_uint8_t		control;
};

struct os_simtimer_s
{
	os_timervalue_t		count;
	os_timervalue_t		mask;
	os_simtimercmp_t	channel[OS_SIMTIMER_NCOMPARE];
};

/* CHECK: NOPARSE */
static
OS_SECTION_PRAGMA(OS_simTimer, .bss.shared.os.OS_simTimer)
os_simtimer_t
	OS_SECTION_ATTRIB_PRE(.bss.shared.os.OS_simTimer)
	OS_simTimer[OS_SIMTIMER_NTIMER]
	OS_SECTION_ATTRIB_POST(.bss.shared.os.OS_simTimer);
/* CHECK: PARSE */

/* Bits in the "control" registers.
*/
#define OS_SIMTIMERCTRL_ENABLED		0x01u
#define OS_SIMTIMERCTRL_PENDING		0x02u

/* Prototypes of internal SimTimer functions */
static void OS_SimTimerInit(const os_hwt_t *td);
static os_timervalue_t OS_SimTimerRead(const os_hwt_t *td);
static os_boolean_t OS_SimTimerStart(const os_hwt_t *td, os_timervalue_t old, os_tick_t delta);
static void OS_SimTimerStop(const os_hwt_t *td);

/*
 * os_simtimerdescr_t
 *
 * For the timer simulator (since there is no real-time necessity) we do
 * not use the "descriptor" approach. Instead, access the timers and their
 * compare/control registers directly using the major/minor device numbers.
 * This also permits the number of timers and compare channels per timer to be
 * configured without having to construct a new descriptor initialiser.
*/

/* Driver table
 *
 * This table tells the kernel what the driver functions are
*/
#include <memmap/Os_mm_const_begin.h>
const os_hwtdrv_t OS_simTimerDriver =
{
	&OS_SimTimerInit,
	&OS_SimTimerRead,
	&OS_SimTimerStart,
	&OS_SimTimerStop
};
#include <memmap/Os_mm_const_end.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_SimTimerInit()
 *
 * Initialises the simulated timer channel specified in the descriptor.
 *
 * If the mask has not been previously initialised, we set it to ffff here
 * (the counter wraps from 65535 to zero).
*/
static void OS_SimTimerInit(const os_hwt_t *td)
{
	os_simtimer_t *tmr = &OS_simTimer[td->major];
	os_timervalue_t mask;

	if ( tmr->mask == 0 )
	{
		mask = td->wrapMask;
		tmr->mask = mask;
		if ( ( mask & (mask + 1) ) != 0 )
		{
			OS_ShutdownNoHooks();
		}

	}
	else
	if ( tmr->mask != td->wrapMask )
	{
		OS_ShutdownNoHooks();
	}
	else
	{
		/* MISRA */
	}

	tmr->channel[td->minor].compare = 0;
	tmr->channel[td->minor].control = 0;
}

/*!
 * OS_SimTimerRead()
 *
 * Reads the timer's counter and returns the result.
*/
static os_timervalue_t OS_SimTimerRead(const os_hwt_t *td)
{
	os_simtimer_t *tmr = &OS_simTimer[td->major];

	return tmr->count;
}

/*!
 * OS_SimTimerStart()
 *
 * Initialises and starts the simulated comparator as specified in the HW timer descriptor.
 *
 * The absolute value given by (old + delta) is stored into the comparator.
 * Since (we hope) the counter cannot advance without the software knowing about it,
 * there's no need to check for an "in the past" value.
 *
 * Return value is always FALSE because the time cannot be in the past.
*/
static os_boolean_t OS_SimTimerStart(const os_hwt_t *td, os_timervalue_t old, os_tick_t delta)
{
	os_simtimer_t *tmr = &OS_simTimer[td->major];

	tmr->channel[td->minor].compare = (old + delta) & tmr->mask;
	tmr->channel[td->minor].control |= OS_SIMTIMERCTRL_ENABLED;

	return OS_FALSE;
}

/*
 * OS_SimTimerStop()
 *
 * Disables the STM comparator as specified in the HW timer descriptor.
*/
static void OS_SimTimerStop(const os_hwt_t *td)
{
	os_simtimer_t *tmr = &OS_simTimer[td->major];

	tmr->channel[td->minor].control = 0;
}

/*
 * OS_SimTimerSetup()
 *
 * This function sets up a timer channel by clearing its compare and control registers
 * and setting its interrupt ID.
 *
 * Return values:
 * - OS_E_ID    - either the tmr index or the compare register number is out of range.
 * - OS_E_VALUE - the ISR id is invalid.
 * - OS_E_OK    - the timer has been successfully set up.
 * (The ErrorHook is not called)
*/
os_result_t OS_SimTimerSetup
(	os_unsigned_t tmr,		/* The index of the timer to set up */
	os_unsigned_t chan,		/* The index of the compare register */
	os_isrid_t isrId		/* The ISR-ID of the associated interrupt */
)
{
	os_result_t result = OS_E_OK;

	if ( tmr >= OS_SIMTIMER_NTIMER )
	{
		result = OS_E_ID;
	}
	else
	if ( chan >= OS_SIMTIMER_NCOMPARE )
	{
		result = OS_E_ID;
	}
	else
	if ( (isrId >= OS_nInterrupts) || ((OS_isrTableBase[isrId].flags & OS_ISRF_CAT) != OS_ISRF_CAT2) )
	{
		result = OS_E_VALUE;
	}
	else
	{
		OS_simTimer[tmr].channel[chan].isr = isrId + 1;
		OS_simTimer[tmr].channel[chan].compare = 0;
		OS_simTimer[tmr].channel[chan].control = 0;;
	}

	return result;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_SimTimerSetup</name>
  <synopsis>Set up a simulated timer channel</synopsis>
  <syntax>
    os_result_t OS_SimTimerSetup
    (   os_unsigned_t tmr,   /@ Index of the timer @/
        os_unsigned_t chan,  /@ Index of the compare register @/
        os_isrid_t isrId    /@ ISR-id of the associated interrupt @/
    )
  </syntax>
  <description>
    <code>OS_SimTimerSetup()</code> sets up a simulated timer channel by clearing its
	compare and control registers and setting its interrupts ID.
  </description>
  <availability>
    No restrictions.
  </availability>
  <returns>OS_E_OK=Success.</returns>
  <returns>OS_E_ID=Timer index is out of range.</returns>
  <returns>OS_E_ID=Compare register index is out of range.</returns>
  <returns>OS_E_VALUE=ISR-id is invalid.</returns>
</api>

 * CHECK: PARSE
*/

/*
 * OS_KernSimTimerAdvance()
*/
os_result_t OS_KernSimTimerAdvance(os_unsigned_t tmr, os_tick_t incr)
{
	os_result_t result = OS_E_OK;
	os_simtimer_t *tmrp;
	os_timervalue_t old;
	os_unsigned_t c;

	if ( tmr >= OS_SIMTIMER_NTIMER )
	{
		result = OS_E_ID;
	}
	else
	{
		tmrp = &OS_simTimer[tmr];

		if ( incr > tmrp->mask )
		{
				result = OS_E_VALUE;
		}
		else
		{
			/* This is where it happens. Advance the counter to the new value but
			 * remember the old value.
			*/
			old = tmrp->count;
			tmrp->count = (tmrp->count + incr) & tmrp->mask;

			for ( c = 0; c < OS_SIMTIMER_NCOMPARE; c++ )
			{
				if ( ( (tmrp->channel[c].control & OS_SIMTIMERCTRL_PENDING) != 0 ) ||
					 ( ((tmrp->channel[c].compare - old) & tmrp->mask) <= incr ) )
				{
					if ( (tmrp->channel[c].control & OS_SIMTIMERCTRL_ENABLED) != 0 )
					{
						if ( tmrp->channel[c].isr != 0 )
						{
							(*OS_isrTableBase[tmrp->channel[c].isr - 1].func)();
						}
						tmrp->channel[c].control &= (os_uint8_t) ~OS_SIMTIMERCTRL_PENDING;
					}
					else
					{
						tmrp->channel[c].control |= OS_SIMTIMERCTRL_PENDING;
					}
				}
			}
		}
	}

	return result;
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_SimTimerAdvance</name>
  <synopsis>Advances a simulated timer by a given value</synopsis>
  <syntax>
    os_result_t OS_SimTimerAdvance
    (   os_unsigned_t tmr,  /@ Index of the timer @/
        os_tick_t incr   /@ Value by which to increment the timer @/
    )
  </syntax>
  <description>
    <code>OS_SimTimerAdvance()</code> increments a simulated timer by the given value.
    It checks for each channel whether the timer is pending or passed the value programmed
    in its compare register. If the channel is enabled, it calls the respective associated
    ISR, otherwise the channel is set to pending.
  </description>
  <availability>
    No restrictions.
  </availability>
  <returns>OS_E_OK=Success.</returns>
  <returns>OS_E_ID=Timer index is out of range.</returns>
  <returns>OS_E_VALUE=Increment value is above the mask value of the timer.</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
