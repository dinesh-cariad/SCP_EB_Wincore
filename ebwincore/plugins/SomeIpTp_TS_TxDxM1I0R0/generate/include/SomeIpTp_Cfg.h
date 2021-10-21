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
#ifndef SOMEIPTP_CFG_H
#define SOMEIPTP_CFG_H

/*==================[includes]==============================================*/
 
 #include <SomeIpTp_Types.h>
 [!INCLUDE "../include/SomeIpTp_PrecompileMacros.m"!][!//
/*==================[macros]================================================*/
/** \brief Support for Development error reporting
  ** This macro defines whether the Det_ReportError() is provided or not.
  ** STD_ON    Function is provided
  ** STD_OFF   Function is not provided
*/
/* !LINKSTO SomeIpTp.ASR43.ECUC_SomeIpTp_00004,1 */
#if (defined SOMEIPTP_DEV_ERROR_DETECT)
#error SOMEIPTP_DEV_ERROR_DETECT already defined!
#endif /* #if (defined SOMEIPTP_DEV_ERROR_DETECT)*/
#define SOMEIPTP_DEV_ERROR_DETECT        STD_[!IF "SomeIpTpGeneral/SomeIpTpDevErrorDetect = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


/** \brief Support of SomeIpTp_GetVersionInfo()
  ** This macro defines whether the SomeIpTp_GetVersionInfo is provided or not.
  ** STD_ON    Function is provided
  ** STD_OFF   Function is not provided
*/
/* !LINKSTO SomeIpTp.ASR43.ECUC_SomeIpTp_00019,1 */
#if (defined SOMEIPTP_VERSION_INFO_API)
#error SOMEIPTP_VERSION_INFO_API already defined!
#endif /* #if (defined SOMEIPTP_VERSION_INFO_API)*/
#define SOMEIPTP_VERSION_INFO_API       STD_[!IF "SomeIpTpGeneral/SomeIpTpVersionInfoApi = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


/** \brief Number of configured Channels
  ** This macro defines number of configured Channels
*/
#if (defined SOMEIPTP_NO_OF_CHANNELS)
#error SOMEIPTP_NO_OF_CHANNELS already defined!
#endif /* #if (defined SOMEIPTP_NO_OF_CHANNELS)*/
#define SOMEIPTP_NO_OF_CHANNELS       [!"num:i($SomeIpTp_ChannelListSize)"!]U

/** \brief Number of configured TxNSdus
  ** This macro defines number of configured TxNSdus
*/
#if (defined SOMEIPTP_NO_OF_TXNSDUS)
#error SOMEIPTP_NO_OF_TXNSDUS already defined!
#endif /* #if (defined SOMEIPTP_NO_OF_TXNSDUS)*/
#define SOMEIPTP_NO_OF_TXNSDUS       [!"num:i($SomeIpTp_TxNsduListSize)"!]U

/** \brief Number of configured RxNSdus
  ** This macro defines number of configured RxNSdus
*/
#if (defined SOMEIPTP_NO_OF_RXNSDUS)
#error SOMEIPTP_NO_OF_RXNSDUS already defined!
#endif /* #if (defined SOMEIPTP_NO_OF_RXNSDUS)*/
#define SOMEIPTP_NO_OF_RXNSDUS       [!"num:i($SomeIpTp_RxNsduListSize)"!]U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#define SOMEIPTP_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <SomeIpTp_MemMap.h>

/** \brief SomeIpTp Channel configuration
 **
 ** This is the SomeIpTp Channel configuration.
 ** Please note that the name of this element is configuration dependent.
 */

extern CONST(SomeIpTp_ChannelConfigType, SOMEIPTP_APPL_CONST) SomeIpTp_ChannelElements[SOMEIPTP_NO_OF_CHANNELS];

[!IF "num:i($SomeIpTp_TxNsduListSize) > num:i(0)"!][!//
/** \brief SomeIpTp TxNSdus configuration
 **
 ** This is the SomeIpTp TxNSdus configuration.
 ** Please note that the name of this element is configuration dependent.
 */
extern CONST(SomeIpTp_TpTxNSduConfigType, SOMEIPTP_APPL_CONST) SomeIpTp_TxNsduElements[SOMEIPTP_NO_OF_TXNSDUS];
[!ENDIF!][!//

[!IF "num:i($SomeIpTp_RxNsduListSize) > num:i(0)"!][!//
/** \brief SomeIpTp RxNSdus configuration
 **
 ** This is the SomeIpTp RxNSdus configuration.
 ** Please note that the name of this element is configuration dependent.
 */
extern CONST(SomeIpTp_TpRxNSduConfigType, SOMEIPTP_APPL_CONST) SomeIpTp_RxNsduElements[SOMEIPTP_NO_OF_RXNSDUS];
[!ENDIF!][!//

#define SOMEIPTP_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <SomeIpTp_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/
#define SOMEIPTP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <SomeIpTp_MemMap.h>

[!IF "num:i($SomeIpTp_TxNsduListSize) > num:i(0)"!][!//
/** \brief contains the state machine per TxNSdu data */
extern VAR(SomeIpTp_TpTxNSduType, SOMEIPTP_VAR) SomeIpTp_TxNSduState[SOMEIPTP_NO_OF_TXNSDUS];
[!ENDIF!][!//

[!IF "num:i($SomeIpTp_RxNsduListSize) > num:i(0)"!][!//
/** \brief contains the state machine per RxNSdu data */
extern VAR(SomeIpTp_TpRxNSduType, SOMEIPTP_VAR) SomeIpTp_RxNSduState[SOMEIPTP_NO_OF_RXNSDUS];
[!ENDIF!][!//

/*------------------[Tx MetaData local Buffers]-------------------*/
[!IF "num:i($SomeIpTp_TxNsduListSize) > num:i(0)"!][!//
[!VAR "ChannelId" = "0"!]
[!LOOP "as:modconf('SomeIpTp')[1]/SomeIpTpChannel/*"!]
[!VAR "i" = "0"!]
[!LOOP "./SomeIpTpTxNSdu/*"!][!//
extern VAR(uint8, SOMEIPTP_VAR) MetaData_SomeIpTpTxNSdu_[!"node:value(./SomeIpTpTxNSduHandleId)"!][8U];

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
extern VAR(uint8, SOMEIPTP_VAR) MetaData_SomeIpTpRxNSdu_[!"node:value(./SomeIpTpRxNPdu/SomeIpTpRxNPduHandleId)"!][8U];

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

#endif /* ifndef SOMEIPTP_CFG_H */
/*==================[end of file]===========================================*/

[!ENDCODE!]