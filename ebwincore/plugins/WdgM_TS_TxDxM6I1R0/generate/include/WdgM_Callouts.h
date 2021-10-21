/**
 * \file
 *
 * \brief AUTOSAR WdgM
 *
 * This file contains the implementation of the AUTOSAR
 * module WdgM.
 *
 * \version 6.1.39
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* !LINKSTO WDGM.EB.TIMEPM.WDGM020200,1 */
[!INCLUDE "WdgM_Cfg.m"!][!//
[!CODE!][!//
#ifndef WDGM_CALLOUTS_H
#define WDGM_CALLOUTS_H

/*==================[includes]===================================================================*/
/* !LINKSTO WDGM.EB.Design.IncludeFileStructure,1 */
#include <WdgM_Types.h>
/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

[!IF "$ActivateAliveSupervisionAPIEnabled"!][!//
/** \brief Activate alive supervision of a considered entity via a pre-configured callout API
 *
 * This function exists for ASR32 compatibility and simply redirects
 * the call to a pre-configured API.
 *
 * Note: In ASR32, this API activated the alive supervision of a supervised entity of
 * the active alive supervision configuration set.
 *
 * \param[in] SEId Id of supervised entity whose alive supervision should be activated
 * \return Success of operation
 * \retval E_OK     Operation successful
 * \retval E_NOT_OK Operation failed
 */
extern FUNC(Std_ReturnType, WDGM_CODE) [!"$ActivateAliveSupervisionAPIName"!]
(
  WdgM_SupervisedEntityIdType SEId
);

[!ENDIF!][!//
[!IF "$DeactivateAliveSupervisionAPIEnabled"!][!//
/** \brief Deactivate alive supervision of a considered entity via a pre-configured callout API
 *
 * This function exists for ASR32 compatibility and simply redirects
 * the call to a pre-configured API.
 *
 * Note: In ASR32, this API deactivated the alive supervision of a supervised entity of
 * the active alive supervision configuration set.
 *
 * \param[in] SEId Id of supervised entity whose alive supervision should be deactivated
 * \return Success of operation
 * \retval E_OK     Operation successful
 * \retval E_NOT_OK Operation failed
 */
extern FUNC(Std_ReturnType, WDGM_CODE) [!"$DeactivateAliveSupervisionAPIName"!]
(
  WdgM_SupervisedEntityIdType SEId
);

[!ENDIF!][!//
[!IF "$GetExpectedInitStateCalloutEnabled"!][!//
/** \brief Provide actual / get expected initialization state
 *
 * The callout is invoked at the beginning of WdgM_MainFunction cycle.
 * Possible values for InitStatus:
 * WDGM_EB_INIT_STATUS_INIT   The WdgM shall be initialized,
 *                         respectively stay initialized.
 * WDGM_EB_INIT_STATUS_DEINIT The WdgM shall be de-initialized,
 *                         respectively stay de-initialized.
 *
 * \param[inout] InitStatus The caller of this API (WdgM)
 *               provides the current initial status,
 *               the expected initial status for the
 *               WdgM shall be returned in case E_OK is returned.
 *
 * \return Success of operation
 * \retval E_OK     The returned value in InitStatus is valid.
 *                  The WdgM changes to the expected state.
 * \retval E_NOT_OK The returned value is not valid and will be ignored.
 *                  WdgM continues normal operation.
 */
extern FUNC(Std_ReturnType, WDGM_CODE) [!"$GetExpectedInitStateCalloutName"!]
(
  P2VAR(WdgM_EB_InitStatusType, AUTOMATIC, WDGM_APPL_DATA) InitStatus
);

[!ENDIF!][!//
[!IF "$InitRedirectionCalloutAPIEnabled"!][!//
/** \brief Redirected callout API for WdgM_Init
 **
 ** \param[in] ConfigPtr Pointer to configuration data, this parameter is
 ** ignored in the current implementation.
 **
 ** \ServiceID{0x00}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, WDGM_CODE) [!"$InitRedirectionCalloutAPIName"!]
(
  P2CONST(WdgM_ConfigType, AUTOMATIC, WDGM_APPL_CONST) ConfigPtr
);

[!ENDIF!][!//
[!IF "$DeInitRedirectionCalloutAPIEnabled"!][!//
/** \brief Redirected callout API for WdgM_DeInit
 **
 ** \ServiceID{0x01}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, WDGM_CODE) [!"$DeInitRedirectionCalloutAPIName"!]
(
  void
);

[!ENDIF!][!//
[!IF "$GetExpectedWdgMModeCalloutEnabled"!][!//
/** \brief Provide actual / get expected WdgM Mode
 *
 * The callout is invoked at the beginning
 * of WdgM_MainFunction cycle if WdgM is initialized and the prior
 * call to WdgMGetExpectedInitStateCallout
 * also returned WDGM_EB_INIT_STATUS_INIT.
 *
 * \param[inout] WdgMMode The caller of this API (WdgM)
 *               provides the current mode,
 *               the expected WdgM mode
 *               shall be returned in case E_OK is returned.
 *
 * \return Success of operation
 * \retval E_OK     The WdgM shall perform a mode switch
 *                  to the mode stored in the argument WdgMMode.
 * \retval E_NOT_OK The returned value is not valid and will be ignored.
 *                  WdgM continues normal operation.
 */
extern FUNC(Std_ReturnType, WDGM_CODE) [!"$GetExpectedWdgMModeCalloutName"!]
(
  P2VAR(WdgM_ModeType, AUTOMATIC, WDGM_APPL_DATA) WdgMMode
);

[!ENDIF!][!//
[!IF "$SetModeRedirectionCalloutAPIEnabled"!][!//
[!IF "$BswCompatibilityMode = 'AUTOSAR_40'"!][!//
/** \brief Redirected callout API for WdgM_SetMode
 **
 ** \param[in] Mode  One of the configured Watchdog Manager modes
 ** \param[in] CallerID  Module ID of the calling module
 ** \return Success of operation
 ** \retval E_OK     Successfully changed to the new mode
 ** \retval E_NOT_OK Changing to the new mode failed
 **
 ** \ServiceID{0x03}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, WDGM_CODE) [!"$SetModeRedirectionCalloutAPIName"!]
(
  WdgM_ModeType Mode,
  uint16 CallerID
);

[!ELSE!][!//
/** \brief Redirected callout API for WdgM_SetMode
 **
 ** \param[in] Mode  One of the configured Watchdog Manager modes
 ** \return Success of operation
 ** \retval E_OK     Successfully changed to the new mode
 ** \retval E_NOT_OK Changing to the new mode failed
 **
 ** \ServiceID{0x03}
 ** \Reentrancy{Non reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, WDGM_CODE) [!"$SetModeRedirectionCalloutAPIName"!]
(
  WdgM_ModeType Mode
);

[!ENDIF!][!//
[!ENDIF!][!//
[!IF "$GetTimeCalloutEnabled"!][!//
/** \brief Get elapsed time
 *
 * \param[inout] PreviousTime The old time is passed in order to calculate the
 *               difference with respect to the actual time. The actual time is
 *               returned via this variable.
 * \param[out] ElapsedTime The elapsed time with respect to the time passed via parameter
 *             \a PreviousTime.
 */
extern FUNC(void, WDGM_CODE) [!"$GetTimeCalloutName"!]
(
  P2VAR(uint32, AUTOMATIC, WDGM_APPL_DATA) PreviousTime,
  P2VAR(uint32, AUTOMATIC, WDGM_APPL_DATA) ElapsedTime
);

[!ENDIF!][!//
[!IF "$IsPerformResetCalloutEnabled"!][!//
/** \brief Get authorization for direct reset
 *
 * \return Returns if the caller of the WdgM Perform Reset API is authorized.
 * \retval E_OK     Caller of Perform Reset is authorized
 *                  and therefore reset request will be executed
 *                  by setting all triggering conditions to 0.
 * \retval E_NOT_OK Caller of Perform Reset is not authorized
 *                  and therefore reset request will be ignored.
 *                  WdgM continues normal operation.
 */
extern FUNC(Std_ReturnType, WDGM_CODE) [!"$IsPerformResetCalloutName"!]
(
  void
);

[!ENDIF!][!//
[!IF "$SupervisionExpiredCalloutEnabled"!][!//
/** \brief Indicate an expired Supervised Entity
 *
 * \param[in] ExpiredSEID Supervised Entity ID that expired
 *                        (one of Deadline Supervision, Logical Supervision, or
 *                         Alive Supervision failed).
 */
extern FUNC(void, WDGM_CODE) [!"$SupervisionExpiredCalloutName"!]
(
  WdgM_SupervisedEntityIdType ExpiredSEID
);
[!ENDIF!][!//
[!IF "$IndividualModeSwitchCalloutEnabled"!][!//
/** \brief Indicate a state transition of a Supervised Entity
 *
 * \param[in] SEID    Supervised Entity ID that performed a mode switch
 * \param[in] OldMode Old WdgMMode of the SEID
 * \param[in] NewMode New WdgMMode of the SEID
 */
extern FUNC(void, WDGM_CODE) [!"$IndividualModeSwitchCalloutName"!]
(
  WdgM_SupervisedEntityIdType SEID,
  WdgM_LocalStatusType OldMode,
  WdgM_LocalStatusType NewMode
);

[!ENDIF!][!//
[!IF "$GlobalModeSwitchCalloutEnabled"!][!//
/** \brief Indicate a state transition of the global Supervision State
 *
 * \param[in] OldMode Old global WdgMMode
 * \param[in] NewMode New global WdgMMode
 */
extern FUNC(void, WDGM_CODE) [!"$GlobalModeSwitchCalloutName"!]
(
  WdgM_GlobalStatusType OldMode,
  WdgM_GlobalStatusType NewMode
);

[!ENDIF!][!//
[!IF "$DetCalloutEnabled"!][!//
/** \brief Indicate an internal error
 *
 * \param[in] SID     ID of the API where an internal error was detected
 * \param[in] ErrorID Internal Error Type
 */
extern FUNC(void, WDGM_CODE) [!"$DetCalloutName"!]
(
  uint8 SID,
  uint8 ErrorID
);

[!ENDIF!][!//

[!IF "$GetCoreIdCalloutEnabled"!][!//
/** \brief The function returns a unique core identifier.
 * \param[in] None
 * \return    The return value is the unique ID of the core.
 */
extern FUNC(WdgM_EB_CoreIdType, WDGM_CODE) [!"$GetCoreIdCalloutNameExtern"!];

[!ENDIF!][!//

/*==================[external constants]=========================================================*/

/*==================[external data]==============================================================*/

#endif /* ifndef WDGM_CALLOUTS_H */
/*==================[end of file]================================================================*/
[!ENDCODE!][!//
