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


/* !LINKSTO Dcm.Dsn.File.RDBPI.Impl,1 */
/* This file contains the implementation of the implementation of the Diagnostic
 * Service ReadDataByPeriodicIdentifier. */

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
 *
 * TASKING-2) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * This warning is a false positive. The false condition (which terminates the loop) is reached.
 * This is proven by the coverage metrics and by the fact that the while loop is not infinite.
 */
/*===============================[includes]======================================================*/

#include <Dcm_Trace.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <TSAutosar.h>
#include <TSMem.h>

#include <Dcm_Dsl_ComMChannelStateManager.h>                /* Dcm Dsl CComMChannelStateManager header
                                                             file */
#include <Dcm_Dsl_Supervisor.h>
#include <Dcm_Dsp_SvcH_ReadDataByPeriodicIdentifier.h>
#include <Dcm_HsmDcm.h>

#if ((DCM_DSP_USE_SERVICE_0X2C == STD_ON) && (DCM_DDDID_CHECK_SOURCES == STD_ON))
#include <Dcm_Dsp_SvcH_DynamicallyDefineDataIdentifier.h>
#endif /* #if ((DCM_DSP_USE_SERVICE_0X2C == STD_ON) && (DCM_DDDID_CHECK_SOURCES == STD_ON)) */

#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)

#if ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U))
#include <Dcm_Dsl_DiagnosticSessionHandler.h>  /* Dcm Dsl Diagnostic Session Handler header file */
#endif /* ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U)) */

#include <SchM_Dcm.h>                                                  /* SchM interface for Dcm */
#include <TSMem.h>                                               /* EB specific memory functions */

/*===========================[macro definitions]=================================================*/

/** \brief Number of cycles + 1 to wait before sampling and sending a periodic DID for the first
 *         time after adding it to the scheduling table */
#if (defined DCM_PDID_DELAY_BEFORE_FIRST_TRANSMISSION)
  #error "DCM_PDID_DELAY_BEFORE_FIRST_TRANSMISSION is already defined"
#endif
#define DCM_PDID_DELAY_BEFORE_FIRST_TRANSMISSION 3U

#if (defined DCM_PDID_TX_CONNECTION_ID_INVALID)
  #error "DCM_PDID_TX_CONNECTION_ID_INVALID is already defined"
#endif
#define DCM_PDID_TX_CONNECTION_ID_INVALID (Dcm_BufferUserType)((DCM_TX_CONNECTION_ID_HIGH) + 1U)

/*===========================[type definitions]=================================================*/

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief  Get the next free slot in the transmission ring
 **
 ** \retval Index of the next free slot in the transmission ring
 **/
STATIC FUNC(uint16, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_GetFreeTransmissionRingSlot(
  void);

/** \brief Service handler for UDS service 0x2A
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 **/
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_SvcStart(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief Validates the PDIDs in a UDS service 0x2A (ReadDataByPeriodicIdentifier) request message
 **
 ** \param[in] RxPduId          PduId on which the request via service 0x2a was received
 ** \param[in] BufferPDIDs      Buffer holding the periodic DIDs (low byte)
 ** \param[in] NumberOfPDIDs    Number of periodic DIDs in that buffer
 **
 ** \retval NRC
 **/
STATIC FUNC(Dcm_NegativeResponseCodeType, DCM_CODE)
      Dcm_Dsp_ReadDataByPeriodicIdentifier_ValidatePDIDs(
  PduIdType RxPduId,
  P2CONST(uint8, AUTOMATIC, DCM_VAR) BufferPDIDs,
  uint8 NumberOfPDIDs);

/** \brief Restarts the timer of a periodically transmitted DID
 **
 ** \param[in] IndexSchedTable  Index of the DID in the periodic scheduler table.
 **
 ** \retval nothing
 **/
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_RestartPeriodicTimer(
  uint8 IndexSchedTable);

/** \brief  Recheck the security, session adn mode rule conditions of all scheduled periodic DIDs
 **
 ** \param none
 **
 ** \retval nothing
 **/
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_ValidateConditions(
  void);

/** \brief Process the counters of scheduled periodic DIDs
 **
 ** \param none
 **
 ** \retval nothing
 **/
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_ProcessTimersAndSamplePDIDs(
  void);

/** \brief Add or a number of given PDID to the periodic scheduler list
 **
 ** \param[in] RxPduId          PduId on which the request via service 0x2a was received
 ** \param[in] BufferPDIDs      Buffer holding the periodic DIDs (low byte)
 ** \param[in] NumberOfPDIDs    Number of periodic DIDs in that buffer
 ** \param[in] TransmissionMode Transmission mode (slow, medium, fast) to which the PDIDs are set
 **
 ** \retval nothing
 **/
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_AddToSchedulerTable(
  PduIdType RxPduId,
  P2CONST(uint8, AUTOMATIC, DCM_VAR) BufferPDIDs,
  uint8 NumberOfPDIDs,
  uint8 TransmissionMode);

/** \brief Validate a ReadDataByPeriodicIdentifier request message
 **
 ** \param[in] MessageBuffer  Buffer holding the bytes of the RDBPI request messasge
 ** \param[in] MessageLength  Number of bytes in the RDBPI request messasge
 **
 **/
STATIC FUNC(Dcm_NegativeResponseCodeType, DCM_CODE)
      Dcm_Dsp_ReadDataByPeriodicIdentifier_ValidateMessage(
  P2CONST(uint8, AUTOMATIC, DCM_VAR) MessageBuffer,
  Dcm_MsgLenType MessageLength);

/** \brief Remove dupliate periodic DID from the RDBPI request message
 **
 ** \param[in] BufferPDIDs    Buffer holding the periodic DIDs (low byte)
 ** \param[in] NumberOfPDIDs  Number of periodic DIDs in that buffer
 **
 **/
STATIC FUNC(uint8, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveDuplicates(
  P2VAR(uint8, AUTOMATIC, DCM_VAR) BufferPDIDs,
  uint8 NumberOfPDIDs);

/** \brief Checks the space left in the periodic scheduler table against the number of PDIDs
 **        to be added
 **
 ** \param[in] BufferPDIDs    Buffer holding the periodic DIDs (low byte)
 ** \param[in] NumberOfPDIDs  Number of periodic DIDs in that buffer
 **
 ** \retval NRC
 **/
STATIC FUNC(Dcm_NegativeResponseCodeType, DCM_CODE)
      Dcm_Dsp_ReadDataByPeriodicIdentifier_ValidateScheduler(
  P2CONST(uint8, AUTOMATIC, DCM_VAR) BufferPDIDs,
  uint8 NumberOfPDIDs);

/** \brief Validate a single PDID for session, security level and mode conditions
 **
 ** \param[in]  RxPduId           PduId on which the configuration request via service 0x2a was
                                  received
 ** \param[in]  RequestedPDID     Periodic identifier to be checked
 ** \param[out] Nrc               Appropriate negative response code if E_NOT_OK is returned.
 ** \param[in]  InitialPDIDConfig Is the verification due to the initial configuration of a PDID?
 **
 ** \retval E_OK              Validation OK
 ** \retval E_NOT_OK          Validation NOT OK - NRC holds the code for the reason
 **/
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_ValidateOnePDID(
  PduIdType RxPduId,
  uint16 RequestedPDID,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc,
  boolean InitialPDIDConfig);

#if ( (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
      (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) )
/** \brief Retrieve the next available sample that maches a given RxPduId (main connection)
 **
 ** \param[in] RxPduId        PduId on which the configuration request via service 0x2a was received
 **
 ** \retval Index of the next sample or INVALID
 **/
STATIC FUNC(uint16, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_GetNextSample(
  PduIdType RxPduId);

STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_AllowNewTransmission(void);

#endif /* ( (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
            (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) ) */

/** \brief Remove samples generated by a given PDID from the transmission ring
 **
 ** \param[in] IndexSchedTable  Index of the DID in the periodic scheduler table.
 ** \param[in] Which            Specifies if to remove all or just one sample
 **
 ** \retval None
 **/
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveSample(
  uint8 IndexSchedTable,
  uint8 Which);


#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*=======================[external data declarations]============================================*/

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief Structure holding the elements for periodic DID processing */
STATIC VAR(Dcm_PeriodicDIDContextType, DCM_VAR) Dcm_PeriodicDIDContext;

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*=======================[internal data declarations]============================================*/

#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/** \brief Holds the number of periodic DIDs which, after validation, can be added to the scheduler
  *        table */
STATIC VAR(uint8, DCM_VAR) Dcm_AcceptedPDIDs[DCM_MAX_PERIODIC_DID_READ];
STATIC VAR(uint8, DCM_VAR) Dcm_NumberOfAcceptedPDIDs;

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief Buffer configuration for ReadDid to write the data to */
STATIC VAR(Dcm_DspMsgBufferCfgType, DCM_VAR) PeriodicReadDid_BufferCfg;

/** \brief Structure holding the context of reading one periodic DID */
STATIC VAR(Dcm_ReadDidContextType, DCM_VAR) Dcm_ReadPeriodicDidContext;

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[external function definitions]==============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.IB.RDBPI.SvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  /* Holds the result of the service handler */
  Std_ReturnType ReturnVal = DCM_E_ABORT;

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_SVCH_ENTRY(OpStatus, pMsgContext);

  /* The implementation only supports synchronous operation, so only care about the init state */
  if (OpStatus == DCM_INITIAL)
  {
    ReturnVal = Dcm_Dsp_ReadDataByPeriodicIdentifier_SvcStart(pMsgContext);
  }
  else
  {
    ReturnVal = DCM_E_DONE;
  }

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_SVCH_EXIT(ReturnVal, OpStatus, pMsgContext);

  return ReturnVal;
}

/* !LINKSTO Dcm.Dsn.IB.RDBPI.Transmit,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_Transmit(void)
{
  Dcm_RequestTypeType AcceptAs = DCM_REJECTED_REQUEST;

  /* Get the index of the periodic DID in the scheduler table to which the sample to be send
   * belongs */

  uint8 TxIdx
    = Dcm_PeriodicDIDContext.TransmissionRing[Dcm_PeriodicDIDContext.HeadTransmissionRing]
                                            .IndexInSchedulerTable;
#if ( (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
      (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) )
  const boolean AllowNewTransmission = Dcm_Dsp_ReadDataByPeriodicIdentifier_AllowNewTransmission();
#endif/* #if ( (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
      (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) ) */

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_TRANSMIT_ENTRY();

  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  /* Periodic DID requests, like all requests, come on behalf of the RxConnection which first
   * configured them. In this case, this is the one with the ID
   * Dcm_PeriodicDIDContext.SchedulerTable[TxIdx].RxPduId
   * Therefore we create a buffer entity containing a Periodic DID request
   * and submit it to the RxConnection.*/
  /* RxPermission = Dcm_Dsl_Supervisor_RequestRxPermission(
                                           Dcm_PeriodicDIDContext.SchedulerTable[TxIdx].RxPduId,
                                           2U,
                                           DCM_INTERNAL_PERIODIC); */



   AcceptAs = Dcm_Dsl_Supervisor_RequestRxPermission(
                                           Dcm_PeriodicDIDContext.SchedulerTable[TxIdx].RxPduId,
                                           2U,
                                           DCM_INTERNAL_PERIODIC);

  /* If allocation of the tx resources was successful send the internal periodic transmission
   * request message */
  if (
      (
       (DCM_PERIODIC_TYPE1_REQUEST == AcceptAs) ||
       (DCM_PERIODIC_TYPE2_REQUEST == AcceptAs)
      )
#if ( (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
      (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) )
      &&
      (TRUE == AllowNewTransmission)
#endif/* #if ( (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
      (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) ) */
     )
  {
    Dcm_BufferType PeriodicRequestBuffer;

    /* Announce the Supervisor that a reception has been started so it may take the necessary
     * actions. */
    Dcm_Dsl_Supervisor_IncomingReception(Dcm_PeriodicDIDContext.SchedulerTable[TxIdx].RxPduId,
                                         AcceptAs);

    /* For a Periodic Response request, the request buffer simply contains the index of the DID
     * to respond to.*/
    Dcm_PeriodicRequest[0] = (uint8)(Dcm_PeriodicDIDContext.SchedulerTable[TxIdx].PDID >> 8U);
    Dcm_PeriodicRequest[1] = (uint8)(Dcm_PeriodicDIDContext.SchedulerTable[TxIdx].PDID);

    PeriodicRequestBuffer.BufferPtr = &Dcm_PeriodicRequest[0];
    PeriodicRequestBuffer.FilledLength = 2U;
    PeriodicRequestBuffer.Size = 2U;
    PeriodicRequestBuffer.BufferId = DCM_BUFFER_INVALID_ID;

    /* Indicate reception to the Dcm */
    Dcm_Dsl_RxConnectionRxIndication(Dcm_PeriodicDIDContext.SchedulerTable[TxIdx].RxPduId,
                                     AcceptAs,
                                     PeriodicRequestBuffer);
  }

  /* LEAVE critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_TRANSMIT_EXIT();
}


/* !LINKSTO Dcm.Dsn.IB.RDBPI.PeriodicDidSchedule,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_Schedule(void)
{
  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_SCHEDULE_ENTRY();

  /* Make sure there are scheduled periodic DIDs */
  if (Dcm_PeriodicDIDContext.NumberOfScheduledPDIDs > 0U)
  {
    /* !LINKSTO Dcm.PeriodicDid.Dsn.SchedData,1 */

    /* If diagnostic session or security level have changed, the conditions for each periodic
     * DID must be checked again and if they do no longer permit the DID to be read, the DID
     * must be removed from the scheduler table. In case that featuer is enabled the
     * sources of dynamically defined periodic DIDs must be checked too. */
    /* !LINKSTO SWS_Dcm_01109,1, Dcm.PeriodicTransmission.DID.SecurityCheck,1 */
    if (Dcm_PeriodicDIDContext.ForceReValidateConditions == TRUE)
    {
      /* Reset flag and execute validation. PDIDs that fail the validation will be removed from
       * the scheduler table and all the samples read from those PDIDs will be deleted */
      Dcm_PeriodicDIDContext.ForceReValidateConditions = FALSE;
      Dcm_Dsp_ReadDataByPeriodicIdentifier_ValidateConditions();
    }

    /* Decrement the counters of active periodic DIDs and sample the signals as soon as the timer
     * expires */
    /* !LINKSTO Dcm.PeriodicDid.Dsn.SchedProcess,1 */
    Dcm_Dsp_ReadDataByPeriodicIdentifier_ProcessTimersAndSamplePDIDs();

    /* If there are entries in the transmission ring try to send them */
    /* !LINKSTO SWS_Dcm_01103_WaitForAllPeriodicTxConfirmations,1 */
    if (Dcm_PeriodicDIDContext.NumberOfTransmittableSamples > 0U)
    {
      Dcm_Dsp_ReadDataByPeriodicIdentifier_Transmit();
    }
  }

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_SCHEDULE_EXIT();
}

/* !LINKSTO Dcm.Dsn.IB.RDBPI.PeriodicDidProcess,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_ProcessPDID(uint8 InstIdx)
{
  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_PROCESSPDID_ENTRY(InstIdx);

  /* Go ahead only if there is a periodic DID ready for transmission */
  if (Dcm_PeriodicDIDContext.NumberOfTransmittableSamples > 0U)
  {
    /* Get pointer to current message context */
    CONSTP2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext
      = &(DCM_HSM_INFO(InstIdx).msgContext);

    BufReq_ReturnType TxBufferAllocationResult = BUFREQ_E_NOT_OK;

    /* Get the index of the periodic DID in the scheduler table to which the
     * sample to be send belongs */
    uint8 TxIdx = Dcm_PeriodicDIDContext.TransmissionRing[Dcm_PeriodicDIDContext
                  .HeadTransmissionRing].IndexInSchedulerTable;

    /* ENTER critical section */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    /* Attempt to allocate the TxBuffer. */

    /* For transmissions, the size of the transmission data is unknown yet, therefore the buffer
       allocator shall ignore this parameter for buffers for which the usage is transmission. */
    TxBufferAllocationResult
      = Dcm_Dsl_BufferAllocate(DCM_PROTOCOL_ID_FROM_INDEX(DCM_CUR_INST_ID(InstIdx)),
                               DCM_BUFFER_TX_PERIODIC,
                               DCM_BUFFER_IGNORE_BUFFER_SIZE,
                               &(DCM_HSM_INFO(InstIdx).TxBuffer));

    /* Check if transmission(Tx) buffer has been successfully allocated for periodic response */
    if (BUFREQ_OK != TxBufferAllocationResult)
    {
      (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(InstIdx),
                                   DCM_HSM_DCM_EV_PROC_ABORT);

      Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveSample(TxIdx, DCM_PDID_TRANSRING_REMOVE_FIRST);
    }

    if (BUFREQ_OK == TxBufferAllocationResult)
    {
      Dcm_RxPduIdInfoType RxPduIdInfo;

      Dcm_ComMChannelOperationStateType ComMChnTxState = DCM_COMM_OPERATION_DISABLED;
      Dcm_MsgLenType TempLength = 0U;

      /* !LINKSTO Dcm.PeriodicDid.Dsn.CancelProcess,1 */
      PduIdType RxPduId = Dcm_PeriodicDIDContext.SchedulerTable[TxIdx].RxPduId;

      /* Assign Tx buffer */
      pMsgContext->resData = DCM_HSM_INFO(InstIdx).TxBuffer.BufferPtr;

      /* Copying DID sample data from the transmission ring item's sampling buffer */
      TS_MemCpy(pMsgContext->resData,
                Dcm_PeriodicDIDContext.TransmissionRing[Dcm_PeriodicDIDContext.HeadTransmissionRing]
                  .SamplingBuffer,
                Dcm_PeriodicDIDContext.TransmissionRing[Dcm_PeriodicDIDContext.HeadTransmissionRing]
                  .Occupied
                );

      /* To make the MISRA checker happy */
      TempLength = Dcm_PeriodicDIDContext.TransmissionRing[Dcm_PeriodicDIDContext
                    .HeadTransmissionRing].Occupied;
      TempLength = TempLength - 1U;
      pMsgContext->resDataLen = TempLength;

      DCM_HSM_INFO(InstIdx).TxBuffer.FilledLength
        = (PduLengthType)(Dcm_PeriodicDIDContext.TransmissionRing[Dcm_PeriodicDIDContext
          .HeadTransmissionRing].Occupied);

      Dcm_Dsl_RxConnectionGetRxPduIdInformation(RxPduId, &RxPduIdInfo);
      /* Check if in full-communication mode */
      /* !LINKSTO Dcm150,1, Dcm155,1 */
      /* !LINKSTO Dcm.EB.PeriodicTransmission.TYPE1.TxPduId,1 */
      ComMChnTxState = Dcm_Dsl_ComMChannelStateManager_GetTxState(RxPduIdInfo.ComMChannelId);
      if (DCM_COMM_OPERATION_ENABLED == ComMChnTxState)
      {
        (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(InstIdx),
                                      DCM_HSM_DCM_EV_PROC_DONE);
      }
      else
      {
        /* No full-communication mode */
        (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(InstIdx),
                                      DCM_HSM_DCM_EV_PROC_ABORT);
      }

      /* !LINKSTO Dcm.PeriodicDid.Dsn.SchedDidDone,1 */
      Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveSample(TxIdx, DCM_PDID_TRANSRING_REMOVE_FIRST);
    }
    else
    {
      (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(InstIdx),
                                    DCM_HSM_DCM_EV_PROC_ABORT);

      Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveSample(TxIdx, DCM_PDID_TRANSRING_REMOVE_FIRST);
    }

    /* LEAVE critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
  }
  else
  {
    TS_PARAM_UNUSED(InstIdx);
  }

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_PROCESSPDID_EXIT();
}

#if ( (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
      (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) )
/* !LINKSTO Dcm.Dsn.IB.RDBPI.ConfirmAndCheckPendingConfirmations,1 */
FUNC(boolean, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_ConfirmAndCheckPendingConfirmations(
  void)
{
  boolean ReturnValue = FALSE;

  /* (Dcm_PeriodicDIDContext.SamplesSentUUDT < 0U) is unreachable. Reasons:
      1) the value of the variable Dcm_PeriodicDIDContext.SamplesSentUUDT is modified when
        a confirmation is made, but at the time it reaches the value 0U it will immediatly
        transition to OutOfService.
      2) the value of the variable Dcm_PeriodicDIDContext.SamplesSentUUDT is modified in
        the state PeriodicSendingUUDT, but it is reset and updated with the number of samples.
  */
  DCM_PRECONDITION_ASSERT(Dcm_PeriodicDIDContext.SamplesSentUUDT > 0U,
                          DCM_INTERNAL_API_ID);

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_CONFIRMANDCHECKPENDINGCONFIRMATIONS_ENTRY();

  Dcm_PeriodicDIDContext.SamplesSentUUDT--;

  /* Got confirmation for all transmissions */
  if (Dcm_PeriodicDIDContext.SamplesSentUUDT == 0U)
  {
    ReturnValue = TRUE;
  }
  else
  {
    ReturnValue = FALSE;
  }

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_CONFIRMANDCHECKPENDINGCONFIRMATIONS_EXIT(ReturnValue);

  return ReturnValue;
}

/* !LINKSTO Dcm.Dsn.IB.RDBPI.AllowNewTransmission,1 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_AllowNewTransmission(void)
{
  boolean ReturnVal = FALSE;

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_ALLOWNEWTRANSMISSION_ENTRY();

  /* If no other transmission is ongoing or waiting for confirmation, allow the new transmission */
  if (Dcm_PeriodicDIDContext.SamplesSentUUDT == 0U)
  {
    ReturnVal = TRUE;
  }
  else
  {
    ReturnVal = FALSE;
  }

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_ALLOWNEWTRANSMISSION_EXIT(ReturnVal);
  return ReturnVal;
}

/* !LINKSTO Dcm.Dsn.IB.RDBPI.UUDTSendNextPDID,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_UUDTSendNextPDID(
  uint8 InstIdx)
{
  PduIdType RxPduId = Dcm_PeriodicDIDContext.CurrentClientRxPduId;

  uint16 RingIdx = Dcm_Dsp_ReadDataByPeriodicIdentifier_GetNextSample(RxPduId);

  /* Get the index of the first and the total number of available periodic connections
   * for this channel */
  Dcm_PeriodicConnectionNumberType NumberOfPeriodicConnections
    = Dcm_PeriodicTransmissionConfig[Dcm_MainConnectionConfig[Dcm_RxPduIdConfig[RxPduId]
                      .MainConnectionIndex].PeriodicTransmissionIndex].NumberOfPeriodicConnections;

  /* !LINKSTO Dcm.EB.PeriodicTransmission.TYPE2.TxPduId,1 */
  Dcm_PeriodicConnectionIndexType IndexOfFirstPeriodicConnection
    = Dcm_PeriodicTransmissionConfig[Dcm_MainConnectionConfig[Dcm_RxPduIdConfig[RxPduId]
    .MainConnectionIndex].PeriodicTransmissionIndex].IndexOfFirstPeriodicConnection;

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_UUDTSENDNEXTPDID_ENTRY(InstIdx);

  /* If another sample with the same RxPduId is available */
  if (RingIdx != DCM_PDID_TRANSRING_INVALID_IDX)
  {
    /* Variable to get the channel id */
    Dcm_RxPduIdInfoType RxPduIdInfo;

    Dcm_PeriodicConnectionIndexType IndexPeriodicConnection;
    PduIdType TxConnectionIndex;

    Dcm_ComMChannelOperationStateType ComMChnTxState = DCM_COMM_OPERATION_DISABLED;

    /* Get the index of the periodic DID in the scheduler table to which the
     * sample to be send belongs */
    uint8 TxIdx = Dcm_PeriodicDIDContext.TransmissionRing[RingIdx].IndexInSchedulerTable;

    /* Get the index of the of the periodic connection to be used */
    IndexPeriodicConnection = IndexOfFirstPeriodicConnection
      + Dcm_PeriodicDIDContext.OffsetToNextPeriodicConnection[RxPduId];

    /* And the associated tx connection index */
    /* !LINKSTO SWS_Dcm_01101_DedicatedIfPdu,1 */
    TxConnectionIndex = Dcm_PeriodicConnectionConfig[IndexPeriodicConnection].ConfirmationTxPduId;

    /* Get the channel id */
    Dcm_Dsl_RxConnectionGetRxPduIdInformation(RxPduId, &RxPduIdInfo);

    /* If the channel is not in full communication state just drop the sample */
    /* !LINKSTO Dcm150,1, Dcm155,1 */
    ComMChnTxState = Dcm_Dsl_ComMChannelStateManager_GetTxState(RxPduIdInfo.ComMChannelId);
    if (DCM_COMM_OPERATION_DISABLED == ComMChnTxState)
    {
      Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveSample(TxIdx, DCM_PDID_TRANSRING_REMOVE_FIRST);
    }
    /* If the TX connection is not idle, try the next one if available */
    /* !LINKSTO Dcm.EB.ReadDataByPeriodicIdentifier.PeriodicTxPduIdSelection,1 */
    else if (Dcm_Dsl_TxConnectionGetState(DCM_TX_CONNECTION_ID_FROM_INDEX(TxConnectionIndex))
              == DCM_TX_CONNECTION_IDLE)
    {
      /* Disposable buffer used to do the periodic transmission */
      Dcm_BufferType VirtualTransmissionBuffer;
      Dcm_TransmissionTimeoutType TransmissionTimeout;

      /* get the context information current instance */
      P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_CONST) curHsmInfo = &(DCM_HSM_INFO(InstIdx));

      VirtualTransmissionBuffer.BufferPtr
        = Dcm_PeriodicDIDContext.TransmissionRing[RingIdx].SamplingBuffer;

      VirtualTransmissionBuffer.FilledLength
        = Dcm_PeriodicDIDContext.TransmissionRing[RingIdx].Occupied;

      VirtualTransmissionBuffer.Size = DCM_PERIODIC_DID_SAMPLING_BUFFER_SIZE;

      VirtualTransmissionBuffer.BufferId = DCM_BUFFER_INVALID_ID;

      /* Set the next tx connection to be used */
      curHsmInfo->TxConnectionId = DCM_TX_CONNECTION_ID_FROM_INDEX(TxConnectionIndex);

      /* The protocol shall need to know the ComM ChannelId in the context of which the
       * transmission is being executed */
      curHsmInfo->ComMChannelId = RxPduIdInfo.ComMChannelId;

      curHsmInfo->TesterSourceAddress = Dcm_MainConnectionConfig[Dcm_RxPduIdConfig[RxPduId]
        .MainConnectionIndex].TesterSourceAddress;

      curHsmInfo->RequestType = DCM_PERIODIC_REQUEST;

      /* Store the TxConnectionId on which the sample is transmitted */
      Dcm_PeriodicDIDContext.TransmissionRing[RingIdx].TxConnectionId = curHsmInfo->TxConnectionId;

      /* With this transmission, the owner of the buffer gets changed to the TxConnection */
      (void)Dcm_Dsl_BufferGive(DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(InstIdx)),
                               curHsmInfo->TxConnectionId,
                               VirtualTransmissionBuffer.BufferId);

      TransmissionTimeout =
        Dcm_Dsl_DiagnosticSessionHandler_GetTimerValue(DCM_INST(InstIdx),
                                                       DCM_P2TIMER);

      /* All the data to be transmitted is available in the buffer at this moment */
      /* !LINKSTO SWS_Dcm_01072,1 */

      /* Start the transmission */
      Dcm_Dsl_TxConnectionTransmit(curHsmInfo->TxConnectionId,
                                   curHsmInfo->ComMChannelId,
                                   DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(InstIdx)),
                                   FALSE,
                                   VirtualTransmissionBuffer.FilledLength,
                                   TransmissionTimeout,
                                   VirtualTransmissionBuffer
                                   );

      Dcm_PeriodicDIDContext.SamplesSentUUDT++;
    }
    else
    {
      /* Do nothing. MISRA mandated else after else if */
    }

    /* Update offset to index of next periodic connection to send data on */
    Dcm_PeriodicDIDContext.OffsetToNextPeriodicConnection[RxPduId]
      = (Dcm_PeriodicDIDContext.OffsetToNextPeriodicConnection[RxPduId] + 1U)
      % NumberOfPeriodicConnections;

    /* If there are more periodic connection to send on try next PDID */
    /* !LINKSTO Dcm.EB.ReadDataByPeriodicIdentifier.MultipleTxPduIds,1 */
    Dcm_PeriodicDIDContext.CurrentClientConnectionsUsed++;
    if (Dcm_PeriodicDIDContext.CurrentClientConnectionsUsed < NumberOfPeriodicConnections)
    {
      /* Trigger sending the next PDID on the next available periodic TxPduId */
      (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(InstIdx),
                                    DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT);
    }
    else
    {
      Dcm_PeriodicDIDContext.CurrentClientConnectionsUsed = 0U;

      /* Trigger the transition to state PeriodicSendingUUDTFinishing where to
       * wait for confirmations, if any */
      (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(InstIdx),
          DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE);
    }
  }
  else
  {
    Dcm_PeriodicDIDContext.CurrentClientConnectionsUsed = 0U;

    /* Trigger the transition to state PeriodicSendingUUDTFinishing where to
     * wait for confirmations, if any */
    (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(InstIdx),
                                  DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE);
  }

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_UUDTSENDNEXTPDID_EXIT();
}

/* !LINKSTO Dcm.Dsn.IB.RDBPI.PeriodicDidProcess,1, Dcm.Dsn.IB.RDBPI.UUDTSetup,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_UUDTSetup(uint8 InstIdx)
{
  Dcm_PeriodicDIDContext.SamplesSentUUDT = 0U;

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_UUDTSETUP_ENTRY(InstIdx);

  /* Go ahead only if there is at least one periodic DID sample ready for transmission */
  if (Dcm_PeriodicDIDContext.NumberOfTransmittableSamples > 0U)
  {
    BufReq_ReturnType TxBufferAllocationResult = BUFREQ_E_NOT_OK;

    /* ENTER critical section */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    /* Attempt to allocate the TxBuffer. */

    /* For transmissions, the size of the transmission data is unknown yet, therefore the buffer
       allocator shall ignore this parameter for buffers for which the usage is transmission. */
    TxBufferAllocationResult
      = Dcm_Dsl_BufferAllocate(DCM_PROTOCOL_ID_FROM_INDEX(DCM_CUR_INST_ID(InstIdx)),
                               DCM_BUFFER_TX_PERIODIC,
                               DCM_BUFFER_IGNORE_BUFFER_SIZE,
                               &(DCM_HSM_INFO(InstIdx).TxBuffer));

    /* LEAVE critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    /* If buffer allocation successful */
    if (BUFREQ_OK == TxBufferAllocationResult)
    {
      /* Get the index of the first periodic DID in the scheduler table to which the sample
       * to be send belongs */
      uint8 TxIdx = Dcm_PeriodicDIDContext.TransmissionRing[Dcm_PeriodicDIDContext
                                                       .HeadTransmissionRing].IndexInSchedulerTable;

      /* Get the client (identified by the RxPduId) which to serve during this cycle. Only
       * one client can be served per cycle. If there are more than one samples generated by a
       * PDID belonging to this client and more than one periodic TxPduIds, one sample per
       * TxPduId and cycle will be sent */
      /* !LINKSTO Dcm.EB.ReadDataByPeriodicIdentifier.SingleClient,1 */
      Dcm_PeriodicDIDContext.CurrentClientRxPduId
        = Dcm_PeriodicDIDContext.SchedulerTable[TxIdx].RxPduId;

      Dcm_PeriodicDIDContext.CurrentClientConnectionsUsed = 0U;

      /* Try to send a sample belonging to the main connection selected by the RxPduId */
      Dcm_Dsp_ReadDataByPeriodicIdentifier_UUDTSendNextPDID(InstIdx);
    }
    else
    {
      /* If no packet could be sent emit a PROC_ABORT event to get back to OOS */
      /* !LINKSTO Dcm.PeriodicDid.Dsn.CancelProcess,1 */
      (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(InstIdx),
                                    DCM_HSM_DCM_EV_PROC_ABORT);
    }
  }
  else
  {
    /* If no packet could be sent emit a PROC_ABORT event to get back to OOS */
    /* !LINKSTO Dcm.PeriodicDid.Dsn.CancelProcess,1 */
    (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(InstIdx),
                                  DCM_HSM_DCM_EV_PROC_ABORT);
  }

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_UUDTSETUP_EXIT();
}

/* !LINKSTO Dcm.Dsn.IB.RDBPI.RemoveUUDTSampleOnConfirmation,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveUUDTSampleOnConfirmation(
  Dcm_BufferUserType TxConnectionId)
{
  uint16_least RingIndex = 0U;

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVEUUDTSAMPLEONCONFIRMATION_ENTRY(TxConnectionId);

  /* Search for the sample which was using the TxConnection on which the confirmation came
   * and clear that buffer location
   * The ConnectionId will always be found */
  /* Deviation TASKING-1 */
  while (RingIndex < DCM_PERIODIC_DID_TRANSMISSION_RING_ENTRIES)
  {
    if (TxConnectionId == Dcm_PeriodicDIDContext.TransmissionRing[RingIndex].TxConnectionId)
    {
     uint8 IndexInSchedTable = Dcm_PeriodicDIDContext.TransmissionRing[RingIndex].IndexInSchedulerTable;

     /* Remove the sample */
     Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveSample(IndexInSchedTable, DCM_PDID_TRANSRING_REMOVE_FIRST);
     /* Terminate the loop */
     RingIndex = DCM_PERIODIC_DID_TRANSMISSION_RING_ENTRIES;
    }
    else
    {
      RingIndex++;
    }
  }

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVEUUDTSAMPLEONCONFIRMATION_EXIT();
}

#endif /* ( (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
            (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) ) */

/* !LINKSTO Dcm.Dsn.IB.RDBPI.Init,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_Init(void)
{
  uint16 Index;
  /* Read data by periodic identifier inits */

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_INIT_ENTRY();

  /* Initialize the periodic DID scheduler table */
  /* Deviation TASKING-1 */
  for (Index=0; Index<DCM_MAX_PERIODIC_DID_SCHEDULER; Index++)
  {
    Dcm_PeriodicDIDContext.SchedulerTable[Index].TransmissionMode
      = DCM_UDS_RDBPI_TRANSMISSIONMODE_STOPSENDING;

    Dcm_PeriodicDIDContext.SchedulerTable[Index].IndexOfNextPeriodicDID
      = DCM_PDID_SCHEDTABLE_INVALID_IDX;
  }
  /* Initialize the transmission ring for the periodic DID samples */
  /* Deviation TASKING-1 */
  for (Index = 0U; Index < DCM_PERIODIC_DID_TRANSMISSION_RING_ENTRIES; Index++)
  {
    Dcm_PeriodicDIDContext.TransmissionRing[Index].IndexInSchedulerTable
      = DCM_PDID_SCHEDTABLE_INVALID_IDX;

#if ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U))
    Dcm_PeriodicDIDContext.TransmissionRing[Index].TxConnectionId = DCM_PDID_TX_CONNECTION_ID_INVALID;
#endif /*#if ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U))*/
  }

#if ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U))
  /* Initialize the offsets to the next periodic connections */
  TS_MemSet(Dcm_PeriodicDIDContext.OffsetToNextPeriodicConnection,0U,DCM_NUM_RX_PDU_ID);

  Dcm_PeriodicDIDContext.SamplesSentUUDT = 0U;
  Dcm_PeriodicDIDContext.CurrentClientRxPduId = 0U;
  Dcm_PeriodicDIDContext.CurrentClientConnectionsUsed = 0U;
#endif /* #if ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) &&
              (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U)) */

  Dcm_PeriodicDIDContext.ForceReValidateConditions = FALSE;
  Dcm_PeriodicDIDContext.NumberOfScheduledPDIDs = 0U;
  Dcm_PeriodicDIDContext.NextPeriodicDIDList = DCM_PDID_SCHEDTABLE_INVALID_IDX;
  Dcm_PeriodicDIDContext.HeadPeriodicDIDList = DCM_PDID_SCHEDTABLE_INVALID_IDX;
  Dcm_PeriodicDIDContext.TailPeriodicDIDList = DCM_PDID_SCHEDTABLE_INVALID_IDX;

  Dcm_PeriodicDIDContext.NumberOfTransmittableSamples = 0U;
  Dcm_PeriodicDIDContext.HeadTransmissionRing = DCM_PDID_TRANSRING_INVALID_IDX;
  Dcm_PeriodicDIDContext.TailTransmissionRing = DCM_PDID_TRANSRING_INVALID_IDX;

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_INIT_EXIT();
}

/* !LINKSTO Dcm.Dsn.IB.RDBPI.ForceReValidate,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_ForceReValidate(void)
{
  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_FORCEREVALIDATE_ENTRY();

  Dcm_PeriodicDIDContext.ForceReValidateConditions = TRUE;

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_FORCEREVALIDATE_EXIT();
}

/* !LINKSTO Dcm.Dsn.IB.RDBPI.IsPeriodicTxChannelEnabled,1 */
FUNC(boolean, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_IsPeriodicTxChannelEnabled(void)
{
  /* !LINKSTO Dcm150,1, Dcm155,1 */

  boolean ReturnValue = FALSE;

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_ISPERIODICTXCHANNELENABLED_ENTRY();

  /* Proceed only if the DID scheduled to be transmitted was not meanwhile disabled.*/
  if (Dcm_PeriodicDIDContext.NumberOfTransmittableSamples > 0U)
  {
    const uint16 HeadTransmissionRing = Dcm_PeriodicDIDContext.HeadTransmissionRing;

    /* Get the index of the periodic DID in the scheduler table to which the sample to be
     * send belongs */
    uint8 TxIdx
      = Dcm_PeriodicDIDContext.TransmissionRing[HeadTransmissionRing].IndexInSchedulerTable;

    /* Because a stop request is not possible this condition will always be true */
    DCM_PRECONDITION_ASSERT(Dcm_PeriodicDIDContext.SchedulerTable[TxIdx].TransmissionMode
      != DCM_UDS_RDBPI_TRANSMISSIONMODE_STOPSENDING, DCM_INTERNAL_API_ID);
    {
      Dcm_RxPduIdInfoType RxPduIdInfo;
      Dcm_ComMChannelOperationStateType ComMChnTxState = DCM_COMM_OPERATION_DISABLED;

      /* Make sure the communication channel on which to send the
       * periodic transmission is in full communication mode */
      const PduIdType RxPduId = Dcm_PeriodicDIDContext.SchedulerTable[TxIdx].RxPduId;

      Dcm_Dsl_RxConnectionGetRxPduIdInformation(RxPduId, &RxPduIdInfo);

      ComMChnTxState = Dcm_Dsl_ComMChannelStateManager_GetTxState(RxPduIdInfo.ComMChannelId);
      if (DCM_COMM_OPERATION_ENABLED == ComMChnTxState)
      {
        ReturnValue = TRUE;
      }
    }
  }

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_ISPERIODICTXCHANNELENABLED_EXIT(ReturnValue);

  return ReturnValue;
}

/* !LINKSTO Dcm.Dsn.IB.RDBPI.RemoveSecuredFromSchedulerTable,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveSecuredFromSchedulerTable(void)
{
  /* !LINKSTO SWS_Dcm_01108,1 */

  uint8 LastIndexSchedTable;
  uint8 IndexSchedTable = 0U;

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVESECUREDFROMSCHEDULERTABLE_ENTRY();

  /* Iterate over all scheduled DIDs and  remove them them if they are found to have security
   * levels configured. Remove the samples they generated */
  LastIndexSchedTable = Dcm_PeriodicDIDContext.HeadPeriodicDIDList;

  for (IndexSchedTable = Dcm_PeriodicDIDContext.HeadPeriodicDIDList;
      IndexSchedTable != DCM_PDID_SCHEDTABLE_INVALID_IDX;
      IndexSchedTable = Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable]
                          .IndexOfNextPeriodicDID
      )
  {
    Dcm_DidAccessInfoBaseTypePtr AccessInfo;
    Dcm_DidConfigIdxType ConfigIdx = 0;
    const uint16 PDID = Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].PDID;

    /* Get the index of the PDID in the Dcm_DidConfig table. As there cannot be any scheduled
     * periodic DIDs that are not in the Dcm_DidConfig, this call can never fail */
    (void)Dcm_Dsp_DidServices_GetDidIndex(PDID, &ConfigIdx);

    /* get the configured access info according to the requested did operation */
    AccessInfo = Dcm_Dsp_DidServices_GetAccessInfo(Dcm_DidConfig[ConfigIdx].DidInfoIdx,
                                                   DCM_DID_OP_PERIODIC);

    /* If it has security levels, stop it */
    if ((AccessInfo != NULL_PTR) && (AccessInfo->NumSecLevels > 0U))
    {
      /* Remove samples generated by this PDID from the transmission ring */
      /* !LINKSTO SWS_Dcm_01117,1 */
      Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveSample(IndexSchedTable,
                                                        DCM_PDID_TRANSRING_REMOVE_ALL);

      /* Clean up the periodic DID and remove it from the list */
      /* Set periodic DID to stopSending */
      Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].TransmissionMode
        = DCM_UDS_RDBPI_TRANSMISSIONMODE_STOPSENDING;

      /* When the periodic scheduler list is empty */
      if (Dcm_PeriodicDIDContext.HeadPeriodicDIDList
          == Dcm_PeriodicDIDContext.TailPeriodicDIDList
         )
      {
        /* Invalidate 'head', 'tail' and 'next' pointers */
        Dcm_PeriodicDIDContext.HeadPeriodicDIDList = DCM_PDID_SCHEDTABLE_INVALID_IDX;
        Dcm_PeriodicDIDContext.TailPeriodicDIDList = DCM_PDID_SCHEDTABLE_INVALID_IDX;
        Dcm_PeriodicDIDContext.NextPeriodicDIDList = DCM_PDID_SCHEDTABLE_INVALID_IDX;
      }
      else
      {
        /* !LINKSTO SWS_Dcm_01104,1 */

        /* Links last item's next index to the current item's next index so the item is taken
         * out of the list */
        Dcm_PeriodicDIDContext.SchedulerTable[LastIndexSchedTable].IndexOfNextPeriodicDID
          = Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].IndexOfNextPeriodicDID;

        /* Adjust 'head' of the list in case it is necessary */
        if (IndexSchedTable == Dcm_PeriodicDIDContext.HeadPeriodicDIDList)
        {
          Dcm_PeriodicDIDContext.HeadPeriodicDIDList
            = Dcm_PeriodicDIDContext
             .SchedulerTable[Dcm_PeriodicDIDContext.HeadPeriodicDIDList].IndexOfNextPeriodicDID;
        }

        /* Adjust 'tail' of the list in case it is necessary */
        if (IndexSchedTable == Dcm_PeriodicDIDContext.TailPeriodicDIDList)
        {
          Dcm_PeriodicDIDContext.TailPeriodicDIDList = LastIndexSchedTable;
        }

        /* Adjust 'next' of the list in case it is necessary */
        if (IndexSchedTable == Dcm_PeriodicDIDContext.NextPeriodicDIDList)
        {
          Dcm_PeriodicDIDContext.NextPeriodicDIDList
            = Dcm_PeriodicDIDContext
             .SchedulerTable[Dcm_PeriodicDIDContext.NextPeriodicDIDList].IndexOfNextPeriodicDID;
        }
      }

      /* One periodic scheduler table entry less */
      Dcm_PeriodicDIDContext.NumberOfScheduledPDIDs--;
      break;
    }

    /* Keep track of last index in scheduler table. This is needed to link a previous entry
     * to a next entry when removing items */
    LastIndexSchedTable = IndexSchedTable;
  }

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVESECUREDFROMSCHEDULERTABLE_EXIT();
}

/* !LINKSTO Dcm.Dsn.IB.RDBPI.RemoveFromSchedulerTable,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveFromSchedulerTable(
  P2CONST(uint8, AUTOMATIC, DCM_VAR) BufferPDIDs,
  uint8 NumberOfPDIDs
  )
{
  uint8 IndexSchedTable = 0U;
  uint8 IndexPDIDs = 0U;

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVEFROMSCHEDULERTABLE_ENTRY(BufferPDIDs,
    NumberOfPDIDs);

  /* Special case where all periodic DIDs must be stopped
   * (none is specified in the request messaage) */
  if (NumberOfPDIDs == 0U)
  {
    uint16 IndexRing = 0U;

    /* Set _all_ periodic DIDs to stopSending */
    for (IndexSchedTable = Dcm_PeriodicDIDContext.HeadPeriodicDIDList;
        IndexSchedTable != DCM_PDID_SCHEDTABLE_INVALID_IDX;
        IndexSchedTable = Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable]
                          .IndexOfNextPeriodicDID
        )
    {
      Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].TransmissionMode
        = DCM_UDS_RDBPI_TRANSMISSIONMODE_STOPSENDING;
    }

    /* Adjust 'head', 'tail' and 'next' of the periodic scheduler table */
    Dcm_PeriodicDIDContext.NumberOfScheduledPDIDs = 0U;
    Dcm_PeriodicDIDContext.HeadPeriodicDIDList = DCM_PDID_SCHEDTABLE_INVALID_IDX;
    Dcm_PeriodicDIDContext.TailPeriodicDIDList = DCM_PDID_SCHEDTABLE_INVALID_IDX;
    Dcm_PeriodicDIDContext.NextPeriodicDIDList = DCM_PDID_SCHEDTABLE_INVALID_IDX;

    /* Remove all entries from the transmission ring */
    /* !LINKSTO SWS_Dcm_01117,1 */
    /* Deviation TASKING-1 */
    for (IndexRing = 0U; IndexRing < DCM_PERIODIC_DID_TRANSMISSION_RING_ENTRIES; IndexRing++)
    {
      Dcm_PeriodicDIDContext.TransmissionRing[IndexRing].IndexInSchedulerTable
        = DCM_PDID_SCHEDTABLE_INVALID_IDX;

#if ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U))
      Dcm_PeriodicDIDContext.TransmissionRing[IndexRing].TxConnectionId = DCM_PDID_TX_CONNECTION_ID_INVALID;
#endif /*#if ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U))*/
    }

    Dcm_PeriodicDIDContext.NumberOfTransmittableSamples = 0U;
    Dcm_PeriodicDIDContext.HeadTransmissionRing = DCM_PDID_TRANSRING_INVALID_IDX;
    Dcm_PeriodicDIDContext.TailTransmissionRing = DCM_PDID_TRANSRING_INVALID_IDX;
  }
  else
  {
    /* Iterate over all scheduled DIDs and  remove them if they are found to match the requested.
     * Remove the samples they generated. */
    for (IndexPDIDs = 0U; IndexPDIDs < NumberOfPDIDs; IndexPDIDs++)
    {
      /* Periodic DIDs are in the range 0xF200 .. 0xF2FF */
      uint16 RequestedPDID = 0xF200U | BufferPDIDs[IndexPDIDs];

      uint8 LastIndexSchedTable = Dcm_PeriodicDIDContext.HeadPeriodicDIDList;

      /* Check all PDIDs in the scheduler list */
      for (IndexSchedTable = Dcm_PeriodicDIDContext.HeadPeriodicDIDList;
          IndexSchedTable != DCM_PDID_SCHEDTABLE_INVALID_IDX;
          IndexSchedTable = Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable]
                              .IndexOfNextPeriodicDID
          )
      {
        /* Check each one if it is the PDID to be removed */
        if (Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].PDID == RequestedPDID)
        {
          /* Remove samples generated by this PDID from the transmission ring */
          /* !LINKSTO SWS_Dcm_01117,1 */
          Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveSample(IndexSchedTable,
                                                            DCM_PDID_TRANSRING_REMOVE_ALL);

          /* Clean up the periodic DID and remove it from the list */
          /* Set periodic DID to stopSending */
          Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].TransmissionMode
            = DCM_UDS_RDBPI_TRANSMISSIONMODE_STOPSENDING;

          /* When the periodic scheduler list is empty */
          if (Dcm_PeriodicDIDContext.HeadPeriodicDIDList
              == Dcm_PeriodicDIDContext.TailPeriodicDIDList
             )
          {
            /* Invalidate 'head', 'tail' and 'next' pointers */
            Dcm_PeriodicDIDContext.HeadPeriodicDIDList = DCM_PDID_SCHEDTABLE_INVALID_IDX;
            Dcm_PeriodicDIDContext.TailPeriodicDIDList = DCM_PDID_SCHEDTABLE_INVALID_IDX;
            Dcm_PeriodicDIDContext.NextPeriodicDIDList = DCM_PDID_SCHEDTABLE_INVALID_IDX;
          }
          else
          {
            /* !LINKSTO SWS_Dcm_01104,1 */

            /* Links last item's next index to the current item's next index so the item is taken
             * out of the list */
            Dcm_PeriodicDIDContext.SchedulerTable[LastIndexSchedTable].IndexOfNextPeriodicDID
              = Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].IndexOfNextPeriodicDID;

            /* Adjust 'head' of the list in case it is necessary */
            if (IndexSchedTable == Dcm_PeriodicDIDContext.HeadPeriodicDIDList)
            {
              Dcm_PeriodicDIDContext.HeadPeriodicDIDList
                = Dcm_PeriodicDIDContext
                 .SchedulerTable[Dcm_PeriodicDIDContext.HeadPeriodicDIDList].IndexOfNextPeriodicDID;
            }

            /* Adjust 'tail' of the list in case it is necessary */
            if (IndexSchedTable == Dcm_PeriodicDIDContext.TailPeriodicDIDList)
            {
              Dcm_PeriodicDIDContext.TailPeriodicDIDList = LastIndexSchedTable;
            }

            /* Adjust 'next' of the list in case it is necessary */
            if (IndexSchedTable == Dcm_PeriodicDIDContext.NextPeriodicDIDList)
            {
              Dcm_PeriodicDIDContext.NextPeriodicDIDList
                = Dcm_PeriodicDIDContext
                 .SchedulerTable[Dcm_PeriodicDIDContext.NextPeriodicDIDList].IndexOfNextPeriodicDID;
            }
          }

          /* One periodic scheduler table entry less */
          Dcm_PeriodicDIDContext.NumberOfScheduledPDIDs--;
          break;
        }

        /* Keep track of last index in scheduler table. This is needed to link a previous entry
         * to a next entry when removing items */
        LastIndexSchedTable = IndexSchedTable;
      }
    }
  }

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVEFROMSCHEDULERTABLE_EXIT();
}

/* !LINKSTO Dcm.Dsn.IB.RDBPI.GetNumberOfTransmittableSamples,1 */
FUNC(uint16, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_GetNumberOfTransmittableSamples(void)
{
  return Dcm_PeriodicDIDContext.NumberOfTransmittableSamples;
}
/* !LINKSTO Dcm.Dsn.IB.RDBPI.SessionChange,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_SessionChange(
  Dcm_SesCtrlType OldSession, Dcm_SesCtrlType NewSession)
{
  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_SESSIONCHANGE_ENTRY(OldSession,NewSession);

  if (DCM_DEFAULT_SESSION == NewSession)
  {
    /* Remove all scheduled periodic DIDs from the scheduler table and
     * transmission ring entries that were generated by those entries. */
    /* !LINKSTO SWS_Dcm_01107,1 */
    Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveFromSchedulerTable(NULL_PTR, 0U);

  }
  else
  {
    /* Only apply the below requirement if the session actually changed */
    /* !LINKSTO SWS_Dcm_01108,1 */
    if (NewSession != OldSession)
    {
      /* Stop all periodic DIDs in the scheduler that have security levels configured */
      Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveSecuredFromSchedulerTable();
    }
  }

  /* Trigger re-checking the conditions of scheduled periodic DIDs */
  Dcm_Dsp_ReadDataByPeriodicIdentifier_ForceReValidate();

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_SESSIONCHANGE_EXIT();
}

/*==================[internal function definitions]==============================================*/

#if ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U))
/* !LINKSTO Dcm.Dsn.IB.RDBPI.GetNextSample,1 */
STATIC FUNC(uint16, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_GetNextSample(PduIdType RxPduId)
{
  /* !LINKSTO Dcm.EB.ReadDataByPeriodicIdentifier.MultipleTxPduIds,1 */

  uint16 FoundIndex = DCM_PDID_TRANSRING_INVALID_IDX;
  uint16 RingIndex = 0U;

  /* Check all periodic DID samples that are currently waiting for transmission */
  for (RingIndex = Dcm_PeriodicDIDContext.HeadTransmissionRing;
       RingIndex != DCM_PDID_TRANSRING_INVALID_IDX;
       RingIndex = Dcm_PeriodicDIDContext.TransmissionRing[RingIndex].SendNext
      )
  {
    /* Find the next active sample that matches the requested RxPduId
     * which is not already being transmitted and waiting for confirmation */
    if ((Dcm_PeriodicDIDContext.TransmissionRing[RingIndex].IndexInSchedulerTable
          != DCM_PDID_SCHEDTABLE_INVALID_IDX
        )
        &&
        (Dcm_PeriodicDIDContext.SchedulerTable[Dcm_PeriodicDIDContext.TransmissionRing[RingIndex]
          .IndexInSchedulerTable].RxPduId == RxPduId
        )
        &&
        (Dcm_PeriodicDIDContext.TransmissionRing[RingIndex].TxConnectionId == DCM_PDID_TX_CONNECTION_ID_INVALID)
       )
    {
      FoundIndex = RingIndex;
      break;
    }
  }

  return FoundIndex;
}
#endif /* #if ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON)
                && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U)) */

/* !LINKSTO Dcm.Dsn.IB.RDBPI.ProcessCounters,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_ProcessTimersAndSamplePDIDs(void)
{
  uint8 IndexSchedTable = 0U;
  uint8 StartAt = 0U;
  uint8 StopAt = 0U;
  boolean TransmissionRingFull = FALSE;
  boolean AllScheduledPDIDsProcessed = FALSE;

  /* Initialize the 'next' pointer if it is not initialized */
  if (Dcm_PeriodicDIDContext.NextPeriodicDIDList == DCM_PDID_SCHEDTABLE_INVALID_IDX)
  {
    Dcm_PeriodicDIDContext.NextPeriodicDIDList = Dcm_PeriodicDIDContext.HeadPeriodicDIDList;
  }

  /* First round goes from 'next' to the end of the list. The second round will go from 'head' to
   * 'next' */
  StartAt = Dcm_PeriodicDIDContext.NextPeriodicDIDList;
  StopAt = DCM_PDID_SCHEDTABLE_INVALID_IDX;

  do
  {
    /* Walk the list of scheduled periodic DIDs */
    /* !LINKSTO SWS_Dcm_01104,1 */
    for (IndexSchedTable = StartAt;
         IndexSchedTable != StopAt;
         IndexSchedTable = Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable]
                           .IndexOfNextPeriodicDID
        )
    {
      /* Count down the periodic timer. When it expires and there is space in the
       * transission ring, the periodic DID will be sampled and the timer
       * reloaded to its initial value depending on its transmission mode */
      if (Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].PeriodicTimer > 0U)
      {
        Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].PeriodicTimer--;
      }

      /* When a periodic DID's timer expires it must be sampled right on the spot */
      /* !LINKSTO Dcm.EB.ReadDataByPeriodicIdentifier.PDID.Sampling,1 */
      if (Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].PeriodicTimer == 0U)
      {
        /* RxPduId used in order to get the protocol for which the session/security
         * permissions are considered when reading the DID */
        /* !LINKSTO Dcm.EB.PeriodicDid.SessionAndSecurity,1 */
        const PduIdType RxPduId = Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].RxPduId;

        /* Check if there is space left in the transmission ring. If no space is left
         * the sample will not be taken but simply ignored */
        if (Dcm_PeriodicDIDContext.NumberOfTransmittableSamples
            < DCM_PERIODIC_DID_TRANSMISSION_RING_ENTRIES
           )
        {
          Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
#if(DCM_DSP_SERVICE_0X22_ASYNC == STD_ON)
          Std_ReturnType LockStatus = Dcm_Dsp_DidServices_LockService();

          /* !LINKSTO Dcm.EB.ReadDataByPeriodicId.SameTimeWithReadDID,1 */
          if(LockStatus == DCM_E_OK)
#endif /* #if (DCM_DSP_SERVICE_0X22_ASYNC == STD_ON) */
          {
            /* Determine the next free slot in the transmission ring to sample the PDID to.
            * This function cannot fail because it was checked above that there are free
            * slots in the transmission ring */
            uint16 FreeSlot = Dcm_Dsp_ReadDataByPeriodicIdentifier_GetFreeTransmissionRingSlot();

            /* Periodic responses do not carry a SID as the first octet,
            * but the lower 8 bits of the periodic DID */
            Dcm_PeriodicDIDContext.TransmissionRing[FreeSlot].SamplingBuffer[0]
              = (uint8)Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].PDID;

            /* Use the sampling buffer of a free transmission ring slot to store the read DID data */
            PeriodicReadDid_BufferCfg.BufferPtr = Dcm_PeriodicDIDContext.TransmissionRing[FreeSlot].SamplingBuffer;
            PeriodicReadDid_BufferCfg.Size      = DCM_PERIODIC_DID_SAMPLING_BUFFER_SIZE;
            PeriodicReadDid_BufferCfg.Occupied  = 1U;

            /* Initialize the reading process for the DID */
            Dcm_Dsp_DidServices_ReadDid_Init(&Dcm_ReadPeriodicDidContext,
                Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].PDID
                ,DCM_DID_OP_PERIODIC,
                DCM_DO_NOT_WRITE_DID_IDS,
                &PeriodicReadDid_BufferCfg,
                DCM_DO_NOT_EXECUTE_SESSION_SECURITY_MODERULE_CHECKS,
                RxPduId
                );

            /* Execute the reading process for the DID data */
            /* !LINKSTO SWS_Dcm_01106,1 */
            serviceProcessStatus = Dcm_Dsp_DidServices_ReadDid_Execute(&Dcm_ReadPeriodicDidContext);

            /* !LINKSTO SWS_Dcm_01105,1 */
            if (E_OK == serviceProcessStatus)
            {
              /* Insert the freshly read data sample into the transmission ring */
              Dcm_PeriodicDIDContext.TransmissionRing[FreeSlot].IndexInSchedulerTable
                = IndexSchedTable;
              Dcm_PeriodicDIDContext.TransmissionRing[FreeSlot].Occupied
                = (uint16)(PeriodicReadDid_BufferCfg.Occupied);

              /* Append the sample to the transmission ring */
              Dcm_PeriodicDIDContext.TransmissionRing[FreeSlot].SendNext
                = DCM_PDID_TRANSRING_INVALID_IDX;

              /* In case the transmission ring is empty setup the head and tail pointers */
              if (Dcm_PeriodicDIDContext.HeadTransmissionRing == DCM_PDID_TRANSRING_INVALID_IDX)
              {
                Dcm_PeriodicDIDContext.HeadTransmissionRing = FreeSlot;
              }
              /* Newly added samples always go to the end of the ring to maintain transmission order*/
              if (Dcm_PeriodicDIDContext.TailTransmissionRing != DCM_PDID_TRANSRING_INVALID_IDX)
              {
                Dcm_PeriodicDIDContext.TransmissionRing[Dcm_PeriodicDIDContext.TailTransmissionRing]
                  .SendNext = FreeSlot;
              }

              /* Entry is the new tail */
              Dcm_PeriodicDIDContext.TailTransmissionRing = FreeSlot;

              /* Account for the new sample in the transmission ring */
              Dcm_PeriodicDIDContext.NumberOfTransmittableSamples++;
            }
            /* else ignore the reading */
            /* !LINKSTO Dcm.PeriodicDid.Dsn.SchedDidDone,1 */

#if(DCM_DSP_SERVICE_0X22_ASYNC == STD_ON)
            /* Unlock DidServices */
            Dcm_Dsp_DidServices_UnlockService();
#endif /* #if(DCM_DSP_SERVICE_0X22_ASYNC == STD_ON) */
          }
          /* Else DidService is locked by other unit, than ignore the reading */


          /* Reset the periodic timer to the initial value */
          /* !LINKSTO SWS_Dcm_01102,1 */
          Dcm_Dsp_ReadDataByPeriodicIdentifier_RestartPeriodicTimer(IndexSchedTable);

        }
        else
        {
          if (TransmissionRingFull == FALSE)
          {
            TransmissionRingFull = TRUE;
            Dcm_PeriodicDIDContext.NextPeriodicDIDList = IndexSchedTable;
          }
        }
      }
    }

    /* In case processing of scheduled periodic DIDs did not start from the head of the
     * scheduler list do also process the ones from start to where processing began before */
    if (StartAt != Dcm_PeriodicDIDContext.HeadPeriodicDIDList)
    {
      AllScheduledPDIDsProcessed = FALSE;
      StopAt = StartAt;
      StartAt = Dcm_PeriodicDIDContext.HeadPeriodicDIDList;
    }
    else
    {
      AllScheduledPDIDsProcessed = TRUE;
    }
  } while (AllScheduledPDIDsProcessed == FALSE);
}

/* !LINKSTO Dcm.Dsn.IB.RDBPI.RestartPeriodicTimer,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_RestartPeriodicTimer(
  uint8 IndexSchedTable)
{
  uint16 PeriodicTimer = 0U;

  /* Restart the timer for the periodic DID by setting it to the value that was configured for
   * its transmission mode */
  switch (Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].TransmissionMode)
  {
    case DCM_UDS_RDBPI_TRANSMISSIONMODE_SLOWRATE:
    {
      PeriodicTimer = DCM_PERIODIC_TRANSMISSION_SLOW_RATE;
    }
    break;

    case DCM_UDS_RDBPI_TRANSMISSIONMODE_MEDIUMRATE:
    {
      PeriodicTimer = DCM_PERIODIC_TRANSMISSION_MEDIUM_RATE;
    }
    break;

    case DCM_UDS_RDBPI_TRANSMISSIONMODE_FASTRATE:
    {
      PeriodicTimer = DCM_PERIODIC_TRANSMISSION_FAST_RATE;
    }
    break;

    /* CHECK: NOPARSE */
    default:
    {
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_RDBPI);
    }
    break;
    /* CHECK: PARSE */
  }

  Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].PeriodicTimer = PeriodicTimer;
}

/* The scheduler table must only be changed when we know we have entries that can be added */
/* !LINKSTO Dcm.Dsn.IB.RDBPI.AddToSchedulerTable,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_AddToSchedulerTable(
  PduIdType RxPduId,
  P2CONST(uint8, AUTOMATIC, DCM_VAR) BufferPDIDs,
  uint8 NumberOfPDIDs,
  uint8 TransmissionMode
  )
{
  uint8 IndexPDIDs = 0U;

  /* For all periodic DIDs within the current RDBPI service request. The requested periodic DIDS
   * were already filtered such to be unique and able to be read in the current diagnostic session,
   * security level and mode condition */
  for (IndexPDIDs = 0U; IndexPDIDs < NumberOfPDIDs; IndexPDIDs++)
  {
    /* Periodic DIDs are in the range 0xF200 .. 0xF2FF */
    uint16 RequestedPDID = 0xF200U | BufferPDIDs[IndexPDIDs];
    uint8 IndexSchedTable = 0U;

    /* Check if the requested periodic DID is already scheduled */
    for (IndexSchedTable = Dcm_PeriodicDIDContext.HeadPeriodicDIDList;
         IndexSchedTable != DCM_PDID_SCHEDTABLE_INVALID_IDX;
         IndexSchedTable = Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable]
                            .IndexOfNextPeriodicDID
        )
    {
      if (RequestedPDID == Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].PDID)
      {
        break;
      }
    }

    /* If the periodic DID is already in the scheduler table */
    if (IndexSchedTable != DCM_PDID_SCHEDTABLE_INVALID_IDX)
    {
      /* Treat it as if it was removed first and then added again i.e. it is initializes again and
       * goes to the tail of the periodic DID scheduler table */
      Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveFromSchedulerTable(&BufferPDIDs[IndexPDIDs], 1U);
    }
    else
    {
      boolean IndexFound = FALSE;

      /* Find a free slot in the scheduler table. This cannot fail as it was already verified that
       * there are free slots in the scheduler table. Implemented like this for the sake of code
       * coverage */
      IndexSchedTable = 0U;
      /* Deviation TASKING-2 */
      while (IndexFound == FALSE)
      {
        if (Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].TransmissionMode
             == DCM_UDS_RDBPI_TRANSMISSIONMODE_STOPSENDING
           )
        {
          IndexFound = TRUE;
        }
        else
        {
          IndexFound = FALSE;
        }

        IndexSchedTable++;
      }

      /* Adjust the index due to the above increment prior to comparison */
      IndexSchedTable--;
    }

    /* !LINKSTO Dcm.PeriodicDid.Dsn.SchedEntry,1 */

    /* Set up the new entry in the scheduler table according to the periodic DID to be transmitted*/
    Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].TransmissionMode = TransmissionMode;
    Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].PDID = RequestedPDID;
    Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].RxPduId = RxPduId;

    /* Set the periodic timer such that the PDID will be transmitted
     * immediately in the next cycle. THE VALUE MUST BE > 0 */
    /* !LINKSTO Dcm.EB.ReadDataByPeriodicIdentifier.InitialTimerValue,1 */
    Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].PeriodicTimer
      = DCM_PDID_DELAY_BEFORE_FIRST_TRANSMISSION;

    /* New entry to periodic scheduler table is added */
    Dcm_PeriodicDIDContext.NumberOfScheduledPDIDs++;

    Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].IndexOfNextPeriodicDID
      = DCM_PDID_SCHEDTABLE_INVALID_IDX;

    /* In case the list is empty setup the head and tail pointers */
    if (Dcm_PeriodicDIDContext.HeadPeriodicDIDList == DCM_PDID_SCHEDTABLE_INVALID_IDX)
    {
      Dcm_PeriodicDIDContext.HeadPeriodicDIDList = IndexSchedTable;
    }
    if (Dcm_PeriodicDIDContext.TailPeriodicDIDList != DCM_PDID_SCHEDTABLE_INVALID_IDX)
    {
      /* Newly added periodic DIDs are always appended to the the end of the list to maintain
       * transmission order according to configuration order */
      /* !LINKSTO SWS_Dcm_01104,1, Dcm.EB.ReadDataByPeriodicIdentifier.Scheduler.Add,1 */
      Dcm_PeriodicDIDContext.SchedulerTable[Dcm_PeriodicDIDContext.TailPeriodicDIDList]
                                                          .IndexOfNextPeriodicDID = IndexSchedTable;
    }
    /* Appended entry is the new tail */
    Dcm_PeriodicDIDContext.TailPeriodicDIDList = IndexSchedTable;
  }
}

/* !LINKSTO Dcm.Dsn.IB.RDBPI.ValidateMessage,1 */
STATIC FUNC(Dcm_NegativeResponseCodeType, DCM_CODE)
      Dcm_Dsp_ReadDataByPeriodicIdentifier_ValidateMessage(
  P2CONST(uint8, AUTOMATIC, DCM_VAR) MessageBuffer,
  Dcm_MsgLenType MessageLength)
{
  Dcm_NegativeResponseCodeType Nrc = DCM_E_GENERALREJECT;

  /* !LINKSTO Dcm.Dsn.IB.RDBPI.SvcH.GenericCondCheck,1 */
  /* Check if in the request there is at least the transmissionMode present */
  /* !LINKSTO SWS_Dcm_01093_AllowedMessageLengths,1 */
  if (MessageLength < 1U)
  {
      /* Set the incorrect message length or invalid format NRC */
    Nrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
  }
  else
  {
    /* Get the requested transmission mode */
    uint8 TransmissionMode = MessageBuffer[0U];

    /* Get the number of PDIDs present in the request message.
     * reqDataLen also contains the Transmission mode, therefore this element must
     * must be subtracted from the total length */
    uint8 PDIDCount = (uint8)(MessageLength - 1U);

    /* Make sure the number of periodic PDIDs present in this request message
     * does not exceed the configured limit */
    /* !LINKSTO SWS_Dcm_00843,1 */
    if (PDIDCount > DCM_MAX_PERIODIC_DID_READ)
    {
      /* Set the incorrect message length or invalid format NRC */
      Nrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    }
    /* Only the stopSending transmissionMode allows for an empty list of PDIDs */
    /* !LINKSTO SWS_Dcm_01093_AllowedMessageLengths,1 */
    else if ((TransmissionMode != DCM_UDS_RDBPI_TRANSMISSIONMODE_STOPSENDING)
             && (MessageLength < 2U)
            )
    {
      /* Set the incorrect message length or invalid format NRC */
      Nrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    }
    /* The comparison of the DCM_PERIODIC_TRANSMISSION_*_RATE macros against 0 is due
     * to the fact that those macros are generated from the configuration and may be
     * configured to 0 which means that the given transmission rate is not available */

    /* Validate transmission mode */
    /* !LINKSTO SWS_Dcm_01094,1 */
    else if ((TransmissionMode != DCM_UDS_RDBPI_TRANSMISSIONMODE_STOPSENDING)
#if (DCM_PERIODIC_TRANSMISSION_SLOW_RATE > 0U)
        && (TransmissionMode != DCM_UDS_RDBPI_TRANSMISSIONMODE_SLOWRATE)
#endif
#if (DCM_PERIODIC_TRANSMISSION_MEDIUM_RATE > 0U)
        && (TransmissionMode != DCM_UDS_RDBPI_TRANSMISSIONMODE_MEDIUMRATE)
#endif
#if (DCM_PERIODIC_TRANSMISSION_FAST_RATE > 0U)
        && (TransmissionMode != DCM_UDS_RDBPI_TRANSMISSIONMODE_FASTRATE)
#endif
       )
    {
      Nrc = DCM_E_REQUESTOUTOFRANGE;
    }
    else
    {
      /* Message validation was OK */
      Nrc = DCM_E_POSITIVERESPONSE;
    }
  }

  return Nrc;
}

/* Remove all duplicated DIDs from the request */
/* !LINKSTO Dcm.Dsn.IB.RDBPI.RemoveDuplicates,1 */
STATIC FUNC(uint8, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveDuplicates(
  P2VAR(uint8, AUTOMATIC, DCM_VAR) BufferPDIDs,
  uint8 NumberOfPDIDs)
{
  /* !LINKSTO Dcm.Service.ReadDataByPeriodicId.ReqDuplicateDid,1 */

  /* beginning of the array of DIDs */
  uint8 StartOfArrayToSort = 0U;

  /* End of the array of DIDs */
  uint8 EndOfSortedDids = (uint8)(NumberOfPDIDs - 1U);

  /* create new pointers to handle the values instead of using indexes */
  uint8 CurrentDidToCompare;

  /* set the value for 'CurrentDidToCompare' both at the beginning and
     at the end of the for loop */
  CurrentDidToCompare = (StartOfArrayToSort + 1U);

  if(NumberOfPDIDs > 1U)
  {
    while (StartOfArrayToSort < EndOfSortedDids)
    {
      while (CurrentDidToCompare <= EndOfSortedDids)
      {
        if (BufferPDIDs[StartOfArrayToSort] == BufferPDIDs[CurrentDidToCompare])
        {
          /* if the currently compared DID and the previous have the same value, the
           * position of the current can be used to store another value which
           * could be different, so take the final value */
          BufferPDIDs[CurrentDidToCompare] = BufferPDIDs[EndOfSortedDids];
          EndOfSortedDids--;
          /* now the next to last value is the new last value */
        }
        else
        {
          /* if the current and the previous value are not equal,
           * compare the first and the one following the current value.
           * Repeat this until the end is reached */
          CurrentDidToCompare++;
        }
      }

      /* Repeat the procedure with a shorted array starting at the next element */
      StartOfArrayToSort++;
      CurrentDidToCompare = (StartOfArrayToSort + 1U);
    }
  }

  /* Adjust the request message length for the eliminated duplicates */
  return (EndOfSortedDids + 1U);
}

/* !LINKSTO Dcm.Dsn.IB.RDBPI.ValidateScheduler,1 */
STATIC FUNC(Dcm_NegativeResponseCodeType, DCM_CODE)
      Dcm_Dsp_ReadDataByPeriodicIdentifier_ValidateScheduler(
  P2CONST(uint8, AUTOMATIC, DCM_VAR) BufferPDIDs,
  uint8 NumberOfPDIDs
  )
{
  /* !LINKSTO SWS_Dcm_00851,1 */

  Dcm_NegativeResponseCodeType Nrc = DCM_E_GENERALREJECT;
  uint8 AlreadyScheduledPDIDs = 0U;
  uint8 IndexPDIDs = 0U;

  /* Make sure the number of concurrently scheduled periodic DIDs does not exceed the
   * configured limit. Take the already scheduled DIDs into account. */

  /* For all periodic DIDs within the current request */
  for (IndexPDIDs = 0U; IndexPDIDs < NumberOfPDIDs; IndexPDIDs++)
  {
    /* Periodic DIDs are in the range 0xF200 .. 0xF2FF */
    uint16 RequestedPDID  = 0xF200U | BufferPDIDs[IndexPDIDs];

    uint8 IndexSchedTable = 0U;

    /* Count the number of periodic DIDs in the request that are already scheduled */
    for (IndexSchedTable = Dcm_PeriodicDIDContext.HeadPeriodicDIDList;
         IndexSchedTable != DCM_PDID_SCHEDTABLE_INVALID_IDX;
         IndexSchedTable = Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable]
                            .IndexOfNextPeriodicDID
        )
    {
      if (RequestedPDID == Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].PDID)
      {
        AlreadyScheduledPDIDs++;
        break;
      }
    }
  }

  /* If there is less space available in the scheduler table than needed to add all requested
   * periodic DIDs */
  /* !LINKSTO SWS_Dcm_00851,1 */
  if ((Dcm_PeriodicDIDContext.NumberOfScheduledPDIDs + (uint8)(NumberOfPDIDs
                                                               - AlreadyScheduledPDIDs))
      > DCM_MAX_PERIODIC_DID_SCHEDULER
     )
  {
    /* Report RequestOutOfRange */
    Nrc = DCM_E_REQUESTOUTOFRANGE;
  }
  else
  {
    /* New periodic DIDs can be added to the scheduler */
    Nrc = DCM_E_POSITIVERESPONSE;
  }

  return Nrc;
}

/* !LINKSTO Dcm.Dsn.IB.RDBPI.SvcH.SvcCondCheck,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_SvcStart(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Dcm_NegativeResponseCodeType Nrc = DCM_E_GENERALREJECT;
  uint8 TransmissionMode = DCM_UDS_RDBPI_TRANSMISSIONMODE_STOPSENDING;

  /* Before anything else make sure the message is well-formed
   * (length, mode, nr. of PDIDs) */
  Nrc = Dcm_Dsp_ReadDataByPeriodicIdentifier_ValidateMessage(pMsgContext->reqData,
                                                             pMsgContext->reqDataLen);

  if (Nrc == DCM_E_POSITIVERESPONSE)
  {
    uint8 NewNumberOfPDIDs = 0U;
    /* Remove duplicate PDIDs from the request */
    NewNumberOfPDIDs
      = Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveDuplicates(&pMsgContext->reqData[1U],
                                                             (uint8)(pMsgContext->reqDataLen - 1U));

    /* Compute in a separate expression to avoid MISRA warning */
    NewNumberOfPDIDs = NewNumberOfPDIDs + 1U;

    pMsgContext->reqDataLen = NewNumberOfPDIDs;

    /* Get the requested transmission mode. */
    TransmissionMode = pMsgContext->reqData[0U];

    /* Checking the PDIDs doesn't need to be done when stopping them */
    /* !LINKSTO SWS_Dcm_01100_StopSendingNoVerification,1 */
    if (TransmissionMode != DCM_UDS_RDBPI_TRANSMISSIONMODE_STOPSENDING)
    {
      /* Make sure there is space in the scheduler table for new PDIDs - taking into account the
       * ones already present in the scheduler table */
      Nrc = Dcm_Dsp_ReadDataByPeriodicIdentifier_ValidateScheduler(&pMsgContext->reqData[1U],
                                            (uint8)(pMsgContext->reqDataLen - 1U));

      if (Nrc == DCM_E_POSITIVERESPONSE)
      {
        /* Make sure the request contains at least one PDID
         * that can be added to the periodic scheduler. */
        Nrc = Dcm_Dsp_ReadDataByPeriodicIdentifier_ValidatePDIDs(pMsgContext->dcmRxPduId,
                                         &pMsgContext->reqData[1U],
                                         (uint8)(pMsgContext->reqDataLen - 1U));
      }
    }
  }

  if (Nrc == DCM_E_POSITIVERESPONSE)
  {
    /* Remove one or more PDID(s) from the scheduler table */
    if (TransmissionMode == DCM_UDS_RDBPI_TRANSMISSIONMODE_STOPSENDING)
    {
      /* Just remove the DID(s) from the scheduler. No attempt is made to cancel
       * already started transmissions */

      /* !LINKSTO SWS_Dcm_01113,1, SWS_Dcm_01114,1, SWS_Dcm_01118,1 */
      Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveFromSchedulerTable(&pMsgContext->reqData[1U],
                                               (uint8)(pMsgContext->reqDataLen - 1U));
    }
    else
    {
      /* Add one or more PDID(s) to the scheduler table */
      PduIdType RxPduId = pMsgContext->dcmRxPduId;
     Dcm_Dsp_ReadDataByPeriodicIdentifier_AddToSchedulerTable(RxPduId, Dcm_AcceptedPDIDs,
                                                              Dcm_NumberOfAcceptedPDIDs,
                                                              TransmissionMode
                                                              );
    }
  }

  if (Nrc == DCM_E_POSITIVERESPONSE)
  {
    /* positive response does not contain data beyond the positive response SID */
    pMsgContext->resDataLen = 0U;
  }
  else
  {
#if (DCM_NUM_PROTOCOL > 1U)
    /* Get the current main connection of the RxPduId */
    Dcm_MainConnectionIndexType MainConnectionIndex
      = Dcm_RxPduIdConfig[pMsgContext->dcmRxPduId].MainConnectionIndex;
#endif

    /* Get the current context info used by Rx Pdu Id */
    CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo =
      &DCM_HSM_INFO(Dcm_MainConnectionConfig[MainConnectionIndex].ProtocolIndex);

#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
    /* The async service processor can access negativeResponseCode in parallel
     * from within a different task context than the HSM */

    /* ENTER critical section */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
#endif

    /* Because a mode rule can be configured to return any NRC, and
     * Dcm_ExternalSetNegResponse() verifies if the NRC is known, set
     * the NRC directly to avoid having to change the source code
     * along with the configuration */
    curHsmInfo->negativeResponseCode = Nrc;

#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
    /* LEAVE critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
#endif
  }

  /* this function NEVER fails and it has a return value only as a result to keep uniform the function signatures */
  return DCM_E_DONE;
}

/* Determine the next free slot in the transmission ring to sample the PDID to */
STATIC FUNC(uint16, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_GetFreeTransmissionRingSlot(
  void)
{
  uint16 FreeSlot = DCM_PDID_SCHEDTABLE_INVALID_IDX;
  uint16 RingIndex = 0U;
  boolean Found = FALSE;

  /* This can not fail since we verified earlier that there are available free slots
   * in the transmission ring */
  while ((RingIndex < DCM_PERIODIC_DID_TRANSMISSION_RING_ENTRIES)
         && (Found == FALSE)
        )
  {
     /* If the index in scheduler table is invalid we have a free one */
    if (Dcm_PeriodicDIDContext.TransmissionRing[RingIndex].IndexInSchedulerTable
        == DCM_PDID_SCHEDTABLE_INVALID_IDX
       )
    {
      FreeSlot = RingIndex;
      Found = TRUE;
    }
    else
    {
      Found = FALSE;
    }

    RingIndex++;
  }

  return FreeSlot;
}

/* !LINKSTO Dcm.Dsn.IB.RDBPI.ValidateOnePDID,2 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_ValidateOnePDID(
  PduIdType RxPduId,
  uint16 RequestedPDID,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc,
  boolean InitialPDIDConfig)
{
  /* !LINKSTO Dcm.Service.ReadDataByPeriodicId.SecLvlNotOk,1 */
  /* !LINKSTO Dcm.Service.ReadDataByPeriodicId.ModeRuleNotOk,1 */

  Dcm_CheckPeriodicDIDType CheckStatus = DCM_CHECK_PDID_NOT_OK;
  Std_ReturnType ReturnVal = E_NOT_OK;
  Dcm_NegativeResponseCodeType LocalNrc = *Nrc;

  CheckStatus = Dcm_Dsp_DidServices_CheckPeriodicDID(RequestedPDID, &LocalNrc);

  /* DIDs that do not exist or cannot be read in the current session will be ignored. However,
   * there should be at least one DID in the request message that can be read */
  if ((CheckStatus == DCM_CHECK_PDID_DOES_NOT_EXIST)
      || (CheckStatus == DCM_CHECK_PDID_UNSUPPORTED_SESSION)
     )
  {
    LocalNrc = DCM_E_POSITIVERESPONSE;
  }
  else
  {
    if (CheckStatus == DCM_CHECK_PDID_OK)
    {
      ReturnVal = E_OK;
    }
  }

#if ((DCM_DSP_USE_SERVICE_0X2C == STD_ON) && (DCM_DDDID_CHECK_SOURCES == STD_ON))
  /* If so configured also check the components of dynamically defined PDIDs */
  if (ReturnVal == E_OK)
  {
    /* Check if dynamically defined, and if so check for all configured components
     * the session, security level and mode conditions */
    /* !LINKSTO SWS_Dcm_01111,1, Dcm.PeriodicTransmission.DDDID.SecurityCheck,1 */
    ReturnVal = Dcm_Dsp_DidServices_ValidateDDDIDSources(RequestedPDID, &LocalNrc);
  }
#endif /* #if ((DCM_DSP_USE_SERVICE_0X2C == STD_ON) && (DCM_DDDID_CHECK_SOURCES == STD_ON)) */

  /* Do a test read to  see if the DID can actually be read considering ConditionCheckRead */
  /* !LINKSTO SWS_Dcm_01098,1 */
  if ((InitialPDIDConfig == TRUE) && (ReturnVal == E_OK))
  {
    /* Temporary buffer to sample the PDID to */
    uint8 TmpBuffer[DCM_PERIODIC_DID_SAMPLING_BUFFER_SIZE];

    /* Use the sampling buffer of a free transmission ring slot to store the read DID data */
    PeriodicReadDid_BufferCfg.BufferPtr = &TmpBuffer[0];
    PeriodicReadDid_BufferCfg.Size      = DCM_PERIODIC_DID_SAMPLING_BUFFER_SIZE;
    /* DCM_PERIODIC_DID_SAMPLING_BUFFER_SIZE includes the PDID as the first octet. Therefore
     * only DCM_PERIODIC_DID_SAMPLING_BUFFER_SIZE - 1 octets are available for payload data */
    PeriodicReadDid_BufferCfg.Occupied  = 1U;

    /* Initialize the reading process for the DID */
    Dcm_Dsp_DidServices_ReadDid_Init(&Dcm_ReadPeriodicDidContext,
                                     RequestedPDID,
                                     DCM_DID_OP_PERIODIC,
                                     DCM_DO_NOT_WRITE_DID_IDS,
                                     &PeriodicReadDid_BufferCfg,
                                     DCM_DO_NOT_EXECUTE_SESSION_SECURITY_MODERULE_CHECKS,
                                     RxPduId
                                     );

    /* Execute the reading process for the DID data */
    /* !LINKSTO Dcm.EB.ReadDataByPeriodicIdentifier.CheckBufferSize,1 */
    ReturnVal = Dcm_Dsp_DidServices_ReadDid_Execute(&Dcm_ReadPeriodicDidContext);

    /* !LINKSTO SWS_Dcm_01105,1 */
    if (E_OK != ReturnVal)
    {
        LocalNrc = DCM_DSP_CONTEXT_NRC(DCM_DSP_CONTEXT(&Dcm_ReadPeriodicDidContext));

        /* A locked reception resource should be treated the same as if the service request
         * returned pending. AS the RDBPI service support ONLY synchronous operation it is not
         * possible to return DCM_E_PENDING. The next most sensible thing to do in this case
         * is to return NRC 0x21 (BusyRepeatRequest) to indicate to the tester that currently
         * a resource is locked but could be free at the time of the repeated request */
        if (ReturnVal == DCM_E_RESOURCE_LOCKED)
        {
          LocalNrc = DCM_E_BUSYREPEATREQUEST;
        }
    }
  }
  *Nrc = LocalNrc;

  return ReturnVal;
}

STATIC FUNC(Dcm_NegativeResponseCodeType, DCM_CODE)
      Dcm_Dsp_ReadDataByPeriodicIdentifier_ValidatePDIDs(
  PduIdType RxPduId,
  P2CONST(uint8, AUTOMATIC, DCM_VAR) BufferPDIDs,
  uint8 NumberOfPDIDs)
{
  Dcm_NegativeResponseCodeType Nrc = DCM_E_GENERALREJECT;
  Std_ReturnType ReturnVal = E_NOT_OK;
  uint8 DidIdx = 0U;

  /* !LINKSTO Dcm.Dsn.IB.RDBPI.PeriodicDidsCondCheck,1 */
  /* Keep track of how many DIDs from the current request can be read, if any */
  Dcm_NumberOfAcceptedPDIDs = 0U;

  /* For all periodic DIDs within the current request */
  for (DidIdx = 0U; DidIdx < NumberOfPDIDs; DidIdx++)
  {
    /* Periodic DIDs are in the range 0xF200 .. 0xF2FF */
    uint16 RequestedPDID = 0xF200U | BufferPDIDs[DidIdx];

    /* !LINKSTO Dcm.ReadDataById.SupportedDid.StaticPeriodicDID, 1 */
    /* Verify the current PDID */
    ReturnVal = Dcm_Dsp_ReadDataByPeriodicIdentifier_ValidateOnePDID(RxPduId,
                                                                     RequestedPDID,
                                                                     &Nrc,
                                                                     TRUE
                                                                     );
    if (ReturnVal == E_OK)
    {
      /* To avoid checking this all again when actually adding the PDIDs,
       * store the accepted PDIDs in a temporary array */
      Dcm_AcceptedPDIDs[Dcm_NumberOfAcceptedPDIDs] = BufferPDIDs[DidIdx];
      Dcm_NumberOfAcceptedPDIDs++;
    }
    else
    {
      if (Nrc != DCM_E_POSITIVERESPONSE)
      {
        break;
      }
      /* else: DIDs that cannot be read in the current session will be ignored */
    }
  }

  /* See if no PDID in this request message can be read.
   * There should be at least one DID in the request message that can be read */
  /* !LINKSTO Dcm.Service.ReadDataByPeriodicId.SessionNotOk,1 */
  if ((Dcm_NumberOfAcceptedPDIDs == 0U) && (Nrc == DCM_E_POSITIVERESPONSE))
  {
    /* Otherwise the negative response code should be set to RequestOutOfRange */
    Nrc = DCM_E_REQUESTOUTOFRANGE;
  }

  return Nrc;
}

/* !LINKSTO Dcm.Dsn.IB.RDBPI.PeriodicDidsCondCheck,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_ValidateConditions(void)
{
  uint8 IndexSchedTable = 0U;

  /* For all scheduled periodic DIDs */
  for (IndexSchedTable = Dcm_PeriodicDIDContext.HeadPeriodicDIDList;
       IndexSchedTable != DCM_PDID_SCHEDTABLE_INVALID_IDX;
       IndexSchedTable = Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable]
                          .IndexOfNextPeriodicDID
       )
  {
    PduIdType RxPduId = Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].RxPduId;
    uint16 RequestedPDID = Dcm_PeriodicDIDContext.SchedulerTable[IndexSchedTable].PDID;
    Dcm_NegativeResponseCodeType Nrc = DCM_E_POSITIVERESPONSE;
    Std_ReturnType ReturnVal = E_OK;

    /* Check if this PDID can be read under current session, security level and mode conditions */
    ReturnVal = Dcm_Dsp_ReadDataByPeriodicIdentifier_ValidateOnePDID(RxPduId,
                                                                     RequestedPDID,
                                                                     &Nrc,
                                                                     FALSE
                                                                     );
    if (ReturnVal != E_OK)
    {
      /* Remove the periodic DID as it can no longer be read under current conditions */
      uint8 Buffer[1];
      Buffer[0] = (uint8)RequestedPDID;
      /* Does not interfere with the iteration over the list, as the next pointer for the entry
       * to be removed is left unchanged */
      Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveFromSchedulerTable(Buffer, 1U);
    }
  }
}

/* !LINKSTO Dcm.Dsn.IB.RDBPI.RemoveSample,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveSample(
  uint8 IndexSchedTable,
  uint8 Which)
{
  uint16 RingIndex;
  uint16 LastRingIndex;

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVESAMPLE_ENTRY(IndexSchedTable, Which);

  LastRingIndex = Dcm_PeriodicDIDContext.HeadTransmissionRing;

  /* If the periodic DID to be disabled happens to be already in the transmission ring
   * invalidate all samples that belong to it, so that they will not be sent and the DID
   * sample data they contain is ignored. */
  for (RingIndex = Dcm_PeriodicDIDContext.HeadTransmissionRing;
      RingIndex != DCM_PDID_TRANSRING_INVALID_IDX;
      RingIndex = Dcm_PeriodicDIDContext.TransmissionRing[RingIndex].SendNext
      )
  {
    /* Is it the sample that should be removed? */
    if (Dcm_PeriodicDIDContext.TransmissionRing[RingIndex].IndexInSchedulerTable
        == IndexSchedTable
       )
    {
      /* Mark the transmission ring entry as invalid. It will not be sent out and the
       * DID sample data within it will be considered trash */
      Dcm_PeriodicDIDContext.TransmissionRing[RingIndex].IndexInSchedulerTable
        = DCM_PDID_SCHEDTABLE_INVALID_IDX;

#if ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U))
      /* Invalidate the associated TxConnectionId */
      Dcm_PeriodicDIDContext.TransmissionRing[RingIndex].TxConnectionId = DCM_PDID_TX_CONNECTION_ID_INVALID;
#endif /*#if ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U))*/

      /* Links last item's next index to the current item's next index so the item is taken
       * out of the list */
      Dcm_PeriodicDIDContext.TransmissionRing[LastRingIndex].SendNext
        = Dcm_PeriodicDIDContext.TransmissionRing[RingIndex].SendNext;

      /* !LINKSTO SWS_Dcm_01104,1 */
      if (Dcm_PeriodicDIDContext.HeadTransmissionRing
          == Dcm_PeriodicDIDContext.TailTransmissionRing)
      {
        Dcm_PeriodicDIDContext.HeadTransmissionRing = DCM_PDID_TRANSRING_INVALID_IDX;
        Dcm_PeriodicDIDContext.TailTransmissionRing = DCM_PDID_TRANSRING_INVALID_IDX;
      }
      else
      {
        /* Adjust head of the list in case it is necessary */
        if (RingIndex == Dcm_PeriodicDIDContext.HeadTransmissionRing)
        {
          Dcm_PeriodicDIDContext.HeadTransmissionRing
            = Dcm_PeriodicDIDContext.TransmissionRing[RingIndex].SendNext;
        }

        /* Adjust tail of the transmission ring in case it is necessary */
        if (RingIndex == Dcm_PeriodicDIDContext.TailTransmissionRing)
        {
          Dcm_PeriodicDIDContext.TailTransmissionRing = LastRingIndex;
        }
      }

      /* Keep track of the removed sample */
      Dcm_PeriodicDIDContext.NumberOfTransmittableSamples--;

      /* If only the removal of one element was requested just stop */
      if (Which == DCM_PDID_TRANSRING_REMOVE_FIRST)
      {
        break;
      }
    }
    else
    {
      /* Keep track of previous entry in transmission ring in case it is needed  */
      LastRingIndex = RingIndex;
    }
  }

  DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVESAMPLE_EXIT();
}


#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if (DCM_DSP_USE_SERVICE_0X2A == STD_ON) */

/*==================[end of file]================================================================*/

