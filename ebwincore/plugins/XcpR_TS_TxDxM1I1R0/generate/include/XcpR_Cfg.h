[!INCLUDE "XcpR_Vars.m"!][!//

#ifndef XCPR_CFG_H
#define XCPR_CFG_H

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
/* This file contains the generated XcpR module configuration. */

/*==[Includes]================================================================*/

#include <Std_Types.h>    /* Autosar platform specific type declarations */
#include <TSAutosar.h>    /* EB specific standard types */
#include <ComStack_Types.h> /* Comstack types */
[!IF "count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverEthernet']) > 0"!][!//
#include <SoAd.h>           /* SoAd types */
[!ENDIF!]
/*==[Macros definitions]======================================================*/

[!VAR "NoOfTxDestCDDPdus"="count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverCDD' and (XcpRPdu = 'XcpRTxPdu')])"!]
[!VAR "NoOfRxDestCDDPdus"="count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverCDD' and (XcpRPdu = 'XcpRRxPdu')])"!]

#if (defined XCPR_MULTIPLE_SLAVES_ENABLED)
#error XCPR_MULTIPLE_SLAVES_ENABLED already defined
#endif
/** \brief Switch, indicating whether support for Multiple XCP Slaves
 ** is enabled or disabled */
#define XCPR_MULTIPLE_SLAVES_ENABLED         [!//
[!IF "text:tolower(XcpRGeneral/XcpRMultipleSlavesEnabled) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_VERSION_INFO_API)
#error XCPR_VERSION_INFO_API already defined
#endif
/** \brief Switch, indicating whether support for GetVersion API
 ** is enabled or disabled */
#define XCPR_VERSION_INFO_API         [!//
[!IF "text:tolower(XcpRGeneral/XcpRVersionInfoApi) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_MAIN_FUNCTION_PERIOD)
#error XCPR_MAIN_FUNCTION_PERIOD already defined
#endif
/** \brief The period between successive ticks in seconds */
#define XCPR_MAIN_FUNCTION_PERIOD     [!//
[!"XcpRGeneral/XcpRMainFunctionPeriod"!]

#if (defined XCPR_NO_OF_RX_SOURCE_PDUS)
#error XCPR_NO_OF_RX_SOURCE_PDUS already defined
#endif
/** \brief The total number of configured Source Rx PDUs */
#define XCPR_NO_OF_RX_SOURCE_PDUS         [!//
[!"num:integer(count(XcpRGeneral/XcpRSourcePDUConfiguration/*[XcpRPdu ='XcpRRxPdu']))"!]U

#if (defined XCPR_NO_OF_TX_SOURCE_PDUS)
#error XCPR_NO_OF_TX_SOURCE_PDUS already defined
#endif
/** \brief The total number of configured Source Tx PDUs */
#define XCPR_NO_OF_TX_SOURCE_PDUS         [!//
[!"num:integer(count(XcpRGeneral/XcpRSourcePDUConfiguration/*[XcpRPdu ='XcpRTxPdu']))"!]U

#if (defined XCPR_NO_OF_RX_DESTINATION_PDUS)
#error XCPR_NO_OF_RX_DESTINATION_PDUS already defined
#endif
/** \brief The total number of configured Destination Rx PDUs */
#define XCPR_NO_OF_RX_DESTINATION_PDUS         [!//
[!"num:integer(count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRPdu ='XcpRRxPdu']))"!]U

#if (defined XCPR_NO_OF_TX_DESTINATION_PDUS)
#error XCPR_NO_OF_TX_DESTINATION_PDUS already defined
#endif
/** \brief The total number of configured Destination Tx PDUs */
#define XCPR_NO_OF_TX_DESTINATION_PDUS         [!//
[!"num:integer(count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRPdu ='XcpRTxPdu']))"!]U

#if (defined XCPR_NO_OF_SRC_PDU_IDS)
#error XCPR_NO_OF_SRC_PDU_IDS already defined
#endif
/** \brief The total number of configured Source PDUs */
#define XCPR_NO_OF_SRC_PDU_IDS         [!//
[!"num:integer(count(XcpRGeneral/XcpRSourcePDUConfiguration/*))"!]U

#if (defined XCPR_NO_OF_DEST_PDU_IDS)
#error XCPR_NO_OF_DEST_PDU_IDS already defined
#endif
/** \brief The total number of configured Destination PDUs */
#define XCPR_NO_OF_DEST_PDU_IDS         [!//
[!"num:integer(count(XcpRGeneral/XcpRDestinationPDUConfiguration/*))"!]U

#if (defined XCPR_NO_OF_RX_SRC_PDU_IDS)
#error XCPR_NO_OF_RX_SRC_PDU_IDS already defined
#endif
/** \brief The total number of configured Source Rx PDUs */
#define XCPR_NO_OF_RX_SRC_PDU_IDS         [!//
[!"num:integer(count(XcpRGeneral/XcpRSourcePDUConfiguration/*[XcpRPdu ='XcpRRxPdu']))"!]U

#if (defined XCPR_NO_OF_TX_SRC_PDU_IDS)
#error XCPR_NO_OF_TX_SRC_PDU_IDS already defined
#endif
/** \brief The total number of configured Source Tx PDUs */
#define XCPR_NO_OF_TX_SRC_PDU_IDS         [!//
[!"num:integer(count(XcpRGeneral/XcpRSourcePDUConfiguration/*[XcpRPdu ='XcpRTxPdu']))"!]U

#if (defined XCPR_NO_OF_RX_DEST_PDU_IDS)
#error XCPR_NO_OF_RX_DEST_PDU_IDS already defined
#endif
/** \brief The total number of configured Destination Rx PDUs */
#define XCPR_NO_OF_RX_DEST_PDU_IDS         [!//
[!"num:integer(count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRPdu ='XcpRRxPdu']))"!]U

#if (defined XCPR_NO_OF_TX_DEST_PDU_IDS)
#error XCPR_NO_OF_TX_DEST_PDU_IDS already defined
#endif
/** \brief The total number of configured Destination Tx PDUs */
#define XCPR_NO_OF_TX_DEST_PDU_IDS         [!//
[!"num:integer(count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRPdu ='XcpRTxPdu']))"!]U

#if (defined XCPR_DEST_ON_CAN_LOWER_LAYER_ENABLED)
#error XCPR_DEST_ON_CAN_LOWER_LAYER_ENABLED already defined
#endif
/** \brief Switch, indicating whether support for interfacing XcpR with Can
 ** as the lower layer, is enabled or disabled */
#define XCPR_DEST_ON_CAN_LOWER_LAYER_ENABLED         [!//
[!IF "num:integer($NoOfDestCanPdus) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_DEST_ON_CANFD_LOWER_LAYER_ENABLED)
#error XCPR_DEST_ON_CANFD_LOWER_LAYER_ENABLED already defined
#endif
/** \brief Switch, indicating whether support for interfacing XcpR with CanFD
 ** as the lower layer, is enabled or disabled */
#define XCPR_DEST_ON_CANFD_LOWER_LAYER_ENABLED         [!//
[!IF "num:integer($NoOfDestCanFDPdus) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED)
#error XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED already defined
#endif
/** \brief Switch, indicating whether support for interfacing XcpR with FlexRay
 ** as the lower layer, is enabled or disabled */
#define XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED         [!//
[!IF "num:integer($NoOfDestFlexRayPdus) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED)
#error XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED already defined
#endif
/** \brief Switch, indicating whether support for interfacing XcpR with Ethernet
 ** as the lower layer, is enabled or disabled */
#define XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED         [!//
[!IF "num:integer($NoOfDestEthernetPdus) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_DEST_ON_CDD_LOWER_LAYER_ENABLED)
#error XCPR_DEST_ON_CDD_LOWER_LAYER_ENABLED already defined
#endif
/** \brief Switch, indicating whether support for interfacing XcpR with a lower layer
 ** CDD (i.e. Cdd_Transmit()) is enabled or disabled */
#define XCPR_DEST_ON_CDD_LOWER_LAYER_ENABLED         [!//
[!IF "num:integer($NoOfTxDestCDDPdus) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_DEST_ON_CDD_UPPER_LAYER_ENABLED)
#error XCPR_DEST_ON_CDD_UPPER_LAYER_ENABLED already defined
#endif
/** \brief Switch, indicating whether support for interfacing XcpR as a CDD with an upper layer
 ** is enabled or disabled (i.e. Xcp_CddRxIndication()) */
#define XCPR_DEST_ON_CDD_UPPER_LAYER_ENABLED         [!//
[!IF "num:integer($NoOfRxDestCDDPdus) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_SRC_ON_CANFD_FROM_LOWER_LAYER_ENABLED)
#error XCPR_SRC_ON_CANFD_FROM_LOWER_LAYER_ENABLED already defined
#endif
/** \brief Switch, indicating whether support for interfacing XcpR with CanFD
 ** as the lower layer, is enabled or disabled */
#define XCPR_SRC_ON_CANFD_FROM_LOWER_LAYER_ENABLED         [!//
[!IF "num:integer($NoOfSrcCanFDPdus) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_SRC_ON_FLEXRAY_FROM_LOWER_LAYER_ENABLED)
#error XCPR_SRC_ON_FLEXRAY_FROM_LOWER_LAYER_ENABLED already defined
#endif
/** \brief Switch, indicating whether support for interfacing XcpR with FlexRay
 ** as the lower layer, is enabled or disabled */
#define XCPR_SRC_ON_FLEXRAY_FROM_LOWER_LAYER_ENABLED         [!//
[!IF "num:integer($NoOfSrcFlexRayPdus) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED)
#error XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED already defined
#endif
/** \brief Switch, indicating whether support for interfacing XcpR with Ethernet
 ** as the lower layer, is enabled or disabled */
#define XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED         [!//
[!IF "num:integer($NoOfSrcEthernetPdus) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_NO_OF_FLEXRAY_DEST_WITH_SEQ_CORR)
#error XCPR_NO_OF_FLEXRAY_DEST_WITH_SEQ_CORR already defined
#endif
/** \brief The number of FlexRay destinations with sequence correction enabled */
#define XCPR_NO_OF_FLEXRAY_DEST_WITH_SEQ_CORR   [!//
[!"num:integer(count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverFlexRay']/*[XcpRDestinationSequenceCorrectionEnabled = 'true']))"!]U

#if (defined XCPR_OPEN_SO_CON)
#error XCPR_OPEN_SO_CON already defined
#endif
/** \brief Switch, indicating whether SoAd_OpenSoCon() will be called during XcpR_Init() */
#define XCPR_OPEN_SO_CON     [!//
[!IF "$XcpROpenSoCon = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_SOAD_PROTOCOL_TCP_SUPPORT)
#error XCPR_SOAD_PROTOCOL_TCP_SUPPORT already defined
#endif
/** \brief Switch, indicating whether the support for TCP protocol is enabled.
 ** If TCP is used the connection has to be terminated in case the
 ** connection is closed at the client side.
 **/
#define XCPR_SOAD_PROTOCOL_TCP_SUPPORT          [!//
[!IF "$XcpRSuportForSoAdSocketProtocolTcp"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!] 

#if (defined XCPR_SOAD_PROTOCOL_UDP_SUPPORT)
#error XCPR_SOAD_PROTOCOL_UDP_SUPPORT already defined
#endif
/** \brief Switch, indicating whether the support for UDP protocol is enabled.
 ** If UDP is used the socket must be closed if it is not used anymore.
 ** Otherwise the socket would be locked to the initial source port,
 ** preventing further requests from other ports.
 **/
#define XCPR_SOAD_PROTOCOL_UDP_SUPPORT          [!//
[!IF "$XcpRSuportForSoAdSocketProtocolUdp"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverEthernet']) > 0"!]
  [!IF "$XcpRSuportForSoAdSocketProtocolTcp = 'true'"!]
#if (defined XCPR_NO_OF_TCPIP_SOCKET_CON)
#error XCPR_NO_OF_TCPIP_SOCKET_CON already defined
#endif
/** \brief Macro defining the maximum number of configured TCPIP socket connections. */
#define XCPR_NO_OF_TCPIP_SOCKET_CON                         [!"num:i($NoOfXcpRTcpIpConnections)"!]U
  [!ENDIF!]
[!ENDIF!]

#if (defined XCPR_ETHERNET_MASTER_CONNECTION)
#error XCPR_ETHERNET_MASTER_CONNECTION already defined
#endif
/** \brief Macro defining if an XcpR destination connection with the master Xcp is present */
#define XCPR_ETHERNET_MASTER_CONNECTION          [!//
[!IF "(count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[(XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverEthernet') and (./XcpRIsDestMasterConnection = 'true')]) > 0)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_ETHERNET_REMOTE_SLAVE_CONNECTION)
#error XCPR_ETHERNET_REMOTE_SLAVE_CONNECTION already defined
#endif
/** \brief Macro defining if an XcpR destination connection with the remote slave Xcp is present */
#define XCPR_ETHERNET_REMOTE_SLAVE_CONNECTION          [!//
[!IF "(count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[(XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverEthernet') and (./XcpRIsDestMasterConnection = 'false')]) > 0)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!VAR "SrcTotalPackageSize"="0"!][!//
[!LOOP "(XcpRGeneral/XcpRSourcePDUConfiguration/*)"!][!//
  [!VAR "FlxMaxMsgLengthInit"="0"!][!//
  [!VAR "EthMaxMsgLengthInit"="0"!][!//
  [!VAR "FlxPackMultiMsgInOneFrame"="'false'"!][!//
  [!VAR "EthPackMultiMsgInOneFrame"="'false'"!][!//
  [!VAR "FlxHeaderAlignment"="''"!][!//
  [!VAR "SequenceCorrection"="'false'"!][!//
  [!VAR "XcpRMaxDataLength" = "num:max(text:split(concat(num:i(node:when(node:exists(XcpRSourcePDUAttributes/XcpRSrcMaxCtoPgm), XcpRSourcePDUAttributes/XcpRSrcMaxCtoPgm, 0)), ' ', num:i(XcpRSourcePDUAttributes/XcpRSrcMaxCto), ' ', num:i(XcpRSourcePDUAttributes/XcpRSrcMaxDto))))"!][!//
  [!IF "XcpRSourceInterfaceType = 'XcpRSourceConnectionOverFlexRay'"!][!//
    [!VAR "SequenceCorrection"="XcpRSourceInterfaceType/XcpRSourceSequenceCorrectionEnabled"!][!//
    [!VAR "FlxHeaderAlignment"="XcpRSourceInterfaceType/XcpRSourceFlxHeaderAlignment"!][!//
    [!VAR "FlxPackMultiMsgInOneFrame"="XcpRSourceInterfaceType/XcpRSourcePackMultiMsgInOneFlexRayFrame"!][!//
    [!IF "XcpRSourceInterfaceType/XcpRSourcePackMultiMsgInOneFlexRayFrame = 'true'"!][!//
      [!VAR "FlxMaxMsgLengthInit"="XcpRSourceInterfaceType/XcpRSourceMaxFlexMsgLength"!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
  [!IF "XcpRSourceInterfaceType = 'XcpRSourceConnectionOverEthernet'"!][!//
    [!VAR "EthPackMultiMsgInOneFrame"="XcpRSourceInterfaceType/XcpRSourcePackMultiMsgInOneEthernetFrame"!][!//
    [!IF "XcpRSourceInterfaceType/XcpRSourcePackMultiMsgInOneEthernetFrame = 'true'"!][!//
      [!VAR "EthMaxMsgLengthInit"="XcpRSourceInterfaceType/XcpRSourceMaxEthernetMsgLength"!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
  [!CALL "GetPackagePduSize","FlxPackMultiMsgInOneFrame"="$FlxPackMultiMsgInOneFrame","EthPackMultiMsgInOneFrame"="$EthPackMultiMsgInOneFrame","FlxMaxMsgLengthInit"="$FlxMaxMsgLengthInit","EthMaxMsgLengthInit"="$EthMaxMsgLengthInit","XcpRConnectionType"="XcpRSourceInterfaceType","FlxHeaderAlignment"="$FlxHeaderAlignment","SequenceCorrection"="$SequenceCorrection","XcpRMaxDataLength"="$XcpRMaxDataLength"!][!//
  [!VAR "SrcTotalPackageSize"="$SrcTotalPackageSize + $XcpRPackagePduSize"!]
[!ENDLOOP!][!//

[!VAR "DestTotalPackageSize"="0"!][!//
[!LOOP "(XcpRGeneral/XcpRDestinationPDUConfiguration/*)"!][!//
  [!VAR "FlxMaxMsgLengthInit"="0"!][!//
  [!VAR "EthMaxMsgLengthInit"="0"!][!//
  [!VAR "FlxPackMultiMsgInOneFrame"="'false'"!][!//
  [!VAR "EthPackMultiMsgInOneFrame"="'false'"!][!//
  [!VAR "FlxHeaderAlignment"="''"!][!//
  [!VAR "SequenceCorrection"="'false'"!][!//
  [!VAR "XcpRMaxDataLength" = "num:max(text:split(concat(num:i(node:when(node:exists(XcpRDestinationPDUAttributes/XcpRDestMaxCtoPgm), XcpRDestinationPDUAttributes/XcpRDestMaxCtoPgm, 0)), ' ', num:i(XcpRDestinationPDUAttributes/XcpRDestMaxCto), ' ', num:i(XcpRDestinationPDUAttributes/XcpRDestMaxDto))))"!][!//
  [!IF "XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverFlexRay'"!][!//
    [!VAR "SequenceCorrection"="XcpRDestinationInterfaceType/XcpRDestinationSequenceCorrectionEnabled"!][!//
    [!VAR "FlxHeaderAlignment"="XcpRDestinationInterfaceType/XcpRDestinationFlxHeaderAlignment"!][!//
    [!VAR "FlxPackMultiMsgInOneFrame"="XcpRDestinationInterfaceType/XcpRDestinationPackMultiMsgInOneFlexRayFrame"!][!//
    [!IF "XcpRDestinationInterfaceType/XcpRDestinationPackMultiMsgInOneFlexRayFrame = 'true'"!][!//
      [!VAR "FlxMaxMsgLengthInit"="XcpRDestinationInterfaceType/XcpRDestinationMaxFlexMsgLength"!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
  [!IF "XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverEthernet'"!][!//
    [!VAR "EthPackMultiMsgInOneFrame"="XcpRDestinationInterfaceType/XcpRDestinationPackMultiMsgInOneEthernetFrame"!]
    [!IF "XcpRDestinationInterfaceType/XcpRDestinationPackMultiMsgInOneEthernetFrame = 'true'"!]
      [!VAR "EthMaxMsgLengthInit"="XcpRDestinationInterfaceType/XcpRDestinationMaxEthernetMsgLength"!]
    [!ENDIF!]
  [!ENDIF!][!//
  [!CALL "GetPackagePduSize","FlxPackMultiMsgInOneFrame"="$FlxPackMultiMsgInOneFrame","EthPackMultiMsgInOneFrame"="$EthPackMultiMsgInOneFrame","FlxMaxMsgLengthInit"="$FlxMaxMsgLengthInit","EthMaxMsgLengthInit"="$EthMaxMsgLengthInit","XcpRConnectionType"="XcpRDestinationInterfaceType","FlxHeaderAlignment"="$FlxHeaderAlignment","SequenceCorrection"="$SequenceCorrection","XcpRMaxDataLength"="$XcpRMaxDataLength"!][!//
  [!VAR "DestTotalPackageSize"="$DestTotalPackageSize + $XcpRPackagePduSize"!]
[!ENDLOOP!][!//

#if (defined XCPR_DEST_BUFFER_SIZE)
#error XCPR_DEST_BUFFER_SIZE already defined
#endif
/** \brief Size of Transmission Buffer */
#define XCPR_DEST_BUFFER_SIZE          [!"num:integer($DestTotalPackageSize)"!]

#if (defined XCPR_SRC_BUFFER_SIZE)
#error XCPR_SRC_BUFFER_SIZE already defined
#endif
/** \brief Size of Reception Buffer  */
#define XCPR_SRC_BUFFER_SIZE           [!"num:integer($SrcTotalPackageSize)"!]

#if (defined XCPR_FR_TRIGGER_TX_API_ENABLED)
#error XCPR_FR_TRIGGER_TX_API_ENABLED already defined
#endif
/** \brief Switch, indicating whether XcpR_TriggerTransmit API is supported */
#define XCPR_FR_TRIGGER_TX_API_ENABLED         [!//
[!IF "count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRPdu = 'XcpRTxPdu']/*[XcpRTxDestinationPduSupportForFrTriggerTransmit = 'true']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined XCPR_CDD_TRIGGER_TX_API_ENABLED)
#error XCPR_CDD_TRIGGER_TX_API_ENABLED already defined
#endif
/** \brief Switch, indicating whether XcpR_TriggerTransmit API is supported */
#define XCPR_CDD_TRIGGER_TX_API_ENABLED         [!//
[!IF "count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRPdu = 'XcpRTxPdu']/*[XcpRTxDestinationPduSupportForCddTriggerTransmit = 'true']) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined XCPR_NO_OF_CONNECTION_GROUPS)
#error XCPR_NO_OF_CONNECTION_GROUPS already defined
#endif
/** \brief The total number of configured Connection groups */
#define XCPR_NO_OF_CONNECTION_GROUPS         [!//
[!"num:integer(count(XcpRGeneral/XcpRRoutingToConnectionMapping/*))"!]U

[!VAR "XcpRMaxRoutingPaths" = "num:i(0)"!][!//
[!LOOP "(XcpRGeneral/XcpRRoutingToConnectionMapping/*)"!][!//
  [!VAR "XcpRCurrentNrOfRoutingPaths" = "num:integer(count(./*/*))"!][!//
  [!IF "$XcpRCurrentNrOfRoutingPaths > $XcpRMaxRoutingPaths"!][!//
    [!VAR "XcpRMaxRoutingPaths" = "$XcpRCurrentNrOfRoutingPaths"!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!//

#if (defined XCPR_MAX_ROUTING_PATHS_PER_CONNECTION)
#error XCPR_MAX_ROUTING_PATHS_PER_CONNECTION already defined
#endif
/** \brief Maximum number of routing paths for a connection group */
#define XCPR_MAX_ROUTING_PATHS_PER_CONNECTION    [!"num:integer($XcpRMaxRoutingPaths)"!]U

/*==[Types declarations]======================================================*/

#if (XCPR_SRC_BUFFER_SIZE > 0xFF)
  typedef uint16 XcpR_SrcPduPosType;
#else
  typedef uint8 XcpR_SrcPduPosType;
#endif

#if (XCPR_DEST_BUFFER_SIZE > 0xFF)
  typedef uint16 XcpR_DestPduPosType;
#else
  typedef uint8 XcpR_DestPduPosType;
#endif

/** brief Data type to store the last position in the internal XcpR destination buffer, 
 ** in case multiple packing is enabled. If there are Ethernet destinations, a value larger 
 ** than 255 bytes can be packed and an uint16 data type is used. */
[!IF "count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverEthernet']) > 0"!][!//
typedef uint16 XcpR_MultiPackPduType;
[!ELSE!][!//
typedef uint8 XcpR_MultiPackPduType;
[!ENDIF!][!//

typedef struct
{
  uint16 MaxPackageSize;
  uint8  MaxCTO;
  uint16 MaxDTO;
  uint8  XcpRMaxCtoPgm;
  PduLengthType PduLengthMax;    /* maximum size for a PDU */
  PduLengthType PduHeaderLength; /* Pdu header size */
} XcpR_PduAttributesType;

typedef struct
{
  uint8 ConnectionType;          /* XcpR connection type (overCAN etc) */
#if ((XCPR_DEST_ON_CANFD_LOWER_LAYER_ENABLED == STD_ON) || (XCPR_SRC_ON_CANFD_FROM_LOWER_LAYER_ENABLED == STD_ON))
  PduLengthType MaxDlcValue;     /* MAX_DLC value */
  uint8 FillValue;               /* Fill byte value */
#endif
#if ((XCPR_DEST_ON_FLEXRAY_LOWER_LAYER_ENABLED == STD_ON) || (XCPR_SRC_ON_FLEXRAY_FROM_LOWER_LAYER_ENABLED == STD_ON))
  uint8 FlexRayNax;                      /* FlexRay network address */
  PduLengthType FlexRayPackageAlignment; /* FlexRay package alignment; PduLengthType is generated by Com in ComStack_Cfg.h */
#endif
  uint8 IntCounterId;            /* Id of internal package counter */
} XcpR_BusAttributesType;

/** \brief RxIndication function pointer type */
typedef P2FUNC(void, XCPR_CONST, XcpRRxIndicationPtr_t)
            (
              PduIdType XcpRTxPduId,
              P2VAR(PduInfoType, AUTOMATIC, XCPR_APPL_DATA) PduInfoPtr
            );

/** \brief TxConfirmation function pointer type */
typedef P2FUNC(void, XCPR_CONST, XcpRTxConfirmationPtr_t)
            (
              PduIdType XcpRTxPduId
            );

/** \brief Trigger transmit function pointer type */
typedef P2FUNC(Std_ReturnType, XCPR_CONST, XcpRTriggerTransmitPtr_t)
            (
              PduIdType XcpRTxPduId,
              P2VAR(PduInfoType, AUTOMATIC, XCPR_APPL_DATA) PduInfoPtr
            );

/** \brief Transmit function pointer type */
typedef P2FUNC(Std_ReturnType, XCPR_CONST, XcpRTransmitPtr_t)
            (
              PduIdType XcpRTxPduId,
              P2CONST(PduInfoType, AUTOMATIC, XCPR_APPL_DATA) PduInfoPtr
            );

typedef struct
{
  boolean IsMasterConnection; /* destination represents a connection to the master XCP or not */
  PduIdType IntPduBufId;  /* map PDU to internal buffer Id */
  PduIdType XcpRDestPduId; /* Pdu Id used by LowerLayer for XcpR_TxConfirmation */
  PduIdType TLDestPduId;    /* Pdu ID for the referenced PDU */
  PduIdType TxConfTriggerTransmitPduId;  /* Pdu ID used for UL TxConfirmation/TriggerTransmit */
[!IF "count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverEthernet']) > 0"!][!//
  SoAd_SoConIdType SoAdTxConId;  /* Id of transmit socket connection */
  boolean IsIPv6; /* flag if SoAd connection is IP V6*/
[!ENDIF!][!//
  uint8 PduType; /* PDU type - Rx or Tx */
  uint8 ProcessingType; /* the type of processing */
  XcpR_DestPduPosType PduStartPos; /* the start position of the PDU data inside SrcPdu buffer */
  XcpRRxIndicationPtr_t ULRxIndicationPtr; /* Pointer to RxIndication function */
  XcpRTxConfirmationPtr_t ULTxConfirmationPtr; /* Pointer to TxConfirmation function */
  XcpRTriggerTransmitPtr_t ULTriggerTransmitPtr; /* Pointer to Trigger Transmit function */
  XcpRTransmitPtr_t LLTransmitPtr; /* Pointer to transmit function */
  /* Attributes per source */
  XcpR_PduAttributesType PduAttributes;
  /* Bus Attributes per source */
  XcpR_BusAttributesType BusAttributes;
  uint8 Properties;                 /* Additional configuration properties:
                                     *  Bit0: Support for packing of multiple PDUs in one frame,
                                     *  Bit1: Support for CanFd MaxDlc enabled,
                                     *  Bit2: Support for automatically opening Socket connection for a destination
                                     *  Bit6: Support for sequence correction,
                                     *  Bit7: Support for trigger transmit AP.,
                                     */
} XcpR_DestPduType;

typedef struct
{
  boolean IsMasterConnection;  /* source represents a connection from master XCP or not */
  PduIdType IntPduBufId; /* map PDU to internal buffer Id */
  PduIdType XcpRSrcPduId; /* Pdu Id used by LowerLayer for XcpR_RxIndication, or UL for XcpR_Transmit */
[!IF "count(XcpRGeneral/XcpRSourcePDUConfiguration/*[XcpRSourceInterfaceType = 'XcpRSourceConnectionOverEthernet']) > 0"!]
  SoAd_SoConIdType SoAdRxConId;  /* Id of receive socket connection */
  boolean IsIPv6; /* flag if SoAd connection is IP V6*/
[!ENDIF!]
  uint8 PduType; /* PDU type - Rx or Tx */
  uint8 ProcessingType; /* the type of processing */
  XcpR_SrcPduPosType PduStartPos; /* the start position of the PDU data inside SrcPdu buffer */
  /* PDU Attributes per source */
  XcpR_PduAttributesType PduAttributes;
  /* Bus Attributes per source */
  XcpR_BusAttributesType BusAttributes;
  uint8 Properties;                 /* Additional configuration properties:
                                     *  Bit0: Support for unpacking of multiple PDUs in one frame
                                     *  Bit1: Support for transmission from RxIndication
                                     *  Bit2: Support for automatically opening Socket connection for a source
                                     */
  /* One source is linked to max 2 destinations, linkage generated from the XcpR Routing Information tab */
  const XcpR_DestPduType * SrcToDestMapping[2];
} XcpR_SrcPduType;

[!IF "num:integer(count(XcpRGeneral/XcpRRoutingToConnectionMapping/*)) > 0"!]
typedef struct
{
  const XcpR_DestPduType * RoutedDestinationPtr;
  PduIdType XcpRSrcPduId;
} XcpR_RoutingGroupType;

typedef struct
{
  XcpR_RoutingGroupType RoutingPath[XCPR_MAX_ROUTING_PATHS_PER_CONNECTION]; /* Array of max nr of routing paths per connection group. */
} XcpR_ConnectionGroupType;
[!ENDIF!]

[!IF "$XcpRSuportForSoAdSocketProtocolTcp = 'true'"!]
/** \brief Data structure mapping TCP/IP socket connection to XcpR internal destination Id. */
typedef struct
{
  SoAd_SoConIdType SoAdConId;      /* Socket connection Id. */
  PduIdType XcpRIntDestId;         /* XcpR internal destination Id. */
} XcpR_TcpIpSocketDestMapType;
[!ENDIF!]

/*==[Declaration of constants with external linkage]==========================*/

#define XCPR_START_SEC_CONST_UNSPECIFIED
#include <XcpR_MemMap.h>

/** \brief Source configuration info */
extern CONST(XcpR_SrcPduType, XCPR_CONST)  XcpR_SrcConf[XCPR_NO_OF_SRC_PDU_IDS];
/** \brief Destination configuration info */
extern CONST(XcpR_DestPduType, XCPR_CONST) XcpR_DestConf[XCPR_NO_OF_DEST_PDU_IDS];
[!IF "num:integer(count(XcpRGeneral/XcpRRoutingToConnectionMapping/*)) > 0"!]
/** \brief Connection group info */
extern CONST(XcpR_ConnectionGroupType, XCPR_CONST) XcpR_ConnectionGroups[XCPR_NO_OF_CONNECTION_GROUPS];
[!ENDIF!]
#define XCPR_STOP_SEC_CONST_UNSPECIFIED
#include <XcpR_MemMap.h>

/*==[Declaration of variables with external linkage]==========================*/

/*==[Declaration of functions with external linkage]==========================*/

#define XCPR_START_SEC_CODE
#include <XcpR_MemMap.h>

#if (XCPR_OPEN_SO_CON == STD_ON)
/** \brief Function used to initialize SoAd connections
 **
 ** The function will initialize the SoAd connections, by opening
 ** the required ones.
 **
 ** \return Result of the operation.
 ** \retval E_OK if the initialization of required SoAd socket is successful
 ** \retval E_NOT_OK if the initialization of required SoAd socket is not successful
 **
 ** */
extern FUNC(Std_ReturnType, XCPR_CODE) XcpR_InitSoAdConnections(void);
#endif

#define XCPR_STOP_SEC_CODE
#include <XcpR_MemMap.h>

#endif /* XCPR_CFG_H */

