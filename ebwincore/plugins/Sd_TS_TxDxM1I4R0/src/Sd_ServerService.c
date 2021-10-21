/**
 * \file
 *
 * \brief AUTOSAR Sd
 *
 * This file contains the implementation of the AUTOSAR
 * module Sd.
 *
 * \version 1.4.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*  MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 *  A conversion should not be performed from pointer to void into pointer to object.
 *
 *   Reason:
 *   Pointer must be cast to more generic type
 *
 *  MISRAC2012-2) Deviated Rule: 13.5 (required)
 *  The right hand operand of a logical && or || operator shall not contain persistent side effects.
 *
 *   Reason:
 *   Right hand operator in this case does not have any persistent side effects.
 *
 */

/*==================[inclusions]============================================*/

#include <Sd.h>               /* Module public API */
#include <Sd_ServerService_Int.h>
#include <Sd_Send_Int.h>

#include <SoAd.h>
#include <BswM_Sd.h>
#include <SchM_Sd.h>           /* SchM symbols for crit. sections */

#include <TSMem.h>
/*==================[macros]================================================*/

#define SD_SERVER_EVENT_START 0x01U
#define SD_SERVER_EVENT_STOP  0x02U
#define SD_SERVER_EVENT_HALT  0x04U
#define SD_SERVER_EVENT_STOPHALT_MASK 0x06U

#define SD_INVALIDEVENTGROUPINDEX 0xFFFFU

/*==================[type definitions]======================================*/
/* !LINKSTO Sd.Design.Data.ServiceStates,1 */
typedef struct
{
  Sd_TimerType TimeOutDelta;
  Sd_ServerServiceSetStateType ServerServiceState;
  uint8 ComPhase;
  uint8 Repetition;
  uint8 EventFlags;
}Sd_ServerServiceStateType;

typedef struct
{
  uint16 ClientCount;
}Sd_EventHandlerStateType;

typedef struct
{
  uint32 TTL;
  SoAd_SoConIdType SockConIdUdp;
  SoAd_SoConIdType SockConIdTcp;
}Sd_ServerSoConStateType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define SD_START_SEC_CODE
#include <Sd_MemMap.h>

STATIC FUNC(boolean, SD_CODE) Sd_ServerService_match
(
  CONSTP2CONST(Sd_ServiceInfoType, AUTOMATIC, SD_APPL_DATA) ServiceInfoPtr,
  CONSTP2CONST(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfoPtr,
  CONSTP2CONST(SdServerService_t, AUTOMATIC, SD_APPL_DATA) ServiceCfgPtr,
  const uint32 MinorVersion
);

STATIC FUNC(uint16, SD_CODE) Sd_ServerService_QuickMatch
(
  CONSTP2CONST(Sd_ServiceInfoType, AUTOMATIC, SD_APPL_DATA) ServiceInfoPtr,
  CONSTP2CONST(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfoPtr,
  const uint32 MinorVersion,
  const uint8 SdInstanceIndex
);

STATIC FUNC(void, SD_CODE) Sd_ServerService_Set_State
(
  const uint16 ServiceIndex,
  const Sd_ServerServiceSetStateType state
);

STATIC FUNC(Std_ReturnType, SD_CODE) Sd_ServerService_addEventGroupSubscriber
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, SD_APPL_DATA) UdpSockAddrPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, SD_APPL_DATA) TcpSockAddrPtr,
  const uint32 TTL,
  const uint16 ServiceIndex,
  uint16 EventgroupIndex,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  const uint8 InstanceIndex
);

STATIC FUNC(void, SD_CODE) Sd_ServerService_Add_TCP_DataPath
(
  CONSTP2CONST(Sd_ServerSoConStateType, AUTOMATIC, SD_APPL_DATA) SrvSoConState,
  CONSTP2CONST(SdEventHandler_t, AUTOMATIC, SD_APPL_CONST) EventHandlerCfgPtr,
  CONSTP2VAR(Sd_EventHandlerStateType,AUTOMATIC,SD_APPL_DATA) EventHandlerStatePtr,
  uint16 EventgroupIndex
);

STATIC FUNC(void, SD_CODE) Sd_ServerService_Add_UDP_DataPath
(
  CONSTP2VAR(Sd_ServerSoConStateType, AUTOMATIC, SD_APPL_DATA) SrvSoConState,
  CONSTP2CONST(SdEventHandler_t, AUTOMATIC, SD_APPL_CONST) EventHandlerCfgPtr,
  CONSTP2VAR(Sd_EventHandlerStateType,AUTOMATIC,SD_APPL_DATA) EventHandlerStatePtr,
  uint16 EventgroupIndex
);

STATIC FUNC(void, SD_CODE) Sd_ServerService_addMulticastEventGroupSubscriber
(
  const uint32 TTL,
  uint16 EventgroupIndex,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  const uint8 counter,
  const uint8 InstanceIndex
);

STATIC FUNC(void, SD_CODE) Sd_ServerService_removeEventGroupSubscriber
(
  CONSTP2VAR(Sd_EventHandlerStateType,AUTOMATIC,SD_APPL_DATA) EventHandlerStatePtr,
  CONSTP2VAR(Sd_ServerSoConStateType, AUTOMATIC, SD_APPL_DATA) SrvSoConState,
  uint16 EventgroupIndex,
  const boolean IsStop
);

STATIC FUNC(void, SD_CODE) Sd_ServerService_ComSM_init(const uint16 ServiceIndex);

STATIC FUNC(boolean, SD_CODE) Sd_ServerService_ComSM_SoConModeCheck
(
  CONSTP2CONST(SdServerService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr
);

/* event */
STATIC FUNC(void, SD_CODE) Sd_ServerService_ComSM_event_timeout
(
  CONSTP2CONST(SdServerService_t,AUTOMATIC,SD_APPL_CONST) SrvServiceCfgPtr,
  CONSTP2VAR(Sd_ServerServiceStateType,AUTOMATIC,SD_APPL_DATA) SrvServiceStatePtr,
  uint16_least SrvServiceIndex
);

/* transitions */

/* state entry functions */
STATIC FUNC(void, SD_CODE) Sd_ServerService_ComSM_entry_DOWN
(
  CONSTP2CONST(SdServerService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ServerServiceStateType,AUTOMATIC,SD_APPL_DATA) ServiceStatePtr,
  const Sd_RemoteNodeIdType RemoteNodeIndexEnd,
  const boolean FollowedByStart
);
STATIC FUNC(void, SD_CODE) Sd_ServerService_ComSM_entry_INITIALWAIT
(
  CONSTP2CONST(SdServerService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ServerServiceStateType,AUTOMATIC,SD_APPL_DATA) ServiceStatePtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr,
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr
);

/* state exit functions */
STATIC FUNC(void, SD_CODE) Sd_ServerService_ComSM_exit_DOWN
(
    CONSTP2CONST(SdServerService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr
);

#define SD_STOP_SEC_CODE
#include <Sd_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/
#define SD_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Sd_MemMap.h>

STATIC VAR(Sd_ServerServiceStateType, SD_VAR) Sd_ServerServiceStates[SD_ARRAY_SIZE(SD_MAX_SERVER_SERVICE_COUNT)];
STATIC VAR(Sd_EventHandlerStateType, SD_VAR) Sd_ServerEventHandlerStates[SD_ARRAY_SIZE(SD_MAX_SERVER_SERVICE_EVENT_COUNT)];
STATIC VAR(Sd_ServerSoConStateType, SD_VAR) Sd_ServerSoConStates[SD_ARRAY_SIZE(SD_MAXNUM_SUBSCRIPTIONS)];

#define SD_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Sd_MemMap.h>

#define SD_START_SEC_VAR_CLEARED_8
#include <Sd_MemMap.h>

/* this bitmap contains a bit for each server service of each instance multiplied by the number of remote nodes configured for each instance:
 * as example:
 * Instance 0 has 3 Server Services and 5 Remote nodes: one byte is reserved for each remote node, but only the first 3 bits of each 5 bytes will
 * tell if a server service has received a FIND message.
 * If server service index 1 receives a FIND from remote node #4 (index 3), then the 2nd bit (index 1) of the 4th byte (index 3) will be set to 1.
 */
STATIC VAR(uint8, SD_VAR) Sd_ServerFindReceivedGlobalBitmap[SD_ARRAY_SIZE(SD_SERVER_FIND_RECEIVED_GLOBAL_MAX_BITMAP_SIZE)];

#define SD_STOP_SEC_VAR_CLEARED_8
#include <Sd_MemMap.h>

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/
#define SD_START_SEC_CODE
#include <Sd_MemMap.h>

/* Module API functions */

FUNC(Std_ReturnType, SD_CODE) Sd_ServerServiceSetState
(
  uint16 ServiceServiceHandleId,
  Sd_ServerServiceSetStateType ServerServiceState
)
{
  Std_ReturnType ReturnValue = E_NOT_OK;

  DBG_SD_SERVERSERVICESETSTATE_ENTRY(ServiceServiceHandleId,ServerServiceState);

#if (SD_DEV_ERROR_DETECT == STD_ON)
  if (Sd_Initialized == FALSE)
  {
    SD_DET_REPORT_ERROR(SD_SERVERSERVICESETSTATE_SVCID,SD_E_NOT_INITIALIZED);
  }
  else if ((ServerServiceState != SD_SERVER_SERVICE_DOWN) && (ServerServiceState != SD_SERVER_SERVICE_AVAILABLE))
  {
    SD_DET_REPORT_ERROR(SD_SERVERSERVICESETSTATE_SVCID,SD_E_INV_MODE);
  }
  else if (ServiceServiceHandleId >= SdConfigPtr->SdServerServiceCount)
  {
    SD_DET_REPORT_ERROR(SD_SERVERSERVICESETSTATE_SVCID,SD_E_INV_ID);
  }
  else
#endif
  {
    SchM_Enter_Sd_SCHM_SD_EXCLUSIVE_AREA_0();

    Sd_ServerService_Set_State(ServiceServiceHandleId,ServerServiceState);

    SchM_Exit_Sd_SCHM_SD_EXCLUSIVE_AREA_0();

    ReturnValue = E_OK;
  }

  DBG_SD_SERVERSERVICESETSTATE_EXIT(ReturnValue,ServiceServiceHandleId,ServerServiceState);

  return ReturnValue;
}


/* !LINKSTO Sd.ArchDesign.ServerService.API.Sd_ServerService_getComphase,1 */
FUNC(uint8, SD_CODE) Sd_ServerService_getComphase
(
  const uint16 ServiceIndex
)
{
  return Sd_ServerServiceStates[ServiceIndex].ComPhase;
}



/* SW-Unit API functions */
/* !LINKSTO Sd.ArchDesign.ServerService.API.Sd_ServerService_init,1 */
TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ServerService_init(void)
{
  uint16 Index;

#if SD_MAXNUM_SUBSCRIPTIONS > 0U
  for (Index = 0U; Index < SD_MAXNUM_SUBSCRIPTIONS; ++Index)
  {
    Sd_ServerSoConStates[Index].SockConIdUdp = SOAD_INVALID_SOCON_ID;
    Sd_ServerSoConStates[Index].SockConIdTcp = SOAD_INVALID_SOCON_ID;
    Sd_ServerSoConStates[Index].TTL = SD_TTL_OFF;
  }
#endif /* SD_MAXNUM_SUBSCRIPTIONS > 0U */

  for (Index = 0U; Index < SdConfigPtr->SdEventHandlerCount; ++Index)
  {
    Sd_ServerEventHandlerStates[Index].ClientCount = 0U;
  }

  /* reset global FINDs bitmap to 0 at init time */
  TS_MemBZero(Sd_ServerFindReceivedGlobalBitmap, SD_SERVER_FIND_RECEIVED_GLOBAL_MAX_BITMAP_SIZE);

  for (Index = 0U; Index < SdConfigPtr->SdServerServiceCount; ++Index)
  {
    Sd_ServerService_ComSM_init(Index);
  }
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ServerServices_init_after_halt
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr
)
{
  /* reset global FINDs bitmap to 0 at init time */
  TS_MemBZero(&Sd_ServerFindReceivedGlobalBitmap[InstanceCfgPtr->FindReceivedBitmapStartByte], (InstanceCfgPtr->FindReceivedBitmapByteSize * SD_MAXNUM_INSTANCE_REMOTENODES));
}



/* this function is only called from Sd.c if there have events recorded for this whole instance
 * since the previous main cycle.
 * Variable "ServicesEventsCount" is a counter that stores the number of events registered since
 * last main cycle. If ServicesEventsCount is equal to 0, this function is not called.
 */
TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ServerService_mainFunction
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr,
  const uint16 ServicesEventsCount
)
{
  uint16_least ServiceIndex = InstanceCfgPtr->SdServerService;
  P2CONST(SdServerService_t,AUTOMATIC,SD_APPL_CONST) ServerServiceCfgPtr = &SD_CFG(SdServerService_t, SdServerServicesRef, ServiceIndex);
  P2VAR(Sd_ServerServiceStateType,AUTOMATIC,SD_APPL_DATA) ServerServiceStatePtr = &Sd_ServerServiceStates[ServiceIndex];
  const uint16_least ServiceIndexEnd = ServiceIndex + InstanceCfgPtr->SdServerServiceCount;
  uint16 ServicesEvents = ServicesEventsCount;

#if (SD_DEV_ERROR_DETECT == STD_ON)
  /* this defensive check covers where there might be no configured server services for this instance and also the case where no service events
   * have been recoreded */
  SD_PRECONDITION_ASSERT( (ServicesEventsCount > 0U) && (ServiceIndexEnd > ServiceIndex), SD_SERVERSERVICEMAINFUNCTION_SVCID );
#endif
  do
  {
    /* only if an event was recorded for this server service since last main cycle */
    if(ServerServiceStatePtr->EventFlags != 0U)
    {
      uint8 Events;
      /* !LINKSTO Sd.Design.ServerService.MainFunction.001,1 */
      SchM_Enter_Sd_SCHM_SD_EXCLUSIVE_AREA_0();
      Events = ServerServiceStatePtr->EventFlags;
      ServerServiceStatePtr->EventFlags = 0U;
      SchM_Exit_Sd_SCHM_SD_EXCLUSIVE_AREA_0();

      switch(ServerServiceStatePtr->ComPhase)
      {
        case SD_SERVER_SERVICE_COMPHASE_DOWN:
          if( (Events & SD_SERVER_EVENT_START) != 0U )
          { /* exit DOWN phase */
            Sd_ServerService_ComSM_exit_DOWN(ServerServiceCfgPtr);
            /* entry code */
            Sd_ServerService_ComSM_entry_INITIALWAIT(ServerServiceCfgPtr, ServerServiceStatePtr, InstanceStatePtr, InstanceCfgPtr);
            --ServicesEvents;
          }
          break;

          case SD_SERVER_SERVICE_COMPHASE_INITIALWAIT:
            /* CHECK: NOPARSE */
#if (SD_DEV_ERROR_DETECT == STD_ON)
            if(Events == SD_SERVER_EVENT_START)
            {
              SD_DET_REPORT_ERROR(SD_SERVERSERVICEMAINFUNCTION_SVCID, SD_E_INV_MODE);
            }
#endif /* SD_DEV_ERROR_DETECT == STD_ON */
            /* CHECK: PARSE */

            if( (Events & SD_SERVER_EVENT_HALT) != 0U )
            {
              Sd_StopTimer(&(ServerServiceStatePtr->TimeOutDelta));

              /* Address has been UNASSIGNED, but just in case it is checked if state ASSIGNED was received also within the same main cycle */
              if( (Events & SD_SERVER_EVENT_START) == 0U )
              { /* go from server service INITIAL WAIT state to server service DOWN state */
                Sd_ServerService_ComSM_entry_DOWN(ServerServiceCfgPtr, ServerServiceStatePtr, InstanceCfgPtr->RemoteNodeCount, FALSE);
                --ServicesEvents;
              }
              else
              { /* a sequence UNASSIGNED and then ASSIGNED has been received whithin the same main cycle */
                ServicesEvents -= 2U;
              }
            }
            else /* this can only be a STOP event or a STOP event followed by a START event */
            {
              Sd_StopTimer(&(ServerServiceStatePtr->TimeOutDelta));

#if (SD_DEV_ERROR_DETECT == STD_ON)
              /* this defensive check covers the case of an underflow in the variable ServerServicesTimersActive */
              SD_PRECONDITION_ASSERT( (InstanceStatePtr->ServerServicesTimersActive > 0U), SD_SERVERSERVICEMAINFUNCTION_SVCID );
#endif
              --InstanceStatePtr->ServerServicesTimersActive;
              if(InstanceStatePtr->ServerServicesTimersActive == 0U)
              {
                InstanceStatePtr->ServerDeltaTimer = 0U;
              }

              if( (Events & SD_SERVER_EVENT_START) == 0U )
              { /* go from server service INITIAL WAIT state to server service DOWN state */
                Sd_ServerService_ComSM_entry_DOWN(ServerServiceCfgPtr, ServerServiceStatePtr, InstanceCfgPtr->RemoteNodeCount, FALSE);
                --ServicesEvents;
              }
              else
              { /* entry code */
                Sd_ServerService_ComSM_entry_INITIALWAIT(ServerServiceCfgPtr, ServerServiceStatePtr, InstanceStatePtr, InstanceCfgPtr);
                ServicesEvents -= 2U;
              }
            }
            break;

          case SD_SERVER_SERVICE_COMPHASE_REPETITION:
          case SD_SERVER_SERVICE_COMPHASE_MAIN:
            /* CHECK: NOPARSE */
#if (SD_DEV_ERROR_DETECT == STD_ON)
            if(Events == SD_SERVER_EVENT_START)
            {
              SD_DET_REPORT_ERROR(SD_CLIENTSERVICE_COMSM_SOCONMODECHECK_SVCID, SD_E_INV_MODE);
            }
#endif /* SD_DEV_ERROR_DETECT == STD_ON */
            /* CHECK: PARSE */
            if( (Events & SD_SERVER_EVENT_HALT) != 0U )
            {
              Sd_StopTimer(&(ServerServiceStatePtr->TimeOutDelta));
              /* go from server service INITIAL WAIT state to server service DOWN state */
              Sd_ServerService_ComSM_entry_DOWN(ServerServiceCfgPtr, ServerServiceStatePtr, InstanceCfgPtr->RemoteNodeCount, (Events & SD_SERVER_EVENT_START));

              --ServicesEvents;
            }
            else /* this can only be a STOP event or a STOP event followed by a START event */
            {
              /* only stop, no halt */
              Sd_Send_queueOfferService(ServerServiceCfgPtr, (uint16)ServiceIndex, TRUE);

              if(ServerServiceStatePtr->TimeOutDelta != 0U)
              {
                Sd_StopTimer(&(ServerServiceStatePtr->TimeOutDelta));
#if (SD_DEV_ERROR_DETECT == STD_ON)
                /* this defensive check covers the case of an underflow in the variable ServerServicesTimersActive */
                SD_PRECONDITION_ASSERT( (InstanceStatePtr->ServerServicesTimersActive > 0U), SD_SERVERSERVICEMAINFUNCTION_SVCID );
#endif
                --InstanceStatePtr->ServerServicesTimersActive;
                if(InstanceStatePtr->ServerServicesTimersActive == 0U)
                {
                  InstanceStatePtr->ServerDeltaTimer = 0U;
                }
              }
              /* go from server service INITIAL WAIT state to server service DOWN state */
              Sd_ServerService_ComSM_entry_DOWN(ServerServiceCfgPtr, ServerServiceStatePtr, InstanceCfgPtr->RemoteNodeCount, (Events & SD_SERVER_EVENT_START));
              --ServicesEvents;
            }
            /* check if there is a START event just after HALT or STOP */
            if( (Events & SD_SERVER_EVENT_START) != 0U )
            { /* entry code */
              Sd_ServerService_ComSM_entry_INITIALWAIT(ServerServiceCfgPtr, ServerServiceStatePtr, InstanceStatePtr, InstanceCfgPtr);
              --ServicesEvents;
            }
            break;
          /* CHECK: NOPARSE */
          default: /* this default case is intended to be used only in development error detection mode */
#if (SD_DEV_ERROR_DETECT == STD_ON)
            SD_UNREACHABLE_CODE_ASSERT(SD_CLIENTSERVICE_COMSM_SOCONMODECHECK_SVCID);
#endif /* SD_DEV_ERROR_DETECT == STD_ON */
            break;
         /* CHECK: PARSE */
      }

      if(ServicesEvents == 0U)
      {
        break;
      }
    }
    /* else no event, nothing to process */
    ++ServerServiceCfgPtr;
    ++ServerServiceStatePtr;
    ++ServiceIndex;
  }
  while(ServiceIndex < ServiceIndexEnd);
}


TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ServerService_mainTimersFunction
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr
)
{
  uint16_least ServiceIndex = InstanceCfgPtr->SdServerService;
  P2CONST(SdServerService_t,AUTOMATIC,SD_APPL_CONST) ServerServiceCfgPtr = &SD_CFG(SdServerService_t, SdServerServicesRef, ServiceIndex);
  P2VAR(Sd_ServerServiceStateType,AUTOMATIC,SD_APPL_DATA) ServerServiceStatePtr = &Sd_ServerServiceStates[ServiceIndex];
  const uint16_least ServiceIndexEnd = ServiceIndex + InstanceCfgPtr->SdServerServiceCount;
  uint16_least ActiveTimers = (uint16_least)InstanceStatePtr->ServerServicesTimersActive;

#if (SD_DEV_ERROR_DETECT == STD_ON)
  /* this defensive check covers where there might be no configured server services for this instance */
  SD_PRECONDITION_ASSERT( (ServiceIndexEnd > ServiceIndex), SD_SERVERSERVICEMAINFUNCTION_SVCID );
#endif
  do
  {
    /* if timer value is different to 0, it means that the server service is in a state different to DOWN,
      or different to MAIN phase with a timer set to 0 to avoid further sending of OFFERS */
    if(ServerServiceStatePtr->TimeOutDelta > 0U)
    {
#if (SD_DEV_ERROR_DETECT == STD_ON)
      /* this defensive check covers the case of an underflow in the variable TimeOutDelta */
      SD_PRECONDITION_ASSERT( (ActiveTimers > 0U), SD_SERVERSERVICEMAINTIMERS_SVCID );
#endif
      --ActiveTimers;

#if (SD_DEV_ERROR_DETECT == STD_ON)
      /* this defensive check covers the case of an underflow in the variable TimeOutDelta */
      SD_PRECONDITION_ASSERT( (ServerServiceStatePtr->TimeOutDelta >= InstanceStatePtr->ServersDelta), SD_SERVERSERVICEMAINTIMERS_SVCID );
#endif
      /* decrement the last instance server delta */
      ServerServiceStatePtr->TimeOutDelta -= InstanceStatePtr->ServersDelta;
      /* check if this services timer timed out */
      if(ServerServiceStatePtr->TimeOutDelta == 0U)
      { /* this function handles a change of server service state machine due to a timeout */
        Sd_ServerService_ComSM_event_timeout(ServerServiceCfgPtr, ServerServiceStatePtr, ServiceIndex);
      }
      /* else this server services timer did not time out */

      /* TimeOutDelta variable can change in the function "Sd_ServerService_ComSM_event_timeout()" thats why its value is checked
        here again to see if the new calculated timer is bigger than 0 */
      if(ServerServiceStatePtr->TimeOutDelta != 0U)
      {
        if( (InstanceStatePtr->ServerDeltaTimer == 0U) || (ServerServiceStatePtr->TimeOutDelta < InstanceStatePtr->ServerDeltaTimer) )
        { /* set next timeout */
          InstanceStatePtr->ServerDeltaTimer = ServerServiceStatePtr->TimeOutDelta;
        }
        /* else nothing to do here, the new timer for this service is bigger or equal than the next timeout timer */
      }
      /* new timer for this service is equal to 0, a timer is no longer running for this server service */
      else
      {
#if (SD_DEV_ERROR_DETECT == STD_ON)
        /* this defensive check covers the case of an underflow in the variable TimeOutDelta */
        SD_PRECONDITION_ASSERT( (InstanceStatePtr->ServerServicesTimersActive > 0U), SD_SERVERSERVICEMAINTIMERS_SVCID );
#endif
        --InstanceStatePtr->ServerServicesTimersActive;
      }
    }
    /* else do nothing, timer is equal to 0 */

    ++ServerServiceCfgPtr;
    ++ServerServiceStatePtr;
    ++ServiceIndex;
  }
  while( (ActiveTimers > 0U) && (ServiceIndex < ServiceIndexEnd) );
}


/* this function checks if there are FIND messages received server services of this instances.
  FIND messages are answered with an unicast OFFER */
TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ServerService_sendEntriesUnicast
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  uint16 ServersFindsToProcess
)
{
  /* each instance has a configured bitmap for all server services on the instance */
  const uint16 ServiceIndexEnd = InstanceCfgPtr->SdServerService + InstanceCfgPtr->SdServerServiceCount;
  const Sd_RemoteNodeIdType NodeIndexRelative = RemoteNodeIndex - InstanceCfgPtr->RemoteNode;
  uint16 ByteIndex = 0U;
  const uint16 ByteIndexEnd = InstanceCfgPtr->FindReceivedBitmapByteSize;
  P2CONST(SdServerService_t,AUTOMATIC,SD_APPL_CONST) ServerServiceCfgPtr = &SD_CFG_REF(SdServerService_t, SdServerServicesRef, 0U);
  /* set a pointer to the first byte within the configured bitmap for this instance */
  P2VAR(uint8, AUTOMATIC, SD_APPL_DATA) Sd_ServerFindsRcvdGlobBitmapPtr =
      &Sd_ServerFindReceivedGlobalBitmap[InstanceCfgPtr->FindReceivedBitmapStartByte + (InstanceCfgPtr->FindReceivedBitmapByteSize * NodeIndexRelative)];

  do
  { /* each time that the pointer to the bit map is increased eight server services are skipped */
    uint16 ServiceIndex = InstanceCfgPtr->SdServerService + (ByteIndex * 8U);
    uint8 bitmapValue = *Sd_ServerFindsRcvdGlobBitmapPtr;
    *Sd_ServerFindsRcvdGlobBitmapPtr = 0U;
    /* check if value contained in the pointer is different to 0, it will mean that at least one of the eight bit mapped services has received a FIND message */
    while(bitmapValue != 0U)
    { /* check bit by bit which services have received a FIND message */
      if( (bitmapValue & 0x01U) != 0U )
      {
        if(Sd_ServerServiceStates[ServiceIndex].ComPhase > SD_SERVER_SERVICE_COMPHASE_INITIALWAIT)
        {
          P2CONST(SdServerService_t,AUTOMATIC,SD_APPL_DATA) ServiceCfgPtr = &ServerServiceCfgPtr[ServiceIndex];
          Sd_Send_queueOfferService(ServiceCfgPtr, ServiceIndex, FALSE);
        }
        --ServersFindsToProcess;
      }
      /* shift right bits of the bit map so that next bit can be checked */
      bitmapValue >>= 1;
      ++ServiceIndex;
      if(ServiceIndex >= ServiceIndexEnd)
      {
        break;
      }
    } /* loop continues as long as at least one bit of this byte is still set to 1 */
    ++Sd_ServerFindsRcvdGlobBitmapPtr;
    ++ByteIndex;
  } /* loop continues until all received FINDs have been processed or
      until all bytes assigned to this instance have been checked */
  while( (ServersFindsToProcess > 0U) && (ByteIndex < ByteIndexEnd) );
}


TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ServerService_resetRemoteConnection
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  const Sd_RemoteNodeIdType RemoteNodeIndex
)
{

  uint16_least ServiceIndex = InstanceCfgPtr->SdServerService;
  const uint16_least ServiceIndexEnd = ServiceIndex + InstanceCfgPtr->SdServerServiceCount;

  P2CONST(SdServerService_t,AUTOMATIC,SD_APPL_CONST) ServiceConfigPtr = &SD_CFG(SdServerService_t, SdServerServicesRef, ServiceIndex);
  const Sd_RemoteNodeIdType NodeIndexRelative = RemoteNodeIndex - InstanceCfgPtr->RemoteNode;

  while(ServiceIndex < ServiceIndexEnd)
  {
    if(ServiceConfigPtr->SdEventHandlerCount > 0U)
    { /* only if this server service has at least a server service configured */
      uint16_least EventgroupIndex = ServiceConfigPtr->SdEventHandler;
      const uint16_least EventgroupIndexEnd = EventgroupIndex + ServiceConfigPtr->SdEventHandlerCount;
      P2VAR(Sd_ServerSoConStateType, AUTOMATIC, SD_APPL_DATA) ServerSoConState = &Sd_ServerSoConStates[(EventgroupIndex * SD_MAXNUM_INSTANCE_REMOTENODES) + NodeIndexRelative];

#if (SD_DEV_ERROR_DETECT == STD_ON)
      /* this defensive checks a possible out of bound in array Sd_ServerSoConStates[] */
      SD_PRECONDITION_ASSERT( (((EventgroupIndex * SD_MAXNUM_INSTANCE_REMOTENODES) + ((uint16_least)RemoteNodeIndex - InstanceCfgPtr->RemoteNode)) < SD_MAXNUM_SUBSCRIPTIONS), SD_SERVERSERVICERESETREMOTECONNECTION_SVCID );
#endif
      do
      {
        if(ServerSoConState->TTL != SD_TTL_OFF)
        { /* remove only if this event handler had a subscription from the remote node that has just rebooted */
          Sd_ServerService_removeEventGroupSubscriber(&Sd_ServerEventHandlerStates[EventgroupIndex], ServerSoConState, (uint16)EventgroupIndex, FALSE);
        }
        ServerSoConState = &ServerSoConState[SD_MAXNUM_INSTANCE_REMOTENODES];
        ++EventgroupIndex;
      }
      while(EventgroupIndex < EventgroupIndexEnd);
    }
    ++ServiceIndex;
    ++ServiceConfigPtr;
  }

  TS_MemBZero(&Sd_ServerFindReceivedGlobalBitmap[InstanceCfgPtr->FindReceivedBitmapStartByte +  (InstanceCfgPtr->FindReceivedBitmapByteSize * NodeIndexRelative)], InstanceCfgPtr->FindReceivedBitmapByteSize);
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ServerService_startServices
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr
)
{
  uint16_least ServiceIndex = InstanceCfgPtr->SdServerService;
  const uint16_least ServiceIndexEnd = ServiceIndex + InstanceCfgPtr->SdServerServiceCount;

  while(ServiceIndex < ServiceIndexEnd)
  {
    CONSTP2VAR(Sd_ServerServiceStateType,AUTOMATIC,SD_APPL_DATA) ServerServiceStatePtr = &Sd_ServerServiceStates[ServiceIndex];

    if(ServerServiceStatePtr->ServerServiceState == SD_SERVER_SERVICE_AVAILABLE)
    {
      ServerServiceStatePtr->EventFlags |= SD_SERVER_EVENT_START;
      ++InstanceStatePtr->ServerServicesEventsCount;
    }
    ++ServiceIndex;
  }
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ServerService_haltServices
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr
)
{
  uint16_least ServiceIndex = InstanceCfgPtr->SdServerService;
  const uint16_least ServiceIndexEnd = ServiceIndex + InstanceCfgPtr->SdServerServiceCount;
  P2VAR(Sd_ServerServiceStateType,AUTOMATIC,SD_APPL_DATA) ServerServiceStatePtr = &Sd_ServerServiceStates[ServiceIndex];

  while(ServiceIndex < ServiceIndexEnd)
  {
    if(ServerServiceStatePtr->ComPhase != SD_SERVER_SERVICE_COMPHASE_DOWN)
    {
      ServerServiceStatePtr->EventFlags = SD_SERVER_EVENT_HALT;
      ++InstanceStatePtr->ServerServicesEventsCount;
    }
    else
    {
      if( (ServerServiceStatePtr->EventFlags & SD_SERVER_EVENT_START) != 0U )
      {
        --InstanceStatePtr->ServerServicesEventsCount;
        ServerServiceStatePtr->EventFlags = 0U;
      }
    }

    ++ServiceIndex;
    ++ServerServiceStatePtr;
  }
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ServerService_eventFind
(
  CONSTP2CONST(Sd_ServiceInfoType, AUTOMATIC, SD_APPL_DATA) ServiceInfoPtr,
  CONSTP2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfoPtr,
  const uint32 MinorVersion,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  const uint8 SdInstanceIndex
)
{
  P2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr =  &SD_CFG(SdInstance_t, SdInstancesRef, SdInstanceIndex);

  if(ServiceInfoPtr->InstanceID == SD_WILDCARD_INSTANCEID)
  {
    /* Instance ID is a wild card, classic search algorithm is going to be used since this FIND message
      might have several server services as targets */
    uint16_least ServiceIndex = InstanceCfgPtr->SdServerService;
    const uint16_least ServiceIndexEnd = ServiceIndex + InstanceCfgPtr->SdServerServiceCount;
    P2CONST(SdServerService_t, AUTOMATIC, SD_APPL_DATA) ServiceCfgPtr = &SD_CFG(SdServerService_t, SdServerServicesRef, ServiceIndex);
    P2VAR(Sd_RemoteNodeStateType,AUTOMATIC,SD_APPL_DATA) RemoteNodeStatePtr = Sd_GetRemoteNodeState(RemoteNodeIndex);
    const uint16 ByteOffset = ( InstanceCfgPtr->FindReceivedBitmapStartByte +
                                (InstanceCfgPtr->FindReceivedBitmapByteSize * ((uint16)RemoteNodeIndex - InstanceCfgPtr->RemoteNode)) );
    uint16 ServiceIndexRelative = 0U;

    while(ServiceIndex < ServiceIndexEnd)
    {
      if(Sd_ServerService_match(ServiceInfoPtr, EntryOptionInfoPtr, ServiceCfgPtr, MinorVersion))
      {
        Sd_ServerFindReceivedGlobalBitmap[ByteOffset + (ServiceIndexRelative / 8U)] |= (uint8)(0x01U << (ServiceIndexRelative % 8U));

        if( (RemoteNodeStatePtr->IsConnectionReady == FALSE) && (RemoteNodeStatePtr->IsConnectionReadyTimer == 0U) )
        {
          RemoteNodeStatePtr->IsConnectionReadyTimer = (8U * SD_LOOPS_PER_SECOND);
        }

        if(RemoteNodeStatePtr->ResponseTimer == 0U)
        {
          P2CONST(SdServerTimer_t, AUTOMATIC, SD_APPL_DATA) TimerCfgPtr = &SD_CFG(SdServerTimer_t, SdServerTimersRef, ServiceCfgPtr->SdServerServiceTimerIndex);

          Sd_RestartNodeResponseTimer
          (
            Sd_GetRandom(TimerCfgPtr->SdServerTimerRequestResponseMinDelay,
                         TimerCfgPtr->SdServerTimerRequestResponseMaxDelay) + 1U,
            RemoteNodeIndex,
            SdInstanceIndex
          );
        }
        ++RemoteNodeStatePtr->ServerServicesFindsReceived;
      }
      ++ServiceIndex;
      ++ServiceIndexRelative;
      ++ServiceCfgPtr;
    }
  }
  else /* no Wilcard present, so binary search algorithm can be used */
  {
    /* search for a server service that matches exactly the end-point received in EntryOptionInfoPtr */
    uint16 ServiceIndex = Sd_ServerService_QuickMatch(ServiceInfoPtr, EntryOptionInfoPtr, MinorVersion, SdInstanceIndex);

    if(ServiceIndex != SD_WILDCARD_SERVICEID)
    {  /* if returned value of ServiceIndex variable is a valid value (any value different from SD_WILDCARD_SERVICEID) then it means that
        * and exact match has been found */

      P2VAR(Sd_RemoteNodeStateType,AUTOMATIC,SD_APPL_DATA) RemoteNodeStatePtr = Sd_GetRemoteNodeState(RemoteNodeIndex);

      const Sd_RemoteNodeIdType NodeIndexRelative = RemoteNodeIndex - InstanceCfgPtr->RemoteNode;
      const uint16 ServiceIndexRelative = (uint16)ServiceIndex - InstanceCfgPtr->SdServerService;
      const uint16 ByteOffset = InstanceCfgPtr->FindReceivedBitmapStartByte +
                                (InstanceCfgPtr->FindReceivedBitmapByteSize * NodeIndexRelative) +
                                (ServiceIndexRelative / 8U);

      Sd_ServerFindReceivedGlobalBitmap[ByteOffset] |= (uint8)(0x01U << (ServiceIndexRelative % 8U));

      if( (RemoteNodeStatePtr->IsConnectionReady == FALSE) && (RemoteNodeStatePtr->IsConnectionReadyTimer == 0U) )
      {
        RemoteNodeStatePtr->IsConnectionReadyTimer = (8U * SD_LOOPS_PER_SECOND);
      }

      if(RemoteNodeStatePtr->ResponseTimer == 0U)
      { /* a FIND message has been received and processed, get a random response time value (between the configured min and max response time)
          and set this response timer */
        CONSTP2CONST(SdServerService_t, AUTOMATIC, SD_APPL_DATA) ServiceCfgRefPtr = &SD_CFG_REF(SdServerService_t, SdServerServicesRef, 0U);
        P2CONST(SdServerTimer_t, AUTOMATIC, SD_APPL_DATA) ServerServiceTimerRef =
            &SD_CFG(SdServerTimer_t, SdServerTimersRef, ServiceCfgRefPtr[ServiceIndex].SdServerServiceTimerIndex);

        Sd_RestartNodeResponseTimer
        (
          Sd_GetRandom(ServerServiceTimerRef->SdServerTimerRequestResponseMinDelay, ServerServiceTimerRef->SdServerTimerRequestResponseMaxDelay) + 1U,
          RemoteNodeIndex,
          SdInstanceIndex
        );
      }
      ++RemoteNodeStatePtr->ServerServicesFindsReceived;
    }
  }
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ServerService_eventSubscribe
(
  CONSTP2CONST(Sd_ServiceInfoType, AUTOMATIC, SD_APPL_DATA) ServiceInfoPtr,
  CONSTP2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfoPtr,
  const uint32 TTL,
  const uint16 ReservedField,
  const uint16 EventgroupID,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  const uint8 SdInstanceIndex
)
{
  if( (ServiceInfoPtr->ServiceID != SD_WILDCARD_SERVICEID) &&
      (ServiceInfoPtr->InstanceID != SD_WILDCARD_INSTANCEID) &&
      (ServiceInfoPtr->MajorVersion != SD_WILDCARD_MAJORVERSION) &&
      (EventgroupID != SD_WILDCARD_EVENTGROUPID)
    )
  {
    /* search for a server service that matches exactly the end-point received in EntryOptionInfoPtr */
    const uint16 ServiceIndex = Sd_ServerService_QuickMatch(ServiceInfoPtr, EntryOptionInfoPtr, SD_WILDCARD_MINORVERSION, SdInstanceIndex);

    if(ServiceIndex != SD_WILDCARD_SERVICEID)
    {  /* if returned value of ServiceIndex variable is a valid value (any value different from SD_WILDCARD_SERVICEID) then it means that
        * an exact match has been found */
      P2CONST(SdServerService_t, AUTOMATIC, SD_APPL_DATA) ServiceCfgPtr = &SD_CFG(SdServerService_t, SdServerServicesRef, ServiceIndex);
      uint16_least EventHandlerIndex = ServiceCfgPtr->SdEventHandler;
      Std_ReturnType RetVal = E_NOT_OK;

      if(Sd_ServerServiceStates[ServiceIndex].ComPhase > SD_SERVER_SERVICE_COMPHASE_INITIALWAIT)
      {
        const uint16_least EventHandlerIndexEnd = EventHandlerIndex + ServiceCfgPtr->SdEventHandlerCount;
        P2CONST(SdEventHandler_t, AUTOMATIC, SD_APPL_CONST) EventHandlerCfgPtr = &SD_CFG(SdEventHandler_t, SdEventHandlersRef, EventHandlerIndex);

        /* look for an event handler Id match along all event handler configured for this server service */
        while(EventHandlerIndex < EventHandlerIndexEnd)
        { /* if a match occurs */
          if (EventHandlerCfgPtr->SdEventHandlerEventGroupId == EventgroupID)
          { /* if this SusbcribeEventgroup entry does not contain any end-point option (neither UDP nor TCP) it means that this event handler
              is going to communicate its event over its multicast connection reference */
            if( (EventHandlerCfgPtr->SdEventHandlerMulticastThreshold == 1U) && (ServiceCfgPtr->SdServerServiceActivationRef == SOAD_INVALID_ROUTINGGROUP_ID) )
            {
              Sd_ServerService_addMulticastEventGroupSubscriber(
                  TTL,
                  (uint16)EventHandlerIndex,
                  RemoteNodeIndex,
                  ((uint8)ReservedField & SD_ENTRYT2_COUNTER_MASK),
                  SdInstanceIndex);

              RetVal = E_OK;
            }
            else
            { /* event handler match found, then it is checked that there is at least and UDP or a TCP option is contained in the message */
              if( (EntryOptionInfoPtr->UdpEndpointOption.domain != TCPIP_AF_UNSPEC) || (EntryOptionInfoPtr->TcpEndpointOption.domain != TCPIP_AF_UNSPEC) )
              {
                RetVal = Sd_ServerService_addEventGroupSubscriber(
                    /* Deviation MISRAC2012-1 */
                    (TcpIp_SockAddrType*)(void*)&EntryOptionInfoPtr->UdpEndpointOption,
                    (TcpIp_SockAddrType*)(void*)&EntryOptionInfoPtr->TcpEndpointOption,
                    TTL,
                    ServiceIndex,
                    (uint16)EventHandlerIndex,
                    RemoteNodeIndex,
                    SdInstanceIndex);
              }
            }
            /* else, nothing to do, a multicast only subscription has been received by a non multicat only server service */
            break;
          }
          ++EventHandlerIndex;
          ++EventHandlerCfgPtr;
        }
      }

      if(RetVal == E_OK)
      {
        Sd_Send_queueSubscribeEventgroupAck(TTL, ServiceIndex, (uint16)EventHandlerIndex, ReservedField);
      }
      else
      {
        Sd_Send_queueSubscribeEventgroupNack(ServiceIndex, ReservedField, EventgroupID);
      }
    }

    if(ServiceIndex == SD_WILDCARD_SERVICEID)
    { /* if service index counter has reached the maximum value it means that service that the subscription message wanted to subscribe is not
       * configured on the list of server services of this instance */
      Sd_Send_queueSubscribeEventgroupNackUnknown(
        ServiceInfoPtr->ServiceID,
        ServiceInfoPtr->InstanceID,
        EventgroupID,
        ReservedField,
        ServiceInfoPtr->MajorVersion
      );
    }
  }
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ServerService_eventStopSubscribe
(
  CONSTP2CONST(Sd_ServiceInfoType, AUTOMATIC, SD_APPL_DATA) ServiceInfoPtr,
  CONSTP2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfoPtr,
  const uint16 ReservedField,
  const uint16 EventgroupID,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  const uint8 SdInstanceIndex
)
{
  TS_PARAM_UNUSED(ReservedField);

  if( (ServiceInfoPtr->ServiceID != SD_WILDCARD_SERVICEID) &&
      (ServiceInfoPtr->InstanceID != SD_WILDCARD_INSTANCEID) &&
      (ServiceInfoPtr->MajorVersion != SD_WILDCARD_MAJORVERSION) &&
      (EventgroupID != SD_WILDCARD_EVENTGROUPID) )
  {
    /* search for a server service that matches exactly the end-point received in EntryOptionInfoPtr */
    const uint16 ServiceIndex = Sd_ServerService_QuickMatch(ServiceInfoPtr, EntryOptionInfoPtr, SD_WILDCARD_MINORVERSION, SdInstanceIndex);

    if(ServiceIndex != SD_WILDCARD_SERVICEID)
    { /* A server service can only receive a stop subscribe to a subscribe it had sent before, to receive a subscribe it should have send a previous OFFER,
        only if a server service is in a state higher than initial wait would have sent an OFFER */
      if(Sd_ServerServiceStates[ServiceIndex].ComPhase > SD_SERVER_SERVICE_COMPHASE_INITIALWAIT)
      { /* if returned value of ServiceIndex variable is a valid value (any value different from SD_WILDCARD_SERVICEID) then it means that
         * and exact match has been found, now a event group index with ID equal to EventgroupID it is searched within this server service */
        P2CONST(SdServerService_t, AUTOMATIC, SD_APPL_DATA) ServiceCfgPtr = &SD_CFG(SdServerService_t, SdServerServicesRef, ServiceIndex);
        P2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr =  &SD_CFG(SdInstance_t, SdInstancesRef, SdInstanceIndex);
        {
          uint16_least EventHandlerIndex = ServiceCfgPtr->SdEventHandler;
          const uint16_least EventHandlerIndexEnd = EventHandlerIndex + ServiceCfgPtr->SdEventHandlerCount;
          P2CONST(SdEventHandler_t, AUTOMATIC, SD_APPL_CONST) EventHandlerCfgPtr = &SD_CFG(SdEventHandler_t, SdEventHandlersRef, EventHandlerIndex);

          /* look for an event handler Id match along all event handler configured for this server service */
          while(EventHandlerIndex < EventHandlerIndexEnd)
          { /* if a match occurs */
            if (EventHandlerCfgPtr->SdEventHandlerEventGroupId == EventgroupID)
            {
              P2VAR(Sd_ServerSoConStateType, AUTOMATIC, SD_APPL_DATA) ServerSoConState =
                              &Sd_ServerSoConStates[(EventHandlerIndex * SD_MAXNUM_INSTANCE_REMOTENODES) +
                                                    ((uint16_least)RemoteNodeIndex - InstanceCfgPtr->RemoteNode)];
              /* make sure before going on that there was a subscriber with this remote index for this event group */
              if(ServerSoConState->TTL != SD_TTL_OFF)
              { /* event handler match found, then it is checked that there is at least and UDP or a TCP option is contained in the message */
                if(   ( (EntryOptionInfoPtr->UdpEndpointOption.domain != TCPIP_AF_UNSPEC) && (ServerSoConState->SockConIdUdp != SOAD_INVALID_SOCON_ID) )
                   || ( (EntryOptionInfoPtr->TcpEndpointOption.domain != TCPIP_AF_UNSPEC) && (ServerSoConState->SockConIdTcp != SOAD_INVALID_SOCON_ID) )
                  /* OR if this SusbcribeEventgroup entry does not contain any end-point option (neither UDP nor TCP) and if the event handler for which the subscription
                    has been received has a Multicast threshold set to 1, it means that this event group is Multicast only */
                   || (   (EventHandlerCfgPtr->SdEventHandlerMulticastThreshold == 1U)
                       && (EntryOptionInfoPtr->UdpEndpointOption.domain == TCPIP_AF_UNSPEC)
                       && (EntryOptionInfoPtr->TcpEndpointOption.domain == TCPIP_AF_UNSPEC)   )   )
                {
                  Sd_ServerService_removeEventGroupSubscriber(&Sd_ServerEventHandlerStates[EventHandlerIndex], ServerSoConState, (uint16)EventHandlerIndex, FALSE);
                }
              }
              break;
            }
            ++EventHandlerIndex;
            ++EventHandlerCfgPtr;
          }
        }
      }
    }
  }
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ServerService_handleTTL
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr
)
{
  uint16_least ServiceIndex = InstanceCfgPtr->SdServerService;
  const uint16_least ServiceIndexEnd = ServiceIndex + InstanceCfgPtr->SdServerServiceCount;
  P2CONST(SdServerService_t, AUTOMATIC, SD_APPL_DATA) ServiceCfgPtr = &SD_CFG(SdServerService_t, SdServerServicesRef, ServiceIndex);

  do
  {
    P2VAR(Sd_ServerSoConStateType,AUTOMATIC,SD_CODE) ConnStatePtr;
    uint16_least EventgroupIndex = ServiceCfgPtr->SdEventHandler;
    const uint16_least EventgroupIndexEnd = EventgroupIndex + ServiceCfgPtr->SdEventHandlerCount;
    P2VAR(Sd_EventHandlerStateType,AUTOMATIC,SD_APPL_DATA) EventHandlerStatePtr = &Sd_ServerEventHandlerStates[EventgroupIndex];
    const Sd_RemoteNodeIdType RemoteNodeIndexEnd = InstanceCfgPtr->RemoteNodeCount;

    while(EventgroupIndex < EventgroupIndexEnd)
    {
      uint16 ClientCount = EventHandlerStatePtr->ClientCount;

      if(ClientCount > 0U)
      {
        SoAd_SoConIdType NodeIndex = 0U;
        ConnStatePtr = &Sd_ServerSoConStates[(EventgroupIndex * SD_MAXNUM_INSTANCE_REMOTENODES)];

        do
        { /* check if this TTL is active */
          if(ConnStatePtr->TTL >= SD_TTL_ALWAYS_ON)
          {
            --ClientCount;

            if(ConnStatePtr->TTL > SD_TTL_ALWAYS_ON)
            {
#if (SD_DEV_ERROR_DETECT == STD_ON)
              /* this defensive check covers the case of an underflow in the variable TTL */
              SD_PRECONDITION_ASSERT( (ConnStatePtr->TTL >= InstanceStatePtr->ServersDeltaTTL), SD_SERVERSERVICEHANDLETTL_SVCID );
#endif
              ConnStatePtr->TTL -= (InstanceStatePtr->ServersDeltaTTL - SD_TTL_TIMED_OUT);

              if (ConnStatePtr->TTL <= SD_TTL_TIMED_OUT)
              {
                Sd_ServerService_removeEventGroupSubscriber(EventHandlerStatePtr, ConnStatePtr, (uint16)EventgroupIndex, FALSE);
              }
              else  /* this nodes TTL did not time out, update its TTL time out */
              { /* this is not the timer that timed out, it has to be updated */
                if( (InstanceStatePtr->ServersDeltaTTLtimer <= SD_TTL_ALWAYS_ON) || (InstanceStatePtr->ServersDeltaTTLtimer > ConnStatePtr->TTL) )
                { /* set next TTL timeout */
                  InstanceStatePtr->ServersDeltaTTLtimer = ConnStatePtr->TTL;
                }
              }
            }
            /* else TTL not running, check next remote nodes TTL */
          }
          ++ConnStatePtr;
          ++NodeIndex;
        }
        while( (ClientCount > 0U) && (NodeIndex < RemoteNodeIndexEnd) );
      }
      ++EventgroupIndex;
      ++EventHandlerStatePtr;
    }
    ++ServiceCfgPtr;
    ++ServiceIndex;
  }
  while(ServiceIndex < ServiceIndexEnd);
}



/*==================[internal function definitions]=========================*/

STATIC FUNC(boolean, SD_CODE) Sd_ServerService_match
(
  CONSTP2CONST(Sd_ServiceInfoType, AUTOMATIC, SD_APPL_DATA) ServiceInfoPtr,
  CONSTP2CONST(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfoPtr,
  CONSTP2CONST(SdServerService_t, AUTOMATIC, SD_APPL_DATA) ServiceCfgPtr,
  const uint32 MinorVersion
)
{
  boolean Match = FALSE;

  if(ServiceCfgPtr->SdServerServiceID == ServiceInfoPtr->ServiceID)
  {
    /* check if the MajorVersion is identical or a wildcard is used in the request */
    if( (ServiceInfoPtr->MajorVersion == SD_WILDCARD_MAJORVERSION) ||
        (ServiceCfgPtr->SdServerServiceMajorVersion == ServiceInfoPtr->MajorVersion) )
    {
      if( (MinorVersion == SD_WILDCARD_MINORVERSION) ||
          (ServiceCfgPtr->SdServerServiceMinorVersion == MinorVersion) )
      {
        SdConfigOptionPtrType ServiceConfigStringRefPtr = &SD_CFG_REF(uint8, SdConfigStringRef, 0U);
        Match = Sd_CapabilityRecordStrCmp(EntryOptionInfoPtr->ConfigOptions,
                                          &ServiceConfigStringRefPtr[ServiceCfgPtr->SdConfigurationStringIndex],
                                          ServiceCfgPtr->SdServerServiceConfigurationStringLength);
      }
    }
  }
  return Match;
}


/* this function uses the search algorithm known as binary search or half-interval search within an array of elements
 * WHICH HAVE BEEN PREVIOUSLY REARRANGED or SORTED in ascending order:
 *
 * It calculates the bottom most left index of the array and the top most right since the number of elements present in the
 * array is known. Then from this two values the middle index is calculated and the content of the array in this middle index
 * compared to the target value.
 * If target value is smaller then the top half part of the array is already discarded, if target value is bigger then it is
 * the bottom half part of the array is discarded. Then, the same operation is successively carried out on the remaining
 * half of the array. That is, a new middle index value is calculated and the same comparison over and over again until either
 * two or one value are left to compare (worst case).
 */
STATIC FUNC(uint16, SD_CODE) Sd_ServerService_QuickMatch
(
  CONSTP2CONST(Sd_ServiceInfoType, AUTOMATIC, SD_APPL_DATA) ServiceInfoPtr,
  CONSTP2CONST(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfoPtr,
  const uint32 MinorVersion,
  const uint8 SdInstanceIndex
)
{
  P2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr = &SD_CFG_REF(SdInstance_t, SdInstancesRef, 0U);
  /* variable to be returned with the matched server service index is initialized here to an invalid value */
  uint16 ServiceHandleId = SD_WILDCARD_SERVICEID;

  InstanceCfgPtr = &InstanceCfgPtr[SdInstanceIndex];
  /* only if there are server services defined in this instance */
  if(InstanceCfgPtr->SdServerServiceCount > 0U)
  { /* bottom left index is the first element index */
    const uint16 ServiceIndexStart = InstanceCfgPtr->SdServerService;
    /* top right index is the last element index, that is the bottom left index plus the number of elements minus 1,
     * since it is an index value. */
    const uint16 ServiceIndexEnd = ServiceIndexStart + (InstanceCfgPtr->SdServerServiceCount - 1U);

    CONSTP2CONST(SdServerService_t, AUTOMATIC, SD_APPL_DATA) ServiceCfgRefPtr = &SD_CFG_REF(SdServerService_t, SdServerServicesRef, 0U);
    CONSTP2CONST(SdServiceIndex_t, AUTOMATIC, SD_APPL_DATA) ServiceIndexCfgPtr = &SD_CFG_REF(SdServiceIndex_t, SdServerServiceIndexRef, 0U);
    SdConfigOptionPtrType ServiceConfigStringRefPtr = &SD_CFG_REF(uint8, SdConfigStringRef, 0U);
    P2CONST(SdServerService_t, AUTOMATIC, SD_APPL_DATA) ServiceCfgPtr;
    /* target value service ID/instance ID is calculated here */
    const uint32 targetCombinedId = ((uint32)ServiceInfoPtr->ServiceID * 0x10000U) + (uint32)ServiceInfoPtr->InstanceID;
    uint32 middleCombinedId, middleIdx;
    uint16_least ServiceIndexCurrent;
    boolean TotalMatch = FALSE;

    /* binary search algorithm starts here */
      /* copy bottom left index value into a local variable */
    uint32 left = ServiceIndexStart;
    /* copy top right index value into a local variable */
    uint32 right = ServiceIndexEnd;

    /* calculate middle value, note that + 1 is added as a convention because on the second part of the search
     * algorithm (after do -while loop) the search for a match to the left starts with current element - 1 */
    middleIdx = (left + right + 1U) / 2U;

    /* if both left and right have the same value, that means there is only
      an element in the in the original array, so it is not necessary to execute this loop */
    if (left != right)
    {
      do
      {
        /* calculate the value contained in the index that is the middle value of the remaining part of the array
         * (or the whole array if it is the first occurrence of this loop) */
        middleCombinedId = ServiceIndexCfgPtr[middleIdx].SdServiceCombinedId;

        /* compare target value with the value in the middle of the remaining array */
        if (middleCombinedId == targetCombinedId)
        {  /* target value service ID/instance ID has been found, this is just a partial match */
           break;
        }
        /* no match, check if target value is smaller than the value in the middle of the array */
        if (targetCombinedId < middleCombinedId)
        { /* target value is smaller, then move top right index value to be the middle index value minus one,
            because now it is known that target value is in the bottom left half of the previous remaining array */
          right = (middleIdx - 1U);
        }
        else
        { /* else, target value is bigger, then move bottom left index value to be the middle index value plus one,
           because now it is certain that target value is in the top right half of the previous remaining array */
          left = (middleIdx + 1U);
        }
        /* now that either the bottom left index or the top right index have been modified to fit the new remaining part
         * of the array, the middle index value is calculated once more */
        middleIdx = (left + right + 1U) / 2U;
      }
      while (left < right);
    }

    /* here begins the second part of the search array, until now the binary search has concluded and middleServiceId contains the
     * index that this binary search algorithm has yielded.
     */

    /* search for an exact match on the top right half of the remaining array */
    for(ServiceIndexCurrent = (uint16_least)middleIdx;
        ( (TotalMatch == FALSE) && (ServiceIndexCurrent <= ServiceIndexEnd) );
        ++ServiceIndexCurrent)
    {
      /* keep looking for a total match as long as current element matches partially (service ID/instance ID)
        and the element has not been totally matched */

      if(ServiceIndexCfgPtr[ServiceIndexCurrent].SdServiceCombinedId != targetCombinedId)
      { /* this element does not match partially, break for loop to look for a total match on the bottom half part of the array */
        break;
      }
      /* this elements matches partially (service ID/instance ID), check it is matches totally */

      ServiceHandleId = ServiceIndexCfgPtr[ServiceIndexCurrent].SdServiceHandleId;
      ServiceCfgPtr = &ServiceCfgRefPtr[ServiceHandleId];

      /* check if the MajorVersion is also identical and configuration array are identical to conform a total match */
      if( (ServiceCfgPtr->SdServerServiceMajorVersion == ServiceInfoPtr->MajorVersion) ||
          (ServiceInfoPtr->MajorVersion == SD_WILDCARD_MAJORVERSION) )
      {
        if( (MinorVersion == SD_WILDCARD_MINORVERSION) ||
            (ServiceCfgPtr->SdServerServiceMinorVersion == MinorVersion) )
        {
          TotalMatch = Sd_CapabilityRecordStrCmp(EntryOptionInfoPtr->ConfigOptions,
                                                 &ServiceConfigStringRefPtr[ServiceCfgPtr->SdConfigurationStringIndex],
                                                 ServiceCfgPtr->SdServerServiceConfigurationStringLength);
        }
      }
    }

    /* the search for a total match on the bottom left half of the remaining array is only possible if
     * the middle ID is bigger than 0, otherwise there just no bottom half of the array.
     * THIS CHECK IS IMPORTANT SINCE THERE IS A RISK OF UNDERFLOW HERE
     */
    if(middleIdx != 0U)
    {
      /* search for an exact match on the bottom left half of the remaining array starting from the middle ID minus one
        (note here that this -1 is connected to the fact that the middle Id value is calculated using a + 1 in the end */
      for(ServiceIndexCurrent = (uint16_least)middleIdx - 1U;
          ( (TotalMatch == FALSE) && (ServiceIndexCurrent >= ServiceIndexStart) );
          --ServiceIndexCurrent)
      {
        /* keep looking for a total match as long as current element matches partially (service ID/instance ID)
          and the element has not been totally matched */

        if(ServiceIndexCfgPtr[ServiceIndexCurrent].SdServiceCombinedId == targetCombinedId)
        {
          /* this elements matches partially (service ID/instance ID), check it is matches totally */

          ServiceHandleId = ServiceIndexCfgPtr[ServiceIndexCurrent].SdServiceHandleId;
          ServiceCfgPtr = &ServiceCfgRefPtr[ServiceHandleId];

          /* check if the MajorVersion is also identical and configuration array are identical to conform a total match */
          if( (ServiceCfgPtr->SdServerServiceMajorVersion == ServiceInfoPtr->MajorVersion) ||
               (ServiceInfoPtr->MajorVersion == SD_WILDCARD_MAJORVERSION) )
          {
            if( (MinorVersion == SD_WILDCARD_MINORVERSION) ||
                (ServiceCfgPtr->SdServerServiceMinorVersion == MinorVersion) )
            {
              TotalMatch = Sd_CapabilityRecordStrCmp(EntryOptionInfoPtr->ConfigOptions,
                                                     &ServiceConfigStringRefPtr[ServiceCfgPtr->SdConfigurationStringIndex],
                                                     ServiceCfgPtr->SdServerServiceConfigurationStringLength);
            }
          }
        }
        /* RISK OF UNDERFLOW HERE */
        /* this index variable is decremented ONLY IF its value is bigger than 0
          otherwise for loop is quit, array is finished on the bottom left half */
        if(ServiceIndexCurrent == 0U)
        {
          break;
        }
      }
    }

    /* if no total match has been found, an invalid service index value is returned */
    if(TotalMatch == FALSE)
    {
      ServiceHandleId = SD_WILDCARD_SERVICEID;
    }
  }

  return ServiceHandleId;
}



STATIC FUNC(void, SD_CODE) Sd_ServerService_Set_State
(
  const uint16 ServiceIndex,
  const Sd_ServerServiceSetStateType state
)
{
  CONSTP2VAR(Sd_ServerServiceStateType,AUTOMATIC,SD_APPL_DATA) ServerServiceStatePtr = &Sd_ServerServiceStates[ServiceIndex];
  /* check if state to be set has not been set yet */
  if(ServerServiceStatePtr->ServerServiceState != state)
  {
    CONSTP2CONST(SdServerService_t, AUTOMATIC, SD_APPL_DATA) ServiceCfgRefPtr = &SD_CFG_REF(SdServerService_t, SdServerServicesRef, 0U);
    const uint8 InstanceIndex = ServiceCfgRefPtr[ServiceIndex].SdInstanceIndex;
    /* new server service state is set */
    ServerServiceStatePtr->ServerServiceState = state;

    /* now it can only be continued with the processing of this request to change the state is this instance has its IP Address and port assigned */
    if(Sd_IsInstanceAddressAssigned(InstanceIndex))
    { /* this server service is made available, set the flag to indicate it so that it can be started in the next main cycle */
      if (state == SD_SERVER_SERVICE_AVAILABLE)
      {
        ServerServiceStatePtr->EventFlags |= SD_SERVER_EVENT_START;
        /* increase the number of events to be processed in the main cycle */
        ++Sd_InstanceStates[InstanceIndex].ServerServicesEventsCount;
      }
      else /* this server service is being made as no longer available, it is going to be set to down state, disabled */
      { /* just in case check for the possibility in which this DOWN command comes straight after an AVAILABLE command,
          before this AVAILABLE command has been processed in the main cycle */
        if( (ServerServiceStatePtr->EventFlags & SD_SERVER_EVENT_START) == 0U )
        { /* this DOWN command does not come straight after an AVAILABLE command that has not been processed yet */
          ++Sd_InstanceStates[InstanceIndex].ServerServicesEventsCount;
        }
        ServerServiceStatePtr->EventFlags = SD_SERVER_EVENT_STOP;
      }
    }
  } /* else, this command is redundant, server service to be set has already been set, no action */
}



STATIC FUNC(void, SD_CODE) Sd_ServerService_addMulticastEventGroupSubscriber
(
  const uint32 TTL,
  uint16 EventgroupIndex,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  const uint8 counter,
  const uint8 InstanceIndex
)
{
  /* no unicast socket connection group is configured for this event group handler */
  P2CONST(SdEventHandler_t, AUTOMATIC, SD_APPL_CONST) EventHandlerCfgPtr = &SD_CFG(SdEventHandler_t, SdEventHandlersRef, EventgroupIndex);
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgRefPtr = &SD_CFG_REF(SdInstance_t, SdInstancesRef, 0U);
  CONSTP2VAR(Sd_ServerSoConStateType,AUTOMATIC,SD_CODE) ServerSoConStatesPtr =
      &Sd_ServerSoConStates[(EventgroupIndex * SD_MAXNUM_INSTANCE_REMOTENODES) + (RemoteNodeIndex - InstanceCfgRefPtr[InstanceIndex].RemoteNode)];

#if (SD_DEV_ERROR_DETECT == STD_ON)
  /* this defensive checks a possible out of bound in array Sd_ServerSoConStates[] */
  SD_PRECONDITION_ASSERT( (((EventgroupIndex * SD_MAXNUM_INSTANCE_REMOTENODES) + ((uint16)RemoteNodeIndex - InstanceCfgRefPtr[InstanceIndex].RemoteNode)) < SD_MAXNUM_SUBSCRIPTIONS), SD_SERVERSERVICEADDMULTEVENTGROUPSUBS_SVCID );
#endif

  TS_PARAM_UNUSED(counter);

  if(ServerSoConStatesPtr->TTL == SD_TTL_OFF)
  {
    ServerSoConStatesPtr->SockConIdUdp = EventHandlerCfgPtr->SdMulticastEventSoConRef;

    if(Sd_ServerEventHandlerStates[EventgroupIndex].ClientCount == 0U)
    {
      /* Subscribe first client. Report to BswM */
      BswM_Sd_EventHandlerCurrentState(EventgroupIndex, SD_EVENT_HANDLER_REQUESTED);

      /* The return value can be ignored as the only possible return value is E_OK */
      (void)SoAd_EnableSpecificRouting(EventHandlerCfgPtr->SdEventHandlerMulticastActivationRef, EventHandlerCfgPtr->SdMulticastEventSoConRef);
    }
    else
    {
      /* multicast is already opened. nothing to do */
    }
    ++Sd_ServerEventHandlerStates[EventgroupIndex].ClientCount;
  }
  if(TTL == SD_ENTRYTTL_INFINITE)
  {
    ServerSoConStatesPtr->TTL = SD_TTL_ALWAYS_ON;
  }
  else
  {
    CONSTP2VAR(Sd_InstanceStateType, AUTOMATIC, SD_APPL_DATA) InstanceStatesPtr = &Sd_InstanceStates[InstanceIndex];

    ServerSoConStatesPtr->TTL = (TTL * SD_LOOPS_PER_SECOND) + 1U;

    if( (InstanceStatesPtr->ServersDeltaTTLtimer <= SD_TTL_ALWAYS_ON) || (ServerSoConStatesPtr->TTL < InstanceStatesPtr->ServersDeltaTTLtimer) )
    {
      InstanceStatesPtr->ServersDeltaTTLtimer = ServerSoConStatesPtr->TTL;
      InstanceStatesPtr->ServersDeltaTTL = ServerSoConStatesPtr->TTL;
    }
    else /* else align this TTL so that it is calculated correctly in the next time out */
    {
      ServerSoConStatesPtr->TTL += (InstanceStatesPtr->ServersDeltaTTL - InstanceStatesPtr->ServersDeltaTTLtimer);
    }
  }
}



STATIC FUNC(Std_ReturnType, SD_CODE) Sd_ServerService_addEventGroupSubscriber
(
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, SD_APPL_DATA) UdpSockAddrPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, SD_APPL_DATA) TcpSockAddrPtr,
  const uint32 TTL,
  const uint16 ServiceIndex,
  uint16 EventgroupIndex,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  const uint8 InstanceIndex
)
{
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgRefPtr = &SD_CFG_REF(SdInstance_t, SdInstancesRef, 0U);
  CONSTP2VAR(Sd_ServerSoConStateType, AUTOMATIC, SD_APPL_DATA) ServerSoConState =
      &Sd_ServerSoConStates[(EventgroupIndex * SD_MAXNUM_INSTANCE_REMOTENODES) + (RemoteNodeIndex - InstanceCfgRefPtr[InstanceIndex].RemoteNode)];
  P2CONST(SdServerService_t, AUTOMATIC, SD_APPL_CONST) ServiceCfgPtr = &SD_CFG(SdServerService_t, SdServerServicesRef, ServiceIndex);
  P2CONST(SdEventHandler_t, AUTOMATIC, SD_APPL_CONST) EventHandlerCfgPtr = &SD_CFG(SdEventHandler_t, SdEventHandlersRef, EventgroupIndex);
  CONSTP2CONST(SdSocketConnectionGroupType, AUTOMATIC, SD_APPL_DATA) SdSoConGroupsCfgRefPtr = &SD_CFG_REF(SdSocketConnectionGroupType, SdSoConGroupsRef, 0U);
  SoAd_SoConIdType SockConId;
  Std_ReturnType RetVal = E_NOT_OK;

#if (SD_DEV_ERROR_DETECT == STD_ON)
  /* this defensive checks a possible out of bound in array Sd_ServerSoConStates[] */
  SD_PRECONDITION_ASSERT( (((EventgroupIndex * SD_MAXNUM_INSTANCE_REMOTENODES) + ((uint16)RemoteNodeIndex - InstanceCfgRefPtr[InstanceIndex].RemoteNode)) < SD_MAXNUM_SUBSCRIPTIONS), SD_SERVERSERVICEADDEVENTGROUPSUBS_SVCID );
#endif
/* ----------------------------------------------------  UDP connection ------------------------------------------------------------------ */
  /* !LINKSTO Sd.ASR40.SWS_SD_00454,1 */
  if(  (ServiceCfgPtr->SoConGroupUdp != SD_INVALID_SOCONGROUP_ID) &&
       (UdpSockAddrPtr->domain != TCPIP_AF_UNSPEC) &&
       ( (EventHandlerCfgPtr->SdEventHandlerUdpActivationRef != SOAD_INVALID_ROUTINGGROUP_ID) || (EventHandlerCfgPtr->SdEventHandlerMulticastThreshold == 1U) )  )
  {
    RetVal = SoAd_SetUniqueRemoteAddr(SdSoConGroupsCfgRefPtr[ServiceCfgPtr->SoConGroupUdp].GroupStart, UdpSockAddrPtr, &SockConId);

    if(RetVal == E_OK)
    {
      /* only if there was no previous UDP connection between this event handler and this remote node or if the previous UDP connection still lives */
      if( (ServerSoConState->SockConIdUdp == SOAD_INVALID_SOCON_ID) || (ServerSoConState->SockConIdUdp == SockConId) )
      {
        if(ServerSoConState->TTL == SD_TTL_OFF)
        {
          CONSTP2VAR(Sd_EventHandlerStateType,AUTOMATIC,SD_APPL_DATA) EventHandlerStatePtr = &Sd_ServerEventHandlerStates[EventgroupIndex];

          ServerSoConState->SockConIdUdp = SockConId;
          /* only if this event handler does not have any Tcp subscription, it is not possible to send and event both per Tcp and Udp */
          Sd_ServerService_Add_UDP_DataPath(ServerSoConState, EventHandlerCfgPtr, EventHandlerStatePtr, EventgroupIndex);
        }
        else
        { /* !LINKSTO Sd.ASR40.SWS_SD_00569,1 */
          SoAd_ReleaseRemoteAddr(SockConId);
        }
      }
      else
      { /* !LINKSTO Sd.ASR40.SWS_SD_00569,1 */
        SoAd_ReleaseRemoteAddr(SockConId);
        RetVal = E_NOT_OK;
      }
    }
#if(SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DEM)
    else
    {
      P2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr = &SD_CFG_REF(SdInstance_t, SdInstancesRef, 0U);

      SD_DEM_REPORT_ERROR_STATUS(InstanceCfgPtr[InstanceIndex].SdDemOutOfResId, DEM_EVENT_STATUS_FAILED);
    }
#elif((SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON))
    else
    {
      SD_DET_REPORT_ERROR(SD_RXINDICATION_SVCID,SD_E_OUT_OF_RES_DEMTODET);
    }
#endif
  }
  else
  {
/* ----------------------------------------------------  TCP connection ------------------------------------------------------------------ */
    /* !LINKSTO Sd.ASR40.SWS_SD_00453,1 */
    if( (ServiceCfgPtr->SoConGroupTcp != SD_INVALID_SOCONGROUP_ID) &&
        (TcpSockAddrPtr->domain != TCPIP_AF_UNSPEC) &&
        (EventHandlerCfgPtr->SdEventHandlerTcpActivationRef != SOAD_INVALID_ROUTINGGROUP_ID) )
    {
      RetVal = SoAd_SetUniqueRemoteAddr(SdSoConGroupsCfgRefPtr[ServiceCfgPtr->SoConGroupTcp].GroupStart, TcpSockAddrPtr, &SockConId);

      if(RetVal == E_OK)
      {
        /* only if there was no previous TCP connection between this event handler and this remote node or if the previous TCP connection still lives */
        if( (ServerSoConState->SockConIdTcp == SOAD_INVALID_SOCON_ID) || (ServerSoConState->SockConIdTcp == SockConId) )
        { /* !LINKSTO Sd.ASR40.SWS_SD_00453,1 */
          SoAd_SoConModeType SoConMode = SOAD_SOCON_OFFLINE;
          /* The return value can be ignored as the SoConMode will not be set in case of an error */
          (void)SoAd_GetSoConMode(SockConId, &SoConMode);

          /* check if there is already a TCP connection open in this socket */
          if(SoConMode == SOAD_SOCON_ONLINE)
          {
            if(ServerSoConState->TTL == SD_TTL_OFF)
            {
              CONSTP2VAR(Sd_EventHandlerStateType,AUTOMATIC,SD_APPL_DATA) EventHandlerStatePtr = &Sd_ServerEventHandlerStates[EventgroupIndex];

              ServerSoConState->SockConIdTcp = SockConId;
              /* only if this event handler does not have any Udp subscription, it is not possible to send and event both per Tcp and Udp */
              Sd_ServerService_Add_TCP_DataPath(ServerSoConState, EventHandlerCfgPtr, EventHandlerStatePtr, EventgroupIndex);
            }
          }
          else
          { /* !LINKSTO Sd.ASR40.SWS_SD_00569,1 */
            SoAd_ReleaseRemoteAddr(SockConId);
            RetVal = E_NOT_OK;
          }
        }
        else
        { /* !LINKSTO Sd.ASR40.SWS_SD_00569,1 */
          SoAd_ReleaseRemoteAddr(SockConId);
          RetVal = E_NOT_OK;
        }
      }
#if(SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DEM)
      else
      {
        P2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr = &SD_CFG_REF(SdInstance_t, SdInstancesRef, 0U);

        SD_DEM_REPORT_ERROR_STATUS(InstanceCfgPtr[InstanceIndex].SdDemOutOfResId, DEM_EVENT_STATUS_FAILED);
      }
#elif((SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON))
      else
      {
        SD_DET_REPORT_ERROR(SD_RXINDICATION_SVCID,SD_E_OUT_OF_RES_DEMTODET);
      }
#endif
    }
  }

  if(RetVal == E_OK)
  {
    if(TTL == SD_ENTRYTTL_INFINITE)
    {
      ServerSoConState->TTL = SD_TTL_ALWAYS_ON;
    }
    else
    {
      CONSTP2VAR(Sd_InstanceStateType, AUTOMATIC, SD_APPL_DATA) InstanceStatesPtr = &Sd_InstanceStates[InstanceIndex];

      ServerSoConState->TTL = (TTL * SD_LOOPS_PER_SECOND) + 1U;

      if( (InstanceStatesPtr->ServersDeltaTTLtimer <= SD_TTL_ALWAYS_ON) || (ServerSoConState->TTL < InstanceStatesPtr->ServersDeltaTTLtimer) )
      {
        InstanceStatesPtr->ServersDeltaTTLtimer = ServerSoConState->TTL;
        InstanceStatesPtr->ServersDeltaTTL = ServerSoConState->TTL;
      }
      else
      {
        ServerSoConState->TTL += (InstanceStatesPtr->ServersDeltaTTL - InstanceStatesPtr->ServersDeltaTTLtimer);
      }
    }
  }
  return RetVal;
}



STATIC FUNC(void, SD_CODE) Sd_ServerService_Add_TCP_DataPath
(
  CONSTP2CONST(Sd_ServerSoConStateType, AUTOMATIC, SD_APPL_DATA) SrvSoConState,
  CONSTP2CONST(SdEventHandler_t, AUTOMATIC, SD_APPL_CONST) EventHandlerCfgPtr,
  CONSTP2VAR(Sd_EventHandlerStateType,AUTOMATIC,SD_APPL_DATA) EventHandlerStatePtr,
  uint16 EventgroupIndex
)
{
  /* first check if there if there is TCP connection configured in this event handler and if the received subscription contains a TCP endpoint
  and if there is a TCP reference configured in this event handler */
  if(EventHandlerCfgPtr->SdEventHandlerTcpTriggeringRef != SOAD_INVALID_ROUTINGGROUP_ID)
  {  /* The return value can be ignored as the only possible return value is E_OK */
    (void)SoAd_IfSpecificRoutingGroupTransmit(EventHandlerCfgPtr->SdEventHandlerTcpTriggeringRef, SrvSoConState->SockConIdTcp);
  }
  (void)SoAd_EnableSpecificRouting(EventHandlerCfgPtr->SdEventHandlerTcpActivationRef, SrvSoConState->SockConIdTcp);

  if(EventHandlerStatePtr->ClientCount == 0U)
  { /* Subscribe first client. Report to BswM */
    BswM_Sd_EventHandlerCurrentState(EventgroupIndex, SD_EVENT_HANDLER_REQUESTED);
  }
  ++EventHandlerStatePtr->ClientCount;
}



STATIC FUNC(void, SD_CODE) Sd_ServerService_Add_UDP_DataPath
(
  CONSTP2VAR(Sd_ServerSoConStateType, AUTOMATIC, SD_APPL_DATA) SrvSoConState,
  CONSTP2CONST(SdEventHandler_t, AUTOMATIC, SD_APPL_CONST) EventHandlerCfgPtr,
  CONSTP2VAR(Sd_EventHandlerStateType,AUTOMATIC,SD_APPL_DATA) EventHandlerStatePtr,
  uint16 EventgroupIndex
)
{
  /* check if there is an UDP event triggering reference, to be send only once when the subscription is received */
  if(EventHandlerCfgPtr->SdEventHandlerUdpTriggeringRef != SOAD_INVALID_ROUTINGGROUP_ID)
  { /* !LINKSTO Sd.ASR40.SWS_SD_00454,1 */
    /* The return value can be ignored as the only possible return value is E_OK */
    (void)SoAd_IfSpecificRoutingGroupTransmit(EventHandlerCfgPtr->SdEventHandlerUdpTriggeringRef, SrvSoConState->SockConIdUdp);
  }
  /* there must be an UDP datapath connection configured for this event handler or otherwise this event is a multicast only */
  if(EventHandlerStatePtr->ClientCount == 0U)
  { /* Subscribe first client. Report to BswM */
    BswM_Sd_EventHandlerCurrentState(EventgroupIndex, SD_EVENT_HANDLER_REQUESTED);
  }
  ++EventHandlerStatePtr->ClientCount;

  /* Enable routing */
  if(EventHandlerStatePtr->ClientCount < EventHandlerCfgPtr->SdEventHandlerMulticastThreshold)
  { /* !LINKSTO Sd.ASR40.SWS_SD_00454,1 */
    /* Client count is below threshold. we enable this clients unicast*/
    /* The return value can be ignored as the only possible return value is E_OK */
    (void)SoAd_EnableSpecificRouting(EventHandlerCfgPtr->SdEventHandlerUdpActivationRef, SrvSoConState->SockConIdUdp);
  }
  else if(EventHandlerStatePtr->ClientCount == EventHandlerCfgPtr->SdEventHandlerMulticastThreshold)
  {
    if(EventHandlerCfgPtr->SdEventHandlerMulticastThreshold > 1U)
    {
      P2VAR(Sd_ServerSoConStateType, AUTOMATIC, SD_APPL_DATA) ServerSoConState = &Sd_ServerSoConStates[EventgroupIndex * SD_MAXNUM_INSTANCE_REMOTENODES];
      uint16 ClientCount = EventHandlerStatePtr->ClientCount - 1U;

      do
      {
        if(ServerSoConState->TTL != SD_TTL_OFF)
        { /* return value can be ignored as the only possible return value is E_OK */
          (void)SoAd_DisableSpecificRouting(EventHandlerCfgPtr->SdEventHandlerUdpActivationRef, ServerSoConState->SockConIdUdp);

          --ClientCount;
        }
        ++ServerSoConState;
      }
      while(ClientCount > 0U);
    }
    /* The return value can be ignored as the only possible return value is E_OK */
    (void)SoAd_EnableSpecificRouting(EventHandlerCfgPtr->SdEventHandlerMulticastActivationRef,
                                     EventHandlerCfgPtr->SdMulticastEventSoConRef);
  }
  else
  {
    /* multicast is already opened. nothing to do */
  }
}



STATIC FUNC(void, SD_CODE) Sd_ServerService_removeEventGroupSubscriber
(
  CONSTP2VAR(Sd_EventHandlerStateType,AUTOMATIC,SD_APPL_DATA) EventHandlerStatePtr,
  CONSTP2VAR(Sd_ServerSoConStateType, AUTOMATIC, SD_APPL_DATA) SrvSoConState,
  uint16 EventgroupIndex,
  const boolean IsStop
)
{

  P2CONST(SdEventHandler_t, AUTOMATIC, SD_APPL_CONST) EventHandlerCfgPtr = &SD_CFG_REF(SdEventHandler_t, SdEventHandlersRef, 0U);
  SrvSoConState->TTL = SD_TTL_OFF;

  /* Check if there is a TCP subscriber with this remote index for this event group */
  if(SrvSoConState->SockConIdTcp != SOAD_INVALID_SOCON_ID)
  { /* there is an active TCP datapath, it is disabled here */
    (void)SoAd_DisableSpecificRouting(EventHandlerCfgPtr[EventgroupIndex].SdEventHandlerTcpActivationRef, SrvSoConState->SockConIdTcp);

    /* this event handler data path is no longer being used, its socket connection can be released to wild card */
    /* !LINKSTO Sd.ASR40.SWS_SD_00569,1 */
    SoAd_ReleaseRemoteAddr(SrvSoConState->SockConIdTcp);

    SrvSoConState->SockConIdTcp = SOAD_INVALID_SOCON_ID;
  }
  /* else there must be an UDP subscriber with this remote index for this event group */
  else /* (LocalSoConIdUdp != SOAD_INVALID_SOCON_ID) */
  {
    const SoAd_SoConIdType LocalSoConIdUdp = SrvSoConState->SockConIdUdp;
    SrvSoConState->SockConIdUdp = SOAD_INVALID_SOCON_ID;

    EventHandlerCfgPtr = &EventHandlerCfgPtr[EventgroupIndex];
    /* if set socket connection id is the multicast reference it means that this server service is a multicast only */
    if(LocalSoConIdUdp == EventHandlerCfgPtr->SdMulticastEventSoConRef)
    {
      /* this is a multicast only event group */
      if(EventHandlerStatePtr->ClientCount == 1U)
      { /* return value can be ignored as the only possible return value is E_OK */
        (void)SoAd_DisableSpecificRouting(EventHandlerCfgPtr->SdEventHandlerMulticastActivationRef, LocalSoConIdUdp);
      }
    }
    /* else, this was an unicast subscriber  */
    else
    {
      if(EventHandlerStatePtr->ClientCount < EventHandlerCfgPtr->SdEventHandlerMulticastThreshold)
      { /* once below the multicast threshold just the unicast datapath is disabled */
        (void)SoAd_DisableSpecificRouting(EventHandlerCfgPtr->SdEventHandlerUdpActivationRef, LocalSoConIdUdp);
      }
      else if (EventHandlerStatePtr->ClientCount == EventHandlerCfgPtr->SdEventHandlerMulticastThreshold)
      { /* The return value can be ignored as the only possible return value is E_OK */
        (void)SoAd_DisableSpecificRouting(EventHandlerCfgPtr->SdEventHandlerMulticastActivationRef,
                                          EventHandlerCfgPtr->SdMulticastEventSoConRef);

        if(IsStop == FALSE)
        {
          if(EventHandlerCfgPtr->SdEventHandlerMulticastThreshold > 1U)
          {
            /* the number of unicast routing data paths to be reactivated is equal to the multicast threshold minus one */
            uint16_least ClientsCount = (uint16_least)EventHandlerStatePtr->ClientCount - 1U;
            const Sd_ServerSoConStateType* ServerSoConState = &Sd_ServerSoConStates[EventgroupIndex * SD_MAXNUM_INSTANCE_REMOTENODES];

            do
            {
              if( (ServerSoConState->TTL != SD_TTL_OFF) && (ServerSoConState->SockConIdUdp != SOAD_INVALID_SOCON_ID) )
              {
                /* Client count is below threshold. we enable this clients unicast*/
                /* The return value can be ignored as the only possible return value is E_OK */
                (void)SoAd_EnableSpecificRouting(EventHandlerCfgPtr->SdEventHandlerUdpActivationRef, ServerSoConState->SockConIdUdp);

                /* each time an unicast routing data path is re-enabled, the counter of unicast clients is decremented */
                --ClientsCount;
              }
              ++ServerSoConState;
            }
            /* keep going until all previously existing unicast routing data-paths have been re-enabled */
            while(ClientsCount > 0U);
          }
        }
      }
      else
      {
        /* still in multicast mode. number of subscribed clients is going to be decremented */
      }
      /* this event handler data path is no longer being used, its socket connection can be released to wild card */
      /* !LINKSTO Sd.ASR40.SWS_SD_00569,1 */
      SoAd_ReleaseRemoteAddr(LocalSoConIdUdp);
    }
  }
#if (SD_DEV_ERROR_DETECT == STD_ON)
  /* this defensive check covers the case of an underflow in the variable ServerServicesTimersActive */
  SD_PRECONDITION_ASSERT( (EventHandlerStatePtr->ClientCount > 0U), SD_SERVERSERVICEREMOVEEVENTGROUPSUBS_SVCID );
#endif
  --EventHandlerStatePtr->ClientCount;

  if(EventHandlerStatePtr->ClientCount == 0U)
  { /* last client has unsubscribed, report to BswM */
    BswM_Sd_EventHandlerCurrentState(EventgroupIndex, SD_EVENT_HANDLER_RELEASED);
  }
}



/*============[Server Service Communicationphase statemachine]===================*/

STATIC FUNC(void, SD_CODE) Sd_ServerService_ComSM_init(const uint16 ServiceIndex)
{
  CONSTP2VAR(Sd_ServerServiceStateType,AUTOMATIC,SD_APPL_DATA) ServiceStatePtr = &Sd_ServerServiceStates[ServiceIndex];
  CONSTP2CONST(SdServerService_t, AUTOMATIC, SD_APPL_CONST) ServiceCfgRefPtr = &SD_CFG_REF(SdServerService_t, SdServerServicesRef, 0U);

  Sd_StopTimer(&ServiceStatePtr->TimeOutDelta);

  if(ServiceCfgRefPtr[ServiceIndex].SdServerServiceAutoAvailable)
  {
    ServiceStatePtr->ServerServiceState = SD_SERVER_SERVICE_AVAILABLE;
  }
  else
  {
    ServiceStatePtr->ServerServiceState = SD_SERVER_SERVICE_DOWN;
  }
  ServiceStatePtr->ComPhase = SD_SERVER_SERVICE_COMPHASE_DOWN;
  ServiceStatePtr->Repetition = 0U;
  ServiceStatePtr->EventFlags = 0U;
}

/* event */



STATIC FUNC(boolean, SD_CODE) Sd_ServerService_ComSM_SoConModeCheck
(
  CONSTP2CONST(SdServerService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr
)
{
  SoAd_SoConModeType SoConModeUdp;
  SoAd_SoConModeType SoConModeTcp;
  boolean IsOnline;

  CONSTP2CONST(SdSocketConnectionGroupType,AUTOMATIC,SD_APPL_CONST) SoConGroupsRefPtr = &SD_CFG_REF(SdSocketConnectionGroupType, SdSoConGroupsRef, 0U);

  if(ServiceCfgPtr->SoConGroupUdp != SD_INVALID_SOCONGROUP_ID)
  { /* if there is a valid UDP socket connection reference, it means that this socket connection group exists and has at leas
      one socket connection configured, in this case the use of a do-while loop is safe */
    SoAd_SoConIdType UnicastUdpSoConId = SoConGroupsRefPtr[ServiceCfgPtr->SoConGroupUdp].GroupStart;
    /* The return value can be ignored as the SoConMode will not be set in case of an error */
    (void)SoAd_GetSoConMode(UnicastUdpSoConId, &SoConModeUdp);
  }
  else
  {
    SoConModeUdp = SOAD_SOCON_OFFLINE;
  }

  if(ServiceCfgPtr->SoConGroupTcp != SD_INVALID_SOCONGROUP_ID)
  { /* if there is a valid TCP socket connection reference, it means that this socket connection group exists and has at leas
      one socket connection configured, in this case the use of a do-while loop is safe */
    SoAd_SoConIdType UnicastTcpSoConId = SoConGroupsRefPtr[ServiceCfgPtr->SoConGroupTcp].GroupStart;
    /* The return value can be ignored as the SoConMode will not be set in case of an error */
    (void)SoAd_GetSoConMode(UnicastTcpSoConId, &SoConModeTcp);
  }
  else
  {
    SoConModeTcp = SOAD_SOCON_OFFLINE;
  }

  if( (SoConModeUdp != SOAD_SOCON_OFFLINE) || (SoConModeTcp != SOAD_SOCON_OFFLINE) )
  {
    IsOnline = TRUE;
  }
  else /* this client service doesn't have an unicast data-path configured, therefore it it not needed to check its unicast socket connection state */
  {
    IsOnline = FALSE;
  }
  return IsOnline;
}



STATIC FUNC(void, SD_CODE) Sd_ServerService_ComSM_event_timeout
(
  CONSTP2CONST(SdServerService_t,AUTOMATIC,SD_APPL_CONST) SrvServiceCfgPtr,
  CONSTP2VAR(Sd_ServerServiceStateType,AUTOMATIC,SD_APPL_DATA) SrvServiceStatePtr,
  uint16_least SrvServiceIndex
)
{
  P2CONST(SdServerTimer_t, AUTOMATIC, SD_APPL_DATA) SrvTimerCfgPtr = &SD_CFG(SdServerTimer_t, SdServerTimersRef, SrvServiceCfgPtr->SdServerServiceTimerIndex);

  switch(SrvServiceStatePtr->ComPhase)
  {
  case SD_SERVER_SERVICE_COMPHASE_INITIALWAIT:
    /* before sending the very first multicast OFFER make sure that socket connections referenced by this server service are not still OFFLINE,
      otherwise, if the OFFER is sent too early, the sending of the first(s) event message could be missed. This check will only be performed
      a number of times, this number of times is set on variable SrvServiceStatePtr->Repetition on entering INTIAL WAIT state */
    /* Deviation MISRAC2012-2 */
    if( (SrvServiceStatePtr->Repetition == 0U) || (Sd_ServerService_ComSM_SoConModeCheck(SrvServiceCfgPtr) != FALSE) )
    { /* timeout in this server service occurred, send an OFFER */
      Sd_Send_queueOfferService(SrvServiceCfgPtr, (uint16)SrvServiceIndex, FALSE);

      /* REPETITION phase shall be skipped if RepetitionMax is set to zero
       * and the main phase shall be entered immediately */
      if(SrvTimerCfgPtr->SdServerTimerInitialOfferRepetitionsMax > 0U)
      { /* REPETITION phase is entered */
        SrvServiceStatePtr->Repetition = 0U;
        Sd_StartTimer(&(SrvServiceStatePtr->TimeOutDelta), SrvTimerCfgPtr->SdServerTimerInitialOfferRepetitionBaseDelay);
        SrvServiceStatePtr->ComPhase = SD_SERVER_SERVICE_COMPHASE_REPETITION;
      }
      else /* repetition max value is equal to 0, it means that REPETITION phase is skipped and MAIN phase in entered directly from INITIAL WAIT*/
      {
        Sd_StartTimer(&(SrvServiceStatePtr->TimeOutDelta), SrvTimerCfgPtr->SdServerTimerOfferCyclicDelay);
        SrvServiceStatePtr->ComPhase = SD_SERVER_SERVICE_COMPHASE_MAIN;
      }
    }
    else
    {
      SrvServiceStatePtr->TimeOutDelta = 1U;
      /* decrease counter of times that OFFLINE state of referenced socket connections is checked */
      --SrvServiceStatePtr->Repetition;
    }
    break;

  case SD_SERVER_SERVICE_COMPHASE_REPETITION:
    /* timeout in this server service occurred, send an OFFER */
    Sd_Send_queueOfferService(SrvServiceCfgPtr, (uint16)SrvServiceIndex, FALSE);

    ++SrvServiceStatePtr->Repetition;
    if(SrvServiceStatePtr->Repetition >= SrvTimerCfgPtr->SdServerTimerInitialOfferRepetitionsMax)
    { /* goes into MAIN phase */
      Sd_StartTimer(&(SrvServiceStatePtr->TimeOutDelta), SrvTimerCfgPtr->SdServerTimerOfferCyclicDelay);

      SrvServiceStatePtr->ComPhase = SD_SERVER_SERVICE_COMPHASE_MAIN;
    }
    else
    {
      /* Remains in REPETITION phase */

      /* Range of Repetition: 1 .. 10 */
      Sd_StartTimer( &(SrvServiceStatePtr->TimeOutDelta),
                     SrvTimerCfgPtr->SdServerTimerInitialOfferRepetitionBaseDelay * (Sd_TimerType)((Sd_TimerType)1U << SrvServiceStatePtr->Repetition) );
    }
    break;

  case SD_SERVER_SERVICE_COMPHASE_MAIN:
    /* timeout in this server service occurred, send an OFFER */
    Sd_Send_queueOfferService(SrvServiceCfgPtr, (uint16)SrvServiceIndex, FALSE);

    /* Remains in MAIN phase */
    Sd_StartTimer(&(SrvServiceStatePtr->TimeOutDelta), SrvTimerCfgPtr->SdServerTimerOfferCyclicDelay);
    break;

  /* CHECK: NOPARSE */
  default: /* default case is intended to be empty */
#if (SD_DEV_ERROR_DETECT == STD_ON)
      SD_UNREACHABLE_CODE_ASSERT(SD_CLIENTSERVICE_COMSM_SOCONMODECHECK_SVCID);
#endif /* SD_DEV_ERROR_DETECT == STD_ON */
    break;
  /* CHECK: PARSE */
  }
}



/* state entry functions */
STATIC FUNC(void, SD_CODE) Sd_ServerService_ComSM_entry_DOWN
(
  CONSTP2CONST(SdServerService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ServerServiceStateType,AUTOMATIC,SD_APPL_DATA) ServiceStatePtr,
  const Sd_RemoteNodeIdType RemoteNodeIndexEnd,
  const boolean FollowedByStart
)
{
  if(FollowedByStart == FALSE)
  {
    CONSTP2CONST(SdSocketConnectionGroupType,AUTOMATIC,SD_APPL_CONST) SoConGroupsRefPtr = &SD_CFG_REF(SdSocketConnectionGroupType, SdSoConGroupsRef, 0U);
    SoAd_SoConIdType SoConId;

    if (ServiceCfgPtr->SdServerServiceActivationRef != SOAD_INVALID_ROUTINGGROUP_ID)
    {
      /* The return value can be ignored as the only possible return value is E_OK */
      (void)SoAd_DisableRouting(ServiceCfgPtr->SdServerServiceActivationRef);
    }

    if(ServiceCfgPtr->SoConGroupUdp != SD_INVALID_SOCONGROUP_ID)
    { /* if there is a valid UDP socket connection reference, it means that this socket connection group exists and has at least
        one socket connection configured, in this case the use of a do-while loop is safe */
      CONSTP2CONST(SdSocketConnectionGroupType,AUTOMATIC,SD_APPL_CONST) SoConGroupUdpPtr = &SoConGroupsRefPtr[ServiceCfgPtr->SoConGroupUdp];
      SoConId = SoConGroupUdpPtr->GroupStart;
      {
        const SoAd_SoConIdType SoConIdEnd = SoConId + SoConGroupUdpPtr->SoConCount;

        do
        {
          /* !LINKSTO Sd.ASR40.SWS_SD_00603,1 */
          /* The return value can be ignored as the only possible return value is E_OK */
          (void)SoAd_CloseSoCon(SoConId, FALSE);
          ++SoConId;
        }
        while(SoConId < SoConIdEnd);
      }
    }

    if(ServiceCfgPtr->SoConGroupTcp != SD_INVALID_SOCONGROUP_ID)
    { /* if there is a valid TCP socket connection reference, it means that this socket connection group exists and has at least
        one socket connection configured, in this case the use of a do-while loop is safe */
      CONSTP2CONST(SdSocketConnectionGroupType,AUTOMATIC,SD_APPL_CONST) SoConGroupTcpPtr = &SoConGroupsRefPtr[ServiceCfgPtr->SoConGroupTcp];
      SoConId = SoConGroupTcpPtr->GroupStart;
      {
        const SoAd_SoConIdType SoConIdEnd = SoConId + SoConGroupTcpPtr->SoConCount;

        do
        {
          /* The return value can be ignored as the only possible return value is E_OK */
          (void)SoAd_CloseSoCon(SoConId, FALSE);
          ++SoConId;
        }
        while(SoConId < SoConIdEnd);
      }
    }
  }
  {
    uint16_least EventgroupIndex = ServiceCfgPtr->SdEventHandler;
    const uint16_least EventgroupIndexEnd = EventgroupIndex + ServiceCfgPtr->SdEventHandlerCount;
    P2VAR(Sd_EventHandlerStateType,AUTOMATIC,SD_APPL_DATA) EventHandlerStatePtr = &Sd_ServerEventHandlerStates[EventgroupIndex];
    Sd_RemoteNodeIdType NodeIndex;

    while(EventgroupIndex < EventgroupIndexEnd)
    {
      if(EventHandlerStatePtr->ClientCount > 0U)
      {
        P2VAR(Sd_ServerSoConStateType, AUTOMATIC, SD_APPL_DATA) ServerSoConState = &Sd_ServerSoConStates[(uint16)EventgroupIndex * SD_MAXNUM_INSTANCE_REMOTENODES];

        NodeIndex = 0U;
        do
        {
          if(ServerSoConState->TTL != SD_TTL_OFF)
          { /* remove only if this event handler had a subscription from this remote node */
            Sd_ServerService_removeEventGroupSubscriber(EventHandlerStatePtr, ServerSoConState, (uint16)EventgroupIndex, TRUE);
          }
          ++ServerSoConState;
          ++NodeIndex;
          /* if there are no more clients subscribed to this event handler there is no reason to keep looping along all remote nodes */
        }
        while( (EventHandlerStatePtr->ClientCount > 0U) && (NodeIndex < RemoteNodeIndexEnd) );
      }
      ++EventgroupIndex;
      ++EventHandlerStatePtr;
    }

  }
  ServiceStatePtr->ComPhase = SD_SERVER_SERVICE_COMPHASE_DOWN;
}



STATIC FUNC(void, SD_CODE) Sd_ServerService_ComSM_entry_INITIALWAIT
(
  CONSTP2CONST(SdServerService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ServerServiceStateType,AUTOMATIC,SD_APPL_DATA) ServiceStatePtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr,
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr
)
{
  CONSTP2CONST(SdServerTimer_t, AUTOMATIC, SD_APPL_DATA) TimerCfgRefPtr = &SD_CFG_REF(SdServerTimer_t, SdServerTimersRef, 0U);
  P2CONST(SdServerTimer_t, AUTOMATIC, SD_APPL_DATA) TimerCfgPtr;
  uint16_least ServerServiceIndex = InstanceCfgPtr->SdServerService;
  const uint16_least ServerServiceIndexEnd = ServerServiceIndex + InstanceCfgPtr->SdServerServiceCount;

  /* --- ServerService Offer Timing Alignment Algorithm --- */
  /* Note: In order to combine multiple OFFERs within one frame, the timings of ServerServices
   * need to be synchronized. This can best be achieved by aligning their phases.
   * When the InitialWait phase of a Service shall be entered, Sd looks up other Services that
   * are also in InitialWait. If the timings of the Services are compatible, the current service
   * is configured to enter the next phase simultaneously with the other service.
   */
  P2CONST(SdServerService_t, AUTOMATIC, SD_APPL_CONST) pServerCfg = &SD_CFG(SdServerService_t, SdServerServicesRef, ServerServiceIndex);
  P2CONST(Sd_ServerServiceStateType,AUTOMATIC,SD_APPL_DATA) pServerState = &Sd_ServerServiceStates[ServerServiceIndex];

  TimerCfgPtr = &TimerCfgRefPtr[ServiceCfgPtr->SdServerServiceTimerIndex];
  /* loop over all ServerServices to find a match, do-while loop is secure because if this function has been called
   * and it is being executed it is because there is at least one server service configured in this instance */
  do
  { /* search all configured server services of this instance to see if there is Service is also in the InitialWait phase
      which has the same Timer configuration */
    if( (pServerCfg != ServiceCfgPtr) &&
        (pServerState->ComPhase == SD_SERVER_SERVICE_COMPHASE_INITIALWAIT) &&
        (&TimerCfgRefPtr[pServerCfg->SdServerServiceTimerIndex] == TimerCfgPtr) )
    { /* now the timer of the new service can be aligned to the matched service only if its timer is still over or equal the minimum timer limit */
      if(InstanceStatePtr->ServerDeltaTimer >= TimerCfgPtr->SdServerTimerInitialOfferDelayMin)
      { /* set the timer of this service to the same value as the matching one */
        ServiceStatePtr->TimeOutDelta = pServerState->TimeOutDelta;
        /* match found, quit loop */
        break;
      }
    }
    ++ServerServiceIndex;
    ++pServerCfg;
    ++pServerState;
  }
  while(ServerServiceIndex < ServerServiceIndexEnd);

  if(ServerServiceIndex == ServerServiceIndexEnd)
  { /* no match */
    /* setup the timer to a random value between mininum and maximum limit because the alignment algorithm did not find a match */
    ServiceStatePtr->TimeOutDelta = Sd_GetRandom(TimerCfgPtr->SdServerTimerInitialOfferDelayMin, TimerCfgPtr->SdServerTimerInitialOfferDelayMax) + 1U;

    if( (InstanceStatePtr->ServerDeltaTimer == 0U) || (ServiceStatePtr->TimeOutDelta < InstanceStatePtr->ServerDeltaTimer) )
    { /* set next timeout */
      InstanceStatePtr->ServerDeltaTimer = ServiceStatePtr->TimeOutDelta;
      InstanceStatePtr->ServersDelta = ServiceStatePtr->TimeOutDelta;
    }
    else
    {
      ServiceStatePtr->TimeOutDelta += (InstanceStatePtr->ServersDelta - InstanceStatePtr->ServerDeltaTimer);
    }
    /* else nothing to do here, the new timer for this service is bigger or equal than the next timeout timer */
  }
  ++InstanceStatePtr->ServerServicesTimersActive;
  /* before the very first multicast OFFER after entering INITIAL WAIT state is sent,
    current state of referenced socket connections of this server service is checked as many times as
    value set to ServiceStatePtr->Repetition only as long as state remains OFFLINE */
  ServiceStatePtr->Repetition = 2U;
  ServiceStatePtr->ComPhase = SD_SERVER_SERVICE_COMPHASE_INITIALWAIT;
}



STATIC FUNC(void, SD_CODE) Sd_ServerService_ComSM_exit_DOWN
(
  CONSTP2CONST(SdServerService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr
)
{
  CONSTP2CONST(SdSocketConnectionGroupType, AUTOMATIC,SD_APPL_CONST) SoConGroupsRefPtr = &SD_CFG_REF(SdSocketConnectionGroupType, SdSoConGroupsRef, 0U);
  SoAd_SoConIdType SoConId;

  if(ServiceCfgPtr->SoConGroupUdp != SD_INVALID_SOCONGROUP_ID)
  { /* if there is a valid UDP socket connection reference, it means that this socket connection group exists and has at leas
      one socket connection configured, in this case the use of a do-while loop is safe */
    CONSTP2CONST(SdSocketConnectionGroupType,AUTOMATIC,SD_APPL_CONST) SoConGroupUdpPtr = &SoConGroupsRefPtr[ServiceCfgPtr->SoConGroupUdp];
    SoConId = SoConGroupUdpPtr->GroupStart;
    {
      const SoAd_SoConIdType SoConIdEnd = SoConId + SoConGroupUdpPtr->SoConCount;
      do
      {
        /* !LINKSTO Sd.ASR40.SWS_SD_00606,1 */
        /* The return value can be ignored as the only possible return value is E_OK */
        (void)SoAd_OpenSoCon(SoConId);
        ++SoConId;
      }
      while(SoConId < SoConIdEnd);
    }
  }

  if(ServiceCfgPtr->SoConGroupTcp != SD_INVALID_SOCONGROUP_ID)
  { /* if there is a valid TCP socket connection reference, it means that this socket connection group exists and has at leas
      one socket connection configured, in this case the use of a do-while loop is safe */
    CONSTP2CONST(SdSocketConnectionGroupType,AUTOMATIC,SD_APPL_CONST) SoConGroupTcpPtr = &SoConGroupsRefPtr[ServiceCfgPtr->SoConGroupTcp];
    SoConId = SoConGroupTcpPtr->GroupStart;
    {
      const SoAd_SoConIdType SoConIdEnd = SoConId + SoConGroupTcpPtr->SoConCount;
      do
      {
        /* !LINKSTO Sd.ASR40.SWS_SD_00606,1 */
        /* The return value can be ignored as the only possible return value is E_OK */
        (void)SoAd_OpenSoCon(SoConId);
        ++SoConId;
      }
      while(SoConId < SoConIdEnd);
    }
  }

  if (ServiceCfgPtr->SdServerServiceActivationRef != SOAD_INVALID_ROUTINGGROUP_ID)
  {
    /* The return value can be ignored as the only possible return value is E_OK */
    (void)SoAd_EnableRouting(ServiceCfgPtr->SdServerServiceActivationRef);
  }
}



#define SD_STOP_SEC_CODE
#include <Sd_MemMap.h>

/*==================[end of file]===========================================*/
