/* Ioc_k_mk_getappcoreid.c
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_k_mk_getappcoreid.c 1923 2017-02-28 16:22:14Z mist9353 $
*/
#include <private/Mk_application.h>
#include <private/Ioc_osinterface.h>

#if (IOC_LCFG_KERNEL_TYPE != IOC_MICROKERNEL)
#error "This file is meant for the microkernel variant of the IOC. Check your makefiles."
#endif

/* Get the core assignment of an application.
*/
ioc_coreid_t IOC_GetAppCoreId(ioc_os_application_t app)
{
	ioc_coreid_t res = IOC_INVALID_COREID;

	/* Note: It isn't possible to get negative application ids from channel configurations.
	*/
	if (app < MK_nApps)
	{
		res = MK_appCfg[app].coreIndex;
	}

	return res;
}

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
