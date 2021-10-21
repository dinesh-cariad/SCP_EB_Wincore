/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef DCM_DSL_ROESERVICES_H
#define DCM_DSL_ROESERVICES_H

/* !LINKSTO Dcm.Dsn.File.RoeServices.PublicApi,1 */
/* This file contains the public API of the ROE services software unit. */

/*==================[inclusions]=================================================================*/

#include <Dcm_Int.h>

#if (DCM_DSP_USE_ROESERVICES == STD_ON)

/*==================[macros]=====================================================================*/

#if (defined DCM_DSP_SVCID_RESPONSEONEVENT)
  #error "DCM_DSP_SVCID_RESPONSEONEVENT is already defined"
#endif
/** \brief Service ID for ROE */
#define DCM_DSP_SVCID_RESPONSEONEVENT   0x86U

/** \brief Persistence flag for ROE operations - persistent */
#if (defined DCM_ROE_STORE_EVENT)
  #error "DCM_ROE_STORE_EVENT is already defined"
#endif
#define DCM_ROE_STORE_EVENT 1U

/** \brief Persistence flag for ROE operations - non persistent */
#if (defined DCM_ROE_DO_NOT_STORE_EVENT)
  #error "DCM_ROE_DO_NOT_STORE_EVENT is already defined"
#endif
#define DCM_ROE_DO_NOT_STORE_EVENT 0U

/** \brief Definition of bit mask of a ROE Persistence bit */
#if (defined DCM_ROE_PERSISTENCE_BIT)
  #error "DCM_ROE_PERSISTENCE_BIT is already defined"
#endif
#define DCM_ROE_PERSISTENCE_BIT 0x40U

/** \brief Subservice ID for stopResponseOnEvent */
#if (defined DCM_ROE_SUBSERVICE_STOPRESPONSEONEVENT)
  #error "DCM_ROE_SUBSERVICE_STOPRESPONSEONEVENT is already defined"
#endif
#define DCM_ROE_SUBSERVICE_STOPRESPONSEONEVENT       0x00U

/** \brief Subservice ID for stopResponseOnEvent */
#if (defined DCM_ROE_SUBSERVICE_ONDTCSTATUSCHANGE)
  #error "DCM_ROE_SUBSERVICE_ONDTCSTATUSCHANGE is already defined"
#endif
#define DCM_ROE_SUBSERVICE_ONDTCSTATUSCHANGE       0x01U

/** \brief Subservice ID for onChangeOfDataIdentifier */
#if (defined DCM_ROE_SUBSERVICE_ONCHANGEOFDATAIDENTIFIER)
  #error "DCM_ROE_SUBSERVICE_ONCHANGEOFDATAIDENTIFIER is already defined"
#endif
#define DCM_ROE_SUBSERVICE_ONCHANGEOFDATAIDENTIFIER  0x03U

/** \brief Subservice ID for reportActivatedEvents */
#if (defined DCM_ROE_SUBSERVICE_REPORTACTIVATEDEVENTS)
  #error "DCM_ROE_SUBSERVICE_REPORTACTIVATEDEVENTS is already defined"
#endif
#define DCM_ROE_SUBSERVICE_REPORTACTIVATEDEVENTS     0x04U

/** \brief Subservice ID for startResponseOnEvent */
#if (defined DCM_ROE_SUBSERVICE_STARTRESPONSEONEVENT)
  #error "DCM_ROE_SUBSERVICE_STARTRESPONSEONEVENT is already defined"
#endif
#define DCM_ROE_SUBSERVICE_STARTRESPONSEONEVENT      0x05U

/** \brief Subservice ID for clearResponseOnEvent */
#if (defined DCM_ROE_SUBSERVICE_CLEARRESPONSEONEVENT)
  #error "DCM_ROE_SUBSERVICE_CLEARRESPONSEONEVENT is already defined"
#endif
#define DCM_ROE_SUBSERVICE_CLEARRESPONSEONEVENT      0x06U

/** \brief Service ID of the ReadDTCInformtion request */
#if (defined DCM_RDTCI_REQUEST_SID)
  #error "DCM_RDTCI_REQUEST_SID is already defined"
#endif
#define DCM_RDTCI_REQUEST_SID                        0x19U

/** \brief Sub-Service ID of the ReadDTCInformtion request */
#if (defined DCM_RDTCI_REQUEST_SSID)
  #error "DCM_RDTCI_REQUEST_SSID is already defined"
#endif
#define DCM_RDTCI_REQUEST_SSID                       0x0EU

/** \brief Length of the ReadDataByIdentifier request with one DID */
#if (defined DCM_RDTCI_REQUEST_LENGTH)
  #error "DCM_RDTCI_REQUEST_LENGTH is already defined"
#endif
#define DCM_RDTCI_REQUEST_LENGTH                     2U

/** \brief Service ID of the ReadDataByIdentifier request */
#if (defined DCM_RDBI_REQUEST_SID)
  #error "DCM_RDBI_REQUEST_SID is already defined"
#endif
#define DCM_RDBI_REQUEST_SID                         0x22U

/** \brief Length of the ReadDataByIdentifier request with one DID */
#if (defined DCM_RDBI_REQUEST_LENGTH)
  #error "DCM_RDBI_REQUEST_LENGTH is already defined"
#endif
#define DCM_RDBI_REQUEST_LENGTH                          3U

/*==================[type definitions]===========================================================*/

/* !LINKSTO SWS_Dcm_00871,1 */
typedef uint8 Dcm_RoeEventStateType;

/* GENERATED SECTION BEGIN EA:{B189A266-02B6-4d04-9DA8-753415988FD1} */
/** \brief Dcm_RoeEventStateType_Values: The ROE event is in CLEARED state **/
#if (defined DCM_ROE_CLEARED)
  #error "DCM_ROE_CLEARED is already defined"
#endif
#define DCM_ROE_CLEARED ((uint8)0U)

/** \brief Dcm_RoeEventStateType_Values: The ROE event is in STARTED state **/
#if (defined DCM_ROE_STARTED)
  #error "DCM_ROE_STARTED is already defined"
#endif
#define DCM_ROE_STARTED ((uint8)2U)

/** \brief Dcm_RoeEventStateType_Values: The ROE event is in STOPPED state **/
#if (defined DCM_ROE_STOPPED)
  #error "DCM_ROE_STOPPED is already defined"
#endif
#define DCM_ROE_STOPPED ((uint8)1U)

/* GENERATED SECTION END   EA:{B189A266-02B6-4d04-9DA8-753415988FD1} */

typedef uint8 Dcm_RoeEventTypeType;

/* GENERATED SECTION BEGIN EA:{8577ADA9-29FD-4694-8F29-615D8F11F09E} */

#if (defined DCM_ROE_EVENT_DTC)
  #error "DCM_ROE_EVENT_DTC is already defined"
#endif
#define DCM_ROE_EVENT_DTC ((uint8)0U)

#if (defined DCM_ROE_EVENT_DID)
  #error "DCM_ROE_EVENT_DID is already defined"
#endif
#define DCM_ROE_EVENT_DID ((uint8)1U)

#if (defined DCM_ROE_EVENT_MAX)
  #error "DCM_ROE_EVENT_MAX is already defined"
#endif
#define DCM_ROE_EVENT_MAX ((uint8)2U)

/* GENERATED SECTION END   EA:{8577ADA9-29FD-4694-8F29-615D8F11F09E} */

typedef uint8 RoeEventWindowTimeType;

/* GENERATED SECTION BEGIN EA:{2ACDA2A9-AD97-400d-9707-9606C42A0A34} */

#if (defined DCM_ROE_EVENT_WINDOW_CURRENT_AND_FOLLOWING_CYCLE)
  #error "DCM_ROE_EVENT_WINDOW_CURRENT_AND_FOLLOWING_CYCLE is already defined"
#endif
#define DCM_ROE_EVENT_WINDOW_CURRENT_AND_FOLLOWING_CYCLE ((uint8)4U)

#if (defined DCM_ROE_EVENT_WINDOW_CURRENT_CYCLE)
  #error "DCM_ROE_EVENT_WINDOW_CURRENT_CYCLE is already defined"
#endif
#define DCM_ROE_EVENT_WINDOW_CURRENT_CYCLE ((uint8)3U)

#if (defined DCM_ROE_EVENT_WINDOW_INFINITE)
  #error "DCM_ROE_EVENT_WINDOW_INFINITE is already defined"
#endif
#define DCM_ROE_EVENT_WINDOW_INFINITE ((uint8)2U)

#if (defined DCM_ROE_EVENT_WINDOW_EXPIRED)
  #error "DCM_ROE_EVENT_WINDOW_EXPIRED is already defined"
#endif
#define DCM_ROE_EVENT_WINDOW_EXPIRED ((uint8)0U)

/* GENERATED SECTION END   EA:{2ACDA2A9-AD97-400d-9707-9606C42A0A34} */

typedef struct
{
  RoeEventWindowTimeType EventWindowTime;
  boolean                StorageState;
} Dcm_RoeEventWindowTimePreconfType;

typedef struct
{
  uint16                Did;
  Dcm_RoeEventStateType InitialEventStatus;
  Dcm_RoeEventTypeType  EventType;
  uint8                 DTCStatusMask;
} Dcm_RoeEventPreconfListType;

typedef struct
{
  /* !LINKSTO Dcm.Dsn.RoeServices.SM,1 */
  Dcm_RoeEventStateType  State;
  Dcm_SesCtrlType        Session;
  RoeEventWindowTimeType EventWindowTime;
  RoeEventWindowTimeType OrigEventWindowTime;
  PduIdType              RxPduId;
  uint8                  PreconfIdx;
  uint8                  ActivationCnt;
} Dcm_RoeEventDataType;

typedef struct
{
  /** \brief buffer handle used for the internal request for the ServiceToRespondTo */
  Dcm_BufferType  RxBuffer;
  /** \brief dedicated ROE data buffer for reception */
  Dcm_MsgItemType MsgBuf[3U];
  /** \brief PduId used for the ROE request */
  PduIdType RxPduId;
} Dcm_RoeRxDataType;

#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RoeSchMSwitchFunctionType)(uint8 state);
#endif

/*==================[external constants]=========================================================*/

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>
/** \brief Preconfigured values of the ROE events */
extern CONST(Dcm_RoeEventPreconfListType, DCM_CONST)
  Dcm_RoeEventPreconfList[DCM_ROE_NUM_PRECONF_EVENT];

/** \brief Preconfigured values of the ROE eventWindowTime */
extern CONST(Dcm_RoeEventWindowTimePreconfType, DCM_CONST)
  Dcm_RoeEventWindowTimePreconfList[DCM_ROE_NUM_PRECONF_EVENT_WINDOW_TIMES];

#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
/** \brief SchMSwitch functions for the preconfigured ROE events */
extern CONST(Dcm_RoeSchMSwitchFunctionType, DCM_CONST)
  Dcm_RoeSchMSwitchFunctionList[DCM_ROE_NUM_PRECONF_EVENT];
#endif

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[external data]==============================================================*/
#if (DCM_ROE_PERSISTENCE == STD_ON)
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief persistent data for ROE
 ** This structure holds the ROE data that needs to be restored after power cycle.
 ** The structure is stored by NvM WriteAll at shutdown, and restored by NvM ReadAll at startup.
 */
extern VAR(Dcm_RoeEventDataType, DCM_VAR) Dcm_Dsl_RoeServices_Persistent_Data[DCM_ROE_EVENT_MAX];

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>
#endif /* (DCM_ROE_PERSISTENCE == STD_ON) */

/*==================[external function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Initializes the ROE services
 **
 ** Initializes the internal ROE variables.
 ** The function is called by Dcm_Init
 **
 ** \param  none
 ** \return none
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_RoeServices_Init(void);

/** \brief Returns the initialization state of the ROE services
 **
 ** \param  none
 ** \return none
 **
 ** \return boolean
 ** \retval TRUE             The ROE service is initialized
 ** \retval FALSE            The ROE service is not initialized
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(boolean, DCM_CODE) Dcm_Dsl_RoeServices_IsInitialized(void);

/** \brief Returns the RxPduId that was used to start ROE services
 **
 ** Based on the stored RxPduId the transfer layer knows where to send the ServiceToRespontTo
 ** requests.
 **
 ** \param[out] RxPduId      The RxPduId that was used to start ROE services
 **
 ** \return Std_ReturnType
 ** \retval E_OK             At least one ROE event is started
 ** \retval E_NOT_OK         No ROE event is started
 **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_RoeServices_GetRxPduId(
  P2VAR(PduIdType, AUTOMATIC, DCM_VAR) RxPduId);

/** \brief Handles events related ROE
 **
 ** This function is called in each cycle of the DCM main function.
 ** It processes the events occurred since the last call.
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, DCM_CODE) Dcm_Dsl_RoeServices_MainFunction(void);

/** \brief Callback function for session change
 **
 ** This function is called when the Session is changed.
 ** Session changes can trigger state changes in ROE events.
 **
 ** \param[in]  OldSession   The old session
 ** \param[in]  NewSession   The new session
 ** \return none
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, DCM_CODE) Dcm_Dsl_RoeServices_SessionChange(
  Dcm_SesCtrlType OldSession, Dcm_SesCtrlType NewSession);

/** \brief Triggers the ServiceToResponseTo request message
 **
 ** This function contains the real functionality of Dcm_TriggerOnEvent API function exposed to the
 ** SWC or CDD. It creates the request message and passes it to ResponseOnOneEvent function which
 ** will try to send to the server as an internal request.
 **
 ** \param[in] RoeEventId     The ID of the event that is triggered
 **
 ** \return Std_ReturnType
 ** \retval E_OK              RoeEventId value is valid
 ** \retval E_NOT_OK          RoeEventId value is not valid
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_RoeServices_TriggerOnEvent(uint8 RoeEventId);

/** \brief Triggers the ServiceToResponseTo request message for OnDTCStatusChange events.
 **
 ** Triggers on changes of the UDS DTC status byte. Allows to trigger on ROE Event
 ** for subservice OnDTCStatusChanged.
 **
 ** \param[in] DTC                This is the DTC the change trigger is assigned to
 ** \param[in] DTCStatusOld       DTC status before change
 ** \param[in] DTCStatusNew       DTC status after change
 **
 ** \return Std_ReturnType
 ** \retval E_OK                 this value is always returned
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, DCM_CODE) Dcm_Dsl_RoeServices_TriggerOnDTCStatus(uint8 DTCStatusNew);

/** \brief Clears the started and stopped events.
 **
 ** This function contains the functionality associated with the clearing of the events
 ** and the reset of the associated process status of the ROE service.
 **
 ** \param[in] ServiceCall    When the function is invoked from the service handler, the value
 **                           shall be TRUE. When called from the RoeServices unit, the value
 **                           shall be FALSE.
 **
 ** \return none
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/

extern FUNC(void, DCM_CODE) Dcm_Dsl_RoeServices_ClearEvents(boolean ServiceCall);
#if ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x00 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x40 == STD_ON))
/** \brief Handles the ROE stop event
 **
 ** \param[in] StoreEvent     If the value contains TRUE, the event is stopped persistently,
 **                           otherwise if other conditions are fulfilled, the event will be
 **                           restarted in the next operational cycle.
 **
 ** \return Std_ReturnType
 ** \retval DCM_E_OK          The stop request is processed in the regular way.
 ** \retval DCM_E_REQUESTSEQUENCEERROR   All RoeEvents are in 'ROE cleared' state
 **
 **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_RoeServices_StopResponseOnEvent(boolean StoreEvent);
#endif /* ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x00 == STD_ON) || \
           (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x40 == STD_ON)) */

#if (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x04 == STD_ON)
/** \brief Reports the activated events.
 **
 ** \param[out] ResData       Message buffer.
 **
 ** \param[out] MsgLen        Length of data written to the message buffer.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, DCM_CODE) Dcm_Dsl_RoeServices_ReportActivatedEvents(
  Dcm_MsgType ResData, P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) MsgLen);
#endif /* (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x04 == STD_ON) */

#if ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x05 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x45 == STD_ON))
/** \brief Handles the ROE start event
 **
 ** \param[in] StoreEvent     If the value contains TRUE, the event is started persistently,
 **                           otherwise the event will not be restarted in the next operational
 **                           cycle.
 ** \param[in] RxPduId        The sourceAddress (PduId) where the request was received.
 **
 ** \return Std_ReturnType
 ** \retval DCM_E_OK          The start request is processed in the regular way.
 ** \retval DCM_E_REQUESTSEQUENCEERROR   All RoeEvents are in 'ROE cleared' state
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_RoeServices_StartResponseOnEvent(
  boolean StoreEvent, PduIdType RxPduId);
#endif /* ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x05 == STD_ON) || \
           (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x45 == STD_ON)) */

#if ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x01 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x41 == STD_ON))
/** \brief Handles the ROE OnDTCStatusChange setup request
**
** \param[in] StoreEvent                   If the value contains TRUE, the event is started
**                                         persistently, otherwise the event will not be
**                                         restarted in the next operational cycle.
** \param[in] DTCMask                      The new DTC mask to monitor DTC changes.
** \param[in] EventWindowTime              Event window time from the request.
**
** \return Std_ReturnType
** \retval DCM_E_OK                        The start request is processed in the regular way.
** \retval DCM_E_REQUESTOUTOFRANGE         Event set for configuration has not been pre-configured.
**
** \Reentrancy{Non-Reentrant}
** \Synchronicity{Synchronous}
**/

extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_RoeServices_SetupEvent_DTCStatusChange(
  boolean StoreEvent, uint8 DTCMask, uint8 EventWindowTime);
#endif /*#if ((DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x01 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x41 == STD_ON))*/

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* (DCM_DSP_USE_ROESERVICES == STD_ON) */

#endif /* ifndef DCM_DSL_ROESERVICES_H */
/*==================[end of file]================================================================*/
