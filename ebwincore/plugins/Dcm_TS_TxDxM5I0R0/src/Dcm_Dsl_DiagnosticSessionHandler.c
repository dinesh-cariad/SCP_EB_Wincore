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

/* !LINKSTO Dcm.Dsn.File.DiagnosticSessionHandler.Impl,1 */
/* This file contains the implementation of the Diagnostic Session Handler software unit. */

 /* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * This warnings is a false positive.
 * The compiler optimizes the 'for' loop (which has only 1 iteration) in the following way
 * - it prepares in advance the index for the next iteration
 * - if the index is equal to '0' it goes back to check the 'for' loop conditions, otherwise it just moves on
 * - since the index is already equal to '1', it never jumps back to check the exit condition
 */
/*==================[includes]===================================================================*/

#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <TSMem.h>                                               /* EB specific memory functions */

#include <Dcm_Trace.h>
#include <Dcm_Int.h>                                             /* Internal declarations of Dcm */
#include <Dcm_HsmDcm.h>                                   /* public statechart model definitions */
#include <Dcm.h>
#include <Dcm_Dsl_Supervisor.h>
#include <Dcm_Dsl_DiagnosticSessionHandler.h>  /* Dcm Dsl Diagnostic Session Handler header file */
#include <Dcm_Dsl_Supervisor.h>
#include <Dcm_Dsl_Supervisor_ProtocolHandling.h>
#include <TSAutosar.h>                                     /* Only for the TS_PARAM_UNUSED macro */
#include <ComM_Dcm.h>                                                  /* ComM interface for Dcm */
#include <SchM_Dcm.h>                                                  /* SchM interface for Dcm */
#if (DCM_BSWM_API_ENABLED == STD_ON)
#include <BswM_DCM.h>                                                  /* BswM interface for Dcm */
#endif

#include <Dcm_Dsl_CommunicationServices.h>
#include <Dcm_Dsl_SecurityLevelHandler.h>

#if (DCM_DSP_USE_SERVICE_0X27 == STD_ON)
#include <Dcm_Dsp_SvcH_SecurityAccess.h>
#endif

#if (DCM_DSP_USE_ROESERVICES == STD_ON)
#include <Dcm_Dsl_RoeServices.h>
#endif

#if (DCM_DSP_USE_SERVICE_0X85 == STD_ON)
#include <Dcm_Dsp_SvcH_ControlDTCSetting.h>
#endif

#if(DCM_DSP_USE_SERVICE_0X2F == STD_ON)
#include <Dcm_Dsp_SvcH_InputOutputControlByIdentifier.h>
#endif

#if (DCM_DSP_USE_SERVICE_0X28 == STD_ON)
#include <Dcm_Dsp_SvcH_CommunicationControl.h>
#endif

#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
#include <Dcm_Dsp_SvcH_ReadDataByPeriodicIdentifier.h>
#endif

#if ( (DCM_DSP_USE_SERVICE_0X03 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X07 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X0A == STD_ON) )
#include <Dcm_Dsp_DemFacade.h>
#endif

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief This function switches the current diagnostic session to the new session.

 ** \param[in]    NewSession identifies the new session which shall be set.
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_DoSwitch(
  Dcm_SesCtrlType NewSession);

/** \brief This function notifies all affected units about the session change.

 ** \param[in]    OldSession identifies the previous diagnostic session.
 ** \param[in]    NewSession identifies the session which has been set.
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_UponSessionChangeNotify(
  Dcm_SesCtrlType OldSession, Dcm_SesCtrlType NewSession);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/* Dcm_DiagnosticSessionResetFlag indicates whether transition to default session is requested */
STATIC VAR(boolean, DCM_VAR) Dcm_DiagnosticSessionResetFlag;

/* Dcm_DiagnosticSessionProtocolIndex stores protocol index of service 0x10 request
 * if service 0x10 is processed, DCM_INVALID_PROTOCOL_INDEX otherwise */
STATIC VAR(Dcm_ProtocolIndexType, DCM_VAR) Dcm_DiagnosticSessionProtocolIndex;

/* Dcm_DiagnosticSessionTableIndex stores index to diagnostic session if service 0x10
 * is processed, 0U otherwise */
STATIC VAR(uint8, DCM_VAR) Dcm_DiagnosticSessionTableIndex;

/* Dcm_DiagnosticSessionMainConnectionIndex stores index of MainConnection
 * which changed the session to non-default */
STATIC VAR(Dcm_MainConnectionIndexType, DCM_VAR) Dcm_DiagnosticSessionMainConnectionIndex;

/* !LINKSTO Dcm022,1 */
/* Dcm_DiagnosticSessionLevel stores current diagnostic session */
STATIC VAR(Dcm_SesCtrlType, DCM_VAR) Dcm_DiagnosticSessionLevel;

/* Dcm_ResetToDefaultSessionOccured[ServiceID] indicates whether reset to default session
 * was executed during service has been processed.
 * Supported services are:
 *             - DCM_SID_SESSION_CONTROL for UDS Service DiagnosticSessionControl (0x10)
 *             - DCM_SID_COMMUNICATIONCONTROL for UDS Service CommunicationControl (0x28) and
 *             - DCM_SID_SECURITYACCESS for UDS Service SecurityAccess (0x27)
 */
STATIC VAR(boolean, DCM_VAR) Dcm_ResetToDefaultSessionOccured[DCM_NUM_FLAGSFORSVCID];

/* Dcm_ResetToDefaultSessionRequester indicates if supervisor requested reset to default
   session */
STATIC VAR(Dcm_RequesterIdType, DCM_VAR) Dcm_ResetToDefaultSessionRequester;

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_16
#include <Dcm_MemMap.h>

/* Owner of P2timer */
STATIC VAR(Dcm_BufferUserType, DCM_VAR) Dcm_P2TimerOwner[DCM_NUM_PROTOCOLS];

/* Session timer value */
/* !LINKSTO Dcm.EB.SessionStateHandling.S3Server.Handling,1 */
STATIC VAR(Dcm_S3TimerValueType, DCM_VAR) Dcm_S3TimerValue;

#define DCM_STOP_SEC_VAR_CLEARED_16
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_32
#include <Dcm_MemMap.h>

/* Protocol timer value */
STATIC VAR(Dcm_TimerValueType, DCM_VAR) Dcm_P2TimerValue[DCM_NUM_PROTOCOLS];

#define DCM_STOP_SEC_VAR_CLEARED_32
#include <Dcm_MemMap.h>

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.Init,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_Init(void)
{
  Dcm_ProtocolIndexType  ProtocolIdx;
  uint8 FlagIdx;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_INIT_ENTRY();

  Dcm_DiagnosticSessionResetFlag = FALSE;

  Dcm_DiagnosticSessionProtocolIndex = DCM_INVALID_PROTOCOL_INDEX;
  Dcm_DiagnosticSessionTableIndex = 0U;
  Dcm_DiagnosticSessionMainConnectionIndex = DCM_INVALID_MAINCONNECTION_INDEX;

  /* !LINKSTO Dcm034,1 */
  Dcm_DiagnosticSessionLevel = DCM_DEFAULT_SESSION;

  Dcm_S3TimerValue = 0U;

  /* Deviation TASKING-1 */
  for (ProtocolIdx = 0U; ProtocolIdx < DCM_NUM_PROTOCOLS; ProtocolIdx++)
  {
    Dcm_P2TimerValue[ProtocolIdx] = 0U;
    Dcm_P2TimerOwner[ProtocolIdx] = DCM_DIAGSESSIONHANDLER_ID;
  }

  Dcm_ResetToDefaultSessionRequester = DCM_RESETTODEFAULTSESSIONREQUESTER_UNDEFINED;

  for (FlagIdx = 0U; FlagIdx < DCM_NUM_FLAGSFORSVCID; FlagIdx++)
  {
    Dcm_ResetToDefaultSessionOccured[FlagIdx] = FALSE;
  }

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_INIT_EXIT();
}


/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.S3TimerHandlingMainFunction,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_S3TimerHandlingMainFunction(void)
{
  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_S3TIMERHANDLINGMAINFUNCTION_ENTRY();

  /* enter critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  /* process session timer */
  if (Dcm_S3TimerValue > 0U)
  {
    /* session timer is running */
    if (Dcm_S3TimerValue == 1U)
    {
      /* session timer is expired */
      if (Dcm_DiagnosticSessionResetFlag == FALSE)
      {
        /* !LINKSTO Dcm.EB.SessionStateHandling.S3Timeout.ResetToDefaultSession.ResetToDefaultSession,1 */
        Dcm_DiagnosticSessionResetFlag = TRUE;
      }

#if (DCM_S3TIMEOUTNOTIFICATION == STD_ON)
      /* !LINKSTO Dcm.EB.SessionStateHandling.Enabled.Notification,1 */
      Dcm_S3TimeoutNotification();
#endif
    }
    --Dcm_S3TimerValue;
  }

  /* exit critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  if (Dcm_DiagnosticSessionResetFlag == TRUE)
  {
    Dcm_Dsl_DiagnosticSessionHandler_DoSwitch(DCM_DEFAULT_SESSION);
  }

  /* reset flag */
  Dcm_ResetToDefaultSessionRequester = DCM_RESETTODEFAULTSESSIONREQUESTER_UNDEFINED;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_S3TIMERHANDLINGMAINFUNCTION_EXIT();
}

/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.P2TimerHandlingMainFunction,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_P2TimerHandlingMainFunction(void)
{
  Dcm_ProtocolIndexType  ProtocolIdx;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_P2TIMERHANDLINGMAINFUNCTION_ENTRY();

  /* enter critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  /* process protocol timers */
  /* Deviation TASKING-1 */
  for (ProtocolIdx = 0U; ProtocolIdx < DCM_NUM_PROTOCOL; ProtocolIdx++)
  {
    /* P2timer is running */
    if (Dcm_P2TimerValue[ProtocolIdx] > 0U)
    {
      --Dcm_P2TimerValue[ProtocolIdx];

      /* P2timer is expired */
      if ( 0U == Dcm_P2TimerValue[ProtocolIdx] )
      {
        if ( DCM_SUPERVISOR_ID != Dcm_P2TimerOwner[ProtocolIdx] )
        {
#if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || (DCM_DSP_USE_SERVICE_0X02 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X03 == STD_ON) || (DCM_DSP_USE_SERVICE_0X06 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X07 == STD_ON) || (DCM_DSP_USE_SERVICE_0X08 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X0A == STD_ON) )
          /* get the context information for the current instance */
          CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo =
            &(DCM_HSM_INFO(ProtocolIdx));

          /* The hsm data curHsmInfo->serviceId is valid if the request has been dispatched
             in previous MainFunction() cycle */
          uint8 serviceId = curHsmInfo->serviceId;

          if( (serviceId == DCM_DSP_OBD_SERVICE_0X01) || (serviceId == DCM_DSP_OBD_SERVICE_0X02) ||
              (serviceId == DCM_DSP_OBD_SERVICE_0X03) || (serviceId == DCM_DSP_OBD_SERVICE_0X06) ||
              (serviceId == DCM_DSP_OBD_SERVICE_0X07) || (serviceId == DCM_DSP_OBD_SERVICE_0X08) ||
              (serviceId == DCM_DSP_OBD_SERVICE_0X0A) )
          {
            /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x01RequestDispatched.P2Timeout.Discard,1 */
            /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x02RequestDispatched.P2Timeout.Discard,1 */
            /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x06RequestDispatched.P2Timeout.Discard,1 */
            /* !LINKSTO Dcm.EB.ResponseHandling.OBD0x08RequestDispatched.P2Timeout.Discard,1 */
            /* !LINKSTO Dcm.EB.OBDSvc0x03.GetNumberOfFilteredDTC.P2Timeout,1 */
            /* !LINKSTO Dcm.EB.OBDSvc0x07.GetNumberOfFilteredDTC.P2Timeout,1 */
            /* !LINKSTO Dcm.EB.OBDSvc0x0A.GetNumberOfFilteredDTC.P2Timeout,1 */
            /* !LINKSTO Dcm.EB.OBDSvc0x03.GetNextFilteredDTC.P2Timeout,1 */
            /* !LINKSTO Dcm.EB.OBDSvc0x07.GetNextFilteredDTC.P2Timeout,1 */
            /* !LINKSTO Dcm.EB.OBDSvc0x0A.GetNextFilteredDTC.P2Timeout,1 */
            /* Emit the event CANCEL_PROCESSING */
            (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(ProtocolIdx),
                                          DCM_HSM_DCM_EV_CANCEL_PROCESSING);
          }
          else
#endif /* #if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || (DCM_DSP_USE_SERVICE_0X02 == STD_ON) || \
                (DCM_DSP_USE_SERVICE_0X03 == STD_ON) || (DCM_DSP_USE_SERVICE_0X06 == STD_ON) || \
                (DCM_DSP_USE_SERVICE_0X07 == STD_ON) || (DCM_DSP_USE_SERVICE_0X08 == STD_ON) || \
                (DCM_DSP_USE_SERVICE_0X0A == STD_ON) ) */
          {
            Dcm_Dsp_Protocol_P2TimerTimeout(ProtocolIdx);
          }
        }
        else
        {
          Dcm_Dsl_Supervisor_P2TimerTimeout(ProtocolIdx);
        }
      }
    }
  }
  /* exit critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_P2TIMERHANDLINGMAINFUNCTION_EXIT();
}

/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.GetSessionMaxRcrrp,1 */
FUNC(uint16, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_GetSessionMaxRcrrp(
  Dcm_ProtocolIndexType ProtocolIndex)
{
  uint16 ReturnMaxRcrrp;
#if (DCM_DIAG_SES_RESP_PEND_ENABLED == STD_ON)
  P2CONST(Dcm_SesTabConfigType, AUTOMATIC, DCM_CONST) SesTabConfig =
                                                               &DCM_SES_TAB_CONFIG(ProtocolIndex);
  P2CONST(Dcm_SesTabEntryConfigType, AUTOMATIC, DCM_CONST) SessionEntry;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETSESSIONMAXRCRRP_ENTRY(ProtocolIndex);

  /* enter critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  if (ProtocolIndex == Dcm_DiagnosticSessionProtocolIndex)
  {
    /* protocol can be in any session */
    SessionEntry = &(SesTabConfig->SessionEntries[Dcm_DiagnosticSessionTableIndex]);
  }
  else
  {
    /* protocol is in default session */
    SessionEntry = &(SesTabConfig->SessionEntries[SesTabConfig->DefSessionIndex]);
  }

  /* exit critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  ReturnMaxRcrrp = SessionEntry->NumResponsePending;
#else
  TS_PARAM_UNUSED(ProtocolIndex);

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETSESSIONMAXRCRRP_ENTRY(ProtocolIndex);

  ReturnMaxRcrrp = DCM_DIAG_RESP_MAX_NUM_RESP_PEND;
#endif /* #if (DCM_DIAG_SES_RESP_PEND_ENABLED == STD_ON) */

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETSESSIONMAXRCRRP_EXIT(ReturnMaxRcrrp);

  return ReturnMaxRcrrp;
}

/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.GetCurrentSession,1 */
FUNC(Dcm_SesCtrlType, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_GetCurrentSession(void)
{
  Dcm_SesCtrlType SessionLevel;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETCURRENTSESSION_ENTRY();

  SessionLevel = Dcm_DiagnosticSessionLevel;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETCURRENTSESSION_EXIT(SessionLevel);

  return SessionLevel;
}

/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.GetNonDefaultSessionProtocolIndex,1 */
FUNC(Dcm_ProtocolIndexType, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_GetNonDefaultSessionProtocolIndex(void)
{
  Dcm_ProtocolIndexType ProtocolIndex;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETNONDEFAULTSESSIONPROTOCOLINDEX_ENTRY();

  ProtocolIndex = Dcm_DiagnosticSessionProtocolIndex;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETNONDEFAULTSESSIONPROTOCOLINDEX_EXIT(ProtocolIndex);

  return ProtocolIndex;
}

/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.GetNonDefaultSessionMainConnection,1 */
FUNC(Dcm_MainConnectionIndexType, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_GetNonDefaultSessionMainConnection(void)
{
  Dcm_MainConnectionIndexType MainConnectionIndex;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETNONDEFAULTSESSIONMAINCONNECTION_ENTRY();

  MainConnectionIndex = Dcm_DiagnosticSessionMainConnectionIndex;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETNONDEFAULTSESSIONMAINCONNECTION_EXIT(MainConnectionIndex);

  return MainConnectionIndex;
}

/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.ResetToDefaultSession,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_ResetToDefaultSession(
  Dcm_RequesterIdType RequesterId)
{
  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_RESETTODEFAULTSESSION_ENTRY(a);

  /* enter critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  if (RequesterId == DCM_RESETTODEFAULTSESSIONREQUESTER_SUPERVISOR)
  {
    /* set requester */
    Dcm_ResetToDefaultSessionRequester = DCM_RESETTODEFAULTSESSIONREQUESTER_SUPERVISOR;

    /* set flag */
    Dcm_DiagnosticSessionResetFlag = TRUE;
  }
  else
  {
    if (Dcm_DiagnosticSessionResetFlag == FALSE)
    {
      /* get the index of the current protocol */
      Dcm_ProtocolIndexType ProtocolIdx =
                    DCM_PROTOCOL_INDEX_FROM_ID(Dcm_Dsl_Supervisor_GetLatestExternalUDSProtocol());

      if (DCM_INVALID_PROTOCOL_INDEX == ProtocolIdx)
      {
        /* since no uds protocol has been started yet, the current session is the default session */
        DCM_PRECONDITION_ASSERT(DCM_DEFAULT_SESSION ==
                                                 Dcm_DiagnosticSessionLevel, DCM_INTERNAL_API_ID);
        /* do nothing */
      }
      else
      {
        /* !LINKSTO Dcm.EB.SessionStateHandling.ResetToDefaultSessionAPI, 1*/
        /* !LINKSTO Dcm.EB.SessionStateHandling.Operation.ResetToDefaultSession, 1*/
        Dcm_DiagnosticSessionResetFlag = TRUE;
      }
    }
  }

  /* exit critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_RESETTODEFAULTSESSION_EXIT();
}

/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.SetSession,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_SetSession(
  Dcm_MainConnectionIndexType MainConnectionIndex,
  Dcm_SesCtrlType NewSession)
{
  Dcm_ProtocolIndexType ProtocolIndex = Dcm_MainConnectionConfig[MainConnectionIndex].ProtocolIndex;
  P2CONST(Dcm_SesTabConfigType, AUTOMATIC, DCM_CONST) SesTabConfig = &DCM_SES_TAB_CONFIG(ProtocolIndex);
  uint8 SessionIdx;
#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
  uint8 RteSesMode = DCM_RTE_DEFAULTSESSION_MODE;
#endif

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_SETSESSION_ENTRY(MainConnectionIndex,NewSession);

  /* get session index into protocol session table */
  for (SessionIdx = 0U; SessionIdx < SesTabConfig->NumEntries; SessionIdx++)
  {
    if ((SesTabConfig->SessionEntries[SessionIdx]).SesCtrl == NewSession)
    {
      break;
    }
  }

  if (SessionIdx < SesTabConfig->NumEntries)
  {
    /* enter critical section */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    Dcm_DiagnosticSessionProtocolIndex = ProtocolIndex;
    Dcm_DiagnosticSessionTableIndex = SessionIdx;
    if (DCM_DEFAULT_SESSION == NewSession)
    {
      Dcm_DiagnosticSessionMainConnectionIndex = DCM_INVALID_MAINCONNECTION_INDEX;
    }
    else
    {
      Dcm_DiagnosticSessionMainConnectionIndex = MainConnectionIndex;
    }

    /* !LINKSTO Dcm022,1 */
    Dcm_DiagnosticSessionLevel = NewSession;

    /* exit critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
    RteSesMode = (SesTabConfig->SessionEntries[SessionIdx]).RteSesMode;

    /* inform SchM about session change */
    /* !LINKSTO Dcm.EB.DiagnosticSessionControl.SchM_Switch,1 */
    (void)SchM_Switch_Dcm_DcmDiagnosticSessionControl(RteSesMode);
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_ON) */
  }

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_SETSESSION_EXIT();
}
/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.SwitchCurrentSession,1 */
#if( (DCM_DSP_USE_SERVICE_0X10 == STD_ON) || \
     (DCM_EXTSVC_ENABLED == STD_ON) || \
     (DCM_JUMP_TO_BL_ENABLED == STD_ON) )
FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_SwitchCurrentSession(
  Dcm_SesCtrlType NewSession)
{
  boolean IsSessionClearedToSwitch;
  boolean ResetToDefaultSessionOccured;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_SWITCHCURRENTSESSION_ENTRY(NewSession);

  IsSessionClearedToSwitch = Dcm_Dsl_Supervisor_IsSessionClearedToSwitch(NewSession);
  ResetToDefaultSessionOccured =
      Dcm_Dsl_DiagnosticSessionHandler_GetResetToDefaultSessionOccured(DCM_SID_SESSION_CONTROL);

  /* !LINKSTO Dcm.DiagnosticSessionControl.NonBootSession.OBD.ExternalHandler,1 */
  /* !LINKSTO Dcm.DiagnosticSessionControl.NonBootSession.OBD.InternalHandler,1 */
  /* !LINKSTO Dcm.EB.ResetToDefaultSessionDuringSessionControlRequest.External,1 */
  /* !LINKSTO Dcm.EB.ResetToDefaultSessionDuringSessionControlRequest.Internal,1 */
  /* !LINKSTO Dcm.EB.SessionStateHandling.DiagnosticSessionControl.NonBootSession.BswM.ResetToDefaultSession.DiscardSessionType,1 */
  /* !LINKSTO Dcm.EB.SessionStateHandling.DiagnosticSessionControl.NonBootSession.ModeSwitch.ResetToDefaultSession.DiscardSessionType,1 */

  if ((IsSessionClearedToSwitch == DCM_SUPERVISOR_SESSION_SWITCH_ACCEPT) &&
      (FALSE == ResetToDefaultSessionOccured))
  {
    Dcm_Dsl_DiagnosticSessionHandler_DoSwitch(NewSession);
  }
#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
#if (DCM_BSWM_API_ENABLED == STD_ON)
  else
  {
    BswM_Dcm_RequestSessionMode(DCM_DEFAULT_SESSION);
  }
#endif /* #if (DCM_BSWM_API_ENABLED == STD_ON) */
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_SWITCHCURRENTSESSION_EXIT();
}
#endif /* #if( (DCM_DSP_USE_SERVICE_0X10 == STD_ON) || \
     (DCM_EXTSVC_ENABLED == STD_ON) || \
     (DCM_JUMP_TO_BL_ENABLED == STD_ON) ) */

/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.StartP2Timer,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_StartP2Timer(
  Dcm_BufferUserType UserId,
  Dcm_ProtocolIndexType ProtocolIndex,
  Dcm_TimerValueType Timeout)
{
  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_STARTP2TIMER_ENTRY(UserId,ProtocolIndex,Timeout);

  if (0U != Timeout)
  {

    /* only protocol timer owner can start P2Timer */
    DCM_PRECONDITION_ASSERT(UserId == Dcm_P2TimerOwner[ProtocolIndex], DCM_INTERNAL_API_ID);

    /* start P2Timer only if it is not running */
    DCM_PRECONDITION_ASSERT(0U == Dcm_P2TimerValue[ProtocolIndex], DCM_INTERNAL_API_ID);
  }

  Dcm_P2TimerOwner[ProtocolIndex] = UserId;

  /* enter critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  Dcm_P2TimerValue[ProtocolIndex] = Timeout;

  /* exit critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  if ( 0U == Dcm_P2TimerValue[ProtocolIndex] )
  {
    if( DCM_SUPERVISOR_ID != Dcm_P2TimerOwner[ProtocolIndex] )
    {
#if ( (DCM_DSP_USE_SERVICE_0X03 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X07 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X0A == STD_ON) )
      /* get the context information for the current instance */
      CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo =
        &(DCM_HSM_INFO(ProtocolIndex));

      if( (curHsmInfo->serviceId == DCM_DSP_OBD_SERVICE_0X03) ||
          (curHsmInfo->serviceId == DCM_DSP_OBD_SERVICE_0X07) ||
          (curHsmInfo->serviceId == DCM_DSP_OBD_SERVICE_0X0A) )
      {
        /* !LINKSTO Dcm.EB.OBDSvc0x03.GetNumberOfFilteredDTC.P2Timeout,1 */
        /* !LINKSTO Dcm.EB.OBDSvc0x07.GetNumberOfFilteredDTC.P2Timeout,1 */
        /* !LINKSTO Dcm.EB.OBDSvc0x0A.GetNumberOfFilteredDTC.P2Timeout,1 */
        /* !LINKSTO Dcm.EB.OBDSvc0x03.GetNextFilteredDTC.P2Timeout,1 */
        /* !LINKSTO Dcm.EB.OBDSvc0x07.GetNextFilteredDTC.P2Timeout,1 */
        /* !LINKSTO Dcm.EB.OBDSvc0x0A.GetNextFilteredDTC.P2Timeout,1 */
        /* Emit the event CANCEL_PROCESSING */
        (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(ProtocolIndex),
                                      DCM_HSM_DCM_EV_CANCEL_PROCESSING);
      }
      else
#endif /* #if ( (DCM_DSP_USE_SERVICE_0X03 == STD_ON) || \
                (DCM_DSP_USE_SERVICE_0X07 == STD_ON) || \
                (DCM_DSP_USE_SERVICE_0X0A == STD_ON) ) */
      {
        Dcm_Dsp_Protocol_P2TimerTimeout(ProtocolIndex);
      }
    }
    else
    {
      Dcm_Dsl_Supervisor_P2TimerTimeout(ProtocolIndex);
    }
  }

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_STARTP2TIMER_EXIT();
}

/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.StartS3Timer,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_StartS3Timer(
  Dcm_TimerValueType Timeout)
{
  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_STARTS3TIMER_ENTRY(Timeout);

  /* start S3Server only in non-default session */
  DCM_PRECONDITION_ASSERT(DCM_DEFAULT_SESSION !=
                                            Dcm_DiagnosticSessionLevel, DCM_INTERNAL_API_ID);

  /* enter critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  /* !LINKSTO Dcm.EB.SessionStateHandling.S3Server.Value,1 */
  Dcm_S3TimerValue = (Dcm_S3TimerValueType) Timeout;

  /* exit critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_STARTS3TIMER_EXIT();
}


/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.StopTimer,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_StopTimer(
  Dcm_BufferUserType UserId,
  Dcm_ProtocolIndexType ProtocolIndex,
  Dcm_TimerIdType TimerId)
{
  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_STOPTIMER_ENTRY(UserId,ProtocolIndex,TimerId);

  TS_PARAM_UNUSED(UserId);

  switch (TimerId)
  {
    case DCM_P2TIMER:
      DCM_PRECONDITION_ASSERT(UserId == Dcm_P2TimerOwner[ProtocolIndex], DCM_INTERNAL_API_ID);

      /* enter critical section */
      SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

      Dcm_P2TimerValue[ProtocolIndex] = 0U;

      /* exit critical section */
      SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
      break;

    case DCM_S3TIMER:
      DCM_PRECONDITION_ASSERT(DCM_DEFAULT_SESSION !=
                                                 Dcm_DiagnosticSessionLevel, DCM_INTERNAL_API_ID);

      /* enter critical section */
      SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

      Dcm_S3TimerValue = 0U;

      /* exit critical section */
      SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement. */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_STOPTIMER_EXIT();
}

/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.AllocateP2Timer,1*/
FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_AllocateP2Timer(
  Dcm_BufferUserType RequesterId,
  Dcm_ProtocolIndexType ProtocolIndex)
{
  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_ALLOCATEP2TIMER_ENTRY(RequesterId, ProtocolIndex);

  /* allocate P2Timer only if it is not allocated */
  DCM_PRECONDITION_ASSERT(DCM_DIAGSESSIONHANDLER_ID == Dcm_P2TimerOwner[ProtocolIndex],
                          DCM_INTERNAL_API_ID);

  Dcm_P2TimerOwner[ProtocolIndex] = RequesterId;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_ALLOCATEP2TIMER_EXIT();

}

/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.FreeP2Timer,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_FreeP2Timer(
  Dcm_BufferUserType ReleaserId,
  Dcm_ProtocolIndexType ProtocolIndex)
{
   DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_FREEP2TIMER_ENTRY(ReleaserId, ProtocolIndex);

  DCM_PRECONDITION_ASSERT(ReleaserId == Dcm_P2TimerOwner[ProtocolIndex], DCM_INTERNAL_API_ID);

  TS_PARAM_UNUSED(ReleaserId);

  Dcm_P2TimerOwner[ProtocolIndex] = DCM_DIAGSESSIONHANDLER_ID;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_FREEP2TIMER_EXIT();

}

/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.GiveP2Timer,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_GiveP2Timer(
  Dcm_BufferUserType ProviderId,
  Dcm_BufferUserType ReceiverId,
  Dcm_ProtocolIndexType ProtocolIndex)
{
  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GIVEP2TIMER_ENTRY(ProviderId,ReceiverId,ProtocolIndex);

  DCM_PRECONDITION_ASSERT(ProviderId == Dcm_P2TimerOwner[ProtocolIndex], DCM_INTERNAL_API_ID);
  TS_PARAM_UNUSED(ProviderId);

  Dcm_P2TimerOwner[ProtocolIndex] = ReceiverId;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GIVEP2TIMER_EXIT();
}


/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.GetTimerValue,1 */
FUNC(Dcm_TimerValueType, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_GetTimerValue(
  Dcm_ProtocolIndexType ProtocolIndex,
  Dcm_TimerIdType TimerId)
{
  Dcm_TimerValueType TimerValue;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETTIMERVALUE_ENTRY(ProtocolIndex,TimerId);

  DCM_PRECONDITION_ASSERT(DCM_P2TIMER == TimerId, DCM_INTERNAL_API_ID);
  TS_PARAM_UNUSED(TimerId);

  /* enter critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  TimerValue = Dcm_P2TimerValue[ProtocolIndex];

  /* exit critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETTIMERVALUE_EXIT(TimerValue);

  return TimerValue;
}

/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.GetP2StarMaxTime,1 */
FUNC(Dcm_TimerValueType, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_GetP2StarMaxTime(
  Dcm_ProtocolIndexType ProtocolIndex)
{
  P2CONST(Dcm_SesTabConfigType, AUTOMATIC, DCM_CONST) SesTabConfig =
                                                               &DCM_SES_TAB_CONFIG(ProtocolIndex);
  P2CONST(Dcm_SesTabEntryConfigType, AUTOMATIC, DCM_CONST) SessionEntry;
  Dcm_TimerValueType P2StarMaxTime;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETP2STARMAXTIME_ENTRY(ProtocolIndex);

  /* enter critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  if (ProtocolIndex == Dcm_DiagnosticSessionProtocolIndex)
  {
    /* protocol can be in any session */
    SessionEntry = &(SesTabConfig->SessionEntries[Dcm_DiagnosticSessionTableIndex]);
  }
  else
  {
    /* protocol is in default session */
    SessionEntry = &(SesTabConfig->SessionEntries[SesTabConfig->DefSessionIndex]);
  }

  /* exit critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  P2StarMaxTime = SessionEntry->P2StarMaxTime;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETP2STARMAXTIME_EXIT(P2StarMaxTime);

  return P2StarMaxTime;
}

/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.GetP2MaxTime,1 */
FUNC(Dcm_TimerValueType, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_GetP2MaxTime(
  Dcm_ProtocolIndexType ProtocolIndex)
{
  P2CONST(Dcm_SesTabConfigType, AUTOMATIC, DCM_CONST) SesTabConfig =
                                                               &DCM_SES_TAB_CONFIG(ProtocolIndex);
  P2CONST(Dcm_SesTabEntryConfigType, AUTOMATIC, DCM_CONST) SessionEntry;
  Dcm_TimerValueType P2MaxTime;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETP2MAXTIME_ENTRY(ProtocolIndex);

  /* enter critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  if (ProtocolIndex == Dcm_DiagnosticSessionProtocolIndex)
  {
    /* protocol can be in any session */
    SessionEntry = &(SesTabConfig->SessionEntries[Dcm_DiagnosticSessionTableIndex]);
  }
  else
  {
    /* protocol is in default session */
    SessionEntry = &(SesTabConfig->SessionEntries[SesTabConfig->DefSessionIndex]);
  }

  /* exit critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  P2MaxTime = SessionEntry->P2MaxTime;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETP2MAXTIME_EXIT(P2MaxTime);

  return P2MaxTime;
}

/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.ClearResetToDefaultSessionOccured,1 */
FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_ClearResetToDefaultSessionOccured(uint8 ServiceID)
{
  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_CLEARRESETTODEFAULTSESSIONOCCURED_ENTRY(ServiceId);

  switch(ServiceID)
  {
    case DCM_SID_SESSION_CONTROL:
      Dcm_ResetToDefaultSessionOccured[DCM_SVC_FLAGDIAGSESCTRL] = FALSE;
      break;

    case DCM_SID_COMMUNICATIONCONTROL:
      Dcm_ResetToDefaultSessionOccured[DCM_SVC_FLAGCOMMCTRL] = FALSE;
      break;

    case DCM_SID_SECURITYACCESS:
      Dcm_ResetToDefaultSessionOccured[DCM_SVC_FLAGSECACCESS] = FALSE;
      break;

    /* CHECK: NOPARSE */
    default:
      /* do nothing */
      break;
    /* CHECK: PARSE */
  }

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_CLEARRESETTODEFAULTSESSIONOCCURED_EXIT();
}

/* !LINKSTO Dcm.Dsn.IB.DiagosticSessionHandler.GetResetToDefaultSessionOccured,1 */
FUNC(boolean, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_GetResetToDefaultSessionOccured(uint8 ServiceID)
{
  boolean GetValue = FALSE;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETRESETTODEFAULTSESSIONOCCURED_ENTRY(ServiceId);

  switch(ServiceID)
  {
    case DCM_SID_SESSION_CONTROL:
      GetValue = Dcm_ResetToDefaultSessionOccured[DCM_SVC_FLAGDIAGSESCTRL];
      break;

    case DCM_SID_COMMUNICATIONCONTROL:
      GetValue = Dcm_ResetToDefaultSessionOccured[DCM_SVC_FLAGCOMMCTRL];
      break;

    case DCM_SID_SECURITYACCESS:
      GetValue = Dcm_ResetToDefaultSessionOccured[DCM_SVC_FLAGSECACCESS];
      break;

    /* CHECK: NOPARSE */
    default:
      /* do nothing */
      break;
    /* CHECK: PARSE */
  }

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETRESETTODEFAULTSESSIONOCCURED_EXIT(GetValue);

  return GetValue;
}

/* !LINKSTO Dcm.Dsn.IB.DiagosticSessionHandler.GetPreemptionOccured,1 */
FUNC(boolean, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_GetPreemptionOccured(void)
{
  boolean GetValue = FALSE;

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETPREEMPTIONOCCURED_ENTRY();

  if (Dcm_ResetToDefaultSessionRequester == DCM_RESETTODEFAULTSESSIONREQUESTER_SUPERVISOR)
  {
    GetValue = TRUE;
  }

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETPREEMPTIONOCCURED_EXIT(GetValue);

  return GetValue;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.IB.DiagnosticSessionHandler.DoSwitch,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_DoSwitch(
  Dcm_SesCtrlType NewSession)
{
  P2CONST(Dcm_SesTabConfigType, AUTOMATIC, DCM_CONST) SesTabConfig;
  Dcm_ProtocolIndexType ProtocolIdx = DCM_INVALID_PROTOCOL_INDEX;
  uint8 SessionIdx;
  uint8 FlagIdx;
  Dcm_SesCtrlType OldSession;
  boolean NewSessionIsValid = FALSE;
#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
  uint8 RteSesMode = DCM_RTE_DEFAULTSESSION_MODE;
#endif

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_DOSWITCH_ENTRY(NewSession);

  OldSession = Dcm_DiagnosticSessionLevel;

  if (DCM_DEFAULT_SESSION != NewSession)
  {
    /* get the index of the current protocol */
    ProtocolIdx = DCM_PROTOCOL_INDEX_FROM_ID(Dcm_Dsl_Supervisor_GetLatestExternalUDSProtocol());

    /* to switch the session, a protocol must have been started before */
    DCM_PRECONDITION_ASSERT(DCM_INVALID_PROTOCOL_INDEX != ProtocolIdx, DCM_INTERNAL_API_ID);

    /* get the session table */
    SesTabConfig = &DCM_SES_TAB_CONFIG(ProtocolIdx);

    /* get the session index of the new session */
    for (SessionIdx = 0U; SessionIdx < SesTabConfig->NumEntries; SessionIdx++)
    {
      if ((SesTabConfig->SessionEntries[SessionIdx]).SesCtrl == NewSession)
      {
        break;
      }
    }
    if (SessionIdx < SesTabConfig->NumEntries)
    {
      /* enter critical section */
      SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

      /* new session is valid */
      Dcm_DiagnosticSessionProtocolIndex = ProtocolIdx;
      Dcm_DiagnosticSessionTableIndex = SessionIdx;
      Dcm_DiagnosticSessionMainConnectionIndex =
        Dcm_RxPduIdConfig[DCM_RX_CONNECTION_INDEX_FROM_ID(DCM_HSM_INFO(ProtocolIdx).RxConnectionId)].MainConnectionIndex;

      /* !LINKSTO Dcm022,1 */
      Dcm_DiagnosticSessionLevel = NewSession;

      /* exit critical section */
      SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
      RteSesMode = (SesTabConfig->SessionEntries[SessionIdx]).RteSesMode;
#endif
      NewSessionIsValid = TRUE;
    }
  }
  else
  {
#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
    /* get the session table */
    SesTabConfig = &DCM_SES_TAB_CONFIG(0U);
    RteSesMode = SesTabConfig->SessionEntries[SesTabConfig->DefSessionIndex].RteSesMode;
#endif
    NewSessionIsValid = TRUE;
  }

  if (NewSessionIsValid == TRUE)
  {

    if ((DCM_DEFAULT_SESSION != OldSession) && (DCM_DEFAULT_SESSION == NewSession))
    {
      /* enter critical section */
      SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

      /* stop S3 timer */
      Dcm_S3TimerValue = 0U;

      /* set default values */
      Dcm_DiagnosticSessionProtocolIndex = DCM_INVALID_PROTOCOL_INDEX;
      Dcm_DiagnosticSessionTableIndex = 0U;
      Dcm_DiagnosticSessionMainConnectionIndex = DCM_INVALID_MAINCONNECTION_INDEX;

     /* !LINKSTO Dcm022,1 */
      Dcm_DiagnosticSessionLevel = NewSession;

      /* exit critical section */
      SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
    }

#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
#if (DCM_BSWM_API_ENABLED == STD_ON)
    if (DCM_DEFAULT_SESSION == NewSession)
    {
      /* !LINKSTO Dcm.EB.SessionStateHandling.ResetToDefaultSession.BswM,1 */
      BswM_Dcm_RequestSessionMode(DCM_DEFAULT_SESSION);
    }
#endif
#else
    /* inform SchM about session change */
    /* !LINKSTO Dcm.EB.DiagnosticSessionControl.SchM_Switch,1 */
    /* !LINKSTO Dcm.EB.SessionStateHandling.ResetToDefaultSession.ModeSwitch,1 */
    (void)SchM_Switch_Dcm_DcmDiagnosticSessionControl(RteSesMode);
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */

    if (DCM_DEFAULT_SESSION == NewSession)
    {
      /* Inform ComM according to Dcm168 */
      Dcm_Dsl_Supervisor_DefaultSessionEntered();
    }
    else
    {
      NetworkHandleType NetworkId;

      /* Get the channel index */
      Dcm_RxPduIdInfoType RxPduIdInfo;
      Dcm_Dsl_RxConnectionGetRxPduIdInformation(DCM_HSM_INFO(ProtocolIdx).RxConnectionId, &RxPduIdInfo);

      /* Get the network identifier */
      NetworkId = RxPduIdInfo.ComMChannelId;

      /* !LINKSTO Dcm.EB.CommunicationModeHandling.SessionChangeToNonDefault.ActiveDiagnosisCOMM_ACTIVE.ActiveDiagnostic,1 */
      /* !LINKSTO Dcm.EB.CommunicationModeHandling.SessionChangeToNonDefault.ActiveDiagnosisFALSE.ActiveDiagnostic,1 */
      (void)Dcm_Dsl_ComMChannelStateManager_RequestActiveDiagnostic(NetworkId);
    }

    /* Diagnostic session has been changed. Notify relevant DCM units */
    Dcm_Dsl_DiagnosticSessionHandler_UponSessionChangeNotify(OldSession, NewSession);

  }
  else
  {
    /* new session is invalid */
    /* keep old session */
  }

  if (DCM_DEFAULT_SESSION == NewSession)
  {
    /* clear request flag */
    Dcm_DiagnosticSessionResetFlag = FALSE;
    for (FlagIdx = 0U; FlagIdx < DCM_NUM_FLAGSFORSVCID; FlagIdx++)
    {
      Dcm_ResetToDefaultSessionOccured[FlagIdx] = TRUE;
    }
  }

  DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_DOSWITCH_EXIT();
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsl_DiagnosticSessionHandler_UponSessionChangeNotify(
  Dcm_SesCtrlType OldSession, Dcm_SesCtrlType NewSession)
{
    DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_UPONSESSIONCHANGENOTIFY_ENTRY(OldSession, NewSession);

    Dcm_Dsl_SecurityLevelHandler_SessionChange(OldSession, NewSession);

#if (DCM_DSP_USE_SERVICE_0X27 == STD_ON)
    Dcm_Dsp_SecurityAccess_SessionChange(OldSession, NewSession);
#endif /* #if (DCM_DSP_USE_SERVICE_0X27 == STD_ON) */

#if (DCM_DSP_USE_ROESERVICES == STD_ON)
    Dcm_Dsl_RoeServices_SessionChange(OldSession, NewSession);
#endif /* #if (DCM_DSP_USE_ROESERVICES == STD_ON) */

#if (DCM_DSP_USE_SERVICE_0X85 == STD_ON)
    Dcm_Dsp_ControlDTCSetting_SessionChange(OldSession, NewSession);
#endif /* #if (DCM_DSP_USE_SERVICE_0X85 == STD_ON) */

#if (DCM_RETURNCONTROLTOECU_ENABLED == STD_ON)
    Dcm_Dsp_InputOutputControlByIdentifier_SessionChange(OldSession, NewSession);
#endif /* #if (DCM_RETURNCONTROLTOECU_ENABLED == STD_ON) */

#if(DCM_DSP_USE_SERVICE_0X28 == STD_ON)
    Dcm_Dsp_CommunicationControl_SessionChange(OldSession, NewSession);
#endif /* #if (DCM_DSP_USE_SERVICE_0X28 == STD_ON) */

#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
    Dcm_Dsp_ReadDataByPeriodicIdentifier_SessionChange(OldSession, NewSession);
#endif /* (DCM_DSP_USE_SERVICE_0X2A == STD_ON) */

    DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_UPONSESSIONCHANGENOTIFY_EXIT();
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[end of file]================================================================*/
