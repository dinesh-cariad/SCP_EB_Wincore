/* Ioc_mkaddon_public_api.h - Public API header for the Inter OS-Application Communicator Add-On
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_mkaddon_public_api.h 1346 2016-11-30 18:46:35Z mist9353 $
*/

/* DCG Deviations:
 *
 * DCG-1) Deviated Rule: [OS_PREPROC_010]
 *  Combinations of preprocessor #if and the defined() operator are not permitted!
 *
 * Reason:
 *  This file might be included in the MK configuration and might, therefore, be included
 *  in MK assembler files. MK headers use MK_ASM to avoid C declarations,
 *  when headers are included in assembler files. However, the IOC uses IOC_ASM.
 *  Hence, IOC_ASM has to be defined, if MK_ASM is defined.
*/

#ifndef IOC_MKADDON_PUBLIC_API_H
#define IOC_MKADDON_PUBLIC_API_H

#include <public/Mk_public_api.h>

/* Deviation DCG-1 <+1> */
#if (defined MK_ASM) && (!(defined IOC_ASM))
#define IOC_ASM 1
#endif

#ifndef IOC_ASM

extern const mk_objectid_t IOC_mkAddonId;

/* Enumerated type for the opcodes in IOC_MkAddOnControl
*/
enum ioc_mkaddonopcodes_e
{
	IOC_MKADDON_SEND,
	IOC_MKADDON_SEND_EXT,
	IOC_MKADDON_WRITE,
	IOC_MKADDON_WRITE_EXT,
	IOC_MKADDON_EMPTY_QUEUE,
	IOC_MKADDON_REINITZERO
};
typedef enum ioc_mkaddonopcodes_e ioc_mkaddonopcodes_t;

#define IOC_MkUsrSendExt(channelID, parameters)	\
	MK_UsrAddOnControl3(IOC_mkAddonId, IOC_MKADDON_SEND_EXT,\
		(mk_parametertype_t)(channelID), (mk_parametertype_t)(parameters))

#define IOC_MkUsrSend(channelID, data)	\
	MK_UsrAddOnControl3(IOC_mkAddonId, IOC_MKADDON_SEND,\
		(mk_parametertype_t)(channelID), (mk_parametertype_t)(data))

#define IOC_MkUsrWrite(channelID, data)	\
	MK_UsrAddOnControl3(IOC_mkAddonId, IOC_MKADDON_WRITE,\
		(mk_parametertype_t)(channelID), (mk_parametertype_t)(data))

#define IOC_MkUsrWriteExt(channelID, parameters)	\
	MK_UsrAddOnControl3(IOC_mkAddonId, IOC_MKADDON_WRITE_EXT,\
		(mk_parametertype_t)(channelID), (mk_parametertype_t)(parameters))

#define IOC_MkUsrEmptyQueue(channelID)	\
	MK_UsrAddOnControl2(IOC_mkAddonId, IOC_MKADDON_EMPTY_QUEUE, (mk_parametertype_t)(channelID))

#define IOC_MkUsrReInitZero(channelID)	\
	MK_UsrAddOnControl2(IOC_mkAddonId, IOC_MKADDON_REINITZERO, (mk_parametertype_t)(channelID))


#endif /* IOC_ASM */

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
