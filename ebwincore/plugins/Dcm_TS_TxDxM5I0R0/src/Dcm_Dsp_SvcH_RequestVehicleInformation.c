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

/* !LINKSTO Dcm.dsn.File.ReqVehInfo.Impl,1 */
/* This file contains the implementation of the implementation of the Diagnostic
 * Service ReadDataByIdentifier. */

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 15.2 (required)
 *     "An unconditional break statement shall terminate every non-empty switch clause."
 *
 *     Reason:
 *     The following actions have to be executed in the same state so the break
 *     statement is removed in order to have shorter code and avoid duplicated code.
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
 *
 */
/*===============================[includes]======================================================*/

#include <Dcm_Trace.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <Dcm_Int.h>
#include <TSMem.h>                                               /* EB specific memory functions */
#include <Dcm_Dsp_SvcH_RequestVehicleInformation.h>
#include <Dcm_Dsp_VinServices.h>
/*===========================[macro definitions]=================================================*/

#if (defined DCM_OBD_VEHINFO_AVAILDATALEN)
#error DCM_OBD_VEHINFO_AVAILDATALEN already defined
#endif
/** \brief Length of a single supported Vehicle InfoType response */
#define DCM_OBD_VEHINFO_AVAILDATALEN 4U /* DATA_A, ... DATA_D */

#if (defined DCM_OBD_VEHINFO_MAX_BUFFER_SIZE)
#error DCM_OBD_VEHINFO_MAX_BUFFER_SIZE already defined
#endif
/** \brief Max buffer size to be passed to callback function*/
#define DCM_OBD_VEHINFO_MAX_BUFFER_SIZE 255U

#if (defined DCM_OBD_VEHINFO_MIN_SIZE_WITH_NODI_PROVRESP_TRUE)
#error DCM_OBD_VEHINFO_MIN_SIZE_WITH_NODI_PROVRESP_TRUE already defined
#endif
/** \brief Minimum response length when DcmDspVehInfoNODIProvResp is set to true */
#define DCM_OBD_VEHINFO_MIN_SIZE_WITH_NODI_PROVRESP_TRUE 2U

#if (defined DCM_OBD_VEHINFO_MIN_SIZE_WITH_NODI_PROVRESP_FALSE)
#error DCM_OBD_VEHINFO_MIN_SIZE_WITH_NODI_PROVRESP_FALSE already defined
#endif
/** \brief Minimum response length when DcmDspVehInfoNODIProvResp is set to false */
#define DCM_OBD_VEHINFO_MIN_SIZE_WITH_NODI_PROVRESP_FALSE 1U

/*===========================[type definitions]=================================================*/

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*==================[internal function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

#if (DCM_DSP_USE_SERVICE_0X09 == STD_ON)
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_ReqVehInfoAvailability(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestVehicleInformation_ServiceExecution(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*=======================[external data declarations]============================================*/
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.RequestVehicleInformation.RuntimeDataElements,1 */
#if (DCM_DSP_USE_SERVICE_0X09 == STD_ON)
VAR(Dcm_DspReqVehInfoContextType, DCM_VAR) Dcm_DspReqVehInfoContext;
#endif

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*=======================[internal data declarations]============================================*/

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.RequestVehicleInformation.RuntimeDataElements.BufferConfig,1 */
#if (DCM_DSP_USE_SERVICE_0X09 == STD_ON)
STATIC VAR(Dcm_DspMsgBufferCfgType, DCM_VAR) Dcm_DspReqVehInfoBufferConfig;
#endif

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.RequestVehicleInformation.RuntimeDataElements.LockStatus,1 */
#if (DCM_DSP_USE_SERVICE_0X09 == STD_ON)
#if(DCM_DSP_VINSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
STATIC VAR(uint8, DCM_VAR) Dcm_RequestVehicleInformation_LockStatus = DCM_TRYING_LOCK;
#endif /* #if(DCM_DSP_VINSERVICES_LOCK_MECHANISM_ENABLED == STD_ON) */
#endif

#define DCM_STOP_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

#if (DCM_DSP_USE_SERVICE_0X09 == STD_ON)

/* !LINKSTO Dcm.Dsn.IB.RequestVehicleInformation.ExecuteAvailableInfoType,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_RequestVehicleInformation_HelperExecuteAvailableInfoType(
    Dcm_DspVehInfoInfoTypeType VehInfoInfoType,
    P2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) svcBufferContext)
{
  Std_ReturnType retVal = DCM_E_NOT_OK;
  const uint16 Occupied = (uint16)(svcBufferContext->Occupied);
  const uint16 Size = (uint16)(svcBufferContext->Size);

  DBG_DCM_REQUESTVEHICLEINFORMATION_HELPEREXECUTEAVAILABLEINFOTYPE_ENTRY(
      VehInfoInfoType, svcBufferContext);

  if (Size >= (Occupied + DCM_OBD_VEHINFO_AVAILDATALEN))
  {
#if ((DCM_DSP_VEHINFO_NUM > 0) && (DCM_DSP_VEHINFO_USED_ARRAY_SIZE > 0))
    uint8_least vehInfoIndex;
    TS_MemSet(&(svcBufferContext->BufferPtr[Occupied]),0U,DCM_OBD_VEHINFO_AVAILDATALEN);

    /* Deviation TASKING-1 */
    for (vehInfoIndex=0U;vehInfoIndex<DCM_DSP_VEHINFO_NUM;vehInfoIndex++)
    {
      if (DCM_IS_VEHINFOUSED_IDX_SET(vehInfoIndex))
      {
        const uint8 currentVehInfoInfoType = Dcm_DspVehInfo[vehInfoIndex].VehInfoInfoType;

        if (Dcm_OBDSupport_UpdateAvailableInfoWithID(currentVehInfoInfoType,
            VehInfoInfoType,
            &(svcBufferContext->BufferPtr[Occupied/*INFTYP*/]))==DCM_E_OK)
        {
          retVal = DCM_E_OK;
        }
      }
    }
#else
    TS_PARAM_UNUSED(VehInfoInfoType);
#endif /* ((DCM_DSP_VEHINFO_NUM > 0) && (DCM_DSP_VEHINFO_USED_ARRAY_SIZE > 0)) */
  }
  if (retVal == DCM_E_OK)
  {
    svcBufferContext->Occupied += DCM_OBD_VEHINFO_AVAILDATALEN;
  }
  else
  {
    /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.InfoType,1 */
    Dcm_DspReqVehInfoContext.nrc = DCM_E_REQUESTOUTOFRANGE;
  }

  DBG_DCM_REQUESTVEHICLEINFORMATION_HELPEREXECUTEAVAILABLEINFOTYPE_EXIT(
      retVal, VehInfoInfoType, svcBufferContext);

  return retVal;
}

/* !LINKSTO Dcm.Dsn.IB.RequestVehicleInformation.ExecuteNonAvailableInfoType,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_RequestVehicleInformation_HelperExecuteNonAvailableInfoType(
    Dcm_OpStatusType OpStatus,
    P2VAR(Dcm_DspReqVehInfoContextType, AUTOMATIC, DCM_VAR) svcContext)
{
  Std_ReturnType InterfaceReturnValue = DCM_E_NOT_OK;

  /* load context a */
  const uint8 VehInfoIdx = svcContext->currentVehInfoIdx;

  DBG_DCM_REQUESTVEHICLEINFORMATION_HELPEREXECUTENONAVAILABLEINFOTYPE_ENTRY(
      OpStatus, svcContext);

#if (DCM_DSP_VEHINFO_NUM > 0)
  if (VehInfoIdx < DCM_DSP_VEHINFO_NUM)
  {
    /* current requested VehInfo */
    CONSTP2CONST(Dcm_DspVehInfoType, AUTOMATIC, DCM_CONST) pVehInfo =
        &(Dcm_DspVehInfo[VehInfoIdx]);
    /* load context b */
    uint8_least VehInfoDataRelIndex;
    CONSTP2VAR(uint8, AUTOMATIC, DCM_VAR) pBuffer =
        svcContext->BufferCfgPtr->BufferPtr;
    uint32 BufferOccupied = svcContext->BufferCfgPtr->Occupied;

#if (DCM_DSP_VEHINFODATA_NUM > 0)
    Dcm_OpStatusType OperationStatus = OpStatus;
    /* for the rest of the VehInfoData */
    for (VehInfoDataRelIndex = svcContext->currentVehInfoDataIdx;
         VehInfoDataRelIndex < pVehInfo->NumOfVehInfoData;
         VehInfoDataRelIndex++)
    {
      /* pointer to current VehInfoData */
      CONSTP2CONST(Dcm_DspVehInfoDataType, AUTOMATIC, DCM_CONST) pVehInfoData =
          &(Dcm_DspVehInfoData[pVehInfo->StartIndexOfVehInfoData + VehInfoDataRelIndex]);

      /* Function pointer to get current VehInfoData */
      const Dcm_GetInfotypeValueDataFunctionPointerType pGetInfotypeValueData =
          pVehInfoData->GetInfotypeValueDataFunctionPointer;
#if (DCM_DSP_VEHINFO_AUTOSAR_VERSION == DCM_DSP_VEHINFO_AUTOSAR_440)
      uint8 DataValueBufferSize = 0U;

      /* Compute the maximum available size which is available for populating */
      if ((svcContext->BufferCfgPtr->Size - BufferOccupied) > DCM_OBD_VEHINFO_MAX_BUFFER_SIZE)
      {
        DataValueBufferSize = DCM_OBD_VEHINFO_MAX_BUFFER_SIZE;
      }
      else
      {
        DataValueBufferSize = (uint8)(svcContext->BufferCfgPtr->Size - BufferOccupied);
      }
      /* call getter function */
      /* !LINKSTO Dcm.RequestVehicleInformation.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortFalse,1 */
      /* !LINKSTO Dcm.RequestVehicleInformation.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortTrue,1 */
      /* !LINKSTO Dcm.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoNODIProvRespTrue,1 */
      InterfaceReturnValue = pGetInfotypeValueData(OperationStatus, &(pBuffer[BufferOccupied]), &DataValueBufferSize);
#else /* (DCM_DSP_VEHINFO_AUTOSAR_VERSION == DCM_DSP_VEHINFO_AUTOSAR_440) */
      /* call getter function */
      /* !LINKSTO Dcm.RequestVehicleInformation.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortFalse,1 */
      /* !LINKSTO Dcm.RequestVehicleInformation.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortTrue,1 */
      /* !LINKSTO Dcm.RequestVehicleInformation.DcmDspVehInfoASRVersion403.NonAvailabilityInfoType.DcmDspVehInfoNODIProvRespFalse,1 */
      InterfaceReturnValue = pGetInfotypeValueData(OperationStatus, &(pBuffer[BufferOccupied]));
#endif /* (DCM_DSP_VEHINFO_AUTOSAR_VERSION == DCM_DSP_VEHINFO_AUTOSAR_440) */
      switch (InterfaceReturnValue)
      {
        case E_OK:
          /* if Getter function OK */
          /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_y,1 */
          {
#if (DCM_DSP_VEHINFO_AUTOSAR_VERSION == DCM_DSP_VEHINFO_AUTOSAR_440)
            uint8 MinDataValueBufferSize = 0U;
            if (pVehInfo->DcmDspVehInfoNODIProvResp == TRUE)
            {
              /* In case the returned buffer size is lower than 2 (at least a data and the NODI shall be present) report Det and return NRC */
              MinDataValueBufferSize = DCM_OBD_VEHINFO_MIN_SIZE_WITH_NODI_PROVRESP_TRUE;
            }
            else
            {
              /* In case the returned buffer size is lower than 1 (at least a data shall be present) report Det and return NRC */
              MinDataValueBufferSize = DCM_OBD_VEHINFO_MIN_SIZE_WITH_NODI_PROVRESP_FALSE;
            }
            if (DataValueBufferSize < MinDataValueBufferSize)
            {
              /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortFalse.DcmDspVehInfoNODIProvRespTrue.DcmDevErrorDetectTrue.InsufficientData.ReportDET,1 */
              /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortFalse.DcmDspVehInfoNODIProvRespFalse.DcmDevErrorDetectTrue.InsufficientData.ReportDET,1 */
              /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortTrue.DcmDspVehInfoNODIProvRespTrue.DcmDevErrorDetectTrue.InsufficientData.ReportDET,1 */
              /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortTrue.DcmDspVehInfoNODIProvRespFalse.DcmDevErrorDetectTrue.InsufficientData.ReportDET,1 */
#if (DCM_DEV_ERROR_DETECT == STD_ON)
              DCM_DET_REPORT_ERROR(DCM_SVCID_GETINFOTYPEVALUEDATA, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
              InterfaceReturnValue  = DCM_E_NOT_OK;
              /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortFalse.DcmDspVehInfoNODIProvRespTrue.InsufficientData.ConditionsNotCorrectNRC,1 */
              /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortFalse.DcmDspVehInfoNODIProvRespFalse.InsufficientData.ConditionsNotCorrectNRC,1 */
              /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortTrue.DcmDspVehInfoNODIProvRespTrue.InsufficientData.ConditionsNotCorrectNRC,1 */
              /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortTrue.DcmDspVehInfoNODIProvRespFalse.InsufficientData.ConditionsNotCorrectNRC,1 */
              svcContext->nrc = DCM_E_CONDITIONSNOTCORRECT;
            }
            else if (DataValueBufferSize > (svcContext->BufferCfgPtr->Size - BufferOccupied))
            {
                /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortFalse.DcmDevErrorDetectTrue.DataValueBufferSizeOverflow.ReportDET,1 */
                /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortTrue.DcmDevErrorDetectTrue.DataValueBufferSizeOverflow.ReportDET,1 */
#if (DCM_DEV_ERROR_DETECT == STD_ON)
                DCM_DET_REPORT_ERROR(DCM_SVCID_GETINFOTYPEVALUEDATA, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
                InterfaceReturnValue  = DCM_E_NOT_OK;
                /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortFalse.DataValueBufferSizeOverflow.ConditionsNotCorrectNRC,1 */
                /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortTrue.DataValueBufferSizeOverflow.ConditionsNotCorrectNRC,1 */
                svcContext->nrc = DCM_E_CONDITIONSNOTCORRECT;
            }
            else
            {
              InterfaceReturnValue = DCM_E_OK;

              if (pVehInfo->DcmDspVehInfoNODIProvResp == TRUE)
              {
                /* increase count of occupied bytes in response buffer */
                if (DataValueBufferSize < (pVehInfoData->VehInfoDataSize + 1U))
                {
                  /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortFalse.DcmDspVehInfoNODIProvRespTrue.LessData,1 */
                  /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortTrue.DcmDspVehInfoNODIProvRespTrue.LessData,1 */
                  BufferOccupied += DataValueBufferSize;
                }
                else
                {
                  /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortFalse.DcmDspVehInfoNODIProvRespTrue.IgnoreExtraData,1 */
                  /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortTrue.DcmDspVehInfoNODIProvRespTrue.IgnoreExtraData,1 */
                  BufferOccupied = BufferOccupied + pVehInfoData->VehInfoDataSize + 1U;
                }
              }
              else
              {
                /* increase count of occupied bytes in response buffer */
                if (DataValueBufferSize < pVehInfoData->VehInfoDataSize)
                {
                  /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortFalse.DcmDspVehInfoNODIProvRespFalse.LessData,1 */
                  /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortTrue.DcmDspVehInfoNODIProvRespFalse.LessData,1 */
                  BufferOccupied += DataValueBufferSize;
                }
                else
                {
                  /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortFalse.DcmDspVehInfoNODIProvRespFalse.IgnoreExtraData,1 */
                  /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortTrue.DcmDspVehInfoNODIProvRespFalse.IgnoreExtraData,1 */
                  BufferOccupied += pVehInfoData->VehInfoDataSize;
                }
              }
            }

#else /* (DCM_DSP_VEHINFO_AUTOSAR_VERSION == DCM_DSP_VEHINFO_AUTOSAR_440) */
            /* increase count of occupied bytes in response buffer */
            BufferOccupied += pVehInfoData->VehInfoDataSize;
            InterfaceReturnValue = DCM_E_OK;
#endif /* (DCM_DSP_VEHINFO_AUTOSAR_VERSION == DCM_DSP_VEHINFO_AUTOSAR_440) */
            /* prepare OperationStatus for next call */
            OperationStatus = DCM_INITIAL;
            break;
          }
        /* No data; conditions not correct */
        case E_NOT_OK:
          /* !LINKSTO Dcm.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortFalse.ConditionsNotCorrectNRC,1 */
          /* !LINKSTO Dcm.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortTrue.ConditionsNotCorrectNRC,1 */
          svcContext->nrc = DCM_E_CONDITIONSNOTCORRECT;
          InterfaceReturnValue = DCM_E_NOT_OK;
          break;
          /* No data; conditions correct;
           * --> NRC 0x78, recall pending */
          /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_z,1 */
        case DCM_E_PENDING:
          /* Do nothing only return DCM_E_PENDING */
          break;
        default:
          /* !LINKSTO Dcm.RequestVehicleInformation.DcmDspVehInfoASRVersion440.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortFalse.ConditionsNotCorrectNRC,1 */
          svcContext->nrc = DCM_E_CONDITIONSNOTCORRECT;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
          /* !LINKSTO Dcm.RequestVehicleInformation.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortFalse.DcmDevErrorDetectTrue.ReportDET,1 */
          /* !LINKSTO Dcm.RequestVehicleInformation.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortTrue.DcmDevErrorDetectTrue.ReportDET,1 */
          DCM_DET_REPORT_ERROR(DCM_SVCID_GETINFOTYPEVALUEDATA, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
          InterfaceReturnValue = DCM_E_NOT_OK;
          break;
        }
        /* If callback function returns DCM_E_PENDING, E_NOT_OK or any other undefined value diffrent than E_OK,
         * stop further processing.
         */
        if (InterfaceReturnValue != DCM_E_OK)
        {
          break;
        }
    }
#else
    TS_PARAM_UNUSED(OpStatus);
#endif /* (DCM_DSP_VEHINFODATA_NUM > 0) */
    /* save context */
    svcContext->currentVehInfoDataIdx= (uint8)VehInfoDataRelIndex;
    svcContext->BufferCfgPtr->Occupied = BufferOccupied;
  }
#else
  TS_PARAM_UNUSED(OpStatus);
  TS_PARAM_UNUSED(svcContext);
  TS_PARAM_UNUSED(VehInfoIdx);
#endif /* (DCM_DSP_VEHINFO_NUM > 0) */

  DBG_DCM_REQUESTVEHICLEINFORMATION_HELPEREXECUTENONAVAILABLEINFOTYPE_EXIT(
      InterfaceReturnValue, OpStatus, svcContext);

  return InterfaceReturnValue;
}

/* This is the generic service handler for UDS service 0x09 */
/* !LINKSTO Dcm.Dsn.IB.RequestVehicleInformation.SvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_RequestVehicleInformation_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  DBG_DCM_DSP_REQUESTVEHICLEINFORMATION_SVCH_ENTRY(OpStatus,pMsgContext);

#if(DCM_DSP_VINSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
  switch(Dcm_RequestVehicleInformation_LockStatus)
  {
    case DCM_TRYING_LOCK:
    {
      if((OpStatus != DCM_CANCEL) &&
         (OpStatus != DCM_CONFIRMED_OK) &&
         (OpStatus != DCM_CONFIRMED_NOK))
      {
        Std_ReturnType VinLockStatus = Dcm_Dsp_VinServices_LockService();

        if (VinLockStatus == DCM_E_RESOURCE_LOCKED)
        {
          /* !LINKSTO Dcm.EB.RequestVehicleInformation.BusyWithReadDataByIdentifier.Postpone,1 */
          ServiceProcessStatus = DCM_E_PENDING;
        }
        else
        {
          Dcm_RequestVehicleInformation_LockStatus = DCM_EXECUTING_SERVICE;
          ServiceProcessStatus = Dcm_Dsp_RequestVehicleInformation_ServiceExecution(DCM_INITIAL,
                                                                                      pMsgContext);
        }
      }
      else
      {
        ServiceProcessStatus = DCM_E_DONE;
      }
    }
    break;

    case DCM_EXECUTING_SERVICE:
    {

      ServiceProcessStatus = Dcm_Dsp_RequestVehicleInformation_ServiceExecution(OpStatus, pMsgContext);
    }
    break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      ServiceProcessStatus = DCM_E_DONE;
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_OBD_DEFENSIVE_BRANCH);
      break;
    /* CHECK: PARSE */
  }
#else
  ServiceProcessStatus = Dcm_Dsp_RequestVehicleInformation_ServiceExecution(OpStatus, pMsgContext);
#endif /* #if(DCM_DSP_VINSERVICES_LOCK_MECHANISM_ENABLED == STD_ON) */

  DBG_DCM_DSP_REQUESTVEHICLEINFORMATION_SVCH_EXIT(ServiceProcessStatus,OpStatus,pMsgContext);

  return ServiceProcessStatus;
}

#endif /* #if(DCM_DSP_USE_SERVICE_0X09 == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

#if (DCM_DSP_USE_SERVICE_0X09 == STD_ON)
/* !LINKSTO SWS_Dcm_00422,1, Dcm.Dsn.IB.RequestVehicleInformation.ReqVehInfoAvailability,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_ReqVehInfoAvailability(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType retVal = DCM_E_NOT_OK;
  Dcm_MsgLenType reqDataLen = pMsgContext->reqDataLen;
  /* Pointer to the list array of VehInfos. Can be either the requestMessage
   * or, in case a common buffer is used, a separate buffer to which the
   * content of the request message is copied before it is overwritten by
   * the response message */
  Dcm_MsgType reqData = pMsgContext->reqData;

  /* check max size */
  /* !LINKSTO  Dcm.Dsn.ReqVehInfo.IB.Check.MaxReqLen,1, Dcm.ISO-15031-5_8.9.1_1,1 */
  if (reqDataLen <= DCM_OBD_MAX_REQ_VEHINFO_AVAIL_SERVICE_09)
  {
    /* check if combined events */
    /* !LINKSTO SWS_Dcm_00949,1 */
    /* !LINKSTO Dcm.EB.RequestVehicleInformation.AvailabilityInfoType.NonAvailabilityInfoType.Mixture.NoResponse,1 */
    if (Dcm_OBDSupport_CheckRequestCombinedIds(reqData, reqDataLen)==DCM_E_OK)
    {
      uint8_least reqIndex = 0U;
      Dcm_MsgLenType ResponsePos = 0U;
      const Dcm_MsgLenType lastResponsePos =
          pMsgContext->resMaxDataLen - (DCM_OBD_VEHINFO_AVAILDATALEN + 1U/*INFTYP*/);

#if (DCM_COMMON_BUFF_CONFIGURED == STD_ON)
      /* Dcm_ReqVehInfoBuffer is used to store the request
       * if a single buffer is configured as Rx and Tx buffer.
       * Size of buffer is the max number of VehInfo that can be
       * requested in service 09 i.e. 6 VehInfo meaning 6 bytes */
      Dcm_MsgItemType Dcm_ReqVehInfoBuffer[DCM_OBD_MAX_REQ_VEHINFO_AVAIL_SERVICE_09];

      if(reqData == pMsgContext->resData)
      {
        TS_MemCpy(Dcm_ReqVehInfoBuffer, reqData, reqDataLen);
        reqData = Dcm_ReqVehInfoBuffer;
      }
#endif
      /* !LINKSTO Dcm.EB.RequestVehicleInformation.AvailabilityInfoType.ResponseBufferTooSmall.LimitAvailabilityInfoType,1 */
      for(reqIndex=0U;
          (reqIndex<reqDataLen)&&(ResponsePos<=lastResponsePos);
          reqIndex++)
      {
        /* send first the InfoType */
        Dcm_DspReqVehInfoBufferConfig.BufferPtr[Dcm_DspReqVehInfoBufferConfig.Occupied] =
          reqData[reqIndex];
        Dcm_DspReqVehInfoBufferConfig.Occupied++;
        /* !LINKSTO Dcm.ISO-15031-5_8.9.2.2_1,1 */
        if (Dcm_RequestVehicleInformation_HelperExecuteAvailableInfoType(
                (Dcm_DspVehInfoInfoTypeType)reqData[reqIndex],
                &Dcm_DspReqVehInfoBufferConfig) == DCM_E_OK)
        {
          retVal = DCM_E_OK;
        }
        else
        {
          /* availability InfoType was not added */
          Dcm_DspReqVehInfoBufferConfig.Occupied--;
        }
      }
      pMsgContext->resDataLen = Dcm_DspReqVehInfoBufferConfig.Occupied;
    }
  }
  else
  {
    Dcm_DspReqVehInfoContext.nrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
  }

  return retVal;
}

/* This is the generic service handler for UDS service 0x09 */
/* !LINKSTO Dcm.Dsn.IB.RequestVehicleInformation.ServiceExecution,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestVehicleInformation_ServiceExecution(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_NO_DATA;
  const Dcm_MsgLenType reqDataLen = pMsgContext->reqDataLen;

  DBG_DCM_DSP_REQUESTVEHICLEINFORMATION_SERVICEEXECUTION_ENTRY(
      OpStatus, pMsgContext);
  /* Deviation MISRAC2012-2 */
  switch (OpStatus)
  {
    /* first call of service function */
    case DCM_INITIAL:
      /* reset NRC */
      Dcm_DspReqVehInfoContext.nrc = DCM_E_POSITIVERESPONSE;

      /* check minimal length */
      /* !LINKSTO Dcm.Dsn.ReqVehInfo.SuppressResponse,1 */
      /* !LINKSTO Dcm.Dsn.ReqVehInfo.IB.Check.MinReqLen,1 */
     if (reqDataLen >= DCM_OBD_MIN_REQ_VEHINFO_SERVICE_09)
      {
       const uint8 firstReqVehInfo = pMsgContext->reqData[0U];
        /* initialize response buffer configuration */
        Dcm_DspReqVehInfoBufferConfig.BufferPtr = pMsgContext->resData;
        Dcm_DspReqVehInfoBufferConfig.Size = pMsgContext->resMaxDataLen;
        Dcm_DspReqVehInfoBufferConfig.Occupied = 0U;

        /* Availability request */
        if (DCM_IS_AVAILABILITY_OBDSUBID(firstReqVehInfo) == TRUE)
        {
          /* Calculate and report requested availability InfoTypes */
          serviceProcessStatus = Dcm_ReqVehInfoAvailability(pMsgContext);
        }
        else
        /* Non availability request */
        /* !LINKSTO Dcm.RequestVehicleInformation.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortFalse,1 */
        /* !LINKSTO Dcm.RequestVehicleInformation.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortTrue,1 */
        {
          /* !LINKSTO Dcm.ISO-15031-5_8.9.1_2,1 */
          if (reqDataLen == DCM_OBD_MAX_REQ_VEHINFO_NONAVAIL_SERVICE_09)
          {
            /* Init the context for a single ReqVehInfoType */
            /* !LINKSTO Dcm.Dsn.ReqVehInfo.IB.SvcInit,1 */
            serviceProcessStatus =
                Dcm_Dsp_VinServices_HelperInit(firstReqVehInfo,
                    &Dcm_DspReqVehInfoContext, &Dcm_DspReqVehInfoBufferConfig);
            if (serviceProcessStatus == DCM_E_OK)
            {
              CONSTP2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) pBuffer =
                  Dcm_DspReqVehInfoContext.BufferCfgPtr;
#if (DCM_DSP_VEHINFO_NUM > 0)
              const uint8 VehInfoIdx = Dcm_DspReqVehInfoContext.currentVehInfoIdx;
#endif

              /* INFTYP */
              pBuffer->BufferPtr[0U] = firstReqVehInfo;
              pBuffer->Occupied += 1U;
              /* NODI */
#if (DCM_DSP_VEHINFO_NUM > 0)
#if (DCM_DSP_VEHINFO_AUTOSAR_VERSION == DCM_DSP_VEHINFO_AUTOSAR_440)
              if (Dcm_DspVehInfo[VehInfoIdx].DcmDspVehInfoNODIProvResp == FALSE)
#endif /* (DCM_DSP_VEHINFO_AUTOSAR_VERSION == DCM_DSP_VEHINFO_AUTOSAR_440) */
              {
                /* In case DcmDspVehInfoNODIProvResp is false the NODI parameter is provided by the DCM*/
                /* !LINKSTO Dcm.RequestVehicleInformation.DcmDspVehInfoASRVersion403.NonAvailabilityInfoType.DcmDspVehInfoNODIProvRespFalse,1 */
                pBuffer->BufferPtr[1U] = Dcm_DspVehInfo[VehInfoIdx].NumOfVehInfoData;
                pBuffer->Occupied += 1U;
              }
#else /* (DCM_DSP_VEHINFO_NUM > 0) */
              pBuffer->BufferPtr[1U] = 0U;
              pBuffer->Occupied += 1U;
#endif /* (DCM_DSP_VEHINFO_NUM > 0) */


              /* Get the requested non-availability VehInfoType */
              serviceProcessStatus =
                  Dcm_RequestVehicleInformation_HelperExecuteNonAvailableInfoType(
                      OpStatus, &Dcm_DspReqVehInfoContext);
            }
          }
          else
          {
            Dcm_DspReqVehInfoContext.nrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
            serviceProcessStatus = E_NOT_OK;
          }
        }
      }
      else
      {
        Dcm_DspReqVehInfoContext.nrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
        serviceProcessStatus = E_NOT_OK;
      }
      break;

    case DCM_PENDING:
      /* !LINKSTO SWS_Dcm_00421,1 */
      serviceProcessStatus =
          Dcm_RequestVehicleInformation_HelperExecuteNonAvailableInfoType(
              OpStatus, &Dcm_DspReqVehInfoContext);
      break;

    /* Deviation MISRAC2012-1 */
    /* Deviation MISRA-1 */
    case DCM_CANCEL:
      Dcm_Dsp_VinServices_HelperCancel(&Dcm_DspReqVehInfoContext);
#if(DCM_DSP_VINSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
      Dcm_Dsp_VinServices_UnlockService();
      Dcm_RequestVehicleInformation_LockStatus = DCM_TRYING_LOCK;
#endif /* #if(DCM_DSP_VINSERVICES_LOCK_MECHANISM_ENABLED == STD_ON) */
      /* no break */

    case DCM_CONFIRMED_OK:
    case DCM_CONFIRMED_NOK:
      serviceProcessStatus = DCM_E_DONE;
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_OBD_DEFENSIVE_BRANCH);
      break;
    /* CHECK: PARSE */
  }

  switch (serviceProcessStatus)
  {
    case DCM_E_OK:
      /* service finished successful */
      /* set response size ... */
      pMsgContext->resDataLen = (uint16)(Dcm_DspReqVehInfoBufferConfig.Occupied);
      /* ... and (positive) return status DONE */
#if(DCM_DSP_VINSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
      Dcm_Dsp_VinServices_UnlockService();
      Dcm_RequestVehicleInformation_LockStatus = DCM_TRYING_LOCK;
#endif /* #if(DCM_DSP_VINSERVICES_LOCK_MECHANISM_ENABLED == STD_ON) */
      serviceProcessStatus = DCM_E_DONE;
      break;
    case DCM_E_NOT_OK:
      /* service failed */
      /* suppress response ... */
      /* !LINKSTO Dcm.Dsn.ReqVehInfo.SuppressResponse,1 */
      /* !LINKSTO Dcm.Dsn.IB.ReqVehInfo.InfoTypeSupported.NoRes,1 */
#if (DCM_DSP_VEHINFO_AUTOSAR_VERSION == DCM_DSP_VEHINFO_AUTOSAR_440)
      if (Dcm_DspReqVehInfoContext.nrc == DCM_E_CONDITIONSNOTCORRECT)
      {
        Dcm_ExternalSetNegResponse(pMsgContext, Dcm_DspReqVehInfoContext.nrc);
      }
      else
#endif /* (DCM_DSP_VEHINFO_AUTOSAR_VERSION == DCM_DSP_VEHINFO_AUTOSAR_440) */
      {
        /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion403.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortTrue.NoResponse,1 */
        /* !LINKSTO Dcm.EB.RequestVehicleInformation.DcmDspVehInfoASRVersion403.NonAvailabilityInfoType.DcmDspVehInfoDataUsePortFalse.NoResponse,1 */
        /* !LINKSTO Dcm.EB.RequestVehicleInformation.NonAvailabilityInfoType.NotConfigured.NoResponse,1 */
        /* !LINKSTO Dcm.EB.RequestVehicleInformation.AvailabilityInfoType.NotConfigured.NoResponse,1 */
        /* !LINKSTO Dcm.EB.RequestVehicleInformation.AvailabilityInfoType.IncorrectRequestMessageLength.NoResponse,1 */
        /* !LINKSTO Dcm.EB.RequestVehicleInformation.NonAvailabilityInfoType.IncorrectRequestMessageLength.NoResponse,1 */
        pMsgContext->msgAddInfo.suppressPosResponse = TRUE;
      }
      /* ... and set (positive) return status DCM_E_DONE */
#if(DCM_DSP_VINSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
      Dcm_Dsp_VinServices_UnlockService();
      Dcm_RequestVehicleInformation_LockStatus = DCM_TRYING_LOCK;
#endif /* #if(DCM_DSP_VINSERVICES_LOCK_MECHANISM_ENABLED == STD_ON) */
      serviceProcessStatus = DCM_E_DONE;
      break;
    case DCM_E_DONE:
      /* all fine; do nothing */
      break;
    case DCM_E_PENDING:
      /* Do nothing only return DCM_E_PENDING */
      break;
    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_OBD_DEFENSIVE_BRANCH);
      break;
    /* CHECK: PARSE */
  }

  DBG_DCM_DSP_REQUESTVEHICLEINFORMATION_SERVICEEXECUTION_EXIT(
      serviceProcessStatus, OpStatus, pMsgContext);

  return serviceProcessStatus;
}
#endif /* (DCM_DSP_USE_SERVICE_0X09 == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[end of file]================================================================*/
