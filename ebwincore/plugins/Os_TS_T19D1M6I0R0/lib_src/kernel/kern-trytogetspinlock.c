/* kern-trytogetspinlock.c
 *
 * This file contains the function OS_KernTryToGetSpinlock().
 * It provides the implementation for the AUTOSAR functions TryToGetSpinlock() and GetSpinlock().
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-trytogetspinlock.c 24025 2016-04-21 12:37:21Z mist9353 $
*/

/* The SID and service info must be defined before including any headers because the
 * optimised error handler file needs the definition to translate internal error codes
 * into OSEK/Autosar error status codes
*/
#define OS_SID OS_SID_TryToGetSpinlock
#define OS_SIF OS_svc_TryToGetSpinlock

#include <Os_kernel.h>
#include <Os_spinlock.h>

/*!
 * OS_KernTryToGetSpinlock
 *
 * Tries to get the AUTOSAR spinlock 'lockid'. The success or failure of the acquisition is reported back via 'out'.
 * See [SWS_Os_00703] in "AUTOSAR V5.3.0, R4.1 Rev 3" for further details.
 *
 * !LINKSTO Kernel.Autosar.API.SystemServices.TryToGetSpinlock, 1
 */
os_result_t OS_KernTryToGetSpinlock(os_autosarspinlockid_t lockId, os_tryspinlock_t *out)
{
	os_result_t result;

	OS_PARAMETERACCESS_DECL

	OS_SAVE_PARAMETER_N(0, (os_paramtype_t)lockId);
	OS_SAVE_PARAMETER_N(1, (os_paramtype_t)out);

	if ( OS_ArchCanWrite(out, sizeof(*out)) == 0 )
	{
		result = OS_ERROR(OS_ERROR_WriteProtect, OS_GET_PARAMETER_VAR());
	}
	else
	{
		result = OS_DoTryToGetSpinlock(lockId, out);
	}

	return result;
}

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
