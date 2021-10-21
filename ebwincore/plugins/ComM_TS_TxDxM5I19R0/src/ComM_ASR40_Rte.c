/**
 * \file
 *
 * \brief AUTOSAR ComM
 *
 * This file contains the implementation of the AUTOSAR
 * module ComM.
 *
 * \version 5.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 8.9 (required)
 * An object should be defined at block scope if its identifier only appears in a single function.
 *
 * Reason:
 * Since array is to large is more efficient to use preprocess definition.
 *
 * MISRAC2012-2) Deviated Rule: 8.3 (required)
 * All declarations of an object or function shall use the same names and type qualifiers.
 *
 * Reason:
 * typedef of generated file .arxml don't use the same name.
 */

/*==================[inclusions]=================================================================*/

#include <ComM_Trace.h>

/* to prevent duplicate declarations of symbols from Rte_ComM.h by our public headers */
#if (defined COMM_INTERNAL_USAGE)
#error COMM_INTERNAL_USAGE already defined
#endif
#define COMM_INTERNAL_USAGE
#include <ComM_BSW.h>                                            /* Declaration of ComM BSW APIs */
#include <ComM_Int.h>                                                   /* Internal declarations */

#if (COMM_INCLUDE_RTE == STD_ON)
#if (COMM_ENABLE_ASR40_SERVICE_API == STD_ON)

#include <ComM_Rte_Cfg.h>  /* Declaration of ComM APIs visible to RTE - consistency enforcement */

#endif /* (COMM_ENABLE_ASR40_SERVICE_API == STD_ON) */
#endif /* (COMM_INCLUDE_RTE == STD_ON) */

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

#if ((COMM_INCLUDE_RTE == STD_ON) && (COMM_NUM_USERS != 0U))
#if (COMM_ENABLE_ASR40_SERVICE_API == STD_ON)

#define COMM_START_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

/* RTE_MODE_ComM_ASR40_Mode_COMM_FULL_COMMUNICATION
 *                                      = 0 <-> COMM_FULL_COMMUNICATION   = 2
 * RTE_MODE_ComM_ASR40_Mode_COMM_NO_COMMUNICATION
 *                                      = 1 <-> COMM_NO_COMMUNICATION     = 0
 * RTE_MODE_ComM_ASR40_Mode_COMM_SILENT_COMMUNICATION
 *                                      = 2 <-> COMM_SILENT_COMMUNICATION = 1
 * see Rte_Types.h
 *
 * Notes: RTE_MODE_ComM_ASR40_Mode_* mode declarations are numbered automatically by
 * the RTE according to the lexical sorting of the symbols. */
/* Deviation MISRAC2012-1 */
STATIC CONST(Rte_ModeType_ComM_ASR40_Mode, COMM_CONST) ComM_ASR40_ComModeToRteMode[] =
{
  RTE_MODE_ComM_ASR40_Mode_COMM_NO_COMMUNICATION,
  RTE_MODE_ComM_ASR40_Mode_COMM_SILENT_COMMUNICATION,
  RTE_MODE_ComM_ASR40_Mode_COMM_FULL_COMMUNICATION
};

#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

#endif /* (COMM_ENABLE_ASR40_SERVICE_API == STD_ON) */
#endif /* (COMM_INCLUDE_RTE == STD_ON) */

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[internal function definitions]==============================================*/

#if (COMM_INCLUDE_RTE == STD_ON)
#if (COMM_ENABLE_ASR40_SERVICE_API == STD_ON)

#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>

FUNC(void, COMM_CODE) ComM_ASR40_RteNotify(
  uint8               User,
  ComM_ASR40_ModeType CurComMMode
)
{
#if (COMM_NUM_USERS != 0U)
  /* Report the mode to RTE, if the current mode was not yet reported to
   * the Rte for this user */
  const Rte_ModeType_ComM_ASR40_Mode newRteMode
    = ComM_ASR40_ComModeToRteMode[CurComMMode];

  ComM_RteNotificationSuccessful[User] = TRUE;

  if (ComM_ASR40_RteModeUser[User] != CurComMMode)
  {
    const Std_ReturnType rteReturnVal = ComM_ASR40_RteSwitchCbk[User](newRteMode);
    if (E_OK == rteReturnVal)
    {
      ComM_ASR40_RteModeUser[User] = CurComMMode;
    }
    else
    {
      ComM_RteNotificationSuccessful[User] = FALSE;
    }
  }
#else
  TS_PARAM_UNUSED(User);
  TS_PARAM_UNUSED(CurComMMode);
#endif
}

#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>

#endif /* (COMM_ENABLE_ASR40_SERVICE_API == STD_ON) */
#endif /* (COMM_INCLUDE_RTE == STD_ON) */

/*==================[external function definitions]==============================================*/

#if (COMM_INCLUDE_RTE == STD_ON)
#if (COMM_ENABLE_ASR40_SERVICE_API == STD_ON)

#define ComM_START_SEC_CODE
#include <ComM_MemMap.h>

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) \
     || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
/* !LINKSTO ComM.EB.ASR32.ComM109,1 */
/* Deviation MISRAC2012-2 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_Rte_GetInhibitionStatus
(
  NetworkHandleType                                                 pdav0,
  P2VAR(ComM_ASR40_InhibitionStatusType, AUTOMATIC, COMM_APPL_DATA) Status
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR40_RTE_GETINHIBITIONSTATUS_ENTRY(pdav0,Status);

  RetVal = ComM_ASR40_GetInhibitionStatus(pdav0, Status);

  DBG_COMM_ASR40_RTE_GETINHIBITIONSTATUS_EXIT(RetVal,pdav0,Status);

  return RetVal;
}
#endif

#if (COMM_NUM_USERS != 0U)
/* !LINKSTO ComM.EB.ASR32.ComM109,1 */
/* Deviation MISRAC2012-2 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_Rte_RequestComMode
(
  ComM_ASR40_UserHandleType pdav0,
  ComM_ASR40_ModeType       ComMode
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR40_RTE_REQUESTCOMMODE_ENTRY(pdav0,ComMode);

  RetVal = ComM_ASR40_RequestComMode(pdav0, ComMode);

  DBG_COMM_ASR40_RTE_REQUESTCOMMODE_EXIT(RetVal,pdav0,ComMode);

  return RetVal;
}
/* !LINKSTO ComM.EB.ASR32.ComM109,1 */
/* Deviation MISRAC2012-2 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_Rte_GetMaxComMode
(
  ComM_ASR40_UserHandleType                             pdav0,
  P2VAR(ComM_ASR40_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR40_RTE_GETMAXCOMMODE_ENTRY(pdav0,ComMode);

  RetVal = ComM_ASR40_GetMaxComMode(pdav0, ComMode);

  DBG_COMM_ASR40_RTE_GETMAXCOMMODE_EXIT(RetVal,pdav0,ComMode);

  return RetVal;
}
/* !LINKSTO ComM.EB.ASR32.ComM109,1 */
/* Deviation MISRAC2012-2 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_Rte_GetRequestedComMode
(
  ComM_ASR40_UserHandleType                             pdav0,
  P2VAR(ComM_ASR40_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR40_RTE_GETREQUESTEDCOMMODE_ENTRY(pdav0,ComMode);

  RetVal = ComM_ASR40_GetRequestedComMode(pdav0, ComMode);

  DBG_COMM_ASR40_RTE_GETREQUESTEDCOMMODE_EXIT(RetVal,pdav0,ComMode);

  return RetVal;
}
/* !LINKSTO ComM.EB.ASR32.ComM109,1 */
/* Deviation MISRAC2012-2 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_Rte_GetCurrentComMode
(
  ComM_ASR40_UserHandleType                             pdav0,
  P2VAR(ComM_ASR40_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR40_RTE_GETCURRENTCOMMODE_ENTRY(pdav0,ComMode);

  RetVal = ComM_ASR40_GetCurrentComMode(pdav0, ComMode);

  DBG_COMM_ASR40_RTE_GETCURRENTCOMMODE_EXIT(RetVal,pdav0,ComMode);

  return RetVal;
}

/* Deviation MISRAC2012-2 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_Rte_GetCurrentPNCComMode
(
  ComM_ASR40_UserHandleType                             pdav0,
  P2VAR(ComM_ASR40_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR40_RTE_GETCURRENTPNCCOMMODE_ENTRY(pdav0,ComMode);

  RetVal = ComM_ASR40_GetCurrentPNCComMode(pdav0, ComMode);

  DBG_COMM_ASR40_RTE_GETCURRENTPNCCOMMODE_EXIT(RetVal,pdav0,ComMode);

  return RetVal;
}
#endif /* #if (COMM_NUM_USERS != 0U) */
#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
/* !LINKSTO ComM.EB.ASR32.ComM109,1 */
/* Deviation MISRAC2012-2 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_Rte_PreventWakeUp
(
  NetworkHandleType pdav0,
  boolean           Status
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR40_RTE_PREVENTWAKEUP_ENTRY(pdav0,Status);

  RetVal = ComM_ASR40_PreventWakeUp(pdav0, Status);

  DBG_COMM_ASR40_RTE_PREVENTWAKEUP_EXIT(RetVal,pdav0,Status);

  return RetVal;
}
#endif

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
/* !LINKSTO ComM.EB.ASR32.ComM109,1 */
/* Deviation MISRAC2012-2 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_Rte_LimitChannelToNoComMode
(
  NetworkHandleType pdav0,
  boolean           Status
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR40_RTE_LIMITCHANNELTONOCOMMODE_ENTRY(pdav0,Status);

  RetVal = ComM_ASR40_LimitChannelToNoComMode(pdav0, Status);

  DBG_COMM_ASR40_RTE_LIMITCHANNELTONOCOMMODE_EXIT(RetVal,pdav0,Status);

  return RetVal;
}

/* !LINKSTO ComM.EB.ASR32.ComM109,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_Rte_LimitECUToNoComMode
(
  boolean Status
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR40_RTE_LIMITECUTONOCOMMODE_ENTRY(Status);

  RetVal = ComM_ASR40_LimitECUToNoComMode(Status);

  DBG_COMM_ASR40_RTE_LIMITECUTONOCOMMODE_EXIT(RetVal,Status);

  return RetVal;
}
#endif

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON)           \
     || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
/* !LINKSTO ComM.EB.ASR32.ComM109,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_Rte_ReadInhibitCounter
(
  P2VAR(uint16, AUTOMATIC, COMM_APPL_DATA) CounterValue
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR40_RTE_READINHIBITCOUNTER_ENTRY(CounterValue);

  RetVal = ComM_ASR40_ReadInhibitCounter(CounterValue);

  DBG_COMM_ASR40_RTE_READINHIBITCOUNTER_EXIT(RetVal,CounterValue);

  return RetVal;
}

/* !LINKSTO ComM.EB.ASR32.ComM109,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_Rte_ResetInhibitCounter
(
  void
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR40_RTE_RESETINHIBITCOUNTER_ENTRY();

  RetVal = ComM_ASR40_ResetInhibitCounter();

  DBG_COMM_ASR40_RTE_RESETINHIBITCOUNTER_EXIT(RetVal);

  return RetVal;
}
/* !LINKSTO ComM.EB.ASR32.ComM109,1 */
/* Deviation MISRAC2012-2 */
FUNC(Std_ReturnType, COMM_CODE) ComM_ASR40_Rte_SetECUGroupClassification
(
  ComM_ASR40_InhibitionStatusType Status
)
{
  Std_ReturnType RetVal;

  DBG_COMM_ASR40_RTE_SETECUGROUPCLASSIFICATION_ENTRY(Status);

  RetVal = ComM_ASR40_SetECUGroupClassification(Status);

  DBG_COMM_ASR40_RTE_SETECUGROUPCLASSIFICATION_EXIT(RetVal,Status);

  return RetVal;
}
#endif

#define ComM_STOP_SEC_CODE
#include <ComM_MemMap.h>

#endif /* (COMM_ENABLE_ASR40_SERVICE_API == STD_ON) */
#endif /* (COMM_INCLUDE_RTE == STD_ON) */

#if ((COMM_INCLUDE_RTE == STD_OFF) || \
     (COMM_ENABLE_ASR40_SERVICE_API == STD_OFF))

#include <TSCompiler.h>           /* usage of macro in TSCompiler_Default.h */

/* Avoid empty translation unit according to ISO C90 */
TS_PREVENTEMPTYTRANSLATIONUNIT

#endif /* ((COMM_INCLUDE_RTE == STD_OFF) || (COMM_ENABLE_ASR40_SERVICE_API == STD_OFF)) */

/*==================[end of file]================================================================*/
