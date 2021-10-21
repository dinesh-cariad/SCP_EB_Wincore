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

/* !LINKSTO Dcm.Dsn.File.ControlDTCSetting.Imp,1 */
/* This file contains the implementation of the Diagnostic Service ControlDTCSetting. */

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 15.2 (required)
 *     "An unconditional break statement shall terminate every non-empty switch clause."
 *
 *     Reason:
 *     The following actions have to be executed in the same state so the break
 *     statement is removed in order to have shorter code and avoid duplicated code.
 *
 */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 16.3 (required)
  *     "An unconditional break statement shall terminate every switch-clause."
  *
  *     Reason:
  *     The following actions have to be executed in the same state so the break
  *     statement is removed in order to have shorter code and avoid duplicated code.
  *
  *  MISRAC2012-2) Deviated Rule: 16.1 (required)
  *     "All switch statements shall be well-formed"
  *
  *     Reason:
  *     This violation is present because rule 16.3 is violated and justified inside the affected
  *     switch statement, as noted under rule 16.1:
  *     some of the restrictions imposed on switch statements by this rule are expounded in the
  *     15.3, 16.2, 16.3, 16.4, 16.5, 16.6 rules, which are violated inside the current switch statement.
  */

/*===============================[includes]======================================================*/

#include <Dcm_Trace.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <TSAutosar.h>
#include <Dcm_Dsp_SvcH_ControlDTCSetting.h>
#include <Dcm_Dsl_Supervisor.h>
#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
#include <SchM_Dcm.h>                                                  /* SchM interface for Dcm */
#endif

#if(DCM_DSP_USE_SERVICE_0X85 == STD_ON)

/*===========================[macro definitions]=================================================*/

/** \brief Value definition for 'enable Dtc setting' subservice */
#if (defined DCM_CONTROLDTCSETTING_ON)
  #error "DCM_CONTROLDTCSETTING_ON is already defined"
#endif
#define DCM_CONTROLDTCSETTING_ON               1U

/** \brief Value definition for 'disable Dtc setting' subservice */
#if (defined DCM_CONTROLDTCSETTING_OFF)
  #error "DCM_CONTROLDTCSETTING_OFF is already defined"
#endif
#define DCM_CONTROLDTCSETTING_OFF              2U

/** \brief Value definition for calling the ModeSwitch function without to check
 **        the response from Dem */
#if (defined DCM_CALLMODESWITCH_WITHOUT_DEMCHECK)
  #error "DCM_CALLMODESWITCH_WITHOUT_DEMCHECK is already defined"
#endif
#define DCM_CALLMODESWITCH_WITHOUT_DEMCHECK    0U

/** \brief Value definition for calling the ModeSwitch function just if Dem return E_OK */
#if (defined DCM_CALLMODESWITCH_WITH_DEMCHECK)
  #error "DCM_CALLMODESWITCH_WITH_DEMCHECK is already defined"
#endif
#define DCM_CALLMODESWITCH_WITH_DEMCHECK       1U

/* Macro for ControlDtcSetting service ID */
#if (defined DCM_DSP_SERVICE_0X85)
  #error "DCM_DSP_SERVICE_0X85 is already defined"
#endif
#define DCM_DSP_SERVICE_0X85                (0x85U)

/*==================[type definitions]===========================================================*/

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*==================[internal function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief UDS service 0x85 internal service handler
 **
 ** \param[in]    OpStatus     Operation state of the function.
 ** \param[inout] pMsgContext  Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ControlDTCSetting_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief UDS service 0x85 internal service handler
 **
 ** \param[in]    OpStatus       Operation state for internal DCM Service function.
 ** \param[inout] pMsgContext    Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ControlDTCSetting_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief This internal function enable/disable the ControlDTCSetting in the DCM module
 **
 ** \param [in]    DemClientId             This parameter defines the Dem client instance.
 ** \param [in]    CallModeSwitch          Specify if the ModeSwitch function need to be called
 **                                        without to check the Dem response.
 ** \param [in]    ControlDTCSettingState  Specify the requested status of ControlDTCSetting.
 **
 ** \retval DCM_CONTROL_DTC_SETTING_OK        EnableDTCSetting/DisableDTCSetting was successful
 ** \retval DCM_CONTROL_DTC_SETTING_PENDING   EnableDTCSetting/DisableDTCSetting did not complete,
 **                                           it must be called again
 **/
STATIC FUNC(Dcm_ReturnControlDTCSettingType, DCM_CODE) Dcm_Dsp_ControlDTCSetting_SetState(
  Dcm_DemClientIdType DemClientId,
  boolean CallModeSwitch,
  Dcm_Dsp_DtcSettingControlType ControlDTCSettingState);

/** \brief This function performs requested actions upon session level change.
 **
 **  \param NewSession [in] Session level after change

 **  \return void
 **/
STATIC FUNC(void, DCM_CODE) Dcm_ResetDTCSettingStatusOnSessionChange(Dcm_SesCtrlType NewSession);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>
/** \brief This global variable hold the sub-service handler, extracted from
 **        ControlDTCSettings(0x85) request in the DCM_INITIAL state.
 **        This variable is used in each call of the ControlDTCSettings handler to call the
 **        sub-service handler whether is external or internal.
 */
STATIC VAR(Dcm_GenericServiceHandlerType, DCM_VAR) Dcm_ControlDTCSetting_SubServiceHandler;
#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>
/** \brief Mutex controlling access to DEM ControlDTCSettting APIs */
STATIC VAR(Dcm_MutexType, DCM_VAR ) Dcm_ControlDTCSetting_Lock;

/** \brief This variable indicates if the Dcm_Dsp_ControlDTCSetting_ReEnable() function needs to
 **        be called in the next MainFunction() because the ControlDTCSetting is locked
 **        - TRUE : The ControlDTCSetting is locked and Dcm_Dsp_ControlDTCSetting_ReEnable() will be called
 **                 in the next MainFunction()
 **        - FALSE: The ControlDTCSetting is unlocked and Dcm_Dsp_ControlDTCSetting_ReEnable() is called
 **/
STATIC VAR(boolean, DCM_VAR) Dcm_ReAttemptEnableDTCSetting;

/** \brief Status of ControlDTCSettings
 **
 ** - DCM_DSP_DTCSETTING_DISABLE  : DTCSettings are disabled
 ** - DCM_DSP_DTCSETTING_ENABLE   : DTCSettings are enabled */
STATIC VAR(Dcm_Dsp_DtcSettingControlType, DCM_VAR ) Dcm_ControlDtcStatus;

/** \brief DemClientId for which ControlDTCSetting shall be re-enabled
 **/
STATIC VAR(Dcm_DemClientIdType, DCM_VAR) Dcm_ReEnableDemClientId;

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.IB.ControlDTCSetting.SvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ControlDTCSetting_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_CONTROLDTCSETTING_SVCH_ENTRY(OpStatus,pMsgContext);

  /* Deviation MISRAC2012-2 */
  switch (OpStatus)
  {
    /* first call of service function */
    /* Deviation MISRAC2012-1 */
    /* Deviation MISRA-1 */
    case DCM_INITIAL:
    {
      /* check request length */
      if ((pMsgContext->reqDataLen - 1U) != DCM_0X85_CONTROLOPTIONRECORD_SIZE)
      {
        /* The length of the message is wrong */
        /* !LINKSTO Dcm.EnableDTCSetting.DTCSettingControlOptionRecord.Length.NRC,1 */
        /* !LINKSTO Dcm.DisableDTCSetting.DTCSettingControlOptionRecord.Length.NRC,1 */
        /* !LINKSTO Dcm.EnableDTCSetting.DcmSupportDTCSettingControlOptionRecord.False.NRC,1 */
        /* !LINKSTO Dcm.DisableDTCSetting.DcmSupportDTCSettingControlOptionRecord.False.NRC,1 */

        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
        serviceProcessStatus = DCM_E_DONE;
        break;
      }
#if (DCM_0X85_CONTROLOPTIONRECORD_ENABLED == STD_ON)
      {
        /* check ControlOptionRecord */
        uint32 ReqDTCGroup = ((uint32)pMsgContext->reqData[1U] << 16U) |
                             ((uint32)pMsgContext->reqData[2U] << 8U)  |
                             ((uint32)pMsgContext->reqData[3U]);
        if (0xFFFFFF != ReqDTCGroup)
        {
          /* The ControlOptionRecord of the message is wrong */
          /* !LINKSTO Dcm.EnableDTCSetting.DTCSettingControlOptionRecord.NRC,1 */
          /* !LINKSTO Dcm.DisableDTCSetting.DTCSettingControlOptionRecord.NRC,1 */
          Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
          serviceProcessStatus = DCM_E_DONE;
          break;
        }
      }
#endif
    /* Valid request, continue processing */
      serviceProcessStatus = Dcm_Dsp_ControlDTCSetting_SvcStart(OpStatus, pMsgContext);
      break;
    }
    case DCM_CANCEL:
    case DCM_PENDING:
      serviceProcessStatus = Dcm_Dsp_ControlDTCSetting_SvcCont(OpStatus, pMsgContext);
      break;
    default:
      serviceProcessStatus = DCM_E_DONE;
      break;
  }

  DBG_DCM_DSP_CONTROLDTCSETTING_SVCH_EXIT(serviceProcessStatus,OpStatus,pMsgContext);

  return serviceProcessStatus;
}

#if (DCM_0X85_0X01_SSVC_ENABLED == STD_ON)
/* This is the UDS service 0x85-0x01 handler */
/* !LINKSTO Dcm.Dsn.IB.ControlDTCSetting.On.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ControlDTCSetting_On_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType MutexLockState = DCM_E_RESOURCE_LOCKED;
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  Dcm_ReturnControlDTCSettingType DTCSettingStatus;

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  if (DCM_INITIAL == OpStatus)
  {
    /* copy sub-function value to response buffer */
    pMsgContext->resData[0U] = DCM_CONTROLDTCSETTING_ON;
    pMsgContext->resDataLen  = 1U;
  }

  /* !LINKSTO Dcm.EB.EnableDTCSetting.Locking,1 */
  MutexLockState = Dcm_Internal_GetLock(&Dcm_ControlDTCSetting_Lock);
  /* CHECK: NOPARSE */
  if(DCM_E_OK == MutexLockState)
  /* CHECK: PARSE */
  {
    /* !LINKSTO SWS_Dcm_01063,1 */
    /* Set the status of ControlDTCSetting to enable. Also the ModeSwitch is made if necessary. */
    DTCSettingStatus = Dcm_Dsp_ControlDTCSetting_SetState(DemClientId, DCM_CALLMODESWITCH_WITH_DEMCHECK,
    DCM_DSP_DTCSETTING_ENABLE);

    Dcm_Internal_ReleaseLock(&Dcm_ControlDTCSetting_Lock);

    if (DTCSettingStatus == DCM_CONTROL_DTC_SETTING_OK)
    {
      serviceProcessStatus = DCM_E_DONE;
    }
    else if (DTCSettingStatus == DCM_CONTROL_DTC_SETTING_PENDING)
    {
      /* !LINKSTO Dcm.EnableDTCSetting.DEM_PENDING,1 */
      serviceProcessStatus = DCM_E_PENDING;
    }
    else
    {
      /* !LINKSTO Dcm.EnableDTCSetting.UnsupportedError.NRC,1 */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
      serviceProcessStatus = DCM_E_DONE;
    }
  }
  /* CHECK: NOPARSE */
  else
  {
    serviceProcessStatus = DCM_E_PENDING;
    DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_CONTROLDTCSETTING);
  }
  /* CHECK: PARSE */
  return serviceProcessStatus;
}
#endif /* DCM_0X85_0X01_SSVC_ENABLED == STD_ON */

#if (DCM_0X85_0X02_SSVC_ENABLED == STD_ON)
/* This is the UDS service 0x85-0x02 handler */
/* !LINKSTO Dcm.Dsn.IB.ControlDTCSetting.Off.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ControlDTCSetting_Off_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  Std_ReturnType MutexLockState = DCM_E_RESOURCE_LOCKED;
  Dcm_ReturnControlDTCSettingType DTCSettingStatus;

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  if (DCM_INITIAL == OpStatus)
  {
    /* copy sub-function value to response buffer */
    pMsgContext->resData[0U] = DCM_CONTROLDTCSETTING_OFF;
    pMsgContext->resDataLen  = 1U;
  }

  /* !LINKSTO Dcm.EB.DisableDTCSetting.Locking,1 */
  MutexLockState = Dcm_Internal_GetLock(&Dcm_ControlDTCSetting_Lock);
  /* CHECK: NOPARSE */
  if(DCM_E_OK == MutexLockState)
  /* CHECK: PARSE */
  {
    /* !LINKSTO SWS_Dcm_00406,1 */
    /* Set the status of ControlDTCSetting to disable. Also the ModeSwitch is made if necessary. */
    DTCSettingStatus = Dcm_Dsp_ControlDTCSetting_SetState(DemClientId, DCM_CALLMODESWITCH_WITH_DEMCHECK,
    DCM_DSP_DTCSETTING_DISABLE);

    Dcm_Internal_ReleaseLock(&Dcm_ControlDTCSetting_Lock);

    if (DTCSettingStatus == DCM_CONTROL_DTC_SETTING_OK)
    {
      serviceProcessStatus = DCM_E_DONE;
    }
    else if (DTCSettingStatus == DCM_CONTROL_DTC_SETTING_PENDING)
    {
      /* !LINKSTO Dcm.DisableDTCSetting.DEM_PENDING,1 */
      serviceProcessStatus = DCM_E_PENDING;
    }
    else
    {
      /* !LINKSTO Dcm.DisableDTCSetting.UnsupportedError.NRC,1 */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
      serviceProcessStatus = DCM_E_DONE;
    }
  }
  /* CHECK: NOPARSE */
  else
  {
    serviceProcessStatus = DCM_E_PENDING;
    DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_CONTROLDTCSETTING);
  }
  /* CHECK: PARSE */

  return serviceProcessStatus;
}
#endif /* DCM_0X85_0X02_SSVC_ENABLED == STD_ON */

/* !LINKSTO Dcm.Dsn.IB.ControlDTCSetting.Init,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_ControlDTCSettingState_Init(void)
{
  DBG_DCM_DSP_CONTROLDTCSETTINGSTATE_INIT_ENTRY();

  Dcm_ControlDTCSetting_Lock = DCM_MUTEX_UNLOCKED;
  Dcm_ControlDtcStatus = DCM_DSP_DTCSETTING_ENABLE;
  Dcm_ReAttemptEnableDTCSetting = FALSE;
  Dcm_ControlDTCSetting_SubServiceHandler = NULL_PTR;
  Dcm_ReEnableDemClientId = 0U;

  DBG_DCM_DSP_CONTROLDTCSETTINGSTATE_INIT_EXIT();
}

/* !LINKSTO Dcm.Dsn.IB.ControlDTCSetting.ReEnable,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_ControlDTCSetting_ReEnable(void)
{
  Std_ReturnType MutexLockState = DCM_E_RESOURCE_LOCKED;
  DBG_DCM_DSP_CONTROLDTCSETTING_REENABLE_ENTRY();

  /* !LINKSTO Dcm.EB.ReEnableDTCSetting.Locking,1 */
  MutexLockState = Dcm_Internal_GetLock(&Dcm_ControlDTCSetting_Lock);
  if(DCM_E_OK == MutexLockState)
  {
    /* Enables the DTC setting for all DTC group. */
    (void) Dcm_Dsp_ControlDTCSetting_SetState(Dcm_ReEnableDemClientId,
        DCM_CALLMODESWITCH_WITHOUT_DEMCHECK,
        DCM_DSP_DTCSETTING_ENABLE);

    /* Set the global status of DTCDisable Status */
    Dcm_ControlDtcStatus = DCM_DSP_DTCSETTING_ENABLE;

    Dcm_ReAttemptEnableDTCSetting = FALSE;

    Dcm_Internal_ReleaseLock(&Dcm_ControlDTCSetting_Lock);
  }
  else
  {
    Dcm_ReAttemptEnableDTCSetting = TRUE;
  }
  DBG_DCM_DSP_CONTROLDTCSETTING_REENABLE_EXIT();
}

/* !LINKSTO Dcm.Dsn.IB.ControlDTCSetting.GetState,1 */
FUNC(Dcm_Dsp_DtcSettingControlType, DCM_CODE) Dcm_Dsp_ControlDTCSetting_GetState(void)
{
  Dcm_Dsp_DtcSettingControlType ControlDtcStatus;

  DBG_DCM_DSP_CONTROLDTCSETTING_GETSTATE_ENTRY();

  ControlDtcStatus = Dcm_ControlDtcStatus;

  DBG_DCM_DSP_CONTROLDTCSETTING_GETSTATE_EXIT(ControlDtcStatus);

  return ControlDtcStatus;
}

/* !LINKSTO Dcm.Dsn.IB.ControlDTCSetting.MainFunction,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_ControlDTCSetting_MainFunction(void)
{
  boolean ModeRuleFailed = FALSE;

#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
  if ((Dcm_DspModeRuleDTCFunction != NULL_PTR) )
  {
    Dcm_NegativeResponseCodeType nrc;

    Dcm_DspModeRuleDTCFunction(&nrc);
    if (nrc != DCM_E_OK)
    {
      /* !LINKSTO Dcm.EnableDTCSetting.ModeRuleNotFullfilled,1 */
      /* !LINKSTO Dcm.EnableDTCSetting.ModeSwitch.ModeRuleNotFullfilled,1 */
      ModeRuleFailed = TRUE;
    }
  }
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_ON) */

  if( (Dcm_Dsp_ControlDTCSetting_GetState() == DCM_DSP_DTCSETTING_DISABLE) &&
      ((ModeRuleFailed == TRUE)|| (Dcm_ReAttemptEnableDTCSetting == TRUE)))
  {
     Dcm_Dsp_ControlDTCSetting_ReEnable();
  }
}

FUNC(void, DCM_CODE) Dcm_Dsp_ControlDTCSetting_SessionChange(
  Dcm_SesCtrlType OldSession, Dcm_SesCtrlType NewSession)
{
  DBG_DCM_DSP_CONTROLDTCSETTING_SESSIONCHANGE_ENTRY(OldSession, NewSession);

  TS_PARAM_UNUSED(OldSession);

  if (DCM_DEFAULT_SESSION == NewSession)
  {
    /* !LINKSTO Dcm.EnableDTCSetting.TransToDefaultSession.S3Timeout,2 */
    /* !LINKSTO Dcm.EnableDTCSetting.TransToDefaultSession.ResetToDefaultSession,2 */
    /* !LINKSTO Dcm.EnableDTCSetting.ModeSwitch.TransToDefaultSession.S3Timeout,2 */
    /* !LINKSTO Dcm.EnableDTCSetting.ModeSwitch.TransToDefaultSession.ResetToDefaultSession,2 */
    /* !LINKSTO Dcm.EnableDTCSetting.TransToDefaultSession.ProtocolPreemption,1 */
    /* !LINKSTO Dcm.EnableDTCSetting.ModeSwitch.TransToDefaultSession.ProtocolPreemption,1 */

    /* Call Dem_EnableDTCSetting() while transitioning to default session, also made the ModeSwitch
     *  see Dcm_Dsp_ControlDTCSetting_SetState() function. */
    if (Dcm_Dsp_ControlDTCSetting_GetState() == DCM_DSP_DTCSETTING_DISABLE)
    {
      Dcm_Dsp_ControlDTCSetting_ReEnable();
    }
  }
  else
  {
    (void)Dcm_ResetDTCSettingStatusOnSessionChange(NewSession);
  }

  DBG_DCM_DSP_CONTROLDTCSETTING_SESSIONCHANGE_EXIT();

}

/*==================[internal function definitions]==============================================*/

/* !LINKSTO Dcm.Dsn.IB.ControlDTCSetting.SvcStart,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ControlDTCSetting_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  /* SWS_Dcm_00764: if get a valid pointer to subServiceHandler, then pMsgContext will point
   * behind the subFunction identifier */
  Dcm_ControlDTCSetting_SubServiceHandler = Dcm_Dsp_ExtractSubServiceHandler(OpStatus, pMsgContext);

  DCM_PRECONDITION_ASSERT(Dcm_ControlDTCSetting_SubServiceHandler != NULL_PTR, DCM_SVCID_CONTROLDTCSETTING);
  /* Call sub-service handler */
  ServiceProcessStatus = Dcm_ControlDTCSetting_SubServiceHandler(OpStatus, pMsgContext);
  /* Sub-service handlers return E_OK, so we must convert to DCM_E_DONE */
  if (E_OK == ServiceProcessStatus)
  {
    ServiceProcessStatus = DCM_E_DONE;
  }

  return ServiceProcessStatus;
}

/* !LINKSTO Dcm.Dsn.IB.ControlDTCSetting.SvcCont,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ControlDTCSetting_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DCM_PRECONDITION_ASSERT(Dcm_ControlDTCSetting_SubServiceHandler != NULL_PTR, DCM_SVCID_CONTROLDTCSETTING);

  /* Call sub-service handler */
  ServiceProcessStatus = Dcm_ControlDTCSetting_SubServiceHandler(OpStatus, pMsgContext);

  /* Sub-service handlers return E_OK and E_NOT_OK, so must be converted to DCM_E_DONE */
  if(E_OK == ServiceProcessStatus)
  {
    ServiceProcessStatus = DCM_E_DONE;
  }

  return ServiceProcessStatus;
}

/* !LINKSTO Dcm.Dsn.IB.ControlDTCSetting.SetState,1 */
STATIC FUNC(Dcm_ReturnControlDTCSettingType, DCM_CODE) Dcm_Dsp_ControlDTCSetting_SetState(
  Dcm_DemClientIdType DemClientId,
  boolean CallModeSwitch,
  Dcm_Dsp_DtcSettingControlType ControlDTCSettingState)
{
  Dcm_ReturnControlDTCSettingType DTCSettingStatus = DCM_CONTROL_DTC_SETTING_OK;

  if(DCM_DSP_DTCSETTING_DISABLE == ControlDTCSettingState)
  {
    /* Disables the DTC setting for the DTC group */
    DTCSettingStatus = Dcm_Dsp_DemFacade_ControlDTCSetting_Disable(DemClientId);

    /* check if DisableDTCSetting was successful */
    if ((DTCSettingStatus == DCM_CONTROL_DTC_SETTING_OK) ||
        (DTCSettingStatus == DCM_CONTROL_DTC_SETTING_PENDING))
    {
      /* Set the global status of DTCDisable Status */
      Dcm_ControlDtcStatus = DCM_DSP_DTCSETTING_DISABLE;
      /* Store the client ID in case it needs to be re-enabled; see [SWS_Dcm_00751], [SWS_Dcm_00752] */
      Dcm_ReEnableDemClientId = DemClientId;
    } /* else: all other return values are handled in the sub-service handler */
  }
  else
  {
    DCM_PRECONDITION_ASSERT(
              (DCM_DSP_DTCSETTING_ENABLE == ControlDTCSettingState), DCM_SVCID_CONTROLDTCSETTING);
    /* Enables the DTC setting for the DTC group */
    DTCSettingStatus = Dcm_Dsp_DemFacade_ControlDTCSetting_Enable(DemClientId);

    /* check if EnableDTCSetting was successful */
    if (DTCSettingStatus == DCM_CONTROL_DTC_SETTING_OK)
    {
      /* Set the global status of DTCDisable Status */
      Dcm_ControlDtcStatus = DCM_DSP_DTCSETTING_ENABLE;

    } /* else: all other return values are handled in the sub-service handler */
  }
#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
  /* ModeSwitch function is called according to CallModeSwitch variable.
   * Requirement SWS_Dcm_00783/SWS_Dcm_00784 says that ModeSwitch function need to be called just
   * if Dem return OK(CallModeSwitch == DCM_CALLMODESWITCH_WITH_DEMCHECK).
   * Requirement SWS_Dcm_00751/SWS_Dcm_00752 says that ModeSwitch function need to be called
   * regardless the return value from Dem (CallModeSwitch == DCM_CALLMODESWITCH_WITHOUT_DEMCHECK).
   */
  /* !LINKSTO SWS_Dcm_00783,1 */
  /* !LINKSTO SWS_Dcm_00784,1 */
  if((CallModeSwitch == DCM_CALLMODESWITCH_WITHOUT_DEMCHECK) ||
     (DTCSettingStatus == DCM_CONTROL_DTC_SETTING_OK))
  {
    if(DCM_DSP_DTCSETTING_DISABLE == ControlDTCSettingState)
    {
      (void)SchM_Switch_Dcm_DcmControlDTCSetting(RTE_MODE_DcmControlDTCSetting_DISABLEDTCSETTING);
    }
    else
    {
      (void)SchM_Switch_Dcm_DcmControlDTCSetting(RTE_MODE_DcmControlDTCSetting_ENABLEDTCSETTING);
    }
  }
#else
  TS_PARAM_UNUSED(CallModeSwitch);
#endif /* (DCM_MODEDECLARATION_SUPPORT == STD_ON) */

  return DTCSettingStatus;
}

STATIC FUNC(void, DCM_CODE) Dcm_ResetDTCSettingStatusOnSessionChange(Dcm_SesCtrlType NewSession)
{
  uint8_least sessionIdx;
  boolean ReEnableControlDTC = TRUE;

  DBG_DCM_RESETDTCSETTINGSTATUSONSESSIONCHANGE_ENTRY(NewSession);

  if (Dcm_Dsp_ControlDTCSetting_GetState() == DCM_DSP_DTCSETTING_DISABLE)
  {
    /* get configuration entry of 0x85 service Id */
    P2CONST(Dcm_SidTabEntryConfigType, AUTOMATIC, DCM_CONST) SidConfig = NULL_PTR;

    Dcm_ProtocolIndexType ProtocolIdx = DCM_PROTOCOL_INDEX_FROM_ID(Dcm_Dsl_Supervisor_GetLatestExternalUDSProtocol());

    /* This function is called with parameter NewSession which is always different from default session.
     * Variable ProtocolIdx must be one valid protocol index since only UDS service DiagnosticSessionContol
     * can request switch to non-default session */
    DCM_PRECONDITION_ASSERT(DCM_INVALID_PROTOCOL_INDEX != ProtocolIdx, DCM_INTERNAL_API_ID);

    (void)Dcm_DspInternal_GetSidConfig(ProtocolIdx, DCM_DSP_SERVICE_0X85, &SidConfig);
    DCM_PRECONDITION_ASSERT(NULL_PTR != SidConfig, DCM_INTERNAL_API_ID);

    DCM_PRECONDITION_ASSERT(DCM_DEFAULT_SESSION != NewSession, DCM_INTERNAL_API_ID);

    /* Enabling DTC setting, if 0x85 service is not supported in the requested session */
    if (SidConfig->NumSesCtrls > 0U)
    {
      for (sessionIdx = 0U; sessionIdx < SidConfig->NumSesCtrls; ++sessionIdx)
      {
        if ((SidConfig->SesCtrls[sessionIdx] == DCM_ALL_SESSION_LEVEL) ||
            (SidConfig->SesCtrls[sessionIdx] == NewSession))
        {
          ReEnableControlDTC = FALSE;
          break;
        }
      }
    }
    else
    {
      ReEnableControlDTC = FALSE;
    }

    if (ReEnableControlDTC == TRUE)
    {
      /* !LINKSTO Dcm.EnableDTCSetting.SessionNoSupportControlDTC,2 */
      /* !LINKSTO Dcm.EnableDTCSetting.TransToDefaultSession.Service0x10,2 */
      /* !LINKSTO Dcm.EnableDTCSetting.ModeSwitch.SessionNoSupportControlDTC,2 */
      /* !LINKSTO Dcm.EnableDTCSetting.ModeSwitch.TransToDefaultSession.Service0x10,2 */

      /* Enables the DTC setting for the all DTC group, also made the ModeSwitch see
       * Dcm_Dsp_ControlDTCSetting_SetState() function. */
      Dcm_Dsp_ControlDTCSetting_ReEnable();
    }
  }

  DBG_DCM_RESETDTCSETTINGSTATUSONSESSIONCHANGE_EXIT();
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if(DCM_DSP_USE_SERVICE_0X85 == STD_ON) */

/*==================[end of file]================================================================*/
