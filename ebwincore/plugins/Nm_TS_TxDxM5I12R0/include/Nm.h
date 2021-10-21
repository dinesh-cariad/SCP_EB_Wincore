#ifndef NM_H
#define NM_H

/**
 * \file
 *
 * \brief AUTOSAR Nm
 *
 * This file contains the implementation of the AUTOSAR
 * module Nm.
 *
 * \version 5.12.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

/* !LINKSTO Nm123_NmStack_Types,1 */
#include <NmStack_Types.h>  /* Nm stack specific types, includes StdTypes.h */

#include <Nm_Version.h>     /* Module's version declaration */
#include <Nm_Cfg.h>         /* Module configuration */

/*==================[macros]================================================*/
#if (defined NM_CC_STATE_WAIT_FOR_SHUTDOWN)
#error NM_CC_STATE_WAIT_FOR_SHUTDOWN already defined
#endif
/** \brief Wait for the shutdown conditions to become true.  This is checked in all
 * relevant Nm functions syncronously. */
#define NM_CC_STATE_WAIT_FOR_SHUTDOWN 0U

#if (defined NM_CC_STATE_WAIT_SYNCHRONIZATION)
#error NM_CC_STATE_WAIT_SYNCHRONIZATION already defined
#endif
/** \brief Wait for synchronization if a synchronizing network is part of the cluster */
#define NM_CC_STATE_WAIT_SYNCHRONIZATION 1U

#if (defined NM_CC_STATE_PREPARE_SHUTDOWN)
#error NM_CC_STATE_PREPARE_SHUTDOWN already defined
#endif
/** \brief Timers are initialized for the shutdown */
#define NM_CC_STATE_PREPARE_SHUTDOWN 2U

#if (defined NM_CC_STATE_SHUTDOWN_INITIATED)
#error NM_CC_STATE_SHUTDOWN_INITIATED already defined
#endif
/** \brief The shutdown sequence is currently running */
#define NM_CC_STATE_SHUTDOWN_INITIATED 3U

#if (defined NM_CC_STATE_SHUTDOWN_ABORTED)
#error NM_CC_STATE_SHUTDOWN_ABORTED already defined
#endif
/** \brief A shudown was aborted and the conditions are rechecked in the next main
 * function call. */
#define NM_CC_STATE_SHUTDOWN_ABORTED 4U

/*------------------[Development Error Codes]-------------------------------*/

#if (defined NM_E_UNINIT)
#error NM_E_UNINIT already defined
#endif
/** \brief Development Error Code
 **
 ** This error is reported if an API function is called prior to
 ** initialization of the Nm module. */
#define NM_E_UNINIT    0U


#if (defined NM_E_HANDLE_UNDEF)
#error NM_E_HANDLE_UNDEF already defined
#endif
/** \brief Development Error Code
 **
 ** This error is reported if an API function is called with invalid
 ** parameters; e.g. an invalid channel ID */
#define NM_E_HANDLE_UNDEF 1U

#if (defined NM_E_PARAM_POINTER)
#error NM_E_PARAM_POINTER already defined
#endif
/** \brief Development Error Code
 **
 ** This error is reported if an API function is called with NULL
 ** pointer; e.g. NULL_PTR */
#define NM_E_PARAM_POINTER 2U

#if (defined NM_E_PASSIVESTARTUP)
#error NM_E_PASSIVESTARTUP already defined
#endif
/** \brief  Error id reported in case SchM call fails for PassiveStartup */
#define NM_E_PASSIVESTARTUP       250U

#if (defined NM_E_NETWORKREQUEST)
#error NM_E_NETWORKREQUEST already defined
#endif
/** \brief  Error id reported in case SchM call fails for NetworkRequest */
#define NM_E_NETWORKREQUEST       249U

#if (defined NM_E_NETWORKRELEASE)
#error NM_E_NETWORKRELEASE already defined
#endif
/** \brief  Error id reported in case SchM call fails for NetworkRelease */
#define NM_E_NETWORKRELEASE       248U

#if (defined NM_E_REQUESTBUSSYNCHRONIZATION)
#error NM_E_REQUESTBUSSYNCHRONIZATION already defined
#endif
/** \brief  Error id reported in case SchM call fails for RequestBusSynchronization */
#define NM_E_REQUESTBUSSYNCHRONIZATION       247U

#if (defined NM_E_NETWORKGWERAREQUEST)
#error NM_E_NETWORKGWERAREQUEST already defined
#endif
/** \brief  Error id reported in case SchM call fails for NetworkGwEraRequest */
#define NM_E_NETWORKGWERAREQUEST       251U
/*------------------[function like macros]----------------------------------*/

/* if DEV_ERROR_DETECT is enabled only real C functions are provided.
 * Otherwise we map functions to generated macros. */
#if (NM_DEV_ERROR_DETECT == STD_OFF)

#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_OFF)
/** \brief define Nm_init()
 **
 ** Expand Nm_init() to nothing if the Nm is implemented as pure macro layer */
#if (defined Nm_Init)
#error Nm_Init already defined
#endif
#define Nm_Init()
#endif

/* Mapping of Network Management Interface API calls
 *
 * The set of macros map the Network Management Interface API to the API of
 * the corresponding bus specific NM layer (CanNm or FrNm or UdpNm) */

#if (NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
/** \brief Map Nm_CheckRemoteSleepIndication() to corresponding
 **        BusNm_CheckRemoteSleepIndication()
 **
 ** \param[in]  nmNetworkHandle Identification of the NM-channel.
 ** \param[out] nmRemoteSleepIndPtr Pointer where check result of remote
 **             sleep indication shall be copied to. */

#if (defined Nm_CheckRemoteSleepIndication)
#error Nm_CheckRemoteSleepIndication already defined
#endif

#define Nm_CheckRemoteSleepIndication(nmNetworkHandle, nmRemoteSleepIndPtr) \
  NM_BUSNM_CHECK_REMOTE_SLEEP_INDICATION(                                   \
    (nmNetworkHandle), (nmRemoteSleepIndPtr))
#endif

#if (NM_NODE_ID_ENABLED == STD_ON)
/** \brief Map Nm_GetLocalNodeIdentifier() to corresponding BusNm_GetLocalNodeIdentifier()
 **
 ** \param[in]  NetworkHandle Identification of the NM-channel.
 ** \param[out] nmNodeIdPtr Pointer where node identifier of the local node
 **             shall be copied to. */
#if (defined Nm_GetLocalNodeIdentifier)
#error Nm_GetLocalNodeIdentifier already defined
#endif

#define Nm_GetLocalNodeIdentifier(NetworkHandle, nmNodeIdPtr) \
  NM_BUSNM_GET_LOCAL_NODE_IDENTIFIER((NetworkHandle), (nmNodeIdPtr))

/** \brief Map Nm_GetNodeIdentifier() to corresponding BusNm_GetNodeIdentifier()
 **
 ** \param[in]  NetworkHandle Identification of the NM-channel.
 ** \param[out] nmNodeIdPtr Pointer where node identifier out of the
 **             last successfully received NM-message shall be copied to. */
#if (defined Nm_GetNodeIdentifier)
#error Nm_GetNodeIdentifier already defined
#endif

#define Nm_GetNodeIdentifier(NetworkHandle, nmNodeIdPtr) \
  NM_BUSNM_GET_NODE_IDENTIFIER((NetworkHandle), (nmNodeIdPtr))
#endif

#if (NM_NODE_DETECTION_ENABLED == STD_ON)
/** \brief Map Nm_RepeatMessageRequest() to corresponding BusNm_RepeatMessageRequest()
 **
 ** \param[in] NetworkHandle Identification of the NM-channel. */

#if (defined Nm_RepeatMessageRequest)
#error Nm_RepeatMessageRequest already defined
#endif

#define Nm_RepeatMessageRequest(NetworkHandle) \
  NM_BUSNM_REPEAT_MESSAGE_REQUEST(NetworkHandle)
#endif

#if (((NM_NODE_ID_ENABLED == STD_ON)         \
    || (NM_NODE_DETECTION_ENABLED == STD_ON) \
    || (NM_USER_DATA_ENABLED == STD_ON)))
/** \brief Map Nm_GetPduData() to corresponding BusNm_GetPduData()
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 ** \param[in] nmPduData Pointer where NM PDU shall be copied to. */
#if (defined Nm_GetPduData)
#error Nm_GetPduData already defined
#endif
#define Nm_GetPduData(NetworkHandle, nmPduData) \
  NM_BUSNM_GET_PDU_DATA((NetworkHandle), (nmPduData))
#endif

/** \brief Map Nm_GetState() to corresponding BusNm_GetState()
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel.
 ** \param[in] nmStatePtr Pointer where state of the network management shall
 **            be copied to.
 ** \param[in] nmModePtr Pointer to the location where the mode of the network
 **            management shall be copied to. */
#if (defined Nm_GetState)
#error Nm_GetState already defined
#endif
#define Nm_GetState(nmNetworkHandle, nmStatePtr, nmModePtr) \
  NM_BUSNM_GET_STATE((nmNetworkHandle), (nmStatePtr), (nmModePtr))

#if (NM_USER_DATA_ENABLED == STD_ON)
/** \brief Map Nm_GetUserData() to corresponding BusNm_GetUserData()
 **
 ** \param[in]  NetworkHandle Channel Identification of the NM-channel.
 ** \param[out] nmUserDataPtr Pointer to a memory area where the extracted
 **             data shall be written to. */
#if (defined Nm_GetUserData)
#error Nm_GetUserData already defined
#endif
#define Nm_GetUserData(NetworkHandle, nmUserDataPtr) \
  (NM_BUSNM_GET_USER_DATA((NetworkHandle), (nmUserDataPtr)))
#endif

#if ((NM_USER_DATA_ENABLED == STD_ON)      \
  && (NM_COM_USER_DATA_ENABLED == STD_OFF) \
  && (NM_PASSIVE_MODE_ENABLED == STD_OFF))
/** \brief Map Nm_SetUserData() to corresponding BusNm_SetUserData()
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 ** \param[in] nmUserDataPtr User data for the next transmitted NM message. */
#if (defined Nm_SetUserData)
#error Nm_SetUserData already defined
#endif
#define Nm_SetUserData(NetworkHandle, nmUserDataPtr) \
  NM_BUSNM_SET_USER_DATA((NetworkHandle), (nmUserDataPtr))
#endif

#if ((NM_PASSIVE_MODE_ENABLED == STD_OFF) \
  && (NM_COORDINATOR_SUPPORT_ENABLED == STD_OFF))
/** \brief Map Nm_NetworkRequest() to corresponding BusNm_NetworkRequest()
 **
 ** \param[in] nmNetworkHandle NM channel for which the network shall be
 **            requested. */
#if (defined Nm_NetworkRequest)
#error Nm_NetworkRequest already defined
#endif
#define Nm_NetworkRequest(nmNetworkHandle) \
  NM_BUSNM_NETWORK_REQUEST(nmNetworkHandle)

/** \brief Map Nm_NetworkRelease() to corresponding BusNm_NetworkRelease()
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel. */
#if (defined Nm_NetworkRelease)
#error Nm_NetworkRelease already defined
#endif
#define Nm_NetworkRelease(nmNetworkHandle) \
  NM_BUSNM_NETWORK_RELEASE(nmNetworkHandle)
#endif

#if ((NM_PASSIVE_MODE_ENABLED == STD_OFF) && \
     (EB_NM_STACK == STD_ON))
/** \brief Map Nm_NetworkGwEraRequest() to corresponding BusNm_NetworkGwEraRequest()
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel. */
#if (defined Nm_NetworkGwEraRequest)
#error Nm_NetworkGwEraRequest already defined
#endif
#define Nm_NetworkGwEraRequest(nmNetworkHandle) \
  NM_BUSNM_NETWORK_GW_ERA_REQUEST(nmNetworkHandle)

#endif /* ((NM_PASSIVE_MODE_ENABLED == STD_OFF) && \
        *  (EB_NM_STACK == STD_ON))
        */
#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_OFF)
/** \brief Map Nm_PassiveStartUp() to corresponding BusNm_PassiveStartUp()
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel. */
#if (defined Nm_PassiveStartUp)
#error Nm_PassiveStartUp already defined
#endif
#define Nm_PassiveStartUp(nmNetworkHandle) \
  NM_BUSNM_PASSIVE_START_UP(nmNetworkHandle)
#endif

#if (NM_COM_CONTROL_ENABLED == STD_ON)
/** \brief Map Nm_DisableCommunication() to corresponding BusNm_DisableCommunication()
 **
 ** \param[in] NetworkHandle Identification of the NM-channel. */
#if (defined Nm_DisableCommunication)
#error Nm_DisableCommunication already defined
#endif
#define Nm_DisableCommunication(NetworkHandle) \
  NM_BUSNM_DISABLE_COMMUNICATION(NetworkHandle)

/** \brief Map Nm_EnableCommunication() to corresponding BusNm_EnableCommunication()
 **
 ** \param[in] NetworkHandle Identification of the NM-channel. */
#if (defined Nm_EnableCommunication)
#error Nm_EnableCommunication already defined
#endif
#define Nm_EnableCommunication(NetworkHandle) \
  NM_BUSNM_ENABLE_COMMUNICATION(NetworkHandle)
#endif

#endif  /* NM_DEV_ERROR_DETECT == STD_OFF */

/*==================[type definitions]======================================*/

#if (NM_BUSNM_NUM > 1U)

/** \brief Function Pointer for standard Nm functions with return type
 ** Std_ReturnType and parameter of NetworkHandleType. */
typedef P2FUNC(Std_ReturnType, NM_APPL_CODE, Nm_TS_BusNm_StdFPtrType)
(
  NetworkHandleType nmChannelHandle
);

/** \brief Function pointer for Nm callback functions with return type void
 ** and parameter of NetworkHandleType. */
typedef P2FUNC(void, NM_APPL_CODE, Nm_TS_BusNm_StdCbkFPtrType)
(
  NetworkHandleType nmChannelHandle
);

/** \brief Function Pointer for Nm functions with return type Std_ReturnType
 ** and 2 parameters as for BusNm_GetUserData() . */
typedef P2FUNC(Std_ReturnType, NM_APPL_CODE, Nm_TS_BusNm_GetInfoFPtrType)
(
  NetworkHandleType                     nmChannelHandle,
  P2VAR(uint8, AUTOMATIC, NM_APPL_DATA) nmPduDataPtr
);

/** \brief Function Pointer for Nm functions with return type Std_ReturnType
 ** and 2 parameters as for BusNm_CheckRemoteSleepIndication() . */
typedef P2FUNC(
  Std_ReturnType, NM_APPL_CODE, Nm_TS_BusNm_ChkRemoteSleepIndFPtrType)
(
  NetworkHandleType                       nmChannelHandle,
  P2VAR(boolean, AUTOMATIC, NM_APPL_DATA) nmRemoteSleepIndPtr
);

/** \brief Function Pointer for Nm functions with return type Std_ReturnType
 ** and 2 parameters as for BusNm_SetUserData() . */
typedef P2FUNC(Std_ReturnType, NM_APPL_CODE, Nm_TS_BusNm_SetUserDataFPtrType)
(
  NetworkHandleType                        nmChannelHandle,
  P2CONST(uint8, AUTOMATIC, NM_APPL_DATA) nmPduDataPtr
);

/** \brief Function Pointer for Nm functions with return type Std_ReturnType
 ** and 3 parameters as for BusNm_GetState() . */
typedef P2FUNC(Std_ReturnType, NM_APPL_CODE, Nm_TS_BusNm_GetStateFPtrType)
(
  NetworkHandleType                            nmChannelHandle,
  P2VAR(Nm_StateType, AUTOMATIC, NM_APPL_DATA) nmStatePtr,
  P2VAR(Nm_ModeType, AUTOMATIC, NM_APPL_DATA)  nmModePtr
);

/** \brief Type holding functions pointers to BusNm functions
 *
 * This type is only used internally in the Nm but must be published in the
 * public module API to enable optimisations. */
typedef struct
{
#if (NM_MULTICORE_ENABLED == STD_OFF)
  /** \brief Points to the BusNm_PassiveStartup() function of the lower layer
   * modules */
  Nm_TS_BusNm_StdFPtrType         PassiveStartUp;
#endif
  /** \brief Points to the BusNm_GetState() function of the lower layer
   * modules */
  Nm_TS_BusNm_GetStateFPtrType    GetState;
#if (NM_MULTICORE_ENABLED == STD_OFF)
#if (NM_PASSIVE_MODE_ENABLED == STD_OFF)
  /** \brief Points to the BusNm_NetworkRequest() function of the lower layer
   * modules */
  Nm_TS_BusNm_StdFPtrType         NetworkRequest;
  /** \brief Points to the BusNm_NetworkRelease() function of the lower layer
   * modules */
  Nm_TS_BusNm_StdFPtrType         NetworkRelease;
#if (EB_NM_STACK == STD_ON)
  /** \brief Points to the BusNm_NetworkGwEraRequest() function of the lower layer
   * modules */
  Nm_TS_BusNm_StdFPtrType         NetworkGwEraRequest;
#endif /* (EB_NM_STACK == STD_ON) */
#endif
#endif
#if (NM_USER_DATA_ENABLED == STD_ON)
  /** \brief Points to the BusNm_GetUserData() function of the lower layer
   * modules */
  Nm_TS_BusNm_GetInfoFPtrType     GetUserData;
#if ((NM_PASSIVE_MODE_ENABLED == STD_OFF) \
   && (NM_COM_USER_DATA_ENABLED == STD_OFF))
  /** \brief Points to the BusNm_SetState() function of the lower layer
   * modules */
  Nm_TS_BusNm_SetUserDataFPtrType SetUserData;
#endif
#endif
#if (NM_NODE_ID_ENABLED == STD_ON)
  /** \brief Points to the BusNm_GetNodeIdentifier() function of the lower
   * layer modules */
  Nm_TS_BusNm_GetInfoFPtrType     GetNodeIdentifier;
  /** \brief Points to the BusNm_GetLocalNodeIdentifier() function of the
   * lower layer modules */
  Nm_TS_BusNm_GetInfoFPtrType     GetLocalNodeIdentifier;
#endif
#if (NM_NODE_DETECTION_ENABLED == STD_ON)
  /** \brief Points to the BusNm_RepeatMessageRequest() function of the lower
   * layer modules */
  Nm_TS_BusNm_StdFPtrType         RepeatMessageRequest;
#endif
#if ((NM_NODE_ID_ENABLED == STD_ON)             \
  || (NM_NODE_DETECTION_ENABLED == STD_ON)      \
  || (NM_USER_DATA_ENABLED == STD_ON))
  /** \brief Points to the BusNm_GetPduData() function of the lower layer
   * modules */
  Nm_TS_BusNm_GetInfoFPtrType     GetPduData;
#endif
#if (NM_BUS_SYNCHRONIZATION_ENABLED == STD_ON)
  /** \brief Points to the BusNm_RequestBusSynchronization() function of the
   * lower layer modules */
  Nm_TS_BusNm_StdFPtrType         RequestBusSynchronization;
#endif
#if (NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
  /** \brief Points to the BusNm_CheckRemoteSleepIndication() function of the
   * lower layer modules */
  Nm_TS_BusNm_ChkRemoteSleepIndFPtrType CheckRemoteSleepIndication;
#endif
#if (NM_COM_CONTROL_ENABLED == STD_ON)
  /** \brief Points to the BusNm_DisableCommunication() function of the lower
   * layer modules */
  Nm_TS_BusNm_StdFPtrType         DisableCommunication;
  /** \brief Points to the BusNm_EnableCommunication() function of the lower
   * layer modules */
  Nm_TS_BusNm_StdFPtrType         EnableCommunication;
#endif
} Nm_BusNmFpType;

#endif  /* NM_BUSNM_NUM > 1U */

/*==================[external function declarations]========================*/

#define NM_START_SEC_CODE
#include <Nm_MemMap.h>

#if ((NM_DEV_ERROR_DETECT == STD_ON)                    \
  || (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON))

/** \brief Initializes the NM Interface.
 **
 ** This function initializes the Nm Interface (Nm).
 **
 ** \ServiceID{0}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, NM_CODE) Nm_Init
(
  void
);

/** \brief Passive start of Network Management.
 **
 ** This function calls the BusNm_PassiveStartUp function (e.g.
 ** CanNm_PassiveStartUp function is called if channel is configured as CAN).
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should have been
 **    initialized for this channel (checked).
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Passive start of network management has failed.
 ** NetworkHandle does not exist (development only) module not yet initialized
 ** development only).
 **
 ** \ServiceID{1}
 ** \Reentrancy{Non-Reentrant for the same NetworkHandle. Reentrant otherwise}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NM_CODE) Nm_PassiveStartUp
(
   NetworkHandleType NetworkHandle
);

#endif /* ((NM_DEV_ERROR_DETECT == STD_ON)
          || (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)) */


#if ((NM_PASSIVE_MODE_ENABLED == STD_OFF)               \
  && ((NM_DEV_ERROR_DETECT == STD_ON)                   \
    || (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)))

/** \brief Requests bus communication.
 **
 ** This function calls the function BusNm_NetworkRequest() (e.g.
 ** CanNm_NetworkRequest() function is called if channel is configured as
 ** CAN).
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should have been
 **   initialized for this channel (checked).
 **
 ** \param[in] NetworkHandle NM channel for which the network shall be
 ** requested.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Requesting of bus communication has failed.
 ** NetworkHandle does not exist (development only). The module is not yet
 ** initialized (development only).
 **
 ** \ServiceID{2}
 ** \Reentrancy{Non-Reentrant for the same NetworkHandle. Reentrant otherwise}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NM_CODE) Nm_NetworkRequest
(
   NetworkHandleType NetworkHandle
);

/** \brief Releases the Bus communication.
 **
 ** This function calls the BusNm_NetworkRelease() bus specific function
 ** (e.g. CanNm_NetworkRelease() function is called if channel is configured
 ** as CAN).
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should have been
 **   initialized for this channel (checked).
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Releasing of bus communication has failed.
 ** NetworkHandle does not exist (development only). The module is not yet
 ** initialized (development only).
 **
 ** \ServiceID{3}
 ** \Reentrancy{Non-Reentrant for the same NetworkHandle. Reentrant otherwise}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NM_CODE) Nm_NetworkRelease
(
   NetworkHandleType NetworkHandle
);

#endif /* ((NM_PASSIVE_MODE_ENABLED == STD_OFF)
          && ((NM_DEV_ERROR_DETECT == STD_ON)
          || (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON))) */

#if ((NM_PASSIVE_MODE_ENABLED == STD_OFF) && \
     (NM_DEV_ERROR_DETECT == STD_ON) && \
     (EB_NM_STACK == STD_ON))
/** \brief Requets the Bus communication.
 **
 ** This function calls the BusNm_NetworkGwEraRequest() bus specific function
 ** (e.g. CanNm_NetworkGwEraRequest() function is called if channel is configured
 ** as CAN).
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should have been
 **   initialized for this channel (checked).
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Requesting of bus communication has failed.
 ** NetworkHandle does not exist (development only). The module is not yet
 ** initialized (development only).
 **
 ** \ServiceID{254}
 ** \Reentrancy{Non-Reentrant for the same NetworkHandle. Reentrant otherwise}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NM_CODE) Nm_NetworkGwEraRequest
(
   NetworkHandleType NetworkHandle
);
#endif /* ((NM_PASSIVE_MODE_ENABLED == STD_OFF) && \
        * (NM_DEV_ERROR_DETECT == STD_ON) && \
        * (EB_NM_STACK == STD_ON))
        */
/*------------------[API for controling communication]----------------------*/

#if ((NM_COM_CONTROL_ENABLED == STD_ON)         \
  && (NM_DEV_ERROR_DETECT == STD_ON))

/** \brief Disable NM-PDU transmissions.
 **
 ** This function calls the respective function at CanNm module, to disable
 ** the NM PDU transmission ability due to a ISO14229 Communication Control
 ** (28hex) service.
 **
 ** This functionality is not provided by FrNm module.
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should have been
 **   initialized for this channel (checked).
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Disabling of NM PDU transmission ability has
 ** failed. NetworkHandle does not exist (development only).  The module is
 ** not yet initialized (development only).
 **
 ** \ServiceID{4}
 ** \Reentrancy{Non-Reentrant for the same NetworkHandle. Reentrant otherwise}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NM_CODE) Nm_DisableCommunication
(
   NetworkHandleType NetworkHandle
);

/** \brief  Enable NM-PDU transmissions.
 **
 ** This function calls the respective function at CanNm module to enable the
 ** NM PDU transmission ability due to a ISO14229 Communication Control
 ** (28hex) service.
 **
 ** This functionality is not provided by FrNm module.
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should have been
 **   initialized for this channel (checked).
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Enabling of NM PDU transmission ability has
 ** failed. NetworkHandle does not exist (development only).  The module is
 ** not yet initialized (development only).
 **
 ** \ServiceID{5}
 ** \Reentrancy{Non-Reentrant for the same NetworkHandle. Reentrant otherwise}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NM_CODE) Nm_EnableCommunication
(
   NetworkHandleType NetworkHandle
);

#endif /* NM_COM_CONTROL_ENABLED == STD_ON && NM_DEV_ERROR_DETECT == STD_ON */


/*------------------[API getting/setting user data]-------------------------*/

#if ((NM_USER_DATA_ENABLED == STD_ON)     \
  && (NM_PASSIVE_MODE_ENABLED == STD_OFF) \
  && (NM_COM_USER_DATA_ENABLED == STD_OFF) \
  && (NM_DEV_ERROR_DETECT == STD_ON))

/** \brief  Set user data.
 **
 ** This function sets the user data for NM messages transmitted next on
 ** the bus. For that purpose BusNm_SetUserData shall be called (e.g.
 ** CanNm_SetUserData function is called if channel is configured as CAN).
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should
 **    have been initialized for this channel (checked).
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 ** \param[in] nmUserDataPtr User data for the next transmitted NM message.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Setting of user data has failed. NetworkHandle does
 ** not exist (development only). The module is not yet initialized
 ** (development only).
 **
 ** \ServiceID{6}
 ** \Reentrancy{Non-Reentrant for the same NetworkHandle. Reentrant otherwise}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NM_CODE) Nm_SetUserData
(
   NetworkHandleType                       NetworkHandle,
   P2CONST(uint8, AUTOMATIC, NM_APPL_DATA) nmUserDataPtr
);

#endif /* ((NM_USER_DATA_ENABLED == STD_ON)
          && (NM_PASSIVE_MODE_ENABLED == STD_OFF)
          && (NM_COM_USER_DATA_ENABLED == STD_OFF)
          && (NM_DEV_ERROR_DETECT == STD_ON)) */


#if ((NM_USER_DATA_ENABLED == STD_ON)           \
  && (NM_DEV_ERROR_DETECT == STD_ON))

/** \brief Get user data out of the last received NM message.
 **
 ** This function gets user data out of the last successfully received NM
 ** message.  For that purpose BusNm_GetUserData shall be called (e.g.
 ** CanNm_GetUserData function is called if channel is configured as CAN).
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should have been
 **   initialized for this channel (checked).
 **
 ** \param[in] NetworkHandle Channel Identification of the NM-channel.
 ** \param[out] nmUserDataPtr Pointer to a memory area where the extracted
 ** data shall be written to.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Getting of user data has failed. NetworkHandle does
 ** not exist (development only). Module not yet initialized (development
 ** only).
 **
 ** \ServiceID{7}
 ** \Reentrancy{Non-Reentrant for the same NetworkHandle. Reentrant otherwise}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NM_CODE) Nm_GetUserData
(
   NetworkHandleType                     NetworkHandle,
   P2VAR(uint8, AUTOMATIC, NM_APPL_DATA) nmUserDataPtr
);

#endif /* ((NM_USER_DATA_ENABLED == STD_ON)
          && (NM_DEV_ERROR_DETECT == STD_ON)) */

/*------------------[API for getting Node IDs]------------------------------*/

#if (((NM_NODE_ID_ENABLED == STD_ON)            \
    || (NM_NODE_DETECTION_ENABLED == STD_ON)    \
    || (NM_USER_DATA_ENABLED == STD_ON))        \
  && (NM_DEV_ERROR_DETECT == STD_ON))

/** \brief Get Pdu Data.
 **
 ** This function gets the whole PDU data out of the most recently received
 ** NM message.
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should have been
 **   initialized for this channel (checked).
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 ** \param[in] nmPduData Pointer where NM PDU shall be copied to.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Getting of NM PDU data has failed. NetworkHandle does
 ** not exist (development only) module not yet initialized (development
 ** only).
 **
 ** \ServiceID{8}
 ** \Reentrancy{Non-Reentrant for the same NetworkHandle. Reentrant otherwise}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NM_CODE) Nm_GetPduData
(
   NetworkHandleType                     NetworkHandle,
   P2VAR(uint8, AUTOMATIC, NM_APPL_DATA) nmPduData
);

#endif /* (((NM_NODE_ID_ENABLED == STD_ON)
          || (NM_NODE_DETECTION_ENABLED == STD_ON)
          || (NM_USER_DATA_ENABLED == STD_ON))
          && (NM_DEV_ERROR_DETECT == STD_ON)) */


/*------------------[API Node Detection]------------------------------------*/

#if ((NM_NODE_DETECTION_ENABLED == STD_ON)      \
  && (NM_DEV_ERROR_DETECT == STD_ON))

/** \brief Set Repeat Message Request bit.
 **
 ** This function sets the Repeat Message Request Bit for NM messages
 ** transmitted next on the bus. For that purpose
 ** BusNm_RepeatMessageRequest() shall be called
 ** (e.g. CanNm_RepeatMessageRequest() function is called if channel is
 ** configured as CAN)
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should have been
 **   initialized for this channel (checked).
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 **
 ** \return Standard Return Code
 ** \retval E_OK No error.
 ** \retval E_NOT_OK Setting of Repeat Message Request Bit has
 ** failed. NetworkHandle does not exist (development only).  Module not yet
 ** initialized (development only).
 **
 ** \ServiceID{9}
 ** \Reentrancy{Non-Reentrant for the same NetworkHandle. Reentrant otherwise}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NM_CODE) Nm_RepeatMessageRequest
(
   NetworkHandleType NetworkHandle
);

#endif /* ((NM_NODE_DETECTION_ENABLED == STD_ON)
          && (NM_DEV_ERROR_DETECT == STD_ON)) */

/*------------------[API for getting Node IDs]------------------------------*/

#if ((NM_NODE_ID_ENABLED == STD_ON)  \
  && (NM_DEV_ERROR_DETECT == STD_ON))

/** \brief Get the Node Identifier.
 **
 ** This function gets the node identifier out of the last successfully
 ** received NM-message.
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should
 **    have been initialized for this channel (checked).
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 ** \param[out] nmNodeIdPtr Pointer where node identifier out of the
 ** last successfully received NM-message shall be copied to.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Getting of the node identifier out of the last
 ** received NM-message has failed. NetworkHandle does not exist (development
 ** only). Module not yet initialized (development only).
 **
 ** \ServiceID{10}
 ** \Reentrancy{Non-Reentrant for the same NetworkHandle. Reentrant otherwise}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NM_CODE) Nm_GetNodeIdentifier
(
   NetworkHandleType                     NetworkHandle,
   P2VAR(uint8, AUTOMATIC, NM_APPL_DATA) nmNodeIdPtr
);

/** \brief Configure the get node identifier for the local node.
 **
 ** The function configures the Get node identifier for the local node. For
 ** that purpose BusNm_GetLocalNodeIdentifier shall be called (e.g.
 ** CanNm_GetLocalNodeIdentifier function is called if channel is configured
 ** as CAN).
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should have been
 **   initialized for this channel (checked).
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 ** \param[out] nmNodeIdPtr Pointer where node identifier of the local node
 ** shall be copied to.
 **
 ** \return Standard Return Code
 ** \retval E_OK No error.
 ** \retval E_NOT_OK Getting of the node identifier of the local node has
 ** failed. NetworkHandle does not exist (development only).  Module not yet
 ** initialized (development only).
 **
 ** \ServiceID{11}
 ** \Reentrancy{Non-Reentrant for the same NetworkHandle. Reentrant
 ** otherwise.}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NM_CODE) Nm_GetLocalNodeIdentifier
(
   NetworkHandleType                     NetworkHandle,
   P2VAR(uint8, AUTOMATIC, NM_APPL_DATA) nmNodeIdPtr
);
#endif /* ((NM_NODE_ID_ENABLED == STD_ON)
          && (NM_DEV_ERROR_DETECT == STD_ON)) */


/*------------------[API Check Remote Sleep]--------------------------------*/

#if ((NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)    \
  && (NM_DEV_ERROR_DETECT == STD_ON))

/** \brief Implementation of Nm_CheckRemoteSleepIndication.
 **
 ** This function checks if remote sleep indication takes place or not. This
 ** in turn calls the BusNm_CheckRemoteSleepIndication() for the bus
 ** specific NM layer (e.g. CanNm_CheckRemoteSleepIndication() function is
 ** called if channel is configured as CAN).
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should
 **    have been initialized for this channel (checked).
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel.
 ** \param[out] nmRemoteSleepIndPtr Pointer where check result of remote
 ** sleep indication shall be copied to.
 **
 ** \return Standard Return Code
 ** \retval E_OK No error.
 ** \retval E_NOT_OK Checking of remote sleep indication bits has failed.
 ** NetworkHandle does not exist (development only). Module not yet
 ** initialized (development only).
 **
 ** \ServiceID{13}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NM_CODE) Nm_CheckRemoteSleepIndication
(
  NetworkHandleType                       nmNetworkHandle,
  P2VAR(boolean, AUTOMATIC, NM_APPL_DATA) nmRemoteSleepIndPtr
);

#endif /* ((NM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
          && (NM_DEV_ERROR_DETECT == STD_ON)) */

#if (NM_COORDINATOR_SUPPORT_ENABLED == STD_ON)
/** \brief Get the State of the Nm Coordinator.
 **
 ** This function returns the state of the Nm coordinator.
 **
 ** Preconditions:
 ** - The coordinator id should be valid and the module should have been
 **   initialized
 **
 ** \param[in] nmCoordinatorId Identification of the coordinator.
 ** \param[in] nmStatePtr Pointer where state of the Coordinator shall
 ** be copied to.
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Getting of NM state has failed.
 ** \ServiceID{30}
 ** \Reentrancy{Reentrant for the same NetworkHandle.}
 ** \Synchronicity{Asynchronous} */
extern FUNC(Std_ReturnType, NM_CODE) Nm_GetCoordinatorState
(
   uint8                                 nmCoordinatorId,
   P2VAR(uint8, AUTOMATIC, NM_APPL_DATA) nmStatePtr
);
#endif

#if (NM_DEV_ERROR_DETECT == STD_ON)

/** \brief Get the State of the network management module.
 **
 ** This function returns the state of the network management. This function
 ** in turn calls the BusNm_GetState function (e.g. CanNm_GetState function
 ** is called if channel is configured as CAN).
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should have been
 **   initialized for this channel (checked).
 **
 ** \param[in] nmNetworkHandle Identification of the NM-channel.
 ** \param[in] nmStatePtr Pointer where state of the network management shall
 ** be copied to.
 ** \param[in] nmModePtr Pointer to the location where the mode of the network
 ** management shall be copied to.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Getting of NM state has failed. NetworkHandle does not
 ** exist (development only). Module not yet initialized (development only).
 **
 ** \ServiceID{14}
 ** \Reentrancy{Non-Reentrant for the same NetworkHandle. Reentrant otherwise}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, NM_CODE) Nm_GetState
(
   NetworkHandleType                            nmNetworkHandle,
   P2VAR(Nm_StateType, AUTOMATIC, NM_APPL_DATA) nmStatePtr,
   P2VAR(Nm_ModeType, AUTOMATIC, NM_APPL_DATA)  nmModePtr
);

#endif /* NM_DEV_ERROR_DETECT == STD_ON */

#if (NM_VERSION_INFO_API == STD_ON)

/** \brief Gets version information for the NM module.
 **
 ** This service returns the version information of this module.
 ** The version information includes:
 ** - Module Id
 ** - Vendor Id
 ** - Vendor specific version numbers (BSW00407).
 **
 ** \param[out] Std_VersionInfoType nmVerInfoPtr Pointer to where to store
 **                  the version information of this module.
 ** \return Standard Return Code
 ** \retval void
 **
 ** \ServiceID{15}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, NM_CODE) Nm_GetVersionInfo(
   P2VAR(Std_VersionInfoType, AUTOMATIC, NM_APPL_DATA) nmVerInfoPtr);

#endif  /* NM_VERSION_INFO_API == STD_ON */

#ifndef RTE_SCHM_SWCBSW_NM_MAINFUNCTION
#define RTE_SCHM_SWCBSW_NM_MAINFUNCTION
/** \brief This function is supplied for the NM coordinator functionality
 **
 ** This function implements the processes of the NM Interface, which need a
 ** fix cyclic scheduling. This function is supplied for the NM coordinator
 ** functionality (Nm020). However, specific implementation may not need it
 ** (Nm093).
 **
 ** Preconditions:
 ** - NM Interface must be initialized before
 **
 ** \ServiceID{16} */
extern FUNC(void, NM_CODE) Nm_MainFunction
(
  void
);
#endif /* RTE_SCHM_SWCBSW_NM_MAINFUNCTION */

#define NM_STOP_SEC_CODE
#include <Nm_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#if (NM_BUSNM_NUM > 1U)

#define NM_START_SEC_CONST_UNSPECIFIED
#include <Nm_MemMap.h>

/** \brief Array holding function pointers to BusNm modules
 *
 * This array is only used internally in the Nm but must be published in the
 * public module API to enable optimisations. */
extern CONST(Nm_BusNmFpType, NM_CONST) Nm_BusNmFp[NM_BUSNM_NUM];

#define NM_STOP_SEC_CONST_UNSPECIFIED
#include <Nm_MemMap.h>

#endif

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
#endif /* ifndef NM_H */
