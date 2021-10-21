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

/*  MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Rule: 8.3 (required)
 *     All declarations of an object or function shall use the same names
 *     and type qualifiers.
 *
 *     Reason:
 *     The requirement 'EcuM3011' defines the arguments of EcuM_ShutdownTarget
 *     (ClientServerInterface EcuM_ShutdownTarget) as 'target' and 'mode' but
 *     the EcuM API specification defines the arguments as 'shutdownTarget' and
 *     'sleepMode' respectively. This issue has been reported in Rfc#58147
 *
 *  MISRAC2012-2) Deviated Rule: 21.2 (required)
 *     A reserved identifier or macro name shall not be declared.
 *
 *     Reason:
 *     In case of functions EcuM_GetMostRecentShutdown and EcuM_GetNextRecentShutdown,
 *     Autosar specifies that the last parameter shall be called 'time'. This is also
 *     the name of a standard library function.
 *
 */

/* !LINKSTO EcuM3023,1 */
/*==================[inclusions]==============================================*/

#include <EcuM_Trace.h>
#include <Os.h>                  /* Os public API */

#include <TSAutosar.h>           /* EB specific standard types */

#include <Mcu.h>                 /* Mcu API */

#include <EcuM.h>
#include <EcuM_Int.h>            /* Module internal API */
#include <SchM_EcuM.h>           /* SchM API for EcuM */
#if (ECUM_INCLUDE_RTE == STD_ON)
#include <Rte_EcuM.h>
#endif

#include <BswM_EcuM.h>           /* BswM callback API */
#include <BswM.h>                /* BswM API, for BswM_Init() and _Deinit() */

/* Include ComM_EcuM.h only if wakeup sources associated with ComM Channel are
 * available */
#if ((ECUM_WKS_COMMCH_ENABLED == STD_ON) || (ECUM_WKS_COMMPNC_ENABLED == STD_ON))
#include <ComM_EcuMBswM.h>       /* ComM callback API */
#include <ComM.h>
#endif
#if ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE))
#include <TSMem.h>
#endif /* ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE)) */
/*==================[macros]==================================================*/
/* !LINKSTO EcuM.Version.Check,1 */
/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef ECUM_VENDOR_ID /* configuration check */
#error ECUM_VENDOR_ID must be defined
#endif

#if (ECUM_VENDOR_ID != 1U) /* vendor check */
#error ECUM_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef ECUM_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error ECUM_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (ECUM_AR_RELEASE_MAJOR_VERSION != 4U)
#error ECUM_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef ECUM_AR_RELEASE_MINOR_VERSION /* configuration check */
#error ECUM_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (ECUM_AR_RELEASE_MINOR_VERSION != 0U )
#error ECUM_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef ECUM_AR_RELEASE_REVISION_VERSION /* configuration check */
#error ECUM_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (ECUM_AR_RELEASE_REVISION_VERSION != 3U )
#error ECUM_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef ECUM_SW_MAJOR_VERSION /* configuration check */
#error ECUM_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (ECUM_SW_MAJOR_VERSION != 5U)
#error ECUM_SW_MAJOR_VERSION wrong (!= 5U)
#endif

#ifndef ECUM_SW_MINOR_VERSION /* configuration check */
#error ECUM_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (ECUM_SW_MINOR_VERSION < 15U)
#error ECUM_SW_MINOR_VERSION wrong (< 15U)
#endif

#ifndef ECUM_SW_PATCH_VERSION /* configuration check */
#error ECUM_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (ECUM_SW_PATCH_VERSION < 7U)
#error ECUM_SW_PATCH_VERSION wrong (< 7U)
#endif

#if (ECUM_INCLUDE_DEM == STD_ON)
#if (ECUM_PROD_ERR_RAM_CHECK_FAILURE == TS_PROD_ERR_REP_TO_DEM)
#if (defined ECUM_RAM_CHECK_STATUS_UNTESTED)
#error ECUM_RAM_CHECK_STATUS_UNTESTED already defined
#endif
/** \brief Status of RAM integrity check for reporting the error to DEM */
#define ECUM_RAM_CHECK_STATUS_UNTESTED  0U

#if (defined ECUM_RAM_CHECK_STATUS_PASSED)
#error ECUM_RAM_CHECK_STATUS_PASSED already defined
#endif
/** \brief Status of RAM integrity check for reporting the error to DEM */
#define ECUM_RAM_CHECK_STATUS_PASSED    1U

#if (defined ECUM_RAM_CHECK_STATUS_FAILED)
#error ECUM_RAM_CHECK_STATUS_FAILED already defined
#endif
/** \brief Status of RAM integrity check for reporting the error to DEM */
#define ECUM_RAM_CHECK_STATUS_FAILED    2U
#endif /* (ECUM_PROD_ERR_RAM_CHECK_FAILURE == TS_PROD_ERR_REP_TO_DEM) */

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)

#if (defined ECUM_E_ALL_RUN_REQUESTS_KILLED)
#error ECUM_E_ALL_RUN_REQUESTS_KILLED already defined
#endif
/** \brief DEM error status that all EcuM run requests have been killed. */
#define ECUM_E_ALL_RUN_REQUESTS_KILLED  1U
#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_ON) */
#endif /* (ECUM_INCLUDE_DEM == STD_ON) */

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
#if (defined ECUM_FIXED_USER_BITMAP_SIZE)
#error ECUM_FIXED_USER_BITMAP_SIZE already defined
#endif
/** \brief The number of bytes within the EcuM-fixed users bitmap array. */
#define ECUM_FIXED_USER_BITMAP_SIZE  (((ECUM_FIXED_NUM_FIXED_USERS) / 7U) + 1U)

#if (defined ECUM_EB_RTEMODESWITCH)
#error ECUM_EB_RTEMODESWITCH already defined
#endif
#if (ECUM_EB_DEFAULT_ASR_SERVICE_API != ECUM_EB_SERVICE_API_NONE)
/** \brief Signalling mode change via RTE
 *
 * The macro receives the new mode from the EcuM point of view and
 * propagates the arguments to the corresponding default RTE handle function.
 *
 * If the default service API is set to NONE then the macro is defined empty.
 *
 * \param[in] mode new mode
 */
#define ECUM_EB_RTEMODESWITCH(mode) EcuM_RteModeSwitch(mode)
#else
#define ECUM_EB_RTEMODESWITCH(mode)
#endif

#if (defined ECUM_EB_ASR32_RTEMODESWITCH)
#error ECUM_EB_ASR32_RTEMODESWITCH already defined
#endif
#if (ECUM_EB_ENABLE_ASR32_SERVICE_API == STD_ON)
/** \brief Signalling mode change via RTE
 *
 * The macro receives the new mode from the EcuM point of view and
 * propagates the arguments to the corresponding Autosar 3.2 RTE handle
 * function.
 *
 * If the AUTOSAR 3.2 service API is not configured then the macro is defined
 * empty.
 *
 * \param[in] mode new mode
 */
#define ECUM_EB_ASR32_RTEMODESWITCH(mode) EcuM_ASR32_RteModeSwitch(mode)
#else
#define ECUM_EB_ASR32_RTEMODESWITCH(mode)
#endif

#if (defined ECUM_EB_ASR40_RTEMODESWITCH)
#error ECUM_EB_ASR40_RTEMODESWITCH already defined
#endif
#if (ECUM_EB_ENABLE_ASR40_SERVICE_API == STD_ON)
/** \brief Signalling mode change via RTE
 *
 * The macro receives the new mode from the EcuM point of view and
 * propagates the arguments to the corresponding Autosar 4.0 RTE handle
 * function.
 *
 * If the AUTOSAR 4.0 service API is not configured then the macro is defined
 * empty.
 *
 * \param[in] mode  new mode
 */
#define ECUM_EB_ASR40_RTEMODESWITCH(mode) EcuM_ASR40_RteModeSwitch(mode)
#else
#define ECUM_EB_ASR40_RTEMODESWITCH(mode)
#endif

#if (defined ECUM_EB_CURRENT_MODE_SWITCH)
#error ECUM_EB_CURRENT_MODE_SWITCH already defined
#endif
/** \brief Passes an EcuM current mode switch to each enabled Rte interface. */
#define ECUM_EB_CURRENT_MODE_SWITCH(mode) \
  do {                                    \
    ECUM_EB_RTEMODESWITCH((mode));        \
    ECUM_EB_ASR32_RTEMODESWITCH((mode));  \
    ECUM_EB_ASR40_RTEMODESWITCH((mode));  \
  } while (0)
#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_ON) */

#if (defined ECUM_GET_CORE_ID)
#error ECUM_GET_CORE_ID already defined
#endif

#if (defined ECUM_GET_RESOURCE_SCHEDULER)
#error ECUM_GET_RESOURCE_SCHEDULER already defined
#endif

#if (defined ECUM_RELEASE_RESOURCE_SCHEDULER)
#error ECUM_RELEASE_RESOURCE_SCHEDULER already defined
#endif

#if (ECUM_MULTICORE_ENABLED == TRUE)
/** \brief Returns the core ID that is currently active. */
#define ECUM_GET_CORE_ID() GetCoreID()

/** \brief Gets the Os scheduler resource of the currently active core. */
#define ECUM_GET_RESOURCE_SCHEDULER() EcuM_GetResourceScheduler()

/** \brief Releases the Os scheduler resource of the currently active core. */
#define ECUM_RELEASE_RESOURCE_SCHEDULER() EcuM_ReleaseResourceScheduler()
#else
#define ECUM_GET_CORE_ID() 0U
#define ECUM_GET_RESOURCE_SCHEDULER() (void)GetResource(RES_SCHEDULER)
#define ECUM_RELEASE_RESOURCE_SCHEDULER() (void)ReleaseResource(RES_SCHEDULER)
#endif /* (ECUM_MULTICORE_ENABLED == TRUE) */

/*==================[type definitions]========================================*/

/*==================[external function declarations]==========================*/

/*==================[internal function declarations]==========================*/

#define ECUM_START_SEC_CODE
/* !LINKSTO EcuM.Impl.MemMap,1 */
#include <EcuM_MemMap.h>

/** \brief Executes the activities of the GoSleep Sequence. */
STATIC FUNC(void, ECUM_CODE) EcuM_GoSleep(void);

/** \brief Executes the activities of the WakeupRestart Sequence. */
STATIC FUNC(void, ECUM_CODE) EcuM_WakeupRestart(void);

#if (ECUM_INCLUDE_DEM == STD_ON) && (ECUM_PROD_ERR_RAM_CHECK_FAILURE == TS_PROD_ERR_REP_TO_DEM)
/** \brief Reports the result of the last RamCheck to DEM */
STATIC FUNC(void, ECUM_CODE) EcuM_ReportRamCheckResult(void);
#endif

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
/** \brief Passes an EcuM current mode switch to each enabled Rte interface */
STATIC FUNC(void, ECUM_CODE) EcuM_CurrentModeSwitch(void);
#endif

/** \brief Executes the GoDown sequence after the slave cores have shutdown. */
STATIC FUNC(Std_ReturnType, ECUM_CODE) EcuM_GoDownMaster(void);

/** \brief Executes the Halt sequence activities. */
STATIC FUNC(void, ECUM_CODE) EcuM_HaltSequence(void);

/** \brief Executes the required actions for setting the SleepMode or ResetMode. */
STATIC FUNC(void, ECUM_CODE) EcuM_SetSleepOrResetMode(void);

/** \brief Sets the wakeup event associated with the Mcu reset reason. */
STATIC FUNC(void, ECUM_CODE) EcuM_SearchResetReason(void);

/** \brief Performs necessary actions before the OS is started. */
STATIC FUNC(void, ECUM_CODE) EcuM_StartPreOS(void);

#if (ECUM_MULTICORE_ENABLED == FALSE)
/** \brief Performs initialization activities in a single core context. */
STATIC FUNC(void, ECUM_CODE) EcuM_Init_SingleCore(void);
#endif

#if (ECUM_MULTICORE_ENABLED == TRUE)
/** \brief Performs initialization activities in a single core context. */
STATIC FUNC(void, ECUM_CODE) EcuM_Init_MultiCore(void);

/** \brief Performs Halt Sequence activities related to a slave core context. */
STATIC FUNC(Std_ReturnType, ECUM_CODE) EcuM_GoHaltSlave(void);
/** \brief Performs Halt Sequence activities related to a master core context. */
STATIC FUNC(Std_ReturnType, ECUM_CODE) EcuM_GoHaltMaster(void);
#endif /* (ECUM_MULTICORE_ENABLED == TRUE) */

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
/** \brief Performs the required actions for RUN and POST_RUN requests. */
STATIC FUNC(void, ECUM_CODE) EcuM_HandleRunRequests(void);
#endif

/** \brief Indicate the wakeup sources status changes to the BswM. */
STATIC FUNC(void, ECUM_CODE) EcuM_WksIndicateStatus(void);

/** \brief Switch the status of wakeup sources
 **
 ** \param[in]  sources       The wakeup sources to transition.
 ** \param[in]  status        The new status of the wakeup sources.
 ** \param[out] CoreInstance  The current core instance of EcuM.
 **
 **
 ** \par The following values may be passed to this function:
 ** \li ECUM_WKSTATUS_DISABLED (super-state and leaf-state)
 ** \li ECUM_WKSTATUS_ENABLED (super-state)
 ** \li ECUM_WKSTATUS_EXPIRED
 ** \li ECUM_WKSTATUS_NONE
 ** \li ECUM_WKSTATUS_PENDING
 ** \li ECUM_WKSTATUS_VALIDATED
 **
 ** Because the wakeup source state machine is broken in the ASR-4.0.3 SWS, a
 ** wakeup source may be disabled or enabled in one of the following states:
 ** EXPIRED, NONE, PENDING or VALIDATED.  Therefore, ENABLE has been introduced
 ** as a super-state of NONE, PENDING, VALIDATED and EXPIRED.  DISABLED serves
 ** as a super-state of DISABLED (itself), NONE, PENDING, VALIDATED and EXPIRED.
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
STATIC FUNC(void, ECUM_CODE) EcuM_SwitchWakeupStatus
(
  EcuM_WakeupSourceType sources,
  EcuM_WakeupStatusType status,
  P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance
);

/** \brief Indicates the wakeup source changes to ECUM_WKSTATUS_DISABLED. */
STATIC FUNC(void, ECUM_CODE) EcuM_WksIndicateStatus_DISABLED(void);

/** \brief Indicates the wakeup source changes to ECUM_WKSTATUS_EXPIRED. */
STATIC FUNC(void, ECUM_CODE) EcuM_WksIndicateStatus_EXPIRED(void);

/** \brief Indicates the wakeup source changes to ECUM_WKSTATUS_NONE. */
STATIC FUNC(void, ECUM_CODE) EcuM_WksIndicateStatus_NONE(void);

/** \brief Indicates the wakeup source changes to ECUM_WKSTATUS_PENDING. */
STATIC FUNC(void, ECUM_CODE) EcuM_WksIndicateStatus_PENDING(void);

/** \brief Indicates the wakeup source changes to ECUM_WKSTATUS_VALIDATED. */
STATIC FUNC(void, ECUM_CODE) EcuM_WksIndicateStatus_VALIDATED(void);

#if (ECUM_WKS_COMMCH_ENABLED == STD_ON)
/** \brief Function to indicate the wakeup status to ComM channels.
 **
 ** Indicates the wakeup status to ComM channels only when ComM has been
 ** successfully initialized.
 **
 ** \param[in] ComMevents  The ComM wakeup events to indicate.
 **/
STATIC FUNC(void, ECUM_CODE) EcuM_ComMWakeupIndication(EcuM_WakeupSourceType ComMevents);
#endif

#if (ECUM_WKS_COMMPNC_ENABLED == STD_ON)
/** \brief Function to indicate the wakeup status to ComM PNCs.
 **
 ** Indicates the wakeup status to ComM PNCs only when ComM has been
 ** successfully initialized.
 **
 ** \param[in] ComMPNCevents  The ComM PNC wakeup events to indicate.
 **/
STATIC FUNC(void, ECUM_CODE) EcuM_ComMPNCWakeupIndication(EcuM_WakeupSourceType ComMPNCevents);
#endif

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
/** \brief Returns true if the EcuM run state has been requested. */
STATIC FUNC(boolean, ECUM_CODE) EcuM_IsRunReq(void);

/** \brief Returns true if the EcuM post run state has been requested. */
STATIC FUNC(boolean, ECUM_CODE) EcuM_IsPostRunReq(void);
#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_ON) */

STATIC FUNC(Std_ReturnType, ECUM_CODE) EcuM_SelectShutdownTargetSequence(EcuM_ASR40_StateType target, uint8 mode);

#define ECUM_STOP_SEC_CODE
#include <EcuM_MemMap.h>

/*==================[external constants]======================================*/

/*==================[internal constants]======================================*/

/*==================[external data]===========================================*/

/*--------------[init variables]--------------------------------------------- */

/*==================[internal data]===========================================*/

/*--------------[init variables]--------------------------------------------- */

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)

#define ECUM_START_SEC_VAR_INIT_8
#include <EcuM_MemMap.h>

/** \brief Stores the requests for the EcuM run state. */
STATIC VAR(uint8, ECUM_VAR) EcuM_RunReq[ECUM_FIXED_USER_BITMAP_SIZE] = {0U};

/** \brief Stores the requests for the EcuM post run state. */
STATIC VAR(uint8, ECUM_VAR) EcuM_PostRunReq[ECUM_FIXED_USER_BITMAP_SIZE] = {0U};

/** \brief Flag to indicate that all run requests have bee killed. */
STATIC VAR(boolean, ECUM_VAR) EcuM_KillAllRUNRequested = FALSE;

#define ECUM_STOP_SEC_VAR_INIT_8
#include <EcuM_MemMap.h>

#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_ON) */

#define ECUM_START_SEC_VAR_INIT_UNSPECIFIED
#include <EcuM_MemMap.h>

P2CONST(EcuM_ConfigType, ECUM_VAR, ECUM_CONST) EcuM_ConfigPtr = NULL_PTR;

#define ECUM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <EcuM_MemMap.h>

/*--------------[power on init variables]-------------------------------------*/

#define ECUM_START_SEC_VAR_POWER_ON_INIT_8
#include <EcuM_MemMap.h>

/** \brief Returns the last shutdown target
 **
 ** This variable holds the shutdown target used in last shutdown sequence.
 **
 ** It is unspecified in the SWS what last shutdown target shall be reported as
 ** default not shutdown sequence was performed after a power on.
 **
 ** This variable must survive ECU resets. */
STATIC VAR(EcuM_ASR40_StateType, ECUM_VAR_POWER_ON_INIT) EcuM_LastShutdownTarget = ECUM_STATE_OFF;

/** \brief Last Sleep Mode
 **
 ** This variable holds the sleep mode of the last sleep sequence.
 **
 ** This variable must survive ECU resets. */
STATIC VAR(uint8, ECUM_VAR_POWER_ON_INIT) EcuM_LastSleepMode = 0U;

/** \brief Last Reset Mode
 **
 ** This variable holds the reset mode of the last reset sequence.
 **
 ** This variable must survive ECU resets. */
STATIC VAR(uint8, ECUM_VAR_POWER_ON_INIT) EcuM_LastResetMode = 0U;

/** \brief Starting index of the buffer which stores the shutdown information.
 **
 ** This variable holds the starting point of the buffer which is having all
 ** the recent shutdown information. The shutdown information is retrieved
 ** with the help of EcuM_ShutdownInfoCurrentIdx, which points to the current
 ** location of the buffer. To get all the stored information, iterate the
 ** current index point till it reaches the start index. Once the buffer is
 ** filled to the maximum capacity, the starting index point shifts forward
 ** and works as an index to ring buffer.
 **
 ** This variable must survive ECU resets. */
STATIC VAR(uint8, ECUM_VAR_POWER_ON_INIT) EcuM_ShutdownInfoStartIdx = 0U;

/** \brief Current location of the buffer which stores the shutdown
 ** information.
 **
 ** This variable holds the current location of the buffer which stores the
 ** shutdown information. The stored shutdown inforamtion are retrieved by
 ** iterationg this index till it reaches EcuM_ShutdownInfoStartIdx.
 **
 ** This variable must survive ECU resets. */
STATIC VAR(uint8, ECUM_VAR_POWER_ON_INIT) EcuM_ShutdownInfoCurrentIdx = 0U;

/** \brief Index variable for getting the shutdown information by iteration
 **
 ** This variable is accessed by EcuM_GetMostRecentShutdown() and
 ** EcuM_GetNextRecentShutdown(). */
STATIC VAR(uint8, ECUM_VAR_POWER_ON_INIT) EcuM_NextShutdownInfoIdx = 0U;

#if ((ECUM_INCLUDE_DEM == STD_ON) && (ECUM_PROD_ERR_RAM_CHECK_FAILURE == TS_PROD_ERR_REP_TO_DEM))
/** \brief variable to store the status of a failed RAM check
 **
 ** Content of variable will be updated in SLEEP phase(Halt sequence)
 ** and will be evaluated after reset in up phase */
STATIC VAR(uint8, ECUM_VAR_POWER_ON_INIT) EcuM_RamCheckStatus = ECUM_RAM_CHECK_STATUS_UNTESTED;
#endif

#define ECUM_STOP_SEC_VAR_POWER_ON_INIT_8
#include <EcuM_MemMap.h>

/*---------------[no init variables]----------------------------------------- */

#define ECUM_START_SEC_VAR_CLEARED_8
#include <EcuM_MemMap.h>

/** \brief Current reset mode for Ecu Reset
 **
 ** This variable holds the current reset mode used in the next shutdown
 ** sequence if the selected shutdown target is ECUM_STATE_RESET. */
STATIC VAR(uint8, ECUM_VAR_CLEARED) EcuM_ResetMode;

/** \brief Currently selected Shutdown Target
 **
 ** This variable holds the current shutdown target used in the next shutdown
 ** sequence. */
STATIC VAR(EcuM_ASR40_StateType, ECUM_VAR_CLEARED) EcuM_ShutdownTarget;

/** \brief Currently selected Shutdown Cause
 **
 ** This variable holds the selected shutdown cause used for the next
 ** shutdown. */
STATIC VAR(EcuM_ASR40_ShutdownCauseType, ECUM_VAR_CLEARED) EcuM_ShutdownCause;

#define ECUM_STOP_SEC_VAR_CLEARED_8
#include <EcuM_MemMap.h>

#define ECUM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <EcuM_MemMap.h>

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
/** \brief Timeout for RUN state
 **
 ** This variable holds the timeout counter for the RUN state in fixed mode. */
STATIC VAR(uint32, ECUM_VAR_CLEARED) EcuM_RunTimeout;
#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_ON) */

#define ECUM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <EcuM_MemMap.h>

/*==================[external function definitions]===========================*/

#define ECUM_START_SEC_CODE
#include <EcuM_MemMap.h>

/*----------------------------------------------------------------------------
 * General
 *----------------------------------------------------------------------------*/

#if (ECUM_VERSION_INFO_API == STD_ON)
/*------------------[EcuM_GetVersionInfo]-------------------------------------*/

FUNC(void, ECUM_CODE) EcuM_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, ECUM_APPL_DATA) VersionInfoPtr
)
{
  DBG_ECUM_GETVERSIONINFO_ENTRY(VersionInfoPtr);

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR == VersionInfoPtr)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_GET_VERSION_INFO, ECUM_E_NULL_POINTER);
  }
  else
#endif
  {
    VersionInfoPtr->vendorID         = ECUM_VENDOR_ID;
    VersionInfoPtr->moduleID         = ECUM_MODULE_ID;
    VersionInfoPtr->sw_major_version = ECUM_SW_MAJOR_VERSION;
    VersionInfoPtr->sw_minor_version = ECUM_SW_MINOR_VERSION;
    VersionInfoPtr->sw_patch_version = ECUM_SW_PATCH_VERSION;
  }

  DBG_ECUM_GETVERSIONINFO_EXIT(VersionInfoPtr);
}
#endif

/*----------------------------------------------------------------------------
 * Initialization and Shutdown Sequences
 *----------------------------------------------------------------------------*/

/*------------------[EcuM_GoDown]---------------------------------------------*/

FUNC(Std_ReturnType, ECUM_CODE) EcuM_GoDown(uint16 caller)
{
  Std_ReturnType ret = E_OK;

#if (ECUM_MULTICORE_ENABLED == TRUE)
  Std_ReturnType ecuMSendSlaveCoreState;
#endif /* (ECUM_MULTICORE_ENABLED == TRUE) */

  DBG_ECUM_GODOWN_ENTRY(caller);

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == EcuM_GetCoreInstance()->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_DOWN, ECUM_E_UNINIT);
  }
  else
#endif
  {
    /* If Defensive behaviour enabled , then check if the given CallerID is in
     * the list of allowed CallerIDs. Report "ECUM_E_IMPROPER_CALLER " to DEM if
     * the check fails.
     *
     * return E_NOT_OK if the request not accepted. */

    /* The current implementation does not support the feature Defensive behaviour. */
    TS_PARAM_UNUSED(caller);

#if (ECUM_MULTICORE_ENABLED == TRUE)
    if (ECUM_CORE_ID_MASTER == ECUM_GET_CORE_ID())
    {
    /* !LINKSTO EcuM.Impl.CallingContext.MasterCoreOnly,2 */
      if (EcuM_SlaveCoreSync() == E_OK)
      {
        ret = EcuM_GoDownMaster();
      }
      else
      {
        ret = E_NOT_OK;
      }
    }
    /* !LINKSTO SWS_EcuM_04021,1 */
    else
    {

      EcuM_OnGoOffOne();

      BswM_Deinit();

      /* Synchronization data must be sent before SchM_Deinit() is called.
       * If  SchM_DeInit() is called first, the SchM will be set to "stopped" on the slave core.
       * Considering this, the Schedule Manager will not send any data because it first checks
       * if the SchM is set to "active" on the slave core. */
      ecuMSendSlaveCoreState = EcuM_Send_SlaveCoreReadyPort();
      ret = (ecuMSendSlaveCoreState == E_OK ) ? E_OK : E_NOT_OK;

      EcuM_SchM_Deinit();

      EcuM_GetCoreInstance()->State = ECUM_STATE_OFF;

    }

#else
    ret = EcuM_GoDownMaster();
#endif /* (ECUM_MULTICORE_ENABLED == TRUE) */
  }

  DBG_ECUM_GODOWN_EXIT(ret, caller);

  return ret;
}

/*------------------[EcuM_GoDownMaster]---------------------------------------*/

/* !LINKSTO SWS_EcuM_04019,1 */
STATIC FUNC(Std_ReturnType, ECUM_CODE) EcuM_GoDownMaster(void)
{
  Std_ReturnType ret = E_OK;

 P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance = EcuM_GetCoreInstance();

  /* This callout notifies the integrator that a shutdown sequence is about to
   * be entered. */
  EcuM_OnGoOffOne();

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
  EcuM_KillAllRUNRequested = FALSE;
#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_ON) */

  /* De-initialize the BSW Mode Manager. */
  BswM_Deinit();

  /* De-initialize the BSW Scheduler. */
  EcuM_SchM_Deinit();

  /* Check for new wakeup events that occur during shutdown. */
  if
  (
    (CoreInstance->WakeupStatus.Pending != 0U) ||
    (CoreInstance->WakeupStatus.Validated != 0U)
  )
  {
    /* !LINKSTO EcuM2756,1 */
    /* Set RESET as shutdown target.  This action shall only be carried out when
     * new wakeup events were detected. */
    EcuM_ShutdownTarget = ECUM_STATE_RESET;
  }

  /* Store the shutdown information at the current location of buffer before
   * shutting down. */
  EcuM_ShutdownInfo[EcuM_ShutdownInfoCurrentIdx].target = EcuM_ShutdownTarget;

  /* If the selected shutdown target is reset then store the reset mode. */
  if (EcuM_ShutdownTarget == ECUM_STATE_RESET)
  {
    EcuM_ShutdownInfo[EcuM_ShutdownInfoCurrentIdx].mode = EcuM_ResetMode;
  }
  else
  {
    /* The SWS does not specify explicitly what mode shall be saved when the
     * target is ECUM_STATE_OFF for the APIs EcuM_SelectShutdownCause(),
     * EcuM_GetShutdownCause(), EcuM_GetMostRecentShutdown and
     * EcuM_GetMostRecentShutdown().  The returned mode value is always 0U if
     * the shutdown target is ECUM_STATE_OFF since no mode exists for
     * ECUM_STATE_OFF. */
    EcuM_ShutdownInfo[EcuM_ShutdownInfoCurrentIdx].mode = 0U;
  }

  EcuM_ShutdownInfo[EcuM_ShutdownInfoCurrentIdx].cause  = EcuM_ShutdownCause;

  /* The returned time value is always set to 0 because alarm clock
   * functionality is not yet supported. */
  EcuM_ShutdownInfo[EcuM_ShutdownInfoCurrentIdx].sdTime = 0U;

  EcuM_ShutdownInfoCurrentIdx++;

  if (EcuM_ShutdownInfoCurrentIdx == ECUM_STORED_SHUTDOWN_OPERATIONS)
  {
    /* Reset the current buffer pointer if it reached max value. */
    EcuM_ShutdownInfoCurrentIdx = 0U;
  }

  if (EcuM_ShutdownInfoCurrentIdx == EcuM_ShutdownInfoStartIdx)
  {
    /* If the current index point of the buffer reaches the
     * EcuM_ShutdownInfoStartIdx then buffer is filled with maximum storage.
     * The EcuM_ShutdownInfoStartIdx has to move forward one step. */
    EcuM_ShutdownInfoStartIdx++;

    if (EcuM_ShutdownInfoStartIdx == ECUM_STORED_SHUTDOWN_OPERATIONS)
    {
      /* Reset the start index pointer to zero if it reaches the max value
       * to rotate throughout the buffer. */
      EcuM_ShutdownInfoStartIdx = 0U;
    }
  }

  EcuM_GetMasterCoreInstance()->State = ECUM_STATE_OFF;

#if (ECUM_MULTICORE_ENABLED == TRUE)
  {
    /* Master Core SHUTDOWN
     * The E_OK is passed to the ShutdownHook and is
     * not evaluated by the OS. A neutral value has
     * chosen in this situation.
     * THIS CALL WILL NOT RETURN.
     */
    ShutdownAllCores(E_OK);
  }
#else
  ShutdownOS(0U);              /* THIS CALL WILL NOT RETURN */
#endif /* (ECUM_MULTICORE_ENABLED == TRUE) */

  return ret;
}

/*------------------[EcuM_GoHalt]---------------------------------------------*/

FUNC(Std_ReturnType, ECUM_CODE) EcuM_GoHalt(void)
{
  Std_ReturnType ret = E_NOT_OK;

  DBG_ECUM_GOHALT_ENTRY();

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == EcuM_GetCoreInstance()->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_HALT, ECUM_E_UNINIT);
  }
  else
#endif
  {
    /* GOSLEEP SEQUENCE */
    EcuM_GoSleep();

#if (ECUM_MULTICORE_ENABLED == TRUE)
    if (ECUM_CORE_ID_MASTER == ECUM_GET_CORE_ID())
    {
      /* !LINKSTO SWS_EcuM_04024,1 */
      ret = EcuM_GoHaltMaster();
    }
    else
    {
      /* !LINKSTO SWS_EcuM_04028,1 */
      ret = EcuM_GoHaltSlave();
    }
#else
    EcuM_HaltSequence();
#endif

    /* WAKEUPRESTART SEQUENCE */
    EcuM_WakeupRestart();
  }
#if (ECUM_MULTICORE_ENABLED == FALSE)
  ret = E_OK;
#endif
  DBG_ECUM_GOHALT_EXIT(ret);
  return ret;
}

/*------------------[EcuM_HaltSequence]---------------------------------------*/
STATIC FUNC(void, ECUM_CODE) EcuM_HaltSequence(void)
{
   P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance = EcuM_GetCoreInstance();
   /* HALT SEQUENCE */

    /* At this point, a wakeup event may have been reported via an interrupt and
     * it may be pending or (automatically) validated.
     *
     * Within the EcuM_PreHalt() callout the interrupts may be disabled for
     * architectures on which disabled interrupts may still lead to a MCU wakeup.
     * Disabling interrupts here prevents ISR routines that may be executed
     * between the checking for pending events and call to Mcu_SetMode(). Such an
     * interruption would set a pending wakeup event, but MCU would still go into
     * halt without reacting to the event. */
    /* !LINKSTO EcuM.Sleep,1 */
    EcuM_PreHalt();

    /* Invoke callout to generate RamHash */
    EcuM_GenerateRamHash();

    if
    (
      (CoreInstance->WakeupStatus.Pending == 0U) &&
      (CoreInstance->WakeupStatus.Validated == 0U)
    )
    {
      /* The CPU is halted here and the function Mcu_SetMode() does not return
       * until a wakeup interrupt has occurred. */
      Mcu_SetMode(EcuM_SleepModeConfigList[CoreInstance->SleepMode].mcuMode);

      /* At this point the MCU has received a wakeup interrupt and the function
       * EcuM_SetWakeupEvent() was called. */
    }

    /* Revert any actions from the EcuM_PreHalt() call. */
    EcuM_PostHalt();

    /* Invoke the callout to check the RamHash. */
    if (0U == EcuM_CheckRamHash())
    {
      /* It is left integrator's discretion to allow EcuM_ErrorHook() to
       * rely the error to the DEM. The following call to EcuM_ErrorHook()
       * will never return. Instead the failing RamCheck may be stored in a
       * flag which survives a reset. */

#if (ECUM_DEV_ERROR_DETECT == STD_ON) && (ECUM_PROD_ERR_RAM_CHECK_FAILURE == TS_PROD_ERR_REP_TO_DET)
      /* The DET may still accept a call here. */
      ECUM_DET_REPORT_ERROR(ECUM_SID_HALT, ECUM_E_DEMTODET_RAM_CHECK_FAILED);
#endif

#if (ECUM_INCLUDE_DEM == STD_ON) && (ECUM_PROD_ERR_RAM_CHECK_FAILURE == TS_PROD_ERR_REP_TO_DEM)
      EcuM_RamCheckStatus = ECUM_RAM_CHECK_STATUS_FAILED;
#endif

      /* This call should never return. */
      EcuM_ErrorHook(ECUM_RAM_CHECK_FAILED);
    }
    else
    {
#if (ECUM_INCLUDE_DEM == STD_ON) && (ECUM_PROD_ERR_RAM_CHECK_FAILURE == TS_PROD_ERR_REP_TO_DEM)
      EcuM_RamCheckStatus = ECUM_RAM_CHECK_STATUS_PASSED;
#endif
    }
}

#if (ECUM_MULTICORE_ENABLED == TRUE)
/*------------------[EcuM_GoHaltMaster]---------------------------------------*/
STATIC FUNC(Std_ReturnType, ECUM_CODE) EcuM_GoHaltMaster(void)
{
  Std_ReturnType ecuMSendMasterCoreState = E_NOT_OK;

  if (EcuM_SlaveCoreSync() == E_OK)
  {
    EcuM_HaltSequence();
    ecuMSendMasterCoreState = SchM_Send_EcuM_EcuM_MasterCoreReadyPort(TRUE);
  }

  return (ecuMSendMasterCoreState == SCHM_E_OK) ? E_OK : E_NOT_OK;
}

/*------------------[EcuM_GoHaltSlave]---------------------------------------*/
STATIC FUNC(Std_ReturnType, ECUM_CODE) EcuM_GoHaltSlave(void)
{
  P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance = EcuM_GetCoreInstance();
  Std_ReturnType retSlaveCoreSend;
  Std_ReturnType retMasterCoreReceive;

  retSlaveCoreSend = EcuM_Send_SlaveCoreReadyPort();

  if
  (
    (CoreInstance->WakeupStatus.Pending == 0U) &&
    (CoreInstance->WakeupStatus.Validated == 0U)
  )
  {
    Mcu_SetMode(EcuM_SleepModeConfigList[CoreInstance->SleepMode].mcuMode);
  }
  retMasterCoreReceive = EcuM_MasterCoreSync();

  return ((retSlaveCoreSend == E_OK) && (retMasterCoreReceive == E_OK)) ? E_OK : E_NOT_OK;
}
#endif /* #if (ECUM_MULTICORE_ENABLED == TRUE) */

/*------------------[EcuM_GoPoll]---------------------------------------------*/

/* !LINKSTO EcuM.MasterCore.Poll.Sequence,1, EcuM.SlaveCore.Poll.Sequence,1 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_GoPoll(void)
{

  P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance = EcuM_GetCoreInstance();
  Std_ReturnType ret = E_NOT_OK;
#if (ECUM_MULTICORE_ENABLED == TRUE)
  Std_ReturnType ecuMSendMasterCoreState;
#endif

  DBG_ECUM_GOPOLL_ENTRY();

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == CoreInstance->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_POLL, ECUM_E_UNINIT);
  }
  else
#endif
  {
    /* GOSLEEP SEQUENCE */
    EcuM_GoSleep();

    /* POLL SEQUENCE */
    Mcu_SetMode(EcuM_SleepModeConfigList[CoreInstance->SleepMode].mcuMode);

    /* Blocking synchronous loop till a wakeup is detected.
     *
     * The scheduling of the OS is deactivated during GoSleep sequence
     * (RES_SCHEDULER is taken) so that the EcuM main function is not periodically
     * called. */
    while
    (
      (CoreInstance->WakeupStatus.Pending == 0U) &&
      (CoreInstance->WakeupStatus.Validated == 0U)
    )
    {
      /* Invoke the callout function. */
      EcuM_SleepActivity();

      /* Check the enabled wakeup sources. */
      EcuM_CheckWakeup(EcuM_SleepModeConfigList[CoreInstance->SleepMode].wakeupSource & ECUM_WKSMASKALL);
    }

    /* Notification to BswM regarding the pending and validated wakeup events is
     * done within the EcuM_MainFuntion */

#if (ECUM_MULTICORE_ENABLED == TRUE)
    if (ECUM_CORE_ID_MASTER == ECUM_GET_CORE_ID())
    {
        ecuMSendMasterCoreState = SchM_Send_EcuM_EcuM_MasterCoreReadyPort(TRUE);
        ret = (ecuMSendMasterCoreState == SCHM_E_OK) ? E_OK : E_NOT_OK;
    }
    else
    {
      ret = EcuM_MasterCoreSync();
    }
#endif /* (ECUM_MULTICORE_ENABLED == TRUE) */

    /* WAKEUPRESTART SEQUENCE */
    EcuM_WakeupRestart();
#if (ECUM_MULTICORE_ENABLED == FALSE)
    ret = E_OK;
#endif/* ECUM_MULTICORE_ENABLED == FALSE */
  }

  DBG_ECUM_GOPOLL_EXIT(ret);
  return ret;
}

STATIC FUNC(void, ECUM_CODE) EcuM_SetSleepOrResetMode(void)
{
  /* Set the default shutdown target. */
  EcuM_ShutdownTarget = EcuM_ConfigPtr->DefaultShutdownTarget;

  if (EcuM_ShutdownTarget == ECUM_STATE_SLEEP)
  {
    /* Set the default sleep mode. It is ensured by configuration checks that
     * the mode will be a sleep mode if the shutdown target is selected as
     * sleep.*/
    EcuM_GetCoreInstance()->SleepMode = (EcuM_ConfigPtr->DefaultMode);
  }
  else if (EcuM_ShutdownTarget == ECUM_STATE_RESET)
  {
    /* Set the default reset mode. It is ensured by configuration checks
     * that mode will be a reset mode if the shutdown target is selected as
     * reset.*/
    EcuM_ResetMode = EcuM_ConfigPtr->DefaultMode;
  }
  else
  {
    /* Do nothing */
  }
}

STATIC FUNC(void, ECUM_CODE) EcuM_SearchResetReason(void)
{
  uint8_least wks;
  Mcu_ResetType mcuResetReason; /* Reset type fetched from Mcu */
  boolean reasonFound = FALSE;

  /* Set wakeup events associated to this Mcu reset reason. */
  mcuResetReason = Mcu_GetResetReason();

  /* Set wakeup events resulting from reset reasons */
  for (wks = 0U; ((wks < ECUM_WKSCONFIGNUM) && (reasonFound != TRUE)); wks++)
  {
    P2CONST(EcuM_WksConfigType, AUTOMATIC, ECUM_CONST) wksPtr = &EcuM_WksConfigList[wks];

    /* If the current wakeup source (wksPtr) has been associated to the reset
    * reason, then set the wakeup event. */
    if ((wksPtr->ResetReason != MCU_RESET_UNDEFINED) && (wksPtr->ResetReason == mcuResetReason))
    {
      /* !LINKSTO EcuM2171,2 */
      EcuM_SetWakeupEvent((EcuM_WakeupSourceType)((EcuM_WakeupSourceType)1U << wksPtr->WksId));
      reasonFound = TRUE;
    }
  }
}

STATIC FUNC(void, ECUM_CODE) EcuM_StartPreOS(void)
{
#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
  EcuM_KillAllRUNRequested = FALSE;
#endif

  /* On ECUs with programmable interrupt priorities, these priorities must be
   * set before the OS is started. */
  EcuM_AL_SetProgrammableInterrupts();

  /* !LINKSTO EcuM2980,1 */
  /* !description The ECU Manager module does not report the errors during their
   *              initialization. */
  ECUM_AL_DRIVERINITONE(); /* initialize configured drivers */

  /* Set the default boot target. */
  EcuM_BootTarget[0] = ECUM_BOOT_TARGET_APP;
  EcuM_BootTarget[1] = ECUM_BOOT_TARGET_APP;

  EcuM_SearchResetReason();

  EcuM_SetSleepOrResetMode();
}

#if (ECUM_MULTICORE_ENABLED == FALSE)
STATIC FUNC(void, ECUM_CODE) EcuM_Init_SingleCore(void)
{
  EcuM_StartPreOS();
  StartOS(EcuM_ConfigPtr->DefaultAppMode);
}
#endif

#if (ECUM_MULTICORE_ENABLED == TRUE)

STATIC FUNC(void, ECUM_CODE) EcuM_Init_MultiCore(void)
{
  /* MASTER CORE STARTUP */
  /* !LINKSTO SWS_EcuM_04015,1 */
  if (ECUM_CORE_ID_MASTER == ECUM_GET_CORE_ID())
  {
    EcuM_StartPreOS();
  }
  if (ECUM_CORE_ID_INITIAL == ECUM_GET_CORE_ID())
  {
    EcuM_StartAllCores();
  }
  /* Slave Core STARTUP */
  /* !LINKSTO SWS_EcuM_04017,1 */
  if (ECUM_CORE_ID_MASTER != ECUM_GET_CORE_ID())
  {
    EcuM_GetCoreInstance()->SleepMode = (EcuM_ConfigPtr->DefaultMode);
  }
  /* start the operating system */
  /* !LINKSTO SWS_EcuM_04093,1 */
  StartOS(EcuM_ConfigPtr->DefaultAppMode);
}
#endif /* (ECUM_MULTICORE_ENABLED == TRUE) */

/*------------------[EcuM_Init]-----------------------------------------------*/

FUNC(void, ECUM_CODE) EcuM_Init(void)
{
 P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance = EcuM_GetCoreInstance();

  DBG_ECUM_INIT_ENTRY();

  *CoreInstance = EcuM_InitialContext;

  CoreInstance->State = ECUM_STATE_STARTUP_ONE;

#if (ECUM_MULTICORE_ENABLED == TRUE)
  if (ECUM_CORE_ID_MASTER == ECUM_GET_CORE_ID())
  {
#endif
    /* Initialize the configured drivers.*/
    ECUM_AL_DRIVERINITZERO();
#if (ECUM_MULTICORE_ENABLED == TRUE)
  }
#endif

  EcuM_ConfigPtr = EcuM_DeterminePbConfiguration();

  if (EcuM_ConfigPtr == NULL_PTR)
  {
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    ECUM_DET_REPORT_ERROR(ECUM_SID_INIT, ECUM_E_NULL_POINTER);
#endif
  }
  else if (E_OK != EcuM_IsValidConfig(EcuM_ConfigPtr))
  {
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    /* Check that configuration pointer is valid. */
    /* !LINKSTO EcuM.Impl.PBVariant.WrongInitConfigParameter,1 */
    ECUM_DET_REPORT_ERROR(ECUM_SID_INIT, ECUM_E_WRONG_CONFIG_PARAM);
#endif
  }
  else
  {
#if (ECUM_MULTICORE_ENABLED == TRUE)
    EcuM_Init_MultiCore();
#else
    EcuM_Init_SingleCore();
#endif
  }
  DBG_ECUM_INIT_EXIT();
}
/*------------------[EcuM_StartupTwo]-----------------------------------------*/

/* !LINKSTO SWS_EcuM_04016,1, SWS_EcuM_04018,1 */
FUNC(void, ECUM_CODE) EcuM_StartupTwo(void)
{
  P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance = EcuM_GetCoreInstance();

  DBG_ECUM_STARTUPTWO_ENTRY();

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == CoreInstance->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_STARTUP_TWO, ECUM_E_UNINIT);
  }
  else
#endif /* (ECUM_DEV_ERROR_DETECT == STD_ON) */
  {
    CoreInstance->State = ECUM_STATE_STARTUP_TWO;

    /* !LINKSTO SWS_EcuM_04093,1 */
    EcuM_SchM_Init();

    /* !LINKSTO SWS_EcuM_04014,1 */
    BswM_Init(NULL_PTR);

    /* UP PHASE */
    CoreInstance->State = ECUM_STATE_RUN;
    /* !LINKSTO EcuM.BswMStateIndication,1 */
    BswM_EcuM_CurrentState(CoreInstance->State);
  }

  DBG_ECUM_STARTUPTWO_EXIT();
}

/*------------------[EcuM_Shutdown]-------------------------------------------*/

FUNC(void, ECUM_CODE) EcuM_Shutdown(void)
{
  DBG_ECUM_SHUTDOWN_ENTRY();

#if (ECUM_MULTICORE_ENABLED == TRUE)
  /* !LINKSTO SWS_EcuM_04020,1 */
  if (ECUM_CORE_ID_MASTER == ECUM_GET_CORE_ID())
  {
#endif /* (ECUM_MULTICORE_ENABLED == TRUE) */

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == EcuM_GetMasterCoreInstance()->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_SHUTDOWN, ECUM_E_UNINIT);
  }
  else
#endif /* (ECUM_DEV_ERROR_DETECT == STD_ON) */
  {
    EcuM_OnGoOffTwo();

    /* Store the last shutdown target. */
    EcuM_LastShutdownTarget = EcuM_ShutdownTarget;

    if (EcuM_ShutdownTarget == ECUM_STATE_RESET)
    {
      EcuM_AL_Reset(EcuM_ResetMode);
    }
    else
    {
      EcuM_AL_SwitchOff();
    }
  }

#if (ECUM_MULTICORE_ENABLED == TRUE)
  }  /* end if (ECUM_CORE_ID_MASTER == ECUM_GET_CORE_ID()) */
  /* !LINKSTO SWS_EcuM_04022,1 */
  else
  {
    EcuM_OnGoOffTwo();
  }
#endif /* (ECUM_MULTICORE_ENABLED == TRUE) */

  DBG_ECUM_SHUTDOWN_EXIT();
}

/*----------------------------------------------------------------------------
 * Shutdown management
 *----------------------------------------------------------------------------*/

/*------------------[EcuM_SelectShutdownTarget]-------------------------------*/

STATIC FUNC(Std_ReturnType, ECUM_CODE) EcuM_SelectShutdownTargetSequence
(
  EcuM_ASR40_StateType target,
  uint8                mode
)
{
  Std_ReturnType ret = E_NOT_OK;
#if (ECUM_MULTICORE_ENABLED == TRUE)
  boolean sendSleepMode = FALSE;
  Std_ReturnType schMSendResult;
#endif

  DBG_ECUM_ASR40_SELECTSHUTDOWNTARGET_ENTRY(target,mode);

  /* !LINKSTO SWS_EcuM_04094,1 */
#if (ECUM_MULTICORE_ENABLED == TRUE)
  if (ECUM_CORE_ID_MASTER != ECUM_GET_CORE_ID())
  {
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    ECUM_DET_REPORT_ERROR(ECUM_SID_SEL_SHUT_TAR, ECUM_E_WRONG_CONTEXT);
#endif
  }
  else
#endif /* (ECUM_MULTICORE_ENABLED == TRUE) */
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == EcuM_GetMasterCoreInstance()->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_SEL_SHUT_TAR, ECUM_E_UNINIT);
  }
  else if
  (
    (target != ECUM_STATE_SLEEP) &&
    ((target != ECUM_STATE_RESET) && (target != ECUM_STATE_OFF))
  )
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_SEL_SHUT_TAR, ECUM_E_STATE_PAR_OUT_OF_RANGE);
  }
  else if ((target == ECUM_STATE_SLEEP) && (mode >= ECUM_SLEEPMODECONFIGNUM))
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_SEL_SHUT_TAR, ECUM_E_INVALID_PAR);
  }
  else
#endif /* (ECUM_DEV_ERROR_DETECT == STD_ON) */
  {

    EcuM_EnterExclusiveArea();

    if (target == ECUM_STATE_SLEEP)
    {
      EcuM_ShutdownTarget = target;
      EcuM_GetCoreInstance()->SleepMode = mode;
#if (ECUM_MULTICORE_ENABLED == TRUE)
      sendSleepMode = TRUE;
#endif
    }
    else if (target == ECUM_STATE_RESET)
    {
      EcuM_ShutdownTarget = target;
      EcuM_ResetMode = mode;
    }
    else if (target == ECUM_STATE_OFF)
    {
      EcuM_ShutdownTarget = target;
    }
    else
    {
      /* Ignore the request if the target is not SLEEP, RESET or OFF. */
    }

    EcuM_ExitExclusiveArea();

#if (ECUM_MULTICORE_ENABLED == TRUE)
    if (sendSleepMode == TRUE)
    {
      schMSendResult = SchM_Send_EcuM_EcuM_SetSleepMode(mode);
      ret = (schMSendResult == SCHM_E_OK) ? E_OK : E_NOT_OK;
    }
    else
#endif
    {
      ret = E_OK;
    }
  }

  DBG_ECUM_ASR40_SELECTSHUTDOWNTARGET_EXIT(ret,target,mode);
  return ret;
}

/* !LINKSTO EcuM.EB.ASR32.EcuM106,1 */
/* If the Rte interface and the multicore functionality are enabled, the prototype for EcuM_SelectShutdownTarget
   is exposed by SchM_EcuM.h since a BswSwcMapping is present in the EcuM BSW module description file */
#if !((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_INCLUDE_RTE == STD_ON))
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_SelectShutdownTarget
(
  EcuM_ASR40_StateType target,
  uint8                mode
)
{
  Std_ReturnType ret = E_NOT_OK;
  ret = EcuM_SelectShutdownTargetSequence(target, mode);

  return ret;
}
#else
FUNC(Std_ReturnType, ECUM_CODE) EcuM_SelectShutdownTarget
(
  EcuM_StateType target,
  uint8          mode
)
{
  Std_ReturnType ret = E_NOT_OK;
  ret = EcuM_SelectShutdownTargetSequence(target, mode);

  return ret;
}
#endif

#if ((ECUM_EB_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (ECUM_EB_DEFAULT_ASR_SERVICE_API == ECUM_EB_SERVICE_API_ASR32))
/* !LINKSTO EcuM.EB.ASR32.EcuM107,1 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR32_SelectShutdownTarget
(
  EcuM_ASR32_StateType target,
  uint8                mode
)
{
  Std_ReturnType ret = E_NOT_OK; /* return value */

  /* EcuM_ASR40_StateType and EcuM_ASR32_StateType are referencing to
   * the same base type and the state definitions are identical.
   * Therefore no argument type mapping is done. */
#if !((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_INCLUDE_RTE == STD_ON))
  ret = EcuM_ASR40_SelectShutdownTarget(target, mode);
#else
  ret = EcuM_SelectShutdownTarget(target, mode);
#endif
  return ret;
}
#endif

/*------------------[EcuM_GetShutdownTarget]----------------------------------*/

/* !LINKSTO EcuM.EB.ASR32.EcuM106,1 */
/* Deviation MISRAC2012-1 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_GetShutdownTarget
(
  P2VAR(EcuM_ASR40_StateType, AUTOMATIC, ECUM_APPL_DATA) shutdownTarget,
  P2VAR(uint8, AUTOMATIC, ECUM_APPL_DATA)                sleepMode
)
{
  Std_ReturnType ret = E_NOT_OK;

  DBG_ECUM_ASR40_GETSHUTDOWNTARGET_ENTRY(shutdownTarget,sleepMode);

  /* !LINKSTO SWS_EcuM_04094,1 */
#if (ECUM_MULTICORE_ENABLED == TRUE)
  if (ECUM_CORE_ID_MASTER != ECUM_GET_CORE_ID())
  {
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    ECUM_DET_REPORT_ERROR(ECUM_SID_GET_SHUT_TAR, ECUM_E_WRONG_CONTEXT);
    ret = E_NOT_OK;
#endif
  }
  else
#endif /* ECUM_MULTICORE_ENABLED == TRUE */
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == EcuM_GetMasterCoreInstance()->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_GET_SHUT_TAR, ECUM_E_UNINIT);
    ret = E_NOT_OK;
  }
  else
#endif
  {
    if (shutdownTarget != NULL_PTR)
    {
      *shutdownTarget = EcuM_ShutdownTarget;
    }
    else
    {
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
      ECUM_DET_REPORT_ERROR(ECUM_SID_GET_SHUT_TAR,ECUM_E_NULL_POINTER);
#endif
    }

    if (sleepMode != NULL_PTR)
    {
      if (EcuM_ShutdownTarget == ECUM_STATE_SLEEP)
      {
        *sleepMode = EcuM_GetMasterCoreInstance()->SleepMode;
      }
      else if (EcuM_ShutdownTarget == ECUM_STATE_RESET)
      {
        *sleepMode = EcuM_ResetMode;
      }
      else
      {
        /* Ignore the mode if the target is not SLEEP or RESET. */
      }
    }
    else
    {
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
      ECUM_DET_REPORT_ERROR(ECUM_SID_GET_SHUT_TAR,ECUM_E_NULL_POINTER);
#endif
    }

    /* check if all activities are done successfully */
    if ((sleepMode != NULL_PTR) && (shutdownTarget != NULL_PTR))
    {
      ret = E_OK;
    }
    else
    {
      ret = E_NOT_OK;
    }
  }

  DBG_ECUM_ASR40_GETSHUTDOWNTARGET_EXIT(ret,shutdownTarget,sleepMode);

  return ret;
}

#if ((ECUM_EB_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (ECUM_EB_DEFAULT_ASR_SERVICE_API == ECUM_EB_SERVICE_API_ASR32))
/* !LINKSTO EcuM.EB.ASR32.EcuM107,1 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR32_GetShutdownTarget
(
  P2VAR(EcuM_ASR32_StateType, AUTOMATIC, ECUM_APPL_DATA) shutdownTarget,
  P2VAR(uint8, AUTOMATIC, ECUM_APPL_DATA)                sleepMode
)
{
  Std_ReturnType ret = E_NOT_OK;

  /* EcuM_ASR40_StateType and EcuM_ASR32_StateType are referencing to
   * the same base type and the state definitions are identical.
   * Therefore no argument type mapping is done. */
  ret = EcuM_ASR40_GetShutdownTarget(shutdownTarget, sleepMode);

  return ret;
}
#endif

/*------------------[EcuM_GetLastShutdownTarget]------------------------------*/

/* !LINKSTO EcuM.EB.ASR32.EcuM106,1 */
/* Deviation MISRAC2012-1 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_GetLastShutdownTarget
(
  P2VAR(EcuM_ASR40_StateType, AUTOMATIC, ECUM_APPL_DATA) shutdownTarget,
  P2VAR(uint8, AUTOMATIC, ECUM_APPL_DATA)                sleepMode
)
{
  Std_ReturnType ret = E_NOT_OK;

  DBG_ECUM_ASR40_GETLASTSHUTDOWNTARGET_ENTRY(shutdownTarget,sleepMode);

#if (ECUM_MULTICORE_ENABLED == TRUE)
  if (ECUM_CORE_ID_MASTER != ECUM_GET_CORE_ID())
  {
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    ECUM_DET_REPORT_ERROR(ECUM_SID_GET_LAST_SHUT_TAR, ECUM_E_WRONG_CONTEXT);
    ret = E_NOT_OK;
#endif
  }
  else
#endif /* ECUM_MULTICORE_ENABLED == TRUE */
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == EcuM_GetMasterCoreInstance()->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_GET_LAST_SHUT_TAR, ECUM_E_UNINIT);
    ret = E_NOT_OK;
  }
  else
#endif
  {
    if (shutdownTarget != NULL_PTR)
    {
      *shutdownTarget = EcuM_LastShutdownTarget;
    }
    else
    {
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
      ECUM_DET_REPORT_ERROR(ECUM_SID_GET_LAST_SHUT_TAR, ECUM_E_NULL_POINTER);
#endif
    }

    if (sleepMode != NULL_PTR)
    {
      if (EcuM_LastShutdownTarget == ECUM_STATE_SLEEP)
      {
        /* !LINKSTO EcuM2336,1 */
        *sleepMode = EcuM_LastSleepMode;
      }
      else if (EcuM_LastShutdownTarget == ECUM_STATE_RESET)
      {
        /* !LINKSTO EcuM2336,1 */
        *sleepMode = EcuM_LastResetMode;
      }
      else
      {
        /* Ignore the mode if the target is not SLEEP or RESET. */
      }
    }
    else
    {
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
      ECUM_DET_REPORT_ERROR(ECUM_SID_GET_LAST_SHUT_TAR,ECUM_E_NULL_POINTER);
#endif
    }
    /* check if all activities are done successfully */
    if ((shutdownTarget != NULL_PTR) && (sleepMode != NULL_PTR))
    {
      ret = E_OK;
    }
    else
    {
      ret = E_NOT_OK;
    }
  }
  DBG_ECUM_ASR40_GETLASTSHUTDOWNTARGET_EXIT(ret,shutdownTarget,sleepMode);
  return ret;
}

/* !LINKSTO EcuM.EB.ASR32.EcuM107,1 */
#if ((ECUM_EB_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (ECUM_EB_DEFAULT_ASR_SERVICE_API == ECUM_EB_SERVICE_API_ASR32))
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR32_GetLastShutdownTarget
(
  P2VAR(EcuM_ASR32_StateType, AUTOMATIC, ECUM_APPL_DATA) shutdownTarget,
  P2VAR(uint8, AUTOMATIC, ECUM_APPL_DATA)                sleepMode
)
{
  Std_ReturnType ret = E_NOT_OK;

  /* EcuM_ASR40_StateType and EcuM_ASR32_StateType are referencing to
   * the same base type and the state definitions are identical.
   * Therefore no argument type mapping is done. */
  ret = EcuM_ASR40_GetLastShutdownTarget(shutdownTarget, sleepMode);

  return ret;
}
#endif

/*------------------[EcuM_SelectShutdownCause]--------------------------------*/

/* !LINKSTO EcuM.EB.ASR32.EcuM106,1 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_SelectShutdownCause
(
  EcuM_ASR40_ShutdownCauseType target
)
{
  Std_ReturnType ret = E_NOT_OK;
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  boolean detOk = TRUE;
  uint8_least idx;
#endif

  DBG_ECUM_ASR40_SELECTSHUTDOWNCAUSE_ENTRY(target);

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  /* Check the shutdown cause is also configured in EcuM */
  for(idx = 0U; idx < ECUM_SHUTDOWNCAUSECONFIGNUM; idx++)
  {
    if (target == EcuM_ValidShutdownCause[idx])
    {
      break;
    }
  }

  if (ECUM_STATE_UNINIT == EcuM_GetMasterCoreInstance()->State)
  {
    detOk = FALSE;
    ECUM_DET_REPORT_ERROR(ECUM_SID_SEL_SHUT_CAUSE, ECUM_E_UNINIT);
  }
  else if (idx >= ECUM_SHUTDOWNCAUSECONFIGNUM)
  {
    detOk = FALSE;
    ECUM_DET_REPORT_ERROR(ECUM_SID_SEL_SHUT_CAUSE, ECUM_E_INVALID_PAR);
  }
#if (ECUM_MULTICORE_ENABLED == TRUE)
  else if (ECUM_CORE_ID_MASTER != ECUM_GET_CORE_ID())
  {
    detOk = FALSE;
    ECUM_DET_REPORT_ERROR(ECUM_SID_SEL_SHUT_CAUSE, ECUM_E_WRONG_CONTEXT);
  }
#endif /* ECUM_MULTICORE_ENABLED == TRUE */
  else
  {
    /* NOTHING TO DO */
  }
#endif /* ECUM_DEV_ERROR_DETECT == STD_ON */
#if (ECUM_MULTICORE_ENABLED == TRUE)
  if (ECUM_CORE_ID_MASTER == ECUM_GET_CORE_ID())
  {
#endif
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    if (TRUE == detOk)
    {
#endif
      ret = E_OK;
      EcuM_ShutdownCause = target;
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    }
#endif
#if (ECUM_MULTICORE_ENABLED == TRUE)
  }
#endif
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  else
  {
    ret = E_NOT_OK;
  }
#endif

  DBG_ECUM_ASR40_SELECTSHUTDOWNCAUSE_EXIT(ret,target);
  return ret;
}

/*------------------[EcuM_GetShutdownCause]-----------------------------------*/

/* !LINKSTO EcuM.EB.ASR32.EcuM106,1 */
/* Deviation MISRAC2012-1 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_GetShutdownCause
(
  P2VAR(EcuM_ASR40_ShutdownCauseType, AUTOMATIC, ECUM_APPL_DATA) shutdownCause
)
{
  Std_ReturnType ret = E_NOT_OK;

  DBG_ECUM_ASR40_GETSHUTDOWNCAUSE_ENTRY(shutdownCause);

#if (ECUM_MULTICORE_ENABLED == TRUE)
  if (ECUM_CORE_ID_MASTER != ECUM_GET_CORE_ID())
  {
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    ECUM_DET_REPORT_ERROR(ECUM_SID_GET_SHUT_CAUSE, ECUM_E_WRONG_CONTEXT);
#endif
  }
  else
#endif /* ECUM_MULTICORE_ENABLED == TRUE */
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == EcuM_GetMasterCoreInstance()->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_GET_SHUT_CAUSE, ECUM_E_UNINIT);
  }
  else
#endif
  {
    if (shutdownCause != NULL_PTR)
    {
      *shutdownCause = EcuM_ShutdownCause;
      ret = E_OK;
    }
    else
    {
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
      ECUM_DET_REPORT_ERROR(ECUM_SID_GET_SHUT_CAUSE,ECUM_E_NULL_POINTER);
#endif
    }
  }

  DBG_ECUM_ASR40_GETSHUTDOWNCAUSE_EXIT(ret,shutdownCause);

  return ret;
}

/*------------------[EcuM_GetMostRecentShutdown]------------------------------*/

/* !LINKSTO EcuM.EB.ASR32.EcuM106,1 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_GetMostRecentShutdown
(
  P2VAR(EcuM_ASR40_StateType, AUTOMATIC, ECUM_APPL_DATA)          target,
  P2VAR(uint8, AUTOMATIC, ECUM_APPL_DATA)                         mode,
  P2VAR(EcuM_ASR40_ShutdownCauseType, AUTOMATIC, ECUM_APPL_DATA)  cause,
  /* Deviation MISRAC2012-2 */
  P2VAR(uint32, AUTOMATIC, ECUM_APPL_DATA)                        time
)
{
  Std_ReturnType ret = E_NOT_OK;

  DBG_ECUM_ASR40_GETMOSTRECENTSHUTDOWN_ENTRY(target, mode, cause, time);

#if (ECUM_MULTICORE_ENABLED == TRUE)
  if (ECUM_CORE_ID_MASTER != ECUM_GET_CORE_ID())
  {
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    ECUM_DET_REPORT_ERROR(ECUM_SID_GET_RCNT_SHUT_CAUSE, ECUM_E_WRONG_CONTEXT);
#endif
  }
  else
#endif /* ECUM_MULTICORE_ENABLED == TRUE */
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == EcuM_GetMasterCoreInstance()->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_GET_RCNT_SHUT_CAUSE, ECUM_E_UNINIT);
  }
  else
#endif
  {
    if
    (
      ((target == NULL_PTR) || (mode == NULL_PTR)) ||
      ((cause == NULL_PTR) || (time == NULL_PTR))
    )
    {
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
      ECUM_DET_REPORT_ERROR(ECUM_SID_GET_RCNT_SHUT_CAUSE, ECUM_E_NULL_POINTER);
#endif
    }
    else
    {
      EcuM_EnterExclusiveArea();
      EcuM_NextShutdownInfoIdx = EcuM_ShutdownInfoCurrentIdx;

      /* Data is available only when the start index and the current index are
       * different. Return E_NOT_OK if there is no data. */
      if (EcuM_NextShutdownInfoIdx != EcuM_ShutdownInfoStartIdx)
      {
        /*  Most recent shutdown information is always stored just before the
         *  location of EcuM_ShutdownInfoCurrentIdx. If the current location is
         *  zero then load the max value to loop throughout the buffer. */
        if (EcuM_NextShutdownInfoIdx == 0U)
        {
          EcuM_NextShutdownInfoIdx = ECUM_STORED_SHUTDOWN_OPERATIONS - 1U;
        }
        else
        {
          EcuM_NextShutdownInfoIdx--;
        }

        /* Get the most recent shutdown information using the current index
         * point of the shutdown info buffer. */
        *target = EcuM_ShutdownInfo[EcuM_NextShutdownInfoIdx].target;
        *mode   = EcuM_ShutdownInfo[EcuM_NextShutdownInfoIdx].mode;
        *cause  = EcuM_ShutdownInfo[EcuM_NextShutdownInfoIdx].cause;
        *time   = EcuM_ShutdownInfo[EcuM_NextShutdownInfoIdx].sdTime;

        ret = E_OK;
      }
      EcuM_ExitExclusiveArea();
    }
  }

  DBG_ECUM_ASR40_GETMOSTRECENTSHUTDOWN_EXIT(ret, target, mode, cause, time);
  return ret;
}

/*------------------[EcuM_GetNextRecentShutdown]------------------------------*/

/* !LINKSTO EcuM.EB.ASR32.EcuM106,1 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_GetNextRecentShutdown
(
  P2VAR(EcuM_ASR40_StateType, AUTOMATIC, ECUM_APPL_DATA)          target,
  P2VAR(uint8, AUTOMATIC, ECUM_APPL_DATA)                         mode,
  P2VAR(EcuM_ASR40_ShutdownCauseType, AUTOMATIC, ECUM_APPL_DATA)  cause,
  /* Deviation MISRAC2012-2 */
  P2VAR(uint32, AUTOMATIC, ECUM_APPL_DATA)                        time
)
{
  Std_ReturnType ret = E_NOT_OK;

  DBG_ECUM_ASR40_GETNEXTRECENTSHUTDOWN_ENTRY(target, mode, cause, time);

#if (ECUM_MULTICORE_ENABLED == TRUE)
  if (ECUM_CORE_ID_MASTER != ECUM_GET_CORE_ID())
  {
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    ECUM_DET_REPORT_ERROR(ECUM_SID_GET_NXT_RCNT_SHUT_CAUSE, ECUM_E_WRONG_CONTEXT);
#endif
  }
  else
#endif /* ECUM_MULTICORE_ENABLED == TRUE */
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == EcuM_GetMasterCoreInstance()->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_GET_NXT_RCNT_SHUT_CAUSE, ECUM_E_UNINIT);
  }
  else
#endif
  {
    EcuM_EnterExclusiveArea();
    if (EcuM_NextShutdownInfoIdx != 0U)
    {
      /* Decrement the buffer index to get next shutdown info. */
      EcuM_NextShutdownInfoIdx--;
    }
    else
    {
      /* If the index pointer EcuM_NextShutdownInfoIdx reached the bottom
       * of the buffer, then start the static index from top. */
      EcuM_NextShutdownInfoIdx = ECUM_STORED_SHUTDOWN_OPERATIONS - 1U;
    }

    if (EcuM_NextShutdownInfoIdx != EcuM_ShutdownInfoStartIdx)
    {
      if
      (
        ((target == NULL_PTR) || (mode == NULL_PTR)) ||
        ((cause == NULL_PTR) || (time == NULL_PTR))
      )
      {
        EcuM_ExitExclusiveArea();
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
        ECUM_DET_REPORT_ERROR(
          ECUM_SID_GET_NXT_RCNT_SHUT_CAUSE, ECUM_E_NULL_POINTER);
#endif
      }
      else
      {

        /* Returns the available shutdown information */
        *target = EcuM_ShutdownInfo[EcuM_NextShutdownInfoIdx].target;
        *mode   = EcuM_ShutdownInfo[EcuM_NextShutdownInfoIdx].mode;
        *cause  = EcuM_ShutdownInfo[EcuM_NextShutdownInfoIdx].cause;
        *time   = EcuM_ShutdownInfo[EcuM_NextShutdownInfoIdx].sdTime;

        EcuM_ExitExclusiveArea();
        ret = E_OK;
      }
    }
    else
    {
    /* else: The iteration point equals the start of buffer index means there
     * is no more data available to retrieve. Call EcuM_GetMostRecentShutdown
     * once next to get the information again. */
       EcuM_ExitExclusiveArea();
     }
  }


  DBG_ECUM_ASR40_GETNEXTRECENTSHUTDOWN_EXIT(ret, target, mode, cause, time);

  return ret;
}

/*----------------------------------------------------------------------------
 * Wakeup handling
 *----------------------------------------------------------------------------*/

/*------------------[EcuM_GetPendingWakeupEvents]-----------------------------*/
/* !LINKSTO EcuM.EB.Dsn.Interaction.EcuMMultiCoreWakeupEventHandling,1 */
FUNC(EcuM_WakeupSourceType, ECUM_CODE) EcuM_GetPendingWakeupEvents(void)
{
  EcuM_WakeupSourceType ret = 0U;

  P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance = EcuM_GetCoreInstance();

  DBG_ECUM_GETPENDINGWAKEUPEVENTS_ENTRY();

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == CoreInstance->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_GET_PEND_WK_EV, ECUM_E_UNINIT);
  }
  else
#endif
  {
    /* !LINKSTO EcuM3003,1 */
    EcuM_EnterExclusiveArea();
    ret = CoreInstance->WakeupStatus.Pending;
    EcuM_ExitExclusiveArea();
  }

  DBG_ECUM_GETPENDINGWAKEUPEVENTS_EXIT(ret);

  return ret;
}

/*------------------[EcuM_ClearWakeupEvent]-----------------------------------*/

FUNC(void, ECUM_CODE) EcuM_ClearWakeupEvent
(
  EcuM_WakeupSourceType sources
)
{

  P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance = EcuM_GetCoreInstance();

  DBG_ECUM_CLEARWAKEUPEVENT_ENTRY(sources);

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == CoreInstance->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_CLEAR_WK_EV, ECUM_E_UNINIT);
  }
  else
#endif
  {
    /* !LINKSTO EcuM2683,1 */
    /* Switch the status of wakeup source to NONE: */
    EcuM_EnterExclusiveArea();

    EcuM_SwitchWakeupStatus(sources, ECUM_WKSTATUS_NONE, CoreInstance);

    /* Clear the started wakeup sources when the main function is executed. As
     * per SWS the started wakeup sources are stopped only when the validation
     * timeout get expired. If the wakeup event is cleared during the ongoing
     * validation, then this event cannot be considered as validated or
     * expired. */
    CoreInstance->WakeupStatus.Stopped |= sources;

    EcuM_ExitExclusiveArea();

    /* The cleared wakeup events are indicated to BswM in EcuM_MainFunction. */
  }

  DBG_ECUM_CLEARWAKEUPEVENT_EXIT(sources);
}

/*------------------[EcuM_GetValidatedWakeupEvents]---------------------------*/

FUNC(EcuM_WakeupSourceType, ECUM_CODE) EcuM_GetValidatedWakeupEvents(void)
{
  EcuM_WakeupSourceType ret = 0U;

  P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance = EcuM_GetCoreInstance();

  DBG_ECUM_GETVALIDATEDWAKEUPEVENTS_ENTRY();

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == CoreInstance->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_GET_VALID_WK_EV, ECUM_E_UNINIT);
  }
  else
#endif
  {
    EcuM_EnterExclusiveArea();
    ret = CoreInstance->WakeupStatus.Validated;
    EcuM_ExitExclusiveArea();
  }

  DBG_ECUM_GETVALIDATEDWAKEUPEVENTS_EXIT(ret);

  return ret;
}

/*------------------[EcuM_GetExpiredWakeupEvents]-----------------------------*/

FUNC(EcuM_WakeupSourceType, ECUM_CODE) EcuM_GetExpiredWakeupEvents(void)
{
  EcuM_WakeupSourceType ret = 0U;

  P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance = EcuM_GetCoreInstance();

  DBG_ECUM_GETEXPIREDWAKEUPEVENTS_ENTRY();

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == CoreInstance->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_GET_EXP_WK_EV, ECUM_E_UNINIT);
  }
  else
#endif
  {
    EcuM_EnterExclusiveArea();
    ret = CoreInstance->WakeupStatus.Expired;
    EcuM_ExitExclusiveArea();
  }

  DBG_ECUM_GETEXPIREDWAKEUPEVENTS_EXIT(ret);

  return ret;
}

/*----------------------------------------------------------------------------
 * Scheduled functions
 *----------------------------------------------------------------------------*/

/*------------------[EcuM_MainFunction]---------------------------------------*/

/* !LINKSTO SWS_EcuM_04011,1 */
FUNC(void, ECUM_CODE) EcuM_MainFunction(void)
{
  EcuM_WakeupSourceType wksPending = 0U;
  EcuM_WakeupSourceType wksValidated = 0U;
  EcuM_WakeupSourceType wksStarted = 0U;
  EcuM_WakeupSourceType wksStopped = 0U;
  uint16 wksValCounter = 0U;

  P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance = EcuM_GetCoreInstance();

  DBG_ECUM_MAINFUNCTION_ENTRY();

#if ((ECUM_ECUM_FIXED_SUPPORT == STD_ON) || (ECUM_DEV_ERROR_DETECT == STD_ON))
  /* The EcuM fixed SWS requires that the EcuM_MainFunction() shall return
   * without any functionality if it is called before EcuM_Init(). Thus we
   * circumvent the DET check here. */
  if (ECUM_STATE_UNINIT == CoreInstance->State)
  {
#if (ECUM_ECUM_FIXED_SUPPORT == STD_OFF)
    ECUM_DET_REPORT_ERROR(ECUM_SID_MAIN_FUNCTION, ECUM_E_UNINIT);
#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_OFF) */
  }
  else
#endif /* ((ECUM_ECUM_FIXED_SUPPORT == STD_ON) || (ECUM_DEV_ERROR_DETECT == STD_ON)) */
  if ((ECUM_STATE_STARTUP_ONE != CoreInstance->State) && (ECUM_STATE_STARTUP_TWO != CoreInstance->State))
  {
    EcuM_EnterExclusiveArea();

    /* Create a snapshot of the current wakeup validation state. */
    CoreInstance->WakeupStatus.Started &=
      ~CoreInstance->WakeupStatus.Stopped;
    CoreInstance->WakeupStatus.Stopped = 0U;
    wksPending = CoreInstance->WakeupStatus.Pending;
    wksValidated = CoreInstance->WakeupStatus.Validated;
    wksStarted = CoreInstance->WakeupStatus.Started;
    wksValCounter = CoreInstance->ValidationCounter;

    EcuM_ExitExclusiveArea();

    if ((CoreInstance->State == ECUM_STATE_WAKEUP_VALIDATION) && (wksValidated != 0))
    {
      CoreInstance->State = ECUM_STATE_RUN;
      /* !LINKSTO EcuM.BswMStateIndication,1 */
      BswM_EcuM_CurrentState(CoreInstance->State);
    }

    /* !LINKSTO EcuM.BswMWakeupIndication,1 */
    EcuM_WksIndicateStatus();

    /* Check for a validation timeout. */
    if ((wksPending != 0U) && (wksValCounter == 0U))
    {
      /* Switch the wakeup status to EXPIRED:
       * Validation timeout has occurred. Since the timeout value corresponds to
       * the longest timeout of the pending wakeup events, all wakeup events
       * must have expired. */

      EcuM_EnterExclusiveArea();

      EcuM_SwitchWakeupStatus(wksPending, ECUM_WKSTATUS_EXPIRED, CoreInstance);

      EcuM_ExitExclusiveArea();

      EcuM_StopWakeupSources(wksPending);
      wksStopped = wksPending;
      wksPending = 0U;
      /* Indicate the expired wakeup sources to the BswM. */
      EcuM_WksIndicateStatus_EXPIRED();
    }

    /* Start the pending wakeup sources that have not yet been started. */
    if ((wksPending & ~wksStarted) != 0U)
    {
      EcuM_StartWakeupSources(wksPending & ~wksStarted);

      /* All pending wakeup sources have been started. */
      wksStarted = wksPending;
    }

    if (wksPending != 0U)
    {
      /* Execute the validation routine if there any pending wakeup events. */
      EcuM_CheckValidation(wksPending);
    }

    EcuM_EnterExclusiveArea();

    /* Wakeup validation iteration complete, save current state. */
    if (CoreInstance->ValidationCounter > 0U)
    {
      CoreInstance->ValidationCounter--;
    }
    CoreInstance->WakeupStatus.Started |= wksStarted;
    CoreInstance->WakeupStatus.Started &= ~wksStopped;

    EcuM_ExitExclusiveArea();

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
    EcuM_CurrentModeSwitch();
#endif

#if (ECUM_MULTICORE_ENABLED == TRUE)
    if (ECUM_CORE_ID_MASTER == ECUM_GET_CORE_ID())
    {
#endif /* ECUM_MULTICORE_ENABLED == TRUE */

#if (ECUM_INCLUDE_DEM == STD_ON) && (ECUM_PROD_ERR_RAM_CHECK_FAILURE == TS_PROD_ERR_REP_TO_DEM)
      EcuM_ReportRamCheckResult();
#endif

#if (ECUM_MULTICORE_ENABLED == TRUE)
    }
#endif /* ECUM_MULTICORE_ENABLED == TRUE */

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
#if (ECUM_MULTICORE_ENABLED == TRUE)
    if (ECUM_CORE_ID_MASTER == ECUM_GET_CORE_ID())
    {
#endif
      EcuM_HandleRunRequests();
#if (ECUM_MULTICORE_ENABLED == TRUE)
    }
#endif
#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_ON) */
  } /* (ECUM_STATE_STARTUP_ONE != CoreInstance->State) && (ECUM_STATE_STARTUP_TWO != CoreInstance->State) */
  else
  {
    /* NOTHING TO DO */
  }

  DBG_ECUM_MAINFUNCTION_EXIT();
}

#if (ECUM_INCLUDE_DEM == STD_ON) && (ECUM_PROD_ERR_RAM_CHECK_FAILURE == TS_PROD_ERR_REP_TO_DEM)
STATIC FUNC(void, ECUM_CODE) EcuM_ReportRamCheckResult(void)
{
  if (EcuM_RamCheckStatus != ECUM_RAM_CHECK_STATUS_UNTESTED)
  {
    /* Report result of last RamCheck.  The last RamCheck is performed after
     * DEM was shutdown. */
    Dem_ReportErrorStatus
    (
      EcuM_DemErrRamChkFailed,
      (
        (EcuM_RamCheckStatus == ECUM_RAM_CHECK_STATUS_FAILED)
        ? DEM_EVENT_STATUS_FAILED
        : DEM_EVENT_STATUS_PASSED
      )
    );

    /* Reset the variable so that the status is reported to Dem only once. */
    EcuM_RamCheckStatus = ECUM_RAM_CHECK_STATUS_UNTESTED;
  }
}
#endif /* (ECUM_INCLUDE_DEM == STD_ON) && (ECUM_PROD_ERR_RAM_CHECK_FAILURE == TS_PROD_ERR_REP_TO_DEM) */

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
STATIC FUNC(void, ECUM_CODE) EcuM_CurrentModeSwitch(void)
{
  EcuM_ASR40_StateType State = EcuM_GetMasterCoreInstance()->State;
#if (ECUM_MULTICORE_ENABLED == TRUE)
  if (ECUM_CORE_ID_MASTER == ECUM_GET_CORE_ID())
  {
#endif
    if (State == ECUM_STATE_WAKEUP_VALIDATION)
    {
      ECUM_EB_CURRENT_MODE_SWITCH(ECUM_EB_MODE_WAKE_SLEEP);
    }
#if (ECUM_MULTICORE_ENABLED == TRUE)
  }
#endif /* ECUM_MULTICORE_ENABLED == TRUE */
}
#endif /* ECUM_ECUM_FIXED_SUPPORT == STD_ON */

/*------------------------------------------------------------------------------
 * Callbacks from BswM
 *----------------------------------------------------------------------------*/

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
FUNC(Std_ReturnType, ECUM_CODE) EcuM_GoRun(void)
{
  Std_ReturnType ret = E_NOT_OK;

  DBG_ECUM_GORUN_ENTRY();

#if (ECUM_MULTICORE_ENABLED == TRUE)
  if (ECUM_CORE_ID_MASTER != ECUM_GET_CORE_ID())
  {
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
    ECUM_DET_REPORT_ERROR(ECUM_SID_GO_RUN, ECUM_E_WRONG_CONTEXT);
#endif
  }
  else
#endif /* ECUM_MULTICORE_ENABLED == TRUE */
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == EcuM_GetMasterCoreInstance()->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_GO_RUN, ECUM_E_UNINIT);
  }
  else
#endif
  {
    EcuM_GetMasterCoreInstance()->State = ECUM_STATE_APP_RUN;

    EcuM_EnterExclusiveArea();
    EcuM_RunTimeout = EcuM_ConfigPtr->RunTimeout;
    EcuM_ExitExclusiveArea();

    ECUM_EB_CURRENT_MODE_SWITCH(ECUM_EB_MODE_RUN);
    EcuM_OnEnterRun();
    ret = E_OK;
  }

  DBG_ECUM_GORUN_EXIT(E_OK);

  return ret;
}
#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_ON) */

/*------------------------------------------------------------------------------
 * Callbacks from wakeup sources
 *----------------------------------------------------------------------------*/

/*------------------[EcuM_SetWakeupEvent]-------------------------------------*/

FUNC(void, ECUM_CODE) EcuM_SetWakeupEvent(EcuM_WakeupSourceType sources)
{

  P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance = EcuM_GetCoreInstance();

  DBG_ECUM_SETWAKEUPEVENT_ENTRY(sources);

  /* checks for development error detection */
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == CoreInstance->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_SET_WK_EV, ECUM_E_UNINIT);
  }
  else if ((sources & ECUM_WKSMASKALL) == 0U)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_SET_WK_EV, ECUM_E_UNKNOWN_WAKEUP_SOURCE);
  }
  else
#endif
  {
    /* LOOP VARIABLE */
    uint8_least wks;

    /* The new wakeup events that require validation. */
    EcuM_WakeupSourceType wksPendingNew = 0U;

    /* The maximal timeout of the wakeup events set. */
    uint16 maxTimeout = 0U;

    /* Only consider events that are associated to the selected sleep mode. */
    sources &= EcuM_SleepModeConfigList[CoreInstance->SleepMode].wakeupSource;

    EcuM_EnterExclusiveArea();
    /* !LINKSTO EcuM2572,1 */
    /* !description Only the new pending wakeup events will be set.  Events for
     *              wakeup sources that do not require validation, or for wakeup
     *              sources that are already pending, will be ignored. */
    wksPendingNew = sources &
      (EcuM_WakeupSourceType)~(CoreInstance->WakeupStatus.Pending | ECUM_WKSMASKNOVAL);

    /* Switch the wakeup status to PENDING */
    EcuM_SwitchWakeupStatus(wksPendingNew, ECUM_WKSTATUS_PENDING, CoreInstance);

    /* Switch the wakeup status to VALIDATED */
    EcuM_SwitchWakeupStatus((sources & ECUM_WKSMASKNOVAL), ECUM_WKSTATUS_VALIDATED, CoreInstance);

    EcuM_ExitExclusiveArea();

    /* !LINKSTO EcuM2712,1 */
    /* !description Only new pending events will prolong the validation
     *              time-out.*/
    wks = 0U;
    while (wksPendingNew != 0U)
    {
      if ((wksPendingNew & 0x01U) != 0U)
      {
        /* If the current validation timeout is smaller than the validation
         * timeout required for this wakeup source, update the current
         * timeout. */
        if (maxTimeout < ECUM_WKS_VAL_TIMEOUT(wks))
        {
          /* !LINKSTO EcuM2709,1 */
          maxTimeout = ECUM_WKS_VAL_TIMEOUT(wks);
        }
      }
      wksPendingNew >>= 1U;
      wks++;
    }

    EcuM_EnterExclusiveArea();
    if (CoreInstance->ValidationCounter < maxTimeout)
    {
      CoreInstance->ValidationCounter = maxTimeout;
    }
    EcuM_ExitExclusiveArea();

  }

  DBG_ECUM_SETWAKEUPEVENT_EXIT(sources);
}

/*------------------[EcuM_ValidateWakeupEvent]--------------------------------*/

FUNC(void, ECUM_CODE) EcuM_ValidateWakeupEvent(EcuM_WakeupSourceType sources)
{

  P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance = EcuM_GetCoreInstance();

  /* Create a local copy of the EcuM state variable first to protect against
   * global state changes during evaluation of passed wakeup events. */
  const EcuM_ASR40_StateType state = CoreInstance->State;

  DBG_ECUM_VALIDATEWAKEUPEVENT_ENTRY(sources);

  /* checks for development error detection */
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == state)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_VALIDATE_WK_EV, ECUM_E_UNINIT);
  }
  else if ((sources & ECUM_WKSMASKALL) == 0U)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_VALIDATE_WK_EV, ECUM_E_UNKNOWN_WAKEUP_SOURCE);
  }
  else
#endif
  {
    /* Remove non pending wakeup sources. */
    sources &= CoreInstance->WakeupStatus.Pending;

    /* If the EcuM enters UP phase it sets the state set to either RUN
     * or ECUM_STATE_WAKEUP_VALIDATION */
    /* !LINKSTO EcuM2791,1 */
    if ((state & (EcuM_ASR40_StateType)~ECUM_SUBSTATE_MASK) == ECUM_STATE_RUN)
    {
      /* !LINKSTO SWS_EcuM_02790,1 */
      /* SWS_EcuM_02790: EcuM_ValidateWakeupEvent shall return without effect for
       * all sources except communication channels when called while the ECU
       * Manager module is in the RUN state. */

      /* Remove wakeup sources without a ComM channel */
      sources &= ECUM_WKSMASKCOMMCH;
    }

    /* Switch the wakeup status to VALIDATED */
    EcuM_EnterExclusiveArea();

    EcuM_SwitchWakeupStatus(sources, ECUM_WKSTATUS_VALIDATED, CoreInstance);

    CoreInstance->WakeupStatus.Stopped |= sources;
    EcuM_ExitExclusiveArea();
  }

  DBG_ECUM_VALIDATEWAKEUPEVENT_EXIT(sources);
}

/*------------------[ShutdownHook]--------------------------------------------*/

#if (ECUM_PROVIDE_SHUTDOWN_HOOK == STD_ON)
void ShutdownHook(StatusType errorID)
{
  /* If enabled within the EcuM configuration, the EcuM provides a minimalistic
   * implementation of the ShutdownHook() callback function for the Os. */

  DBG_SHUTDOWNHOOK_ENTRY(errorID);

  TS_PARAM_UNUSED(errorID);
  EcuM_Shutdown();

  DBG_SHUTDOWNHOOK_EXIT(errorID);
}
#endif

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_RequestRUN(EcuM_ASR40_UserType user)
{
  Std_ReturnType retVal = E_NOT_OK;

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == EcuM_GetMasterCoreInstance()->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_REQUEST_RUN, ECUM_E_UNINIT);
    retVal = E_NOT_OK;
  }
  /* !LINKSTO SWS_EcuM_04094,1 */
#if (ECUM_MULTICORE_ENABLED == TRUE)
  else if (ECUM_CORE_ID_MASTER != ECUM_GET_CORE_ID())
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_REQUEST_RUN, ECUM_E_WRONG_CONTEXT);
    retVal = E_NOT_OK;
  }
#endif /* ECUM_MULTICORE_ENABLED == TRUE */
  else if((EcuM_RunReq[ECUM_USER_IDX(user)] & ((uint8)(1U << ECUM_USER_FLG(user)))) != 0U)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_REQUEST_RUN, ECUM_E_MULTIPLE_RUN_REQUESTS);
    retVal = E_NOT_OK;
  }
  else
#endif /* (ECUM_DEV_ERROR_DETECT == STD_ON) */
  {
    EcuM_EnterExclusiveArea();
    if(EcuM_KillAllRUNRequested == FALSE)
    {
      EcuM_RunReq[ECUM_USER_IDX(user)] |= (uint8)(1U << ECUM_USER_FLG(user));
      retVal = E_OK;
    }
    EcuM_ExitExclusiveArea();
  }

  return retVal;
}

/* !LINKSTO EcuM.EB.ASR32.EcuM107,1 */
#if ((ECUM_EB_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (ECUM_EB_DEFAULT_ASR_SERVICE_API == ECUM_EB_SERVICE_API_ASR32))
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR32_RequestRUN(EcuM_ASR32_UserType user)
{
  Std_ReturnType retVal = E_NOT_OK;

  /* EcuM_ASR40_UserType and EcuM_ASR32_UserType are referencing to the same
   * base type. Therefore no argument type mapping is done. */
  retVal = EcuM_ASR40_RequestRUN(user);

  return retVal;
}
#endif

FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_ReleaseRUN(EcuM_ASR40_UserType user)
{
  Std_ReturnType retVal = E_NOT_OK;

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == EcuM_GetMasterCoreInstance()->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_RELEASE_RUN, ECUM_E_UNINIT);
  }
  /* !LINKSTO SWS_EcuM_04094,1 */
#if (ECUM_MULTICORE_ENABLED == TRUE)
  else if (ECUM_CORE_ID_MASTER != ECUM_GET_CORE_ID())
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_RELEASE_RUN, ECUM_E_WRONG_CONTEXT);
  }
#endif /* ECUM_MULTICORE_ENABLED == TRUE */
  else if((EcuM_RunReq[ECUM_USER_IDX(user)] & ((uint8)(1U << ECUM_USER_FLG(user)))) == 0U)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_RELEASE_RUN, ECUM_E_MISMATCHED_RUN_RELEASE);
  }
  else
#endif /* (ECUM_DEV_ERROR_DETECT == STD_ON) */
  {
    EcuM_EnterExclusiveArea();
    EcuM_RunReq[ECUM_USER_IDX(user)] &= (uint8)~((uint8)(1U << ECUM_USER_FLG(user)));
    EcuM_ExitExclusiveArea();
    retVal = E_OK;
  }

  return retVal;
}

/* !LINKSTO EcuM.EB.ASR32.EcuM107,1 */
#if ((ECUM_EB_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (ECUM_EB_DEFAULT_ASR_SERVICE_API == ECUM_EB_SERVICE_API_ASR32))
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR32_ReleaseRUN(EcuM_ASR32_UserType user)
{
  Std_ReturnType retVal = E_NOT_OK;

  /* EcuM_ASR40_UserType and EcuM_ASR32_UserType are referencing to the same
   * base type. Therefore no argument type mapping is done. */
  retVal = EcuM_ASR40_ReleaseRUN(user);

  return retVal;
}
#endif

FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_RequestPOST_RUN(EcuM_ASR40_UserType user)
{
  Std_ReturnType retVal = E_NOT_OK;

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == EcuM_GetMasterCoreInstance()->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_REQUEST_POST_RUN, ECUM_E_UNINIT);
    retVal = E_NOT_OK;
  }
  /* !LINKSTO SWS_EcuM_04094,1 */
#if (ECUM_MULTICORE_ENABLED == TRUE)
  else if (ECUM_CORE_ID_MASTER != ECUM_GET_CORE_ID())
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_REQUEST_POST_RUN, ECUM_E_WRONG_CONTEXT);
    retVal = E_NOT_OK;
  }
#endif /* ECUM_MULTICORE_ENABLED == TRUE */
  else if((EcuM_PostRunReq[ECUM_USER_IDX(user)] & ((uint8)(1U << ECUM_USER_FLG(user)))) != 0U)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_REQUEST_POST_RUN, ECUM_E_MULTIPLE_RUN_REQUESTS);
    retVal = E_NOT_OK;
  }
  else
#endif /* (ECUM_DEV_ERROR_DETECT == STD_ON) */
  {
    EcuM_EnterExclusiveArea();
    if(EcuM_KillAllRUNRequested == FALSE)
    {
      EcuM_PostRunReq[ECUM_USER_IDX(user)] |= (uint8)(1U << ECUM_USER_FLG(user));
      retVal = E_OK;
    }
    EcuM_ExitExclusiveArea();
  }

  return retVal;
}

/* !LINKSTO EcuM.EB.ASR32.EcuM107,1 */
#if ((ECUM_EB_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (ECUM_EB_DEFAULT_ASR_SERVICE_API == ECUM_EB_SERVICE_API_ASR32))
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR32_RequestPOST_RUN(EcuM_ASR32_UserType user)
{
  Std_ReturnType retVal = E_NOT_OK;

  /* EcuM_ASR40_UserType and EcuM_ASR32_UserType are referencing to the same
   * base type. Therefore no argument type mapping is done. */
  retVal = EcuM_ASR40_RequestPOST_RUN(user);

  return retVal;
}
#endif

FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR40_ReleasePOST_RUN(EcuM_ASR40_UserType user)
{
  Std_ReturnType retVal = E_NOT_OK;

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == EcuM_GetMasterCoreInstance()->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_RELEASE_POST_RUN, ECUM_E_UNINIT);
  }
  /* !LINKSTO SWS_EcuM_04094,1 */
#if (ECUM_MULTICORE_ENABLED == TRUE)
  else if (ECUM_CORE_ID_MASTER != ECUM_GET_CORE_ID())
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_RELEASE_POST_RUN, ECUM_E_WRONG_CONTEXT);
  }
#endif /* ECUM_MULTICORE_ENABLED == TRUE */
  else if((EcuM_PostRunReq[ECUM_USER_IDX(user)] & ((uint8)(1U << ECUM_USER_FLG(user)))) == 0U)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_RELEASE_POST_RUN, ECUM_E_MISMATCHED_RUN_RELEASE);
  }
  else
#endif /* (ECUM_DEV_ERROR_DETECT == STD_ON) */
  {
    EcuM_EnterExclusiveArea();
    EcuM_PostRunReq[ECUM_USER_IDX(user)] &= (uint8)~((uint8)(1U << ECUM_USER_FLG(user)));
    EcuM_ExitExclusiveArea();
    retVal = E_OK;
  }

  return retVal;
}

/* !LINKSTO EcuM.EB.ASR32.EcuM107,1 */
#if ((ECUM_EB_ENABLE_ASR32_SERVICE_API == STD_ON) || \
    (ECUM_EB_DEFAULT_ASR_SERVICE_API == ECUM_EB_SERVICE_API_ASR32))
FUNC(Std_ReturnType, ECUM_CODE) EcuM_ASR32_ReleasePOST_RUN(EcuM_ASR32_UserType user)
{
  Std_ReturnType retVal = E_NOT_OK;

  /* EcuM_ASR40_UserType and EcuM_ASR32_UserType are referencing to the same
   * base type. Therefore no argument type mapping is done. */
  retVal = EcuM_ASR40_ReleasePOST_RUN(user);

  return retVal;
}
#endif

FUNC(void, ECUM_CODE) EcuM_KillAllRUNRequests(void)
{
  uint8 i;

  DBG_ECUM_KILLALLRUNREQUESTS_ENTRY();

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == EcuM_GetMasterCoreInstance()->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_KILL_ALL_RUN_REQUESTS, ECUM_E_UNINIT);
  }
#if (ECUM_MULTICORE_ENABLED == TRUE)
  else if (ECUM_CORE_ID_MASTER != ECUM_GET_CORE_ID())
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_KILL_ALL_RUN_REQUESTS, ECUM_E_WRONG_CONTEXT);
  }
#endif /* (ECUM_MULTICORE_ENABLED == TRUE) */
  else
#endif /* (ECUM_DEV_ERROR_DETECT == STD_ON) */
  {
#if ((ECUM_INCLUDE_DEM == STD_ON) \
 && (ECUM_PROD_ERR_ALL_RUN_REQUESTS_KILLED == TS_PROD_ERR_REP_TO_DEM))
    Dem_ReportErrorStatus
    (
      EcuM_DemErrAllRUNRequestsKilled,
      ECUM_E_ALL_RUN_REQUESTS_KILLED
    );
#endif /* (ECUM_INCLUDE_DEM == STD_ON) */

    EcuM_EnterExclusiveArea();
    EcuM_KillAllRUNRequested = TRUE;
    for(i = 0U; i < ECUM_FIXED_USER_BITMAP_SIZE; i++)
    {
      EcuM_RunReq[i] = 0U;
      EcuM_PostRunReq[i] = 0U;
    }
    EcuM_ExitExclusiveArea();
  }

  DBG_ECUM_KILLALLRUNREQUESTS_EXIT();
}

FUNC(EcuM_WakeupStatusType, ECUM_CODE) EcuM_GetStatusOfWakeupSource(EcuM_WakeupSourceType sources)
{
  EcuM_WakeupStatusType status = ECUM_WKSTATUS_DISABLED;
  EcuM_WakeupContextType wkStatus = EcuM_GetMasterCoreInstance()->WakeupStatus;
#if (ECUM_MULTICORE_ENABLED == TRUE)
  const EcuMCoreIdType coreID = ECUM_GET_CORE_ID();
#endif

  DBG_ECUM_GETSTATUSOFWAKEUPSOURCE_ENTRY(sources);

#if (ECUM_DEV_ERROR_DETECT == STD_ON)
  if (ECUM_STATE_UNINIT == EcuM_GetMasterCoreInstance()->State)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_GET_STAT_OF_WK_SRC, ECUM_E_UNINIT);
  }
#if (ECUM_MULTICORE_ENABLED == TRUE)
  else if (ECUM_CORE_ID_MASTER != coreID)
  {
    ECUM_DET_REPORT_ERROR(ECUM_SID_GET_STAT_OF_WK_SRC, ECUM_E_WRONG_CONTEXT);
  }
#endif /* ECUM_MULTICORE_ENABLED == TRUE */
  else
#endif /* ECUM_DEV_ERROR_DETECT == STD_ON */
  {
    /* RANGE CHECK */
    if (sources == ECUM_WKSOURCE_ALL_SOURCES)
    {
      sources = ECUM_WKSMASKALL;
    }
    else if((sources & ((EcuM_WakeupSourceType)~ECUM_WKSMASKALL)) != 0U)
    {
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
      ECUM_DET_REPORT_ERROR(ECUM_SID_GET_STAT_OF_WK_SRC, ECUM_E_UNKNOWN_WAKEUP_SOURCE);
#endif
      sources &= ECUM_WKSMASKALL;
    }
    else
    {
      /* All wakeup sources are in range, nothing to do. */
    }

    /* The wakeup status of the Master Core is validated */
    if((sources & wkStatus.Validated) != 0U)
    {
      status = ECUM_WKSTATUS_VALIDATED;
    }
    else if((sources & wkStatus.Pending) != 0U)
    {
      status = ECUM_WKSTATUS_PENDING;
    }
    else if((sources & wkStatus.Expired) != 0U)
    {
      status = ECUM_WKSTATUS_EXPIRED;
    }
    else
    {
      status = ECUM_WKSTATUS_NONE;
    }
  }

  DBG_ECUM_GETSTATUSOFWAKEUPSOURCE_EXIT(WakeupStatus,sources);
  return status;
}

#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_ON) */
/*==================[internal function definitions]===========================*/

FUNC(Std_ReturnType, ECUM_CODE) EcuM_IsValidConfig
(
  P2CONST(void, AUTOMATIC, ECUM_APPL_CONST) voidConfigPtr
)
{
  /* Assume an invalid configuration */
  Std_ReturnType result = E_NOT_OK;
  P2CONST(EcuM_ConfigType, AUTOMATIC, ECUM_APPL_CONST) ConfigPtr = voidConfigPtr;

  /* Check if the configuration fits to the platform */
  if(TS_PlatformSigIsValid(ConfigPtr->PlatformSignature) == TRUE )
  {
    /* Validate the post build configuration against the compile time configuration */
    if(ECUM_CFG_SIGNATURE == ConfigPtr->CfgSignature)
    {
      /* Validate the post build configuration against the link time configuration */
      if(EcuM_LcfgSignature == ConfigPtr->LcfgSignature)
      {
        /* Validate the Published information in post build configuration against the
         * Published information in compile time configuration
         */
        if(ECUM_PUBLIC_INFO_SIGNATURE == ConfigPtr->PublicInfoSignature)
        {
          /* Indicate that the configuration is valid */
          result = E_OK;
        }
      }
    }
  }

  return result;
}

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
/*------------------[EcuM_HandleRunRequests]----------------------------------*/

STATIC FUNC(void, ECUM_CODE) EcuM_HandleRunRequests(void)
{
  switch(EcuM_GetMasterCoreInstance()->State)
  {
    case ECUM_STATE_APP_RUN:
    {
      if (EcuM_RunTimeout > 0U)
      {
        EcuM_EnterExclusiveArea();
        EcuM_RunTimeout--;
        EcuM_ExitExclusiveArea();
      }
      /* Timeout has expired and there are no RUN requests, thus call the
       * callout, disable the ComM channels and switch to APP_POST_RUN state. */
      else if (EcuM_IsRunReq() == FALSE)
      {
        EcuM_OnExitRun();
        EcuM_GetMasterCoreInstance()->State = ECUM_STATE_APP_POST_RUN;
        ECUM_EB_CURRENT_MODE_SWITCH(ECUM_EB_MODE_POST_RUN);
      }
      else
      {
        /* DO NOTHING */
      }
      break;
    }
    case ECUM_STATE_APP_POST_RUN:
    {
      /* If a RUN request is made, switch back to APP_RUN state immediately. */
      if(EcuM_IsRunReq() == TRUE)
      {
        /* Currently always returns E_OK */
        (void)EcuM_GoRun();
      }
      /* If there are no POST_RUN requests, call the callout and
       * go to shutdown state. BswM does the rest then. */
      else if (EcuM_IsPostRunReq() == FALSE)
      {
        EcuM_GetMasterCoreInstance()->State = ECUM_STATE_PREP_SHUTDOWN;
        if (ECUM_STATE_SLEEP == EcuM_ShutdownTarget)
        {
          ECUM_EB_CURRENT_MODE_SWITCH(ECUM_EB_MODE_SLEEP);
        }
        else
        {
          /* ECUM_STATE_RESET or ECUM_STATE_OFF */
          ECUM_EB_CURRENT_MODE_SWITCH(ECUM_EB_MODE_SHUTDOWN);
        }
        EcuM_OnExitPostRun();
      }
      else
      {
        /* DO NOTHING */
      }
      break;
    }
    default:
    {
      /* DO NOTHING */
      break;
    }
  }

}
#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_ON) */

/*------------------[EcuM_WksIndicateStatus]---------------------------------*/
STATIC FUNC(void, ECUM_CODE) EcuM_WksIndicateStatus(void)
{
  EcuM_WksIndicateStatus_NONE();
  EcuM_WksIndicateStatus_PENDING();
  EcuM_WksIndicateStatus_VALIDATED();
  EcuM_WksIndicateStatus_DISABLED();
}

/*------------------[EcuM_GoSleep]--------------------------------------------*/

STATIC FUNC(void, ECUM_CODE) EcuM_GoSleep(void)
{

  P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance = EcuM_GetCoreInstance();

  EcuM_WakeupSourceType wksSleepMode = EcuM_SleepModeConfigList[CoreInstance->SleepMode].wakeupSource;
  EcuM_WakeupSourceType wksCleared;

  CoreInstance->State = ECUM_STATE_GO_SLEEP;
  /* !LINKSTO EcuM.BswMStateIndication,1 */
  BswM_EcuM_CurrentState(CoreInstance->State);

#if (ECUM_MULTICORE_ENABLED == TRUE)
  /* !LINKSTO EcuM.MasterCore.GoSleep.Sequence,1 */
  if (ECUM_CORE_ID_MASTER == ECUM_GET_CORE_ID())
  {
#endif /* (ECUM_MULTICORE_ENABLED == TRUE) */

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
  EcuM_KillAllRUNRequested = FALSE;
  EcuM_OnGoSleep();
#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_ON) */

  EcuM_EnterExclusiveArea();

  /* PENDING and VALIDATED wakeup events shall not be cleared. */
  wksCleared = wksSleepMode & (EcuM_WakeupSourceType)~(CoreInstance->WakeupStatus.Pending | CoreInstance->WakeupStatus.Validated);
  /* Switch the status of wakeup source to NONE: */
  EcuM_SwitchWakeupStatus(wksCleared, ECUM_WKSTATUS_NONE, CoreInstance);

  EcuM_ExitExclusiveArea();

  /* Indicate to BswM the state of the cleared wakeup sources. */
  EcuM_WksIndicateStatus_NONE();

  /* !LINKSTO EcuM2546,1 */
  EcuM_EnableWakeupSources(wksSleepMode);

  EcuM_EnterExclusiveArea();

 /* Switch the status of wakeup source to ENABLED:
  * No notification will be sent to the BswM since ENABLED is not a
  * specified state. This is for internal use only. */
  EcuM_SwitchWakeupStatus(wksSleepMode, ECUM_WKSTATUS_ENABLED, CoreInstance);

  EcuM_ExitExclusiveArea();

  /* Prevent scheduling: The first Task which is allowed to be run after wakeup
   * is the task containing the EcuM_MainFunction(). */
  /* !LINKSTO SWS_EcuM_03010,1 */
  ECUM_GET_RESOURCE_SCHEDULER();

  /* store last shutdown target and sleep mode */
  EcuM_LastShutdownTarget = EcuM_ShutdownTarget;
  EcuM_LastSleepMode      = CoreInstance->SleepMode;
  EcuM_LastResetMode      = EcuM_ResetMode;

#if (ECUM_MULTICORE_ENABLED == TRUE)
  }  /* end if (ECUM_CORE_ID_MASTER == ECUM_GET_CORE_ID()) */
  /* !LINKSTO EcuM.SlaveCore.GoSleep.Sequence,1 */
  else
  {
    EcuM_EnterExclusiveArea();

    /* PENDING and VALIDATED wakeup events shall not be cleared. */
    wksCleared = wksSleepMode & (EcuM_WakeupSourceType)~(CoreInstance->WakeupStatus.Pending | CoreInstance->WakeupStatus.Validated);
    /* Switch the status of wakeup source to NONE: */
    EcuM_SwitchWakeupStatus(wksCleared, ECUM_WKSTATUS_NONE, CoreInstance);

    EcuM_ExitExclusiveArea();

    /* Indicate to BswM the state of the cleared wakeup sources. */
    EcuM_WksIndicateStatus_NONE();

    /* !LINKSTO EcuM2546,1 */
    EcuM_EnableWakeupSources(wksSleepMode);

    EcuM_EnterExclusiveArea();

   /* Switch the status of wakeup source to ENABLED:
    * No notification will be sent to the BswM since ENABLED is not a
    * specified state. This is for internal use only. */
    EcuM_SwitchWakeupStatus(wksSleepMode, ECUM_WKSTATUS_ENABLED, CoreInstance);

    EcuM_ExitExclusiveArea();

    ECUM_GET_RESOURCE_SCHEDULER();
  }
#endif /* (ECUM_MULTICORE_ENABLED == TRUE) */

  CoreInstance->State = ECUM_STATE_SLEEP;
  /* !LINKSTO EcuM.BswMStateIndication,1 */
  BswM_EcuM_CurrentState(CoreInstance->State);

}

/*------------------[EcuM_WakeupRestart---------------------------------------*/
/* !LINKSTO SWS_EcuM_04030,1 */
STATIC FUNC(void, ECUM_CODE) EcuM_WakeupRestart(void)
{

  P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance = EcuM_GetCoreInstance();

  /* WAKEUP RESTART SEQUENCE */

  /* Restore the MCU normal mode. */
  Mcu_SetMode(ECUM_NORMALMCUMODE);

  /* Disable the wakeup sources that were set. */
  if
  (
    (CoreInstance->WakeupStatus.Pending != 0U) ||
    (CoreInstance->WakeupStatus.Validated != 0U)
  )
  {
    /* Invoke callout for disabling current pending or validated wakeup sources.
     * Disable currently pending wakeup source but leave the others armed so
     * that later wakeups are possible. */
    EcuM_WakeupSourceType wksDisable =
      CoreInstance->WakeupStatus.Pending |
      CoreInstance->WakeupStatus.Validated;

    EcuM_DisableWakeupSources(wksDisable);

    /* Switch wakeup status to DISABLED */
    EcuM_EnterExclusiveArea();

    EcuM_SwitchWakeupStatus(wksDisable, ECUM_WKSTATUS_DISABLED, CoreInstance);

    EcuM_ExitExclusiveArea();
   /* The disabled wakeup events are indicated to BswM in the
    * EcuM_MainFunction. */
  }

#if (ECUM_MULTICORE_ENABLED == TRUE)
 /* !LINKSTO SWS_EcuM_04026,1 */
 if (ECUM_CORE_ID_MASTER == ECUM_GET_CORE_ID())
  {
#endif /* (ECUM_MULTICORE_ENABLED == TRUE) */

#if (ECUM_INITLIST_RESTART == STD_ON)
  /* Restart driver of restart list: Initialize drivers that need restarting. */
  EcuM_AL_DriverRestart(EcuM_ConfigPtr);
#endif /* ECUM_INITLIST_RESTART == STD_ON */

#if (ECUM_MULTICORE_ENABLED == TRUE)
  }
#endif /* (ECUM_MULTICORE_ENABLED == TRUE) */

  /* ENTER WAKEUP VALIDATION */

  /* The validation timer was wound up when EcuM_SetWakeupEvent() was called. */
  CoreInstance->State = ECUM_STATE_WAKEUP_VALIDATION;
  /* !LINKSTO EcuM.BswMStateIndication,1 */
  BswM_EcuM_CurrentState(CoreInstance->State);

  /* ALLOW SCHEDULING */
  ECUM_RELEASE_RESOURCE_SCHEDULER();

}

/*------------------[EcuM Wakeup Source State Management]---------------------*/

STATIC FUNC(void, ECUM_CODE) EcuM_SwitchWakeupStatus
(
  EcuM_WakeupSourceType sources,
  EcuM_WakeupStatusType status,
  P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance
)
{

  switch(status)
  {
    case ECUM_WKSTATUS_ENABLED:
    {
      /* No notification will be sent to the BswM since ENABLED is not a
       * specified state. This is for internal use only. See the function
       * prototype comment for more information on the state ENABLED. */
      CoreInstance->WakeupStatus.Disabled &= (EcuM_WakeupSourceType)~sources;
      break;
    }
    case ECUM_WKSTATUS_DISABLED:
    {
      CoreInstance->WakeupIndication.Disabled |= sources;
      CoreInstance->WakeupStatus.Disabled |= sources;
      break;
    }
    case ECUM_WKSTATUS_NONE:
    {
      CoreInstance->WakeupIndication.None |= sources;
      CoreInstance->WakeupStatus.None |= sources;

      CoreInstance->WakeupStatus.Expired &= (EcuM_WakeupSourceType)~sources;
      CoreInstance->WakeupStatus.Pending &= (EcuM_WakeupSourceType)~sources;
      CoreInstance->WakeupStatus.Validated &= (EcuM_WakeupSourceType)~sources;
      break;
    }
    case ECUM_WKSTATUS_PENDING:
    {
      CoreInstance->WakeupIndication.Pending |= sources;
      CoreInstance->WakeupStatus.Pending |= sources;

      CoreInstance->WakeupStatus.Expired &= (EcuM_WakeupSourceType)~sources;
      CoreInstance->WakeupStatus.None &= (EcuM_WakeupSourceType)~sources;
      CoreInstance->WakeupStatus.Validated &= (EcuM_WakeupSourceType)~sources;
      break;
    }
    case ECUM_WKSTATUS_VALIDATED:
    {
      CoreInstance->WakeupIndication.Validated |= sources;
      CoreInstance->WakeupStatus.Validated |= sources;

      CoreInstance->WakeupStatus.Expired &= (EcuM_WakeupSourceType)~sources;
      CoreInstance->WakeupStatus.None &= (EcuM_WakeupSourceType)~sources;
      CoreInstance->WakeupStatus.Pending &= (EcuM_WakeupSourceType)~sources;
      break;
    }
    case ECUM_WKSTATUS_EXPIRED:
    {
      CoreInstance->WakeupIndication.Expired |= sources;
      CoreInstance->WakeupStatus.Expired |= sources;

      CoreInstance->WakeupStatus.None &= (EcuM_WakeupSourceType)~sources;
      CoreInstance->WakeupStatus.Pending &= (EcuM_WakeupSourceType)~sources;
      CoreInstance->WakeupStatus.Validated &= (EcuM_WakeupSourceType)~sources;
      break;
    }
    /* CHECK: NOPARSE */
    default:
    {
      ECUM_UNREACHABLE_CODE_ASSERT(ECUM_GET_CORE_ID(), ECUM_INTERNAL_API_ID);
      /* Should never be entered. */
      break;
    }
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, ECUM_CODE) EcuM_WksIndicateStatus_DISABLED(void)
{
  EcuM_WakeupSourceType wksDisabled;

  P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance = EcuM_GetCoreInstance();

  EcuM_EnterExclusiveArea();
  wksDisabled = CoreInstance->WakeupIndication.Disabled;
  CoreInstance->WakeupIndication.Disabled = 0U;
  EcuM_ExitExclusiveArea();

  if (wksDisabled != 0U)
  {
    BswM_EcuM_CurrentWakeup(wksDisabled, ECUM_WKSTATUS_DISABLED);
  }
}

STATIC FUNC(void, ECUM_CODE) EcuM_WksIndicateStatus_PENDING(void)
{
  EcuM_WakeupSourceType wksPending;

  P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance = EcuM_GetCoreInstance();

  EcuM_EnterExclusiveArea();
  wksPending = CoreInstance->WakeupIndication.Pending;
  CoreInstance->WakeupIndication.Pending = 0U;
  EcuM_ExitExclusiveArea();

  if (wksPending != 0U)
  {
    BswM_EcuM_CurrentWakeup(wksPending, ECUM_WKSTATUS_PENDING);
  }
}

STATIC FUNC(void, ECUM_CODE) EcuM_WksIndicateStatus_VALIDATED(void)
{
  EcuM_WakeupSourceType wksValidated;

  P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance = EcuM_GetCoreInstance();

  EcuM_EnterExclusiveArea();
  wksValidated = CoreInstance->WakeupIndication.Validated;
  CoreInstance->WakeupIndication.Validated = 0U;
  EcuM_ExitExclusiveArea();

  if (wksValidated != 0U)
  {
    BswM_EcuM_CurrentWakeup(wksValidated, ECUM_WKSTATUS_VALIDATED);
#if (ECUM_WKS_COMMCH_ENABLED == STD_ON)
    EcuM_ComMWakeupIndication(wksValidated & ECUM_WKSMASKCOMMCH);
#endif
#if (ECUM_WKS_COMMPNC_ENABLED == STD_ON)
    EcuM_ComMPNCWakeupIndication(wksValidated & ECUM_WKSMASKCOMMPNC);
#endif
  }
}

STATIC FUNC(void, ECUM_CODE) EcuM_WksIndicateStatus_EXPIRED(void)
{
  EcuM_WakeupSourceType wksExpired;

  P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance = EcuM_GetCoreInstance();

  EcuM_EnterExclusiveArea();
  wksExpired = CoreInstance->WakeupIndication.Expired;
  CoreInstance->WakeupIndication.Expired = 0U;
  EcuM_ExitExclusiveArea();

  /* EcuM_WksIndicateStatus_EXPIRED() is called only if one or more wakeup
   * sources have just expired.  Therefore, the guard check is not needed. */
  BswM_EcuM_CurrentWakeup(wksExpired, ECUM_WKSTATUS_EXPIRED);

}

STATIC FUNC(void, ECUM_CODE) EcuM_WksIndicateStatus_NONE(void)
{
  EcuM_WakeupSourceType wksNone;

  P2VAR(EcuM_CoreContextType, ECUM_VAR, ECUM_APPL_DATA) CoreInstance = EcuM_GetCoreInstance();

  EcuM_EnterExclusiveArea();
  wksNone = CoreInstance->WakeupIndication.None;
  CoreInstance->WakeupIndication.None = 0U;
  EcuM_ExitExclusiveArea();

  if (wksNone != 0U)
  {
    BswM_EcuM_CurrentWakeup(wksNone, ECUM_WKSTATUS_NONE);
  }

}

/*------------------[EcuM_ComMWakeupIndication]-------------------------------*/

#if (ECUM_WKS_COMMCH_ENABLED == STD_ON)
STATIC FUNC(void, ECUM_CODE) EcuM_ComMWakeupIndication
(
  EcuM_WakeupSourceType ComMevents
)
{
  uint8_least         wks = 0U;
  ComM_InitStatusType Status;
  Std_ReturnType      RetVal = E_NOT_OK;
#if ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE))
  P2VAR(uint8, ECUM_VAR, ECUM_APPL_DATA) ComMChannelCall = EcuM_GetCoreInstance()->ComMChannelCall;
  uint8 foundindex = 0U;
  uint8 count = 0U;
#endif /* ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE)) */

#if ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE))
  TS_MemSet(ComMChannelCall, 0U, ECUM_COMM_NUM_CHANNELS_BYTES);
#endif /* ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE)) */
  /* Requirement: EcuM.ComMWakeupIndication
   *
   * Get the initialization status of Communication Manager and indicate wakeup
   * notification to ComM only if ComM had successfully initialized and is
   * usable. */
   /* !LINKSTO EcuM.ComMWakeupIndication,1 */
  RetVal = ComM_GetStatus(&Status);

  if ((E_OK == RetVal) && (COMM_INIT == Status))
  {
    while (ComMevents != 0U)
    {
      if ((ComMevents & 1U) != 0U)
      {
#if ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE))
        /* CHECK: NOPARSE */
        /* Since ECUM_WKSID_TO_CFG(wks).ComMCh is always present in
         * EcuM_ComM_Channel_Translation, expression
         * count < ECUM_COMM_CHANNELS_NUMBER never evaluates to false.
         */
        for (count = 0U; count < ECUM_COMM_CHANNELS_NUMBER; count++)
        {
          if(EcuM_ComM_Channel_Translation[count] == ECUM_WKSID_TO_CFG(wks).ComMCh)
          {
            foundindex = count;
            break;
          }
        }
        /* CHECK: PARSE */
        if ((ComMChannelCall[foundindex / 8U] & (1U << (foundindex % 8U))) == 0U)
        {
          /* !LINKSTO EcuM.EB.SchM_Call_ComM_EcuM_WakeUpIndication,1 */
          if (EcuM_ComM_Channel_WakeupList[foundindex].FctPtrs[ECUM_GET_CORE_ID()](ECUM_WKSID_TO_CFG(wks).ComMCh) != SCHM_E_OK)
          {
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
            /* !LINKSTO EcuM.EB.SchM_Call_Client_Server_Channel_DET,1 */
            ECUM_DET_REPORT_ERROR(ECUM_SID_MAIN_FUNCTION, ECUM_E_COMMWAKEUPINDICATION);
#endif /* #if (ECUM_DEV_ERROR_DETECT == STD_ON) */
          }
          ComMChannelCall[foundindex / 8U] |= (1U << (foundindex % 8U));
        }
#else /* ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE)) */
        /* Inform ComM about the event. */
        ComM_EcuM_WakeUpIndication(ECUM_WKSID_TO_CFG(wks).ComMCh);
#endif /* ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE)) */
      }
      ComMevents >>= 1U;
      ++wks;
    }
  }
}
#endif

/*------------------[EcuM_ComMPNCWakeupIndication]-------------------------------*/

#if (ECUM_WKS_COMMPNC_ENABLED == STD_ON)
STATIC FUNC(void, ECUM_CODE) EcuM_ComMPNCWakeupIndication
(
  EcuM_WakeupSourceType ComMPNCevents
)
{
  uint8_least         wks = 0U;
  ComM_InitStatusType Status;
  Std_ReturnType      RetVal = E_NOT_OK;
#if ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE))
  P2VAR(uint8, ECUM_VAR, ECUM_APPL_DATA) ComMPNCCall = EcuM_GetCoreInstance()->ComMPNCCall;
  uint8 foundindex = 0U;
  uint8 count = 0U;
#endif /* ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE)) */

#if ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE))
  TS_MemSet(ComMPNCCall, 0U, ECUM_COMM_NUM_PNCS_BYTES);
#endif /* ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE)) */
  /* Requirement: EcuM.ComMPNCWakeupIndication
   *
   * Get the initialization status of Communication Manager and indicate wakeup
   * notification to ComM only if ComM had successfully initialized and is
   * usable. */
   /* !LINKSTO EcuM.ComMPNCWakeupIndication,1 */
  RetVal = ComM_GetStatus(&Status);

  if ((E_OK == RetVal) && (COMM_INIT == Status))
  {
    while (ComMPNCevents != 0U)
    {
      if ((ComMPNCevents & 1U) != 0U)
      {
#if ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE))
        /* CHECK: NOPARSE */
        /* Since ECUM_WKSID_TO_CFG(wks).ComMPNC is always present in
         * EcuM_ComM_Pnc_Translation, expression
         * count < ECUM_COMM_PNC_NUMBER never evaluates to false.
         */
        for (count = 0U; count < ECUM_COMM_PNC_NUMBER; count++)
        {
          if(EcuM_ComM_Pnc_Translation[count] == ECUM_WKSID_TO_CFG(wks).ComMPNC)
          {
            foundindex = count;
            break;
          }
        }
        /* CHECK: PARSE */
        if ((ComMPNCCall[foundindex / 8U] & (1U << (foundindex % 8U))) == 0U)
        {
          /* !LINKSTO EcuM.EB.SchM_Call_ComM_EcuM_PNCWakeUpIndication,1 */
          if (EcuM_ComM_PNC_WakeupList[foundindex].FctPtrs[ECUM_GET_CORE_ID()](ECUM_WKSID_TO_CFG(wks).ComMPNC) != SCHM_E_OK)
          {
#if (ECUM_DEV_ERROR_DETECT == STD_ON)
            /* !LINKSTO EcuM.EB.SchM_Call_Client_Server_PNC_DET,1 */
            ECUM_DET_REPORT_ERROR(ECUM_SID_MAIN_FUNCTION, ECUM_E_COMMPNCWAKEUPINDICATION);
#endif
          }
          ComMPNCCall[foundindex / 8U] |= (1U << (foundindex % 8U));
        }
#else /* #if ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE)) */
        /* Inform ComM about the event. */
        ComM_EcuM_PNCWakeUpIndication(ECUM_WKSID_TO_CFG(wks).ComMPNC);
#endif /* #if ((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_CROSS_CORE_COMM_WAKEUP == TRUE)) */
      }
      ComMPNCevents >>= 1U;
      ++wks;
    }
  }
}
#endif

#if (ECUM_ECUM_FIXED_SUPPORT == STD_ON)
STATIC FUNC(boolean, ECUM_CODE) EcuM_IsRunReq(void)
{
  uint8 i;
  boolean retVal = FALSE;

  for(i = 0U; i < ECUM_FIXED_USER_BITMAP_SIZE; i++)
  {
    if(EcuM_RunReq[i] != 0U)
    {
       retVal = TRUE;
    }
  }

  return retVal;
}

STATIC FUNC(boolean, ECUM_CODE) EcuM_IsPostRunReq(void)
{
  uint8 i;
  boolean retVal = FALSE;

  for(i = 0U; i < ECUM_FIXED_USER_BITMAP_SIZE; i++)
  {
    if(EcuM_PostRunReq[i] != 0U)
    {
       retVal = TRUE;
    }
  }

  return retVal;
}
#endif /* (ECUM_ECUM_FIXED_SUPPORT == STD_ON) */

#define ECUM_STOP_SEC_CODE
#include <EcuM_MemMap.h>

/*------------------[EcuM Multi-Core internal function definitions]---------------------*/

#if (ECUM_MULTICORE_ENABLED == TRUE)

#define ECUM_START_SEC_CODE
#include <EcuM_MemMap.h>

/* !LINKSTO EcuM.Impl.EcuM_SlaveCoreSync,1 */
/* !LINKSTO EcuM.Impl.Source.MasterSlaveCoreSync,1 */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_SlaveCoreSync(void)
{
  Std_ReturnType EcuM_Status;
  /** \brief a bit field to store synchronized slave cores. */
  uint32 EcuM_SlaveSync = 0U;
  EcuM_SyncActionType syncResult = ECUM_CONTINUE_SYNC;

  do
  {
    uint8 EcuM_SlaveCoreId;
    EcuM_Status = SchM_Receive_EcuM_EcuM_SlaveCoreSyncPort(&EcuM_SlaveCoreId);
    if (SCHM_E_OK == EcuM_Status)
    {
      EcuM_SlaveSync |= ((uint32)1U << EcuM_SlaveCoreId);
    }
    if ((uint32)ECUM_SLAVE_SYNC_MASK == EcuM_SlaveSync)
    {
      break;
    }
    /* !LINKSTO EcuM.Impl.EcuM_OnCoreSync,1 */
    syncResult = EcuM_OnCoreSync();
  } while (syncResult != ECUM_ABORT_SYNC);

  return (ECUM_CONTINUE_SYNC == syncResult) ? E_OK : E_NOT_OK;
}

#define ECUM_STOP_SEC_CODE
#include <EcuM_MemMap.h>

#endif /* ECUM_MULTICORE_ENABLED == TRUE */

/*==================[end of file]=============================================*/

