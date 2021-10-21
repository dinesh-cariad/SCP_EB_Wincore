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

/* !LINKSTO Dcm.Dsn.File.DidSignals.Impl,1 */
/* This file contains the implementation of the Did Signals software unit. */

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 15.2 (required)
 *     An unconditional break statement shall terminate every non-empty switch clause.
 *
 *     Reason:
 *     The following actions have to be executed in the same state so the break
 *     statement is removed in order to have shorter code and avoid duplicated code.
 *
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
 *  MISRAC2012-2) Deviated Rule: 11.5 (advisory)
 *     "A conversion should not be performed from pointer to void into pointer to object"
 *
 *     Reason:
 *     The uint8 global array which is used for Read/Write Did operations is directly passed as
 *     parameter for sender-receiver interfaces which can require a different parameter type.
 *     The array is first cast to void and then to the type required by the interface.
 *
 *  MISRAC2012-3) Deviated Rule: 8.9 (advisory)
 *     "An object should be defined at block scope if its identifier only appears
 *     in a single function."
 *
 *     Reason:
 *     The variable should be declared as static in the function, but the AUTOSAR
 *     specification does not allow this based on SWS_MemMap_00018 and SWS_MemMap_00023
 *
 *  MISRAC2012-4) Deviated Rule: 16.1 (required)
 *     "All switch statements shall be well-formed"
 *
 *     Reason:
 *     This violation is present because rule 16.3 is violated and justified inside the affected
 *     switch statement, as noted under rule 16.1:
 *     some of the restrictions imposed on switch statements by this rule are expounded in the
 *     15.3, 16.2, 16.3, 16.4, 16.5, 16.6 rules, which are violated inside the current switch statement.
 *
 */
/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always false violated
 *
 * Reason:
 * Condition depends on preprocessor settings that is configuration specific. under certain configurations
 * the conditions can be either always true or always false. This does not pose any functional risk
 *
 */
/*===============================[includes]======================================================*/

#include <Dcm_Trace.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <TSAutosar.h>

#include <Dcm_Dsp_DidSignals.h>

/* The functions provided and implemented by this unit are enabled only if internal service
 * handlers which need DID services are configured. */
#if ((DCM_DID_SIGNALS_COUNT > 0) && (DCM_DSP_USE_DIDSERVICES == STD_ON))

#include <TSMem.h>                                               /* EB specific memory functions */
#include <SchM_Dcm.h>                                                  /* SchM interface for Dcm */
#include <Dcm_Dsp_DidServices.h>

#if ((DCM_DSP_USE_DIDSERVICES == STD_ON) && (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON))
#include <Dcm_Dsp_EndiannessInterpretation.h>
#endif

/*===========================[macro definitions]=================================================*/

/** \brief Signal data type is 8 bits long */
#define DCM_DIDSERVICES_SIGNAL_DATATYPE_8BITS 8U
/** \brief Signal data type is 16 bits long */
#define DCM_DIDSERVICES_SIGNAL_DATATYPE_16BITS 16U
/** \brief Signal data type is 32 bits long */
#define DCM_DIDSERVICES_SIGNAL_DATATYPE_32BITS 32U

#if (defined DCM_RTE_E_COM_BUSY)
#error DCM_RTE_E_COM_BUSY already defined
#endif /* if (defined DCM_RTE_E_COM_BUSY) */
/** \brief Dcm own definition of RTE_E_COM_BUSY since the current RTE has a
 * deviation and does not generate it */
#define DCM_RTE_E_COM_BUSY (0x8DU)

#if (defined DCM_RTE_E_SEG_FAULT)
#error DCM_RTE_E_SEG_FAULT already defined
#endif /* if (defined DCM_RTE_E_SEG_FAULT) */
/** \brief Dcm own definition of RTE_E_SEG_FAULT since the current RTE has a
 * deviation and does not generate it */
#define DCM_RTE_E_SEG_FAULT (0x88U)

#if (defined DCM_RTE_E_OUT_OF_RANGE)
#error DCM_RTE_E_OUT_OF_RANGE already defined
#endif /* if (defined DCM_RTE_E_OUT_OF_RANGE) */
/** \brief Dcm own definition of RTE_E_OUT_OF_RANGE since the current RTE has a
 * deviation and does not generate it */
#define DCM_RTE_E_OUT_OF_RANGE (0x89U)

/** \brief Macro to check if DcmDspData has Synchronous access */
#define DCM_DATA_HAS_SYNC_ACCESS(DataDescriptor) \
  ((((DataDescriptor) & DCM_USE_DATA_SYNCH_CLIENT_SERVER) == \
    DCM_USE_DATA_SYNCH_CLIENT_SERVER) || \
  (((DataDescriptor) & DCM_USE_DATA_SYNCH_FNC) == DCM_USE_DATA_SYNCH_FNC))

/** \brief Macro to check if DcmDspData has Asynchronous access */
#define DCM_DATA_HAS_ASYNC_ACCESS(DataDescriptor) \
  ((((DataDescriptor) & DCM_USE_DATA_ASYNCH_CLIENT_SERVER) == \
    DCM_USE_DATA_ASYNCH_CLIENT_SERVER) || \
  (((DataDescriptor) & DCM_USE_DATA_ASYNCH_FNC) == DCM_USE_DATA_ASYNCH_FNC))

/** \brief Macro to check if DcmDspData has ConditionCheckRead fnc enabled */
#define DCM_IS_CONDITIONCHECKREAD_AVAILABLE(DataDescriptor) \
  (((DataDescriptor) & DCM_DID_DATA_CONDITIONCHECKREAD) != FALSE)

/** \brief Macro to check if DcmDspData has ReadDataLength fnc enabled */
#define DCM_IS_READDATALENGTH_AVAILABLE(DataDescriptor) \
  (((DataDescriptor) & DCM_DID_DATA_READDATALENGTH) != FALSE)

/** \brief Macro to check if DcmDspData has ReadData fnc enabled */
#define DCM_IS_READDATA_AVAILABLE(DataDescriptor) \
  (((DataDescriptor) & DCM_DID_DATA_READDATA) != FALSE)

#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
/** \brief Macro to check if DcmDspData has ReadScalingData fnc enabled */
#define DCM_IS_READSCALINGDATA_AVAILABLE(DataDescriptor) \
  (((DataDescriptor) & DCM_DID_DATA_READSCALINGDATA) != FALSE)
#endif

/** \brief Macro to check if DcmDspData has WriteData fnc enabled */
#define DCM_IS_WRITEDATA_AVAILABLE(DataDescriptor) \
  (((DataDescriptor) & DCM_DID_DATA_WRITEDATA) != FALSE)

/** \brief Macro to check if DcmDspData has IoControl Freeze fnc enabled */
#define DCM_IS_IOCTRLFREEZE_AVAILABLE(DataDescriptor) \
  (((DataDescriptor) & DCM_DID_DATA_IOCTRLFREEZE) != FALSE)

/** \brief Macro to check if DcmDspData has Short Term Adjustment fnc enabled */
#define DCM_IS_SHORTTERMADJ_AVAILABLE(DataDescriptor) \
  (((DataDescriptor) & DCM_DID_DATA_SHORTTERMADJ) != FALSE)

/** \brief Macro to check if DcmDspData has IoControl Reset to Default fnc enabled */
#define DCM_IS_IOCTRLRESET_AVAILABLE(DataDescriptor) \
  (((DataDescriptor) & DCM_DID_DATA_IOCTRLRESET) != FALSE)

/** \brief Macro to check if DcmDspData has IoControl Return to ECU fnc enabled */
#define DCM_IS_IOCTRLRETURN_AVAILABLE(DataDescriptor) \
  (((DataDescriptor) & DCM_DID_DATA_IOCTRLRETURN) != FALSE)

/** \brief Macro to check if DcmDspData has SenderReceiver read access */
#define DCM_IS_READ_SR_AVAILABLE(DataDescriptor) \
  (((DataDescriptor) & DCM_DID_DATA_READDATA_SR) != FALSE)

/** \brief Macro to check if DcmDspData has SenderReceiver write access */
#define DCM_IS_WRITE_SR_AVAILABLE(DataDescriptor) \
  (((DataDescriptor) & DCM_DID_DATA_WRITEDATA_SR) != FALSE)

/** \brief Macro to extract Data Endianness from DidDataDescriptor (bits 5 and 6 of the second byte) */
#if (defined DCM_GET_DID_DATA_ENDIANNESS)
  #error "DCM_GET_DID_DATA_ENDIANNESS is already defined"
#endif
#define DCM_GET_DID_DATA_ENDIANNESS(DidDataDescriptor)          (((DidDataDescriptor) & 0x6000U) >> 13U)

/** \brief Macro to advance the DataCfg pointer with one index */
#define DCM_NEXT_DID_DATA_FUNC_POINTER(DataCfg) \
  ((DataCfg) = &(DataCfg)[1U])


/** \brief The number of bits to be shifted right to get the DataType from DataDescriptor */
#define DCM_DATATYPE_SHIFTS            (0x04U)
/** \brief The mask to determine DataType from DataDescriptor */
#define DCM_DATATYPE_MASK              (0x0FU)
/** \brief The mask to determine AccessType from DataDescriptor */
#define DCM_ACCESSINTERFACE_MASK       (0x07U)

/*===========================[type definitions]==================================================*/

/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/
#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

#if (DCM_NUM_DID_DATA > 0)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_InitDidDataFncPointers(
  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_VAR) DidDataPtr);

#if (DCM_DID_DATA_TABLE_SIZE > 0)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_GetDidDataSyncFuncPointers(
  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_VAR) DidDataPtr,
  uint16 DidDataIndex);

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_GetDidDataAsyncFuncPointers(
  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_VAR) DidDataPtr,
  uint16 DidDataIndex);

#if (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_GetDidDataSRPointers_Read(
  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_VAR) DidDataPtr,
  uint16 DidDataIndex);
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_GetDidDataSRPointers_Write(
  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_VAR) DidDataPtr,
  uint16 DidDataIndex,
  boolean ReadAccessPresent);
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DidSignals_ReadSignal_SenderReceiver(
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_CONST) DataCfg,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) SignalBuffer);
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DidSignals_WriteSignal_SenderReceiver(
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_CONST) DataCfg,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) WriteSignalBuffer);
#endif /* (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON) */

#endif /* (DCM_DID_DATA_TABLE_SIZE > 0) */
#endif /* (DCM_NUM_DID_DATA > 0) */

#if ((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON))
/** \brief Calculates a DID signal length if Endianness Conversion is enabled for
 ** the service ReadDataByIdentifier: 0x22 and IOControlByIdentifier: 0x2F
 **
 ** \param[in] DataConfig the Current signal Data in the table Dcm_DidDataTable[]
 **
 */
STATIC FUNC(Dcm_MsgLenType, DCM_CODE) Dcm_Dsp_DidSignals_EndianessConversion_CalculateSignalLength (
  Dcm_DidDataByteType DataType,
  Dcm_MsgLenType              DataLength);
#endif /* ((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON)) */

/** \brief For the current signal processed it checks the access type
 **        USE_DATA_SYNCH_FNC, USE_DATA_ASYNCH_FNC, USE_DATA_SYNCH_CLIENT_SERVER,
 **        USE_DATA_ASYNCH_CLIENT_SERVER. If found, the ConditionCheckRead operation is executed.
 **
 ** \param[in]   DataCfg Configuration of the signal being processed.
 ** \param[in]   OpStatus Operation state for internal Dcm Service function.
 ** \param[out]  Nrc NRC occurred due to condition checks.
 **
 ** \retval DCM_E_OK        'ConditionCheckStatus' is ok
 ** \retval DCM_E_NOT_OK    'ConditionCheckStatus' is not ok
 ** \retval DCM_E_PENDING   Operation did not complete, it must be called again.
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_ReadConditionCheck(
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_CONST)          DataCfg,
  Dcm_OpStatusType                                        OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc);

/** \brief Calls 'ConditionCheckRead' Sync operations and handles return values
 **
 ** \param[in]   DataCfg Configuration of the signal being processed
 ** \param[out]  Nrc NRC reported during condition checks.
 **
 ** \retval DCM_E_OK        'ConditionCheckRead' is ok
 ** \retval DCM_E_NOT_OK    'ConditionCheckRead' is not ok
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_ReadConditionCheckSync (
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_CONST) DataCfg,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc);

/** \brief Calls 'ConditionCheckRead' Async operations and handles return values
 **
 ** \param[in]  DataCfg Configuration of the signal being processed
 ** \param[in]  OpStatus Operation state for internal Dcm Service function.
 ** \param[out] Nrc NRC reported during condition checks.
 **
 ** \retval DCM_E_OK        'ConditionCheckRead' is ok
 ** \retval DCM_E_NOT_OK    'ConditionCheckRead' is not ok
 ** \retval DCM_E_PENDING   Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_ReadConditionCheckAsync(
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_CONST)          DataCfg,
  Dcm_OpStatusType                                        OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc);

/** \brief Reads data length of a signal mapped to a Did
 **
 ** \param[in]  DataCfg Configuration of the signal being processed
 ** \param[in]  OpStatus Operation state for internal Dcm Service function.
 ** \param[out] Nrc NRC reported by operations.
 ** \param[out] SignalLength Length of the signal in bytes returned by the readdatalength operation.
 **
 ** \retval DCM_E_OK        Reading is ok
 ** \retval DCM_E_NOT_OK    Reading is not ok
 ** \retval DCM_E_PENDING   Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_ReadDataLength
(
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_CONST)          DataCfg,
  Dcm_OpStatusType                                        OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc,
  P2VAR(uint16, AUTOMATIC, DCM_VAR)                       SignalLength
);

/** \brief Calls the read operation, and if any data is read, copies it to the response buffer.
 **
 ** \param[inout] ReadContext Context in which this operation is performed
 ** \param[out]   Nrc NRC occurred due to condition checks.
 **
 ** \retval DCM_E_OK            Data was read and copied to the signal buffer.
 ** \retval DCM_E_NOT_OK        Data was not read successfully.
 ** \retval DCM_E_PENDING       Operation did not complete, it must be called again.
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_CopyToBuffer(
  Dcm_DidSignalReadContextTypePtr ReadContext,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc);

/** \brief Handles Reading a signal for the services ReadDataByIdentifier(0x22),
           ResponseOnEvent(0x86) and InputOutputControlByIdentifier(0x2F)
 **
 ** \param[in]  DataCfg Configuration of the signal being processed
 ** \param[in]  OpStatus Operation state for internal Dcm Service function
 ** \param[out] Nrc NRC occurred due to read
 ** \param[out] SignalBuffer Pointer to where the data is read
 **
 ** \retval E_OK                Reading is ok
 ** \retval E_NOT_OK            Reading is not ok
 ** \retval DCM_E_PENDING       Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_ReadData(
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_VAR) DataCfg,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) SignalBuffer);

#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
/** \brief Handles Reading a signal for the services ReadScalingDataByIdentifier(0x24)
 **
 ** \param[in]  DataCfg Configuration of the signal being processed
 ** \param[in]  OpStatus Operation state for internal Dcm Service function
 ** \param[out] Nrc NRC occurred due to read
 ** \param[out] SignalBuffer Pointer to where the data is read
 **
 ** \retval E_OK                Reading is ok
 ** \retval E_NOT_OK            Reading is not ok
 ** \retval DCM_E_PENDING       Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_ReadScalingData(
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_VAR) DataCfg,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) SignalBuffer);
#endif /* #if (DCM_DSP_USE_SERVICE_0X24 == STD_ON) */

#if (DCM_READ_DID_BLOCK_ID_CONFIGURED == STD_ON)
/** \brief The function for reading data from NVM block
 **
 ** \param[in]  NvRamBlockId The Id of the NvM block to read from
 ** \param[in]  OpStatus Operation state for internal Dcm Service function.
 ** \param[out] Nrc
 ** \param[out] SignalBuffer Pointer to where the data is read from the NvM block
 **
 ** \retval DCM_E_OK            Reading is OK
 ** \retval DCM_E_NOT_OK        Reading is not OK
 ** \retval DCM_E_PENDING       Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_ReadNvmData(
  NvM_BlockIdType                                         NvRamBlockId,
  Dcm_OpStatusType                                        OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc,
  P2VAR(uint8, AUTOMATIC, DCM_VAR)                        SignalBuffer);
#endif /* (DCM_READ_DID_BLOCK_ID_CONFIGURED == STD_ON) */

/** \brief The function for writing data via user functions(Synchronous)
 **
 ** \param[in]  DataCfg        Configuration of signal being processed
 ** \param[in]  SignalBuffer   Holds the signal data.
 ** \param[in]  SignalLength   Signal length
 ** \param[out] Nrc
 **
 ** \retval DCM_E_OK      Operation completed successfully
 ** \retval DCM_E_NOT_OK  Operation completed with errors
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_WriteDataSync
(
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_CONST) DataCfg,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) SignalBuffer,
  uint16 SignalLength,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc);

/** \brief The function for writing data via user functions(Asynchronous)
 **
 ** \param[in] DataCfg        Configuration of signal being processed
 ** \param[in] SignalBuffer   Holds the signal data.
 ** \param[in] SignalLength   Signal length.
 ** \param[in] OpStatus       Operation status of Dcm.
 ** \param[in] Nrc
 **
 ** \retval DCM_E_OK      Operation completed successfully
 ** \retval DCM_E_NOT_OK  Operation completed with errors
 ** \retval DCM_E_PENDING Operation not completed
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_WriteDataAsync
(
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_CONST) DataCfg,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) SignalBuffer,
  uint16 SignalLength,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc);

#if (DCM_WRITE_DID_BLOCK_ID_CONFIGURED == STD_ON)
/** \brief The function for writing data to NVM block
 **
 ** \param[in]  NvRamBlockId The Id of the NvM block to write to
 ** \param[in]  OpStatus Operation state for internal Dcm Service function.
 ** \param[out] SignalBuffer Pointer from where the data is written to the NvM block
 **
 ** \retval DCM_E_OK                Write is OK
 ** \retval DCM_E_NOT_OK            Write is not OK
 ** \retval DCM_E_PENDING       Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_WriteNvmData(
  NvM_BlockIdType                    NvRamBlockId,
  Dcm_OpStatusType                   OpStatus,
  P2CONST(uint8, AUTOMATIC, DCM_VAR) SignalBuffer);
#endif /* (DCM_WRITE_DID_BLOCK_ID_CONFIGURED == STD_ON) */


#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>
/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/

#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/** \brief Buffer the Dcm uses whenever reading periodic DID signals */
/* Deviation MISRAC2012-3 */
STATIC VAR(uint8, DCM_VAR) Dcm_PeriodicDidSignalBuffer[DCM_DATA_MAX_SIZE_BYTE_ARRAY];

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>
#endif /* (DCM_DSP_USE_SERVICE_0X2A == STD_ON) */


#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#if (((DCM_DSP_USE_SERVICE_0X22 == STD_ON) || (DCM_DSP_USE_SERVICE_0X2E == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X2F == STD_ON) || (DCM_DSP_USE_SERVICE_0X24 == STD_ON) ) && \
      (DCM_DID_SIGNALS_COUNT > 0) )
  /** \brief Buffer that Dcm uses whenever reading and writing DID signals */
  /* Deviation MISRAC2012-3 */
  STATIC VAR(Dcm_SignalBufferContextType, DCM_VAR) Dcm_SignalBufferContext;
#endif

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.IB.DidSignals.GetDidSignalConfig,1 */
FUNC_P2CONST(Dcm_DidSignalType, AUTOMATIC, DCM_CODE) Dcm_Dsp_DidSignals_GetDidSignalConfig(const uint16 DidSignalIndex)
{
  P2CONST(Dcm_DidSignalType, AUTOMATIC, DCM_CONST) pDidSignalCfg = NULL_PTR;
  /* Fetch DID signal configuration. */
  pDidSignalCfg = &Dcm_DidSignals[DidSignalIndex];
  /* Return Did signal configuration. */
  return pDidSignalCfg;
}

FUNC_P2VAR(Dcm_SignalBufferContextType, AUTOMATIC, DCM_CODE) Dcm_Dsp_DidServices_GetDidSignalBuffer(void)
{
  P2VAR(Dcm_SignalBufferContextType, AUTOMATIC, DCM_VAR) pDidSignalBufferContext = NULL_PTR;

#if (((DCM_DSP_USE_SERVICE_0X22 == STD_ON) || (DCM_DSP_USE_SERVICE_0X2E == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X2F == STD_ON) || (DCM_DSP_USE_SERVICE_0X24 == STD_ON)) && \
      (DCM_DID_SIGNALS_COUNT > 0) )
  if (NULL_PTR == Dcm_SignalBufferContext.SignalBuffer)
  {
    Dcm_SignalBufferContext.SignalBuffer = Dcm_Dsp_DidServices_GetDidGeneralBuffer();
  }
  pDidSignalBufferContext = &Dcm_SignalBufferContext;
#endif

  DBG_DCM_DSP_DIDSERVICES_GETDIDSIGNALBUFFER_EXIT(pDidSignalBufferContext);

  return pDidSignalBufferContext;
}

#if (DCM_NUM_DID_DATA > 0)
/* !LINKSTO Dcm.Dsn.IB.DidSignals.GetDidDataConfig,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_GetDidDataConfig(
  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_VAR) DidDataPtr,
  uint16 DidDataIndex)
{
  const Dcm_DidDataDescriptorType
    DataDescriptor = Dcm_DidDataIndexingArray[DidDataIndex].DidDataDescriptor;

  /* GetDidDataConfig should receive a valid pointer for the DidData config to be populated */
  DCM_PRECONDITION_ASSERT(DidDataPtr != NULL_PTR, DCM_INTERNAL_API_ID);

  Dcm_Dsp_DidSignals_InitDidDataFncPointers(DidDataPtr);

  /* Get DataType */
  DidDataPtr->DataType =
    (Dcm_DidDataByteType)((DataDescriptor >> DCM_DATATYPE_SHIFTS) & DCM_DATATYPE_MASK);

  /* Get AccessInterface Type */
  DidDataPtr->AccessInterface =
    (Dcm_DidDataAccesType)(DataDescriptor & DCM_ACCESSINTERFACE_MASK);

  /* Get DataLength */
  DidDataPtr->DataLength = Dcm_DidDataSizeArray[DidDataIndex];

#if ((DCM_READ_DID_BLOCK_ID_CONFIGURED == STD_ON) || \
     (DCM_WRITE_DID_BLOCK_ID_CONFIGURED == STD_ON))
  /* Init the NvRamBlockId with 0 */
  DidDataPtr->NvRamBlockId = 0U;
#endif /* #if ((DCM_READ_DID_BLOCK_ID_CONFIGURED == STD_ON) || \
               (DCM_WRITE_DID_BLOCK_ID_CONFIGURED == STD_ON)) */

#if ((DCM_DID_DATA_TABLE_SIZE > 0) || \
     (DCM_READ_DID_BLOCK_ID_CONFIGURED == STD_ON) || \
     (DCM_WRITE_DID_BLOCK_ID_CONFIGURED == STD_ON))

  switch(DidDataPtr->AccessInterface)
  {
#if (DCM_DID_DATA_TABLE_SIZE > 0)
  /* Get necessary function pointers */

    case DCM_USE_DATA_SYNCH_CLIENT_SERVER:
    case DCM_USE_DATA_SYNCH_FNC:
    {
      Dcm_Dsp_DidSignals_GetDidDataSyncFuncPointers(DidDataPtr, DidDataIndex);
      break;
    }
    case DCM_USE_DATA_ASYNCH_CLIENT_SERVER:
    case DCM_USE_DATA_ASYNCH_FNC:
    {
      Dcm_Dsp_DidSignals_GetDidDataAsyncFuncPointers(DidDataPtr, DidDataIndex);
      break;
    }
#if (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON)
    case DCM_USE_DATA_SENDER_RECEIVER:
    {
      /* The SR interfaces pointers for a DID signal in DidDataTable are:
       * - Read Interface pointer
       * - Write Interface pointer
       * This flag indicates if the write interface pointer position
       * needs to be incremented (in case the read interface is present)
       * otherwise the write pointer is the first */
      boolean ReadAccessPresent = FALSE;

      if (DCM_IS_READ_SR_AVAILABLE(DataDescriptor))
      {
        Dcm_Dsp_DidSignals_GetDidDataSRPointers_Read(DidDataPtr, DidDataIndex);
        ReadAccessPresent = TRUE;
      }
      if (DCM_IS_WRITE_SR_AVAILABLE(DataDescriptor))
      {
        Dcm_Dsp_DidSignals_GetDidDataSRPointers_Write(
          DidDataPtr, DidDataIndex, ReadAccessPresent);
      }
      break;
    }
#endif /*#if (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON)*/

#endif /*(DCM_DID_DATA_TABLE_SIZE > 0)*/

#if ((DCM_READ_DID_BLOCK_ID_CONFIGURED == STD_ON) || \
     (DCM_WRITE_DID_BLOCK_ID_CONFIGURED == STD_ON))
    case DCM_USE_BLOCK_ID:
    {
      /* Get NvramBlockID */
      DidDataPtr->NvRamBlockId =
        (NvM_BlockIdType)Dcm_DidDataIndexingArray[DidDataIndex].DidDataPosition;
      break;
    }
#endif /* #if ((DCM_READ_DID_BLOCK_ID_CONFIGURED == STD_ON) || \
               (DCM_WRITE_DID_BLOCK_ID_CONFIGURED == STD_ON)) */

    /* CHECK: NOPARSE */
    default:
    {
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
    }
    break;
    /* CHECK: PARSE */
  }
#endif /*((DCM_DID_DATA_TABLE_SIZE > 0) || \
     (DCM_READ_DID_BLOCK_ID_CONFIGURED == STD_ON) || \
     (DCM_WRITE_DID_BLOCK_ID_CONFIGURED == STD_ON))*/

  /* Get Length type */
  /* Data with SENDER RECEIVER access always has fixed length. This is insured via xdm checks */
  if ((DataDescriptor & DCM_DID_DATA_FIXED_LENGTH) != FALSE)
  {
    DidDataPtr->FixedLength = TRUE;
  }
  else
  {
    DidDataPtr->FixedLength = FALSE;
  }

#if (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON)
  /* Get the Endianness of the signal */
  DidDataPtr->DataEndianness = DCM_GET_DID_DATA_ENDIANNESS(DataDescriptor);
#endif
}

#endif /* (DCM_NUM_DID_DATA > 0) */

#if (DCM_DID_BLOCK_ID_CONFIGURED == STD_ON)

FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_SignalBufferContextNvM_Init(void)
{
  P2VAR(Dcm_SignalBufferContextType, AUTOMATIC, DCM_VAR) pDidSignalBufferContext =
      Dcm_Dsp_DidServices_GetDidSignalBuffer();

  pDidSignalBufferContext->BlockId = 0U;
  pDidSignalBufferContext->NvMService = DCM_DIDNVMOPERATION_NONE;
}

FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_SignalBufferOperationsPoll(void)
{
  /* Indicate NvM service that is keeping the buffer locked */
  NvM_BlockIdType LocalSigBufBlockId;
  /* Local copy of BlockId structure member of global variable Dcm_SignalBufferContext. */
  Dcm_DidNvmOperationType LocalSigBufNvMService;

  /* Flag indicating whether the signal buffer has to be released */
  boolean FreeSignalBuffer = FALSE;

  P2VAR(Dcm_SignalBufferContextType, AUTOMATIC, DCM_VAR) pDidSignalBufferContext =
      Dcm_Dsp_DidServices_GetDidSignalBuffer();

/*   Cache BlockId and NvMService values from global variable Dcm_SignalBufferContext
     into local variables to minimize the time spent in critical sections.*/

#if (DCM_ASYNC_DID_SERVICES == STD_ON)

  /* ENTER critical section

     Only use critical sections if either of services 0x22 or 0x2E or 0x2F use an Async
     service handler, for performance reasons. Only a preemption by that handler can
     have an impact on the Dcm_SignalBufferContext. */

  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
#endif /* #if (DCM_ASYNC_DID_SERVICES == STD_ON) */

  LocalSigBufBlockId = pDidSignalBufferContext->BlockId;
  LocalSigBufNvMService = pDidSignalBufferContext->NvMService;

#if (DCM_ASYNC_DID_SERVICES == STD_ON)
  /* LEAVE critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
#endif /* #if (DCM_ASYNC_DID_SERVICES == STD_ON) */

  /* if the signal buffer is locked by an NvM operation */
  if (LocalSigBufNvMService != DCM_DIDNVMOPERATION_NONE)
  {
    NvM_RequestResultType NvMRequestResult = NVM_REQ_NOT_OK;
    /* check result of the last NvM operation */
    /* !LINKSTO Dcm.EB.MainFunction.ReleaseBuffer,1 */
    Std_ReturnType NvMOperationStatus = NvM_GetErrorStatus(LocalSigBufBlockId, &NvMRequestResult);

    if (NvMOperationStatus == E_OK)
    {
      if (NvMRequestResult != NVM_REQ_PENDING)
      {
        /* NvM operation completed, release SignalBuffer */
        FreeSignalBuffer = TRUE;
      }
    }
    else
    {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      if (NvMOperationStatus != E_NOT_OK)
      {
        /* !LINKSTO Dcm.EB.MainFunction.ReleaseBuffer.NvMRead.InvalidValue,1 */
        /* !LINKSTO Dcm.EB.MainFunction.ReleaseBuffer.NvMWrite.InvalidValue,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_NVMGETERRORSTATUS, DCM_E_INTERFACE_RETURN_VALUE);
      }

      if (LocalSigBufNvMService == DCM_DIDNVMOPERATION_READNVMDATA)
      {
        /* !LINKSTO Dcm.EB.MainFunction.ReleaseBuffer.NvMRead.Failed,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_NVMREADDATA, DCM_E_NVM_READ_FAIL);
      }
      else
      {
        DCM_PRECONDITION_ASSERT((LocalSigBufNvMService == DCM_DIDNVMOPERATION_WRITENVMDATA),
                                DCM_SERVID_MAINFUNCTION);
        /* !LINKSTO Dcm.EB.MainFunction.ReleaseBuffer.NvMWrite.Failed,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_NVMWRITEDATA, DCM_E_NVM_WRITE_FAIL);
      }
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */

      /* NvM_GetErrorStatus() failed, releasing SignalBuffer to prevent
         Dcm DID services from remaining locked  */
      FreeSignalBuffer = TRUE;
    }
  }
  if (FreeSignalBuffer == TRUE)
  {
    if (LocalSigBufNvMService == DCM_DIDNVMOPERATION_WRITENVMDATA)
    {
      /* !LINKSTO Dcm.EB.WriteNvmData.CancelJobs.LockBlock,1 */
      /* Lock back the NVRAM block */
      NvM_SetBlockLockStatus(LocalSigBufBlockId, TRUE);
    }

#if (DCM_ASYNC_DID_SERVICES == STD_ON)
    /* ENTER critical section

     if the signal buffer needs to be freed, it must be done in
     a critical section, to prevent preemption*/

    SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
#endif /* if (DCM_ASYNC_DID_SERVICES == STD_ON) */

    /* Free the signal buffer */
    pDidSignalBufferContext->BlockId = 0U;
    pDidSignalBufferContext->NvMService = DCM_DIDNVMOPERATION_NONE;
#if (DCM_ASYNC_DID_SERVICES == STD_ON)
    /* LEAVE critical section */
    SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
#endif /* if (DCM_ASYNC_DID_SERVICES == STD_ON) */
  }
}
#endif /* (DCM_DID_BLOCK_ID_CONFIGURED == STD_ON) */

#if ((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON))
/* !LINKSTO Dcm.Dsn.DidServices.IB.EndianessConversion,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_Endianness_Conversion(
  P2VAR(uint8, AUTOMATIC, DCM_VAR) InBuffer,
  Dcm_MsgLenType                           DataLength,
  Dcm_DidDataByteType              DataType)
{
  uint16_least ByteCounter;

  DBG_DCM_DSP_DIDSIGNALS_ENDIANNESS_CONVERSION_ENTRY(InBuffer, DataLength, DataType);

  if (InBuffer != NULL_PTR)
  {
    switch (DataType)
    {
      /* if the data type is SINT16 or UINT16 */
      case DCM_TYPE_SINT16:
      case DCM_TYPE_UINT16:
      {
        for (ByteCounter = 0U; ByteCounter < DataLength; ByteCounter += 2U)
        {
          /* the assignment is byte-wise, do not rely on a
           * specific alignment */
          const uint8 TmpByte = InBuffer[ByteCounter];

          InBuffer[ByteCounter]      = InBuffer[ByteCounter + 1U];
          InBuffer[ByteCounter + 1U] = TmpByte;
        }
      }
      break;

      /* if the data type is SINT32 ot UINT32 */
      case DCM_TYPE_SINT32:
      case DCM_TYPE_UINT32:
      {
        for (ByteCounter = 0U; ByteCounter < DataLength; ByteCounter += 4U)
        {
          /* the assignment is byte-wise, do not rely on a
           * specific alignment */
          uint8 TmpByte;

          TmpByte = InBuffer[ByteCounter + 3U];
          InBuffer[ByteCounter + 3U] = InBuffer[ByteCounter];
          InBuffer[ByteCounter]      = TmpByte;

          TmpByte = InBuffer[ByteCounter + 2U];
          InBuffer[ByteCounter + 2U] = InBuffer[ByteCounter + 1U];
          InBuffer[ByteCounter + 1U] = TmpByte;
        }
      }
      break;

      default:
      {
        /* do nothing */
      }
      break;
    }
  }

  DBG_DCM_DSP_DIDSIGNALS_ENDIANNESS_CONVERSION_EXIT(InBuffer, DataLength, DataType);
}
#endif /* ((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON)) */

/* !LINKSTO Dcm.Dsn.IB.DidSignals.ReadInit,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_Read_Init(
  Dcm_DidSignalReadContextTypePtr ReadContext,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) OutputBuffer,
  uint32 BufferLimit,
  uint16 DidDataIndex,
  uint16 SignalOffset,
  uint32 BufferOffset,
  Dcm_DidOpType DidOp,
  boolean EndiannessConversion
  )
{
  CONSTP2VAR(Dcm_DidDataType, AUTOMATIC, DCM_VAR) DataCfg = &(ReadContext->DataCfg);

  /* Get DcmDspData configuration */
  Dcm_Dsp_DidSignals_GetDidDataConfig(DataCfg, DidDataIndex);

  ReadContext->OpStatus = DCM_INITIAL;
#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
  if (DidOp == DCM_DID_OP_READSCALING)
  {
    ReadContext->ReadScalingSignalLength = DCM_GET_BITS(Dcm_Dsp_DidSignals_GetScalingInfoSize(DidDataIndex));
    ReadContext->ProcessingState = DCM_SIGNALREAD_STATE_READSCALING;
    ReadContext->EndiannessConversion = FALSE;
  }
  else
#endif
  {
    ReadContext->ProcessingState = DCM_SIGNALREAD_STATE_INIT;
    ReadContext->EndiannessConversion = EndiannessConversion;
  }
  ReadContext->BufferLimit = BufferLimit;
  ReadContext->OutputBuffer = OutputBuffer;
  ReadContext->SignalLength = 0U;
  ReadContext->BufferOffset = BufferOffset;
  ReadContext->ConditionCheckFailed = FALSE;
  ReadContext->Nrc = E_OK;

#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
  if (DCM_DID_OP_PERIODIC == DidOp)
  {
    /* !LINKSTO Dcm.Dsn.DidSignals.SignalBufferConsistency,1 */
    ReadContext->SignalBuffer = Dcm_PeriodicDidSignalBuffer;
    ReadContext->CheckBufferAvailability = FALSE;
  }
  else
#else
  TS_PARAM_UNUSED(DidOp);
#endif
  {
    P2VAR(Dcm_SignalBufferContextType, AUTOMATIC, DCM_VAR) pDidSignalBufferContext =
      Dcm_Dsp_DidServices_GetDidSignalBuffer();
    ReadContext->SignalBuffer = pDidSignalBufferContext->SignalBuffer->BufferUint8;
    ReadContext->CheckBufferAvailability = TRUE;
  }

  ReadContext->SignalOffset = SignalOffset;
}

/* !LINKSTO Dcm.Dsn.IB.DidSignals.ReadCancel,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_Read_Cancel(
  Dcm_DidSignalReadContextTypePtr ReadContext)
{
  /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.AsyncFnc.Pending.Cancel,1 */
  /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.AsyncCS.Pending.Cancel,1 */
  /* !LINKSTO Dcm.ReadDataById.ReadDataLength.AsyncFnc.Pending.Cancel,1 */
  /* !LINKSTO Dcm.ReadDataById.ReadDataLength.AsyncCS.Pending.Cancel,1 */
  /* !LINKSTO Dcm.ReadDataById.ReadData.AsyncFnc.Pending.Cancel,1 */
  /* !LINKSTO Dcm.ReadDataById.ReadData.AsyncCS.Pending.Cancel,1 */
  ReadContext->OpStatus = DCM_CANCEL;
  /* For the CANCEL operation the return value is not used */
  (void)Dcm_Dsp_DidSignals_Read_Execute(ReadContext);
}

/* !LINKSTO Dcm.Dsn.IB.DidSignals.ReadExecute,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_Read_Execute(
  Dcm_DidSignalReadContextTypePtr ReadContext)
{
  Std_ReturnType RetVal = DCM_E_NOT_OK;
  Dcm_NegativeResponseCodeType Nrc = E_OK;
  /* holds the configuration of signal being processed */
  CONSTP2VAR(Dcm_DidDataType, AUTOMATIC, DCM_VAR) DataCfg = &(ReadContext->DataCfg);
  /* Deviation MISRAC2012-4 */
  switch (ReadContext->ProcessingState)
  {
    /* Deviation MISRAC2012-1 */
    /* Deviation MISRA-1 */
    case DCM_SIGNALREAD_STATE_INIT:
    {
      ReadContext->ProcessingState = DCM_SIGNALREAD_STATE_CONDITIONCHECK;
    }
    /* no break - the processing can go to the next state in the same MainFunction call */
    /* Deviation MISRAC2012-1 */
    /* Deviation MISRA-1 */
    case DCM_SIGNALREAD_STATE_CONDITIONCHECK:
    {
      RetVal = Dcm_Dsp_DidSignals_ReadConditionCheck(DataCfg, ReadContext->OpStatus, &Nrc);
      if (RetVal != DCM_E_OK)
      {
        if (RetVal == DCM_E_NOT_OK)
        {
          /* This branch is executed also for the CANCEL case of an Asynch operation. If the
             operation was not cancelled, Nrc is set from the ConditionCheck operation */
          ReadContext->ConditionCheckFailed = TRUE;
          /* If the processing is stopped, invalidate the state machine */
          ReadContext->ProcessingState = DCM_SIGNALREAD_STATE_INVALID;
        }
        else
        {
          /* Retval must be DCM_E_PENDING */
          DCM_PRECONDITION_ASSERT(DCM_E_PENDING == RetVal, DCM_INTERNAL_API_ID);
          /* maintain the current OpStatus */
        }
        /* Either an error occurred or an asynchronous operation returned a PENDING status */
        break;
      }
      else
      {
        /* Condition check operation completed, go to next state */
        ReadContext->ProcessingState = DCM_SIGNALREAD_STATE_DATALENGTH;
        ReadContext->OpStatus = DCM_INITIAL;
      }
    }
    /* no break - the processing can go to the next state in the same MainFunction call. If the
                  return value from this state was PENDING or error, a break was used to jump out */
    /* Deviation MISRAC2012-1 */
    /* Deviation MISRA-1 */
    case DCM_SIGNALREAD_STATE_DATALENGTH:
    {
      if (((DataCfg->AccessInterface == DCM_USE_DATA_ASYNCH_FNC) ||
           (DataCfg->AccessInterface == DCM_USE_DATA_ASYNCH_CLIENT_SERVER) ||
           (DataCfg->AccessInterface == DCM_USE_DATA_SYNCH_FNC) ||
           (DataCfg->AccessInterface == DCM_USE_DATA_SYNCH_CLIENT_SERVER)) &&
          (DataCfg->FixedLength == FALSE))
      {
        /* For variable length signals, it is needed to obtain the length from the application */
        /* !LINKSTO Dcm.ReadDataById.ReadDataLength.AsyncFnc.Pending.Cancel,1 */
        /* !LINKSTO Dcm.ReadDataById.ReadDataLength.AsyncCS.Pending.Cancel,1 */
        RetVal = Dcm_Dsp_DidSignals_ReadDataLength(
          DataCfg,
          ReadContext->OpStatus,
          &Nrc,
          &ReadContext->SignalLength);

        if (DCM_CANCEL == ReadContext->OpStatus)
        {
          /* If the OpStatus was CANCEL, ignore the return value, and set the state to invalid */
          ReadContext->ProcessingState = DCM_SIGNALREAD_STATE_INVALID;
          break;
        }

        if (RetVal != DCM_E_OK)
        {
          if (RetVal != DCM_E_PENDING)
          {
            /* The NRC is set by the Dcm_Dsp_DidSignals_ReadDataLength function (this branch
               shall be reached only if DET is enabled) */
            RetVal = DCM_E_NOT_OK;
            /* If the processing is stopped, invalidate the state machine */
            ReadContext->ProcessingState = DCM_SIGNALREAD_STATE_INVALID;
          }
          else
          {
            /* RetVal == DCM_E_PENDING */
            /* maintain the current OpStatus */
          }
          break;
        }
        else
        {
          /* Data length operation completed, go to next state */
          ReadContext->ProcessingState = DCM_SIGNALREAD_STATE_READ;
          ReadContext->OpStatus = DCM_INITIAL;
        }
      }
      else
      {
        DCM_PRECONDITION_ASSERT(((DataCfg->AccessInterface == DCM_USE_BLOCK_ID) ||
               (DataCfg->AccessInterface == DCM_USE_DATA_SENDER_RECEIVER) ||
               (((DataCfg->AccessInterface == DCM_USE_DATA_ASYNCH_FNC) ||
                 (DataCfg->AccessInterface == DCM_USE_DATA_ASYNCH_CLIENT_SERVER) ||
                 (DataCfg->AccessInterface == DCM_USE_DATA_SYNCH_FNC) ||
                 (DataCfg->AccessInterface == DCM_USE_DATA_SYNCH_CLIENT_SERVER)) &&
                (DataCfg->FixedLength == TRUE))), DCM_INTERNAL_API_ID);
        /* For NvM signals, S/R signals and fixed length signals, obtain the signal length from
           the configuration data */
        ReadContext->SignalLength = DataCfg->DataLength;
      }
#if ((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON))
      /* The legacy endianness conversion does not apply to SR interfaces */
      /* If the conversion is enabled, update the signal length */
      if ((ReadContext->EndiannessConversion == TRUE) &&
          (DataCfg->AccessInterface != DCM_USE_DATA_SENDER_RECEIVER))
      {
        /* !LINKSTO Dcm.EB.EndianessConversion.TRUE.ReadDataById.SignalLength.DataTypeLimit,1 */
        /* !LINKSTO Dcm.EB.EndianessConversion.TRUE.ReadDataById.SignalLength.NoDataTypeLimit,1 */
        ReadContext->SignalLength =
          (uint16)(Dcm_Dsp_DidSignals_EndianessConversion_CalculateSignalLength(
            DataCfg->DataType, ReadContext->SignalLength));
      }
#endif /* ((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON)) */
    }
    /* no break - the processing can go to the next state in the same MainFunction call. If the
                  return value from this state was PENDING or error, a break was used to jump out */
    case DCM_SIGNALREAD_STATE_READ:
#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
    case DCM_SIGNALREAD_STATE_READSCALING:
#endif
    {
#if (DCM_DID_BLOCK_ID_CONFIGURED == STD_ON)
      P2VAR(Dcm_SignalBufferContextType, AUTOMATIC, DCM_VAR) pDidSignalBufferContext =
          Dcm_Dsp_DidServices_GetDidSignalBuffer();
      /* Check if the signal buffer is available */
      if ((ReadContext->CheckBufferAvailability == FALSE) ||
          ((ReadContext->CheckBufferAvailability == TRUE) &&
           (pDidSignalBufferContext->NvMService == DCM_DIDNVMOPERATION_NONE)))
#endif /* (DCM_DID_BLOCK_ID_CONFIGURED == STD_ON) */
      {
        RetVal = Dcm_Dsp_DidSignals_CopyToBuffer(ReadContext, &Nrc);
      }
#if (DCM_DID_BLOCK_ID_CONFIGURED == STD_ON)
      else
      {
        /* Signal buffer is busy, return NRC */
        /* !LINKSTO Dcm.EB.ReadDataByIdentifier.CONDITIONSNOTCORRECT,1 */
        /* !LINKSTO Dcm.EB.UDSServices.ReadScalingDataByIdentifierServiceRequest.ReadDataByIdCanceled.SignalBufferLocked.TransmitNRC0x22,1 */
        /* !LINKSTO Dcm.EB.UDSServices.ReadScalingDataByIdentifierServiceRequest.WriteDataByIdCanceled.SignalBufferLocked.TransmitNRC0x22,1 */
        Nrc   = DCM_E_CONDITIONSNOTCORRECT;
        RetVal = DCM_E_NOT_OK;
        /* If the processing is stopped, invalidate the state machine */
        ReadContext->ProcessingState = DCM_SIGNALREAD_STATE_INVALID;
      }
#endif /* #if (DCM_DID_BLOCK_ID_CONFIGURED == STD_ON) */
    }
    break;
    /* CHECK: NOPARSE */
    default:
    {
      /* Defensive branch. Default case at the end of the switch statement
         Also covers DCM_SIGNALREAD_STATE_INVALID  */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
    }
    break;
    /* CHECK: PARSE */
  }

  if (RetVal == DCM_E_PENDING)
  {
    /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.AsyncFnc.RetVal.Pending,1 */
    /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.AsyncCS.RetVal.Pending,1 */
    /* !LINKSTO Dcm.ReadDataById.ReadDataLength.AsyncFnc.RetVal.Pending,1 */
    /* !LINKSTO Dcm.ReadDataById.ReadDataLength.AsyncCS.RetVal.Pending,1 */
    /* !LINKSTO Dcm.ReadDataById.ReadData.AsyncFnc.RetVal.Pending,1 */
    /* !LINKSTO Dcm.ReadDataById.ReadData.AsyncCS.RetVal.Pending,1 */
    ReadContext->OpStatus = DCM_PENDING;
  }
  ReadContext->Nrc = Nrc;

  return RetVal;
}

/* !LINKSTO Dcm.Dsn.IB.DidSignals.WriteInit,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_Write_Init(
  Dcm_DidSignalWriteContextTypePtr WriteContext,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) InputBuffer,
  uint32 BufferLength,
  uint16 DidDataIndex,
  uint16 SignalOffset,
  boolean EndiannessConversion)
{
  Std_ReturnType RetVal = DCM_E_NOT_OK;

  P2VAR(Dcm_SignalBufferContextType, AUTOMATIC, DCM_VAR) pDidSignalBufferContext =
      Dcm_Dsp_DidServices_GetDidSignalBuffer();

#if (DCM_DID_BLOCK_ID_CONFIGURED == STD_ON)
  /* Check if the signal buffer is available */
  if (pDidSignalBufferContext->NvMService == DCM_DIDNVMOPERATION_NONE)
#endif /* (DCM_DID_BLOCK_ID_CONFIGURED == STD_ON) */
  {
    /* not used for fixed length signals */
    uint16 SignalLengthInBytes = 0U;
    Dcm_MsgLenType SignalLength = 0U;
    CONSTP2VAR(Dcm_DidDataType, AUTOMATIC, DCM_VAR) DataCfg = &(WriteContext->DataCfg);

    /* The DidCheckLen guarantees that the buffer size is less the uint16 */
    DCM_PRECONDITION_ASSERT(BufferLength < DCM_UINT16_MAX, DCM_INTERNAL_API_ID);

    /* Get DcmDspData configuration */
    Dcm_Dsp_DidSignals_GetDidDataConfig(DataCfg, DidDataIndex);

    WriteContext->SignalBuffer = pDidSignalBufferContext->SignalBuffer->BufferUint8;

    TS_MemBZero(WriteContext->SignalBuffer, DCM_DATA_MAX_SIZE_BYTE_ARRAY);

    /* Calculate Signal Length */
    if (DataCfg->FixedLength == TRUE)
    {
      SignalLength = DataCfg->DataLength;
    }
    else
    {
      /* Length of variable signal taking into account the starting position in the
       * request message */
      SignalLength = DCM_GET_BITS(BufferLength) - SignalOffset;

      /* Ignore superfluous bits */
      if (SignalLength > DataCfg->DataLength)
      {
        SignalLength = DataCfg->DataLength;
      }
    }

#if (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON)
    if (DataCfg->AccessInterface != DCM_USE_DATA_SENDER_RECEIVER)
#endif
    {
      /* Copy the data of currently processed signal to WriteSignalBuffer */
      Dcm_DspInternal_CopySignalData(
        InputBuffer,
        WriteContext->SignalBuffer,
        SignalOffset,
        0U,
        SignalLength,
        DCM_COPY_FROM_INBUFFER_OPAQUE,
        DCM_BYTE_DIRECTION_INCREMENT);
    }
#if (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON)
    else
    {
      /* !LINKSTO SWS_Dcm_00639_USE_DATA_SENDER_RECEIVER,1 */
      /* !LINKSTO SWS_Dcm_00639_USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
      Dcm_Dsp_EndiannessInterpretation_CopyData(InputBuffer,
                                       DCM_ENDIANNESSINTERPRETATION_COPY_DATA_FROM_IN_BUFFER,
                                       WriteContext->SignalBuffer,
                                       DataCfg->DataEndianness,
                                       DataCfg->DataType,
                                       DataCfg->DataLength,
                                       SignalOffset);
    }
#endif

#if ((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON))
    if ( ((DataCfg->DataType == DCM_TYPE_SINT16) || (DataCfg->DataType == DCM_TYPE_UINT16) ||
          (DataCfg->DataType == DCM_TYPE_SINT32) || (DataCfg->DataType == DCM_TYPE_UINT32)) &&
         (EndiannessConversion == TRUE) &&
         (DataCfg->AccessInterface != DCM_USE_DATA_SENDER_RECEIVER))
    {
      /* !LINKSTO Dcm.EB.EndianessConversion.TRUE.WriteDataById.SignalLength.DataTypeLimit,1 */
      /* !LINKSTO Dcm.EB.EndianessConversion.TRUE.WriteDataById.SignalLength.NoDataTypeLimit,1 */
      SignalLength =
        Dcm_Dsp_DidSignals_EndianessConversion_CalculateSignalLength(
          DataCfg->DataType, SignalLength);

      /* !LINKSTO Dcm.EB.EndianessConversion.TRUE.WriteDataById.SignalData.INT16,1 */
      /* !LINKSTO Dcm.EB.EndianessConversion.TRUE.WriteDataById.SignalData.INT32,1 */
      Dcm_Dsp_DidSignals_Endianness_Conversion(
        WriteContext->SignalBuffer,
        (uint16)DCM_GET_BYTES(SignalLength),
        DataCfg->DataType);
    }
#else
  TS_PARAM_UNUSED(EndiannessConversion);
#endif /* ((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON)) */

    SignalLengthInBytes = (uint16)DCM_GET_BYTES(SignalLength);

    WriteContext->SignalLength = SignalLengthInBytes;
    WriteContext->OpStatus = DCM_INITIAL;
    WriteContext->ProcessingState = DCM_SIGNALWRITE_STATE_INIT;
    WriteContext->Nrc = DCM_E_OK;

    RetVal = DCM_E_OK;
  }
#if (DCM_DID_BLOCK_ID_CONFIGURED == STD_ON)
  else
  {
    RetVal = DCM_E_NOT_OK;
    /* !LINKSTO Dcm.EB.WriteDataByIdentifier.CONDITIONSNOTCORRECT,1 */
    WriteContext->Nrc = DCM_E_CONDITIONSNOTCORRECT;
  }
#endif /* #if (DCM_DID_BLOCK_ID_CONFIGURED == STD_ON) */

  return RetVal;
}

/* !LINKSTO Dcm.Dsn.IB.DidSignals.WriteCancel,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_Write_Cancel(
  Dcm_DidSignalWriteContextTypePtr WriteContext)
{
  /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncFnc.FixLength.Pending.Cancel,1 */
  /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncFnc.VarLength.Pending.Cancel,1 */
  /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncCS.FixLength.Pending.Cancel,1 */
  /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncCS.VarLength.Pending.Cancel,1 */
  WriteContext->OpStatus = DCM_CANCEL;
  /* For the CANCEL operation the return value is not used */
  (void)Dcm_Dsp_DidSignals_Write_Execute(WriteContext);
}

/* !LINKSTO Dcm.Dsn.IB.DidSignals.WriteExecute,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_Write_Execute(
  Dcm_DidSignalWriteContextTypePtr WriteContext
)
{
  Std_ReturnType RetVal = DCM_E_NOT_OK;
  Dcm_NegativeResponseCodeType Nrc = E_OK;

  /* Deviation MISRAC2012-4 */
  switch (WriteContext->ProcessingState)
  {
    /* Deviation MISRAC2012-1 */
    /* Deviation MISRA-1 */
    case DCM_SIGNALWRITE_STATE_INIT:
    {
      WriteContext->ProcessingState = DCM_SIGNALWRITE_STATE_WRITE;
    }
    /* no break - the processing can go to the next state in the same MainFunction call. */
    case DCM_SIGNALWRITE_STATE_WRITE:
    {
      switch ((WriteContext->DataCfg).AccessInterface)
      {
#if (DCM_WRITE_DID_BLOCK_ID_CONFIGURED == STD_ON)
        /* if the data needs to be written via NVRAM interface */
        case DCM_USE_BLOCK_ID:
        {
          RetVal = Dcm_Dsp_DidSignals_WriteNvmData(
                     WriteContext->DataCfg.NvRamBlockId,
                     WriteContext->OpStatus,
                     WriteContext->SignalBuffer);

          if ((RetVal == DCM_E_OK) || (RetVal == DCM_E_PENDING))
          {
            /* Nothing to do */
          }
          else
          {
            /* !LINKSTO Dcm.WriteDataById.NvM.TriggerNrc.Error,1 */
            Nrc = DCM_E_GENERALPROGRAMMINGFAILURE;
          }
        }
        break;
#endif /* (DCM_WRITE_DID_BLOCK_ID_CONFIGURED == STD_ON) */

        case DCM_USE_DATA_SYNCH_FNC:
        case DCM_USE_DATA_SYNCH_CLIENT_SERVER:
        {
          RetVal = Dcm_Dsp_DidSignals_WriteDataSync(
                      &(WriteContext->DataCfg),
                      WriteContext->SignalBuffer,
                      WriteContext->SignalLength,
                      &Nrc);
        }
        break;

        /* !LINKSTO Dcm.EB.WriteDataByIdentifier.NRC.0x31.DataRecord.Valid,1 */
        case DCM_USE_DATA_ASYNCH_FNC:
        case DCM_USE_DATA_ASYNCH_CLIENT_SERVER:
        {
          RetVal = Dcm_Dsp_DidSignals_WriteDataAsync(
                      &(WriteContext->DataCfg),
                      WriteContext->SignalBuffer,
                      WriteContext->SignalLength,
                      WriteContext->OpStatus,
                      &Nrc);
        }
        break;

#if (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON)
        case DCM_USE_DATA_SENDER_RECEIVER:
        {
          Std_ReturnType WriteSRResult = DCM_E_NOT_OK;

          WriteSRResult = Dcm_DidSignals_WriteSignal_SenderReceiver(
                            &(WriteContext->DataCfg),
                            WriteContext->SignalBuffer);

          if (DCM_E_OK != WriteSRResult)
          {
            /* !LINKSTO Dcm.WriteDataById.WriteData.SR.RetVal.ConditionsNotCorrect,1 */
            /* !LINKSTO Dcm.WriteDataById.WriteData.SRS.RetVal.ConditionsNotCorrect,1 */
            /* !LINKSTO Dcm.WriteDataById.WriteData.SR.RetVal.OutOfRange.Nrc,1 */
            /* !LINKSTO Dcm.WriteDataById.WriteData.SRS.RetVal.OutOfRange.Nrc,1 */
            RetVal = DCM_E_NOT_OK;
            Nrc = WriteSRResult;
          }
          else
          {
            RetVal = DCM_E_OK;
          }
        }
        break;
#endif /* (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON) */

        /* CHECK: NOPARSE */
        case DCM_USE_ECU_SIGNAL:
        {
          /* ECU signal writing is not supported currently */
          DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
          Nrc    = DCM_E_GENERALREJECT;
          RetVal = DCM_E_NOT_OK;
        }
        break;
        /* CHECK: PARSE */

        /* CHECK: NOPARSE */
        default:
        {
          /* Defensive branch. Default case at the end of the switch statement */
          DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
        }
        break;
        /* CHECK: PARSE */
      }
    }
    break;
    /* CHECK: NOPARSE */
    default:
    {
      /* Defensive branch. Default case at the end of the switch statement
         Also covers DCM_SIGNALWRITE_STATE_INVALID  */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
    }
    break;
    /* CHECK: PARSE */
  }

  if ((RetVal == DCM_E_PENDING) && (DCM_CANCEL != WriteContext->OpStatus))
  {
    /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncFnc.FixLength.RetVal.Pending,1 */
    /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncFnc.VarLength.RetVal.Pending,1 */
    /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncCS.FixLength.RetVal.Pending,1 */
    /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncCS.VarLength.RetVal.Pending,1 */
    WriteContext->OpStatus = DCM_PENDING;
  }
  else
  {
    /* If the write has finished with ok, or error, calling again the function will result
       in an error */
    WriteContext->ProcessingState = DCM_SIGNALWRITE_STATE_INVALID;
  }
  WriteContext->Nrc = Nrc;

  return RetVal;
}

/*==================[internal function definitions]==============================================*/

#if (DCM_NUM_DID_DATA > 0)

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_InitDidDataFncPointers(
  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_VAR) DidDataPtr)
{
  DidDataPtr->ConditionCheckReadSync                                               = NULL_PTR;
  DidDataPtr->ConditionCheckReadAsync                                              = NULL_PTR;
  DidDataPtr->ReadDataLengthSync                                                   = NULL_PTR;
  DidDataPtr->ReadDataLengthAsync                                                  = NULL_PTR;
  DidDataPtr->ReadDataFncSync                                                      = NULL_PTR;
  DidDataPtr->ReadDataFncAsync                                                     = NULL_PTR;
#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
  DidDataPtr->ReadScalingDataFncSync                                               = NULL_PTR;
  DidDataPtr->ReadScalingDataFncAsync                                              = NULL_PTR;
#endif /* #if (DCM_DSP_USE_SERVICE_0X24 == STD_ON) */
  DidDataPtr->WriteFixedDataSync                                                   = NULL_PTR;
  DidDataPtr->WriteVariableDataSync                                                = NULL_PTR;
  DidDataPtr->WriteFixedLengthDataAsync                                            = NULL_PTR;
  DidDataPtr->WriteVarLengthDataAsync                                              = NULL_PTR;
  DidDataPtr->ResultWriteData                                                      = NULL_PTR;
  DidDataPtr->FreezeCurrentStateSyncUnion.FreezeCurrentStateSync                   = NULL_PTR;
  DidDataPtr->FreezeCurrentStateAsync                                              = NULL_PTR;
  DidDataPtr->ShortTermAdjustmentFixedSyncUnion.ShortTermAdjustmentFixedSync       = NULL_PTR;
  DidDataPtr->ShortTermAdjustmentVariableSyncUnion.ShortTermAdjustmentVariableSync = NULL_PTR;
  DidDataPtr->ShortTermAdjustmentFixedAsync                                        = NULL_PTR;
  DidDataPtr->ShortTermAdjustmentVariableAsync                                     = NULL_PTR;
  DidDataPtr->ResetToDefaultSyncUnion.ResetToDefaultSync                           = NULL_PTR;
  DidDataPtr->ResetToDefaultAsync                                                  = NULL_PTR;
  DidDataPtr->ReturnControlToEcuSyncUnion.ReturnControlToEcuSync                   = NULL_PTR;
  DidDataPtr->ReturnControlToEcuAsync                                              = NULL_PTR;

#if (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON)
  /* Rte_Read and Rte_Write is a union, initialize only one element */
  DidDataPtr->Rte_Read.Data_boolean     = NULL_PTR;
  DidDataPtr->Rte_Write.Data_boolean    = NULL_PTR;
#endif /* (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON) */
}

#if (DCM_DID_DATA_TABLE_SIZE > 0)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_GetDidDataSyncFuncPointers(
  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_VAR) DidDataPtr,
  uint16 DidDataIndex)
{
  const Dcm_DidDataDescriptorType
    DataDescriptor = Dcm_DidDataIndexingArray[DidDataIndex].DidDataDescriptor;
  const uint32 DataPosition = Dcm_DidDataIndexingArray[DidDataIndex].DidDataPosition;

  P2CONST(Dcm_VariableFunctionPointerRecordType, AUTOMATIC, DCM_CONST) DataCfg =
    &Dcm_DidDataTable[DataPosition];

  if (DCM_IS_CONDITIONCHECKREAD_AVAILABLE(DataDescriptor))
  {
    /* Get ConditionCheckRead Function pointer */
    DidDataPtr->ConditionCheckReadSync = DataCfg->ConditionCheckReadSync;
    DCM_NEXT_DID_DATA_FUNC_POINTER(DataCfg);
  }

  if (DCM_IS_READDATALENGTH_AVAILABLE(DataDescriptor))
  {
    /* Get ReadDataLength Function pointer */
    DidDataPtr->ReadDataLengthSync = DataCfg->ReadDataLengthSync;
    DCM_NEXT_DID_DATA_FUNC_POINTER(DataCfg);
  }

  if (DCM_IS_READDATA_AVAILABLE(DataDescriptor))
  {
    /* Get ReadData Function pointer */
    DidDataPtr->ReadDataFncSync = DataCfg->ReadDataFncSync;
    DCM_NEXT_DID_DATA_FUNC_POINTER(DataCfg);
  }

  if (DCM_IS_WRITEDATA_AVAILABLE(DataDescriptor))
  {
    /* Get WriteData Function pointer */
    if ((DataDescriptor & DCM_DID_DATA_FIXED_LENGTH) != FALSE)
    {
      DidDataPtr->WriteFixedDataSync = DataCfg->WriteFixedDataSync;
    }
    else
    {
      DidDataPtr->WriteVariableDataSync = DataCfg->WriteVariableDataSync;
    }

    DCM_NEXT_DID_DATA_FUNC_POINTER(DataCfg);
  }

#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
  if (DCM_IS_READSCALINGDATA_AVAILABLE(DataDescriptor))
  {
    /* Get ReadScalingData Function pointer */
    DidDataPtr->ReadScalingDataFncSync = DataCfg->ReadScalingDataFncSync;
    DCM_NEXT_DID_DATA_FUNC_POINTER(DataCfg);
  }
#endif

  if (DCM_IS_IOCTRLFREEZE_AVAILABLE(DataDescriptor))
  {
    /* Get FreezeCurrentState Function pointer */
    DidDataPtr->FreezeCurrentStateSyncUnion.FreezeCurrentStateSync = DataCfg->FreezeCurrentStateSync;

    DCM_NEXT_DID_DATA_FUNC_POINTER(DataCfg);
  }

  if (DCM_IS_SHORTTERMADJ_AVAILABLE(DataDescriptor))
  {
    /* Get ShortTermAdjustment Function pointer */
    if ((DataDescriptor & DCM_DID_DATA_FIXED_LENGTH) != FALSE)
    {
      DidDataPtr->ShortTermAdjustmentFixedSyncUnion.ShortTermAdjustmentFixedSync =
        DataCfg->ShortTermAdjustmentFixedSync;
    }
    else
    {
      DidDataPtr->ShortTermAdjustmentVariableSyncUnion.ShortTermAdjustmentVariableSync =
        DataCfg->ShortTermAdjustmentVariableSync;
    }

    DCM_NEXT_DID_DATA_FUNC_POINTER(DataCfg);
  }

  if (DCM_IS_IOCTRLRESET_AVAILABLE(DataDescriptor))
  {
    /* Get FreezeCurrentState Function pointer */
    DidDataPtr->ResetToDefaultSyncUnion.ResetToDefaultSync = DataCfg->ResetToDefaultSync;
    DCM_NEXT_DID_DATA_FUNC_POINTER(DataCfg);
  }

  if (DCM_IS_IOCTRLRETURN_AVAILABLE(DataDescriptor))
  {
    /* Get FreezeCurrentState Function pointer */
    DidDataPtr->ReturnControlToEcuSyncUnion.ReturnControlToEcuSync = DataCfg->ReturnControlToEcuSync;
  }
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_GetDidDataAsyncFuncPointers(
  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_VAR) DidDataPtr,
  uint16 DidDataIndex)
{
  const Dcm_DidDataDescriptorType
    DataDescriptor = Dcm_DidDataIndexingArray[DidDataIndex].DidDataDescriptor;
  const uint32 DataPosition = Dcm_DidDataIndexingArray[DidDataIndex].DidDataPosition;

  P2CONST(Dcm_VariableFunctionPointerRecordType, AUTOMATIC, DCM_CONST) DataCfg =
    &Dcm_DidDataTable[DataPosition];

  if (DCM_IS_CONDITIONCHECKREAD_AVAILABLE(DataDescriptor))
  {
    /* Get ConditionCheckRead Function pointer */
    DidDataPtr->ConditionCheckReadAsync = DataCfg->ConditionCheckReadAsync;
    DCM_NEXT_DID_DATA_FUNC_POINTER(DataCfg);
  }

  if (DCM_IS_READDATALENGTH_AVAILABLE(DataDescriptor))
  {
    /* Get ReadDataLength Function pointer */
    if(DCM_DEFAULT_ASR_SERVICE_API == DCM_SERVICE_API_ASR40)
    {
      DidDataPtr->ReadDataLengthSync = DataCfg->ReadDataLengthSync;
    }
    else
    {
      DidDataPtr->ReadDataLengthAsync = DataCfg->ReadDataLengthAsync;
    }

    DCM_NEXT_DID_DATA_FUNC_POINTER(DataCfg);
  }

  if (DCM_IS_READDATA_AVAILABLE(DataDescriptor))
  {
    /* Get ReadData Function pointer */
    DidDataPtr->ReadDataFncAsync = DataCfg->ReadDataFncAsync;
    DCM_NEXT_DID_DATA_FUNC_POINTER(DataCfg);
  }

  if (DCM_IS_WRITEDATA_AVAILABLE(DataDescriptor))
  {
    /* Get WriteData Function pointer */
    if ((DataDescriptor & DCM_DID_DATA_FIXED_LENGTH) != FALSE)
    {
      DidDataPtr->WriteFixedLengthDataAsync = DataCfg->WriteFixedLengthDataAsync;
    }
    else
    {
      DidDataPtr->WriteVarLengthDataAsync = DataCfg->WriteVarLengthDataAsync;
    }

    DCM_NEXT_DID_DATA_FUNC_POINTER(DataCfg);
  }

#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
  if (DCM_IS_READSCALINGDATA_AVAILABLE(DataDescriptor))
  {
    /* Get ReadScalingData Function pointer */
    DidDataPtr->ReadScalingDataFncAsync = DataCfg->ReadScalingDataFncAsync;
    DCM_NEXT_DID_DATA_FUNC_POINTER(DataCfg);
  }
#endif

  if (DCM_IS_IOCTRLFREEZE_AVAILABLE(DataDescriptor))
  {
    /* Get FreezeCurrentState Function pointer */
    DidDataPtr->FreezeCurrentStateAsync = DataCfg->FreezeCurrentStateAsync;
    DCM_NEXT_DID_DATA_FUNC_POINTER(DataCfg);
  }

  if (DCM_IS_SHORTTERMADJ_AVAILABLE(DataDescriptor))
  {
    /* Get ShortTermAdjustment Function pointer */
    if ((DataDescriptor & DCM_DID_DATA_FIXED_LENGTH) != FALSE)
    {
      DidDataPtr->ShortTermAdjustmentFixedAsync =
        DataCfg->ShortTermAdjustmentFixedAsync;
    }
    else
    {
      DidDataPtr->ShortTermAdjustmentVariableAsync =
        DataCfg->ShortTermAdjustmentVariableAsync;
    }

    DCM_NEXT_DID_DATA_FUNC_POINTER(DataCfg);
  }

  if (DCM_IS_IOCTRLRESET_AVAILABLE(DataDescriptor))
  {
    /* Get FreezeCurrentState Function pointer */
    DidDataPtr->ResetToDefaultAsync = DataCfg->ResetToDefaultAsync;
    DCM_NEXT_DID_DATA_FUNC_POINTER(DataCfg);
  }

  if (DCM_IS_IOCTRLRETURN_AVAILABLE(DataDescriptor))
  {
    /* Get FreezeCurrentState Function pointer */
    DidDataPtr->ReturnControlToEcuAsync = DataCfg->ReturnControlToEcuAsync;
  }
}


#if (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_GetDidDataSRPointers_Read(
  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_VAR) DidDataPtr,
  uint16 DidDataIndex)
{
  const uint32 DataPosition = Dcm_DidDataIndexingArray[DidDataIndex].DidDataPosition;

  P2CONST(Dcm_VariableFunctionPointerRecordType, AUTOMATIC, DCM_CONST) DataCfg =
    &Dcm_DidDataTable[DataPosition];

  switch (DidDataPtr->DataType)
  {
    case DCM_TYPE_BOOLEAN:
    {
      DidDataPtr->Rte_Read.Data_boolean = DataCfg->Rte_Read_DataServices_boolean;
      break;
    }

    case DCM_TYPE_SINT16:
    case DCM_TYPE_SINT16_N:
    {
      DidDataPtr->Rte_Read.Data_sint16 = DataCfg->Rte_Read_DataServices_sint16;
      break;
    }
    case DCM_TYPE_SINT32:
    case DCM_TYPE_SINT32_N:
    {
      DidDataPtr->Rte_Read.Data_sint32 = DataCfg->Rte_Read_DataServices_sint32;
      break;
    }
    case DCM_TYPE_SINT8:
    case DCM_TYPE_SINT8_N:
    {
      DidDataPtr->Rte_Read.Data_sint8 = DataCfg->Rte_Read_DataServices_sint8;
      break;
    }
    case DCM_TYPE_UINT16:
    case DCM_TYPE_UINT16_N:
    {
      DidDataPtr->Rte_Read.Data_uint16 = DataCfg->Rte_Read_DataServices_uint16;
      break;
    }
    case DCM_TYPE_UINT32:
    case DCM_TYPE_UINT32_N:
    {
      DidDataPtr->Rte_Read.Data_uint32 = DataCfg->Rte_Read_DataServices_uint32;
      break;
    }
    case DCM_TYPE_UINT8:
    case DCM_TYPE_UINT8_N:
    {
      DidDataPtr->Rte_Read.Data_uint8 = DataCfg->Rte_Read_DataServices_uint8;
      break;
    }

    /* CHECK: NOPARSE */
    default:
    {
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
    }
    break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DidSignals_GetDidDataSRPointers_Write(
  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_VAR) DidDataPtr,
  uint16 DidDataIndex,
  boolean ReadAccessPresent)
{
  uint32 DataPosition = Dcm_DidDataIndexingArray[DidDataIndex].DidDataPosition;

  P2CONST(Dcm_VariableFunctionPointerRecordType, AUTOMATIC, DCM_CONST) DataCfg =
    &Dcm_DidDataTable[DataPosition];

  if (TRUE == ReadAccessPresent)
  {
    /* In case read access is configured, the write interface pointer
     * needs to point one position further */
    DCM_NEXT_DID_DATA_FUNC_POINTER(DataCfg);
  }

  switch (DidDataPtr->DataType)
  {
    case DCM_TYPE_BOOLEAN:
    {
      DidDataPtr->Rte_Write.Data_boolean = DataCfg->Rte_Write_DataServices_boolean;
      break;
    }

    case DCM_TYPE_SINT16:
    {
      DidDataPtr->Rte_Write.Data_sint16 = DataCfg->Rte_Write_DataServices_sint16;
      break;
    }

    case DCM_TYPE_SINT16_N:
    {
      DidDataPtr->Rte_Write.Data_sint16N = DataCfg->Rte_Write_DataServices_sint16N;
      break;
    }
    case DCM_TYPE_SINT32:
    {
      DidDataPtr->Rte_Write.Data_sint32 = DataCfg->Rte_Write_DataServices_sint32;
      break;
    }

    case DCM_TYPE_SINT32_N:
    {
      DidDataPtr->Rte_Write.Data_sint32N = DataCfg->Rte_Write_DataServices_sint32N;
      break;
    }
    case DCM_TYPE_SINT8:
    {
      DidDataPtr->Rte_Write.Data_sint8 = DataCfg->Rte_Write_DataServices_sint8;
      break;
    }

    case DCM_TYPE_SINT8_N:
    {
      DidDataPtr->Rte_Write.Data_sint8N = DataCfg->Rte_Write_DataServices_sint8N;
      break;
    }
    case DCM_TYPE_UINT16:
    {
      DidDataPtr->Rte_Write.Data_uint16 = DataCfg->Rte_Write_DataServices_uint16;
      break;
    }

    case DCM_TYPE_UINT16_N:
    {
      DidDataPtr->Rte_Write.Data_uint16N = DataCfg->Rte_Write_DataServices_uint16N;
      break;
    }
    case DCM_TYPE_UINT32:
    {
      DidDataPtr->Rte_Write.Data_uint32 = DataCfg->Rte_Write_DataServices_uint32;
      break;
    }

    case DCM_TYPE_UINT32_N:
    {
      DidDataPtr->Rte_Write.Data_uint32N = DataCfg->Rte_Write_DataServices_uint32N;
      break;
    }

    case DCM_TYPE_UINT8:
    {
      DidDataPtr->Rte_Write.Data_uint8 = DataCfg->Rte_Write_DataServices_uint8;
      break;
    }

    case DCM_TYPE_UINT8_N:
    {
      DidDataPtr->Rte_Write.Data_uint8N = DataCfg->Rte_Write_DataServices_uint8N;
      break;
    }

    /* CHECK: NOPARSE */
    default:
    {
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
    }
    break;
    /* CHECK: PARSE */
  }
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DidSignals_ReadSignal_SenderReceiver(
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_CONST) DataCfg,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) SignalBuffer)
{
  Std_ReturnType ReadSignalSR = E_NOT_OK;
  Std_ReturnType ReadCallbackRetVal = E_NOT_OK;
  uint16 DataLength = DCM_GET_BYTES(DataCfg->DataLength);

  /* !LINKSTO Dcm.ReadDataById.ReadData.SR.Invoke,1 */
  /* !LINKSTO Dcm.ReadDataById.ReadData.SRS.Invoke,1 */
  switch (DataCfg->DataType)
  {
    case DCM_TYPE_BOOLEAN:
    {
      boolean ReadData = 0U;
      ReadCallbackRetVal = DataCfg->Rte_Read.Data_boolean(&ReadData);

      /* Copy the data into the signal buffer which will later on be copied
       * into the response buffer */
      SignalBuffer[0U] = ReadData;

      break;
    }

    case DCM_TYPE_SINT16:
    {
      sint16 ReadData = 0U;
      P2VAR(sint16, AUTOMATIC, AUTOMATIC) LocalPtr = &ReadData;
      ReadCallbackRetVal = DataCfg->Rte_Read.Data_sint16(&ReadData);
      DataLength = 2U;

      /* Copy the data into the signal buffer which will later on be copied
       * into the response buffer */
      TS_MemCpy(SignalBuffer, LocalPtr, DataLength);

      break;
    }
    case DCM_TYPE_SINT16_N:
    {
      /* Deviation MISRAC2012-2 */
      P2VAR(sint16, AUTOMATIC, DCM_VAR) LocalPtr = (sint16*)(void*)SignalBuffer;
      ReadCallbackRetVal = DataCfg->Rte_Read.Data_sint16(LocalPtr);
      break;
    }
    case DCM_TYPE_SINT32:
    {
      sint32 ReadData = 0U;
      P2VAR(sint32, AUTOMATIC, AUTOMATIC) LocalPtr = &ReadData;
      ReadCallbackRetVal = DataCfg->Rte_Read.Data_sint32(&ReadData);
      DataLength = 4U;

      /* Copy the data into the signal buffer which will later on be copied
       * into the response buffer */
      TS_MemCpy(SignalBuffer, LocalPtr, DataLength);

      break;
    }
    case DCM_TYPE_SINT32_N:
    {
      /* Deviation MISRAC2012-2 */
      P2VAR(sint32, AUTOMATIC, DCM_VAR) LocalPtr = (sint32*)(void*)SignalBuffer;
      ReadCallbackRetVal = DataCfg->Rte_Read.Data_sint32(LocalPtr);
      break;
    }
    case DCM_TYPE_SINT8:
    {
      sint8 ReadData = 0U;
      P2VAR(sint8, AUTOMATIC, AUTOMATIC) LocalPtr = &ReadData;
      ReadCallbackRetVal = DataCfg->Rte_Read.Data_sint8(&ReadData);
      DataLength = 1U;

      /* Copy the data into the signal buffer which will later on be copied
       * into the response buffer */
      TS_MemCpy(SignalBuffer, LocalPtr, DataLength);

      break;
    }

    case DCM_TYPE_SINT8_N:
    {
      /* Deviation MISRAC2012-2 */
      P2VAR(sint8, AUTOMATIC, DCM_VAR) LocalPtr = (sint8*)(void*)SignalBuffer;
      ReadCallbackRetVal = DataCfg->Rte_Read.Data_sint8(LocalPtr);
      break;
    }
    case DCM_TYPE_UINT16:
    {
      uint16 ReadData = 0U;
      P2VAR(uint16, AUTOMATIC, AUTOMATIC) LocalPtr = &ReadData;
      ReadCallbackRetVal = DataCfg->Rte_Read.Data_uint16(&ReadData);
      DataLength = 2U;

      /* Copy the data into the signal buffer which will later on be copied
       * into the response buffer */
      TS_MemCpy(SignalBuffer, LocalPtr, DataLength);

      break;
    }
    case DCM_TYPE_UINT16_N:
    {
      /* Deviation MISRAC2012-2 */
      P2VAR(uint16, AUTOMATIC, DCM_VAR) LocalPtr = (uint16*)(void*)SignalBuffer;
      ReadCallbackRetVal = DataCfg->Rte_Read.Data_uint16(LocalPtr);
      break;
    }
    case DCM_TYPE_UINT32:
    {
      uint32 ReadData = 0U;
      P2VAR(uint32, AUTOMATIC, AUTOMATIC) LocalPtr = &ReadData;
      ReadCallbackRetVal = DataCfg->Rte_Read.Data_uint32(&ReadData);
      DataLength = 4U;

      /* Copy the data into the signal buffer which will later on be copied
       * into the response buffer */
      TS_MemCpy(SignalBuffer, LocalPtr, DataLength);

      break;
    }
    case DCM_TYPE_UINT32_N:
    {
      /* Deviation MISRAC2012-2 */
      P2VAR(uint32, AUTOMATIC, DCM_VAR) LocalPtr = (uint32*)(void*)SignalBuffer;
      ReadCallbackRetVal = DataCfg->Rte_Read.Data_uint32(LocalPtr);
      break;
    }
    case DCM_TYPE_UINT8:
    {
      uint8 ReadData;
      ReadCallbackRetVal = DataCfg->Rte_Read.Data_uint8(&ReadData);

      /* Copy the data into the signal buffer which will later on be copied
       * into the response buffer */
      SignalBuffer[0U] = ReadData;

      break;
    }
    case DCM_TYPE_UINT8_N:
    {
      ReadCallbackRetVal = DataCfg->Rte_Read.Data_uint8(SignalBuffer);
      break;
    }

    /* CHECK: NOPARSE */
    default:
    {
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
    }
    break;
    /* CHECK: PARSE */
  }

  /* Ignore the RTE_E_MAX_AGE_EXCEEDED Overlayed Error */
  ReadCallbackRetVal &= ~RTE_E_MAX_AGE_EXCEEDED;

  /* Translate the result into Dcm known return values */
  if (RTE_E_OK == ReadCallbackRetVal)
  {
    ReadSignalSR = DCM_E_OK;
  }
  else
  {
    /* !LINKSTO Dcm.ReadDataById.ReadData.SR.RetVal.ConditionsNotCorrect,1 */
    /* !LINKSTO Dcm.ReadDataById.ReadData.SRS.RetVal.ConditionsNotCorrect,1 */
    ReadSignalSR = DCM_E_CONDITIONSNOTCORRECT;

    /* Dcm provides its own definitions of RTE_E_OUT_OF_RANGE and RTE_E_COM_BUSY according
     * to the ASR 4.3.0 Rte specifications, since the current Rte implementation deviates
     * from the standard and does not generate them */
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    if ((RTE_E_UNCONNECTED != ReadCallbackRetVal) &&
        (RTE_E_COM_STOPPED != ReadCallbackRetVal) &&
        (RTE_E_NEVER_RECEIVED != ReadCallbackRetVal) &&
        (DCM_RTE_E_COM_BUSY != ReadCallbackRetVal) &&
        (RTE_E_HARD_TRANSFORMER_ERROR != ReadCallbackRetVal) &&
        (RTE_E_INVALID != ReadCallbackRetVal) &&
        (DCM_RTE_E_OUT_OF_RANGE != ReadCallbackRetVal) &&
        (RTE_E_SOFT_TRANSFORMER_ERROR != ReadCallbackRetVal))
    {
      /* !LINKSTO Dcm.ReadDataById.ReadData.SR.RetVal.OutOfRange.Det,1 */
      /* !LINKSTO Dcm.ReadDataById.ReadData.SRS.RetVal.OutOfRange.Det,1 */
      DCM_DET_REPORT_ERROR(DCM_INTERNAL_API_ID, DCM_E_INTERFACE_RETURN_VALUE);

      /* !LINKSTO Dcm.ReadDataById.ReadData.SR.RetVal.OutOfRange.Nrc,1 */
      /* !LINKSTO Dcm.ReadDataById.ReadData.SRS.RetVal.OutOfRange.Nrc,1 */
      ReadSignalSR = DCM_E_GENERALREJECT;
    }
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
  }

  return ReadSignalSR;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DidSignals_WriteSignal_SenderReceiver(
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_CONST) DataCfg,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) WriteSignalBuffer)
{
  Std_ReturnType WriteSignalSR = E_NOT_OK;
  Std_ReturnType WriteCallbackRetVal = E_NOT_OK;

  /* !LINKSTO Dcm.WriteDataById.WriteData.SR.Invoke,1 */
  /* !LINKSTO Dcm.WriteDataById.WriteData.SRS.Invoke,1 */
  switch (DataCfg->DataType)
  {
    /* Deviation MISRAC2012-2 <START> */
    case DCM_TYPE_BOOLEAN:
    {
      WriteCallbackRetVal = DataCfg->Rte_Write.Data_boolean(*WriteSignalBuffer);
      break;
    }

    case DCM_TYPE_SINT16:
    {
      WriteCallbackRetVal = DataCfg->Rte_Write.Data_sint16(*(sint16*)(void*)WriteSignalBuffer);
      break;
    }
    case DCM_TYPE_SINT16_N:
    {
      P2CONST(sint16, AUTOMATIC, DCM_VAR) LocalPtr = (sint16*)(void*)WriteSignalBuffer;

      WriteCallbackRetVal = DataCfg->Rte_Write.Data_sint16N(LocalPtr);
      break;
    }
    case DCM_TYPE_SINT32:
    {
      WriteCallbackRetVal = DataCfg->Rte_Write.Data_sint32(*(sint32*)(void*)WriteSignalBuffer);
      break;
    }
    case DCM_TYPE_SINT32_N:
    {
      P2CONST(sint32, AUTOMATIC, DCM_VAR) LocalPtr = (sint32*)(void*)WriteSignalBuffer;

      WriteCallbackRetVal = DataCfg->Rte_Write.Data_sint32N(LocalPtr);
      break;
    }
    case DCM_TYPE_SINT8:
    {
      WriteCallbackRetVal = DataCfg->Rte_Write.Data_sint8(*(sint8*)(void*)WriteSignalBuffer);
      break;
    }

    case DCM_TYPE_SINT8_N:
    {
      P2CONST(sint8, AUTOMATIC, DCM_VAR) LocalPtr = (sint8*)(void*)WriteSignalBuffer;

      WriteCallbackRetVal = DataCfg->Rte_Write.Data_sint8N(LocalPtr);
      break;
    }
    case DCM_TYPE_UINT16:
    {
      WriteCallbackRetVal = DataCfg->Rte_Write.Data_uint16(*(uint16*)(void*)WriteSignalBuffer);
      break;
    }
    case DCM_TYPE_UINT16_N:
    {
      P2CONST(uint16, AUTOMATIC, DCM_VAR) LocalPtr = (uint16*)(void*)WriteSignalBuffer;

      WriteCallbackRetVal = DataCfg->Rte_Write.Data_uint16N(LocalPtr);
      break;
    }
    case DCM_TYPE_UINT32:
    {
      WriteCallbackRetVal = DataCfg->Rte_Write.Data_uint32(*(uint32*)(void*)WriteSignalBuffer);
      break;
    }
    case DCM_TYPE_UINT32_N:
    {
      P2CONST(uint32, AUTOMATIC, DCM_VAR) LocalPtr = (uint32*)(void*)WriteSignalBuffer;

      WriteCallbackRetVal = DataCfg->Rte_Write.Data_uint32N(LocalPtr);
      break;
    }
    case DCM_TYPE_UINT8:
    {
      WriteCallbackRetVal = DataCfg->Rte_Write.Data_uint8(*WriteSignalBuffer);
      break;
    }
    case DCM_TYPE_UINT8_N:
    {
      WriteCallbackRetVal = DataCfg->Rte_Write.Data_uint8N(WriteSignalBuffer);
      break;
    }
    /* Deviation MISRAC2012-2 <STOP> */
    /* CHECK: NOPARSE */
    default:
    {
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
    }
    break;
    /* CHECK: PARSE */
  }

  /* Translate the result into Dcm known return values */
  if (RTE_E_OK == WriteCallbackRetVal)
  {
    WriteSignalSR = DCM_E_OK;
  }
  else
  {
    /* !LINKSTO Dcm.WriteDataById.WriteData.SR.RetVal.ConditionsNotCorrect,1 */
    /* !LINKSTO Dcm.WriteDataById.WriteData.SRS.RetVal.ConditionsNotCorrect,1 */
    WriteSignalSR = DCM_E_CONDITIONSNOTCORRECT;

    /* Dcm provides its own definitions of RTE_E_OUT_OF_RANGE and RTE_E_COM_BUSY according
     * to the ASR 4.3.0 Rte specifications, since the current Rte implementation deviates
     * from the standard and does not generate them */
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    if ((RTE_E_COM_STOPPED != WriteCallbackRetVal) &&
        (DCM_RTE_E_SEG_FAULT != WriteCallbackRetVal) &&
        (DCM_RTE_E_COM_BUSY != WriteCallbackRetVal) &&
        (RTE_E_HARD_TRANSFORMER_ERROR != WriteCallbackRetVal) &&
        (RTE_E_SOFT_TRANSFORMER_ERROR != WriteCallbackRetVal))
    {
      /* !LINKSTO Dcm.WriteDataById.WriteData.SR.RetVal.OutOfRange.Det,1 */
      /* !LINKSTO Dcm.WriteDataById.WriteData.SRS.RetVal.OutOfRange.Det,1 */
      DCM_DET_REPORT_ERROR(DCM_INTERNAL_API_ID, DCM_E_INTERFACE_RETURN_VALUE);

      /* !LINKSTO Dcm.WriteDataById.WriteData.SR.RetVal.OutOfRange.Nrc,1 */
      /* !LINKSTO Dcm.WriteDataById.WriteData.SRS.RetVal.OutOfRange.Nrc,1 */
      WriteSignalSR = DCM_E_GENERALREJECT;
    }
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
  }

  return WriteSignalSR;
}
#endif /* (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON) */
#endif /* (DCM_DID_DATA_TABLE_SIZE > 0) */
#endif /* (DCM_NUM_DID_DATA > 0) */

#if ((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON))

STATIC FUNC(Dcm_MsgLenType, DCM_CODE) Dcm_Dsp_DidSignals_EndianessConversion_CalculateSignalLength (
  Dcm_DidDataByteType DataType,
  Dcm_MsgLenType              DataLength)
{
  Dcm_MsgLenType SignalLength = 0U;
  uint8 DataTypeBase = 0U;
  boolean ConversionNeeded = FALSE;

  switch (DataType)
  {
    case DCM_TYPE_SINT16:
    case DCM_TYPE_UINT16:
    {
      DataTypeBase = DCM_DIDSERVICES_SIGNAL_DATATYPE_16BITS;
      ConversionNeeded = TRUE;
    }
    break;

    case DCM_TYPE_SINT32:
    case DCM_TYPE_UINT32:
    {
      DataTypeBase = DCM_DIDSERVICES_SIGNAL_DATATYPE_32BITS;
      ConversionNeeded = TRUE;
    }
    break;

    default:
    {
      DCM_PRECONDITION_ASSERT(
        ((DataType == DCM_TYPE_SINT8) ||
        (DataType == DCM_TYPE_UINT8) ||
        (DataType == DCM_TYPE_BOOLEAN)),
        DCM_INTERNAL_API_ID);

      /* If the data type base is 8 bits, no endianess conversion is done
       * and the signal length is the configured DataLength */
      SignalLength = DataLength;
      ConversionNeeded = FALSE;
    }
    break;
  }

  if (TRUE == ConversionNeeded)
  {
    if ((DataLength % DataTypeBase) == 0U)
    {
      /* If the configured data length is an exact multiple of DataType base size,
       * the signal length is that exact data length */
      SignalLength = DataLength;
    }
    else
    {
      /* If the configured data length is not an exact multiple of DataType base size,
       * the signal length is the data length + 1 data type size */
      SignalLength = ((DataLength / DataTypeBase) + 1U) * DataTypeBase;
    }
  }

  return SignalLength;
}

#endif /* ((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON)) */

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_ReadConditionCheck(
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_CONST)          DataCfg,
  Dcm_OpStatusType                                        OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc)
{
  Std_ReturnType ConditionCheckStatus = DCM_E_NOT_OK;

  if ((DataCfg->AccessInterface == DCM_USE_DATA_SYNCH_FNC) ||
      (DataCfg->AccessInterface == DCM_USE_DATA_SYNCH_CLIENT_SERVER))
  {
    if (NULL_PTR != DataCfg->ConditionCheckReadSync)
    {
      ConditionCheckStatus = Dcm_Dsp_DidSignals_ReadConditionCheckSync(DataCfg, Nrc);
    }
    else
    {
      ConditionCheckStatus = DCM_E_OK;
    }
  }
  else if ((DataCfg->AccessInterface == DCM_USE_DATA_ASYNCH_FNC) ||
           (DataCfg->AccessInterface == DCM_USE_DATA_ASYNCH_CLIENT_SERVER))
  {
    if (NULL_PTR != DataCfg->ConditionCheckReadAsync)
    {
      if (DCM_CANCEL != OpStatus)
      {
        ConditionCheckStatus = Dcm_Dsp_DidSignals_ReadConditionCheckAsync(DataCfg, OpStatus, Nrc);
      }
      else
      {
        /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.AsyncFnc.Pending.Cancel,1 */
        /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.AsyncCS.Pending.Cancel,1 */
        (void)Dcm_Dsp_DidSignals_ReadConditionCheckAsync(DataCfg, DCM_CANCEL, Nrc);
      }
    }
    else
    {
      ConditionCheckStatus = DCM_E_OK;
    }
  }
  else
  {
    DCM_PRECONDITION_ASSERT(
      ((DataCfg->AccessInterface == DCM_USE_BLOCK_ID) ||
       (DataCfg->AccessInterface == DCM_USE_DATA_SENDER_RECEIVER)),
      DCM_INTERNAL_API_ID);
    ConditionCheckStatus = DCM_E_OK;
  }

  return ConditionCheckStatus;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_ReadConditionCheckSync(
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_CONST)          DataCfg,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc)
{
  Std_ReturnType InterfaceRetVal;
  Std_ReturnType OperationRetVal;

  /* This function should not be called with NULL_PTR */
  DCM_PRECONDITION_ASSERT(NULL_PTR != DataCfg->ConditionCheckReadSync, DCM_INTERNAL_API_ID);

  /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.SyncFnc.Invoke,1 */
  /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.SyncCS.Invoke,1 */
  InterfaceRetVal = DataCfg->ConditionCheckReadSync(Nrc);

  if (E_OK != InterfaceRetVal)
  {
    OperationRetVal = DCM_E_NOT_OK;

    if (E_NOT_OK == InterfaceRetVal)
    {
      /* NRC is set directly by the callback */
      /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.SyncFnc.RetVal.NotOk,1 */
      /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.SyncCS.RetVal.NotOk,1 */
      if (DCM_E_OK == *Nrc)
      {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.SyncFnc.RetVal.Invalid.Det,1 */
        /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.SyncCS.RetVal.Invalid.Det,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_CONDITIONCHECKREAD_SYNC, DCM_E_INVALID_VALUE);

        /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.SyncFnc.RetVal.Invalid.Nrc,1 */
        /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.SyncCS.RetVal.Invalid.Nrc,1 */
        *Nrc = DCM_E_GENERALREJECT;
#else /* (DCM_DEV_ERROR_DETECT == STD_ON) */
        /* !LINKSTO Dcm.EB.ReadDataById.ConditionCheckRead.SyncFnc.NRC0x00,1 */
        /* !LINKSTO Dcm.EB.ReadDataById.ConditionCheckRead.SyncCS.NRC0x00,1 */
        *Nrc = DCM_E_REQUESTOUTOFRANGE;
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
      }
    }
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    else
    {
      /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.SyncFnc.RetVal.OutOfRange.Det,1 */
      /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.SyncCS.RetVal.OutOfRange.Det,1 */
      DCM_DET_REPORT_ERROR(DCM_SVCID_CONDITIONCHECKREAD_SYNC, DCM_E_INTERFACE_RETURN_VALUE);

      /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.SyncFnc.RetVal.OutOfRange.Nrc,1 */
      /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.SyncCS.RetVal.OutOfRange.Nrc,1 */
      *Nrc = DCM_E_GENERALREJECT;
    }
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    /* If the Condition check read returns DCM_E_OK, set the nrc to DCM_E_OK */
    *Nrc = DCM_E_OK;
    OperationRetVal = DCM_E_OK;
  }

  return OperationRetVal;
}

/* Calls the ConditionCheckRead Async operation and handles the return values */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_ReadConditionCheckAsync(
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_CONST)          DataCfg,
  Dcm_OpStatusType                                        OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc)
{
  Std_ReturnType InterfaceRetVal;
  Std_ReturnType OperationRetVal = DCM_E_NOT_OK;

  /* This function should not be called with NULL_PTR */
  DCM_PRECONDITION_ASSERT(NULL_PTR != DataCfg->ConditionCheckReadAsync, DCM_INTERNAL_API_ID);

  /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.AsyncFnc.Initial.Invoke,1 */
  /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.AsyncCS.Initial.Invoke,1 */
  InterfaceRetVal = DataCfg->ConditionCheckReadAsync(OpStatus, Nrc);

  if (DCM_CANCEL != OpStatus)
  {
    if (E_OK != InterfaceRetVal)
    {
      if (DCM_E_PENDING == InterfaceRetVal)
      {
        OperationRetVal = DCM_E_PENDING;
      }
      else if (E_NOT_OK == InterfaceRetVal)
      {
        OperationRetVal = DCM_E_NOT_OK;
        /* NRC is set directly by the callback */
        /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.AsyncFnc.RetVal.NotOk,1 */
        /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.AsyncCS.RetVal.NotOk,1 */
        if (DCM_E_OK == *Nrc)
        {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
          /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.AsyncFnc.RetVal.Invalid.Det,1 */
          /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.AsyncCS.RetVal.Invalid.Det,1 */
          DCM_DET_REPORT_ERROR(DCM_SVCID_CONDITIONCHECKREAD_ASYNC, DCM_E_INVALID_VALUE);

          /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.AsyncFnc.RetVal.Invalid.Nrc,1 */
          /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.AsyncCS.RetVal.Invalid.Nrc,1 */
          *Nrc = DCM_E_GENERALREJECT;
#else /* (DCM_DEV_ERROR_DETECT == STD_ON) */
          /* !LINKSTO Dcm.EB.ReadDataById.ConditionCheckRead.AsyncFnc.NRC0x00,1 */
          /* !LINKSTO Dcm.EB.ReadDataById.ConditionCheckRead.AsyncCS.NRC0x00,1 */
          *Nrc = DCM_E_REQUESTOUTOFRANGE;
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
        }
      }
      else
      {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.AsyncFnc.RetVal.OutOfRange.Det,1 */
        /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.AsyncCS.RetVal.OutOfRange.Det,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_CONDITIONCHECKREAD_ASYNC, DCM_E_INTERFACE_RETURN_VALUE);

        /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.AsyncFnc.RetVal.OutOfRange.Nrc,1 */
        /* !LINKSTO Dcm.ReadDataById.ConditionCheckRead.AsyncCS.RetVal.OutOfRange.Nrc,1 */
        OperationRetVal = DCM_E_NOT_OK;
        *Nrc = DCM_E_GENERALREJECT;
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
      }
    }
    else
    {
      /* If the Condition check read returns DCM_E_OK, set the Nrc to DCM_E_OK */
      *Nrc = DCM_E_OK;
      OperationRetVal = DCM_E_OK;
    }
  }
  else
  {
    /* CANCEL case */
    OperationRetVal = DCM_E_NOT_OK;
  }

  return OperationRetVal;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_ReadDataLength(
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_CONST)          DataCfg,
  Dcm_OpStatusType                                        OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc,
  P2VAR(uint16, AUTOMATIC, DCM_VAR)                       SignalLength)
{
  Std_ReturnType InterfaceRetVal = E_NOT_OK;
  Std_ReturnType OperationRetVal = DCM_E_NOT_OK;
  uint16 ReadSignalDataLength    = 0U;
  boolean PendingAllowed         = FALSE;

  *Nrc = E_OK;

  /* AUTOSAR version 4.2.1 specifies that the ReadDataLength ASYNC operation always
   * takes an OpStatus parameter and may return PENDING, case in which it must be
   * called again. */
  if (((DataCfg->AccessInterface == DCM_USE_DATA_ASYNCH_FNC) ||
       (DataCfg->AccessInterface == DCM_USE_DATA_ASYNCH_CLIENT_SERVER)) &&
      (DCM_DEFAULT_ASR_SERVICE_API == DCM_SERVICE_API_ASR42))
  {
    DCM_PRECONDITION_ASSERT(NULL_PTR != DataCfg->ReadDataLengthAsync, DCM_INTERNAL_API_ID);

    /* !LINKSTO Dcm.ReadDataById.ReadDataLength.AsyncFnc.Initial.Invoke,1 */
    /* !LINKSTO Dcm.ReadDataById.ReadDataLength.AsyncCS.Initial.Invoke,1 */
    InterfaceRetVal = DataCfg->ReadDataLengthAsync(OpStatus, &ReadSignalDataLength);
    PendingAllowed = TRUE;
  }
  else
  {
    DCM_PRECONDITION_ASSERT(NULL_PTR != DataCfg->ReadDataLengthSync, DCM_INTERNAL_API_ID);
    /* !LINKSTO Dcm.ReadDataById.ReadDataLength.SyncFnc.Invoke,1 */
    /* !LINKSTO Dcm.ReadDataById.ReadDataLength.AsyncFnc.ASR40.Invoke,1 */
    /* !LINKSTO Dcm.ReadDataById.ReadDataLength.SyncCS.Invoke,1 */
    /* !LINKSTO Dcm.ReadDataById.ReadDataLength.AsyncCS.ASR40.Invoke,1 */
    InterfaceRetVal = DataCfg->ReadDataLengthSync(&ReadSignalDataLength);
  }

  if (E_OK != InterfaceRetVal)
  {
    /* Deviation TASKING-1 */
    if ((DCM_E_PENDING == InterfaceRetVal) && (TRUE == PendingAllowed))
    {
      OperationRetVal = DCM_E_PENDING;
    }
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    else
    {
      /* Deviation TASKING-1 */
      if (TRUE == PendingAllowed)
      {
        /* !LINKSTO Dcm.ReadDataById.ReadDataLength.AsyncFnc.RetVal.OutOfRange.Det,1 */
        /* !LINKSTO Dcm.ReadDataById.ReadDataLength.AsyncCS.RetVal.OutOfRange.Det,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_READDATALENGTH_ASYNC, DCM_E_INTERFACE_RETURN_VALUE);
      }
      else
      {
        /* !LINKSTO Dcm.ReadDataById.ReadDataLength.SyncFnc.RetVal.OutOfRange.Det,1 */
        /* !LINKSTO Dcm.ReadDataById.ReadDataLength.AsyncFnc.ASR40.RetVal.OutOfRange.Det,1 */
        /* !LINKSTO Dcm.ReadDataById.ReadDataLength.SyncCS.RetVal.OutOfRange.Det,1 */
        /* !LINKSTO Dcm.ReadDataById.ReadDataLength.AsyncCS.ASR40.RetVal.OutOfRange.Det,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_READDATALENGTH_SYNC, DCM_E_INTERFACE_RETURN_VALUE);
      }

      /* !LINKSTO Dcm.ReadDataById.ReadDataLength.SyncFnc.RetVal.OutOfRange.Nrc,1 */
      /* !LINKSTO Dcm.ReadDataById.ReadDataLength.AsyncFnc.ASR40.RetVal.OutOfRange.Nrc,1 */
      /* !LINKSTO Dcm.ReadDataById.ReadDataLength.AsyncFnc.RetVal.OutOfRange.Nrc,1 */
      /* !LINKSTO Dcm.ReadDataById.ReadDataLength.SyncCS.RetVal.OutOfRange.Nrc,1 */
      /* !LINKSTO Dcm.ReadDataById.ReadDataLength.AsyncCS.ASR40.RetVal.OutOfRange.Nrc,1 */
      /* !LINKSTO Dcm.ReadDataById.ReadDataLength.AsyncCS.RetVal.OutOfRange.Nrc,1 */
      OperationRetVal = DCM_E_NOT_OK;
      *Nrc = DCM_E_GENERALREJECT;
    }
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    /* The returned Signal length is in bytes. Converting to number of bits */
    ReadSignalDataLength = DCM_GET_BITS(ReadSignalDataLength);
    *SignalLength = ReadSignalDataLength;

    OperationRetVal = DCM_E_OK;
  }

  return OperationRetVal;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_CopyToBuffer(
  Dcm_DidSignalReadContextTypePtr ReadContext,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc)
{
  Std_ReturnType BufferCheckStatus = DCM_E_NOT_OK;

  /* Pointer to the Did signal data configuration table */
  CONSTP2VAR(Dcm_DidDataType, AUTOMATIC, DCM_VAR) DataCfg = &(ReadContext->DataCfg);
  /* Total number of bytes occupied by one signal in a Did */
  uint16 NumSignalBytes = 0U;

#if (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON)
  if ((DataCfg->AccessInterface == DCM_USE_DATA_SENDER_RECEIVER) &&
      (DataCfg->DataEndianness == DCM_BIG_ENDIAN))
  {
    NumSignalBytes = (uint16)DCM_GET_BYTES(ReadContext->SignalOffset + 1U);
  }
  else
#endif
  {
#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
    if(ReadContext->ProcessingState == DCM_SIGNALREAD_STATE_READSCALING)
    {
      NumSignalBytes = (uint16)DCM_GET_BYTES(ReadContext->SignalOffset + ReadContext->ReadScalingSignalLength );
    }
    else
#endif /* #if (DCM_DSP_USE_SERVICE_0X24 == STD_ON) */
    {
      NumSignalBytes = (uint16)DCM_GET_BYTES(ReadContext->SignalOffset + ReadContext->SignalLength );
    }
  }

  /* Whether buffer is free to hold signal data*/
  if ((ReadContext->BufferOffset + NumSignalBytes) <= ReadContext->BufferLimit)
  {

#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
  if(ReadContext->ProcessingState == DCM_SIGNALREAD_STATE_READSCALING)
  {
    /* !LINKSTO Dcm.UDSServices.ProcessReadScalingDataByIdRequest.InvokeGetScalingInformation,1 */
    /* !LINKSTO Dcm.EB.UDSServices.GetScalingInformation.AsyncClientServer.ReturnsPending,1 */
    /* !LINKSTO Dcm.EB.UDSServices.GetScalingInformation.AsyncFunction.ReturnsPending,1 */
    /* !LINKSTO Dcm.EB.UDSServices.ReadScalingDataByIdentifierServiceRequestCanceled.AsyncFunctionPending.Cancel,1 */
    /* !LINKSTO Dcm.EB.UDSServices.ReadScalingDataByIdentifierServiceRequestCanceled.AsyncClientServerPending.Cancel,1 */
    BufferCheckStatus = Dcm_Dsp_DidSignals_ReadScalingData(
                                    DataCfg, ReadContext->OpStatus, Nrc, ReadContext->SignalBuffer);
  }
  else
#endif /* if (DCM_DSP_USE_SERVICE_0X24 == STD_ON) */
  {
    /* !LINKSTO Dcm.ReadDataById.ReadData.AsyncFnc.Pending.Cancel,1 */
    /* !LINKSTO Dcm.ReadDataById.ReadData.AsyncCS.Pending.Cancel,1 */
    BufferCheckStatus = Dcm_Dsp_DidSignals_ReadData(
                                  DataCfg, ReadContext->OpStatus, Nrc, ReadContext->SignalBuffer);
  }

    if (ReadContext->OpStatus != DCM_CANCEL)
    {
      if (BufferCheckStatus == DCM_E_OK)
      {
        /* Pointer for storing the DID and signals */
        Dcm_MsgType DataPtr = &ReadContext->OutputBuffer[ReadContext->BufferOffset];

#if ((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON))
        if (((DataCfg->DataType == DCM_TYPE_SINT16) ||
             (DataCfg->DataType == DCM_TYPE_UINT16) ||
             (DataCfg->DataType == DCM_TYPE_SINT32) ||
             (DataCfg->DataType == DCM_TYPE_UINT32)) &&
            (ReadContext->EndiannessConversion == TRUE) &&
            (DataCfg->AccessInterface != DCM_USE_DATA_SENDER_RECEIVER))
        {
          /* !LINKSTO Dcm.EB.EndianessConversion.TRUE.ReadDataById.SignalData.INT16,1 */
          /* !LINKSTO Dcm.EB.EndianessConversion.TRUE.ReadDataById.SignalData.INT32,1 */
          Dcm_Dsp_DidSignals_Endianness_Conversion(
            ReadContext->SignalBuffer,
            NumSignalBytes,
            DataCfg->DataType);
        }
#endif /* ((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON)) */

#if (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON)
          if (DataCfg->AccessInterface != DCM_USE_DATA_SENDER_RECEIVER)
#endif
          {
            uint32 SizeToCopy;
#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
            if(ReadContext->ProcessingState == DCM_SIGNALREAD_STATE_READSCALING)
            {
              SizeToCopy = ReadContext->ReadScalingSignalLength;
            }
            else
#endif /* #if (DCM_DSP_USE_SERVICE_0X24 == STD_ON) */
            {
              SizeToCopy = ReadContext->SignalLength;
            }
            /* Copying signals to response buffer as per configured */
            /* !LINKSTO Dcm.ReadDataById.ReadData.SyncFnc.RetVal.Ok,1 */
            /* !LINKSTO Dcm.ReadDataById.ReadData.SyncCS.RetVal.Ok,1 */
            /* !LINKSTO Dcm.ReadDataById.ReadData.AsyncFnc.RetVal.Ok,1 */
            /* !LINKSTO Dcm.ReadDataById.ReadData.AsyncCS.RetVal.Ok,1 */
            /* !LINKSTO Dcm.ReadDataById.ReadData.SR.RetVal.Ok,1 */
            /* !LINKSTO Dcm.ReadDataById.ReadData.SRS.RetVal.Ok,1 */
            /* !LINKSTO Dcm.EB.UDSServices.GetScalingInformation.AsyncClientServer.ReturnsOK.TransmitResponse,1 */
            /* !LINKSTO Dcm.EB.UDSServices.GetScalingInformation.AsyncFunction.ReturnsOK.TransmitResponse,1 */
            /* !LINKSTO Dcm.EB.UDSServices.GetScalingInformation.SyncClientServer.ReturnsOK.TransmitResponse,1 */
            /* !LINKSTO Dcm.EB.UDSServices.GetScalingInformation.SyncFunction.ReturnsOK.TransmitResponse,1 */
            Dcm_DspInternal_CopySignalData(ReadContext->SignalBuffer,
              DataPtr,
              0U,
              ReadContext->SignalOffset,
              SizeToCopy,
              DCM_COPY_TO_OUTBUFFER_OPAQUE,
              DCM_BYTE_DIRECTION_INCREMENT);
          }
#if (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON)
          else
          {
            /* Copying signals to response buffer as per configured
             * SenderReceiver signals are copied in the context of endianness interpretation */
            /* !LINKSTO SWS_Dcm_00638_USE_DATA_SENDER_RECEIVER,1 */
            /* !LINKSTO SWS_Dcm_00638_USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
            /* !LINKSTO SWS_Dcm_00716_USE_DATA_SENDER_RECEIVER,1 */
            /* !LINKSTO SWS_Dcm_00716_USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
            Dcm_Dsp_EndiannessInterpretation_CopyData(ReadContext->SignalBuffer,
                                             DCM_ENDIANNESSINTERPRETATION_COPY_DATA_TO_OUT_BUFFER,
                                             DataPtr,
                                             DataCfg->DataEndianness,
                                             DataCfg->DataType,
                                             ReadContext->SignalLength,
                                             ReadContext->SignalOffset);
          }
#endif
        /* This is for updating resDataLen after all signals read in a Did */
        ReadContext->BufferOffset = NumSignalBytes;
      }
      else if (BufferCheckStatus == DCM_E_PENDING)
      {
      }
      else
      {
        /* Do Nothing */
      }
    }
    else
    {
      /* OpStatus == DCM_CANCEL */
      BufferCheckStatus = DCM_E_OK;
    }
  }
  /* If buffer is too small for the response data */
  else
  {
    /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.Opaque.OutOfBounds.USE_DATA_SENDER_RECEIVER,1 */
    /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.Opaque.OutOfBounds.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
    /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.LittleEndian.OutOfBounds.USE_DATA_SENDER_RECEIVER,1 */
    /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.LittleEndian.OutOfBounds.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
    /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.BigEndian.OutOfBounds.USE_DATA_SENDER_RECEIVER,1 */
    /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.BigEndian.OutOfBounds.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
    *Nrc   = DCM_E_RESPONSETOOLONG;
    BufferCheckStatus = DCM_E_NOT_OK;
  }

  if (DCM_E_PENDING != BufferCheckStatus)
  {
    /* Reading is finished with or without error, invalidate the state machine */
    ReadContext->ProcessingState = DCM_SIGNALREAD_STATE_INVALID;
  }

  return BufferCheckStatus;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_ReadData(
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_VAR) DataCfg,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) SignalBuffer)
{
  Std_ReturnType InterfaceRetVal = E_NOT_OK;
  Std_ReturnType OperationRetVal = DCM_E_NOT_OK;

  switch (DataCfg->AccessInterface)
  {
#if (DCM_READ_DID_BLOCK_ID_CONFIGURED == STD_ON)
    case DCM_USE_BLOCK_ID:
    {
      /* Call the NVRam read with the block id configured */
      OperationRetVal = Dcm_Dsp_DidSignals_ReadNvmData(
                          DataCfg->NvRamBlockId,
                          OpStatus,
                          Nrc,
                          SignalBuffer);
    }
    break;
#endif /* (DCM_READ_DID_BLOCK_ID_CONFIGURED == STD_ON) */

    case DCM_USE_DATA_SYNCH_FNC:
    case DCM_USE_DATA_SYNCH_CLIENT_SERVER:
    {
      /* !LINKSTO Dcm.ReadDataById.ReadData.SyncFnc.Invoke,1 */
      /* !LINKSTO Dcm.ReadDataById.ReadData.SyncCS.Invoke,1 */
      InterfaceRetVal = DataCfg->ReadDataFncSync(SignalBuffer);

      if (E_OK == InterfaceRetVal)
      {
        OperationRetVal = DCM_E_OK;
      }
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      else
      {
        /* !LINKSTO Dcm.ReadDataById.ReadData.SyncFnc.RetVal.OutOfRange.Det,1 */
        /* !LINKSTO Dcm.ReadDataById.ReadData.SyncCS.RetVal.OutOfRange.Det,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_READDATA_SYNC, DCM_E_INTERFACE_RETURN_VALUE);

        /* !LINKSTO Dcm.ReadDataById.ReadData.SyncFnc.RetVal.OutOfRange.Nrc,1 */
        /* !LINKSTO Dcm.ReadDataById.ReadData.SyncCS.RetVal.OutOfRange.Nrc,1 */
        OperationRetVal = DCM_E_NOT_OK;
        *Nrc = DCM_E_GENERALREJECT;
      }
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
    }
    break;

    case DCM_USE_DATA_ASYNCH_FNC:
    case DCM_USE_DATA_ASYNCH_CLIENT_SERVER:
    {
      /* !LINKSTO Dcm.ReadDataById.ReadData.AsyncFnc.Initial.Invoke,1 */
      /* !LINKSTO Dcm.ReadDataById.ReadData.AsyncCS.Initial.Invoke,1 */
      InterfaceRetVal = DataCfg->ReadDataFncAsync(OpStatus, SignalBuffer);

      if (E_OK != InterfaceRetVal)
      {
        if (DCM_E_PENDING == InterfaceRetVal)
        {
          OperationRetVal = DCM_E_PENDING;
        }
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        else
        {
          /* !LINKSTO Dcm.ReadDataById.ReadData.AsyncFnc.RetVal.OutOfRange.Det,1 */
          /* !LINKSTO Dcm.ReadDataById.ReadData.AsyncCS.RetVal.OutOfRange.Det,1 */
          DCM_DET_REPORT_ERROR(DCM_SVCID_READDATA_ASYNC, DCM_E_INTERFACE_RETURN_VALUE);

          /* !LINKSTO Dcm.ReadDataById.ReadData.AsyncFnc.RetVal.OutOfRange.Nrc,1 */
          /* !LINKSTO Dcm.ReadDataById.ReadData.AsyncCS.RetVal.OutOfRange.Nrc,1 */
          OperationRetVal = DCM_E_NOT_OK;
          *Nrc = DCM_E_GENERALREJECT;
        }
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
      }
      else
      {
        OperationRetVal = DCM_E_OK;
      }
    }
    break;

#if (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON)
    case DCM_USE_DATA_SENDER_RECEIVER:
    {
      OperationRetVal = Dcm_DidSignals_ReadSignal_SenderReceiver(DataCfg, SignalBuffer);

      if (DCM_E_OK != OperationRetVal)
      {
        /* !LINKSTO Dcm.ReadDataById.ReadData.SR.RetVal.ConditionsNotCorrect,1 */
        /* !LINKSTO Dcm.ReadDataById.ReadData.SRS.RetVal.ConditionsNotCorrect,1 */
        /* !LINKSTO Dcm.ReadDataById.ReadData.SR.RetVal.OutOfRange.Nrc,1 */
        /* !LINKSTO Dcm.ReadDataById.ReadData.SRS.RetVal.OutOfRange.Nrc,1 */
        *Nrc = OperationRetVal;
        OperationRetVal = DCM_E_NOT_OK;
      }
    }
    break;
#endif /* (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON) */

    /* CHECK: NOPARSE */
    case DCM_USE_ECU_SIGNAL:
    {
      /* ECU signal reading is not supported currently */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      *Nrc = DCM_E_GENERALREJECT;
      OperationRetVal = DCM_E_NOT_OK;
    }
    break;
    /* CHECK: PARSE */

    /* CHECK: NOPARSE */
    default:
    {
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
    }
    break;
    /* CHECK: PARSE */
  }

  return OperationRetVal;
}

#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_ReadScalingData(
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_VAR) DataCfg,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) SignalBuffer)
{
  Std_ReturnType InterfaceRetVal = E_NOT_OK;
  Std_ReturnType OperationRetVal = DCM_E_NOT_OK;

  switch (DataCfg->AccessInterface)
  {

    case DCM_USE_DATA_SYNCH_FNC:
    case DCM_USE_DATA_SYNCH_CLIENT_SERVER:
    {
      /* !LINKSTO Dcm.UDSServices.GetScalingInformation.SyncClientServer.ReturnsFailure.ValidErrorCode.TransmitNRC,1 */
      /* !LINKSTO Dcm.UDSServices.GetScalingInformation.SyncFunction.ReturnsFailure.ValidErrorCode.TransmitNRC,1 */
      InterfaceRetVal = DataCfg->ReadScalingDataFncSync(SignalBuffer, Nrc);

      if (E_OK != InterfaceRetVal)
      {
        if(E_NOT_OK == InterfaceRetVal)
        {
          if(*Nrc == DCM_E_OK)
          {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
            /* !LINKSTO Dcm.UDSServices.GetScalingInformation.SyncFunction.ReturnsFailure.InvalidErrorCode.ReportDETError,1 */
            /* !LINKSTO Dcm.UDSServices.GetScalingInformation.SyncClientServer.ReturnsFailure.InvalidErrorCode.ReportDETError,1 */
            DCM_DET_REPORT_ERROR(DCM_SVCID_READSCALINGDATA_SYNC, DCM_E_INVALID_VALUE);
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */

            /* !LINKSTO Dcm.EB.UDSServices.GetScalingInformation.SyncFunction.ReturnsFailure.InvalidErrorCode.TransmitNRC0x10,1 */
            /* !LINKSTO Dcm.EB.UDSServices.GetScalingInformation.SyncClientServer.ReturnsFailure.InvalidErrorCode.TransmitNRC0x10,1 */
            OperationRetVal = DCM_E_NOT_OK;
            *Nrc = DCM_E_GENERALREJECT;
          }
        }
        else
        {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
          /* !LINKSTO Dcm.EB.UDSServices.GetScalingInformation.SyncClientServer.UnsupportedReturnValue.ReportDETError,1 */
          /* !LINKSTO Dcm.EB.UDSServices.GetScalingInformation.SyncFunction.UnsupportedReturnValue.ReportDETError,1 */
          DCM_DET_REPORT_ERROR(DCM_SVCID_READSCALINGDATA_SYNC, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
          /* !LINKSTO Dcm.EB.UDSServices.GetScalingInformation.SyncClientServer.UnsupportedReturnValue.TransmitNRC0x10,1 */
          /* !LINKSTO Dcm.EB.UDSServices.GetScalingInformation.SyncFunction.UnsupportedReturnValue.TransmitNRC0x10,1 */
          *Nrc = DCM_E_GENERALREJECT;
          OperationRetVal = DCM_E_NOT_OK;
        }
      }
      else
      {
        OperationRetVal = DCM_E_OK;
      }
    }
    break;

    case DCM_USE_DATA_ASYNCH_FNC:
    case DCM_USE_DATA_ASYNCH_CLIENT_SERVER:
    {
      /* !LINKSTO Dcm.UDSServices.GetScalingInformation.AsyncClientServer.ReturnsFailure.ValidErrorCode.TransmitNRC,1 */
      /* !LINKSTO Dcm.UDSServices.GetScalingInformation.AsyncFunction.ReturnsFailure.ValidErrorCode.TransmitNRC,1 */
      InterfaceRetVal = DataCfg->ReadScalingDataFncAsync(OpStatus, SignalBuffer, Nrc);

      if (E_OK != InterfaceRetVal)
      {
        if (DCM_E_PENDING == InterfaceRetVal)
        {
          OperationRetVal = DCM_E_PENDING;
        }
        else if (E_NOT_OK == InterfaceRetVal)
        {
          if(*Nrc == DCM_E_OK)
          {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
            /* !LINKSTO Dcm.UDSServices.GetScalingInformation.AsyncFunction.ReturnsFailure.InvalidErrorCode.ReportDETError,1 */
            /* !LINKSTO Dcm.UDSServices.GetScalingInformation.AsyncClientServer.ReturnsFailure.InvalidErrorCode.ReportDETError,1 */
            DCM_DET_REPORT_ERROR(DCM_SVCID_READSCALINGDATA_ASYNC, DCM_E_INVALID_VALUE);
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
            /* !LINKSTO Dcm.EB.UDSServices.GetScalingInformation.AsyncFunction.ReturnsFailure.InvalidErrorCode.TransmitNRC0x10,1 */
            /* !LINKSTO Dcm.EB.UDSServices.GetScalingInformation.AsyncClientServer.ReturnsFailure.InvalidErrorCode.TransmitNRC0x10,1 */
            *Nrc = DCM_E_GENERALREJECT;
            OperationRetVal = DCM_E_NOT_OK;
          }
        }
        else
        {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
          /* !LINKSTO Dcm.EB.UDSServices.GetScalingInformation.AsyncFunction.UnsupportedReturnValue.ReportDETError,1 */
          /* !LINKSTO Dcm.EB.UDSServices.GetScalingInformation.AsyncClientServer.UnsupportedReturnValue.ReportDETError,1 */
          DCM_DET_REPORT_ERROR(DCM_SVCID_READSCALINGDATA_ASYNC, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
          /* !LINKSTO Dcm.EB.UDSServices.GetScalingInformation.AsyncFunction.UnsupportedReturnValue.TransmitNRC0x10,1 */
          /* !LINKSTO Dcm.EB.UDSServices.GetScalingInformation.AsyncClientServer.UnsupportedReturnValue.TransmitNRC0x10,1 */
          *Nrc = DCM_E_GENERALREJECT;
          OperationRetVal = DCM_E_NOT_OK;
        }
      }
      else
      {
        OperationRetVal = DCM_E_OK;
      }
    }
    break;

    /* CHECK: NOPARSE */
    default:
    {
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
    }
    break;
    /* CHECK: PARSE */
  }

  return OperationRetVal;
}
#endif /* #if (DCM_DSP_USE_SERVICE_0X24 == STD_ON) */

#if (DCM_READ_DID_BLOCK_ID_CONFIGURED == STD_ON)
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_ReadNvmData(
  NvM_BlockIdType                                         NvRamBlockId,
  Dcm_OpStatusType                                        OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc,
  P2VAR(uint8, AUTOMATIC, DCM_VAR)                        SignalBuffer)
{
  Std_ReturnType OperationRetVal = DCM_E_NOT_OK;

  *Nrc = DCM_E_POSITIVERESPONSE;

  switch (OpStatus)
  {
    case DCM_INITIAL:
    {
      /* !LINKSTO Dcm560,1 */
      Std_ReturnType ReadBlockRetVal = NvM_ReadBlock(NvRamBlockId, SignalBuffer);

      if (E_OK == ReadBlockRetVal)
      {
        /* if the request is accepted, check the result in next call */
        OperationRetVal = DCM_E_PENDING;
      }
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      else if (E_NOT_OK == ReadBlockRetVal)
      {
        /* !LINKSTO Dcm.EB.ReadNvmData.ReadBlock.Failed,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_NVMREADDATA, DCM_E_NVM_READ_FAIL);
        OperationRetVal = DCM_E_NOT_OK;
      }
      else
      {
        /* !LINKSTO Dcm.EB.ReadNvmData.ReadBlock.InvalidValue,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_NVMREADBLOCK, DCM_E_INTERFACE_RETURN_VALUE);
        OperationRetVal = DCM_E_NOT_OK;
      }
#else
      else
      {
        OperationRetVal = DCM_E_NOT_OK;
      }
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
    }
    break;

    case DCM_PENDING:
    case DCM_CANCEL:
    {
      NvM_RequestResultType NvmRequestResult;
      /* check status of NVM read or cancel operation */
      /* !LINKSTO Dcm.EB.ReadNvmData.GetErrorStatus,1 */
      Std_ReturnType NvMOperationStatus = NvM_GetErrorStatus(NvRamBlockId, &NvmRequestResult);

      if (E_OK == NvMOperationStatus)
      {
        if (DCM_CANCEL != OpStatus)
        {
          /* OpStatus == PENDING */
          switch (NvmRequestResult)
          {
            case NVM_REQ_OK:
            case NVM_REQ_RESTORED_FROM_ROM:
            case NVM_REQ_REDUNDANCY_FAILED:
            {
              /* !LINKSTO Dcm.EB.NvM_GetErrorStatus.UsefulReturnValues,1 */
              OperationRetVal = DCM_E_OK;
            }
            break;

            case NVM_REQ_PENDING:
            {
              /* !LINKSTO Dcm.EB.ReadNvmData.GetErrorStatus.Result.Pending,1 */
              OperationRetVal = DCM_E_PENDING;
            }
            break;

            case NVM_REQ_NV_INVALIDATED:
            {
              /* !LINKSTO Dcm.EB.ReadNvmData.GetErrorStatus.Result.NvInvalidated,1 */
              *Nrc = DCM_E_CONDITIONSNOTCORRECT;
              OperationRetVal = DCM_E_NOT_OK;
            }
            break;

            case NVM_REQ_NOT_OK:
            {
              /* !LINKSTO Dcm.EB.ReadNvmData.GetErrorStatus.Result.NotOk,1 */
              *Nrc = DCM_E_GENERALREJECT;
              OperationRetVal = DCM_E_NOT_OK;
            }
            break;

            default:
            {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
              /* NvmRequestResult is an unexpected value */
              /* !LINKSTO Dcm.EB.ReadNvmData.GetErrorStatus.Result.InvalidValue.Det,1 */
              DCM_DET_REPORT_ERROR(DCM_SVCID_NVMREADBLOCK, DCM_E_INTERFACE_RETURN_VALUE);
              /* !LINKSTO Dcm.EB.ReadNvmData.GetErrorStatus.Result.InvalidValue.Nrc,1 */
              *Nrc = DCM_E_GENERALREJECT;
              OperationRetVal = DCM_E_NOT_OK;
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
            }
            break;
          }
        }
        else
        {
          /* OpStatus == CANCEL */
          if (NVM_REQ_PENDING == NvmRequestResult)
          {
            P2VAR(Dcm_SignalBufferContextType, AUTOMATIC, DCM_VAR) pDidSignalBufferContext =
                Dcm_Dsp_DidServices_GetDidSignalBuffer();
            /* Call NvM_CancelJobs() to cancel all jobs pending in the queue for the NV Block */
            /* !LINKSTO Dcm.ReadDataById.NvM.CancelJobs,1 */
            Std_ReturnType CancelJobsRetVal = NvM_CancelJobs(NvRamBlockId);

#if (DCM_ASYNC_DID_SERVICES == STD_ON)
            /* ENTER critical section
               Only use critical sections if either of services 0x22 or 0x2e or 0x2f use an Async
               service handler, for performance reasons. Only a preemption by the MainFunction can
               have an impact on the Dcm_SignalBufferContext. */
            SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
#endif

            /* Mark SignalBuffer as busy and memorize the BlockId which is keeping it locked*/
            /* !LINKSTO Dcm.EB.ReadNvmData.CancelJobs.LockBuffer,1 */
            pDidSignalBufferContext->BlockId    = NvRamBlockId;
            pDidSignalBufferContext->NvMService = DCM_DIDNVMOPERATION_READNVMDATA;

#if (DCM_ASYNC_DID_SERVICES == STD_ON)
            /* LEAVE critical section */
            SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
#endif

            /* NRC cannot be sent in case of CANCEL OpStatus, only DET can be reported if DET is
               enabled */
            if (E_OK == CancelJobsRetVal)
            {
              OperationRetVal = DCM_E_OK;
            }
#if (DCM_DEV_ERROR_DETECT == STD_ON)
            else if (E_NOT_OK == CancelJobsRetVal)
            {
              /* !LINKSTO Dcm.EB.ReadNvmData.CancelJobs.Failed,1 */
              DCM_DET_REPORT_ERROR(DCM_SVCID_NVMREADDATA, DCM_E_NVM_CANCEL_JOBS_FAILED);
            }
            else
            {
              /* !LINKSTO Dcm.EB.ReadNvmData.CancelJobs.Invalid,1 */
              DCM_DET_REPORT_ERROR(DCM_SVCID_NVMCANCELJOBS, DCM_E_INTERFACE_RETURN_VALUE);
            }
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
          }
        }
      }
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      else if (E_NOT_OK == NvMOperationStatus)
      {
        /* !LINKSTO Dcm.EB.ReadNvmData.GetErrorStatus.Failed,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_NVMREADDATA, DCM_E_NVM_READ_FAIL);
        OperationRetVal = DCM_E_NOT_OK;
      }
      else
      {
        /* !LINKSTO Dcm.EB.ReadNvmData.GetErrorStatus.InvalidValue,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_NVMGETERRORSTATUS, DCM_E_INTERFACE_RETURN_VALUE);
        OperationRetVal = DCM_E_NOT_OK;
      }
#else
      else
      {
        OperationRetVal = DCM_E_NOT_OK;
      }
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
    }
    break;

    /* CHECK: NOPARSE */
    default:
    {
      /* Defensive branch. Default case at the end of the switch statement cannot be reached */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
    }
    break;
    /* CHECK: PARSE */
  }

  if ((DCM_E_NOT_OK == OperationRetVal) && (DCM_E_POSITIVERESPONSE == *Nrc))
  {
    *Nrc = DCM_E_GENERALREJECT;
  }

  return OperationRetVal;
}
#endif /* (DCM_READ_DID_BLOCK_ID_CONFIGURED == STD_ON) */

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_WriteDataSync (
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_CONST) DataCfg,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) SignalBuffer,
  uint16 SignalLength,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc)
{
  Std_ReturnType InterfaceRetVal = E_NOT_OK;
  Std_ReturnType OperationRetVal = DCM_E_NOT_OK;

  /* Call the user function for writing the signal data */
  if (DataCfg->FixedLength == TRUE)
  {
    /* !LINKSTO Dcm.WriteDataById.WriteData.SyncFnc.FixLength.Invoke,1 */
    /* !LINKSTO Dcm.WriteDataById.WriteData.SyncCS.FixLength.Invoke,1 */
    InterfaceRetVal = DataCfg->WriteFixedDataSync(SignalBuffer, Nrc);
  }
  else
  {
    /* !LINKSTO Dcm.WriteDataById.WriteData.SyncFnc.VarLength.Invoke,1 */
    /* !LINKSTO Dcm.WriteDataById.WriteData.SyncCS.VarLength.Invoke,1 */
    InterfaceRetVal = DataCfg->WriteVariableDataSync(SignalBuffer, SignalLength, Nrc);
  }

  if (E_OK != InterfaceRetVal)
  {
    if (E_NOT_OK == InterfaceRetVal)
    {
      /* NRC is set directly by the callback */
      /* !LINKSTO Dcm.WriteDataById.WriteData.SyncFnc.FixLength.RetVal.NotOk,1 */
      /* !LINKSTO Dcm.WriteDataById.WriteData.SyncFnc.VarLength.RetVal.NotOk,1 */
      /* !LINKSTO Dcm.WriteDataById.WriteData.SyncCS.FixLength.RetVal.NotOk,1 */
      /* !LINKSTO Dcm.WriteDataById.WriteData.SyncCS.VarLength.RetVal.NotOk,1 */
      OperationRetVal = DCM_E_NOT_OK;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      if (DCM_E_OK == *Nrc)
      {
        if (DataCfg->FixedLength == TRUE)
        {
          /* !LINKSTO Dcm.WriteDataById.WriteData.SyncFnc.FixLength.RetVal.Invalid.Det,1 */
          /* !LINKSTO Dcm.WriteDataById.WriteData.SyncCS.FixLength.RetVal.Invalid.Det,1 */
          DCM_DET_REPORT_ERROR(DCM_SVCID_WRITEDATA_SYNC_FIXLENGTH, DCM_E_INVALID_VALUE);
        }
        else
        {
          /* !LINKSTO Dcm.WriteDataById.WriteData.SyncFnc.VarLength.RetVal.Invalid.Det,1 */
          /* !LINKSTO Dcm.WriteDataById.WriteData.SyncCS.VarLength.RetVal.Invalid.Det,1 */
          DCM_DET_REPORT_ERROR(DCM_SVCID_WRITEDATA_SYNC_VARLENGTH, DCM_E_INVALID_VALUE);
        }

        /* !LINKSTO Dcm.WriteDataById.WriteData.SyncFnc.FixLength.RetVal.Invalid.Nrc,1 */
        /* !LINKSTO Dcm.WriteDataById.WriteData.SyncFnc.VarLength.RetVal.Invalid.Nrc,1 */
        /* !LINKSTO Dcm.WriteDataById.WriteData.SyncCS.FixLength.RetVal.Invalid.Nrc,1 */
        /* !LINKSTO Dcm.WriteDataById.WriteData.SyncCS.VarLength.RetVal.Invalid.Nrc,1 */
        OperationRetVal = DCM_E_NOT_OK;
        *Nrc = DCM_E_GENERALREJECT;
      }
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
    }
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    else
    {
      if (DataCfg->FixedLength == TRUE)
      {
        /* !LINKSTO Dcm.WriteDataById.WriteData.SyncFnc.FixLength.RetVal.OutOfRange.Det,1 */
        /* !LINKSTO Dcm.WriteDataById.WriteData.SyncCS.FixLength.RetVal.OutOfRange.Det,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_WRITEDATA_SYNC_FIXLENGTH, DCM_E_INTERFACE_RETURN_VALUE);
      }
      else
      {
        /* !LINKSTO Dcm.WriteDataById.WriteData.SyncFnc.VarLength.RetVal.OutOfRange.Det,1 */
        /* !LINKSTO Dcm.WriteDataById.WriteData.SyncCS.VarLength.RetVal.OutOfRange.Det,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_WRITEDATA_SYNC_VARLENGTH, DCM_E_INTERFACE_RETURN_VALUE);
      }
      /* !LINKSTO Dcm.WriteDataById.WriteData.SyncFnc.FixLength.RetVal.OutOfRange.Nrc,1 */
      /* !LINKSTO Dcm.WriteDataById.WriteData.SyncFnc.VarLength.RetVal.OutOfRange.Nrc,1 */
      /* !LINKSTO Dcm.WriteDataById.WriteData.SyncCS.FixLength.RetVal.OutOfRange.Nrc,1 */
      /* !LINKSTO Dcm.WriteDataById.WriteData.SyncCS.VarLength.RetVal.OutOfRange.Nrc,1 */
      OperationRetVal = DCM_E_NOT_OK;
      *Nrc = DCM_E_GENERALREJECT;
    }
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    *Nrc = DCM_E_OK;
    OperationRetVal = DCM_E_OK;
  }

  return OperationRetVal;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_WriteDataAsync(
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_CONST) DataCfg,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) SignalBuffer,
  uint16 SignalLength,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) Nrc)
{
  Std_ReturnType InterfaceRetVal = E_NOT_OK;
  Std_ReturnType OperationRetVal = DCM_E_NOT_OK;

  /* Call the user function for writing the signal data */
  if (DataCfg->FixedLength == TRUE)
  {
    /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncFnc.FixLength.Initial.Invoke,1 */
    /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncCS.FixLength.Initial.Invoke,1 */
    InterfaceRetVal = DataCfg->WriteFixedLengthDataAsync(SignalBuffer, OpStatus, Nrc);
  }
  else
  {
    /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncFnc.VarLength.Initial.Invoke,1 */
    /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncCS.VarLength.Initial.Invoke,1 */
    InterfaceRetVal = DataCfg->WriteVarLengthDataAsync(SignalBuffer, SignalLength, OpStatus, Nrc);
  }

  if (E_OK == InterfaceRetVal)
  {
    OperationRetVal = DCM_E_OK;
  }
  else if (E_NOT_OK == InterfaceRetVal)
  {
    /* NRC is set directly by the callback */
    /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncFnc.FixLength.RetVal.NotOk,1 */
    /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncFnc.VarLength.RetVal.NotOk,1 */
    /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncCS.FixLength.RetVal.NotOk,1 */
    /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncCS.VarLength.RetVal.NotOk,1 */
    OperationRetVal = DCM_E_NOT_OK;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    if (DCM_E_OK == *Nrc)
    {
      if (DataCfg->FixedLength == TRUE)
      {
        /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncFnc.FixLength.RetVal.Invalid.Det,1 */
        /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncCS.FixLength.RetVal.Invalid.Det,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_WRITEDATA_ASYNC_FIXLENGTH, DCM_E_INVALID_VALUE);
      }
      else
      {
        /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncFnc.VarLength.RetVal.Invalid.Det,1 */
        /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncCS.VarLength.RetVal.Invalid.Det,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_WRITEDATA_ASYNC_VARLENGTH, DCM_E_INVALID_VALUE);
      }
      /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncFnc.FixLength.RetVal.Invalid.Nrc,1 */
      /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncFnc.VarLength.RetVal.Invalid.Nrc,1 */
      /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncCS.FixLength.RetVal.Invalid.Nrc,1 */
      /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncCS.VarLength.RetVal.Invalid.Nrc,1 */
      OperationRetVal = DCM_E_NOT_OK;
      *Nrc = DCM_E_GENERALREJECT;
    }
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
  }
  else if (DCM_E_PENDING == InterfaceRetVal)
  {
    /* if writing of signal is not completed */
    OperationRetVal = DCM_E_PENDING;
  }
  else
  {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    if (DataCfg->FixedLength == TRUE)
    {
      /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncFnc.FixLength.RetVal.OutOfRange.Det,1 */
      /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncCS.FixLength.RetVal.OutOfRange.Det,1 */
      DCM_DET_REPORT_ERROR(DCM_SVCID_WRITEDATA_ASYNC_FIXLENGTH, DCM_E_INTERFACE_RETURN_VALUE);
    }
    else
    {
      /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncFnc.VarLength.RetVal.OutOfRange.Det,1 */
      /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncCS.VarLength.RetVal.OutOfRange.Det,1 */
      DCM_DET_REPORT_ERROR(DCM_SVCID_WRITEDATA_ASYNC_VARLENGTH, DCM_E_INTERFACE_RETURN_VALUE);
    }
    /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncFnc.FixLength.RetVal.OutOfRange.Nrc,1 */
    /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncFnc.VarLength.RetVal.OutOfRange.Nrc,1 */
    /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncCS.FixLength.RetVal.OutOfRange.Nrc,1 */
    /* !LINKSTO Dcm.WriteDataById.WriteData.AsyncCS.VarLength.RetVal.OutOfRange.Nrc,1 */
    OperationRetVal = DCM_E_NOT_OK;
    *Nrc = DCM_E_GENERALREJECT;
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
  }

  return OperationRetVal;
}

#if (DCM_WRITE_DID_BLOCK_ID_CONFIGURED == STD_ON)
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DidSignals_WriteNvmData(
  NvM_BlockIdType                    NvRamBlockId,
  Dcm_OpStatusType                   OpStatus,
  P2CONST(uint8, AUTOMATIC, DCM_VAR) SignalBuffer)
{
  Std_ReturnType OperationRetVal = DCM_E_NOT_OK;

  boolean NvmOperationPending  = FALSE;

  switch (OpStatus)
  {
    case DCM_INITIAL:
    {
      Std_ReturnType WriteBlockRetVal = E_NOT_OK;
      /* !LINKSTO Dcm.WriteDataById.NvM.UnlockBlock,1 */
      NvM_SetBlockLockStatus(NvRamBlockId, FALSE);
      /* !LINKSTO Dcm.WriteDataById.NvM.WriteBlock,1 */
      WriteBlockRetVal = NvM_WriteBlock(NvRamBlockId, SignalBuffer);

      if (E_OK == WriteBlockRetVal)
      {
        /* if the request is accepted, check the result in next call */
        OperationRetVal = DCM_E_PENDING;
      }
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      else if (E_NOT_OK == WriteBlockRetVal)
      {
        /* !LINKSTO Dcm.EB.WriteNvmData.WriteBlock.Failed.Det,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_NVMWRITEDATA, DCM_E_NVM_WRITE_FAIL);
        /* !LINKSTO Dcm.EB.WriteNvmData.WriteBlock.Failed.Det.Nrc,1 */
        OperationRetVal = DCM_E_NOT_OK;
      }
      else
      {
        /* !LINKSTO Dcm.EB.WriteNvmData.WriteBlock.InvalidValue.Det,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_NVMWRITEBLOCK, DCM_E_INTERFACE_RETURN_VALUE);
        /* !LINKSTO Dcm.EB.WriteNvmData.WriteBlock.InvalidValue.Det.Nrc,1 */
        OperationRetVal = DCM_E_NOT_OK;
      }
#else
      else
      {
        /* !LINKSTO Dcm.EB.WriteNvmData.WriteBlock.NotOk,1 */
        OperationRetVal = DCM_E_NOT_OK;
      }
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
    }
    break;

    case DCM_PENDING:
    case DCM_CANCEL:
    {
      NvM_RequestResultType NvmRequestResult;
      /* check result of NVM write or cancel operation */
      /* !LINKSTO Dcm.WriteDataById.NvM.GetErrorStatus,1 */
      Std_ReturnType NvMOperationStatus = NvM_GetErrorStatus(NvRamBlockId, &NvmRequestResult);

      if (E_OK == NvMOperationStatus)
      {
        if (DCM_CANCEL != OpStatus)
        {
          /* OpStatus == PENDING */
          switch (NvmRequestResult)
          {
            case NVM_REQ_OK:
            {
              /* Operation successfully completed  */
              OperationRetVal = DCM_E_OK;
            }
            break;

            case NVM_REQ_PENDING:
            {
              /* !LINKSTO Dcm.EB.WriteNvmData.GetErrorStatus.Result.Pending,1 */
              OperationRetVal = DCM_E_PENDING;
            }
            break;

            case NVM_REQ_NOT_OK:
            {
              /* !LINKSTO Dcm.WriteDataById.NvM.TriggerNrc.ReqNotOk,1 */
              OperationRetVal = DCM_E_NOT_OK;
            }
            break;

            default:
            {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
              /* NvmRequestResult is an unexpected value */
              /* !LINKSTO Dcm.EB.WriteNvmData.GetErrorStatus.Result.InvalidValue.Det,1 */
              DCM_DET_REPORT_ERROR(DCM_SVCID_NVMWRITEBLOCK, DCM_E_INTERFACE_RETURN_VALUE);
              /* !LINKSTO Dcm.WriteDataById.NvM.TriggerNrc.InvalidRequestResult,1 */
              OperationRetVal = DCM_E_NOT_OK;
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
            }
            break;
          }
        }
        else
        {
          /* OpStatus == CANCEL */
          if (NVM_REQ_PENDING == NvmRequestResult)
          {
            P2VAR(Dcm_SignalBufferContextType, AUTOMATIC, DCM_VAR) pDidSignalBufferContext =
            Dcm_Dsp_DidServices_GetDidSignalBuffer();
            /* Call NvM_CancelJobs() to cancel all jobs pending in the queue for the NV Block */
            /* !LINKSTO Dcm.WriteDataById.NvM.CancelJobs,1 */
            Std_ReturnType CancelJobsRetVal = NvM_CancelJobs(NvRamBlockId);
            NvmOperationPending = TRUE;

#if (DCM_ASYNC_DID_SERVICES == STD_ON)
            /* ENTER critical section
               Only use critical sections if either of services 0x22 or 0x2e or 0x2f use an Async
               service handler, for performance reasons. Only a preemption by the MainFunction can
               have an impact on the Dcm_SignalBufferContext. */
            SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
#endif /* (DCM_ASYNC_DID_SERVICES == STD_ON) */

            /* Mark SignalBuffer as busy and memorize the BlockId which is keeping it locked*/
            /* !LINKSTO Dcm.EB.WriteNvmData.CancelJobs.LockBuffer,1 */
            pDidSignalBufferContext->BlockId    = NvRamBlockId;
            pDidSignalBufferContext->NvMService = DCM_DIDNVMOPERATION_WRITENVMDATA;

#if (DCM_ASYNC_DID_SERVICES == STD_ON)
            /* LEAVE critical section */
            SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();
#endif /* (DCM_ASYNC_DID_SERVICES == STD_ON) */

            /* NRC cannot be sent in case of CANCEL OpStatus, NRC 0x10 (GeneralReject) is returned
               by the generic handling. Only DET can be reported if DET is enabled */
            if (E_OK == CancelJobsRetVal)
            {
              OperationRetVal = DCM_E_OK;
            }
#if (DCM_DEV_ERROR_DETECT == STD_ON)
            else if (E_NOT_OK == CancelJobsRetVal)
            {
              /* !LINKSTO Dcm.EB.WriteNvmData.CancelJobs.Failed.Det,1 */
              DCM_DET_REPORT_ERROR(DCM_SVCID_NVMWRITEDATA, DCM_E_NVM_CANCEL_JOBS_FAILED);
            }
            else
            {
              /* !LINKSTO Dcm.EB.WriteNvmData.CancelJobs.InvalidValue.Det,1 */
              DCM_DET_REPORT_ERROR(DCM_SVCID_NVMCANCELJOBS, DCM_E_INTERFACE_RETURN_VALUE);
            }
#else
            else
            {
              /* !LINKSTO Dcm.EB.WriteNvmData.CancelJobs.Error,1 */
              OperationRetVal = DCM_E_NOT_OK;
            }
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
          }
        }
      }
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      else if (E_NOT_OK == NvMOperationStatus)
      {
        /* !LINKSTO Dcm.EB.WriteNvmData.GetErrorStatus.Failed,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_NVMWRITEDATA, DCM_E_NVM_WRITE_FAIL);
        /* !LINKSTO Dcm.WriteDataById.NvM.TriggerNrc.NotOk,1 */
        OperationRetVal = DCM_E_NOT_OK;
      }
      else
      {
        /* !LINKSTO Dcm.EB.WriteNvmData.GetErrorStatus.InvalidValue,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_NVMGETERRORSTATUS, DCM_E_INTERFACE_RETURN_VALUE);
        /* !LINKSTO Dcm.WriteDataById.NvM.TriggerNrc.InvalidValue,1 */
        OperationRetVal = DCM_E_NOT_OK;
      }
#else
      else
      {
        /* !LINKSTO Dcm.WriteDataById.NvM.TriggerNrc.Error,1 */
        OperationRetVal = DCM_E_NOT_OK;
      }
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
    }
    break;

    /* CHECK: NOPARSE */
    default:
    {
      /* Defensive branch. Default case at the end of the switch statement cannot be reached */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
    }
    break;
    /* CHECK: PARSE */
  }

  /* if the write operation successfully completed */
  if (DCM_E_PENDING != OperationRetVal)
  {
    /* if there is no error while writing to NVM or a job cancellation pending */
    if (FALSE == NvmOperationPending)
    {
      /* !LINKSTO Dcm.WriteDataById.NvM.LockBlock,1 */
      NvM_SetBlockLockStatus(NvRamBlockId, TRUE);
    }
  }

  return OperationRetVal;
}
#endif /* (DCM_WRITE_DID_BLOCK_ID_CONFIGURED == STD_ON) */

#if (DCM_DSP_USE_SERVICE_0X24 == STD_ON)
FUNC(Dcm_ReadScalingSignalSizeType, DCM_CODE) Dcm_Dsp_DidSignals_GetScalingInfoSize(
  uint16 SignalIndex)
{
  Dcm_ReadScalingSignalSizeType ReadScalingSignalLength = 0;
  boolean SignalFound = FALSE;

#if (DCM_NUM_READSCALING_DID_DATA > 0)
  uint32 LowSignalIdx = 0U;
  uint32 HighSignalIdx = DCM_NUM_READSCALING_DID_DATA - 1U;
#else
  TS_PARAM_UNUSED(SignalIndex);
#endif /* #if (DCM_NUM_READSCALING_DID_DATA > 0) */

  while ((HighSignalIdx >= LowSignalIdx) && (SignalFound != TRUE))
  {
    /* divide the set */
    const uint32 MidDidIdx = LowSignalIdx + ((HighSignalIdx - LowSignalIdx) >> 1U);

    if (SignalIndex < Dcm_DidScalingDataSignalsIndexingArray[MidDidIdx].DidDataSignalPosition)
    {
      /* continue searching in the lower half of the set */
        HighSignalIdx = MidDidIdx - 1U;
    }
    else if (SignalIndex > Dcm_DidScalingDataSignalsIndexingArray[MidDidIdx].DidDataSignalPosition)
    {
      /* continue searching in the upper half of the set */
      LowSignalIdx = MidDidIdx + 1U;
    }
    else
    {
      /* the did has been found in the list of statically defined dids */
      SignalFound = TRUE;
      ReadScalingSignalLength = Dcm_DidScalingDataSignalsIndexingArray[MidDidIdx].ReadScalingSignalLength;
    }
  }

  return ReadScalingSignalLength;
}
#endif /* #if (DCM_DSP_USE_SERVICE_0X24 == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* ((DCM_DID_SIGNALS_COUNT > 0) && (DCM_DSP_USE_DIDSERVICES == STD_ON)) */

/*==================[end of file]================================================================*/
