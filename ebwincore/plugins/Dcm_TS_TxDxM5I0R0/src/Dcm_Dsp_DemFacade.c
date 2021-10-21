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

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 14.6 (required)
 *     "For any iteration statement there shall be at most one break statement used
 *     for loop termination"
 *
 *     Reason:
 *     The iteration needs to be terminated when some particular conditions are satisfied.
 *     Adapting such algorithm optimizes the code.
 *
 *  MISRA-2) Deviated Rule: 15.2 (required)
 *     "An unconditional break statement shall terminate every non-empty switch clause."
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
  *  MISRAC2012-2) Deviated Rule: 15.4 (required)
  *     "There should be no more than one break or goto statement used to terminate any iteration
  *     statement."
  *
  *     Reason:
  *     The iteration needs to be terminated when some particular conditions are satisfied.
  *     Adapting such algorithm optimizes the code.
  *
  *  MISRAC2012-3) Deviated Rule: 8.9 (advisory)
  *     "An object should be defined at block scope if its identifier only
  *     appears in a single function."
  *
  *     Reason:
  *     The object is defined in this way because the values that are stored
  *     in them are required for the next call of this function and should be
  *     hold in the data segment memory.
  *
  *  MISRAC2012-4) Deviated Rule: 16.1 (required)
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
 * TASKING-2) Deviated Rule: W549
 * W549: condition is always true violated
 *
 * Reason:
 * This warning is a false positive. The false condition (which terminates the loop) is reached.
 * This is proven by the coverage metrics and by the fact that the while loop is not infinite.
 *
 */
/*==================[inclusions]==================================================================*/

#include <Std_Types.h>                                                  /* AUTOSAR standard types */
#include <TSAutosar.h>

#include <Dcm_Dsp_DemFacade.h>

#if (DCM_DEM_CLIENT_REF_USED == STD_ON)
#include <Dem.h>
#endif /* #if (DCM_DEM_CLIENT_REF_USED == STD_ON) */

#include <Dcm_Trace.h>

#if (DCM_DSP_USE_DEMFACADE == STD_ON)

/*==================[macro definitions]===========================================================*/

#if (defined DCM_GET_CFG_FOR_RX_PDU)
  #error "DCM_GET_CFG_FOR_RX_PDU is already defined"
#endif
#define DCM_GET_CFG_FOR_RX_PDU(DcmRxPduId) \
  Dcm_MainConnectionConfig[Dcm_RxPduIdConfig[(DcmRxPduId)].MainConnectionIndex]

#if (defined DCM_GET_CFG_FOR_RX_CTXT)
  #error "DCM_GET_CFG_FOR_RX_CTXT is already defined"
#endif
#define DCM_GET_CFG_FOR_RX_CTXT(pMsgContext) DCM_GET_CFG_FOR_RX_PDU((pMsgContext)->dcmRxPduId)

#if (defined DCM_FIVE_BYTES_OF_HEADER_PER_DTC)
  #error "DCM_FIVE_BYTES_OF_HEADER_PER_DTC is already defined"
#endif
#define DCM_FIVE_BYTES_OF_HEADER_PER_DTC 5U

/*==================[type declarations]===========================================================*/

/*==================[internal function declarations]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

#if ((DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON))

/** \brief _Setup function for obtaining DTC status from DEM */
STATIC FUNC(void, DCM_CODE) Dcm_DemFacade_GetDTCStatus_Setup
(
    P2VAR(Dcm_GetDTCStatusContextType, AUTOMATIC, DCM_VAR) GetDTCStatusContext,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) GetDTCStatusContextResponseBuffer,
    Dcm_MsgLenType AvailableBufferSize
);

/** \brief _Execute function for obtaining DTC Status from DEM */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_GetDTCStatus_Execute
(
    const Dcm_DemClientIdType ClientId,
    P2VAR(Dcm_GetDTCStatusContextType, AUTOMATIC, DCM_VAR) GetDTCStatusContext,
#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
    uint8 RequestedService,
#endif /* #if (DCM_DSP_USE_SERVICE_0XAF == STD_ON) */
    uint8 RequestedSubService
);
#endif /* #if ((DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON)) */

#if (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON)

#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
/** \brief _Setup function for obtaining SI30 from DEM */
STATIC FUNC(void, DCM_CODE) Dcm_DemFacade_GetSI30_Setup
(
    P2VAR(Dcm_GetSI30ContextType, AUTOMATIC, DCM_VAR) GetSI30Context,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) GetDTCStatusContextResponseBuffer,
    Dcm_MsgLenType AvailableBufferSize
);
#endif /*#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)*/

/** \brief _Setup function for obtaining Extended Data Records from DEM */
STATIC FUNC(void, DCM_CODE) Dcm_DemFacade_ReadEventDataRecord_Setup
(
    P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) ReadExtDataRecContextResponseBuffer,
    Dcm_MsgLenType AvailableBufferSize,
    uint32 DTCMaskSetup,
    uint8 DataRecordNumber,
    Dcm_MsgLenType SizeOfOnePage,
    uint8 DataKind,
    uint8 DTCFormat
);

/** \brief _Execute function for obtaining Extended Data Records from DEM */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_ReadEventDataRecord_Execute
(
  const Dcm_DemClientIdType ClientId
#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
  ,
  P2VAR(Dcm_GetSI30ContextType, AUTOMATIC, DCM_VAR) GetSI30Context
  ,
  P2VAR(boolean, AUTOMATIC, DCM_VAR) CurrentDTCProcessingReady
#endif /* #if(DCM_DSP_USE_SERVICE_0XAF == STD_ON) */
);

/** \brief The function for disabling the DTC Record Update
 **
 ** \param[in]     ClientId                Identifier corresponding to UDS/OBD client.
 ** \param[in/out] ControlRecordUpdate     Pointer to flag variable to indicate success or failure.
 **
 ** \retval DCM_FACADE_OK                  Disable was successful
 ** \retval DCM_FACADE_NOT_OK              Disable Failed
 ** \retval DCM_FACADE_PENDING             Disable operation returns Pending.
 ** \retval DCM_FACADE_REQUESTOUTOFRANGE   Disable operation returns Wrong DTC
 **                                        or Wrong DTC origin.
 ** \retval DCM_FACADE_WRONGCONDITION      Disable operation returns Wrong condition.
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_DisableDTCRecordUpdate
(
  const Dcm_DemClientIdType ClientId,
  P2VAR(boolean, AUTOMATIC, DCM_VAR) ControlRecordUpdate
);

#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
/** \brief The function for disabling the DTC Record Update
 **
 ** \param[in]     ClientId              Identifier corresponding to UDS/OBD client.
 ** \param[in/out] DiagnosticDataContext Diagnostic Data Context for current operation.
 ** \param[in/out] GetSI30Context Status Indicator Specific Context.
 **
 ** \retval DCM_FACADE_OK                 Read was successful.
 ** \retval DCM_FACADE_NOT_OK             Read Failed.
 ** \retval DCM_FACADE_WRONGCONDITION     Wrong Parameters provided to DEM.
 ** \retval DCM_FACADE_BUFFERFULL         Provided buffer cannot hold the response.
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_GetStatusIndicator
(
  Dcm_DemClientIdType ClientId,
  P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext,
  P2VAR(Dcm_GetSI30ContextType, AUTOMATIC, DCM_VAR) GetSI30Context
);
#endif /*#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)*/

/** \brief The function for requesting DEM to select an event data (EDR/FF).
 **
 ** \param[in]     ClientId               Identifier corresponding to UDS/OBD client.
 ** \param[in/out] GetSI30Context         Status Indicator Specific Context.
 ** \param[in/out] DiagnosticDataContext  Diagnostic Data Context for current operation.
 **
 ** \retval DCM_FACADE_OK                 Read was successful.
 ** \retval DCM_FACADE_NOT_OK             Read Failed.
 ** \retval DCM_FACADE_PENDING            DEM has returned with PENDING condition.
 ** \retval DCM_FACADE_REQUESTOUTOFRANGE  DEM has rejected the request with provided arguments.
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_SelectEventData
(
  Dcm_DemClientIdType ClientId,
#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
  P2VAR(Dcm_GetSI30ContextType, AUTOMATIC, DCM_VAR) GetSI30Context,
#endif
  P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext
);

#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
/** \brief The function is used to obtain the status indicator for service ReadGenericInformation (0xAF).
 **
 ** \param[in]    ClientId               Identifier corresponding to UDS/OBD client.
 ** \param[inout] DiagnosticDataContext  Pointer to the diagnostic data context.
 ** \param[inout] GetSI30Context         Pointer to StatusIndicator context.
 ** \param[inout] ReportErrorFromDEM     Pointer to the DemErrorStatus.
 **
 ** \retval DCM_FACADE_OK                 StatusIndicator was obtained.
 ** \retval DCM_FACADE_NOT_OK             StatusIndicator could not be selected or read.
 ** \retval DCM_FACADE_PENDING            Operation did not complete, it must be called again.
 ** \retval DCM_FACADE_REQUESTOUTOFRANGE  WrongDTC or WrongDTCOrigin selected.
 ** \retval DCM_FACADE_WRONGCONDITION     The process has not been able to obtain the SI.
 ** \retval DCM_FACADE_BUFFERFULL         Buffer is not enough to write StatusIndicator
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_ReadStatusIndicator_Execute(
    Dcm_DemClientIdType ClientId,
    P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext,
    P2VAR(Dcm_GetSI30ContextType, AUTOMATIC, DCM_VAR) GetSI30Context,
    P2VAR(boolean, AUTOMATIC, DCM_VAR) ReportErrorFromDEM);

#endif /* #if (DCM_DSP_USE_SERVICE_0XAF == STD_ON) */

/** \brief The function for reading the expected amount of data to read.
 **
 ** \param[in]     ClientId              Identifier corresponding to UDS/OBD client.
 ** \param[in/out] DiagnosticDataContext Diagnostic Data Context for current operation.
 **
 ** \retval DCM_FACADE_OK                 Read was successful.
 ** \retval DCM_FACADE_NOT_OK             Read Failed.
 ** \retval DCM_FACADE_PENDING            DEM has returned with PENDING condition.
 ** \retval DCM_FACADE_REQUESTOUTOFRANGE  DEM has rejected the request with provided arguments.
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_GetSizeOfEventData
(
  Dcm_DemClientIdType ClientId,
  P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext
);

/** \brief The function for reading requested data from DEM.
 **
 ** \param[in]     ClientId              Identifier corresponding to UDS/OBD client.
 ** \param[in/out] DiagnosticDataContext Diagnostic Data Context for current operation.
 **
 ** \retval DCM_FACADE_OK                 Read was successful.
 ** \retval DCM_FACADE_NOT_OK             Read Failed.
 ** \retval DCM_FACADE_PENDING            DEM has returned with PENDING condition.
 ** \retval DCM_FACADE_REQUESTOUTOFRANGE  DEM has rejected the request with provided arguments.
 ** \retval DCM_FACADE_BUFFERFULL         Provided buffer cannot hold the response.
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_GetEventData
(
  Dcm_DemClientIdType ClientId,
  P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext
);

/** \brief The function for enabling the DTC Record Update
 **
 ** \param[in]     ClientId                Identifier corresponding to UDS/OBD client.
 ** \param[in/out] ControlRecordUpdate     Pointer to flag variable to indicate success or failure.
 **
 ** \retval DCM_FACADE_OK                  Enable was successful
 ** \retval DCM_FACADE_NOT_OK              Enable failed
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_EnableDTCRecordUpdate
(
  const Dcm_DemClientIdType ClientId,
  P2VAR(boolean, AUTOMATIC, DCM_VAR) ControlRecordUpdate
);
/** \brief Function to process the DEM response from the select data function.
 **
 ** \param[in]     ResultCodeFromDEM     Result code from Last DEM interaction.
 ** \param[in/out] DiagnosticDataContext Diagnostic Data Context for current operation
 **
 ** \retval DCM_FACADE_OK                 Read was successful.
 ** \retval DCM_FACADE_NOT_OK             Read Failed.
 ** \retval DCM_FACADE_PENDING            DEM has returned with PENDING condition.
 ** \retval DCM_FACADE_REQUESTOUTOFRANGE  DEM has rejected the request with provided arguments.
 ** \retval DCM_FACADE_BUFFERFULL         Provided buffer cannot hold the response.
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_ProcessDEMResponseFromSelectData
(
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext,
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
    uint8 ResultCodeFromDEM
);

/** \brief Function to process the DEM response from the get data function.
 **
 ** \param[in]     ResultCodeFromDEM     Result code from Last DEM interaction.
 ** \param[in/out] DiagnosticDataContext Diagnostic Data Context for current operation
 **
 ** \retval DCM_FACADE_OK                 Read was successful.
 ** \retval DCM_FACADE_NOT_OK             Read Failed.
 ** \retval DCM_FACADE_PENDING            DEM has returned with PENDING condition.
 ** \retval DCM_FACADE_REQUESTOUTOFRANGE  DEM has rejected the request with provided arguments.
 ** \retval DCM_FACADE_BUFFERFULL         Provided buffer cannot hold the response.
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_ProcessDEMResponseFromGetData
(
    P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext,
    uint8 ResultCodeFromDEM
);

/** \brief The function for reading requested data from DEM.
 **
 ** \param[in] DataKind                     Variable to see the data kind (EDR/FF).
 ** \param[in] ResultCodeFromDEM            Result code from Last DEM interaction.
 ** \param[in] CurrentExtendedRecordNumber  Current Data record being processed.
 ** \param[in] AmountOfDataToStore          Amount of data to store reported from Dem
 **                                         in the current run.
 ** \param[in/out] DiagnosticDataContext    Diagnostic Data Context for current operation
 **
 ** \retval DCM_FACADE_OK                 Read was successful.
 ** \retval DCM_FACADE_NOT_OK             Read Failed.
 ** \retval DCM_FACADE_PENDING            DEM has returned with PENDING condition.
 ** \retval DCM_FACADE_REQUESTOUTOFRANGE  DEM has rejected the request with provided arguments.
 ** \retval DCM_FACADE_BUFFERFULL         Provided buffer cannot hold the response.
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_ProcessDEMResponseFromGetSize
(
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    uint8 DataKind,
#endif
    uint8 ResultCodeFromDEM
);
#endif /* #if (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) */

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON)
/** \brief _Setup function for reportSeverityInformationOfDTC sub-service */
STATIC FUNC(void, DCM_CODE) Dcm_DemFacade_SeverityInformation_Setup
(
    P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) ReadSeverityInformationContextResponseBuffer,
    Dcm_MsgLenType AvailableBufferSize,
    uint8 DTCFormat,
    uint32 DTCMaskSetup,
    P2VAR(Dcm_GetDTCSeverityContextType, AUTOMATIC, DCM_VAR) GetDTCSeverityContext
);

/** \brief _Execute function for obtaining the Severity of a DTC from DEM */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_GetSeverityOfDTC_Execute
(
    const Dcm_DemClientIdType ClientId,
    P2VAR(Dcm_GetDTCSeverityContextType, AUTOMATIC, DCM_VAR) GetDTCSeverityContext,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) DTCSeverity
);

/** \brief _Execute function for obtaining the Functional Unit Of DTC of a DTC from DEM */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_GetFunctionalUnitOfDTC_Execute
(
    const Dcm_DemClientIdType ClientId,
    P2VAR(Dcm_GetDTCSeverityContextType, AUTOMATIC, DCM_VAR) GetDTCSeverityContext,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) FunctionalUnitOfDTC
);
#endif /* #if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON) */

#if ((DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) || (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON))
/** \brief Data supplier for the DTC Status Availability Mask */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_DTCStatusAvailabilityMask
(
  const Dcm_DemClientIdType ClientId,
  Dcm_DataSupplierOperationType Operation,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) DataBuffer,
  P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) Size
);
#endif /* #if ((DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) || (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON)) */

#if (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON)
/** \brief _Setup function for obtaining the Number of filtered DTC from DEM */
STATIC FUNC(void, DCM_CODE) Dcm_DemFacade_NumberOfFilteredDTC_Setup
(
  P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) NumberOfFilteredDTCResponseBuffer,
  Dcm_MsgLenType AvailableBufferSize
);

/** \brief _Execute function for obtaining the Number of filtered DTCs from DEM */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_SetFilterObtainNumberOfFilteredDTC_Execute
(
  const Dcm_DemClientIdType ClientId,
  P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) GetDataOfFilteredDTCContext
);

/** \brief Auxiliary function to obtain the expected amount of data */
STATIC FUNC(Dcm_MsgLenType, DCM_CODE) Dcm_DemFacade_ExpectedAmountOfData(
  uint16 NumberOfFilteredDTCs,
  uint8 FilteredState,
  uint8 DTCFormat,
  uint8 SubServiceId
);

/** \brief Execute function for obtaining the filtered DTCs from DEM */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_GetFilteredDTC_Execute
(
  const Dcm_DemClientIdType ClientId,
  P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) GetDataOfFilteredDTCContext
);

/** \brief Execute function for obtaining the severity filtered DTCs from DEM */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_GetSeverityFilteredDTC_Execute
(
  const Dcm_DemClientIdType ClientId,
  P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) GetDataOfFilteredDTCContext
);

#endif /* #if (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) */

#if ( (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) || \
      (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) )
/** \brief Evaluate and process the result of a Dem_GetNextFilteredDTC facade call
 **
 ** \param[in] OpStatus Status of the service processing
 ** \param[in] FacadeReturnCodeGetEventData Return value of the facade call
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[inout] DiagnosticDataContext Pointer to the diagnostic data context
 ** \param[inout] ReadEventDataSubServiceContext Pointer to the subservice context
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 ** \retval DCM_E_NOT_OK       Current page buffer is full for services not support page buffering
 **/
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ProcessEventDataResult(
  Dcm_OpStatusType OpStatus,
  Std_ReturnType FacadeReturnCodeGetEventData,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext,
  P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ReadEventDataSubServiceContext);

/** \brief The function for processing the response in case of buffer full.
 **
 ** \param[in] OpStatus Status of the service processing
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[inout] DiagnosticDataContext Pointer to the diagnostic data context
 ** \param[inout] ReadEventDataSubServiceContext Pointer to the subservice context
 **
 ** \retval DCM_E_NOT_OK           Page buffering is active and no message was written in the current page.
 ** \retval DCM_E_INHIBIT          The request is for obd service and response in inhibited.
 ** \retval DCM_E_PROCESS_PAGE     Page buffering is active and message was written in page.
 ** \retval DCM_E_DONE             Page buffering is not active. Do not process anymore and return generalReject
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_ProcessServiceIDBufferfull
(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
    P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext,
#endif /* (DCM_PAGEDBUFFER_ENABLED == STD_ON) */
  P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ReadEventDataSubServiceContext
);
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
/** \brief Setup the Page Buffering functions service functions
 **
 ** \param[in] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[in] SubServiceID
 **
 **/
STATIC FUNC(void, DCM_CODE) Dcm_DspInternal_SetupPageBuffering(
  P2CONST(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  uint8 SubServiceID);
#endif /* #if (DCM_PAGEDBUFFER_ENABLED == STD_ON) */
#endif /* #if ( (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) || \
                (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) ) */
#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal constant definitions]===============================================*/

/*==================[internal data definitions]===================================================*/

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#if ( (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) || \
      (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || \
      (DCM_0XAF_EXT_SSVCH_ENABLED == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X14 == STD_ON) || (DCM_DSP_USE_SERVICE_0X04 == STD_ON) || \
      (DCM_DEMFACADE_LOCK_MECHANISM_ENABLE == STD_ON))

/** \brief This is the context variable for the Dem client,
 ** which maintains status and data information for the current Dem client being processed */
STATIC VAR(Dcm_DemClientContextType, DCM_VAR) Dcm_DemClientContext[DCM_NUM_DEM_CLIENTS];

#endif /* #if ( (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) || \
                (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || \
                (DCM_0XAF_EXT_SSVCH_ENABLED == STD_ON) || \
                (DCM_DSP_USE_SERVICE_0X14 == STD_ON) || (DCM_DSP_USE_SERVICE_0X04 == STD_ON)) || \
                (DCM_DEMFACADE_LOCK_MECHANISM_ENABLE == STD_ON)) */

#if ( (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON) )

/** \brief This is the context variable for holding status and data
 ** information for the GetDTC operation */
/* Deviation MISRAC2012-3 */
STATIC VAR(Dcm_GetDTCStatusContextType, DCM_VAR) Dcm_GetDTCStatusContext;

#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
/** \brief This is the context variable for holding status and data
 ** information for the GetDTC operation */
/* Deviation MISRAC2012-3 */
STATIC VAR(Dcm_GetSI30ContextType, DCM_VAR) Dcm_GetSI30Context;
#endif /*#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)*/

#endif /* #if ( (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON) ) */

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON)
/** \brief This is the context variable for holding the status
 ** information for the SeverityOfDTC and FunctionalUnitOfDTC operations. */
/* Deviation MISRAC2012-3 */
STATIC VAR(Dcm_GetDTCSeverityContextType, DCM_VAR) Dcm_GetDTCSeverityContext;
#endif /* #if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON) */

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[external constant definitions]===============================================*/

/*==================[external data definitions]===================================================*/

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#if ( (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) || \
      (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || \
      (DCM_0XAF_EXT_SSVCH_ENABLED == STD_ON) )

#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
/** \brief This is the context variable for the Service context */
VAR(Dcm_ReadeventEventDataServicesContextType, DCM_VAR) Dcm_ReadEventDataServicesContext;
#endif /*#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)*/

#endif /* #if ( (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) || \
                (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || \
                (DCM_0XAF_EXT_SSVCH_ENABLED == STD_ON) ) */

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[external function definitions]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

/* !LINKSTO Dcm.Dsn.IB.DemFacade.Init,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_DemFacade_Init(void)
{
#if(DCM_DEMFACADE_LOCK_MECHANISM_ENABLE == STD_ON)
  uint16_least ClientIndex;
#endif

  DBG_DCM_DSP_DEMFACADE_INIT_ENTRY();

#if(DCM_DEMFACADE_LOCK_MECHANISM_ENABLE == STD_ON)
  /* Deviation TASKING-1 */
  for (ClientIndex=0U; ClientIndex<DCM_NUM_DEM_CLIENTS; ClientIndex++)
  {
    Dcm_DemClientContext[ClientIndex].Mutex = DCM_MUTEX_UNLOCKED;
  }
#endif

  DBG_DCM_DSP_DEMFACADE_INIT_EXIT();
}

#if (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.DemFacade.GetDTCInfoBySelect_Process,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_GetDTCInfoBySelect_Process(
  Dcm_DemClientIdType ClientId,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  Std_ReturnType FacadeReturnCodeGetDTC = DCM_FACADE_OK;
  Std_ReturnType ReturnCodeFromSelect = E_NOT_OK;
  Std_ReturnType FacadeReturnCodeGetEventData = DCM_FACADE_NOT_OK;
  P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(ClientId);

  DBG_DCM_DSP_DEMFACADE_GETDTCINFOBYSELECT_PROCESS_ENTRY(ClientId, OpStatus, pMsgContext);
  /* Deviation MISRAC2012-4 */
  switch(OpStatus)
  {
    /* Deviation MISRAC2012-1 */
    /* Deviation MISRA-2 */
    case DCM_INITIAL:
    {
      Dcm_MsgLenType PossitionOnResponse = pMsgContext->resDataLen;
      Dcm_MsgLenType AmountOfDataAvailableOnResponseBuffer = 0U;


      if (pMsgContext->resMaxDataLen > pMsgContext->resDataLen)
      {
        AmountOfDataAvailableOnResponseBuffer =
            pMsgContext->resMaxDataLen-pMsgContext->resDataLen;
      }
      /*-------------------------------------- INITIAL --------------------------------------*/
#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
      if ( (ClientContextPtr->ServiceID == DCM_DSP_SERVICE_0XAF) &&
           (AmountOfDataAvailableOnResponseBuffer < 2U) )
      {
        Dcm_GetDTCStatusContext.StatusOfDTC = DCM_NOT_OBTAINED;
        FacadeReturnCodeGetDTC = DCM_FACADE_BUFFERFULL;
      }
      else
#endif
      {
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.InitialDemAPI,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.InitialDemAPI,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.InitialDemAPI,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.InitialDemAPISpecificDTC,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.InitialDemAPISpecificDTC,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.RetrieveFreezeFrame,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.SelectDTC.Parameters,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.SelectDTC.Parameters,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextFilteredStoredDTC.E_OK,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextFilteredStoredDTC.E_OK,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
        /* !LINKSTO Dcm.ReadGenericInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
        ReturnCodeFromSelect = Dem_SelectDTC(ClientId,
                                             ClientContextPtr->DTCMask,
                                             ClientContextPtr->DTCFormat,
                                             ClientContextPtr->MemoryOrigin);
        if(E_OK == ReturnCodeFromSelect)
        {
          Dcm_DemFacade_GetDTCStatus_Setup(&Dcm_GetDTCStatusContext,
          &(pMsgContext->resData[PossitionOnResponse]),
          AmountOfDataAvailableOnResponseBuffer);
        }
        else if(DEM_BUSY == ReturnCodeFromSelect)
        {
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.SelectDTC.DEM_BUSY,1*/
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.SelectDTC.DEM_BUSY,1*/
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.SelectDTC.DEM_BUSY,1*/
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.SelectDTC.DEM_BUSY,1*/
          /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SelectDTC.DEM_BUSY.NRC,1*/
          /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SelectDTC.DEM_BUSY.NRC,1*/
          Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
          break;
        }
        else
        {
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.SelectDTC.UnsupportedError.NRC,1*/
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.SelectDTC.UnsupportedError.DET,1*/
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.SelectDTC.UnsupportedError.NRC,1*/
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.SelectDTC.UnsupportedError.DET,1*/
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.SelectDTC.UnsupportedError.NRC,1*/
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.SelectDTC.UnsupportedError.DET,1*/
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.SelectDTC.UnsupportedError.NRC,1*/
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.SelectDTC.UnsupportedError.DET,1*/
          /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SelectDTC.UnsupportedError.NRC,1*/
          /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SelectDTC.UnsupportedError.DET,1*/
          /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SelectDTC.UnsupportedError.NRC,1*/
          /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SelectDTC.UnsupportedError.DET,1*/
          Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
#if (DCM_DEV_ERROR_DETECT == STD_ON)
          DCM_DET_REPORT_ERROR(DCM_SVCID_READDTCINFORMATION, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
          break;
        }
#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
        /* Set-up the end indicator for page buffering */
        ClientContextPtr->PageBufferingDone = FALSE;
#endif
        PossitionOnResponse++;
        AmountOfDataAvailableOnResponseBuffer--;
#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
        if (ClientContextPtr->ServiceID == DCM_DSP_SERVICE_0XAF)
        {
          /* For the SI30, only the setup of a position in the response
           * buffer needs to be setup */
          Dcm_DemFacade_GetSI30_Setup(&Dcm_GetSI30Context,
                                      &(pMsgContext->resData[PossitionOnResponse]),
                                      AmountOfDataAvailableOnResponseBuffer);
          PossitionOnResponse++;
          AmountOfDataAvailableOnResponseBuffer--;
        }
        ClientContextPtr->DiagnosticDataContext.ServiceID = ClientContextPtr->ServiceID;
#endif
        Dcm_DemFacade_ReadEventDataRecord_Setup(&(ClientContextPtr->DiagnosticDataContext),
                                          &(pMsgContext->resData[PossitionOnResponse]),
                                          AmountOfDataAvailableOnResponseBuffer,
                                          ClientContextPtr->DTCMask,
                                          ClientContextPtr->DataRecordNumber,
                                          ClientContextPtr->SizeOfOnePage,
                                          ClientContextPtr->DataKind,
                                          ClientContextPtr->DTCFormat);
      }
    }
    /* no break */
    case DCM_PENDING:
      /*-------------------------------------- PENDING --------------------------------------*/
#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
      if(FacadeReturnCodeGetDTC == DCM_FACADE_OK)
#endif /* #if (DCM_DSP_USE_SERVICE_0XAF == STD_ON) */
      {
        FacadeReturnCodeGetDTC = Dcm_DemFacade_GetDTCStatus_Execute(ClientId,
                                                                    &Dcm_GetDTCStatusContext,
#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
                                                                    ClientContextPtr->ServiceID,
#endif /* #if (DCM_DSP_USE_SERVICE_0XAF == STD_ON) */
                                                                    ClientContextPtr->SubServiceID);
      }
      if(FacadeReturnCodeGetDTC == DCM_FACADE_OK)
      {
        /* Execute read of Event Data Record only if obtaining the DTC Status has not failed */
        FacadeReturnCodeGetEventData = Dcm_DemFacade_ReadEventDataRecord_Execute(
                                       ClientId
#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
                                       ,
                                       &Dcm_GetSI30Context
                                       ,
                                       &ClientContextPtr->CurrentDTCProcessingReady
#endif /*#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)*/
                                       );
      }
      break;

    case DCM_UPDATE_PAGE:
      /*-------------------------------------- UPDATE_PAGE --------------------------------------*/
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
      /* Update the size of the available buffer */
      ClientContextPtr->DiagnosticDataContext.BufferSize = pMsgContext->resMaxDataLen;
      /* Reset write pointer and offset pointer */
      ClientContextPtr->DiagnosticDataContext.ResponseBuffer = &(pMsgContext->resData[0]);
      ClientContextPtr->DiagnosticDataContext.NextPossitionToFillOnResponseBuffer=0U;
      /* reset counter of data counter for next execution */
      ClientContextPtr->DiagnosticDataContext.AmountOfDataStoredOnResponseOnCurrentRun = 0U;

      /* Execute read of Extended Data Record. On arrival to this point,
       * complete size of data should be known. */
      FacadeReturnCodeGetEventData = Dcm_DemFacade_ReadEventDataRecord_Execute(
                                      ClientId
#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
                                      ,
                                      &Dcm_GetSI30Context
                                      ,
                                      &ClientContextPtr->CurrentDTCProcessingReady
#endif /*#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)*/
                                       );

#endif /*#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)*/
      break;

    case DCM_CANCEL:
      /* !LINKSTO Dcm.EB.DemIF.RequestCancellation, 1 */
      if( DCM_RECORDUPDATE_DISABLED == ClientContextPtr->DiagnosticDataContext.ControlRecordUpdate)
      {
        ClientContextPtr->DiagnosticDataContext.ControlRecordUpdate = DCM_RECORDUPDATE_DOENABLE;
        (void)Dcm_DemFacade_EnableDTCRecordUpdate(ClientId,
                                                  &ClientContextPtr->DiagnosticDataContext.ControlRecordUpdate);
      }
      ServiceProcessStatus = DCM_E_DONE;
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_READDTCINFORMATION);
      break;
    /* CHECK: PARSE */
  }
  if(OpStatus != DCM_CANCEL)
  {
    /* If the obtaining of the Status of the DTC has failed process error code from executer */
    if(Dcm_GetDTCStatusContext.StatusOfDTC == DCM_NOT_OBTAINED)
    {
      ServiceProcessStatus =
        Dcm_DspInternal_ProcessEventDataResult(OpStatus,
                                             FacadeReturnCodeGetDTC,
                                             pMsgContext,
                                             &(ClientContextPtr->DiagnosticDataContext),
                                             ClientContextPtr);
    }
    else
    {
      if((ClientContextPtr->DTCStatusWrittenInResponseBuffer == FALSE)&&
         (FacadeReturnCodeGetEventData!=DCM_FACADE_PENDING))
      {
        /* Update response length for the DTC Status and use the control flag to
         * avoid incrementing erroneously
         * the resDataLen for next pending processing */
        pMsgContext->resDataLen++;
        ClientContextPtr->AmountOfDataWrittenOnCurrentPage++;
        /* Set flag to true as to not re-increase the count of written data erroneously
         * in later re-entry */
        ClientContextPtr->DTCStatusWrittenInResponseBuffer = TRUE;
        /* Account for Status Indication if relevant */
#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
        if((Dcm_GetSI30Context.StatusOfSI30ForCurrentDTC == DCM_OBTAINED)&&
           (ClientContextPtr->ServiceID == DCM_DSP_SERVICE_0XAF))
        {
          /* Account for the Status Indicator */
          pMsgContext->resDataLen++;
          ClientContextPtr->AmountOfDataWrittenOnCurrentPage++;
          /* Reset flag */
          Dcm_GetSI30Context.StatusOfSI30ForCurrentDTC = DCM_NOT_OBTAINED;
        }
#endif /*#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)*/
      }
      ServiceProcessStatus =
        Dcm_DspInternal_ProcessEventDataResult(OpStatus,
                                             FacadeReturnCodeGetEventData,
                                             pMsgContext,
                                             &(ClientContextPtr->DiagnosticDataContext),
                                             ClientContextPtr);
    }
  }

#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
  if ( (FacadeReturnCodeGetEventData != DCM_FACADE_PENDING) &&
       (FacadeReturnCodeGetEventData != DCM_FACADE_BUFFERFULL) )
  {
    ClientContextPtr->CurrentDTCProcessingReady = TRUE;
  }
#endif

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
  if((FacadeReturnCodeGetEventData != DCM_FACADE_PENDING) &&
      (ClientContextPtr->DiagnosticDataContext.AmountOfDataToWriteOnResponse ==
       ClientContextPtr->DiagnosticDataContext.AmountOfDataStoredOnCompleteResponse))
  {
    ClientContextPtr->PageBufferingDone = TRUE;
  }
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U)) */

  DBG_DCM_DSP_DEMFACADE_GETDTCINFOBYSELECT_PROCESS_EXIT(ServiceProcessStatus, ClientId, OpStatus, pMsgContext);

  return ServiceProcessStatus;
}
#endif /* #if (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) */

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.DemFacade.GetDTCSeverityInfoBySelect,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_GetDTCSeverityInfoBySelect_Process(
  Dcm_DemClientIdType ClientId,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  Std_ReturnType FacadeReturnCodeGetDTC = DCM_FACADE_OK;
  P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(ClientId);
  Dcm_MsgLenType PossitionOnResponse = pMsgContext->resDataLen;
  Dcm_MsgLenType AmountOfDataAvailableOnResponseBuffer = 0U;
  Dcm_MsgLenType DataSize = 0U;

  DBG_DCM_DSP_DEMFACADE_GETDTCSEVERITYINFOBYSELECT_PROCESS_ENTRY(ClientId, OpStatus, pMsgContext);
  /* Deviation MISRAC2012-4 */
  switch(OpStatus)
  {
    /* Deviation MISRAC2012-1 */
    /* Deviation MISRA-2 */
    case DCM_INITIAL:
    {

      AmountOfDataAvailableOnResponseBuffer =
          pMsgContext->resMaxDataLen-pMsgContext->resDataLen;

      /* Initialization of DTC Status Mask */
      /* The follow operation returns always DCM_E_DONE for this data supplier */
      (void)Dcm_DemFacade_DTCStatusAvailabilityMask(ClientId,
                                                    DCM_DATA_SUPPLIER_SETUP, NULL_PTR, NULL_PTR);
      /* The follow operation returns always DCM_E_DONE for this data supplier */
      (void)Dcm_DemFacade_DTCStatusAvailabilityMask(ClientId,
                                                      DCM_DATA_SUPPLIER_GET_SIZE,
                                                      NULL_PTR,
                                                      &DataSize);
      /* !LINKSTO Dcm.ReadDTCInfo.reportSeverityInformationOfDTC.SelectDTC.Parameters,1 */
      FacadeReturnCodeGetDTC =
        Dcm_DemFacade_DTCStatusAvailabilityMask(ClientId,
                                                 DCM_DATA_SUPPLIER_GET_DATA,
                                                 &pMsgContext->resData[pMsgContext->resDataLen],
                                                 &DataSize);
      /* For positive response, update the message length. */
      if(DCM_FACADE_OK == FacadeReturnCodeGetDTC)
      {
        ClientContextPtr->AvailabilityMaskState = DCM_DATA_SUPPLIER_DONE;
        /* Update response length for the Status Availability Mask */
        pMsgContext->resDataLen += DataSize;
        PossitionOnResponse += DataSize;
        ClientContextPtr->AmountOfDataWrittenOnCurrentPage += DataSize;
      }
      else
      {
        ClientContextPtr->AvailabilityMaskState = DCM_DATA_SUPPLIER_FAILED;
      }

      /* Call Dem_SelectDTC */
      if( ClientContextPtr->AvailabilityMaskState == DCM_DATA_SUPPLIER_DONE)
      {
        /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
        if(E_OK != Dem_SelectDTC(ClientId,
                               ClientContextPtr->DTCMask,
                               ClientContextPtr->DTCFormat,
                               ClientContextPtr->MemoryOrigin))
        {
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.SelectDTC.DEM_BUSY.NRC,1 */
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.SelectDTC.UnsupportedError.NRC,1 */
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.SelectDTC.DEM_BUSY.DET,1 */
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.SelectDTC.UnsupportedError.DET,1 */
          FacadeReturnCodeGetDTC = DCM_FACADE_INVALID_VALUE;
          break;
        }
        Dcm_DemFacade_SeverityInformation_Setup(&(ClientContextPtr->DiagnosticDataContext),
                                        &(pMsgContext->resData[PossitionOnResponse]),
                                        AmountOfDataAvailableOnResponseBuffer,
                                        ClientContextPtr->DTCFormat,
                                        ClientContextPtr->DTCMask,
                                        &Dcm_GetDTCSeverityContext
                                        );
      }
    }
    /* no break */
    case DCM_PENDING:
    {
      /*-------------------------------------- PENDING --------------------------------------*/
      /* Only for sub-service 0x09 reportSeverityInformationOfDTC. */
      /* Reset position and response data */
      PossitionOnResponse = 0x02U;
      pMsgContext->resDataLen = 0x02U;

      /* Check if previous interface returned DCM_FACADE_OK and continue if this is TRUE. */
      if(FacadeReturnCodeGetDTC == DCM_FACADE_OK)
      {
        /* Call Dcm_DemFacade_GetSeverityOfDTC_Execute API. */
        FacadeReturnCodeGetDTC = Dcm_DemFacade_GetSeverityOfDTC_Execute(ClientId,&Dcm_GetDTCSeverityContext,&(pMsgContext->resData[PossitionOnResponse]));
      }

      /* Check if previous interface returned DCM_FACADE_OK and contiue if this is TRUE. */
      if(FacadeReturnCodeGetDTC == DCM_FACADE_OK)
      {
        /* Update response length and position in the buffer. */
        PossitionOnResponse++;
        pMsgContext->resDataLen++;
        AmountOfDataAvailableOnResponseBuffer--;

        /* Call Dcm_DemFacade_GetFunctionalUnitOfDTC_Execute API. */
        FacadeReturnCodeGetDTC = Dcm_DemFacade_GetFunctionalUnitOfDTC_Execute(ClientId,&Dcm_GetDTCSeverityContext,&(pMsgContext->resData[PossitionOnResponse]));
      }

      /* Check if previous interface returned DCM_FACADE_OK and contiue if this is TRUE. */
      if(FacadeReturnCodeGetDTC == DCM_FACADE_OK)
      {
        /* Update response length and position in the buffer. */
        PossitionOnResponse++;
        pMsgContext->resDataLen++;
        AmountOfDataAvailableOnResponseBuffer--;

        /* Populate buffer with the DTC number from request. */
        pMsgContext->resData[pMsgContext->resDataLen] =
        (uint8)(ClientContextPtr->DTCMask >> 16U);

        pMsgContext->resData[pMsgContext->resDataLen+1U] =
        (uint8)(ClientContextPtr->DTCMask >> 8U);

        pMsgContext->resData[pMsgContext->resDataLen+2U] =
        (uint8) ClientContextPtr->DTCMask;

        /* Update response length and position in the buffer. */
        pMsgContext->resDataLen += 3U;
        PossitionOnResponse += 3U;
        AmountOfDataAvailableOnResponseBuffer-= 3U;

        Dcm_DemFacade_GetDTCStatus_Setup(&Dcm_GetDTCStatusContext,
        &(pMsgContext->resData[PossitionOnResponse]),
        AmountOfDataAvailableOnResponseBuffer);

        /* Call Dem_GetDTCStatus API. */
        FacadeReturnCodeGetDTC = Dcm_DemFacade_GetDTCStatus_Execute(ClientId,
                                                                    &Dcm_GetDTCStatusContext,
#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
                                                                    ClientContextPtr->ServiceID,
#endif
                                                                    ClientContextPtr->SubServiceID);

        /* Update response length previous interface returned DCM_FACADE_OK. */
        if (FacadeReturnCodeGetDTC == DCM_FACADE_OK)
        {
          pMsgContext->resDataLen++;
        }
        else
          /* !LINKSTO Dcm.ReadDTCInfo.reportSeverityInformationOfDTC.GetStatusOfDTC.DEM_NO_SUCH_ELEMENT,1 */
          /* If Dem_GetDTCStatus returned DCM_FACADE_NO_SUCH_ELEMENT, return only positive response with mandatory parameters. */
          if (FacadeReturnCodeGetDTC == DCM_FACADE_NO_SUCH_ELEMENT)
          {
            pMsgContext->resDataLen = 0x02U;
            FacadeReturnCodeGetDTC = DCM_FACADE_OK;
          }
          else
          {
            /* Do nothing. */
          }
      }
    }
    break;

    case DCM_CANCEL:
      /* !LINKSTO Dcm.EB.DemIF.RequestCancellation, 1 */
      ServiceProcessStatus = DCM_E_DONE;
      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_READDTCINFORMATION);
      break;
    /* CHECK: PARSE */
  }

  if(OpStatus != DCM_CANCEL)
  {

        ServiceProcessStatus =
          Dcm_DspInternal_ProcessEventDataResult(OpStatus,
                                               FacadeReturnCodeGetDTC,
                                               pMsgContext,
                                               &(ClientContextPtr->DiagnosticDataContext),
                                               ClientContextPtr);
  }

  DBG_DCM_DSP_DEMFACADE_GETDTCSEVERITYINFOBYSELECT_PROCESS_EXIT(ServiceProcessStatus, ClientId, OpStatus, pMsgContext);

  return ServiceProcessStatus;
}
#endif /* #if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON) */

#if (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON)
/* Common Functionality to SetDTC filter for Sub-services
 * 0x01, 0x07, 0x11, 0x12 and 0x02, 0x08, 0x0A, 0x13, 0x14, 0x15, 0x17
 * Additionally it is used for the OBD-Services 0x03, 0x07, 0x0A */

/* !LINKSTO Dcm.Dsn.IB.DemFacade.GetDTCInfoByFilter_Process,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_GetDTCInfoByFilter_Process(
  Dcm_DemClientIdType ClientId,
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  Std_ReturnType FacadeReturnCode = DCM_FACADE_NOT_OK;
  P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(ClientId);

  DBG_DCM_DSP_DEMFACADE_GETDTCINFOBYFILTER_PROCESS_ENTRY(ClientId, OpStatus, pMsgContext);
  /* Deviation MISRAC2012-4 */
  switch (OpStatus)
  {
    /* Deviation MISRAC2012-1 */
    /* Deviation MISRA-2 */
    case DCM_INITIAL:
    {
      Dcm_MsgLenType PossitionOnResponse = 0U;
      Dcm_MsgLenType AvailableBufferSize =
          ClientContextPtr->SizeOfOnePage -
          ClientContextPtr->AmountOfDataWrittenOnCurrentPage;

      /* SubService-ID, only if Subservice */
      if (ClientContextPtr->SubServiceID !=
                     DCM_DSP_READDTCINFORMATION_UNSPECIFIC_ID)
      {
        PossitionOnResponse++;
        /* No AvailableBufferSize -= 1U;
         * if a subSID exists, it is already considered in AmountOfDataWrittenOnResponseBuffer */
      }
      if (ClientContextPtr->MemoryOriginState == DCM_DATA_SUPPLIER_REQUIRED)
      {
        PossitionOnResponse += 1U;
        AvailableBufferSize -= 1U;
      }
      if (ClientContextPtr->AvailabilityMaskState ==
              DCM_DATA_SUPPLIER_REQUIRED)
      {
        /* For operation DCM_DATA_SUPPLIER_GET_SIZE is not size necessary */
        Dcm_MsgLenType DataSize = 0U;
        /* The follow operation returns always DCM_E_DONE for this data supplier */
        (void)Dcm_DemFacade_DTCStatusAvailabilityMask(ClientId,
                                                      DCM_DATA_SUPPLIER_SETUP, NULL_PTR, NULL_PTR);
        /* The follow operation returns always DCM_E_DONE for this data supplier */
        (void)Dcm_DemFacade_DTCStatusAvailabilityMask(ClientId,
                                                      DCM_DATA_SUPPLIER_GET_SIZE,
                                                      NULL_PTR,
                                                      &DataSize);
        PossitionOnResponse += DataSize;
        AvailableBufferSize -= DataSize;
      }

      Dcm_DemFacade_NumberOfFilteredDTC_Setup(
          ClientContextPtr,
          &pMsgContext->resData[PossitionOnResponse],
          AvailableBufferSize);
    }
    /* no break */

    case DCM_PENDING:
    {
      /* ---------------------------- Report User-defined memory origin ----------------------- */
      if (ClientContextPtr->MemoryOriginState == DCM_DATA_SUPPLIER_REQUIRED)
      {
        pMsgContext->resData[pMsgContext->resDataLen] = (uint8)ClientContextPtr->MemoryOrigin;
        pMsgContext->resDataLen++;
        ClientContextPtr->AmountOfDataWrittenOnCurrentPage++;
        ClientContextPtr->MemoryOriginState = DCM_DATA_SUPPLIER_DONE;
      }
      /* ---------------------------- Obtain Status Availability Mask ------------------------- */
      if (ClientContextPtr->AvailabilityMaskState ==
              DCM_DATA_SUPPLIER_REQUIRED)
      {
        Dcm_MsgLenType DataSize = ClientContextPtr->SizeOfOnePage -
                      ClientContextPtr->AmountOfDataWrittenOnCurrentPage;
        /* !LINKSTO Dcm.ReadDTCInfo.reportNumberOfDTCBySeverityMaskRecord.Response,1 */
        FacadeReturnCode =
          Dcm_DemFacade_DTCStatusAvailabilityMask(ClientId,
                                                   DCM_DATA_SUPPLIER_GET_DATA,
                                                   &pMsgContext->resData[pMsgContext->resDataLen],
                                                   &DataSize);
        /* If previous interface returned a positive response, increase the response length. */
        if(DCM_FACADE_OK == FacadeReturnCode)
        {
          ClientContextPtr->AvailabilityMaskState = DCM_DATA_SUPPLIER_DONE;
          /* Update response length for the Status Availability Mask */
          pMsgContext->resDataLen += DataSize;
          ClientContextPtr->AmountOfDataWrittenOnCurrentPage += DataSize;
        }
        else
        {
          ClientContextPtr->AvailabilityMaskState = DCM_DATA_SUPPLIER_FAILED;
        }
      }

      /* ----------------- Set DTC Filter and Obtain number of Filtered DTCs ------------------ */
      if ( (ClientContextPtr->AvailabilityMaskState !=
                DCM_DATA_SUPPLIER_FAILED) &&
           ( (ClientContextPtr->FilteredDtcsState ==
                  DCM_DATA_SUPPLIER_REQUIRED) ||
             (ClientContextPtr->FilteredDtcsCountState ==
                  DCM_DATA_SUPPLIER_REQUIRED) ||
             (ClientContextPtr->DtcFormatIdState ==
                  DCM_DATA_SUPPLIER_REQUIRED) ) )
      {
        /* Report FormatId, set filter and obtain number of filtered DTCs */
        FacadeReturnCode = Dcm_DemFacade_SetFilterObtainNumberOfFilteredDTC_Execute(
                             ClientId, &(ClientContextPtr->DiagnosticDataContext));

        if(FacadeReturnCode == DCM_FACADE_OK)
        {
          /* Update response length for the number of filters.*/
          pMsgContext->resDataLen +=
              ClientContextPtr->DiagnosticDataContext.AmountOfDataStoredOnResponseOnCurrentRun;
          ClientContextPtr->DiagnosticDataContext.NextPossitionToFillOnResponseBuffer +=
              ClientContextPtr->DiagnosticDataContext.AmountOfDataStoredOnResponseOnCurrentRun;
          ServiceProcessStatus = DCM_E_DONE;
        }
      }
      /* --------------------------------- Read Filtered DTCs ---------------------------------- */
      if ( (ClientContextPtr->DiagnosticDataContext.ReportOnGetNumberOfFilters == DCM_OBTAINED)&&
           (ClientContextPtr->FilteredDtcsState ==
                DCM_DATA_SUPPLIER_REQUIRED))
      {
        if(ClientContextPtr->DiagnosticDataContext.FilterWithSeverity == TRUE)
        {
          FacadeReturnCode = Dcm_DemFacade_GetSeverityFilteredDTC_Execute(ClientId,
                              &(ClientContextPtr->DiagnosticDataContext));
        }
        else
        {
          FacadeReturnCode = Dcm_DemFacade_GetFilteredDTC_Execute(ClientId,
                              &(ClientContextPtr->DiagnosticDataContext));
        }
        ClientContextPtr->AmountOfDataWrittenOnCurrentPage +=
          ClientContextPtr->DiagnosticDataContext.AmountOfDataStoredOnResponseOnCurrentRun;
        ClientContextPtr->DiagnosticDataContext.AmountOfDataStoredOnResponseOnCurrentRun = 0U;

      }
      break;
    }
    case DCM_UPDATE_PAGE:
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
      /* Update the size of the available buffer */
      ClientContextPtr->DiagnosticDataContext.BufferSize =
          pMsgContext->resMaxDataLen - ClientContextPtr->DiagnosticDataContext.NextPossitionToFillOnResponseBuffer;
      /* Reset write pointer and offset pointer */
      ClientContextPtr->DiagnosticDataContext.ResponseBuffer = &(pMsgContext->resData[0]);
      /* reset counter of data counter for next execution */
      ClientContextPtr->DiagnosticDataContext.AmountOfDataStoredOnResponseOnCurrentRun = 0U;
      /* rewind response buffer and continue with loop to obtain data with facade function. */
      if(ClientContextPtr->DiagnosticDataContext.FilterWithSeverity == TRUE)
      {
        FacadeReturnCode = Dcm_DemFacade_GetSeverityFilteredDTC_Execute(ClientId,
                            &(ClientContextPtr->DiagnosticDataContext));
      }
      else
      {
        FacadeReturnCode = Dcm_DemFacade_GetFilteredDTC_Execute(ClientId,
                            &(ClientContextPtr->DiagnosticDataContext));
      }
#endif
      break;
    case DCM_CANCEL:
      /* !LINKSTO Dcm.EB.DemIF.RequestCancellation, 1 */
      ServiceProcessStatus = DCM_E_DONE;
      break;
    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_GETFILTEREDDTC);
      break;
    /* CHECK: PARSE */
  }
  if(OpStatus != DCM_CANCEL)
  {
    ServiceProcessStatus =
      Dcm_DspInternal_ProcessEventDataResult(OpStatus,
                                             FacadeReturnCode,
                                             pMsgContext,
                                             &(ClientContextPtr->DiagnosticDataContext),
                                             ClientContextPtr);
  }
  DBG_DCM_DSP_DEMFACADE_GETDTCINFOBYFILTER_PROCESS_EXIT(ServiceProcessStatus, ClientId, OpStatus, pMsgContext);
  return ServiceProcessStatus;
}
#endif  /* #if (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON)  */

#if ((DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON)||(DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON))
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
/* Auxiliary function to Update amount of data filled on response buffer */
FUNC(void, DCM_CODE) Dcm_DspInternal_UpdateAmountOfFilledDataOnResponseBuffer(PduIdType DcmRxPduId,
  Dcm_MsgLenType AmountOfFilledData)
{
#if (DCM_NUM_PROTOCOL > 1U)
  uint8 instIdx = DCM_GET_CFG_FOR_RX_PDU(DcmRxPduId).ProtocolIndex;
#else
  TS_PARAM_UNUSED(DcmRxPduId);
#endif

  DBG_DCM_DSPINTERNAL_UPDATEAMOUNTOFFILLEDDATAONRESPONSEBUFFER_ENTRY(DcmRxPduId,
    AmountOfFilledData);

  /* Update the amount of filled data on the response buffer, after the execution has finished */
  DCM_HSM_INFO(instIdx).TxBuffer.FilledLength = (PduLengthType) AmountOfFilledData;

  DBG_DCM_DSPINTERNAL_UPDATEAMOUNTOFFILLEDDATAONRESPONSEBUFFER_EXIT(DcmRxPduId,
    AmountOfFilledData);

}
#endif /*#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)*/
#endif /*#if ( (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) ||
               (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) )*/

#if ((DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) || \
     (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON))
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
/* Used to enable process page function to be called from Call Service function of DSP SM*/
FUNC(void, DCM_CODE) Dcm_DspInternal_EnableProcessPage (
  P2CONST(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  uint8 ProcessPageFuncId
)
{
  DBG_DCM_DSPINTERNAL_ENABLEPROCESSPAGE_ENTRY(pMsgContext,ProcessPageFuncId);

#if (DCM_NUM_PROTOCOL == 1U)
    TS_PARAM_UNUSED(pMsgContext);
#endif
  /* set process page function id */
  DCM_HSM_INFO(Dcm_MainConnectionConfig[Dcm_RxPduIdConfig[
      pMsgContext->dcmRxPduId].MainConnectionIndex].ProtocolIndex).processPageFuncId
    = (uint8)ProcessPageFuncId;

  DBG_DCM_DSPINTERNAL_ENABLEPROCESSPAGE_EXIT(pMsgContext,ProcessPageFuncId);
}
#endif /* #if (DCM_PAGEDBUFFER_ENABLED == STD_ON) */
#endif /* #if ((DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) ||
               (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON)) */

#if(DCM_DEMFACADE_LOCK_MECHANISM_ENABLE == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.DemFacade.LockClient,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_LockClient(Dcm_DemClientIdType ClientId)
{
  Std_ReturnType LockStatus = DCM_E_RESOURCE_LOCKED;
  P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr;

  DBG_DCM_DSP_DEMFACADE_LOCKCLIENT_ENTRY(ClientId);

  ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(ClientId);
  LockStatus = Dcm_Internal_GetLock(&(ClientContextPtr->Mutex));

  DBG_DCM_DSP_DEMFACADE_LOCKCLIENT_EXIT(ClientId, LockStatus);

  return LockStatus;
}

/* !LINKSTO Dcm.Dsn.IB.DemFacade.UnlockClient,1 */
FUNC(void, DCM_CODE) Dcm_Dsp_DemFacade_UnlockClient(Dcm_DemClientIdType ClientId)
{
  P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr;

  DBG_DCM_DSP_DEMFACADE_UNLOCKCLIENT_ENTRY(ClientId);

  ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(ClientId);
  Dcm_Internal_ReleaseLock(&(ClientContextPtr->Mutex));

  DBG_DCM_DSP_DEMFACADE_UNLOCKCLIENT_EXIT(ClientId);
}
#endif /* #if(DCM_DEMFACADE_LOCK_MECHANISM_ENABLE == STD_ON) */


#if ((DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) || \
     (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || \
     (DCM_0XAF_EXT_SSVCH_ENABLED == STD_ON) || \
     (DCM_DSP_USE_SERVICE_0X14 == STD_ON) || (DCM_DSP_USE_SERVICE_0X04 == STD_ON) || \
     (DCM_DEMFACADE_LOCK_MECHANISM_ENABLE == STD_ON))
FUNC_P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_CODE) Dcm_Dsp_DemFacade_GetClient(Dcm_DemClientIdType ClientId)
{
  P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_DemClientContext;
  uint16_least ClientIndex;

  DBG_DCM_DSP_DEMFACADE_GETCLIENT_ENTRY(ClientId);

  /* Deviation TASKING-1 */
  for (ClientIndex=0U; ClientIndex<DCM_NUM_DEM_CLIENTS; ClientIndex++)
  {
    if (ClientId == Dcm_DemClientIds[ClientIndex])
    {
      ClientContextPtr = &(Dcm_DemClientContext[ClientIndex]);
      break;
    }
  }

  DCM_POSTCONDITION_ASSERT(ClientIndex != DCM_NUM_DEM_CLIENTS, DCM_INTERNAL_API_ID);

  DBG_DCM_DSP_DEMFACADE_GETCLIENT_EXIT(ClientId, ClientContextPtr);

  return ClientContextPtr;
}
#endif /* #if ((DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) || \
               (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || \
               (DCM_0XAF_EXT_SSVCH_ENABLED == STD_ON) || \
               (DCM_DSP_USE_SERVICE_0X14 == STD_ON) || (DCM_DSP_USE_SERVICE_0X04 == STD_ON)) || \
               (DCM_DEMFACADE_LOCK_MECHANISM_ENABLE == STD_ON)) */

#if (DCM_DSP_USE_SERVICE_0X02 == STD_ON)

/* !LINKSTO Dcm.Dsn.IB.DemFacade.ReadDataOfOBDFreezeFrame,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_ReadDataOfOBDFreezeFrame(
  uint8 PID,
  uint8 DataElementIndexOfPID,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) DestBuffer,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) BufSize
)
{
  Std_ReturnType ReadDataResult = DCM_E_NOT_OK;

  DBG_DCM_DSP_DEMFACADE_READDATAOFOBDFREEZEFRAME_ENTRY(
    PID, DataElementIndexOfPID, DestBuffer, BufSize);

  ReadDataResult = Dem_DcmReadDataOfOBDFreezeFrame(PID, DataElementIndexOfPID, DestBuffer, BufSize);

  DBG_DCM_DSP_DEMFACADE_READDATAOFOBDFREEZEFRAME_EXIT(
    ReadDataResult, PID, DataElementIndexOfPID, DestBuffer, BufSize);

  return ReadDataResult;
}

/* !LINKSTO Dcm.Dsn.IB.DemFacade.GetDTCOfOBDFreezeFrame,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_GetDTCOfOBDFreezeFrame(
  P2VAR(uint32, AUTOMATIC, DCM_VAR) DTC)
{
  Std_ReturnType GetFFResult = DCM_E_NOT_OK;

  DBG_DCM_DSP_DEMFACADE_GETDTCOFOBDFREEZEFRAME_ENTRY(DTC);

  GetFFResult = Dem_DcmGetDTCOfOBDFreezeFrame(0x00U, DTC, DEM_DTC_FORMAT_OBD);

  DBG_DCM_DSP_DEMFACADE_GETDTCOFOBDFREEZEFRAME_EXIT(GetFFResult, DTC);

  return GetFFResult;
}

#endif /*(DCM_DSP_USE_SERVICE_0X02 == STD_ON) */

#if ((DCM_DSP_USE_SERVICE_0X06 == STD_ON) && (DCM_OBDMID_SUPPORT_SWC == STD_OFF))
/* !LINKSTO Dcm.Dsn.IB.DemFacade.GetAvailableOBDMIDs,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_GetAvailableOBDMIDs(
  uint8 ObdMid,
  P2VAR(uint32, AUTOMATIC, DCM_VAR) pObdMidValue)
{
  Std_ReturnType ReturnValue = E_NOT_OK;

  DBG_DCM_DSP_DEMFACADE_GETAVAILABLEOBDMIDS_ENTRY(ObdMid, pObdMidValue);

  ReturnValue = Dem_DcmGetAvailableOBDMIDs(ObdMid, pObdMidValue);

  DBG_DCM_DSP_DEMFACADE_GETAVAILABLEOBDMIDS_EXIT(ReturnValue, ObdMid, pObdMidValue);

  return ReturnValue;
}

/* !LINKSTO Dcm.Dsn.IB.DemFacade.GetNumTIDsOfOBDMID,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_GetNumTIDsOfOBDMID(
  uint8 ObdMid,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pNumberOfTIDs)
{
  Std_ReturnType ReturnValue = E_NOT_OK;

  DBG_DCM_DSP_DEMFACADE_GETNUMTIDSOFOBDMID_ENTRY(ObdMid, pNumberOfTIDs);

  ReturnValue = Dem_DcmGetNumTIDsOfOBDMID(ObdMid, pNumberOfTIDs);

  DBG_DCM_DSP_DEMFACADE_GETNUMTIDSOFOBDMID_EXIT(ReturnValue, ObdMid, pNumberOfTIDs);

  return ReturnValue;
}

/* !LINKSTO Dcm.Dsn.IB.DemFacade.GetDTRData,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_GetDTRData(
  uint8 ObdMid,
  uint8 TIDIndex,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pTIDValue,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) pUaSID,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) pTestValue,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) pLowLimValue,
  P2VAR(uint16, AUTOMATIC, DCM_VAR) pUppLimValue)
{
  Std_ReturnType ReturnValue = E_NOT_OK;

  DBG_DCM_DSP_DEMFACADE_GETDTRDATA_ENTRY(ObdMid, TIDIndex, pTIDValue, pUaSID, pTestValue
                                        ,pLowLimValue, pUppLimValue);

  ReturnValue = Dem_DcmGetDTRData(ObdMid, TIDIndex
                                  ,pTIDValue, pUaSID, pTestValue, pLowLimValue, pUppLimValue);

  DBG_DCM_DSP_DEMFACADE_GETDTRDATA_EXIT(ReturnValue, ObdMid, TIDIndex, pTIDValue, pUaSID,
                                        pTestValue, pLowLimValue, pUppLimValue);

  return ReturnValue;
}
#endif /* #if ((DCM_DSP_USE_SERVICE_0X06 == STD_ON) && (DCM_OBDMID_SUPPORT_SWC == STD_OFF)) */

#if (DCM_DSP_READDTCINFORMATION_GETDTCBYOCCURENCETIME == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.DemFacade.GetDTCInfoByOccurrence_Process,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_GetDTCInfoByOccurrence_Process(
  Dcm_DemClientIdType ClientId,
  Dem_DTCRequestType DTCRequest,
  Dcm_OpStatusType OpStatus,
  Dcm_MsgType RespBufPtr,
  P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) RespLenPtr,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) NrcPtr
)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  Dcm_NegativeResponseCodeType LocalNrc = *NrcPtr;
  Dcm_MsgLenType LocalResponseLength = 0U;

  TS_PARAM_UNUSED(ClientId);
  DBG_DCM_DSP_DEMFACADE_GETDTCINFOBYOCCURRENCE_PROCESS_ENTRY(ClientId, DTCRequest, OpStatus, RespBufPtr, RespLenPtr, NrcPtr);

  /* Deviation MISRAC2012-4 */
  switch (OpStatus)
  {
    /* Deviation MISRAC2012-1 */
    /* Deviation MISRA-2 */
    case DCM_INITIAL:
      {
        Std_ReturnType GetDTCByOccurenceTimeResult;
        Std_ReturnType GetDTCStatusAvailabilityResult;
        uint8 StatusAvailabilityMask = 0U;

        LocalResponseLength = 0U;

        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstTestFailedDTC.InitialDemAPI,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstConfirmedDTC.InitialDemAPI,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentTestFailedDTC.InitialDemAPI,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentConfirmedDTC.InitialDemAPI,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
        GetDTCStatusAvailabilityResult
          = Dem_GetDTCStatusAvailabilityMask(ClientId, &StatusAvailabilityMask);
        if (GetDTCStatusAvailabilityResult == E_OK)
        {
          uint32 DTC;

          /* Add the subfunction id to the response message which corresponds to the requested
           * DTC occurrence time type */
          if (DTCRequest == DEM_FIRST_FAILED_DTC)
          {
            RespBufPtr[0U] = 0x0BU;
          }
          else if(DTCRequest == DEM_FIRST_DET_CONFIRMED_DTC)
          {
            RespBufPtr[0U] = 0x0CU;
          }
          else if (DTCRequest == DEM_MOST_RECENT_FAILED_DTC)
          {
            RespBufPtr[0U] = 0x0DU;
          }
          else
          {
            /* DEM_MOST_REC_DET_CONFIRMED_DTC */
            RespBufPtr[0U] = 0x0EU;
          }

          /* Add the status availability mask to the response message */
          RespBufPtr[1U] = StatusAvailabilityMask;
          LocalResponseLength = 2U;

          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstTestFailedDTC.GetDTCStatusAvailabilityMask.E_OK,1*/
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstConfirmedDTC.GetDTCStatusAvailabilityMask.E_OK,1*/
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentTestFailedDTC.GetDTCStatusAvailabilityMask.E_OK,1*/
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentConfirmedDTC.GetDTCStatusAvailabilityMask.E_OK,1*/
          /* !LINKSTO Dcm.ReadDTCInfo.reportFirstTestFailedDTC.GetDTCByOccurrenceTime.Parameters,1*/
          /* !LINKSTO Dcm.ReadDTCInfo.reportFirstConfirmedDTC.GetDTCByOccurrenceTime.Parameters,1*/
          /* !LINKSTO Dcm.ReadDTCInfo.reportMostRecentTestFailedDTC.GetDTCByOccurrenceTime.Parameters,1*/
          /* !LINKSTO Dcm.ReadDTCInfo.reportMostRecentConfirmedDTC.GetDTCByOccurrenceTime.Parameters,1*/
          /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
          GetDTCByOccurenceTimeResult = Dem_GetDTCByOccurrenceTime(ClientId, DTCRequest, &DTC);
          if (GetDTCByOccurenceTimeResult == E_OK)
          {
            Std_ReturnType ResponseFromDemSelect = E_NOT_OK;
            /* Write the acquired DTC to the response buffer */
            RespBufPtr[2U] = (uint8)(DTC >> 16U);
            RespBufPtr[3U] = (uint8)(DTC >> 8U);
            RespBufPtr[4U] = (uint8)DTC;
            LocalResponseLength = 5U;
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstTestFailedDTC.GetDTCByOccurrenceTime.E_OK,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstConfirmedDTC.GetDTCByOccurrenceTime.E_OK,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentTestFailedDTC.GetDTCByOccurrenceTime.E_OK,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentConfirmedDTC.GetDTCByOccurrenceTime.E_OK,1*/
            /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1*/
            ResponseFromDemSelect = Dem_SelectDTC(ClientId,
                                                  DTC,
                                                  DEM_DTC_FORMAT_UDS,
                                                  DEM_DTC_ORIGIN_PRIMARY_MEMORY);
            if(E_OK != ResponseFromDemSelect)
            {
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstTestFailedDTC.SelectDTC.DEM_BUSY,1*/
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstConfirmedDTC.SelectDTC.DEM_BUSY,1*/
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentTestFailedDTC.SelectDTC.DEM_BUSY,1*/
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentConfirmedDTC.SelectDTC.DEM_BUSY,1*/
              LocalNrc = DCM_E_GENERALREJECT;
              ServiceProcessStatus = DCM_E_NOT_OK;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
              if(DEM_BUSY != ResponseFromDemSelect)
              {
                /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstTestFailedDTC.SelectDTC.UnsupportedError.NRC,1*/
                /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstTestFailedDTC.SelectDTC.UnsupportedError.DET,1*/
                /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstConfirmedDTC.SelectDTC.UnsupportedError.NRC,1*/
                /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstConfirmedDTC.SelectDTC.UnsupportedError.DET,1*/
                /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentTestFailedDTC.SelectDTC.UnsupportedError.NRC,1*/
                /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentTestFailedDTC.SelectDTC.UnsupportedError.DET,1*/
                /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentConfirmedDTC.SelectDTC.UnsupportedError.NRC,1*/
                /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentConfirmedDTC.SelectDTC.UnsupportedError.DET,1*/
                DCM_DET_REPORT_ERROR(DCM_SVCID_READDTCINFORMATION, DCM_E_INTERFACE_RETURN_VALUE);
              }
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
              break;
            }
          }
          else if(DEM_NO_SUCH_ELEMENT == GetDTCByOccurenceTimeResult)
          {
          /* !LINKSTO Dcm.ReadDTCInfo.reportFirstTestFailedDTC.GetDTCByOccurrenceTime.DEM_NO_SUCH_ELEMENT,1*/
          /* !LINKSTO Dcm.ReadDTCInfo.reportFirstConfirmedDTC.GetDTCByOccurrenceTime.DEM_NO_SUCH_ELEMENT,1*/
          /* !LINKSTO Dcm.ReadDTCInfo.reportMostRecentTestFailedDTC.GetDTCByOccurrenceTime.DEM_NO_SUCH_ELEMENT,1*/
          /* !LINKSTO Dcm.ReadDTCInfo.reportMostRecentConfirmedDTC.GetDTCByOccurrenceTime.DEM_NO_SUCH_ELEMENT,1*/
            ServiceProcessStatus = DCM_E_DONE;
            break;
          }
          else
          {
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstTestFailedDTC.GetDTCByOccurrenceTime.UnsupportedError.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstTestFailedDTC.GetDTCByOccurrenceTime.UnsupportedError.DET,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstConfirmedDTC.GetDTCByOccurrenceTime.UnsupportedError.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstConfirmedDTC.GetDTCByOccurrenceTime.UnsupportedError.DET,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentTestFailedDTC.GetDTCByOccurrenceTime.UnsupportedError.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentTestFailedDTC.GetDTCByOccurrenceTime.UnsupportedError.DET,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentConfirmedDTC.GetDTCByOccurrenceTime.UnsupportedError.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentConfirmedDTC.GetDTCByOccurrenceTime.UnsupportedError.DET,1*/

            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstTestFailedDTC.GetDTCByOccurrenceTime.E_NOT_OK.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstTestFailedDTC.GetDTCByOccurrenceTime.E_NOT_OK.DET,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstConfirmedDTC.GetDTCByOccurrenceTime.E_NOT_OK.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstConfirmedDTC.GetDTCByOccurrenceTime.E_NOT_OK.DET,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentTestFailedDTC.GetDTCByOccurrenceTime.E_NOT_OK.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentTestFailedDTC.GetDTCByOccurrenceTime.E_NOT_OK.DET,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentConfirmedDTC.GetDTCByOccurrenceTime.E_NOT_OK.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentConfirmedDTC.GetDTCByOccurrenceTime.E_NOT_OK.DET,1*/

            LocalNrc = DCM_E_GENERALREJECT;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
            DCM_DET_REPORT_ERROR(DCM_SVCID_READDTCINFORMATION, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
            ServiceProcessStatus = DCM_E_NOT_OK;
            break;
          }
        }
        else
        {
          if(GetDTCStatusAvailabilityResult != E_NOT_OK )
          {
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstTestFailedDTC.GetDTCStatusAvailabilityMask.UnsupportedError.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstTestFailedDTC.GetDTCStatusAvailabilityMask.UnsupportedError.DET,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstConfirmedDTC.GetDTCStatusAvailabilityMask.UnsupportedError.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstConfirmedDTC.GetDTCStatusAvailabilityMask.UnsupportedError.DET,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentTestFailedDTC.GetDTCStatusAvailabilityMask.UnsupportedError.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentTestFailedDTC.GetDTCStatusAvailabilityMask.UnsupportedError.DET,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentConfirmedDTC.GetDTCStatusAvailabilityMask.UnsupportedError.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentConfirmedDTC.GetDTCStatusAvailabilityMask.UnsupportedError.DET,1*/
            LocalNrc = DCM_E_GENERALREJECT;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
            DCM_DET_REPORT_ERROR(DCM_SVCID_READDTCINFORMATION,DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
          }
          else
          {
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstTestFailedDTC.GetDTCStatusAvailabilityMask.E_NOT_OK,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstConfirmedDTC.GetDTCStatusAvailabilityMask.E_NOT_OK,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentTestFailedDTC.GetDTCStatusAvailabilityMask.E_NOT_OK,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentConfirmedDTC.GetDTCStatusAvailabilityMask.E_NOT_OK,1*/
            LocalNrc = DCM_E_REQUESTOUTOFRANGE;
          }
          ServiceProcessStatus = E_NOT_OK;
          break;
        }
      }
    /* Fall through intended */

    case DCM_PENDING:
      {
        Std_ReturnType GetStatusOfDTCResult;
        uint8 DTCStatus;
        /* !LINKSTO Dcm.ReadDTCInfo.reportFirstTestFailedDTC.GetStatusOfDTC,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportFirstConfirmedDTC.GetStatusOfDTC,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportMostRecentTestFailedDTC.GetStatusOfDTC,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportMostRecentConfirmedDTC.GetStatusOfDTC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstTestFailedDTC.SelectDTC.E_OK,1*/
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstConfirmedDTC.SelectDTC.E_OK,1*/
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentTestFailedDTC.SelectDTC.E_OK,1*/
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentConfirmedDTC.SelectDTC.E_OK,1*/
        /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1*/
        GetStatusOfDTCResult = Dem_GetStatusOfDTC(ClientId, &DTCStatus);

        switch (GetStatusOfDTCResult)
        {
          /* Add the DTC status an the positive response message is ready to be sent */
          case E_OK:
            /* !LINKSTO Dcm.ReadDTCInfo.reportFirstTestFailedDTC.Response,1 */
            /* !LINKSTO Dcm.ReadDTCInfo.reportMostRecentTestFailedDTC.Response,1 */
            /* !LINKSTO Dcm.ReadDTCInfo.reportMostRecentConfirmedDTC.Response,1 */
            /* !LINKSTO Dcm.ReadDTCInfo.reportFirstConfirmedDTC.Response,1 */
            RespBufPtr[5U] = DTCStatus;
            LocalResponseLength = 6U;
            ServiceProcessStatus = DCM_E_DONE;
            break;

          case DEM_NO_SUCH_ELEMENT:
            LocalResponseLength = 2U;
            ServiceProcessStatus = DCM_E_DONE;
            break;

          /* Do nothing. Service will be called again next cycle */
          case DEM_PENDING:
            /* !LINKSTO Dcm.ReadDTCInfo.reportFirstTestFailedDTC.GetStatusOfDTC.DEM_PENDING,1*/
            /* !LINKSTO Dcm.ReadDTCInfo.reportFirstConfirmedDTC.GetStatusOfDTC.DEM_PENDING,1*/
            /* !LINKSTO Dcm.ReadDTCInfo.reportMostRecentTestFailedDTC.GetStatusOfDTC.DEM_PENDING,1*/
            /* !LINKSTO Dcm.ReadDTCInfo.reportMostRecentConfirmedDTC.GetStatusOfDTC.DEM_PENDING,1*/
            ServiceProcessStatus = DCM_E_PENDING;
            break;

          /* Deviation MISRAC2012-1 */
          /* Deviation MISRA-2 */
          case DEM_WRONG_DTC:
          /* Fall through intended */
          case DEM_WRONG_DTCORIGIN:
            /* !LINKSTO Dcm.ReadDTCInfo.reportFirstTestFailedDTC.GetStatusOfDTC.DEM_WRONG_DTC,1 */
            /* !LINKSTO Dcm.ReadDTCInfo.reportFirstConfirmedDTC.GetStatusOfDTC.DEM_WRONG_DTC,1 */
            /* !LINKSTO Dcm.ReadDTCInfo.reportMostRecentTestFailedDTC.GetStatusOfDTC.DEM_WRONG_DTC,1 */
            /* !LINKSTO Dcm.ReadDTCInfo.reportMostRecentConfirmedDTC.GetStatusOfDTC.DEM_WRONG_DTC,1 */
            /* !LINKSTO Dcm.ReadDTCInfo.reportFirstTestFailedDTC.GetStatusOfDTC.DEM_WRONG_DTCORIGIN,1 */
            /* !LINKSTO Dcm.ReadDTCInfo.reportFirstConfirmedDTC.GetStatusOfDTC.DEM_WRONG_DTCORIGIN,1 */
            /* !LINKSTO Dcm.ReadDTCInfo.reportMostRecentTestFailedDTC.GetStatusOfDTC.DEM_WRONG_DTCORIGIN,1 */
            /* !LINKSTO Dcm.ReadDTCInfo.reportMostRecentConfirmedDTC.GetStatusOfDTC.DEM_WRONG_DTCORIGIN,1 */
            LocalNrc = DCM_E_REQUESTOUTOFRANGE;
            ServiceProcessStatus = E_NOT_OK;
            break;

          case E_NOT_OK:
          case DEM_BUSY:
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstTestFailedDTC.GetStatusOfDTC.E_NOT_OK.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstTestFailedDTC.GetStatusOfDTC.E_NOT_OK.DET,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstConfirmedDTC.GetStatusOfDTC.E_NOT_OK.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstConfirmedDTC.GetStatusOfDTC.E_NOT_OK.DET,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentTestFailedDTC.GetStatusOfDTC.E_NOT_OK.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentTestFailedDTC.GetStatusOfDTC.E_NOT_OK.DET,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentConfirmedDTC.GetStatusOfDTC.E_NOT_OK.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentConfirmedDTC.GetStatusOfDTC.E_NOT_OK.DET,1*/

            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstTestFailedDTC.GetStatusOfDTC.DEM_BUSY.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstTestFailedDTC.GetStatusOfDTC.DEM_BUSY.DET,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstConfirmedDTC.GetStatusOfDTC.DEM_BUSY.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstConfirmedDTC.GetStatusOfDTC.DEM_BUSY.DET,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentTestFailedDTC.GetStatusOfDTC.DEM_BUSY.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentTestFailedDTC.GetStatusOfDTC.DEM_BUSY.DET,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentConfirmedDTC.GetStatusOfDTC.DEM_BUSY.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentConfirmedDTC.GetStatusOfDTC.DEM_BUSY.DET,1*/
            LocalNrc = DCM_E_GENERALREJECT;
            ServiceProcessStatus = E_NOT_OK;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
            DCM_DET_REPORT_ERROR(DCM_SVCID_READDTCINFORMATION, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
            break;

          default:
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstTestFailedDTC.GetStatusOfDTC.UnsupportedError.DET,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstConfirmedDTC.GetStatusOfDTC.UnsupportedError.DET,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentTestFailedDTC.GetStatusOfDTC.UnsupportedError.DET,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentConfirmedDTC.GetStatusOfDTC.UnsupportedError.DET,1*/
#if (DCM_DEV_ERROR_DETECT == STD_ON)
            DCM_DET_REPORT_ERROR(DCM_SVCID_READDTCINFORMATION, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstTestFailedDTC.GetStatusOfDTC.UnsupportedError.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportFirstConfirmedDTC.GetStatusOfDTC.UnsupportedError.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentTestFailedDTC.GetStatusOfDTC.UnsupportedError.NRC,1*/
            /* !LINKSTO Dcm.EB.ReadDTCInfo.reportMostRecentConfirmedDTC.GetStatusOfDTC.UnsupportedError.NRC,1*/
            ServiceProcessStatus = E_NOT_OK;
            break;
        }
      }
      break;

    /* Deviation MISRAC2012-1 */
    /* Deviation MISRA-2 */
    case DCM_CANCEL:
    /* !LINKSTO Dcm.EB.DemIF.RequestCancellation, 1 */
    /* Fall through intended */

    default:
      ServiceProcessStatus = DCM_E_DONE;
      break;
  }

  *RespLenPtr =  LocalResponseLength;
  *NrcPtr = LocalNrc;

  DBG_DCM_DSP_DEMFACADE_GETDTCINFOBYOCCURRENCE_PROCESS_EXIT(ClientId, DTCRequest, OpStatus, RespBufPtr
                                                            ,RespLenPtr, NrcPtr, ServiceProcessStatus);

  return ServiceProcessStatus;
}
#endif /* #if (DCM_DSP_READDTCINFORMATION_GETDTCBYOCCURENCETIME == STD_ON) */

#if (DCM_DSP_USE_SERVICE_0X85 == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.DemFacade.ControlDTCSetting.Enable,1 */
FUNC(Dcm_ReturnControlDTCSettingType, DCM_CODE) Dcm_Dsp_DemFacade_ControlDTCSetting_Enable(
  const Dcm_DemClientIdType DemClientId)
{
  Std_ReturnType DemDTCSettingStatus = E_NOT_OK;
  Dcm_ReturnControlDTCSettingType DcmDTCSettingStatus = DCM_CONTROL_DTC_UNKNOWN_VALUE;

  DBG_DCM_DSP_DEMFACADE_CONTROLDTCSETTING_ENABLE_ENTRY(DemClientId);

  /* Enables the DTC setting for the DTC group */
  /* !LINKSTO Dcm.ControlDTCSetting.DcmDslProtocolRow.DcmDemClientRef,1 */
  DemDTCSettingStatus = Dem_EnableDTCSetting(DemClientId);

  if (DemDTCSettingStatus == E_OK)
  {
    DcmDTCSettingStatus = DCM_CONTROL_DTC_SETTING_OK;
  }
  else if (DemDTCSettingStatus == DEM_PENDING)
  {
    /* !LINKSTO Dcm.EnableDTCSetting.DEM_PENDING,1 */
    DcmDTCSettingStatus = DCM_CONTROL_DTC_SETTING_PENDING;
  }
  else
  {
    /* !LINKSTO Dcm.EnableDTCSetting.UnsupportedError.NRC,1 */
    /* !LINKSTO Dcm.EnableDTCSetting.UnsupportedError.DET,1 */
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    DCM_DET_REPORT_ERROR(DCM_SVCID_CONTROLDTCSETTING, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  }

  DBG_DCM_DSP_DEMFACADE_CONTROLDTCSETTING_ENABLE_EXIT(DcmDTCSettingStatus);

  return DcmDTCSettingStatus;
}

/* !LINKSTO Dcm.Dsn.IB.DemFacade.ControlDTCSetting.Disable,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_ControlDTCSetting_Disable(
  const Dcm_DemClientIdType DemClientId)
{
  Std_ReturnType DemDTCSettingStatus = E_NOT_OK;
  Dcm_ReturnControlDTCSettingType DcmDTCSettingStatus = DCM_CONTROL_DTC_UNKNOWN_VALUE;

  DBG_DCM_DSP_DEMFACADE_CONTROLDTCSETTING_DISABLE_ENTRY(DemClientId);

  /* Disable the DTC setting for the DTC group */
  /* !LINKSTO Dcm.ControlDTCSetting.DcmDslProtocolRow.DcmDemClientRef,1 */
  DemDTCSettingStatus = Dem_DisableDTCSetting(DemClientId);

  if (DemDTCSettingStatus == E_OK)
  {
    DcmDTCSettingStatus = DCM_CONTROL_DTC_SETTING_OK;
  }
  else if (DemDTCSettingStatus == DEM_PENDING)
  {
    /* !LINKSTO Dcm.DisableDTCSetting.DEM_PENDING,1 */
    DcmDTCSettingStatus = DCM_CONTROL_DTC_SETTING_PENDING;
  }
  else
  {
    /* !LINKSTO Dcm.DisableDTCSetting.UnsupportedError.NRC,1 */
    /* !LINKSTO Dcm.DisableDTCSetting.UnsupportedError.DET,1 */
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    DCM_DET_REPORT_ERROR(DCM_SVCID_CONTROLDTCSETTING, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  }

  DBG_DCM_DSP_DEMFACADE_CONTROLDTCSETTING_DISABLE_EXIT(DcmDTCSettingStatus);

  return DcmDTCSettingStatus;
}

#endif /* #if (DCM_DSP_USE_SERVICE_0X85 == STD_ON) */

#if (DCM_DSP_USE_SERVICE_0X14 == STD_ON) || (DCM_DSP_USE_SERVICE_0X04 == STD_ON)
/* !LINKSTO Dcm.Dsn.IB.DemFacade.ClearDTCInfo_Setup,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_ClearDTCInfo_Setup(
  Dcm_DemClientIdType DemClientId, uint32 DTC, uint8 DTCFormat, uint16 DTCOrigin,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) NrcPtr
)
{
  Std_ReturnType resultSelect = E_NOT_OK;
  Dcm_NegativeResponseCodeType LocalNrc = E_NOT_OK;
  P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr;

  DBG_DCM_DSP_DEMFACADE_CLEARDTCINFO_SETUP_ENTRY(DemClientId, DTC, DTCFormat, DTCOrigin, NrcPtr);

  /* Get Client context data and set DTC format */
  ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);
  ClientContextPtr->DTCFormat = DTCFormat;

  /* !LINKSTO SWS_Dcm_00004,1 */
  /* !LINKSTO SWS_Dcm_01263,1 */
  /* !LINKSTO Dcm.ClearDiagnosticInformation.DcmDslProtocolRow.DcmDemClientRef,1 */
  /* !LINKSTO Dcm.OBD.0x04Svc.DcmDslProtocolRow.DcmDemClientRef,1 */
  resultSelect = Dem_SelectDTC(DemClientId, DTC, DTCFormat, DTCOrigin);

  switch(resultSelect)
  {
    case E_OK:
      /* Set ClearDTCInfoState, determine next function call */
      if (DTCFormat == DEM_DTC_FORMAT_UDS)
      {
        /* The next function to be called is Dem_GetDTCSelectionResultForClearDTC() */
        /* !LINKSTO SWS_Dcm_01400,1 */
        ClientContextPtr->ClearDTCInfoState = STATE_CLEARDTCINFO_GETDTCSELECTIONRESULTFORCLEARDTC;
      }
      else
      {
        /* The next function to be called is Dem_ClearDTC() */
        /* !LINKSTO SWS_Dcm_01401,1 */
        /* !LINKSTO SWS_Dcm_00005,1 */
        ClientContextPtr->ClearDTCInfoState = STATE_CLEARDTCINFO_CLEARDTC;
      }
      LocalNrc = E_OK;
      break;

    case DEM_BUSY:
      /* Dem is currently processing the previous operation */
      /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.SelectDTC.DEM_BUSY.NRC,1 */
      LocalNrc = DCM_E_GENERALREJECT;
      break;

    /* CHECK: NOPARSE */
    default:
    {
      /* Defensive branch to catch all invalid return values */
      /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.SelectDTC.UnsupportedError.NRC,1 */
      LocalNrc = DCM_E_GENERALREJECT;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.SelectDTC.UnsupportedError.DET,1 */
      /* !LINKSTO Dcm.EB.ClearResetEmissionRelatedDiagInfo.SelectDTC.UnsupportedError.DET,1 */
      DCM_DET_REPORT_ERROR(DCM_SVCID_CLEARDTCINFOSETUP, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
      break;
    }
    /* CHECK: PARSE */
  }

   *NrcPtr = LocalNrc;

  DBG_DCM_DSP_DEMFACADE_CLEARDTCINFO_SETUP_EXIT(LocalNrc);

  return DCM_E_DONE;
}
/* !LINKSTO Dcm.Dsn.IB.DemFacade.ClearDTCInfo_Process,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_ClearDTCInfo_Process(
  Dcm_DemClientIdType DemClientId,
  P2VAR(Dcm_NegativeResponseCodeType, AUTOMATIC, DCM_VAR) NrcPtr)
{
  Std_ReturnType resultDTC = E_NOT_OK;
  Dcm_NegativeResponseCodeType LocalNrc = E_NOT_OK;
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr;

  DBG_DCM_DSP_DEMFACADE_CLEARDTCINFO_PROCESS_ENTRY(DemClientId, NrcPtr);

  /* Get Client context data */
  ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);

  /* Deviation MISRAC2012-4 */
  switch (ClientContextPtr->ClearDTCInfoState)
  {
    /* Deviation MISRAC2012-1 */
    /* Deviation MISRA-2 */
    case STATE_CLEARDTCINFO_GETDTCSELECTIONRESULTFORCLEARDTC:
      /* !LINKSTO SWS_Dcm_01400,1 */
      /* !LINKSTO Dcm.ClearDiagnosticInformation.DcmDslProtocolRow.DcmDemClientRef,1 */
      resultDTC = Dem_GetDTCSelectionResultForClearDTC(DemClientId);
      if (resultDTC != E_OK)
        {
          break;
        }
      ClientContextPtr->ClearDTCInfoState = STATE_CLEARDTCINFO_CLEARDTC;
      /* no break */

    case STATE_CLEARDTCINFO_CLEARDTC:
      /* !LINKSTO SWS_Dcm_01401,1 */
      /* !LINKSTO SWS_Dcm_00005,1 */
      /* !LINKSTO Dcm.ClearDiagnosticInformation.DcmDslProtocolRow.DcmDemClientRef,1 */
      /* !LINKSTO Dcm.OBD.0x04Svc.DcmDslProtocolRow.DcmDemClientRef,1 */
      resultDTC = Dem_ClearDTC(DemClientId);
      break;

    /* CHECK: NOPARSE */
    default:
    {
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_CLEARDTCINFOPROCESS);
      break;
    }
    /* CHECK: PARSE */
  }

  /* NRC mapping */
  switch (resultDTC)
  {
    case E_OK:
      /* !LINKSTO SWS_Dcm_00413,1 */
      /* !LINKSTO SWS_Dcm_00705,1 */
      LocalNrc = E_OK;
      serviceProcessStatus = DCM_E_DONE;
      break;

    case E_NOT_OK:
      /* Select function not called */
      /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.GetDTCSelectionResultForClearDTC.E_NOT_OK.NRC,1 */
      /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.ClearDTC.E_NOT_OK.NRC,1 */
      LocalNrc = DCM_E_GENERALREJECT;
      serviceProcessStatus = DCM_E_DONE;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.GetDTCSelectionResultForClearDTC.E_NOT_OK.DET,1*/
      /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.ClearDTC.E_NOT_OK.DET,1*/
      /* !LINKSTO Dcm.EB.ClearResetEmissionRelatedDiagInfo.ClearDTC.E_NOT_OK.DET,1*/
      DCM_DET_REPORT_ERROR(DCM_SVCID_CLEARDTCINFOPROCESS, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
      break;

    case DEM_PENDING:
      /* DTC clearance pending */
      /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.GetDTCSelectionResultForClearDTC.DEM_PENDING,1 */
      /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.ClearDTC.DEM_PENDING,1 */
      /* !LINKSTO SWS_Dcm_00703,1 */
      serviceProcessStatus = DCM_E_PENDING;
      break;

    case DEM_BUSY:
      /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.GetDTCSelectionResultForClearDTC.DEM_BUSY.NRC,1 */
      /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.ClearDTC.DEM_BUSY.NRC,1 */
      LocalNrc = DCM_E_GENERALREJECT;
      serviceProcessStatus = DCM_E_DONE;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.GetDTCSelectionResultForClearDTC.DEM_BUSY.DET,1 */
        /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.ClearDTC.DEM_BUSY.DET,1 */
        /* !LINKSTO Dcm.EB.ClearResetEmissionRelatedDiagInfo.ClearDTC.DEM_BUSY.DET,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_CLEARDTCINFOPROCESS, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
      break;

    case DEM_WRONG_DTC:
    case DEM_WRONG_DTCORIGIN:
      /* !LINKSTO SWS_Dcm_00708,1 */
      /* !LINKSTO SWS_Dcm_01265,1 */
      /* !LINKSTO SWS_Dcm_01408,1 */
      /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.GetDTCSelectionResultForClearDTC.DEM_WRONG_DTCORIGIN.NRC,1 */
      LocalNrc = DCM_E_REQUESTOUTOFRANGE;
      serviceProcessStatus = DCM_E_DONE;
      break;

    case DEM_CLEAR_BUSY:
    case DEM_CLEAR_FAILED:
      /* !LINKSTO SWS_Dcm_00704,1 */
      /* !LINKSTO SWS_Dcm_00967,1 */
      /* !LINKSTO SWS_Dcm_00707,1 */
      /* !LINKSTO SWS_Dcm_00966,1 */
      if (ClientContextPtr->ClearDTCInfoState == STATE_CLEARDTCINFO_CLEARDTC)
      {
        LocalNrc = DCM_E_CONDITIONSNOTCORRECT;
      }
      else
      {
        /* Invalid return values */
        /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.GetDTCSelectionResultForClearDTC.UnsupportedError.NRC,1*/
        LocalNrc = DCM_E_GENERALREJECT;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.GetDTCSelectionResultForClearDTC.UnsupportedError.DET,1*/
        DCM_DET_REPORT_ERROR(DCM_SVCID_CLEARDTCINFOPROCESS, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
      }
      serviceProcessStatus = DCM_E_DONE;
      break;

    case DEM_CLEAR_MEMORY_ERROR:
      if (ClientContextPtr->ClearDTCInfoState == STATE_CLEARDTCINFO_CLEARDTC)
      {
        if (ClientContextPtr->DTCFormat == DEM_DTC_FORMAT_UDS)
        {
          /* !LINKSTO SWS_Dcm_01060,1 */
          LocalNrc = DCM_E_GENERALPROGRAMMINGFAILURE;
        }
        else
        {
          /* !LINKSTO SWS_Dcm_01067,1 */
          LocalNrc = DCM_E_CONDITIONSNOTCORRECT;
        }
      }
      else
      {
        /* Invalid return values */
        /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.GetDTCSelectionResultForClearDTC.UnsupportedError.NRC,1*/
        LocalNrc = DCM_E_GENERALREJECT;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.GetDTCSelectionResultForClearDTC.UnsupportedError.DET,1*/
        DCM_DET_REPORT_ERROR(DCM_SVCID_CLEARDTCINFOPROCESS, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
      }
      serviceProcessStatus = DCM_E_DONE;
      break;

    default:
    {
      /* Invalid return values */
      /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.GetDTCSelectionResultForClearDTC.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.ClearDTC.UnsupportedError.NRC,1 */
      LocalNrc = DCM_E_GENERALREJECT;
      serviceProcessStatus = DCM_E_DONE;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.GetDTCSelectionResultForClearDTC.UnsupportedError.DET,1*/
      /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.ClearDTC.UnsupportedError.DET,1 */
      /* !LINKSTO Dcm.EB.ClearResetEmissionRelatedDiagInfo.ClearDTC.UnsupportedError.DET,1*/
      DCM_DET_REPORT_ERROR(DCM_SVCID_CLEARDTCINFOPROCESS, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
      break;
    }
  }
  *NrcPtr = LocalNrc;

  DBG_DCM_DSP_DEMFACADE_CLEARDTCINFO_PROCESS_EXIT(serviceProcessStatus, LocalNrc);

  return serviceProcessStatus;
}

/* !LINKSTO Dcm.Dsn.IB.DemFacade.ClearDTCInfo_Cancel,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_DemFacade_ClearDTCInfo_Cancel(
  Dcm_DemClientIdType DemClientId)
{

  DBG_DCM_DSP_DEMFACADE_CLEARDTCINFO_CANCEL_ENTRY(DemClientId);

  /* Call Dem_SelectDTC() to inform Dem that operation is canceled */
  /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.Canceling,1 */
  /* !LINKSTO Dcm.EB.ClearResetEmissionRelatedDiagInfo.Canceling,1 */
  /* !LINKSTO Dcm.ClearDiagnosticInformation.DcmDslProtocolRow.DcmDemClientRef,1 */
  /* !LINKSTO Dcm.OBD.0x04Svc.DcmDslProtocolRow.DcmDemClientRef,1 */
  (void)Dem_SelectDTC(DemClientId, DEM_DTC_GROUP_ALL_DTCS, DEM_DTC_FORMAT_UDS, DEM_DTC_ORIGIN_PRIMARY_MEMORY);

  DBG_DCM_DSP_DEMFACADE_CLEARDTCINFO_CANCEL_EXIT();

  return DCM_E_DONE;
}

#endif /* #if (DCM_DSP_USE_SERVICE_0X14 == STD_ON) || (DCM_DSP_USE_SERVICE_0X04 == STD_ON) */

/*==================[internal function definitions]==============================================*/

#if ((DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON))

/** \brief _Setup function for obtaining DTC status from DEM */
STATIC FUNC(void, DCM_CODE) Dcm_DemFacade_GetDTCStatus_Setup
(
    P2VAR(Dcm_GetDTCStatusContextType, AUTOMATIC, DCM_VAR) GetDTCStatusContext,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) GetDTCStatusContextResponseBuffer,
    Dcm_MsgLenType AvailableBufferSize
)
{
  /* Gather necessary data from the request for this data unit */
    GetDTCStatusContext->ResponseBuffer = GetDTCStatusContextResponseBuffer;
    GetDTCStatusContext->BufferSize = AvailableBufferSize;
    /* Set-up relevant data for program flow control */
    GetDTCStatusContext->StatusOfDTC = DCM_NOT_OBTAINED;
    return;
}

/** \brief _Execute function for obtaining DTC Status from DEM */
/* !LINKSTO Dcm.Dsn.IB.DemFacade.GetDTCStatus,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_GetDTCStatus_Execute
(
  const Dcm_DemClientIdType ClientId,
  P2VAR(Dcm_GetDTCStatusContextType, AUTOMATIC, DCM_VAR) GetDTCStatusContext,
#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
  uint8 RequestedService,
#endif /* #if (DCM_DSP_USE_SERVICE_0XAF == STD_ON) */
  uint8 RequestedSubService
)
{
  Std_ReturnType ResultCodeFromExecute = DCM_FACADE_NOT_OK;
  Std_ReturnType ResultCodeFromDEM = E_NOT_OK;

  if(GetDTCStatusContext->StatusOfDTC == DCM_NOT_OBTAINED)
  {
      /* Execute access to DEM interface */
      /* !LINKSTO Dcm.ReadDTCInfo.reportSeverityInformationOfDTC.GetStatusOfDTC.Parameters,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.InitialDemAPI,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.SelectDTC.E_OK,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.SelectDTC.E_OK,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.SelectDTC.E_OK,1*/
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.SelectDTC.E_OK,1*/
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SelectDTC.E_OK,1*/
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SelectDTC.E_OK,1*/
      /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1*/
      /* !LINKSTO Dcm.ReadGenericInfo.DcmDslProtocolRow.DcmDemClientRef,1*/
      ResultCodeFromDEM = Dem_GetStatusOfDTC(ClientId,
                                           &GetDTCStatusContext->ResponseBuffer[0]);

    if(ResultCodeFromDEM == E_OK)
    {
      /* Update Information with regards of the status of the Set-up Data */
      GetDTCStatusContext->StatusOfDTC = DCM_OBTAINED;
      ResultCodeFromExecute = DCM_FACADE_OK;
#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
      Dcm_ReadEventDataServicesContext.HeaderData[3] =
          GetDTCStatusContext->ResponseBuffer[0];
#endif /* #if (DCM_DSP_USE_SERVICE_0XAF == STD_ON) */
    }
    /* !LINKSTO Dcm.ReadDTCInfo.reportSeverityInformationOfDTC.GetStatusOfDTC.PENDING,1 */
    else if(ResultCodeFromDEM == DEM_PENDING)
    {
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetStatusOfDTC.DEM_PENDING,1*/
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetStatusOfDTC.DEM_PENDING,1*/
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.DEM_PENDING,1*/
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.DEM_PENDING,1*/
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetStatusOfDTC.DEM_PENDING,1*/
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetStatusOfDTC.DEM_PENDING,1*/
      ResultCodeFromExecute = DCM_FACADE_PENDING;
    }
    else if((ResultCodeFromDEM == DEM_WRONG_DTC)||
            (ResultCodeFromDEM == DEM_WRONG_DTCORIGIN))
    {
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetStatusOfDTC.DEM_WRONG_DTC,1*/
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetStatusOfDTC.DEM_WRONG_DTCORIGIN,1*/
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetStatusOfDTC.DEM_WRONG_DTC,1*/
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetStatusOfDTC.DEM_WRONG_DTCORIGIN,1*/
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.DEM_WRONG_DTC,1*/
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.DEM_WRONG_DTCORIGIN,1*/
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.DEM_WRONG_DTC,1*/
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.DEM_WRONG_DTCORIGIN,1*/
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetStatusOfDTC.DEM_WRONG_DTC,1*/
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetStatusOfDTC.DEM_WRONG_DTCORIGIN,1*/
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetStatusOfDTC.DEM_WRONG_DTC,1*/
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetStatusOfDTC.DEM_WRONG_DTCORIGIN,1*/
      /* !LINKSTO Dcm.ReadDTCInfo.reportSeverityInformationOfDTC.GetStatusOfDTC.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportSeverityInformationOfDTC.GetStatusOfDTC.DEM_WRONG_DTCORIGIN,1 */
      ResultCodeFromExecute = DCM_FACADE_REQUESTOUTOFRANGE;
    }
    else if(ResultCodeFromDEM == DEM_NO_SUCH_ELEMENT)
    {
      /* Specific handling for sub-service 0x09. */
      if(RequestedSubService == DCM_DSP_READDTCINFORMATION_0x09_ID)
      {
        /* !LINKSTO Dcm.ReadDTCInfo.reportSeverityInformationOfDTC.GetStatusOfDTC.DEM_NO_SUCH_ELEMENT,1 */
        ResultCodeFromExecute = DCM_FACADE_NO_SUCH_ELEMENT;
      }
      else
      {
#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
        if(DCM_DSP_SERVICE_0XAF == RequestedService)
        {
          /* !LINKSTO Dcm.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetStatusOfDTC.DEM_NO_SUCH_ELEMENT,1*/
          /* !LINKSTO Dcm.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetStatusOfDTC.DEM_NO_SUCH_ELEMENT,1*/
          ResultCodeFromExecute = DCM_FACADE_NOT_OK;
        }
        else
#endif /* #if (DCM_DSP_USE_SERVICE_0XAF == STD_ON) */
        {
          /* !LINKSTO Dcm.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetStatusOfDTC.DEM_NO_SUCH_ELEMENT,1*/
          /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetStatusOfDTC.DEM_NO_SUCH_ELEMENT,1*/
          /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.DEM_NO_SUCH_ELEMENT,1*/
          /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.DEM_NO_SUCH_ELEMENT,1*/
          /* !LINKSTO Dcm.ReadDTCInfo.reportFirstTestFailedDTC.GetStatusOfDTC.DEM_NO_SUCH_ELEMENT,1*/
          /* !LINKSTO Dcm.ReadDTCInfo.reportFirstConfirmedDTC.GetStatusOfDTC.DEM_NO_SUCH_ELEMENT,1*/
          /* !LINKSTO Dcm.ReadDTCInfo.reportMostRecentTestFailedDTC.GetStatusOfDTC.DEM_NO_SUCH_ELEMENT,1*/
          /* !LINKSTO Dcm.ReadDTCInfo.reportMostRecentConfirmedDTC.GetStatusOfDTC.DEM_NO_SUCH_ELEMENT,1*/
          ResultCodeFromExecute = DCM_FACADE_OK;
        }
      }
    }
    else
    {
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetStatusOfDTC.E_NOT_OK.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetStatusOfDTC.E_NOT_OK.DET,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetStatusOfDTC.E_NOT_OK.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetStatusOfDTC.E_NOT_OK.DET,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.E_NOT_OK.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.E_NOT_OK.DET,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.E_NOT_OK.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.E_NOT_OK.DET,1*/
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetStatusOfDTC.E_NOT_OK.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetStatusOfDTC.E_NOT_OK.DET,1*/
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetStatusOfDTC.E_NOT_OK.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetStatusOfDTC.E_NOT_OK.DET,1*/

      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetStatusOfDTC.DEM_BUSY.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetStatusOfDTC.DEM_BUSY.DET,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetStatusOfDTC.DEM_BUSY.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetStatusOfDTC.DEM_BUSY.DET,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.DEM_BUSY.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.DEM_BUSY.DET,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.DEM_BUSY.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.DEM_BUSY.DET,1*/
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetStatusOfDTC.DEM_BUSY.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetStatusOfDTC.DEM_BUSY.DET,1*/
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetStatusOfDTC.DEM_BUSY.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetStatusOfDTC.DEM_BUSY.DET,1*/

      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetStatusOfDTC.UnsupportedError.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetStatusOfDTC.UnsupportedError.DET,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetStatusOfDTC.UnsupportedError.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetStatusOfDTC.UnsupportedError.DET,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.UnsupportedError.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.UnsupportedError.DET,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.UnsupportedError.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.UnsupportedError.DET,1*/
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetStatusOfDTC.UnsupportedError.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetStatusOfDTC.UnsupportedError.DET,1*/
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetStatusOfDTC.UnsupportedError.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetStatusOfDTC.UnsupportedError.DET,1*/

      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetStatusOfDTC.E_NOT_OK.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetStatusOfDTC.DEM_BUSY.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetStatusOfDTC.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetStatusOfDTC.E_NOT_OK.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetStatusOfDTC.DEM_BUSY.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetStatusOfDTC.UnsupportedError.DET,1 */

      ResultCodeFromExecute = DCM_FACADE_NOT_OK;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      DCM_DET_REPORT_ERROR(DCM_SVCID_READDTCINFORMATION, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
    }
  }
  else
  {
    ResultCodeFromExecute = DCM_FACADE_OK;
  }
  return ResultCodeFromExecute;
}

#endif /* #if ((DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON)) */

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON)
/** \brief _Setup function for reportSeverityInformationOfDTC sub-service */
STATIC FUNC(void, DCM_CODE) Dcm_DemFacade_SeverityInformation_Setup
(
    P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) ReadSeverityInformationContextResponseBuffer,
    Dcm_MsgLenType AvailableBufferSize,
    uint8 DTCFormat,
    uint32 DTCMaskSetup,
    P2VAR(Dcm_GetDTCSeverityContextType, AUTOMATIC, DCM_VAR) GetDTCSeverityContext
)
{
  /* Gather necessary data from the request for this data unit */
  DiagnosticDataContext->DTCMask = DTCMaskSetup;
  DiagnosticDataContext->ResponseBuffer = ReadSeverityInformationContextResponseBuffer;
  DiagnosticDataContext->BufferSize = AvailableBufferSize;
  /*Set-up relevant Data for program flow control */
  DiagnosticDataContext->AmountOfDataToWriteOnResponse = 0U;
  DiagnosticDataContext->AmountOfDataStoredOnCompleteResponse = 0U;
  DiagnosticDataContext->AmountOfDataStoredOnResponseOnCurrentRun = 0U;
  DiagnosticDataContext->DTCFormat = DTCFormat;
  DiagnosticDataContext->NextPossitionToFillOnResponseBuffer = 0U;
  GetDTCSeverityContext->SeverityOfDTC = DCM_NOT_OBTAINED;
  GetDTCSeverityContext->FunctionalUnitOfDTC = DCM_NOT_OBTAINED;
  return;
}

/** \brief _Execute function for obtaining the Severity of a DTC from DEM */
/* !LINKSTO Dcm.Dsn.IB.DemFacade.GetSeverityOfDTC, 1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_GetSeverityOfDTC_Execute
(
  const Dcm_DemClientIdType ClientId,
  P2VAR(Dcm_GetDTCSeverityContextType, AUTOMATIC, DCM_VAR) GetDTCSeverityContext,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) DTCSeverity
)
{
  Std_ReturnType ResultCodeFromExecute = DCM_FACADE_NOT_OK;
  Std_ReturnType ResultCodeFromDEM = E_NOT_OK;

  if(GetDTCSeverityContext->SeverityOfDTC != DCM_OBTAINED)
  {
    /* Call Dem_GetSeverityOfDTC API. */
    /* !LINKSTO Dcm.ReadDTCInfo.reportSeverityInformationOfDTC.GetSeverityOfDTC.Parameters,1 */
    /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
    ResultCodeFromDEM = Dem_GetSeverityOfDTC(ClientId,
                                             DTCSeverity);
    /* Evaluate return code. */
    /* !LINKSTO SWS_Dcm_00381,1 */
    if(ResultCodeFromDEM == E_OK)
    {
      ResultCodeFromExecute = DCM_FACADE_OK;
      GetDTCSeverityContext->SeverityOfDTC = DCM_OBTAINED;
    }
    /* !LINKSTO Dcm.ReadDTCInfo.reportSeverityInformationOfDTC.GetSeverityOfDTC.PENDING,1 */
    else if(ResultCodeFromDEM == DEM_PENDING)
    {
     ResultCodeFromExecute = DCM_FACADE_PENDING;
    }
    /* !LINKSTO Dcm.ReadDTCInfo.reportSeverityInformationOfDTC.GetSeverityOfDTC.DEM_WRONG_DTC,1 */
    /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetSeverityOfDTC.DEM_WRONG_DTCORIGIN,1 */
    else if((ResultCodeFromDEM == DEM_WRONG_DTC)||
            (ResultCodeFromDEM == DEM_WRONG_DTCORIGIN))
    {
      ResultCodeFromExecute = DCM_FACADE_REQUESTOUTOFRANGE;
    }
    else
    {
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetSeverityOfDTC.E_NOT_OK.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetSeverityOfDTC.DEM_BUSY.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetSeverityOfDTC.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetSeverityOfDTC.E_NOT_OK.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetSeverityOfDTC.UnsupportedError.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetSeverityOfDTC.DEM_BUSY.DET, 1 */
      ResultCodeFromExecute = DCM_FACADE_INVALID_VALUE;
    }
  }
  else
  {
    /* Value already obtained. */
    ResultCodeFromExecute = DCM_FACADE_OK;
  }

  return ResultCodeFromExecute;
}

/** \brief _Execute function for obtaining the Functional Unit Of DTC of a DTC from DEM */
/* !LINKSTO Dcm.Dsn.IB.DemFacade.GetFunctionalUnitOfDTC,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_GetFunctionalUnitOfDTC_Execute
(
  const Dcm_DemClientIdType ClientId,
  P2VAR(Dcm_GetDTCSeverityContextType, AUTOMATIC, DCM_VAR) GetDTCSeverityContext,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) FunctionalUnitOfDTC
)
{
  Std_ReturnType ResultCodeFromExecute = DCM_FACADE_NOT_OK;
  Std_ReturnType ResultCodeFromDEM = E_NOT_OK;

  if(GetDTCSeverityContext->FunctionalUnitOfDTC != DCM_OBTAINED)
  {
    /* Call Dem_GetFunctionalUnitOfDTC API. */
    /* !LINKSTO Dcm.ReadDTCInfo.reportSeverityInformationOfDTC.GetFunctionalUnitOfDTC.Parameters,1 */
    /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
    ResultCodeFromDEM = Dem_GetFunctionalUnitOfDTC(ClientId,
                                             FunctionalUnitOfDTC);
    /* Evaluate return code. */
    if(ResultCodeFromDEM == E_OK)
    {
      ResultCodeFromExecute = DCM_FACADE_OK;
      GetDTCSeverityContext->FunctionalUnitOfDTC = DCM_OBTAINED;
    }
    /* !LINKSTO Dcm.ReadDTCInfo.reportSeverityInformationOfDTC.GetFunctionalUnitOfDTC.PENDING,1 */
    else if(ResultCodeFromDEM == DEM_PENDING)
    {
     ResultCodeFromExecute = DCM_FACADE_PENDING;
    }
    /* !LINKSTO Dcm.ReadDTCInfo.reportSeverityInformationOfDTC.GetFunctionalUnitOfDTC.DEM_WRONG_DTC,1 */
    /* !LINKSTO Dcm.ReadDTCInfo.reportSeverityInformationOfDTC.GetFunctionalUnitOfDTC.DEM_WRONG_DTCORIGIN,1 */
    else if((ResultCodeFromDEM == DEM_WRONG_DTC)||
            (ResultCodeFromDEM == DEM_WRONG_DTCORIGIN))
    {
      ResultCodeFromExecute = DCM_FACADE_REQUESTOUTOFRANGE;
    }
    else
    {
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetFunctionalUnitOfDTC.E_NOT_OK.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetFunctionalUnitOfDTC.DEM_BUSY.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetFunctionalUnitOfDTC.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetFunctionalUnitOfDTC.E_NOT_OK.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetFunctionalUnitOfDTC.DEM_BUSY.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetFunctionalUnitOfDTC.UnsupportedError.DET,1 */
      ResultCodeFromExecute = DCM_FACADE_INVALID_VALUE;
    }
  }
  else
  {
    /* Value already obtained. */
    ResultCodeFromExecute = DCM_FACADE_OK;
  }

  return ResultCodeFromExecute;
}
#endif /* #if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON) */

#if (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON)

/** \brief _Setup function for obtaining SI30 from DEM */
#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_DemFacade_GetSI30_Setup
(
  P2VAR(Dcm_GetSI30ContextType, AUTOMATIC, DCM_VAR) GetSI30Context,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) GetDTCStatusContextResponseBuffer,
  Dcm_MsgLenType AvailableBufferSize
)
{
  GetSI30Context->ResponseBuffer = GetDTCStatusContextResponseBuffer;
  GetSI30Context->BufferSize = AvailableBufferSize;
  GetSI30Context->StatusOfSI30ForCurrentDTC = DCM_NOT_OBTAINED;
  GetSI30Context->ReportOnSelectStatusIndicator = DCM_NOT_OBTAINED;
  return;
}
#endif /* #if (DCM_DSP_USE_SERVICE_0XAF == STD_ON) */

/** \brief _Setup function for obtaining Extended Data Records from DEM */
STATIC FUNC(void, DCM_CODE) Dcm_DemFacade_ReadEventDataRecord_Setup
(
    P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext,
    P2VAR(uint8, AUTOMATIC, DCM_VAR) ReadExtDataRecContextResponseBuffer,
    Dcm_MsgLenType AvailableBufferSize,
    uint32 DTCMaskSetup,
    uint8 DataRecordNumber,
    Dcm_MsgLenType SizeOfOnePage,
    uint8 DataKind,
    uint8 DTCFormat
)
{
  /* Gather necessary data from the request for this data unit */
  DiagnosticDataContext->DTCMask = DTCMaskSetup;
  DiagnosticDataContext->RequestedDataRecordNumber = DataRecordNumber;
  DiagnosticDataContext->ResponseBuffer = ReadExtDataRecContextResponseBuffer;
  DiagnosticDataContext->BufferSize = AvailableBufferSize;
  /*Set-up relevant Data for program flow control */
  DiagnosticDataContext->AmountOfDataToWriteOnResponse = 0U;
  DiagnosticDataContext->AmountOfDataStoredOnCompleteResponse = 0U;
  DiagnosticDataContext->AmountOfDataStoredOnResponseOnCurrentRun = 0U;
  DiagnosticDataContext->DataKind = DataKind;
  DiagnosticDataContext->DTCFormat = DTCFormat;
  if (DataKind == DCM_FACADE_EXTENDED_DATA_RECORDS)
  {
    DiagnosticDataContext->SelectDataDemFunction  = &Dem_SelectExtendedDataRecord;
    DiagnosticDataContext->GetSizeOfDataDemFunction = &Dem_GetSizeOfExtendedDataRecordSelection;
    DiagnosticDataContext->GetDataDemFunction = &Dem_GetNextExtendedDataRecord;
  }
  else
  {
    DCM_PRECONDITION_ASSERT((DataKind == DCM_FACADE_FREEZE_FRAME_DATA),
                             DCM_SID_READ_DTC_INFORMATION);

    DiagnosticDataContext->SelectDataDemFunction = &Dem_SelectFreezeFrameData;
    DiagnosticDataContext->GetSizeOfDataDemFunction = &Dem_GetSizeOfFreezeFrameSelection;
    DiagnosticDataContext->GetDataDemFunction = &Dem_GetNextFreezeFrameData;
  }

  DiagnosticDataContext->NextPossitionToFillOnResponseBuffer = 0U;
  DiagnosticDataContext->SizeOfOnePage = SizeOfOnePage;
  DiagnosticDataContext->ReportOnSelectData = DCM_NOT_OBTAINED;
  DiagnosticDataContext->ReportOnSizeOfDataToRead = DCM_NOT_OBTAINED;
  DiagnosticDataContext->ReportOnReadOfExtendedDataRecord = DCM_NOT_COMPLETED;
  DiagnosticDataContext->ControlRecordUpdate = DCM_RECORDUPDATE_ENABLED;
  return;
}

/** \brief _Execute function for obtaining Extended Data Records from DEM */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_ReadEventDataRecord_Execute
(
  const Dcm_DemClientIdType ClientId
#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
  ,
  P2VAR(Dcm_GetSI30ContextType, AUTOMATIC, DCM_VAR) GetSI30Context
  ,
  P2VAR(boolean, AUTOMATIC, DCM_VAR) CurrentDTCProcessingReady
#endif /* #if(DCM_DSP_USE_SERVICE_0XAF == STD_ON) */
)
{
  Std_ReturnType ResultCodeFromExecute = DCM_FACADE_NOT_OK;
  Std_ReturnType ResultCodeFromRecordUpdate = DCM_FACADE_NOT_OK;
  Std_ReturnType SelectEventDataResult = DCM_FACADE_NOT_OK;
  Std_ReturnType ResultCodeFromGetSizeOfEventData = DCM_FACADE_NOT_OK;
  Std_ReturnType ResultCodeFromGetEventData = DCM_FACADE_NOT_OK;
  boolean ReportErrorFromDEM = FALSE;
  P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext =
    &((Dcm_Dsp_DemFacade_GetClient(ClientId))->DiagnosticDataContext);

  /* ---- DISABLE DTC RECORD UPDATE ------------------------------- */

  ResultCodeFromRecordUpdate =
    Dcm_DemFacade_DisableDTCRecordUpdate(ClientId,
      &DiagnosticDataContext->ControlRecordUpdate);

  /* Modify result of the complete execute only if is relevant  */
  if(ResultCodeFromRecordUpdate != DCM_FACADE_OK)
  {
    ResultCodeFromExecute = ResultCodeFromRecordUpdate;
    ReportErrorFromDEM = TRUE;
  }
  if( DCM_RECORDUPDATE_DISABLED == DiagnosticDataContext->ControlRecordUpdate )
  {
#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
    ResultCodeFromExecute = Dcm_DemFacade_ReadStatusIndicator_Execute(ClientId,
                                                                      DiagnosticDataContext,
                                                                      GetSI30Context,
                                                                      &ReportErrorFromDEM);

#endif /* #if(DCM_DSP_USE_SERVICE_0XAF == STD_ON) */

    if((DiagnosticDataContext->ReportOnSelectData == DCM_NOT_OBTAINED) &&
       (ReportErrorFromDEM == FALSE) )
    {
      /* !LINKSTO Dcm.EB.UDSServices.reportDTCExtDataRecordByDTCNumber.SupportedRecordNumber.CollectExtendedDataRecord,1 */
      /* !LINKSTO Dcm.EB.UDSServices.reportUserDefMemoryDTCExtDataRecordByDTCNumber.SupportedRecordNumber.CollectExtendedDataRecord,1 */
      /* !LINKSTO Dcm.EB.UDSServices.reportGenericExtendedDataByDTCNumber.SupportedRecordNumber.CollectExtendedDataRecord,1 */
      SelectEventDataResult = Dcm_DemFacade_SelectEventData(ClientId,
#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
                                                            NULL_PTR,
#endif
                                                            DiagnosticDataContext);
      if(DCM_FACADE_OK != SelectEventDataResult)
      {
        ResultCodeFromExecute = SelectEventDataResult;
        ReportErrorFromDEM = TRUE;
      }
    }
    /* Obtain Size of Data to access */
    if((DiagnosticDataContext->ReportOnSizeOfDataToRead == DCM_NOT_OBTAINED)&&
       (DiagnosticDataContext->ReportOnSelectData == DCM_OBTAINED) &&
       (ReportErrorFromDEM == FALSE))
    {
      /* ---- CALCULATION OF DATA SIZE ------------------------------- */

      ResultCodeFromGetSizeOfEventData = Dcm_DemFacade_GetSizeOfEventData(ClientId, DiagnosticDataContext);
      /* Modify result of the complete execute only if is relevant  */
      if (DCM_FACADE_OK != ResultCodeFromGetSizeOfEventData)
      {
        ResultCodeFromExecute = ResultCodeFromGetSizeOfEventData;
        ReportErrorFromDEM = TRUE;
      }
    }
    /* ---- FETCH DATA ------------------------------------- */
    /* After Amount of data to read has been obtained is possible to proceed with fetching of data
     * and re-enable of DTC Record Data, if no error condition has been detected so far
     * (ResultCodeFromExecute has not changed) */
    if((DiagnosticDataContext->ReportOnSizeOfDataToRead == DCM_OBTAINED)&&
       (ReportErrorFromDEM == FALSE))
    {
      /* if the amount of data to read is not equal to zero, access
       * the Dem_GetNextExtendedDataRecord  */
      if(DiagnosticDataContext->AmountOfDataToWriteOnResponse != 0U)
      {
        ResultCodeFromGetEventData = Dcm_DemFacade_GetEventData(ClientId, DiagnosticDataContext);
        if(ResultCodeFromGetEventData  != DCM_FACADE_OK)
        {
          ResultCodeFromExecute = ResultCodeFromGetEventData;
          ReportErrorFromDEM = TRUE;
        }
      }
      else
      {
        DiagnosticDataContext->ControlRecordUpdate = DCM_RECORDUPDATE_DOENABLE;
      }
    }
    /* ---- ENABLE DTC RECORD UPDATE ------------------------------------- */
    ResultCodeFromRecordUpdate =
        Dcm_DemFacade_EnableDTCRecordUpdate(ClientId,
                                            &DiagnosticDataContext->ControlRecordUpdate);
    if(FALSE == ReportErrorFromDEM)
    {
#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
        *CurrentDTCProcessingReady = TRUE;
#endif /* #if(DCM_DSP_USE_SERVICE_0XAF == STD_ON) */
      ResultCodeFromExecute = ResultCodeFromRecordUpdate;
    }


  }
  return ResultCodeFromExecute;
}

/* Auxiliary function to disable DTC Record Update */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_DisableDTCRecordUpdate
(
  const Dcm_DemClientIdType ClientId,
  P2VAR(boolean, AUTOMATIC, DCM_VAR) ControlRecordUpdate
)
{
  Std_ReturnType ResultFromDisableRecordUpdate = DCM_FACADE_NOT_OK;
  Std_ReturnType ResultCodeFromDEM = E_NOT_OK;
  /* Execute access to DEM interface */
  /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetStatusOfDTC.E_OK,1 */
  /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetStatusOfDTC.E_OK,1 */
  /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.E_OK,1 */
  /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetStatusOfDTC.E_OK,1 */
  /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetStatusOfDTC.E_OK,1 */
  /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetStatusOfDTC.E_OK,1 */
  /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
  /* !LINKSTO Dcm.ReadGenericInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
  if(DCM_RECORDUPDATE_ENABLED == (*ControlRecordUpdate))
  {
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportDTCSnapshotRecordByDTCNumber.ReadEventData.LockDTCRecordUpdate,1 */
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportDTCExtDataRecordByDTCNumber.ReadEventData.LockDTCRecordUpdate,1 */
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.ReadEventData.LockDTCRecordUpdate,1 */
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportUserDefMemoryDTCExtDataRecordByDTCNumber.ReadEventData.LockDTCRecordUpdate,1 */
    /* !LINKSTO Dcm.UDSServices.ReadGenericInformation.reportGenericSnapshotByDTCNumber.ReadEventData.LockDTCRecordUpdate,1 */
    /* !LINKSTO Dcm.UDSServices.ReadGenericInformation.reportGenericExtendedDataByDTCNumber.ReadEventData.LockDTCRecordUpdate,1 */
    ResultCodeFromDEM = Dem_DisableDTCRecordUpdate(ClientId);
    switch(ResultCodeFromDEM)
    {
      case E_OK:
      {
        /* Signal that the DTC record Update has been disabled */
        *ControlRecordUpdate = DCM_RECORDUPDATE_DISABLED;
        ResultFromDisableRecordUpdate = DCM_FACADE_OK;
        break;
      }
      case DEM_PENDING:
      {
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.DisableDTCRecordUpdate.DEM_PENDING,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.DisableDTCRecordUpdate.DEM_PENDING,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.DisableDTCRecordUpdate.DEM_PENDING,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.DisableDTCRecordUpdate.DEM_PENDING,1 */
        /* !LINKSTO Dcm.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.DisableDTCRecordUpdate.DEM_PENDING,1 */
        /* !LINKSTO Dcm.ReadGenericInfo.reportGenericSnapshotByDTCNumber.DisableDTCRecordUpdate.DEM_PENDING,1 */
        ResultFromDisableRecordUpdate = DCM_FACADE_PENDING;
        break;
      }
      case DEM_WRONG_DTC:
      case DEM_WRONG_DTCORIGIN:
      {
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.DisableDTCRecordUpdate.DEM_WRONG_DTC,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.DisableDTCRecordUpdate.DEM_WRONG_DTCORIGIN,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.DisableDTCRecordUpdate.DEM_WRONG_DTCORIGIN,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.DisableDTCRecordUpdate.DEM_WRONG_DTC,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.DisableDTCRecordUpdate.DEM_WRONG_DTCORIGIN,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.DisableDTCRecordUpdate.DEM_WRONG_DTC,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.DisableDTCRecordUpdate.DEM_WRONG_DTC,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.DisableDTCRecordUpdate.DEM_WRONG_DTCORIGIN,1 */
        /* !LINKSTO Dcm.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.DisableDTCRecordUpdate.DEM_WRONG_DTC,1 */
        /* !LINKSTO Dcm.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.DisableDTCRecordUpdate.DEM_WRONG_DTCORIGIN,1 */
        /* !LINKSTO Dcm.ReadGenericInfo.reportGenericSnapshotByDTCNumber.DisableDTCRecordUpdate.DEM_WRONG_DTC,1 */
        /* !LINKSTO Dcm.ReadGenericInfo.reportGenericSnapshotByDTCNumber.DisableDTCRecordUpdate.DEM_WRONG_DTCORIGIN,1 */
        ResultFromDisableRecordUpdate = DCM_FACADE_REQUESTOUTOFRANGE;
        break;
      }
      default:
      {
        /* In case of DEM_BUSY, E_NOT_OK to avoid code duplicate or
           when Enabling the DTC Record disabled fails */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.DisableDTCRecordUpdate.DEM_BUSY.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.DisableDTCRecordUpdate.DEM_BUSY.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.DisableDTCRecordUpdate.DEM_BUSY.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.DisableDTCRecordUpdate.DEM_BUSY.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.DisableDTCRecordUpdate.DEM_BUSY.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.DisableDTCRecordUpdate.DEM_BUSY.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.DisableDTCRecordUpdate.DEM_BUSY.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.DisableDTCRecordUpdate.DEM_BUSY.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.DisableDTCRecordUpdate.DEM_BUSY.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.DisableDTCRecordUpdate.DEM_BUSY.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.DisableDTCRecordUpdate.DEM_BUSY.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.DisableDTCRecordUpdate.DEM_BUSY.DET,1 */

        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.DisableDTCRecordUpdate.E_NOT_OK.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.DisableDTCRecordUpdate.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.DisableDTCRecordUpdate.E_NOT_OK.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.DisableDTCRecordUpdate.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.DisableDTCRecordUpdate.E_NOT_OK.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.DisableDTCRecordUpdate.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.DisableDTCRecordUpdate.E_NOT_OK.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.DisableDTCRecordUpdate.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.DisableDTCRecordUpdate.E_NOT_OK.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.DisableDTCRecordUpdate.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.DisableDTCRecordUpdate.E_NOT_OK.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.DisableDTCRecordUpdate.E_NOT_OK.DET,1 */

        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.DisableDTCRecordUpdate.UnsupportedError.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.DisableDTCRecordUpdate.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.DisableDTCRecordUpdate.UnsupportedError.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.DisableDTCRecordUpdate.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.DisableDTCRecordUpdate.UnsupportedError.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.DisableDTCRecordUpdate.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.DisableDTCRecordUpdate.UnsupportedError.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.DisableDTCRecordUpdate.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.DisableDTCRecordUpdate.UnsupportedError.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.DisableDTCRecordUpdate.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.DisableDTCRecordUpdate.UnsupportedError.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.DisableDTCRecordUpdate.UnsupportedError.DET,1 */
        ResultFromDisableRecordUpdate = DCM_FACADE_NOT_OK;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        DCM_DET_REPORT_ERROR(DCM_SVCID_DISABLEDTCRECORDUPDATE, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
        break;
      }
    }
  }
  else
  {
    ResultFromDisableRecordUpdate = DCM_FACADE_OK;
  }

  return ResultFromDisableRecordUpdate;
}

#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
/* Auxiliary function to Get Status Indicator EDR 0x30 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_GetStatusIndicator
(
  Dcm_DemClientIdType ClientId,
  P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext,
  P2VAR(Dcm_GetSI30ContextType, AUTOMATIC, DCM_VAR) GetSI30Context
)
{
  Std_ReturnType ResultFromGetStatusIndicator = DCM_FACADE_NOT_OK;
  uint8 ResultCodeFromDEM = E_NOT_OK;
  uint16 AvailableBufferSize;

  AvailableBufferSize = (uint16)GetSI30Context->BufferSize;
  /* !LINKSTO Dcm.ReadGenericInformation.ObtainStatusIndicator,2 */
  /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SelectExtendedDataRecord.0x30.E_OK,1 */
  /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SelectExtendedDataRecord.0x30.E_OK,1 */
  /* !LINKSTO Dcm.ReadGenericInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
  ResultCodeFromDEM = (uint8)Dem_GetNextExtendedDataRecord(ClientId,
                                                     /* Write the data one position
                                                     after the record number */
                                                     GetSI30Context->ResponseBuffer,
                                                     &AvailableBufferSize);
  if(ResultCodeFromDEM == E_OK)
  {
    /* After accessing the DEM, the AvailableBufferSize variable contains
     * the amount of data which has been read */
    if(AvailableBufferSize != 0U)
    {
      GetSI30Context->BufferSize -= AvailableBufferSize;

      Dcm_ReadEventDataServicesContext.HeaderData[4] = GetSI30Context->ResponseBuffer[0];
      Dcm_ReadEventDataServicesContext.WriteHeaderDataBeforeRecordNumber = FALSE;
      GetSI30Context->StatusOfSI30ForCurrentDTC = DCM_OBTAINED;
      ResultFromGetStatusIndicator = DCM_FACADE_OK;
    }
    else
    {
      /* Signal to caller that the process has not been able to obtain the SI */
      ResultFromGetStatusIndicator = DCM_FACADE_WRONGCONDITION;
    }
  }
  else if(ResultCodeFromDEM == DEM_PENDING)
  {
    /* !LINKSTO Dcm.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.0x30.DEM_PENDING,1 */
    /* !LINKSTO Dcm.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextExtendedDataRecord.0x30.DEM_PENDING,1 */
    ResultFromGetStatusIndicator = DCM_FACADE_PENDING;
  }
  else if(DEM_NO_SUCH_ELEMENT == ResultCodeFromDEM)
  {
    DiagnosticDataContext->ControlRecordUpdate = DCM_RECORDUPDATE_DOENABLE;
    /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.0x30.DEM_NO_SUCH_ELEMENT,1 */
    /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextExtendedDataRecord.0x30.DEM_NO_SUCH_ELEMENT,1 */
    ResultFromGetStatusIndicator = DCM_FACADE_REQUESTOUTOFRANGE;
  }
  else
  {
    ResultFromGetStatusIndicator = Dcm_DemFacade_ProcessDEMResponseFromGetData(
                                                  DiagnosticDataContext,
                                                  ResultCodeFromDEM);
  }
  return ResultFromGetStatusIndicator;
}
#endif /* #if(DCM_DSP_USE_SERVICE_0XAF == STD_ON) */

/* Auxiliary function to Select EDR */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_SelectEventData
(
  Dcm_DemClientIdType ClientId,
#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
  P2VAR(Dcm_GetSI30ContextType, AUTOMATIC, DCM_VAR) GetSI30Context,
#endif
  P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext)
{

  Std_ReturnType ResultFromSelectData = DCM_FACADE_NOT_OK;
  uint8 ResultCodeFromDEM = E_NOT_OK;
  uint8 RequestedDataRecord = DiagnosticDataContext->RequestedDataRecordNumber;

#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
  if (NULL_PTR != GetSI30Context)
  {
    /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.DisableDTCRecordUpdate.E_OK,1 */
    /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.DisableDTCRecordUpdate.E_OK,1 */
    /* !LINKSTO Dcm.ReadGenericInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
    ResultCodeFromDEM = (uint8) Dem_SelectExtendedDataRecord(ClientId, DCM_STATUS_INDICATOR);
  }
  else
#endif
  {
    /* Execute access to DEM interface to select expected data */
    /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.DisableDTCRecordUpdate.E_OK,1 */
    /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.DisableDTCRecordUpdate.E_OK,1 */
    /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.DisableDTCRecordUpdate.E_OK,1 */
    /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.RetrieveFreezeFrame,1 */
    /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.DisableDTCRecordUpdate.E_OK,1 */
    /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.0x30.E_OK,1 */
    /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextExtendedDataRecord.0x30.E_OK,1 */
    /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
    /* !LINKSTO Dcm.ReadGenericInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
    ResultCodeFromDEM = (uint8)DiagnosticDataContext->SelectDataDemFunction(ClientId,
                                                                            RequestedDataRecord);
  }
  /* Return value from interfaces for EDR and Freeze Frames
   * are identical in value and significance */
  if(ResultCodeFromDEM == E_OK)
  {
#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
    if (NULL_PTR != GetSI30Context)
    {
      GetSI30Context->ReportOnSelectStatusIndicator = DCM_OBTAINED;
    }
    else
#endif
    {
    DiagnosticDataContext->ReportOnSelectData = DCM_OBTAINED;
    }
    ResultFromSelectData = DCM_FACADE_OK;
  }
  else if(DEM_PENDING == ResultCodeFromDEM)
  {
    /* For this case there is no to store additional data, because on re-entry
     * the call to DEM interface will be made with the parameter already provided
     * on the _setup function */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.SelectExtendedDataRecord.DEM_PENDING,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.SelectExtendedDataRecord.DEM_PENDING,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.SelectFreezeFrameData.DEM_PENDING,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.SelectFreezeFrameData.DEM_PENDING,1 */
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SelectExtendedDataRecord.DEM_PENDING,1 */
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SelectFreezeFrameData.DEM_PENDING,1 */
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SelectExtendedDataRecord.0x30.DEM_PENDING,1 */
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SelectExtendedDataRecord.0x30.DEM_PENDING,1 */
    ResultFromSelectData = DCM_FACADE_PENDING;
  }
  else if(DEM_BUSY == ResultCodeFromDEM)
  {
    DiagnosticDataContext->ControlRecordUpdate = DCM_RECORDUPDATE_DOENABLE;
    /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.SelectExtendedDataRecord.DEM_BUSY.NRC,1 */
    /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.SelectExtendedDataRecord.DEM_BUSY.NRC,1 */
    /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.SelectFreezeFrameData.DEM_BUSY.NRC,1 */
    /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.SelectFreezeFrameData.DEM_BUSY.NRC,1 */
    /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SelectExtendedDataRecord.0x30.DEM_BUSY.NRC,1 */
    /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SelectExtendedDataRecord.0x30.DEM_BUSY.NRC,1 */
    /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SelectExtendedDataRecord.DEM_BUSY.NRC,1 */
    /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SelectFreezeFrameData.DEM_BUSY.NRC,1 */
    ResultFromSelectData = DCM_FACADE_NOT_OK;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    if(DiagnosticDataContext->DataKind == DCM_FACADE_EXTENDED_DATA_RECORDS)
    {
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.SelectExtendedDataRecord.DEM_BUSY.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.SelectExtendedDataRecord.DEM_BUSY.DET,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SelectExtendedDataRecord.0x30.DEM_BUSY.DET,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SelectExtendedDataRecord.DEM_BUSY.DET,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SelectExtendedDataRecord.0x30.DEM_BUSY.DET,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SelectFreezeFrameData.DEM_BUSY.DET,1 */
      DCM_DET_REPORT_ERROR(DCM_SVCID_GETEXTENDEDDATABYDTC, DCM_E_INTERFACE_RETURN_VALUE);
    }
    else
    {
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.SelectFreezeFrameData.DEM_BUSY.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.SelectFreezeFrameData.DEM_BUSY.DET,1 */
      DCM_DET_REPORT_ERROR(DCM_SVCID_GETFREEZEFRAMEDATABYDTC, DCM_E_INTERFACE_RETURN_VALUE);
    }
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  }
  else
  {
    DiagnosticDataContext->ControlRecordUpdate = DCM_RECORDUPDATE_DOENABLE;
    ResultFromSelectData = Dcm_DemFacade_ProcessDEMResponseFromSelectData(
#if (DCM_DEV_ERROR_DETECT == STD_ON)
                                               DiagnosticDataContext,
#endif /*  (DCM_DEV_ERROR_DETECT == STD_ON) */
                                               ResultCodeFromDEM);
  }

  return ResultFromSelectData;
}

/* Auxiliary function to Get Size of EDR */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_GetSizeOfEventData
(
  Dcm_DemClientIdType ClientId,
  P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext
)
{
  Std_ReturnType ResultFromGetSizeOfExtendedDataRecord = DCM_FACADE_NOT_OK;
  uint8 ResultCodeFromDEM = E_NOT_OK;
  uint8 RequestedDataRecord = DiagnosticDataContext->RequestedDataRecordNumber;
  uint16 SizeOfEventData;

  /* Execute access to DEM interface to obtain size of expected data*/
  /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.SelectExtendedDataRecord.E_OK,1 */
  /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.SelectExtendedDataRecord.E_OK,1 */
  /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.SelectFreezeFrameData.E_OK,1 */
  /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.SizeOfData,1 */
  /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.SelectFreezeFrameData.E_OK,1 */
  /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SelectExtendedDataRecord.E_OK,1 */
  /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SelectFreezeFrameData.E_OK,1 */
  /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.SizeOfData,1 */
  /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
  /* !LINKSTO Dcm.ReadGenericInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
  ResultCodeFromDEM = (uint8)
  DiagnosticDataContext->GetSizeOfDataDemFunction(ClientId,
  &SizeOfEventData);

  /* Return value from interfaces for EDR and Freeze Frames
   * are identical in value and significance */
  if(ResultCodeFromDEM == E_OK)
  {
    /* Update Value of total amount of data to be written on the response
     * buffer including the Extended Data Record number, only if current
     * Data Record has data to report */
    if((SizeOfEventData != 0U)&&
    (SizeOfEventData <= DiagnosticDataContext->SizeOfOnePage)&&
    (RequestedDataRecord != 0xFFU))
    {
      /* If the request is NOT for all EDRs, and the size of the data of the requested EDR
       * can fit in one buffer page */
      DiagnosticDataContext->AmountOfDataToWriteOnResponse = SizeOfEventData;

      /* Update information regarding having obtained the total data size */
      DiagnosticDataContext->ReportOnSizeOfDataToRead = DCM_OBTAINED;

      /* Signal to calling entity */
      ResultFromGetSizeOfExtendedDataRecord = DCM_FACADE_OK;
    }
    else if(RequestedDataRecord == 0xFFU)
    {
      /* If the request is for all EDRs, store the total expected size regardless
       * of it's size */
      DiagnosticDataContext->AmountOfDataToWriteOnResponse = SizeOfEventData;

      /* Update information regarding having obtained the total data size */
      DiagnosticDataContext->ReportOnSizeOfDataToRead = DCM_OBTAINED;

      /* Signal to calling entity */
      ResultFromGetSizeOfExtendedDataRecord = DCM_FACADE_OK;
    }
    else if((SizeOfEventData >= DiagnosticDataContext->SizeOfOnePage)&&
    (RequestedDataRecord != 0xFFU))
    {
      /* If the request is for one particular EDR, and the data will not fit on the page */
      ResultFromGetSizeOfExtendedDataRecord = DCM_FACADE_NOT_OK;
    }
    else
    {
      /* Update information regarding having obtained the total data size */
      DiagnosticDataContext->ReportOnSizeOfDataToRead = DCM_OBTAINED;

      /* Signal to calling entity */
      ResultFromGetSizeOfExtendedDataRecord = DCM_FACADE_OK;
    }

  }
  else if(ResultCodeFromDEM == E_NOT_OK)
  {
    DiagnosticDataContext->ControlRecordUpdate = DCM_RECORDUPDATE_DOENABLE;
    /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetSizeOfExtendedDataRecordSelection.E_NOT_OK.NRC,1 */
    /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetSizeOfExtendedDataRecordSelection.E_NOT_OK.NRC,1 */
    /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetSizeOfFreezeFrameSelection.E_NOT_OK.NRC,1 */
    /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetSizeOfFreezeFrameSelection.E_NOT_OK.NRC,1 */
    /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetSizeOfExtendedDataRecordSelection.E_NOT_OK.NRC,1 */
    /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetSizeOfFreezeFrameSelection.E_NOT_OK.NRC,1 */
    ResultFromGetSizeOfExtendedDataRecord = DCM_FACADE_NOT_OK;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    if(DiagnosticDataContext->DataKind == DCM_FACADE_EXTENDED_DATA_RECORDS)
    {
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetSizeOfExtendedDataRecordSelection.E_NOT_OK.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetSizeOfExtendedDataRecordSelection.E_NOT_OK.DET,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetSizeOfExtendedDataRecordSelection.E_NOT_OK.DET,1 */
      DCM_DET_REPORT_ERROR(DCM_SVCID_GETEXTENDEDDATABYDTC, DCM_E_INTERFACE_RETURN_VALUE);
    }
    else
    {
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetSizeOfFreezeFrameSelection.E_NOT_OK.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetSizeOfFreezeFrameSelection.E_NOT_OK.DET,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetSizeOfFreezeFrameSelection.E_NOT_OK.DET,1 */
      DCM_DET_REPORT_ERROR(DCM_SVCID_GETFREEZEFRAMEDATABYDTC, DCM_E_INTERFACE_RETURN_VALUE);
    }
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  }
  else if(ResultCodeFromDEM == DEM_PENDING)
  {
    /* For this case there is no to store additional data, because on re-entry
     * the call to DEM interface will be made with the parameter already provided
     * on the _setup function */
    /* !LINKSTO Dcm.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetSizeOfExtendedDataRecordSelection.DEM_PENDING,1 */
    /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetSizeOfExtendedDataRecordSelection.DEM_PENDING,1 */
    /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetSizeOfFreezeFrameSelection.DEM_PENDING,1 */
    /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetSizeOfFreezeFrameSelection.DEM_PENDING,1 */
    /* !LINKSTO Dcm.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetSizeOfExtendedDataRecordSelection.DEM_PENDING,1 */
    /* !LINKSTO Dcm.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetSizeOfFreezeFrameSelection.DEM_PENDING,1 */
    ResultFromGetSizeOfExtendedDataRecord = DCM_FACADE_PENDING;
  }
  else
  {
    DiagnosticDataContext->ControlRecordUpdate = DCM_RECORDUPDATE_DOENABLE;
    ResultFromGetSizeOfExtendedDataRecord = Dcm_DemFacade_ProcessDEMResponseFromGetSize(
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    DiagnosticDataContext->DataKind,
#endif
    ResultCodeFromDEM);
  }

  /* If the amount of Data to read from DEM is bigger than the provided Buffer... */
  if(DiagnosticDataContext->AmountOfDataToWriteOnResponse > DiagnosticDataContext->BufferSize)
  {
#if (DCM_PAGEDBUFFER_ENABLED == STD_OFF)
    DiagnosticDataContext->ControlRecordUpdate = DCM_RECORDUPDATE_DOENABLE;
#endif /* (DCM_PAGEDBUFFER_ENABLED == STD_OFF) */
    /* ... Signal the caller that Page buffering must be activated */
    ResultFromGetSizeOfExtendedDataRecord = DCM_FACADE_BUFFERFULL;
  }

  return ResultFromGetSizeOfExtendedDataRecord;
}

/* Auxiliary function to Get Data of EDR */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_GetEventData
(
  Dcm_DemClientIdType ClientId,
  P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext
)
{
  Std_ReturnType ResultFromGetExtendedDataRecord = DCM_FACADE_NOT_OK;
  uint8 ResultCodeFromDEM = E_OK;
  Dcm_MsgLenType CurrentWritePositionOnReponseBuffer;
  uint16 AvailableBufferSize;
  uint16 WriteOffset = 0U;
  uint16 WriteAFHeaderOffset = 0U;

  /* initialize the index for writing into the response buffer, since it is required to keep
   * the format: "... [DTCExtDataRecordNumber][DTCExtDataRecordNumber_Data] ...",
   * where the DataRecord number is on the first position [0] of the current buffer section
   * being written, and the Actual data of the
   * DataRecord starts on the next position [1]. See Table 276 of ISO 14229-1:2013 */
  CurrentWritePositionOnReponseBuffer =
    DiagnosticDataContext->NextPossitionToFillOnResponseBuffer;
  /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.E_OK,1 */
  /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.E_OK,1 */
  /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.E_OK,1 */
  /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.E_OK,1 */
  /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.E_OK,1 */
  /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextFreezeFrameData.E_OK,1 */
  /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.CollectFreezeFrameData,1 */
  /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.CollectFreezeFrameData,1 */
  /* Deviation TASKING-2 */
  while (E_OK == ResultCodeFromDEM)
  {
#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
    if((DiagnosticDataContext->ServiceID == DCM_DSP_SERVICE_0XAF)&&
        (DiagnosticDataContext->DataKind == DCM_FACADE_FREEZE_FRAME_DATA)&&
        (Dcm_ReadEventDataServicesContext.WriteHeaderDataBeforeRecordNumber == TRUE))
    {
      WriteAFHeaderOffset = 5U;
    }
    else
#endif /* #if (DCM_DSP_USE_SERVICE_0XAF == STD_ON) */
    {
      WriteAFHeaderOffset = 0U;
    }
    WriteOffset = WriteAFHeaderOffset;
    AvailableBufferSize = (uint16)DiagnosticDataContext->BufferSize;
    /* Avoid accessing the DEM interface if there is no more place on the response buffer */
    if(AvailableBufferSize > WriteOffset)
    {
      AvailableBufferSize -= WriteOffset;
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetSizeOfExtendedDataRecordSelection.E_OK,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetSizeOfExtendedDataRecordSelection.E_OK,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetSizeOfFreezeFrameSelection.E_OK,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetSizeOfFreezeFrameSelection.E_OK,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetSizeOfExtendedDataRecordSelection.E_OK,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetSizeOfFreezeFrameSelection.E_OK,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
      /* !LINKSTO Dcm.ReadGenericInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
      ResultCodeFromDEM =
        (uint8)DiagnosticDataContext->GetDataDemFunction(ClientId,
               /* Write the data one position after
               the record number */
               &DiagnosticDataContext->ResponseBuffer[CurrentWritePositionOnReponseBuffer
                                                      + WriteOffset],
               &AvailableBufferSize);
    }
    else
    {
      /* This would be the expected response from the DEM interface when the provided buffer
       * is of size zero, but  by hijacking the response code like this, an unnecessary entry
       * to the DEM interface is avoided.*/
      ResultCodeFromDEM = DEM_BUFFER_TOO_SMALL;
    }
    if(ResultCodeFromDEM == E_OK)
    {
      /* After accessing the DEM, the AvailableBufferSize variable contains the amount
       * of data which has been read */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.BuffSizeZero,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.BuffSizeZero,1 */
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.BuffSizeZero,1 */
      if(AvailableBufferSize != 0U)
      {
        /* If the response buffer size was big enough is already checked by DEM-read-function */
        DCM_POSTCONDITION_ASSERT(
            DiagnosticDataContext->BufferSize >=
              ((Dcm_MsgLenType)AvailableBufferSize + (Dcm_MsgLenType)WriteOffset),
            DCM_SID_READ_DTC_INFORMATION);

#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
        if((DiagnosticDataContext->ServiceID == DCM_DSP_SERVICE_0XAF)&&
           (DiagnosticDataContext->DataKind == DCM_FACADE_FREEZE_FRAME_DATA))
        {
          if(Dcm_ReadEventDataServicesContext.WriteHeaderDataBeforeRecordNumber == TRUE)
          {
            /* Store Extended Data Number only if there is something to write */
            DiagnosticDataContext->ResponseBuffer[CurrentWritePositionOnReponseBuffer] =
                Dcm_ReadEventDataServicesContext.HeaderData[0];
            DiagnosticDataContext->ResponseBuffer[CurrentWritePositionOnReponseBuffer + 1] =
                Dcm_ReadEventDataServicesContext.HeaderData[1];
            DiagnosticDataContext->ResponseBuffer[CurrentWritePositionOnReponseBuffer + 2] =
                Dcm_ReadEventDataServicesContext.HeaderData[2];
            DiagnosticDataContext->ResponseBuffer[CurrentWritePositionOnReponseBuffer + 3] =
                Dcm_ReadEventDataServicesContext.HeaderData[3];
            DiagnosticDataContext->ResponseBuffer[CurrentWritePositionOnReponseBuffer + 4] =
                Dcm_ReadEventDataServicesContext.HeaderData[4];
          }
          else
          {
            Dcm_ReadEventDataServicesContext.WriteHeaderDataBeforeRecordNumber = TRUE;
          }

        }
#endif /* #if (DCM_DSP_USE_SERVICE_0XAF == STD_ON) */
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.Response,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.Response,1 */
        /* Advance on the response buffer only if the read was successful */
        CurrentWritePositionOnReponseBuffer +=
         ((Dcm_MsgLenType)AvailableBufferSize + (Dcm_MsgLenType)WriteOffset);

        /* Remember position used for writing on the response */
        DiagnosticDataContext->NextPossitionToFillOnResponseBuffer =
          CurrentWritePositionOnReponseBuffer;

        /* Update amount of data written on the buffer */
        DiagnosticDataContext->AmountOfDataStoredOnCompleteResponse +=
          (Dcm_MsgLenType)AvailableBufferSize;

        /* Update report of data written on current execution.
         * This has to be set to 0 every time the executer is called */
        DiagnosticDataContext->AmountOfDataStoredOnResponseOnCurrentRun +=
          ((Dcm_MsgLenType)AvailableBufferSize + (Dcm_MsgLenType)WriteOffset);

        /* Update amount of available space on the buffer */
        DiagnosticDataContext->BufferSize -= ((Dcm_MsgLenType)AvailableBufferSize +
            (Dcm_MsgLenType)WriteOffset);
      }
    }
  }
  /* If the expected amount of data matches the amount of data already obtained... */
  if(DiagnosticDataContext->AmountOfDataStoredOnCompleteResponse ==
    DiagnosticDataContext->AmountOfDataToWriteOnResponse)
  {
    /* ...then update control flow flag to indicate that the data has been read */
    DiagnosticDataContext->ControlRecordUpdate = DCM_RECORDUPDATE_DOENABLE;
    DiagnosticDataContext->ReportOnReadOfExtendedDataRecord = DCM_COMPLETED;
    ResultFromGetExtendedDataRecord = DCM_FACADE_OK;
  }
  else
  {
    ResultFromGetExtendedDataRecord =
        Dcm_DemFacade_ProcessDEMResponseFromGetData(DiagnosticDataContext, ResultCodeFromDEM);
  }

  return ResultFromGetExtendedDataRecord;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_EnableDTCRecordUpdate
(
  const Dcm_DemClientIdType ClientId,
  P2VAR(boolean, AUTOMATIC, DCM_VAR) ControlRecordUpdate
)
{
  Std_ReturnType ResultFromEnableRecordUpdate = DCM_FACADE_NOT_OK;
  Std_ReturnType ResultCodeFromDEM = E_NOT_OK;
  /* Execute access to DEM interface to enable Record Update*/
  /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
  /* !LINKSTO Dcm.ReadGenericInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
  if( DCM_RECORDUPDATE_DOENABLE == (*ControlRecordUpdate) )
  {
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportDTCSnapshotRecordByDTCNumber.ReadEventData.PositiveResponse.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportDTCSnapshotRecordByDTCNumber.ReadEventData.Cancel.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportDTCSnapshotRecordByDTCNumber.ReadEventData.NegativeResponse.SelectFreezeFrameData.Error.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportDTCSnapshotRecordByDTCNumber.ReadEventData.NegativeResponse.GetSizeOfFreezeFrameSelection.Error.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportDTCSnapshotRecordByDTCNumber.PageBufferEnabled.ReadEventData.NegativeResponse.GetNextFreezeFrameData.Error.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.EB.UDSServices.ReadDTCInformation.reportDTCSnapshotRecordByDTCNumber.PageBufferDisabled.ReadEventData.NegativeResponse.GetSizeOfFreezeFrameSelection.BufferFull.UnlockDTCRecordUpdate,1 */

    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportDTCExtDataRecordByDTCNumber.ReadEventData.PositiveResponse.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportDTCExtDataRecordByDTCNumber.ReadEventData.Cancel.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportDTCExtDataRecordByDTCNumber.ReadEventData.NegativeResponse.SelectExtendedDataRecord.Error.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportDTCExtDataRecordByDTCNumber.ReadEventData.NegativeResponse.GetSizeOfExtendedDataRecordSelection.Error.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportDTCExtDataRecordByDTCNumber.PageBufferEnabled.ReadEventData.NegativeResponse.GetNextExtendedDataRecord.Error.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.EB.UDSServices.ReadDTCInformation.reportDTCExtDataRecordByDTCNumber.PageBufferDisabled.ReadEventData.NegativeResponse.GetSizeOfExtendedDataRecordSelection.BufferFull.UnlockDTCRecordUpdate,1 */

    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.ReadEventData.PositiveResponse.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.ReadEventData.Cancel.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.ReadEventData.NegativeResponse.SelectFreezeFrameData.Error.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.ReadEventData.NegativeResponse.GetSizeOfFreezeFrameSelection.Error.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.PageBufferEnabled.ReadEventData.NegativeResponse.GetNextFreezeFrameData.Error.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.EB.UDSServices.ReadDTCInformation.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.PageBufferDisabled.ReadEventData.NegativeResponse.GetSizeOfFreezeFrameSelection.BufferFull.UnlockDTCRecordUpdate,1 */

    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportUserDefMemoryDTCExtDataRecordByDTCNumber.ReadEventData.PositiveResponse.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportUserDefMemoryDTCExtDataRecordByDTCNumber.ReadEventData.Cancel.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportUserDefMemoryDTCExtDataRecordByDTCNumber.ReadEventData.NegativeResponse.SelectExtendedDataRecord.Error.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportUserDefMemoryDTCExtDataRecordByDTCNumber.ReadEventData.NegativeResponse.GetSizeOfExtendedDataRecordSelection.Error.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportUserDefMemoryDTCExtDataRecordByDTCNumber.PageBufferEnabled.ReadEventData.NegativeResponse.GetNextExtendedDataRecord.Error.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.EB.UDSServices.ReadDTCInformation.reportUserDefMemoryDTCExtDataRecordByDTCNumber.PageBufferDisabled.ReadEventData.NegativeResponse.GetSizeOfExtendedDataRecordSelection.BufferFull.UnlockDTCRecordUpdate,1 */

    /* !LINKSTO Dcm.UDSServices.ReadGenericInformation.reportGenericSnapshotByDTCNumber.ReadEventData.PositiveResponse.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadGenericInformation.reportGenericSnapshotByDTCNumber.ReadEventData.Cancel.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadGenericInformation.reportGenericSnapshotByDTCNumber.ReadEventData.NegativeResponse.SelectExtendedDataRecord.Error.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadGenericInformation.reportGenericSnapshotByDTCNumber.ReadEventData.NegativeResponse.GetSizeOfExtendedDataRecordSelection.Error.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadGenericInformation.reportGenericSnapshotByDTCNumber.PageBufferEnabled.ReadEventData.NegativeResponse.GetNextExtendedDataRecord.Error.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.EB.UDSServices.ReadGenericInformation.reportGenericSnapshotByDTCNumber.PageBufferDisabled.ReadEventData.NegativeResponse.GetSizeOfExtendedDataRecordSelection.BufferFull.UnlockDTCRecordUpdate,1 */

    /* !LINKSTO Dcm.UDSServices.ReadGenericInformation.reportGenericExtendedDataByDTCNumber.ReadEventData.PositiveResponse.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadGenericInformation.reportGenericExtendedDataByDTCNumber.ReadEventData.Cancel.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadGenericInformation.reportGenericExtendedDataByDTCNumber.ReadEventData.NegativeResponse.SelectExtendedDataRecord.Error.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadGenericInformation.reportGenericExtendedDataByDTCNumber.ReadEventData.NegativeResponse.GetSizeOfExtendedDataRecordSelection.Error.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.UDSServices.ReadGenericInformation.reportGenericExtendedDataByDTCNumber.PageBufferEnabled.ReadEventData.NegativeResponse.GetNextExtendedDataRecord.Error.UnlockDTCRecordUpdate,1*/
    /* !LINKSTO Dcm.EB.UDSServices.ReadGenericInformation.reportGenericExtendedDataByDTCNumber.PageBufferDisabled.ReadEventData.NegativeResponse.GetSizeOfExtendedDataRecordSelection.BufferFull.UnlockDTCRecordUpdate,1 */

    ResultCodeFromDEM = Dem_EnableDTCRecordUpdate(ClientId);
    if( E_OK == ResultCodeFromDEM )
    {
      (*ControlRecordUpdate) = DCM_RECORDUPDATE_ENABLED;
      ResultFromEnableRecordUpdate = DCM_FACADE_OK;
    }
    else
    {
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.EnableDTCRecordUpdate.E_NOT_OK.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.EnableDTCRecordUpdate.E_NOT_OK.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.EnableDTCRecordUpdate.E_NOT_OK.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.EnableDTCRecordUpdate.E_NOT_OK.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.EnableDTCRecordUpdate.E_NOT_OK.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.EnableDTCRecordUpdate.E_NOT_OK.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.EnableDTCRecordUpdate.E_NOT_OK.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.EnableDTCRecordUpdate.E_NOT_OK.DET,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.EnableDTCRecordUpdate.E_NOT_OK.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.EnableDTCRecordUpdate.E_NOT_OK.DET,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.EnableDTCRecordUpdate.E_NOT_OK.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.EnableDTCRecordUpdate.E_NOT_OK.DET,1 */

      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.EnableDTCRecordUpdate.DEM_BUSY.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.EnableDTCRecordUpdate.DEM_BUSY.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.EnableDTCRecordUpdate.DEM_BUSY.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.EnableDTCRecordUpdate.DEM_BUSY.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.EnableDTCRecordUpdate.DEM_BUSY.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.EnableDTCRecordUpdate.DEM_BUSY.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.EnableDTCRecordUpdate.DEM_BUSY.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.EnableDTCRecordUpdate.DEM_BUSY.DET,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.EnableDTCRecordUpdate.DEM_BUSY.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.EnableDTCRecordUpdate.DEM_BUSY.DET,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.EnableDTCRecordUpdate.DEM_BUSY.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.EnableDTCRecordUpdate.DEM_BUSY.DET,1 */

      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.EnableDTCRecordUpdate.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.EnableDTCRecordUpdate.UnsupportedError.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.EnableDTCRecordUpdate.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.EnableDTCRecordUpdate.UnsupportedError.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.EnableDTCRecordUpdate.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.EnableDTCRecordUpdate.UnsupportedError.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.EnableDTCRecordUpdate.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.EnableDTCRecordUpdate.UnsupportedError.DET,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.EnableDTCRecordUpdate.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.EnableDTCRecordUpdate.UnsupportedError.DET,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.EnableDTCRecordUpdate.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.EnableDTCRecordUpdate.UnsupportedError.DET,1 */

      /* In case of DEM_BUSY, E_NOT_OK to avoid code duplicate or
         when Enabling the DTC Record update fails */
      ResultFromEnableRecordUpdate = DCM_FACADE_NOT_OK;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      DCM_DET_REPORT_ERROR(DCM_SVCID_ENABLEDTCRECORDUPDATE, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
    }
  }
  return ResultFromEnableRecordUpdate;
}

/* Auxiliary function to process response from DEM*/
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_ProcessDEMResponseFromSelectData
(
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext,
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
    uint8 ResultCodeFromDEM
)
{
  Std_ReturnType ResultFromOperation = DCM_FACADE_NOT_OK;
  /* Deviation MISRAC2012-4 */
  switch (ResultCodeFromDEM)
  {
    case DEM_WRONG_DTC:
    case DEM_WRONG_DTCORIGIN:
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.SelectExtendedDataRecord.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.SelectExtendedDataRecord.DEM_WRONG_DTCORIGIN,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.SelectExtendedDataRecord.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.SelectExtendedDataRecord.DEM_WRONG_DTCORIGIN,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.SelectFreezeFrameData.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.SelectFreezeFrameData.DEM_WRONG_DTCORIGIN,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.SelectFreezeFrameData.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.SelectFreezeFrameData.DEM_WRONG_DTCORIGIN,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SelectExtendedDataRecord.0x30.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SelectExtendedDataRecord.0x30.DEM_WRONG_DTCORIGIN,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SelectExtendedDataRecord.0x30.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SelectExtendedDataRecord.0x30.DEM_WRONG_DTCORIGIN,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SelectExtendedDataRecord.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SelectExtendedDataRecord.DEM_WRONG_DTCORIGIN,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SelectFreezeFrameData.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SelectFreezeFrameData.DEM_WRONG_DTCORIGIN,1 */
      ResultFromOperation = DCM_FACADE_REQUESTOUTOFRANGE;
      break;

    default:
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.SelectExtendedDataRecord.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.SelectExtendedDataRecord.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.SelectFreezeFrameData.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.SelectFreezeFrameData.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SelectExtendedDataRecord.0x30.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SelectExtendedDataRecord.0x30.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SelectExtendedDataRecord.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SelectFreezeFrameData.UnsupportedError.NRC,1 */
      ResultFromOperation = DCM_FACADE_NOT_OK;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      if(DiagnosticDataContext->DataKind == DCM_FACADE_EXTENDED_DATA_RECORDS)
      {
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.SelectExtendedDataRecord.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.SelectExtendedDataRecord.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SelectExtendedDataRecord.0x30.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SelectExtendedDataRecord.0x30.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SelectExtendedDataRecord.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SelectFreezeFrameData.UnsupportedError.DET,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_GETEXTENDEDDATABYDTC, DCM_E_INTERFACE_RETURN_VALUE);
      }
      else
      {
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.SelectFreezeFrameData.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.SelectFreezeFrameData.UnsupportedError.DET,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_GETFREEZEFRAMEDATABYDTC, DCM_E_INTERFACE_RETURN_VALUE);
      }
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
      break;
  }
  return ResultFromOperation;
}

/* Auxiliary function to process response from DEM*/
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_ProcessDEMResponseFromGetData
(
    P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext,
    uint8 ResultCodeFromDEM
)
{
  Std_ReturnType ResultFromOperation = DCM_FACADE_NOT_OK;

  switch (ResultCodeFromDEM)
  {
    case DEM_BUFFER_TOO_SMALL:
#if (DCM_PAGEDBUFFER_ENABLED == STD_OFF)
      /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportDTCSnapshotRecordByDTCNumber.PageBufferDisabled.ReadEventData.NegativeResponse.GetNextFreezeFrameData.Error.UnlockDTCRecordUpdate,1 */
      /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportDTCExtDataRecordByDTCNumber.PageBufferDisabled.ReadEventData.NegativeResponse.GetNextExtendedDataRecord.Error.UnlockDTCRecordUpdate,1 */
      /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.PageBufferDisabled.ReadEventData.NegativeResponse.GetNextFreezeFrameData.Error.UnlockDTCRecordUpdate,1 */
      /* !LINKSTO Dcm.UDSServices.ReadDTCInformation.reportUserDefMemoryDTCExtDataRecordByDTCNumber.PageBufferDisabled.ReadEventData.NegativeResponse.GetNextExtendedDataRecord.Error.UnlockDTCRecordUpdate,1 */
      /* !LINKSTO Dcm.UDSServices.ReadGenericInformation.reportGenericSnapshotByDTCNumber.PageBufferDisabled.ReadEventData.NegativeResponse.GetNextExtendedDataRecord.Error.UnlockDTCRecordUpdate,1 */
      /* !LINKSTO Dcm.UDSServices.ReadGenericInformation.reportGenericExtendedDataByDTCNumber.PageBufferDisabled.ReadEventData.NegativeResponse.GetNextExtendedDataRecord.Error.UnlockDTCRecordUpdate,1 */
      DiagnosticDataContext->ControlRecordUpdate = DCM_RECORDUPDATE_DOENABLE;
#endif /* (DCM_PAGEDBUFFER_ENABLED == STD_OFF) */

      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.DEM_BUFFER_TOO_SMALL,2 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.DEM_BUFFER_TOO_SMALL,2 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.DEM_BUFFER_TOO_SMALL,2 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.DEM_BUFFER_TOO_SMALL,2 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.0x30.DEM_BUFFER_TOO_SMALL,2 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextExtendedDataRecord.0x30.DEM_BUFFER_TOO_SMALL,2 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.DEM_BUFFER_TOO_SMALL,2 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextFreezeFrameData.DEM_BUFFER_TOO_SMALL,2 */
      ResultFromOperation = DCM_FACADE_BUFFERFULL;
      break;

    case DEM_PENDING:
      /* No Data Variance occurs, so no need to update
       * Buffer Size or Obtained Data on EDRContext */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.DEM_PENDING,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.DEM_PENDING,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.DEM_PENDING,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.DEM_PENDING,1 */
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.DEM_PENDING,1 */
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextFreezeFrameData.DEM_PENDING,1 */
      ResultFromOperation = DCM_FACADE_PENDING;
      break;

    case DEM_WRONG_DTC:
    case DEM_WRONG_DTCORIGIN:
      DiagnosticDataContext->ControlRecordUpdate = DCM_RECORDUPDATE_DOENABLE;
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.DEM_WRONG_DTCORIGIN,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.DEM_WRONG_DTCORIGIN,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.DEM_WRONG_DTCORIGIN,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.DEM_WRONG_DTCORIGIN,1 */
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextFreezeFrameData.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextFreezeFrameData.DEM_WRONG_DTCORIGIN,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.0x30.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.0x30.DEM_WRONG_DTCORIGIN,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextExtendedDataRecord.0x30.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextExtendedDataRecord.0x30.DEM_WRONG_DTCORIGIN,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.DEM_WRONG_DTCORIGIN,1 */
      ResultFromOperation = DCM_FACADE_REQUESTOUTOFRANGE;
      break;

    case DEM_NO_SUCH_ELEMENT:
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.DEM_NO_SUCH_ELEMENT.Value,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.DEM_NO_SUCH_ELEMENT.NoValue,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.DEM_NO_SUCH_ELEMENT.Value,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.DEM_NO_SUCH_ELEMENT.NoValue,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.DEM_NO_SUCH_ELEMENT.Value,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.DEM_NO_SUCH_ELEMENT.NoValue,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.DEM_NO_SUCH_ELEMENT.Value,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.DEM_NO_SUCH_ELEMENT.NoValue,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.FreezeFrameData.Supported,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.FreezeFrameData.Supported,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.DEM_NO_SUCH_ELEMENT.Value,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.DEM_NO_SUCH_ELEMENT.NoValue,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextFreezeFrameData.DEM_NO_SUCH_ELEMENT.Value,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextFreezeFrameData.DEM_NO_SUCH_ELEMENT.NoValue,1 */
      DiagnosticDataContext->ControlRecordUpdate = DCM_RECORDUPDATE_DOENABLE;
      DiagnosticDataContext->AmountOfDataToWriteOnResponse =
            DiagnosticDataContext->AmountOfDataStoredOnCompleteResponse;
      ResultFromOperation = DCM_FACADE_OK;
      break;

    default:
      DiagnosticDataContext->ControlRecordUpdate = DCM_RECORDUPDATE_DOENABLE;
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.E_NOT_OK.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.E_NOT_OK.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.E_NOT_OK.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.E_NOT_OK.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.0x30.E_NOT_OK.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextExtendedDataRecord.0x30.E_NOT_OK.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.E_NOT_OK.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextFreezeFrameData.E_NOT_OK.NRC,1 */

      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.0x30.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextExtendedDataRecord.0x30.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextFreezeFrameData.UnsupportedError.NRC,1 */
      ResultFromOperation = DCM_FACADE_NOT_OK;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      if(DiagnosticDataContext->DataKind == DCM_FACADE_EXTENDED_DATA_RECORDS)
      {
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.0x30.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextExtendedDataRecord.0x30.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextFreezeFrameData.E_NOT_OK.DET,1 */

        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetNextExtendedDataRecord.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.0x30.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextExtendedDataRecord.0x30.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextFreezeFrameData.UnsupportedError.DET,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_GETEXTENDEDDATABYDTC, DCM_E_INTERFACE_RETURN_VALUE);
      }
      else
      {
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.E_NOT_OK.DET,1 */

        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetNextFreezeFrameData.UnsupportedError.DET,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_GETFREEZEFRAMEDATABYDTC, DCM_E_INTERFACE_RETURN_VALUE);
      }
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
      break;
  }
  return ResultFromOperation;
}

/* Auxiliary function to process response from DEM*/
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_ProcessDEMResponseFromGetSize
(
#if (DCM_DEV_ERROR_DETECT == STD_ON)
    uint8 DataKind,
#endif
    uint8 ResultCodeFromDEM
)
{
  Std_ReturnType ResultFromOperation = DCM_FACADE_NOT_OK;
  switch (ResultCodeFromDEM)
  {
    case DEM_WRONG_DTC:
    case DEM_WRONG_DTCORIGIN:
    case DEM_NO_SUCH_ELEMENT:
    {
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetSizeOfExtendedDataRecordSelection.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetSizeOfExtendedDataRecordSelection.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetSizeOfFreezeFrameSelection.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetSizeOfFreezeFrameSelection.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetSizeOfExtendedDataRecordSelection.DEM_WRONG_DTC,1 */
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetSizeOfFreezeFrameSelection.DEM_WRONG_DTC,1 */

      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetSizeOfExtendedDataRecordSelection.DEM_WRONG_DTCORIGIN,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetSizeOfExtendedDataRecordSelection.DEM_WRONG_DTCORIGIN,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetSizeOfFreezeFrameSelection.DEM_WRONG_DTCORIGIN,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetSizeOfFreezeFrameSelection.DEM_WRONG_DTCORIGIN,1 */
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetSizeOfExtendedDataRecordSelection.DEM_WRONG_DTCORIGIN,1 */
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetSizeOfFreezeFrameSelection.DEM_WRONG_DTCORIGIN,1 */

      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetSizeOfExtendedDataRecordSelection.DEM_NO_SUCH_ELEMENT,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetSizeOfExtendedDataRecordSelection.DEM_NO_SUCH_ELEMENT,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetSizeOfFreezeFrameSelection.DEM_NO_SUCH_ELEMENT,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetSizeOfFreezeFrameSelection.DEM_NO_SUCH_ELEMENT,1 */
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetSizeOfExtendedDataRecordSelection.DEM_NO_SUCH_ELEMENT,1 */
      /* !LINKSTO Dcm.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetSizeOfFreezeFrameSelection.DEM_NO_SUCH_ELEMENT,1 */

      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.FreezeFrameData.NotSupported,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.FreezeFrameData.NotSupported,1 */
      ResultFromOperation = DCM_FACADE_REQUESTOUTOFRANGE;
    }
    break;
    default:
    {
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetSizeOfExtendedDataRecordSelection.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetSizeOfExtendedDataRecordSelection.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetSizeOfFreezeFrameSelection.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetSizeOfFreezeFrameSelection.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetSizeOfExtendedDataRecordSelection.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetSizeOfFreezeFrameSelection.UnsupportedError.NRC,1 */
      ResultFromOperation = DCM_FACADE_NOT_OK;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      if(DataKind == DCM_FACADE_EXTENDED_DATA_RECORDS)
      {
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCExtDataRecordByDTCNumber.GetSizeOfExtendedDataRecordSelection.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.GetSizeOfExtendedDataRecordSelection.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetSizeOfExtendedDataRecordSelection.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetSizeOfFreezeFrameSelection.UnsupportedError.DET,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_GETEXTENDEDDATABYDTC, DCM_E_INTERFACE_RETURN_VALUE);
      }
      else
      {
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.GetSizeOfFreezeFrameSelection.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.GetSizeOfFreezeFrameSelection.UnsupportedError.DET,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_GETFREEZEFRAMEDATABYDTC, DCM_E_INTERFACE_RETURN_VALUE);
      }
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
    }
    break;
  }
  return ResultFromOperation;
}

#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_ReadStatusIndicator_Execute(
    Dcm_DemClientIdType ClientId,
    P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext,
    P2VAR(Dcm_GetSI30ContextType, AUTOMATIC, DCM_VAR) GetSI30Context,
    P2VAR(boolean, AUTOMATIC, DCM_VAR) ReportErrorFromDEM)
{
  Std_ReturnType ResultCodeFromExecute = DCM_FACADE_NOT_OK;
  Std_ReturnType SelectEventDataResult = DCM_FACADE_NOT_OK;

  if (DiagnosticDataContext->ServiceID == DCM_DSP_SERVICE_0XAF)
  {
    Std_ReturnType ResultCodeFromGetStatusIndicator = DCM_FACADE_NOT_OK;

    if((DCM_NOT_OBTAINED == GetSI30Context->ReportOnSelectStatusIndicator)&&
       (FALSE == *ReportErrorFromDEM))
    {
      SelectEventDataResult = Dcm_DemFacade_SelectEventData(ClientId,
                                                            GetSI30Context,
                                                            DiagnosticDataContext);
      if(DCM_FACADE_OK != SelectEventDataResult)
      {
        ResultCodeFromExecute = SelectEventDataResult;
        *ReportErrorFromDEM = TRUE;
      }
    }

    if((DCM_NOT_OBTAINED == GetSI30Context->StatusOfSI30ForCurrentDTC)&&
       (DCM_OBTAINED == GetSI30Context->ReportOnSelectStatusIndicator)&&
       (FALSE == *ReportErrorFromDEM))
    {
      /* ---- STATUS INDICATOR (Service 0xAF only) ------------------------------- */
      ResultCodeFromGetStatusIndicator = Dcm_DemFacade_GetStatusIndicator(ClientId,
                                                                          DiagnosticDataContext,
                                                                          GetSI30Context);
      if(ResultCodeFromGetStatusIndicator != DCM_FACADE_OK)
      {
        ResultCodeFromExecute = ResultCodeFromGetStatusIndicator;
        *ReportErrorFromDEM = TRUE;
      }
    }
  }
  return ResultCodeFromExecute;
}
#endif /* #if(DCM_DSP_USE_SERVICE_0XAF == STD_ON) */
#endif /* #if (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) */


#if ((DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) || (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON))
/* Data supplier for the DTC Status Availability Mask */
/* !LINKSTO Dcm.Dsn.IB.DemFacade.GetDTCStatusAvailabilityMask,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_DTCStatusAvailabilityMask
(
 const Dcm_DemClientIdType ClientId,
 Dcm_DataSupplierOperationType Operation,
 P2VAR(uint8, AUTOMATIC, DCM_VAR) DataBuffer,
 P2VAR(Dcm_MsgLenType, AUTOMATIC, DCM_VAR) Size
)
{
 Std_ReturnType ResultCodeFromExecute = DCM_FACADE_NOT_OK;
 Std_ReturnType ResultCodeFromDem = DCM_FACADE_NOT_OK;
 P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ServiceContext = Dcm_Dsp_DemFacade_GetClient(ClientId);

 switch (Operation)
 {
   case DCM_DATA_SUPPLIER_SETUP:
     /* Data are get synchronous, no context required */
     ResultCodeFromExecute = DCM_FACADE_OK;
     break;
   case DCM_DATA_SUPPLIER_GET_SIZE:
     *Size = 1U;
     ResultCodeFromExecute = DCM_FACADE_OK;
     break;
   case DCM_DATA_SUPPLIER_GET_DATA:
     /* At this point should be a length check!
      * But this data is only respond as third or fourth byte;
      * because a response buffer is at least 8 byte long
      * a DCM_E_RESPONSETOOLONG will never reported */
     DCM_PRECONDITION_ASSERT((*Size>=1), DCM_SID_READ_DTC_INFORMATION);

     /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCByStatusMask.InitialDemAPI,1 */
     /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCByStatusMask.InitialDemAPI,1 */
     /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSupportedDTC.InitialDemAPI,1 */
     /* !LINKSTO Dcm.EB.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.InitialDemAPI,1 */
     /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCWithPermanentStatus.InitialDemAPI,1 */
     /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.InitialDemAPI,1 */
     /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
     ResultCodeFromDem = Dem_GetDTCStatusAvailabilityMask(ClientId, DataBuffer);
     /* !LINKSTO SWS_Dcm_00007,1 */
     if (E_OK == ResultCodeFromDem)
     {
       /* optimization: mask RequestedStatusBits with the availability status mask */
       ServiceContext->RequestedStatusBits &= *DataBuffer;
       ResultCodeFromExecute = DCM_FACADE_OK;
       *Size = 1U;
     }
     else if(E_NOT_OK == ResultCodeFromDem)
     {
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCBySeverityMaskRecord.GetDTCStatusAvailabilityMask.E_NOT_OK,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCBySeverityMaskRecord.GetDTCStatusAvailabilityMask.E_NOT_OK,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetDTCStatusAvailabilityMask.E_NOT_OK,1 */
       ResultCodeFromExecute = DCM_FACADE_REQUESTOUTOFRANGE;
     }
     else
     {
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCBySeverityMaskRecord.GetDTCStatusAvailabilityMask.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCBySeverityMaskRecord.GetDTCStatusAvailabilityMask.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetDTCStatusAvailabilityMask.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCBySeverityMaskRecord.GetDTCStatusAvailabilityMask.UnsupportedError.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCBySeverityMaskRecord.GetDTCStatusAvailabilityMask.UnsupportedError.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.GetDTCStatusAvailabilityMask.UnsupportedError.DET,1 */
       ResultCodeFromExecute = DCM_FACADE_INVALID_VALUE;
     }
     break;
   /* Because of this data supplier is synchronous, follow case is not needed:
   case DCM_DATA_SUPPLIER_CANCEL: */
   /* CHECK: NOPARSE */
   default:
     /* Defensive branch. Default case at the end of the switch statement */
     DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
     break;
   /* CHECK: PARSE */
 }
 return ResultCodeFromExecute;
}
#endif /* #if ((DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) || (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON)) */

#if (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON)

/* Setup function for obtaining the Number of filtered DTC from DEM */
STATIC FUNC(void, DCM_CODE) Dcm_DemFacade_NumberOfFilteredDTC_Setup
(
  P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) NumberOfFilteredDTCResponseBuffer,
  Dcm_MsgLenType AvailableBufferSize
)
{
  /* initialize pointer where response(s) form DEM will be stored */
  ClientContextPtr->DiagnosticDataContext.ResponseBuffer = NumberOfFilteredDTCResponseBuffer;
  ClientContextPtr->DiagnosticDataContext.BufferSize = AvailableBufferSize;
  ClientContextPtr->DiagnosticDataContext.SubServiceId = ClientContextPtr->SubServiceID;
  ClientContextPtr->DiagnosticDataContext.RequestedStatusMask = ClientContextPtr->RequestedStatusMask;
  ClientContextPtr->DiagnosticDataContext.DTCStatusMask = 0x00U;
  ClientContextPtr->DiagnosticDataContext.DTCKind = ClientContextPtr->DataKind;
  ClientContextPtr->DiagnosticDataContext.DTCFormat = ClientContextPtr->DTCFormat;
  ClientContextPtr->DiagnosticDataContext.StoredDTCs = 0U;
  ClientContextPtr->DiagnosticDataContext.FilterWithSeverity = FALSE;
  ClientContextPtr->DiagnosticDataContext.DTCSeverityMask = DEM_SEVERITY_NO_SEVERITY;
  ClientContextPtr->DiagnosticDataContext.AmountOfDataStoredOnCompleteResponse = 0U;
  ClientContextPtr->DiagnosticDataContext.AmountOfDataStoredOnResponseOnCurrentRun = 0U;
  ClientContextPtr->DiagnosticDataContext.AmountOfDataToWriteOnResponse = 0U;
  ClientContextPtr->DiagnosticDataContext.NextPossitionToFillOnResponseBuffer = 0U;
  ClientContextPtr->DiagnosticDataContext.ReportOnReadOfNumberOfFilteredDtc = DCM_NOT_OBTAINED;
  /* The read of translation type can be made dependent on the requesting sub-service */
  ClientContextPtr->DiagnosticDataContext.RequestOfReadOfTranslationType = DCM_REQUIRED;
  ClientContextPtr->DiagnosticDataContext.ReportOnGetNumberOfFilters = DCM_NOT_COMPLETED;
  ClientContextPtr->DiagnosticDataContext.ReportOnSetDTCFilter = DCM_NOT_OBTAINED;
  switch (ClientContextPtr->SubServiceID)
  {
    case DCM_DSP_READDTCINFORMATION_0x01_ID:
      ClientContextPtr->DiagnosticDataContext.FilterForFaultDetectionCounter = FALSE;
      break;
    /* !LINKSTO Dcm.ReadDTCInfo.reportNumberOfDTCBySeverityMaskRecord.Dem_SetDTCFilter.Parameters,1 */
    case DCM_DSP_READDTCINFORMATION_0x07_ID:
    case DCM_DSP_READDTCINFORMATION_0x08_ID:
      ClientContextPtr->DiagnosticDataContext.FilterForFaultDetectionCounter = FALSE;
      ClientContextPtr->DiagnosticDataContext.FilterWithSeverity = TRUE;
#if ( (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x07 == STD_ON) || \
      (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08 == STD_ON) )
      ClientContextPtr->DiagnosticDataContext.DTCSeverityMask = ClientContextPtr->RequestedSeverityMask;
#endif /* #if ( (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x07 == STD_ON) || \
                (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08 == STD_ON) ) */
      break;
    case DCM_DSP_READDTCINFORMATION_0x02_ID:
    case DCM_DSP_READDTCINFORMATION_0x0A_ID:
    case DCM_DSP_READDTCINFORMATION_0x13_ID:
    case DCM_DSP_READDTCINFORMATION_0x15_ID:
    case DCM_DSP_READDTCINFORMATION_0x17_ID:
      ClientContextPtr->DiagnosticDataContext.FilterForFaultDetectionCounter = FALSE;
      break;
    case DCM_DSP_READDTCINFORMATION_0x14_ID:
      ClientContextPtr->DiagnosticDataContext.FilterForFaultDetectionCounter = TRUE;
      break;
    case DCM_DSP_READDTCINFORMATION_UNSPECIFIC_ID:
      break;
    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_GETFILTEREDDTC);
      break;
    /* CHECK: PARSE */
  }

  return;
}

/* Execute function for obtaining the Number of filtered DTCs from DEM */
/* !LINKSTO Dcm.Dsn.IB.DemFacade.SetFilterObtainNumberOfFilteredDTC,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_SetFilterObtainNumberOfFilteredDTC_Execute
(
    const Dcm_DemClientIdType ClientId,
    P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) GetDataOfFilteredDTCContext
)
{
  P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) SubServiceContext = Dcm_Dsp_DemFacade_GetClient(ClientId);
  Std_ReturnType ResultCodeFromDEM = E_NOT_OK;
  Std_ReturnType ResultCodeFromExecute = DCM_FACADE_NOT_OK;
  Dem_DTCTranslationFormatType DemTranslationFormatType;
  uint16 NumberOfFilteredDTCs = 0U;
  Dcm_MsgLenType CurrentWritePositionOnResponse =
                   GetDataOfFilteredDTCContext->AmountOfDataStoredOnResponseOnCurrentRun;

  if (SubServiceContext->DtcFormatIdState == DCM_DATA_SUPPLIER_REQUIRED)
  {
    /* Call Dem_GetTranslationType API to retrieve the translation type. */
    /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCByStatusMask.GetDTCStatusAvailabilityMask.E_OK,1*/
    /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
    DemTranslationFormatType = Dem_GetTranslationType(ClientId);
    if(DemTranslationFormatType > DEM_DTC_TRANSLATION_J2012DA_FORMAT_04)
    {
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCBySeverityMaskRecord.GetTranslationType.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCBySeverityMaskRecord.GetTranslationType.UnsupportedError.DET,1 */
      ResultCodeFromExecute = DCM_FACADE_INVALID_VALUE;
    }
    else
    {
      /* !LINKSTO Dcm.ReadDTCInfo.reportNumberOfDTCBySeverityMaskRecord.Response,1 */
      GetDataOfFilteredDTCContext->ResponseBuffer[CurrentWritePositionOnResponse] =
        DemTranslationFormatType;
      CurrentWritePositionOnResponse++;
      /* The buffer size decreases by one byte after obtaining the Translation Type. */
      GetDataOfFilteredDTCContext->BufferSize--;
      /* The amount of data written to the answer increases by one byte */
      GetDataOfFilteredDTCContext->AmountOfDataStoredOnResponseOnCurrentRun++;
      SubServiceContext->DtcFormatIdState = DCM_DATA_SUPPLIER_DONE;
    }
  }
  /* If Dem_GetTranslationType returned a valid value, continue. */
  if(ResultCodeFromExecute != DCM_FACADE_INVALID_VALUE)
  {
    if((SubServiceContext->RequestedStatusBits != 0x00U) &&
        ((SubServiceContext->FilteredDtcsCountState == DCM_DATA_SUPPLIER_REQUIRED) ||
         (SubServiceContext->FilteredDtcsState== DCM_DATA_SUPPLIER_REQUIRED)))
    {
#if ( (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x07 == STD_ON) || \
      (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08 == STD_ON) )
      /* If sub-services 0x07 and 0x08 are configured and the active request is one of them,
       * the requested severity mask has to be checked, to be different than 0x00.
       * Otherwise, if the active request is not one of them,Dem_SetDTCFilter can be called.*/
      if( ((SubServiceContext->RequestedSeverityMask != 0x00U) &&
          ((GetDataOfFilteredDTCContext->SubServiceId == DCM_DSP_READDTCINFORMATION_0x07_ID) ||
          (GetDataOfFilteredDTCContext->SubServiceId == DCM_DSP_READDTCINFORMATION_0x08_ID)) ) ||
          ((GetDataOfFilteredDTCContext->SubServiceId != DCM_DSP_READDTCINFORMATION_0x07_ID) &&
          (GetDataOfFilteredDTCContext->SubServiceId != DCM_DSP_READDTCINFORMATION_0x08_ID)) )
      {
#endif /* #if ( (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x07 == STD_ON) || \
        *       (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08 == STD_ON) ) */

        if(GetDataOfFilteredDTCContext->ReportOnSetDTCFilter == DCM_NOT_OBTAINED)
        {
          /* Execute access to DEM with parameters initiated on the _Setup function of the Facade */
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCByStatusMask.GetDTCStatusAvailabilityMask.E_OK,1*/
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSupportedDTC.GetDTCStatusAvailabilityMask.E_OK,1*/
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.GetDTCStatusAvailabilityMask.E_OK,1*/
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCWithPermanentStatus.GetDTCStatusAvailabilityMask.E_OK,1*/
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.GetDTCStatusAvailabilityMask.E_OK,1*/
          /* !LINKSTO Dcm.ReadDTCInfo.reportNumberOfDTCByStatusMask.Dem_SetDTCFilter.Parameters,1 */
          /* !LINKSTO Dcm.ReadDTCInfo.reportDTCByStatusMask.SetDTCFilter,1 */
          /* !LINKSTO Dcm.ReadDTCInfo.reportSupportedDTC.SetDTCFilter,1 */
          /* !LINKSTO Dcm.ReadDTCInfo.reportDTCWithPermanentStatus.SetDTCFilter,1 */
          /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.SetDTCFilter,1 */
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCByStatusMask.GetTranslationType,1 */
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCFaultDetectionCounter.InitialDemAPI,1 */
          /* !LINKSTO Dcm.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.SetDTCFilter,1 */
          /* !LINKSTO Dcm.OBD.0x03Svc.SetDTCFilter,1 */
          /* !LINKSTO Dcm.OBD.0x07Svc.SetDTCFilter,1 */
          /* !LINKSTO Dcm.OBD.0x0ASvc.SetDTCFilter,1 */
          /* !LINKSTO Dcm.EB.OBD.0x03Svc.InitialDemAPI,1 */
          /* !LINKSTO Dcm.EB.OBD.0x07Svc.InitialDemAPI,1 */
          /* !LINKSTO Dcm.EB.OBD.0x0ASvc.InitialDemAPI,1 */
          /* !LINKSTO SWS_Dcm_00519,1 */
          /* !LINKSTO Dcm.OBD.0x03Svc.DTCOrigin,1 */
          /* !LINKSTO Dcm.OBD.0x07Svc.DTCOrigin,1 */
          /* !LINKSTO Dcm.OBD.0x0ASvc.DTCOrigin,1 */
          /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
          /* !LINKSTO Dcm.OBD.0x03Svc.DcmDslProtocolRow.DcmDemClientRef,1 */
          /* !LINKSTO Dcm.OBD.0x07Svc.DcmDslProtocolRow.DcmDemClientRef,1 */
          /* !LINKSTO Dcm.OBD.0x0ASvc.DcmDslProtocolRow.DcmDemClientRef,1 */

          ResultCodeFromDEM = Dem_SetDTCFilter(ClientId,
                                                SubServiceContext->RequestedStatusMask,
                                                SubServiceContext->DTCFormat,
                                                SubServiceContext->MemoryOrigin,
                                                GetDataOfFilteredDTCContext->FilterWithSeverity,
                                                GetDataOfFilteredDTCContext->DTCSeverityMask,
                                                SubServiceContext->FilterForFaultDetectionCounter);
          if(ResultCodeFromDEM == E_OK)
          {
            GetDataOfFilteredDTCContext->ReportOnSetDTCFilter = DCM_OBTAINED;
          }
        }
        if(GetDataOfFilteredDTCContext->ReportOnSetDTCFilter == DCM_OBTAINED)
        {
          if(GetDataOfFilteredDTCContext->ReportOnGetNumberOfFilters == DCM_NOT_OBTAINED)
          {
            /* Execute access to GetNumberOfFilteredDTC DEM Interface  */
            /* !LINKSTO SWS_Dcm_00681,1 */
            /* !LINKSTO Dcm.ReadDTCInfo.reportNumberOfDTCByStatusMask.SetDTCFilter.E_OK,1 */
            /* !LINKSTO Dcm.ReadDTCInfo.reportDTCByStatusMask.SetDTCFilter.E_OK,1 */
            /* !LINKSTO Dcm.ReadDTCInfo.reportSupportedDTC.SetDTCFilter.E_OK,1 */
            /* !LINKSTO Dcm.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.SetDTCFilter.E_OK,1 */
            /* !LINKSTO Dcm.ReadDTCInfo.reportDTCWithPermanentStatus.SetDTCFilter.E_OK,1 */
            /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.SetDTCFilter.E_OK,1 */
            /* !LINKSTO Dcm.ReadDTCInfo.reportDTCFaultDetectionCounter.SetDTCFilter.E_OK,1 */
            /* !LINKSTO Dcm.OBD.0x03Svc.SetDTCFilter.E_OK,1 */
            /* !LINKSTO Dcm.OBD.0x07Svc.SetDTCFilter.E_OK,1 */
            /* !LINKSTO Dcm.OBD.0x0ASvc.SetDTCFilter.E_OK,1 */
            /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
            /* !LINKSTO Dcm.OBD.0x03Svc.DcmDslProtocolRow.DcmDemClientRef,1 */
            /* !LINKSTO Dcm.OBD.0x07Svc.DcmDslProtocolRow.DcmDemClientRef,1 */
            /* !LINKSTO Dcm.OBD.0x0ASvc.DcmDslProtocolRow.DcmDemClientRef,1 */
            ResultCodeFromDEM = Dem_GetNumberOfFilteredDTC(ClientId, &NumberOfFilteredDTCs);
            if (ResultCodeFromDEM == E_OK)
            {
              GetDataOfFilteredDTCContext->NumberOfFilteredDTCs = NumberOfFilteredDTCs;
              GetDataOfFilteredDTCContext->ReportOnGetNumberOfFilters = DCM_OBTAINED;
              /* Obtain the expected amount of data to write on the response */
              GetDataOfFilteredDTCContext->AmountOfDataToWriteOnResponse =
                Dcm_DemFacade_ExpectedAmountOfData(NumberOfFilteredDTCs,
                                                   SubServiceContext->FilteredDtcsState,
                                                      SubServiceContext->DTCFormat,
                                                      GetDataOfFilteredDTCContext->SubServiceId);
              if (SubServiceContext->FilteredDtcsCountState == DCM_DATA_SUPPLIER_REQUIRED)
              {
                SubServiceContext->FilteredDtcsCountState = DCM_DATA_SUPPLIER_IN_PROGRESS;
              }
              /* Signal end of process */
              ResultCodeFromExecute = DCM_FACADE_OK;
            }
            else if (ResultCodeFromDEM == DEM_PENDING)
            {
              /* !LINKSTO Dcm.ReadDTCInfo.reportNumberOfDTCByStatusMask.GetNumberOfFilteredDTC.DEM_PENDING,1 */
              /* !LINKSTO Dcm.ReadDTCInfo.reportDTCByStatusMask.GetNumberOfFilteredDTC.DEM_PENDING,1 */
              /* !LINKSTO Dcm.ReadDTCInfo.reportSupportedDTC.GetNumberOfFilteredDTC.DEM_PENDING,1 */
              /* !LINKSTO Dcm.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.GetNumberOfFilteredDTC.DEM_PENDING,1 */
              /* !LINKSTO Dcm.ReadDTCInfo.reportDTCWithPermanentStatus.GetNumberOfFilteredDTC.DEM_PENDING,1 */
              /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.GetNumberOfFilteredDTC.DEM_PENDING,1 */
              /* !LINKSTO Dcm.ReadDTCInfo.reportDTCFaultDetectionCounter.GetNumberOfFilteredDTC.DEM_PENDING,1 */
              /* !LINKSTO Dcm.ReadDTCInfo.reportNumberOfDTCBySeverityMaskRecord.GetNumberOfFilteredDTC.PENDING,1 */
              /* !LINKSTO Dcm.ReadDTCInfo.reportDTCBySeverityMaskRecord.GetNumberOfFilteredDTC.PENDING,1 */
              /* !LINKSTO Dcm.OBD.0x03Svc.GetNumberOfFilteredDTC.DEM_PENDING,1 */
              /* !LINKSTO Dcm.OBD.0x07Svc.GetNumberOfFilteredDTC.DEM_PENDING,1 */
              /* !LINKSTO Dcm.OBD.0x0ASvc.GetNumberOfFilteredDTC.DEM_PENDING,1 */
              /* Signal Pending to the sub-service handler */
              ResultCodeFromExecute = DCM_FACADE_PENDING;
            }
            else
            {
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCByStatusMask.GetNumberOfFilteredDTC.E_NOT_OK.NRC,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCByStatusMask.GetNumberOfFilteredDTC.E_NOT_OK.DET,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCByStatusMask.GetNumberOfFilteredDTC.E_NOT_OK.NRC,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCByStatusMask.GetNumberOfFilteredDTC.E_NOT_OK.DET,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSupportedDTC.GetNumberOfFilteredDTC.E_NOT_OK.NRC,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSupportedDTC.GetNumberOfFilteredDTC.E_NOT_OK.DET,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.GetNumberOfFilteredDTC.E_NOT_OK.NRC,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.GetNumberOfFilteredDTC.E_NOT_OK.DET,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCWithPermanentStatus.GetNumberOfFilteredDTC.E_NOT_OK.NRC,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCWithPermanentStatus.GetNumberOfFilteredDTC.E_NOT_OK.DET,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.GetNumberOfFilteredDTC.E_NOT_OK.NRC,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.GetNumberOfFilteredDTC.E_NOT_OK.DET,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCFaultDetectionCounter.GetNumberOfFilteredDTC.E_NOT_OK.NRC,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCFaultDetectionCounter.GetNumberOfFilteredDTC.E_NOT_OK.DET,1 */
              /* !LINKSTO Dcm.EB.OBD.0x03Svc.GetNumberOfFilteredDTC.E_NOT_OK.DET,1 */
              /* !LINKSTO Dcm.EB.OBD.0x07Svc.GetNumberOfFilteredDTC.E_NOT_OK.DET,1 */
              /* !LINKSTO Dcm.EB.OBD.0x0ASvc.GetNumberOfFilteredDTC.E_NOT_OK.DET,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCByStatusMask.GetNumberOfFilteredDTC.UnsupportedError.NRC,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCByStatusMask.GetNumberOfFilteredDTC.UnsupportedError.DET,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCByStatusMask.GetNumberOfFilteredDTC.UnsupportedError.NRC,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCByStatusMask.GetNumberOfFilteredDTC.UnsupportedError.DET,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSupportedDTC.GetNumberOfFilteredDTC.UnsupportedError.NRC,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSupportedDTC.GetNumberOfFilteredDTC.UnsupportedError.DET,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.GetNumberOfFilteredDTC.UnsupportedError.NRC,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.GetNumberOfFilteredDTC.UnsupportedError.DET,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCWithPermanentStatus.GetNumberOfFilteredDTC.UnsupportedError.NRC,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCWithPermanentStatus.GetNumberOfFilteredDTC.UnsupportedError.DET,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.GetNumberOfFilteredDTC.UnsupportedError.NRC,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.GetNumberOfFilteredDTC.UnsupportedError.DET,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCFaultDetectionCounter.GetNumberOfFilteredDTC.UnsupportedError.NRC,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCFaultDetectionCounter.GetNumberOfFilteredDTC.UnsupportedError.DET,1 */
              /* !LINKSTO Dcm.EB.OBD.0x03Svc.GetNumberOfFilteredDTC.UnsupportedError.DET,1 */
              /* !LINKSTO Dcm.EB.OBD.0x07Svc.GetNumberOfFilteredDTC.UnsupportedError.DET,1 */
              /* !LINKSTO Dcm.EB.OBD.0x0ASvc.GetNumberOfFilteredDTC.UnsupportedError.DET,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCBySeverityMaskRecord.GetNumberOfFilteredDTC.E_NOT_OK.NRC,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCBySeverityMaskRecord.GetNumberOfFilteredDTC.E_NOT_OK.NRC,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCBySeverityMaskRecord.GetNumberOfFilteredDTC.UnsupportedError.NRC,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCBySeverityMaskRecord.GetNumberOfFilteredDTC.UnsupportedError.NRC,1 */
              ResultCodeFromExecute = DCM_FACADE_NOT_OK;
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCBySeverityMaskRecord.GetNumberOfFilteredDTC.UnsupportedError.DET,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCBySeverityMaskRecord.GetNumberOfFilteredDTC.UnsupportedError.DET,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCBySeverityMaskRecord.GetNumberOfFilteredDTC.E_NOT_OK.DET,1 */
              /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCBySeverityMaskRecord.GetNumberOfFilteredDTC.E_NOT_OK.DET,1 */
    #if (DCM_DEV_ERROR_DETECT == STD_ON)
              DCM_DET_REPORT_ERROR(DCM_SVCID_GETFILTEREDDTC, DCM_E_INTERFACE_RETURN_VALUE);
    #endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
            }
          }
        }
        else if (ResultCodeFromDEM == E_NOT_OK )/* Wrong Filter */
        {
          /* !LINKSTO Dcm.ReadDTCInfo.reportNumberOfDTCByStatusMask.SetDTCFilter.E_NOT_OK,1 */
          /* !LINKSTO Dcm.ReadDTCInfo.reportDTCByStatusMask.SetDTCFilter.E_NOT_OK,1 */
          /* !LINKSTO Dcm.ReadDTCInfo.reportSupportedDTC.SetDTCFilter.E_NOT_OK,1 */
          /* !LINKSTO Dcm.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.SetDTCFilter.E_NOT_OK,1 */
          /* !LINKSTO Dcm.ReadDTCInfo.reportDTCWithPermanentStatus.SetDTCFilter.E_NOT_OK,1 */
          /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.SetDTCFilter.E_NOT_OK,1 */
          /* !LINKSTO Dcm.ReadDTCInfo.reportDTCFaultDetectionCounter.SetDTCFilter.E_NOT_OK,1 */
          /* !LINKSTO Dcm.OBD.0x03Svc.SetDTCFilter.E_NOT_OK,1 */
          /* !LINKSTO Dcm.OBD.0x07Svc.SetDTCFilter.E_NOT_OK,1 */
          /* !LINKSTO Dcm.OBD.0x0ASvc.SetDTCFilter.E_NOT_OK,1 */
          ResultCodeFromExecute = DCM_FACADE_REQUESTOUTOFRANGE;
        }
        else
        {
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCByStatusMask.SetDTCFilter.UnsupportedError.NRC,1 */
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCByStatusMask.SetDTCFilter.UnsupportedError.DET,1 */
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCByStatusMask.SetDTCFilter.UnsupportedError.NRC,1 */
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCByStatusMask.SetDTCFilter.UnsupportedError.DET,1 */
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSupportedDTC.SetDTCFilter.UnsupportedError.NRC,1 */
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSupportedDTC.SetDTCFilter.UnsupportedError.DET,1 */
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.SetDTCFilter.UnsupportedError.NRC,1 */
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.SetDTCFilter.UnsupportedError.DET,1 */
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCWithPermanentStatus.SetDTCFilter.UnsupportedError.NRC,1 */
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCWithPermanentStatus.SetDTCFilter.UnsupportedError.DET,1 */
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.SetDTCFilter.UnsupportedError.NRC,1 */
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.SetDTCFilter.UnsupportedError.DET,1 */
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCFaultDetectionCounter.SetDTCFilter.UnsupportedError.NRC,1 */
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCFaultDetectionCounter.SetDTCFilter.UnsupportedError.DET,1 */
          /* !LINKSTO Dcm.EB.OBD.0x03Svc.SetDTCFilter.UnsupportedError.DET,1 */
          /* !LINKSTO Dcm.EB.OBD.0x07Svc.SetDTCFilter.UnsupportedError.DET,1 */
          /* !LINKSTO Dcm.EB.OBD.0x0ASvc.SetDTCFilter.UnsupportedError.DET,1 */
          ResultCodeFromExecute = DCM_FACADE_NOT_OK;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
          DCM_DET_REPORT_ERROR(DCM_SVCID_GETFILTEREDDTC, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
        }
#if ( (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x07 == STD_ON) || \
      (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08 == STD_ON) )
      }
      else
      {
        /* Because DTCSeverityMask is equal to 0x00, same approach as for DTCStatusMask equal to 0x00 shall be used. */
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCBySeverityMaskRecord.DTCSeverityMask.0x00,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportNumberOfDTCBySeverityMaskRecord.DTCSeverityMask.0x00,1 */
        if (SubServiceContext->FilteredDtcsCountState == DCM_DATA_SUPPLIER_REQUIRED)
        {
          SubServiceContext->FilteredDtcsCountState = DCM_DATA_SUPPLIER_IN_PROGRESS;
        }
        /* Signal end of process */
        ResultCodeFromExecute = DCM_FACADE_OK;
      }
#endif /* #if ( (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x07 == STD_ON) || \
        *       (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08 == STD_ON) ) */
    }
    else
    {
      /* Because of requested DTCStatusMask equal 0x00,
       * use no DEM interface and report the count as 0 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCBySeverityMaskRecord.DTCStatusMask.0x00,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportNumberOfDTCBySeverityMaskRecord.DTCStatusMask.0x00,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportNumberOfDTCByStatusMask.DTCStatusMask.0x00,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCByStatusMask.DTCStatusMask.0x00,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.DTCStatusMask.0x00,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.DTCStatusMask.0x00,1 */
      if (SubServiceContext->FilteredDtcsCountState == DCM_DATA_SUPPLIER_REQUIRED)
      {
        SubServiceContext->FilteredDtcsCountState = DCM_DATA_SUPPLIER_IN_PROGRESS;
      }
      /* Signal end of process */
      ResultCodeFromExecute = DCM_FACADE_OK;
    }
    if (SubServiceContext->FilteredDtcsCountState == DCM_DATA_SUPPLIER_IN_PROGRESS)
    {
      /* For these sub-services, report the Number of Filtered DTCs
       * directly on the response buffer. */
      if (SubServiceContext->DTCFormat == DEM_DTC_FORMAT_UDS)
      {
        /* !LINKSTO Dcm.ReadDTCInfo.reportNumberOfDTCByStatusMask.Response,1 */
        /* Update DTCCountHighByte */
        GetDataOfFilteredDTCContext->ResponseBuffer[CurrentWritePositionOnResponse] =
          (uint8)(NumberOfFilteredDTCs >> 8U);
        CurrentWritePositionOnResponse++;
        GetDataOfFilteredDTCContext->BufferSize--;
        GetDataOfFilteredDTCContext->AmountOfDataStoredOnResponseOnCurrentRun++;
      }
      /* Update DTCCountLowByte respective #OFDTC */
      GetDataOfFilteredDTCContext->ResponseBuffer[CurrentWritePositionOnResponse] =
        (uint8)(NumberOfFilteredDTCs);
      GetDataOfFilteredDTCContext->BufferSize--;
      GetDataOfFilteredDTCContext->AmountOfDataStoredOnResponseOnCurrentRun++;

      /* Update report on obtaining of Number of filtered DTCs */
      GetDataOfFilteredDTCContext->ReportOnReadOfNumberOfFilteredDtc = DCM_OBTAINED;

        SubServiceContext->FilteredDtcsCountState = DCM_DATA_SUPPLIER_DONE;
    }
  }

  return ResultCodeFromExecute;
}
STATIC FUNC(Dcm_MsgLenType, DCM_CODE) Dcm_DemFacade_ExpectedAmountOfData(
  uint16 NumberOfFilteredDTCs,
  uint8 FilteredState,
  uint8 DTCFormat,
  uint8 SubServiceId
)
{
  uint8 DtcSize = 0U;
  Dcm_MsgLenType AmountOfDataToWrite = 0U;
  if (FilteredState == DCM_DATA_SUPPLIER_REQUIRED)
  {
    switch (DTCFormat)
    {
      case DEM_DTC_FORMAT_UDS:
        if(SubServiceId == DCM_DSP_READDTCINFORMATION_0x08_ID)
        {
          DtcSize = DCM_SIZE_OF_DTC_AND_STATUS_WITH_SEVERITY;
        }
        else
        {
        DtcSize = DCM_SIZE_OF_DTC_AND_STATUS;
        }
        break;
      case DEM_DTC_FORMAT_OBD:
        DtcSize = DCM_SIZE_OF_OBD_DTC;
        break;
      /* CHECK: NOPARSE */
      default:
        /* Defensive branch. Default case at the end of the switch statement */
        DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_GETFILTEREDDTC);
        break;
      /* CHECK: PARSE */
    }
    /* Return the expected amount of data to read from DEM */
    AmountOfDataToWrite = ((Dcm_MsgLenType)NumberOfFilteredDTCs * (Dcm_MsgLenType)DtcSize);
  }
  return AmountOfDataToWrite;
}

/* Execute function for obtaining the filtered DTCs from DEM */
/* !LINKSTO Dcm.Dsn.IB.DemFacade.GetFilteredDTC,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_GetFilteredDTC_Execute
(
    const Dcm_DemClientIdType ClientId,
    P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) GetDataOfFilteredDTCContext
)
{
  Std_ReturnType ResultCodeFromExecute = DCM_FACADE_NOT_OK;
  Std_ReturnType ResultDemInterface = E_OK;
  uint16 ExpectedAmountOfFilteredDTCs = GetDataOfFilteredDTCContext->NumberOfFilteredDTCs;
  uint8 DtcStatusMask;
  sint8 DtcFaultDetectionCounter;
  uint8 DtcReportedData = 0U;
  uint32 RequestedDtc = 0U;
  Dcm_MsgLenType CurrentWritePosition =
                   GetDataOfFilteredDTCContext->NextPossitionToFillOnResponseBuffer;
  P2CONST(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) SubServiceContext = Dcm_Dsp_DemFacade_GetClient(ClientId);
  uint8 RequestedDtcSize = DCM_SIZE_OF_DTC_AND_STATUS; /* DEM_DTC_FORMAT_UDS */

  if (GetDataOfFilteredDTCContext->DTCFormat == DEM_DTC_FORMAT_OBD)
  {
    RequestedDtcSize = DCM_SIZE_OF_OBD_DTC;
  }

/* Loop for obtaining Filtered DTCs */
  while ( (ExpectedAmountOfFilteredDTCs > 0U) &&
          ( (ResultDemInterface == E_OK) ||
            (ResultDemInterface == DEM_NO_SUCH_ELEMENT) ) &&
          (GetDataOfFilteredDTCContext->BufferSize >= RequestedDtcSize) )
  {
    if(SubServiceContext->FilterForFaultDetectionCounter == FALSE)
    {
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCByStatusMask.GetNumberOfFilteredDTC.E_OK,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSupportedDTC.GetNumberOfFilteredDTC.E_OK,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.GetNumberOfFilteredDTC.E_OK,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCWithPermanentStatus.GetNumberOfFilteredDTC.E_OK,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.GetNumberOfFilteredDTC.E_OK,1 */
      /* !LINKSTO Dcm.EB.OBD.0x03Svc.GetNumberOfFilteredDTC.E_OK,1 */
      /* !LINKSTO Dcm.EB.OBD.0x07Svc.GetNumberOfFilteredDTC.E_OK,1 */
      /* !LINKSTO Dcm.EB.OBD.0x0ASvc.GetNumberOfFilteredDTC.E_OK,1 */
      /* !LINKSTO Dcm.OBD.0x03Svc.GetNextFilteredDTC,1 */
      /* !LINKSTO Dcm.OBD.0x07Svc.GetNextFilteredDTC,1 */
      /* !LINKSTO Dcm.OBD.0x0ASvc.GetNextFilteredDTC,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
      /* !LINKSTO Dcm.OBD.0x03Svc.DcmDslProtocolRow.DcmDemClientRef,1 */
      /* !LINKSTO Dcm.OBD.0x07Svc.DcmDslProtocolRow.DcmDemClientRef,1 */
      /* !LINKSTO Dcm.OBD.0x0ASvc.DcmDslProtocolRow.DcmDemClientRef,1 */
      ResultDemInterface = Dem_GetNextFilteredDTC(ClientId, &RequestedDtc, &DtcStatusMask);
      DtcReportedData = DtcStatusMask;
    }
    else
    {
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCFaultDetectionCounter.GetNumberOfFilteredDTC.E_OK,1 */
      /* !LINKSTO SWS_Dcm_00465, 1 */
      /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
      ResultDemInterface =
        Dem_GetNextFilteredDTCAndFDC(ClientId, &RequestedDtc, &DtcFaultDetectionCounter);
      DtcReportedData = (uint8) DtcFaultDetectionCounter;
    }

    if (ResultDemInterface == DEM_NO_SUCH_ELEMENT)
    {
#if DCM_PAGEDBUFFER_ENABLED == STD_ON
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCByStatusMask.PageBufferEnabled,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportSupportedDTC.PageBufferEnabled,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.PageBufferEnabled,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCWithPermanentStatus.PageBufferEnabled,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.PageBufferEnabled,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCFaultDetectionCounter.PageBufferEnabled,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCByStatusMask.PageBufferEnabled.DTCZero,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportSupportedDTC.PageBufferEnabled.DTCZero,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.PageBufferEnabled.DTCZero,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCWithPermanentStatus.PageBufferEnabled.DTCZero,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.PageBufferEnabled.DTCZero,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCFaultDetectionCounter.PageBufferEnabled.DTCZero,1 */
      RequestedDtc = 0x000000U;
      DtcReportedData = 0x00U;
#else

      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCFaultDetectionCounter.PageBufferDisabled,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCByStatusMask.PageBufferDisabled,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportSupportedDTC.PageBufferDisabled,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.PageBufferDisabled,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCWithPermanentStatus.PageBufferDisabled,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.PageBufferDisabled,1 */
      GetDataOfFilteredDTCContext->AmountOfDataToWriteOnResponse -=
          (uint32)((uint32)(ExpectedAmountOfFilteredDTCs) * (uint32)(RequestedDtcSize));
      ExpectedAmountOfFilteredDTCs = 0U;
#endif
    }

    if (ResultDemInterface == E_OK
#if DCM_PAGEDBUFFER_ENABLED == STD_ON
        || (ResultDemInterface == DEM_NO_SUCH_ELEMENT)
#endif
       )
    {
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCByStatusMask.Response,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportSupportedDTC.Response,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.Response,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCWithPermanentStatus.Response,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.Response,1 */
      GetDataOfFilteredDTCContext->ResponseBuffer[CurrentWritePosition] =
        (uint8)(RequestedDtc >> 16U);
      CurrentWritePosition++;
      GetDataOfFilteredDTCContext->ResponseBuffer[CurrentWritePosition] =
        (uint8)(RequestedDtc >> 8U);
      CurrentWritePosition++;
      if (GetDataOfFilteredDTCContext->DTCFormat == DEM_DTC_FORMAT_UDS)
      {
        GetDataOfFilteredDTCContext->ResponseBuffer[CurrentWritePosition] =
          (uint8)(RequestedDtc);
        CurrentWritePosition++;
        GetDataOfFilteredDTCContext->ResponseBuffer[CurrentWritePosition] =
          (uint8)(DtcReportedData);
        CurrentWritePosition++;
      }
      GetDataOfFilteredDTCContext->BufferSize -= RequestedDtcSize;
      GetDataOfFilteredDTCContext->AmountOfDataStoredOnResponseOnCurrentRun += RequestedDtcSize;
      ExpectedAmountOfFilteredDTCs--;
    }
  }

  /* Store the new amount of filters expected */
  GetDataOfFilteredDTCContext->NumberOfFilteredDTCs = ExpectedAmountOfFilteredDTCs;
  if ((GetDataOfFilteredDTCContext->BufferSize < RequestedDtcSize)&&
      (ExpectedAmountOfFilteredDTCs != 0U))
  {
    GetDataOfFilteredDTCContext->NextPossitionToFillOnResponseBuffer = 0U;
    ResultCodeFromExecute = DCM_FACADE_BUFFERFULL;
  }
  else
  {
    switch (ResultDemInterface)
    {
      case E_OK:
        GetDataOfFilteredDTCContext->NextPossitionToFillOnResponseBuffer = 0U;
        ResultCodeFromExecute = DCM_FACADE_OK;
        break;

      case DEM_PENDING:
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCByStatusMask.GetNextFilteredDTC.DEM_PENDING,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportSupportedDTC.GetNextFilteredDTC.DEM_PENDING,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.GetNextFilteredDTC.DEM_PENDING,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCWithPermanentStatus.GetNextFilteredDTC.DEM_PENDING,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.GetNextFilteredDTC.DEM_PENDING,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCFaultDetectionCounter.GetNextFilteredDTCAndFDC.DEM_PENDING,1 */
        /* !LINKSTO Dcm.OBD.0x03Svc.GetNextFilteredDTC.DEM_PENDING,1 */
        /* !LINKSTO Dcm.OBD.0x07Svc.GetNextFilteredDTC.DEM_PENDING,1 */
        /* !LINKSTO Dcm.OBD.0x0ASvc.GetNextFilteredDTC.DEM_PENDING,1 */
        GetDataOfFilteredDTCContext->NextPossitionToFillOnResponseBuffer = CurrentWritePosition;
        ResultCodeFromExecute = DCM_FACADE_PENDING;
        break;

      case DEM_NO_SUCH_ELEMENT:
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCByStatusMask.GetNextFilteredDTC.DEM_NO_SUCH_ELEMENT.Value,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCByStatusMask.GetNextFilteredDTC.DEM_NO_SUCH_ELEMENT.NoValue,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportSupportedDTC.GetNextFilteredDTC.DEM_NO_SUCH_ELEMENT.Value,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportSupportedDTC.GetNextFilteredDTC.DEM_NO_SUCH_ELEMENT.NoValue,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.GetNextFilteredDTC.DEM_NO_SUCH_ELEMENT.Value,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.GetNextFilteredDTC.DEM_NO_SUCH_ELEMENT.NoValue,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCWithPermanentStatus.GetNextFilteredDTC.DEM_NO_SUCH_ELEMENT.Value,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCWithPermanentStatus.GetNextFilteredDTC.DEM_NO_SUCH_ELEMENT.NoValue,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.GetNextFilteredDTC.DEM_NO_SUCH_ELEMENT.Value,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.GetNextFilteredDTC.DEM_NO_SUCH_ELEMENT.NoValue,1 */
        /* !LINKSTO SWS_Dcm_01232,1 */
        /* !LINKSTO SWS_Dcm_01233,1 */
        /* !LINKSTO Dcm.OBD.0x03Svc.GetNextFilteredDTC.DEM_NO_SUCH_ELEMENT.Value,1 */
        /* !LINKSTO Dcm.OBD.0x03Svc.GetNextFilteredDTC.DEM_NO_SUCH_ELEMENT.NoValue,1 */
        /* !LINKSTO Dcm.OBD.0x07Svc.GetNextFilteredDTC.DEM_NO_SUCH_ELEMENT.Value,1 */
        /* !LINKSTO Dcm.OBD.0x07Svc.GetNextFilteredDTC.DEM_NO_SUCH_ELEMENT.NoValue,1 */
        /* !LINKSTO Dcm.OBD.0x0ASvc.GetNextFilteredDTC.DEM_NO_SUCH_ELEMENT.Value,1 */
        /* !LINKSTO Dcm.OBD.0x0ASvc.GetNextFilteredDTC.DEM_NO_SUCH_ELEMENT.NoValue,1 */
        GetDataOfFilteredDTCContext->NextPossitionToFillOnResponseBuffer = 0U;
        GetDataOfFilteredDTCContext->NumberOfFilteredDTCs = 0U;
        ResultCodeFromExecute = DCM_FACADE_OK;
        break;

      default:
        /* no filter or wrong filter is set */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCByStatusMask.GetNextFilteredDTC.E_NOT_OK.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCByStatusMask.GetNextFilteredDTC.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSupportedDTC.GetNextFilteredDTC.E_NOT_OK.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSupportedDTC.GetNextFilteredDTC.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.GetNextFilteredDTC.E_NOT_OK.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.GetNextFilteredDTC.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCWithPermanentStatus.GetNextFilteredDTC.E_NOT_OK.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCWithPermanentStatus.GetNextFilteredDTC.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.GetNextFilteredDTC.E_NOT_OK.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.GetNextFilteredDTC.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCFaultDetectionCounter.GetNextFilteredDTCAndFDC.E_NOT_OK.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCFaultDetectionCounter.GetNextFilteredDTCAndFDC.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.OBD.0x03Svc.GetNextFilteredDTC.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.OBD.0x07Svc.GetNextFilteredDTC.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.OBD.0x0ASvc.GetNextFilteredDTC.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCByStatusMask.GetNextFilteredDTC.UnsupportedError.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCByStatusMask.GetNextFilteredDTC.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSupportedDTC.GetNextFilteredDTC.UnsupportedError.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSupportedDTC.GetNextFilteredDTC.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.GetNextFilteredDTC.UnsupportedError.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.GetNextFilteredDTC.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCWithPermanentStatus.GetNextFilteredDTC.UnsupportedError.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCWithPermanentStatus.GetNextFilteredDTC.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.GetNextFilteredDTC.UnsupportedError.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.GetNextFilteredDTC.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCFaultDetectionCounter.GetNextFilteredDTCAndFDC.UnsupportedError.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCFaultDetectionCounter.GetNextFilteredDTCAndFDC.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.OBD.0x03Svc.GetNextFilteredDTC.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.OBD.0x07Svc.GetNextFilteredDTC.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.OBD.0x0ASvc.GetNextFilteredDTC.UnsupportedError.DET,1 */
        ResultCodeFromExecute = DCM_FACADE_NOT_OK;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        DCM_DET_REPORT_ERROR(DCM_SVCID_GETFILTEREDDTC, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
        break;
    }
  }
  return ResultCodeFromExecute;
}

/* Execute function for obtaining the severity filtered DTCs from DEM */
/* !LINKSTO Dcm.Dsn.IB.DemFacade.GetSeverityFilteredDTC,1 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_GetSeverityFilteredDTC_Execute
(
    const Dcm_DemClientIdType ClientId,
    P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) GetDataOfFilteredDTCContext
)
{
  Std_ReturnType ResultCodeFromExecute = DCM_FACADE_NOT_OK;
  Std_ReturnType ResultDemInterface = E_OK;
  uint16 ExpectedAmountOfFilteredDTCs = GetDataOfFilteredDTCContext->NumberOfFilteredDTCs;
  uint8 DtcStatusMask;
  uint32 RequestedDtc = 0U;
  uint8 DTCSeverity = 0U;
  uint8 DTCFunctionalUnit = 0U;

  Dcm_MsgLenType CurrentWritePosition =
                   GetDataOfFilteredDTCContext->NextPossitionToFillOnResponseBuffer;
  uint8 RequestedDtcSize = DCM_SIZE_OF_DTC_AND_STATUS_WITH_SEVERITY; /* DEM_DTC_FORMAT_UDS_WITH_SEVERITY */

/* Loop for obtaining Filtered DTCs */
  while ( (ExpectedAmountOfFilteredDTCs > 0U) &&
          ( (ResultDemInterface == E_OK) ||
            (ResultDemInterface == DEM_NO_SUCH_ELEMENT) ) &&
          (GetDataOfFilteredDTCContext->BufferSize >= RequestedDtcSize) )
  {
    /* !LINKSTO SWS_Dcm_00379,1 */
    /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
    ResultDemInterface = Dem_GetNextFilteredDTCAndSeverity(ClientId, &RequestedDtc, &DtcStatusMask, &DTCSeverity, &DTCFunctionalUnit);

    /* !LINKSTO Dcm.ReadDTCInfo.reportDTCBySeverityMaskRecord.GetNextFilteredDTCAndSeverity.DEM_NO_SUCH_ELEMENT.Value,1 */
    /* !LINKSTO Dcm.ReadDTCInfo.reportDTCBySeverityMaskRecord.GetNextFilteredDTCAndSeverity.DEM_NO_SUCH_ELEMENT.NoValue,1 */
    if (ResultDemInterface == DEM_NO_SUCH_ELEMENT)
    {
      RequestedDtc = 0x0000U;
      DtcStatusMask = 0x00U;
      DTCSeverity = 0x00U;
      DTCFunctionalUnit = 0x00U;
      GetDataOfFilteredDTCContext->AmountOfDataToWriteOnResponse = (uint32) GetDataOfFilteredDTCContext->StoredDTCs * DCM_SIZE_OF_DTC_AND_STATUS_WITH_SEVERITY;
      break;
    }

    if (ResultDemInterface == E_OK)
    {
      /* Store DTC Severity */
      GetDataOfFilteredDTCContext->ResponseBuffer[CurrentWritePosition] = (uint8)(DTCSeverity);
      CurrentWritePosition++;

      /* Store DTC Functional Unit */
      GetDataOfFilteredDTCContext->ResponseBuffer[CurrentWritePosition] = (uint8)(DTCFunctionalUnit);
      CurrentWritePosition++;

      /* Store DTC number*/
      GetDataOfFilteredDTCContext->ResponseBuffer[CurrentWritePosition] = (uint8)(RequestedDtc >> 16U);
      CurrentWritePosition++;
      GetDataOfFilteredDTCContext->ResponseBuffer[CurrentWritePosition] = (uint8)(RequestedDtc >> 8U);
      CurrentWritePosition++;

      GetDataOfFilteredDTCContext->ResponseBuffer[CurrentWritePosition] = (uint8)(RequestedDtc);
      CurrentWritePosition++;

      /* Store status of DTC */
      GetDataOfFilteredDTCContext->ResponseBuffer[CurrentWritePosition] = (uint8)(DtcStatusMask);
      CurrentWritePosition++;

      GetDataOfFilteredDTCContext->BufferSize -= RequestedDtcSize;
      GetDataOfFilteredDTCContext->AmountOfDataStoredOnResponseOnCurrentRun += RequestedDtcSize;
      ExpectedAmountOfFilteredDTCs--;
      GetDataOfFilteredDTCContext->StoredDTCs++;
    }
  }

  /* Store the new amount of filters expected */
  GetDataOfFilteredDTCContext->NumberOfFilteredDTCs = ExpectedAmountOfFilteredDTCs;
  /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCBySeverityMaskRecord.ResponseTooLong.NRC,1 */
  if ((GetDataOfFilteredDTCContext->BufferSize < RequestedDtcSize)&&
      (ExpectedAmountOfFilteredDTCs != 0U))
  {
    GetDataOfFilteredDTCContext->NextPossitionToFillOnResponseBuffer = 0U;
    ResultCodeFromExecute = DCM_FACADE_BUFFERFULL;
  }
  else
  {
    switch (ResultDemInterface)
    {
      case E_OK:
        GetDataOfFilteredDTCContext->NextPossitionToFillOnResponseBuffer = 0U;
        ResultCodeFromExecute = DCM_FACADE_OK;
        break;

      case DEM_PENDING:
        GetDataOfFilteredDTCContext->NextPossitionToFillOnResponseBuffer = CurrentWritePosition;
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCBySeverityMaskRecord.GetNextFilteredDTCAndSeverity.PENDING,1 */
        ResultCodeFromExecute = DCM_FACADE_PENDING;
        break;

      case DEM_NO_SUCH_ELEMENT:
        GetDataOfFilteredDTCContext->NextPossitionToFillOnResponseBuffer = 0U;
        GetDataOfFilteredDTCContext->NumberOfFilteredDTCs = 0U;
        ResultCodeFromExecute = DCM_FACADE_OK;
        break;

      default:
        /* no filter or wrong filter is set */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCBySeverityMaskRecord.GetNextFilteredDTCAndSeverity.E_NOT_OK.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCBySeverityMaskRecord.GetNextFilteredDTCAndSeverity.UnsupportedError.NRC,1 */
        ResultCodeFromExecute = DCM_FACADE_NOT_OK;
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCBySeverityMaskRecord.GetNextFilteredDTCAndSeverity.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCBySeverityMaskRecord.GetNextFilteredDTCAndSeverity.UnsupportedError.DET,1 */
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        DCM_DET_REPORT_ERROR(DCM_SVCID_GETFILTEREDDTC, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
        break;
    }
  }
  return ResultCodeFromExecute;
}

#endif /* #if (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) */

#if ( (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || \
      (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) )
/** \brief Auxiliary function for processing the result from the Event Data fetch operation*/
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ProcessEventDataResult
(
  Dcm_OpStatusType OpStatus,
  Std_ReturnType FacadeReturnCodeGetEventData,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext,
  P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ReadEventDataSubServiceContext
)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  Dcm_NegativeResponseCodeType SetErrorCode;

  switch(FacadeReturnCodeGetEventData)
  {
    case DCM_FACADE_OK:
    {
      /* The amount of data written on the current page */
      ReadEventDataSubServiceContext->AmountOfDataWrittenOnCurrentPage  +=
        DiagnosticDataContext->AmountOfDataStoredOnResponseOnCurrentRun;

      if((OpStatus==DCM_INITIAL)||(OpStatus==DCM_PENDING))
      {

#if ( (DCM_DSP_USE_SERVICE_0XAF == STD_ON) && \
      (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON))
        if((DiagnosticDataContext->ServiceID == DCM_DSP_SERVICE_0XAF)&&
           (DiagnosticDataContext->DataKind == DCM_FACADE_FREEZE_FRAME_DATA))
        {
          if ((DiagnosticDataContext->AmountOfDataToWriteOnResponse == 0U) &&
              (pMsgContext->resDataLen >= DCM_FIVE_BYTES_OF_HEADER_PER_DTC))
          {
            /* Second condition is to ensure that a positive response is sent in case the selected DTC
             * does not have an assigned DTC status (DEM_NO_SUCH_ELEMENT returned from GetStatusOFDTC) */
            /* Discount written bytes for DTC, DTC Status and Status Indicator */
            pMsgContext->resDataLen -= DCM_FIVE_BYTES_OF_HEADER_PER_DTC;
          }
          else
          {
            pMsgContext->resDataLen +=
                (PduLengthType)DiagnosticDataContext->AmountOfDataStoredOnResponseOnCurrentRun;
          }
        }
        else
#endif /* #if ( (DCM_DSP_USE_SERVICE_0XAF == STD_ON) && \
      (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON)) */
        {
          pMsgContext->resDataLen +=
              (PduLengthType)DiagnosticDataContext->AmountOfDataToWriteOnResponse;
        }
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
        /* Update the amount of filled data on the response buffer,
         * after the execution has finished */
        Dcm_DspInternal_UpdateAmountOfFilledDataOnResponseBuffer(pMsgContext->dcmRxPduId,
            ReadEventDataSubServiceContext->AmountOfDataWrittenOnCurrentPage);
#endif/*#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)*/
        /* Signalizes that the process is completed */
        ServiceProcessStatus = DCM_E_DONE;
      }
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
      if(OpStatus==DCM_UPDATE_PAGE)
      {
        /* Indicate to start processing of the page*/
        Dcm_DsdInternal_ProcessPage(
            DiagnosticDataContext->AmountOfDataStoredOnResponseOnCurrentRun);

        /* Flag to indicate that the page buffer can be now disabled */
        ReadEventDataSubServiceContext->PageBufferingFlag = FALSE;

        /* Signalizes that the process is completed */
        ServiceProcessStatus = DCM_E_PROCESS_PAGE;
        ReadEventDataSubServiceContext->FirstPageSent = FALSE;
      }
#endif /*#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)*/

      /* Reset counter of the mount of data written into the response for next processing.*/
      ReadEventDataSubServiceContext->AmountOfDataWrittenOnCurrentPage = 0U;

      break;
    }
    case DCM_FACADE_PENDING:
    {
      /* The amount written in the current page */
      ReadEventDataSubServiceContext->AmountOfDataWrittenOnCurrentPage +=
        DiagnosticDataContext->AmountOfDataStoredOnResponseOnCurrentRun;
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
      /* Update the amount of filled data on the response buffer,
      after the execution has finished */
      Dcm_DspInternal_UpdateAmountOfFilledDataOnResponseBuffer(pMsgContext->dcmRxPduId,
          ReadEventDataSubServiceContext->AmountOfDataWrittenOnCurrentPage);
#endif /*#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)*/
      ServiceProcessStatus = DCM_E_PENDING;
      break;
    }
    case DCM_FACADE_BUFFERFULL:
    {
#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
      if (ReadEventDataSubServiceContext->ServiceID == DCM_DSP_SERVICE_0XAF)
      {
        /* Service 0xAF does not offer support for Page Buffering */
        /* !LINKSTO Dcm.EB.UDSServices.ReadGenericInformation.reportGenericSnapshotByDTCNumber.ReadEventData.NegativeResponse.GetSizeOfExtendedDataRecordSelection.BufferFull.ResponseTooLong,1 */
        /* !LINKSTO Dcm.EB.UDSServices.ReadGenericInformation.reportGenericExtendedDataByDTCNumber.ReadEventData.NegativeResponse.GetSizeOfExtendedDataRecordSelection.BufferFull.ResponseTooLong,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_RESPONSETOOLONG);
        /* In the special case of 0xAF,
         * the Not OK response will be translated to DONE on the service handler
         * in order to avoid premature interruption of the read cycle.*/
        ServiceProcessStatus = DCM_E_NOT_OK;
      }
      else
#endif
      {
        ServiceProcessStatus = Dcm_DemFacade_ProcessServiceIDBufferfull(OpStatus,
                                                                        pMsgContext,
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
                                                                        DiagnosticDataContext,
#endif /* (DCM_PAGEDBUFFER_ENABLED == STD_ON) */
                                                                        ReadEventDataSubServiceContext);
      }
      break;
    }
    case DCM_FACADE_WRONGCONDITION:
    {
      /* For now no OBD service throw this error */
      DCM_PRECONDITION_ASSERT(DiagnosticDataContext->DTCFormat == DEM_DTC_FORMAT_UDS,
                              DCM_SID_READ_DTC_INFORMATION);
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
      ServiceProcessStatus = DCM_E_DONE;
      break;
    }
    case DCM_FACADE_NOT_OK:
    case DCM_FACADE_REQUESTOUTOFRANGE:
    {
      if(FacadeReturnCodeGetEventData==DCM_FACADE_NOT_OK)
      {
        SetErrorCode = DCM_E_GENERALREJECT;
      }
      else
      {
        /* !LINKSTO Dcm.ReadDTCInfo.reportNumberOfDTCBySeverityMaskRecord.SetDTCFilter.E_NOT_OK,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCBySeverityMaskRecord.SetDTCFilter.E_NOT_OK,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCByStatusMask.GetDTCStatusAvailabilityMask.E_NOT_OK,1*/
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCByStatusMask.GetDTCStatusAvailabilityMask.E_NOT_OK,1*/
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSupportedDTC.GetDTCStatusAvailabilityMask.E_NOT_OK,1*/
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.GetDTCStatusAvailabilityMask.E_NOT_OK,1*/
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCWithPermanentStatus.GetDTCStatusAvailabilityMask.E_NOT_OK,1*/
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.GetDTCStatusAvailabilityMask.E_NOT_OK,1*/
        SetErrorCode = DCM_E_REQUESTOUTOFRANGE;
      }
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
      if(ReadEventDataSubServiceContext->FirstPageSent == TRUE)
      {
        ServiceProcessStatus = DCM_E_NOT_OK;
        ReadEventDataSubServiceContext->FirstPageSent = FALSE;
      }
      else
#endif /*#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)*/
      {
        switch (DiagnosticDataContext->DTCFormat)
        {
          case DEM_DTC_FORMAT_UDS:
            Dcm_ExternalSetNegResponse(pMsgContext, SetErrorCode);
            ServiceProcessStatus = DCM_E_DONE;
            break;
          case DEM_DTC_FORMAT_OBD:
            ServiceProcessStatus = DCM_E_INHIBIT;
            break;
          /* CHECK: NOPARSE */
          default:
            /* Defensive branch. Default case at the end of the switch statement */
            DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_GETFILTEREDDTC);
            break;
          /* CHECK: PARSE */
        }
      }
      break;
    }
    case DCM_FACADE_INVALID_VALUE:
    {
      DCM_PRECONDITION_ASSERT(DiagnosticDataContext->DTCFormat == DEM_DTC_FORMAT_UDS, DCM_SID_READ_DTC_INFORMATION);
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCBySeverityMaskRecord.SetDTCFilter.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCBySeverityMaskRecord.SetDTCFilter.UnsupportedError.NRC,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCByStatusMask.GetDTCStatusAvailabilityMask.UnsupportedError.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCByStatusMask.GetDTCStatusAvailabilityMask.UnsupportedError.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSupportedDTC.GetDTCStatusAvailabilityMask.UnsupportedError.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.GetDTCStatusAvailabilityMask.UnsupportedError.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCWithPermanentStatus.GetDTCStatusAvailabilityMask.UnsupportedError.NRC,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.GetDTCStatusAvailabilityMask.UnsupportedError.NRC,1*/
      SetErrorCode = DCM_E_GENERALREJECT;
      Dcm_ExternalSetNegResponse(pMsgContext, SetErrorCode);
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCBySeverityMaskRecord.SetDTCFilter.UnsupportedError.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCBySeverityMaskRecord.SetDTCFilter.UnsupportedError.DET,1 */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCByStatusMask.GetDTCStatusAvailabilityMask.UnsupportedError.DET,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCByStatusMask.GetDTCStatusAvailabilityMask.UnsupportedError.DET,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSupportedDTC.GetDTCStatusAvailabilityMask.UnsupportedError.DET,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportEmissionsOBDDTCByStatusMask.GetDTCStatusAvailabilityMask.UnsupportedError.DET,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCWithPermanentStatus.GetDTCStatusAvailabilityMask.UnsupportedError.DET,1*/
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.GetDTCStatusAvailabilityMask.UnsupportedError.DET,1*/
      DCM_DET_REPORT_ERROR(DCM_INTERNAL_API_ID, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* if (DCM_DEV_ERROR_DETECT == STD_ON) */
      ServiceProcessStatus = DCM_E_DONE;
    }
      break;
    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
  return ServiceProcessStatus;
}
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
/** \brief auxiliary function to enable page buffering */
STATIC FUNC(void, DCM_CODE) Dcm_DspInternal_SetupPageBuffering(
  P2CONST(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  uint8 SubServiceID)
{
  /* Setup page buffering service functions */
  switch(SubServiceID)
  {
    case DCM_DSP_READDTCINFORMATION_0x04_ID:
    case DCM_DSP_READDTCINFORMATION_0x06_ID:
    case DCM_DSP_READDTCINFORMATION_0x09_ID:
    case DCM_DSP_READDTCINFORMATION_0x18_ID:
    case DCM_DSP_READDTCINFORMATION_0x19_ID:
    {
      Dcm_DspInternal_EnableProcessPage(pMsgContext, DCM_PROCESS_PAGE_DTC_EXT_DATA_RECORD);
      break;
    }
    case DCM_DSP_READDTCINFORMATION_0x02_ID:
    case DCM_DSP_READDTCINFORMATION_0x07_ID:
    case DCM_DSP_READDTCINFORMATION_0x08_ID:
    case DCM_DSP_READDTCINFORMATION_0x0A_ID:
    case DCM_DSP_READDTCINFORMATION_0x13_ID:
    case DCM_DSP_READDTCINFORMATION_0x14_ID:
    case DCM_DSP_READDTCINFORMATION_0x15_ID:
    case DCM_DSP_READDTCINFORMATION_0x17_ID:
    {
      Dcm_DspInternal_EnableProcessPage(pMsgContext, DCM_PROCESS_PAGE_FILTERED_DTC);
      break;
    }

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }
}

#endif /* #if (DCM_PAGEDBUFFER_ENABLED == STD_ON) */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DemFacade_ProcessServiceIDBufferfull(
    Dcm_OpStatusType OpStatus,
    P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
    P2VAR(Dcm_DiagnosticDataContextType, AUTOMATIC, DCM_VAR) DiagnosticDataContext,
#endif /* (DCM_PAGEDBUFFER_ENABLED == STD_ON) */
    P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ReadEventDataSubServiceContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_NOT_OK;

#if( (DCM_DSP_USE_SERVICE_0X03 == STD_ON) || \
     (DCM_DSP_USE_SERVICE_0X07 == STD_ON) || \
     (DCM_DSP_USE_SERVICE_0X0A == STD_ON))
  if((DCM_DSP_OBD_SERVICE_0X03 == ReadEventDataSubServiceContext->ServiceID) ||
     (DCM_DSP_OBD_SERVICE_0X07 == ReadEventDataSubServiceContext->ServiceID) ||
     (DCM_DSP_OBD_SERVICE_0X0A == ReadEventDataSubServiceContext->ServiceID))
  {
    ServiceProcessStatus = DCM_E_INHIBIT;
  }
  else
#endif
  {
    if ((OpStatus == DCM_INITIAL) || (ReadEventDataSubServiceContext->PageBufferingFlag == FALSE))
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
    {
      /* Update response length for the data expected to be read */
      pMsgContext->resDataLen +=
      (PduLengthType)DiagnosticDataContext->AmountOfDataToWriteOnResponse;

      /*Enable Page Buffering*/
      Dcm_DspInternal_SetupPageBuffering(pMsgContext, ReadEventDataSubServiceContext->SubServiceID);

      /* Update the amount of filled data on the response buffer,
       after the execution has finished */
      Dcm_DspInternal_UpdateAmountOfFilledDataOnResponseBuffer(pMsgContext->dcmRxPduId,
      ReadEventDataSubServiceContext->AmountOfDataWrittenOnCurrentPage);

      /* Start paged processing*/
      Dcm_DsdInternal_StartPagedProcessing(pMsgContext);

      /* Flag to indicate that the page buffer mechanism has been enabled */
      ReadEventDataSubServiceContext->PageBufferingFlag = TRUE;

      ReadEventDataSubServiceContext->FirstPageSent = FALSE;
    }
    if((OpStatus == DCM_UPDATE_PAGE) || (OpStatus == DCM_PENDING))
    {
      ReadEventDataSubServiceContext->AmountOfDataWrittenOnCurrentPage +=
      DiagnosticDataContext->AmountOfDataStoredOnResponseOnCurrentRun;
      if( ReadEventDataSubServiceContext->AmountOfDataWrittenOnCurrentPage != 0U )
      {
        /* Indicate to start processing of the page */
        Dcm_DsdInternal_ProcessPage(
        DiagnosticDataContext->AmountOfDataStoredOnResponseOnCurrentRun);

        /* Reset the amount of data written on the current page */
        if(OpStatus != DCM_PENDING)
        {
          ReadEventDataSubServiceContext->AmountOfDataWrittenOnCurrentPage = 0U;
        }
        /* Translate code to DCM */
        ServiceProcessStatus = DCM_E_PROCESS_PAGE;

        ReadEventDataSubServiceContext->FirstPageSent = TRUE;
      }
      else
      {
        ServiceProcessStatus = DCM_E_NOT_OK;
      }
    }
#else
    {
      /* If page buffering is not available, do not process any further */
      /* !LINKSTO Dcm.EB.UDSServices.ReadDTCInformation.reportDTCSnapshotRecordByDTCNumber.PageBufferDisabled.ReadEventData.NegativeResponse.GetSizeOfFreezeFrameSelection.BufferFull.GeneralReject,1 */
      /* !LINKSTO Dcm.EB.UDSServices.ReadDTCInformation.reportDTCExtDataRecordByDTCNumber.PageBufferDisabled.ReadEventData.NegativeResponse.GetSizeOfExtendedDataRecordSelection.BufferFull.GeneralReject,1 */
      /* !LINKSTO Dcm.EB.UDSServices.ReadDTCInformation.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.PageBufferDisabled.ReadEventData.NegativeResponse.GetSizeOfFreezeFrameSelection.BufferFull.GeneralReject,1 */
      /* !LINKSTO Dcm.EB.UDSServices.ReadDTCInformation.reportUserDefMemoryDTCExtDataRecordByDTCNumber.PageBufferDisabled.ReadEventData.NegativeResponse.GetSizeOfExtendedDataRecordSelection.BufferFull.GeneralReject,1 */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
      ServiceProcessStatus = DCM_E_DONE;
    }
#endif /* #if (DCM_PAGEDBUFFER_ENABLED == STD_ON)*/
  }

  return ServiceProcessStatus;
}

#endif /* #if ( (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || \
                (DCM_SUBSERVICES_FILTERED_DTC_GROUP == STD_ON) ) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if (DCM_DSP_USE_DEMFACADE == STD_ON) */
/*==================[end of file]================================================================*/
