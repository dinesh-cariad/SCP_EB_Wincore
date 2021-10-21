/**
 * \file
 *
 * \brief AUTOSAR UdpNm
 *
 * This file contains the implementation of the AUTOSAR
 * module UdpNm.
 *
 * \version 2.9.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef UDPNM_API_H
#define UDPNM_API_H

/*==================[inclusions]============================================*/

/* !LINKSTO EB_SWS_UdpNm_00082_6,1 */
#include <NmStack_Types.h>       /* Nm specific types */
#include <UdpNm_Version.h>       /* Version information for UdpNm-module  */

#include <UdpNm_Cfg.h>
#include <UdpNm_Adaptive.h>

#if (UDPNM_ADAPTIVE == STD_OFF)
#include <UdpNm_Types.h>

#if (UDPNM_PBCFGM_SUPPORT_ENABLED == STD_ON)
#include <PbcfgM_Api.h>         /* Post build configuration manager */
#endif /* UDPNM_PBCFGM_SUPPORT_ENABLED */
#endif /* UDPNM_ADAPTIVE == STD_OFF */

/*==================[macros]================================================*/

/* ------------------[ list of AUTOSAR API service IDs ]--------------------*/

/* There seems to be no binding advice how the service ID macro names should
 * be constructed */
#if (defined UDPNM_SERVID_INIT)
#error UDPNM_SERVID_INIT already defined
#endif /* if (defined UDPNM_SERVID_INIT) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of UDPNM_SERVID_INIT.
 */
#define UDPNM_SERVID_INIT                               0x01U

#if (defined UDPNM_SERVID_PASSIVESTARTUP)
#error UDPNM_SERVID_PASSIVESTARTUP already defined
#endif /* if (defined UDPNM_SERVID_PASSIVESTARTUP) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of UDPNM_SERVID_PASSIVESTARTUP
 */
#define UDPNM_SERVID_PASSIVESTARTUP                     0x0eU

#if (defined UDPNM_SERVID_NETWORKREQUEST)
#error UDPNM_SERVID_NETWORKREQUEST already defined
#endif /* if (defined UDPNM_SERVID_NETWORKREQUEST) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of UDPNM_SERVID_NETWORKREQUEST.
 */
#define UDPNM_SERVID_NETWORKREQUEST                     0x02U

#if (defined UDPNM_SERVID_NETWORKGWERAREQUEST)
#error UDPNM_SERVID_NETWORKGWERAREQUEST already defined
#endif /* if (defined UDPNM_SERVID_NETWORKGWERAREQUEST) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of UDPNM_SERVID_NETWORKGWERAREQUEST.
 */
#define UDPNM_SERVID_NETWORKGWERAREQUEST                     0xFEU

#if (defined UDPNM_SERVID_NETWORKRELEASE)
#error UDPNM_SERVID_NETWORKRELEASE already defined
#endif /* if (defined UDPNM_SERVID_NETWORKRELEASE) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of UDPNM_SERVID_NETWORKRELEASE.
 */
#define UDPNM_SERVID_NETWORKRELEASE                     0x03U

#if (defined UDPNM_SERVID_SETUSERDATA)
#error UDPNM_SERVID_SETUSERDATA already defined
#endif /* if (defined UDPNM_SERVID_SETUSERDATA) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of UDPNM_SERVID_SETUSERDATA.
 */
#define UDPNM_SERVID_SETUSERDATA                        0x04U

#if (defined UDPNM_SERVID_GETUSERDATA)
#error UDPNM_SERVID_GETUSERDATA already defined
#endif /* if (defined UDPNM_SERVID_GETUSERDATA) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of UDPNM_SERVID_GETUSERDATA.
 */
#define UDPNM_SERVID_GETUSERDATA                        0x05U

#if (defined UDPNM_SERVID_GETNODEIDENTIFIER)
#error UDPNM_SERVID_GETNODEIDENTIFIER already defined
#endif /* if (defined UDPNM_SERVID_GETNODEIDENTIFIER) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of UDPNM_SERVID_GETNODEIDENTIFIER.
 */
#define UDPNM_SERVID_GETNODEIDENTIFIER                  0x06U

#if (defined UDPNM_SERVID_GETLOCALNODEIDENTIFIER)
#error UDPNM_SERVID_GETLOCALNODEIDENTIFIER already defined
#endif /* if (defined UDPNM_SERVID_GETLOCALNODEIDENTIFIER) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of UDPNM_SERVID_GETLOCALNODEIDENTIFIER.
 */
#define UDPNM_SERVID_GETLOCALNODEIDENTIFIER             0x07U

#if (defined UDPNM_SERVID_REPEATMESSAGEREQUEST)
#error UDPNM_SERVID_REPEATMESSAGEREQUEST already defined
#endif /* if (defined UDPNM_SERVID_REPEATMESSAGEREQUEST) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of UDPNM_SERVID_REPEATMESSAGEREQUEST.
 */
#define UDPNM_SERVID_REPEATMESSAGEREQUEST               0x08U

#if (defined UDPNM_SERVID_GETPDUDATA)
#error UDPNM_SERVID_GETPDUDATA already defined
#endif /* if (defined UDPNM_SERVID_GETPDUDATA) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of UDPNM_SERVID_GETPDUDATA.
 */
#define UDPNM_SERVID_GETPDUDATA                         0x0AU

#if (defined UDPNM_SERVID_GETSTATE)
#error UDPNM_SERVID_GETSTATE already defined
#endif /* if (defined UDPNM_SERVID_GETSTATE) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of UDPNM_SERVID_GETSTATE.
 */
#define UDPNM_SERVID_GETSTATE                           0x0BU

#if (defined UDPNM_SERVID_DISABLECOMMUNICATION)
#error UDPNM_SERVID_DISABLECOMMUNICATION already defined
#endif /* if (defined UDPNM_SERVID_DISABLECOMMUNICATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of UDPNM_SERVID_DISABLECOMMUNICATION.
 */
#define UDPNM_SERVID_DISABLECOMMUNICATION               0x0CU

#if (defined UDPNM_SERVID_ENABLECOMMUNICATION)
#error UDPNM_SERVID_ENABLECOMMUNICATION already defined
#endif /* if (defined UDPNM_SERVID_ENABLECOMMUNICATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of UDPNM_SERVID_ENABLECOMMUNICATION.
 */
#define UDPNM_SERVID_ENABLECOMMUNICATION                0x0DU

#if (defined UDPNM_SERVID_TXCONFIRMATION)
#error UDPNM_SERVID_TXCONFIRMATION already defined
#endif /* if (defined UDPNM_SERVID_TXCONFIRMATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of UDPNM_SERVID_TXCONFIRMATION.
 */
#define UDPNM_SERVID_TXCONFIRMATION                     0x0FU

#if (defined UDPNM_SERVID_RXINDICATION)
#error UDPNM_SERVID_RXINDICATION already defined
#endif /* if (defined UDPNM_SERVID_RXINDICATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of UDPNM_SERVID_RXINDICATION.
 */
#define UDPNM_SERVID_RXINDICATION                       0x10U

#if (defined UDPNM_SERVID_MAINFUNCTION_X)
#error UDPNM_SERVID_MAINFUNCTION_X already defined
#endif /* if (defined UDPNM_SERVID_MAINFUNCTION_X) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of UDPNM_SERVID_MAINFUNCTION_X.
 */
#define UDPNM_SERVID_MAINFUNCTION_X                     0x13U

#if (defined UDPNM_SERVID_REQUESTBUSSYNCHRONIZATION)
#error UDPNM_SERVID_REQUESTBUSSYNCHRONIZATION already defined
#endif /* if (defined UDPNM_SERVID_REQUESTBUSSYNCHRONIZATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of UDPNM_SERVID_REQUESTBUSSYNCHRONIZATION.
 */
#define UDPNM_SERVID_REQUESTBUSSYNCHRONIZATION          0x14U

#if (defined UDPNM_SERVID_CHECKREMOTESLEEPINDICATION)
#error UDPNM_SERVID_CHECKREMOTESLEEPINDICATION already defined
#endif /* if (defined UDPNM_SERVID_CHECKREMOTESLEEPINDICATION) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of UDPNM_SERVID_CHECKREMOTESLEEPINDICATION.
 */
#define UDPNM_SERVID_CHECKREMOTESLEEPINDICATION         0x11U

#if (defined UDPNM_SERVID_GETVERSIONINFO)
#error UDPNM_SERVID_GETVERSIONINFO already defined
#endif /* if (defined UDPNM_SERVID_GETVERSIONINFO) */

/** \brief AUTOSAR API service ID.
 **
 ** Definition of UDPNM_SERVID_GETVERSIONINFO.
 */
#define UDPNM_SERVID_GETVERSIONINFO                     0x09U

/* ---------------------[ internal API service IDs ]------------------------*/
/* ---------------------[ other constants ]-------------------------------- */
/* ---------------------[ Module instance IDs ]---------------------------- */
#if (defined UDPNM_INSTANCE_ID)
#error UDPNM_INSTANCE_ID already defined
#endif
/** \brief Instance Id of UdpNm */
/* !LINKSTO SWS_UdpNm_00210,2 */
#define UDPNM_INSTANCE_ID                                  0U

#if (defined UDPNM_INVALID_PDU_INSTANCE_ID)
#error UDPNM_INVALID_PDU_INSTANCE_ID already defined
#endif
/** \brief Instance Id of UdpNm when an invalid PDU is passed */
#define UDPNM_INVALID_PDU_INSTANCE_ID                    255U

/* ---------------------[ DET Error IDs ]---------------------------------- */
#if (defined UDPNM_E_NO_INIT)
#error UDPNM_E_NO_INIT already defined
#endif
/** \brief Initialization status before module initilaization */
#define UDPNM_E_NO_INIT                                 0x01U

#if (defined UDPNM_E_INVALID_CHANNEL)
#error UDPNM_E_INVALID_CHANNEL already defined
#endif
/** \brief Error Code for Invalid channel */
#define UDPNM_E_INVALID_CHANNEL                         0x02U

#if (defined UDPNM_E_INVALID_PDUID)
#error UDPNM_E_INVALID_PDUID already defined
#endif
/** \brief API service called with wrong PDU ID. */
#define UDPNM_E_INVALID_PDUID                           0x03U

#if (defined UDPNM_E_NULL_POINTER)
#error UDPNM_E_NULL_POINTER already defined
#endif
/** \brief Error code for NULL pointers */
#define UDPNM_E_NULL_POINTER                            0x12U

#if (defined UDPNM_E_INIT_FAILED)
#error UDPNM_E_INIT_FAILED already defined
#endif
/** \brief UdpNm initialization has failed, e.g. selected configuration set doesn't exist */
#define UDPNM_E_INIT_FAILED                             0x04U

#if (defined UDPNM_E_NETWORKSTARTINDICATION)
#error UDPNM_E_NETWORKSTARTINDICATION already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_NetworkStartIndication fails*/
#define UDPNM_E_NETWORKSTARTINDICATION         250U

#if (defined UDPNM_E_NETWORKMODE)
#error UDPNM_E_NETWORKMODE already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_NetworkMode fails*/
#define UDPNM_E_NETWORKMODE                    249U

#if (defined UDPNM_E_BUSSLEEPMODE)
#error UDPNM_E_BUSSLEEPMODE already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_BusSleepMode fails*/
#define UDPNM_E_BUSSLEEPMODE                   248U

#if (defined UDPNM_E_PREPAREBUSSLEEPMODE)
#error UDPNM_E_PREPAREBUSSLEEPMODE already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_PrepareBusSleepMode fails*/
#define UDPNM_E_PREPAREBUSSLEEPMODE            247U

#if (defined UDPNM_E_REMOTESLEEPINDICATION)
#error UDPNM_E_REMOTESLEEPINDICATION already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_RemoteSleepIndication fails*/
#define UDPNM_E_REMOTESLEEPINDICATION          246U

#if (defined UDPNM_E_REMOTESLEEPCANCELLATION)
#error UDPNM_E_REMOTESLEEPCANCELLATION already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_RemoteSleepCancellation fails*/
#define UDPNM_E_REMOTESLEEPCANCELLATION        245U

#if (defined UDPNM_E_PDURXINDICATION)
#error UDPNM_E_PDURXINDICATION already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_PduRxIndication fails*/
#define UDPNM_E_PDURXINDICATION                244U

#if (defined UDPNM_E_STATECHANGENOTIFICATION)
#error UDPNM_E_STATECHANGENOTIFICATION already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_StateChangeNotification fails*/
#define UDPNM_E_STATECHANGENOTIFICATION        243U

#if (defined UDPNM_E_REPEATMESSAGEINDICATION)
#error UDPNM_E_REPEATMESSAGEINDICATION already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_RepeatMessageIndication fails*/
#define UDPNM_E_REPEATMESSAGEINDICATION        242U

#if (defined UDPNM_E_CARWAKEUPINDICATION)
#error UDPNM_E_CARWAKEUPINDICATION already defined
#endif
/** \brief Error code for case in which SchM_Call for Nm_CarWakeUpIndication fails*/
#define UDPNM_E_CARWAKEUPINDICATION            241U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
#define UDPNM_START_SEC_CODE
#include <UdpNm_MemMap.h>

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
extern FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_IsValidConfig
(
    P2CONST(void, AUTOMATIC, UDPNM_APPL_CONST) voidConfigPtr
);

#ifndef RTE_SCHM_SWCBSW_UDPNM_PASSIVESTARTUP
#define RTE_SCHM_SWCBSW_UDPNM_PASSIVESTARTUP
/** \brief Passive startup of UdpNm module.
 **
 ** This function performs a passive startup of the AUTOSAR SoAd.
 ** It triggers the transition from Bus-Sleep Mode or Prepare Bus Sleep to the Network Mode
 ** in Repeat Message State.
 **
 ** This service has no effect if the current state is not equal to
 ** Bus-Sleep Mode or Prepare Bus Sleep. In that case E_NOT_OK is returned.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Passive startup of network management has
 **            failed/not executed.
 **
 ** \ServiceID{0x0e}
 ** \Reentrancy{Reentrant (But not for the same NM Channel)}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_PassiveStartUp
(
   NetworkHandleType nmChannelHandle
);
#endif /* RTE_SCHM_SWCBSW_UDPNM_PASSIVESTARTUP */

#if (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)
#ifndef RTE_SCHM_SWCBSW_UDPNM_NETWORKREQUEST
#define RTE_SCHM_SWCBSW_UDPNM_NETWORKREQUEST
/** \brief Network Request.
 **
 ** This function request the network when bus communication is needed.
 ** Network state shall be changed to requested.
 **
 ** \param[in] nmChannelHandle Identification of the NM channel.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Requesting of network has failed.
 **
 ** \ServiceID{0x02}
 ** \Reentrancy{Reentrant (But not for the same NM Channel)}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_NetworkRequest
(
   NetworkHandleType nmChannelHandle
);
#endif /* RTE_SCHM_SWCBSW_UDPNM_NETWORKREQUEST */
#ifndef RTE_SCHM_SWCBSW_UDPNM_NETWORKGWERAREQUEST
#define RTE_SCHM_SWCBSW_UDPNM_NETWORKGWERAREQUEST
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
extern FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_NetworkGwEraRequest
(
   NetworkHandleType nmChannelHandle
);
#endif /* RTE_SCHM_SWCBSW_UDPNM_NETWORKGWERAREQUEST */
#ifndef RTE_SCHM_SWCBSW_UDPNM_NETWORKRELEASE
#define RTE_SCHM_SWCBSW_UDPNM_NETWORKRELEASE
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
 ** \ServiceID{0x03}
 ** \Reentrancy{Reentrant (But not for the same NM Channel)}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_NetworkRelease
(
   NetworkHandleType nmChannelHandle
);
#endif /* RTE_SCHM_SWCBSW_UDPNM_NETWORKRELEASE */
#endif

#if (UDPNM_COM_CONTROL_ENABLED == STD_ON)
/** \brief Disable NM PDU transmission.
 **
 ** This function disables the NM PDU transmission ability due to
 ** a ISO14229 Communication Control (28hex) service.
 **
 ** \param[in] nmChannelHandle Identification of the NM channel.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Disabling of NM PDU transmission ability has failed/not executed.
 **
 ** \ServiceID{0x0c}
 ** \Reentrancy{Reentrant (But not for the same NM Channel)}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_DisableCommunication
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
 ** \retval E_NOT_OK Enabling of NM PDU transmission ability has failed/not executed.
 **
 ** \ServiceID{0x0d}
 ** \Reentrancy{Reentrant (But not for the same NM Channel)}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_EnableCommunication
(
   NetworkHandleType nmChannelHandle
);
#endif

/* !LINKSTO EB_SWS_UdpNm_00163_3,1 */
#if ((UDPNM_NODE_DETECTION_ENABLED == STD_ON) && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF))
/* !LINKSTO SWS_UdpNm_00135,1 */
/** \brief Set the Repeat Message Request Bit.
 **
 ** This function sets the Repeat Message Request Bit for NM messages
 ** transmitted next on the bus.
 **
 ** \param[in] nmChannelHandle Identification of the NM channel.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Setting of Repeat Message Request Bit has failed/not executed.
 **
 ** \ServiceID{0x08}
 ** \Reentrancy{Reentrant (but not for the same NM Channel)}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_RepeatMessageRequest
(
   NetworkHandleType nmChannelHandle
);
#endif /* #if (UDPNM_NODE_DETECTION_ENABLED == STD_ON && UDPNM_PASSIVE_MODE_ENABLED == STD_OFF)*/

/** \brief Get the State and mode of the Network Management.
 **
 ** This function returns the state and the mode of the network management.
 **
 ** \param[in]  nmChannelHandle Identification of the NM-channel.
 ** \param[out] nmStatePtr Pointer to state of network management.
 ** \param[out] nmModePtr Pointer to mode of network management.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Getting of NM state has failed.
 **
 ** \ServiceID{0x0b}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_GetState
(
   NetworkHandleType nmChannelHandle,
   P2VAR(Nm_StateType, AUTOMATIC, UDPNM_APPL_DATA) nmStatePtr,
   P2VAR(Nm_ModeType, AUTOMATIC, UDPNM_APPL_DATA) nmModePtr
);

/* !LINKSTO EB_SWS_UdpNm_00163_1,1 */
#if ((UDPNM_BUS_SYNCHRONIZATION_ENABLED == STD_ON) && (UDPNM_PASSIVE_MODE_ENABLED == STD_OFF))
/* !LINKSTO SWS_UdpNm_00185,2 */
#ifndef RTE_SCHM_SWCBSW_UDPNM_REQUESTBUSSYNCHRONIZATION
#define RTE_SCHM_SWCBSW_UDPNM_REQUESTBUSSYNCHRONIZATION
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
 ** \ServiceID{0x014}
 ** \Reentrancy{Reentrant (but not for the same NM Channel)}
 ** \Synchronicity{synchronous}
 */
extern FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_RequestBusSynchronization
(
   NetworkHandleType nmChannelHandle
);
#endif /* RTE_SCHM_SWCBSW_UDPNM_REQUESTBUSSYNCHRONIZATION */
#endif

#if (UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)
/** \brief Check if sleep indication has taken place.
 **
 ** This function checks if remote sleep indication has taken place or not.
 **
 ** \param[in] nmChannelHandle Identification of the NM-channel.
 ** \param[out] nmRemoteSleepIndPtr Pointer where check result
 **                                 of remote sleep indication shall be copied to.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Checking of remote sleep indication bits has failed/not executed.
 **
 ** \ServiceID{0x011}
 ** \Reentrancy{Reentrant (but not for the same NM-Channel)}
 ** \Synchronicity{Asynchronous}
 */
/* !LINKSTO SWS_UdpNm_00149,1 */
extern FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_CheckRemoteSleepIndication
(
   NetworkHandleType nmChannelHandle,
   P2VAR(boolean, AUTOMATIC, UDPNM_APPL_DATA) nmRemoteSleepIndPtr
);
#endif /* #if (UDPNM_REMOTE_SLEEP_IND_ENABLED == STD_ON)*/

/** \brief Initialization of UdpNm module.
 **
 ** Initialize the complete
 ** UdpNm module, i.e. all channels which are activated at configuration
 ** time are initialized.  A UDP socket shall be set up with the TCP/IP stack.
 **
 ** \param[in] udpnmConfigPtr Pointer to selected configuration structure.
 **
 ** \ServiceID{0x01}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, UDPNM_CODE) UdpNm_Init
(
   P2CONST(UdpNm_ConfigType, AUTOMATIC, UDPNM_APPL_DATA) udpnmConfigPtr
);

#if (UDPNM_NODE_ID_ENABLED == STD_ON)
/** \brief Get Node Identifier.
 **
 ** This function gets the node identifier out of the most recently
 ** received NM PDU.
 **
 ** \param[in]  nmChannelHandle Identification of the NM channel.
 ** \param[out] nmNodeIdPtr Pointer where node identifier out of
 **                         the most recently received NM PDU shall be copied to.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Getting of the node identifier out of
 **                  the most recently received NM PDU has failed.
 **
 ** \ServiceID{0x06}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_GetNodeIdentifier
(
   NetworkHandleType nmChannelHandle,
   P2VAR(uint8, AUTOMATIC, UDPNM_APPL_DATA) nmNodeIdPtr
);

/** \brief Get Local Node Identifier.
 **
 ** This function gets the node identifier configured as the local node.
 **
 ** \param[in]  nmChannelHandle Identification of the NM channel.
 ** \param[out] nmNodeIdPtr     Pointer where node identifier of the
 **                             local node shall be copied to.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Getting of the node identifier of the local node has failed.
 **
 ** \ServiceID{0x07}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_GetLocalNodeIdentifier
(
   NetworkHandleType nmChannelHandle,
   P2VAR(uint8, AUTOMATIC, UDPNM_APPL_DATA) nmNodeIdPtr
);
#endif /* (UDPNM_NODE_ID_ENABLED == STD_ON) */

#if (UDPNM_USER_DATA_ENABLED == STD_ON)
/** \brief Get User Data from NM messages.
 **
 ** This function retrieves the user data from the last received NM message.
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should
 **    have been initialized for this channel (checked).
 **
 ** \param[in] nmChannelHandle Identification of the NM channel.
 ** \param[in] nmUserDataPtr Pointer to where user data out of
 **                          the most recently received NM message shall be copied to.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Getting of user data has failed.
 **
 ** \ServiceID{0x05}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_GetUserData
(
   NetworkHandleType nmChannelHandle,
   P2VAR(uint8, AUTOMATIC, UDPNM_APPL_DATA) nmUserDataPtr
);

#if ((UDPNM_PASSIVE_MODE_ENABLED == STD_OFF) && (UDPNM_COM_USER_DATA_SUPPORT == STD_OFF))

/** \brief Set User Data for NM messages.
 **
 ** This function sets the user data for the next NM message that is transmitted on the bus.
 **
 ** Preconditions:
 ** - The channel handle should be valid and the module should
 **     have been initialized for this channel (checked).
 **
 ** \param[in] nmChannelHandle Identification of the NM channel.
 ** \param[in] nmUserDataPtr Pointer where the user data for the
 **                          next transmitted NM message shall be copied from.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Setting of user data has failed.
 **
 ** \ServiceID{0x04}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_SetUserData
(
   NetworkHandleType nmChannelHandle,
   P2CONST(uint8, AUTOMATIC, UDPNM_APPL_DATA) nmUserDataPtr
);
#endif /* ((UDPNM_PASSIVE_MODE_ENABLED == STD_OFF) && (UDPNM_COM_USER_DATA_SUPPORT == STD_OFF)) */
#endif /* (UDPNM_USER_DATA_ENABLED == STD_ON) */

#if (UDPNM_COM_USER_DATA_SUPPORT == STD_ON)

/** \brief Dummy function.
 **
 ** UdpNm_Transmit is implemented as an empty function and shall always return E_OK.
 ** The function UdpNm_Transmit is only available if the configuration switch
 ** UdpNmComUserDataSupport is enabled.
 **
 ** Preconditions:
 ** - None
 **
 ** \param[in] UdpNmTxPduId This parameter contains a unique identifier referencing to the
                            PDU Routing Table and therby specifiying the socket to be
                            used for tranmission of the data.
 ** \param[in] PduInfoPtr   A pointer to a structure with socket related data: data length
 **                         and pointer to a data buffer.
 **
 ** \return Standard Return Code
 ** \retval E_OK:    The request has been accepted
 ** \retval E_NOT_OK: The request has not been accepted, e.g. due to
 **                   a still ongoing transmission in the corresponding socket or
 **                   the to be transmitted message is too long.
 **
 ** \ServiceID{0x015}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_Transmit
(
    PduIdType UdpNmTxPduId,
    P2CONST(PduInfoType, AUTOMATIC, UDPNM_APPL_DATA) PduInfoPtr
);
#endif /* (UDPNM_COM_USER_DATA_SUPPORT == STD_ON) */

#if ((UDPNM_USER_DATA_ENABLED == STD_ON)                                \
   || (UDPNM_NODE_ID_ENABLED == STD_ON)                                 \
   || (UDPNM_NODE_DETECTION_ENABLED == STD_ON))
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
 ** \param[in] nmPduDataPtr    Pointer where NM PDU data
 **                            shall be copied to.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Error.
 ** \retval E_NOT_OK Getting of NM PDU data has failed.
 **
 ** \ServiceID{0x0a}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, UDPNM_CODE) UdpNm_GetPduData
(
   NetworkHandleType nmChannelHandle,
   P2VAR(uint8, AUTOMATIC, UDPNM_APPL_DATA) nmPduDataPtr
);
#endif /* ((UDPNM_USER_DATA_ENABLED == STD_ON)
        * || (UDPNM_NODE_ID_ENABLED == STD_ON)
        * || (UDPNM_NODE_DETECTION_ENABLED == STD_ON)) */

#if (UDPNM_VERSION_INFO_API == STD_ON)
/** \brief This service returns the version information of this module.
 **
 ** This service returns the version information of this module.
 **
 ** \param[out] versioninfo Pointer to where to store the version
 ** information of this module.
 **
 ** \ServiceID{0x09}
 ** \Reentrancy{reentrant}
 ** \Synchronicity{synchronous}
 **/
extern FUNC(void, UDPNM_CODE) UdpNm_GetVersionInfo
(
   P2VAR(Std_VersionInfoType, AUTOMATIC, UDPNM_APPL_DATA) versioninfo
);
#endif /* (UDPNM_VERSION_INFO_API == STD_ON) */
#define UDPNM_STOP_SEC_CODE
#include <UdpNm_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef UDPNM_API_H */
/*==================[end of file]===========================================*/
