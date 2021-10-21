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

/* !LINKSTO Dcm.Dsn.File.LowerLayerInterfaces.TxConnection.Impl,1 */
/* This file contains the implementation of the TxConnections software unit. */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 16.1 (required)
  *     "All switch statements shall be well-formed"
  *
  *     Reason:
  *     This violation is present because rule 16.4 is violated and justified inside the affected
  *     switch statement, as noted under rule 16.1:
  *     some of the restrictions imposed on switch statements by this rule are expounded in the
  *     15.3, 16.2, 16.3, 16.4, 16.5, 16.6 rules, which are violated inside the current switch statement.
  *
  *  MISRAC2012-2) Deviated Rule: 16.4 (required)
  *     "Every switch statement shall have a default label"
  *
  *     Reason:
  *     On the default case no action is established for the connections that are in Idle state
  *     or an unknown value which could have corrupted the connection state variable.
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
#include <Dcm_Dsl_CommunicationServices_TxConnections.h>  /* Dcm Dsl TxConnections header file */
#include <Dcm_Dsl_CommunicationServices_BufferManager.h>  /* Dcm Dsl BufferManager header file */
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
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

STATIC FUNC(PduIdType, DCM_CODE) Dcm_Dsl_GetTxPduIdFromTxConnectionIndex(
  Dcm_TxConnectionIndexType TxConnectionIndex
);

#if (DCM_META_DATA_HANDLING_ENABLED == STD_ON)
STATIC FUNC(PduIdType, DCM_CODE) Dcm_Dsl_GetGenericConnIdFromTxConnectionIndex(
  Dcm_TxConnectionIndexType TxConnectionIndex,
  P2VAR(boolean, AUTOMATIC, DCM_VAR) GenericConnIdFound,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) EcuCPduId
);
#endif /* #if (DCM_META_DATA_HANDLING_ENABLED == STD_ON) */

/** \brief Sets up a TxConnection entity so that a data transmission may take place on
 *         the corresponding TxPduId.
 *
 *  \param TxConnectionIndex [in]         Index of the TxConnection which is to be set up. The
 *                                        index of the TxConnection is the same as the
 *                                        ConfirmationTxPduId associated to the TxPduId on which
 *                                        the transmission shall take place.
 *  \param ContextComMChannelIdIndex [in] The ComMChannelId to which the RxPduId on which the
 *                                        request which triggered this transmission belongs.
 *  \param Requester [in]                 The unique ID of the BufferUser entity requesting this
 *                                        transmission.
 *  \param SimulateTransmission [in]      Designates whether this transmission shall be simulated
 *                                        (suppressed) or not.
 *  \param AmountToTransmit [in]          The complete number of bytes which are to be transmitted
 *                                        in this transmission session.
 *  \param TransmissionTimeout [in]       The maximum number of Dcm_MainFunction() cycles to wait
 *                                        for the ComMChannelId to enter FullCommunicationMode
 *                                        in case it is in a different mode before the transmission
 *                                        is considered to have failed.
 *  \param Buffer [in]                    A Buffer entity that contains data to be transmitted. If
 *                                        Paged Buffer handling is used, this Buffer is not
 *                                        required to contain the complete amount of data to be
 *                                        sent. In this case, the TxConnection shall request the
 *                                        next packet of data by calling the _ProvideNextTxBuffer
 *                                        function of the Requester entity.
 *
 *  \return none
 */
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_SetupTxConnection(
  Dcm_TxConnectionIndexType TxConnectionIndex,
  Dcm_RxComMChannelIDsIndexType ContextComMChannelIdIndex,
  Dcm_BufferUserType Requester,
  boolean SimulateTransmission,
  Dcm_MsgLenType AmountToTransmit,
  Dcm_TransmissionTimeoutType TransmissionTimeout,
  Dcm_BufferType Buffer
);

/** \brief Attempts to start the state machine of a TxConnection that has been previously set up
 *         using the Dcm_Dsl_SetupTxConnection() function.
 *
 *  \param TxConnectionIndex [in]     Index of the TxConnection which is to be set up. The
 *                                    index of the TxConnection is the same as the
 *                                    ConfirmationTxPduId associated to the TxPduId on which
 *                                    the transmission shall take place.
 *
 *  \return Dcm_TxResultType
 *  \retval DCM_TX_OK                 The TxConnection state machine has been started successfully
 *                                    and the transmission is now in progress.
 *  \retval DCM_TX_FAILED             The TxConnection state machine has not been started
 *                                    either as a result of the PduR_DcmTransmit() API returning
 *                                    an E_NOT_OK status, or the TxConnection having timed out
 *                                    waiting for the corresponding ComMChannel to enter
 *                                    FULL_COMMUNICATION_MODE. This transmission has not been
 *                                    initiated and no Dcm_(Tp)TxConfirmation() call is expected.
 *  \retval DCM_TX_DELAYED            The ComMChannel has not entered FullCommunicationMode, but
 *                                    the timeout given in the Dcm_TxConnectionTransmit() call has
 *                                    not expired. Starting the state machine of this TxConnection
 *                                    shall be retried in the next Dcm_MainFunction() cycle.
 */
STATIC FUNC (Dcm_TxResultType, DCM_CODE) Dcm_Dsl_TxConnectionStart(
  Dcm_TxConnectionIndexType TxConnectionIndex
);

/** \brief Notifies that a transmission has timed waiting for the next page of data to be provided.
 *         NOTE: in order for this transmission session to be considered as complete, the
 *         corresponding Dcm_(Tp)TxConfirmation() call must still be executed by the lower layer.
 *         All Dcm_CopyTxData() calls executed by the lower layer shall return BUFREQ_E_NOT_OK
 *         after such a timeout, until the Dcm_(Tp)TxConfirmation() API is called by the lower
 *         layer.
 *
 *  \param Requester [in]               The unique BufferUser ID of the entity which requested this
 *                                      transmission session.
 *  \param TxConnectionId [in]          The unique BufferUser ID of the TxConnection requesting the
 *                                      next transmission Buffer to be provided.
 *
 *  \return none
 */
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_NotifyTransmissionTimeoutToRequester(
  Dcm_BufferUserType Requester,
  Dcm_BufferUserType TxConnectionId
);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.RuntimeDataElements,1 */

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.ParallelTxConnections,1 */
/** \brief Contains the state variables of all the TxConnection entities.
 */
VAR(Dcm_TxConnectionType, DCM_VAR) Dcm_TxConnections[DCM_NUM_TX_CONNECTIONS];

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.DiagnosticSessionLayer.RuntimeDataElements,1 */


#define DCM_START_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/** \brief Controls whether there is at least one TxConnection which needs to have its current
 *         state serviced during the next Dcm_MainFunction() cycle.
 */
VAR (boolean, DCM_VAR) Dcm_ProcessTxConnectionsOnMainFunction = FALSE;

#define DCM_STOP_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.API,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData,1 */
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_CopyTxData(
  PduIdType DcmTxPduId,
  P2VAR(PduInfoType, AUTOMATIC, DCM_APPL_DATA) PduInfoPtr,
  P2VAR(RetryInfoType, AUTOMATIC, DCM_APPL_DATA) RetryInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DCM_APPL_DATA) TxDataCntPtr
)
{
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.DETCheckFails.ReturnBUFREQ_E_NOT_OK,1 */
  BufReq_ReturnType DataCopyResult = BUFREQ_E_NOT_OK;

  DBG_DCM_COPYTXDATA_ENTRY(DcmTxPduId,PduInfoPtr,RetryInfoPtr,TxDataCntPtr);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.ModuleNotInitialized.ReportDETError,1 */
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SVCID_COPYTXDATA, DCM_E_UNINIT);
  }
  /* DcmTxPduId is actually the ConfirmationTxPduId, therefore the check verifies its
   * validity against the total number of Transmission(Tx) Confirmation Pdu Ids
   */
   /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.InvalidDcmTxPduId.ReportDETError,1 */
  else if (DcmTxPduId >= DCM_NUM_TX_CONNECTIONS)
  {
    DCM_DET_REPORT_ERROR(DCM_SVCID_COPYTXDATA, DCM_E_PARAM);
  }
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.InvalidRetryInfoPtrInvalidTpDataState.ReportDETError,1 */
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.InvalidPduInfoPtr.ReportDETError,1 */
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.InvalidTxDataCntPtr.ReportDETError,1 */
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.InvalidSduDataPtrSduLengthIsZero.ReportDETError,1 */
  else if ( (PduInfoPtr == NULL_PTR) ||
            (TxDataCntPtr == NULL_PTR) ||
            ( (NULL_PTR == PduInfoPtr->SduDataPtr) &&
              (0U != PduInfoPtr->SduLength)
            )
          )
  {
    DCM_DET_REPORT_ERROR(DCM_SVCID_COPYTXDATA, DCM_E_PARAM_POINTER);
  }
  /* Shortcut processing allows us to check whether the pointer is NULL
     before dereferencing it */
  else if((RetryInfoPtr != NULL_PTR) &&
          (RetryInfoPtr->TpDataState != TP_CONFPENDING) &&
          (RetryInfoPtr->TpDataState != TP_DATARETRY) &&
          (RetryInfoPtr->TpDataState != TP_DATACONF))
  {
    DCM_DET_REPORT_ERROR(DCM_SVCID_COPYTXDATA, DCM_E_PARAM);
  }
  else
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */
  {
    /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.InterruptContext,1 */
    /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.CopyData,1 */
    /* ENTER critical section */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    if (!((DCM_TX_CONNECTION_WAITING_FOR_DATA == Dcm_TxConnections[DcmTxPduId].Status) ||
          (DCM_TX_CONNECTION_TRANSMITTING == Dcm_TxConnections[DcmTxPduId].Status)
         )
        )
    {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      if ((DCM_TX_CONNECTION_IDLE == Dcm_TxConnections[DcmTxPduId].Status) ||
          (DCM_TX_CONNECTION_WAITING_FOR_CHANNEL == Dcm_TxConnections[DcmTxPduId].Status) ||
          (DCM_TX_CONNECTION_REQUEST_DATA == Dcm_TxConnections[DcmTxPduId].Status)
         )
      {
        /* This Dcm_CopyTxData() call does not follow a PduR_DcmTransmit() call
           and we therefore have a sequencing error. */

        DCM_DET_REPORT_ERROR(DCM_SVCID_COPYTXDATA, DCM_E_PARAM);
      }
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */

      /* It is possible that this TxConnection has already been cancelled by its
         requester. In this case just return an error to the lower layer. */
      /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.CancelReceptionsOnLowerPriorityProtocolsOnSameStack.ReturnBUFREQ_E_NOT_OK,1 */
      DataCopyResult = BUFREQ_E_NOT_OK;
    }
    else
    {
      /* Because ConfirmationTxPduIds are 0-based and consecutive,
         the DcmTxPduId parameter actually gives the index of
         the TxConnection as well. */

      Std_ReturnType RetryInformationProcessing;
      PduLengthType PreviousBufferConfirmedDataMark =
        Dcm_TxConnections[DcmTxPduId].BufferConfirmedDataMark;

      /* Adjust the DataMark and the ConfirmedDataMark of the current TxBuffer
         according to the information within the RetryInfoPtr. */
      RetryInformationProcessing = Dcm_Dsl_ProcessRetryInformation(RetryInfoPtr,
                                     &Dcm_TxConnections[DcmTxPduId].BufferDataMark,
                                     &Dcm_TxConnections[DcmTxPduId].BufferConfirmedDataMark);

      /* If the RetryInfoPtr contains valid data and if the amount of data requested to be copied
         does not exceed the total amount of data left to be copied, continue. */
      if (E_OK == RetryInformationProcessing)
      {
        /* Update the total amount of bytes left to be copied to the lower layer. This will
        go down by the amount that was confirmed within the current call. */
        Dcm_TxConnections[DcmTxPduId].AmountLeftToCopy -= Dcm_TxConnections[
            DcmTxPduId].BufferConfirmedDataMark - PreviousBufferConfirmedDataMark;

        if (((Dcm_TxConnections[DcmTxPduId].BufferDataMark -
              Dcm_TxConnections[DcmTxPduId].BufferConfirmedDataMark) +
              PduInfoPtr->SduLength
             ) <= Dcm_TxConnections[DcmTxPduId].AmountLeftToCopy)
        {
          switch (Dcm_TxConnections[DcmTxPduId].Status)
          {
            case DCM_TX_CONNECTION_WAITING_FOR_DATA:
              /* This TxConnection is waiting for a new page of data (a buffer) to be
                 provided by the upper layer. Therefore,
                 report to the lower layer that 0 bytes are available. */

              *TxDataCntPtr = 0U;

              if (PduInfoPtr->SduLength != 0U)
              {
                /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.NoData,1 */
                /* If any data was requested, return a BUFREQ_E_BUSY status.
                 */
                DataCopyResult = BUFREQ_E_BUSY;
              }
              else
              {
                /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.AvailableData,1 */
                /* The PduR may still inquire about the amount of data available in the Tx buffer,
                 * therefore, if this API is called with PduInfoPtr->SduLength == 0U,
                 * return BUFREQ_OK
                 */
                DataCopyResult = BUFREQ_OK;
              }

              break;

            case DCM_TX_CONNECTION_TRANSMITTING:
              /* This TxConnection has data available in its current buffer. */

              if((Dcm_TxConnections[DcmTxPduId].Buffer.FilledLength -
                  Dcm_TxConnections[DcmTxPduId].BufferDataMark) < PduInfoPtr->SduLength)
              {
                DataCopyResult = BUFREQ_E_BUSY;
              }
              else
              {
                /* Just in case the lower layer is just inquiring the amount of available
                   data. */
                PduLengthType SourceIndex = Dcm_TxConnections[DcmTxPduId].BufferDataMark;
                P2VAR(uint8, AUTOMATIC, DCM_VAR) SourcePtr =
                  (uint8*)(&((Dcm_TxConnections[DcmTxPduId].Buffer.BufferPtr)[SourceIndex]));

                if(PduInfoPtr->SduLength != 0U)
                {
                  /* Copy 'SduLength' bytes from the Buffer to the pointer 'SduDataPtr' */
                  TS_MemCpy(PduInfoPtr->SduDataPtr,
                    SourcePtr,
                    PduInfoPtr->SduLength);

                  /* Mark the position of payload data already transmitted within the Buffer */
                  Dcm_TxConnections[DcmTxPduId].BufferDataMark += PduInfoPtr->SduLength;
                }
                DataCopyResult = BUFREQ_OK;
              }

              /* The remaining amount of data in the buffer has to be returned in all cases. */
              *TxDataCntPtr = (PduLengthType)(Dcm_TxConnections[DcmTxPduId].Buffer.FilledLength -
                Dcm_TxConnections[DcmTxPduId].BufferDataMark);

              /* At the end of this copying operation, whether there was an actual copying or not,
                 if the end of the buffer has been reached, either a new buffer is needed or
                 the copying of the data has finished. */
              if (Dcm_TxConnections[DcmTxPduId].BufferConfirmedDataMark ==
                      Dcm_TxConnections[DcmTxPduId].Buffer.FilledLength)
              {
                /* All data in the current Buffer has been copied and confirmed */
                if (0U == Dcm_TxConnections[DcmTxPduId].AmountLeftToCopy)
                {
                   /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.CopyDataSuccessfully.ReturnBUFREQ_OK,1 */
                   /* If all data has been transmitted, basically there's nothing left to
                      do for this connection than to wait for the appropriate
                      Dcm_(Tp)TxConfirmation() call. */
                      DataCopyResult = BUFREQ_OK;
                }
                else
                {
                  /* If the current buffer has been completely copied, but there is still
                     data that needs to be copied in the context of the current TxConenction,
                     a new buffer is requested from the requester of this transmission session. */

                  /* Give the transmission buffer back to the Requester. */
                  (void)Dcm_Dsl_BufferGive(DCM_TX_CONNECTION_ID_FROM_INDEX(DcmTxPduId),
                                           Dcm_TxConnections[DcmTxPduId].Requester,
                                           Dcm_TxConnections[DcmTxPduId].Buffer.BufferId);

                  Dcm_Dsl_ProvideNextTxBuffer(
                      Dcm_TxConnections[DcmTxPduId].Requester,
                      DCM_TX_CONNECTION_ID_FROM_INDEX(DcmTxPduId));

                    Dcm_TxConnections[DcmTxPduId].Status = DCM_TX_CONNECTION_WAITING_FOR_DATA;
                    Dcm_TxConnections[DcmTxPduId].TransmissionTimeout = DCM_PAGED_BUFFER_TIMEOUT;
                    Dcm_ProcessTxConnectionsOnMainFunction = TRUE;

                    if (PduInfoPtr->SduLength != 0U)
                    {
                    /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.NoData,1 */
                    /* If any data was requested, return a BUFREQ_E_BUSY status.
                       */
                      DataCopyResult = BUFREQ_E_BUSY;
                    }
                    else
                    {
                    /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.AvailableData,1 */
                    /* The PduR may still inquire about the amount of data available
                       * in the Tx buffer, therefore, if this API is called with
                       * PduInfoPtr->SduLength == 0U, return BUFREQ_OK
                       */
                      DataCopyResult = BUFREQ_OK;
                    }
                  }
                  }
              break;

            /* CHECK: NOPARSE */
            default:
              /* Unreachable default case mandated by MISRA */
              DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
              break;
              /* CHECK: PARSE */
          }
        }
        else
        {
          /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.ExceededLength.ReturnBUFREQ_E_NOT_OK,1 */
          /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.NoTransmission.ReturnBUFREQ_E_NOT_OK,1 */
          /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.Dcm_CopyTxDataReturnsBUFREQ_E_NOT_OK.CallDcm_TpTxConfirmation,1 */
          /* The lower layer requests more data than would ever be available for transmission. */
#if (DCM_DEV_ERROR_DETECT == STD_ON)
          DCM_DET_REPORT_ERROR(DCM_SVCID_COPYTXDATA, DCM_E_PARAM);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
          DataCopyResult = BUFREQ_E_NOT_OK;
        }
      }
      else
      {
        /* An error occurred while processing the retry information */
        DataCopyResult = BUFREQ_E_NOT_OK;
      }

      /* Any error in processing this API results in the TxConnection being cancelled. */
      if (BUFREQ_E_NOT_OK == DataCopyResult)
      {
        Dcm_TxConnections[DcmTxPduId].Status = DCM_TX_CONNECTION_ERROR;
      }
    }

    /* EXIT critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
  }

  DBG_DCM_COPYTXDATA_EXIT(DataCopyResult,DcmTxPduId,PduInfoPtr,RetryInfoPtr,TxDataCntPtr);

  return DataCopyResult;
}

/* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_TpTxConfirmation.API,1 */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TpTxConfirmation,1 */
FUNC(void, DCM_CODE) Dcm_TpTxConfirmation(
  PduIdType DcmTxPduId,
  NotifResultType Result
)
{
  DBG_DCM_TPTXCONFIRMATION_ENTRY(DcmTxPduId,Result);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_TpTxConfirmation.ModuleNotInitialized.ReportDETError,1 */
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SVCID_TPTXCONFIRMATION, DCM_E_UNINIT);
  }
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_TpTxConfirmation.InvalidDcmTxPduIdInvalidResult.ReportDETError,1 */
  else if ((DCM_NUM_CONFIRMATION_TX_PDU_ID <= DcmTxPduId) ||
           (DCM_INVALID_NTFRSLT_RANGE_START <= Result))
  {
    DCM_DET_REPORT_ERROR(DCM_SVCID_TPTXCONFIRMATION, DCM_E_PARAM);
  }
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_TpTxConfirmation.ConnectionStatusIsIdle.ReportDETError,1 */
  else if (DCM_TX_CONNECTION_IDLE == Dcm_TxConnections[DcmTxPduId].Status)
  {
    DCM_DET_REPORT_ERROR(DCM_SVCID_TPTXCONFIRMATION, DCM_E_PARAM);
  }
  else
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */
  {
    Dcm_TxResultType TransmissionResult;

    /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_TpTxConfirmation.InterruptContext,1 */
    /* ENTER critical section */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    /* Give the transmission buffer back to the Requester. */
    (void)Dcm_Dsl_BufferGive(DCM_TX_CONNECTION_ID_FROM_INDEX(DcmTxPduId),
                               Dcm_TxConnections[DcmTxPduId].Requester,
                               Dcm_TxConnections[DcmTxPduId].Buffer.BufferId);

    if (NTFRSLT_OK == Result)
    {
      TransmissionResult = DCM_TX_OK;
    }
    else
    {
      TransmissionResult = DCM_TX_FAILED;
    }

    /* Notify the requester of this transmission session that the
       transmission is complete. */
    Dcm_Dsl_ConfirmTransmissionToRequester(Dcm_TxConnections[DcmTxPduId].Requester,
                                           TransmissionResult,
                                           DCM_TX_CONNECTION_ID_FROM_INDEX(DcmTxPduId));

    /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_TpTxConfirmation.ResultDifferentThanNTFRSLT_OK.UnlockTransmitBuffer,1 */
    /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_TpTxConfirmation.ResultSetToNTFSRSLT_OK.UnlockTransmitBuffer,1 */
    /* Reinitialize the TxConnection */
    Dcm_Dsl_TxConnectionInitSingleTxConnection(DcmTxPduId);

    /* EXIT critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
  }

  DBG_DCM_TPTXCONFIRMATION_EXIT(DcmTxPduId,Result);
}

/* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_TxConfirmation.PeriodicTransmission,1 */
/* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.PeriodicTransmission.Dcm_TxConfirmation.API,1 */
FUNC(void, DCM_CODE) Dcm_TxConfirmation(
  PduIdType DcmTxPduId
)
{
  DBG_DCM_TXCONFIRMATION_ENTRY(DcmTxPduId);
#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SVCID_TXCONFIRMATION, DCM_E_UNINIT);
  }
  else if (DcmTxPduId >= DCM_NUM_CONFIRMATION_TX_PDU_ID)
  {
    DCM_DET_REPORT_ERROR(DCM_SVCID_TXCONFIRMATION, DCM_E_PARAM);
  }
  else if (DCM_TX_CONNECTION_IDLE == Dcm_TxConnections[DcmTxPduId].Status)
  {
    DCM_DET_REPORT_ERROR(DCM_SVCID_TXCONFIRMATION, DCM_E_PARAM);
  }
  else
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */
  {
    /* ENTER critical section */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    /* Give the transmission buffer back to the Requester. */
    (void)Dcm_Dsl_BufferGive(DCM_TX_CONNECTION_ID_FROM_INDEX(DcmTxPduId),
                               Dcm_TxConnections[DcmTxPduId].Requester,
                               Dcm_TxConnections[DcmTxPduId].Buffer.BufferId);

    /* Notify the requester of this transmission session that the
       transmission is complete. */
    Dcm_Dsl_ConfirmTransmissionToRequester(Dcm_TxConnections[DcmTxPduId].Requester,
                                           DCM_TX_OK,
                                           DCM_TX_CONNECTION_ID_FROM_INDEX(DcmTxPduId));

    /* Reinitialize the TxConnection */
    Dcm_Dsl_TxConnectionInitSingleTxConnection(DcmTxPduId);

    /* EXIT critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
  }
  DBG_DCM_TXCONFIRMATION_EXIT(DcmTxPduId);
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.GetState,1 */
/* Get the state of a tx connection */
FUNC(Dcm_TxConnectionStatusType, DCM_CODE) Dcm_Dsl_TxConnectionGetState(
  Dcm_BufferUserType TxConnectionId)
{
  Dcm_TxConnectionIndexType TxConnectionIndex = DCM_TX_CONNECTION_INDEX_FROM_ID(TxConnectionId);
  Dcm_TxConnectionStatusType TxConnectionStatus;

  DBG_DCM_DSL_TXCONNECTIONGETSTATE_ENTRY(TxConnectionId);

  DCM_PRECONDITION_ASSERT((TxConnectionIndex < DCM_NUM_TX_CONNECTIONS), DCM_INTERNAL_API_ID);

  TxConnectionStatus = Dcm_TxConnections[TxConnectionIndex].Status;

  DBG_DCM_DSL_TXCONNECTIONGETSTATE_EXIT(TxConnectionStatus,TxConnectionId);

  return TxConnectionStatus;
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.CancelTransmit,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_TxConnectionCancelTransmit(
  Dcm_BufferUserType TxConnectionId
)
{
  Dcm_TxConnectionIndexType TxConnectionIndex;
  PduIdType TxPduId;
  boolean PduRCancelTransmitCallNecessary;

  DBG_DCM_DSL_TXCONNECTIONCANCELTRANSMIT_ENTRY(TxConnectionId);

  TxConnectionIndex = DCM_TX_CONNECTION_INDEX_FROM_ID(TxConnectionId);

  /* Pretty straight-forward, a canceled TxConnection will not transmit any data but it will
     only be truly stopped by a TxConfirmation from the lower layer. Also, only cancel
     TxConnections which are currently doing something. TxConnections which have already
     been stopped (via a call to Dcm_TpTxConfirmation()) by the lower layer are not cancelled */

  TxPduId = Dcm_Dsl_GetTxPduIdFromTxConnectionIndex(TxConnectionIndex);

  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  /* Transmissions which have either not called the PduR for transmission because they are
     either cancelled, in a wait state or suppressed, do not need to call the
     PduR_DcmCancelTransmit() API. For these, immediately report to the Dsp that the transmission
     has failed. */
  if ((DCM_TX_CONNECTION_IDLE != Dcm_TxConnections[TxConnectionIndex].Status) &&
      (DCM_TX_CONNECTION_WAITING_FOR_CHANNEL != Dcm_TxConnections[TxConnectionIndex].Status) &&
      (DCM_TX_CONNECTION_CANCELLED != Dcm_TxConnections[TxConnectionIndex].Status) &&
      (FALSE == Dcm_TxConnections[TxConnectionIndex].SimulateTransmission)
     )
  {
    /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.CancelTransmit.PduRTriggered.TxConfirmation,1 */
    /* Only attempt to cancel a TxConnection which has actually called the PduR for transmission */
    Dcm_TxConnections[TxConnectionIndex].Status = DCM_TX_CONNECTION_CANCELLED;

    /* Only TxConnections which are ongoing (not idle) need to be cancelled. */
    PduRCancelTransmitCallNecessary = TRUE;
  }
  else
  {
    PduRCancelTransmitCallNecessary = FALSE;
  }

  /* LEAVE critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  if (TRUE == PduRCancelTransmitCallNecessary)
  {
    /* Avoid calling an API of another module from within a critical section. */

    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.OBDRequest.NonDefaultSession.ExternalUDS.CancelTransmission,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.OBDRequest.NonDefaultSession.ExternalUDS.CancelTransmission,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.OBDRequest.NonDefaultSession.ApplicationUDS.CancelTransmission,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.OBDRequest.NonDefaultSession.ApplicationUDS.CancelTransmission,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.OBDRequest.NonDefaultSession.TYPE1Periodic.CancelTransmission,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.OBDRequest.NonDefaultSession.TYPE1Periodic.CancelTransmission,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.TYPE1ROE.CancelTransmission,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.TYPE1ROE.CancelTransmission,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.TYPE2ROE.CancelTransmission,1 */
    /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.TYPE2ROE.CancelTransmission,1 */

    (void)PduR_DcmCancelTransmit(TxPduId);
  }
  else
  {
    /* If the TxConnection has not called the PduR for transmission, notify the ending of
       transmission immediately. In this case we can exit the critical section as no
       preemption by a call to any of the transmission API functions from the lower layer
       will take place. */

    /* Give the transmission buffer back to the Requester. */
    (void)Dcm_Dsl_BufferGive(TxConnectionId,
                             Dcm_TxConnections[TxConnectionIndex].Requester,
                             Dcm_TxConnections[TxConnectionIndex].Buffer.BufferId);


    /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.CancelTransmit.PduRNotTriggered.TxConfirmation,1 */
    Dcm_Dsl_ConfirmTransmissionToRequester(Dcm_TxConnections[TxConnectionIndex].Requester,
                                           DCM_TX_FAILED,
                                           TxConnectionId);

    /* Reinitialize the TxConnection */
    Dcm_Dsl_TxConnectionInitSingleTxConnection(TxConnectionIndex);
  }

  DBG_DCM_DSL_TXCONNECTIONCANCELTRANSMIT_EXIT(TxConnectionId);
}

/* Puts one buffer on the bus */
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.Transmit,1, Dcm114,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_TxConnectionTransmit(
  Dcm_BufferUserType TxConnectionId,
  NetworkHandleType ContextComMChannelId,
  Dcm_BufferUserType Requester,
  boolean SuppressTransmission,
  Dcm_MsgLenType AmountToTransmit,
  Dcm_TransmissionTimeoutType TransmissionTimeout,
  Dcm_BufferType Buffer
)
{
  /* Contains the internal information for this TxPduId*/
  Dcm_TxConnectionIndexType TxConnectionIndex;
  boolean TransmissionIsVirtual;
  boolean isSuppressTransmission;

  DBG_DCM_DSL_TXCONNECTIONTRANSMIT_ENTRY(TxConnectionId, ContextComMChannelId, Requester,
    SuppressTransmission, AmountToTransmit, TransmissionTimeout, Buffer);

  isSuppressTransmission = SuppressTransmission;

  /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.DcmDslProtocolTx.ZeroMultiplicity.NoResponse,1 */
  /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.VirtualTxConnection,1 */
  /* !LINKSTO Dcm.EB.Virtual.TxConnection.ResponseSuppression,1 */
  TransmissionIsVirtual = DCM_TX_CONNECTION_IS_VIRTUAL(TxConnectionId);
  if(TransmissionIsVirtual == TRUE)
  {
    isSuppressTransmission = TRUE;
  }

  TxConnectionIndex = DCM_TX_CONNECTION_INDEX_FROM_ID(TxConnectionId);

  /* In case the whole amount of data to be transmitted is already present in the
     transmission buffer and response suppression is activated, there is no use in setting
     up a TxConnection. We may immediately confirm the transmission as being successful
     to the Requester. */

  if (DCM_TX_CONNECTION_IDLE != Dcm_TxConnections[TxConnectionIndex].Status)
  {
    /* Change the owner of the transmission buffer back to the Protocol. */
    (void)Dcm_Dsl_BufferGive(TxConnectionId,
                             Requester,
                             Buffer.BufferId);

    Dcm_Dsl_ConfirmTransmissionToRequester(Requester,
                                           DCM_TX_FAILED,
                                           TxConnectionId);
  }
  else if ((TRUE == isSuppressTransmission) && (AmountToTransmit == Buffer.FilledLength))
  {
    /* Change the owner of the transmission buffer back to the Protocol. */
    (void)Dcm_Dsl_BufferGive(TxConnectionId,
                             Requester,
                             Buffer.BufferId);

    Dcm_Dsl_ConfirmTransmissionToRequester(Requester,
                                           DCM_TX_OK,
                                           TxConnectionId);
  }
  else
  {
    /* All other cases require the actual transmission to be
       handled in the context of the TxConnection. Set up the TxConnection */
    Dcm_RxComMChannelIDsIndexType ContextComMChannelIdIndex;
    boolean ComMChannelFound = FALSE;

    /* Retrieve the index of the ComMChannel to monitor for status */
    ContextComMChannelIdIndex = 0;

    while ( (ContextComMChannelIdIndex < DCM_NUM_RX_COMM_CHANNELS) && (FALSE == ComMChannelFound) )
    {
      if (ContextComMChannelId == Dcm_RxComMChannelIDs[ContextComMChannelIdIndex])
      {
        ComMChannelFound = TRUE;
      }
      else
      {
        ContextComMChannelIdIndex++;
      }
    }

    /* ComMChannelId is always valid */
    DCM_PRECONDITION_ASSERT((FALSE != ComMChannelFound), DCM_INTERNAL_API_ID);

    /* TxConnections are indexed using the zero-based and consecutive ConfirmationTxPduId,
       which is always in a 1:1 relationship with the TxPduId. */
    Dcm_Dsl_SetupTxConnection(TxConnectionIndex,
                              ContextComMChannelIdIndex,
                              Requester,
                              isSuppressTransmission,
                              AmountToTransmit,
                              TransmissionTimeout,
                              Buffer
                             );

    if (FALSE == isSuppressTransmission)
    {
       /* This is an unsuppressed transmission. Now that the connection has been set up,
          attempt to start the transmission immediately, and delay if this is unsuccessful
          and if the given TransmissionTimeout allows it. */

       Dcm_TxResultType TransmissionStartStatus;

       TransmissionStartStatus = Dcm_Dsl_TxConnectionStart(TxConnectionIndex);

       if (DCM_TX_DELAYED == TransmissionStartStatus)
       {
         /* Transmission was delayed. In this case, mark that there is a waiting connection
            so that its Timeout counter can be decremented*/
         Dcm_ProcessTxConnectionsOnMainFunction = TRUE;
       }
    }
    else
    {
      /* Suppressed page-buffered transmissions always start successfully. On each and every
         Dcm_MainFunction() call a new buffer shall be requested from the Dsp and discarded. */
      Dcm_TxConnections[TxConnectionIndex].Status = DCM_TX_CONNECTION_REQUEST_DATA;

      Dcm_ProcessTxConnectionsOnMainFunction = TRUE;
    }
  }

  DBG_DCM_DSL_TXCONNECTIONTRANSMIT_EXIT(TxConnectionId, ContextComMChannelId, Requester,
    SuppressTransmission, AmountToTransmit, TransmissionTimeout, Buffer);
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.ProcessTxConnections,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_TxConnectionProcessTxConnections(
  void
)
{
  uint16_least TxConnectionIndex;
  boolean WaitingConnectionsPresent = FALSE;

  DBG_DCM_DSL_TXCONNECTIONPROCESSTXCONNECTIONS_ENTRY();

  if (TRUE == Dcm_ProcessTxConnectionsOnMainFunction)
  {
    /* Reset processing flag. If at the end of this function we notice that there are connections
       which still need actions, we set it back to true. This prevents concurrency issues when
       Dcm_CopyTxData() sets this flag to TRUE this function sets it to FALSE. */
    Dcm_ProcessTxConnectionsOnMainFunction = FALSE;

 /* Deviation TASKING-1 */
    for (TxConnectionIndex = 0; TxConnectionIndex < DCM_NUM_TX_CONNECTIONS; TxConnectionIndex++)
    {
      /* Deviation MISRAC2012-1 */
      switch (Dcm_TxConnections[TxConnectionIndex].Status)
      {
        case DCM_TX_CONNECTION_WAITING_FOR_CHANNEL:
          {
            Dcm_TxResultType TransmissionStartStatus;

            /* Re-attempt to start the TxConnection if it is still waiting for the ComMChannel
               to enter FullCommunicationMode*/

            TransmissionStartStatus = Dcm_Dsl_TxConnectionStart(
                (Dcm_TxConnectionIndexType)TxConnectionIndex);

            if (DCM_TX_DELAYED == TransmissionStartStatus)
            {
              WaitingConnectionsPresent = TRUE;
            }
          }
          break;

        case DCM_TX_CONNECTION_WAITING_FOR_DATA:

          DBG_DCM_DSLINTERNAL_CONNECTIONWAITINGFORDATA_ENTRY();

          /* Access to this TxConnection's entry in the Dcm_TxConnections[] table needs
             to be protected since a preemption of the Dcm_MainFunction() by one of the
             Dcm's transmission APIs can occur.*/

          /* ENTER critical section */
          SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

          if (DCM_TX_CONNECTION_WAITING_FOR_DATA == Dcm_TxConnections[TxConnectionIndex].Status)
          {
            /* Do nothing in case this TxConnection has already been terminated by the lower layer.
               This is in case the current function is preempted by a call from the lower layer
               taking the TxConnection to another state between the moment of checking the value
               of the TxConnection state and entering the critical section. */

            if (0U == Dcm_TxConnections[TxConnectionIndex].TransmissionTimeout)
            {
              /* This transmission has timed out waiting for data from the Dsp.
                 Cancel this transmission, but only if the lower layer hasn't terminated
                 it yet. */
              PduIdType CurrentTxPduId;

              CurrentTxPduId = Dcm_Dsl_GetTxPduIdFromTxConnectionIndex(
                  (Dcm_TxConnectionIndexType)TxConnectionIndex);

              Dcm_TxConnections[TxConnectionIndex].Status = DCM_TX_CONNECTION_TIMED_OUT;

               /* Give the transmission buffer back to the Requester. */
              (void)Dcm_Dsl_BufferGive(DCM_TX_CONNECTION_ID_FROM_INDEX(TxConnectionIndex),
                                       Dcm_TxConnections[TxConnectionIndex].Requester,
                                       Dcm_TxConnections[TxConnectionIndex].Buffer.BufferId);
              /* Notify the Requester that the transmission has timed out. */
              Dcm_Dsl_NotifyTransmissionTimeoutToRequester(
                  Dcm_TxConnections[TxConnectionIndex].Requester,
                  CurrentTxPduId);
            }
            else
            {
              /* Decrement the delay timeout if the connection is waiting for the next page of data */
              Dcm_TxConnections[TxConnectionIndex].TransmissionTimeout--;

              /* Waiting connections still are present */
              WaitingConnectionsPresent = TRUE;
            }
          }

          /* EXIT critical section */
          SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

          DBG_DCM_DSLINTERNAL_CONNECTIONWAITINGFORDATA_EXIT();

          break;

        case DCM_TX_CONNECTION_REQUEST_DATA:
          {
            /* In the context a suppressed Page-buffered transmission, simply discard
               the data from the buffer */

            /* Note: protection of access to this TxConnection's Dcm_TxConnections[] entry
                     is not needed as the lower layer hasn't been invoked. Therefore, no
                     preemption by the Dcm's transmission APIs can take place in the context
                     of this TxConnection. */

            Dcm_TxConnections[TxConnectionIndex].AmountLeftToCopy -=
              (PduLengthType)(Dcm_TxConnections[TxConnectionIndex].Buffer.FilledLength);

            /* Give the transmission buffer back to the Requester */
            (void)Dcm_Dsl_BufferGive(DCM_TX_CONNECTION_ID_FROM_INDEX(TxConnectionIndex),
                                     Dcm_TxConnections[TxConnectionIndex].Requester,
                                     Dcm_TxConnections[TxConnectionIndex].Buffer.BufferId);

            if (0U == Dcm_TxConnections[TxConnectionIndex].AmountLeftToCopy)
            {
              /* If this is the last page of data, a confirmation
                 of the 'transmission' may be sent to the Requester */

              Dcm_Dsl_ConfirmTransmissionToRequester(
                  Dcm_TxConnections[TxConnectionIndex].Requester,
                  DCM_TX_OK,
                  DCM_TX_CONNECTION_ID_FROM_INDEX(TxConnectionIndex));

              /* Reinitialize the TxConnection */
              Dcm_Dsl_TxConnectionInitSingleTxConnection(
                (Dcm_TxConnectionIndexType)TxConnectionIndex);
            }
            else
            {
              /* Request the next page of data */
              Dcm_Dsl_ProvideNextTxBuffer(
                  Dcm_TxConnections[TxConnectionIndex].Requester,
                  DCM_TX_CONNECTION_ID_FROM_INDEX(TxConnectionIndex));

                Dcm_TxConnections[TxConnectionIndex].Status = DCM_TX_CONNECTION_WAITING_FOR_DATA;
                Dcm_TxConnections[TxConnectionIndex].TransmissionTimeout = DCM_PAGED_BUFFER_TIMEOUT;

                /* Waiting connections still are present */
                WaitingConnectionsPresent = TRUE;
              }
            }
          break;

        /* Deviation MISRAC2012-2 */
        default:
          break;
      }
    }
  }

  if (TRUE == WaitingConnectionsPresent)
  {
    Dcm_ProcessTxConnectionsOnMainFunction = TRUE;
  }

  DBG_DCM_DSL_TXCONNECTIONPROCESSTXCONNECTIONS_EXIT();
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.Give,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_TxConnectionGive(
  Dcm_BufferUserType Provider,
  Dcm_BufferUserType Receiver,
  Dcm_BufferUserType TxConnectionId
)
{

  Std_ReturnType OwnerTransferResult = E_NOT_OK;
  Dcm_TxConnectionIndexType TxConnectionIndex;

  TxConnectionIndex = DCM_TX_CONNECTION_INDEX_FROM_ID(TxConnectionId);

  DBG_DCM_DSL_BUFFERGIVE_ENTRY(Provider, Receiver, BufferId);
  /* Only the current user of a TxConnection may change its owner. */

  if (Provider == Dcm_TxConnections[TxConnectionIndex].Requester)
  {
    Dcm_TxConnections[TxConnectionIndex].Requester = Receiver;

    OwnerTransferResult = E_OK;
  }
  else
  {
    OwnerTransferResult = E_NOT_OK;
  }

  return OwnerTransferResult;
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.ProcessNextTxBuffer,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_TxConnectionProcessNextTxBuffer(
  Dcm_BufferUserType TxConnectionId,
  Dcm_BufferType Buffer
)
{
  Dcm_TxConnectionIndexType TxConnectionIndex = DCM_TX_CONNECTION_INDEX_FROM_ID(TxConnectionId);

  DBG_DCM_DSL_TXCONNECTIONPROCESSNEXTTXBUFFER_ENTRY(TxConnectionId, Buffer);

  if (TRUE == Dcm_TxConnections[TxConnectionIndex].SimulateTransmission)
  {
    /* in case this is a simulated (suppressed) transmission, in case there is
       still data to fetch from the Dsp, give the buffer back to the Dsp in the
       next Dcm_MainFunction() cycle and request the remaining data. */

    /* The connection must once more request data */
    Dcm_TxConnections[TxConnectionIndex].Status = DCM_TX_CONNECTION_REQUEST_DATA;
    Dcm_TxConnections[TxConnectionIndex].Buffer.FilledLength = Buffer.FilledLength;
    Dcm_TxConnections[TxConnectionIndex].Buffer.BufferId = Buffer.BufferId;

    /* The datamarks and the pointer to the buffer data are not used, since no data
       is really copied.  */

    Dcm_ProcessTxConnectionsOnMainFunction = TRUE;
  }
  else
  {
    /* In this case, the consistency of the item in Dcm_TxConnections array describing
       this TxConnection has to be protected against concurrent access from the lower
       layer. */

    /* ENTER critical section */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    if (DCM_TX_CONNECTION_WAITING_FOR_DATA == Dcm_TxConnections[TxConnectionIndex].Status)
    {
      /* The state of the TxBuffer is changed to reflect a transmission in progress.
         Since the buffer now belongs to the connection, it is allowed to change its state. */
      (void)Dcm_Dsl_BufferChangeState(DCM_TX_CONNECTION_ID_FROM_INDEX(TxConnectionIndex),
                                    DCM_BUFFER_TX,
                                    Buffer.BufferId);

      /* The connection is once more transmitting data */
      Dcm_TxConnections[TxConnectionIndex].Status = DCM_TX_CONNECTION_TRANSMITTING;
      Dcm_TxConnections[TxConnectionIndex].Buffer.FilledLength = Buffer.FilledLength;
      Dcm_TxConnections[TxConnectionIndex].Buffer.BufferId = Buffer.BufferId;
      Dcm_TxConnections[TxConnectionIndex].Buffer.BufferPtr = Buffer.BufferPtr;

      /* Reset the datamarks as this buffer is now beginning to be copied. */
      Dcm_TxConnections[TxConnectionIndex].BufferDataMark = 0U;
      Dcm_TxConnections[TxConnectionIndex].BufferConfirmedDataMark = 0U;
    }
    else
    {
      if (DCM_TX_CONNECTION_IDLE == Dcm_TxConnections[TxConnectionIndex].Status)
      {
        /* There is a chance that the TxConnection has already been terminated
           by the lower layer. In this case, this TxConnection has received the
           Dcm_TpTxConfirmation() call, but still owns the TxBuffer. Thus, the
           TxConnection must relinquish the buffer immediately so that the Protocol
           may free it. */

        (void)Dcm_Dsl_BufferGive(DCM_TX_CONNECTION_ID_FROM_INDEX(TxConnectionIndex),
                                 Dcm_TxConnections[TxConnectionIndex].Requester,
                                 Dcm_TxConnections[TxConnectionIndex].Buffer.BufferId);
      }
    }

    /* Release the lock for this TxConnection's data. */

    /* EXIT critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
  }
  DBG_DCM_DSL_TXCONNECTIONPROCESSNEXTTXBUFFER_EXIT(TxConnectionId, Buffer);
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.GetTxPduIdFromTxConnectionIndex,1 */
STATIC FUNC(PduIdType, DCM_CODE) Dcm_Dsl_GetTxPduIdFromTxConnectionIndex(
  Dcm_TxConnectionIndexType TxConnectionIndex
)
{
#if ((DCM_PRECONDITION_ASSERT_ENABLED == STD_ON) || \
     ((STD_ON == DCM_DSP_USE_ROESERVICES) && (DCM_NUM_ROE_CONNECTIONS > 0U)) || \
     ((STD_ON == DCM_PERIODIC_USED) && (DCM_NUM_PERIODIC_CONNECTIONS > 0U)))
  boolean TxPduIdFound = FALSE;
#endif
  uint16_least ConnectionIndex;

  PduIdType TxPduId = 0;

 /* Deviation TASKING-1 */
  for (ConnectionIndex = 0 ; ConnectionIndex < DCM_NUM_MAIN_CONNECTIONS; ConnectionIndex++)
  {
    if (Dcm_MainConnectionConfig[ConnectionIndex].ConfirmationTxPduId == TxConnectionIndex)
    {
      TxPduId = Dcm_MainConnectionConfig[ConnectionIndex].TxPduId;
#if (DCM_PRECONDITION_ASSERT_ENABLED == STD_ON)
      TxPduIdFound = TRUE;
#endif
      break;
    }
  }

#if ((STD_ON == DCM_DSP_USE_ROESERVICES) && (DCM_NUM_ROE_CONNECTIONS > 0U))
 /* Deviation TASKING-1 */
  if (FALSE == TxPduIdFound)
  {
    /* Deviation TASKING-1 */
    for (ConnectionIndex = 0 ; ConnectionIndex < DCM_NUM_ROE_CONNECTIONS; ConnectionIndex++)
    {
      if (Dcm_RoeConnectionConfig[ConnectionIndex].ConfirmationTxPduId == TxConnectionIndex)
      {
        TxPduId = Dcm_RoeConnectionConfig[ConnectionIndex].TxPduId;
#if (DCM_PRECONDITION_ASSERT_ENABLED == STD_ON)
        TxPduIdFound = TRUE;
#endif
        break;
      }
    }
  }
#endif /* #if ((STD_ON == DCM_DSP_USE_ROESERVICES) && (DCM_NUM_ROE_CONNECTIONS > 0U)) */

#if ((STD_ON == DCM_PERIODIC_USED) && (DCM_NUM_PERIODIC_CONNECTIONS > 0U))
  /* Deviation TASKING-1 */
  if (FALSE == TxPduIdFound)
  {
    uint16_least TransmissionIndex;

    /* Deviation TASKING-1 <+2> */
    for (TransmissionIndex = 0;
        TransmissionIndex < DCM_NUM_PERIODIC_TRANSMISSIONS;
        TransmissionIndex++)
    {
      if (0 != Dcm_PeriodicTransmissionConfig[TransmissionIndex].NumberOfPeriodicConnections)
      {
        const Dcm_PeriodicConnectionIndexType LastPeriodicConnectionIndex =
          Dcm_PeriodicTransmissionConfig[TransmissionIndex].IndexOfFirstPeriodicConnection +
          Dcm_PeriodicTransmissionConfig[TransmissionIndex].NumberOfPeriodicConnections;

        for (ConnectionIndex = Dcm_PeriodicTransmissionConfig[TransmissionIndex].
                  IndexOfFirstPeriodicConnection;
             ConnectionIndex < LastPeriodicConnectionIndex;
             ConnectionIndex++)
        {
          if (Dcm_PeriodicConnectionConfig[ConnectionIndex].
                ConfirmationTxPduId == TxConnectionIndex)
          {
            TxPduId = Dcm_PeriodicConnectionConfig[ConnectionIndex].TxPduId;
            TxPduIdFound = TRUE;
            break;
          }
        }

        if (TRUE == TxPduIdFound)
        {
          break;
        }
      }
    }
  }
#endif /* #if ((STD_ON == DCM_PERIODIC_USED) && (DCM_NUM_PERIODIC_CONNECTIONS > 0U)) */

  DCM_PRECONDITION_ASSERT((TxPduIdFound != FALSE), DCM_SVCID_COPYTXDATA);

  return TxPduId;
}

#if (DCM_META_DATA_HANDLING_ENABLED == STD_ON)
STATIC FUNC(PduIdType, DCM_CODE) Dcm_Dsl_GetGenericConnIdFromTxConnectionIndex(
  Dcm_TxConnectionIndexType TxConnectionIndex,
  P2VAR(boolean, AUTOMATIC, DCM_VAR) GenericConnIdFound,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) EcuCPduId
)
{
  uint16_least ConnectionIndex;

  PduIdType GenericConnId = 0;
  for (ConnectionIndex = 0 ; ConnectionIndex < DCM_NUM_MAIN_CONNECTIONS; ConnectionIndex++)
  {
    if (TRUE == Dcm_MainConnectionConfig[ConnectionIndex].MetaDataIsConfigured)
    {
      if (Dcm_MainConnectionConfig[ConnectionIndex].ConfirmationTxPduId == TxConnectionIndex)
      {
        GenericConnId = Dcm_MainConnectionConfig[ConnectionIndex].GenericConnectionIndex;
        *EcuCPduId = Dcm_MainConnectionConfig[ConnectionIndex].TxEcuCPduId;
        *GenericConnIdFound = TRUE;
        break;
      }
      else
      {
#if ((STD_ON == DCM_PERIODIC_USED) && (DCM_NUM_PERIODIC_CONNECTIONS > 0U))
        Dcm_PeriodicTransmissionIndexType PeriodicTransmissionId;
#endif /* #if ((STD_ON == DCM_PERIODIC_USED) && (DCM_NUM_PERIODIC_CONNECTIONS > 0U)) */
#if ((STD_ON == DCM_DSP_USE_ROESERVICES) && (DCM_NUM_ROE_CONNECTIONS > 0U))
        Dcm_RoeConnectionIndexType RoeConnId =
            Dcm_MainConnectionConfig[ConnectionIndex].RoeConnectionIndex;
        if ( (DCM_NUM_ROE_CONNECTIONS > RoeConnId) &&
             (Dcm_RoeConnectionConfig[RoeConnId].ConfirmationTxPduId == TxConnectionIndex) )
        {
          GenericConnId = Dcm_RoeConnectionConfig[RoeConnId].GenericConnectionIndex;
          *EcuCPduId = Dcm_MainConnectionConfig[ConnectionIndex].TxEcuCPduId;
          *GenericConnIdFound = TRUE;
          break;
        }
#endif /* #if ((STD_ON == DCM_DSP_USE_ROESERVICES) && (DCM_NUM_ROE_CONNECTIONS > 0U)) */
#if ((STD_ON == DCM_PERIODIC_USED) && (DCM_NUM_PERIODIC_CONNECTIONS > 0U))
        PeriodicTransmissionId = Dcm_MainConnectionConfig[ConnectionIndex].PeriodicTransmissionIndex;
        if ( (DCM_INVALID_PERIODIC_TRANSMISSION > PeriodicTransmissionId) &&
             (0 != Dcm_PeriodicTransmissionConfig[PeriodicTransmissionId].NumberOfPeriodicConnections) )
        {

          const Dcm_PeriodicTransmissionIndexType FirstPeriodicIdx =
              Dcm_PeriodicTransmissionConfig[PeriodicTransmissionId].IndexOfFirstPeriodicConnection;
          const Dcm_PeriodicConnectionIndexType LastPeriodicConnIdx =
                        FirstPeriodicIdx + Dcm_PeriodicTransmissionConfig[PeriodicTransmissionId].NumberOfPeriodicConnections;
          Dcm_PeriodicConnectionIndexType PerTransIdx;
          /* Deviation TASKING-1 */
          for (PerTransIdx = FirstPeriodicIdx; PerTransIdx < LastPeriodicConnIdx; PerTransIdx++)
          {
            if ( (TRUE == Dcm_MainConnectionConfig[ConnectionIndex].MetaDataIsConfigured) &&
                (TxConnectionIndex == Dcm_PeriodicConnectionConfig[PerTransIdx].ConfirmationTxPduId) )
            {
              GenericConnId = Dcm_PeriodicConnectionConfig[PerTransIdx].GenericConnectionIndex;
              *EcuCPduId = Dcm_MainConnectionConfig[ConnectionIndex].TxEcuCPduId;
              *GenericConnIdFound = TRUE;
              break;
            }

          }
        }
#endif /* #if ((STD_ON == DCM_PERIODIC_USED) && (DCM_NUM_PERIODIC_CONNECTIONS > 0U)) */
      }
    }
  }

  return GenericConnId;
}
#endif /* #if (DCM_META_DATA_HANDLING_ENABLED == STD_ON) */

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.IsTYPE2PeriodicTxConnection,1 */
#if ((STD_ON == DCM_PERIODIC_USED) && (DCM_NUM_PERIODIC_CONNECTIONS > 0U))
FUNC(boolean, DCM_CODE) Dcm_Dsl_IsTYPE2PeriodicTxConnection(
  Dcm_BufferUserType TxConnectionId)
{
  boolean TxConnectionIsPeriodic = FALSE;

  uint16_least TransmissionIndex;

  DBG_DCM_DSL_ISTYPE2PERIODICTXCONNECTION_ENTRY(TxConnectionId);

  /* Deviation TASKING-1 <+2> */
  for (TransmissionIndex = 0;
      TransmissionIndex < DCM_NUM_PERIODIC_TRANSMISSIONS;
      TransmissionIndex++)
  {
    if (0 != Dcm_PeriodicTransmissionConfig[TransmissionIndex].NumberOfPeriodicConnections)
    {
      uint16_least ConnectionIndex;
      const Dcm_PeriodicConnectionIndexType LastPeriodicConnectionIndex =
        Dcm_PeriodicTransmissionConfig[TransmissionIndex].IndexOfFirstPeriodicConnection +
        Dcm_PeriodicTransmissionConfig[TransmissionIndex].NumberOfPeriodicConnections;

      for (ConnectionIndex = Dcm_PeriodicTransmissionConfig[TransmissionIndex].
                IndexOfFirstPeriodicConnection;
           ConnectionIndex < LastPeriodicConnectionIndex;
           ConnectionIndex++)
      {
        if (Dcm_PeriodicConnectionConfig[ConnectionIndex].ConfirmationTxPduId ==
            DCM_TX_CONNECTION_INDEX_FROM_ID(TxConnectionId))
        {
          TxConnectionIsPeriodic = TRUE;

          /* stop searching */
          break;
        }
      }

      /* check if the connection has been found */
      if (TRUE == TxConnectionIsPeriodic)
      {
        /* stop searching */
        break;
      }
    }
  }

  DBG_DCM_DSL_ISTYPE2PERIODICTXCONNECTION_EXIT(TxConnectionId, TxConnectionIsPeriodic);

  return TxConnectionIsPeriodic;
}
#endif /* #if ((STD_ON == DCM_PERIODIC_USED) && (DCM_NUM_PERIODIC_CONNECTIONS > 0U)) */

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.SetupTxConnection,1 */
/* Initializes a TxConnection state machine */
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_SetupTxConnection(
  Dcm_TxConnectionIndexType TxConnectionIndex,
  Dcm_RxComMChannelIDsIndexType ContextComMChannelIdIndex,
  Dcm_BufferUserType Requester,
  boolean SimulateTransmission,
  Dcm_MsgLenType AmountToTransmit,
  Dcm_TransmissionTimeoutType TransmissionTimeout,
  Dcm_BufferType Buffer
)
{
  /* The state of the TxBuffer is changed to reflect a transmission in progress.
     Since the buffer now belongs to the connection, it is allowed to change its state. */
  (void)Dcm_Dsl_BufferChangeState(DCM_TX_CONNECTION_ID_FROM_INDEX(TxConnectionIndex),
                                  DCM_BUFFER_TX,
                                  Buffer.BufferId);

  Dcm_TxConnections[TxConnectionIndex].Buffer.BufferId = Buffer.BufferId;
  Dcm_TxConnections[TxConnectionIndex].Buffer.BufferPtr = Buffer.BufferPtr;
  Dcm_TxConnections[TxConnectionIndex].Buffer.FilledLength = Buffer.FilledLength;

  /* When transmission of the data in the buffer is complete, the Requester
     shall be notified. */
  Dcm_TxConnections[TxConnectionIndex].Requester = Requester;

  /* Can be different from the amount of data in the buffer in case of Page-buffered
     transmission. */
  Dcm_TxConnections[TxConnectionIndex].AmountLeftToCopy = (PduLengthType)(AmountToTransmit);

  Dcm_TxConnections[TxConnectionIndex].BufferDataMark = 0U;

  Dcm_TxConnections[TxConnectionIndex].BufferConfirmedDataMark = 0U;

  Dcm_TxConnections[TxConnectionIndex].Status = DCM_TX_CONNECTION_WAITING_FOR_CHANNEL;

  /* If TRUE, the TxConnection will simulate transmission, with the lower layer not
     being invoked. The state of the ComM channel is also ignored. */
  Dcm_TxConnections[TxConnectionIndex].SimulateTransmission = SimulateTransmission;

  /* Should the transmission be unsuccessful within this time frame, the waiting Requester
     is notified that the transmission of the data has failed. A value of 0 signifies that
     immediate transmission is necessary.

     Note: After the timeout, the connection will be:
           - Reset to IDLE if the PduR_DcmTransmit() API has not been called yet as a result
             of the ComM channel not being in Full Communication Mode
           - Left in a waiting state if the PduR_DcmTransmit() API has been called, as the
             Dcm_(Tp)TxConfirmation() API also needs to be called in order to end the
             transmission session with the lower layer. This functionality is only
             provided for page-buffered transmission. In this case, this timeout
             represents the delay allowed between the moment when a new buffer containing data
             is requested from the Requester and the moment the TxConnection receives
             the new buffer. */

  /* !LINKSTO SWS_Dcm_01142,1 */
  Dcm_TxConnections[TxConnectionIndex].TransmissionTimeout = TransmissionTimeout;

  /* The index of the ComM ChannelId which will be monitored for FullCommunicationMode before
     transmission commences. */
  Dcm_TxConnections[TxConnectionIndex].ContextComMChannelIdIndex = ContextComMChannelIdIndex;

  Dcm_TxConnections[TxConnectionIndex].TxPduInfo.SduLength = (PduLengthType)(AmountToTransmit);

#if ((STD_ON == DCM_PERIODIC_USED) && (DCM_NUM_PERIODIC_CONNECTIONS > 0U))
  if (TRUE == Dcm_Dsl_IsTYPE2PeriodicTxConnection(DCM_TX_CONNECTION_ID_FROM_INDEX(TxConnectionIndex)))
  {
    /* !LINKSTO SWS_Dcm_01101_TransmissionDataPointerValid,1 */
    Dcm_TxConnections[TxConnectionIndex].TxPduInfo.SduDataPtr = Dcm_TxConnections[
        TxConnectionIndex].Buffer.BufferPtr;
  }
  else
  {
    Dcm_TxConnections[TxConnectionIndex].TxPduInfo.SduDataPtr = NULL_PTR;
  }
#endif /* #if ((STD_ON == DCM_PERIODIC_USED) && (DCM_NUM_PERIODIC_CONNECTIONS > 0U)) */
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.Start,1 */
/* Attempts to start the TxConnection state machine */
STATIC FUNC (Dcm_TxResultType, DCM_CODE) Dcm_Dsl_TxConnectionStart(
  Dcm_TxConnectionIndexType TxConnectionIndex
)
{
  Dcm_TxResultType TransmissionStartStatus = DCM_TX_FAILED;
  NetworkHandleType ComMChannelId =
                Dcm_RxComMChannelIDs[Dcm_TxConnections[TxConnectionIndex].ContextComMChannelIdIndex];

  Dcm_ComMChannelOperationStateType ComMChnTxState = Dcm_Dsl_ComMChannelStateManager_GetTxState(ComMChannelId);

#if (DCM_META_DATA_HANDLING_ENABLED == STD_ON)
  Std_ReturnType SetMetaDataInfoResult;
  boolean GenericConnIdFound = FALSE;
  PduIdType EcuCPduId;
  PduInfoType Info;
  Dcm_GenericConnectionIndexType GenericConnId =
      (uint8)(Dcm_Dsl_GetGenericConnIdFromTxConnectionIndex(TxConnectionIndex, &GenericConnIdFound, &EcuCPduId));
#endif /* #if (DCM_META_DATA_HANDLING_ENABLED == STD_ON) */

  if (DCM_COMM_OPERATION_ENABLED == ComMChnTxState)
  {
    /* If the ComM channel is in FullCommunicationMode, transmission can be initiated. */

    /* The TxPduId is only used once. This can be determined from the configuration */

    PduIdType TxPduId;
    Std_ReturnType PduRTransmitResult;

    TxPduId = Dcm_Dsl_GetTxPduIdFromTxConnectionIndex(TxConnectionIndex);

    /* Consider the TxConnection started for now. In case the PduR rejects this, we will
       reset its status to idle later on. */

    /* Because between the call to the PduR_DcmTransmit() API and this point, the
       Dcm_CopyTxData() API might get called, this temporary setting is needed. */
    Dcm_TxConnections[TxConnectionIndex].Status = DCM_TX_CONNECTION_TRANSMITTING;

#if (DCM_META_DATA_HANDLING_ENABLED == STD_ON)
    if(TRUE == GenericConnIdFound)
    {
      SetMetaDataInfoResult = Dcm_Dsl_SetEcuCMetaDataInfo(EcuCPduId,
                                                          GenericConnId,
                                                          &Info);

      if (E_OK != SetMetaDataInfoResult)
      {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO Dcm.EB.LowerLayerInterfaces.Response.DcmGenericConnectionUsage.SetMetaDataSourceAddr.ReturnsNotE_OK.ReportDETError,1 */
        /* !LINKSTO Dcm.EB.LowerLayerInterfaces.TYPE1ROE.DcmGenericConnectionUsage.SetMetaDataSourceAddr.ReturnsNotE_OK.ReportDETError,1 */
        /* !LINKSTO Dcm.EB.LowerLayerInterfaces.TYPE2ROE.DcmGenericConnectionUsage.SetMetaDataSourceAddr.ReturnsNotE_OK.ReportDETError,1 */
        /* !LINKSTO Dcm.EB.LowerLayerInterfaces.TYPE1Periodic.DcmGenericConnectionUsage.SetMetaDataSourceAddr.ReturnsNotE_OK.ReportDETError,1 */
        /* !LINKSTO Dcm.EB.LowerLayerInterfaces.TYPE2Periodic.DcmGenericConnectionUsage.SetMetaDataSourceAddr.ReturnsNotE_OK.ReportDETError,1 */
        /* !LINKSTO Dcm.EB.LowerLayerInterfaces.Response.DcmGenericConnectionUsage.SetMetaDataTargetAddr.ReturnsNotE_OK.ReportDETError,1 */
        /* !LINKSTO Dcm.EB.LowerLayerInterfaces.TYPE1ROE.DcmGenericConnectionUsage.SetMetaDataTargetAddr.ReturnsNotE_OK.ReportDETError,1 */
        /* !LINKSTO Dcm.EB.LowerLayerInterfaces.TYPE2ROE.DcmGenericConnectionUsage.SetMetaDataTargetAddr.ReturnsNotE_OK.ReportDETError,1 */
        /* !LINKSTO Dcm.EB.LowerLayerInterfaces.TYPE1Periodic.DcmGenericConnectionUsage.SetMetaDataTargetAddr.ReturnsNotE_OK.ReportDETError,1 */
        /* !LINKSTO Dcm.EB.LowerLayerInterfaces.TYPE2Periodic.DcmGenericConnectionUsage.SetMetaDataTargetAddr.ReturnsNotE_OK.ReportDETError,1 */
        DCM_DET_REPORT_ERROR(DCM_INTERNAL_API_ID, DCM_E_INTERFACE_RETURN_VALUE);
#endif
      }
    }
#endif /* #if (DCM_META_DATA_HANDLING_ENABLED == STD_ON) */

    /* Elektrobit's PduR has a deviation that requires using an own API
     * function when sending UUDT messages directly to the If layer */
    /* !LINKSTO SWS_Dcm_01101_TransmissionThroughPduR,1 */
#if ((DCM_USE_ELEKTROBIT_SUPPLIED_PDUR == STD_ON) && \
    ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U)))
    if(TRUE ==
       Dcm_Dsl_IsTYPE2PeriodicTxConnection(DCM_TX_CONNECTION_ID_FROM_INDEX(TxConnectionIndex)))
    {
      PduRTransmitResult = PduR_DcmIfTransmit(TxPduId, &(Dcm_TxConnections[TxConnectionIndex]
                           .TxPduInfo));
    }
    else
#endif /* #if ((DCM_USE_ELEKTROBIT_SUPPLIED_PDUR == STD_ON) &&
    ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U))) */
    {
      /* !LINKSTO Dcm.LowerLayerInterfaces.PduRInterface.Dcm_TpRxIndication.TriggerTransmission,1 */

      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForBRRTransmission.NotConcurentTesterPresent.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.RequestedProtocolBusyTYPE2Periodic.BRREnabled.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.OBDRequestOBDAlreadyRunningOnDifferentStack.BRREnabled.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.UDSRequestUDSAlreadyRunningOnDifferentStack.BRREnabled.TransmitBRR,1 */

      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.WarmStart.AcceptedForProcessing.NoCurrentProtocol.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */

      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StopProtocol.ReturnsNotE_OK.TransmitNRC,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StopProtocol.ReturnsNotE_OK.TransmitNRC,1 */

      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */

      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.ReturnsNotE_OK.TransmitNRC,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StopProtocol.ReturnsNotE_OK.TransmitNRC,1 */

      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */

      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StopProtocol.ReturnsNotE_OK.TransmitNRC,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StopProtocol.ReturnsNotE_OK.TransmitNRC,1 */

      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.StartProtocol.ReturnsNotE_OK.TransmitNRC,1 */


      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NoCurrentProtocol.PreemptionTimeout.BRREnabled.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NoCurrentProtocol.PreemptionTimeout.BRRDisabled.RCRRPSent.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.OnCurrentProtocol.PreemptionTimeout.BRREnabled.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.OnCurrentProtocol.PreemptionTimeout.BRRDisabled.RCRRPSent.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.PreemptionTimeout.BRREnabled.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.PreemptionTimeout.BRRDisabled.RCRRPSent.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.PreemptionTimeout.BRREnabled.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.PreemptionTimeout.BRRDisabled.RCRRPSent.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.PreemptionTimeout.BRREnabled.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.RxIndication.ResultNTFRSLT_OK.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.PreemptionTimeout.BRRDisabled.RCRRPSent.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NoCurrentProtocol.PreemptionTimeout.BRREnabled.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NoCurrentProtocol.PreemptionTimeout.BRRDisabled.RCRRPSent.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.OnCurrentProtocol.PreemptionTimeout.BRREnabled.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.OnCurrentProtocol.PreemptionTimeout.BRRDisabled.RCRRPSent.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.PreemptionTimeout.BRREnabled.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.CurrentProtocolIdle.DefaultSession.PreemptionTimeout.BRRDisabled.RCRRPSent.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.PreemptionTimeout.BRREnabled.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolIdle.PreemptionTimeout.BRRDisabled.RCRRPSent.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.PreemptionTimeout.BRREnabled.TransmitBRR,1 */
      /* !LINKSTO Dcm.EB.DiagnosticProtocolHandling.ApplicationRequest.Dispatch.AcceptedForProcessing.NotOnCurrentProtocol.RequestedProtocolHigherPrio.CurrentProtocolBusy.PreemptionTimeout.BRRDisabled.RCRRPSent.TransmitBRR,1 */

      PduRTransmitResult = PduR_DcmTransmit(TxPduId, &(Dcm_TxConnections[
          TxConnectionIndex].TxPduInfo));
    }

    if (E_OK != PduRTransmitResult)
    {
      TransmissionStartStatus = DCM_TX_FAILED;

       /* Note: Protection against concurrent access to this TxConnection's entry
                in the Dcm_TxConnections[] table is not necessary since the lower
                layer has rejected the transmission request. Therefore, no preemption
                by the Dcm's transmission APIs can take place in the context of this
                TxConnection. */

       /* Give the transmission buffer back to the Requester. */
      (void)Dcm_Dsl_BufferGive(DCM_TX_CONNECTION_ID_FROM_INDEX(TxConnectionIndex),
                               Dcm_TxConnections[TxConnectionIndex].Requester,
                               Dcm_TxConnections[TxConnectionIndex].Buffer.BufferId);

      /* Immediately notify the Requester that the transmission is over. */
      Dcm_Dsl_ConfirmTransmissionToRequester(Dcm_TxConnections[TxConnectionIndex].Requester,
                                             DCM_TX_FAILED,
                                             DCM_TX_CONNECTION_ID_FROM_INDEX(TxConnectionIndex));

      /* Reinitialize the TxConnection */
      Dcm_Dsl_TxConnectionInitSingleTxConnection(TxConnectionIndex);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
      if (E_NOT_OK != PduRTransmitResult)
      {
        DCM_DET_REPORT_ERROR(DCM_SVCID_PDUR_DCMTRANSMIT, DCM_E_INTERFACE_RETURN_VALUE);
      }
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
    }
    else
    {
      TransmissionStartStatus = DCM_TX_OK;
    }
  }
  else
  {
    if (0U == Dcm_TxConnections[TxConnectionIndex].TransmissionTimeout)
    {
      /* Note: Protection against concurrent access to this TxConnection's entry
               in the Dcm_TxConnections[] table is not necessary since the lower
               layer hasn't been invoked yet. Therefore, no preemption by the Dcm's
               transmission APIs can take place in the context of this TxConnection. */

      /* !LINKSTO Dcm.Dsl.TxConnection.TxDisalbed.TimeoutWaitingForChannel,1 */
      TransmissionStartStatus = DCM_TX_FAILED;

       /* Give the transmission buffer back to the Requester. */
      (void)Dcm_Dsl_BufferGive(DCM_TX_CONNECTION_ID_FROM_INDEX(TxConnectionIndex),
                               Dcm_TxConnections[TxConnectionIndex].Requester,
                               Dcm_TxConnections[TxConnectionIndex].Buffer.BufferId);

      /* Notify the requester of this transmission session that the
         transmission is complete. */
      /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.Supervisor.TimeoutWaitingForChannel,1 */
      /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.Protocol.TimeoutWaitingForChannel,1 */
      Dcm_Dsl_ConfirmTransmissionToRequester(Dcm_TxConnections[TxConnectionIndex].Requester,
                                             DCM_TX_FAILED,
                                             DCM_TX_CONNECTION_ID_FROM_INDEX(TxConnectionIndex));
      /* Reinitialize the TxConnection */
      Dcm_Dsl_TxConnectionInitSingleTxConnection(TxConnectionIndex);
    }
    else
    {
      /* if the channel is not yet up, decrement the transmission timeout */
      Dcm_TxConnections[TxConnectionIndex].TransmissionTimeout--;
      TransmissionStartStatus = DCM_TX_DELAYED;
    }
  }

  return TransmissionStartStatus;
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.ProvideNextTxBuffer,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_ProvideNextTxBuffer(
  Dcm_BufferUserType Requester,
  Dcm_BufferUserType TxConnectionId
)
{
  /* TxConnection takes care not to ask for more data than it should,
   * and the Protocol will always report that it can provide more data */

    /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.Protocol.ProvideNextTxBuffer,1 */
  Dcm_Dsp_ProtocolProvideNextTxBuffer(Requester, TxConnectionId);
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.TxConnection.ConfirmTransmissionToRequester,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_ConfirmTransmissionToRequester(
  Dcm_BufferUserType Requester,
  NotifResultType Result,
  Dcm_BufferUserType TxConnectionId
)
{
  /* !LINKSTO Dcm117,2 */
  /* Only a Protocol or the Supervisor are allowed to transmit data.
     In case a Requester has multiple transmission sessions running,
     the transmission session is identified by the TxConnectionId. */
  if(DCM_USER_IS_PROTOCOL(Requester))
  {
    /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.Protocol.TransmissionFinished,1 */
    Dcm_Dsp_ProtocolTransmissionFinished(Requester, Result, TxConnectionId);
  }
  else
  {
    /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.Supervisor.TransmissionFinished,1 */
    Dcm_Dsl_Supervisor_TransmissionFinished(TxConnectionId, Result);
  }
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.TxConnection.NotifyTransmissionTimeoutToRequester,1 */
STATIC FUNC (void, DCM_CODE) Dcm_Dsl_NotifyTransmissionTimeoutToRequester(
  Dcm_BufferUserType Requester,
  Dcm_BufferUserType TxConnectionId
)
{
  /* Only a Protocol or the Supervisor are allowed to transmit data.
     In case a Requester has multiple transmission sessions running,
     the transmission session is identified by the TxConnectionId. */

  DCM_PRECONDITION_ASSERT(DCM_USER_IS_PROTOCOL(Requester), DCM_INTERNAL_API_ID);

    /* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.Protocol.TimeoutWaitingForBuffer,2 */
    Dcm_Dsp_ProtocolTransmissionTimedOutWaitingForData(Requester, TxConnectionId);
  }

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.Init,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_TxConnectionInit(
  void
)
{
  uint16_least TxConnectionIndex;

  /* Deviation TASKING-1 */
  for (TxConnectionIndex = 0; TxConnectionIndex < DCM_NUM_TX_CONNECTIONS ; TxConnectionIndex++ )
  {
    Dcm_Dsl_TxConnectionInitSingleTxConnection((Dcm_TxConnectionIndexType)TxConnectionIndex);
  }
}

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.InitSingleConnection,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_TxConnectionInitSingleTxConnection(
  Dcm_TxConnectionIndexType TxConnectionIndex)
{
  Dcm_TxConnections[TxConnectionIndex].Buffer.BufferId = DCM_BUFFER_INVALID_ID;
  Dcm_TxConnections[TxConnectionIndex].Buffer.BufferPtr = NULL_PTR;
  Dcm_TxConnections[TxConnectionIndex].Buffer.FilledLength = 0U;
  Dcm_TxConnections[TxConnectionIndex].Requester = DCM_INVALID_USER_ID;
  Dcm_TxConnections[TxConnectionIndex].AmountLeftToCopy = 0U;
  Dcm_TxConnections[TxConnectionIndex].BufferDataMark = 0U;
  Dcm_TxConnections[TxConnectionIndex].BufferConfirmedDataMark = 0U;
  Dcm_TxConnections[TxConnectionIndex].SimulateTransmission = FALSE;
  Dcm_TxConnections[TxConnectionIndex].Status = DCM_TX_CONNECTION_IDLE;
}

FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_ProcessRetryInformation(
  P2VAR(RetryInfoType, AUTOMATIC, DCM_APPL_DATA) RetryInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DCM_VAR) BufferDataMark,
  P2VAR(PduLengthType, AUTOMATIC, DCM_VAR) BufferConfirmedDataMark
)
{
  Std_ReturnType RetryInformationProcessing = E_NOT_OK;

  PduLengthType LocalBufferDataMark = *BufferDataMark;
  PduLengthType LocalBufferConfirmedDataMark = *BufferConfirmedDataMark;

  /* The lower layer has requested data to be copied. */
  if(RetryInfoPtr != NULL_PTR)
  {
    switch (RetryInfoPtr->TpDataState)
    {
      case TP_DATARETRY:
        /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.RetryError,1 */
        /* If RetryInfoPtr->TpDataState is TP_RETRY, and RetryInfoPtr->TxTpDataCnt is larger
         * than the available payload data prior to the current transmitted data mark
         * Dcm_CopyTxData() shall return BUFREQ_E_NOT_OK.
         */
        if(RetryInfoPtr->TxTpDataCnt > (LocalBufferDataMark - LocalBufferConfirmedDataMark))
        {
          RetryInformationProcessing = E_NOT_OK;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
          DCM_DET_REPORT_ERROR(DCM_SVCID_COPYTXDATA, DCM_E_PARAM);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
        }
        else
        {
          /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.Retry,1 */
          /* Rewind the buffer to the point where the retry was requested */
          LocalBufferDataMark -= RetryInfoPtr->TxTpDataCnt;
          /* All data before this location is implicitly considered as confirmed. */
          LocalBufferConfirmedDataMark = LocalBufferDataMark;
          RetryInformationProcessing = E_OK;
        }

        break;

      case TP_DATACONF:
        LocalBufferConfirmedDataMark = LocalBufferDataMark;
        RetryInformationProcessing = E_OK;
        break;

      case TP_CONFPENDING:
        /* The lower layer requests not to confirm any already copied data yet */
        /* !LINKSTO Dcm.EB.LowerLayerInterfaces.PduRInterface.Dcm_CopyTxData.Retry.CONFPENDING,1 */
        RetryInformationProcessing = E_OK;
        break;

      /* CHECK: NOPARSE */
      default:
        /* Unreachable code as this error condition was captured in the preamble
           of the Dcm_CopyTxData() function . This is required by MISRA.*/
        DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_COPYTXDATA);
        break;
      /* CHECK: PARSE */
    }

    *BufferDataMark = LocalBufferDataMark;
    *BufferConfirmedDataMark = LocalBufferConfirmedDataMark;
  }
  else
  {
    /* No retry information to process. All data before this location
       is implicitly considered as confirmed. */
    *BufferConfirmedDataMark = LocalBufferDataMark;
    RetryInformationProcessing = E_OK;
  }

  return RetryInformationProcessing;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*==================[end of file]================================================================*/
