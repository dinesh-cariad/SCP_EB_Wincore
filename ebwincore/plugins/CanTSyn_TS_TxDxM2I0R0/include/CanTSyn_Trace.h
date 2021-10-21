/**
 * \file
 *
 * \brief AUTOSAR CanTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTSyn.
 *
 * \version 2.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef CANTSYN_TRACE_H
#define CANTSYN_TRACE_H


/*==================[macros]================================================*/
/* ====== CanTSyn public APIs ====== */

#ifndef DBG_CANTSYN_GETVERSIONINFO_ENTRY
/** \brief Entry point of function CanTSyn_GetVersionInfo() */
#define DBG_CANTSYN_GETVERSIONINFO_ENTRY(a)
#endif

#ifndef DBG_CANTSYN_GETVERSIONINFO_EXIT
/** \brief Exit point of function CanTSyn_GetVersionInfo() */
#define DBG_CANTSYN_GETVERSIONINFO_EXIT(a)
#endif

#ifndef DBG_CANTSYN_INIT_ENTRY
/** \brief Entry point of function CanTSyn_Init() */
#define DBG_CANTSYN_INIT_ENTRY(a)
#endif

#ifndef DBG_CANTSYN_INIT_EXIT
/** \brief Exit point of function CanTSyn_Init() */
#define DBG_CANTSYN_INIT_EXIT(a)
#endif

#ifndef DBG_CANTSYN_MAINFUNCTION_ENTRY
/** \brief Entry point of function CanTSyn_MainFunction() */
#define DBG_CANTSYN_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_CANTSYN_MAINFUNCTION_EXIT
/** \brief Exit point of function CanTSyn_MainFunction() */
#define DBG_CANTSYN_MAINFUNCTION_EXIT()
#endif

#ifndef DBG_CANTSYN_SETTRANSMISSIONMODE_ENTRY
/** \brief Entry point of function CanTSyn_SetTransmissionMode() */
#define DBG_CANTSYN_SETTRANSMISSIONMODE_ENTRY(a,b)
#endif

#ifndef DBG_CANTSYN_SETTRANSMISSIONMODE_EXIT
/** \brief Exit point of function CanTSyn_SetTransmissionMode() */
#define DBG_CANTSYN_SETTRANSMISSIONMODE_EXIT(a,b)
#endif

#ifndef DBG_CANTSYN_RXINDICATION_ENTRY
/** \brief Entry point of function CanTSyn_RxIndication() */
#define DBG_CANTSYN_RXINDICATION_ENTRY(a,b)
#endif

#ifndef DBG_CANTSYN_RXINDICATION_EXIT
/** \brief Exit point of function CanTSyn_RxIndication() */
#define DBG_CANTSYN_RXINDICATION_EXIT(a,b)
#endif

#ifndef DBG_CANTSYN_TXCONFIRMATION_ENTRY
/** \brief Entry point of function CanTSyn_TxConfirmation() */
#define DBG_CANTSYN_TXCONFIRMATION_ENTRY(a)
#endif

#ifndef DBG_CANTSYN_TXCONFIRMATION_EXIT
/** \brief Exit point of function CanTSyn_TxConfirmation() */
#define DBG_CANTSYN_TXCONFIRMATION_EXIT(a)
#endif

#ifndef DBG_CANTSYN_FILLTXSYNCFRAME_ENTRY
/** \brief Entry point of function CanTSyn_FillTxSyncFrame() */
#define DBG_CANTSYN_FILLTXSYNCFRAME_ENTRY(a, b)
#endif

#ifndef DBG_CANTSYN_FILLTXSYNCFRAME_EXIT
/** \brief Exit point of function CanTSyn_FillTxSyncFrame() */
#define DBG_CANTSYN_FILLTXSYNCFRAME_EXIT(a,b)
#endif

#ifndef DBG_CANTSYN_FILLTXFUPFRAME_ENTRY
/** \brief Entry point of function CanTSyn_FillTxFupFrame() */
#define DBG_CANTSYN_FILLTXFUPFRAME_ENTRY(a,b,c)
#endif

#ifndef DBG_CANTSYN_FILLTXFUPFRAME_EXIT
/** \brief Exit point of function CanTSyn_FillTxFupFrame() */
#define DBG_CANTSYN_FILLTXFUPFRAME_EXIT(a,b,c)
#endif

#ifndef DBG_CANTSYN_FILLTXOFSFRAME_ENTRY
/** \brief Entry point of function CanTSyn_FillTxOfsFrame() */
#define DBG_CANTSYN_FILLTXOFSFRAME_ENTRY(a,b,c)
#endif

#ifndef DBG_CANTSYN_FILLTXOFSFRAME_EXIT
/** \brief Exit point of function CanTSyn_FillTxOfsFrame() */
#define DBG_CANTSYN_FILLTXOFSFRAME_EXIT(a,b,c)
#endif

#ifndef DBG_CANTSYN_FILLTXOFNSFRAME_ENTRY
/** \brief Entry point of function CanTSyn_FillTxOfnsFrame() */
#define DBG_CANTSYN_FILLTXOFNSFRAME_ENTRY(a,b)
#endif

#ifndef DBG_CANTSYN_FILLTXOFNSFRAME_EXIT
/** \brief Exit point of function CanTSyn_FillTxOfnsFrame() */
#define DBG_CANTSYN_FILLTXOFNSFRAME_EXIT(a,b)
#endif

#ifndef DBG_CANTSYN_TXSYNCRESET_ENTRY
/** \brief Entry point of function CanTSyn_TxSyncReset() */
#define DBG_CANTSYN_TXSYNCRESET_ENTRY(a)
#endif

#ifndef DBG_CANTSYN_TXSYNCRESET_EXIT
/** \brief Exit point of function CanTSyn_TxSyncReset() */
#define DBG_CANTSYN_TXSYNCRESET_EXIT(a)
#endif

#ifndef DBG_CANTSYN_TXOFFSETRESET_ENTRY
/** \brief Entry point of function CanTSyn_TxOffsetReset() */
#define DBG_CANTSYN_TXOFFSETRESET_ENTRY(a)
#endif

#ifndef DBG_CANTSYN_TXOFFSETRESET_EXIT
/** \brief Exit point of function CanTSyn_TxOffsetReset() */
#define DBG_CANTSYN_TXOFFSETRESET_EXIT(a)
#endif

#ifndef DBG_CANTSYN_SENDSYNCMESSAGE_ENTRY
/** \brief Entry point of function CanTSyn_SendSyncMessage() */
#define DBG_CANTSYN_SENDSYNCMESSAGE_ENTRY(a, b)
#endif

#ifndef DBG_CANTSYN_SENDSYNCMESSAGE_EXIT
/** \brief Exit point of function CanTSyn_SendSyncMessage() */
#define DBG_CANTSYN_SENDSYNCMESSAGE_EXIT(a, b)
#endif

#ifndef DBG_CANTSYN_SENDFUPMESSAGE_ENTRY
/** \brief Entry point of function CanTSyn_SendFupMessage() */
#define DBG_CANTSYN_SENDFUPMESSAGE_ENTRY(a)
#endif

#ifndef DBG_CANTSYN_SENDFUPMESSAGE_EXIT
/** \brief Exit point of function CanTSyn_SendFupMessage() */
#define DBG_CANTSYN_SENDFUPMESSAGE_EXIT(a)
#endif

#ifndef DBG_CANTSYN_ISTXSYNCMESSAGEAWAITED_ENTRY
/** \brief Entry point of function CanTSyn_IsTxSYNCMessageAwaited() */
#define DBG_CANTSYN_ISTXSYNCMESSAGEAWAITED_ENTRY(a, b)
#endif

#ifndef DBG_CANTSYN_ISTXSYNCMESSAGEAWAITED_EXIT
/** \brief Exit point of function CanTSyn_IsTxSYNCMessageAwaited() */
#define DBG_CANTSYN_ISTXSYNCMESSAGEAWAITED_EXIT(a, b)
#endif

#ifndef DBG_CANTSYN_CONFIRMSYNCMSG_ENTRY
/** \brief Entry point of function CanTSyn_ConfirmSyncMsg() */
#define DBG_CANTSYN_CONFIRMSYNCMSG_ENTRY(a)
#endif

#ifndef DBG_CANTSYN_CONFIRMSYNCMSG_EXIT
/** \brief Exit point of function CanTSyn_ConfirmSyncMsg() */
#define DBG_CANTSYN_CONFIRMSYNCMSG_EXIT(a)
#endif

#ifndef DBG_CANTSYN_CONFIRMOFSMSG_ENTRY
/** \brief Entry point of function CanTSyn_ConfirmOfsMsg() */
#define DBG_CANTSYN_CONFIRMOFSMSG_ENTRY(a)
#endif

#ifndef DBG_CANTSYN_CONFIRMOFSMSG_EXIT
/** \brief Exit point of function CanTSyn_ConfirmOfsMsg() */
#define DBG_CANTSYN_CONFIRMOFSMSG_EXIT(a)
#endif

#ifndef DBG_CANTSYN_SENDOFSMESSAGE_ENTRY
/** \brief Entry point of function CanTSyn_SendOfsMessage() */
#define DBG_CANTSYN_SENDOFSMESSAGE_ENTRY(a, b, c)
#endif

#ifndef DBG_CANTSYN_SENDOFSMESSAGE_EXIT
/** \brief Exit point of function CanTSyn_SendOfsMessage() */
#define DBG_CANTSYN_SENDOFSMESSAGE_EXIT(a, b, c)
#endif

#ifndef DBG_CANTSYN_ISTXOFSMESSAGEAWAITED_ENTRY
/** \brief Entry point of function CanTSyn_IsTxOFSMessageAwaited() */
#define DBG_CANTSYN_ISTXOFSMESSAGEAWAITED_ENTRY(a, b)
#endif

#ifndef DBG_CANTSYN_ISTXOFSMESSAGEAWAITED_EXIT
/** \brief Exit point of function CanTSyn_IsTxOFSMessageAwaited() */
#define DBG_CANTSYN_ISTXOFSMESSAGEAWAITED_EXIT(a, b)
#endif

#ifndef DBG_CANTSYN_TXPROCESSOFSMSG_ENTRY
/** \brief Entry point of function CanTSyn_TxProcessOfsMsg() */
#define DBG_CANTSYN_TXPROCESSOFSMSG_ENTRY(a)
#endif

#ifndef DBG_CANTSYN_TXPROCESSOFSMSG_EXIT
/** \brief Exit point of function CanTSyn_TxProcessOfsMsg() */
#define DBG_CANTSYN_TXPROCESSOFSMSG_EXIT(a)
#endif

#ifndef DBG_CANTSYN_TXPROCESSSYNCMSG_ENTRY
/** \brief Entry point of function CanTSyn_TxProcessSyncMsg() */
#define DBG_CANTSYN_TXPROCESSSYNCMSG_ENTRY(a)
#endif

#ifndef DBG_CANTSYN_TXPROCESSSYNCMSG_EXIT
/** \brief Exit point of function CanTSyn_TxProcessSyncMsg() */
#define DBG_CANTSYN_TXPROCESSSYNCMSG_EXIT(a)
#endif

#ifndef DBG_CANTSYN_CALCULATECRC_ENTRY
/** \brief Entry point of function CanTSyn_CalculateCRC() */
#define DBG_CANTSYN_CALCULATECRC_ENTRY(a,b,c)
#endif

#ifndef DBG_CANTSYN_CALCULATECRC_EXIT
/** \brief Exit point of function CanTSyn_CalculateCRC() */
#define DBG_CANTSYN_CALCULATECRC_EXIT(a,b,c)
#endif

#ifndef DBG_CANTSYN_CHECKMSGTYPE_ENTRY
/** \brief Entry point of function CanTSyn_CheckMsgType() */
#define DBG_CANTSYN_CHECKMSGTYPE_ENTRY(a,b,c)
#endif

#ifndef DBG_CANTSYN_CHECKMSGTYPE_EXIT
/** \brief Exit point of function CanTSyn_CheckMsgType() */
#define DBG_CANTSYN_CHECKMSGTYPE_EXIT(a,b,c)
#endif

#ifndef DBG_CANTSYN_CHECKMSGCRC_ENTRY
/** \brief Entry point of function CanTSyn_CheckMsgCRC() */
#define DBG_CANTSYN_CHECKMSGCRC_ENTRY(a,b,c)
#endif

#ifndef DBG_CANTSYN_CHECKMSGCRC_EXIT
/** \brief Exit point of function CanTSyn_CheckMsgCRC() */
#define DBG_CANTSYN_CHECKMSGCRC_EXIT(a,b,c)
#endif

#ifndef DBG_CANTSYN_SYNCMSGRXCRCCHECK_ENTRY
/** \brief Entry point of function CanTSyn_SyncMsg_CRC_Check() */
#define DBG_CANTSYN_SYNCMSGRXCRCCHECK_ENTRY(a,b)
#endif

#ifndef DBG_CANTSYN_SYNCMSGRXCRCCHECK_EXIT
/** \brief Exit point of function CanTSyn_SyncMsg_CRC_Check() */
#define DBG_CANTSYN_SYNCMSGRXCRCCHECK_EXIT(a,b)
#endif

#ifndef DBG_CANTSYN_FUPMSGRXCRCCHECK_ENTRY
/** \brief Entry point of function CanTSyn_FupMsg_Rx_CRC_Check() */
#define DBG_CANTSYN_FUPMSGRXCRCCHECK_ENTRY(a,b)
#endif

#ifndef DBG_CANTSYN_FUPMSGRXCRCCHECK_EXIT
/** \brief Exit point of function CanTSyn_FupMsg_Rx_CRC_Check() */
#define DBG_CANTSYN_FUPMSGRXCRCCHECK_EXIT(a,b)
#endif

#ifndef DBG_CANTSYN_OFSMSGRXCRCCHECK_ENTRY
/** \brief Entry point of function CanTSyn_OfsMsg_Rx_CRC_Check() */
#define DBG_CANTSYN_OFSMSGRXCRCCHECK_ENTRY(a,b)
#endif

#ifndef DBG_CANTSYN_OFSMSGRXCRCCHECK_EXIT
/** \brief Exit point of function CanTSyn_OfsMsg_Rx_CRC_Check() */
#define DBG_CANTSYN_OFSMSGRXCRCCHECK_EXIT(a,b)
#endif

#ifndef DBG_CANTSYN_OFNSMSGRXCRCCHECK_ENTRY
/** \brief Entry point of function CanTSyn_OfnsMsg_Rx_CRC_Check() */
#define DBG_CANTSYN_OFSMSGRXCRCCHECK_ENTRY(a,b)
#endif

#ifndef DBG_CANTSYN_OFNSMSGRXCRCCHECK_EXIT
/** \brief Exit point of function CanTSyn_OfnsMsg_Rx_CRC_Check() */
#define DBG_CANTSYN_OFNSMSGRXCRCCHECK_EXIT(a,b)
#endif

#ifndef DBG_CANTSYN_PROCESSRXSYNCFRAME_ENTRY
/** \brief Entry point of function CanTSyn_ProcessRxSyncFrame() */
#define DBG_CANTSYN_PROCESSRXSYNCFRAME_ENTRY(a,b,c,d)
#endif

#ifndef DBG_CANTSYN_PROCESSRXSYNCFRAME_EXIT
/** \brief Exit point of function CanTSyn_ProcessRxSyncFrame() */
#define DBG_CANTSYN_PROCESSRXSYNCFRAME_EXIT(a,b,c,d)
#endif

#ifndef DBG_CANTSYN_PROCESSRXFUPFRAME_ENTRY
/** \brief Entry point of function CanTSyn_ProcessRxFupFrame() */
#define DBG_CANTSYN_PROCESSRXFUPFRAME_ENTRY(a,b,c)
#endif

#ifndef DBG_CANTSYN_PROCESSRXFUPFRAME_EXIT
/** \brief Exit point of function CanTSyn_ProcessRxFupFrame() */
#define DBG_CANTSYN_PROCESSRXFUPFRAME_EXIT(a,b,c)
#endif

#ifndef DBG_CANTSYN_RXSYNCRESET_ENTRY
/** \brief Entry point of function CanTSyn_RxSyncReset() */
#define DBG_CANTSYN_RXSYNCRESET_ENTRY(a)
#endif

#ifndef DBG_CANTSYN_RXSYNCRESET_EXIT
/** \brief Exit point of function CanTSyn_RxSyncReset() */
#define DBG_CANTSYN_RXSYNCRESET_EXIT(a)
#endif

#ifndef DBG_CANTSYN_RXOFFSETRESET_ENTRY
/** \brief Entry point of function CanTSyn_RxOffsetReset() */
#define DBG_CANTSYN_RXOFFSETRESET_ENTRY(a)
#endif

#ifndef DBG_CANTSYN_RXOFFSETRESET_EXIT
/** \brief Exit point of function CanTSyn_RxOffsetReset() */
#define DBG_CANTSYN_RXOFFSETRESET_EXIT(a)
#endif

#ifndef DBG_CANTSYN_PROCESSRXOFSFRAME_ENTRY
/** \brief Entry point of function CanTSyn_ProcessRxOfsFrame() */
#define DBG_CANTSYN_PROCESSRXOFSFRAME_ENTRY(a,b)
#endif

#ifndef DBG_CANTSYN_PROCESSRXOFSFRAME_EXIT
/** \brief Exit point of function CanTSyn_ProcessRxOfsFrame() */
#define DBG_CANTSYN_PROCESSRXOFSFRAME_EXIT(a,b)
#endif

#ifndef DBG_CANTSYN_PROCESSEXTRXOFSFRAME_ENTRY
/** \brief Entry point of function CanTSyn_ProcessExtRxOfsFrame() */
#define DBG_CANTSYN_PROCESSEXTRXOFSFRAME_ENTRY(a,b)
#endif

#ifndef DBG_CANTSYN_PROCESSEXTRXOFSFRAME_EXIT
/** \brief Exit point of function CanTSyn_ProcessExtRxOfsFrame() */
#define DBG_CANTSYN_PROCESSEXTRXOFSFRAME_EXIT(a,b)
#endif

#ifndef DBG_CANTSYN_PROCESSRXOFNSFRAME_ENTRY
/** \brief Entry point of function CanTSyn_ProcessRxOfnsFrame() */
#define DBG_CANTSYN_PROCESSRXOFNSFRAME_ENTRY(a,b,c)
#endif

#ifndef DBG_CANTSYN_PROCESSRXOFNSFRAME_EXIT
/** \brief Exit point of function CanTSyn_ProcessRxOfnsFrame() */
#define DBG_CANTSYN_PROCESSRXOFNSFRAME_EXIT(a,b,c)
#endif

#ifndef DBG_CANTSYN_PROCESSRXFRAME_ENTRY
/** \brief Entry point of function CanTSyn_ProcessRxFrame() */
#define DBG_CANTSYN_PROCESSRXFRAME_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_CANTSYN_PROCESSRXFRAME_EXIT
/** \brief Exit point of function CanTSyn_ProcessRxFrame() */
#define DBG_CANTSYN_PROCESSRXFRAME_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_CANTSYN_CHECKCRC_ENTRY
/** \brief Entry point of function CanTSyn_CheckCRC() */
#define DBG_CANTSYN_CHECKCRC_ENTRY(a,b,c)
#endif

#ifndef DBG_CANTSYN_CHECKCRC_EXIT
/** \brief Exit point of function CanTSyn_CheckCRC() */
#define DBG_CANTSYN_CHECKCRC_EXIT(a,b,c)
#endif

#ifndef DBG_CANTSYN_GETRXSTATUS_ENTRY
/** \brief Entry point of function CanTSyn_GetRxStatus() */
#define DBG_CANTSYN_GETRXSTATUS_ENTRY(a,b)
#endif

#ifndef DBG_CANTSYN_GETRXSTATUS_EXIT
/** \brief Exit point of function CanTSyn_GetRxStatus() */
#define DBG_CANTSYN_GETRXSTATUS_EXIT(a,b)
#endif

#ifndef DBG_CANTSYN_RXRESET_ENTRY
/** \brief Entry point of function CanTSyn_RxReset() */
#define DBG_CANTSYN_RXRESET_ENTRY(a)
#endif

#ifndef DBG_CANTSYN_RXRESET_EXIT
/** \brief Exit point of function CanTSyn_RxReset() */
#define DBG_CANTSYN_RXRESET_EXIT(a)
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* ifndef CANTSYN_TRACE_H */


