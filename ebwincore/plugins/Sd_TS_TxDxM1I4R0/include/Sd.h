#ifndef SD_H
#define SD_H

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

#include <Sd_Types.h>        /* Module public types */
#include <Sd_Cfg.h>
#include <Sd_Version.h>
#include <Sd_Types_ExtGen.h>

/*==================[macros]================================================*/

#if (defined SD_INSTANCE_ID)
#error SD_INSTANCE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define SD_INSTANCE_ID         0U

#if (defined SD_E_NOT_INITIALIZED)
#error SD_E_NOT_INITIALIZED is already defined
#endif
/** \brief API service called before initializing the module */
#define SD_E_NOT_INITIALIZED                        0x01U

#if (defined SD_E_PARAM_POINTER)
#error SD_E_PARAM_POINTER is already defined
#endif
/** \brief Null pointer has been passed as an argument */
#define SD_E_PARAM_POINTER                   0x02U

#if (defined SD_E_INVALID_ARG)
#error SD_E_INVALID_ARG is already defined
#endif
/** \brief Initialization failed error */
#define SD_E_INVALID_ARG                   0x03U

#if (defined SD_E_INV_ID)
#error SD_E_INV_ID is already defined
#endif
/** \brief API service called with invalid Id */
#define SD_E_INV_ID                   0x04U

#if (defined SD_E_INV_MODE)
#error SD_E_INV_MODE is already defined
#endif
/** \brief API service called with invalid mode request */
#define SD_E_INV_MODE                   0x05U

#if (defined SD_E_COUNT_OF_RETRY_SUBSCRIPTION_EXCEEDED)
#error SD_E_COUNT_OF_RETRY_SUBSCRIPTION_EXCEEDED is already defined
#endif
/** \brief Initialization failed error */
#define SD_E_COUNT_OF_RETRY_SUBSCRIPTION_EXCEEDED           0x06U

#if (defined SD_E_CLIENT_IMPOSSIBLE_STATE)
#error SD_E_CLIENT_IMPOSSIBLE_STATE is already defined
#endif
/** \brief Initialization failed error */
#define SD_E_CLIENT_IMPOSSIBLE_STATE       0x07U

#if (defined SD_E_OUT_OF_RES)
#error SD_E_OUT_OF_RES is already defined
#endif
/** \brief Initialization failed error */
#define SD_E_OUT_OF_RES             0x08U

#if (defined SD_CONTROL_SOCKET_NOT_RELEASED)
#error SD_CONTROL_SOCKET_NOT_RELEASED is already defined
#endif
/** \brief service called with invalid mode request */
#define SD_CONTROL_SOCKET_NOT_RELEASED     0x09U

#if (defined SD_CONTROL_SOCKET_IMPOSSIBLE)
#error SD_CONTROL_SOCKET_IMPOSSIBLE is already defined
#endif
/** \brief service called with invalid mode request */
#define SD_CONTROL_SOCKET_IMPOSSIBLE       0x0AU

#if (defined SD_CONNECTION_SETTING_FAILED)
#error SD_CONNECTION_SETTING_FAILED is already defined
#endif
/** \brief service called with invalid mode request */
#define SD_CONNECTION_SETTING_FAILED       0x0BU

#if (defined SD_E_IPADDR_ASSIGNMENT_NOT_FINISHED)
#error SD_E_IPADDR_ASSIGNMENT_NOT_FINISHED is already defined
#endif
/** \brief IP Address assignment not finished error */
#define SD_E_IPADDR_ASSIGNMENT_NOT_FINISHED 0x0CU

#if (defined SD_E_DIVISOR_IS_NEGATIVE)
#error SD_E_DIVISOR_IS_NEGATIVE is already defined
#endif
/** \brief Divisor is negative error */
#define SD_E_DIVISOR_IS_NEGATIVE           0x0DU

#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
#if (defined SD_E_RETRY_INFINITE_TTL_NOT_INFINITE)
#error SD_E_RETRY_INFINITE_TTL_NOT_INFINITE is already defined
#endif
/** \brief IP Address assignment not finished error */
#define SD_E_RETRY_INFINITE_TTL_NOT_INFINITE 0x0EU

#if (defined SD_E_RETRY_PROCESS_BIGGER_THAN_TTL)
#error SD_E_RETRY_PROCESS_BIGGER_THAN_TTL is already defined
#endif
/** \brief Divisor is negative error */
#define SD_E_RETRY_PROCESS_BIGGER_THAN_TTL  0x0FU
#endif

/** \brief Defines API id of function Sd_Init */
#define SD_INIT_SVCID                       0x01U

/** \brief Defines API id of function Sd_GetVersionInfo */
#define SD_GETVERSIONINFO_SVCID             0x02U

/** \brief Defines API id of function Sd_LocalIpAddrAssignmentChg */
#define SD_COMPAREADDRESS_SVCID             0x04U

/** \brief Defines API id of function Sd_LocalIpAddrAssignmentChg */
#define SD_LOCALIPADDRASSIGNMENTCHG_SVCID   0x05U

/** \brief Defines API id of function Sd_MainFunction */
#define SD_MAINFUNCTION_SVCID               0x06U

/** \brief Defines API id of function Sd_ServerServiceSetState */
#define SD_SERVERSERVICESETSTATE_SVCID      0x07U

/** \brief Defines API id of function Sd_ClientServiceSetState */
#define SD_CLIENTSERVICESETSTATE_SVCID      0x08U

/** \brief Defines API id of function Sd_ConsumedEventGroupSetState */
#define SD_CONSUMEDEVENTGROUPSETSTATE_SVCID 0x09U

/** \brief Defines API id of function Sd_Instance_MainFunction */
#define SD_INSTANCEMAINFUNCTION_SVCID      0x0AU

/** \brief Defines API id of function Sd_ClientService_ComSM_event_timeout */
#define SD_CLIENTSERVICE_COMSM_EVENT_TIMEOUT  0x0BU

/** \brief Defines API id of function Sd_ClientService_TTL_Run_Out */
#define SD_CLIENTSERVICE_TTL_RUN_OUT  0x0CU

/** \brief Defines API id of function Sd_ClientService_ComSM_event_start */
#define SD_CLIENTSERVICE_COMSMEVENT_START_SVCID 0x10U

/** \brief Defines API id of function Sd_ClientService_ComSM_event_stop */
#define SD_CLIENTSERVICE_COMSMEVENT_STOP_SVCID 0x11U

/** \brief Defines API id of function Sd_ClientService_ComSM_event_timeout */
#define SD_CLIENTSERVICE_COMSMEVENT_TIMEOUT_SVCID 0x12U

/** \brief Defines API id of function Sd_ClientService_ComSM_event_Offer */
#define SD_CLIENTSERVICE_COMSMEVENT_OFFER_SVCID 0x13U

/** \brief Defines API id of function Sd_ClientService_ComSM_event_Stopoffer */
#define SD_CLIENTSERVICE_COMSMEVENT_STOPOFFER_SVCID 0x14U

/** \brief Defines API id of function Sd_ClientService_ComSM_SoConModeCheck */
#define SD_CLIENTSERVICE_COMSM_SOCONMODECHECK_SVCID 0x15U

/** \brief Defines API id of function Sd_ClientService_mainTimersFunction */
#define SD_CLIENTSERVICEMAINTIMERS_SVCID         0x16U

/** \brief Defines API id of function Sd_ClientService_mainTTL */
#define SD_CLIENTSERVICEMAINTTL_SVCID           0x17U

/** \brief Defines API id of function Sd_ClientService_mainFunction */
#define SD_CLIENTSERVICEMAINFUNCTION_SVCID      0x18U

/** \brief Defines API id of function Sd_ClientService_mainFunction */
#define SD_CLIENTSERVICEGOTOAVAILABLE_SVCID     0x19U

/** \brief Defines API id of function Sd_ClientServices_init_after_halt */
#define SD_CLIENTSERVICESINITAFTERTHALT_SVCID      0x1AU

/** \brief Defines API id of function Sd_ClientService_CEvg_exit_AVAILABLE */
#define SD_CLIENTSERVICECEVGEXITAVAILABLE_SVCID    0x1BU

/** \brief Defines API id of function Sd_ClientService_eventOfferService */
#define SD_CLIENTSERVICE_EVENT_OFFER_SERVICE_SVCID    0x1CU

/** \brief Defines API id of function Sd_ClientService_ProcessOffers */
#define SD_CLIENTSERVICE_PROCESSOFFERS_SVCID      0x1DU

/** \brief Defines API id of function Sd_ClientService_consumedEventGroupSetState */
#define SD_CLIENTSERVICE_CONSUMEDEVENTGROUPSETSTATE_SVCID    0x1EU

/** \brief Defines API id of function Sd_ClientService_ComSM_event_stop */
#define SD_CLIENTSERVICE_COMSM_EVENT_STOP_SVCID    0x1FU

/** \brief Defines API id of function Sd_ServerService_ComSM_event_start */
#define SD_SERVERSERVICE_COMSMEVENT_START_SVCID    0x20U

/** \brief Defines API id of function Sd_ServerService_ComSM_event_stophalt */
#define SD_SERVERSERVICE_COMSMEVENT_STOPHALT_SVCID 0x21U

/** \brief Defines API id of function Sd_ServerService_ComSM_event_timeout */
#define SD_SERVERSERVICE_COMSMEVENT_TIMEOUT_SVCID 0x22U

/** \brief Defines API id of function Sd_ClientService_mainTimersFunction */
#define SD_SERVERSERVICEMAINTIMERS_SVCID       0x23U

/** \brief Defines API id of function Sd_ClientService_mainTTL */
#define SD_SERVERSERVICEHANDLETTL_SVCID        0x24U

/** \brief Defines API id of function Sd_ClientService_mainFunction */
#define SD_SERVERSERVICEMAINFUNCTION_SVCID      0x25U

/** \brief Defines API id of function Sd_ServerService_resetRemoteConnection */
#define SD_SERVERSERVICERESETREMOTECONNECTION_SVCID   0x26U

/** \brief Defines API id of function Sd_ServerService_eventStopSubscribe */
#define SD_SERVERSERVICEEVENTSTOPSUBSCRIBE_SVCID   0x27U

/** \brief Defines API id of function Sd_ServerService_addMulticastEventGroupSubscriber */
#define SD_SERVERSERVICEADDMULTEVENTGROUPSUBS_SVCID   0x28U

/** \brief Defines API id of function Sd_ServerService_addEventGroupSubscriber */
#define SD_SERVERSERVICEADDEVENTGROUPSUBS_SVCID   0x29U

/** \brief Defines API id of function Sd_ServerService_removeEventGroupSubscriber */
#define SD_SERVERSERVICEREMOVEEVENTGROUPSUBS_SVCID   0x2AU

/** \brief Defines API id of function Sd_RxIndication */
#define SD_RXINDICATION_SVCID                  0x42U

/** \brief Defines API id of function Sd_Send_queueOptions */
#define SD_SENDQUEUEOPTIONS_SVCID              0x43U

/** \brief Defines API id of function Sd_Send_checkBufferFillLevel */
#define SD_CHECKBUFFERFILLLEVEL_SVCID          0x44U

/** \brief Defines API id of function Sd_Send_flushBuffer */
#define SD_SEND_FLUSHBUFFER_SVCID              0x45U

/** \brief Defines API id of function Sd_Receive_getEntryOptions */
#define SD_RECEVIVEGETENTRYOPTIONS_SVCID       0x50U

/** \brief Defines API id of function Sd_Get_Random */
#define SD_GETRANDOM_SVCID                     0x60U

/** \brief Defines API id of function Sd_ResetRemoteNodeRxState */
#define SD_RESETREMOTENODERXSTATE_SVCID        0x61U

/** \brief Defines API id of function Sd_HandleResponseTimers */
#define SD_HANDLERESPONSETIMERS_SVCID          0x62U

/** \brief Definition of defensive programming SD_E_SHOULD_NOT_HAPPEN  */
#define SD_E_SHOULD_NOT_HAPPEN              0xffU

/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define SD_START_SEC_CODE
#include <Sd_MemMap.h>

#if (SD_VERSION_INFO_API == STD_ON)

/** \brief Get version information of the Sd module.
 **
 ** This service returns the version information of this module. The version
 ** information includes:
 **  - Module Id
 **  - Vendor Id
 **  - Vendor specific version numbers
 **
 ** \param[out] versioninfo Pointer to where to store the version
 **                         information of this module.
 **
 ** \ServiceID{0x0B}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, SD_CODE) Sd_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, SD_APPL_DATA) Sd_VersionInfo
);
#endif /* SOAD_VERSION_INFO_API == STD_ON */



/** \brief Checks compatibility of the post-build-time configuration
 *
 * This service checks the compatibility of the post-build-time configuration
 * against the source code.
 *
 * \param[in] voidConfigPtr Pointer to the configuration data of the Sd module.
 *
 * \return Result of compatibility check
 * \retval E_OK      Provided configuration is compatible.
 * \retval E_NOT_OK  Provided configuration is not compatible.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, SD_CODE) Sd_IsValidConfig
(
  P2CONST(void, AUTOMATIC, SD_APPL_CONST) voidConfigPtr
);



/* !LINKSTO Sd.ASR40.SWS_SD_00119,1 */
/** \brief Initializes the Sd module.
 **
 ** This service initializes Sd module internal variables.
 **
 ** \ServiceID{0x01}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, SD_CODE) Sd_Init
(
  P2CONST(Sd_ConfigType, AUTOMATIC, SD_APPL_CONST) ConfigPtr
);



/* !LINKSTO Sd.ASR40.SWS_SD_00496,1 */
/** \brief Sets the Server Service instance state.
 **
 ** This API function sets the Server Service instance state.
 **
 ** \param[in] ServiceServiceHandleId   ID to identify the Server Service instance.
 ** \param[in] ServerServiceState       The state the Server Service instance shall be set to.
 **
 ** \return Result of function call
 ** \retval E_OK      State accepted
 ** \retval E_NOT_OK  State not accepted
 **
 ** \ServiceID{0x07}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, SD_CODE) Sd_ServerServiceSetState
(
  uint16 ServiceServiceHandleId,
  Sd_ServerServiceSetStateType ServerServiceState
);



/* !LINKSTO Sd.ASR40.SWS_SD_00409,1 */
/** \brief Sets the Client Service instance state.
 **
 ** This API function sets the Client Service instance state.
 **
 ** \param[in] ClientServiceHandleId    ID to identify the Client Service instance.
 ** \param[in] ClientServiceState       The state the Client Service instance shall be set to.
 **
 ** \return Result of function call
 ** \retval E_OK      State accepted
 ** \retval E_NOT_OK  State not accepted
 **
 ** \ServiceID{0x08}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, SD_CODE) Sd_ClientServiceSetState
(
  uint16 ClientServiceHandleId,
  Sd_ClientServiceSetStateType ClientServiceState
);



/* !LINKSTO Sd.ASR40.SWS_SD_00560,1 */
/** \brief Sets the Client EventGroup state.
 **
 ** This API function sets the Client EventGroup state.
 **
 ** \param[in] ConsumedEventGroupHandleId ID to identify the Client EventGroup instance.
 ** \param[in] ConsumedEventGroupState    The state the Client EventGroup instance shall be set to.
 **
 ** \return Result of function call
 ** \retval E_OK      State accepted
 ** \retval E_NOT_OK  State not accepted
 **
 ** \ServiceID{0x09}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, SD_CODE) Sd_ConsumedEventGroupSetState
(
  uint16 ConsumedEventGroupHandleId,
  Sd_ConsumedEventGroupSetStateType ConsumedEventGroupState
);



/** \brief Main Function of module Sd.
 **
 ** This API triggers all peridoci actions to be performed by Sd.
 **
 ** \ServiceID{0x06}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, SD_CODE) Sd_MainFunction
(
  void
);

/** \brief Initialize the random number generator.
 **
 ** Initialize the random number generator for random delays.
 **
 ** @param Seed Start value for number generator
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, SD_CODE) Sd_StartRandom
(
  uint32 Seed
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

#endif /* ifndef SD_H */
/*==================[end of file]===========================================*/
