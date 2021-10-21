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

#ifndef DCM_DSP_DIDSIGNALS_H
#define DCM_DSP_DIDSIGNALS_H

/* !LINKSTO Dcm.Dsn.File.DidSignals.PublicApi,1 */
/* This file contains the public API of the Did Signals software unit. */

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 18.4 (required)
 *     Unions shall not be used.
 *
 *     Reason:
 *     Union is used to declare a type of all possible pointers to functions
 *     used for DID operations. The type is needed to declare an array of such pointers.
 *     Since the function pointers have different types, they are declared as instances
 *     of this union.
 *     By using union instead of structure, memory usage is reduced.
 *
 *  MISRA-2) Deviated Rule: 8.9 (required)
 *     An identifier with external linkage shall have exactly one external definition.
 *
 *     Reason:
 *     This include file is a central include file containing declarations and definitions
 *     used by all software units.
 *     During unit testing, the untested software units of the Dcm are stubbed. These stubs do
 *     provide the definition for some of the global data structures.
 *     By deviating from this rule it is possible to detect unspecified communication in legacy
 *     code using global variables. This is a temporary deviation.
 */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 19.2 (required)
  *     "The union keyword should not be used."
  *
  *     Reason:
  *     Union is used to declare a type of all possible pointers to functions
  *     used for DID operations. The type is needed to declare an array of such pointers.
  *     Since the function pointers have different types, they are declared as instances
  *     of this union.
  *     By using union instead of structure, memory usage is reduced.
  *     A secondary use case for an union is to ensure that a uint8 array is aligned to 4 bytes in
  *     memory so that casting a uint8* to this array to a uint32* will not lead to undefined
  *     behavior.
  *
  *  MISRAC2012-2) Deviated Rule: 8.6 (required)
  *     "An identifier with external linkage shall have exactly one external definition."
  *
  *     Reason:
  *     This include file is a central include file containing declarations and definitions
  *     used by all software units.
  *     During unit testing, the untested software units of the Dcm are stubbed. These stubs do
  *     provide the definition for some of the global data structures.
  *     By deviating from this rule it is possible to detect unspecified communication in legacy
  *     code using global variables. This is a temporary deviation.
  */


/*==================[inclusions]=================================================================*/

#include <Dcm_Int.h>

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/

#define DCM_SIGNALWRITE_STATE_INIT     0U
#define DCM_SIGNALWRITE_STATE_WRITE    1U
#define DCM_SIGNALWRITE_STATE_INVALID  2U
typedef uint8 Dcm_WriteSignalProcessingStType;

#define DCM_SIGNALREAD_STATE_INIT           0U
#define DCM_SIGNALREAD_STATE_CONDITIONCHECK 1U
#define DCM_SIGNALREAD_STATE_DATALENGTH     2U
#define DCM_SIGNALREAD_STATE_READ           3U
#define DCM_SIGNALREAD_STATE_INVALID        4U
#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
#define DCM_SIGNALREAD_STATE_READSCALING    5U
#endif
typedef uint8 Dcm_ReadSignalProcessingStType;

#if (((DCM_DSP_USE_SERVICE_0X22 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X2E == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X2F == STD_ON)) && \
     (DCM_DID_SIGNALS_COUNT > 0))

#if (DCM_DID_BLOCK_ID_CONFIGURED == STD_ON)
/* DID NvM Operation IDs  */
/** \brief DID NvM Operation ID for no operation     */
#define DCM_DIDNVMOPERATION_NONE          0x00U
/** \brief DID NvM Operation ID for reading NvM data */
#define DCM_DIDNVMOPERATION_READNVMDATA   0x01U
/** \brief DID NvM Operation ID for writing NvM data  */
#define DCM_DIDNVMOPERATION_WRITENVMDATA  0x02U
typedef uint8 Dcm_DidNvmOperationType;
#endif /* (DCM_DID_BLOCK_ID_CONFIGURED == STD_ON) */

#endif /* (((DCM_DSP_USE_SERVICE_0X22 == STD_ON) || \
         (DCM_DSP_USE_SERVICE_0X2E == STD_ON) || \
         (DCM_DSP_USE_SERVICE_0X2F == STD_ON)) && \
        (DCM_DID_SIGNALS_COUNT > 0)) */

/* !LINKSTO Dcm.Dsn.Type.Dcm_DidDataDescriptorType,1 */
/** \brief Mask to extract DidData Fixed Length information from DidDataDescriptor */
#define DCM_DID_DATA_FIXED_LENGTH        0x0008U
/** \brief Mask to extract ReadData availability from DidDataDescriptor */
#define DCM_DID_DATA_READDATA            0x0100U
/** \brief Mask to extract ReadDataLength availability from DidDataDescriptor */
#define DCM_DID_DATA_READDATALENGTH      0x0200U
/** \brief Mask to extract ConditionCheckRead availability from DidDataDescriptor */
#define DCM_DID_DATA_CONDITIONCHECKREAD  0x0400U
/** \brief Mask to extract WriteData availability from DidDataDescriptor */
#define DCM_DID_DATA_WRITEDATA           0x0800U
/** \brief Mask to extract ShortTermAdjustment availability from DidDataDescriptor */
#define DCM_DID_DATA_SHORTTERMADJ        0x1000U
/** \brief Mask to extract IoControl Freeze availability from DidDataDescriptor */
#define DCM_DID_DATA_IOCTRLFREEZE        0x2000U
/** \brief Mask to extract IoControl Reset to Default availability from DidDataDescriptor */
#define DCM_DID_DATA_IOCTRLRESET         0x4000U
/** \brief Mask to extract Return Control to ECU availability from DidDataDescriptor */
#define DCM_DID_DATA_IOCTRLRETURN        0x8000U
/** \brief Mask to extract ReadData sender received  availability from DidDataDescriptor */
#define DCM_DID_DATA_READDATA_SR         0x0100U
/** \brief Mask to extract WriteData sender received  availability from DidDataDescriptor */
#define DCM_DID_DATA_WRITEDATA_SR        0x0200U
#if(DCM_DSP_USE_SERVICE_0X24 == STD_ON)
/** \brief Mask to extract ReadScalingData availability from DidDataDescriptor */
#define DCM_DID_DATA_READSCALINGDATA     0x10000U
/** \brief DID Data descriptor field type */
typedef uint32 Dcm_DidDataDescriptorType;
#else
typedef uint16 Dcm_DidDataDescriptorType;
#endif

/* Different types access interfaces of DcmDspData for a Did */
/** \brief DcmDspData access type: access data using the NVRAM Apis */
#if (defined DCM_USE_BLOCK_ID)
  #error "DCM_USE_BLOCK_ID is already defined"
#endif
#define DCM_USE_BLOCK_ID 0U

/** \brief DcmDspData access type: access data using a Port requiring an asynchronous
 *         ClientServerInterface */
#if (defined DCM_USE_DATA_ASYNCH_CLIENT_SERVER)
  #error "DCM_USE_DATA_ASYNCH_CLIENT_SERVER is already defined"
#endif
#define DCM_USE_DATA_ASYNCH_CLIENT_SERVER 1U

/** \brief DcmDspData access type: access data using the functions that are defined in parameters
 *         of type EcucFunctionNameDef; OpStatus defined as IN parameter */
#if (defined DCM_USE_DATA_ASYNCH_FNC)
  #error "DCM_USE_DATA_ASYNCH_FNC is already defined"
#endif
#define DCM_USE_DATA_ASYNCH_FNC 2U

/** \brief DcmDspData access type: access data using a Port requiring a SenderReceiverInterface
 *         (USE_DATA_SENDER_RECEIVER or USE_DATA_SENDER_RECEIVER_AS_SERVICE) */
#if (defined DCM_USE_DATA_SENDER_RECEIVER)
  #error "DCM_USE_DATA_SENDER_RECEIVER is already defined"
#endif
#define DCM_USE_DATA_SENDER_RECEIVER 3U

/** \brief DcmDspData access type: access data using a Port requiring a synchronous
 *         ClientServerInterface */
#if (defined DCM_USE_DATA_SYNCH_CLIENT_SERVER)
  #error "DCM_USE_DATA_SYNCH_CLIENT_SERVER is already defined"
#endif
#define DCM_USE_DATA_SYNCH_CLIENT_SERVER 4U

/** \brief DcmDspData access type: access data using the functions that are defined in parameters
 *         of type EcucFunctionNameDef; OpStatus parameter does not exist in the prototype */
#if (defined DCM_USE_DATA_SYNCH_FNC)
  #error "DCM_USE_DATA_SYNCH_FNC is already defined"
#endif
#define DCM_USE_DATA_SYNCH_FNC 5U

/** \brief DcmDspData access type: access data using a direct access to IoHwAb */
#if (defined DCM_USE_ECU_SIGNAL)
  #error "DCM_USE_ECU_SIGNAL is already defined"
#endif
#define DCM_USE_ECU_SIGNAL 6U
/** \brief Type for DidData AccessInterface */
typedef uint8 Dcm_DidDataAccesType;


/** \brief Type of function to check if the DID can be read (Sync operations) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ConditionCheckReadTypeSyncType) (
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to check if the DID can be read (Async operations) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ConditionCheckReadTypeAsyncType) (
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function for reading the length of the DID Data (Sync operation) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ReadDataLengthTypeSyncType) (
  P2VAR(uint16, AUTOMATIC, DCM_VAR) DataLength);

/** \brief Type of function for reading the length of the DID Data (Async operation) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ReadDataLengthTypeAsyncType) (
  Dcm_OpStatusType OpStatus, P2VAR(uint16, AUTOMATIC, DCM_VAR) DataLength);

/** \brief Type of function for reading the DID using sender-receiver, Synch operation */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ReadDataFncTypeSyncType) (
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Data);

/** \brief Type of function for reading the Data of a DID signal (Async operation) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ReadDataFncTypeAsyncType) (
  Dcm_OpStatusType OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Data);

#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
/** \brief Type of function for reading the Scaling DID using Synch operation */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ReadScalingDataFncTypeSyncType) (
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Data,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function for reading the Scaling Data of a DID signal (Async operation) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ReadScalingDataFncTypeAsyncType) (
  Dcm_OpStatusType OpStatus,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Data,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);
#endif

/** \brief Type of function for writing the DID in case of fixed length data (Sync operation) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_WriteDataFixedTypeSyncType) (
  P2CONST(uint8, AUTOMATIC, DCM_CONST) Data,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function for writing the DID in case of variable length data (Sync operation) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_WriteDataVariableTypeSyncType) (
  P2CONST(uint8, AUTOMATIC, DCM_CONST) Data,
  uint16 DataLength,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function for writing the DID in case of fixed length data (Async operation) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_WriteDataFixedTypeAsyncType) (
  P2CONST(uint8, AUTOMATIC, DCM_CONST) Data,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function for writing the DID in case of variable length data (Async operation) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_WriteDataVariableTypeAsyncType) (
  P2CONST(uint8, AUTOMATIC, DCM_CONST) Data,
  uint16 DataLength,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of Rte_Result_Xxx function for writing the DID */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteResultWriteDataType) (
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request application to freeze the current state of an IOControl
      (Sync operation) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_FreezeCurrentStateSyncType) (
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request application to freeze the current state of an IOControl
      (Sync operation) where DcmDspDidControlMaskSize is set to 1 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_FreezeCurrentStateSyncMaskExternSize1Type) (
  uint8 ControlMask,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request application to freeze the current state of an IOControl
      (Sync operation) where DcmDspDidControlMaskSize is set to 2 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_FreezeCurrentStateSyncMaskExternSize2Type) (
  uint16 ControlMask,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request application to freeze the current state of an IOControl
      (Sync operation) where DcmDspDidControlMaskSize is set to 3 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_FreezeCurrentStateSyncMaskExternSize3Type) (
  uint32 ControlMask,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request application to freeze the current state of an IOControl
      (Sync operation) where DcmDspDidControlMaskSize is set to 4 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_FreezeCurrentStateSyncMaskExternSize4Type) (
  uint32 ControlMask,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request application to freeze the current state of an IO Control
      (Async operation) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_FreezeCurrentStateAsyncType) (
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to adjust the IO signal (Sync operation)
    with fixed length data */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ShortTermAdjustmentFixedTypeSyncType) (
  P2CONST(uint8, AUTOMATIC, DCM_CONST) ControlOptionRecord,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to adjust the IO signal (Sync operation)
    with fixed length data and DcmDspDidControlMaskSize is set to 1 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ShortTermAdjustmentFixedTypeSyncMaskExternSize1Type) (
  P2CONST(uint8, AUTOMATIC, DCM_CONST) ControlOptionRecord,
  uint8 ControlMask,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to adjust the IO signal (Sync operation)
    with fixed length data and DcmDspDidControlMaskSize is set to 2 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ShortTermAdjustmentFixedTypeSyncMaskExternSize2Type) (
  P2CONST(uint8, AUTOMATIC, DCM_CONST) ControlOptionRecord,
  uint16 ControlMask,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to adjust the IO signal (Sync operation)
    with fixed length data and DcmDspDidControlMaskSize is set to 3 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ShortTermAdjustmentFixedTypeSyncMaskExternSize3Type) (
  P2CONST(uint8, AUTOMATIC, DCM_CONST) ControlOptionRecord,
  uint32 ControlMask,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to adjust the IO signal (Sync operation)
    with fixed length data and DcmDspDidControlMaskSize is set to 4 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ShortTermAdjustmentFixedTypeSyncMaskExternSize4Type) (
  P2CONST(uint8, AUTOMATIC, DCM_CONST) ControlOptionRecord,
  uint32 ControlMask,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to adjust the IO signal (Sync operation)
    with variable length data */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ShortTermAdjustmentVariableTypeSyncType) (
  P2CONST(uint8, AUTOMATIC, DCM_CONST) ControlOptionRecord,
  uint16 DataLength,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to adjust the IO signal (Sync operation)
    with variable length data and DcmDspDidControlMaskSize is set to 1 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ShortTermAdjustmentVariableTypeSyncMaskExternSize1Type) (
  P2CONST(uint8, AUTOMATIC, DCM_CONST) ControlOptionRecord,
  uint16 DataLength,
  uint8  ControlMask,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to adjust the IO signal (Sync operation)
    with variable length data and DcmDspDidControlMaskSize is set to 2 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ShortTermAdjustmentVariableTypeSyncMaskExternSize2Type) (
  P2CONST(uint8, AUTOMATIC, DCM_CONST) ControlOptionRecord,
  uint16 DataLength,
  uint16 ControlMask,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to adjust the IO signal (Sync operation)
    with variable length data and DcmDspDidControlMaskSize is set to 3 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ShortTermAdjustmentVariableTypeSyncMaskExternSize3Type) (
  P2CONST(uint8, AUTOMATIC, DCM_CONST) ControlOptionRecord,
  uint16 DataLength,
  uint32 ControlMask,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to adjust the IO signal (Sync operation)
    with variable length data and DcmDspDidControlMaskSize is set to 4 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ShortTermAdjustmentVariableTypeSyncMaskExternSize4Type) (
  P2CONST(uint8, AUTOMATIC, DCM_CONST) ControlOptionRecord,
  uint16 DataLength,
  uint32 ControlMask,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);


/** \brief Type of function to request to application to adjust the IO signal (Async operation)
    and fixed length data */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ShortTermAdjustmentFixedTypeAsyncType) (
  P2CONST(uint8, AUTOMATIC, DCM_CONST) ControlOptionRecord,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to adjust the IO signal (Async operation)
    and variable length data */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ShortTermAdjustmentVariableTypeAsyncType) (
  P2CONST(uint8, AUTOMATIC, DCM_CONST) ControlOptionRecord,
  uint16 DataLength,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to reset an IOControl to default value
      (Sync operation) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ResetToDefaultSyncType) (
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to reset an IOControl to default value
      (Sync operation) where DcmDspDidControlMaskSize is set to 1 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ResetToDefaultSyncMaskExternSize1Type) (
  uint8 ControlMask,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to reset an IOControl to default value
      (Sync operation) where DcmDspDidControlMaskSize is set to 2 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ResetToDefaultSyncMaskExternSize2Type) (
  uint16 ControlMask,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to reset an IOControl to default value
      (Sync operation) where DcmDspDidControlMaskSize is set to 3 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ResetToDefaultSyncMaskExternSize3Type) (
  uint32 ControlMask,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to reset an IOControl to default value
      (Sync operation) where DcmDspDidControlMaskSize is set to 4 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ResetToDefaultSyncMaskExternSize4Type) (
  uint32 ControlMask,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request application to reset an IOControl to default value
     (Async operation) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ResetToDefaultAsyncType) (
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to return control to ECU of an IOControl
      (Sync operation) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ReturnControlToEcuSyncType) (
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to return control to ECU of an IOControl
      (Sync operation) where DcmDspDidControlMaskSize is set to 1 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ReturnControlToEcuSyncMaskExternSize1Type) (
  uint8 ControlMask,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to return control to ECU of an IOControl
      (Sync operation) where DcmDspDidControlMaskSize is set to 1 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ReturnControlToEcuSyncMaskExternSize2Type) (
  uint16 ControlMask,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to return control to ECU of an IOControl
      (Sync operation) where DcmDspDidControlMaskSize is set to 1 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ReturnControlToEcuSyncMaskExternSize3Type) (
  uint32 ControlMask,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to return control to ECU of an IOControl
      (Sync operation) where DcmDspDidControlMaskSize is set to 1 */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ReturnControlToEcuSyncMaskExternSize4Type) (
  uint32 ControlMask,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Type of function to request to application to return control to ECU of an IOControl
        (Async operation) */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_ReturnControlToEcuAsyncType) (
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) ErrorCode);

/** \brief Rte_Read operation on the DataServices_<data> port for boolean data **/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteRead_DataServices_booleanType) (
  P2VAR(boolean, AUTOMATIC, DCM_VAR) Data);

/** \brief Rte_Read operation on the DataServices_<data> port for SINT16 or
 *  SINT16_N data type **/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteRead_DataServices_sint16Type) (
  P2VAR(sint16, AUTOMATIC, DCM_VAR) Data);

/** \brief Rte_Read operation on the DataServices_<data> port for SINT32 or
 *  SINT32_N data type **/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteRead_DataServices_sint32Type) (
  P2VAR(sint32, AUTOMATIC, DCM_VAR) Data);

/** \brief Rte_Read operation on the DataServices_<data> port for SINT8 or SINT8_N
 *  data type **/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteRead_DataServices_sint8Type) (
  P2VAR(sint8, AUTOMATIC, DCM_VAR) Data);

/** \brief Rte_Read operation on the DataServices_<data> port for UINT16 or
 *  UINT16_N data type **/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteRead_DataServices_uint16Type) (
  P2VAR(uint16, AUTOMATIC, DCM_VAR) Data);

/** \brief Rte_Read operation on the DataServices_<data> port for UINT32 or
 *  UINT32_N data type **/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteRead_DataServices_uint32Type) (
  P2VAR(uint32, AUTOMATIC, DCM_VAR) Data);

/** \brief Rte_Read operation on the DataServices_<data> port for UINT8 or UINT8_N
 *  data type **/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteRead_DataServices_uint8Type) (
  P2VAR(uint8, AUTOMATIC, DCM_VAR) Data);

/** \brief Rte_Write operation on the DataServices_<data> port for boolean data **/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteWrite_DataServices_booleanType) (
  boolean Data);

/** \brief Rte_Write operation on the DataServices_<data> port for SINT16 data type **/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteWrite_DataServices_sint16Type) (
  VAR(sint16, DCM_VAR) Data);

/** \brief Rte_Write operation on the DataServices_<data> port for SINT32  data type **/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteWrite_DataServices_sint32Type) (
  VAR(sint32, DCM_VAR) Data);

/** \brief Rte_Write operation on the DataServices_<data> port for SINT8  data type **/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteWrite_DataServices_sint8Type) (
  VAR(sint8, DCM_VAR) Data);

/** \brief Rte_Write operation on the DataServices_<data> port for UINT16 data type **/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteWrite_DataServices_uint16Type) (
  VAR(uint16, DCM_VAR) Data);

/** \brief Rte_Write operation on the DataServices_<data> port for UINT32 data type **/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteWrite_DataServices_uint32Type) (
  VAR(uint32, DCM_VAR) Data);

/** \brief Rte_Write operation on the DataServices_<data> port for UINT8 data type **/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteWrite_DataServices_uint8Type) (
  VAR(uint8, DCM_VAR) Data);

/** \brief Rte_Write operation on the DataServices_<data> port for SINT16_N data type **/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteWrite_DataServices_sint16NType) (
  P2CONST(sint16, AUTOMATIC, DCM_VAR) Data);

/** \brief Rte_Write operation on the DataServices_<data> port for SINT32_N data type **/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteWrite_DataServices_sint32NType) (
  P2CONST(sint32, AUTOMATIC, DCM_VAR) Data);

/** \brief Rte_Write operation on the DataServices_<data> port for SINT8_N data type **/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteWrite_DataServices_sint8NType) (
  P2CONST(sint8, AUTOMATIC, DCM_VAR) Data);

/** \brief Rte_Write operation on the DataServices_<data> port for UINT16_N data type **/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteWrite_DataServices_uint16NType) (
  P2CONST(uint16, AUTOMATIC, DCM_VAR) Data);

/** \brief Rte_Write operation on the DataServices_<data> port for UINT32_N data type **/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteWrite_DataServices_uint32NType) (
  P2CONST(uint32, AUTOMATIC, DCM_VAR) Data);

/** \brief Rte_Write operation on the DataServices_<data> port for UINT8_N data type **/
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RteWrite_DataServices_uint8NType) (
  P2CONST(uint8, AUTOMATIC, DCM_VAR) Data);

/** \brief General pointer to function for DID operations */
typedef P2FUNC(void, DCM_APPL_CODE, Dcm_GeneralDidOperationFuncType) (void);

/* !LINKSTO Dcm.Dsn.Type.Dcm_VariableFunctionPointerRecordType,1 */
/** \brief Union containing the types of pointers to functions for DID operations */
/* Deviation MISRAC2012-1 */
/* Deviation MISRA-1 */
typedef union
{
  Dcm_GeneralDidOperationFuncType     GeneralDidOperationFunc;
  Dcm_ConditionCheckReadTypeSyncType  ConditionCheckReadSync;
  Dcm_ConditionCheckReadTypeAsyncType ConditionCheckReadAsync;
  Dcm_ReadDataLengthTypeSyncType      ReadDataLengthSync;
  Dcm_ReadDataLengthTypeAsyncType     ReadDataLengthAsync;
  Dcm_ReadDataFncTypeSyncType         ReadDataFncSync;
  Dcm_ReadDataFncTypeAsyncType        ReadDataFncAsync;
#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
  Dcm_ReadScalingDataFncTypeSyncType  ReadScalingDataFncSync;
  Dcm_ReadScalingDataFncTypeAsyncType ReadScalingDataFncAsync;
#endif /* #if (DCM_DSP_USE_SERVICE_0X24 == STD_ON) */
  Dcm_WriteDataFixedTypeSyncType      WriteFixedDataSync;
  Dcm_WriteDataVariableTypeSyncType   WriteVariableDataSync;
  Dcm_WriteDataFixedTypeAsyncType     WriteFixedLengthDataAsync;
  Dcm_WriteDataVariableTypeAsyncType  WriteVarLengthDataAsync;
  Dcm_RteResultWriteDataType      ResultWriteData;
  Dcm_FreezeCurrentStateSyncType      FreezeCurrentStateSync;
  Dcm_FreezeCurrentStateSyncMaskExternSize1Type FreezeCurrentStateSyncMaskExternSize1;
  Dcm_FreezeCurrentStateSyncMaskExternSize2Type FreezeCurrentStateSyncMaskExternSize2;
  Dcm_FreezeCurrentStateSyncMaskExternSize3Type FreezeCurrentStateSyncMaskExternSize3;
  Dcm_FreezeCurrentStateSyncMaskExternSize4Type FreezeCurrentStateSyncMaskExternSize4;
  Dcm_FreezeCurrentStateAsyncType     FreezeCurrentStateAsync;
  Dcm_ShortTermAdjustmentFixedTypeSyncType      ShortTermAdjustmentFixedSync;
  Dcm_ShortTermAdjustmentFixedTypeSyncMaskExternSize1Type ShortTermAdjustmentFixedSyncMaskExternalSize1;
  Dcm_ShortTermAdjustmentFixedTypeSyncMaskExternSize2Type ShortTermAdjustmentFixedSyncMaskExternalSize2;
  Dcm_ShortTermAdjustmentFixedTypeSyncMaskExternSize3Type ShortTermAdjustmentFixedSyncMaskExternalSize3;
  Dcm_ShortTermAdjustmentFixedTypeSyncMaskExternSize4Type ShortTermAdjustmentFixedSyncMaskExternalSize4;
  Dcm_ShortTermAdjustmentVariableTypeSyncType   ShortTermAdjustmentVariableSync;
  Dcm_ShortTermAdjustmentVariableTypeSyncMaskExternSize1Type ShortTermAdjustmentVariableTypeSyncMaskExternSize1;
  Dcm_ShortTermAdjustmentVariableTypeSyncMaskExternSize2Type ShortTermAdjustmentVariableTypeSyncMaskExternSize2;
  Dcm_ShortTermAdjustmentVariableTypeSyncMaskExternSize3Type ShortTermAdjustmentVariableTypeSyncMaskExternSize3;
  Dcm_ShortTermAdjustmentVariableTypeSyncMaskExternSize4Type ShortTermAdjustmentVariableTypeSyncMaskExternSize4;
  Dcm_ShortTermAdjustmentFixedTypeAsyncType     ShortTermAdjustmentFixedAsync;
  Dcm_ShortTermAdjustmentVariableTypeAsyncType  ShortTermAdjustmentVariableAsync;
  Dcm_ResetToDefaultSyncType          ResetToDefaultSync;
  Dcm_ResetToDefaultSyncMaskExternSize1Type ResetToDefaultSyncMaskExternSize1;
  Dcm_ResetToDefaultSyncMaskExternSize2Type ResetToDefaultSyncMaskExternSize2;
  Dcm_ResetToDefaultSyncMaskExternSize3Type ResetToDefaultSyncMaskExternSize3;
  Dcm_ResetToDefaultSyncMaskExternSize4Type ResetToDefaultSyncMaskExternSize4;
  Dcm_ResetToDefaultAsyncType         ResetToDefaultAsync;
  Dcm_ReturnControlToEcuSyncType      ReturnControlToEcuSync;
  Dcm_ReturnControlToEcuSyncMaskExternSize1Type      ReturnControlToEcuSyncMaskExternSize1;
  Dcm_ReturnControlToEcuSyncMaskExternSize2Type      ReturnControlToEcuSyncMaskExternSize2;
  Dcm_ReturnControlToEcuSyncMaskExternSize3Type      ReturnControlToEcuSyncMaskExternSize3;
  Dcm_ReturnControlToEcuSyncMaskExternSize4Type      ReturnControlToEcuSyncMaskExternSize4;
  Dcm_ReturnControlToEcuAsyncType     ReturnControlToEcuAsync;

#if (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON)
  Dcm_RteRead_DataServices_booleanType  Rte_Read_DataServices_boolean;
  Dcm_RteRead_DataServices_sint16Type   Rte_Read_DataServices_sint16;
  Dcm_RteRead_DataServices_sint32Type   Rte_Read_DataServices_sint32;
  Dcm_RteRead_DataServices_sint8Type    Rte_Read_DataServices_sint8;
  Dcm_RteRead_DataServices_uint16Type   Rte_Read_DataServices_uint16;
  Dcm_RteRead_DataServices_uint32Type   Rte_Read_DataServices_uint32;
  Dcm_RteRead_DataServices_uint8Type    Rte_Read_DataServices_uint8;
  Dcm_RteWrite_DataServices_booleanType Rte_Write_DataServices_boolean;
  Dcm_RteWrite_DataServices_sint16Type  Rte_Write_DataServices_sint16;
  Dcm_RteWrite_DataServices_sint32Type  Rte_Write_DataServices_sint32;
  Dcm_RteWrite_DataServices_sint8Type   Rte_Write_DataServices_sint8;
  Dcm_RteWrite_DataServices_uint16Type  Rte_Write_DataServices_uint16;
  Dcm_RteWrite_DataServices_uint32Type  Rte_Write_DataServices_uint32;
  Dcm_RteWrite_DataServices_uint8Type   Rte_Write_DataServices_uint8;
  Dcm_RteWrite_DataServices_sint16NType  Rte_Write_DataServices_sint16N;
  Dcm_RteWrite_DataServices_sint32NType  Rte_Write_DataServices_sint32N;
  Dcm_RteWrite_DataServices_sint8NType   Rte_Write_DataServices_sint8N;
  Dcm_RteWrite_DataServices_uint16NType  Rte_Write_DataServices_uint16N;
  Dcm_RteWrite_DataServices_uint32NType  Rte_Write_DataServices_uint32N;
  Dcm_RteWrite_DataServices_uint8NType   Rte_Write_DataServices_uint8N;
#endif /*#if (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON)*/

} Dcm_VariableFunctionPointerRecordType;

/** \brief  This type contains the configuration information for datas belonging to a Did
*/
typedef struct
{
  /** \brief Pointer to ConditionCheckRead() for sync function or operation **/
  Dcm_ConditionCheckReadTypeSyncType ConditionCheckReadSync;
  /** \brief Pointer to ConditionCheckRead() for async function or operation **/
  Dcm_ConditionCheckReadTypeAsyncType ConditionCheckReadAsync;
  /** \brief Pointer to ReadDataLength for sync function or operation **/
  Dcm_ReadDataLengthTypeSyncType ReadDataLengthSync;
  /** \brief Pointer to ReadDataLength for async function or operation according to
   *  the AUTOSAR4.2.1 API **/
  Dcm_ReadDataLengthTypeAsyncType ReadDataLengthAsync;
  /** \brief Pointer to ReadData() for sync function or operation **/
  Dcm_ReadDataFncTypeSyncType ReadDataFncSync;
  /** \brief Pointer to ReadData() for async function or operation **/
  Dcm_ReadDataFncTypeAsyncType ReadDataFncAsync;
#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
  /** \brief Pointer to ReadScalingData() for sync function or operation **/
  Dcm_ReadScalingDataFncTypeSyncType ReadScalingDataFncSync;
  /** \brief Pointer to ReadScalingData() for async function or operation **/
  Dcm_ReadScalingDataFncTypeAsyncType ReadScalingDataFncAsync;
#endif /* #if (DCM_DSP_USE_SERVICE_0X24 == STD_ON) */
  /** \brief Pointer to WriteData() for fixed length data sync function or operation **/
  Dcm_WriteDataFixedTypeSyncType WriteFixedDataSync;
  /** \brief Pointer to WriteData() for variable length data sync function or operation **/
  Dcm_WriteDataVariableTypeSyncType WriteVariableDataSync;
  /** \brief Pointer to WriteData() for fixed length data async function or operation **/
  Dcm_WriteDataFixedTypeAsyncType WriteFixedLengthDataAsync;
  /** \brief Pointer to WriteData() for variable length data async function or operation **/
  Dcm_WriteDataVariableTypeAsyncType WriteVarLengthDataAsync;
  /** \brief Pointer to result of WriteData() for async function or operation **/
  Dcm_RteResultWriteDataType ResultWriteData;
  /** \brief Pointer to FreezeCurrentState() to freeze the current state of an
   *  IOControl sync function or operation **/
/* Deviation MISRAC2012-1 */
/* Deviation MISRA-1 */
  union
  {
    Dcm_FreezeCurrentStateSyncType FreezeCurrentStateSync;
    Dcm_FreezeCurrentStateSyncMaskExternSize1Type FreezeCurrentStateSyncMaskExternSize1;
    Dcm_FreezeCurrentStateSyncMaskExternSize2Type FreezeCurrentStateSyncMaskExternSize2;
    Dcm_FreezeCurrentStateSyncMaskExternSize3Type FreezeCurrentStateSyncMaskExternSize3;
    Dcm_FreezeCurrentStateSyncMaskExternSize4Type FreezeCurrentStateSyncMaskExternSize4;
  } FreezeCurrentStateSyncUnion;
  /** \brief Pointer to FreezeCurrentState() to freeze the current state of an
   *  IOControl async function or operation **/
  Dcm_FreezeCurrentStateAsyncType FreezeCurrentStateAsync;
  /** \brief Pointer to ShortTermAdjustment() fixed length sync function or operation **/
/* Deviation MISRAC2012-1 */
/* Deviation MISRA-1 */
  union
  {
    Dcm_ShortTermAdjustmentFixedTypeSyncType ShortTermAdjustmentFixedSync;
    Dcm_ShortTermAdjustmentFixedTypeSyncMaskExternSize1Type ShortTermAdjustmentFixedSyncMaskExternalSize1;
    Dcm_ShortTermAdjustmentFixedTypeSyncMaskExternSize2Type ShortTermAdjustmentFixedSyncMaskExternalSize2;
    Dcm_ShortTermAdjustmentFixedTypeSyncMaskExternSize3Type ShortTermAdjustmentFixedSyncMaskExternalSize3;
    Dcm_ShortTermAdjustmentFixedTypeSyncMaskExternSize4Type ShortTermAdjustmentFixedSyncMaskExternalSize4;
  } ShortTermAdjustmentFixedSyncUnion;
  /** \brief Pointer to ShortTermAdjustment() variable length sync function or operation **/
/* Deviation MISRAC2012-1 */
/* Deviation MISRA-1 */
  union
  {
    Dcm_ShortTermAdjustmentVariableTypeSyncType ShortTermAdjustmentVariableSync;
    Dcm_ShortTermAdjustmentVariableTypeSyncMaskExternSize1Type ShortTermAdjustmentVariableTypeSyncMaskExternSize1;
    Dcm_ShortTermAdjustmentVariableTypeSyncMaskExternSize2Type ShortTermAdjustmentVariableTypeSyncMaskExternSize2;
    Dcm_ShortTermAdjustmentVariableTypeSyncMaskExternSize3Type ShortTermAdjustmentVariableTypeSyncMaskExternSize3;
    Dcm_ShortTermAdjustmentVariableTypeSyncMaskExternSize4Type ShortTermAdjustmentVariableTypeSyncMaskExternSize4;
  } ShortTermAdjustmentVariableSyncUnion;
  /** \brief Pointer to ShortTermAdjustment(), fixed length to adjust the IO signal
   *  async function or operation **/
  Dcm_ShortTermAdjustmentFixedTypeAsyncType ShortTermAdjustmentFixedAsync;
  /** \brief Pointer to ShortTermAdjustment() variable length async function or operation **/
  Dcm_ShortTermAdjustmentVariableTypeAsyncType ShortTermAdjustmentVariableAsync;
  /** \brief Pointer to ResetToDefault() to reset an IOControl to default value, sync function
   *  or operation **/
/* Deviation MISRAC2012-1 */
/* Deviation MISRA-1 */
  union
  {
    Dcm_ResetToDefaultSyncType ResetToDefaultSync;
    Dcm_ResetToDefaultSyncMaskExternSize1Type ResetToDefaultSyncMaskExternSize1;
    Dcm_ResetToDefaultSyncMaskExternSize2Type ResetToDefaultSyncMaskExternSize2;
    Dcm_ResetToDefaultSyncMaskExternSize3Type ResetToDefaultSyncMaskExternSize3;
    Dcm_ResetToDefaultSyncMaskExternSize4Type ResetToDefaultSyncMaskExternSize4;
  } ResetToDefaultSyncUnion;
  /** \brief Pointer to ResetToDefault() to reset an IOControl to default value, async function
   *  or operation **/
  Dcm_ResetToDefaultAsyncType ResetToDefaultAsync;

  /** \brief Pointer to ReturnControlToEcu() to return control to ECU of an IOControl
   *  sync function or operation **/
/* Deviation MISRAC2012-1 */
/* Deviation MISRA-1 */
  union
  {
    Dcm_ReturnControlToEcuSyncType ReturnControlToEcuSync;
    Dcm_ReturnControlToEcuSyncMaskExternSize1Type      ReturnControlToEcuSyncMaskExternSize1;
    Dcm_ReturnControlToEcuSyncMaskExternSize2Type      ReturnControlToEcuSyncMaskExternSize2;
    Dcm_ReturnControlToEcuSyncMaskExternSize3Type      ReturnControlToEcuSyncMaskExternSize3;
    Dcm_ReturnControlToEcuSyncMaskExternSize4Type      ReturnControlToEcuSyncMaskExternSize4;
  } ReturnControlToEcuSyncUnion;
  /** \brief Pointer to ReturnControlToEcu() to return control to ECU of an IOControl
   *  async function or operation **/
  Dcm_ReturnControlToEcuAsyncType ReturnControlToEcuAsync;

#if (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON)
/* Deviation MISRAC2012-1 */
/* Deviation MISRA-1 */
  union
  {
    /** \brief Rte_Read operation on the DataServices_<data> port for boolean data **/
    Dcm_RteRead_DataServices_booleanType Data_boolean;
    /** \brief Rte_Read operation on the DataServices_<data> port for SINT16 or
     *  SINT16_N data type **/
    Dcm_RteRead_DataServices_sint16Type Data_sint16;
    /** \brief Rte_Read operation on the DataServices_<data> port for SINT32 or
     *  SINT32_N data type **/
    Dcm_RteRead_DataServices_sint32Type Data_sint32;
    /** \brief Rte_Read operation on the DataServices_<data> port for SINT8 or SINT8_N
     *  data type **/
    Dcm_RteRead_DataServices_sint8Type Data_sint8;
    /** \brief Rte_Read operation on the DataServices_<data> port for UINT16 or
     *  UINT16_N data type **/
    Dcm_RteRead_DataServices_uint16Type Data_uint16;
    /** \brief Rte_Read operation on the DataServices_<data> port for UINT32 or
     *  UINT32_N data type **/
    Dcm_RteRead_DataServices_uint32Type Data_uint32;
    /** \brief Rte_Read operation on the DataServices_<data> port for UINT8 or UINT8_N
     *  data type **/
    Dcm_RteRead_DataServices_uint8Type Data_uint8;
  } Rte_Read;

/* Deviation MISRAC2012-1 */
/* Deviation MISRA-1 */
  union
  {
    /** \brief Rte_Write operation on the DataServices_<data> port for boolean data **/
    Dcm_RteWrite_DataServices_booleanType Data_boolean;
    /** \brief Rte_Write operation on the DataServices_<data> port for SINT16 data type **/
    Dcm_RteWrite_DataServices_sint16Type Data_sint16;
    /** \brief Rte_Write operation on the DataServices_<data> port for SINT32 data type **/
    Dcm_RteWrite_DataServices_sint32Type Data_sint32;
    /** \brief Rte_Write operation on the DataServices_<data> port for SINT8 data type **/
    Dcm_RteWrite_DataServices_sint8Type Data_sint8;
    /** \brief Rte_Write operation on the DataServices_<data> port for UINT16 data type **/
    Dcm_RteWrite_DataServices_uint16Type Data_uint16;
    /** \brief Rte_Write operation on the DataServices_<data> port for UINT32 data type **/
    Dcm_RteWrite_DataServices_uint32Type Data_uint32;
    /** \brief Rte_Write operation on the DataServices_<data> port for UINT8 data type **/
    Dcm_RteWrite_DataServices_uint8Type Data_uint8;
    /** \brief Rte_Write operation on the DataServices_<data> port for SINT16_N data type **/
    Dcm_RteWrite_DataServices_sint16NType  Data_sint16N;
    /** \brief Rte_Write operation on the DataServices_<data> port for SINT32_N data type **/
    Dcm_RteWrite_DataServices_sint32NType  Data_sint32N;
    /** \brief Rte_Write operation on the DataServices_<data> port for SINT8_N data type **/
    Dcm_RteWrite_DataServices_sint8NType   Data_sint8N;
    /** \brief Rte_Write operation on the DataServices_<data> port for UINT16_N data type **/
    Dcm_RteWrite_DataServices_uint16NType  Data_uint16N;
    /** \brief Rte_Write operation on the DataServices_<data> port for UINT32_N data type **/
    Dcm_RteWrite_DataServices_uint32NType  Data_uint32N;
    /** \brief Rte_Write operation on the DataServices_<data> port for UINT8_N data type **/
    Dcm_RteWrite_DataServices_uint8NType   Data_uint8N;
  } Rte_Write;
#endif /*#if (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON)*/

  /** \brief Length of the data in bits */
  uint16                          DataLength;
#if (DCM_NVM_USAGE_REQUIRED == STD_ON)
  /** \brief Identifier of NVMBlock */
  NvM_BlockIdType                 NvRamBlockId;
#endif /* #if (DCM_NVM_USAGE_REQUIRED == STD_ON) */
  /** \brief Data type of signal */
  Dcm_DidDataByteType             DataType;
  /** \brief Data access interface type */
  Dcm_DidDataAccesType            AccessInterface;
  #if (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON)
  /** \brief Data endianness type */
  Dcm_EndiannessType              DataEndianness;
#endif
  /** \brief Indicates whether the data length is fixed */
  boolean                         FixedLength;
}
Dcm_DidDataType;

typedef struct
{
  /** \brief Contains the DcmDspData configuration */
  Dcm_DidDataType DataCfg;
  /** \brief Pointer to buffer to be used when writing the signal data */
  P2VAR(uint8, TYPEDEF, DCM_VAR) SignalBuffer;
  /** \brief Length of currently processed DID signal in bytes */
  uint16 SignalLength;
  /* !LINKSTO Dcm.Dsn.DidSignals.WriteSM,1 */
  /** \brief Current state of signal writing process */
  Dcm_WriteSignalProcessingStType ProcessingState;
  /** \brief Negative response code  */
  Dcm_NegativeResponseCodeType Nrc;
  /** \brief Parameter which transmits Operation Status */
  Dcm_OpStatusType OpStatus;
} Dcm_DidSignalWriteContextType;

typedef P2VAR(Dcm_DidSignalWriteContextType, TYPEDEF, DCM_VAR) Dcm_DidSignalWriteContextTypePtr;


typedef struct
{
  /** \brief Contains the DcmDspData configuration */
  Dcm_DidDataType DataCfg;
  /** \brief Maximum number of bytes that may be written in the buffer (for error checking) */
  uint32 BufferLimit;
  uint32 BufferOffset;
#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
  Dcm_ReadScalingSignalSizeType ReadScalingSignalLength;
#endif
  /** \brief Pointer to buffer to be used when writing the signal data */
  P2VAR(uint8, TYPEDEF, DCM_VAR) SignalBuffer;
  /** \brief Length of currently processed DID signal in bytes */
  P2VAR(uint8, TYPEDEF, DCM_VAR) OutputBuffer;
  uint16 SignalLength;
  /* signal start bit */
  uint16 SignalOffset;
  /* !LINKSTO Dcm.Dsn.DidSignals.ReadSM,1 */
  /** \brief Current state of signal reading process */
  Dcm_ReadSignalProcessingStType ProcessingState;
  /** \brief Negative response code  */
  Dcm_NegativeResponseCodeType Nrc;
  boolean EndiannessConversion;
  /** \brief Flag that indicates wether to check if the buffer is locked by an ongoing NvM
   ** acceess or not */
  boolean CheckBufferAvailability;
  /** \brief Parameter which transmits Operation Status */
  Dcm_OpStatusType OpStatus;
  boolean ConditionCheckFailed;
}
Dcm_DidSignalReadContextType;

typedef P2VAR(Dcm_DidSignalReadContextType, TYPEDEF, DCM_VAR) Dcm_DidSignalReadContextTypePtr;

/** \brief This structure indicates the position of a DID Data in Dcm_DidDataTable
 ** and describes how the elements are to be decoded  */
typedef struct
{
  /* Did Data starting index in Dcm_DidDataTable
   * If DidData is configured USE_BLOCK_ID, then this member represents the  NvMBlockId */
  uint32 DidDataPosition;
  /* Describes how the entries in Dcm_DidDataTable are to be decoded for each element */
  Dcm_DidDataDescriptorType DidDataDescriptor;
}
Dcm_DidDataIndexingArrayEntryType;

/** \brief This structure indicates the position of an signal that have the scaling enabled
 **        and the scaling info size configured to it */
typedef struct
{
  /* Did Data signal position index in DcmDspData table */
  uint16 DidDataSignalPosition;
  /* Read scaling info size */
  Dcm_ReadScalingSignalSizeType ReadScalingSignalLength;
}
Dcm_DidScalingDataSignalsIndexingArrayEntryType;

/* Deviation MISRAC2012-1 <START> */
/* Deviation MISRA-1 */
typedef union
{
  /** \brief Array for holding the read/write data for a signal
   * A union is created to ensure 32 bit alignment for the buffer for safe
   * casting from uint8* to 16/32* */
  uint32 BufferUint32[DCM_DATA_MAX_SIZE_DWORD_ARRAY];
  sint32 BufferSint32[DCM_DATA_MAX_SIZE_DWORD_ARRAY];
  uint16 BufferUint16[DCM_DATA_MAX_SIZE_WORD_ARRAY];
  sint16 BufferSint16[DCM_DATA_MAX_SIZE_WORD_ARRAY];
  sint8 BufferSint8[DCM_DATA_MAX_SIZE_BYTE_ARRAY];
  uint8 BufferUint8[DCM_DATA_MAX_SIZE_BYTE_ARRAY];
}
Dcm_Union_BufferType;
/* Deviation MISRAC2012-1 <STOP> */

/** \brief Type contains signal Buffer and the lock status */
typedef struct
{
  P2VAR(Dcm_Union_BufferType, TYPEDEF, DCM_VAR) SignalBuffer;

#if (DCM_DID_BLOCK_ID_CONFIGURED == STD_ON)
  /** \brief Indicate id of block used for reading/writing */
  NvM_BlockIdType BlockId;
  /** \brief Indicate NvM service that is keeping the buffer locked */
  Dcm_DidNvmOperationType NvMService;
#endif /* (DCM_DID_BLOCK_ID_CONFIGURED == STD_ON) */
}
Dcm_SignalBufferContextType;

/*==================[external function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Returns a pointer to internal buffer that hold signal context
 **
 ** This function must not be used outside of service processing.
 **
 ** \return Pointer to element of Dcm_SignalBufferContextType.
 */
/* Deviation MISRAC2012-1 <+2> */
extern FUNC_P2VAR(Dcm_SignalBufferContextType, AUTOMATIC, DCM_CODE) Dcm_Dsp_DidServices_GetDidSignalBuffer(void);

#if (DCM_DID_SIGNALS_COUNT > 0)

/** \brief Initialize the context of signal writing
 **
 ** \param[inout] WriteContext          Pointer to write signal context
 ** \param[in]    InputBuffer           A pointer to the buffer that contains the data to be
 **                                     written. The buffer has the data for all the signals for the
 **                                     current DID.
 ** \param[in]    BufferLength          Length of the input buffer
 ** \param[in]    DidDataIndex          The index of the current signal Dcm_DidDataIndexingArray
 **                                     array
 ** \param[in]    SignalOffset          The offset where the data of the signal is stored in the
 **                                     request buffer
 ** \param[in]    EndiannessConversion  Indicates whether endianness conversion should be performed
 **
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_Write_Init(
  Dcm_DidSignalWriteContextTypePtr WriteContext,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) InputBuffer,
  uint32 BufferLength,
  uint16 DidDataIndex,
  uint16 SignalOffset,
  boolean EndiannessConversion);

extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_Write_Execute(
  Dcm_DidSignalWriteContextTypePtr WriteContext);

extern FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_Write_Cancel(
  Dcm_DidSignalWriteContextTypePtr WriteContext);

/** \brief Initializes the context for reading signal data
 **
 ** \param[inout] ReadContext           Pointer to read signal context
 ** \param[in]    OutputBuffer          A pointer to the buffer for the output data. The signal
 **                                     will be written to the index BufferOffset
 ** \param[in]    BufferLimit           Maximum size of the buffer (in bytes)
 ** \param[in]    DidDataIndex          The index of the current signal Dcm_DidDataIndexingArray
 **                                     array
 ** \param[in]    SignalOffset          The offset where the data of the signal should be written
 **                                     within the current DID (in bits)
 ** \param[in]    BufferOffset          The offset in the OutputBuffer where the data of the signal
 **                                     should be written (in bytes)
 ** \param[in]    DidOp                 Type of the operation
 ** \param[in]    EndiannessConversion  Indicates whether endianness conversion should be performed
 **
 */
extern FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_Read_Init(
  Dcm_DidSignalReadContextTypePtr ReadContext,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) OutputBuffer,
  uint32 BufferLimit,
  uint16 DidDataIndex,
  uint16 SignalOffset,
  uint32 BufferOffset,
  Dcm_DidOpType DidOp,
  boolean EndiannessConversion);

extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_Read_Execute(
  Dcm_DidSignalReadContextTypePtr ReadContext);

extern FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_Read_Cancel(
  Dcm_DidSignalReadContextTypePtr ReadContext);

/** \brief This function is used to retrieve the configuration of a DidSignal.
 **
 ** \param[in] DidSignalIndex     Index of DidSignal
 *  \return    pDidSignalCfg      Pointer to signal configuration
*/
extern FUNC_P2CONST(Dcm_DidSignalType, AUTOMATIC, DCM_CODE) Dcm_Dsp_DidSignals_GetDidSignalConfig(
  const uint16 DidSignalIndex);

#if ((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON))

/** \brief Handles Endianness conversion of given data based on datatype
 **
 ** \param[inout] inBuffer    Pointer to source buffer
 ** \param[in]    dataLength  Length of the data to be converted
 ** \param[in]    dataType    Data type of data being processed
 **
 */
extern FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_Endianness_Conversion(
  P2VAR(uint8, AUTOMATIC, DCM_VAR) InBuffer,
  Dcm_MsgLenType DataLength,
  Dcm_DidDataByteType DataType);

#endif /* ((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON)) */

#if (DCM_NUM_DID_DATA > 0)
/** \brief The function receives a pointer to a Dcm_DidDataType element and the index in
 *  Dcm_DidDataIndexingArray and gathers all the information necessary in order to populate the
 *  intermediary Dcm_DidDataType element so that it may further be used by the rest of the unit.
 *  The function can not receive a NULL_PTR as it needs a valid pointer where to populate the
 *  DidData configuration.
 *
 *  \param DidDataPtr [in] Pointer to a variable which will be filled with the DcmDspData
 *  configuration
 *  \param DidDataIndex [in] Index of the configured DcmDspData
 *  \return void **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_GetDidDataConfig(
  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_VAR) DidDataPtr,
  uint16 DidDataIndex
);
#endif /*(DCM_NUM_DID_DATA > 0)*/

#if (DCM_DID_BLOCK_ID_CONFIGURED == STD_ON)
/** \brief This function initializes Dcm_SignalBufferContext NvM related parameters:
 ** BlockId and NvMService.
 **
 **  \return void
 **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_SignalBufferContextNvM_Init(void);

/** \brief Check if there are any ongoing NvM operations on SignalBufferContext
 **
 **  \return void
 **/
extern FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_SignalBufferOperationsPoll(void);
#endif /* (DCM_DID_BLOCK_ID_CONFIGURED == STD_ON) */
#endif /* (DCM_DID_SIGNALS_COUNT > 0) */

#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
/** \brief Search for a signal that uses read scaling data and return the scaling info size that
 **        is configured to it.
 **
 ** \param[in]  Index of the current signal in signal configuration table
 **
 ** \retval Scaling info size.
 */
extern FUNC(Dcm_ReadScalingSignalSizeType, DCM_CODE) Dcm_Dsp_DidSignals_GetScalingInfoSize(
  uint16 SignalIndex);
#endif /* #if (DCM_DSP_USE_SERVICE_0X24 == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[external constants]=========================================================*/

#if (DCM_DID_SIGNALS_COUNT > 0)

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

extern CONST(Dcm_DidSignalType, DCM_CONST) Dcm_DidSignals[DCM_DID_SIGNALS_COUNT];

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

#if ((DCM_DSP_USE_DIDSERVICES == STD_ON) && (DCM_NUM_DID_DATA > 0))

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

#if (DCM_DID_DATA_TABLE_SIZE > 0)
/** \brief Configuration of DidData function pointers */
extern CONST(Dcm_VariableFunctionPointerRecordType, DCM_CONST)
  Dcm_DidDataTable[DCM_DID_DATA_TABLE_SIZE];
#endif

/** \brief Holds the information to populate a DidDataType struct based on Dcm_DidDataTable */
extern CONST(Dcm_DidDataIndexingArrayEntryType, DCM_CONST)
  Dcm_DidDataIndexingArray[DCM_NUM_DID_DATA];

#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
#if(DCM_NUM_READSCALING_DID_DATA > 0)
/** \brief Holds the ReadScalingDataSize information for all configured DcmDspData
 *         that have scaling info size configured */
extern CONST(Dcm_DidScalingDataSignalsIndexingArrayEntryType, DCM_CONST)
  Dcm_DidScalingDataSignalsIndexingArray[DCM_NUM_READSCALING_DID_DATA];
#endif /* #if(DCM_NUM_READSCALING_DID_DATA > 0) */
#endif /* #if (DCM_DSP_USE_SERVICE_0X24 == STD_ON) */

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

#define DCM_START_SEC_CONST_16
#include <Dcm_MemMap.h>

/** \brief Holds the DataSize information for all configured DcmDspData */
extern CONST(uint16, DCM_CONST) Dcm_DidDataSizeArray[DCM_NUM_DID_DATA];

#define DCM_STOP_SEC_CONST_16
#include <Dcm_MemMap.h>

#endif /* ((DCM_DSP_USE_DIDSERVICES == STD_ON) && (DCM_NUM_DID_DATA > 0)) */
#endif /* (DCM_DID_SIGNALS_COUNT > 0) */

/*==================[external data]==============================================================*/

#endif /* ifndef DCM_DSP_DIDSIGNALS_H */
/*==================[end of file]================================================================*/
