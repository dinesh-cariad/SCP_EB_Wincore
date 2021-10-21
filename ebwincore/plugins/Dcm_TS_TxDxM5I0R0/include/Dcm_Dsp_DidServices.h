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

#ifndef DCM_DSP_DIDSERVICES_H
#define DCM_DSP_DIDSERVICES_H

/* !LINKSTO Dcm.Dsn.File.DidServices.PublicApi,1 */
/* This file contains the public API of the DID services software unit. */

/*==================[inclusions]=================================================================*/

#include <Dcm_Int.h>

/* provides generic functions for asynchronous service processing */
#include <Dcm_Dsp_Contexts.h>
#include <Dcm_Dsp_DidSignals.h>
#include <Dcm_RequestVehicleInformation_Cfg.h>

/* legacy support, needed by ClearDDDid */
#if(DCM_DSP_USE_SERVICE_0X2A == STD_ON)
#include <Dcm_Dsp_SvcH_ReadDataByPeriodicIdentifier.h>
#endif

#if (DCM_DSP_USE_SERVICE_0X2C == STD_ON)
/* provides type definitions needed for reading memory via dynamically defined Dids */
#include <Dcm_Dsp_MemoryServices.h>
#endif

#if (DCM_DSP_VEHINFO_NUM > 0)
#include <Dcm_Dsp_SvcH_RequestVehicleInformation.h>
#endif

/*==================[macros]=====================================================================*/

#if (defined DCM_DID_DMC_LOW_LIMIT)
#error DCM_DID_DMC_LOW_LIMIT already defined
#endif /* if (defined DCM_DID_DMC_LOW_LIMIT) */
/** \brief Dynamic Did lower limit */
#define DCM_DID_DMC_LOW_LIMIT (0xF200U)

#if (defined DCM_DID_DMC_UP_LIMIT)
#error DCM_DID_DMC_UP_LIMIT already defined
#endif /* if (defined DCM_DID_DMC_UP_LIMIT) */
/** \brief Dynamic Did upper limit */
#define DCM_DID_DMC_UP_LIMIT (0xF3FFU)

#if (DCM_DSP_USE_DIDSERVICES == STD_ON)

#if (defined DCM_DSP_DIDSERVICES_USE_OBDSERVICES)
  #error "DCM_DSP_DIDSERVICES_USE_OBDSERVICES is already defined"
#endif
/* Check if a OBD service relevant for Did reading is available */
#if ( (DCM_NUM_PID_CFG > 0U) ||                  \
      (DCM_DSP_USE_SERVICE_0X01 == STD_ON) ||    \
      (DCM_DSP_USE_SERVICE_0X06 == STD_ON) ||    \
      (DCM_DSP_VEHINFO_NUM > 0) )
/** \brief Enables the use of OBD services for Did reading */
#define DCM_DSP_DIDSERVICES_USE_OBDSERVICES STD_ON
#else
/** \brief Disables the use of OBD services for Did reading */
#define DCM_DSP_DIDSERVICES_USE_OBDSERVICES STD_OFF
#endif

/** \brief Enable or disable support for dynamic Dids.
 **        This feature is directly related to the availability service 0x2C. */
#if (defined DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT)
  #error "DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT is already defined"
#endif
#define DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT DCM_DSP_USE_SERVICE_0X2C

/* !LINKSTO Dcm.Dsn.Type.Dcm_DidType,1 */
/** \brief Configuration does not contain entries for requested Did */
#if (defined DCM_DID_UNCONFIGURED)
  #error "DCM_DID_UNCONFIGURED is already defined"
#endif
#define DCM_DID_UNCONFIGURED       ((uint8)0U)

/** \brief Did is statically defined by Id (DspDspDid) */
#if (defined DCM_DID_TYPE_ID_STATIC)
  #error "DCM_DID_TYPE_ID_STATIC is already defined"
#endif
#define DCM_DID_TYPE_ID_STATIC     ((uint8)1U)

/** \brief Did is dynamically defined */
#if (defined DCM_DID_TYPE_ID_DYNAMIC)
  #error "DCM_DID_TYPE_ID_DYNAMIC is already defined"
#endif
#define DCM_DID_TYPE_ID_DYNAMIC    ((uint8)2U)

/** \brief Did is statically defined by Range (DspDspDidRange without gaps) */
#if (defined DCM_DID_TYPE_RANGE_NO_GAPS)
  #error "DCM_DID_TYPE_RANGE_NO_GAPS is already defined"
#endif
#define DCM_DID_TYPE_RANGE_NO_GAPS ((uint8)3U)

/** \brief Did is dynamically defined by Range (DspDspDidRange with gaps) */
#if (defined DCM_DID_TYPE_RANGE_GAPS)
  #error "DCM_DID_TYPE_RANGE_GAPS is already defined"
#endif
#define DCM_DID_TYPE_RANGE_GAPS    ((uint8)4U)

/** \brief Did is an OBD PID */
#if (defined DCM_DID_TYPE_OBD_PID)
  #error "DCM_DID_TYPE_OBD_PID is already defined"
#endif
#define DCM_DID_TYPE_OBD_PID       ((uint8)5U)

/** \brief Did is an OBD info type */
#if (defined DCM_DID_TYPE_OBD_INFO)
  #error "DCM_DID_TYPE_OBD_INFO is already defined"
#endif
#define DCM_DID_TYPE_OBD_INFO      ((uint8)6U)

/** \brief Did is an OBD monitor Id */
#if (defined DCM_DID_TYPE_OBD_MID)
  #error "DCM_DID_TYPE_OBD_MID is already defined"
#endif
#define DCM_DID_TYPE_OBD_MID       ((uint8)7U)

#if (defined DCM_DID_UNCONFIGURED_DEFAULT_INTERFACE)
/** \brief Did is unconfigured and Default Read is active. */
  #error "DCM_DID_UNCONFIGURED_DEFAULT_INTERFACE is already defined"
#endif /* (defined DCM_DID_UNCONFIGURED_DEFAULT_INTERFACE) */
#define DCM_DID_UNCONFIGURED_DEFAULT_INTERFACE ((uint8)8U)

/** \brief Size of one DID identifier in bytes */
#if (defined DCM_DID_ID_SIZE)
  #error "DCM_DID_ID_SIZE is already defined"
#endif
#define DCM_DID_ID_SIZE ((uint8) 2U)

/** \brief Flag to a service function to add the Did id(s) to the response */
#if (defined DCM_WRITE_DID_IDS)
  #error "DCM_WRITE_DID_IDS is already defined"
#endif
#define  DCM_WRITE_DID_IDS  (TRUE)

/** \brief Flag to a service function to omit the Did id(s) to the response */
#if (defined DCM_DO_NOT_WRITE_DID_IDS)
  #error "DCM_DO_NOT_WRITE_DID_IDS is already defined"
#endif
#define  DCM_DO_NOT_WRITE_DID_IDS (FALSE)

/** \brief Invalid marker for an index to the periodic scheduler table **/
#if (defined DCM_PDID_SCHEDTABLE_INVALID_IDX)
  #error "DCM_PDID_SCHEDTABLE_INVALID_IDX is already defined"
#endif
#define DCM_PDID_SCHEDTABLE_INVALID_IDX DCM_MAX_PERIODIC_DID_SCHEDULER

/** \brief Invalid marker for an index to the transmission ring **/
#if (defined DCM_PDID_TRANSRING_INVALID_IDX)
  #error "DCM_PDID_TRANSRING_INVALID_IDX is already defined"
#endif
#define DCM_PDID_TRANSRING_INVALID_IDX DCM_PERIODIC_DID_TRANSMISSION_RING_ENTRIES

/* Check if the lock mechanism for DidServices unit is needed */
#if ((DCM_DSP_USE_SERVICE_0X2F == STD_ON) ||                 \
     ((DCM_HAS_ROE_TYPE2 == STD_ON) &&                       \
      (DCM_ROE_NUM_ONCHANGEOFDATAIDENTIFIER_EVENTS > 0U)) || \
     ((DCM_DSP_USE_SERVICE_0X2A == STD_ON) &&                \
      ((DCM_DSP_SERVICE_0X22_ASYNC == STD_ON) ||             \
       (DCM_DSP_SERVICE_0X2F_ASYNC == STD_ON) ||             \
       (DCM_DSP_SERVICE_0X2E_ASYNC == STD_ON)))              \
    )
/** \brief Enables the use of lock mechanism for DidServices unit */
#define DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED STD_ON

/** \brief Macros for LockStatus used in InputOutputControlByIdentifier and WriteDataByIdentifier
 **        services.
 */
#define DCM_TRYING_LOCK                            (0x00U)
#define DCM_EXECUTING_SERVICE                      (0x01U)

#else
/** \brief Disables the use of lock mechanism for DidServices unit */
#define DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED STD_OFF
#endif

/*==================[type definitions]===========================================================*/

/* ========== Legacy data structures related to signals - START ========== */

/** \brief This type describes the possible return values when for the function that checks the
 **        existance and readability of the periodical data identifiers */

/** \brief Check successfully completed. The periodic DID exists and can be read */
#if (defined DCM_CHECK_PDID_OK)
  #error "DCM_CHECK_PDID_OK is already defined"
#endif
#define DCM_CHECK_PDID_OK                  0U

/** \brief Check failed. The periodic DID exists but cannot be read in the current security level
 *         or mode condition */
#if (defined DCM_CHECK_PDID_NOT_OK)
  #error "DCM_CHECK_PDID_NOT_OK is already defined"
#endif
#define DCM_CHECK_PDID_NOT_OK              1U

/** \brief Check failed.  The periodic DID does not exist */
#if (defined DCM_CHECK_PDID_DOES_NOT_EXIST)
  #error "DCM_CHECK_PDID_DOES_NOT_EXIST is already defined"
#endif
#define DCM_CHECK_PDID_DOES_NOT_EXIST      2U

/** \brief Check failed. The periodic DID exists but cannot be read in the current diag. session */
#if (defined DCM_CHECK_PDID_UNSUPPORTED_SESSION)
  #error "DCM_CHECK_PDID_UNSUPPORTED_SESSION is already defined"
#endif
#define DCM_CHECK_PDID_UNSUPPORTED_SESSION 3U

typedef uint8 Dcm_CheckPeriodicDIDType;

/** \brief  This type contains the context information for executing a reading of Did signals */
typedef struct
{
  Dcm_DidSignalReadContextType SignalReadContext;
  /** \brief Maximum number of bytes that may be written in the buffer (for error checking) */
  uint32 BufferLimit;
  /** \brief Number of bytes in the buffer already used */
  uint32 AmountOfBufferOccupied;
  /** \brief Pointer to the output buffer where the DID data shall be written */
  P2VAR(uint8, TYPEDEF, DCM_VAR) outputBuffer;
  /** \brief Index of DID Signal currently being processed */
  uint16 SignalInd;
  /** \brief Index of the DID configuration of the DID currently processed */
  Dcm_DidConfigIdxType CurrentDidCfgIndex;
  /** \brief Parameter which transmits special Operation Status */
  Dcm_OpStatusType CurrentOpStatus;
  /** \brief Holds the global NRC for the currently processed request */
  Dcm_NegativeResponseCodeType NrcConditions;
  /** \brief Flag whether endianness conversion shall be performed for signals or not */
  boolean EndiannessConversion;
  Dcm_DidOpType DidOp;
  boolean NewSignal;
}
Dcm_ReadSignalContextType;


/* ========== Legacy data structures related to signals - END   ========== */

/* !LINKSTO Dcm.Dsn.Type.Dcm_DidInfoIdxType,1 */
/** \brief Type of an index into a DidInfo array */
#if (DCM_DSP_DIDSERVICES_NUM_DID_INFO > 0xFFU)
typedef uint16 Dcm_DidInfoIdxType;
#else
typedef uint8 Dcm_DidInfoIdxType;
#endif

#if (defined DCM_DIDREADACCESSINFOIDX_INVALID)
  #error "DCM_DIDREADACCESSINFOIDX_INVALID is already defined"
#endif
/* !LINKSTO Dcm.Dsn.Type.Dcm_DidReadAccessInfoIdxType,1 */
/** \brief Type of an index into a DcmDspDidInfo array */
#if (DCM_DSP_DIDSERVICES_NUM_DID_READACCESS_INFO > 0xFEU)
typedef uint16 Dcm_DidReadAccessInfoIdxType;
#define DCM_DIDREADACCESSINFOIDX_INVALID 0xFFFFU
#else
typedef uint8 Dcm_DidReadAccessInfoIdxType;
#define DCM_DIDREADACCESSINFOIDX_INVALID 0xFFU
#endif

#if (defined DCM_DIDWRITEACCESSINFOIDX_INVALID)
  #error "DCM_DIDWRITEACCESSINFOIDX_INVALID is already defined"
#endif
/* !LINKSTO Dcm.Dsn.Type.Dcm_DidWriteAccessInfoIdxType,1 */
/** \brief Type of an index into a DcmDspDidInfo array */
#if (DCM_DSP_DIDSERVICES_NUM_DID_WRITEACCESS_INFO > 0xFEU)
typedef uint16 Dcm_DidWriteAccessInfoIdxType;
#define DCM_DIDWRITEACCESSINFOIDX_INVALID 0xFFFFU
#else
typedef uint8 Dcm_DidWriteAccessInfoIdxType;
#define DCM_DIDWRITEACCESSINFOIDX_INVALID 0xFFU
#endif

#if (defined DCM_DIDIOCTLACCESSINFOIDX_INVALID)
  #error "DCM_DIDIOCTLACCESSINFOIDX_INVALID is already defined"
#endif
/* !LINKSTO Dcm.Dsn.Type.Dcm_DidIoctlAccessInfoIdxType,1 */
/** \brief Type of an index into a DcmDspDidInfo array */
#if (DCM_DSP_DIDSERVICES_NUM_DID_IOCTLACCESS_INFO > 0xFEU)
typedef uint16 Dcm_DidIoctlAccessInfoIdxType;
#define DCM_DIDIOCTLACCESSINFOIDX_INVALID 0xFFFFU
#else
typedef uint8 Dcm_DidIoctlAccessInfoIdxType;
#define DCM_DIDIOCTLACCESSINFOIDX_INVALID 0xFFU
#endif

#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
#if (defined DCM_DIDREADSCALINGACCESSINFOIDX_INVALID)
  #error "DCM_DIDREADSCALINGACCESSINFOIDX_INVALID is already defined"
#endif
/* !LINKSTO Dcm.Dsn.Type.Dcm_DidReadScalingAccessInfoIdxType,1 */
/** \brief Type of an index into a DcmDspDidInfo array */
#if (DCM_DSP_DIDSERVICES_NUM_DID_READSCALINGACCESS_INFO > 0xFEU)
typedef uint16 Dcm_DidReadScalingAccessInfoIdxType;
#define DCM_DIDREADSCALINGACCESSINFOIDX_INVALID 0xFFFFU
#else
typedef uint8 Dcm_DidReadScalingAccessInfoIdxType;
#define DCM_DIDREADSCALINGACCESSINFOIDX_INVALID 0xFFU
#endif
#endif /* #if (DCM_DSP_USE_SERVICE_0X24 == STD_ON) */

#if (defined DCM_DIDREFERSIDX_INVALID)
  #error "DCM_DIDREFERSIDX_INVALID is already defined"
#endif
/* !LINKSTO Dcm.Dsn.Type.Dcm_DidRefersIdxType,1 */
/** \brief Type of an index into a Dcm_DidRefers array */
#if (DCM_DID_REF_COUNT > 0xFEU)
typedef uint16 Dcm_DidRefersIdxType;
#define DCM_DIDREFERSIDX_INVALID 0xFFFFU
#else
typedef uint8 Dcm_DidRefersIdxType;
#define DCM_DIDREFERSIDX_INVALID 0xFFU
#endif

#if (defined DCM_DIDSIGNALSIDX_INVALID)
  #error "DCM_DIDSIGNALSIDX_INVALID is already defined"
#endif
/* !LINKSTO Dcm.Dsn.Type.Dcm_DidSignalsIdxType,1 */
/** \brief Type of an index into a Dcm_DidSignals array */
#if (DCM_DID_SIGNALS_COUNT > 0xFEU)
typedef uint16 Dcm_DidSignalsIdxType;
#define DCM_DIDSIGNALSIDX_INVALID 0xFFFFU
#else
typedef uint8 Dcm_DidSignalsIdxType;
#define DCM_DIDSIGNALSIDX_INVALID 0xFFU
#endif

/* !LINKSTO Dcm.Dsn.Type.Dcm_DidType,1 */
/** \brief Type of a Did configuration  */
typedef uint8  Dcm_DidType;
/** \brief Type of a DcmDspDidRange limit */
typedef uint16 Dcm_DidRangeLimitType;

#if (defined DCM_DDDIDSRCTABLEIDX_INVALID)
  #error "DCM_DDDIDSRCTABLEIDX_INVALID is already defined"
#endif
#if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON)
/** \brief Type of an index into an array of sources of a dynamically defined DID */
#if (DCM_NUM_DDDID_SRCTAB_ELEMENTS > 0xFEU)
typedef uint16 Dcm_DDDidSrcTableIdxType;
#define DCM_DDDIDSRCTABLEIDX_INVALID 0xFFFFU
#else
typedef uint8 Dcm_DDDidSrcTableIdxType;
#define DCM_DDDIDSRCTABLEIDX_INVALID 0xFFU
#endif

#endif /* (DCM_DSP_USE_SERVICE_0X2C == STD_ON) */

#if (DCM_DSP_DIDSERVICES_DIDRANGES_MAX_DATA_LENGTH > 0xFFU)
/* !LINKSTO Dcm.Dsn.Type.Dcm_DidRangeMaxDataLengthType,1 */
/** \brief Type of the maximum length of a Did range  */
typedef uint16 Dcm_DidRangeMaxDataLengthType;
#else
typedef uint8 Dcm_DidRangeMaxDataLengthType;
#endif

/* !LINKSTO SWS_Dcm_00803,1 */
/** \brief Function type to request from application if a specific DID is available within the
           range or not */
typedef P2FUNC (Std_ReturnType, DCM_APPL_CODE, Dcm_DidRangeIsDidAvailableFncType)(
  uint16 DID,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_DidSupportedType, AUTOMATIC, DCM_VAR) supported);

/* !LINKSTO SWS_Dcm_00804,1 */
/** \brief Function type to request from application the data range value of a DID */
typedef P2FUNC (Std_ReturnType, DCM_APPL_CODE, Dcm_DidRangeReadFncType)(
  uint16 DID,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Data,
  Dcm_OpStatusType OpStatus,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) DataLength,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/* !LINKSTO SWS_Dcm_00805,1 */
/** \brief Function type to request application to write the data range value of a DID */
typedef P2FUNC (Std_ReturnType, DCM_APPL_CODE, Dcm_DidRangeWriteFncType)(
  uint16 DID,
  P2CONST(uint8, AUTOMATIC, DCM_VAR) Data,
  Dcm_OpStatusType OpStatus,
  uint16 DataLength,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/* !LINKSTO Dcm.Dsn.Type.Dcm_DidConfigType,1 */
typedef struct
{
  /** \brief Identifier of the DID */
  uint16 Did;
  /** \brief Number of signals configured in this DID */
  uint16 NumSignals;
  /** \brief Index of the referenced Dcm_DidInfoType element in the Dcm_DidInfo array */
  Dcm_DidInfoIdxType DidInfoIdx;
  /** \brief Index of the first referred DID element in the Dcm_DidRefers array */
  Dcm_DidRefersIdxType DidRefersIdx;
  /** \brief Index of the first signal in the Dcm_DidSignals array */
  Dcm_DidSignalsIdxType DidSignalsIdx;
  /** \brief Number of referred DIDs */
  uint8 NumRefDids;
#if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON)
  /** \brief Index of the first Dynamically Defined DID entry in the Dcm_DDDidSrcTable array */
  Dcm_DDDidSrcTableIdxType DDDidSrcTableIdx;
#endif
} Dcm_DidConfigType;

#if (DCM_DSP_USE_DIDRANGES == STD_ON)
/* !LINKSTO Dcm.Dsn.Type.Dcm_DidRangeConfigType,1 */
typedef struct
{
  /** \brief Function to call to check the availability of the DID */
  Dcm_DidRangeIsDidAvailableFncType IsDidAvailableFnc;
  /** \brief Function to call to read the data of the DID */
  Dcm_DidRangeReadFncType ReadFnc;
  /** \brief Function to call to write the data of the DID */
  Dcm_DidRangeWriteFncType WriteFnc;
  /** \brief Lower limit of DID range */
  uint16 LowerLimit;
  /** \brief Upper limit of DID range */
  uint16 UpperLimit;
  /** \brief Index of the referenced Dcm_DidInfoType element in the Dcm_DidInfo array */
  Dcm_DidInfoIdxType DidInfoIdx;
  /** \brief Maximum data length of DIDs in this range */
  Dcm_DidRangeMaxDataLengthType MaxDataLength;
  /** \brief Range has gaps, triggers calling the IsDidAvailableFnc */
  boolean HasGaps;
} Dcm_DidRangeConfigType;
#endif /* (DCM_DSP_USE_DIDRANGES == STD_ON) */

/* !LINKSTO Dcm.Dsn.Type.Dcm_DidInfoType,1 */
typedef struct
{
  /** \brief Index of the referenced Dcm_DidReadAccessInfoType element in the
             Dcm_DidReadAccessInfo  array */
  Dcm_DidReadAccessInfoIdxType ReadAccessInfoIdx;
  /** \brief Index of the referenced Dcm_DidWriteAccessInfoType element in the
             Dcm_DidWriteAccessInfo array */
  Dcm_DidWriteAccessInfoIdxType WriteAccessInfoIdx;
  /** \brief Index of the referenced Dcm_DidIoctlAccessInfoType element in the
             Dcm_DidIoctlAccessInfo array */
  Dcm_DidIoctlAccessInfoIdxType IoctlAccessInfoIdx;
#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
  /** \brief Index of the referenced Dcm_DidReadScalingAccessInfoType element in the
             Dcm_DidReadScalingccessInfo array */
  Dcm_DidReadScalingAccessInfoIdxType ReadScalingAccessInfoIdx;
#endif
#if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON)
  uint8 DDDidMaxNumSrcElements;
#endif
} Dcm_DidInfoType;

/** \brief Pointer to array of allowed security levels for accessing the DID */
typedef P2CONST(Dcm_SecLevelType, TYPEDEF, DCM_CONST) Dcm_SecLevelPtrType;
/** \brief Pointer to array of allowed sessions for accessing the DID */
typedef P2CONST(Dcm_SesCtrlType, TYPEDEF, DCM_CONST) Dcm_SesCtrlPtrType;

/* !LINKSTO Dcm.Dsn.Type.Dcm_DidAccessInfoBaseType,1 */
typedef struct
{
  Dcm_SecLevelPtrType SecLevels;
  Dcm_SesCtrlPtrType SesCtrls;
#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
  Dcm_ModeRuleFunctionType ModeRuleDidFunction;
#endif
  uint8 NumSecLevels;
  uint8 NumSesCtrls;
} Dcm_DidAccessInfoBaseType;

/* !LINKSTO Dcm.Dsn.Type.Dcm_DidIoctlAccessInfoExtType,1 */
typedef struct
{
  unsigned int FreezeCurrentStateEnable:1;
  unsigned int ResetToDefaultEnable:1;
  unsigned int ShortTermAdjustmentEnable:1;
  unsigned int ReturnControlToEcuEnable:1;
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
  unsigned int DidControlMask:2;
  unsigned int DidControlMaskSize:3;
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
} Dcm_DidIoctlAccessInfoExtType;

/* !LINKSTO Dcm.Dsn.Type.Dcm_DidReadAccessInfoType,1 */
typedef struct
{
  Dcm_DidAccessInfoBaseType AccessInfoBase;
} Dcm_DidReadAccessInfoType;

/* !LINKSTO Dcm.Dsn.Type.Dcm_DidWriteAccessInfoType,1 */
typedef struct
{
  Dcm_DidAccessInfoBaseType AccessInfoBase;
} Dcm_DidWriteAccessInfoType;

/* !LINKSTO Dcm.Dsn.Type.Dcm_DidIoctlAccessInfoType,1 */
typedef struct
{
  Dcm_DidAccessInfoBaseType AccessInfoBase;
  Dcm_DidIoctlAccessInfoExtType AccessInfoExt;
} Dcm_DidIoctlAccessInfoType;

#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
/* !LINKSTO Dcm.Dsn.Type.Dcm_DidReadScalingAccessInfoType,1 */
typedef struct
{
  Dcm_DidAccessInfoBaseType AccessInfoBase;
} Dcm_DidReadScalingAccessInfoType;
#endif

/* Context type definitions for function: IsDidAvailable */
typedef struct
{
  uint32                ReqLen;
  uint16                Did;
  Dcm_DidConfigIdxType  ConfigIdx;
  Dcm_DidInfoIdxType    InfoIdx;
  uint8                 ProtocolIdx;
  Dcm_DidType           ConfigType;
  Dcm_DidOpType         DidOp;
  boolean               ModeRuleFailed;
} Dcm_DidInfoContextExtType;

typedef struct
{
  Dcm_Dsp_ContextBaseType   BaseContext;
  Dcm_DidInfoContextExtType DidInfoContext;
} Dcm_DidInfoContextType;

typedef P2VAR(Dcm_DidInfoContextType, TYPEDEF, DCM_VAR) Dcm_DidInfoContextTypePtr;

/* Context type definitions for subfunction: ReadSingleStaticDid */
typedef struct
{
  P2VAR(Dcm_DidInfoContextExtType, TYPEDEF, DCM_VAR)  DidInfoPtr;
  P2VAR(Dcm_DspMsgBufferCfgType, TYPEDEF, DCM_VAR) BufferCfgPtr;
  Dcm_ReadSignalContextType ReadSignalContext;
#if (DCM_DSP_DIDSERVICES_USE_OBDSERVICES == STD_ON)
#if (DCM_DSP_VEHINFO_NUM > 0)
  /** \brief context used by function ReqVehInfo */
  Dcm_DspReqVehInfoContextType ReadInfoContext;
#endif
#endif
  boolean WriteDidIds;
} Dcm_ReadSingleStaticDidContextExtType;

typedef struct
{
  Dcm_Dsp_ContextBaseType BaseContext;
  Dcm_ReadSingleStaticDidContextExtType ReadSingleStaticDidContext;
} Dcm_ReadSingleStaticDidContextType;

typedef P2VAR(Dcm_ReadSingleStaticDidContextType, TYPEDEF, DCM_VAR)
  Dcm_ReadSingleStaticDidContextTypePtr;

/* Context type definitions for subfunction: ReadStaticDid */
typedef struct
{
#if (DCM_DID_REF_COUNT > 0U)
  Dcm_DidInfoContextType RefDidInfoContext;
#endif
  Dcm_ReadSingleStaticDidContextType ReadSingleStaticDidContext;
  P2VAR(Dcm_DidInfoContextExtType, TYPEDEF, DCM_VAR) DidInfoPtr;
  P2VAR(Dcm_DspMsgBufferCfgType, TYPEDEF, DCM_VAR) BufferCfgPtr;
  uint16 Did;
  boolean WriteDidIds;
#if (DCM_DID_REF_COUNT > 0U)
  Dcm_DidConfigIdxType   RefDidConfigIdx;
  uint8 RefDidIdx;
  uint8 NumRefDids;
#endif
} Dcm_ReadStaticDidContextExtType;

typedef struct
{
  Dcm_Dsp_ContextBaseType         BaseContext;
  Dcm_ReadStaticDidContextExtType ReadStaticDidContext;
} Dcm_ReadStaticDidContextType;

typedef P2VAR(Dcm_ReadStaticDidContextType, TYPEDEF, DCM_VAR) Dcm_ReadStaticDidContextTypePtr;

/* Context type definitions for function: ReadDid */
typedef struct
{
  /** \brief context used by function (FSM) GetDidInfo */
  Dcm_DidInfoContextType DidInfoContext;
  /** \brief context used by function (FSM) ReadStaticDid */
  Dcm_ReadStaticDidContextType ReadStaticDidContext;
#if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON)
#if (DCM_DSP_USE_MEMORYSERVICES == STD_ON)
  /** \brief Subcontext used for reading memory areas via dynamically defined Dids */
  Dcm_ReadMemoryContextType DynDidReadMemContext;
#endif
  Dcm_DidInfoContextType  DynDidInfoContext;
  Dcm_DspMsgBufferCfgType DynDidBufferCfg;
#endif
  /** \brief pointer to buffer configuration which is shared between all functions */
  P2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) DidBufferCfgPtr;
#if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON)
  uint8                   DynDidIdx;
  Dcm_DidConfigIdxType    DynDidConfigIdx;
  Dcm_DidInfoIdxType      DynDidInfoIdx;
#endif
  boolean                 WriteDidIds;
  boolean                 EnableChecks;
} Dcm_ReadDidContextExtType;

typedef struct
{
  Dcm_Dsp_ContextBaseType   BaseContext;
  Dcm_ReadDidContextExtType ReadDidContext;
} Dcm_ReadDidContextType;

typedef P2VAR(Dcm_ReadDidContextType, TYPEDEF, DCM_VAR) Dcm_ReadDidContextTypePtr;

/* Context type definitions for function: WriteDid */
typedef struct
{
  /** \brief Structure for the DID properties filled by GetDidInfo */
  Dcm_DidInfoContextType DidInfoContext;
  /** \brief Buffer descriptor */
  Dcm_DspMsgBufferCfgType BufferCfg;
  /** \brief Context for signal writing */
  Dcm_DidSignalWriteContextType SignalWriteContext;
  /** \brief Index of DID Signal currently being processed */
  uint16 SignalInd;
  /** \brief Indicates whether EndiannessConversion is required */
  boolean EndiannessConversion;
} Dcm_WriteDidContextExtType;

typedef struct
{
  Dcm_Dsp_ContextBaseType    BaseContext;
  Dcm_WriteDidContextExtType WriteDidContext;
} Dcm_WriteDidContextType;

typedef P2VAR(Dcm_WriteDidContextType, TYPEDEF, DCM_VAR) Dcm_WriteDidContextTypePtr;

#if (DCM_0X2C_0X01_SSVC_ENABLED == STD_ON)
/** \brief Context for defining dynamic Dids (as received by service 0x2C) */
typedef struct
{
  Dcm_DidDataType DataCfg;
  Dcm_DidInfoContextType srcDidInfoContext;
  P2VAR(Dcm_DDDidSrcTblEntryType, TYPEDEF, DCM_VAR) DDDidSrcTab;
  uint16 dddidSize;
  uint8  nrSrcReq;
  uint8  nrSrcUsed;
  uint8  srcDidIdx;
  uint8  ProtocolId;
} Dcm_DDDIDContextType;
#endif /* DCM_0X2C_0X01_SSVC_ENABLED == STD_ON */

/** \brief Shortcut to the read access configuration (generic data set) */
#if (defined DCM_DID_READACCESS_INFO)
  #error "DCM_DID_READACCESS_INFO is already defined"
#endif
#define DCM_DID_READACCESS_INFO(AccessInfoIdx)          \
  (Dcm_DidReadAccessInfo[(AccessInfoIdx)].AccessInfoBase)

/** \brief Shortcut to the read access configuration (generic data set) */
#if (defined DCM_DID_WRITEACCESS_INFO)
  #error "DCM_DID_WRITEACCESS_INFO is already defined"
#endif
#define DCM_DID_WRITEACCESS_INFO(AccessInfoIdx)         \
  (Dcm_DidWriteAccessInfo[(AccessInfoIdx)].AccessInfoBase)

/** \brief Shortcut to the read access configuration (generic data set) */
#if (defined DCM_DID_IOCTLACCESS_INFO)
  #error "DCM_DID_IOCTLACCESS_INFO is already defined"
#endif
#define DCM_DID_IOCTLACCESS_INFO(AccessInfoIdx)         \
  (Dcm_DidIoctlAccessInfo[(AccessInfoIdx)].AccessInfoBase)

/** \brief Shortcut to the read access configuration (specific data set) */
#if (defined DCM_DID_IOCTLACCESS_INFO_EXT)
  #error "DCM_DID_IOCTLACCESS_INFO_EXT is already defined"
#endif
#define DCM_DID_IOCTLACCESS_INFO_EXT(AccessInfoIdx)     \
  (Dcm_DidIoctlAccessInfo[(AccessInfoIdx)].AccessInfoExt)

#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
/** \brief Shortcut to the read access configuration (generic data set) */
#if (defined DCM_DID_READSCALINGACCESS_INFO)
  #error "DCM_DID_READSCALINGACCESS_INFO is already defined"
#endif
#define DCM_DID_READSCALINGACCESS_INFO(AccessInfoIdx)         \
  (Dcm_DidReadScalingAccessInfo[(AccessInfoIdx)].AccessInfoBase)
#endif /* #if (DCM_DSP_USE_SERVICE_0X24 == STD_ON) */

typedef P2CONST(Dcm_DidAccessInfoBaseType, TYPEDEF, DCM_CONST) Dcm_DidAccessInfoBaseTypePtr;

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* GENERATED SECTION BEGIN EA:{80F6F63A-5FB2-4913-9614-66B22899B427} */

/* Interface IfDidServices defines all support functions for accessing  DIDs and its data.
 *
 * The DidServices is designed to be stateless where possible. To achieve this it defines a
 * context type for each function which a DidServices user has to instantiate and pass to the
 * DidServices functions.
 *
 * Note: The DidServices functions support concurrent invocation but some underlying operations
 * might not. If executed in parallel, the current implementation of signal reading might call a
 * call-out function concurrently. As long as underlying operations do not support queuing /
 * locking the respective service handler has to ensure that no parallel operations run. */

/** \brief Initializes the internals of the DidServices unit
 */
extern FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_Init(void);

/** \brief Initializes the context to get the Did information.
 *
 *  The operation requested is either DID_OP_READ in the context of service ReadDataByIdentifier or
 *  ReadDataByPeriodicIdentifier, DID_OP_WRITE in the context of service WriteDataByIdentifier,
 *  DID_OP_IOCONTROL in the context of service InputOutputControlByIdentifier or DID_OP_DYNAMIC in
 *  the context of service DynamicallyDefineDataIdentifier (when configuring dynamic Dids).
 *
 *  \param DidInfoContext [in] Context data structure to be initialized.
 *  \param Did [in] DID to be checked for availability.
 *  \param DidOp [in] Operation for the DID about to be performed.
 *  \param ProtocolIdx [in] Index of the protocol in the context of which diagnostic session and
 *  security level checks shall be performed.
 *  \return void **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_GetDidInfo_Init(
  Dcm_DidInfoContextTypePtr DidInfoContext,
  uint16 Did,
  Dcm_DidOpType DidOp,
  uint8 ProtocolIdx
);

/** \brief Executes the functions to read Did information by calling Dsp_Context_Execute().
 *
 *  Before the first call, the context must be initialized using the GetDidInfo_Init() function.
 *  For an explanation of the return values see ::Dcm_Dsp_Context_Execute()
 *
 *  \param DidInfoContext [in] Pointer to the context to be processed.
 *  \return Std_ReturnType **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_GetDidInfo_Execute(
  Dcm_DidInfoContextTypePtr DidInfoContext
);

/** \brief Cancels reading Did information.
 *
 *  \param DidInfoContext [in] Pointer to the context to be cancelled.
 *  \return Std_ReturnType **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_GetDidInfo_Cancel(
  Dcm_DidInfoContextTypePtr DidInfoContext
);

/** \brief Returns a pointer to internal DID buffer that is used inside General DID purposes
 **
 ** This function must not be used outside of service processing.
 **
  ** \return Pointer to element of Dcm_Union_BufferType.
 */
/* Deviation MISRAC2012-1 <+2> */
extern FUNC_P2VAR(Dcm_Union_BufferType, AUTOMATIC, DCM_CODE) Dcm_Dsp_DidServices_GetDidGeneralBuffer(void);

/** \brief Initializes the context to read a Did.
 *
 *  \param ReadDidContext [in] Context data structure to be initialized.
 *  \param Did [in] Did to be read.
 *  \param WriteDidIds [in] Flag if the Did shall be added in front of the DID data.
 *  \param DidBufferConfig [in] Output buffer configuration.
 *  \param EnableRuleChecks [in] Flag if
 *  \param RxPduId [in] PduId of the RxPdu to get the protocol in the context of which diagnostic
 *  session and security level checks shall be performed.
 *  \return void **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_Init(
  Dcm_ReadDidContextTypePtr ReadDidContext,
  uint16 Did,
  Dcm_DidOpType DidOp,
  boolean WriteDidIds,
  P2VAR(Dcm_DspMsgBufferCfgType, AUTOMATIC, DCM_VAR) DidBufferConfig,
  boolean EnableRuleChecks,
  PduIdType RxPduId
);

/** \brief Executes the functions to read Did information by calling Dsp_Context_Execute().
 *
 *  Before the first call, the context must be initialized using the ReadDud_Init() function. For
 *  an explanation of the return values see ::Dcm_Dsp_Context_Execute()
 *
 *  \param ReadDidContext [in] Pointer to the context to be processed.
 *  \return Std_ReturnType **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_Execute(
  Dcm_ReadDidContextTypePtr ReadDidContext
);

/** \brief Cancels reading the Did.
 *
 *  Cancels a DID read operation so that the currently processed asynchronous subfunction receive a
 *  final call with OpStatus set to DCM_CANCEL.
 *
 *  \param ReadDidContext [in] Pointer to the context to be cancelled.
 *  \return Std_ReturnType **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_ReadDid_Cancel(
  Dcm_ReadDidContextTypePtr ReadDidContext
);

/** \brief Initializes the context to write a Did.
 *
 *  \param WriteDidContext [in] Context data structure to be initialized.
 *  \param Did [in] Did to be written.
 *  \param DataPtr [in] Pointer to the data to be written.
 *  \param DataLen [in] Length of the data to be written.
 *  \param ProtocolIdx [in] Index of the protocol in the context of which diagnostic session and
 *  security level checks shall be performed.
 *  \return void **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_WriteDid_Init(
  Dcm_WriteDidContextTypePtr WriteDidContext,
  uint16 Did,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) DataPtr,
  uint32 DataLen,
  uint8 ProtocolIdx
);

/** \brief Executes the functions to write Did information by calling Dsp_Context_Execute().
 *
 *  Before the first call, the context must be initialized using the WriteDid_Init() function. For
 *  an explanation of the return values see ::Dcm_Dsp_Context_Execute()
 *
 *  \param WriteDidContext [in] Pointer to the context to be processed.
 *  \return Std_ReturnType **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_WriteDid_Execute(
  Dcm_WriteDidContextTypePtr WriteDidContext
);

/** \brief Cancels writing the Did.
 *
 *  Cancels a DID write operation so that the currently processed asynchronous subfunction receive
 *  a final call with OpStatus set to DCM_CANCEL.
 *
 *  \param WriteDidContext [in] Pointer to the context to be cancelled.
 *  \return Std_ReturnType **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_WriteDid_Cancel(
  Dcm_WriteDidContextTypePtr WriteDidContext
);

/** \brief Returns the maximum size of the data of a DID
 *
 *  \param DidInfo [in] Did Identifier of a DID
 *  \return uint32 **/
extern FUNC(uint32, DCM_CODE) Dcm_Dsp_DidServices_GetMaximumDidLength(
  P2CONST(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfo
);

#if (DCM_0X2C_0X03_SSVC_ENABLED == STD_ON)
/** \brief Clears dynamically defined Did by removing all of its source table entries.
 *
 *  \param Did [in] Dynamic Did that has to be deleted.
 *  \return void **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_DynamicDid_Clear(
  uint16 Did
);
#endif /* DCM_0X2C_0X03_SSVC_ENABLED == STD_ON */

/* GENERATED SECTION END   EA:{80F6F63A-5FB2-4913-9614-66B22899B427} */

#if (DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON)
/** \brief The function returns whether the Dynamically Defined DID service is initialized or not.
 *         If the service is not initialized not initialized the Dynamically Defined DIDs cannot
 *         be used.
 *
 *  \return boolean
 *  \retval FALSE              The DDDID service is not initialized
 *  \retval TRUE               The DDDID service is initialized */
extern FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_DynamicDid_Initialized(void);

/** \brief The function initializes the Dynamically Defined DIDs.
 *
 *  \return void */
extern FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_DynamicDid_Init(void);

#if (DCM_DDDID_PERSISTENCE == STD_ON)
/** \brief The function initializes the Dynamically Defined DID service in case when persistence
 *         is configured for DDDIDs. It is invoked from Dcm_MainFunction to handle the
 *         NVM_REQ_PENDING response from NvM_GetErrorStatus.
 *
 *  \return void */
extern FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_DynamicDid_InitPersistent(void);
#endif /* DCM_DDDID_PERSISTENCE == STD_ON */
#endif /* DCM_DSP_DIDSERVICES_DYNAMIC_DID_SUPPORT == STD_ON */

/* == Legacy functions == */

extern FUNC_P2CONST(Dcm_DidAccessInfoBaseType, AUTOMATIC, DCM_CODE) Dcm_Dsp_DidServices_GetAccessInfo(
  Dcm_DidInfoIdxType didInfoIdx,
  Dcm_DidOpType didOpType);

/** \brief Searches a Did configuration if it provides the requested Did.
 **
 ** \note This is in DidServices internal function, external use is deprecated
 **       and only provided for legacy support (SvcH 0x2F and 0x2C).
 **
 ** \param [in]  Did  Requested Did.
 ** \param [out] ConfigIdx Index of the config providing the Did.
 **
 ** \retval TRUE   A matching Did has been found.
 ** \retval FALSE  No matching Did has been found.
 */
extern FUNC(boolean, DCM_CODE) Dcm_Dsp_DidServices_GetDidIndex(
  uint16 Did,
  P2VAR(Dcm_DidConfigIdxType, AUTOMATIC, DCM_VAR) ConfigIdx);

/** \brief Retrieves the results of a DID read operation, be it a reading of a
 **        single DID, a single DID's data or a list of DIDs
 **
 ** \param [in] pReadSignalContext Pointer to a signal reading context
 ** \param [out] Nrc In case reading failed, this parameter contains the negative
 **             response code to be sent to the tester as a reason for the failure
 ** \param [out] AmountOfBufferOccupied Holds the amount of data now present in the buffer
 **             given to the pReadSignalContext when the related
 **             _Setup function was called.
 **
 ** \retval E_DONE  Reading of the signal data for this DID has finished successfully
 ** \retval DCM_E_NOT_OK  Reading of the signal data for this DID has failed
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_ReadSignal_GetResults(
  P2CONST(Dcm_ReadSignalContextType, AUTOMATIC, DCM_VAR) pReadSignalContext,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc,
  P2VAR(uint32, AUTOMATIC, DCM_VAR) AmountOfBufferOccupied );

/** \brief Handles Reading of signals in a Did
 **
 ** This function is available for other units using legacy algorithms to read signals.
 **
 ** \param [inout] pReadSignalContext Pointer to the current DID reading context
 **
 ** \retval E_DONE         Reading of the signal data for this DID has finished successfully
 ** \retval DCM_E_NOT_OK   Reading of the signal data for this DID has failed
 ** \retval DCM_E_PENDING Operation did not complete, it must be called again
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_ReadDataOfSingleDid_Execute(
  P2VAR(Dcm_ReadSignalContextType, AUTOMATIC, DCM_VAR) pReadSignalContext);

#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)

#if ((DCM_DSP_USE_SERVICE_0X2C == STD_ON) && (DCM_DDDID_CHECK_SOURCES == STD_ON))
/** \brief Function that validates all sources of a DDDID against current session, security level
 **        and mode rule.
 **
 ** \param[in]  RequestedPDID  ID of the periodic, dynamically defined, data identifier
 ** \param[out] Nrc            Pointer to a variable to which the negative response code can be
 **                              written
 **
 ** \retval E_OK            Operation successfully completed
 ** \retval E_NOT_OK        Operation NOT successully completed. NRC value holds the failure reason
 **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_ValidateDDDIDSources(
  uint16 RequestedPDID,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc);
#endif /* #if ((DCM_DSP_USE_SERVICE_0X2C == STD_ON) && (DCM_DDDID_CHECK_SOURCES == STD_ON)) */

/** \brief Function that checks a periodic DID to determine if it exists and
 **        can be added to the scheduler under the current current session,
 **        security level and mode condtions.
 **
 ** \param[in]  RequestedPDID  ID of the periodic, dynamically defined, data identifier
 ** \param[out] Nrc            Pointer to a variable to which the negative response code can be
 **                            written
 **
 ** \retval DCM_CHECK_PDID_OK                   Operation successfully completed
 ** \retval DCM_CHECK_PDID_NOT_OK               Operation NOT successully completed
 **                                             NRC value holds the failure reason
 ** \retval DCM_CHECK_PDID_DOES_NOT_EXIST       Requested DID does not exist
 **                                             NRC is not modifed
 ** \retval DCM_CHECK_PDID_UNSUPPORTED_SESSION  Requested DID exists but cannot be read in current
 **                                             diagnostic session
 **/
extern FUNC(Dcm_CheckPeriodicDIDType, DCM_CODE) Dcm_Dsp_DidServices_CheckPeriodicDID(
  uint16 RequestedPDID,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc);
#endif /* #if (DCM_DSP_USE_SERVICE_0X2A == STD_ON) */

#if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
/** \brief Function to lock the DidServices when they need to be used exclusively by one user
 **
 ** \retval DCM_E_OK                Service successfully locked
 ** \retval DCM_E_RESOURCE_LOCKED   Service locked and not available
 **/
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidServices_LockService(void);

/** \brief Function to unlock the DidServices after they were locked
 **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_DidServices_UnlockService(void);
#endif /* #if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

#if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0U)
extern CONST(Dcm_DidConfigType, DCM_CONST) Dcm_DidConfig[DCM_DSP_DIDSERVICES_NUM_DIDS];
#endif

#if (DCM_DSP_DIDSERVICES_NUM_DID_INFO > 0U)
extern CONST(Dcm_DidInfoType, DCM_CONST) Dcm_DidInfo[DCM_DSP_DIDSERVICES_NUM_DID_INFO];
#endif

#if (DCM_DSP_DIDSERVICES_NUM_DID_READACCESS_INFO > 0U)
extern CONST(Dcm_DidReadAccessInfoType, DCM_CONST)
  Dcm_DidReadAccessInfo[DCM_DSP_DIDSERVICES_NUM_DID_READACCESS_INFO];
#endif
#if (DCM_DSP_DIDSERVICES_NUM_DID_WRITEACCESS_INFO > 0U)
extern CONST(Dcm_DidWriteAccessInfoType, DCM_CONST)
  Dcm_DidWriteAccessInfo[DCM_DSP_DIDSERVICES_NUM_DID_WRITEACCESS_INFO];
#endif

#if (DCM_DSP_USE_DIDRANGES == STD_ON)
extern CONST(Dcm_DidRangeConfigType, DCM_CONST)
  Dcm_DidRangeConfig[DCM_DSP_DIDSERVICES_NUM_DID_RANGES];
#endif

#if (DCM_DSP_DIDSERVICES_NUM_DID_IOCTLACCESS_INFO > 0U)
extern CONST(Dcm_DidIoctlAccessInfoType, DCM_CONST)
  Dcm_DidIoctlAccessInfo[DCM_DSP_DIDSERVICES_NUM_DID_IOCTLACCESS_INFO];
#endif

#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
#if (DCM_DSP_DIDSERVICES_NUM_DID_READSCALINGACCESS_INFO > 0U)
extern CONST(Dcm_DidReadScalingAccessInfoType, DCM_CONST)
  Dcm_DidReadScalingAccessInfo[DCM_DSP_DIDSERVICES_NUM_DID_READSCALINGACCESS_INFO];
#endif
#endif /* #if (DCM_DSP_USE_SERVICE_0X24 == STD_ON) */

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

#define DCM_START_SEC_CONST_16
#include <Dcm_MemMap.h>

#if (DCM_DID_REF_COUNT > 0U)
extern CONST(uint16, DCM_CONST) Dcm_DidRefers[DCM_DID_REF_COUNT];
#endif

#define DCM_STOP_SEC_CONST_16
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#if (DCM_0X2C_0X01_SSVC_ENABLED == STD_ON)
/** \brief Context for defining dynamic Dids (as received by service 0x2C) */
extern VAR(Dcm_DDDIDContextType, DCM_VAR) Dcm_DDDIDContext;
#endif /* DCM_0X2C_0X01_SSVC_ENABLED == STD_ON */


#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* #if (DCM_DSP_USE_DIDSERVICES == STD_ON) */

#endif /* ifndef DCM_DSP_DIDSERVICES_H */
/*==================[end of file]================================================================*/
