/* Ioc_k_ac_getapplicationid_kernel.c
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_k_ac_getapplicationid_kernel.c 1860 2017-02-20 12:26:31Z mist9353 $
*/
#include <private/Ioc_osinterface.h>
#include <Os_kernel.h>

#if (IOC_LCFG_KERNEL_TYPE != IOC_AUTOCOREOS)
#error "This file is only meant for the AutoCore OS variant of the IOC. Check your makefiles."
#endif

/* IOC_GetApplicationId_Kernel - Gets the application ID of the caller in kernel mode.
*/
ioc_os_application_t IOC_GetApplicationId_Kernel(void)
{
	return OS_DoGetApplicationId();
}
