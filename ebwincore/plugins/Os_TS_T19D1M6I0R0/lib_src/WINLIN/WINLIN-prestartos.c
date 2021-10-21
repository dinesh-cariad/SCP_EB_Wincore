/* WINLIN-prestartos.c
 *
 * This file contains the OS_WINLINPreStartOs() function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-prestartos.c 31511 2020-03-02 12:18:05Z arla271030 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: PrintfFormatIdNotSupported
 *   ISO C does not support language feature
 *
 * Reason:
 *  Not an issue, a constant is defined along with 'I64' ms_printf
 *  length modifier and used in a printf statement to avoid warning
 *  because of -Woverlength-strings compiler option.
*/

#include <Os_kernel.h>
#include <Os_tool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <WINLIN/Os_WINLIN_timer.h>
#include <WINLIN/Os_WINLIN_cpu.h>
#include <Os_api_arch.h>
#if (OS_N_CORES > 1)
#include <Os_spinlock.h>
#endif

/* Declarations just for MISRA. */
static void OS_WINLINPrintUsage(os_char_t const*);
static void OS_WINLINParseCommandLine(os_int_t, os_char_t const * const * argv);
static void OS_WINLINInitializeCoreThreadIds(void);
static void OS_WINLINInitializesTlsIndices(void);
static void OS_WINLINBecomeInitCoreThread(void);
#if (OS_ARCH == OS_WINDOWS)
static void OS_WINDOWSTerminateProcessInTheEnd(void);
#endif
#if (OS_N_CORES > 1)
static void OS_WINLINInitializeSpinlocks(void);
#endif

#include <memmap/Os_mm_code_begin.h>

#if (OS_ARCH == OS_WINDOWS)
/** \brief Terminates the whole process in the end.
 *
 * It was observed, that sometimes the process termination does not work reliably:
 * the whole process gets stuck in the C library's exit() function (or in methods called by it).
 * This is especially true on WinXP. Windows7 suffers from the same problem, but
 * there TerminateProcess() works quite well to terminate the process. On WinXP
 * even this brutal method does not work sometimes.
 *
 * Therefore, the function OS_WINDOWSTerminateProcessInTheEnd() is registered with the process
 * termination mechanism and gets called, when exit() is called. To ensure, that this is the
 * last callback invoked by this mechanism, it is registered prior to any other callbacks.
 */
static void OS_WINDOWSTerminateProcessInTheEnd(void)
{
	OS_WINLIN_OSAL_TerminateCurrentProcess(0);
}
#endif

/** \brief Prints usage information to stdout.
 *
 * \param[in] programPath		The path of the program.
 */
static void OS_WINLINPrintUsage(os_char_t const* programPath)
{
	os_char_t const* const progInfo =
			"Usage: %s [-h | --help]\n"
			"           [--log-level <log level>]\n"
			"           [--time-warp-factor <factor>]\n";

	os_char_t const* const helpOptInfo =
			"\n"
			"-h, --help\n"
			"    Show this message.\n";

	os_char_t const* const logLevelOptInfo =
			"\n"
			"--log-level <log level>\n"
			"    Set the log message filter threshold to <log level>\n"
			"    so that less severe messages can't pass the filter.\n"
			"    Valid levels are: DEBUG, INFO, WARNING, ERROR or DISABLE.\n"
			"    The level DISABLE switches logging effectively off.\n"
			"    The default level is WARNING.\n";

	/* Note, the following string timeWarpFactorOptInfoX is split up to avoid the following warning:
	 * "string length '844' is greater than the length '509' ISO C90 compilers are required to support
	 * [-Woverlength-strings]".
	 */
	 os_char_t const* const timeWarpFactorOptInfo0 =
			"\n"
			"--time-warp-factor <factor>\n"
			"    Normally, the timing constraints seen in embedded software are not feasible\n"
			"    on PCs. To avoid having to change an AUTOSAR application and adjust all\n"
			"    timing constraints according to Windows' capabilities, one can specify a warp\n"
			"    factor. The time as perceived by an AUTOSAR application is slowed down or warped\n"
			"    proportionally by the given factor. Hence, a factor of n means that x nanoseconds\n";

	/* Possible diagnostic TOOLDIAG-1 */
	os_char_t const* const timeWarpFactorOptInfo1 = 
			"    in the AUTOSAR world represent x * n nanoseconds in the Windows world. This time\n"
			"    time warping is transparent to an AUTOSAR application. <factor> must be >= 1.\n"
			"    The currently used value is %" PRId64 ", which is the value configured via EB tresos\n"
			"    Studio. Use the value 1, if you don't want time warping at all. Warp factors\n"
			"    specified on the command-line take precedence over configured ones.\n";

	os_char_t const* const timerInfo =
			"\n"
			"Please note that the hardware timer resolution is %u millisecond(s).\n"
			"All AUTOSAR timers TIMERx work with this resolution. Please adjust your timing\n"
			"constraints accordingly and/or use the time warp factor.\n";

	os_char_t const* const programName = OS_WINLINGetStartOfFilename(programPath);

	(void)printf(progInfo, (programName == OS_NULL ? "<program name>" : programName));
	(void)printf(helpOptInfo);
	(void)printf(logLevelOptInfo);
	(void)printf(timeWarpFactorOptInfo0);
	(void)printf(timeWarpFactorOptInfo1, OS_winlinTimeWarpFactor);
	(void)printf(timerInfo, OS_WINLIN_OSAL_GetMinimumPeriod());

	if (OS_WINLIN_OSAL_IsPerformanceCounterAvailable())
	{
		(void)printf(
				"\n"
				"The time-stamp timer INTERNAL is available.\n"
				"It is clocked with a frequency of %" PRId64 "Hz.\n",
				OS_WINLIN_OSAL_QueryPerformanceFrequency());
	}
	else
	{
		(void)printf(
				"\n"
				"The time-stamp timer INTERNAL is not available.\n"
				"Please use one of the other timers TIMERx with x out of [0,3].\n");
	}

#if (OS_ARCH == OS_LINUX)
	{
		os_int_t minRealTimePrio, maxRealTimePrio, numRealTimePrios;

		OS_LINUXGetSchedulingPriorityRange(&minRealTimePrio, &maxRealTimePrio);

		numRealTimePrios = maxRealTimePrio - minRealTimePrio + 1;
		if (numRealTimePrios < OS_LINUX_NUMBER_REAL_TIME_PRIOS_REQUIRED)
		{
			(void)printf(
					"\n"
					"The host OS does not offer the required number of real-time priorities.\n"
					"Only %d real-time priorities are supported, but %d are needed.\n"
					"Hence, the option --real-time-priority is not usable.\n",
					numRealTimePrios,
					OS_LINUX_NUMBER_REAL_TIME_PRIOS_REQUIRED);
		}
		else
		{
			maxRealTimePrio = maxRealTimePrio - OS_LINUX_NUMBER_REAL_TIME_PRIOS_REQUIRED + 1;
			(void)printf(
					"\n"
					"The real-time priority for the option --real-time-priority may be in the range [%d, %d].\n",
					minRealTimePrio,
					maxRealTimePrio);
		}
	}
#endif
}

/** \brief Parses the command-line arguments.
 *
 * \param[in] argc		The number of command-line arguments passed to the program.
 *						If no arguments were passed to this program, i.e. \a argc <= 1,
 *						default values are assumed. Otherwise the given values overwrite
 *						the defaults.
 * \param[in] argv		The actual command-line arguments.
 */
static void OS_WINLINParseCommandLine(os_int_t argc, os_char_t const * const * argv)
{
	os_int_t i;
	for (i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "--log-level") == 0)
		{
			if ((i + 1) < argc)
			{
				if (strcmp(argv[i + 1], "DEBUG") == 0)
				{
					OS_winlinLogMessageFilterSeverity = OS_WINLIN_LOG_DEBUG;
				}
				else if (strcmp(argv[i + 1], "INFO") == 0)
				{
					OS_winlinLogMessageFilterSeverity = OS_WINLIN_LOG_INFO;
				}
				else if (strcmp(argv[i + 1], "WARNING") == 0)
				{
					OS_winlinLogMessageFilterSeverity = OS_WINLIN_LOG_WARNING;
				}
				else if (strcmp(argv[i + 1], "ERROR") == 0)
				{
					OS_winlinLogMessageFilterSeverity = OS_WINLIN_LOG_ERROR;
				}
				else if (strcmp(argv[i + 1], "DISABLE") == 0)
				{
					OS_winlinLogMessageFilterSeverity = OS_WINLIN_LOG_DISABLE;
				}
				else
				{
					(void)fprintf(
							stderr,
							"The argument '%s' is not a valid threshold level to filter log messages.\n",
							argv[i + 1]);
					exit(EXIT_FAILURE);
				}
				/* Skip the next command-line argument, because it can't be a valid option name;
				 * it's the debug level just parsed.
				 */
				++i;
			}
			else
			{
				(void)fprintf(stderr, "Please specify a log level after --log-level.\n");
				exit(EXIT_FAILURE);
			}
		}
		else if (strcmp(argv[i], "--time-warp-factor") == 0)
		{
			if ((i + 1) < argc)
			{
				OS_winlinTimeWarpFactor = atoi(argv[i + 1]);
				if (OS_winlinTimeWarpFactor == 0)
				{
					(void)fprintf(
							stderr,
							"The argument '%s' is not a valid time warp factor.\n"
							"Please specify an integer number >= 1.\n",
							argv[i + 1]);
					exit(EXIT_FAILURE);
				}
				/* Skip the next command-line argument, because it can't be a valid option name.
				 * It's the time warp factor to use.
				 */
				++i;
			}
			else
			{
				(void)fprintf(stderr, "Please specify a warp factor after --time-warp-factor.\n");
				exit(EXIT_FAILURE);
			}
		}
#if (OS_ARCH == OS_LINUX)
		else if (strcmp(argv[i], "--real-time-priority") == 0)
		{
			if ((i + 1) < argc)
			{
				os_int_t minRealTimePrio, maxRealTimePrio, numRealTimePrios;

				OS_LINUXGetSchedulingPriorityRange(&minRealTimePrio, &maxRealTimePrio);

				numRealTimePrios = maxRealTimePrio - minRealTimePrio + 1;
				if (numRealTimePrios < OS_LINUX_NUMBER_REAL_TIME_PRIOS_REQUIRED)
				{
					(void)fprintf(
							stderr,
							"This program needs %d real-time priorities for its operations, "
							"but only %d are supported by the host OS.\n"
							"Hence, the option --real-time-priority is not usable.\n",
							OS_LINUX_NUMBER_REAL_TIME_PRIOS_REQUIRED,
							numRealTimePrios);
					exit(EXIT_FAILURE);
				}

				maxRealTimePrio = maxRealTimePrio - OS_LINUX_NUMBER_REAL_TIME_PRIOS_REQUIRED + 1;
				OS_linuxRealTimePriority = atoi(argv[i + 1]);
				if (!((minRealTimePrio <= OS_linuxRealTimePriority) && (OS_linuxRealTimePriority <= maxRealTimePrio)))
				{
					(void)fprintf(
							stderr,
							"Please specify a real-time priority in the range [%d, %d].\n",
							minRealTimePrio,
							maxRealTimePrio);
					exit(EXIT_FAILURE);
				}
				/* Skip the next command-line argument, because it can't be a valid option name.
				 * It's the real-time priority to use.
				 */
				++i;
			}
			else
			{
				(void)fprintf(stderr, "Please specify a real-time priority after --real-time-priority.\n");
				exit(EXIT_FAILURE);
			}
		}
#endif /* OS_ARCH == OS_LINUX */
		else if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0))
		{
			OS_WINLINPrintUsage(argv[0]);
			exit(EXIT_SUCCESS);
		}
		else
		{
			(void)fprintf(
					stderr,
					"Unknown command-line argument '%s' found.\n",
					argv[i]);
			exit(EXIT_FAILURE);
		}
	}
}

/** \brief Initializes the core thread IDs (TID) for all cores.
 *
 * We use the special value 0 to indicate, that a core thread hasn't
 * started yet. This value is use to initialize the variables representing
 * the core thread IDs.
 */
static void OS_WINLINInitializeCoreThreadIds(void)
{
	os_coreid_t coreIdIter;
	for (coreIdIter = 0; coreIdIter < OS_N_CORES_MAX; ++coreIdIter)
	{
		if (OS_WINLINIsOsUsedOnCore(coreIdIter))
		{
			OS_WINLINSetCoreThreadIdOfCore(coreIdIter, 0);
		}
	}
}

/** \brief Prepares the thread local storage slots.
 *
 * Allocates slots in thread local storage. These slots are used to store
 * -	a thread's (AUTOSAR) core ID,
 * -	the pointer to its entry in OS_kernelArray[],
 * -	the pointer to its system call buffer and
 * -	a task or ISR thread's sync-spot.
 */
static void OS_WINLINInitializesTlsIndices(void)
{
	OS_winlinTlsCoreId		= OS_WINLIN_OSAL_TlsAlloc();
	OS_winlinTlsKernPtr		= OS_WINLIN_OSAL_TlsAlloc();
	OS_winlinTlsSyscallPort	= OS_WINLIN_OSAL_TlsAlloc();
#if OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
	OS_linuxTlsSyncSpot		= OS_WINLIN_OSAL_TlsAlloc();
#endif
}

/** \brief Turns the caller into the core thread of the init-core.
 *
 * The so called init-core is responsible for system initialization. This duty
 * naturally devolves to the application's main thread, i.e. the one, which is created
 * by the host OS when the application is started.
 */
static void OS_WINLINBecomeInitCoreThread(void)
{
	OS_WINLINInitializeCoreThread(OS_INITCORE);
	OS_WINLIN_OSAL_MakeThisThreadVeryImportant();
#if (OS_ARCH == OS_LINUX)
	{
		os_char_t threadName[4/*for 'core'*/ + 3/*for 3 digits*/ + 1/*for '\0'*/];
		(void)snprintf(&threadName[0], sizeof(threadName), "core%u", OS_INITCORE);
		OS_LINUXSetNameOfCurrentThread(&threadName[0]);
	}
#endif
}

#if (OS_N_CORES > 1)
/** \brief Initializes all spinlocks in the system. */
static void OS_WINLINInitializeSpinlocks(void)
{
	os_uint_t i;
	for (i = 0; i < OS_nSoftwareLocks; ++i)
	{
		OS_WINLIN_OSAL_InitSpinlock(&OS_softwareLock[i]);
	}
}
#endif

/** \brief Initializes the environment to run AutoCore OS on WINDOWS.
 *
 * This function sets up the global environment necessary to call any OS-function
 * including StartCore() and StartOS().
 *
 * It will effectively promote the calling thread to be the core-thread of the
 * init-core.
 *
 * This function will silently ignore multiple invocations, so it is possible to
 * execute the same main()-function on every core.
 *
 * \attention This function must be called right at the entry into main(),
 * before any other function is called.
 *
 * \param[in] argc		The number of command-line arguments passed to the program.
 * \param[in] argv		The actual command-line arguments.
 */
void OS_WINLINPreStartOs(os_int_t argc, os_char_t **argv)
{
	if (OS_WINLINIsInitialized() == OS_FALSE)	/* then, we've not yet been here ... */
	{
		OS_winlinArgc = argc;
		OS_winlinArgv = argv;

		OS_WINLIN_OSAL_Init();
#if (OS_ARCH == OS_WINDOWS)
		OS_WINLIN_OSAL_AtExit(&OS_WINDOWSTerminateProcessInTheEnd);
#endif
#if (OS_N_CORES > 1)
		OS_WINLINInitializeSpinlocks();
#endif
		OS_WINLINInitializeLogging();

		OS_WINLINLog(
				OS_WINLIN_LOG_DEBUG,
				"Going to initialize the environment.",
				OS_WINLINLOG_UNUSED_PARAM);

		OS_WINLIN_OSAL_DisablePriorityBoostOfThisProcess();

		OS_WINLINParseCommandLine(argc, (os_char_t const * const *)argv);

		OS_WINLINInitializeCoreThreadIds();
		OS_WINLINInitializesTlsIndices();
		OS_WINLINBecomeInitCoreThread();

		OS_FrcGlobalInit();
		OS_WINLINPreDynamicStopInit();

		OS_WINLINSignalEndOfInitialization();
	}
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
