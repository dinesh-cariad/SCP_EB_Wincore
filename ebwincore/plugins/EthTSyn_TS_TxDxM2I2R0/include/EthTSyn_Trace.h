/**
 * \file
 *
 * \brief AUTOSAR EthTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module EthTSyn.
 *
 * \version 2.2.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef ETHTSYN_TRACE_H
#define ETHTSYN_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h> /* AUTOSAR standard types */

/*==================[macros]================================================*/

/* ====== EthTSyn public APIs ====== */
#ifndef DBG_ETHTSYN_INIT_ENTRY
/** \brief Entry point of function EthTSyn_Init() */
#define DBG_ETHTSYN_INIT_ENTRY(a)
#endif

#ifndef DBG_ETHTSYN_INIT_EXIT
/** \brief Exit point of function EthTSyn_Init() */
#define DBG_ETHTSYN_INIT_EXIT(a)
#endif

#ifndef DBG_ETHTSYN_MAINFUNCTION_ENTRY
/** \brief Entry point of function EthTSyn_MainFunction() */
#define DBG_ETHTSYN_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_ETHTSYN_MAINFUNCTION_EXIT
/** \brief Exit point of function EthTSyn_MainFunction() */
#define DBG_ETHTSYN_MAINFUNCTION_EXIT()
#endif

#ifndef DBG_ETHTSYN_GETVERSIONINFO_ENTRY
/** \brief Entry point of function EthTSyn_GetVersionInfo() */
#define DBG_ETHTSYN_GETVERSIONINFO_ENTRY(a)
#endif

#ifndef DBG_ETHTSYN_GETVERSIONINFO_EXIT
/** \brief Exit point of function EthTSyn_GetVersionInfo() */
#define DBG_ETHTSYN_GETVERSIONINFO_EXIT(a)
#endif

#ifndef DBG_ETHTSYN_RXINDICATION_ENTRY
/** \brief Entry point of function EthTSyn_RxIndication() */
#define DBG_ETHTSYN_RXINDICATION_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_ETHTSYN_RXINDICATION_EXIT
/** \brief Exit point of function EthTSyn_RxIndication() */
#define DBG_ETHTSYN_RXINDICATION_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_ETHTSYN_TXCONFIRMATION_ENTRY
/** \brief Entry point of function EthTSyn_TxConfirmation() */
#define DBG_ETHTSYN_TXCONFIRMATION_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_TXCONFIRMATION_EXIT
/** \brief Exit point of function EthTSyn_TxConfirmation() */
#define DBG_ETHTSYN_TXCONFIRMATION_EXIT(a,b)
#endif

#ifndef DBG_ETHTSYN_TRCVLINKSTATECHG_ENTRY
/** \brief Entry point of function EthTSyn_TrcvLinkStateChg() */
#define DBG_ETHTSYN_TRCVLINKSTATECHG_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_TRCVLINKSTATECHG_EXIT
/** \brief Exit point of function EthTSyn_TrcvLinkStateChg() */
#define DBG_ETHTSYN_TRCVLINKSTATECHG_EXIT(a,b)
#endif

#ifndef DBG_ETHTSYN_SETTRANSMISSIONMODE_ENTRY
/** \brief Entry point of function EthTSyn_SetTransmissionMode() */
#define DBG_ETHTSYN_SETTRANSMISSIONMODE_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_SETTRANSMISSIONMODE_EXIT
/** \brief Exit point of function EthTSyn_SetTransmissionMode() */
#define DBG_ETHTSYN_SETTRANSMISSIONMODE_EXIT(a,b)
#endif

/* ====== EthTSyn internal functions ====== */

#ifndef DBG_ETHTSYN_TRANSLATEETHIFCTRLIDX_ENTRY
/** \brief Entry point of function EthTSyn_TranslateEthIfCtrlIdx() */
#define DBG_ETHTSYN_TRANSLATEETHIFCTRLIDX_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_TRANSLATEETHIFCTRLIDX_EXIT
/** \brief Exit point of function EthTSyn_TranslateEthIfCtrlIdx() */
#define DBG_ETHTSYN_TRANSLATEETHIFCTRLIDX_EXIT(a,b,c)
#endif

#ifndef DBG_ETHTSYN_TRANSMITSYNCFRAME_ENTRY
/** \brief Entry point of function EthTSyn_TransmitSyncFrame() */
#define DBG_ETHTSYN_TRANSMITSYNCFRAME_ENTRY(a)
#endif

#ifndef DBG_ETHTSYN_TRANSMITSYNCFRAME_EXIT
/** \brief Exit point of function EthTSyn_TransmitSyncFrame() */
#define DBG_ETHTSYN_TRANSMITSYNCFRAME_EXIT(a)
#endif
#if(ETHTSYN_SWITCH_USED == STD_ON)
#ifndef DBG_ETHTSYN_INC02_TRANSMITSWTSYNCFRAMEPERPORT_ENTRY
/** \brief Entry point of function EthTSyn_Inc02_TransmitSwtSyncFramePerPort() */
#define DBG_ETHTSYN_INC02_TRANSMITSWTSYNCFRAMEPERPORT_ENTRY(a)
#endif

#ifndef DBG_ETHTSYN_INC02_TRANSMITSWTSYNCFRAMEPERPORT_EXIT
/** \brief Exit point of function EthTSyn_Inc02_TransmitSwtSyncFramePerPort() */
#define DBG_ETHTSYN_INC02_TRANSMITSWTSYNCFRAMEPERPORT_EXIT(a)
#endif

#ifndef DBG_ETHTSYN_TRANSMITSWTSYNCHOSTPORTFRAME_ENTRY
/** \brief Entry point of function EthTSyn_Inc02_TransmitSwtOnHostPortSyncFrame() */
#define DBG_ETHTSYN_TRANSMITSWTSYNCHOSTPORTFRAME_ENTRY(a)
#endif

#ifndef DBG_ETHTSYN_TRANSMITSWTSYNCHOSTPORTFRAME_EXIT
/** \brief Exit point of function EthTSyn_Inc02_TransmitSwtOnHostPortSyncFrame() */
#define DBG_ETHTSYN_TRANSMITSWTSYNCHOSTPORTFRAME_EXIT(a)
#endif

#ifndef DBG_ETHTSYN_INC02_TRANSMITSYNCSWTFUPFRAMEPERPORT_ENTRY
/** \brief Entry point of function EthTSyn_TransmitSyncSwtFUpFrame() */
#define DBG_ETHTSYN_INC02_TRANSMITSYNCSWTFUPFRAMEPERPORT_ENTRY(a)
#endif

#ifndef DBG_ETHTSYN_INC02_TRANSMITSYNCSWTFUPFRAMEPERPORT_EXIT
/** \brief Exit point of function EthTSyn_TransmitSyncSwtFUpFrame() */
#define DBG_ETHTSYN_INC02_TRANSMITSYNCSWTFUPFRAMEPERPORT_EXIT(a)
#endif

#ifndef DBG_ETHTSYN_INC02_TRANSMITSWTPDELAYREQFRAMEPERPORT_ENTRY
/** \brief Entry point of function EthTSyn_Inc02_TransmitSwtPdelayReqFramePerPort() */
#define DBG_ETHTSYN_INC02_TRANSMITSWTPDELAYREQFRAMEPERPORT_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_INC02_TRANSMITSWTPDELAYREQFRAMEPERPORT_EXIT
/** \brief Exit point of function EthTSyn_Inc02_TransmitSwtPdelayReqFramePerPort() */
#define DBG_ETHTSYN_INC02_TRANSMITSWTPDELAYREQFRAMEPERPORT_EXIT(a,b)
#endif

#ifndef DBG_ETHTSYN_INC02_TRANSMITSWTPDELAYRESPFRAMEPERPORT_ENTRY
/** \brief Entry point of function EthTSyn_Inc02_TransmitSwtPdelayRespFramePerPort() */
#define DBG_ETHTSYN_INC02_TRANSMITSWTPDELAYRESPFRAMEPERPORT_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_INC02_TRANSMITSWTPDELAYRESPFRAMEPERPORT_EXIT
/** \brief Exit point of function EthTSyn_Inc02_TransmitSwtPdelayRespFramePerPort() */
#define DBG_ETHTSYN_INC02_TRANSMITSWTPDELAYRESPFRAMEPERPORT_EXIT(a,b)
#endif

#ifndef DBG_ETHTSYN_INC02_TRANSMITSWTPDELAYRESPFUPFRAMEPERPORT_ENTRY
/** \brief Entry point of function EthTSyn_Inc02_TransmitSwtPdelayRespFupFramePerPort() */
#define DBG_ETHTSYN_INC02_TRANSMITSWTPDELAYRESPFUPFRAMEPERPORT_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_INC02_TRANSMITSWTPDELAYRESPFUPFRAMEPERPORT_EXIT
/** \brief Exit point of function EthTSyn_Inc02_TransmitSwtPdelayRespFupFramePerPort() */
#define DBG_ETHTSYN_INC02_TRANSMITSWTPDELAYRESPFUPFRAMEPERPORT_EXIT(a,b)
#endif

#ifndef DBG_ETHTSYN_INC02_GETSWTTXBUFFER_ENTRY
/** \brief Entry point of function EthTSyn_Inc02_GetSwtTxBuffer() */
#define DBG_ETHTSYN_INC02_GETSWTTXBUFFER_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_ETHTSYN_INC02_GETSWTTXBUFFER_EXIT
/** \brief Exit point of function EthTSyn_Inc02_GetSwtTxBuffer() */
#define DBG_ETHTSYN_INC02_GETSWTTXBUFFER_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_ETHTSYN_INC02_TRANSMITSWTFRAME_ENTRY
/** \brief Entry point of function EthTSyn_Inc02_TransmitSwtFrame() */
#define DBG_ETHTSYN_INC02_TRANSMITSWTFRAME_ENTRY(a,b,c,d,e,f,g)
#endif

#ifndef DBG_ETHTSYN_INC02_TRANSMITSWTFRAME_EXIT
/** \brief Exit point of function EthTSyn_Inc02_TransmitSwtFrame() */
#define DBG_ETHTSYN_INC02_TRANSMITSWTFRAME_EXIT(a,b,c,d,e,f,g,h)
#endif

#endif/* (ETHTSYN_SWITCH_USED == STD_ON) */

#ifndef DBG_ETHTSYN_TRANSMITSYNCFUPFRAME_ENTRY
/** \brief Entry point of function EthTSyn_TransmitSyncFUpFrame() */
#define DBG_ETHTSYN_TRANSMITSYNCFUPFRAME_ENTRY(a)
#endif

#ifndef DBG_ETHTSYN_TRANSMITSYNCFUPFRAME_EXIT
/** \brief Exit point of function EthTSyn_TransmitSyncFUpFrame() */
#define DBG_ETHTSYN_TRANSMITSYNCFUPFRAME_EXIT(a)
#endif

#ifndef DBG_ETHTSYN_TRASNMITANNOUNCEFRAME_ENTRY
/** \brief Entry point of function EthTSyn_TransmitAnnounceFrame() */
#define DBG_ETHTSYN_TRASNMITANNOUNCEFRAME_ENTRY(a)
#endif

#ifndef DBG_ETHTSYN_TRASNMITANNOUNCEFRAME_EXIT
/** \brief Exit point of function EthTSyn_TransmitAnnounceFrame() */
#define DBG_ETHTSYN_TRASNMITANNOUNCEFRAME_EXIT(a)
#endif

#ifndef DBG_ETHTSYN_TRANSMITPDELAYREQFRAME_ENTRY
/** \brief Entry point of function EthTSyn_TransmitPdelayReqFrame() */
#define DBG_ETHTSYN_TRANSMITPDELAYREQFRAME_ENTRY(a)
#endif

#ifndef DBG_ETHTSYN_TRANSMITPDELAYREQFRAME_EXIT
/** \brief Exit point of function EthTSyn_TransmitPdelayReqFrame() */
#define DBG_ETHTSYN_TRANSMITPDELAYREQFRAME_EXIT(a)
#endif

#ifndef DBG_ETHTSYN_TRANSMITPDELAYRESPFRAME_ENTRY
/** \brief Entry point of function EthTSyn_TransmitPdelayRespFrame() */
#define DBG_ETHTSYN_TRANSMITPDELAYRESPFRAME_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_TRANSMITPDELAYRESPFRAME_EXIT
/** \brief Exit point of function EthTSyn_TransmitPdelayRespFrame() */
#define DBG_ETHTSYN_TRANSMITPDELAYRESPFRAME_EXIT(a,b)
#endif

#ifndef DBG_ETHTSYN_TRANSMITPDELAYRESPFUPFRAME_ENTRY
/** \brief Entry point of function EthTSyn_TransmitPdelayRespFUpFrame() */
#define DBG_ETHTSYN_TRANSMITPDELAYRESPFUPFRAME_ENTRY(a)
#endif

#ifndef DBG_ETHTSYN_TRANSMITPDELAYRESPFUPFRAME_EXIT
/** \brief Exit point of function EthTSyn_TransmitPdelayRespFUpFrame() */
#define DBG_ETHTSYN_TRANSMITPDELAYRESPFUPFRAME_EXIT(a)
#endif

#ifndef DBG_ETHTSYN_GETTXBUFFER_ENTRY
/** \brief Entry point of function EthTSyn_GetTxBuffer() */
#define DBG_ETHTSYN_GETTXBUFFER_ENTRY(a,b,c,d)
#endif

#ifndef DBG_ETHTSYN_GETTXBUFFER_EXIT
/** \brief Exit point of function EthTSyn_GetTxBuffer() */
#define DBG_ETHTSYN_GETTXBUFFER_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_ETHTSYN_FILLPTPHEADER_ENTRY
/** \brief Entry point of function EthTSyn_FillPTPHeader() */
#define DBG_ETHTSYN_FILLPTPHEADER_ENTRY(a,b,c)
#endif

#ifndef DBG_ETHTSYN_FILLPTPHEADER_EXIT
/** \brief Exit point of function EthTSyn_FillPTPHeader() */
#define DBG_ETHTSYN_FILLPTPHEADER_EXIT(a,b,c)
#endif

#ifndef DBG_ETHTSYN_FILLFUPTLV_ENTRY
/** \brief Entry point of function EthTSyn_FillFUpTLV() */
#define DBG_ETHTSYN_FILLFUPTLV_ENTRY(a)
#endif

#ifndef DBG_ETHTSYN_FILLFUPTLV_EXIT
/** \brief Exit point of function EthTSyn_FillFUpTLV() */
#define DBG_ETHTSYN_FILLFUPTLV_EXIT(a)
#endif

#ifndef DBG_ETHTSYN_FILLANNOUNCEPAYLOAD_ENTRY
/** \brief Entry point of function EthTSyn_FillAnnouncePayload() */
#define DBG_ETHTSYN_FILLANNOUNCEPAYLOAD_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_FILLANNOUNCEPAYLOAD_EXIT
/** \brief Exit point of function EthTSyn_FillAnnouncePayload() */
#define DBG_ETHTSYN_FILLANNOUNCEPAYLOAD_EXIT(a,b)
#endif

#ifndef DBG_ETHTSYN_TRANSMITFRAME_ENTRY
/** \brief Entry point of function EthTSyn_TransmitFrame() */
#define DBG_ETHTSYN_TRANSMITFRAME_ENTRY(a,b,c)
#endif

#ifndef DBG_ETHTSYN_TRANSMITFRAME_EXIT
/** \brief Exit point of function EthTSyn_TransmitFrame() */
#define DBG_ETHTSYN_TRANSMITFRAME_EXIT(a,b,c,d)
#endif

#ifndef DBG_ETHTSYN_TIMESTAMPTOARRAY_ENTRY
/** \brief Entry point of function EthTSyn_TimeStampToArray() */
#define DBG_ETHTSYN_TIMESTAMPTOARRAY_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_TIMESTAMPTOARRAY_EXIT
/** \brief Exit point of function EthTSyn_TimeStampToArray() */
#define DBG_ETHTSYN_TIMESTAMPTOARRAY_EXIT(a,b)
#endif

#ifndef DBG_ETHTSYN_ARRAYTOTIMESTAMP_ENTRY
/** \brief Entry point of function EthTSyn_ArrayToTimeStamp() */
#define DBG_ETHTSYN_ARRAYTOTIMESTAMP_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_ARRAYTOTIMESTAMP_EXIT
/** \brief Exit point of function EthTSyn_ArrayToTimeStamp() */
#define DBG_ETHTSYN_ARRAYTOTIMESTAMP_EXIT(a,b)
#endif

#ifndef DBG_ETHTSYN_PROCESSRXSYNCFRAME_ENTRY
/** \brief Entry point of function EthTSyn_ProcessRxSyncFrame() */
#define DBG_ETHTSYN_PROCESSRXSYNCFRAME_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_PROCESSRXSYNCFRAME_EXIT
/** \brief Exit point of function EthTSyn_ProcessRxSyncFrame() */
#define DBG_ETHTSYN_PROCESSRXSYNCFRAME_EXIT(a,b)
#endif

#ifndef DBG_ETHTSYN_CHECKSC_ENTRY
/** \brief Entry point of function EthTSyn_CheckSC() */
#define DBG_ETHTSYN_CHECKSC_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_CHECKSC_EXIT
/** \brief Exit point of function EthTSyn_CheckSC() */
#define DBG_ETHTSYN_CHECKSC_EXIT(a,b,c)
#endif


#ifndef DBG_ETHTSYN_PROCESSRXSYNCFUPFRAME_ENTRY
/** \brief Entry point of function EthTSyn_ProcessRxSynFUpFrame() */
#define DBG_ETHTSYN_PROCESSRXSYNCFUPFRAME_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_PROCESSRXSYNCFUPFRAME_EXIT
/** \brief Exit point of function EthTSyn_ProcessRxSynFUpFrame() */
#define DBG_ETHTSYN_PROCESSRXSYNCFUPFRAME_EXIT(a,b)
#endif

#ifndef DBG_ETHTSYN_PROCESSRXPDELAYREQFRAME_ENTRY
/** \brief Entry point of function EthTSyn_ProcessRxPdelayReqFrame() */
#define DBG_ETHTSYN_PROCESSRXPDELAYREQFRAME_ENTRY(a,b,c)
#endif

#ifndef DBG_ETHTSYN_PROCESSRXPDELAYREQFRAME_EXIT
/** \brief Entry point of function EthTSyn_ProcessRxPdelayReqFrame() */
#define DBG_ETHTSYN_PROCESSRXPDELAYREQFRAME_EXIT(a,b,c)
#endif

#ifndef DBG_ETHTSYN_PROCESSPDELAYRESPFRAME_ENTRY
/** \brief Entry point of function EthTSyn_ProcessRxPdelayRespFrame() */
#define DBG_ETHTSYN_PROCESSPDELAYRESPFRAME_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_PROCESSPDELAYRESPFRAME_EXIT
/** \brief Exit point of function EthTSyn_ProcessRxPdelayRespFrame() */
#define DBG_ETHTSYN_PROCESSPDELAYRESPFRAME_EXIT(a,b)
#endif

#ifndef DBG_ETHTSYN_PROCESSPDELAYRESPFUPFRAME_ENTRY
/** \brief Entry point of function EthTSyn_ProcessRxPdelayRespFUpFrame() */
#define DBG_ETHTSYN_PROCESSPDELAYRESPFUPFRAME_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_PROCESSPDELAYRESPFUPFRAME_EXIT
/** \brief Exit point of function EthTSyn_ProcessRxPdelayRespFUpFrame() */
#define DBG_ETHTSYN_PROCESSPDELAYRESPFUPFRAME_EXIT(a,b)
#endif

#ifndef DBG_ETHTSYN_GETINGRESSTIMESTAMP_ENTRY
/** \brief Entry point of function EthTSyn_GetIngressTimeStamp() */
#define DBG_ETHTSYN_GETINGRESSTIMESTAMP_ENTRY(a,b,c)
#endif

#ifndef DBG_ETHTSYN_GETINGRESSTIMESTAMP_EXIT
/** \brief Exit point of function EthTSyn_GetIngressTimeStamp() */
#define DBG_ETHTSYN_GETINGRESSTIMESTAMP_EXIT(a,b,c,d)
#endif

#ifndef DBG_ETHTSYN_GETEGRESSTIMESTAMP_ENTRY
/** \brief Entry point of function EthTSyn_GetEgressTimeStamp() */
#define DBG_ETHTSYN_GETEGRESSTIMESTAMP_ENTRY(a,b,c)
#endif

#ifndef DBG_ETHTSYN_GETEGRESSTIMESTAMP_EXIT
/** \brief Exit point of function EthTSyn_GetEgressTimeStamp() */
#define DBG_ETHTSYN_GETEGRESSTIMESTAMP_EXIT(a,b,c,d)
#endif

#if(ETHTSYN_HW_TIMESTAMP_SUPPORT != STD_ON)
#ifndef DBG_ETHTSYN_GETTIMEFROMSTBM_ENTRY
/** \brief Entry point of function EthTSyn_GetTimeFromStbM() */
#define DBG_ETHTSYN_GETTIMEFROMSTBM_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_GETTIMEFROMSTBM_EXIT
/** \brief Exit point of function EthTSyn_GetTimeFromStbM() */
#define DBG_ETHTSYN_GETTIMEFROMSTBM_EXIT(a,b,c)
#endif
#endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT != STD_ON */

#ifndef DBG_ETHTSYN_GETHEADERSEQUNUMBER_ENTRY
/** \brief Entry point of function EthTSyn_GetPTPHeaderSequNumber() */
#define DBG_ETHTSYN_GETHEADERSEQUNUMBER_ENTRY(a)
#endif

#ifndef DBG_ETHTSYN_GETHEADERSEQUNUMBER_EXIT
/** \brief Exit point of function EthTSyn_GetPTPHeaderSequNumber() */
#define DBG_ETHTSYN_GETHEADERSEQUNUMBER_EXIT(a,b)
#endif

#ifndef DBG_ETHTSYN_ADDTIMESTAMPS_ENTRY
/** \brief Entry point of function EthTSyn_AddTimeStamps() */
#define DBG_ETHTSYN_ADDTIMESTAMPS_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_ADDTIMESTAMPS_EXIT
/** \brief Exit point of function EthTSyn_AddTimeStamps() */
#define DBG_ETHTSYN_ADDTIMESTAMPS_EXIT(a,b)
#endif

#ifndef DBG_ETHTSYN_SUBTIMESTAMPS_ENTRY
/** \brief Entry point of function EthTSyn_SubTimeStamps() */
#define DBG_ETHTSYN_SUBTIMESTAMPS_ENTRY(a,b,c)
#endif

#ifndef DBG_ETHTSYN_SUBTIMESTAMPS_EXIT
/** \brief Exit point of function EthTSyn_SubTimeStamps() */
#define DBG_ETHTSYN_SUBTIMESTAMPS_EXIT(a,b,c)
#endif

#ifndef DBG_ETHTSYN_CALCULATEORIGINTIMESTAMP_ENTRY
/** \brief Entry point of function EthTSyn_CalculateOriginTimeStamp() */
#define DBG_ETHTSYN_CALCULATEORIGINTIMESTAMP_ENTRY(a)
#endif

#ifndef DBG_ETHTSYN_CALCULATEORIGINTIMESTAMP_EXIT
/** \brief Exit point of function EthTSyn_CalculateOriginTimeStamp() */
#define DBG_ETHTSYN_CALCULATEORIGINTIMESTAMP_EXIT(a,b)
#endif

#ifndef DBG_ETHTSYN_TIMESTAMP_ISGREATEROREQUAL_ENTRY
/** \brief Entry point of function EthTSyn_TimeStamp_IsGreaterOrEqual() */
#define DBG_ETHTSYN_TIMESTAMP_ISGREATEROREQUAL_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_TIMESTAMP_ISGREATEROREQUAL_EXIT
/** \brief Exit point of function EthTSyn_TimeStamp_IsGreaterOrEqual() */
#define DBG_ETHTSYN_TIMESTAMP_ISGREATEROREQUAL_EXIT(a,b,c)
#endif

#ifndef DBG_ETHTSYN_INT01_ADDVIRTUALLOCALTIMES_ENTRY
/** \brief Entry point of function EthTSyn_Int01_AddVirtualLocalTimes() */
#define DBG_ETHTSYN_INT01_ADDVIRTUALLOCALTIMES_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_INT01_ADDVIRTUALLOCALTIMES_EXIT
/** \brief Exit point of function EthTSyn_Int01_AddVirtualLocalTimes() */
#define DBG_ETHTSYN_INT01_ADDVIRTUALLOCALTIMES_EXIT(a,b,c)
#endif

#ifndef DBG_ETHTSYN_INT01_SUBVIRTUALLOCALTIMES_ENTRY
/** \brief Entry point of function EthTSyn_SubVirtualLocalTimes() */
#define DBG_ETHTSYN_INT01_SUBVIRTUALLOCALTIMES_ENTRY(a,b,c)
#endif

#ifndef DBG_ETHTSYN_INT01_SUBVIRTUALLOCALTIMES_EXIT
/** \brief Exit point of function EthTSyn_Int01_SubVirtualLocalTimes() */
#define DBG_ETHTSYN_INT01_SUBVIRTUALLOCALTIMES_EXIT(a,b,c,d)
#endif

#ifndef DBG_ETHTSYN_INT01_VIRTUALLOCALTIME_ISGREATEROREQUAL_ENTRY
/** \brief Entry point of function EthTSyn_Int01_VirtualLocalTime_IsGreaterOrEqual() */
#define DBG_ETHTSYN_INT01_VIRTUALLOCALTIME_ISGREATEROREQUAL_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_INT01_VIRTUALLOCALTIME_ISGREATEROREQUAL_EXIT
/** \brief Exit point of function EthTSyn_VirtualLocalTime_IsGreaterOrEqual() */
#define DBG_ETHTSYN_INT01_VIRTUALLOCALTIME_ISGREATEROREQUAL_EXIT(a,b,c)
#endif

#ifndef DBG_ETHTSYN_INT01_CONVERTETHTIMETOVIRTUALTIME_ENTRY
/** \brief Entry point of function EthTSyn_Int01_ConvertEthTimeToVirtualTime() */
#define DBG_ETHTSYN_INT01_CONVERTETHTIMETOVIRTUALTIME_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_INT01_CONVERTETHTIMETOVIRTUALTIME_EXIT
/** \brief Exit point of function EthTSyn_Int01_ConvertEthTimeToVirtualTime() */
#define DBG_ETHTSYN_INT01_CONVERTETHTIMETOVIRTUALTIME_EXIT(a,b,c)
#endif

#ifndef DBG_ETHTSYN_INT01_CONVERTVIRTUALTIMETOETHTIME_ENTRY
/** \brief Entry point of function EthTSyn_Int01_ConvertVirtualTimeToEthTime() */
#define DBG_ETHTSYN_INT01_CONVERTVIRTUALTIMETOETHTIME_ENTRY(a,b)
#endif

#ifndef DBG_ETHTSYN_INT01_CONVERTVIRTUALTIMETOETHTIME_EXIT
/** \brief Exit point of function EthTSyn_Int01_ConvertVirtualTimeToEthTime() */
#define DBG_ETHTSYN_INT01_CONVERTVIRTUALTIMETOETHTIME_EXIT(a,b)
#endif


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef ETHTSYN_TRACE_H */
/*==================[end of file]===========================================*/
