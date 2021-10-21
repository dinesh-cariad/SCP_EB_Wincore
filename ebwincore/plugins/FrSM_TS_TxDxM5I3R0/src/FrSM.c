/**
 * \file
 *
 * \brief AUTOSAR FrSM
 *
 * This file contains the implementation of the AUTOSAR
 * module FrSM.
 *
 * \version 5.3.18
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
 *  All declarations of an object or function shall use the same names
 *  and type qualifiers.
 *
 *  Reason:
 *  Function prototype of generated file .arxml does not use the same
 *  typedef for the datatypes.
 *
 */

/*======================[Include Section]=====================================*/

#include <FrIf.h>                           /* !LINKSTO FrSm.ASR40.FrSm058, 1 */
#include <SchM_FrSM.h>
#include <FrSM.h>                           /* !LINKSTO FrSm.ASR40.FrSm055, 1 */
#include <TSAtomic_Assign.h>                /* Atomic assignment macros */

#if (FRSM_FRNM_STARTUPERROR_ENABLE == STD_ON)
#include <FrNm.h>
#endif /* FRSM_FRNM_STARTUPERROR_ENABLE == STD_ON */

#if (FRSM_COMM_INDICATION_ENABLE == STD_ON)
#include <ComM_BusSm.h>
#endif /* FRSM_COMM_INDICATION_ENABLE */

#if (   (FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP == TS_PROD_ERR_REP_TO_DEM) \
     || (FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS == TS_PROD_ERR_REP_TO_DEM) )
#include <Dem.h>                            /* !LINKSTO FrSm.ASR40.FrSm059, 1 */
#endif /* FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP ... */

#if (FRSM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h> /* get development error tracer interface */
#endif /* FRSM_DEV_ERROR_DETECT == STD_ON */

#if (FRSM_REPORT_TO_BSMW_ENABLE == STD_ON)
#include <BswM_FrSM.h>
#endif /* (FRSM_REPORT_TO_BSMW_ENABLE == STD_ON) */

#if (FRSM_SYNC_LOSS_ERROR_INDICATION_ENABLE == STD_ON)
#include FRSM_SYNC_LOSS_ERROR_INDICATION_HEADER
#endif /* FRSM_SYNC_LOSS_ERROR_INDICATION_ENABLE == STD_ON */

#include <FrSM_guards.h> /* After External/API/Cfg, before Trace/internal/private */
#include <FrSM_Trace.h>
#include <FrSM_Int.h>

/*======================[Version Check]=======================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef FRSM_VENDOR_ID /* configuration check */
#error FRSM_VENDOR_ID must be defined
#endif

#if (FRSM_VENDOR_ID != 1U) /* vendor check */
#error FRSM_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef FRSM_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error FRSM_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (FRSM_AR_RELEASE_MAJOR_VERSION != 4U)
#error FRSM_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef FRSM_AR_RELEASE_MINOR_VERSION /* configuration check */
#error FRSM_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (FRSM_AR_RELEASE_MINOR_VERSION != 0U )
#error FRSM_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef FRSM_AR_RELEASE_REVISION_VERSION /* configuration check */
#error FRSM_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (FRSM_AR_RELEASE_REVISION_VERSION != 3U )
#error FRSM_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef FRSM_SW_MAJOR_VERSION /* configuration check */
#error FRSM_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (FRSM_SW_MAJOR_VERSION != 5U)
#error FRSM_SW_MAJOR_VERSION wrong (!= 5U)
#endif

#ifndef FRSM_SW_MINOR_VERSION /* configuration check */
#error FRSM_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (FRSM_SW_MINOR_VERSION < 3U)
#error FRSM_SW_MINOR_VERSION wrong (< 3U)
#endif

#ifndef FRSM_SW_PATCH_VERSION /* configuration check */
#error FRSM_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (FRSM_SW_PATCH_VERSION < 18U)
#error FRSM_SW_PATCH_VERSION wrong (< 18U)
#endif

/*======================[Global Macros]=======================================*/

#define FRSM_INSTANCE_ID            0U /**< instance ID */





/*------------------------[Defensive programming]----------------------------*/

#if (defined FRSM_PRECONDITION_ASSERT)
#error FRSM_PRECONDITION_ASSERT is already defined
#endif

#if (defined FRSM_PRECONDITION_ASSERT_NO_EVAL)
#error FRSM_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (FRSM_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRSM_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), FRSM_MODULE_ID, FRSM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRSM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), FRSM_MODULE_ID, FRSM_INSTANCE_ID, (ApiId))
#else
#define FRSM_PRECONDITION_ASSERT(Condition, ApiId)
#define FRSM_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined FRSM_POSTCONDITION_ASSERT)
#error FRSM_POSTCONDITION_ASSERT is already defined
#endif

#if (defined FRSM_POSTCONDITION_ASSERT_NO_EVAL)
#error FRSM_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (FRSM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRSM_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), FRSM_MODULE_ID, FRSM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRSM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), FRSM_MODULE_ID, FRSM_INSTANCE_ID, (ApiId))
#else
#define FRSM_POSTCONDITION_ASSERT(Condition, ApiId)
#define FRSM_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined FRSM_INVARIANT_ASSERT)
#error FRSM_INVARIANT_ASSERT is already defined
#endif

#if (defined FRSM_INVARIANT_ASSERT_NO_EVAL)
#error FRSM_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (FRSM_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRSM_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), FRSM_MODULE_ID, FRSM_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define FRSM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), FRSM_MODULE_ID, FRSM_INSTANCE_ID, (ApiId))
#else
#define FRSM_INVARIANT_ASSERT(Condition, ApiId)
#define FRSM_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined FRSM_STATIC_ASSERT)
# error FRSM_STATIC_ASSERT is already defined
#endif
#if (FRSM_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define FRSM_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define FRSM_STATIC_ASSERT(expr)
#endif

#if (defined FRSM_UNREACHABLE_CODE_ASSERT)
#error FRSM_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (FRSM_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define FRSM_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(FRSM_MODULE_ID, FRSM_INSTANCE_ID, (ApiId))
#else
#define FRSM_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined FRSM_INTERNAL_API_ID)
#error FRSM_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define FRSM_INTERNAL_API_ID DET_INTERNAL_API_ID



/* states of the internal state machine */
#define FRSM_STATE_HALT_REQ         0U /**< waiting for halt request to complete */
#define FRSM_STATE_READY            1U /**< ready state - controller is initialized */
#define FRSM_STATE_WAKEUP           2U /**< wakeup state - waiting for wakeup to complete */
#define FRSM_STATE_STARTUP          3U /**< startup state - waiting for startup to complete */
#define FRSM_STATE_ONLINE_PASSIVE   4U /**< online passive - controller is in normal-passive POC state */
#define FRSM_STATE_ONLINE           5U /**< online state - reached synchronization */
#if (FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON)
#define FRSM_STATE_KEYSLOT_ONLY     6U /**< keyslot only - data only transmitted in key slots */
#endif /* FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON */

/**
 * \brief Maximum number of states used.
 *
 * Some states may be disabled by pre-compile configuration parameters.
 */
#define FRSM_STATE_NUMSTATES        7U


/* defines of startup options bitfield */
#define FRSM_OPTIONS_TRCV_CHANNEL_A          0x01U   /**< channel A transceiver is connected */
#define FRSM_OPTIONS_TRCV_CHANNEL_B          0x02U   /**< channel B transceiver is connected */
#define FRSM_OPTIONS_IS_COLDSTART_ECU        0x04U   /**< shall this node perform a coldstart? */
#define FRSM_OPTIONS_IS_WAKEUP_ECU           0x08U   /**< shall this node perform a wakeup? */
#define FRSM_OPTIONS_CHECK_WAKEUP_REASON     0x10U   /**< shall this node check the wakeup reason to
                                                       *  skip wakeup in case of wakeup by bus? */
#define FRSM_OPTIONS_IS_DUAL_CHANNEL_NODE    0x20U   /**< is this node connected to both channels
                                                       *  of the cluster? */
#define FRSM_OPTIONS_DELAY_STARTUP_WO_WAKEUP 0x40U   /**< shall timer t1 be started instead of
                                                       *  immediately calling FrIf_AllowColdstart
                                                       *  in case of a startup without wakeup? */


/* FrSM API service IDs */
#define FRSM_SID_INIT               0x01U  /**< SID for FrSM_Init() */
#define FRSM_SID_REQUESTCOMMODE     0x02U  /**< SID for FrSM_RequestComMode() */
#define FRSM_SID_GETCURRENTCOMMODE  0x03U  /**< SID for FrSM_GetCurrentComMode() */
#define FRSM_SID_GETVERSIONINFO     0x04U  /**< SID for FrSM_GetVersionInfo() */
#if (FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON)
#define FRSM_SID_ALLSLOTS           0x05U  /**< SID for FrSM_AllSlots() */
#endif /* FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON */
#define FRSM_SID_SETECUPASSIVE      0x06U  /**< SID for FrSM_SetEcuPassive() */
#define FRSM_SID_MAINFUNCTION       0x80U  /**< SID for FrSM_MainFunction_#() */


#if (FRSM_DEV_ERROR_DETECT == STD_ON)

/**
 * \brief macro for simplified DET usage - map to Det - API function call
 */
#define FRSM_DET_REPORTERROR(nErrorID, nFuncID) \
    FRSM_DET_REPORTERROR_WITH_INSTANCE((nErrorID), (nFuncID), FRSM_INSTANCE_ID)

/**
 * \brief macro for simplified DET usage with custom instance ID - map to Det - API function call
 */
#define FRSM_DET_REPORTERROR_WITH_INSTANCE(nErrorID, nFuncID, nInstanceId) \
    (void) Det_ReportError( \
                            FRSM_MODULE_ID,   \
                            (nInstanceId),    \
                            (nFuncID),        \
                            (nErrorID)        \
                          )
#endif /* (FRSM_DEV_ERROR_DETECT == STD_ON) */

/** \brief index into first array dimension of FrSM_Int2BswMStateMapping*/
#if (FRSM_SETECUPASSIVE_ENABLE == STD_ON)
#define FRSM_TRCV_STATE_IDX      ((FrSM_PassiveState != FALSE) ? 1U : 0U)
#else
#define FRSM_TRCV_STATE_IDX      0U
#endif /* FRSM_SETECUPASSIVE_ENABLE == STD_ON */

/**
 * \brief macro to check if timer t3 is active
 */
#define FRSM_T3_NOTACTIVE(pClstCfg, FrSM_ClstIdx) \
    (((pClstCfg)->T3CounterStart != 0U) && \
     (FrSM_ClstRuntimeData[(FrSM_ClstIdx)].T3Counter == 0U))

/* \brief Macros managing the preemption state of the state machine */
#define FRSM_STATEMACHINE_RESET                     0x00U
#define FRSM_STATEMACHINE_LOCK                      0x01U
#define FRSM_STATEMACHINE_MAINFUNCTION_SUSPENDED    0x02U
#define FRSM_STATEMACHINE_REQUESTCOMMODE_SUSPENDED  0x04U


#define FRSM_UNINIT 0U /**< FrSM is not initialized */
#define FRSM_INIT 1U   /**< FrSM is initialized */

/*======================[Global Data Types]===================================*/

/** \brief Values of SWS state machine condition WUReason */
typedef enum
{
    FRSM_NO_WU_BY_BUS,
    FRSM_PARTIAL_WU_BY_BUS,
    FRSM_ALL_WU_BY_BUS
} FrSM_ConditionWUReasonType;

/** \brief Which transition is taken from FRSM_WAKEUP to FRSM_STARTUP? */
typedef enum
{
    FRSM_TRANSITION_T03_A,
    FRSM_TRANSITION_T03_B,
    FRSM_TRANSITION_T03_C,
    FRSM_TRANSITION_T03_D,
    FRSM_TRANSITION_T03_E
} FrSM_TransitionT03Type;

/*======================[Global Data]=========================================*/

#define FRSM_START_SEC_VAR_INIT_8
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

/**
 * \brief global variable for the actual initialization status of the FrSM
 */
STATIC VAR(uint8, FRSM_VAR_FAST) FrSM_InitStatus = FRSM_UNINIT;

#define FRSM_STOP_SEC_VAR_INIT_8
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */



#define FRSM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

/**
 * \brief variable for the pointer to the config of FrSM
 */
STATIC P2CONST(FrSM_ConfigType, FRSM_VAR_CLEARED, FRSM_APPL_CONST) FrSM_GlobalConfigPtr;

#if (FRSM_COMM_INDICATION_ENABLE == STD_ON)

#if (FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_OFF)
/** \brief Initialized to COMM_NO_COMMUNICATION, passed to Comm_BusSM_ModeIndication() */
STATIC VAR(ComM_ModeType,FRSM_VAR_CLEARED) FrSM_NoCommunication;
/** \brief Initialized to COMM_FULL_COMMUNICATION, passed to Comm_BusSM_ModeIndication() */
STATIC VAR(ComM_ModeType,FRSM_VAR_CLEARED) FrSM_FullCommunication;
#endif /* FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_OFF */

#endif /* FRSM_COMM_INDICATION_ENABLE */

#define FRSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */



#define FRSM_START_SEC_CONST_UNSPECIFIED
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

/** \brief Maps internal state to external state */
STATIC CONST(ComM_ModeType,FRSM_CONST) FrSM_Int2ExtStateMapping[FRSM_STATE_NUMSTATES] =
{
    COMM_NO_COMMUNICATION,   /* FRSM_STATE_HALT_REQ        */
    COMM_NO_COMMUNICATION,   /* FRSM_STATE_READY           */
    COMM_NO_COMMUNICATION,   /* FRSM_STATE_WAKEUP          */
    COMM_NO_COMMUNICATION,   /* FRSM_STATE_STARTUP         */
    COMM_NO_COMMUNICATION,   /* FRSM_STATE_ONLINE_PASSIVE  */
    COMM_FULL_COMMUNICATION, /* FRSM_STATE_ONLINE          */
    COMM_NO_COMMUNICATION    /* FRSM_STATE_KEYSLOT_ONLY    */
};

#if (FRSM_REPORT_TO_BSMW_ENABLE == STD_ON)

/** \brief Maps internal state to BswM state
 *
 * First array dimension maps TrcvCtrl state machine
 */
#if (FRSM_SETECUPASSIVE_ENABLE == STD_ON)
STATIC CONST(FrSM_BswM_StateType,FRSM_CONST) FrSM_Int2BswMStateMapping[2U][FRSM_STATE_NUMSTATES] =
#else /* FRSM_SETECUPASSIVE_ENABLE == STD_ON */
STATIC CONST(FrSM_BswM_StateType,FRSM_CONST) FrSM_Int2BswMStateMapping[1U][FRSM_STATE_NUMSTATES] =
#endif /* FRSM_SETECUPASSIVE_ENABLE == STD_ON */
{
    /* transceiver active */
    {
        FRSM_BSWM_HALT_REQ,                     /* FRSM_STATE_HALT_REQ       */
        FRSM_BSWM_READY,                        /* FRSM_STATE_READY          */
        FRSM_BSWM_WAKEUP,                       /* FRSM_STATE_WAKEUP         */
        FRSM_BSWM_STARTUP,                      /* FRSM_STATE_STARTUP        */
        FRSM_BSWM_ONLINE_PASSIVE,               /* FRSM_STATE_ONLINE_PASSIVE */
        FRSM_BSWM_ONLINE,                       /* FRSM_STATE_ONLINE         */
        FRSM_BSWM_KEYSLOT_ONLY                  /* FRSM_STATE_KEYSLOT_ONLY   */
    }
#if (FRSM_SETECUPASSIVE_ENABLE == STD_ON)
    /* transceiver passive */
    ,{
        FRSM_BSWM_HALT_REQ_ECU_PASSIVE,         /* FRSM_STATE_HALT_REQ       */
        FRSM_BSWM_READY_ECU_PASSIVE,            /* FRSM_STATE_READY          */
        FRSM_BSWM_WAKEUP_ECU_PASSIVE,           /* FRSM_STATE_WAKEUP         */
        FRSM_BSWM_STARTUP_ECU_PASSIVE,          /* FRSM_STATE_STARTUP        */
        FRSM_BSWM_ONLINE_PASSIVE_ECU_PASSIVE,   /* FRSM_STATE_ONLINE_PASSIVE */
        FRSM_BSWM_ONLINE_ECU_PASSIVE,           /* FRSM_STATE_ONLINE         */
        FRSM_BSWM_KEYSLOT_ONLY_ECU_PASSIVE      /* FRSM_STATE_KEYSLOT_ONLY   */
    }
#endif /* FRSM_SETECUPASSIVE_ENABLE == STD_ON */
};

#endif /* (FRSM_REPORT_TO_BSMW_ENABLE == STD_ON) */

#define FRSM_STOP_SEC_CONST_UNSPECIFIED
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */


#define FRSM_START_SEC_VAR_CLEARED_8
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

#if (FRSM_SETECUPASSIVE_ENABLE == STD_ON)
/** \brief state of transceiver state machine */
STATIC VAR(boolean, FRSM_VAR_CLEARED) FrSM_PassiveState;
#endif /* FRSM_SETECUPASSIVE_ENABLE == STD_ON */

#define FRSM_STOP_SEC_VAR_CLEARED_8
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

/*======================[Static Function Definitions]=========================*/

#define FRSM_START_SEC_CODE
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

/* if single cluster optimization is enabled, there are a couple of macros that translate to simplified code */
#if (FRSM_SINGLE_CLST_OPT_ENABLE == STD_ON)

#define FRSM_CLSTIDX 0U                                 /**< always use cluster index 0 */

#else /* FRSM_SINGLE_CLST_OPT_ENABLE */

#define FRSM_CLSTIDX (FrSM_ClstIdx)                     /**< use cluster index variable */

STATIC FUNC(uint8,FRSM_CODE) FrSM_LookupClstIdx(const NetworkHandleType NetworkHandle);


/**
 * \brief Definition of a lookup function that translates a NetworkHandle into a cluster index
 *
 * \return Returns FrSM cluster index, if network handle is valid.
 * Returns FrSM_GlobalConfigPtr->NumClst + 1, if network handle is invalid.
 * Returns 0 if development error detection is enabled and module is not initialized
 */
STATIC FUNC(uint8,FRSM_CODE) FrSM_LookupClstIdx(const NetworkHandleType NetworkHandle)
{
    uint8 retVal = 0U;

/* check if development error detection is enabled */
#if (FRSM_DEV_ERROR_DETECT == STD_ON)
    if (FrSM_InitStatus == FRSM_INIT)
#endif /* FRSM_DEV_ERROR_DETECT */
    {
        uint8_least iClst; /**< cluster iterator variable */

        /* loop over all clusters contained in the configuration */
        for(iClst = 0U; iClst < FrSM_GlobalConfigPtr->NumClst; iClst++)
        {
            /* test handle index of this cluster */
            if(FrSM_GlobalConfigPtr->aClst[iClst].NetworkHandleId == NetworkHandle)
            {
                /* if we hit the right cluster, stop looping */
                break;
            }
        }

        /* return the index found */
        retVal = (uint8)iClst;
    }

    return retVal;
}

#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */

STATIC FUNC(void,FRSM_CODE) FrSM_RequestComMode_NoCommunication
    (
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg
    );

STATIC FUNC(void,FRSM_CODE) FrSM_MainFunctionInternal
    (
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        uint8 RequestedState
    );


STATIC FUNC(void,FRSM_CODE) FrSM_Tick(
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        CONSTP2VAR(boolean,AUTOMATIC,AUTOMATIC) t1Elapsed,
        CONSTP2VAR(boolean,AUTOMATIC,AUTOMATIC) t2Elapsed,
        CONSTP2VAR(boolean,AUTOMATIC,AUTOMATIC) t3Elapsed
    );

/**
 * \brief Implements progress of time. Decrements timers t1, t2, t3 and sets flags while a
 * timer expires or if a timer is not active any more.
 *
 * \param[in,out] t1Elapsed If t1 expires in this main function invocation, FrSM_Tick() sets this
 * parameter to TRUE (unless FrSMDurationT1 = 0).
 * \param[in,out] t2Elapsed If t2 expires in this main function invocation, FrSM_Tick() sets this
 * parameter to TRUE.
 * \param[in,out] t3Elapsed If t3 expires in this main function invocation, FrSM_Tick() sets this
 * parameter to TRUE.
 * \param[in,out] t3IsNotActive If t3 is not active and does not expire in this main function
 * invocation and if configuration parameter FrSMDurationT3 > 0, FrSM_Tick() sets this parameter
 * to TRUE.
 */
STATIC FUNC(void,FRSM_CODE) FrSM_Tick(
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        CONSTP2VAR(boolean,AUTOMATIC,AUTOMATIC) t1Elapsed,
        CONSTP2VAR(boolean,AUTOMATIC,AUTOMATIC) t2Elapsed,
        CONSTP2VAR(boolean,AUTOMATIC,AUTOMATIC) t3Elapsed
    )
{
    /* a value of 0 means that t1 is not used -
     * in context of its usages this means that it fires immediately
     */

    if(pClstCfg->T1CounterStart != 0U)
    {
        /* otherwise check if the timer has elapsed */
        if(FrSM_ClstRuntimeData[FRSM_CLSTIDX].T1Counter == 1U)
        {
            /* mark timer as elapsed */
            *t1Elapsed = TRUE;

            /* stop timer */
            FrSM_ClstRuntimeData[FRSM_CLSTIDX].T1Counter = 0U;
        }
        else if(FrSM_ClstRuntimeData[FRSM_CLSTIDX].T1Counter > 0U)
        {
            /* decrement timer counter */
            FrSM_ClstRuntimeData[FRSM_CLSTIDX].T1Counter--;
        }
        else
        {
            /* nothing to do */
        }
    }

    /* a value of 0 means that t2 is not used -
     * in context of its usages this means that it never fires
     */
    if(pClstCfg->T2CounterStart != 0U)
    {
        /* otherwise check if the timer has elapsed */
        if(FrSM_ClstRuntimeData[FRSM_CLSTIDX].T2Counter == 1U)
        {
            /* mark timer as elapsed */
            *t2Elapsed = TRUE;

            /* stop timer */
            FrSM_ClstRuntimeData[FRSM_CLSTIDX].T2Counter = 0U;
        }
        else if(FrSM_ClstRuntimeData[FRSM_CLSTIDX].T2Counter > 0U)
        {
            /* decrement timer counter */
            FrSM_ClstRuntimeData[FRSM_CLSTIDX].T2Counter--;
        }
        else
        {
            /* nothing to do */
        }
    }

    /* a value of 0 means that t3 never fires */
    if(pClstCfg->T3CounterStart != 0U)
    {
        /* otherwise check if the timer has elapsed */
        if(FrSM_ClstRuntimeData[FRSM_CLSTIDX].T3Counter == 1U)
        {
            /* mark timer as elapsed */
            *t3Elapsed = TRUE;

            /* stop timer */
            FrSM_ClstRuntimeData[FRSM_CLSTIDX].T3Counter = 0U;
        }
        else if(FrSM_ClstRuntimeData[FRSM_CLSTIDX].T3Counter > 0U)
        {
            /* decrement timer counter */
            FrSM_ClstRuntimeData[FRSM_CLSTIDX].T3Counter--;
        }
        else
        {
            /* nothing to do */
        }
    }
}

#if ( (FRSM_SYNC_LOSS_ERROR_INDICATION_ENABLE == STD_ON)                        \
      || (FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS == TS_PROD_ERR_REP_TO_DEM)   \
      || ( (FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS == TS_PROD_ERR_REP_TO_DET) \
           && (FRSM_DEV_ERROR_DETECT == STD_ON)                                 \
          ))

STATIC FUNC(void,FRSM_CODE) FrSM_FE_DEM_SYNC_LOSS
    (
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg
    );

/**
 * \brief Funcional element FE_DEM_SYNC_LOSS of state machine
 *
 * This function performs the functional element FE_DEM_SYNC_LOSS
 * during certain state transitions as defined on page 22 of the
 * AUTOSAR FlexRay State Manager SWS.
 *
 * \param[in] pClstCfg  Points to configuration data of the actual cluster
 */
STATIC FUNC(void,FRSM_CODE) FrSM_FE_DEM_SYNC_LOSS
    (
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg
    )
{

#if (FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS == TS_PROD_ERR_REP_TO_DEM)
    if (pClstCfg->ClusterSyncLossDemEventId != FRSM_INVALID_DEM_EVENTID)
    {
        Dem_ReportErrorStatus(pClstCfg->ClusterSyncLossDemEventId, DEM_EVENT_STATUS_FAILED);
    }
#elif ((FRSM_DEV_ERROR_DETECT == STD_ON) && (FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS == TS_PROD_ERR_REP_TO_DET))
    FRSM_DET_REPORTERROR_WITH_INSTANCE(
                                        FRSM_E_DEMTODET_CLUSTER_SYNC_LOSS,
                                        FRSM_SID_MAINFUNCTION,
                                        pClstCfg->NetworkHandleId
                                      );
#endif /* FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS */

#if (FRSM_SYNC_LOSS_ERROR_INDICATION_ENABLE == STD_ON)
    FRSM_SYNC_LOSS_ERROR_INDICATION_NAME(pClstCfg->NetworkHandleId, TRUE);
#endif /* FRSM_SYNC_LOSS_ERROR_INDICATION_ENABLE == STD_ON */

}

#else /* condition if FrSM_FE_DEM_SYNC_LOSS present in code */

#define FrSM_FE_DEM_SYNC_LOSS(pClstCfg)

#endif  /* condition if FrSM_FE_DEM_SYNC_LOSS present in code */

#if ( (FRSM_SYNC_LOSS_ERROR_INDICATION_ENABLE == STD_ON)                      \
      || (FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS == TS_PROD_ERR_REP_TO_DEM) \
    )

STATIC FUNC(void,FRSM_CODE) FrSM_FE_DEM_SYNC_LOSS_PASSED
    (
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg
    );

/**
 * \brief Funcional element FE_DEM_SYNC_LOSS_PASSED of state machine
 *
 * This function performs the functional element FE_DEM_SYNC_LOSS_PASSED
 * during certain state transitions as defined on page 23 of the
 * AUTOSAR FlexRay State Manager SWS.
 *
 * \param[in] pClstCfg  Points to configuration data of the actual cluster
 */
STATIC FUNC(void,FRSM_CODE) FrSM_FE_DEM_SYNC_LOSS_PASSED
    (
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg
    )
{

#if (FRSM_SYNC_LOSS_ERROR_INDICATION_ENABLE == STD_ON)
    FRSM_SYNC_LOSS_ERROR_INDICATION_NAME(pClstCfg->NetworkHandleId, FALSE);
#endif /* FRSM_SYNC_LOSS_ERROR_INDICATION_ENABLE == STD_ON */

#if (FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS == TS_PROD_ERR_REP_TO_DEM)
    if (pClstCfg->ClusterSyncLossDemEventId != FRSM_INVALID_DEM_EVENTID)
    {
        Dem_ReportErrorStatus(pClstCfg->ClusterSyncLossDemEventId, DEM_EVENT_STATUS_PASSED);
    }
#endif /* FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS */
}

#else /* condition if FrSM_FE_DEM_SYNC_LOSS_PASSED present in code */

#define FrSM_FE_DEM_SYNC_LOSS_PASSED(pClstCfg)

#endif /* condition if FrSM_FE_DEM_SYNC_LOSS_PASSED present in code */

STATIC FUNC(void,FRSM_CODE) FrSM_HandleTransitions_T30_T31_T32_T33
    (
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        const boolean t3Elapsed
    );

/**
 * \brief Checks event/conditions and executes actions for transitions T30/T31/T32/T33.
 *
 * Invoked in states FRSM_STARTUP, FRSM_WAKEUP and FRSM_ONLINE_PASSIVE.
 * Note that t3 can only expire in the aforementioned states.
 *
 * \param[in] t3Elapsed TRUE if timer t3 expires in this main function invocation, FALSE otherwise
 * \param[in] t3IsNotActive TRUE if timer t3 is not active and does not expire during this main
 * function invocation and if configuration parameter FrSMDurationT3 > 0, FALSE otherwise
 */
STATIC FUNC(void,FRSM_CODE) FrSM_HandleTransitions_T30_T31_T32_T33
    (
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        const boolean t3Elapsed
    )
{

    if (t3Elapsed)
    {
        /* execute T30 */

        /* perform functionality 'FE_DEM_STATUS_FAILED' */
#if (FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP == TS_PROD_ERR_REP_TO_DEM)
        if (pClstCfg->ClusterStartupDemEventId != FRSM_INVALID_DEM_EVENTID)
        {
            Dem_ReportErrorStatus(pClstCfg->ClusterStartupDemEventId, DEM_EVENT_STATUS_FAILED);
        }
#elif ((FRSM_DEV_ERROR_DETECT == STD_ON) && (FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP == TS_PROD_ERR_REP_TO_DET))
        FRSM_DET_REPORTERROR_WITH_INSTANCE(
                                            FRSM_E_DEMTODET_CLUSTER_STARTUP,
                                            FRSM_SID_MAINFUNCTION,
                                            pClstCfg->NetworkHandleId
                                          );
#endif /* FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP */

#if (FRSM_FRNM_STARTUPERROR_ENABLE == STD_ON)
        /* perform functionality 'FE_STARTUP_ERROR_IND' */
        FrNm_StartupError(pClstCfg->NetworkHandleId);
#endif /* FRSM_FRNM_STARTUPERROR_ENABLE == STD_ON */
    }
#if (FRSM_FRNM_STARTUPERROR_ENABLE == STD_ON)
    else if (FRSM_T3_NOTACTIVE(pClstCfg, FRSM_CLSTIDX))
    {
        /* execute T31 or T32 or T33 */

        /* perform functionality 'FE_STARTUP_ERROR_IND' */
        FrNm_StartupError(pClstCfg->NetworkHandleId);
    }
    else
    {
        /* do nothing */
    }
#endif /* FRSM_FRNM_STARTUPERROR_ENABLE == STD_ON */

#if (FRSM_FRNM_STARTUPERROR_ENABLE == STD_OFF) && \
    (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
    TS_PARAM_UNUSED(FrSM_ClstIdx);
#endif /* FRSM_FRNM_STARTUPERROR_ENABLE == STD_OFF && ... */

#if ( \
      (FRSM_FRNM_STARTUPERROR_ENABLE == STD_OFF) && \
      (FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP != TS_PROD_ERR_REP_TO_DEM) && \
      ( \
        (FRSM_DEV_ERROR_DETECT != STD_ON) || \
        (FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP != TS_PROD_ERR_REP_TO_DET) \
      ) \
    )
    TS_PARAM_UNUSED(pClstCfg);
#endif /* ... */

}


STATIC FUNC(uint8, FRSM_CODE) FrSM_DoTransition_T03
    (
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        const uint8 frIfCtrlIdx
    );

/**
 * \brief Checks which transition out of T03[abcde] shall be taken and executes the corresponding
 * actions.
 *
 * \return New state of internal state machine.
 */
STATIC FUNC(uint8, FRSM_CODE) FrSM_DoTransition_T03
    (
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        const uint8 frIfCtrlIdx
    )
{
    /* which transition is taken to FRSM_STARTUP? */
    FrSM_TransitionT03Type transitionT03;

    if(FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupType == FRSM_SINGLE_CHANNEL_WAKEUP)
    {
        /* execute T03 (abc) */

        if(pClstCfg->NumWakeupPatterns == 1U)
        {
            /* execute T03 (a) */
            transitionT03 = FRSM_TRANSITION_T03_A;
        }
        else if(
               (FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupTransmitted)
               ||
               (FrSM_ClstRuntimeData[FRSM_CLSTIDX].T1Counter == 0U)
              )
        {
            /* execute T03 (b) */
            transitionT03 = FRSM_TRANSITION_T03_B;
        }
        else
        {
            /* execute T03 (c) */
            transitionT03 = FRSM_TRANSITION_T03_C;
        }
    }
    else /* FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupType != FRSM_SINGLE_CHANNEL_WAKEUP */
    {
        /* execute T03 (de) */

        if (FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupType == FRSM_DUAL_CHANNEL_WAKEUP)
        {
            /* execute T03 (d) */
            transitionT03 = FRSM_TRANSITION_T03_D;
        }
        else
        {
            FRSM_INVARIANT_ASSERT
                (
                    FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupType
                    ==
                    FRSM_DUAL_CHANNEL_WAKEUP_FORWARD,
                    FRSM_SID_MAINFUNCTION
                );

            /* execute T03 (e) */
            transitionT03 = FRSM_TRANSITION_T03_E;
        }
    }

    /* perform functionality 'FE_START' */
    (void)FrIf_StartCommunication(frIfCtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

    switch (transitionT03)
    {
        case FRSM_TRANSITION_T03_A:
            /* perform functionality 'cancel t1' and 'start t1' */
            FrSM_ClstRuntimeData[FRSM_CLSTIDX].T1Counter = pClstCfg->T1CounterStart;
            break;
        case FRSM_TRANSITION_T03_B:
            /* perform functionality 'cancel t1' */
            FrSM_ClstRuntimeData[FRSM_CLSTIDX].T1Counter = 0U;
            break;
        default:
            /* do nothing */
            break;
    }

    /* perform functionality 'start t2' */
    FrSM_ClstRuntimeData[FRSM_CLSTIDX].T2Counter = pClstCfg->T2CounterStart;

    switch (transitionT03)
    {
        case FRSM_TRANSITION_T03_B:
        case FRSM_TRANSITION_T03_E:
            /* perform functionality 'FE_ALLOW_COLDSTART' */
            if((pClstCfg->StartupOptions&FRSM_OPTIONS_IS_COLDSTART_ECU) != 0U)
            {
                (void)FrIf_AllowColdstart(frIfCtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
            }
            break;
        default:
            /* if t1 is disabled, perform 'FE_ALLOW_COLDSTART' immediately (FrSm142) */
            if(pClstCfg->T1CounterStart == 0U)
            {
                /* perform functionality 'FE_ALLOW_COLDSTART' */
                if((pClstCfg->StartupOptions&FRSM_OPTIONS_IS_COLDSTART_ECU) != 0U)
                {
                    (void)FrIf_AllowColdstart(frIfCtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
                }
            }
            break;
    }

    return FRSM_STATE_STARTUP;
}


STATIC FUNC(uint8,FRSM_CODE) FrSM_HandleTransitions_T10ab_T16ab_T20ab
    (
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        uint8 newState,
        const uint8 frIfCtrlIdx,
        CONSTP2VAR(Fr_POCStatusType, AUTOMATIC, AUTOMATIC) pocStatus
    );

/**
 * \brief Checks conditions and executes actions for transitions T10[ab], T16[ab], T20[ab].
 *
 * \param[in] newState FRSM_ONLINE or FRSM_KEYSLOT_ONLY
 *
 * \return New state of internal state machine. If no transition is triggered, parameter
 * \a newState is returned.
 */
STATIC FUNC(uint8,FRSM_CODE) FrSM_HandleTransitions_T10ab_T16ab_T20ab
    (
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        uint8 newState,
        const uint8 frIfCtrlIdx,
        CONSTP2VAR(Fr_POCStatusType, AUTOMATIC, AUTOMATIC) pocStatus
    )
{
    /* check trigger for T10[ab], T20[ab]
     * assume Wakeup is finished if FlexRay CC fails */

    if(
        (pocStatus->State == FR_POCSTATE_HALT)
        ||
        (pocStatus->Freeze)
      )
    {
        const boolean triggerT10ab =
            (
                ((pClstCfg->StartupOptions&FRSM_OPTIONS_CHECK_WAKEUP_REASON) != 0U)
                ||
                ((pClstCfg->StartupOptions&FRSM_OPTIONS_IS_WAKEUP_ECU) == 0U)
            ) ? TRUE : FALSE;

        const boolean triggerT20ab = (!triggerT10ab) ? TRUE : FALSE;

        FRSM_INVARIANT_ASSERT(
            FrSM_ClstRuntimeData[FRSM_CLSTIDX].RequestedState == FRSM_STATE_ONLINE,
            FRSM_SID_MAINFUNCTION);

        if (triggerT20ab)
        {
            FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupType = FRSM_SINGLE_CHANNEL_WAKEUP;
        }

        /* perform functionality 'FE_DEM_SYNC_LOSS' */
        FrSM_FE_DEM_SYNC_LOSS(pClstCfg);

        /* perform functionality 'FE_STOP_FRIF' */
        (void)FrIf_SetState(pClstCfg->FrIfClstIdx,FRIF_GOTO_OFFLINE); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

#if (FRSM_COMM_INDICATION_ENABLE == STD_ON)
        /* perform functionality 'FE_NO_COM_IND' */

#if (FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_OFF)
        FRSM_CALL_COMM_BUSSM_MODEINDICATION(pClstCfg->NetworkHandleId,&FrSM_NoCommunication);
#else
        /* !LINKSTO FrSM.EB.SchM_Call_ComM_BusSM_ModeIndication, 1 */
        if (FRSM_CALL_COMM_BUSSM_MODEINDICATION(pClstCfg->FrIfClstIdx,pClstCfg->NetworkHandleId,COMM_NO_COMMUNICATION) != SCHM_E_OK)
        {
#if (FRSM_DEV_ERROR_DETECT == STD_ON)
          /* !LINKSTO FrSM.EB.SchM_Call_ComM_BusSM_ModeIndication_DET, 1 */
            FRSM_DET_REPORTERROR_WITH_INSTANCE(
                                                FRSM_E_MODEINDICATION,
                                                FRSM_SID_MAINFUNCTION,
                                                pClstCfg->NetworkHandleId
                                              );
#endif /* FRSM_DEV_ERROR_DETECT == STD_ON */
        }
#endif /* FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_OFF */

#endif /* FRSM_COMM_INDICATION_ENABLE */

        /* perform functionality 'FE_CONFIG' */
        (void)FrIf_ControllerInit(frIfCtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

        if (triggerT10ab)
        {
            /* perform functionality 'FE_START' */
            (void)FrIf_StartCommunication(frIfCtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
        }
        else /* triggerT20ab holds */
        {
            /* perform functionality 'FE_WAKEUP' */
            (void)FrIf_SendWUP(frIfCtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
        }

        /* perform functionality 'startupCounter := 1' */
        FrSM_ClstRuntimeData[FRSM_CLSTIDX].StartupCounter = 1U;

        if (triggerT10ab)
        {
            /* perform functionality 'start t2' */
            FrSM_ClstRuntimeData[FRSM_CLSTIDX].T2Counter = pClstCfg->T2CounterStart;
        }
        else /* triggerT20ab holds */
        {
            /* perform functionality 'start t1' */
            FrSM_ClstRuntimeData[FRSM_CLSTIDX].T1Counter = pClstCfg->T1CounterStart;
        }

        /* perform functionality 'start t3' */
        FrSM_ClstRuntimeData[FRSM_CLSTIDX].T3Counter = pClstCfg->T3CounterStart;

        if (triggerT10ab)
        {
            newState = FRSM_STATE_STARTUP;
        }
        else /* triggerT20ab */
        {
            newState = FRSM_STATE_WAKEUP;
        }
    }
    /* check trigger for T16ab */
    else if(pocStatus->State == FR_POCSTATE_NORMAL_PASSIVE)
    {
        /* perform functionality 'FE_DEM_SYNC_LOSS' */
        FrSM_FE_DEM_SYNC_LOSS(pClstCfg);

        /* perform functionality 'FE_STOP_FRIF' */
        (void)FrIf_SetState(pClstCfg->FrIfClstIdx,FRIF_GOTO_OFFLINE); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

#if (FRSM_COMM_INDICATION_ENABLE == STD_ON)
        /* perform functionality 'FE_NO_COM_IND' */


#if (FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_OFF)
        FRSM_CALL_COMM_BUSSM_MODEINDICATION(pClstCfg->NetworkHandleId,&FrSM_NoCommunication);
#else
        /* !LINKSTO FrSM.EB.SchM_Call_ComM_BusSM_ModeIndication, 1 */
        if (FRSM_CALL_COMM_BUSSM_MODEINDICATION(pClstCfg->FrIfClstIdx,pClstCfg->NetworkHandleId,COMM_NO_COMMUNICATION) != SCHM_E_OK)
        {
#if (FRSM_DEV_ERROR_DETECT == STD_ON)
          /* !LINKSTO FrSM.EB.SchM_Call_ComM_BusSM_ModeIndication_DET, 1 */
            FRSM_DET_REPORTERROR_WITH_INSTANCE(
                                                FRSM_E_MODEINDICATION,
                                                FRSM_SID_MAINFUNCTION,
                                                pClstCfg->NetworkHandleId
                                              );
#endif /* FRSM_DEV_ERROR_DETECT == STD_ON */
        }
#endif /* FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_OFF */

#endif /* FRSM_COMM_INDICATION_ENABLE */

        /* perform functionality 'start t3' */
        FrSM_ClstRuntimeData[FRSM_CLSTIDX].T3Counter = pClstCfg->T3CounterStart;

        /* switch to new state */
        newState = FRSM_STATE_ONLINE_PASSIVE;
    }
    else
    {
        /* nothing to do */
    }

    return newState;
}

STATIC FUNC(FrSM_ConditionWUReasonType, FRSM_CODE) FrSM_ConditionWUReason
    (
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        const uint8 frIfCtrlIdx,
        CONSTP2VAR(boolean,AUTOMATIC,AUTOMATIC) pChannelAWakeUpDetected
    );

/**
 * \brief Returns SWS state machine condition WUReason
 *
 * \param[out] pChannelAWakeUpDetected Set to TRUE if wakeup is detected on channel A, otherwise
 * set to FALSE.
 */
STATIC FUNC(FrSM_ConditionWUReasonType, FRSM_CODE) FrSM_ConditionWUReason
    (
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        const uint8 frIfCtrlIdx,
        CONSTP2VAR(boolean,AUTOMATIC,AUTOMATIC) pChannelAWakeUpDetected
    )
{
    FrSM_ConditionWUReasonType result = FRSM_NO_WU_BY_BUS;

    boolean channelAWakeUpDetected = FALSE;
    boolean channelBWakeUpDetected = FALSE;

    if((pClstCfg->StartupOptions&FRSM_OPTIONS_CHECK_WAKEUP_REASON) != 0U)
    {
#if (FRSM_FRTRCV_CONTROL_ENABLE == STD_ON)
        const boolean trcvAPresent =
            ((pClstCfg->StartupOptions&FRSM_OPTIONS_TRCV_CHANNEL_A) != 0U) ? TRUE : FALSE;
        const boolean trcvBPresent =
            ((pClstCfg->StartupOptions&FRSM_OPTIONS_TRCV_CHANNEL_B) != 0U) ? TRUE : FALSE;

        /* call FrIf_GetTransceiverWUReason() for each transceiver, if present */
        if (trcvAPresent)
        {
            /* check channel A */
            FrTrcv_TrcvWUReasonType trcvChannelAWUReason = FRTRCV_WU_NOT_SUPPORTED;

            (void)FrIf_GetTransceiverWUReason(frIfCtrlIdx, FR_CHANNEL_A, &trcvChannelAWUReason); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

            if (trcvChannelAWUReason == FRTRCV_WU_BY_BUS)
            {
                channelAWakeUpDetected = TRUE;
            }
        }

        if (trcvBPresent)
        {
            /* check channel B */
            FrTrcv_TrcvWUReasonType trcvChannelBWUReason = FRTRCV_WU_NOT_SUPPORTED;

            (void)FrIf_GetTransceiverWUReason(frIfCtrlIdx, FR_CHANNEL_B, &trcvChannelBWUReason); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

            if (trcvChannelBWUReason == FRTRCV_WU_BY_BUS)
            {
                channelBWakeUpDetected = TRUE;
            }
        }
#endif /* FRSM_FRTRCV_CONTROL_ENABLE == STD_ON */

        /* call FrIf_GetWakeupRxStatus() */
        {
            uint8 wakeupRxStatus = 0x0U;

            (void) FrIf_GetWakeupRxStatus(frIfCtrlIdx, &wakeupRxStatus); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

            /* bit masks according to FrIf_GetWakeupRxStatus() specification */
            if ((wakeupRxStatus&0x01U) != 0U)
            {
                channelAWakeUpDetected = TRUE;
            }
            if ((wakeupRxStatus&0x02U) != 0U)
            {
                channelBWakeUpDetected = TRUE;
            }
        }

        {
            const boolean isDualChannelNode =
                ((pClstCfg->StartupOptions&FRSM_OPTIONS_IS_DUAL_CHANNEL_NODE) != 0U) ? TRUE : FALSE;

            if (
                (isDualChannelNode && (channelAWakeUpDetected && channelBWakeUpDetected))
                ||
                ((!isDualChannelNode) && (channelAWakeUpDetected || channelBWakeUpDetected))
               )
            {
                result = FRSM_ALL_WU_BY_BUS;
            }
            else if (channelAWakeUpDetected || channelBWakeUpDetected)
            {
                result = FRSM_PARTIAL_WU_BY_BUS;
            }
            else
            {
                result = FRSM_NO_WU_BY_BUS;
            }
        }
    }

    *pChannelAWakeUpDetected = channelAWakeUpDetected;

    return result;
}

#define FRSM_STOP_SEC_CODE
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

/*======================[Global Function Definitions]=========================*/

/* start code section declaration */
#define FRSM_START_SEC_CODE
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

/**
 * \brief   Initializes the FrSM according to the post-build-time configuration
 *          passed as argument.
 *
 * \param   FrSM_ConfigPtr (in)     Address of module post-build-time configuration
 *
 */
FUNC(void,FRSM_CODE) FrSM_Init
    (
        P2CONST(FrSM_ConfigType,AUTOMATIC,FRSM_APPL_CONST) FrSM_ConfigPtr
    )
{
    DBG_FRSM_INIT_ENTRY(FrSM_ConfigPtr);

/* check if development error detection is enabled */
#if (FRSM_DEV_ERROR_DETECT == STD_ON)

    /* check that configuration pointer is valid */
    if(FrSM_ConfigPtr == NULL_PTR)
    {
        /* Report to DET */
        FRSM_DET_REPORTERROR(FRSM_E_NULL_PTR, FRSM_SID_INIT);
    }
    else
#endif /* FRSM_DEV_ERROR_DETECT */
    {
#if (FRSM_COMM_INDICATION_ENABLE == STD_ON)
#if (FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_OFF)
        FrSM_NoCommunication = COMM_NO_COMMUNICATION;
        FrSM_FullCommunication = COMM_FULL_COMMUNICATION;
#endif /* FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_OFF */
#endif /* FRSM_COMM_INDICATION_ENABLE == STD_ON */

#if (FRSM_SETECUPASSIVE_ENABLE == STD_ON)

    DBG_FRSM_PASSIVESTATE((FrSM_PassiveState),(FALSE));
        FrSM_PassiveState = FALSE;
#endif /* FRSM_SETECUPASSIVE_ENABLE == STD_ON */

        /* save configuration pointer */
        FrSM_GlobalConfigPtr = FrSM_ConfigPtr;  /* !LINKSTO FrSm.ASR40.FrSm127, 1 */

        {

#if (FRSM_SINGLE_CLST_OPT_ENABLE == STD_OFF)

            uint8_least iClst;

            for(iClst = 0U; iClst < FrSM_GlobalConfigPtr->NumClst; iClst++)
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
            {

#if (FRSM_SINGLE_CLST_OPT_ENABLE == STD_OFF)
                const uint8 FrSM_ClstIdx = (uint8)iClst;
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */

                /* initialize FlexRay controllers */
                (void)FrIf_ControllerInit(FrSM_GlobalConfigPtr->aClst[FRSM_CLSTIDX].FrIfCtrlIdx);  /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

                FrSM_ClstRuntimeData[FRSM_CLSTIDX].T1Counter = 0U;
                FrSM_ClstRuntimeData[FRSM_CLSTIDX].T2Counter = 0U;
                FrSM_ClstRuntimeData[FRSM_CLSTIDX].T3Counter = 0U;
                FrSM_ClstRuntimeData[FRSM_CLSTIDX].StartupCounter = 0U;
                FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupCounter = 0U;
                DBG_FRSM_STATE_GRP(FRIF_CLSTIDX, (FrSM_ClstRuntimeData[FRSM_CLSTIDX].State), (FRSM_STATE_READY));
                FrSM_ClstRuntimeData[FRSM_CLSTIDX].State = FRSM_STATE_READY;
                DBG_FRSM_REQUESTEDSTATE_GRP(FRIF_CLSTIDX, (FrSM_ClstRuntimeData[FRSM_CLSTIDX].RequestedState), (FRSM_STATE_READY));
                FrSM_ClstRuntimeData[FRSM_CLSTIDX].RequestedState = FRSM_STATE_READY;
                FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupType = FRSM_NO_WAKEUP;
                FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupTransmitted = FALSE;
                FrSM_ClstRuntimeData[FRSM_CLSTIDX].PartialWakeupOnChannelA = FALSE;

#if (FRSM_REPORT_TO_BSMW_ENABLE == STD_ON)
                /* report state 'READY' to BSW Mode Manager */
                BswM_FrSM_CurrentState(FrSM_GlobalConfigPtr->aClst[FRSM_CLSTIDX].NetworkHandleId,
                                       FrSM_Int2BswMStateMapping[FRSM_TRCV_STATE_IDX][FRSM_STATE_READY]);
#endif /* (FRSM_REPORT_TO_BSMW_ENABLE == STD_ON) */
            }

        }

        /* module was successfully initialized - set marker */

        DBG_FRSM_INITSTATUS((FrSM_InitStatus),(FRSM_INIT));
        FrSM_InitStatus = FRSM_INIT;
    }

    DBG_FRSM_INIT_EXIT(FrSM_ConfigPtr);
}

/**
 * \brief   Requests a ComMode for a particular network cluster.
 *
 * \param   NetworkHandle (in)    Handle that identifies the cluster
 * \param   ComM_Mode (in)        Com mode to be requested
 *
 */
/* Deviation MISRAC2012-1 */
FUNC(Std_ReturnType,FRSM_CODE) FrSM_RequestComMode
    (
        NetworkHandleType NetworkHandle,
        ComM_ModeType ComM_Mode
    )
{
    Std_ReturnType retVal = E_NOT_OK;

#if (FRSM_SINGLE_CLST_OPT_ENABLE == STD_OFF)

    /* first of all translate the NetworkHandle to a ClstIdx */
    const uint8 FrSM_ClstIdx = FrSM_LookupClstIdx(NetworkHandle);

#else /* FRSM_SINGLE_CLST_OPT_ENABLE */
    TS_PARAM_UNUSED(NetworkHandle);
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */

    DBG_FRSM_REQUESTCOMMODE_ENTRY(NetworkHandle,ComM_Mode);

/* check if development error detection is enabled */
#if (FRSM_DEV_ERROR_DETECT == STD_ON)

    /* check for successfully initialized module */
    if (FrSM_InitStatus == FRSM_UNINIT)
    {
        /* Report to DET and return Error in case module was not initialized before */
        FRSM_DET_REPORTERROR(FRSM_E_UNINIT, FRSM_SID_REQUESTCOMMODE);
    }
    /* check for a valid HandleId */
    else if((FRSM_CLSTIDX >= FrSM_GlobalConfigPtr->NumClst) ||
            (NetworkHandle != FrSM_GlobalConfigPtr->aClst[FRSM_CLSTIDX].NetworkHandleId))
    {
        /* Report to DET */
        FRSM_DET_REPORTERROR(FRSM_E_INV_HANDLE, FRSM_SID_REQUESTCOMMODE);
    }
    /* check ComM_Mode for being valid */
    else if(((ComM_Mode != COMM_NO_COMMUNICATION) &&
             (ComM_Mode != COMM_FULL_COMMUNICATION)) &&
             (ComM_Mode != COMM_SILENT_COMMUNICATION))
    {
        /* Report to DET */
        FRSM_DET_REPORTERROR(FRSM_E_INV_MODE, FRSM_SID_REQUESTCOMMODE);
    }
    else
#endif /* FRSM_DEV_ERROR_DETECT */
    if(ComM_Mode == COMM_SILENT_COMMUNICATION)
    {
        /* silently ignore request according to EB requirement FrSm.EB.FrSm141 */
        retVal = E_OK;
    }
    else
    {
        {
            /* variable to hold the new requested state */
            uint8 FrSM_NewRequestedState;
            boolean myLock = FALSE;

            CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg =
                &FrSM_GlobalConfigPtr->aClst[FRSM_CLSTIDX];

            /* evaluate the new internal state requested */
            if(ComM_Mode == COMM_FULL_COMMUNICATION)
            {
                /* go to 'ONLINE' if communication was requested */
                FrSM_NewRequestedState = FRSM_STATE_ONLINE;
            }
            else
            {
                /* go to 'READY' if no communication was requested */
                FrSM_NewRequestedState = FRSM_STATE_READY;
            }

            /* enter critical area */
            SchM_Enter_FrSM_SCHM_FRSM_EXCLUSIVE_AREA_0();

            DBG_FRSM_REQUESTEDSTATE_GRP(FRIF_CLSTIDX, (FrSM_ClstRuntimeData[FRSM_CLSTIDX].RequestedState), (FrSM_NewRequestedState));
            FrSM_ClstRuntimeData[FRSM_CLSTIDX].RequestedState = FrSM_NewRequestedState;

            if (FrSM_NewRequestedState == FRSM_STATE_READY)
            {
                /* lock state machine */
                if((FrSM_ClstRuntimeData[FRSM_CLSTIDX].preemptionData&FRSM_STATEMACHINE_LOCK) != 0U)
                {
                  FrSM_ClstRuntimeData[FRSM_CLSTIDX].preemptionData |= FRSM_STATEMACHINE_REQUESTCOMMODE_SUSPENDED;
                }
                else
                {
                  FrSM_ClstRuntimeData[FRSM_CLSTIDX].preemptionData |= FRSM_STATEMACHINE_LOCK;
                  myLock = TRUE;
                }
            }

            /* leave critical area */
            SchM_Exit_FrSM_SCHM_FRSM_EXCLUSIVE_AREA_0();

            if(myLock)
            {
                boolean mainSuspended = FALSE;

                FrSM_RequestComMode_NoCommunication
                (
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
                          FRSM_CLSTIDX,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
                          pClstCfg
                );

                /* enter critical area */
                SchM_Enter_FrSM_SCHM_FRSM_EXCLUSIVE_AREA_0();

                mainSuspended = ((FrSM_ClstRuntimeData[FRSM_CLSTIDX].preemptionData&FRSM_STATEMACHINE_MAINFUNCTION_SUSPENDED) != 0U) ? TRUE : FALSE;
                FrSM_ClstRuntimeData[FRSM_CLSTIDX].preemptionData = FRSM_STATEMACHINE_RESET;

                /* leave critical area */
                SchM_Exit_FrSM_SCHM_FRSM_EXCLUSIVE_AREA_0();

                if(mainSuspended)
                {
                  FrSM_MainFunctionInternal(
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
                          FRSM_CLSTIDX,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
                      FrSM_NewRequestedState);
                }
            }
        }

        /* function execution was successful */
        retVal = E_OK;
    }


#if (FRSM_DEV_ERROR_DETECT == STD_OFF) && (FRSM_SINGLE_CLST_OPT_ENABLE == STD_ON) && \
    (FRSM_COMM_INDICATION_ENABLE == STD_OFF) && (FRSM_REPORT_TO_BSMW_ENABLE == STD_OFF)
    TS_PARAM_UNUSED(NetworkHandle);
#endif /* ... */


    DBG_FRSM_REQUESTCOMMODE_EXIT(retVal,NetworkHandle,ComM_Mode);
    return retVal;
}

/**
 * \brief   Returns the current ComMode of a particular network cluster.
 *
 * \param   NetworkHandle (in)    Handle that identifies the cluster
 * \param   ComM_ModePtr (out)     Address the Com_Mode is written to
 *
 */
/* Deviation MISRAC2012-1 */
FUNC(Std_ReturnType,FRSM_CODE) FrSM_GetCurrentComMode
    (
        NetworkHandleType NetworkHandle,
        P2VAR(ComM_ModeType,AUTOMATIC,FRSM_APPL_DATA) ComM_ModePtr
    )
{
    Std_ReturnType retVal = E_NOT_OK;

#if (FRSM_SINGLE_CLST_OPT_ENABLE == STD_OFF)

    /* first of all translate the NetworkHandle to a ClstIdx */
    const uint8 FrSM_ClstIdx = FrSM_LookupClstIdx(NetworkHandle);

#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */

    DBG_FRSM_GETCURRENTCOMMODE_ENTRY(NetworkHandle,ComM_ModePtr);

/* check if development error detection is enabled */
#if (FRSM_DEV_ERROR_DETECT == STD_ON)

    if (FrSM_InitStatus == FRSM_UNINIT)
    {
        /* Report to DET and return Error in case module was not initialized before */
        FRSM_DET_REPORTERROR(FRSM_E_UNINIT, FRSM_SID_GETCURRENTCOMMODE);
    }
    /* check for a valid NetworkHandleId */
    else if((FRSM_CLSTIDX >= FrSM_GlobalConfigPtr->NumClst) ||
            (NetworkHandle != FrSM_GlobalConfigPtr->aClst[FRSM_CLSTIDX].NetworkHandleId))
    {
        /* Report to DET */
        FRSM_DET_REPORTERROR(FRSM_E_INV_HANDLE, FRSM_SID_GETCURRENTCOMMODE);
    }
    /* check pointer for being valid */
    else if(ComM_ModePtr == NULL_PTR)
    {
        /* Report to DET */
        FRSM_DET_REPORTERROR(FRSM_E_NULL_PTR, FRSM_SID_GETCURRENTCOMMODE);
    }
    else
#endif /* FRSM_DEV_ERROR_DETECT */
    {
        /* translate the internal state to the external communication mode */
        uint8 internalState;

        TS_AtomicAssign8(internalState, FrSM_ClstRuntimeData[FRSM_CLSTIDX].State);
        *ComM_ModePtr = FrSM_Int2ExtStateMapping[internalState];

        retVal = E_OK;
    }

/* check if single cluster optimization is enabled - then we don't need the NetworkHandle */
#if ((FRSM_DEV_ERROR_DETECT == STD_OFF) && (FRSM_SINGLE_CLST_OPT_ENABLE == STD_ON))
    TS_PARAM_UNUSED(NetworkHandle);
#endif /* (FRSM_DEV_ERROR_DETECT == STD_OFF) && (FRSM_SINGLE_CLST_OPT_ENABLE == STD_ON) */


    DBG_FRSM_GETCURRENTCOMMODE_EXIT(retVal,NetworkHandle,ComM_ModePtr);
    return retVal;
}

STATIC FUNC(void,FRSM_CODE) FrSM_RequestComMode_NoCommunication
    (
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg
    )
{
    boolean triggerT09ab = FALSE;

    /* check for trigger T09ab */
    /* we are currently in POC: normal-active and a halt was requested */
    if (FrSM_ClstRuntimeData[FRSM_CLSTIDX].State == FRSM_STATE_ONLINE)
    {
        triggerT09ab = TRUE;
    }
#if (FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON)
    else if (FrSM_ClstRuntimeData[FRSM_CLSTIDX].State == FRSM_STATE_KEYSLOT_ONLY)
    {
        triggerT09ab = TRUE;
    }
    else
    {
        /* do nothing */
    }
#endif /* FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON */

    if (triggerT09ab)
    {
        /* perform functionality 'FE_STOP_FRIF' */
        (void)FrIf_SetState(pClstCfg->FrIfClstIdx,FRIF_GOTO_OFFLINE); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

        /* perform functionality 'FE_HALT' */
        (void)FrIf_HaltCommunication(pClstCfg->FrIfCtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

#if (FRSM_COMM_INDICATION_ENABLE == STD_ON)
        /* perform functionality 'FE_NO_COM_IND' */

#if (FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_OFF)
        FRSM_CALL_COMM_BUSSM_MODEINDICATION(pClstCfg->NetworkHandleId,&FrSM_NoCommunication);
#else
        /* !LINKSTO FrSM.EB.SchM_Call_ComM_BusSM_ModeIndication, 1 */
        if (FRSM_CALL_COMM_BUSSM_MODEINDICATION(pClstCfg->FrIfClstIdx,pClstCfg->NetworkHandleId,COMM_NO_COMMUNICATION) != SCHM_E_OK)
        {
#if (FRSM_DEV_ERROR_DETECT == STD_ON)
          /* !LINKSTO FrSM.EB.SchM_Call_ComM_BusSM_ModeIndication_DET, 1 */
            FRSM_DET_REPORTERROR_WITH_INSTANCE(
                                                FRSM_E_MODEINDICATION,
                                                FRSM_SID_MAINFUNCTION,
                                                pClstCfg->NetworkHandleId
                                              );
#endif /* FRSM_DEV_ERROR_DETECT == STD_ON */
        }
#endif /* FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_OFF */

#endif /* FRSM_COMM_INDICATION_ENABLE */

        /* Set new state */
        DBG_FRSM_STATE_GRP(FRIF_CLSTIDX, (FrSM_ClstRuntimeData[FRSM_CLSTIDX].State), (FRSM_STATE_HALT_REQ));
        TS_AtomicAssign8(FrSM_ClstRuntimeData[FRSM_CLSTIDX].State, FRSM_STATE_HALT_REQ);

#if (FRSM_REPORT_TO_BSMW_ENABLE == STD_ON)
        /* report state 'HALT_REQUEST' to BSW Mode Manager */
        BswM_FrSM_CurrentState(pClstCfg->NetworkHandleId,
                               FrSM_Int2BswMStateMapping[FRSM_TRCV_STATE_IDX][FRSM_STATE_HALT_REQ]);
#endif /* (FRSM_REPORT_TO_BSMW_ENABLE == STD_ON) */
    }
    /* check for trigger T14 */
    else if(FrSM_ClstRuntimeData[FRSM_CLSTIDX].State == FRSM_STATE_ONLINE_PASSIVE)
    {
        /* perform functionality 'FE_DEM_SYNC_LOSS_PASSED' */
        FrSM_FE_DEM_SYNC_LOSS_PASSED(pClstCfg);

        /* perform functionality 'FE_HALT' */
        (void)FrIf_HaltCommunication(pClstCfg->FrIfCtrlIdx);  /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

        /* perform functionality 'cancel t3' */
        FrSM_ClstRuntimeData[FRSM_CLSTIDX].T3Counter = 0U;

        /* Set new state */
        DBG_FRSM_STATE_GRP(FRIF_CLSTIDX, (FrSM_ClstRuntimeData[FRSM_CLSTIDX].State), (FRSM_STATE_HALT_REQ));
        TS_AtomicAssign8(FrSM_ClstRuntimeData[FRSM_CLSTIDX].State, FRSM_STATE_HALT_REQ);


#if (FRSM_REPORT_TO_BSMW_ENABLE == STD_ON)
        /* report state 'HALT_REQUEST' to BSW Mode Manager */
        BswM_FrSM_CurrentState(pClstCfg->NetworkHandleId,
                               FrSM_Int2BswMStateMapping[FRSM_TRCV_STATE_IDX][FRSM_STATE_HALT_REQ]);
#endif /* (FRSM_REPORT_TO_BSMW_ENABLE == STD_ON) */
    }
    else
    {
        /* execute other transitions via FrSM_MainFunction_#() */
    }
}

/* only if transceiver shall be controlled */
#if (FRSM_FRTRCV_CONTROL_ENABLE == STD_ON)

STATIC FUNC(void,FRSM_CODE) FrSM_FE_TRCV_STANDBY(const uint8 FrIf_CtrlIdx, const uint8 Options);
STATIC FUNC(void,FRSM_CODE) FrSM_FE_TRCV_STANDBY(const uint8 FrIf_CtrlIdx, const uint8 Options)
{
    /* check if a channel A transceiver is configured */

    if((Options&FRSM_OPTIONS_TRCV_CHANNEL_A) != 0x00U)
    {
        /* perform operations as described in SWS */
        (void)FrIf_SetTransceiverMode(FrIf_CtrlIdx,FR_CHANNEL_A,FRTRCV_TRCVMODE_STANDBY); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
    }

    /* check if a channel B transceiver is configured */
    if((Options&FRSM_OPTIONS_TRCV_CHANNEL_B) != 0x00U)
    {
        /* perform operations as described in SWS */
        (void)FrIf_SetTransceiverMode(FrIf_CtrlIdx,FR_CHANNEL_B,FRTRCV_TRCVMODE_STANDBY); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
    }
}

STATIC FUNC(void,FRSM_CODE) FrSM_FE_TRCV_NORMAL(const uint8 FrIf_CtrlIdx,
                                                const uint8 Options);

STATIC FUNC(void,FRSM_CODE) FrSM_FE_TRCV_NORMAL(const uint8 FrIf_CtrlIdx,
                                                const uint8 Options)
{
  FrTrcv_TrcvModeType newTrcvMode = FRTRCV_TRCVMODE_NORMAL;

#if (FRSM_SETECUPASSIVE_ENABLE == STD_ON)
  if (FrSM_PassiveState == TRUE)
  {
    newTrcvMode = FRTRCV_TRCVMODE_RECEIVEONLY;
  }
#endif /* FRSM_SETECUPASSIVE_ENABLE == STD_ON */

  /* check if a channel A transceiver is configured */
  if((Options&FRSM_OPTIONS_TRCV_CHANNEL_A) != 0x00U)
  {
      /* perform operations as described in SWS */
      (void)FrIf_SetTransceiverMode(FrIf_CtrlIdx, FR_CHANNEL_A, newTrcvMode); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
      (void)FrIf_ClearTransceiverWakeup(FrIf_CtrlIdx, FR_CHANNEL_A); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
  }

  /* check if a channel B transceiver is configured */
  if((Options&FRSM_OPTIONS_TRCV_CHANNEL_B) != 0x00U)
  {
      /* perform operations as described in SWS */
      (void)FrIf_SetTransceiverMode(FrIf_CtrlIdx,FR_CHANNEL_B,newTrcvMode); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
      (void)FrIf_ClearTransceiverWakeup(FrIf_CtrlIdx,FR_CHANNEL_B); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
  }
}

#endif /* FRSM_FRTRCV_CONTROL_ENABLE */

STATIC FUNC(uint8,FRSM_CODE) FrSM_HandleState_FRSM_STATE_HALT_REQ
    (
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        CONSTP2VAR(Fr_POCStatusType, AUTOMATIC, AUTOMATIC) pocStatus,
        const uint8 FrIf_CtrlIdx
    );

/**
 * \brief Sub-function handling state FRSM_STATE_HALT_REQ
 *
 * \return New state of internal state machine
 */
STATIC FUNC(uint8,FRSM_CODE) FrSM_HandleState_FRSM_STATE_HALT_REQ
    (
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        CONSTP2VAR(Fr_POCStatusType, AUTOMATIC, AUTOMATIC) pocStatus,
        const uint8 FrIf_CtrlIdx
    )
{
    uint8 newState = FRSM_STATE_HALT_REQ;

    /* check trigger for T11 */
    if(
         (pocStatus->State == FR_POCSTATE_HALT)
         ||
         (pocStatus->Freeze)
      )
    {
/* only if transceiver shall be controlled */
#if (FRSM_FRTRCV_CONTROL_ENABLE == STD_ON)

        /* perform functionality 'FE_TRCV_STANDBY' */
        FrSM_FE_TRCV_STANDBY(FrIf_CtrlIdx, pClstCfg->StartupOptions);

#endif /* FRSM_FRTRCV_CONTROL_ENABLE */

        /* perform functionality 'FE_CONFIG' */
        (void)FrIf_ControllerInit(FrIf_CtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

        /* switch to new state */
        newState = FRSM_STATE_READY;
    }

#if (FRSM_FRTRCV_CONTROL_ENABLE == STD_OFF)
    TS_PARAM_UNUSED(pClstCfg);
#endif /* FRSM_FRTRCV_CONTROL_ENABLE == STD_OFF */

    return newState;
}

STATIC FUNC(uint8,FRSM_CODE) FrSM_HandleState_FRSM_STATE_READY
    (
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        const uint8 FrIf_CtrlIdx,
        const uint8 requestedState
    );

/**
 * \brief Sub-function handling state FRSM_STATE_READY
 *
 * \param[in] requestedState FRSM_STATE_READY or FRSM_STATE_ONLINE
 *
 * \return New state of internal state machine
 */
STATIC FUNC(uint8,FRSM_CODE) FrSM_HandleState_FRSM_STATE_READY
    (
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        const uint8 FrIf_CtrlIdx,
        const uint8 requestedState
    )
{
    uint8 newState = FRSM_STATE_READY;

    if(requestedState == FRSM_STATE_ONLINE)
    {
        /* initialize to avoid warning */
        FrSM_ConditionWUReasonType conditionWUReasonType = FRSM_NO_WU_BY_BUS;

        /* initialize to avoid warning */
        boolean channelAWakeUpDetected = FALSE;

        const boolean isWakeupEcu =
                ((pClstCfg->StartupOptions&FRSM_OPTIONS_IS_WAKEUP_ECU) != 0U) ? TRUE : FALSE;

        if(isWakeupEcu)
        {
            conditionWUReasonType =
                    FrSM_ConditionWUReason(pClstCfg, FrIf_CtrlIdx, &channelAWakeUpDetected);
        }

/* only if transceiver shall be controlled */
#if (FRSM_FRTRCV_CONTROL_ENABLE == STD_ON)
        /* perform functionality 'FE_TRCV_NORMAL' */
        FrSM_FE_TRCV_NORMAL(FrIf_CtrlIdx, pClstCfg->StartupOptions);
#endif /* FRSM_FRTRCV_CONTROL_ENABLE */

        /* perform functionality 'startupCounter := 1' */
        FrSM_ClstRuntimeData[FRSM_CLSTIDX].StartupCounter = 1U;

        if(isWakeupEcu)
        {
            if(conditionWUReasonType != FRSM_ALL_WU_BY_BUS)
            {
                /* execute T01 (abc) */

                if((pClstCfg->StartupOptions&FRSM_OPTIONS_IS_DUAL_CHANNEL_NODE) != 0U)
                {
                    if(conditionWUReasonType == FRSM_NO_WU_BY_BUS)
                    {
                        /* execute TO1 (b) */
                        FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupType = FRSM_DUAL_CHANNEL_WAKEUP;
                        /* perform functionality 'FE_SET_WU_CHANNEL_INITIAL' */
                        (void)FrIf_SetWakeupChannel(FrIf_CtrlIdx, FR_CHANNEL_A); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
                    }
                    else /* (conditionWUReasonType == FRSM_PARTIAL_WU_BY_BUS) holds */
                    {
                        /* execute T01 (c) */
                        FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupType = FRSM_DUAL_CHANNEL_ECHO_WAKEUP;

                        /* record on which channel wakeup was detected and
                         * perform functionality 'FE_SET_WU_CHANNEL_ECHO' */
                        if (channelAWakeUpDetected)
                        {
                            FrSM_ClstRuntimeData[FRSM_CLSTIDX].PartialWakeupOnChannelA = TRUE;
                            (void)FrIf_SetWakeupChannel(FrIf_CtrlIdx, FR_CHANNEL_A); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
                        }
                        else
                        {
                            FrSM_ClstRuntimeData[FRSM_CLSTIDX].PartialWakeupOnChannelA = FALSE;
                            (void)FrIf_SetWakeupChannel(FrIf_CtrlIdx, FR_CHANNEL_B); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
                        }
                    }
                }
                else /* execute T01 (a) */
                {
                    FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupType = FRSM_SINGLE_CHANNEL_WAKEUP;
                    /* start t1 */
                    FrSM_ClstRuntimeData[FRSM_CLSTIDX].T1Counter = pClstCfg->T1CounterStart;
                }

                FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupTransmitted = FALSE;

                /* perform functionality 'FE_WAKEUP' */
                (void)FrIf_SendWUP(FrIf_CtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

                /* start t3 */
                FrSM_ClstRuntimeData[FRSM_CLSTIDX].T3Counter = pClstCfg->T3CounterStart;

                /* switch to new state */
                newState = FRSM_STATE_WAKEUP;
            }
        }

        if((!isWakeupEcu) || (conditionWUReasonType == FRSM_ALL_WU_BY_BUS))
        {
            /* execute T02 (ab) */
            boolean do_FE_ALLOW_COLDSTART = FALSE;

            FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupType = FRSM_NO_WAKEUP;

            /* perform functionality 'FE_START' */
            (void)FrIf_StartCommunication(FrIf_CtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

            if ((pClstCfg->StartupOptions&FRSM_OPTIONS_DELAY_STARTUP_WO_WAKEUP) == 0U)
            {
                /* execute T02 (a) */
                do_FE_ALLOW_COLDSTART = TRUE;
            }
            else /* execute T02 (b) */
            {
                if(pClstCfg->T1CounterStart == 0U)
                {
                    /* according to FrSm142 */
                    do_FE_ALLOW_COLDSTART = TRUE;
                }
                else
                {
                    /* start timer t1 */
                    FrSM_ClstRuntimeData[FRSM_CLSTIDX].T1Counter =
                            pClstCfg->T1CounterStart;
                }
            }

            if (do_FE_ALLOW_COLDSTART)
            {
                /* perform functionality 'FE_ALLOW_COLDSTART' */
                if((pClstCfg->StartupOptions&FRSM_OPTIONS_IS_COLDSTART_ECU) != 0x00U)
                {
                    (void)FrIf_AllowColdstart(FrIf_CtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
                }
            }

            /* perform functionality 'start t2', 'start t3' */
            FrSM_ClstRuntimeData[FRSM_CLSTIDX].T2Counter = pClstCfg->T2CounterStart;
            FrSM_ClstRuntimeData[FRSM_CLSTIDX].T3Counter = pClstCfg->T3CounterStart;

            /* switch to new state */
            newState = FRSM_STATE_STARTUP;
        }
    }

    return newState;
}

STATIC FUNC(uint8,FRSM_CODE) FrSM_HandleState_FRSM_STATE_WAKEUP
    (
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        CONSTP2VAR(Fr_POCStatusType, AUTOMATIC, AUTOMATIC) pocStatus,
        const uint8 FrIf_CtrlIdx,
        const uint8 requestedState
    );

/**
 * \brief Sub-function handling state FRSM_STATE_WAKEUP
 *
 * \param[in] requestedState FRSM_STATE_READY or FRSM_STATE_ONLINE
 *
 * \return New state of internal state machine
 */
STATIC FUNC(uint8,FRSM_CODE) FrSM_HandleState_FRSM_STATE_WAKEUP
    (
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        CONSTP2VAR(Fr_POCStatusType, AUTOMATIC, AUTOMATIC) pocStatus,
        const uint8 FrIf_CtrlIdx,
        const uint8 requestedState
    )
{
    uint8 newState = FRSM_STATE_WAKEUP;

    /* check if a 'lower' state is requested */
    /* check trigger for T13 */
    if(requestedState == FRSM_STATE_READY)
    {
        /* perform functionality 'FE_DEM_SYNC_LOSS_PASSED' */
        FrSM_FE_DEM_SYNC_LOSS_PASSED(pClstCfg);

/* only if transceiver shall be controlled */
#if (FRSM_FRTRCV_CONTROL_ENABLE == STD_ON)

        /* perform functionality 'FE_TRCV_STANDBY' */
        FrSM_FE_TRCV_STANDBY(FrIf_CtrlIdx, pClstCfg->StartupOptions);

#endif /* FRSM_FRTRCV_CONTROL_ENABLE */

        /* perform functionality 'FE_CONFIG' */
        (void)FrIf_ControllerInit(FrIf_CtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

        /* perform functionality 'cancel t1 and t3' */
        FrSM_ClstRuntimeData[FRSM_CLSTIDX].T1Counter = 0U;
        FrSM_ClstRuntimeData[FRSM_CLSTIDX].T3Counter = 0U;

        /* switch to new state */
        newState = FRSM_STATE_READY;
    }
    /* if applicable: check T103, perform wakeup pattern transmission, then check T03 (abcde) */
    else
    {
        boolean doT03 = FALSE;

        /* on error, exit wakeup state */
        if (
             (pocStatus->Freeze) || (pocStatus->State == FR_POCSTATE_HALT)
           )
        {
            doT03 = TRUE;
        }
        /* wait until wakeup is finished */
        else if (pocStatus->State == FR_POCSTATE_READY)
        {
            /* check T103 */
            if (FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupType == FRSM_DUAL_CHANNEL_ECHO_WAKEUP)
            {
                /* T103 */
                FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupType = FRSM_DUAL_CHANNEL_WAKEUP_FORWARD;

                /* perform functionality 'FE_SET_WU_CHANNEL_FORWARD' */
                {
                    if (FrSM_ClstRuntimeData[FRSM_CLSTIDX].PartialWakeupOnChannelA)
                    {
                        (void)FrIf_SetWakeupChannel(FrIf_CtrlIdx, FR_CHANNEL_B); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
                    }
                    else
                    {
                        (void)FrIf_SetWakeupChannel(FrIf_CtrlIdx, FR_CHANNEL_A); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
                    }
                }

                /* perform functionality 'FE_WAKEUP' */
                (void)FrIf_SendWUP(FrIf_CtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
            }
            else
            /* wakeup pattern transmission according to FrSm183
             * (unlike SWS pseudo-code, implementation uses a do-while-style)
             * (if WakeupCounter exceeds NumWakeupPatterns afterwards, T03 (abcde) will be
             * executed */
            {
                boolean busTrafficDetected = FALSE;

                switch (pocStatus->WakeupStatus)
                {
                    case FR_WAKEUP_RECEIVED_HEADER:
                    case FR_WAKEUP_RECEIVED_WUP:
                        busTrafficDetected = TRUE;
                        break;
                    case FR_WAKEUP_TRANSMITTED:
                        FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupTransmitted = TRUE;
                        break;
                    default:
                        /* do nothing */
                        break;
                }

                if(
                   (!busTrafficDetected)
                   &&
                   (FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupCounter < pClstCfg->NumWakeupPatterns)
                  )
                {
                    /* perform functionality 'FE_WAKEUP' */
                    (void)FrIf_SendWUP(FrIf_CtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
                }
                else
                {
                    /* wakeup finished */
                    doT03 = TRUE;
                }

                /* increment wakeupCounter */
                FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupCounter++;
            }
        }
        else
        {
          /* wait for wakeup to finish */
        }

        if (doT03)
        {
            newState = FrSM_DoTransition_T03
                (
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
                    FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
                    pClstCfg,
                    FrIf_CtrlIdx
                );
        }
    }

    return newState;
}


STATIC FUNC(uint8,FRSM_CODE) FrSM_HandleState_FRSM_STATE_STARTUP
    (
/* if single cluster optimization is disabled, then an runtime index must be used */
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        CONSTP2VAR(Fr_POCStatusType, AUTOMATIC, AUTOMATIC) pocStatus,
        const uint8 FrIf_CtrlIdx,
        const uint8 requestedState,
        const boolean t1Elapsed,
        const boolean t2Elapsed
    );

/**
 * \brief Sub-function handling state FRSM_STATE_STARTUP
 *
 * \param[in] requestedState FRSM_STATE_READY or FRSM_STATE_ONLINE
 * \param[in] t1Elapsed TRUE if timer t1 expires in this main function invocation, FALSE otherwise
 * \param[in] t2Elapsed TRUE if timer t2 expires in this main function invocation, FALSE otherwise
 *
 * \return New state of internal state machine
 */
STATIC FUNC(uint8,FRSM_CODE) FrSM_HandleState_FRSM_STATE_STARTUP
    (
/* if single cluster optimization is disabled, then an runtime index must be used */
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        CONSTP2VAR(Fr_POCStatusType, AUTOMATIC, AUTOMATIC) pocStatus,
        const uint8 FrIf_CtrlIdx,
        const uint8 requestedState,
        const boolean t1Elapsed,
        const boolean t2Elapsed
    )
{
    /* variable that holds the next state */
    uint8 NewState = FRSM_STATE_STARTUP;

    const uint16 startupCounter = FrSM_ClstRuntimeData[FRSM_CLSTIDX].StartupCounter;

    if(requestedState == FRSM_STATE_READY)
    {
        /* T12 */

        /* perform functionalities 'cancel t1', 'cancel t2', 'cancel t3' */
        FrSM_ClstRuntimeData[FRSM_CLSTIDX].T1Counter = 0U;
        FrSM_ClstRuntimeData[FRSM_CLSTIDX].T2Counter = 0U;
        FrSM_ClstRuntimeData[FRSM_CLSTIDX].T3Counter = 0U;

        /* perform functionality 'FE_DEM_SYNC_LOSS_PASSED' */
        FrSM_FE_DEM_SYNC_LOSS_PASSED(pClstCfg);

#if (FRSM_FRTRCV_CONTROL_ENABLE == STD_ON)
        /* perform functionality 'FE_TRCV_STANDBY' */
        FrSM_FE_TRCV_STANDBY(FrIf_CtrlIdx, pClstCfg->StartupOptions);
#endif /* FRSM_FRTRCV_CONTROL_ENABLE */

        /* perform functionality 'FE_CONFIG' */
        (void)FrIf_ControllerInit(FrIf_CtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

        NewState = FRSM_STATE_READY;
    }
    else if(
             (pocStatus->State == FR_POCSTATE_NORMAL_ACTIVE)
             &&
             (!pocStatus->Freeze)
           )
    {
        /* T08 or T108 */
#if (FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON)
        boolean triggerT08 = (pocStatus->SlotMode == FR_SLOTMODE_ALL) ? TRUE : FALSE;
#endif /* FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON */

        /* perform functionalities 'cancel t1', 'cancel t2' */
        FrSM_ClstRuntimeData[FRSM_CLSTIDX].T1Counter = 0U;
        FrSM_ClstRuntimeData[FRSM_CLSTIDX].T2Counter = 0U;

        /* perform functionality 'FE_START_FRIF' */
        (void)FrIf_SetState(pClstCfg->FrIfClstIdx,FRIF_GOTO_ONLINE); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

        /* perform functionality 'FE_DEM_STATUS_PASSED' */
#if (FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP == TS_PROD_ERR_REP_TO_DEM)
        if (pClstCfg->ClusterStartupDemEventId != FRSM_INVALID_DEM_EVENTID)
        {
            Dem_ReportErrorStatus(pClstCfg->ClusterStartupDemEventId, DEM_EVENT_STATUS_PASSED);
        }
#endif /* FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP */

        /* perform functionality 'FE_DEM_SYNC_LOSS_PASSED' */
        FrSM_FE_DEM_SYNC_LOSS_PASSED(pClstCfg);

#if (FRSM_COMM_INDICATION_ENABLE == STD_ON)
#if (FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON)
        if(triggerT08)
#endif /* FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON */
        {
            /* perform functionality 'FE_FULL_COM_IND' */
#if (FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_OFF)
            FRSM_CALL_COMM_BUSSM_MODEINDICATION(pClstCfg->NetworkHandleId,&FrSM_FullCommunication);
#else
            /* !LINKSTO FrSM.EB.SchM_Call_ComM_BusSM_ModeIndication, 1 */
            if (FRSM_CALL_COMM_BUSSM_MODEINDICATION(pClstCfg->FrIfClstIdx,pClstCfg->NetworkHandleId,COMM_FULL_COMMUNICATION) != SCHM_E_OK)
            {
#if (FRSM_DEV_ERROR_DETECT == STD_ON)
              /* !LINKSTO FrSM.EB.SchM_Call_ComM_BusSM_ModeIndication_DET, 1 */
                FRSM_DET_REPORTERROR_WITH_INSTANCE(
                                                    FRSM_E_MODEINDICATION,
                                                    FRSM_SID_MAINFUNCTION,
                                                    pClstCfg->NetworkHandleId
                                                  );
#endif /* FRSM_DEV_ERROR_DETECT == STD_ON */
            }
#endif /* FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_OFF */

        }
#endif /* FRSM_COMM_INDICATION_ENABLE */

        /* perform functionality 'cancel t3' */
        FrSM_ClstRuntimeData[FRSM_CLSTIDX].T3Counter = 0U;

#if (FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON)
        if (triggerT08)
#endif /* FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON */
        {
            NewState = FRSM_STATE_ONLINE;
        }
#if (FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON)
        else /* T108 triggered */
        {
            NewState = FRSM_STATE_KEYSLOT_ONLY;
        }
#endif /* FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON */
    }
    else
    {
        /* check triggers for T04[ab], T05, T06 */

        boolean doT05 = FALSE;
        boolean doT06 = FALSE;

        if(t2Elapsed)
        {
            /* check triggers for T05, T06 */

            if(
                (startupCounter <= pClstCfg->StartupRepetitionsWithWakeup)
                &&
                (FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupType != FRSM_NO_WAKEUP)
                )
            {
                /* T05 */
                doT05 = TRUE;

#if (FRSM_FRTRCV_CONTROL_ENABLE == STD_ON)
                /* perform functionality 'FE_TRCV_NORMAL' */
                FrSM_FE_TRCV_NORMAL(FrIf_CtrlIdx, pClstCfg->StartupOptions);
#endif /* FRSM_FRTRCV_CONTROL_ENABLE */

                /* perform functionality 'FE_CONFIG' */
                (void)FrIf_ControllerInit(FrIf_CtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

                /* perform functionality 'FE_WAKEUP' */
                (void)FrIf_SendWUP(FrIf_CtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

                /* perform functionality 'startupCounter := startupCounter + 1',
                 * using saturating counter to implement value "infinite" of config parameters */
                if(startupCounter < FRSM_REPETITIONS_INFINITE_VALUE)
                {
                    FrSM_ClstRuntimeData[FRSM_CLSTIDX].StartupCounter = startupCounter + 1U;
                }

                NewState = FRSM_STATE_WAKEUP;
            }
            else if(startupCounter <= pClstCfg->StartupRepetitions)
            {
                /* T06 */
                doT06 = TRUE;

#if (FRSM_FRTRCV_CONTROL_ENABLE == STD_ON)
                /* perform functionality 'FE_TRCV_NORMAL' */
                FrSM_FE_TRCV_NORMAL(FrIf_CtrlIdx, pClstCfg->StartupOptions);
#endif /* FRSM_FRTRCV_CONTROL_ENABLE */

                /* perform functionality 'FE_CONFIG' */
                (void)FrIf_ControllerInit(FrIf_CtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

                /* perform functionality 'FE_START' */
                (void)FrIf_StartCommunication(FrIf_CtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

                /* perform functionality 'FE_ALLOW_COLDSTART' */
                if((pClstCfg->StartupOptions&FRSM_OPTIONS_IS_COLDSTART_ECU) != 0U)
                {
                    (void)FrIf_AllowColdstart(FrIf_CtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
                }

                /* perform functionality 'startupCounter := startupCounter + 1',
                 * using saturating counter to implement value "infinite" of config parameters */
                if (startupCounter < FRSM_REPETITIONS_INFINITE_VALUE)
                {
                    FrSM_ClstRuntimeData[FRSM_CLSTIDX].StartupCounter = startupCounter + 1U;
                }

                /* perform functionality 'start t2' */
                FrSM_ClstRuntimeData[FRSM_CLSTIDX].T2Counter = pClstCfg->T2CounterStart;

                /* remain in state */
            }
            else
            {
                /* do nothing */
            }
        }

        if (
             (
               (!doT05) && (!doT06)
             )
             &&
             (
               (pocStatus->State != FR_POCSTATE_NORMAL_ACTIVE)
               &&
               (!pocStatus->Freeze)
             )
           )
        {
            const boolean triggerT04a = t1Elapsed;
            boolean triggerT04b = FALSE;

            /* check trigger for T04b */
            if(FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupType == FRSM_DUAL_CHANNEL_WAKEUP)
            {
                /* check SWS condition AllChannelsAwake:
                 * Wakeup received on channel B?
                 * (This node sent a wakeup pattern on channel A)
                 */
                uint8 wakeupRxStatus = 0x0U;
                (void) FrIf_GetWakeupRxStatus(FrIf_CtrlIdx, &wakeupRxStatus); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
                /* bit mask for channel B according to FrIf_GetWakeupRxStatus() specification */
                if ((wakeupRxStatus&0x02U) != 0U)
                {
                    triggerT04b = TRUE;
                }
            }

            if(triggerT04a || triggerT04b)
            {
                /* perform functionality 'FE_ALLOW_COLDSTART' */
                if((pClstCfg->StartupOptions&FRSM_OPTIONS_IS_COLDSTART_ECU) != 0U)
                {
                    (void)FrIf_AllowColdstart(FrIf_CtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */
                }
            }
        }
    }

    return NewState;
}

STATIC FUNC(uint8,FRSM_CODE) FrSM_HandleState_FRSM_STATE_ONLINE_PASSIVE
    (
/* if single cluster optimization is disabled, then an runtime index must be used */
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        CONSTP2VAR(Fr_POCStatusType, AUTOMATIC, AUTOMATIC) pocStatus,
        const uint8 FrIf_CtrlIdx
    );

/**
 * \brief Sub-function handling state FRSM_STATE_ONLINE_PASSIVE
 *
 * \return New state of internal state machine
 */
STATIC FUNC(uint8,FRSM_CODE) FrSM_HandleState_FRSM_STATE_ONLINE_PASSIVE
    (
/* if single cluster optimization is disabled, then an runtime index must be used */
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        CONSTP2VAR(Fr_POCStatusType, AUTOMATIC, AUTOMATIC) pocStatus,
        const uint8 FrIf_CtrlIdx
    )
{
    /* variable that holds the next state */
    uint8 NewState = FRSM_STATE_ONLINE_PASSIVE;

    if(
        (pocStatus->State == FR_POCSTATE_NORMAL_ACTIVE)
        &&
        (!pocStatus->Freeze)
      )
    {
        /* execute T15 or T115 */

        /* perform functionality 'FE_DEM_SYNC_LOSS_PASSED' */
        FrSM_FE_DEM_SYNC_LOSS_PASSED(pClstCfg);

        /* perform functionality 'FE_START_FRIF' */
        (void)FrIf_SetState(pClstCfg->FrIfClstIdx,FRIF_GOTO_ONLINE); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

        /* cancel t3 */
        FrSM_ClstRuntimeData[FRSM_CLSTIDX].T3Counter = 0U;

#if (FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON)
        if(pocStatus->SlotMode == FR_SLOTMODE_ALL)
#endif /* FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON */
        {
            /* execute T15 */

#if (FRSM_COMM_INDICATION_ENABLE == STD_ON)
            /* perform functionality 'FE_FULL_COM_IND' */
#if (FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_OFF)
            FRSM_CALL_COMM_BUSSM_MODEINDICATION(pClstCfg->NetworkHandleId,&FrSM_FullCommunication);
#else
            /* !LINKSTO FrSM.EB.SchM_Call_ComM_BusSM_ModeIndication, 1 */
            if (FRSM_CALL_COMM_BUSSM_MODEINDICATION(pClstCfg->FrIfClstIdx,pClstCfg->NetworkHandleId,COMM_FULL_COMMUNICATION) != SCHM_E_OK)
            {
#if (FRSM_DEV_ERROR_DETECT == STD_ON)
              /* !LINKSTO FrSM.EB.SchM_Call_ComM_BusSM_ModeIndication_DET, 1 */
                FRSM_DET_REPORTERROR_WITH_INSTANCE(
                                                    FRSM_E_MODEINDICATION,
                                                    FRSM_SID_MAINFUNCTION,
                                                    pClstCfg->NetworkHandleId
                                                  );
#endif /* FRSM_DEV_ERROR_DETECT == STD_ON */
            }
#endif /* FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_OFF */
#endif /* FRSM_COMM_INDICATION_ENABLE */

            NewState = FRSM_STATE_ONLINE;
        }
#if (FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON)
        else
        {
            /* execute T115 */
            NewState = FRSM_STATE_KEYSLOT_ONLY;
        }
#endif /* FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON */
    }
    else if(
             (pocStatus->State == FR_POCSTATE_HALT)
             ||
             (pocStatus->Freeze)
           )
    {
        if  (
              ((pClstCfg->StartupOptions&FRSM_OPTIONS_CHECK_WAKEUP_REASON) != 0U)
              ||
              ((pClstCfg->StartupOptions&FRSM_OPTIONS_IS_WAKEUP_ECU) == 0U)
            )
        {
            /* execute T17 */

            /* perform functionality 'FE_CONFIG' */
            (void)FrIf_ControllerInit(FrIf_CtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

            FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupType = FRSM_NO_WAKEUP;

            /* perform functionality 'FE_START' */
            (void)FrIf_StartCommunication(FrIf_CtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

            FrSM_ClstRuntimeData[FRSM_CLSTIDX].StartupCounter = 1U;

            /* perform functionality 'start t2' */
            FrSM_ClstRuntimeData[FRSM_CLSTIDX].T2Counter = pClstCfg->T2CounterStart;

            NewState = FRSM_STATE_STARTUP;
        }
        else
        {
            /* execute T20c */

            FRSM_INVARIANT_ASSERT(
                FrSM_ClstRuntimeData[FRSM_CLSTIDX].RequestedState == FRSM_STATE_ONLINE,
                FRSM_SID_MAINFUNCTION);

            FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupType = FRSM_SINGLE_CHANNEL_WAKEUP;

            /* perform functionality 'FE_CONFIG' */
            (void)FrIf_ControllerInit(FrIf_CtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

            /* perform functionality 'FE_WAKEUP' */
            (void)FrIf_SendWUP(FrIf_CtrlIdx); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

            FrSM_ClstRuntimeData[FRSM_CLSTIDX].StartupCounter = 1U;

            /* start timer t1, t3 */
            FrSM_ClstRuntimeData[FRSM_CLSTIDX].T1Counter = pClstCfg->T1CounterStart;
            FrSM_ClstRuntimeData[FRSM_CLSTIDX].T3Counter = pClstCfg->T3CounterStart;

            NewState = FRSM_STATE_WAKEUP;
        }
    }
    else
    {
        /* do nothing */
    }

    return NewState;
}


STATIC FUNC(uint8,FRSM_CODE) FrSM_HandleState_FRSM_STATE_ONLINE
    (
/* if single cluster optimization is disabled, then an runtime index must be used */
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        CONSTP2VAR(Fr_POCStatusType, AUTOMATIC, AUTOMATIC) pocStatus,
        const uint8 FrIf_CtrlIdx
    );

/**
 * \brief Sub-function handling state FRSM_STATE_ONLINE
 *
 * \return New state of internal state machine
 */
STATIC FUNC(uint8,FRSM_CODE) FrSM_HandleState_FRSM_STATE_ONLINE
    (
/* if single cluster optimization is disabled, then an runtime index must be used */
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        CONSTP2VAR(Fr_POCStatusType, AUTOMATIC, AUTOMATIC) pocStatus,
        const uint8 FrIf_CtrlIdx
    )
{
    /* variable that holds the next state */
    uint8 NewState = FRSM_STATE_ONLINE;

    /* FRSM_KEYSLOT_ONLY and FRSM_ONLINE have similar transitions T10[ab], T16[ab], T20[ab] */
    NewState = FrSM_HandleTransitions_T10ab_T16ab_T20ab
        (
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
            FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
            pClstCfg,
            NewState,
            FrIf_CtrlIdx,
            pocStatus
        );

    return NewState;
}


#if (FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON)
STATIC FUNC(uint8,FRSM_CODE) FrSM_HandleState_FRSM_STATE_KEYSLOT_ONLY
    (
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        CONSTP2VAR(Fr_POCStatusType, AUTOMATIC, AUTOMATIC) pocStatus,
        const uint8 FrIf_CtrlIdx
    );

/**
 * \brief Sub-function handling state FRSM_STATE_KEYSLOT_ONLY
 *
 * \return New state of internal state machine
 */
STATIC FUNC(uint8,FRSM_CODE) FrSM_HandleState_FRSM_STATE_KEYSLOT_ONLY
    (
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg,
        CONSTP2VAR(Fr_POCStatusType, AUTOMATIC, AUTOMATIC) pocStatus,
        const uint8 FrIf_CtrlIdx
    )
{
    /* variable that holds the next state */
    uint8 NewState = FRSM_STATE_KEYSLOT_ONLY;

    /* check T101 */
    if (
         (!pocStatus->Freeze)
         &&
         (
           (pocStatus->State == FR_POCSTATE_NORMAL_ACTIVE)
           &&
           (pocStatus->SlotMode == FR_SLOTMODE_ALL)
         )
       )
    {
#if (FRSM_COMM_INDICATION_ENABLE == STD_ON)
        /* perform functionality 'FE_FULL_COM_IND' */
#if (FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_OFF)
        FRSM_CALL_COMM_BUSSM_MODEINDICATION(pClstCfg->NetworkHandleId,&FrSM_FullCommunication);
#else
        /* !LINKSTO FrSM.EB.SchM_Call_ComM_BusSM_ModeIndication, 1 */
        if (FRSM_CALL_COMM_BUSSM_MODEINDICATION(pClstCfg->FrIfClstIdx,pClstCfg->NetworkHandleId,COMM_FULL_COMMUNICATION) != SCHM_E_OK)
        {
#if (FRSM_DEV_ERROR_DETECT == STD_ON)
          /* !LINKSTO FrSM.EB.SchM_Call_ComM_BusSM_ModeIndication_DET, 1 */
            FRSM_DET_REPORTERROR_WITH_INSTANCE(
                                                FRSM_E_MODEINDICATION,
                                                FRSM_SID_MAINFUNCTION,
                                                pClstCfg->NetworkHandleId
                                              );
#endif /* FRSM_DEV_ERROR_DETECT == STD_ON */
        }
#endif /* FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_OFF */
#endif /* FRSM_COMM_INDICATION_ENABLE */

        NewState = FRSM_STATE_ONLINE;
    }
    else
    {
        /* FRSM_KEYSLOT_ONLY and FRSM_ONLINE have similar transitions T10[ab], T16[ab], T20[ab] */
        NewState = FrSM_HandleTransitions_T10ab_T16ab_T20ab(
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
            FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
            pClstCfg,
            FRSM_STATE_KEYSLOT_ONLY,
            FrIf_CtrlIdx,
            pocStatus);
    }

    return NewState;
}
#endif /* FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON */


/**
 * \brief Implements functionality of main function.
 *
 * \param[in] FrSM_ClstIdx Index to access cluster-specific runtime information (index is omitted
 * if single cluster optimization is enabled)
 */
STATIC FUNC(void,FRSM_CODE) FrSM_MainFunctionInternal
    (
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        const uint8 FrSM_ClstIdx,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        uint8 RequestedState
    )
{
    /* get current state into local variable */
    const uint8 State = FrSM_ClstRuntimeData[FRSM_CLSTIDX].State;

    /* obtain pointer to cluster specific configuration structure */
    CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg =
        &FrSM_GlobalConfigPtr->aClst[FRSM_CLSTIDX];

    /* get requested state into local variable */
    const uint8 FrIf_CtrlIdx = pClstCfg->FrIfCtrlIdx;

    /* create a variable that holds the newly evaluated state */
    uint8 NewState = State;

    /* variable holding elapsed status of timer t2 */
    boolean t2Elapsed = FALSE;

    /* variable holding elapsed status of timer t1 */
    boolean t1Elapsed = FALSE;

    /* elapsed status of timer t3 */
    boolean t3Elapsed = FALSE;

    Fr_POCStatusType pocStatus;

    /* get POC Status from FrIf */
    const Std_ReturnType RetCode =
      FrIf_GetPOCStatus(FrIf_CtrlIdx,&pocStatus); /* !LINKSTO FrSm.ASR40.FrSm149, 1 */

    if(RetCode != E_OK)
    {
      pocStatus.Freeze = TRUE;
    }

    FrSM_Tick(
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
        FRSM_CLSTIDX,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        pClstCfg,
        &t1Elapsed,
        &t2Elapsed,
        &t3Elapsed);

    /* dispatch function which handles state */
    switch(State)
    {
        case FRSM_STATE_HALT_REQ:
        {
            NewState = FrSM_HandleState_FRSM_STATE_HALT_REQ(
                pClstCfg,
                &pocStatus,
                FrIf_CtrlIdx);
            break;
        }
        case FRSM_STATE_READY:
        {
            NewState = FrSM_HandleState_FRSM_STATE_READY(
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
                FRSM_CLSTIDX,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
                pClstCfg,
                FrIf_CtrlIdx,
                RequestedState);
            break;
        }
        case FRSM_STATE_WAKEUP:
        {
            NewState = FrSM_HandleState_FRSM_STATE_WAKEUP(
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
                    FRSM_CLSTIDX,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
                    pClstCfg,
                    &pocStatus,
                    FrIf_CtrlIdx,
                    RequestedState);
            break;
        }
        case FRSM_STATE_STARTUP:
        {
            NewState = FrSM_HandleState_FRSM_STATE_STARTUP(
/* if single cluster optimization is disabled, then an runtime index must be used */
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
                FRSM_CLSTIDX,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
                pClstCfg,
                &pocStatus,
                FrIf_CtrlIdx,
                RequestedState,
                t1Elapsed,
                t2Elapsed);
            break;
        }
        case FRSM_STATE_ONLINE_PASSIVE:
        {
            NewState = FrSM_HandleState_FRSM_STATE_ONLINE_PASSIVE(
/* if single cluster optimization is disabled, then an runtime index must be used */
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
                FRSM_CLSTIDX,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
                pClstCfg,
                &pocStatus,
                FrIf_CtrlIdx);
            break;
        }
        case FRSM_STATE_ONLINE:
        {
            NewState = FrSM_HandleState_FRSM_STATE_ONLINE(
/* if single cluster optimization is disabled, then an runtime index must be used */
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
                FRSM_CLSTIDX,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
                pClstCfg,
                &pocStatus,
                FrIf_CtrlIdx);
            break;
        }
#if (FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON)
        case FRSM_STATE_KEYSLOT_ONLY:
        {
            NewState = FrSM_HandleState_FRSM_STATE_KEYSLOT_ONLY(
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
                FRSM_CLSTIDX,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
                pClstCfg,
                &pocStatus,
                FrIf_CtrlIdx);
            break;
        }
#endif /* FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON */
        /* CHECK: NOPARSE */
        default:
        {
            FRSM_UNREACHABLE_CODE_ASSERT(FRSM_SID_MAINFUNCTION);
            break;
        }
        /* CHECK: PARSE */
    }

    /* write back new state to global variable */
    DBG_FRSM_STATE_GRP(FRIF_CLSTIDX, (FrSM_ClstRuntimeData[FRSM_CLSTIDX].State), (NewState));
    TS_AtomicAssign8(FrSM_ClstRuntimeData[FRSM_CLSTIDX].State, NewState);

    /* set wakeupCounter on entering state FRSM_WAKEUP */
    if ((State != FRSM_STATE_WAKEUP) && (NewState == FRSM_STATE_WAKEUP))
    {
        FrSM_ClstRuntimeData[FRSM_CLSTIDX].WakeupCounter = 1U;
    }

#if (FRSM_REPORT_TO_BSMW_ENABLE == STD_ON)
    /* Check new state against old state - if the state changed report the new state to BswM */
    if (State != NewState)
    {
        BswM_FrSM_CurrentState(pClstCfg->NetworkHandleId,
                               FrSM_Int2BswMStateMapping[FRSM_TRCV_STATE_IDX][NewState]);
    }
#endif /* (FRSM_REPORT_TO_BSMW_ENABLE == STD_ON) */

    /* if the new state is either FRSM_STARTUP, FRSM_WAKEUP, or FRSM_ONLINE_PASSIVE
     * check for transitions T30, T31, T32, T33 */
    if ((NewState == FRSM_STATE_STARTUP) || (NewState == FRSM_STATE_WAKEUP) ||
        (NewState == FRSM_STATE_ONLINE_PASSIVE))
    {
        FrSM_HandleTransitions_T30_T31_T32_T33(
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
            FRSM_CLSTIDX,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
            pClstCfg,
            t3Elapsed
        );
    }
}




/**
 * \brief Implements precondition checks of FrSM main function. Calls FrSM_MainFunctionInternal()
 * to execute functionality.
 */
FUNC(void,FRSM_CODE) FrSM_MainFunction
    (
        const uint8 FrSM_ClstIdx
    )
{

    /* FrSM_MainFunction shall not be executed if module has not been initialized */

    DBG_FRSM_MAINFUNCTION_ENTRY(FrSM_ClstIdx);
    if (FrSM_InitStatus == FRSM_INIT)
    {
        boolean myLock = FALSE;
        boolean requestComModeSuspended = FALSE;
        uint8 RequestedState;

        /* enter critical area */
        SchM_Enter_FrSM_SCHM_FRSM_EXCLUSIVE_AREA_0();

        /* get requested state into local variable */
        RequestedState = FrSM_ClstRuntimeData[FRSM_CLSTIDX].RequestedState;

        /* lock state machine */
        if((FrSM_ClstRuntimeData[FRSM_CLSTIDX].preemptionData&FRSM_STATEMACHINE_LOCK) != 0U)
        {
          FrSM_ClstRuntimeData[FRSM_CLSTIDX].preemptionData |= FRSM_STATEMACHINE_MAINFUNCTION_SUSPENDED;
        }
        else
        {
          FrSM_ClstRuntimeData[FRSM_CLSTIDX].preemptionData |= FRSM_STATEMACHINE_LOCK;
          myLock = TRUE;
        }

        /* leave critical area */
        SchM_Exit_FrSM_SCHM_FRSM_EXCLUSIVE_AREA_0();

        if(myLock)
        {
            FrSM_MainFunctionInternal(
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
                FRSM_CLSTIDX,
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
                RequestedState
            );

            /* enter critical area */
            SchM_Enter_FrSM_SCHM_FRSM_EXCLUSIVE_AREA_0();

            requestComModeSuspended = ((FrSM_ClstRuntimeData[FRSM_CLSTIDX].preemptionData&FRSM_STATEMACHINE_REQUESTCOMMODE_SUSPENDED) != 0U) ? TRUE : FALSE;
            FrSM_ClstRuntimeData[FRSM_CLSTIDX].preemptionData = FRSM_STATEMACHINE_RESET;

            /* leave critical area */
            SchM_Exit_FrSM_SCHM_FRSM_EXCLUSIVE_AREA_0();

            if(requestComModeSuspended)
            {
              CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg =
                  &FrSM_GlobalConfigPtr->aClst[FRSM_CLSTIDX];

              FrSM_RequestComMode_NoCommunication
              (
    #if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON)
                        FRSM_CLSTIDX,
    #endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
                        pClstCfg
              );
            }
        }
    }

#if (FRSM_SINGLE_CLST_OPT_ENABLE == STD_ON)
    TS_PARAM_UNUSED(FrSM_ClstIdx);
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */


    DBG_FRSM_MAINFUNCTION_EXIT(FrSM_ClstIdx);
}

/* only if version info API is enabled */
#if (FRSM_VERSION_INFO_API == STD_ON)

/**
 * \brief   Returns the module version information.
 *
 * \param versioninfo (out)      Address the version information should be written to.
 *
 */
FUNC(void,FRSM_CODE) FrSM_GetVersionInfo
    (
        P2VAR(Std_VersionInfoType,AUTOMATIC,FRSM_APPL_DATA) versioninfo
    )
{
    DBG_FRSM_GETVERSIONINFO_ENTRY(versioninfo);

/* check if development error detection is enabled */
#if (FRSM_DEV_ERROR_DETECT == STD_ON)

    /* check if controller index is within supported bounds */
    if(versioninfo == NULL_PTR)
    {
        /* Report to DET */
        FRSM_DET_REPORTERROR(FRSM_E_NULL_PTR, FRSM_SID_GETVERSIONINFO);
    }
    else
#endif  /* FRSM_DEV_ERROR_DETECT */
    {
        /* assign logistic values to structure members */
        versioninfo->vendorID = FRSM_VENDOR_ID;
        versioninfo->moduleID = FRSM_MODULE_ID;
        versioninfo->sw_major_version = FRSM_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = FRSM_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = FRSM_SW_PATCH_VERSION;
    }

    DBG_FRSM_GETVERSIONINFO_EXIT(versioninfo);
}

#endif /* FRSM_VERSION_INFO_API */

/* only if transceiver shall be controlled and service is enabled */
#if ( (FRSM_FRTRCV_CONTROL_ENABLE == STD_ON) && (FRSM_SETECUPASSIVE_ENABLE == STD_ON) )

FUNC(Std_ReturnType,FRSM_CODE) FrSM_SetEcuPassive
(
    boolean FrSM_Passive
)
{
    Std_ReturnType retVal = E_NOT_OK;
#if (FRSM_SINGLE_CLST_OPT_ENABLE == STD_OFF)
    uint8_least iClst; /* cluster loop variable */
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */

    DBG_FRSM_SETECUPASSIVE_ENTRY(FrSM_Passive);

#if (FRSM_DEV_ERROR_DETECT == STD_ON)

    /* check for successfully initialized module */
    if (FrSM_InitStatus == FRSM_UNINIT)
    {
        /* Report to DET and return Error in case module was not initialized before */
        FRSM_DET_REPORTERROR(FRSM_E_UNINIT, FRSM_SID_SETECUPASSIVE);
    }
    else
#endif /* FRSM_DEV_ERROR_DETECT == STD_ON */
    {
        /* set transceiver state */

        DBG_FRSM_PASSIVESTATE((FrSM_PassiveState),(FrSM_Passive));
        FrSM_PassiveState = FrSM_Passive;

#if (FRSM_SINGLE_CLST_OPT_ENABLE == STD_OFF)
        for( iClst = 0U ; iClst < FrSM_GlobalConfigPtr->NumClst ; iClst++ )
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */
        {
#if (FRSM_SINGLE_CLST_OPT_ENABLE == STD_OFF)
            const uint8 FrSM_ClstIdx = (uint8)iClst;
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */

            /* obtain pointer to cluster specific configuration structure */
            CONSTP2CONST(FrSM_ClstCfgType,AUTOMATIC,FRSM_APPL_CONST) pClstCfg =
                &FrSM_GlobalConfigPtr->aClst[FRSM_CLSTIDX];

            const uint8 state = FrSM_ClstRuntimeData[pClstCfg->FrIfClstIdx].State;

            /* execute FE_TRCV_NORMAL if cluster is not in state FRSM_READY */
            if ( state != FRSM_STATE_READY)
            {
                FrSM_FE_TRCV_NORMAL( pClstCfg->FrIfCtrlIdx,
                                    pClstCfg->StartupOptions);
            }

#if (FRSM_REPORT_TO_BSMW_ENABLE == STD_ON)
        /* report state to BswM */
          BswM_FrSM_CurrentState(pClstCfg->NetworkHandleId,
                               FrSM_Int2BswMStateMapping[FRSM_TRCV_STATE_IDX][state]);
#endif /* FRSM_REPORT_TO_BSMW_ENABLE == STD_ON */
        }

        retVal = E_OK;
    }


    DBG_FRSM_SETECUPASSIVE_EXIT(retVal,FrSM_Passive);
    return retVal;
}
#endif /* ( (FRSM_FRTRCV_CONTROL_ENABLE == STD_ON) && (FRSM_SETECUPASSIVE_ENABLE == STD_ON) ) */


#if (FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON)
FUNC(Std_ReturnType, FRSM_CODE) FrSM_AllSlots(
  NetworkHandleType NetworkHandle)
{
    Std_ReturnType retVal = E_NOT_OK;

#if (FRSM_SINGLE_CLST_OPT_ENABLE == STD_OFF)
    const uint8 FrSM_ClstIdx = FrSM_LookupClstIdx(NetworkHandle);
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE == STD_OFF */

    DBG_FRSM_ALLSLOTS_ENTRY(NetworkHandle);

#if (FRSM_DEV_ERROR_DETECT == STD_ON)
    if (FrSM_InitStatus == FRSM_UNINIT)
    {
        FRSM_DET_REPORTERROR(FRSM_E_UNINIT, FRSM_SID_ALLSLOTS);
    }
    /* check for a valid HandleId */
    else if(
             (FRSM_CLSTIDX >= FrSM_GlobalConfigPtr->NumClst)
             ||
             (NetworkHandle != FrSM_GlobalConfigPtr->aClst[FRSM_CLSTIDX].NetworkHandleId)
           )
    {
        FRSM_DET_REPORTERROR(FRSM_E_INV_HANDLE, FRSM_SID_ALLSLOTS);
    }
    else
#endif /* FRSM_DEV_ERROR_DETECT == STD_ON */
    {
        retVal = FrIf_AllSlots(FrSM_GlobalConfigPtr->aClst[FRSM_CLSTIDX].FrIfCtrlIdx);
    }

#if ( (FRSM_SINGLE_CLST_OPT_ENABLE == STD_ON) && (FRSM_DEV_ERROR_DETECT == STD_OFF) )
    TS_PARAM_UNUSED(NetworkHandle);
#endif /* (FRSM_SINGLE_CLST_OPT_ENABLE == STD_ON) && (FRSM_DEV_ERROR_DETECT == STD_OFF) */


    DBG_FRSM_ALLSLOTS_EXIT(retVal,NetworkHandle);
    return retVal;
}
#endif /* FRSM_KEYSLOTONLYMODE_ENABLE == STD_ON */


/* start code section declaration */
#define FRSM_STOP_SEC_CODE
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

