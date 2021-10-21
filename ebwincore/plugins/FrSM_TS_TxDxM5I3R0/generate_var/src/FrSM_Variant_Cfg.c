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

/*==================[includes]==============================================*/

[!INCLUDE "../../generate/include/FrSM_Macros.m"!][!//
[!INCLUDE "../../generate/include/FrSM_Variables.m"!][!//

#include <FrSM_Cfg.h>
#include <FrSM_Int.h>
#include <FrSM_CfgTypes.h>

/*==================[external constants]====================================*/

#define FRSM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

[!VAR "cfgName"="FrSMConfig/*[1]/@name"!]

[!IF "var:defined('postBuildVariant')"!]
[!/* Current postbuild configuration name
*/!][!VAR "initPredefinedCfgName"="concat($cfgName,'_',$postBuildVariant)"!]
[!ELSE!]
[!/* Current postbuild name
*/!][!VAR "initPredefinedCfgName"="$cfgName"!]
[!ENDIF!]

CONST(FrSM_CfgType, FRSM_APPL_CONST) [!"$initPredefinedCfgName"!]_Layout =
{
[!VAR "LoopCounter" = "0"!][!//
[!//
[!LOOP "FrSMConfig/*[1]/FrSMCluster/*"!][!//
[!IF "num:integer($LoopCounter) <= 1"!][!//
    {
[!ENDIF!][!//
[!IF "num:integer($LoopCounter) = 0"!][!//
        [!"$VarNumClst"!]U,
        {
[!ENDIF!][!//
            {
#if (FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP == TS_PROD_ERR_REP_TO_DEM)
                [!IF "node:exists(./FrSMClusterDemEventParameterRefs/FRSM_E_CLUSTER_STARTUP)"!][!/*
            */!][!"as:ref(FrSMClusterDemEventParameterRefs/FRSM_E_CLUSTER_STARTUP)/DemEventId"!]U,
                [!ELSE!][!/*
            */!]FRSM_INVALID_DEM_EVENTID, /* Disabled for this network */
                [!ENDIF!]
#endif /* FRSM_PROD_ERR_HANDLING_CLUSTER_STARTUP == TS_PROD_ERR_REP_TO_DEM */
#if (FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS == TS_PROD_ERR_REP_TO_DEM)
                [!IF "node:exists(./FrSMClusterDemEventParameterRefs/FRSM_E_CLUSTER_SYNC_LOSS)"!][!/*
            */!][!"as:ref(FrSMClusterDemEventParameterRefs/FRSM_E_CLUSTER_SYNC_LOSS)/DemEventId"!]U,
                [!ELSE!][!/*
            */!]FRSM_INVALID_DEM_EVENTID, /* Disabled for this network */
                [!ENDIF!]
#endif /* FRSM_PROD_ERR_HANDLING_CLUSTER_SYNC_LOSS == TS_PROD_ERR_REP_TO_DEM */
                [!CALL "TimeoutToTicks", "Time"="./FrSMDurationT1", "MainFunctionCycleTime"="./FrSMMainFunctionCycleTime"!]U,
                [!CALL "TimeoutToTicks", "Time"="./FrSMDurationT2", "MainFunctionCycleTime"="./FrSMMainFunctionCycleTime"!]U,
                [!CALL "TimeoutToTicks", "Time"="./FrSMDurationT3", "MainFunctionCycleTime"="./FrSMMainFunctionCycleTime"!]U,
                [!IF "node:exists(./FrSMStartupRepetitions)"!][!/*
            */!][!"./FrSMStartupRepetitions"!]U,[!/*
            */!][!ELSE!][!/*
            */!]FRSM_REPETITIONS_INFINITE_VALUE,[!/*
            */!][!ENDIF!]
                [!IF "node:exists(./FrSMStartupRepetitionsWithWakeup)"!][!/*
            */!][!"./FrSMStartupRepetitionsWithWakeup"!]U,[!/*
            */!][!ELSE!][!/*
            */!]FRSM_REPETITIONS_INFINITE_VALUE,[!/*
            */!][!ENDIF!]
                [!"as:ref(./FrSMComMNetworkHandleRef)/ComMChannelId"!]U,
                [!"as:ref(./FrSMFrIfClusterRef)/FrIfClstIdx"!]U,
                [!"as:ref(./FrSMFrIfClusterRef)/FrIfController/*[1]/FrIfCtrlIdx"!]U,
                [!VAR "StartupOptions"="0"!][!/*
            */!][!IF "count(as:ref(./FrSMFrIfClusterRef)/FrIfController/*[1]/FrIfTransceiver/*[FrIfClusterChannel = 'FRIF_CHANNEL_A']) > 0"!][!VAR "StartupOptions"="$StartupOptions + 1"!][!ENDIF!][!/*
            */!][!IF "count(as:ref(./FrSMFrIfClusterRef)/FrIfController/*[1]/FrIfTransceiver/*[FrIfClusterChannel = 'FRIF_CHANNEL_B']) > 0"!][!VAR "StartupOptions"="$StartupOptions + 2"!][!ENDIF!][!/*
            */!][!IF "./FrSMIsColdstartEcu = 'true'"!][!VAR "StartupOptions"="$StartupOptions + 4"!][!ENDIF!][!/*
            */!][!IF "./FrSMIsWakeupEcu = 'true'"!][!VAR "StartupOptions"="$StartupOptions + 8"!][!ENDIF!][!/*
            */!][!IF "./FrSMCheckWakeupReason = 'true'"!][!VAR "StartupOptions"="$StartupOptions + 16"!][!ENDIF!][!/*
            */!][!IF "as:ref(./FrSMFrIfClusterRef)/FrIfGChannels = 'FR_CHANNEL_AB'"!][!VAR "StartupOptions"="$StartupOptions + 32"!][!ENDIF!][!/*
            */!][!IF "./FrSMDelayStartupWithoutWakeup = 'true'"!][!VAR "StartupOptions"="$StartupOptions + 64"!][!ENDIF!][!/*
            */!][!"num:integer($StartupOptions)"!]U,
                [!"FrSMNumWakeupPatterns"!]U
            },
[!IF "num:integer($LoopCounter) = 0"!][!//
        }
[!IF "num:integer(count(as:modconf('FrSM')[1]/FrSMConfig/*[1]/FrSMCluster/*)) > 1"!][!//
    },
[!ENDIF!][!//
[!ENDIF!][!//
[!//
[!VAR "LoopCounter"="$LoopCounter + 1"!][!//
[!ENDLOOP!][!//
    }
};

#define FRSM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

/*==================[end of file]===========================================*/
