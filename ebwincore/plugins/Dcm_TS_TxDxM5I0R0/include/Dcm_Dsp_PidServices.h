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

#ifndef DCM_DSP_PIDSERVICES_H
#define DCM_DSP_PIDSERVICES_H

/* This file contains the public API of the DID services software unit. */

/*==================[inclusions]=================================================================*/

#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <Dcm_Int.h>

/*==================[macros]=====================================================================*/

#if (defined DCM_SERVICE_01)
#error DCM_SERVICE_01 already defined
#endif/* #if (defined DCM_SERVICE_02) */

/** \brief General macro to identify Dcm Service 01  */
#define DCM_SERVICE_01 0x01U

#if (defined DCM_SERVICE_02)
#error DCM_SERVICE_02 already defined
#endif/* #if (defined DCM_SERVICE_02) */

/** \brief General macro to identify Dcm Service 02  */
#define DCM_SERVICE_02 0x02U

#if (defined DCM_DSP_SERVICE01_MASK)
#error DCM_DSP_SERVICE01_MASK already defined
#endif/* #if (defined DCM_DSP_SERVICE01_MASK) */

/* !LINKSTO Dcm.dsn.Dsp.Pid.BitField.config.0002,2 */
/** \brief Mask to use on PidConfig Bitfield to get the value of bit 0 */
#define DCM_DSP_SERVICE01_MASK 0x01U

#if (defined DCM_DSP_SERVICE02_MASK)
#error DCM_DSP_SERVICE02_MASK already defined
#endif/* #if (defined DCM_DSP_SERVICE02_MASK) */

/* !LINKSTO Dcm.dsn.Dsp.Pid.BitField.config.0003,2 */
/** \brief Mask to use on PidConfig Bitfield to get the value of bit 1 */
#define DCM_DSP_SERVICE02_MASK 0x02U

#if (defined DCM_OBD_MAX_REQ_PID_SERVICE_01)
#error DCM_OBD_MAX_REQ_PID_SERVICE_01 already defined
#endif /* if (defined DCM_OBD_MAX_REQ_PID_SERVICE_01) */

/** \brief OBD Maximum number of PIDs in a Service 02 request */
#define DCM_OBD_MAX_REQ_PID_SERVICE_01 (6U)

#if (defined DCM_OBD_MAX_REQ_PID_SERVICE_02)
#error DCM_OBD_MAX_REQ_PID_SERVICE_02 already defined
#endif /* if (defined DCM_OBD_MAX_REQ_PID_SERVICE_02) */

/** \brief OBD Maximum number of PIDs in a Service 02 request */
#define DCM_OBD_MAX_REQ_PID_SERVICE_02 (3U)

/*==================[type definitions]===========================================================*/

/** \brief Type definition for DcmDspPidDataPos */
/* !LINKSTO Dcm.dsn.Dsp.PidData.config.0002,2 */
typedef uint16 Dcm_DspPidDataPosType;

/** \brief Type definition for DcmDspPidDataSize */
/* !LINKSTO Dcm.dsn.Dsp.PidData.config.003,2 */
typedef uint16 Dcm_DspPidDataSizeType;

/** \brief Type definition for Service Identifier */
typedef uint8 Dcm_ServiceIdType;

/** \brief Type of function for reading the PID using sender-receiver, user function and
  *  synch interface */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_PidReadDataFncTypeSync) (
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Data);

/** \brief Structure of all DcmDspPidDataConfig elements configured in Dcm*/
typedef struct
{
  /* !LINKSTO Dcm.dsn.Dsp.PidData.config.004,2 */
  /** \brief Pointer to ReadData function of the Pid */
  Dcm_PidReadDataFncTypeSync Service0x01ReadFnc;

  /* !LINKSTO Dcm.dsn.Dsp.PidData.config.0002,2 */
  /** \brief Position in bit of the data element inside the PID */
  Dcm_DspPidDataPosType DataPos;

  /* !LINKSTO Dcm.dsn.Dsp.PidData.config.003,2 */
  /** \brief Length of the data element in bit */
  Dcm_DspPidDataSizeType DataSize;
}
Dcm_DspPidDataConfigType;

/** \brief Type definition for DcmDspPidSize */
/* !LINKSTO Dcm.dsn.Dsp.Pid.config.0003,1 */
typedef uint8 Dcm_DspPidSizeType;

/** \brief Type definition for an element position in Dcm_DspPidDataConfig[] */
/* !LINKSTO Dcm.dsn.Dsp.Pid.config.0004,2 */
typedef uint32 Dcm_DspPidDataNumOfDataType;

/** \brief Type definition for the number of elements
     in Dcm_DspPidDataConfig[] for a DcmDspPid */
/* !LINKSTO Dcm.dsn.Dsp.Pid.config.0005,2 */
typedef uint16 Dcm_DspPidNumOfDataType;

/** \brief Type definition for an element position in Dcm_DspPidSupInfoConfig[] */
/* !LINKSTO Dcm.dsn.Dsp.Pid.config.0006,1 */
typedef uint16 Dcm_DspPidSupInfoNoOfSupInfoType;

/** \brief Type definition for the number of elements
     in Dcm_DspPidSupInfoConfig[] for a DcmDspPid */
/* !LINKSTO Dcm.dsn.Dsp.Pid.config.0007,1 */
typedef uint16 Dcm_DspPidNumOfSupInfoType;

typedef uint8 Dcm_DspPidBitFieldType;

/** \brief Configuration structure for one DcmDspPid container*/
typedef struct
{
  /* !LINKSTO Dcm.dsn.Dsp.Pid.config.0004,2 */
  /** \brief First element in Dcm_DspPidDataConfig[] for this element */
  Dcm_DspPidDataNumOfDataType DataStartIndex;

  /* !LINKSTO Dcm.dsn.Dsp.Pid.config.0005,2 */
  /** \brief The number of elements in Dcm_DspPidDataConfig[] for this DcmDspPid */
  Dcm_DspPidNumOfDataType NumOfData;

  /* !LINKSTO Dcm.dsn.Dsp.Pid.config.0006,1 */
  /** \brief First element in Dcm_DspPidSupInfoConfig[] for this element */
  Dcm_DspPidSupInfoNoOfSupInfoType SupInfoStartIndex;

  /* !LINKSTO Dcm.dsn.Dsp.Pid.config.0007,1 */
  /** \brief The number of elements in Dcm_DspPidSupInfoConfig[] for this DcmDspPid  */
  Dcm_DspPidNumOfSupInfoType NumOfSupInfo;

  /* !LINKSTO Dcm.dsn.Dsp.Pid.config.0002,2 */
  /** \brief The value of PidIdentifier */
  Dcm_DspPidIdentifierType PidIdentifier;

  /* !LINKSTO Dcm.dsn.Dsp.Pid.config.0003,1 */
  /** \brief Size of the Pid */
  Dcm_DspPidSizeType PidSize;

   /* !LINKSTO Dcm.dsn.Dsp.Pid.config.001,1 */
  /** \brief Hold Boolean values for this Pid (Services 01 and 02 support) */
  Dcm_DspPidBitFieldType BitField;
}
Dcm_DspPidConfigType;

/** \brief Type definition for an element position in Dcm_DspPidSupInfoData[] */
/* !LINKSTO Dcm.dsn.Dsp.PidSupInfo.config.0002,1 */
typedef uint32 Dcm_DspPidSupInfoNumOfDataType;

/** \brief Type definition for DcmDspPidSupportInfoLen */
/* !LINKSTO Dcm.dsn.Dsp.PidSupInfo.config.0003,1 */
typedef uint8 Dcm_DspSupInfoLenType;

/** \brief Type definition for DcmDspPidSupportInfoPos */
/* !LINKSTO Dcm.dsn.Dsp.PidSupInfo.config.0004,1 */
typedef uint8 Dcm_DspSupInfoPosType;

/** \brief Configuration structure for  all support information DcmDspPidSupportInfo*/
typedef struct
{
  /* !LINKSTO Dcm.dsn.Dsp.PidSupInfo.config.0002,1 */
  /** \brief First element in Dcm_DspPidSupInfoData[] for this element */
  Dcm_DspPidSupInfoNumOfDataType SupInfoDataStartIndex;

  /* !LINKSTO Dcm.dsn.Dsp.PidSupInfo.config.0003,1 */
  /** \brief Length of the supported information */
  Dcm_DspSupInfoLenType SupInfoLen;

  /* !LINKSTO Dcm.dsn.Dsp.PidSupInfo.config.0004,1 */
  /** \brief Position inside the Pid service response */
  Dcm_DspSupInfoPosType SupInfoPos;
}
Dcm_DspPidSupInfoConfigType;

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

#if(DCM_DSP_USE_SERVICE_0X01 == STD_ON)

/** \brief The generic service handler for OBD service 0x01
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP  The DCM shall send an "response pending" immediately
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */

extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestCurrentPowertrainDiagnosticData_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#endif /*(DCM_DSP_USE_SERVICE_0X01 == STD_ON) */

#if(DCM_DSP_USE_SERVICE_0X02 == STD_ON)

/** \brief The generic service handler for OBD service 0x02
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP  The DCM shall send an "response pending" immediately
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */

extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_RequestPowertrainFreezeFrameData_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#endif /*(DCM_DSP_USE_SERVICE_0X02 == STD_ON)*/

#if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || (DCM_NUM_PID_CFG > 0U) )
/** \brief Interface used by Service 0x22 to read PID Data
 **
 ** \param[in] Pid Pid Identifier
 ** \param[out] Nrc Negative Response Code
 ** \param[inout] BufferCfg Pointer to data Buffer configuration
 **
 ** \retval E_OK                Operation Success
 ** \retval E_NOT_OK            Operation Failed
 */

extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_PidServices_Execute(
  Dcm_GenericUint8IdType PidId,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc,
  P2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) BufferCfg);

#endif /* #if ( (DCM_DSP_USE_SERVICE_0X01 == STD_ON) || (DCM_NUM_PID_CFG > 0U) )*/

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

#define DCM_START_SEC_CONST_8
#include <Dcm_MemMap.h>

#if (DCM_DSP_PID_SUPINFO_DATA_NUM > 0)
/** \brief Array of Support Info */
extern CONST(uint8,DCM_CONST) Dcm_DspPidSupInfoData[DCM_DSP_PID_SUPINFO_DATA_NUM];
#endif

#define DCM_STOP_SEC_CONST_8
#include <Dcm_MemMap.h>

#if (DCM_DSP_PID_USED_BUF_SIZE > 0)
#define DCM_START_SEC_CALIB_8
#include <Dcm_MemMap.h>

/** \brief Contains one bit per configured DcmDspPid (true if DcmDspPidUsed is set to true) */
extern CONST(uint8,DCM_CONST) Dcm_DspPidUsed[DCM_DSP_PID_USED_BUF_SIZE];

#define DCM_STOP_SEC_CALIB_8
#include <Dcm_MemMap.h>
#endif

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

#if (DCM_DSP_PID_DATA_NUM > 0)
/** \brief List of elements of DcmDspPidDataConfig */
extern CONST(Dcm_DspPidDataConfigType,DCM_CONST) Dcm_DspPidDataConfig[DCM_DSP_PID_DATA_NUM];
#endif

#if (DCM_DSP_PID_SUPINFO_CONFIG_NUM > 0)
/** \brief List of all support information DcmDspPidSupportInfo
 ** ordered by DcmDspPidSupportInfoPos */
extern CONST(Dcm_DspPidSupInfoConfigType,DCM_CONST)
    Dcm_DspPidSupInfoConfig[DCM_DSP_PID_SUPINFO_CONFIG_NUM];
#endif

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

#if (DCM_NUM_PID_CFG > 0)
#define DCM_START_SEC_CALIB_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief List with Pids configurations */
extern CONST(Dcm_DspPidConfigType,DCM_CONST) Dcm_DspPidConfig[DCM_NUM_PID_CFG];

#define DCM_STOP_SEC_CALIB_UNSPECIFIED
#include <Dcm_MemMap.h>
#endif

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_DSP_PIDSERVICES_H */
/*==================[end of file]================================================================*/
