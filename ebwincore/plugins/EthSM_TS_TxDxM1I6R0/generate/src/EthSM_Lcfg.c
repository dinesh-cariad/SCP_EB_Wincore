/**
 * \file
 *
 * \brief AUTOSAR EthSM
 *
 * This file contains the implementation of the AUTOSAR
 * module EthSM.
 *
 * \version 1.6.14
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[includes]==============================================*/

#include <EthSM_Lcfg.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external constants]=====================================*/

#define ETHSM_START_SEC_CONST_UNSPECIFIED
#include <EthSM_MemMap.h>

#if (ETHSM_MULTI_CORE == STD_ON)
[!VAR "LoopCounter" = "0"!][!//
[!VAR "NumEthSMNetwork" = "num:integer(count(as:modconf('EthSM')[1]/EthSMNetwork/*))"!][!//
CONST(EthSM_ComM_BusSM_ModeIndicationFpType, ETHSM_APPL_CONST)
  EthSM_ComM_BusSM_ModeIndicationFpList[[!"num:i(count(as:modconf('EthSM')[1]/EthSMNetwork/*))"!]U] =
{
[!LOOP "node:order(as:modconf('EthSM')[1]/EthSMNetwork/*, 'as:ref(./EthSMComMNetworkHandleRef)/ComMChannelId')"!]
[!VAR "LoopCounter"="$LoopCounter + 1"!][!//
&SchM_Call_EthSM_RequiredCSEntry_ComM_BusSM_ModeIndication_EthChannel_[!"num:i(as:ref(./EthSMComMNetworkHandleRef)/ComMChannelId)"!][!IF "$LoopCounter < $NumEthSMNetwork"!],[!ENDIF!]
[!ENDLOOP!][!//
};
#endif /* ETHSM_MULTI_CORE == STD_ON */
#define ETHSM_STOP_SEC_CONST_UNSPECIFIED
#include <EthSM_MemMap.h>

#define ETHSM_START_SEC_CONST_32
#include <EthSM_MemMap.h>

/* Value used to validate post build configuration against link time configuration. */
CONST(uint32, ETHSM_CONST) EthSM_LcfgSignature = [!"asc:getConfigSignature(as:modconf('EthSM')[1]//*[not(child::*) and (node:configclass() = 'Link')])"!]U;

#define ETHSM_STOP_SEC_CONST_32
#include <EthSM_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
