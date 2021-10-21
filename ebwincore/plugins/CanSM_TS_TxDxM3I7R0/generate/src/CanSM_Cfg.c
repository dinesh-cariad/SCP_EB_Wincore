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

[!INCLUDE "../include/CanSM_Macros.m"!][!//
[!AUTOSPACING!]

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
#include <ComStack_Types.h>         /* AUTOSAR communication stack types */

#include <CanSM_Int_Stc.h>          /* Module internal static header */
#include <CanSM_Cfg.h>              /* Module configuration */
[!IF "$multiCoreSupport = 'true'"!]
#include <SchM_CanSM.h>
[!ENDIF!]

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

[!IF "($globalPnSupport = 'true') or ($multiCoreSupport = 'true')"!]
#define CANSM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <CanSM_MemMap.h>

CONST(CanSM_NetworkConfigType, CANSM_APPL_CONST) CanSM_NetworkConfig[CANSM_NETWORKS_NUM] =
{
[!INDENT "2"!]
  [!LOOP "node:order(CanSMConfiguration/*[1]/CanSMManagerNetwork/*,'as:ref(CanSMComMNetworkHandleRef)/ComMChannelId')"!]
  {
    [!INDENT "4"!]
      [!IF "$globalPnSupport = 'true'"!]
       [!//
      [!IF "CanSMActivatePN = 'true'"!][!//
      TRUE,[!//
      [!ELSE!][!//
      FALSE,[!ENDIF!] /* partial networking */[!//
      [!ENDIF!]
      [!IF "$multiCoreSupport = 'true'"!][!//
      &SchM_Call_CanSM_RequiredCSEntry_ComM_BusSM_ModeIndication_[!"as:ref(CanSMComMNetworkHandleRef)/ComMChannelId"!], /* SchM function pointer */[!//
      [!ENDIF!][!//
    [!ENDINDENT!]
  },
  [!ENDLOOP!]
[!ENDINDENT!]
};

#define CANSM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <CanSM_MemMap.h>
[!ENDIF!][!//($globalPnSupport = 'true') or ($multiCoreSupport = 'true')

/*==================[external data]=========================================*/

#define CANSM_START_SEC_VAR_CLEARED_8
#include <CanSM_MemMap.h>

VAR(uint8, CANSM_VAR) CanSM_CtrlModeIndication[CANSM_CONTROLLERS_NUM];

#define CANSM_STOP_SEC_VAR_CLEARED_8
#include <CanSM_MemMap.h>

#define CANSM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <CanSM_MemMap.h>

VAR(CanSM_NetworkInfoType, CANSM_VAR) CanSM_NetworkInfo[CANSM_NETWORKS_NUM];

#define CANSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <CanSM_MemMap.h>

/*==================[external function definitions]=========================*/

#define CANSM_START_SEC_CODE
#include <CanSM_MemMap.h>

FUNC(CanSM_NetworkIndexType, CANSM_CODE) CanSM_GetNetworkIndexFromHandle(
  NetworkHandleType nw
)
{
  CanSM_NetworkIndexType nwidx = 0U;

  /* Deviation TASKING-1 */
  for (nwidx = 0U; nwidx < CANSM_NETWORKS_NUM; nwidx++)
  {
    if (nw == (CanSM_ConfigPtr->NetRef[(nwidx)].ComMChId))
    {
      break;
    }
  }

  return nwidx;
}

FUNC(NetworkHandleType, CANSM_CODE) CanSM_GetNetworkHandleFromIndex(
  CanSM_NetworkIndexType nwidx
)
{
  NetworkHandleType ret;

  ret = (CanSM_ConfigPtr->NetRef[(nwidx)].ComMChId);
  return ret;
}

FUNC(boolean, CANSM_CODE) CanSM_NetworkHandleIsValid(
  NetworkHandleType nw
)
{
  CanSM_NetworkIndexType nwidx = 0U;
  boolean ret;

  /* Deviation TASKING-1 */
  for (nwidx = 0U; nwidx < CANSM_NETWORKS_NUM; nwidx++)
  {
    if (nw == (CanSM_ConfigPtr->NetRef[(nwidx)].ComMChId))
    {
      break;
    }
  }

  ret = (uint8) ((nwidx == CANSM_NETWORKS_NUM) ? FALSE : TRUE);

  return ret;
}

FUNC(CanSM_NetworkIndexType, CANSM_CODE) CanSM_NwIdxFromTrcvId(
  uint8 trcvID
)
{
  CanSM_NetworkIndexType nwidx;

  /* Deviation TASKING-1 */
  for (nwidx = 0U; nwidx < CANSM_NETWORKS_NUM; nwidx++)
  {
    if (trcvID == (CanSM_ConfigPtr->NetRef[(nwidx)].TrcvId))
    {
      break;
    }
  }

  return nwidx;
}

FUNC(CanSM_NetworkIndexType, CANSM_CODE) CanSM_NwIdxFromCtrlId(
  uint8 ctrlID
)
{
  CanSM_NetworkIndexType nwidx;
  CanSM_NetworkIndexType nwidxret = CANSM_NETWORKS_NUM;
  uint8 ctrlIdx;

  /* Deviation TASKING-1 */
  for (nwidx = 0U; nwidx < CANSM_NETWORKS_NUM; nwidx++)
  {
    for (ctrlIdx = 0U; ctrlIdx < CanSM_ConfigPtr->NetRef[(nwidx)].NumNwCtrl; ctrlIdx++)
    {
      if (ctrlID == (CanSM_ConfigPtr->NetRef[(nwidx)].CtrlRef[ctrlIdx].ControllerId))
      {
        nwidxret = nwidx;
        break;
      }
    }
    if (nwidxret != CANSM_NETWORKS_NUM)
    {
      break;
    }
  }

  return nwidxret;
}

FUNC(uint8, CANSM_CODE) CanSM_CtrlIdxFromCtrlId(
  uint8 ctrlID
)
{
  uint8 nwidx;
  uint8 ctrlIdxRet = CANSM_CONTROLLERS_NUM;
  uint8 ctrlIdx = 0U;
  uint8 ctrlNwIdx;

  /* Deviation TASKING-1 */
  for (nwidx = 0U; nwidx < CANSM_NETWORKS_NUM; nwidx++)
  {
    for (ctrlNwIdx = 0U; ctrlNwIdx < CanSM_ConfigPtr->NetRef[(nwidx)].NumNwCtrl; ctrlNwIdx++)
    {
      if (ctrlID == (CanSM_ConfigPtr->NetRef[(nwidx)].CtrlRef[ctrlNwIdx].ControllerId))
      {
        ctrlIdx += ctrlNwIdx;
        ctrlIdxRet = ctrlIdx;
        break;
      }
    }
    ctrlIdx += ctrlNwIdx;
    if (ctrlIdxRet != CANSM_CONTROLLERS_NUM)
    {
      break;
    }
  }

  return ctrlIdxRet;
}

#define CANSM_STOP_SEC_CODE
#include <CanSM_MemMap.h>


/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/

