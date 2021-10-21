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
 * MISRAC2012-1) Deviated Rule: 12.2 (required)
 * The right hand operand of a shift operator shall lie in the range zero to one less
 * than the width in bits of the essential type of the left hand operand.
 *
 * Reason:
 * This is a false positive violation of rule 12.2. Shifting right with 8, 16 and 24
 * bits is only done if sizeof(type) is sufficiently large.
 *
 * MISRAC2012-2) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type
 *
 * Reason:
 * SoAd API's use TcpIp_SockAddrType* but either a TcpIp_SockAddrInetType* or TcpIp_SockAddrInet6Type* 
 * must be passed
 *
 */

/*==[Includes]================================================================*/

#include <TSMem.h>          /* EB memory functions */
/* !LINKSTO XcpR.dsn.SchMInclusion, 1 */
#include <SchM_XcpR.h>      /* Exclusive area definitions */
#include <XcpR.h>
#include <XcpR_Int.h>

/*==[Macros]==================================================================*/

/* Deviation MISRAC2012-1 */
#define XCPR_ENQUEUE_VALUE(value, type)                 \
do                                                      \
{                                                       \
  XcpR_EnqueueByteInMsgQueue((uint8) (value));          \
  if (sizeof (type) >= 2U)                              \
  {                                                     \
    XcpR_EnqueueByteInMsgQueue((uint8)((value) >> 8U)); \
  }                                                     \
  if (sizeof (type) == 4U)                              \
  {                                                     \
    XcpR_EnqueueByteInMsgQueue((uint8)((value) >> 16U));\
    XcpR_EnqueueByteInMsgQueue((uint8)((value) >> 24U));\
  }                                                     \
}while(0)

/*==[Types]===================================================================*/

typedef struct
{
  /* ------------- Src Pdu buffer data ------------- */
  /* Length of each Src Pdu */
  PduLengthType PduLength[XCPR_NO_OF_SRC_PDU_IDS];
  /* store the XcpR SrcPduId of the stored data */
  PduIdType PduIntId[XCPR_NO_OF_SRC_PDU_IDS];
  /* Src Pdu buffer used to contingently store the Src PDU data, for all available PDUs */
  uint8 PduBuffer[XCPR_SRC_BUFFER_SIZE];
  /* bit flag indicating if the buffer has new data in it. */
  uint8 IsBufferFullFlag[XCPR_SRC_PDU_FLAG_SIZE];

} XcpR_SrcPduDataType;

/*==[Declaration of functions with internal linkage]==========================*/

#define XCPR_START_SEC_CODE
#include <XcpR_MemMap.h>

/** \brief Function to get the length of the XCPR data segment.
 **
 ** \param[in]  RemainingRxSpace      Remaining frame space to unpack inside I-PDU
 ** \param[in]  SduDataPtr            Pointer to I-PDU
 ** \param[out] DataLengthPtr         Length of the XCPR data segment
 ** \param[out] TailLengthPtr         Length of the XCPR tail
 ** \param[in]  HeaderLength          Length of the XCPR header
 ** \param[in]  Conf_SrcInternalPduId Received PDU Id.
 ** \return     If the package length has been successfully retrieved
 ** \retval     E_OK the package length has been successfully retrieved
 ** \retval     E_NOT_OK the package length could not be retrieved
 **
 **/
STATIC FUNC(Std_ReturnType, XCPR_CODE) XcpR_GetPduDataLength
(
  PduLengthType RemainingRxSpace,
  P2CONST(uint8, AUTOMATIC, AUTOSAR_COMSTACKDATA) SduDataPtr,
  P2VAR(PduLengthType, AUTOMATIC, XCPR_VAR) DataLengthPtr,
  P2VAR(PduLengthType, AUTOMATIC, XCPR_VAR) TailLengthPtr,
  PduLengthType HeaderLength,
  PduIdType Conf_SrcInternalPduId
);

#if ((XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON))
/** \brief Function used for handling an Ethernet source
 ** \param[in]  Conf_SrcInternalPduId      Configured source internal PduId
 ** \param[in]  SoAdFramePending           Flag for indicating currently processed SoAd frame
 ** */
STATIC FUNC(void, XCPR_CODE) XcpR_ProcessEthernetSource
(
  PduIdType Conf_SrcInternalPduId,
  boolean SoAdFramePending
);
#endif

#if (XCPR_SRC_ON_FLEXRAY_FROM_LOWER_LAYER_ENABLED == STD_ON)
/** \brief Function used for handling an FlexRay source
 ** \param[in]  Conf_SrcInternalPduId      Configured source internal PduId
 ** \param[in]  ConnectMode                Received connection mode (local or remote Xcp slave)
 ** */
STATIC FUNC(void, XCPR_CODE) XcpR_ProcessFlexRaySource
(
  PduIdType Conf_SrcInternalPduId,
  uint8 ConnectMode
);
#endif

#define XCPR_STOP_SEC_CODE
#include <XcpR_MemMap.h>

/*==[Constants with internal linkage]=========================================*/

/*==[Variables with internal linkage]=========================================*/

#define XCPR_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <XcpR_MemMap.h>

/** \brief Source queue containing source data and length information */
STATIC VAR(XcpR_SrcPduDataType,  XCPR_VAR_CLEARED) XcpR_SrcPduData;

/* The internal XcpR message queue. */
VAR(XcpR_MsgQueueType, XCPR_VAR_CLEARED) XcpR_MsgQueue;

#define XCPR_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <XcpR_MemMap.h>

#define XCPR_START_SEC_VAR_8
#include <XcpR_MemMap.h>

#if ((XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON))
/** \brief Flag to signal that a SoAd UDP or TCP socket configuration is needed */
STATIC VAR(boolean, XCPR_VAR) XcpR_ConfigureSocketConnect = FALSE;
#endif

#define XCPR_STOP_SEC_VAR_8
#include <XcpR_MemMap.h>

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/

#define XCPR_START_SEC_CODE
#include <XcpR_MemMap.h>

/*------------------[XcpR_InitReceiveProcessor]-----------------------------*/

FUNC(void, XCPR_CODE) XcpR_InitReceiveProcessor(void)
{
  PduIdType Iterator;

  for(Iterator = 0U; Iterator < XCPR_NO_OF_SRC_PDU_IDS; Iterator++)
  {
    /* --- clear Src Pdu buffer data --- */
    XcpR_SrcPduData.PduLength[Iterator] = 0U;
  }

  for(Iterator = 0U; Iterator < XCPR_SRC_PDU_FLAG_SIZE; Iterator++)
  {
    XcpR_SrcPduData.IsBufferFullFlag[Iterator] = 0U;
  }
}

/*------------------[XcpR_ReceiveRxData]------------------------------------*/

FUNC(Std_ReturnType, XCPR_CODE) XcpR_ReceiveRxData
(
  PduIdType XcpRSrcPduId,
  PduIdType Conf_SrcInternalPduId,
  P2CONST(PduInfoType, AUTOMATIC, XCPR_APPL_DATA) XcpRRxSrcPduPtr,
  uint8 SrcPduType
)
{
  /* Pointer to XcpR Src runtime data */
  P2VAR(uint8, AUTOMATIC, XCPR_VAR_CLEARED) XcpRSrcBufferPtr;

  Std_ReturnType RetVal = E_NOT_OK;

  /* local variables to hold configured data */
  PduLengthType Conf_SrcPduHeaderLength;
  uint8 Pid;

#if (XCPR_SRC_ON_FLEXRAY_FROM_LOWER_LAYER_ENABLED == STD_ON)
  boolean IsPackageAccepted = TRUE;
#endif

  boolean ReportDetPduBufferFull = FALSE;

#if (XCPR_MULTIPLE_ROUTING_PER_CONNECTION == STD_ON)
  boolean RequestAccepted = FALSE;
  uint8 Iterator;
#endif

  /* Get PDU header length from configuration */
  Conf_SrcPduHeaderLength = XcpR_SrcConf[Conf_SrcInternalPduId].PduAttributes.PduHeaderLength;

  /* get PDU start position inside the SrcPdu buffer */
  XcpRSrcBufferPtr = &(XcpR_SrcPduData.PduBuffer[XcpR_SrcConf[Conf_SrcInternalPduId].PduStartPos]);

#if (XCPR_SRC_ON_FLEXRAY_FROM_LOWER_LAYER_ENABLED == STD_ON)
  if (XcpR_SrcConf[Conf_SrcInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY)
  {
    /* Validate received Nax info with configured Src Nax */
    if(XcpRRxSrcPduPtr->SduDataPtr[0U] != XcpR_SrcConf[Conf_SrcInternalPduId].BusAttributes.FlexRayNax)
    {
      IsPackageAccepted = FALSE;
    }
  }
  if (IsPackageAccepted == TRUE)
#endif /* (XCPR_SRC_ON_FLEXRAY_FROM_LOWER_LAYER_ENABLED == STD_ON) */
  {
    if (XcpR_Status.ConnectionStatus == XCPR_STATE_DISCONNECT_REQUESTED)
    {
      if (XcpR_SrcConf[Conf_SrcInternalPduId].IsMasterConnection == FALSE)
      {
        XcpR_ProcessAutonomousDisconnectResponse();
      }
    }
    else
    {
#if (XCPR_MULTIPLE_ROUTING_PER_CONNECTION == STD_ON)
      if (XcpR_Status.ConnectionStatus == XCPR_STATE_CONNECTED)
      {
        /* search PduId in all Routing Paths from the Active Connection Group */
        for  (Iterator = 0U; Iterator < XCPR_MAX_ROUTING_PATHS_PER_CONNECTION; Iterator++)
        {
          /* Check if XcpRSrcPduId is found in the Active Connection Group */
          if (XcpRSrcPduId == (XcpR_Status.ActiveConnectionGroup->RoutingPath[Iterator].XcpRSrcPduId))
          {
            SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
            /* Search for the linked DestinationPduId - saved in XcpR_Status.CurrentSrcLinkedDestInternalId */
            XcpR_FindDestinationPduId(XCPR_INVALID_PID, XcpR_Status.ConnectionMode, Conf_SrcInternalPduId, XcpRSrcPduId, SrcPduType, XCPR_SID_RECEIVE_RX_DATA);
            SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

            if (XcpR_Status.CurrentSrcLinkedDestInternalId == XcpR_Status.ActiveConnectionGroup->RoutingPath[Iterator].RoutedDestinationPtr->IntPduBufId)
            {
              /* !LINKSTO XcpR.Impl.ConnectionGroups.ActiveGroupNewRxRequest, 1 */
              RequestAccepted = TRUE;
              break;
            }
          }
        }
      }
      else if ((XcpRRxSrcPduPtr->SduDataPtr[Conf_SrcPduHeaderLength] == 0xFF) || (XcpRRxSrcPduPtr->SduDataPtr[Conf_SrcPduHeaderLength] == 0xFE))
      {
        /* Accept a CONNECT request/positive response/negative response, before the Active Connection Group is established. */
        RequestAccepted = TRUE;
      }
      else
      {
        /* Nothing to do. */
      }

      if (RequestAccepted == TRUE)
      {
#endif
        /* !LINKSTO XcpR.dsn.ExclusiveArea_GlobalData, 1 */
        SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

        /* if the buffer is not full set it to full and lock the access */
        if ((XcpR_SrcPduData.IsBufferFullFlag[XCPR_BYTE_IDX(Conf_SrcInternalPduId)] & (uint8)(1U << XCPR_FLAG_IDX(Conf_SrcInternalPduId))) == 0U )
        {
          /* mark the buffer as full */
          XcpR_SrcPduData.IsBufferFullFlag[XCPR_BYTE_IDX(Conf_SrcInternalPduId)] |= (uint8)(1U << XCPR_FLAG_IDX(Conf_SrcInternalPduId));
          /* copy received data, of the incoming PDU, into assigned Src buffer */
          TS_MemCpy(XcpRSrcBufferPtr, XcpRRxSrcPduPtr->SduDataPtr, XcpRRxSrcPduPtr->SduLength);
          /* store length of the incoming PDU */
          XcpR_SrcPduData.PduLength[Conf_SrcInternalPduId] = XcpRRxSrcPduPtr->SduLength;
          /* store the mapped Src Pdu Id */
          XcpR_SrcPduData.PduIntId[Conf_SrcInternalPduId] = XcpRSrcPduId;
        }
        else
        {
          /* A message was lost because the corresponding Pdu buffer is full.
           * Therefore, a DET notification is issued to signal this situation. */
          ReportDetPduBufferFull = TRUE;
        }

        SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

#if (XCPR_MULTIPLE_ROUTING_PER_CONNECTION == STD_ON)
      }
      else
      {
        /* Discard the packet and DO NOT store it in queue: */
        /* reset the buffer flag */
        SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
        XcpR_SrcPduData.IsBufferFullFlag[XCPR_BYTE_IDX(Conf_SrcInternalPduId)] &= (uint8)~((uint8)(1U << XCPR_FLAG_IDX(Conf_SrcInternalPduId)));
        SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

        if (XcpR_Status.ConnectionStatus != XCPR_STATE_CONNECTED)
        {
          XCPR_DET_REPORT_RUNTIME_ERROR(XCPR_SID_IF_RX_INDICATION, XCPR_E_PDU_LOST);
        }
#if (XCPR_DEV_ERROR_DETECT == STD_ON)
        else
        {
          XCPR_DET_REPORT_ERROR(XCPR_SID_IF_RX_INDICATION, XCPR_E_PDU_OUTSIDE_CONNECTION_GROUP);
        }
#endif
      }
#endif /* (XCPR_MULTIPLE_ROUTING_PER_CONNECTION == STD_ON) */

      if (ReportDetPduBufferFull == TRUE)
      {
        XCPR_DET_REPORT_RUNTIME_ERROR(XCPR_SID_IF_RX_INDICATION, XCPR_E_PDU_BUFFER_FULL);
      }

      if (XcpR_Status.ConnectionStatus == XCPR_STATE_DISCONNECTED)
      {
        /* Local variable to hold the data packet PID (the first byte) */
        Pid = XcpRSrcBufferPtr[Conf_SrcPduHeaderLength];

        if (XcpR_SrcConf[Conf_SrcInternalPduId].IsMasterConnection == TRUE)
        {
          if (Pid != 0xFF) /* PID is not connect */
          {
            /* Discard the packet and DO NOT store it in queue: */
            /* reset the buffer flag */
            SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
            XcpR_SrcPduData.IsBufferFullFlag[XCPR_BYTE_IDX(Conf_SrcInternalPduId)] &= (uint8)~((uint8)(1U << XCPR_FLAG_IDX(Conf_SrcInternalPduId)));
            SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
            /* Trigger PDU lost det error */
            XCPR_DET_REPORT_RUNTIME_ERROR(XCPR_SID_IF_RX_INDICATION, XCPR_E_PDU_LOST);
          }
        }
      }

#if (XCPR_MULTIPLE_ROUTING_PER_CONNECTION == STD_ON)
      if (RequestAccepted == TRUE)
      {
#endif
#if (XCPR_RECEIVE_FROM_INDICATION == STD_ON)
        /* check if reception from Rx indication is enabled for this PDU */
        if (XcpR_SrcConf[Conf_SrcInternalPduId].ProcessingType == XCPR_RECEPTION_ON_RX_INDICATION)
        {
          XcpR_ProcessSource(XCPR_SID_IF_RX_INDICATION, Conf_SrcInternalPduId, SrcPduType);
        }
#else
        TS_PARAM_UNUSED(SrcPduType);
#endif /* (XCPR_RECEIVE_FROM_INDICATION == STD_ON) */

        RetVal = E_OK;
#if (XCPR_MULTIPLE_ROUTING_PER_CONNECTION == STD_ON)
      }
#endif
    }
  }

  return RetVal;
}

/*------------------[XcpR_ProcessSource]------------------------------------*/

FUNC(void, XCPR_CODE) XcpR_ProcessSource
(
  uint8 SidId,
  PduIdType Conf_SrcInternalPduId,
  uint8 SrcPduType
)
{
  Std_ReturnType GetLengthResult;
  /* XcpR data length and tail */
  PduLengthType DataLength;
  PduLengthType TailLength;
  /* get the usable Pdu length*/
  PduLengthType RemainingRxSpace;
  uint8 IsFramePending;
#if (XCPR_SRC_ON_FLEXRAY_FROM_LOWER_LAYER_ENABLED == STD_ON)
  uint8 ConnectMode;
#endif
#if ((XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON))
  /* Local variable to hold the data packet PID (the first byte) */
  uint8 Pid;
  boolean SoAdFramePending = FALSE;
#endif

  /* Pointer to XcpR Src runtime data */
  P2VAR(uint8, AUTOMATIC, XCPR_VAR_CLEARED) XcpRSrcBufferPtr;
  /* local variables to hold configured data */
  PduLengthType Conf_SrcPduHeaderLength;
  /* local variable to hold the message to be added in the Queue */
  XcpR_MsgType MessageToBeQueued;

  /* Pointer to the Src configuration array */
  P2CONST(XcpR_SrcPduType, AUTOMATIC, XCPR_APPL_CONST) Conf_SrcPduIdPtr = &XcpR_SrcConf[Conf_SrcInternalPduId];

  boolean ReportDetInternalBufferOverflow = FALSE;
#if (XCPR_DEV_ERROR_DETECT == STD_ON)
  boolean ReportDetInvalidMessageLength = FALSE;
#endif

#if (XCPR_TRANSMIT_FROM_RXINDICATION == STD_OFF)
  TS_PARAM_UNUSED(SidId);
#endif

  /* XcpR data length and tail */
  DataLength = 0U;
  TailLength = 0U;

  /* !LINKSTO XcpR.dsn.ExclusiveArea_GlobalData, 1 */
  SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

  /* get the usable Pdu length*/
  RemainingRxSpace = XcpR_SrcPduData.PduLength[Conf_SrcInternalPduId];

  /* get PDU start position inside the SrcPdu buffer */
  XcpRSrcBufferPtr = &(XcpR_SrcPduData.PduBuffer[XcpR_SrcConf[Conf_SrcInternalPduId].PduStartPos]);
  /* Get PDU header length from configuration */
  Conf_SrcPduHeaderLength = XcpR_SrcConf[Conf_SrcInternalPduId].PduAttributes.PduHeaderLength;

#if (XCPR_SRC_ON_FLEXRAY_FROM_LOWER_LAYER_ENABLED == STD_ON)
  ConnectMode = XcpRSrcBufferPtr[Conf_SrcPduHeaderLength+1U];
#endif

  /* Check if Frame is pending (buffer full) */
  IsFramePending = (XcpR_SrcPduData.IsBufferFullFlag[XCPR_BYTE_IDX(Conf_SrcInternalPduId)] & (uint8)(1U << XCPR_FLAG_IDX(Conf_SrcInternalPduId)));

  SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

  if (IsFramePending != 0U )
  {
#if ((XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON))
    SoAdFramePending = TRUE;
#endif

    /* unpack PDUs */
    do
    {
      SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

      /* Reset tail length */
      TailLength = 0U;

#if (XCPR_SRC_ON_FLEXRAY_FROM_LOWER_LAYER_ENABLED == STD_ON)
      XcpR_ProcessFlexRaySource(Conf_SrcInternalPduId, ConnectMode);
#endif

      /* get data segment length */
      /* !LINKSTO XcpR.Impl.RemoveBusRelatedInfo, 1 */
      GetLengthResult = XcpR_GetPduDataLength(RemainingRxSpace,
                                              XcpRSrcBufferPtr,
                                              &DataLength, &TailLength,
                                              Conf_SrcPduHeaderLength,
                                              Conf_SrcInternalPduId);

      SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

      if((DataLength == 0U) && ((XCPR_RECEPTION_MULTIPLE_PACKING & XcpR_SrcConf[Conf_SrcInternalPduId].Properties) != 0U))
      {
        break;
      }

      if ((GetLengthResult == E_OK) && (DataLength != 0U))
      {
        if (DataLength <= (XCPR_MAX_PACKAGE_SIZE_RECEPTION + 1U))
        {
          SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

          /* select the first address of the data segment into Rx package */
          XcpRSrcBufferPtr = &XcpRSrcBufferPtr[Conf_SrcPduHeaderLength];

#if ((XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON))
          Pid = XcpRSrcBufferPtr[0U];

          if (((Conf_SrcPduIdPtr->BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_UDPIP) || (Conf_SrcPduIdPtr->BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_TCPIP))
            && (Conf_SrcPduIdPtr->IsMasterConnection == TRUE))
          {
            if (Pid == 0xFF) /* Pid is CONNECT */
            {
              /* Set flag to change the SoAD */
              XcpR_ConfigureSocketConnect = TRUE;
            }
          }
#endif

          /* store the mapped Src Pdu Id */
          /* !LINKSTO XcpR.Impl.Store.Internal.Meta.Data, 1 */
          MessageToBeQueued.SrcPduId = XcpR_SrcPduData.PduIntId[Conf_SrcInternalPduId];
          /* store the mapped Src type */
          MessageToBeQueued.SrcPduType = SrcPduType;

          SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

          /* Check if Pdu is configured with trigger transmit and if the request comes from XcpR_Transmit() */
          if (((XCPR_SUPPORT_FOR_TRIGGER_TRANSMIT & Conf_SrcPduIdPtr->SrcToDestMapping[0]->Properties) != 0) && (SrcPduType == XCPR_TX_SRC_TYPE))
          {
            PduInfoType TriggerTransmitData;

            /* Copy data into local variable */
            TriggerTransmitData.SduDataPtr = &MessageToBeQueued.Msg[0U];
            TriggerTransmitData.SduLength = 0U;

            /* We use Index 0 because for a Tx source there is only one destination configurable - Tx */
            /* !LINKSTO XcpR.Impl.TriggerTransmit.UpperLayer, 1 */
            Conf_SrcPduIdPtr->SrcToDestMapping[0]->ULTriggerTransmitPtr(Conf_SrcPduIdPtr->SrcToDestMapping[0]->TxConfTriggerTransmitPduId, &TriggerTransmitData);

            /* Calculate message length into local variable */
            MessageToBeQueued.Length = TriggerTransmitData.SduLength + XCPR_INTERNAL_DATA_SIZE;
          }
          else
          {
            /* Calculate message length into local variable */
            /* !LINKSTO XcpR.Impl.StoreDataBytesOnly, 1 */
            MessageToBeQueued.Length = DataLength + XCPR_INTERNAL_DATA_SIZE;

            SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

            /* Copy data into local variable (only data bytes - without header) */
            TS_MemCpy(&MessageToBeQueued.Msg[0U], XcpRSrcBufferPtr, DataLength);

            SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
          }

          /* !LINKSTO XcpR.dsn.Queueing, 1 */
          if (E_NOT_OK == (XcpR_InsertMsgInQueue(&MessageToBeQueued)))
          {
            ReportDetInternalBufferOverflow = TRUE;
          }
          else if (SrcPduType == XCPR_TX_SRC_TYPE)
          {
            /* Data has been successfully written in the XcpR internal queue - UL_TxConfirmation can be triggered  */
            /* Pointer to the Src configuration array */
            Conf_SrcPduIdPtr = &XcpR_SrcConf[Conf_SrcInternalPduId];
            /* We use Index 0 because for a Tx source there is only one destination configurable - Tx */
            /* !LINKSTO XcpR.Impl.TxConfirmation.UpperLayer, 1 */
            Conf_SrcPduIdPtr->SrcToDestMapping[0]->ULTxConfirmationPtr(Conf_SrcPduIdPtr->SrcToDestMapping[0]->TxConfTriggerTransmitPduId);
          }
          else
          {
            /* Nothing to do. */
          }

          SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

          /* select the address of the next Rx PDU into package */
          XcpRSrcBufferPtr = &XcpRSrcBufferPtr[(DataLength + TailLength)];

          SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

          /* Safety measure to catch an overflow, should not happen */
          if ((Conf_SrcPduHeaderLength + DataLength + TailLength) > RemainingRxSpace)
          {
            RemainingRxSpace = 0U;
          }
          else
          {
            /* update remaining space */
            RemainingRxSpace -= (Conf_SrcPduHeaderLength + DataLength + TailLength);
          }
        }
        else /*  DataLength > (XCPR_MAX_PACKAGE_SIZE_RECEPTION + 1U) */
        {
          /* Discard the message. */
#if (XCPR_DEV_ERROR_DETECT == STD_ON)
          ReportDetInvalidMessageLength = TRUE;
#endif
         /* abort unpacking */
          RemainingRxSpace = 0U;
        }
      }
      else
      {
#if (XCPR_DEV_ERROR_DETECT == STD_ON)
        ReportDetInvalidMessageLength = TRUE;
#endif
        /* take action to abort unpacking */
        RemainingRxSpace = 0U;
      }
#if ((XCPR_RECEPTION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) && (XCPR_SRC_ON_FLEXRAY_FROM_LOWER_LAYER_ENABLED == STD_ON))
      if (Conf_SrcPduIdPtr->BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY)
      {
        /* Check if unpacking of multiple messages is enabled for this Source Pdu Id */
        if ((XCPR_RECEPTION_MULTIPLE_PACKING & XcpR_SrcConf[Conf_SrcInternalPduId].Properties) != 0U)
        {
          /*  for subsequent FlexRay concatenated messages the length is only one byte containing the LEN field */
          Conf_SrcPduHeaderLength =  1U;
        }
      }
#endif
    } while (RemainingRxSpace > Conf_SrcPduHeaderLength); /* until we reach the end of packed PDUs */

    SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

    /* Data has been unpacked - mark buffer as empty */
    XcpR_SrcPduData.IsBufferFullFlag[XCPR_BYTE_IDX(Conf_SrcInternalPduId)] &= (uint8)~((uint8)(1U << XCPR_FLAG_IDX(Conf_SrcInternalPduId)));

    SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
  }

#if ((XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON))
  XcpR_ProcessEthernetSource(Conf_SrcInternalPduId, SoAdFramePending);
#endif

  XcpR_TriggerDetErrorsProcessDest();

  if (ReportDetInternalBufferOverflow == TRUE)
  {
    XCPR_DET_REPORT_RUNTIME_ERROR(XCPR_SID_MAIN_FUNCTION, XCPR_E_INTERNAL_BUFFER_OVERFLOW);
  }

#if (XCPR_DEV_ERROR_DETECT == STD_ON)
  if (ReportDetInvalidMessageLength == TRUE)
  {
    XCPR_DET_REPORT_ERROR(XCPR_SID_MAIN_FUNCTION, XCPR_E_INVALID_MESSAGE_LENGTH);
  }
#endif

#if (XCPR_TRANSMIT_FROM_RXINDICATION == STD_ON)
  if (((XCPR_TRANSMISSION_FROM_RXINDICATION & XcpR_SrcConf[Conf_SrcInternalPduId].Properties) != 0U) && (SidId == XCPR_SID_IF_RX_INDICATION))
  {
    /* !LINKSTO XcpR.Impl.TransmitFromRxIndication, 1 */
    XcpR_ProcessDestinations();
  }
#endif

}

/*-----------------------------[XcpR_GetPduDataLength]----------------------*/

STATIC FUNC(Std_ReturnType, XCPR_CODE) XcpR_GetPduDataLength
(
  PduLengthType RemainingRxSpace,
  P2CONST(uint8, AUTOMATIC, AUTOSAR_COMSTACKDATA) SduDataPtr,
  P2VAR(PduLengthType, AUTOMATIC, XCPR_VAR) DataLengthPtr,
  P2VAR(PduLengthType, AUTOMATIC, XCPR_VAR) TailLengthPtr,
  PduLengthType HeaderLength,
  PduIdType Conf_SrcInternalPduId
)
{
  Std_ReturnType RetVal;
  /* local variable used to retrieve the package data length */
  PduLengthType DataLength = 0U;
#if (XCPR_SRC_ON_FLEXRAY_FROM_LOWER_LAYER_ENABLED == STD_ON)
  PduIdType DestInternalPduId;
#endif
  PduLengthType LocalRemainingRxSpace = RemainingRxSpace;

#if ((XCPR_SRC_ON_FLEXRAY_FROM_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_RECEPTION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))
  /* get the number of bytes of alignment */
  const PduLengthType NumberOfBytesAllignment = XcpR_SrcConf[Conf_SrcInternalPduId].BusAttributes.FlexRayPackageAlignment;
#else
  TS_PARAM_UNUSED(SduDataPtr);
  TS_PARAM_UNUSED(Conf_SrcInternalPduId);
#endif /* ((XCPR_SRC_ON_FLEXRAY_FROM_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_RECEPTION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)) */

#if (XCPR_RECEPTION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_OFF)
  TS_PARAM_UNUSED(TailLengthPtr);
#endif

#if (XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED == STD_ON)
  if ((XcpR_SrcConf[Conf_SrcInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_TCPIP) ||
      (XcpR_SrcConf[Conf_SrcInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_UDPIP))
      {
        TS_PARAM_UNUSED(TailLengthPtr);
      }
#endif

  if (LocalRemainingRxSpace <= HeaderLength)
  {
    /* the unpacking operation will be aborted */
    RetVal = E_NOT_OK;
  }
  else
  {
    /* update remaining space. strip-out the header */
    LocalRemainingRxSpace -= HeaderLength;

    /* initialize data length with default value */
    DataLength = LocalRemainingRxSpace;

    RetVal = E_OK;

#if (XCPR_SRC_ON_FLEXRAY_FROM_LOWER_LAYER_ENABLED == STD_ON)
    if (XcpR_SrcConf[Conf_SrcInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY)
    {
#if (XCPR_RECEPTION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
      /* Check if unpacking of multiple messages is enabled for this Source Pdu Id */
      /* !LINKSTO XcpR.dsn.ReceptionPackedFrame.StoreMessageInternally, 1 */
      if ((XCPR_RECEPTION_MULTIPLE_PACKING & XcpR_SrcConf[Conf_SrcInternalPduId].Properties) != 0U)
      {
        PduLengthType MessageUnalignedBytes;

        /* return calculated value for FlexRay bus */
        DataLength = (PduLengthType)(SduDataPtr[HeaderLength - 1U]);

        /* remove tail from the package, if it was added for alignment purposes */
        MessageUnalignedBytes = DataLength % NumberOfBytesAllignment;

        if (MessageUnalignedBytes > 0U)
        {
           *TailLengthPtr = NumberOfBytesAllignment - MessageUnalignedBytes;
        }
      }
      else
#endif /* (XCPR_RECEPTION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) */
      {

        if (XcpR_SrcConf[Conf_SrcInternalPduId].IsMasterConnection == TRUE)
        {
          DestInternalPduId = XcpR_Status.DestinationFromMasterInternalId;
        }
        else /* (XcpR_SrcConf[Conf_SrcInternalPduId].IsMasterConnection == FALSE) */
        {
          DestInternalPduId = XcpR_Status.DestinationFromSlaveInternalId;
        }

        /* Decision regarding tail removal. */
        /* check if message is odd */
        if ((DataLength % 2U) != 0U)
        {
          uint16 DestMaxPackageSize;
          DestMaxPackageSize = (XcpR_DestConf[DestInternalPduId].PduAttributes.MaxPackageSize + 1U);

          /* If the Length after removing the header is equal to (DestMaxPackageSize + 1), then 1 byte for the tail shall be removed */
          if (DataLength == DestMaxPackageSize)
          {
            DataLength -= 1U;
          }
          else if (DataLength > DestMaxPackageSize)
          {
            /* discard message */
            DataLength = 0U;
            RetVal = E_NOT_OK;
          }
          else /* DataLength < (XcpR_DestConf[DestInternalPduId].PduAttributes.MaxPackageSize + 1U) */
          {
            /* DataLength remains unchanged; Only the header is removed. */
          }
        }
      }
    }
#endif /* (XCPR_SRC_ON_FLEXRAY_FROM_LOWER_LAYER_ENABLED == STD_ON) */

#if (XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED == STD_ON)
    if ((XcpR_SrcConf[Conf_SrcInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_TCPIP) ||
        (XcpR_SrcConf[Conf_SrcInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_UDPIP))
    {
#if (XCPR_RECEPTION_OF_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
      /* Return calculated value of the request length in endianness order as received on Ethernet bus.*/
      DataLength = (PduLengthType)(((PduLengthType)SduDataPtr[1U]) << 8U);
      DataLength = DataLength | (PduLengthType)(SduDataPtr[0U]);
#endif
    }
#endif

    *DataLengthPtr = DataLength;
  }

  return RetVal;
}

#if ((XCPR_SRC_ON_ETHERNET_FROM_LOWER_LAYER_ENABLED == STD_ON) && (XCPR_DEST_ON_ETHERNET_LOWER_LAYER_ENABLED == STD_ON))
/*-----------------------------[XcpR_ProcessEthernetSource]-----------------*/
STATIC FUNC(void, XCPR_CODE) XcpR_ProcessEthernetSource
(
  PduIdType Conf_SrcInternalPduId,
  boolean SoAdFramePending
)
{
  /* Pointer to the Src configuration array */
  P2CONST(XcpR_SrcPduType, AUTOMATIC, XCPR_APPL_CONST) Conf_SrcPduIdPtr = &XcpR_SrcConf[Conf_SrcInternalPduId];

  SoAd_SoConIdType SourceSocketId = Conf_SrcPduIdPtr->SoAdRxConId;
  boolean IsIPv6 = Conf_SrcPduIdPtr->IsIPv6;
  boolean IsConnectCmd = FALSE;
  boolean TriggerDisconnect = FALSE;
  PduIdType LocalDestFromMasterInternalId = (PduIdType) ~0U;
  boolean ReportDetPduLost = FALSE;

  /* Variable to hold the remote address received on the Rx socket */
  TcpIp_SockAddrInet6Type RemoteAddressV6;
  /* Variable to hold the remote address received on the Rx socket */
  TcpIp_SockAddrInetType RemoteAddress;

  /* Pointer to the remote address variable, of type expected by SoAd functions */
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, XCP_CODE) SoAdRemoteAddressPtr;

  /* Fill in the required domain */
  if (IsIPv6 == TRUE)
  {
    /* Deviation MISRAC2012-2 <START> */
    SoAdRemoteAddressPtr = (P2VAR(TcpIp_SockAddrType, AUTOMATIC, XCP_CODE))&RemoteAddressV6;
    /* Deviation MISRAC2012-2 <STOP> */
    RemoteAddressV6.domain = (TcpIp_DomainType)TCPIP_AF_INET6;
  }
  else
  {
    /* Deviation MISRAC2012-2 <START> */
    SoAdRemoteAddressPtr = (P2VAR(TcpIp_SockAddrType, AUTOMATIC, XCP_CODE))&RemoteAddress;
    /* Deviation MISRAC2012-2 <STOP> */
    RemoteAddress.domain = (TcpIp_DomainType)TCPIP_AF_INET;
  }

  if (SoAdFramePending == TRUE)
  {
    if ((Conf_SrcPduIdPtr->BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_TCPIP) && (Conf_SrcPduIdPtr->IsMasterConnection == TRUE))
    {
      SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();
      IsConnectCmd = XcpR_ConfigureSocketConnect;
      SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

      /* Get the current address for the Rx socket */
      if (SoAd_GetRemoteAddr(SourceSocketId, SoAdRemoteAddressPtr) == E_OK)
      {
        SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

        if (IsConnectCmd == TRUE)
        {
          /* save address info only when connecting. */
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
        }

        /* Error handling for TCP (For UDP this is checked in XcpR_CopySoAdRemoteAddress() only if
         * SoAd_GetRemoteAddr() and SoAd_SetRemoteAddr() return without error) */
        if ((XcpR_Status.ConnectionStatus == XCPR_STATE_CONNECTED) && (Conf_SrcPduIdPtr->IsMasterConnection == TRUE))
        {
          if (IsIPv6 == TRUE)
          {
            if (XcpR_Status.EthernetActiveIPv6Address.port != RemoteAddressV6.port)
            {
              XcpR_RemoveLastMsgFromQueue();

              ReportDetPduLost = TRUE;
            }
            else
            {
              if (TS_MemCmp(XcpR_Status.EthernetActiveIPv6Address.addr,RemoteAddressV6.addr,XCPR_SOAD_IPV6ADDRSIZE) == E_NOT_OK)
              {
                /* if the received packet comes from a different IP than the one which originated the CONNECT command,
                 * the packet will be discarded  */
                XcpR_RemoveLastMsgFromQueue();

                ReportDetPduLost = TRUE;
              }
            }
          }
          else
          {
            if ((XcpR_Status.EthernetActiveIPv4Address.addr[0U] != RemoteAddress.addr[0U])
               || (XcpR_Status.EthernetActiveIPv4Address.port != RemoteAddress.port))
            {
              /* if the received packet comes from a different IP than the one which originated the CONNECT command,
               * the packet will be discarded  */
              XcpR_RemoveLastMsgFromQueue();

              ReportDetPduLost = TRUE;
            }
          }
        }

        SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

        if (ReportDetPduLost == TRUE)
        {
          XCPR_DET_REPORT_RUNTIME_ERROR(XCPR_SID_MAIN_FUNCTION, XCPR_E_PDU_LOST);
        }
      }
      else /* SoAd_GetRemoteAddr() returns E_NOT_OK */
      {
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

      /* clear flag */
      XcpR_ConfigureSocketConnect = FALSE;
    }

#if (XCPR_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON)
    if ((Conf_SrcPduIdPtr->BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_UDPIP) && (Conf_SrcPduIdPtr->IsMasterConnection == TRUE))
    {
      /* If the command is CONNECT, we must configure the socket for transmission with the IP and
       * port received on the reception socket */
      XcpR_CopySoAdRemoteAddress(Conf_SrcPduIdPtr->SoAdRxConId,
                                 XcpR_ConfigureSocketConnect,
                                 IsIPv6);

      /* clear flag */
      XcpR_ConfigureSocketConnect = FALSE;
    }
#endif
  }
}
#endif

#if (XCPR_SRC_ON_FLEXRAY_FROM_LOWER_LAYER_ENABLED == STD_ON)
STATIC FUNC(void, XCPR_CODE) XcpR_ProcessFlexRaySource
(
  PduIdType Conf_SrcInternalPduId,
  uint8 ConnectMode
)
{
  Std_ReturnType GetActiveDestResult = E_OK;

  if (XcpR_SrcConf[Conf_SrcInternalPduId].BusAttributes.ConnectionType == XCPR_CONNECTION_OVER_FLEXRAY)
  {
    if ((XCPR_RECEPTION_MULTIPLE_PACKING & XcpR_SrcConf[Conf_SrcInternalPduId].Properties) == 0U) /* reception of multiple packages disabled */
    {
      /* FR tail handling */
      if (XcpR_Status.ConnectionStatus == XCPR_STATE_DISCONNECTED)
      {
        if (XcpR_SrcConf[Conf_SrcInternalPduId].IsMasterConnection == TRUE)
        {
          /* All master sources represent XcpR_RxIndication() from the master Xcp
           * All XcpR messages reaching this point contain a Pid with the connect command (0xFF)
           * The protection check is done in XcpR_ReceiveRxData(). Message is not stored in the
           * internal XcpR queue, if XcpR is not connected and Pid is not 0xFF. */
          if (ConnectMode == XCPR_MODE_LOCAL_ECU)
          {
            GetActiveDestResult = XcpR_GetActiveDestination(Conf_SrcInternalPduId, XCPR_MODE_LOCAL_ECU, XCPR_SID_GET_PDU_DATA_LENGTH);
          }
          else /* if (ConnectMode == XCPR_MODE_REMOTE_ECU) */
          {
            GetActiveDestResult = XcpR_GetActiveDestination(Conf_SrcInternalPduId, XCPR_MODE_REMOTE_ECU, XCPR_SID_GET_PDU_DATA_LENGTH);
          }
        }
        else /* XcpR_SrcConf[Conf_SrcInternalPduId].IsMasterConnection == FALSE */
        {
          if (XcpR_Status.isPreviousCommandConnect == TRUE)
          {
            /* We use XCPR_MODE_REMOTE_ECU because the destination is always Tx for a source
             * which is not originating from master Xcp */
            GetActiveDestResult = XcpR_GetActiveDestination(Conf_SrcInternalPduId, XCPR_MODE_REMOTE_ECU, XCPR_SID_GET_PDU_DATA_LENGTH);
          }
          else
          {
            XcpR_ReportDetProcessDestCtr[XCPR_DET_REPORT_E_UNEXPECTED_MSG] += 1U;
          }
        }
      }
    }

    if(GetActiveDestResult == E_NOT_OK)
    {
      XcpR_ReportDetProcessDestCtr[XCPR_DET_REPORT_E_INVALID_ACTIVE_DESTINATION] += 1U;
    }
  }
}
#endif


/*-----------------------------[XcpR_FlushMsgQueue]-------------------------*/

FUNC(void, XCPR_CODE) XcpR_FlushMsgQueue(void)
{
  /* Reset queue head */
  XcpR_MsgQueue.Head      =  0U;
  /* Reset queue tail */
  XcpR_MsgQueue.Tail      =  0U;
  /* Reset number of bytes in queue */
  XcpR_MsgQueue.NoOfBytes =  0U;
  XcpR_MsgQueue.IsTailRolledOver = FALSE;
}

/*------------------[XcpR_DeQueueMsgQueue]----------------------------------*/

/* DeQueue one Msg from the Msg queue. */
FUNC(void, XCPR_CODE) XcpR_DeQueueMsgQueue
(
  PduLengthType DataLength,
  P2VAR(uint8, AUTOMATIC, XCPR_APPL_DATA) BufferPtr
)
{
  if ((XcpR_MsgQueue.Head + DataLength) < XCPR_MSG_QUEUE_SIZE)
  {
    TS_MemCpy(&(BufferPtr[0U]), &(XcpR_MsgQueue.Queue[XcpR_MsgQueue.Head]), DataLength);
  }
  else
  {
    XcpR_MsgNoOfBytesType LengthTillEndOfQueue = (XCPR_MSG_QUEUE_SIZE - XcpR_MsgQueue.Head);
    XcpR_MsgNoOfBytesType RemainingLength = ((XcpR_MsgNoOfBytesType)DataLength - LengthTillEndOfQueue);

    /* Read from available space until the end of the XcpR internal queue */
    TS_MemCpy(&(BufferPtr[0U]), &(XcpR_MsgQueue.Queue[XcpR_MsgQueue.Head]), LengthTillEndOfQueue);

    /* Read remaining data from the beginning of the XcpR internal queue */
    TS_MemCpy(&(BufferPtr[LengthTillEndOfQueue]), &(XcpR_MsgQueue.Queue[0]), RemainingLength);

    XcpR_MsgQueue.IsTailRolledOver = FALSE;
  }

  XcpR_MsgQueue.NoOfBytes -= (XcpR_MsgNoOfBytesType)DataLength;
  /* Update the head */
  /* as this operation can overflow XcpR_MsgNoOfBytesType, cast to the larger XcpR_MsgNoOfBytesNextType type */
  XcpR_MsgQueue.Head = ((XcpR_MsgNoOfBytesNextType)XcpR_MsgQueue.Head + DataLength) % XCPR_MSG_QUEUE_SIZE;
}

/*------------------[XcpR_GetMsgLength]-------------------------------------*/

FUNC(PduLengthType, XCPR_CODE) XcpR_GetMsgLength(void)
{
  PduLengthType Iterator, CurrentByte, ShiftedByteValue;
  PduLengthType PduLength = 0U;

  for (Iterator = 0U; Iterator < sizeof(PduLengthType); Iterator++)
  {
    CurrentByte = (XcpR_MsgQueue.Queue[XcpR_MsgQueue.Head]);
    ShiftedByteValue = (CurrentByte << (Iterator * 8U));

    PduLength = PduLength | ShiftedByteValue;
    XcpR_UpdateMsgQueueHead();
  }

  return PduLength;
}

/*------------------[XcpR_GetMsgSrcPduId]-----------------------------------*/

FUNC(PduIdType, XCPR_CODE) XcpR_GetMsgSrcPduId(void)
{
  PduIdType Iterator, CurrentByte, ShiftedByteValue;
  PduIdType PduId = 0U;

  for (Iterator = 0U; Iterator < sizeof(PduIdType); Iterator++)
  {
    CurrentByte = (XcpR_MsgQueue.Queue[XcpR_MsgQueue.Head]);
    ShiftedByteValue = (CurrentByte << (Iterator * 8U));

    PduId = PduId | ShiftedByteValue;
    XcpR_UpdateMsgQueueHead();
  }

  return PduId;
}

/*------------------[XcpR_GetMsgSrcPduType]-----------------------------------*/

FUNC(uint8, XCPR_CODE) XcpR_GetMsgSrcPduType(void)
{
  uint8 PduType;

  PduType = XcpR_MsgQueue.Queue[XcpR_MsgQueue.Head];
  XcpR_UpdateMsgQueueHead();

  return PduType;
}

/*------------------[XcpR_ReadMsgLength]-------------------------------------*/

FUNC(PduLengthType, XCPR_CODE) XcpR_ReadMsgLength(void)
{
  PduLengthType Iterator, CurrentByte, ShiftedByteValue;
  XcpR_MsgNoOfBytesType localHead;
  PduLengthType PduLength = 0U;

  localHead = XcpR_MsgQueue.Head;

  for (Iterator = 0U; Iterator < sizeof(PduLengthType); Iterator++)
  {
    CurrentByte = (XcpR_MsgQueue.Queue[localHead]);
    ShiftedByteValue = (CurrentByte << (Iterator * 8U));
    PduLength = PduLength | ShiftedByteValue;

    ++localHead;
    localHead  %= XCPR_MSG_QUEUE_SIZE;
  }

  return PduLength;
}

/*------------------[XcpR_ReadMsgSrcPduId]-----------------------------------*/

FUNC(PduIdType, XCPR_CODE) XcpR_ReadMsgSrcPduId(void)
{
  PduIdType Iterator, CurrentByte, ShiftedByteValue;
  XcpR_MsgNoOfBytesType localHead;
  PduIdType PduId = 0U;

  localHead = ((XcpR_MsgNoOfBytesNextType)XcpR_MsgQueue.Head + sizeof(PduLengthType)) % XCPR_MSG_QUEUE_SIZE;

  for (Iterator = 0U; Iterator < sizeof(PduIdType); Iterator++)
  {
    CurrentByte = (XcpR_MsgQueue.Queue[localHead ]);
    ShiftedByteValue = (CurrentByte << (Iterator * 8U));
    PduId = PduId | ShiftedByteValue;

    ++localHead;
    localHead  %= XCPR_MSG_QUEUE_SIZE;
  }

  return PduId;
}

/*------------------[XcpR_ReadMsgSrcPduType]-----------------------------------*/

FUNC(uint8, XCPR_CODE) XcpR_ReadMsgSrcPduType(void)
{
  XcpR_MsgNoOfBytesType localHead = ((XcpR_MsgNoOfBytesNextType)XcpR_MsgQueue.Head + sizeof(PduLengthType) + sizeof(PduIdType)) % XCPR_MSG_QUEUE_SIZE;
  return XcpR_MsgQueue.Queue[localHead];
}

/*------------------[XcpR_ReadMsgFirstByte]-----------------------------------*/

FUNC(uint8, XCPR_CODE) XcpR_ReadMsgFirstByte(void)
{
  XcpR_MsgNoOfBytesType localHead = ((XcpR_MsgNoOfBytesNextType)XcpR_MsgQueue.Head + XCPR_INTERNAL_DATA_SIZE) % XCPR_MSG_QUEUE_SIZE;
  return XcpR_MsgQueue.Queue[localHead];
}

/*------------------[XcpR_ReadMsgSecondByte]-----------------------------------*/

FUNC(uint8, XCPR_CODE) XcpR_ReadMsgSecondByte(void)
{
  XcpR_MsgNoOfBytesType localHead;
  localHead = ((XcpR_MsgNoOfBytesNextType)XcpR_MsgQueue.Head + XCPR_INTERNAL_DATA_SIZE + 1U) % XCPR_MSG_QUEUE_SIZE;
  return XcpR_MsgQueue.Queue[localHead];
}

/*------------------[XcpR_IsMsgQueueEmpty]-----------------------------------*/

FUNC(boolean, XCPR_CODE) XcpR_IsMsgQueueEmpty(void)
{
  boolean RetVal;

  /* !LINKSTO XcpR.dsn.ExclusiveArea_GlobalData, 1 */
  SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

  RetVal = ((0U == XcpR_MsgQueue.NoOfBytes)? TRUE:FALSE);

  SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

  return RetVal;
}

/*==[Definition of functions with internal linkage]===========================*/

/*------------------[XcpR_InsertMsgInQueue]---------------------------------*/

FUNC(Std_ReturnType, XCPR_CODE) XcpR_InsertMsgInQueue
(
  P2VAR(XcpR_MsgType, AUTOMATIC, XCPR_APPL_DATA) MsgPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  XcpR_MsgNoOfBytesType CurrentQueueSize, AvailableQueueLength;

  PduLengthType DataLength;
  PduLengthType RemainingDataLength;

  SchM_Enter_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

  DataLength = (MsgPtr->Length - XCPR_INTERNAL_DATA_SIZE);
  CurrentQueueSize = (XCPR_MSG_QUEUE_SIZE - XcpR_MsgQueue.NoOfBytes);

  /* Do not write data if there is not sufficient space for the entire message, including internal data
   * If the message does not fit in the remaining available Queue length, do not store it,
   * as it results in a buffer overflow. */
  if (CurrentQueueSize < (MsgPtr->Length))
  {
    RetVal = E_NOT_OK;
  }
  else
  {
    /* Enqueue MSG length */
    XCPR_ENQUEUE_VALUE(MsgPtr->Length, PduLengthType);

    /* Enqueue MSG SrcPduId */
    XCPR_ENQUEUE_VALUE(MsgPtr->SrcPduId, PduIdType);

    /* Enqueue MSG SrcPdu type */
    XcpR_EnqueueByteInMsgQueue(MsgPtr->SrcPduType);

    if (XcpR_MsgQueue.IsTailRolledOver == FALSE)
    {
      /* Update available length until the end of the queue, after adding PduLength, PduId and SrcPdu type */
       AvailableQueueLength = (XCPR_MSG_QUEUE_SIZE - XcpR_MsgQueue.Tail);

      /* Enqueue the entire MSG bytes into MSG Queue, since there is available space */
      if (DataLength <= AvailableQueueLength)
      {
        TS_MemCpy(&(XcpR_MsgQueue.Queue[XcpR_MsgQueue.Tail]), &MsgPtr->Msg[0U], DataLength);

        /* Update the tail */
        XcpR_MsgQueue.Tail += (XcpR_MsgNoOfBytesType)DataLength;
        XcpR_MsgQueue.Tail %= XCPR_MSG_QUEUE_SIZE;

        /* Set a flag in case Tail has been rolled over */
        if (XcpR_MsgQueue.Tail == 0U)
        {
          XcpR_MsgQueue.IsTailRolledOver = TRUE;
        }

        /* Update number of bytes in Msg Queue */
        XcpR_MsgQueue.NoOfBytes += (XcpR_MsgNoOfBytesType)DataLength;
      }
      else
      {
        /* Copy into available space until the end of the XcpR internal queue */
        TS_MemCpy(&(XcpR_MsgQueue.Queue[XcpR_MsgQueue.Tail]), &MsgPtr->Msg[0U], AvailableQueueLength);

        /* Update the tail */
        XcpR_MsgQueue.Tail += AvailableQueueLength;
        XcpR_MsgQueue.Tail %= XCPR_MSG_QUEUE_SIZE;
        /* Update number of bytes in Msg Queue */
        XcpR_MsgQueue.NoOfBytes += AvailableQueueLength;

        /* Set a flag in case Tail has been rolled over */
        /* CHECK: NOPARSE */
        if (XcpR_MsgQueue.Tail == 0U)
        /* CHECK: PARSE */
        {
          XcpR_MsgQueue.IsTailRolledOver = TRUE;
        }
        /* CHECK: NOPARSE */
        else
        {
         /* Unreachable code.
          * If AvailableQueueLength data is being written, Tail should always be 0.
          * However, if-clause is kept as an additional safety check. */
          XCPR_UNREACHABLE_CODE_ASSERT(XCPR_SID_MAIN_FUNCTION);
        }
        /* CHECK: PARSE */

        /* Calculate remaining data to be written, after a part has already been copied until the end of the queue */
        RemainingDataLength = (DataLength - AvailableQueueLength);

        /* Copy remaining data from the beginning of the XcpR internal queue */
        TS_MemCpy(&(XcpR_MsgQueue.Queue[XcpR_MsgQueue.Tail]), &MsgPtr->Msg[AvailableQueueLength], RemainingDataLength);

        /* Update the tail */
        XcpR_MsgQueue.Tail += (XcpR_MsgNoOfBytesType)RemainingDataLength;

        XcpR_MsgQueue.Tail %= XCPR_MSG_QUEUE_SIZE;
        /* Update number of bytes in Msg Queue */
        XcpR_MsgQueue.NoOfBytes += (XcpR_MsgNoOfBytesType)RemainingDataLength;
      }
    }
    else /* (XcpR_MsgQueue.IsTailRolledOver == TRUE) */
    {
      AvailableQueueLength = (XcpR_MsgQueue.Head - XcpR_MsgQueue.Tail);

      /* Enqueue the entire MSG bytes into MSG Queue, since there is available space */
      /* CHECK: NOPARSE */
      if (DataLength <= AvailableQueueLength)
      /* CHECK: PARSE */
      {
        TS_MemCpy(&(XcpR_MsgQueue.Queue[XcpR_MsgQueue.Tail]), &MsgPtr->Msg[0U], DataLength);

        /* Update the tail */
        XcpR_MsgQueue.Tail += (XcpR_MsgNoOfBytesType)DataLength;
        XcpR_MsgQueue.Tail %= XCPR_MSG_QUEUE_SIZE;
        /* Update number of bytes in Msg Queue */
        XcpR_MsgQueue.NoOfBytes += (XcpR_MsgNoOfBytesType)DataLength;
      }
      /* CHECK: NOPARSE */
      else
      {
       /* Unreachable code.
        The check is already done if there is sufficient space for all data, at the beginning of the function */
        XCPR_UNREACHABLE_CODE_ASSERT(XCPR_SID_MAIN_FUNCTION);
      }
      /* CHECK: PARSE */
    }

    RetVal = E_OK;
  }

  SchM_Exit_XcpR_SCHM_XCPR_EXCLUSIVE_AREA_0();

  return RetVal;
}

/*------------------[XcpR_EnqueueByteInMsgQueue]----------------------------*/

FUNC(void, XCPR_CODE) XcpR_EnqueueByteInMsgQueue
(
  uint8 DataByte
)
{
  /* Enqueue the byte */
  XcpR_MsgQueue.Queue[XcpR_MsgQueue.Tail] = DataByte;

  /* Update the tail */
  XcpR_MsgQueue.Tail++;

  /* Check if Tail needs to be rolled over */
  XcpR_MsgQueue.Tail %= XCPR_MSG_QUEUE_SIZE;
  /* Update number of bytes in Msg Queue */
  XcpR_MsgQueue.NoOfBytes++;

  /* Set a flag in case Tail has been rolled over */
  if (XcpR_MsgQueue.Tail == 0U)
  {
    XcpR_MsgQueue.IsTailRolledOver = TRUE;
  }

}

/*------------------[XcpR_UpdateMsgQueueHead]----------------------------*/

FUNC(void, XCPR_CODE) XcpR_UpdateMsgQueueHead
(
  void
)
{
  /* Update total number of bytes in Msg queue */
  XcpR_MsgQueue.NoOfBytes--;
  /* Check whether queue head needs to be rolled over */
  ++XcpR_MsgQueue.Head;
  if (XcpR_MsgQueue.Head == XCPR_MSG_QUEUE_SIZE)
  {
    XcpR_MsgQueue.Head = 0U;
    XcpR_MsgQueue.IsTailRolledOver = FALSE;
  }
}

#define XCPR_STOP_SEC_CODE
#include <XcpR_MemMap.h>

