/**
 * \file
 *
 * \brief AUTOSAR FrNm
 *
 * This file contains the implementation of the AUTOSAR
 * module FrNm.
 *
 * \version 5.16.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef FRNM_API_H
#define FRNM_API_H

/*==================[inclusions]=========================================*/
/* !LINKSTO FRNM066_NmStack_Types,1 */
#include <NmStack_Types.h>       /* Nm specific types */
/* !LINKSTO FRNM400,1 */
#include <FrNm_Version.h>       /* Version information for FrNm-module  */
#include <FrNm_Cfg.h>
#include <FrNm_Types.h>

#if (FRNM_PBCFGM_SUPPORT_ENABLED == STD_ON)
#include <PbcfgM_Api.h>         /* Post build configuration manager */
#endif /* FRNM_PBCFGM_SUPPORT_ENABLED */

/*==================[macros]=============================================*/

/* -----------------[ list of AUTOSAR API service IDs ]------------------*/

#if (defined FRNM_SERVID_INIT)
#error FRNM_SERVID_INIT already defined
#endif /* if (defined FRNM_SERVID_INIT) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_INIT.
 */
#define FRNM_SERVID_INIT                               0x00U

#if (defined FRNM_SERVID_PASSIVESTARTUP)
#error FRNM_SERVID_PASSIVESTARTUP already defined
#endif /* if (defined FRNM_SERVID_PASSIVESTARTUP) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_PASSIVESTARTUP
 */
#define FRNM_SERVID_PASSIVESTARTUP                     0x01U


#if (defined FRNM_SERVID_NETWORKREQUEST)
#error FRNM_SERVID_NETWORKREQUEST already defined
#endif /* if (defined FRNM_SERVID_NETWORKREQUEST) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_NETWORKREQUEST.
 */
#define FRNM_SERVID_NETWORKREQUEST                     0x02U

#if (defined FRNM_SERVID_NETWORKGWERAREQUEST)
#error FRNM_SERVID_NETWORKGWERAREQUEST already defined
#endif /* if (defined FRNM_SERVID_NETWORKGWERAREQUEST) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_NETWORKGWERAREQUEST
 */
#define FRNM_SERVID_NETWORKGWERAREQUEST                     0xFEU

#if (defined FRNM_SERVID_NETWORKRELEASE)
#error FRNM_SERVID_NETWORKRELEASE already defined
#endif /* if (defined FRNM_SERVID_NETWORKRELEASE) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_NETWORKRELEASE.
 */
#define FRNM_SERVID_NETWORKRELEASE                     0x03U



#if (defined FRNM_SERVID_SETUSERDATA)
#error FRNM_SERVID_SETUSERDATA already defined
#endif /* if (defined FRNM_SERVID_SETUSERDATA) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_SETUSERDATA.
 */
#define FRNM_SERVID_SETUSERDATA                        0x06U


#if (defined FRNM_SERVID_GETUSERDATA)
#error FRNM_SERVID_GETUSERDATA already defined
#endif /* if (defined FRNM_SERVID_GETUSERDATA) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_GETUSERDATA.
 */
#define FRNM_SERVID_GETUSERDATA                        0x07U

#if (defined FRNM_SERVID_GETPDUDATA)
#error FRNM_SERVID_GETPDUDATA already defined
#endif /* if (defined FRNM_SERVID_GETPDUDATA) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_GETPDUDATA.
 */
#define FRNM_SERVID_GETPDUDATA                         0x08U

#if (defined FRNM_SERVID_REPEATMESSAGEREQUEST)
#error FRNM_SERVID_REPEATMESSAGEREQUEST already defined
#endif /* if (defined FRNM_SERVID_REPEATMESSAGEREQUEST) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_REPEATMESSAGEREQUEST.
 */
#define FRNM_SERVID_REPEATMESSAGEREQUEST               0x09U


#if (defined FRNM_SERVID_GETNODEIDENTIFIER)
#error FRNM_SERVID_GETNODEIDENTIFIER already defined
#endif /* if (defined FRNM_SERVID_GETNODEIDENTIFIER) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_GETNODEIDENTIFIER.
 */
#define FRNM_SERVID_GETNODEIDENTIFIER                  0x0aU


#if (defined FRNM_SERVID_GETLOCALNODEIDENTIFIER)
#error FRNM_SERVID_GETLOCALNODEIDENTIFIER already defined
#endif /* if (defined FRNM_SERVID_GETLOCALNODEIDENTIFIER) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_GETLOCALNODEIDENTIFIER.
 */
#define FRNM_SERVID_GETLOCALNODEIDENTIFIER             0x0bU

#if (defined FRNM_SERVID_REQUESTBUSSYNCHRONIZATION)
#error FRNM_SERVID_REQUESTBUSSYNCHRONIZATION already defined
#endif /* if (defined FRNM_SERVID_REQUESTBUSSYNCHRONIZATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_REQUESTBUSSYNCHRONIZATION.
 */
#define FRNM_SERVID_REQUESTBUSSYNCHRONIZATION          0xC0U

#if (defined FRNM_SERVID_CHECKREMOTESLEEPINDICATION)
#error FRNM_SERVID_CHECKREMOTESLEEPINDICATION already defined
#endif /* if (defined FRNM_SERVID_CHECKREMOTESLEEPINDICATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_CHECKREMOTESLEEPINDICATION.
 */
#define FRNM_SERVID_CHECKREMOTESLEEPINDICATION         0x0DU

#if (defined FRNM_SERVID_GETSTATE)
#error FRNM_SERVID_GETSTATE already defined
#endif /* if (defined FRNM_SERVID_GETSTATE) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_GETSTATE.
 */
#define FRNM_SERVID_GETSTATE                           0x0EU

#if (defined FRNM_SERVID_GETVERSIONINFO)
#error FRNM_SERVID_GETVERSIONINFO already defined
#endif /* if (defined FRNM_SERVID_GETVERSIONINFO) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_GETVERSIONINFO.
 */
#define FRNM_SERVID_GETVERSIONINFO                     0x0FU

#if (defined FRNM_SERVID_STARTUPERROR)
#error FRNM_SERVID_STARTUPERROR already defined
#endif /* if (defined FRNM_SERVID_STARTUPERROR) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_STARTUPERROR.
 */
#define FRNM_SERVID_STARTUPERROR                     0x10U

#if (defined FRNM_SERVID_RXINDICATION)
#error FRNM_SERVID_RXINDICATION already defined
#endif /* if (defined FRNM_SERVID_RXINDICATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_RXINDICATION.
 */
#define FRNM_SERVID_RXINDICATION                      0xE1U

#if (defined FRNM_SERVID_TRANSMIT)
#error FRNM_SERVID_TRANSMIT already defined
#endif /* if (defined FRNM_SERVID_TRANSMIT) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_TRANSMIT.
 */
#define FRNM_SERVID_TRANSMIT                       0x00U

#if (defined FRNM_SERVID_TRIGGERTRANSMIT)
#error FRNM_SERVID_TRIGGERTRANSMIT already defined
#endif /* if (defined FRNM_SERVID_TRIGGERTRANSMIT) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_TRIGGERTRANSMIT.
 */
#define FRNM_SERVID_TRIGGERTRANSMIT                       0xE4U

#if (defined FRNM_SERVID_TXCONFIRMATION)
#error FRNM_SERVID_TXCONFIRMATION already defined
#endif /* if (defined FRNM_SERVID_TXCONFIRMATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_TXCONFIRMATION.
 */
#define FRNM_SERVID_TXCONFIRMATION                     0xE0U

#if (defined FRNM_SERVID_MAINFUNCTION_X)
#error FRNM_SERVID_MAINFUNCTION_X already defined
#endif /* if (defined FRNM_SERVID_MAINFUNCTION_X) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_MAINFUNCTION_X.
 */
#define FRNM_SERVID_MAINFUNCTION_X                     0xF0U


#if (defined FRNM_SERVID_DISABLECOMMUNICATION)
#error FRNM_SERVID_DISABLECOMMUNICATION already defined
#endif /* if (defined FRNM_SERVID_DISABLECOMMUNICATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_DISABLECOMMUNICATION.
 */
#define FRNM_SERVID_DISABLECOMMUNICATION               0x0CU

#if (defined FRNM_SERVID_ENABLECOMMUNICATION)
#error FRNM_SERVID_ENABLECOMMUNICATION already defined
#endif /* if (defined FRNM_SERVID_ENABLECOMMUNICATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_ENABLECOMMUNICATION.
 */
#define FRNM_SERVID_ENABLECOMMUNICATION                0x0DU

#if (defined FRNM_SERVID_SETSLEEPREADYBIT)
#error FRNM_SERVID_SETSLEEPREADYBIT already defined
#endif /* if (defined FRNM_SERVID_SETSLEEPREADYBIT) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_SERVID_SETSLEEPREADYBIT.
 */
#define FRNM_SERVID_SETSLEEPREADYBIT                   0x12U
/* -----------------[ internal API service IDs ]-------------------------*/

/* -----------------[ other constants ]----------------------------------*/


#if (defined FRNM_PDU_BYTE_0)
#error FRNM_PDU_BYTE_0 already defined
#endif /* if (defined FRNM_PDU_BYTE_0) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_PDU_BYTE_0.
 */
#define FRNM_PDU_BYTE_0                                   0U

#if (defined FRNM_PDU_BYTE_1)
#error FRNM_PDU_BYTE_1 already defined
#endif /* if (defined FRNM_PDU_BYTE_1) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_PDU_BYTE_1.
 */
#define FRNM_PDU_BYTE_1                                   1U

#if (defined FRNM_PDU_OFF)
#error FRNM_PDU_OFF already defined
#endif /* if (defined FRNM_PDU_OFF) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of FRNM_PDU_OFF.
 */
#define FRNM_PDU_OFF                                      3U

/* -----------------[ Module instance IDs ]------------------------------*/

/** \brief Instance Id of FrNm */
#define FRNM_INSTANCE_ID                                  0U

/* -----------------[ DET Error IDs ]------------------------------------*/
/** \brief Initialization status before module initilaization */
#if (defined FRNM_E_UNINIT)
#error FRNM_E_UNINIT already defined
#endif
#define FRNM_E_UNINIT                                     0x01U

/** \brief Error Code for Invalid channel */
#if (defined FRNM_E_INVALID_CHANNEL)
#error FRNM_E_INVALID_CHANNEL already defined
#endif
#define FRNM_E_INVALID_CHANNEL                            0x02U

/** \brief Error code for Invalid pointers */
#if (defined FRNM_E_INVALID_POINTER)
#error FRNM_E_INVALID_POINTER already defined
#endif
#define FRNM_E_INVALID_POINTER                            0x03U

/** \brief Error code for PDU ID as input parameter*/
#if (defined FRNM_E_PDU_ID_INVALID)
#error FRNM_E_PDU_ID_INVALID already defined
#endif
#define FRNM_E_PDU_ID_INVALID                             0x04U

#if (defined FRNM_E_INVALID_FUNCTION_ARG)
#error FRNM_E_INVALID_FUNCTION_ARG already defined
#endif
#define FRNM_E_INVALID_FUNCTION_ARG                       0x05U

/** \brief Error code reported to DET in case FrNm_Init fails */
/* !LINKSTO FrNm.Init.Failed,1 */
#if (defined FRNM_E_INIT_FAILED)
#error FRNM_E_INIT_FAILED already defined
#endif
#define FRNM_E_INIT_FAILED                                0x06U

#if (defined FRNM_E_NETWORKSTARTINDICATION)
#error FRNM_E_NETWORKSTARTINDICATION already defined
#endif
/** \brief Error code for SchM Nm_NetworkStartIndication */
#define FRNM_E_NETWORKSTARTINDICATION                     250U

#if (defined FRNM_E_NETWORKMODE)
#error FRNM_E_NETWORKMODE already defined
#endif
/** \brief Error code for SchM Nm_NetworkMode */
#define FRNM_E_NETWORKMODE                                249U

#if (defined FRNM_E_BUSSLEEPMODE)
#error FRNM_E_BUSSLEEPMODE already defined
#endif
/** \brief Error code for SchM Nm_BusSleepMode */
#define FRNM_E_BUSSLEEPMODE                               248U

#if (defined FRNM_E_REMOTESLEEPINDICATION)
#error FRNM_E_REMOTESLEEPINDICATION already defined
#endif
/** \brief Error code for SchM Nm_RemoteSleepIndication */
#define FRNM_E_REMOTESLEEPINDICATION                      247U

#if (defined FRNM_E_REMOTESLEEPCANCELLATION)
#error FRNM_E_REMOTESLEEPCANCELLATION already defined
#endif
/** \brief Error code for SchM Nm_RemoteSleepCancellation */
#define FRNM_E_REMOTESLEEPCANCELLATION                    246U

#if (defined FRNM_E_COORDREADYTOSLEEPINDICATION)
#error FRNM_E_COORDREADYTOSLEEPINDICATION already defined
#endif
/** \brief Error code for SchM Nm_CoordReadyToSLeepIndication */
#define FRNM_E_COORDREADYTOSLEEPINDICATION                245U

#if (defined FRNM_E_SYNCHRONIZATIONPOINT)
#error FRNM_E_SYNCHRONIZATIONPOINT already defined
#endif
/** \brief Error code for SchM Nm_SynchronizationPoint */
#define FRNM_E_SYNCHRONIZATIONPOINT                       244U

#if (defined FRNM_E_PDURXINDICATION)
#error FRNM_E_PDURXINDICATION already defined
#endif
/** \brief Error code for SchM Nm_PduRxIndication */
#define FRNM_E_PDURXINDICATION                            243U

#if (defined FRNM_E_STATECHANGENOTIFICATION)
#error FRNM_E_STATECHANGENOTIFICATION already defined
#endif
/** \brief Error code for SchM Nm_StateChangeNotification */
#define FRNM_E_STATECHANGENOTIFICATION                    242U

#if (defined FRNM_E_TXTIMEOUTEXCEPTION)
#error FRNM_E_TXTIMEOUTEXCEPTION already defined
#endif
/** \brief Error code for SchM Nm_TxTimeoutException */
#define FRNM_E_TXTIMEOUTEXCEPTION                         241U

#if (defined FRNM_E_CARWAKEUPINDICATION)
#error FRNM_E_CARWAKEUPINDICATION already defined
#endif
/** \brief Error code for SchM Nm_CarWakeUpIndication */
#define FRNM_E_CARWAKEUPINDICATION                        240U

/*==================[type definitions]===================================*/

/*==================[external function declarations]=====================*/
/* !LINKSTO FRNM367,1 */
#define FRNM_START_SEC_CODE
#include <FrNm_MemMap.h>

/** \brief Initialization of FrNm module.
 **
 ** This function shall initialize the FrNm module.
 **
 ** \param[in] nmConfigPtr Pointer to selected configuration set.
 **
 ** \ServiceID{0x00}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, FRNM_CODE) FrNm_Init
(

   CONSTP2CONST(FrNm_ConfigType, AUTOMATIC, FRNM_APPL_CONST) nmConfigPtr
);

#ifndef RTE_SCHM_SWCBSW_FRNM_PASSIVESTARTUP
#define RTE_SCHM_SWCBSW_FRNM_PASSIVESTARTUP
/** \brief Initiates the Passive Startup of the FlexRay NM.
 **
 ** This function shall initiate the Passive Startup of the FlexRay NM.
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 **
 ** \return Std_ReturnType
 ** \retval E_OK No error
 ** \retval E_NOT_OK Start of network management has failed.
 **
 ** \ServiceID{0x01}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, FRNM_CODE) FrNm_PassiveStartUp
(
   NetworkHandleType NetworkHandle
);
#endif /* RTE_SCHM_SWCBSW_FRNM_PASSIVESTARTUP */

#if (FRNM_PASSIVE_MODE_ENABLED == STD_OFF)
#ifndef RTE_SCHM_SWCBSW_FRNM_NETWORKREQUEST
#define RTE_SCHM_SWCBSW_FRNM_NETWORKREQUEST
/** \brief This function requests the network because the ECU needs to communicate on the bus.
 ** Network state shall be changed to "requested".
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 **
 ** \return Std_ReturnType
 ** \retval E_OK No error.
 ** \retval E_NOT_OK Requesting of bus communication has failed.
 **
 ** \ServiceID{0x02}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */

extern FUNC(Std_ReturnType, FRNM_CODE) FrNm_NetworkRequest
(
   NetworkHandleType NetworkHandle
);
#endif /* RTE_SCHM_SWCBSW_FRNM_NETWORKREQUEST */

#ifndef RTE_SCHM_SWCBSW_FRNM_NETWORKGWERAREQUEST
#define RTE_SCHM_SWCBSW_FRNM_NETWORKGWERAREQUEST
/** \brief This function requests the network because the ECU needs to communicate on the bus.
 ** Network state shall be changed to "requested".
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 **
 ** \return Std_ReturnType
 ** \retval E_OK No error.
 ** \retval E_NOT_OK Requesting of bus communication has failed.
 **
 ** \ServiceID{0xFE}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */

extern FUNC(Std_ReturnType, FRNM_CODE) FrNm_NetworkGwEraRequest
(
   NetworkHandleType NetworkHandle
);
#endif /* RTE_SCHM_SWCBSW_FRNM_NETWORKGWERAREQUEST */

#ifndef RTE_SCHM_SWCBSW_FRNM_NETWORKRELEASE
#define RTE_SCHM_SWCBSW_FRNM_NETWORKRELEASE
/** \brief This function releases the network because the ECU doesn't have to
 **        communicate on the bus. Network state shall be changed to "released".
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 **
 ** \return Std_ReturnType
 ** \retval E_OK No error.
 ** \retval E_NOT_OK Releasing of bus communication has failed
 **
 ** \ServiceID{0x03}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, FRNM_CODE) FrNm_NetworkRelease
(
   NetworkHandleType NetworkHandle
);
#endif /* RTE_SCHM_SWCBSW_FRNM_NETWORKRELEASE */
#endif /* (FRNM_PASSIVE_MODE_ENABLED == STD_OFF) */

#if ((FRNM_USER_DATA_ENABLED == STD_ON)   \
  && (FRNM_COM_USER_DATA_SUPPORT == STD_OFF))
/** \brief This function sets user data for NM-Data transmitted next on the bus.
 **
 ** If user data handling is enabled for the FrNm module,
 ** then this function shall set the user data.
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 ** \param[in] nmUserDataPtr User data for the next transmitted NM message.
 **
 ** \return Std_ReturnType
 ** \retval E_OK No error
 ** \retval E_NOT_OK Setting of user data has failed
 **
 ** \ServiceID{0x06}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, FRNM_CODE) FrNm_SetUserData
(
   NetworkHandleType NetworkHandle,
   P2CONST(uint8, AUTOMATIC, FRNM_APPL_DATA) nmUserDataPtr
);
#endif /* (FRNM_USER_DATA_ENABLED == STD_ON) && .. */

#if (FRNM_USER_DATA_ENABLED == STD_ON)
/** \brief This function gets user data from the last successfully received NM message.
 **
 ** If user data handling is enabled for the FrNm module,
 ** then this function shall provide the user data from the last received NM-Data PDU.
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 **
 ** \param[out] nmUserDataPtr Pointer to the location where the user data from the
 **                           last successfully received NM message shall be copied.
 **
 ** \return Std_ReturnType
 ** \retval E_OK No error.
 ** \retval E_NOT_OK Getting of user data has failed
 **
 ** \ServiceID{0x07}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, FRNM_CODE) FrNm_GetUserData
(
   NetworkHandleType NetworkHandle,
   P2VAR(uint8, AUTOMATIC, FRNM_APPL_DATA) nmUserDataPtr
);
#endif /* (FRNM_USER_DATA_ENABLED == STD_ON) */

#if ((FRNM_CONTROL_BIT_VECTOR_ENABLED == STD_ON) \
   || (FRNM_SOURCE_NODE_IDENTIFIER_ENABLED == STD_ON) \
   || (FRNM_USER_DATA_ENABLED == STD_ON))
/** \brief This function Gets PDU data.
 **
 ** This function shall get the whole NM PDU data out of the most recently received NM message.
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 **
 ** \param[out] nmPduData Pointer where NM PDU shall be copied to.
 **
 ** \return Std_ReturnType
 ** \retval E_OK No error
 ** \retval E_NOT_OK Getting of NM PDU data has failed
 **
 ** \ServiceID{0x08}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, FRNM_CODE) FrNm_GetPduData
(
   NetworkHandleType NetworkHandle,
   P2VAR(uint8, AUTOMATIC, FRNM_APPL_DATA) nmPduData
);
#endif /* ((FRNM_CONTROL_BIT_VECTOR_ENABLED == STD_ON)   \
        *|| (FRNM_SOURCE_NODE_IDENTIFIER_ENABLED == STD_ON) ||   \
        *|| (FRNM_USER_DATA_ENABLED == STD_ON)) */

#if (FRNM_NODE_DETECTION_ENABLED == STD_ON)
/** \brief This function causes a Repeat Message Request to be transmitted next on the bus.
 **
 ** If the node detection feature is enabled, then this function shall request
 ** node detection on the FlexRay Bus NM nodes.
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 **
 ** \return Std_ReturnType
 ** \retval E_OK No error
 ** \retval E_NOT_OK Repeat Message Request  has failed
 **
 ** \ServiceID{0x09}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, FRNM_CODE) FrNm_RepeatMessageRequest
(
   NetworkHandleType NetworkHandle
);
#endif /* (FRNM_NODE_DETECTION_ENABLED == STD_ON) */

#if (FRNM_NODE_DETECTION_ENABLED == STD_ON) && (FRNM_SOURCE_NODE_IDENTIFIER_ENABLED == STD_ON)
/** \brief This function gets the node identifier from the last successfully received NM-message.
 **
 ** If the node detection feature is enabled, then this function shall provide the node
 ** identifier from the most recently received NM-message.
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 **
 ** \param[out] nmNodeIdPtr Pointer to the location where the node identifier from the
 **              last successfully received NM-message shall be copied.
 **
 ** \return Std_ReturnType
 ** \retval E_OK No error
 ** \retval E_NOT_OK Getting of the node identifier out of the last received
 **                        NM-message has failed
 **
 ** \ServiceID{0x0a}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, FRNM_CODE) FrNm_GetNodeIdentifier
(
   NetworkHandleType NetworkHandle,
   P2VAR(uint8, AUTOMATIC, FRNM_APPL_DATA) nmNodeIdPtr
);

/** \brief This function gets the node identifier configured for the local node.
 **
 ** If node detection is enabled, then this function shall provide the node
 ** identifier configured for the local host node.
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 **
 ** \param[out] nmNodeIdPtr Pointer the location where the node identifier of the
 **                         local node shall be copied.
 **
 ** \return Std_ReturnType
 ** \retval E_OK No error
 ** \retval E_NOT_OK Getting of the node identifier of the local node has failed
 **
 ** \ServiceID{0x0b}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, FRNM_CODE) FrNm_GetLocalNodeIdentifier
(
   NetworkHandleType NetworkHandle,
   P2VAR(uint8, AUTOMATIC, FRNM_APPL_DATA) nmNodeIdPtr
);
#endif /*(FRNM_NODE_DETECTION_ENABLED == STD_ON)&&(FRNM_SOURCE_NODE_IDENTIFIER_ENABLED == STD_ON)*/

#if (FRNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON)
#ifndef RTE_SCHM_SWCBSW_FRNM_REQUESTBUSSYNCHRONIZATION
#define RTE_SCHM_SWCBSW_FRNM_REQUESTBUSSYNCHRONIZATION
/** \brief This function has no functionality -
 ** the service is provided only to be compatible to future extensions and to be
 ** compatible to the FR-NM interface.
 **
 ** \param[in] NetworkHandle Identification of the NM-Cluster.
 **
 ** \return Std_ReturnType
 ** \retval E_OK No error
 ** \retval E_NOT_OK Function failed
 **
 ** \ServiceID{0xc0}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, FRNM_CODE) FrNm_RequestBusSynchronization
(
   NetworkHandleType NetworkHandle
);
#endif /* RTE_SCHM_SWCBSW_FRNM_REQUESTBUSSYNCHRONIZATION */
#endif /* (FRNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON) */

#if (FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON)
/** \brief This function checks if remote sleep indication has taken place or not.
 **
 ** This FlexRay NM function shall provide the information about current status
 ** of Remote Sleep Indication.
 **
 ** \param[in] NetworkHandle Identification of the NM-Cluster.
 **
 ** \param[out] nmRemoteSleepIndPtr Pointer to the location where the check result
 **                                  of remote sleep indication shall be copied.
 **
 ** \return Std_ReturnType
 ** \retval E_OK No error
 ** \retval E_NOT_OK Checking of remote sleep indication bits has failed
 **
 ** \ServiceID{0x0d}
 ** \Reentrancy{Reentrant(but not for the same Nm-channel)}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, FRNM_CODE) FrNm_CheckRemoteSleepIndication
(
   NetworkHandleType NetworkHandle,
   P2VAR(boolean, AUTOMATIC, FRNM_APPL_DATA) nmRemoteSleepIndPtr
);
#endif /* (FRNM_REMOTE_SLEEP_INDICATION_ENABLED == STD_ON) */

/** \brief This function returns the state and the mode of the network management.
 **
 ** This function shall provide consistent information about the current state and
 ** the current mode of the NM state machine.
 **
 ** Note: Consistency between the provided values and the current values of
 **       the state and mode should be ensured.
 **
 ** \param[in] NetworkHandle Identification of the NM-Cluster.
 **
 ** \param[out] nmStatePtr Pointer to the location where the state
 **                         of the network management shall be copied.
 ** \param[out] nmModePtr Pointer to the location where the mode of
 **                        the network management shall be copied.
 **
 ** \return Std_ReturnType
 ** \retval E_OK No error
 ** \retval E_NOT_OK Getting of NM state has failed
 **
 ** \ServiceID{0x0e}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, FRNM_CODE) FrNm_GetState
(
   NetworkHandleType NetworkHandle,
   P2VAR(Nm_StateType, AUTOMATIC, FRNM_APPL_DATA) nmStatePtr,
   P2VAR(Nm_ModeType, AUTOMATIC, FRNM_APPL_DATA) nmModePtr
);

#if (FRNM_VERSION_INFO_API == STD_ON)
/** \brief Returns the version information.
 **
 ** This function shall return the version information of this module.
 ** The version information includes:
 ** - Module Id
 ** - Vendor Id
 ** - Vendor specific version numbers(BSW00407).
 **
 ** \param[out] NmVerInfoPtr Pointer to the location where the version information
 **                          of this module shall be copied.
 **
 ** \ServiceID{0x0f}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, FRNM_CODE) FrNm_GetVersionInfo
(
   P2VAR(Std_VersionInfoType, AUTOMATIC, FRNM_APPL_DATA) NmVerInfoPtr
);
#endif /* (FRNM_VERSION_INFO_API == STD_ON) */

/** \brief This function is called by the FrSM when synchronization of the
 **        FlexRay cluster could not be achieved.
 **
 ** \param[in] NetworkHandle Identification of the NM-channel.
 **
 ** \ServiceID{0x10}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, FRNM_CODE) FrNm_StartupError
(
   NetworkHandleType NetworkHandle
);

/** \brief This is an empty function returning E_NOT_OK at any time.
 ** This requirement is relevant to avoid linker errors as PduR expects this API to be provided.
 **
 ** \param[in] FrNmTxPduId L-PDU handle of FlexRay L-PDU to be transmitted.
 **                        This handle specifies the corresponding FlexRay
 **                        L-PDU ID and implicitly the FlexRay Driver instance
 **                        as well as the corresponding FlexRay controller
 **                        device.
 ** \param[in] PduInfoPtr  Pointer to a structure with FlexRay L-PDU related
 **                        data: DLC and pointer to FlexRay L-SDU buffer.
 **
 ** \return Std_ReturnType
 ** \retval E_OK No Transmit request has been accepted.
 ** \retval E_NOT_OK Transmit request has not been accepted
 **                  (FrNm is not in RM or NO).
 **
 ** \ServiceID{0x11}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, FRNM_CODE) FrNm_Transmit
(
  PduIdType FrNmTxPduId,
  P2CONST(PduInfoType, AUTOMATIC, FRNM_APPL_CONST) PduInfoPtr
);

/** \brief Enable the NM PDU transmission ability due to a ISO14229
 **        Communication Control (28hex) service.
 **
 ** \param[in] nmChannelHandle Identification of the NM-channel.
 **
 ** \return Std_ReturnType
 ** \retval E_OK No error
 ** \retval E_NOT_OK Enabling of NM PDU transmission ability has failed
 **
 ** \ServiceID{0x05}
 ** \Reentrancy{Reentrant (but not for the same NM-channel)}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, FRNM_CODE) FrNm_EnableCommunication
(
   NetworkHandleType nmChannelHandle
);

/** \brief Disable the NM PDU transmission ability due to a ISO14229
 **       Communication Control (28hex) service.
 **
 ** \param[in] nmChannelHandle Identification of the NM-channel.
 **
 ** \return Std_ReturnType
 ** \retval E_OK No error
 ** \retval E_NOT_OK Disabling of NM PDU transmission ability has failed
 **
 ** \ServiceID{0x0c}
 ** \Reentrancy{Reentrant (but not for the same NM-channel)}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, FRNM_CODE) FrNm_DisableCommunication
(
   NetworkHandleType nmChannelHandle
);

#if (FRNM_COORDINATOR_SYNC_SUPPORT == STD_ON)
/** \brief Set the NM Coordinator Sleep Ready bit in the Control Bit Vector
 **
 ** \param[in] nmChannelHandle Identification of the NM-channel.
 **
 ** \param[in] nmSleepReadyBit Value written to ReadySleep Bit in CBV
 **
 ** \return Std_ReturnType
 ** \retval E_OK No error
 ** \retval E_NOT_OK Writing of remote sleep indication bit has failed.
 **
 ** \ServiceID{0x12}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, FRNM_CODE) FrNm_SetSleepReadyBit
(
   NetworkHandleType nmChannelHandle,
   boolean nmSleepReadyBit
);
#endif /* (FRNM_COORDINATOR_SYNC_SUPPORT == STD_ON) */
#define FRNM_STOP_SEC_CODE
#include <FrNm_MemMap.h>

/*==================[internal function declarations]=====================*/

/*==================[external constants]=================================*/

/*==================[internal constants]=================================*/

/*==================[external data]======================================*/

/*==================[internal data]======================================*/

/*==================[external function declarations]======================*/

#define FRNM_START_SEC_CODE
#include <FrNm_MemMap.h>
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
extern FUNC(Std_ReturnType, FRNM_CODE) FrNm_IsValidConfig
(
    P2CONST(void, AUTOMATIC, FRNM_APPL_CONST) voidConfigPtr
);

#define FRNM_STOP_SEC_CODE
#include <FrNm_MemMap.h>
/*==================[internal function declarations]======================*/

#endif /* ifndef FRNM_API_H */
/*==================[end of file]========================================*/
