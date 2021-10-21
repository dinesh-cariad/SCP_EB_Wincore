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

/* mgcc62 Deviation List
 *
 * MGCC62-1) Deviated Rule: redundant-decls
 * warning: redundant redeclaration [-Wredundant-decls]
 *
 * Reason:
 * FrSM_MainFunction can be redefined by the BSW Scheduler (SchM).
 */

#ifndef FRSM_LCFG_H
#define FRSM_LCFG_H

/*
 * Include Section
 */

#include <FrSM.h>      /* FrSM interface headerfile */
#include <SchM_FrSM.h>

/*
 * Global Function Declarations
 */

#define FRSM_START_SEC_CODE
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

[!LOOP "FrSMConfig/*[1]/FrSMCluster/*"!]
/**
 * \brief FrSM main function for FlexRay cluster with FrIfClstIdx [!"as:ref(./FrSMFrIfClusterRef)/FrIfClstIdx"!]
 *
 * \ServiceID{0x80}
 */
/* Deviation MGCC62-1 */
extern FUNC(void,FRSM_CODE) FrSM_MainFunction_[!"as:ref(./FrSMFrIfClusterRef)/FrIfClstIdx"!](void);
[!ENDLOOP!]

#define FRSM_STOP_SEC_CODE
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

#define FRSM_START_SEC_CONST_UNSPECIFIED
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

#if (FRSM_COMM_INDICATION_ENABLE == STD_ON && FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_ON)
typedef P2FUNC(Std_ReturnType, TYPEDEF, FrSM_ComM_BusSM_ModeIndication_FctPtr)
(
  uint8 Channel,
  uint8 ComMode
);

extern CONST(FrSM_ComM_BusSM_ModeIndication_FctPtr, FRSM_CONST)
  FrSM_ComM_BusSM_ModeIndicationFpList[[!"num:integer(count(FrSMConfig/*[1]/FrSMCluster/*))"!]U];
#endif /* FRSM_COMM_INDICATION_ENABLE == STD_ON && FRSM_MULTI_CORE_SUPPORT_ENABLE == STD_ON */

#define FRSM_STOP_SEC_CONST_UNSPECIFIED
#include <FrSM_MemMap.h>  /* !LINKSTO FrSm.ASR40.FrSm057,1 */

#endif /* FRSM_LCFG_H */


/*
 * = eof ======================================================================
 */
