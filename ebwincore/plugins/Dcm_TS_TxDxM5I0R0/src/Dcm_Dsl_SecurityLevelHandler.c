/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* !LINKSTO Dcm.Dsn.File.DslSecurityLevelHandler.Impl,1 */
/* This file contains the implementation of the implementation of the Security Level Handler
  software unit. */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * The condition is the result of a false positive condition.
 * The false condition can be reached with any service 0x10 request to change the session to
 * non-default while in the default session. The available coverage reports show that this
 * condition is covered.
 */
/*=================[includes]====================================================================*/

#include <Dcm_Trace.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <TSAutosar.h>                                             /* EB specific standard types */

#include <Dcm_Dsl_SecurityLevelHandler.h>

#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
/* Needed by SetSecurityLevel to retrigger the periodic DIDs evaluation */
#include <Dcm_Dsp_SvcH_ReadDataByPeriodicIdentifier.h>
#endif
/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/
#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm020,1 */
VAR(Dcm_SecLevelType, DCM_VAR) Dcm_CurrentSecurityLevel;

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>
/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.SecurityLevelHandler.InitSecurityLevel,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_SecurityLevelHandler_Init(void)
{
  DBG_DCM_DSL_SECURITYLEVELHANDLER_INIT_ENTRY();

  /* !LINKSTO Dcm033,1 */
  Dcm_CurrentSecurityLevel = DCM_SEC_LEV_LOCKED;

  DBG_DCM_DSL_SECURITYLEVELHANDLER_INIT_EXIT();
}

/* !LINKSTO Dcm.Dsn.SecurityLevelHandler.SetSecurityLevel,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_SecurityLevelHandler_SetSecurityLevel(
  Dcm_SecLevelType newSecLevel)
{
  DBG_DCM_DSL_SECURITYLEVELHANDLER_SETSECURITYLEVEL_ENTRY(newSecLevel);

  Dcm_CurrentSecurityLevel = newSecLevel;

#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
  /* !LINKSTO Dcm.SecurityLevelHandler.PeriodicTransmission,1 */
  /* Trigger rechecking the conditions of scheduled periodic DIDs */
  Dcm_Dsp_ReadDataByPeriodicIdentifier_ForceReValidate();
#endif

  DBG_DCM_DSL_SECURITYLEVELHANDLER_SETSECURITYLEVEL_EXIT(newSecLevel);
}

/* !LINKSTO Dcm.Dsn.SecurityLevelHandler.GetSecurityLevel,1 */
FUNC(Dcm_SecLevelType, DCM_CODE) Dcm_Dsl_SecurityLevelHandler_GetSecurityLevel(void)
{
  Dcm_SecLevelType CurrentSecurityLevel;

  DBG_DCM_DSL_SECURITYLEVELHANDLER_GETSECURITYLEVEL_ENTRY();

  CurrentSecurityLevel = Dcm_CurrentSecurityLevel;

  DBG_DCM_DSL_SECURITYLEVELHANDLER_GETSECURITYLEVEL_EXIT(CurrentSecurityLevel);

  return CurrentSecurityLevel;

}
/* !LINKSTO Dcm.Dsn.IB.SecurityLevelHandler.SessionChange,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_SecurityLevelHandler_SessionChange(
  Dcm_SesCtrlType OldSession, Dcm_SesCtrlType NewSession)
{
  boolean PreemptionOccured;
  DBG_DCM_DSL_SECURITYLEVEL_SESSIONCHANGE_ENTRY(OldSession, NewSession);

  TS_PARAM_UNUSED(NewSession);

  PreemptionOccured = Dcm_Dsl_DiagnosticSessionHandler_GetPreemptionOccured();
  /* Change in security level on session transitions (as per Dcm139)
     Default -> Default : No change
     Default -> Non-default : No change
     Non-Default -> Default : Reset to 0x00
     Non-Default -> Non-Default : Reset to 0x00
     or
     Default -> Default : Reset to 0x00 in case of preemption
  */

  /* Deviation TASKING-1 <+2> */
  if ((DCM_DEFAULT_SESSION != OldSession) ||
      ((DCM_DEFAULT_SESSION == OldSession) && (PreemptionOccured == TRUE)))
  {
    /* reset security level */
    /* !LINKSTO Dcm.SecuritylevelReset.SessionTransCondition01, 1 */
    /* !LINKSTO Dcm.SecuritylevelReset.SessionTransCondition02, 1 */
    /* !LINKSTO Dcm.SecuritylevelReset.SessionTransToDeault.OnS3Timeout, 1 */
    Dcm_Dsl_SecurityLevelHandler_SetSecurityLevel(DCM_SEC_LEV_LOCKED);
  }

  DBG_DCM_DSL_SECURITYLEVEL_SESSIONCHANGE_EXIT();
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function definitions]==============================================*/

/*==================[end of file]================================================================*/
