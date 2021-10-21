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
 * MISRAC2012-1) Deviated Rule: 8.9 (advisory)
 * An object should be defined at block scope if its identifier only appears in a single function.
 *
 * Reason:
 * The object has internal linkage for readability improvement.
 *
 * MISRAC2012-2) Deviated Rule: 8.3 (required)
 * All declarations of an object or function shall use the same names and type qualifiers.
 *
 * Reason:
 * typedef of generated file .arxml don't use the same name.
 *
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

/* !LINKSTO ComM506_Refine_SchM,1 */
#include <SchM_ComM.h>                                    /* Header of Schedule Manager for ComM */

#if (COMM_INCLUDE_RTE == STD_ON)

#include <ComM_Rte_Cfg.h>  /* Declaration of ComM APIs visible to RTE - consistency enforcement */

#endif /* (COMM_INCLUDE_RTE == STD_ON) */

#include <ComM_BSW_Cfg.h>

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

#if ((COMM_INCLUDE_RTE == STD_ON) && (COMM_NUM_USERS != 0U))
#if (COMM_DEFAULT_ASR_SERVICE_API != COMM_SERVICE_API_NONE)

#define COMM_START_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

#if (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR40)
/* RTE_MODE_ComMMode_COMM_FULL_COMMUNICATION
 *                                      = 0 <-> COMM_FULL_COMMUNICATION   = 2
 * RTE_MODE_ComMMode_COMM_NO_COMMUNICATION
 *                                      = 1 <-> COMM_NO_COMMUNICATION     = 0
 * RTE_MODE_ComMMode_COMM_SILENT_COMMUNICATION
 *                                      = 2 <-> COMM_SILENT_COMMUNICATION = 1
 * see Rte_Types.h
 *
 * Notes: RTE_MODE_ComMMode_* mode declarations are numbered automatically by
 * the RTE according to the lexical sorting of the symbols. */
/* !LINKSTO ComM.ASR32.ComM761,1 */
/* Deviation MISRAC2012-1 */
STATIC CONST(Rte_ModeType_ComMMode, COMM_CONST) ComM_ComModeToRteMode[] =
{
  RTE_MODE_ComMMode_COMM_NO_COMMUNICATION,
  RTE_MODE_ComMMode_COMM_SILENT_COMMUNICATION,
  RTE_MODE_ComMMode_COMM_FULL_COMMUNICATION
};

#elif (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR32)

/* RTE_MODE_ComMMode_FULL_COMMUNICATION
 *                                      = 0 <-> COMM_FULL_COMMUNICATION   = 2
 * RTE_MODE_ComMMode_NO_COMMUNICATION
 *                                      = 1 <-> COMM_NO_COMMUNICATION     = 0
 * RTE_MODE_ComMMode_SILENT_COMMUNICATION
 *                                      = 2 <-> COMM_SILENT_COMMUNICATION = 1
 * see Rte_Types.h
 *
 * Notes: RTE_MODE_ComMMode_* mode declarations are numbered automatically by
 * the RTE according to the lexical sorting of the symbols. */
/* Deviation MISRAC2012-1 */
STATIC CONST(Rte_ModeType_ComMMode, COMM_CONST) ComM_ComModeToRteMode[] =
{
  RTE_MODE_ComMMode_NO_COMMUNICATION,
  RTE_MODE_ComMMode_SILENT_COMMUNICATION,
  RTE_MODE_ComMMode_FULL_COMMUNICATION
};
#endif /* (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR32) */

#define COMM_STOP_SEC_CONST_UNSPECIFIED
#include <ComM_MemMap.h>

#endif /* (COMM_DEFAULT_ASR_SERVICE_API != COMM_SERVICE_API_NONE) */
#endif /* (COMM_INCLUDE_RTE == STD_ON) */

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

#define COMM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <ComM_MemMap.h>

#if (COMM_CURRENTCHANNELREQUEST_ENABLED == STD_ON)
#if (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR40)
/** \brief state information for ComM_CurrentChannelRequest RTE interface
 *
 * this data must be global so that interfering calls of ComM_RequestComMode
 * for a channel will all signal a consistent state
 */
/* Deviation MISRAC2012-1 */
STATIC VAR(ComM_UserHandleArrayType, COMM_VAR) ComM_FullComRequestors[COMM_NUM_CHANNELS];
#endif

#if (COMM_ENABLE_ASR40_SERVICE_API == STD_ON)
/** \brief ASR40 specific state information for ComM_CurrentChannelRequest RTE interface
 *
 * this data must be global so that interfering calls of ComM_RequestComMode
 * for a channel will all signal a consistent state. If the default service API is set
 * to AUTOSAR_40 then it must be held consistent with Com_FullComRequestors as well.
 */
/* Deviation MISRAC2012-1 */
STATIC VAR(ComM_ASR40_UserHandleArrayType, COMM_VAR)
  ComM_ASR40_FullComRequestors[COMM_NUM_CHANNELS];
#endif

#endif

#define COMM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <ComM_MemMap.h>

/*==================[internal function definitions]==============================================*/

#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>

FUNC(void, COMM_CODE) ComM_Enter_SCHM_COMM_EXCLUSIVE_AREA_0 (void)
{
  /* SchM function has no return value */

  DBG_COMM_ENTER_SCHM_COMM_EXCLUSIVE_AREA_0_ENTRY();
  SchM_Enter_ComM_SCHM_COMM_EXCLUSIVE_AREA_0();
  DBG_COMM_ENTER_SCHM_COMM_EXCLUSIVE_AREA_0_EXIT();
}

FUNC(void, COMM_CODE) ComM_Exit_SCHM_COMM_EXCLUSIVE_AREA_0 (void)
{
  /* SchM function has no return value */
  DBG_COMM_EXIT_SCHM_COMM_EXCLUSIVE_AREA_0_ENTRY();
  SchM_Exit_ComM_SCHM_COMM_EXCLUSIVE_AREA_0();
  DBG_COMM_EXIT_SCHM_COMM_EXCLUSIVE_AREA_0_EXIT();
}

/*------------------[ComM_EvaluateChannel]----------------------------------*/

FUNC(void, COMM_CODE) ComM_EvaluateChannel
(
  uint8 Channel
)
{
  const uint8_least NumUser = COMM_NUM_USERS_OF_CHANNEL(Channel);
#if (COMM_NUM_PNC > 0)
  const uint8_least NumPncs = COMM_NUM_PNCS_OF_CHANNEL(Channel);
#endif
  uint8               ElementPos;
  ComM_ASR40_ModeType ComMode = COMM_NO_COMMUNICATION;
#if (COMM_PNC_GW_ENABLED == STD_ON)
  ComM_ASR40_ModeType ComModeWithoutPnc = COMM_NO_COMMUNICATION;
#endif /* (COMM_PNC_GW_ENABLED == STD_ON) */

#if (COMM_CURRENTCHANNELREQUEST_ENABLED == STD_ON)
#if (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR40)
  CONSTP2VAR(ComM_UserHandleArrayType, AUTOMATIC, COMM_VAR) FullComReq =
    &ComM_FullComRequestors[Channel];
#endif
#if (COMM_ENABLE_ASR40_SERVICE_API == STD_ON)
  CONSTP2VAR(ComM_ASR40_UserHandleArrayType, AUTOMATIC, COMM_VAR) FullASR40ComReq =
    &ComM_ASR40_FullComRequestors[Channel];
#endif
#endif

/* In this compilation unit we can call the SchM functions without using the
 * wrapper functions above.
 */
  SchM_Enter_ComM_SCHM_COMM_EXCLUSIVE_AREA_0();

#if (COMM_CURRENTCHANNELREQUEST_ENABLED == STD_ON)
#if (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR40)
  FullComReq->numberOfRequesters = 0U;
#endif

#if (COMM_ENABLE_ASR40_SERVICE_API == STD_ON)
  FullASR40ComReq->numberOfRequesters = 0U;
#endif
#endif

  /* Search highest mode requested by users of a channel */
  for (ElementPos = 0U; ElementPos < NumUser; ElementPos++)
  {
    const uint8 UserOfChannel = COMM_USER_OF_CHANNEL(Channel, ElementPos);
    const ComM_ASR40_ModeType UserReqMode = ComM_EB_GetUserRequestedComMode(UserOfChannel);

    /* !LINKSTO ComM092,1 */
    if (ComMode < UserReqMode)
    {
      ComMode = UserReqMode;
    }
#if (COMM_PNC_GW_ENABLED == STD_ON)
    if (ComModeWithoutPnc < UserReqMode)
    {
      ComModeWithoutPnc = UserReqMode;
    }
#endif /* (COMM_PNC_GW_ENABLED == STD_ON) */

#if (COMM_CURRENTCHANNELREQUEST_ENABLED == STD_ON)
    if (UserReqMode == COMM_FULL_COMMUNICATION)
    {
#if (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR40)
      FullComReq->handleArray[FullComReq->numberOfRequesters] = UserOfChannel;
      FullComReq->numberOfRequesters++;
#endif
#if (COMM_ENABLE_ASR40_SERVICE_API == STD_ON)
      FullASR40ComReq->handleArray[FullASR40ComReq->numberOfRequesters] = UserOfChannel;
      FullASR40ComReq->numberOfRequesters++;
#endif
    }
#endif
  }

#if (COMM_NUM_PNC > 0)
#if(COMM_0_PNC_VECTOR_AVOIDANCE == STD_ON)
  /* !LINKSTO SWS_ComM_00966,1 */
  if (ComM_PncZeroVectorAvoidanceRelease[Channel] == TRUE)
  {
    for (ElementPos = 0U; ElementPos < NumPncs; ElementPos++)
    {
      const ComM_ASR40_ModeType PncReqMode = ComM_EB_GetPncRequestedComMode0PncVectorAvoidance(Channel, ElementPos);
      if (ComMode < PncReqMode)
      {
        ComMode = PncReqMode;
      }
    }
  }
  else
#endif
  {
    for (ElementPos = 0U; ElementPos < NumPncs; ElementPos++)
    {
      const ComM_ASR40_ModeType PncReqMode = ComM_EB_GetPncRequestedComMode(Channel, ElementPos);
      if (ComMode < PncReqMode)
      {
        ComMode = PncReqMode;
      }
    }
  }
#endif /* (COMM_NUM_PNC > 0) */

  /* Set global variable for most recently requested external mode of the
   * ComM */
  /* !LINKSTO ComM092,1 */
#if (COMM_PNC_GW_ENABLED == STD_ON)
  ComM_EB_SetRequestedComMode(Channel, ComMode, ComModeWithoutPnc);
#else /* (COMM_PNC_GW_ENABLED == STD_ON) */
  ComM_EB_SetRequestedComMode(Channel, ComMode);
#endif /* (COMM_PNC_GW_ENABLED == STD_ON) */

  SchM_Exit_ComM_SCHM_COMM_EXCLUSIVE_AREA_0();

  /* signal RTE */
  /* !LINKSTO ComM.RteNrOfRequesters,1 */
#if (COMM_CURRENTCHANNELREQUEST_ENABLED == STD_ON)
#if (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR40)
  if (ComM_RteWriteCbk[Channel] != NULL_PTR)
  {
    ComM_RteWriteCbk[Channel](FullComReq);
  }
#endif
#if (COMM_ENABLE_ASR40_SERVICE_API == STD_ON)
  if (ComM_ASR40_RteWriteCbk[Channel] != NULL_PTR)
  {
    ComM_ASR40_RteWriteCbk[Channel](FullASR40ComReq);
  }
#endif
#endif
}

#if (COMM_INCLUDE_RTE == STD_ON)
#if (COMM_DEFAULT_ASR_SERVICE_API != COMM_SERVICE_API_NONE)
/* !LINKSTO ComM091,1 */
FUNC(void, COMM_CODE) ComM_RteNotify(
  uint8               User,
  ComM_ASR40_ModeType CurComMMode
)
{
#if (COMM_NUM_USERS != 0U)
  /* Report the mode to RTE, if the current mode was not yet reported to
   * the Rte for this user */
  const Rte_ModeType_ComMMode newRteMode = ComM_ComModeToRteMode[CurComMMode];

  ComM_RteNotificationSuccessful[User] = TRUE;

  if (ComM_RteModeUser[User] != CurComMMode)
  {
    const Std_ReturnType rteReturnVal = ComM_RteSwitchCbk[User](newRteMode);

    if (E_OK == rteReturnVal)
    {
      ComM_RteModeUser[User] = CurComMMode;
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

#endif /* (COMM_DEFAULT_ASR_SERVICE_API != COMM_SERVICE_API_NONE) */
#endif /* (COMM_INCLUDE_RTE == STD_ON) */

#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>

/*==================[external function definitions]==============================================*/

#if (COMM_INCLUDE_RTE == STD_ON)
#if (COMM_DEFAULT_ASR_SERVICE_API != COMM_SERVICE_API_NONE)

#if (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR32)

#define ComM_START_SEC_CODE
#include <ComM_MemMap.h>

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) \
     || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
/* !LINKSTO ComM.EB.ASR32.ComM110,1 */
/* Deviation MISRAC2012-2 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_GetInhibitionStatus
(
  NetworkHandleType                                           pdav0,
  P2VAR(ComM_InhibitionStatusType, AUTOMATIC, COMM_APPL_DATA) Status
)
{
  Std_ReturnType RetVal;
  DBG_COMM_RTE_GETINHIBITIONSTATUS_ENTRY(pdav0,Status);
  /* ComM_InhibitionStatusType and ComM_ASR32_InhibitionStatusType map to the
   * same basic type, thus a simple type cast is sufficient.
   */
  RetVal = ComM_ASR32_GetInhibitionStatus(pdav0,
   (P2VAR(ComM_ASR32_InhibitionStatusType, AUTOMATIC, COMM_APPL_DATA)) Status);
  DBG_COMM_RTE_GETINHIBITIONSTATUS_EXIT(RetVal,pdav0,Status);
  return RetVal;
}
#endif

#if (COMM_NUM_USERS != 0U)
/* !LINKSTO ComM.EB.ASR32.ComM110,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_RequestComMode
(
  uint8           pdav0,
  ComM_ModeType   ComMode
)
{
  Std_ReturnType ret;
  /* ComM_ModeType and ComM_ASR32_ModeType map to the
     same basic type, thus a simple type cast is sufficient. */
  DBG_COMM_RTE_REQUESTCOMMODE_ENTRY(pdav0, ComMode);

  ret = ComM_ASR32_RequestComMode(pdav0, (ComM_ASR32_ModeType) ComMode);

  DBG_COMM_RTE_REQUESTCOMMODE_EXIT(ret, pdav0, ComMode);
  return ret;
}

/* !LINKSTO ComM.EB.ASR32.ComM110,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_GetMaxComMode
(
  uint8                                           pdav0,
  P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
  Std_ReturnType ret;
  /* ComM_ModeType and ComM_ASR32_ModeType map to the
     same basic type, thus a simple type cast is sufficient. */

  DBG_COMM_RTE_GETMAXCOMMODE_ENTRY(pdav0, ComMode);

  ret = ComM_ASR32_GetMaxComMode(pdav0,
      (P2VAR(ComM_ASR32_ModeType, AUTOMATIC, COMM_APPL_DATA)) ComMode);

  DBG_COMM_RTE_GETMAXCOMMODE_EXIT(ret, pdav0, ComMode);

  return ret;
}

/* !LINKSTO ComM.EB.ASR32.ComM110,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_GetRequestedMode
(
  uint8                                           pdav0,
  P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
  Std_ReturnType ret;
  /* ComM_ModeType and ComM_ASR32_ModeType map to the
     same basic type, thus a simple type cast is sufficient. */

  DBG_COMM_RTE_GETREQUESTEDMODE_ENTRY(pdav0, ComMode);

  ret = ComM_ASR32_GetRequestedMode(pdav0,
      (P2VAR(ComM_ASR32_ModeType, AUTOMATIC, COMM_APPL_DATA)) ComMode);

  DBG_COMM_RTE_GETREQUESTEDMODE_EXIT(ret, pdav0, ComMode);

  return ret;
}

/* !LINKSTO ComM.EB.ASR32.ComM110,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_GetCurrentComMode
(
  uint8                                           pdav0,
  P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
  Std_ReturnType ret;
  /* ComM_ModeType and ComM_ASR32_ModeType map to the
     same basic type, thus a simple type cast is sufficient. */
  DBG_COMM_RTE_GETCURRENTCOMMODE_ENTRY(pdav0, ComMode);

  ret = ComM_ASR32_GetCurrentComMode(pdav0,
      (P2VAR(ComM_ASR32_ModeType, AUTOMATIC, COMM_APPL_DATA)) ComMode);

  DBG_COMM_RTE_GETCURRENTCOMMODE_EXIT(ret, pdav0, ComMode);

  return ret;
}

#endif /* #if (COMM_NUM_USERS != 0U) */

#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
/* !LINKSTO ComM.EB.ASR32.ComM110,1 */
/* Deviation MISRAC2012-2 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_PreventWakeUp
(
  NetworkHandleType pdav0,
  boolean           Status
)
{
  Std_ReturnType ret;
  DBG_COMM_RTE_PREVENTWAKEUP_ENTRY(pdav0, Status);
  ret = ComM_ASR32_PreventWakeUp(pdav0, Status);
  DBG_COMM_RTE_PREVENTWAKEUP_EXIT(ret, pdav0, Status);
  return ret;
}
#endif

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
/* !LINKSTO ComM.EB.ASR32.ComM110,1 */
/* Deviation MISRAC2012-2 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_LimitChannelToNoComMode
(
  NetworkHandleType pdav0,
  boolean           Status
)
{
  Std_ReturnType ret;
  DBG_COMM_RTE_LIMITCHANNELTONOCOMMODE_ENTRY(pdav0, Status);
  ret  = ComM_ASR32_LimitChannelToNoComMode(pdav0, Status);
  DBG_COMM_RTE_LIMITCHANNELTONOCOMMODE_EXIT(ret, pdav0, Status);
  return ret;
}

/* !LINKSTO ComM.EB.ASR32.ComM110,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_LimitECUToNoComMode
(
  boolean Status
)
{
  Std_ReturnType ret;
  DBG_COMM_RTE_LIMITECUTONOCOMMODE_ENTRY(Status);
  ret = ComM_ASR32_LimitECUToNoComMode(Status);
  DBG_COMM_RTE_LIMITECUTONOCOMMODE_EXIT(ret, Status);
  return ret;
}
#endif

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON)           \
     || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
/* !LINKSTO ComM.EB.ASR32.ComM110,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_ReadInhibitCounter
(
  P2VAR(uint16, AUTOMATIC, COMM_APPL_DATA) CounterValue
)
{
  Std_ReturnType ret;
  DBG_COMM_RTE_READINHIBITCOUNTER_ENTRY(CounterValue);
  ret = ComM_ASR32_ReadInhibitCounter(CounterValue);
  DBG_COMM_RTE_READINHIBITCOUNTER_EXIT(ret,CounterValue);
  return ret;
}

/* !LINKSTO ComM.EB.ASR32.ComM110,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_ResetInhibitCounter
(
  void
)
{
  Std_ReturnType ret;
  DBG_COMM_RTE_RESETINHIBITCOUNTER_ENTRY();
  ret = ComM_ASR32_ResetInhibitCounter();
  DBG_COMM_RTE_RESETINHIBITCOUNTER_EXIT(ret);
  return ret;
}

/* !LINKSTO ComM.EB.ASR32.ComM110,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_SetECUGroupClassification
(
  ComM_InhibitionStatusType Status
)
{
  Std_ReturnType ret;
  DBG_COMM_RTE_SETECUGROUPCLASSIFICATION_ENTRY(Status);
  ret = ComM_ASR32_SetECUGroupClassification(Status);
  DBG_COMM_RTE_SETECUGROUPCLASSIFICATION_EXIT(ret, Status);
  return ret;
}
#endif

#define ComM_STOP_SEC_CODE
#include <ComM_MemMap.h>

#elif (COMM_DEFAULT_ASR_SERVICE_API == COMM_SERVICE_API_ASR40)

#define ComM_START_SEC_CODE
#include <ComM_MemMap.h>

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) \
     || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
/* !LINKSTO ComM.EB.ASR32.ComM111,1 */
/* Deviation MISRAC2012-2 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_GetInhibitionStatus
(
  NetworkHandleType                                           pdav0,
  P2VAR(ComM_InhibitionStatusType, AUTOMATIC, COMM_APPL_DATA) Status
)
{
  Std_ReturnType ret;
  DBG_COMM_RTE_GETINHIBITIONSTATUS_ENTRY(pdav0, Status);
  ret = ComM_ASR40_GetInhibitionStatus(pdav0, Status);
  DBG_COMM_RTE_GETINHIBITIONSTATUS_EXIT(ret, pdav0, Status);
  return ret;
}
#endif

#if (COMM_NUM_USERS != 0U)
/* !LINKSTO ComM.EB.ASR32.ComM111,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_RequestComMode
(
  ComM_UserHandleType pdav0,
  ComM_ModeType       ComMode
)
{
  Std_ReturnType ret;
  DBG_COMM_RTE_REQUESTCOMMODE_ENTRY(pdav0, ComMode);
  ret = ComM_ASR40_RequestComMode(pdav0, ComMode);
  DBG_COMM_RTE_REQUESTCOMMODE_EXIT(ret, pdav0, ComMode);
  return ret;
}

/* !LINKSTO ComM.EB.ASR32.ComM111,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_GetMaxComMode
(
  ComM_UserHandleType                             pdav0,
  P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
  Std_ReturnType ret;
  DBG_COMM_RTE_GETMAXCOMMODE_ENTRY(pdav0, ComMode);
  ret = ComM_ASR40_GetMaxComMode(pdav0, ComMode);
  DBG_COMM_RTE_GETMAXCOMMODE_EXIT(ret, pdav0, ComMode);
  return ret;
}

/* !LINKSTO ComM.EB.ASR32.ComM111,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_GetRequestedComMode
(
  ComM_UserHandleType                             pdav0,
  P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
  Std_ReturnType ret;
  DBG_COMM_RTE_GETREQUESTEDCOMMODE_ENTRY(pdav0, ComMode);
  ret = ComM_ASR40_GetRequestedComMode(pdav0, ComMode);
  DBG_COMM_RTE_GETREQUESTEDCOMMODE_EXIT(ret, pdav0, ComMode);
  return ret;
}

/* !LINKSTO ComM.EB.ASR32.ComM111,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_GetCurrentComMode
(
  ComM_UserHandleType                             pdav0,
  P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
  Std_ReturnType ret;
  DBG_COMM_RTE_GETCURRENTCOMMODE_ENTRY(pdav0, ComMode);
  ret = ComM_ASR40_GetCurrentComMode(pdav0, ComMode);
  DBG_COMM_RTE_GETCURRENTCOMMODE_EXIT(ret, pdav0, ComMode);
  return ret;
}

FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_GetCurrentPNCComMode
(
  ComM_UserHandleType                             pdav0,
  P2VAR(ComM_ModeType, AUTOMATIC, COMM_APPL_DATA) ComMode
)
{
  Std_ReturnType ret;
  DBG_COMM_RTE_GETCURRENTPNCCOMMODE_ENTRY(pdav0, ComMode);
  ret = ComM_ASR40_GetCurrentPNCComMode(pdav0, ComMode);
  DBG_COMM_RTE_GETCURRENTPNCCOMMODE_EXIT(ret, pdav0, ComMode);
  return ret;
}
#endif /* #if (COMM_NUM_USERS != 0U) */
#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
/* !LINKSTO ComM.EB.ASR32.ComM111,1 */
/* Deviation MISRAC2012-2 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_PreventWakeUp
(
  NetworkHandleType pdav0,
  boolean           Status
)
{
  Std_ReturnType ret;
  DBG_COMM_RTE_PREVENTWAKEUP_ENTRY(pdav0, Status);
  ret = ComM_ASR40_PreventWakeUp(pdav0, Status);
  DBG_COMM_RTE_PREVENTWAKEUP_EXIT(ret, pdav0, Status);
  return ret;
}
#endif

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
/* !LINKSTO ComM.EB.ASR32.ComM111,1 */
/* Deviation MISRAC2012-2 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_LimitChannelToNoComMode
(
  NetworkHandleType pdav0,
  boolean           Status
)
{
  Std_ReturnType ret;
  DBG_COMM_RTE_LIMITCHANNELTONOCOMMODE_ENTRY(pdav0, Status);
  ret = ComM_ASR40_LimitChannelToNoComMode(pdav0, Status);
  DBG_COMM_RTE_LIMITCHANNELTONOCOMMODE_EXIT(ret, pdav0, Status);
  return ret;
}

/* !LINKSTO ComM.EB.ASR32.ComM111,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_LimitECUToNoComMode
(
  boolean Status
)
{
  Std_ReturnType ret;
  DBG_COMM_RTE_LIMITECUTONOCOMMODE_ENTRY(Status);
  ret = ComM_ASR40_LimitECUToNoComMode(Status);
  DBG_COMM_RTE_LIMITECUTONOCOMMODE_EXIT(ret, Status);
  return ret;
}
#endif

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON)           \
     || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))
/* !LINKSTO ComM.EB.ASR32.ComM111,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_ReadInhibitCounter
(
  P2VAR(uint16, AUTOMATIC, COMM_APPL_DATA) CounterValue
)
{
  Std_ReturnType ret;
  DBG_COMM_RTE_READINHIBITCOUNTER_ENTRY(CounterValue);
  ret = ComM_ASR40_ReadInhibitCounter(CounterValue);
  DBG_COMM_RTE_READINHIBITCOUNTER_EXIT(ret, CounterValue);
  return ret;
}

/* !LINKSTO ComM.EB.ASR32.ComM111,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_ResetInhibitCounter
(
  void
)
{
  Std_ReturnType ret;
  DBG_COMM_RTE_RESETINHIBITCOUNTER_ENTRY();
  ret = ComM_ASR40_ResetInhibitCounter();
  DBG_COMM_RTE_RESETINHIBITCOUNTER_EXIT(ret);
  return ret;
}

/* !LINKSTO ComM.EB.ASR32.ComM111,1, ComM.RteTypes_Implicit3,1 */
FUNC(Std_ReturnType, COMM_CODE) ComM_Rte_SetECUGroupClassification
(
  ComM_InhibitionStatusType Status
)
{
  Std_ReturnType ret;
  DBG_COMM_RTE_SETECUGROUPCLASSIFICATION_ENTRY(Status);
  ret = ComM_ASR40_SetECUGroupClassification(Status);
  DBG_COMM_RTE_SETECUGROUPCLASSIFICATION_EXIT(ret, Status);
  return ret;
}
#endif

#define ComM_STOP_SEC_CODE
#include <ComM_MemMap.h>

#endif

#endif /* (COMM_DEFAULT_ASR_SERVICE_API != COMM_SERVICE_API_NONE) */
#endif /* (COMM_INCLUDE_RTE == STD_ON) */

/*==================[end of file]================================================================*/
