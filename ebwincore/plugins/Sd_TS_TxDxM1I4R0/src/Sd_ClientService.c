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
 */

 /* Code-Metric Deviation List
 *
 * CODEMETRIC-1) Deviated Rule: HIS_V(G)
 * not 26 <= 20 violated
 *
 * Reason:
 * Sd_ClientService_eventOfferService processes received OFFER entries, an OFFER can have so many options in its endpoints 
 * that the number of il-else branches is too big. This function will be optimized without impacting run-time.
 *
 */

/*==================[inclusions]============================================*/

#include <Sd.h>               /* Module public API */
#include <Sd_ClientService_Int.h>
#include <Sd_Send_Int.h>

#include <SoAd.h>
#include <BswM_Sd.h>
#include <SchM_Sd.h>           /* SchM symbols for crit. sections */
#if(SD_DEM_DETECT == STD_ON)
/* !LINKSTO Sd.ASR40.SWS_SD_00003,1 */
/* !LINKSTO Sd.ASR40.SWS_SD_00106,1 */
#include <Dem.h>
#endif /* SD_DEM_DETECT == STD_ON */

/*==================[macros]================================================*/

#define SD_CLIENT_EVENT_START           0x01U
#define SD_CLIENT_EVENT_STOP            0x02U
#define SD_CLIENT_EVENT_HALT            0x04U
#define SD_CLIENT_EVENT_MULTICAST_OFFER 0x08U
#define SD_CLIENT_EVENT_UNICAST_OFFER   0x10U
#define SD_CLIENT_EVENT_STOPOFFER_0     0x20U
#define SD_CLIENT_EVENT_STOPOFFER_1     0x40U

/* Consumed event group states */
/* OFF state, consumed event group is released (not requested yet)
  The state of the client service where this consumed event group is contained,
  does not have any influence in this state */
#define SD_CONSUMED_EVENTGROUP_STATE_RELEASED                  0U
/* STAND BY state, consumed event group has been requested and the client service,
  where this consumed event group is contained, has been requested (is switched ON)
  but it has not receive an OFFER yet */
#define SD_CONSUMED_EVENTGROUP_STATE_REQUESTED_NO_OFFER        1U
/* READY state, consumed event group has been requested and the client service,
  where this consumed event group is contained, has received an OFFER */
#define SD_CONSUMED_EVENTGROUP_STATE_REQUESTED_OFFER_RECEIVED  2U
/* WAIT state, consumed event group has been requested, has sent a subscription to the received OFFER
  has received a subscription ACK as answer to the sent subscription and it is waiting
  until the socket connection that has been opened goes to state ONLINE */
#define SD_CONSUMED_EVENTGROUP_STATE_WAIT_FOR_AVALIABILITY     3U
/* ON state, consumed event group has been requested, has sent a subscription to the received OFFER
  has received a subscription ACK as answer to the sent subscription
  and the socket connection that has been opened is in state ONLINE */
#define SD_CONSUMED_EVENTGROUP_STATE_AVAILABLE                 4U


#define SD_CLIENT_SUBSCRIBEFLAG_NO_ACT  0U
#define SD_CLIENT_SUBSCRIBEFLAG_SENT    1U
#define SD_CLIENT_SUBSCRIBEFLAG_NORMAL  2U
#define SD_CLIENT_SUBSCRIBEFLAG_MC_RESP 3U

/* consumed event group has not sent any subscription yet */
#define SD_CEVG_NO_SUBSCRIPTION_SENT    0xFFU
/* consumed event group has sent at least a subscription but last one sent has not been acknowledged */
#define SD_CEVG_NOT_ACKNOWLEDGED        0U
/* consumed event group has sent at least a subscription and last one sent has been acknowledged */
#define SD_CEVG_ACKNOWLEDGED            1U

/*==================[type definitions]======================================*/
/* !LINKSTO Sd.Design.Data.ServiceStates,1 */
typedef struct
{
  Sd_TimerType TTL;
  SoAd_SoConIdType MulticastSoConId;
  uint8 MachineState;
  uint8 Acknowledged;
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
  uint8 SdSubscribeEventgroupRetryCounter;
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
  boolean SendStopSubsc;
}Sd_ConsumedEventGroupStateType;

typedef struct
{
  Sd_TimerType StateMachineTimer;
  uint32 TTLreceived;
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
  /* number of consumed event groups in the client service that has not been acknowledged already */
  uint16 SdSubscribeEventGroupsNotAck;
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
  SoAd_SoConIdType SoConIdUdp;
  SoAd_SoConIdType SoConIdTcp;
  Sd_RemoteNodeIdType NodeIndex;
  Sd_ClientServiceSetStateType RequestedState;
  uint8 ComPhase;
  uint8 Repetition;
  uint8 EventFlags;
  uint8 SdOfferEvents;
  uint8 SubscribeFlag;
  boolean OfferReceived;
  boolean SubsAckEvent;
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
  /* indicates when TRUE that a OFFER with TTL infinite has been received and that sending
    of consumed event group retries is enabled for this client service */
  boolean SdSubscribeEventGroupTimersActive;
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
}Sd_ClientServiceStateType;

typedef struct
{
  boolean                 AckReceived;
}Sd_ClientConsumEventGroupMulticastRefStateType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#define SD_START_SEC_CODE
#include <Sd_MemMap.h>


STATIC FUNC(uint16, SD_CODE) Sd_ClientService_QuickMatch
(
  CONSTP2CONST(Sd_ServiceInfoType, AUTOMATIC, SD_APPL_DATA) ServiceInfoPtr,
  CONSTP2CONST(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfoPtr,
  const uint32 MinorVersion,
  const uint8 SdInstanceIndex,
  const boolean CheckMinorVersion
);

/*=================== Consumed Eventgroup Statemachine ==================================*/
STATIC FUNC(void, SD_CODE) Sd_ClientService_CEvg_init(uint16 EventgroupIndex);

STATIC FUNC(boolean, SD_CODE) Sd_ClientService_CEvg_event_SubscribeEventgroupAck
(
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, SD_APPL_DATA) MulticastEndPointSockAddrPtr,
  const uint32 TTL,
  uint16 EventgroupIndex,
  const uint8 InstanceIndex
);

STATIC FUNC(boolean, SD_CODE) Sd_ClientService_CEvg_entry_AVAILABLE
(
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, SD_APPL_DATA) MulticastSockAddrPtr,
  CONSTP2CONST(SdConsumedEventGroup_t, AUTOMATIC, SD_APPL_CONST) EventGroupCfgPtr,
  uint16 EventgroupIndex
);

STATIC FUNC(void, SD_CODE) Sd_ClientService_CEvg_GoTo_AVAILABLE
(
  CONSTP2CONST(SdConsumedEventGroup_t,AUTOMATIC,SD_APPL_CONST) EventGroupCfgPtr,
  CONSTP2VAR(Sd_ConsumedEventGroupStateType,AUTOMATIC,SD_APPL_CONST) EventGroupStePtr,
  uint16 EventgroupIdx
);

STATIC FUNC(void, SD_CODE) Sd_ClientService_CEvg_exit_AVAILABLE
(
  CONSTP2CONST(SdConsumedEventGroup_t,AUTOMATIC,SD_APPL_CONST) EventGroupCfgPtr,
  CONSTP2VAR(Sd_ConsumedEventGroupStateType,AUTOMATIC,SD_APPL_CONST) EventGroupStatPtr,
  uint16 EventgroupIndex
);

STATIC FUNC(void, SD_CODE) Sd_ClientService_ComSM_entry_INITIALWAIT
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ClientServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ClientServiceStatePtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr
);

STATIC FUNC(void, SD_CODE) Sd_ClientService_ComSM_init(const uint16 ServiceIndex);

STATIC FUNC(void, SD_CODE) Sd_ClientService_Set_State
(
  const uint16 ServiceIndex,
  const Sd_ClientServiceSetStateType State
);

STATIC FUNC(Std_ReturnType, SD_CODE) Sd_ClientService_consumedEventGroupSetState
(
  const uint16 EventgroupIndex,
  const Sd_ConsumedEventGroupSetStateType State
);

STATIC FUNC(void, SD_CODE) Sd_ClientService_sendSubscribeEventGroupsOfService
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  const Sd_RemoteNodeIdType RemoteNodeIndex
);

/* event */
STATIC FUNC(void, SD_CODE) Sd_ClientService_ComSM_event_stop
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  uint16 ServiceIndex,
  const boolean isStop,
  const boolean FollowedByStart
);

STATIC FUNC(void, SD_CODE) Sd_ClientService_ComSM_event_timeout
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  uint16 ServiceIndex
);
STATIC FUNC(void, SD_CODE) Sd_ClientService_TTL_Run_Out
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  uint16 ServiceIndex
);
STATIC FUNC(void, SD_CODE) Sd_ClientService_ComSM_event_Offer
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  uint16 ServiceIndex,
  const boolean isMulticast
);
STATIC FUNC(void, SD_CODE) Sd_ClientService_ComSM_event_StopOffer
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  uint16 ServiceIndex,
  const boolean OfferFollows
);
STATIC FUNC(void, SD_CODE) Sd_ClientService_ComSM_SoConModeCheck
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  uint16 ServiceIndex
);

/* transitions */
STATIC FUNC(void, SD_CODE) Sd_ClientService_ComSM_entry_AVAILABLE
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  const SoAd_SoConIdType UdpSoConId,
  const SoAd_SoConIdType TcpSoConId,
  uint16 ServiceIndex
);

/* state exit functions */
STATIC FUNC(void, SD_CODE) Sd_ClientService_ComSM_exit_DOWN
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ClientServiceCfgPtr
);

STATIC FUNC(void, SD_CODE) Sd_ClientService_Disable_All_ConsumedEventGroups
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  const boolean isStop
);

STATIC FUNC(void, SD_CODE) Sd_ClientService_ComSM_exit_AVAILABLE
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  uint16 ServiceIndex,
  const boolean isStop,
  const boolean PendingOffer
);

STATIC FUNC(void, SD_CODE) Sd_ClientService_ProcessOffers
(
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstStatePtr,
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  uint16_least *ClientOffersSubAcksCtrPtr,
  uint16 ServiceIdx
);

STATIC FUNC(void, SD_CODE) Sd_Sd_ClientService_ProcessSubsAcks
(
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstStatePtr,
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  uint16_least *ClientOffersSubAcksCtrPtr
);

STATIC FUNC(boolean, SD_CODE) SdCheckIfMinorVersionBlackListed
(
  CONSTP2CONST(SdClientService_t, AUTOMATIC, SD_APPL_DATA) ClientServiceCfgPtr,
  const uint32 MinorVersion
);

#define SD_STOP_SEC_CODE
#include <Sd_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/
#define SD_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Sd_MemMap.h>

STATIC VAR(Sd_ClientServiceStateType, SD_VAR) Sd_ClientServiceStates[SD_ARRAY_SIZE(SD_MAX_CLIENT_SERVICE_COUNT)];
STATIC VAR(Sd_ConsumedEventGroupStateType, SD_VAR) Sd_ConsumedEventGroupStates[SD_ARRAY_SIZE(SD_MAX_CLIENT_SERVICE_EVENTGROUP_COUNT)];

#define SD_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Sd_MemMap.h>

#define SD_START_SEC_VAR_CLEARED_16
#include <Sd_MemMap.h>

STATIC VAR(uint16, SD_VAR) Sd_ClientConsumEventGroupMulticastRefSubsCounter[SD_ARRAY_SIZE(SD_MAX_CLIENT_CONSUMED_GROUPS_MULTICAST_EVENTREF_COUNT)];

#define SD_STOP_SEC_VAR_CLEARED_16
#include <Sd_MemMap.h>

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define SD_START_SEC_CODE
#include <Sd_MemMap.h>


/* Module API functions */

FUNC(Std_ReturnType, SD_CODE) Sd_ClientServiceSetState
(
  uint16 ClientServiceHandleId,
  Sd_ClientServiceSetStateType ClientServiceState
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SD_CLIENTSERVICESETSTATE_ENTRY(ClientServiceHandleId,ClientServiceState);

#if (SD_DEV_ERROR_DETECT == STD_ON)
  if (Sd_Initialized == FALSE)
  {
    SD_DET_REPORT_ERROR(SD_CLIENTSERVICESETSTATE_SVCID,SD_E_NOT_INITIALIZED);
  }
  else if((ClientServiceState != SD_CLIENT_SERVICE_RELEASED) &&
          (ClientServiceState != SD_CLIENT_SERVICE_REQUESTED))
  {
    SD_DET_REPORT_ERROR(SD_CLIENTSERVICESETSTATE_SVCID,SD_E_INV_MODE);
  }
  else if (ClientServiceHandleId >= SdConfigPtr->SdClientServiceCount)
  {
    SD_DET_REPORT_ERROR(SD_CLIENTSERVICESETSTATE_SVCID,SD_E_INV_ID);
  }
  else
#endif
  {
    SchM_Enter_Sd_SCHM_SD_EXCLUSIVE_AREA_0();

    Sd_ClientService_Set_State(ClientServiceHandleId,ClientServiceState);

    SchM_Exit_Sd_SCHM_SD_EXCLUSIVE_AREA_0();

    RetVal = E_OK;
  }

  DBG_SD_CLIENTSERVICESETSTATE_EXIT(RetVal,ClientServiceHandleId,ClientServiceState);

  return RetVal;
}



FUNC(Std_ReturnType, SD_CODE) Sd_ConsumedEventGroupSetState
(
  uint16 ConsumedEventGroupHandleId,
  Sd_ConsumedEventGroupSetStateType ConsumedEventGroupState
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  DBG_SD_CONSUMEDEVENTGROUPSETSTATE_ENTRY(ConsumedEventGroupHandleId,ConsumedEventGroupState);

#if (SD_DEV_ERROR_DETECT == STD_ON)
  if (Sd_Initialized == FALSE)
  {
    SD_DET_REPORT_ERROR(SD_CONSUMEDEVENTGROUPSETSTATE_SVCID,SD_E_NOT_INITIALIZED);
  }
  else if ((ConsumedEventGroupState != SD_CONSUMED_EVENTGROUP_RELEASED) && (ConsumedEventGroupState != SD_CONSUMED_EVENTGROUP_REQUESTED))
  {
    SD_DET_REPORT_ERROR(SD_CONSUMEDEVENTGROUPSETSTATE_SVCID,SD_E_INV_MODE);
  }
  else if (ConsumedEventGroupHandleId >= SdConfigPtr->SdConsumedEventGroupCount)
  {
    SD_DET_REPORT_ERROR(SD_CONSUMEDEVENTGROUPSETSTATE_SVCID,SD_E_INV_ID);
  }
  else
#endif
  {
    SchM_Enter_Sd_SCHM_SD_EXCLUSIVE_AREA_0();
    RetVal = Sd_ClientService_consumedEventGroupSetState(ConsumedEventGroupHandleId,ConsumedEventGroupState);
    SchM_Exit_Sd_SCHM_SD_EXCLUSIVE_AREA_0();
  }

  DBG_SD_CONSUMEDEVENTGROUPSETSTATE_EXIT(RetVal,ConsumedEventGroupHandleId,ConsumedEventGroupState);

  return RetVal;
}



FUNC(uint8, SD_CODE) Sd_ClientService_getComphase
(
  const uint16 ServiceIndex
)
{
  return Sd_ClientServiceStates[ServiceIndex].ComPhase;
}


/* SW-Unit API functions */

TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ClientService_init(void)
{
  uint16 Index;

  for (Index = 0U; Index < SdConfigPtr->SdClientServiceCount; ++Index)
  {
    Sd_ClientService_ComSM_init(Index);
  }

  for (Index = 0U; Index < SdConfigPtr->SdConsumedEventGroupCount; ++Index)
  {
    Sd_ClientService_CEvg_init(Index);
  }

#if SD_MAX_CLIENT_CONSUMED_GROUPS_MULTICAST_EVENTREF_COUNT > 0U
  Index = 0U;
  do
  {
    Sd_ClientConsumEventGroupMulticastRefSubsCounter[Index] = 0U;
    ++Index;
  }
  while(Index < SD_MAX_CLIENT_CONSUMED_GROUPS_MULTICAST_EVENTREF_COUNT);
#endif
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ClientServices_init_after_halt
(
    CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr
)
{
  if(InstanceCfgPtr->SdClientServiceCount > 0U)
  {
    uint16_least ClientIndex = InstanceCfgPtr->SdClientService;
    /* get the last client service index of this instance in a constant variable */
    const uint16_least ClientIndexEnd = ClientIndex + InstanceCfgPtr->SdClientServiceCount;

    P2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ClientServiceCfgPtr = &SD_CFG(SdClientService_t, SdClientServicesRef, ClientIndex);

    do
    {
      CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr = &Sd_ClientServiceStates[ClientIndex];

      ServiceStatePtr->SubscribeFlag = SD_CLIENT_SUBSCRIBEFLAG_NO_ACT;
      ServiceStatePtr->OfferReceived = FALSE;
      ServiceStatePtr->SdOfferEvents = 0U;
      ServiceStatePtr->SubsAckEvent = FALSE;
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
      ServiceStatePtr->SdSubscribeEventGroupsNotAck = 0U;
      ServiceStatePtr->SdSubscribeEventGroupTimersActive = FALSE;
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
      if(ClientServiceCfgPtr->SdConsumedEventGroupCount > 0U)
      {
        uint16_least EventgroupIndex = ClientServiceCfgPtr->SdConsumedEventGroup;
        const uint16_least EventgroupIndexEnd = EventgroupIndex + ClientServiceCfgPtr->SdConsumedEventGroupCount;
        P2VAR(Sd_ConsumedEventGroupStateType,AUTOMATIC,SD_APPL_CONST) EventGroupStatePtr = &Sd_ConsumedEventGroupStates[EventgroupIndex];
        P2CONST(SdConsumedEventGroup_t,AUTOMATIC,SD_APPL_CONST) ConsumedEventGroupCfgPtr = &SD_CFG_REF(SdConsumedEventGroup_t, SdConsumedEventGroupsRef, 0U);

        /* Disable all pending send stop subscription from this client */
        do
        {
          EventGroupStatePtr->SendStopSubsc = FALSE;
          EventGroupStatePtr->Acknowledged = SD_CEVG_NO_SUBSCRIPTION_SENT;
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
          EventGroupStatePtr->SdSubscribeEventgroupRetryCounter = 0U;
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
          if(EventGroupStatePtr->MulticastSoConId != SOAD_INVALID_SOCON_ID)
          {
            EventGroupStatePtr->MulticastSoConId = SOAD_INVALID_SOCON_ID;
#if (SD_DEV_ERROR_DETECT == STD_ON)
            /* this defensive check covers the case of an underflow in the variable ServerServicesTimersActive */
            SD_PRECONDITION_ASSERT( (Sd_ClientConsumEventGroupMulticastRefSubsCounter[ConsumedEventGroupCfgPtr[EventgroupIndex].SdConsumEventGroupMulticastRefIndex] > 0U), SD_CLIENTSERVICESINITAFTERTHALT_SVCID );
#endif
            --Sd_ClientConsumEventGroupMulticastRefSubsCounter[ConsumedEventGroupCfgPtr[EventgroupIndex].SdConsumEventGroupMulticastRefIndex];
          }
          ++EventgroupIndex;
          ++EventGroupStatePtr;
        }
        while(EventgroupIndex < EventgroupIndexEnd);
      }
      ++ClientIndex;
      ++ClientServiceCfgPtr;
    }
    while(ClientIndex < ClientIndexEnd);
  }
}



/* this function is only called from Sd.c if there have events recorded for this whole instance
 * since the previous main cycle.
 * Variable "ClientEventsCount" is a counter that stores the number of BswM events registered since
 * last main cycle.
 * Variable "ClientOfferEventsCount" is a counter that stores the number of SD events (OFFER or STOP OFFER)
 * registered since last main cycle.
 * If ClientEventsCount is equal to 0 and ClientOfferEventsCount is also equal to 0, this function is not called.
 */
TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ClientService_mainFunction
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr,
  uint16 ClientEventsCount,
  uint16 ClientOffersSubAcksCount
)
{
  /* get the first client service index of this instance */
  uint16_least ServiceIndex = InstanceCfgPtr->SdClientService;
  /* position pointers to the first clients service of this instance */
  P2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ClientServiceStatePtr = &Sd_ClientServiceStates[ServiceIndex];
  P2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ClientServiceCfgPtr = &SD_CFG_REF(SdClientService_t, SdClientServicesRef, 0U);
  /* get the last client service index of this instance in a constant variable */
  const uint16_least ServiceIndexEnd = ServiceIndex + InstanceCfgPtr->SdClientServiceCount;
  uint16_least ClientEventsCounter = (uint16_least)ClientEventsCount;
  uint16_least ClientOffersSubAcksCounter = (uint16_least)ClientOffersSubAcksCount;

#if (SD_DEV_ERROR_DETECT == STD_ON)
  /* this defensive check covers where there might be no configured client services for this instance */
  SD_PRECONDITION_ASSERT( (ServiceIndexEnd > ServiceIndex), SD_CLIENTSERVICEMAINTIMERS_SVCID );
  /* this defensive check protects against the case where no events are recorded */
  SD_PRECONDITION_ASSERT( (ClientEventsCount != 0U) || (ClientOffersSubAcksCount != 0U), SD_CLIENTSERVICEMAINFUNCTION_SVCID );
#endif

  ClientServiceCfgPtr = &ClientServiceCfgPtr[ServiceIndex];
  do
  {
/* -----------------------------------  HANDLE BSWM EVENTS  ---------------------------------------------------------------------- */

    /* if there are BswM events in this instance to be processed and if there are BswM events in this service */
    if(ClientServiceStatePtr->EventFlags != 0U)
    {
      uint8 Events;
      /* read service BswM events in a local variable and initialize the flags */
      SchM_Enter_Sd_SCHM_SD_EXCLUSIVE_AREA_0();
      Events = ClientServiceStatePtr->EventFlags;
      ClientServiceStatePtr->EventFlags = 0U;
      SchM_Exit_Sd_SCHM_SD_EXCLUSIVE_AREA_0();


      if((Events & SD_CLIENT_EVENT_HALT) != 0U)
      { /* SD_CLIENT_EVENT_HALT means that function Sd_LocalIpAddrAssignmentChg() is called with a state other than TCPIP_IPADDR_STATE_ASSIGNED,
          therefore ALL client services of this instance which has been halted are going to be set to DOWN state */
        Sd_StopTimer(&ClientServiceStatePtr->StateMachineTimer);
        --ClientEventsCounter;

        if((Events & SD_CLIENT_EVENT_START) == 0U)
        { /* !LINKSTO Sd.ASR40.SWS_SD_00712,1 */
          /* this client service is going to be completely stopped */
          Sd_ClientService_ComSM_event_stop(ClientServiceCfgPtr, ClientServiceStatePtr, (uint16)ServiceIndex, FALSE, FALSE);
        }
        else
        {
          Sd_ClientService_ComSM_event_stop(ClientServiceCfgPtr, ClientServiceStatePtr, (uint16)ServiceIndex, FALSE, TRUE);
          --ClientEventsCounter;

          Sd_ClientService_ComSM_entry_INITIALWAIT(ClientServiceCfgPtr, ClientServiceStatePtr, InstanceStatePtr);
        }
      }
      else
      { /* this client service has been stopped */
        if((Events & SD_CLIENT_EVENT_STOP) != 0U)
        {
          if(ClientServiceStatePtr->StateMachineTimer != 0U)
          {
            Sd_StopTimer(&ClientServiceStatePtr->StateMachineTimer);
#if (SD_DEV_ERROR_DETECT == STD_ON)
            /* this defensive check covers the case of an underflow in the variable ClientServicesTimersActive */
            SD_PRECONDITION_ASSERT( (InstanceStatePtr->ClientServicesTimersActive > 0U), SD_CLIENTSERVICEMAINFUNCTION_SVCID );
#endif
            --InstanceStatePtr->ClientServicesTimersActive;
            if(InstanceStatePtr->ClientServicesTimersActive == 0U)
            {
              InstanceStatePtr->ClientsDeltaTimer = 0U;
            }
          }
          Sd_ClientService_ComSM_event_stop(ClientServiceCfgPtr, ClientServiceStatePtr, (uint16)ServiceIndex, TRUE, (Events & SD_CLIENT_EVENT_START));
          --ClientEventsCounter;
        }
        /* check if an START event has happen in this same main cycle too */
        if((Events & SD_CLIENT_EVENT_START) != 0U)
        {
          /* make sure this service is switched off (DOWN state) otherwise this event is redundant and not processed */
#if (SD_DEV_ERROR_DETECT == STD_ON)
          /* this defensive check covers the case of an underflow in the variable ClientServicesTimersActive */
          SD_PRECONDITION_ASSERT( (ClientServiceStatePtr->ComPhase == SD_CLIENT_SERVICE_COMPHASE_DOWN), SD_CLIENTSERVICEMAINFUNCTION_SVCID );
#endif
          if(Events == SD_CLIENT_EVENT_START)
          { /* only if there was not stop or halt event in this same main cycle */
            Sd_ClientService_ComSM_exit_DOWN(ClientServiceCfgPtr);
          }

          /* this client service had been requested (it had received an offer) while it was in DOWN state */
          if(ClientServiceStatePtr->OfferReceived != FALSE)
          {  /* then it goes directly to MAIN state */
            ClientServiceStatePtr->ComPhase = SD_CLIENT_SERVICE_COMPHASE_MAIN;
            ClientServiceStatePtr->SdOfferEvents |= SD_CLIENT_EVENT_MULTICAST_OFFER;
            ++ClientOffersSubAcksCounter;
          }
          else /* else, it had not been requested, so it goes to INITIAL WAIT state */
          {
            Sd_ClientService_ComSM_entry_INITIALWAIT(ClientServiceCfgPtr, ClientServiceStatePtr, InstanceStatePtr);
          }
          /* decrement the counter with the total number of events processed for this instance */
          --ClientEventsCounter;
        }
      }
    }

/* ---------------------------------  HANDLE SD EVENTS (OFFER AND STOP OFFER EVENTS) -------------------------------------------------------------- */
    /* if there are SD events in this instance to be processed and if there are SD events in this service */
    if(ClientServiceStatePtr->SdOfferEvents != 0U)
    {
      Sd_ClientService_ProcessOffers(InstanceStatePtr, ClientServiceCfgPtr, ClientServiceStatePtr, &ClientOffersSubAcksCounter, (uint16)ServiceIndex);
    }

/* -----------------------------------------  HANDLE SUBSCRIBE ACKNOWLEDGE EVENTS ------------------------------------------------------------------ */
    if(ClientServiceStatePtr->SubsAckEvent != FALSE)
    {
      ClientServiceStatePtr->SubsAckEvent = FALSE;

      Sd_Sd_ClientService_ProcessSubsAcks(InstanceStatePtr, ClientServiceCfgPtr, ClientServiceStatePtr, &ClientOffersSubAcksCounter);
    }

    ++ServiceIndex;
    ++ClientServiceStatePtr;
    ++ClientServiceCfgPtr;

    /* this loop runs until all event counters (ClientOfferEventsCounter and ClientEventsCounter) are both 0,
     * or until it has been through all services.
     * If any of these two counters is still different to 0, it means there are still events to be processed.
     */
  }
  while(  ( (ClientOffersSubAcksCounter != 0U) || (ClientEventsCounter != 0U) ) && (ServiceIndex < ServiceIndexEnd)  );
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ClientService_mainTimersFunction
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr
)
{
  uint16_least ServiceIndex = InstanceCfgPtr->SdClientService;
  P2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ClientServiceStatePtr = &Sd_ClientServiceStates[ServiceIndex];
  P2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ClientServiceCfgPtr = &SD_CFG(SdClientService_t, SdClientServicesRef, ServiceIndex);
  const uint16_least ServiceIndexEnd = ServiceIndex + InstanceCfgPtr->SdClientServiceCount;
  uint16_least ActiveTimers = (uint16_least)InstanceStatePtr->ClientServicesTimersActive;

#if (SD_DEV_ERROR_DETECT == STD_ON)
  /* this defensive check covers where there might be no configured client services for this instance */
  SD_PRECONDITION_ASSERT( (ServiceIndexEnd > ServiceIndex), SD_CLIENTSERVICEMAINTIMERS_SVCID );
#endif

  do
  {
 /* ---------------------------------  HANDLE CYCLIC TIMERS -------------------------------------------------------------- */
    /* Check client service state machine timers, used only during INITIAL WAIT and REPETITION phases */
    if(ClientServiceStatePtr->StateMachineTimer > 0U)
    {
#if (SD_DEV_ERROR_DETECT == STD_ON)
      /* this defensive check covers the case of an underflow in the variable TimeOutDelta */
      SD_PRECONDITION_ASSERT( (ActiveTimers > 0U), SD_CLIENTSERVICEMAINTIMERS_SVCID );
#endif
      --ActiveTimers;

#if (SD_DEV_ERROR_DETECT == STD_ON)
      /* this defensive check covers the case of an underflow in the variable StateMachineTimer */
      SD_PRECONDITION_ASSERT( (ClientServiceStatePtr->StateMachineTimer >= InstanceStatePtr->ClientsDelta), SD_CLIENTSERVICEMAINTIMERS_SVCID );
#endif
      ClientServiceStatePtr->StateMachineTimer -= InstanceStatePtr->ClientsDelta;

      /* timer reached 0, check what time out action has to be executed  */
      if(ClientServiceStatePtr->StateMachineTimer == 0U)
      {
        Sd_ClientService_ComSM_event_timeout(ClientServiceCfgPtr, ClientServiceStatePtr, (uint16)ServiceIndex);
      }

      /* StateMachineTimer variable can change in the function "Sd_ClientService_ComSM_event_timeout()" thats why its value is checked
        here again to see if the new calculated timer is bigger than 0 */
      if(ClientServiceStatePtr->StateMachineTimer != 0U)
      {
        if( (InstanceStatePtr->ClientsDeltaTimer == 0U) || (ClientServiceStatePtr->StateMachineTimer < InstanceStatePtr->ClientsDeltaTimer) )
        { /* set next timeout */
          InstanceStatePtr->ClientsDeltaTimer = ClientServiceStatePtr->StateMachineTimer;
        }
         /* else nothing to do here, the new timer for this service is bigger or equal than the next timeout timer */
      }
      /* new timer for this service is equal to 0, a timer is no longer running for this server service */
      else
      {
#if (SD_DEV_ERROR_DETECT == STD_ON)
        /* this defensive check covers the case of an underflow in the variable ClientServicesTimersActive */
        SD_PRECONDITION_ASSERT( (InstanceStatePtr->ClientServicesTimersActive > 0U), SD_CLIENTSERVICEMAINTIMERS_SVCID );
#endif
        --InstanceStatePtr->ClientServicesTimersActive;
      }
    }
    ++ServiceIndex;
    ++ClientServiceStatePtr;
    ++ClientServiceCfgPtr;
  }
  while( (ActiveTimers > 0U) && (ServiceIndex < ServiceIndexEnd) );
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ClientService_mainTTL
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr
)
{
  uint16_least ServiceIndex = InstanceCfgPtr->SdClientService;
  P2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ClientServiceStatePtr = &Sd_ClientServiceStates[ServiceIndex];
  P2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ClientServiceCfgPtr = &SD_CFG(SdClientService_t, SdClientServicesRef, ServiceIndex);
  const uint16_least ServiceIndexEnd = ServiceIndex + InstanceCfgPtr->SdClientServiceCount;

  do
  {
 /* ---------------------------------  HANDLE TTL timers -------------------------------------------------------------- */
    /* check if there is a living OFFER to this client */
    if(ClientServiceStatePtr->TTLreceived != SD_TTL_OFF)
    { /* check if the time to live of this offer is not infinite (always on) */
      if(ClientServiceStatePtr->TTLreceived > SD_TTL_ALWAYS_ON)
      {
#if (SD_DEV_ERROR_DETECT == STD_ON)
        /* this defensive check covers the case of an underflow in the variable TTLreceived */
        SD_PRECONDITION_ASSERT( (ClientServiceStatePtr->TTLreceived >= InstanceStatePtr->ClientsDeltaTTL), SD_CLIENTSERVICEMAINTTL_SVCID );
#endif
        /* subtract previous minimum delta */
        ClientServiceStatePtr->TTLreceived -= (InstanceStatePtr->ClientsDeltaTTL - SD_TTL_TIMED_OUT);

        /* check it this TTL has run out */
        if(ClientServiceStatePtr->TTLreceived <= SD_TTL_TIMED_OUT)
        { /* TTL of service expired */
#if(SD_DEM_DETECT_SD_E_SERVER_NOT_AVAILABLE != TS_PROD_ERR_DISABLE) || (SD_SERVER_NOT_AVAILABLE_CALLBACK == STD_ON)
          /* !LINKSTO Sd.EB.DEMReporting.ServerService_Not_Available,1 */
          if(ClientServiceStatePtr->RequestedState == SD_CLIENT_SERVICE_REQUESTED)
          {
#if(SD_DEM_DETECT_SD_E_SERVER_NOT_AVAILABLE == TS_PROD_ERR_REP_TO_DEM)
            SD_DEM_REPORT_ERROR_STATUS(InstanceCfgPtr->SdDemServerNotAvailableId, DEM_EVENT_STATUS_FAILED);
#elif((SD_DEM_DETECT_SD_E_SERVER_NOT_AVAILABLE == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON))
            SD_DET_REPORT_ERROR(SD_RXINDICATION_SVCID, SD_E_SERVER_NOT_AVAILABLE_DEMTODET);
#endif
#if(SD_SERVER_NOT_AVAILABLE_CALLBACK == STD_ON)
            {
              P2VAR(TcpIp_SockAddrType, TYPEDEF, SD_APPL_DATA) UdpRemoteAddrPtr;
              P2VAR(TcpIp_SockAddrType, TYPEDEF, SD_APPL_DATA) TcpRemoteAddrPtr;

              TcpIp_SockAddrInet6Type UdpRemoteAddr;
              TcpIp_SockAddrInet6Type TcpRemoteAddr;

              UdpRemoteAddr.domain = InstanceStatePtr->TxState.Address.domain;
              TcpRemoteAddr.domain = InstanceStatePtr->TxState.Address.domain;

              if(ClientServiceStatePtr->SoConIdUdp != SOAD_INVALID_SOCON_ID)
              {
                (void)SoAd_GetRemoteAddr(ClientServiceStatePtr->SoConIdUdp, (P2VAR(TcpIp_SockAddrType, TYPEDEF, SD_APPL_DATA))(void *)&UdpRemoteAddr);
                UdpRemoteAddrPtr = (P2VAR(TcpIp_SockAddrType, TYPEDEF, SD_APPL_DATA))(void *)&UdpRemoteAddr;
              }
              else
              {
                UdpRemoteAddrPtr = NULL_PTR;
              }

              if(ClientServiceStatePtr->SoConIdTcp != SOAD_INVALID_SOCON_ID)
              {
                (void)SoAd_GetRemoteAddr(ClientServiceStatePtr->SoConIdUdp, (P2VAR(TcpIp_SockAddrType, TYPEDEF, SD_APPL_DATA))(void *)&TcpRemoteAddr);
                TcpRemoteAddrPtr = (P2VAR(TcpIp_SockAddrType, TYPEDEF, SD_APPL_DATA))(void *)&TcpRemoteAddr;
              }
              else
              {
                TcpRemoteAddrPtr = NULL_PTR;
              }
              /* !LINKSTO Sd.EB.CallBack.ServerService_Not_Available,1 */
              Sd_User_Server_Not_Available_Fp(ClientServiceCfgPtr->SdClientServiceID, ClientServiceCfgPtr->SdClientServiceInstanceId, UdpRemoteAddrPtr, TcpRemoteAddrPtr);
            }
#endif /* SD_SERVER_NOT_AVAILABLE_CALLBACK == STD_ON */
          }
#endif /* (SD_DEM_DETECT_SD_E_SERVER_NOT_AVAILABLE != TS_PROD_ERR_DISABLE) || (SD_SERVER_NOT_AVAILABLE_CALLBACK == STD_ON) */
          Sd_ClientService_TTL_Run_Out(ClientServiceCfgPtr, ClientServiceStatePtr, (uint16)ServiceIndex);
          {
            P2VAR(Sd_RemoteNodeStateType,AUTOMATIC,SD_APPL_DATA) RemoteNodeStatePtr = Sd_GetRemoteNodeState(ClientServiceStatePtr->NodeIndex);
#if (SD_DEV_ERROR_DETECT == STD_ON)
            /* this defensive check covers the case of an underflow in the variable ClientServicesTimersActive */
            SD_PRECONDITION_ASSERT( (RemoteNodeStatePtr->ClientServicesOffersReceived > 0U), SD_CLIENTSERVICEMAINTTL_SVCID );
#endif
            --RemoteNodeStatePtr->ClientServicesOffersReceived;
            if( (RemoteNodeStatePtr->ClientServicesOffersReceived == 0U) && (RemoteNodeStatePtr->ServerServicesFindsReceived == 0U) )
            {
              RemoteNodeStatePtr->ResponseTimer = 0U;
            }
            ClientServiceStatePtr->OfferReceived = FALSE;
            ClientServiceStatePtr->NodeIndex = SD_INVALID_REMOTENODE_ID;
            if(ClientServiceStatePtr->SoConIdUdp != SOAD_INVALID_SOCON_ID)
            {
              SoAd_ReleaseRemoteAddr(ClientServiceStatePtr->SoConIdUdp);
              ClientServiceStatePtr->SoConIdUdp = SOAD_INVALID_SOCON_ID;
            }
            if(ClientServiceStatePtr->SoConIdTcp != SOAD_INVALID_SOCON_ID)
            {
              SoAd_ReleaseRemoteAddr(ClientServiceStatePtr->SoConIdTcp);
              ClientServiceStatePtr->SoConIdTcp = SOAD_INVALID_SOCON_ID;
            }
          }

          if(ClientServiceStatePtr->StateMachineTimer != 0U)
          {
            ++InstanceStatePtr->ClientServicesTimersActive;
          }
        }
        else /* no timeout, then check if this newly calculated TTL is the smallest one so far */
        {
          if( (InstanceStatePtr->ClientsDeltaTTLtimer <= SD_TTL_ALWAYS_ON) || (ClientServiceStatePtr->TTLreceived < InstanceStatePtr->ClientsDeltaTTLtimer) )
          { /* for now this is the smallest active TTL, set next timeout */
            InstanceStatePtr->ClientsDeltaTTLtimer = ClientServiceStatePtr->TTLreceived;
          }
        }
      }
      /* TTLreceived == 0 could mean TTL infinite, so check the consumed event dependent on this client service */
      {
        uint16_least EventgroupIndex = ClientServiceCfgPtr->SdConsumedEventGroup;
        const uint16_least EventgroupIndexEnd = EventgroupIndex + ClientServiceCfgPtr->SdConsumedEventGroupCount;
        P2VAR(Sd_ConsumedEventGroupStateType,AUTOMATIC,SD_APPL_CONST) EventGroupStatePtr = &Sd_ConsumedEventGroupStates[EventgroupIndex];
        /* binary search is not possible here because not just an element is searched for but several */
        while(EventgroupIndex < EventgroupIndexEnd)
        {
          /* check if subscription acknowledge to this consumed event group still lives */
          if(EventGroupStatePtr->TTL > SD_TTL_ALWAYS_ON)
          {
#if (SD_DEV_ERROR_DETECT == STD_ON)
            /* this defensive check covers the case of an underflow in the variable TTL */
            SD_PRECONDITION_ASSERT( (EventGroupStatePtr->TTL >= InstanceStatePtr->ClientsDeltaTTL), SD_CLIENTSERVICEMAINTTL_SVCID );
#endif
            /* timer is decremented and it is checked if its value reached the timeout value */
            EventGroupStatePtr->TTL -= (InstanceStatePtr->ClientsDeltaTTL - 1U);

            if(EventGroupStatePtr->TTL <= SD_TTL_TIMED_OUT)
            { /* timer has run out, this consumed event group is no longer in the machine state AVAILABLE */
              P2CONST(SdConsumedEventGroup_t,AUTOMATIC,SD_APPL_CONST) ConsumedEventGroupCfgRefPtr = &SD_CFG_REF(SdConsumedEventGroup_t, SdConsumedEventGroupsRef, 0U);
              Sd_ClientService_CEvg_exit_AVAILABLE(&ConsumedEventGroupCfgRefPtr[EventgroupIndex], EventGroupStatePtr, (uint16)EventgroupIndex);
              /* set consumed event group to machine state: consumed event group requested, and client has received and offer received */
              EventGroupStatePtr->MachineState = SD_CONSUMED_EVENTGROUP_STATE_REQUESTED_OFFER_RECEIVED;
            }
            else /* this TTL has not timed out, its new TTL delta has been calculated check if it is the smallest delta so far */
            {
              if( (InstanceStatePtr->ClientsDeltaTTLtimer <= SD_TTL_ALWAYS_ON) || (EventGroupStatePtr->TTL < InstanceStatePtr->ClientsDeltaTTLtimer) )
              { /* set next timeout */
                InstanceStatePtr->ClientsDeltaTTLtimer = EventGroupStatePtr->TTL;
              }
            }
          } /* else, this consumed eventgroups TTL is either OFF or infinite, so nothing to do as its state can't change due to the TTL */
          ++EventgroupIndex;
          ++EventGroupStatePtr;
        }
      }
    }
    ++ServiceIndex;
    ++ClientServiceStatePtr;
    ++ClientServiceCfgPtr;
  }
  while(ServiceIndex < ServiceIndexEnd);
}


TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ClientService_sendEntriesUnicast
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_RemoteNodeStateType,AUTOMATIC,SD_VAR) RemoteNodeStatePtr,
  const Sd_RemoteNodeIdType RemoteNodeIndex
)
{
  uint16_least ServiceIndex = InstanceCfgPtr->SdClientService;
  P2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ClientServiceStatePtr = &Sd_ClientServiceStates[ServiceIndex];
  const uint16_least ServiceIndexEnd = ServiceIndex + InstanceCfgPtr->SdClientServiceCount;
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ClientServiceCfgRefPtr = &SD_CFG_REF(SdClientService_t, SdClientServicesRef, 0U);
  uint16 ClientsSubscribessToProcess = RemoteNodeStatePtr->ClientServicesOffersReceived;

  /* Send SubscribeEventgroup entries, binary search is not possible here because not just an element is searched for but several */
  do
  {
    if(ClientServiceStatePtr->NodeIndex == RemoteNodeIndex)
    {
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
      const uint16 SdSubscribeEventGroupsNotAckPrev = ClientServiceStatePtr->SdSubscribeEventGroupsNotAck;
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */

      Sd_ClientService_sendSubscribeEventGroupsOfService(&ClientServiceCfgRefPtr[ServiceIndex], ClientServiceStatePtr, RemoteNodeIndex);
      --ClientsSubscribessToProcess;

#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
      /* if in this client service there is at least one of consumed event group that has been subscribed, then
        the counter with the number of client services with consumed event groups still not acknowledged is in turn increased */
      if(ClientServiceStatePtr->SdSubscribeEventGroupsNotAck > SdSubscribeEventGroupsNotAckPrev)
      {
        ++RemoteNodeStatePtr->ClientServicesRetriesToProcess;
      }
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
    }
    ++ServiceIndex;
    ++ClientServiceStatePtr;
  }
  while( (ClientsSubscribessToProcess > 0U) && (ServiceIndex < ServiceIndexEnd) );
}



#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
TS_MOD_PRIV_DEFN FUNC(uint16, SD_CODE) Sd_ClientService_sendRetriesOfSubscribeEventGroups
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  uint16 RemoteNodeClientRetriesToSend
)
{
  uint16_least ServiceIndex = InstanceCfgPtr->SdClientService;
  const uint16_least ServiceIndexEnd = ServiceIndex + InstanceCfgPtr->SdClientServiceCount;
  P2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ClientServiceStatePtr = &Sd_ClientServiceStates[ServiceIndex];
  P2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ClientServiceCfgPtr = &SD_CFG(SdClientService_t, SdClientServicesRef, ServiceIndex);
  uint16 RemoteNodeClientRetriesToProcess = 0U;

  /* Send SubscribeEventgroup entries, binary search is not possible here because not just an element is searched for but several */
  do
  {
    if( (ClientServiceStatePtr->SdSubscribeEventGroupsNotAck > 0U) && (ClientServiceStatePtr->NodeIndex == RemoteNodeIndex) )
    {
      uint16_least EventgroupIndex = ClientServiceCfgPtr->SdConsumedEventGroup;
      const uint16_least EventgroupIndexEnd = EventgroupIndex + ClientServiceCfgPtr->SdConsumedEventGroupCount;
      P2VAR(Sd_ConsumedEventGroupStateType,AUTOMATIC,SD_APPL_CONST) EventGroupStatePtr = &Sd_ConsumedEventGroupStates[EventgroupIndex];
      uint16 SdClientRetriesSubscribeEventgroups = ClientServiceStatePtr->SdSubscribeEventGroupsNotAck;
      /* !LINKSTO Sd.ASR40.SWS_SD_00703,1 */
      const SoAd_SoConIdType UnicastUdpSoConId = ClientServiceStatePtr->SoConIdUdp;
      /* !LINKSTO Sd.ASR40.SWS_SD_00702,1 */
      const SoAd_SoConIdType UnicastTcpSoConId = ClientServiceStatePtr->SoConIdTcp;

      ClientServiceStatePtr->SdSubscribeEventGroupsNotAck = 0U;
      do
      { /* check if retry counter of this consumed event group is different to 0, which would mean that its previous subscription has not been acknowledged yet */
        if(EventGroupStatePtr->SdSubscribeEventgroupRetryCounter > 0U)
        { /* !LINKSTO Sd.ASR40.SWS_SD_00737,1 */
          /* !LINKSTO Sd.ASR40.SWS_SD_00739,1 */
          /* if the number of retries has already been reached, then retry counter is initialized to 0 */
          if(EventGroupStatePtr->SdSubscribeEventgroupRetryCounter <= InstanceCfgPtr->SdSubscribeEventgroupRetryMax)
          { /* if current value of the retry counter has reached the maximum then it is initialized back to 1 */
            if(EventGroupStatePtr->SdSubscribeEventgroupRetryCounter == SD_INFINITE_RETRIES)
            { /* retries continue indefinitely until either an ACK or a NACK are received */
              EventGroupStatePtr->SdSubscribeEventgroupRetryCounter = 1U;
            }
            else
            { /* counter of retries is incremented */
              ++EventGroupStatePtr->SdSubscribeEventgroupRetryCounter;
            }
            /* first a STOP SUBSCRIBE is sent out to cancel the previous one */
            Sd_Send_queueSubscribeEventgroup((uint16)EventgroupIndex, UnicastUdpSoConId, UnicastTcpSoConId, TRUE);
            /* then a retry of previous SUBSCRIBE is sent out */
            Sd_Send_queueSubscribeEventgroup((uint16)EventgroupIndex, UnicastUdpSoConId, UnicastTcpSoConId, FALSE);
            /* this consumed event group has sent its retry but as the counter is not exceeded yet it will have to send a further retry,
              therefore counter with the number of event groups with an ongoing retry is increased */
            ++ClientServiceStatePtr->SdSubscribeEventGroupsNotAck;
          }
          else
          { /* !LINKSTO Sd.ASR40.SWS_SD_00738,1 */
            /* the number of retries has been exceeded and this consumed event group has not been acknowledged */
            EventGroupStatePtr->SdSubscribeEventgroupRetryCounter = 0U;
            /* !LINKSTO Sd.ASR40.SWS_SD_00742,1 */
#if (SD_DEV_ERROR_DETECT == STD_ON)
            /* a development error is generated here to indicate that the retry counter has been exceeded and this consumed event group has not been acknowledged */
            SD_DET_REPORT_ERROR(SD_CLIENTSERVICE_COMSM_EVENT_TIMEOUT,SD_E_COUNT_OF_RETRY_SUBSCRIPTION_EXCEEDED);
#endif /* SD_DEV_ERROR_DETECT == STD_ON */
          }
          --SdClientRetriesSubscribeEventgroups;
        }
        ++EventgroupIndex;
        ++EventGroupStatePtr;
      }
      while( (SdClientRetriesSubscribeEventgroups > 0U) && (EventgroupIndex < EventgroupIndexEnd) );

      if(ClientServiceStatePtr->SdSubscribeEventGroupsNotAck > 0U)
      {
        ++RemoteNodeClientRetriesToProcess;
      }
      --RemoteNodeClientRetriesToSend;
    }
    ++ServiceIndex;
    ++ClientServiceStatePtr;
    ++ClientServiceCfgPtr;
  }
  while( (RemoteNodeClientRetriesToSend > 0U) && (ServiceIndex < ServiceIndexEnd) );

  return RemoteNodeClientRetriesToProcess;
}
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */



#if(  (SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DEM) || \
      ( (SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON) )  )
/* Deviation CODEMETRIC-1 */
TS_MOD_PRIV_DEFN FUNC(uint8, SD_CODE) Sd_ClientService_eventOfferService
(
  CONSTP2CONST(Sd_ServiceInfoType, AUTOMATIC, SD_APPL_DATA) ServiceInfoPtr,
  CONSTP2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfoPtr,
  const uint32 TTL,
  const uint32 MinorVersion,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  const uint8 SdInstanceIndex,
  const boolean IsMulticast
)
{
#if( (SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DEM) || ((SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON)) )
  uint8 DemErrorCode = 0U;
#endif
#else
/* Deviation CODEMETRIC-1 */
TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ClientService_eventOfferService
(
  CONSTP2CONST(Sd_ServiceInfoType, AUTOMATIC, SD_APPL_DATA) ServiceInfoPtr,
  CONSTP2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfoPtr,
  const uint32 TTL,
  const uint32 MinorVersion,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  const uint8 SdInstanceIndex,
  const boolean IsMulticast
)
{
#endif
  if( (ServiceInfoPtr->ServiceID != SD_WILDCARD_SERVICEID) &&
      (ServiceInfoPtr->InstanceID != SD_WILDCARD_INSTANCEID) &&
      (MinorVersion != SD_WILDCARD_MINORVERSION) )
  {
    /* search for a client service that matches exactly the end-point received in EntryOptionInfoPtr */
    const uint16 ServiceIndex = Sd_ClientService_QuickMatch(ServiceInfoPtr, EntryOptionInfoPtr, MinorVersion, SdInstanceIndex, TRUE);

    if(ServiceIndex != SD_WILDCARD_SERVICEID)
    {
      CONSTP2VAR(Sd_InstanceStateType, AUTOMATIC, SD_APPL_DATA) InstanceStatesPtr = &Sd_InstanceStates[SdInstanceIndex];
      CONSTP2VAR(Sd_ClientServiceStateType, AUTOMATIC, SD_APPL_DATA) ClientServiceStatePtr = &Sd_ClientServiceStates[ServiceIndex];
      boolean bNewRemoteNode = FALSE;

      /* make sure that this service has not received an OFFER, that still lives, from a different remote node */
      if( (ClientServiceStatePtr->OfferReceived == FALSE) || (ClientServiceStatePtr->NodeIndex == RemoteNodeIndex) )
      { /* no living offer from another remote node is present */
        P2CONST(SdClientService_t, AUTOMATIC, SD_APPL_DATA) ServiceCfgPtr = &SD_CFG(SdClientService_t, SdClientServicesRef, ServiceIndex);
        Std_ReturnType RetValUdp = E_NOT_OK;
        Std_ReturnType RetValTcp = E_NOT_OK;
        SoAd_SoConIdType SoConId;

        /* if the incoming entry has an UDP endpoint and this client service has an UDP reference configured */
        if(EntryOptionInfoPtr->UdpEndpointOption.domain != TCPIP_AF_UNSPEC)
        {
          if (ServiceCfgPtr->SdClientServiceSoConGroupUdp != SD_INVALID_SOCONGROUP_ID)
          {
            P2CONST(SdSocketConnectionGroupType,AUTOMATIC,SD_APPL_CONST) SoConGroupPtr = &SD_CFG_REF(SdSocketConnectionGroupType, SdSoConGroupsRef, 0U);
            RetValUdp = SoAd_SetUniqueRemoteAddr(
                SoConGroupPtr[ServiceCfgPtr->SdClientServiceSoConGroupUdp].GroupStart,
                /* Deviation MISRAC2012-1 */
                (TcpIp_SockAddrType*)(void*)&EntryOptionInfoPtr->UdpEndpointOption,
                &SoConId);

            if(RetValUdp == E_OK)
            {
              if( (ClientServiceStatePtr->SoConIdUdp == SOAD_INVALID_SOCON_ID) || (ClientServiceStatePtr->SoConIdUdp == SoConId) )
              {
                if(ClientServiceStatePtr->SoConIdUdp == SOAD_INVALID_SOCON_ID)
                {
                  ClientServiceStatePtr->SoConIdUdp = SoConId;
                  /* store the TxQueue index for later transmissions */
                  ClientServiceStatePtr->NodeIndex = RemoteNodeIndex;
                  bNewRemoteNode = TRUE;
                }
                else
                { /* !LINKSTO Sd.ASR40.SWS_SD_00569,1 */
                  /* decrease counter increased by SoAd_SetUniqueRemoteAddress */
                  SoAd_ReleaseRemoteAddr(SoConId);
                }
              }
              else
              { /* !LINKSTO Sd.ASR40.SWS_SD_00569,1 */
                SoAd_ReleaseRemoteAddr(SoConId);
                RetValUdp = E_NOT_OK;
              }
            }
#if( (SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DEM) || ((SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON)) )
            else
            {
              DemErrorCode = DEM_OUT_OF_RESOURCES_UDP_BITMAP;
            }
#endif
          }
          /* This client does not have a configured UDP end-point although it has received an OFFER with an UDP endpoint,
           * if the OFFER does not have a TCP endpoint then this will be treated as a multicast only consumed event */
          else if( (EntryOptionInfoPtr->TcpEndpointOption.domain == TCPIP_AF_UNSPEC) ||
                   (ServiceCfgPtr->SdClientServiceSoConGroupTcp == SD_INVALID_SOCONGROUP_ID) )
          {
            if(ClientServiceStatePtr->NodeIndex == SD_INVALID_REMOTENODE_ID)
            {
              ClientServiceStatePtr->NodeIndex = RemoteNodeIndex;
              bNewRemoteNode = TRUE;
            }
            RetValUdp = E_OK;
          }
          else
          {
            /* This OFFER has an UDP and a TCP endpoint at the same time, the TCP endpoint is going to be processed right below in this same function */
          }
        }
        /* if the incoming entry has an TCP endpoint and this client service has an TCP reference configured */
        if( (EntryOptionInfoPtr->TcpEndpointOption.domain != TCPIP_AF_UNSPEC) && (ServiceCfgPtr->SdClientServiceSoConGroupTcp != SD_INVALID_SOCONGROUP_ID) )
        {
          P2CONST(SdSocketConnectionGroupType,AUTOMATIC,SD_APPL_CONST) SoConGroupPtr = &SD_CFG_REF(SdSocketConnectionGroupType, SdSoConGroupsRef, 0U);
          RetValTcp = SoAd_SetUniqueRemoteAddr(SoConGroupPtr[ServiceCfgPtr->SdClientServiceSoConGroupTcp].GroupStart,
                                               /* Deviation MISRAC2012-1 */
                                               (TcpIp_SockAddrType*)(void*)&EntryOptionInfoPtr->TcpEndpointOption,
                                               &SoConId);

          if(RetValTcp == E_OK)
          {
            if( (ClientServiceStatePtr->SoConIdTcp == SOAD_INVALID_SOCON_ID) || (ClientServiceStatePtr->SoConIdTcp == SoConId) )
            {
              if(ClientServiceStatePtr->SoConIdTcp == SOAD_INVALID_SOCON_ID)
              {
                ClientServiceStatePtr->SoConIdTcp = SoConId;
                /* store the TxQueue index for later transmissions */
                ClientServiceStatePtr->NodeIndex = RemoteNodeIndex;
                bNewRemoteNode = TRUE;
              }
            }
            else
            { /* !LINKSTO Sd.ASR40.SWS_SD_00569,1 */
              SoAd_ReleaseRemoteAddr(SoConId);
              RetValTcp = E_NOT_OK;
            }
          }
#if( (SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DEM) || ((SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON)) )
          else
          {
            DemErrorCode |= DEM_OUT_OF_RESOURCES_TCP_BITMAP;
          }
#endif
        }

        if( (RetValUdp == E_OK) || (RetValTcp == E_OK) )
        { /* !LINKSTO Sd.ASR40.SWS_SD_00352,1 */
          /* !LINKSTO Sd.ASR40.SWS_SD_00365,1 */
          /* !LINKSTO Sd.ASR40.SWS_SD_00376,1 */
          if(TTL != SD_ENTRYTTL_INFINITE)
          {
            ClientServiceStatePtr->TTLreceived = (TTL * SD_LOOPS_PER_SECOND) + 1U;

            if( (InstanceStatesPtr->ClientsDeltaTTLtimer <= SD_TTL_ALWAYS_ON) || (ClientServiceStatePtr->TTLreceived < InstanceStatesPtr->ClientsDeltaTTLtimer) )
            {
              InstanceStatesPtr->ClientsDeltaTTLtimer = ClientServiceStatePtr->TTLreceived;
              InstanceStatesPtr->ClientsDeltaTTL = ClientServiceStatePtr->TTLreceived;
            }
            else /* else align this TTL so that it is calculated correctly in the next time out */
            {
              ClientServiceStatePtr->TTLreceived += (InstanceStatesPtr->ClientsDeltaTTL - InstanceStatesPtr->ClientsDeltaTTLtimer);
            }
          }
          /* !LINKSTO Sd.ASR40.SWS_SD_00514,1 */
          else
          {
            ClientServiceStatePtr->TTLreceived = SD_TTL_ALWAYS_ON;
          }

          if(IsMulticast != FALSE)
          {
            ClientServiceStatePtr->SdOfferEvents |= SD_CLIENT_EVENT_MULTICAST_OFFER;
          }
          else
          {
            ClientServiceStatePtr->SdOfferEvents |= SD_CLIENT_EVENT_UNICAST_OFFER;
          }
          ClientServiceStatePtr->SdOfferEvents &= ~(uint8)SD_CLIENT_EVENT_STOPOFFER_1;
          ClientServiceStatePtr->OfferReceived = TRUE;
          InstanceStatesPtr->ClientServicesEventsCount += SD_INCREASE_CLIENT_OFFER_EVENT;
          /* check if this is the very first OFFER received by this client service from this remote node */
          if(bNewRemoteNode != FALSE)
          { /* first OFFER received from this remote ECU */
            P2VAR(Sd_RemoteNodeStateType,AUTOMATIC,SD_APPL_DATA) RemoteNodeStatePtr = Sd_GetRemoteNodeState(RemoteNodeIndex);
            ++RemoteNodeStatePtr->ClientServicesOffersReceived;
            RemoteNodeStatePtr->IsConnectionReadyTimer = (8U * SD_LOOPS_PER_SECOND);

#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
            {
              P2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr = &SD_CFG(SdInstance_t, SdInstancesRef, SdInstanceIndex);
              /* This client has received a OFFER with a TTL value equal to infinite then if this instance is configured to send retries of subscriptions
                to consumed event groups, then local variable "SdSubscribeEventGroupTimersActive" is set to TRUE to indicate it.
               */
              /* !LINKSTO Sd.ASR40.SWS_SD_00735,1 */
              if(InstanceCfgPtr->SdSubscribeEventgroupRetryMax > 0U)
              { /* this client service will send retries of SUBSCRIBEs */
                ClientServiceStatePtr->SdSubscribeEventGroupTimersActive = TRUE;
                ClientServiceStatePtr->SdSubscribeEventGroupsNotAck = 0U;
#if (SD_DEV_ERROR_DETECT == STD_ON)
                if(InstanceCfgPtr->SdSubscribeEventgroupRetryMax == SD_INFINITE_RETRIES)
                { /* if maximum number of retries configures is infinite then if the TTL of the received OFFER is not infinite then a det error is generated
                     to indicate this inconsistency in the system configuration */
                  if (ClientServiceStatePtr->TTLreceived != SD_TTL_ALWAYS_ON)
                  { /* !LINKSTO Sd.EB.DET.Error.Retry.Infinite.TTL.Not.Infinite,1 */
                    SD_DET_REPORT_ERROR(SD_CLIENTSERVICE_EVENT_OFFER_SERVICE_SVCID, SD_E_RETRY_INFINITE_TTL_NOT_INFINITE);
                  }
                }
                else /* number of retries is a finite number */
                { /* if the received OFFER is multicast and if its TTL timer is not infinite */
                  if( (ClientServiceStatePtr->SdOfferEvents == SD_CLIENT_EVENT_MULTICAST_OFFER) && (ClientServiceStatePtr->TTLreceived != SD_TTL_ALWAYS_ON) )
                  {
                    const uint32 TotalTimeRetryProcess = ServiceCfgPtr->SdClientTimerRequestResponseMinDelay +
                                      ((uint32)InstanceCfgPtr->SdSubscribeEventgroupRetryMax * (uint32)InstanceCfgPtr->SdSubscribeEventgroupRetryDelay);
                    /* if time need to respond to this multicast OFFER plus the time needed for generated all configured retries then this is an inconsistency
                     * on the system configuration and therefore a det error is generated to indicate it */
                    if (ClientServiceStatePtr->TTLreceived <= TotalTimeRetryProcess)
                    { /* !LINKSTO Sd.EB.DET.Error.Retry.Process.Bigger.Than.TTL,1 */
                      SD_DET_REPORT_ERROR(SD_CLIENTSERVICE_EVENT_OFFER_SERVICE_SVCID, SD_E_RETRY_PROCESS_BIGGER_THAN_TTL);
                    }
                  }
                }
#endif
              }
            }
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
          }
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
          else
          { /* Retry mechanism is re-started only for each multicast OFFER */
            if( (ClientServiceStatePtr->SdSubscribeEventGroupTimersActive != FALSE) && (ClientServiceStatePtr->SdOfferEvents == SD_CLIENT_EVENT_MULTICAST_OFFER) )
            { /* !LINKSTO Sd.ASR40.SWS_SD_00739,1 */
              /* !LINKSTO Sd.ASR40.SWS_SD_00740,1 */
              /* clear the number of not acknowledged consumed event groups */
              ClientServiceStatePtr->SdSubscribeEventGroupsNotAck = 0U;
            }
          }
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
        }
      }
    }
  }
#if(  (SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DEM) || \
      ( (SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON) )  )
  return DemErrorCode;
#endif
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ClientService_eventStopOfferService
(
  CONSTP2CONST(Sd_ServiceInfoType, AUTOMATIC, SD_APPL_DATA) ServiceInfoPtr,
  CONSTP2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfoPtr,
  const uint32 MinorVersion,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  const uint8 SdInstanceIndex
)
{
  if( (ServiceInfoPtr->ServiceID != SD_WILDCARD_SERVICEID) &&
      (ServiceInfoPtr->InstanceID != SD_WILDCARD_INSTANCEID) &&
      (MinorVersion != SD_WILDCARD_MINORVERSION) )
  {
    /* search for a client service that matches exactly the end-point received in EntryOptionInfoPtr */
    const uint16 ServiceIndex = Sd_ClientService_QuickMatch(ServiceInfoPtr, EntryOptionInfoPtr, MinorVersion, SdInstanceIndex, TRUE);

    if(ServiceIndex != SD_WILDCARD_SERVICEID)
    {
      CONSTP2VAR(Sd_InstanceStateType, AUTOMATIC, SD_APPL_DATA) InstanceStatesPtr = &Sd_InstanceStates[SdInstanceIndex];
      CONSTP2VAR(Sd_ClientServiceStateType, AUTOMATIC, SD_APPL_DATA) ClientServiceStatePtr = &Sd_ClientServiceStates[ServiceIndex];

      /* make sure that this service had already received an OFFER, that still lives, and from this same remote node */
      if( (ClientServiceStatePtr->TTLreceived != SD_TTL_OFF) && (ClientServiceStatePtr->NodeIndex == RemoteNodeIndex) )
      {
        ClientServiceStatePtr->SdOfferEvents |= (SD_CLIENT_EVENT_STOPOFFER_0 | SD_CLIENT_EVENT_STOPOFFER_1);
        ClientServiceStatePtr->TTLreceived = SD_TTL_OFF;
        ClientServiceStatePtr->OfferReceived = FALSE;
        InstanceStatesPtr->ClientServicesEventsCount += SD_INCREASE_CLIENT_OFFER_EVENT;
      }
    }
  }
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ClientService_eventSubscribeAck
(
  CONSTP2CONST(Sd_ServiceInfoType, AUTOMATIC, SD_APPL_DATA) ServiceInfoPtr,
  CONSTP2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfoPtr,
  const uint32 TTL,
  const uint32 MinorVersion,
  const uint16 EventgroupID,
  Sd_RemoteNodeIdType RemoteNodeIndex,
  const uint8 SdInstanceIndex
)
{
  if( (ServiceInfoPtr->ServiceID != SD_WILDCARD_SERVICEID) &&
      (ServiceInfoPtr->InstanceID != SD_WILDCARD_INSTANCEID) &&
      (MinorVersion != SD_WILDCARD_MINORVERSION) &&
      (EventgroupID != SD_WILDCARD_EVENTGROUPID) )
  {
    const uint16 ServiceIndex = Sd_ClientService_QuickMatch(ServiceInfoPtr, EntryOptionInfoPtr, MinorVersion, SdInstanceIndex, FALSE);

    /* search for a client service that matches exactly the end-point received in EntryOptionInfoPtr */
    if(ServiceIndex != SD_WILDCARD_SERVICEID)
    {
      CONSTP2VAR(Sd_ClientServiceStateType, AUTOMATIC, SD_APPL_DATA) ClientServiceStatePtr = &Sd_ClientServiceStates[ServiceIndex];
      /* only if this client service is still state AVAILABLE */
      if( (ClientServiceStatePtr->ComPhase == SD_CLIENT_SERVICE_COMPHASE_AVAILABLE)
        && (ClientServiceStatePtr->NodeIndex == RemoteNodeIndex) )
      {
        P2CONST(SdClientService_t, AUTOMATIC, SD_APPL_CONST) ServiceCfgPtr = &SD_CFG(SdClientService_t, SdClientServicesRef, ServiceIndex);
        {
          uint16_least EventgroupIndex = ServiceCfgPtr->SdConsumedEventGroup;
          const uint16_least EventgroupIndexEnd = EventgroupIndex + ServiceCfgPtr->SdConsumedEventGroupCount;
          P2CONST(SdConsumedEventGroup_t, AUTOMATIC, SD_APPL_CONST) ServiceConsumedEventGroupsRefCfgPtr =
              &SD_CFG_REF(SdConsumedEventGroup_t, SdConsumedEventGroupsRef, 0U);
          /* there is already binary search for the service match but not for event groups because usually there are 1 or 2 */
          do
          {
            if(ServiceConsumedEventGroupsRefCfgPtr[EventgroupIndex].SdConsumedEventGroupID == EventgroupID)
            {
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
              /* this consumed event group has been acknowledged, only if this client service was is enabled for
                sending consumed event group retries then the retry counter of this consumed group will be reset to 0
                because for this consumed event group as it is already acknowledged no more retries will be sent */
              if(ClientServiceStatePtr->SdSubscribeEventGroupTimersActive != FALSE)
              { /* !LINKSTO Sd.ASR40.SWS_SD_00738,1 */
                /* !LINKSTO Sd.ASR40.SWS_SD_00739,1 */
                Sd_ConsumedEventGroupStates[EventgroupIndex].SdSubscribeEventgroupRetryCounter = 0U;
              }
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
              if(TTL != 0U)
              {
                if(Sd_ClientService_CEvg_event_SubscribeEventgroupAck(
                   /* Deviation MISRAC2012-1 */
                   (TcpIp_SockAddrType *)(void *)&EntryOptionInfoPtr->MulticastOption,
                   TTL,
                   (uint16)EventgroupIndex,
                   SdInstanceIndex) != FALSE)
                {
                  Sd_InstanceStates[SdInstanceIndex].ClientServicesEventsCount += SD_INCREASE_CLIENT_OFFER_EVENT;
                  ClientServiceStatePtr->SubsAckEvent = TRUE;
                }
              }
              else
              {
#if(SD_DEM_DETECT_SD_E_SUBSCR_NACK_RECV == TS_PROD_ERR_REP_TO_DEM)
                P2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr = &SD_CFG(SdInstance_t, SdInstancesRef, SdInstanceIndex);
                SD_DEM_REPORT_ERROR_STATUS(InstanceCfgPtr->SdDemSubscrNackRecvId, DEM_EVENT_STATUS_FAILED);
#elif((SD_DEM_DETECT_SD_E_SUBSCR_NACK_RECV == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON))
                SD_DET_REPORT_ERROR(SD_RXINDICATION_SVCID,SD_E_SUBSCR_NACK_RECV_DEMTODET);
#endif
#if(SD_SUBSCR_NACK_RECV_CALLBACK == STD_ON)
                /* !LINKSTO Sd.EB.CallBack.SubscribeNackReceived,1 */
                Sd_User_SubscribeNackReceived_Fp();
#endif
              }
              break;
            }
            ++EventgroupIndex;
          }
          while(EventgroupIndex < EventgroupIndexEnd);
        }
      }
    }
  }
}


TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ClientService_startServices
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr
)
{
  uint16_least ServiceIndex = InstanceCfgPtr->SdClientService;
  const uint16_least ServiceIndexEnd = ServiceIndex + InstanceCfgPtr->SdClientServiceCount;
  P2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_APPL_DATA) ClientServiceStatePtr = &Sd_ClientServiceStates[ServiceIndex];

  while(ServiceIndex < ServiceIndexEnd)
  {
    if(ClientServiceStatePtr->RequestedState == SD_CLIENT_SERVICE_REQUESTED)
    {
      ClientServiceStatePtr->EventFlags |= SD_CLIENT_EVENT_START;
      ++InstanceStatePtr->ClientServicesEventsCount;
    }
    ++ClientServiceStatePtr;
    ++ServiceIndex;
  }
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ClientService_haltServices
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr
)
{
  uint16_least ServiceIndex = InstanceCfgPtr->SdClientService;
  const uint16_least ServiceIndexEnd = ServiceIndex + InstanceCfgPtr->SdClientServiceCount;
  P2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_APPL_DATA) ClientServiceStatePtr = &Sd_ClientServiceStates[ServiceIndex];

  while(ServiceIndex < ServiceIndexEnd)
  {
    if(ClientServiceStatePtr->ComPhase != SD_CLIENT_SERVICE_COMPHASE_DOWN)
    {
      ClientServiceStatePtr->EventFlags = SD_CLIENT_EVENT_HALT;
      ++InstanceStatePtr->ClientServicesEventsCount;
    }
    else
    {
      if( (ClientServiceStatePtr->EventFlags & SD_CLIENT_EVENT_START) != 0U )
      {
        /* this RELEASE command does not come straight after an REQUEST command that has not been processed yet */
        --InstanceStatePtr->ClientServicesEventsCount;
        ClientServiceStatePtr->EventFlags = 0U;
      }
    }
    ++ClientServiceStatePtr;
    ++ServiceIndex;
  }
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ClientService_resetRemoteConnection
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType, AUTOMATIC, SD_APPL_DATA) InstanceStatesPtr,
  const Sd_RemoteNodeIdType RemoteNodeIndex
)
{
  uint16_least ServiceIndex = InstanceCfgPtr->SdClientService;
  const uint16_least ServiceIndexEnd = ServiceIndex + InstanceCfgPtr->SdClientServiceCount;

  while(ServiceIndex < ServiceIndexEnd)
  {
    if (Sd_ClientServiceStates[ServiceIndex].NodeIndex == RemoteNodeIndex)
    {
      /* Handle the reboot as if a Stop Offer entry was received */
      Sd_ClientServiceStates[ServiceIndex].SdOfferEvents |= (SD_CLIENT_EVENT_STOPOFFER_0 | SD_CLIENT_EVENT_STOPOFFER_1);
      InstanceStatesPtr->ClientServicesEventsCount += SD_INCREASE_CLIENT_OFFER_EVENT;
    }
    ++ServiceIndex;
  }
}



/*==================[internal function definitions]=========================*/

STATIC FUNC(void, SD_CODE) Sd_ClientService_Set_State
(
  const uint16 ServiceIndex,
  const Sd_ClientServiceSetStateType State
)
{
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_APPL_DATA) ClientServiceStatePtr = &Sd_ClientServiceStates[ServiceIndex];
  /* check if state to be set has not been set yet */
  if(ClientServiceStatePtr->RequestedState != State)
  {
    CONSTP2CONST(SdClientService_t, AUTOMATIC, SD_APPL_CONST) ServiceCfgRefPtr = &SD_CFG(SdClientService_t, SdClientServicesRef, ServiceIndex);
    uint8 InstanceIndex = ServiceCfgRefPtr->SdInstanceIndex;

    /* Service is getting requested. call state machine only if IP address is available */
    if(Sd_IsInstanceAddressAssigned(InstanceIndex))
    { /* if this client service is being requested */
      if (State == SD_CLIENT_SERVICE_REQUESTED)
      { /* set event flag to indicate that his client event can be started in the next main cycle */
        if(ClientServiceStatePtr->ComPhase == SD_CLIENT_SERVICE_COMPHASE_DOWN)
        {
          ClientServiceStatePtr->EventFlags = SD_CLIENT_EVENT_START;
        }
        else
        {
          ClientServiceStatePtr->EventFlags |= SD_CLIENT_EVENT_START;
        }
        ++Sd_InstanceStates[InstanceIndex].ClientServicesEventsCount;
      }
      else /* this client service is being made as no longer available, it is going to be set to down state, disabled */
      {
        if(ClientServiceStatePtr->ComPhase != SD_CLIENT_SERVICE_COMPHASE_DOWN)
        {
          /* just in case check for the possibility in which this RELEASE command comes straight after the REQUESTED command,
          before this REQUEST command has been processed in the main cycle */
          if( (ClientServiceStatePtr->EventFlags & SD_CLIENT_EVENT_START) == 0U )
          { /* this RELEASE command does not come straight after an REQUEST command that has not been processed yet */
            ++Sd_InstanceStates[InstanceIndex].ClientServicesEventsCount;
          }
          else
          {
            --Sd_InstanceStates[InstanceIndex].ClientServicesEventsCount;
          }

          ClientServiceStatePtr->EventFlags = SD_CLIENT_EVENT_STOP;
        }
        else
        {
          ClientServiceStatePtr->EventFlags = 0U;
          --Sd_InstanceStates[InstanceIndex].ClientServicesEventsCount;
        }
      }
    }
    ClientServiceStatePtr->RequestedState = State;
  }
}



STATIC FUNC(Std_ReturnType, SD_CODE) Sd_ClientService_consumedEventGroupSetState
(
  const uint16 EventgroupIndex,
  const Sd_ConsumedEventGroupSetStateType State
)
{
  CONSTP2CONST(SdConsumedEventGroup_t,AUTOMATIC,SD_APPL_CONST) EventGroupCfgPtr = &SD_CFG(SdConsumedEventGroup_t, SdConsumedEventGroupsRef, EventgroupIndex);
  const uint16 ServiceHandleId = EventGroupCfgPtr->ServiceHandleId;
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr = &Sd_ClientServiceStates[ServiceHandleId];
  CONSTP2VAR(Sd_ConsumedEventGroupStateType,AUTOMATIC,SD_APPL_CONST) EventGroupStatePtr = &Sd_ConsumedEventGroupStates[EventgroupIndex];
  Std_ReturnType RetVal = E_NOT_OK;

  if(State == SD_CONSUMED_EVENTGROUP_REQUESTED)
  { /* if this client service is OFF and a consumed event group belonging to it is activated, this doesn't make sense and is not executed */
    if(ServiceStatePtr->RequestedState != SD_CLIENT_SERVICE_RELEASED)
    {
      if(EventGroupStatePtr->MachineState == SD_CONSUMED_EVENTGROUP_STATE_RELEASED)
      {
        EventGroupStatePtr->MachineState = SD_CONSUMED_EVENTGROUP_STATE_REQUESTED_NO_OFFER;

        if(ServiceStatePtr->ComPhase == SD_CLIENT_SERVICE_COMPHASE_AVAILABLE)
        { /* ONLY if this consumed event group has a TCP activation routing group configured AND if the received OFFER had a TCP option attached to it
           * then this TCP routing datapath will be enabled */
          if( (EventGroupCfgPtr->SdConsumedEventGroupTcpActivationRef != SOAD_INVALID_ROUTINGGROUP_ID) && (ServiceStatePtr->SoConIdTcp != SOAD_INVALID_SOCON_ID) )
          { /* !LINKSTO Sd.ASR40.SWS_SD_00702,1 */
            /* The return value can be ignored as the only possible return value is E_OK */
            (void)SoAd_EnableSpecificRouting(EventGroupCfgPtr->SdConsumedEventGroupTcpActivationRef, ServiceStatePtr->SoConIdTcp);
            EventGroupStatePtr->MachineState = SD_CONSUMED_EVENTGROUP_STATE_REQUESTED_OFFER_RECEIVED;
          }
          /* open UDP socket connections for multicast receptions only if this consumed event group has a multicast UDP reference configured */
          if(EventGroupCfgPtr->SdConsumedEventGroupMulticastSoConId != SOAD_INVALID_SOCON_ID)
          {
            uint16_least SoConId = EventGroupCfgPtr->SdConsumedEventGroupMulticastSoConId;
            const uint16_least SoConIdxEnd = SoConId + EventGroupCfgPtr->SdConsumEventGroupMulticastRefCount;

            /* open all socket connections of this multicast socket connection group */
            do
            { /* The return value can be ignored as the only possible return value is E_OK */
              (void)SoAd_OpenSoCon((SoAd_SoConIdType)SoConId);
              ++SoConId;
            }
            while(SoConId < SoConIdxEnd);
            EventGroupStatePtr->MachineState = SD_CONSUMED_EVENTGROUP_STATE_REQUESTED_OFFER_RECEIVED;
          }
          /* ONLY if this consumed event group has an UDP activation routing group configured AND if the received OFFER had a UDP option attached to it
           * then this UDP routing datapath will be enabled */
          if( (EventGroupCfgPtr->SdConsumedEventGroupUdpActivationRef != SOAD_INVALID_ROUTINGGROUP_ID) && (ServiceStatePtr->SoConIdUdp != SOAD_INVALID_SOCON_ID) )
          { /* !LINKSTO Sd.ASR40.SWS_SD_00703,1 */
            /* The return value can be ignored as the only possible return value is E_OK */
            (void)SoAd_EnableSpecificRouting(EventGroupCfgPtr->SdConsumedEventGroupUdpActivationRef, ServiceStatePtr->SoConIdUdp);
            EventGroupStatePtr->MachineState = SD_CONSUMED_EVENTGROUP_STATE_REQUESTED_OFFER_RECEIVED;
          }
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
          if(EventGroupStatePtr->MachineState == SD_CONSUMED_EVENTGROUP_STATE_REQUESTED_OFFER_RECEIVED)
          { /* only if this client service was is enabled for sending consumed event group retries,
              then the retry counter of this consumed group will be set to 1 to indicate that the counter of retries has been started */
            if( (ServiceStatePtr->SdSubscribeEventGroupTimersActive != FALSE) && (ServiceStatePtr->SubscribeFlag == SD_CLIENT_SUBSCRIBEFLAG_SENT) )
            { /* !LINKSTO Sd.ASR40.SWS_SD_00741,1 */
              CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgRefPtr = &SD_CFG(SdClientService_t, SdClientServicesRef, ServiceHandleId);
              CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr = &SD_CFG(SdInstance_t, SdInstancesRef, ServiceCfgRefPtr->SdInstanceIndex);
              CONSTP2VAR(Sd_RemoteNodeStateType,AUTOMATIC,SD_APPL_DATA) RemoteNodeStatePtr = Sd_GetRemoteNodeState(ServiceStatePtr->NodeIndex);
              CONSTP2VAR(Sd_InstanceStateType, AUTOMATIC, SD_APPL_DATA) InstanceStatePtr = &Sd_InstanceStates[ServiceCfgRefPtr->SdInstanceIndex];

              RemoteNodeStatePtr->RetryTimer = InstanceCfgPtr->SdSubscribeEventgroupRetryDelay;
              ++RemoteNodeStatePtr->ClientServicesRetriesToProcess;

              /* !LINKSTO Sd.ASR40.SWS_SD_00736,1 */
              if(InstanceStatePtr->RetryTimersDeltaTimer == 0U)
              { /* if retry timer delta of this instance has not been initialized yet, then it is done here */
                InstanceStatePtr->RetryTimersDeltaTimer = RemoteNodeStatePtr->RetryTimer;
                InstanceStatePtr->RetryTimersDelta = InstanceStatePtr->RetryTimersDeltaTimer;
              }
              else
              { /* if there is already a retry timer running on this instance then the retry timer of this remote node has to be synchronized to it */
                RemoteNodeStatePtr->RetryTimer += (InstanceCfgPtr->SdSubscribeEventgroupRetryDelay - InstanceStatePtr->RetryTimersDeltaTimer);
              }
              EventGroupStatePtr->SdSubscribeEventgroupRetryCounter = 1U;
              /* increase counter to indicate how many consumed event groups from this client service have to be acknowledged */
              ++ServiceStatePtr->SdSubscribeEventGroupsNotAck;
            }
          }
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
        }
      }
      RetVal = E_OK;
    }
  }
  else /* State == SD_CONSUMED_EVENTGROUP_RELEASED */
  {
    if( (EventGroupStatePtr->MachineState == SD_CONSUMED_EVENTGROUP_STATE_REQUESTED_OFFER_RECEIVED) ||
        (EventGroupStatePtr->MachineState == SD_CONSUMED_EVENTGROUP_STATE_WAIT_FOR_AVALIABILITY) ||
        (EventGroupStatePtr->MachineState == SD_CONSUMED_EVENTGROUP_STATE_AVAILABLE) )
    {
      CONSTP2CONST(SdClientService_t, AUTOMATIC, SD_APPL_CONST) ServiceCfgRefPtr = &SD_CFG_REF(SdClientService_t, SdClientServicesRef, 0U);

      if(EventGroupStatePtr->MachineState == SD_CONSUMED_EVENTGROUP_STATE_AVAILABLE)
      {
        Sd_ClientService_CEvg_exit_AVAILABLE(EventGroupCfgPtr, EventGroupStatePtr, (uint16)EventgroupIndex);
        EventGroupStatePtr->TTL = SD_TTL_OFF;
      }
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
      else
      {
        if(EventGroupStatePtr->SdSubscribeEventgroupRetryCounter > 0U)
        {
          /* !LINKSTO Sd.ASR40.SWS_SD_00738,1 */
          /* !LINKSTO Sd.ASR40.SWS_SD_00713,1 */
          /* this consumed event group has been released, so its retry counter is cleared */
          EventGroupStatePtr->SdSubscribeEventgroupRetryCounter = 0U;
#if (SD_DEV_ERROR_DETECT == STD_ON)
          /* this defensive check covers the case of an underflow in the variable ClientServicesTimersActive */
          SD_PRECONDITION_ASSERT( (ServiceStatePtr->SdSubscribeEventGroupsNotAck > 0U), SD_CONSUMEDEVENTGROUPSETSTATE_SVCID );
#endif
          --ServiceStatePtr->SdSubscribeEventGroupsNotAck;
        }
      }
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
      if( (EventGroupCfgPtr->SdConsumedEventGroupUdpActivationRef != SOAD_INVALID_ROUTINGGROUP_ID) && (ServiceStatePtr->SoConIdUdp != SOAD_INVALID_SOCON_ID) )
      {  /* The return value can be ignored as the only possible return value is E_OK */
        (void)SoAd_DisableSpecificRouting(EventGroupCfgPtr->SdConsumedEventGroupUdpActivationRef, ServiceStatePtr->SoConIdUdp);
      }
      if( (EventGroupCfgPtr->SdConsumedEventGroupTcpActivationRef != SOAD_INVALID_ROUTINGGROUP_ID) && (ServiceStatePtr->SoConIdTcp != SOAD_INVALID_SOCON_ID) )
      {  /* The return value can be ignored as the only possible return value is E_OK */
         (void)SoAd_DisableSpecificRouting(EventGroupCfgPtr->SdConsumedEventGroupTcpActivationRef, ServiceStatePtr->SoConIdTcp);
      }
      /* close UDP socket connections for multicast receptions */
      if( (EventGroupCfgPtr->SdConsumedEventGroupMulticastSoConId != SOAD_INVALID_SOCON_ID) &&
          (Sd_ClientConsumEventGroupMulticastRefSubsCounter[EventGroupCfgPtr->SdConsumEventGroupMulticastRefIndex] == 0U) )
      {
        uint16_least SoConId = EventGroupCfgPtr->SdConsumedEventGroupMulticastSoConId;
        const uint16_least SoConIdxEnd = SoConId + EventGroupCfgPtr->SdConsumEventGroupMulticastRefCount;

        /* close all socket connections of this multicast socket connection group */
        do
        { /* The return value can be ignored as the only possible return value is E_OK */
          (void)SoAd_CloseSoCon((SoAd_SoConIdType)SoConId, FALSE);
          ++SoConId;
        }
        while(SoConId < SoConIdxEnd);
      }
      /* set flag to send a stop subscribe to indicate that this consumed group is no longer requested
        only if this consumed event group had already sent out a Subscription first */
      if(EventGroupStatePtr->Acknowledged != SD_CEVG_NO_SUBSCRIPTION_SENT)
      { /* !LINKSTO Sd.ASR40.SWS_SD_00713,1 */
        EventGroupStatePtr->SendStopSubsc = TRUE;
        EventGroupStatePtr->Acknowledged = SD_CEVG_NO_SUBSCRIPTION_SENT;
        Sd_RestartNodeResponseTimer(SD_TIMER_IMMEDIATE, ServiceStatePtr->NodeIndex, ServiceCfgRefPtr[ServiceHandleId].SdInstanceIndex);
      }
    }
    EventGroupStatePtr->MachineState = SD_CONSUMED_EVENTGROUP_STATE_RELEASED;
    RetVal = E_OK;
  }
  return RetVal;
}



STATIC FUNC(boolean, SD_CODE) SdCheckIfMinorVersionBlackListed
(
  CONSTP2CONST(SdClientService_t, AUTOMATIC, SD_APPL_DATA) ClientServiceCfgPtr,
  const uint32 MinorVersion
)
{
  boolean bRetVal = FALSE;

  if(ClientServiceCfgPtr->SdBlacklistedVersionsCount > 0U)
  { /* there are black listed minor versions configured in this client service */
    uint16 BlackListedVersionIndex = ClientServiceCfgPtr->SdBlacklistedVersion;
    const uint16 BlackListedVersionEnd = BlackListedVersionIndex + ClientServiceCfgPtr->SdBlacklistedVersionsCount;
    P2CONST(uint32, AUTOMATIC, SD_APPL_CONST) BlackListedVersionsCfgPtr = &SD_CFG(uint32, SdBlackListedVersionsRef, BlackListedVersionIndex);

    do
    { /* go through the list of black listed minor versions in this client service */
      if(*BlackListedVersionsCfgPtr == MinorVersion)
      { /* return value set to TRUE to indicate that the minor version received is in the black list of minor versions */
        bRetVal = TRUE;
        /* no need to keep searching */
        break;
      }
      ++BlackListedVersionIndex;
      ++BlackListedVersionsCfgPtr;
    }
    while(BlackListedVersionIndex < BlackListedVersionEnd);
  }
  return bRetVal;
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
STATIC FUNC(uint16, SD_CODE) Sd_ClientService_QuickMatch
(
  CONSTP2CONST(Sd_ServiceInfoType, AUTOMATIC, SD_APPL_DATA) ServiceInfoPtr,
  CONSTP2CONST(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfoPtr,
  const uint32 MinorVersion,
  const uint8 SdInstanceIndex,
  const boolean CheckMinorVersion
)
{
  P2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr = &SD_CFG(SdInstance_t, SdInstancesRef, SdInstanceIndex);
  /* variable to be returned with the matched client service index is initialized here to an invalid value */
  uint16 ServiceHandleId = SD_WILDCARD_SERVICEID;

  /* only if there are client services defined in this instance */
  if(InstanceCfgPtr->SdClientServiceCount > 0U)
  { /* bottom left index is the first element index */
    const uint16 ServiceIndexStart = InstanceCfgPtr->SdClientService;
    /* top right index is the last element index, that is the bottom left index plus the number of elements minus 1,
     * since it is an index value. */
    const uint16 ServiceIndexEnd = ServiceIndexStart + (InstanceCfgPtr->SdClientServiceCount - 1U);
    /* target value service ID/instance ID is calculated here */
    const uint32 targetCombinedId = ((uint32)ServiceInfoPtr->ServiceID * 0x10000U) + (uint32)ServiceInfoPtr->InstanceID;
    CONSTP2CONST(SdClientService_t, AUTOMATIC, SD_APPL_DATA) ServiceCfgRefPtr = &SD_CFG_REF(SdClientService_t, SdClientServicesRef, 0U);
    CONSTP2CONST(SdServiceIndex_t, AUTOMATIC, SD_APPL_DATA) ServiceIndexCfgRefPtr = &SD_CFG_REF(SdServiceIndex_t, SdClientServiceIndexRef, 0U);
    SdConfigOptionPtrType ServiceConfigStringRefPtr = &SD_CFG_REF(uint8, SdConfigStringRef, 0U);
    P2CONST(SdClientService_t, AUTOMATIC, SD_APPL_DATA) ServiceCfgPtr;
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
      { /* calculate the value contained in the index that is the middle value of the remaining part of the array
         * (or the whole array if it is the first occurrence of this loop) */
        middleCombinedId = ServiceIndexCfgRefPtr[middleIdx].SdServiceCombinedId;

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
    { /* keep looking for a total match as long as current element matches partially (service ID/instance ID)
         and the element has not been totally matched */
      if (ServiceIndexCfgRefPtr[ServiceIndexCurrent].SdServiceCombinedId != targetCombinedId)
      {
        break;
      }
      /* this elements matches partially (service ID/instance ID), check it is matches totally */
      ServiceHandleId = ServiceIndexCfgRefPtr[ServiceIndexCurrent].SdServiceHandleId;
      ServiceCfgPtr = &ServiceCfgRefPtr[ServiceHandleId];

      /* check if MajorVersion is also identical */
      if(ServiceCfgPtr->SdClientServiceMajorVersion == ServiceInfoPtr->MajorVersion)
      { /* check if MinorVersion is also identical and configuration array are identical to conform a total match */
        /* all services with the same Service ID, Instance ID and Major Version are compatibles */
        if( /* first of all minor version is only verified if flag CheckMinorVersion does not have value FALSE */
            /* Deviation MISRAC2012-2 */
            (CheckMinorVersion == FALSE) ||
            /* !LINKSTO Sd.ASR40.SWS_SD_01503,1 */
            ( /* check if minor version received is in the black list of minor versions of this client service */
              (SdCheckIfMinorVersionBlackListed(ServiceCfgPtr, MinorVersion) == FALSE) &&
              ( /* !LINKSTO Sd.ASR40.SWS_SD_00488,1 */
                /* !LINKSTO Sd.ASR40.SWS_SD_00489,1 */
                (  (ServiceCfgPtr->SdVersionDrivenFindBehavior == SD_EXACT_OR_ANY_MINOR_VERSION) &&
                   ( (ServiceCfgPtr->SdClientServiceMinorVersion == SD_WILDCARD_MINORVERSION) || (ServiceCfgPtr->SdClientServiceMinorVersion == MinorVersion) )  ) ||
                /* !LINKSTO Sd.ASR40.SWS_SD_04089,1 */
                ( (ServiceCfgPtr->SdVersionDrivenFindBehavior == SD_MINIMUM_MINOR_VERSION) && (MinorVersion >= ServiceCfgPtr->SdClientServiceMinorVersion) )
              )
            )
          )
        {
          TotalMatch = Sd_CapabilityRecordStrCmp(EntryOptionInfoPtr->ConfigOptions,
                                                 &ServiceConfigStringRefPtr[ServiceCfgPtr->SdConfigurationStringIndex],
                                                 ServiceCfgPtr->SdClientServiceConfigurationStringLength);
        }
      }
    }
    /* the search for a total match on the bottom left half of the remaining array is only possible if
     * the middle ID is bigger than 0, otherwise there just no bottom half of the array.
     * THIS CHECK IS IMPORTANT SINCE THERE IS A RISK OF UNDERFLOW HERE
     */
    if(middleIdx != 0U)
    { /* search for an exact match on the bottom left half of the remaining array starting from the middle ID minus one
        (note here that this -1 is connected to the fact that the middle Id value is calculated using a + 1 in the end */
      for(ServiceIndexCurrent = (uint16_least)middleIdx - 1U;
          ( (TotalMatch == FALSE) && (ServiceIndexCurrent >= ServiceIndexStart) );
          --ServiceIndexCurrent)
      { /* keep looking for a total match as long as current element matches partially (service ID/instance ID)
          and the element has not been totally matched */
        if (ServiceIndexCfgRefPtr[ServiceIndexCurrent].SdServiceCombinedId == targetCombinedId)
        {
          /* this elements matches partially (service ID/instance ID), check it is matches totally */
          ServiceHandleId = ServiceIndexCfgRefPtr[ServiceIndexCurrent].SdServiceHandleId;
          ServiceCfgPtr = &ServiceCfgRefPtr[ServiceHandleId];

          /* check if the MajorVersion is also identical */
          if(ServiceCfgPtr->SdClientServiceMajorVersion == ServiceInfoPtr->MajorVersion)
          {
            /* check if MinorVersion is also identical and configuration array are identical to conform a total match */
            /* all services with the same Service ID, Instance ID and Major Version are compatibles */
            if( /* first of all minor version is only verified if flag CheckMinorVersion does not have value FALSE */
                /* Deviation MISRAC2012-2 */
                (CheckMinorVersion == FALSE) ||
                /* !LINKSTO Sd.ASR40.SWS_SD_01503,1 */
                ( /* check if minor version received is in the black list of minor versions of this client service */
                  (SdCheckIfMinorVersionBlackListed(ServiceCfgPtr, MinorVersion) == FALSE) &&
                  ( /* !LINKSTO Sd.ASR40.SWS_SD_00488,1 */
                  /* !LINKSTO Sd.ASR40.SWS_SD_00489,1 */
                  (  (ServiceCfgPtr->SdVersionDrivenFindBehavior == SD_EXACT_OR_ANY_MINOR_VERSION) &&
                     ( (ServiceCfgPtr->SdClientServiceMinorVersion == SD_WILDCARD_MINORVERSION) || (ServiceCfgPtr->SdClientServiceMinorVersion == MinorVersion) )  ) ||
                  /* !LINKSTO Sd.ASR40.SWS_SD_04089,1 */
                  ( (ServiceCfgPtr->SdVersionDrivenFindBehavior == SD_MINIMUM_MINOR_VERSION) && (MinorVersion >= ServiceCfgPtr->SdClientServiceMinorVersion) )
                  )
                )
              )
            {
              TotalMatch = Sd_CapabilityRecordStrCmp(EntryOptionInfoPtr->ConfigOptions,
                                                     &ServiceConfigStringRefPtr[ServiceCfgPtr->SdConfigurationStringIndex],
                                                     ServiceCfgPtr->SdClientServiceConfigurationStringLength);
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

    if(TotalMatch == FALSE)
    {
      ServiceHandleId = SD_WILDCARD_SERVICEID;
    }
  }

  return ServiceHandleId;
}



/*=================== Consumed Eventgroup Statemachine ==================================*/
STATIC FUNC(void, SD_CODE) Sd_ClientService_CEvg_init(uint16 EventgroupIndex)
{
  CONSTP2VAR(Sd_ConsumedEventGroupStateType,AUTOMATIC,SD_APPL_CONST) EventGroupStatePtr = &Sd_ConsumedEventGroupStates[EventgroupIndex];
  CONSTP2CONST(SdConsumedEventGroup_t, AUTOMATIC, SD_APPL_CONST) ServiceConsumedEventGroupsRefCfgPtr = &SD_CFG_REF(SdConsumedEventGroup_t, SdConsumedEventGroupsRef, 0U);

  Sd_StopTimer(&EventGroupStatePtr->TTL);

  if(ServiceConsumedEventGroupsRefCfgPtr[EventgroupIndex].SdConsumedEventGroupAutoRequire)
  {
    EventGroupStatePtr->MachineState = SD_CONSUMED_EVENTGROUP_STATE_REQUESTED_NO_OFFER;;
  }
  else
  {
    EventGroupStatePtr->MachineState = SD_CONSUMED_EVENTGROUP_STATE_RELEASED;
  }
  EventGroupStatePtr->SendStopSubsc = FALSE;
  EventGroupStatePtr->Acknowledged = SD_CEVG_NO_SUBSCRIPTION_SENT;
  EventGroupStatePtr->MulticastSoConId = SOAD_INVALID_SOCON_ID;
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
  EventGroupStatePtr->SdSubscribeEventgroupRetryCounter = 0U;
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
}



STATIC FUNC(boolean, SD_CODE) Sd_ClientService_CEvg_event_SubscribeEventgroupAck
(
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, SD_APPL_DATA) MulticastEndPointSockAddrPtr,
  const uint32 TTL,
  uint16 EventgroupIndex,
  const uint8 InstanceIndex
)
{
  P2CONST(SdConsumedEventGroup_t, AUTOMATIC, SD_APPL_CONST) EventgroupCfgPtr = &SD_CFG_REF(SdConsumedEventGroup_t, SdConsumedEventGroupsRef, 0U);
  CONSTP2VAR(Sd_ConsumedEventGroupStateType,AUTOMATIC,SD_APPL_CONST) EventGroupStatePtr = &Sd_ConsumedEventGroupStates[EventgroupIndex];
  const boolean MulticastEndpointReceived = ( (MulticastEndPointSockAddrPtr->domain != TCPIP_AF_UNSPEC) ? TRUE : FALSE );
  boolean retVal = FALSE;

  EventgroupCfgPtr = &EventgroupCfgPtr[EventgroupIndex];

  if( (EventgroupCfgPtr->SdConsumedEventGroupMulticastActivationRef == SOAD_INVALID_ROUTINGGROUP_ID) &&
      (MulticastEndpointReceived != FALSE) )
  {
    /* this consumed event group doesn't have any multicast activation data path routing group configured, but the server
     * event handler has sent a multicast endpoint, but as there can't be any multicast communication between the remote server
     * event handler and this consumed event group, this subscription acknowledge message is ignored.
     */
  }
  else if(  ( (EventgroupCfgPtr->SdConsumedEventGroupUdpActivationRef == SOAD_INVALID_ROUTINGGROUP_ID)
             && (EventgroupCfgPtr->SdConsumedEventGroupTcpActivationRef == SOAD_INVALID_ROUTINGGROUP_ID) )
          && (MulticastEndpointReceived == FALSE)  )
  {
    /* This consumed event group doesn't have a any unicast activation data path routing group configured,
     * that means that it is but the server a multicast only consumed event group but the event handler has not sent any multicast endpoint.
     * In this case, as there can't be any multicast communication between this remote server service event handler
     * and this consumed event group, this subscription acknowledge message is ignored.
     */
  }
  else
  {
    if(EventGroupStatePtr->MachineState == SD_CONSUMED_EVENTGROUP_STATE_REQUESTED_OFFER_RECEIVED)
    {
      if(Sd_ClientService_CEvg_entry_AVAILABLE(MulticastEndPointSockAddrPtr, EventgroupCfgPtr, EventgroupIndex)  == FALSE)
      {
        EventGroupStatePtr->MachineState = SD_CONSUMED_EVENTGROUP_STATE_WAIT_FOR_AVALIABILITY;
        retVal = TRUE;
      }
      else
      {
        EventGroupStatePtr->MachineState = SD_CONSUMED_EVENTGROUP_STATE_AVAILABLE;
      }
    }

    if( (EventGroupStatePtr->MachineState == SD_CONSUMED_EVENTGROUP_STATE_WAIT_FOR_AVALIABILITY) ||
        (EventGroupStatePtr->MachineState == SD_CONSUMED_EVENTGROUP_STATE_AVAILABLE) )
    {
      if(TTL != SD_ENTRYTTL_INFINITE)
      {
        CONSTP2VAR(Sd_InstanceStateType, AUTOMATIC, SD_APPL_DATA) InstanceStatesPtr = &Sd_InstanceStates[InstanceIndex];

        EventGroupStatePtr->TTL = (TTL * SD_LOOPS_PER_SECOND) + 1U;

        if( (InstanceStatesPtr->ClientsDeltaTTLtimer <= SD_TTL_ALWAYS_ON) || (EventGroupStatePtr->TTL < InstanceStatesPtr->ClientsDeltaTTLtimer) )
        { /* set next timeout */
          InstanceStatesPtr->ClientsDeltaTTLtimer = EventGroupStatePtr->TTL;
          InstanceStatesPtr->ClientsDeltaTTL = EventGroupStatePtr->TTL;
        }
        else
        {
          EventGroupStatePtr->TTL += (InstanceStatesPtr->ClientsDeltaTTL - InstanceStatesPtr->ClientsDeltaTTLtimer);
        }
      }
      else
      {
        EventGroupStatePtr->TTL = SD_TTL_ALWAYS_ON;
      }
    }
    EventGroupStatePtr->Acknowledged = SD_CEVG_ACKNOWLEDGED;
  }
  return retVal;
}



STATIC FUNC(boolean, SD_CODE) Sd_ClientService_CEvg_entry_AVAILABLE
(
  P2VAR(TcpIp_SockAddrType, AUTOMATIC, SD_APPL_DATA) MulticastSockAddrPtr,
  CONSTP2CONST(SdConsumedEventGroup_t, AUTOMATIC, SD_APPL_CONST) EventGroupCfgPtr,
  uint16 EventgroupIndex
)
{
  boolean retVal;

  if( (EventGroupCfgPtr->SdConsumedEventGroupMulticastActivationRef != SOAD_INVALID_ROUTINGGROUP_ID) && (MulticastSockAddrPtr->domain != TCPIP_AF_UNSPEC) )
  {
    /* The return value can be ignored as the only possible return value is E_OK */
    (void)SoAd_RequestIpAddrAssignment(EventGroupCfgPtr->SdConsumedEventGroupMulticastSoConId,
                                       TCPIP_IPADDR_ASSIGNMENT_STATIC,
                                       MulticastSockAddrPtr);
    retVal = FALSE;
  }
  else
  {
    BswM_Sd_ConsumedEventGroupCurrentState(EventgroupIndex, SD_CONSUMED_EVENTGROUP_AVAILABLE);
    retVal = TRUE;
  }
  return retVal;
}



STATIC FUNC(void, SD_CODE) Sd_ClientService_CEvg_GoTo_AVAILABLE
(
  CONSTP2CONST(SdConsumedEventGroup_t,AUTOMATIC,SD_APPL_CONST) EventGroupCfgPtr,
  CONSTP2VAR(Sd_ConsumedEventGroupStateType,AUTOMATIC,SD_APPL_CONST) EventGroupStePtr,
  uint16 EventgroupIdx
)
{
  /* mark this socket connection of as the one being used */
  /* !LINKSTO Sd.ASR40.SWS_SD_00704,1 */
  EventGroupStePtr->MulticastSoConId = EventGroupCfgPtr->SdConsumedEventGroupMulticastSoConId;

  /* enable this specific routing using the socket connection that was made available in the function SoAd_SetUniqueRemoteAddr */
  (void)SoAd_EnableSpecificRouting(EventGroupCfgPtr->SdConsumedEventGroupMulticastActivationRef, EventGroupCfgPtr->SdConsumedEventGroupMulticastSoConId);

  BswM_Sd_ConsumedEventGroupCurrentState(EventgroupIdx, SD_CONSUMED_EVENTGROUP_AVAILABLE);

  ++Sd_ClientConsumEventGroupMulticastRefSubsCounter[EventGroupCfgPtr->SdConsumEventGroupMulticastRefIndex];
}



STATIC FUNC(void, SD_CODE) Sd_ClientService_CEvg_exit_AVAILABLE
(
  CONSTP2CONST(SdConsumedEventGroup_t,AUTOMATIC,SD_APPL_CONST) EventGroupCfgPtr,
  CONSTP2VAR(Sd_ConsumedEventGroupStateType,AUTOMATIC,SD_APPL_CONST) EventGroupStatPtr,
  uint16 EventgroupIndex
)
{
  /* !LINKSTO Sd.ASR40.SWS_SD_00713,1 */
  BswM_Sd_ConsumedEventGroupCurrentState(EventgroupIndex, SD_CONSUMED_EVENTGROUP_DOWN);

  /* only if this event group has a multicast datapath open */
  if(EventGroupStatPtr->MulticastSoConId != SOAD_INVALID_SOCON_ID)
  {
    /* return value can be ignored as the only possible return value is E_OK */
    (void)SoAd_DisableSpecificRouting(EventGroupCfgPtr->SdConsumedEventGroupMulticastActivationRef, EventGroupStatPtr->MulticastSoConId);

    /* reset this value to indicate that this event group does not habe a datapath open anymore */
    EventGroupStatPtr->MulticastSoConId = SOAD_INVALID_SOCON_ID;
#if (SD_DEV_ERROR_DETECT == STD_ON)
    /* this defensive check covers the case of an underflow in the variable ServerServicesTimersActive */
    SD_PRECONDITION_ASSERT( (Sd_ClientConsumEventGroupMulticastRefSubsCounter[EventGroupCfgPtr->SdConsumEventGroupMulticastRefIndex] > 0U), SD_CLIENTSERVICECEVGEXITAVAILABLE_SVCID );
#endif
    /* as more than one consumed event group can be referenced to the same multicast socket connection group, just decrement
     * the counter with the number of subscribed consumed event groups using this multicast socket connection group */
    --Sd_ClientConsumEventGroupMulticastRefSubsCounter[EventGroupCfgPtr->SdConsumEventGroupMulticastRefIndex];
    /* if no consumed event group is using this multicast consumed event group the local address can be released */
    if(Sd_ClientConsumEventGroupMulticastRefSubsCounter[EventGroupCfgPtr->SdConsumEventGroupMulticastRefIndex] == 0U)
    { /* !LINKSTO Sd.ASR40.SWS_SD_00706,1 */
      (void)SoAd_ReleaseIpAddrAssignment(EventGroupCfgPtr->SdConsumedEventGroupMulticastSoConId);
    }
  }
}



/*============[Client Service Communication phase state machine]===================*/

STATIC FUNC(void, SD_CODE) Sd_ClientService_ComSM_init(const uint16 ServiceIndex)
{
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr = &Sd_ClientServiceStates[ServiceIndex];
  CONSTP2CONST(SdClientService_t, AUTOMATIC, SD_APPL_DATA) ServiceCfgRefPtr = &SD_CFG_REF(SdClientService_t, SdClientServicesRef, 0U);

  Sd_StopTimer(&ServiceStatePtr->StateMachineTimer);
  ServiceStatePtr->TTLreceived = SD_TTL_OFF;
  ServiceStatePtr->SoConIdUdp = SOAD_INVALID_SOCON_ID;
  ServiceStatePtr->SoConIdTcp = SOAD_INVALID_SOCON_ID;
  ServiceStatePtr->NodeIndex = SD_INVALID_REMOTENODE_ID;
  if(ServiceCfgRefPtr[ServiceIndex].SdClientServiceAutoRequire)
  {
    ServiceStatePtr->RequestedState = SD_CLIENT_SERVICE_REQUESTED;
  }
  else
  {
    ServiceStatePtr->RequestedState = SD_CLIENT_SERVICE_RELEASED;
  }
  ServiceStatePtr->ComPhase = SD_CLIENT_SERVICE_COMPHASE_DOWN;
  ServiceStatePtr->Repetition = 0U;
  ServiceStatePtr->EventFlags = 0U;
  ServiceStatePtr->SubscribeFlag = SD_CLIENT_SUBSCRIBEFLAG_NO_ACT;
  ServiceStatePtr->SdOfferEvents = 0U;
  ServiceStatePtr->OfferReceived = FALSE;
  ServiceStatePtr->SubsAckEvent = FALSE;
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
  ServiceStatePtr->SdSubscribeEventGroupsNotAck = 0U;
  ServiceStatePtr->SdSubscribeEventGroupTimersActive = FALSE;
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
}


STATIC FUNC(void, SD_CODE) Sd_ClientService_sendSubscribeEventGroupsOfService
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  const Sd_RemoteNodeIdType RemoteNodeIndex
)
{
  uint16_least EventgroupIndex = ServiceCfgPtr->SdConsumedEventGroup;
  const uint16_least EventgroupIndexEnd = EventgroupIndex + ServiceCfgPtr->SdConsumedEventGroupCount;
  P2VAR(Sd_ConsumedEventGroupStateType,AUTOMATIC,SD_APPL_CONST) EventGroupStatePtr = &Sd_ConsumedEventGroupStates[EventgroupIndex];

  /* !LINKSTO Sd.ASR40.SWS_SD_00703,1 */
  const SoAd_SoConIdType UnicastUdpSoConId = ServiceStatePtr->SoConIdUdp;
  /* !LINKSTO Sd.ASR40.SWS_SD_00702,1 */
  const SoAd_SoConIdType UnicastTcpSoConId = ServiceStatePtr->SoConIdTcp;

  while(EventgroupIndex < EventgroupIndexEnd)
  {
    if(EventGroupStatePtr->SendStopSubsc == TRUE)
    {
      EventGroupStatePtr->SendStopSubsc = FALSE;
      Sd_Send_queueSubscribeEventgroup((uint16)EventgroupIndex, UnicastUdpSoConId, UnicastTcpSoConId, TRUE);
    }
    ++EventgroupIndex;
    ++EventGroupStatePtr;
  }

  if( (ServiceStatePtr->SubscribeFlag == SD_CLIENT_SUBSCRIBEFLAG_MC_RESP) ||
      (ServiceStatePtr->SubscribeFlag == SD_CLIENT_SUBSCRIBEFLAG_NORMAL) )
  {
    if(ServiceStatePtr->ComPhase == SD_CLIENT_SERVICE_COMPHASE_AVAILABLE)
    {
      EventgroupIndex = ServiceCfgPtr->SdConsumedEventGroup;
      EventGroupStatePtr = &Sd_ConsumedEventGroupStates[EventgroupIndex];
      while(EventgroupIndex < EventgroupIndexEnd)
      {
        if( (EventGroupStatePtr->MachineState == SD_CONSUMED_EVENTGROUP_STATE_REQUESTED_OFFER_RECEIVED) ||
            (EventGroupStatePtr->MachineState == SD_CONSUMED_EVENTGROUP_STATE_WAIT_FOR_AVALIABILITY) ||
            (EventGroupStatePtr->MachineState == SD_CONSUMED_EVENTGROUP_STATE_AVAILABLE) )
        {
          if(ServiceStatePtr->SubscribeFlag == SD_CLIENT_SUBSCRIBEFLAG_MC_RESP)
          {
            if(EventGroupStatePtr->Acknowledged == SD_CEVG_NOT_ACKNOWLEDGED)
            { /* !LINKSTO Sd.ASR40.SWS_SD_00695,1 */
              Sd_Send_queueSubscribeEventgroup((uint16)EventgroupIndex, UnicastUdpSoConId, UnicastTcpSoConId, TRUE);
            }
            EventGroupStatePtr->Acknowledged = SD_CEVG_NOT_ACKNOWLEDGED;
          }
          else
          {
            EventGroupStatePtr->Acknowledged = SD_CEVG_ACKNOWLEDGED;
          }
          /* !LINKSTO Sd.ASR40.SWS_SD_00695,1 */
          Sd_Send_queueSubscribeEventgroup((uint16)EventgroupIndex, UnicastUdpSoConId, UnicastTcpSoConId, FALSE);
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
          /* only if this client service was is enabled for sending consumed event group retries,
            then the retry counter of this consumed group will be set to 1 to indicate that the counter of retries has been started */
          if(ServiceStatePtr->SdSubscribeEventGroupTimersActive != FALSE)
          {
            EventGroupStatePtr->SdSubscribeEventgroupRetryCounter = 1U;
            /* increase counter to indicate how many consumed event groups from this client service have to be acknowledged */
            ++ServiceStatePtr->SdSubscribeEventGroupsNotAck;
          }
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
        }
        ++EventgroupIndex;
        ++EventGroupStatePtr;
      }
      ServiceStatePtr->SubscribeFlag = SD_CLIENT_SUBSCRIBEFLAG_SENT;
    }
    else if((ServiceStatePtr->RequestedState == SD_CLIENT_SERVICE_REQUESTED) && (ServiceStatePtr->OfferReceived == TRUE))
    {
      /* Retry in next MainFunction */
      /* !LINKSTO Sd.EB.ClientService.SubscribeEventgroup.DelayedTransmission,1 */
      ServiceStatePtr->SubscribeFlag = SD_CLIENT_SUBSCRIBEFLAG_NORMAL;
      Sd_RestartNodeResponseTimer(SD_TIMER_IMMEDIATE, RemoteNodeIndex, ServiceCfgPtr->SdInstanceIndex);
    }
    else
    {
      /* Service is not requested or TTL has expired */
      ServiceStatePtr->SubscribeFlag = SD_CLIENT_SUBSCRIBEFLAG_NO_ACT;
    }
  }
}



STATIC FUNC(void, SD_CODE) Sd_ClientService_ComSM_event_stop
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  uint16 ServiceIndex,
  const boolean isStop,
  const boolean FollowedByStart
)
{
  if(ServiceStatePtr->ComPhase == SD_CLIENT_SERVICE_COMPHASE_AVAILABLE)
  { /* exit code */
    /* !LINKSTO Sd.ASR40.SWS_SD_00712,1 */
    Sd_ClientService_ComSM_exit_AVAILABLE(ServiceCfgPtr, ServiceStatePtr, ServiceIndex, isStop, FALSE);
  }
  if( (isStop == FALSE) && (ServiceStatePtr->NodeIndex != SD_INVALID_REMOTENODE_ID) )
  {
    P2VAR(Sd_RemoteNodeStateType,AUTOMATIC,SD_APPL_DATA) RemoteNodeStatePtr = Sd_GetRemoteNodeState(ServiceStatePtr->NodeIndex);
#if (SD_DEV_ERROR_DETECT == STD_ON)
    /* this defensive check covers the case of an underflow in the variable ClientServicesTimersActive */
    SD_PRECONDITION_ASSERT( (RemoteNodeStatePtr->ClientServicesOffersReceived > 0U), SD_CLIENTSERVICE_COMSM_EVENT_STOP_SVCID );
#endif
    --RemoteNodeStatePtr->ClientServicesOffersReceived;
    ServiceStatePtr->NodeIndex = SD_INVALID_REMOTENODE_ID;
  }

  if(FollowedByStart == FALSE)
  {
    if(ServiceCfgPtr->SdClientServiceSoConGroupUdp != SD_INVALID_SOCONGROUP_ID)
    {
      /* close UDP socket connections for unicast communication */
      P2CONST(SdSocketConnectionGroupType,AUTOMATIC,SD_APPL_CONST) SoConGroupCfgPtr = &SD_CFG(SdSocketConnectionGroupType, SdSoConGroupsRef, ServiceCfgPtr->SdClientServiceSoConGroupUdp);
      {
        SoAd_SoConIdType SoConId = SoConGroupCfgPtr->GroupStart;
        const SoAd_SoConIdType SoConIdEndOfGroup = SoConId + SoConGroupCfgPtr->SoConCount;

        do
        {
          /* The return value can be ignored as the only possible return value is E_OK */
          (void)SoAd_CloseSoCon(SoConId, FALSE);
          ++SoConId;
        }
        while(SoConId < SoConIdEndOfGroup);
      }
    }

    if(ServiceCfgPtr->SdClientServiceSoConGroupTcp != SD_INVALID_SOCONGROUP_ID)
    {
      /* close TCP socket connection for unicast communication */
      P2CONST(SdSocketConnectionGroupType,AUTOMATIC,SD_APPL_CONST) SoConGroupCfgPtr = &SD_CFG(SdSocketConnectionGroupType, SdSoConGroupsRef, ServiceCfgPtr->SdClientServiceSoConGroupTcp);
      {
        SoAd_SoConIdType SoConId = SoConGroupCfgPtr->GroupStart;
        const SoAd_SoConIdType SoConIdEndOfGroup = SoConId + SoConGroupCfgPtr->SoConCount;

        do
        {
          /* The return value can be ignored as the only possible return value is E_OK */
          (void)SoAd_CloseSoCon(SoConId, FALSE);
          ++SoConId;
        }
        while(SoConId < SoConIdEndOfGroup);
      }
    }
  }
  ServiceStatePtr->ComPhase = SD_CLIENT_SERVICE_COMPHASE_DOWN;
}



STATIC FUNC(void, SD_CODE) Sd_ClientService_ComSM_event_timeout
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  uint16 ServiceIndex
)
{
  P2CONST(SdClientTimer_t, AUTOMATIC, SD_APPL_DATA) TimerCfgPtr =
      &SD_CFG(SdClientTimer_t, SdClientTimersRef, ServiceCfgPtr->SdClientServiceTimerIndex);

  switch(ServiceStatePtr->ComPhase)
  {
    case SD_CLIENT_SERVICE_COMPHASE_INITIALWAIT:

      Sd_Send_queueFindService(ServiceCfgPtr, ServiceIndex);

      /* the repetition phase shall be skipped if RepetitionMax is set to zero
       * and the main phase shall be entered immediately */
      if(TimerCfgPtr->SdClientTimerInitialFindRepetitionsMax > 0U)
      {
        ServiceStatePtr->Repetition = 0U;

        Sd_StartTimer(&(ServiceStatePtr->StateMachineTimer), TimerCfgPtr->SdClientTimerInitialFindRepetitionsBaseDelay);

        ServiceStatePtr->ComPhase = SD_CLIENT_SERVICE_COMPHASE_REPETITION;
      }
      else /* no repetitions configured in this client service timer, so MAIN phase is entered directly from INITIAL WAIT phase */
      {
        ServiceStatePtr->ComPhase = SD_CLIENT_SERVICE_COMPHASE_MAIN;
      }
      break;

    case SD_CLIENT_SERVICE_COMPHASE_REPETITION:
      /* REPETITION phase timeout, queue FIND message  */
      Sd_Send_queueFindService(ServiceCfgPtr, ServiceIndex);

      /* and increase the counter of REPETITION timeouts*/
      ++ServiceStatePtr->Repetition;
      if(ServiceStatePtr->Repetition >= TimerCfgPtr->SdClientTimerInitialFindRepetitionsMax)
      { /* once maximum number of REPETITION phases have been reached MAIN phase is entered */
        ServiceStatePtr->ComPhase = SD_CLIENT_SERVICE_COMPHASE_MAIN;
      }
      else
      {  /* Range of Repetition: 1 .. 10 */
         Sd_StartTimer( &ServiceStatePtr->StateMachineTimer,
                        TimerCfgPtr->SdClientTimerInitialFindRepetitionsBaseDelay * (Sd_TimerType)((Sd_TimerType)1U << ServiceStatePtr->Repetition) );
      }
      break;

    /* CHECK: NOPARSE */
    case SD_CLIENT_SERVICE_COMPHASE_DOWN:
    case SD_CLIENT_SERVICE_COMPHASE_MAIN:
    case SD_CLIENT_SERVICE_COMPHASE_AVAILABLE:
      /* these cases are intended to be used only in development error detection mode */
#if (SD_DEV_ERROR_DETECT == STD_ON)
      SD_DET_REPORT_ERROR(SD_CLIENTSERVICE_COMSM_EVENT_TIMEOUT,SD_E_CLIENT_IMPOSSIBLE_STATE);
#endif /* SD_DEV_ERROR_DETECT == STD_ON */
      break;

    default: /* this default case is intended to be used only in development error detection mode */
#if (SD_DEV_ERROR_DETECT == STD_ON)
      SD_UNREACHABLE_CODE_ASSERT(SD_CLIENTSERVICE_COMSMEVENT_TIMEOUT_SVCID);
#endif /* SD_DEV_ERROR_DETECT == STD_ON */
      break;
      /* CHECK: PARSE */
  }
}



STATIC FUNC(void, SD_CODE) Sd_ClientService_TTL_Run_Out
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  uint16 ServiceIndex
)
{
  switch(ServiceStatePtr->ComPhase)
  {
    case SD_CLIENT_SERVICE_COMPHASE_DOWN:
      /* TTL of service expired */
      break;

    case SD_CLIENT_SERVICE_COMPHASE_MAIN:
    {
      /* Timer will be checked and decremented at the end of the current Mainfunction invocation */
      P2CONST(SdClientTimer_t, AUTOMATIC, SD_APPL_DATA) TimerCfgPtr = &SD_CFG(SdClientTimer_t, SdClientTimersRef, ServiceCfgPtr->SdClientServiceTimerIndex);

      Sd_StartTimer(&ServiceStatePtr->StateMachineTimer,
                    Sd_GetRandom(TimerCfgPtr->SdClientTimerInitialFindDelayMin, TimerCfgPtr->SdClientTimerInitialFindDelayMax) + 1U);

      /* timeout, previously received OFFER is no longer valid, so state machine goes back to INITIAL WAIT */
      ServiceStatePtr->ComPhase = SD_CLIENT_SERVICE_COMPHASE_INITIALWAIT;
    }
    break;

    case SD_CLIENT_SERVICE_COMPHASE_AVAILABLE:
    {
      P2CONST(SdClientTimer_t, AUTOMATIC, SD_APPL_DATA) TimerCfgPtr = &SD_CFG(SdClientTimer_t, SdClientTimersRef, ServiceCfgPtr->SdClientServiceTimerIndex);

      /* timeout, previously received OFFER is no longer valid, so state machine goes back to INITIAL WAIT */
      ServiceStatePtr->ComPhase = SD_CLIENT_SERVICE_COMPHASE_INITIALWAIT;

      /* Timer will be checked and decremented at the end of the current Mainfunction invocation */
      ServiceStatePtr->StateMachineTimer = Sd_GetRandom(TimerCfgPtr->SdClientTimerInitialFindDelayMin, TimerCfgPtr->SdClientTimerInitialFindDelayMax) + 1U;

      /* exit code */
      Sd_ClientService_ComSM_exit_AVAILABLE(ServiceCfgPtr, ServiceStatePtr, ServiceIndex, FALSE, FALSE);
      ServiceStatePtr->SubscribeFlag = SD_CLIENT_SUBSCRIBEFLAG_NO_ACT;
      ServiceStatePtr->SubsAckEvent = FALSE;
    }
    break;

    /* CHECK: NOPARSE */
    case SD_CLIENT_SERVICE_COMPHASE_INITIALWAIT:
    case SD_CLIENT_SERVICE_COMPHASE_REPETITION:
      /* both of these cases are intended to be used only in development error detection mode */
#if (SD_DEV_ERROR_DETECT == STD_ON)
      SD_DET_REPORT_ERROR(SD_CLIENTSERVICE_TTL_RUN_OUT,SD_E_CLIENT_IMPOSSIBLE_STATE);
#endif /* SD_DEV_ERROR_DETECT == STD_ON */
      break;

    default: /* this default case is intended to be used only in development error detection mode */
#if (SD_DEV_ERROR_DETECT == STD_ON)
      SD_UNREACHABLE_CODE_ASSERT(SD_CLIENTSERVICE_COMSMEVENT_TIMEOUT_SVCID);
#endif /* SD_DEV_ERROR_DETECT == STD_ON */
      break;
      /* CHECK: PARSE */
  }
}



STATIC FUNC(void, SD_CODE) Sd_ClientService_ComSM_event_Offer
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  uint16 ServiceIndex,
  const boolean isMulticast
)
{
  switch(ServiceStatePtr->ComPhase)
  {
    case SD_CLIENT_SERVICE_COMPHASE_INITIALWAIT:
    case SD_CLIENT_SERVICE_COMPHASE_REPETITION:
      /* !LINKSTO Sd.ASR40.SWS_SD_00352,1 */
      /* !LINKSTO Sd.ASR40.SWS_SD_00365,1 */
      ServiceStatePtr->ComPhase = SD_CLIENT_SERVICE_COMPHASE_MAIN;
      Sd_ClientService_ComSM_SoConModeCheck(ServiceCfgPtr, ServiceStatePtr, ServiceIndex);
      break;

    case SD_CLIENT_SERVICE_COMPHASE_MAIN:
      Sd_ClientService_ComSM_SoConModeCheck(ServiceCfgPtr, ServiceStatePtr, ServiceIndex);
      break;

    case SD_CLIENT_SERVICE_COMPHASE_AVAILABLE:
      /* check if a TCP reference is configured for this client service */
      if(ServiceStatePtr->SoConIdTcp != SOAD_INVALID_SOCON_ID)
      {
        SoAd_SoConModeType SoConModeTcp = SOAD_SOCON_OFFLINE;

        /* return value can be ignored as the SoConMode will not be set in case of an error */
        (void)SoAd_GetSoConMode(ServiceStatePtr->SoConIdTcp, &SoConModeTcp);

        if(SoConModeTcp != SOAD_SOCON_ONLINE)
        {
          Sd_ClientService_ComSM_exit_AVAILABLE(ServiceCfgPtr, ServiceStatePtr, ServiceIndex, FALSE, TRUE);
          ServiceStatePtr->ComPhase = SD_CLIENT_SERVICE_COMPHASE_MAIN;
        }
      }
      break;

    case SD_CLIENT_SERVICE_COMPHASE_DOWN:
      break;

    /* CHECK: NOPARSE */
    default: /* this default case is intended to be used only in development error detection mode */
#if (SD_DEV_ERROR_DETECT == STD_ON)
      SD_UNREACHABLE_CODE_ASSERT(SD_CLIENTSERVICE_COMSMEVENT_OFFER_SVCID);
#endif /* SD_DEV_ERROR_DETECT == STD_ON */
      break;
      /* CHECK: PARSE */
  }

  /* !LINKSTO Sd.Design.ClientService.eventOffer.007,1 */
  if(ServiceCfgPtr->SdConsumedEventGroupCount != 0U)
  {
    if(isMulticast != FALSE)
    {
      Sd_RestartNodeResponseTimer
      (
        Sd_GetRandom(ServiceCfgPtr->SdClientTimerRequestResponseMinDelay,
                     ServiceCfgPtr->SdClientTimerRequestResponseMaxDelay) + 1U,
        ServiceStatePtr->NodeIndex,
        ServiceCfgPtr->SdInstanceIndex
      );
      ServiceStatePtr->SubscribeFlag = SD_CLIENT_SUBSCRIBEFLAG_MC_RESP;
    }
    else
    {
      Sd_RestartNodeResponseTimer(SD_TIMER_IMMEDIATE, ServiceStatePtr->NodeIndex, ServiceCfgPtr->SdInstanceIndex);
      ServiceStatePtr->SubscribeFlag = SD_CLIENT_SUBSCRIBEFLAG_NORMAL;
    }
  }
}



STATIC FUNC(void, SD_CODE) Sd_ClientService_ComSM_event_StopOffer
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  uint16 ServiceIndex,
  const boolean OfferFollows
)
{
  if(ServiceStatePtr->ComPhase == SD_CLIENT_SERVICE_COMPHASE_AVAILABLE)
  {
    ServiceStatePtr->ComPhase = SD_CLIENT_SERVICE_COMPHASE_MAIN;

    Sd_ClientService_ComSM_exit_AVAILABLE(ServiceCfgPtr, ServiceStatePtr, ServiceIndex, FALSE, OfferFollows);
  }

  ServiceStatePtr->SubscribeFlag = SD_CLIENT_SUBSCRIBEFLAG_NO_ACT;
}



STATIC FUNC(void, SD_CODE) Sd_ClientService_ComSM_SoConModeCheck
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  uint16 ServiceIndex
)
{
  const SoAd_SoConIdType UnicastUdpSoConId = ServiceStatePtr->SoConIdUdp;
  const SoAd_SoConIdType UnicastTcpSoConId = ServiceStatePtr->SoConIdTcp;

  /* check if an unicast UDP reference is configured for this client service */
  if( (UnicastUdpSoConId != SOAD_INVALID_SOCON_ID) || (UnicastTcpSoConId != SOAD_INVALID_SOCON_ID) )
  {
    SoAd_SoConModeType SoConModeUdp;
    SoAd_SoConModeType SoConModeTcp;

    if(UnicastUdpSoConId != SOAD_INVALID_SOCON_ID)
    { /* The return value can be ignored as the SoConMode will not be set in case of an error */
      (void)SoAd_GetSoConMode(UnicastUdpSoConId, &SoConModeUdp);
    }
    else
    {
      SoConModeUdp = SOAD_SOCON_ONLINE;
    }

    if(UnicastTcpSoConId != SOAD_INVALID_SOCON_ID)
    { /* The return value can be ignored as the SoConMode will not be set in case of an error */
      (void)SoAd_GetSoConMode(UnicastTcpSoConId, &SoConModeTcp);
    }
    else
    {
      SoConModeTcp = SOAD_SOCON_ONLINE;
    }

    if( (SoConModeUdp == SOAD_SOCON_ONLINE) && (SoConModeTcp == SOAD_SOCON_ONLINE) )
    {
      Sd_ClientService_ComSM_entry_AVAILABLE(ServiceCfgPtr, UnicastUdpSoConId, UnicastTcpSoConId, ServiceIndex);

      ServiceStatePtr->ComPhase = SD_CLIENT_SERVICE_COMPHASE_AVAILABLE;
    }
  }
  else /* this client service doesn't have an unicast data-path configured, therefore it it not needed to check its unicast socket connection state */
  {
    Sd_ClientService_ComSM_entry_AVAILABLE(ServiceCfgPtr, UnicastUdpSoConId, UnicastTcpSoConId, ServiceIndex);

    ServiceStatePtr->ComPhase = SD_CLIENT_SERVICE_COMPHASE_AVAILABLE;
  }
}



STATIC FUNC(void, SD_CODE) Sd_ClientService_ComSM_exit_DOWN
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ClientServiceCfgPtr
)
{
  if(ClientServiceCfgPtr->SdClientServiceSoConGroupUdp != SD_INVALID_SOCONGROUP_ID)
  { /* open UDP socket connections for unicast communication */
    P2CONST(SdSocketConnectionGroupType,AUTOMATIC,SD_APPL_CONST) SoConGroupCfgPtr =
        &SD_CFG(SdSocketConnectionGroupType, SdSoConGroupsRef, ClientServiceCfgPtr->SdClientServiceSoConGroupUdp);
    {
      SoAd_SoConIdType SoConId = SoConGroupCfgPtr->GroupStart;
      const SoAd_SoConIdType SoConIdEnd = SoConId + SoConGroupCfgPtr->SoConCount;

      do
      { /* The return value can be ignored as the only possible return value is E_OK */
        /* !LINKSTO Sd.ASR40.SWS_SD_00604,1 */
        /* !LINKSTO Sd.ASR40.SWS_SD_00464,1 */
        (void)SoAd_OpenSoCon(SoConId);
        ++SoConId;
      }
      while(SoConId < SoConIdEnd);
    }
  }

  if(ClientServiceCfgPtr->SdClientServiceSoConGroupTcp != SD_INVALID_SOCONGROUP_ID)
  { /* open TCP socket connections */
    P2CONST(SdSocketConnectionGroupType,AUTOMATIC,SD_APPL_CONST) SoConGroupCfgPtr =
        &SD_CFG(SdSocketConnectionGroupType, SdSoConGroupsRef, ClientServiceCfgPtr->SdClientServiceSoConGroupTcp);
    {
      SoAd_SoConIdType SoConId = SoConGroupCfgPtr->GroupStart;
      const SoAd_SoConIdType SoConIdEnd = SoConId + SoConGroupCfgPtr->SoConCount;

      do
      { /* The return value can be ignored as the only possible return value is E_OK */
        /* !LINKSTO Sd.ASR40.SWS_SD_00604,1 */
        /* !LINKSTO Sd.ASR40.SWS_SD_00464,1 */
        (void)SoAd_OpenSoCon(SoConId);
        ++SoConId;
      }
      while(SoConId < SoConIdEnd);
    }
  }
}



STATIC FUNC(void, SD_CODE) Sd_ClientService_ComSM_entry_INITIALWAIT
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ClientServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ClientServiceStatePtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr
)
{
   P2CONST(SdClientTimer_t, AUTOMATIC, SD_APPL_DATA) TimerCfgPtr =
       &SD_CFG(SdClientTimer_t, SdClientTimersRef, ClientServiceCfgPtr->SdClientServiceTimerIndex);
  /* Get random FIND timer value for this client service,
     this timer value will be checked and decremented at the end of the current Mainfunction invocation */
   /* timer for the first FIND message is started here */
   Sd_StartTimer(&ClientServiceStatePtr->StateMachineTimer,
                 Sd_GetRandom(TimerCfgPtr->SdClientTimerInitialFindDelayMin, TimerCfgPtr->SdClientTimerInitialFindDelayMax) + 1U);

   if( (InstanceStatePtr->ClientsDeltaTimer == 0U) || (ClientServiceStatePtr->StateMachineTimer < InstanceStatePtr->ClientsDeltaTimer) )
   { /* set next timeout */
     InstanceStatePtr->ClientsDeltaTimer = ClientServiceStatePtr->StateMachineTimer;
     InstanceStatePtr->ClientsDelta = ClientServiceStatePtr->StateMachineTimer;
   }
   else
   {
     ClientServiceStatePtr->StateMachineTimer += (InstanceStatePtr->ClientsDelta - InstanceStatePtr->ClientsDeltaTimer);
   }
   ++InstanceStatePtr->ClientServicesTimersActive;
   /* state machine for this client service is set to INITIAL WAIT */
   ClientServiceStatePtr->ComPhase = SD_CLIENT_SERVICE_COMPHASE_INITIALWAIT;
}



STATIC FUNC(void, SD_CODE) Sd_ClientService_ComSM_entry_AVAILABLE
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  const SoAd_SoConIdType UdpSoConId,
  const SoAd_SoConIdType TcpSoConId,
  uint16 ServiceIndex
)
{
  /* activate SoAdRoutingGroup for consumed methods (if any exist) */
  if(ServiceCfgPtr->SdClientServiceActivationRef != SOAD_INVALID_ROUTINGGROUP_ID)
  {
    if(UdpSoConId != SOAD_INVALID_SOCON_ID)
    { /* !LINKSTO Sd.ASR40.SWS_SD_00362,1 */
      /* The return value can be ignored as the only possible return value is E_OK */
      (void)SoAd_EnableSpecificRouting(ServiceCfgPtr->SdClientServiceActivationRef, UdpSoConId);
    }
    /* a received OFFER can contain both an UDP endpoint and a TCP endpoint, in the future
     * it will be possible to receive multicast only OFFER messages, but this is not supported for the moment
     */
    if(TcpSoConId != SOAD_INVALID_SOCON_ID)
    {  /* return value can be ignored as the only possible return value is E_OK */
      (void)SoAd_EnableSpecificRouting(ServiceCfgPtr->SdClientServiceActivationRef, TcpSoConId);
    }
  }

  BswM_Sd_ClientServiceCurrentState(ServiceIndex, SD_CLIENT_SERVICE_AVAILABLE);

  if(ServiceCfgPtr->SdConsumedEventGroupCount > 0U)
  {
    uint16_least EventgroupIndex = ServiceCfgPtr->SdConsumedEventGroup;
    const uint16_least EventgroupIndexEnd = EventgroupIndex + ServiceCfgPtr->SdConsumedEventGroupCount;
    P2CONST(SdConsumedEventGroup_t, AUTOMATIC, SD_APPL_CONST) EventGroupCfgPtr = &SD_CFG(SdConsumedEventGroup_t, SdConsumedEventGroupsRef, EventgroupIndex);
    P2VAR(Sd_ConsumedEventGroupStateType,AUTOMATIC,SD_APPL_CONST) EventGroupStatePtr = &Sd_ConsumedEventGroupStates[EventgroupIndex];

    do
    { /* check if this consumed event group has been requested */
      if(EventGroupStatePtr->MachineState != SD_CONSUMED_EVENTGROUP_STATE_RELEASED)
      { /* ONLY if this consumed event group has a TCP activation routing group configured AND if the received OFFER had a TCP option attached to it
         * then this TCP routing datapath will be enabled */
        if( (EventGroupCfgPtr->SdConsumedEventGroupTcpActivationRef != SOAD_INVALID_ROUTINGGROUP_ID) && (TcpSoConId != SOAD_INVALID_SOCON_ID) )
        { /* !LINKSTO Sd.ASR40.SWS_SD_00702,1 */
          /* The return value can be ignored as the only possible return value is E_OK */
          (void)SoAd_EnableSpecificRouting(EventGroupCfgPtr->SdConsumedEventGroupTcpActivationRef, TcpSoConId);
          EventGroupStatePtr->MachineState = SD_CONSUMED_EVENTGROUP_STATE_REQUESTED_OFFER_RECEIVED;
        }
        /* ONLY if this consumed event group has an UDP activation routing group configured AND if the received OFFER had a UDP option attached to it
         * then this UDP routing datapath will be enabled */
        if( (EventGroupCfgPtr->SdConsumedEventGroupUdpActivationRef != SOAD_INVALID_ROUTINGGROUP_ID) && (UdpSoConId != SOAD_INVALID_SOCON_ID) )
        { /* !LINKSTO Sd.ASR40.SWS_SD_00703,1 */
          /* The return value can be ignored as the only possible return value is E_OK */
          (void)SoAd_EnableSpecificRouting(EventGroupCfgPtr->SdConsumedEventGroupUdpActivationRef, UdpSoConId);
          EventGroupStatePtr->MachineState = SD_CONSUMED_EVENTGROUP_STATE_REQUESTED_OFFER_RECEIVED;
        }
        /* open UDP socket connections for multicast receptions only if this consumed event group has a multicast UDP reference configured */
        if(EventGroupCfgPtr->SdConsumedEventGroupMulticastSoConId != SOAD_INVALID_SOCON_ID)
        {
          uint16_least SoConId = EventGroupCfgPtr->SdConsumedEventGroupMulticastSoConId;
          const uint16_least SoConIdxEnd = SoConId + EventGroupCfgPtr->SdConsumEventGroupMulticastRefCount;

          /* close all socket connections of this multicast socket connection group */
          do
          {  /* return value can be ignored as the only possible return value is E_OK */
            (void)SoAd_OpenSoCon((SoAd_SoConIdType)SoConId);
            ++SoConId;
          }
          while(SoConId < SoConIdxEnd);
          EventGroupStatePtr->MachineState = SD_CONSUMED_EVENTGROUP_STATE_REQUESTED_OFFER_RECEIVED;
        }
      }
      /* increment counter and all event group pointers */
      ++EventgroupIndex;
      ++EventGroupCfgPtr;
      ++EventGroupStatePtr;
    }
    while(EventgroupIndex < EventgroupIndexEnd);
  }
}



STATIC FUNC(void, SD_CODE) Sd_ClientService_ComSM_exit_AVAILABLE
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  uint16 ServiceIndex,
  const boolean isStop,
  const boolean PendingOffer
)
{
  const SoAd_SoConIdType UnicastUdpSoConId = ServiceStatePtr->SoConIdUdp;
  const SoAd_SoConIdType UnicastTcpSoConId = ServiceStatePtr->SoConIdTcp;

  if(ServiceCfgPtr->SdConsumedEventGroupCount > 0U)
  {
    Sd_ClientService_Disable_All_ConsumedEventGroups(ServiceCfgPtr, ServiceStatePtr, isStop);
  }

  BswM_Sd_ClientServiceCurrentState(ServiceIndex, SD_CLIENT_SERVICE_DOWN);

  /* deactivate SoAdRoutingGroup for consumed methods (if any exist) */
  if(ServiceCfgPtr->SdClientServiceActivationRef != SOAD_INVALID_ROUTINGGROUP_ID)
  {
    if(UnicastUdpSoConId != SOAD_INVALID_SOCON_ID)
    {
      (void)SoAd_DisableSpecificRouting(ServiceCfgPtr->SdClientServiceActivationRef, UnicastUdpSoConId);
    }
    /* for the moment a received OFFER has to contain either an UDP endpoint oder a TCP endpoint, in the future
     * it will be possible to receive multicast only OFFER messages, but this is not supported for the moment
     */
    if(UnicastTcpSoConId != SOAD_INVALID_SOCON_ID)
    {
      (void)SoAd_DisableSpecificRouting(ServiceCfgPtr->SdClientServiceActivationRef, UnicastTcpSoConId);
    }
  }
  if(PendingOffer == FALSE)
  {  /* unicast socket connection is only released if there is no pending OFFER to be processed in the same main cycle */
    if(isStop == FALSE)
    {
      if(UnicastUdpSoConId != SOAD_INVALID_SOCON_ID)
      { /* this consumed event group's data path is no longer being used, its socket connection can be released to wild card */
        /* !LINKSTO Sd.ASR40.SWS_SD_00705,1 */
        SoAd_ReleaseRemoteAddr(UnicastUdpSoConId);
        ServiceStatePtr->SoConIdUdp = SOAD_INVALID_SOCON_ID;
      }
      if(UnicastTcpSoConId != SOAD_INVALID_SOCON_ID)
      { /* this consumed event group's data path is no longer being used, its socket connection can be released to wild card */
        /* !LINKSTO Sd.ASR40.SWS_SD_00705,1 */
        SoAd_ReleaseRemoteAddr(UnicastTcpSoConId);
        ServiceStatePtr->SoConIdTcp = SOAD_INVALID_SOCON_ID;
      }
    }
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
    /* !LINKSTO Sd.ASR40.SWS_SD_00740,1 */
    /* this client service has been either switched off or is no longer requested, then all flags used to handle retries of subscription */
    ServiceStatePtr->SdSubscribeEventGroupTimersActive = FALSE;
    ServiceStatePtr->SdSubscribeEventGroupsNotAck = 0U;
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
  }
}



STATIC FUNC(void, SD_CODE) Sd_ClientService_Disable_All_ConsumedEventGroups
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  const boolean isStop
)
{
  uint16_least EventgroupIndex = ServiceCfgPtr->SdConsumedEventGroup;
  const uint16_least EventgroupIndexEnd = EventgroupIndex + ServiceCfgPtr->SdConsumedEventGroupCount;
  P2VAR(Sd_ConsumedEventGroupStateType,AUTOMATIC,SD_APPL_CONST) EventGroupStatePtr = &Sd_ConsumedEventGroupStates[EventgroupIndex];
  P2CONST(SdConsumedEventGroup_t, AUTOMATIC, SD_APPL_CONST) EventGroupCfgPtr = &SD_CFG(SdConsumedEventGroup_t, SdConsumedEventGroupsRef, EventgroupIndex);

  const SoAd_SoConIdType UnicastUdpSoConId = ServiceStatePtr->SoConIdUdp;
  const SoAd_SoConIdType UnicastTcpSoConId = ServiceStatePtr->SoConIdTcp;

  /* Disable all consumed eventgroups */
  do
  { /* check if this event group has been requested, that is, if it is active */
    if( (EventGroupStatePtr->MachineState == SD_CONSUMED_EVENTGROUP_STATE_REQUESTED_OFFER_RECEIVED) ||
        (EventGroupStatePtr->MachineState == SD_CONSUMED_EVENTGROUP_STATE_WAIT_FOR_AVALIABILITY) ||
        (EventGroupStatePtr->MachineState == SD_CONSUMED_EVENTGROUP_STATE_AVAILABLE) )
    {
      /* if its current state is AVAILABE, then this state has to be exited */
      if(EventGroupStatePtr->MachineState == SD_CONSUMED_EVENTGROUP_STATE_AVAILABLE)
      { /* event group is active, and action is needed to be deactivated */
        /* !LINKSTO Sd.ASR40.SWS_SD_00712,1 */
        Sd_ClientService_CEvg_exit_AVAILABLE(EventGroupCfgPtr, EventGroupStatePtr, (uint16)EventgroupIndex);
        EventGroupStatePtr->TTL = SD_TTL_OFF;
      }
      /* ONLY if this consumed event group has a TCP activation routing group configured AND if the received OFFER had a TCP option attached to it
       * then this TCP routing datapath will be disabled */
      if( (EventGroupCfgPtr->SdConsumedEventGroupTcpActivationRef != SOAD_INVALID_ROUTINGGROUP_ID) && (UnicastTcpSoConId != SOAD_INVALID_SOCON_ID) )
      {
        /* return value can be ignored as the only possible return value is E_OK */
        (void)SoAd_DisableSpecificRouting(EventGroupCfgPtr->SdConsumedEventGroupTcpActivationRef, UnicastTcpSoConId);
      }
      /* ONLY if this consumed event group has an UDP activation routing group configured AND if the received OFFER had a UDP option attached to it
       * then this UDP routing datapath will be enabled */
      if( (EventGroupCfgPtr->SdConsumedEventGroupUdpActivationRef != SOAD_INVALID_ROUTINGGROUP_ID) && (UnicastUdpSoConId != SOAD_INVALID_SOCON_ID) )
      { /* return value can be ignored as the only possible return value is E_OK */
        (void)SoAd_DisableSpecificRouting(EventGroupCfgPtr->SdConsumedEventGroupUdpActivationRef, UnicastUdpSoConId);
      }
      /* close UDP socket connections for multicast receptions */
      if( (EventGroupCfgPtr->SdConsumedEventGroupMulticastSoConId != SOAD_INVALID_SOCON_ID) &&
          (Sd_ClientConsumEventGroupMulticastRefSubsCounter[EventGroupCfgPtr->SdConsumEventGroupMulticastRefIndex] == 0U) )
      {
        uint16_least SoConId = EventGroupCfgPtr->SdConsumedEventGroupMulticastSoConId;
        const uint16_least SoConIdxEnd = SoConId + EventGroupCfgPtr->SdConsumEventGroupMulticastRefCount;

        /* close all socket connections of this multicast socket connection group */
        do
        { /* The return value can be ignored as the only possible return value is E_OK */
          (void)SoAd_CloseSoCon((SoAd_SoConIdType)SoConId, FALSE);
          ++SoConId;
        }
        while(SoConId < SoConIdxEnd);
      }
      /* set state machine back to consumed event group requested but its client does not have a received offer */
      EventGroupStatePtr->MachineState = SD_CONSUMED_EVENTGROUP_STATE_REQUESTED_NO_OFFER;

      if( (isStop != FALSE) && (EventGroupStatePtr->Acknowledged != SD_CEVG_NO_SUBSCRIPTION_SENT) )
      {
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
        /* !LINKSTO Sd.ASR40.SWS_SD_00738,1 */
        /* !LINKSTO Sd.ASR40.SWS_SD_00712,1 */
        EventGroupStatePtr->SdSubscribeEventgroupRetryCounter = 0U;
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
        EventGroupStatePtr->SendStopSubsc = TRUE;
        EventGroupStatePtr->Acknowledged = SD_CEVG_NO_SUBSCRIPTION_SENT;
        Sd_RestartNodeResponseTimer(SD_TIMER_IMMEDIATE, ServiceStatePtr->NodeIndex, ServiceCfgPtr->SdInstanceIndex);
      }
    }
    if(isStop != FALSE)
    {
      EventGroupStatePtr->MachineState = SD_CONSUMED_EVENTGROUP_STATE_RELEASED;
    }
    ++EventGroupCfgPtr;
    ++EventgroupIndex;
    ++EventGroupStatePtr;
  }
  while(EventgroupIndex < EventgroupIndexEnd);
}



STATIC FUNC(void, SD_CODE) Sd_ClientService_ProcessOffers
(
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstStatePtr,
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  uint16_least *ClientOffersSubAcksCtrPtr,
  uint16 ServiceIdx
)
{
  uint16_least ClientOffersSubAcksCounter = *ClientOffersSubAcksCtrPtr;
  const uint8 OfferEvents = ServiceStatePtr->SdOfferEvents;
  ServiceStatePtr->SdOfferEvents = 0U;

  /* check if this client service received a STOP OFFER SD event since that main cycle */
  if((OfferEvents & SD_CLIENT_EVENT_STOPOFFER_0) != 0U)
  {
    /* process STOP OFFER event */
    Sd_ClientService_ComSM_event_StopOffer(ServiceCfgPtr, ServiceStatePtr, ServiceIdx, !(OfferEvents & SD_CLIENT_EVENT_STOPOFFER_1));
    --ClientOffersSubAcksCounter;
  }

  /* now check if also an OFFER message has been received since the last main cycle */
  if( (OfferEvents & SD_CLIENT_EVENT_UNICAST_OFFER) != 0U )
  {
    if(ServiceStatePtr->StateMachineTimer != 0U)
    { /* !LINKSTO Sd.ASR40.SWS_SD_00352,1 */
      /* !LINKSTO Sd.ASR40.SWS_SD_00365,1 */
      /* in case this client service is in INITIAL_WAIT, REPETITION of in MAIN phase, its state timer has to be reset when an offer is received */
      Sd_StopTimer(&ServiceStatePtr->StateMachineTimer);
#if (SD_DEV_ERROR_DETECT == STD_ON)
      /* this defensive check covers the case of an underflow in the variable ClientServicesTimersActive */
      SD_PRECONDITION_ASSERT( (InstStatePtr->ClientServicesTimersActive > 0U), SD_CLIENTSERVICEMAINFUNCTION_SVCID );
#endif
      --InstStatePtr->ClientServicesTimersActive;
      if(InstStatePtr->ClientServicesTimersActive == 0U)
      {
        InstStatePtr->ClientsDeltaTimer = 0U;
      }
    }

    Sd_ClientService_ComSM_event_Offer(ServiceCfgPtr, ServiceStatePtr, ServiceIdx, FALSE);
    --ClientOffersSubAcksCounter;

    if( (OfferEvents & SD_CLIENT_EVENT_MULTICAST_OFFER) != 0U )
    {
      --ClientOffersSubAcksCounter;
    }
    if( (ServiceStatePtr->ComPhase == SD_CLIENT_SERVICE_COMPHASE_MAIN) && ((OfferEvents & SD_CLIENT_EVENT_STOPOFFER_1) == 0U) )
    {
      InstStatePtr->ClientServicesEventsCount += SD_INCREASE_CLIENT_OFFER_EVENT;
      ServiceStatePtr->SdOfferEvents = SD_CLIENT_EVENT_UNICAST_OFFER;
    }
  }
  else if( (OfferEvents & SD_CLIENT_EVENT_MULTICAST_OFFER) != 0U )
  {
    if(ServiceStatePtr->StateMachineTimer != 0U)
    {
      Sd_StopTimer(&ServiceStatePtr->StateMachineTimer);
#if (SD_DEV_ERROR_DETECT == STD_ON)
      /* this defensive check covers the case of an underflow in the variable ClientServicesTimersActive */
      SD_PRECONDITION_ASSERT( (InstStatePtr->ClientServicesTimersActive > 0U), SD_CLIENTSERVICE_PROCESSOFFERS_SVCID );
#endif
      --InstStatePtr->ClientServicesTimersActive;
      if(InstStatePtr->ClientServicesTimersActive == 0U)
      {
        InstStatePtr->ClientsDeltaTimer = 0U;
      }
    }

    Sd_ClientService_ComSM_event_Offer(ServiceCfgPtr, ServiceStatePtr, ServiceIdx, TRUE);
    --ClientOffersSubAcksCounter;
    if( (ServiceStatePtr->ComPhase == SD_CLIENT_SERVICE_COMPHASE_MAIN) && ((OfferEvents & SD_CLIENT_EVENT_STOPOFFER_1) == 0U) )
    {
      InstStatePtr->ClientServicesEventsCount += SD_INCREASE_CLIENT_OFFER_EVENT;
      ServiceStatePtr->SdOfferEvents = SD_CLIENT_EVENT_MULTICAST_OFFER;
    }
  }
  else
  {
    /* do nothing, no OFFER event */
  }

  if((OfferEvents & SD_CLIENT_EVENT_STOPOFFER_1) != 0U)
  {
    P2VAR(Sd_RemoteNodeStateType,AUTOMATIC,SD_APPL_DATA) RemoteNodeStatePtr = Sd_GetRemoteNodeState(ServiceStatePtr->NodeIndex);

    Sd_ClientService_ComSM_event_StopOffer(ServiceCfgPtr, ServiceStatePtr, ServiceIdx, FALSE);

#if (SD_DEV_ERROR_DETECT == STD_ON)
      /* this defensive check covers the case of an underflow in the variable ClientServicesTimersActive */
      SD_PRECONDITION_ASSERT( (RemoteNodeStatePtr->ClientServicesOffersReceived > 0U), SD_CLIENTSERVICE_PROCESSOFFERS_SVCID );
#endif
    --RemoteNodeStatePtr->ClientServicesOffersReceived;
    if( (RemoteNodeStatePtr->ClientServicesOffersReceived == 0U) && (RemoteNodeStatePtr->ServerServicesFindsReceived == 0U) )
    {
      RemoteNodeStatePtr->ResponseTimer = 0U;
    }
    ServiceStatePtr->NodeIndex = SD_INVALID_REMOTENODE_ID;
    ServiceStatePtr->SoConIdUdp = SOAD_INVALID_SOCON_ID;
    ServiceStatePtr->SoConIdTcp = SOAD_INVALID_SOCON_ID;
    ServiceStatePtr->TTLreceived = SD_TTL_OFF;
    ServiceStatePtr->OfferReceived = FALSE;
    ServiceStatePtr->SubsAckEvent = FALSE;
  }
  *ClientOffersSubAcksCtrPtr = ClientOffersSubAcksCounter;
}



STATIC FUNC(void, SD_CODE) Sd_Sd_ClientService_ProcessSubsAcks
(
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstStatePtr,
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_CONST) ServiceCfgPtr,
  CONSTP2VAR(Sd_ClientServiceStateType,AUTOMATIC,SD_VAR) ServiceStatePtr,
  uint16_least *ClientOffersSubAcksCtrPtr
)
{
  uint16_least ClientOffersSubAcksCounter = *ClientOffersSubAcksCtrPtr;
  uint16_least EventgroupIndex = ServiceCfgPtr->SdConsumedEventGroup;
  const uint16_least EventgroupIndexEnd = EventgroupIndex + ServiceCfgPtr->SdConsumedEventGroupCount;
  P2VAR(Sd_ConsumedEventGroupStateType,AUTOMATIC,SD_APPL_CONST) EventGroupStatePtr = &Sd_ConsumedEventGroupStates[EventgroupIndex];
  P2CONST(SdConsumedEventGroup_t, AUTOMATIC, SD_APPL_CONST) EventGroupConfigPtr = &SD_CFG(SdConsumedEventGroup_t, SdConsumedEventGroupsRef, EventgroupIndex);

  do
  {
    if(EventGroupStatePtr->MachineState == SD_CONSUMED_EVENTGROUP_STATE_WAIT_FOR_AVALIABILITY)
    {
      SoAd_SoConModeType SoConMode = SOAD_SOCON_OFFLINE;
      /* The return value can be ignored as the SoConMode will not be set in case of an error */
      (void)SoAd_GetSoConMode(EventGroupConfigPtr->SdConsumedEventGroupMulticastSoConId, &SoConMode);

      if(SoConMode != SOAD_SOCON_OFFLINE)
      {
        Sd_ClientService_CEvg_GoTo_AVAILABLE(EventGroupConfigPtr, EventGroupStatePtr, (uint16)EventgroupIndex);
        EventGroupStatePtr->MachineState = SD_CONSUMED_EVENTGROUP_STATE_AVAILABLE;
      }
      else
      {
        InstStatePtr->ClientServicesEventsCount += SD_INCREASE_CLIENT_OFFER_EVENT;
        ServiceStatePtr->SubsAckEvent = TRUE;
      }
      --ClientOffersSubAcksCounter;
    }
    ++EventgroupIndex;
    ++EventGroupConfigPtr;
    ++EventGroupStatePtr;
  }
  while( (ClientOffersSubAcksCounter != 0U) && (EventgroupIndex < EventgroupIndexEnd) );

  *ClientOffersSubAcksCtrPtr = ClientOffersSubAcksCounter;
}


#define SD_STOP_SEC_CODE
#include <Sd_MemMap.h>

/*==================[end of file]===========================================*/
