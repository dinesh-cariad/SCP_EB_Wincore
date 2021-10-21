/* WINLIN-startcore.c
 *
 * This file contains the implementation of the StartCore() API.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: WINLIN-startcore.c 27660 2017-10-18 14:07:36Z olme8414 $
 */

#include <Os_kernel.h>
/* The function snprintf() is part of the C99 standard. Hence, we must define the following macro
 * to expose its prototype in case this code is compiled with the option "-std=iso9899:199409",
 * what means C90 code. This avoids warnings due to the option "-Wimplicit-function-declaration".
 */
#define _ISOC99_SOURCE 1
#include <stdio.h>

#include <memmap/Os_mm_code_begin.h>

/* MISRA-C requires a declaration. */
extern void main(os_int_t, char* argv[]);
#if (OS_ARCH == OS_WINDOWS)
static os_winlinDword_t OS_WINDOWS_STDCALL OS_WINLINCoreThread(void*);
#elif (OS_ARCH == OS_LINUX)
static void* OS_WINLINCoreThread(void*);
#else
#error "Unsupported architecture. Please check your makefiles."
#endif

/** \brief Starts the given core.
 *
 * \param coreId		The ID of the core to be started.
 */
void OS_ArchStartCore(os_coreid_t coreId)
{
	OS_WINLINLog(
			OS_WINLIN_LOG_DEBUG,
			"Going to start core %u.",
			coreId);

	{
		os_char_t threadName[4/*for 'core'*/ + 3/*for 3 digits*/ + 1/*for '\0'*/];
		os_winlincorethreadparams_t* params = &OS_winlinCoreThreadParams[coreId];

		params->coreId = coreId;
		(void)snprintf(&threadName[0], (os_uint_t)sizeof(threadName), "core%u", (os_int_t)coreId);

		(void)OS_WINLIN_OSAL_CreateThread(
				&OS_WINLINCoreThread,
				params,
				OS_NULL,
				&threadName[0],
				OS_WINLIN_THREAD_TYPE_CORE);

		/* Note, we don't have to wait until 'coreThread' is up and running, because
		 * the core threads are synchronized anyway by StartOS().
		 */
	}
}

/** \brief The bootstrap function of a core thread.
 *
 * All cores except the master core are represented by a dedicated thread.
 * The master core uses the main application thread instead.
 *
 * \param args		Points to an instance of os_winlincorethreadparams_t, which
 *					contains the parameters to start a core thread.
 *
 * \return Always returns OS_NULL.
 */
#if (OS_ARCH == OS_WINDOWS)
static os_winlinDword_t OS_WINDOWS_STDCALL OS_WINLINCoreThread(void* args)
#elif (OS_ARCH == OS_LINUX)
static void* OS_WINLINCoreThread(void* args)
#endif
{
	os_winlincorethreadparams_t* const params = (os_winlincorethreadparams_t*)args;

	OS_WINLINAssert(
			(params->coreId != OS_INITCORE)
			&& (params->coreId != OS_CORE_ID_INVALID));

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,6,4)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#endif
#endif

	OS_WINLINInitializeCoreThread(params->coreId);

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,6,4)
#pragma GCC diagnostic pop
#endif
#endif

	OS_WINLINLog(
			OS_WINLIN_LOG_DEBUG,
			"Core thread for core %u is going to call main().",
			params->coreId);

	/* Here we start (almost) all over again. Each core thread runs main(),
	 * but only the master core initializes the Windows environment. All other
	 * core threads simply skip this part, i.e. OS_WINLINPreStartOs().
	 */
	main(OS_winlinArgc, OS_winlinArgv);

	/* We must not reach this point. In embedded systems, main() never returns.
	 * The applies also to this environment.
	 */
	OS_WINLINPanic();

#if (OS_ARCH == OS_WINDOWS)
	return 0;		/* Just to please the compiler. */
#elif (OS_ARCH == OS_LINUX)
	return OS_NULL;	/* Just to please the compiler. */
#endif
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
