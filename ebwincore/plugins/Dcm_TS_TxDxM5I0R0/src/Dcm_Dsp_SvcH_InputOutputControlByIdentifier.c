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

/* !LINKSTO Dcm.Dsn.File.IOCBI.Impl,1 */
/* This file contains the implementation of the Diagnostic Service InputOutputControlByIdentifier */

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 11.4 (advisory)
 *     A cast should not be performed between a pointer to object type and a different pointer to
 *     object type
 *
 *     Reason:
 *     The Dcm_DidAccessInfoBaseTypePtr can safely be casted to Dcm_DidIoctlAccessInfoType
 *     in case the operation is of the type DCM_DID_OP_IOCONTROL, as the
 *     Dcm_Dsp_DidServices_GetAccessInfo() function returns a Dcm_DidIoctlAccessInfoType
 *     data in this case.
 *
 */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 11.3 (required)
  *     "A cast shall not be performed between a pointer to object type and a pointer to a different
  *     object type."
  *
  *     Reason:
  *     The Dcm_DidAccessInfoBaseTypePtr can safely be casted to Dcm_DidIoctlAccessInfoType
  *     in case the operation is of the type DCM_DID_OP_IOCONTROL, as the
  *     Dcm_Dsp_DidServices_GetAccessInfo() function returns a Dcm_DidIoctlAccessInfoType
  *     data in this case.
  */

/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * This warning appears in very unique configurations where Did related services are enabled
 * but no Did is configured. Normally the condition is not always true. The warning  does not
 * pose any risk.
 */

/*===============================[includes]======================================================*/

#include <Dcm_Trace.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <TSAutosar.h>

#include <Dcm.h>                                                      /* Declaration of Dcm APIs */
#include <Dcm_HsmDcm.h>                                   /* public statechart model definitions */
#include <Dcm_DspExternal.h>                           /* Callbacks provided by Central Diag SwC */
#include <Dcm_Int.h>                                              /* Module internal definitions */

#include <Dcm_Dsl_CommunicationServices.h>

#include <Dcm_Dsl_Supervisor.h>                                /* Dcm Dsl Supervisor header file */

#include <Dcm_Dsp_SvcH_InputOutputControlByIdentifier.h>

#include <Dcm_Dsp_DidServices.h>            /* include DidServices SW unit header for IO control */

#include <SchM_Dcm.h>                                                  /* SchM interface for Dcm */
#include <TSMem.h>                                               /* EB specific memory functions */


#if(DCM_DSP_USE_SERVICE_0X2F == STD_ON)

/*===========================[macro definitions]=================================================*/

/* Control Mask for 0x2F to parse 'controlEnableMaskRecord' */
#if (defined DCM_CONTROL_MASK_MSB)
  #error "DCM_CONTROL_MASK_MSB is already defined"
#endif
#define DCM_CONTROL_MASK_MSB                (0x80U)


/* Macros for InputOutputControlParameter */
#if (defined DCM_RETURNCONTROLTOECU)
  #error "DCM_RETURNCONTROLTOECU is already defined"
#endif
#define DCM_RETURNCONTROLTOECU              (0x00U)

#if (defined DCM_RESETTODEFAULT)
  #error "DCM_RESETTODEFAULT is already defined"
#endif
#define DCM_RESETTODEFAULT                  (0x01U)

#if (defined DCM_FREEZECURRENTSTATE)
  #error "DCM_FREEZECURRENTSTATE is already defined"
#endif
#define DCM_FREEZECURRENTSTATE              (0x02U)

#if (defined DCM_SHORTTERMADJUSTMENT)
  #error "DCM_SHORTTERMADJUSTMENT is already defined"
#endif
#define DCM_SHORTTERMADJUSTMENT             (0x03U)

#if (defined DCM_0x2F_REQUESTLENGTH)
  #error "DCM_0x2F_REQUESTLENGTH is already defined"
#endif
#define DCM_0x2F_REQUESTLENGTH              (0x03U)

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*==================[internal function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/** \brief Validates the UDS service 0x2F request received
 **
 ** \param[in] opState Status of the service processing
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOControlByIdentifier_SvcStart(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#if ((DCM_DID_SIGNALS_COUNT > 0U) || (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON))
/** \brief UDS service 0x2F internal service handler
 **
 ** \param[in] opState Status of the service processing
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOControlByIdentifier_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif

#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
/** \brief SvcContinue Handler for an Unconfigured Did.
 **
 ** \param[in] OpStatus Status of the service processing
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOControlByIdentifier_SvcCont_UnconfiguredDid(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief Read DID information after the IoControl request is finished.
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[in] OpStatus Status of the service processing.
 ** \param[in] DID Index of DID currently being processed.
 ** \param[out] DestinationBuffer Pointer to the location in the output buffer where DID data is written.
 ** \param[out] Nrc pointer to which the application shall write the NRC to be returned or whether
 **             the DID is supported or not. Valid only if the return value is E_NOT_OK.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOControl_DefaultDidRead(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  Dcm_DIDIDType DID,
  uint8 *DestinationBuffer,
  Dcm_NegativeResponseCodeType *Nrc);

/** \brief UDS service 0x2F 'returnControlToEcu' handler to request to application
 **          to return control to ECU for unconfigured Dids
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[in] opState Status of the service processing
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOReturnControlToECU_Default_Data(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  Dcm_OpStatusType OpStatus);

/** \brief UDS service 0x2F 'resetToDefault' handler to request to application
 **          to return control to ECU for unconfigured Dids
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[in] opState Status of the service processing
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOResetToDefault_Default_Data(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  Dcm_OpStatusType OpStatus);

/** \brief UDS service 0x2F 'freezeCurrentState' handler to request to application
 **          to return control to ECU for unconfigured Dids
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[in] opState Status of the service processing
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOFreezeCurrentState_Default_Data(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  Dcm_OpStatusType OpStatus);

/** \brief UDS service 0x2F 'shortTermAdjustment' handler to request to application
 **          to return control to ECU for unconfigured Dids
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[in] opState Status of the service processing
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOShortTermAdjustment_Default_Data(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  Dcm_OpStatusType OpStatus);
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */

#if (DCM_DID_SIGNALS_COUNT > 0U)
/** \brief SvcContinue Handler for a Configured Did.
 **
 ** \param[in] OpStatus Status of the service processing
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOControlByIdentifier_SvcCont_ConfiguredDid(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief Gets the status whether the 'ControlMask' is enabled for the current signal
 **
 ** \param[inout] reqData Pointer to the 'ControlMask'.
 ** \param[inout] pIOControlDIDContext holds the context information of currently processed DID.
 ** \param[in] numSignals Holds number of signals configured for DID.
 **
 ** \retval TRUE     Signal value shall be controlled
 ** \retval FALSE    Signal value should not be controlled
 */
STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_IOControl_CheckControlMask(
  P2CONST(uint8, AUTOMATIC, DCM_VAR) ReqData,
  P2CONST(Dcm_IOControlDIDContextType, AUTOMATIC, DCM_VAR) IOControlDIDContextPtr,
  uint16 NumSignals);

/** \brief The function for checking the request data length of IOControl request
 **
 ** \param[in] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[in] DidControlMask specifies did control mask type (DCM_CONTROLMASK_EXTERNAL or DCM_CONTROLMASK_NOT_DEFINED)
 ** \param[in] DidControlMaskSize specifies did control mask size (1 - 4)
 **
 ** \retval DCM_E_OK      Operation completed successfully
 ** \retval DCM_E_NOT_OK  Operation completed with errors
 */
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOControlDidRequestLengthChk(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  uint8 DidControlMask,
  uint8 DidControlMaskSize);
#else /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOControlDidRequestLengthChk(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */

/** \brief UDS service 0x2F 'returnControlToEcu' handler to request to application
 **          to return control to ECU
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[in] instIdx Current HSM instance Id.
 ** \param[in] opState Status of the service processing
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOReturnControlToECU_Data(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  uint8 instIdx,
  Dcm_OpStatusType OpStatus);

/** \brief UDS service 0x2F 'resetToDefault' handler to request to application
 **          to reset an IOControl to default value
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[in] instIdx Current HSM instance Id.
 ** \param[in] opState Status of the service processing.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOResetToDefault_Data(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  uint8 instIdx,
  Dcm_OpStatusType OpStatus);

/** \brief UDS service 0x2F 'FreezeCurrentState' handler to request to application
 **          to freeze the current state of an IOControl
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[in] instIdx Current HSM instance Id.
 ** \param[in] opState Status of the service processing.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOFreezeCurrentState_Data(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  uint8 instIdx,
  Dcm_OpStatusType OpStatus);

/** \brief UDS service 0x2F 'ShortTermAdjustment' handler to request to application
 **         to adjust the IO signal in case DcmDspDidControlMask is set to DCM_CONTROLMASK_EXTERNAL
 **
 ** \param[in]  pIOControlDIDContext Holds the context information of the IOControl request
 ** \param[out] nrc Negative response code variable
 ** \param[in]  signalLength length of the signal being processed
 ** \param[in]  DcmDspDidControlMaskSize value defined by parameter DcmDspDidControlMaskSize
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_HandleShortTermAdjustmentSynchMaskExternal
(
  P2VAR(Dcm_IOControlDIDContextType, AUTOMATIC, DCM_VAR)  pIOControlDIDContext,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) nrc,
  uint16  signalLength,
  uint8   DcmDspDidControlMaskSize
);
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */

/** \brief UDS service 0x2F 'ShortTermAdjustment' handler to request to application
 **         to adjust the IO signal in case DcmDspDidControlMask is not enabled
 **
 ** \param[in]  dataCfg Holds the configuration of signal being processed
 ** \param[out] nrc Negative response code variable
 ** \param[in]  signalLength length of the signal being processed
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_HandleShortTermAdjustmentSynchMaskNotEnabled
(
  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_CONST)            dataCfg,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) nrc,
  uint16  signalLength
);

/** \brief UDS service 0x2F 'ShortTermAdjustment' handler to request to application
 **         to adjust the IO signal
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[in]    pIOControlDIDContext Holds the context information of the IOControl request
 ** \param[out]   nrc Negative response code variable
 ** \param[in]    didDataPosn bit-position of signal data in request buffer
 ** \param[in]    signalLength length of the signal being processed
 ** \param[in]    controlMaskStatus specifies if Signal value shall be controlled or not
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_HandleShortTermAdjustmentSynch
(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR)           pMsgContext,
  P2VAR(Dcm_IOControlDIDContextType, AUTOMATIC, DCM_VAR)  pIOControlDIDContext,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) nrc,
  uint16  didDataPosn,
  uint16  signalLength,
  uint8   instIdx,
  boolean controlMaskStatus
);

/** \brief UDS service 0x2F 'ShortTermAdjustment' handler to request to application
 **         to adjust the IO signal
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[in] instIdx Current HSM instance Id.
 ** \param[in] opState Status of the service processing.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOShortTermAdjustment_Data(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  uint8 instIdx,
  Dcm_OpStatusType OpStatus);

/** \brief Copies the data of currently processed signal to signalBuffer
 **
 ** \param[in] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[in] dataCfg holds the configuration of signal being processed.
 ** \param[in] didDataPosition Position in bits of the DID data.
 ** \param[out] signalBuffer Status of the service processing.
 ** \param[in] instIdx Current HSM instance Id.
 **
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_SignalDataCopying(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_CONST) dataCfg,
  uint16 didDataPosition,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) signalBuffer,
  uint8 instIdx);

/** \brief Reads the signal data
 **
 ** \param[in] OpStatus Status of the service processing
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[in] instIdx Current HSM instance Id.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOControl_ReadSignals(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  uint8 instIdx);

/** \brief Validates the return values of 'IOControl' interface calls
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[inout] pIOControlDIDContext holds the context information of currently processed DID.
 ** \param[in] dataCfg holds the configuration of signal being processed.
 ** \param[in] nrc NRC occurred during condition checks.
 ** \param[in] interfaceRetVal holds interface function return value.
 ** \param[in] interfaceID holds interface ID value
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOControl_InterfaceRetValCheck(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  P2VAR(Dcm_IOControlDIDContextType, AUTOMATIC, DCM_VAR) pIOControlDIDContext,
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_CONST) dataCfg,
  uint8 nrc, Std_ReturnType InterfaceRetVal, uint8 interfaceID);

/** \brief Sets up the read operation for a single DID by index.
 **
 ** \param[in] pReadSignalContext Signal reading context to use for this read operation.
 ** \param[in] DidIndex Index of DID currently being processed
 ** \param[in] Pointer to Output Buffer
 ** \param[in] BufferSize The maximum size of the buffer
 ** \param[in] BufferOffset The offset from which writing into the buffer starts.
 ** \param[in] EndiannessConversion Whether endianness conversion should be performed on
 **            the DID data
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataOfSingleDid_SetupByIndex(
  P2VAR(Dcm_ReadSignalContextType, AUTOMATIC, DCM_VAR) pReadSignalContext,
  Dcm_DidConfigIdxType DidIndex,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) OutputBuffer,
  uint32 BufferSize,
  uint32 BufferOffset,
  Dcm_PerformEndiannessConversionType EndiannessConversion);

#endif /*#if (DCM_DID_SIGNALS_COUNT > 0U)*/

/** \brief This function is used to execute the InputOutputControlByIdentifier service functionality
 **        after the DidService unit was locked by InputOutputControlByIdentifier.
 **
 ** \param[in]    OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_InputOutputControlByIdentifier_ServiceExecution(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
/** \brief This function handles the ReturnControlToECU for all Unconfigured Signals
 **        when the configuration parameter DcmDspDidEnableDefaultInterfaces is set to TRUE.
 **
 ** \param[in] opState Status of the service processing
 ** \retval       NONE
 */
STATIC FUNC(void, DCM_CODE)
  Dcm_Dsp_InputOutputControlByIdentifier_UnconfiguredDID_ReturnControlToECUForAllSignals(Dcm_OpStatusType OpStatus);
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */

#if (DCM_RETURNCONTROLTOECU_ENABLED == STD_ON)
/** \brief This function handles the ReturnControlToECU for all configured Signals.
 **
 ** \param[in]    NONE
 ** \retval       NONE
 */
STATIC FUNC(void, DCM_CODE)
  Dcm_Dsp_InputOutputControlByIdentifier_ConfiguredDID_ReturnControlToECUForAllSignals(void);

#if (DCM_DID_SIGNALS_COUNT > 0U)
/** \brief UDS service 0x2F 'ShortTermAdjustment' handler to request to application
 **         to adjust the IO signal
 **
 ** \param[in]    dataCfg Holds the configuration of signal being processed
 ** \param[out]   curNrc Negative response code variable
 ** \param[in]    DcmDspDidControlMask value defined by parameter DcmDspDidControlMask
 ** \param[in]    DcmDspDidControlMaskSize value defined by parameter DcmDspDidControlMaskSize
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReturnControlToEcuWithAllOnesMask(
  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_CONST) dataCfg,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) curNrc,
  uint8 DidControlMask,
  uint8 DidControlMaskSize
);
#else /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReturnControlToEcuWithAllOnesMask(
  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_CONST) dataCfg,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) curNrc
);
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
#endif /* #if (DCM_DID_SIGNALS_COUNT > 0U) */
#endif /* #if (DCM_RETURNCONTROLTOECU_ENABLED == STD_ON) */
#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief Context variable used to hold information regarding the processing of
           returning control to ECU for signals */
STATIC VAR(Dcm_IOReturnControlToEcuStatusType, DCM_VAR) Dcm_ReturnControlToEcuContext;

#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
/** \brief Context variable used to hold information regarding the status of the
 *         IoControl functionality when the DcmDspDidEnableDefaultInterfaces configuration
 *         parameter is set to TRUE. */
STATIC VAR(Dcm_DefaultInterfacesStatusType, DCM_VAR) Dcm_IoControlDefaultStatus;
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */

#if (DCM_DID_SIGNALS_COUNT > 0U)
/* Holds the context information of a IOControl request */
/* Rationale for mapping Dcm_IOControlDIDContext to NOINIT section.
 *
 * Members of Dcm_IOControlDIDContextType are initialized and accessed as described below:
 * RequestProcessingSt : Initialized in Dcm_Dsp_InputOutputControlByIdentifier_SvcH()
 *                       before being accessed from
 *                       Dcm_Dsp_IOControlByIdentifier_SvcStart().
 * SignalInd  : Initialized in Dcm_Dsp_IOControlByIdentifier_SvcStart() before being
 *              accessed from the Dcm_Dsp_IOShortTermAdjustment_Data().
 */
STATIC VAR(Dcm_IOControlDIDContextType, DCM_VAR) Dcm_IOControlDIDContext[DCM_NUM_PROTOCOL];

/** \brief Holds the context information of IOControl to access signals.
 **
 ** If parallel execution is possible one context per instance is needed */
STATIC VAR(Dcm_ReadSignalContextType, DCM_VAR) Dcm_IoctrlReadSignalContext;
#endif /*#if (DCM_DID_SIGNALS_COUNT > 0U)*/

#if (DCM_NUM_PROTOCOL > 1U)
#if (defined DCM_IO_CONTROL_DID_CONTEXT)
  #error "DCM_IO_CONTROL_DID_CONTEXT is already defined"
#endif
#define DCM_IO_CONTROL_DID_CONTEXT(instIdx) Dcm_IOControlDIDContext[(instIdx)]
#else
#if (defined DCM_IO_CONTROL_DID_CONTEXT)
  #error "DCM_IO_CONTROL_DID_CONTEXT is already defined"
#endif
#define DCM_IO_CONTROL_DID_CONTEXT(instIdx) Dcm_IOControlDIDContext[0U]
#endif /* #if (DCM_NUM_PROTOCOL > 1U) */

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/** \brief This variable holds if the InputOutputControlByIdentifier service has locked
 **         the DidServices unit.
 **  DCM_TRYING_LOCK       - InputOutputControlByIdentifier service has the lock over DidServices
 **                          unit.
 **  DCM_EXECUTING_SERVICE - InputOutputControlByIdentifier does not have the lock over DidServices
 **                          unit.
 */
STATIC VAR(uint8, DCM_VAR) Dcm_InputOutputControlByIdentifier_LockStatus = DCM_TRYING_LOCK;

#if(DCM_RETURNCONTROLTOECU_ENABLED == STD_ON)
/** \brief This variable holds if the ReturnControlToEcuForAllSignal has locked
 **         the DidServices unit.
 **  TRUE  - ReturnControlToEcuForAllSignal has the lock over DidServices unit.
 **  FALSE - ReturnControlToEcuForAllSignal does not have the lock over DidServices unit.
 */
STATIC VAR(boolean, DCM_VAR) Dcm_ReturnControlToEcuForAllSignals_HasTheLock = FALSE;
#endif /* #if(DCM_RETURNCONTROLTOECU_ENABLED == STD_ON) */

#define DCM_STOP_SEC_VAR_INIT_8
#include <Dcm_MemMap.h>

/*==================[internal function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOControlByIdentifier_SvcStart(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  /* Holds the index of the currently processed DID in the configuration array */
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  /* Check request length (min. length  check) */
  if (pMsgContext->reqDataLen < DCM_0x2F_REQUESTLENGTH)
  {
    /* Incorrect request length */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    serviceProcessStatus = DCM_E_DONE;
  }
  else if ((pMsgContext->reqData[2U] == DCM_RETURNCONTROLTOECU) ||
           (pMsgContext->reqData[2U] == DCM_RESETTODEFAULT) ||
           (pMsgContext->reqData[2U] == DCM_FREEZECURRENTSTATE) ||
           (pMsgContext->reqData[2U] == DCM_SHORTTERMADJUSTMENT))
  {
#if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0)
#if (DCM_DID_SIGNALS_COUNT > 0U)
    Std_ReturnType ReturnVal = DCM_E_OK;
#endif
    Dcm_NegativeResponseCodeType nrc = DCM_E_OK;
    /* Holds the instance id */
    uint8 instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                    DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);
    /* Calculate the requested DID */
    uint8 HighByte = (uint8)pMsgContext->reqData[0U];
    uint8 LowByte = (uint8)pMsgContext->reqData[1U];

    uint16 reqDid =(uint16) DCM_COMPOSE_16BIT_WORD_FROM_MSB_AND_LSB(HighByte, LowByte);

#endif /* #if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0) */
    Dcm_DidInfoContextType DidInfoContext;
#if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0)

    Dcm_Dsp_DidServices_GetDidInfo_Init(&DidInfoContext, reqDid, DCM_DID_OP_IOCONTROL, instIdx);

    /* pre-check if the Did is configured by an Id (DcmDspDid) */
    if (Dcm_Dsp_DidServices_GetDidIndex(reqDid, &DidInfoContext.DidInfoContext.ConfigIdx) != FALSE)
    {
      const Std_ReturnType result = Dcm_Dsp_DidServices_GetDidInfo_Execute(&DidInfoContext);
      if ( (result != DCM_E_OK) ||
           (DidInfoContext.DidInfoContext.ConfigType == DCM_DID_UNCONFIGURED) )
      {
#if (DCM_DID_SIGNALS_COUNT > 0U)
        ReturnVal = DCM_E_NOT_OK;
#endif
        nrc = DidInfoContext.BaseContext.nrc;
      }
      else
      {
#if (DCM_DID_SIGNALS_COUNT > 0U)
        ReturnVal = DCM_E_OK;
#else
        /* !LINKSTO Dcm.EB.InputOutputControlByIdentifier.ReturnControlToECU.NoDidSignals.NRC,1 */
        /* !LINKSTO Dcm.EB.InputOutputControlByIdentifier.ResetToDefault.NoDidSignals.NRC,1 */
        /* !LINKSTO Dcm.EB.InputOutputControlByIdentifier.FreezeCurrentState.NoDidSignals.NRC,1 */
        /* !LINKSTO Dcm.EB.InputOutputControlByIdentifier.ShortTermAdjustment.NoDidSignals.NRC,1 */
        nrc = DCM_E_REQUESTOUTOFRANGE;
#endif
      }
    }
    else
    {

      /* Did is configured by a DidRange or unconfigured, both is unsupported by this SvcH */
#if (DCM_DID_SIGNALS_COUNT > 0U)
      ReturnVal = DCM_E_NOT_OK;
#endif
#endif /* #if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0) */
#if(DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
    /* Did has not been found. Default read option is active */
    DidInfoContext.DidInfoContext.ConfigType = DCM_DID_UNCONFIGURED_DEFAULT_INTERFACE;
#endif /* DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ */

#if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0)
      nrc = DidInfoContext.BaseContext.nrc;
    }
#endif /* #if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0) */

#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
    /* If the type of the Did is DCM_DID_UNCONFIGURED and the Did Default Interface is enabled,
     * call the SvcCont handler with the DCM_INITIAL OpStatus. */
    /* Deviation TASKING-1 */
    if(DCM_DID_UNCONFIGURED_DEFAULT_INTERFACE == DidInfoContext.DidInfoContext.ConfigType)
    {
      /* The requested Did is unconfigured. */
      Dcm_IoControlDefaultStatus.IsDidUnconfigured = TRUE;
      /* Fall through to request data processing. */
      serviceProcessStatus = Dcm_Dsp_IOControlByIdentifier_SvcCont(DCM_INITIAL,
                                                                           pMsgContext);
    }
    else
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */
    {

#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
      /* The requested Did is configured. */
      Dcm_IoControlDefaultStatus.IsDidUnconfigured = FALSE;

#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */
#if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0)
#if (DCM_DID_SIGNALS_COUNT > 0U)
      if(ReturnVal == DCM_E_OK)
      {
        /* Holds the context information of the IOControl request */
        P2VAR(Dcm_IOControlDIDContextType, AUTOMATIC, DCM_VAR) pIOControlDIDContext =
                                                        &DCM_IO_CONTROL_DID_CONTEXT(instIdx);

        P2VAR(Dcm_DidInfoContextExtType, AUTOMATIC, DCM_VAR) DidInfo = &DidInfoContext.DidInfoContext;
        Dcm_DidAccessInfoBaseTypePtr AccessInfo =
          Dcm_Dsp_DidServices_GetAccessInfo(DidInfo->InfoIdx, DidInfo->DidOp);

        /* Deviation MISRAC2012-1 */
        /* Deviation MISRA-1 */
        P2CONST(Dcm_DidIoctlAccessInfoExtType, AUTOMATIC, DCM_CONST) AccessInfoExt =
          &((const Dcm_DidIoctlAccessInfoType*)AccessInfo)->AccessInfoExt;
        /* Verifying whether requested 'inputOutputControlParameter' is enabled or not */
        /* !LINKSTO Dcm.EB.InputOutputControlByIdentifier.ReturnControlToECU.NoDidSignals.NRC,1 */
        /* !LINKSTO Dcm.EB.InputOutputControlByIdentifier.ResetToDefault.NoDidSignals.NRC,1 */
        /* !LINKSTO Dcm.EB.InputOutputControlByIdentifier.FreezeCurrentState.NoDidSignals.NRC,1 */
        /* !LINKSTO Dcm.EB.InputOutputControlByIdentifier.ShortTermAdjustment.NoDidSignals.NRC,1 */
        pIOControlDIDContext->DidConfigIdx = DidInfoContext.DidInfoContext.ConfigIdx;
        if ( (((pMsgContext->reqData[2U] == DCM_RETURNCONTROLTOECU) &&
              (AccessInfoExt->ReturnControlToEcuEnable == TRUE)) ||
             ((pMsgContext->reqData[2U] == DCM_RESETTODEFAULT) &&
              (AccessInfoExt->ResetToDefaultEnable == TRUE)) ||
             ((pMsgContext->reqData[2U] == DCM_FREEZECURRENTSTATE) &&
              (AccessInfoExt->FreezeCurrentStateEnable == TRUE)) ||
             ((pMsgContext->reqData[2U] == DCM_SHORTTERMADJUSTMENT) &&
              (AccessInfoExt->ShortTermAdjustmentEnable == TRUE)) ) &&
             (Dcm_DidConfig[DidInfoContext.DidInfoContext.ConfigIdx].NumSignals != 0)
           )
        {
          /* Check the request data length (total length check) */
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
          uint8 DidControlMask     = AccessInfoExt->DidControlMask;
          uint8 DidControlMaskSize = AccessInfoExt->DidControlMaskSize;
          ReturnVal = Dcm_Dsp_IOControlDidRequestLengthChk(pMsgContext, DidControlMask, DidControlMaskSize);
#else /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
         ReturnVal = Dcm_Dsp_IOControlDidRequestLengthChk(pMsgContext);
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */

          if (ReturnVal == DCM_E_OK)
          {
            /* Pointer for storing the DID and signals */
            Dcm_MsgType dataPtr = &pMsgContext->resData[pMsgContext->resDataLen];
            /* Initialize the IOControl context for starting the signal processing */
            pIOControlDIDContext->SignalInd = 0U;
            /* Update the processing status of IOControl request to 'request processing' */
            pIOControlDIDContext->RequestProcessingSt = DCM_DID_REQUEST_PROCESSING;
            /* Update status to indicate initial signal processing */
            pIOControlDIDContext->InitSignalProcessing = TRUE;
            /* Update the IOControl request received */
            pIOControlDIDContext->IOControlParameter = pMsgContext->reqData[2U];
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
            /* Populate DcmDspDidControlMask, DcmDspDidControlMaskSize and mask value */
            pIOControlDIDContext->DcmDspDidControlMask = DidControlMask;
            pIOControlDIDContext->DcmDspDidControlMaskSize = DidControlMaskSize;
            pIOControlDIDContext->DcmDspDidControlMaskUint32 = 0U;
            pIOControlDIDContext->DcmDspDidControlMaskUint16 = 0U;
            pIOControlDIDContext->DcmDspDidControlMaskUint8 = 0U;
            if (DidControlMask == DCM_CONTROLMASK_EXTERNAL)
            {
              /* !LINKSTO Dcm.EB.InputOutputControlByIdentifier.ControlMaskCopy,1 */
              switch (DidControlMaskSize)
              {
                case 1U:
                  TS_MemCpy(&(pIOControlDIDContext->DcmDspDidControlMaskUint8),
                            &pMsgContext->reqData[pMsgContext->reqDataLen - 1U],
                            (uint16)1);
                  break;

                case 2U:
                  TS_MemCpy(&(pIOControlDIDContext->DcmDspDidControlMaskUint16),
                            &pMsgContext->reqData[pMsgContext->reqDataLen - 2U],
                            (uint16)2);
                  break;

                case 3U:
                  TS_MemCpy(&(pIOControlDIDContext->DcmDspDidControlMaskUint32),
                            &pMsgContext->reqData[pMsgContext->reqDataLen - 3U],
                            (uint16)3);
                  break;

                case 4U:
                  TS_MemCpy(&(pIOControlDIDContext->DcmDspDidControlMaskUint32),
                            &pMsgContext->reqData[pMsgContext->reqDataLen - 4U],
                            (uint16)4);
                  break;

                /* CHECK: NOPARSE */
                default:
                  DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_INPUTOUTPUTCONTROLBYIDENTIFIER);
                  break;
                /* CHECK: PARSE */
              }
            }
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
            /* Assemble the DID in the response.*/
            dataPtr[0U] = pMsgContext->reqData[0U];
            dataPtr[1U] = pMsgContext->reqData[1U];
            /* Copy IOControl value to response buffer */
            dataPtr[2U] = pMsgContext->reqData[2U];
            /* DID size + IOControl Parameter byte */
            pMsgContext->resDataLen += DCM_DID_ID_SIZE + 1U;
            /* Initialie context for accessing signals */
            Dcm_IoctrlReadSignalContext.SignalInd = 0U;
            Dcm_IoctrlReadSignalContext.NewSignal = TRUE;

            /* Fall through to request data processing */
            serviceProcessStatus = Dcm_Dsp_IOControlByIdentifier_SvcCont(DCM_INITIAL,
                                                                                 pMsgContext);
          }
          else
          {
            /* Set NRC and exit processing */
            Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
            serviceProcessStatus = DCM_E_DONE;
          }
        }
        else
        {
          /* Requested 'inputOutputControlParameter' not enabled, send nrc */
          Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
          serviceProcessStatus = DCM_E_DONE;
        }
      }
      else
#endif /* #if (DCM_DID_SIGNALS_COUNT > 0U)*/
      {
        /* Verification failed; set the respective error code */
        Dcm_ExternalSetNegResponse(pMsgContext, nrc);
        serviceProcessStatus = DCM_E_DONE;
      }
#endif /* #if (DCM_DSP_DIDSERVICES_NUM_DIDS > 0) */
    }
  }
  else
  {
    /* !LINKSTO Dcm.EB.IOControl.InvalidParameter,1 */
    /*  the value contained in the inputOuptputControlParameter is invalid */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
    serviceProcessStatus = DCM_E_DONE;
  }

  return serviceProcessStatus;
}
#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOControlByIdentifier_SvcCont_UnconfiguredDid(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType,
  AUTOMATIC, DCM_VAR) pMsgContext)
{
  /* Holds the index of the currently processed DID in the configuration array. */
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;

  /* Request data processing. */
  switch (pMsgContext->reqData[2])
  {
    case DCM_RETURNCONTROLTOECU:
      /* Call 'Dcm_DefaultDIDReturnControlToECU' handler to request to application to return control to ECU */
      ServiceProcessStatus = Dcm_Dsp_IOReturnControlToECU_Default_Data(pMsgContext,
                                                                       OpStatus);
      break;

    case DCM_RESETTODEFAULT:
      /* Call 'Dcm_DefaultDIDResetToDefault' handler to request to application to reset an IOControl
       * to default value. */
      ServiceProcessStatus = Dcm_Dsp_IOResetToDefault_Default_Data(pMsgContext,
                                                                   OpStatus);
      break;

    case DCM_FREEZECURRENTSTATE:
      /* Call 'Dcm_DefaultDIDFreezeCurrentState' handler to request to application to freeze the
       * current state of an InputOutput Control. */
      ServiceProcessStatus = Dcm_Dsp_IOFreezeCurrentState_Default_Data(pMsgContext,
                                                                       OpStatus);
      break;

    case DCM_SHORTTERMADJUSTMENT:
      /* Call 'Dcm_DefaultDIDShortTermAdjustment' handler to request to application to adjust the IO signal. */
      ServiceProcessStatus = Dcm_Dsp_IOShortTermAdjustment_Default_Data(pMsgContext,
                                                                        OpStatus);
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement. */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_INPUTOUTPUTCONTROLBYIDENTIFIER);
      break;
    /* CHECK: PARSE */
  }

  return ServiceProcessStatus;
}
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */

#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_InputOutputControlByIdentifier_UnconfiguredDID_ReturnControlToECUForAllSignals(Dcm_OpStatusType OpStatus)
{
  Std_ReturnType DefaultRetVal = E_NOT_OK;

  /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.ReturnControlToECUForAll,1 */
  DefaultRetVal = Dcm_DefaultDIDReturnControlToECUForAll(OpStatus);

  /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.ReturnControlToECUForAll.DefaultSession.PENDING,1 */
  if(DCM_E_PENDING == DefaultRetVal)
  {
    /* Function Dcm_DefaultDIDReturnControlToECUForAll() is returning DCM_E_PENDING */
    Dcm_IoControlDefaultStatus.ReturnControlToECUForAllSignalsStatus = DCM_RET_CTRL_DEFAULT_PENDING;
#if (DCM_DSP_RETURN_CONTROLTOECU_TIMEOUT_REPORT == STD_ON)
    /* Dcm_DefaultDIDReturnControlToECUForAll operation returns PENDING, so increment the timeoutCount */
    Dcm_ReturnControlToEcuContext.timeoutCount++;
#endif /* (DCM_DSP_RETURN_CONTROLTOECU_TIMEOUT_REPORT == STD_ON) */
  }
  /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.ReturnControlToECUForAll.DefaultSession.NOT_OK,1 */
  else
  {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    if((E_OK != DefaultRetVal) && (E_NOT_OK != DefaultRetVal))
    {
      /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.ReturnControlToECUForAll.DefaultSession.DET,1 */
      /* Report Development Error for invalid return value */
      DCM_DET_REPORT_ERROR(DCM_SERVID_DEFAULTDIDRETURNCONTROLTOECUFORALL_DEFAULTINTERFACE, DCM_E_INTERFACE_RETURN_VALUE);
    }
    else
    {
      /* Do nothing. The result is either E_OK or E_NOT_OK. */
    }
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */
    /* Processing for all signals and DIS's completed. Set the finished status finally. */
    Dcm_IoControlDefaultStatus.ReturnControlToECUForAllSignalsStatus = DCM_RET_CTRL_DEFAULT_FINISHED;
#if (DCM_DSP_RETURN_CONTROLTOECU_TIMEOUT_REPORT == STD_ON)
    /* returnControlToEcu operation completed, so reset the timeoutCount. */
    Dcm_ReturnControlToEcuContext.timeoutCount = 0U;
#endif /* (DCM_DSP_RETURN_CONTROLTOECU_TIMEOUT_REPORT == STD_ON) */
  }
}
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */

#if ((DCM_DID_SIGNALS_COUNT > 0U ) || (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON))
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOControlByIdentifier_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
  if(TRUE == Dcm_IoControlDefaultStatus.IsDidUnconfigured)
  {
    serviceProcessStatus = Dcm_Dsp_IOControlByIdentifier_SvcCont_UnconfiguredDid(OpStatus,
                                                                         pMsgContext);
  }
  else
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */
  {
#if (DCM_DID_SIGNALS_COUNT > 0U)
    serviceProcessStatus = Dcm_Dsp_IOControlByIdentifier_SvcCont_ConfiguredDid(OpStatus,
                                                                        pMsgContext);
#endif /* (DCM_DID_SIGNALS_COUNT > 0U) */
  }

  return serviceProcessStatus;
}
#endif /* ((DCM_DID_SIGNALS_COUNT > 0U ) || (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)) */

#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
/* Sub-handler for DefaultDidRead function for IoControl. */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOControl_DefaultDidRead(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  Dcm_DIDIDType DID,
  uint8 *DestinationBuffer,
  Dcm_NegativeResponseCodeType *Nrc)
{
  /* Return value of the function DefaultDidRead(). */
  Std_ReturnType ServiceProcessStatusRead = DCM_E_ABORT;

  uint32 Length = 0U;

  /* Call the user's Default Read custom implementation. */
  /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ReturnControlToECU.OK,1 */
  /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ResetToDefault.OK,1 */
  /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.FreezeCurrentState.OK,1 */
  /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ShortTermAdjustment.OK,1 */
  ServiceProcessStatusRead = Dcm_DefaultDIDRead(
                                  OpStatus,
                                  DID,
                                  DestinationBuffer,
                                  &Length,
                                  Nrc);

  if(E_OK == ServiceProcessStatusRead)
  {
    /* Check for a possible buffer overflow. */
    if(((pMsgContext->reqDataLen) + Length) <= (pMsgContext->resMaxDataLen))
    {
      /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ReadOK,1 */
      /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ReadOKPosRes,1 */
      /* Create the positive response. */
      pMsgContext->resData[0U] = pMsgContext->reqData[0U];
      pMsgContext->resData[1U] = pMsgContext->reqData[1U];
      /* Copy IOControl value to response buffer. */
      pMsgContext->resData[2U] = pMsgContext->reqData[2U];
      /* Update length. */
      pMsgContext->resDataLen += DCM_DID_ID_SIZE + 1U;
      /* Update the length with the data received from Dcm_DefaultDIDRead() function. */
      pMsgContext->resDataLen += Length;
    }
    else
    {

      /* Buffer is too small for the response data; Set the Negative Response Code. */
      /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.ReturnControlToECU.NRC0x14,1 */
      /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.ResetToDefault.NRC0x14,1 */
      /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.FreezeCurrentState.NRC0x14,1 */
      /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.ShortTermAdjustment.NRC0x14,1 */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_RESPONSETOOLONG);
    }

    /* Dcm_DefaultDIDRead() function is not returning DCM_E_PENDING. Clear the flag. */
    Dcm_IoControlDefaultStatus.IsDefaultDidReadReturnPending = FALSE;
    /* Processing done. */
     ServiceProcessStatusRead = DCM_E_DONE;
  }
  else if(E_NOT_OK == ServiceProcessStatusRead)
  {
    if(DCM_E_POSITIVERESPONSE == *Nrc)
    {
      /* Return generalReject as a Negative Response Code. */
      /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.0x00,1 */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
    }
    else
    {
      /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ReadNRC,1 */
      Dcm_ExternalSetNegResponse(pMsgContext, *Nrc);
    }

    /* Dcm_DefaultDIDRead() function is not returning DCM_E_PENDING. Clear the flag. */
    Dcm_IoControlDefaultStatus.IsDefaultDidReadReturnPending = FALSE;

    /* Processing done. */
    ServiceProcessStatusRead = DCM_E_DONE;
  }
  else if (DCM_E_PENDING == ServiceProcessStatusRead)
  {
    /* Stop calling function Dcm_DefaultDIDShortTermAdjustment() in the next Dcm_MainFunction(). */
    Dcm_IoControlDefaultStatus.IsDefaultDidReadReturnPending = TRUE;

    /* ServiceProcessStatus remains DCM_E_PENDING */
    /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.Pending,1 */
  }
  else
  {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    /* Report the DCM_E_INTERFACE_RETURN_VALUE DET error. */
    /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.UnsupportedError.DET,1 */
    DCM_DET_REPORT_ERROR(DCM_SERVID_DEFAULTDIDREAD_DEFAULTINTERFACE, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */

    /* Dcm_DefaultDIDRead() function is not returning DCM_E_PENDING. Clear the flag. */
    Dcm_IoControlDefaultStatus.IsDefaultDidReadReturnPending = FALSE;

    /* Return generalReject as a Negative Response Code. */
    /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.UnsupportedError.NRC,1 */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
    /* Processing done. */
    ServiceProcessStatusRead = DCM_E_DONE;
  }

  return ServiceProcessStatusRead;
}

/* Sub-handler for IOControl 'ReturnControlToECU'. Default Interface option enabled. */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOReturnControlToECU_Default_Data(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  Dcm_OpStatusType OpStatus)
{
  /* Return value for the function Dcm_DefaultDIDShortTermAdjustment(). */
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  Dcm_NegativeResponseCodeType Nrc = DCM_E_GENERALREJECT;
  /* Holds the Did from the Request Message. */
  Dcm_DIDIDType ReqDID;

  /* Build the Did from the first and second byte from the Request Message. */
  ReqDID = (uint16)DCM_COMPOSE_16BIT_WORD_FROM_MSB_AND_LSB(pMsgContext->reqData[0U], pMsgContext->reqData[1U]);

  /* The DefaultInterface for IoControl is called only when the function Dcm_DefaultDIDRead() is not returning Pending. */
  if(FALSE == Dcm_IoControlDefaultStatus.IsDefaultDidReadReturnPending)
  {
    /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.ReturnControlToECU,1 */
    /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ReturnControlToECU.Timeout.Cancellation,1 */
    /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ReturnControlToECU.ProtocolPreemption.Cancellation,1 */
    ServiceProcessStatus = Dcm_DefaultDIDReturnControlToECU(
                             OpStatus,
                             ReqDID,
                             &Nrc);

    if(E_OK == ServiceProcessStatus)
    {
      ServiceProcessStatus = Dcm_Dsp_IOControl_DefaultDidRead(
                              DCM_INITIAL,
                              pMsgContext,
                              ReqDID,
                              &(pMsgContext->resData[1U + DCM_DID_ID_SIZE]),
                              &Nrc);
    }
    else if(DCM_E_NOT_OK == ServiceProcessStatus)
    {

      if(DCM_E_POSITIVERESPONSE == Nrc)
      {
        /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ReturnControlToECU.0x00,1 */
        /* Return a Negative Response Code which is equal to generalReject (0x10). */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
      }
      else
      {
        /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ReturnControlToECU.NRC,1 */
        /* Return a Negative Response Code which is equal to the value inherited from Dcm_DefaultDIDReturnControlToECU(). */
        Dcm_ExternalSetNegResponse(pMsgContext, Nrc);
      }
        /* Processing done. */
        ServiceProcessStatus = DCM_E_DONE;
    }
    else if(DCM_E_PENDING == ServiceProcessStatus)
    {
      /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ReturnControlToECU.Pending,1 */
      /* Do nothing. The Status will remain DCM_E_PENDING. */
    }
    else
    {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      /* Report the DCM_E_INTERFACE_RETURN_VALUE DET error. */
      /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ReturnControlToECU.UnsupportedError.DET,1 */
      DCM_DET_REPORT_ERROR(DCM_SERVID_DEFAULTDIDRETURNCONTROLTOECU_DEFAULTINTERFACE, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */

      /* Return generalReject as a NegativeResponseCode. */
      /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ReturnControlToECU.UnsupportedError.NRC,1 */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
      /* Processing done. */
      ServiceProcessStatus = DCM_E_DONE;
    }
  }
  else
  {
    ServiceProcessStatus = Dcm_Dsp_IOControl_DefaultDidRead(
                             OpStatus,
                             pMsgContext,
                             ReqDID,
                             &(pMsgContext->resData[1U + DCM_DID_ID_SIZE]),
                             &Nrc);
  }

  return ServiceProcessStatus;
}

/* Sub-handler for IOControl 'ResetToDefault'. Default Interface option enabled. */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOResetToDefault_Default_Data(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  Dcm_OpStatusType OpStatus)
{
  /* Return value for the function Dcm_DefaultDIDResetToDefault(). */
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  Dcm_NegativeResponseCodeType Nrc = DCM_E_GENERALREJECT;
  /* Holds the Did from the Request Message. */
  Dcm_DIDIDType ReqDID;

  /* Build the Did from the first and second byte from the Request Message. */
  ReqDID = (uint16)DCM_COMPOSE_16BIT_WORD_FROM_MSB_AND_LSB(pMsgContext->reqData[0U], pMsgContext->reqData[1U]);

  /* The DefaultInterface for IoControl is called only when the function Dcm_DefaultDIDRead() is not returning Pending. */
  if(FALSE == Dcm_IoControlDefaultStatus.IsDefaultDidReadReturnPending)
  {
    /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.ResetToDefault,1 */
    /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ResetToDefault.Timeout.Cancellation,1 */
    /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ResetToDefault.ProtocolPreemption.Cancellation,1 */
    ServiceProcessStatus = Dcm_DefaultDIDResetToDefault(
                            OpStatus,
                            ReqDID,
                            &Nrc);

    if(E_OK == ServiceProcessStatus)
    {
      ServiceProcessStatus = Dcm_Dsp_IOControl_DefaultDidRead(
                              DCM_INITIAL,
                              pMsgContext,
                              ReqDID,
                              &(pMsgContext->resData[1U + DCM_DID_ID_SIZE]),
                              &Nrc);
    }
    else if(DCM_E_NOT_OK == ServiceProcessStatus)
    {
      if(DCM_E_POSITIVERESPONSE == Nrc)
      {
        /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ResetToDefault.0x00,1 */
        /* Return a Negative Response Code which is equal to generalReject (0x10). */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
      }
      else
      {
        /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ResetToDefault.NRC,1 */
        /* Return a Negative Response Code which is equal to the value inherited from Dcm_DefaultDIDResetToDefault() function. */
        Dcm_ExternalSetNegResponse(pMsgContext, Nrc);
      }
      /* Processing done. */
      ServiceProcessStatus = DCM_E_DONE;
    }
    else if(DCM_E_PENDING == ServiceProcessStatus)
    {
      /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ResetToDefault.Pending,1 */
      /* Do nothing. The status will remain DCM_E_PENDING. */
    }
    else
    {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      /* Report the DCM_E_INTERFACE_RETURN_VALUE DET error. */
      /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ResetToDefault.UnsupportedError.DET,1 */
      DCM_DET_REPORT_ERROR(DCM_SERVID_DEFAULTDIDRESETTODEFAULT_DEFAULTINTERFACE, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */

      /* Return generalReject as a NegativeResponseCode. */
      /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ResetToDefault.UnsupportedError.NRC,1 */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
      /* Processing done. */
      ServiceProcessStatus = DCM_E_DONE;
    }
  }
  else
  {
    ServiceProcessStatus = Dcm_Dsp_IOControl_DefaultDidRead(
                             OpStatus,
                             pMsgContext,
                             ReqDID,
                             &(pMsgContext->resData[1U + DCM_DID_ID_SIZE]),
                             &Nrc);
  }

  return ServiceProcessStatus;
}

/* Sub-handler for IOControl 'FreezeCurrentState'. Default Interface option enabled. */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOFreezeCurrentState_Default_Data(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  Dcm_OpStatusType OpStatus)
{
  /* Return value for the function Dcm_DefaultDIDFreezeCurrentState(). */
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  Dcm_NegativeResponseCodeType Nrc = DCM_E_GENERALREJECT;
  /* Holds the Did from the Request Message. */
  Dcm_DIDIDType ReqDID;

  /* Build the Did from the first and second byte from the Request Message. */
  ReqDID = (uint16)DCM_COMPOSE_16BIT_WORD_FROM_MSB_AND_LSB(pMsgContext->reqData[0U], pMsgContext->reqData[1U]);

  /* The DefaultInterface for IoControl is called only when the function Dcm_DefaultDIDRead() is not returning Pending. */
  if(FALSE == Dcm_IoControlDefaultStatus.IsDefaultDidReadReturnPending)
  {
    /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.FreezeCurrentState,1 */
    /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.FreezeCurrentState.Timeout.Cancellation,1 */
    /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.FreezeCurrentState.ProtocolPreemption.Cancellation,1 */
    ServiceProcessStatus = Dcm_DefaultDIDFreezeCurrentState(
                            OpStatus,
                            ReqDID,
                            &Nrc);

    if(E_OK == ServiceProcessStatus)
    {
      ServiceProcessStatus = Dcm_Dsp_IOControl_DefaultDidRead(
                              DCM_INITIAL,
                              pMsgContext,
                              ReqDID,
                              &(pMsgContext->resData[1U + DCM_DID_ID_SIZE]),
                              &Nrc);
    }
    else if(E_NOT_OK == ServiceProcessStatus)
    {
      if(DCM_E_POSITIVERESPONSE == Nrc)
      {
        /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.FreezeCurrentState.0x00,1 */
        /* Return a Negative Response Code which is equal to generalReject (0x10). */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
      }
      else
      {
        /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.FreezeCurrentState.NRC,1 */
        /* Return a Negative Response Code which is equal to the value inherited from Dcm_DefaultDIDFreezeCurrentState() function. */
        Dcm_ExternalSetNegResponse(pMsgContext, Nrc);
      }
      /* Processing done. */
      ServiceProcessStatus = DCM_E_DONE;
    }
    else if(DCM_E_PENDING == ServiceProcessStatus)
    {
      /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.FreezeCurrentState.Pending,1 */
      /* Do nothing. The status will remain DCM_E_PENDING. */
    }
    else
    {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      /* Report the DCM_E_INTERFACE_RETURN_VALUE DET error. */
      /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.FreezeCurrentState.UnsupportedError.DET,1 */
      DCM_DET_REPORT_ERROR(DCM_SERVID_DEFAULTDIDFREEZECURRENTSTATE_DEFAULTINTERFACE, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */

      /* Return generalReject as a NegativeResponseCode. */
      /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.FreezeCurrentState.UnsupportedError.NRC,1 */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
      /* Processing done. */
      ServiceProcessStatus = DCM_E_DONE;
    }
  }
  else
  {
    ServiceProcessStatus = Dcm_Dsp_IOControl_DefaultDidRead(
                             OpStatus,
                             pMsgContext,
                             ReqDID,
                             &(pMsgContext->resData[1U + DCM_DID_ID_SIZE]),
                             &Nrc);
  }

  return ServiceProcessStatus;
}

/* Sub-handler for IOControl 'ShortTermAdjustment'. Default Interface option enabled. */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOShortTermAdjustment_Default_Data(
  P2VAR(Dcm_MsgContextType,
  AUTOMATIC, DCM_VAR) pMsgContext,
  Dcm_OpStatusType OpStatus)
{
  /* Return value for the function Dcm_DefaultDIDShortTermAdjustment(). */
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  Dcm_NegativeResponseCodeType Nrc = DCM_E_GENERALREJECT;
  /* Holds the Did from the Request Message. */
  Dcm_DIDIDType ReqDID;

  /* Build the Did from the first and second byte from the Request Message. */
  ReqDID = (uint16)DCM_COMPOSE_16BIT_WORD_FROM_MSB_AND_LSB(pMsgContext->reqData[0U], pMsgContext->reqData[1U]);

  /* The DefaultInterface for IoControl is called only when the function Dcm_DefaultDIDRead() is not returning Pending. */
  if(FALSE == Dcm_IoControlDefaultStatus.IsDefaultDidReadReturnPending)
  {
    /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.ShortTermAdjustment,1 */
    /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ShortTermAdjustment.Timeout.Cancellation,1 */
    /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ShortTermAdjustment.ProtocolPreemption.Cancellation,1 */
    ServiceProcessStatus = Dcm_DefaultDIDShortTermAdjustment(
                            OpStatus,
                            ReqDID,
                            /* Position of the pointer = Response address + (Did Id Size + Sub-Function Size). */
                            (uint8*)(&pMsgContext->reqData[DCM_DID_ID_SIZE + 1]),
                            /* Length = Total Request Size - (Did Id Size + Sub-Function Size). */
                            (pMsgContext->reqDataLen - (DCM_DID_ID_SIZE + 1)),
                            &Nrc);

    if(E_OK == ServiceProcessStatus)
    {
      ServiceProcessStatus = Dcm_Dsp_IOControl_DefaultDidRead(
                              DCM_INITIAL,
                              pMsgContext,
                              ReqDID,
                              &(pMsgContext->resData[1U + DCM_DID_ID_SIZE]),
                              &Nrc);
    }
    else if(E_NOT_OK == ServiceProcessStatus)
    {
      if(DCM_E_POSITIVERESPONSE == Nrc)
      {
        /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ShortTermAdjustment.0x00,1 */
        /* Return a Negative Response Code which is equal to generalReject (0x10). */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
      }
      else
      {
        /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ShortTermAdjustment.NRC,1 */
        /* Return a Negative Response Code which is equal to the value inherited from Dcm_DefaultDIDShortTermAdjustment() function. */
        Dcm_ExternalSetNegResponse(pMsgContext, Nrc);
      }
    }
    else if(DCM_E_PENDING == ServiceProcessStatus)
    {
      /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ShortTermAdjustment.Pending,1 */
      /* Do nothing. The status will remain DCM_E_PENDING. */
    }
    else
    {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      /* Report the DCM_E_INTERFACE_RETURN_VALUE DET error. */
      /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ShortTermAdjustment.UnsupportedError.DET,1 */
      DCM_DET_REPORT_ERROR(DCM_SERVID_DEFAULTDIDSHORTTERMADJUSTMENT_DEFAULTINTERFACE, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* (DCM_DEV_ERROR_DETECT == STD_ON) */

      /* Return generalReject as a NegativeResponseCode. */
      /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.UnsupportedDidControl.ShortTermAdjustment.UnsupportedError.NRC,1 */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
      /* Processing done. */
      ServiceProcessStatus = DCM_E_DONE;
    }
  }
  else
  {
    ServiceProcessStatus = Dcm_Dsp_IOControl_DefaultDidRead(
                             OpStatus,
                             pMsgContext,
                             ReqDID,
                             &(pMsgContext->resData[1U + DCM_DID_ID_SIZE]),
                             &Nrc);
  }

  return ServiceProcessStatus;
}
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */

#if (DCM_DID_SIGNALS_COUNT > 0U )
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOControlByIdentifier_SvcCont_ConfiguredDid(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  /* Holds the instance id */
  uint8 instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                    DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);

  /* Request data processing */
  switch (DCM_IO_CONTROL_DID_CONTEXT(instIdx).IOControlParameter)
  {
    case DCM_RETURNCONTROLTOECU:
      /* Call 'returnControlToEcu' handler to request to application to return control to ECU */
      serviceProcessStatus = Dcm_Dsp_IOReturnControlToECU_Data(pMsgContext,
                                                              instIdx, OpStatus);
      break;

    case DCM_RESETTODEFAULT:
      /* Call 'resetToDefault' handler to request to application to reset an IOControl
         to default value */
      serviceProcessStatus = Dcm_Dsp_IOResetToDefault_Data(pMsgContext,
                                                              instIdx, OpStatus);
      break;

    case DCM_FREEZECURRENTSTATE:
      /* Call 'FreezeCurrentState' handler to request to application to freeze the
         current state of an IOControl */
      serviceProcessStatus = Dcm_Dsp_IOFreezeCurrentState_Data(pMsgContext,
                                                              instIdx, OpStatus);
      break;

    case DCM_SHORTTERMADJUSTMENT:
      /* Call 'ShortTermAdjustment' handler to request to application to adjust the IO signal */
      serviceProcessStatus = Dcm_Dsp_IOShortTermAdjustment_Data(pMsgContext,
                                                              instIdx, OpStatus);
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_INPUTOUTPUTCONTROLBYIDENTIFIER);
      break;
    /* CHECK: PARSE */
  }

  return serviceProcessStatus;
}
#endif /* #if (DCM_DID_SIGNALS_COUNT > 0U ) */

#if (DCM_RETURNCONTROLTOECU_ENABLED == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_InputOutputControlByIdentifier_ConfiguredDID_ReturnControlToECUForAllSignals(void)
{
#if (DCM_DID_SIGNALS_COUNT > 0U)
  if (Dcm_ReturnControlToEcuForAllSignals_HasTheLock == FALSE)
  {
    Dcm_Dsp_InputOutputControlByIdentifier_ReturnControlToEcuForAllSignalsTryLock();
  }

  while((Dcm_ReturnControlToEcuContext.configIdx < DCM_DSP_DIDSERVICES_NUM_DIDS) &&
        (Dcm_ReturnControlToEcuForAllSignals_HasTheLock == TRUE))
  {
    Std_ReturnType RetVal = E_OK;
    Dcm_DidInfoIdxType infoIdx = Dcm_DidConfig[Dcm_ReturnControlToEcuContext.configIdx].DidInfoIdx;
    Dcm_DidIoctlAccessInfoIdxType AccessInfoIdx = Dcm_DidInfo[infoIdx].IoctlAccessInfoIdx;

    if (AccessInfoIdx != DCM_DIDIOCTLACCESSINFOIDX_INVALID)
    {
      P2CONST(Dcm_DidIoctlAccessInfoExtType, AUTOMATIC, DCM_CONST) AccessInfoExt =
        &(Dcm_DidIoctlAccessInfo[AccessInfoIdx].AccessInfoExt);
      /* Process all the signals in the DID */
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
      uint8 DidControlMask     = AccessInfoExt->DidControlMask;
      uint8 DidControlMaskSize = AccessInfoExt->DidControlMaskSize;
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
      while( (RetVal != DCM_E_PENDING) && (Dcm_ReturnControlToEcuContext.returnToEcuSigInd <
              Dcm_DidConfig[Dcm_ReturnControlToEcuContext.configIdx].NumSignals))
     {
        if( (AccessInfoExt->FreezeCurrentStateEnable == TRUE) ||
            (AccessInfoExt->ResetToDefaultEnable == TRUE) ||
            (AccessInfoExt->ShortTermAdjustmentEnable == TRUE))
        {
          Dcm_NegativeResponseCodeType curNrc = DCM_E_POSITIVERESPONSE;

          /* Holds the configuration table of signals in the DID */
          P2CONST(Dcm_DidSignalType, AUTOMATIC, DCM_CONST) SignalCfg =
            Dcm_Dsp_DidSignals_GetDidSignalConfig(Dcm_DidConfig[Dcm_ReturnControlToEcuContext.configIdx].DidSignalsIdx);

          /* Holds the configuration of signal being processed */
          P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_CONST) dataCfg =
            &Dcm_ReturnControlToEcuContext.dataCfg;

          /* Get DcmDspData configuration */
          Dcm_Dsp_DidSignals_GetDidDataConfig(dataCfg,
            SignalCfg[Dcm_ReturnControlToEcuContext.returnToEcuSigInd].DspDataIndex);

          /* Validate the conditions to stop all the control in progress and return
           * control to ECU.*/
          if( (dataCfg->AccessInterface == DCM_USE_DATA_ASYNCH_FNC) ||
              (dataCfg->AccessInterface == DCM_USE_DATA_ASYNCH_CLIENT_SERVER))
          {
            /* An X-Path check guarantees that the call-out function is provided in
             * the configuration */
            DCM_PRECONDITION_ASSERT((dataCfg->ReturnControlToEcuAsync != NULL_PTR),
                                                                    DCM_SVCID_RETURNCONTROLTOECU);

              /* Call the user function to return control to ECU of an IOControl */
              if (Dcm_ReturnControlToEcuContext.InitSignalProcessing == TRUE)
              {
                RetVal = dataCfg->ReturnControlToEcuAsync(DCM_INITIAL, &curNrc);

                Dcm_ReturnControlToEcuContext.InitSignalProcessing = FALSE;
              }
              else
              {
                RetVal = dataCfg->ReturnControlToEcuAsync(DCM_PENDING, &curNrc);
              }

              if(RetVal == E_OK)
              {
                /* Status will be set to DCM_RET_CTRL_FINISHED at the end of the function, after
                  completion of processing all the signals for all DID's */
              }
              else if(RetVal == DCM_E_PENDING)
              {
                Dcm_ReturnControlToEcuContext.returnCtrlToECUStatus = DCM_RET_CTRL_PENDING;
#if (DCM_DSP_RETURN_CONTROLTOECU_TIMEOUT_REPORT == STD_ON)
                /* returnControlToEcu operation returns PENDING, so increment the timeoutCount */
                Dcm_ReturnControlToEcuContext.timeoutCount++;
#endif
              }
              else
              {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
              if(RetVal != E_NOT_OK)
              {
                /* Report Development Error for invalid return value */
                DCM_DET_REPORT_ERROR(DCM_SVCID_RETURNCONTROLTOECU, DCM_E_INTERFACE_RETURN_VALUE);
              }
              else
              {
                /* Report Development Error */
                DCM_DET_REPORT_ERROR(DCM_SVCID_RETURNCONTROLTOECU, DCM_E_RETURNCONTROLTOECU_FAIL);
              }
#endif
              }
          }
          else
          {
            if( (dataCfg->AccessInterface == DCM_USE_DATA_SYNCH_FNC) ||
                (dataCfg->AccessInterface == DCM_USE_DATA_SYNCH_CLIENT_SERVER))
            {
              /* An X-Path check guarantees that the call-out function is provided
               * in the configuration */
              DCM_PRECONDITION_ASSERT((dataCfg->ReturnControlToEcuSyncUnion.ReturnControlToEcuSync != NULL_PTR),
                                      DCM_SVCID_RETURNCONTROLTOECU);
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
              RetVal = Dcm_Dsp_ReturnControlToEcuWithAllOnesMask(dataCfg,
                                                                 &curNrc,
                                                                 DidControlMask,
                                                                 DidControlMaskSize);
#else /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
              RetVal = Dcm_Dsp_ReturnControlToEcuWithAllOnesMask(dataCfg,
                                                                 &curNrc);
#endif
#if (DCM_DEV_ERROR_DETECT == STD_ON)
              if(RetVal == E_NOT_OK)
              {
                /* Report Development Error */
                DCM_DET_REPORT_ERROR(DCM_SVCID_RETURNCONTROLTOECU,
                                     DCM_E_RETURNCONTROLTOECU_FAIL);
              }
              else
              {
                if(RetVal != E_OK)
                {
                  /* Report Development Error for invalid return value */
                  DCM_DET_REPORT_ERROR(DCM_SVCID_RETURNCONTROLTOECU,
                                       DCM_E_INTERFACE_RETURN_VALUE);
                }
              }
#endif
            }
          }
          /* Proceed with next signal if interface returns other than DCM_E_PENDING */
          if(RetVal != DCM_E_PENDING)
          {
            Dcm_ReturnControlToEcuContext.InitSignalProcessing = TRUE;
            Dcm_ReturnControlToEcuContext.returnToEcuSigInd++;
          }
          else
          {
            Dcm_ReturnControlToEcuContext.InitSignalProcessing = FALSE;
          }
        }
        else
        {
          Dcm_ReturnControlToEcuContext.returnToEcuSigInd++;
        }
      }
      if( Dcm_ReturnControlToEcuContext.returnToEcuSigInd ==
         Dcm_DidConfig[Dcm_ReturnControlToEcuContext.configIdx].NumSignals
        )
      {
        Dcm_ReturnControlToEcuContext.returnToEcuSigInd = 0U;
        Dcm_ReturnControlToEcuContext.configIdx++;
      }
      else
      {
        /* One of the signal returns PENDING, hence exit DID loop */
        break;
      }
    }
    else
    {
      Dcm_ReturnControlToEcuContext.returnToEcuSigInd = 0U;
      Dcm_ReturnControlToEcuContext.configIdx++;
    }
  }


  if((Dcm_ReturnControlToEcuContext.configIdx == DCM_DSP_DIDSERVICES_NUM_DIDS) &&
     (Dcm_ReturnControlToEcuForAllSignals_HasTheLock == TRUE))
  {
    /* Unlock DidServices */
    Dcm_Dsp_DidServices_UnlockService();
    Dcm_ReturnControlToEcuForAllSignals_HasTheLock = FALSE;
    /* Processing for all signals and DIS's completed. Set the finished status finally. */
    Dcm_ReturnControlToEcuContext.returnCtrlToECUStatus = DCM_RET_CTRL_FINISHED;
#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_OFF)
    /* returnControlToEcu operation completed, so reset the timeoutCount. */
    Dcm_ReturnControlToEcuContext.timeoutCount = 0U;
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_OFF) */
    Dcm_ReturnControlToEcuContext.InitSignalProcessing = TRUE;
    Dcm_ReturnControlToEcuContext.configIdx = 0U;
  }
#else
  Dcm_ReturnControlToEcuContext.returnCtrlToECUStatus = DCM_RET_CTRL_FINISHED;
#endif /*#if (DCM_DID_SIGNALS_COUNT > 0U)*/
}

#endif /* (DCM_RETURNCONTROLTOECU_ENABLED == STD_ON) */

#if (DCM_DID_SIGNALS_COUNT > 0U )
/* Sub-handler for IOControl 'ReturnControlToECU' */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOReturnControlToECU_Data(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  uint8 instIdx,
  Dcm_OpStatusType OpStatus)
{
  Std_ReturnType RetVal = DCM_E_NOT_OK;
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  /* Holds the context information of currently processed DID */
  P2VAR(Dcm_IOControlDIDContextType, AUTOMATIC, DCM_VAR) pIOControlDIDContext =
                                                    &DCM_IO_CONTROL_DID_CONTEXT(instIdx);
  /* Holds the number of signals in the DID */
  uint16 numSignals = Dcm_DidConfig[pIOControlDIDContext->DidConfigIdx].NumSignals;
  boolean controlMaskStatus = FALSE;
  Dcm_NegativeResponseCodeType nrc = E_OK;

  /* Dcm581: In case of more than one supported I/O signal per DataIdentifier, the DCM shall
     internally consider the parameter controlEnableMaskRecord and control only the
     included signals in the request message */
  while ((pIOControlDIDContext->SignalInd < numSignals) &&
         (serviceProcessStatus != DCM_E_PENDING) && (serviceProcessStatus != DCM_E_NOT_OK) &&
         (pIOControlDIDContext->RequestProcessingSt != DCM_DID_READ_PENDING))
  {
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
    uint8 DcmDspDidControlMask     = pIOControlDIDContext->DcmDspDidControlMask;
    uint8 DcmDspDidControlMaskSize = pIOControlDIDContext->DcmDspDidControlMaskSize;
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
    /* Holds the configuration of the signal being processed */
    Dcm_DidSignalsIdxType signalIdx =
      Dcm_DidConfig[pIOControlDIDContext->DidConfigIdx].DidSignalsIdx;

    P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_CONST) dataCfg = &(pIOControlDIDContext->dataCfg);

    P2CONST(Dcm_DidSignalType, AUTOMATIC, DCM_CONST) SignalCfg = Dcm_Dsp_DidSignals_GetDidSignalConfig(signalIdx + pIOControlDIDContext->SignalInd);

    /* Get DcmDspData configuration */
    Dcm_Dsp_DidSignals_GetDidDataConfig(dataCfg, SignalCfg->DspDataIndex);
    /* If DcmDspDidControlMask is set to DCM_CONTROLMASK_EXTERNAL, call ReturnControlToEcu callback
     * function all the time.
     * In case DcmDspDidControlMask is not defined, ReturnControlToEcu is called if in the bytes
     * representing the mask, the mask bit for the corresponding signal is set.
     */
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
    if (DcmDspDidControlMask == DCM_CONTROLMASK_EXTERNAL)
    {
      controlMaskStatus = TRUE;
    }
    else
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
    {
      controlMaskStatus = Dcm_Dsp_IOControl_CheckControlMask(&(pMsgContext->reqData[3U]),
                                                             pIOControlDIDContext, numSignals);
    }

    /* The interface type of the currently processed signal data */
    switch (dataCfg->AccessInterface)
    {
#if (DCM_INCLUDE_RTE == STD_ON)
      case DCM_USE_DATA_SYNCH_CLIENT_SERVER:
#endif /* DCM_INCLUDE_RTE == STD_ON */
      case DCM_USE_DATA_SYNCH_FNC:
        if(controlMaskStatus == TRUE)
        {
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
          if (DcmDspDidControlMask == DCM_CONTROLMASK_EXTERNAL)
          {
            switch (DcmDspDidControlMaskSize)
            {
              case 1U:
                RetVal = dataCfg->ReturnControlToEcuSyncUnion.ReturnControlToEcuSyncMaskExternSize1
                        (pIOControlDIDContext->DcmDspDidControlMaskUint8, &nrc);
                break;

              case 2U:
                RetVal = dataCfg->ReturnControlToEcuSyncUnion.ReturnControlToEcuSyncMaskExternSize2
                        (pIOControlDIDContext->DcmDspDidControlMaskUint16, &nrc);
                break;

              case 3U:
                RetVal = dataCfg->ReturnControlToEcuSyncUnion.ReturnControlToEcuSyncMaskExternSize3
                        (pIOControlDIDContext->DcmDspDidControlMaskUint32, &nrc);
                break;

              case 4U:
                RetVal = dataCfg->ReturnControlToEcuSyncUnion.ReturnControlToEcuSyncMaskExternSize4
                         (pIOControlDIDContext->DcmDspDidControlMaskUint32, &nrc);
                break;

              /* CHECK: NOPARSE */
              default:
                DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_RETURNCONTROLTOECU);
                break;
              /* CHECK: PARSE */
            }
          }
          else
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
          {
            /* Call the interface function 'ReturnControlToEcu' for sync operation */
            RetVal = dataCfg->ReturnControlToEcuSyncUnion.ReturnControlToEcuSync(&nrc);
          }
        }
        else
        {
          RetVal = DCM_E_OK;
        }
        break;

#if (DCM_INCLUDE_RTE == STD_ON)
      case DCM_USE_DATA_ASYNCH_CLIENT_SERVER:
#endif /* DCM_INCLUDE_RTE == STD_ON */
      case DCM_USE_DATA_ASYNCH_FNC:
        if(controlMaskStatus == TRUE)
        {
          if (pIOControlDIDContext->InitSignalProcessing == TRUE)
          {
            /* Call the interface function 'ReturnControlToEcu' for async operation */
            RetVal = dataCfg->ReturnControlToEcuAsync(DCM_INITIAL, &nrc);
            pIOControlDIDContext->InitSignalProcessing = FALSE;
          }
          else
          {
            /* Call the interface function 'ReturnControlToEcu' for async operation */
            RetVal = dataCfg->ReturnControlToEcuAsync(OpStatus, &nrc);
          }
        }
        else
        {
          RetVal = DCM_E_OK;
        }
        break;

      /* CHECK: NOPARSE */
      default:
        /* Defensive branch. Default case at the end of the switch statement */
        DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_RETURNCONTROLTOECU);
        break;
      /* CHECK: PARSE */
    }

    serviceProcessStatus = Dcm_Dsp_IOControl_InterfaceRetValCheck(pMsgContext,
                         pIOControlDIDContext, dataCfg, nrc, RetVal, DCM_SVCID_RETURNCONTROLTOECU);
  }

  if ((((DCM_E_OK == serviceProcessStatus) && (E_OK == RetVal)) && (DCM_CANCEL != OpStatus)) ||
      (pIOControlDIDContext->RequestProcessingSt == DCM_DID_READ_PENDING))
  {
    /* NRC is already set from inside the function if the signal reading fails */
    serviceProcessStatus = Dcm_Dsp_IOControl_ReadSignals(OpStatus, pMsgContext, instIdx);
  }
  else
  {
    /* Do Nothing */
  }

  return serviceProcessStatus;
}

/* Sub-handler for IOControl 'ResetToDefault' */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOResetToDefault_Data(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  uint8 instIdx,
  Dcm_OpStatusType OpStatus)
{
  Std_ReturnType RetVal = E_NOT_OK;
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  /* holds the context information of currently processed DID */
  P2VAR(Dcm_IOControlDIDContextType, AUTOMATIC, DCM_VAR) pIOControlDIDContext =
                                                    &DCM_IO_CONTROL_DID_CONTEXT(instIdx);
  /* holds the number of signals in the DID */
  uint16 numSignals = Dcm_DidConfig[pIOControlDIDContext->DidConfigIdx].NumSignals;
  boolean controlMaskStatus = FALSE;
  Dcm_NegativeResponseCodeType nrc = DCM_E_OK;

  /* Dcm581: In case of more than one supported I/O signal per DataIdentifier, the DCM shall
     internally consider the parameter controlEnableMaskRecord and control only the
     included signals in the request message */
  while ((pIOControlDIDContext->SignalInd < numSignals) &&
         (serviceProcessStatus != DCM_E_PENDING) && (serviceProcessStatus != DCM_E_NOT_OK) &&
         (pIOControlDIDContext->RequestProcessingSt != DCM_DID_READ_PENDING))
  {
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
    uint8 DcmDspDidControlMask     = pIOControlDIDContext->DcmDspDidControlMask;
    uint8 DcmDspDidControlMaskSize = pIOControlDIDContext->DcmDspDidControlMaskSize;
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
    /* holds the configuration of signal being processed */
    Dcm_DidSignalsIdxType signalIdx =
      Dcm_DidConfig[pIOControlDIDContext->DidConfigIdx].DidSignalsIdx;

    P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_CONST) dataCfg = &(pIOControlDIDContext->dataCfg);

    P2CONST(Dcm_DidSignalType, AUTOMATIC, DCM_CONST) SignalCfg = Dcm_Dsp_DidSignals_GetDidSignalConfig(signalIdx + pIOControlDIDContext->SignalInd);

    /* Get DcmDspData configuration */
    Dcm_Dsp_DidSignals_GetDidDataConfig(dataCfg, SignalCfg->DspDataIndex);

    /* If DcmDspDidControlMask is set to DCM_CONTROLMASK_EXTERNAL call ResetToDefault callback
     * function all the time
     * In case DcmDspDidControlMask is not defined, ResetToDefault is called if in the bytes
     * representing the mask, the mask bit for the corresponding signal is set.
     */
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
    if (DcmDspDidControlMask == DCM_CONTROLMASK_EXTERNAL)
    {
      controlMaskStatus = TRUE;
    }
    else
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
    {
      controlMaskStatus = Dcm_Dsp_IOControl_CheckControlMask(&(pMsgContext->reqData[3U]),
                                                          pIOControlDIDContext, numSignals);
    }
    /* As per the Data interface of the signal Data*/
    switch (dataCfg->AccessInterface)
    {
#if (DCM_INCLUDE_RTE == STD_ON)
      case DCM_USE_DATA_SYNCH_CLIENT_SERVER:
#endif /*DCM_INCLUDE_RTE == STD_ON*/
      case DCM_USE_DATA_SYNCH_FNC:
        if(controlMaskStatus == TRUE)
        {
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
          if (DcmDspDidControlMask == DCM_CONTROLMASK_EXTERNAL)
          {
            switch (DcmDspDidControlMaskSize)
            {
              case 1U:
                RetVal = dataCfg->ResetToDefaultSyncUnion.ResetToDefaultSyncMaskExternSize1
                        (pIOControlDIDContext->DcmDspDidControlMaskUint8, &nrc);
                break;

              case 2U:
                RetVal = dataCfg->ResetToDefaultSyncUnion.ResetToDefaultSyncMaskExternSize2
                        (pIOControlDIDContext->DcmDspDidControlMaskUint16, &nrc);
                break;

              case 3U:
                RetVal = dataCfg->ResetToDefaultSyncUnion.ResetToDefaultSyncMaskExternSize3
                         (pIOControlDIDContext->DcmDspDidControlMaskUint32, &nrc);
                break;

              case 4U:
                RetVal = dataCfg->ResetToDefaultSyncUnion.ResetToDefaultSyncMaskExternSize4
                         (pIOControlDIDContext->DcmDspDidControlMaskUint32, &nrc);
                break;

              /* CHECK: NOPARSE */
              default:
                DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_RESETTODEFAULT);
                break;
              /* CHECK: PARSE */
            }
          }
          else
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
          {
            /* Call the interface function 'ResetToDefault' for sync operation */
            RetVal = dataCfg->ResetToDefaultSyncUnion.ResetToDefaultSync(&nrc);
          }
        }
        else
        {
          RetVal = E_OK;
        }
        break;

#if (DCM_INCLUDE_RTE == STD_ON)
      case DCM_USE_DATA_ASYNCH_CLIENT_SERVER:
#endif /*DCM_INCLUDE_RTE == STD_ON*/
      case DCM_USE_DATA_ASYNCH_FNC:
        if(controlMaskStatus == TRUE)
        {
          if (pIOControlDIDContext->InitSignalProcessing == TRUE)
          {
            /* Call the interface function 'ResetToDefault' for async operation */
            RetVal = dataCfg->ResetToDefaultAsync(DCM_INITIAL, &nrc);
            pIOControlDIDContext->InitSignalProcessing = FALSE;
          }
          else
          {
            /* Call the interface function 'ResetToDefault' for async operation */
            RetVal = dataCfg->ResetToDefaultAsync(OpStatus, &nrc);
          }
        }
        else
        {
          RetVal = E_OK;
        }
        break;

      /* CHECK: NOPARSE */
      default:
        /* Defensive branch. Default case at the end of the switch statement */
        DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_RESETTODEFAULT);
        break;
      /* CHECK: PARSE */
    }

    serviceProcessStatus = Dcm_Dsp_IOControl_InterfaceRetValCheck(pMsgContext,
                           pIOControlDIDContext, dataCfg, nrc, RetVal, DCM_SVCID_RESETTODEFAULT);
  }

  if ((((DCM_E_OK == serviceProcessStatus) && (E_OK == RetVal)) && (DCM_CANCEL != OpStatus)) ||
      (pIOControlDIDContext->RequestProcessingSt == DCM_DID_READ_PENDING))
  {
    /* NRC is already set from inside the function if the signal reading fails */
    serviceProcessStatus = Dcm_Dsp_IOControl_ReadSignals(OpStatus, pMsgContext, instIdx);
  }
  else
  {
    /* Do Nothing */
  }

  return serviceProcessStatus;
}

/* Sub-handler for IOControl 'FreezeCurrentState' */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOFreezeCurrentState_Data(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  uint8 instIdx,
  Dcm_OpStatusType OpStatus)
{
  Std_ReturnType RetVal = E_NOT_OK;
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  /* Holds the context information of currently processed DID */
  P2VAR(Dcm_IOControlDIDContextType, AUTOMATIC, DCM_VAR) pIOControlDIDContext =
                                                    &DCM_IO_CONTROL_DID_CONTEXT(instIdx);
  /* Holds the number of signals in the DID */
  uint16 numSignals = Dcm_DidConfig[pIOControlDIDContext->DidConfigIdx].NumSignals;
  boolean controlMaskStatus = FALSE;
  Dcm_NegativeResponseCodeType nrc = E_OK;

  /* Dcm581: In case of more than one supported I/O signal per DataIdentifier, the DCM shall
     internally consider the parameter controlEnableMaskRecord and control only the
     included signals in the request message */
  while ((pIOControlDIDContext->SignalInd < numSignals) &&
         (serviceProcessStatus != DCM_E_PENDING) && (serviceProcessStatus != DCM_E_NOT_OK) &&
         (pIOControlDIDContext->RequestProcessingSt != DCM_DID_READ_PENDING))
  {
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
    uint8 DcmDspDidControlMask     = pIOControlDIDContext->DcmDspDidControlMask;
    uint8 DcmDspDidControlMaskSize = pIOControlDIDContext->DcmDspDidControlMaskSize;
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
    /* holds the configuration of signal being processed */
    Dcm_DidSignalsIdxType signalIdx =
      Dcm_DidConfig[pIOControlDIDContext->DidConfigIdx].DidSignalsIdx;

    P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_CONST) dataCfg = &(pIOControlDIDContext->dataCfg);

    P2CONST(Dcm_DidSignalType, AUTOMATIC, DCM_CONST) SignalCfg = Dcm_Dsp_DidSignals_GetDidSignalConfig(signalIdx + pIOControlDIDContext->SignalInd);

    /* Get DcmDspData configuration */
    Dcm_Dsp_DidSignals_GetDidDataConfig(dataCfg, SignalCfg->DspDataIndex);
    /* If DcmDspDidControlMask is set to DCM_CONTROLMASK_EXTERNAL call FreezeCurrentState callback
     * function all the time.
     * In case DcmDspDidControlMask is not defined, ReturnControlToEcu is called if in the bytes
     * representing the mask, the mask bit for the corresponding signal is set.
     */
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
    if (DcmDspDidControlMask == DCM_CONTROLMASK_EXTERNAL)
    {
      controlMaskStatus = TRUE;
    }
    else
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
    {
      controlMaskStatus = Dcm_Dsp_IOControl_CheckControlMask(&(pMsgContext->reqData[3U]),
                                                              pIOControlDIDContext, numSignals);
    }
    /* As per the Data interface of the signal Data*/
    switch (dataCfg->AccessInterface)
    {
#if (DCM_INCLUDE_RTE == STD_ON)
      case DCM_USE_DATA_SYNCH_CLIENT_SERVER:
#endif /*DCM_INCLUDE_RTE == STD_ON*/
      case DCM_USE_DATA_SYNCH_FNC:
        if(controlMaskStatus == TRUE)
        {

#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
          if (DcmDspDidControlMask == DCM_CONTROLMASK_EXTERNAL)
          {
            switch(DcmDspDidControlMaskSize)
            {
              case 1U:
                RetVal = dataCfg->FreezeCurrentStateSyncUnion.FreezeCurrentStateSyncMaskExternSize1
                         (pIOControlDIDContext->DcmDspDidControlMaskUint8, &nrc);
                break;

              case 2U:
                RetVal = dataCfg->FreezeCurrentStateSyncUnion.FreezeCurrentStateSyncMaskExternSize2
                         (pIOControlDIDContext->DcmDspDidControlMaskUint16, &nrc);
                break;

              case 3U:
                RetVal = dataCfg->FreezeCurrentStateSyncUnion.FreezeCurrentStateSyncMaskExternSize3
                         (pIOControlDIDContext->DcmDspDidControlMaskUint32, &nrc);
                break;

              case 4U:
                RetVal = dataCfg->FreezeCurrentStateSyncUnion.FreezeCurrentStateSyncMaskExternSize4
                         (pIOControlDIDContext->DcmDspDidControlMaskUint32, &nrc);
                break;

              /* CHECK: NOPARSE */
              default:
                DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_FREEZECURRENTSTATE);
                break;
              /* CHECK: PARSE */
            }
          }
          else
 #endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
          {
            /* Call the interface function 'FreezeCurrentState' for sync operation */
            RetVal = dataCfg->FreezeCurrentStateSyncUnion.FreezeCurrentStateSync(&nrc);
          }
        }
        else
        {
          RetVal = E_OK;
        }
        break;

#if (DCM_INCLUDE_RTE == STD_ON)
      case DCM_USE_DATA_ASYNCH_CLIENT_SERVER:
#endif /*DCM_INCLUDE_RTE == STD_ON*/
      case DCM_USE_DATA_ASYNCH_FNC:
        if(controlMaskStatus == TRUE)
        {
          if (pIOControlDIDContext->InitSignalProcessing == TRUE)
          {
            /* Call the interface function 'FreezeCurrentState' for async operation */
            RetVal = dataCfg->FreezeCurrentStateAsync(DCM_INITIAL, &nrc);
            pIOControlDIDContext->InitSignalProcessing = FALSE;
          }
          else
          {
            /* Call the interface function 'FreezeCurrentState' for async operation */
            RetVal = dataCfg->FreezeCurrentStateAsync(OpStatus, &nrc);
          }
        }
        else
        {
          RetVal = E_OK;
        }
        break;

      /* CHECK: NOPARSE */
      default:
        /* Defensive branch. Default case at the end of the switch statement */
        DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_FREEZECURRENTSTATE);
        break;
      /* CHECK: PARSE */
    }

    serviceProcessStatus = Dcm_Dsp_IOControl_InterfaceRetValCheck(pMsgContext,
                        pIOControlDIDContext, dataCfg, nrc, RetVal, DCM_SVCID_FREEZECURRENTSTATE);
  }

  if ((((DCM_E_OK == serviceProcessStatus) && (E_OK == RetVal)) && (DCM_CANCEL != OpStatus)) ||
      (pIOControlDIDContext->RequestProcessingSt == DCM_DID_READ_PENDING))
  {
    /* NRC is already set from inside the function if the signal reading fails */
    serviceProcessStatus = Dcm_Dsp_IOControl_ReadSignals(OpStatus, pMsgContext, instIdx);
  }
  else
  {
    /* Do Nothing */
  }

  return serviceProcessStatus;
}

#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_HandleShortTermAdjustmentSynchMaskExternal
(
  P2VAR(Dcm_IOControlDIDContextType, AUTOMATIC, DCM_VAR)  pIOControlDIDContext,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) nrc,
  uint16  signalLength,
  uint8   DcmDspDidControlMaskSize
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_CONST) dataCfg =
        &(pIOControlDIDContext->dataCfg);

  P2VAR(Dcm_SignalBufferContextType, AUTOMATIC, DCM_VAR) pDidSignalBufferContext = Dcm_Dsp_DidServices_GetDidSignalBuffer();

  if (dataCfg->FixedLength == TRUE)
  {
    switch (DcmDspDidControlMaskSize)
    {
      case 1U:
        RetVal = dataCfg->ShortTermAdjustmentFixedSyncUnion.ShortTermAdjustmentFixedSyncMaskExternalSize1
                 (pDidSignalBufferContext->SignalBuffer->BufferUint8,
                  pIOControlDIDContext->DcmDspDidControlMaskUint8,
                  nrc);
              break;

      case 2U:
        RetVal = dataCfg->ShortTermAdjustmentFixedSyncUnion.ShortTermAdjustmentFixedSyncMaskExternalSize2
                 (pDidSignalBufferContext->SignalBuffer->BufferUint8,
                  pIOControlDIDContext->DcmDspDidControlMaskUint16,
                  nrc);
        break;

      case 3U:
        RetVal = dataCfg->ShortTermAdjustmentFixedSyncUnion.ShortTermAdjustmentFixedSyncMaskExternalSize4
                 (pDidSignalBufferContext->SignalBuffer->BufferUint8,
                  pIOControlDIDContext->DcmDspDidControlMaskUint32,
                  nrc);
        break;

      case 4U:
        RetVal = dataCfg->ShortTermAdjustmentFixedSyncUnion.ShortTermAdjustmentFixedSyncMaskExternalSize4
                 (pDidSignalBufferContext->SignalBuffer->BufferUint8,
                  pIOControlDIDContext->DcmDspDidControlMaskUint32,
                  nrc);
        break;

      /* CHECK: NOPARSE */
      default:
        DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_SHORTTERMADJUSTMENT);
        break;
      /* CHECK: PARSE */
    }
  }
  else
  {
    switch (DcmDspDidControlMaskSize)
    {
      case 1U:
        RetVal = dataCfg->ShortTermAdjustmentVariableSyncUnion.ShortTermAdjustmentVariableTypeSyncMaskExternSize1
                 (pDidSignalBufferContext->SignalBuffer->BufferUint8,
                  signalLength,
                  pIOControlDIDContext->DcmDspDidControlMaskUint8,
                  nrc);
        break;

      case 2U:
        RetVal = dataCfg->ShortTermAdjustmentVariableSyncUnion.ShortTermAdjustmentVariableTypeSyncMaskExternSize2
                 (pDidSignalBufferContext->SignalBuffer->BufferUint8,
                  signalLength,
                  pIOControlDIDContext->DcmDspDidControlMaskUint16,
                  nrc);
        break;

      case 3U:
        RetVal = dataCfg->ShortTermAdjustmentVariableSyncUnion.ShortTermAdjustmentVariableTypeSyncMaskExternSize3
                 (pDidSignalBufferContext->SignalBuffer->BufferUint8,
                  signalLength,
                  pIOControlDIDContext->DcmDspDidControlMaskUint32,
                  nrc);
        break;

      case 4U:
        RetVal = dataCfg->ShortTermAdjustmentVariableSyncUnion.ShortTermAdjustmentVariableTypeSyncMaskExternSize4
                 (pDidSignalBufferContext->SignalBuffer->BufferUint8,
                  signalLength,
                  pIOControlDIDContext->DcmDspDidControlMaskUint32,
                  nrc);
        break;
      /* CHECK: NOPARSE */
      default:
        DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_SHORTTERMADJUSTMENT);
        break;
      /* CHECK: PARSE */
    }
  }
  return RetVal;
}
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_HandleShortTermAdjustmentSynchMaskNotEnabled
(
  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_CONST)            dataCfg,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) nrc,
  uint16  signalLength
)
{
  Std_ReturnType RetVal = E_NOT_OK;

  P2VAR(Dcm_SignalBufferContextType, AUTOMATIC, DCM_VAR) pDidSignalBufferContext = Dcm_Dsp_DidServices_GetDidSignalBuffer();

  if (dataCfg->FixedLength == TRUE)
  {
    RetVal
        = dataCfg->ShortTermAdjustmentFixedSyncUnion.ShortTermAdjustmentFixedSync(pDidSignalBufferContext->SignalBuffer->BufferUint8, nrc);
  }
  else
  {
    RetVal
        = dataCfg->ShortTermAdjustmentVariableSyncUnion.ShortTermAdjustmentVariableSync(pDidSignalBufferContext->SignalBuffer->BufferUint8,
                                                      signalLength, nrc);
  }
  return RetVal;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_HandleShortTermAdjustmentSynch
(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR)           pMsgContext,
  P2VAR(Dcm_IOControlDIDContextType, AUTOMATIC, DCM_VAR)  pIOControlDIDContext,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) nrc,
  uint16  didDataPosn,
  uint16  signalLength,
  uint8   instIdx,
  boolean controlMaskStatus
)

{
  Std_ReturnType RetVal = E_NOT_OK;
      P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_CONST) dataCfg =
        &(pIOControlDIDContext->dataCfg);

  P2VAR(Dcm_SignalBufferContextType, AUTOMATIC, DCM_VAR) pDidSignalBufferContext = Dcm_Dsp_DidServices_GetDidSignalBuffer();

#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
    uint8 DcmDspDidControlMask     = pIOControlDIDContext->DcmDspDidControlMask;
    uint8 DcmDspDidControlMaskSize = pIOControlDIDContext->DcmDspDidControlMaskSize;
#endif  /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
  if(controlMaskStatus == TRUE)
  {
    Dcm_Dsp_SignalDataCopying(pMsgContext, dataCfg, didDataPosn,
                              pDidSignalBufferContext->SignalBuffer->BufferUint8, instIdx);
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
    if (DcmDspDidControlMask == DCM_CONTROLMASK_EXTERNAL)
    {
      RetVal = Dcm_Dsp_HandleShortTermAdjustmentSynchMaskExternal(pIOControlDIDContext,
                                                                  nrc,
                                                                  signalLength,
                                                                  DcmDspDidControlMaskSize);
    }
    else
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
    {
      /* Call the interface function of 'ShortTermAdjustment' for sync operation */

      RetVal = Dcm_Dsp_HandleShortTermAdjustmentSynchMaskNotEnabled(dataCfg,
                                                                    nrc,
                                                                    signalLength);
    }
  }
  else
  {
    RetVal = E_OK;
  }
  return RetVal;
}

/* Sub-handler for IOControl 'ShortTermAdjustment' */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOShortTermAdjustment_Data(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  uint8 instIdx,
  Dcm_OpStatusType OpStatus)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  Std_ReturnType RetVal = E_NOT_OK;
  uint16 DidDataLength;
  boolean controlMaskStatus = FALSE;
  uint16 signalLength = 0U;
  Dcm_NegativeResponseCodeType nrc = DCM_E_POSITIVERESPONSE;

  /* Holds the context information of currently processed DID */
  P2VAR(Dcm_IOControlDIDContextType, AUTOMATIC, DCM_VAR) pIOControlDIDContext =
                                                    &DCM_IO_CONTROL_DID_CONTEXT(instIdx);
  /* Holds the number of signals in the DID */
  uint16 numSignals = Dcm_DidConfig[pIOControlDIDContext->DidConfigIdx].NumSignals;
  /* Holds the configuration table of signals in the DID */
  P2CONST(Dcm_DidSignalType, AUTOMATIC, DCM_CONST) SignalCfg =
          Dcm_Dsp_DidSignals_GetDidSignalConfig(Dcm_DidConfig[pIOControlDIDContext->DidConfigIdx].DidSignalsIdx);

  P2CONST(Dcm_DidSignalType, AUTOMATIC, DCM_CONST) LastSignalCfg =
    Dcm_Dsp_DidSignals_GetDidSignalConfig((Dcm_DidConfig[pIOControlDIDContext->DidConfigIdx].DidSignalsIdx +
    numSignals) - 1U);

  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_CONST) LastSignaldataCfg =
    &(pIOControlDIDContext->dataCfg);

  P2VAR(Dcm_SignalBufferContextType, AUTOMATIC, DCM_VAR) pDidSignalBufferContext = Dcm_Dsp_DidServices_GetDidSignalBuffer();

  /* Get DcmDspData configuration */
  Dcm_Dsp_DidSignals_GetDidDataConfig(LastSignaldataCfg, LastSignalCfg->DspDataIndex);

  DidDataLength = DCM_GET_BYTES(LastSignalCfg->DataPos + LastSignaldataCfg->DataLength);


#if ((DCM_READ_DID_BLOCK_ID_CONFIGURED == STD_ON) || (DCM_WRITE_DID_BLOCK_ID_CONFIGURED == STD_ON))
  /* Check if the signal buffer is available */
  if (pDidSignalBufferContext->NvMService == DCM_DIDNVMOPERATION_NONE)
  {
#endif /* if ((DCM_READ_DID_BLOCK_ID_CONFIGURED == STD_ON) || (DCM_WRITE_DID_BLOCK_ID_CONFIGURED == STD_ON)) */
    /* Dcm581: In case of more than one supported I/O signal per DataIdentifier, the DCM shall
       internally consider the parameter controlEnableMaskRecord and control only the
       included signals in the request message */
    while (   (pIOControlDIDContext->SignalInd < numSignals)
           && (serviceProcessStatus != DCM_E_PENDING) && (serviceProcessStatus != DCM_E_NOT_OK)
           && (pIOControlDIDContext->RequestProcessingSt != DCM_DID_READ_PENDING)
          )
    {
      /* bit-position of signal data in request buffer */
      uint16 didDataPosn = SignalCfg[pIOControlDIDContext->SignalInd].DataPos;
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
      uint8 DcmDspDidControlMask     = pIOControlDIDContext->DcmDspDidControlMask;
      uint8 DcmDspDidControlMaskSize = pIOControlDIDContext->DcmDspDidControlMaskSize;
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
      /* holds the configuration of signal being processed */
      P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_CONST) dataCfg =
        &(pIOControlDIDContext->dataCfg);

      /* Get DcmDspData configuration */
      Dcm_Dsp_DidSignals_GetDidDataConfig(dataCfg,
        SignalCfg[pIOControlDIDContext->SignalInd].DspDataIndex);

      TS_MemBZero(pDidSignalBufferContext->SignalBuffer->BufferUint8, DCM_DATA_MAX_SIZE_BYTE_ARRAY);
    /* If DcmDspDidControlMask is set to DCM_CONTROLMASK_EXTERNAL call ShortTermAdjustment callback
     * function all the time.
     * In case DcmDspDidControlMask is not defined, ReturnControlToEcu is called if in the bytes
     * representing the mask, the mask bit for the corresponding signal is set.
     */
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
      if (DcmDspDidControlMask == DCM_CONTROLMASK_EXTERNAL)
      {
        controlMaskStatus = TRUE;
      }
      else
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
      {
        controlMaskStatus = Dcm_Dsp_IOControl_CheckControlMask(
                 &(pMsgContext->reqData[3U + DidDataLength]), pIOControlDIDContext, numSignals);
      }

      /* If there are more than 1 signals in the DID, depending on the actual number of the
       * signals there are one or more controlEnableMaskRecord bytes present in the request
       * message */
      if (numSignals > 1U)
      {
        Dcm_MsgLenType NumSigalBytes;
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
        /* The length of the signal is the length of the request, minus the DID identifier size
         * (2 bytes),minus one byte for ControlOptionRecord and n bytes for controlEnableMaskRecord,
         * according to parameter DcmDspDidControlMaskSize, minus the starting
         * position of the signal data in the request buffer */
        if (DcmDspDidControlMask == DCM_CONTROLMASK_EXTERNAL)
        {
          NumSigalBytes = (uint16)(pMsgContext->reqDataLen - DCM_DID_ID_SIZE - 1U
                           - (uint16)DcmDspDidControlMaskSize);
        }
        else
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
        {
          /* The length of the signal is the length of the request, minus the DID identifier size
           * (2 bytes),minus one byte for ControlOptionRecord and n bytes for controlEnableMaskRecord,
           * according to the number of signals (1 bit per signal) in the DID, minus the starting
           * position of the signal data in the request buffer */
            NumSigalBytes = (uint16)(pMsgContext->reqDataLen - DCM_DID_ID_SIZE - 1U
                          - (uint16)DCM_GET_BYTES(numSignals));
        }


        signalLength = (uint16)DCM_GET_BITS(NumSigalBytes) - didDataPosn;

        /* Ignore superfluous bits */
        if (signalLength > dataCfg->DataLength)
        {
          signalLength = dataCfg->DataLength;
        }

        /* Convert to octets */
        signalLength = DCM_GET_BYTES(signalLength);
      }
      else
      {
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
        if (DcmDspDidControlMask == DCM_CONTROLMASK_EXTERNAL)
        {
          /* The length of the signal is the length of the request, minus the DID identifier size
           * (2 bytes), minus one byte of ControlOptionRecord, minus DcmDspDidControlMaskSize. */
          signalLength = (uint16)pMsgContext->reqDataLen;
          signalLength -= DCM_DID_ID_SIZE;
          signalLength -= 1U;
          signalLength -= DcmDspDidControlMaskSize;
        }
        else
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
        {
          /* The length of the signal is the length of the request, minus the DID identifier size
           * (2 bytes), minus one byte of ControlOptionRecord. */
          signalLength = (uint16)(pMsgContext->reqDataLen - (DCM_DID_ID_SIZE + 1U));
        }
      }

      /* The interface type of the currently processed signal data */
      switch (dataCfg->AccessInterface)
      {
#if (DCM_INCLUDE_RTE == STD_ON)
        case DCM_USE_DATA_SYNCH_CLIENT_SERVER:
#endif /*DCM_INCLUDE_RTE == STD_ON*/
        case DCM_USE_DATA_SYNCH_FNC:
         RetVal = Dcm_Dsp_HandleShortTermAdjustmentSynch(pMsgContext,
                                                         pIOControlDIDContext,
                                                         &nrc,
                                                         didDataPosn,
                                                         signalLength,
                                                         instIdx,
                                                         controlMaskStatus);
          break;

#if (DCM_INCLUDE_RTE == STD_ON)
        case DCM_USE_DATA_ASYNCH_CLIENT_SERVER:
#endif /*DCM_INCLUDE_RTE == STD_ON*/
        case DCM_USE_DATA_ASYNCH_FNC:
          if(controlMaskStatus == TRUE)
          {
            Dcm_Dsp_SignalDataCopying(pMsgContext, dataCfg, didDataPosn,
                                      pDidSignalBufferContext->SignalBuffer->BufferUint8, instIdx);

            /* Call the interface function of 'ShortTermAdjustment' for async operation */
            if (dataCfg->FixedLength == TRUE)
            {
              if (pIOControlDIDContext->InitSignalProcessing == TRUE)
              {
                RetVal
                  = dataCfg->ShortTermAdjustmentFixedAsync(pDidSignalBufferContext->SignalBuffer->BufferUint8,
                                                           DCM_INITIAL, &nrc);

                pIOControlDIDContext->InitSignalProcessing = FALSE;
              }
              else
              {
                RetVal
                  = dataCfg->ShortTermAdjustmentFixedAsync(pDidSignalBufferContext->SignalBuffer->BufferUint8,
                                                           OpStatus, &nrc);
              }
            }
            else
            {
              if (pIOControlDIDContext->InitSignalProcessing == TRUE)
              {
                RetVal
                  = dataCfg->ShortTermAdjustmentVariableAsync(pDidSignalBufferContext->SignalBuffer->BufferUint8,
                                                              signalLength, DCM_INITIAL, &nrc);

                pIOControlDIDContext->InitSignalProcessing = FALSE;
              }
              else
              {
                RetVal
                  = dataCfg->ShortTermAdjustmentVariableAsync(pDidSignalBufferContext->SignalBuffer->BufferUint8,
                                                              signalLength, OpStatus, &nrc);
              }
            }
          }
          else
          {
            RetVal = E_OK;
          }
          break;

        /* CHECK: NOPARSE */
        default:
          /* Defensive branch. Default case at the end of the switch statement */
          DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_SHORTTERMADJUSTMENT);
          break;
        /* CHECK: PARSE */
      }

      serviceProcessStatus = Dcm_Dsp_IOControl_InterfaceRetValCheck(pMsgContext,
                         pIOControlDIDContext, dataCfg, nrc, RetVal, DCM_SVCID_SHORTTERMADJUSTMENT);
    }

    if ((((DCM_E_OK == serviceProcessStatus) && (E_OK == RetVal)) && (DCM_CANCEL != OpStatus)) ||
        (pIOControlDIDContext->RequestProcessingSt == DCM_DID_READ_PENDING))
    {
      /* NRC is already set from inside the function if the signal reading fails */
      serviceProcessStatus = Dcm_Dsp_IOControl_ReadSignals(OpStatus, pMsgContext, instIdx);
    }
    else
    {
      /* Do Nothing */
    }
#if ((DCM_READ_DID_BLOCK_ID_CONFIGURED == STD_ON) || (DCM_WRITE_DID_BLOCK_ID_CONFIGURED == STD_ON))
  }
  else
  {
    /* Signal buffer busy with another DID operation. Set NRC */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
    serviceProcessStatus = DCM_E_NOT_OK;
  }
#endif /* #if (DCM_READ_DID_BLOCK_ID_CONFIGURED) || (DCM_WRITE_DID_BLOCK_ID_CONFIGURED)) */
  return serviceProcessStatus;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOControl_InterfaceRetValCheck(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  P2VAR(Dcm_IOControlDIDContextType, AUTOMATIC, DCM_VAR) pIOControlDIDContext,
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_CONST) dataCfg,
  uint8 nrc, Std_ReturnType InterfaceRetVal, uint8 interfaceID)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

#if (DCM_DEV_ERROR_DETECT == STD_OFF)
  TS_PARAM_UNUSED(interfaceID);
#endif
  /* If the currently processed signal was controlled successfully, increment the signal index */
  if (E_OK == InterfaceRetVal)
  {
    pIOControlDIDContext->SignalInd++;
    pIOControlDIDContext->InitSignalProcessing = TRUE;
    serviceProcessStatus = DCM_E_OK;
  }
  else if(E_NOT_OK == InterfaceRetVal)
  {
    Dcm_ExternalSetNegResponse(pMsgContext, nrc);
    serviceProcessStatus = DCM_E_NOT_OK;
  }
  else if ((DCM_E_PENDING == InterfaceRetVal) &&
           ((DCM_USE_DATA_ASYNCH_FNC == dataCfg->AccessInterface) ||
            (DCM_USE_DATA_ASYNCH_CLIENT_SERVER == dataCfg->AccessInterface)))
  {
    serviceProcessStatus = DCM_E_PENDING;
  }
  else
  {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    /* Report Development Error */
    DCM_DET_REPORT_ERROR(interfaceID, DCM_E_INTERFACE_RETURN_VALUE);
#endif
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
    serviceProcessStatus = DCM_E_NOT_OK;
  }

  return serviceProcessStatus;
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ReadDataOfSingleDid_SetupByIndex
(
  P2VAR(Dcm_ReadSignalContextType, AUTOMATIC, DCM_VAR) pReadSignalContext,
  Dcm_DidConfigIdxType DidIndex,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) OutputBuffer,
  uint32 BufferSize,
  uint32 BufferOffset,
  Dcm_PerformEndiannessConversionType EndiannessConversion
)
{
  pReadSignalContext->SignalInd = 0U;
  pReadSignalContext->CurrentDidCfgIndex = DidIndex;
  pReadSignalContext->NrcConditions = DCM_E_OK;
  pReadSignalContext->outputBuffer = OutputBuffer;
  pReadSignalContext->BufferLimit = BufferSize;
  pReadSignalContext->AmountOfBufferOccupied = BufferOffset;
  pReadSignalContext->EndiannessConversion = EndiannessConversion;
  pReadSignalContext->CurrentOpStatus = DCM_INITIAL;
  pReadSignalContext->DidOp = DCM_DID_OP_IOCONTROL;
  pReadSignalContext->NewSignal = TRUE;

}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOControl_ReadSignals(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  uint8 instIdx)
{
  /* Holds the context information of currently processed DID */
  P2VAR(Dcm_IOControlDIDContextType, AUTOMATIC, DCM_VAR) pIOControlDIDContext =
                                                    &DCM_IO_CONTROL_DID_CONTEXT(instIdx);
  CONSTP2CONST(Dcm_ProtocolConfigType, AUTOMATIC, DCM_CONST) curProtocolConfig
          = &DCM_PROTOCOL_CONFIG(instIdx);
  Dcm_NegativeResponseCodeType nrc;
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  Dcm_BufferSizeType AmountOfBufferOccupied;

#if (DCM_NUM_PROTOCOL == 1U)
  /* Holds the instance id */
  TS_PARAM_UNUSED(instIdx);
#endif

  if (DCM_DID_REQUEST_PROCESSING == pIOControlDIDContext->RequestProcessingSt)
  {
    Dcm_Dsp_ReadDataOfSingleDid_SetupByIndex(&Dcm_IoctrlReadSignalContext,
                                             pIOControlDIDContext->DidConfigIdx,
                                             pMsgContext->resData,
                                             pMsgContext->resMaxDataLen,
                                             pMsgContext->resDataLen,
                                             curProtocolConfig->EndiannessConversion);
  }

  if (DCM_CANCEL == OpStatus)
  {
    Dcm_IoctrlReadSignalContext.CurrentOpStatus = DCM_CANCEL;
  }

  serviceProcessStatus =
    Dcm_Dsp_DidServices_ReadDataOfSingleDid_Execute(&Dcm_IoctrlReadSignalContext);

  if (DCM_CANCEL != OpStatus)
  {
    if (DCM_E_PENDING != serviceProcessStatus)
    {
      serviceProcessStatus = Dcm_Dsp_DidServices_ReadSignal_GetResults(
        &Dcm_IoctrlReadSignalContext, &nrc, &AmountOfBufferOccupied);

      if (DCM_E_DONE == serviceProcessStatus)
      {
         /* Set the 'RequestProcessingSt' to INVALID indicating that signal reading is over */
        pMsgContext->resDataLen = (Dcm_MsgLenType)AmountOfBufferOccupied;
        pIOControlDIDContext->RequestProcessingSt = DCM_DID_REQUEST_INVALID;
      }
      else
      {
        /* at this point, status can only be DCM_E_NOT_OK */
        DCM_PRECONDITION_ASSERT((DCM_E_NOT_OK == serviceProcessStatus), DCM_SVCID_JUMPTOBL);

        Dcm_ExternalSetNegResponse(pMsgContext, Dcm_IoctrlReadSignalContext.NrcConditions);
        serviceProcessStatus = DCM_E_DONE;
      }
    }
    else
    {
      pIOControlDIDContext->RequestProcessingSt = DCM_DID_READ_PENDING;
    }
  }
  else
  {
    pIOControlDIDContext->RequestProcessingSt = DCM_DID_REQUEST_INVALID;
    serviceProcessStatus = DCM_E_DONE;
  }

  return serviceProcessStatus;
}

STATIC FUNC(boolean, DCM_CODE) Dcm_Dsp_IOControl_CheckControlMask(
  P2CONST(uint8, AUTOMATIC, DCM_VAR) ReqData,
  P2CONST(Dcm_IOControlDIDContextType, AUTOMATIC, DCM_VAR) IOControlDIDContextPtr,
  uint16 NumSignals)
{
  /* Indicates if the current signal is marked for control or not */
  boolean ControlMaskStatus = FALSE;

  if (NumSignals == 1U)
  {
    ControlMaskStatus = TRUE;
  }
  else
  {
    /* Index for iterating DidSignals inside the Dcm_DidSignals[] configuration */
    uint16_least SignalIndex;
    /* Marks the signal position in the ControlMask byte starting from the most significant bit
     * of the byte with the lowest index */
    uint8 SignalPosInControlMask = 0U;

    /* Index of the first signal of the requested Did inside the Dcm_DidSignals[] configuration */
    const uint16 FirstSignalIndex = Dcm_DidConfig[IOControlDIDContextPtr->DidConfigIdx].DidSignalsIdx;
    /* Index of the last signal of the requested Did inside the Dcm_DidSignals[] configuration */
    const uint16 LastSignalIndex = Dcm_DidConfig[IOControlDIDContextPtr->DidConfigIdx].DidSignalsIdx + NumSignals;
    /* Indicates the byte where the start position of the requested signal is situated */
    const uint16 RequestedSignalBytePos = Dcm_Dsp_DidSignals_GetDidSignalConfig(FirstSignalIndex+IOControlDIDContextPtr->SignalInd)->DataPos / 8U;
    /* Indicates the bit where the start position of the requested signal is situated */
    const uint16 RequestedSignalBitPos = Dcm_Dsp_DidSignals_GetDidSignalConfig(FirstSignalIndex+IOControlDIDContextPtr->SignalInd)->DataPos % 8U;

    /* !LINKSTO Dcm680.ByteCounting,1 */
    /* !LINKSTO Dcm680.BitCounting,1 */
    /* !LINKSTO Dcm680.ControlMaskBitsEvaluation,1 */
    /* Go through all signals belonging to the requested Did */
    for (SignalIndex=FirstSignalIndex; SignalIndex<LastSignalIndex; SignalIndex++)
    {
      uint16 CurrentSignalBytePos = Dcm_Dsp_DidSignals_GetDidSignalConfig((uint16)SignalIndex)->DataPos / 8U;
      uint16 CurrentSignalBitPos = Dcm_Dsp_DidSignals_GetDidSignalConfig((uint16)SignalIndex)->DataPos % 8U;

      if ((CurrentSignalBytePos < RequestedSignalBytePos) ||
          ((CurrentSignalBytePos == RequestedSignalBytePos) &&
           (CurrentSignalBitPos > RequestedSignalBitPos)))
      {
        /* If there is another signal for which the start position is in a byte with a lower index than the index
         * of the requested signal
         * OR
         * If there is another signal which starts in the same byte as the requested signal,
         * and if the start bit of the requested signal is less significant
         * Then advance the requested signal's position in the control mask */
        SignalPosInControlMask++;
      }
    }

    /* Check the signal's corresponding bit in the controlEnableMaskRecord bytes
     * (there might be more than 8 signals in the Did in which case controlEnableMaskRecord
     * consists in more than one byte) */
    if (((ReqData[SignalPosInControlMask/8U]) & (DCM_CONTROL_MASK_MSB >> (SignalPosInControlMask%8U))) != 0U)
    {
      /* Control mask for the corresponding signal is set */
      ControlMaskStatus = TRUE;
    }
  }

  return ControlMaskStatus;
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_SignalDataCopying(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  P2CONST(Dcm_DidDataType, AUTOMATIC, DCM_CONST) dataCfg,
  uint16 didDataPosition,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) signalBuffer,
  uint8 instIdx)
{
#if((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON))
  /* Get configuration for current HSM instance */
  CONSTP2CONST(Dcm_ProtocolConfigType, AUTOMATIC, DCM_CONST) curProtocolConfig =
        &DCM_PROTOCOL_CONFIG(instIdx);
#endif /* #if((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON)) */

  /* Holds the context information of currently processed DID */
  P2VAR(Dcm_IOControlDIDContextType, AUTOMATIC, DCM_VAR) pIOControlDIDContext =
                                                          &DCM_IO_CONTROL_DID_CONTEXT(instIdx);
  /* Holds the number of signals in the DID */
  uint16 numSignals = Dcm_DidConfig[pIOControlDIDContext->DidConfigIdx].NumSignals;

  /* Holds the length of the signal */
  Dcm_MsgLenType signalLength = 0U;

#if ((DCM_NUM_PROTOCOL == 1U) || (DCM_ENDIANNESS_CONVERSION == STD_OFF))
  TS_PARAM_UNUSED(instIdx);
#endif

  /* If signal data length is fixed */
  if (dataCfg->FixedLength == TRUE)
  {
    /* Copy the data of currently processed signal to signalBuffer */
    Dcm_DspInternal_CopySignalData(&pMsgContext->reqData[DCM_DID_ID_SIZE + 1U], signalBuffer,
                                   didDataPosition, 0U, dataCfg->DataLength,
                                   DCM_COPY_TO_OUTBUFFER_OPAQUE,
                                   DCM_BYTE_DIRECTION_INCREMENT);
  }
  else
  {
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
      uint8 DcmDspDidControlMask     = pIOControlDIDContext->DcmDspDidControlMask;
      uint8 DcmDspDidControlMaskSize = pIOControlDIDContext->DcmDspDidControlMaskSize;
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
    /* If there are more than 1 signals in the DID, depending on the actual number of the
     * signals there are one or more controlEnableMaskRecord bytes present in the request
     * message */
    if (numSignals > 1U)
    {

      Dcm_MsgLenType NumSigalBytes;
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
      if (DcmDspDidControlMask == DCM_CONTROLMASK_EXTERNAL)
      {
        /* The length of the signal is the length of the request, minus the DID identifier size
         * (2 bytes),minus one byte for ControlOptionRecord and n bytes for DcmDspDidControlMaskSize,
         * minus the starting position of the signal data in the request buffer
         */
        NumSigalBytes = (uint16)(pMsgContext->reqDataLen - DCM_DID_ID_SIZE - 1U
                         - (uint16)DcmDspDidControlMaskSize);
      }
      else
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
      {
        /* The length of the signal is the length of the request, minus the DID identifier size
         * (2 bytes),minus one byte for ControlOptionRecord and n bytes for controlEnableMaskRecord,
         * according to the number of signals (1 bit per signal) in the DID, minus the starting
         * position of the signal data in the request buffer */
        NumSigalBytes = (uint16)(pMsgContext->reqDataLen - DCM_DID_ID_SIZE - 1U
                         - (uint16)DCM_GET_BYTES(numSignals));
      }
      signalLength = DCM_GET_BITS(NumSigalBytes) - didDataPosition;

      /* Ignore superfluous bits */
      if (signalLength > dataCfg->DataLength)
      {
        signalLength = dataCfg->DataLength;
      }
    }
    else
    {
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
      if (DcmDspDidControlMask == DCM_CONTROLMASK_EXTERNAL)
      {
        /* The length of the signal is the length of the request, minus the DID identifier size
         * (2 bytes), minus one byte of ControlOptionRecord, minus DcmDspDidControlMaskSize */
        signalLength = pMsgContext->reqDataLen;
        signalLength -= DCM_DID_ID_SIZE;
        signalLength -= 1U;
        signalLength -= DcmDspDidControlMaskSize;
      }
      else
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
      {
        /* The length of the signal is the length of the request, minus the DID identifier size
         * (2 bytes), minus one byte of ControlOptionRecord. */
        signalLength = (uint16)(pMsgContext->reqDataLen - (DCM_DID_ID_SIZE + 1U));
      }
      signalLength = DCM_GET_BITS(signalLength);
    }

    /* Copy all the data in the request, after DID, to signalBuffer */
    Dcm_DspInternal_CopySignalData(&pMsgContext->reqData[DCM_DID_ID_SIZE + 1U], signalBuffer,
                                   didDataPosition, 0U, signalLength,
                                   DCM_COPY_TO_OUTBUFFER_OPAQUE,
                                   DCM_BYTE_DIRECTION_INCREMENT);
  }
#if((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON))
  if(((dataCfg->DataType == DCM_TYPE_SINT16) || (dataCfg->DataType == DCM_TYPE_UINT16) ||
      (dataCfg->DataType == DCM_TYPE_SINT32) || (dataCfg->DataType == DCM_TYPE_UINT32)) &&
      (curProtocolConfig->EndiannessConversion == TRUE)
    )
  {
    Dcm_Dsp_DidSignals_Endianness_Conversion(signalBuffer, (uint16)DCM_GET_BYTES(dataCfg->DataLength), dataCfg->DataType);
  }
#endif /* #if((DCM_ENDIANNESS_CONVERSION == STD_ON) && (DCM_DID_ENDIANNESS_CONVERSION == STD_ON)) */

}

/* The function for checking the request data length of IOControl request */
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOControlDidRequestLengthChk(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  uint8 DidControlMask,
  uint8 DidControlMaskSize)
#else /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_IOControlDidRequestLengthChk(
    P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
{
  Std_ReturnType RetVal = DCM_E_NOT_OK;
#if (DCM_NUM_PROTOCOL > 1U)
  /* Holds the instance id */
  uint8 instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                    DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);
#endif
  /* Holds the context information of currently processed DID */
  P2VAR(Dcm_IOControlDIDContextType, AUTOMATIC, DCM_VAR) pIOControlDIDContext =
                                                    &DCM_IO_CONTROL_DID_CONTEXT(instIdx);
  /* Index of DID currently being processed */
  Dcm_DidConfigIdxType didIndex = DCM_IO_CONTROL_DID_CONTEXT(instIdx).DidConfigIdx;
  uint16 numSignals = Dcm_DidConfig[didIndex].NumSignals;
  P2CONST(Dcm_DidSignalType, AUTOMATIC, DCM_CONST) SignalCfg =
    Dcm_Dsp_DidSignals_GetDidSignalConfig((Dcm_DidConfig[didIndex].DidSignalsIdx + numSignals) - 1U);

  /* holds the configuration of signal being processed */
  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_CONST) dataCfg =
    &(pIOControlDIDContext->dataCfg);

  /* Get DcmDspData configuration */
  Dcm_Dsp_DidSignals_GetDidDataConfig(dataCfg, SignalCfg->DspDataIndex);


#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
  if (DidControlMask == DCM_CONTROLMASK_EXTERNAL)
  {
    if(pMsgContext->reqData[2U] == DCM_SHORTTERMADJUSTMENT)
    {
      /* The minimum length and maximum length is computed for the last signal only */
      Dcm_MsgLenType MaxLength = DCM_GET_BYTES(SignalCfg->DataPos + dataCfg->DataLength);
      Dcm_MsgLenType MinLength = DCM_GET_BYTES(SignalCfg->DataPos);

      MaxLength += 1U;
      MaxLength += DCM_DID_ID_SIZE;
      MaxLength += DidControlMaskSize;

      MinLength += 1U;
      MinLength += DCM_DID_ID_SIZE ;
      MinLength += DidControlMaskSize;
      if (dataCfg->FixedLength == TRUE)
      {
        if (pMsgContext->reqDataLen == MaxLength)
        {
          RetVal = DCM_E_OK;
        }
      }
      else
      {
        if ((pMsgContext->reqDataLen >= MinLength) &&
            (pMsgContext->reqDataLen <= MaxLength))
        {
          RetVal = DCM_E_OK;
        }
      }

    }
    else
    {
      Dcm_MsgLenType DataLength = 1U;
      DataLength += DCM_DID_ID_SIZE;
      DataLength += DidControlMaskSize;
      if (pMsgContext->reqDataLen == DataLength)
      {
        RetVal = DCM_E_OK;
      }
    }
  }
  else
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
  /* The controlEnableMaskRecord is only present,
   * if the DataIdentifer supports more than one signal.
  */
  {
    /*Holds the control mask length.*/
    uint16 ControlMaskLength = 0U;

    /*Holds the DID signal data length */
    uint32 DidSignalsDataLength = 0U;
    if (numSignals > 1U)
    {
      ControlMaskLength = DCM_GET_BYTES(numSignals);
    }

    /* Incase of ShortTermAdjustment IOControl parameter, the total number of
     * data bytes in request can be calculated by adding the position and the
     * size of the last signal configured for the DID. If IOControl parameter is
     * other than ShortTermAdjustment, then no data byte is present in the request.
     */
    if(pMsgContext->reqData[2U] == DCM_SHORTTERMADJUSTMENT)
    {
      DidSignalsDataLength = DCM_GET_BYTES((uint32)(SignalCfg->DataPos) + (uint32)(dataCfg->DataLength));
    }

    /* !LINKSTO Dcm.EB.IOControl.ShortTermAdjustment.OnlyFixedLengthSignals.LengthCheck,1 */
    /* !LINKSTO Dcm.EB.IOControl.ReturnControlToECU.LengthCheck,1 */
    /* !LINKSTO Dcm.EB.IOControl.ResetToDefault.LengthCheck,1 */
    /* !LINKSTO Dcm.EB.IOControl.FreezeCurrentState.LengthCheck,1 */
    /* if data length for the signal is fixed, total length configured for the DID shall be
     * equal to the (length of request data - (size of DID[2 bytes] + 1 + ControlMaskLength)).
     */
    if (dataCfg->FixedLength == TRUE)
    {
      if (DidSignalsDataLength ==
                               (pMsgContext->reqDataLen - (DCM_DID_ID_SIZE + 1U + ((uint32) ControlMaskLength))))
      {
        RetVal = DCM_E_OK;
      }
    }
    /* If the data length for the signal is dynamic, total length of the DID shall be greater than or equal to
     * the minimum length (number of bytes required based on dataposition of the signal) and shall be less
     * than or equal to the maximum length (length of request data - (size of DID[2 bytes] + 1 + ControlMaskLength)).
     */
    else
    {
      uint32 MinDidSignalDataLength = DCM_GET_BYTES((uint32)(SignalCfg->DataPos));
      uint32 DidSigDataLenInReqMsg =
                               (pMsgContext->reqDataLen - (DCM_DID_ID_SIZE + 1U + ((uint32) ControlMaskLength)));
      /* !LINKSTO Dcm.EB.IOControl.ShortTermAdjustment.VariableSignal.MaximalLengthCheck,1 */
      /* !LINKSTO Dcm.EB.IOControl.ShortTermAdjustment.VariableSignal.MinimalLengthCheck,1 */
      if ((DidSignalsDataLength >= DidSigDataLenInReqMsg) &&
          (MinDidSignalDataLength <= DidSigDataLenInReqMsg))
      {
        RetVal = DCM_E_OK;
      }
    }
  }
  return RetVal;
}
#endif /*#if (DCM_DID_SIGNALS_COUNT > 0U)*/

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_InputOutputControlByIdentifier_ServiceExecution(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  switch (OpStatus)
  {
    /* First call of service function */
    case DCM_INITIAL:
    {
      serviceProcessStatus = Dcm_Dsp_IOControlByIdentifier_SvcStart(pMsgContext);
    }
    break;

#if ((DCM_DID_SIGNALS_COUNT > 0U) || (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON))
    case DCM_PENDING:
    {
      /* If IOControlByIdentifier service has the lock for DidServices unit then continue the
       * processing otherwise try to lock the unit. */
      serviceProcessStatus = Dcm_Dsp_IOControlByIdentifier_SvcCont(OpStatus, pMsgContext);
    }
    break;

    case DCM_CANCEL:
    {
        serviceProcessStatus = Dcm_Dsp_IOControlByIdentifier_SvcCont(OpStatus, pMsgContext);
    }
    break;
#endif /* #if ((DCM_DID_SIGNALS_COUNT > 0U) || (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)) */

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      serviceProcessStatus = DCM_E_DONE;
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_INPUTOUTPUTCONTROLBYIDENTIFIER);
      break;
    /* CHECK: PARSE */
  }
#if(DCM_DSP_DIDSERVICES_LOCK_MECHANISM_ENABLED == STD_ON)
  if(  (serviceProcessStatus == DCM_E_OK)
     ||(serviceProcessStatus == DCM_E_NOT_OK)
     ||(serviceProcessStatus == DCM_E_ABORT)
     ||(serviceProcessStatus == DCM_E_DONE)
     || (OpStatus == DCM_CANCEL))
  {
    /* Unlock DidServices */
    Dcm_Dsp_DidServices_UnlockService();
    Dcm_InputOutputControlByIdentifier_LockStatus = DCM_TRYING_LOCK;
  }
#endif

  return serviceProcessStatus;
}

/*==================[external function definitions]==============================================*/

FUNC(void, DCM_CODE) Dcm_Dsp_InputOutputControlByIdentifier_Init(void)
{
  DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_INIT_ENTRY();

  Dcm_ReturnControlToEcuContext.returnToEcuSigInd = 0U;
  Dcm_ReturnControlToEcuContext.configIdx = 0U;
  Dcm_ReturnControlToEcuContext.timeoutCount = 0U;
  Dcm_ReturnControlToEcuContext.InitSignalProcessing = TRUE;
#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
  Dcm_IoControlDefaultStatus.IsDefaultDidReadReturnPending = FALSE;
  Dcm_IoControlDefaultStatus.IsDidUnconfigured = FALSE;
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */

  Dcm_InputOutputControlByIdentifier_LockStatus = DCM_TRYING_LOCK;
#if (DCM_RETURNCONTROLTOECU_ENABLED == STD_ON)
  Dcm_ReturnControlToEcuForAllSignals_HasTheLock = FALSE;
#endif
  DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_INIT_EXIT();
}

FUNC(uint16,DCM_CODE) Dcm_Dsp_InputOutputControlByIdentifier_GetReturnControlToEcuTimeoutCount(void)
{
  uint16 TimeoutCount;

  DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_GETRETURNCONTROLTOECUTIMEOUTCOUNT_ENTRY();

  TimeoutCount = Dcm_ReturnControlToEcuContext.timeoutCount;

  DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_GETRETURNCONTROLTOECUTIMEOUTCOUNT_EXIT(TimeoutCount);

  return TimeoutCount;
}

FUNC(Dcm_DidReturnControlToEcuStatusType, DCM_CODE)
 Dcm_Dsp_InputOutputControlByIdentifier_GetReturnControlToEcuStatus(void)
{
  Dcm_DidReturnControlToEcuStatusType RetCtrlToECUStatus;

  DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_GETRETURNCONTROLTOECUSTATUS_ENTRY();

  RetCtrlToECUStatus = Dcm_ReturnControlToEcuContext.returnCtrlToECUStatus;

  DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_GETRETURNCONTROLTOECUSTATUS_EXIT(RetCtrlToECUStatus);

  return  RetCtrlToECUStatus;
}

FUNC(void, DCM_CODE) Dcm_Dsp_InputOutputControlByIdentifier_SetReturnControlToEcuStatus(
  Dcm_DidReturnControlToEcuStatusType ReturnCtrlToECUStatus
  )
{
  DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SETRETURNCONTROLTOECUSTATUS_ENTRY(
    ReturnCtrlToECUStatus);

  Dcm_ReturnControlToEcuContext.returnCtrlToECUStatus = ReturnCtrlToECUStatus;

  DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SETRETURNCONTROLTOECUSTATUS_EXIT();
}

#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
FUNC(void, DCM_CODE) Dcm_Dsp_InputOutputControlByIdentifier_SetReturnControlToEcuDefaultStatus(
  uint8 ReturnCtrlToECUStatus
  )
{
  DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SETRETURNCONTROLTOECUSTATUS_DEFAULTINTERFACE_ENTRY(
  ReturnCtrlToECUStatus);

  Dcm_IoControlDefaultStatus.ReturnControlToECUForAllSignalsStatus = ReturnCtrlToECUStatus;

  DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SETRETURNCONTROLTOECUSTATUS_DEFAULTINTERFACE_EXIT();
}

FUNC(Dcm_DidReturnControlToEcuStatusDefaultType, DCM_CODE) Dcm_Dsp_InputOutputControlByIdentifier_GetReturnControlToEcuDefaultStatus(void)
{
  Dcm_DidReturnControlToEcuStatusDefaultType RetCtrlToECUStatus;

  DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_GETRETURNCONTROLTOECUSTATUS_DEFAULTINTERFACE_ENTRY();

  RetCtrlToECUStatus = Dcm_IoControlDefaultStatus.ReturnControlToECUForAllSignalsStatus;

  DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_GETRETURNCONTROLTOECUSTATUS_DEFAULTINTERFACE_EXIT(RetCtrlToECUStatus);

  return RetCtrlToECUStatus;
}
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */
/*==================[Service and Sub-Service Handler definitions]================================*/

FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_InputOutputControlByIdentifier_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext
  )
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
#if ((DCM_HSM_INST_MULTI_ENABLED == STD_ON) && (DCM_DID_SIGNALS_COUNT > 0U))
  /* Holds the instance id */
  uint8 instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                    DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);
#endif
  DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SVCH_ENTRY(OpStatus,pMsgContext);

  switch(Dcm_InputOutputControlByIdentifier_LockStatus)
  {
    case DCM_TRYING_LOCK:
    {
      if((OpStatus != DCM_CANCEL) &&
         (OpStatus != DCM_CONFIRMED_OK) &&
         (OpStatus != DCM_CONFIRMED_NOK))
      {
        Std_ReturnType LockStatus = Dcm_Dsp_DidServices_LockService();

        if (LockStatus == DCM_E_RESOURCE_LOCKED)
        {
          /* !LINKSTO Dcm.EB.DidServices.Locking.InputOutputControlByIdentifierWhileLockedByReturnControlToECU,1 */
          ServiceProcessStatus = DCM_E_PENDING;
        }
        else
        {
          DCM_POSTCONDITION_ASSERT(LockStatus == DCM_E_OK, DCM_INTERNAL_API_ID);

          Dcm_InputOutputControlByIdentifier_LockStatus = DCM_EXECUTING_SERVICE;
          ServiceProcessStatus =
                  Dcm_Dsp_InputOutputControlByIdentifier_ServiceExecution(DCM_INITIAL, pMsgContext);
        }
      }
      else
      {
#if(DCM_DID_SIGNALS_COUNT > 0U)
        /* Reset the IOControl request processing status */
        DCM_IO_CONTROL_DID_CONTEXT(instIdx).RequestProcessingSt = DCM_DID_REQUEST_INVALID;
#endif /* #if (DCM_DID_SIGNALS_COUNT > 0U) */
        ServiceProcessStatus = DCM_E_DONE;
      }
    }
    break;

    case DCM_EXECUTING_SERVICE:
    {
      ServiceProcessStatus =
                     Dcm_Dsp_InputOutputControlByIdentifier_ServiceExecution(OpStatus, pMsgContext);

    }
    break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      ServiceProcessStatus = DCM_E_DONE;
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_INPUTOUTPUTCONTROLBYIDENTIFIER);
      break;
    /* CHECK: PARSE */
  }

  DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SVCH_EXIT(serviceProcessStatus, OpStatus, pMsgContext);

  return ServiceProcessStatus;
}

#if (DCM_RETURNCONTROLTOECU_ENABLED == STD_ON)
FUNC(void, DCM_CODE) Dcm_Dsp_InputOutputControlByIdentifier_ReturnControlToEcuForAllSignalsTryLock(void)
{
  Std_ReturnType LockStatus = Dcm_Dsp_DidServices_LockService();

  DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_RETURNCONTROLTOECUFORALLSIGNALSTRYLOCK_ENTRY();

  Dcm_ReturnControlToEcuContext.returnCtrlToECUStatus = DCM_RET_CTRL_PENDING;

  /* !LINKSTO Dcm.EB.DidServices.Locking.ReturnControlToECUWhileLockedByRoeRequest,1 */
  if (LockStatus == DCM_E_RESOURCE_LOCKED)
  {
    Dcm_ReturnControlToEcuForAllSignals_HasTheLock = FALSE;
  }
  else
  {
    DCM_POSTCONDITION_ASSERT(LockStatus == DCM_E_OK, DCM_INTERNAL_API_ID);
    Dcm_ReturnControlToEcuForAllSignals_HasTheLock = TRUE;
  }

  DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_RETURNCONTROLTOECUFORALLSIGNALSTRYLOCK_EXIT();
}
#if (DCM_DID_SIGNALS_COUNT > 0U)
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReturnControlToEcuWithAllOnesMask(
  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_CONST) dataCfg,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) curNrc,
  uint8 DidControlMask,
  uint8 DidControlMaskSize
)
#else /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReturnControlToEcuWithAllOnesMask(
  P2VAR(Dcm_DidDataType, AUTOMATIC, DCM_CONST) dataCfg,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) curNrc
)
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
{
  Std_ReturnType RetVal = E_OK;
#if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON)
  if (DidControlMask == DCM_CONTROLMASK_EXTERNAL)
  {
    switch (DidControlMaskSize)
    {
      case 1U:
        RetVal = dataCfg->ReturnControlToEcuSyncUnion.ReturnControlToEcuSyncMaskExternSize1(0xFFU, curNrc);
        break;

      case 2U:
        RetVal = dataCfg->ReturnControlToEcuSyncUnion.ReturnControlToEcuSyncMaskExternSize2(0xFFFFU, curNrc);
        break;

      case 3U:
        RetVal = dataCfg->ReturnControlToEcuSyncUnion.ReturnControlToEcuSyncMaskExternSize3(0xFFFFFFFFU, curNrc);
        break;

      case 4U:
        RetVal = dataCfg->ReturnControlToEcuSyncUnion.ReturnControlToEcuSyncMaskExternSize4(0xFFFFFFFFU, curNrc);
        break;

      /* CHECK: NOPARSE */
      default: DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_RETURNCONTROLTOECU);
               break;
      /* CHECK: PARSE */
    }
  }
  else
#endif /* #if (DCM_CONTROL_MASK_EXTERNAL_ENABLED == STD_ON) */
  {
    /* Call the user function to return control to ECU of an IOControl */
    RetVal = dataCfg->ReturnControlToEcuSyncUnion.ReturnControlToEcuSync(curNrc);
  }
  return RetVal;
}
#endif /* #if (DCM_DID_SIGNALS_COUNT > 0U) */

FUNC(void, DCM_CODE) Dcm_Dsp_InputOutputControlByIdentifier_ReturnControlToEcuForAllSignals(void)
{
  DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_RETURNCONTROLTOECUFORALLSIGNALS_ENTRY();

#if(DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
  /* The ReturnControlToECUForAll operation for configured DIDs is not called if the
   * ReturnControlToECUForAll for unconfigured DIDs is returning Pending. */
  if(DCM_RET_CTRL_DEFAULT_PENDING != Dcm_IoControlDefaultStatus.ReturnControlToECUForAllSignalsStatus)
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */
  {
    /* Call the ReturnControlToEcuForAllSignals handler for the configured DIDs. */
    Dcm_Dsp_InputOutputControlByIdentifier_ConfiguredDID_ReturnControlToECUForAllSignals();
  }
#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
  /* !LINKSTO Dcm.EB.IOControl.DefaultInterface.Enabled.ReturnControlToECUForAll.DefaultSession,1 */
  /* Check if the ReturnControlToECUForAll operation is finished for the configured DIDs.
   * ReturnControlToECUForAll operation for unconfigured DIDs is called only after the configured DIDs are finished. */
  if(DCM_RET_CTRL_FINISHED == Dcm_ReturnControlToEcuContext.returnCtrlToECUStatus)
  {
    if(DCM_RET_CTRL_DEFAULT_PENDING == Dcm_IoControlDefaultStatus.ReturnControlToECUForAllSignalsStatus)
    {
      /* Call the ReturnControlToEcuForAllSignals handler with DCM_PENDING. */
      Dcm_Dsp_InputOutputControlByIdentifier_UnconfiguredDID_ReturnControlToECUForAllSignals(DCM_PENDING);
    }
    else
    {
      /* Call the ReturnControlToEcuForAllSignals handler with DCM_INITIAL. */
      Dcm_Dsp_InputOutputControlByIdentifier_UnconfiguredDID_ReturnControlToECUForAllSignals(DCM_INITIAL);
    }
  }
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */

  DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_RETURNCONTROLTOECUFORALLSIGNALS_EXIT();
}

FUNC(void, DCM_CODE) Dcm_Dsp_InputOutputControlByIdentifier_SessionChange(
  Dcm_SesCtrlType OldSession, Dcm_SesCtrlType NewSession)
{

  DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SESSIONCHANGE_ENTRY(OldSession, NewSession);

  TS_PARAM_UNUSED(OldSession);

  if (DCM_DEFAULT_SESSION == NewSession)
  {
    const Dcm_DidReturnControlToEcuStatusType ReturnCtrlToECUStatus
      = Dcm_Dsp_InputOutputControlByIdentifier_GetReturnControlToEcuStatus();

#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
    const Dcm_DidReturnControlToEcuStatusDefaultType ReturnCtrlToECUDefaultStatus
      = Dcm_Dsp_InputOutputControlByIdentifier_GetReturnControlToEcuDefaultStatus();
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */

    /* The state of the DIDs is undefined until control has been returned to ECU for all. */
#if (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON)
    if((DCM_RET_CTRL_PENDING != ReturnCtrlToECUStatus)
        && (DCM_RET_CTRL_DEFAULT_PENDING != ReturnCtrlToECUDefaultStatus))
#else
    if((DCM_RET_CTRL_PENDING != ReturnCtrlToECUStatus))
#endif /* (DCM_REQUEST_SUPPLIER_DEFAULT_DID_READ == STD_ON) */
    {
      /* !LINKSTO Dcm.EB.IOControl.ProtocolPreemption.ReturnControlToEcu.LockDidServices,1 */
      /* ReturnControlToECU interface calls are done immediately. */
      Dcm_Dsp_InputOutputControlByIdentifier_ReturnControlToEcuForAllSignals();
    }
  }

  DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SESSIONCHANGE_EXIT();
}

#endif /* #if (DCM_RETURNCONTROLTOECU_ENABLED == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if(DCM_DSP_USE_SERVICE_0X2F == STD_ON) */

/*==================[end of file]================================================================*/
