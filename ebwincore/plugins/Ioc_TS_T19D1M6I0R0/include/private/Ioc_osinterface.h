/* Ioc_osinterface.h
 *
 * This file defines the OsInterface of the IOC.
 * It defines a mapping of the needed Os functionality to AutoCore Os or the microkernel.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_osinterface.h 2524 2018-02-19 12:17:30Z kosc261728 $
*/

#ifndef IOC_OSINTERFACE_H
#define IOC_OSINTERFACE_H

#ifndef IOC_ASM

#include <public/Ioc_libconfig.h>
#include <public/Ioc_basic_types.h>


#if (IOC_LCFG_KERNEL_TYPE == IOC_MICROKERNEL)
#include <public/Mk_basic_types.h>
/* MK IOC Addon */
#include <public/Ioc_mkaddon_public_api.h>

#elif (IOC_LCFG_KERNEL_TYPE == IOC_AUTOCOREOS)

/* Include headers for kernel and user side implementation. */
#include <Os_types.h>
#include <Ioc/Ioc_kern.h>

#else /* IOC_LCFG_KERNEL_TYPE */
#error "Unknown kernel type."
#endif /* IOC_LCFG_KERNEL_TYPE */


/* ioc_addressrange_t - holds a certain address range.
 *
 * "Begin" is the first accessible address of a range.
 * "Last" is the last accessible address.
 * "End" is one beyond the last accessible address.
 *
 * The current [begin,end) representation, allows to express empty ranges.
 * It doesn't allow to express ranges, which end at the very end of the address space.
 *
 * All address range macros (except _INIT) expect scalar arguments of type ioc_address_t.
*/
typedef struct ioc_addressrange_s ioc_addressrange_t;
struct ioc_addressrange_s
{
	ioc_address_t begin;
	ioc_address_t end;
};
#define IOC_ADDRESSRANGE_BEGIN_END_INIT(aBegin, aEnd) { \
		(ioc_address_t)(aBegin), \
		(ioc_address_t)(aEnd) \
	}
#define IOC_EMPTY_ADDRESSRANGE_INIT { 0u, 0u }
#define IOC_AddressRangeIsEmpty(range) ((range).begin == (range).end)
#define IOC_AddressRangeIsInvalid(range) ((range).begin > (range).end)



#if (IOC_LCFG_KERNEL_TYPE == IOC_MICROKERNEL)

/* ioc_os_application_t - Equivalent to ApplicationType. */
typedef mk_objectid_t ioc_os_application_t;

/* IOC_INVALID_OSAPPLICATION - Equivalent to INVALID_OSAPPLICATION. */
#define IOC_INVALID_OSAPPLICATION ((ioc_os_application_t)MK_APPL_NONE)

/* ioc_coreid_t - Equivalent to CoreIdType. */
typedef mk_objectid_t ioc_coreid_t;
#define IOC_INVALID_COREID	(-1)

/* System Call API */
typedef mk_parametertype_t ioc_os_parametertype_t;
typedef mk_statusandvalue_t ioc_statusandvalue_t;
#define IOC_SyscallSend(channelID, data)						IOC_MkUsrSend((channelID), (data))
#define IOC_SyscallSendExt(channelID, parameters)				IOC_MkUsrSendExt((channelID), (parameters))
#define IOC_SyscallEmptyQueue(channelID)						IOC_MkUsrEmptyQueue(channelID)

#define IOC_SyscallWrite(channelID, data)						IOC_MkUsrWrite((channelID), (data))
#define IOC_SyscallWriteExt(channelID, parameters)				IOC_MkUsrWriteExt((channelID), (parameters))
#define IOC_SyscallReInitZero(channelID)						IOC_MkUsrReInitZero(channelID)

extern const ioc_addressrange_t IOC_appReadableRegions[];


#elif (IOC_LCFG_KERNEL_TYPE == IOC_AUTOCOREOS)

/* ioc_os_application_t - Equivalent to ApplicationType. */
typedef os_applicationid_t ioc_os_application_t;

/* IOC_INVALID_OSAPPLICATION - Equivalent to INVALID_OSAPPLICATION. */
#define IOC_INVALID_OSAPPLICATION OS_NULLAPP

/* ioc_coreid_t - Equivalent to CoreIdType. */
typedef os_coreid_t ioc_coreid_t;
#define IOC_INVALID_COREID	OS_CORE_ID_INVALID

/* System Call API */
#define IOC_OS_ID_WRITE_EXT		0u
#define IOC_OS_ID_SEND_EXT		1u
#define IOC_OS_ID_REINITZERO	2u
typedef os_paramtype_t ioc_os_parametertype_t;
typedef struct ioc_statusandvalue_s ioc_statusandvalue_t;
struct ioc_statusandvalue_s
{
	os_uint32_t statusCode;
	ioc_os_parametertype_t requestedValue;
};
#define IOC_SyscallSend(channelID, data)				IOC_CH_UserIocSend((channelID), (data))
#define IOC_SyscallSendExt(channelID, parameters)		IOC_CH_UserIocSendExt((channelID), (parameters))
#define IOC_SyscallEmptyQueue(channelID)				IOC_CH_UserIocEmptyQueue(channelID)

#define IOC_SyscallWrite(channelID, data)				IOC_CH_UserIocWrite((channelID), (data))
#define IOC_SyscallWriteExt(channelID, parameters)		IOC_CH_UserIocWriteExt((channelID), (parameters))
#define IOC_SyscallReInitZero(channelID)				IOC_CH_UserIocReInitZero(channelID)

extern ioc_statusandvalue_t IOC_CH_UserIocSend(ioc_channelid_t, const void *);
extern ioc_statusandvalue_t IOC_CH_UserIocSendExt(ioc_channelid_t, const void *);
extern ioc_statusandvalue_t IOC_CH_UserIocEmptyQueue(ioc_channelid_t);
extern ioc_statusandvalue_t IOC_CH_UserIocWrite(ioc_channelid_t, const void *);
extern ioc_statusandvalue_t IOC_CH_UserIocWriteExt(ioc_channelid_t, const void *);
extern ioc_statusandvalue_t IOC_CH_UserIocReInitZero(ioc_channelid_t);

extern ioc_return_t IOC_SysOsExtDemux(ioc_uint16_t, ioc_channelid_t, const void *);


#else /* IOC_LCFG_KERNEL_TYPE */
#error "Unknown kernel type."
#endif /* IOC_LCFG_KERNEL_TYPE */


/* IOC_GetApplicationId_Kernel - Equivalent to AUTOSAR's GetApplicationID. */
extern ioc_os_application_t IOC_GetApplicationId_User(void);

/* IOC_GetApplicationId_Kernel - Query Id of active application from kernel mode. */
extern ioc_os_application_t IOC_GetApplicationId_Kernel(void);

/* IOC_GetAppCoreId - Get the core assignment of the given application.
 *
 * This is an OS-specific helper function for IOC_Init.
 * This function is only meant for application ids which are read from a channel configuration.
*/
extern ioc_coreid_t IOC_GetAppCoreId(ioc_os_application_t);

/* IOC_GetCoreId_Kernel - Get the index of the calling core from kernel mode.
 *
 * This is an OS-specific helper function for IOC_Init.
*/
extern ioc_coreid_t IOC_GetCoreId_Kernel(void);


#endif /* IOC_ASM */

#endif /* IOC_OSINTERFACE_H */

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
