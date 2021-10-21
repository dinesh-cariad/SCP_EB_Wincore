/* --------{ EB Automotive C Source File }-------- */

/* !LINKSTO Dcm.Dsn.File.ComMChannelStateManager.PublicApi,1 */
/* This file contains the Public API of the ComMChannelStateManager software
   unit as well as data types and macro definitions used by this unit. */

#ifndef DCM_DSL_COMMCHANNELSTATEMANAGER_EXTERNAL_INTERFACE_H
#define DCM_DSL_COMMCHANNELSTATEMANAGER_EXTERNAL_INTERFACE_H

/*==================[inclusions]=================================================================*/

#include <ComStack_Types.h>                                           /* AUTOSAR Com Stack types */

#include <Dcm_Version.h>                                          /* Module version declarations */

#include <Dcm_Types.h>                                                    /* DCM type definition */

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* hide declaration in own header if included by DCM itself and RTE is
 * enabled */
#if (DCM_INCLUDE_RTE == STD_OFF)

#if (DCM_DSL_ENABLE_SET_ACTIVE_DIAGNOSIS == STD_ON)
/** \brief Allows to activate and deactivate the call of ComM_DCM_ActiveDiagnostic() function.
 *
 *  \param  [in] active    true   do call the ComM_DCM_ActiveDiagnostic() function
 *                         false  do NOT call the ComM_DCM_ActiveDiagnostic() function
 *
 *  \return Std_ReturnType
 *  \retval E_OK                   Operation succeeded (is always returned)
 *
 ** \ServiceID{::DCM_SERVID_SETACTIVEDIAGNOSTIC}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_SetActiveDiagnostic(boolean active);
#endif /* #if (DCM_DSL_ENABLE_SET_ACTIVE_DIAGNOSIS == STD_ON) */

#endif /* #if (DCM_INCLUDE_RTE == STD_OFF) */

#if (DCM_COMM_MULTICORE_ENABLED == STD_OFF)

/** \brief Notification of No Communication Mode
 **
 ** All kind of transmissions (receive and transmit) are be stopped immediately. This means that
 ** the ResponseOnEvent and PeriodicId and also the transmission of the normal communication
 ** (PduR_DcmTransmit) are be disabled.
 **
 ** \param[in] NetworkId   Identifier of the network concerned by the mode change
 **
 ** \ServiceID{::DCM_SERVID_COMMNOCOMMODEENTERED}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, DCM_CODE) Dcm_ComM_NoComModeEntered(uint8 NetworkId);

/** \brief Notification of Silent Communication Mode
 **
 ** All outgoing transmissions are be stopped immediately. This means that the ResponseOnEvent and
 ** PeriodicId and also the transmission of the normal communication (PduR_DcmTransmit)
 ** are be disabled.
 **
 ** \param[in] NetworkId   Identifier of the network concerned by the mode change
 **
 ** \ServiceID{::DCM_SERVID_COMMSILENTCOMMODEENTERED}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, DCM_CODE) Dcm_ComM_SilentComModeEntered(uint8 NetworkId);

/** \brief Notification of Full Communication Mode
 **
 ** All kind of transmissions shall be enabled immediately. This means that the ResponseOnEvent and
 ** PeriodicId and also the transmission of the normal communication (PduR_DcmTransmit)
 ** shall be enabled.
 **
 ** \param[in] NetworkId   Identifier of the network concerned by the mode change
 **
 ** \ServiceID{::DCM_SERVID_COMMFULLCOMMODEENTERED}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, DCM_CODE) Dcm_ComM_FullComModeEntered(uint8 NetworkId);

#endif /* (DCM_COMM_MULTICORE_ENABLED == STD_OFF) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_DSL_COMMCHANNELSTATEMANAGER_EXTERNAL_INTERFACE_H */
/*==================[end of file]================================================================*/
