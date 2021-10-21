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
/* !LINKSTO Dcm.Dsn.File.ApplicationDiagnosticRequests.Impl,1 */
/*==================[inclusions]==================================================================*/


#include <Std_Types.h>                                    /* AUTOSAR standard types */
#include <TSAutosar.h>
#include <Dcm_Dsp_ApplicationDiagnosticRequests.h>
#include <Dcm_Dsl_CommunicationServices.h>
#include <Dcm_Dsl_Supervisor.h>
#include <TSMem.h>                                        /* EB specific memory functions */
#include <SchM_Dcm.h>                                     /* Manage critical DCM sections */
#include <Dcm_Dsl_Cfg.h>
#include <Dcm_Int.h>

/* Enable code only if feature is present */
#if (DCM_DSL_APPLICATION_DIAGNOSTIC_REQUESTS == STD_ON)
/*==================[macro definitions]===========================================================*/

/*==================[type declarations]===========================================================*/

/*==================[internal function declarations]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief This function dispatches the request packed into the Dcm_RequestToInjectData structure
 *         for further processing by the DCM. *
 *
 *  \return Dcm_StatusType
 *  \retval DCM_E_NOT_OK        The dispatch of the request could not be executedon the current call
 *  \retval DCM_E_OK            The dispatch of the requet has been successful on the current call
 **/

STATIC FUNC(Dcm_StatusType, DCM_CODE) Dcm_Dsl_ApplicationDiagnosticRequests_RequestDispatch(void);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*==================[internal constant definitions]===============================================*/

/*==================[internal data definitions]===================================================*/

#define DCM_START_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>
/** \brief This variable indicates if the Dcm_StartOfReception() function need to
 **        be called in the next MainFunction() because RxConnection is blocked.
 **  - TRUE : The RxConnection is locked and Dcm_StartOfReception() will be called
 **           in the next MainFunction()
 **  - FALSE: The RxConnection is unlocked and the request will be processed.
 **/
STATIC VAR(boolean, DCM_VAR) Dcm_ReattemptRequest = FALSE;

/** \brief This variable holds the injected request from CDD
 **/
 /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.InternalBuffer,1 */
STATIC VAR(uint8, DCM_VAR) Dcm_InjectedRequestBuffer[DCM_MAX_INJECTION_BUFFER_SIZE];

#define DCM_STOP_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>
/** \brief This variable holds the relevant data information for the injection:
 *
 * RxBuffer : Buffer element containing the pointer and the size of the reqeust to be injected.
 * RequestType : The type of request to be issued (Physical or Functional)
 * InjectionRxPduId : The RxPduId used to execute the injection.
 *
 **/
STATIC VAR(Dcm_InjectedRxDataType, DCM_VAR) Dcm_RequestToInjectData;

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>
/*==================[external constant definitions]===============================================*/

/*==================[external data definitions]===================================================*/

/*==================[external function definitions]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>
/* !LINKSTO Dcm.Dsn.IB.ApplicationDiagnosticRequests.Init,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_ApplicationDiagnosticRequests_Init(void)
{
  uint32_least BufferIndex;

  /* Reset request variables and flags */
  Dcm_ReattemptRequest = FALSE;
  Dcm_RequestToInjectData.InjectionRxPduId = 0U;
  Dcm_RequestToInjectData.RxBuffer.FilledLength = 0U;
  Dcm_RequestToInjectData.RxBuffer.Size = 0U;
  Dcm_RequestToInjectData.OriginalRequestSize = 0U;

  for(BufferIndex= 0U; BufferIndex< DCM_MAX_INJECTION_BUFFER_SIZE; BufferIndex++)
  {
    /* Clear the request buffer */
    /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.EnqueuedRequest.FreeBuffer,1 */
    Dcm_InjectedRequestBuffer[BufferIndex] = 0U;
  }

}

/* !LINKSTO Dcm.Dsn.IB.ApplicationDiagnosticRequests.Injector,1 */
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_Dsl_ApplicationDiagnosticRequests_Injector
(
  PduIdType RxPduID,
  P2CONST(uint8, AUTOMATIC, DCM_VAR) RequestData,
  PduLengthType RequestLength
)
{
  BufReq_ReturnType BufferRequestResult = BUFREQ_E_NOT_OK;
  Dcm_RxPduIdInfoType CurrentRxPduIdInfo;

  /* Check if Requested RxPduId is correct */
  if(RxPduID < DCM_NUM_RX_PDU_ID)
  {
    /* Get RxPduId information */
    Dcm_Dsl_RxConnectionGetRxPduIdInformation(RxPduID, &CurrentRxPduIdInfo);

    /* Check if RxConnection and the internal buffer can allocate the request */
    if((RequestLength <= CurrentRxPduIdInfo.RxBufferSize)&&
       (RequestLength <= DCM_MAX_INJECTION_BUFFER_SIZE))
    {
      /* Enter critical section */
      SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
      /* If an injected requested has already been flagged, do not attempt to enqueue another one */
      if(Dcm_ReattemptRequest == FALSE)
      {
        /* Make sure the DCM is initialized and reception of request by the DCM is not blocked */
        /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.DcmInitialized,1 */
        /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.ProcessingBlocked,1 */
        if (Dcm_Dsl_Supervisor_RequestProcessingInhibited() == FALSE)
        {
          Dcm_StatusType DispatchResult = DCM_E_NOT_OK;
          /* Store the diagnostic request to units buffer Dcm_InjectedRequestBuffer*/
          /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.EnqueueRequest,1 */
          TS_MemCpy(&Dcm_InjectedRequestBuffer[0], &RequestData[0], RequestLength);

          /* Store the diagnostic request's RxPduId */
          Dcm_RequestToInjectData.InjectionRxPduId = RxPduID;

          /* Store the length of the request */
          Dcm_RequestToInjectData.OriginalRequestSize = RequestLength;

          /* Store the type of request */
          if (CurrentRxPduIdInfo.AddressingType == DCM_PHYSICAL_ADDRESSING)
          {
            Dcm_RequestToInjectData.RequestType = DCM_NORMAL_PHYSICAL_REQUEST;
          }
          else
          {
            Dcm_RequestToInjectData.RequestType = DCM_NORMAL_FUNCTIONAL_REQUEST;
          }

          /* Announce the Supervisor that a reception has been started so
           * it may take the necessary actions. */
          Dcm_Dsl_Supervisor_AnnounceIncomingApplicationDiagnosticRequest(Dcm_RequestToInjectData.InjectionRxPduId);

          /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.EnqueuedRequest.Dispatch,1 */
          DispatchResult = Dcm_Dsl_ApplicationDiagnosticRequests_RequestDispatch();

          if (DispatchResult == DCM_E_OK)
          {
            /* Reset everything if the request has been accepted or if it has failed due to the RX
             * buffer not being available */
            Dcm_Dsl_ApplicationDiagnosticRequests_Init();
          }
          else
          {
            /* The request cannot be processed on this run. Keep the data to try on next Main execution */
            /* The request will be enqueued only if conditions allow for this */
            Dcm_ReattemptRequest = TRUE;
          }

          /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.OK,1 */
          BufferRequestResult = BUFREQ_OK;
        }
      }
      else
      {
        /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.BufferBusy,1 */
        BufferRequestResult = BUFREQ_E_BUSY;
      }
      /* Leave the critical section */
      SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
    }
    else
    {
      /* RxConnection Buffer too small or Injector buffer too small  */
      /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.RxConnectionBufferTooSmall,1 */
      /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.InjectorBufferTooSmall,1 */
      /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.BufferOverflow.RequestRejection,1 */
      BufferRequestResult = BUFREQ_E_OVFL;
    }
  }
  else
  {
    /* RxPduId out of range */
    /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.IncorrectRxPduID,1 */
    BufferRequestResult = BUFREQ_E_NOT_OK;
  }

  return BufferRequestResult;
}

/* !LINKSTO Dcm.Dsn.IB.ApplicationDiagnosticRequests.Main,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_ApplicationDiagnosticRequests_MainFunction(void)
{
  Dcm_StatusType DispatchResult;

  /* This is the main function of the injector.It will be called from the Dcm_Main function on every new dcm
     execution cycle. It will check the injection flag, and if active, it will forward the request stored
     on the injector buffer to RxConnection for processing. */

  /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.EnqueuedRequest.Poll,1 */
  if(Dcm_ReattemptRequest == TRUE)
  {
    /* Enter critical section */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.EnqueuedRequest.Dispatch,1 */
    DispatchResult = Dcm_Dsl_ApplicationDiagnosticRequests_RequestDispatch();

    if (DispatchResult == DCM_E_OK)
    {
        /* Reset everything if the request has been accepted or if it has failed due to the RX
         * buffer not being available */
        Dcm_Dsl_ApplicationDiagnosticRequests_Init();
    }
    else
    {
      /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.EnqueuedRequest.HigherPriorityRunning.Repoll,2 */
      /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.EnqueuedRequest.EqualPriorityRunning.Repoll,2 */
      /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.EnqueuedRequest.SameProtocol.Repoll,1 */
      /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.EnqueuedRequest.SameRxPduId.Repoll,1 */
      /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.EnqueuedRequest.RxBufferBusy.Repoll,1 */
      /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.EnqueuedRequest.HigherPriorityNotExecuting.Repoll,2 */
      /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.EnqueuedRequest.EqualPriorityNotExecuting.Repoll,2 */
      /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.EnqueuedRequest.RxDisabled.Repoll,1 */
      /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.EnqueuedRequest.SameType.Repoll, 1 */
      /* Do nothing. Re-attempt on next execution cycle. */
    }
    /* Leave the critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
  }
}

/*==================[internal function definitions]==============================================*/

STATIC FUNC(Dcm_StatusType, DCM_CODE) Dcm_Dsl_ApplicationDiagnosticRequests_RequestDispatch(void)
{
  Dcm_StatusType              DispatchResult = DCM_E_NOT_OK;
  Dcm_RequestTypeType         RxPermission = DCM_RX_REJECT;
  BufReq_ReturnType           BufferAllocationResult = BUFREQ_E_NOT_OK;

  if(Dcm_Dsl_RxConnectionGetState(Dcm_RequestToInjectData.InjectionRxPduId) == DCM_RX_CONNECTION_IDLE)
  {
    /* Verify Rx permission for the used RxPduId */
    RxPermission = Dcm_Dsl_Supervisor_RequestRxPermission(Dcm_RequestToInjectData.InjectionRxPduId,
                                                          Dcm_RequestToInjectData.OriginalRequestSize,
                                                          DCM_EXTERNAL);
    /* Allocate transmission buffer */
    if((RxPermission == DCM_NORMAL_FUNCTIONAL_REQUEST)||
        (RxPermission == DCM_NORMAL_PHYSICAL_REQUEST)
      )
    {
      Dcm_BufferUsageType InjectionRequester = DCM_BUFFER_RX_NORMAL;

      BufferAllocationResult = Dcm_Dsl_BufferAllocate(Dcm_RequestToInjectData.InjectionRxPduId,
                                                      InjectionRequester,
                                                      Dcm_RequestToInjectData.OriginalRequestSize,
                                                      &Dcm_RequestToInjectData.RxBuffer);
      if (BufferAllocationResult == BUFREQ_OK)
      {
        /* Populate request buffer after buffer allocation. */
        TS_MemCpy(Dcm_RequestToInjectData.RxBuffer.BufferPtr,
                  &Dcm_InjectedRequestBuffer[0],
                  Dcm_RequestToInjectData.OriginalRequestSize);

        /* Set the Filled Length to request Length before handling to Supervisor */
        Dcm_RequestToInjectData.RxBuffer.FilledLength = Dcm_RequestToInjectData.OriginalRequestSize;

        /* Announce the Supervisor that a reception has been started so
         * it may take the necessary actions.*/
        Dcm_Dsl_Supervisor_IncomingReception(Dcm_RequestToInjectData.InjectionRxPduId,
            Dcm_RequestToInjectData.RequestType);

        /* Signal the reception of this injected request on the RxConnection of
           the respective RxPduId. */
        Dcm_Dsl_RxConnectionRxIndication(Dcm_RequestToInjectData.InjectionRxPduId,
            Dcm_RequestToInjectData.RequestType,
            Dcm_RequestToInjectData.RxBuffer);

        DispatchResult = DCM_E_OK;
      }
    }
  }
  return DispatchResult;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
#endif /* #if (DCM_DSL_APPLICATION_DIAGNOSTIC_REQUESTS == STD_ON) */

/*==================[end of file]================================================================*/
