/**
 * \file
 *
 * \brief AUTOSAR SoAd
 *
 * This file contains the implementation of the AUTOSAR
 * module SoAd.
 *
 * \version 1.8.16
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef SOAD_H
#define SOAD_H

/*  MISRA-C:2012 Deviation List
 *
 *  MISRA-1) Deviated Rule: 20.5 (advisory)
 *   #undef shall not be used.
 *
 *   Reason:
 *   Macro SOAD_NO_PBCFG_REQUIRED and SOAD_NO_CFG_REQUIRED may be defined in
 *   more than one instance which will cause compile
 *   warning.
 */
/*==================[inclusions]============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>
#include <Eth_GeneralTypes.h>  /* Ethernet general public types */
/* !LINKSTO SoAd.ASR42.SWS_SoAd_00073, 1 */
#include <TcpIp.h>

/*==========================================================================*/
/* !LINKSTO SoAd.ASR42.SWS_SoAd_00072, 1 */
#include <SoAd_Types.h>         /* Module public types */
#include <SoAd_Cfg.h>           /* Module configurable parameters */
#include <SoAd_Cbk.h>           /* Module callbacks */
#include <SoAd_Version.h>       /* Module Version Info */

/* Exclude post-build-time config include file if requested to do so */
#ifndef SOAD_NO_PBCFG_REQUIRED
#ifndef SOAD_NO_CFG_REQUIRED
#if(SOAD_PBCFGM_SUPPORT_ENABLED == STD_OFF)
#include <SoAd_PBcfg.h>         /* get SoAd post build configuration */
#endif /* SOAD_PBCFGM_SUPPORT_ENABLED == STD_OFF */
#endif /* SOAD_NO_CFG_REQUIRED */
#endif /* SOAD_NO_PBCFG_REQUIRED */


/* Deviation MISRA-1 */
#undef SOAD_NO_PBCFG_REQUIRED
/* Deviation MISRA-1 */
#undef SOAD_NO_CFG_REQUIRED

/*==================[macros]================================================*/


/*==================[Error codes]===========================================*/

#if (defined SOAD_E_NOTINIT)
#error SOAD_E_NOTINIT is already defined
#endif
/** \brief API service called before initializing the module */
#define SOAD_E_NOTINIT            0x01U

#if (defined SOAD_E_PARAM_POINTER)
#error SOAD_E_PARAM_POINTER is already defined
#endif
/** \brief API requests called with NULL pointer */
#define SOAD_E_PARAM_POINTER           0x02U

#if (defined SOAD_E_INV_ARG)
#error SOAD_E_INV_ARG is already defined
#endif
/** \brief API requests called with invalid argument */
#define SOAD_E_INV_ARG            0x03U

#if (defined SOAD_E_NOBUFS)
#error SOAD_E_NOBUFS is already defined
#endif
/** \brief Error - No buffer space available */
#define SOAD_E_NOBUFS             0x04U

#if (defined SOAD_E_INV_PDUHEADER_ID)
#error SOAD_E_INV_PDUHEADER_ID is already defined
#endif
/** \brief Error - Unknown PduHeader ID */
#define SOAD_E_INV_PDUHEADER_ID   0x05U

#if (defined SOAD_E_INV_PDUID)
#error SOAD_E_INV_PDUID is already defined
#endif
/** \brief Error - Unknown PDU ID */
#define SOAD_E_INV_PDUID          0x06U

#if (defined SOAD_E_INV_SOCKETID)
#error SOAD_E_INV_SOCKETID is already defined
#endif
/** \brief Error - Unknown Socket Address */
#define SOAD_E_INV_SOCKETID       0x07U

#if (defined SOAD_E_INV_METADATA)
#error SOAD_E_INV_METADATA is already defined
#endif
/** \brief Error - Invalid meta data */
#define SOAD_E_INV_METADATA       0x09U

/*==================[Vendor-specific Error codes]===========================*/

#if (defined SOAD_E_TWO_PDUS_SAME_SOCON)
#error SOAD_E_TWO_PDUS_SAME_SOCON is already defined
#endif
/** \brief Error - Two Pdus cannot be configured for the same Socket Connection simultaneously */
#define SOAD_E_TWO_PDUS_SAME_SOCON       0xFBU

#if (defined SOAD_E_TWO_SOCON_SAME_PDU)
#error SOAD_E_TWO_SOCON_SAME_PDU is already defined
#endif
/** \brief Error - Two Socket Connections cannot use the same PDU within the same Socongroup*/
#define SOAD_E_TWO_SOCON_SAME_PDU       0xFCU

#if (defined SOAD_E_INV_LENGTH_IN_PDUHEADER)
#error SOAD_E_INV_LENGTH_IN_PDUHEADER is already defined
#endif
/** \brief Error - Length in PduHeader exceeds the possible maximum */
#define SOAD_E_INV_LENGTH_IN_PDUHEADER       0xFDU

#if (defined SOAD_E_TRIGGERTXBUF)
#error SOAD_E_TRIGGERTXBUF is already defined
#endif
/** \brief Error - Insufficient TriggerTransmitBuffer */
#define SOAD_E_TRIGGERTXBUF       0xFEU

#if (defined SOAD_INVALID_ULFUNCID)
#error SOAD_INVALID_ULFUNCID is already defined
#endif
/** \brief Invalid upper layer function array index
 */
#define SOAD_INVALID_ULFUNCID     0xFFU

/*------------------[Service/instance IDs]----------------------------------*/

#if (defined SOAD_INSTANCE_ID)
#error SOAD_INSTANCE_ID is already defined
#endif
/** \brief Defines the instance number of this Socket Adapter.
 **        Since multiple instances of Socket Adapter are not supported
 **        the Instance Id is always zero.
 **/
#define SOAD_INSTANCE_ID                    0U

#if (defined SOAD_INIT_SVCID)
#error SOAD_INIT_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_Init()
 **/
#define SOAD_INIT_SVCID                     0x01U

#if (defined SOAD_GETVERSIONINFO_SVCID)
#error SOAD_GETVERSIONINFO_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_GetVersionInfo()
 **/
#define SOAD_GETVERSIONINFO_SVCID           0x02U

#if (defined SOAD_IFTRANSMIT_SVCID)
#error SOAD_IFTRANSMIT_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_IfTransmit()
 **/
#define SOAD_IFTRANSMIT_SVCID               0x03U

#if (defined SOAD_TPTRANSMIT_SVCID)
#error SOAD_TPTRANSMIT_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_TpTransmit()
 **/
#define SOAD_TPTRANSMIT_SVCID               0x04U

#if (defined SOAD_TPCANCELTRANSMIT_SVCID)
#error SOAD_TPCANCELTRANSMIT_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_CancelTransmit()
 **/
#define SOAD_TPCANCELTRANSMIT_SVCID         0x05U

#if (defined SOAD_TPCANCELRECEIVE_SVCID)
#error SOAD_TPCANCELRECEIVE_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_CancelReceive()
 **/
#define SOAD_TPCANCELRECEIVE_SVCID          0x06U

#if (defined SOAD_GETSOCONID_SVCID)
#error SOAD_GETSOCONID_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_GetSoConId()
 **/
#define SOAD_GETSOCONID_SVCID               0x07U

#if (defined SOAD_OPENSOCON_SVCID)
#error SOAD_OPENSOCON_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_OpenSoCon()
 **/
#define SOAD_OPENSOCON_SVCID                0x08U

#if (defined SOAD_CLOSESOCON_SVCID)
#error SOAD_CLOSESOCON_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_CloseSoCon()
 **/
#define SOAD_CLOSESOCON_SVCID               0x09U

#if (defined SOAD_REQUESTIPADDRASSIGNMENT_SVCID)
#error SOAD_REQUESTIPADDRASSIGNMENT_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_RequestIpAddrAssignment()
 **/
#define SOAD_REQUESTIPADDRASSIGNMENT_SVCID  0x0AU

#if (defined SOAD_RELEASEIPADDRASSIGNMENT_SVCID)
#error SOAD_RELEASEIPADDRASSIGNMENT_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_ReleaseIpAddrAssignment()
 **/
#define SOAD_RELEASEIPADDRASSIGNMENT_SVCID  0x0BU

#if (defined SOAD_GETLOCALADDR_SVCID)
#error SOAD_GETLOCALADDR_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_GetLocalAddr()
 **/
#define SOAD_GETLOCALADDR_SVCID             0x0CU

#if (defined SOAD_GETPHYSADDR_SVCID)
#error SOAD_GETPHYSADDR_SVCID is already defined
#endif
/** \brief Defines API id of function  SoAd_GetPhysAddr()
 **/
#define SOAD_GETPHYSADDR_SVCID              0x0DU

#if (defined SOAD_ENABLEROUTING_SVCID)
#error SOAD_ENABLEROUTING_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_EnableRouting()
 **/
#define SOAD_ENABLEROUTING_SVCID            0x0EU

#if (defined SOAD_ENABLESPECIFICROUTING_SVCID)
#error SOAD_ENABLESPECIFICROUTING_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_EnableSpecificRouting()
 **/
#define SOAD_ENABLESPECIFICROUTING_SVCID    0x20U

#if (defined SOAD_DISABLEROUTING_SVCID)
#error SOAD_DISABLEROUTING_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_DisableRouting()
 **/
#define SOAD_DISABLEROUTING_SVCID           0x0FU

#if (defined SOAD_DISABLESPECIFICROUTING_SVCID)
#error SOAD_DISABLESPECIFICROUTING_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_DisableSpecificRouting()
 **/
#define SOAD_DISABLESPECIFICROUTING_SVCID   0x21U

#if (defined SOAD_SETREMOTEADDR_SVCID)
#error SOAD_SETREMOTEADDR_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_SetRemoteAddr()
 **/
#define SOAD_SETREMOTEADDR_SVCID            0x10U

#if (defined SOAD_TPCHANGEPARAMETER_SVCID)
#error SOAD_TPCHANGEPARAMETER_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_TpChangeParameter()
 **/
#define SOAD_TPCHANGEPARAMETER_SVCID          0x11U

#if (defined SOAD_RXINDICATION_SVCID)
#error SOAD_RXINDICATION_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_TcpIpRxIndication()
 **/
#define SOAD_RXINDICATION_SVCID             0x12U

#if (defined SOAD_COPYTXDATA_SVCID)
#error SOAD_COPYTXDATA_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_CopyTxData()
 **/
#define SOAD_COPYTXDATA_SVCID               0x13U

#if (defined SOAD_TXCONFIRMATION_SVCID)
#error SOAD_TXCONFIRMATION_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_TxConfirmation()
 **/
#define SOAD_TXCONFIRMATION_SVCID           0x14U

#if (defined SOAD_TCPACCEPTED_SVCID)
#error SOAD_TCPACCEPTED_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_TcpAccepted()
 **/
#define SOAD_TCPACCEPTED_SVCID              0x15U

#if (defined SOAD_TCPCONNECTED_SVCID)
#error SOAD_TCPCONNECTED_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_TcpConnected()
 **/
#define SOAD_TCPCONNECTED_SVCID             0x16U

#if (defined SOAD_TCPIPEVENT_SVCID)
#error SOAD_TCPIPEVENT_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_TcpIpEvent()
 **/
#define SOAD_TCPIPEVENT_SVCID               0x17U

#if (defined SOAD_LOCALIPADDRASSIGNMENTCHG_SVCID)
#error SOAD_LOCALIPADDRASSIGNMENTCHG_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_LocalIpAddrAssignmentChg()
 **/
#define SOAD_LOCALIPADDRASSIGNMENTCHG_SVCID 0x18U

#if (defined SOAD_MAINFUNCTION_SVCID)
#error SOAD_MAINFUNCTION_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_MainFunction()
 **/
#define SOAD_MAINFUNCTION_SVCID             0x19U

#if (defined SOAD_READDHCPHOSTNAMEOPTION_SVCID)
#error SOAD_READDHCPHOSTNAMEOPTION_SVCID is already defined
#endif
/** \brief Define API id of function SoAd_ReadDhcpHostNameOption()
 **/
#define SOAD_READDHCPHOSTNAMEOPTION_SVCID   0x1AU

#if (defined SOAD_WRITEDHCPHOSTNAMEOPTION_SVCID)
#error SOAD_WRITEDHCPHOSTNAMEOPTION_SVCID is already defined
#endif
/** \brief Define API id of function SoAd_WriteDhcpHostNameOption()
 **/
#define SOAD_WRITEDHCPHOSTNAMEOPTION_SVCID  0x1BU

#if (defined SOAD_GETREMOTEADDR_SVCID)
#error SOAD_GETREMOTEADDR_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_GetRemoteAddr()
 **/
#define SOAD_GETREMOTEADDR_SVCID            0x1CU

#if (defined SOAD_IFROUTINGGROUPTRANSMIT_SVCID)
#error SOAD_IFROUTINGGROUPTRANSMIT_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_IfRoutingGroupTransmit()
 **/
#define SOAD_IFROUTINGGROUPTRANSMIT_SVCID   0x1DU

#if (defined SOAD_IFSPECIFICROUTINGGROUPTRANSMIT_SVCID)
#error SOAD_IFSPECIFICROUTINGGROUPTRANSMIT_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_IfSpecificRoutingGroupTransmit()
 **/
#define SOAD_IFSPECIFICROUTINGGROUPTRANSMIT_SVCID   0x1FU

#if (defined SOAD_SETUNIQUEREMOTEADDR_SVCID)
#error SOAD_SETUNIQUEREMOTEADDR_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_SetUniqueRemoteAddr()
 **/
#define SOAD_SETUNIQUEREMOTEADDR_SVCID      0x1EU

#if (defined SOAD_RELLEASEREMOTEADDR_SVCID)
#error SOAD_RELLEASEREMOTEADDR_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_ReleaseRemoteAddr()
 **/
#define SOAD_RELEASEREMOTEADDR_SVCID             0x23U

#if (defined SOAD_GETANDRESETMEASUREMENTDATA_SVCID)
#error SOAD_GETANDRESETMEASUREMENTDATA_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_GetAndResetMeasurementData()
 **/
#define SOAD_GETANDRESETMEASUREMENTDATA_SVCID    0x45U

#if (defined SOAD_GETSOCONMODE_SVCID)
#error SOAD_GETSOCONMODE_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_GetSoConMode()
 **/
#define SOAD_GETSOCONMODE_SVCID             0xF0U

#if (defined SOAD_CHANGEPARAMETER_SVCID)
#error SOAD_CHANGEPARAMETER_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_ChangeParameter()
 **/
#define SOAD_CHANGEPARAMETER_SVCID          0xF1U

#if (defined SOAD_MAINFUNCTIONTX_SVCID)
#error SOAD_MAINFUNCTIONTX_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_MainFunction()
 **/
#define SOAD_MAINFUNCTIONTX_SVCID             0xF2U

#if (defined SOAD_ISCONNECTIONREADY_SVCID)
#error SOAD_ISCONNECTIONREADY_SVCID is already defined
#endif
/** \brief Defines API id of function SoAd_IsConnectionReady()
 **/
#define SOAD_ISCONNECTIONREADY_SVCID   0xF3U

#if (defined SOAD_INTERNAL_SVCID)
#error SOAD_INTERNAL_SVCID is already defined
#endif
/** \brief Defines API id of internal functions
 **/
#define SOAD_INTERNAL_SVCID                 0xFFU

/*==================[macro checks]==========================================*/

/*== The following macros allow backward compatibility to upper layers ======*
 *== refering to AUTOSAR Specification of Socket Adapter prior to V2.0.22 ==*/
#if (defined SoAdIf_Transmit)
#error SoAdIf_Transmit is already defined
#endif
#define SoAdIf_Transmit       SoAd_IfTransmit

#if (defined SoAdTp_Transmit)
#error SoAdTp_Transmit is already defined
#endif
#define SoAdTp_Transmit       SoAd_TpTransmit

#if (defined SoAdTp_CancelTransmit)
#error SoAdTp_CancelTransmit is already defined
#endif
#define SoAdTp_CancelTransmit SoAd_TpCancelTransmit

#if (defined SoAdTp_CancelReceive)
#error SoAdTp_CancelReceive is already defined
#endif
#define SoAdTp_CancelReceive  SoAd_TpCancelReceive

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define SOAD_START_SEC_CODE
#include <SoAd_MemMap.h>

#if (SOAD_VERSION_INFO_API == STD_ON)

/** \brief Get version information of the SoAd module.
 **
 ** This service returns the version information of this module. The version
 ** information includes:
 **  - Module Id
 **  - Vendor Id
 **  - Vendor specific version numbers
 **
 ** \param[out] Versioninfo Pointer to where to store the version
 **                         information of this module.
 **
 ** \ServiceID{0x02}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, SOAD_CODE) SoAd_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, SOAD_APPL_DATA) Versioninfo
);
#endif /* SOAD_VERSION_INFO_API == STD_ON */

/** \brief Initialize the SoAd module.
 **
 ** This function initializes the SoAd module.
 **
 ** \param[in] SoAdConfigPtr Points to the implementation specific structure
 **
 ** \ServiceID{0x01}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, SOAD_CODE) SoAd_Init
(
  P2CONST(SoAd_ConfigType, AUTOMATIC, SOAD_APPL_CONST) SoAdConfigPtr
);

 /** \brief Checks compatibility of the post-build-time configuration.
  **
  ** This service checks the compatibility of the post-build-time configuration
  ** against the source code.
  **
  ** \param[in] SoAdConfigPtr Pointer to the configuration data of the SoAd module.
  **
  ** \Reentrancy{Reentrant}
  ** \Synchronicity{Synchronous}
  */
 extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_IsValidConfig
 (
   P2CONST(void, AUTOMATIC, SOAD_APPL_CONST) SoAdConfigPtr
 );

/** \brief Transfer L-PDU.
 **
 ** This service is used to request the transfer of L-PDU.
 **
 ** \param[in] SoAdSrcPduId      This parameter contains a unique identifier
 **                              referencing to the PDU Routing Table and
 **                              thereby specifying the socket to be used
 **                              for transmission of the data
 ** \param[in] SoAdSrcPduInfoPtr A pointer to a structure with socket related
 **                              data: data length and pointer to a data buffer.
 **
 ** \return Std_ReturnType
 ** \retval E_OK     The request has been accepted
 ** \retval E_NOT_OK The request has not been accepted( e.g. due to a still
 **                  ongoing transmission in the corresponding socket or
 **                  the to be transmitted message is too long)
 **
 ** \ServiceID{0x03}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfTransmit
(
  PduIdType SoAdSrcPduId,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr
);

/** \brief Initiate transmission of IF-TxPDUs belonging to a given routing group.
 **
 ** Triggers the transmission of all If-TxPDUs identified by the parameter id after
 ** requesting the data from the related upper layer.
 **
 ** \param[in] Id  Identification of the routing group.
 **
 ** \returns Std_ReturnType
 ** \retval E_OK     The request was successful
 ** \retval E_NOT_OK The request was not successful

 ** \ServiceID{0x1D}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfRoutingGroupTransmit
(
    SoAd_RoutingGroupIdType Id
);

/** \brief Initiate transmission of IF-TxPDUs belonging to a given routing group
 **        on a specific socket connection.
 **
 ** Triggers the transmission of all If-TxPDUs identified by the parameter id on the
 ** socket connection specified by SoConId after requesting the data from the related
 ** upper layer.
 **
 ** \param[in] Id       Identification of the routing group.
 ** \param[in] SoConId  Identification of the socket connection.
 **
 ** \returns Std_ReturnType
 ** \retval E_OK     The request was successful
 ** \retval E_NOT_OK The request was not successful

 ** \ServiceID{0x1F}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_IfSpecificRoutingGroupTransmit
(
    SoAd_RoutingGroupIdType Id,
    SoAd_SoConIdType SoConId
);

/** \brief Checks if ARP entry and IpSec SA exist in TcpIp.
 **
 ** Calls TcpIp with the correct SocketId and RemoteAddr of the corresponding SoConId.
 ** Passes the return value of TcpIp to upper layer.
 **
 ** \param[in] SoConId  Identification of the socket connection.
 **
 ** \returns TcpIp_ReturnType
 ** \retval TCPIP_E_PENDING   ARP entry or IpSec SA does not exist
 ** \retval TCPIP_E_NOT_OK    Transmission not allowed according to policy table
 ** \retval TCPIP_OK          ARP and IpSec SA exists data can be sent

 ** \ServiceID{0x1F}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(TcpIp_ReturnType, SOAD_CODE) SoAd_IsConnectionReady
(
    SoAd_SoConIdType SoConId,
    P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr
);

/** \brief Transfer data.
 **
 ** This service is used to request the transfer of data.
 **
 ** \param[in] SoAdSrcPduId      This parameter contains a unique identifier
 **                              referencing to the PDU Routing Table and
 **                              thereby specifying the socket to be used
 **                              for transmission of the data
 ** \param[in] SoAdSrcPduInfoPtr A pointer to a structure with socket related
 **                              data. Only the length data is valid.
 **
 ** \return Std_ReturnType
 ** \retval E_OK     The request has been accepted
 ** \retval E_NOT_OK The request has not been accepted( e.g. due to a still
 **                  ongoing transmission in the corresponding socket or
 **                  the to be transmitted message is too long)
 **
 ** \ServiceID{0x04}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_TpTransmit
(
  PduIdType SoAdSrcPduId,
  P2CONST(PduInfoType, AUTOMATIC, SOAD_APPL_DATA) SoAdSrcPduInfoPtr
);

/** \brief Cancel Transmission
 **
 ** Requests cancellation of the transmission via TP for a specific I-PDU.
 **
 ** \param[in] PduId - Identification of the I-PDU to be cancelled.
 **
 ** \return Std_ReturnType
 ** \retval E_OK      The request accepted (but not yet performed).
 ** \retval E_NOT_OK  The request not accepted (e.g. cancellation not possible).
 **
 ** \ServiceID{0x05}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId.}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_TpCancelTransmit
(
  PduIdType PduId
);

/** \brief Cancel Reception
 **
 ** Requests cancellation of the reception via TP for a specific I-PDU.
 **
 ** \param[in] PduId - Identification of the I-PDU to be cancelled.
 **
 ** \return Std_ReturnType
 ** \retval E_OK      The request accepted (but not yet performed).
 ** \retval E_NOT_OK  The request not accepted (e.g. cancellation not possible).
 **
 ** \ServiceID{0x06}
 ** \Reentrancy{Reentrant for different PduIds. Non reentrant for the same PduId.}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_TpCancelReceive
(
  PduIdType PduId
);

/** \brief Get socket connection ID for given PduId
 **
 ** Returns socket connection index related to specified transmit PduId. In case a fan-
 ** out is configured for TxPduId (i.e. multiple SoAdPduRouteDest specified)
 ** E_NOT_OK shall be returned.
 **
 ** \param[in]  TxPduId     Transmit PduId specifying the SoAd socket connection for
 **                         which the socket connection index shall be returned.
 ** \param[out] SoConIdPtr  Pointer to memory receiving the socket connection index
 **
 ** \returns Std_ReturnType
 ** \retval E_OK     The request was successful
 ** \retval E_NOT_OK The request was not successful
 **
 ** \ServiceID{0x07}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetSoConId
(
  PduIdType TxPduId,
  P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_DATA) SoConIdPtr
);

/** \brief Get socket connection mode for the given socket connection index
 **
 ** Returns socket connection mode related to the specified socket connection.
 **
 ** \param[in]  SoConId     Index of the socket connection which mode shall be returned.
 ** \param[out] ModePtr     Pointer to memory where the socket connection mode shall be stored
 **
 ** \returns Std_ReturnType
 ** \retval E_OK     The request was successful
 ** \retval E_NOT_OK The request was not successful
 **
 ** \ServiceID{0xF0}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetSoConMode
(
  SoAd_SoConIdType SoConId,
  P2VAR(SoAd_SoConModeType, AUTOMATIC, SOAD_APPL_DATA) ModePtr
);

/** \brief Open a socket connection
 **
 ** This service opens the socket connection specified by SoConId.
 **
 ** \param[in] SoConId  Socket connection index specifying the socket connection which
 **                     shall be opened
 **
 ** \returns Std_ReturnType
 ** \retval E_OK     The request was successful
 ** \retval E_NOT_OK The request was not successful
 **
 ** \ServiceID{0x08}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_OpenSoCon
(
  SoAd_SoConIdType SoConId
);

/** \brief Close a socket connection
 **
 ** This service closes the socket connection specified by SoConId.
 **
 ** \param[in] SoConId  Socket connection index specifying the socket connection
 **                     which shall be closed
 ** \param[in] Abort    TRUE:  socket connection will immediately be terminated.
 **                     FALSE: socket connection will be terminated if no other upper
 **                            layer is using this socket connection.
 **
 ** \returns Std_ReturnType
 ** \retval E_OK     The request was successful
 ** \retval E_NOT_OK The request was not successful

 ** \ServiceID{0x09}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_CloseSoCon
(
  SoAd_SoConIdType SoConId,
  boolean Abort
);

/** \brief Request IP address assignment
 **
 ** By this API service the local IP address assignment which shall be used for the socket
 ** connection specified by SoConId is initiated.
 **
 ** \param[in] SoConId  socket connection index specifying the socket connection for which the IP
 **                     address shall be set
 ** \param[in] Type     type of IP address assignment which shall be initiated.
 ** \param[in] LocalIpAddrPtr  pointer to structure containing the IP address which shall be
 **                            assigned to the EthIf controller indirectly specified via SoConId.
 **                            Note:This parameter is only use in case parameter Type is set to
 **                            TCPIP_IPADDR_ASSIGNMENT_STATIC.
 ** \returns Std_ReturnType
 ** \retval E_OK     The request has been accepted
 ** \retval E_NOT_OK The request has not been accepted
 **
 ** \ServiceID{0x0A}
 ** \Reentrancy{Reentrant for different SoConIds. Non reentrant for the same SoConId. }
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_RequestIpAddrAssignment
(
  SoAd_SoConIdType SoConId,
  TcpIp_IpAddrAssignmentType Type,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) LocalIpAddrPtr
);

/** \brief Release IP address assignment
 **
 ** By this API service the local IP address assignment used for the socket connection
 ** specified by SoConId is released.
 **
 ** \param[in] SoConId  socket connection index specifying the socket connection for which the
 **                     IP address shall be released
 **
 ** \returns Std_ReturnType
 ** \retval E_OK     The request has been accepted
 ** \retval E_NOT_OK  The request has not been accepted

 ** \ServiceID{0x0B}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_ReleaseIpAddrAssignment
(
  SoAd_SoConIdType SoConId
);

/** \brief Get the current local Ip address
 **
 ** Obtains the local address (IP address and port) actually used for the SoAd socket
 ** connection specified by SoConId, the netmask and default router
 **
 ** \param[in] SoConId           socket connection index representing the SoAd socket
 **                              connection for which the actual local IP address shall be obtained
 ** \param[out] LocalAddrPtr     Pointer to a struct where the local address (IP address and port)
 **                              is stored.
 ** \param[out] NetmaskPtr       Pointer to memory where Network mask of IPv4 address or
 **                              address prefix of IPv6 address in CIDR Notation is stored
 ** \param[out] DefaultRouterPtr Pointer to struct where the IP address of the default router
 **                              (gateway) is stored (struct member port is not used and of
 **                              arbitrary value)
 **
 ** \returns Std_ReturnType
 ** \retval E_OK     The request was successful
 ** \retval E_NOT_OK The request was not successful

 ** \ServiceID{0x0C}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetLocalAddr
(
  SoAd_SoConIdType SoConId,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) LocalAddrPtr,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) NetmaskPtr,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) DefaultRouterPtr
);

/** \brief Get the physical Ip address
 **
 ** Obtains the physical source address of the EthIf controller used by the SoAd
 ** socket connection specified by SoConId.
 **
 ** \param[in]  SoConId      socket connection index representing the SoAd socket
 **                          connection for which the physical source address of the related
 **                          EthIf controller shall be obtained.
 ** \param[out] PhysAddrPtr  Pointer to the memory where the physical source address (MAC
 **                          address) in network byte order is stored
 **
 ** \returns Std_ReturnType
 ** \retval E_OK     The request was successful
 ** \retval E_NOT_OK The request was not successful

 ** \ServiceID{0x0D}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetPhysAddr
(
  SoAd_SoConIdType SoConId,
  P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) PhysAddrPtr
);

/** \brief Enable routing of a group of PDUs
 **
 ** Enables routing of a group of PDUs in the SoAd related to the RoutingGroup specified by
 ** parameter id.
 ** Routing of PDUs can be either forwarding of PDUs from the upper layer to a TCP or UDP
 ** socket of the TCP/IP stack specified by a PduRoute or the other way around specified by a
 ** SocketRoute.
 **
 ** \param[in] Id  Identification of the routing group.
 **
 ** \returns Std_ReturnType
 ** \retval E_OK     The request was successful
 ** \retval E_NOT_OK The request was not successful

 ** \ServiceID{0x0E}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_EnableRouting
(
  SoAd_RoutingGroupIdType Id
);


/** \brief Enable routing of a group of PDUs on a specific socket connection.
 **
 ** Enables routing of a group of PDUs in the SoAd related to the RoutingGroup specified by
 ** parameter id on a specific socket connection specified by SoConId.
 ** Routing of PDUs can be either forwarding of PDUs from the upper layer to a TCP or UDP
 ** socket of the TCP/IP stack specified by a PduRoute or the other way around specified by a
 ** SocketRoute.
 **
 ** \param[in] Id       Identification of the routing group.
 ** \param[in] SoConId  Identification of the socket connection.
 **
 ** \returns Std_ReturnType
 ** \retval E_OK     The request was successful
 ** \retval E_NOT_OK The request was not successful

 ** \ServiceID{0x20}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_EnableSpecificRouting
(
  SoAd_RoutingGroupIdType Id,
  SoAd_SoConIdType SoConId
);

/** \brief Disable routing of a group of PDUs
 **
 ** Disables routing of a group of PDUs in the SoAd related to the RoutingGroup specified by
 ** parameter id.
 ** Routing of PDUs can be either forwarding of PDUs from the upper layer to a TCP or UDP
 ** socket of the TCP/IP stack specified by a PduRoute or the other way around specified by a
 ** SocketRoute.
 **
 ** \param[in] Id  Identification of the routing group.
 **
 ** \returns Std_ReturnType
 ** \retval E_OK     The request was successful
 ** \retval E_NOT_OK The request was not successful

 ** \ServiceID{0x0F}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_DisableRouting
(
  SoAd_RoutingGroupIdType Id
);

/** \brief Disable routing of a group of PDUs on a specific socket connection.
 **
 ** Disables routing of a group of PDUs in the SoAd related to the RoutingGroup specified by
 ** parameter id on a specific socket connection specified by SoConId.
 ** Routing of PDUs can be either forwarding of PDUs from the upper layer to a TCP or UDP
 ** socket of the TCP/IP stack specified by a PduRoute or the other way around specified by a
 ** SocketRoute.
 **
 ** \param[in] Id       Identification of the routing group.
 ** \param[in] SoConId  Identification of the socket connection.
 **
 ** \returns Std_ReturnType
 ** \retval E_OK     The request was successful
 ** \retval E_NOT_OK The request was not successful

 ** \ServiceID{0x21}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_DisableSpecificRouting
(
  SoAd_RoutingGroupIdType Id,
  SoAd_SoConIdType SoConId
);

/** \brief Set the remote address of a socket connection
 **
 ** By this API service the remote address (IP address and port) of the specified socket connection
 ** shall be set.
 **
 ** \param[in] SoConId    socket connection index specifying the socket connection for which
 **                       the remote address shall be set
 ** \param[in] IpAddrPtr  Struct containing the IP address and port to be set.
 **
 ** \returns Std_ReturnType
 ** \retval E_OK     The request was successful
 ** \retval E_NOT_OK The request was not successful

 ** \ServiceID{0x10}
 ** \Reentrancy{Reentrant for different SoConIds. Non reentrant for the same SoConId.}
 ** \Synchronicity{Synchronous}
 */

extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_SetRemoteAddr
(
  SoAd_SoConIdType SoConId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) IpAddrPtr
);

/** \brief Returns the index of a socket connection where the given remote address is set.
 **
 ** This API service shall either return the socket connection index of the
 ** SoAdSocketConnectionGroup where the specified remote address (IP address and port) is set or
 ** assign the remote address to an unused socket connection from the same
 ** SoAdSocketConnectionGroup.
 **
 ** \param[in]  SoConId             Index of any socket connection that is part of the
 **                                 SoAdSocketConnectionGroup.
 ** \param[in]  RemoteAddrPtr       Pointer to the structure containing the requested remote
 **                                 IP address and port.
 ** \param[out] AssignedSoConIdPtr  Pointer to the SoAd_SoConIdType where the index of the socket
 **                                 connection configured with the remote address (IpAddrPtr)
 **                                 shall be stored.
 **
 ** \returns Std_ReturnType
 ** \retval E_OK     The request was accepted.
 ** \retval E_NOT_OK The request was rejected, AssignedSoConIdPtr remains unchanged.
 **
 ** \ServiceID{0x1e}
 ** \Reentrancy{Reentrant for different SoConGroups. Non reentrant for the same SoConGroup.}
 ** \Synchronicity{Synchronous}
 */

extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_SetUniqueRemoteAddr
(
  SoAd_SoConIdType SoConId,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) RemoteAddrPtr,
  P2VAR(SoAd_SoConIdType, AUTOMATIC, SOAD_APPL_DATA) AssignedSoConIdPtr
);

/** \brief Change a parameter of the TCP/TP Stack
 **
 ** By this API service the SoAd or TCP/IP stack is requested to change a connection
 ** parameter. E.g. the Nagle algorithm may be controlled by this API.
 **
 ** \param[in] Id - Identification of the I-PDU which the parameter change shall affect.
 ** \param[in] Parameter - Identifier of the parameter to be changed.
 ** \param[in] Value - New parameter value.
 **
 ** \returns Std_ReturnType
 ** \retval E_OK - The request has been accepted.
 ** \retval E_NOT_OK - The request has not been accepted.
 **
 ** \ServiceID{0x11}
 ** \Reentrancy{Reentrant for different SoConIds. Non reentrant for the same SoConId.}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_TpChangeParameter
(
  PduIdType Id,
  TPParameterType Parameter,
  uint16 Value
);

/** \brief Change a parameter of the TCP/TP Stack
 **
 ** By this API service the SoAd or TCP/IP stack is requested to change a connection
 ** parameter. E.g. the Nagle algorithm may be controlled by this API.
 **
 ** \param[in] SoConId            socket connection index specifying the socket connection for which
 **                               the parameter shall be changed
 ** \param[in] ParameterId        Identifier of the parameter to be changed
 ** \param[in] ParameterValuePtr  Pointer to memory containing the new parameter value
 **
 ** \returns Std_ReturnType
 ** \retval E_OK     The request was successful
 ** \retval E_NOT_OK The request was not successful

 ** \ServiceID{0x11}
 ** \Reentrancy{Reentrant for different SoConIds. Non reentrant for the same SoConId. }
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_ChangeParameter
(
    SoAd_SoConIdType SoConId,
    uint8 ParameterId,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) ParameterValuePtr
);


/** \brief Read the DHCP hostname
 **
 ** By this API service an upper layer of the SoAd can read the currently configured
 ** hostname option in the DHCP submodule of the TCP/IP stack.
 **
 ** \param[in] SoConId            Socket connection index specifying the socket connection for
 **                               which the parameter shall be read
 ** \param[in] Length             As input parameter, contains the length of the provided data
 **                               buffer. Will be overwritten with the length of the actual data.
 ** \param[in] Data               Pointer to provided memory buffer the value will be copied to.
 **
 ** \returns Std_ReturnType
 ** \retval E_OK     The request was successful
 ** \retval E_NOT_OK The request was not successful
 **
 ** \ServiceID{0x1A}
 ** \Reentrancy{Reentrant for different SoConIds. Non reentrant for the same SoConId. }
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_ReadDhcpHostNameOption
(
    SoAd_SoConIdType SoConId,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) Length,
    P2VAR(uint8, AUTOMATIC, SOAD_APPL_DATA) Data
);

/** \brief Write the DHCP hostname
 **
 ** By this API service an upper layer of the SoAd can set the hostname option in the
 ** DHCP submodule of the TCP/IP stack.
 **
 ** \param[in] SoConId            Socket connection index specifying the socket connection for
 **                               which the parameter shall be changed
 ** \param[in] Length             Length of parameter value to be set.
 **                               will be overwritten with the length of the actual data.
 ** \param[in] Data               Pointer to memory containing the new parameter value.
 **
 ** \returns Std_ReturnType
 ** \retval E_OK     The request was successful
 ** \retval E_NOT_OK The request was not successful
 **
 ** \ServiceID{0x1B}
 ** \Reentrancy{Reentrant for different SoConIds. Non reentrant for the same SoConId. }
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_WriteDhcpHostNameOption
(
    SoAd_SoConIdType SoConId,
    uint8 Length,
    P2CONST(uint8, AUTOMATIC, SOAD_APPL_CONST) Data
);

/**\brief Main function of the SoAd.
 **
 ** This function is the main function for scheduling SoAd.
 **
 ** \ServiceID{0x19}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, SOAD_CODE) SoAd_MainFunction
(
  void
);

#if(SOAD_ENABLE_MAIN_FUNCTION_TX == STD_ON)
/** \brief Main function containing only transmit part of the SoAd
 **
 ** Transmit part of the main function if enabled to be external
 **
 ** \ServiceID{0xF2}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, SOAD_CODE) SoAd_MainFunctionTx(void) ;
#endif


/** \brief Get the remote address of a socket connection
 **
 ** Obtains the remote address (IP address and port) of the specified socket connection.
 ** If no remote address is set for the socket connection E_NOT_OK is returned.
 **
 ** \param[in]  SoConId      socket connection index representing the SoAd socket
 **                          connection from which the remote address shall be obtained.
 ** \param[out] IpAddrPtr    Pointer to the memory where the remote address is stored
 **
 ** \returns Std_ReturnType
 ** \retval E_OK     The request was successful
 ** \retval E_NOT_OK The request was not successful

 ** \ServiceID{0x1C}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetRemoteAddr
(
  SoAd_SoConIdType SoConId,
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, SOAD_APPL_DATA) IpAddrPtr
);

/** \brief Release the remote address of a socket connection
 **
 **  By this API service the remote address (IP address and port) of the specified
 **  socket connection shall be released, i.e. set back to the configured remote
 **  address setting.
 **
 ** \param[in]  SoConId      Index of the socket connection for which the remote
 **                          address shall be released.
 **
 ** \return value none

 ** \ServiceID{0x23}
 ** \Reentrancy{Reentrant for different SoConIds. Non reentrant for the same SoConId.}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, SOAD_CODE) SoAd_ReleaseRemoteAddr
(
  SoAd_SoConIdType SoConId
);

#if (SOAD_GETANDRESETMEASUREMENTDATA_API == STD_ON)
/**
 * \brief   Reads and resets measurement data.
 *
 * This service allows to read and reset detailed measurement data for diagnostic purposes.
 * Get all MeasurementIdx's at once is not supported. SOAD_MEAS_ALL shall only be used to reset
 * all MeasurementIdx's at once. A NULL_PTR shall be provided for MeasurementDataPtr in this case.
 *
 * \param[in] MeasurementIdx         Index to select specific measurement data:
 *                                   SOAD_MEAS_DROP_TCP (0x01) - Measurement index of dropped
 *                                   PDUs caused by invalid destination TCP-Port.
 *                                   SOAD_MEAS_DROP_UDP (0x02) - Measurement index of dropped
 *                                   PDUs caused by invalid destination UDP-Port.
 *                                   SOAD_MEAS_RESERVED_1 (0x03-0x7F) - Reserved by AUTOSAR.
 *                                   SOAD_MEAS_RESERVED_2 (0x80-0xEF) - Vendor specific range.
 *                                   SOAD_MEAS_RESERVED_3 (0xF0-0xFE) - Reserved by AUTOSAR
 *                                                                       (future use).
 *                                   SOAD_MEAS_ALL (0xFF) - Represents all measurement indexes.
 * \param[in] MeasurementResetNeeded Flag to trigger a reset of the measurement data.
 * \param[out] MeasurementDataPtr    Pointer to data buffer, where to copy measurement data.
 *
 * \returns Std_ReturnType
 * \retval  E_OK     The function has been successfully executed.
 * \retval  E_NOT_OK The function could not be successfully executed.
 * \ServiceID{0x45}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SOAD_CODE) SoAd_GetAndResetMeasurementData
(
  SoAd_MeasurementIdxType MeasurementIdx,
  boolean MeasurementResetNeeded,
  P2VAR(uint32, AUTOMATIC, SOAD_APPL_DATA) MeasurementDataPtr
);
#endif /* SOAD_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#define SOAD_STOP_SEC_CODE
#include <SoAd_MemMap.h>


/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef SOAD_H */
/*==================[end of file]===========================================*/
