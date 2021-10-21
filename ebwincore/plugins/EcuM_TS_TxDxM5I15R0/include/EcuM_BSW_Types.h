#ifndef ECUM_BSW_TYPES_H
#define ECUM_BSW_TYPES_H

/**
 * \file
 *
 * \brief AUTOSAR EcuM
 *
 * This file contains the implementation of the AUTOSAR
 * module EcuM.
 *
 * \version 5.15.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <EcuM_Cfg.h>           /* Module configuration */

/*==================[macros]================================================*/

/*------------------[macros for WakeupStatusType]---------------------------*/

#if (defined ECUM_WKSTATUS_NONE) /* to prevent double declaration */
#error ECUM_WKSTATUS_NONE already defined
#endif
/** \brief Value for EcuM_WakeupStatusType
 **
 **  No pending wakeup event was detected. */
#define ECUM_WKSTATUS_NONE                          0U

#if (defined ECUM_WKSTATUS_PENDING) /* to prevent double declaration */
#error ECUM_WKSTATUS_PENDING already defined
#endif
/** \brief Value for EcuM_WakeupStatusType
 **
 ** The wakeup event was detected but not yet validated. */
#define ECUM_WKSTATUS_PENDING                       1U

#if (defined ECUM_WKSTATUS_VALIDATED) /* to prevent double declaration */
#error ECUM_WKSTATUS_VALIDATED already defined
#endif
/** \brief Value for EcuM_WakeupStatusType
 **
 ** The wakeup event is valid. */
#define ECUM_WKSTATUS_VALIDATED                     2U

#if (defined ECUM_WKSTATUS_EXPIRED) /* to prevent double declaration */
#error ECUM_WKSTATUS_EXPIRED already defined
#endif
/** \brief Value for EcuM_WakeupStatusType
 **
 ** The wakeup event has not been validated and has therefore expired. */
#define ECUM_WKSTATUS_EXPIRED                       3U

#if (defined ECUM_WKSTATUS_DISABLED) /* to prevent double declaration */
#error ECUM_WKSTATUS_DISABLED already defined
#endif
/** \brief Value for EcuM_WakeupStatusType
 **
 ** The wakeup source is disabled and does not detect wakeup events. */
#define ECUM_WKSTATUS_DISABLED                      4U

#if (defined ECUM_WKSTATUS_ENABLED) /* to prevent double declaration */
#error ECUM_WKSTATUS_ENABLED already defined
#endif
/** \brief Value for EcuM_WakeupStatusType
 **
 ** The wakeup source is enabled and does not detect wakeup events. */
#define ECUM_WKSTATUS_ENABLED                      6U

/*------------------[macros for EcuM_WakeupSourceType]------------------------*/

#if (defined ECUM_WKSOURCE_ALL_SOURCES) /* to prevent double declaration */
#error ECUM_WKSOURCE_ALL_SOURCES already defined
#endif
/** \brief Value for EcuM_WakeupSourceType
 **
 **  Set all wakeup sources. */
#define ECUM_WKSOURCE_ALL_SOURCES                   0xFFFFFFFFU

/*------------------[macros for error codes]--------------------------------*/

#if (defined ECUM_E_UNINIT) /* to prevent double declaration */
#error ECUM_E_UNINIT already defined
#endif
/** \brief Development Error Code
 **
 ** A service was called prior to initialization. */
#define ECUM_E_UNINIT                               0x10U

#if (defined ECUM_E_SERVICE_DISABLED) /* to prevent double declaration */
#error ECUM_E_SERVICE_DISABLED already defined
#endif
/** \brief Development Error Code
 **
 ** A service was called which was disabled by configuration. */
#define ECUM_E_SERVICE_DISABLED                     0x11U

#if (defined ECUM_E_NULL_POINTER) /* to prevent double declaration */
#error ECUM_E_NULL_POINTER already defined
#endif
/** \brief  Development Error Code
 **
 ** A null pointer was passed as an argument. */
#define ECUM_E_NULL_POINTER                         0x12U

#if (defined ECUM_E_INVALID_PAR) /* to prevent double declaration */
#error ECUM_E_INVALID_PAR already defined
#endif
/** \brief Development Error Code
 **
 ** A parameter was invalid (unspecific). */
#define ECUM_E_INVALID_PAR                          0x13U

#if (defined ECUM_E_STATE_PAR_OUT_OF_RANGE) /* to prevent double declaration*/
#error ECUM_E_STATE_PAR_OUT_OF_RANGE already defined
#endif
/** \brief Development Error Code
 **
 ** A state, passed as an argument to a service, was out of range (specific
 ** parameter test) */
#define ECUM_E_STATE_PAR_OUT_OF_RANGE               0x16U

#if (defined ECUM_E_UNKNOWN_WAKEUP_SOURCE) /* to prevent double declaration */
#error ECUM_E_UNKNOWN_WAKEUP_SOURCE already defined
#endif
/** \brief Development Error Code
 **
 ** An unknown wakeup source was passed as a parameter to an API. */
#define ECUM_E_UNKNOWN_WAKEUP_SOURCE                0x17U

#if (defined ECUM_E_PARAM_POINTER) /* to prevent double declaration */
#error ECUM_E_PARAM_POINTER already defined
#endif
/** \brief Development Error Code
 **
 ** API service called with a NULL pointer. */
#define ECUM_E_PARAM_POINTER                        0x03U

#if (defined ECUM_E_MULTIPLE_RUN_REQUESTS) /* to prevent double declaration */
#error ECUM_E_MULTIPLE_RUN_REQUESTS already defined
#endif
/** \brief Development Error Code
 **
 ** RUN / POSTRUN was requested multiple times by the same user. */
#define ECUM_E_MULTIPLE_RUN_REQUESTS                0x14U

#if (defined ECUM_E_MISMATCHED_RUN_RELEASE) /* to prevent double declaration */
#error ECUM_E_MISMATCHED_RUN_RELEASE already defined
#endif
/** \brief Development Error Code
 **
 ** RUN / POSTRUN was released though it was not requested. */
#define ECUM_E_MISMATCHED_RUN_RELEASE               0x15U
#if (defined ECUM_E_RTE_SWITCH_FAILED) /* to prevent double declaration */
#error ECUM_E_RTE_SWITCH_FAILED already defined
#endif
/** \brief Development Error Code
 **
 ** Failed to report EcuM mode switch to RTE. */
#define ECUM_E_RTE_SWITCH_FAILED                    0x18U

#if (defined ECUM_E_WRONG_CONTEXT) /* to prevent double declaration */
#error ECUM_E_WRONG_CONTEXT already defined
#endif
/** \brief  Development Error Code
 **
 ** The API has been called from an invalid core context. */
#define ECUM_E_WRONG_CONTEXT                         0x19U

#if (defined ECUM_E_UNSUCCESSFUL_CORE_START) /* to prevent double declaration */
#error ECUM_E_UNSUCCESSFUL_CORE_START already defined
#endif
/** \brief  Development Error Code
 **
 ** The Os core has not been succesfully started. */
#define ECUM_E_UNSUCCESSFUL_CORE_START                    0x1AU

#if (defined ECUM_E_WRONG_CONFIG_PARAM) /* to prevent double declaration */
#error ECUM_E_WRONG_CONFIG_PARAM already defined
#endif
/** \brief  Development Error Code
 **
 ** The provided configuration is inconsistent. */
#define ECUM_E_WRONG_CONFIG_PARAM                         0x1BU

#if (defined ECUM_E_COMMWAKEUPINDICATION) /* to prevent double declaration */
#error ECUM_E_COMMWAKEUPINDICATION already defined
#endif
/** \brief  Development Error Code
 **
 ** The Schm ComM_Ecum_Wakeup function returned a value different than SCHM_E_OK. */
#define ECUM_E_COMMWAKEUPINDICATION 0x1CU

#if (defined ECUM_E_COMMPNCWAKEUPINDICATION) /* to prevent double declaration */
#error ECUM_E_COMMPNCWAKEUPINDICATION already defined
#endif
/** \brief  Development Error Code
 **
 ** The Schm ComM_Ecum_PNCWakeup function returned a value different than SCHM_E_OK. */
#define ECUM_E_COMMPNCWAKEUPINDICATION 0x1DU

/*------------------[macros for service IDs]--------------------------------*/

#if (defined ECUM_SID_GET_VERSION_INFO)
#error ECUM_SID_GET_VERSION_INFO already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_GetVersionInfo(). */
#define ECUM_SID_GET_VERSION_INFO                   0U

#if (defined ECUM_SID_DOWN)
#error ECUM_SID_DOWN already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_GoDown() */
#define ECUM_SID_DOWN                               31U

#if (defined ECUM_SID_HALT)
#error ECUM_SID_HALT already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_GoHalt() */
#define ECUM_SID_HALT                               32U

#if (defined ECUM_SID_POLL)
#error ECUM_SID_POLL already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_GoPoll() */
#define ECUM_SID_POLL                               33U

#if (defined ECUM_SID_INIT)
#error ECUM_SID_INIT already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_Init() */
#define ECUM_SID_INIT                               1U

#if (defined ECUM_SID_SHUTDOWN)
#error ECUM_SID_SHUTDOWN already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_Shutdown() */
#define ECUM_SID_SHUTDOWN                           2U

#if (defined ECUM_SID_REQUEST_RUN)
#error ECUM_SID_REQUEST_RUN already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_RequestRUN() */
#define ECUM_SID_REQUEST_RUN                        3U

#if (defined ECUM_SID_RELEASE_RUN)
#error ECUM_SID_RELEASE_RUN already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_ReleaseRUN() */
#define ECUM_SID_RELEASE_RUN                        4U

#if (defined ECUM_SID_KILL_ALL_RUN_REQUESTS)
#error ECUM_SID_KILL_ALL_RUN_REQUESTS already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_KillAllRUNRequests() */
#define ECUM_SID_KILL_ALL_RUN_REQUESTS              5U
#if (defined ECUM_SID_SEL_SHUT_TAR)
#error ECUM_SID_SEL_SHUT_TAR already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_SelectShutdownTarget(). */
#define ECUM_SID_SEL_SHUT_TAR                       6U

#if (defined ECUM_SID_GET_LAST_SHUT_TAR)
#error ECUM_SID_GET_LAST_SHUT_TAR already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_GetLastShutdownTarget(). */
#define ECUM_SID_GET_LAST_SHUT_TAR                  8U

#if (defined ECUM_SID_GET_SHUT_TAR)
#error ECUM_SID_GET_SHUT_TAR already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_GetShutdownTarget(). */
#define ECUM_SID_GET_SHUT_TAR                       9U
#if (defined ECUM_SID_REQUEST_POST_RUN)
#error ECUM_SID_REQUEST_POST_RUN already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_RequestPOST_RUN() */
#define ECUM_SID_REQUEST_POST_RUN                   10U

#if (defined ECUM_SID_RELEASE_POST_RUN)
#error ECUM_SID_RELEASE_POST_RUN already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_ReleasePOST_RUN() */
#define ECUM_SID_RELEASE_POST_RUN                   11U

#if (defined ECUM_SID_SEL_SHUT_CAUSE)
#error ECUM_SID_SEL_SHUT_CAUSE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_SelectShutdownCause(). */
#define ECUM_SID_SEL_SHUT_CAUSE                     27U

#if (defined ECUM_SID_GET_SHUT_CAUSE)
#error ECUM_SID_GET_SHUT_CAUSE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_GetShutdownCause(). */
#define ECUM_SID_GET_SHUT_CAUSE                     28U

#if (defined ECUM_SID_GET_RCNT_SHUT_CAUSE)
#error ECUM_SID_GET_RCNT_SHUT_CAUSE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_GetMostRecentShutdown(). */
#define ECUM_SID_GET_RCNT_SHUT_CAUSE                29U

#if (defined ECUM_SID_GET_NXT_RCNT_SHUT_CAUSE)
#error ECUM_SID_GET_NXT_RCNT_SHUT_CAUSE already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_GetNextRecentShutdown(). */
#define ECUM_SID_GET_NXT_RCNT_SHUT_CAUSE            30U

#if (defined ECUM_SID_SET_WK_EV)
#error ECUM_SID_SET_WK_EV already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_SetWakeupEvent(). */
#define ECUM_SID_SET_WK_EV                          12U

#if (defined ECUM_SID_GET_PEND_WK_EV)
#error ECUM_SID_GET_PEND_WK_EV already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_GetPendingWakeupEvent(). */
#define ECUM_SID_GET_PEND_WK_EV                     13U

#if (defined ECUM_SID_SEL_BOOT_TAR)
#error ECUM_SID_SEL_BOOT_TAR already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_SelectBootTarget(). */
#define ECUM_SID_SEL_BOOT_TAR                       18U

#if (defined ECUM_SID_GET_BOOT_TAR)
#error ECUM_SID_GET_BOOT_TAR already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_GetBootTarget(). */
#define ECUM_SID_GET_BOOT_TAR                       19U

#if (defined ECUM_SID_VALIDATE_WK_EV)
#error ECUM_SID_VALIDATE_WK_EV already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_ValidateWakeupEvent. */
#define ECUM_SID_VALIDATE_WK_EV                     20U

#if (defined ECUM_SID_GET_VALID_WK_EV)
#error ECUM_SID_GET_VALID_WK_EV already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_GetValidatedWakeupEvents(). */
#define ECUM_SID_GET_VALID_WK_EV                    21U

#if (defined ECUM_SID_CLEAR_WK_EV)
#error ECUM_SID_CLEAR_WK_EV already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_ClearWakeupEvent(). */
#define ECUM_SID_CLEAR_WK_EV                        22U

#if (defined ECUM_SID_GET_STAT_OF_WK_SRC)
#error ECUM_SID_GET_STAT_OF_WK_SRC already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_GetStatusOfWakeupSource(). */
#define ECUM_SID_GET_STAT_OF_WK_SRC                 23U
#if (defined ECUM_SID_MAIN_FUNCTION)
#error ECUM_SID_MAIN_FUNCTION already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_MainFunction(). */
#define ECUM_SID_MAIN_FUNCTION                      24U

#if (defined ECUM_SID_GET_EXP_WK_EV)
#error ECUM_SID_GET_EXP_WK_EV already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_GetExpiredWakeupEvent(). */
#define ECUM_SID_GET_EXP_WK_EV                      25U

#if (defined ECUM_SID_STARTUP_TWO)
#error ECUM_SID_STARTUP_TWO already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_StartupTwo(). */
#define ECUM_SID_STARTUP_TWO                        26U

#if (defined ECUM_SID_BSWM_INIT_FINISHED)
#error ECUM_SID_BSWM_INIT_FINISHED already defined
#endif
/** \brief AUTOSAR API service ID.
 **
 ** Definition of service ID for EcuM_BswMInitFinished(). */
#define ECUM_SID_GO_RUN                             99U
/*------------------[EcuM state masks]--------------------------------------*/

#if (defined ECUM_SUBSTATE_MASK)
#error ECUM_SUBSTATE_MASK already defined
#endif
/** \brief Bitmask for sub states */
#define ECUM_SUBSTATE_MASK                          0x0FU

#if (defined ECUM_SUPERSTATE_MASK)
#error ECUM_SUPERSTATE_MASK already defined
#endif
/** \brief Bitmask for super states */
#define ECUM_SUPERSTATE_MASK                        0xF0U
#define ECUM_WKACT_RUN                              0x00U
#define ECUM_WKACT_TTII                             0x02U
#define ECUM_WKACT_SHUTDOWN                         0x03U

/*==================[type definitions]======================================*/

/** \brief Type for wakeup sources
 **
 ** The bit field provides one bit for each wakeup source.  In WAKEUP state,
 ** all bits cleared indicates that no wakeup source is known.  In STARTUP
 ** state, all bits cleared indicates that no reason for restart or reset is
 ** known (in this case, ECUM_WKSOURCE_RESET shall be assumed.)
 **
 ** All valid values for this type are defined as preprocessor macros.
 ** Additional values may be added at configuration time. */
/* !LINKSTO EcuM.EcuM4040_Implicit_1,1 */
typedef uint32 EcuM_WakeupSourceType;

/** \brief Type for the status of a wakeup source
 **
 ** Describes the possible outcomes of the WAKEUP VALIDATION state.  This type
 ** may be applied to one wakeup source or a collection of wakeup sources.
 **
 ** All valid values for this type are defined as preprocessor macros. */
 /* !LINKSTO EcuM4041,1 */
typedef uint8 EcuM_WakeupStatusType;

/** \brief Type for the Reset mechanism
 **
 ** Describes the reset mechanisms supported by the ECU State Manager.
 ** It can be extended by configuration.
 **
 ** All valid values for this type are defined as preprocessor macros. */
/* !LINKSTO EcuM4044,1 */
typedef uint8 EcuM_ResetType;

typedef uint8 EcuM_WakeupReactionType;

/* This file is decoupled from the RTE. Thus we must define types which are
   otherwise defined in Rte_Type.h without checking ECUM_INCLUDE_RTE */
#ifndef RTE_TYPE_EcuM_ASR32_StateType
#define RTE_TYPE_EcuM_ASR32_StateType
/* !LINKSTO EcuM.EB.ASR32.EcuM104,1 */
/** \brief Type for EcuM configuration
 **
 ** Encodes states and sub-states of the ECU State Manager. States are encoded
 ** in the hi-nibble and sub-states are encoded in the lo-nibble. The
 ** sub-state can be determined by ANDing the state value with
 ** ECUM_SUBSTATE_MASK.
 **
 ** All valid values for this type are defined as preprocessor macros. */
typedef uint8 EcuM_ASR32_StateType;
#endif

#ifndef RTE_TYPE_EcuM_ASR32_UserType
#define RTE_TYPE_EcuM_ASR32_UserType
/* !LINKSTO EcuM.EB.ASR32.EcuM104,1 */
/** \brief Type for a user ID of the EcuM
 **
 ** A unique User ID, which must be defined for each user at system generation
 ** time. */
typedef uint8 EcuM_ASR32_UserType;
#endif

#ifndef RTE_TYPE_EcuM_ASR40_StateType
#define RTE_TYPE_EcuM_ASR40_StateType
/* !LINKSTO EcuM.EB.ASR32.EcuM103,1 */
/** \brief Type for EcuM configuration
 **
 ** Encodes states and sub-states of the ECU State Manager. States are encoded
 ** in the hi-nibble and sub-states are encoded in the lo-nibble. The
 ** sub-state can be determined by ANDing the state value with
 ** ECUM_SUBSTATE_MASK.
 **
 ** All valid values for this type are defined as preprocessor macros. */
/* !LINKSTO EcuM4039,1 */
typedef uint8 EcuM_ASR40_StateType;
#endif

#ifndef RTE_TYPE_EcuM_ASR40_UserType
#define RTE_TYPE_EcuM_ASR40_UserType
/* !LINKSTO EcuM.EB.ASR32.EcuM103,1 */
/** \brief Type for a user ID of the EcuM
 **
 ** A unique User ID, which must be defined for each user at system generation
 ** time. */
/* !LINKSTO EcuM4067,1 */
typedef uint8 EcuM_ASR40_UserType;
#endif

#ifndef RTE_TYPE_EcuM_ASR40_ShutdownCauseType
#define RTE_TYPE_EcuM_ASR40_ShutdownCauseType
/* !LINKSTO EcuM.EB.ASR32.EcuM103,1 */
/** \brief Type for the cause of shutdown
 **
 ** Describes the cause for a shutdown by the ECU State Manager. It can be
 ** extended by configuration.
 **
 ** All valid values for this type are defined as preprocessor macros. */
/* !LINKSTO EcuM4045,1 */
typedef uint8 EcuM_ASR40_ShutdownCauseType;
#endif

/** \brief Type for the status of a wakeup source
 **
 ** This type wraps the Mcu_ModeType. This is done to avoid types of other
 ** packages in this API specification. The system designer can define a list
 ** containing elements of this type to define the different sleep modes. */
typedef struct
{
  /** \brief Sleep period for which all set wakeup sources will be enabled.*/
  EcuM_WakeupSourceType wakeupSource;

  /** \brief Sleep mode of the MCU.
   *
   * Index for the configuration tables of the MCU driver. */
  uint8                 mcuMode;

  /** \brief Suspend mode flag
   *
   * A Flag which is set to true if the CPU is suspended, halted, or powered
   * off during sleep mode. This flag must be set to false if the CPU keeps
   * running while in sleep mode.  */
  boolean               suspendCPU;
} EcuM_SleepModeConfigType;

/* --------------------- Boot Target Types --------------------------------- */

/* This file is decoupled from the RTE. Thus we must define types which are
   otherwise defined in Rte_Type.h without checking ECUM_INCLUDE_RTE */
# ifndef RTE_TYPE_EcuM_ASR40_BootTargetType
#  define RTE_TYPE_EcuM_ASR40_BootTargetType
/* !LINKSTO EcuM.EB.ASR32.EcuM103,1 */
/** \brief Type for boot target
 **
 ** Stores the type of the next boot target.
 **
 ** All valid values for this type are defined as preprocessor macros. */
/* !LINKSTO EcuM4042,1 */
typedef uint8 EcuM_ASR40_BootTargetType;
# endif

# ifndef RTE_TYPE_EcuM_ASR32_BootTargetType
#  define RTE_TYPE_EcuM_ASR32_BootTargetType
/* !LINKSTO EcuM.EB.ASR32.EcuM104,1 */
/** \brief Type for boot target
 **
 ** Stores the type of the next boot target.
 **
 ** All valid values for this type are defined as preprocessor macros. */
/* !LINKSTO EcuM4042,1 */
typedef uint8 EcuM_ASR32_BootTargetType;
# endif

/** \brief Type of internal 64 Bit boot target flag
 **
 ** Type for storing the 64 Bit representation of the boot target flag */
typedef uint32 EcuM_BootTargetInternalType[2];


/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
#endif /* ifndef ECUM_BSW_TYPES_H */

