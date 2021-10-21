/**
 * \file
 *
 * \brief AUTOSAR CanNm
 *
 * This file contains the implementation of the AUTOSAR
 * module CanNm.
 *
 * \version 6.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CANNM_API_H
#define CANNM_API_H

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 8.5 (required)
 * An external object or function shall be declared in one and only one file.
 *
 * Reason:
 * The files SchM_CanNm.h and CanNm.h contain declarations of CanNm_MainFunction. This is due to
 * the issue reported in http://www.autosar.org/bugzilla/show_bug.cgi?id=53853 and should be fixed
 * in a future version of the Rte.
 */

/*==================[inclusions]============================================*/

/* !LINKSTO CANNM309,1 */
#include <NmStack_Types.h>       /* Nm specific types */
/* !LINKSTO CANNM021,1 */
#include <CanNm_Version.h>       /* Version information for CanNm-module  */

#include <CanNm_Cfg.h>
#include <CanNm_Types.h>

#if (CANNM_PBCFGM_SUPPORT_ENABLED == STD_ON)
#include <PbcfgM_Api.h>         /* Post build configuration manager */
#endif /* CANNM_PBCFGM_SUPPORT_ENABLED */

/*==================[macros]================================================*/

/* ------------------[ list of AUTOSAR API service IDs ]--------------------*/

/* There seems to be no binding advice how the service ID macro names should
 * be constructed */
#if (defined CANNM_SERVID_INIT)
#error CANNM_SERVID_INIT already defined
#endif /* if (defined CANNM_SERVID_INIT) */

/* !LINKSTO CANNM210,1 */
/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_INIT.
 */
#define CANNM_SERVID_INIT                               0x00U


#if (defined CANNM_SERVID_PASSIVESTARTUP)
#error CANNM_SERVID_PASSIVESTARTUP already defined
#endif /* if (defined CANNM_SERVID_PASSIVESTARTUP) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_PASSIVESTARTUP
 */
#define CANNM_SERVID_PASSIVESTARTUP                     0x01U


#if (defined CANNM_SERVID_NETWORKREQUEST)
#error CANNM_SERVID_NETWORKREQUEST already defined
#endif /* if (defined CANNM_SERVID_NETWORKREQUEST) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_NETWORKREQUEST.
 */
#define CANNM_SERVID_NETWORKREQUEST                     0x02U

#if (defined CANNM_SERVID_NETWORKGWERAREQUEST)
#error CANNM_SERVID_NETWORKGWERAREQUEST already defined
#endif /* if (defined CANNM_SERVID_NETWORKGWERAREQUEST) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_NETWORKGWERAREQUEST.
 */
#define CANNM_SERVID_NETWORKGWERAREQUEST                     0xFEU

#if (defined CANNM_SERVID_NETWORKRELEASE)
#error CANNM_SERVID_NETWORKRELEASE already defined
#endif /* if (defined CANNM_SERVID_NETWORKRELEASE) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_NETWORKRELEASE.
 */
#define CANNM_SERVID_NETWORKRELEASE                     0x03U


#if (defined CANNM_SERVID_SETUSERDATA)
#error CANNM_SERVID_SETUSERDATA already defined
#endif /* if (defined CANNM_SERVID_SETUSERDATA) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_SETUSERDATA.
 */
#define CANNM_SERVID_SETUSERDATA                        0x04U


#if (defined CANNM_SERVID_GETUSERDATA)
#error CANNM_SERVID_GETUSERDATA already defined
#endif /* if (defined CANNM_SERVID_GETUSERDATA) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_GETUSERDATA.
 */
#define CANNM_SERVID_GETUSERDATA                        0x05U


#if (defined CANNM_SERVID_GETNODEIDENTIFIER)
#error CANNM_SERVID_GETNODEIDENTIFIER already defined
#endif /* if (defined CANNM_SERVID_GETNODEIDENTIFIER) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_GETNODEIDENTIFIER.
 */
#define CANNM_SERVID_GETNODEIDENTIFIER                  0x06U


#if (defined CANNM_SERVID_GETLOCALNODEIDENTIFIER)
#error CANNM_SERVID_GETLOCALNODEIDENTIFIER already defined
#endif /* if (defined CANNM_SERVID_GETLOCALNODEIDENTIFIER) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_GETLOCALNODEIDENTIFIER.
 */
#define CANNM_SERVID_GETLOCALNODEIDENTIFIER             0x07U


#if (defined CANNM_SERVID_REPEATMESSAGEREQUEST)
#error CANNM_SERVID_REPEATMESSAGEREQUEST already defined
#endif /* if (defined CANNM_SERVID_REPEATMESSAGEREQUEST) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_REPEATMESSAGEREQUEST.
 */
#define CANNM_SERVID_REPEATMESSAGEREQUEST               0x08U

#if (defined CANNM_SERVID_GETPDUDATA)
#error CANNM_SERVID_GETPDUDATA already defined
#endif /* if (defined CANNM_SERVID_GETPDUDATA) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_GETPDUDATA.
 */
#define CANNM_SERVID_GETPDUDATA                         0x0AU

#if (defined CANNM_SERVID_GETSTATE)
#error CANNM_SERVID_GETSTATE already defined
#endif /* if (defined CANNM_SERVID_GETSTATE) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_GETSTATE.
 */
#define CANNM_SERVID_GETSTATE                           0x0BU

#if (defined CANNM_SERVID_DISABLECOMMUNICATION)
#error CANNM_SERVID_DISABLECOMMUNICATION already defined
#endif /* if (defined CANNM_SERVID_DISABLECOMMUNICATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_DISABLECOMMUNICATION.
 */
#define CANNM_SERVID_DISABLECOMMUNICATION               0x0CU

#if (defined CANNM_SERVID_ENABLECOMMUNICATION)
#error CANNM_SERVID_ENABLECOMMUNICATION already defined
#endif /* if (defined CANNM_SERVID_ENABLECOMMUNICATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_ENABLECOMMUNICATION.
 */
#define CANNM_SERVID_ENABLECOMMUNICATION                0x0DU

#if (defined CANNM_SERVID_TXCONFIRMATION)
#error CANNM_SERVID_TXCONFIRMATION already defined
#endif /* if (defined CANNM_SERVID_TXCONFIRMATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_TXCONFIRMATION.
 */
#define CANNM_SERVID_TXCONFIRMATION                     0x0FU

#if (defined CANNM_SERVID_RXINDICATION)
#error CANNM_SERVID_RXINDICATION already defined
#endif /* if (defined CANNM_SERVID_RXINDICATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_RXINDICATION.
 */
#define CANNM_SERVID_RXINDICATION                       0x10U

#if (defined CANNM_SERVID_MAINFUNCTION_X)
#error CANNM_SERVID_MAINFUNCTION_X already defined
#endif /* if (defined CANNM_SERVID_MAINFUNCTION_X) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_MAINFUNCTION_X.
 */
#define CANNM_SERVID_MAINFUNCTION_X                     0x13U

#if (defined CANNM_SERVID_CANNMTRANSMIT)
#error CANNM_SERVID_CANNMTRANSMIT already defined
#endif /* if (defined CANNM_SERVID_CANNMTRANSMIT) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_CANNMTRANSMIT.
 */
#define CANNM_SERVID_CANNMTRANSMIT                     0x14U

#if (defined CANNM_SERVID_REQUESTBUSSYNCHRONIZATION)
#error CANNM_SERVID_REQUESTBUSSYNCHRONIZATION already defined
#endif /* if (defined CANNM_SERVID_REQUESTBUSSYNCHRONIZATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_REQUESTBUSSYNCHRONIZATION.
 */
#define CANNM_SERVID_REQUESTBUSSYNCHRONIZATION          0xC0U

#if (defined CANNM_SERVID_CHECKREMOTESLEEPINDICATION)
#error CANNM_SERVID_CHECKREMOTESLEEPINDICATION already defined
#endif /* if (defined CANNM_SERVID_CHECKREMOTESLEEPINDICATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_CHECKREMOTESLEEPINDICATION.
 */
#define CANNM_SERVID_CHECKREMOTESLEEPINDICATION         0xD0U

#if (defined CANNM_SERVID_GETVERSIONINFO)
#error CANNM_SERVID_GETVERSIONINFO already defined
#endif /* if (defined CANNM_SERVID_GETVERSIONINFO) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_GETVERSIONINFO.
 */
#define CANNM_SERVID_GETVERSIONINFO                     0xF1U

#if (defined CANNM_SERVID_CONFIRMPNAVAILABILITY)
#error CANNM_SERVID_CONFIRMPNAVAILABILITY  already defined
#endif /* if (defined CANNM_SERVID_RXINDICATION) */
/** \brief AUTOSAR API service ID.
**
** Definition of CANNM_SERVID_CONFIRMPNAVAILABILITY.
*/
#define CANNM_SERVID_CONFIRMPNAVAILABILITY              0x16U

/* ---------------------[ internal API service IDs ]------------------------*/
#if (defined CANNM_SERVID_TXTIMEOUTEXCEPTION)
#error CANNM_SERVID_TXTIMEOUTEXCEPTION already defined
#endif /* if (defined CANNM_SERVID_TXTIMEOUTEXCEPTION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_SERVID_TXTIMEOUTEXCEPTION.
 */
#define CANNM_SERVID_TXTIMEOUTEXCEPTION                 0x27U

/* ---------------------[ other constants ]-------------------------------- */

#if (defined CANNM_PDU_BYTE_0)
#error CANNM_PDU_BYTE_0 already defined
#endif /* if (defined CANNM_PDU_BYTE_0) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_PDU_BYTE_0.
 */
#define CANNM_PDU_BYTE_0                                   0U

#if (defined CANNM_PDU_BYTE_1)
#error CANNM_PDU_BYTE_1 already defined
#endif /* if (defined CANNM_PDU_BYTE_1) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_PDU_BYTE_1.
 */
#define CANNM_PDU_BYTE_1                                   1U

#if (defined CANNM_PDU_OFF)
#error CANNM_PDU_OFF already defined
#endif /* if (defined CANNM_PDU_OFF) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of CANNM_PDU_OFF.
 */
#define CANNM_PDU_OFF                                      3U

/* ---------------------[ Module instance IDs ]---------------------------- */

#if (defined CANNM_INSTANCE_ID)
#error CANNM_INSTANCE_ID already defined
#endif
/** \brief Instance Id of CanNm */
#define CANNM_INSTANCE_ID                                  0U

#if (defined CANNM_INVALID_PDU_INSTANCE_ID)
#error CANNM_INVALID_PDU_INSTANCE_ID already defined
#endif
/** \brief Instance Id of CanNm when an invalid PDU is passed */
#define CANNM_INVALID_PDU_INSTANCE_ID                    255U

/* ---------------------[ DET Error IDs ]---------------------------------- */

#if (defined CANNM_E_NO_INIT)
#error CANNM_E_NO_INIT already defined
#endif
/** \brief Initialization status before module initilaization */
#define CANNM_E_NO_INIT                                 0x01U

#if (defined CANNM_E_INVALID_CHANNEL)
#error CANNM_E_INVALID_CHANNEL already defined
#endif
/** \brief Error Code for Invalid channel */
#define CANNM_E_INVALID_CHANNEL                         0x02U

#if (defined CANNM_E_NETWORK_TIMEOUT)
#error CANNM_E_NETWORK_TIMEOUT already defined
#endif
/** \brief Error code for unexpected timeout of NM timer */
#define CANNM_E_NETWORK_TIMEOUT                         0x11U

#if (defined CANNM_E_NULL_POINTER)
#error CANNM_E_NULL_POINTER already defined
#endif
/** \brief Error code for NULL pointers */
#define CANNM_E_NULL_POINTER                            0x12U

#if (defined CANNM_E_INVALID_PDUID)
#error CANNM_E_INVALID_PDUID already defined
#endif
/** \brief Error code for Invalid PDU Id */
#define CANNM_E_INVALID_PDUID                           0x21U

#if (defined CANNM_E_NET_START_IND)
#error CANNM_E_NET_START_IND already defined
#endif
/** \brief Error code for Reception of NM messages in Bus-Sleep Mode. */
#define CANNM_E_NET_START_IND                           0x04U

#if (defined CANNM_E_INVALID_FUNCTION_ARG)
#error CANNM_E_INVALID_FUNCTION_ARG already defined
#endif
/** \brief Error code for other invalid API function argument in API */
#define CANNM_E_INVALID_FUNCTION_ARG                    0x23U

#if (defined CANNM_E_INIT_FAILED)
#error CANNM_E_INIT_FAILED already defined
#endif
/** \brief Error code for the case in which CanNm initialization fails*/
#define CANNM_E_INIT_FAILED                             0x05U


#if (defined CANNM_E_NETWORKSTARTINDICATION)
#error CANNM_E_NETWORKSTARTINDICATION already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_NetworkStartIndication fails*/
#define CANNM_E_NETWORKSTARTINDICATION    250U

#if (defined CANNM_E_NETWORKMODE)
#error CANNM_E_NETWORKMODE already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_NetworkMode fails*/
#define CANNM_E_NETWORKMODE               249U

#if (defined CANNM_E_BUSSLEEPMODE)
#error CANNM_E_BUSSLEEPMODE already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_BusSleepMode fails*/
#define CANNM_E_BUSSLEEPMODE              248U

#if (defined CANNM_E_PREPAREBUSSLEEPMODE)
#error CANNM_E_PREPAREBUSSLEEPMODE already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_PrepareBusSleepMode fails*/
#define CANNM_E_PREPAREBUSSLEEPMODE       247U

#if (defined CANNM_E_REMOTESLEEPINDICATION)
#error CANNM_E_REMOTESLEEPINDICATION already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_RemoteSleepIndication fails*/
#define CANNM_E_REMOTESLEEPINDICATION     246U

#if (defined CANNM_E_REMOTESLEEPCANCELLATION)
#error CANNM_E_REMOTESLEEPCANCELLATION already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_RemoteSleepCancellation fails*/
#define CANNM_E_REMOTESLEEPCANCELLATION   245U

#if (defined CANNM_E_PDURXINDICATION)
#error CANNM_E_PDURXINDICATION already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_PduRxIndication fails*/
#define CANNM_E_PDURXINDICATION           244U

#if (defined CANNM_E_STATECHANGENOTIFICATION)
#error CANNM_E_STATECHANGENOTIFICATION already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_StateChangeNotification fails*/
#define CANNM_E_STATECHANGENOTIFICATION   243U

#if (defined CANNM_E_REPEATMESSAGEINDICATION)
#error CANNM_E_REPEATMESSAGEINDICATION already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_RepeatMessageIndication fails*/
#define CANNM_E_REPEATMESSAGEINDICATION   242U

#if (defined CANNM_E_TXTIMEOUTEXCEPTION)
#error CANNM_E_TXTIMEOUTEXCEPTION already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_TxTimeoutException fails*/
#define CANNM_E_TXTIMEOUTEXCEPTION        241U

#if (defined CANNM_E_CARWAKEUPINDICATION)
#error CANNM_E_CARWAKEUPINDICATION already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_CarWakeUpIndication fails*/
#define CANNM_E_CARWAKEUPINDICATION       240U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
/* !LINKSTO CANNM302,1 */

#define CANNM_START_SEC_CODE
#include <CanNm_MemMap.h>

/** \brief Validate configuration
 **
 ** Checks if the post build configuration fits to the link time configuration part.
 **
 ** \return E_OK if the given module configurations is valid otherwise E_NOT_OK.
 **
 ** \ServiceID{0x60}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_IsValidConfig
(
    P2CONST(void, AUTOMATIC, CANNM_APPL_CONST) voidConfigPtr
);

#ifndef RTE_SCHM_SWCBSW_CANNM_PASSIVESTARTUP
#define RTE_SCHM_SWCBSW_CANNM_PASSIVESTARTUP
/** \brief Passive startup of CanNm module.
 **
 ** This function performs a passive startup of the AUTOSAR CAN NM.
 ** It triggers the transition from Bus-Sleep Mode to the Network Mode
 ** in Repeat Message State.
 **
 ** This service has no effect if the current state is not equal to
 ** Bus-Sleep Mode. In that case E_NOT_OK is returned.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Passive startup of network management has
 **            failed/not executed.
 **
 ** \ServiceID{1}
 ** \Reentrancy{Reentrant (But not for the same NM Channel)}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_PassiveStartUp
(
   NetworkHandleType nmChannelHandle
);
#endif /* RTE_SCHM_SWCBSW_CANNM_PASSIVESTARTUP */

#if (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)
#ifndef RTE_SCHM_SWCBSW_CANNM_NETWORKREQUEST
#define RTE_SCHM_SWCBSW_CANNM_NETWORKREQUEST
/** \brief Network Request.
 **
 ** This function request the network when bus communication is needed.
 ** Network state shall be changed to requested.
 ** If function is called active wakeup bit is set
 **
 ** \param[in] nmChannelHandle Identification of the NM channel.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Requesting of network has failed.
 **
 ** \ServiceID{2}
 ** \Reentrancy{Reentrant (But not for the same NM Channel)}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_NetworkRequest
(
   NetworkHandleType nmChannelHandle
);
#endif /* RTE_SCHM_SWCBSW_CANNM_NETWORKREQUEST */

#ifndef RTE_SCHM_SWCBSW_CANNM_NETWORKGWERAREQUEST
#define RTE_SCHM_SWCBSW_CANNM_NETWORKGWERAREQUEST
/** \brief Network Gateway Era Request
 **
 ** This function request the network when bus communication is needed.
 ** Network state shall be changed to requested.
 ** If function is called active wakeup bit is not set
 **
 ** \param[in] nmChannelHandle Identification of the NM channel.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Requesting of network has failed.
 **
 ** \ServiceID{254}
 ** \Reentrancy{Reentrant (But not for the same NM Channel)}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_NetworkGwEraRequest
(
   NetworkHandleType nmChannelHandle
);
#endif /* RTE_SCHM_SWCBSW_CANNM_NETWORKGWERAREQUEST */

#ifndef RTE_SCHM_SWCBSW_CANNM_NETWORKRELEASE
#define RTE_SCHM_SWCBSW_CANNM_NETWORKRELEASE
/** \brief Release the Network.
 **
 ** This function releases the network, when there is no need for bus
 ** communication. Network state shall be changed to released.
 **
 ** \param[in] nmChannelHandle Identification of the NM channel.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Releasing of network has failed/not executed.
 **
 ** \ServiceID{3}
 ** \Reentrancy{Reentrant (But not for the same NM Channel)}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_NetworkRelease
(
   NetworkHandleType nmChannelHandle
);
#endif /* RTE_SCHM_SWCBSW_CANNM_NETWORKRELEASE */
#endif

#if (CANNM_COM_CONTROL_ENABLED == STD_ON)
/** \brief Disable NM PDU transmission.
 **
 ** This function disables the NM PDU transmission ability due to
 ** a ISO14229 Communication Control (28hex) service.
 **
 ** \param[in] nmChannelHandle Identification of the NM channel.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Disabling of NM PDU transmission
 **            ability has failed/not executed.
 **
 ** \ServiceID{12}
 ** \Reentrancy{Reentrant (But not for the same NM Channel)}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_DisableCommunication
(
   NetworkHandleType nmChannelHandle
);


/** \brief Enable NM PDU transmission.
 **
 ** This function enables the NM PDU transmission ability due to a
 ** ISO14229 Communication Control (28hex) service.
 **
 ** \param[in] nmChannelHandle Identification of the NM channel.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Enabling of NM PDU transmission
 **            ability has failed/not executed.
 **
 ** \ServiceID{13}
 ** \Reentrancy{Reentrant (But not for the same NM Channel)}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_EnableCommunication
(
   NetworkHandleType nmChannelHandle
);
#endif

#if (CANNM_NODE_DETECTION_ENABLED == STD_ON)
/** \brief Set the Repeat Message Request Bit.
 **
 ** This function sets the Repeat Message Request Bit for NM messages
 ** transmitted next on the bus.
 **
 ** \param[in] nmChannelHandle Identification of the NM channel.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Setting of Repeat Message Request Bit has
 **            failed/not executed.
 **
 ** \ServiceID{8}
 ** \Reentrancy{Reentrant (but not for the same NM Channel)}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_RepeatMessageRequest
(
   NetworkHandleType nmChannelHandle
);
#endif /* #if (CANNM_NODE_DETECTION_ENABLED == STD_ON)*/

/** \brief Get the State and mode of the Network Management.
 **
 ** This function returns the state and the mode of the network management.
 **
 ** \param[in] nmChannelHandle Identification of the NM-channel.
 ** \param[out] nmStatePtr Pointer to state of network management.
 ** \param[out] nmModePtr Pointer to mode of network management.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Getting of NM state has failed.
 **
 ** \ServiceID{11}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_GetState
(
   NetworkHandleType nmChannelHandle,
   P2VAR(Nm_StateType, AUTOMATIC, CANNM_APPL_DATA) nmStatePtr,
   P2VAR(Nm_ModeType, AUTOMATIC, CANNM_APPL_DATA)  nmModePtr
);


#if ((CANNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON)      \
   && (CANNM_PASSIVE_MODE_ENABLED == STD_OFF))
#ifndef RTE_SCHM_SWCBSW_CANNM_REQUESTBUSSYNCHRONIZATION
#define RTE_SCHM_SWCBSW_CANNM_REQUESTBUSSYNCHRONIZATION
/** \brief Request Bus Synchorization
 **
 ** This function requests bus synchronization.
 **
 ** \param[in] nmChannelHandle Identification of the NM-channel.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Requesting of bus synchronization
 **             has failed/not executed.
 **
 ** \ServiceID{192}
 ** \Reentrancy{Reentrant (but not for the same NM Channel)}
 ** \Synchronicity{synchronous}
 */
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_RequestBusSynchronization
(
   NetworkHandleType nmChannelHandle
);
#endif /* RTE_SCHM_SWCBSW_CANNM_REQUESTBUSSYNCHRONIZATION */
#endif /* if ((CANNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON)
        * && (CANNM_PASSIVE_MODE_ENABLED == STD_OFF)) */

#if (CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
/** \brief Check if sleep indication has taken place.
 **
 ** This function checks if remote sleep indication has taken place
 ** or not.
 **
 ** \param[in] nmChannelHandle Identification of the NM-channel.
 ** \param[out] nmRemoteSleepIndPtr Pointer where check result
 **                   of remote sleep indication shall be copied to.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Checking of remote sleep indication
 **            bits has failed/not executed.
 **
 ** \ServiceID{208}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_CheckRemoteSleepIndication
(
   NetworkHandleType nmChannelHandle,
   P2VAR(boolean, AUTOMATIC, CANNM_APPL_DATA) nmRemoteSleepIndPtr
);
#endif /* #if (CANNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)*/

/** \brief Initialization of CanNm module.
 **
 ** This function initializes the CanNm module and
 ** starts the cyclic transmission of NM-packages.
 **
 ** This function has to be called after initialization of the CanIf.
 **
 ** \param[in] cannmConfigPtr Pointer to selected configuration structure.
 **
 ** \ServiceID{1}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, CANNM_CODE) CanNm_Init
(
   P2CONST(CanNm_ConfigType, AUTOMATIC, CANNM_APPL_DATA) cannmConfigPtr
);

#ifndef RTE_SCHM_SWCBSW_CANNM_MAINFUNCTION
#define RTE_SCHM_SWCBSW_CANNM_MAINFUNCTION
/** \brief Main function of the CanNm.
 **
 ** This function handles scheduled tasks such as timers.
 **
 ** \ServiceID{19}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
/* Deviation MISRAC2012-1 */
extern FUNC(void, CANNM_CODE) CanNm_MainFunction(void);
#endif

#if (CANNM_NODE_ID_ENABLED == STD_ON)
/** \brief Get Node Identifier.
 **
 ** This function gets the node identifier out of the most recently
 ** received NM PDU.
 **
 ** \param[in] nmChannelHandle Identification of the NM channel.
 ** \param[out] nmNodeIdPtr Pointer where node identifier out of
 **                   the most recently received NM PDU shall be copied to.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Getting of the node identifier out of
 **            the most recently received NM PDU has failed.
 **
 ** \ServiceID{6}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_GetNodeIdentifier
(
   NetworkHandleType nmChannelHandle,
   P2VAR(uint8, AUTOMATIC, CANNM_APPL_DATA) nmNodeIdPtr
);

/** \brief Get Local Node Identifier.
 **
 ** This function gets the node identifier configured as the local node.
 **
 ** \param[in] nmChannelHandle Identification of the NM channel.
 ** \param[out] nmNodeIdPtr Pointer where node identifier of the
 **                   local node shall be copied to.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Getting of the node identifier of the
 **            local node has failed.
 **
 ** \ServiceID{7}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_GetLocalNodeIdentifier
(
   NetworkHandleType nmChannelHandle,
   P2VAR(uint8, AUTOMATIC, CANNM_APPL_DATA) nmNodeIdPtr
);
#endif /* (CANNM_NODE_ID_ENABLED == STD_ON) */

#if (CANNM_USER_DATA_ENABLED == STD_ON)
/** \brief Get User Data from NM messages.
 **
 ** This function retrieves the user data from the last received
 ** NM message.
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should
 **    have been initialized for this channel (checked).
 **
 ** \param[in] nmChannelHandle Identification of the NM channel.
 ** \param[in] nmUserDataPtr Pointer to where user data out of
 **                the most recently received NM message shall be
 **                copied to.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Getting of user data has failed.
 **
 ** \ServiceID{5}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_GetUserData
(
   NetworkHandleType nmChannelHandle,
   P2VAR(uint8, AUTOMATIC, CANNM_APPL_DATA) nmUserDataPtr
);

#if ((CANNM_PASSIVE_MODE_ENABLED == STD_OFF) &&                             \
     (CANNM_COM_USER_DATA_SUPPORT == STD_OFF))
/** \brief Set User Data for NM messages.
 **
 ** This function sets the user data for the next NM message that is
 ** transmitted on the bus.
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should
 **     have been initialized for this channel (checked).
 **
 ** \param[in] nmChannelHandle Identification of the NM channel.
 ** \param[in] nmUserDataPtr Pointer where the user data for the
 **                next transmitted NM message shall be copied from.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Setting of user data has failed.
 **
 ** \ServiceID{4}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_SetUserData
(
   NetworkHandleType nmChannelHandle,
   P2CONST(uint8, AUTOMATIC, CANNM_APPL_DATA) nmUserDataPtr
);
#endif /* ((CANNM_PASSIVE_MODE_ENABLED == STD_OFF)  &&
        *  (CANNM_COM_USER_DATA_SUPPORT == STD_OFF)) */
#endif /* (CANNM_USER_DATA_ENABLED == STD_ON) */

#if (CANNM_COM_USER_DATA_SUPPORT == STD_ON)

/** \brief Dummy function.
 **
 ** CanNm_Transmit is implemented as an empty function and
 ** always returns E_OK. The function CanNm_Transmit is only
 ** available if the configuration switch CanNmComUserDataSupport
 ** is enabled.
 **
 ** Preconditions:
 ** - None
 **
 ** \param[in] CanTxPduId Identification of the NM channel.
 ** \param[in] PduInfoPtr Pointer to a structure with CAN L-PDU
 **                       related data: DLC and pointer to CAN
 **                       L-SDU buffer
 **
 ** \return Standard Return Code
 ** \retval E_OK  always
 **
 ** \ServiceID{0}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_Transmit(
    PduIdType CanTxPduId,
    P2CONST(PduInfoType, AUTOMATIC, CANNM_APPL_DATA) PduInfoPtr
);
#endif /* (CANNM_COM_USER_DATA_SUPPORT == STD_ON) */

#if ((CANNM_USER_DATA_ENABLED == STD_ON)                                \
   || (CANNM_NODE_ID_ENABLED == STD_ON)                                 \
   || (CANNM_NODE_DETECTION_ENABLED == STD_ON))
/** \brief Retrieve the data of the last received NM message.
 **
 ** This function retrieves the whole PDU data out of the most
 ** recently received NM message.
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should
 **   have been initialized for this channel (checked).
 **
 ** \param[in] nmChannelHandle Identification of the NM channel.
 ** \param[in] nmPduDataPtr Pointer where NM PDU data
 **                shall be copied to.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Getting of NM PDU data has failed.
 **
 ** \ServiceID{10}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, CANNM_CODE) CanNm_GetPduData
(
   NetworkHandleType nmChannelHandle,
   P2VAR(uint8, AUTOMATIC, CANNM_APPL_DATA) nmPduDataPtr
);
#endif /* ((CANNM_USER_DATA_ENABLED == STD_ON)
        * || (CANNM_NODE_ID_ENABLED == STD_ON)
        * || (CANNM_NODE_DETECTION_ENABLED == STD_ON)) */


#if (CANNM_PN_SUPPORTED == STD_ON)
 /** \brief Enable PN Filtering.
 **
 ** Enables the PN filter functionality on the indicated NM channel.
 ** Availability:
 **
 ** Preconditions:
 ** - The API is only available if CanNmPnSupported is TRUE.
 **
 ** \param[in] nmChannelHandle Identification of the NM channel.
 **
 ** \return void
 **
 ** \ServiceID{0x16}
 ** \Reentrancy{Reentrant (but not for the same NM Channel)}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, CANNM_CODE) CanNm_ConfirmPnAvailability
(
   NetworkHandleType nmChannelHandle
);
#endif /* (CANNM_PN_SUPPORTED == STD_ON) */

#if (CANNM_VERSION_INFO_API == STD_ON)
/** \brief Get version information for the CAN Network Management.
 **
 ** This service returns the version information of this module.
 ** The version information includes:
 ** - Module Id
 ** - Vendor Id
 ** - Vendor specific version numbers (BSW00407).
 **
 ** Note: This function can be called even if CanNm is not initialized.
 **
 ** \param[out] versioninfo Pointer to where to store the version
 ** information of this module.
 **
 ** \ServiceID{241}
 ** \Reentrancy{reentrant}
 ** \Synchronicity{synchronous}
 **/
extern FUNC(void, CANNM_CODE) CanNm_GetVersionInfo
(
   P2VAR (Std_VersionInfoType, AUTOMATIC, CANNM_APPL_DATA) versioninfo
);
#endif /* (CANNM_VERSION_INFO_API == STD_ON) */
#define CANNM_STOP_SEC_CODE
#include <CanNm_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef CANNM_API_H */
/*==================[end of file]===========================================*/
