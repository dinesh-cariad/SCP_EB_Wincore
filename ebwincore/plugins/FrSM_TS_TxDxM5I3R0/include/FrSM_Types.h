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

#ifndef FRSM_TYPES_H
#define FRSM_TYPES_H

/*======================[Include Section]=====================================*/

#include <FrSM_Version.h>     /* version check macros */
#include <TSAutosar.h>        /* EB specific standard types */
#include <FrSM_Cfg.h>         /* pre compile time config */

#if (   (FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP == TS_PROD_ERR_REP_TO_DEM) \
     || (FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS == TS_PROD_ERR_REP_TO_DEM) )
#include <Dem.h>
#endif /* FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP == TS_PROD_ERR_REP_TO_DEM ... */

/*======================[Global Macros]=======================================*/

/*======================[Global Data Types]===================================*/

/** \brief Post-build configuration of a cluster */
typedef struct
{
#if (FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP == TS_PROD_ERR_REP_TO_DEM)
    const Dem_EventIdType ClusterStartupDemEventId;
    /**< FRSM_E_CLUSTER_STARTUP Dem Event ID */
#endif /* FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP == TS_PROD_ERR_REP_TO_DEM */

#if (FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS == TS_PROD_ERR_REP_TO_DEM)
    const Dem_EventIdType ClusterSyncLossDemEventId;
    /**< FRSM_E_CLUSTER_SYNC_LOSS Dem Event ID */
#endif /* FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS == TS_PROD_ERR_REP_TO_DEM */

    const uint16 T1CounterStart;
    /**< initial counter value for timeout T1 (in number of FrSM_MainFunction() invocations */

    const uint16 T2CounterStart;
    /**< initial counter value for timeout T2 (in number of FrSM_MainFunction() invocations */

    const uint16 T3CounterStart;
    /**< initial counter value for timer t3 (in number of FrSM_MainFunction() invocations */

    const uint16 StartupRepetitions;
    /**< Number of startup repetitions.
     *   The value FRSM_REPETITIONS_INFINITE_VALUE shall be interpreted as positive infinity. */

    const uint16 StartupRepetitionsWithWakeup;
    /**< Number of startup repetitions that are preceded by a wakeup symbol transmission.
     *   The value FRSM_REPETITIONS_INFINITE_VALUE shall be interpreted as positive infinity. */

    const uint8 NetworkHandleId;
    /**< Network handle Id associated to this config item - the network handle Id is passsed to FrSM to
     *   identify the cluster to control. */

    const uint8 FrIfClstIdx;
    /**< FrIf cluster index associated to this config item - the cluster index is used to identify the
     *   FrIf cluster to control via FrSM. */

    const uint8 FrIfCtrlIdx;
    /**< FrIf controller index associated to this config item. */

    const uint8 StartupOptions;
    /**< Bitcoded option field:
     *   Bit 0: Control transceivers on channel A
     *   Bit 1: Control transceivers on channel B
     *   Bit 2: IsColdstartECU
     *   Bit 3: IsWakeupECU
     *   Bit 4: CheckWakeupReason
     *   Bit 5: IsDualChannelNode
     *   Bit 6: DelayStartupWithoutWakeup */

    const uint8 NumWakeupPatterns;
    /**< Maximum number of Wakeup Patterns the node may send before going to FRSM_STARTUP */
}FrSM_ClstCfgType;


/* !LINKSTO FrSm.ASR40.FrSm_ConfigType, 1 */
/** \brief Post-build configuration structure passed to \a FrSM_Init() */
typedef struct
{
    const uint8 NumClst;
    /**< Number of FlexRay Clusters covered by this configuration */

    const FrSM_ClstCfgType aClst[1U];
    /**< List of FlexRay cluster configurations
     *   This list will be extended immediately behind the structure of type
     *   FrSM_ConfigType. Therefore we allocate the first array member within this
     *   structure.
     */
}FrSM_ConfigType;

/**
 * \brief State for BswM reporting
 *
 * This type defines the states that are reported to the BswM using BswM_FrSM_CurrentState.
 */
#define FRSM_BSWM_READY 0U
    /**< FrSM Cluster State: FRSM_READY, Passive State: FRSM_ECU_ACTIVE */
#define FRSM_BSWM_READY_ECU_PASSIVE 1U
    /**< FrSM Cluster State: FRSM_READY, Passive State: FRSM_ECU_PASSIVE */
#define FRSM_BSWM_STARTUP 2U
    /**< FrSM Cluster State: FRSM_STARTUP, Passive State: FRSM_ECU_ACTIVE */
#define FRSM_BSWM_STARTUP_ECU_PASSIVE 3U
    /**< FrSM Cluster State: FRSM_STARTUP, Passive State: FRSM_ECU_PASSIVE */
#define FRSM_BSWM_WAKEUP 4U
    /**< FrSM Cluster State: FRSM_WAKEUP, Passive State: FRSM_ECU_ACTIVE */
#define FRSM_BSWM_WAKEUP_ECU_PASSIVE 5U
    /**< FrSM Cluster State: FRSM_WAKEUP, Passive State: FRSM_ECU_PASSIVE */
#define FRSM_BSWM_HALT_REQ 6U
    /**< FrSM Cluster State: FRSM_HALT_REQUEST, Passive State: FRSM_ECU_ACTIVE */
#define FRSM_BSWM_HALT_REQ_ECU_PASSIVE 7U
    /**< FrSM Cluster State: FRSM_HALT_REQUEST, Passive State: FRSM_ECU_PASSIVE */
#define FRSM_BSWM_KEYSLOT_ONLY 8U
    /**< FrSM Cluster State: FRSM_KEYSLOT_ONLY, Passive State: FRSM_ECU_ACTIVE */
#define FRSM_BSWM_KEYSLOT_ONLY_ECU_PASSIVE 9U
    /**< FrSM Cluster State: FRSM_KEYSLOT_ONLY, Passive State: FRSM_ECU_PASSIVE */
#define FRSM_BSWM_ONLINE 10U
    /**< FrSM Cluster State: FRSM_ONLINE, Passive State: FRSM_ECU_ACTIVE */
#define FRSM_BSWM_ONLINE_ECU_PASSIVE 11U
    /**< FrSM Cluster State: FRSM_ONLINE, Passive State: FRSM_ECU_PASSIVE */
#define FRSM_BSWM_ONLINE_PASSIVE 12U
    /**< FrSM Cluster State: FRSM_ONLINE_PASSIVE, Passive State: FRSM_ECU_ACTIVE */
#define FRSM_BSWM_ONLINE_PASSIVE_ECU_PASSIVE 13U
    /**< FrSM Cluster State: FRSM_ONLINE_PASSIVE, Passive State: FRSM_ECU_PASSIVE */

typedef uint8 FrSM_BswM_StateType;

/*======================[Global Data]=========================================*/

/*======================[Global Function Declarations]========================*/

#endif /* FRSM_TYPES_H */

/*
 * = eof ======================================================================
 */

