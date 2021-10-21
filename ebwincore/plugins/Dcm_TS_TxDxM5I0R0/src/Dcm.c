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

/* !LINKSTO Dcm.Dsn.File.Dcm.Impl,1 */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 20.7 (required)
  *     "Expressions resulting from the expansion of macro parameters shall
  *     be enclosed in parentheses."
  *
  *     Reason:
  *     The macro is used in function parameter declarations and definitions
  *     where the number of parentheses matter.
  *
  */

/*==================[inclusions]=================================================================*/

#include <Dcm_Trace.h>
#include <Dcm_Trace_Internal.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <TSAutosar.h>                                             /* EB specific standard types */

#include <Dcm.h>                                                      /* Declaration of Dcm APIs */
#include <Dcm_Cbk.h>                                             /* Declaration of Dcm Callbacks */
#if (DCM_INCLUDE_RTE == STD_ON)
#include <Rte_Dcm.h>                                                /* Dcm Mainfunction prototype*/
#endif
#include <Dcm_HsmDcm.h>                                   /* public statechart model definitions */
#include <Dcm_Int.h>                                    /* Module internal header dependent part */
#include <Dcm_RoutineControl_Cfg.h>                             /*  RoutineControl Configuration */

#include <Dcm_DspExternal.h>                           /* Callbacks provided by Central Diag SwC */
/* do not indirectly include PduR_SymbolicNames_PBcfg.h via PduR_Dcm.h */
#define PDUR_NO_PBCFG_REQUIRED
#include <PduR_Dcm.h>                                                  /* PduR interface for Dcm */
#include <ComM_Dcm.h>                                                  /* ComM interface for Dcm */
#if (DCM_BSWM_API_ENABLED == STD_ON)
#include <BswM_DCM.h>                                                  /* BswM interface for Dcm */
#endif
#include <SchM_Dcm.h>
#include <Dcm_Dsl_Supervisor.h>
#include <Dcm_Dsl_PostBootOperationsHandler.h>

#include <Dcm_Dsl_ComMChannelStateManager.h>    /* Dcm Dsl CComMChannelStateManager header file */

#include <Dcm_Dsl_DiagnosticSessionHandler.h>  /* Dcm Dsl Diagnostic Session Handler header file */
#include <Dcm_Dsl_SecurityLevelHandler.h>

#if (DCM_DSP_USE_SERVICE_0X06 == STD_ON)
#include <Dcm_Dsp_MidServices.h>
#endif

#if (DCM_DSP_USE_DIDSERVICES == STD_ON)
#include <Dcm_Dsp_DidServices.h>
#endif

/* needed for legacy functions directly initializing data transfer services */
#include <Dcm_Dsp_DataTransferServices.h>

#if (DCM_DSP_USE_ROESERVICES == STD_ON)
#include <Dcm_Dsl_RoeServices.h>
#endif

#include <Dcm_Dsp_SvcH_CommunicationControl.h>

#if (DCM_DSP_USE_SERVICE_0X2C == STD_ON)
#include <Dcm_Dsp_SvcH_DynamicallyDefineDataIdentifier.h>
#endif

#if(DCM_DSP_USE_SERVICE_0X2F == STD_ON)
#include <Dcm_Dsp_SvcH_InputOutputControlByIdentifier.h>
#endif

#if(DCM_DSP_USE_SERVICE_0X22 == STD_ON)
#include <Dcm_Dsp_SvcH_ReadDataByIdentifier.h>
#endif

#if(DCM_DSP_USE_SERVICE_0X24 == STD_ON)
#include <Dcm_Dsp_SvcH_ReadScalingDataByIdentifier.h>
#endif

#if(DCM_DSP_USE_SERVICE_0X2E == STD_ON)
#include <Dcm_Dsp_SvcH_WriteDataByIdentifier.h>
#endif

#if(DCM_DSP_USE_SERVICE_0X87 == STD_ON)
#include <Dcm_Dsp_SvcH_LinkControl.h>
#endif

#if (DCM_DSP_USE_SERVICE_0X27 == STD_ON)
#include <Dcm_Dsp_SvcH_SecurityAccess.h>
#endif

#if (DCM_DSP_USE_DEMFACADE == STD_ON)
#include <Dcm_Dsp_DemFacade.h>
#endif

#if (DCM_DSP_USE_SERVICE_0X85 == STD_ON)
#include <Dcm_Dsp_SvcH_ControlDTCSetting.h>
#endif

#if(DCM_DSP_USE_SERVICE_0X28 == STD_ON)
#include <Dcm_Dsp_SvcH_CommunicationControl.h>
#endif

#if (DCM_DSP_USE_SERVICE_0X3E == STD_ON)
#include <Dcm_Dsp_SvcH_TesterPresent.h>
#endif

#if (DCM_DEV_ERROR_DETECT == STD_ON)
/* Development Error Tracer (DCM_DET_REPORT_ERROR() macros) */
#include <Det.h>
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */

#if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) ||\
     (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X35 == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X3D == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X34 == STD_ON))
#if (DCM_DEV_ERROR_DETECT == STD_ON)
#include <Dcm_Dsp_MemoryServices.h>
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
#endif /* #if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) ||\
     (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X35 == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X3D == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X34 == STD_ON)) */

#if (DCM_DSP_USE_SERVICE_0X31 == STD_ON)
#include <Dcm_Dsp_SvcH_RoutineControl.h>
#endif

/* Enable code only if feature is present */
#if (DCM_DSL_APPLICATION_DIAGNOSTIC_REQUESTS == STD_ON)
#include <Dcm_Dsp_ApplicationDiagnosticRequests.h>
#endif /* #if (DCM_DSL_APPLICATION_DIAGNOSTIC_REQUESTS == STD_ON) */

/*==================[macros]=====================================================================*/
/* !LINKSTO Dcm.Version.Check,2 */
/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef DCM_VENDOR_ID /* configuration check */
#error DCM_VENDOR_ID must be defined
#endif

#if (DCM_VENDOR_ID != 1U) /* vendor check */
#error DCM_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef DCM_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error DCM_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (DCM_AR_RELEASE_MAJOR_VERSION != 4U)
#error DCM_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef DCM_AR_RELEASE_MINOR_VERSION /* configuration check */
#error DCM_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (DCM_AR_RELEASE_MINOR_VERSION != 0U )
#error DCM_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef DCM_AR_RELEASE_REVISION_VERSION /* configuration check */
#error DCM_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (DCM_AR_RELEASE_REVISION_VERSION != 3U )
#error DCM_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef DCM_SW_MAJOR_VERSION /* configuration check */
#error DCM_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (DCM_SW_MAJOR_VERSION != 5U)
#error DCM_SW_MAJOR_VERSION wrong (!= 5U)
#endif

#ifndef DCM_SW_MINOR_VERSION /* configuration check */
#error DCM_SW_MINOR_VERSION must be defined
#endif

#ifndef DCM_SW_PATCH_VERSION /* configuration check */
#error DCM_SW_PATCH_VERSION must be defined
#endif


/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, DCM_CODE) Dcm_Internal_ProcessRetryTimer(DCM_PDL_SF(const uint8 instIdx));

#if (DCM_RETURNCONTROLTOECU_ENABLED == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Internal_ProcessReturnControlToECU(void);
#endif

#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Internal_ProcessAsynchronousProtocolCancellation(
  Dcm_ProtocolIndexType ProtocolIndex
);
#endif /* #if (DCM_ASYNC_SERVICE_HANDLER == STD_ON) */

#if (DCM_DSP_USE_SERVICE_0X22 == STD_ON) || \
    (DCM_DSP_USE_SERVICE_0X2E == STD_ON) || \
    (DCM_DSP_USE_SERVICE_0X2F == STD_ON) || \
    (DCM_DSP_USE_SERVICE_0X2A == STD_ON) || \
    (DCM_DSP_USE_SERVICE_0X2C == STD_ON)
/* Initializes the control structures for services using DID elements */
STATIC FUNC(void, DCM_CODE) Dcm_DIDInit(void);
#endif

/* Initializes the Dsp elements */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_Init(void);

/* Executes the first part of Dcm_MainFunction() */
STATIC FUNC(void, DCM_CODE) Dcm_MainFunction_Part1(void);

/* Executes the second part of Dcm_MainFunction()*/
STATIC FUNC(void, DCM_CODE) Dcm_MainFunction_Part2(void);

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

#define DCM_START_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/** \brief Status of DCM initialization
 *         TRUE:  The Dcm is initialized
 *         FALSE: The Dcm is not initialized
 */
VAR(boolean, DCM_VAR) Dcm_Initialized = FALSE;

#define DCM_STOP_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief Global variable holding the value of RxPduId on which the request was received */
VAR(PduIdType, DCM_VAR) Dcm_RxPduIdBL;

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/* Rationale for mapping Dcm_HsmInfo to NOINIT section.
 *
 * Members of Dcm_HsmInfo are are initialized and accessed as described below:
 * msgContext                   : Elements of structure msgContext.
 *                   reqData    : Initialized in Dcm_DsdInternal_DispatchSvcReq before
 *                                being accessed from Dcm_DsdInternal_DispatchSvcReq().
 *                   reqDataLen : Initialized in Dcm_DsdInternal_DispatchSvcReq before
 *                                being accessed from different service handlers.
 *                   resData    : Initialized in Dcm_DsdInternal_DispatchSvcReq before
 *                                being accessed from different service handlers.
 *                   resDataLen : Initialized in Dcm_DsdInternal_DispatchSvcReq before
 *                                being accessed from different service handlers.
 *                   msgAddInfo : Initialized in Dcm_DsdInternal_DispatchSvcReq() before
 *                                being accessed from Dcm_HsmSfServiceCalledGuard4() and
 *                                Dcm_HsmSfServiceCalledGuard1()
 *                resMaxDataLen : Initialized in Dcm_DsdInternal_DispatchSvcReq before
 *                                being accessed from different service handlers.
 *                   idContext  : Initialized in Dcm_DsdInternal_DispatchSvcReq() before
 *                                being accessed from Dcm_Confirmation()
 *                   dcmRxPduId : Initialized in Dcm_DsdInternal_DispatchSvcReq() before
 *                                being accessed from different service handlers
 * sentLength                   : Initialized in Dcm_DsdInternal_StartPagedProcessing() in
 *                                case it is used in a simulated paged-buffer transmission.
 *                                Initialized in Dcm_HsmDcmSfNormalProcessingEntry(),
 *                                Dcm_HsmDcmSfSendingGrEntry(),
 *                                Dcm_HsmDcmSfRealPgBufProcessingEntry() or
 *                                Dcm_HsmDcmSfSendRpEntry()  in case it is used in any other
 *                                transmission mode before being accessed from
 *                                Dcm_CopyTxData()
 * curTxBuffer                  : Initialized in Dcm_HsmDcmSfTopEntry before being
 *                                accessed from Dcm_CopyTxData().
 * retryTimer                   : Initialized in Dcm_HsmDcmSfTopEntry before being
 *                                accessed from Dcm_Internal_ProcessRetryTimer().
 * TxPduId                      : Initialized in Dcm_StartOfReception() before being
 *                                accessed from Dcm_HsmDcmSfNormalProcessingEntry() or
 *                                Dcm_HsmDcmSfRealPgBufProcessingEntry() or
 *                                Dcm_HsmDcmSfSendRpEntry() or
 *                                Dcm_HsmDcmSfSendingGrEntry()
 * sidTabEntryInd               : Initialized in Dcm_DsdInternal_ValidateSvcReq before being
 *                                accessed from Dcm_DspInternal_SvcFunc().
 * confirmedDiagSession         : Initialized in Dcm_HsmDcmSfTopEntry before being
 *                                accessed from Dcm_DspInternal_DiagnosticSessionControl_SvcH().
 * requestedResetMode           : Initialized in Dcm_HsmDcmSfTopEntry before being
 *                                accessed from Dcm_DspInternal_JumpToBL().
 * confirmedResetMode           : Initialized in Dcm_HsmDcmSfTopEntry before being
 *                                accessed from Dcm_DspInternal_JumpToBL().
 * serviceId                    : Initialized in Dcm_HsmDcmSfInServiceEntry before being
 *                                accessed from Dcm_DsdInternal_DispatchSvcReq().
 * subServiceId                 : Initialized in Dcm_HsmDcmSfInServiceEntry before being
 *                                accessed from Dcm_DsdInternal_ValidateSubSvcReq().
 * subServiceIdIdx              : Initialized in Dcm_DspInternal_ReadDTCInformation_SvcStart or
 *                                Dcm_DspInternal_ResponseOnEvent_SvcStart before being used in
 *                                Dcm_DspInternal_ReadDTCInformation_SvcCont or
 *                                Dcm_DspInternal_ResponseOnEvent_SvcCont.
 * negativeResponseCode         : Initialized in Dcm_HsmSfInServiceEntry()before being accessed
 *                                from Dcm_DsdInternal_DispatchSvcReq().
 * processPageFuncId            : Initialized in Dcm_HsmSfInServiceEntry before being accessed
 *                                from Dcm_DspInternal_ReadDTCInformation_SvcH
 * rpCntr                       : Initialized in Dcm_HsmDcmSfServiceCalledEntry before being
 *                                accessed from Dcm_HsmDcmSfWaitForApplGuard2() and
 *                                Dcm_HsmDcmSfWaitForApplGuard4().
 *                                Incremented in Dcm_HsmDcmSfSendRpEntry.
 * txDataState                  : Initialized in Dcm_HsmDcmSfInServiceEntry() before being
 *                                accessed from Dcm_CopyTxData().
 * responsePendingTxStatus      : Initialized in Dcm_HsmDcmSfTopEntry before being
 *                                accessed from Dcm_TpTxConfirmation().
 * jumpToBLSupport              : Initialized in Dcm_HsmDcmSfTopEntry before being
 *                                accessed from Dcm_DspInternal_DiagnosticSessionControl_SvcH().
 * jumpToBLExecution            : Initialized in Dcm_HsmDcmSfTopEntry before being
 *                                accessed from Dcm_DspInternal_JumpToBL,
 *                                Dcm_DspInternal_ProcessSetProgConditions() and the
 *                                Dcm_TpTxConfirmation() API.
 * dspService                   : Initialized in Dcm_HsmDcmSfInServiceEntry before being
 *                                accessed from Dcm_DspInternal_SetSessionState().
 */
VAR(Dcm_HsmInfoType, DCM_VAR) Dcm_HsmInfo[DCM_NUM_PROTOCOL];

/** \brief Allows the integrator to store relevant information prior to jumping to bootloader */
VAR(Dcm_ProgConditionsType, DCM_VAR) Dcm_ProgConditions;

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[internal data]==========================================*/



/*==================[external function definitions]==========================*/

#if (DCM_INCLUDE_RTE == STD_OFF)
#define DCM_START_SEC_CODE
#else
#define Dcm_START_SEC_CODE
#endif
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.EB.DCMServicesIf.Operation.GetSecurityLevel,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetSecurityLevel(
  P2VAR(Dcm_SecLevelType, AUTOMATIC, DCM_APPL_DATA) SecLevel)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_DCM_GETSECURITYLEVEL_ENTRY(SecLevel);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_GETSECURITYLEVEL, DCM_E_UNINIT);
  }
  else if (SecLevel == NULL_PTR)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_GETSECURITYLEVEL, DCM_E_PARAM_POINTER);
  }
  else
#endif /* DCM_DEV_ERROR_DETECT */
  {
    /* Get current security level*/
    *SecLevel = Dcm_Dsl_SecurityLevelHandler_GetSecurityLevel();

    retVal = E_OK;
  }

  DBG_DCM_GETSECURITYLEVEL_EXIT(retVal,SecLevel);
  return retVal;
}

/* !LINKSTO Dcm.EB.DCMServicesIf.Operation.GetSesCtrlType,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetSesCtrlType(
  P2VAR(Dcm_SesCtrlType, AUTOMATIC, DCM_APPL_DATA) SesCtrlType)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_DCM_GETSESCTRLTYPE_ENTRY(SesCtrlType);

  retVal = Dcm_GetSesCtrlTypeExt(SesCtrlType);

  DBG_DCM_GETSESCTRLTYPE_EXIT(retVal,SesCtrlType);
  return retVal;
}

#if (DCM_DSL_APPLICATION_DIAGNOSTIC_REQUESTS == STD_ON)
/* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.Dcm_InjectDiagnosticRequest,1*/
FUNC(BufReq_ReturnType, DCM_CODE) Dcm_InjectDiagnosticRequest
(
  PduIdType RxPduID,
  P2CONST(uint8, AUTOMATIC, DCM_VAR) RequestData,
  PduLengthType RequestLength
)
{
  BufReq_ReturnType InjectionResult = BUFREQ_E_NOT_OK;

/* Enable code only if feature is present */
#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.DcmInitialized.DET_Error,1 */
    DCM_DET_REPORT_ERROR(DCM_SVCID_INJECTOR, DCM_E_UNINIT);
  }
  else if(RequestData == NULL_PTR)
  {
    /* !LINKSTO Dcm.EB.ApplicationDiagnosticRequests.Enabled.InvalidRequest,1 */
    DCM_DET_REPORT_ERROR(DCM_SVCID_INJECTOR, DCM_E_PARAM_POINTER);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
    InjectionResult = Dcm_Dsl_ApplicationDiagnosticRequests_Injector(RxPduID,
                                                                     RequestData,
                                                                     RequestLength);
  }

  return InjectionResult;
}
#endif /* #if (DCM_DSL_APPLICATION_DIAGNOSTIC_REQUESTS == STD_ON) */

/* !LINKSTO Dcm.EB.DCMServicesIf.Operation.GetActiveProtocol,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetActiveProtocol(
  P2VAR(Dcm_ProtocolType, AUTOMATIC, DCM_APPL_DATA) ActiveProtocol)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_DCM_GETACTIVEPROTOCOL_ENTRY(ActiveProtocol);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_GETACTIVEPROTOCOL, DCM_E_UNINIT);
  }
  else if (ActiveProtocol == NULL_PTR)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_GETACTIVEPROTOCOL, DCM_E_PARAM_POINTER);
  }
  else
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */
  {
    /* ENTER critical section */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
    {
      Dcm_ProtocolConfigType CurProtocolConfig;

      retVal = Dcm_Dsl_Supervisor_GetLatestExternalUDSProtocolConfig(&CurProtocolConfig);

      if (retVal == DCM_E_OK)
      {
        *ActiveProtocol = CurProtocolConfig.Type;
      }
      else
      {
        *ActiveProtocol = DCM_NO_ACTIVE_PROTOCOL;
      }
    }
    /* LEAVE critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

    retVal = E_OK;
  }

  DBG_DCM_GETACTIVEPROTOCOL_EXIT(retVal,ActiveProtocol);
  return retVal;
}

/* !LINKSTO Dcm.ResetToDefaultSession.API,1 */
/* !LINKSTO Dcm.EB.DCMServicesIf.Operation.ResetToDefaultSession,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_ResetToDefaultSession(void)
{
  Std_ReturnType Retval = E_NOT_OK;

  DBG_DCM_RESETTODEFAULTSESSION_ENTRY();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    /* !LINKSTO Dcm.EB.ResetToDefaultSession.DET,2 */
    DCM_DET_REPORT_ERROR(DCM_SERVID_RESETTODEFAULTSESSION, DCM_E_UNINIT);
  }
  else
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */
  {
    Dcm_Dsl_DiagnosticSessionHandler_ResetToDefaultSession(
                      DCM_RESETTODEFAULTSESSIONREQUESTER_APPLICATION);
    Retval = E_OK;
  }

  DBG_DCM_RESETTODEFAULTSESSION_EXIT(Retval);

  return Retval;
}

/* !LINKSTO SWS_Dcm_00521,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_TriggerOnEvent(uint8 RoeEventId)
{
  Std_ReturnType apiRetVal = E_NOT_OK;

  DBG_DCM_TRIGGERONEVENT_ENTRY(RoeEventId);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_TRIGGERONEVENT , DCM_E_UNINIT);
  }
  else if (RoeEventId == 0xFFU)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_TRIGGERONEVENT, DCM_E_PARAM);
    apiRetVal = E_NOT_OK;
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
#if (DCM_DSP_USE_ROESERVICES == STD_ON)
    apiRetVal = Dcm_Dsl_RoeServices_TriggerOnEvent(RoeEventId);
#else
    TS_PARAM_UNUSED(RoeEventId);
#endif /* (DCM_DSP_USE_ROESERVICES == STD_ON) */
  }

  DBG_DCM_TRIGGERONEVENT_EXIT(apiRetVal, RoeEventId);
  return apiRetVal;
}

#if (DCM_INCLUDE_RTE == STD_OFF)
#define DCM_STOP_SEC_CODE
#else
#define Dcm_STOP_SEC_CODE
#endif
#include <Dcm_MemMap.h>

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

#if (DCM_VERSION_INFO_API == STD_ON)
FUNC(void, DCM_CODE) Dcm_GetVersionInfo(
  P2VAR(Std_VersionInfoType, AUTOMATIC, DCM_APPL_DATA) versioninfo)
{
  DBG_DCM_GETVERSIONINFO_ENTRY(versioninfo);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (versioninfo == NULL_PTR)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_GETVERSIONINFO, DCM_E_PARAM_POINTER);
  }
  else
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */
  {
    /* get version info of this module */
    versioninfo->vendorID         = DCM_VENDOR_ID;
    versioninfo->moduleID         = DCM_MODULE_ID;
    versioninfo->sw_major_version = DCM_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = DCM_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = DCM_SW_PATCH_VERSION;
  }

  DBG_DCM_GETVERSIONINFO_EXIT(versioninfo);
}

#endif /* DCM_VERSION_INFO_API */

/* !LINKSTO Dcm.Dsn.IB.GetLock,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Internal_GetLock(
  P2VAR(Dcm_MutexType, AUTOMATIC, DCM_VAR) MutexPtr
  )
{
  Std_ReturnType RetVal = DCM_E_RESOURCE_LOCKED;
  Dcm_MutexType PreviousMutexState = DCM_MUTEX_LOCKED;

  DBG_DCM_INTERNAL_GETLOCK_ENTRY(MutexPtr);

  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  /* Current state of the lock */
  PreviousMutexState = *MutexPtr;

  *MutexPtr = DCM_MUTEX_LOCKED;

  /* LEAVE critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  /* Check if the mutex was previously already locked */
  if (PreviousMutexState == DCM_MUTEX_LOCKED)
  {
    RetVal = DCM_E_RESOURCE_LOCKED;
  }
  else
  {
    RetVal = DCM_E_OK;
  }

  DBG_DCM_INTERNAL_GETLOCK_EXIT(RetVal);

  return RetVal;
}

/* !LINKSTO Dcm.Dsn.IB.ReleaseLock,1 */
FUNC(void, DCM_CODE) Dcm_Internal_ReleaseLock(
  P2VAR(Dcm_MutexType, AUTOMATIC, DCM_VAR) MutexPtr
  )
{
#if (DCM_DEV_ERROR_DETECT == STD_ON)
  Dcm_MutexType PreviousMutexState = DCM_MUTEX_LOCKED;
#endif
  DBG_DCM_INTERNAL_RELEASELOCK_ENTRY(*MutexPtr);

  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  /* Current state of the lock */
  PreviousMutexState = *MutexPtr;
#endif
  *MutexPtr = DCM_MUTEX_UNLOCKED;

  /* LEAVE critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  /* Check if the mutex wasn't previously already unlocked and report a Det error if
   * an attempt is made to unlock an already unlocked mutex */
  if (PreviousMutexState == DCM_MUTEX_UNLOCKED)
  {
    DCM_DET_REPORT_ERROR(DCM_INTERNAL_API_ID, DCM_E_RELEASELOCK_FAILED);
  }
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */

  DBG_DCM_INTERNAL_RELEASELOCK_EXIT(*MutexPtr);
}

FUNC(void, DCM_CODE) Dcm_MainFunction(void)
{
  DBG_DCM_MAINFUNCTION_ENTRY();

  if ( Dcm_Initialized == TRUE )
  {
    Dcm_MainFunction_Part1();
    Dcm_MainFunction_Part2();
  }

  DBG_DCM_MAINFUNCTION_EXIT();
}

/* !LINKSTO Dcm.DemTriggerOnDTCStatus.API,2 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DemTriggerOnDTCStatus(
  uint32 DTC,
  uint8 DTCStatusOld,
  uint8 DTCStatusNew)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_DCM_DEMTRIGGERONDTCSTATUS_ENTRY(DTC, DTCStatusOld, DTCStatusNew);
  TS_PARAM_UNUSED(DTC);
  TS_PARAM_UNUSED(DTCStatusOld);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_DEMTRIGGERONDTC, DCM_E_UNINIT);
    retVal = E_NOT_OK;
  }
  else
#endif
  {
#if (DCM_DSP_USE_ROESERVICES == STD_ON)
    Dcm_Dsl_RoeServices_TriggerOnDTCStatus(DTCStatusNew);
#else
    TS_PARAM_UNUSED(DTCStatusNew);
#endif /* (DCM_DSP_USE_ROESERVICES == STD_ON) */
    retVal = E_OK;
  }

  DBG_DCM_DEMTRIGGERONDTCSTATUS_EXIT(retVal, DTC, DTCStatusOld, DTCStatusNew);

  return retVal;
}
#if ((DCM_MODEDECLARATION_SUPPORT == STD_OFF) && (DCM_BSWM_API_ENABLED == STD_ON))
/* !LINKSTO Dcm.EB.ModeEntry.API,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_HardResetModeEntry(void)
{
  DBG_DCM_HARDRESETMODEENTRY_ENTRY();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_COMMON_COMM_MODE_ENTRY, DCM_E_UNINIT);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* get Hsm information */
    CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = Dcm_Dsp_GetCurrentHsmInfo();
    /* Update the confirmed Reset Mode */
    curHsmInfo->confirmedResetMode = DCM_HARD_RESET;
  }

  DBG_DCM_HARDRESETMODEENTRY_EXIT(E_OK);

  return E_OK;
}

/* !LINKSTO Dcm.EB.ModeEntry.API,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_KeyOnOffResetModeEntry(void)
{
  DBG_DCM_KEYONOFFRESETMODEENTRY_ENTRY();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_COMMON_COMM_MODE_ENTRY, DCM_E_UNINIT);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* get Hsm information */
    CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = Dcm_Dsp_GetCurrentHsmInfo();
    /* Update the confirmed Reset Mode */
    curHsmInfo->confirmedResetMode = DCM_KEY_ON_OFF_RESET;
  }

  DBG_DCM_KEYONOFFRESETMODEENTRY_EXIT(E_OK);

  return E_OK;
}

/* !LINKSTO Dcm.EB.ModeEntry.API,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_SoftResetModeEntry(void)
{
  DBG_DCM_SOFTRESETMODEENTRY_ENTRY();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_COMMON_COMM_MODE_ENTRY, DCM_E_UNINIT);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* get Hsm information */
    CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = Dcm_Dsp_GetCurrentHsmInfo();
    /* Update the confirmed Reset Mode */
    curHsmInfo->confirmedResetMode = DCM_SOFT_RESET;
  }

  DBG_DCM_SOFTRESETMODEENTRY_EXIT(E_OK);

  return E_OK;
}

/* !LINKSTO Dcm.EB.ModeEntry.API,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_EnableRapidPowerShutdownResetModeEntry(void)
{
  DBG_DCM_ENABLERAPIDPOWERSHUTDOWNRESETMODEENTRY_ENTRY();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_COMMON_COMM_MODE_ENTRY, DCM_E_UNINIT);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* get Hsm information */
    CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = Dcm_Dsp_GetCurrentHsmInfo();
    /* Update the confirmed Reset Mode */
    curHsmInfo->confirmedResetMode = DCM_ENABLE_RAPID_POWER_SHUTDOWN_RESET;
  }

  DBG_DCM_ENABLERAPIDPOWERSHUTDOWNRESETMODEENTRY_EXIT(E_OK);

  return E_OK;
}

/* !LINKSTO Dcm.EB.ModeEntry.API,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DisableRapidPowerShutdownResetModeEntry(void)
{
  DBG_DCM_DISABLERAPIDPOWERSHUTDOWNRESETMODEENTRY_ENTRY();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_COMMON_COMM_MODE_ENTRY, DCM_E_UNINIT);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
  /* get Hsm information */
  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = Dcm_Dsp_GetCurrentHsmInfo();
  /* Update the confirmed Reset Mode */
  curHsmInfo->confirmedResetMode = DCM_DISABLE_RAPID_POWER_SHUTDOWN_RESET;
  }

  DBG_DCM_DISABLERAPIDPOWERSHUTDOWNRESETMODEENTRY_EXIT(E_OK);

  return E_OK;
}

/* !LINKSTO Dcm.EB.ModeEntry.API,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_BootloaderResetModeEntry(void)
{
  DBG_DCM_BOOTLOADERRESETMODEENTRY_ENTRY();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_COMMON_COMM_MODE_ENTRY, DCM_E_UNINIT);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* get Hsm information */
    CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = Dcm_Dsp_GetCurrentHsmInfo();
    /* Update the confirmed Reset Mode */
    curHsmInfo->confirmedResetMode = DCM_BOOTLOADER_RESET;
  }

  DBG_DCM_BOOTLOADERRESETMODEENTRY_EXIT(E_OK);

  return E_OK;
}

/* !LINKSTO Dcm.EB.ModeEntry.API,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_SsBootloaderResetModeEntry(void)
{
  DBG_DCM_SSBOOTLOADERRESETMODEENTRY_ENTRY();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_COMMON_COMM_MODE_ENTRY, DCM_E_UNINIT);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
  /* get Hsm information */
  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = Dcm_Dsp_GetCurrentHsmInfo();
  /* Update the confirmed Reset Mode */
  curHsmInfo->confirmedResetMode = DCM_SS_BOOTLOADER_RESET;
  }

  DBG_DCM_SSBOOTLOADERRESETMODEENTRY_EXIT(E_OK);

  return E_OK;
}

#if(DCM_DSP_USE_SERVICE_0X28 == STD_ON)

/* !LINKSTO Dcm.EB.ModeEntry.API,1 */
/* !LINKSTO Dcm.EB.CommunicationControlEnableRxTxNorm.ModeEntry,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_EnableRxTxNormModeEntry(void)
{
  DBG_DCM_ENABLERXTXNORMMODEENTRY_ENTRY();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_COMMON_COMM_MODE_ENTRY, DCM_E_UNINIT);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Update the confirmed Communication Mode */
    Dcm_Dsp_CommunicationControl_SetConfirmedCommunicationMode(DCM_ENABLE_RX_TX_NORM);
  }

  DBG_DCM_ENABLERXTXNORMMODEENTRY_EXIT(E_OK);

  return E_OK;
}

/* !LINKSTO Dcm.EB.ModeEntry.API,1 */
/* !LINKSTO Dcm.EB.CommunicationControlEnableRxDisableTxNorm.ModeEntry,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_EnableRxDisableTxNormModeEntry(void)
{
  DBG_DCM_ENABLERXDISABLETXNORMMODEENTRY_ENTRY();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_COMMON_COMM_MODE_ENTRY, DCM_E_UNINIT);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Update the confirmed Communication Mode */
    Dcm_Dsp_CommunicationControl_SetConfirmedCommunicationMode(DCM_ENABLE_RX_DISABLE_TX_NORM);
  }

  DBG_DCM_ENABLERXDISABLETXNORMMODEENTRY_EXIT(E_OK);

  return E_OK;
}

/* !LINKSTO Dcm.EB.ModeEntry.API,1 */
/* !LINKSTO Dcm.EB.CommunicationControlDisableRxEnableTxNorm.ModeEntry,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DisableRxEnableTxNormModeEntry(void)
{
  DBG_DCM_DISABLERXENABLETXNORMMODEENTRY_ENTRY();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_COMMON_COMM_MODE_ENTRY, DCM_E_UNINIT);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Update the confirmed Communication Mode */
    Dcm_Dsp_CommunicationControl_SetConfirmedCommunicationMode(DCM_DISABLE_RX_ENABLE_TX_NORM);
  }

  DBG_DCM_DISABLERXENABLETXNORMMODEENTRY_EXIT(E_OK);

  return E_OK;
}

/* !LINKSTO Dcm.EB.ModeEntry.API,1 */
/* !LINKSTO Dcm.EB.CommunicationControlDisableRxTxNorm.ModeEntry,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DisableRxTxNormModeEntry(void)
{
  DBG_DCM_DISABLERXTXNORMMODEENTRY_ENTRY();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_COMMON_COMM_MODE_ENTRY, DCM_E_UNINIT);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Update the confirmed Communication Mode */
    Dcm_Dsp_CommunicationControl_SetConfirmedCommunicationMode(DCM_DISABLE_RX_TX_NORM);
  }

  DBG_DCM_DISABLERXTXNORMMODEENTRY_EXIT(E_OK);

  return E_OK;
}

/* !LINKSTO Dcm.EB.ModeEntry.API,1 */
/* !LINKSTO Dcm.EB.CommunicationControlEnableRxTxNm.ModeEntry,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_EnableRxTxNmModeEntry(void)
{
  DBG_DCM_ENABLERXTXNMMODEENTRY_ENTRY();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_COMMON_COMM_MODE_ENTRY, DCM_E_UNINIT);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Update the confirmed Communication Mode */
    Dcm_Dsp_CommunicationControl_SetConfirmedCommunicationMode(DCM_ENABLE_RX_TX_NM);
  }

  DBG_DCM_ENABLERXTXNMMODEENTRY_EXIT(E_OK);

  return E_OK;
}

/* !LINKSTO Dcm.EB.ModeEntry.API,1 */
/* !LINKSTO Dcm.EB.CommunicationControlEnableRxDisableTxNm.ModeEntry,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_EnableRxDisableTxNmModeEntry(void)
{
  DBG_DCM_ENABLERXDISABLETXNMMODEENTRY_ENTRY();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_COMMON_COMM_MODE_ENTRY, DCM_E_UNINIT);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Update the confirmed Communication Mode */
    Dcm_Dsp_CommunicationControl_SetConfirmedCommunicationMode(DCM_ENABLE_RX_DISABLE_TX_NM);
  }

  DBG_DCM_ENABLERXDISABLETXNMMODEENTRY_EXIT(E_OK);

  return E_OK;
}

/* !LINKSTO Dcm.EB.ModeEntry.API,1 */
/* !LINKSTO Dcm.EB.CommunicationControlDisableRxEnableTxNm.ModeEntry,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DisableRxEnableTxNmModeEntry(void)
{
  DBG_DCM_DISABLERXENABLETXNMMODEENTRY_ENTRY();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_COMMON_COMM_MODE_ENTRY, DCM_E_UNINIT);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Update the confirmed Communication Mode */
    Dcm_Dsp_CommunicationControl_SetConfirmedCommunicationMode(DCM_DISABLE_RX_ENABLE_TX_NM);
  }

  DBG_DCM_DISABLERXENABLETXNMMODEENTRY_EXIT(E_OK);

  return E_OK;
}

/* !LINKSTO Dcm.EB.ModeEntry.API,1 */
/* !LINKSTO Dcm.EB.CommunicationControlDisableRxTxNm.ModeEntry,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DisableRxTxNmModeEntry(void)
{
  DBG_DCM_DISABLERXTXNMMODEENTRY_ENTRY();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_COMMON_COMM_MODE_ENTRY, DCM_E_UNINIT);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Update the confirmed Communication Mode */
    Dcm_Dsp_CommunicationControl_SetConfirmedCommunicationMode(DCM_DISABLE_RX_TX_NM);
  }

  DBG_DCM_DISABLERXTXNMMODEENTRY_EXIT(E_OK);

  return E_OK;
}

/* !LINKSTO Dcm.EB.ModeEntry.API,1 */
/* !LINKSTO Dcm.EB.CommunicationControlEnableRxTxNormNm.ModeEntry,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_EnableRxTxNormNmModeEntry(void)
{
  DBG_DCM_ENABLERXTXNORMNMMODEENTRY_ENTRY();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_COMMON_COMM_MODE_ENTRY, DCM_E_UNINIT);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Update the confirmed Communication Mode */
    Dcm_Dsp_CommunicationControl_SetConfirmedCommunicationMode(DCM_ENABLE_RX_TX_NORM_NM);
  }

  DBG_DCM_ENABLERXTXNORMNMMODEENTRY_EXIT(E_OK);

  return E_OK;
}

/* !LINKSTO Dcm.EB.ModeEntry.API,1 */
/* !LINKSTO Dcm.EB.CommunicationControlEnableRxDisableTxNormNm.ModeEntry,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_EnableRxDisableTxNormNmModeEntry(void)
{
  DBG_DCM_ENABLERXDISABLETXNORMNMMODEENTRY_ENTRY();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_COMMON_COMM_MODE_ENTRY, DCM_E_UNINIT);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Update the confirmed Communication Mode */
    Dcm_Dsp_CommunicationControl_SetConfirmedCommunicationMode(DCM_ENABLE_RX_DISABLE_TX_NORM_NM);
  }

  DBG_DCM_ENABLERXDISABLETXNORMNMMODEENTRY_EXIT(E_OK);

  return E_OK;
}

/* !LINKSTO Dcm.EB.ModeEntry.API,1 */
/* !LINKSTO Dcm.EB.CommunicationControlDisableRxEnableTxNormNm.ModeEntry,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DisableRxEnableTxNormNmModeEntry(void)
{
  DBG_DCM_DISABLERXENABLETXNORMNMMODEENTRY_ENTRY();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_COMMON_COMM_MODE_ENTRY, DCM_E_UNINIT);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Update the confirmed Communication Mode */
    Dcm_Dsp_CommunicationControl_SetConfirmedCommunicationMode(DCM_DISABLE_RX_ENABLE_TX_NORM_NM);
  }

  DBG_DCM_DISABLERXENABLETXNORMNMMODEENTRY_EXIT(E_OK);

  return E_OK;
}

/* !LINKSTO Dcm.EB.ModeEntry.API,1 */
/* !LINKSTO Dcm.EB.CommunicationControlDisableRxTxNormNm.ModeEntry,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DisableRxTxNormNmModeEntry(void)
{
  DBG_DCM_DISABLERXTXNORMNMMODEENTRY_ENTRY();

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_COMMON_COMM_MODE_ENTRY, DCM_E_UNINIT);
  }
  else
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  {
    /* Update the confirmed Communication Mode */
    Dcm_Dsp_CommunicationControl_SetConfirmedCommunicationMode(DCM_DISABLE_RX_TX_NORM_NM);
  }

  DBG_DCM_DISABLERXTXNORMNMMODEENTRY_EXIT(E_OK);

  return E_OK;
}

#endif /* #if(DCM_DSP_USE_SERVICE_0X28 == STD_ON) */
#endif /* ((DCM_MODEDECLARATION_SUPPORT == STD_OFF) && (DCM_BSWM_API_ENABLED == STD_ON)) */

/* !LINKSTO Dcm.EB.DCMServicesIf.Operation.GetSesCtrlType,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_GetSesCtrlTypeExt(
  P2VAR(Dcm_SesCtrlType, AUTOMATIC, DCM_APPL_DATA) SesCtrlType)
{
  Std_ReturnType retVal = E_NOT_OK;

  DBG_DCM_GETSESCTRLTYPEEXT_ENTRY(SesCtrlType);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_GETSESCTRLTYPE, DCM_E_UNINIT);
  }
  else if (SesCtrlType == NULL_PTR)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_GETSESCTRLTYPE, DCM_E_PARAM_POINTER);
  }
  else
#endif /* DCM_DEV_ERROR_DETECT */
  {
    /* ENTER critical section. */
    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
    {
      /* get SesCtrlType */
      *SesCtrlType = Dcm_Dsl_DiagnosticSessionHandler_GetCurrentSession();
      retVal = E_OK;
    }
    /* LEAVE critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
  }

  DBG_DCM_GETSESCTRLTYPEEXT_EXIT(retVal,SesCtrlType);
  return retVal;
}

/*==================[internal function definitions]==============================================*/

#if (DCM_DSP_USE_SERVICE_0X22 == STD_ON) || \
    (DCM_DSP_USE_SERVICE_0X2E == STD_ON) || \
    (DCM_DSP_USE_SERVICE_0X2F == STD_ON) || \
    (DCM_DSP_USE_SERVICE_0X2A == STD_ON) || \
    (DCM_DSP_USE_SERVICE_0X2C == STD_ON)
/* Initializes the control structures for services using DID elements */
STATIC FUNC(void, DCM_CODE) Dcm_DIDInit(void)
{
#if (((DCM_DSP_USE_SERVICE_0X22 == STD_ON) && (DCM_READ_DID_BLOCK_ID_CONFIGURED == STD_ON)) || \
    ((DCM_DSP_USE_SERVICE_0X2E == STD_ON) && (DCM_WRITE_DID_BLOCK_ID_CONFIGURED == STD_ON)))
  /* initialize the signal buffer NvM context for DIDs */
  Dcm_Dsp_DidSignals_SignalBufferContextNvM_Init();
#endif

#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
  Dcm_Dsp_ReadDataByPeriodicIdentifier_Init();
#endif /* (DCM_DSP_USE_SERVICE_0X2A == STD_ON) */

#if (DCM_DSP_USE_SERVICE_0X2C == STD_ON)
  Dcm_Dsp_DidServices_DynamicDid_Init();
#endif

#if (DCM_DSP_USE_SERVICE_0X2F == STD_ON)
  Dcm_Dsp_InputOutputControlByIdentifier_Init();
  Dcm_Dsp_InputOutputControlByIdentifier_SetReturnControlToEcuStatus(DCM_RET_CTRL_INACTIVE);
#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
  Dcm_Dsp_InputOutputControlByIdentifier_SetReturnControlToEcuDefaultStatus(DCM_RET_CTRL_DEFAULT_INACTIVE);
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */
#endif /* #if (DCM_DSP_USE_SERVICE_0X2F == STD_ON) */

#if (DCM_DSP_USE_SERVICE_0X22 == STD_ON)
  Dcm_Dsp_ReadDataByIdentifier_Init();
#endif /* #if (DCM_DSP_USE_SERVICE_0X22 == STD_ON) */

#if (DCM_DSP_USE_SERVICE_0X2E == STD_ON)
  Dcm_Dsp_WriteDataByIdentifier_Init();
#endif /* #if (DCM_DSP_USE_SERVICE_0X2E == STD_ON) */

}
#endif /* #if (DCM_DSP_USE_SERVICE_0X22 == STD_ON) || \
              (DCM_DSP_USE_SERVICE_0X2E == STD_ON) || \
              (DCM_DSP_USE_SERVICE_0X2F == STD_ON) || \
              (DCM_DSP_USE_SERVICE_0X2A == STD_ON) */


/* definition of Dcm_Init */
FUNC(void, DCM_CODE) Dcm_Init(
  P2CONST(Dcm_ConfigType, AUTOMATIC, DCM_APPL_CONST) ConfigPtr)
{
  DBG_DCM_INIT_ENTRY(ConfigPtr);

  TS_PARAM_UNUSED(ConfigPtr);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  /* !LINKSTO Dcm.EB.BasicInit.AlreadyInit.DET,1 */
  if (TRUE == Dcm_Initialized)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_INIT, DCM_E_CONDITIONSNOTCORRECT);
  }
  else
#endif /* DCM_DEV_ERROR_DETECT */
  {
    Dcm_Dsl_DiagnosticSessionHandler_Init();

    Dcm_Dsl_Supervisor_Init();

    /* Initializes the Dsp elements */
    Dcm_Dsp_Init();

#if(DCM_DSP_USE_SERVICE_0X06 == STD_ON)
    Dcm_Dsp_MidServices_Init();
#endif

#if (DCM_DSP_USE_ROESERVICES == STD_ON)
    /* !LINKSTO SWS_Dcm_00872,1 */
    Dcm_Dsl_RoeServices_Init();
#endif
#if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) ||\
     (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X35 == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X3D == STD_ON) ||\
     (DCM_DSP_USE_SERVICE_0X34 == STD_ON))
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    Dcm_Dsl_MemoryServices_Init();
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
#endif /* #if ((DCM_DSP_USE_SERVICE_0X23 == STD_ON) ||\
           (DCM_0X2C_0X02_SSVC_ENABLED == STD_ON) ||\
           (DCM_DSP_USE_SERVICE_0X35 == STD_ON) ||\
           (DCM_DSP_USE_SERVICE_0X3D == STD_ON) ||\
           (DCM_DSP_USE_SERVICE_0X34 == STD_ON)) */

    Dcm_Dsl_Init();

    /* Initialize all ComM Channels that Dcm uses to NoCommunicationMode */
    Dcm_Dsl_ComMChannelStateManager_Init();

    Dcm_Dsl_SecurityLevelHandler_Init();

    Dcm_Dsl_PostBootOperationsHandler_Init();

    Dcm_Initialized = TRUE;
  }

  DBG_DCM_INIT_EXIT(ConfigPtr);
}

/* Deviation MISRAC2012-1 */
STATIC FUNC(void, DCM_CODE) Dcm_Internal_ProcessRetryTimer(DCM_PDL_SF(const uint8 instIdx))
{
  /* get the context information for the current instance */
  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo =
    &(DCM_HSM_INFO(instIdx));


  /* ENTER critical section
     Protect timers from preemption */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  /* if Dsp retry timer is running */
  if (curHsmInfo->retryTimer > 0U)
  {
    /* The only admitted non-zero value for the RetryTimer is 1 */
    DCM_PRECONDITION_ASSERT((curHsmInfo->retryTimer == 1U),
    DCM_INTERNAL_API_ID);

    /* Emit the event RETRY to Dsp */
    (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(instIdx), DCM_HSM_DCM_EV_RETRY);

    --curHsmInfo->retryTimer;
  }

  /* LEAVE critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

}

#if (DCM_COMM_MULTICORE_ENABLED == STD_ON)
/* CHECK: NOPARSE
 * Dcm_HighestPrioFunction() context is configured as caller context for
 * SchM_Call_ComM_Dcm_ActiveDiagnostic and SchM_Call_ComM_Dcm_InactiveDiagnostic. */
FUNC(void, DCM_CODE) Dcm_HighestPrioFunction(void)
{
}
/* CHECK: PARSE */
#endif

#if (DCM_RETURNCONTROLTOECU_ENABLED == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Internal_ProcessReturnControlToECU(
  void
)
{
  const Dcm_DidReturnControlToEcuStatusType ReturnCtrlToECUStatus
    = Dcm_Dsp_InputOutputControlByIdentifier_GetReturnControlToEcuStatus();
#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
  const uint8 ReturnCtrlToECUDefaultStatus
    = Dcm_Dsp_InputOutputControlByIdentifier_GetReturnControlToEcuDefaultStatus();
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */

  /* Process returnControlToEcu if not completed */
#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
  if((ReturnCtrlToECUStatus == DCM_RET_CTRL_PENDING)
       || (DCM_RET_CTRL_DEFAULT_PENDING == ReturnCtrlToECUDefaultStatus))
#else
  if(ReturnCtrlToECUStatus == DCM_RET_CTRL_PENDING)
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */
  {
#if (DCM_DSP_RETURN_CONTROLTOECU_TIMEOUT_REPORT == STD_ON)
    const uint16 TimeoutCount
      = Dcm_Dsp_InputOutputControlByIdentifier_GetReturnControlToEcuTimeoutCount();

    /* Processing time for returncontroltoecu operation expired */
    if(TimeoutCount == DCM_DSP_RETURN_CONTROLTOECU_TIMEOUT)
    {
      /* Reset all the structure variables as the timeout occur */
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      DCM_DET_REPORT_ERROR(DCM_SVCID_RETURNCONTROLTOECU, DCM_E_RETURNCONTROLTOECU_TIMEOUT);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
      if(DCM_RET_CTRL_DEFAULT_PENDING == ReturnCtrlToECUDefaultStatus)
      {
        /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.ReturnControlToECUForAll.DefaultSession.CANCEL,1 */
        /* The timer has expired. Call the function with with DCM_CANCEL OpStatus. */
        (void)Dcm_DefaultDIDReturnControlToECUForAll(DCM_CANCEL);
        /* Reset the counter and initialize the configured DIDs. */
        Dcm_Dsp_InputOutputControlByIdentifier_Init();
        /* Set the status of the ReturnControlToECUForAllSignals for the unconfigured DIDs as Finished. */
        Dcm_Dsp_InputOutputControlByIdentifier_SetReturnControlToEcuDefaultStatus(DCM_RET_CTRL_DEFAULT_FINISHED);
      }
      else
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */
      {
        Dcm_Dsp_InputOutputControlByIdentifier_Init();
        Dcm_Dsp_InputOutputControlByIdentifier_SetReturnControlToEcuStatus(DCM_RET_CTRL_FINISHED);
        /* Unlock DidServices */
        Dcm_Dsp_DidServices_UnlockService();
      }
    }
    else
#endif /* #if (DCM_DSP_RETURN_CONTROLTOECU_TIMEOUT_REPORT == STD_ON) */
    {
      Dcm_Dsp_InputOutputControlByIdentifier_ReturnControlToEcuForAllSignals();
    }
  }
}
#endif /* #if (DCM_RETURNCONTROLTOECU_ENABLED == STD_ON) */

#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Internal_ProcessAsynchronousProtocolCancellation(
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  if (DCM_ASYNC_SERVICE_HANDLER_CANCEL_IN_NEXT_CYCLE ==
    DCM_HSM_INFO(ProtocolIndex).CancellationConditions.RetryAsyncServiceCancellation)
  {
    /* Retry calling the service handler with the DCM_CANCEL OpStatus */
    Dcm_Dsp_ProtocolCancelProcessing(ProtocolIndex);
  }
  else
  {
    /* Simply inquire the state of the asynchronous service handler. Since the
       service handler dispatcher waits until the asynchronous service handler
       is finished processing to take action, trigger one single polling
       of the asynchronous service handler state. */
    Dcm_DspInternal_SvcFunc(DCM_CUR_INST_ID(ProtocolIndex), DCM_PENDING);
  }
}
#endif /* #if (DCM_ASYNC_SERVICE_HANDLER == STD_ON) */

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_Init(void)
{
#if (DCM_DSP_USE_DIDSERVICES == STD_ON)
  Dcm_Dsp_DidServices_Init();
#endif

  /* Trigger initial transitions of all state machines */
  Dcm_HsmInit(&Dcm_HsmScDcm);

  Dcm_Dsp_ServiceProcessorInit();

#if (DCM_DSP_USE_SERVICE_0X28 == STD_ON)
  Dcm_Dsp_CommunicationControl_Init();
#endif

#if (DCM_DSP_USE_SERVICE_0X27 == STD_ON)
  Dcm_Dsp_SecurityAccess_Init();
#endif

#if (DCM_DSP_USE_SERVICE_0X31 == STD_ON)
  Dcm_Dsp_RoutineControl_Init();
#endif

#if(DCM_DSP_USE_SERVICE_0X34 == STD_ON) || \
   (DCM_DSP_USE_SERVICE_0X35 == STD_ON) || \
   (DCM_DSP_USE_SERVICE_0X36 == STD_ON) || \
   (DCM_DSP_USE_SERVICE_0X37 == STD_ON)
  Dcm_UploadDownloadInit();
#endif /* #if(DCM_DSP_USE_SERVICE_0X34 == STD_ON) || \
             (DCM_DSP_USE_SERVICE_0X35 == STD_ON) || \
             (DCM_DSP_USE_SERVICE_0X36 == STD_ON) || \
             (DCM_DSP_USE_SERVICE_0X37 == STD_ON) */

#if(DCM_DSP_USE_SERVICE_0X87 == STD_ON)
  Dcm_Dsp_LinkControl_Init();
#endif

#if (DCM_DSP_USE_SERVICE_0X22 == STD_ON) || \
    (DCM_DSP_USE_SERVICE_0X2E == STD_ON) || \
    (DCM_DSP_USE_SERVICE_0X2F == STD_ON) || \
    (DCM_DSP_USE_SERVICE_0X2A == STD_ON) || \
    (DCM_DSP_USE_SERVICE_0X2C == STD_ON)
  Dcm_DIDInit();
#endif /* #if (DCM_DSP_USE_SERVICE_0X22 == STD_ON) || \
              (DCM_DSP_USE_SERVICE_0X2E == STD_ON) || \
              (DCM_DSP_USE_SERVICE_0X2F == STD_ON) || \
              (DCM_DSP_USE_SERVICE_0X2A == STD_ON) || \
              (DCM_DSP_USE_SERVICE_0X2C == STD_ON) */

#if (DCM_DSP_USE_DEMFACADE == STD_ON)
  Dcm_Dsp_DemFacade_Init();
#endif

#if (DCM_DSP_USE_SERVICE_0X85 == STD_ON)
  Dcm_Dsp_ControlDTCSettingState_Init();
#endif

#if (DCM_DSL_APPLICATION_DIAGNOSTIC_REQUESTS == STD_ON)
  Dcm_Dsl_ApplicationDiagnosticRequests_Init();
#endif
}

STATIC FUNC(void, DCM_CODE) Dcm_MainFunction_Part1(void)
{
  DBG_DCM_MAINFUNCTION_PART1_ENTRY();
#if (DCM_RETURNCONTROLTOECU_ENABLED == STD_ON)
  /* !LINKSTO Dcm.EB.IOControl.ProtocolPreemption.ReturnControlToEcu.CallInterfaces,1 */
  Dcm_Internal_ProcessReturnControlToECU();
#endif

#if(DCM_DSP_USE_SERVICE_0X85 == STD_ON)
  Dcm_Dsp_ControlDTCSetting_MainFunction();
#endif

/* !LINKSTO Dcm.PeriodicDid.Dsn.SchedInvocation,1 */
#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
  /* Sample and schedule for transmission periodic DIDs */
  Dcm_Dsp_ReadDataByPeriodicIdentifier_Schedule();
#endif /* DCM_DSP_USE_SERVICE_0X2A == STD_ON */

#if (DCM_DSP_USE_ROESERVICES == STD_ON)
  Dcm_Dsl_RoeServices_MainFunction();
#endif /* (DCM_DSP_USE_ROESERVICES == STD_ON) */

#if (DCM_DSL_APPLICATION_DIAGNOSTIC_REQUESTS == STD_ON)
  Dcm_Dsl_ApplicationDiagnosticRequests_MainFunction();
#endif

  Dcm_Dsl_PostBootOperationsHandler_MainFunction();

  Dcm_Dsl_Supervisor_PreProcessingMainFunction();

  /* session timer handling */
  Dcm_Dsl_DiagnosticSessionHandler_S3TimerHandlingMainFunction();

#if (DCM_DSP_COMMUNICATIONCONTROL_REENABLE_MODE_RULE_SUPPORT == STD_ON)
  Dcm_Dsp_CommunicationControl_ProcessModeRules();
#endif /* #if (DCM_DSP_COMMUNICATIONCONTROL_REENABLE_MODE_RULE_SUPPORT == STD_ON) */

  DBG_DCM_MAINFUNCTION_PART1_EXIT();

}

STATIC FUNC(void, DCM_CODE) Dcm_MainFunction_Part2(void)
{
#if ((DCM_HSM_DCM_NUM_INST > 1U) || ((DCM_MODEDECLARATION_SUPPORT == STD_ON) && \
    (DCM_DSP_COMMUNICATIONCONTROL_REENABLE_MODE_RULE_SUPPORT == STD_ON)))
  uint8_least i = 0U;
#endif /* #if ((DCM_HSM_DCM_NUM_INST > 1U) || ((DCM_MODEDECLARATION_SUPPORT == STD_ON) && \
              (DCM_DSP_COMMUNICATIONCONTROL_REENABLE_MODE_RULE_SUPPORT == STD_ON))) */

  DBG_DCM_MAINFUNCTION_PART2_ENTRY();

#if (DCM_HSM_DCM_NUM_INST > 1U)
  for (i = 0U; i<DCM_HSM_DCM_NUM_INST; ++i)
#endif
  {
#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
    /* In case an asynchronous service handler is still running in the background,
       and it needs to be cancelled, reattempt cancellation. */
    if (DCM_ASYNC_SERVICE_HANDLER_INVALID_CANCELLATION !=
        DCM_HSM_INFO(i).CancellationConditions.RetryAsyncServiceCancellation)
    {
      Dcm_Internal_ProcessAsynchronousProtocolCancellation(
          (Dcm_ProtocolIndexType)(DCM_CUR_INST_ID(i)));
    }
#endif /* #if (DCM_ASYNC_SERVICE_HANDLER == STD_ON) */
    /* process retry timer */
    Dcm_Internal_ProcessRetryTimer(DCM_PL_SF((uint8)i));
  }

#if (((DCM_DSP_USE_SERVICE_0X22 == STD_ON) && (DCM_READ_DID_BLOCK_ID_CONFIGURED == STD_ON)) || \
    ((DCM_DSP_USE_SERVICE_0X2E == STD_ON) && (DCM_WRITE_DID_BLOCK_ID_CONFIGURED == STD_ON)))
  Dcm_Dsp_DidSignals_SignalBufferOperationsPoll();
#endif

#if ((DCM_DSP_USE_SERVICE_0X2C == STD_ON) && \
     (DCM_DDDID_PERSISTENCE == STD_ON))
  Dcm_Dsp_DidServices_DynamicDid_InitPersistent();
#endif

#if (DCM_DSP_USE_SERVICE_0X27 == STD_ON)
#if (DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U)
#if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON)
  Dcm_Dsp_SecurityAccess_MainFunction();
#endif /* #if (DCM_DSP_SECURITYACCESS_DELAY_TIMERS_ENABLED == STD_ON) */
#endif /* #if (DCM_NUM_CONFIGURED_SECURITY_LEVELS > 0U) */
#endif /* #if (DCM_DSP_USE_SERVICE_0X27 == STD_ON) */

  Dcm_Dsl_TxConnectionProcessTxConnections();

  /* Main processing function of HSM for multiple state charts */
  Dcm_HsmMain(&Dcm_HsmScDcm);

  /* protocol timer handling */
  Dcm_Dsl_DiagnosticSessionHandler_P2TimerHandlingMainFunction();

  Dcm_Dsl_Supervisor_PostProcessingMainFunction();

  DBG_DCM_MAINFUNCTION_PART2_EXIT();
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[end of file]================================================================*/
