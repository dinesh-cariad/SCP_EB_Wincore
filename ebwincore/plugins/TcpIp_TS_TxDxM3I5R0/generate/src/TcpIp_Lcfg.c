/**
 * \file
 *
 * \brief AUTOSAR TcpIp
 *
 * This file contains the implementation of the AUTOSAR
 * module TcpIp.
 *
 * \version 3.5.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR comstack types */
#include <TcpIp_Lcfg.h>         /* link time configuration */
#include <TcpIp_UpLDummy.h>     /* dummy function declarations */

[!IF "(as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV4General/TcpIpIpV4Enabled = 'true') and
      (as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV4General/TcpIpDhcpClientEnabled = 'true')"!][!//
#include <TcpIp_DhcpV4_Ext.h>   /* used SW-unit interface file */
[!ENDIF!][!//
[!IF "(as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true') and
      (as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV6General/TcpIpDhcpV6ClientEnabled = 'true')"!][!//
#include <TcpIp_DhcpV6_Ext.h>   /* used SW-unit interface file */
[!ENDIF!][!//

[!SELECT "as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpSocketOwnerConfig/TcpIpSocketOwner"!][!//
[!//
[!LOOP "./*"!][!//
[!IF "TcpIpSocketOwnerUpperLayerType = 'SOAD'"!][!//
#include <SoAd_Cbk.h>           /* SoAd API declarations */
[!ELSE!][!//
[!IF "not(node:empty(TcpIpSocketOwnerHeaderFileName))"!][!//
#include <[!"TcpIpSocketOwnerHeaderFileName"!]>         /* [!"node:name(.)"!] API declarations */
[!ENDIF!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDSELECT!][!//
[!IF "node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpIcmpConfig/TcpIpIcmpMsgHandler)"!][!//
[!SELECT "TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpIcmpConfig/TcpIpIcmpMsgHandler"!][!//
#include <[!"TcpIpIcmpMsgHandlerHeaderFileName"!]>         /* IcmpMsgHandlerHeader declarations */
[!ENDSELECT!][!//
[!ENDIF!][!//
[!IF "node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpIpV4ArpPacketFilter)"!][!//
[!SELECT "TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpIpV4ArpPacketFilter"!][!//
#include <[!"TcpIpIpV4ArpPacketFilterFileName"!]>         /* IpV4ArpPacketFilter declarations */
[!ENDSELECT!][!//
[!ENDIF!][!//
[!IF "node:exists(TcpIpConfig/TcpIpPhysAddrConfig/TcpIpPhysAddrChgHandler)"!][!//
[!SELECT "TcpIpConfig/TcpIpPhysAddrConfig/TcpIpPhysAddrChgHandler"!][!//
#include <[!"TcpIpPhysAddrChgHandlerHeaderFileName"!]>         /* PhysAddrChgHandler declarations */
[!ENDSELECT!][!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpIcmpV6Config/TcpIpIcmpV6MsgHandler)"!][!//
[!SELECT "TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpIcmpV6Config/TcpIpIcmpV6MsgHandler"!][!//
#include <[!"TcpIpIcmpV6MsgHandlerHeaderFileName"!]>         /* IcmpV6MsgHandlerHeader declarations */
[!ENDSELECT!][!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpDuplicateAddressDetectionConfig)"!][!//
[!SELECT "TcpIpConfig/TcpIpDuplicateAddressDetectionConfig"!][!//
#include <[!"TcpIpDuplicateAddressDetectionHeaderFileName"!]>         /* DADAddressConflictHandler declarations */
[!ENDSELECT!][!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpRxPolicyCheckIngressHandlerConfig)"!][!//
[!SELECT "TcpIpConfig/TcpIpRxPolicyCheckIngressHandlerConfig"!][!//
#include <[!"TcpIpRxPolicyCheckIngressHandlerHeaderFileName"!]>         /* RxPolicyCheckIngressHandlerHeader declarations */
[!ENDSELECT!][!//
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpRxPolicyCheckRoutingHandlerConfig)"!][!//
[!SELECT "TcpIpConfig/TcpIpRxPolicyCheckRoutingHandlerConfig"!][!//
#include <[!"TcpIpRxPolicyCheckRoutingHandlerHeaderFileName"!]>         /* RxPolicyCheckRoutingHandlerHeader declarations */
[!ENDSELECT!][!//
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpRxPacketPostProcessHandlerConfig)"!][!//
[!SELECT "TcpIpConfig/TcpIpRxPacketPostProcessHandlerConfig"!][!//
#include <[!"TcpIpRxPacketPostProcessHandlerHeaderFileName"!]>         /* RxPacketPostProcessHandlerHeader declarations */
[!ENDSELECT!][!//
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpRxPacketDropHandlerConfig)"!][!//
[!SELECT "TcpIpConfig/TcpIpRxPacketDropHandlerConfig"!][!//
#include <[!"TcpIpRxPacketDropHandlerHeaderFileName"!]>                /* RxPacketDropHandlerHeader declarations */
[!ENDSELECT!][!//
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecReportErrorHandler)"!][!//
[!SELECT "TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecReportErrorHandler"!][!//
#include <[!"TcpIpIpSecReportErrorHeaderFileName"!]>                      /* TcpIpIpSecReportingError declarations */
[!ENDSELECT!][!//
[!ENDIF!][!//
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/


#define TCPIP_START_SEC_CONST_8
#include <TcpIp_MemMap.h>

[!SELECT "as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpSocketOwnerConfig/TcpIpSocketOwner"!][!//
[!//]
/** \brief Number of total socket owners */
CONST(uint8, TCPIP_APPL_CONST) TcpIp_NumSockOwner = [!//
[!VAR "NumOfLayers"="0"!][!//
[!VAR "NumConfSockOwner"="count(./*)"!][!//
[!IF "(as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV4General/TcpIpIpV4Enabled = 'true') and
      (as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV4General/TcpIpDhcpClientEnabled = 'true')"!][!//
[!VAR "NumOfLayers"="$NumOfLayers + 1"!][!//
[!ENDIF!][!//
[!IF "(as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true') and
      (as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV6General/TcpIpDhcpV6ClientEnabled = 'true')"!][!//
[!VAR "NumOfLayers"="$NumOfLayers + 1"!][!//
[!ENDIF!][!//

[!"num:i($NumOfLayers + $NumConfSockOwner)"!]U;

/** \brief Number of socket owner configured through TcpIpSocketOwnerConfig (without DHCP) */
CONST(uint8, TCPIP_APPL_CONST) TcpIp_NumConfSockOwner = [!"num:i($NumConfSockOwner)"!]U;
[!//
[!ENDSELECT!][!//

#define TCPIP_STOP_SEC_CONST_8
#include <TcpIp_MemMap.h>

/* start data section declaration */
#define TCPIP_START_SEC_CONST_32
#include <TcpIp_MemMap.h>

/* value used to validate post build configuration against link time configuration */
CONST(uint32, TCPIP_CONST) TcpIp_LcfgSignature = [!"asc:getConfigSignature(as:modconf('TcpIp')[1]//*[not(child::*) and (node:configclass() = 'Link')])"!]U;

/* stop data section declaration */
#define TCPIP_STOP_SEC_CONST_32
#include <TcpIp_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#define TCPIP_START_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>

[!SELECT "as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpSocketOwnerConfig/TcpIpSocketOwner"!][!//
/** \brief Function pointer array for <Up>_RxIndication */
/* !LINKSTO TcpIp.DesignLcfg,1 */
CONST(TcpIp_RxIndicationFpType, TCPIP_APPL_CONST) TcpIp_RxIndicationAPI[] =
{
[!VAR "first"="0"!][!//
[!VAR "sizeof_TcpIp_RxIndicationAPI"="0"!][!//
[!LOOP "./*"!][!//
[!IF "TcpIpSocketOwnerUpperLayerType = 'SOAD'"!][!//
[!VAR "function"="'SoAd_RxIndication'"!][!//
[!ELSE!][!//
[!IF "node:empty(TcpIpSocketOwnerRxIndicationName)"!][!//
[!VAR "function"="'TcpIp_SocketOwnerRxIndication_dummy'"!][!//
[!ELSE!][!//
[!VAR "function"="TcpIpSocketOwnerRxIndicationName"!][!//
[!ENDIF!][!//
[!ENDIF!][!//
  [!IF "$first = 0"!][!VAR "first"="1"!][!ELSE!],[!ENDIF!]&[!"$function"!]
  [!VAR "sizeof_TcpIp_RxIndicationAPI"="$sizeof_TcpIp_RxIndicationAPI + 1"!][!//
[!ENDLOOP!][!//
[!IF "(as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV4General/TcpIpIpV4Enabled = 'true') and
      (as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV4General/TcpIpDhcpClientEnabled = 'true')"!][!//
[!VAR "first"="1"!][!//
  [!IF "$first = 0"!][!VAR "first"="1"!][!ELSE!],[!ENDIF!]&TcpIp_DhcpV4_rxIndication
  [!VAR "sizeof_TcpIp_RxIndicationAPI"="$sizeof_TcpIp_RxIndicationAPI + 1"!][!//
[!ENDIF!][!//
[!IF "(as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true') and
      (as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV6General/TcpIpDhcpV6ClientEnabled = 'true')"!][!//
[!VAR "first"="1"!][!//
  [!IF "$first = 0"!][!VAR "first"="1"!][!ELSE!],[!ENDIF!]&TcpIp_DhcpV6_rxIndication
  [!VAR "sizeof_TcpIp_RxIndicationAPI"="$sizeof_TcpIp_RxIndicationAPI + 1"!][!//
[!ENDIF!][!//
};

[!IF "$sizeof_TcpIp_RxIndicationAPI != ($NumConfSockOwner + $NumOfLayers)"!][!ERROR!]The number of TcpIp socket owners should
be the same as number of TcpIp_RxIndicationAPIs.[!ENDERROR!][!ENDIF!]

/** \brief Function pointer array for <Up>_LocalIpAddrAssignmentChg */
/* !LINKSTO TcpIp.DesignLcfg,1 */
CONST(TcpIp_LocalIpAddrAssignChgFpType, TCPIP_APPL_CONST) TcpIp_LocalIpAddrAssignChgAPI[] =
{
[!VAR "first"="0"!][!//
[!VAR "sizeof_TcpIp_LocalIpAddrAssignChgAPI"="0"!][!//
[!LOOP "./*"!][!//
[!IF "TcpIpSocketOwnerUpperLayerType = 'SOAD'"!][!//
[!VAR "function"="'SoAd_LocalIpAddrAssignmentChg'"!][!//
[!ELSE!][!//
[!IF "node:empty(TcpIpSocketOwnerLocalIpAddrAssignmentChgName)"!][!//
[!VAR "function"="'TcpIp_SocketOwnerLocalIpAddrAssignmentChg_dummy'"!][!//
[!ELSE!][!//
[!VAR "function"="TcpIpSocketOwnerLocalIpAddrAssignmentChgName"!][!//
[!ENDIF!][!//
[!ENDIF!][!//
  [!IF "$first = 0"!][!VAR "first"="1"!][!ELSE!],[!ENDIF!]&[!"$function"!]
  [!VAR "sizeof_TcpIp_LocalIpAddrAssignChgAPI"="$sizeof_TcpIp_LocalIpAddrAssignChgAPI + 1"!][!//
[!ENDLOOP!][!//
[!IF "(as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV4General/TcpIpIpV4Enabled = 'true') and
      (as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV4General/TcpIpDhcpClientEnabled = 'true')"!][!//
[!VAR "first"="1"!][!//
  [!IF "$first = 0"!][!VAR "first"="1"!][!ELSE!],[!ENDIF!]&TcpIp_DhcpV4_LocalIpAddrAssignmentChg
  [!VAR "sizeof_TcpIp_LocalIpAddrAssignChgAPI"="$sizeof_TcpIp_LocalIpAddrAssignChgAPI + 1"!][!//
[!ENDIF!][!//
[!IF "(as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true') and
      (as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV6General/TcpIpDhcpV6ClientEnabled = 'true')"!][!//
[!VAR "first"="1"!][!//
  [!IF "$first = 0"!][!VAR "first"="1"!][!ELSE!],[!ENDIF!]&TcpIp_DhcpV6_LocalIpAddrAssignmentChg
  [!VAR "sizeof_TcpIp_LocalIpAddrAssignChgAPI"="$sizeof_TcpIp_LocalIpAddrAssignChgAPI + 1"!][!//
[!ENDIF!][!//
};

[!IF "$sizeof_TcpIp_LocalIpAddrAssignChgAPI != ($NumConfSockOwner + $NumOfLayers)"!][!ERROR!]The number of TcpIp socket owners should
be the same as number of TcpIp_LocalIpAddrAssignChgAPIs.[!ENDERROR!][!ENDIF!]

/** \brief Function pointer array for <Up>_CopyTxData */
/* !LINKSTO TcpIp.DesignLcfg,1 */
CONST(TcpIp_CopyTxDataFpType, TCPIP_APPL_CONST) TcpIp_CopyTxDataAPI[] =
{
[!VAR "first"="0"!][!//
[!VAR "sizeof_TcpIp_CopyTxDataAPI"="0"!][!//
[!LOOP "./*"!][!//
[!IF "TcpIpSocketOwnerUpperLayerType = 'SOAD'"!][!//
[!VAR "function"="'SoAd_CopyTxData'"!][!//
[!ELSE!][!//
[!IF "node:empty(TcpIpSocketOwnerCopyTxDataName)"!][!//
[!VAR "function"="'TcpIp_SocketOwnerCopyTxData_dummy'"!][!//
[!ELSE!][!//
[!VAR "function"="TcpIpSocketOwnerCopyTxDataName"!][!//
[!ENDIF!][!//
[!ENDIF!][!//
  [!IF "$first = 0"!][!VAR "first"="1"!][!ELSE!],[!ENDIF!]&[!"$function"!]
  [!VAR "sizeof_TcpIp_CopyTxDataAPI"="$sizeof_TcpIp_CopyTxDataAPI + 1"!][!//
[!ENDLOOP!][!//
[!IF "(as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV4General/TcpIpIpV4Enabled = 'true') and
      (as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV4General/TcpIpDhcpClientEnabled = 'true')"!][!//
[!VAR "first"="1"!][!//
  [!IF "$first = 0"!][!VAR "first"="1"!][!ELSE!],[!ENDIF!]&TcpIp_DhcpV4_CopyTxData
  [!VAR "sizeof_TcpIp_CopyTxDataAPI"="$sizeof_TcpIp_CopyTxDataAPI + 1"!][!//
[!ENDIF!][!//
[!IF "(as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true') and
      (as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV6General/TcpIpDhcpV6ClientEnabled = 'true')"!][!//
[!VAR "first"="1"!][!//
  [!IF "$first = 0"!][!VAR "first"="1"!][!ELSE!],[!ENDIF!]&TcpIp_DhcpV6_CopyTxData
  [!VAR "sizeof_TcpIp_CopyTxDataAPI"="$sizeof_TcpIp_CopyTxDataAPI + 1"!][!//
[!ENDIF!][!//
};

[!IF "$sizeof_TcpIp_CopyTxDataAPI != ($NumConfSockOwner + $NumOfLayers)"!][!ERROR!]The number of TcpIp socket owners should
be the same as number of TcpIp_CopyTxDataAPIs.[!ENDERROR!][!ENDIF!]

/** \brief Function pointer array for <Up>_TcpIpEvent */
/* !LINKSTO TcpIp.DesignLcfg,1 */
CONST(TcpIp_TcpIpEventFpType, TCPIP_APPL_CONST) TcpIp_TcpIpEventAPI[] =
{
[!VAR "first"="0"!][!//
[!VAR "sizeof_TcpIp_TcpIpEventAPI"="0"!][!//
[!LOOP "./*"!][!//
[!IF "TcpIpSocketOwnerUpperLayerType = 'SOAD'"!][!//
[!VAR "function"="'SoAd_TcpIpEvent'"!][!//
[!ELSE!][!//
[!IF "node:empty(TcpIpSocketOwnerTcpIpEventName)"!][!//
[!VAR "function"="'TcpIp_SocketOwnerTcpIpEvent_dummy'"!][!//
[!ELSE!][!//
[!VAR "function"="TcpIpSocketOwnerTcpIpEventName"!][!//
[!ENDIF!][!//
[!ENDIF!][!//
  [!IF "$first = 0"!][!VAR "first"="1"!][!ELSE!],[!ENDIF!]&[!"$function"!]
  [!VAR "sizeof_TcpIp_TcpIpEventAPI"="$sizeof_TcpIp_TcpIpEventAPI + 1"!][!//
[!ENDLOOP!][!//
[!IF "(as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV4General/TcpIpIpV4Enabled = 'true') and
      (as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV4General/TcpIpDhcpClientEnabled = 'true')"!][!//
[!VAR "first"="1"!][!//
  [!IF "$first = 0"!][!VAR "first"="1"!][!ELSE!],[!ENDIF!]&TcpIp_SocketOwnerTcpIpEvent_dummy
  [!VAR "sizeof_TcpIp_TcpIpEventAPI"="$sizeof_TcpIp_TcpIpEventAPI + 1"!][!//
[!ENDIF!][!//
[!IF "(as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true') and
      (as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpIpV6General/TcpIpDhcpV6ClientEnabled = 'true')"!][!//
[!VAR "first"="1"!][!//
  [!IF "$first = 0"!][!VAR "first"="1"!][!ELSE!],[!ENDIF!]&TcpIp_SocketOwnerTcpIpEvent_dummy
  [!VAR "sizeof_TcpIp_TcpIpEventAPI"="$sizeof_TcpIp_TcpIpEventAPI + 1"!][!//
[!ENDIF!][!//
};

[!IF "$sizeof_TcpIp_TcpIpEventAPI != ($NumConfSockOwner + $NumOfLayers)"!][!ERROR!]The number of TcpIp socket owners should
be the same as number of TcpIp_TcpIpEventAPI.[!ENDERROR!][!ENDIF!]

[!IF "as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpTcpEnabled = 'true'"!][!//
[!//
/** \brief Function pointer array for <Up>_TcpAccepted */
/* !LINKSTO TcpIp.DesignLcfg,1 */
CONST(TcpIp_TcpAcceptedFpType, TCPIP_APPL_CONST) TcpIp_TcpAcceptedAPI[] =
{
[!VAR "first"="0"!][!//
[!VAR "sizeof_TcpIp_TcpAcceptedAPI"="0"!][!//
[!LOOP "./*"!][!//
[!IF "TcpIpSocketOwnerUpperLayerType = 'SOAD'"!][!//
[!VAR "function"="'SoAd_TcpAccepted'"!][!//
[!ELSE!][!//
[!IF "node:empty(TcpIpSocketOwnerTcpAcceptedName)"!][!//
[!VAR "function"="'TcpIp_SocketOwnerTcpAccepted_dummy'"!][!//
[!ELSE!][!//
[!VAR "function"="TcpIpSocketOwnerTcpAcceptedName"!][!//
[!ENDIF!][!//
[!ENDIF!][!//
  [!IF "$first = 0"!][!VAR "first"="1"!][!ELSE!],[!ENDIF!]&[!"$function"!]
  [!VAR "sizeof_TcpIp_TcpAcceptedAPI"="$sizeof_TcpIp_TcpAcceptedAPI + 1"!][!//
[!ENDLOOP!][!//
};

[!IF "$sizeof_TcpIp_TcpAcceptedAPI != $NumConfSockOwner"!][!ERROR!]The number of configured TcpIp socket owners should
be the same as number of TcpIp_TcpAcceptedAPIs.[!ENDERROR!][!ENDIF!]

/** \brief Function pointer array for <Up>_TcpConnected */
/* !LINKSTO TcpIp.DesignLcfg,1 */
CONST(TcpIp_TcpConnectedFpType, TCPIP_APPL_CONST) TcpIp_TcpConnectedAPI[] =
{
[!VAR "first"="0"!][!//
[!VAR "sizeof_TcpIp_TcpConnectedAPI"="0"!][!//
[!LOOP "./*"!][!//
[!IF "TcpIpSocketOwnerUpperLayerType = 'SOAD'"!][!//
[!VAR "function"="'SoAd_TcpConnected'"!][!//
[!ELSE!][!//
[!IF "node:empty(TcpIpSocketOwnerTcpConnectedName)"!][!//
[!VAR "function"="'TcpIp_SocketOwnerTcpConnected_dummy'"!][!//
[!ELSE!][!//
[!VAR "function"="TcpIpSocketOwnerTcpConnectedName"!][!//
[!ENDIF!][!//
[!ENDIF!][!//
  [!IF "$first = 0"!][!VAR "first"="1"!][!ELSE!],[!ENDIF!]&[!"$function"!]
  [!VAR "sizeof_TcpIp_TcpConnectedAPI"="$sizeof_TcpIp_TcpConnectedAPI + 1"!][!//
[!ENDLOOP!][!//
};

[!IF "$sizeof_TcpIp_TcpConnectedAPI != $NumConfSockOwner"!][!ERROR!]The number of configured TcpIp socket owners should
be the same as number of TcpIp_TcpConnectedAPIs.[!ENDERROR!][!ENDIF!]

/** \brief Function pointer array for <Up>_TxConfirmation */
/* !LINKSTO TcpIp.DesignLcfg,1 */
CONST(TcpIp_TxConfirmationFpType, TCPIP_APPL_CONST) TcpIp_TxConfirmationAPI[] =
{
[!VAR "first"="0"!][!//
[!VAR "sizeof_TcpIp_TxConfirmationAPI"="0"!][!//
[!LOOP "./*"!][!//
[!IF "TcpIpSocketOwnerUpperLayerType = 'SOAD'"!][!//
[!VAR "function"="'SoAd_TxConfirmation'"!][!//
[!ELSE!][!//
[!IF "node:empty(TcpIpSocketOwnerTxConfirmationName)"!][!//
[!VAR "function"="'TcpIp_SocketOwnerTxConfirmation_dummy'"!][!//
[!ELSE!][!//
[!VAR "function"="TcpIpSocketOwnerTxConfirmationName"!][!//
[!ENDIF!][!//
[!ENDIF!][!//
  [!IF "$first = 0"!][!VAR "first"="1"!][!ELSE!],[!ENDIF!]&[!"$function"!]
  [!VAR "sizeof_TcpIp_TxConfirmationAPI"="$sizeof_TcpIp_TxConfirmationAPI + 1"!][!//
[!ENDLOOP!][!//
};

[!IF "$sizeof_TcpIp_TxConfirmationAPI != $NumConfSockOwner"!][!ERROR!]The number of configured TcpIp socket owners should
be the same as number of TcpIp_TxConfirmationAPIs.[!ENDERROR!][!ENDIF!]
[!//
[!ENDIF!][!//
[!ENDSELECT!][!//

[!IF "node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpIcmpConfig/TcpIpIcmpMsgHandler)"!][!//
[!//
/** \brief Function pointer for <Up>_IcmpMsgHandler */
CONST(TcpIp_IcmpMsgHandlerFpType, TCPIP_APPL_CONST) TcpIp_IcmpMsgHandlerAPI =
  &[!"TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpIcmpConfig/TcpIpIcmpMsgHandler/TcpIpIcmpMsgHandlerName"!];
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpIpV4ArpPacketFilter)"!][!//
[!//
/** \brief Function pointer for <Up>_IpV4ArpPacketFilter */
CONST(TcpIp_IpV4ArpPacketFilterFpType, TCPIP_APPL_CONST) TcpIp_IpV4ArpPacketFilterAPI =
  &[!"TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpIpV4ArpPacketFilter/TcpIpIpV4ArpPacketFilterName"!];
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpPhysAddrConfig/TcpIpPhysAddrChgHandler)"!][!//
[!//
/** \brief Function pointer for <Up>_PhysAddrTableChg */
CONST(TcpIp_PhysAddrTableChgFpType, TCPIP_APPL_CONST) TcpIp_PhysAddrTableChgAPI =
  &[!"TcpIpConfig/TcpIpPhysAddrConfig/TcpIpPhysAddrChgHandler/TcpIpPhysAddrChgHandlerName"!];
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpIcmpV6Config/TcpIpIcmpV6MsgHandler)"!][!//
[!//
/** \brief Function pointer for <Up>_IcmpMsgHandler */
CONST(TcpIp_IcmpMsgHandlerFpType, TCPIP_APPL_CONST) TcpIp_IcmpV6MsgHandlerAPI =
  &[!"TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpIcmpV6Config/TcpIpIcmpV6MsgHandler/TcpIpIcmpV6MsgHandlerName"!];
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpDuplicateAddressDetectionConfig)"!][!//
[!//
/** \brief Function pointer for <Up>_DADAddressConflict */
CONST(TcpIp_DADAddressConflictFpType, TCPIP_APPL_CONST) TcpIp_DADAddressConflictAPI =
  &[!"TcpIpConfig/TcpIpDuplicateAddressDetectionConfig/TcpIpDuplicateAddressDetectionCalloutName"!];
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpRxPolicyCheckIngressHandlerConfig)"!][!//
[!//
/** \brief Function pointer for <Up>_RxPolicyCheckIngressHandler */
CONST(TcpIp_RxPolicyCheckIngressHandlerFpType, TCPIP_APPL_CONST) TcpIp_RxPolicyCheckIngressHandlerAPI =
  &[!"TcpIpConfig/TcpIpRxPolicyCheckIngressHandlerConfig/TcpIpRxPolicyCheckIngressHandlerName"!];
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpRxPolicyCheckRoutingHandlerConfig)"!][!//
[!//
/** \brief Function pointer for <Up>_RxPolicyCheckRoutingHandler */
CONST(TcpIp_RxPolicyCheckRoutingHandlerFpType, TCPIP_APPL_CONST) TcpIp_RxPolicyCheckRoutingHandlerAPI =
  &[!"TcpIpConfig/TcpIpRxPolicyCheckRoutingHandlerConfig/TcpIpRxPolicyCheckRoutingHandlerName"!];
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpRxPacketPostProcessHandlerConfig)"!][!//
[!//
/** \brief Function pointer for <Up>_RxPacketPostProcessHandler */
CONST(TcpIp_RxPacketPostProcessHandlerFpType, TCPIP_APPL_CONST) TcpIp_RxPacketPostProcessHandlerAPI =
  &[!"TcpIpConfig/TcpIpRxPacketPostProcessHandlerConfig/TcpIpRxPacketPostProcessHandlerName"!];
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpRxPacketDropHandlerConfig)"!][!//
[!//
/** \brief Function pointer for <Up>_RxPacketDropHandler */
CONST(TcpIp_RxPacketDropHandlerFpType, TCPIP_APPL_CONST) TcpIp_RxPacketDropHandlerAPI =
  &[!"TcpIpConfig/TcpIpRxPacketDropHandlerConfig/TcpIpRxPacketDropHandlerName"!];
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecReportErrorHandler)"!][!//
[!//
/** \brief Function pointer for <Up>_IpSecReportError */
CONST(TcpIp_IpSecReportErrorFpType, TCPIP_APPL_CONST) TcpIp_IpSecReportErrorAPI =
  &[!"TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecReportErrorHandler/TcpIpIpSecReportErrorHandlerName"!];
[!//
[!ENDIF!][!//

#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
