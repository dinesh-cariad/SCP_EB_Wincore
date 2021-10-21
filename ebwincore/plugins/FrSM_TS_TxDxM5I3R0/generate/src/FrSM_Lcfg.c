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

#include <FrSM_Lcfg.h>
#include <FrSM_Int.h>

/*==================[external data]=========================================*/

#define FRSM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

VAR(FrSM_ClstRuntimeDataType,FRSM_VAR_CLEARED) FrSM_ClstRuntimeData[[!"num:integer(count(FrSMConfig/*[1]/FrSMCluster/*))"!]U];

#define FRSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

#define FRSM_START_SEC_CONST_UNSPECIFIED
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

#if ((FRSM_COMM_INDICATION_ENABLE == STD_ON) && (FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_ON))
[!VAR "LoopCounter" = "0"!][!//
[!VAR "NumFrSMCluster" = "num:integer(count(FrSMConfig/*[1]/FrSMCluster/*))"!][!//
CONST(FrSM_ComM_BusSM_ModeIndication_FctPtr, FRSM_CONST)
  FrSM_ComM_BusSM_ModeIndicationFpList[[!"$NumFrSMCluster"!]U] =
{
[!LOOP "node:order(as:modconf('FrSM')[1]/FrSMConfig/*[1]/FrSMCluster/*, 'as:ref(./FrSMFrIfClusterRef)/FrIfClstIdx')"!][!//
[!VAR "LoopCounter"="$LoopCounter + 1"!][!//
&SchM_Call_FrSM_RequiredCSEntry_ComM_BusSM_ModeIndication_FrComMChannel_[!"num:i(as:ref(./FrSMComMNetworkHandleRef)/ComMChannelId)"!][!IF "$LoopCounter < $NumFrSMCluster"!],[!ENDIF!]
[!ENDLOOP!][!//
};
#endif /* (FRSM_COMM_INDICATION_ENABLE == STD_ON) && (FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_ON) */

#define FRSM_STOP_SEC_CONST_UNSPECIFIED
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

/*==================[external function definitions]=========================*/

#define FRSM_START_SEC_CODE
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

[!VAR "LoopCounter" = "0"!][!//
[!LOOP "FrSMConfig/*[1]/FrSMCluster/*"!]
/* FrSM_MainFunction_[!"as:ref(./FrSMFrIfClusterRef)/FrIfClstIdx"!]() implementation */
FUNC(void,FRSM_CODE) FrSM_MainFunction_[!"as:ref(./FrSMFrIfClusterRef)/FrIfClstIdx"!](void)
{
    FrSM_MainFunction([!"num:integer($LoopCounter)"!]U);[!// Translate FrIfClstIdx to FrSM index
}
[!VAR "LoopCounter"="$LoopCounter + 1"!][!//
[!ENDLOOP!]

#define FRSM_STOP_SEC_CODE
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

/*==================[end of file]===========================================*/
