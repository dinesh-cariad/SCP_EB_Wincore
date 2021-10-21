/**
 * \file
 *
 * \brief AUTOSAR Dlt
 *
 * This file contains the implementation of the AUTOSAR
 * module Dlt.
 *
 * \version 1.8.6
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef DLT_TRACE_H
#define DLT_TRACE_H

/* !LINKSTO Dlt.swdd.DltConfig.Template.Dlt_Trace_h,1 */

/*==================[inclusions]============================================*/


#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif

/*==================[macros]================================================*/

#ifndef DBG_DLT_INIT_ENTRY
/** \brief Entry point of function Dlt_Init() */
#define DBG_DLT_INIT_ENTRY()
#endif

#ifndef DBG_DLT_INIT_EXIT
/** \brief Exit point of function Dlt_Init() */
#define DBG_DLT_INIT_EXIT()
#endif

#ifndef DBG_DLT_GETVERSIONINFO_ENTRY
/** \brief Entry point of function Dlt_GetVersionInfo() */
#define DBG_DLT_GETVERSIONINFO_ENTRY(a)
#endif

#ifndef DBG_DLT_GETVERSIONINFO_EXIT
/** \brief Exit point of function Dlt_GetVersionInfo() */
#define DBG_DLT_GETVERSIONINFO_EXIT(a)
#endif

#ifndef DBG_DLT_SENDLOGMESSAGE_ENTRY
/** \brief Entry point of function Dlt_SendLogMessage() */
#define DBG_DLT_SENDLOGMESSAGE_ENTRY(a,b,c,d)
#endif

#ifndef DBG_DLT_SENDLOGMESSAGE_EXIT
/** \brief Exit point of function Dlt_SendLogMessage() */
#define DBG_DLT_SENDLOGMESSAGE_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_DLT_SENDTRACEMESSAGE_ENTRY
/** \brief Entry point of function Dlt_SendTraceMessage() */
#define DBG_DLT_SENDTRACEMESSAGE_ENTRY(a,b,c,d)
#endif

#ifndef DBG_DLT_SENDTRACEMESSAGE_EXIT
/** \brief Exit point of function Dlt_SendTraceMessage() */
#define DBG_DLT_SENDTRACEMESSAGE_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_DLT_REGISTERCONTEXT_ENTRY
/** \brief Entry point of function Dlt_RegisterContext() */
#define  DBG_DLT_REGISTERCONTEXT_ENTRY(a,b,c,d,e,f,g)
#endif

#ifndef DBG_DLT_REGISTERCONTEXTNOTIFICATION_ENTRY
/** \brief Entry point of function Dlt_RegisterContextNotification() */
#define DBG_DLT_REGISTERCONTEXTNOTIFICATION_ENTRY(a,b,c,d);
#endif

#ifndef DBG_DLT_REGISTERCONTEXTNOTIFICATION_EXIT
/** \brief Exit point of function Dlt_RegisterContextNotification() */
#define DBG_DLT_REGISTERCONTEXTNOTIFICATION_EXIT(a,b,c,d);
#endif

#ifndef DBG_DLT_REGISTERCONTEXT_EXIT
/** \brief Exit point of function Dlt_RegisterContext() */
#define DBG_DLT_REGISTERCONTEXT_EXIT(a,b,c,d,e,f,g,h)
#endif

#ifndef DBG_DLT_UNREGISTERCONTEXT_ENTRY
/** \brief Entry point of function Dlt_UnregisterContext() */
#define DBG_DLT_UNREGISTERCONTEXT_ENTRY(a,b,c)
#endif

#ifndef DBG_DLT_UNREGISTERCONTEXT_EXIT
/** \brief Exit point of function Dlt_UnregisterContext() */
#define DBG_DLT_UNREGISTERCONTEXT_EXIT(a,b,c,d)
#endif

#ifndef DBG_DLT_COMSTARTOFRECEPTION_ENTRY
/** \brief Entry point of function Dlt_ComStartOfReception() */
#define DBG_DLT_COMSTARTOFRECEPTION_ENTRY(a,b,c)
#endif

#ifndef DBG_DLT_COMSTARTOFRECEPTION_EXIT
/** \brief Exit point of function Dlt_ComStartOfReception() */
#define DBG_DLT_COMSTARTOFRECEPTION_EXIT(a,b,c,d)
#endif

#ifndef DBG_DLT_COMCOPYRXDATA_ENTRY
/** \brief Entry point of function Dlt_ComCopyRxData() */
#define DBG_DLT_COMCOPYRXDATA_ENTRY(a,b,c)
#endif

#ifndef DBG_DLT_COMCOPYRXDATA_EXIT
/** \brief Exit point of function Dlt_ComCopyRxData() */
#define DBG_DLT_COMCOPYRXDATA_EXIT(a,b,c,d)
#endif

#ifndef DBG_DLT_COMRXINDICATION_ENTRY
/** \brief Entry point of function Dlt_ComRxIndication() */
#define DBG_DLT_COMRXINDICATION_ENTRY(a)
#endif

#ifndef DBG_DLT_COMRXINDICATION_EXIT
/** \brief Exit point of function Dlt_ComRxIndication() */
#define DBG_DLT_COMRXINDICATION_EXIT(a)
#endif

#ifndef DBG_DLT_COMTXCONFIRMATION_ENTRY
/** \brief Entry point of function Dlt_ComTxConfirmation() */
#define DBG_DLT_COMTXCONFIRMATION_ENTRY(a,b)
#endif

#ifndef DBG_DLT_COMTXCONFIRMATION_EXIT
/** \brief Exit point of function Dlt_ComTxConfirmation() */
#define DBG_DLT_COMTXCONFIRMATION_EXIT(a,b)
#endif

#ifndef DBG_DLT_COMCOPYTXDATA_ENTRY
/** \brief Entry point of function Dlt_ComCopyTxData() */
#define DBG_DLT_COMCOPYTXDATA_ENTRY(a,b,c,d)
#endif

#ifndef DBG_DLT_SENDPRIORITYREQUEST_ENTRY
/** \brief Point entry of send higher priority message */
#define DBG_DLT_SENDPRIORITYREQUEST_ENTRY()
#endif

#ifndef DBG_DLT_SENDPRIORITYREQUEST_EXIT
/** \brief Point exit of send higher priority message */
#define DBG_DLT_SENDPRIORITYREQUEST_EXIT()
#endif

#ifndef DBG_DLT_COMCOPYTXDATA_EXIT
/** \brief Exit point of function Dlt_ComCopyTxData() */
#define DBG_DLT_COMCOPYTXDATA_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_DLT_MAINFUNCTION_ENTRY
/** \brief Entry point of function Dlt_MainFunction() */
#define DBG_DLT_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_DLT_MAINFUNCTION_EXIT
/** \brief Exit point of function Dlt_MainFunction() */
#define DBG_DLT_MAINFUNCTION_EXIT()
#endif

#ifndef DBG_DLT_TRIGGERTRANSMIT_ENTRY
/** \brief Entry point of function Dlt_TriggerTransmit() */
#define DBG_DLT_TRIGGERTRANSMIT_ENTRY(a,b)
#endif

#ifndef DBG_DLT_TRIGGERTRANSMIT_EXIT
/** \brief Exit point of function Dlt_TriggerTransmit() */
#define DBG_DLT_TRIGGERTRANSMIT_EXIT(a,b)
#endif

#ifndef DBG_DLT_TRIGGERTRANSMIT_EXIT_43
/** \brief Exit point of function Dlt_TriggerTransmit() for AUTOSAR_431*/
#define DBG_DLT_TRIGGERTRANSMIT_EXIT_43(a,b,c)
#endif

#ifndef DBG_DLT_GETCURMESSAGELENGTH_ENTRY
/** \brief Entry point of function Dlt_GetCurMessageLength() */
#define DBG_DLT_GETCURMESSAGELENGTH_ENTRY(a,b)
#endif

#ifndef DBG_DLT_GETCURMESSAGELENGTH_EXIT
/** \brief Exit point of function Dlt_GetCurMessageLength() */
#define DBG_DLT_GETCURMESSAGELENGTH_EXIT(a,b,c)
#endif

#ifndef DBG_DLT_NEXTMESSAGE_ENTRY
/** \brief Entry point of function Dlt_NextMessage() */
#define DBG_DLT_NEXTMESSAGE_ENTRY()
#endif

#ifndef DBG_DLT_NEXTMESSAGE_EXIT
/** \brief Exit point of function Dlt_NextMessage() */
#define DBG_DLT_NEXTMESSAGE_EXIT()
#endif

#ifndef DBG_DLT_STOREINBUFFER_ENTRY
/** \brief Entry point of function Dlt_StoreInBuffer() */
#define DBG_DLT_STOREINBUFFER_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_DLT_STOREINBUFFER_EXIT
/** \brief Exit point of function Dlt_StoreInBuffer() */
#define DBG_DLT_STOREINBUFFER_EXIT(a,b,c,d,e,f,g)
#endif

#ifndef DBG_DLT_WRITEDWORDTOBYTEARRAY_EXIT
/** \brief Exit point of function Dlt_WriteDwordToByteArray() */
#define DBG_DLT_WRITEDWORDTOBYTEARRAY_EXIT(a)
#endif

#ifndef DBG_DLT_WRITEWORDTOBYTEARRAY_ENTRY
/** \brief Entry point of function Dlt_WriteWordToByteArray() */
#define DBG_DLT_WRITEWORDTOBYTEARRAY_ENTRY(a)
#endif

#ifndef DBG_DLT_WRITEWORDTOBYTEARRAY_EXIT
/** \brief Exit point of function Dlt_WriteWordToByteArray() */
#define DBG_DLT_WRITEWORDTOBYTEARRAY_EXIT(a)
#endif

#ifndef DBG_DLT_WRITEDWORDTOBYTEARRAY_ENTRY
/** \brief Entry point of function Dlt_WriteDwordToByteArray() */
#define DBG_DLT_WRITEDWORDTOBYTEARRAY_ENTRY(a)
#endif

#ifndef DBG_DLT_READFROMBUFFER_ENTRY
/** \brief Entry point of function Dlt_ReadFromBuffer() */
#define DBG_DLT_READFROMBUFFER_ENTRY(a,b,c)
#endif

#ifndef DBG_DLT_READFROMBUFFER_EXIT
/** \brief Exit point of function Dlt_ReadFromBuffer() */
#define DBG_DLT_READFROMBUFFER_EXIT(a,b,c,d)
#endif

#ifndef DBG_DLT_INTERNALSENDMESSAGE_ENTRY
/** \brief Entry point of function Dlt_InternalSendMessage() */
#define DBG_DLT_INTERNALSENDMESSAGE_ENTRY(a,b,c,d,e,f,g)
#endif

#ifndef DBG_DLT_INTERNALSENDMESSAGE_EXIT
/** \brief Exit point of function Dlt_InternalSendMessage() */
#define DBG_DLT_INTERNALSENDMESSAGE_EXIT(a,b,c,d,e,f,g,h)
#endif

#ifndef DBG_DLT_INTERNALREGISTERCONTEXT_ENTRY
/** \brief Entry point of function Dlt_InternalRegisterContext() */
#define DBG_DLT_INTERNALREGISTERCONTEXT_ENTRY(a,b,c,d,e,f,g)
#endif

#ifndef DBG_DLT_INTERNALREGISTERCONTEXT_EXIT
/** \brief Exit point of function Dlt_InternalRegisterContext() */
#define DBG_DLT_INTERNALREGISTERCONTEXT_EXIT(a,b,c,d,e,f,g,h)
#endif

#ifndef DBG_DLT_INTERNALREGISTERLOCALCONTEXT_ENTRY
/** \brief Entry point of function Dlt_InternalRegisterLocalContext() */
#define DBG_DLT_INTERNALREGISTERLOCALCONTEXT_ENTRY(a,b,c,d,e,f,g,h)
#endif

#ifndef DBG_DLT_INTERNALREGISTERLOCALCONTEXT_EXIT
/** \brief Exit point of function Dlt_InternalRegisterLocalContext() */
#define DBG_DLT_INTERNALREGISTERLOCALCONTEXT_EXIT(a,b,c,d,e,f,g,h)
#endif

#ifndef DBG_DLT_STOREPERSISTENT_ENTRY
/** \brief Entry point of function Dlt_StorePersistent() */
#define DBG_DLT_STOREPERSISTENT_ENTRY()
#endif

#ifndef DBG_DLT_STOREPERSISTENT_EXIT
/** \brief Exit point of function Dlt_StorePersistent() */
#define DBG_DLT_STOREPERSISTENT_EXIT(a)
#endif

#ifndef DBG_DLT_STORECONFIGURATION_ENTRY
/** \brief Entry point of function Dlt_StoreConfiguration() */
#define DBG_DLT_STORECONFIGURATION_ENTRY()
#endif

#ifndef DBG_DLT_NVMSTORAGE_INJECTION
/** \brief Injection point of function before Dlt_IssueWriteRequestToNvM() */
#define DBG_DLT_NVMSTORAGE_INJECTION()
#endif

#ifndef DBG_DLT_STORECONFIGURATION_EXIT
/** \brief Exit point of function Dlt_StoreConfiguration() */
#define DBG_DLT_STORECONFIGURATION_EXIT(a)
#endif

#ifndef DBG_DLT_NVMWRITERAMBLOCKTONVDATASETCBK_ENTRY
/** \brief Entry point of function Dlt_NvMWriteRamBlockToNvMDataSetCbk() */
#define DBG_DLT_NVMWRITERAMBLOCKTONVDATASETCBK_ENTRY(a)
#endif

#ifndef DBG_DLT_NVMWRITERAMBLOCKTONVDATASETCBK_EXIT
/** \brief Exit point of function Dlt_NvMWriteRamBlockToNvMDataSetCbk() */
#define DBG_DLT_NVMWRITERAMBLOCKTONVDATASETCBK_EXIT(a, b)
#endif

#ifndef DBG_DLT_NVMWRITERAMBLOCKTONVNATIVECBK_ENTRY
/** \brief Entry point of function Dlt_NvMWriteRamBlockToNvMNativeCbk() */
#define DBG_DLT_NVMWRITERAMBLOCKTONVNATIVECBK_ENTRY(a)
#endif

#ifndef DBG_DLT_NVMWRITERAMBLOCKTONVNATIVECBK_EXIT
/** \brief Exit point of function Dlt_NvMWriteRamBlockToNvMNativeCbk() */
#define DBG_DLT_NVMWRITERAMBLOCKTONVNATIVECBK_EXIT(a, b)
#endif

#ifndef DBG_DLT_NVMINITDATASETBLOCKCBK_ENTRY
/** \brief Entry point of function Dlt_NvMInitDataSetBlockCbk() */
#define DBG_DLT_NVMINITDATASETBLOCKCBK_ENTRY()
#endif

#ifndef DBG_DLT_NVMINITDATASETBLOCKCBK_EXIT
/** \brief Exit point of function Dlt_NvMInitDataSetBlockCbk() */
#define DBG_DLT_NVMINITDATASETBLOCKCBK_EXIT(a)
#endif

#ifndef DBG_DLT_NVMINITNATIVECBK_ENTRY
/** \brief Entry point of function Dlt_NvMInitNativeBlockCbk() */
#define DBG_DLT_NVMINITNATIVECBK_ENTRY()
#endif

#ifndef DBG_DLT_NVMINITNATIVECBK_EXIT
/** \brief Exit point of function Dlt_NvMInitNativeBlockCbk() */
#define DBG_DLT_NVMINITNATIVECBK_EXIT(a)
#endif

#ifndef DBG_DLT_NVMSINGLEBLOCKCALLBACKNATIVE_ENTRY
/** \brief Entry point of function Dlt_NvMSingleBlockCallbackNative() */
#define DBG_DLT_NVMSINGLEBLOCKCALLBACKNATIVE_ENTRY(a, b)
#endif

#ifndef DBG_DLT_NVMSINGLEBLOCKCALLBACKNATIVE_EXIT
/** \brief Exit point of function Dlt_NvMSingleBlockCallbackNative() */
#define DBG_DLT_NVMSINGLEBLOCKCALLBACKNATIVE_EXIT(a, b, c)
#endif

#ifndef DBG_DLT_WRITEDWORDTOBYTEARRAY_ENTRY
/** \brief Entry point of function DLT_WriteDwordToByteArray() */
#define DBG_DLT_WRITEDWORDTOBYTEARRAY_ENTRY(a)
#endif

#ifndef DBG_DLT_WRITEDWORDTOBYTEARRAY_EXIT
/** \brief Exit point of function DLT_WriteDwordToByteArray() */
#define DBG_DLT_WRITEDWORDTOBYTEARRAY_EXIT(a)
#endif

#ifndef DBG_DLT_READDWORDFROMBYTEARRAY_ENTRY
/** \brief Entry point of function Dlt_ReadDwordFromByteArray() */
#define DBG_DLT_READDWORDFROMBYTEARRAY_ENTRY(a,b)
#endif

#ifndef DBG_DLT_READDWORDFROMBYTEARRAY_EXIT
/** \brief Exit point of function Dlt_ReadDwordFromByteArray() */
#define DBG_DLT_READDWORDFROMBYTEARRAY_EXIT(a,b)
#endif

#ifndef DBG_DLT_APPLGETCONFIGFACTORYDEFAULT_ENTRY
/** \brief Entry point of function Dlt_ApplGetConfigFactoryDefault() */
#define DBG_DLT_APPLGETCONFIGFACTORYDEFAULT_ENTRY(a, b, c, d, e)
#endif

#ifndef DBG_DLT_APPLGETCONFIGFACTORYDEFAULT_EXIT
/** \brief Exit point of function Dlt_ApplGetConfigFactoryDefault() */
#define DBG_DLT_APPLGETCONFIGFACTORYDEFAULT_EXIT(a, b, c, d, e, f)
#endif

#ifndef DBG_DLT_SETLOGLEVEL_EXCL_ENTRY
/** \brief Entry point of function Dlt_ASR42_SetLogLevel()'s exclusive area */
#define DBG_DLT_SETLOGLEVEL_EXCL_ENTRY()
#endif

#ifndef DBG_DLT_SETLOGCHANNELASSIGNMENT_ENTRY
/** \brief Entry point of function Dlt_SetLogChannelAssignment() */
#define DBG_DLT_SETLOGCHANNELASSIGNMENT_ENTRY(a,b,c,d)
#endif

#ifndef DBG_DLT_SETLOGCHANNELASSIGNMENT_EXIT
/** \brief Exit point of function Dlt_SetLogChannelAssignment() */
#define DBG_DLT_SETLOGCHANNELASSIGNMENT_EXIT(a,b,c,d)
#endif

#ifndef DBG_DLT_GETLOGCHANNELNAMES_ENTRY
/** \brief Entry point of function Dlt_GetLogChannelNames() */
#define DBG_DLT_GETLOGCHANNELNAMES_ENTRY(a,b)
#endif

#ifndef DBG_DLT_GETLOGCHANNELNAMES_EXIT
/** \brief Exit point of function Dlt_GetLogChannelNames() */
#define DBG_DLT_GETLOGCHANNELNAMES_EXIT(a,b)
#endif

#ifndef DBG_DLT_SETLOGCHANNELTHRESHOLD_ENTRY
/** \brief Entry point of function Dlt_SetLogChannelThreshold() */
#define DBG_DLT_SETLOGCHANNELTHRESHOLD_ENTRY(a,b,c)
#endif

#ifndef DBG_DLT_SETLOGCHANNELTHRESHOLD_EXIT
/** \brief Exit point of function Dlt_SetLogChannelThreshold() */
#define DBG_DLT_SETLOGCHANNELTHRESHOLD_EXIT(a,b,c)
#endif

#ifndef DBG_DLT_GETLOGCHANNELTHRESHOLD_ENTRY
/** \brief Entry point of function Dlt_GetLogChannelThreshold() */
#define DBG_DLT_GETLOGCHANNELTHRESHOLD_ENTRY(a,b,c)
#endif

#ifndef DBG_DLT_GETLOGCHANNELTHRESHOLD_EXIT
/** \brief Exit point of function Dlt_GetLogChannelThreshold() */
#define DBG_DLT_GETLOGCHANNELTHRESHOLD_EXIT(a,b,c)
#endif

#ifndef DBG_DLT_SETLOGLEVEL_ENTRY
/** \brief Entry point of function Dlt_ASR42_SetLogLevel() */
#define DBG_DLT_SETLOGLEVEL_ENTRY(a,b,c)
#endif

#ifndef DBG_DLT_SETLOGLEVEL_EXIT
/** \brief Exit point of function Dlt_ASR42_SetLogLevel() */
#define DBG_DLT_SETLOGLEVEL_EXIT(a,b,c,d)
#endif

#ifndef DBG_DLT_SET_LOGLEVEL_ENTRY
/** \brief Entry point of function Dlt_ASR43_SetLogLevel() */
#define DBG_DLT_SET_LOGLEVEL_ENTRY(a,b,c)
#endif

#ifndef DBG_DLT_SET_LOGLEVEL_EXIT
/** \brief Exit point of function Dlt_ASR43_SetLogLevel() */
#define DBG_DLT_SET_LOGLEVEL_EXIT(a,b,c,d)
#endif

#ifndef DBG_DLT_SETDEFAULTLOGLEVEL_ENTRY
/** \brief Entry point of function Dlt_ASR42_SetDefaultLogLevel() */
#define DBG_DLT_SETDEFAULTLOGLEVEL_ENTRY(a)
#endif

#ifndef DBG_DLT_SETDEFAULTLOGLEVEL_EXIT
/** \brief Exit point of function Dlt_ASR42_SetDefaultLogLevel() */
#define DBG_DLT_SETDEFAULTLOGLEVEL_EXIT(a,b)
#endif

#ifndef DBG_DLT_SET_DEFAULTLOGLEVEL_ENTRY
/** \brief Entry point of function Dlt_ASR43_SetDefaultLogLevel() */
#define DBG_DLT_SET_DEFAULTLOGLEVEL_ENTRY(a)
#endif

#ifndef DBG_DLT_SET_DEFAULTLOGLEVEL_EXIT
/** \brief Exit point of function Dlt_ASR43_SetDefaultLogLevel() */
#define DBG_DLT_SET_DEFAULTLOGLEVEL_EXIT(a,b)
#endif

#ifndef DBG_DLT_GETLOGLEVEL_ENTRY
/** \brief Entry point of function Dlt_GetLogLevel() */
#define DBG_DLT_GETLOGLEVEL_ENTRY(a,b,c)
#endif

#ifndef DBG_DLT_GETLOGLEVEL_EXIT
/** \brief Exit point of function Dlt_GetLogLevel() */
#define DBG_DLT_GETLOGLEVEL_EXIT(a,b,c,d)
#endif

#ifndef DBG_DLT_GETLOGINFO_ENTRY
/** \brief Entry point of function Dlt_GetLogInfo() */
#define DBG_DLT_GETLOGINFO_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_DLT_GETLOGINFO_EXIT
/** \brief Exit point of function Dlt_GetLogInfo() */
#define DBG_DLT_GETLOGINFO_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_DLT_GETLOGINFOINTERNAL_ENTRY
/** \brief Entry point of function Dlt_GetLogInfo() */
#define DBG_DLT_GETLOGINFOINTERNAL_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_DLT_GETLOGINFOINTERNAL_EXIT
/** \brief Exit point of function Dlt_GetLogInfo() */
#define DBG_DLT_GETLOGINFOINTERNAL_EXIT(a,b,c,d,e,f,g)
#endif

#ifndef DBG_DLT_GETDEFAULTLOGLEVEL_ENTRY
/** \brief Entry point of function Dlt_ASR42_GetDefaultLogLevel() */
#define DBG_DLT_GETDEFAULTLOGLEVEL_ENTRY()
#endif

#ifndef DBG_DLT_GETDEFAULTLOGLEVEL_EXIT
/** \brief Exit point of function Dlt_ASR42_GetDefaultLogLevel() */
#define DBG_DLT_GETDEFAULTLOGLEVEL_EXIT(a)
#endif

#ifndef DBG_DLT_GET_DEFAULTLOGLEVEL_ENTRY
/** \brief Entry point of function Dlt_ASR43_GetDefaultLogLevel() */
#define DBG_DLT_GET_DEFAULTLOGLEVEL_ENTRY(a)
#endif

#ifndef DBG_DLT_GET_DEFAULTLOGLEVEL_EXIT
/** \brief Exit point of function Dlt_ASR43_GetDefaultLogLevel() */
#define DBG_DLT_GET_DEFAULTLOGLEVEL_EXIT(a,b)
#endif

#ifndef DBG_DLT_SETTRACESTATUS_EXCL_ENTRY
/** \brief Entry point of function Dlt_ASR42_SetTraceStatus()'s exclusive area */
#define DBG_DLT_SETTRACESTATUS_EXCL_ENTRY()
#endif

#ifndef DBG_DLT_SETTRACESTATUS_ENTRY
/** \brief Entry point of function Dlt_ASR42_SetTraceStatus() */
#define DBG_DLT_SETTRACESTATUS_ENTRY(a,b,c)
#endif

#ifndef DBG_DLT_SETTRACESTATUS_EXIT
/** \brief Exit point of function Dlt_ASR42_SetTraceStatus() */
#define DBG_DLT_SETTRACESTATUS_EXIT(a,b,c,d)
#endif

#ifndef DBG_DLT_SET_TRACESTATUS_ENTRY
/** \brief Entry point of function Dlt_ASR43_SetTraceStatus() */
#define DBG_DLT_SET_TRACESTATUS_ENTRY(a,b,c)
#endif

#ifndef DBG_DLT_SET_TRACESTATUS_EXIT
/** \brief Exit point of function Dlt_ASR43_SetTraceStatus() */
#define DBG_DLT_SET_TRACESTATUS_EXIT(a,b,c,d)
#endif

#ifndef DBG_DLT_SETDEFAULTTRACESTATUS_ENTRY
/** \brief Entry point of function Dlt_ASR42_SetDefaultTraceStatus() */
#define DBG_DLT_SETDEFAULTTRACESTATUS_ENTRY(a)
#endif

#ifndef DBG_DLT_SETDEFAULTTRACESTATUS_EXIT
/** \brief Exit point of function Dlt_ASR42_SetDefaultTraceStatus() */
#define DBG_DLT_SETDEFAULTTRACESTATUS_EXIT(a,b)
#endif

#ifndef DBG_DLT_SET_DEFAULTTRACESTATUS_ENTRY
/** \brief Entry point of function Dlt_ASR43_SetDefaultTraceStatus() */
#define DBG_DLT_SET_DEFAULTTRACESTATUS_ENTRY(a)
#endif

#ifndef DBG_DLT_SET_DEFAULTTRACESTATUS_EXIT
/** \brief Exit point of function Dlt_ASR43_SetDefaultTraceStatus() */
#define DBG_DLT_SET_DEFAULTTRACESTATUS_EXIT(a,b)
#endif

#ifndef DBG_DLT_GETDEFAULTTRACESTATUS_ENTRY
/** \brief Entry point of function Dlt_ASR42_GetDefaultTraceStatus() */
#define DBG_DLT_GETDEFAULTTRACESTATUS_ENTRY()
#endif

#ifndef DBG_DLT_GETDEFAULTTRACESTATUS_EXIT
/** \brief Exit point of function Dlt_ASR42_GetDefaultTraceStatus() */
#define DBG_DLT_GETDEFAULTTRACESTATUS_EXIT(a)
#endif

#ifndef DBG_DLT_GET_DEFAULTTRACESTATUS_ENTRY
/** \brief Entry point of function Dlt_ASR43_GetDefaultTraceStatus() */
#define DBG_DLT_GET_DEFAULTTRACESTATUS_ENTRY(a)
#endif

#ifndef DBG_DLT_GET_DEFAULTTRACESTATUS_EXIT
/** \brief Exit point of function Dlt_ASR43_GetDefaultTraceStatus() */
#define DBG_DLT_GET_DEFAULTTRACESTATUS_EXIT(a,b)
#endif

#ifndef DBG_DLT_GETTRACESTATUS_ENTRY
/** \brief Entry point of function Dlt_ASR42_GetTraceStatus() */
#define DBG_DLT_GETTRACESTATUS_ENTRY(a,b,c)
#endif

#ifndef DBG_DLT_GETTRACESTATUS_EXIT
/** \brief Exit point of function Dlt_ASR42_GetTraceStatus() */
#define DBG_DLT_GETTRACESTATUS_EXIT(a,b,c,d)
#endif

#ifndef DBG_DLT_GET_TRACESTATUS_ENTRY
/** \brief Entry point of function Dlt_ASR43_GetTraceStatus() */
#define DBG_DLT_GET_TRACESTATUS_ENTRY(a,b,c)
#endif

#ifndef DBG_DLT_GET_TRACESTATUS_EXIT
/** \brief Exit point of function Dlt_ASR43_GetTraceStatus() */
#define DBG_DLT_GET_TRACESTATUS_EXIT(a,b,c,d)
#endif

#ifndef DBG_DLT_SETMESSAGEFILTERING_ENTRY
/** \brief Entry point of function Dlt_ASR42_SetMessageFiltering() */
#define DBG_DLT_SETMESSAGEFILTERING_ENTRY(a)
#endif

#ifndef DBG_DLT_SETMESSAGEFILTERING_EXIT
/** \brief Exit point of function Dlt_ASR42_SetMessageFiltering() */
#define DBG_DLT_SETMESSAGEFILTERING_EXIT(a,b)
#endif

#ifndef DBG_DLT_SET_MESSAGEFILTERING_ENTRY
/** \brief Entry point of function Dlt_ASR43_SetMessageFiltering() */
#define DBG_DLT_SET_MESSAGEFILTERING_ENTRY(a)
#endif

#ifndef DBG_DLT_SET_MESSAGEFILTERING_EXIT
/** \brief Exit point of function Dlt_ASR43_SetMessageFiltering() */
#define DBG_DLT_SET_MESSAGEFILTERING_EXIT(a,b)
#endif

#ifndef DBG_DLT_GETMESSAGEFILTERINGSTATUS_ENTRY
/** \brief Entry point of function Dlt_GetMessageFiltering() */
#define DBG_DLT_GETMESSAGEFILTERINGSTATUS_ENTRY()
#endif

#ifndef DBG_DLT_GETMESSAGEFILTERINGSTATUS_EXIT
/** \brief Exit point of function Dlt_GetMessageFiltering() */
#define DBG_DLT_GETMESSAGEFILTERINGSTATUS_EXIT(a)
#endif

#ifndef DBG_DLT_RESETTOFACTORYDEFAULT_ENTRY
/** \brief Entry point of function Dlt_ResetToFactoryDefault() */
#define DBG_DLT_RESETTOFACTORYDEFAULT_ENTRY()
#endif

#ifndef DBG_DLT_RESETTOFACTORYDEFAULT_EXIT
/** \brief Exit point of function Dlt_ResetToFactoryDefault() */
#define DBG_DLT_RESETTOFACTORYDEFAULT_EXIT(a)
#endif

#ifndef DBG_DLT_SETUSEECUID_ENTRY
/** \brief Entry point of function Dlt_SetUseECUID() */
#define DBG_DLT_SETUSEECUID_ENTRY(a)
#endif

#ifndef DBG_DLT_SETUSEECUID_EXIT
/** \brief Exit point of function Dlt_SetUseECUID() */
#define DBG_DLT_SETUSEECUID_EXIT(a,b)
#endif

#ifndef DBG_DLT_GETUSEECUID_ENTRY
/** \brief Entry point of function Dlt_GetUseECUID() */
#define DBG_DLT_GETUSEECUID_ENTRY()
#endif

#ifndef DBG_DLT_GETUSEECUID_EXIT
/** \brief Exit point of function Dlt_GetUseECUID() */
#define DBG_DLT_GETUSEECUID_EXIT(a)
#endif


#ifndef DBG_DLT_SETVERBOSEMODE_ENTRY
/** \brief Entry point of function Dlt_SetVerboseMode() */
#define DBG_DLT_SETVERBOSEMODE_ENTRY(a,b,c)
#endif

#ifndef DBG_DLT_SETVERBOSEMODE_EXIT
/** \brief Exit point of function Dlt_SetVerboseMode() */
#define DBG_DLT_SETVERBOSEMODE_EXIT(a,b,c,d)
#endif

#ifndef DBG_DLT_GETVERBOSEMODESTATUS_ENTRY
/** \brief Entry point of function Dlt_GetVerboseModeStatus() */
#define DBG_DLT_GETVERBOSEMODESTATUS_ENTRY(a,b,c)
#endif

#ifndef DBG_DLT_GETVERBOSEMODESTATUS_EXIT
/** \brief Exit point of function Dlt_GetVerboseModeStatus() */
#define DBG_DLT_GETVERBOSEMODESTATUS_EXIT(a,b,c,d)
#endif

#ifndef DBG_DLT_SETUSESESSIONID_ENTRY
/** \brief Entry point of function Dlt_SetUseSessionId() */
#define DBG_DLT_SETUSESESSIONID_ENTRY(a)
#endif

#ifndef DBG_DLT_SETUSESESSIONID_EXIT
/** \brief Exit point of function Dlt_SetUseSessionId() */
#define DBG_DLT_SETUSESESSIONID_EXIT(a,b)
#endif

#ifndef DBG_DLT_GETUSESESSIONID_ENTRY
/** \brief Entry point of function Dlt_GetUseSessionId() */
#define DBG_DLT_GETUSESESSIONID_ENTRY()
#endif

#ifndef DBG_DLT_GETUSESESSIONID_EXIT
/** \brief Exit point of function Dlt_GetUseSessionId() */
#define DBG_DLT_GETUSESESSIONID_EXIT(a)
#endif

#ifndef DBG_DLT_SETUSETIMESTAMP_ENTRY
/** \brief Entry point of function Dlt_SetUseTimestamp() */
#define DBG_DLT_SETUSETIMESTAMP_ENTRY(a)
#endif

#ifndef DBG_DLT_SETUSETIMESTAMP_EXIT
/** \brief Exit point of function Dlt_SetUseTimestamp() */
#define DBG_DLT_SETUSETIMESTAMP_EXIT(a,b)
#endif

#ifndef DBG_DLT_GETUSETIMESTAMP_ENTRY
/** \brief Entry point of function Dlt_GetUseTimestamp() */
#define DBG_DLT_GETUSETIMESTAMP_ENTRY()
#endif

#ifndef DBG_DLT_GETUSETIMESTAMP_EXIT
/** \brief Exit point of function Dlt_GetUseTimestamp() */
#define DBG_DLT_GETUSETIMESTAMP_EXIT(a)
#endif

#ifndef DBG_DLT_SETUSEEXTENDEDHEADER_ENTRY
/** \brief Entry point of function Dlt_SetUseExtendedHeader() */
#define DBG_DLT_SETUSEEXTENDEDHEADER_ENTRY(a)
#endif

#ifndef DBG_DLT_SETUSEEXTENDEDHEADER_EXIT
/** \brief Exit point of function Dlt_SetUseExtendedHeader() */
#define DBG_DLT_SETUSEEXTENDEDHEADER_EXIT(a,b)
#endif

#ifndef DBG_DLT_GETUSEEXTENDEDHEADER_ENTRY
/** \brief Entry point of function Dlt_GetUseExtendedHeader() */
#define DBG_DLT_GETUSEEXTENDEDHEADER_ENTRY()
#endif

#ifndef DBG_DLT_GETUSEEXTENDEDHEADER_EXIT
/** \brief Exit point of function Dlt_GetUseExtendedHeader() */
#define DBG_DLT_GETUSEEXTENDEDHEADER_EXIT(a)
#endif

#ifndef DBG_DLT_GETTIMESTAMP_ENTRY
/** \brief Entry point of function Dlt_GetTimestamp() */
#define DBG_DLT_GETTIMESTAMP_ENTRY()
#endif

#ifndef DBG_DLT_GETTIMESTAMP_EXIT
/** \brief Exit point of function Dlt_GetTimestamp() */
#define DBG_DLT_GETTIMESTAMP_EXIT(a)
#endif

#ifndef DBG_DLT_SETGLOBALLOGGING_ENTRY
/** \brief Entry point of function Dlt_SetGlobalLogging() */
#define DBG_DLT_SETGLOBALLOGGING_ENTRY(a)
#endif

#ifndef DBG_DLT_SETGLOBALLOGGING_EXIT
/** \brief Exit point of function Dlt_SetGlobalLogging() */
#define DBG_DLT_SETGLOBALLOGGING_EXIT(a,b)
#endif

#ifndef DBG_DLT_GETGLOBALLOGGING_ENTRY
/** \brief Entry point of function Dlt_GetGlobalLogging() */
#define DBG_DLT_GETGLOBALLOGGING_ENTRY()
#endif

#ifndef DBG_DLT_GETGLOBALLOGGING_EXIT
/** \brief Exit point of function Dlt_GetGlobalLogging() */
#define DBG_DLT_GETGLOBALLOGGING_EXIT(a)
#endif

#ifndef DBG_DLT_SETCOMINTERFACEMAXBANDWIDTH_ENTRY
/** \brief Entry point of function Dlt_SetComInterfaceMaxBandwidth() */
#define DBG_DLT_SETCOMINTERFACEMAXBANDWIDTH_ENTRY(a)
#endif

#ifndef DBG_DLT_SETCOMINTERFACEMAXBANDWIDTH_EXIT
/** \brief Exit point of function Dlt_SetComInterfaceMaxBandwidth() */
#define DBG_DLT_SETCOMINTERFACEMAXBANDWIDTH_EXIT(a,b)
#endif

#ifndef DBG_DLT_GETCOMINTERFACEMAXBANDWIDTH_ENTRY
/** \brief Entry point of function Dlt_GetComInterfaceMaxBandwidth() */
#define DBG_DLT_GETCOMINTERFACEMAXBANDWIDTH_ENTRY()
#endif

#ifndef DBG_DLT_GETCOMINTERFACEMAXBANDWIDTH_EXIT
/** \brief Exit point of function Dlt_GetComInterfaceMaxBandwidth() */
#define DBG_DLT_GETCOMINTERFACEMAXBANDWIDTH_EXIT(a)
#endif

#ifndef DBG_DLT_GETCONTEXTENTRY_ENTRY
/** \brief Entry point of function Dlt_GetContextEntry() */
#define DBG_DLT_GETCONTEXTENTRY_ENTRY(a,b,c)
#endif

#ifndef DBG_DLT_GETCONTEXTENTRY_EXIT
/** \brief Exit point of function Dlt_GetContextEntry() */
#define DBG_DLT_GETCONTEXTENTRY_EXIT(a,b,c,d)
#endif

#ifndef DBG_DLT_CHECKBUFFER_ENTRY
/** \brief Entry point of function Dlt_CheckBuffer() */
#define DBG_DLT_CHECKBUFFER_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_DLT_CHECKBUFFER_EXIT
/** \brief Exit point of function Dlt_CheckBuffer() */
#define DBG_DLT_CHECKBUFFER_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_DLT_CREATESTANDARDHEADER_ENTRY
/** \brief Entry point of function Dlt_CreateStandardHeader() */
#define DBG_DLT_CREATESTANDARDHEADER_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_DLT_CREATESTANDARDHEADER_EXIT
/** \brief Exit point of function Dlt_CreateStandardHeader() */
#define DBG_DLT_CREATESTANDARDHEADER_EXIT(a,b,c,d,e,f,g)
#endif

#ifndef DBG_DLT_CREATEEXTENDEDHEADER_ENTRY
/** \brief Entry point of function Dlt_CreateExtenededHeader() */
#define DBG_DLT_CREATEEXTENDEDHEADER_ENTRY(a,b,c)
#endif

#ifndef DBG_DLT_CREATEEXTENDEDHEADER_EXIT
/** \brief Exit point of function Dlt_CreateExtenededHeader() */
#define DBG_DLT_CREATEEXTENDEDHEADER_EXIT(a,b,c,d)
#endif

#ifndef DBG_DLT_NVMREADRAMBLOCKFROMNVMNATIVECBK_ENTRY
/** \brief Entry point of function Dlt_NvMReadRamBlockFromNvMNativeCbk() */
#define DBG_DLT_NVMREADRAMBLOCKFROMNVMNATIVECBK_ENTRY(a)
#endif

#ifndef DBG_DLT_NVMREADRAMBLOCKFROMNVMNATIVECBK_EXIT
/** \brief Exit point of function Dlt_ReadRamBlockFromNvMNativeCbk() */
#define DBG_DLT_NVMREADRAMBLOCKFROMNVMNATIVECBK_EXIT(a,b)
#endif

#ifndef DBG_DLT_NVMREADRAMBLOCKFROMNVMDATASETCBK_ENTRY
/** \brief Entry point of function Dlt_NvMReadRamBlockFromNvMDataSetCbk() */
#define DBG_DLT_NVMREADRAMBLOCKFROMNVMDATASETCBK_ENTRY(a)
#endif

#ifndef DBG_DLT_NVMREADRAMBLOCKFROMNVMDATASETCBK_EXIT
/** \brief Exit point of function Dlt_NvMReadRamBlockFromNvMDataSetCbk() */
#define DBG_DLT_NVMREADRAMBLOCKFROMNVMDATASETCBK_EXIT(a,b)
#endif

#ifndef DBG_DLT_NVMSINGLEBLOCKCALLBACKDATASET_ENTRY
/** \brief Entry point of function Dlt_NvMSingleBlockCallbackDataSet() */
#define DBG_DLT_NVMSINGLEBLOCKCALLBACKDATASET_ENTRY(a,b)
#endif

#ifndef DBG_DLT_NVMSINGLEBLOCKCALLBACKDATASET_EXIT
/** \brief Exit point of function Dlt_NvMSingleBlockCallbackDataSet() */
#define DBG_DLT_NVMSINGLEBLOCKCALLBACKDATASET_EXIT(a,b,c)
#endif

#ifndef DBG_DLT_INTERNALDISPATCHMESSAGE_ENTRY
/** \brief Entry point of function Dlt_InternalDispatchMessage() */
#define DBG_DLT_INTERNALDISPATCHMESSAGE_ENTRY(a,b,c,d,e,f,g)
#endif

#ifndef DBG_DLT_INTERNALDISPATCHMESSAGE_EXIT
/** \brief Exit point of function Dlt_InternalDispatchMessage() */
#define DBG_DLT_INTERNALDISPATCHMESSAGE_EXIT(a,b,c,d,e,f,g,h)
#endif

#ifndef DBG_DLT_RECEIVEMESSAGEFROMSATELLITE_ENTRY
/** \brief Entry point of function Dlt_ReceiveMessageFromSatellite() */
#define DBG_DLT_RECEIVEMESSAGEFROMSATELLITE_ENTRY()
#endif

#ifndef DBG_DLT_RECEIVEMESSAGEFROMSATELLITE_EXIT
/** \brief Exit point of function Dlt_ReceiveMessageFromSatellite() */
#define DBG_DLT_RECEIVEMESSAGEFROMSATELLITE_EXIT()
#endif

#ifndef DBG_DLT_SENDTABLESTOSATELLITES_ENTRY
/** \brief Entry point of function Dlt_SendTablesToSatellites() */
#define DBG_DLT_SENDTABLESTOSATELLITES_ENTRY()
#endif

#ifndef DBG_DLT_SENDTABLESTOSATELLITES_EXIT
/** \brief Exit point of function Dlt_SendTablesToSatellites() */
#define DBG_DLT_SENDTABLESTOSATELLITES_EXIT()
#endif

#ifndef DBG_DLT_RECEIVETABLESFROMMASTER_ENTRY
/** \brief Entry point of function Dlt_ReceiveTablesFromMaster() */
#define DBG_DLT_RECEIVETABLESFROMMASTER_ENTRY()
#endif

#ifndef DBG_DLT_RECEIVETABLESFROMMASTER_EXIT
/** \brief Exit point of function Dlt_ReceiveTablesFromMaster() */
#define DBG_DLT_RECEIVETABLESFROMMASTER_EXIT()
#endif

#ifndef DBG_DLT_GETCALLBACKIDX_ENTRY
/** \brief Entry point of function Dlt_GetCallbackIdx() */
#define DBG_DLT_GETCALLBACKIDX_ENTRY(a)
#endif

#ifndef DBG_DLT_GETCALLBACKIDX_EXIT
/** \brief Exit point of function Dlt_GetCallbackIdx() */
#define DBG_DLT_GETCALLBACKIDX_EXIT(a,b)
#endif

#ifndef DBG_DLT_UNREGISTER_ALL
/** \brief Injection point of the Dlt_UnregisterContext() API for all registered contexts */
#define DBG_DLT_UNREGISTER_ALL()
#endif

#ifndef DBG_DLT_GETSOFTWAREVERSION_ENTRY
/** \brief Entry point of function Dlt_GetSoftwareVersion() */
#define DBG_DLT_GETSOFTWAREVERSION_ENTRY(a,b)
#endif

#ifndef DBG_DLT_GETSOFTWAREVERSION_EXIT
/** \brief Exit point of function Dlt_GetSoftwareVersion() */
#define DBG_DLT_GETSOFTWAREVERSION_EXIT(a,b,c)
#endif


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef DLT_TRACE_H */
/*==================[end of file]===========================================*/

