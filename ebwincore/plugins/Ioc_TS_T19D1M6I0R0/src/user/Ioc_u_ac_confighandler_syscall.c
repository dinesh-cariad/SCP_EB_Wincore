/* Ioc_u_ac_confighandler_syscall.c
 *
 * This file contains the interfaces for making system calls when using the Autocore OS
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_u_ac_confighandler_syscall.c 2964 2018-11-14 14:05:00Z olme8414 $
*/
/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 12.3 (advisory)
 * The comma operator should not be used.
 *
 * Reason:
 * A function-call kernel maps API calls to the function OS_ArchLeaveKernel().
 * This function takes one argument, which is the return value of the requested
 * kernel service. The necessity to call OS_EnterKernel() before this service
 * requires the use of the comma operator. Otherwise, the API macros which are
 * used for this mapping do no longer expand into plain C-language expressions,
 * which is required here.
 *
 * MISRAC2012-2) Deviated Rule: 13.3 (advisory)
 * A full expression containing an increment (++) or decrement (--) operator
 * should have no other potential side effects other than that caused by the
 * increment or decrement operator.
 *
 * Reason:
 * The ++ operator is used in the context of OS_EnterKernel() for function-call
 * kernels. As function-call kernels use the comma operator, which is a
 * sequence point, to separate the increment operator from other expressions,
 * there is no undefined behavior.
*/

#include <public/Ioc_basic_types.h>
#include <public/Ioc_libconfig.h>
#include <private/Ioc_lockinterface.h>
#include <private/Ioc_tool.h>
#include <private/Ioc_queueinterface.h>
#include <private/Ioc_osinterface.h>
#include <private/Ioc_configinterface.h>
#include <Os.h>

#if (IOC_LCFG_KERNEL_TYPE != IOC_AUTOCOREOS)
#error "This file is only needed for the AutoCore OS variant of the IOC - check your makefiles."
#endif

/*
 * !LINKSTO IOC.Function.IOC_CH_UserIocSend, 1
*/
ioc_statusandvalue_t IOC_CH_UserIocSend(ioc_channelid_t channelID, const void *data)
{
	ioc_statusandvalue_t syscallRet;

	syscallRet.statusCode = E_OK;
	/* Deviation MISRAC2012-1, MISRAC2012-2 */
	syscallRet.requestedValue = OS_UserIocSend(channelID, data);
	return syscallRet;
}

/*
 * !LINKSTO IOC.Function.IOC_CH_UserIocSendExt, 1
*/
ioc_statusandvalue_t IOC_CH_UserIocSendExt(ioc_channelid_t channelID, const void *data)
{
	ioc_statusandvalue_t syscallRet;

	syscallRet.statusCode = E_OK;
	/* Deviation MISRAC2012-1, MISRAC2012-2 */
	syscallRet.requestedValue = OS_UserIocAddonExt(IOC_OS_ID_SEND_EXT, channelID, data);
	return syscallRet;
}

/*
 * !LINKSTO IOC.Function.IOC_CH_UserIocEmptyQueue, 1
*/
ioc_statusandvalue_t IOC_CH_UserIocEmptyQueue(ioc_channelid_t channelID)
{
	ioc_statusandvalue_t syscallRet;

	syscallRet.statusCode = E_OK;
	/* Deviation MISRAC2012-1, MISRAC2012-2 */
	syscallRet.requestedValue = OS_UserIocEmptyQueue(channelID);
	return syscallRet;
}

/*
 * !LINKSTO IOC.Function.IOC_CH_UserIocWrite, 1
*/
ioc_statusandvalue_t IOC_CH_UserIocWrite(ioc_channelid_t channelID, const void *data)
{
	ioc_statusandvalue_t syscallRet;

	syscallRet.statusCode = E_OK;
	/* Deviation MISRAC2012-1, MISRAC2012-2 */
	syscallRet.requestedValue = OS_UserIocWrite(channelID, data);
	return syscallRet;
}

/*
 * !LINKSTO IOC.Function.IOC_CH_UserIocWriteExt, 1
*/
ioc_statusandvalue_t IOC_CH_UserIocWriteExt(ioc_channelid_t channelID, const void *data)
{
	ioc_statusandvalue_t syscallRet;

	syscallRet.statusCode = E_OK;
	/* Deviation MISRAC2012-1, MISRAC2012-2 */
	syscallRet.requestedValue = OS_UserIocAddonExt(IOC_OS_ID_WRITE_EXT, channelID, data);
	return syscallRet;
}

/*
 * !LINKSTO IOC.Function.IOC_CH_UserIocReInitZero, 1
*/
ioc_statusandvalue_t IOC_CH_UserIocReInitZero(ioc_channelid_t channelID)
{
	ioc_statusandvalue_t syscallRet;

	syscallRet.statusCode = E_OK;
	/* Deviation MISRAC2012-1, MISRAC2012-2 */
	syscallRet.requestedValue = OS_UserIocAddonExt(IOC_OS_ID_REINITZERO, channelID, IOC_NULL);
	return syscallRet;
}

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
