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


/* Implementation of the state handler functions of the state machine
 * Dcm.
 *
 * This file contains the implementation of the state functions.  It
 * is generated but must be edited to implement the real actions.  If the
 * state machine model is updated and the code generator regenerates the
 * files the user must manually merge the changes with any existing hand
 * written code.
 */

/*  MISRA-C:2004 Deviation List
 *
 *
 *  MISRA-1) Deviated Rule: 6.3 (advisory)
 *    "'typedefs' that indicate size and signedness should be used in place of the basic types."
 *
 *    Reason: The data type is required here since the structure used provides this element
 *            using this type. Refer to this for the main reason.
 */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 20.7 (required)
  *     "Expressions resulting from the expansion of macro parameters shall
  *     be enclosed in parentheses."
  *
  *     Reason:
  *     The macro is used in function parameter declarations and definitions
  *     where the number of parentheses matter.
  *
  *  MISRAC2012-2) Deviated Rule: D4.6 (advisory)
  *     "typedefs that indicate size and signedness should be used in place of the basic numerical
  *     types."
  *
  *     Reason:
  *     The data type is required here since the structure used provides this element
  *     using this type. Refer to this for the main reason.
  */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * This warning is a false positive. The false condition (which terminates the loop) is reached.
 * This is proven by the coverage metrics and by the fact that the while loop is not infinite.
 */
/*==================[inclusions]============================================*/

#include <Dcm_Trace.h>
#include <Std_Types.h>                            /* AUTOSAR standard types */
#include <TSAutosar.h>

#include <Dcm_Hsm.h>                              /* state machine driver interface */
#include <Dcm_HsmDcm.h>                           /* public statechart model definitions */
#include <Dcm_HsmDcmInt.h>                        /* internal statechart model definitions */
#include <Dcm_Dsl_CommunicationServices.h>        /* Comstack stuff */
#include <Dcm_Dsl_DiagnosticSessionHandler.h>     /* Dcm Dsl Diagnostic Session Handler header file */
#include <Dcm_Dsl_Supervisor_ProtocolHandling.h>  /* Dcm Dsl Supervisor Main Connection Handling header file */
#include <Dcm_Dsl_ComMChannelStateManager.h>      /* Dcm Dsl CComMChannelStateManager header file */
#include <Dcm_Int.h>                              /* Module intenal definitions */
#include <Dcm_DspExternal.h>                      /* Callbacks provided by Central Diag SwC */
/* do not indirectly include PduR_SymbolicNames_PBcfg.h via PduR_Dcm.h */
/* PDUR_NO_PBCFG_REQUIRED is a switch which activates some of options/definitions in PDUR */
#if (defined PDUR_NO_PBCFG_REQUIRED)
  #error "PDUR_NO_PBCFG_REQUIRED is already defined"
#endif
#define PDUR_NO_PBCFG_REQUIRED

#include <PduR_Dcm.h>                             /* PduR interface for Dcm */
#include <ComM_Dcm.h>                             /* ComM interface for Dcm */
#include <SchM_Dcm.h>                             /* SchM interface for Dcm */
#include <Dcm_Dsl_Supervisor.h>                   /* Dcm Dsl Supervisor header file */

#if (DCM_DEM_CLIENT_REF_USED == STD_ON)
#include <Dem.h>
#endif /* #if (DCM_DEM_CLIENT_REF_USED == STD_ON) */

#if (DCM_BSWM_API_ENABLED == STD_ON)
#include <BswM_DCM.h>                             /* BswM interface for Dcm */
#endif
#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
#include <Dcm_Dsp_SvcH_ReadDataByPeriodicIdentifier.h>
#endif
#if (DCM_DSP_USE_SERVICE_0X2F == STD_ON)
#include <Dcm_Dsp_SvcH_InputOutputControlByIdentifier.h>
#endif
#if ((DCM_DSP_USE_SERVICE_0X28 == STD_ON) || (DCM_MODEDECLARATION_SUPPORT == STD_OFF))
#include <Dcm_Dsp_SvcH_CommunicationControl.h>
#endif
#if (DCM_DSP_USE_ROESERVICES == STD_ON)
#include <Dcm_Dsl_RoeServices.h>
#endif /* (DCM_DSP_USE_ROESERVICES == STD_ON) */
/*==================[macros]================================================*/

/* JumpFromBootloader positive response length with timing values */
#if (defined DCM_JUMPFROMBL_RESPONSELENGTH_TIMING_VALUES)
  #error "DCM_JUMPFROMBL_RESPONSELENGTH_TIMING_VALUES is already defined"
#endif
#define DCM_JUMPFROMBL_RESPONSELENGTH_TIMING_VALUES     (6U)

/* JumpFromBootloader positive response length without timing values */
#if (defined DCM_JUMPFROMBL_RESPONSELENGTH_NO_TIMING_VALUES)
  #error "DCM_JUMPFROMBL_RESPONSELENGTH_NO_TIMING_VALUES is already defined"
#endif
#define DCM_JUMPFROMBL_RESPONSELENGTH_NO_TIMING_VALUES  (2U)

/* JumpFromBootloader positive response for ECU Reset */
#if (defined DCM_JUMPFROMBL_RESPONSELENGTH_ENABLERAPIDPOWERSHUTDOWN_ON)
  #error "DCM_JUMPFROMBL_RESPONSELENGTH_ENABLERAPIDPOWERSHUTDOWN_ON is already defined"
#endif
#define DCM_JUMPFROMBL_RESPONSELENGTH_ENABLERAPIDPOWERSHUTDOWN_ON   (3U)

#if (defined DCM_JUMPFROMBL_RESPONSELENGTH_ENABLERAPIDPOWERSHUTDOWN_OFF)
  #error "DCM_JUMPFROMBL_RESPONSELENGTH_ENABLERAPIDPOWERSHUTDOWN_OFF is already defined"
#endif
#define DCM_JUMPFROMBL_RESPONSELENGTH_ENABLERAPIDPOWERSHUTDOWN_OFF  (2U)

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* Deviation MISRAC2012-1 <+2> */
STATIC FUNC(void, DCM_CODE) RespPendInFirstCycle(DCM_PDL_SF(const uint8 instIdx));
STATIC FUNC(boolean, DCM_CODE) Dcm_SuppressResponse(const uint8 instIdx);

/** \brief Check if is fullfill condition to return the NRC GeneralReject
 **
 ** This function can be called to determine if is fullfill condition to return
 ** the NRC GeneralReject on both cases: - FORCE_RCRRP
 **                                      - timeout PENDING
 **
 ** \param[in] instIdx  Hsm instance
 **
 ** \Reentrancy{Non-reentrant}
 ** \Synchronicity{Synchronous}
 **
 */
/* Deviation MISRAC2012-1 <+2> */
STATIC FUNC(boolean, DCM_CODE) Dcm_CheckForGeneralRejectConditions(DCM_PDL_SF(const uint8 instIdx));
STATIC FUNC(boolean, DCM_CODE) Dcm_BootloaderResponseRequired(DCM_PDL_SF(const uint8 instIdx));
STATIC FUNC(boolean, DCM_CODE) Dcm_Roe_CheckForTxEnable(void);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/
/*==================[external function definitions]=========================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* ************************ state functions ******************************* */

/* ************************************************************************
 * State: Top
 * Parent state: none
 * Init substate: OutOfService
 * Transitions originating from this state:
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfTopEntry(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* entry action 'InitHsmVariables();' */
  /* By default ServiceProcessingDone should indicate availablity for processing. */
  DCM_HSM_INFO(instIdx).CancellationConditions.ServiceProcessingDone = TRUE;

  /* Initialize status information for each configured protocol */
  DBG_DCM_HSMDCMSFTOPENTRY_ENTRY(DCM_INST(instIdx));

  /* Stop the retry timer */
  DCM_HSM_INFO(instIdx).retryTimer = (uint16)0U;
#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
  /* Set confirmed diagnostics session to invalid */
  DCM_HSM_INFO(instIdx).confirmedDiagSession = DCM_INVALID_SESSION;

  /* Initialize the requested Reset Mode */
  DCM_HSM_INFO(instIdx).requestedResetMode = DCM_NO_RESET;

  /* Initialize the confirmed Reset Mode */
  DCM_HSM_INFO(instIdx).confirmedResetMode = DCM_NO_RESET;
#endif

  /* Initialize the Jump to bootloader support status flag */
  DCM_HSM_INFO(instIdx).jumpToBLSupport = FALSE;

  /* Initialize the Jump to bootloader mode execution pending status flag */
  DCM_HSM_INFO(instIdx).jumpToBLExecution = DCM_PREPARE_JUMP;

  /* Initialize the message type flag */
  DCM_HSM_INFO(instIdx).msgContext.msgAddInfo.requestOrigin = DCM_INVALID_ORIGIN;

  /* Initialize 'Response Pending' transmission status */
  DCM_HSM_INFO(instIdx).responsePendingTxStatus = DCM_RP_NO_DATA;

  /* Initialize the initial request validation status */
  DCM_HSM_INFO(instIdx).requestValidationStatus = FALSE;

  /* Initialize the RxConnectionId */
  DCM_HSM_INFO(instIdx).RxConnectionId = DCM_INVALID_USER_ID;

  DBG_DCM_HSMDCMSFTOPENTRY_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: WaitForFullCom
 * Parent state: Top
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) RETRY[]/if (RetryTimer == 0) ReassessChannelState
 * 2) WaitForFullCom -> NormalSending: BL_TX_RESPONSE[ResponseRequired]/Fill response
 * 3) WaitForFullCom -> Finishing: BL_TX_RESPONSE[!ResponseRequired]/
 *    Dcm_Dsl_Supervisor_ProtocolFree();
 */
/* Deviation MISRAC2012-1 <+2> */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfWaitForFullComAction1(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* action 'if (RetryTimer == 0) ReassessChannelState'
   * for RETRY[]/...
   * internal transition */

  Dcm_ComMChannelOperationStateType ComMChnRxState = DCM_COMM_OPERATION_DISABLED;
  Dcm_ComMChannelOperationStateType ComMChnTxState = DCM_COMM_OPERATION_DISABLED;

  Dcm_RxPduIdInfoType RxPduIdInfo;
  Dcm_Dsl_RxConnectionGetRxPduIdInformation(DCM_HSM_INFO(instIdx).NextJob.JobRxConnectionId, &RxPduIdInfo);
  /* ENTER critical section
     The paired reading of Dcm_TxEnabled and Dcm_RxEnabled must be protected
     from preemption */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  /* Check if Full-Communication is enabled on the corresponding 'channelIdIndex' */
  ComMChnRxState = Dcm_Dsl_ComMChannelStateManager_GetRxState(RxPduIdInfo.ComMChannelId);
  ComMChnTxState = Dcm_Dsl_ComMChannelStateManager_GetTxState(RxPduIdInfo.ComMChannelId);

  /* LEAVE critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  if (
      ((DCM_COMM_OPERATION_ENABLED == ComMChnRxState) &&
       (DCM_COMM_OPERATION_ENABLED == ComMChnTxState))
     )
  {
    DCM_HSM_INFO(instIdx).retryTimer = 0U;

    (void)DCM_HSMEMITTOSELFINST_WRAPPER(&Dcm_HsmScDcm,
      DCM_CUR_INST_ID(instIdx),
      DCM_HSM_DCM_EV_BL_TX_RESPONSE);
  }
  else
  {
#if (DCM_COMM_MULTICORE_ENABLED == STD_ON)
    if (FALSE == Dcm_Dsl_ComMChannelStateManager_GetAndResetOperationFailedState())
#endif /* #if (DCM_COMM_MULTICORE_ENABLED == STD_ON) */
    {
      /* No failure to activate ComMChannel detected, we wait some more. */
      DCM_HSM_INFO(instIdx).retryTimer = 1U;
    }
#if (DCM_COMM_MULTICORE_ENABLED == STD_ON)
    else
    {
      /* ComM operation has failed. At this point we abandon sending a response and
         go on with a silent boot up. Switch the job type to a request with no response. */

      /* !LINKSTO Dcm.EB.CommunicationModeHandling.SchMCallComMDcmActiveDiagnostic.ResultNotOkAtPostBootResponse.NoResponse,1 */
      Dcm_HsmInfo[DCM_INST(instIdx)].NextJob.JobRequestType = DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST;

      DCM_HSM_INFO(instIdx).retryTimer = 0U;

      (void)DCM_HSMEMITTOSELFINST_WRAPPER(&Dcm_HsmScDcm,
      DCM_CUR_INST_ID(instIdx),
      DCM_HSM_DCM_EV_BL_TX_RESPONSE);
    }
#endif /* #if (DCM_COMM_MULTICORE_ENABLED == STD_ON) */
  }
}
/* Deviation MISRAC2012-1 <+2> */
FUNC(boolean, DCM_CODE) Dcm_HsmDcmSfWaitForFullComGuard2(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* guard condition 'ResponseRequired'
   * for BL_TX_RESPONSE[...]/Fill response
   * external transition to state NormalSending */

  boolean ResponseRequired;

  DBG_DCM_HSMDCMSFWAITFORFULLCOMGUARD2_ENTRY(DCM_INST(instIdx));

  ResponseRequired = Dcm_BootloaderResponseRequired(DCM_PL_SF(instIdx));

  DBG_DCM_HSMDCMSFWAITFORFULLCOMGUARD2_EXIT(ResponseRequired, DCM_INST(instIdx));
  return ResponseRequired;
}

/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfWaitForFullComAction2(
  DCM_PDL_SF(const uint8 instIdx))
{
   /* action 'Fill response'
   * for BL_TX_RESPONSE[ResponseRequired]/...
   * external transition to state NormalSending */

#if (DCM_TIMING_VALUES_IN_POSITIVE_RESPONSE == STD_ON)
  uint16 P2StarTime;
  boolean SessionIndexFound = FALSE;
  uint8_least SessionIndex = 0;
  uint8 sesTabIdxNew = 0U;
  /* Get SesTabConfig info */
  P2CONST(Dcm_SesTabConfigType, AUTOMATIC, DCM_CONST) sesTabConfig =
    &Dcm_SesTabConfig[DCM_CUR_INST_ID(instIdx)];

#endif /* #if (DCM_TIMING_VALUES_IN_POSITIVE_RESPONSE == STD_ON) */

  /* get the context information current instance */
  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo
    = &DCM_HSM_INFO(instIdx);

  const uint8 currentServiceID    = curHsmInfo->NextJob.JobRxBuffer.BufferPtr[0U];
#if (DCM_DSP_SUBSERVICE_ENABLERAPIDPOWERSHUTDOWN_0x04 == STD_ON)
  const uint8 currentSubserviceID = curHsmInfo->NextJob.JobRxBuffer.BufferPtr[1U];
#endif

  DBG_DCM_HSMDCMSFWAITFORFULLCOMACTION2_ENTRY(DCM_INST(instIdx));

#if (DCM_TIMING_VALUES_IN_POSITIVE_RESPONSE == STD_ON)
  if (DCM_SID_SESSION_CONTROL == currentServiceID)
  {
    /* getting new session table index for the new session control */
    /* Deviation TASKING-1 */
    while (FALSE == SessionIndexFound)
    {
      if ((sesTabConfig->SessionEntries[SessionIndex]).SesCtrl == curHsmInfo->NextJob.JobRxBuffer.BufferPtr[1U])
      {
        SessionIndexFound = TRUE;
        sesTabIdxNew = (uint8)SessionIndex;
      }
      else
      {
        SessionIndex++;
      }
    }
  }
#endif /* #if (DCM_TIMING_VALUES_IN_POSITIVE_RESPONSE == STD_ON) */

  /* Aquiring the transmission buffer for this protocol. This buffer should always be free. */
  (void)Dcm_Dsl_BufferAllocate(DCM_PROTOCOL_ID_FROM_INDEX(DCM_CUR_INST_ID(instIdx)),
                               DCM_BUFFER_TX_NORMAL,
                               DCM_BUFFER_IGNORE_BUFFER_SIZE,
                               &(curHsmInfo->TxBuffer));

  /* Assembling positive response */
  /* NextJob is required here, since the data for the current job is updated only in the
     InServiceEntry function */
  curHsmInfo->TxBuffer.BufferPtr[0U] = curHsmInfo->NextJob.JobRxBuffer.BufferPtr[0U] | DCM_SID_RESPONSE_BIT;
  curHsmInfo->TxBuffer.BufferPtr[1U] = curHsmInfo->NextJob.JobRxBuffer.BufferPtr[1U];

  if (DCM_SID_SESSION_CONTROL == currentServiceID)
  {
#if (DCM_TIMING_VALUES_IN_POSITIVE_RESPONSE == STD_ON)
    /* !LINKSTO EB_Dcm767_DiagnosticSession_TimingValues,1 */
    P2StarTime = (uint16)((sesTabConfig->SessionEntries[sesTabIdxNew].P2StarMaxTime *
                   DCM_TASK_TIME)/ DCM_P2STARTIME_RESOLUTION);

    curHsmInfo->TxBuffer.BufferPtr[2U] =
     (uint8)((sesTabConfig->SessionEntries[sesTabIdxNew].P2MaxTime *
          DCM_TASK_TIME) >> DCM_BYTE_SWAPPING);
    curHsmInfo->TxBuffer.BufferPtr[3U] = (uint8)(sesTabConfig->SessionEntries[
          sesTabIdxNew].P2MaxTime * DCM_TASK_TIME);
    curHsmInfo->TxBuffer.BufferPtr[4U] = (uint8)(P2StarTime >> DCM_BYTE_SWAPPING);
    curHsmInfo->TxBuffer.BufferPtr[5U] = (uint8)(P2StarTime);

    /* The response length always has to be adjusted by subtracting one byte for the
       service ID. The reason behind this is that service handlers will only
       write starting from the position after the positive or negative response code/identifier
       and this routine will act in the same fashion so as to create a similar message context
       structure. */
    curHsmInfo->msgContext.resDataLen = DCM_JUMPFROMBL_RESPONSELENGTH_TIMING_VALUES - 1U;
    curHsmInfo->TxBuffer.FilledLength = DCM_JUMPFROMBL_RESPONSELENGTH_TIMING_VALUES;
#else
    /* !LINKSTO EB_Dcm767_DiagnosticSession_NoTimingValues,1 */
    curHsmInfo->msgContext.resDataLen  = DCM_JUMPFROMBL_RESPONSELENGTH_NO_TIMING_VALUES - 1U;
    curHsmInfo->TxBuffer.FilledLength = DCM_JUMPFROMBL_RESPONSELENGTH_NO_TIMING_VALUES;
#endif
  }
  else
  {
    DCM_PRECONDITION_ASSERT( DCM_SID_ECU_RESET == currentServiceID, DCM_INTERNAL_API_ID);

#if (DCM_DSP_SUBSERVICE_ENABLERAPIDPOWERSHUTDOWN_0x04 == STD_ON)
    /* !LINKSTO EB_Dcm767_EcuReset_EnableRapidPowerShutDown,1 */
    if (DCM_ENABLE_RAPID_POWER_SHUTDOWN_RESET == currentSubserviceID)
    {
      curHsmInfo->TxBuffer.BufferPtr[2U] = DCM_DSP_POWER_DOWN_TIME;

      curHsmInfo->msgContext.resDataLen = DCM_JUMPFROMBL_RESPONSELENGTH_ENABLERAPIDPOWERSHUTDOWN_ON - 1U;
      curHsmInfo->TxBuffer.FilledLength = DCM_JUMPFROMBL_RESPONSELENGTH_ENABLERAPIDPOWERSHUTDOWN_ON;
    }
    else
    {
      curHsmInfo->msgContext.resDataLen = DCM_JUMPFROMBL_RESPONSELENGTH_ENABLERAPIDPOWERSHUTDOWN_OFF - 1U;
      curHsmInfo->TxBuffer.FilledLength = DCM_JUMPFROMBL_RESPONSELENGTH_ENABLERAPIDPOWERSHUTDOWN_OFF;
    }
#else
      /* !LINKSTO EB_Dcm767_EcuReset_NoEnableRapidPowerShutDown, 1 */
      curHsmInfo->msgContext.resDataLen = DCM_JUMPFROMBL_RESPONSELENGTH_ENABLERAPIDPOWERSHUTDOWN_OFF - 1U;
      curHsmInfo->TxBuffer.FilledLength = DCM_JUMPFROMBL_RESPONSELENGTH_ENABLERAPIDPOWERSHUTDOWN_OFF;
#endif
  }
  /* NextJob is required here, since the data for the current job is updated only in the
     InServiceEntry function */
  curHsmInfo->msgContext.idContext = curHsmInfo->NextJob.JobRxBuffer.BufferPtr[0];

  DBG_DCM_HSMDCMSFWAITFORFULLCOMACTION2_EXIT(DCM_INST(instIdx));
}

/* Deviation MISRAC2012-1 <+2> */
FUNC(boolean, DCM_CODE) Dcm_HsmDcmSfWaitForFullComGuard3(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* guard condition '!ResponseRequired'
   * for BL_TX_RESPONSE[...]/Dcm_Dsl_Supervisor_ProtocolFree();
   * external transition to state Finishing */

  boolean ResponseNotRequired;

  DBG_DCM_HSMDCMSFWAITFORFULLCOMGUARD1_ENTRY(DCM_INST(instIdx));

  ResponseNotRequired = !(Dcm_BootloaderResponseRequired(DCM_PL_SF(instIdx)));

  DBG_DCM_HSMDCMSFWAITFORFULLCOMGUARD1_EXIT(ResponseNotRequired, DCM_INST(instIdx));
  return ResponseNotRequired;
}

/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfWaitForFullComAction3(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* action 'Dcm_Dsl_Supervisor_ProtocolFree();'
   * for BL_TX_RESPONSE[!ResponseRequired]/...
   * external transition to state Finishing */

  DBG_DCM_HSMDCMSFWAITFORFULLCOMACTION1_ENTRY(DCM_INST(instIdx));

#if ((DCM_HSM_INST_MULTI_ENABLED == STD_ON) && (DCM_DBG_ENABLED == STD_OFF))
  TS_PARAM_UNUSED(instIdx);
#endif
  /* Update the TxConnectionId at this point, since an updated value for this variable
   * has to be provided in later stages in the case of a warm start (jump from Bootloader) */
  DCM_HSM_INFO(instIdx).TxConnectionId = DCM_HSM_INFO(instIdx).NextJob.JobTxConnectionId;

  DCM_HSM_INFO(instIdx).RxConnectionId = DCM_HSM_INFO(instIdx).NextJob.JobRxConnectionId;

  /* As processing is now finished, reset the flag for current processing in execution */
  Dcm_Dsl_Supervisor_ProtocolFree(
    DCM_PROTOCOL_ID_FROM_INDEX(DCM_CUR_INST_ID(instIdx)));

  DBG_DCM_HSMDCMSFWAITFORFULLCOMACTION1_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: OutOfService
 * Parent state: Top
 * Init substate: OosInit
 * Transitions originating from this state:
 * 1) OutOfService -> PeriodicResponseProcessing: RDBPI_TYPE1[TxEnabled]/
 * 2) OutOfService -> OutOfService: RDBPI_TYPE1[!TxEnabled]/Free Rx Buffer;
 *    Dcm_Dsl_Supervisor_ProtocolFree();
 * 3) OutOfService -> PeriodicProcessingUUDT: RDBPI_TYPE2[TxEnabled]/
 * 4) OutOfService -> OutOfService: RDBPI_TYPE2[!TxEnabled]/Free Rx Buffer;
 *    Dcm_Dsl_Supervisor_ProtocolFree();
 * 5) OutOfService -> RequestProcessing: REQUEST_RECEIVED[]/
 * 6) OutOfService -> SendRpNormal: REQUEST_RECEIVED_RCRRP[]/
 * 7) OutOfService -> ROEResponseProcessing: ROE[TxEnabled]/Set-up Protocol
 * 8) OutOfService -> OutOfService: ROE[!TxEnabled]/Free Rx Buffer;
 *    Dcm_Dsl_Supervisor_ProtocolFree();
 */

/* Deviation MISRAC2012-1 <+2> */
FUNC(boolean, DCM_CODE) Dcm_HsmDcmSfOutOfServiceGuard1(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* guard condition 'TxEnabled'
   * for RDBPI_TYPE1[...]/
   * external transition to state PeriodicResponseProcessing */

  boolean guardResult = FALSE;

  DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD1_ENTRY(DCM_INST(instIdx));
  TS_PARAM_UNUSED(DCM_INST(instIdx));

#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
  guardResult = Dcm_Dsp_ReadDataByPeriodicIdentifier_IsPeriodicTxChannelEnabled();
#endif /* (DCM_DSP_USE_SERVICE_0X2A == STD_ON) */
  DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD1_EXIT(guardResult, DCM_INST(instIdx));

  return guardResult;
}
/* Deviation MISRAC2012-1 <+2> */
FUNC(boolean, DCM_CODE) Dcm_HsmDcmSfOutOfServiceGuard2(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* guard condition '!TxEnabled'
   * for RDBPI_TYPE1[...]/Free Rx Buffer; Dcm_Dsl_Supervisor_ProtocolFree()
   * external transition to state OutOfService */

  boolean guardResult = FALSE;

  DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD2_ENTRY(DCM_INST(instIdx));
  TS_PARAM_UNUSED(DCM_INST(instIdx));

#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
  guardResult = !(Dcm_Dsp_ReadDataByPeriodicIdentifier_IsPeriodicTxChannelEnabled());
#endif /* (DCM_DSP_USE_SERVICE_0X2A == STD_ON) */
  DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD2_EXIT(guardResult, DCM_INST(instIdx));

  return guardResult;
}
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfOutOfServiceAction2(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* action 'Free Rx Buffer; Dcm_Dsl_Supervisor_ProtocolFree()'
   * for RDBPI_TYPE1[!TxEnabled]/...
   * external transition to state OutOfService */

  DBG_DCM_HSMDCMSFOUTOFSERVICEACTION2_ENTRY(DCM_PDL_SF(instIdx));

  /* Release rx and tx buffers */
  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  /* NextJob is required here, since the data for the current job is updated only in the
     InServiceEntry function */
  (void)Dcm_Dsl_BufferFree(DCM_PROTOCOL_ID_FROM_INDEX(DCM_CUR_INST_ID(instIdx)),
                           DCM_HSM_INFO(instIdx).NextJob.JobRxBuffer.BufferId);

  /* LEAVE critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  Dcm_Dsl_Supervisor_ProtocolFree(
    DCM_PROTOCOL_ID_FROM_INDEX(DCM_CUR_INST_ID(instIdx)));

  DBG_DCM_HSMDCMSFOUTOFSERVICEACTION2_EXIT(DCM_PDL_SF(instIdx));
}
/* Deviation MISRAC2012-1 <+2> */
FUNC(boolean, DCM_CODE) Dcm_HsmDcmSfOutOfServiceGuard3(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* guard condition 'TxEnabled'
   * for RDBPI_TYPE2[...]/
   * external transition to state PeriodicProcessingUUDT */

  boolean guardResult = FALSE;

  DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD3_ENTRY(DCM_INST(instIdx));
  TS_PARAM_UNUSED(DCM_INST(instIdx));

#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
  guardResult = Dcm_Dsp_ReadDataByPeriodicIdentifier_IsPeriodicTxChannelEnabled();
#endif /* (DCM_DSP_USE_SERVICE_0X2A == STD_ON) */
  DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD3_EXIT(guardResult, DCM_INST(instIdx));

  return guardResult;
}
/* Deviation MISRAC2012-1 <+2> */
FUNC(boolean, DCM_CODE) Dcm_HsmDcmSfOutOfServiceGuard4(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* guard condition '!TxEnabled'
   * for RDBPI_TYPE2[...]/Free Rx Buffer; Dcm_Dsl_Supervisor_ProtocolFree()
   * external transition to state OutOfService */

  boolean guardResult = FALSE;

  DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD4_ENTRY(DCM_INST(instIdx));
  TS_PARAM_UNUSED(DCM_INST(instIdx));

#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
  guardResult = !(Dcm_Dsp_ReadDataByPeriodicIdentifier_IsPeriodicTxChannelEnabled());
#endif /* (DCM_DSP_USE_SERVICE_0X2A == STD_ON) */
  DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD4_EXIT(guardResult, DCM_INST(instIdx));

  return guardResult;
}
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfOutOfServiceAction4(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* action 'Free Rx Buffer; Dcm_Dsl_Supervisor_ProtocolFree()'
   * for RDBPI_TYPE2[!TxEnabled]/...
   * external transition to state OutOfService */

  DBG_DCM_HSMDCMSFOUTOFSERVICEACTION4_ENTRY(DCM_PDL_SF(instIdx));

  /* Release rx and tx buffers */
  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  /* NextJob is required here, since the data for the current job is updated only in the
     InServiceEntry function */
  (void)Dcm_Dsl_BufferFree(DCM_PROTOCOL_ID_FROM_INDEX(DCM_CUR_INST_ID(instIdx)),
                           DCM_HSM_INFO(instIdx).NextJob.JobRxBuffer.BufferId);

  /* LEAVE critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  Dcm_Dsl_Supervisor_ProtocolFree(
    DCM_PROTOCOL_ID_FROM_INDEX(DCM_CUR_INST_ID(instIdx)));

  DBG_DCM_HSMDCMSFOUTOFSERVICEACTION4_EXIT(DCM_PDL_SF(instIdx));
}
/* Deviation MISRAC2012-1 <+2> */
FUNC(boolean, DCM_CODE) Dcm_HsmDcmSfOutOfServiceGuard7(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* guard condition 'TxEnabled'
   * for ROE[...]/Set-up Protocol
   * external transition to state ROEResponseProcessing */
  boolean retVal = FALSE;
  TS_PARAM_UNUSED(DCM_INST(instIdx));

  DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD7_ENTRY(DCM_INST(instIdx));

  retVal = Dcm_Roe_CheckForTxEnable();

  DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD7_EXIT(retVal, DCM_INST(instIdx));

  return retVal;
}
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfOutOfServiceAction7(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* action 'Set-up Protocol'
   * for ROE[TxEnabled]/...
   * external transition to state ROEResponseProcessing */

  DBG_DCM_HSMDCMSFOUTOFSERVICEACTION7_ENTRY(DCM_PDL_SF(const uint8 instIdx));

  TS_PARAM_UNUSED(DCM_INST(instIdx));

  DBG_DCM_HSMDCMSFOUTOFSERVICEACTION7_EXIT(DCM_PDL_SF(const uint8 instIdx));
}
/* Deviation MISRAC2012-1 <+2> */
FUNC(boolean, DCM_CODE) Dcm_HsmDcmSfOutOfServiceGuard8(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* guard condition '!TxEnabled'
   * for ROE[...]/Free Rx Buffer; Dcm_Dsl_Supervisor_ProtocolFree()
   * external transition to state OutOfService */
  boolean retVal = FALSE;
  TS_PARAM_UNUSED(DCM_INST(instIdx));

  DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD8_ENTRY(DCM_INST(instIdx));

  retVal = !Dcm_Roe_CheckForTxEnable();

  DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD8_EXIT(retVal, DCM_INST(instIdx));
  return retVal;
}

/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfOutOfServiceAction8(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* action 'Free Rx Buffer; Dcm_Dsl_Supervisor_ProtocolFree()'
   * for ROE[!TxEnabled]/...
   * external transition to state OutOfService */
  DBG_DCM_HSMDCMSFOUTOFSERVICEACTION8_ENTRY(DCM_PDL_SF(instIdx));

  /* Release rx and tx buffers */
  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  /* NextJob is required here, since the data for the current job is updated only in the
     InServiceEntry function */
  (void)Dcm_Dsl_BufferFree(DCM_PROTOCOL_ID_FROM_INDEX(DCM_CUR_INST_ID(instIdx)),
                           DCM_HSM_INFO(instIdx).NextJob.JobRxBuffer.BufferId);

  /* LEAVE critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  Dcm_Dsl_Supervisor_ProtocolFree(
    DCM_PROTOCOL_ID_FROM_INDEX(DCM_CUR_INST_ID(instIdx)));

  DBG_DCM_HSMDCMSFOUTOFSERVICEACTION8_EXIT(DCM_PDL_SF(instIdx));
}

/* ************************************************************************
 * State: OosInit
 * Parent state: OutOfService
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) OosInit -> WaitForFullCom: RETURN_FROM_BL[]/ComM_DCM_ActiveDiagnostic(); Set-up Protocol
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfOosInitAction1(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* action 'ComM_DCM_ActiveDiagnostic(); Set-up Protocol'
   * for RETURN_FROM_BL[]/...
   * external transition to state WaitForFullCom */

  uint8_least j = 0U;
  const uint8 currentServiceID = DCM_HSM_INFO(instIdx).NextJob.JobRxBuffer.BufferPtr[0];
   /* Get SesTabConfig info */
  P2CONST(Dcm_SesTabConfigType, AUTOMATIC, DCM_CONST) sesTabConfig =
    &Dcm_SesTabConfig[DCM_CUR_INST_ID(instIdx)];

  DBG_DCM_HSMDCMSFOOSINITACTION1_ENTRY(DCM_INST(instIdx));


  if (DCM_SID_SESSION_CONTROL == currentServiceID)
  {
    /* Check if the requested session is supported */
    for (j = 0U; j < sesTabConfig->NumEntries; ++j)
    {
      /* The job package must first be processed to see whether it is valid. The Protocol's HSM info
         shall then be updated if the package proves to be valid. In this case, validity means that
         the requested session for a jump FROM the bootloader is supported. */
      if ((sesTabConfig->SessionEntries[j]).SesCtrl == DCM_HSM_INFO(instIdx).NextJob.JobRxBuffer.BufferPtr[1U])
      {
        break;
      }
    }
  }

  /* If the requested session level is not configured, report error */
  if ((j >= sesTabConfig->NumEntries) && (DCM_SID_SESSION_CONTROL == currentServiceID))
  {
    /* No response can be sent, we therefore suppress any response. */
    /* Do this modification in the job package, because in the next state will be copied to HSM INFO */
    DCM_HSM_INFO(instIdx).NextJob.JobRequestType = DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST;

    DCM_HSM_INFO(instIdx).retryTimer = 1U;

#if (DCM_DEV_ERROR_DETECT == STD_ON)
    DCM_DET_REPORT_ERROR(DCM_SVCID_GETPROGCONDITIONS, DCM_E_PARAM);
#endif
  }
  else
  {
    /* Get the channel index */
    Dcm_RxPduIdInfoType RxPduIdInfo;

    /* The job package must first be processed to see whether it is valid. The Protocol's HSM info
       shall then be updated if the package proves to be valid. In this case, validity means that
       the requested session for a jump FROM the bootloader is supported. */
    Dcm_Dsl_RxConnectionGetRxPduIdInformation(DCM_HSM_INFO(instIdx).NextJob.JobRxConnectionId, &RxPduIdInfo);

    DCM_HSM_INFO(instIdx).retryTimer = 1U;
  }

  DBG_DCM_HSMDCMSFOOSINITACTION1_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: OosNotified
 * Parent state: OutOfService
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfOosNotifiedEntry(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* entry action 'if (default session) ComM_DCM_InactiveDiagnostic();' */
  DBG_DCM_HSMDCMSFOOSNOTIFIEDENTRY_ENTRY(DCM_INST(instIdx));

  /* If the Protocol was handling anything external, end the communication session now. */
  if (
      (
       (DCM_NORMAL_PHYSICAL_REQUEST == DCM_HSM_INFO(instIdx).RequestType) ||
       (DCM_NORMAL_FUNCTIONAL_REQUEST == DCM_HSM_INFO(instIdx).RequestType) ||
       (DCM_JUMP_FROM_BL_RESPONSE_REQUEST == DCM_HSM_INFO(instIdx).RequestType) ||
       (DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST == DCM_HSM_INFO(instIdx).RequestType)
      ) &&
      (DCM_INVALID_USER_ID != DCM_HSM_INFO(instIdx).RxConnectionId)
     )
  {
    Dcm_Dsl_Supervisor_ProcessingEnd(
      DCM_HSM_INFO(instIdx).RxConnectionId);
  }

  /* Initialize 'Status of failures before dispatching a service request */
  DCM_HSM_INFO(instIdx).requestValidationStatus = FALSE;

  DBG_DCM_HSMDCMSFOOSNOTIFIEDENTRY_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: InService
 * Parent state: Top
 * Init substate: RequestProcessing
 * Transitions originating from this state:
 * 1) RETRY[]/if (RetryTimer==0) CallService(PENDING)
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfInServiceEntry(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* entry action 'Initialize protocol data' */
  DBG_DCM_HSMDCMSFINSERVICEENTRY_ENTRY(DCM_PDL_SF(const uint8 instIdx));

  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  DCM_HSM_INFO(instIdx).CancellationConditions.CancellationType = DCM_CANCEL_INVALID;
  DCM_HSM_INFO(instIdx).CancellationConditions.FinalTransmissionDone = FALSE;
  DCM_HSM_INFO(instIdx).negativeResponseCode = DCM_E_POSITIVERESPONSE;
  DCM_HSM_INFO(instIdx).msgContext.msgAddInfo.suppressPosResponse = FALSE;

  /* After the protocol processing re-enters to InService state the HSM INFO can be updated */
  DCM_HSM_INFO(instIdx).RxConnectionId = DCM_HSM_INFO(instIdx).NextJob.JobRxConnectionId;
  DCM_HSM_INFO(instIdx).ComMChannelId = DCM_HSM_INFO(instIdx).NextJob.JobComMChannelId;
  DCM_HSM_INFO(instIdx).TxConnectionId = DCM_HSM_INFO(instIdx).NextJob.JobTxConnectionId;
  DCM_HSM_INFO(instIdx).TesterSourceAddress = DCM_HSM_INFO(instIdx).NextJob.JobTesterSourceAddress;
  DCM_HSM_INFO(instIdx).RxBuffer = DCM_HSM_INFO(instIdx).NextJob.JobRxBuffer;
  DCM_HSM_INFO(instIdx).RequestType = DCM_HSM_INFO(instIdx).NextJob.JobRequestType;
  DCM_HSM_INFO(instIdx).rpCntr = DCM_HSM_INFO(instIdx).NextJob.JobRpCntr;
  DCM_HSM_INFO(instIdx).rpCntrConfirmed = DCM_HSM_INFO(instIdx).NextJob.JobRpCntrConfirmed;
  DCM_HSM_INFO(instIdx).responsePendingTxStatus = DCM_HSM_INFO(instIdx).NextJob.JobResponsePendingTxStatus;

  /* Invalidate next job */
  DCM_HSM_INFO(instIdx).NextJob.JobRxConnectionId = DCM_NUM_RX_CONNECTIONS;
  DCM_HSM_INFO(instIdx).NextJob.JobComMChannelId = DCM_NUM_RX_COMM_CHANNELS;
  DCM_HSM_INFO(instIdx).NextJob.JobTxConnectionId = DCM_NUM_TX_CONNECTIONS;
  DCM_HSM_INFO(instIdx).NextJob.JobTesterSourceAddress = 0;
  DCM_HSM_INFO(instIdx).NextJob.JobRequestType = DCM_INVALID_REQUEST_TYPE;

  /* EXIT critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  DBG_DCM_HSMDCMSFINSERVICEENTRY_EXIT(DCM_PDL_SF(const uint8 instIdx));
}
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfInServiceAction1(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* action 'if (RetryTimer==0) CallService(PENDING)'
   * for RETRY[]/...
   * internal transition */

  DBG_DCM_HSMDCMSFINSERVICEACTION1_ENTRY(DCM_INST(instIdx));

  /* Call service handler, when retry timer expires */
  Dcm_DspInternal_SvcFunc(DCM_CUR_INST_ID(instIdx), DCM_PENDING);

  DBG_DCM_HSMDCMSFINSERVICEACTION1_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: Finishing
 * Parent state: InService
 * Init substate: ConfirmingOk
 * Transitions originating from this state:
 * 1) Finishing -> OosNotified: PROC_DONE[]/
 */


/* ************************************************************************
 * State: ConfirmingNOk
 * Parent state: Finishing
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfConfirmingNOkEntry(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* entry action 'DspInternal_DcmConfirmation(NOT_OK);' */

  /* get the context information current instance */
  P2CONST(Dcm_HsmInfoType, AUTOMATIC, DCM_CONST) curHsmInfo
    = &(DCM_HSM_INFO(instIdx));
  /* holds the confirmation status */
  Dcm_ConfirmationStatusType status;

  DBG_DCM_HSMDCMSFCONFIRMINGNOKENTRY_ENTRY(DCM_INST(instIdx));

  if(curHsmInfo->negativeResponseCode == DCM_E_OK)
  {
    status = DCM_RES_POS_NOT_OK;
  }
  else
  {
    status = DCM_RES_NEG_NOT_OK;
  }

  if (DCM_PERIODIC_TYPE1_REQUEST != curHsmInfo->RequestType)
  {
    /* confirm the transmission */
    Dcm_DspInternal_DcmConfirmation(DCM_INST(instIdx),
                                    status);
  }
  else
  {
    (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, instIdx, DCM_HSM_DCM_EV_PROC_DONE);
  }

  DBG_DCM_HSMDCMSFCONFIRMINGNOKENTRY_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: ConfirmingOk
 * Parent state: Finishing
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfConfirmingOkEntry(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* entry action 'DspInternal_DcmConfirmation(OK);' */

  /* get the context information current instance */
  P2CONST(Dcm_HsmInfoType, AUTOMATIC, DCM_CONST) curHsmInfo
    = &(DCM_HSM_INFO(instIdx));
  /* holds the confirmation status */
  Dcm_ConfirmationStatusType status;

  DBG_DCM_HSMDCMSFCONFIRMINGOKENTRY_ENTRY(DCM_INST(instIdx));

  if(curHsmInfo->negativeResponseCode == DCM_E_OK)
  {
    status = DCM_RES_POS_OK;
  }
  else
  {
    status = DCM_RES_NEG_OK;
  }

  if (DCM_PERIODIC_TYPE1_REQUEST != curHsmInfo->RequestType)
  {
    /* confirm the transmission */
    Dcm_DspInternal_DcmConfirmation(DCM_INST(instIdx),
                                    status);
  }
  else
  {
    (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, instIdx, DCM_HSM_DCM_EV_PROC_DONE);
  }

  DBG_DCM_HSMDCMSFCONFIRMINGOKENTRY_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: PeriodicProcessing
 * Parent state: InService
 * Init substate: PeriodicResponseProcessing
 * Transitions originating from this state:
 * 1) PeriodicProcessing -> OosNotified: PROC_ABORT[]/Free Rx Buffer; Free Tx Buffer;
 *    Dcm_Dsl_SupervisorProtocolFree();
 * 2) PeriodicProcessing -> Sending: PROC_DONE[]/
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfPeriodicProcessingAction1(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* action 'Free Rx Buffer; Free Tx Buffer; Dcm_Dsl_SupervisorProtocolFree();'
   * for PROC_ABORT[]/...
   * external transition to state OosNotified */
  DBG_DCM_HSMDCMSFPERIODICPROCESSINGACTION1_ENTRY(DCM_INST(instIdx));

  Dcm_Dsl_BufferFreeRxTxNRC(DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                            DCM_HSM_INFO(instIdx).RxBuffer.BufferId,
                            DCM_HSM_INFO(instIdx).TxBuffer.BufferId,
                            DCM_HSM_INFO(instIdx).NRCBuffer.BufferId);

  Dcm_Dsl_Supervisor_ProtocolFree(
    DCM_PROTOCOL_ID_FROM_INDEX(DCM_CUR_INST_ID(instIdx)));

  DBG_DCM_HSMDCMSFPERIODICPROCESSINGACTION1_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: PeriodicResponseProcessing
 * Parent state: PeriodicProcessing
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfPeriodicResponseProcessingEntry(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* entry action 'PeriodicDidProcess()' */
  DBG_DCM_HSMDCMSFPERIODICRESPONSEPROCESSINGENTRY_ENTRY(DCM_INST(instIdx));

#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
  Dcm_Dsp_ReadDataByPeriodicIdentifier_ProcessPDID(DCM_INST(instIdx));
#else
  TS_PARAM_UNUSED(DCM_INST(instIdx));
#endif /* (DCM_DSP_USE_SERVICE_0X2A == STD_ON) */

  DBG_DCM_HSMDCMSFPERIODICRESPONSEPROCESSINGENTRY_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: PeriodicProcessingUUDT
 * Parent state: InService
 * Init substate: PeriodicSendingUUDT
 * Transitions originating from this state:
 * 1) PeriodicProcessingUUDT -> OosNotified: PROC_ABORT[]/Free Rx Buffer; Free Tx Buffer;
 *    Dcm_Dsl_Supervisor_ProtocolFree();
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfPeriodicProcessingUUDTAction1(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* action 'Free Rx Buffer; Free Tx Buffer; Dcm_Dsl_Supervisor_ProtocolFree();'
   * for PROC_ABORT[]/...
   * external transition to state OosNotified */
  DBG_DCM_HSMDCMSFPERIODICPROCESSINGUUDTACTION1_ENTRY(DCM_INST(instIdx));

#if ( (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
      (DCM_DSP_USE_SERVICE_0X2A == STD_ON) && \
      (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) )

  /* Release rx and tx buffers */
  Dcm_Dsl_BufferFreeRxTxNRC(DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                            DCM_HSM_INFO(instIdx).RxBuffer.BufferId,
                            DCM_HSM_INFO(instIdx).TxBuffer.BufferId,
                            DCM_HSM_INFO(instIdx).NRCBuffer.BufferId);

  Dcm_Dsl_Supervisor_ProtocolFree(
    DCM_PROTOCOL_ID_FROM_INDEX(DCM_CUR_INST_ID(instIdx)));

#else /* ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
          (DCM_DSP_USE_SERVICE_0X2A == STD_ON) && \
          (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U)
         ) */
   TS_PARAM_UNUSED(DCM_INST(instIdx));

#endif /* ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
           (DCM_DSP_USE_SERVICE_0X2A == STD_ON) && \
           (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U)
          ) */
  DBG_DCM_HSMDCMSFPERIODICPROCESSINGUUDTACTION1_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: PeriodicSendingUUDT
 * Parent state: PeriodicProcessingUUDT
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) PERIODIC_UUDT_NEXT[]/While samples and TxPduIds available
 * 2) PeriodicSendingUUDT -> PeriodicSendingUUDTFinishing: PERIODIC_UUDT_DONE[]/
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfPeriodicSendingUUDTEntry(
  DCM_PDL_SF(const uint8 instIdx))
{
  DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTENTRY_ENTRY(DCM_INST(instIdx));
  /* entry action 'Dcm_Dsp_ReadDataByPeriodicIdentifier_UUDTSetup()' */
#if ( (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
      (DCM_DSP_USE_SERVICE_0X2A == STD_ON) && \
      (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) )
  Dcm_Dsp_ReadDataByPeriodicIdentifier_UUDTSetup(DCM_INST(instIdx));
#else
  TS_PARAM_UNUSED(DCM_INST(instIdx));
#endif /* ( (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
            (DCM_DSP_USE_SERVICE_0X2A == STD_ON) && \
            (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) ) */
  DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTENTRY_EXIT(DCM_INST(instIdx));
}
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfPeriodicSendingUUDTAction1(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* action 'While samples and TxPduIds available'
   * for PERIODIC_UUDT_NEXT[]/...
   * internal transition */
  DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTACTION1_ENTRY(DCM_INST(instIdx));
#if ( (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
      (DCM_DSP_USE_SERVICE_0X2A == STD_ON) && \
      (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) )
  Dcm_Dsp_ReadDataByPeriodicIdentifier_UUDTSendNextPDID(DCM_INST(instIdx));
#else
  TS_PARAM_UNUSED(DCM_INST(instIdx));
#endif /* ( (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
            (DCM_DSP_USE_SERVICE_0X2A == STD_ON) && \
            (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) ) */
  DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTACTION1_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: PeriodicSendingUUDTFinishing
 * Parent state: PeriodicProcessingUUDT
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) PERIODIC_UUDT_CONFIRM[]/While UUDT confirmations missing
 * 2) PeriodicSendingUUDTFinishing -> OosNotified: PROC_DONE[]/Free Rx Buffer; Free Tx Buffer;
 *    Dcm_Dsl_SupervisorProtocolFree();
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfPeriodicSendingUUDTFinishingAction1(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* action 'While UUDT confirmations missing'
   * for PERIODIC_UUDT_CONFIRM[]/...
   * internal transition */
#if ( (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
      (DCM_DSP_USE_SERVICE_0X2A == STD_ON) && \
      (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) )
  const boolean ReturnValue
    = Dcm_Dsp_ReadDataByPeriodicIdentifier_ConfirmAndCheckPendingConfirmations();

  DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTFINISHINGACTION1_ENTRY(DCM_INST(instIdx));
  /* Got confirmation for all transmissions */
  /* !LINKSTO SWS_Dcm_01103_WaitForAllPeriodicTxConfirmations,1 */
  if (ReturnValue == TRUE)
  {
    uint16 NumberOfTransmittableSamples;

    /* Emit the event PROC_DONE to move the HSM back to OutOfService state */
    (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm ,DCM_CUR_INST_ID(instIdx),
                                  DCM_HSM_DCM_EV_PROC_DONE);

    /* If there are entries in the transmission ring try to send them immediately to avoid
     * having to wait for the next Dcm main function call */
    /* !LINKSTO SWS_Dcm_01103_WaitForAllPeriodicTxConfirmations,1 */

    NumberOfTransmittableSamples
      = Dcm_Dsp_ReadDataByPeriodicIdentifier_GetNumberOfTransmittableSamples();

    if (NumberOfTransmittableSamples > 0U)
    {
        Dcm_Dsp_ReadDataByPeriodicIdentifier_Transmit();
    }
  }
#else
  DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTFINISHINGACTION1_ENTRY(DCM_INST(instIdx));
  TS_PARAM_UNUSED(DCM_INST(instIdx));
#endif /* ( (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
            (DCM_DSP_USE_SERVICE_0X2A == STD_ON) && \
            (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) ) */
  DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTFINISHINGACTION1_EXIT(DCM_INST(instIdx));
}
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfPeriodicSendingUUDTFinishingAction2(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* action 'Free Rx Buffer; Free Tx Buffer; Dcm_Dsl_SupervisorProtocolFree();'
   * for PROC_DONE[]/...
   * external transition to state OosNotified */
  DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTFINISHINGACTION2_ENTRY(DCM_INST(instIdx));
#if ( (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
      (DCM_DSP_USE_SERVICE_0X2A == STD_ON) && \
      (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) )

  /* Release rx and tx buffers */
  Dcm_Dsl_BufferFreeRxTxNRC(DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                            DCM_HSM_INFO(instIdx).RxBuffer.BufferId,
                            DCM_HSM_INFO(instIdx).TxBuffer.BufferId,
                            DCM_HSM_INFO(instIdx).NRCBuffer.BufferId);

  Dcm_Dsl_Supervisor_ProtocolFree(
    DCM_PROTOCOL_ID_FROM_INDEX(DCM_CUR_INST_ID(instIdx)));

#else
  TS_PARAM_UNUSED(DCM_INST(instIdx));
#endif /* ( (DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && \
            (DCM_DSP_USE_SERVICE_0X2A == STD_ON) && \
            (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U) ) */

  DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTFINISHINGACTION2_EXIT(DCM_INST(instIdx));
}
/* ************************************************************************
 * State: ROEProcessing
 * Parent state: InService
 * Init substate: ROEResponseProcessing
 * Transitions originating from this state:
 * 1) ROEProcessing -> OosNotified: CANCELLATION_DONE[]/
 * 2) ROEProcessing -> PgBufSending: PROCESS_PAGE[]/
 * 3) ROEProcessing -> OosNotified: PROC_ABORT[]/Free Rx Buffer; Free Tx Buffer;
 *    Dcm_Dsl_Supervisor_ProtocolFree(); Dcm_Dsp_ServiceProcessorInit();
 * 4) ROEProcessing -> NormalSending: PROC_DONE[]/
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfROEProcessingAction3(
  DCM_PDL_SF(const uint8 instIdx))
{
  DBG_DCM_HSMDCMSFROEPROCESSINGACTION3_ENTRY(DCM_INST(instIdx));
  /* action 'Free Rx Buffer; Free Tx Buffer; Dcm_Dsl_Supervisor_ProtocolFree();
   * Dcm_Dsp_ServiceProcessorInit();'
   * for PROC_ABORT[]/...
   * external transition to state OosNotified */

  /* Release rx and tx buffers */
  Dcm_Dsl_BufferFreeRxTxNRC(DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                            DCM_HSM_INFO(instIdx).RxBuffer.BufferId,
                            DCM_HSM_INFO(instIdx).TxBuffer.BufferId,
                            DCM_HSM_INFO(instIdx).NRCBuffer.BufferId);

  /* Service processing is now finished. */

  Dcm_Dsl_Supervisor_ProtocolFree(
    DCM_PROTOCOL_ID_FROM_INDEX(DCM_CUR_INST_ID(instIdx)));

  DBG_DCM_HSMDCMSFROEPROCESSINGACTION3_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: ROEResponseProcessing
 * Parent state: ROEProcessing
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) ROEResponseProcessing -> WaitForRoeProcessingCancellation: CANCEL_PROCESSING[]/
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfROEResponseProcessingEntry(
  DCM_PDL_SF(const uint8 instIdx))
{
  DBG_DCM_HSMDCMSFROERESPONSEPROCESSINGENTRY_ENTRY(DCM_INST(instIdx));
  /* entry action 'DispatchServiceReq();' */

#if(DCM_DSP_USE_ROESERVICES == STD_ON)
  /* !LINKSTO SWS_Dcm_00922,1 */
  Dcm_DsdInternal_DispatchSvcReq(DCM_PL_SF(instIdx));
#else
  TS_PARAM_UNUSED(DCM_INST(instIdx));
#endif /* if(DCM_DSP_USE_ROESERVICES == STD_ON) */
  DBG_DCM_HSMDCMSFROERESPONSEPROCESSINGENTRY_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: WaitForRoeProcessingCancellation
 * Parent state: ROEProcessing
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfWaitForRoeProcessingCancellationEntry(
  DCM_PDL_SF(const uint8 instIdx))
{
  DBG_DCM_HSMDCMSFWAITFORROEPROCESSINGCANCELLATIONENTRY_ENTRY(DCM_INST(instIdx));
  /* entry action 'Dcm_Dsp_ProtocolCancelProcessing();
     Dcm_Dsp_ProtocolSetUpCancellation(NORMAL_PROCESSING_NO_TX);' */

  Dcm_Dsp_ProtocolSetUpCancellation(DCM_CANCEL_NORMAL_PROCESSING_NO_TX, DCM_INST(instIdx));
  Dcm_Dsp_ProtocolCancelProcessing(DCM_INST(instIdx));
  DBG_DCM_HSMDCMSFWAITFORROEPROCESSINGCANCELLATIONENTRY_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: RequestProcessing
 * Parent state: InService
 * Init substate: WaitForAppl
 * Transitions originating from this state:
 * 1) RequestProcessing -> OosNotified: CANCELLATION_DONE[]/
 * 2) RequestProcessing -> PgBufSending: PROCESS_PAGE[]/
 * 3) RequestProcessing -> OosNotified: PROC_ABORT[]/Free Rx Buffer; Free Tx Buffer;
 *    Stop and free P2Timer; Dcm_Dsl_SupervisorProtocolFree();
 * 4) RequestProcessing -> NormalSending: PROC_DONE[]/
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfRequestProcessingEntry(
  DCM_PDL_SF(const uint8 instIdx))
{
  DBG_DCM_HSMDCMSFREQUESTPROCESSINGENTRY_ENTRY(DCM_INST(instIdx));
  /* entry action 'DispatchServiceReq();if (default session) ComM_DCM_ActiveDiagnostic();
   * RespPendInFirstCycle();RpCntr=Dcm_Dsl_Supervisor_ProtocolHandling_GetNumberOfRCRRP();
   * RpCntrConfirmed = Dcm_Supervisor_ProtocolHandling_GetNumberOfConfirmedRCRRP();' */

  /* !LINKSTO Dcm.EB.ReadMemoryByAddress.ReadMemory.RCRRP.WaitTillConfirmation,1 */
  /* !LINKSTO Dcm.EB.TransferData.ReadMemory.RCRRP.WaitTillConfirmation,1 */
  /* !LINKSTO Dcm.EB.ReadDataByIdentifier.ReadMemory.RCRRP.WaitTillConfirmation,1 */
  /* !LINKSTO Dcm.EB.WriteMemoryByAddress.WriteMemory.RCRRP.WaitTillConfirmation,1 */
  /* !LINKSTO Dcm.EB.TransferData.WriteMemory.RCRRP.WaitTillConfirmation,1 */

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
  /* reset the forced response counter */
  DCM_HSM_INFO(instIdx).rpForcedCntr = 0U;

  /* reset the confirmation forced counter */
  DCM_HSM_INFO(instIdx).rpForcedCntrConfirmed = 0U;
#endif /* #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON) */

  /* reset the control flag for access to SetProgConditions() */
  DCM_HSM_INFO(instIdx).SetProgConditionsFinished = FALSE;

  /* process the service request */
  Dcm_DsdInternal_DispatchSvcReq(DCM_PL_SF(instIdx));

  if ((DCM_HSM_INFO(instIdx).rpCntr == 0U) && (DCM_HSM_INFO(instIdx).rpCntrConfirmed == 0U))
  {
    /* If necessary send a RCRRP in the first cycle */
    RespPendInFirstCycle(DCM_PL_SF(instIdx));
  }
  DBG_DCM_HSMDCMSFREQUESTPROCESSINGENTRY_EXIT(DCM_INST(instIdx));
}
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfRequestProcessingAction3(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* action 'Free Rx Buffer; Free Tx Buffer; Stop and free P2Timer;
     Dcm_Dsl_SupervisorProtocolFree();'
   * for PROC_ABORT[]/...
   * external transition to state OosNotified */

  Dcm_ProtocolIndexType protocolIndex = DCM_INST(instIdx);

  DBG_DCM_HSMDCMSFREQUESTPROCESSINGACTION3_ENTRY(DCM_INST(instIdx));

  /* Release rx and tx buffers */
  Dcm_Dsl_BufferFreeRxTxNRC(DCM_PROTOCOL_ID_FROM_INDEX(protocolIndex),
                            DCM_HSM_INFO(instIdx).RxBuffer.BufferId,
                            DCM_HSM_INFO(instIdx).TxBuffer.BufferId,
                            DCM_HSM_INFO(instIdx).NRCBuffer.BufferId);

  /* Stop and release P2Timer */
  (void)Dcm_Dsl_DiagnosticSessionHandler_StopTimer(
                            DCM_PROTOCOL_ID_FROM_INDEX(protocolIndex),
                            protocolIndex,
                            DCM_P2TIMER);

  (void)Dcm_Dsl_DiagnosticSessionHandler_FreeP2Timer(
                            DCM_PROTOCOL_ID_FROM_INDEX(protocolIndex),
                            protocolIndex);

  /* Service processing is now finished. */
  Dcm_Dsl_Supervisor_ProtocolFree(
    DCM_PROTOCOL_ID_FROM_INDEX(DCM_CUR_INST_ID(instIdx)));

  DBG_DCM_HSMDCMSFREQUESTPROCESSINGACTION3_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: RequestCancelling
 * Parent state: RequestProcessing
 * Init substate: CancelWithGeneralReject
 * Transitions originating from this state:
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfRequestCancellingEntry(
  DCM_PDL_SF(const uint8 instIdx))
{
  DBG_DCM_HSMDCMSFREQUESTCANCELLINGENTRY_ENTRY(DCM_INST(instIdx));
  /* entry action 'Stop and free P2Timer();StopRetryTimer();' */
  (void)Dcm_Dsl_DiagnosticSessionHandler_StopTimer(DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                                                   DCM_INST(instIdx),
                                                   DCM_P2TIMER);

  (void)Dcm_Dsl_DiagnosticSessionHandler_FreeP2Timer(DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                                                     DCM_INST(instIdx));

  DCM_HSM_INFO(instIdx).retryTimer = 0U;
  DBG_DCM_HSMDCMSFREQUESTCANCELLINGENTRY_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: CancelNoGeneralReject
 * Parent state: RequestCancelling
 * Init substate: WaitForProtocol
 * Transitions originating from this state:
 */

/* ************************************************************************
 * State: WaitForProtocol
 * Parent state: CancelNoGeneralReject
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfWaitForProtocolEntry(
  DCM_PDL_SF(const uint8 instIdx))
{
  DBG_DCM_HSMDCMSFWAITFORPROTOCOLENTRY_ENTRY(DCM_INST(instIdx));
  /* entry action 'Dcm_Dsp_ProtocolSetUpCancellation(NORMAL_PROCESSING_NO_TX);
     Dcm_Dsp_ProtocolCancelProcessing();' */

  Dcm_Dsp_ProtocolSetUpCancellation(DCM_CANCEL_NORMAL_PROCESSING_NO_TX, DCM_INST(instIdx));
  Dcm_Dsp_ProtocolCancelProcessing(DCM_INST(instIdx));
  DBG_DCM_HSMDCMSFWAITFORPROTOCOLENTRY_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: WaitForTxAndProtocol
 * Parent state: CancelNoGeneralReject
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfWaitForTxAndProtocolEntry(
  DCM_PDL_SF(const uint8 instIdx))
{
  DBG_DCM_HSMDCMSFWAITFORTXANDPROTOCOLENTRY_ENTRY(DCM_INST(instIdx));
  /* entry action 'Dcm_Dsp_ProtocolSetUpCancellation(NORMAL_PROCESSING_TX);
     Dcm_Dsp_ProtocolCancelProcessing();Dcm_Dsl_TxConnectionCancelTransmit();' */

  Dcm_Dsp_ProtocolSetUpCancellation(DCM_CANCEL_NORMAL_PROCESSING_TX, DCM_INST(instIdx));
  Dcm_Dsp_ProtocolCancelProcessing(DCM_INST(instIdx));

  Dcm_Dsl_TxConnectionCancelTransmit(DCM_HSM_INFO(instIdx).TxConnectionId);
  DBG_DCM_HSMDCMSFWAITFORTXANDPROTOCOLENTRY_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: CancelWithGeneralReject
 * Parent state: RequestCancelling
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) CANCEL_PROCESSING[]/if(GR pending) {Dcm_Dsl_TxConnectionCancelTransmit(TxConnection)}
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfCancelWithGeneralRejectEntry(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* entry action 'Dcm_Dsl_BufferGive(NRCBuffer, TxConnection);
                   Dcm_Dsl_TxConnectionTransmit(NRCBuffer, TxConnection);
                   Dcm_Dsp_ProtocolCancelProcessing();
                   NRCBuffer = GeneralRejectSetUpCancellation(NORMAL_PROCESSING_TX);' */

  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);

  DBG_DCM_HSMDCMSFCANCELWITHGENERALREJECTENTRY_ENTRY(DCM_PDL_SF(instIdx));
#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
  /* The async service processor can access negativeResponseCode in parallel
   * from within a different task context than the HSM */

  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
#endif

#if ((DCM_DSP_USE_SERVICE_0X04 == STD_ON) || (DCM_DSP_USE_SERVICE_0X09 == STD_ON))
  if ((curHsmInfo->serviceId == DCM_SID_CLEAR_DTC) || (curHsmInfo->serviceId == DCM_SID_REQUEST_VEHICLE_INFORMATION))
  {
    /* !LINKSTO Dcm.Service04.DcmDslDiagRespMaxNumRespPend.PENDING.InsideLimitsRCRRP.NRC.ConditionsNotCorrect,1 */
    /* !LINKSTO Dcm.Service04.DcmDslDiagRespMaxNumRespPend.PENDING.LowLimitRCRRP.NRC.ConditionsNotCorrect,1 */
    /* !LINKSTO Dcm.Service04.DcmDspDiagSesRespMaxNumRespPend.PENDING.InsideLimitsRCRRP.NRC.ConditionsNotCorrect,1 */
    /* !LINKSTO Dcm.Service04.DcmDspDiagSesRespMaxNumRespPend.PENDING.LowLimitRCRRP.NRC.ConditionsNotCorrect,1 */
    /* !LINKSTO Dcm.Service09.DcmDslDiagRespMaxNumRespPend.PENDING.InsideLimitsRCRRP.NRC.ConditionsNotCorrect,1 */
    /* !LINKSTO Dcm.Service09.DcmDslDiagRespMaxNumRespPend.PENDING.LowLimitRCRRP.NRC.ConditionsNotCorrect,1 */
    /* !LINKSTO Dcm.Service09.DcmDspDiagSesRespMaxNumRespPend.PENDING.InsideLimitsRCRRP.NRC.ConditionsNotCorrect,1 */
    /* !LINKSTO Dcm.Service09.DcmDspDiagSesRespMaxNumRespPend.PENDING.LowLimitRCRRP.NRC.ConditionsNotCorrect,1 */
    /* for the $04 and $09 the NRC GeneralReject is not supported according to the Table 10, Table 11
     * Table 16 from ISO 15031-5- 2011-04-15 */
    /* set NRC ConditionsNotCorrect */
    curHsmInfo->negativeResponseCode = DCM_E_CONDITIONSNOTCORRECT;
  }
  else
#endif /* ((DCM_DSP_USE_SERVICE_0X04 == STD_ON) || (DCM_DSP_USE_SERVICE_0X09 == STD_ON)) */
  {
    /* set NRC GeneralReject */
    curHsmInfo->negativeResponseCode = DCM_E_GENERALREJECT;
  }
#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
  /* LEAVE critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
#endif

  Dcm_Dsp_ProtocolSetUpCancellation(DCM_CANCEL_NORMAL_PROCESSING_TX, DCM_INST(instIdx));

  /* assemble negative response including:
   *     - currently processed service Id
   *     - currently occurred negative response code */
  curHsmInfo->NRCBuffer.BufferPtr[0] = 0x7F;
  curHsmInfo->NRCBuffer.BufferPtr[1] = curHsmInfo->serviceId;
  curHsmInfo->NRCBuffer.BufferPtr[2] = curHsmInfo->negativeResponseCode;
  curHsmInfo->NRCBuffer.FilledLength = 3U;

  /* With this transmission, the owner of the buffer gets changed to the TxConnection */
  (void)Dcm_Dsl_BufferGive(DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                           curHsmInfo->TxConnectionId,
                           curHsmInfo->NRCBuffer.BufferId);

  /* !LINKSTO Dcm114, 1 */
  Dcm_Dsl_TxConnectionTransmit(curHsmInfo->TxConnectionId,
                               curHsmInfo->ComMChannelId,
                               DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                               FALSE,
                               3U,
                               0U,
                               curHsmInfo->NRCBuffer
                              );

  Dcm_Dsp_ProtocolCancelProcessing(DCM_INST(instIdx));
  DBG_DCM_HSMDCMSFCANCELWITHGENERALREJECTENTRY_EXIT(DCM_PDL_SF(instIdx));
}
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfCancelWithGeneralRejectAction1(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* action 'if(GR pending) {Dcm_Dsl_TxConnectionCancelTransmit(TxConnection)}'
   * for CANCEL_PROCESSING[]/...
   * internal transition */

  DBG_DCM_HSMDCMSFCANCELWITHGENERALREJECTACTION1_ENTRY(DCM_PDL_SF(instIdx));
  if(FALSE == DCM_HSM_INFO(instIdx).CancellationConditions.FinalTransmissionDone)
  {
    /* Cancel the pending transmission of the GeneralReject */
    Dcm_Dsl_TxConnectionCancelTransmit(DCM_HSM_INFO(instIdx).TxConnectionId);
  }
  DBG_DCM_HSMDCMSFCANCELWITHGENERALREJECTACTION1_EXIT(DCM_PDL_SF(instIdx));
}

/* ************************************************************************
 * State: SendRp
 * Parent state: RequestProcessing
 * Init substate: SendRpNormal
 * Transitions originating from this state:
 * 1) SendRp -> WaitForTxAndProtocol: CANCEL_PROCESSING[]/
 * 2) SendRp -> WaitForProtocol: TX_CONF_NOT_OK[]/
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfSendRpEntry(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* entry action '++RpCntr;SendNegResp(ResponsePending);spr=false;' */
  boolean SendRCRRP = FALSE;

  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);

  DBG_DCM_HSMDCMSFSENDRPENTRY_ENTRY(DCM_INST(instIdx));

  /* Reset suppress positive response bit */
  curHsmInfo->msgContext.msgAddInfo.suppressPosResponse = FALSE;

  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  if ((curHsmInfo->rpCntr == curHsmInfo->rpCntrConfirmed) && (curHsmInfo->responsePendingTxStatus != DCM_RP_CONFIRMED_TX))
  {
    SendRCRRP = TRUE;
  }

  /* LEAVE critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  if (SendRCRRP == TRUE)
  {
    /* Set flag to indicate 'response pending' message transmission */
    curHsmInfo->responsePendingTxStatus = DCM_RP_PENDING_CONF;

    /* !LINKSTO Dcm.DcmDslDiagRespMaxNumRespPend.PENDING.InfinityLimit255.HighLimitRCRRP.NRC.SubsequentRCRRP,1 */
    /* !LINKSTO Dcm.DcmDspDiagSesRespMaxNumRespPend.PENDING.InfinityLimit255.HighLimitRCRRP.NRC.SubsequentRCRRP,1 */
    /* !LINKSTO Dcm.DcmDslDiagRespMaxNumRespPend.FORCE_RCRRP.InfinityLimit255.HighLimitRCRRP.NRC.Unlimited_RCRRP,1 */
    /* !LINKSTO Dcm.DcmDspDiagSesRespMaxNumRespPend.FORCE_RCRRP.InfinityLimit255.HighLimitRCRRP.NRC.Unlimited_RCRRP,1 */
    /* !LINKSTO Dcm.DcmDslDiagRespMaxNumRespPend.PENDING.InfinityLimit65535.HighLimitRCRRP.NRC.SubsequentRCRRP,1 */
    /* !LINKSTO Dcm.DcmDslDiagRespMaxNumRespPend.FORCE_RCRRP.InfinityLimit65535.HighLimitRCRRP.NRC.Unlimited_RCRRP,1 */
    /* !LINKSTO Dcm.DcmDspDiagSesRespMaxNumRespPend.PENDING.InfinityLimit65535.HighLimitRCRRP.NRC.SubsequentRCRRP,1 */
    /* !LINKSTO Dcm.DcmDspDiagSesRespMaxNumRespPend.FORCE_RCRRP.InfinityLimit65535.HighLimitRCRRP.NRC.Unlimited_RCRRP,1 */
    /* increment Response Pending timeout counter */
    curHsmInfo->rpCntr++;

    /* Fill buffer with error code 'response pending' and service Id */
    curHsmInfo->NRCBuffer.BufferPtr[0] = 0x7F;
    curHsmInfo->NRCBuffer.BufferPtr[1] = curHsmInfo->serviceId;
    curHsmInfo->NRCBuffer.BufferPtr[2] = DCM_E_REQUESTCORRECTLYRECEIVEDRESPONSEPENDING;
    curHsmInfo->NRCBuffer.FilledLength = 3U;

    /* With this transmission, the owner of the buffer gets changed to the TxConnection */
    (void)Dcm_Dsl_BufferGive(DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                             curHsmInfo->TxConnectionId,
                             curHsmInfo->NRCBuffer.BufferId);

    /* !LINKSTO Dcm114, 1 */
    Dcm_Dsl_TxConnectionTransmit(curHsmInfo->TxConnectionId,
                                 curHsmInfo->ComMChannelId,
                                 DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                                 FALSE,
                                 3U,
                                 0U,
                                 curHsmInfo->NRCBuffer
                                );
  }
  else
  {
    /* The RCRRP is already sent. We need to wait the confirmation. */
  }

  (void)Dcm_Dsl_DiagnosticSessionHandler_StopTimer(DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                                                   DCM_INST(instIdx),
                                                   DCM_P2TIMER);

  DBG_DCM_HSMDCMSFSENDRPENTRY_EXIT(DCM_INST(instIdx));
}
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfSendRpExit(
   DCM_PDL_SF(const uint8 instIdx))
{
  /* exit action 'Start P2Timer();' */

  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);

  Dcm_TimerValueType  P2StarMaxTime;
  Dcm_TimerValueType  P2StarServerAdjustTime;

  DBG_DCM_HSMDCMSFSENDRPEXIT_ENTRY(DCM_INST(instIdx));

  P2StarMaxTime = Dcm_Dsl_DiagnosticSessionHandler_GetP2StarMaxTime(DCM_INST(instIdx));
  P2StarServerAdjustTime = (Dcm_TimerValueType)
                           (Dcm_ProtocolConfig[DCM_INST(instIdx)].P2StarServerAdjustTime);
  /* Start timer P2StarServerMax */
  /* P2StarTime = P2StarMax of current session - Star Adjust time of current protocol*/
  if (P2StarMaxTime > P2StarServerAdjustTime)
  {
    (void)Dcm_Dsl_DiagnosticSessionHandler_StartP2Timer(DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                                                        DCM_INST(instIdx),
                                                        P2StarMaxTime - P2StarServerAdjustTime);
  }
  else
  {
  /* If the P2StarMaxTime is less than or equal to 'P2StarServerAdjustTime' then start
   * the timer with zero and this will emit the event TIMEOUT2 immediately. */
    (void)Dcm_Dsl_DiagnosticSessionHandler_StartP2Timer(DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                                                        DCM_INST(instIdx),
                                                        0U);
  }

  /* Reset status of transmission */
  curHsmInfo->responsePendingTxStatus = DCM_RP_NO_DATA;

  DBG_DCM_HSMDCMSFSENDRPEXIT_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: SendRpForced
 * Parent state: SendRp
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) SendRpForced -> WaitForAppl: TX_CONF_OK[]/CallService(FORCE_RCRRP_OK);
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfSendRpForcedAction1(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* action 'CallService(FORCE_RCRRP_OK);'
   * for TX_CONF_OK[]/...
   * external transition to state WaitForAppl */

  DBG_DCM_HSMDCMSFSENDRPFORCEDACTION1_ENTRY(DCM_INST(instIdx));

  /* Call service handler, after application triggered response pending transmission */
  Dcm_DspInternal_SvcFunc(DCM_CUR_INST_ID(instIdx), DCM_FORCE_RCRRP_OK);

  DBG_DCM_HSMDCMSFSENDRPFORCEDACTION1_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: SendRpNormal
 * Parent state: SendRp
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) SendRpNormal -> WaitForAppl: TX_CONF_OK[]/
 */

/* ************************************************************************
 * State: WaitForAppl
 * Parent state: RequestProcessing
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) WaitForAppl -> WaitForProtocol: CANCEL_PROCESSING[]/
 * 2) WaitForAppl -> CancelWithGeneralReject: FORCE_RCRRP[RpCntr >= Limit]/
 * 3) WaitForAppl -> SendRpForced: FORCE_RCRRP[RpCntr < Limit]/
 * 4) WaitForAppl -> CancelWithGeneralReject: TIMEOUT2[RpCntr >= Limit]/
 * 5) WaitForAppl -> SendRpNormal: TIMEOUT2[RpCntr < Limit]/
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(boolean, DCM_CODE) Dcm_HsmDcmSfWaitForApplGuard2(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* guard condition 'RpCntr >= Limit'
   * for FORCE_RCRRP[...]/
   * external transition to state CancelWithGeneralReject */

  /* Depending on the configuration this function may always allow the
   * transition to SendingResponse if DCM_DIAG_RESP_MAX_NUM_RESP_PEND is
   * configured to be equal to 0.  This prevents the sending of any "response
   * pending" negative response and will lead to an "general reject" if the
   * diagnostic application does not respond within the P2 timeout. */
  boolean SendGeneralReject;

  DBG_DCM_HSMDCMSFWAITFORAPPLGUARD2_ENTRY(DCM_INST(instIdx));

/* !LINKSTO Dcm.DcmDslDiagRespMaxNumRespPend.FORCE_RCRRP.InsideLimitsRCRRP.NRC.GeneralReject,2 */
/* !LINKSTO Dcm.DcmDslDiagRespMaxNumRespPend.FORCE_RCRRP.InsideLimitsRCRRP.Process.Cancellation,2 */
/* !LINKSTO Dcm.DcmDspDiagSesRespMaxNumRespPend.FORCE_RCRRP.InsideLimitsRCRRP.NRC.GeneralReject,2 */
/* !LINKSTO Dcm.DcmDspDiagSesRespMaxNumRespPend.FORCE_RCRRP.InsideLimitsRCRRP.Process.Cancellation,2 */
/* !LINKSTO Dcm.DcmDslDiagRespMaxNumRespPend.FORCE_RCRRP.LowLimitRCRRP.NRC.GeneralReject,1 */
/* !LINKSTO Dcm.DcmDslDiagRespMaxNumRespPend.FORCE_RCRRP.LowLimitRCRRP.Process.Cancellation,1 */
/* !LINKSTO Dcm.DcmDspDiagSesRespMaxNumRespPend.FORCE_RCRRP.LowLimitRCRRP.NRC.GeneralReject,1 */
/* !LINKSTO Dcm.DcmDspDiagSesRespMaxNumRespPend.FORCE_RCRRP.LowLimitRCRRP.Process.Cancellation,1 */
  SendGeneralReject = Dcm_CheckForGeneralRejectConditions(DCM_PL_SF(instIdx));

  DBG_DCM_HSMDCMSFWAITFORAPPLGUARD2_EXIT(SendGeneralReject, DCM_INST(instIdx));

  return SendGeneralReject;
}
/* Deviation MISRAC2012-1 <+2> */
FUNC(boolean, DCM_CODE) Dcm_HsmDcmSfWaitForApplGuard3(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* guard condition 'RpCntr < Limit'
   * for FORCE_RCRRP[...]/
   * external transition to state SendRpForced */
  boolean SendResponsePending;

  DBG_DCM_HSMDCMSFWAITFORAPPLGUARD3_ENTRY(DCM_INST(instIdx));

  SendResponsePending = !(Dcm_CheckForGeneralRejectConditions(DCM_PL_SF(instIdx)));

  DBG_DCM_HSMDCMSFWAITFORAPPLGUARD3_EXIT(SendResponsePending, DCM_INST(instIdx));

  return SendResponsePending;
}
/* Deviation MISRAC2012-1 <+2> */
FUNC(boolean, DCM_CODE) Dcm_HsmDcmSfWaitForApplGuard4(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* guard condition 'RpCntr >= Limit'
   * for TIMEOUT2[...]/
   * external transition to state CancelWithGeneralReject */

  /* Depending on the configuration this function may always allow the
   * transition to SendingResponse if DCM_DIAG_RESP_MAX_NUM_RESP_PEND is
   * configured to be equal to 0.  This prevents the sending of any "response
   * pending" negative response and will lead to an "general reject" if the
   * diagnostic application does not respond within the P2 timeout. */
  boolean SendGeneralReject;

  DBG_DCM_HSMDCMSFWAITFORAPPLGUARD4_ENTRY(DCM_INST(instIdx));

/* !LINKSTO Dcm.DcmDslDiagRespMaxNumRespPend.PENDING.InsideLimitsRCRRP.NRC.GeneralReject,4 */
/* !LINKSTO Dcm.DcmDslDiagRespMaxNumRespPend.PENDING.InsideLimitsRCRRP.Process.Cancellation,2 */
/* !LINKSTO Dcm.DcmDspDiagSesRespMaxNumRespPend.PENDING.InsideLimitsRCRRP.NRC.GeneralReject,4 */
/* !LINKSTO Dcm.DcmDspDiagSesRespMaxNumRespPend.PENDING.InsideLimitsRCRRP.Process.Cancellation,2 */
/* !LINKSTO Dcm.DcmDslDiagRespMaxNumRespPend.PENDING.LowLimitRCRRP.NRC.GeneralReject,3 */
/* !LINKSTO Dcm.DcmDslDiagRespMaxNumRespPend.PENDING.LowLimitRCRRP.Process.Cancellation,1 */
/* !LINKSTO Dcm.DcmDspDiagSesRespMaxNumRespPend.PENDING.LowLimitRCRRP.NRC.GeneralReject,3 */
/* !LINKSTO Dcm.DcmDspDiagSesRespMaxNumRespPend.PENDING.LowLimitRCRRP.Process.Cancellation,1 */
/* !LINKSTO Dcm.DcmDslDiagRespMaxNumRespPend.PENDING.InfinityLimit255.HighLimitRCRRP.NRC.FirstRCRRP,1 */
/* !LINKSTO Dcm.DcmDspDiagSesRespMaxNumRespPend.PENDING.InfinityLimit255.HighLimitRCRRP.NRC.FirstRCRRP,1 */
/* !LINKSTO Dcm.DcmDslDiagRespMaxNumRespPend.PENDING.InfinityLimit65535.HighLimitRCRRP.NRC.FirstRCRRP,1 */
/* !LINKSTO Dcm.DcmDspDiagSesRespMaxNumRespPend.PENDING.InfinityLimit65535.HighLimitRCRRP.NRC.FirstRCRRP,1 */
  SendGeneralReject = Dcm_CheckForGeneralRejectConditions(DCM_PL_SF(instIdx));

  DBG_DCM_HSMDCMSFWAITFORAPPLGUARD4_EXIT(SendGeneralReject, DCM_INST(instIdx));

  return SendGeneralReject;
}
/* Deviation MISRAC2012-1 <+2> */
FUNC(boolean, DCM_CODE) Dcm_HsmDcmSfWaitForApplGuard5(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* guard condition 'RpCntr < Limit'
   * for TIMEOUT2[...]/
   * external transition to state SendRpNormal */

  boolean SendResponse;

  DBG_DCM_HSMDCMSFWAITFORAPPLGUARD5_ENTRY(DCM_INST(instIdx));

  SendResponse = !(Dcm_CheckForGeneralRejectConditions(DCM_PL_SF(instIdx)));

  DBG_DCM_HSMDCMSFWAITFORAPPLGUARD5_EXIT(SendResponse, DCM_INST(instIdx));

  return SendResponse;
}

/* ************************************************************************
 * State: Sending
 * Parent state: InService
 * Init substate: NormalSending
 * Transitions originating from this state:
 * 1) Sending -> ConfirmingNOk: CANCELLATION_DONE[]/
 * 2) Sending -> ConfirmingNOk: TX_CONF_NOT_OK[]/
 * 3) Sending -> ConfirmingOk: TX_CONF_OK[]/
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfSendingEntry(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* entry action 'Dcm_Dsl_BufferGive(ResponseBuffer, TxConnection);
   * Dcm_Dsl_TxConnectionTransmit(ResponseBuffer, TxConnection); Stop P2Timer();' */
  /* get the context information current instance */
  P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_CONST) curHsmInfo
    = &(DCM_HSM_INFO(instIdx));
  boolean SimulateTransmission;
  Dcm_TransmissionTimeoutType TransmissionTimeout;

  DBG_DCM_HSMDCMSFSENDINGENTRY_ENTRY(DCM_INST(instIdx));

  /* Determine if the transmission of the response should be
     simulated. */

  SimulateTransmission = Dcm_SuppressResponse(DCM_INST(instIdx));

  TransmissionTimeout =
    Dcm_Dsl_DiagnosticSessionHandler_GetTimerValue(DCM_INST(instIdx),
                                                   DCM_P2TIMER);

  if (curHsmInfo->negativeResponseCode != DCM_E_OK)
  {
    /* assemble negative response including:
     *     - currently processed service Id
     *     - currently occurred negative response code */
    curHsmInfo->NRCBuffer.BufferPtr[0] = 0x7F;
    curHsmInfo->NRCBuffer.BufferPtr[1] = curHsmInfo->serviceId;
    curHsmInfo->NRCBuffer.BufferPtr[2] = curHsmInfo->negativeResponseCode;
    curHsmInfo->NRCBuffer.FilledLength = 3U;

    /* With this transmission, the owner of the buffer gets changed to the TxConnection */
    (void)Dcm_Dsl_BufferGive(DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                             curHsmInfo->TxConnectionId,
                             curHsmInfo->NRCBuffer.BufferId);

    /* !LINKSTO Dcm114, 1 */
    Dcm_Dsl_TxConnectionTransmit(curHsmInfo->TxConnectionId,
                                 curHsmInfo->ComMChannelId,
                                 DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                                 SimulateTransmission,
                                 3U,
                                 TransmissionTimeout,
                                 curHsmInfo->NRCBuffer
                                );
  }
  else
  {
    /* With this transmission, the owner of the buffer gets changed to the TxConnection */
    (void)Dcm_Dsl_BufferGive(DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                             curHsmInfo->TxConnectionId,
                             curHsmInfo->TxBuffer.BufferId);

    /* !LINKSTO Dcm114, 1 */
    Dcm_Dsl_TxConnectionTransmit(curHsmInfo->TxConnectionId,
                                 curHsmInfo->ComMChannelId,
                                 DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                                 SimulateTransmission,
                                 curHsmInfo->msgContext.resDataLen + 1U,
                                 TransmissionTimeout,
                                 curHsmInfo->TxBuffer
                                );
  }

  /* Transmission is initiated, stop the P2/P2* timer as the
     TxConnexion will handle timeouts from now on. */

  if((DCM_NORMAL_PHYSICAL_REQUEST == DCM_HSM_INFO(instIdx).RequestType) ||
     (DCM_NORMAL_FUNCTIONAL_REQUEST == DCM_HSM_INFO(instIdx).RequestType))
  {
    (void)Dcm_Dsl_DiagnosticSessionHandler_StopTimer(
                                 DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                                 DCM_INST(instIdx),
                                 DCM_P2TIMER);

    (void)Dcm_Dsl_DiagnosticSessionHandler_FreeP2Timer(
                                 DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                                 DCM_INST(instIdx));
  }
  DBG_DCM_HSMDCMSFSENDINGENTRY_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: NormalSending
 * Parent state: Sending
 * Init substate: NormalSendingProcessing
 * Transitions originating from this state:
 */

/* ************************************************************************
 * State: NormalSendingCancelling
 * Parent state: NormalSending
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfNormalSendingCancellingEntry(
  DCM_PDL_SF(const uint8 instIdx))
{
  /* entry action 'Dcm_Dsp_ProtocolSetUpCancellation(NORMAL_SENDING);'
     Dcm_Dsl_TxConnectionCancelTransmit(TxConnection);*/
  DBG_DCM_HSMDCMSFNORMALSENDINGCANCELLINGENTRY_ENTRY(DCM_PDL_SF(instIdx));

  Dcm_Dsp_ProtocolSetUpCancellation(DCM_CANCEL_NORMAL_SENDING, DCM_INST(instIdx));

  /* cancel any pending transmissions. */

  Dcm_Dsl_TxConnectionCancelTransmit(DCM_HSM_INFO(instIdx).TxConnectionId);

  DBG_DCM_HSMDCMSFNORMALSENDINGCANCELLINGENTRY_EXIT(DCM_PDL_SF(instIdx));
}

/* ************************************************************************
 * State: NormalSendingProcessing
 * Parent state: NormalSending
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) NormalSendingProcessing -> NormalSendingCancelling: CANCEL_PROCESSING[]/
 */

/* ************************************************************************
 * State: PgBufSending
 * Parent state: Sending
 * Init substate: PgBufSendingProcessing
 * Transitions originating from this state:
 */

/* ************************************************************************
 * State: PgBufSendingCancelling
 * Parent state: PgBufSending
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfPgBufSendingCancellingEntry(
  DCM_PDL_SF(const uint8 instIdx))
{
  DBG_DCM_HSMDCMSFPGBUFSENDINGCANCELLINGENTRY_ENTRY(DCM_INST(instIdx));
  /* entry action 'Dcm_Dsp_ProtocolSetUpCancellation(PGBUF_SENDING)
        Dcm_DspInternal_ProtocolCancelProcessing();
     Dcm_Dsl_TxConnectionCancelTransmit(TxConnection)' */

  Dcm_Dsp_ProtocolSetUpCancellation(DCM_CANCEL_PG_BUF_SENDING, DCM_INST(instIdx));

  Dcm_Dsp_ProtocolCancelProcessing(DCM_INST(instIdx));

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.TimeoutWaitingForBuffer.CancelOngoingTransmission,1 */
  Dcm_Dsl_TxConnectionCancelTransmit(DCM_HSM_INFO(instIdx).TxConnectionId);
  DBG_DCM_HSMDCMSFPGBUFSENDINGCANCELLINGENTRY_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: PgBufSendingProcessing
 * Parent state: PgBufSending
 * Init substate: PgBufDataAvailable
 * Transitions originating from this state:
 * 1) PgBufSendingProcessing -> PgBufSendingCancelling: CANCEL_PROCESSING[]/
 */

/* ************************************************************************
 * State: PgBufDataAvailable
 * Parent state: PgBufSendingProcessing
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) PgBufDataAvailable -> PgBufDataRequested: PROVIDE_TX_BUF[]/DspInternal_UpdatePage();
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfPgBufDataAvailableAction1(
  DCM_PDL_SF(const uint8 instIdx))
{
  DBG_DCM_HSMDCMSFPGBUFDATAAVAILABLEACTION1_ENTRY(DCM_INST(instIdx));
  /* action 'DspInternal_UpdatePage();'
   * for PROVIDE_TX_BUF[]/...
   * external transition to state PgBufDataRequested */

#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)

  /* Get new data from service */
  if (DCM_HSM_INFO(instIdx).dspService)
  {
    DCM_HSM_INFO(instIdx).TxBuffer.FilledLength = 0;

    Dcm_DspInternal_UpdatePage(
      (Dcm_MsgType)(&(DCM_HSM_INFO(instIdx).TxBuffer.BufferPtr[0])),
      /* buffer size minus length of data already stored in buffer */
      (Dcm_MsgLenType)(DCM_HSM_INFO(instIdx).TxBuffer.Size));
  }
  /* CHECK: NOPARSE */
  else
  {
    /* Defensive branch. Paged-buffer handling is not available for user-implementated services */
    DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
  }
  /* CHECK: PARSE */

#else
#if (DCM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif /* #if (DCM_HSM_INST_MULTI_ENABLED == STD_ON) */
#endif /* #if (DCM_PAGEDBUFFER_ENABLED == STD_ON) */
  DBG_DCM_HSMDCMSFPGBUFDATAAVAILABLEACTION1_EXIT(DCM_INST(instIdx));
}

/* ************************************************************************
 * State: PgBufDataRequested
 * Parent state: PgBufSendingProcessing
 * Init substate: none, this is a leaf state
 * Transitions originating from this state:
 * 1) PgBufDataRequested -> PgBufDataAvailable: PROCESS_PAGE[]/
          Dcm_Dsl_BufferGive(TxBuffer, TxConnection) Dcm_Dsl_TxConnectionProcessNextTxBuffer(
                TxBuffer, TxConnection)
 */
/* Deviation MISRAC2012-1 <+2> */
FUNC(void, DCM_CODE) Dcm_HsmDcmSfPgBufDataRequestedAction1(
  DCM_PDL_SF(const uint8 instIdx))
{
  DBG_DCM_HSMDCMSFPGBUFDATAREQUESTEDACTION1_ENTRY(DCM_INST(instIdx));
  /* action 'Dcm_Dsl_BufferGive(TxBuffer, TxConnection)
          Dcm_Dsl_TxConnectionProcessNextTxBuffer(TxBuffer, TxConnection)'
   * for PROCESS_PAGE[]/...
   * external transition to state PgBufDataAvailable */
  DBG_DCM_DSDINTERNAL_PGBUFDATAREQUESTED_ENTRY();
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
  /* Stop timers P2ServerMax, P2StarServerMax and PgBufTimeout */

  /* Give the transmission buffer back to the TxConnection. */
  (void)Dcm_Dsl_BufferGive(DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                           DCM_HSM_INFO(instIdx).TxConnectionId,
                           DCM_HSM_INFO(instIdx).TxBuffer.BufferId);

  /* Notify the TxConnection that fresh data is now available. */
  Dcm_Dsl_TxConnectionProcessNextTxBuffer(
                           DCM_HSM_INFO(instIdx).TxConnectionId,
                           DCM_HSM_INFO(instIdx).TxBuffer);

#endif /* #if (DCM_PAGEDBUFFER_ENABLED == STD_ON) */
#if (DCM_HSM_INST_MULTI_ENABLED == STD_ON)
  TS_PARAM_UNUSED(instIdx);
#endif /* #if (DCM_HSM_INST_MULTI_ENABLED == STD_ON) */
  DBG_DCM_HSMDCMSFPGBUFDATAREQUESTEDACTION1_EXIT(DCM_INST(instIdx));
}

/*==================[internal function definitions]=========================*/

STATIC FUNC(boolean, DCM_CODE) Dcm_SuppressResponse(const uint8 instIdx)
{
  boolean retVal = FALSE;

  const Dcm_NegativeResponseCodeType nrc
    = DCM_HSM_INFO(instIdx).negativeResponseCode;

#if (DCM_HSM_INST_MULTI_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(instIdx);
#endif

  /* !LINKSTO Dcm.EB.ResponseHandling.ExternalRequest.UdsServiceHasSubfunction.suppressPosRspMsgIndicationBit.RCRRPSent.TransmitPositiveReponse,1 */
  /* !LINKSTO Dcm.EB.ResponseHandling.ApplicationRequest.UdsServiceHasSubfunction.suppressPosRspMsgIndicationBit.RCRRPSent.TransmitPositiveReponse,1 */
  if ((DCM_HSM_INFO(instIdx).rpCntr != 0U) && (FALSE == DCM_IS_OBD_SVCID(DCM_HSM_INFO(instIdx).serviceId)))
  {
    retVal = FALSE;
  }
  else if(DCM_HSM_INFO(instIdx).RequestType == DCM_PERIODIC_TYPE1_REQUEST)
  {
    /* positive response shall not be suppressed */
    retVal = FALSE;
  }
  /* if negative response needs to be sent */
  else if (DCM_E_OK != nrc)
  {
    /* check for exception according to Dcm001 */
#if (DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2013)
    if (
        /* !LINKSTO Dcm.EB.ResponseHandling.ExternalRequest.UdsServiceOnFunctionalAddress.ServiceNotSupported.RCRRPSent.NRC_SNS,1 */
        /* !LINKSTO Dcm.EB.ResponseHandling.ExternalRequest.UdsServiceOnFunctionalAddress.SubFunctionNotSupported.RCRRPSent.NRC_SFNS,1 */
        /* !LINKSTO Dcm.EB.ResponseHandling.ExternalRequest.UdsServiceOnFunctionalAddress.ServiceNotSupportedInActiveSession.RCRRPSent.NRC_SNSIAS,1 */
        /* !LINKSTO Dcm.EB.ResponseHandling.ExternalRequest.UdsServiceOnFunctionalAddress.SubFunctionNotSupportedInActiveSession.RCRRPSent.NRC_SFNSIAS,1 */
        /* !LINKSTO Dcm.EB.ResponseHandling.ExternalRequest.UdsServiceOnFunctionalAddress.RequestOutOfRange.RCRRPSent.NRC_ROOR,1 */
        /* !LINKSTO Dcm.EB.ResponseHandling.ApplicationRequest.UdsServiceOnFunctionalAddress.ServiceNotSupported.RCRRPSent.NRC_SNS,1 */
        /* !LINKSTO Dcm.EB.ResponseHandling.ApplicationRequest.UdsServiceOnFunctionalAddress.SubFunctionNotSupported.RCRRPSent.NRC_SFNS,1 */
        /* !LINKSTO Dcm.EB.ResponseHandling.ApplicationRequest.UdsServiceOnFunctionalAddress.ServiceNotSupportedInActiveSession.RCRRPSent.NRC_SNSIAS,1 */
        /* !LINKSTO Dcm.EB.ResponseHandling.ApplicationRequest.UdsServiceOnFunctionalAddress.SubFunctionNotSupportedInActiveSession.RCRRPSent.NRC_SFNSIAS,1 */
        /* !LINKSTO Dcm.EB.ResponseHandling.ApplicationRequest.UdsServiceOnFunctionalAddress.RequestOutOfRange.RCRRPSent.NRC_ROOR,1 */
        /* reqType == functional and nrc either 0x11, 0x12, 0x7F, 0x7E or 0x31, so that this nrc
        shall not be sent i.e suppressed */
        ((DCM_FUNCTIONAL_TYPE == DCM_HSM_INFO(instIdx).msgContext.msgAddInfo.reqType)
          && ((DCM_E_SERVICENOTSUPPORTED == nrc)
              || (DCM_E_SUBFUNCTIONNOTSUPPORTED == nrc)
              || (DCM_E_REQUESTOUTOFRANGE == nrc)
              || (DCM_E_SUBFUNCTIONNOTSUPPORTEDINACTIVESESSION == nrc)
              || (DCM_E_SERVICENOTSUPPORTEDINACTIVESESSION == nrc)
             )
        )||
          /* A suppressed serviceNotSupported NRC in case of an OBD service means that
             the service is not configured and that the NRC was not triggered by a mode
             rule check. In this case the Dcm shall send no response, as it is required in
             requirement Dcm.EB.OBD.AllSvc.RequestedServiceNotSupported
             (respective Dcm.ISO-15031-5_6.2.4.3.7_Table11_a1 and Dcm.ISO-15031-5_6.2.4.3.7_Table11_d).
             Even though the response is suppressed, theoretically the buffer still contains
             a negative response. The NRC Buffer is therefore updated with the required
             FilledLenght. */
        ((DCM_IS_OBD_SVCID(DCM_HSM_INFO(instIdx).serviceId)) &&
         (TRUE == DCM_HSM_INFO(instIdx).msgContext.msgAddInfo.suppressPosResponse) &&
         (DCM_E_SERVICENOTSUPPORTED == nrc)
        )
       )
#else
    if (
        /* !LINKSTO Dcm.EB.ResponseHandling.ExternalRequest.UdsServiceOnFunctionalAddress.ServiceNotSupported.RCRRPSent.NRC_SNS,1 */
        /* !LINKSTO Dcm.EB.ResponseHandling.ExternalRequest.UdsServiceOnFunctionalAddress.SubFunctionNotSupported.RCRRPSent.NRC_SFNS,1 */
        /* !LINKSTO Dcm.EB.ResponseHandling.ExternalRequest.UdsServiceOnFunctionalAddress.RequestOutOfRange.RCRRPSent.NRC_ROOR,1 */
        /* !LINKSTO Dcm.EB.ResponseHandling.ApplicationRequest.UdsServiceOnFunctionalAddress.ServiceNotSupported.RCRRPSent.NRC_SNS,1 */
        /* !LINKSTO Dcm.EB.ResponseHandling.ApplicationRequest.UdsServiceOnFunctionalAddress.SubFunctionNotSupported.RCRRPSent.NRC_SFNS,1 */
        /* !LINKSTO Dcm.EB.ResponseHandling.ApplicationRequest.UdsServiceOnFunctionalAddress.RequestOutOfRange.RCRRPSent.NRC_ROOR,1 */
        /* reqType == functional and nrc either 0x11, 0x12, or 0x31, so that this nrc
        shall not be sent i.e suppressed */
        ((DCM_FUNCTIONAL_TYPE == DCM_HSM_INFO(instIdx).msgContext.msgAddInfo.reqType)
          && ((DCM_E_SERVICENOTSUPPORTED == nrc)
              || (DCM_E_SUBFUNCTIONNOTSUPPORTED == nrc)
              || (DCM_E_REQUESTOUTOFRANGE == nrc)
             )
        )||
          /* A suppressed serviceNotSupported NRC in case of an OBD service means that
             the service is not configured and that the NRC was not triggered by a mode
             rule check. In this case the Dcm shall send no response, as it is required in
             requirement Dcm.EB.OBD.AllSvc.RequestedServiceNotSupported
             (respective Dcm.ISO-15031-5_6.2.4.3.7_Table11_a1 and Dcm.ISO-15031-5_6.2.4.3.7_Table11_d).
             Even though the response is suppressed, theoretically the buffer still contains
             a negative response. The NRC Buffer is therefore updated with the required
             FilledLenght. */
        ((DCM_IS_OBD_SVCID(DCM_HSM_INFO(instIdx).serviceId)) &&
         (TRUE == DCM_HSM_INFO(instIdx).msgContext.msgAddInfo.suppressPosResponse) &&
         (DCM_E_SERVICENOTSUPPORTED == nrc)
        )
       )
#endif /* #if (DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2013) */
    {
      retVal = TRUE;
    }
    else
    {
     retVal = FALSE;
    }
  } /* as no nrc, check if positive response shall be suppressed */
  else if (TRUE == DCM_HSM_INFO(instIdx).msgContext.msgAddInfo.suppressPosResponse)
  {
    retVal = TRUE;
  }
  else
  {
    /* positive response shall not be suppressed */
    retVal = FALSE;
  }

  return retVal;
}

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, DCM_CODE) RespPendInFirstCycle(DCM_PDL_SF(const uint8 instIdx))
{
  Dcm_TimerValueType  P2MaxTime;
  Dcm_TimerValueType  P2ServerAdjustTime;

  P2MaxTime = Dcm_Dsl_DiagnosticSessionHandler_GetP2MaxTime(DCM_INST(instIdx));
  P2ServerAdjustTime = (Dcm_TimerValueType)(Dcm_ProtocolConfig[DCM_INST(instIdx)].P2ServerAdjustTime);

  /* If P2Time = 1, emit DCM_HSM_DCM_EV_TIMEOUT2 event */

  if ((P2MaxTime == (P2ServerAdjustTime + 1U)) && (DCM_HSM_INFO(instIdx).requestValidationStatus == TRUE))
  {
      /* We need to send a RCRRP NRC in the first cycle */
      (void)Dcm_Dsl_DiagnosticSessionHandler_StartP2Timer(
                DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                DCM_INST(instIdx),
                0U);
  }
}
/* Deviation MISRAC2012-1 */
STATIC FUNC(boolean, DCM_CODE) Dcm_CheckForGeneralRejectConditions(DCM_PDL_SF(const uint8 instIdx))
{
  boolean GeneralRejectConditionFulfill = FALSE;

/* Get info about the maximun number of allowed RCRRPs */
  uint16 MaxNumRP =
            Dcm_Dsl_DiagnosticSessionHandler_GetSessionMaxRcrrp(DCM_INST(instIdx));

  /* If SessionMaxNumRP = Infinity limit (255 or 65535 based on configuration), then NRC 0x10
     response shall never be sent. If SessionMaxNumRP = 0x00, then NRC 0x10 response shall
     be sent on first check. All of these conditions are posible because of the construction
     that we use to enable increment the variable member rpCntr. */
  if ((DCM_HSM_INFO(instIdx).rpCntr == MaxNumRP) &&
      (DCM_DIAG_DSL_INFINITY_LIMIT_RESP_PEND != MaxNumRP))
  {
    GeneralRejectConditionFulfill = TRUE;
  }
  return GeneralRejectConditionFulfill;
}
/* Deviation MISRAC2012-1 */
STATIC FUNC(boolean, DCM_CODE) Dcm_BootloaderResponseRequired(DCM_PDL_SF(const uint8 instIdx))
{
  boolean ConditionResult = TRUE;

  if (DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST == Dcm_HsmInfo[DCM_INST(instIdx)].NextJob.JobRequestType)
  {
    ConditionResult = FALSE;
  }

  return ConditionResult;

}

/* Deviation MISRAC2012-1 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Roe_CheckForTxEnable(void)
{

#if(DCM_DSP_USE_ROESERVICES == STD_ON)
  PduIdType RxPduId = 0U;
  Std_ReturnType GetRxPduIdResult;
#endif /* if(DCM_DSP_USE_ROESERVICES == STD_ON) */

  boolean retValTxEnabled = FALSE;

#if(DCM_DSP_USE_ROESERVICES == STD_ON)
  /* Get the network channel */
  GetRxPduIdResult = Dcm_Dsl_RoeServices_GetRxPduId(&RxPduId);
  if (DCM_E_OK == GetRxPduIdResult)
  {
    Dcm_RxPduIdInfoType RxPduIdInfo;
    Dcm_ComMChannelOperationStateType ComMChnTxState = DCM_COMM_OPERATION_DISABLED;

    Dcm_Dsl_RxConnectionGetRxPduIdInformation(RxPduId, &RxPduIdInfo);

    ComMChnTxState = Dcm_Dsl_ComMChannelStateManager_GetTxState(RxPduIdInfo.ComMChannelId);
    if (DCM_COMM_OPERATION_ENABLED == ComMChnTxState)
    {
      retValTxEnabled = TRUE;
    }
    else
    {
      retValTxEnabled = FALSE;
    }
  }
  else
  {
    /* The event does not exist */
    retValTxEnabled = FALSE;
  }
#endif /* if(DCM_DSP_USE_ROESERVICES == STD_ON) */
  return retValTxEnabled;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[end of file]===========================================*/
