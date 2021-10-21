/* --------{ EB Automotive C Source File }-------- */

#ifndef DCM_DSL_DIAGNOSTIC_SESSION_HANDLER_H
#define DCM_DSL_DIAGNOSTIC_SESSION_HANDLER_H

/* !LINKSTO Dcm.Dsn.File.DiagnosticSessionHandler.PublicApi,1 */
/* This file contains the public API of the Diagnostic Session Handler software unit. */

/*==================[inclusions]=================================================================*/

#include <Dcm_Int.h>      /* Declaration of Dcm internals */
#include <Dcm_Types.h>    /* DCM type definition */

/*==================[macros]=====================================================================*/

#define DCM_P2TIMER        0x02U
#define DCM_S3TIMER        0x03U

#define DCM_RESETTODEFAULTSESSIONREQUESTER_APPLICATION    0x02U
#define DCM_RESETTODEFAULTSESSIONREQUESTER_SUPERVISOR     0x01U
#define DCM_RESETTODEFAULTSESSIONREQUESTER_UNDEFINED      0x00U

/* Macro for flag index of service ID 0x10 */
#if (defined DCM_SVC_FLAGDIAGSESCTRL)
  #error "DCM_SVC_FLAGDIAGSESCTRL is already defined"
#endif
#define DCM_SVC_FLAGDIAGSESCTRL             (0x00U)

/*  Macro for flag index of service ID 0x28 */
#if (defined DCM_SVC_FLAGCOMMCTRL)
  #error "DCM_SVC_FLAGCOMMCTRL is already defined"
#endif
#define DCM_SVC_FLAGCOMMCTRL                (0x01U)

/*  Macro for flag index of service ID 0x27 */
#if (defined DCM_SVC_FLAGSECACCESS)
  #error "DCM_SVC_FLAGSECACCESS is already defined"
#endif
#define DCM_SVC_FLAGSECACCESS                (0x02U)

/* Macro for number of flags for service ID which resets to default session */
#if (defined DCM_NUM_FLAGSFORSVCID)
  #error "DCM_NUM_FLAGSFORSVCID is already defined"
#endif
#define DCM_NUM_FLAGSFORSVCID               (0x03U)


#if (defined DCM_DIAGSESSIONHANDLER_ID)
  #error "DCM_DIAGSESSIONHANDLER_ID is already defined"
#endif
#define DCM_DIAGSESSIONHANDLER_ID (Dcm_BufferUserType)((DCM_SUPERVISOR_ID) + 2U)

/*==================[type definitions]===========================================================*/

typedef uint8   Dcm_RequesterIdType;
typedef uint8   Dcm_TimerIdType;
typedef uint32  Dcm_TimerValueType;
typedef uint16  Dcm_S3TimerValueType;

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief This function initializes the diagnostic session handler.
 *
 *  \return void
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_Init(void);


/** \brief This function executes the diagnostic session handler part for handling protocol timers
 *         in the MainFunction context.
 *  \return void
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_P2TimerHandlingMainFunction(void);


/** \brief This function executes the diagnostic session handler part for handling session timer
 *         in the MainFunction context.
 *  \return void
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_S3TimerHandlingMainFunction(void);


/** \brief This function provides the maximum number of RCRRP for the session identified by the
 *         protocol index.
 *  \param[in] ProtocolIndex identifies the protocol's session for which MaxRcrrp value shall
 *             be returned.
 *  \return uint8
 */
extern FUNC(uint16, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_GetSessionMaxRcrrp(
  Dcm_ProtocolIndexType ProtocolIndex);


/** \brief This function provides the P2StarMaxTime for the session identified by the
 *         protocol index.
 *  \param[in] ProtocolIndex identifies the protocol's session for which P2StarMaxTime value
 *             shall be returned.
 *  \return Dcm_TimerValueType
 */
extern FUNC(Dcm_TimerValueType, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_GetP2StarMaxTime(
  Dcm_ProtocolIndexType ProtocolIndex);


/** \brief This function provides the P2MaxTime for the session identified by the protocol index.
 *
 *  \param[in] ProtocolIndex identifies the protocol's session for which P2MaxTime value shall be returned.
 *  \return Dcm_TimerValueType
 */
extern FUNC(Dcm_TimerValueType, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_GetP2MaxTime(
  Dcm_ProtocolIndexType ProtocolIndex);


/** \brief This function provides the current diagnostic session.
 *
 *  \return Dcm_SesCtrlType
 */
extern FUNC(Dcm_SesCtrlType, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_GetCurrentSession(
  void);


/** \brief This function provides the index of the protocol,
 *         - which changed the session to non-default.
 *         - DCM_INVALID_INDEX otherwise.
 *
 *  \return Dcm_ProtocolIndexType
 */
extern FUNC(Dcm_ProtocolIndexType, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_GetNonDefaultSessionProtocolIndex(
  void);


/** \brief This function provides the index of the MainConnection,
 *         - which changed the session to non-default.
 *         - DCM_INVALID_MAINCONNECTION_INDEX otherwise.
 *
 *  \return Dcm_MainConnectionIndexType
 */
extern FUNC(Dcm_MainConnectionIndexType, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_GetNonDefaultSessionMainConnection(void);


/** \brief This function triggers a diagnostic session switch to DCM_DEFAULT_SESSION.
 *         The session switch is executed in MainFunction context.
 *  \param[in] RequesterId identifies requester of session switch.
 *  \return void
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_ResetToDefaultSession(
       Dcm_RequesterIdType RequesterId);


/** \brief This function sets the current diagnostic session to the new session.
 *
 *  \param[in] MainConnectionIndex identifies the MainConnection, which changes the session.
 *  \param[in] NewSession identifies the new session which shall be set.
 *  \return void
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_SetSession(Dcm_MainConnectionIndexType MainConnectionIndex,
                                                                        Dcm_SesCtrlType NewSession);


#if( (DCM_DSP_USE_SERVICE_0X10 == STD_ON) || \
     (DCM_EXTSVC_ENABLED == STD_ON) || \
     (DCM_JUMP_TO_BL_ENABLED == STD_ON) )

/** \brief This function checks the conditions for switching the current diagnostic
 *         session to the new session requested by UDS Service 0x10.
 *
 *  \param[in] NewSession identifies the new session which shall be set.
 *
 *  \return void
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_SwitchCurrentSession(
       Dcm_SesCtrlType NewSession);

#endif

/** \brief This function starts the protocol timer. The timer expires and stops when it decrements
 *         to zero.
 *  \param[in] UserId It becomes the current owner of the protocol timer.
 *                    The owner is notified when the protocol timer expires.
 *                    The owner can give the protocol timer.
 *                    The owner can stop the protocol timer.
 *                    The owner can be the supervisor or the protocol.
 *  \param[in] ProtocolIndex It identifies the protocol which timer shall be started.
 *  \param[in] Timeout Identifies the start value at which the protocol timer starts decrementing.
 *                     A start value of zereo forces the protocol timer to expire immediately.
 *  \return void
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_StartP2Timer(
  Dcm_BufferUserType UserId,
  Dcm_ProtocolIndexType ProtocolIndex,
  Dcm_TimerValueType Timeout);


/** \brief This function starts the session timer. The timer expires and stops when it decrements
 *         to zero.
 *  \param[in] Timeout Identifies the start value at which the session timer starts decrementing.
 *                     A start value of zero does not start the timer.
 *  \return void
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_StartS3Timer(
  Dcm_TimerValueType Timeout);


/** \brief This function stops the selected timer.
 *
 *  \param[in] UserId is applicable for protocol timers (P2Server/P2*Server) only.
 *                    It shall be the current owner of the protocol timer.
 *                    The owner can be the supervisor or the protocol.
 *  \param[in] ProtocolIndex is applicable for protocol timers (P2Server/P2*Server) only.
 *                           It identifies the protocol which timer shall be stopped.
 *  \param[in] TimerId identifies the timer. Supported timer IDs are
 *                     - DCM_P2TIMER for protocol timers (P2Server/P2*Server)
 *                     - DCM_S3TIMER for session timer (S3Server)
 *  \return void
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_StopTimer(
  Dcm_BufferUserType UserId,
  Dcm_ProtocolIndexType ProtocolIndex,
  Dcm_TimerIdType TimerId);


/** \brief This function changes the ownership of the selected protocol timer.
 *
 *  \param[in] ProviderId It shall be the current owner of the protocol timer.
 *
 *  \param[in] ReceiverId It becomes the new owner of the protocol timer.
 *                        The owner is notified when the protocol timer expires.
 *                        The owner can give the protocol timer.
 *                        The owner can start the protocol timer.
 *                        The owner can stop the protocol timer.
 *                        The owner can be the diagnostic session handler, supervisor
 *                        or the protocol.
 *  \param[in] ProtocolIndex It identifies the protocol timer which ownership shall be changed.
 *  \return void
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_GiveP2Timer(
  Dcm_BufferUserType ProviderId,
  Dcm_BufferUserType ReceiverId,
  Dcm_ProtocolIndexType ProtocolIndex);


/** \brief This function allocates the protocol timer.
 *
 *  \param[in] RequesterId The requester can be the supervisor or the protocol.
 *  \param[in] ProtocolIndex It identifies the protocol which timer shall be allocated.
 *  \return void
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_AllocateP2Timer(
  Dcm_BufferUserType RequesterId,
  Dcm_ProtocolIndexType ProtocolIndex);


/** \brief This function releases the protocol timer.
 *
 *  \param[in] ReleaserId The releaser can be the supervisor or the protocol.
 *  \param[in] ProtocolIndex It identifies the protocol which timer shall be released.
 *  \return void
 */
extern FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_FreeP2Timer(
  Dcm_BufferUserType ReleaserId,
  Dcm_ProtocolIndexType ProtocolIndex);


/** \brief This function provides the current value of the selected protocol timer.
 *
 *  \param[in] ProtocolIndex is applicable for protocol timers (P2Server/P2*Server) only.
 *                           It identifies the protocol timer which current value shall
 *                           be returned.
 *  \param[in] TimerId identifies the timer. Supported timer IDs are - DCM_P2TIMER
 *                     for protocol timers (P2Server/P2*Server)
 *  \return Dcm_TimerValueType
 */
extern FUNC(Dcm_TimerValueType, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_GetTimerValue(
  Dcm_ProtocolIndexType ProtocolIndex,
  Dcm_TimerIdType TimerId);


/** \brief This function clears the indication of reset to default session execution.
 *         This function shall be called before each service processing is started.
 *  \param[in] ServiceID identifies the service. Supported service IDs are
 *             - DCM_SID_SESSION_CONTROL for UDS Service DiagnosticSessionControl (0x10)
 *             - DCM_SID_COMMUNICATIONCONTROL for UDS Service CommunicationControl (0x28) and
 *             - DCM_SID_SECURITYACCESS for UDS Service SecurityAccess (0x27).
 *  \return void
 */
extern FUNC(void, DCM_CODE)
       Dcm_Dsl_DiagnosticSessionHandler_ClearResetToDefaultSessionOccured(uint8 ServiceID);


/** \brief This function indicates if a reset to default session was executed since the service
 *         is running.
 *  \param[in] ServiceID identifies the service. Supported service IDs are
 *             - DCM_SID_SESSION_CONTROL for UDS Service DiagnosticSessionControl (0x10)
 *             - DCM_SID_COMMUNICATIONCONTROL for UDS Service CommunicationControl (0x28) and
 *             - DCM_SID_SECURITYACCESS for UDS Service SecurityAccess (0x27).
 *  \return boolean
 */
extern FUNC(boolean, DCM_CODE)
       Dcm_Dsl_DiagnosticSessionHandler_GetResetToDefaultSessionOccured(uint8 ServiceID);


/** \brief This function indicates if a reset to default session was executed in context
 *         of protocol preemption.
 *
 *  \return boolean
 */
extern FUNC(boolean, DCM_CODE)
       Dcm_Dsl_DiagnosticSessionHandler_GetPreemptionOccured(void);


#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_DSL_DIAGNOSTIC_SESSION_HANDLER_H */
/*==================[end of file]================================================================*/
