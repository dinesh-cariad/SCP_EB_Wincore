/* Ioc_k_ac_getcoreid_kernel.c
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_k_ac_getcoreid_kernel.c 1923 2017-02-28 16:22:14Z mist9353 $
*/
#include <Os_kernel.h>
#include <private/Ioc_osinterface.h>

#if (IOC_LCFG_KERNEL_TYPE != IOC_AUTOCOREOS)
#error "This file is only meant for the AutoCore OS variant of the IOC. Check your makefiles."
#endif

/* IOC_GetCoreId_Kernel - Get the index of the calling core from kernel mode.
*/
ioc_coreid_t IOC_GetCoreId_Kernel(void)
{
	return (ioc_coreid_t) OS_ArchGetCoreId();
}

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
