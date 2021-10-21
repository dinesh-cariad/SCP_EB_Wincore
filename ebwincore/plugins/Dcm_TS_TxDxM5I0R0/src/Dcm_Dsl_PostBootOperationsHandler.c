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

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 15.2 (required)
 *     An unconditional break statement shall terminate every non-empty switch clause.
 *
 *     Reason:
 *     The following actions have to be executed in the same state so the break
 *     statement is removed in order to have shorter code and avoid duplicated code.
 *
 */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 16.3 (required)
  *     "An unconditional break statement shall terminate every switch-clause."
  *
  *     Reason:
  *     The following actions have to be executed in the same state so the break
  *     statement is removed in order to have shorter code and avoid duplicated code.
  *
  *
  *  MISRAC2012-2) Deviated Rule: 8.9 (advisory)
  *     "An object should be defined at block scope if its identifier only appears
  *     in a single function."
  *
  *     Reason:
  *     The variable must be defined in a global scope as a pointer to this memory space
  *     is supplied outside the function using it. Placing it on the stack would not be
  *     correct, and static local variables are not allowed by AUTOSAR.
  *
  *  MISRAC2012-3) Deviated Rule: 16.1 (required)
  *     "All switch statements shall be well-formed"
  *
  *     Reason:
  *     This violation is present because rule 16.3 is violated and justified inside the affected
  *     switch statement, as noted under rule 16.1:
  *     some of the restrictions imposed on switch statements by this rule are expounded in the
  *     16.3 rule, which is violated inside the current switch statement.
  */
/* !LINKSTO Dcm.Dsn.File.PostBootOperationsHandler.Impl,1 */
/* This file contains the implementation of the PostBootOperationsHandler */

/*===============================[includes]======================================================*/

#include <Dcm_Int.h>                             /* Internal Dcm declarations for things such as
                                                    service IDs and shared types. Also contains
                                                    Dcm.h and includes Common Dcm APIs, such as
                                                    the ones for getting and setting programming
                                                    conditions.*/
#if (DCM_BSWM_API_ENABLED == STD_ON)
#include <BswM_DCM.h>                            /* BswM interface for Dcm for setting status of
                                                    updated application. */
#endif /* #if (DCM_BSWM_API_ENABLED == STD_ON) */
#include <Dcm_Dsl_PostBootOperationsHandler.h>
#include <Dcm_Dsl_CommunicationServices.h>       /* CommunicationServices API for generating a
                                                    request after boot-up. */
#include <Dcm_Dsl_Supervisor.h>               /* Supervisor API for permissions before the
                                                    request. */
#include <SchM_Dcm.h>                            /* SchM interface for Dcm */

/*=======================[internal function declarations]========================================*/

/*=======================[internal data declarations]============================================*/
#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/** \brief Variable holding the state of the PostBootOperationsHandler.  */
STATIC VAR(Dcm_PostBootOperationsStateType, DCM_VAR) Dcm_PostBootOperationsState;

/** \brief Buffer which holds the service-to-respond-to data for a PostBootOperation request. */
/* Deviation MISRAC2012-2 */
STATIC VAR(uint8, DCM_VAR) Dcm_BootloaderRequestToRespondTo[2];

/** \brief Inhibits jumping to the Bootloader in order to avoid Dcm_ProgConditions corruption
           TRUE - jump to the Bootloader not allowed
           FALSE - jump to the Bootloader allowed */
STATIC VAR(boolean, DCM_VAR) Dcm_InhibitJumpToBootloader;

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>
/*=======================[external function definitions]=========================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.IA.PostBootOperationsHandle.Init,1 */
/* Handles jump to/from Bootloader actions */
FUNC(void, DCM_CODE) Dcm_Dsl_PostBootOperationsHandler_Init(void)
{
  Dcm_EcuStartModeType EcuStartModeType;

  Dcm_PostBootOperationsState = DCM_POSTBOOT_OPERATIONS_FINISHED;

  /* !LINKSTO Dcm.EB.ServiceProcessing.Init.GetProgConditions.Bootloader,1 */
  /* !LINKSTO Dcm.EB.ServiceProcessing.Init.GetProgConditions.ECUReset,1 */
  /* Call Dcm_GetProgConditions() to know if the initialization is the
     consequence of a jump from the bootloader/ECUReset */
  EcuStartModeType = Dcm_GetProgConditions(&Dcm_ProgConditions);

  /* The ECU starts from a bootloader jump */
  if ( EcuStartModeType == DCM_WARM_START )
  {
#if (DCM_BSWM_API_ENABLED == STD_ON)
    /* !LINKSTO Dcm768,1 */
    if ( (DCM_SID_SESSION_CONTROL == Dcm_ProgConditions.Sid) && (TRUE == Dcm_ProgConditions.ApplUpdated) )
    {
      /* Invoke BswM_Dcm_ApplicationUpdated() only if the ApplUpdated flag in Dcm_ProgConditionsType is
         set to true and the jump from the bootloader was due to UDS Service DiagnosticSessionControl (0x10).*/
      BswM_Dcm_ApplicationUpdated();
    }
#endif

    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.UnsuppressedResponse.PduR_DcmTransmit.ReturnsNotE_OK.RejectRx,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.UnsuppressedResponse.TpTxConfirmation.ReturnsNotE_OK.RejectRx,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.UnsuppressedResponse.DcmComMMultiCoreSupportDisabled.FullCommunicationDisabled.RejectRx,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.SuppressedResponse.DcmComMMultiCoreSupportDisabled.FullCommunicationDisabled.RejectRx,1 */
    /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.UnsuppressedResponse.TpTxConfirmation.ReturnsE_OK.RejectRx,1 */

    /* Set the flag Dcm_InhibitRequestProcessing to TRUE in order to reject any subsequent Dcm_StartOfReception() calls with BUFREQ_E_NOT_OK
       until the Dcm has finished the transmission of final response. Upon successful transmission of the final response, the DSP calls the function
       Dcm_Dsl_PostBootOperationsHandler_ActionsCompleted() which sets the next state of PostBootOperationsHandler to DCM_POSTBOOT_ACTIONS_AFTER_PROCESSING
       in which the Rx is enabled again.*/

    Dcm_Dsl_Supervisor_InhibitRequestProcessing();

    /* Set the flag to indicate protocol preemption is pending */
    Dcm_PostBootOperationsState = DCM_POSTBOOT_INITIALIZE_WARM_START;
  }
  else
  {
     /* Set the flag to allow request */
     Dcm_Dsl_Supervisor_DisinhibitRequestProcessing();
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    if (EcuStartModeType != DCM_COLD_START)
    {
      /* !LINKSTO Dcm.EB.ServiceProcessing.DetEnabled.GetProgConditions.UnknownReturnValues,1 */
      DCM_DET_REPORT_ERROR(DCM_SVCID_GETPROGCONDITIONS, DCM_E_INTERFACE_RETURN_VALUE);
    }
#endif
  }

  Dcm_InhibitJumpToBootloader = FALSE;
}

/* !LINKSTO Dcm.Dsn.IA.PostBootOperationsHandle.ActionsCompleted,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_PostBootOperationsHandler_ActionsCompleted(
  void
)
{
  Dcm_PostBootOperationsState = DCM_POSTBOOT_ACTIONS_AFTER_PROCESSING;
}

/* !LINKSTO Dcm.Dsn.IA.PostBootOperationsHandle.JumpInhibited,1 */
FUNC (boolean, DCM_CODE) Dcm_Dsl_PostBootOperationsHandler_JumpInhibited(
  void
)
{
  return Dcm_InhibitJumpToBootloader;
}

/* !LINKSTO Dcm.Dsn.IA.PostBootOperationsHandle.MainFunction,1 */
FUNC (void, DCM_CODE) Dcm_Dsl_PostBootOperationsHandler_MainFunction(
  void
)
{
  /* Deviation MISRAC2012-3 */
  switch (Dcm_PostBootOperationsState)
  {
    case DCM_POSTBOOT_INITIALIZE_WARM_START:
    {
      boolean TesterSourceAddrFound = FALSE;

      /* Variable used to loop in RxPduIdConfig. */
      uint16_least RxPduIdx = 0U;

      /* Get the 'RxPduId' corresponding to the 'TesterSourceAddr' */
      while( (RxPduIdx < DCM_NUM_RX_PDU_ID) && (FALSE == TesterSourceAddrFound) )
      {
        if (Dcm_MainConnectionConfig[Dcm_RxPduIdConfig[RxPduIdx].MainConnectionIndex].TesterSourceAddress ==
          Dcm_ProgConditions.TesterSourceAddr)
        {
          TesterSourceAddrFound = TRUE;
        }
        else
        {
          RxPduIdx++;
        }
      }

      /* If the 'TesterSourceAddr' is not configured, report error */
      if (FALSE == TesterSourceAddrFound)
      {

#if (DCM_DEV_ERROR_DETECT == STD_ON)
        DCM_DET_REPORT_ERROR(DCM_SVCID_GETPROGCONDITIONS, DCM_E_PARAM);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */

        /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.UnsuppressedResponse.TesterSourceAddress.NotConfigured.RejectRx,1 */
        /* !LINKSTO Dcm.EB.ServiceProcessing.WarmStart.SuppressedResponse.TesterSourceAddress.NotConfigured.RejectRx,1 */
        /* !LINKSTO Dcm.EB.JumpToBl.InvalidGetProgConditions.SetProgConditionsCall.ResetResponseRequired,1 */
        /* !LINKSTO Dcm.EB.JumpToBl.InvalidGetProgConditions.SetProgConditionsCall.ResetApplUpdated,1 */
        /* Set the state to DCM_POSTBOOT_ACTIONS_AFTER_PROCESSING to clear the ResponseRequired and ApplUpdated flags.*/
        Dcm_PostBootOperationsState = DCM_POSTBOOT_ACTIONS_AFTER_PROCESSING;
      }
      else
      {
        Dcm_RequestOriginType RequestOrigin;
        Dcm_RequestTypeType RequestType;


        /* Buffer containing the response */
        Dcm_BufferType BootloaderResponseRequest;

        /* Set the RxPduId on which the request was received */
        Dcm_RxPduIdBL = (PduIdType)RxPduIdx;

        Dcm_BootloaderRequestToRespondTo[0] = Dcm_ProgConditions.Sid;
        Dcm_BootloaderRequestToRespondTo[1] = Dcm_ProgConditions.SubFncId;

        BootloaderResponseRequest.BufferPtr = &Dcm_BootloaderRequestToRespondTo[0];
        BootloaderResponseRequest.FilledLength = 2U;
        BootloaderResponseRequest.Size = 2U;
        BootloaderResponseRequest.BufferId = DCM_BUFFER_INVALID_ID;

        if (FALSE == Dcm_ProgConditions.ResponseRequired)
        {
          RequestOrigin = DCM_INTERNAL_POST_BOOT_NO_ANSWER;
        }
        else
        {
          RequestOrigin = DCM_INTERNAL_POST_BOOT_WITH_ANSWER;
        }

        /* ENTER critical section */
        SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

        /* the message length (2nd parameter) is not processed in case of a post-boot request */
        RequestType =
          Dcm_Dsl_Supervisor_RequestRxPermission(Dcm_RxPduIdBL, 0U, RequestOrigin);

        DCM_POSTCONDITION_ASSERT(
          (
           (DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST == RequestType) ||
           (DCM_JUMP_FROM_BL_RESPONSE_REQUEST == RequestType)
          ), DCM_INTERNAL_API_ID);

        Dcm_Dsl_Supervisor_IncomingReception(Dcm_RxPduIdBL, RequestType);

        /* Hand over the internally generated request to the RxConnection corresponding to the
           memorized TesterSourceAddress. Note: such a "dummy" buffer has no ID and therefore no
           owner. */
        Dcm_Dsl_RxConnectionRxIndication(Dcm_RxPduIdBL,
                                         RequestType,
                                         BootloaderResponseRequest);

        Dcm_PostBootOperationsState = DCM_POSTBOOT_WAIT_FOR_PROCESSING;


        /* EXIT critical section */
        SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
      }

    break;
    }

    case DCM_POSTBOOT_WAIT_FOR_PROCESSING:
      /* Waits for the PostBoot response processing to be performed by the Supervisor/Protocol. Nothing to
         do here until we receive a go-ahead that the response, if any, has been transmitted, in order to
         contine setting the session. */
      break;

    /* Deviation MISRAC2012-1 */
    /* Deviation MISRA-1 */
    case DCM_POSTBOOT_ACTIONS_AFTER_PROCESSING:

      Dcm_Dsl_Supervisor_DisinhibitRequestProcessing();

      /* Check if 'ApplUpdated' or 'ResponseRequired' flag is set, so as to reset them */
      if ((Dcm_ProgConditions.ResponseRequired == TRUE) ||
          (Dcm_ProgConditions.ApplUpdated == TRUE))
      {

        /* !LINKSTO  Dcm.EB.ServiceProcessing.SetProgConditions.CleanupResponseRequired.Dcm_SetProgConditions.Call,1 */
        Dcm_ProgConditions.ResponseRequired = FALSE;
        /* !LINKSTO  Dcm.EB.ServiceProcessing.SetProgConditions.CleanupApplUpdated.Dcm_SetProgConditions.Call,1 */
        Dcm_ProgConditions.ApplUpdated = FALSE;
        /* Call Dcm_SetProgConditions() to clear the flags:
           Fall through to the next state so that further processing
           is carried out in the same cycle */
        Dcm_PostBootOperationsState = DCM_POSTBOOT_SET_PROGRAMMING_CONDITIONS;

        /* Disallow new jump to bootloader until Dcm_SetProgConditions() finishes.
           This is to avoid corrupting the Dcm_ProgConditions by a reentrant call to
           Dcm_SetProgConditions */
        Dcm_InhibitJumpToBootloader = TRUE;
      }
      else
      {
        /* PostBoot Operations have been finished. */

        Dcm_PostBootOperationsState = DCM_POSTBOOT_OPERATIONS_FINISHED;

        break;
      }
      /* no break */

    case DCM_POSTBOOT_SET_PROGRAMMING_CONDITIONS:
    {
      Std_ReturnType SetProgConditionsReturn = E_OK;

      /* Call Dcm_SetProgConditions() to clear the flags */
      SetProgConditionsReturn = Dcm_SetProgConditions(&Dcm_ProgConditions);

      if (SetProgConditionsReturn == DCM_E_OK)
      {
        /* Jump to the bootloader allowed again. */
        Dcm_InhibitJumpToBootloader = FALSE;

        /* Dcm_SetProgConditions() is successfull, Enable normal processing;
           Fall through to the next state so that Normal processing
           is started in the same cycle */

        Dcm_PostBootOperationsState = DCM_POSTBOOT_OPERATIONS_FINISHED;
      }
      else if (SetProgConditionsReturn == DCM_E_PENDING)
      {
        /* !LINKSTO Dcm.EB.ServiceProcessing.SetProgConditions.E_PENDING,1 */
        /* do nothing, just perform the call in the next cycle again */
      }
      else
      {
        /* Jump to the bootloader allowed again. */
        Dcm_InhibitJumpToBootloader = FALSE;

        /* Error condition, the setting of the flags has most probably failed,
           the only thing which can be done at this point is to continue with
           normal functions and report a relevant error. */

        Dcm_PostBootOperationsState = DCM_POSTBOOT_OPERATIONS_FINISHED;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        if (SetProgConditionsReturn == DCM_E_NOT_OK)
        {
          /* !LINKSTO Dcm.EB.ServiceProcessing.DetEnabled.SetProgConditions.E_NOT_OK,1 */
          DCM_DET_REPORT_ERROR(DCM_SVCID_SETPROGCONDITIONS, DCM_E_SET_PROG_CONDITIONS_FAIL);
        }
        else
        {
          /* !LINKSTO Dcm.EB.ServiceProcessing.DetEnabled.SetProgConditions.UnknownReturnValues,1 */
          DCM_DET_REPORT_ERROR(DCM_SVCID_SETPROGCONDITIONS, DCM_E_INTERFACE_RETURN_VALUE);
        }
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
      }

      break;
    }

    case DCM_POSTBOOT_OPERATIONS_FINISHED:
      /* Nothing more to do. */
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SERVID_MAINFUNCTION);
      break;
    /* CHECK: PARSE */
  }

}
#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[end of file]================================================================*/
