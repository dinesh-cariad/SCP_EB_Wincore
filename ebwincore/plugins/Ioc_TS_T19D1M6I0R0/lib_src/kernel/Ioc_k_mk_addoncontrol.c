/* Ioc_k_mk_addoncontrol.c - IOC_MkAddOnControl
 *
 * This file contains the function IOC_MkAddOnControl(). which handles all "AddOnControl" system
 * calls that are directed at the IOC add-on (microkernel version 2.x).
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_k_mk_addoncontrol.c 2492 2018-02-05 18:39:24Z mist9353 $
*/
#include <Mk_kconfig.h>
#include <public/Mk_error.h>
/* Includes of IOC Addon */
#include <private/Ioc_mkaddon_private.h>
/* Includes of IOC Plugin */
#include <public/Ioc_basic_types.h>
#include <private/Ioc_queueinterface.h>
#include <private/Ioc_bufferinterface.h>
#include <private/Ioc_tool.h>

/* IOC_MkAddOnControl() - handle the AddOnControl system calls directed at the IOC add-on.
 *
 * The parameters opcode, channelId and data are the parameters passed to the system call.
 * All parameters need to be checked for range and validity before they are used.
 *
 * !LINKSTO IOC.Function.IOC_MkAddOnControl, 1
*/
mk_statusandvalue_t IOC_MkAddOnControl
(	mk_kernelcontrol_t *unused_coreVars,
	const mk_addondescriptor_t *unused_addOn,
	mk_parametertype_t opCode,
	mk_parametertype_t channelId,
	mk_parametertype_t args
)
{
	mk_statusandvalue_t returnValue;

	/* This function has to implement the mk_addoncontrolfunc_t interface. */
	IOC_PARAM_UNUSED(unused_coreVars);
	IOC_PARAM_UNUSED(unused_addOn);

	returnValue.statusCode = MK_eid_NoError;
	returnValue.requestedValue = 0u;

	switch ( opCode )
	{
	case IOC_MKADDON_SEND:
		returnValue.requestedValue = IOC_SysSend(channelId, (const void *)args);
		break;
	case IOC_MKADDON_WRITE:
		returnValue.requestedValue = IOC_SysWrite(channelId, (const void *)args);
		break;
	case IOC_MKADDON_EMPTY_QUEUE:
		returnValue.requestedValue = IOC_SysEmptyQueue(channelId);
		break;
	case IOC_MKADDON_WRITE_EXT:
		returnValue.requestedValue = IOC_SysWriteExt(channelId, (const ioc_extinput_t *)args);
		break;
	case IOC_MKADDON_SEND_EXT:
		returnValue.requestedValue = IOC_SysSendExt(channelId, (const ioc_extinput_t *)args);
		break;
	case IOC_MKADDON_REINITZERO:
		returnValue.requestedValue = IOC_SysReInitZero(channelId);
		break;
	default:
		returnValue.statusCode = MK_eid_OsUnknownSystemCall;
		break;
	}

	return returnValue;
}

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
