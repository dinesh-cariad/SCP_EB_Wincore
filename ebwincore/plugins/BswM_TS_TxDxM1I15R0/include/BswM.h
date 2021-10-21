#ifndef BSWM_H
#define BSWM_H
/**
 * \file
 *
 * \brief AUTOSAR BswM
 *
 * This file contains the implementation of the AUTOSAR
 * module BswM.
 *
 * \version 1.15.6
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[includes]===============================================*/
/* !LINKSTO BswM0025,1 */
#include <ComStack_Types.h>   /* AUTOSAR Com Stack header           */

#include <BswM_Version.h>     /* This modules' version declarations */

#include <BswM_Types.h>

#ifndef BSWM_NO_CFG_REQUIRED
#define BSWM_NO_CFGCLASSMIX_REQUIRED
#include <BswM_Cfg.h>         /* Generated module configuration     */
#endif

#include <BswM_CfgTypes.h>

/*==================[macros]=================================================*/

/*------------------[macros for service IDs]---------------------------------*/
#if (defined BSWM_SID_INIT)
#error BSWM_SID_INIT already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_Init(). */
#define BSWM_SID_INIT                              0x00U

#if (defined BSWM_SID_GET_VERSION_INFO)
#error BSWM_SID_GET_VERSION_INFO already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_GetVersionInfo(). */
#define BSWM_SID_GET_VERSION_INFO                  0x01U

#if (defined BSWM_SID_REQUESTMODE)
#error BSWM_SID_REQUESTMODE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for bswm_requestmode(). */
#define BSWM_SID_REQUESTMODE                       0x02U

#if (defined BSWM_SID_MAINFUNCTION)
#error BSWM_SID_MAINFUNCTION already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_MainFunction(). */
#define BSWM_SID_MAINFUNCTION                      0x03U

#if (defined BSWM_SID_DEINIT)
#error BSWM_SID_DEINIT already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_Deinit(). */
#define BSWM_SID_DEINIT                            0x04U

#if (defined BSWM_SID_CANSM_CURRENT_STATE)
#error BSWM_SID_CANSM_CURRENT_STATE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_CanSM_CurrentState(). */
#define BSWM_SID_CANSM_CURRENT_STATE               0x05U

#if (defined BSWM_SID_DCM_COMMUNICATIONMODE_CURRENT_STATE)
#error BSWM_SID_DCM_COMMUNICATIONMODE_CURRENT_STATE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_Dcm_CommunicationMode_CurrentState(). */
#define BSWM_SID_DCM_COMMUNICATIONMODE_CURRENT_STATE    0x06U

#if (defined BSWM_SID_DCM_REQUEST_COMMUNICATION_MODE)
#error BSWM_SID_DCM_REQUEST_COMMUNICATION_MODE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_Dcm_RequestCommunicationMode(). */
#define BSWM_SID_DCM_REQUEST_COMMUNICATION_MODE    0xFEU

#if (defined BSWM_SID_DCM_REQUEST_RESET_MODE)
#error BSWM_SID_DCM_REQUEST_RESET_MODE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_Dcm_RequestResetMode(). */
#define BSWM_SID_DCM_REQUEST_RESET_MODE            0x07U

#if (defined BSWM_SID_DCM_REQUEST_SESSION_MODE)
#error BSWM_SID_DCM_REQUEST_SESSION_MODE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_Dcm_RequestSessionMode(). */
#define BSWM_SID_DCM_REQUEST_SESSION_MODE          0x08U


#if (defined BSWM_SID_LINSM_CURRENT_STATE)
#error BSWM_SID_LINSM_CURRENT_STATE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_LinSM_CurrentState(). */
#define BSWM_SID_LINSM_CURRENT_STATE               0x09U

#if (defined BSWM_SID_LINSM_CURRENT_SCHEDULE)
#error BSWM_SID_LINSM_CURRENT_SCHEDULE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_LinSM_CurrentSchedule(). */
#define BSWM_SID_LINSM_CURRENT_SCHEDULE            0x0AU

#if (defined BSWM_SID_LINTP_REQUESTMODE)
#error BSWM_SID_LINTP_REQUESTMODE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_LinTp_RequestMode(). */
#define BSWM_SID_LINTP_REQUESTMODE                 0x0BU

#if (defined BSWM_SID_FRSM_CURRENT_STATE)
#error BSWM_SID_FRSM_CURRENT_STATE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_FrSM_CurrentState(). */
#define BSWM_SID_FRSM_CURRENT_STATE                0x0CU

#if (defined BSWM_SID_ETHSM_CURRENT_STATE)
#error BSWM_SID_ETHSM_CURRENT_STATE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_EthSM_CurrentState(). */
#define BSWM_SID_ETHSM_CURRENT_STATE               0x0DU

#if (defined BSWM_SID_COMM_CURRENT_MODE)
#error BSWM_SID_COMM_CURRENT_MODE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_ComM_CurrentMode(). */
#define BSWM_SID_COMM_CURRENT_MODE                 0x0EU

#if (defined BSWM_SID_ECUM_CURRENT_STATE)
#error BSWM_SID_ECUM_CURRENT_STATE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_EcuM_CurrentState(). */
#define BSWM_SID_ECUM_CURRENT_STATE                0x0FU

#if (defined BSWM_SID_ECUM_CURRENT_WAKEUP)
#error BSWM_SID_ECUM_CURRENT_WAKEUP already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_EcuM_CurrentWakeup(). */
#define BSWM_SID_ECUM_CURRENT_WAKEUP               0x10U

#if (defined BSWM_SID_WDGM_REQUEST_PARTITION_RESET)
#error BSWM_SID_WDGM_REQUEST_PARTITION_RESET already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_WdgM_RequestPartitionReset(). */
#define BSWM_SID_WDGM_REQUEST_PARTITION_RESET      0x11U

#if (defined BSWM_SID_TRIGGER_STARTUP_PHASE2)
#error BSWM_SID_TRIGGER_STARTUP_PHASE2 already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_TriggerStartUpPhase2(). */
#define BSWM_SID_TRIGGER_STARTUP_PHASE2            0x12U

#if (defined BSWM_SID_TRIGGER_SLAVE_RTE_STOP)
#error BSWM_SID_TRIGGER_SLAVE_RTE_STOP already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_TriggerSlaveRTEStop(). */
#define BSWM_SID_TRIGGER_SLAVE_RTE_STOP            0x13U

#if (defined BSWM_SID_DCM_APPLICATION_UPDATED)
#error BSWM_SID_DCM_APPLICATION_UPDATED already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_Dcm_ApplicationUpdated(). */
#define BSWM_SID_DCM_APPLICATION_UPDATED    0x14U

#if (defined BSWM_SID_COMM_CURRENT_PNC_MODE)
#error BSWM_SID_COMM_CURRENT_PNC_MODE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_ComM_CurrentPNCMode(). */
#define BSWM_SID_COMM_CURRENT_PNC_MODE             0x15U

#if (defined BSWM_SID_NVM_CURRENT_BLOCK_MODE)
#error BSWM_SID_NVM_CURRENT_BLOCK_MODE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_NvM_CurrentBlockMode(). */
#define BSWM_SID_NVM_CURRENT_BLOCK_MODE            0x16U

#if (defined BSWM_SID_NVM_CURRENT_JOB_MODE)
#error BSWM_SID_NVM_CURRENT_JOB_MODE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_NvM_CurrentJobMode(). */
#define BSWM_SID_NVM_CURRENT_JOB_MODE            0x17U

#if (defined BSWM_SID_SD_CLIENTSERVICE_CURRENTSTATE)
#error BSWM_SID_SD_CLIENTSERVICE_CURRENTSTATE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_Sd_ClientServiceCurrentState(). */
#define BSWM_SID_SD_CLIENTSERVICE_CURRENTSTATE          0x1FU

#if (defined BSWM_SID_SD_EVENTHANDLER_CURRENTSTATE)
#error BSWM_SID_SD_EVENTHANDLER_CURRENTSTATE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_Sd_EventHandlerCurrentState(). */
#define BSWM_SID_SD_EVENTHANDLER_CURRENTSTATE           0x20U

#if (defined BSWM_SID_SD_CONSUMEDEVENTGROUP_CURRENTSTATE)
#error BSWM_SID_SD_CONSUMEDEVENTGROUP_CURRENTSTATE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_Sd_ConsumedEventGroupCurrentState(). */
#define BSWM_SID_SD_CONSUMEDEVENTGROUP_CURRENTSTATE     0x21U

#if (defined BSWM_SID_ETHIF_PORTGROUPLINKSTATECHG)
#error BSWM_SID_ETHIF_PORTGROUPLINKSTATECHG already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_EthIf_PortGroupLinkStateChg(). */
#define BSWM_SID_ETHIF_PORTGROUPLINKSTATECHG 0x26U

#if (defined BSWM_SID_NM_CAR_WAKEUP_INDICATION)
#error BSWM_SID_NM_CAR_WAKEUP_INDICATION already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_NmIf_CarWakeUpIndication(). */
#define BSWM_SID_NM_CAR_WAKEUP_INDICATION 0x24U

#if (defined BSWM_SID_J1939DCMBROADCASTSTATUS)
#error BSWM_SID_J1939DCMBROADCASTSTATUS already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_J1939DcmBroadcastStatus(). */
#define BSWM_SID_J1939DCMBROADCASTSTATUS      0x1BU

#if (defined BSWM_SID_J1939NM_STATECHANGENOTIFICATION)
#error BSWM_SID_J1939NM_STATECHANGENOTIFICATION already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_J1939Nm_StateChangeNotification(). */
#define BSWM_SID_J1939NM_STATECHANGENOTIFICATION      0x18U

#if (defined BSWM_SID_COMM_INITIATE_RESET)
#error BSWM_SID_COMM_INITIATE_RESET already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for BswM_ComM_InitiateReset(). */
#define BSWM_SID_COMM_INITIATE_RESET 0x22U

#if (defined BSWM_SID_INVALID)
#error BSWM_SID_INVALID already defined
#endif
#define BSWM_SID_INVALID 0xFFU

/*------------------[error codes]--------------------------------------------*/

#if (defined BSWM_E_NO_INIT)
#error BSWM_E_NO_INIT already defined
#endif
/** \brief Error Code.
 **
 ** A service was called prior to initialization. */
#define BSWM_E_NO_INIT                0x01U

#if (defined BSWM_E_NULL_POINTER)
#error BSWM_E_NULL_POINTER already defined
#endif
/** \brief Error Code.
 **
 ** A null pointer was passed as an argument. */
#define BSWM_E_NULL_POINTER           0x02U

#if (defined BSWM_E_INVALID_PAR)
#error BSWM_E_INVALID_PAR already defined
#endif
/** \brief Error Code.
 **
 ** A parameter was invalid. */
#define BSWM_E_INVALID_PAR            0x03U

#if (defined BSWM_E_REQ_USER_OUT_OF_RANGE)
#error BSWM_E_REQ_USER_OUT_OF_RANGE already defined
#endif
/** \brief Error Code.
 **
 ** A requesting user was out of range. */
#define BSWM_E_REQ_USER_OUT_OF_RANGE  0x04U

#if (defined BSWM_E_REQ_MODE_OUT_OF_RANGE)
#error BSWM_E_REQ_MODE_OUT_OF_RANGE already defined
#endif
/** \brief Error Code.
 **
 ** A requested mode was out of range. */
#define BSWM_E_REQ_MODE_OUT_OF_RANGE  0x05U

#if (defined BSWM_E_PARAM_CONFIG )
#error BSWM_E_PARAM_CONFIG  already defined
#endif
/** \brief Error Code.
 **
 ** The provided configuration is inconsistent. */
#define BSWM_E_PARAM_CONFIG           0x06U

#if (defined BSWM_E_PARAM_POINTER)
#error BSWM_E_PARAM_POINTER already defined
#endif
/** \brief Error Code.
 **
 ** A parameter pointer was invalid. */
#define BSWM_E_PARAM_POINTER          0x07U

#if (defined BSWM_E_WRONG_CONTEXT)
#error BSWM_E_WRONG_CONTEXT already defined
#endif
/** \brief Error code.
 **
 ** The API has been called from an invalid partition
 ** context. */
#define BSWM_E_WRONG_CONTEXT 0x08U

#if (defined BSWM_E_QUEUE_FULL)
#error BSWM_E_QUEUE_FULL already defined
#endif
/** \brief Error Code.
 **
 ** A new mode request needs to be queued but the mode arbitration queue
 ** has reached its maximum capacity.
 */
#define BSWM_E_QUEUE_FULL 0x09U

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

#define BSWM_START_SEC_CODE
#include <BswM_MemMap.h>

/** \brief Initializes the BSW Mode Manager
 **
 ** This routine initializes the BSW Mode Manager. After execution of this
 ** routine the BSW Mode Manager is ready to arbitrate incoming mode requests
 **
 ** Precondition: None
 **
 ** \param[in]    ConfigPtr - Pointer to a selected configuration structure.
 **
 ** \ServiceID{0}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, BSWM_CODE) BswM_Init
(
  P2CONST(BswM_ConfigType, AUTOMATIC, BSWM_APPL_CONST) ConfigPtr
);

#if (BSWM_VERSION_INFO_API == STD_ON)

/** \brief Return the modules version information
 **
 ** This function provides the information to module vendor ID, module ID and
 ** software version major.minor.patch
 **
 ** Precondition: ::BSWM_VERSION_INFO_API = ::STD_ON
 **
 ** \param[out]    VersionInfoPtr - Pointer to struct to be filled with the
 **                                 version information.
 ** \ServiceID{1}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, BSWM_CODE) BswM_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, BSWM_APPL_DATA) VersionInfoPtr
);

#endif /* (BSWM_VERSION_INFO_API == STD_ON) */


/** \brief Deinitializes the BSW Mode Manager
 **
 ** After a call of BswM_Deinit no mode processing shall be performed by BswM
 ** even if any mode requests are made or the BswM main function is called
 **
 ** Precondition: None
 **
 ** \ServiceID{4}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, BSWM_CODE) BswM_Deinit
(
  void
);

#if (BSWM_GENERIC_REQUEST_API_ENABLED == STD_ON)
/** \brief Generic function call to request modes
 **
 ** This function is used to request modes and shall only be used by other
 ** BSW modules that does not have a specific mode request interface.
 **
 ** Precondition: None
 **
 ** \param[in]     requesting_user The user that requests the mode
 ** \param[in]     requested_mode  The requested mode.
 **
 ** \ServiceID{2}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, BSWM_CODE) BswM_RequestMode
(
  BswM_UserType requesting_user,
  BswM_ModeType requested_mode
) ;
#endif /* (BSWM_GENERIC_REQUEST_API_ENABLED == STD_ON) */

/* * \brief Starts phase two on a slave core
 * *
 * * This function starts a OS task on the slave core which
 * * starts the Scheduler Manager and the RTE.
 * *
 * * Precondition: None
 * *
 * * \param[in]    CoreID - The identifier of the slave core.
 * * \param[out]   None.
 * *
 * * \return success of operation
 * * \retval E_OK      - Task has been started correctly
 * * \retval E_OS_CORE - Error during task creation
 * *
 * * \ServiceID{0x12}
 * * \Reentrancy{Reentrant}
 * * \Synchronicity{Synchronous} */

/* Not supported in this version
extern FUNC(Std_ReturnType, BSWM_CODE) BswM_TriggerStartUpPhase2
(
  CoreIdType CoreID
);
*/

/* * \brief Stops the RTE of a slave core
 * *
 * * This function starts a OS task on the slave core
 * * which stops the RTE.
 * *
 * * Precondition: None
 * *
 * * \param[in]    CoreID - The identifier of the slave core.
 * * \param[out]   None.
 * *
 * * \return success of operation
 * * \retval E_OK      - Task has been started correctly
 * * \retval E_OS_CORE - Error during task creation
 * *
 * * \ServiceID{0x13}
 * * \Reentrancy{Reentrant}
 * * \Synchronicity{Synchronous} */

/* Not supported in this release
extern FUNC(Std_ReturnType, BSWM_CODE) BswM_TriggerSlaveRTEStop
(
  CoreIdType CoreID
) ;
*/

/* !LINKSTO BswM.Impl.IsValid,1 */

/** \brief Validates the post-build configuration data structure passed to the
 ** BswM_Init function.
 **
 **
 ** \param voidPtr The pointer to a BswM post-build data structure.  If a
 ** NULL_PTR is passed, the BswM will attempt to retrieve the BswM post-build
 ** configuration from the PbcfgM module.
 ** \retval E_OK When the pre-compile, link-time and platform hash values stored
 ** within the post-build structure correspond to the hash values of the
 ** compiled source files.  Otherwise, E_NOT_OK will be returned. */
FUNC(Std_ReturnType, BSWM_CODE) BswM_IsValidConfig
(
  P2CONST(void, AUTOMATIC, BSWM_APPL_CONST) voidPtr
);

#define BSWM_STOP_SEC_CODE
#include <BswM_MemMap.h>

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef BSWM_H */
/*==================[end of file]============================================*/
