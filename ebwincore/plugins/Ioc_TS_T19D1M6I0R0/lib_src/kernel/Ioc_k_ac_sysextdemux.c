/* Ioc_u_ac_confighandler_syscall.c
 *
 * This file contains the system call handler for the IOC_SysOsExtDemux system
 * call. The EB tresos AutoCore OS calls this function, whenever it handles
 * the system call OS_UserIocAddonExt().
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_k_ac_sysextdemux.c 1658 2017-01-20 15:14:55Z stpo8218 $
*/

#include <public/Ioc_basic_types.h>
#include <public/Ioc_libconfig.h>
#include <private/Ioc_queueinterface.h>
#include <private/Ioc_bufferinterface.h>
#include <private/Ioc_osinterface.h>

#if (IOC_LCFG_KERNEL_TYPE != IOC_AUTOCOREOS)
#error "This file is only needed for the AutoCore OS variant of the IOC - check your makefiles."
#endif

/* !LINKSTO IOC.Function.IOC_SysOsExtDemux, 1
*/
ioc_return_t IOC_SysOsExtDemux(ioc_uint16_t fn, ioc_channelid_t channelID, const void *vpparams)
{
	ioc_return_t ret = IOC_E_NOK;

	switch (fn)
	{
	case IOC_OS_ID_WRITE_EXT:
		ret = IOC_SysWriteExt(channelID, (const ioc_extinput_t*)vpparams);
		break;
	case IOC_OS_ID_SEND_EXT:
		ret = IOC_SysSendExt(channelID, (const ioc_extinput_t*)vpparams);
		break;
	case IOC_OS_ID_REINITZERO:
		ret = IOC_SysReInitZero(channelID);
		break;
	default:
		ret = IOC_E_NOFUNC;
		break;
	}
	return ret;
}

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
