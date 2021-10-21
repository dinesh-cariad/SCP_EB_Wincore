/**
 * \file
 *
 * \brief AUTOSAR FrNm
 *
 * This file contains the implementation of the AUTOSAR
 * module FrNm.
 *
 * \version 5.16.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/* !LINKSTO FRNM064_Refine,1 */
[!AUTOSPACING!][!//
/*==================[inclusions]============================================*/

#define FRNM_NO_CFGCLASSMIX_REQUIRED
#ifndef FRNM_NO_PBCFG_REQUIRED
#define FRNM_NO_PBCFG_REQUIRED
#endif

#include <FrNm_Int.h>        /* FrNm internal dependent information */
#include <SchM_FrNm.h>
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[internal data]=========================================*/

/*==================[external data]=========================================*/
#define FRNM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <FrNm_MemMap.h>

[!IF "as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmComUserDataSupport = 'true'"!]
CONST(FrNm_UserDataConfigType, FRNM_CONST)
  FrNm_ComUserDataInfo[FRNM_NUMBER_OF_CHANNELS]=
{
[!LOOP "node:order(FrNmChannelConfig/*/FrNmChannel/*, 'node:value(node:ref(FrNmChannelIdentifiers/FrNmChannelHandle)/FrIfClstIdx)')"!]
  /* Configuration of FR NM channel [!"name(.)"!] : */
  {
   [!/* Userdata length is calculated from the minimum of PduLength of the Tx and Rx containers */!][!//
[!IF "node:exists(FrNmChannelIdentifiers/FrNmUserDataTxPdu)"!][!//
    [!"FrNmChannelIdentifiers/FrNmUserDataTxPdu/FrNmTxUserDataPduId"!]U, /* User Data Tx PduId */[!/*
    Get User Data TxConf PduId by EcuC reference from PduR;
    existence is already checked in FrNm_Checks.m.
    */!]
[!ELSE!][!//
    0U, /* User Data Tx PduId */
[!ENDIF!][!//
[!IF "node:exists(FrNmChannelIdentifiers/FrNmUserDataTxPdu)"!][!//
    TRUE, /* ComUserTxPduEnabled */
[!ELSE!][!//
    FALSE, /* ComUserTxPduEnabled */
[!ENDIF!][!//
[!IF "node:exists(FrNmChannelIdentifiers/FrNmUserDataRxPdu)"!][!//
    TRUE /* ComUserRxPduEnabled */
[!ELSE!][!//
    FALSE /* ComUserRxPduEnabled */
[!ENDIF!][!//
  },
[!ENDLOOP!]
};
[!ENDIF!]

/* Configuration data structure for one channel or cluster */
CONST(FrNm_ChnConfigType, FRNM_CONST) FrNm_ChanConfig[FRNM_NUMBER_OF_CHANNELS] =
{
  [!LOOP "node:order(FrNmChannelConfig/*/FrNmChannel/*, 'node:value(node:ref(FrNmChannelIdentifiers/FrNmChannelHandle)/FrIfClstIdx)')"!][!//
  {
[!IF "as:modconf('FrNm')[1]/FrNmGeneral/FrNmMultiCoreSupport='true'"!]
     &SchM_Call_FrNm_RequiredCSEntry_Nm_NetworkStartIndication_[!"node:name(.)"!],
     &SchM_Call_FrNm_RequiredCSEntry_Nm_NetworkMode_[!"node:name(.)"!],
     &SchM_Call_FrNm_RequiredCSEntry_Nm_BusSleepMode_[!"node:name(.)"!],
[!IF "as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmRemoteSleepIndicationEnabled = 'true'"!]
     &SchM_Call_FrNm_RequiredCSEntry_Nm_RemoteSleepIndication_[!"node:name(.)"!],
     &SchM_Call_FrNm_RequiredCSEntry_Nm_RemoteSleepCancellation_[!"node:name(.)"!],
[!ENDIF!]
[!IF "as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmCoordinatorSyncSupport = 'true'"!]
     &SchM_Call_FrNm_RequiredCSEntry_Nm_CoordReadyToSleepIndication_[!"node:name(.)"!],
[!ENDIF!]
[!IF "num:i(count(as:modconf('FrNm')[1]/FrNmChannelConfig/*/FrNmChannel/*/FrNmChannelIdentifiers[FrNmSynchronizationPointEnabled = 'true'])) != 0"!]
[!IF "FrNmChannelIdentifiers/FrNmSynchronizationPointEnabled='true'"!]
     &SchM_Call_FrNm_RequiredCSEntry_Nm_SynchronizationPoint_[!"node:name(.)"!],
[!ELSE!]
     NULL_PTR,
[!ENDIF!]
[!ENDIF!]
[!IF "as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmPduRxIndicationEnabled = 'true'"!]
     &SchM_Call_FrNm_RequiredCSEntry_Nm_PduRxIndication_[!"node:name(.)"!],
[!ENDIF!]
[!IF "as:modconf('FrNm')[1]/FrNmGlobalConfig/FrNmGlobalFeatures/FrNmStateChangeIndicationEnabled = 'true'"!]
     &SchM_Call_FrNm_RequiredCSEntry_Nm_StateChangeNotification_[!"node:name(.)"!],
[!ENDIF!]
     &SchM_Call_FrNm_RequiredCSEntry_Nm_TxTimeoutException_[!"node:name(.)"!],
[!IF "count(as:modconf('FrNm')[1]/FrNmChannelConfig/*[1]/FrNmChannel/*[FrNmChannelIdentifiers/FrNmCarWakeUpRxEnabled = 'true']) > 0"!]
[!IF "FrNmChannelIdentifiers/FrNmCarWakeUpRxEnabled='true'"!]
     &SchM_Call_FrNm_RequiredCSEntry_Nm_CarWakeUpIndication_[!"node:name(.)"!],
[!ELSE!]
     NULL_PTR,
[!ENDIF!]
[!ENDIF!]
[!ENDIF!]
    [!SELECT "FrNmChannelIdentifiers"!]
    {
#if (FRNM_PN_ERA_CALC_ENABLED == STD_ON)
      [!IF "(node:exists(FrNmPnEraCalcEnabled)) and (FrNmPnEraCalcEnabled='true')"!][!//
        [!"as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/PduRRoutingPath/*/
             PduRSrcPdu[PduRSrcPduRef=node:current()/FrNmPnEraRxNSduRef]/
             PduRSourcePduHandleId"!]U, /* EraPduId */
      1U, /* EraCalcEnabled */
      [!ELSE!]
      0U, /* EraPduId */
      0U, /* EraCalcEnabled */
      [!ENDIF!][!//
#endif
[!IF "count(as:modconf('FrNm')[1]/FrNmChannelConfig/*[1]/FrNmChannel/*[FrNmChannelIdentifiers/FrNmCarWakeUpRxEnabled = 'true']) > 0"!]
  [!IF "node:exists(FrNmCarWakeUpBitPosition)"!][!//
      [!"num:i(FrNmCarWakeUpBitPosition)"!]U, /* FrNmCarWakeUpBitPosition */
  [!ELSE!][!//
      0U, /* FrNmCarWakeUpBitPosition */
  [!ENDIF!][!//
  [!IF "node:exists(FrNmCarWakeUpBytePosition)"!][!//
      [!"num:i(FrNmCarWakeUpBytePosition)"!]U, /* FrNmCarWakeUpBytePosition */
  [!ELSE!][!//
      0U, /* FrNmCarWakeUpBytePosition */
  [!ENDIF!][!//
  [!IF "node:exists(FrNmCarWakeUpFilterNodeId)"!][!//
      [!"num:i(FrNmCarWakeUpFilterNodeId)"!]U, /* FrNmCarWakeUpFilterNodeId */
  [!ELSE!][!//
      0U, /* FrNmCarWakeUpFilterNodeId */
  [!ENDIF!][!//
[!ENDIF!]
      /* !LINKSTO FrNm0070_Conf_Refine,1 */
      [!"num:i(as:ref(FrNmComMNetworkHandleRef)/ComMChannelId)"!]U, /* ComM channel Id */
  [!IF "FrNmSourceNodeIdentifierEnabled = 'true'"!]
      0x02U,   /* FrNmReservedBytes */
      TRUE,    /* FrNmSourceNodeIdentifierEnabled */
  [!ELSE!]
      0x01U,   /* FrNmReservedBytes */
      FALSE,   /* FrNmSourceNodeIdentifierEnabled */
  [!ENDIF!]
  [!IF "FrNmNodeDetectionEnabled = 'true'"!]
      TRUE,  /* FrNmNodeDetectionEnabled */
  [!ELSE!]
      FALSE, /* FrNmNodeDetectionEnabled */
  [!ENDIF!]
[!IF "num:i(count(as:modconf('FrNm')[1]/FrNmChannelConfig/*/FrNmChannel/*[(node:exists(FrNmChannelIdentifiers/FrNmActiveWakeupBitEnabled)) and (FrNmChannelIdentifiers/FrNmActiveWakeupBitEnabled = 'true')])) != 0"!]
      [!IF "FrNmActiveWakeupBitEnabled = 'true'"!][!//
      TRUE, /* FrNmActiveWakeupBitEnabled */
      [!ELSE!]
      FALSE, /* FrNmActiveWakeupBitEnabled */
      [!ENDIF!][!//
[!ENDIF!]
[!IF "count(as:modconf('FrNm')[1]/FrNmChannelConfig/*[1]/FrNmChannel/*[FrNmChannelIdentifiers/FrNmCarWakeUpRxEnabled = 'true']) > 0"!]
  [!IF "node:exists(FrNmCarWakeUpFilterEnabled)"!][!//
    [!IF "FrNmCarWakeUpFilterEnabled = 'true'"!][!//
      TRUE, /* FrNmCarWakeUpFilterEnabled */
    [!ELSE!][!//
      FALSE, /* FrNmCarWakeUpFilterEnabled */
    [!ENDIF!][!//
  [!ELSE!][!//
      FALSE, /* FrNmCarWakeUpFilterEnabled */
  [!ENDIF!][!//
  [!IF "FrNmCarWakeUpRxEnabled = 'true'"!][!//
      TRUE, /* FrNmCarWakeUpRxEnabled */
  [!ELSE!]
      FALSE, /* FrNmCarWakeUpRxEnabled */
  [!ENDIF!]
[!ENDIF!]
    },
    [!ENDSELECT!]
    [!SELECT "FrNmChannelTiming"!]
    {
      [!"FrNmReadySleepCnt"!], /* Number of repetitions in the ready sleep state */
      [!"num:i((FrNmRemoteSleepIndTime * 1000) div (FrNmMainFunctionPeriod * 1000))"!]U, /* Remote sleep indication Time */
      [!"num:i((FrNmRepeatMessageTime * 1000) div (FrNmMainFunctionPeriod * 1000))"!]U, /* Timeout for repeat message state */
      [!"FrNmDataCycle"!], /* Number of FlexRay schedule cycles for Nm data be transmitted*/
      [!"FrNmRepetitionCycle"!], /* Number of FlexRay schedule cycles for Nm vote be changed*/
      [!"FrNmVotingCycle"!], /* Number of FlexRay schedule cycles for Nm vote be transmitted*/
    },
    [!ENDSELECT!]
    [!IF "(node:exists(FrNmAllNmMessagesKeepAwake)) and (FrNmAllNmMessagesKeepAwake = 'true')"!][!//
    TRUE /* AllNmMessagesKeepAwake */
    [!ELSE!][!//
    FALSE /* AllNmMessagesKeepAwake */
    [!ENDIF!][!//
  },
[!ENDLOOP!][!//
};

#define FRNM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <FrNm_MemMap.h>

/*==================[external function definitions]=========================*/
#define FRNM_START_SEC_CODE
#include <FrNm_MemMap.h>

[!VAR "LeastMainFnPeriodChId" = "0"!][!//
[!VAR "LeastMainFnPeriodChId" = "node:value(node:ref(FrNmChannelConfig/*/FrNmChannel/*[FrNmChannelTiming/FrNmMainFunctionPeriod = num:min(../*/FrNmChannelTiming/FrNmMainFunctionPeriod)]/FrNmChannelIdentifiers/FrNmChannelHandle)/FrIfClstIdx)"!]

[!FOR "I"="0" TO "num:i(FrNmGlobalConfig/FrNmGlobalConstants/FrNmNumberOfClusters)-1"!]
  FUNC(void, FRNM_CODE) FrNm_MainFunction_[!"num:i($I)"!](void)
  {
    FrNm_MainFunction([!"num:i($I)"!]U);
[!IF "$I = $LeastMainFnPeriodChId"!]
  if (FrNm_InitStatus == FRNM_INIT)
  {
#if (FRNM_PN_EIRA_CALC_ENABLED == STD_ON)
    FrNm_HandlePnTimers(&FrNm_PnEiraStatus, FRNM_PN_EIRA_PDUID);
#endif
  }
[!ENDIF!]
  }
[!ENDFOR!]

#define FRNM_STOP_SEC_CODE
#include <FrNm_MemMap.h>
/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
