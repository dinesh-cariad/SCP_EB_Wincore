/* WINLIN-logging.c
 *
 * This file contains the implementation of the logging facility.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-logging.c 27660 2017-10-18 14:07:36Z olme8414 $
 */

#include <Os_defs.h>
#if (OS_ARCH == OS_WINDOWS)
#include <WINDOWS/Os_WINDOWS_WIN32.h>
#endif

#include <Os_kernel.h>
#include <Os_api_arch.h>
#include <WINLIN/Os_WINLIN_critsec.h>
#include <WINLIN/Os_WINLIN_syscalls.h>
/* The function snprintf() is part of the C99 standard. Hence, we must define the following macro
 * to expose its prototype in case this code is compiled with the option "-std=iso9899:199409",
 * what means C90 code. This avoids warnings due to the option "-Wimplicit-function-declaration".
 */
#define _ISOC99_SOURCE 1
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,5,0)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"
#pragma GCC diagnostic ignored "-Wbad-function-cast"
#endif
#endif

/** \brief Determines, whether a log message passes the current log message filter.
 *
 * The current filter threshold level is stored in OS_winlinLogMessageFilterSeverity.
 * \a severity must be greater than or equal to this threshold to pass the filter.
 *
 * \param severity		The log message severity to be compared to the current filter threshold.
 *
 * \retval OS_FALSE	The log message is rejected.
 * \retval OS_TRUE	The log message may pass the filter.
 */
#define OS_WINLINLOGDoesPassFilter(severity) ((severity) >= OS_winlinLogMessageFilterSeverity)

#include <memmap/Os_mm_var_begin.h>

/** \brief Locks the console to prevent interleaved output.
 *
 * This lock is used by OS_WINLINDefaultLogFunction() to prevent multiple
 * PC-cores from printing simultaneously to the console.
 *
 * This lock may only be used, when the using thread is inside a region
 * embraced with OS_WINLINGoingToUseHostOsService() and
 * OS_WINLINFinishedUsingWindowsService().
 */
static os_winlinCriticalSection_t OS_winlinConsoleLock;

#include <memmap/Os_mm_var_end.h>

#include <memmap/Os_mm_code_begin.h>

/* MISRA-C requires a declaration. */
static void OS_WINLINPrintLinePrefix(os_char_t const*, os_int_t, FILE*);

/** \brief The default log function.
 *
 * This is the default logging function. It prints the formatted log message \a message to
 * stdout or stderr depending on its severity.
 *
 * \param severity			The severity of the log message. Only more or equally severe log
 *							messages can pass the filter, of which the threshold defined by the
 *							current value of OS_winlinLogMessageFilterSeverity.
 *							Less severe log messages are discarded.
 * \param filename			The name of the file to which the log message pertains to.
 *							Specify OS_NULL if there is no such file.
 * \param lineNumber		The line number in the file \a filename. If \a filename is
 *							OS_NULL this parameter is not regarded.
 * \param message			The actual log message in
 *							<a href="http://cplusplus.com/reference/cstdio/printf/">printf()</a>-
 *							style
 */
void OS_WINLINDefaultLogFunction(
		os_winlinLogMessageSeverity_t severity,
		os_char_t const* filename,
		os_int_t lineNumber,
		os_char_t const* message,
		...)
{
	if (OS_WINLINLOGDoesPassFilter(severity))
	{
		os_intstatus_t is = OS_WINLINGoingToUseHostOsService();
		{
			va_list argList;
			os_char_t const* const startOfFilename = OS_WINLINGetStartOfFilename(filename);
			FILE* stream = stdout;

			if (severity == OS_WINLIN_LOG_ERROR)
			{
				stream = stderr;
			}

			OS_WINLIN_OSAL_EnterCriticalSection(&OS_winlinConsoleLock);
			OS_WINLINPrintLinePrefix(startOfFilename, lineNumber, stream);

			va_start(argList, message);
			(void)vfprintf(stream, message, argList);
			va_end(argList);

			(void)fflush(stream);	/* Make sure that output is visible in the console. */
			OS_WINLIN_OSAL_LeaveCriticalSection(&OS_winlinConsoleLock);
		}
		OS_WINLINFinishedUsingHostOsService(is);
	}
}

/** \brief Initializes the default logging facility.
 *
 * \remarks There is no equivalent function to destroy the resources initialized
 * by this function, because it would be called at application termination only
 * and then all of a process' resources are cleaned up by windows anyway.
 */
void OS_WINLINInitializeLogging(void)
{
	OS_WINLIN_OSAL_InitializeCriticalSection(&OS_winlinConsoleLock);
}

/** \brief Logs which ISRs are observed by a core thread.
 *
 * \param[in] associatedIsrs	The array of ISRs currently observed by a core thread as
 *								returned by OS_WINLINAssembleCoreThreadEvents().
 * \param[in] length			The number of entries in \a associatedIsrs.
 */
void OS_WINLINLogObservedIsrs(os_isr_t const* const associatedIsrs[], os_size_t length)
{
	os_char_t str[256/*max number of ISRs*/
	              * (3/*max digits per ISR*/
	                 + 1/*one separator os_char_t per ISR*/)
	              + 1/*one start separator*/
	              + 1/*for '\0'*/];
	os_char_t* strIter = &str[0];

	if (OS_WINLINLOGDoesPassFilter(OS_WINLIN_LOG_DEBUG))
	{
		if ((length != 0) && (associatedIsrs[0] != OS_NULL)) /* then, at least one ISR event is
														      * observed */
		{
			os_size_t numTotalWritten = 0;

			*strIter = '|'; /* prepend the start separator */
			++strIter;

			{
				os_size_t i;
				for (i = 0; i < length; ++i)
				{
					if (associatedIsrs[i] != OS_NULL)
					{
						os_int_t numWritten = snprintf(
								strIter,
								(os_uint_t)(sizeof(str) - numTotalWritten),
								"%u|",
								(os_uint_t)associatedIsrs[i]->isrId);
						OS_WINLINAssert(numWritten >= 0); /* Array str is too small, if fails. */
						strIter += numWritten;
						numTotalWritten += (os_size_t)numWritten;
					}
				}
			}

			str[sizeof(str) - 1U] = '\0'; /* Just to be sure. */

			OS_WINLINLog(
					OS_WINLIN_LOG_DEBUG,
					"Core %u observes ISR IDs: %s",
					OS_GetMyCoreId(),
					str);
		}
		else /* ... no ISRs can be triggered at present. */
		{
			OS_WINLINLog(
					OS_WINLIN_LOG_DEBUG,
					"Core %u observes no ISRs.",
					OS_GetMyCoreId());
		}
	}
}

/** \brief Logs that a core thread is going to make a syscall.
 *
 * \param[in] pSyscallPort		The syscall port used to make the syscall.
 */
void OS_WINLINLogSyscall(os_syscallport_t const* pSyscallPort)
{
	if (OS_WINLINLOGDoesPassFilter(OS_WINLIN_LOG_DEBUG))
	{
		os_uint8_t const inFunction = OS_GetKernelData()->inFunction;

		if (OS_WINLINGetSyscallBuffer(pSyscallPort)->syscallId == OS_SC_WINLINFinishedIrq)
		{
			os_isr_t const* const pIsr =
					(os_isr_t const*)OS_WINLINGetSyscallBuffer(pSyscallPort)->p1;
			OS_WINLINLog(
					OS_WINLIN_LOG_DEBUG,
					"ISR %s is going to make a syscall: syscallId = %u.",
					OS_WINLINGetNameOfIsr(pIsr),
					OS_WINLINGetSyscallBuffer(pSyscallPort)->syscallId);
		}
		else if (((OS_INHOOKMIN <= inFunction) && (inFunction <= OS_INHOOKMAX))
				 || (inFunction == OS_INACB))
		{
			OS_WINLINLog(
					OS_WINLIN_LOG_DEBUG,
					"A hook function (inFunction = %u) is going to make a "
					"syscall: syscallId = %u.",
					inFunction,
					OS_WINLINGetSyscallBuffer(pSyscallPort)->syscallId);
		}
		else if ((OS_INCAT1 == inFunction) || (OS_INCAT2 == inFunction))
		{
			os_isrid_t const isrId = OS_GetKernelData()->isrCurrent;
			os_isr_t const* const pIsr = &OS_isrTableBase[isrId];
			OS_WINLINLog(
					OS_WINLIN_LOG_DEBUG,
					"ISR %s is going to make a syscall: syscallId = %u.",
					OS_WINLINGetNameOfIsr(pIsr),
					OS_WINLINGetSyscallBuffer(pSyscallPort)->syscallId);
		}
		else if (OS_INTASK == inFunction)
		{
			os_task_t const* const pTask = OS_GetKernelData()->taskCurrent;
			OS_WINLINLog(
					OS_WINLIN_LOG_DEBUG,
					"Task %s is going to make a syscall: syscallId = %u.",
					OS_WINLINGetNameOfTask(pTask),
					OS_WINLINGetSyscallBuffer(pSyscallPort)->syscallId);
		}
		else
		{
			OS_WINLINLog(
					OS_WINLIN_LOG_DEBUG,
					"Internal component (inFunction = %u) is going to make "
					"a syscall: syscallId = %u.",
					inFunction,
					OS_WINLINGetSyscallBuffer(pSyscallPort)->syscallId);
		}
	}
}

/** \brief Prints the prefix of a log message to the given file stream.
 *
 * Each log message starts with the prefix printed by this function to \a stream.
 *
 * \param[in] startOfFilename	The name of the file the log message pertains to.
 * \param[in] lineNumber		The number of the line in \a startOfFilename.
 * \param[in,out] stream		The file stream where to print the prefix to.
 */
static void OS_WINLINPrintLinePrefix(
		os_char_t const* startOfFilename,
		os_int_t lineNumber,
		FILE* stream)
{
	os_winlinTid_t const myTid = OS_WINLIN_OSAL_GetCurrentThreadId();

	if (startOfFilename != OS_NULL)
	{
		(void)fprintf(stream,
				"\n%#6lx=%-5ld %2u %3u %s[%d]: ",
				myTid,
				myTid,
				OS_GetMyCoreId(),
				((OS_WINLINIsThisAnExternalThread())
						? 0xff
						: OS_WINLINGetCoreInterruptDisableLevel()),
				startOfFilename,
				lineNumber);
	}
	else
	{
		(void)fprintf(stream,
				"\n%#6lx=%-5ld %2u %3u: ",
				myTid,
				myTid,
				OS_GetMyCoreId(),
				((OS_WINLINIsThisAnExternalThread())
						? 0xff
						: OS_WINLINGetCoreInterruptDisableLevel()));
	}
}
#include <memmap/Os_mm_code_end.h>

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,5,0)
#pragma GCC diagnostic pop
#endif
#endif

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
