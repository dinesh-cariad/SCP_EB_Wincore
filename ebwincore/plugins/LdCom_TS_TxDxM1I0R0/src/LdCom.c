/**
 * \file
 *
 * \brief AUTOSAR LdCom
 *
 * This file contains the implementation of the AUTOSAR
 * module LdCom.
 *
 * \version 1.0.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */


/*
 *  MISRA-C:2012 Deviation List
 *
 *   MISRAC2012-1) Deviated Rule: 20.7 (Required)
 *   Expressions resulting from the expansion of macro parameters shall be enclosed in parentheses.
 *
 *   Reason:
 *   Macros are used for type independent casts. Such a parameter cannot be
 *   enclosed in parentheses as this would lead to a syntax error.
 *
 *   MISRAC2012-2) Deviated Rule: 11.1 (Required)
 *   Conversion shall not be performed between a pointer to a function and any other type.
 *
 *   Reason:
 *   This violation only occurs in a specific testing scenario only where these macros are used for
 *   function trace recoding. Thus this is not contained in production code.
 */

/*==================[inclusions]============================================*/

#include <LdCom_Trace.h>
#include <TSAutosar.h>         /* EB specific standard types */
#include <TSPBConfig_Signature.h> /* Post build signature check */
#include <ComStack_Types.h>
#include <LdCom.h>              /* Module public API */
#include <LdCom_Version.h>      /* Module Version Info */

#if (LDCOM_PBCFGM_SUPPORT_ENABLED == STD_OFF)
/* !LINKSTO LdCom.ASR42.SWS_LDCOM_00001,1 */
#include <LdCom_PBcfg.h>        /* Post-build configuration */
#endif /* (LDCOM_PBCFGM_SUPPORT_ENABLED == STD_OFF) */

/* !LINKSTO LdCom.ASR42.SWS_LDCOM_00001,1 */
#include <LdCom_Lcfg.h>         /* generated link-time configuration */
#include <LdCom_Cbk.h>          /* callback types and functions */
#include <LdCom_DummyCbk.h>     /* dummy-callback functions */
/* !LINKSTO LdCom.ASR42.SWS_LDCOM_00001,1 */
#include <PduR_LdCom.h>
/* !LINKSTO LdCom.ASR42.SWS_LDCOM_00050,1 */
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>               /* Det API */
#endif /* LDCOM_DEV_ERROR_DETECT == STD_ON */

#if (LDCOM_PBCFGM_SUPPORT_ENABLED == STD_ON)
#include <PbcfgM_Api.h>               /* Post build configuration manager */
#endif /* LDCOM_PBCFGM_SUPPORT_ENABLED */

#define TS_RELOCATABLE_CFG_ENABLE LDCOM_RELOCATABLE_CFG_ENABLE
#include <TSPBConfig_Access.h>    /* Post Build Config Helpers (TS_UNCHECKEDGET..) */
#undef TS_RELOCATABLE_CFG_ENABLE

#include <TSMem.h>             /* TS_MemCpy */

/*==================[version check]=========================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef LDCOM_VENDOR_ID /* configuration check */
#error LDCOM_VENDOR_ID must be defined
#endif

#if (LDCOM_VENDOR_ID != 1U) /* vendor check */
#error LDCOM_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef LDCOM_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error LDCOM_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (LDCOM_AR_RELEASE_MAJOR_VERSION != 4U)
#error LDCOM_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef LDCOM_AR_RELEASE_MINOR_VERSION /* configuration check */
#error LDCOM_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (LDCOM_AR_RELEASE_MINOR_VERSION != 0U )
#error LDCOM_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef LDCOM_AR_RELEASE_REVISION_VERSION /* configuration check */
#error LDCOM_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (LDCOM_AR_RELEASE_REVISION_VERSION != 3U )
#error LDCOM_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef LDCOM_SW_MAJOR_VERSION /* configuration check */
#error LDCOM_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (LDCOM_SW_MAJOR_VERSION != 1U)
#error LDCOM_SW_MAJOR_VERSION wrong (!= 1U)
#endif

#ifndef LDCOM_SW_MINOR_VERSION /* configuration check */
#error LDCOM_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (LDCOM_SW_MINOR_VERSION < 0U)
#error LDCOM_SW_MINOR_VERSION wrong (< 0U)
#endif

#ifndef LDCOM_SW_PATCH_VERSION /* configuration check */
#error LDCOM_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (LDCOM_SW_PATCH_VERSION < 26U)
#error LDCOM_SW_PATCH_VERSION wrong (< 26U)
#endif

/*==================[macros]================================================*/

#if (defined LDCOM_DET_REPORT_ERROR) /* to prevent double declaration */
#error LDCOM_DET_REPORT_ERROR already defined
#endif /* if (defined LDCOM_DET_REPORT_ERROR) */

#if (LDCOM_DEV_ERROR_DETECT == STD_ON)

/** \brief Macro for reporting an error to Det
 **
 ** \param[in] SID Service ID of the API function
 ** \param[in] ERROR_CODE Error code reported to Det module
 **/
#define LDCOM_DET_REPORT_ERROR(SID, ERROR_CODE)\
   do{\
         (void)Det_ReportError(\
               (uint16) (LDCOM_MODULE_ID),\
               (uint8) (LDCOM_INSTANCE_ID),\
               (uint8) (SID),\
               (uint8) (ERROR_CODE)\
               );\
   }while(0)

#endif /* if (LDCOM_DEV_ERROR_DETECT == STD_ON) */


#if (defined LDCOM_GET_CONFIG_ADDR) /* to prevent double declaration */
#error LDCOM_GET_CONFIG_ADDR already defined
#endif /* if (defined LDCOM_GET_CONFIG_ADDR) */

/** \brief Define LDCOM_GET_CONFIG_ADDR macro for getting a reference to a config element */
#define LDCOM_GET_CONFIG_ADDR(type, offset) \
TS_UNCHECKEDGETCFG(LdCom_GConfigPtr, type, LDCOM, offset)

#define LDCOM_UNINIT_ACCESS \
((LdCom_State != LDCOM_ONLINE) || (LdCom_GConfigPtr == NULL_PTR))

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#define LDCOM_START_SEC_VAR_INIT_8
#include <LdCom_MemMap.h>

STATIC VAR(LdCom_StateType, LDCOM_VAR) LdCom_State = LDCOM_UNINIT;

#define LDCOM_STOP_SEC_VAR_INIT_8
#include <LdCom_MemMap.h>

#define LDCOM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <LdCom_MemMap.h>

P2CONST(LdCom_ConfigType, LDCOM_VAR_CLEARED, LDCOM_APPL_CONST) LdCom_GConfigPtr;

#define LDCOM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <LdCom_MemMap.h>

/*==================[external function definitions]=========================*/

#define LDCOM_START_SEC_CODE
#include <LdCom_MemMap.h>

#if (LDCOM_VERSION_INFO_API == STD_ON)
FUNC(void, LDCOM_CODE) LdCom_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, LDCOM_APPL_DATA) versioninfo
)
{
  DBG_LDCOM_GETVERSIONINFO_ENTRY(versioninfo);

#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
  if (NULL_PTR == versioninfo)
  {
    LDCOM_DET_REPORT_ERROR(LDCOM_SID_GETVERSIONINFO, LDCOM_E_PARAM_POINTER);
  }
  else
#endif /* LDCOM_DEV_ERROR_DETECT == STD_ON */
  {
    /* assign logistic values to structure members */
    versioninfo->vendorID = (uint16) LDCOM_VENDOR_ID;
    versioninfo->moduleID = (uint16) LDCOM_MODULE_ID;
    versioninfo->sw_major_version = (uint8) LDCOM_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = (uint8) LDCOM_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = (uint8) LDCOM_SW_PATCH_VERSION;
  }

  DBG_LDCOM_GETVERSIONINFO_EXIT(versioninfo);
}
#endif /* LDCOM_VERSION_INFO_API == STD_ON */


FUNC(void, LDCOM_CODE) LdCom_Init
(
  P2CONST(LdCom_ConfigType, AUTOMATIC, LDCOM_APPL_CONST) config
)
{
  P2CONST(LdCom_ConfigType, AUTOMATIC, LDCOM_APPL_CONST) LocalConfigPtr = config;

  DBG_LDCOM_INIT_ENTRY(config);

#if (LDCOM_PBCFGM_SUPPORT_ENABLED == STD_ON)
  /* If the initialization function is called with an null pointer get the configuration from the
   * post build configuration manager */

  if (LocalConfigPtr == NULL_PTR)
  {
    PbcfgM_ModuleConfigPtrType ModuleConfig = NULL_PTR;
    (void) PbcfgM_GetConfig(
            LDCOM_MODULE_ID,
            LDCOM_INSTANCE_ID,
            &ModuleConfig);

    LocalConfigPtr = (P2CONST(LdCom_ConfigType, AUTOMATIC, LDCOM_APPL_CONST)) ModuleConfig;
  }
#endif /* LDCOM_PBCFGM_SUPPORT_ENABLED == STD_ON */

#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
  /* check that configuration pointer is valid and API not called before */
  if (( E_OK != LdCom_IsValidConfig(LocalConfigPtr)) ||
      (LdCom_State == LDCOM_ONLINE))
  {
    LDCOM_DET_REPORT_ERROR(LDCOM_SID_INIT, LDCOM_E_INIT_FAILED);
  }
  else
#endif /* #if (LDCOM_DEV_ERROR_DETECT == STD_ON) */
  {
    LdCom_State = LDCOM_ONLINE;
    LdCom_GConfigPtr = LocalConfigPtr;
  }

  DBG_LDCOM_INIT_EXIT(config);
}


FUNC(void, LDCOM_CODE) LdCom_DeInit(void)
{
  LdCom_State = LDCOM_UNINIT;
}


FUNC(Std_ReturnType, LDCOM_CODE) LdCom_TpTransmit
(
  PduIdType Id,
  P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr
)
{
   Std_ReturnType RetVal = E_NOT_OK;

/* permanently defined API for use in PduR module independent on Rx or Tx transmission */
#if (LDCOM_TPTXAPIS_AVAILABLE == STD_OFF)
   TS_PARAM_UNUSED(Id);
   TS_PARAM_UNUSED(PduInfoPtr);

#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
     /* reported unexpected call to Det in development mode */
     LDCOM_DET_REPORT_ERROR(LDCOM_SID_TPTRANSMIT, LDCOM_E_INVALID_PDU_SDU_ID);
#endif

#else

  /* check if LdCom module is initialized */
  if(LDCOM_UNINIT_ACCESS)
  {
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
     /* reported to Det in development mode */
     LDCOM_DET_REPORT_ERROR(LDCOM_SID_TPTRANSMIT, LDCOM_E_UNINIT);
  }
  /* range check of PDU-ID */
  else if (Id >= LdCom_GConfigPtr->TpTxPathConfig.TxPathCount)
  {
      LDCOM_DET_REPORT_ERROR(LDCOM_SID_TPTRANSMIT, LDCOM_E_INVALID_PDU_SDU_ID);
  }
  /* check if input parameter is not NULL */
  else if (PduInfoPtr == NULL_PTR)
  {
      LDCOM_DET_REPORT_ERROR(LDCOM_SID_TPTRANSMIT, LDCOM_E_PARAM_POINTER);
#endif /* #if (LDCOM_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    CONSTP2CONST(PduIdType, AUTOMATIC, LDCOM_APPL_CONST) PduIdRef =
       LDCOM_GET_CONFIG_ADDR(PduIdType, LdCom_GConfigPtr->TpTxPathConfig.TxPathRef);

    RetVal = PduR_LdComTpTransmit(PduIdRef[Id], PduInfoPtr);
  }
#endif /* (LDCOM_TPTXAPIS_AVAILABLE == STD_ON) */

  return RetVal;
}


FUNC(Std_ReturnType, LDCOM_CODE) LdCom_IfTransmit
(
  PduIdType Id,
  P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

/* permanently defined API for use in PduR module independent on Rx or Tx transmission */
#if (LDCOM_IFTXAPIS_AVAILABLE == STD_OFF)
   TS_PARAM_UNUSED(Id);
   TS_PARAM_UNUSED(PduInfoPtr);

#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
     /* reported unexpected call to Det in development mode */
     LDCOM_DET_REPORT_ERROR(LDCOM_SID_IFTRANSMIT, LDCOM_E_INVALID_PDU_SDU_ID);
#endif

#else

  /* check if LdCom module is initialized */
  if(LDCOM_UNINIT_ACCESS)
  {
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
     /* reported to Det in development mode */
     LDCOM_DET_REPORT_ERROR(LDCOM_SID_IFTRANSMIT, LDCOM_E_UNINIT);
  }
  /* range check of PDU-ID */
  else if (Id >= LdCom_GConfigPtr->IfTxPathConfig.TxPathCount)
  {
      LDCOM_DET_REPORT_ERROR(LDCOM_SID_IFTRANSMIT, LDCOM_E_INVALID_PDU_SDU_ID);
  }
  /* check if input parameter is not NULL */
  else if (PduInfoPtr == NULL_PTR)
  {
      LDCOM_DET_REPORT_ERROR(LDCOM_SID_IFTRANSMIT, LDCOM_E_PARAM_POINTER);
#endif /* #if (LDCOM_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    CONSTP2CONST(PduIdType, AUTOMATIC, LDCOM_APPL_CONST) PduIdRef =
       LDCOM_GET_CONFIG_ADDR(PduIdType, LdCom_GConfigPtr->IfTxPathConfig.TxPathRef);

    RetVal = PduR_LdComTransmit(PduIdRef[Id], PduInfoPtr);
  }
#endif /* (LDCOM_IFTXAPIS_AVAILABLE == STD_ON) */

  return RetVal;
}


/* ------------------------- static callback functions --------------------- */
FUNC(BufReq_ReturnType, LDCOM_CODE) LdCom_CopyTxData
(
  PduIdType Id,
  P2VAR(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) Info,
  P2VAR(RetryInfoType, AUTOMATIC, LDCOM_APPL_DATA) Retry,
  P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) AvailableDataPtr
)
{
  BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;

/* permanently defined API for use in PduR module independent on Rx or Tx transmission */
#if (LDCOM_TPTXAPIS_AVAILABLE == STD_OFF)
   TS_PARAM_UNUSED(Id);
   TS_PARAM_UNUSED(Info);
   TS_PARAM_UNUSED(Retry);
   TS_PARAM_UNUSED(AvailableDataPtr);

#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
     /* reported unexpected call to Det in development mode */
     LDCOM_DET_REPORT_ERROR(LDCOM_SID_COPYTXDATA, LDCOM_E_INVALID_PDU_SDU_ID);
#endif

#else

  /* check if LdCom module is initialized */
  if(LDCOM_UNINIT_ACCESS)
  {
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
     /* reported to Det in development mode */
     LDCOM_DET_REPORT_ERROR(LDCOM_SID_COPYTXDATA, LDCOM_E_UNINIT);
  }
  /* range check of PDU-ID */
  else if (Id >= LdCom_GConfigPtr->TpTxPathConfig.TxPathCount)
  {
      LDCOM_DET_REPORT_ERROR(LDCOM_SID_COPYTXDATA, LDCOM_E_INVALID_PDU_SDU_ID);
  }
  /* check if input parameter is not NULL */
  else if (Info == NULL_PTR)
  {
      LDCOM_DET_REPORT_ERROR(LDCOM_SID_COPYTXDATA, LDCOM_E_PARAM_POINTER);
  }
  /* check if output parameter is not NULL */
  else if (AvailableDataPtr == NULL_PTR)
  {
      LDCOM_DET_REPORT_ERROR(LDCOM_SID_COPYTXDATA, LDCOM_E_PARAM_POINTER);
#endif /* #if (LDCOM_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    /* get target API */
    const LdCom_RteCbkCopyTxDataFpType RteCbkCopyTxDataFp = LdCom_TpTxApi[Id].CopyTxDataFp;

    RetVal = RteCbkCopyTxDataFp(Info, Retry, AvailableDataPtr);
  }
#endif /* (LDCOM_TPTXAPIS_AVAILABLE == STD_ON) */

  return RetVal;
}


FUNC(void, LDCOM_CODE) LdCom_TpTxConfirmation
(
  PduIdType Id,
  NotifResultType Result
)
{
/* permanently defined API for use in PduR module independent on Rx or Tx transmission */
#if (LDCOM_TPTXAPIS_AVAILABLE == STD_OFF)
   TS_PARAM_UNUSED(Id);
   TS_PARAM_UNUSED(Result);

#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
     /* reported unexpected call to Det in development mode */
     LDCOM_DET_REPORT_ERROR(LDCOM_SID_TPTXCONFIRMATION, LDCOM_E_INVALID_PDU_SDU_ID);
#endif

#else

  /* check if LdCom module is initialized */
  if(LDCOM_UNINIT_ACCESS)
  {
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
     /* reported to Det in development mode */
     LDCOM_DET_REPORT_ERROR(LDCOM_SID_TPTXCONFIRMATION, LDCOM_E_UNINIT);
  }
  /* range check of PDU-ID */
  else if (Id >= LdCom_GConfigPtr->TpTxPathConfig.TxPathCount)
  {
      LDCOM_DET_REPORT_ERROR(LDCOM_SID_TPTXCONFIRMATION, LDCOM_E_INVALID_PDU_SDU_ID);
#endif /* #if (LDCOM_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    /* get target API */
    const LdCom_RteCbkTpTxConfirmationFpType RteCbkTpTxConfirmationFp = LdCom_TpTxApi[Id].TpTxConfFp;

    RteCbkTpTxConfirmationFp(Result);
  }
#endif /* (LDCOM_TPTXAPIS_AVAILABLE == STD_ON) */

}


FUNC(BufReq_ReturnType, LDCOM_CODE) LdCom_StartOfReception
(
  PduIdType Id,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) BufferSizePtr
)
{
  BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;

/* permanently defined API for use in PduR module independent on Rx or Tx transmission */
#if (LDCOM_TPRXAPIS_AVAILABLE == STD_OFF)
   TS_PARAM_UNUSED(Id);
   TS_PARAM_UNUSED(TpSduLength);
   TS_PARAM_UNUSED(BufferSizePtr);

#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
     /* reported unexpected call to Det in development mode */
     LDCOM_DET_REPORT_ERROR(LDCOM_SID_STARTOFRECEPTION, LDCOM_E_INVALID_PDU_SDU_ID);
#endif

#else

  /* check if LdCom module is initialized */
  if(LDCOM_UNINIT_ACCESS)
  {
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
     /* reported to Det in development mode */
     LDCOM_DET_REPORT_ERROR(LDCOM_SID_STARTOFRECEPTION, LDCOM_E_UNINIT);
  }
  /* range check of PDU-ID */
  else if (Id >= LdCom_GConfigPtr->TpRxPathCount)
  {
      LDCOM_DET_REPORT_ERROR(LDCOM_SID_STARTOFRECEPTION, LDCOM_E_INVALID_PDU_SDU_ID);
  }
  /* check if output parameter is not NULL */
  else if (BufferSizePtr == NULL_PTR)
  {
      LDCOM_DET_REPORT_ERROR(LDCOM_SID_STARTOFRECEPTION, LDCOM_E_PARAM_POINTER);
#endif /* #if (LDCOM_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    /* get target API */
    const LdCom_RteCbkStartOfReceptionFpType RteCbkStartOfReceptionFp = LdCom_TpRxApi[Id].StartOfRecFp;

    RetVal = RteCbkStartOfReceptionFp(TpSduLength, BufferSizePtr);
  }
#endif /* (LDCOM_TPRXAPIS_AVAILABLE == STD_ON) */

  return RetVal;
}


FUNC(BufReq_ReturnType, LDCOM_CODE)  LdCom_CopyRxData
(
  PduIdType Id,
  P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) Info,
  P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) BufferSizePtr
)
{
  BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;

/* permanently defined API for use in PduR module independent on Rx or Tx transmission */
#if (LDCOM_TPRXAPIS_AVAILABLE == STD_OFF)
   TS_PARAM_UNUSED(Id);
   TS_PARAM_UNUSED(Info);
   TS_PARAM_UNUSED(BufferSizePtr);

#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
     /* reported unexpected call to Det in development mode */
     LDCOM_DET_REPORT_ERROR(LDCOM_SID_COPYRXDATA, LDCOM_E_INVALID_PDU_SDU_ID);
#endif

#else

  /* check if LdCom module is initialized */
  if(LDCOM_UNINIT_ACCESS)
  {
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
     /* reported to Det in development mode */
     LDCOM_DET_REPORT_ERROR(LDCOM_SID_COPYRXDATA, LDCOM_E_UNINIT);
  }
  /* range check of PDU-ID */
  else if (Id >= LdCom_GConfigPtr->TpRxPathCount)
  {
      LDCOM_DET_REPORT_ERROR(LDCOM_SID_COPYRXDATA, LDCOM_E_INVALID_PDU_SDU_ID);
  }
  /* check if input/output parameter is not NULL */
  else if ((Info == NULL_PTR) ||
           (BufferSizePtr == NULL_PTR))
  {
      LDCOM_DET_REPORT_ERROR(LDCOM_SID_COPYRXDATA, LDCOM_E_PARAM_POINTER);
#endif /* #if (LDCOM_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    /* get target API */
    const LdCom_RteCbkCopyRxDataFpType RteCbkCopyRxDataFp = LdCom_TpRxApi[Id].CopyRxDataFp;

    RetVal = RteCbkCopyRxDataFp(Info, BufferSizePtr);
  }
#endif /* (LDCOM_TPRXAPIS_AVAILABLE == STD_ON) */

  return RetVal;
}


FUNC(void, LDCOM_CODE) LdCom_TpRxIndication
(
  PduIdType Id,
  NotifResultType Result
)
{
/* permanently defined API for use in PduR module independent on Rx or Tx transmission */
#if (LDCOM_TPRXAPIS_AVAILABLE == STD_OFF)
   TS_PARAM_UNUSED(Id);
   TS_PARAM_UNUSED(Result);

#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
     /* reported unexpected call to Det in development mode */
     LDCOM_DET_REPORT_ERROR(LDCOM_SID_TPRXINDICATION, LDCOM_E_INVALID_PDU_SDU_ID);
#endif

#else

  /* check if LdCom module is initialized */
  if(LDCOM_UNINIT_ACCESS)
  {
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
     /* reported to Det in development mode */
     LDCOM_DET_REPORT_ERROR(LDCOM_SID_TPRXINDICATION, LDCOM_E_UNINIT);
  }
  /* range check of PDU-ID */
  else if (Id >= LdCom_GConfigPtr->TpRxPathCount)
  {
      LDCOM_DET_REPORT_ERROR(LDCOM_SID_TPRXINDICATION, LDCOM_E_INVALID_PDU_SDU_ID);
#endif /* #if (LDCOM_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    /* get target API */
    const LdCom_RteCbkTpRxIndicationFpType RteCbkTpRxIndicationFp = LdCom_TpRxApi[Id].TpRxIndFp;

    RteCbkTpRxIndicationFp(Result);
  }
#endif /* (LDCOM_TPRXAPIS_AVAILABLE == STD_ON) */
}


FUNC(void, LDCOM_CODE) LdCom_RxIndication
(
  PduIdType RxPduId,
  P2VAR(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr
)
{
/* permanently defined API for use in PduR module independent on Rx or Tx transmission */
#if (LDCOM_IFRXAPIS_AVAILABLE == STD_OFF)
   TS_PARAM_UNUSED(RxPduId);
   TS_PARAM_UNUSED(PduInfoPtr);

#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
     /* reported unexpected call to Det in development mode */
     LDCOM_DET_REPORT_ERROR(LDCOM_SID_RXINDICATION, LDCOM_E_INVALID_PDU_SDU_ID);
#endif

#else

  /* check if LdCom module is initialized */
  if(LDCOM_UNINIT_ACCESS)
  {
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
     /* reported to Det in development mode */
     LDCOM_DET_REPORT_ERROR(LDCOM_SID_RXINDICATION, LDCOM_E_UNINIT);
  }
  /* range check of PDU-ID */
  else if (RxPduId >= LdCom_GConfigPtr->IfRxPathCount)
  {
      LDCOM_DET_REPORT_ERROR(LDCOM_SID_RXINDICATION, LDCOM_E_INVALID_PDU_SDU_ID);
  }
  /* check if input parameter is not NULL */
  else if (PduInfoPtr == NULL_PTR)
  {
      LDCOM_DET_REPORT_ERROR(LDCOM_SID_RXINDICATION, LDCOM_E_PARAM_POINTER);
#endif /* #if (LDCOM_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    /* get target API */
    const LdCom_RteCbkRxIndicationFpType RteCbkRxIndicationFp = LdCom_IfRxApi[RxPduId].RxIndFp;

    RteCbkRxIndicationFp(PduInfoPtr);
  }
#endif /* (LDCOM_IFRXAPIS_AVAILABLE == STD_ON) */
}


FUNC(Std_ReturnType, LDCOM_CODE) LdCom_TriggerTransmit
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr
)
{
   Std_ReturnType RetVal = E_NOT_OK;

#if (LDCOM_TRIGGERTRANSMIT_AVAILABLE == STD_OFF)
   TS_PARAM_UNUSED(TxPduId);
   TS_PARAM_UNUSED(PduInfoPtr);

#if (LDCOM_DEV_ERROR_DETECT == STD_ON) /* might be included if dependence on the if condition below */
     /* reported unexpected call to Det in development mode */
     LDCOM_DET_REPORT_ERROR(LDCOM_SID_TRIGGERTRANSMIT, LDCOM_E_INVALID_PDU_SDU_ID);
#endif

#else

  /* check if LdCom module is initialized */
  if(LDCOM_UNINIT_ACCESS)
  {
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
     /* reported to Det in development mode */
     LDCOM_DET_REPORT_ERROR(LDCOM_SID_TRIGGERTRANSMIT, LDCOM_E_UNINIT);
  }
  /* range check of PDU-ID */
  else if (TxPduId >= LdCom_GConfigPtr->IfTxPathConfig.TxPathCount)
  {
      LDCOM_DET_REPORT_ERROR(LDCOM_SID_TRIGGERTRANSMIT, LDCOM_E_INVALID_PDU_SDU_ID);
  }
  /* check if input parameter is not NULL */
  else if (PduInfoPtr == NULL_PTR)
  {
      LDCOM_DET_REPORT_ERROR(LDCOM_SID_TRIGGERTRANSMIT, LDCOM_E_PARAM_POINTER);
#endif /* #if (LDCOM_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    /* get target API */
    const LdCom_RteCbkTriggerTransmitFpType RteCbkTriggerTransmitFp = LdCom_IfTxApi[TxPduId].TrigTxFp;

    RetVal = RteCbkTriggerTransmitFp(PduInfoPtr);

  }
#endif /* (LDCOM_TRIGGERTRANSMIT_AVAILABLE == STD_ON) */

  return RetVal;
}


FUNC(void, LDCOM_CODE) LdCom_TxConfirmation
(
  PduIdType TxPduId
)
{
#if (LDCOM_TXCONFIRMATION_AVAILABLE == STD_OFF)
  TS_PARAM_UNUSED(TxPduId);

#if (LDCOM_DEV_ERROR_DETECT == STD_ON) /* might be included if dependence on the if condition below */
     /* reported unexpected call to Det in development mode */
     LDCOM_DET_REPORT_ERROR(LDCOM_SID_TXCONFIRMATION, LDCOM_E_INVALID_PDU_SDU_ID);
#endif

#else

  /* check if LdCom module is initialized */
  if(LDCOM_UNINIT_ACCESS)
  {
#if (LDCOM_DEV_ERROR_DETECT == STD_ON)
     /* reported to Det in development mode */
     LDCOM_DET_REPORT_ERROR(LDCOM_SID_TXCONFIRMATION, LDCOM_E_UNINIT);
  }
  /* range check of PDU-ID */
  else if (TxPduId >= LdCom_GConfigPtr->IfTxPathConfig.TxPathCount)
  {
      LDCOM_DET_REPORT_ERROR(LDCOM_SID_TXCONFIRMATION, LDCOM_E_INVALID_PDU_SDU_ID);
#endif /* #if (LDCOM_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    /* get target API */
    const LdCom_RteCbkTxConfirmationFpType RteCbkTxConfirmationFp = LdCom_IfTxApi[TxPduId].TxConfFp;

    RteCbkTxConfirmationFp();

  }
#endif /* #if (LDCOM_TXCONFIRMATION_AVAILABLE == STD_ON) */
}


FUNC(Std_ReturnType, LDCOM_CODE) LdCom_IsValidConfig
(
   P2CONST(void, AUTOMATIC, LDCOM_APPL_CONST) ConfigPtr
)
{
    /* Assume an invalid configuration */
    Std_ReturnType RetVal = E_NOT_OK;

    if (ConfigPtr != NULL_PTR)
    {
        CONSTP2CONST(LdCom_ConfigType, AUTOMATIC, LDCOM_APPL_CONST) PBConfigPtr =
                (P2CONST(LdCom_ConfigType, AUTOMATIC, LDCOM_APPL_CONST)) ConfigPtr;

        /* Check if the configuration fits to the platform */
        if (TS_PlatformSigIsValid(PBConfigPtr->PlatformSignature) == TRUE)
        {
            /* Validate the post build configuration against the compile time configuration */
            if (LDCOM_CFG_SIGNATURE == PBConfigPtr->CfgSignature)
            {
                /* Validate the post build configuration against the link time configuration */
                if (LdCom_LcfgSignature == PBConfigPtr->LcfgSignature)
                {
                   /* Validate the post build configuration against the published information
                      configuration */
                   if (LDCOM_PUBLIC_INFO_SIGNATURE == PBConfigPtr->PublicInfoSignature)
                   {
                      /* Indicate that the configuration is valid */
                      RetVal = E_OK;
                   }
                }
            }
        }
    }

    return RetVal;
}


#if (LDCOM_TPRXAPIS_AVAILABLE == STD_ON)
FUNC(BufReq_ReturnType, LDCOM_CODE) LdCom_DummyCbkStartOfReception
(
  PduLengthType SduLength,
  P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) RxBufferSizePtr
)
{
  BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;

  /* Deviation MISRAC2012-1, MISRAC2012-2 */
  DBG_LDCOM_DUMMYCBKSTARTOFRECEPTION_ENTRY((SduLength),(RxBufferSizePtr));

  TS_PARAM_UNUSED(SduLength);
  TS_PARAM_UNUSED(RxBufferSizePtr);

  DBG_LDCOM_DUMMYCBKSTARTOFRECEPTION_EXIT((SduLength),(RxBufferSizePtr),(RetVal));

  return RetVal;
}
#endif /* (LDCOM_TPRXAPIS_AVAILABLE == STD_ON) */


#if (LDCOM_TPRXAPIS_AVAILABLE == STD_ON)
FUNC(BufReq_ReturnType, LDCOM_CODE)  LdCom_DummyCbkCopyRxData
(
  P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) SduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) RxBufferSizePtr
)
{
  BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;

  /* Deviation MISRAC2012-1, MISRAC2012-2 */
  DBG_LDCOM_DUMMYCBKCOPYRXDATA_ENTRY((SduInfoPtr),(RxBufferSizePtr));

  TS_PARAM_UNUSED(SduInfoPtr);
  TS_PARAM_UNUSED(RxBufferSizePtr);

  DBG_LDCOM_DUMMYCBKCOPYRXDATA_EXIT((SduInfoPtr),(RxBufferSizePtr),(RetVal));

  return RetVal;
}
#endif /* (LDCOM_TPRXAPIS_AVAILABLE == STD_ON) */


#if (LDCOM_TPRXAPIS_AVAILABLE == STD_ON)
FUNC(void, LDCOM_CODE) LdCom_DummyCbkTpRxIndication
(
  NotifResultType Result
)
{
  /* Deviation MISRAC2012-1, MISRAC2012-2 */
  DBG_LDCOM_DUMMYCBKTPRXINDICATION_ENTRY((Result));

  TS_PARAM_UNUSED(Result);

  DBG_LDCOM_DUMMYCBKTPRXINDICATION_EXIT((Result));
}
#endif /* (LDCOM_TPRXAPIS_AVAILABLE == STD_ON) */


#if (LDCOM_TPTXAPIS_AVAILABLE == STD_ON)
FUNC(BufReq_ReturnType, LDCOM_CODE) LdCom_DummyCbkCopyTxData
(
  P2VAR(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) SduInfoPtr,
  P2VAR(RetryInfoType, AUTOMATIC, LDCOM_APPL_DATA) RetryInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, LDCOM_APPL_DATA) TxDataCntPtr
)
{
  BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;

  /* Deviation MISRAC2012-1, MISRAC2012-2 */
  DBG_LDCOM_DUMMYCBKCOPYTXDATA_ENTRY((SduInfoPtr),(RetryInfoPtr),(TxDataCntPtr));

  TS_PARAM_UNUSED(SduInfoPtr);
  TS_PARAM_UNUSED(RetryInfoPtr);
  TS_PARAM_UNUSED(TxDataCntPtr);

  DBG_LDCOM_DUMMYCBKCOPYTXDATA_EXIT((SduInfoPtr),(RetryInfoPtr),(TxDataCntPtr),(RetVal));

  return RetVal;
}
#endif /* (LDCOM_TPTXAPIS_AVAILABLE == STD_ON) */


#if (LDCOM_TPTXAPIS_AVAILABLE == STD_ON)
FUNC(void, LDCOM_CODE) LdCom_DummyCbkTpTxConfirmation
(
  NotifResultType Result
)
{
  /* Deviation MISRAC2012-1, MISRAC2012-2 */
  DBG_LDCOM_DUMMYCBKTPTXCONFIRMATION_ENTRY((Result));

  TS_PARAM_UNUSED(Result);

  DBG_LDCOM_DUMMYCBKTPTXCONFIRMATION_EXIT((Result));
}
#endif /* (LDCOM_TPTXAPIS_AVAILABLE == STD_ON) */


#if (LDCOM_IFRXAPIS_AVAILABLE == STD_ON)
FUNC(void, LDCOM_CODE) LdCom_DummyCbkRxIndication
(
  P2CONST(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr
)
{
  /* Deviation MISRAC2012-1, MISRAC2012-2 */
  DBG_LDCOM_DUMMYCBKRXINDICATION_ENTRY((PduInfoPtr));

  TS_PARAM_UNUSED(PduInfoPtr);

  DBG_LDCOM_DUMMYCBKRXINDICATION_EXIT((PduInfoPtr));
}
#endif /* (LDCOM_IFRXAPIS_AVAILABLE == STD_ON) */


FUNC(Std_ReturnType, LDCOM_CODE) LdCom_DummyCbkTriggerTransmit
(
  P2VAR(PduInfoType, AUTOMATIC, LDCOM_APPL_DATA) PduInfoPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  /* Deviation MISRAC2012-1, MISRAC2012-2 */
  DBG_LDCOM_DUMMYCBKTRIGGERTRANSMIT_ENTRY((PduInfoPtr));

  TS_PARAM_UNUSED(PduInfoPtr);

  DBG_LDCOM_DUMMYCBKTRIGGERTRANSMIT_EXIT((PduInfoPtr),(RetVal));

  return RetVal;
}


FUNC(void, LDCOM_CODE) LdCom_DummyCbkTxConfirmation
(
  void
)
{
  /* Deviation MISRAC2012-1 */
  DBG_LDCOM_DUMMYCBKTXCONFIRMATION_ENTRY();
  DBG_LDCOM_DUMMYCBKTXCONFIRMATION_EXIT();
}


#define LDCOM_STOP_SEC_CODE
#include <LdCom_MemMap.h>

/*==================[end of file]===========================================*/
