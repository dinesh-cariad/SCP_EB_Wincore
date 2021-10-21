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

#ifndef FRSM_INT_H
#define FRSM_INT_H

/*======================[Include Section]=====================================*/

#include <FrSM_Cfg.h>
#include <FrSM_CfgTypes.h>
#include <FrSM_guards.h> /* After External/API/Cfg, before Trace/internal/private */

/*======================[Macro check section]=================================*/


/* check for defined STD_ON macro value */
#ifndef STD_ON
#error STD_ON not defined
#endif  /* STD_ON */

/* check for defined STD_OFF macro value */
#ifndef STD_OFF
#error STD_OFF not defined
#endif  /* STD_OFF */


/* check for correctly defined switch FRSM_VERSION_INFO_API */
#ifndef FRSM_VERSION_INFO_API
#error macro FRSM_VERSION_INFO_API not defined
#endif /* FRSM_VERSION_INFO_API */

/* test for valid value of FRSM_VERSION_INFO_API */
#if (FRSM_VERSION_INFO_API != STD_ON) && \
    (FRSM_VERSION_INFO_API != STD_OFF)
#error macro FRSM_VERSION_INFO_API has an invalid value
#endif /* FRSM_VERSION_INFO_API */

/* check for correctly defined switch FRSM_DEV_ERROR_DETECT */
#ifndef FRSM_DEV_ERROR_DETECT
#error macro FRSM_DEV_ERROR_DETECT not defined
#endif /* FRSM_DEV_ERROR_DETECT */

/* test for valid value of FRSM_DEV_ERROR_DETECT */
#if (FRSM_DEV_ERROR_DETECT != STD_ON) && \
    (FRSM_DEV_ERROR_DETECT != STD_OFF)
#error macro FRSM_DEV_ERROR_DETECT has an invalid value
#endif /* FRSM_DEV_ERROR_DETECT */


/* test for valid macro definition of FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP */
#ifndef FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP
#error FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP not defined
#endif  /* FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP */

/* test for valid value of FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP */
#if (FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP != TS_PROD_ERR_REP_TO_DEM) && \
    (FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP != TS_PROD_ERR_REP_TO_DET) && \
    (FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP != TS_PROD_ERR_DISABLE)
#error macro FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP has an invalid value
#endif /* FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP */

/* test for valid macro definition of FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS */
#ifndef FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS
#error FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS not defined
#endif  /* FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS */

/* test for valid value of FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS */
#if (FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS != TS_PROD_ERR_REP_TO_DEM) && \
    (FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS != TS_PROD_ERR_REP_TO_DET) && \
    (FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS != TS_PROD_ERR_DISABLE)
#error macro FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS has an invalid value
#endif /* FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS */


/* test for valid macro definition of FRSM_COMM_INDICATION_ENABLE */
#ifndef FRSM_COMM_INDICATION_ENABLE
#error FRSM_COMM_INDICATION_ENABLE not defined
#endif  /* FRSM_COMM_INDICATION_ENABLE */

/* test for valid value of FRSM_COMM_INDICATION_ENABLE */
#if (FRSM_COMM_INDICATION_ENABLE != STD_ON) && \
    (FRSM_COMM_INDICATION_ENABLE != STD_OFF)
#error macro FRSM_COMM_INDICATION_ENABLE has an invalid value
#endif /* FRSM_COMM_INDICATION_ENABLE */


/* test for valid macro definition of FRSM_FRTRCV_CONTROL_ENABLE */
#ifndef FRSM_FRTRCV_CONTROL_ENABLE
#error FRSM_FRTRCV_CONTROL_ENABLE not defined
#endif  /* FRSM_FRTRCV_CONTROL_ENABLE */

/* test for valid value of FRSM_FRTRCV_CONTROL_ENABLE */
#if (FRSM_FRTRCV_CONTROL_ENABLE != STD_ON) && \
    (FRSM_FRTRCV_CONTROL_ENABLE != STD_OFF)
#error macro FRSM_FRTRCV_CONTROL_ENABLE has an invalid value
#endif /* FRSM_FRTRCV_CONTROL_ENABLE */


/* test for valid macro definition of FRSM_SINGLE_CLST_OPT_ENABLE */
#ifndef FRSM_SINGLE_CLST_OPT_ENABLE
#error FRSM_SINGLE_CLST_OPT_ENABLE not defined
#endif  /* FRSM_SINGLE_CLST_OPT_ENABLE */

/* test for valid value of FRSM_SINGLE_CLST_OPT_ENABLE */
#if (FRSM_SINGLE_CLST_OPT_ENABLE != STD_ON) && \
    (FRSM_SINGLE_CLST_OPT_ENABLE != STD_OFF)
#error macro FRSM_SINGLE_CLST_OPT_ENABLE has an invalid value
#endif /* FRSM_SINGLE_CLST_OPT_ENABLE */


/* test for valid value of FRSM_REPORT_TO_BSMW_ENABLE */
#if (FRSM_REPORT_TO_BSMW_ENABLE != STD_ON) && \
    (FRSM_REPORT_TO_BSMW_ENABLE != STD_OFF)
#error macro FRSM_REPORT_TO_BSMW_ENABLE has an invalid value
#endif /* FRSM_REPORT_TO_BSMW_ENABLE */


/* test for valid macro definition of FRSM_FRNM_STARTUPERROR_ENABLE */
#ifndef FRSM_FRNM_STARTUPERROR_ENABLE
#error FRSM_FRNM_STARTUPERROR_ENABLE not defined
#endif  /* FRSM_FRNM_STARTUPERROR_ENABLE */

/* test for valid value of FRSM_FRNM_STARTUPERROR_ENABLE */
#if (FRSM_FRNM_STARTUPERROR_ENABLE != STD_ON) && \
    (FRSM_FRNM_STARTUPERROR_ENABLE != STD_OFF)
#error macro FRSM_FRNM_STARTUPERROR_ENABLE has an invalid value
#endif /* FRSM_FRNM_STARTUPERROR_ENABLE */


/* test for valid macro definition of FRSM_KEYSLOTONLYMODE_ENABLE */
#ifndef FRSM_KEYSLOTONLYMODE_ENABLE
#error FRSM_KEYSLOTONLYMODE_ENABLE not defined
#endif  /* FRSM_KEYSLOTONLYMODE_ENABLE */

/* test for valid value of FRSM_KEYSLOTONLYMODE_ENABLE */
#if (FRSM_KEYSLOTONLYMODE_ENABLE != STD_ON) && \
    (FRSM_KEYSLOTONLYMODE_ENABLE != STD_OFF)
#error macro FRSM_KEYSLOTONLYMODE_ENABLE has an invalid value
#endif /* FRSM_KEYSLOTONLYMODE_ENABLE */

#if (defined FRSM_REPETITIONS_INFINITE_VALUE)
#error FRSM_REPETITIONS_INFINITE_VALUE is already defined
#endif /* defined FRSM_REPETITIONS_INFINITE_VALUE */

/** \brief If the FrSM_ClstCfgType members StartupRepetitions or StartupRepetitionsWithWakeup have
 * this value, they shall be treated as corresponding to positive infinity */
#define FRSM_REPETITIONS_INFINITE_VALUE 0xffffU


#if (defined FRSM_INVALID_DEM_EVENTID)
#error FRSM_INVALID_DEM_EVENTID is already defined
#endif /* defined FRSM_INVALID_DEM_EVENTID */

/** \brief 0 is not a valid Dem_EventIdType value */
#define FRSM_INVALID_DEM_EVENTID 0U

/*======================[Global Data Types]===================================*/

/** \brief Values of SWS state machine variable wakeupType */
typedef enum
{
    FRSM_SINGLE_CHANNEL_WAKEUP,
    FRSM_DUAL_CHANNEL_WAKEUP,
    FRSM_DUAL_CHANNEL_WAKEUP_FORWARD,
    FRSM_DUAL_CHANNEL_ECHO_WAKEUP,
    FRSM_NO_WAKEUP
} FrSM_WakeupTypeType;

typedef struct
{
    /** \brief internal counter representing timer T1 */
    uint16 T1Counter;

    /** \brief internal counter representing timer T2 */
    uint16 T2Counter;

    /** \brief internal counter representing timer t3 */
    uint16 T3Counter;

    /** \brief (saturating) startup counter */
    uint16 StartupCounter;

    /** \brief wakeup counter */
    uint8 WakeupCounter;

    /** \brief state of internal state machine */
    uint8 State;

    /** \brief Requested state of internal state machine
      *
      * Either FRSM_STATE_READY (corresponds to reqComMode = NoCom)
      * or FRSM_STATE_ONLINE (corresponds to reqComMode = FullCom) */
    uint8 RequestedState;

    /** \brief WS state machine variable wakeupType
      *
      * Values are of type FrSM_WakeupTypeType */
    uint8 WakeupType;

    /** \brief contains information of state machine lock and preemption state
      *  Bit 0: statemachine is currently processed (mutex)
      *  Bit 1: requestComMode_NoCommunication has been suspended
      *  Bit 2: mainFunction has been suspended
      */
    uint8 preemptionData;

    /** \brief SWS state machine variable wakeupTransmitted */
    boolean WakeupTransmitted;

    /** \brief Partial wakeup was detected on channel A.
      *
      * Only valid if WakeupType == FRSM_DUAL_CHANNEL_ECHO_WAKEUP */
    boolean PartialWakeupOnChannelA;
}FrSM_ClstRuntimeDataType;

/*======================[Global Data]=========================================*/

#define FRSM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

extern VAR(FrSM_ClstRuntimeDataType,FRSM_VAR_CLEARED) FrSM_ClstRuntimeData[];

#define FRSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

/*======================[Global Function Declarations]========================*/

/* start code section declaration */
#define FRSM_START_SEC_CODE
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

/**
 * \brief FrSM Main Function
 *
 * Main function that drives the statemachine. Executed per cluster.
 *
 * \param[in] FrSM_ClstIdx Index of FrSMCluster container according to document order
 *
 ** \ProductionError{FRSM_E_CLUSTER_STARTUP, thrown\, if Fr startup could not reach the state normal
                     active within the configured time.}
 ** \ProductionError{FRSM_E_CLUSTER_SYNC_LOSS, thrown\, if Fr cluster has lost its synchronization.}
 *
 * \ServiceID{0x80}
 * \Reentrancy{Non Re-entrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void,FRSM_CODE) FrSM_MainFunction
    (
        const uint8 FrSM_ClstIdx
    );

/* stop code section declaration */
#define FRSM_STOP_SEC_CODE
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */


#endif /* FRSM_INT_H */

/*
 * = eof ======================================================================
 */

