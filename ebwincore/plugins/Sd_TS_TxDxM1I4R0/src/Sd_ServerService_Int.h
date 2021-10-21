#ifndef SD_SERVERSERVICE_INT_H_
#define SD_SERVERSERVICE_INT_H_

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

/* Server service com phases*/
#define SD_SERVER_SERVICE_COMPHASE_DOWN        0U
#define SD_SERVER_SERVICE_COMPHASE_INITIALWAIT 1U
#define SD_SERVER_SERVICE_COMPHASE_REPETITION  2U
#define SD_SERVER_SERVICE_COMPHASE_MAIN        3U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define SD_START_SEC_CODE
#include <Sd_MemMap.h>

/** \brief Initialize the SW-Unit ServerService internal variables.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ServerService_init(void);

/** \brief Initialize the SW-Unit ClientService internal variables after a halt
 **
 ** \param[in] InstanceCfgPtr       Pointer to the configuration data of this instance
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ServerServices_init_after_halt
(
    CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr
);

/** \brief Main-Function part of SW-Unit ServerService
 **
 ** Handles events from BswM module
 ** Drives the timer-based actions of the ServerService state-machine.
 **
 ** \param[in] InstanceCfgPtr       Pointer to configuration data of this instance
 ** \param[in] InstanceStatePtr     Pointer to this instances current state flags and variables
 ** \param[in] ServicesEventsCount  Number of BswM events
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ServerService_mainFunction
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr,
  const uint16 ServicesEventsCount
);

/** \brief Process this instances clients timers
 **
 ** This function is called periodically for each Sd instance
 ** to process this instances servers state machine related timers
 **
 ** \param[in] InstanceCfgPtr    Pointer to the configuration data of this instance
 ** \param[in] InstanceStatePtr  Pointer to this instances current state flags and variables
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ServerService_mainTimersFunction
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr
);

/** \brief Process this instances clients TTLs
 **
 ** This function is called periodically for each Sd instance
 ** to process this instances servers TTLs
 **
 ** \param[in] InstanceCfgPtr    Pointer to the configuration data of this instance
 ** \param[in] InstanceStatePtr  Pointer to this instances current state flags and variables
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ServerService_handleTTL
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr
);

/** \brief Transmission of Offer entries oder unicast
 **
 ** This function is called periodically for each Sd instance and each remote node.
 ** It transmits the Offer entries per remote node if required.
 **
 ** \param[in] InstanceCfgPtr         Pointer to the configuration data of this instance
 ** \param[in] RemoteNodeIndex        Index of the remote node
 ** \param[in] ServersFindsToProcess  Number of finds events to be processed since last main cycle
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ServerService_sendEntriesUnicast
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  uint16 ServersFindsToProcess
);

/** \brief starts all server services of a particular Sd instance
 **
 ** \param[in] InstanceCfgPtr    Pointer to the configuration data of this instance
 ** \param[in] InstanceStatePtr  Pointer to this instances current state flags and variables
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ServerService_startServices
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr
);

/** \brief halts all server services of a particular Sd instance
 **
 ** \param[in] InstanceCfgPtr    Pointer to the configuration data of this instance
 ** \param[in] InstanceStatePtr  Pointer to this instances current state flags and variables
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ServerService_haltServices
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr
);

/** \brief Process received Find entry
 **
 ** \param[in] ServiceInfoPtr   Service Info received
 ** \param[in] EntryOptionInfoPtr Options of entry
 ** \param[in] MinorVersion     minor version received
 ** \param[in] SdInstanceIndex  index of network instance
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ServerService_eventFind
(
  CONSTP2CONST(Sd_ServiceInfoType, AUTOMATIC, SD_APPL_DATA) ServiceInfoPtr,
  CONSTP2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfoPtr,
  const uint32 MinorVersion,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  const uint8 SdInstanceIndex
);

/** \brief Process received SubscribeEventgroup entry
 **
 ** \param[in] ServiceInfoPtr   Service Info received
 ** \param[in] EntryOptionInfoPtr Options of entry
 ** \param[in] TTL              time-to-live received
 ** \param[in] EventgroupID     Eventgroup ID received
 ** \param[in] ReservedField    Reserved Field
 ** \param[in] RemoteNodeIndex  index of the remote node
 ** \param[in] SdInstanceIndex  index of network instance
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ServerService_eventSubscribe
(
  CONSTP2CONST(Sd_ServiceInfoType, AUTOMATIC, SD_APPL_DATA) ServiceInfoPtr,
  CONSTP2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfoPtr,
  const uint32 TTL,
  const uint16 ReservedField,
  const uint16 EventgroupID,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  const uint8 SdInstanceIndex
);

/** \brief Process received StopSubscribeEventgroup entry
 **
 ** \param[in] ServiceInfoPtr   Service Info received
 ** \param[in] EntryOptionInfoPtr Options of entry
 ** \param[in] EventgroupID     Eventgroup ID received
 ** \param[in] ReservedField    Reserved Field
 ** \param[in] RemoteNodeIndex  index of the remote node
 ** \param[in] SdInstanceIndex  index of network instance
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ServerService_eventStopSubscribe
(
  CONSTP2CONST(Sd_ServiceInfoType, AUTOMATIC, SD_APPL_DATA) ServiceInfoPtr,
  CONSTP2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfoPtr,
  const uint16 ReservedField,
  const uint16 EventgroupID,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  const uint8 SdInstanceIndex
);

/** \brief indicates a particular remote node has been reset.
 **
 ** \param[in] InstanceCfgPtr    Pointer to the configuration data of this instance
 ** \param[in] RemoteNodeIndex  index of the remote node
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ServerService_resetRemoteConnection
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  const Sd_RemoteNodeIdType RemoteNodeIndex
);

/** \brief Get actual state of ServerService state machine
 **
 ** \param[in] ServiceIndex    server service handle Id
 **
 ** \return state of server service
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(uint8, SD_CODE) Sd_ServerService_getComphase
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

#endif /* SD_SERVERSERVICE_INT_H_ */
