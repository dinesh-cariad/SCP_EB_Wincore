/**
 * \file
 *
 * \brief AUTOSAR Xcp
 *
 * This file contains the implementation of the AUTOSAR
 * module Xcp.
 *
 * \version 2.12.0
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
 * MISRAC2012-1) Deviated Rule: 15.4 (advisory)
 * There should be no more than one break or goto statement used to terminate any iteration statement.
 *
 * Reason:
 * The iteration needs to be terminated when some particular conditions are satisfied
 * (storing position for current DTO is found).
 *
 * MISRAC2012-2) Deviated Rule: 13.5 (required)
 * The right hand operand of a logical && or || operator shall not contain persistent side effects.
 *
 * Reason:
 * Xcp_IsPosibleToProcessEvent() doesn't have side effects.
 *
 * 
 * MISRAC2012-3) Deviated Rule: 17.2 (required)
 * Functions shall not call themselves, either directly or indirectly
 *
 * Reason:
 * Memory proxy is designed in order to access user memory. Code base generic for accessing the memory
 * in the same partition or cross partition. Events are processed sequentially and each event may use
 * one or multiple memory proxy in order to access user data. In order to avoid delays in processing the events,
 * the next pending event is triggered as soon as the processing of the current event is complete.
 * Call graph recursion may only occur in the case when memory proxy is accessing local data on the same partition.
 * The number of possible call graph recursions is limited by the number of events to be processed.
 *
 */

/*==================[inclusions]============================================*/

#include <Xcp.h>               /* Module public API */
#if ( (XCP_DEFAULT_XCP_MODULE_STATE == STD_ON) || (XCP_ENABLE_XCP_CONTROL_API == STD_ON) )
#include <TSAutosar.h>         /* EB specific standard types */
#include <TSAtomic_Assign.h>   /* EB specific standard types */
#include <TSMem.h>             /* EB memory functions */
#include <SchM_Xcp.h>          /* Needed for exclusive area definition */
#include <Xcp_Trace.h>

#include <Xcp_Int.h>           /* Module internal interface */
#include <Xcp_UserCallouts.h>  /* XCP user callout functions */

#if (XCP_MEMORY_ACCESS_AREAS == STD_ON)
#include <Xcp_MemoryProxy.h>
#include <Os.h>                  /* Os counter is required */
#endif

#if (XCP_TIMESTAMP_TYPE != XCP_NO_TIME_STAMP_TS_MASK)

#if ((XCP_USER_TIMESTAMP_SUPPORTED == STD_OFF) && (XCP_MEMORY_ACCESS_AREAS == STD_OFF))
#include <Os.h>                  /* Os counter is required */
#endif

#endif /* (XCP_TIMESTAMP_TYPE != XCP_NO_TIME_STAMP_TS_MASK) */


/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if (XCP_MAX_DAQ != 0U)

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

/** \brief Function to update the currently processed event
 **
 ** This function checks the consistency of the current processed set event
 ** and sets up the DAQ list/ ODT to be processed depending on the consistency level of that event.
 **
 ** \param[in] EventID                The event channel which needs to be processed
 ** \param[in] EventProcessStatus     Event processing status
 */
STATIC FUNC(void, XCP_CODE) Xcp_UpdateEvent
(
  uint16 EventID,
  Xcp_ReturnType EventProcessStatus
);

/** \brief Function to update or clear the event for DAQ consistency
 **
 ** This function checks if all the DAQ lists of the currently processed event are covered:
 ** if covered the event will be clear, else the next DAQ list will be selected to be processed.
 **
 ** \param[in] EventID     The Event ID which needs to be processed
 ** \param[in] EvDaqIndex  The DAQ list which needs to be processed
 */
STATIC FUNC(void, XCP_CODE) Xcp_UpdateDaq
(
  uint16 EventID,
  uint8 EvDaqIndex
);

/** \brief Function to update or clear the currently processed event for Odt consistency
 **
 ** This function checks if all the ODTs of the currently processed event are covered: if covered
 ** then the next DAQ list information in the event list will be considered, if all the associated
 ** DAQ lists are covered then the event will be clear.
 **
 ** \param[in] EventID     The Event ID which needs to be processed
 ** \param[in] EvDaqIndex  The Daq Index which needs to be processed
 ** \param[in] OdtIndex    The Odt Index which needs to be processed
 */
STATIC FUNC(void, XCP_CODE) Xcp_UpdateOdt
(
  uint16 EventID,
  uint8 EvDaqIndex,
  uint8 OdtIndex
);

/** \brief Function to Sample the DTOs
 **
 ** This function is used to sample the DTOs depending on the consistency of
 ** the event.
 **
 ** \param[in] EventID     The Event ID which needs to be processed
 */
STATIC FUNC(void, XCP_CODE) Xcp_SampleData
(
  uint16 EventID
);

/** \brief Function to Sample the Data of the DTO
 **
 ** This function is used to get the data from the address, and store the
 ** entire DTO information in the DTO queue.
 **
 ** \param[in] DaqId       The DaqList Id needs to be processed
 ** \param[in] OdtIndex    The Odt Index which needs to be processed
 */
STATIC FUNC(void, XCP_CODE) Xcp_SampleDtoData
(
  Xcp_DaqIdType DaqId,
  uint8 OdtIndex
);

/** \brief Function to Sample the DAQ for DTO
 **
 ** This function is used to get the data from the address, and store the
 ** entire DTO information in the DTO queue for the entire Daq.
 **
 ** \param[in] DaqId       The DaqList Id needs to be processed
 */
STATIC FUNC(void, XCP_CODE) Xcp_SampleDaq
(
  Xcp_DaqIdType DaqId
);

/** \brief Function to Sample the Event for DTO
 **
 ** This function is used to get the data from the address, and store the
 ** entire DTO information in the DTO queue for the entire Event.
 **
 ** \param[in] EventID     The Event Id which needs to be processed
 */
STATIC FUNC(void, XCP_CODE) Xcp_SampleEvent
(
  uint16 EventID
);

/** \brief Function to find the next ODT to be sampled or stimulated for the currently processed event.
 **
 ** This function is used to find the next ODT to be processed based on the direction of the
 ** DAQ lists for the currently processed event. If the DAQ List direction is 'DAQ',
 ** it calculates the total memory needed for sampling data of the ODT.
 ** If the DAQ List direction is 'STIM', it checks whether the data for the ODT(s) are available
 ** in the STIM buffer.
 **
 ** \param[in] EventID        The event channel which is currently processed
 **
 ** \retval XCP_OK           Event is ready to be processed
 ** \retval XCP_NOT_OK       Event is not ready to be processed. No DaqList is ready
 **                          to be processed.
 ** \retval XCP_STIM_TIMEOUT Event is not ready to be processed. STIM_TIMEOUT error
 **                          is detected for at least one DaqList.
 */
STATIC FUNC(Xcp_ReturnType, XCP_CODE)  Xcp_GetEventProcReadyStatus
(
  uint16 EventID
);

/** \brief Function to calculate the total memory needed to store only the header of a single ODT
 ** in DTO
 **
 ** This function is used to calculate the total memory needed to store the header of a single ODT
 **
 ** \param[in] DaqListId   The DAQ list Id which needs to be processed
 ** \param[in] OdtIndex    The Odt Index which needs to be processed
 **
 ** \return The amount of memory required in the DTO queue for storing the header of a single ODT.
 */
STATIC FUNC(uint16, XCP_CODE)  Xcp_GetDtoHeaderSize(Xcp_DaqIdType DaqListId, uint8 OdtIndex);

/** \brief Function to determine whether the Event, having consistency EVENT,
 **        is ready to be processed consistently.
 **
 ** Use this function in case event consistency is EVENT.
 **
 ** This function checks whether the event can be processed consistently:
 ** - all Stim packages for the active DaqList are received
 ** - we have at least one active DaqList for acquisition/stimulation for
 **   the selected event.
 **
 ** \retval XCP_OK           Event is ready to be processed
 ** \retval XCP_NOT_OK       Event is not ready to be processed. No DaqList is ready
 **                          to be processed.
 ** \retval XCP_STIM_TIMEOUT Event is not ready to be processed. STIM_TIMEOUT error
 **                          is detected for at least one DaqList.
 */
STATIC FUNC(Xcp_ReturnType, XCP_CODE)  Xcp_GetEventProcStatus(uint16 EventId);

/** \brief Function to determine whether the Event, having consistency DAQ or ODT,
 **        is ready to be processed.
 **
 ** Use this function in case event consistency is DAQ or ODT.
 **
 ** \retval XCP_OK           Event is ready to be processed
 ** \retval XCP_NOT_OK       Event is not ready to be processed. No DaqList is ready
 **                          to be processed.
 ** \retval XCP_STIM_TIMEOUT Event is not ready to be processed. STIM_TIMEOUT error
 **                          is detected for the selected DaqList.
 */
STATIC FUNC(Xcp_ReturnType, XCP_CODE)  Xcp_GetNextDaqListProcStatus(uint16 EventId);

/** \brief Function to determine whether a DAQ list is ready to be sampled
 **
 ** Use this function before sampling a DAQ List or before calculating memory
 ** required by the DTOs while servicing an event.
 **
 ** This function checks whether the DAQ list is of direction DAQ and whether
 ** the DAQ List is running, if so a DAQ list is ready if its prescaler counter reads 1(one).
 ** Otherwise the prescaler shall be decremented when the DAQ is verified for the first ODT.
 **
 ** \param[in] DaqListNumber  The number of the required DAQ List
 ** \param[in] OdtIndex       The number of the required ODT in the specified DAQ List
 ** \return if the daq list is ready to be sampled.
 **
 */
STATIC FUNC(boolean, XCP_CODE) Xcp_IsDAQReadyToBeProcessed
(
  Xcp_DaqIdType DaqListNumber,
  uint8 OdtIndex
);

#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
/** \brief Function to prepare the EV_STIM_TIMEOUT event package notification
 **
 ** Function will send the EV_STIM_TIMEOUT to the XCP master with the required information.
 **
 ** \param[in] EventInfoType  The Info type of the event to be sent
 ** \param[in] EventInfo      Event channel number or DAQ list number depending on Info Type
 */
STATIC FUNC(void, XCP_CODE)  Xcp_EmitSTIMTimeoutEvent
(
  uint8 EventInfoType,
  uint16 EventInfo
);
#endif /* (XCP_EVENT_PACKET_ENABLED == STD_ON) */

/** \brief Function to check if the Stim packages are received for the selected DaqList.
**
** \param[in] EventId     The id of the processed event channel
** \param[in] DaqListId   Id of the DaqList for which the checking is made.
*/
STATIC FUNC(boolean, XCP_CODE) Xcp_IsStimProcReady
(
  uint16 EventId,
  Xcp_DaqIdType DaqListId
);

#endif /* (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK) */

#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
/** \brief Function to check if there is already an Event which requested
**  data sample of stimulation from MemProxy
*/
STATIC FUNC(boolean, XCP_CODE) Xcp_IsPosibleToProcessEvent(void);

/** \brief Function to check if there is posible to enqueue an Event
*/
STATIC FUNC(boolean, XCP_CODE) Xcp_IsPosibleToEnqueueEvent(uint16 EventID);

/** \brief Function to check if there are Events for which the request data
**  have been processed but the request to MemProxy was not sent
*/
STATIC FUNC(void, XCP_CODE) Xcp_ProcessQueuedEvents(void);
#endif

/** \brief Function to update the Daq Prio Queue
*/
STATIC FUNC(void, XCP_CODE) Xcp_UpdateDaqPrioQueue
(
  Xcp_DaqIdType DaqId,
  uint8 OdtIndex,
  Xcp_OdtSizeType DataLength
);

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
#define XCP_START_SEC_VAR_16
#include <Xcp_MemMap.h>

VAR(uint16, XCP_VAR) Xcp_ProcessedEventId;

#define XCP_STOP_SEC_VAR_16
#include <Xcp_MemMap.h>

#define XCP_START_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

VAR(Xcp_RequestToMemProxyStructType, XCP_VAR) Xcp_RequestToMemProxy;
VAR(Xcp_DaqPrioQueueOfRequestType, XCP_VAR) Xcp_DaqPrioQueueOfRequest;
VAR(Xcp_RequestToMemProxyType, XCP_VAR) Xcp_RequestBuffer[XCP_MAX_SIZE_OF_REQUEST_BUFFER];
VAR(Xcp_RequestForEventType, XCP_VAR) Xcp_RequestForEvent[XCP_MAX_EVENT_CHANNEL];

#define XCP_STOP_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>
#endif

/*==================[internal data]=========================================*/

#define XCP_START_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
STATIC VAR(Xcp_RequestQueueType, XCP_VAR) Xcp_RequestQueue;
#endif

#define XCP_STOP_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

#define XCP_START_SEC_VAR_8
#include <Xcp_MemMap.h>

#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
STATIC VAR(boolean, XCP_VAR) Xcp_EventIsProcessing = FALSE;
#endif

#define XCP_STOP_SEC_VAR_8
#include <Xcp_MemMap.h>
/*==================[external function definitions]=========================*/

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>


/*------------------[Xcp_ResetEventProcessor]--------------------------------*/

FUNC(void, XCP_CODE) Xcp_ResetEventProcessor
(
  void
)
{
  /* Local variable to hold DAQ List number */
  Xcp_DaqIdType DaqListNumber;
  /* Local variable to hold Event Channel number */
  uint16 EventChannel;
  /* Local variable to hold the index of associated DaqList */
  uint8 EvDaqIndex;

  DBG_XCP_RESETEVENTPROCESSOR_ENTRY();

  /* Reset the runtime configuration for each DAQ List */
  for (DaqListNumber = 0U; DaqListNumber < XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(Xcp_DaqIdType);
       DaqListNumber++)
  {
    /* Reset the running status and selected status */
    Xcp_ResetDaqListData(DaqListNumber);
  }

  /* Initialize Event channel run time properties:
   *  - Reset number of DAQ List associated with the event.
   *  - Reset run time status of the event channel.
   *  - Reset DAQ List Ids associated with the event.
   */
  for (EventChannel = 0U; EventChannel < XCP_MAX_EVENT_CHANNEL; ++EventChannel)
  {
    /* Reset run time status of the event channel */
    Xcp_Event[EventChannel].EventStatusFlag = 0U;
    /* Reset number of DAQ Lists associated with the event channel */
    Xcp_Event[EventChannel].DaqIdListCount = Xcp_EventInfo[EventChannel].DaqIdListCount;

    /* Set Event Channel Time Cycle Counter */
    Xcp_Event[EventChannel].TimeCycleCounter = Xcp_EventInfo[EventChannel].CycleCounterMax;

    /* Reset associated DaqId list for the event */
    for (EvDaqIndex = 0U; EvDaqIndex < Xcp_Event[EventChannel].DaqIdListCount; ++EvDaqIndex)
    {
      Xcp_Event[EventChannel].DaqIdListPtr[EvDaqIndex] =
        Xcp_EventInfo[EventChannel].DaqIdListPtr[EvDaqIndex];
    }
  }

  DBG_XCP_RESETEVENTPROCESSOR_EXIT();
}

/*------------------[Xcp_ResetDaqListData]--------------------------------*/

FUNC(void, XCP_CODE) Xcp_ResetDaqListData
(
  Xcp_DaqIdType DaqListNumber
)
{
  DBG_XCP_RESETDAQLISTDATA_ENTRY(DaqListNumber);

#if (XCP_DAQ_COUNT > 0U)
#if (XCP_MIN_DAQ > 0)
  if (DaqListNumber >= XCP_MIN_DAQ) /* DAQ list is dynamically configured */
#endif
  {
    /* Get the pointer to the DAQ list */
    P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqListNumber);

    /* Reset data */
    DaqListPtr->OdtList = NULL_PTR;
    DaqListPtr->EventId = XCP_INVALID_EVENT_CHANNEL;
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_ABSOLUTE_IF_MASK)
    DaqListPtr->FirstPID = 0U;
#endif
    DaqListPtr->Flags = 0U;
    DaqListPtr->MaxOdt = 0U;
    DaqListPtr->MaxOdtEntries = 0U;
    DaqListPtr->Mode = 0U;
    DaqListPtr->Prescaler = 0U;
    DaqListPtr->PrescalerCnt = 0U;
    DaqListPtr->Priority = 0U;
    DaqListPtr->Properties = 0U;
  }
#if (XCP_MIN_DAQ > 0)
  else
#endif
#endif /* (XCP_DAQ_COUNT > 0U) */
  {
#if ((XCP_MAX_DAQ != 0U) && (XCP_MAX_DAQ > XCP_DAQ_COUNT))
    /* Initialize the DAQ list with data from ROM  */
    TS_MemCpy(XCP_GET_DAQ_LIST_PTR(DaqListNumber),&Xcp_DaqListsDefault.Xcp_Daq[DaqListNumber],
        (uint16)(sizeof(Xcp_DaqType)));
#endif
  }

  DBG_XCP_RESETDAQLISTDATA_EXIT(DaqListNumber);
}

#if (XCP_MAX_MF_PROCESSED_EVENTS > 0)
/*------------------[Xcp_ProcessMFEvents]--------------------------------*/

FUNC(void, XCP_CODE) Xcp_ProcessMFEvents(void)
{
  uint16 MFEventId, EventChannelIntId;

  for (MFEventId=0U; MFEventId< XCP_MAX_MF_PROCESSED_EVENTS; MFEventId++)
  {
    EventChannelIntId = Xcp_MFEventCfg[MFEventId];
    /* If event was set */
    if (XCP_IS_BYTE_FLAG_SET(XCP_EVENT_IS_SET_FLAG_POS,
                          &(Xcp_Event[EventChannelIntId].EventStatusFlag)) == TRUE)
    {
      /* process selected event */
      Xcp_ProcessEvents(EventChannelIntId);
#if(XCP_PROCESS_MULTI_MF_EVENTS == STD_OFF)
      /* process only one event per Xcp_MainFunction in case only one is allowed */
      break;
#endif
    }
  }
}
#endif /* (XCP_MAX_MF_PROCESSED_EVENTS > 0) */

#if ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) || (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK))

/*------------------[Xcp_ProcessEvents]---------------------------------*/
FUNC(void, XCP_CODE) Xcp_ProcessEvents(uint16 EventId)
{

  Xcp_ReturnType EventProcessingStatus;

#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
  Xcp_MemoryProxyDataType Xcp_MemoryProxyData;
  uint8 AppIdx;
  uint16 OldIdx;
#endif

  DBG_XCP_PROCESSEVENTS_ENTRY();

#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
  /* check if event could not be processed by the MemProxy for the previous MF */
  if (Xcp_RequestForEvent[EventId].RequestStatus != XCP_NO_MEMPROXY_REQUESTED)
  {
    if (Xcp_MemoryProxyErrorPending == FALSE)
    {
      Xcp_MemoryProxyErrorPending = TRUE;
      /* !LINKSTO Xcp.dsn.BSWDistribution.EventProcessing.MemProxy.ReportOverloadError,1 */
      /* Report Error only on the first MF */
      XCP_DET_REPORT_RUNTIME_ERROR(XCP_SID_MAIN_FUNCTION, XCP_E_MEMORY_PROXY_MF_SYNC_ERROR);
    }
  }
  else
  {
    Xcp_RequestForEvent[EventId].ProcessingCtr = 0U;
  }
#endif
  /* get event processing status */
  EventProcessingStatus = Xcp_GetEventProcReadyStatus(EventId);

  if (EventProcessingStatus == XCP_OK)
  {
#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
    if ((Xcp_IsPosibleToEnqueueEvent(EventId) == TRUE) &&(Xcp_MemoryProxyErrorPending == FALSE) &&
        (Xcp_EventIsProcessing == FALSE))
#endif
    {
#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
      /* enter critical section */
      SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

      for (AppIdx = 0U; AppIdx < XCP_NUM_OF_OS_APPS; AppIdx++)
      {
        Xcp_RequestForEvent[EventId].EntriesForAppIdAvailable[AppIdx] = FALSE;
        Xcp_RequestForEvent[EventId].NoOfMemProxyRequests = 0U;
      }

      Xcp_ProcessedEventId = EventId;
      OldIdx = Xcp_RequestToMemProxy.WriteIdx;
      Xcp_EventIsProcessing = TRUE;

      /* exit critical section */
      SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
#endif
#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
      /* Stimulate the data */
      Xcp_StimulateData(EventId);
#endif /* (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK) */

#if (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)
      /* Sample data */
      Xcp_SampleData(EventId);

#if((XCP_EVENT_PACKET_ENABLED == STD_ON) && \
    (XCP_OVERLOAD_MSB_SUPPORT == STD_OFF))
    if(XCP_IS_BYTE_FLAG_SET(XCP_EVENT_OVELOAD_FLAG_POS,
                         &(Xcp_Event[EventId].EventStatusFlag)) == TRUE)
    {
      /* enter critical section */
      SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

      /* Emit an Event CTO with DAQ overload */
      Xcp_EmitDAQOverloadEvent();

      /* Clear event overload flag */
      XCP_CLEAR_BYTE_FLAG(XCP_EVENT_OVELOAD_FLAG_POS,
                          &(Xcp_Event[EventId].EventStatusFlag));

      /* exit critical section */
      SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
    }
#endif /* ((XCP_EVENT_PACKET_ENABLED == STD_ON) && \
        *  (XCP_OVERLOAD_MSB_SUPPORT == STD_OFF)) */
#endif /* (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) */
#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
      if (Xcp_RequestToMemProxy.WriteIdx == OldIdx)
      {
        Xcp_RequestToMemProxy.NoOfEntries[EventId] = 0U;
      }
      else if (Xcp_RequestToMemProxy.WriteIdx > OldIdx)
      {
        Xcp_RequestToMemProxy.NoOfEntries[EventId] = Xcp_RequestToMemProxy.WriteIdx - OldIdx;
      }
      else
      {
        Xcp_RequestToMemProxy.NoOfEntries[EventId] =
          Xcp_RequestToMemProxy.WriteIdx + XCP_MAX_SIZE_OF_REQUEST_BUFFER - OldIdx;
      }
      for (AppIdx = 0U; AppIdx < XCP_NUM_OF_OS_APPS; AppIdx++)
      {
        /* enter critical section */
        SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

        if (Xcp_RequestForEvent[EventId].EntriesForAppIdAvailable[AppIdx] == TRUE)
        {
          Xcp_RequestForEvent[EventId].NoOfMemProxyRequests++;
          Xcp_RequestForEvent[EventId].IndexOfRequest = OldIdx;
        }

        /* exit critical section */
        SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
      }
      /* If MemProxy is available we can send the request, otherwise the request for event
         will be queued */
      /* !LINKSTO Xcp.dsn.BSWDistribution.EventProcessing.MemProxy.SendRequest,1 */
      /* !LINKSTO Xcp.BswDistribution.Communication.UnitInteraction.CoreLocal,1 */
      /* Deviation MISRAC2012-2 <START> */
      if ((Atomics_Load(&Xcp_MemoryProxyState) == XCP_MEMORY_PROXY_AVAILABLE) &&
          (Xcp_IsPosibleToProcessEvent() == TRUE))
      /* Deviation MISRAC2012-2 <STOP> */
      {
        Atomic_value_t partition = Atomics_Load(&Xcp_PartitionOfActiveConnection);

        for (AppIdx = 0U; AppIdx < XCP_NUM_OF_OS_APPS; AppIdx++)
        {
          if (Xcp_RequestForEvent[EventId].EntriesForAppIdAvailable[AppIdx] == TRUE)
          {
            Xcp_MemoryProxyData.type = XCP_MEMORYPROXYDATA_EVENT_PROCESSOR;
            Xcp_MemoryProxyData.size = Xcp_RequestToMemProxy.NoOfEntries[EventId];
            /* Not used for this kind of request */
            Xcp_MemoryProxyData.address = NULL_PTR;
            Xcp_MemoryProxyData.RequestBuffer = &Xcp_RequestBuffer[0U];
            Xcp_MemoryProxyData.IdxRequestBuffer = Xcp_RequestForEvent[EventId].IndexOfRequest;
            Xcp_MemoryProxyData.EventId = EventId;
            Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_LOCKED);
            Xcp_RequestForEvent[EventId].RequestStatus = XCP_PROCESS_ON_MEMPROXY_REQUESTED;
            if (AppIdx != partition)
            {
              /* !LINKSTO Xcp.dsn.BSWDistribution.EventProcessing.DataAquisition.MemProxy,1 */
              /* !LINKSTO Xcp.dsn.BSWDistribution.EventProcessing.DataStimulation.MemProxy,1 */
              (void)Xcp_SendMemoryProxyData[partition][AppIdx](&Xcp_MemoryProxyData);
            }
            else
            {
              /* send request localy */
              Xcp_MemoryProxyHandler(&Xcp_MemoryProxyData);
            }
          }
        }
      }
      else
      {
        /* !LINKSTO Xcp.dsn.BSWDistribution.EventProcessing.MemProxy.EnqueueEvents,1 */
        /* queue the event for processing when possible */
        Xcp_RequestQueue.EventId[Xcp_RequestQueue.WriteIdx] = EventId;
        if ((Xcp_RequestQueue.WriteIdx + 1) < XCP_NUMBER_OF_QUEUED_EVENTS)
        {
          Xcp_RequestQueue.WriteIdx++;
        }
        else
        {
          Xcp_RequestQueue.WriteIdx = 0U;
        }
      }
      Xcp_EventIsProcessing = FALSE;
#endif
    }
  }

#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_OFF)
  /* Update event processing parameters */
  Xcp_UpdateEvent(EventId, EventProcessingStatus);
#endif

  DBG_XCP_PROCESSEVENTS_EXIT();
}

#endif

/*------------------[Xcp_FlushDTORunTimeData]-------------------------------------*/

FUNC(void, XCP_CODE) Xcp_FlushDTORunTimeData(void)
{
  Xcp_DaqIdType DaqListId;
  Xcp_DaqIdType NewOdtMaskId;

  DBG_XCP_FLUSHDTORUNTIMEDATA_ENTRY();

  Xcp_DaqPrioQueue.NoOfEntries = 0U;  /* Reset number of Entries from Daq Priority queue */

  for(DaqListId = 0U; DaqListId < XCP_MAX_DAQ; DaqListId++)
  {
    for(NewOdtMaskId = 0U; NewOdtMaskId < XCP_MAX_ODT_MASK_SIZE; NewOdtMaskId++)
    {
      /* reset DTO new data flags */
      Xcp_DaqRunTimeInfo[DaqListId].IsNewOdtDataMask[NewOdtMaskId] = 0U;
    }
  }

  DBG_XCP_FLUSHDTORUNTIMEDATA_EXIT();
}

/*-----------------------[Xcp_ClearStoppedDtos]------------------------------*/
FUNC(void, XCP_CODE) Xcp_ClearStoppedDtos(void)
{
  Xcp_DaqIdType QueueIndex = 0U;
  Xcp_DaqIdType DaqID;

  /* Variable to hold address of the ongoing DAQ List */
  P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr;

  DBG_XCP_CLEARSTOPPEDDTOS_ENTRY();

  /* enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* Process all DTOs in the queue */
  while (QueueIndex < Xcp_DaqPrioQueue.NoOfEntries)
  {
    /* Get DAQ list ID */
    DaqID = XCP_GET_DAQ_ID_FROM_PRIO(QueueIndex);

    /* Get a pointer to the DAQ list based on the DAQ list Id obtained in the previous call */
    DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqID);

    /* Check whether DAQ List is already running */
    if (XCP_IS_DAQ_LIST_RUNNING(DaqListPtr) != TRUE)
    {
#if (XCP_NO_DAQ_DTOS > 1U)
      /* Iterator */
      Xcp_DaqIdType DaqPrioQueueIndex;
      /* Remove from QUEUE the ODT related to the stopped DAQ */
      for(DaqPrioQueueIndex = QueueIndex;
          DaqPrioQueueIndex < (Xcp_DaqPrioQueue.NoOfEntries - 1U);
          DaqPrioQueueIndex++)
      {
        /* Out of bounds access shall not happen */
        XCP_PRECONDITION_ASSERT(((DaqPrioQueueIndex+1U) < XCP_NO_DAQ_DTOS), XCP_SID_MAIN_FUNCTION);

        /* Shift to left the DaqPrioQueeue */
        Xcp_DaqPrioQueue.DaqRuntimeDataArray[DaqPrioQueueIndex] =
            Xcp_DaqPrioQueue.DaqRuntimeDataArray[DaqPrioQueueIndex+1U];
      }
#endif
      /* Decrement number of entries from DaqPrioQueue*/
      Xcp_DaqPrioQueue.NoOfEntries--;
    }
    else
    {
      /* Increment Index of Xcp_DaqPrioQueue */
      QueueIndex++;
    }
  }

  /* exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  DBG_XCP_CLEARSTOPPEDDTOS_EXIT();
}

/*-----------------------[Xcp_ClearDaqListDtos]------------------------------*/

FUNC(void, XCP_CODE) Xcp_ClearDaqListDtos(Xcp_DaqIdType DaqId)
{
  /* Function shall be called inside exclusive area */
  Xcp_DaqIdType QueueIndex = 0U;

  DBG_XCP_CLEARDAQLISTDTOS_ENTRY(DaqId);

  /* Process all DTOs in the queue */
  while (QueueIndex < Xcp_DaqPrioQueue.NoOfEntries)
  {
    /* Check whether DAQ List is in prio list */
    if (XCP_GET_DAQ_ID_FROM_PRIO(QueueIndex) == DaqId)
    {
#if (XCP_NO_DAQ_DTOS > 1U)
      /* Iterator */
      Xcp_DaqIdType DaqPrioQueueIndex;
      /* Remove from QUEUE the ODT related to the stopped DAQ list */
      for(DaqPrioQueueIndex = QueueIndex;
          DaqPrioQueueIndex < (Xcp_DaqPrioQueue.NoOfEntries - 1U);
          DaqPrioQueueIndex++)
      {
        /* Out of bounds access shall not happen */
        XCP_PRECONDITION_ASSERT(((DaqPrioQueueIndex+1U) < XCP_NO_DAQ_DTOS), XCP_SID_MAIN_FUNCTION);

        /* Shift to left the DaqPrioQueeue */
        Xcp_DaqPrioQueue.DaqRuntimeDataArray[DaqPrioQueueIndex] =
            Xcp_DaqPrioQueue.DaqRuntimeDataArray[DaqPrioQueueIndex+1U];
      }
#endif
      /* Decrement number of entries from DaqPrioQueue*/
      Xcp_DaqPrioQueue.NoOfEntries--;
    }
    else
    {
      /* Increment Index of Xcp_DaqPrioQueue */
      QueueIndex++;
    }
  }

  DBG_XCP_CLEARDAQLISTDTOS_EXIT();
}

/*-----------------------------[Xcp_ResetEventChannels]---------------------*/

FUNC(void, XCP_CODE) Xcp_ResetEventChannels(void)
{
  /* iterator */
  uint16 i;

  DBG_XCP_RESETEVENTCHANNELS_ENTRY();
  /* Reset all events runtime values */
  for (i=0U; i< XCP_MAX_EVENT_CHANNEL; i++)
  {
    Xcp_Event[i].EventStatusFlag = 0U;
    Xcp_Event[i].NextDaqIndex = 0U;
    Xcp_Event[i].NextOdtIndex = 0U;
#if (XCP_NUMBER_OF_EVENT_MAIN_FUNCTIONS > 0U)
    Xcp_Event[i].NumberOfRunningDaqLists = 0U;
#endif
  }

  DBG_XCP_RESETEVENTCHANNELS_EXIT();
}

/*-----------------------------[Xcp_GetDaqIDFromDto]---------------------*/

FUNC(void, XCP_CODE) Xcp_GetDaqIDFromDto
(
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) DtoPtr,
  P2VAR(Xcp_DaqIdType, AUTOMATIC, AUTOMATIC) DaqID,
  P2VAR(uint8, AUTOMATIC, AUTOMATIC) OdtIndex
)
{
  uint8 OdtIndexLocal = DtoPtr[0U];
  Xcp_DaqIdType DaqIDLocal = 0U;
  /* Get the DAQ list */
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_ABSOLUTE_IF_MASK)
  P2CONST(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr;
#endif

  DBG_XCP_GETDAQIDFROMDTO_ENTRY(DtoPtr,DaqID,OdtIndex);

#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_ABSOLUTE_IF_MASK)

  /* Find the DAQ list that has the queried ODT */
  for (DaqIDLocal = 0U; DaqIDLocal < XCP_GET_NO_OF_CONFIGURED_DAQ_LISTS(Xcp_DaqIdType);
       DaqIDLocal++)
  {
    /* Get the DAQ list */
    DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqIDLocal);
    if (OdtIndexLocal < (DaqListPtr->FirstPID + DaqListPtr->MaxOdt))
    {
      /* We have found our DAQ list that has the ODT corresponding to the PID,
       * calculate the original ODT index */
      OdtIndexLocal -= DaqListPtr->FirstPID;
      break;
    }
  }

#elif (XCP_IDENTIFICATION_FIELD_TYPE == XCP_RELATIVE_BYTE_IF_MASK)

  /* Get the Daq Id directly from DTO */
  DaqIDLocal = DtoPtr[1U];

#elif (XCP_IDENTIFICATION_FIELD_TYPE == XCP_RELATIVE_WORD_IF_MASK)

  /* Get the 16-bit Daq Id directly from DTO */
  DaqIDLocal = XCP_UINT16_FROM_BYTE_ARRAY(&DtoPtr[1U]);

#else

  /* Get the 16-bit Daq Id directly from DTO */
  DaqIDLocal = XCP_UINT16_FROM_BYTE_ARRAY(&DtoPtr[2U]);

#endif

  *DaqID = DaqIDLocal;
  *OdtIndex = OdtIndexLocal;

  DBG_XCP_GETDAQIDFROMDTO_EXIT(DtoPtr,DaqID,OdtIndex);
}

/*==================[internal function definitions]=========================*/

/*------------------[Xcp_IsDAQReadyToBeProcessed]-----------------------------*/

STATIC FUNC(boolean, XCP_CODE) Xcp_IsDAQReadyToBeProcessed
(
  Xcp_DaqIdType DaqListNumber,
  uint8 OdtIndex
)
{
  /* Flag to check if the DAQ list is OK to be sampled now */
  boolean RetVal = FALSE;

  /* Get the DAQ list based on the DAQ list number */
  P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqListNumber);

  DBG_XCP_ISDAQREADYTOBESAMPLED_ENTRY(DaqListNumber,OdtIndex);

#if (XCP_PRESCALER_SUPPORTED != XCP_PRESCALER_MASK)
  TS_PARAM_UNUSED(OdtIndex);
#endif

  /*If the DAQ list active and the direction is DAQ */
  if ((DaqListPtr->Mode & XCP_MASK_DAQLIST_RUNNING) == XCP_MASK_DAQLIST_RUNNING)
  {
    uint8 len = 0U;
    if ((DaqListPtr->OdtList != NULL_PTR) && (DaqListPtr->OdtList->OdtEntry != NULL_PTR))
    {
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_ON)
      len = *(uint8*)Xcp_GetOdtEntryData(DaqListPtr->OdtList->OdtEntry,0,XCP_ODT_ENTRY_FIELD_LENGTH);
#else
      len = DaqListPtr->OdtList->OdtEntry->Length;
#endif
    }
    /* DAQ list can be sampled only if the first ODT entry of the first ODT has length > 0 (ASAM requirement) */
    if (len > 0U)
    {
#if (XCP_OVERLOAD_MSB_SUPPORT == STD_ON)
      /* handle overload reporting when detection doesn't affect the current daqlist */
      if (((DaqListPtr->Mode & XCP_MASK_DAQLIST_DIRECTION) == XCP_MASK_DAQLIST_DIRECTION_DAQ) &&
          ((DaqListPtr->Flags & XCP_MASK_DAQLIST_SKIPOVCHECK) == XCP_MASK_DAQLIST_SKIPOVCHECK))
      {
        if((DaqListPtr->Flags & XCP_MASK_DAQLIST_OVCHECKSKIPPED) == XCP_MASK_DAQLIST_OVCHECKSKIPPED)
        {
          /* enable the checking for event channel overload detection */
          DaqListPtr->Flags &= (uint8)(~XCP_MASK_DAQLIST_SKIPOVCHECK);
          DaqListPtr->Flags &= (uint8)(~XCP_MASK_DAQLIST_OVCHECKSKIPPED);
        }
        else
        {
          /* prevent checking for Event overload for one Event processing cycle.
           * this solves the send of erroneous overload packages when the daq list
           * direction is changed for STIM to DAQ and the event channel overload
           * event was previously detected, when the direction was STIM */
          DaqListPtr->Flags |= (uint8)XCP_MASK_DAQLIST_OVCHECKSKIPPED;
        }
      }
#endif
#if (XCP_PRESCALER_SUPPORTED == XCP_PRESCALER_MASK)
      if((DaqListPtr->Mode & XCP_MASK_DAQLIST_DIRECTION) == XCP_MASK_DAQLIST_DIRECTION_DAQ)
      {
        if (OdtIndex == 0U)
        {
          /* Check whether prescaler for the DAQ List expired */
          if (DaqListPtr->PrescalerCnt == XCP_PRESCALER_MIN)
          {
            /* DAQ List is ready to be processed */
            RetVal = TRUE;
          }
          else
          {
            /* Decrement the prescaler count */
            DaqListPtr->PrescalerCnt--;
#if (XCP_OVERLOAD_MSB_SUPPORT == STD_ON)
            /* prevent checking for Event overload for one Event processing cycle
             * for the detected overload events when the daq list was not ready
             * because of the configured prescaler value. */
            DaqListPtr->Flags |= (uint8)XCP_MASK_DAQLIST_SKIPOVCHECK;
            DaqListPtr->Flags &= (uint8)(~XCP_MASK_DAQLIST_OVCHECKSKIPPED);
#endif
          }
        }
        else
        {
          /* DAQ List is ready to be processed */
          RetVal = TRUE;
        }
      }
      else
#endif
      {
        /* DAQ List is ready to be processed */
        RetVal = TRUE;
      }
    }
  }

  if (RetVal == TRUE)
  {
    /* DAQ List is ready to be sampled */
    DaqListPtr->Flags |= XCP_MASK_DAQLIST_READY;

#if (XCP_PRESCALER_SUPPORTED == XCP_PRESCALER_MASK)
    /* reset prescaler */
    DaqListPtr->PrescalerCnt = DaqListPtr->Prescaler;
#endif
  }
  else
  {
    /* DAQ List is not ready to be sampled */
    DaqListPtr->Flags &= (uint8)(~XCP_MASK_DAQLIST_READY);
  }

  DBG_XCP_ISDAQREADYTOBESAMPLED_EXIT(DaqListNumber,OdtIndex);

  return RetVal;
}

/*----------------------------[Xcp_IsDaqPrioQueueEmpty]----------------------------*/

FUNC(boolean, XCP_CODE) Xcp_IsDaqPrioQueueEmpty(void)
{
  boolean RetVal;
  DBG_XCP_ISDAQPRIOQUEUEEMPTY_ENTRY();
  RetVal = ((0U == Xcp_DaqPrioQueue.NoOfEntries)? TRUE:FALSE);
  DBG_XCP_ISDAQPRIOQUEUEEMPTY_EXIT(RetVal);
  return RetVal;
}

/*----------------------------[Xcp_GetDtoLength]----------------------------*/

FUNC(PduLengthType, XCP_CODE) Xcp_GetDtoLength(void)
{
  PduLengthType RetVal;
  /* Daq ID information */
  const Xcp_DaqIdType DaqID = XCP_GET_DAQ_ID_FROM_PRIO(0U);
  const uint8  OdtId = XCP_GET_ODT_ID_FROM_PRIO(0U);
  /* The DAQ list */
  CONSTP2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr =
      XCP_GET_DAQ_LIST_PTR(DaqID);

  DBG_XCP_GETDTOLENGTH_ENTRY();

  /* Get Size of Header for DTO stored in first position from DaqPrioQueue */
  RetVal = Xcp_GetDtoHeaderSize(DaqID, OdtId);
  /* Get DataSize of DTO stored in first position from DaqPrioQueue */
  RetVal += DaqListPtr->OdtList[OdtId].DTODataSize;

  DBG_XCP_GETDTOLENGTH_EXIT(RetVal);
  return RetVal;
}

/*-----------------------[Xcp_GetDtoData]------------------------------*/
FUNC(void, XCP_CODE) Xcp_GetDtoData
(
  PduLengthType DataLength,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) BufferPtr
)
{
  /* Iterator */
  PduLengthType DataIndex = 0U;
  /* Daq ID information */
  Xcp_DaqIdType DaqID;
  /* ODT ID information */
  uint8 OdtIndex;
  /* The DAQ list */
  P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr;
  /* Initialize the Data count */
  Xcp_MirrorSizeType DTOMirrorOfset;
  PduLengthType DataLen;

  DBG_XCP_GETDTODATA_ENTRY(DataLength, BufferPtr);

/* Check if Event Packet is Enabled */
    /* get daq list id */
    DaqID = XCP_GET_DAQ_ID_FROM_PRIO(0U);
    /* get daq list pointer */
    DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqID);
    /* get Odt id */
    OdtIndex = XCP_GET_ODT_ID_FROM_PRIO(0U);
    /* Is Pid ON */
    if ((DaqListPtr->Mode & XCP_MASK_DAQLIST_PID_MODE) == XCP_MASK_DAQLIST_PID_ON_MODE)
    {
      /* Write OdtIndex as the PID of the DTO in user buffer. */
      BufferPtr[DataIndex] = OdtIndex;
      DataIndex++;

    /* for absolute PID: PID = FIRST_PID + relative_ODT_NUMBER */
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_ABSOLUTE_IF_MASK)
      BufferPtr[(DataIndex - 1U)] += DaqListPtr->FirstPID;
#endif /* (XCP_IDENTIFICATION_FIELD_TYPE == XCP_ABSOLUTE_IF_MASK) */
#if (XCP_OVERLOAD_MSB_SUPPORT == STD_ON)
      if(Xcp_DaqPrioQueue.DaqRuntimeDataArray[0U].OverloadOdt == XCP_EV_DAQ_OVERLOAD_EV_MASK)
      {
        /* set PID most significant bit to 1 to indicate overload detection */
          BufferPtr[(DataIndex - 1U)] |= 0x80U;
      }
#endif /* (XCP_OVERLOAD_MSB_SUPPORT == STD_ON) */
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_RELATIVE_BYTE_IF_MASK)
      /* Is absolute DAQ list number (BYTE) configured, get the DAQ ID */
      BufferPtr[DataIndex] = (uint8) DaqID;
      DataIndex++;

#elif (XCP_IDENTIFICATION_FIELD_TYPE == XCP_RELATIVE_WORD_ALIGNED_IF_MASK)
      /* Is absolute DAQ list number (WORD, aligned) configured, leave a byte
       * for alignment and set it to the Xcp slave global configured value for fill bytes */
      BufferPtr[DataIndex] = XCP_RESERVED_BYTE;
      DataIndex++;
#endif

      /* Is absolute DAQ list number (WORD) or (WORD, aligned) configured,get the DAQ ID */
#if ((XCP_IDENTIFICATION_FIELD_TYPE == XCP_RELATIVE_WORD_ALIGNED_IF_MASK) || \
     (XCP_IDENTIFICATION_FIELD_TYPE == XCP_RELATIVE_WORD_IF_MASK))
      Xcp_WriteWordToByteArray(&BufferPtr[DataIndex], DaqID);
      DataIndex += XCP_SIZE_OF_WORD;
#endif
    }

#if (XCP_TIMESTAMP_SUPPORTED == XCP_TIMESTAMP_MASK)
    if (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Timestamp_Enabled == TRUE)
    {
      /* Is time stamp supported and this is the first Odt of the DAQ list */
      if ((((DaqListPtr->Mode & XCP_MASK_DAQLIST_TIMESTAMPED_MODE) ==
                                  XCP_MASK_DAQLIST_TIMESTAMPED_MODE)) &&
          (0U == OdtIndex))
      {
        /* Store the value of the time stamp depending on the configuration */
#if (XCP_TIMESTAMP_TYPE == XCP_ONE_BYTE_TS_MASK)
        BufferPtr[DataIndex] = Xcp_DaqRunTimeInfo[DaqID].TimeStamp;

#elif (XCP_TIMESTAMP_TYPE == XCP_TWO_BYTE_TS_MASK)
        Xcp_WriteWordToByteArray(&BufferPtr[DataIndex], Xcp_DaqRunTimeInfo[DaqID].TimeStamp);

#elif (XCP_TIMESTAMP_TYPE == XCP_FOUR_BYTE_TS_MASK)
        Xcp_WriteDwordToByteArray(&BufferPtr[DataIndex], Xcp_DaqRunTimeInfo[DaqID].TimeStamp);
#endif
        DataIndex += XCP_TIMESTAMP_TYPE;
      }
    }
#endif

    /* get Odt data start position from mirror */
    DTOMirrorOfset = DaqListPtr->OdtList[OdtIndex].MirrorOffsetPos;
    DataLen = DataLength - DataIndex;
    /* Copy ODT bytes from OdtMirrorDataArray in Buffer */
    TS_MemCpy( &BufferPtr[DataIndex],
               XCP_GET_MIRROR_PTR(DaqID, DTOMirrorOfset),
               DataLen);

  Xcp_ClearDTOEntryFromDaqPrioQueue();

  DBG_XCP_GETDTODATA_EXIT(BufferPtr);
}

FUNC(void, XCP_CODE) Xcp_ClearDTOEntryFromDaqPrioQueue(void)
{
#if (XCP_NO_DAQ_DTOS > 1)
  Xcp_DaqIdType DaqPrioQueueIndex;
  /* Clear DTO entry from Xcp_DaqPrioQueue */
  for(DaqPrioQueueIndex = 0U; DaqPrioQueueIndex < (Xcp_DaqPrioQueue.NoOfEntries - 1U);
      DaqPrioQueueIndex++)
  {
    /* Shift to left the DaqPrioQueue */
    Xcp_DaqPrioQueue.DaqRuntimeDataArray[DaqPrioQueueIndex] =
        Xcp_DaqPrioQueue.DaqRuntimeDataArray[DaqPrioQueueIndex + 1U];
  }
#endif

  /* Decrement number of entries from DaqPrioQueue*/
  Xcp_DaqPrioQueue.NoOfEntries--;
}

/*------------------[Xcp_UpdateEvent]--------------------------------*/
STATIC FUNC(void, XCP_CODE) Xcp_UpdateEvent
(
  uint16 EventID,
  Xcp_ReturnType EventProcessStatus
)
{
  /* Consistency of the event */

  DBG_XCP_UPDATEEVENT_ENTRY(EventID,EventProcessStatus);

  switch (XCP_GET_EVENT_CHANNEL_CONSISTENCY(EventID))
  {
    case XCP_EVENT_CHANNEL_CONSISTENCY_ODT:
      /* For ODT consistency */
      if(EventProcessStatus != XCP_NOT_OK)
      {
        /* go to the next Odt in case the DaqList is ready to be processed */
        Xcp_UpdateOdt(EventID, Xcp_Event[EventID].NextDaqIndex, Xcp_Event[EventID].NextOdtIndex);
      }
      break;
    case XCP_EVENT_CHANNEL_CONSISTENCY_DAQ:
      /* For DAQ consistency */
      if(EventProcessStatus != XCP_NOT_OK)
      {
        /* go to the next Odt in case the DaqList is ready to be processed */
        Xcp_UpdateDaq(EventID, Xcp_Event[EventID].NextDaqIndex);
      }
      break;
    default: /* XCP_EVENT_CHANNEL_CONSISTENCY_EVENT */
#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
      if (EventProcessStatus == XCP_STIM_TIMEOUT)
      {
        /* If STIM Timeout detected clear event set flag */
        XCP_CLEAR_BYTE_FLAG_ATOMIC(XCP_EVENT_IS_SET_FLAG_POS,
                            &(Xcp_Event[EventID].EventStatusFlag));
      }
      else
#endif
      {
        /* mark event as processed by clearing event status flags */
        Xcp_Event[EventID].EventStatusFlag = 0U;
      }
      break;
  }

  DBG_XCP_UPDATEEVENT_EXIT();
}

/*------------------[Xcp_UpdateDaq]-----------------------------------------*/
STATIC FUNC(void, XCP_CODE) Xcp_UpdateDaq
(
  uint16 EventID,
  uint8 EvDaqIndex
)
{

  DBG_XCP_UPDATEDAQ_ENTRY(EventID,EvDaqIndex);

  /* Increment DAQ Index. */
  EvDaqIndex++;

  /* Initialize the ODT index to zero for the currently processed event.
   * Rationale: when the DaqList index we start from the beginning */
  Xcp_Event[EventID].NextOdtIndex = 0U;

  /* If not all DAQ lists associated to the event are processed */
  if (EvDaqIndex < Xcp_Event[EventID].DaqIdListCount)
  {
    /* Update the current processed event with next DAQ index. */
    Xcp_Event[EventID].NextDaqIndex = EvDaqIndex;
  }
  else
  {
    /* reset start positions */
    Xcp_Event[EventID].NextDaqIndex = 0U;
    /* mark event as processed by clearing event status flags */
    Xcp_Event[EventID].EventStatusFlag = 0U;
  }

  DBG_XCP_UPDATEDAQ_EXIT(EventID,EvDaqIndex);
}

/*------------------[Xcp_UpdateOdt]-----------------------------------------*/
STATIC FUNC(void, XCP_CODE) Xcp_UpdateOdt
(
  uint16 EventID,
  uint8 EvDaqIndex,
  uint8 OdtIndex
)
{
  /* Get the DAQ list based on the DAQ list number found in the current Event */
  CONSTP2CONST(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr =
      XCP_GET_DAQ_LIST_PTR(Xcp_Event[EventID].DaqIdListPtr[EvDaqIndex]);

  DBG_XCP_UPDATEODT_ENTRY(EventID,EvDaqIndex,OdtIndex);

  /* Increment ODT. */
  OdtIndex++;

  /* If not all ODTs in the DAQ list are covered */
  if (OdtIndex < DaqListPtr->MaxOdt)
  {
    /* Initialize the ODT index to the new Odt Index*/
    Xcp_Event[EventID].NextOdtIndex = OdtIndex;
  }
  else
  {
    /* Update the Daq information of the currently processed event */
    Xcp_UpdateDaq(EventID, EvDaqIndex);
  }

  DBG_XCP_UPDATEODT_EXIT(EventID,EvDaqIndex,OdtIndex);
}

/*------------------[Xcp_SampleData]----------------------------------------*/
STATIC FUNC(void, XCP_CODE) Xcp_SampleData
(
  uint16 EventID
)
{
  Xcp_DaqIdType DaqId = Xcp_Event[EventID].DaqIdListPtr[Xcp_Event[EventID].NextDaqIndex];
  CONSTP2CONST(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr =
      XCP_GET_DAQ_LIST_PTR(DaqId);

  DBG_XCP_SAMPLEDATA_ENTRY(EventID);

  switch (XCP_GET_EVENT_CHANNEL_CONSISTENCY(EventID))
  {
    case XCP_EVENT_CHANNEL_CONSISTENCY_ODT:
      /* For Odt consistency */
      if((DaqListPtr->Mode & XCP_MASK_DAQLIST_DIRECTION) == XCP_MASK_DAQLIST_DIRECTION_DAQ)
      {
        Xcp_SampleDtoData(DaqId, Xcp_Event[EventID].NextOdtIndex);
      }
      break;
    case XCP_EVENT_CHANNEL_CONSISTENCY_DAQ:
      /* For DAQ consistency */
      if ((DaqListPtr->Mode & XCP_MASK_DAQLIST_DIRECTION) == XCP_MASK_DAQLIST_DIRECTION_DAQ)
      {
        Xcp_SampleDaq(DaqId);
      }
      break;
    default:
      /* For Event consistency */
      Xcp_SampleEvent(EventID);
      break;
  }

  DBG_XCP_SAMPLEDATA_EXIT();
}

/*------------------[Xcp_SampleDtoData]-------------------------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_SampleDtoData
(
  Xcp_DaqIdType DaqId,
  uint8 OdtIndex
)
{
  /* Initialize the Odt Entry count to zero */
  uint8 OdtEntryCount;
  /* The length of the odt entry */
  uint8 OdtEntryLength;
  /* Initialise the length for data stored in mirror starting from DataCount*/
  Xcp_OdtSizeType DataLength = 0U;
  /* The DAQ list to be sampled */
  P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqId);
  /* Initialize the Data count */
  Xcp_MirrorSizeType DTOMirrorWritePos = DaqListPtr->OdtList[OdtIndex].MirrorOffsetPos;
  /* Entry pointer */
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
  P2VAR(Xcp_OdtEntryType, AUTOMATIC, XCP_CONST) OdtEntryPtr = DaqListPtr->OdtList[OdtIndex].OdtEntry;
#else
  P2VAR(usize, AUTOMATIC, XCP_CONST) OdtEntryPtr = DaqListPtr->OdtList[OdtIndex].OdtEntry;
#endif
#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
  const Xcp_MemoryAreaType* MemoryArea;
  uint8 MemoryAreaIdx = 0U;
#endif

#if (XCP_TIMESTAMP_SUPPORTED == XCP_TIMESTAMP_MASK)
  Xcp_TimestampType TimeStamp; /* The Timestamp value */
#if (XCP_USER_TIMESTAMP_SUPPORTED == STD_OFF)
    /* OS Timestamp */
    TickType CounterValue; /* The counter value */
#endif /* (XCP_USER_TIMESTAMP_SUPPORTED == STD_OFF) */
#endif /* (XCP_TIMESTAMP_SUPPORTED == XCP_TIMESTAMP_MASK) */

/* !LINKSTO Xcp.ProtectedMemoryLocations.General,1 */
#if (XCP_MEMORY_AREAS_ACCESS_SUPPORTED == STD_ON)
    boolean MemoryIsAccessible = TRUE;
#endif

  DBG_XCP_SAMPLEDTODATA_ENTRY(DaqId,OdtIndex);
#if (XCP_TIMESTAMP_SUPPORTED == XCP_TIMESTAMP_MASK)
  if (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Timestamp_Enabled == TRUE)
  {
    /* Is time stamp supported and this is the first Odt of the DAQ list */
    if ((((DaqListPtr->Mode & XCP_MASK_DAQLIST_TIMESTAMPED_MODE) ==
          XCP_MASK_DAQLIST_TIMESTAMPED_MODE)) &&
        (0U == OdtIndex))
    {
#if (XCP_USER_TIMESTAMP_SUPPORTED == STD_OFF)
      /* Get the counter value */
      (void) GetCounterValue(XCP_OS_COUNTER, &CounterValue);
      TimeStamp = (Xcp_TimestampType)CounterValue;
#else
      /* User Timestamp - Use the user provided function to get timestamps */
      TimeStamp = Xcp_ApplGetTimestamp();
#endif /* (XCP_USER_TIMESTAMP_SUPPORTED == STD_OFF) */
      /* Set timestamp */
      TS_AtomicAssignGeneric(Xcp_DaqRunTimeInfo[DaqId].TimeStamp, TimeStamp);
    }
  }
#endif

  /* refresh DTO data into DTO mirror */
#if (XCP_MEMORY_AREAS_ACCESS_SUPPORTED == STD_ON)
  for(OdtEntryCount = 0U;
      ((OdtEntryCount < DaqListPtr->OdtList[OdtIndex].NrOfOdtEntries) &&
       (MemoryIsAccessible == TRUE));
      OdtEntryCount++)
#else
  for(OdtEntryCount = 0U;
     (OdtEntryCount < DaqListPtr->OdtList[OdtIndex].NrOfOdtEntries);
      OdtEntryCount++)
#endif /* XCP_MEMORY_AREAS_ACCESS_SUPPORTED == STD_ON */
  {
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
    OdtEntryLength = (uint8)(OdtEntryPtr[OdtEntryCount].Length * XCP_GRANULARITY_ODT_ENTRY_SIZE_DAQ);
#else
    OdtEntryLength = (uint8)(*(uint8*)Xcp_GetOdtEntryData(OdtEntryPtr,OdtEntryCount,XCP_ODT_ENTRY_FIELD_LENGTH) * XCP_GRANULARITY_ODT_ENTRY_SIZE_DAQ);
#endif

    if (OdtEntryLength != 0U)
    {
#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
      MemoryArea = Xcp_FindMemoryArea(XCP_APPL_GET_ADDRESS( 0U, OdtEntryPtr[OdtEntryCount].Address ), OdtEntryLength, XCP_MEMORY_AREA_TYPE_READ, XCP_MEMORY_AREA_SCOPE_DAQ_STIM, &MemoryAreaIdx);
#else
      MemoryArea = Xcp_FindMemoryArea(XCP_APPL_GET_ADDRESS( 0U, (uint8*)(*((uint32*)Xcp_GetOdtEntryData(OdtEntryPtr,OdtEntryCount,XCP_ODT_ENTRY_FIELD_ADDRESS))) ), OdtEntryLength, XCP_MEMORY_AREA_TYPE_READ, XCP_MEMORY_AREA_SCOPE_DAQ_STIM, &MemoryAreaIdx);
#endif
      if (MemoryArea == NULL_PTR)
      {
#if (XCP_PROD_ERR_HANDLING_ILLEGAL_MEMORY_ACCESS != TS_PROD_ERR_DISABLE)
        /* !LINKSTO Xcp.BswDistribution.EventProcessing.NoMemoryAccessAreaFound,1 */
        /* report error */
        XCP_REPORT_ILLEGAL_MEMORY_ACCESS(XCP_SID_EVENT_MAIN_FUNCTION);
#endif
        MemoryIsAccessible = FALSE;
      }
      else
      {
        uint8* odtEntryAddress;
        /* unused for Daq */
        uint8 DummyBitMask = 0U;
#if ((XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF) || \
    ((XCP_ENABLE_DAQ_OPTIMIZATION == STD_ON) && (XCP_ENABLE_BIT_OFFSET == STD_ON)))
        uint8 bitOffset;
#endif
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
        odtEntryAddress = OdtEntryPtr[OdtEntryCount].Address;
#else
        odtEntryAddress = (uint8*)(*((uint32*)Xcp_GetOdtEntryData(OdtEntryPtr,OdtEntryCount,XCP_ODT_ENTRY_FIELD_ADDRESS)));
#endif
        SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
#if ((XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF) || \
    ((XCP_ENABLE_DAQ_OPTIMIZATION == STD_ON) && (XCP_ENABLE_BIT_OFFSET == STD_ON)))
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_ON)
        bitOffset = *(uint8*)Xcp_GetOdtEntryData(OdtEntryPtr,OdtEntryCount,XCP_ODT_ENTRY_FIELD_BITOFFSET);
#else
        bitOffset = OdtEntryPtr[OdtEntryCount].BitOffset;
#endif
        /* !LINKSTO Xcp.ASAM.BITWISE_STIMULATION.006,1 */
        if (bitOffset != XCP_BIT_OFFSET_IGNORE_VALUE)
        {
          uint8 Type;
          /* !LINKSTO Xcp.ASAM.BITWISE_STIMULATION.GENERAL002,1 */
          uint8 bitMask = 1U << bitOffset;

          /* Return TRUE when bit is set */
          if (((*(uint8 *)XCP_APPL_GET_ADDRESS( 0U, odtEntryAddress)) & bitMask) > 0 )
          {
            /* !LINKSTO Xcp.ASAM.BITWISE_STIMULATION.002,1 */
            Type = XCP_MEMORYPROXYDATA_SAMPLE_1;
          }
          else
          {
            /* !LINKSTO Xcp.ASAM.BITWISE_STIMULATION.003,1 */
            Type = XCP_MEMORYPROXYDATA_SAMPLE_0;
          }
          Xcp_MemoryProxyEnqueueRequest(DaqId, OdtEntryLength, DTOMirrorWritePos, MemoryArea->ApplicationId, Type, XCP_APPL_GET_ADDRESS( 0U, OdtEntryPtr[OdtEntryCount].Address), DummyBitMask);
        }
        else
#endif
        {
          Xcp_MemoryProxyEnqueueRequest(DaqId, OdtEntryLength, DTOMirrorWritePos, MemoryArea->ApplicationId, XCP_MEMORYPROXYDATA_SAMPLE, XCP_APPL_GET_ADDRESS( 0U, OdtEntryPtr[OdtEntryCount].Address), DummyBitMask);
        }
        SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

        DTOMirrorWritePos += OdtEntryLength;         /* Increment the data count by length */
        DataLength += OdtEntryLength;                /* Increment the data Length by length */
      }
#endif /* (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON) */
#if ((XCP_MEMORY_AREAS_ACCESS_SUPPORTED == STD_ON) && (XCP_MEMORY_ACCESS_AREAS == STD_OFF) && (XCP_BSW_DISTRIBUTION_ENABLED == STD_OFF))
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
      if (Xcp_IsMemoryAreaAccessible(
           OdtEntryPtr[OdtEntryCount].Address,
           OdtEntryLength,
           XCP_MEMORY_AREA_TYPE_READ,
           XCP_MEMORY_AREA_SCOPE_DAQ_STIM) == FALSE
         )
#else
      if (Xcp_IsMemoryAreaAccessible(
           (uint8*)*(uint32*)Xcp_GetOdtEntryData(OdtEntryPtr,OdtEntryCount,XCP_ODT_ENTRY_FIELD_ADDRESS),
           OdtEntryLength,
           XCP_MEMORY_AREA_TYPE_READ,
           XCP_MEMORY_AREA_SCOPE_DAQ_STIM) == FALSE
         )
#endif
      {
#if (XCP_PROD_ERR_HANDLING_ILLEGAL_MEMORY_ACCESS != TS_PROD_ERR_DISABLE)
        /* !LINKSTO Xcp.ProtectedMemoryLocations.NegativeResponse.DAQ,1 */
        /* report error */
        XCP_REPORT_ILLEGAL_MEMORY_ACCESS(XCP_SID_EVENT_MAIN_FUNCTION);
#endif
        MemoryIsAccessible = FALSE;
      }
      else
#endif /* XCP_MEMORY_AREAS_ACCESS_SUPPORTED == STD_ON */
#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_OFF)
      {
        /* Store the Address information into the DTO Entry */

        /* protect data sampling from concurrent access */
        SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
        /* !LINKSTO Xcp.ASAM.BITWISE_STIMULATION.006,1 */
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
        if (OdtEntryPtr[OdtEntryCount].BitOffset != XCP_BIT_OFFSET_IGNORE_VALUE)
#elif (XCP_ENABLE_BIT_OFFSET == STD_ON)
        if (*(uint8*)Xcp_GetOdtEntryData(OdtEntryPtr,OdtEntryCount,XCP_ODT_ENTRY_FIELD_BITOFFSET) != XCP_BIT_OFFSET_IGNORE_VALUE)
#endif
#if ((XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF) || \
    ((XCP_ENABLE_DAQ_OPTIMIZATION == STD_ON) && (XCP_ENABLE_BIT_OFFSET == STD_ON)))
        {
          /* !LINKSTO Xcp.ASAM.BITWISE_STIMULATION.GENERAL002,1 */

#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
          uint8 bitMask = 1U << OdtEntryPtr[OdtEntryCount].BitOffset;
#else
          uint8 bitMask = 1U << *(uint8*)Xcp_GetOdtEntryData(OdtEntryPtr,OdtEntryCount,XCP_ODT_ENTRY_FIELD_BITOFFSET);
#endif

          /* Return TRUE when bit is set */
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
          if (((*(uint8 *)XCP_APPL_GET_ADDRESS( 0U, OdtEntryPtr[OdtEntryCount].Address)) &  bitMask) > 0 )
#else
          if (((*(uint8*)XCP_APPL_GET_ADDRESS( 0U, (uint8*)(*((uint32*)Xcp_GetOdtEntryData(OdtEntryPtr,OdtEntryCount,XCP_ODT_ENTRY_FIELD_ADDRESS))))) &  bitMask) > 0 )
#endif
          {
            /* !LINKSTO Xcp.ASAM.BITWISE_STIMULATION.002,1 */
           *(uint8 *)XCP_GET_MIRROR_PTR(DaqId, DTOMirrorWritePos)= 0x01;
          }
          else
          {
            /* !LINKSTO Xcp.ASAM.BITWISE_STIMULATION.003,1 */
           *(uint8 *)XCP_GET_MIRROR_PTR(DaqId, DTOMirrorWritePos) = 0x00;
          }
        }
        else
#endif
        {
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
          TS_MemCpy(XCP_GET_MIRROR_PTR(DaqId, DTOMirrorWritePos),
                  XCP_APPL_GET_ADDRESS( 0U, OdtEntryPtr[OdtEntryCount].Address ),
                  OdtEntryLength);
#else
          TS_MemCpy(XCP_GET_MIRROR_PTR(DaqId, DTOMirrorWritePos),
                  XCP_APPL_GET_ADDRESS( 0U, ((uint8*)*((uint32*)Xcp_GetOdtEntryData(OdtEntryPtr,OdtEntryCount,XCP_ODT_ENTRY_FIELD_ADDRESS)))),
                  OdtEntryLength);
#endif
        }
        SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

        DTOMirrorWritePos += OdtEntryLength;         /* Increment the data count by length */
        DataLength += OdtEntryLength;                /* Increment the data Length by length */
      }
#endif /* XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_OFF */
    }
    else
    {
      /* We need to stop as soon as we find an ODT entry with size 0 - ASAM requirement */
      break;
    }
  }

#if (XCP_MEMORY_AREAS_ACCESS_SUPPORTED == STD_ON)
  if (MemoryIsAccessible == TRUE)
#endif
  {
#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_OFF)
    Xcp_UpdateDaqPrioQueue(DaqId, OdtIndex, DataLength);
#else
    Xcp_DaqPrioQueueOfRequest.Queue[Xcp_DaqPrioQueueOfRequest.WriteIdx].OdtIndex = OdtIndex;
    Xcp_DaqPrioQueueOfRequest.Queue[Xcp_DaqPrioQueueOfRequest.WriteIdx].DataLength = DataLength;
    Xcp_DaqPrioQueueOfRequest.Queue[Xcp_DaqPrioQueueOfRequest.WriteIdx].DaqId = DaqId;
    Xcp_DaqPrioQueueOfRequest.NoOfEntries[Xcp_ProcessedEventId]++;
    Xcp_DaqPrioQueueOfRequest.WriteIdx++;
    if (Xcp_DaqPrioQueueOfRequest.WriteIdx == XCP_MAX_SIZE_OF_DAQ_PRIO_LIST)
    {
      Xcp_DaqPrioQueueOfRequest.WriteIdx = 0U;
    }
#endif
  }

  DBG_XCP_SAMPLEDTODATA_EXIT();
}

/*------------------[Xcp_SampleDaq]-----------------------------------------*/
STATIC FUNC(void, XCP_CODE) Xcp_SampleDaq
(
  Xcp_DaqIdType DaqId
)
{
  /* Variable to loop through each ODT */
  uint8 OdtIndex;
  /* The DAQ list to be sampled */
  CONSTP2CONST(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr =
      XCP_GET_DAQ_LIST_PTR(DaqId);

  DBG_XCP_SAMPLEDAQ_ENTRY(DaqId);

  /* Are all the ODTs covered */
  for (OdtIndex = 0U; OdtIndex < DaqListPtr->MaxOdt; OdtIndex++)
  {
    /* Sample the Data */
    Xcp_SampleDtoData(DaqId, OdtIndex);
  }

  DBG_XCP_SAMPLEDAQ_EXIT();
}

/*------------------[Xcp_SampleEvent]---------------------------------------*/
STATIC FUNC(void, XCP_CODE) Xcp_SampleEvent
(
  uint16 EventID
)
{
  uint8 DaqIdListCount; /* The number of DAQ IDs of this event */
  uint8 EvDaqIndex; /* Variable to loop through each Daq */
  Xcp_DaqIdType DaqId;
  /* DAQ list associated to this event */
  P2CONST(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr;

  DBG_XCP_SAMPLEEVENT_ENTRY(EventID);

  /* Get the number of DAQ lists associated to this event */
  DaqIdListCount = Xcp_Event[EventID].DaqIdListCount;

  /* For all DAQ lists associated with this event, sample them */
  for (EvDaqIndex = 0U; EvDaqIndex < DaqIdListCount; EvDaqIndex++)
  {
    /* get daqlist Id */
    DaqId = Xcp_Event[EventID].DaqIdListPtr[EvDaqIndex];
    /* Get a DAQ list copy */
    DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqId);

    /* If DAQ List can be sampled */
    if (((DaqListPtr->Mode & XCP_MASK_DAQLIST_DIRECTION) == XCP_MASK_DAQLIST_DIRECTION_DAQ) &&
        ((DaqListPtr->Flags & XCP_MASK_DAQLIST_READY) == XCP_MASK_DAQLIST_READY))
    {
      /* Sample all the ODTs in the DAQ list */
      Xcp_SampleDaq(DaqId);
    }
  }

  DBG_XCP_SAMPLEEVENT_EXIT();
}

/*------------------[Xcp_GetEventProcReadyStatus]----------------------------------------*/
STATIC FUNC(Xcp_ReturnType, XCP_CODE)  Xcp_GetEventProcReadyStatus
(
  uint16 EventID
)
{
  Xcp_ReturnType RetVal;

  DBG_XCP_GETNEVENTPROCREADYSTATUS_ENTRY(EventID);

  /* get event global status */
  if (XCP_GET_EVENT_CHANNEL_CONSISTENCY(EventID) == XCP_EVENT_CHANNEL_CONSISTENCY_EVENT)
  {
    RetVal = Xcp_GetEventProcStatus(EventID);
  }
  else
  {
    RetVal = Xcp_GetNextDaqListProcStatus(EventID);
  }

  DBG_XCP_GETNEVENTPROCREADYSTATUS_EXIT(RetVal);

  return RetVal;
}

/*------------------[Xcp_GetEventProcStatus]----------------------------------------*/

STATIC FUNC(Xcp_ReturnType, XCP_CODE)  Xcp_GetEventProcStatus(uint16 EventId)
{
  /* Holds the status of DAQ from Event. Initialize with "nothing to process" indicator */
  Xcp_ReturnType RetVal = XCP_NOT_OK;
#if ((XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) && \
     (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK))
  /* flag used to indicate if at least one daq list is ready to be sampled */
  boolean IsAcqEventDetected = FALSE;
#endif
  /* Holds the index of Daq */
  uint8 EvDaqIndex = 0U;
  /* Local variable to hold DAQ List number */
  Xcp_DaqIdType DaqID;
  /* Points to the DAQ list corresponding to the event */
  P2VAR(Xcp_DaqIdType, AUTOMATIC, XCP_VAR) EvDaqListPtr = Xcp_Event[EventId].DaqIdListPtr;
#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
  /* The DAQ list */
  P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr;
#endif

  DBG_XCP_GETEVENTPROCSTATUS_ENTRY(EventId);

  /* Loop through all the DAQ lists of the event to check if they are ready to be sampled */
  while(EvDaqIndex < Xcp_Event[EventId].DaqIdListCount)
  {
    /* get daqlist number */
    DaqID = EvDaqListPtr[EvDaqIndex];
    /* Identify whether DAQ is ready to be sampled.
       For event consistency the whole DAQ will be considered and
       thus the parameter OdtIndex is set to 0(zero)*/
    if(Xcp_IsDAQReadyToBeProcessed(DaqID, 0U) == TRUE)
    {
#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
      /* get daqlist pointer */
      DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqID);

      if((DaqListPtr->Mode & XCP_MASK_DAQLIST_DIRECTION) == XCP_MASK_DAQLIST_DIRECTION_STIM)
      {
        if(Xcp_IsStimProcReady(EventId, DaqID) == FALSE)
        {
          /* event is not ready to be processed due to STIM_TIMEOUT detected error */
          RetVal = XCP_STIM_TIMEOUT;
        }
        else
        {
          if(RetVal == XCP_NOT_OK)
          {
            /* the event is ready for stimulate */
            RetVal = XCP_OK;
          }
        }
      }
      else
#endif /* (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK) */
      {
#if (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)
#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
        /* at least one daqlist is ready to be sampled */
        IsAcqEventDetected = TRUE;
        /* check if we detected stim timeout error */
        if(RetVal == XCP_NOT_OK)
#endif /* (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK) */
        {
          /* the event is ready for sampling */
          RetVal = XCP_OK;
        }
#endif /* (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) */
      }
    }
    EvDaqIndex++;
  }

#if ((XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK) && \
     ((XCP_EVENT_PACKET_ENABLED == STD_ON) || \
      (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)))
  if(RetVal == XCP_STIM_TIMEOUT)
  {
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
    /* Emit an Event CTO with Event channel number as info */
    Xcp_EmitSTIMTimeoutEvent(XCP_EV_STIM_TIMEOUT_INFO_EVENT, EventId);
#endif /* (XCP_EVENT_PACKET_ENABLED == STD_ON) */
#if (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)
    if(IsAcqEventDetected == TRUE)
    {
      /* When the consistency is Event, we have to consistently do
       * both acquisition and stimulation, the result of acquisition
       * depending on the result of stimulation. This is the only situation
       * when we have this dependency between the two different processes
       * and cases when the acquisition shall not to be processed. */
      /* Set event overload flag */
      XCP_SET_BYTE_FLAG_ATOMIC(XCP_EVENT_OVELOAD_FLAG_POS,
                        &(Xcp_Event[EventId].EventStatusFlag));
    }
#endif /* (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) */
  }
#endif /* ((XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK) && \
        *  ((XCP_EVENT_PACKET_ENABLED == STD_ON) || \
        *   ((XCP_OVERLOAD_MSB_SUPPORT == STD_ON) || \
        *    (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)))) */

  DBG_XCP_GETEVENTPROCSTATUS_EXIT(RetVal);

  return RetVal;
}

/*------------------[Xcp_GetNextDaqListProcStatus]----------------------------------------*/

STATIC FUNC(Xcp_ReturnType, XCP_CODE)  Xcp_GetNextDaqListProcStatus(uint16 EventId)
{
  /* Holds the status of DAQ from Event */
  Xcp_ReturnType RetVal = XCP_NOT_OK;
  uint8 EvDaqIndex = Xcp_Event[EventId].NextDaqIndex;
  uint8 EvOdtIndex;
  /* Local variable to hold DAQ List number */
  Xcp_DaqIdType DaqID;
#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
  P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr;
#endif
  /* Points to the DAQ list corresponding to the event */
  P2VAR(Xcp_DaqIdType, AUTOMATIC, XCP_VAR) EvDaqListPtr =
      Xcp_Event[EventId].DaqIdListPtr;

  DBG_XCP_GETNEXTDAQLISTPROCSTATUS_ENTRY(EventId);

  while((EvDaqIndex < Xcp_Event[EventId].DaqIdListCount) && (RetVal == XCP_NOT_OK))
  {
    EvOdtIndex = Xcp_Event[EventId].NextOdtIndex;
    DaqID = EvDaqListPtr[EvDaqIndex];

    if(Xcp_IsDAQReadyToBeProcessed(DaqID, EvOdtIndex) == TRUE)
    {
#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
      /* get daq list pointer */
      DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqID);

      if((DaqListPtr->Mode & XCP_MASK_DAQLIST_DIRECTION) == XCP_MASK_DAQLIST_DIRECTION_STIM)
      {
        if(Xcp_IsStimProcReady(EventId, DaqID) == TRUE)
        {
          /* daq list is ready for stimulation */
          RetVal = XCP_OK;
        }
        else
        {
          /* daq list is not ready for stimulation. The STIM_TIMEOUT error code is detected */
          RetVal = XCP_STIM_TIMEOUT;
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)
          {
            P2VAR(Xcp_OdtType, AUTOMATIC, XCP_VAR) OdtPtr = &(DaqListPtr->OdtList[EvOdtIndex]);
            uint8 OdtEntryId;
            for (OdtEntryId = 0U; OdtEntryId < OdtPtr->NrOfOdtEntries; OdtEntryId++)
            {
              /* !LINKSTO Xcp.ODT.Processing.ODTSize0.STIM.Event,1 */
              /* Emit Timeout event only if there is at least one valid ODT entry in ODT */
#if (XCP_ENABLE_DAQ_OPTIMIZATION == STD_OFF)
              if (OdtPtr->OdtEntry[OdtEntryId].Length > 0U)
#else
              if (*(uint8*)Xcp_GetOdtEntryData(OdtPtr->OdtEntry,OdtEntryId,XCP_ODT_ENTRY_FIELD_LENGTH) > 0U)
#endif
              {
                /* Emit an Event CTO with Event channel number as info */
                Xcp_EmitSTIMTimeoutEvent(XCP_EV_STIM_TIMEOUT_INFO_DAQLIST, DaqID);
#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
                Xcp_ProcessTransmit(XCP_SID_MAIN_FUNCTION);
#endif
                break;
              }
            }
          }
#endif /* (XCP_EVENT_PACKET_ENABLED == STD_ON) */
        }
      }
      else
#endif /* (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK) */
      {
#if (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK)
        /* daq list is ready for stimulation */
        RetVal = E_OK;
#endif /* (XCP_RESOURCE_DAQ == XCP_RESOURCE_DAQ_MASK) */
      }
      /* we found the next daq to process and the status of the selected daq */
      break;
    }
    else
    {
      /* go to the next DaqList, if the end of list is not reached */
      Xcp_UpdateDaq(EventId, EvDaqIndex);
    }
    /* go to the next index */
    EvDaqIndex++;
  }

  DBG_XCP_GETNEXTDAQLISTPROCSTATUS_EXIT(RetVal);

  return RetVal;
}

/*------------------[Xcp_GetDtoHeaderSize]----------------------------------------*/

STATIC FUNC(uint16, XCP_CODE)  Xcp_GetDtoHeaderSize(Xcp_DaqIdType DaqListId, uint8 OdtIndex)
{
  /* Holds the calculated total memory */
  uint16 TotalMemory = 0;
  /* The DAQ list to be sampled */
  CONSTP2CONST(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqListId);

  DBG_XCP_GETDTOHEADERSIZE_ENTRY(DaqListId,OdtIndex);

  /* Is Pid enabled */
  if ((DaqListPtr->Mode & XCP_MASK_DAQLIST_PID_MODE) == XCP_MASK_DAQLIST_PID_ON_MODE)
  {
    /* Is absolute PID configured: PID = FIRST_PID + relative_ODT_NUMBER */
#if (XCP_IDENTIFICATION_FIELD_TYPE == XCP_ABSOLUTE_IF_MASK)

    /* Increment the memory size by 1, since identifier field consists of 1 byte PID */
    TotalMemory++;

    /* Is absolute DAQ list number (BYTE) configured */
#elif (XCP_IDENTIFICATION_FIELD_TYPE == XCP_RELATIVE_BYTE_IF_MASK)

    /* Increment the memory size by 2, since identifier field consists of 1 byte PID and
       1 byte DAQ List index */
    TotalMemory += XCP_SIZE_OF_WORD;

    /* Is absolute DAQ list number (WORD) configured */
#elif (XCP_IDENTIFICATION_FIELD_TYPE == XCP_RELATIVE_WORD_IF_MASK)

    /* Increment the memory size by 3, since identifier field consists of 1 byte PID and
       2 byte DAQ List index */
    TotalMemory += XCP_SIZE_OF_PID_RELATIVE_WORD;

    /* Is absolute DAQ list number (WORD, aligned) configured */
#elif (XCP_IDENTIFICATION_FIELD_TYPE == XCP_RELATIVE_WORD_ALIGNED_IF_MASK)

    /* Increment the memory size by 4, since identifier field consists of 1 byte PID,
       1 byte alignment and 2 byte DAQ List index */
    TotalMemory += XCP_SIZE_OF_DWORD;

#endif
  }

  /* Is time stamp enabled  */
#if (XCP_TIMESTAMP_SUPPORTED == XCP_TIMESTAMP_MASK)
  if (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Xcp_Timestamp_Enabled == TRUE)
  {
    /* If time stamp is enabled for the DAQ list and
       the first ODT of the DAQ list is being processed */
    if ((((DaqListPtr->Mode & XCP_MASK_DAQLIST_TIMESTAMPED_MODE) == XCP_MASK_DAQLIST_TIMESTAMPED_MODE)) &&
        (OdtIndex == 0U))
    {
      /* Update the required total memory depending up on configuration */
      TotalMemory += XCP_TIMESTAMP_TYPE;
    }
    else
    {
      TS_PARAM_UNUSED(OdtIndex);
    }
  }
#else
  TS_PARAM_UNUSED(OdtIndex);
#endif

  DBG_XCP_GETDTOHEADERSIZE_EXIT(TotalMemory);

  return TotalMemory;
}

#if((XCP_EVENT_PACKET_ENABLED == STD_ON) && \
    (XCP_OVERLOAD_MSB_SUPPORT == STD_OFF))

/*--------------------------[Xcp_EmitDAQOverloadEvent]-----------------------------*/

FUNC(void, XCP_CODE) Xcp_EmitDAQOverloadEvent(void)
{
  Xcp_EventCTOType EventCTO;

  DBG_XCP_EMITDAQOVERLOADEVENT_ENTRY();

  /* prepare event response CTO for DAQ Overload */
  EventCTO.EVENT_CTO[0U] = XCP_EV_PID;
  EventCTO.EVENT_CTO[1U] = XCP_EV_DAQ_OVERLOAD;
  EventCTO.Length = 2U;

  /* enqueue event package into event CTo queue */
  Xcp_EnqueueCtoEvent(&EventCTO);

  DBG_XCP_EMITDAQOVERLOADEVENT_EXIT();
}
#endif

#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
#if (XCP_EVENT_PACKET_ENABLED == STD_ON)

/*--------------------------[Xcp_EmitSTIMTimeoutEvent]-----------------------------*/

STATIC FUNC(void, XCP_CODE)  Xcp_EmitSTIMTimeoutEvent
(
  uint8 EventInfoType,
  uint16 EventInfo
)
{
  Xcp_EventCTOType EventCTO;

  DBG_XCP_EMITEVENTSTIMTIMEOUT_ENTRY(EventInfoType,EventInfo);

  /* prepare event response CTO for STIM Timeout */
  EventCTO.EVENT_CTO[0U] = XCP_EV_PID;
  EventCTO.EVENT_CTO[1U] = XCP_EV_STIM_TIMEOUT;
  EventCTO.EVENT_CTO[2U] = EventInfoType; /* Info type: Event */
  EventCTO.EVENT_CTO[3U] = XCP_RESERVED_BYTE;
  Xcp_WriteWordToByteArray(&(EventCTO.EVENT_CTO[4U]), EventInfo);
  EventCTO.Length = 0x06U;

  /* Protect concurrent access to Event CTO Queue */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  /* enqueue event package into event CTo queue */
  Xcp_EnqueueCtoEvent(&EventCTO);

  /* exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  DBG_XCP_EMITEVENTSTIMTIMEOUT_EXIT();
}
#endif /* (XCP_EVENT_PACKET_ENABLED == STD_ON) */

/*--------------------------[Xcp_IsStimProcReady]-----------------------------*/

STATIC FUNC(boolean, XCP_CODE) Xcp_IsStimProcReady
(
  uint16 EventId,
  Xcp_DaqIdType DaqListId
)
{
  boolean RetVal = FALSE;
  /* The DAQ list */
  P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqListId);
  uint8 OdtId;

  DBG_XCP_ISSTIMPROCREADY_ENTRY(EventId,DaqListId);

  if(XCP_GET_EVENT_CHANNEL_CONSISTENCY(EventId) == XCP_EVENT_CHANNEL_CONSISTENCY_ODT)
  {
    OdtId = Xcp_Event[EventId].NextOdtIndex;
    RetVal = XCP_IS_BYTE_FLAG_SET(OdtId, (Xcp_DaqRunTimeInfo[DaqListId].IsNewOdtDataMask));
  }
  else
  {
    for(OdtId = 0U; OdtId < DaqListPtr->MaxOdt; OdtId++)
    {
      RetVal = XCP_IS_BYTE_FLAG_SET(OdtId, (Xcp_DaqRunTimeInfo[DaqListId].IsNewOdtDataMask));
      if(RetVal == FALSE)
      {
        /* we detected an error. there is no need to go further */
        break;
      }
    }
  }

  DBG_XCP_ISSTIMPROCREADY_EXIT(RetVal);

  return RetVal;
}

#endif /* (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK) */

#if (XCP_DAQ_COUNT > 0U)
FUNC_P2VAR(uint8, XCP_VAR_CLEARED, XCP_CODE) Xcp_GetMirrorPtr
(
  Xcp_DaqIdType DaqID,
  Xcp_MirrorSizeType MirrorOffsetPosition
)
{
  /* The queried pointer to DTO Data Mirror */
  P2VAR(uint8, AUTOMATIC, XCP_VAR_CLEARED) RetValue;

  DBG_XCP_GETMIRRORTPTR_ENTRY(DaqID, MirrorOffsetPosition);

#if (XCP_MIN_DAQ > 0)
  RetValue = (DaqID<XCP_MIN_DAQ)?
    &(Xcp_DTOEntryMirror.NonDynamicOdtMirrorDataArray[MirrorOffsetPosition]):
    &(Xcp_DTOEntryMirror.DynamicOdtMirrorDataArray[MirrorOffsetPosition]);
#else
  RetValue = &(Xcp_DTOEntryMirror.DynamicOdtMirrorDataArray[MirrorOffsetPosition]);
  TS_PARAM_UNUSED(DaqID);
#endif

  DBG_XCP_GETMIRRORTPTR_EXIT(RetValue);

  return RetValue;
}

#endif /* (XCP_DAQ_COUNT > 0U) */


#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
FUNC(void, XCP_CODE) Xcp_ResetRequestData(void)
{
  uint16 EvIdx;

  Xcp_RequestQueue.ReadIndex = 0U;
  Xcp_RequestQueue.WriteIdx = 0U;
  Xcp_ProcessedEventId = XCP_E_INVALID_EVENT;

  for (EvIdx = 0U; EvIdx < XCP_MAX_EVENT_CHANNEL; EvIdx++)
  {
    Xcp_RequestForEvent[EvIdx].RequestStatus = XCP_NO_MEMPROXY_REQUESTED;
    Xcp_RequestForEvent[EvIdx].ProcessingCtr = 0U;
    Xcp_RequestForEvent[EvIdx].NoOfMemProxyRequests = 0U;
    Xcp_RequestToMemProxy.NoOfEntries[EvIdx] = 0U;
    Xcp_DaqPrioQueueOfRequest.NoOfEntries[EvIdx] = 0U;
    if (EvIdx < XCP_NUMBER_OF_QUEUED_EVENTS)
    {
      Xcp_RequestQueue.EventId[EvIdx] = XCP_E_INVALID_EVENT;
    }
  }

  Xcp_RequestToMemProxy.WriteIdx = 0U;
  Xcp_DaqPrioQueueOfRequest.WriteIdx = 0U;
  Xcp_DaqPrioQueueOfRequest.ReadIdx = 0U;
}

FUNC(void, XCP_CODE) Xcp_MemoryProxyEnqueueRequest
(
  Xcp_DaqIdType DaqId,
  uint8 OdtEntryLength,
  Xcp_MirrorSizeType DTOMirrorWritePos,
  uint8 AppId,
  uint8 Type,
  P2VAR(uint8, AUTOMATIC, XCP_APPL_DATA) MemoryAreaAddress,
  uint8 bitMask
)
{
  Xcp_RequestForEvent[Xcp_ProcessedEventId].EntriesForAppIdAvailable[AppId] = TRUE;
  /* serialize the request in the buffer of the core */
  Xcp_RequestBuffer[Xcp_RequestToMemProxy.WriteIdx].size = OdtEntryLength;
  Xcp_RequestBuffer[Xcp_RequestToMemProxy.WriteIdx].address1 = MemoryAreaAddress;
  Xcp_RequestBuffer[Xcp_RequestToMemProxy.WriteIdx].address2 = XCP_GET_MIRROR_PTR(DaqId, DTOMirrorWritePos);
  Xcp_RequestBuffer[Xcp_RequestToMemProxy.WriteIdx].type = Type;
  Xcp_RequestBuffer[Xcp_RequestToMemProxy.WriteIdx].appId = AppId;
  Xcp_RequestBuffer[Xcp_RequestToMemProxy.WriteIdx].bitMask = bitMask;
  /* update the Index for further events */
  if (Xcp_RequestToMemProxy.WriteIdx == XCP_MAX_SIZE_OF_REQUEST_BUFFER - 1U)
  {
    Xcp_RequestToMemProxy.WriteIdx = 0U;
  }
  else
  {
    /* increment the writting index */
    Xcp_RequestToMemProxy.WriteIdx++;
  }
#if (XCP_DAQ_COUNT == 0U)
  TS_PARAM_UNUSED(DaqId);
#endif
}

STATIC FUNC(boolean, XCP_CODE) Xcp_IsPosibleToProcessEvent(void)
{
  boolean RetValue = TRUE;
  uint16 EventIdx;

  for (EventIdx = 0U; EventIdx < XCP_MAX_EVENT_CHANNEL; EventIdx++)
  {
    if (Xcp_RequestForEvent[EventIdx].RequestStatus == XCP_PROCESS_ON_MEMPROXY_REQUESTED)
    {
      RetValue = FALSE;
      break;
    }
  }

  return RetValue;
}

/* Deviation MISRAC2012-3 */
STATIC FUNC(boolean, XCP_CODE) Xcp_IsPosibleToEnqueueEvent(uint16 EventID)
{
  boolean RetValue = TRUE;
  uint8 EventIdx;

  /* check if there available space in the queue */
  if ((Xcp_RequestQueue.WriteIdx == Xcp_RequestQueue.ReadIndex) &&
      (Xcp_RequestQueue.EventId[Xcp_RequestQueue.ReadIndex] != XCP_E_INVALID_EVENT))
  {
    RetValue = FALSE;
  }
  else
  {
    /* check if event was not already queued */
    for (EventIdx = 0U; EventIdx < XCP_NUMBER_OF_QUEUED_EVENTS; EventIdx++)
    {
      if (Xcp_RequestQueue.EventId[EventIdx] == EventID)
      {
        RetValue = FALSE;
        break;
      }
    }
  }

  return RetValue;
}

/* Deviation MISRAC2012-3 */
STATIC FUNC(void, XCP_CODE) Xcp_ProcessQueuedEvents(void)
{
  uint8 AppIdx;
  Xcp_MemoryProxyDataType Xcp_MemoryProxyData;
  Atomic_value_t partition = Atomics_Load(&Xcp_PartitionOfActiveConnection);
  uint16 EventId;
  uint8 OldReadIdx;

  /* enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  EventId = Xcp_RequestQueue.EventId[Xcp_RequestQueue.ReadIndex];

  /* exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  if (EventId != XCP_E_INVALID_EVENT)
  {
    /* enter critical section */
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

    OldReadIdx = Xcp_RequestQueue.ReadIndex;

    /* update the read index in the Request Queue */
    if ((Xcp_RequestQueue.ReadIndex + 1) < XCP_NUMBER_OF_QUEUED_EVENTS)
    {
      Xcp_RequestQueue.ReadIndex++;
    }
    else
    {
      Xcp_RequestQueue.ReadIndex = 0U;
    }

    /* exit critical section */
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

    Xcp_RequestForEvent[EventId].RequestStatus = XCP_PROCESS_ON_MEMPROXY_REQUESTED;
    for (AppIdx = 0U; AppIdx < XCP_NUM_OF_OS_APPS; AppIdx++)
    {
      if (Xcp_RequestForEvent[EventId].EntriesForAppIdAvailable[AppIdx] == TRUE)
      {
        Xcp_MemoryProxyData.size = Xcp_RequestToMemProxy.NoOfEntries[EventId];
        Xcp_MemoryProxyData.type = XCP_MEMORYPROXYDATA_EVENT_PROCESSOR;
        /* Not used for this kind of request */
        Xcp_MemoryProxyData.address = NULL_PTR;
        Xcp_MemoryProxyData.RequestBuffer = &Xcp_RequestBuffer[0U];
        Xcp_MemoryProxyData.IdxRequestBuffer = Xcp_RequestForEvent[EventId].IndexOfRequest;
        Xcp_MemoryProxyData.EventId = EventId;
        Xcp_RequestQueue.EventId[OldReadIdx] = XCP_E_INVALID_EVENT;
        /* !LINKSTO Xcp.BswDistribution.Communication.UnitInteraction.CoreLocal,1 */
        Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_LOCKED);

        if (AppIdx != partition)
        {
          /* !LINKSTO Xcp.dsn.BSWDistribution.EventProcessing.DataAquisition.MemProxy,1 */
          /* !LINKSTO Xcp.dsn.BSWDistribution.EventProcessing.DataStimulation.MemProxy,1 */
          (void)Xcp_SendMemoryProxyData[partition][AppIdx](&Xcp_MemoryProxyData);
        }
        else
        {
          /* send request localy */
          Xcp_MemoryProxyHandler(&Xcp_MemoryProxyData);
        }
      }
    }
  }
}
#endif

STATIC FUNC(void, XCP_CODE) Xcp_UpdateDaqPrioQueue
(
  Xcp_DaqIdType DaqId,
  uint8 OdtIndex,
  Xcp_OdtSizeType DataLength
)
{
  /* semaphore indicating if the Dto is already in the priority list */
  boolean DtoIsAlreadyInPrioList = FALSE;
  /* initialize with the position at the end of DaqList priority list.
   * Rationale: we update the Id in case we found a suitable position inside the queue */
  Xcp_DaqIdType QueueInsertPosition;
  /* Initialize the DAQ Prio Queue count to zero */
  Xcp_DaqIdType DaqPrioQueueCount;
  /* The DAQ list to be sampled */
  P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr = XCP_GET_DAQ_LIST_PTR(DaqId);
    /* current Ids*/
  Xcp_DaqIdType CurrentDaqId;
  uint8 CurrentOdtId;
  P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListCurrentPtr;

  /* !LINKSTO Xcp.ODT.Processing.ODTSize0.DTO,1 */
  if (DataLength != 0U)
  {
    /* Update DTO Data Size */
    DaqListPtr->OdtList[OdtIndex].DTODataSize = DataLength;

    /* the access to DaqList transmission priority list has to be done atomically
     * and we have to protect the operation via critical section */
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

    /* initialize with the position at the end of DaqList priority list.
     * Rationale: we update the Id in case we found a suitable position inside the queue */
    QueueInsertPosition = Xcp_DaqPrioQueue.NoOfEntries;

    /* Find position from Daq Priority Queue in which should be insert information for current DTO */
    for(DaqPrioQueueCount = 0U;
        DaqPrioQueueCount < Xcp_DaqPrioQueue.NoOfEntries;
        DaqPrioQueueCount++)
    {
      /* store selected DaqList id */
      CurrentDaqId = XCP_GET_DAQ_ID_FROM_PRIO(DaqPrioQueueCount);
      /* store selected Odt id */
      CurrentOdtId = XCP_GET_ODT_ID_FROM_PRIO(DaqPrioQueueCount);
      /* Check if queue contain information for DTOs from current DAQ */
      if(CurrentDaqId == DaqId)
      {
        /* Find Insert Position in Queue considering ODT Index */
        if(CurrentOdtId == OdtIndex)
        {
#if (XCP_OVERLOAD_MSB_SUPPORT == STD_ON)
          /* Set flag to indicate event overload for current ODT */
          Xcp_DaqPrioQueue.DaqRuntimeDataArray[DaqPrioQueueCount].OverloadOdt |=
              XCP_EV_DAQ_OVERLOAD_EV_MASK;
#elif (XCP_EVENT_PACKET_ENABLED == STD_ON)
          Xcp_EmitDAQOverloadEvent();
#endif
          /* Information for current DTO is already in Queue and is not stored again*/
          DtoIsAlreadyInPrioList = TRUE;
          /* Deviation MISRAC2012-1 <START> */
          break;
        }
        else
        {
          if(CurrentOdtId > OdtIndex)
          {
            /* second priority criterion: for DTOs belonging to the same DaqList,
             * the DTO with lower PID has higher priority */
            /* we found the storing position */
            QueueInsertPosition = DaqPrioQueueCount;

            break;
          }
        }
      }
      else
      {
        /* Get the DAQ list from the current position in DaqPrioQueue */
        DaqListCurrentPtr = XCP_GET_DAQ_LIST_PTR(CurrentDaqId);
        /* Check if actual priority is higher then priority from Queue */
        if(DaqListCurrentPtr->Priority < DaqListPtr->Priority)
        {
          /* first priority criterion: for DTOs belonging to different DaqList,
           * the DTO belonging to the DaqList having higher priority, has
           * higher priority. */
          /* we found the storing position */
          QueueInsertPosition = DaqPrioQueueCount;
          break;
          /* Deviation MISRAC2012-1 <STOP> */
        }
      }
    }

    if(DtoIsAlreadyInPrioList == FALSE)
    {
      /*Shift to right the DaqPrioQueeue starting from position QueueInsertPosition*/
      for(DaqPrioQueueCount = Xcp_DaqPrioQueue.NoOfEntries;
          DaqPrioQueueCount > QueueInsertPosition;
          DaqPrioQueueCount--)
      {
        Xcp_DaqPrioQueue.DaqRuntimeDataArray[DaqPrioQueueCount] =
            Xcp_DaqPrioQueue.DaqRuntimeDataArray[DaqPrioQueueCount - 1U];
      }
      /* Store DAQ Id related to current DTO Entry */
      Xcp_DaqPrioQueue.DaqRuntimeDataArray[QueueInsertPosition].DaqId = DaqId;
      /* Store ODT Id related to current DTO Entry */
      Xcp_DaqPrioQueue.DaqRuntimeDataArray[QueueInsertPosition].OdtId = OdtIndex;
#if (XCP_OVERLOAD_MSB_SUPPORT == STD_ON)

      /* check if an overload was detected in the mean time */

      if(((DaqListPtr->Flags & XCP_MASK_DAQLIST_SKIPOVCHECK) != XCP_MASK_DAQLIST_SKIPOVCHECK) &&
          (TRUE == (XCP_IS_BYTE_FLAG_SET(XCP_EVENT_OVELOAD_FLAG_POS,
              &(Xcp_Event[DaqListPtr->EventId].EventStatusFlag)))))

      {
        /* Set flag to indicate event overload for current ODT */
        Xcp_DaqPrioQueue.DaqRuntimeDataArray[DaqPrioQueueCount].OverloadOdt =
            XCP_EV_DAQ_OVERLOAD_EV_MASK;
      }
      else
      {
        /* initialize overload flag */
        Xcp_DaqPrioQueue.DaqRuntimeDataArray[DaqPrioQueueCount].OverloadOdt = 0U;
      }
#endif
      /* Increment number of entries */
      Xcp_DaqPrioQueue.NoOfEntries++;
    }
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
  }
}

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

#endif /* #if (XCP_MAX_DAQ != 0U) */

#if ( (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON) || (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON) )

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

/* Deviation MISRAC2012-3 */
FUNC(void, XCP_CODE) Xcp_MemoryProxyResponseHandler(P2VAR(uint16, AUTOMATIC, XCP_APPL_DATA) EventId)
{
#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_OFF)
  TS_PARAM_UNUSED(EventId);
#else
  uint16 DTOIdx;
  /* For DAQ/STIM request the EventId is a valid id, for Download invalid event value is used */
  if (*EventId != XCP_E_INVALID_EVENT)
  {
    Xcp_RequestForEvent[*EventId].ProcessingCtr++;

    /* check if all memproxy instances have respond */
    if (Xcp_RequestForEvent[*EventId].ProcessingCtr == Xcp_RequestForEvent[*EventId].NoOfMemProxyRequests)
    {
      if (Xcp_CyclicEventOverloaded == FALSE)
      {
        uint16 IdxInRequest = Xcp_RequestForEvent[*EventId].IndexOfRequest;
        uint16 ReadIdx = 0U;

        for (DTOIdx = 0U; DTOIdx < Xcp_RequestToMemProxy.NoOfEntries[*EventId]; DTOIdx++)
        {
          if (Xcp_RequestBuffer[IdxInRequest].type == XCP_MEMORYPROXYDATA_SAMPLE)
          {
            /* !LINKSTO Xcp.dsn.BSWDistribution.EventProcessing.MemProxy.CopyDataIntoMirror,1 */
            /* copy data from MemProxy internal buffer into the Mirror RAM */
            TS_MemCpy(Xcp_RequestBuffer[IdxInRequest].address2,
              &Xcp_MemoryProxyBuffer[ReadIdx],
              Xcp_RequestBuffer[IdxInRequest].size);
            ReadIdx += Xcp_RequestBuffer[IdxInRequest].size;
          }
          if (IdxInRequest == XCP_MAX_SIZE_OF_REQUEST_BUFFER - 1U)
          {
            IdxInRequest = 0U;
          }
          else
          {
            IdxInRequest++;
          }
        }
        for (DTOIdx = 0U; DTOIdx < Xcp_DaqPrioQueueOfRequest.NoOfEntries[*EventId]; DTOIdx++)
        {
          Xcp_UpdateDaqPrioQueue(Xcp_DaqPrioQueueOfRequest.Queue[Xcp_DaqPrioQueueOfRequest.ReadIdx].DaqId,
                                 Xcp_DaqPrioQueueOfRequest.Queue[Xcp_DaqPrioQueueOfRequest.ReadIdx].OdtIndex,
                                 Xcp_DaqPrioQueueOfRequest.Queue[Xcp_DaqPrioQueueOfRequest.ReadIdx].DataLength);
          Xcp_DaqPrioQueueOfRequest.ReadIdx++;
          if (Xcp_DaqPrioQueueOfRequest.ReadIdx == XCP_MAX_SIZE_OF_DAQ_PRIO_LIST)
          {
            Xcp_DaqPrioQueueOfRequest.ReadIdx = 0U;
          }
        }
        Xcp_DaqPrioQueueOfRequest.NoOfEntries[*EventId] = 0U;

        /* Update event processing parameters */
        Xcp_UpdateEvent(*EventId, E_OK);
        /* Call transmit function */
        Xcp_ProcessTransmit(XCP_SID_MAIN_FUNCTION);
      }
      else
      {
        /* prepare for new memory proxy request */
        Xcp_CyclicEventOverloaded = FALSE;
#if((XCP_EVENT_PACKET_ENABLED == STD_ON) && \
    (XCP_OVERLOAD_MSB_SUPPORT == STD_OFF))
        /* Clear event overload flag */
        XCP_CLEAR_BYTE_FLAG(XCP_EVENT_OVELOAD_FLAG_POS,
                            &(Xcp_Event[*EventId].EventStatusFlag));
#endif
      }
      Xcp_RequestToMemProxy.NoOfEntries[*EventId] = 0U;
      Xcp_MemoryProxyErrorPending = FALSE;
      Xcp_RequestForEvent[*EventId].RequestStatus = XCP_NO_MEMPROXY_REQUESTED;
      Atomics_Store(&Xcp_MemoryProxyState, XCP_MEMORY_PROXY_AVAILABLE);
      Xcp_RequestForEvent[*EventId].ProcessingCtr = 0U;
      /* !LINKSTO Xcp.dsn.BSWDistribution.EventProcessing.MemProxy.DequeueRequest,1 */
      /* check if other event is available for processing */
      Xcp_ProcessQueuedEvents();
    }
  }
  else
#endif
  {
#if (XCP_RAM_CALLOUT_PROXY_ENABLED == STD_ON)
    Atomics_Store(&Xcp_RAMCalloutProxyState, XCP_RESULT_ALREADY_PROCESSED);
#endif
#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
    /* !LINKSTO Xcp.dsn.BSWDistribution.EventProcessing.MemProxy.DequeueRequest,1 */
    /* check if other event is available for processing */
    Xcp_ProcessQueuedEvents();
#endif
  }
}
#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

#endif

#endif /* ( (XCP_DEFAULT_XCP_MODULE_STATE == STD_ON) || (XCP_ENABLE_XCP_CONTROL_API == STD_ON) ) */

/*==================[end of file]===========================================*/
