/* Ioc_k_ac_getappcoreid.c
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_k_ac_getappcoreid.c 2152 2017-06-23 04:40:43Z masa8317 $
*/
/* TOOLDIAG List of possible tool diagnostics
 *
 * TOOLDIAG-1) Possible diagnostic: UnusedVariable
 *   Variable is set but never used.
 *
 * Reason: Not an issue, the macro OS_GetAppCoreId() which is defined by the OS
 *   doesn't use the variable in single-core systems.
*/

#include <Os_kernel.h>
#include <private/Ioc_osinterface.h>

#if (IOC_LCFG_KERNEL_TYPE != IOC_AUTOCOREOS)
#error "This file is only meant for the AutoCore OS variant of the IOC. Check your makefiles."
#endif

/* Get the core assignment of an application.
*/
ioc_coreid_t IOC_GetAppCoreId(ioc_os_application_t app)
{
	ioc_coreid_t res = IOC_INVALID_COREID;

	if (app < OS_nApps)
	{
		/* Possible diagnostic TOOLDIAG-1 <1> */
		const os_appcontext_t *acx = &OS_appTableBase[app];
		res = OS_GetAppCoreId(acx);
	}

	return res;
}

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
