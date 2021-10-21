/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* !LINKSTO Dcm.Dsn.File.LowerLayerInterfaces.RxConnection.Impl,1 */
/* This file contains the implementation of the RxConnection software unit. */

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 16.7 (advisory)
 *     "A pointer parameter in a function prototype should be declared as pointer to const if
 *      the pointer is not used to modify the addressed object."
 *
 *     Reason:
 *     AUTOSAR SWS 4.0.3 specifies the parameter as non-const. Since the function defined here
 *     is a dummy one, the pointer parameter is unused.
 *
 */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 8.13 (advisory)
  *     "A pointer should point to a const-qualifed type whenever possible."
  *
  *     Reason:
  *     AUTOSAR SWS 4.0.3 specifies the parameter as non-const. Since the function defined here
  *     is a dummy one, the pointer parameter is unused.
  *
  */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
  * Reason:
  * This warnings is a false positive.
  * The compiler optimizes the 'for' loop (which has only 1 iteration) in the following way
  * - it prepares in advance the index for the next iteration
  * - if the index is equal to '0' it goes back to check the 'for' loop conditions, otherwise it just moves on
  * - since the index is already equal to '1', it never jumps back to check the exit condition
  */

/*==================[inclusions]=================================================================*/

#include <ComStack_Types.h>                               /* AUTOSAR Communication Stack types */
#include <TSAutosar.h>
#include <TSMem.h>

#include <Dcm_Dsl_CommunicationServices_RxConnections.h>  /* Dcm Dsl RxConnections header file */
#include <Dcm_Dsl_ComMChannelStateManager.h>              /* Dcm Dsl CComMChannelStateManager header
                                                             file */
#include <Dcm_Dsl_Supervisor.h>                           /* Dcm Dsl Supervisor header file */

#include <Dcm.h>                                          /* Definition of DCM_MODULE_ID */
#include <Dcm_Int.h>                                      /* Module intenal definitions */
#include <Dcm_HsmDcm.h>                                   /* public statechart model definitions */
#include <TSMem.h>                                        /* EB specific memory functions */
#include <SchM_Dcm.h>                                     /* SchM interface for Dcm */
#include <PduR_Dcm.h>                                     /* PduR interface for Dcm */
#include <ComM_Dcm.h>                                     /* ComM interface for Dcm */
#include <Dcm_Trace.h>
#include <Dcm_Cbk.h>

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.RxConnection,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.ParallelRxConnections,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.RxConnection.RuntimeDataElements,1 */
/** \brief Contains the state variables of all the RxConnection entities.
 */
VAR(Dcm_RxConnectionType, DCM_VAR) Dcm_RxConnections[DCM_NUM_RX_CONNECTIONS];

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_StartOfReception.API,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.RxConnection.StartOfReception,1 */
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_StartOfReception(
  PduIdType DcmRxPduId,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) RxBufferSizePtr
)
{
  BufReq_ReturnType BufferAllocationResult = BUFREQ_E_NOT_OK;

  DBG_DCM_STARTOFRECEPTION_ENTRY(DcmRxPduId,TpSduLength,RxBufferSizePtr);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_StartOfReception.ModuleNotInitialized.ReportDETError,1 */
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_StartOfReception.DETCheckFails.DiscardReservedBuffer,1 */
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_StartOfReception.DETCheckFails.ReturnBUFREQ_E_NOT_OK,1 */
  if (TRUE != Dcm_Initialized)
  {
    DCM_DET_REPORT_ERROR(DCM_SVCID_STARTOFRECEPTION, DCM_E_UNINIT);
  }
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_StartOfReception.InvalidDcmRxPduId.ReportDETError,1 */
  else if (DCM_NUM_RX_PDU_ID <= DcmRxPduId)
  {
    DCM_DET_REPORT_ERROR(DCM_SVCID_STARTOFRECEPTION, DCM_E_PARAM);
  }
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_StartOfReception.InvalidRxBufferSizePtr.ReportDETError,1 */
  else if (NULL_PTR == RxBufferSizePtr)
  {
    DCM_DET_REPORT_ERROR(DCM_SVCID_STARTOFRECEPTION, DCM_E_PARAM_POINTER);
  }
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_StartOfReception.RxPduIdInUse.ReportDETError,1 */
  else if (DCM_RX_CONNECTION_IDLE != Dcm_RxConnections[DcmRxPduId].Status)
  {
    /* !LINKSTO Dcm.EB.RequestHandling.StartOfReception.RxConnectionBusy.RejectRx,1 */
    /* A processing is already in progress on the same RxPduId */

    DCM_DET_REPORT_ERROR(DCM_SVCID_STARTOFRECEPTION, DCM_E_PARAM);
  }
  else
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */
  {
    Dcm_BufferType RxBuffer;

    if (0U != TpSduLength)
    {
      Dcm_RequestTypeType AcceptAs;

      /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_StartOfReception.InterruptContext,1 */
      SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

      AcceptAs = Dcm_Dsl_Supervisor_RequestRxPermission(DcmRxPduId, TpSduLength, DCM_EXTERNAL);

      if (DCM_REJECTED_REQUEST != AcceptAs)
      {
        BufferAllocationResult = Dcm_Dsl_AllocateRxBuffer(DcmRxPduId, TpSduLength,
                                                          &AcceptAs, &RxBuffer);

        if (BUFREQ_OK == BufferAllocationResult)
        {
          Dcm_Dsl_SetupRxConnection(DcmRxPduId,
                                    AcceptAs,
                                    RxBuffer,
                                    TpSduLength);

          /* Announce the Supervisor that a reception has been started so
           * it may take the necessary actions. */
          Dcm_Dsl_Supervisor_IncomingReception(DcmRxPduId,
                                                  Dcm_RxConnections[DcmRxPduId].RequestType);

          *RxBufferSizePtr = (PduLengthType)(RxBuffer.Size);
        }
      }
      else
      {
        BufferAllocationResult = BUFREQ_E_NOT_OK;
      }

      SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
    }
    else
    {
      /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_StartOfReception.RequestLengthIsZero.AcceptRequest,1 */
      BufferAllocationResult = BUFREQ_OK;

      /* Without allocating a buffer, simply return the size of the
         Normal Buffer assigned to this RxConnection*/
      Dcm_Dsl_BufferGetInfo(DcmRxPduId, DCM_BUFFER_RX_NORMAL, &RxBuffer);

      /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_StartOfReception.RequestLengthIsZero.ReturnBufferSize,1 */
      *RxBufferSizePtr = (PduLengthType)(RxBuffer.Size);
    }
  }

  DBG_DCM_STARTOFRECEPTION_EXIT(BufferAllocationResult,DcmRxPduId,TpSduLength,RxBufferSizePtr);

  return BufferAllocationResult;
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Dcm_CopyRxData ,1 */
/* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_CopyRxData.API,1 */
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_CopyRxData(
  PduIdType DcmRxPduId,
  P2CONST(PduInfoType, AUTOMATIC, DCM_APPL_DATA) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) RxBufferSizePtr)
{
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyRxData.DETCheckFails.ReturnBUFREQ_E_NOT_OK,1 */
  BufReq_ReturnType DataCopyResult = BUFREQ_E_NOT_OK;

  DBG_DCM_COPYRXDATA_ENTRY(DcmRxPduId,PduInfoPtr,RxBufferSizePtr);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyRxData.ModuleNotInitialized.ReportDETError,1 */
  if (TRUE != Dcm_Initialized)
  {
    DCM_DET_REPORT_ERROR(DCM_SVCID_COPYRXDATA, DCM_E_UNINIT);
  }
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyRxData.InvalidDcmRxPduId.ReportDETError,1 */
  else if (DCM_NUM_RX_PDU_ID <= DcmRxPduId)
  {
    DCM_DET_REPORT_ERROR(DCM_SVCID_COPYRXDATA, DCM_E_PARAM);
  }
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyRxData.InvalidPduInfoPtrInvalidRxBufferSizePtr.ReportDETError,1 */
  else if ((NULL_PTR == PduInfoPtr) || (NULL_PTR == RxBufferSizePtr))
  {
    DCM_DET_REPORT_ERROR(DCM_SVCID_COPYRXDATA, DCM_E_PARAM_POINTER);
  }
  else
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */
  {
    /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_CopyRxData.InterruptContext,1 */
    /* ENTER critical section */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    /* if receive session ongoing for this RxPduId */
    if ((DCM_RX_CONNECTION_RECEIVING == Dcm_RxConnections[DcmRxPduId].Status) &&
        ((DCM_NORMAL_PHYSICAL_REQUEST == Dcm_RxConnections[DcmRxPduId].RequestType) ||
         (DCM_NORMAL_FUNCTIONAL_REQUEST == Dcm_RxConnections[DcmRxPduId].RequestType) ||
         (DCM_BUSYREPEATREQUEST_REQUEST == Dcm_RxConnections[DcmRxPduId].RequestType) ||
         (DCM_CONCURRENT_TESTERPRESENT_REQUEST == Dcm_RxConnections[DcmRxPduId].RequestType)
        )
       )
    {
      /* Check if service request is  invoked with SduLength of PduInfoPtr equal to 0 */
      if ((PduInfoPtr->SduLength == 0U) || (PduInfoPtr->SduDataPtr == NULL_PTR))
      {
        /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_CopyRxData.RequestLengthIsZero.ReturnBUFREQ_OK,1 */
        DataCopyResult = BUFREQ_OK;
        /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_CopyRxData.RequestLengthIsZero.ReturnBufferSize,1 */
        *RxBufferSizePtr = (PduLengthType)(
          (Dcm_RxConnections[DcmRxPduId].Buffer.Size) -
          (Dcm_RxConnections[DcmRxPduId].Buffer.FilledLength));
      }
      else
      {
        /* Checking whether the current requested data length is within the range of total
         * diagnostic request length stored during Dcm_StartOfReception() API . If not, return
         * BUFREQ_E_NOT_OK and wait for the Dcm_RxIndication() call to signal the failure
         * of reception.
         */

        if (((Dcm_RxConnections[DcmRxPduId].Buffer.FilledLength) + (PduInfoPtr->SduLength)) <=
            (Dcm_RxConnections[DcmRxPduId].ExpectedRxLength))
        {
          PduLengthType AmountToCopy;
#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_OFF)
          /* First check whether we are receiving data into a default buffer and adjust the
             amount of data to copy accordingly. */
          if ((DCM_BUSYREPEATREQUEST_REQUEST == Dcm_RxConnections[DcmRxPduId].RequestType) ||
              (DCM_CONCURRENT_TESTERPRESENT_REQUEST == Dcm_RxConnections[DcmRxPduId].RequestType)
             )
          {
            /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.DefaultBufferReception,1 */
            if (DCM_BUFFER_DEFAULT_BUFFER_SIZE > Dcm_RxConnections[DcmRxPduId].Buffer.FilledLength)
            {
              /* Only if there is still free space inside the Default Buffer do we need to
                 adjust how much data we can copy*/
              if ((DCM_BUFFER_DEFAULT_BUFFER_SIZE - Dcm_RxConnections[
                  DcmRxPduId].Buffer.FilledLength) < PduInfoPtr->SduLength)
              {
                /* The amount to copy will be the minimum between the PduInfoPtr->SduLength and
                   the amount of space remaining in the Default Buffer. */
                AmountToCopy = (PduLengthType)(
                  DCM_BUFFER_DEFAULT_BUFFER_SIZE -
                  Dcm_RxConnections[DcmRxPduId].Buffer.FilledLength);
              }
              else
              {
                AmountToCopy = PduInfoPtr->SduLength;
              }
            }
            else
            {
              /* Since all the space in the Default Buffer has been filled, actually copy 0 bytes
                 but pretend to have copied the rest of the data. */
              AmountToCopy = 0U;
            }
          }
          else
#endif /* #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_OFF) */
          {
            /* This is a normal buffer, therefore the amount of data
               to copy is PduInfoPtr->SduLength*/
            AmountToCopy = PduInfoPtr->SduLength;
          }

          /* Only copy data if either this is a reception in a Normal Buffer or a reception
             in a Default Buffer which isn't filled up yet. */
          if (!(((DCM_BUSYREPEATREQUEST_REQUEST == Dcm_RxConnections[DcmRxPduId].RequestType) ||
                 (DCM_CONCURRENT_TESTERPRESENT_REQUEST == Dcm_RxConnections[DcmRxPduId].RequestType)
                ) && (0U == AmountToCopy)
               )
             )
          {
            /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_CopyRxData.Dcm_StartOfReceptionReturnsBUFREQ_OK.CopyDataToDcmDslBuffer,1 */
            /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyRxData.SduDataPtrIsNull.CopyNoData,1 */
            TS_MemCpy(&(Dcm_RxConnections[DcmRxPduId].Buffer.BufferPtr[
                Dcm_RxConnections[DcmRxPduId].Buffer.FilledLength]),
                      PduInfoPtr->SduDataPtr, AmountToCopy);
          }

          /* Update dataMark */
          Dcm_RxConnections[DcmRxPduId].Buffer.FilledLength += PduInfoPtr->SduLength;

          /* Return remaining free place in receive buffer after completion of this call */
          /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_CopyRxData.Dcm_StartOfReceptionReturnsBUFREQ_OK.UpdateBufferSizePtr,1 */
          /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyRxData.SduDataPtrIsNull.ReturnBUFREQ_OK,1 */
          *RxBufferSizePtr = (PduLengthType)
            (Dcm_RxConnections[DcmRxPduId].Buffer.Size -
             Dcm_RxConnections[DcmRxPduId].Buffer.FilledLength);

          DataCopyResult = BUFREQ_OK;
        }
        else
        {
          /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyRxData.ExceededSignaledData.ReturnBUFREQ_E_NOT_OK,1 */
          /* As Dcm does not support paged buffer reception, this condition will never happen.
           * Instead, an error BUFFREQ_E_NOT_OK might be received since an illegal condition will
           * have been reached (the PduR tries to give the Dcm more data than it notified
           * in Dcm_StartOfReception).
           */

          DataCopyResult = BUFREQ_E_NOT_OK;
        }
      }
    }
    else
    {
      /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyRxData.NoOngoingReception.ReturnBUFREQ_E_NOT_OK,1 */
      /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyRxData.CancelledRxConnection,1 */
      /* No reception initiated on this RxPduId or the RxConnection was
         previously cancelled */
      *RxBufferSizePtr = 0U;
      /* Restart of S3 timer is not required in CopyRxData, as for success or failure in
       * reception of data, PduR has to invoke TpRxIndication
       */
    }

    /* Any error cancels the current RxConnection. */
    if (BUFREQ_OK != DataCopyResult)
    {
      /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyRxData.CancelRxConnection,1 */
      Dcm_RxConnections[DcmRxPduId].Status = DCM_RX_CONNECTION_CANCELLED;
    }

    /* LEAVE critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
  }

  DBG_DCM_COPYRXDATA_EXIT(DataCopyResult,DcmRxPduId,PduInfoPtr,RxBufferSizePtr);

  return DataCopyResult;
}

/* !LINKSTO Dcm.EB.Dcm_RxIndication,1 */
/* This function is only a stub to provide the function symbol */
/* Deviation MISRAC2012-1 <+3>*/
/* Deviation MISRA-1 <+3> */
FUNC(void, DCM_CODE) Dcm_RxIndication(
  PduIdType DcmRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) PduInfoPtr
)
{
  DBG_DCM_RXINDICATION_ENTRY(DcmRxPduId, PduInfoPtr);
  TS_PARAM_UNUSED(DcmRxPduId);
  TS_PARAM_UNUSED(PduInfoPtr);
  DBG_DCM_RXINDICATION_EXIT(DcmRxPduId, PduInfoPtr);
}

/* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_TpRxIndication.API,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.RxConnection.TpRxIndication,1 */
FUNC(void, DCM_CODE) Dcm_TpRxIndication(
  PduIdType DcmRxPduId,
  NotifResultType Result
)
{
  DBG_DCM_TPRXINDICATION_ENTRY(DcmRxPduId,Result);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_TpRxIndication.ModuleNotInitialized.ReportDETError,1 */
  if (TRUE != Dcm_Initialized)
  {
    DCM_DET_REPORT_ERROR(DCM_SVCID_TPRXINDICATION, DCM_E_UNINIT);
  }
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_TpRxIndication.InvalidId,1 */
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_TpRxIndication.InvalidDcmRxPduIdInvalidResult.ReportDETError,1 */
  else if ((DCM_NUM_RX_PDU_ID <= DcmRxPduId) || (DCM_INVALID_NTFRSLT_RANGE_START <= Result))
  {
    DCM_DET_REPORT_ERROR(DCM_SVCID_TPRXINDICATION, DCM_E_PARAM);
  }
  else
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */
  {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    if((Result == NTFRSLT_OK) && ((Dcm_RxConnections[DcmRxPduId].ExpectedRxLength) !=
      (Dcm_RxConnections[DcmRxPduId].Buffer.FilledLength)))
    {
      /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_TpRxIndication.InterruptContext,1 */
      /* ENTER critical section */
      SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

      /* Release the used reception buffer */
      (void)Dcm_Dsl_BufferInitializeSingleBuffer(DcmRxPduId,
                                                 Dcm_RxConnections[DcmRxPduId].Buffer.BufferId);

      /* Notify the Supervisor that the reception has ended. */
      Dcm_Dsl_Supervisor_IncomingReceptionFinished(DcmRxPduId);

      /* This RxConnection can be freed now. This adds robustness in this error case so that the
         RxConnection may still be used. */
      Dcm_Dsl_RxConnectionInitSingleRxConnection(DcmRxPduId);

      /* LEAVE critical section */
      SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

      /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_TpRxIndication.InvalidRequestLength.ReportDETError,1 */
      DCM_DET_REPORT_ERROR(DCM_SVCID_TPRXINDICATION, DCM_E_INVALID_REQLEN);
    }
    else if((DCM_NORMAL_PHYSICAL_REQUEST != Dcm_RxConnections[DcmRxPduId].RequestType) &&
            (DCM_NORMAL_FUNCTIONAL_REQUEST != Dcm_RxConnections[DcmRxPduId].RequestType) &&
            (DCM_BUSYREPEATREQUEST_REQUEST != Dcm_RxConnections[DcmRxPduId].RequestType) &&
            (DCM_CONCURRENT_TESTERPRESENT_REQUEST != Dcm_RxConnections[DcmRxPduId].RequestType)
           )
    {
      DCM_DET_REPORT_ERROR(DCM_SVCID_TPRXINDICATION, DCM_E_PARAM);
    }
    else
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */
    {
      /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_TpRxIndication.InterruptContext,1 */
      /* ENTER critical section */
      SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

      if ((NTFRSLT_OK == Result) &&
          (DCM_RX_CONNECTION_CANCELLED != Dcm_RxConnections[DcmRxPduId].Status)
         )
      {
        Dcm_BufferType ReceivedBuffer;

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_OFF)
        if ((DCM_BUSYREPEATREQUEST_REQUEST == Dcm_RxConnections[DcmRxPduId].RequestType) ||
            (DCM_CONCURRENT_TESTERPRESENT_REQUEST == Dcm_RxConnections[DcmRxPduId].RequestType))
        {
          /* Receptions which go to Default Buffers will have to have the buffer data adjusted
          accordingly, since only a maximum of two bytes will be valid in this case. */
          ReceivedBuffer.Size = DCM_BUFFER_DEFAULT_BUFFER_SIZE;

          if (DCM_BUFFER_DEFAULT_BUFFER_SIZE < Dcm_RxConnections[DcmRxPduId].Buffer.FilledLength)
          {
            ReceivedBuffer.FilledLength = DCM_BUFFER_DEFAULT_BUFFER_SIZE;
          }
          else
          {
            ReceivedBuffer.FilledLength = Dcm_RxConnections[DcmRxPduId].Buffer.FilledLength;
          }
        }
        else
#endif /* #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_OFF) */
        {
          /* Store the amount of thata that was received */
          ReceivedBuffer.Size = Dcm_RxConnections[DcmRxPduId].Buffer.Size;

          /* Store the amount of thata that was received */
          ReceivedBuffer.FilledLength = Dcm_RxConnections[DcmRxPduId].Buffer.FilledLength;
        }

        /* Where the data is */
        ReceivedBuffer.BufferPtr = Dcm_RxConnections[DcmRxPduId].Buffer.BufferPtr;

        /* And the ID of this buffer, so that the Protocol or whoever owns
           it further down the way may deallocate it. */
        ReceivedBuffer.BufferId = Dcm_RxConnections[DcmRxPduId].Buffer.BufferId;

        if (DCM_BUFFER_IS_NORMAL_BUFFER(ReceivedBuffer.BufferId))
        {
          /* Only a normal buffer has a state */
          (void)Dcm_Dsl_BufferChangeState(DcmRxPduId,
                                          DCM_BUFFER_RX_NORMAL_DONE,
                                          ReceivedBuffer.BufferId);
        }

        Dcm_Dsl_RxConnectionRxIndication(DcmRxPduId,
                                         Dcm_RxConnections[DcmRxPduId].RequestType,
                                         ReceivedBuffer);
      }
      else
      {
        /* !LINKSTO Dcm.EB.RequestHandling.RxIndication.ResultNotNTFRSLT_OK.Discard,1 */
        /* Release the used reception buffer */
        (void)Dcm_Dsl_BufferInitializeSingleBuffer(DcmRxPduId,
                                                   Dcm_RxConnections[DcmRxPduId].Buffer.BufferId);

        /* Notify the Supervisor that the reception has ended. */
        Dcm_Dsl_Supervisor_IncomingReceptionFinished(DcmRxPduId);
      }

      /* This RxConnection can be freed now. Even though the RxBuffer might still be occupied,
         the RxConnection may still receive ONLY potential concurrent TesterPresent requests. */
      Dcm_Dsl_RxConnectionInitSingleRxConnection(DcmRxPduId);

      /* LEAVE critical section */
      SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
    }
  }

  DBG_DCM_TPRXINDICATION_EXIT(DcmRxPduId,Result);

  return;
}

/* Get the state of a rx connection */
FUNC(Dcm_RxConnectionStatusType, DCM_CODE) Dcm_Dsl_RxConnectionGetState(
  Dcm_BufferUserType RxConnectionId)
{
  Dcm_RxConnectionIndexType RxConnectionIndex = DCM_RX_CONNECTION_INDEX_FROM_ID(RxConnectionId);
  Dcm_RxConnectionStatusType RxConnectionStatus;

  DBG_DCM_DSL_RXCONNECTIONGETSTATE_ENTRY(RxConnectionId);

  DCM_PRECONDITION_ASSERT((RxConnectionIndex < DCM_NUM_RX_CONNECTIONS), DCM_INTERNAL_API_ID);

  RxConnectionStatus = Dcm_RxConnections[RxConnectionIndex].Status;

  DBG_DCM_DSL_RXCONNECTIONGETSTATE_EXIT(RxConnectionStatus,RxConnectionId);

  return RxConnectionStatus;
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.RxConnection.GetRxPduIdInformation,1 */
/* Fetches all data relevant to a certain RxPduId */
FUNC(void, DCM_CODE) Dcm_Dsl_RxConnectionGetRxPduIdInformation(
  PduIdType RxPduId,
  P2VAR(Dcm_RxPduIdInfoType, AUTOMATIC, DCM_VAR) RxPduIdInfo
)
{
  DBG_DCM_DSL_RXCONNECTIONGETRXPDUIDINFORMATION_ENTRY(RxPduId, RxPduIdInfo);

  RxPduIdInfo->RxConnectionIndex = RxPduId;
  RxPduIdInfo->RxConnectionId = RxPduId;
  RxPduIdInfo->MainConnectionIndex = Dcm_RxPduIdConfig[RxPduId].MainConnectionIndex;
  RxPduIdInfo->ProtocolIndex = Dcm_MainConnectionConfig[RxPduIdInfo->MainConnectionIndex]
                                                       .ProtocolIndex;
  RxPduIdInfo->ProtocolId = DCM_PROTOCOL_ID_FROM_INDEX(RxPduIdInfo->ProtocolIndex);
  RxPduIdInfo->RxBufferId = Dcm_RxPduIdConfig[RxPduId].BufferIndex;
  RxPduIdInfo->DefaultRxBufferId = DCM_DEFAULT_BUFFER_ID_FROM_RX_CONNECTION_ID(RxPduId);
  RxPduIdInfo->AddressingType = Dcm_RxPduIdConfig[RxPduId].AddressingType;
  RxPduIdInfo->ChannelIdIndex = Dcm_RxPduIdConfig[RxPduId].ChannelIdIndex;
  RxPduIdInfo->RxBufferSize = Dcm_BufferConfig[Dcm_RxPduIdConfig[RxPduId].BufferIndex].Size;
  RxPduIdInfo->ComMChannelId = Dcm_RxComMChannelIDs[Dcm_RxPduIdConfig[RxPduId].ChannelIdIndex];

  DBG_DCM_DSL_RXCONNECTIONGETRXPDUIDINFORMATION_EXIT(RxPduId, RxPduIdInfo);
}


/* This is a wrapper function so as to abstract the path requests comming from different
   units. Basically, the RxConnection either receives a request from the outside and
   forwards it or receives an internally generated request from either the ROE, PeriodicResponse
   or BootloaderJump units and then forwards it to the Supervisor module, which then
   has to decide how to treat the request. */

FUNC(void, DCM_CODE) Dcm_Dsl_RxConnectionRxIndication(
  Dcm_BufferUserType RxConnectionId,
  Dcm_RequestTypeType RequestType,
  Dcm_BufferType RequestBuffer
)
{
  DBG_DCM_DSL_RXCONNECTIONRXINDICATION_ENTRY(RxConnectionId, RequestType, RequestBuffer);

  /* Give the rx buffer to the supervisor for further processing */
  (void)Dcm_Dsl_BufferGive(RxConnectionId,
                           DCM_SUPERVISOR_ID,
                           RequestBuffer.BufferId);

  /* Now formally "hand over" the request to the Supervisor.
     In order to further "obscure" the data of the lower layer, the upper layer receives
     everything it needs to know about the buffer it receives: address, received length.
     It NEVER needs to access the information structures of the lower layer. */
  Dcm_Dsl_Supervisor_ProcessRequest(RxConnectionId, RequestType, RequestBuffer);
  DBG_DCM_DSL_RXCONNECTIONRXINDICATION_EXIT(RxConnectionId, RequestType, RequestBuffer);
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.RxConnection.CancelReceive,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_RxConnectionCancelReceive(
  Dcm_BufferUserType RxConnectionId
)
{
  boolean PduRCancelReceiveCallNecessary = FALSE;

  DBG_DCM_DSL_RXCONNECTIONCANCELRECEIVE_ENTRY(RxConnectionId);
  /* Pretty straight-forward, a cancelled RxConnection will not receive any data but it will
     only be truly stopped by an RxIndication from the lower layer */

  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  if (DCM_RX_CONNECTION_IDLE != Dcm_RxConnections[RxConnectionId].Status)
  {
    Dcm_RxConnections[RxConnectionId].Status = DCM_RX_CONNECTION_CANCELLED;

    /* Only RxConnections which are ongoing (not idle) need to be cancelled. */
    PduRCancelReceiveCallNecessary = TRUE;
  }

  /* LEAVE critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  if (TRUE == PduRCancelReceiveCallNecessary)
  {
    /* Avoid calling an API of another module from a critical section. */

    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.CancelReceptionsOnLowerPriorityProtocolsOnSameStack,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.CancelReceptionsOnLowerPriorityProtocolsOnSameStack,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.TYPE1Periodic.AcceptedForProcessing.CancelReceptionsOnLowerPriorityProtocolsOnSameStack,1 */
    (void)PduR_DcmCancelReceive(RxConnectionId);
  }


  DBG_DCM_DSL_RXCONNECTIONCANCELRECEIVE_EXIT(RxConnectionId);
}


#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.RxConnection.SetupRxConnection,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_SetupRxConnection(
  Dcm_RxConnectionIndexType RxConnectionIndex,
  Dcm_RequestTypeType RequestType,
  Dcm_BufferType Buffer,
  PduLengthType ExpectedRxLength
)
{
   Dcm_RxConnections[RxConnectionIndex].RequestType = RequestType;

   /* store the received DataLength */
   Dcm_RxConnections[RxConnectionIndex].Buffer.BufferId = Buffer.BufferId;
   Dcm_RxConnections[RxConnectionIndex].Buffer.BufferPtr = Buffer.BufferPtr;
   Dcm_RxConnections[RxConnectionIndex].ExpectedRxLength = ExpectedRxLength;
   Dcm_RxConnections[RxConnectionIndex].Buffer.Size = Buffer.Size;
   /* set current dataMark to 0 to signal where to copy new data. */
   Dcm_RxConnections[RxConnectionIndex].Buffer.FilledLength = 0U;

   /* The current RxConnection is now receiving something */
   Dcm_RxConnections[RxConnectionIndex].Status = DCM_RX_CONNECTION_RECEIVING;
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.AllocateRxBuffer,1 */
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_Dsl_AllocateRxBuffer(
  Dcm_BufferUserType RxConnectionId,
  PduLengthType RequestedSize,
  P2VAR(Dcm_RequestTypeType, AUTOMATIC, DCM_VAR)  RequestType,
  P2VAR(Dcm_BufferType, AUTOMATIC, DCM_VAR) Buffer
)
{
  BufReq_ReturnType BufferAllocationResult = BUFREQ_E_NOT_OK;
  Dcm_RequestTypeType LocalRequestType = *RequestType;

  switch (LocalRequestType)
  {
    case DCM_NORMAL_PHYSICAL_REQUEST:
    case DCM_NORMAL_FUNCTIONAL_REQUEST:
    {
      BufferAllocationResult = Dcm_Dsl_BufferAllocate(RxConnectionId,
                                                      DCM_BUFFER_RX_NORMAL,
                                                      RequestedSize,
                                                      Buffer);

      /* If buffer allocation fails because the buffer is already
         busy we may return a BusyRepeatRequest NRC. This is not
         the same feature as the RespOnSecondDeclinedRequest flag and
         is therefore independent. This always results in the assignment
         to a default 3-byte buffer because the incoming request may even
         be a concurrent TesterPresent */
      if (BUFREQ_OK == BufferAllocationResult)
      {
        /* Do nothing */
      }
      else if (BUFREQ_E_BUSY == BufferAllocationResult)
      {
        /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_TpRxIndication.ResultSetToNTFRSLT_OK.UnlockReceiveBuffer,1 */
        /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_TpRxIndication.ResultDifferentThanNTFRSLT_OK.UnlockReceiveBuffer,1 */
        /* If the regular buffer is busy, we assign the
           default buffer for this reception */

#if (DCM_RESPOND_WITH_BUSYREPEATREQUEST == STD_OFF)
        /* Only accept potential concurrent TesterPresent requests in
           the RxConnection's Default Buffer */
        if ((2U == RequestedSize) &&
            (DCM_FUNCTIONAL_ADDRESSING == Dcm_RxPduIdConfig[RxConnectionId].AddressingType))
        {
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NoCurrentProtocol.RequestedProtocolRunningTYPE2ROE.BufferBusy.BRRDisabled.PotentialConcurrentTesterPresent.AcceptRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.OnCurrentProtocol.RequestedProtocolRunningTYPE2ROE.BufferBusy.BRRDisabled.PotentialConcurrentTesterPresent.AcceptRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.RequestedProtocolNotRunningTYPE2ROE.BufferBusy.BRRDisabled.PotentialConcurrentTesterPresent.AcceptRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.RequestedProtocolRunningTYPE2ROE.BufferBusy.BRRDisabled.PotentialConcurrentTesterPresent.AcceptRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.NonDefaultSession.RequestedProtocolNotRunningTYPE2ROE.BufferBusy.BRRDisabled.PotentialConcurrentTesterPresent.AcceptRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.NonDefaultSession.RequestedProtocolRunningTYPE2ROE.BufferBusy.BRRDisabled.PotentialConcurrentTesterPresent.AcceptRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolRunningTYPE1ROE.DefaultSession.BufferBusy.BRRDisabled.PotentialConcurrentTesterPresent.AcceptRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.RequestedProtocolNotRunningTYPE2ROE.BufferBusy.BRRDisabled.PotentialConcurrentTesterPresent.AcceptRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.RequestedProtocolRunningTYPE2ROE.BufferBusy.BRRDisabled.PotentialConcurrentTesterPresent.AcceptRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.OnCurrentProtocol.RequestedProtocolRunningTYPE1ROE.BufferBusy.BRRDisabled.PotentialConcurrentTesterPresent.AcceptRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NoCurrentProtocol.RequestedProtocolNotRunningTYPE2ROE.BufferBusy.BRRDisabled.PotentialConcurrentTesterPresent.AcceptRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.OnCurrentProtocol.RequestedProtocolIdle.BufferBusy.BRRDisabled.PotentialConcurrentTesterPresent.AcceptRx,1 */

#endif /* #if (DCM_RESPOND_WITH_BUSYREPEATREQUEST == STD_OFF) */
          /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.DefaultBufferSize,1 */

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
          BufferAllocationResult = Dcm_Dsl_BufferAllocate(RxConnectionId,
                                                          DCM_BUFFER_RX_DEFAULT,
                                                          RequestedSize,
                                                          Buffer);
#else
          BufferAllocationResult = Dcm_Dsl_BufferAllocate(RxConnectionId,
                                                          DCM_BUFFER_RX_DEFAULT,
                                                          DCM_BUFFER_DEFAULT_BUFFER_SIZE,
                                                          Buffer);
#endif

#if (DCM_RESPOND_WITH_BUSYREPEATREQUEST == STD_OFF)
          LocalRequestType = DCM_CONCURRENT_TESTERPRESENT_REQUEST;
        }
        else
        {
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NoCurrentProtocol.RequestedProtocolRunningTYPE2ROE.BufferBusy.BRRDisabled.NotConcurrentTesterPresentSize.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NoCurrentProtocol.RequestedProtocolRunningTYPE2ROE.BufferBusy.BRRDisabled.NotConcurrentTesterPresentFunctional.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.OnCurrentProtocol.RequestedProtocolRunningTYPE2ROE.BufferBusy.BRRDisabled.NotConcurrentTesterPresentSize.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.OnCurrentProtocol.RequestedProtocolRunningTYPE2ROE.BufferBusy.BRRDisabled.NotConcurrentTesterPresentFunctional.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.RequestedProtocolNotRunningTYPE2ROE.BufferBusy.BRRDisabled.NotConcurrentTesterPresentSize.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.RequestedProtocolNotRunningTYPE2ROE.BufferBusy.BRRDisabled.NotConcurrentTesterPresentFunctional.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.RequestedProtocolRunningTYPE2ROE.BufferBusy.BRRDisabled.NotConcurrentTesterPresentSize.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.RequestedProtocolRunningTYPE2ROE.BufferBusy.BRRDisabled.NotConcurrentTesterPresentFunctional.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.NonDefaultSession.RequestedProtocolNotRunningTYPE2ROE.BufferBusy.BRRDisabled.NotConcurrentTesterPresentSize.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.NonDefaultSession.RequestedProtocolNotRunningTYPE2ROE.BufferBusy.BRRDisabled.NotConcurrentTesterPresentFunctional.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.NonDefaultSession.RequestedProtocolRunningTYPE2ROE.BufferBusy.BRRDisabled.NotConcurrentTesterPresentSize.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.NonDefaultSession.RequestedProtocolRunningTYPE2ROE.BufferBusy.BRRDisabled.NotConcurrentTesterPresentFunctional.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolRunningTYPE1ROE.DefaultSession.BufferBusy.BRRDisabled.NotConcurrentTesterPresentSize.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolRunningTYPE1ROE.DefaultSession.BufferBusy.BRRDisabled.NotConcurrentTesterPresentFunctional.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.RequestedProtocolNotRunningTYPE2ROE.BufferBusy.BRRDisabled.NotConcurrentTesterPresentSize.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.RequestedProtocolNotRunningTYPE2ROE.BufferBusy.BRRDisabled.NotConcurrentTesterPresentFunctional.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.RequestedProtocolRunningTYPE2ROE.BufferBusy.BRRDisabled.NotConcurrentTesterPresentSize.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.RequestedProtocolRunningTYPE2ROE.BufferBusy.BRRDisabled.NotConcurrentTesterPresentFunctional.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.OnCurrentProtocol.RequestedProtocolRunningTYPE1ROE.BufferBusy.BRRDisabled.NotConcurrentTesterPresentSize.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.OnCurrentProtocol.RequestedProtocolRunningTYPE1ROE.BufferBusy.BRRDisabled.NotConcurrentTesterPresentFunctional.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NoCurrentProtocol.RequestedProtocolNotRunningTYPE2ROE.BufferBusy.BRRDisabled.NotConcurrentTesterPresentSize.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NoCurrentProtocol.RequestedProtocolNotRunningTYPE2ROE.BufferBusy.BRRDisabled.NotConcurrentTesterPresentFunctional.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.OnCurrentProtocol.RequestedProtocolIdle.BufferBusy.BRRDisabled.NotConcurrentTesterPresentSize.RejectRx,1 */
          /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.OnCurrentProtocol.RequestedProtocolIdle.BufferBusy.BRRDisabled.NotConcurrentTesterPresentFunctional.RejectRx,1 */

          BufferAllocationResult = BUFREQ_E_NOT_OK;

          LocalRequestType = DCM_REJECTED_REQUEST;
        }
#endif /* #if (DCM_RESPOND_WITH_BUSYREPEATREQUEST == STD_OFF) */
#if (DCM_RESPOND_WITH_BUSYREPEATREQUEST == STD_ON)
        /* Accept all requests into the RxConnection's DefaultBuffer */

        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NoCurrentProtocol.RequestedProtocolRunningTYPE2ROE.BufferBusy.BRREnabled.AcceptRx,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.OnCurrentProtocol.RequestedProtocolRunningTYPE2ROE.BufferBusy.BRREnabled.AcceptRx,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.RequestedProtocolNotRunningTYPE2ROE.BufferBusy.BRREnabled.AcceptRx,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.RequestedProtocolRunningTYPE2ROE.BufferBusy.BRREnabled.AcceptRx,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.NonDefaultSession.RequestedProtocolNotRunningTYPE2ROE.BufferBusy.BRREnabled.AcceptRx,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.NonDefaultSession.RequestedProtocolRunningTYPE2ROE.BufferBusy.BRREnabled.AcceptRx,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolLowerPrio.CurrentProtocolRunningTYPE1ROE.DefaultSession.BufferBusy.BRREnabled.AcceptRx,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.RequestedProtocolNotRunningTYPE2ROE.BufferBusy.BRREnabled.AcceptRx,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.RequestedProtocolRunningTYPE2ROE.BufferBusy.BRREnabled.AcceptRx,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.OnCurrentProtocol.RequestedProtocolRunningTYPE1ROE.BufferBusy.BRREnabled.AcceptRx,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.NoCurrentProtocol.RequestedProtocolNotRunningTYPE2ROE.BufferBusy.BRREnabled.AcceptRx,1 */
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.OnCurrentProtocol.RequestedProtocolIdle.BufferBusy.BRREnabled.AcceptRx,1 */

        LocalRequestType = DCM_BUSYREPEATREQUEST_REQUEST;
#endif /* #if (DCM_RESPOND_WITH_BUSYREPEATREQUEST == STD_ON) */
      }
      else
      {
        /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.StartOfReception.TpSduLengthTooLarge.RejectRx,1 */
        /* If there is an overflow, we simply need to reject the request */
        LocalRequestType = DCM_REJECTED_REQUEST;
      }
      }
    break;

    case DCM_CONCURRENT_TESTERPRESENT_REQUEST:
#if (DCM_RESPOND_WITH_BUSYREPEATREQUEST == STD_ON)
    case DCM_BUSYREPEATREQUEST_REQUEST:
#endif /* #if (DCM_RESPOND_WITH_BUSYREPEATREQUEST == STD_ON) */
#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
      BufferAllocationResult = Dcm_Dsl_BufferAllocate(RxConnectionId,
                                                      DCM_BUFFER_RX_DEFAULT,
                                                      RequestedSize,
                                                      Buffer);
#else
      BufferAllocationResult = Dcm_Dsl_BufferAllocate(RxConnectionId,
                                                      DCM_BUFFER_RX_DEFAULT,
                                                      DCM_BUFFER_DEFAULT_BUFFER_SIZE,
                                                      Buffer);
#endif
      break;

    /* CHECK: NOPARSE*/
    default:
      /* Unreachable default case mandated by MISRA */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE*/
  }

  *RequestType = LocalRequestType;

  return BufferAllocationResult;
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.RxConnection.InitSingleRxConnection,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_RxConnectionInitSingleRxConnection(
  Dcm_RxConnectionIndexType RxConnectionIndex)
{
  Dcm_RxConnections[RxConnectionIndex].RequestType = DCM_INVALID_REQUEST_TYPE;
  Dcm_RxConnections[RxConnectionIndex].Buffer.BufferId = DCM_BUFFER_INVALID_ID;
  Dcm_RxConnections[RxConnectionIndex].Buffer.BufferPtr = NULL_PTR;
  Dcm_RxConnections[RxConnectionIndex].ExpectedRxLength = 0U;
  Dcm_RxConnections[RxConnectionIndex].Buffer.Size = 0U;
  Dcm_RxConnections[RxConnectionIndex].Buffer.FilledLength = 0U;
  Dcm_RxConnections[RxConnectionIndex].Status = DCM_RX_CONNECTION_IDLE;
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.RxConnection.Init,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_RxConnectionInit(
  void
)
{
  uint16_least RxConnectionIndex;

 /* Deviation TASKING-1 */
  for (RxConnectionIndex = 0; RxConnectionIndex < DCM_NUM_RX_CONNECTIONS ; RxConnectionIndex++ )
  {
    Dcm_Dsl_RxConnectionInitSingleRxConnection((Dcm_RxConnectionIndexType)RxConnectionIndex);
  }
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*==================[end of file]================================================================*/
