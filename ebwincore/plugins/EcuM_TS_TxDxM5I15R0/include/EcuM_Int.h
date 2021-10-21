#ifndef ECUM_INT_H
#define ECUM_INT_H

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

#include <ComStack_Types.h>
#include <EcuM_Types.h>
#include <EcuM_Int_Cfg.h>
#include <EcuM_PbCfgTypes.h>
#include <EcuM_PBcfg.h>
#include <TSAutosar.h>     /* EB specific standard types */

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>          /* Det API */
#endif

#if (ECUM_INCLUDE_DEM == STD_ON)
/* !LINKSTO EcuM2875,1 */
#include <Dem.h>          /* Dem error types */
#endif

#include <Mcu.h>

#if (defined TS_RELOCATABLE_CFG_ENABLE)
#error TS_RELOCATABLE_CFG_ENABLE already defined
#endif
#define TS_RELOCATABLE_CFG_ENABLE ECUM_RELOCATABLE_CFG_ENABLE

#if (defined TS_PB_CFG_PTR_CLASS)
#error TS_PB_CFG_PTR_CLASS already defined
#endif
#define TS_PB_CFG_PTR_CLASS ECUM_APPL_CONST

#include <TSPBConfig.h>


/*==================[macros]================================================*/

/** \brief Returns a pointer to a dynamic post build element.
 **
 ** This macro will function for both post-build selectable (by reference) and
 ** loadable (by offset) variants.
 **
 ** \param type The data type of the post-build member.
 ** \param element The symbol of post-build member to access.
 ** \retval Pointer the post-build member within the ConfigLayoutType
 ** structure. */
#if (defined ECUM_PBCFG_ACCESS)
#error ECUM_PBCFG_ACCESS already defined
#endif
#define ECUM_PBCFG_ACCESS(type, element) \
    (TS_UNCHECKEDGETCFG(EcuM_ConfigPtr, type, ECUM, (element)))

#if (defined ECUM_DRIVER_INIT_LIST_ONE)
#error ECUM_DRIVER_INIT_LIST_ONE already defined
#endif
#define ECUM_DRIVER_INIT_LIST_ONE(driverId) (uint8)\
   ( ECUM_PBCFG_ACCESS(uint8,EcuM_ConfigPtr->PtrDriverInitListOne)[(driverId)])

#if (defined ECUM_NUM_DRIVERS_INIT_LIST_ONE)
#error ECUM_NUM_DRIVERS_INIT_LIST_ONE already defined
#endif
#define ECUM_NUM_DRIVERS_INIT_LIST_ONE ( (EcuM_ConfigPtr->NumDriversInitListOne) )

#if (defined ECUM_DRIVER_RESTART_LIST)
#error ECUM_DRIVER_RESTART_LIST already defined
#endif
#define ECUM_DRIVER_RESTART_LIST(driverId) (uint8)\
   ( ECUM_PBCFG_ACCESS(uint8,EcuM_ConfigPtr->PtrDriverRestartList)[(driverId)])

#if (defined ECUM_NUM_DRIVERS_RESTART_LIST)
#error ECUM_NUM_DRIVERS_RESTART_LIST already defined
#endif
#define ECUM_NUM_DRIVERS_RESTART_LIST ( (EcuM_ConfigPtr->NumDriversRestartList) )

#if (defined ECUM_INSTANCE_ID)
#error ECUM_INSTANCE_ID already defined
#endif
/** \brief Instance ID of EcuM */
#define ECUM_INSTANCE_ID 0U

/*------------------[OS identifiers for EcuM MultiCore]---------------------*/

#ifndef OS_CORE_ID_MASTER
#define OS_CORE_ID_MASTER 0U
typedef uint8 EcuMCoreIdType;
#else
typedef CoreIdType EcuMCoreIdType;
#endif

/*------------------[AUTOSAR development check]-----------------------------*/

#if (defined ECUM_DET_REPORT_ERROR)
#error ECUM_DET_REPORT_ERROR already defined
#endif
#if (ECUM_DEV_ERROR_DETECT == STD_ON)

/** \brief Macro for reporting an error to Det
 **
 ** \param[in] SID Service ID of the API function
 ** \param[in] ERROR_CODE Error code reported to Det module */
#if (ECUM_MULTICORE_ENABLED == TRUE)
/* !LINKSTO EcuM.MultipleInstanceDetErrorHandling,1 */
#define ECUM_DET_REPORT_ERROR(SID,ERROR_CODE)                           \
  (void)Det_ReportError(ECUM_MODULE_ID, GetCoreID(), (SID), (ERROR_CODE))
#else
#define ECUM_DET_REPORT_ERROR(SID,ERROR_CODE)                           \
  (void)Det_ReportError(ECUM_MODULE_ID, ECUM_INSTANCE_ID, (SID), (ERROR_CODE))
#endif

#else
/** \brief  If ECUM_DEV_ERROR_DETECT turned off then
 ** expand ECUM_DET_REPORT_ERROR to nothing */
#define ECUM_DET_REPORT_ERROR(SID,ERROR_CODE)
#endif





/*------------------------[Defensive programming]----------------------------*/

#if (defined ECUM_PRECONDITION_ASSERT)
#error ECUM_PRECONDITION_ASSERT is already defined
#endif

#if (defined ECUM_PRECONDITION_ASSERT_NO_EVAL)
#error ECUM_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (ECUM_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **
 ** \param[in] InstId The EcuM instance which peforms the assertion
 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ECUM_PRECONDITION_ASSERT(InstId, Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), ECUM_MODULE_ID, (InstId), (ApiId))

/** \brief Report an assertion violation to Det
 **
 ** \param[in] InstId The EcuM instance which peforms the assertion
 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ECUM_PRECONDITION_ASSERT_NO_EVAL(InstId, Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), ECUM_MODULE_ID, (InstId), (ApiId))
#else
#define ECUM_PRECONDITION_ASSERT(InstId, Condition, ApiId)
#define ECUM_PRECONDITION_ASSERT_NO_EVAL(InstId, Condition, ApiId)
#endif

#if (defined ECUM_POSTCONDITION_ASSERT)
#error ECUM_POSTCONDITION_ASSERT is already defined
#endif

#if (defined ECUM_POSTCONDITION_ASSERT_NO_EVAL)
#error ECUM_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (ECUM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **
 ** \param[in] InstId The EcuM instance which peforms the assertion
 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ECUM_POSTCONDITION_ASSERT(InstId, Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), ECUM_MODULE_ID, (InstId), (ApiId))

/** \brief Report an assertion violation to Det
 **
 ** \param[in] InstId The EcuM instance which peforms the assertion
 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ECUM_POSTCONDITION_ASSERT_NO_EVAL(InstId, Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), ECUM_MODULE_ID, (InstId), (ApiId))
#else
#define ECUM_POSTCONDITION_ASSERT(InstId, Condition, ApiId)
#define ECUM_POSTCONDITION_ASSERT_NO_EVAL(InstId, Condition, ApiId)
#endif

#if (defined ECUM_INVARIANT_ASSERT)
#error ECUM_INVARIANT_ASSERT is already defined
#endif

#if (defined ECUM_INVARIANT_ASSERT_NO_EVAL)
#error ECUM_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (ECUM_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **
 ** \param[in] InstId The EcuM instance which peforms the assertion
 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ECUM_INVARIANT_ASSERT(InstId, Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), ECUM_MODULE_ID, (InstId), (ApiId))

/** \brief Report an assertion violation to Det
 **
 ** \param[in] InstId The EcuM instance which peforms the assertion
 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ECUM_INVARIANT_ASSERT_NO_EVAL(InstId, Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), ECUM_MODULE_ID, (InstId), (ApiId))
#else
#define ECUM_INVARIANT_ASSERT(InstId, Condition, ApiId)
#define ECUM_INVARIANT_ASSERT_NO_EVAL(InstId, Condition, ApiId)
#endif

#if (defined ECUM_STATIC_ASSERT)
# error ECUM_STATIC_ASSERT is already defined
#endif
#if (ECUM_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define ECUM_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define ECUM_STATIC_ASSERT(expr)
#endif

#if (defined ECUM_UNREACHABLE_CODE_ASSERT)
#error ECUM_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (ECUM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **
 ** \param[in] InstId The EcuM instance which peforms the assertion
 ** \param[in] ApiId Service ID of the API function */
#define ECUM_UNREACHABLE_CODE_ASSERT(InstId, ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(ECUM_MODULE_ID, (InstId), (ApiId))
#else
#define ECUM_UNREACHABLE_CODE_ASSERT(InstId, ApiId)
#endif

#if (defined ECUM_INTERNAL_API_ID)
#error ECUM_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define ECUM_INTERNAL_API_ID DET_INTERNAL_API_ID



/*------------------[other internal identifiers]----------------------------*/

#if (defined ECUM_STATE_UNINIT)
#error ECUM_STATE_UNINIT already defined
#endif
/** \brief If EcuM_State is not set, it is equal to 0 */
#define ECUM_STATE_UNINIT               0U

#if (defined ECUM_WKS_DEFAULT)
#error ECUM_WKS_DEFAULT already defined
#endif
/** \brief All default wakeup sources.
 *
 * Bitwise 'or' of the default wakeup sources:
 *  - ECUM_WKSOURCE_POWER          0x01
 *  - ECUM_WKSOURCE_RESET          0x02
 *  - ECUM_WKSOURCE_INTERNAL_RESET 0x04
 *  - ECUM_WKSOURCE_INTERNAL_WDG   0x08
 *  - ECUM_WKSOURCE_EXTERNAL_WDG   0x10 */
#define ECUM_WKS_DEFAULT                                                \
  (ECUM_WKSOURCE_POWER | ECUM_WKSOURCE_RESET | ECUM_WKSOURCE_INTERNAL_RESET \
   | ECUM_WKSOURCE_INTERNAL_WDG | ECUM_WKSOURCE_EXTERNAL_WDG)


#if (defined ECUM_NO_COMM_PNC)
#error ECUM_NO_COMM_PNC already defined
#endif
/** \brief PNCHandle value for no ComM PNC
 *
 * Used as value for the EcuM_WksConfigType::ComMPNC if the wakeup source has
 * no ComM PNC associated. */
#define ECUM_NO_COMM_PNC                 0xFFU

#if (defined ECUM_NO_COMM_CH)
#error ECUM_NO_COMM_CH already defined
#endif
/** \brief NetworkHandle value for no ComM channel
 *
 * Used as value for the EcuM_WksConfigType::ComMCh if the wakeup source has
 * no ComM channel associated. */
#define ECUM_NO_COMM_CH                 0xFFU

/** \brief Invalid value for a Wakeup Source Index
 *
 * Used as value for the EcuM_WksMapping array if the wakeup source Id has
 * not been configured. */
#define ECUM_WKS_INVALID_ID             0xFFU

/** \brief Translate the Wakeup Source Id to Wakeup source configuration */
#define ECUM_WKSID_TO_CFG(WksId)        (EcuM_WksConfigList[EcuM_WksMapping[(WksId)]])

/** \brief Get the validation timeout for the Wakeup Source Id */
#define ECUM_WKS_VAL_TIMEOUT(WksId)     (EcuM_WksValTimeout[EcuM_WksMapping[(WksId)]])

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
#define ECUM_FIXED_STATE_INVALID        0x00U
#define ECUM_FIXED_STATE_STARTUP        0x01U
#define ECUM_FIXED_STATE_RUN            0x02U
#define ECUM_FIXED_STATE_POST_RUN       0x04U
#define ECUM_FIXED_STATE_SHUTDOWN       0x05U
#define ECUM_FIXED_STATE_OFF            0x06U
#define ECUM_USER_IDX(usr) ((uint8)((usr)/8U))
#define ECUM_USER_FLG(usr) ((usr)%8U)

/*------------------[internal mode defines]----------------------------*/
#if (defined ECUM_EB_MODE_STARTUP)
#error ECUM_EB_MODE_STARTUP already defined
#endif
/** \brief internal define for STARTUP mode
 *
 * This define is required to separate BSW specific code from Rte specific
 * code. */
#define ECUM_EB_MODE_STARTUP 0U

#if (defined ECUM_EB_MODE_RUN)
#error ECUM_EB_MODE_RUN already defined
#endif
/** \brief internal define for RUN mode
 *
 * This define is required to separate BSW specific code from Rte specific
 * code. */
#define ECUM_EB_MODE_RUN 1U

#if (defined ECUM_EB_MODE_POST_RUN)
#error ECUM_EB_MODE_POST_RUN already defined
#endif
/** \brief internal define for POST_RUN mode
 *
 * This define is required to separate BSW specific code from Rte specific
 * code. */
#define ECUM_EB_MODE_POST_RUN 2U


#if (defined ECUM_EB_MODE_SHUTDOWN)
#error ECUM_EB_MODE_SHUTDOWN already defined
#endif
/** \brief internal define for SHUTDOWN mode
 *
 * This define is required to separate BSW specific code from Rte specific
 * code. */
#define ECUM_EB_MODE_SHUTDOWN 3U

#if (defined ECUM_EB_MODE_SLEEP)
#error ECUM_EB_MODE_SLEEP already defined
#endif
/** \brief internal define for SLEEP mode
 *
 * This define is required to separate BSW specific code from Rte specific
 * code. */
#define ECUM_EB_MODE_SLEEP 4U

#if (defined ECUM_EB_MODE_WAKE_SLEEP)
#error ECUM_EB_MODE_WAKE_SLEEP already defined
#endif
/** \brief internal define for WAKE_SLEEP mode
 *
 * This define is required to separate BSW specific code from Rte specific
 * code. */
#define ECUM_EB_MODE_WAKE_SLEEP 5U
#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_ON) */

/*==================[type definitions]======================================*/
#if ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE))
/** \brief Function pointer to SchM EcuM_ComM_Wakeup function */
typedef P2FUNC(Std_ReturnType, ECUM_CODE, EcuM_ComM_WakeupFctPtrType)(
  uint8 Channel);

/** \brief Function pointer to SchM EcuM_ComM_PNCWakeup function */
typedef P2FUNC(Std_ReturnType, ECUM_CODE, EcuM_ComM_PNCWakeupFctPtrType)(
  uint8 PNCid);

/** \brief Function pointer list to SchM EcuM_ComM_Wakeup functions */
typedef struct
{
  EcuM_ComM_WakeupFctPtrType FctPtrs[ECUM_NUM_CORES];
} EcuM_ComMWakeupType;

/** \brief Function pointer list to SchM EcuM_ComM_PNCWakeup functions */
typedef struct
{
  EcuM_ComM_PNCWakeupFctPtrType FctPtrs[ECUM_NUM_CORES];
} EcuM_ComMPNCWakeupType;
#endif /* ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE)) */
/** \brief Type for holding a wakeup source configuration */
typedef struct
{
  /** \brief ComM PNC associated to this Wakeup Source */
  PNCHandleType ComMPNC;
  /** \brief ComM channel associated to this Wakeup Source */
  NetworkHandleType ComMCh;
  /** \brief Reset reason associated to this Wakeup Source */
  Mcu_ResetType     ResetReason;
  /** \brief The identifier of this Wakeup Source */
  uint8             WksId;
} EcuM_WksConfigType;

/** \brief Defines the status of all wakeup events. */
typedef struct
{
  /** \brief Disabled Wakeup Sources */
  VAR(EcuM_WakeupSourceType, TYPEDEF) Disabled;
  /** \brief Cleared Wakeup Sources */
  VAR(EcuM_WakeupSourceType, TYPEDEF) None;
  /** \brief Pending Wakeup Sources */
  VAR(EcuM_WakeupSourceType, TYPEDEF) Pending;
  /** \brief Validated Wakeup Sources */
  VAR(EcuM_WakeupSourceType, TYPEDEF) Validated;
  /** \brief Expired Wakeup Sources */
  VAR(EcuM_WakeupSourceType, TYPEDEF) Expired;
  /** \brief Started Wakeup Sources */
  VAR(EcuM_WakeupSourceType, TYPEDEF) Started;
  /** \brief Wakeup Sources to be stopped when the main function is executed. */
  VAR(EcuM_WakeupSourceType, TYPEDEF) Stopped;
} EcuM_WakeupContextType;

/** \brief Defines the wakeup event state changes to be indicated to the BswM */
typedef struct
{
  /** \brief The disabled wakeup events to be indicated to the BswM. */
  VAR(EcuM_WakeupSourceType, TYPEDEF) Disabled;
  /** \brief The expired wakeup events to be indicated to the BswM. */
  VAR(EcuM_WakeupSourceType, TYPEDEF) Expired;
  /** \brief The cleared wakeup events to be indicated to the BswM. */
  VAR(EcuM_WakeupSourceType, TYPEDEF) None;
  /** \brief The pending wakeup events to be indicated to the BswM. */
  VAR(EcuM_WakeupSourceType, TYPEDEF) Pending;
  /** \brief The validated wakeup events to be indicated to the BswM. */
  VAR(EcuM_WakeupSourceType, TYPEDEF) Validated;
} EcuM_WakeupIndicationType;


/** \brief Defines all core specific context data. */
/* !LINKSTO EcuM.Impl.DataStructure.EcuM_CoreContextType,1 */
typedef struct
{
  /** \brief The wakeup event status for a core. */
  VAR(EcuM_WakeupContextType, TYPEDEF) WakeupStatus;
  /** \brief The wakeup wake state changes for a core. */
  VAR(EcuM_WakeupIndicationType, TYPEDEF) WakeupIndication;
  /** \brief Counter for time-out measuring for wakeup validation on a core. */
  VAR(uint16, TYPEDEF) ValidationCounter;
  /** \brief The EcuM state of each core. */
  VAR(EcuM_ASR40_StateType, TYPEDEF) State;
  /** \brief Current sleep mode for shutdown
   ** This variable holds the current sleep mode used in the next shutdown
   ** sequence. */
  VAR(uint8, TYPEDEF) SleepMode;
#if ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE))
#if (ECUM_WKS_COMMCH_ENABLED == STD_ON)
  /** \brief Holds information indicating if ComM_EcuM_WakeUpIndication
   **        was called already in the current mainfunction. */
  VAR(uint8, TYPEDEF) ComMChannelCall[ECUM_COMM_NUM_CHANNELS_BYTES];
#endif /*(ECUM_WKS_COMMCH_ENABLED == STD_ON) */
#if (ECUM_WKS_COMMPNC_ENABLED == STD_ON)
  /** \brief Holds information indicating if ComM_EcuM_PNCWakeUpIndication
   **        was called already in the current mainfunction. */
  VAR(uint8, TYPEDEF) ComMPNCCall[ECUM_COMM_NUM_PNCS_BYTES];
#endif /* (ECUM_WKS_COMMPNC_ENABLED == STD_ON) */
#endif /* #if ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE)) */
} EcuM_CoreContextType;

/** \brief State variables for shutdown management. */
typedef struct
{
  /** \brief time at which the shutdown happened */
  uint32 sdTime;
  /** \brief configured shutdown target */
  EcuM_ASR40_StateType target;
  /** \brief configured sleep/reset mode */
  uint8 mode;
  /** \brief real cause of shutdown */
  EcuM_ASR40_ShutdownCauseType cause;
} EcuM_ShutdownInfoType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define ECUM_START_SEC_CODE
#include <EcuM_MemMap.h>

/** \brief Proxy function for SchM_Init
 **
 ** This function was introduced to avoid RTE includes
 ** within compilation units EcuM.c and EcuM_BootTarget.c
 **
 ** \return No return value. */
FUNC(void, ECUM_CODE) EcuM_SchM_Init(void);

/** \brief Proxy function for SchM_Deinit
 **
 ** This function was introduced to avoid RTE includes
 ** within compilation units EcuM.c and EcuM_BootTarget.c
 **
 ** \return No return value. */
FUNC(void, ECUM_CODE) EcuM_SchM_Deinit(void);

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
#if (ECUM_EB_DEFAULT_ASR_SERVICE_API != ECUM_EB_SERVICE_API_NONE)
/** \brief Signal mode change via RTE
 *
 * The function receives the status from the EcuM point of view and has to
 * translate them into the values expected by the RTE. The reported values depend
 * on the AUTOSAR service API selected as default.
 *
 * \param[in] mode  new mode */
FUNC(void, ECUM_CODE) EcuM_RteModeSwitch(uint8 mode);
#endif /* (ECUM_EB_DEFAULT_ASR_SERVICE_API != ECUM_EB_SERVICE_API_NONE) */

#if (ECUM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
/** \brief Signal mode change via RTE
 *
 * The function receives the status from the EcuM point of view and has to
 * translate them into the Autosar 3.2 values expected by the RTE.
 *
 * \param[in] mode  new mode */
FUNC(void, ECUM_CODE) EcuM_ASR32_RteModeSwitch(uint8 mode);
#endif /* (ECUM_EB_ENABLE_ASR32_SERVICE_API == STD_ON) */

#if (ECUM_EB_ENABLE_ASR40_SERVICE_API == STD_ON)
/** \brief Signal mode change via RTE
 *
 * The function receives the status from the EcuM point of view and has to
 * translate them into the Autosar 4.0 values expected by the RTE.
 *
 * \param[in] mode  new mode */
FUNC(void, ECUM_CODE) EcuM_ASR40_RteModeSwitch(uint8 mode);
#endif /* (ECUM_EB_ENABLE_ASR40_SERVICE_API == STD_ON) */
#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_ON) */

#define ECUM_STOP_SEC_CODE
#include <EcuM_MemMap.h>

/*-------------[EcuM Multi-Core internal function declarations]-------------*/

#define ECUM_START_SEC_CODE
#include <EcuM_MemMap.h>

#if (ECUM_MULTICORE_ENABLED == TRUE)

extern FUNC(void, ECUM_CODE) EcuM_GetResourceScheduler(void);

extern FUNC(void, ECUM_CODE) EcuM_ReleaseResourceScheduler(void);

/** \brief Blocks the master core until all slave cores have signalled that they
 **        are ready.
 **
 ** Each slave core shall signal the master core that it is ready by passing the
 ** coreID to the SlaveCoreSynchPort: SchM_Send_EcuM_SlaveCoreReadyPort(coreId)
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, ECUM_CODE) EcuM_SlaveCoreSync(void);

/** \brief Blocks a slave core until the master core has signalled that it is
 **  ready.
 **
 ** The master core shall signal the slave cores by making a single call to the
 ** MasterCoreSyncPort: SchM_Send_EcuM_MasterCoreReadyPort(TRUE)
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, ECUM_CODE) EcuM_MasterCoreSync(void);

extern FUNC(Std_ReturnType, ECUM_CODE) EcuM_Send_SlaveCoreReadyPort(void);

#endif /* (ECUM_MULTICORE_ENABLED == TRUE) */

/** \brief Wrapper functions for entering and exiting exclusive areas
 ** on the currently active core.
 **/
extern FUNC(void, ECUM_CODE) EcuM_EnterExclusiveArea(void);

extern FUNC(void, ECUM_CODE) EcuM_ExitExclusiveArea(void);

/* The EcuM_GetCoreInstance() and EcuM_GetMasterCoreInstance() APIs are not guarded by the
 * ECUM_MULTICORE_ENABLED macro since they are used also in a single core context to verify the
 * data structure of the single core instance.
 */

/** \brief Returns the core context data structure for the core which
 **  called the function.
 **/
extern FUNC_P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_CODE) EcuM_GetCoreInstance(void);

/** \brief Returns the master core context data structure.
 **/
extern FUNC_P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_CODE) EcuM_GetMasterCoreInstance(void);

#define ECUM_STOP_SEC_CODE
#include <EcuM_MemMap.h>

/*==================[external constants]====================================*/

#define ECUM_START_SEC_CONST_8
#include <EcuM_MemMap.h>

/** \brief Wake-up Source ID mapping
 **
 ** Translation array to map a Wake-up Source ID to an array index
 ** inside "EcuM_WksConfigList" or "EcuM_WksValTimeout".*/
extern CONST(uint8, ECUM_CONST) EcuM_WksMapping[ECUM_WKS_ID_MAXVAL + 1U];

#if ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE))
#if (ECUM_COMM_CHANNELS_NUMBER > 0)
/** \brief EcuM channel to ComM channel translation */
extern CONST(uint8, ECUM_CONST) EcuM_ComM_Channel_Translation[ECUM_COMM_CHANNELS_NUMBER];
#endif /* (ECUM_COMM_CHANNELS_NUMBER > 0) */
#if (ECUM_COMM_PNC_NUMBER > 0)
/** \brief EcuM PNC to ComM PNC translation */
extern CONST(uint8, ECUM_CONST) EcuM_ComM_Pnc_Translation[ECUM_COMM_PNC_NUMBER];
#endif /* (ECUM_COMM_PNC_NUMBER > 0) */
#endif /* ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE))) */

#define ECUM_STOP_SEC_CONST_8
#include <EcuM_MemMap.h>

#define ECUM_START_SEC_CONST_16
#include <EcuM_MemMap.h>

/** \brief Timeout for wakeup source */
extern CONST(uint16, ECUM_CONST) EcuM_WksValTimeout[ECUM_WKSCONFIGNUM];

#define ECUM_STOP_SEC_CONST_16
#include <EcuM_MemMap.h>


#define ECUM_START_SEC_CONST_UNSPECIFIED
#include <EcuM_MemMap.h>

/** \brief List of Sleep Modes Configurations */
extern CONST(EcuM_SleepModeConfigType, ECUM_CONST)
  EcuM_SleepModeConfigList[ECUM_SLEEPMODECONFIGNUM];

/** \brief List of Wakeup Source Configurations */
extern CONST(EcuM_WksConfigType, ECUM_CONST)
  EcuM_WksConfigList[ECUM_WKSCONFIGNUM];

#if (ECUM_INCLUDE_DEM == STD_ON)
  #if(ECUM_PROD_ERR_RAM_CHECK_FAILURE == TS_PROD_ERR_REP_TO_DEM)
/** \brief Event ID for ECUM_E_RAM_CHECK_FAILED */
extern CONST(Dem_EventIdType, ECUM_CONST) EcuM_DemErrRamChkFailed;
  #endif

  #if ((ECUM_ECUM_FIXED_SUPPORT == STD_ON) && (ECUM_PROD_ERR_ALL_RUN_REQUESTS_KILLED == TS_PROD_ERR_REP_TO_DEM))
/** \brief Event ID for ECUM_E_ALL_RUN_REQUESTS_KILLED */
extern CONST(Dem_EventIdType, ECUM_CONST) EcuM_DemErrAllRUNRequestsKilled;
  #endif /* ((ECUM_ECUM_FIXED_SUPPORT == STD_ON) && (ECUM_PROD_ERR_ALL_RUN_REQUESTS_KILLED == TS_PROD_ERR_REP_TO_DEM)) */
#endif /* ECUM_INCLUDE_DEM == STD_ON */

#define ECUM_STOP_SEC_CONST_UNSPECIFIED
#include <EcuM_MemMap.h>

#define ECUM_START_SEC_CONST_8
#include <EcuM_MemMap.h>

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
/** \brief Array holding the configured values of Shutdown Causes */
extern CONST(EcuM_ShutdownCauseType, ECUM_CONST)
   EcuM_ValidShutdownCause[ECUM_SHUTDOWNCAUSECONFIGNUM];
#endif

#define ECUM_STOP_SEC_CONST_8
#include <EcuM_MemMap.h>

#define ECUM_START_SEC_CONST_UNSPECIFIED
#include <EcuM_MemMap.h>
/** \brief The initial core context for a single core. */
extern CONST(EcuM_CoreContextType, ECUM_CONST) EcuM_InitialContext;

#if ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE))
#if (ECUM_COMM_CHANNELS_NUMBER > 0)
/** \brief The SchM function pointer list for Channels. */
extern CONST(EcuM_ComMWakeupType, ECUM_CONST) EcuM_ComM_Channel_WakeupList[ECUM_COMM_CHANNELS_NUMBER];
#endif /* (ECUM_COMM_CHANNELS_NUMBER > 0) */

#if (ECUM_COMM_PNC_NUMBER > 0)
/** \brief The SchM function pointer list for Pncs. */
extern CONST(EcuM_ComMPNCWakeupType, ECUM_CONST) EcuM_ComM_PNC_WakeupList[ECUM_COMM_PNC_NUMBER];
#endif /* (ECUM_COMM_PNC_NUMBER > 0) */
#endif /* #if ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE)) */

#define ECUM_STOP_SEC_CONST_UNSPECIFIED
#include <EcuM_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/
/*--------------[init variables]-----------------------------------*/

#define ECUM_START_SEC_VAR_INIT_UNSPECIFIED
#include <EcuM_MemMap.h>

/** \brief Pointer to Post-Build Time Configuration for EcuM
 **
 ** Pointer to the configuration returned from the function
 ** EcuM_DeterminePbConfiguration(). Set in EcuM_Init(). */
extern P2CONST(EcuM_ConfigType,AUTOMATIC,ECUM_APPL_CONST) EcuM_ConfigPtr;

#define ECUM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <EcuM_MemMap.h>

/*--------------[power on init variables]-----------------------------------*/

#define ECUM_START_SEC_INTERNAL_VAR_POWER_ON_INIT_UNSPECIFIED
#include <EcuM_MemMap.h>

/** \brief Previous shutdown information
 **
 ** This variable holds all the previous shutdown information which contains
 ** shutdown target, cause of shutdown, time and sleep mode.
 **
 ** This variable must survive ECU resets. */
extern VAR(EcuM_ShutdownInfoType, ECUM_VAR_POWER_ON_INIT)
  EcuM_ShutdownInfo[ECUM_STORED_SHUTDOWN_OPERATIONS];

#define ECUM_STOP_SEC_INTERNAL_VAR_POWER_ON_INIT_UNSPECIFIED
#include <EcuM_MemMap.h>
/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#undef TS_RELOCATABLE_CFG_ENABLE
#undef TS_PB_CFG_PTR_CLASS

/*==================[end of file]===========================================*/
#endif /* ifndef ECUM_INT_H */

