/**
 * \file
 *
 * \brief AUTOSAR IpduM
 *
 * This file contains the implementation of the AUTOSAR
 * module IpduM.
 *
 * \version 3.3.40
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef IPDUM_COMCALLOUT_CFG_H
#define IPDUM_COMCALLOUT_CFG_H

[!AUTOSPACING!]
/*==================[includes]==============================================*/

#include <TSAutosar.h>              /* global configuration */

[!INCLUDE "../../generate_macros/IpduM_Macros.m"!]

/*==================[macros]================================================*/

/* Network ordering of Global EcuId */
#if (defined IPDUM_NETWORK_BIG_ENDIAN)
#error IPDUM_NETWORK_BIG_ENDIAN already defined!
#endif /* #if (defined IPDUM_NETWORK_BIG_ENDIAN)*/
#define IPDUM_NETWORK_BIG_ENDIAN        STD_OFF

[!IF "node:exists(IpduMRequestMessageConfiguration)"!]
#if (defined IPDUM_SERVICE_MESSAGES)
#error IPDUM_SERVICE_MESSAGES already defined!
#endif /* #if (defined IPDUM_SERVICE_MESSAGES)*/
#define IPDUM_SERVICE_MESSAGES          STD_ON

/* Number of service message */
#if (defined IPDUM_MAP_INFO_LENGTH)
#error IPDUM_MAP_INFO_LENGTH already defined!
#endif /* #if (defined IPDUM_MAP_INFO_LENGTH)*/
#define IPDUM_MAP_INFO_LENGTH           [!"num:i(count(IpduMRequestMessageConfiguration/IpduMRequestMessageMapInfo/*))"!]U
/* Defines the byte position of the data field in the received message */
#if (defined IPDUM_RX_CALLOUT_BYTEPOS)
#error IPDUM_RX_CALLOUT_BYTEPOS already defined!
#endif /* #if (defined IPDUM_RX_CALLOUT_BYTEPOS)*/
#define IPDUM_RX_CALLOUT_BYTEPOS        [!"IpduMRequestMessageConfiguration/IpduMRequestMessageIdBytePos"!]U
/* Defines the length of the data field */
#if (defined IPDUM_RX_CALLOUT_LENGTH)
#error IPDUM_RX_CALLOUT_LENGTH already defined!
#endif /* #if (defined IPDUM_RX_CALLOUT_LENGTH)*/
#define IPDUM_RX_CALLOUT_LENGTH         [!"IpduMRequestMessageConfiguration/IpduMRequestMessageIdLength"!]U

/* Table for mapping of global PDU to ComIPduHandleId for requesting PDU */
#if (defined IPDUM_MAP_INFO_CFG)
#error IPDUM_MAP_INFO_CFG already defined!
#endif /* #if (defined IPDUM_MAP_INFO_CFG)*/
#define IPDUM_MAP_INFO_CFG \
  { \
  [!IF "IpduMRequestMessageConfiguration/IpduMRequestMessageIdLength <= 2"!]
    [!LOOP "IpduMRequestMessageConfiguration/IpduMRequestMessageMapInfo/*"!]
     [!CALL "GetComHandleId","IpduMRequestMessagePduRef"="IpduMRequestedMessagePduRef"!]
     /* ComTxPduId, ServiceMessageId */                 \
     { [!"$PduID"!]U, [!"IpduMRequestedMessageId"!]U }, \
   [!ENDLOOP!]
  [!ELSE!]
    [!LOOP "IpduMRequestMessageConfiguration/IpduMRequestMessageMapInfo/*"!]
     [!CALL "GetComHandleId","IpduMRequestMessagePduRef"="IpduMRequestedMessagePduRef"!]
      /* ServiceMessageId, ComTxPduId */                \
     { [!"IpduMRequestedMessageId"!]U, [!"$PduID"!]U }, \
    [!ENDLOOP!]
  [!ENDIF!]
  }
[!ELSE!]
#if (defined IPDUM_SERVICE_MESSAGES)
#error IPDUM_SERVICE_MESSAGES already defined!
#endif /* #if (defined IPDUM_SERVICE_MESSAGES)*/
#define IPDUM_SERVICE_MESSAGES          STD_OFF
[!ENDIF!]

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef IPDUM_COMCALLOUT_CFG_H */
/*==================[end of file]===========================================*/
