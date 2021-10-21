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

/* !LINKSTO Dcm.Dsn.File.RoutineControl.Impl,1 */
/* This file contains the implementation of the Diagnostic Service RoutineControl */


/*===============================[includes]======================================================*/

#include <Dcm_Trace.h>
#include <TSAutosar.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <Dcm_DspExternal.h>
#include <TSMem.h>

#include <Dcm_Dsp_SvcH_RoutineControl.h>
#include <Dcm_Dsl_SecurityLevelHandler.h>
#if(DCM_DSP_USE_SERVICE_0X08 == STD_ON)
#include <Dcm_Dsp_SvcH_RequestControlOfOnBoardSysTstComp.h>
#endif

#if (DCM_DSP_USE_SERVICE_0X31 == STD_ON)

/*==================[macro definitions]==========================================================*/

#if (defined DCM_ROUTINE_CONTROL_START)
  #error "DCM_ROUTINE_CONTROL_START is already defined"
#endif
/** \brief Macro for routine control subfunction Start */
#define DCM_ROUTINE_CONTROL_START 1U

#if (defined DCM_ROUTINE_CONTROL_STOP)
  #error "DCM_ROUTINE_CONTROL_STOP is already defined"
#endif
/** \brief Macro for routine control subfunction Stop */
#define DCM_ROUTINE_CONTROL_STOP 2U

#if (defined DCM_ROUTINE_CONTROL_REQUESTRESULTS)
  #error "DCM_ROUTINE_CONTROL_REQUESTRESULTS is already defined"
#endif
/** \brief Macro for routine control subfunction Request Results */
#define DCM_ROUTINE_CONTROL_REQUESTRESULTS 3U

#if (defined DCM_ROUTINE_CONTROL_MIN_REQ_LENGTH)
  #error "DCM_ROUTINE_CONTROL_MIN_REQ_LENGTH is already defined"
#endif
/** \brief  Macro for routine control subfunction Request Results */
#define DCM_ROUTINE_CONTROL_MIN_REQ_LENGTH 3U

#if (defined DCM_ROUTINE_CONTROL_RESPONS_HEADER_SIZE)
  #error "DCM_ROUTINE_CONTROL_RESPONS_HEADER_SIZE is already defined"
#endif
/** \brief Macro for size of response header, SubFunction 1 byte and RoutineID 2 bytes */
#define DCM_ROUTINE_CONTROL_RESPONS_HEADER_SIZE 3U

#if (defined DCM_ROUTINE_CONTROL_INFOBYTE_SIZE)
  #error "DCM_ROUTINE_CONTROL_INFOBYTE_SIZE is already defined"
#endif
/** \brief Macro for size of InfoByte */
#define DCM_ROUTINE_CONTROL_INFOBYTE_SIZE       8U

/*==================[type definitions]===========================================================*/

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief UDS service 0x31 internal service handler
 **
 ** \param[in] OpStatus Status of the service processing
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP  The DCM shall send a "response pending" immediately
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RoutineControl_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief UDS service 0x31 internal service handler
 **
 ** \param[in] OpStatus Status of the service processing
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP  The DCM shall send a "response pending" immediately
 */

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RoutineControl_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#if (DCM_NUM_ROUTINES > 0)
/** \brief This function searches the RoutineId in the configuration and gets the routine
 **  configuration index
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval TRUE          Routine was found in the configuration
 ** \retval FALSE         Routine was not found in the configuration
 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_RoutineControl_SetCurrentRoutine(
  P2CONST(uint8, AUTOMATIC, DCM_VAR) RequestData);

/** \brief This function checks if the routine is allowed to be executed in the current session
 **
 ** \retval TRUE          Routine is allowed in the current session
 ** \retval FALSE         Routine is not allowed in the current session
 */
STATIC FUNC (boolean, DCM_CODE) Dcm_Dsp_RoutineControl_IsRoutineAvailableInCurrentSession(void);

/** \brief This function checks if the routine is allowed to be executed in the current security level
 **
 ** \retval TRUE          Routine is allowed in the current security level
 ** \retval FALSE         Routine is not allowed in the current security level
 */
STATIC FUNC (boolean, DCM_CODE) Dcm_Dsp_RoutineControl_IsRoutineAvailableInCurrentSecurityLevel(void);
#endif /*#if (DCM_NUM_ROUTINES > 0)*/

#if(DCM_DSP_USE_SERVICE_0X08 == STD_ON)
/** \brief This function checks if the Routine Id is in the OBD range and calls the
 **        handler shell for OBD
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval TRUE         The routine is in OBD range
 ** \retval FALSE        The routine is not in OBD range
 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_RoutineControl_ProcessRoutineInOBDRange(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /*#if(DCM_DSP_USE_SERVICE_0X08 == STD_ON)*/

#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
/** \brief This function checks if the routine is allowed to be executed in the current mode condition
 **
 ** \retval TRUE          Routine can be executed in the current mode condition
 ** \retval FALSE         Routine can not be executed in the current mode condition
 */
STATIC FUNC (boolean, DCM_CODE) Dcm_Dsp_RoutineControl_ModeRuleResult(
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) CurrentNrc);
#endif /*DCM_MODEDECLARATION_SUPPORT */

#if (DCM_ROUTINE_DEFAULT_INTERFACES == STD_ON)
/** \brief This function calls a default Application function for every requested routine
 **        which is not supported and handles the API function result.
 **
 ** \param[in]    OpStatus Operation mode of the function.
 ** \param[in]    SubServiceID SubService for the unsupported routine.
 ** \param[out]   pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RoutineControl_UnsupportedRoutineHandler(
  uint8 SubServiceID,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /*(DCM_ROUTINE_DEFAULT_INTERFACES == STD_ON)*/

#if (DCM_ROUTINE_ROUTINEINFOBYTE == STD_ON)
/** \brief This function is used to handle byte shifting of the routine response info in case
 **         there is added one more byte to the response using the application interface
 **         Dcm_AddRoutineInfoByte
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval NONE
 */
  STATIC FUNC(void, DCM_CODE) Dcm_Dsp_RoutineControl_RoutineInfoByteHandler(
    P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if (DCM_ROUTINE_ROUTINEINFOBYTE == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[external constants declarations]============================================*/

/*===================[internal constants declarations]===========================================*/

/*==================[external data declarations]=================================================*/

/*==================[internal data declarations]=================================================*/
#if (DCM_ROUTINE_DEFAULT_INTERFACES == STD_ON)

#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/** \brief This global variable is used to identify if there it was requested
 *         a routine which is not supported (not configured or not used).
 *         It is set as TRUE if DcmDspRoutineEnableDefaultInterfaces
 *         configuration parameter is set to TRUE and an unconfigured or an unused routine
 *         is found.
 *         It is set to FALSE at initialization or after the Default Interfaces
 *         (Dcm_DefaultRoutineStart,Dcm_DefaultRoutineStop or Dcm_DefaultRoutineRequestRoutineResults)
 *         are called from application.
 */
STATIC VAR(boolean, DCM_VAR) Dcm_RoutineControl_UnsupportedRoutine;

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

#endif /*#if (DCM_ROUTINE_DEFAULT_INTERFACES == STD_ON)*/

/* If at least one routine is configured */
#if((DCM_NUM_ROUTINES > 0U) && (DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2006))

#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/** \brief Array holding the status of each routine. If the NRC policy ISOVersion 2006 is selected,
 *         this array is used to determine if a requested routine is started.
 *         The index of Dcm_DspRoutineStatus array corresponds to the index from Dcm_DspRoutineConfig
 *         array that contains the routine configuration.
 */
STATIC VAR(uint8, DCM_CONST) Dcm_RoutineControl_RoutineStatus[DCM_NUM_ROUTINES];

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

#endif /*#if((DCM_NUM_ROUTINES > 0U) &&
             (DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2006)) */

#define DCM_START_SEC_VAR_CLEARED_16
#include <Dcm_MemMap.h>

/** \brief This global variable holds the configuration index of the current processing routine.
 */
STATIC VAR(uint16, DCM_VAR) Dcm_RoutineControl_CurrentRoutineConfigIndex;

#define DCM_STOP_SEC_VAR_CLEARED_16
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief This global variable holds the sub-service handler, extracted from
 **        RoutineControl(0x31) request in the DCM_INITIAL state.
 **        This variable is used in each call of the RoutineControl handler to call the
 **        sub-service handler.
 */
STATIC VAR(Dcm_GenericServiceHandlerType, DCM_VAR) Dcm_RoutineControl_SubServiceHandler;

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* Initializes the control structures of all Routine elements used for the
   RoutineControl (0x31) service handler */
/* !LINKSTO Dcm.Dsn.IB.RoutineControl.Initialization,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_RoutineControl_Init(void)
{
#if ((DCM_NUM_ROUTINES > 0U) && (DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2006))

  /* Initialize array holding the status information of each routine */
  TS_MemSet(Dcm_RoutineControl_RoutineStatus,DCM_ROUTINE_INACTIVE,DCM_NUM_ROUTINES);

#endif /* #if ((DCM_NUM_ROUTINES > 0U) &&
               (DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2006)) */

  /* Set sub-service handler to NULL_PTR */
  Dcm_RoutineControl_SubServiceHandler = NULL_PTR;
  /* Set the current processing routine index to an invalid value */
  Dcm_RoutineControl_CurrentRoutineConfigIndex = DCM_NUM_ROUTINES;

#if (DCM_ROUTINE_DEFAULT_INTERFACES == STD_ON)
  /* Init the flag as FALSE, which means no unsupported routine is found. */
  Dcm_RoutineControl_UnsupportedRoutine = FALSE;
#endif /* #if (DCM_ROUTINE_DEFAULT_INTERFACES == STD_ON) */

}

/* !LINKSTO Dcm.Dsn.IB.RoutineControl.SvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RoutineControl_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_ROUTINECONTROL_SVCH_ENTRY(OpStatus,pMsgContext);

  switch (OpStatus)
  {
    case DCM_INITIAL:
      ServiceProcessStatus = Dcm_Dsp_RoutineControl_SvcStart(OpStatus, pMsgContext);
      break;
    case DCM_PENDING:
    case DCM_FORCE_RCRRP_OK:
    case DCM_CANCEL:
      ServiceProcessStatus = Dcm_Dsp_RoutineControl_SvcCont(OpStatus, pMsgContext);
      break;

    /* Nothing to do for DCM_CONFIRMED_OK and DCM_CONFIRMED_NOK */
    case DCM_CONFIRMED_OK:
    case DCM_CONFIRMED_NOK:
      ServiceProcessStatus = DCM_E_DONE;
      break;

    /* CHECK: NOPARSE */
    default:
    {
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_ROUTINECONTROL);
      ServiceProcessStatus = DCM_E_DONE;
    }
    break;
    /* CHECK: PARSE */
  }

  DBG_DCM_DSP_ROUTINECONTROL_SVCH_EXIT(ServiceProcessStatus,OpStatus,pMsgContext);
  return ServiceProcessStatus;
}

/* !LINKSTO Dcm.EB.RoutineControl.SubFunction.StartRoutine,1 */
/* !LINKSTO Dcm.Dsn.IB.RoutineControl.StartRoutine.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RoutineControl_StartRoutine_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
#if (DCM_NUM_ROUTINES > 0)
  Dcm_NegativeResponseCodeType ErrorCode = DCM_E_GENERALREJECT;
  Std_ReturnType RoutineSubfunctionResult = E_NOT_OK;
#endif /* #if (DCM_NUM_ROUTINES > 0) */
#if (DCM_ROUTINE_DEFAULT_INTERFACES == STD_ON)
  /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnconfiguredRoutine.Start.InitialCall,1 */
  /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnusedRoutine.Start.InitialCall,1 */
  /* Verify if an unsupported routine is requested */
  if(TRUE == Dcm_RoutineControl_UnsupportedRoutine)
  {
    /* Call the UnsupportedRoutineHandler with Start sub-function */
    ServiceProcessStatus = Dcm_Dsp_RoutineControl_UnsupportedRoutineHandler(DCM_ROUTINE_CONTROL_START,
                                                                            OpStatus,
                                                                            pMsgContext);
  }
  else
#endif /* #if (DCM_ROUTINE_DEFAULT_INTERFACES == STD_ON) */
  {
#if (DCM_NUM_ROUTINES > 0)
    /* Call function handler for request to application to start a routine */
    RoutineSubfunctionResult = Dcm_DspRoutineConfig[Dcm_RoutineControl_CurrentRoutineConfigIndex].StartFncHandler(
        OpStatus, pMsgContext, &ErrorCode);

    if(OpStatus != DCM_CANCEL)
    {
      switch (RoutineSubfunctionResult)
      {
        case DCM_E_NOT_OK:
        {
          if (ErrorCode != 0x00U)
          {
            /* !LINKSTO Dcm.EB.RoutineControl.XxxStart.Callout.ReturnNOK.DiffNRC0x00,1 */
            /* !LINKSTO Dcm.EB.RoutineControl.XxxStart.CS.ReturnNOK.DiffNRC0x00,1 */
            Dcm_ExternalSetNegResponse(pMsgContext, ErrorCode);
          }
          else
          {
            /* !LINKSTO Dcm.EB.RoutineControl.XxxStart.Callout.ReturnNOK.NRC0x00,1 */
            /* !LINKSTO Dcm.EB.RoutineControl.XxxStart.CS.ReturnNOK.NRC0x00,1 */
            Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
          }
          ServiceProcessStatus = DCM_E_DONE;
        }
        break;

        case DCM_E_OK:
        {
#if(DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2006)
          Dcm_RoutineControl_RoutineStatus[Dcm_RoutineControl_CurrentRoutineConfigIndex] = DCM_ROUTINE_STARTED;
#endif /* #if(DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2006) */

          /* If the routine was successful, write the sub-function and the routine Id in the response */
          pMsgContext->resData[0U] = DCM_ROUTINE_CONTROL_START;
          pMsgContext->resData[1U] = pMsgContext->reqData[0U];
          pMsgContext->resData[2U] = pMsgContext->reqData[1U];
#if (DCM_ROUTINE_ROUTINEINFOBYTE == STD_ON)
        /* !LINKSTO Dcm.EB.RoutineControl.RoutineInfoByte.Enabled.StartRoutine.AddRoutineInfoByte.Call,1 */
        Dcm_Dsp_RoutineControl_RoutineInfoByteHandler(pMsgContext);
#endif /* #if (DCM_ROUTINE_ROUTINEINFOBYTE == STD_ON) */
          /* !LINKSTO Dcm.EB.RoutineControl.XxxStart.Callout.ReturnOK.DiffNRC0x00,1 */
          /* !LINKSTO Dcm.EB.RoutineControl.XxxStart.CS.ReturnOK.DiffNRC0x00,1 */
          ServiceProcessStatus = DCM_E_DONE;
        }
        break;

#if (DCM_ROUTINECONTROL_USE_RTE == STD_ON)
        case DCM_E_NO_DATA:
        {
          /* !LINKSTO Dcm.EB.RoutineControl.XxxStart.CS.ReturnNoData,1 */
          ServiceProcessStatus = DCM_E_PENDING;
        }
        break;
#endif /*(DCM_ROUTINECONTROL_USE_RTE == STD_ON)*/

        case DCM_E_PENDING:
        case DCM_E_FORCE_RCRRP:
        {
          /* For DCM_E_PENDING and DCM_E_FORCE_RCRRP return the respective value */
          /* !LINKSTO Dcm.EB.RoutineControl.XxxStart.Callout.ReturnForceRCRRP,1 */
          /* !LINKSTO Dcm.EB.RoutineControl.XxxStart.CS.ReturnForceRCRRP,1 */
          ServiceProcessStatus = RoutineSubfunctionResult;
        }
        break;

        default:
        {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
          /* !LINKSTO Dcm.EB.RoutineControl.XxxStart.Callout.Unknown.ReturnValue.Det,1 */
          /* !LINKSTO Dcm.EB.RoutineControl.XxxStart.CS.Unknown.ReturnValue.Det,1 */
          DCM_DET_REPORT_ERROR(DCM_SVCID_ROUTINECONTROLSTART, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */

          /* !LINKSTO Dcm.EB.RoutineControl.XxxStart.Callout.Unknown.ReturnValue,2 */
          /* !LINKSTO Dcm.EB.RoutineControl.XxxStart.CS.Unknown.ReturnValue,2 */
          Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
          ServiceProcessStatus = DCM_E_DONE;
        }
        break;
      }
    }
    else
    {
      ServiceProcessStatus = DCM_E_DONE;
    }
#endif /*#if (DCM_NUM_ROUTINES > 0)*/
  }
  return ServiceProcessStatus;
}

/* !LINKSTO Dcm.EB.RoutineControl.SubFunction.StopRoutine,1 */
/* !LINKSTO Dcm.Dsn.IB.RoutineControl.StopRoutine.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RoutineControl_StopRoutine_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
#if (DCM_NUM_ROUTINES > 0)
  Dcm_NegativeResponseCodeType ErrorCode = DCM_E_GENERALREJECT;
#endif /*#if (DCM_NUM_ROUTINES > 0)*/
#if (DCM_ROUTINE_DEFAULT_INTERFACES == STD_ON)
  /* Verify if an unsupported routine is requested */
  if(TRUE == Dcm_RoutineControl_UnsupportedRoutine)
  {
    /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnconfiguredRoutine.Stop.InitialCall,1 */
    /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnusedRoutine.Stop.InitialCall,1 */
    /* Call the UnsupportedRoutineHandler with Stop sub-function */
    ServiceProcessStatus = Dcm_Dsp_RoutineControl_UnsupportedRoutineHandler(DCM_ROUTINE_CONTROL_STOP,
                                                                            OpStatus,
                                                                            pMsgContext);
  }
  else
#endif /* #if (DCM_ROUTINE_DEFAULT_INTERFACES == STD_ON) */
  {
#if (DCM_NUM_ROUTINES > 0)
    /* Get pointer to routine configuration table  */
    P2CONST(Dcm_DspRoutineServicesType, AUTOMATIC, DCM_CONST) CurrentRoutineConfig
      = &Dcm_DspRoutineConfig[Dcm_RoutineControl_CurrentRoutineConfigIndex];

    if (OpStatus == DCM_INITIAL)
    {
      if (CurrentRoutineConfig->StopFncHandler == NULL_PTR)
      {
        /* !LINKSTO Dcm.EB.RoutineControl.DcmDspStopRoutineSupported,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_SUBFUNCTIONNOTSUPPORTED);
        ServiceProcessStatus = DCM_E_DONE;
      }
      /* !LINKSTO Dcm.EB.RoutineControl.StopRoutine.ISO2013,1 */
#if(DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2006)
      else
      {
        if (Dcm_RoutineControl_RoutineStatus[Dcm_RoutineControl_CurrentRoutineConfigIndex] != DCM_ROUTINE_STARTED)
        {
          /* !LINKSTO Dcm.EB.RoutineControl.StopRoutine.ISO2006.NRC0x24,1 */
          Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTSEQUENCEERROR);
          ServiceProcessStatus = DCM_E_DONE;
        }
      }
#endif /* #if(DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2006) */
    }

    /* If the processing was not previously terminated by an error */
    if (ServiceProcessStatus != DCM_E_DONE)
    {
      Std_ReturnType RoutineSubfunctionResult = E_NOT_OK;
      /* Call function handler for request to application to stop a routine */
      RoutineSubfunctionResult =
        CurrentRoutineConfig->StopFncHandler(OpStatus, pMsgContext, &ErrorCode);

      if(OpStatus != DCM_CANCEL)
      {
        switch (RoutineSubfunctionResult)
        {
          case DCM_E_NOT_OK:
          {
            if (ErrorCode != 0x00U)
            {
              /* !LINKSTO Dcm.EB.RoutineControl.XxxStop.Callout.ReturnNOK.DiffNRC0x00,1 */
              /* !LINKSTO Dcm.EB.RoutineControl.XxxStop.CS.ReturnNOK.DiffNRC0x00,1 */
              Dcm_ExternalSetNegResponse(pMsgContext, ErrorCode);
            }
            else
            {
              /* !LINKSTO Dcm.EB.RoutineControl.XxxStop.Callout.ReturnNOK.NRC0x00,1 */
              /* !LINKSTO Dcm.EB.RoutineControl.XxxStop.CS.ReturnNOK.NRC0x00,1 */
              Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
            }
            ServiceProcessStatus = DCM_E_DONE;
          }
          break;

          case DCM_E_OK:
          {
#if(DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2006)
            Dcm_RoutineControl_RoutineStatus[Dcm_RoutineControl_CurrentRoutineConfigIndex] = DCM_ROUTINE_STOPPED;
#endif /* #if(DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2006) */

            /* If the routine was successful, write the subfunction and the routine Id in the response */
            pMsgContext->resData[0U] = DCM_ROUTINE_CONTROL_STOP;
            pMsgContext->resData[1U] = pMsgContext->reqData[0U];
            pMsgContext->resData[2U] = pMsgContext->reqData[1U];
#if (DCM_ROUTINE_ROUTINEINFOBYTE == STD_ON)
          /* !LINKSTO Dcm.EB.RoutineControl.RoutineInfoByte.Enabled.StopRoutine.AddRoutineInfoByte.Call,1 */
          Dcm_Dsp_RoutineControl_RoutineInfoByteHandler(pMsgContext);
#endif /* #if (DCM_ROUTINE_ROUTINEINFOBYTE == STD_ON) */

            /* !LINKSTO Dcm.EB.RoutineControl.XxxStop.Callout.ReturnOK.DiffNRC0x00,1 */
            /* !LINKSTO Dcm.EB.RoutineControl.XxxStop.CS.ReturnOK.DiffNRC0x00,1 */
            ServiceProcessStatus = DCM_E_DONE;
          }
          break;

#if (DCM_ROUTINECONTROL_USE_RTE == STD_ON)
          case DCM_E_NO_DATA:
          {
            /* !LINKSTO Dcm.EB.RoutineControl.XxxStop.CS.ReturnNoData,1 */
            ServiceProcessStatus = DCM_E_PENDING;
          }
          break;
#endif /*(DCM_ROUTINECONTROL_USE_RTE == STD_ON)*/

          case DCM_E_PENDING:
          case DCM_E_FORCE_RCRRP:
          {
            /* For DCM_E_PENDING and DCM_E_FORCE_RCRRP return the respective value */
            /* !LINKSTO Dcm.EB.RoutineControl.XxxStop.Callout.ReturnForceRCRRP,1 */
            /* !LINKSTO Dcm.EB.RoutineControl.XxxStop.CS.ReturnForceRCRRP,1 */
            ServiceProcessStatus = RoutineSubfunctionResult;
          }
          break;

          default:
          {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
            /* !LINKSTO Dcm.EB.RoutineControl.XxxStop.Callout.Unknown.ReturnValue.Det,1 */
            /* !LINKSTO Dcm.EB.RoutineControl.XxxStop.CS.Unknown.ReturnValue.Det,1 */
            DCM_DET_REPORT_ERROR(DCM_SVCID_ROUTINECONTROLSTOP, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */

            /* !LINKSTO Dcm.EB.RoutineControl.XxxStop.Callout.Unknown.ReturnValue,2 */
            /* !LINKSTO Dcm.EB.RoutineControl.XxxStop.CS.Unknown.ReturnValue,2 */
            Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
            ServiceProcessStatus = DCM_E_DONE;
          }
          break;
        }
      }
      else
      {
        ServiceProcessStatus = DCM_E_DONE;
      }
    }
#endif /*#if (DCM_NUM_ROUTINES > 0)*/
  }
  return ServiceProcessStatus;
}

/* !LINKSTO Dcm.EB.RoutineControl.SubFunction.RequestsRoutineResults,1 */
/* !LINKSTO Dcm.Dsn.IB.RoutineControl.RequestResultsRoutine.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RoutineControl_RequestResults_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
#if (DCM_NUM_ROUTINES > 0)
  Dcm_NegativeResponseCodeType ErrorCode = DCM_E_GENERALREJECT;
#endif /*#if (DCM_NUM_ROUTINES > 0)*/
#if (DCM_ROUTINE_DEFAULT_INTERFACES == STD_ON)
  /* Verify if an unsupported routine is requested */
  if(TRUE == Dcm_RoutineControl_UnsupportedRoutine)
  {
    /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnconfiguredRoutine.RequestResults.InitialCall,1 */
    /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnusedRoutine.RequestResults.InitialCall,1 */
    /* Call the UnsupportedRoutineHandler with RequestResults sub-function */
    ServiceProcessStatus = Dcm_Dsp_RoutineControl_UnsupportedRoutineHandler(DCM_ROUTINE_CONTROL_REQUESTRESULTS,
                                                                            OpStatus,
                                                                            pMsgContext);
  }
  else
#endif /* #if (DCM_ROUTINE_DEFAULT_INTERFACES == STD_ON) */
  {
#if (DCM_NUM_ROUTINES > 0)
    /* get pointer to routine configuration table  */
    P2CONST(Dcm_DspRoutineServicesType, AUTOMATIC, DCM_CONST) CurrentRoutineConfig
      = &Dcm_DspRoutineConfig[Dcm_RoutineControl_CurrentRoutineConfigIndex];

    if (OpStatus == DCM_INITIAL)
    {
      if (CurrentRoutineConfig->RequestResultsFncHandler == NULL_PTR)
      {
        /* !LINKSTO Dcm.EB.RoutineControl.DcmDspRequestResultsRoutineSupported,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_SUBFUNCTIONNOTSUPPORTED);
        ServiceProcessStatus = DCM_E_DONE;
      }
      /* !LINKSTO Dcm.EB.RoutineControl.RequestRoutineResults.ISO2013,1 */
#if(DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2006)
      else
      {
        if (Dcm_RoutineControl_RoutineStatus[Dcm_RoutineControl_CurrentRoutineConfigIndex] == DCM_ROUTINE_INACTIVE)
        {
          /* !LINKSTO Dcm.EB.RoutineControl.RequestRoutineResults.ISO2006.NRC0x24,1 */
          Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTSEQUENCEERROR);
          ServiceProcessStatus = DCM_E_DONE;
        }
      }
#endif /* #if(DCM_NRC_POLICY_ISO_VERSION == DCM_ISO14229_2006) */
    }

    /* If the processing was not previously terminated by an error */
    if (ServiceProcessStatus != DCM_E_DONE)
    {
      Std_ReturnType RoutineSubfunctionResult = E_NOT_OK;
      /* Call function handler for request to application, the results of a routine */
      RoutineSubfunctionResult =
        CurrentRoutineConfig->RequestResultsFncHandler(OpStatus, pMsgContext,&ErrorCode);

      if(OpStatus != DCM_CANCEL)
      {
        switch (RoutineSubfunctionResult)
        {
          case DCM_E_NOT_OK:
          {
            if (ErrorCode != 0x00U)
            {
              /* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResults.Callout.ReturnNOK.DiffNRC0x00,1 */
              /* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResults.CS.ReturnNOK.DiffNRC0x00,1 */
              Dcm_ExternalSetNegResponse(pMsgContext, ErrorCode);
            }
            else
            {
              /* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResults.Callout.ReturnNOK.NRC0x00,1 */
              /* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResults.CS.ReturnNOK.NRC0x00,1 */
              Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
            }
            ServiceProcessStatus = DCM_E_DONE;
          }
          break;

          case DCM_E_OK:
          {
            /* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResults.Callout.ReturnOK.DiffNRC0x00,1 */
            /* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResults.CS.ReturnOK.DiffNRC0x00,1 */
            pMsgContext->resData[0U] = DCM_ROUTINE_CONTROL_REQUESTRESULTS;
            pMsgContext->resData[1U] = pMsgContext->reqData[0U];
            pMsgContext->resData[2U] = pMsgContext->reqData[1U];

#if (DCM_ROUTINE_ROUTINEINFOBYTE == STD_ON)
        /* !LINKSTO Dcm.EB.RoutineControl.RoutineInfoByte.Enabled.RequestRoutineResults.AddRoutineInfoByte.Call,1 */
        Dcm_Dsp_RoutineControl_RoutineInfoByteHandler(pMsgContext);
#endif /* #if (DCM_ROUTINE_ROUTINEINFOBYTE == STD_ON) */
            ServiceProcessStatus = DCM_E_DONE;
          }
          break;

#if (DCM_ROUTINECONTROL_USE_RTE == STD_ON)
          case DCM_E_NO_DATA:
          {
            /* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResults.CS.ReturnNoData,1 */
            ServiceProcessStatus = DCM_E_PENDING;
          }
          break;
#endif /*(DCM_ROUTINECONTROL_USE_RTE == STD_ON)*/

          case DCM_E_PENDING:
          case DCM_E_FORCE_RCRRP:
          {
            /* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResults.Callout.ReturnForceRCRRP,1 */
            /* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResults.CS.ReturnForceRCRRP,1 */
            /* For DCM_E_PENDING and DCM_E_FORCE_RCRRP return the respective value */
            ServiceProcessStatus = RoutineSubfunctionResult;
          }
          break;

          default:
          {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
            /* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResults.Callout.Unknown.ReturnValue.Det,1 */
            /* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResults.CS.Unknown.ReturnValue.Det,1 */
            DCM_DET_REPORT_ERROR(DCM_SVCID_ROUTINECONTROLROUTINERESULTS, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */

            /* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResults.Callout.Unknown.ReturnValue,2 */
            /* !LINKSTO Dcm.EB.RoutineControl.XxxRequestResults.CS.Unknown.ReturnValue,2 */
            Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
            ServiceProcessStatus = DCM_E_DONE;
          }
          break;
        }
      }
      else
      {
        ServiceProcessStatus = DCM_E_DONE;
      }
    }
#endif /*#if (DCM_NUM_ROUTINES > 0)*/
  }
  return ServiceProcessStatus;
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

#if (DCM_NUM_ROUTINES > 0)
/* !LINKSTO Dcm.Dsn.IB.RoutineControl.SetCurrentRoutine,1 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_RoutineControl_SetCurrentRoutine(
  P2CONST(uint8, AUTOMATIC, DCM_VAR) RequestData)
{
  boolean RoutineIdFound = FALSE;
  uint16_least RoutineTabIndex = 0U;

  /* Routine Identifier */
  const uint8 RoutineIdHighByte = RequestData[0U];
  const uint8 RoutineIdLowByte = RequestData[1U];
  const uint16 RoutineID = (uint16) DCM_COMPOSE_16BIT_WORD_FROM_MSB_AND_LSB(RoutineIdHighByte, RoutineIdLowByte);

  /* Set the current Routine index to an invalid value */
  Dcm_RoutineControl_CurrentRoutineConfigIndex = DCM_NUM_ROUTINES;

  /* search list of configured routines */
  for (RoutineTabIndex=0U; (RoutineTabIndex<DCM_NUM_ROUTINES)&&(FALSE==RoutineIdFound); RoutineTabIndex++)
  {
    if ((Dcm_DspRoutineConfig[RoutineTabIndex].Identifier) == RoutineID)
    {
      /* Store the index of the routine in the configuration */
      Dcm_RoutineControl_CurrentRoutineConfigIndex = (uint16)RoutineTabIndex;
      RoutineIdFound = TRUE;
    }
  }

  return RoutineIdFound;
}
#endif /*#if (DCM_NUM_ROUTINES > 0)*/

/* !LINKSTO Dcm.Dsn.IB.RoutineControl.SvcStart,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RoutineControl_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  /* Flag to indicate if the Routine Handler can be called to perform the routine request.
   * It is set as TRUE when:
   * (DCM_ROUTINE_DEFAULT_INTERFACES == STD_ON) and an unsupported (unconfigured or unused)
   * routine is requested
   * or
   * (DCM_ROUTINE_DEFAULT_INTERFACES == STD_OFF) and all verifications for supported
   * requested routine have been successful.
   * It is initialized with FALSE.*/
   boolean RoutineControlIsValid = FALSE;

  /* !LINKSTO Dcm.EB.RoutineControl.ISO2013.NRCsOrder,1 */
  /* !LINKSTO Dcm.EB.RoutineControl.ISO2006.NRCsOrder,1 */
  /* !LINKSTO Dcm.Dsn.IB.RoutineControl.NRCsOrder.ISO2006,1 */
  /* !LINKSTO Dcm.Dsn.IB.RoutineControl.NRCsOrder.ISO2013,1 */
  /* Check minimal request length */
  if (pMsgContext->reqDataLen < DCM_ROUTINE_CONTROL_MIN_REQ_LENGTH)
  {
    /* !LINKSTO Dcm.EB.RoutineControl.MinimalLength.NRC0x13,1 */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    ServiceProcessStatus = DCM_E_DONE;
  }
  else
  {
#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
    Dcm_NegativeResponseCodeType CurrentNrc = DCM_E_GENERALREJECT;
#endif /*#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)*/

#if (DCM_NUM_ROUTINES > 0)
    /* Flag to indicate if the routine index exists in the configuration
     * Give the address in reqData where the RoutineId begins */
    boolean RoutineIndexFound = Dcm_Dsp_RoutineControl_SetCurrentRoutine(&pMsgContext->reqData[1U]);
#else
    boolean RoutineIndexFound = FALSE;
#endif /*#if (DCM_NUM_ROUTINES > 0)*/

#if(DCM_DSP_USE_SERVICE_0X08 == STD_ON)

    boolean RoutineIsInOBDRange =
      Dcm_Dsp_RoutineControl_ProcessRoutineInOBDRange(pMsgContext);

    if (RoutineIsInOBDRange != FALSE)
    {
      ServiceProcessStatus = DCM_E_DONE;
    }
    else
#endif
    /* First make sure this RID is configured */
    if (RoutineIndexFound != TRUE)
    {
#if (DCM_ROUTINE_DEFAULT_INTERFACES == STD_ON)
      /* The following flag is set to TRUE when an unsupported
       * (not configured or not used) routine is found */
      Dcm_RoutineControl_UnsupportedRoutine = TRUE;
      /* The unsupported routine is allowed to be handled by setting
       * the flag to TRUE.*/
      RoutineControlIsValid = TRUE;
#else
      /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Disabled.UnconfiguredRoutine,1 */
      /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Disabled.UnusedRoutine,1 */
      /* Verification failed; set the RequestOutOfRange error code for unsupported RIDs */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
      ServiceProcessStatus = DCM_E_DONE;
#endif /* #if (DCM_ROUTINE_DEFAULT_INTERFACES == STD_ON) */
    }
#if (DCM_NUM_ROUTINES > 0)
    else if (Dcm_Dsp_RoutineControl_IsRoutineAvailableInCurrentSession() == FALSE)
    {
      /* !LINKSTO Dcm.EB.RoutineControl.StartRoutine.WrongSession,1 */
      /* !LINKSTO Dcm.EB.RoutineControl.StopRoutine.WrongSession,1 */
      /* !LINKSTO Dcm.EB.RoutineControl.RequestRoutineResults.WrongSession,1 */
      /* Verification failed; set the respective error code */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
      ServiceProcessStatus = DCM_E_DONE;
    }
    else if (Dcm_Dsp_RoutineControl_IsRoutineAvailableInCurrentSecurityLevel() == FALSE)
    {
      /* !LINKSTO Dcm.EB.RoutineControl.StartRoutine.WrongSecurityLevel,1 */
      /* !LINKSTO Dcm.EB.RoutineControl.StopRoutine.WrongSecurityLevel,1 */
      /* !LINKSTO Dcm.EB.RoutineControl.RequestRoutineResults.WrongSecurityLevel,1 */
      /* Verification failed; set the respective error code */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_SECURITYACCESSDENIED);
      ServiceProcessStatus = DCM_E_DONE;
    }
#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
    else if (Dcm_Dsp_RoutineControl_ModeRuleResult(&CurrentNrc) == FALSE)
    {
      /* !LINKSTO Dcm.EB.RoutineControl.StartRoutine.ModeCondition,1 */
      /* !LINKSTO Dcm.EB.RoutineControl.StopRoutine.ModeCondition,1 */
      /* !LINKSTO Dcm.EB.RoutineControl.RequestRoutineResults.ModeCondition,1 */
      Dcm_ExternalSetNegResponse(pMsgContext, CurrentNrc);
      ServiceProcessStatus = DCM_E_DONE;
    }
#endif
    else /* If all verifications have been successful, call the Routine handler */
    {
     /* RoutineControlIsValid could be set as TRUE if:
      * (DCM_ROUTINE_DEFAULT_INTERFACES == STD_ON) and an unsupported (unconfigured or unused)
      * routine was requested
      * or
      * (DCM_ROUTINE_DEFAULT_INTERFACES == STD_OFF) and all verifications for a supported routine
      * have been successful. */
       RoutineControlIsValid = TRUE;
    }
#endif /*#if (DCM_NUM_ROUTINES > 0)*/
    /* Verify if the routine request can be processed */
    if(TRUE == RoutineControlIsValid)
    {
      /* Extract the sub-service identifier from the current request */
       const uint8 SubServiceIdentifier = pMsgContext->reqData[0U];

      /* Verify if the sub-service identifier is valid*/
      if ((SubServiceIdentifier != DCM_ROUTINE_CONTROL_START) &&
          (SubServiceIdentifier != DCM_ROUTINE_CONTROL_STOP) &&
          (SubServiceIdentifier != DCM_ROUTINE_CONTROL_REQUESTRESULTS))
      {
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_SUBFUNCTIONNOTSUPPORTED);
        ServiceProcessStatus = DCM_E_DONE;
      }
      else
      {
        Dcm_RoutineControl_SubServiceHandler =
                      Dcm_Dsp_ExtractSubServiceHandler(OpStatus, pMsgContext);

        ServiceProcessStatus = Dcm_Dsp_RoutineControl_SvcCont(OpStatus, pMsgContext);
      }
    }
  }

  return ServiceProcessStatus;
}

/* !LINKSTO Dcm.Dsn.IB.RoutineControl.SvcCont,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RoutineControl_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DCM_PRECONDITION_ASSERT(
    Dcm_RoutineControl_SubServiceHandler != NULL_PTR, DCM_INTERNAL_API_ID);

  /* Call the respective sub-service handler */
  ServiceProcessStatus = Dcm_RoutineControl_SubServiceHandler(OpStatus, pMsgContext);

  return ServiceProcessStatus;
}

#if (DCM_NUM_ROUTINES > 0)
/* !LINKSTO Dcm.Dsn.IB.RoutineControl.IsRoutineAvailableInCurrentSession,1 */
STATIC FUNC (boolean, DCM_CODE) Dcm_Dsp_RoutineControl_IsRoutineAvailableInCurrentSession(void)
{
  boolean RoutineIsAvailableInCurrentSession = FALSE;

  /* Get pointer to routine configuration info table  */
  P2CONST(Dcm_DspRoutineServicesInfoType, AUTOMATIC, DCM_CONST) CurrentRoutineConfigInfo
    = Dcm_DspRoutineConfig[Dcm_RoutineControl_CurrentRoutineConfigIndex].RoutineInfoRef;

  if (CurrentRoutineConfigInfo->NumSesLevels > 0U)
  {
    Dcm_SesCtrlType CurrentSesionCtrl;
    uint8_least SessionIdx;

    /* No need to check the return value - at this point Dcm_GetSesCtrlType() will return E_OK
     * Dcm is initialized and &CurrentSesionCtrl is not NULL */
    (void)Dcm_GetSesCtrlTypeExt(&CurrentSesionCtrl);

    /* check if routine is allowed in current session */
    for (SessionIdx = 0U;
        (SessionIdx < CurrentRoutineConfigInfo->NumSesLevels) && (FALSE == RoutineIsAvailableInCurrentSession);
        SessionIdx++)
    {
      if ((CurrentRoutineConfigInfo->SessionRef[SessionIdx] == DCM_ALL_SESSION_LEVEL) ||
          (CurrentRoutineConfigInfo->SessionRef[SessionIdx] == CurrentSesionCtrl))
      {
        RoutineIsAvailableInCurrentSession = TRUE;
      }
    }
  }
  else
  {
    RoutineIsAvailableInCurrentSession = TRUE;
  }

  return RoutineIsAvailableInCurrentSession;
}

/* !LINKSTO Dcm.Dsn.IB.RoutineControl.IsRoutineAvailableInSecLevel,1 */
STATIC FUNC (boolean, DCM_CODE) Dcm_Dsp_RoutineControl_IsRoutineAvailableInCurrentSecurityLevel(void)
{
  boolean RoutineIsAvailableInCurrentSecLevel = FALSE;

  /* Get pointer to routine configuration info table  */
  P2CONST(Dcm_DspRoutineServicesInfoType, AUTOMATIC, DCM_CONST) CurrentRoutineConfigInfo
    = Dcm_DspRoutineConfig[Dcm_RoutineControl_CurrentRoutineConfigIndex].RoutineInfoRef;

  if (CurrentRoutineConfigInfo->NumSecLevels != 0U)
  {
    /* Get current security level */
    Dcm_SecLevelType CurrentSecLevel = Dcm_Dsl_SecurityLevelHandler_GetSecurityLevel();

    /* If DCM is not in locked state */
    if (CurrentSecLevel != DCM_SEC_LEV_LOCKED)
    {
      uint8_least SecurityLevelIndex;

      /* Check if Routine is allowed with current security level */
      for (SecurityLevelIndex = 0U;
          (SecurityLevelIndex < CurrentRoutineConfigInfo->NumSecLevels) && (FALSE == RoutineIsAvailableInCurrentSecLevel);
          SecurityLevelIndex++)
      {
        if (CurrentRoutineConfigInfo->SecurityLevelRef[SecurityLevelIndex] == CurrentSecLevel)
        {
          RoutineIsAvailableInCurrentSecLevel = TRUE;
        }
      }
    }
  }
  else
  {
    RoutineIsAvailableInCurrentSecLevel = TRUE;
  }

  return RoutineIsAvailableInCurrentSecLevel;
}
#endif /*#if (DCM_NUM_ROUTINES > 0)*/

#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.RoutineControl.ModeRuleResult,1 */
STATIC FUNC (boolean, DCM_CODE) Dcm_Dsp_RoutineControl_ModeRuleResult(
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) CurrentNrc)
{
  /* Variable for mode rule result - false means that the mode rule check failed */
  boolean ModeRuleCheckResult = FALSE;

  /* Get pointer to routine configuration info table  */
  P2CONST(Dcm_DspRoutineServicesType, AUTOMATIC, DCM_CONST) CurrentRoutineConfig
    = &Dcm_DspRoutineConfig[Dcm_RoutineControl_CurrentRoutineConfigIndex];

  if (CurrentRoutineConfig->ModeRuleRoutineFunction != NULL_PTR)
  {
    /* Invoke the mode rule function for the service
     * If the mode rule resulted in failure CurrentNrc will have the calculated nrc,
     * else zero which implies there is no failure */
    ModeRuleCheckResult = CurrentRoutineConfig->ModeRuleRoutineFunction(CurrentNrc);
  }
  else
  {
    ModeRuleCheckResult = TRUE;
  }

  return ModeRuleCheckResult;
}
#endif /*DCM_MODEDECLARATION_SUPPORT */

#if(DCM_DSP_USE_SERVICE_0X08 == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.RoutineControl.IsRoutineInOBDRange,1 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_RoutineControl_ProcessRoutineInOBDRange(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{

  boolean RoutineIsInOBDRange = FALSE;

  /* Routine Identifier */
  const uint8 RoutineIdHighByte = (uint8)pMsgContext->reqData[1U];
  const uint8 RoutineIdLowByte = (uint8)pMsgContext->reqData[2U];

  /* For every requested RID inside the OBD range (E000-E0FF), the DCM module
   * shall get the RID value as defined for OBD Service $08 */
  if (RoutineIdHighByte == DCM_DSP_OBD_RID_RANGE_HIGH_BYTE)
  {
    /* !LINKSTO Dcm701,1, SWS_Dcm_00701,1 */
    /* Only non-availability OBD TIDs are checked for presence in the routines config */
    if ((DCM_IS_AVAILABILITY_OBDSUBID(RoutineIdLowByte) == FALSE)
        && (Dcm_RoutineControl_CurrentRoutineConfigIndex == DCM_NUM_ROUTINES))
    {
      /* Verification failed; set the RequestOutOfRange error code for unknown RIDs */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
    }
    else
    {
      /* Call the handler for this service through a shell which takes care
       * of the fact that the request and response messages are of UDS type */
      Dcm_DspInternal_ReqCtrlViaRoutineCtrl_UDSShell(pMsgContext);
    }

    RoutineIsInOBDRange = TRUE;
  }

  return RoutineIsInOBDRange;
}
#endif /*#if(DCM_DSP_USE_SERVICE_0X08 == STD_ON)*/

#if (DCM_ROUTINE_DEFAULT_INTERFACES == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.RoutineControl.UnsupportedRoutineHandler,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RoutineControl_UnsupportedRoutineHandler(
  uint8 SubServiceID,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  Std_ReturnType RoutineSubfunctionResult = E_NOT_OK;
  Dcm_NegativeResponseCodeType ErrorCode = DCM_E_GENERALREJECT;

  /* 2-byte Unsupported Routine Identifier */
  const uint8 RoutineIdHighByte = pMsgContext->reqData[0U];
  const uint8 RoutineIdLowByte = pMsgContext->reqData[1U];
  const Dcm_RoutineIDType RoutineID = (Dcm_RoutineIDType) DCM_COMPOSE_16BIT_WORD_FROM_MSB_AND_LSB(RoutineIdHighByte, RoutineIdLowByte);

  /* Unsupported Routine Control Option Record length*/
  const uint32 InDataLength = pMsgContext->reqDataLen - 2U;

  /* Call the Application default functions for the requested SubService */
  switch(SubServiceID)
  {
    case DCM_ROUTINE_CONTROL_START:
    {
      /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.Dcm_DefaultRoutineStart,1 */
      /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.Start.Cancellation.Timeout,1 */
      /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.Start.Cancellation.ProtocolPreemption,1 */
      /* Call the API default function for Start SubService with different OpStatus.
       * If the request has been cancelled, the API function will be called with DCM_CANCEL OpStatus. */
      RoutineSubfunctionResult = Dcm_DefaultRoutineStart(OpStatus,
                                                         RoutineID,
                                                         (uint8*)&pMsgContext->reqData[2],
                                                         InDataLength,
                                                         (uint8*)&pMsgContext->resData[3],
                                                         (uint32*)&pMsgContext->resDataLen,
                                                         &ErrorCode);
    }
    break;

    case DCM_ROUTINE_CONTROL_STOP:
    {
      /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.Dcm_DefaultRoutineStop,1 */
      /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.Stop.Cancellation.Timeout,1 */
      /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.Stop.Cancellation.ProtocolPreemption,1 */
      /* Call the API default function for Stop SubService with different OpStatus.
       * If the request has been cancelled, the API function will be called with DCM_CANCEL OpStatus. */
      RoutineSubfunctionResult = Dcm_DefaultRoutineStop(OpStatus,
                                                        RoutineID,
                                                        (uint8*)&pMsgContext->reqData[2],
                                                        InDataLength,
                                                        (uint8*)&pMsgContext->resData[3],
                                                        (uint32*)&pMsgContext->resDataLen,
                                                        &ErrorCode);
    }
    break;

    case DCM_ROUTINE_CONTROL_REQUESTRESULTS:
    {
      /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.Dcm_DefaultRoutineRequestRoutineResults,1 */
      /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.RequestResults.Cancellation.Timeout,1 */
      /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.RequestResults.Cancellation.ProtocolPreemption,1 */
      /* Call the API default function for RequestResults SubService with different OpStatus.
       * If the request has been cancelled, the API function will be called with DCM_CANCEL OpStatus. */
      RoutineSubfunctionResult = Dcm_DefaultRoutineRequestRoutineResults(OpStatus,
                                                                         RoutineID,
                                                                         (uint8*)&pMsgContext->reqData[2],
                                                                         InDataLength,
                                                                         (uint8*)&pMsgContext->resData[3],
                                                                         (uint32*)&pMsgContext->resDataLen,
                                                                         &ErrorCode);
    }
    break;

    /* CHECK: NOPARSE */
    default:
    {
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_ROUTINECONTROL);
    }
    break;
   /* CHECK: PARSE */
  }

  if(OpStatus != DCM_CANCEL)
  {
    /* Verify the API default function result */
    switch (RoutineSubfunctionResult)
    {
      /* Request was not successful */
      case E_NOT_OK:
      {
        if (ErrorCode != DCM_E_POSITIVERESPONSE)
        {
          /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.Start.NRC,1 */
          /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.Stop.NRC,1 */
          /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.RequestResults.NRC,1 */
          /* The value of the Nrc parameter is set */
          Dcm_ExternalSetNegResponse(pMsgContext, ErrorCode);
        }
        else
        {
          /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.Start.0x00,1 */
          /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.Stop.0x00,1 */
          /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.RequestResults.0x00,1 */
          /* ConditionsNotCorrect(0x22) NRC is set */
          Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
        }
        /* After the unsupported routine is handled, the flag is set as FALSE */
        Dcm_RoutineControl_UnsupportedRoutine = FALSE;
        ServiceProcessStatus = DCM_E_DONE;
      }
      break;

      /* Request was successful */
      case E_OK:
      {
        /* If the routine was successful, write the sub-function and the routine Id in the response */
        pMsgContext->resData[0U] = SubServiceID;
        pMsgContext->resData[1U] = pMsgContext->reqData[0U];
        pMsgContext->resData[2U] = pMsgContext->reqData[1U];
        pMsgContext->resDataLen += 3U;

        /* After the unsupported routine is handled, the flag is set as FALSE */
        Dcm_RoutineControl_UnsupportedRoutine = FALSE;

        /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.Start.OK,1 */
        /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.Stop.OK,1 */
        /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.RequestResults.OK,1 */
        ServiceProcessStatus = DCM_E_DONE;
      }
      break;

      /* Request is not yet finished, further call(s) required to finish */
      case DCM_E_PENDING:
      {
        /* For DCM_E_PENDING return the respective value */
        /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.Start.Pending,1 */
        /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.Stop.Pending,1 */
        /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.RequestResults.Pending,1 */
        /* Retry the call in next main function cycle */
        ServiceProcessStatus = DCM_E_PENDING;
      }
      break;

      /* The result of the API function is unknown */
      default:
      {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        /* If the Error code is unknown, report the DCM_E_INTERFACE_RETURN_VALUE DET error */
        if(SubServiceID == DCM_ROUTINE_CONTROL_START)
        {
            /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.Start.UnsupportedError.DET,1 */
            DCM_DET_REPORT_ERROR(DCM_SVCID_ROUTINECONTROLSTART, DCM_E_INTERFACE_RETURN_VALUE);
        }
        else if(SubServiceID == DCM_ROUTINE_CONTROL_STOP)
        {
          /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.Stop.UnsupportedError.DET,1 */
          DCM_DET_REPORT_ERROR(DCM_SVCID_ROUTINECONTROLSTOP, DCM_E_INTERFACE_RETURN_VALUE);
        }
        else
        {
          /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.RequestResults.UnsupportedError.DET,1 */
          DCM_DET_REPORT_ERROR(DCM_SVCID_ROUTINECONTROLROUTINERESULTS, DCM_E_INTERFACE_RETURN_VALUE);
        }
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */

        /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.Start.UnsupportedError.NRC,1 */
        /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.Stop.UnsupportedError.NRC,1 */
        /* !LINKSTO Dcm.EB.RoutineControl.DefaultRoutines.Enabled.UnsupportedRoutine.RequestResults.UnsupportedError.NRC,1 */
        /* GeneralReject (0x10) is set for an unknown Error code */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
        ServiceProcessStatus = DCM_E_DONE;
      }
      break;
    }
  }
  /* OpStatus value is DCM_CANCEL */
  else
  {
    /* The result of this call is ignored */
    ServiceProcessStatus = DCM_E_DONE;
  }

  return ServiceProcessStatus;
}
#endif /*#if (DCM_ROUTINE_DEFAULT_INTERFACES == STD_ON)*/

#if (DCM_ROUTINE_ROUTINEINFOBYTE == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.RoutineControl.RoutineInfoByteHandler,1 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_RoutineControl_RoutineInfoByteHandler(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  /* 2-byte Routine Identifier */
  const uint8 RoutineIdHighByte = pMsgContext->reqData[0U];
  const uint8 RoutineIdLowByte = pMsgContext->reqData[1U];
  const Dcm_RoutineIDType RoutineID = (Dcm_RoutineIDType) DCM_COMPOSE_16BIT_WORD_FROM_MSB_AND_LSB(RoutineIdHighByte, RoutineIdLowByte);

  /* Variable to check if a byte was requested to be added from the application */
  boolean RoutineInfoByteStatus = FALSE;

  /* Variable to store the byte to be added in the response message */
  uint8 Dcm_RoutineInfoByte = 0U;

  /* !LINKSTO Dcm.EB.RoutineControl.RoutineInfoByte.Enabled.Dcm_AddRoutineInfoByte,1 */
  /* !LINKSTO Dcm.EB.RoutineControl.RoutineInfoByte.Enabled.StartRoutine.AddRoutineInfoByte.Call,1 */
  /* !LINKSTO Dcm.EB.RoutineControl.RoutineInfoByte.Enabled.StopRoutine.AddRoutineInfoByte.Call,1 */
  /* !LINKSTO Dcm.EB.RoutineControl.RoutineInfoByte.Enabled.RequestRoutineResults.AddRoutineInfoByte.Call,1 */
  /* Check if a byte was requested to be added from the application */
  RoutineInfoByteStatus = Dcm_AddRoutineInfoByte(RoutineID, &Dcm_RoutineInfoByte);

  if(pMsgContext->resMaxDataLen > pMsgContext->resDataLen)
  {
    if(TRUE == RoutineInfoByteStatus)
    {
      /* !LINKSTO Dcm.EB.RoutineControl.RoutineInfoByte.Enabled.StartRoutine.AddRoutineInfoByte.TRUE,1 */
      /* !LINKSTO Dcm.EB.RoutineControl.RoutineInfoByte.Enabled.StopRoutine.AddRoutineInfoByte.TRUE,1 */
      /* !LINKSTO Dcm.EB.RoutineControl.RoutineInfoByte.Enabled.RequestRoutineResults.AddRoutineInfoByte.TRUE,1 */
      /* Shift the response message by 1 byte after the Routine ID */
      Dcm_DspInternal_ShiftBitsInBuffer(pMsgContext->resData,
                                        DCM_GET_BITS(DCM_ROUTINE_CONTROL_RESPONS_HEADER_SIZE),
                                        DCM_GET_BITS(DCM_ROUTINE_CONTROL_RESPONS_HEADER_SIZE) + DCM_ROUTINE_CONTROL_INFOBYTE_SIZE,
                                        DCM_GET_BITS((pMsgContext->resDataLen - DCM_ROUTINE_CONTROL_RESPONS_HEADER_SIZE)),
                                        FALSE);

      /* Write the byte from the application to the first byte after RoutineID and
       * increase the message length by 1 */
      pMsgContext->resData[DCM_ROUTINE_CONTROL_RESPONS_HEADER_SIZE] = Dcm_RoutineInfoByte;
      pMsgContext->resDataLen += 1U;
    }
  }
/* CHECK: NOPARSE */
  else
  {
    /* Defensive branch.
     * This branch should not be reached because we ensure from configuration with a XPath warning
     * that the transmission buffer is sufficient to hold the response data and the InfoByte
     * of the routine with maximum output signal length. Even if the warning will be ignore the memory
     * will not be corrupted.
     */
    DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_ROUTINECONTROLADDINFOBYTE);
  }
/* CHECK: PARSE */
}
#endif /* #if (DCM_ROUTINE_ROUTINEINFOBYTE == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if(DCM_DSP_USE_SERVICE_0X31 == STD_ON) */

/*==================[end of file]================================================================*/
