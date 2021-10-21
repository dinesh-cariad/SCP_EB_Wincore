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

#ifndef DCM_DSP_SVC_REQUESTVEHICLEINFORMATION_H
#define DCM_DSP_SVC_REQUESTVEHICLEINFORMATION_H

/* !LINKSTO Dcm.dsn.File.ReqVehInfo.PublicApi,1 */
/* This file contains the public API of the implementation of the
 * Diagnostic Service RequestVehicleInformation. */

/*==================[inclusions]=================================================================*/

#include <Std_Types.h>         /* AUTOSAR standard types */
#include <Dcm_Types.h>         /* DCM type definition */
#include <Dcm_RequestVehicleInformation_Cfg.h>
/*==================[macros]=====================================================================*/

#if (DCM_DSP_VEHINFO_USED_ARRAY_SIZE > 0)
#if (defined DCM_IS_VEHINFOUSED_IDX_SET)
#error DCM_IS_VEHINFOUSED_IDX_SET already defined
#endif
#define DCM_IS_VEHINFOUSED_IDX_SET(idx) DCM_UINT8_IS_BIT_SET(Dcm_DspVehInfoUsed[(idx)/8U], (idx)%8U)
#endif

#if (defined DCM_OBD_MIN_REQ_VEHINFO_SERVICE_09)
#error DCM_OBD_MIN_REQ_VEHINFO_SERVICE_09 already defined
#endif /* if (defined DCM_OBD_MIN_REQ_VEHINFO_SERVICE_09) */

/** \brief Minimun number of requested Vehicle Info in a Service 09 request */
#define DCM_OBD_MIN_REQ_VEHINFO_SERVICE_09 (1U)

#if (defined DCM_OBD_MAX_REQ_VEHINFO_NONAVAIL_SERVICE_09)
#error DCM_OBD_MAX_REQ_VEHINFO_NONAVAIL_SERVICE_09 already defined
#endif /* if (defined DCM_OBD_MAX_REQ_VEHINFO_NONAVAIL_SERVICE_09) */

/** \brief Maximum number of requested non availability Vehicle Info
 *         in a Service 09 request */
#define DCM_OBD_MAX_REQ_VEHINFO_NONAVAIL_SERVICE_09 1U

#if (defined DCM_OBD_MAX_REQ_VEHINFO_AVAIL_SERVICE_09)
#error DCM_OBD_MAX_REQ_VEHINFO_AVAIL_SERVICE_09 already defined
#endif /* if (defined DCM_OBD_MAX_REQ_VEHINFO_AVAIL_SERVICE_09) */

/** \brief Maximum number of requested availability Vehicle Info
 *         in a Service 09 request */
#define DCM_OBD_MAX_REQ_VEHINFO_AVAIL_SERVICE_09 6U

/*==================[type definitions]===========================================================*/

/* !LINKSTO Dcm.Dsn.RequestVehicleInformation.RuntimeDataTypes,1 */
/* !LINKSTO Dcm.Dsn.ReqVehInfo.Type.DcmDspReqVehInfoContext,1 */
/** \brief Structure to store the current service execution context */
typedef struct{
  /** \brief Pointer to buffer configuration */
  P2VAR(Dcm_DspMsgBufferCfgType, TYPEDEF, DCM_VAR) BufferCfgPtr;
  /** \brief Index of the current requested Vehicle Info in global configuration */
  uint8 currentVehInfoIdx;
  /** \brief Index of the current VehInfoData in the DcmDspVehInfoData[]
   *  relative to first VehInfoData belongs to requested Vehicle Info */
  uint8 currentVehInfoDataIdx;
  /** \brief Response code for current request */
  Dcm_NegativeResponseCodeType nrc;
}Dcm_DspReqVehInfoContextType;

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

#if(DCM_DSP_USE_SERVICE_0X09 == STD_ON)

/** \brief The generic service handler for OBD service 0x09
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again

 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_RequestVehicleInformation_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

extern FUNC(Std_ReturnType, DCM_CODE) Dcm_RequestVehicleInformation_HelperExecuteAvailableInfoType(
    Dcm_DspVehInfoInfoTypeType VehInfoInfoType,
    P2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) svcBufferContext);

extern FUNC(Std_ReturnType, DCM_CODE)
  Dcm_RequestVehicleInformation_HelperExecuteNonAvailableInfoType(
    Dcm_OpStatusType OpStatus,
    P2VAR(Dcm_DspReqVehInfoContextType, AUTOMATIC, DCM_VAR) svcContext);

#endif /* #if (DCM_DSP_USE_SERVICE_0X09 == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[internal data]==============================================================*/

/*==================[external data]==============================================================*/
#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#if (DCM_DSP_USE_SERVICE_0X09 == STD_ON)
extern VAR(Dcm_DspReqVehInfoContextType, DCM_VAR) Dcm_DspReqVehInfoContext;
#endif

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>
/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_DSP_SVC_REQUESTVEHICLEINFORMATION_H */
/*==================[end of file]================================================================*/
