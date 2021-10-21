[!INCLUDE "XcpR_Vars.m"!][!//

#ifndef XCPR_INT_CFG_H
#define XCPR_INT_CFG_H

/**
 * \file
 *
 * \brief AUTOSAR XcpR
 *
 * This file contains the implementation of the AUTOSAR
 * module XcpR.
 *
 * \version 1.1.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==[Includes]================================================================*/

/*==[Macros definitions]======================================================*/

#if (defined XCPR_DEV_ERROR_DETECT)
#error XCPR_DEV_ERROR_DETECT already defined
#endif
/** \brief Switch, indicating whether the detection of development errors is
 ** activated or deactivated for XCPR */
#define XCPR_DEV_ERROR_DETECT         [!//
[!IF "text:tolower(XcpRGeneral/XcpRDevErrorDetect) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_MSG_QUEUE_SIZE)
#error XCPR_MSG_QUEUE_SIZE already defined
#endif
/** \brief Size of MSG Queue */
#define XCPR_MSG_QUEUE_SIZE           [!//
[!"(num:integer(XcpRGeneral/XcpRQueueSize))"!]U

#if (defined XCPR_EVENT_PACKET_ENABLED)
#error XCPR_EVENT_PACKET_ENABLED already defined
#endif
/** \brief Switch, indicating whether the transmission of EV_SESSION_TERMINATED from the XcpR device to
 ** the master device is activated or deactivated */
#define XCPR_EVENT_PACKET_ENABLED         [!//
[!IF "text:tolower(XcpRGeneral/XcpREventPacketEnabled) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/* !LINKSTO XcpR.Impl.ConnectionGroups.Disabled, 1 */
/* !LINKSTO XcpR.Impl.ConnectionGroups.Enabled, 1 */
#if (defined XCPR_MULTIPLE_ROUTING_PER_CONNECTION)
#error XCPR_MULTIPLE_ROUTING_PER_CONNECTION already defined
#endif
/** \brief Switch for support of multiple routing paths in a connection group */
#define XCPR_MULTIPLE_ROUTING_PER_CONNECTION     [!//
[!IF "num:integer(count(XcpRGeneral/XcpRRoutingToConnectionMapping/*)) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT)
#error XCPR_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT already defined
#endif
/** \brief Switch for FlexRay sequence correction support */
#define XCPR_FLEXRAY_SEQUENCE_CORRECTION_SUPPORT     [!//
[!IF "num:integer(count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverFlexRay']/*[XcpRDestinationSequenceCorrectionEnabled = 'true'])) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_NO_OF_ETHERNET_DESTINATIONS)
#error XCPR_NO_OF_ETHERNET_DESTINATIONS already defined
#endif
/** \brief Macro defining the number of configured XcpR destinations over Ethernet. */
#define XCPR_NO_OF_ETHERNET_DESTINATIONS     [!//
[!"num:integer($NoOfDestEthernetPdus)"!]

[!VAR "NoOfFlxMultiplePackSrc"="num:integer(count(XcpRGeneral/XcpRSourcePDUConfiguration/*[XcpRSourceInterfaceType = 'XcpRSourceConnectionOverFlexRay']/*[XcpRSourcePackMultiMsgInOneFlexRayFrame = 'true']))"!][!//
[!VAR "NoOfEthMultiplePackSrc"="num:integer(count(XcpRGeneral/XcpRSourcePDUConfiguration/*[XcpRSourceInterfaceType = 'XcpRSourceConnectionOverEthernet']/*[XcpRSourcePackMultiMsgInOneEthernetFrame = 'true']))"!][!//
[!VAR "NoOfFlxMultiplePackDest"="num:integer(count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverFlexRay']/*[XcpRDestinationPackMultiMsgInOneFlexRayFrame = 'true']))"!][!//
[!VAR "NoOfEthMultiplePackDest"="num:integer(count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverEthernet']/*[XcpRDestinationPackMultiMsgInOneEthernetFrame = 'true']))"!][!//
#if (defined XCPR_RECEPTION_OF_MULTI_PDUS_IN_FRAME_SUPPORT)
#error XCPR_RECEPTION_OF_MULTI_PDUS_IN_FRAME_SUPPORT already defined
#endif
/** \brief Switch for enabling support of reception of multiple XCPR messages in one frame */
#define XCPR_RECEPTION_OF_MULTI_PDUS_IN_FRAME_SUPPORT         [!//
[!IF "($NoOfFlxMultiplePackSrc > 0) or ($NoOfEthMultiplePackSrc > 0)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT)
#error XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT already defined
#endif
/** \brief Switch for enabling support of transmission of multiple XCPR messages in one frame */
#define XCPR_TRANSMISSION_OF_MULTI_PDUS_IN_FRAME_SUPPORT         [!//
[!IF "($NoOfFlxMultiplePackDest > 0) or ($NoOfEthMultiplePackDest > 0)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_NO_OF_DEST_MULTI_PDUS_IN_FRAME)
#error XCPR_NO_OF_DEST_MULTI_PDUS_IN_FRAME already defined
#endif
/** \brief Number of  destination PDUs configured with multiple messages in one frame support  */
#define XCPR_NO_OF_DEST_MULTI_PDUS_IN_FRAME       [!"num:integer($NoOfFlxMultiplePackDest + $NoOfEthMultiplePackDest)"!]

#if (defined XCPR_DEST_MAX_DLC_REQUIRED)
#error XCPR_DEST_MAX_DLC_REQUIRED already defined
#endif
/** \brief Switch to indicate maxDlc is enabled */
#define XCPR_DEST_MAX_DLC_REQUIRED      [!//
[!IF "num:integer(count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverCANFD']/*[XcpRDestinationCanFdMaxDlcRequired = 'true'])) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_RECEIVE_FROM_INDICATION)
#error XCPR_RECEIVE_FROM_INDICATION already defined
#endif
/** \brief Switch to indicate if support for reception from Rx is enabled */
#define XCPR_RECEIVE_FROM_INDICATION    [!//
[!IF "(num:integer(count(XcpRGeneral/XcpRSourcePDUConfiguration/*[XcpRPdu ='XcpRRxPdu']/*[XcpRSrcPduSupportRxFromRxIndication = 'true'])) > 0) or (num:integer(count(XcpRGeneral/XcpRSourcePDUConfiguration/*[XcpRPdu ='XcpRTxPdu']/*[XcpRSrcPduSupportRxFromXcpRTransmit = 'true'])) > 0)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_TRANSMIT_FROM_RXINDICATION)
#error XCPR_TRANSMIT_FROM_RXINDICATION already defined
#endif
/** \brief Switch to indicate if transmission for a message received via reception from RxIndication, is enabled */
#define XCPR_TRANSMIT_FROM_RXINDICATION    [!//
[!IF "(num:integer(count(XcpRGeneral/XcpRSourcePDUConfiguration/*[XcpRPdu ='XcpRRxPdu']/*[XcpRRxSrcPduSupportTxFromRxIndication = 'true'])) > 0) or (num:integer(count(XcpRGeneral/XcpRSourcePDUConfiguration/*[XcpRPdu ='XcpRTxPdu']/*[XcpRTxSrcPduSupportTxFromXcpRTransmit = 'true'])) > 0)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_TRANSMIT_FROM_CONFIRMATION)
#error XCPR_TRANSMIT_FROM_CONFIRMATION already defined
#endif
/** \brief Switch to indicate if support for transmission from TxConfirmation is enabled */
#define XCPR_TRANSMIT_FROM_CONFIRMATION    [!//
[!IF "num:integer(count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRPdu ='XcpRTxPdu']/*[XcpRDestPduSupportTxFromTxConfirmation = 'true'])) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!VAR "XcpRMaxSrcLength" = "num:i(0)"!][!//
[!LOOP "(XcpRGeneral/XcpRSourcePDUConfiguration/*)"!][!//
  [!VAR "XcpRCurrentSrcLength" = "num:max(text:split(concat(num:i(node:when(node:exists(XcpRSourcePDUAttributes/XcpRSrcMaxCtoPgm), XcpRSourcePDUAttributes/XcpRSrcMaxCtoPgm, 0)), ' ', num:i(XcpRSourcePDUAttributes/XcpRSrcMaxCto), ' ', num:i(XcpRSourcePDUAttributes/XcpRSrcMaxDto))))"!][!//
  [!IF "$XcpRCurrentSrcLength > $XcpRMaxSrcLength"!][!//
    [!VAR "XcpRMaxSrcLength" = "$XcpRCurrentSrcLength"!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!//

[!LOOP "(XcpRGeneral/XcpRDestinationPDUConfiguration/*)"!][!//
  [!VAR "XcpRMaxDestLength" = "num:max(text:split(concat(num:i(node:when(node:exists(XcpRDestinationPDUAttributes/XcpRDestMaxCtoPgm), XcpRDestinationPDUAttributes/XcpRDestMaxCtoPgm, 0)), ' ', num:i(XcpRDestinationPDUAttributes/XcpRDestMaxCto), ' ', num:i(XcpRDestinationPDUAttributes/XcpRDestMaxDto))))"!][!//
[!ENDLOOP!][!//
#if (defined XCPR_MAX_PACKAGE_SIZE_ALL_ROUTES)
#error XCPR_MAX_PACKAGE_SIZE_ALL_ROUTES already defined
#endif
/** \brief Max size of all configured sources and destinations */
#define XCPR_MAX_PACKAGE_SIZE_ALL_ROUTES    [!//
[!IF "$XcpRMaxSrcLength > $XcpRMaxDestLength"!][!"num:integer($XcpRMaxSrcLength)"!][!ELSE!][!"num:integer($XcpRMaxDestLength)"!][!ENDIF!]

#if (defined XCPR_MAX_PACKAGE_SIZE_RECEPTION)
#error XCPR_MAX_PACKAGE_SIZE_RECEPTION already defined
#endif
/** \brief Size of one message in the MSG Queue */
#define XCPR_MAX_PACKAGE_SIZE_RECEPTION    [!"num:integer($XcpRMaxSrcLength)"!]

#if (defined XCPR_NO_OF_SOURCE_PDUS)
#error XCPR_NO_OF_SOURCE_PDUS already defined
#endif
/** \brief The total number of configured Source PDUs */
#define XCPR_NO_OF_SOURCE_PDUS         [!//
[!"num:integer(count(XcpRGeneral/XcpRSourcePDUConfiguration/*))"!]U

#if (defined XCPR_NO_OF_DESTINATION_PDUS)
#error XCPR_NO_OF_DESTINATION_PDUS already defined
#endif
/** \brief The total number of configured Destination PDUs */
#define XCPR_NO_OF_DESTINATION_PDUS         [!//
[!"num:integer(count(XcpRGeneral/XcpRDestinationPDUConfiguration/*))"!]U

#if (defined XCPR_TX_RETRY_COUNT)
#error XCPR_TX_RETRY_COUNT already defined
#endif
/** \brief The number of times the data will be retried for transmission.
 ** This retry mechanism is meant for the case when <TP>If_Transmit() rejects
 ** our transmission request.
 **
 ** \note Macro is introduced to have limit on number of transmission retries
 ** when transmission fails.
 **
 **/
#define XCPR_TX_RETRY_COUNT           [!//
[!"XcpRGeneral/XcpRTxRetryCount"!]U

#if (defined XCPR_TX_BUS_TO_COUNTER_MAX_VAL)
#error XCPR_TX_BUS_TO_COUNTER_MAX_VAL already defined
#endif
/** \brief The number of XcpR_MainFunction calls until the XCPR is autonomously DISCONNECTED.
 **
 ** Macro is introduced to have the Bus Timeout limit in case the XCPR Destination processor remains
 ** stuck in one of the following states: TX Trigger/ TX Confirmation, after the pending message
 ** transmission was accepted by the underlying communication.
 **
 **/
#define XCPR_TX_BUS_TO_COUNTER_MAX_VAL [!//
[!"num:i(round(num:f(num:div(XcpRGeneral/XcpRTxBusTimeout, XcpRGeneral/XcpRMainFunctionPeriod))))"!]U

#if (defined XCPR_TX_BUS_RETRY_COUNT)
#error XCPR_TX_BUS_RETRY_COUNT already defined
#endif
/** \brief The number of times the PDU will be retried for transmission.
 ** Whenever we are waiting for the Tx Confirmation, if the bus timeout mechanism
 ** expires, we retry the entire transmission process for the configured amount of times.
 **
 ** \note Not to be confused with XCPR_TX_RETRY_COUNT which retries only the <TP>If_Transmit() requests
 ** when they are rejected.
 **/
#define XCPR_TX_BUS_RETRY_COUNT           [!//
[!"XcpRGeneral/XcpRTxBusRetry"!]U

/*==[Types declarations]======================================================*/

[!IF "num:i(round(num:f(num:div(XcpRGeneral/XcpRTxBusTimeout, XcpRGeneral/XcpRMainFunctionPeriod)))) < bit:shl(1,8)"!]
  [!VAR "BusToTypeSize" = "num:i(8)"!]
[!ELSEIF "num:i(round(num:f(num:div(XcpRGeneral/XcpRTxBusTimeout, XcpRGeneral/XcpRMainFunctionPeriod)))) < bit:shl(1,16)"!]
  [!VAR "BusToTypeSize" = "num:i(16)"!]
[!ELSE!]
  [!VAR "BusToTypeSize" = "num:i(32)"!]
[!ENDIF!]

/** \brief Definition for the Bus Timeout Counter value data type used to count the amount of
 ** XcpR_MainFunction cycles until the TX pending message is send by the underlying communication
 ** from the moment the transmission was accepted. */
typedef uint[!"$BusToTypeSize"!] XcpR_BusTimeoutCounterValueType;

/*==[Declaration of constants with external linkage]==========================*/
[!IF "count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverEthernet']) > 0"!]
  [!IF "$XcpRSuportForSoAdSocketProtocolTcp = 'true'"!]
#define XCPR_START_SEC_CONST_UNSPECIFIED
#include <XcpR_MemMap.h>

/** \brief List used to map TCP/IP socket connections towards an XcpR destination Id. */
extern CONST(XcpR_TcpIpSocketDestMapType, XCPR_CONST) XcpR_TcpIpSocketDestMapCfg[XCPR_NO_OF_TCPIP_SOCKET_CON];

#define XCPR_STOP_SEC_CONST_UNSPECIFIED
#include <XcpR_MemMap.h>
  [!ENDIF!]
[!ENDIF!]
/*==[Declaration of variables with external linkage]==========================*/

/*==[Declaration of functions with external linkage]==========================*/

#endif /* XCPR_INT_CFG_H */

