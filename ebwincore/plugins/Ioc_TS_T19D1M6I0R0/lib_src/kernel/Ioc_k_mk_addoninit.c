/* Ioc_k_mk_addoninit.c - IOC_MkAddOnInit
 *
 * This file contains the function IOC_MkAddOnInit() which initializes the IOC.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_k_mk_addoninit.c 2503 2018-02-07 14:50:09Z mist9353 $
*/

#include <public/Mk_error.h>
#include <public/Ioc_basic_types.h>
#include <public/Ioc_public_api.h>
#include <private/Mk_addon.h>
#include <private/Mk_core.h>
#include <private/Ioc_tool.h>
#include <private/Ioc_mkaddon_private.h>

/* IOC_MkAddOnInit() - Initializes the IOC Add-On
 *
 * !LINKSTO IOC.Function.IOC_MkAddOnInit, 2
*/
void IOC_MkAddOnInit(mk_kernelcontrol_t *unused_coreVars, const mk_addondescriptor_t *unused_addOn)
{
	/* This function has to implement the mk_addoninitfunc_t interface. */
	IOC_PARAM_UNUSED(unused_coreVars);
	IOC_PARAM_UNUSED(unused_addOn);

	if (IOC_Init() != IOC_E_OK)
	{
		MK_StartupPanic(MK_panic_StartupCheckFailed);
	}
}

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
