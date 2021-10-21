/**
 * \file
 *
 * \brief AUTOSAR ComM
 *
 * This file contains the implementation of the AUTOSAR
 * module ComM.
 *
 * \version 5.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef COMM_BSW_H
#define COMM_BSW_H

/*==================[inclusions]============================================*/

#include <ComStack_Types.h> /* Autosar standard types */
/* !LINKSTO ComM280,1 */
#include <ComM_Version.h>   /* Module version */
#include <ComM_BSW_Types.h> /* ComM's BSW type definitions */
#include <ComM_Types.h>
#include <ComM_Types_Int.h> /* PostBuild type definitions */

/*==================[macros]================================================*/

#if (defined COMM_INSTANCE_ID)
#error COMM_INSTANCE_ID already defined
#endif
/** \brief Id of instance of ComM */
#if (COMM_MULTICORE_ENABLED == STD_ON)
#define COMM_INSTANCE_ID                 COMM_MASTER_CORE_ID
#else
#define COMM_INSTANCE_ID                 0U
#endif


/*------------------[macros for service IDs]--------------------------------*/

#if (defined COMM_SID_INIT)
#error COMM_SID_INIT already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_Init(). */
#define COMM_SID_INIT                    1U

#if (defined COMM_SID_DEINIT)
#error COMM_SID_DEINIT already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_DeInit(). */
#define COMM_SID_DEINIT                  2U

#if (defined COMM_SID_GET_STATE)
#error COMM_SID_GET_STATE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_GetState(). */
#define COMM_SID_GET_STATE              52U

#if (defined COMM_SID_GET_STATUS)
#error COMM_SID_GET_STATUS already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_GetStatus(). */
#define COMM_SID_GET_STATUS              3U

#if (defined COMM_SID_GET_INHIBITION_STATUS)
#error COMM_SID_GET_INHIBITION_STATUS already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_GetInhibitionStatus(). */
#define COMM_SID_GET_INHIBITION_STATUS   4U

#if (defined COMM_SID_REQUEST_COM_MODE)
#error COMM_SID_REQUEST_COM_MODE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_GetRequestComMode(). */
#define COMM_SID_REQUEST_COM_MODE        5U

#if (defined COMM_SID_GET_MAX_COM_MODE)
#error COMM_SID_GET_MAX_COM_MODE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_GetMaxComMode(). */
#define COMM_SID_GET_MAX_COM_MODE        6U

#if (defined COMM_SID_GET_REQUESTED_COM_MODE)
#error COMM_SID_GET_REQUESTED_COM_MODE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_GetRequestedComMode(). */
#define COMM_SID_GET_REQUESTED_COM_MODE  7U

#if (defined COMM_SID_GET_CURRENT_COM_MODE)
#error COMM_SID_GET_CURRENT_COM_MODE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_GetCurrentComMode(). */
#define COMM_SID_GET_CURRENT_COM_MODE    8U

#if (defined COMM_SID_GET_CURRENT_PNC_COM_MODE)
#error COMM_SID_GET_CURRENT_PNC_COM_MODE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_GetCurrentPNCComMode(). */
#define COMM_SID_GET_CURRENT_PNC_COM_MODE    254U

#if (defined COMM_SID_PREVENT_WAKE_UP)
#error COMM_SID_PREVENT_WAKE_UP already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_PreventWakeUp(). */
#define COMM_SID_PREVENT_WAKE_UP         9U

#if (defined COMM_SID_LIMIT_CHANNEL_TO_NO_COM)
#error COMM_SID_LIMIT_CHANNEL_TO_NO_COM already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_LimitChannelToNoComMOde(). */
#define COMM_SID_LIMIT_CHANNEL_TO_NO_COM 11U

#if (defined COMM_SID_LIMIT_ECU_TO_NO_COM)
#error COMM_SID_LIMIT_ECU_TO_NO_COM already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_LimitECUToNoComMode(). */
#define COMM_SID_LIMIT_ECU_TO_NO_COM    12U

#if (defined COMM_SID_READ_INHIBIT_COUNTER)
#error COMM_SID_READ_INHIBIT_COUNTER already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_ReadInhibitCounter(). */
#define COMM_SID_READ_INHIBIT_COUNTER   13U

#if (defined COMM_SID_RESET_INHIBIT_COUNTER)
#error COMM_SID_RESET_INHIBIT_COUNTER already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_ResetInhibitCounter(). */
#define COMM_SID_RESET_INHIBIT_COUNTER  14U

#if (defined COMM_SID_SET_ECU_GROUP_CLASS)
#error COMM_SID_SET_ECU_GROUP_CLASS already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_SetECUGroupClassification(). */
#define COMM_SID_SET_ECU_GROUP_CLASS    15U

#if (defined COMM_SID_GET_VERSION_INFO)
#error COMM_SID_GET_VERSION_INFO already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_GetVersionInfo(). */
#define COMM_SID_GET_VERSION_INFO       16U

#if (defined COMM_SID_MAIN_FUNCTION)
#error COMM_SID_MAIN_FUNCTION already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for ComM_MainFunction_<Channel_ID>(). */
#define COMM_SID_MAIN_FUNCTION  0x60U

/** \brief Slave Data Set SID */
#if (defined COMM_SID_SLAVE_DATA_SET)
#error COMM_SID_SLAVE_DATA_SET already defined
#endif
#define COMM_SID_SLAVE_DATA_SET        250U

/** \brief Slave Data Get SID */
#if (defined COMM_SID_SLAVE_DATA_GET)
#error COMM_SID_SLAVE_DATA_GET already defined
#endif
#define COMM_SID_SLAVE_DATA_GET        249U

/** \brief Master Data Set SID */
#if (defined COMM_SID_MASTER_DATA_SET)
#error COMM_SID_MASTER_DATA_SET already defined
#endif
#define COMM_SID_MASTER_DATA_SET       248U

/** \brief Master Data Get SID */
#if (defined COMM_SID_MASTER_DATA_GET)
#error COMM_SID_MASTER_DATA_GET already defined
#endif
#define COMM_SID_MASTER_DATA_GET       247U

/*------------------[error codes]-------------------------------------------*/

#if (defined COMM_E_NOT_INITED)
#error COMM_E_NOT_INITED already defined
#endif
/** \brief Error Code.
 **
 ** API service used without module initialization. */
#define COMM_E_NOT_INITED                1U

#if (defined COMM_E_WRONG_PARAMETERS)
#error COMM_E_WRONG_PARAMETERS already defined
#endif
/** \brief Error Code.
 **
 ** API service used with wrong parameters, e.g. a NULL pointer. */
#define COMM_E_WRONG_PARAMETERS          2U

/*------------------[extended values of Std_ReturnType]-----------------------*/

#if (defined COMM_E_MODE_LIMITATION)
#error COMM_E_MODE_LIMITATION already defined
#endif

/** \brief Function call has been successfully but mode can not be
 *  granted because of mode inhibition */
#define COMM_E_MODE_LIMITATION             0x02U


#if (defined COMM_E_UNINIT)
#error COMM_E_UNINIT already defined
#endif

/** \brief Return value when ComM not initialized */
#define COMM_E_UNINIT                      3U

#if (defined COMM_E_MULTIPLE_PNC_ASSIGNED)
#error COMM_E_MULTIPLE_PNC_ASSIGNED already defined
#endif

/* !LINKSTO ComM.EB.SWS_ComM_00649.COMM_E_MULTIPLE_PNC_ASSIGNED,1 */
/** \brief Return value when ComM when multiple PNCs are assigned to a user */
#define COMM_E_MULTIPLE_PNC_ASSIGNED       3U

#if (defined COMM_E_NO_PNC_ASSIGNED)
#error COMM_E_NO_PNC_ASSIGNED already defined
#endif

/* !LINKSTO ComM.EB.SWS_ComM_00649.COMM_E_NO_PNC_ASSIGNED,1 */
/** \brief Return value when ComM when no PNCs are assigned to a user */
#define COMM_E_NO_PNC_ASSIGNED             4U

/** \brief Slave Data Set error Id */
#if (defined COMM_E_SLAVE_DATA_SET)
#error COMM_E_SLAVE_DATA_SET already defined
#endif

#define COMM_E_SLAVE_DATA_SET                      250U

/** \brief Slave Data Get error Id */
#if (defined COMM_E_SLAVE_DATA_GET)
#error COMM_E_SLAVE_DATA_GET already defined
#endif

#define COMM_E_SLAVE_DATA_GET                      249U

/** \brief Master Data Set error Id */
#if (defined COMM_E_MASTER_DATA_SET)
#error COMM_E_MASTER_DATA_SET already defined
#endif

#define COMM_E_MASTER_DATA_SET                     248U

/** \brief Master Data Get error Id */
#if (defined COMM_E_MASTER_DATA_GET)
#error COMM_E_MASTER_DATA_GET already defined
#endif

#define COMM_E_MASTER_DATA_GET                     247U

/** \brief Master BusSm Request Com Mode error Id */
#if (defined COMM_E_BUSSM_REQUESTCOMMODE)
#error COMM_E_BUSSM_REQUESTCOMMODE already defined
#endif
#define COMM_E_BUSSM_REQUESTCOMMODE                246U

/** \brief Dcm Full Com Entered Notification error Id */
#if (defined COMM_E_DCM_COMM_FULLCOMMODEENTERED)
#error COMM_E_DCM_COMM_FULLCOMMODEENTERED already defined
#endif
#define COMM_E_DCM_COMM_FULLCOMMODEENTERED         245U

/** \brief Dcm Silent Com Entered Notification error Id */
#if (defined COMM_E_DCM_COMM_SILENTCOMMODEENTERED)
#error COMM_E_DCM_COMM_SILENTCOMMODEENTERED already defined
#endif
#define COMM_E_DCM_COMM_SILENTCOMMODEENTERED       244U

/** \brief Dcm No Com Entered Notification error Id */
#if (defined COMM_E_DCM_COMM_NOCOMMODEENTERED)
#error COMM_E_DCM_COMM_NOCOMMODEENTERED already defined
#endif
#define COMM_E_DCM_COMM_NOCOMMODEENTERED           243U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>

/** \brief Initialize the ComM module.
 **
 ** This function initializes the ComM module by setting all internal
 ** state variables to defined values.
 **
 **Precondition:
 ** This API function has to be called before any other function
 ** (except ComM_GetStatus()) can be called.
 **
 ** \param[in]    ConfigPtr - Pointer to a selected configuration structure.
 **
 ** \ServiceID{::COMM_SID_INIT}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, COMM_CODE) ComM_Init
(
    P2CONST(ComM_ConfigType, AUTOMATIC, COMM_APPL_CONST) ConfigPtr
);

/** \brief Deinitialize ComM module.
 **
 ** This function deinitializes (terminates) the ComM module. De-init
 ** shall only be performed if all channels controlled by ComM are in
 ** "No Communication" state. The function call will be ignored if the
 ** state is not "No Communication".
 **
 ** \ServiceID{::COMM_SID_DEINIT}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, COMM_CODE) ComM_DeInit
(
  void
);

/** \brief Return current state.
 **
 ** This function returns current state, including sub-state, of the ComM state machine.
 **
 ** \pre Channel state is in the range of ComM_StateType
 ** \param[in]  Channel
 ** \param[out] State (state of ComM.)
 **
 ** \return Standard Return Code
 ** \retval E_OK Function Successfully return current state of ComM state machine.
 ** \retval E_NOT_OK Return of current state of ComM state machine failed .
 ** \retval COMM_E_UNINIT ComM not initialized
 **
 ** \ServiceID{::COMM_SID_GET_STATE}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_GetState
(
  NetworkHandleType Channel,
  P2VAR(ComM_StateType, AUTOMATIC, COMM_APPL_DATA) State
);

/** \brief Return the initialization status of the ComM module.
 **
 ** This function returns the initialization status of the ComM module.
 **
 ** \pre ComM_InitStatus hold COMM_UNINIT or COMM_INIT
 **
 ** \param[out] Status Current initialization status of ComM.
 **
 ** \return Standard Return Code
 ** \retval E_OK Function serviced successfully.
 ** \retval E_NOT_OK Function execution failed.
 **
 ** \ServiceID{::COMM_SID_GET_STATUS}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_GetStatus
(
  P2VAR(ComM_InitStatusType, AUTOMATIC, COMM_APPL_DATA) Status
);

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
/** \brief Retrieve inhibition status.
 **
 ** This function returns the inhibition status of a ComM channel.
 **
 ** \param[in] Channel Network channel on which the mode transition
 **                    has occurred.
 ** \param[out] Status Current inhibition status of ComM.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Getting of Inhibition Status failed.
 **
 ** \ServiceID{::COMM_SID_GET_INHIBITION_STATUS}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_GetInhibitionStatus
(
  NetworkHandleType Channel,
  P2VAR(ComM_ASR40_InhibitionStatusType, AUTOMATIC, COMM_APPL_DATA) Status
);
#endif


/** \brief Request of a communication mode by a user.
 **
 ** This function requests the given communication mode for the given user.
 **
 ** If Rte usage is enables in the ComM configuration the declaration of this
 ** function is provided by \verbatim Rte_ComM.h\endverbatim.
 **
 ** \param[in] User Handle of the user requesting the communication mode.
 ** \param[in] ComMode Desired communication mode.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Requesting of Communication Mode failed.
 **
 ** \ServiceID{::COMM_SID_REQUEST_COM_MODE}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_RequestComMode
(
  ComM_ASR40_UserHandleType User,
  ComM_ASR40_ModeType ComMode
);

/** \brief Retrieve the maximum allowed communication mode.
 **
 ** This function retrieves the maximum allowed communication
 ** mode of the corresponding user.
 **
 ** \param[in] User Handle of the user for which the query shall
 **                be performed.
 ** \param[out] ComMode Maximum allowed communication mode.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Getting of Maximum Communication
 **            Mode failed.
 **
 ** \ServiceID{::COMM_SID_GET_MAX_COM_MODE}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_GetMaxComMode
(
  ComM_ASR40_UserHandleType                             User,
  P2VAR(ComM_ASR40_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
);

/** \brief Retrieve the communication mode requested by a user.
 **
 ** This function retrieves the communication mode that has been
 ** requested most recently by the given user.
 **
 ** \pre       Communication mode is valid
 ** \param[in] User Handle of the user for which the query shall be
 **                performed.
 ** \param[out] ComMode Communication mode that has been requested
 **                  most recently by that user.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Getting of the Requested Communication
 **            Mode failed.
 **
 ** \ServiceID{::COMM_SID_GET_REQUESTED_COM_MODE}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_GetRequestedComMode
(
  ComM_ASR40_UserHandleType                             User,
  P2VAR(ComM_ASR40_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
);

/** \brief Retrieve the current mode of ComM.
 **
 ** This function retrieves the current communication mode of
 ** the ComM state machine.
 ** \param[in] User Handle of the user for which the query shall
 **                be performed.
 ** \param[out] ComMode Current communication mode.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Getting of Current Communication
 **            Mode failed.
 **
 ** \ServiceID{::COMM_SID_GET_CURRENT_COM_MODE}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_GetCurrentComMode
(
  ComM_ASR40_UserHandleType                             User,
  P2VAR(ComM_ASR40_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
);

/** \brief Retrieve the current mode of a PNC.
 **
 ** This function retrieves the current communication mode of
 ** the ComM PNC state machine.
 **
 ** \param[in] User Handle of the user for which the query shall
 **                be performed.
 ** \param[out] ComMode Current communication mode.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Getting of Current Communication
 **            Mode failed.
 ** \retval E_MULTIPLE_PNC_ASSIGNED multiple PNCs attached to the user
 ** \retval E_NO_PNC_ASSIGNED no PNC attached to the user
 **
 ** \ServiceID{::COMM_SID_GET_CURRENT_PNC_COM_MODE}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_GetCurrentPNCComMode
(
  ComM_ASR40_UserHandleType                             User,
  P2VAR(ComM_ASR40_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
);

#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
/** \brief Changes Wakeup inhibition status for requested channel
 **
 ** This function allows user to enable or disable
 ** the Wake Up inhibition for a requested channel.
 **
 ** \param[in] Channel on which a Wake Up inhibition will be
 ** en(-dis)abled
 ** \param[in] Status En(-dis)ables Wake up Inhibition.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Change of Wakeup Inhibition Status failed
 **
 ** \ServiceID{::COMM_SID_PREVENT_WAKE_UP}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_PreventWakeUp
(
  NetworkHandleType Channel,
  boolean Status
);
#endif

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
/** \brief Limit channel to "No Communication".
 **
 ** This function en(-dis)ables a limitation for a channel.
 **
 ** \param[in] Channel Channel on which a limitations will be en(-dis)abled.
 ** \param[in] Status En(-dis)ables channel limitation.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Limiting of channel failed.
 **
 ** \ServiceID{::COMM_SID_LIMIT_CHANNEL_TO_NO_COM}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_LimitChannelToNoComMode
(
  NetworkHandleType Channel,
  boolean Status
);
#endif

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
/** \brief Limit ECU to "No Communication".
 **
 ** This function en(-dis)ables a limitation for a ECU.
 **
 ** \param[in] Status En(-dis)ables limitation.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Limiting of ECU failed.
 **
 ** \ServiceID{::COMM_SID_LIMIT_ECU_TO_NO_COM}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_LimitECUToNoComMode
(
  boolean Status
);
#endif

#if ((COMM_WAKEUP_INHIBITION_ENABLED == STD_ON) || (COMM_MODE_LIMITATION_ENABLED == STD_ON))
/** \brief Return the amount of rejected user requests.
 **
 ** This function returns the amount of rejected "Full Communication"
 ** user requests.
 **
 ** \param[out] CounterValue Amount of rejected user requests.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Reading of Inhibit Counter failed.
 **
 ** \ServiceID{::COMM_SID_READ_INHIBIT_COUNTER}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_ReadInhibitCounter
(
  P2VAR(uint16, AUTOMATIC, COMM_APPL_DATA) CounterValue
);

/** \brief Reset inhibition counter.
 **
 ** This function resets the "Inhibited Full Communication
 ** Request Counter".
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Resetting of Inhibit Counter failed.
 **
 ** \ServiceID{::COMM_SID_RESET_INHIBIT_COUNTER}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_ResetInhibitCounter
(
  void
);

/** \brief Change the ECU Group Classification status.
 **
 ** This function changes the ECU Group Classification status.
 **
 ** \param[in] Status Inhibition status.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Setting of the ECU Group Classification
 **            failed.
 **
 ** \ServiceID{::COMM_SID_SET_ECU_GROUP_CLASS}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_SetECUGroupClassification
(
  ComM_ASR40_InhibitionStatusType  Status
);
#endif

#if ((COMM_ENABLE_ASR32_SERVICE_API == STD_ON) || \
     (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR32))

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) \
     || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
/** \brief Retrieve inhibition status via AUTOSAR 3.2 API.
 **
 ** This function returns the inhibition status of a ComM channel.
 **
 ** \param[in] Channel Network channel on which the mode transition
 **                    has occurred.
 ** \param[out] Status Current inhibition status of ComM.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Getting of Inhibition Status failed.
 **
 ** \ServiceID{::COMM_SID_GET_INHIBITION_STATUS}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
/* !LINKSTO ComM.EB.ASR32.ComM102,1 */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_GetInhibitionStatus
(
  NetworkHandleType Channel,
  P2VAR(ComM_ASR32_InhibitionStatusType, AUTOMATIC, COMM_APPL_DATA) Status
);
#endif


/** \brief Request of a communication mode by a user via AUTOSAR 3.2 API.
 **
 ** This function requests the given communication mode for the given user.
 **
 ** If Rte usage is enables in the ComM configuration the declaration of this
 ** function is provided by \verbatim Rte_ComM.h\endverbatim.
 **
 ** \param[in] User Handle of the user requesting the communication mode.
 ** \param[in] ComMode Desired communication mode.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Requesting of Communication Mode failed.
 **
 ** \ServiceID{::COMM_SID_REQUEST_COM_MODE}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
/* !LINKSTO ComM.EB.ASR32.ComM102,1 */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_RequestComMode
(
  ComM_ASR32_UserHandleType User,
  ComM_ASR32_ModeType ComMode
);

/** \brief Retrieve the maximum allowed communication mode via AUTOSAR 3.2 API.
 **
 ** This function retrieves the maximum allowed communication
 ** mode of the corresponding user.
 **
 ** \param[in] User Handle of the user for which the query shall
 **                be performed.
 ** \param[out] ComMode Maximum allowed communication mode.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Getting of Maximum Communication
 **            Mode failed.
 **
 ** \ServiceID{::COMM_SID_GET_MAX_COM_MODE}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
/* !LINKSTO ComM.EB.ASR32.ComM102,1 */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_GetMaxComMode
(
  ComM_ASR32_UserHandleType                             User,
  P2VAR(ComM_ASR32_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
);

/** \brief Retrieve the communication mode requested by a user via AUTOSAR 3.2 API.
 **
 ** This function retrieves the communication mode that has been
 ** requested most recently by the given user.
 **
 ** \param[in] User Handle of the user for which the query shall be
 **                performed.
 ** \param[out] ComMode Communication mode that has been requested
 **                  most recently by that user.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Getting of the Requested Communication
 **            Mode failed.
 **
 ** \ServiceID{::COMM_SID_GET_REQUESTED_COM_MODE}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
/* !LINKSTO ComM.EB.ASR32.ComM102,1 */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_GetRequestedMode
(
  ComM_ASR32_UserHandleType                             User,
  P2VAR(ComM_ASR32_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
);

/** \brief Retrieve the current mode of ComM via AUTOSAR 3.2 API.
 **
 ** This function retrieves the current communication mode of
 ** the ComM state machine.
 **
 ** \param[in] User Handle of the user for which the query shall
 **                be performed.
 ** \param[out] ComMode Current communication mode.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Getting of Current Communication
 **            Mode failed.
 **
 ** \ServiceID{::COMM_SID_GET_CURRENT_COM_MODE}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
/* !LINKSTO ComM.EB.ASR32.ComM102,1 */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_GetCurrentComMode
(
  ComM_ASR32_UserHandleType                             User,
  P2VAR(ComM_ASR32_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
);

#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
/** \brief Changes Wakeup inhibition status for requested channel via AUTOSAR 3.2 API
 **
 ** This function allows user to enable or disable
 ** the Wake Up inhibition for a requested channel.
 **
 ** \param[in] Channel on which a Wake Up inhibition will be
 ** en(-dis)abled
 ** \param[in] Status En(-dis)ables Wake up Inhibition.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Change of Wakeup Inhibition Status failed
 **
 ** \ServiceID{::COMM_SID_PREVENT_WAKE_UP}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
/* !LINKSTO ComM.EB.ASR32.ComM102,1 */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_PreventWakeUp
(
  NetworkHandleType Channel,
  boolean Status
);
#endif

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
/** \brief Limit channel to "No Communication" via AUTOSAR 3.2 API.
 **
 ** This function en(-dis)ables a limitation for a channel.
 **
 ** \param[in] Channel Channel on which a limitations will be en(-dis)abled.
 ** \param[in] Status En(-dis)ables channel limitation.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Limiting of channel failed.
 **
 ** \ServiceID{::COMM_SID_LIMIT_CHANNEL_TO_NO_COM}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
/* !LINKSTO ComM.EB.ASR32.ComM102,1 */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_LimitChannelToNoComMode
(
  NetworkHandleType Channel,
  boolean Status
);
#endif

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
/** \brief Limit ECU to "No Communication" via AUTOSAR 3.2 API.
 **
 ** This function en(-dis)ables a limitation for a ECU.
 **
 ** \param[in] Status En(-dis)ables limitation.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Limiting of ECU failed.
 **
 ** \ServiceID{::COMM_SID_LIMIT_ECU_TO_NO_COM}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
/* !LINKSTO ComM.EB.ASR32.ComM102,1 */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_LimitECUToNoComMode
(
  boolean Status
);
#endif

#if ((COMM_WAKEUP_INHIBITION_ENABLED == STD_ON) || (COMM_MODE_LIMITATION_ENABLED == STD_ON))
/** \brief Return the amount of rejected user requests via AUTOSAR 3.2 API.
 **
 ** This function returns the amount of rejected "Full Communication"
 ** user requests.
 **
 ** \param[out] CounterValue Amount of rejected user requests.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Reading of Inhibit Counter failed.
 **
 ** \ServiceID{::COMM_SID_READ_INHIBIT_COUNTER}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
/* !LINKSTO ComM.EB.ASR32.ComM102,1 */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_ReadInhibitCounter
(
  P2VAR(uint16, AUTOMATIC, COMM_APPL_DATA) CounterValue
);

/** \brief Reset inhibition counter via AUTOSAR 3.2 API.
 **
 ** This function resets the "Inhibited Full Communication
 ** Request Counter".
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Resetting of Inhibit Counter failed.
 **
 ** \ServiceID{::COMM_SID_RESET_INHIBIT_COUNTER}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
/* !LINKSTO ComM.EB.ASR32.ComM102,1 */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_ResetInhibitCounter
(
  void
);

/** \brief Change the ECU Group Classification status via AUTOSAR 3.2 API.
 **
 ** This function changes the ECU Group Classification status.
 **
 ** \param[in] Status Inhibition status.
 **
 ** \return Standard Return Code
 ** \retval E_OK No Errors.
 ** \retval E_NOT_OK Setting of the ECU Group Classification
 **            failed.
 **
 ** \ServiceID{::COMM_SID_SET_ECU_GROUP_CLASS}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
/* !LINKSTO ComM.EB.ASR32.ComM102,1 */
extern FUNC(Std_ReturnType, COMM_CODE) ComM_ASR32_SetECUGroupClassification
(
  ComM_ASR32_InhibitionStatusType  Status
);
#endif
#endif
/* ((COMM_ENABLE_ASR32_SERVICE_API == STD_ON) ||
    (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR32)) */

#if (COMM_VERSION_INFO_API == STD_ON)
/** \brief Get version information of the ComM module.
 **
 ** This service returns the version information of this module. The version
 ** information includes:
 **  - Module Id
 **  - Vendor Id
 **  - Vendor specific version numbers
 **
 ** \param[out] VersionInfoPtr Pointer to where to store the version
 **                  information of this module.
 **
 ** \ServiceID{::COMM_SID_GET_VERSION_INFO}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
extern FUNC(void, COMM_CODE) ComM_GetVersionInfo(
   P2VAR(Std_VersionInfoType, AUTOMATIC, COMM_APPL_DATA) VersionInfoPtr);
#endif

#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#define COMM_START_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
/** \brief Inhibition status of ComM in ROM
 **
 ** Default Inhibition status of ComM to be stored in NvM */
extern CONST(ComM_NvDataType, COMM_CONST) ComM_EB_NvROM;
#endif

#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
/** \brief Inhibition status of ComM in RAM
 **
 ** Current Inhibition status of ComM to be stored in NvM */

#define COMM_START_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
#include <ComM_MemMap.h>

extern VAR(ComM_NvDataType, COMM_VAR_POWER_ON_CLEARED) ComM_EB_NvRAM;

#define COMM_STOP_SEC_VAR_POWER_ON_CLEARED_UNSPECIFIED
#include <ComM_MemMap.h>

#endif

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef COMM_BSW_H */
/*==================[end of file]===========================================*/
