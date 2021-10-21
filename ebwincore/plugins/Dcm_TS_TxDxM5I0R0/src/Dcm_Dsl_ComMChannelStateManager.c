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

/* !LINKSTO Dcm.Dsn.File.ComMChannelStateManager.Impl,1 */
/* This file contains the implementation of the ComMChannelStateManager_
   software unit. */

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

/* This file contains the implementation of the ComMChannelStateManager
   software unit. */


/*==================[inclusions]=================================================================*/

#include <ComStack_Types.h>                               /* AUTOSAR Communication Stack types */
#include <TSAutosar.h>

#include <Dcm_Dsl_ComMChannelStateManager.h>              /* Dcm Dsl CComMChannelStateManager header
                                                             file */


#include <Dcm.h>                                          /* Definition of DCM_MODULE_ID */
#include <Dcm_Int.h>                                      /* Module intenal definitions */
#include <TSMem.h>                                        /* EB specific memory functions */
#include <SchM_Dcm.h>                                     /* SchM interface for Dcm */
#include <ComM_Dcm.h>                                     /* ComM interface for Dcm */
#include <Dcm_Trace.h>
#include <Dcm_Cbk.h>
#if (DCM_INCLUDE_RTE == STD_ON)
#include <Rte_Dcm.h>                                                /* Dcm Mainfunction prototype*/
#endif

/*==================[macros]=====================================================================*/

#if (defined DCM_COMM_RX)
#error DCM_COMM_RX already defined
#endif /* if (defined DCM_COMM_RX) */

/** \brief ID code of reception operation allowed on communication channel */
#define DCM_COMM_RX                     1U

#if (defined DCM_COMM_TX)
#error DCM_COMM_TX already defined
#endif /* if (defined DCM_COMM_TX) */

/** \brief ID code of transmission operation allowed on communication channel */
#define DCM_COMM_TX                     2U



/*==================[type definitions]===========================================================*/

/** \brief Type definitions a type for ComMChannel on the Dcm Module
 *
 *  This type is a bits field thats store the allowed operations on a communication channel
 *  as it is follow :
 *  - bit 0 = set allow reception on this channels
 *  - bit 1 = set allow transmission on this channels
 *  - bit 2-7 = reserved
 */
typedef uint8 Dcm_ComMChannelOperationEnabledType;

/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Initializes ChannelStatus array.
 *
 *  \return none
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsl_ComMChannelStateManger_ChannelInfoInit(void);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/
#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

#if (DCM_DSL_ENABLE_SET_ACTIVE_DIAGNOSIS == STD_ON)
/* !LINKSTO Dcm.Dsn.ComMChannelStateManager.ActiveDiagnostic,1 */
/** \brief Activate and deactivate the call of ComM_DCM_ActiveDiagnostic() function. */
STATIC VAR(Dcm_ActiveDiagnosticType, DCM_VAR) Dcm_ActiveDiagnostic;
#endif /* #if (DCM_DSL_ENABLE_SET_ACTIVE_DIAGNOSIS == STD_ON) */

#if (DCM_COMM_MULTICORE_ENABLED == STD_ON)
/** \brief Is TRUE if a ComM operation failed. */
STATIC VAR(boolean, DCM_VAR) Dcm_ComMOperationFailed;
#endif /* #if (DCM_COMM_MULTICORE_ENABLED == STD_ON) */

/* Rationale for mapping the Tx and Rx Operations related to communication
 * channels on NOINIT section.
 *
 * This array is initialized in Dcm_Init before being accessed from functions
 * Dcm_DslInternal_ResponseOnOneEvent, Dcm_HsmSfOutOfServiceGuard3 or
 * Dcm_HsmSfOutOfServiceGuard5 or from Dcm_StartOfReception on case of rx.
 */
/** \brief Map into an array of the transmission and reception operations which are allowed
 *         on each communication channel, as mask of bits that makes easier atomic operations. */
STATIC VAR(Dcm_ComMChannelOperationEnabledType, DCM_VAR) Dcm_ComMChannelOperationEnabled[DCM_NUM_RX_COMM_CHANNELS];

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.DiagnosticSessionLayer.RuntimeDataElements,1 */
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief Contains the active state of all the ComM Channels.
 */
VAR(Dcm_ComMChannelInfoType, DCM_VAR) Dcm_ComMChannelInfo[DCM_NUM_RX_COMM_CHANNELS];

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.IB.ComMChannelStateManager.Init,1 */
/* Function disable for all the ComMChannels the ComM operations */
FUNC(void, DCM_CODE) Dcm_Dsl_ComMChannelStateManager_Init(void)
{
  uint8_least ComMChannelIndex;

  DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_INIT_ENTRY();
#if (DCM_DSL_ENABLE_SET_ACTIVE_DIAGNOSIS == STD_ON)
  /* !LINKSTO Dcm.EB.CommunicationModeHandling.SetActiveDiagnostic.Init,1 */
  Dcm_ActiveDiagnostic = DCM_COMM_ACTIVE;
#endif

#if (DCM_COMM_MULTICORE_ENABLED == STD_ON)
  Dcm_ComMOperationFailed = FALSE;
#endif /* #if (DCM_COMM_MULTICORE_ENABLED == STD_ON) */

  Dcm_Dsl_ComMChannelStateManger_ChannelInfoInit();

  /* Disabling the diagnostic response transmission and reception. */
 /* Deviation TASKING-1 */
  for (ComMChannelIndex = 0U; ComMChannelIndex < DCM_NUM_RX_COMM_CHANNELS; ComMChannelIndex++)
  {
    Dcm_ComMChannelOperationEnabled[ComMChannelIndex] &= ((uint8)~(DCM_COMM_RX | DCM_COMM_TX));
  }

  DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_INIT_EXIT();
}

/* !LINKSTO Dcm.Dsn.IB.ComMChannelStateManager.GetRxState,1 */
/* Function checks if the ComMChannelID that is given as a parameter,
 * it is set to enable a reception operation */
FUNC(Dcm_ComMChannelOperationStateType, DCM_CODE) Dcm_Dsl_ComMChannelStateManager_GetRxState(
  NetworkHandleType ComMChannelID
)
{
  uint8_least ChannelIndex;
  boolean Found = FALSE;

  Dcm_ComMChannelOperationStateType RxStateResult = DCM_COMM_OPERATION_DISABLED;

  DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_GETRXSTATE_ENTRY(ComMChannelID);

  for (ChannelIndex = 0U; ((ChannelIndex < DCM_NUM_RX_COMM_CHANNELS)&&(Found == FALSE)); ChannelIndex++)
  {
    if(Dcm_ComMChannelInfo[ChannelIndex].NetworkID == ComMChannelID)
    {
      if ((Dcm_ComMChannelOperationEnabled[ChannelIndex] & DCM_COMM_RX) ==  DCM_COMM_RX)
      {
        RxStateResult = DCM_COMM_OPERATION_ENABLED;
      }
      Found = TRUE;
    }
  }

  DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_GETRXSTATE_EXIT(RxStateResult, ComMChannelID);

  return RxStateResult;
}

/* !LINKSTO Dcm.Dsn.IB.ComMChannelStateManager.GetTxState,1 */
/* Function checks if the ComMChannelID that is given as a parameter,
 * it is set to enable a transmission operation */
FUNC(Dcm_ComMChannelOperationStateType, DCM_CODE) Dcm_Dsl_ComMChannelStateManager_GetTxState(
  NetworkHandleType ComMChannelID
)
{
  uint8_least ChannelIndex;
  boolean Found = FALSE;

  Dcm_ComMChannelOperationStateType TxStateResult = DCM_COMM_OPERATION_DISABLED;

  DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_GETTXSTATE_ENTRY(ComMChannelID);

  for (ChannelIndex = 0U; ((ChannelIndex < DCM_NUM_RX_COMM_CHANNELS)&&(Found == FALSE)); ChannelIndex++)
  {
    if(Dcm_ComMChannelInfo[ChannelIndex].NetworkID == ComMChannelID)
    {
      if ((Dcm_ComMChannelOperationEnabled[ChannelIndex] & DCM_COMM_TX) ==  DCM_COMM_TX)
      {
        TxStateResult = DCM_COMM_OPERATION_ENABLED;
      }
      Found = TRUE;
    }
  }

  DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_GETTXSTATE_EXIT(TxStateResult, ComMChannelID);

  return TxStateResult;
}

/* !LINKSTO Dcm.EB.CommunicationModeHandling.NoCommunicationMode.Interface,1 */
FUNC(void, DCM_CODE) Dcm_ComM_NoComModeEntered(uint8 NetworkId)
{
  DBG_DCM_COMM_NOCOMMODEENTERED_ENTRY(NetworkId);

  Dcm_Dsl_ComMChannelStateManager_EnterMode(NetworkId, DCM_NO_COMMUNICATION_MODE);

  DBG_DCM_COMM_NOCOMMODEENTERED_EXIT(NetworkId);
}

/* !LINKSTO Dcm.EB.CommunicationModeHandling.SilentCommunicationMode.Interface,1 */
FUNC(void, DCM_CODE) Dcm_ComM_SilentComModeEntered(uint8 NetworkId)
{
  DBG_DCM_COMM_SILENTCOMMODEENTERED_ENTRY(NetworkId);

  Dcm_Dsl_ComMChannelStateManager_EnterMode(NetworkId, DCM_SILENT_COMMUNICATION_MODE);

  DBG_DCM_COMM_SILENTCOMMODEENTERED_EXIT(NetworkId);
}

/* !LINKSTO Dcm.EB.CommunicationModeHandling.FullCommunicationMode.Interface,1 */
FUNC(void, DCM_CODE) Dcm_ComM_FullComModeEntered(uint8 NetworkId)
{
  DBG_DCM_COMM_FULLCOMMODEENTERED_ENTRY(NetworkId);

  Dcm_Dsl_ComMChannelStateManager_EnterMode(NetworkId, DCM_FULL_COMMUNICATION_MODE);

  DBG_DCM_COMM_FULLCOMMODEENTERED_EXIT(NetworkId);
}

/* !LINKSTO Dcm.Dsn.IB.ComMChannelStateManager.EnterMode,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_ComMChannelStateManager_EnterMode(
  Dcm_NetworkIdType NetworkId,
  Dcm_ComMChannelModeType CommunicationMode
)
{
#if (DCM_DEV_ERROR_DETECT == STD_ON)
  uint8 CallerServiceIdOnError = DCM_INTERNAL_API_ID;
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  uint8 EnableOp = 0U;
  uint8_least ChannelIndex;

  DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_ENTERMODE_ENTRY(NetworkId, CommunicationMode);

  switch (CommunicationMode)
  {
    case DCM_NO_COMMUNICATION_MODE:
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.NoCommunicationMode.Uninit.DET,1 */
      CallerServiceIdOnError = DCM_SERVID_COMMNOCOMMODEENTERED;
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
      break;

    case DCM_SILENT_COMMUNICATION_MODE:
      EnableOp = DCM_COMM_RX;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.SilentCommunicationMode.Uninit.DET,1 */
      CallerServiceIdOnError = DCM_SERVID_COMMSILENTCOMMODEENTERED;
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
      break;

    case DCM_FULL_COMMUNICATION_MODE:
      EnableOp = (DCM_COMM_RX | DCM_COMM_TX);
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.FullCommunicationMode.Uninit.DET,1 */
      CallerServiceIdOnError = DCM_SERVID_COMMFULLCOMMODEENTERED;
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
      break;

    /* CHECK: NOPARSE*/
    default:
      /* Unreachable default case mandated by MISRA */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE*/
  }
#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(CallerServiceIdOnError, DCM_E_UNINIT);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Deviation TASKING-1 */
    for (ChannelIndex = 0U; ChannelIndex < DCM_NUM_RX_COMM_CHANNELS; ChannelIndex++)
    {
      if(Dcm_ComMChannelInfo[ChannelIndex].NetworkID == NetworkId)
      {

        /* ENTER critical section
           The paired writing of Dcm_TxEnabled and Dcm_RxEnabled must be protected from preemption
         */
        SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

        Dcm_ComMChannelOperationEnabled[ChannelIndex] = EnableOp;

        /* LEAVE critical section */
        SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

        break;
      }
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.NoCommunicationMode.InvalidNetworkId.IgnoreNotification,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.FullCommunicationMode.InvalidNetworkId.IgnoreNotification,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.SilentCommunicationMode.InvalidNetworkId.IgnoreNotification,1 */
      /* No else branch in order to fulfill SWS_Dcm_01324, SWS_Dcm_01325 and SWS_Dcm_01326 */
    }
  }

  DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_ENTERMODE_EXIT(NetworkId, CommunicationMode);
}

#if (DCM_DSL_ENABLE_SET_ACTIVE_DIAGNOSIS == STD_ON)
/* !LINKSTO Dcm.EB.CommunicationModeHandling.SetActiveDiagnostic.Interface,1 */
/* !LINKSTO Dcm.Dsn.IB.ComMChannelStateManager.SetActiveDiagnostic,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_SetActiveDiagnostic(boolean active)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_DCM_SETACTIVEDIAGNOSTIC_ENTRY(active);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO Dcm.EB.CommunicationModeHandling.SetActiveDiagnostic.Uninit.DET,1 */
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_SETACTIVEDIAGNOSTIC, DCM_E_UNINIT);
  }
  else
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */
  {
    if (active == TRUE)
    {
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.SetActiveDiagnostic.TRUE,1 */
      Dcm_ActiveDiagnostic = DCM_COMM_ACTIVE;
    }
    else
    {
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.SetActiveDiagnostic.FALSE,1 */
      Dcm_ActiveDiagnostic = DCM_COMM_NOT_ACTIVE;
    }

    retVal = E_OK;
  }

  DBG_DCM_SETACTIVEDIAGNOSTIC_EXIT(active, retVal);

  return retVal;
}

#endif /* #if (DCM_DSL_ENABLE_SET_ACTIVE_DIAGNOSIS == STD_ON) */

/* !LINKSTO Dcm.Dsn.IB.ComMChannelStateManager.RequestActiveDiagnostic,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_ComMChannelStateManager_RequestActiveDiagnostic(
  NetworkHandleType NetworkId
)
{
  uint8_least NetworkStatusArrayIndex;
  boolean NetworkStatusFound = FALSE;
  Std_ReturnType Result = DCM_E_NOT_OK;

  DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_REQUESTACTIVEDIAGNOSTIC_ENTRY(NetworkId);


#if (DCM_DSL_ENABLE_SET_ACTIVE_DIAGNOSIS == STD_ON)
  if (Dcm_ActiveDiagnostic == DCM_COMM_ACTIVE)
  {
#endif /* #if (DCM_DSL_ENABLE_SET_ACTIVE_DIAGNOSIS == STD_ON) */

    /* Request the ComManager for the full communication */
    for(NetworkStatusArrayIndex = 0;
        ((NetworkStatusArrayIndex < DCM_NUM_RX_COMM_CHANNELS)&&(NetworkStatusFound == FALSE));
        NetworkStatusArrayIndex++)
    {
      /* Mark ComMChannel as active */
      if(NetworkId == Dcm_ComMChannelInfo[NetworkStatusArrayIndex].NetworkID)
      {
        if (FALSE == Dcm_ComMChannelInfo[NetworkStatusArrayIndex].ActiveFlag)
        {
        /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ActiveDiagnosisCOMM_ACTIVE.ActiveDiagnostic,1 */
        /* !LINKSTO Dcm.EB.CommunicationModeHandling.ImmediateApplicationRequest.ActiveDiagnosisCOMM_ACTIVE.ActiveDiagnostic,1 */
        /* !LINKSTO Dcm.EB.CommunicationModeHandling.DelayedApplicationRequest.ActiveDiagnosisCOMM_ACTIVE.ActiveDiagnostic,1 */
        /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ActiveDiagnosisFALSE.ActiveDiagnostic,1 */
        /* !LINKSTO Dcm.EB.CommunicationModeHandling.ImmediateApplicationRequest.ActiveDiagnosisFALSE.ActiveDiagnostic,1 */
        /* !LINKSTO Dcm.EB.CommunicationModeHandling.DelayedApplicationRequest.ActiveDiagnosisFALSE.ActiveDiagnostic,1 */
        /* !LINKSTO Dcm.EB.CommunicationModeHandling.WarmStart.JumpFromBootloader.ActiveDiagnosisCOMM_ACTIVE.ActiveDiagnostic,1 */
        /* !LINKSTO Dcm.EB.CommunicationModeHandling.WarmStart.JumpFromECUReset.ActiveDiagnosisCOMM_ACTIVE.ActiveDiagnostic,1 */
        /* !LINKSTO Dcm.EB.CommunicationModeHandling.WarmStart.JumpFromBootloader.ActiveDiagnosisFALSE.ActiveDiagnostic,1 */
        /* !LINKSTO Dcm.EB.CommunicationModeHandling.WarmStart.JumpFromECUReset.ActiveDiagnosisFALSE.ActiveDiagnostic,1 */
        /* !LINKSTO Dcm.EB.CommunicationModeHandling.SessionChangeToNonDefault.ActiveDiagnosisCOMM_ACTIVE.ActiveDiagnostic,1 */
        /* !LINKSTO Dcm.EB.CommunicationModeHandling.SessionChangeToNonDefault.ActiveDiagnosisFALSE.ActiveDiagnostic,1 */
#if (DCM_COMM_MULTICORE_ENABLED == STD_OFF)
          ComM_DCM_ActiveDiagnostic(NetworkId);
          Result = DCM_E_OK;

#else /* #if (DCM_COMM_MULTICORE_ENABLED == STD_OFF) */
          uint8_least NetworkSchmSwitchArrayIndex;
          boolean ChannelSchMSwitchFound = FALSE;

          /* !LINKSTO Dcm.EB.CommunicationModeHandling.SupportOfDcmAndComMOnDifferentPartitionsCores.SchM_Call_ComM_Dcm_ActiveDiagnostic.Call,2 */
          for(NetworkSchmSwitchArrayIndex = 0;
              ((NetworkSchmSwitchArrayIndex < DCM_NUM_RX_COMM_CHANNELS)&&(ChannelSchMSwitchFound == FALSE));
              NetworkSchmSwitchArrayIndex++)
          {
            if(NetworkId == Dcm_DiagnosticNotificationLookUpTable[NetworkSchmSwitchArrayIndex].ComMChannelId)
            {
              if (Dcm_DiagnosticNotificationLookUpTable[NetworkSchmSwitchArrayIndex].ActiveDiagnostic(NetworkId) == SCHM_E_OK)
              {
                Result = DCM_E_OK;
              }
              else
              {

                Dcm_ComMOperationFailed = TRUE;

#if (DCM_DEV_ERROR_DETECT == STD_ON)
                /* !LINKSTO Dcm.EB.CommunicationModeHandling.SupportOfDcmAndComMOnDifferentPartitionsCores.SchM_Call_ComM_Dcm_ActiveDiagnostic.NotOk,2 */
                /* There are multiple outstanding asynchronous calls */
                DCM_DET_REPORT_ERROR(DCM_SERVID_MAINFUNCTION, DCM_E_SCHM_CALL_ACTIVEDIAGNOSTIC_FAILED);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
              }

              ChannelSchMSwitchFound = TRUE;
            }
          }
#endif /* #if (DCM_COMM_MULTICORE_ENABLED == STD_OFF) */

          Dcm_ComMChannelInfo[NetworkStatusArrayIndex].ActiveFlag = TRUE;
        }

        NetworkStatusFound = TRUE;
      }
    }
#if (DCM_DSL_ENABLE_SET_ACTIVE_DIAGNOSIS == STD_ON)
  }
#endif /* #if (DCM_DSL_ENABLE_SET_ACTIVE_DIAGNOSIS == STD_ON)*/

  DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_REQUESTACTIVEDIAGNOSTIC_EXIT(NetworkId);
  return Result;
}

/* !LINKSTO Dcm.Dsn.IB.ComMChannelStateManager.RequestInactiveDiagnostic,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_ComMChannelStateManager_RequestInactiveDiagnostic(
  NetworkHandleType NetworkId
)
{
  uint8_least NetworkStatusArrayIndex;
  boolean NetworkStatusFound = FALSE;
  Std_ReturnType Result = DCM_E_NOT_OK;


  DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_REQUESTINACTIVEDIAGNOSTIC_ENTRY();

  /* Request the ComManager to drop communication */
  for(NetworkStatusArrayIndex = 0;
     ((NetworkStatusArrayIndex < DCM_NUM_RX_COMM_CHANNELS)&&(NetworkStatusFound == FALSE));
     NetworkStatusArrayIndex++)
  {
    if(NetworkId == Dcm_ComMChannelInfo[NetworkStatusArrayIndex].NetworkID)
    {
      if(Dcm_ComMChannelInfo[NetworkStatusArrayIndex].ActiveFlag == TRUE)
      {
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.NonDefaultSession.OBDRequestReceived.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ProtocolPreemption.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.NonDefaultSession.S3Timeout.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ResetToDefaultSessionAPI.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ResetToDefaultSessionClientServerOperation.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ProcessingComplete.DefaultSession.SuppressedPositiveResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.WarmStart.ProcessingComplete.DefaultSession.SuppressedPositiveResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ProcessingComplete.DefaultSession.SuppressedFunctionalNegativeResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ProcessingComplete.DefaultSession.SuppressedOBDNegativeResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ProcessingComplete.DefaultSession.ProcessingCancelled.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ProcessingComplete.DefaultSession.ManufacturerAbortWithoutResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ProcessingComplete.DefaultSession.SupplierAbortWithoutResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.ProcessingComplete.DefaultSession.SuppressedPositiveResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.ProcessingComplete.DefaultSession.SuppressedFunctionalNegativeResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.ProcessingComplete.DefaultSession.SuppressedOBDNegativeResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.ProcessingComplete.DefaultSession.ProcessingCancelled.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.ProcessingComplete.DefaultSession.ManufacturerAbortWithoutResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.ProcessingComplete.DefaultSession.SupplierAbortWithoutResponse.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ProcessingComplete.DefaultSession.TxConfirmation.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ProcessingComplete.DefaultSession.NoFullComMode.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.WarmStart.ProcessingComplete.NoFullComMode.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.ProcessingComplete.DefaultSession.DcmTransmitNotOK.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.WarmStart.ProcessingComplete.DcmTransmitNotOK.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.ProcessingComplete.DefaultSession.TxConfirmation.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.WarmStart.ProcessingComplete.DefaultSession.TxConfirmation.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.ProcessingComplete.DefaultSession.NoFullComMode.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.ProcessingComplete.DefaultSession.DcmTransmitNotOK.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.DefaultSession.PreemptionTimeoutBBR.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.DefaultSession.PreemptionTimeoutBBR.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.DefaultSession.ProtocolStartConditionsNotCorrect.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.DefaultSession.ProtocolStartConditionsNotCorrect.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.WarmStart.ProtocolStartConditionsNotCorrect.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.DefaultSession.ProtocolStopConditionsNotCorrect.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.DefaultSession.ProtocolStopConditionsNotCorrect.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.DefaultSession.NewProtocolNotStarted.RequestCorrectlyReceivedResponsePending.TxConfirmationNotOK.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.DefaultSession.NewProtocolNotStarted.RequestCorrectlyReceivedResponsePending.TxConfirmationNotOK.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.DefaultSession.NewProtocolNotStarted.RequestCorrectlyReceivedResponsePending.DcmTransmitNotOK.InactiveDiagnostic ,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.DefaultSession.NewProtocolNotStarted.RequestCorrectlyReceivedResponsePending.DcmTransmitNotOK.InactiveDiagnostic ,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.DefaultSession.NewProtocolNotStarted.RequestCorrectlyReceivedResponsePending.NoFullComMode.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.DefaultSession.NewProtocolNotStarted.RequestCorrectlyReceivedResponsePending.NoFullComMode.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.DefaultSession.NewProtocolStarted.RequestCorrectlyReceivedResponsePending.TxConfirmationNotOK.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.DefaultSession.NewProtocolStarted.RequestCorrectlyReceivedResponsePending.TxConfirmationNotOK.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.DefaultSession.NewProtocolStarted.RequestCorrectlyReceivedResponsePending.DcmTransmitNotOK.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.DefaultSession.NewProtocolStarted.RequestCorrectlyReceivedResponsePending.DcmTransmitNotOK.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.External.DefaultSession.NewProtocolStarted.RequestCorrectlyReceivedResponsePending.NoFullComMode.InactiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.ApplicationRequest.DefaultSession.NewProtocolStarted.RequestCorrectlyReceivedResponsePending.NoFullComMode.InactiveDiagnostic,1 */

#if (DCM_COMM_MULTICORE_ENABLED == STD_OFF)
        ComM_DCM_InactiveDiagnostic(Dcm_ComMChannelInfo[NetworkStatusArrayIndex].NetworkID);

        Result = DCM_E_OK;
#else /* #if (DCM_COMM_MULTICORE_ENABLED == STD_OFF) */
        uint8_least NetworkSchmSwitchArrayIndex;
        boolean ChannelSchMSwitchFound = FALSE;

        /* !LINKSTO Dcm.EB.CommunicationModeHandling.SupportOfDcmAndComMOnDifferentPartitionsCores.SchM_Call_ComM_Dcm_InactiveDiagnostic.Call,2 */
        for(NetworkSchmSwitchArrayIndex = 0;
            ((NetworkSchmSwitchArrayIndex < DCM_NUM_RX_COMM_CHANNELS)&&(ChannelSchMSwitchFound == FALSE));
            NetworkSchmSwitchArrayIndex++)
        {
          if(NetworkId == Dcm_DiagnosticNotificationLookUpTable[NetworkSchmSwitchArrayIndex].ComMChannelId)
          {
            if (Dcm_DiagnosticNotificationLookUpTable[NetworkSchmSwitchArrayIndex].InactiveDiagnostic(NetworkId) == SCHM_E_OK)
            {
              Result = DCM_E_OK;
            }
            else
            {
              Dcm_ComMOperationFailed = TRUE;

#if (DCM_DEV_ERROR_DETECT == STD_ON)
              /* !LINKSTO Dcm.EB.CommunicationModeHandling.SupportOfDcmAndComMOnDifferentPartitionsCores.SchM_Call_ComM_Dcm_InactiveDiagnostic.NotOk,2 */
              /* There are multiple outstanding asynchronous calls */
              DCM_DET_REPORT_ERROR(DCM_SERVID_MAINFUNCTION, DCM_E_SCHM_CALL_INACTIVEDIAGNOSTIC_FAILED);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
            }

            ChannelSchMSwitchFound = TRUE;
          }
        }
#endif /* #if (DCM_COMM_MULTICORE_ENABLED == STD_OFF) */

        Dcm_ComMChannelInfo[NetworkStatusArrayIndex].ActiveFlag = FALSE;
      }

      NetworkStatusFound = TRUE;
    }
  }

  DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_REQUESTINACTIVEDIAGNOSTIC_EXIT();
  return Result;
}

#if (DCM_COMM_MULTICORE_ENABLED == STD_ON)
FUNC(boolean, DCM_CODE) Dcm_Dsl_ComMChannelStateManager_GetAndResetOperationFailedState(
  void
)
{
  boolean LocalComMOperationFailed = Dcm_ComMOperationFailed;

  Dcm_ComMOperationFailed = FALSE;

  return LocalComMOperationFailed;
}
#endif /* #if (DCM_COMM_MULTICORE_ENABLED == STD_ON) */


#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

STATIC FUNC(void, DCM_CODE) Dcm_Dsl_ComMChannelStateManger_ChannelInfoInit(
  void
)
{
  uint8_least ChannelIndex;
  /* Deviation TASKING-1 <+2> */
  for (ChannelIndex = 0;
      ChannelIndex < DCM_NUM_RX_COMM_CHANNELS;
      ChannelIndex++ )
  {
    Dcm_ComMChannelInfo[ChannelIndex].NetworkID = Dcm_RxComMChannelIDs[ChannelIndex];
    Dcm_ComMChannelInfo[ChannelIndex].ActiveFlag = FALSE;
  }
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*==================[end of file]================================================================*/
