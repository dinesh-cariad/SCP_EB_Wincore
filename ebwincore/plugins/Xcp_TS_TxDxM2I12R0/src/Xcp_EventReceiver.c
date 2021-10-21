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

/*==================[inclusions]============================================*/
#include <Xcp.h>               /* Module public API */
#if ( (XCP_DEFAULT_XCP_MODULE_STATE == STD_ON) || (XCP_ENABLE_XCP_CONTROL_API == STD_ON) )
#include <TSAutosar.h>         /* EB specific standard types */
#include <SchM_Xcp.h>          /* Needed for exclusive area definition */
#include <Xcp_Trace.h>

#include <Xcp_Int.h>           /* Internal defines and declarations */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#if (XCP_MAX_EVENT_CHANNEL > 0U) && (XCP_MAX_DAQ > 0U)

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

/** \brief Function to check whether any DAQ List of an event is active.
 **
 ** Use this function to check whether any DAQ List corresponding to the Event Id is active.
 **
 ** \pre This function is called within a critical section.
 **
 ** \param[in]  EventId  XCP DAQ Event Id.
 ** \return whether the event has a daq list active
 ** \retval E_OK     At least one DAQ List corresponding to the event is running
 ** \retval E_NOT_OK No DAQ List is running
 **/
STATIC FUNC(Std_ReturnType, XCP_CODE) Xcp_CheckDaqListActive
(
  uint16 EventId
);

#if(XCP_MAX_MF_PROCESSED_EVENTS > 0U) || (XCP_MAX_CYCLIC_EVENT_CHANNEL > 0U)
/** \brief Function to mark an event as being set
 **
 ** Use this function to mark an event as being set. This implies that it will begin
 ** sampling/stimulating its associated DAQ lists.
 **
 ** \param[in]  EventId  XCP Event Id.
 ** \return success of operation
 ** \retval XCP_OK       The event has been marked as set
 ** \retval XCP_OVERLOAD The event wasn't marked as set as it was previously set - OVERLOAD situation.
 **/
STATIC FUNC(Xcp_ReturnType, XCP_CODE) Xcp_MarkEventAsSet
(
  uint16 EventId
);
#endif /* (XCP_MAX_MF_PROCESSED_EVENTS > 0U) || (XCP_MAX_CYCLIC_EVENT_CHANNEL > 0U) */

#if (XCP_MAX_CYCLIC_EVENT_CHANNEL > 0U)
/** \brief Function to Set One Cyclic Event
 **
 ** \param[in]  EventId  XCP Event Id.
 **/
STATIC FUNC(void, XCP_CODE) Xcp_SetOneCyclicEvent
(
  uint16 EventId
);
#endif

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

#endif /* (XCP_MAX_EVENT_CHANNEL > 0U) && (XCP_MAX_DAQ > 0U) */
/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

FUNC(Xcp_ReturnType, XCP_CODE) Xcp_SetEvent(uint16 EventId)
{
  Xcp_ReturnType RetValue;
#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
  Atomic_value_t partition = Atomics_Load(&Xcp_PartitionOfActiveConnection);
  if (XCP_MASTER_APPLICATION == partition)
  {
    RetValue = Xcp_SetEvent_Internal(EventId);
  }
  else
  {
#if (XCP_REMOTE_DESTINATIONS == STD_ON)
    Xcp_RemoteDispatchType data;
    data.type = XCP_REMOTE_DISPATCH_SETEVENT;
    data.p16[0] = EventId;
    (void) Xcp_RemoteDispatch_Handler(data);
#endif
    RetValue = XCP_OK;
  }
#else
  RetValue = Xcp_SetEvent_Internal(EventId);
#endif
  return RetValue;
}

/*------------------[Xcp_SetEvent]---------------------------------*/
FUNC(Xcp_ReturnType, XCP_CODE) Xcp_SetEvent_Internal
(
  uint16 EventId
)
{
  Xcp_ReturnType RetVal = XCP_NOT_OK;

  DBG_XCP_SETEVENT_ENTRY(EventId);

  if (Xcp_ControlStateOfXcpModule != XCP_STATE_INACTIVE)
  {
#if (XCP_DEV_ERROR_DETECT == STD_ON)
    if (Xcp_Initialized == FALSE)
    {
      XCP_DET_REPORT_ERROR(XCP_SID_SET_EVENT, XCP_E_NOT_INITIALIZED);
      RetVal = XCP_NOT_INITIALIZED;
    }
#if (XCP_MAX_EVENT_CHANNEL > 0U)
    else if ((EventId >= XCP_MAX_EVENT_CHANNEL) ||
             (Xcp_EventInfo[EventId].TimeCycle != 0U))
    {
      XCP_DET_REPORT_ERROR(XCP_SID_SET_EVENT, XCP_E_INVALID_EVENT);
    }
#endif
    else
#endif /* (XCP_DEV_ERROR_DETECT == STD_ON) */
    {
#if(XCP_MAX_MF_PROCESSED_EVENTS > 0U)
      {
        /* enter critical section */
        SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

        /* The event needs to be processed only if, XCP is not in disconnected state and
           at least one DAQ list is configured in the event */
        if (Xcp_ConnectionStatus.State != XCP_STATE_DISCONNECTED)
        {
#if (XCP_MAX_DAQ > 0U)
          /* If any (currently running) DAQ List is associated with the event */
          if( Xcp_CheckDaqListActive(EventId) == E_OK )
          {
            /* Mark the event as set */
            RetVal = Xcp_MarkEventAsSet( EventId );
          }
          else
#endif /* (XCP_MAX_DAQ > 0U) */
          {
            RetVal = XCP_NO_ACTIVE_LIST;
          }
        }
        else
        {
          RetVal = XCP_NOT_CONNECTED;
        }

        /* exit critical section */
        SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
      }
#endif /* (XCP_MAX_MF_PROCESSED_EVENTS > 0U) */
    }
  }
  else /*(Xcp_ControlStateOfXcpModule == XCP_STATE_INACTIVE)*/
  {
    /* Nothing to do */
  }

#if (XCP_MAX_EVENT_CHANNEL == 0U)
  TS_PARAM_UNUSED(EventId);
#endif

  DBG_XCP_SETEVENT_EXIT(RetVal);
  return RetVal;
}

/*==================[internal function definitions]=========================*/

#if (XCP_MAX_EVENT_CHANNEL > 0U) && (XCP_MAX_DAQ > 0U)

#if (XCP_MAX_CYCLIC_EVENT_CHANNEL > 0U)
/*------------------[Xcp_SetCyclicEvents]---------------------------------*/

FUNC(void, XCP_CODE) Xcp_SetCyclicEvents(void)
{
  DBG_XCP_SETCYCLICEVENTS_ENTRY();

  /* Check if Slave is not disconnected */
  if (Xcp_ConnectionStatus.State != XCP_STATE_DISCONNECTED)
  {
    /* Local variable to hold Event Channel number */
    uint16 EventChannelNumber;

    /* Loop for all the Cyclic Event Channels */
    for( EventChannelNumber = 0U;
         EventChannelNumber < XCP_MAX_CYCLIC_EVENT_CHANNEL;
         EventChannelNumber++ )
    {
      /* The first events are the cyclic ones */
      /* Set the Cyclic Event Channel */
      Xcp_SetOneCyclicEvent( EventChannelNumber );
    }

  } /* ( Xcp_CheckConnected() == E_OK ) */

  DBG_XCP_SETCYCLICEVENTS_EXIT();
}

/*------------------[Xcp_SetOneCyclicEvent]---------------------------------*/
STATIC FUNC(void, XCP_CODE) Xcp_SetOneCyclicEvent
(
  uint16 EventId
)
{
  const uint16 IntEventId = Xcp_MFCyclicEventCfg[EventId];

  DBG_XCP_SETONECYCLICEVENT_ENTRY(EventId);

  /* Check whether any DAQ List of this event is currently running */
  if ( Xcp_CheckDaqListActive(IntEventId) == E_OK )
  {
    /* Decrement the Cyclic Event Counter */
    Xcp_Event[IntEventId].TimeCycleCounter--;

    /* Check the Cyclic Event Counter */
    if( Xcp_Event[IntEventId].TimeCycleCounter == 0U )
    {
      /* Cyclic Event Counter = 0U. Event cyclic time has elapsed. */
      /* Reinitialize the Cycle Event Counter to its Maximum value */
      Xcp_Event[IntEventId].TimeCycleCounter =
          Xcp_EventInfo[IntEventId].CycleCounterMax;

      /* enter critical section */
      SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
      /* Mark the cyclic event as set. No meaningful way to react on error situations here,
       * therefore ignoring the return value. */
      (void)Xcp_MarkEventAsSet( IntEventId );
      /* exit critical section */
      SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
#if (XCP_DAQSTIM_MEMORY_ACCESS_AREAS == STD_ON)
      if ((Xcp_CyclicEventOverloaded == FALSE) &&
          (Xcp_RequestForEvent[IntEventId].RequestStatus != XCP_NO_MEMPROXY_REQUESTED))
      {
        /* mark event as overload */
        Xcp_CyclicEventOverloaded = TRUE;

        XCP_DET_REPORT_RUNTIME_ERROR(XCP_SID_MAIN_FUNCTION, XCP_E_MEMORY_PROXY_CYCLIC_EVENT_SYNC_ERROR);
#if((XCP_EVENT_PACKET_ENABLED == STD_ON) && \
    (XCP_OVERLOAD_MSB_SUPPORT == STD_OFF))
        /* enter critical section */
        SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

        /* Emit an Event CTO with DAQ overload */
        Xcp_EmitDAQOverloadEvent();

        /* exit critical section */
        SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
#endif
      }
#endif
    } /* ( Xcp_Event[EventId].TimeCycleCounter == 0U ) */

  } /* ( E_OK == Xcp_CheckDaqListActive(EventId) ) */

  DBG_XCP_SETONECYCLICEVENT_EXIT();
}
#endif /* (XCP_MAX_CYCLIC_EVENT_CHANNEL > 0U) */

/*------------------[Xcp_CheckDaqListActive]--------------------------------*/

STATIC FUNC(Std_ReturnType, XCP_CODE) Xcp_CheckDaqListActive
(
  uint16 EventId
)
{
  /* Set as no DAQ List is running */
  Std_ReturnType RetVal = E_NOT_OK;

  uint8 i = 0;
  uint8 DaqListCount;
  /* Variable to hold address of a DAQ List */
  P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr;

  DBG_XCP_CHECKDAQLISTACTIVE_ENTRY(EventId);

  /* Total DAQ Lists associated with the particular event taken */
  DaqListCount = Xcp_Event[EventId].DaqIdListCount;

  /* Check whether any DAQ List is currently running */
  if ((Xcp_Status.SessionStatus & XCP_MASK_DAQ_RUNNING ) != 0U )
  {
    /* Loop through all DAQ Lists corresponding to the Event */
    while(i < DaqListCount)
    {
      /* Retrieve DAQ List pointer */
      DaqListPtr = XCP_GET_DAQ_LIST_PTR(Xcp_Event[EventId].DaqIdListPtr[i]);

      /* Check whether DAQ List is currently running */
      if(XCP_IS_DAQ_LIST_RUNNING(DaqListPtr) == TRUE)
      {
        RetVal = E_OK;
        break;
      }
      /* Select next DAQ List */
      i++;
    }
  }

  DBG_XCP_CHECKDAQLISTACTIVE_EXIT(RetVal);
  return RetVal;
}

#if(XCP_MAX_MF_PROCESSED_EVENTS > 0U) || (XCP_MAX_CYCLIC_EVENT_CHANNEL > 0U)

/*------------------[Xcp_MarkEventAsSet]--------------------------------*/

STATIC FUNC(Xcp_ReturnType, XCP_CODE) Xcp_MarkEventAsSet
(
  uint16 EventId
)
{
  /* The function return value */
  Xcp_ReturnType RetVal = XCP_NOT_OK;
#if (XCP_OVERLOAD_MSB_SUPPORT == STD_ON)
  uint8 EvDaqIndex;
  P2VAR(Xcp_DaqType, AUTOMATIC, XCP_VAR_CLEARED) DaqListPtr;
#endif

  DBG_XCP_MARKEVENTASSET_ENTRY(EventId);

  /* Check if the event was not previously set */
  if (XCP_IS_BYTE_FLAG_SET(XCP_EVENT_IS_SET_FLAG_POS,
                           &(Xcp_Event[EventId].EventStatusFlag)) == FALSE)
  {
    /* Update the event as being set */
    XCP_SET_BYTE_FLAG(XCP_EVENT_IS_SET_FLAG_POS,
                      &(Xcp_Event[EventId].EventStatusFlag));
    RetVal = XCP_E_OK;
  }
  else
  {
    /* Overload situation is detected. Set event overload flag */
    XCP_SET_BYTE_FLAG(XCP_EVENT_OVELOAD_FLAG_POS,
                      &(Xcp_Event[EventId].EventStatusFlag));

#if (XCP_OVERLOAD_MSB_SUPPORT == STD_ON)
    for(EvDaqIndex = 0U; EvDaqIndex < Xcp_Event[EventId].DaqIdListCount; EvDaqIndex++)
    {
      /* select daq list configuration */
      DaqListPtr = XCP_GET_DAQ_LIST_PTR(Xcp_Event[EventId].DaqIdListPtr[EvDaqIndex]);
      /* enable the checking for event channel overload detection, for the selected daqlist */
      DaqListPtr->Flags &= (uint8)(~XCP_MASK_DAQLIST_SKIPOVCHECK);
      DaqListPtr->Flags &= (uint8)(~XCP_MASK_DAQLIST_OVCHECKSKIPPED);
    }
#endif
    RetVal = XCP_OVERLOAD;
  }

  DBG_XCP_MARKEVENTASSET_EXIT(RetVal);
  return RetVal;
}
#endif /* (XCP_MAX_MF_PROCESSED_EVENTS > 0U) || (XCP_MAX_CYCLIC_EVENT_CHANNEL > 0U) */

#endif /* (XCP_MAX_EVENT_CHANNEL > 0U) && (XCP_MAX_DAQ > 0U) */

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

#endif /* ( (XCP_DEFAULT_XCP_MODULE_STATE == STD_ON) || (XCP_ENABLE_XCP_CONTROL_API == STD_ON) ) */

/*==================[end of file]===========================================*/
