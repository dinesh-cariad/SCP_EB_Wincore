#ifndef SD_CLIENTSERVICE_INT_H_
#define SD_CLIENTSERVICE_INT_H_

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

/*==================[inclusions]============================================*/
#include <Sd_Int.h>

/*==================[macros]================================================*/

/* Client service com phases*/
#define SD_CLIENT_SERVICE_COMPHASE_DOWN        0U
#define SD_CLIENT_SERVICE_COMPHASE_INITIALWAIT 1U
#define SD_CLIENT_SERVICE_COMPHASE_REPETITION  2U
#define SD_CLIENT_SERVICE_COMPHASE_MAIN        3U
#define SD_CLIENT_SERVICE_COMPHASE_AVAILABLE   4U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
#define SD_START_SEC_CODE
#include <Sd_MemMap.h>

/** \brief Initialize the SW-Unit ClientService internal variables.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ClientService_init(void);

/** \brief Initialize the SW-Unit ClientService internal variables.
 **
  ** \param[in] InstanceCfgPtr    Pointer to the configuration data of this instance
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ClientServices_init_after_halt
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr
);

/** \brief Main-Function part of SW-Unit ClientService
 **
 ** Drives the timer-based actions of the ClientService state-machine.
 **
 ** \param[in] InstanceCfgPtr    Pointer to the configuration data of this instance
 ** \param[in] InstanceStatePtr  Pointer to this instances current state flags and variables
 ** \param[in] ClientEventsCount Number of BswM events
 ** \param[in] ClientOffersSubAcksCount  Number of OFFERS or SUBSACKS received in this instance since last main cycle
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ClientService_mainFunction
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr,
  uint16 ClientEventsCount,
  uint16 ClientOffersSubAcksCount
);

/** \brief Process this instances clients timers
 **
 ** This function is called periodically for each Sd instance
 ** to process this instances clients state machine related timers
 **
 ** \param[in] InstanceCfgPtr  Pointer to the configuration data of this instance
 ** \param[in] InstanceStatePtr  Pointer to this instances current state flags and variables
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ClientService_mainTimersFunction
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr
);

/** \brief Process this instances TTLs
 **
 ** This function is called periodically for each Sd instance
 ** to process this instances clients TTLs
 **
 ** \param[in] InstanceCfgPtr  Pointer to the configuration data of this instance
 ** \param[in] InstanceStatePtr  Pointer to this instances current state flags and variables
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ClientService_mainTTL
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr
);

/** \brief Transmission of SubscribeEventGroup entries
 **
 ** This function is called periodically for each Sd instance and each remote node.
 ** It transmits the SubscribeEventGroup entries per remote node if required.
 **
 ** \param[in] InstanceCfgPtr  Pointer to the configuration data of this instance
 ** \param[in] RemoteNodeIndex  index of the remote node
 ** \param[in] ClientsSubscribessToProcess Number of subscribes to process pro remote node
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ClientService_sendEntriesUnicast
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_RemoteNodeStateType,AUTOMATIC,SD_VAR) RemoteNodeStatePtr,
  const Sd_RemoteNodeIdType RemoteNodeIndex
);

#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
/** \brief Transmission of SubscribeEventGroup retries
 **
 ** This function is called periodically for each Sd instance and each remote node.
 ** It transmits the SubscribeEventGroup entries per remote node if required.
 **
 ** \param[in] InstanceCfgPtr  Pointer to the configuration data of this instance
 ** \param[in] RemoteNodeIndex  index of the remote node
 ** \param[in] RemoteNodeClientRetriesToSend Number of retries to process pro remote node
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
TS_MOD_PRIV_DECL FUNC(uint16, SD_CODE) Sd_ClientService_sendRetriesOfSubscribeEventGroups
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  uint16 RemoteNodeClientRetriesToSend
);
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */

/** \brief received 'OFFER' entry trigger function
 **
 ** This function shall be called for each offerService entry received.
 ** This function performs the clientService statemachine related transitions.
 **
 ** \param[in] ServiceInfoPtr   Service Info received
 ** \param[in] UdpSockAddrPtr   UDP socket address
 ** \param[in] TTL              time-to-live received
 ** \param[in] MinorVersion     minor version received
 ** \param[in] RemoteNodeIndex  index of the remote node
 ** \param[in] SdInstanceIndex  index of network instance
 ** \param[in] IsMulticast          TRUE: received as multicast message, FALSE: received as unicast message
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
#if(  (SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DEM) || \
      ( (SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON) )  )
TS_MOD_PRIV_DECL FUNC(uint8, SD_CODE) Sd_ClientService_eventOfferService
#else
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ClientService_eventOfferService
#endif
(
  CONSTP2CONST(Sd_ServiceInfoType, AUTOMATIC, SD_APPL_DATA) ServiceInfoPtr,
  CONSTP2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfoPtr,
  const uint32 TTL,
  const uint32 MinorVersion,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  const uint8 SdInstanceIndex,
  const boolean IsMulticast
);

/** \brief received ' STOP OFFER' entry trigger function
 **
 ** This function shall be called for each offerService entry received.
 ** This function performs the clientService statemachine related transitions.
 **
 ** \param[in] ServiceInfoPtr   Service Info received
 ** \param[in] UdpSockAddrPtr   UDP socket address
 ** \param[in] MinorVersion     minor version received
 ** \param[in] RemoteNodeIndex  index of the remote node
 ** \param[in] SdInstanceIndex  index of network instance
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ClientService_eventStopOfferService
(
  CONSTP2CONST(Sd_ServiceInfoType, AUTOMATIC, SD_APPL_DATA) ServiceInfoPtr,
  CONSTP2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfoPtr,
  const uint32 MinorVersion,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  const uint8 SdInstanceIndex
);

/** \brief received 'subscribeEventGroupAck' entry trigger function
 **
 ** This function shall be called for each subscribeEventGroupAck entry received.
 ** This function performs the clientService statemachine related transitions.
 **
 ** \param[in] ServiceInfoPtr   Service Info received
 ** \param[in] UdpSockAddrPtr   UDP socket address
 ** \param[in] TTL              time-to-live received (TTL must not be equal to 0)
 ** \param[in] EventgroupID     Eventgroup ID received
 ** \param[in] ReservedField    Reserved Field
 ** \param[in] SdInstanceIndex  index of network instance
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ClientService_eventSubscribeAck
(
  CONSTP2CONST(Sd_ServiceInfoType, AUTOMATIC, SD_APPL_DATA) ServiceInfoPtr,
  CONSTP2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfoPtr,
  const uint32 TTL,
  const uint32 MinorVersion,
  const uint16 EventgroupID,
  Sd_RemoteNodeIdType RemoteNodeIndex,
  const uint8 SdInstanceIndex
);

/** \brief starts all client services of a particular Sd instance
 **
 ** \param[in] InstanceCfgPtr  Pointer to the configuration data of this instance
 ** \param[in] InstanceStatePtr  Pointer to this instances current state flags and variables
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ClientService_startServices
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr
);

/** \brief halts all client services of a particular Sd instance
 **
 ** \param[in] InstanceCfgPtr  Pointer to the configuration data of this instance
 ** \param[in] InstanceStatePtr  Pointer to this instances current state flags and variables
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ClientService_haltServices
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr
);

/** \brief indicates a particular remote node has been reset.
 **
 ** \param[in] InstanceCfgPtr  Pointer to the configuration data of this instance
 ** \param[in] RemoteNodeIndex  index of the remote node
 ** \param[in] SdInstanceIndex  index of network instance
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ClientService_resetRemoteConnection
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType, AUTOMATIC, SD_APPL_DATA) InstanceStatesPtr,
  const Sd_RemoteNodeIdType RemoteNodeIndex
);

/** \brief Get actual state of ClientService state machine
 **
 ** \param[in] ServiceIndex    server service handle Id
 **
 ** \return state of client service
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(uint8, SD_CODE) Sd_ClientService_getComphase
(
  const uint16 ServiceIndex
);

#define SD_STOP_SEC_CODE
#include <Sd_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* SD_CLIENTSERVICE_INT_H_ */
