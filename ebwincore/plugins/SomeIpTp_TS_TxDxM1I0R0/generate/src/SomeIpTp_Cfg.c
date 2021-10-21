/**
 * \file
 *
 * \brief AUTOSAR SomeIpTp
 *
 * This file contains the implementation of the AUTOSAR
 * module SomeIpTp.
 *
 * \version 1.0.28
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!CODE!]
[!AUTOSPACING!]
/*==================[inclusions]============================================*/

#include <SomeIpTp_Cfg.h> /* Generated pre-compile configuration. */
[!INCLUDE "../include/SomeIpTp_PrecompileMacros.m"!][!//

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/
#define SOMEIPTP_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <SomeIpTp_MemMap.h>

CONST(SomeIpTp_ChannelConfigType, SOMEIPTP_APPL_CONST) SomeIpTp_ChannelElements[SOMEIPTP_NO_OF_CHANNELS] =
{
  [!VAR "ChannelId" = "0"!]
  [!LOOP "as:modconf('SomeIpTp')[1]/SomeIpTpChannel/*"!]
  /* [!"node:name(.)"!] */
  {
    [!CALL "TxTimeToMFCTX", "TxValue" = "node:value(./SomeIpTpNPduSeparationTime)"!]
    [!"$MFCTx"!]U, /* TxSeparationPeriod in terms of TxMainFunction cycles */
    [!CALL "RxTimeoutTimeToMFCRX", "RxValue" = "node:value(./SomeIpTpRxTimeoutTime)"!]
    [!"$MFCRx"!]U, /* RxTimeoutTime in terms of RxMainFunction cycles */
    [!CALL "TxTimeToMFCTX", "TxValue" = "node:value(./SomeIpTpTxConfirmationTimeout)"!]
    [!"$MFCTx"!]U /* TxConfTimeoutTime in terms of TxMainFunction cycles */
    }, /* [!"num:i($ChannelId)"!] */
  
  [!VAR "ChannelId"="$ChannelId + 1"!]
  [!ENDLOOP!]
};

[!IF "num:i($SomeIpTp_TxNsduListSize) > num:i(0)"!][!//
CONST(SomeIpTp_TpTxNSduConfigType, SOMEIPTP_APPL_CONST) SomeIpTp_TxNsduElements[SOMEIPTP_NO_OF_TXNSDUS] =
{
  [!VAR "ChannelId" = "0"!]
  [!LOOP "as:modconf('SomeIpTp')[1]/SomeIpTpChannel/*"!]
  [!VAR "i" = "0"!]
  [!LOOP "./SomeIpTpTxNSdu/*"!][!//
  /* [!"node:name(.)"!] */
  {
#if(TS_SIZE_PduLengthType == 2U) /* PduLengthType = UINT16 */
    MetaData_SomeIpTpTxNSdu_[!"node:value(./SomeIpTpTxNSduHandleId)"!], /* local buffer to contain SOME/IP Header */
    [!CALL "TxNPduToSrcPdu", "value" = "node:path(node:ref(./SomeIpTpTxNPdu/SomeIpTpTxNPduRef))"!][!//
    [!CALL "TxNSduToDestPdu", "value" = "node:path(node:ref(./SomeIpTpTxNSduRef))"!][!//
    [!"$ActualTxNPduLength"!]U, /* Configured Npdu length for segmented msgs */
#elif(TS_SIZE_PduLengthType == 4U) /* PduLengthType = UINT32 */
    [!CALL "TxNPduToSrcPdu", "value" = "node:path(node:ref(./SomeIpTpTxNPdu/SomeIpTpTxNPduRef))"!][!//
    [!CALL "TxNSduToDestPdu", "value" = "node:path(node:ref(./SomeIpTpTxNSduRef))"!][!//
    [!"$ActualTxNPduLength"!]U, /* Configured Npdu length for segmented msgs */
    MetaData_SomeIpTpTxNSdu_[!"node:value(./SomeIpTpTxNSduHandleId)"!], /* local buffer to contain SOME/IP Header */
#else
#error TS_SIZE_PduLengthType is not correctly defined!
#endif /* TS_SIZE_PduLengthType */
    [!"num:i($ChannelId)"!]U, /* Channel Id */
    [!"$DestPduId"!]U, /* PduR DestPduId equivalent for TxNSdu at Txconfirmation and CopyTx */
    [!"$SrcPduId"!]U /* PduR SrcPduId equivalent for TxNPdu at transmission */
  }, /* [!"num:i($i)"!] */
  
  [!VAR "i"="$i + 1"!]
  [!ENDLOOP!]
  [!VAR "ChannelId"="$ChannelId + 1"!]
  [!ENDLOOP!]
};
[!ENDIF!][!//

[!IF "num:i($SomeIpTp_RxNsduListSize) > num:i(0)"!][!//
CONST(SomeIpTp_TpRxNSduConfigType, SOMEIPTP_APPL_CONST) SomeIpTp_RxNsduElements[SOMEIPTP_NO_OF_RXNSDUS] =
{
  [!VAR "ChannelId" = "0"!]
  [!LOOP "as:modconf('SomeIpTp')[1]/SomeIpTpChannel/*"!]
  [!VAR "i" = "0"!]
  [!LOOP "./SomeIpTpRxNSdu/*"!][!//
  /* [!"node:name(.)"!] */
  {
    MetaData_SomeIpTpRxNSdu_[!"node:value(./SomeIpTpRxNPdu/SomeIpTpRxNPduHandleId)"!], /* local buffer to contain SOME/IP Header */
    [!"num:i($ChannelId)"!]U, /* Channel Id */
    [!CALL "RxNSduToDestPdu", "value" = "node:path(node:ref(./SomeIpTpRxSduRef))"!][!//
    [!"$DstPduId"!]U /* PduR DestPduId equivalent for RxNSdu at RxIndication */
  }, /* [!"num:i($i)"!] */
  
  [!VAR "i"="$i + 1"!]
  [!ENDLOOP!]
  [!VAR "ChannelId"="$ChannelId + 1"!]
  [!ENDLOOP!]
};
[!ENDIF!][!//


#define SOMEIPTP_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <SomeIpTp_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/
#define SOMEIPTP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <SomeIpTp_MemMap.h>

[!IF "num:i($SomeIpTp_TxNsduListSize) > num:i(0)"!][!//
/** \brief contains the state machine per TxNSdu data */
VAR(SomeIpTp_TpTxNSduType, SOMEIPTP_VAR) SomeIpTp_TxNSduState[SOMEIPTP_NO_OF_TXNSDUS];
[!ENDIF!][!//

[!IF "num:i($SomeIpTp_RxNsduListSize) > num:i(0)"!][!//
/** \brief contains the state machine per RxNSdu data */
VAR(SomeIpTp_TpRxNSduType, SOMEIPTP_VAR) SomeIpTp_RxNSduState[SOMEIPTP_NO_OF_RXNSDUS];
[!ENDIF!][!//

/*------------------[Tx MetaData local Buffers]-------------------*/
[!IF "num:i($SomeIpTp_TxNsduListSize) > num:i(0)"!][!//
[!VAR "ChannelId" = "0"!]
[!LOOP "as:modconf('SomeIpTp')[1]/SomeIpTpChannel/*"!]
[!VAR "i" = "0"!]
[!LOOP "./SomeIpTpTxNSdu/*"!][!//
VAR(uint8, SOMEIPTP_VAR) MetaData_SomeIpTpTxNSdu_[!"node:value(./SomeIpTpTxNSduHandleId)"!][8U];

[!VAR "i"="$i + 1"!]
[!ENDLOOP!]
[!VAR "ChannelId"="$ChannelId + 1"!]
[!ENDLOOP!]
[!ENDIF!][!//
/*------------------[Rx MetaData local Buffers]-------------------*/
[!IF "num:i($SomeIpTp_RxNsduListSize) > num:i(0)"!][!//
[!VAR "ChannelId" = "0"!]
[!LOOP "as:modconf('SomeIpTp')[1]/SomeIpTpChannel/*"!]
[!VAR "i" = "0"!]
[!LOOP "./SomeIpTpRxNSdu/*"!][!//
VAR(uint8, SOMEIPTP_VAR) MetaData_SomeIpTpRxNSdu_[!"node:value(./SomeIpTpRxNPdu/SomeIpTpRxNPduHandleId)"!][8U];

[!VAR "i"="$i + 1"!]
[!ENDLOOP!]
[!VAR "ChannelId"="$ChannelId + 1"!]
[!ENDLOOP!]
[!ENDIF!][!//

#define SOMEIPTP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <SomeIpTp_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
[!ENDCODE!]
