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

#ifndef ETHSM_LCFG_H
#define ETHSM_LCFG_H

/*==================[includes]==============================================*/

#include <EthSM.h>      /* EthSM interface headerfile */
#include <SchM_EthSM.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

typedef P2FUNC(Std_ReturnType, TYPEDEF, EthSM_ComM_BusSM_ModeIndicationFpType)
(
  uint8 Channel,
  uint8 ComMode
);

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

#define ETHSM_START_SEC_CONST_UNSPECIFIED
#include <EthSM_MemMap.h>

#if (ETHSM_MULTI_CORE == STD_ON)
extern CONST(EthSM_ComM_BusSM_ModeIndicationFpType, ETHSM_APPL_CONST)
  EthSM_ComM_BusSM_ModeIndicationFpList[[!"num:i(count(as:modconf('EthSM')[1]/EthSMNetwork/*))"!]U];
#endif /* ETHSM_MULTI_CORE == STD_ON */

#define ETHSM_STOP_SEC_CONST_UNSPECIFIED
#include <EthSM_MemMap.h>

#define ETHSM_START_SEC_CONST_32
#include <EthSM_MemMap.h>

/* Value used to validate post build configuration against link time configuration. */
extern CONST(uint32, ETHSM_CONST) EthSM_LcfgSignature;

#define ETHSM_STOP_SEC_CONST_32
#include <EthSM_MemMap.h>

/*==================[end of file]===========================================*/

#endif /* ETHSM_LCFG_H */
