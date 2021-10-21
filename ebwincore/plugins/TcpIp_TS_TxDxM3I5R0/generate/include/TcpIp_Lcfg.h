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
#ifndef TCPIP_LCFG_H
#define TCPIP_LCFG_H

[!AUTOSPACING!]
/*==================[inclusions]============================================*/

#include <TSAutosar.h>              /* global configuration */
#include <ComStack_Types.h>         /* Com Stack Types */
#include <TcpIp_Socket_Api.h>       /* Type definitions of Socket SW-unit */
#include <TcpIp_IpAddrM_Api.h>      /* Type definitions of IpAddrM SW-unit*/
#include <TcpIp_Ip_Api.h>           /* Type definitions of Ip SW-unit*/

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/** \brief Function pointer type for <Up>_RxIndication */
typedef P2FUNC(void, TCPIP_APPL_CODE, TcpIp_RxIndicationFpType)
(
  TcpIp_SocketIdType SocketId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) BufPtr,
  uint16 Length
);

/** \brief Function pointer type for <Up>_LocalIpAddrAssignmentChg */
typedef P2FUNC(void, TCPIP_APPL_CODE, TcpIp_LocalIpAddrAssignChgFpType)
(
  TcpIp_LocalAddrIdType IpAddrId,
  TcpIp_IpAddrStateType State
);

/** \brief Function pointer type for <Up>_CopyTxData */
typedef P2FUNC(BufReq_ReturnType, TCPIP_APPL_CODE, TcpIp_CopyTxDataFpType)
(
  TcpIp_SocketIdType SocketId,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) BufPtr,
  uint16 BufLength
);

/** \brief Function pointer type for <Up>_TcpIpEvent */
typedef P2FUNC(void, TCPIP_APPL_CODE, TcpIp_TcpIpEventFpType)
(
  TcpIp_SocketIdType SocketId,
  TcpIp_EventType Event
);

[!IF "as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpTcpEnabled = 'true'"!][!//
[!//
/** \brief Function pointer type for <Up>_TcpAccepted */
typedef P2FUNC(Std_ReturnType, TCPIP_APPL_CODE, TcpIp_TcpAcceptedFpType)
(
  TcpIp_SocketIdType SocketId,
  TcpIp_SocketIdType SocketIdConnected,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr
);

/** \brief Function pointer type for <Up>_TcpConnected */
typedef P2FUNC(void, TCPIP_APPL_CODE, TcpIp_TcpConnectedFpType)
(
  TcpIp_SocketIdType SocketId
);

/** \brief Function pointer type for <Up>_TxConfirmation */
typedef P2FUNC(void, TCPIP_APPL_CODE, TcpIp_TxConfirmationFpType)
(
  TcpIp_SocketIdType SocketId,
  uint16 Length
);
[!//
[!ENDIF!][!//

[!IF "(node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpIcmpConfig/TcpIpIcmpMsgHandler)) or
      (node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpIcmpV6Config/TcpIpIcmpV6MsgHandler))"!][!//
[!//
/** \brief Function pointer type for <Up>_IcmpMsgHandler */
typedef P2FUNC(void, TCPIP_APPL_CODE, TcpIp_IcmpMsgHandlerFpType)
(
  TcpIp_LocalAddrIdType LocalAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) RemoteAddrPtr,
  uint8 Ttl,
  uint8 Type,
  uint8 Code,
  uint16 DataLength,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr
);
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpIpV4ArpPacketFilter)"!][!//
[!//
/** \brief Function pointer type for <Up>_IpV4ArpPacketFilter */
typedef P2FUNC(boolean, TCPIP_APPL_CODE, TcpIp_IpV4ArpPacketFilterFpType)
(
  TcpIp_LocalAddrIdType localAddrId,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 lenByte
);
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpPhysAddrConfig/TcpIpPhysAddrChgHandler)"!][!//
[!//
/** \brief Function pointer type for <Up>_PhysAddrTableChg */
typedef P2FUNC(void, TCPIP_APPL_CODE, TcpIp_PhysAddrTableChgFpType)
(
  uint8 CtrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) IpAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  boolean valid
);
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpDuplicateAddressDetectionConfig)"!][!//
[!//
/** \brief Function pointer type for <Up>_DADAddressConflict */
typedef P2FUNC(void, TCPIP_APPL_CODE, TcpIp_DADAddressConflictFpType)
(
  TcpIp_LocalAddrIdType IpAddrId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC,TCPIP_APPL_DATA) IpAddrPtr,
  P2CONST(uint8, AUTOMATIC,TCPIP_APPL_DATA) LocalPhysAddrPtr,
  P2CONST(uint8, AUTOMATIC,TCPIP_APPL_DATA) RemotePhysAddrPtr
);
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpRxPolicyCheckIngressHandlerConfig)"!][!//
[!//
/** \brief Function pointer for <Up>_RxPolicyCheckIngressHandler */
typedef P2FUNC(Std_ReturnType, TCPIP_APPL_CODE, TcpIp_RxPolicyCheckIngressHandlerFpType)
(
  P2VAR(TcpIp_Ip_RxReturnType, AUTOMATIC, TCPIP_APPL_DATA) RxRetValPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DstIpAddrPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SrcIpAddrPtr,
  uint8 CtrlIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 LenByte,
  boolean IsBroadcast
);
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpRxPolicyCheckRoutingHandlerConfig)"!][!//
[!//
/** \brief Function pointer for <Up>_RxPolicyCheckRoutingHandler */
typedef P2FUNC(TcpIp_Ip_RxReturnType, TCPIP_APPL_CODE, TcpIp_RxPolicyCheckRoutingHandlerFpType)
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DstIpAddrPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SrcIpAddrPtr,
  uint8 CtrlIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 LenByte,
  boolean IsBroadcast
);
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpRxPacketPostProcessHandlerConfig)"!][!//
[!//
/** \brief Function pointer for <Up>_RxPacketPostProcessHandler */
typedef P2FUNC(void, TCPIP_APPL_CODE, TcpIp_RxPacketPostProcessHandlerFpType)
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) DstIpAddrPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) SrcIpAddrPtr,
  uint8 CtrlIdx,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 LenByte,
  boolean IsBroadcast
);
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpRxPacketDropHandlerConfig)"!][!//
[!//
/** \brief Function pointer for <Up>_RxPacketDropHandler */
typedef P2FUNC(void, TCPIP_APPL_CODE, TcpIp_RxPacketDropHandlerFpType)
(
  TcpIp_Ip_RxReturnType RxRetVal,
  uint8 CtrlIdx,
  Eth_FrameType FrameType,
  boolean IsBroadcast,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) PhysAddrPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) DataPtr,
  uint16 LenByte
);
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecReportErrorHandler)"!][!//
[!//
/** \brief Function pointer type for <Up>_IpSecportError */
typedef P2FUNC(void, TCPIP_APPL_CODE, TcpIp_IpSecReportErrorFpType)
(
  uint8 ctrlIdx,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) localSockAddPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteSockAddPtr,
  uint32 numberOfValidPolicies,
  uint32 numberOfInvalidPolicies,
  uint8 errorType
);
[!//
[!ENDIF!][!//


/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#define TCPIP_START_SEC_CONST_8
#include <TcpIp_MemMap.h>

/** \brief Number of total socket owners */
extern CONST(uint8, TCPIP_APPL_CONST) TcpIp_NumSockOwner;

/** \brief Number of socket owner configured through TcpIpSocketOwnerConfig (without DHCP) */
extern CONST(uint8, TCPIP_APPL_CONST) TcpIp_NumConfSockOwner;

#define TCPIP_STOP_SEC_CONST_8
#include <TcpIp_MemMap.h>

/* start data section declaration */
#define TCPIP_START_SEC_CONST_32
#include <TcpIp_MemMap.h>

/* value used to validate post build configuration against link time configuration */
extern CONST(uint32, TCPIP_CONST) TcpIp_LcfgSignature;

/* stop data section declaration */
#define TCPIP_STOP_SEC_CONST_32
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>

/** \brief Function pointer array for <Up>_RxIndication */
extern CONST(TcpIp_RxIndicationFpType, TCPIP_APPL_CONST) TcpIp_RxIndicationAPI[];

/** \brief Function pointer array for <Up>_LocalIpAddrAssignmentChg */
extern CONST(TcpIp_LocalIpAddrAssignChgFpType, TCPIP_APPL_CONST) TcpIp_LocalIpAddrAssignChgAPI[];

/** \brief Function pointer array for <Up>_CopyTxData */
extern CONST(TcpIp_CopyTxDataFpType, TCPIP_APPL_CONST) TcpIp_CopyTxDataAPI[];

/** \brief Function pointer array for <Up>_TcpIpEvent */
extern CONST(TcpIp_TcpIpEventFpType, TCPIP_APPL_CONST) TcpIp_TcpIpEventAPI[];

[!IF "as:modconf('TcpIp')[1]/TcpIpGeneral/TcpIpTcpEnabled = 'true'"!][!//
[!//
/** \brief Function pointer array for <Up>_TcpAccepted */
extern CONST(TcpIp_TcpAcceptedFpType, TCPIP_APPL_CONST) TcpIp_TcpAcceptedAPI[];

/** \brief Function pointer array for <Up>_TcpConnected */
extern CONST(TcpIp_TcpConnectedFpType, TCPIP_APPL_CONST) TcpIp_TcpConnectedAPI[];

/** \brief Function pointer array for <Up>_TxConfirmation */
extern CONST(TcpIp_TxConfirmationFpType, TCPIP_APPL_CONST) TcpIp_TxConfirmationAPI[];
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpIcmpConfig/TcpIpIcmpMsgHandler)"!][!//
[!//
/** \brief Function pointer for <Up>_IcmpMsgHandler */
extern CONST(TcpIp_IcmpMsgHandlerFpType, TCPIP_APPL_CONST) TcpIp_IcmpMsgHandlerAPI;
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpIcmpV6Config/TcpIpIcmpV6MsgHandler)"!][!//
[!//
/** \brief Function pointer for <Up>_IcmpMsgHandler for IpV6 */
extern CONST(TcpIp_IcmpMsgHandlerFpType, TCPIP_APPL_CONST) TcpIp_IcmpV6MsgHandlerAPI;
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpIpV4ArpPacketFilter)"!][!//
[!//
/** \brief Function pointer for <Up>_IpV4ArpPacketFilter */
extern CONST(TcpIp_IpV4ArpPacketFilterFpType, TCPIP_APPL_CONST) TcpIp_IpV4ArpPacketFilterAPI;
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpPhysAddrConfig/TcpIpPhysAddrChgHandler)"!][!//
[!//
/** \brief Function pointer for <Up>_PhysAddrTableChg */
extern CONST(TcpIp_PhysAddrTableChgFpType, TCPIP_APPL_CONST) TcpIp_PhysAddrTableChgAPI;
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpDuplicateAddressDetectionConfig)"!][!//
[!//
/** \brief Function pointer for <Up>_DADAddressConflict */
extern CONST(TcpIp_DADAddressConflictFpType, TCPIP_APPL_CONST) TcpIp_DADAddressConflictAPI;
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpRxPolicyCheckIngressHandlerConfig)"!][!//
[!//
/** \brief Function pointer for <Up>_RxPolicyCheckIngressHandler */
extern CONST(TcpIp_RxPolicyCheckIngressHandlerFpType, TCPIP_APPL_CONST) TcpIp_RxPolicyCheckIngressHandlerAPI;
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpRxPolicyCheckRoutingHandlerConfig)"!][!//
[!//
/** \brief Function pointer for <Up>_RxPolicyCheckRoutingHandler */
extern CONST(TcpIp_RxPolicyCheckRoutingHandlerFpType, TCPIP_APPL_CONST) TcpIp_RxPolicyCheckRoutingHandlerAPI;
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpRxPacketPostProcessHandlerConfig)"!][!//
[!//
/** \brief Function pointer for <Up>_RxPacketPostProcessHandler */
extern CONST(TcpIp_RxPacketPostProcessHandlerFpType, TCPIP_APPL_CONST) TcpIp_RxPacketPostProcessHandlerAPI;
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpRxPacketDropHandlerConfig)"!][!//
[!//
/** \brief Function pointer for <Up>_RxPacketDropHandler */
extern CONST(TcpIp_RxPacketDropHandlerFpType, TCPIP_APPL_CONST) TcpIp_RxPacketDropHandlerAPI;
[!//
[!ENDIF!][!//

[!IF "node:exists(TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecReportErrorHandler)"!][!//
[!//
/** \brief Function pointer for <Up>_IpSecReportError */
extern CONST(TcpIp_IpSecReportErrorFpType, TCPIP_APPL_CONST) TcpIp_IpSecReportErrorAPI;
[!//
[!ENDIF!][!//


#define TCPIP_STOP_SEC_CONST_UNSPECIFIED
#include <TcpIp_MemMap.h>

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef TCPIP_LCFG_H */
/*==================[end of file]===========================================*/
