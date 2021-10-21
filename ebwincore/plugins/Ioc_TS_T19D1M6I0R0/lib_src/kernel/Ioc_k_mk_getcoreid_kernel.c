/* Ioc_k_mk_getcoreid_kernel.c
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_k_mk_getcoreid_kernel.c 2497 2018-02-06 12:11:28Z olme8414 $
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

#include <public/Mk_public_types.h>
#include <private/Mk_core.h>
#include <private/Ioc_osinterface.h>

#if (IOC_LCFG_KERNEL_TYPE != IOC_MICROKERNEL)
#error "This file is meant for the microkernel variant of the IOC. Check your makefiles."
#endif

/* IOC_GetCoreId_Kernel - Get the index of the calling core from kernel mode.
*/
ioc_coreid_t IOC_GetCoreId_Kernel(void)
{
	/* Deviation MISRAC2012-1 <1> */
	mk_kernelcontrol_t *coreVars = MK_GetCoreVars();
	ioc_coreid_t returnValue = coreVars->coreIndex;

	return returnValue;
}

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
