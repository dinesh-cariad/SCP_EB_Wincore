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
/* !LINKSTO Dcm.Dsn.File.LowerLayerInterfaces.MetaData.Impl,1 */
/*==================[inclusions]=================================================================*/

#include <TSAutosar.h>
#include <TSMem.h>                                        /* EB specific memory functions */

#include <Dcm_Dsl_CommunicationServices_MetaData.h>       /* Dcm Dsl MetaData header file */
#include <Dcm_Dsl_CommunicationServices_TxConnections.h>

#include <Dcm_Int.h>                                      /* Module intenal definitions */
#include <Dcm_Trace.h>

#if (DCM_DEV_ERROR_DETECT == STD_ON)
/* Development Error Tracer (DCM_DET_REPORT_ERROR() macros) */
#include <Det.h>
#endif
#if (DCM_META_DATA_HANDLING_ENABLED == STD_ON)
#include <EcuC.h>
#endif /* #if (DCM_META_DATA_HANDLING_ENABLED == STD_ON) */
/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#if (DCM_META_DATA_HANDLING_ENABLED == STD_ON)
/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.MetaData.RuntimeDataElements, 1 */
VAR(Dcm_PduInfoType, DCM_VAR) Dcm_PduInfo[DCM_NUM_GENERIC_CONNECTIONS];
#endif /* #if (DCM_META_DATA_HANDLING_ENABLED == STD_ON) */

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

#if (DCM_META_DATA_HANDLING_ENABLED == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.LowerLayerInterfaces.MetaData.GetEcuCMetaDataInfo,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_GetEcuCMetaDataInfo(
  PduIdType RxPduIdx)
{
  Std_ReturnType ReturnFromEcuC = E_NOT_OK;
  Dcm_MainConnectionIndexType MainConnIdx = Dcm_RxPduIdConfig[RxPduIdx].MainConnectionIndex;
  Dcm_TxConnectionIndexType TxConnIdx = Dcm_MainConnectionConfig[MainConnIdx].ConfirmationTxPduId;
  Dcm_GenericConnectionIndexType GenericConnIdx = Dcm_MainConnectionConfig[MainConnIdx].GenericConnectionIndex;
  PduIdType EcuPduId = Dcm_RxPduIdConfig[RxPduIdx].RxEcuCPduId;

  DBG_DCM_DSL_GETECUCMETADATAINFO_ENTRY(MainConnIdx);

  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.GetMetaDataSourceAddr,1 */
  ReturnFromEcuC = EcuC_GetMetaDataSourceAddr(EcuPduId,
     &Dcm_TxConnections[TxConnIdx].TxPduInfo, &Dcm_PduInfo[GenericConnIdx].SourceAddress);

  if(E_OK == ReturnFromEcuC)
  {
    /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.GetMetaDataTargetAddr,1 */
    ReturnFromEcuC = EcuC_GetMetaDataTargetAddr(EcuPduId,
        &Dcm_TxConnections[TxConnIdx].TxPduInfo, &Dcm_PduInfo[GenericConnIdx].TargetAddress);
  }

  DBG_DCM_DSL_GETECUCMETADATAINFO_EXIT();

  return ReturnFromEcuC;
}

/* !LINKSTO Dcm.Dsn.IB.LowerLayerInterfaces.MetaData.SetEcuCMetaDataInfo,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsl_SetEcuCMetaDataInfo(
  PduIdType EcuPduId,
  Dcm_MainConnectionIndexType GenericConIdx,
  P2VAR(PduInfoType, AUTOMATIC, EBSTUBS_APPL_CONST) PduInfoPtr)
{
  Std_ReturnType ReturnFromEcuC = E_NOT_OK;

  DBG_DCM_DSL_SETECUCMETADATAINFO_ENTRY(EcuPduId, GenericConIdx, Info);

  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.Response.DcmGenericConnectionUsage.SetMetaDataSourceAddr,1 */
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.TYPE1ROE.DcmGenericConnectionUsage.SetMetaDataSourceAddr,1 */
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.TYPE2ROE.DcmGenericConnectionUsage.SetMetaDataSourceAddr,1 */
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.TYPE1Periodic.DcmGenericConnectionUsage.SetMetaDataSourceAddr,1 */
  /* !LINKSTO Dcm.EB.LowerLayerInterfaces.TYPE2Periodic.DcmGenericConnectionUsage.SetMetaDataSourceAddr,1 */
  ReturnFromEcuC = EcuC_SetMetaDataSourceAddr(EcuPduId,
                                              PduInfoPtr,
                                              Dcm_PduInfo[GenericConIdx].TargetAddress);

  if(E_OK == ReturnFromEcuC)
  {
    /* !LINKSTO Dcm.EB.LowerLayerInterfaces.Response.DcmGenericConnectionUsage.SetMetaDataTargetAddr,1 */
    /* !LINKSTO Dcm.EB.LowerLayerInterfaces.TYPE1ROE.DcmGenericConnectionUsage.SetMetaDataTargetAddr,1 */
    /* !LINKSTO Dcm.EB.LowerLayerInterfaces.TYPE2ROE.DcmGenericConnectionUsage.SetMetaDataTargetAddr,1 */
    /* !LINKSTO Dcm.EB.LowerLayerInterfaces.TYPE1Periodic.DcmGenericConnectionUsage.SetMetaDataTargetAddr,1 */
    /* !LINKSTO Dcm.EB.LowerLayerInterfaces.TYPE2Periodic.DcmGenericConnectionUsage.SetMetaDataTargetAddr,1 */
    ReturnFromEcuC = EcuC_SetMetaDataTargetAddr(EcuPduId,
                                                PduInfoPtr,
                                                Dcm_PduInfo[GenericConIdx].SourceAddress);
  }

  DBG_DCM_DSL_SETECUCMETADATAINFO_EXIT();

  return ReturnFromEcuC;

}
#endif /* #if (DCM_META_DATA_HANDLING_ENABLED == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*==================[end of file]================================================================*/
