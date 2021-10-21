/* Ioc_k_mk_getapplicationid_kernel.c
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_k_mk_getapplicationid_kernel.c 2497 2018-02-06 12:11:28Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 *  A conversion should not be performed from pointer to void into pointer to
 *  object.
 *
 * Reason:
 *  On some architectures the macro MK_GetCoreVars() may use data returned by
 *  hardware to gain access to the core variables. Because hardware does not
 *  know about C language types, a type conversion is required.
 */

#include <public/Ioc_basic_types.h>
#include <private/Ioc_osinterface.h>
#include <public/Mk_basic_types.h>
#include <public/Mk_public_types.h>
#include <public/Mk_autosar.h>
#include <private/Mk_core.h>
#include <private/Mk_thread.h>

#if (IOC_LCFG_KERNEL_TYPE != IOC_MICROKERNEL)
#error "This file is only meant for the Safety OS variant of the IOC. Check your makefiles."
#endif

/* IOC_GetApplicationId_Kernel - Gets the application ID of the caller in kernel mode.
*/
ioc_os_application_t IOC_GetApplicationId_Kernel(void)
{
	ioc_os_application_t callerApp = IOC_INVALID_OSAPPLICATION;

	/* Deviation MISRAC2012-1 <1> */
	mk_kernelcontrol_t *coreVars = MK_GetCoreVars();
	mk_thread_t *currThread = coreVars->currentThread;
	mk_objecttype_t objtyp = currThread->objectType;

	if ( (objtyp == MK_OBJTYPE_TASK) ||
		 (objtyp == MK_OBJTYPE_ISR) )
	{
		callerApp = currThread->applicationId;
	}

	return callerApp;
}
