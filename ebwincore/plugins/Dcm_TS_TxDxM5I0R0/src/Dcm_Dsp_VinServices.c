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

/* !LINKSTO Dcm.Dsn.File.VinServices.Impl,1 */
/* This file contains the implementation of the implementation of the VinServices software unit. */

/*===============================[includes]======================================================*/

#include <Dcm_Trace.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <TSAutosar.h>

#include <Dcm_Dsp_VinServices.h>

/*===========================[macro definitions]=================================================*/
#if (defined DCM_OBD_VEHINFO_AVAILDATALEN)
#error DCM_OBD_VEHINFO_AVAILDATALEN already defined
#endif
/** \brief Length of a single supported Vehicle InfoType response */
#define DCM_OBD_VEHINFO_AVAILDATALEN 4U /* DATA_A, ... DATA_D */
/*===========================[type definitions]==================================================*/

/*==================[internal function declarations]=============================================*/

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*==================[internal function declarations]=============================================*/

/*=======================[external data declarations]============================================*/

#define DCM_START_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.VinServices.RuntimeDataElements,1 */
/* !LINKSTO Dcm.Dsn.VinServices.RuntimeDataElements.ServiceBusy,1 */
#if(DCM_DSP_VINSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
STATIC VAR(Dcm_MutexType, DCM_VAR) Dcm_Dsp_VinServices_ServiceBusy = DCM_MUTEX_UNLOCKED;
#endif /* #if(DCM_DSP_VINSERVICES_LOCK_MECHANISM_ENABLED == STD_ON) */

#define DCM_STOP_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/*=======================[internal data declarations]============================================*/

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

#if (DCM_DSP_USE_SERVICE_0X09 == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.VinServices.HelperInit,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_VinServices_HelperInit(
  Dcm_DspVehInfoInfoTypeType  VehInfoInfoType,
  P2VAR(Dcm_DspReqVehInfoContextType, AUTOMATIC, DCM_VAR) svcContext,
  P2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) ResBuffer)
{
  Std_ReturnType retVal = DCM_E_NOT_OK;

  DBG_DCM_REQUESTVEHICLEINFORMATION_HELPERINIT_ENTRY(
      VehInfoInfoType, svcContext, ResBuffer);

  if (DCM_IS_AVAILABILITY_OBDSUBID(VehInfoInfoType) == TRUE)
  /* Availability vehicle information request */
  {
    /* initialize response buffer */
    if (ResBuffer->Size >= DCM_OBD_VEHINFO_AVAILDATALEN)
    {
      svcContext->BufferCfgPtr = ResBuffer;

      /* nothing to check, all fine */
      retVal = DCM_E_OK;
    }
  }
  else
  /* Non availability vehicle information request */
  {
#if ((DCM_DSP_VEHINFO_NUM > 0) && (DCM_DSP_VEHINFODATA_NUM > 0))
    uint8_least vehInfoIndex;

    for (vehInfoIndex=0U;vehInfoIndex < DCM_DSP_VEHINFO_NUM;vehInfoIndex++)
    {
      /* if VehInfo exists */
      /* !LINKSTO Dcm.ISO-15031-5_6.2.4.3.7_Table11_bb,1 */
      /* !LINKSTO Dcm.Dsn.ReqVehInfo.IB.Check.UnknowVehInfo,1 */
      if (Dcm_DspVehInfo[vehInfoIndex].VehInfoInfoType == VehInfoInfoType)
      {
#if (DCM_DSP_VEHINFO_USED_ARRAY_SIZE > 0)
        /* if VehInfo is used */
        if (DCM_IS_VEHINFOUSED_IDX_SET(vehInfoIndex))
        {
          /* calculate needed buffer size */
          uint16_least VehInfoDataSize = 0U;
          const uint16 StartIndexOfVehInfoData =
              Dcm_DspVehInfo[vehInfoIndex].StartIndexOfVehInfoData;
          const uint16 EndIndexOfVehInfoData =
              StartIndexOfVehInfoData + Dcm_DspVehInfo[vehInfoIndex].NumOfVehInfoData;
          uint16_least VehInfoDataIndex = 2U; /* INFTYP + NODI */
          for (VehInfoDataIndex=StartIndexOfVehInfoData;
              VehInfoDataIndex<EndIndexOfVehInfoData;VehInfoDataIndex++)
          {
            VehInfoDataSize += Dcm_DspVehInfoData[VehInfoDataIndex].VehInfoDataSize;
          }
          /* if response buffer size is sufficient */
          if (VehInfoDataSize <= ResBuffer->Size)
          {
            /* initialize SvcH context */
            svcContext->currentVehInfoIdx = (uint8)vehInfoIndex;
            svcContext->currentVehInfoDataIdx = 0U;
            svcContext->BufferCfgPtr = ResBuffer;
            retVal = DCM_E_OK;
            break;
          }
        }
#endif /* (DCM_DSP_VEHINFO_USED_ARRAY_SIZE > 0) */
      }
    }
#endif /* ((DCM_DSP_VEHINFO_NUM > 0) && (DCM_DSP_VEHINFODATA_NUM > 0)) */
  }

  if (retVal == DCM_E_NOT_OK)
  {
    svcContext->nrc = DCM_E_REQUESTOUTOFRANGE;
  }

  DBG_DCM_REQUESTVEHICLEINFORMATION_HELPERINIT_EXIT(
      retVal, VehInfoInfoType, svcContext, ResBuffer);

  return retVal;
}

/* !LINKSTO Dcm.Dsn.IB.VinServices.HelperExecute,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_VinServices_HelperExecute(
  Dcm_OpStatusType OpStatus,
  Dcm_DspVehInfoInfoTypeType VehInfoInfoType,
  P2VAR(Dcm_DspReqVehInfoContextType, AUTOMATIC, DCM_VAR) svcContext)
{
  Std_ReturnType retVal = DCM_E_NOT_OK;

  DBG_DCM_REQUESTVEHICLEINFORMATION_HELPEREXECUTE_ENTRY(
      OpStatus, VehInfoInfoType, svcContext);

  /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.InfoType,1 */
  if (DCM_IS_AVAILABILITY_OBDSUBID(VehInfoInfoType) == TRUE)
  {
    retVal = Dcm_RequestVehicleInformation_HelperExecuteAvailableInfoType(VehInfoInfoType,
        svcContext->BufferCfgPtr);

    if (retVal == E_NOT_OK)
    {
      svcContext->nrc = Dcm_DspReqVehInfoContext.nrc;
    }
  }
  else
  {
    retVal = Dcm_RequestVehicleInformation_HelperExecuteNonAvailableInfoType(OpStatus, svcContext);
  }

  DBG_DCM_REQUESTVEHICLEINFORMATION_HELPEREXECUTE_EXIT(
      retVal, OpStatus, VehInfoInfoType, svcContext);

  return retVal;
}

/* !LINKSTO Dcm.Dsn.IB.VinServices.HelperCancel,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_VinServices_HelperCancel(
    P2VAR(Dcm_DspReqVehInfoContextType, AUTOMATIC, DCM_VAR) svcContext)
{
  DBG_DCM_REQUESTVEHICLEINFORMATION_HELPERCANCEL_ENTRY(svcContext);

#if (DCM_DSP_VEHINFO_NUM > 0)
  if(svcContext->currentVehInfoIdx < DCM_DSP_VEHINFO_NUM)
  {
    CONSTP2CONST(Dcm_DspVehInfoType, AUTOMATIC, DCM_CONST) pVehInfo =
        &(Dcm_DspVehInfo[svcContext->currentVehInfoIdx]);
#if (DCM_DSP_VEHINFODATA_NUM > 0)
    const uint8 currDataIdx = svcContext->currentVehInfoDataIdx;

    if (currDataIdx < pVehInfo->NumOfVehInfoData)
    {
      CONSTP2CONST(Dcm_DspVehInfoDataType, AUTOMATIC, DCM_CONST) pVehInfoData =
          &(Dcm_DspVehInfoData[pVehInfo->StartIndexOfVehInfoData + currDataIdx]);
      CONSTP2VAR(uint8, AUTOMATIC, DCM_VAR) pBuffer =
         svcContext->BufferCfgPtr->BufferPtr;
#if (DCM_DSP_VEHINFO_AUTOSAR_VERSION == DCM_DSP_VEHINFO_AUTOSAR_440)
      uint8 DataValueBufferSize = 0;
      (void)pVehInfoData->GetInfotypeValueDataFunctionPointer(DCM_CANCEL, &pBuffer[0], &DataValueBufferSize);
#else /* #if (DCM_DSP_VEHINFO_AUTOSAR_VERSION == DCM_DSP_VEHINFO_AUTOSAR_440) */
      (void)pVehInfoData->GetInfotypeValueDataFunctionPointer(DCM_CANCEL, &pBuffer[0]);
#endif /* #if (DCM_DSP_VEHINFO_AUTOSAR_VERSION == DCM_DSP_VEHINFO_AUTOSAR_440) */
    }
#endif /* (DCM_DSP_VEHINFODATA_NUM > 0) */
  }
#endif /* (DCM_DSP_VEHINFO_NUM > 0) */

  svcContext->currentVehInfoIdx = DCM_DSP_VEHINFO_NUM;
  svcContext->currentVehInfoDataIdx = 0U;
  svcContext->BufferCfgPtr = NULL_PTR;
  svcContext->nrc = DCM_E_POSITIVERESPONSE;

  DBG_DCM_REQUESTVEHICLEINFORMATION_HELPERCANCEL_EXIT(svcContext);
}

#if(DCM_DSP_VINSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.VinServices.LockService,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_VinServices_LockService(void)
{
  Std_ReturnType RetVal = DCM_E_RESOURCE_LOCKED;

  DBG_DCM_DSP_DIDSERVICES_LOCKSERVICE_ENTRY();

  RetVal = Dcm_Internal_GetLock(&Dcm_Dsp_VinServices_ServiceBusy);

  DBG_DCM_DSP_DIDSERVICES_LOCKSERVICE_EXIT(RetVal);

  return RetVal;
}

/* !LINKSTO Dcm.Dsn.IB.VinServices.UnlockService,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_VinServices_UnlockService(void)
{
  DBG_DCM_DSP_DIDSERVICES_UNLOCKSERVICE_ENTRY();

  Dcm_Internal_ReleaseLock(&Dcm_Dsp_VinServices_ServiceBusy);

  DBG_DCM_DSP_DIDSERVICES_UNLOCKSERVICE_EXIT();
}
#endif /* #if(DCM_DSP_VINSERVICES_LOCK_MECHANISM_ENABLED == STD_ON) */

#endif /* #if(DCM_DSP_USE_SERVICE_0X09 == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[end of file]================================================================*/
