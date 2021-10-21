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

/* !LINKSTO Dcm.Dsn.File.RoeServices.Impl,1 */
/* This file contains the implementation of the RoeServices software unit. */

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
/*===============================[includes]======================================================*/

#include <Dcm_Trace.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <Dcm_Dsl_Supervisor.h>
#include <Dcm_Dsl_RoeServices.h>

/* The functions provided and implemented by this unit are enabled only if internal service
 * handlers which need ROE services are configured. */
#if (DCM_DSP_USE_ROESERVICES == STD_ON)

#include <TSMem.h>                                               /* EB specific memory functions */
#include <SchM_Dcm.h>                                                  /* SchM interface for Dcm */
#if (DCM_INCLUDE_RTE == STD_ON)
#include <Rte_Dcm.h>                                              /* Dcm_GetSesCtrlType prototype*/
#endif

/*===========================[macro definitions]=================================================*/

#if (defined DCM_STATE_STOPPED_AND_TIME_NOT_EXPIRED)
  #error "DCM_STATE_STOPPED_AND_TIME_NOT_EXPIRED is already defined"
#endif
#define DCM_STATE_STOPPED_AND_TIME_NOT_EXPIRED         ((uint8)0U)

#if (defined DCM_STATE_STOPPED_OR_STARTED_AND_TIME_EXPIRED)
  #error "DCM_STATE_STOPPED_OR_STARTED_AND_TIME_EXPIRED is already defined"
#endif
#define DCM_STATE_STOPPED_OR_STARTED_AND_TIME_EXPIRED  ((uint8)1U)

#if (defined DCM_STATE_STARTED_AND_TIME_NOT_EXPIRED)
  #error "DCM_STATE_STARTED_AND_TIME_NOT_EXPIRED is already defined"
#endif
#define DCM_STATE_STARTED_AND_TIME_NOT_EXPIRED         ((uint8)2U)

#if (defined DCM_STATE_CLEAR_TIME_IRRELEVANT)
  #error "DCM_STATE_CLEAR_TIME_IRRELEVANT is already defined"
#endif
#define DCM_STATE_CLEAR_TIME_IRRELEVANT                ((uint8)3U)

#if (defined DCM_STATE_ERROR_TIME_ERROR)
  #error "DCM_STATE_ERROR_TIME_ERROR is already defined"
#endif
#define DCM_STATE_ERROR_TIME_ERROR                     ((uint8)4U)

/*==================[type definitions]===========================================================*/

typedef uint8 Dcm_RoeEnqueueEventType;

#if (defined DCM_ROE_NO_EVENT)
  #error "DCM_ROE_NO_EVENT is already defined"
#endif
#define DCM_ROE_NO_EVENT 0U

#if (defined DCM_ROE_NORMAL_EVENT)
  #error "DCM_ROE_NORMAL_EVENT is already defined"
#endif
#define DCM_ROE_NORMAL_EVENT 1U

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Changes the state of a ROE event
 **
 ** \param[in] Event          The ID of the event to be changed
 ** \param[in] State          The new state of the event
 **
 ** \return none
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsl_RoeServices_ChangeState(
  Dcm_RoeEventTypeType Event, Dcm_RoeEventStateType State);

/** \brief Initializes the ROE services based on the preconfigured data
 **
 ** \param  none
 ** \return none
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsl_RoeServices_InitPreconf(void);

/** \brief Initializes the ROE services
 **
 ** If the NvM returns NVM_REQ_PENDING the ROE services will not be initialized and the function
 ** needs to be called until the NvM returns something different than NVM_REQ_PENDING.
 ** The function is called by Dcm_Dsl_RoeServices_MainFunction.
 **
 ** \param  none
 ** \return none
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsl_RoeServices_InitInternal(void);

/** \brief Send Response of one Event
 ** This function provides the processing of one event, requested by the Central Diagnostic
 ** Software Component.
 **
 ** \return Permission of DCM
 **   - ::DCM_E_OK: ResponseOnOneEvent request is accepted by DCM and the request data is copied
 **                 into internal reception buffer,
 **   - ::DCM_E_ROE_NOT_ACCEPTED: ResponseOnOneEvent request is not accepted by DCM
 **                               (e.g. current pending request is not finished)
 **
 ** \param[in] MsgPtr Content of requested service
 ** \param[in] MsgLen Length in bytes
 ** \param[in] DcmRxPduId PDU ID through which ROE service request is received
 */
STATIC FUNC(Dcm_StatusType, DCM_CODE) Dcm_Dsl_RoeServices_ResponseOnOneEvent(
  P2VAR(Dcm_MsgItemType, AUTOMATIC, DCM_CONST) MsgPtr,
  Dcm_MsgLenType                 MsgLen,
  PduIdType                      DcmRxPduId);

/** \brief Tries to transmit a ROE message
 **
 ** The function must be placed in a critical section. It is not interruptible.
 **
 ** \param[in]  RoeRxData                Data structure containing details for message reception
 ** \return     Dcm_StatusType
 ** \retval     DCM_E_OK                 The message was accepted or postponed
 ** \retval     DCM_E_ROE_NOT_ACCEPTED   The message cannot be transmitted
 */
STATIC FUNC(Dcm_StatusType, DCM_CODE) Dcm_Dsl_RoeServices_RoeEventDispatch(
  P2VAR(Dcm_RoeRxDataType, AUTOMATIC, DCM_VAR) RoeRxData);

/** \brief Trigger common code
 **
 ** The function simplifies the trigger code for DTC and DID events.
 **
 ** \param[in]  EventType                Event type to be triggered.
 **
 ** \return none
 **
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dem_RoeServices_CommonTriggerProcedure(Dcm_RoeEventTypeType EventType);

/** \brief Retrieves and stores RoeRxDataInformation
 **
 ** \param[in]   RoeRxData         Data structure containing details for message reception.
 ** \param[out]  RoeRxDataCopy     Data structure containing details for storing the received message.
 **
 ** \return none
 **
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsl_RoeServices_GetRoeRxDataInformation(
  P2CONST(Dcm_RoeRxDataType, AUTOMATIC, DCM_VAR) RoeRxData,
  P2VAR(Dcm_RoeRxDataType, AUTOMATIC, DCM_VAR) RoeRxDataCopy);

#if (DCM_ROE_PERSISTENCE == STD_ON)
/** \brief Maps a constant describing a case of processing for certain combinations of the State
 **        and EventWindowTime value of an event.
 **
 ** \param[in]  State                    The state of the event (STARTED | STOPPED | CLEARED)
 ** \param[in]  EventWindowTime          The event window time value of the event
 **
 ** \return uint8
 **
 */
STATIC FUNC(uint8, DCM_CODE) Dcm_Dsl_RoeServices_MapToRelevantCase(
    Dcm_RoeEventStateType  State, RoeEventWindowTimeType EventWindowTime);
#endif /* (DCM_ROE_PERSISTENCE == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*=======================[external data declarations]============================================*/
#if (DCM_ROE_PERSISTENCE == STD_ON)
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief persistent data for ROE
 ** This structure holds the ROE data that needs to be restored after power cycle.
 ** The structure is stored by NvM WriteAll at shutdown, and restored by NvM ReadAll at startup.
 */
VAR(Dcm_RoeEventDataType, DCM_VAR) Dcm_Dsl_RoeServices_Persistent_Data[DCM_ROE_EVENT_MAX];

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>
#endif /* (DCM_ROE_PERSISTENCE == STD_ON) */

/*=======================[internal data declarations]============================================*/
#define DCM_START_SEC_VAR_INIT_16
#include <Dcm_MemMap.h>
STATIC VAR(uint16, DCM_VAR) Dcm_RoeInterMsgCycle = 0U;
#define DCM_STOP_SEC_VAR_INIT_16
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/** \brief Reflects the initialization state of the Roe Services.
 ** If the service is not initialized, the ROE functionality is disabled. Triggers are not
 ** accepted.
 */
STATIC VAR(boolean, DCM_VAR) Dcm_Dsl_RoeServices_Initialized = FALSE;

/** \brief Flag to enqueue ROE event */
STATIC VAR(Dcm_RoeEnqueueEventType, DCM_VAR) Dcm_RoeEnqueueEvent = DCM_ROE_NO_EVENT;

#define DCM_STOP_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief Contains runtime information for ROE  */
STATIC VAR(Dcm_RoeEventDataType, DCM_VAR) Dcm_Dsl_RoeServices_Runtime_Data[DCM_ROE_EVENT_MAX];

/** \brief Contains information necessary for ROE reception */
STATIC VAR(Dcm_RoeRxDataType, DCM_VAR) Dcm_Dsl_RoeRxData;

/** \brief Value that will copy the information from "Dcm_Dsl_RoeRxData"
 *         It will be used only if the Supervisor will accept the request and it will be used
 *         further instead of "Dcm_Dsl_RoeRxData"
 */
STATIC VAR(Dcm_RoeRxDataType, DCM_VAR) Dcm_Dsl_RoeRxDataCopy;

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>
/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.IB.RoeServices.ClearEvents,1 */
/* !LINKSTO Dcm.Dsn.IB.RoeServices.ClearROE,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_RoeServices_ClearEvents(boolean ServiceCall)
{
  Dcm_RoeEventTypeType EventType;

  /* !LINKSTO SWS_Dcm_00901.ClearROE,1 */
  /* !LINKSTO SWS_Dcm_00906.ClearROE,1*/
  TS_MemBZero(Dcm_Dsl_RoeServices_Runtime_Data, sizeof(Dcm_Dsl_RoeServices_Runtime_Data));

  for (EventType=DCM_ROE_EVENT_DTC; EventType<DCM_ROE_EVENT_MAX; ++EventType)
  {
    /* !LINKSTO SWS_Dcm_00875,1 */
    /* !LINKSTO SWS_Dcm_00884,1 */
    if (TRUE == ServiceCall)
    {
      /* !LINKSTO SWS_Dcm_00900.Clear,1 */
      /* !LINKSTO SWS_Dcm_00902.Clear,1 */
      Dcm_Dsl_RoeServices_ChangeState(EventType, DCM_ROE_CLEARED);
    }
    else
    {
      Dcm_Dsl_RoeServices_Runtime_Data[EventType].State = DCM_ROE_CLEARED;
    }
#if (DCM_ROE_PERSISTENCE == STD_ON)
    /* !LINKSTO SWS_Dcm_00897,1 */
    Dcm_Dsl_RoeServices_Persistent_Data[EventType].State = DCM_ROE_CLEARED;
    (void)NvM_SetRamBlockStatus(DCM_ROE_NVM_BLOCK_ID, TRUE);
#endif
  }

  /* Reset ROE processing status for the internal call */
  if (FALSE == ServiceCall)
  {
    DBG_DCM_ROEENQUEUEEVENT((Dcm_RoeEnqueueEvent), (DCM_ROE_NO_EVENT))
    Dcm_RoeEnqueueEvent = DCM_ROE_NO_EVENT;
  }
}

/* !LINKSTO Dcm.Dsn.IB.RoeServices.MainFunction,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_RoeServices_MainFunction(void)
{
  DBG_DCM_DSL_ROESERVICES_MAINFUNCTION_ENTRY();

  if (Dcm_Dsl_RoeServices_Initialized != TRUE)
  {
    Dcm_Dsl_RoeServices_InitInternal();
  }

  if (Dcm_RoeInterMsgCycle > 0)
  {
    --Dcm_RoeInterMsgCycle;
  }

  if (DCM_ROE_NO_EVENT != Dcm_RoeEnqueueEvent)
  {
    /* ENTER critical section */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    /* Not interested in return value. ROE does not support queuing and retry. Postponing of a
       message is handled, in error cases the event will be discarded */
    (void)Dcm_Dsl_RoeServices_RoeEventDispatch(&Dcm_Dsl_RoeRxData);

    /* LEAVE critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
  }

  DBG_DCM_DSL_ROESERVICES_MAINFUNCTION_EXIT();
}

/* !LINKSTO Dcm.Dsn.IB.RoeServices.Init,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_RoeServices_Init(void)
{
  DBG_DCM_DSL_ROESERVICES_INIT_ENTRY();

  Dcm_Dsl_RoeServices_Initialized = FALSE;
  Dcm_RoeEnqueueEvent = DCM_ROE_NO_EVENT;
  Dcm_RoeInterMsgCycle = 0;

  DBG_DCM_DSL_ROESERVICES_INIT_EXIT();
}

/* !LINKSTO Dcm.Dsn.IB.RoeServices.IsInitialized,1 */
FUNC(boolean, DCM_CODE) Dcm_Dsl_RoeServices_IsInitialized(void)
{
  boolean Initialized = Dcm_Dsl_RoeServices_Initialized;

  DBG_DCM_DSL_ROESERVICES_ISINITIALIZED_ENTRY();

  DBG_DCM_DSL_ROESERVICES_ISINITIALIZED_EXIT(Initialized);

  return Initialized;
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsl_RoeServices_InitPreconf(void)
{
  uint8 PreconfEventIndex;
#if (DCM_ROE_PERSISTENCE == STD_ON)
  boolean NvMChanged = FALSE;
#endif

  /* clear all the events for runtime and persistent data */
  Dcm_Dsl_RoeServices_ClearEvents(FALSE);

  /* initialize runtime data based on preconf data */
  /* Deviation TASKING-1 <+2> */
  /* !LINKSTO SWS_Dcm_00951.NvMError,1, SWS_Dcm_00954,1, SWS_Dcm_00888.Ref,1 */
  for (PreconfEventIndex = 0; PreconfEventIndex < DCM_ROE_NUM_PRECONF_EVENT; ++PreconfEventIndex)
  {
    /* At most one event can be in STOPPED state for each event type
      (one for OnChangeOfDataIdentifier and one for onDTCStatusChange). */
    if (Dcm_RoeEventPreconfList[PreconfEventIndex].InitialEventStatus == DCM_ROE_STOPPED)
    {
      /* calculate the index for the runtime/persistent data array */
      const Dcm_RoeEventTypeType EventType = Dcm_RoeEventPreconfList[PreconfEventIndex].EventType;

      Dcm_Dsl_RoeServices_Runtime_Data[EventType].PreconfIdx = PreconfEventIndex;
      Dcm_Dsl_RoeServices_Runtime_Data[EventType].OrigEventWindowTime = DCM_ROE_EVENT_WINDOW_INFINITE;
      Dcm_Dsl_RoeServices_Runtime_Data[EventType].EventWindowTime = DCM_ROE_EVENT_WINDOW_INFINITE;
      Dcm_Dsl_RoeServices_Runtime_Data[EventType].Session = DCM_INVALID_SESSION;
      /* !LINKSTO SWS_Dcm_00951.NoPersistency,1 */
      Dcm_Dsl_RoeServices_ChangeState(EventType, DCM_ROE_STOPPED);

#if (DCM_ROE_PERSISTENCE == STD_ON)
      /* copy runtime data to persistent data */
      TS_MemCpy(&Dcm_Dsl_RoeServices_Persistent_Data[EventType],
        &Dcm_Dsl_RoeServices_Runtime_Data[EventType],
        sizeof(Dcm_RoeEventDataType));
      NvMChanged = TRUE;
#endif /* (DCM_ROE_PERSISTENCE == STD_ON) */
    }
  }

#if (DCM_ROE_PERSISTENCE == STD_ON)
  if (NvMChanged == TRUE)
  {
    (void)NvM_SetRamBlockStatus(DCM_ROE_NVM_BLOCK_ID, TRUE);
  }
#endif /* (DCM_ROE_PERSISTENCE == STD_ON) */
}

/* !LINKSTO Dcm.Dsn.IB.RoeServices.InitInternal,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsl_RoeServices_InitInternal(void)
{
#if (DCM_ROE_PERSISTENCE == STD_ON)
  NvM_RequestResultType RestoredNvmData = NVM_REQ_NOT_OK;
  Std_ReturnType NvMRetVal;
  /* get status from NvM of Dcm ROE persistent block */
  NvMRetVal = NvM_GetErrorStatus(DCM_ROE_NVM_BLOCK_ID, &RestoredNvmData);

  if (E_OK == NvMRetVal)
  {
    /* !LINKSTO Dcm.EB.ROE.NvM_GetErrorStatus.RedundancyFailed,1 */
    if ((NVM_REQ_OK == RestoredNvmData) || (NVM_REQ_REDUNDANCY_FAILED == RestoredNvmData))
    {
      /* Nvm just got initialized, update runtime data */
      Dcm_RoeEventTypeType EventType;
      boolean NvMChanged = FALSE;
      for (EventType=DCM_ROE_EVENT_DTC; EventType<DCM_ROE_EVENT_MAX; ++EventType)
      {
        /* Update EventWindowTime from previous cycle if the event is set up (STOPPED or STARTED) */
        /* LINKSTO SWS_Dcm_00901,1 - phase2 */
        /* !LINKSTO SWS_Dcm_00900.NextCycle,1 */
        if ((Dcm_Dsl_RoeServices_Persistent_Data[EventType].State == DCM_ROE_STOPPED) ||
            (Dcm_Dsl_RoeServices_Persistent_Data[EventType].State == DCM_ROE_STARTED))
        {
          switch (Dcm_Dsl_RoeServices_Persistent_Data[EventType].EventWindowTime)
          {
            case DCM_ROE_EVENT_WINDOW_CURRENT_CYCLE:
              Dcm_Dsl_RoeServices_Persistent_Data[EventType].EventWindowTime =
                DCM_ROE_EVENT_WINDOW_EXPIRED;
              NvMChanged = TRUE;
              break;
            case DCM_ROE_EVENT_WINDOW_CURRENT_AND_FOLLOWING_CYCLE:
              Dcm_Dsl_RoeServices_Persistent_Data[EventType].EventWindowTime =
                DCM_ROE_EVENT_WINDOW_CURRENT_CYCLE;
              NvMChanged = TRUE;
              break;
            default:
              /* no update required */
              break;
          }
        }
      }

      /* Mark NvM block for update, only if something has changed */
      if (NvMChanged == TRUE)
      {
        (void)NvM_SetRamBlockStatus(DCM_ROE_NVM_BLOCK_ID, TRUE);
      }

      for (EventType=DCM_ROE_EVENT_DTC; EventType<DCM_ROE_EVENT_MAX; ++EventType)
      {
        uint8 State_Time_Scenario = Dcm_Dsl_RoeServices_MapToRelevantCase(
            Dcm_Dsl_RoeServices_Persistent_Data[EventType].State,
            Dcm_Dsl_RoeServices_Persistent_Data[EventType].EventWindowTime);
        switch(State_Time_Scenario)
        {
          case DCM_STATE_STOPPED_AND_TIME_NOT_EXPIRED:
          {
            /* copy persistent data to runtime data */
            TS_MemCpy(&Dcm_Dsl_RoeServices_Runtime_Data[EventType],
              &Dcm_Dsl_RoeServices_Persistent_Data[EventType],
              sizeof(Dcm_RoeEventDataType));

            Dcm_Dsl_RoeServices_ChangeState(EventType, DCM_ROE_STOPPED);
            break;
          }
          case DCM_STATE_STOPPED_OR_STARTED_AND_TIME_EXPIRED:
          {
            /* copy persistent data to runtime data */
            TS_MemCpy(&Dcm_Dsl_RoeServices_Runtime_Data[EventType],
              &Dcm_Dsl_RoeServices_Persistent_Data[EventType],
              sizeof(Dcm_RoeEventDataType));

            Dcm_Dsl_RoeServices_ChangeState(EventType, DCM_ROE_STOPPED);
            break;
          }
          case DCM_STATE_STARTED_AND_TIME_NOT_EXPIRED:
          {
            /* copy persistent data to runtime data */
            TS_MemCpy(&Dcm_Dsl_RoeServices_Runtime_Data[EventType],
              &Dcm_Dsl_RoeServices_Persistent_Data[EventType],
              sizeof(Dcm_RoeEventDataType));

            Dcm_Dsl_RoeServices_ChangeState(EventType, DCM_ROE_STARTED);
            break;
          }
          case DCM_STATE_CLEAR_TIME_IRRELEVANT:
          {
            Dcm_Dsl_RoeServices_Persistent_Data[EventType].EventWindowTime =
              DCM_ROE_EVENT_WINDOW_EXPIRED;
            /* copy persistent data to runtime data */
            TS_MemCpy(&Dcm_Dsl_RoeServices_Runtime_Data[EventType],
              &Dcm_Dsl_RoeServices_Persistent_Data[EventType],
              sizeof(Dcm_RoeEventDataType));

            Dcm_Dsl_RoeServices_ChangeState(EventType, DCM_ROE_CLEARED);
            break;
          }
          /* CHECK: NOPARSE */
          default:
            /* Unreachable default case mandated by MISRA */
            DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
            break;
            /* CHECK: PARSE */
        }
      }

      Dcm_Dsl_RoeServices_Initialized = TRUE;
    }
    else if (NVM_REQ_PENDING == RestoredNvmData)
    {
      /* Nvm returned PENDING, wait for next cycle */
    }
    else
    {
      /* Some problem occurred, use the preconfigured values */
      /* !LINKSTO Dcm.EB.ROE.NvM_GetErrorStatus.Failed.DetReport, 2 */

#if (DCM_DEV_ERROR_DETECT == STD_ON)
      DCM_DET_REPORT_ERROR(DCM_SVCID_NVMREADDATA, DCM_E_NVM_READ_FAIL);
#endif

      Dcm_Dsl_RoeServices_InitPreconf();
      Dcm_Dsl_RoeServices_Initialized = TRUE;
    }
  }
  else
  {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    if (E_NOT_OK != NvMRetVal)
    {
      /* !LINKSTO Dcm.DET.InterfaceReturnValue, 2 */
      DCM_DET_REPORT_ERROR(DCM_SVCID_NVMGETERRORSTATUS, DCM_E_INTERFACE_RETURN_VALUE);
    }

    /* !LINKSTO Dcm.EB.ROE.NvM_GetErrorStatus.DetReport, 2 */
    DCM_DET_REPORT_ERROR(DCM_SVCID_NVMREADDATA, DCM_E_NVM_READ_FAIL);
#endif
    Dcm_Dsl_RoeServices_InitPreconf();
    Dcm_Dsl_RoeServices_Initialized = TRUE;
  }

#else /* (DCM_ROE_PERSISTENCE == STD_ON) */
  Dcm_Dsl_RoeServices_InitPreconf();
  Dcm_Dsl_RoeServices_Initialized = TRUE;
#endif /* (DCM_ROE_PERSISTENCE == STD_ON) */
}

#if (DCM_ROE_PERSISTENCE == STD_ON)
STATIC FUNC(uint8, DCM_CODE) Dcm_Dsl_RoeServices_MapToRelevantCase(
    Dcm_RoeEventStateType  State, RoeEventWindowTimeType EventWindowTime)
{

  uint8 ReturnValue = DCM_STATE_ERROR_TIME_ERROR;

  /* !LINKSTO SWS_Dcm_00874,1 */
  if ((State == DCM_ROE_STOPPED) &&
      (EventWindowTime != DCM_ROE_EVENT_WINDOW_EXPIRED))
  {
    ReturnValue = DCM_STATE_STOPPED_AND_TIME_NOT_EXPIRED;
  }

  /* !LINKSTO SWS_Dcm_00878,1 */
  if (((State == DCM_ROE_STOPPED) ||
       (State == DCM_ROE_STARTED)) &&
      (EventWindowTime == DCM_ROE_EVENT_WINDOW_EXPIRED))
  {
    ReturnValue = DCM_STATE_STOPPED_OR_STARTED_AND_TIME_EXPIRED;
  }

  /* !LINKSTO SWS_Dcm_00889,1, SWS_Dcm_00898,1, SWS_Dcm_00965,1, SWS_Dcm_00890,1  */
  if ((State == DCM_ROE_STARTED) &&
      (EventWindowTime != DCM_ROE_EVENT_WINDOW_EXPIRED))
  {
    ReturnValue = DCM_STATE_STARTED_AND_TIME_NOT_EXPIRED;
  }

  if (State == DCM_ROE_CLEARED)
  {
    ReturnValue = DCM_STATE_CLEAR_TIME_IRRELEVANT;
  }

  return ReturnValue;
}
#endif /* (DCM_ROE_PERSISTENCE == STD_ON) */

/* !LINKSTO Dcm.Dsn.IB.RoeServices.GetRxPduId,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_RoeServices_GetRxPduId(
  P2VAR(PduIdType, AUTOMATIC, DCM_VAR) RxPduId)
{
  Std_ReturnType RetVal = DCM_E_NOT_OK;
  Dcm_RoeEventTypeType EventType;

  DBG_DCM_DSL_ROESERVICES_GETRXPDUID_ENTRY(RxPduId);

  for (EventType=DCM_ROE_EVENT_DTC; EventType<DCM_ROE_EVENT_MAX; ++EventType)
  {
    if (Dcm_Dsl_RoeServices_Runtime_Data[EventType].State == DCM_ROE_STARTED)
    {
      *RxPduId = Dcm_Dsl_RoeServices_Runtime_Data[EventType].RxPduId;
      RetVal = DCM_E_OK;
      break;
    }
  }

  DBG_DCM_DSL_ROESERVICES_GETRXPDUID_EXIT(PduId, RxPduId);

  return RetVal;
}

/* !LINKSTO Dcm.Dsn.IB.RoeServices.SessionChange,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_RoeServices_SessionChange(
  Dcm_SesCtrlType OldSession, Dcm_SesCtrlType NewSession)
{
  Dcm_RoeEventTypeType EventType;

  DBG_DCM_DSL_ROESERVICES_SESSIONCHANGE_ENTRY(OldSession, NewSession);

  if ((OldSession != NewSession) && (TRUE == Dcm_Dsl_RoeServices_Initialized))
  {
    /* !LINKSTO SWS_Dcm_00879,1 */
    /* leaving the current session */
    for (EventType=DCM_ROE_EVENT_DTC; EventType<DCM_ROE_EVENT_MAX; ++EventType)
    {
      if (Dcm_Dsl_RoeServices_Runtime_Data[EventType].State == DCM_ROE_STARTED)
      {
        Dcm_Dsl_RoeServices_ChangeState(EventType, DCM_ROE_STOPPED);
        if (Dcm_Dsl_RoeServices_Runtime_Data[EventType].Session != DCM_DEFAULT_SESSION)
        {
          /* !LINKSTO SWS_Dcm_00906.SessionChange,1 */
          Dcm_Dsl_RoeServices_Runtime_Data[EventType].EventWindowTime =
            DCM_ROE_EVENT_WINDOW_EXPIRED;
          Dcm_Dsl_RoeServices_Runtime_Data[EventType].Session = DCM_INVALID_SESSION;
        }
      }
    }

    /* !LINKSTO SWS_Dcm_00902.Ref,1, SWS_Dcm_00902.Stop,1 */
    /* entering default session */
    if (NewSession == DCM_DEFAULT_SESSION)
    {
      for (EventType=DCM_ROE_EVENT_DTC; EventType<DCM_ROE_EVENT_MAX; ++EventType)
      {
        if ((Dcm_Dsl_RoeServices_Runtime_Data[EventType].State == DCM_ROE_STOPPED) &&
            (Dcm_Dsl_RoeServices_Runtime_Data[EventType].EventWindowTime !=
              DCM_ROE_EVENT_WINDOW_EXPIRED) &&
            (Dcm_Dsl_RoeServices_Runtime_Data[EventType].Session != DCM_INVALID_SESSION))
        {
          Dcm_Dsl_RoeServices_ChangeState(EventType, DCM_ROE_STARTED);
        }
      }
    }
  }

  DBG_DCM_DSL_ROESERVICES_SESSIONCHANGE_EXIT();
}

/* !LINKSTO Dcm.Dsn.IB.RoeServices.TriggerOnEvent,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_RoeServices_TriggerOnEvent(uint8 RoeEventId)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_DCM_DSL_ROESERVICES_TRIGGERONEVENT_ENTRY(RoeEventId);

  if ((TRUE == Dcm_Dsl_RoeServices_Initialized) &&
      (RoeEventId == Dcm_Dsl_RoeServices_Runtime_Data[DCM_ROE_EVENT_DID].PreconfIdx))
  {

    /* RoeEventId value is valid */
    RetVal = E_OK;
    /* !LINKSTO SWS_Dcm_00601,1 */
    /* !LINKSTO SWS_Dcm_00920,1 */
    if ((Dcm_Dsl_RoeServices_Runtime_Data[DCM_ROE_EVENT_DID].State == DCM_ROE_STARTED) &&
        (Dcm_RoeInterMsgCycle == 0U))
    {
      Dcm_Dem_RoeServices_CommonTriggerProcedure(DCM_ROE_EVENT_DID);
    }
  }

  DBG_DCM_DSL_ROESERVICES_TRIGGERONEVENT_EXIT(RetVal, RoeEventId);

  return RetVal;
}
/* !LINKSTO Dcm.Dsn.IB.RoeServices.DemTriggerOnDtcStatusChange,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_RoeServices_TriggerOnDTCStatus(uint8 DTCStatusNew)
{

  DBG_DCM_DSL_ROESERVICES_TRIGGERONDTCSTATUS_ENTRY(DTC, DTCStatusOld, DTCStatusNew);
  if (TRUE == Dcm_Dsl_RoeServices_Initialized)
  {
    uint8 PreconfIdx;
    /* !LINKSTO SWS_Dcm_00601,1 */
    /* !LINKSTO SWS_Dcm_00920,1 */
    if ((Dcm_Dsl_RoeServices_Runtime_Data[DCM_ROE_EVENT_DTC].State == DCM_ROE_STARTED) &&
        (Dcm_RoeInterMsgCycle == 0U))
    {
      /* Obtain the ID of the event so it can be found on the preconfiguration table */
      PreconfIdx = Dcm_Dsl_RoeServices_Runtime_Data[DCM_ROE_EVENT_DTC].PreconfIdx;
      /* Mask DTCNewState with the Set DTCStatusMask */
      /* !LINKSTO SWS_Dcm_00914,1 */
      if((DTCStatusNew & Dcm_RoeEventPreconfList[PreconfIdx].DTCStatusMask) != 0U)
      {
        Dcm_Dem_RoeServices_CommonTriggerProcedure(DCM_ROE_EVENT_DTC);
      }
    }
  }
  DBG_DCM_DSL_ROESERVICES_TRIGGERONDTCSTATUS_EXIT(RetVal, DTC, DTCStatusOld, DTCStatusNew);
}

STATIC FUNC(void, DCM_CODE) Dcm_Dem_RoeServices_CommonTriggerProcedure(Dcm_RoeEventTypeType EventType)
{

  Dcm_StatusType triggerRetVal = DCM_E_ROE_NOT_ACCEPTED;
  uint8 PreconfIdx = Dcm_Dsl_RoeServices_Runtime_Data[EventType].PreconfIdx;
  Dcm_MsgLenType ServiceToRespondToLenght = 0U;

  /* !LINKSTO SWS_Dcm_00923,1 */
  if(Dcm_RoeEnqueueEvent == DCM_ROE_NORMAL_EVENT)
  {
    Dcm_RoeEnqueueEvent = DCM_ROE_NO_EVENT;
  }
  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
  if(EventType == DCM_ROE_EVENT_DTC)
  {
    /* !LINKSTO SWS_Dcm_00915,1 */
    /* prepare the service request message */
    /* Service ID    */
    Dcm_Dsl_RoeRxData.MsgBuf[0] = DCM_RDTCI_REQUEST_SID;
    /* Sub-Service ID     */
    Dcm_Dsl_RoeRxData.MsgBuf[1] = DCM_RDTCI_REQUEST_SSID;
    ServiceToRespondToLenght = DCM_RDTCI_REQUEST_LENGTH;
  }
  if(EventType == DCM_ROE_EVENT_DID)
  {
    /* prepare the service request message */
    /* Service ID    */
    Dcm_Dsl_RoeRxData.MsgBuf[0] = DCM_RDBI_REQUEST_SID;
    /* DID high byte */
    Dcm_Dsl_RoeRxData.MsgBuf[1] = (uint8)(Dcm_RoeEventPreconfList[PreconfIdx].Did >> 8U);
    /* DID low  byte */
    Dcm_Dsl_RoeRxData.MsgBuf[2] = (uint8)(Dcm_RoeEventPreconfList[PreconfIdx].Did);
    ServiceToRespondToLenght = DCM_RDBI_REQUEST_LENGTH;
  }

  /* !LINKSTO SWS_Dcm_00921,1, SWS_Dcm_00128.Ref,2 */
  /* send the internal request */
  triggerRetVal = Dcm_Dsl_RoeServices_ResponseOnOneEvent(
    &(Dcm_Dsl_RoeRxData.MsgBuf[0]), ServiceToRespondToLenght,
    Dcm_Dsl_RoeServices_Runtime_Data[EventType].RxPduId);

  /* LEAVE critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  if (DCM_E_OK == triggerRetVal)
  {
    Dcm_Dsl_RoeServices_Runtime_Data[EventType].ActivationCnt++;
    Dcm_RoeInterMsgCycle = DCM_ROE_INTER_MSG_CYCLE_COUNT;
  }
  else
  {
    /* Nothing to do if an error has happened (SWS_Dcm_00521) */
  }
}
#if ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x00 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x40 == STD_ON))
/* !LINKSTO Dcm.Dsn.IB.RoeServices.StopROE,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_RoeServices_StopResponseOnEvent(boolean StoreEvent)
{
  Std_ReturnType RetVal = DCM_E_NOT_OK;
  Dcm_RoeEventTypeType EventType;
  Dcm_RoeEventTypeType ClearedEvents = 0U;
  boolean EventsToStop[DCM_ROE_EVENT_MAX] = {FALSE, FALSE};

  DBG_DCM_DSL_ROESERVICES_STOPRESPONSEONEVENT_ENTRY(StoreEvent);

  /* In the first loop mark the events to be stopped */
  for (EventType = DCM_ROE_EVENT_DTC; EventType < DCM_ROE_EVENT_MAX; ++EventType)
  {
    if ((Dcm_Dsl_RoeServices_Runtime_Data[EventType].State == DCM_ROE_STARTED) ||
        (Dcm_Dsl_RoeServices_Runtime_Data[EventType].State == DCM_ROE_STOPPED))
    {
      /* !LINKSTO SWS_Dcm_01076.Stop,1 */
      if ((Dcm_Dsl_RoeServices_Runtime_Data[EventType].EventWindowTime ==
          DCM_ROE_EVENT_WINDOW_CURRENT_CYCLE) && (StoreEvent == TRUE))
      {
        RetVal = DCM_E_REQUESTOUTOFRANGE;
        break;
      }
      EventsToStop[EventType] = TRUE;
    }
    else
    {
      DCM_PRECONDITION_ASSERT(
        Dcm_Dsl_RoeServices_Runtime_Data[EventType].State == DCM_ROE_CLEARED, DCM_INTERNAL_API_ID);
      ClearedEvents++;
    }
  }

  if ((DCM_E_NOT_OK == RetVal) && (ClearedEvents == DCM_ROE_EVENT_MAX))
  {
  /* !LINKSTO SWS_Dcm_00885,1 */
    RetVal = DCM_E_REQUESTSEQUENCEERROR;
  }

  /* If no error value was set, stop the marked events */
  if (DCM_E_NOT_OK == RetVal)
  {
    for (EventType = DCM_ROE_EVENT_DTC; EventType < DCM_ROE_EVENT_MAX; ++EventType)
    {
      if (TRUE == EventsToStop[EventType])
      {
        /* !LINKSTO SWS_Dcm_00877,1 */
        /* !LINKSTO SWS_Dcm_00900.Stop,1 */
        /* !LINKSTO SWS_Dcm_00906.StopROE,1 */
        if (Dcm_Dsl_RoeServices_Runtime_Data[EventType].State == DCM_ROE_STARTED)
        {
          Dcm_Dsl_RoeServices_ChangeState(EventType, DCM_ROE_STOPPED);
        }
#if (DCM_ROE_PERSISTENCE == STD_ON)
        /* !LINKSTO SWS_Dcm_00901.StopROE,1 */
        /* The event will not be stored persistently in case of
            DCM_ROE_EVENT_WINDOW_CURRENT_CYCLE and DEFAULT_SESSION */
        if (StoreEvent == TRUE)
        {
          Dcm_Dsl_RoeServices_Persistent_Data[EventType].Session = DCM_INVALID_SESSION;
          Dcm_Dsl_RoeServices_Persistent_Data[EventType].State = DCM_ROE_STOPPED;
          (void)NvM_SetRamBlockStatus(DCM_ROE_NVM_BLOCK_ID, TRUE);
        }
#endif /* (DCM_ROE_PERSISTENCE == STD_ON) */
        /* Invalidate the session, not to restore the event on default session re-entry */
        Dcm_Dsl_RoeServices_Runtime_Data[EventType].Session = DCM_INVALID_SESSION;
      }
    }
    RetVal = DCM_E_OK;
  }

  DBG_DCM_DSL_ROESERVICES_STOPRESPONSEONEVENT_EXIT(RetVal, StoreEvent);

  return RetVal;
}
#endif /* ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x00 == STD_ON) || \
           (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x40 == STD_ON)) */

#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x04 == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.RoeServices.ReportActivatedEvents,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_RoeServices_ReportActivatedEvents(
    Dcm_MsgType ResData, P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) MsgLen)
{
  Dcm_MsgLenType offset = 1U; /* offset 0: number of active events; offset 1: first event record */
  uint8 numOfActiveEvents = 0U;
  Dcm_RoeEventTypeType EventType;

  DBG_DCM_DSL_ROESERVICES_REPORTACTIVATEDEVENTS_ENTRY(ResData, MsgLen);

  for (EventType=DCM_ROE_EVENT_DTC; EventType<DCM_ROE_EVENT_MAX; ++EventType)
  {
    if (Dcm_Dsl_RoeServices_Runtime_Data[EventType].State == DCM_ROE_STARTED)
    {
      Dcm_RoeEventPreconfListType event =
        Dcm_RoeEventPreconfList[Dcm_Dsl_RoeServices_Runtime_Data[EventType].PreconfIdx];

      switch (event.EventType)
      {
        case DCM_ROE_EVENT_DID:
          /* eventType (subFunction) */
          ResData[offset] = (Dcm_MsgItemType)(DCM_ROE_SUBSERVICE_ONCHANGEOFDATAIDENTIFIER);
#if (DCM_ROE_PERSISTENCE == STD_ON)
          /* Due to the fact that the current implementation does not contain a Setup
           * function for OnChangeOfDID, this condition is not reachable, because is not
           * possible for the status on the persistent data of the DID event to be CLEAR
           * and at the same time for the DID event to have reached the STARTED state. The
           * only way to reach the STARTED state is to have a pre-configured DID event to the
           * STOPPED state.*/
          DCM_PRECONDITION_ASSERT(
            (Dcm_Dsl_RoeServices_Persistent_Data[DCM_ROE_EVENT_DID].State != DCM_ROE_CLEARED),
              DCM_INTERNAL_API_ID);
          ResData[offset] |= DCM_ROE_PERSISTENCE_BIT;
#endif
          offset++;
          /* eventWindowTime */
          ResData[offset] = Dcm_Dsl_RoeServices_Runtime_Data[DCM_ROE_EVENT_DID].EventWindowTime;
          offset++;
          /* eventTypeRecord */
          ResData[offset] = DCM_HIBYTE(event.Did);     /* DID high byte */
          offset++;
          ResData[offset] = DCM_LOBYTE(event.Did);     /* DID low  byte */
          offset++;
          /* serviceToResponseTo */
          ResData[offset] = DCM_RDBI_REQUEST_SID;
          offset++;
          ResData[offset] = DCM_HIBYTE(event.Did);     /* DID high byte */
          offset++;
          ResData[offset] = DCM_LOBYTE(event.Did);     /* DID low  byte */
          offset++;
          break;
        case DCM_ROE_EVENT_DTC:
          /* eventType (subFunction) */
          ResData[offset] = (Dcm_MsgItemType)(DCM_ROE_SUBSERVICE_ONDTCSTATUSCHANGE);
#if (DCM_ROE_PERSISTENCE == STD_ON)
          /* if the event was setup permanently, the state is not CLEARED */
          if (Dcm_Dsl_RoeServices_Persistent_Data[DCM_ROE_EVENT_DTC].State != DCM_ROE_CLEARED)
          {
            ResData[offset] |= DCM_ROE_PERSISTENCE_BIT;
          }
#endif
          offset++;
          /* eventWindowTime */
          ResData[offset] = Dcm_Dsl_RoeServices_Runtime_Data[DCM_ROE_EVENT_DTC].EventWindowTime;
          offset++;
          /* eventTypeRecord */
          ResData[offset] = event.DTCStatusMask;     /* DTC mask set */
          offset++;
          /* serviceToResponseTo */
          ResData[offset] = DCM_RDTCI_REQUEST_SID;
          offset++;
          ResData[offset] = DCM_RDTCI_REQUEST_SSID;
          offset++;
          break;
        /* CHECK: NOPARSE */
        default:
          /* Unreachable default case mandated by MISRA */
          DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
          break;
          /* CHECK: PARSE */
      }

      numOfActiveEvents++;
    }
  }

  /* offset 0: number of active events */
  ResData[0U] = numOfActiveEvents;

  /* set the message length */
  *MsgLen = offset;

  DBG_DCM_DSL_ROESERVICES_REPORTACTIVATEDEVENTS_EXIT(ResData, MsgLen);
}
#endif /* (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x04 == STD_ON) */

#if ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x05 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x45 == STD_ON))
/* !LINKSTO Dcm.Dsn.IB.RoeServices.StartROE,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_RoeServices_StartResponseOnEvent(
  boolean StoreEvent, PduIdType RxPduId)
{
  Std_ReturnType RetVal = DCM_E_NOT_OK;
  Dcm_RoeEventTypeType EventType;
  Dcm_RoeEventTypeType ClearedEvents = 0;
  boolean EventsToStart[DCM_ROE_EVENT_MAX] = {FALSE, FALSE};

  DBG_DCM_DSL_ROESERVICES_STARTRESPONSEONEVENT_ENTRY(StoreEvent, RxPduId);

  /* In the first loop mark the events to be started */
  for (EventType = DCM_ROE_EVENT_DTC; EventType < DCM_ROE_EVENT_MAX; ++EventType)
  {
    if (Dcm_Dsl_RoeServices_Runtime_Data[EventType].State == DCM_ROE_STOPPED)
    {
      /* !LINKSTO SWS_Dcm_01076.Start,1 */
      if ((Dcm_Dsl_RoeServices_Runtime_Data[EventType].OrigEventWindowTime ==
          DCM_ROE_EVENT_WINDOW_CURRENT_CYCLE) && (StoreEvent == TRUE))
      {
        RetVal = DCM_E_REQUESTOUTOFRANGE;
        break;
      }

      EventsToStart[EventType] = TRUE;
    }
    else if (Dcm_Dsl_RoeServices_Runtime_Data[EventType].State == DCM_ROE_CLEARED)
    {
      ClearedEvents++;
    }
    else
    {
      /* No action required (MISRA) */
    }
  }

  if ((DCM_E_NOT_OK == RetVal) && (ClearedEvents == DCM_ROE_EVENT_MAX))
  {
    /* !LINKSTO SWS_Dcm_00886,1 */
    RetVal = DCM_E_REQUESTSEQUENCEERROR;
  }

  /* If no error value was set, start the marked events */
  if (DCM_E_NOT_OK == RetVal)
  {
    /* !LINKSTO SWS_Dcm_00876,1 */
    for (EventType = DCM_ROE_EVENT_DTC; EventType < DCM_ROE_EVENT_MAX; ++EventType)
    {
      if (TRUE == EventsToStart[EventType])
      {
        Dcm_SesCtrlType CurrentSession;

        (void)Dcm_GetSesCtrlTypeExt(&CurrentSession);

        /* !LINKSTO SWS_Dcm_00127.Ref,2 */
        Dcm_Dsl_RoeServices_Runtime_Data[EventType].RxPduId = RxPduId;

        Dcm_Dsl_RoeServices_Runtime_Data[EventType].Session = CurrentSession;
        Dcm_Dsl_RoeServices_Runtime_Data[EventType].ActivationCnt = 0;
        Dcm_Dsl_RoeServices_Runtime_Data[EventType].EventWindowTime =
          Dcm_Dsl_RoeServices_Runtime_Data[EventType].OrigEventWindowTime;

        Dcm_Dsl_RoeServices_ChangeState(EventType, DCM_ROE_STARTED);

        #if (DCM_ROE_PERSISTENCE == STD_ON)
        /* !LINKSTO SWS_Dcm_00901.EndOfPowerCycle,1 */
        /* The event will not be stored persistently in case of
            DCM_ROE_EVENT_WINDOW_CURRENT_CYCLE and DEFAULT_SESSION */
        /* !LINKSTO SWS_Dcm_00906.PowerCycleEnd,1 */
        if ((StoreEvent == TRUE) && (CurrentSession == DCM_DEFAULT_SESSION))
        {
          /* Store persistently only if it was started in default session */
          /* copy runtime data to persistent data */
          TS_MemCpy(&Dcm_Dsl_RoeServices_Persistent_Data[EventType],
            &Dcm_Dsl_RoeServices_Runtime_Data[EventType],
            sizeof(Dcm_RoeEventDataType));
          (void)NvM_SetRamBlockStatus(DCM_ROE_NVM_BLOCK_ID, TRUE);
        }
#endif /* (DCM_ROE_PERSISTENCE == STD_ON) */

      }
    }
    RetVal = DCM_E_OK;
  }

  DBG_DCM_DSL_ROESERVICES_STARTRESPONSEONEVENT_EXIT(RetVal, StoreEvent, RxPduId);

  return RetVal;
}
#endif /* ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x05 == STD_ON) || \
           (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x45 == STD_ON)) */

#if ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x01 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x41 == STD_ON))
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_RoeServices_SetupEvent_DTCStatusChange(
  boolean StoreEvent, uint8 DTCMask, uint8 EventWindowTime)
{
  Std_ReturnType RetVal = DCM_E_NOT_OK;
  uint8 PreconfEventIndex;
  boolean DTCEventPreconfigured = FALSE;
#if (DCM_ROE_PERSISTENCE == STD_ON)
  boolean isStoreEvent;
#endif /* (DCM_ROE_PERSISTENCE == STD_ON) */

  DBG_DCM_DSL_ROESERVICES_SETUPEVENT_DTCSTATUSCHANGE_ENTRY(
                                        StoreEvent, DTCMask, EventWindowTime);

#if (DCM_ROE_PERSISTENCE == STD_ON)
  isStoreEvent = StoreEvent;
#else
  TS_PARAM_UNUSED(StoreEvent);
#endif /* (DCM_ROE_PERSISTENCE == STD_ON) */


  /* Search the pre-configured Event */
  /* Deviation TASKING-1 <+2> */
  /* !LINKSTO SWS_Dcm_00908.PreconfiguredRequests,1 */
  for (PreconfEventIndex = 0U; PreconfEventIndex < DCM_ROE_NUM_PRECONF_EVENT; ++PreconfEventIndex)
  {
    /* !LINKSTO SWS_Dcm_00952,1 */
    /* !LINKSTO SWS_Dcm_00953,1 */
    if(DTCMask == Dcm_RoeEventPreconfList[PreconfEventIndex].DTCStatusMask)
    {
      DTCEventPreconfigured = TRUE;
      Dcm_Dsl_RoeServices_Runtime_Data[DCM_ROE_EVENT_DTC].PreconfIdx = PreconfEventIndex;
      break;
    }
  }
  /* Event has been found */
  if(DTCEventPreconfigured == TRUE)
  {
    uint8_least PreconfEventWindowIndex;
    boolean EventWindowPreconfigured = FALSE;
    /* See if event window is pre-configured */
    /* !LINKSTO SWS_Dcm_00903,1 */
    /* Deviation TASKING-1 <+2> */
    for (PreconfEventWindowIndex = 0U;
         PreconfEventWindowIndex < DCM_ROE_NUM_PRECONF_EVENT_WINDOW_TIMES;
         ++PreconfEventWindowIndex)
    {
      if( EventWindowTime == Dcm_RoeEventWindowTimePreconfList[PreconfEventWindowIndex].EventWindowTime)
      {
        EventWindowPreconfigured = TRUE;
        Dcm_Dsl_RoeServices_Runtime_Data[DCM_ROE_EVENT_DTC].OrigEventWindowTime = EventWindowTime;
        Dcm_Dsl_RoeServices_Runtime_Data[DCM_ROE_EVENT_DTC].EventWindowTime =
            Dcm_Dsl_RoeServices_Runtime_Data[DCM_ROE_EVENT_DTC].OrigEventWindowTime;
#if (DCM_ROE_PERSISTENCE == STD_ON)
        /* Overwrite the storage parameter, if needed. */
        /* !LINKSTO Dcm.EB.ECUC_Dcm_00983,1 */
        isStoreEvent = isStoreEvent && (Dcm_RoeEventWindowTimePreconfList[PreconfEventWindowIndex].StorageState);
#endif /* (DCM_ROE_PERSISTENCE == STD_ON) */
        break;
      }
    }
    /* Event Window has been found */
    if(EventWindowPreconfigured == TRUE)
    {
      Dcm_SesCtrlType CurrentSession;

      (void)Dcm_GetSesCtrlTypeExt(&CurrentSession);
      /* All conditions for processing have passed */
      /* !LINKSTO SWS_Dcm_00873.OnDTCStatusChange,1 */
      Dcm_Dsl_RoeServices_ChangeState(DCM_ROE_EVENT_DTC, DCM_ROE_STOPPED);

#if (DCM_ROE_PERSISTENCE == STD_ON)
      /* !LINKSTO SWS_Dcm_00901.EndOfPowerCycle,1 */
      /* The event will not be stored persistently in case of
          DCM_ROE_EVENT_WINDOW_CURRENT_CYCLE and DEFAULT_SESSION */
      /* !LINKSTO SWS_Dcm_00906.PowerCycleEnd,1 */
      if ((isStoreEvent == TRUE) &&
          (CurrentSession == DCM_DEFAULT_SESSION) &&
          (EventWindowTime != DCM_ROE_EVENT_WINDOW_CURRENT_CYCLE))
      {
        /* Store persistently only if it was started in default session */
        /* copy runtime data to persistent data */
        TS_MemCpy(&Dcm_Dsl_RoeServices_Persistent_Data[DCM_ROE_EVENT_DTC],
          &Dcm_Dsl_RoeServices_Runtime_Data[DCM_ROE_EVENT_DTC],
          sizeof(Dcm_RoeEventDataType));
        (void)NvM_SetRamBlockStatus(DCM_ROE_NVM_BLOCK_ID, TRUE);
      }
#endif /* (DCM_ROE_PERSISTENCE == STD_ON) */
      RetVal = DCM_E_OK;
    }
    else
    {
      /* !LINKSTO SWS_Dcm_00896,1 */
      RetVal = DCM_E_REQUESTOUTOFRANGE;
    }
  }
  else
  {
    RetVal = DCM_E_REQUESTOUTOFRANGE;
  }

  DBG_DCM_DSL_ROESERVICES_SETUPEVENT_DTCSTATUSCHANGE_EXIT(
                                   RetVal, StoreEvent, DTCMask, EventWindowTime);
  return RetVal;
}
#endif /*#if ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x01 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x41 == STD_ON))*/
/*==================[internal function definitions]==============================================*/

/* !LINKSTO Dcm.Dsn.IB.RoeServices.ChangeState,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsl_RoeServices_ChangeState(
  Dcm_RoeEventTypeType Event, Dcm_RoeEventStateType State)
{

  Dcm_Dsl_RoeServices_Runtime_Data[Event].State = State;

#if DCM_MODEDECLARATION_SUPPORT == STD_ON
  /* !LINKSTO SWS_Dcm_00934,1 */
  (void)Dcm_RoeSchMSwitchFunctionList[Dcm_Dsl_RoeServices_Runtime_Data[Event].PreconfIdx](State);
#endif

  /* !LINKSTO SWS_Dcm_00925.Stopped,1 */
  /* !LINKSTO SWS_Dcm_00925.Cleared,1 */
  if ((DCM_ROE_NORMAL_EVENT == Dcm_RoeEnqueueEvent) &&
      ((DCM_ROE_CLEARED == State) || (DCM_ROE_STOPPED == State)))
  {
    DBG_DCM_ROEENQUEUEEVENT((Dcm_RoeEnqueueEvent), (DCM_ROE_NO_EVENT))
    Dcm_RoeEnqueueEvent = DCM_ROE_NO_EVENT;
  }
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsl_RoeServices_GetRoeRxDataInformation(
  P2CONST(Dcm_RoeRxDataType, AUTOMATIC, DCM_VAR) RoeRxData,
  P2VAR(Dcm_RoeRxDataType, AUTOMATIC, DCM_VAR) RoeRxDataCopy)
{
  RoeRxDataCopy->MsgBuf[0] = RoeRxData->MsgBuf[0];
  RoeRxDataCopy->MsgBuf[1] = RoeRxData->MsgBuf[1];
  RoeRxDataCopy->MsgBuf[2] = RoeRxData->MsgBuf[2];
  RoeRxDataCopy->RxBuffer.FilledLength = RoeRxData->RxBuffer.FilledLength;
  RoeRxDataCopy->RxBuffer.Size = RoeRxData->RxBuffer.FilledLength;;
  RoeRxDataCopy->RxBuffer.BufferPtr = &(RoeRxDataCopy->MsgBuf[0]);
  RoeRxDataCopy->RxPduId = RoeRxData->RxPduId;
}

/* !LINKSTO Dcm.Dsn.IB.RoeServices.EventDispatch,1 */
STATIC FUNC(Dcm_StatusType, DCM_CODE) Dcm_Dsl_RoeServices_RoeEventDispatch(
  P2VAR(Dcm_RoeRxDataType, AUTOMATIC, DCM_VAR) RoeRxData)
{
  Dcm_StatusType              RetVal = DCM_E_ROE_NOT_ACCEPTED;
  Dcm_RequestTypeType AcceptAs = DCM_REJECTED_REQUEST;

  /* Query the Supervisor whether the request can be received by it right now */
  /* the message length (2nd parameter) is not processed in case of ROE */
  /*  RxPermission = Dcm_Dsl_Supervisor_RequestRxPermission(RoeRxData->RxPduId, 0U, DCM_INTERNAL_ROE); */


  AcceptAs = Dcm_Dsl_Supervisor_RequestRxPermission(RoeRxData->RxPduId, 0U, DCM_INTERNAL_ROE);

  if (
      (DCM_ROE_TYPE1_PHYSICAL_REQUEST == AcceptAs) ||
      (DCM_ROE_TYPE1_FUNCTIONAL_REQUEST == AcceptAs) ||
      (DCM_ROE_TYPE2_PHYSICAL_REQUEST == AcceptAs) ||
      (DCM_ROE_TYPE2_FUNCTIONAL_REQUEST == AcceptAs)
     )
  {
    /* Make a copy of "RoeRxData" and use the copy further in the code. */
    Dcm_Dsl_RoeServices_GetRoeRxDataInformation(RoeRxData, &Dcm_Dsl_RoeRxDataCopy);

    /* Announce the Supervisor that a reception has been started so
     * it may take the necessary actions. */
    Dcm_Dsl_Supervisor_IncomingReception(Dcm_Dsl_RoeRxDataCopy.RxPduId, AcceptAs);

    /* Signal the reception of this ROE-generated request on the RxConnection of
       the respective RxPduId. */
    Dcm_Dsl_RxConnectionRxIndication(
      Dcm_Dsl_RoeRxDataCopy.RxPduId, AcceptAs, Dcm_Dsl_RoeRxDataCopy.RxBuffer);

    DBG_DCM_ROEENQUEUEEVENT((Dcm_RoeEnqueueEvent), (DCM_ROE_NO_EVENT))
    Dcm_RoeEnqueueEvent = DCM_ROE_NO_EVENT;
    RetVal = DCM_E_OK;
  }
  else if (DCM_POSTPONED_ROE_REQUEST == AcceptAs)
  {
    DBG_DCM_ROEENQUEUEEVENT((Dcm_RoeEnqueueEvent), (DCM_ROE_NORMAL_EVENT))
    Dcm_RoeEnqueueEvent = DCM_ROE_NORMAL_EVENT;
    RetVal = DCM_E_OK;
  }
  else
  {
    RetVal = DCM_E_ROE_NOT_ACCEPTED;
  }

  return RetVal;
}

/* !LINKSTO Dcm.Dsn.IB.RoeServices.ResponseOnOneEvent,1 */
STATIC FUNC(Dcm_StatusType, DCM_CODE) Dcm_Dsl_RoeServices_ResponseOnOneEvent(
  P2VAR(Dcm_MsgItemType, AUTOMATIC, DCM_CONST) MsgPtr,
  Dcm_MsgLenType                 MsgLen,
  PduIdType                      DcmRxPduId)
{
  Dcm_StatusType RetVal = DCM_E_ROE_NOT_ACCEPTED;

  /* Fill in the dedicated ROE reception buffer descriptor */
  Dcm_Dsl_RoeRxData.RxBuffer.FilledLength = MsgLen;
  Dcm_Dsl_RoeRxData.RxBuffer.Size = MsgLen;

  /* !LINKSTO SWS_Dcm_00132,1 */
  Dcm_Dsl_RoeRxData.RxBuffer.BufferPtr = MsgPtr;

  Dcm_Dsl_RoeRxData.RxPduId = DcmRxPduId;

  RetVal = Dcm_Dsl_RoeServices_RoeEventDispatch(&Dcm_Dsl_RoeRxData);

  return RetVal;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* (DCM_DSP_USE_DIDSERVICES == STD_ON) */
/*==================[end of file]================================================================*/
