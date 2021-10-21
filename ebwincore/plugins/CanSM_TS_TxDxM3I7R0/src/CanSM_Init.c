/**
 * \file
 *
 * \brief AUTOSAR CanSM
 *
 * This file contains the implementation of the AUTOSAR
 * module CanSM.
 *
 * \version 3.7.5
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * False-positive from the Tasking v5.0r2 compiler (TCVX-41885)
 */

/*==================[inclusions]============================================*/

#include <CanSM_Trace.h>
#include <TSAutosar.h>              /* EB specific standard types */
#include <ComStack_Types.h>         /* AUTOSAR communication stack types */

#include <CanSM_PBcfg.h>            /* Module post-build generated configuration */
#include <CanSM_Api_Static.h>       /* Module public API */
#include <CanSM_Cfg.h>              /* Module generated configuration */
#include <CanSM_Internal.h>         /* Module interna */
#include <ComM.h>                   /* ComM_ModeType */

/*==================[version check]=========================================*/

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#define CANSM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <CanSM_MemMap.h>

/** \brief Global CanSM configuration
 **
 ** This is a pointer to the current CanSM configuration.
 */
P2CONST( CanSM_ConfigType, CANSM_VAR, CANSM_APPL_CONST )
  CanSM_ConfigPtr;

#define CANSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <CanSM_MemMap.h>

/*==================[external function definitions]=========================*/

#define CANSM_START_SEC_CODE
#include <CanSM_MemMap.h>

FUNC(void, CANSM_CODE) CanSM_Init (
  P2CONST( CanSM_ConfigType, AUTOMATIC, CANSM_APPL_CONST ) ConfigPtr
)
{
  CanSM_NetworkIndexType nw;

  P2CONST(CanSM_ConfigType, AUTOMATIC, CANSM_APPL_CONST) LocalConfigPtr = ConfigPtr;

  DBG_CANSM_INIT_ENTRY(ConfigPtr);

#if (CANSM_DEV_ERROR_DETECT == STD_ON)
  /* check whether parameter LocalConfigPtr is a NULL_PTR */
  if (LocalConfigPtr == NULL_PTR)
  {
    /* found a NULL_PTR, report to DET */
    CANSM_DET( CANSM_SID_INIT,
               CANSM_E_PARAM_POINTER
             );
  }
  else
#endif /* CANIF_DEV_ERROR_DETECT */
  {
    /* set global configuration pointer */
    CanSM_ConfigPtr = LocalConfigPtr;

    /* Deviation TASKING-1 */
    for ( nw=0U; nw<CANSM_NETWORKS_NUM; nw++)
    {
      CANSM_NW_INFO_STATE(nw) = CANSM_UNINITED;
      CANSM_NW_INFO_INTERNALSTATE(nw) = CANSM_S_NOIN;
      CANSM_NW_INFO_REQCOMMODE(nw) = COMM_NO_COMMUNICATION;
      CANSM_NW_INFO_TRANSITION_ONGOING(nw) = FALSE;
    }

    CanSM_GlobalInit = CANSM_GLOBAL_INIT;

    /* Deviation TASKING-1 */
    for ( nw=0U; nw<CANSM_NETWORKS_NUM; nw++)
    {
      CanSM_DoTransitionSequence(nw, CANSM_T_RNOCO_INITIAL, FALSE);
    }
  }

  DBG_CANSM_INIT_EXIT(ConfigPtr);

}

#define CANSM_STOP_SEC_CODE
#include <CanSM_MemMap.h>

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/

