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
/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type
 *
 * Reason:
 * The type of variable holding the address expected by SoAd_GetRemoteAddr and
 * SoAd_SetRemoteAddr functions is different from the type used in XcpR.
 *
 */

/*==[Includes]================================================================*/

#include <XcpR.h>
#include <XcpR_Int.h>
#include <XcpR_Cbk.h>
#include <SchM_XcpR.h>      /* Exclusive area definitions */
#include <TsMem.h>
#if (XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED == STD_ON)
#include <TcpIp.h>          /* TcpIp types */
#endif
/*==[Macros]==================================================================*/

/*==[Types]===================================================================*/


/*==[Includes]================================================================*/

/*==[Macros]==================================================================*/

/*==[Types]===================================================================*/

/*==[Declaration of functions with internal linkage]==========================*/
#if ((XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON))
#if (XCPR_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON)
/** \brief Function used for error handling an Ethernet source
 ** */
STATIC FUNC(void, XCPR_CODE) XcpR_SoAdRemoteAddrErrHandling
(
  void
);
#endif
#endif
/*==[Constants with internal linkage]=========================================*/

/*==[Variables with internal linkage]=========================================*/

#define XCPR_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <XcpR_MemMap.h>

/* Status of XCPR module */
VAR(XcpR_StatusType, XCPR_VAR) XcpR_Status;

#define XCPR_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <XcpR_MemMap.h>

#define XCPR_START_SEC_VAR_8
#include <XcpR_MemMap.h>

/* Initialization status of the XCPR module */
VAR(boolean, XCPR_VAR) XcpR_Initialized = FALSE;

#define XCPR_STOP_SEC_VAR_8
#include <XcpR_MemMap.h>

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/

/*==[Definition of functions with internal linkage]===========================*/

#define XCPR_START_SEC_CODE
#include <XcpR_MemMap.h>

#if (XCPR_VERSION_INFO_API == STD_ON)
/*------------------[XcpR_GetVersionInfo]-------------------------------------*/

/* !LINKSTO XcpR.API.GetVersionInfo, 1 */
FUNC(void, XCPR_CODE) XcpR_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, XCPR_APPL_DATA) VersionInfoPtr
)
{
#if (XCPR_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR == VersionInfoPtr)
  {
    XCPR_DET_REPORT_ERROR(XCPR_SID_GET_VERSION_INFO, XCPR_E_NULL_POINTER);
  }
  else
#endif
  {
    VersionInfoPtr->vendorID         = XCPR_VENDOR_ID;
    VersionInfoPtr->moduleID         = XCPR_MODULE_ID;
    VersionInfoPtr->sw_major_version = XCPR_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version = XCPR_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version = XCPR_SW_PATCH_VERSION;
  }
}

#endif /* (XCPR_VERSION_INFO_API == STD_ON) */

/*------------------[XcpR_Init]-----------------------------------------------*/

/* !LINKSTO XcpR.API.Init, 1 */
FUNC(void, XCPR_CODE) XcpR_Init
(
  void
)
{
#if ((XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON))
  uint8 i;
#endif

  XcpR_FlushMsgQueue();
  XcpR_InitReceiveProcessor();
  XcpR_InitTransmitProcessor();

  XcpR_Status.ConnectionStatus = XCPR_STATE_DISCONNECTED;
  XcpR_Status.isPreviousCommandConnect = FALSE;
  XcpR_Status.isWaitingForConnectConfirmation = FALSE;

  XcpR_Status.MasterSource.Id = (PduIdType) XCPR_INVALID_PDU_ID;
  XcpR_Status.MasterSource.Type = XCPR_INVALID_SOURCE_TYPE;
  XcpR_Status.DestinationFromMaster.Id = (PduIdType) XCPR_INVALID_PDU_ID;
  XcpR_Status.DestinationFromMaster.Type = XCPR_INVALID_DESTINATION_TYPE;
  XcpR_Status.DestinationFromMasterInternalId = (PduIdType) XCPR_INVALID_PDU_ID;

  XcpR_Status.SlaveSource.Id = (PduIdType) XCPR_INVALID_PDU_ID;
  XcpR_Status.SlaveSource.Type = XCPR_INVALID_SOURCE_TYPE;
  XcpR_Status.DestinationFromSlave.Id = (PduIdType) XCPR_INVALID_PDU_ID;
  XcpR_Status.DestinationFromSlave.Type = XCPR_INVALID_DESTINATION_TYPE;
  XcpR_Status.DestinationFromSlaveInternalId = (PduIdType) XCPR_INVALID_PDU_ID;

  XcpR_Status.CurrentSourceOrigin = XCPR_UNKNOWN_SRC_ORIGIN;
  XcpR_Status.ConnectionMode = XCPR_INVALID_CONNECTION_MODE;

#if (XCPR_MULTIPLE_ROUTING_PER_CONNECTION == STD_ON)
  XcpR_Status.CurrentSrcLinkedDestInternalId = (PduIdType) XCPR_INVALID_PDU_ID;
  XcpR_Status.ActiveConnectionGroup = NULL_PTR;
#endif

#if (((XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON)))
  XcpR_Status.EthernetActiveIPv6Address.domain = (TcpIp_DomainType) XCPR_INVALID_ETHERNET_DOMAIN;
  XcpR_Status.EthernetActiveIPv6Address.port = (uint16) XCPR_INVALID_ETHERNET_PORT;
  for (i= 0U; i < XCPR_SOAD_IPV6ADDRSIZE; i++)
  {
    XcpR_Status.EthernetActiveIPv6Address.addr[i] = XCPR_INVALID_ETHERNET_ADDRESS;
  }

  XcpR_Status.EthernetActiveIPv4Address.domain = (TcpIp_DomainType) XCPR_INVALID_ETHERNET_DOMAIN;
  XcpR_Status.EthernetActiveIPv4Address.port = (uint16) XCPR_INVALID_ETHERNET_PORT;
  XcpR_Status.EthernetActiveIPv4Address.addr[0U] = XCPR_INVALID_ETHERNET_ADDRESS;
#endif

#if (((XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON)) && (XCPR_OPEN_SO_CON == STD_ON))
  if (XcpR_InitSoAdConnections() != E_OK)
  {
    XcpR_Initialized = FALSE;
  }
  else
#endif /* ((XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_OPEN_SO_CON == STD_ON)) */
  {
    XcpR_Initialized = TRUE;
  }
}

/*------------------[XcpR_MainFunction]-----------------------------------------------*/

/* !LINKSTO XcpR.API.MainFunction, 1 */
FUNC(void, XCPR_CODE) XcpR_MainFunction(void)
{
  /* Variable used to iterate over all configured SrcPdus */
  PduIdType SrcIterator;

  if (XcpR_Initialized == TRUE)
  {
    /* process the bus monitor */
    XcpR_ProcessBusMonitor();

    /* iterate over all configured SrcPduIds */
    for(SrcIterator = 0U; SrcIterator < XCPR_NO_OF_RX_SRC_PDU_IDS; SrcIterator++)
    {
      XcpR_ProcessSource(XCPR_SID_MAIN_FUNCTION, SrcIterator, XCPR_RX_SRC_TYPE);
    }
    for(SrcIterator = XCPR_NO_OF_RX_SRC_PDU_IDS; SrcIterator < XCPR_NO_OF_SRC_PDU_IDS; SrcIterator++)
    {
      XcpR_ProcessSource(XCPR_SID_MAIN_FUNCTION, SrcIterator, XCPR_TX_SRC_TYPE);
    }

    /* After all sources have been considered, process their destinations */
    XcpR_ProcessDestinations();
  }
  else
  {
    /* Nothing to do. */
  }
}

/*------------------[XcpR_RxIndication]----------------------------------*/

/* !LINKSTO XcpR.API.RxIndication, 1 */
FUNC(void, XCPR_CODE) XcpR_RxIndication
(
  PduIdType XcpRRxSrcPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCPR_APPL_DATA) XcpRRxSrcPduPtr
)
{
  PduIdType Conf_SrcInternalPduId = (PduIdType) XCPR_INVALID_PDU_ID;
  PduIdType RxSrcIterator = 0;
#if (XCPR_DEV_ERROR_DETECT == STD_ON)
  PduLengthType Conf_SrcPduLengthMax = 0U;
#endif /* (XCPR_DEV_ERROR_DETECT == STD_ON) */

  if (XcpR_Initialized == FALSE)
  {
#if (XCPR_DEV_ERROR_DETECT == STD_ON)
    XCPR_DET_REPORT_ERROR(XCPR_SID_IF_RX_INDICATION, XCPR_E_NOT_INITIALIZED);
#endif /* (XCPR_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    if (XcpRRxSrcPduId >= XCPR_NO_OF_RX_SRC_PDU_IDS)
    {
#if (XCPR_DEV_ERROR_DETECT == STD_ON)
      XCPR_DET_REPORT_ERROR(XCPR_SID_IF_RX_INDICATION, XCPR_E_INVALID_PDUID);
#endif /* (XCPR_DEV_ERROR_DETECT == STD_ON) */
    }
    else
    {

      /* Identify the PDU configuration as an entry in the XcpR_SrcConf */
      for (RxSrcIterator = 0U; RxSrcIterator < XCPR_NO_OF_RX_SRC_PDU_IDS; RxSrcIterator++)
      {
        if (XcpRRxSrcPduId == XcpR_SrcConf[RxSrcIterator].XcpRSrcPduId)
        {
          Conf_SrcInternalPduId = XcpR_SrcConf[RxSrcIterator].IntPduBufId;
        }
      }

#if (XCPR_DEV_ERROR_DETECT == STD_ON)
      Conf_SrcPduLengthMax  = XcpR_SrcConf[Conf_SrcInternalPduId].PduAttributes.PduLengthMax;

      if ((XcpRRxSrcPduPtr == NULL_PTR)|| (XcpRRxSrcPduPtr->SduDataPtr == NULL_PTR))
      {
        XCPR_DET_REPORT_ERROR(XCPR_SID_IF_RX_INDICATION, XCPR_E_NULL_POINTER);
      }
      else if (XcpRRxSrcPduPtr->SduLength > Conf_SrcPduLengthMax)
      {
        XCPR_DET_REPORT_ERROR(XCPR_SID_IF_RX_INDICATION, XCPR_E_INVALID_RX_PDU_LENGTH);
      }
      else
#endif /* (XCPR_DEV_ERROR_DETECT == STD_ON) */
      {
        (void)XcpR_ReceiveRxData(XcpRRxSrcPduId, Conf_SrcInternalPduId, (P2CONST(PduInfoType, AUTOMATIC, XCPR_APPL_DATA))XcpRRxSrcPduPtr, XCPR_RX_SRC_TYPE);
      }
    }
  }
}

/*-----------------------------[XcpR_Transmit]---------------------*/
#if (XCPR_NO_OF_TX_SOURCE_PDUS > 0)
/* !LINKSTO XcpR.API.Transmit, 1 */
FUNC(Std_ReturnType, XCPR_CODE) XcpR_Transmit
(
  PduIdType TxPduId,
  P2CONST(PduInfoType, AUTOMATIC, XCPR_APPL_CONST) PduInfoPtr
)
{
  PduIdType RxSrcIterator;
  PduIdType Conf_SrcInternalPduId = (PduIdType) XCPR_INVALID_PDU_ID;

  Std_ReturnType RetVal = E_NOT_OK;
#if (XCPR_DEV_ERROR_DETECT == STD_ON)
  PduLengthType Conf_SrcPduLengthMax = (PduLengthType) XCPR_INVALID_PDU_LENGTH;
#endif /* (XCPR_DEV_ERROR_DETECT == STD_ON) */

  if (XcpR_Initialized == FALSE)
  {
#if (XCPR_DEV_ERROR_DETECT == STD_ON)
    XCPR_DET_REPORT_ERROR(XCPR_SID_IF_TRANSMIT, XCPR_E_NOT_INITIALIZED);
#endif /* (XCPR_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    if (TxPduId >= XCPR_NO_OF_TX_SRC_PDU_IDS)
    {
#if (XCPR_DEV_ERROR_DETECT == STD_ON)
      XCPR_DET_REPORT_ERROR(XCPR_SID_IF_TRANSMIT, XCPR_E_INVALID_PDUID);
#endif /* (XCPR_DEV_ERROR_DETECT == STD_ON) */
    }
    else
    {
      /* Identify the PDU configuration as an entry in the XcpR_SrcConf */
      for (RxSrcIterator = XCPR_NO_OF_RX_SRC_PDU_IDS; RxSrcIterator < XCPR_NO_OF_SRC_PDU_IDS; RxSrcIterator++)
      {
        if (TxPduId == XcpR_SrcConf[RxSrcIterator].XcpRSrcPduId)
        {
          Conf_SrcInternalPduId = XcpR_SrcConf[RxSrcIterator].IntPduBufId;
        }
      }

#if (XCPR_DEV_ERROR_DETECT == STD_ON)
      /* Get the value from configuration (XcpR_Transmit() is called with a Tx PDU, but represents a source for XcpR) */
      Conf_SrcPduLengthMax = XcpR_SrcConf[Conf_SrcInternalPduId].PduAttributes.PduLengthMax;

      if ((PduInfoPtr == NULL_PTR)|| (PduInfoPtr->SduDataPtr == NULL_PTR))
      {
        XCPR_DET_REPORT_ERROR(XCPR_SID_IF_TRANSMIT, XCPR_E_NULL_POINTER);
      }
      else if (PduInfoPtr->SduLength > Conf_SrcPduLengthMax)
      {
        XCPR_DET_REPORT_ERROR(XCPR_SID_IF_TRANSMIT, XCPR_E_INVALID_TX_PDU_LENGTH);
      }
      else
#endif /* (XCPR_DEV_ERROR_DETECT == STD_ON) */
      {
        RetVal = XcpR_ReceiveRxData(TxPduId, Conf_SrcInternalPduId, PduInfoPtr, XCPR_TX_SRC_TYPE);
      }
    }
  }

  return RetVal;
}
#endif

/*-----------------------------[XcpR_TxConfirmation]---------------------*/

/* !LINKSTO XcpR.API.TxConfirmation, 1 */
FUNC(void, XCPR_CODE) XcpR_TxConfirmation
(
  PduIdType XcpRTxPduId
)
{
  if (XcpR_Initialized == FALSE)
  {
#if (XCPR_DEV_ERROR_DETECT == STD_ON)
    XCPR_DET_REPORT_ERROR(XCPR_SID_IF_TX_CONFIRMATION, XCPR_E_NOT_INITIALIZED);
#endif /* (XCPR_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    XcpR_IndicateTxConfirmation(XcpRTxPduId);
  }
}

#if ((XCPR_FR_TRIGGER_TX_API_ENABLED == STD_ON) || (XCPR_CDD_TRIGGER_TX_API_ENABLED == STD_ON))

/*-----------------------------[XcpR_TriggerTransmit]---------------------*/

/* !LINKSTO XcpR.API.TriggerTransmit, 1 */
FUNC(Std_ReturnType, XCPR_CODE) XcpR_TriggerTransmit
(
  PduIdType XcpRTxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCPR_APPL_DATA) PduInfoPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  if (FALSE == XcpR_Initialized)
  {
#if (XCPR_DEV_ERROR_DETECT == STD_ON)
    XCPR_DET_REPORT_ERROR(XCPR_SID_IF_TRIGGER_TRANSMIT, XCPR_E_NOT_INITIALIZED);
#endif
  }
  else
  {
#if (XCPR_DEV_ERROR_DETECT == STD_ON)
    if (XcpRTxPduId >= XCPR_NO_OF_DEST_PDU_IDS)
    {
      XCPR_DET_REPORT_ERROR(XCPR_SID_IF_TRIGGER_TRANSMIT, XCPR_E_INVALID_PDUID);
    }
    else if ((PduInfoPtr == NULL_PTR) || (PduInfoPtr->SduDataPtr == NULL_PTR))
    {
      XCPR_DET_REPORT_ERROR(XCPR_SID_IF_TRIGGER_TRANSMIT, XCPR_E_NULL_POINTER);
    }
    else
#endif
    {
      /* Get data to be transmitted */
      RetVal = XcpR_ProvideTxData(XcpRTxPduId, PduInfoPtr);
    }
  }

  return RetVal;
}

#endif /* #if ((XCPR_FR_TRIGGER_TX_API_ENABLED == STD_ON) || (XCPR_CDD_TRIGGER_TX_API_ENABLED == STD_ON)) */

#if ((XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON))
#if (XCPR_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON)
/*------------------[XcpR_CopySoAdRemoteAddress]-----------------------------------*/

FUNC(void, XCPR_CODE) XcpR_CopySoAdRemoteAddress
(
  SoAd_SoConIdType SourceSocketId,
  boolean IsConnectCmd,
  boolean IsIPv6
)
{
  SoAd_SoConIdType DestSocketId;

  /* Variable to hold the status for get and set Remote Address */
  boolean RemoteAddrWrongSts = FALSE;

  /* We need a conversion to TcpIp_SockAddrInetType or TcpIp_SockAddrInet6Type
  in order to gain access to the port stored at the remote address  */

  /* Variable to hold the remote address received on the Rx socket */
  TcpIp_SockAddrInet6Type RemoteAddressV6;
  /* Variable to hold the remote address received on the Rx socket */
  TcpIp_SockAddrInetType RemoteAddress;


  /* Pointer to the remote address variable, of type expected by SoAd functions */
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, XCP_CODE) SoAdRemoteAddressPtr;

  /* Fill in the required domain */
  if (IsIPv6 == TRUE)
  {
    /* Deviation MISRAC2012-1 <START> */
    SoAdRemoteAddressPtr = (P2VAR(TcpIp_SockAddrType, AUTOMATIC, XCP_CODE))&RemoteAddressV6;
    /* Deviation MISRAC2012-1 <STOP> */
    RemoteAddressV6.domain = (TcpIp_DomainType)TCPIP_AF_INET6;
  }
  else
  {
    /* Deviation MISRAC2012-1 <START> */
    SoAdRemoteAddressPtr = (P2VAR(TcpIp_SockAddrType, AUTOMATIC, XCP_CODE))&RemoteAddress;
    /* Deviation MISRAC2012-1 <STOP> */
    RemoteAddress.domain = (TcpIp_DomainType)TCPIP_AF_INET;
  }

  if (IsConnectCmd)
  {
    /* Get the current address for the Rx socket */
    if (SoAd_GetRemoteAddr(SourceSocketId, SoAdRemoteAddressPtr) == E_OK)
    {
      uint8 i;

      for (i = 0U; i < XCPR_NO_OF_DESTINATION_PDUS; i++)
      {
        /* Set the SoAdRemoteAddr for all configured XcpR UDPIP destinations */
        if ((XcpR_DestConf[i].IsMasterConnection == TRUE) && (XcpR_DestConf[i].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_UDPIP) && (XcpR_DestConf[i].IsIPv6 == IsIPv6))
        {
          DestSocketId = XcpR_DestConf[i].SoAdTxConId;

          if (SoAd_SetRemoteAddr(DestSocketId, SoAdRemoteAddressPtr) != E_OK)
          {
            RemoteAddrWrongSts = TRUE;
          }
        }
      }

      if (RemoteAddrWrongSts == FALSE)
      {
        SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

        if (IsIPv6 == TRUE)
        {
          XcpR_Status.EthernetActiveIPv6Address.domain = RemoteAddressV6.domain;
          XcpR_Status.EthernetActiveIPv6Address.port = RemoteAddressV6.port;
          TS_MemCpy( XcpR_Status.EthernetActiveIPv6Address.addr, RemoteAddressV6.addr, XCPR_SOAD_IPV6ADDRMEMSIZE);
        }
        else
        {
          XcpR_Status.EthernetActiveIPv4Address.domain = RemoteAddress.domain;
          XcpR_Status.EthernetActiveIPv4Address.port = RemoteAddress.port;
          XcpR_Status.EthernetActiveIPv4Address.addr[0U] = RemoteAddress.addr[0U];
        }

        SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
      }
    }
    else
    {
      RemoteAddrWrongSts = TRUE;
    }
  }

  /* Get the current address for the Rx socket */
  if (SoAd_GetRemoteAddr(SourceSocketId, SoAdRemoteAddressPtr) == E_OK)
  {
    /* We reconfigure the socket only if the Rx socket was being reconfigured to not accept all ports */
    if ((IsIPv6 == TRUE) && (RemoteAddressV6.port != 0U))
    {
      /* set the Rx socket to 0 (wild card, i.e. accept all ports)*/
      TS_AtomicAssignGeneric(RemoteAddressV6.port, 0U);
      if (SoAd_SetRemoteAddr(SourceSocketId, SoAdRemoteAddressPtr) != E_OK)
      {
        RemoteAddrWrongSts = TRUE;
      }
    }
    else if ((IsIPv6 == FALSE) && (RemoteAddress.port != 0U))
    {
      TS_AtomicAssignGeneric(RemoteAddress.port, 0U);
      if (SoAd_SetRemoteAddr(SourceSocketId, SoAdRemoteAddressPtr) != E_OK)
      {
        RemoteAddrWrongSts = TRUE;
      }
    }
    else
    {
      /* Nothing to do */
    }
  }
  else
  {
    RemoteAddrWrongSts = TRUE;
  }

  if (RemoteAddrWrongSts == FALSE)
  {
    SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

    if (XcpR_Status.ConnectionStatus == XCPR_STATE_CONNECTED)
    {
       if (IsIPv6 == TRUE)
       {
          if (TS_MemCmp(XcpR_Status.EthernetActiveIPv6Address.addr,RemoteAddressV6.addr,XCPR_SOAD_IPV6ADDRSIZE) == E_NOT_OK)
          {
            /* if the received packet comes from a different IP than the one which originated the CONNECT command,
             * the packet will be discarded  */
            XcpR_RemoveLastMsgFromQueue();

            XcpR_ReportDetProcessDestCtr[XCPR_DET_REPORT_E_PDU_LOST] += 1U;
          }
       }
       else
       {
         if (XcpR_Status.EthernetActiveIPv4Address.addr[0U] != RemoteAddress.addr[0U])
         {
           /* if the received packet comes from a different IP than the one which originated the CONNECT command,
            * the packet will be discarded  */
           XcpR_RemoveLastMsgFromQueue();

           XcpR_ReportDetProcessDestCtr[XCPR_DET_REPORT_E_PDU_LOST] += 1U;
         }
       }
    }

    /* Exit critical section.*/
    SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
  }
  else
  {
    XcpR_SoAdRemoteAddrErrHandling();
  }

  XcpR_TriggerDetErrorsProcessDest();
}

STATIC FUNC(void, XCPR_CODE) XcpR_SoAdRemoteAddrErrHandling
(
  void
)
{
  boolean TriggerDisconnect = FALSE;
  PduIdType LocalDestFromMasterInternalId = (PduIdType) ~0U;

  /* enter critical section*/
  SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

  if (XcpR_Status.ConnectionStatus == XCPR_STATE_CONNECTED)
  {
    /* Consume the data from the queue since the last command will not be sent to the Xcp slave */
    XcpR_RemoveLastMsgFromQueue();

    TriggerDisconnect = TRUE;
  }
  else
  {
    /* If XcpR is disconnected this branch is only reached with a CONNECT command, otherwise the packet is discarded
     * in XcpR_ReceiveRxData().
     * Consume the data from the queue since the CONNECT command will not be sent to the Xcp slave */
    XcpR_RemoveLastMsgFromQueue();
  }

  LocalDestFromMasterInternalId = XcpR_Status.DestinationFromMasterInternalId;

  SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

  if (TriggerDisconnect == TRUE)
  {
    /* the socket connection is not reliable and therefore the connection will be terminated
     * XcpR will emit a session terminated event to the Xcp master and disconnect from Xcp slave */
    XcpR_TriggerDisconnectCommand(LocalDestFromMasterInternalId, TRUE);
  }
}

#endif /* (XCPR_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON) */

FUNC(void, XCPR_CODE) XcpR_RemoveLastMsgFromQueue(void)
{
  uint8 XcpR_ExtractedMessage[XCPR_MAX_PACKAGE_SIZE_RECEPTION + 1U];
  PduLengthType extractedPduLength;

  while (XcpR_IsMsgQueueEmpty() == FALSE)
  {
    extractedPduLength = XcpR_GetMsgLength();
    (void)XcpR_GetMsgSrcPduId();
    (void)XcpR_GetMsgSrcPduType();

    XcpR_DeQueueMsgQueue((extractedPduLength - XCPR_INTERNAL_DATA_SIZE), &XcpR_ExtractedMessage[0U]);
  }
}

#if (XCPR_SOAD_PROTOCOL_TCP_SUPPORT == STD_ON)
/*------------------[XcpR_SoAdSoConModeChg]-----------------------------------*/

FUNC(void,XCP_CODE) XcpR_SoAdSoConModeChg
(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode
)
{
  uint8 Iterator;

#if (XCPR_DEV_ERROR_DETECT == STD_ON)
    /* Check whether XcpR is initialized */
    if (XcpR_Initialized == FALSE)
    {
      XCPR_DET_REPORT_ERROR(XCPR_SID_SOAD_SO_CON_MODE_CHG, XCPR_E_NOT_INITIALIZED);
    }
    else
#endif
    {
      if (XcpR_Status.ConnectionStatus == XCPR_STATE_CONNECTED)
      {
        /* search throughout configured SoAd TcpIp connections */
        for (Iterator = 0U; Iterator < XCPR_NO_OF_TCPIP_SOCKET_CON; Iterator++)
        {
          /* check if the Socket connection id belongs to any of the configured
           * XcpR destinations over TcpIp. */
          if (XcpR_TcpIpSocketDestMapCfg[Iterator].SoAdConId == SoConId)
          {
            /* check if the selected XcpR destination is part of the active connection */
            if ((XcpR_TcpIpSocketDestMapCfg[Iterator].XcpRIntDestId == XcpR_Status.DestinationFromMasterInternalId) ||
                (XcpR_TcpIpSocketDestMapCfg[Iterator].XcpRIntDestId == XcpR_Status.DestinationFromSlaveInternalId))
            {
              /* If XcpR socket connection is not ONLINE */
              if (Mode != SOAD_SOCON_ONLINE)
              {
                /* the socket connection is not reliable and therefore the connection will be terminated
                 * XcpR will emit a session terminated event to the Xcp master and disconnect from Xcp slave */
                XcpR_TriggerDisconnectCommand(XcpR_Status.DestinationFromMasterInternalId, TRUE);
              }

              /* the Id is found and is part of the active connection, so the processing will be stopped */
              break;
            }
          }
        }
      }
    }

}
#endif /* (XCPR_SOAD_PROTOCOL_TCP_SUPPORT == STD_ON) */
#endif /* (XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED == STD_ON) */

#define XCPR_STOP_SEC_CODE
#include <XcpR_MemMap.h>

