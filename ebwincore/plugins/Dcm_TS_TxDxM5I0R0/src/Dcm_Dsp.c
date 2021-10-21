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

/*This file contains the implementation of the Diagnostic Service
  * Processing module in Dcm. */

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 15.2 (required)
 *     "An unconditional break statement shall terminate every non-empty switch clause."
 *
 *     Reason:
 *     The following actions have to be executed in the same state so the break
 *     statement is removed in order to have shorter code and avoid duplicated code.
 *
 *  MISRA-2) Deviated Rule: 16.7 (required)
 *     "A pointer parameter in a function prototype should be declared as
 *     pointer to const if the pointer is not used to modify the addressed
 *     object."
 *
 *     Reason:
 *     a) Service and sub-service handlers share a common signature. It may not be possible to use
 *     constants since the functional use of these pointer parameters is different in various
 *     service and sub-service handlers.
 *     b) The pointer can be modified in a routine that is called from the function.
 *
 *  MISRA-3) Deviated Rule: 14.6 (required)
 *     "For any iteration statement there shall be at most one break statement used
 *     for loop termination"
 *
 *     Reason:
 *     The iteration needs to be terminated when some particular conditions are satisfied.
 *     Adapting such algorithm optimizes the code.
 *
 */

 /* MISRA-C:2012 Deviation List
  *
  *  MISRAC2012-1) Deviated Rule: 8.13 (advisory)
  *     "A pointer should point to a const-qualifed type whenever possible."
  *
  *     Reason:
  *     a) Service and sub-service handlers share a common signature. It may not be possible to use
  *     constants since the functional use of these pointer parameters is different in various
  *     service and sub-service handlers.
  *     b) The pointer can be modified in a routine that is called from the function.
  *
  *  MISRAC2012-2) Deviated Rule: 15.4 (required)
  *     "There should be no more than one break or goto statement used to terminate any iteration
  *     statement."
  *
  *     Reason:
  *     The iteration needs to be terminated when some particular conditions are satisfied.
  *     Adapting such algorithm optimizes the code.
  *
  *  MISRAC2012-3) Deviated Rule: 16.3 (required)
  *     "An unconditional break statement shall terminate every switch-clause."
  *
  *     Reason:
  *     The following actions have to be executed in the same state so the break
  *     statement is removed in order to have shorter code and avoid duplicated code.
  *
  *  MISRAC2012-4) Deviated Rule: 20.7 (required)
  *     "Expressions resulting from the expansion of macro parameters shall
  *     be enclosed in parentheses."
  *
  *     Reason:
  *     The parameter is used in function parameter declarations, definitions
  *     or as structure members and enclosing it in parentheses results
  *     in compiler syntax error.
  *
  *  MISRAC2012-5) Deviated Rule: 16.1 (required)
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
 */
/*===============================[includes]======================================================*/

#include <Dcm_Trace.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <TSAutosar.h>

#include <Dcm.h>                                                      /* Declaration of Dcm APIs */
#include <Dcm_HsmDcm.h>                                   /* public statechart model definitions */
#include <Dcm_DspExternal.h>                           /* Callbacks provided by Central Diag SwC */
#include <Dcm_Int.h>                                              /* Module internal definitions */
#include <Dcm_Dsl_Supervisor.h>

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
#include <Dcm_Dsl_Supervisor_MainConnectionHandling.h>        /* Dcm Dsl Supervisor header file */
#endif

#include <Dcm_Dsl_PostBootOperationsHandler.h>
#if (DCM_DSP_USE_ROESERVICES == STD_ON)
#include <Dcm_Dsl_RoeServices.h>
#endif

#include <Dcm_Dsl_SecurityLevelHandler.h>
#include <Dcm_Dsl_DiagnosticSessionHandler.h>   /* Dcm Dsl Diagnostic Session Handler header file */

#include <Dcm_Dsl_ComMChannelStateManager.h>                /* Dcm Dsl CComMChannelStateManager header
                                                             file */
#if( (DCM_DSP_USE_SERVICE_0X10 == STD_ON) || \
     (DCM_EXTSVC_ENABLED == STD_ON) || \
     (DCM_JUMP_TO_BL_ENABLED == STD_ON) )
#include <Dcm_Dsl_Supervisor_ProtocolHandling.h>
#endif
#if(DCM_DSP_USE_SERVICE_0X28 == STD_ON)
#include <Dcm_Dsp_SvcH_CommunicationControl.h>
#endif
#if(DCM_DSP_USE_SERVICE_0X01 == STD_ON) || \
   (DCM_DSP_USE_SERVICE_0X02 == STD_ON)
#include <Dcm_Dsp_PidServices.h>
#endif
#if ( (DCM_DSP_USE_SERVICE_0X03 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X04 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X14 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X07 == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X0A == STD_ON) )
#include <Dcm_Dsp_DemFacade.h>
#endif
#if(DCM_DSP_USE_SERVICE_0X06 == STD_ON)
#include <Dcm_Dsp_SvcH_RequestOnboardMonitoringTestResults.h>
#endif
#if(DCM_DSP_USE_SERVICE_0X08 == STD_ON)
#include <Dcm_Dsp_SvcH_RequestControlOfOnBoardSysTstComp.h>
#endif
#if(DCM_DSP_USE_SERVICE_0X22 == STD_ON)
#include <Dcm_Dsp_SvcH_ReadDataByIdentifier.h>
#endif
#if(DCM_DSP_USE_SERVICE_0X24 == STD_ON)
#include <Dcm_Dsp_SvcH_ReadScalingDataByIdentifier.h>
#endif
#if(DCM_DSP_USE_SERVICE_0X2E == STD_ON)
#include <Dcm_Dsp_SvcH_WriteDataByIdentifier.h>
#endif
#if(DCM_DSP_USE_SERVICE_0X2F == STD_ON)
#include <Dcm_Dsp_SvcH_InputOutputControlByIdentifier.h>
#endif
#if ((DCM_DSP_USE_SERVICE_0X2F == STD_ON) || (DCM_DSP_USE_SERVICE_0X86 == STD_ON))
#include <Dcm_Dsp_DidServices.h>  /* include DidServices SW unit header for IO control */
#endif
#if(DCM_DSP_USE_SERVICE_0X85 == STD_ON)
#include <Dcm_Dsp_SvcH_ControlDTCSetting.h>
#endif
/* as long as other services are no separate units the DidServices API is needed here */
#if ( (DCM_DSP_USE_SERVICE_0X2A == STD_ON) || \
      (DCM_DSP_USE_SERVICE_0X2C == STD_ON) )
#include <Dcm_Dsp_DidServices.h>                            /* DID services for service handlers */
#endif
#if (DCM_DSP_USE_SERVICE_0X27 == STD_ON)
#include <Dcm_Dsp_SvcH_SecurityAccess.h>
#endif

#if(DCM_DSP_USE_SERVICE_0X3E ==STD_ON)
#include <Dcm_Dsp_SvcH_TesterPresent.h>
#endif

#if ((DCM_DSP_USE_DIDSERVICES == STD_ON) && (DCM_DSP_USE_SENDERRECEIVER_INTERFACES == STD_ON))
#include <Dcm_Dsp_EndiannessInterpretation.h>
#endif

#if (DCM_DEM_CLIENT_REF_USED == STD_ON)
#include <Dem.h>
#endif /* #if (DCM_DEM_CLIENT_REF_USED == STD_ON) */

#if (DCM_BSWM_API_ENABLED == STD_ON)
#include <BswM_DCM.h>                                                  /* BswM interface for Dcm */
#endif
#include <ComM_Dcm.h>                                                  /* ComM interface for Dcm */
#include <SchM_Dcm.h>                                                  /* SchM interface for Dcm */
#include <TSMem.h>                                               /* EB specific memory functions */
/*===========================[macro definitions]=================================================*/

/* The number of bits to be shifted right to get the byte position */
#if (defined DCM_BYTE_POSITION_SHIFTS)
  #error "DCM_BYTE_POSITION_SHIFTS is already defined"
#endif
#define DCM_BYTE_POSITION_SHIFTS            (0x03U)

/* The mask to determine the bit position within a byte */
#if (defined DCM_BIT_POSITION_MASK)
  #error "DCM_BIT_POSITION_MASK is already defined"
#endif
#define DCM_BIT_POSITION_MASK               (0x07U)

/* The maximum number of bits to be copied in a byte */
#if (defined DCM_MAX_BIT_POSITION)
  #error "DCM_MAX_BIT_POSITION is already defined"
#endif
#define DCM_MAX_BIT_POSITION                (0x07U)

/* 0x10 service response length with timing values in response */
#if (defined DCM_0x10_RESPONSELENGTH_TIMING_VALUES)
  #error "DCM_0x10_RESPONSELENGTH_TIMING_VALUES is already defined"
#endif
#define DCM_0x10_RESPONSELENGTH_TIMING_VALUES      (5U)

/* 0x10 service response length without timing values in response */
#if (defined DCM_0x10_RESPONSELENGTH_NO_TIMING_VALUES)
  #error "DCM_0x10_RESPONSELENGTH_NO_TIMING_VALUES is already defined"
#endif
#define DCM_0x10_RESPONSELENGTH_NO_TIMING_VALUES   (1U)


/* Macros that define size of DTC record related to service */
#if (defined DCM_SIZEOFREC_OBDDTC)
  #error "DCM_SIZEOFREC_OBDDTC is already defined"
#endif
#define DCM_SIZEOFREC_OBDDTC                (0x02U)

#if (defined DCM_SIZEOFREC_UDSDTC)
  #error "DCM_SIZEOFREC_UDSDTC is already defined"
#endif
#define DCM_SIZEOFREC_UDSDTC                (0x04U)


/* Macros to identify requesting which Obd Service call function */
#if (defined DCM_DSP_OBD_DTC_CONFIRMED)
  #error "DCM_DSP_OBD_DTC_CONFIRMED is already defined"
#endif
#define DCM_DSP_OBD_DTC_CONFIRMED           (0x83U)

#if (defined DCM_DSP_OBD_DTC_PENDING)
  #error "DCM_DSP_OBD_DTC_PENDING is already defined"
#endif
#define DCM_DSP_OBD_DTC_PENDING             (0x87U)

#if (defined DCM_DSP_OBD_DTC_PERMANENT)
  #error "DCM_DSP_OBD_DTC_PERMANENT is already defined"
#endif
#define DCM_DSP_OBD_DTC_PERMANENT           (0x8AU)


/* Macros to identify which bits will be reported from DTC status */
#if (defined DCM_REPORT_CONFIRMEDDTC)
  #error "DCM_REPORT_CONFIRMEDDTC is already defined"
#endif
#define DCM_REPORT_CONFIRMEDDTC             (0x08U)

#if (defined DCM_REPORT_PENDINGDTC)
  #error "DCM_REPORT_PENDINGDTC is already defined"
#endif
#define DCM_REPORT_PENDINGDTC               (0x04U)

#if (defined DCM_INVALID_ASYNCHRONOUS)
  #error "DCM_INVALID_ASYNCHRONOUS is already defined"
#endif
#define DCM_INVALID_ASYNCHRONOUS               (0x00U)

#if (defined DCM_UDS_ASYNCHRONOUS)
  #error "DCM_UDS_ASYNCHRONOUS is already defined"
#endif
#define DCM_UDS_ASYNCHRONOUS               (0x01U)

#if (defined DCM_OBD_ASYNCHRONOUS)
  #error "DCM_OBD_ASYNCHRONOUS is already defined"
#endif
#define DCM_OBD_ASYNCHRONOUS               (0x02U)

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/

/*===========================[type definitions]==================================================*/

typedef uint8 Dcm_AsynchronousProtocolType;

/*==================[internal function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>


#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_TriggerServiceAsync(void);
#endif /* #if (DCM_ASYNC_SERVICE_HANDLER == STD_ON) */

/** \brief Sets up the processing of one diagnostic service request in the context of the
 **        currently running protocol
 **
 ** \param[in]  instIdx                  The index of the protocol in the context of which
 **                                      service execution shall take place
 **
 ** \param[out] ServiceProcessingContext Context variable containing the data necessary for
 **                                      the execution of a diagnostic service request
 **                                      processing.
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ServiceProcessorSetup(
    Dcm_ServiceProcessingContext_PtrType ServiceProcessingContextPtr,
    uint8 instIdx);

/** \brief Resets the provided service processing context
 **
 ** \param[out] ServiceProcessingContext Initialized context variable.
 */
STATIC FUNC (void, DCM_CODE) Dcm_Dsp_ServiceProcessorContextInit(
    Dcm_ServiceProcessingContext_PtrType ServiceProcessingContextPtr);

/** \brief Helper function for processing the evaluation of the service
 **        handler response
 **
 ** \param[in]  instIdx                  The index of the protocol in the context of which
 **                                      service execution shall take place
 **
 ** \param[in]  LocalOpStatus                 The OpStatus of the next invocation of the service handler
 **                                      (only in the case of asynchronous service handlers)
 **
 ** \param[out] ServiceProcessingContext Context variable containing the data necessary for
 **                                      the execution of a diagnostic service request
 **                                      processing.
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ServiceProcessorReturnValueProcessing(
    uint8 instIdx,
    Dcm_OpStatusType LocalOpStatus,
    Dcm_ServiceProcessingContext_PtrType ServiceProcessingContextPtr);


/** \brief Helper function for processing the default case for the evaluation of the service
 **        handler response
 **
 ** \param[in]  instIdx                  The index of the protocol in the context of which
 **                                      service execution shall take place
 **
 ** \param[out] ServiceProcessingContext Context variable containing the data necessary for
 **                                      the execution of a diagnostic service request
 **                                      processing.
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DefaultReturnValueProcessing(
    uint8 instIdx,
    Dcm_ServiceProcessingContext_PtrType ServiceProcessingContextPtr);

/** \brief Returns a pointer to the ServiceProcessingContext belonging to the ProtocolIdx parameter.
 **
 ** \param[in] ProtocolIdx              Index of the requested protocol
 */
STATIC FUNC_P2VAR(Dcm_ServiceProcessingContext_Type, AUTOMATIC, DCM_CODE) Dcm_Dsp_GetServiceProcessingContext(
    Dcm_ProtocolIndexType ProtocolIdx);

/** \brief Executes one call to the diagnostic service handler.
 **
 ** \param[in] ServiceProcessingContext Context variable containing the data necessary for
 **                                     the execution of a diagnostic service request
 **                                     processing.
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ServiceProcessorExecute(
    Dcm_ServiceProcessingContext_PtrType ServiceProcessingContextPtr
);

/** \brief Confirm RCRRP transmission sent by supervisor when HSM is not started and P2Timeout is
 **        reached.
 **
 ** \ param[in] ProtocolId          Unique BufferUser ID of the Protocol which requested the
 **                                 transmission..
 ** \ param[in] TransmissionResult  The result of the transmission.
 ** \ param[in] TxConnectionId      Unique BufferUser ID of the TxConnection which notifies the
 **                                 end of the transmission session.
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ProtocolTransmissionFinishedBeforeHSMStarted(
    Dcm_BufferUserType ProtocolId,
    Dcm_TxResultType TransmissionResult,
    Dcm_BufferUserType TxConnectionId
);

/** \brief Used by a TxConnection in order to notify a Protocol that a trasmission has ended
 **
 ** \ param[in] ProtocolId          Unique BufferUser ID of the Protocol which requested the
 **                                 transmission.
 ** \ param[in] TransmissionResult  The result of the transmission.
 ** \ param[in] TxConnectionId      Unique BufferUser ID of the TxConnection which notifies the
 **                                 end of the transmission session.
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ProtocolTransmissionFinishedAfterHSMStarted(
    Dcm_BufferUserType ProtocolId,
    Dcm_TxResultType TransmissionResult,
    Dcm_BufferUserType TxConnectionId
);

/** \brief This function emits HSM events
 **
 ** \ param[in] ProtocolIndex       The index of the Protocol; Indexes the arrays Dcm_HsmInfo[] and
 **                                 Dcm_ProtocolConfig[].
 ** \ param[in] TransmissionResult  The result of the transmission.
 ** \ param[in] TxConnectionId      Unique BufferUser ID of the TxConnection which notifies the
 **                                 end of the transmission session.
 */
STATIC FUNC(void, DCM_CODE) Dcm_Dsp_EmitHSMEventOnTxConfirmation(
    Dcm_ProtocolIndexType ProtocolIndex,
    Dcm_TxResultType TransmissionResult,
    Dcm_BufferUserType TxConnectionId
);

#if(DCM_DSP_USE_SERVICE_0X10 == STD_ON)
/** \brief UDS service 0x10
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP  The DCM shall send an "response pending" immediately
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_DiagnosticSessionControl_SvcStart(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief UDS service 0x10
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP  The DCM shall send an "response pending" immediately
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_DiagnosticSessionControl_SvcCont(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#if(DCM_JUMP_TO_BL_ENABLED == STD_ON)
/** \brief Internal handler for Jump to Bootloader
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 ** \param[in] OpStatus Status of the service processing
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP  The DCM shall send an "response pending" immediately
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_JumpToBL(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  Dcm_OpStatusType OpStatus);

/** \brief Internal handler to process SetProgConditions() call-out
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP  The DCM shall send an "response pending" immediately
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ProcessSetProgConditions(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#endif /* #if(DCM_JUMP_TO_BL_ENABLED == STD_ON) */
#endif /* #if(DCM_DSP_USE_SERVICE_0X10 == STD_ON) */


#if(DCM_DSP_USE_SERVICE_0X11 == STD_ON)
/** \brief UDS service 0x11 internal service handler
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP  The DCM shall send an "response pending" immediately
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ECUReset_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

/** \brief UDS service 0x11 internal service handler
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP  The DCM shall send an "response pending" immediately
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ECUReset_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#if ((DCM_DSP_SUBSERVICE_HARDRESET_0x01 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_KEYOFFONRESET_0x02 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_SOFTRESET_0x03 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_ENABLERAPIDPOWERSHUTDOWN_0x04 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_DISABLERAPIDPOWERSHUTDOWN_0x05 == STD_ON))

#if ((DCM_MODEDECLARATION_SUPPORT == STD_OFF) && (DCM_BSWM_API_ENABLED == STD_ON))
/** \brief Internal handler for Setting ResetMode
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval None
 */
STATIC FUNC(void, DCM_CODE) Dcm_DspInternal_SetResetMode(
  P2CONST(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif  /* #if ((DCM_MODEDECLARATION_SUPPORT == STD_OFF) && (DCM_BSWM_API_ENABLED == STD_ON)) */

#endif  /* #if ((DCM_DSP_SUBSERVICE_HARDRESET_0x01 == STD_ON) || \
                (DCM_DSP_SUBSERVICE_KEYOFFONRESET_0x02 == STD_ON) || \
                (DCM_DSP_SUBSERVICE_SOFTRESET_0x03 == STD_ON) || \
                (DCM_DSP_SUBSERVICE_ENABLERAPIDPOWERSHUTDOWN_0x04 == STD_ON) || \
                (DCM_DSP_SUBSERVICE_DISABLERAPIDPOWERSHUTDOWN_0x05 == STD_ON))  */
#endif /* #if(DCM_DSP_USE_SERVICE_0X11 == STD_ON) */

#if ((DCM_DSP_USE_SERVICE_0X2E == STD_ON) || (DCM_DSP_USE_SERVICE_0X22 == STD_ON) || \
 (DCM_DSP_USE_SERVICE_0X86 == STD_ON) || (DCM_DSP_USE_SERVICE_0X31 == STD_ON) || \
 (DCM_DSP_USE_SERVICE_0X2F == STD_ON) || (DCM_DSP_USE_SERVICE_0X2A == STD_ON) || \
 (DCM_DSP_USE_SERVICE_0X2C == STD_ON) || (DCM_DSP_USE_SERVICE_0X24 == STD_ON) || \
 (((DCM_DSP_USE_SERVICE_0X01 == STD_ON) || \
   (DCM_DSP_USE_SERVICE_0X02 == STD_ON)) && \
  (DCM_NUM_PID_CFG > 0U)))

/** \brief Determine the next byte in source in the context of data copy
 *
 *  \param[in] CurrentByteInSourceBuffer       Current byte in source
 *  \param[in] CopyMethod                      Copy method depending on copy direction and endianness
 *  \param[in] ByteAdvanceDirection            Byte direction depending on cpu byte order
 *
 *  \return The next byte to be copied from the source buffer
 */
STATIC FUNC(Dcm_MsgLenType, DCM_CODE) Dcm_DspInternal_DetermineNextByteInSourceBuffer(
  Dcm_MsgLenType CurrentByteInSourceBuffer,
  Dcm_DataCopyMethodType CopyMethod,
  Dcm_ByteAdvanceDirectionType ByteAdvanceDirection);

/** \brief Determine the next byte in destination in the context of data copy
 *
 *  \param[in] CurrentByteInDestBuffer         Current byte in destination
 *  \param[in] CopyMethod                      Copy method depending on copy direction and endianness
 *  \param[in] ByteAdvanceDirection            Byte direction depending on cpu byte order
 *
 *  \return The next byte in the destination buffer
 */
STATIC FUNC(Dcm_MsgLenType, DCM_CODE) Dcm_DspInternal_DetermineNextByteInDestBuffer(
  Dcm_MsgLenType CurrentByteInDestBuffer,
  Dcm_DataCopyMethodType CopyMethod,
  Dcm_ByteAdvanceDirectionType ByteAdvanceDirection);

#endif /*#if ((DCM_DSP_USE_SERVICE_0X2E == STD_ON) || (DCM_DSP_USE_SERVICE_0X22 == STD_ON) || \
 (DCM_DSP_USE_SERVICE_0X86 == STD_ON) || (DCM_DSP_USE_SERVICE_0X31 == STD_ON) || \
 (DCM_DSP_USE_SERVICE_0X2F == STD_ON) || (DCM_DSP_USE_SERVICE_0X2A == STD_ON) || \
 (DCM_DSP_USE_SERVICE_0X2C == STD_ON) || (DCM_DSP_USE_SERVICE_0X24 == STD_ON) || \
 (((DCM_DSP_USE_SERVICE_0X01 == STD_ON) || \
   (DCM_DSP_USE_SERVICE_0X02 == STD_ON)) && \
  (DCM_NUM_PID_CFG > 0U)))*/

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

STATIC VAR(Dcm_ServiceProcessingContext_Type, DCM_VAR) Dcm_ServiceProcessingContext_UDS;

STATIC VAR(Dcm_ServiceProcessingContext_Type, DCM_VAR) Dcm_ServiceProcessingContext_OBD;

#if (DCM_HAS_ROE_TYPE2 == STD_ON)
/** \brief If ROE TYPE2 is configured, a separate ServiceProcessingContext is used for this
           internal request */
STATIC VAR(Dcm_ServiceProcessingContext_Type, DCM_VAR) Dcm_ServiceProcessingContext_ROE_TYPE2;
#endif

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

STATIC VAR(Dcm_AsynchronousProtocolType, DCM_VAR) Dcm_AsynchronousProtocol;


#if (DCM_DSP_USE_SERVICE_0X10 == STD_ON)
/* index of the new diagnostic session into Dcm_SesTabEntryConfig[] */
STATIC VAR(uint8, DCM_VAR) Dcm_NewDiagnosticSessionTableIndex;
#endif

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ServiceProcessorExecute(
    Dcm_ServiceProcessingContext_PtrType ServiceProcessingContextPtr)
{
  if ((DCM_FORCE_RCRRP_OK == ServiceProcessingContextPtr->OpStatus) &&
      (DCM_EXTERNAL_SERVICE_HANDLER == ServiceProcessingContextPtr->ServiceHandlerType))
  {
    ServiceProcessingContextPtr->OpStatus = DCM_PENDING;
  }

  ServiceProcessingContextPtr->RetVal =
     ServiceProcessingContextPtr->ServiceFunction(ServiceProcessingContextPtr->OpStatus,
                                                 ServiceProcessingContextPtr->curMsgContext);

  if (DCM_CANCEL == ServiceProcessingContextPtr->OpStatus)
  {
    /* Ignore the return value of the service handler by overwriting it with a
       status signifying successful completion */
    if (DCM_INTERNAL_SERVICE_HANDLER == ServiceProcessingContextPtr->ServiceHandlerType)
    {
      ServiceProcessingContextPtr->RetVal = DCM_E_DONE;
    }
    else
    {
      ServiceProcessingContextPtr->RetVal = E_OK;
    }
  }
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ServiceProcessorSetup(
    Dcm_ServiceProcessingContext_PtrType ServiceProcessingContextPtr,
    uint8 instIdx)
{
 /* get configuration for current service table */
  CONSTP2CONST(Dcm_SidTabConfigType, AUTOMATIC, DCM_CONST) curSidTabConfig
    = &Dcm_SidTabConfig[DCM_PROTOCOL_CONFIG(instIdx).ServiceTableIndex];

  /* get the context information current instance */
  P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);
  /* Once the service processor belongs to the protocol, mark the processing as 'ongoing'
   * ServiceProcessingDone will be set to TRUE as soon as the SvcH returns a final value
   * or when the service processing is cancelled and CANCELATION DONE is processed */
  curHsmInfo->CancellationConditions.ServiceProcessingDone = FALSE;

  ServiceProcessingContextPtr->curMsgContext = &(curHsmInfo->msgContext);
  ServiceProcessingContextPtr->OpStatus = DCM_INITIAL;
  ServiceProcessingContextPtr->RetVal = DCM_E_NOT_OK;
  ServiceProcessingContextPtr->ServiceProcessingInProgress = TRUE;
  ServiceProcessingContextPtr->DcmInstanceIndex = instIdx;

#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
  if((curSidTabConfig->Entries[curHsmInfo->sidTabEntryInd]).AsyncServiceExecution == TRUE)
  {
#if (DCM_HAS_ROE_TYPE2 == STD_ON)
    const boolean IsTYPE2ROEPresent = Dcm_Dsl_Supervisor_IsTYPE2ROEPresent(instIdx);

    /* !LINKSTO Dcm.EB.ROE.SyncServiceToRespondTo,1 */
    if (TRUE == IsTYPE2ROEPresent)
    {
      ServiceProcessingContextPtr->ServiceExecutionType = DCM_SYNC_EXECUTION;
    }
    else
    {
      ServiceProcessingContextPtr->ServiceExecutionType = DCM_ASYNC_EXECUTION;

      if (DCM_PROTOCOL_UDS_GROUP == Dcm_Dsl_Supervisor_GetProtocolGroupTypeForProtocol(instIdx))
      {
        Dcm_AsynchronousProtocol = DCM_UDS_ASYNCHRONOUS;
      }
      else
      {

        Dcm_AsynchronousProtocol = DCM_OBD_ASYNCHRONOUS;
      }
    }
#else
    ServiceProcessingContextPtr->ServiceExecutionType = DCM_ASYNC_EXECUTION;

    if (DCM_PROTOCOL_UDS_GROUP == Dcm_Dsl_Supervisor_GetProtocolGroupTypeForProtocol(instIdx))
    {
      Dcm_AsynchronousProtocol = DCM_UDS_ASYNCHRONOUS;
    }
    else
    {
      Dcm_AsynchronousProtocol = DCM_OBD_ASYNCHRONOUS;
    }
#endif
  }
  else
  {
    ServiceProcessingContextPtr->ServiceExecutionType = DCM_SYNC_EXECUTION;
  }

  ServiceProcessingContextPtr->AsynchronousProcessingState = DCM_ASYNC_PROCESSING_IDLE;

  curHsmInfo->CancellationConditions.RetryAsyncServiceCancellation = FALSE;

#endif /* #if (DCM_ASYNC_SERVICE_HANDLER == STD_ON) */

  /* depending on the configuration, an internal or user provided service handler
   * function will be called */
  if ((curSidTabConfig->Entries[curHsmInfo->sidTabEntryInd]).ExtSrvHandler != NULL_PTR)
  {
    /* Configure external service handler function*/
    ServiceProcessingContextPtr->ServiceFunction =
      (curSidTabConfig->Entries[curHsmInfo->sidTabEntryInd]).ExtSrvHandler;
    ServiceProcessingContextPtr->ServiceHandlerType = DCM_EXTERNAL_SERVICE_HANDLER;
  }
  else
  {
    /* Configure internal service handler function */
    ServiceProcessingContextPtr->ServiceFunction =
      (curSidTabConfig->Entries[curHsmInfo->sidTabEntryInd]).IntSrvHandler;
    ServiceProcessingContextPtr->ServiceHandlerType = DCM_INTERNAL_SERVICE_HANDLER;
  }
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ProtocolTransmissionFinishedBeforeHSMStarted(
    Dcm_BufferUserType ProtocolId,
    Dcm_TxResultType TransmissionResult,
    Dcm_BufferUserType TxConnectionId)
{
  const Dcm_ProtocolIndexType ProtocolIndex = DCM_PROTOCOL_INDEX_FROM_ID(ProtocolId);
  Dcm_HsmEventType ResultEvent;

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
  uint16_least MainConnectionIndex;
  Dcm_ResponseConfirmationTypeType ResponseConfirmationType = DCM_TIMEOUT_RCRRP;
  boolean TransmissionSuccessful = TRUE;
  if (TransmissionResult != DCM_TX_OK)
  {
    TransmissionSuccessful = FALSE;
  }
#endif /* #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON) */

  /* next job is waiting for processing */
  /* this confirmation is for RCRRP sent by supervisor */
  DCM_PRECONDITION_ASSERT(DCM_HSM_INFO(ProtocolIndex).NextJob.JobRpCntrConfirmed !=
                          DCM_HSM_INFO(ProtocolIndex).NextJob.JobRpCntr, DCM_INTERNAL_API_ID);

  if ((DCM_DIAG_DSL_INFINITY_LIMIT_RESP_PEND ==
       Dcm_Dsl_DiagnosticSessionHandler_GetSessionMaxRcrrp(ProtocolIndex)) &&
      (DCM_DIAG_DSL_INFINITY_LIMIT_RESP_PEND == DCM_HSM_INFO(ProtocolIndex).NextJob.JobRpCntr))
  {
    /* Instead of incrementing rpCntrConfirmed counter decrement rpCntr counter. */
    DCM_HSM_INFO(ProtocolIndex).NextJob.JobRpCntr--;
  }
  else
  {
    /* Account confirmed RCRRPs */
    DCM_HSM_INFO(ProtocolIndex).NextJob.JobRpCntrConfirmed++;
  }

  /* Set flag to indicate confirmation of 'Response Pending'  transmission */
  DCM_HSM_INFO(ProtocolIndex).NextJob.JobResponsePendingTxStatus = DCM_RP_CONFIRMED_TX;

  if (DCM_TX_OK == TransmissionResult)
  {
    /* !LINKSTO Dcm.EB.TxConfirmationNotification.Enabled.Notification.TimeoutRCRRP.OK,1 */
    ResultEvent = (Dcm_HsmEventType)DCM_HSM_DCM_EV_TX_CONF_OK;
  }
  else
  {
    /* !LINKSTO Dcm.EB.TxConfirmationNotification.Enabled.Notification.TimeoutRCRRP.Failure,1 */
    ResultEvent = (Dcm_HsmEventType)DCM_HSM_DCM_EV_TX_CONF_NOT_OK;
  }

  (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(ProtocolIndex),
                                ResultEvent);

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
  /* Set Dcm_ApplicationTransmisionConfirmationEnqueued flag to TRUE. Mark the transmission for
   * being process in the next supervisor main function. */
  Dcm_Dsl_Supervisor_ApplicationTransmisionConfirmationEnqueued_SetFlag(TRUE);

  /* Searched for the main connection index using the TxConnectionId. */
  MainConnectionIndex =
                  Dcm_Dsl_Supervisor_MainConnectionHandling_SearchForMainConnection(TxConnectionId);

  Dcm_Dsl_Supervisor_MainConnectionHandling_StoreMainConnectionStatusTransmission(
                  (Dcm_MainConnectionIndexType)MainConnectionIndex,
                  NULL_PTR,
                  ResponseConfirmationType,
                  DCM_HSM_INFO(ProtocolIndex).negativeResponseCode,
                  TransmissionSuccessful);
#endif /* #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON) */

#if (!(DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON))
  TS_PARAM_UNUSED(TxConnectionId);
#endif
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ProtocolTransmissionFinishedAfterHSMStarted(
    Dcm_BufferUserType ProtocolId,
    Dcm_TxResultType TransmissionResult,
    Dcm_BufferUserType TxConnectionId)
{
  const Dcm_ProtocolIndexType ProtocolIndex = DCM_PROTOCOL_INDEX_FROM_ID(ProtocolId);
#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
  uint16_least MainConnectionIndex;
  Dcm_SupervisorProtocolRequestTypeType ProtocolRequestType =
                            Dcm_Dsl_Supervisor_GetProtocolRequestType(ProtocolId);

  P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(ProtocolIndex);
  Dcm_MsgContextPtrType pMsgContext = &curHsmInfo->msgContext;
  Dcm_ResponseConfirmationTypeType ResponseConfirmationType = DCM_FINALRESPONSE;
  boolean TransmissionSuccessful = TRUE;
  if (TransmissionResult != DCM_TX_OK)
  {
    TransmissionSuccessful = FALSE;
  }
#endif /* #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON) */

  /* Reject new incoming diagnostic requests during EcuReset processing or if
     immediately after a NRC that is returned by ECUReset, is processed an internal request */
  if ((DCM_SID_ECU_RESET == DCM_HSM_INFO(ProtocolIndex).serviceId) &&
      (DCM_ENABLE_RAPID_POWER_SHUTDOWN_RESET != DCM_HSM_INFO(ProtocolIndex).subServiceId) &&
      (DCM_DISABLE_RAPID_POWER_SHUTDOWN_RESET != DCM_HSM_INFO(ProtocolIndex).subServiceId) &&
      (DCM_E_OK == DCM_HSM_INFO(ProtocolIndex).negativeResponseCode) &&
      (DCM_IS_NORMAL_REQUEST(DCM_HSM_INFO(ProtocolIndex).RequestType)) &&
      (DCM_RP_PENDING_CONF != DCM_HSM_INFO(ProtocolIndex).responsePendingTxStatus) &&
      (DCM_TX_OK == TransmissionResult)
     )
  {
    /* Reject new incoming diagnostic requests during EcuReset processing */
    /* !LINKSTO Dcm.EB.UDSServices.Success.NewStartOfReceptionCall.UnsuppressedResponse.RejectRx,2 */
    /* !LINKSTO Dcm.EB.UDSServices.Success.NewStartOfReceptionCall.SuppressedResponse.RejectRx,2 */
    /* !LINKSTO Dcm.EB.UDSServices.Success.NewTYPE2ROEEvent.UnsuppressedResponse.Discard,1 */
    /* !LINKSTO Dcm.EB.UDSServices.Success.NewTYPE2ROEEvent.SuppressedResponse.Discard,1 */
    /* !LINKSTO Dcm.EB.UDSServices.Success.NewTYPE1ROEEvent.UnsuppressedResponse.Discard,2 */
    /* !LINKSTO Dcm.EB.UDSServices.Success.NewTYPE1ROEEvent.SuppressedResponse.Discard,2 */
    Dcm_Dsl_Supervisor_InhibitRequestProcessing();
  }

  if ((DCM_HSM_INFO(ProtocolIndex).rpCntrConfirmed !=
       DCM_HSM_INFO(ProtocolIndex).rpCntr))
  {
    if ((DCM_DIAG_DSL_INFINITY_LIMIT_RESP_PEND ==
         Dcm_Dsl_DiagnosticSessionHandler_GetSessionMaxRcrrp(ProtocolIndex)) &&
        (DCM_DIAG_DSL_INFINITY_LIMIT_RESP_PEND == DCM_HSM_INFO(ProtocolIndex).rpCntr))
    {
      /* Instead of incrementing rpCntrConfirmed counter decrement rpCntr counter. */
      DCM_HSM_INFO(ProtocolIndex).rpCntr--;
#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
      ResponseConfirmationType = DCM_TIMEOUT_RCRRP;
      if ((DCM_HSM_INFO(ProtocolIndex).rpForcedCntrConfirmed) != (DCM_HSM_INFO(ProtocolIndex).rpForcedCntr))
      {
        ResponseConfirmationType = DCM_FORCED_RCRRP;
        /* Instead of incrementing rpForcedCntrConfirmed counter decrement rpForcedCntr counter. */
        DCM_HSM_INFO(ProtocolIndex).rpForcedCntr--;
      }
#endif /* #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON) */
    }
    else
    {
      /* Account confirmed RCRRPs */
      DCM_HSM_INFO(ProtocolIndex).rpCntrConfirmed++;
#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
      ResponseConfirmationType = DCM_TIMEOUT_RCRRP;
      if ((DCM_HSM_INFO(ProtocolIndex).rpForcedCntrConfirmed) != (DCM_HSM_INFO(ProtocolIndex).rpForcedCntr))
      {
        ResponseConfirmationType = DCM_FORCED_RCRRP;
        /* Account confirmed forced RCRRPs */
        DCM_HSM_INFO(ProtocolIndex).rpForcedCntrConfirmed++;
      }
#endif /* #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON) */
    }
  }

  if ((DCM_HSM_INFO(ProtocolIndex).responsePendingTxStatus == DCM_RP_PENDING_CONF) &&
    (TransmissionResult == DCM_TX_OK) &&
    (DCM_CANCEL_INVALID == DCM_HSM_INFO(ProtocolIndex).CancellationConditions.CancellationType))
  {
    /* Set flag to indicate successful transmission of 'Response Pending' */
    DCM_HSM_INFO(ProtocolIndex).responsePendingTxStatus = DCM_RP_CONFIRMED_TX;

    if (DCM_HSM_INFO(ProtocolIndex).jumpToBLExecution == DCM_FIRST_RCRRP_PENDING)
    {
      /* If a jump to the bootloader is waiting for a RCRRP, confirm that the RCRRP
       * has been sent */
      DCM_HSM_INFO(ProtocolIndex).jumpToBLExecution = DCM_JUMP_IN_PROGRESS;
    }
  }
  else
  {
    if (DCM_CANCEL_INVALID !=
          DCM_HSM_INFO(ProtocolIndex).CancellationConditions.CancellationType)
    {
      /* Since this protocol is waiting for cancellation conditions to be fulfilled
         (all pending transmissions are ended, all pending service processing
          is ended), set the appropriate flag that transmission is finished. */

      /* !LINKSTO Dcm.EB.TxConfirmationNotification.Enabled.Notification.CancellationGeneralReject.Failure,1 */
      /* !LINKSTO Dcm.EB.TxConfirmationNotification.Enabled.Notification.CancellationGeneralReject.OK,1 */
      Dcm_Dsp_CancellationDone(DCM_TRANSMISSION_CANCELLATION, ProtocolIndex);
    }
    else
    {
      if (DCM_RP_PENDING_CONF != DCM_HSM_INFO(ProtocolIndex).responsePendingTxStatus)
      {
        /* In case of periodic TYPE2 transmissions the protocol and relevant buffers
         * must not be freed before all transmissions are confirmed. The freeing will
         * happen on the transition from state PeriodicSendingUUDTFinishing to OosNotified
         * upon the PROC_DONE event. */
#if ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U))
        if (Dcm_Dsl_IsTYPE2PeriodicTxConnection(TxConnectionId) != TRUE)
        {
#endif
          /* A final transmission from a protocol (a transmission which is not
             for a RCRRP NRC) will cause the protocol processing to be considered
             complete. */
          Dcm_Dsl_BufferFreeRxTxNRC(DCM_PROTOCOL_ID_FROM_INDEX(DCM_CUR_INST_ID(ProtocolIndex)),
                                   DCM_HSM_INFO(DCM_CUR_INST_ID(ProtocolIndex)).RxBuffer.BufferId,
                                   DCM_HSM_INFO(DCM_CUR_INST_ID(ProtocolIndex)).TxBuffer.BufferId,
                                   DCM_HSM_INFO(DCM_CUR_INST_ID(ProtocolIndex)).NRCBuffer.BufferId);

          Dcm_Dsl_Supervisor_ProtocolFree(
            DCM_PROTOCOL_ID_FROM_INDEX(DCM_CUR_INST_ID(ProtocolIndex)));
#if ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U))
        }
#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)
        else
        {
          /* The sample stored in the TransmissionBuffer can now be removed since the transmission is confirmed.
           * No need to wait until the transition from state PeriodicSendingUUDTFinishing to OosNotified */
          Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveUUDTSampleOnConfirmation(TxConnectionId);
        }
#endif /*#if (DCM_DSP_USE_SERVICE_0X2A == STD_ON)*/
#endif /*#if ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U))*/
      }
    }

    /* Stop the retry timer */
    DCM_HSM_INFO(DCM_CUR_INST_ID(ProtocolIndex)).retryTimer = (uint16)0U;

    if (DCM_RP_PENDING_CONF == DCM_HSM_INFO(DCM_CUR_INST_ID(ProtocolIndex)).responsePendingTxStatus)
    {
      /* Set flag to indicate confirmation of 'Response Pending' transmission */
      DCM_HSM_INFO(DCM_CUR_INST_ID(ProtocolIndex)).responsePendingTxStatus = DCM_RP_CONFIRMED_TX;
    }
    DCM_HSM_INFO(DCM_CUR_INST_ID(ProtocolIndex)).jumpToBLSupport = FALSE;
    DCM_HSM_INFO(DCM_CUR_INST_ID(ProtocolIndex)).jumpToBLExecution = DCM_PREPARE_JUMP;
  }

  Dcm_Dsp_EmitHSMEventOnTxConfirmation(ProtocolIndex, TransmissionResult, TxConnectionId);

#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
  if( (FALSE == DCM_IS_PERIODIC_REQUEST(ProtocolRequestType)) &&
      (FALSE == DCM_IS_ROE_REQUEST(ProtocolRequestType))
    )
  {
    /* Set Dcm_ApplicationTransmisionConfirmationEnqueued flag to TRUE. Mark the transmission for
     * being process in the next supervisor main function. */
    Dcm_Dsl_Supervisor_ApplicationTransmisionConfirmationEnqueued_SetFlag(TRUE);

    /* Searched for the main connection index using the TxConnectionId. */
    MainConnectionIndex = Dcm_Dsl_Supervisor_MainConnectionHandling_SearchForMainConnection(TxConnectionId);

    Dcm_Dsl_Supervisor_MainConnectionHandling_StoreMainConnectionStatusTransmission((Dcm_MainConnectionIndexType)MainConnectionIndex,
                                                                                    pMsgContext,
                                                                                    ResponseConfirmationType,
                                                                                    DCM_HSM_INFO(ProtocolIndex).negativeResponseCode,
                                                                                    TransmissionSuccessful);
  }
#endif /* #if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON) */

  if(DCM_HSM_INFO(ProtocolIndex).RequestType == DCM_JUMP_FROM_BL_RESPONSE_REQUEST)
  {
    Dcm_Dsl_Supervisor_DisinhibitRequestProcessing();
  }

#if (!((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U)))
  TS_PARAM_UNUSED(TxConnectionId);
#endif
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_EmitHSMEventOnTxConfirmation(
    Dcm_ProtocolIndexType ProtocolIndex,
    Dcm_TxResultType TransmissionResult,
    Dcm_BufferUserType TxConnectionId)
{
  Dcm_HsmEventType ResultEvent;

  if (DCM_CANCEL_INVALID ==
      DCM_HSM_INFO(ProtocolIndex).CancellationConditions.CancellationType)
  {
    /* This protocol is not waiting for cancellation, therefore emit
       the necessary confirmation signals. */
#if ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U))
    if (Dcm_Dsl_IsTYPE2PeriodicTxConnection(TxConnectionId) == TRUE)
    {
      ResultEvent = (Dcm_HsmEventType)DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM;
    }
    else
#endif /* #if ((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) &&
            (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U)) */
    {
      if (DCM_TX_OK == TransmissionResult)
      {
        /* !LINKSTO Dcm.EB.TxConfirmationNotification.Enabled.Notification.TimeoutRCRRP.OK,1 */
        /* !LINKSTO Dcm.EB.TxConfirmationNotification.Enabled.Notification.ForcedRCRRP.OK,1 */
        /* !LINKSTO Dcm.EB.TxConfirmationNotification.Enabled.Notification.FinalResponse.NotSuppressed.OK,1 */
        /* !LINKSTO Dcm.EB.TxConfirmationNotification.Enabled.Notification.FinalResponse.Suppressed,1 */
        ResultEvent = (Dcm_HsmEventType)DCM_HSM_DCM_EV_TX_CONF_OK;
      }
      else
      {
        /* !LINKSTO Dcm.EB.TxConfirmationNotification.Enabled.Notification.FinalResponse.NotSuppressed.Failure,1 */
        /* !LINKSTO Dcm.EB.TxConfirmationNotification.Enabled.Notification.TimeoutRCRRP.Failure,1 */
        /* !LINKSTO Dcm.EB.TxConfirmationNotification.Enabled.Notification.ForcedRCRRP.Failure,1 */
        ResultEvent = (Dcm_HsmEventType)DCM_HSM_DCM_EV_TX_CONF_NOT_OK;
      }
    }

    (void)DCM_HSMEMITTOSELFINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(ProtocolIndex),
                                        ResultEvent);
  }
#if (DCM_NUM_PROTOCOL == 1U)
  TS_PARAM_UNUSED(ProtocolIndex);
#endif /* #if (DCM_NUM_PROTOCOL == 1U) */
#if (!((DCM_HAS_PERIODIC_RESPONSE_TYPE2 == STD_ON) && (DCM_NUM_PERIODIC_TRANSMISSIONS > 0U)))
  TS_PARAM_UNUSED(TxConnectionId);
#endif
}

STATIC FUNC_P2VAR(Dcm_ServiceProcessingContext_Type, AUTOMATIC, DCM_CODE) Dcm_Dsp_GetServiceProcessingContext(
    Dcm_ProtocolIndexType ProtocolIdx)
{
  /* For Periodic TYPE2 processing there is no need for a separate processing context. */
  Dcm_ServiceProcessingContext_PtrType ServiceProcessingContextPtr = NULL_PTR;

#if (DCM_HAS_ROE_TYPE2 == STD_ON)

#if (DCM_NUM_PROTOCOL == 1U)
  TS_PARAM_UNUSED(ProtocolIdx);
#endif

  if ((DCM_ROE_TYPE2_PHYSICAL_REQUEST   == DCM_HSM_INFO(ProtocolIdx).RequestType) ||
      (DCM_ROE_TYPE2_FUNCTIONAL_REQUEST == DCM_HSM_INFO(ProtocolIdx).RequestType))
  {
    ServiceProcessingContextPtr = &Dcm_ServiceProcessingContext_ROE_TYPE2;
  }
  else
#else
  TS_PARAM_UNUSED(ProtocolIdx);
#endif
  {
    if (DCM_PROTOCOL_UDS_GROUP == Dcm_Dsl_Supervisor_GetProtocolGroupTypeForProtocol(ProtocolIdx))
    {
      ServiceProcessingContextPtr = &Dcm_ServiceProcessingContext_UDS;
    }
    else
    {
      ServiceProcessingContextPtr = &Dcm_ServiceProcessingContext_OBD;
    }
  }

  return ServiceProcessingContextPtr;
}

FUNC(void, DCM_CODE) Dcm_DspInternal_SvcFunc(
    uint8 instIdx,
    Dcm_OpStatusType OpStatus)
{
  Dcm_ServiceProcessingContext_PtrType ServiceProcessingContextPtr =
    Dcm_Dsp_GetServiceProcessingContext(instIdx);

  Dcm_OpStatusType LocalOpStatus = OpStatus;

#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
  uint8 LocalAsynchronousProcessingState;
#endif /* #if (DCM_ASYNC_SERVICE_HANDLER == STD_ON) */

  DBG_DCM_DSPINTERNAL_SVCFUNC_ENTRY(DCM_INST(instIdx),OpStatus);

  if (FALSE == ServiceProcessingContextPtr->ServiceProcessingInProgress)
  {
    /* Only one protocol/HSM may "own" the service processor at one time. Therefore
       check whether the service processor is idle and if yes, initialize it. */
    Dcm_Dsp_ServiceProcessorSetup(ServiceProcessingContextPtr, instIdx);

    /* Override the DCM_PENDING OpStatus with the DCM_INITIAL OpStatus as the service
       handler will be called for the very first time. */
    LocalOpStatus = DCM_INITIAL;

    ServiceProcessingContextPtr->ServiceProcessingInProgress = TRUE;
  }


  if (instIdx == ServiceProcessingContextPtr->DcmInstanceIndex)
  {
    /* If the service processor "belongs" to the current protocol, either as a result
     * of having been reserved in the previous stage, or because this is a subsequent call,
     * continue with execution.
     * Otherwise, wait for one more cycle and retry. */
#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
    if ((DCM_SYNC_EXECUTION == ServiceProcessingContextPtr->ServiceExecutionType) ||
        (DCM_CONFIRMED_OK == LocalOpStatus) ||
        (DCM_CONFIRMED_NOK == LocalOpStatus)
       )
    {
#endif /* #if (DCM_ASYNC_SERVICE_HANDLER == STD_ON) */
      /* !LINKSTO Dcm.ReportDTCFaultDetectionCounter.Sync,1 */
      /* Rationale: for synchronously-processed service handlers, all API calls
                    by the service handler are executed from the context of the
                    Dcm_MainFunction(). */

      ServiceProcessingContextPtr->OpStatus = LocalOpStatus;
      Dcm_Dsp_ServiceProcessorExecute(ServiceProcessingContextPtr);

      /* Process the error code returned by the service handler. */
      Dcm_Dsp_ServiceProcessorReturnValueProcessing(instIdx,
                                                    LocalOpStatus,
                                                    ServiceProcessingContextPtr);
#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
    }
    else
    {
      /* Fetch the current status of the asynchronous service handler and use this
       * value for further checks */
      LocalAsynchronousProcessingState = ServiceProcessingContextPtr->AsynchronousProcessingState;

      if (DCM_ASYNC_PROCESSING_IDLE == LocalAsynchronousProcessingState)
      {
        /* Either the previous job completed processing, or the asynchronous service handler
         * is idle, therefore the service handler is ready to accept a new job */
        ServiceProcessingContextPtr->OpStatus = LocalOpStatus;
        ServiceProcessingContextPtr->AsynchronousProcessingState = DCM_ASYNC_PROCESSING_RUNNING;

        /* Trigger one asynchronous call to the diagnostic service processor */

        /* !LINKSTO Dcm.EB.DcmAsyncServiceExecution.Task, 2 */
        Dcm_TriggerServiceAsync();

        if (DCM_CANCEL == ServiceProcessingContextPtr->OpStatus)
        {
          /* Cancellation instruction has been successfully sent. From this point on, the
             cancellation is not handled by the regular retry timer. */
          DCM_HSM_INFO(instIdx).CancellationConditions.RetryAsyncServiceCancellation =
            DCM_ASYNC_SERVICE_HANDLER_CANCELLATION_PENDING;
        }
        else
        {
          /* The asynchronous service processor couldn't have by this time done anything
             (it still needs to start). Therefore prepare a run in the next Dcm_MainFunction()
             cycle. */
          DCM_HSM_INFO(instIdx).retryTimer = 1U;
        }
      }
      else if (DCM_ASYNC_PROCESSING_RUNNING == LocalAsynchronousProcessingState)
      {
        /* The asynchronous service processor is running, but hasn't reached a result,
           therefore we transmit, in its place, a PENDING result */

        if ((DCM_ASYNC_SERVICE_HANDLER_CANCELLATION_PENDING !=
               DCM_HSM_INFO(instIdx).CancellationConditions.RetryAsyncServiceCancellation
             ) &&
            (DCM_CANCEL == LocalOpStatus)
           )
        {
          /* If the asynchronous service handler needs to be cancelled and cancellation
             has not been attempted yet (the service handler called with a DCM_CANCEL OpStatus),
             attempt the cancellation in the next call of the Dcm_MainFunction */
          DCM_HSM_INFO(instIdx).CancellationConditions.RetryAsyncServiceCancellation =
            DCM_ASYNC_SERVICE_HANDLER_CANCEL_IN_NEXT_CYCLE;
        }
        else
        {
          /* !LINKSTO Dcm.EB.DcmAsyncServiceExecution.Sequencing1, 1 */
          /* Poll for the return value of the service handler in the next cycle. */
          DCM_HSM_INFO(instIdx).retryTimer = 1U;
        }
      }
      else
      {
        /* The service processor finished one run, some data is available to be processed. */

        /* !LINKSTO Dcm.EB.DcmAsyncServiceExecution.Sequencing2, 1 */
        DCM_PRECONDITION_ASSERT(
            (DCM_ASYNC_PROCESSING_DATA_AVAILABLE == LocalAsynchronousProcessingState),
            DCM_SVCID_ASYNCSERVICEDISPATCHER);

        /* Initialize the service handler to be ready for another run in the next cycle, if needed.
         */
        ServiceProcessingContextPtr->AsynchronousProcessingState = DCM_ASYNC_PROCESSING_IDLE;

        /* Process the error code returned by the service handler. */
        Dcm_Dsp_ServiceProcessorReturnValueProcessing(instIdx,
                                                      LocalOpStatus,
                                                      ServiceProcessingContextPtr);
      }
    }
#endif /* #if (DCM_ASYNC_SERVICE_HANDLER == STD_ON) */
  }
  else
  {
    /* Wait one more cycle in order to reserve the service processor */
    DCM_HSM_INFO(instIdx).retryTimer = 1U;
  }

  DBG_DCM_DSPINTERNAL_SVCFUNC_EXIT(DCM_INST(instIdx),OpStatus);
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_ServiceProcessorReturnValueProcessing(
    uint8 instIdx,
    Dcm_OpStatusType LocalOpStatus,
    Dcm_ServiceProcessingContext_PtrType ServiceProcessingContextPtr)
{
#if (DCM_ASYNC_SERVICE_HANDLER == STD_OFF)
  TS_PARAM_UNUSED(LocalOpStatus);
#endif /* #if (DCM_ASYNC_SERVICE_HANDLER == STD_ON) */

#if (DCM_NUM_PROTOCOL == 1U)
  TS_PARAM_UNUSED(instIdx);
#endif

  /* First determine whether a cancellation of service processing was requested.
     If this is the case, then if the current error code returned by the service handler
     is one of the "final" error codes, i.e. DCM_E_DONE, DCM_OK, DCM_E_NOT_OK, this means
     that there is no more operation to cancel and we can confirm the cancellation as being
     successful already. */
#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
  if ( /* This run was not a cancellation run, but a cancellation was requested so the
          next one should be a cancellation run.*/
      ((DCM_CANCEL == LocalOpStatus) ||
       (DCM_CANCEL == ServiceProcessingContextPtr->OpStatus)
      ) &&
      (/* This is a "final" error code. */
       (E_OK == ServiceProcessingContextPtr->RetVal) ||
       (DCM_E_NOT_OK == ServiceProcessingContextPtr->RetVal) ||
       (DCM_E_DONE == ServiceProcessingContextPtr->RetVal)
      )
     )
#else
  if (/* This run was a cancellation run. */
      (DCM_CANCEL == ServiceProcessingContextPtr->OpStatus)
      &&
      (/* This is a "final" error code. */
       (E_OK == ServiceProcessingContextPtr->RetVal) ||
       (DCM_E_NOT_OK == ServiceProcessingContextPtr->RetVal) ||
       (DCM_E_DONE == ServiceProcessingContextPtr->RetVal)
      )
     )
#endif /* #if (DCM_ASYNC_SERVICE_HANDLER == STD_ON) */
  {
    /* This is either a completed cancellation, or a cancellation was requested but
       is no longer necessary. Perform the actions to acknowledge cancellation of
       the service handler. */
#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
    /* Cancellation has been successfully performed. */
    DCM_HSM_INFO(instIdx).CancellationConditions.RetryAsyncServiceCancellation =
      DCM_ASYNC_SERVICE_HANDLER_INVALID_CANCELLATION;
#endif /* #if (DCM_ASYNC_SERVICE_HANDLER == STD_ON) */
    Dcm_Dsp_CancellationDone(DCM_SERVICE_CANCELLATION, instIdx);


    if (DCM_HSM_INFO(instIdx).CancellationConditions.CancellationType !=
          DCM_CANCEL_PG_BUF_SENDING)
    {
      /* In case this service handler is not executing a page-buffered transmission, this
         is always the final step of processing of a service handler. Release the
         service processor so that it may be used by another protocol.

         Note: in case of page-buffered transmissions, since the service handler has already
         sent out the first page by this time, it needs to be called one final time during the
         confirmation from the lower layer. This is the reason why the service handler processor
         is not reinitialized. */
      Dcm_Dsp_ServiceProcessorReset(instIdx);
    }
  }
#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
  else if((DCM_ASYNC_SERVICE_HANDLER_CANCELLATION_PENDING !=
             DCM_HSM_INFO(instIdx).CancellationConditions.RetryAsyncServiceCancellation
           ) &&
          (DCM_CANCEL == LocalOpStatus)
         )
  {
    /* This was not a final run, and a cancellation shall follow. */

    /* If the asynchronous service handler needs to be cancelled and cancellation
       has not been attempted yet (the service handler called with a DCM_CANCEL OpStatus),
       attempt the cancellation in the next call of the Dcm_MainFunction */
    DCM_HSM_INFO(instIdx).CancellationConditions.RetryAsyncServiceCancellation =
      DCM_ASYNC_SERVICE_HANDLER_CANCEL_IN_NEXT_CYCLE;
  }
#endif /* #if (DCM_ASYNC_SERVICE_HANDLER == STD_ON) */
  else
  {
    /* The error value returned by the service processor needs to be processed. */
    switch(ServiceProcessingContextPtr->RetVal)
    {
      case DCM_E_FORCE_RCRRP:
#if (DCM_APPL_TXCONFIRMATION_NOTIFICATION == STD_ON)
        DCM_HSM_INFO(instIdx).rpForcedCntr++;
#endif
        /* trigger sending of immediate resp pending */
        (void) DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, instIdx, DCM_HSM_DCM_EV_FORCE_RCRRP);
        break;

      case DCM_E_PENDING:
        DCM_HSM_INFO(instIdx).retryTimer = 1U;
        break;

      case DCM_E_PROCESS_PAGE:
        /* Wait for event UPDATE_PAGE and subsequent service function call with
         * OpStatus == UPDATE_PAGE or OpStatus == CONFIRMED */
        /* DsdInternal_ProcessPage() must have been called within the service function */
        /* Trigger the HSM to process the page. */
        break;

      case DCM_E_ABORT:
        (void) DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(instIdx),
                                       DCM_HSM_DCM_EV_PROC_ABORT);


        /* This is always the final step of processing of a service handler. Release the
           service processor so that it may be used by another protocol. */
        Dcm_Dsp_ServiceProcessorReset(instIdx);
        break;

      case E_OK:
        /* !LINKSTO Dcm.ExternalProcessingDone.API,1, Dcm.ExternalProcessingDone.ExternalServices,2 */
        /* Emit PROC_DONE here for external services (only external services use
          Dcm_ExternalProcessingDone according to the current design). */
        (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, instIdx, DCM_HSM_DCM_EV_PROC_DONE);

        /* Indicate that a cancellation shall not be necessary for this service handler. */
        DCM_HSM_INFO(instIdx).CancellationConditions.ServiceProcessingDone = TRUE;

        /* This is always the final step of processing of a service handler. Release the
           service processor so that it may be used by another protocol. */
        Dcm_Dsp_ServiceProcessorReset(instIdx);

        break;

      case DCM_E_NOT_OK:
        /* Indicate that a cancellation shall not be necessary for this service handler. */
        DCM_HSM_INFO(instIdx).CancellationConditions.ServiceProcessingDone = TRUE;

        if (DCM_EXTERNAL_SERVICE_HANDLER == ServiceProcessingContextPtr->ServiceHandlerType)
        {
          /* !LINKSTO Dcm.ExternalProcessingDone.API,1, Dcm.ExternalProcessingDone.ExternalServices,2 */
          /* Emit PROC_DONE here for external services (only external services use
          Dcm_ExternalProcessingDone according to the current design). */
          (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, instIdx, DCM_HSM_DCM_EV_PROC_DONE);

          /* This is always the final step of processing of a service handler. Release the
             service processor so that it may be used by another protocol. */
          Dcm_Dsp_ServiceProcessorReset(instIdx);
        }
        else
        {
          /* !LINKSTO Dcm.EB.ExternalDiagnosticService.DCM_E_NOT_OK,1 */
          /* Internal service handlers may also return DCM_E_NOT_OK, but never when called
             for cancellation or for confirmation. One more call to the service handler
             shall be executed for confirmation in this case, therefore do not
             reinitialize the service hander. */

          DCM_HSM_INFO(instIdx).TxBuffer.FilledLength =
            (ServiceProcessingContextPtr->curMsgContext->resDataLen + 1U);

            (void) DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, instIdx, DCM_HSM_DCM_EV_PROC_DONE);
        }
        break;
      case DCM_E_DONE:
      default:
        /* Indicate that this service handler has finished all its operations. */
        DCM_HSM_INFO(instIdx).CancellationConditions.ServiceProcessingDone = TRUE;
        Dcm_Dsp_DefaultReturnValueProcessing(instIdx, ServiceProcessingContextPtr);
        break;
    }
  }
}

STATIC FUNC(void, DCM_CODE) Dcm_Dsp_DefaultReturnValueProcessing(
    uint8 instIdx,
    Dcm_ServiceProcessingContext_PtrType ServiceProcessingContextPtr)
{

#if (DCM_NUM_PROTOCOL == 1U)
  TS_PARAM_UNUSED(instIdx);
#endif /* #if (DCM_NUM_PROTOCOL == 1U) */


  if ((DCM_CONFIRMED_OK != ServiceProcessingContextPtr->OpStatus) &&
      (DCM_CONFIRMED_NOK != ServiceProcessingContextPtr->OpStatus))
  {
    /* If this is not the final call to the service handler with a confirmation.
       Update the FilledLength of the
       transmission buffer with the amount of data the service handler wrote, plus one
       byte for the service ID. */
    DCM_HSM_INFO(instIdx).TxBuffer.FilledLength =
      (ServiceProcessingContextPtr->curMsgContext->resDataLen + 1U);

      (void) DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, instIdx, DCM_HSM_DCM_EV_PROC_DONE);
  }
  else
  {
    /* This is always the final step of processing of a service handler. Release the
       service processor so that it may be used by another protocol. */
    Dcm_Dsp_ServiceProcessorReset(instIdx);

    (void) DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, instIdx, DCM_HSM_DCM_EV_PROC_DONE);
  }
}

#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)

FUNC(void, DCM_CODE) Dcm_ProcessServiceAsync(void)
{
  DBG_DCM_PROCESSSERVICEASYNC_ENTRY();
  /* ROE TYPE2 processing is always handled in a synchronous way. */
  if (DCM_UDS_ASYNCHRONOUS == Dcm_AsynchronousProtocol)
  {
    Dcm_Dsp_ServiceProcessorExecute(&Dcm_ServiceProcessingContext_UDS);
    Dcm_ServiceProcessingContext_UDS.AsynchronousProcessingState = DCM_ASYNC_PROCESSING_DATA_AVAILABLE;
  }
  else
  {


    Dcm_Dsp_ServiceProcessorExecute(&Dcm_ServiceProcessingContext_OBD);
    Dcm_ServiceProcessingContext_OBD.AsynchronousProcessingState = DCM_ASYNC_PROCESSING_DATA_AVAILABLE;
  }

  DBG_DCM_PROCESSSERVICEASYNC_EXIT();
}

STATIC FUNC(void, DCM_CODE) Dcm_TriggerServiceAsync(void)
{
  SchM_ActMainFunction_Dcm_Dcm_InternalTriggerServiceAsync();
}
#endif /* #if(DCM_ASYNC_SERVICE_HANDLER == STD_ON) */

FUNC(Dcm_GenericServiceHandlerType, DCM_CODE) Dcm_Dsp_ExtractSubServiceHandler(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Dcm_GenericServiceHandlerType handler = NULL_PTR;

#if (DCM_NUM_PROTOCOL > 1U)
  /* Get current protocol id */
  const uint8_least ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
         DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
#endif

  /* get configuration for current service table */
  CONSTP2CONST(Dcm_SidTabConfigType, AUTOMATIC, DCM_CONST) curSidTabConfig
    = &Dcm_SidTabConfig[DCM_PROTOCOL_CONFIG(ProtocolIndex).ServiceTableIndex];

  /* get the context information of current instance */
  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(ProtocolIndex);

  /* get corresponding sub-service array configuration */
  CONSTP2CONST(Dcm_SubSidTabEntryConfigType, AUTOMATIC, DCM_CONST) curSsConfig
    = (curSidTabConfig->Entries[curHsmInfo->sidTabEntryInd]).SubServices;

  /* get index of subservice function */
  const uint8 subSrvIdx = curHsmInfo->subServiceIdIdx;

  DBG_DCM_DSP_EXTRACTSUBSERVICEHANDLER_ENTRY(OpStatus, pMsgContext);

  DCM_PRECONDITION_ASSERT(((pMsgContext->reqDataLen >= 1U) || (OpStatus != DCM_INITIAL)),
      DCM_SSVCID_HANDLEREXTRACTOR);

  if (OpStatus == DCM_INITIAL)
  {
    /* set request data pointer to received data without service and sub-service Ids */
    pMsgContext->reqData = &pMsgContext->reqData[1U];

    /* decrement the reqData length as sub-function is removed */
    pMsgContext->reqDataLen = pMsgContext->reqDataLen - 1U;
  }

  if (curSsConfig[subSrvIdx].IntSrvHandler != NULL_PTR)
  {
    /* Assign internal sub-service handler */
    handler = curSsConfig[subSrvIdx].IntSrvHandler;
  }
  else
  {
    /* Assign External sub-service handler */
    handler = curSsConfig[subSrvIdx].ExtSrvHandler;
  }

  DBG_DCM_DSP_EXTRACTSUBSERVICEHANDLER_EXIT(handler, OpStatus, pMsgContext);
  return handler;
}

/*==================[Service and Sub-Service Handler definitions]================================*/

#if ((DCM_DSP_USE_SERVICE_0X28 == STD_ON) || (DCM_DSP_USE_SERVICE_0X85 == STD_ON))
FUNC(boolean, DCM_CODE) Dcm_DspInternal_GetSidConfig(
    uint8 instIdx,
    uint8 serviceId,
    P2VAR(Dcm_SidTabEntryConfigPtrType, AUTOMATIC, DCM_CONST) pServiceIdCfg)
{
  uint8_least sidIdxExt;
  boolean result = FALSE;
  /* get configuration for current HSM instance */
  CONSTP2CONST(Dcm_ProtocolConfigType, AUTOMATIC, DCM_CONST) curProtocolConfig =
      &DCM_PROTOCOL_CONFIG(instIdx);
  /* get configuration for current service table */
  CONSTP2CONST(Dcm_SidTabConfigType, AUTOMATIC, DCM_CONST) curSidTabConfig =
      &Dcm_SidTabConfig[curProtocolConfig->ServiceTableIndex];

#if (DCM_NUM_PROTOCOL == 1U)
    TS_PARAM_UNUSED(instIdx);
#endif

  DBG_DCM_DSPINTERNAL_GETSIDCONFIG_ENTRY(instIdx, serviceId, pServiceIdCfg);
  /* search list of configured services for service Id */
  for (sidIdxExt = 0U;
    (sidIdxExt < curSidTabConfig->NumEntries)&&(result == FALSE);
    sidIdxExt++)
  {
    if ((curSidTabConfig->Entries[sidIdxExt]).ServiceId == serviceId)
    {
      *pServiceIdCfg = &(curSidTabConfig->Entries[sidIdxExt]);
      result = TRUE;
    }
  }

  DBG_DCM_DSPINTERNAL_GETSIDCONFIG_EXIT(result, instIdx, serviceId, pServiceIdCfg);
  return result;
}
#endif /* ((DCM_DSP_USE_SERVICE_0X28 == STD_ON) || (DCM_DSP_USE_SERVICE_0X85 == STD_ON)) */

#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
FUNC(void, DCM_CODE) Dcm_Dsp_DiagnosticSessionControl_ConfirmSession(Dcm_SesCtrlType ConfirmedSession)
{
#if (DCM_NUM_PROTOCOL > 1U)
  const Dcm_ProtocolIndexType ProtocolIdx = DCM_PROTOCOL_INDEX_FROM_ID(Dcm_Dsl_Supervisor_GetLatestExternalUDSProtocol());

  if (DCM_DEFAULT_SESSION != ConfirmedSession)
  {
    /* Since the diagnostic session will only be changed to a non-default session as a result of a
       DiagnosticSessionControl (0x10) request, there will always be a current protocol since
       it had to exist in order to process the request. */
    DCM_INVARIANT_ASSERT(DCM_INVALID_PROTOCOL_INDEX != ProtocolIdx, DCM_INTERNAL_API_ID);
  }
  if (DCM_INVALID_PROTOCOL_INDEX != ProtocolIdx)
  {
#endif
    /* get information */
    CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(ProtocolIdx);

    if (curHsmInfo->msgContext.idContext == DCM_SID_SESSION_CONTROL)
    {
      curHsmInfo->confirmedDiagSession = ConfirmedSession;
    }
#if (DCM_NUM_PROTOCOL > 1U)
  }
#endif
}
#endif /* (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */
#if (DCM_DSP_USE_SERVICE_0X10 == STD_ON)
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_DiagnosticSessionControl_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  uint8 SessionIdx;

#if(DCM_JUMP_TO_BL_ENABLED == STD_ON)
  /* indicates whether the requested session is a boot session */
  boolean isBootSession = FALSE;
#endif

#if (DCM_NUM_PROTOCOL > 1U)
  /* Get current protocol id */
  uint8 instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                    DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);
#endif

  /* get information */
  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);

  /* Get SesTabConfig info */
  P2CONST(Dcm_SesTabConfigType, AUTOMATIC, DCM_CONST) sesTabConfig = &DCM_SES_TAB_CONFIG(instIdx);

  DBG_DCM_DSPINTERNAL_DIAGNOSTICSESSIONCONTROL_SVCH_ENTRY(OpStatus,pMsgContext);

  switch (OpStatus)
  {
    /* first call of service function */
    case DCM_INITIAL:

      /* get the session index of the new session */
      /* subfunction ID (= session level) validation was done by DSD */
      for (SessionIdx = 0U; SessionIdx < sesTabConfig->NumEntries; SessionIdx++)
      {
        if ((sesTabConfig->SessionEntries[SessionIdx]).SesCtrl == curHsmInfo->subServiceId)
        {
          break;
        }
      }

      Dcm_NewDiagnosticSessionTableIndex = SessionIdx;

#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
      curHsmInfo->confirmedResetMode = DCM_NO_RESET;
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */

      curHsmInfo->jumpToBLInhibited = FALSE;

#if(DCM_JUMP_TO_BL_ENABLED == STD_ON)
      if ((Dcm_NewDiagnosticSessionTableIndex < sesTabConfig->NumEntries) &&
         ((sesTabConfig->SessionEntries[Dcm_NewDiagnosticSessionTableIndex].SessionForBoot == DCM_OEM_BOOT ) ||
          (sesTabConfig->SessionEntries[Dcm_NewDiagnosticSessionTableIndex].SessionForBoot == DCM_SYS_BOOT)))
      {
        isBootSession = TRUE;
      }

      if ((TRUE == Dcm_Dsl_PostBootOperationsHandler_JumpInhibited()) &&
          (TRUE == isBootSession))
      {
        serviceProcessStatus = DCM_E_PENDING;

        curHsmInfo->jumpToBLInhibited = TRUE;
      }
      else
#endif
      {
        curHsmInfo->jumpToBLSupport = FALSE;
        curHsmInfo->jumpToBLExecution = DCM_PREPARE_JUMP;

        serviceProcessStatus = Dcm_DspInternal_DiagnosticSessionControl_SvcStart(pMsgContext);

        curHsmInfo->jumpToBLInhibited = FALSE;
      }

      break;

      /* If _SvcStart() has returned PENDING before
          or _SvcStart() call was inhibited by a still pending SetProgConditions() call */
    case DCM_PENDING:

#if(DCM_JUMP_TO_BL_ENABLED == STD_ON)
      /* if _SvcStart() call was inhibited by a still pending SetProgConditions() call */
      if (curHsmInfo->jumpToBLInhibited == TRUE)
      {
        /* if the previous SetProgConditions() call is still pending */
        if (TRUE == Dcm_Dsl_PostBootOperationsHandler_JumpInhibited())
        {
          serviceProcessStatus = DCM_E_PENDING;
        }
        else
        {
          /* the previous SetProgConditions() call has completed, call _SvcStart() */

          curHsmInfo->jumpToBLSupport = FALSE;
          curHsmInfo->jumpToBLExecution = DCM_PREPARE_JUMP;
          curHsmInfo->jumpToBLInhibited = FALSE;

          serviceProcessStatus = Dcm_DspInternal_DiagnosticSessionControl_SvcStart(pMsgContext);
        }
      }
      else
#endif
      {
        /* if bootloader support is not configured, service normal SessionControl request */
        if ( curHsmInfo->jumpToBLSupport != TRUE )
        {
          /* !LINKSTO SWS_Dcm_01372,1 */
          if (Dcm_Dsl_Supervisor_IsSessionClearedToSwitch((Dcm_SesCtrlType)curHsmInfo->subServiceId)
              == DCM_SUPERVISOR_SESSION_SWITCH_ACCEPT)
          {
            /* Session change request not yet accepted */
            serviceProcessStatus = Dcm_DspInternal_DiagnosticSessionControl_SvcCont(pMsgContext);
          }
          else
          {
            serviceProcessStatus = DCM_E_PENDING;
          }
        }
#if(DCM_JUMP_TO_BL_ENABLED == STD_ON)
        else
        {
          /* Service the Jump to BL request */
          serviceProcessStatus = Dcm_DspInternal_JumpToBL(pMsgContext, OpStatus);
        }
#endif /* #if(DCM_JUMP_TO_BL_ENABLED == STD_ON) */
      }

      break;

    /* successful transmission of the response */
    case DCM_CONFIRMED_OK:
      DCM_PRECONDITION_ASSERT((curHsmInfo->jumpToBLSupport != TRUE), DCM_SID_SESSION_CONTROL);
        /* if a positive response was transmitted for previous SessionControl request */
      if ( curHsmInfo->negativeResponseCode == DCM_E_OK )
      {
        /* switch to requested session */
        Dcm_Dsl_DiagnosticSessionHandler_SwitchCurrentSession((Dcm_SesCtrlType)curHsmInfo->subServiceId);
      }
      Dcm_Dsl_DiagnosticSessionHandler_ClearResetToDefaultSessionOccured(curHsmInfo->serviceId);
      serviceProcessStatus = DCM_E_DONE;
#if(DCM_JUMP_TO_BL_ENABLED == STD_ON)

#endif /* #if(DCM_JUMP_TO_BL_ENABLED == STD_ON) */
      break;

    default:
      serviceProcessStatus = DCM_E_DONE;
      curHsmInfo->jumpToBLSupport = FALSE;
      curHsmInfo->jumpToBLInhibited = FALSE;
      curHsmInfo->jumpToBLExecution = DCM_PREPARE_JUMP;
      /* reset the request mode parameters */
#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
      curHsmInfo->confirmedResetMode = DCM_NO_RESET;
      curHsmInfo->requestedResetMode = DCM_NO_RESET;
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */

      break;
  }

  DBG_DCM_DSPINTERNAL_DIAGNOSTICSESSIONCONTROL_SVCH_EXIT(serviceProcessStatus,
                                                         OpStatus,pMsgContext);
  return serviceProcessStatus;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_DiagnosticSessionControl_SvcStart(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  /* check request length */
  if (pMsgContext->reqDataLen != 1U)
  {
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    serviceProcessStatus = DCM_E_DONE;
  }
  else
  {
    /* assign the data from msgcontext for service to that of sub-function.*/
    /* set request data pointer to received data without service and sub-service Ids */
    pMsgContext->reqData = &pMsgContext->reqData[1U];
    /* decrement the reqData length as sub-function is removed */
    pMsgContext->reqDataLen = pMsgContext->reqDataLen - 1U;
    /* Call common sub-service handler */
    serviceProcessStatus = Dcm_DspInternal_DiagnosticSessionControlCommon_SSvcH(DCM_INITIAL,
      pMsgContext);
  }

  return serviceProcessStatus;
}

FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_DiagnosticSessionControlCommon_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

#if (DCM_NUM_PROTOCOL > 1U)
  /* Get current protocol id */
  uint8 instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                    DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);
#endif

  /* Get SesTabConfig info */
  P2CONST(Dcm_SesTabConfigType, AUTOMATIC, DCM_CONST) sesTabConfig = &DCM_SES_TAB_CONFIG(instIdx);

  DBG_DCM_DSPINTERNAL_DIAGNOSTICSESSIONCONTROLCOMMON_SSVCH_ENTRY(OpStatus,pMsgContext);

  TS_PARAM_UNUSED (OpStatus);

  if (Dcm_NewDiagnosticSessionTableIndex < sesTabConfig->NumEntries)
  {
    /* set context id for post processing a session change*/
    pMsgContext->idContext = DCM_SID_SESSION_CONTROL;
#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
    /* Set confirmed diagnostics session to invalid */
    DCM_HSM_INFO(instIdx).confirmedDiagSession = DCM_INVALID_SESSION;

#if (DCM_BSWM_API_ENABLED == STD_ON)
    /* invoke BswM with the requested session as parameter */
    BswM_Dcm_RequestSessionMode(DCM_HSM_INFO(instIdx).subServiceId);
#endif /* (DCM_BSWM_API_ENABLED == STD_ON) */
#endif /* (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */

    /* Set return value to PENDING status, so that the service handler is called again in
    the next main function cycle */
    serviceProcessStatus = DCM_E_PENDING;
  }
  else
  {
    /* requested session is not configured for the current protocol, return an error */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_SUBFUNCTIONNOTSUPPORTED);
    serviceProcessStatus = DCM_E_DONE;
  }

  DBG_DCM_DSPINTERNAL_DIAGNOSTICSESSIONCONTROLCOMMON_SSVCH_EXIT(serviceProcessStatus,
                                                                OpStatus,pMsgContext);
  return serviceProcessStatus;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_DiagnosticSessionControl_SvcCont(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

#if (DCM_NUM_PROTOCOL > 1U)
  /* Get current protocol id */
  uint8 instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                    DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);
#endif

#if (DCM_TIMING_VALUES_IN_POSITIVE_RESPONSE == STD_ON)
  uint16 P2StarTime;
#endif

#if((DCM_JUMP_TO_BL_ENABLED == STD_ON) || \
    (DCM_TIMING_VALUES_IN_POSITIVE_RESPONSE == STD_ON))

  /* Get SesTabConfig info */
  P2CONST(Dcm_SesTabConfigType, AUTOMATIC, DCM_CONST) sesTabConfig = &DCM_SES_TAB_CONFIG(instIdx);

#endif /* #if((DCM_JUMP_TO_BL_ENABLED == STD_ON) || \
              (DCM_TIMING_VALUES_IN_POSITIVE_RESPONSE == STD_ON)) */

  /* get information */
  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);

#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
  /* check if a ModeEntry confirmation is received from BswM */
  if ( curHsmInfo->confirmedDiagSession != DCM_INVALID_SESSION )
  {
    /* check if the session transition is accepted by BswM.
      ie requested mode is same as mode entered */
    if ( curHsmInfo->subServiceId == curHsmInfo->confirmedDiagSession )
    {
#endif

#if(DCM_JUMP_TO_BL_ENABLED == STD_ON)
      if (((sesTabConfig->SessionEntries[Dcm_NewDiagnosticSessionTableIndex].SessionForBoot) ==
          DCM_OEM_BOOT ) ||
          ((sesTabConfig->SessionEntries[Dcm_NewDiagnosticSessionTableIndex].SessionForBoot) ==
          DCM_SYS_BOOT))
      {
        curHsmInfo->jumpToBLSupport = TRUE;

        /* Initiate the Jump to BL request */
        serviceProcessStatus = Dcm_DspInternal_JumpToBL(pMsgContext, DCM_INITIAL);
      }
      else
      {
#endif  /* DCM_JUMP_TO_BL_ENABLED */
        /* !LINKSTO Dcm222,1 */
        pMsgContext->resData[0U] = curHsmInfo->subServiceId;
#if (DCM_TIMING_VALUES_IN_POSITIVE_RESPONSE == STD_ON)
        /* Assemble positive response for DiagnosticSessionControl request */
        P2StarTime =
          (uint16)((sesTabConfig->SessionEntries[Dcm_NewDiagnosticSessionTableIndex].P2StarMaxTime *
                                                    DCM_TASK_TIME) / DCM_P2STARTIME_RESOLUTION);

        pMsgContext->resData[1U] =
               (uint8)((sesTabConfig->SessionEntries[Dcm_NewDiagnosticSessionTableIndex].P2MaxTime *
                                                             DCM_TASK_TIME) >> DCM_BYTE_SWAPPING);
        pMsgContext->resData[2U] =
          (uint8)(sesTabConfig->SessionEntries[Dcm_NewDiagnosticSessionTableIndex].P2MaxTime *
            DCM_TASK_TIME);
        pMsgContext->resData[3U] = (uint8)(P2StarTime >> DCM_BYTE_SWAPPING);
        pMsgContext->resData[4U] = (uint8)(P2StarTime);
        pMsgContext->resDataLen = DCM_0x10_RESPONSELENGTH_TIMING_VALUES;
#else
        pMsgContext->resDataLen = DCM_0x10_RESPONSELENGTH_NO_TIMING_VALUES;
#endif
        serviceProcessStatus = DCM_E_DONE;
#if(DCM_JUMP_TO_BL_ENABLED == STD_ON)
      }
#endif
#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
    }
    else
    {
      /* Session transition not accepted. Set negative response */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
      serviceProcessStatus = DCM_E_DONE;
    }

    /* Reset confirmed diagnostic session */
    curHsmInfo->confirmedDiagSession = DCM_INVALID_SESSION;
  }
  else
  {
    serviceProcessStatus = DCM_E_PENDING;
  }
#endif

  return serviceProcessStatus;
}

#if(DCM_JUMP_TO_BL_ENABLED == STD_ON)
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_JumpToBL(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext,
  Dcm_OpStatusType OpStatus)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
#if ((DCM_MODE_DECLARATION_OEM_BL == STD_ON) \
  || (DCM_MODE_DECLARATION_SYS_BL == STD_ON))
  Std_ReturnType EcuResetResult = DCM_E_ABORT;
#endif /* #if ((DCM_MODE_DECLARATION_OEM_BL == STD_ON) \
            || (DCM_MODE_DECLARATION_SYS_BL == STD_ON)) */

#if (DCM_NUM_PROTOCOL > 1U)
  /* Get current protocol id */
  uint8 instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                    DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);
#endif

  /* Get SesTabConfig info */
  P2CONST(Dcm_SesTabConfigType, AUTOMATIC, DCM_CONST) sesTabConfig = &DCM_SES_TAB_CONFIG(instIdx);

  /* get information */
  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);

  CONSTP2CONST(Dcm_ProtocolConfigType, AUTOMATIC, DCM_CONST) curProtocolConfig
    = &DCM_PROTOCOL_CONFIG(instIdx);

  switch (OpStatus)
  {
    case DCM_INITIAL:
      /* If the session is used to jump to OEM boot-loader */
      if ( (sesTabConfig->SessionEntries[Dcm_NewDiagnosticSessionTableIndex].SessionForBoot) ==
            DCM_OEM_BOOT )
      {
#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
#if (DCM_BSWM_API_ENABLED == STD_ON)
        /* Invoke BswM and set the 'requestedResetMode' */
        BswM_Dcm_RequestResetMode(DCM_BOOTLOADER_RESET);
        curHsmInfo->requestedResetMode = DCM_BOOTLOADER_RESET;
#endif /* #if (DCM_BSWM_API_ENABLED == STD_ON) */
#else /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */
#if (DCM_MODE_DECLARATION_OEM_BL == STD_ON)
          /* Dcm532 */
        EcuResetResult = SchM_Switch_Dcm_DcmEcuReset(RTE_MODE_DcmEcuReset_JUMPTOBOOTLOADER);
#endif /* #if (DCM_MODE_DECLARATION_OEM_BL == STD_ON) */
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */
      }
      /* If the session is used to jump to System Supplier boot-loader */
      else
      {
#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
#if (DCM_BSWM_API_ENABLED == STD_ON)
        /* Invoke BswM and set the 'requestedResetMode' */
        BswM_Dcm_RequestResetMode(DCM_SS_BOOTLOADER_RESET);
        curHsmInfo->requestedResetMode = DCM_SS_BOOTLOADER_RESET;
#endif /* #if (DCM_BSWM_API_ENABLED == STD_ON) */
#else /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */
#if (DCM_MODE_DECLARATION_SYS_BL == STD_ON)
        /* Dcm592 */
        EcuResetResult = SchM_Switch_Dcm_DcmEcuReset(RTE_MODE_DcmEcuReset_JUMPTOSYSSUPPLIERBOOTLOADER);
#endif /* #if (DCM_MODE_DECLARATION_SYS_BL == STD_ON) */
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */
      }

#if ((DCM_MODE_DECLARATION_SYS_BL == STD_ON)||(DCM_MODE_DECLARATION_OEM_BL == STD_ON))
      if(EcuResetResult != SCHM_E_OK)
      {
        /* !LINKSTO Dcm.EB.EcuReset.NRC0x22,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
        serviceProcessStatus = DCM_E_DONE;
      }
      else
#endif /* #if ((DCM_MODE_DECLARATION_SYS_BL == STD_ON)||(DCM_MODE_DECLARATION_OEM_BL == STD_ON)) */
      {
        curHsmInfo->jumpToBLExecution = DCM_PREPARE_JUMP;
        serviceProcessStatus = DCM_E_PENDING;
      }
      break;

    case DCM_PENDING:

#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
      /* check if a reset mode entry confirmation is received from BswM */
      if ( curHsmInfo->confirmedResetMode != DCM_NO_RESET )
      {
        /* check if the reset request is accepted by BswM. ie requested reset mode
           is same as the reset mode entered */
        if ((curHsmInfo->confirmedResetMode) == (curHsmInfo->requestedResetMode))
        {
#endif
          if (curHsmInfo->jumpToBLExecution == DCM_PREPARE_JUMP)
          {
            /* Check if a 'ResponsePending' needs to be transmitted */
            if (curProtocolConfig->SendRespPendOnTransToBoot == TRUE)
            {
              /* Set status to indicate that Response pending Tx confirmation is Pending */
              curHsmInfo->responsePendingTxStatus = DCM_RP_PENDING_CONF;
              curHsmInfo->jumpToBLExecution = DCM_FIRST_RCRRP_PENDING;
              /* Forcing the event TIMEOUT2 to trigger the transmission of
                 'Response Pending' message */
              Dcm_Dsl_DiagnosticSessionHandler_StartP2Timer(DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                                                            DCM_INST(instIdx),
                                                            0U);

              serviceProcessStatus = DCM_E_PENDING;
            }
            else
            {
              curHsmInfo->jumpToBLExecution = DCM_JUMP_IN_PROGRESS;

              /* [Dcm720]: If the jump to boot-loader is requested and the configuration parameter
              DcmSendRespPendOnTransToBoot is set to FALSE, the DCM shall call
              Dcm_SetProgConditions() immediately */
              serviceProcessStatus = Dcm_DspInternal_ProcessSetProgConditions(pMsgContext);

              /* !LINKSTO Dcm.EB.ModeSwitch.JumpToBL.SetProgConditions,1 */
              if(serviceProcessStatus == DCM_E_OK)
              {
                /* Return ABORT since there is no response to send */
                serviceProcessStatus = DCM_E_ABORT;
              }
            }
          }
          else if (curHsmInfo->jumpToBLExecution == DCM_JUMP_IN_PROGRESS)
          {
            /* [Dcm535]: If the jump to boot-loader is requested and the configuration parameter
            DcmSendRespPendOnTransToBoot is set to TRUE, the DCM shall call Dcm_SetProgConditions()
            after confirmation of the transmission of NRC 0x78 (Response pending) */
            serviceProcessStatus = Dcm_DspInternal_ProcessSetProgConditions(pMsgContext);

            /* !LINKSTO Dcm.EB.ModeSwitch.JumpToBL.SetProgConditions,1 */
            if(serviceProcessStatus == DCM_E_OK)
            {
              /* Retrun ABORT since there is no response to send */
              serviceProcessStatus = DCM_E_ABORT;
            }
          }
          else
          {
            /* waiting for the confirmation to the RCRRP, nothing is done */
            serviceProcessStatus = DCM_E_PENDING;
          }
#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
        }
        else
        {
          curHsmInfo->jumpToBLSupport = FALSE;
          /* reset the jump to boot-loader state */
          curHsmInfo->jumpToBLExecution = DCM_PREPARE_JUMP;
#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
          /* reset the request mode parameters */
          curHsmInfo->confirmedResetMode = DCM_NO_RESET;
          curHsmInfo->requestedResetMode = DCM_NO_RESET;
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */
          /* Jump to boot-loader mode is not accepted, trigger the transmission of
             NRC 0x22 (Conditions not correct) */
          Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
          serviceProcessStatus = DCM_E_DONE;
        }
      }
      else
      {
        serviceProcessStatus = DCM_E_PENDING;
      }
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */

      break;

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_JUMPTOBL);
      break;
    /* CHECK: PARSE */
  }

  return serviceProcessStatus;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ProcessSetProgConditions(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType SetProgConditionsReturn = DCM_E_OK;
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  boolean ResetToDefaultSessionOccured;

  /* Get current protocol id */
  const uint8 instIdx = Dcm_Dsl_Supervisor_GetProtocolIndexFromRxPduIdAndRequestType(
                                             pMsgContext->dcmRxPduId, DCM_NONE);

  /* get information */
  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);

  /* get pointer to current message context */
  CONSTP2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) curMsgContext = &curHsmInfo->msgContext;

  /* Get tester source address */
  uint16 testerSourceAddr = Dcm_MainConnectionConfig[Dcm_RxPduIdConfig[
      pMsgContext->dcmRxPduId].MainConnectionIndex].TesterSourceAddress;

#if (DCM_NUM_PROTOCOL == 1U)
    TS_PARAM_UNUSED(instIdx);
#endif

  Dcm_ProgConditions.ProtocolId = DCM_PROTOCOL_CONFIG(instIdx).Type;
  Dcm_ProgConditions.Sid = DCM_SID_SESSION_CONTROL;
  Dcm_ProgConditions.SubFncId = curHsmInfo->subServiceId;
  Dcm_ProgConditions.TesterSourceAddr = testerSourceAddr;
  Dcm_ProgConditions.ReprogramingRequest = TRUE;

  /* Check if response required flag is set */
  if (curMsgContext->msgAddInfo.suppressPosResponse == TRUE)
  {
    Dcm_ProgConditions.ResponseRequired = FALSE;
  }
  else
  {
    Dcm_ProgConditions.ResponseRequired = TRUE;
  }

  /* Call Dcm_SetProgConditions() to store all relevant information,
     prior to jumping to the boot-loader */
  if(DCM_HSM_INFO(instIdx).SetProgConditionsFinished == FALSE)
  {
    SetProgConditionsReturn = Dcm_SetProgConditions(&Dcm_ProgConditions);
  }

  if( SetProgConditionsReturn == DCM_E_PENDING )
  {
    /* Set the variable to indicate 'SetProgConditions' returned Pending status */
    curHsmInfo->jumpToBLExecution = DCM_JUMP_IN_PROGRESS;

    serviceProcessStatus = DCM_E_PENDING;
  }
  else
  {
    if( SetProgConditionsReturn == DCM_E_OK )
    {
      /* Set ProgConditions execution as finished */
      DCM_HSM_INFO(instIdx).SetProgConditionsFinished = TRUE;

      ResetToDefaultSessionOccured =
                              Dcm_Dsl_DiagnosticSessionHandler_GetResetToDefaultSessionOccured(
                              curHsmInfo->serviceId);
      /* Invoke the BswM interface to perform the jump to boot-loader */
      /* only if all outstanding RCRRPs have been aknowledged. */
      if(DCM_HSM_INFO(instIdx).rpCntrConfirmed == DCM_HSM_INFO(instIdx).rpCntr)
      {
        /* !LINKSTO Dcm.DiagnosticSessionControl.BootSession.NoOBD.EXECUTE,1 */
          if ((Dcm_Dsl_Supervisor_IsSessionClearedToSwitch((Dcm_SesCtrlType)curHsmInfo->subServiceId)
                      == DCM_SUPERVISOR_SESSION_SWITCH_ACCEPT) &&
              (FALSE == ResetToDefaultSessionOccured))
        {
#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
#if (DCM_BSWM_API_ENABLED == STD_ON)
          /* !LINKSTO Dcm.EB.JumpToBl.OutstandingRCRRPs,1 */
          BswM_Dcm_RequestResetMode(DCM_RESET_EXECUTION);
#endif
#else
          /* Dcm719 */
          /* !LINKSTO Dcm.EB.JumpToBl.OutstandingRCRRPs,1 */
          (void)SchM_Switch_Dcm_DcmEcuReset(RTE_MODE_DcmEcuReset_EXECUTE);
#endif
#if (((DCM_MODEDECLARATION_SUPPORT == STD_OFF)&&\
      (DCM_BSWM_API_ENABLED == STD_ON)) ||\
     (DCM_MODEDECLARATION_SUPPORT == STD_ON))
        /* Stop timer 2 only if one of the interfaces has been called */
        /* !LINKSTO Dcm.EB.JumpToBl.P2TimerSuppression,1 */
        Dcm_Dsl_DiagnosticSessionHandler_StopTimer(DCM_PROTOCOL_ID_FROM_INDEX(DCM_INST(instIdx)),
                                                   DCM_INST(instIdx),
                                                   DCM_P2TIMER);
#endif
          /* Service execution finished. Now return E_OK to perform the RESET */
          serviceProcessStatus = DCM_E_OK;
        }
        else
        {
          /* OBD request processing is active, send NRC 0x22 (Conditions not correct) */
          /* !LINKSTO Dcm.DiagnosticSessionControl.BootSession.OBD.NRC0x22,1 */
          /* !LINKSTO Dcm.EB.SessionStateHandling.DiagnosticSessionControl.BootSession.BswM.ResetToDefaultSession.NRC_CNC,1 */
          /* !LINKSTO Dcm.EB.SessionStateHandling.DiagnosticSessionControl.BootSession.ModeSwitch.ResetToDefaultSession.NRC_CNC,1 */
          Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
          serviceProcessStatus = DCM_E_DONE;
#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
#if (DCM_BSWM_API_ENABLED == STD_ON)
          /* Set BswM_Dcm_RequestSessionMode back to the current session */
          BswM_Dcm_RequestSessionMode(Dcm_Dsl_DiagnosticSessionHandler_GetCurrentSession());
#endif /* #if (DCM_BSWM_API_ENABLED == STD_ON) */
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */
        }
      }
      else
      {
        serviceProcessStatus = DCM_E_PENDING;
      }
    }
    else
    {
#if (DCM_DEV_ERROR_DETECT == STD_ON)
      if ( SetProgConditionsReturn == DCM_E_NOT_OK )
      {
        DCM_DET_REPORT_ERROR(DCM_SVCID_SETPROGCONDITIONS, DCM_E_SET_PROG_CONDITIONS_FAIL);
      }
      else
      {
        /* !LINKSTO Dcm.EB.ServiceProcessing.DetEnabled.SetProgConditions.UnknownReturnValues,1 */
        DCM_DET_REPORT_ERROR(DCM_SVCID_SETPROGCONDITIONS, DCM_E_INTERFACE_RETURN_VALUE);
      }
#endif
      /* Dcm_SetProgConditions() failed; trigger the transmission of
         NRC 0x22 (Conditions not correct) */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
      serviceProcessStatus = DCM_E_DONE;
    }

    /* In case of successful or failed BL Jump, Reset the variables, unless waiting */
    if(serviceProcessStatus != DCM_E_PENDING)
    {
      curHsmInfo->jumpToBLSupport = FALSE;
#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
      /* reset the request mode parameters */
      curHsmInfo->confirmedResetMode = DCM_NO_RESET;
      curHsmInfo->requestedResetMode = DCM_NO_RESET;
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */
      curHsmInfo->jumpToBLExecution = DCM_PREPARE_JUMP;
    }
  }
  return serviceProcessStatus;
}
#endif /* #if(DCM_JUMP_TO_BL_ENABLED == STD_ON) */
#endif /* (DCM_DSP_USE_SERVICE_0X10 == STD_ON) */

#if(DCM_DSP_USE_SERVICE_0X11 == STD_ON)
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ECUReset_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

#if (DCM_NUM_PROTOCOL > 1U)
  /* Get current protocol id */
  uint8 curProtocolId = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                          DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);
#endif

  /* get information */
  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(curProtocolId);

  DBG_DCM_DSPINTERNAL_ECURESET_SVCH_ENTRY(OpStatus,pMsgContext);

  switch (OpStatus)
  {
    /* first call of service function */
    case DCM_INITIAL:
      serviceProcessStatus = Dcm_DspInternal_ECUReset_SvcStart(DCM_INITIAL, pMsgContext);
      break;

    /* Consecutive call after the function returned PENDING before */
    case DCM_PENDING:
      serviceProcessStatus = Dcm_DspInternal_ECUReset_SvcCont(DCM_PENDING, pMsgContext);
      break;

    /* Cancel the ongoing service processing */
    case DCM_CANCEL:
      serviceProcessStatus = Dcm_DspInternal_ECUReset_SvcCont(DCM_CANCEL, pMsgContext);
      break;

    /* actions performed on successful sending of the response */
    case DCM_CONFIRMED_OK:

      /* if a positive response was transmitted for previous 'ECUReset' request */
      if ( curHsmInfo->negativeResponseCode == DCM_E_OK )
      {
#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
#if (DCM_BSWM_API_ENABLED == STD_ON)
        /* Invoke the BswM interface for reset execution */
        BswM_Dcm_RequestResetMode(DCM_RESET_EXECUTION);
#endif
#else
        /* Dcm594 */
        (void)SchM_Switch_Dcm_DcmEcuReset(RTE_MODE_DcmEcuReset_EXECUTE);
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */

#if (DCM_DSP_USE_SERVICE_0X85 == STD_ON)
        if(Dcm_Dsp_ControlDTCSetting_GetState() == DCM_DSP_DTCSETTING_DISABLE)
        {
          /* !LINKSTO Dcm.EnableDTCSetting.TransToDefaultSession.Service0x11,2 */
          /* !LINKSTO Dcm.EnableDTCSetting.ModeSwitch.TransToDefaultSession.Service0x11,2 */
          /* If a successful ECUReset is performed, then DCM module shall call
           * Dem_EnableDTCSetting() to re-enable the setting of all DTCs, also made the ModeSwitch
           * see Dcm_Dsp_ControlDTCSetting_SetState() function. */
          Dcm_Dsp_ControlDTCSetting_ReEnable();
        }
#endif /* #if (DCM_DSP_USE_SERVICE_0X85 == STD_ON) */
      }
      serviceProcessStatus = DCM_E_DONE;
      break;

    default:


      serviceProcessStatus = DCM_E_DONE;
      break;
  }

  DBG_DCM_DSPINTERNAL_ECURESET_SVCH_EXIT(serviceProcessStatus,OpStatus,pMsgContext);
  return serviceProcessStatus;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ECUReset_SvcStart(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  /* holds the processing status of the service function */
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

#if (DCM_NUM_PROTOCOL > 1U)
  /* Get current protocol id */
  uint8 curProtocolId = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                          DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);
#endif

  /* get information */
  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(curProtocolId);

  /* get configuration for current service table */
  CONSTP2CONST(Dcm_SidTabConfigType, AUTOMATIC, DCM_CONST) curSidTabConfig
    = &Dcm_SidTabConfig[DCM_PROTOCOL_CONFIG(curProtocolId).ServiceTableIndex];

  /* get corresponding sub-service array configuration */
  CONSTP2CONST(Dcm_SubSidTabEntryConfigType, AUTOMATIC, DCM_CONST) curSsConfig
    = (curSidTabConfig->Entries[curHsmInfo->sidTabEntryInd]).SubServices;

  TS_PARAM_UNUSED(OpStatus);

  if (pMsgContext->reqDataLen == 1U)
  {
    /* get index of subservice function */
    const uint8 subSrvIdx = curHsmInfo->subServiceIdIdx;

    /* assign the data from msgcontext for service to that of sub-function.*/
    /* set request data pointer to received data without service and sub-service Ids */
    pMsgContext->reqData = &pMsgContext->reqData[1U];

    /* decrement the reqData length as sub-function is removed */
    pMsgContext->reqDataLen = pMsgContext->reqDataLen - 1U;

    if (curSsConfig[subSrvIdx].IntSrvHandler != NULL_PTR)
    {
      /* Call internal sub-service handler */
      serviceProcessStatus = curSsConfig[subSrvIdx].IntSrvHandler(DCM_INITIAL, pMsgContext);
    }
    else
    {
      /* Call External sub-service handler */
      serviceProcessStatus = curSsConfig[subSrvIdx].ExtSrvHandler(DCM_INITIAL, pMsgContext);

      /* Sub-service handlers return E_OK, so we must convert to DCM_E_DONE */
      if (serviceProcessStatus == E_OK)
      {
        serviceProcessStatus = DCM_E_DONE;
      }
    }
  }
  else
  {
    /* check request length */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    serviceProcessStatus = DCM_E_DONE;
  }

  return serviceProcessStatus;
}

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ECUReset_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

#if (DCM_NUM_PROTOCOL > 1U)
  /* Get current protocol id */
  uint8 curProtocolId = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                          DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);
#endif

  /* get information */
  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(curProtocolId);

  /* get configuration for current service table */
  CONSTP2CONST(Dcm_SidTabConfigType, AUTOMATIC, DCM_CONST) curSidTabConfig
    = &Dcm_SidTabConfig[DCM_PROTOCOL_CONFIG(curProtocolId).ServiceTableIndex];

  /* get corresponding sub-service array configuration */
  CONSTP2CONST(Dcm_SubSidTabEntryConfigType, AUTOMATIC, DCM_CONST) curSsConfig
    = (curSidTabConfig->Entries[curHsmInfo->sidTabEntryInd]).SubServices;

  /* get index of subservice function */
  const uint8 subSrvIdx = curHsmInfo->subServiceIdIdx;

  if (curSsConfig[subSrvIdx].IntSrvHandler != NULL_PTR)
  {
    if (OpStatus != DCM_CANCEL)
    {
#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
      /* check if a reset mode entry confirmation is received from BswM */
      if (curHsmInfo->confirmedResetMode != DCM_NO_RESET)
      {
        /* check if the reset request is accepted by BswM. ie requested reset mode
           is same as the reset mode entered */
        if ((curHsmInfo->confirmedResetMode) == (curHsmInfo->requestedResetMode))
        {
          /* if the confirmed mode is 'enableRapidPowerShutDown' */
          if (curHsmInfo->confirmedResetMode == DCM_ENABLE_RAPID_POWER_SHUTDOWN_RESET )
          {
            /* assemble positive response */
            pMsgContext->resDataLen = 2U;

            /* set the powerDownTime in response */
            pMsgContext->resData[1U] = DCM_DSP_POWER_DOWN_TIME;
          }
          else
          {
            /* assemble positive response */
            pMsgContext->resDataLen = 1U;
          }
          pMsgContext->resData[0U] = curHsmInfo->subServiceId;
        }
        else
        {
          /* reset request not accepted. Set negative response */
          Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
        }
        /* reset the request mode parameters */
        curHsmInfo->confirmedResetMode = DCM_NO_RESET;
        curHsmInfo->requestedResetMode = DCM_NO_RESET;
        serviceProcessStatus = DCM_E_DONE;
      }
      else
      {
        serviceProcessStatus = DCM_E_PENDING;
      }
#else
      if (curHsmInfo->subServiceId == DCM_ENABLE_RAPID_POWER_SHUTDOWN_RESET)
      {
        /* assemble positive response */
        pMsgContext->resDataLen = 2U;
        /* set the powerDownTime in response */
        pMsgContext->resData[1U] = DCM_DSP_POWER_DOWN_TIME;
      }
      else
      {
        /* assemble positive response */
        pMsgContext->resDataLen = 1U;
      }
      pMsgContext->resData[0U] = curHsmInfo->subServiceId;
     serviceProcessStatus = DCM_E_DONE;
#endif
    }
    else
    {
      serviceProcessStatus = DCM_E_DONE;
    }
  }
  else
  {
    /* Call External sub-service handler */
    serviceProcessStatus = curSsConfig[subSrvIdx].ExtSrvHandler(OpStatus, pMsgContext);

    /* Sub-service handlers return E_OK, so we must convert to DCM_E_DONE */
    if (serviceProcessStatus == E_OK)
    {
      serviceProcessStatus = DCM_E_DONE;
    }
  }

  return serviceProcessStatus;
}

#if (DCM_DSP_SUBSERVICE_HARDRESET_0x01 == STD_ON)
/* This is the UDS service 0x11-0x01 handler */
/* Deviation MISRAC2012-1 <+3>*/
/* Deviation MISRA-2 <+3> */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_HardReset_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_PENDING;
#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
  Std_ReturnType EcuResetResult = DCM_E_ABORT;
#endif
  DBG_DCM_DSPINTERNAL_HARDRESET_SSVCH_ENTRY(OpStatus,pMsgContext);

  TS_PARAM_UNUSED(OpStatus);
#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
#if (DCM_BSWM_API_ENABLED == STD_ON)
  /* Set Reset Mode */
  Dcm_DspInternal_SetResetMode(pMsgContext);
#else /* #if (DCM_BSWM_API_ENABLED == STD_ON) */
  TS_PARAM_UNUSED(pMsgContext);
#endif /* #if (DCM_BSWM_API_ENABLED == STD_ON) */
#else /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */
  TS_PARAM_UNUSED(pMsgContext);
  /* Dcm373 */
  EcuResetResult = SchM_Switch_Dcm_DcmEcuReset(RTE_MODE_DcmEcuReset_HARD);

  if(EcuResetResult != SCHM_E_OK)
  {
    /* !LINKSTO Dcm.EB.EcuReset.NRC0x22,1 */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
    serviceProcessStatus = DCM_E_DONE;
  }
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */

  DBG_DCM_DSPINTERNAL_HARDRESET_SSVCH_EXIT(DCM_E_PENDING,OpStatus,pMsgContext);
  return serviceProcessStatus;
}
#endif  /* #if (DCM_DSP_SUBSERVICE_HARDRESET_0x01 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_KEYOFFONRESET_0x02 == STD_ON)
/* This is the UDS service 0x11-0x02 handler */
/* Deviation MISRAC2012-1 <+3>*/
/* Deviation MISRA-2 <+3> */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_KeyOffOnReset_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_PENDING;
#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
  Std_ReturnType EcuResetResult = DCM_E_ABORT;
#endif
  DBG_DCM_DSPINTERNAL_KEYOFFONRESET_SSVCH_ENTRY(OpStatus,pMsgContext);

  TS_PARAM_UNUSED(OpStatus);
#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
#if (DCM_BSWM_API_ENABLED == STD_ON)
  /* Set Reset Mode */
  Dcm_DspInternal_SetResetMode(pMsgContext);
#else /* #if (DCM_BSWM_API_ENABLED == STD_ON) */
  TS_PARAM_UNUSED(pMsgContext);
#endif /* #if (DCM_BSWM_API_ENABLED == STD_ON) */
#else /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */
  TS_PARAM_UNUSED(pMsgContext);
  /* Dcm373 */
  EcuResetResult = SchM_Switch_Dcm_DcmEcuReset(RTE_MODE_DcmEcuReset_KEYONOFF);
  if(EcuResetResult != SCHM_E_OK)
  {
    /* !LINKSTO Dcm.EB.EcuReset.NRC0x22,1 */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
    serviceProcessStatus = DCM_E_DONE;
  }
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */

  DBG_DCM_DSPINTERNAL_KEYOFFONRESET_SSVCH_EXIT(DCM_E_PENDING,OpStatus,pMsgContext);
  return serviceProcessStatus;
}
#endif  /* #if (DCM_DSP_SUBSERVICE_KEYOFFONRESET_0x02 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_SOFTRESET_0x03 == STD_ON)
/* This is the UDS service 0x11-0x03 handler */
/* Deviation MISRAC2012-1 <+3>*/
/* Deviation MISRA-2 <+3> */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_SoftReset_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_PENDING;
#if (DCM_MODEDECLARATION_SUPPORT == STD_ON)
  Std_ReturnType EcuResetResult = DCM_E_ABORT;
#endif
  DBG_DCM_DSPINTERNAL_SOFTRESET_SSVCH_ENTRY(OpStatus,pMsgContext);

  TS_PARAM_UNUSED(OpStatus);
#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
#if (DCM_BSWM_API_ENABLED == STD_ON)
  /* Set Reset Mode */
  Dcm_DspInternal_SetResetMode(pMsgContext);
#else /* #if (DCM_BSWM_API_ENABLED == STD_ON) */
  TS_PARAM_UNUSED(pMsgContext);
#endif /* #if (DCM_BSWM_API_ENABLED == STD_ON) */
#else /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */
  TS_PARAM_UNUSED(pMsgContext);
  /* Dcm373 */
  EcuResetResult = SchM_Switch_Dcm_DcmEcuReset(RTE_MODE_DcmEcuReset_SOFT);
  if(EcuResetResult != SCHM_E_OK)
  {
    /* !LINKSTO Dcm.EB.EcuReset.NRC0x22,1 */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
    serviceProcessStatus = DCM_E_DONE;
  }
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */

  DBG_DCM_DSPINTERNAL_SOFTRESET_SSVCH_EXIT(DCM_E_PENDING,OpStatus,pMsgContext);
  return serviceProcessStatus;
}
#endif  /* #if (DCM_DSP_SUBSERVICE_SOFTRESET_0x03 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_ENABLERAPIDPOWERSHUTDOWN_0x04 == STD_ON)
/* This is the UDS service 0x11-0x04 handler */
/* Deviation MISRAC2012-1 <+3>*/
/* Deviation MISRA-2 <+3> */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_EnableRapidPowerShutDown_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  DBG_DCM_DSPINTERNAL_ENABLERAPIDPOWERSHUTDOWN_SSVCH_ENTRY(OpStatus,pMsgContext);

  TS_PARAM_UNUSED(OpStatus);
#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
#if (DCM_BSWM_API_ENABLED == STD_ON)
  /* Set Reset Mode */
  Dcm_DspInternal_SetResetMode(pMsgContext);
#else /* #if (DCM_BSWM_API_ENABLED == STD_ON) */
  TS_PARAM_UNUSED(pMsgContext);
#endif /* #if (DCM_BSWM_API_ENABLED == STD_ON) */
#else /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */
  TS_PARAM_UNUSED(pMsgContext);
  /* Dcm818 */
  (void)SchM_Switch_Dcm_DcmRapidPowerShutdown(
    RTE_MODE_DcmRapidPowerShutdown_ENABLE_RAPIDPOWERSHUTDOWN);
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */

  DBG_DCM_DSPINTERNAL_ENABLERAPIDPOWERSHUTDOWN_SSVCH_EXIT(DCM_E_PENDING,OpStatus,pMsgContext);
  return DCM_E_PENDING;
}
#endif  /* #if (DCM_DSP_SUBSERVICE_ENABLERAPIDPOWERSHUTDOWN_0x04 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_DISABLERAPIDPOWERSHUTDOWN_0x05 == STD_ON)
/* This is the UDS service 0x11-0x05 handler */
/* Deviation MISRAC2012-1 <+3>*/
/* Deviation MISRA-2 <+3> */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_DisableRapidPowerShutDown_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  DBG_DCM_DSPINTERNAL_DISABLERAPIDPOWERSHUTDOWN_SSVCH_ENTRY(OpStatus,pMsgContext);

  TS_PARAM_UNUSED(OpStatus);
#if (DCM_MODEDECLARATION_SUPPORT == STD_OFF)
#if (DCM_BSWM_API_ENABLED == STD_ON)
  /* Set Reset Mode */
  Dcm_DspInternal_SetResetMode(pMsgContext);
#else /* #if (DCM_BSWM_API_ENABLED == STD_ON) */
  TS_PARAM_UNUSED(pMsgContext);
#endif /* #if (DCM_BSWM_API_ENABLED == STD_ON) */
#else /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */
  TS_PARAM_UNUSED(pMsgContext);
  /* Dcm818 */
  (void)SchM_Switch_Dcm_DcmRapidPowerShutdown(
    RTE_MODE_DcmRapidPowerShutdown_DISABLE_RAPIDPOWERSHUTDOWN);
#endif /* #if (DCM_MODEDECLARATION_SUPPORT == STD_OFF) */

  DBG_DCM_DSPINTERNAL_DISABLERAPIDPOWERSHUTDOWN_SSVCH_EXIT(DCM_E_PENDING,OpStatus,pMsgContext);
  return DCM_E_PENDING;
}
#endif  /* #if (DCM_DSP_SUBSERVICE_DISABLERAPIDPOWERSHUTDOWN_0x05 == STD_ON) */

#if ((DCM_DSP_SUBSERVICE_HARDRESET_0x01 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_KEYOFFONRESET_0x02 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_SOFTRESET_0x03 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_ENABLERAPIDPOWERSHUTDOWN_0x04 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_DISABLERAPIDPOWERSHUTDOWN_0x05 == STD_ON))

#if ((DCM_MODEDECLARATION_SUPPORT == STD_OFF) && (DCM_BSWM_API_ENABLED == STD_ON))
/* this function is to set & perform request reset mode */
STATIC FUNC(void, DCM_CODE) Dcm_DspInternal_SetResetMode(
  P2CONST(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
#if (DCM_NUM_PROTOCOL > 1U)
  /* Get current protocol id */
  uint8 curProtocolId = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                          DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId), DCM_EXTERNAL);
#endif

  /* get information */
  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(curProtocolId);

  /* store softReset value to local buffer*/
  Dcm_ResetModeType resetModeType = curHsmInfo->subServiceId;

#if ((DCM_NUM_PROTOCOL == 1U) && (DCM_DBG_ENABLED == STD_OFF))
  TS_PARAM_UNUSED(pMsgContext);
#endif

  /* Update the requested Reset Mode and reset confirmed Reset Mode */
  curHsmInfo->requestedResetMode = resetModeType;
  curHsmInfo->confirmedResetMode = DCM_NO_RESET;

  /* Invoke the BswM interface with requested Reset Mode as parameter */
  BswM_Dcm_RequestResetMode(resetModeType);
}
#endif /* #if ((DCM_MODEDECLARATION_SUPPORT == STD_OFF) && (DCM_BSWM_API_ENABLED == STD_ON)) */

#endif  /* #if ((DCM_DSP_SUBSERVICE_HARDRESET_0x01 == STD_ON) || \
                (DCM_DSP_SUBSERVICE_KEYOFFONRESET_0x02 == STD_ON) || \
                (DCM_DSP_SUBSERVICE_SOFTRESET_0x03 == STD_ON) || \
                (DCM_DSP_SUBSERVICE_ENABLERAPIDPOWERSHUTDOWN_0x04 == STD_ON) || \
                (DCM_DSP_SUBSERVICE_DISABLERAPIDPOWERSHUTDOWN_0x05 == STD_ON))  */

#endif /* #if (DCM_DSP_USE_SERVICE_0X11 == STD_ON) */

#if(DCM_DSP_USE_SERVICE_0X14 == STD_ON)
/* !LINKSTO SWS_Dcm_00247,1 */
/* This is the generic service handler for UDS service 0x14 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ClearDiagnosticInformation_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  uint32 reqDTC;
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  Dcm_NegativeResponseCodeType Nrc = E_OK;
#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U) &&\
     (DCM_POSTCONDITION_ASSERT_ENABLED == STD_ON))
   Std_ReturnType DcmFacadeLockStatus = DCM_E_RESOURCE_LOCKED;
#endif

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));

  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSPINTERNAL_CLEARDIAGNOSTICINFORMATION_SVCH_ENTRY(OpStatus,pMsgContext);

    switch (OpStatus)
    {
      case DCM_INITIAL:
        /* Check if length of request data is correct */
        if (pMsgContext->reqDataLen != 3U)
        {
          /* Invalid request length */
          /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.WrongMsgLength,1 */
          Nrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
          serviceProcessStatus = DCM_E_DONE;
        }
        else
        {
          reqDTC = ((uint32)pMsgContext->reqData[0U] << 16U) |
                   ((uint32)pMsgContext->reqData[1U] << 8U)  |
                   ((uint32)pMsgContext->reqData[2U]);

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
#if (DCM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
          /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.DemFacade.Locking,1 */
          DcmFacadeLockStatus = Dcm_Dsp_DemFacade_LockClient(DemClientId);
#else
          (void)Dcm_Dsp_DemFacade_LockClient(DemClientId);
#endif /* #if (DCM_POSTCONDITION_ASSERT_ENABLED == STD_ON) */

          /* The above locking operation should never fail */
          DCM_POSTCONDITION_ASSERT(DcmFacadeLockStatus != DCM_E_RESOURCE_LOCKED, DCM_INTERNAL_API_ID);

#endif /* #if((DCM_HAS_ROE_TYPE2 == STD_ON)&&(DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U)) */

          /* !LINKSTO SWS_Dcm_01263,1 */
          serviceProcessStatus = Dcm_Dsp_DemFacade_ClearDTCInfo_Setup(DemClientId, reqDTC,
                                       DEM_DTC_FORMAT_UDS, DEM_DTC_ORIGIN_PRIMARY_MEMORY, &Nrc);
          if (E_OK == Nrc)
          {
            serviceProcessStatus = Dcm_Dsp_DemFacade_ClearDTCInfo_Process(DemClientId, &Nrc);
          }
        }
        break;
      /* Status PENDING was returned before */
      case DCM_PENDING:
        serviceProcessStatus = Dcm_Dsp_DemFacade_ClearDTCInfo_Process(DemClientId, &Nrc);
        break;

      case DCM_CANCEL:
        /* !LINKSTO Dcm.EB.ClearDiagnosticInformation.Canceling,1 */
        serviceProcessStatus = Dcm_Dsp_DemFacade_ClearDTCInfo_Cancel(DemClientId);
        break;

      /* CHECK: NOPARSE */
      default:
      {
        /* No need of any processing for all the remaining operation states */
        serviceProcessStatus = DCM_E_DONE;
        break;
      }
      /* CHECK: PARSE */
    }

    if ((E_OK != Nrc) && (serviceProcessStatus == DCM_E_DONE))
    {
      /* Set negative response code */
      Dcm_ExternalSetNegResponse(pMsgContext, Nrc);
    }

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
  if (( (serviceProcessStatus == DCM_E_OK)
      ||(serviceProcessStatus == DCM_E_NOT_OK)
      ||(serviceProcessStatus == DCM_E_ABORT)
      ||(serviceProcessStatus == DCM_E_DONE)
     ) && (OpStatus != DCM_CONFIRMED_OK) && (OpStatus != DCM_CONFIRMED_NOK) &&
      (pMsgContext->reqDataLen == 3U))
  {
    /* Unlock DemFacade */
    Dcm_Dsp_DemFacade_UnlockClient(DemClientId);
  }
#endif

  DBG_DCM_DSPINTERNAL_CLEARDIAGNOSTICINFORMATION_SVCH_EXIT(serviceProcessStatus,
                                                           OpStatus,pMsgContext);
  return serviceProcessStatus;
}

#endif /* (DCM_DSP_USE_SERVICE_0X14 == STD_ON) */

/*==================[internal function definitions]==============================================*/

/* if Paged buffer handling enabled */
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
FUNC(void, DCM_CODE) Dcm_DspInternal_UpdatePage(
    Dcm_MsgType PagedBufPtr,
    Dcm_MsgLenType PageLen)
{
  DBG_DCM_DSPINTERNAL_UPDATEPAGE_ENTRY(PagedBufPtr,PageLen);

#if (DCM_DEV_ERROR_DETECT == STD_ON)
  if (Dcm_Initialized != TRUE)
  {
    DCM_DET_REPORT_ERROR(DCM_SERVID_DSD_UPDATEPAGE, DCM_E_UNINIT);
  }
  else
#endif /* DCM_DEV_ERROR_DETECT == STD_ON */
  {
  /* get HSM instance which corresponds to the Tx Pdu Id used by current protocol
   *
   * Due to the lack of an argument defining the protocol instance in the function signature,
   * it is not possible to derive the current HSM instance.
   * Consequently, we have to derive it from the RxPduID on which the request was received.
   */
    const uint8 ProtocolIdx = DCM_PROTOCOL_INDEX_FROM_ID(Dcm_Dsl_Supervisor_GetLatestExternalUDSProtocol());
    DCM_INVARIANT_ASSERT(DCM_INVALID_PROTOCOL_INDEX != ProtocolIdx, DCM_INTERNAL_API_ID);

    /* Update the response data buffer with PagedBufPtr */
    DCM_HSM_INFO(ProtocolIdx).msgContext.resData = PagedBufPtr;

    /* Update the response data buffer size */
    DCM_HSM_INFO(ProtocolIdx).msgContext.resMaxDataLen = PageLen;

    Dcm_DspInternal_SvcFunc(ProtocolIdx, DCM_UPDATE_PAGE);
  }

  DBG_DCM_DSPINTERNAL_UPDATEPAGE_EXIT(PagedBufPtr,PageLen);
}

FUNC(void, DCM_APPL_CODE) Dcm_DspInternal_CancelPagedBufferProcessing(void)
{
  /* get HSM instance which corresponds to the Tx Pdu Id used by current protocol
   *
   * Due to the lack of an argument defining the protocol instance in the function signature,
   * it is not possible to derive the current HSM instance.
   * Consequently, we have to derive it from the RxPduID on which the request was received.
   */
  const uint8 ProtocolIdx = DCM_PROTOCOL_INDEX_FROM_ID(Dcm_Dsl_Supervisor_GetLatestExternalUDSProtocol());
  DCM_INVARIANT_ASSERT(DCM_INVALID_PROTOCOL_INDEX != ProtocolIdx, DCM_INTERNAL_API_ID);

  DBG_DCM_DSPINTERNAL_CANCELPAGEDBUFFERPROCESSING_ENTRY();

  Dcm_Dsp_ProtocolCancel(DCM_PROTOCOL_ID_FROM_INDEX(ProtocolIdx));

  DBG_DCM_DSPINTERNAL_CANCELPAGEDBUFFERPROCESSING_EXIT();
}
#endif /* #if (DCM_PAGEDBUFFER_ENABLED == STD_ON) */

FUNC(void, DCM_APPL_CODE) Dcm_Dsp_ProtocolCancelProcessing(Dcm_ProtocolIndexType ProtocolIndex)
{
  DBG_DCM_DSP_PROTOCOLCANCELPROCESSING_ENTRY(ProtocolIndex);
  /* Call the service handler function one last time in order to notify pending processes
     of the cancellation. This is only done if by this point the service handler hasn't already
     returned a final error code. */
  if (DCM_HSM_INFO(ProtocolIndex).CancellationConditions.ServiceProcessingDone != TRUE)
  {
    Dcm_DspInternal_SvcFunc(ProtocolIndex, DCM_CANCEL);
  }
  else
  {
    /* The service handler has already finished processing. No need to cancel it anymore.
       Acknowledge a successful cancellation. */
#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
    /* Cancellation has been successfully performed. */
    DCM_HSM_INFO(ProtocolIndex).CancellationConditions.RetryAsyncServiceCancellation =
      DCM_ASYNC_SERVICE_HANDLER_INVALID_CANCELLATION;
#endif /* #if (DCM_ASYNC_SERVICE_HANDLER == STD_ON) */
    Dcm_Dsp_CancellationDone(DCM_SERVICE_CANCELLATION, ProtocolIndex);

    /* This is always the final step of processing of a service handler. Release the
       service processor so that it may be used by another protocol. */
    if (DCM_HSM_INFO(ProtocolIndex).CancellationConditions.CancellationType !=
          DCM_CANCEL_PG_BUF_SENDING)
    {
      Dcm_Dsp_ServiceProcessorReset(ProtocolIndex);
    }
  }
  DBG_DCM_DSP_PROTOCOLCANCELPROCESSING_EXIT(ProtocolIndex);
}

FUNC(void, DCM_CODE) Dcm_DspInternal_DcmConfirmation(
  Dcm_ProtocolIndexType ProtocolIndex,
  Dcm_ConfirmationStatusType Status)
{
  uint8_least sessionIdx;
  uint8 Sid;
  uint8 ReqType;
  Dcm_MainConnectionIndexType MainConnIdx;
  uint16 SourceAddress;
  Dcm_SesCtrlType newSesCtrl = 0U;
  P2CONST(Dcm_SesTabConfigType, AUTOMATIC, DCM_CONST) sesTabConfig;

  /* get the context information current instance */
  P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(ProtocolIndex);

  DBG_DCM_DSPINTERNAL_DCMCONFIRMATION_ENTRY(ProtocolIndex,Status);

  Sid = curHsmInfo->serviceId;
  ReqType = Dcm_RxPduIdConfig[curHsmInfo->RxConnectionId].AddressingType;

  MainConnIdx = Dcm_RxPduIdConfig[curHsmInfo->RxConnectionId].MainConnectionIndex;

  SourceAddress = Dcm_MainConnectionConfig[MainConnIdx].TesterSourceAddress;
#if (DCM_META_DATA_HANDLING_ENABLED == STD_ON)
  if( TRUE == Dcm_MainConnectionConfig[MainConnIdx].MetaDataIsConfigured )
  {
    /* !LINKSTO Dcm.EB.LowerLayerInterfaces.DcmGenericConnectionUsage.Xxx_Confirmation.TargetAddress,1 */
    SourceAddress = Dcm_PduInfo[Dcm_MainConnectionConfig[MainConnIdx].GenericConnectionIndex].TargetAddress;
  }
#endif /* #if (DCM_META_DATA_HANDLING_ENABLED == STD_ON) */

#if ((DCM_INCLUDE_RTE == STD_ON) && \
  ((DCM_REQUEST_MANUFACTURER_NOTIFICATION_ENABLED == STD_ON) || \
  (DCM_REQUEST_SUPPLIER_NOTIFICATION_ENABLED == STD_ON)) )
  {
#if (DCM_REQUEST_MANUFACTURER_NOTIFICATION_ENABLED == STD_ON)
  /* Deviation TASKING-1 */
  for (sessionIdx = 0U; sessionIdx < DCM_RTE_TABLE_REQUEST_MANUFACTURER_SIZE; ++sessionIdx)
    {
      (void)(Dcm_RteManufacturerNotificationTable[sessionIdx].Confirmation
                                      (Sid, ReqType, SourceAddress, Status));
    }
#endif /* #if (DCM_REQUEST_MANUFACTURER_NOTIFICATION_ENABLED == STD_ON) */
#if (DCM_REQUEST_SUPPLIER_NOTIFICATION_ENABLED == STD_ON)
  /* Deviation TASKING-1 */
  for (sessionIdx = 0U; sessionIdx < DCM_RTE_TABLE_REQUEST_SUPPLIER_SIZE; ++sessionIdx)
    {
      (void)(Dcm_RteSupplierNotificationTable[sessionIdx].Confirmation
                                      (Sid, ReqType, SourceAddress, Status));
    }
#endif /* #if (DCM_REQUEST_SUPPLIER_NOTIFICATION_ENABLED == STD_ON) */
  }
#else /* #if ((DCM_INCLUDE_RTE == STD_ON) && \
  ((DCM_REQUEST_MANUFACTURER_NOTIFICATION_ENABLED == STD_ON) || \
  (DCM_REQUEST_SUPPLIER_NOTIFICATION_ENABLED == STD_ON)) ) */
  Rte_DcmConfirmation(Sid, ReqType, SourceAddress, Status);
#endif /* #if(DCM_INCLUDE_RTE == STD_ON) && \
       ((DCM_REQUEST_MANUFACTURER_NOTIFICATION_ENABLED == STD_ON) || \
       (DCM_REQUEST_SUPPLIER_NOTIFICATION_ENABLED == STD_ON)) */

  if (
      (DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST == curHsmInfo->RequestType) ||
      (DCM_JUMP_FROM_BL_RESPONSE_REQUEST == curHsmInfo->RequestType)
     )
  {

    /* Switch session to the SubFncId in Dcm_ProgConditionsType only in case of jump from the bootloader
       i.e. if the warm start was triggered by the UDS Service DiagnosticSessionControl (0x10).*/
    if (DCM_SID_SESSION_CONTROL == Dcm_ProgConditions.Sid)
    {
      /* Response successfully transmitted; set the active session */
      newSesCtrl = Dcm_ProgConditions.SubFncId;

      /* Get SesTabConfig info */
      sesTabConfig = &DCM_SES_TAB_CONFIG(ProtocolIndex);

      /* Get active session index */
      for (sessionIdx = 0U; sessionIdx < sesTabConfig->NumEntries; ++sessionIdx)
      {
        if ((sesTabConfig->SessionEntries[sessionIdx]).SesCtrl == newSesCtrl)
        {
          uint16_least RxPduIdIndex = 0U;
          boolean RxPduIdFound = FALSE;

          /* Get the 'RxPduId' corresponding to the tester 'SourceAddress' */
          /* Deviation TASKING-2 */
          while (FALSE == RxPduIdFound)
          {
            if (Dcm_MainConnectionConfig[Dcm_RxPduIdConfig[RxPduIdIndex].MainConnectionIndex].
                  TesterSourceAddress == Dcm_ProgConditions.TesterSourceAddr)
            {
              RxPduIdFound = TRUE;
            }
            else
            {
              RxPduIdIndex++;
            }
          }

          /* set new active session */
          Dcm_Dsl_DiagnosticSessionHandler_SetSession(
            Dcm_RxPduIdConfig[RxPduIdIndex].MainConnectionIndex, newSesCtrl);
        }
      }
    }
    /* Emit PROC_DONE after the confirmation */
    (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, ProtocolIndex, DCM_HSM_DCM_EV_PROC_DONE);

    Dcm_Dsl_PostBootOperationsHandler_ActionsCompleted();
  }
  else
  {
    /* Provide confirmation for internal service */
    if (DCM_HSM_INFO(ProtocolIndex).dspService)
    {
      Dcm_OpStatusType OpStatus;
      if((Status == DCM_RES_POS_OK) || (Status == DCM_RES_NEG_OK))
      {
        OpStatus = DCM_CONFIRMED_OK;
      }
      else
      {
        OpStatus = DCM_CONFIRMED_NOK;
      }

      /* service handler should not be invoked in case of failures during service validation */
      if(DCM_HSM_INFO(ProtocolIndex).requestValidationStatus == TRUE)
      {
        /* No failures during service request validation */
        Dcm_DspInternal_SvcFunc(ProtocolIndex, OpStatus);
      }
      else
      {
        /* Emit PROC_DONE event here as there is failure in validation  */
        (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, ProtocolIndex, DCM_HSM_DCM_EV_PROC_DONE);
      }
    }
#if(DCM_EXTSVC_ENABLED == STD_ON)
    else
    {
      /* For user configured services, Confirmation callout will be invoked */
      Dcm_Confirmation(curHsmInfo->msgContext.idContext, curHsmInfo->RxConnectionId, Status);

      /* if transmission of confirmation was successful and
       * if a positive response was transmitted for previous SessionControl request */
      if ((Status == DCM_RES_POS_OK) &&
          (curHsmInfo->msgContext.idContext == DCM_SID_SESSION_CONTROL) &&
          (curHsmInfo->negativeResponseCode == DCM_E_OK))
      {
        /* switch to requested session */
        Dcm_Dsl_DiagnosticSessionHandler_SwitchCurrentSession((Dcm_SesCtrlType)curHsmInfo->subServiceId);

        Dcm_Dsl_DiagnosticSessionHandler_ClearResetToDefaultSessionOccured(curHsmInfo->serviceId);
      }

      /* For external service handlers, PROC_DONE is emitted after the confirmation */
      (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, ProtocolIndex, DCM_HSM_DCM_EV_PROC_DONE);
    }
#endif /* #if(DCM_EXTSVC_ENABLED == STD_ON) */
  }

  DBG_DCM_DSPINTERNAL_DCMCONFIRMATION_EXIT(ProtocolIndex,Status);
}

#if (DCM_DSP_USE_SERVICE_0X31 == STD_ON)
/* This function moves an arbitrary number of bits present in a buffer (noOfBits), starting from
 * a given position (fromPosition) to a given position (toPosition).
 * This function is only used with the distance between the source position and destination position
 * being a maximum of 7 bits. It will not work in a different case.
 * If clearJunkBits is TRUE,
 * this function willbehave like a SHIFT function, i.e. all bits "traversed" by the
 * moved chunk of data will be cleared. */
FUNC(void, DCM_CODE) Dcm_DspInternal_ShiftBitsInBuffer(
  P2VAR(Dcm_MsgItemType, AUTOMATIC, DCM_VAR) buffer,
  Dcm_MsgLenType fromPosition,
  Dcm_MsgLenType toPosition,
  Dcm_MsgLenType noOfBits,
  boolean clearJunkBits)
{
  uint32 shiftCount;
  Dcm_MsgLenType InClearByte;
  uint8_least destBitPos,sourceBitPos;
  Dcm_MsgLenType destBytePos,sourceBytePos;
  DBG_DCM_DSPINTERNAL_SHIFTBITSINBUFFER_ENTRY(buffer, fromPosition, toPosition, noOfBits,
  clearJunkBits);
  /* Rightwise shift */
  if (fromPosition < toPosition)
  {
    sourceBytePos = ((Dcm_MsgLenType)(fromPosition + noOfBits) - 1U ) / DCM_BYTE_BITS;
    sourceBitPos  = (uint8)(((Dcm_MsgLenType)(fromPosition + noOfBits) - 1U ) % DCM_BYTE_BITS);
    destBytePos   = ((Dcm_MsgLenType)(toPosition + noOfBits) - 1U ) / DCM_BYTE_BITS;
    destBitPos    = (uint8)(((Dcm_MsgLenType)(toPosition + noOfBits) - 1U ) % DCM_BYTE_BITS);

    /* right shifting of buffer */
    for( shiftCount = 0U; shiftCount < noOfBits ; shiftCount++)
    {
      if (DCM_UINT8_IS_BIT_SET(buffer[sourceBytePos], (uint8)sourceBitPos))
      {
        DCM_UINT8_SET_BIT(buffer[destBytePos], (uint8)destBitPos);
      }
      else
      {
        DCM_UINT8_CLEAR_BIT(buffer[destBytePos], (uint8)destBitPos);
      }

      if(sourceBitPos == 0U)
      {
        sourceBytePos--;
        sourceBitPos = 7U ;
      }
      else
      {
        sourceBitPos--;
      }
      if(destBitPos == 0U)
      {
        destBytePos--;
        destBitPos= 7U;
      }
      else
      {
        destBitPos--;
      }
    }
  }
  /* leftwise shift */
  else
  {
    /* Initilisation of source and destination bit and byte positions for left shift */
    destBitPos = (uint8)(toPosition % DCM_BYTE_BITS);
    destBytePos = toPosition / DCM_BYTE_BITS;
    sourceBitPos = (uint8)(fromPosition % DCM_BYTE_BITS);
    sourceBytePos = fromPosition / DCM_BYTE_BITS;

    for( shiftCount = 0U; shiftCount < noOfBits ; shiftCount++)
    {
      if (DCM_UINT8_IS_BIT_SET(buffer[sourceBytePos], (uint8)sourceBitPos))
      {
        DCM_UINT8_SET_BIT(buffer[destBytePos], (uint8)destBitPos);
      }
      else
      {
        DCM_UINT8_CLEAR_BIT(buffer[destBytePos], (uint8)destBitPos);
      }

      if(sourceBitPos == 7U)
      {
        sourceBytePos++;
        sourceBitPos = 0U;
      }
      else
      {
        sourceBitPos++;
      }
      if(destBitPos == 7U)
      {
        destBytePos++;
        destBitPos = 0U;
      }
      else
      {
        destBitPos++;
      }
    }
  }

  /* Clearing Junk bits */
  if(clearJunkBits == TRUE)
  {
    /* CHECK: NOPARSE */
    /* clearing junk bits left in front of the moved chunk of data */
    if (fromPosition < toPosition)
    {
      /* This function is only used within the RoutineControl service and with the current
       * usage of the function, fromPosition is always greater than the toPosition */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);

      InClearByte = fromPosition / DCM_BYTE_BITS;
      buffer[InClearByte] =
        buffer[InClearByte] & (uint8)(~(uint8)(0xFFU << (fromPosition % DCM_BYTE_BITS)));
    }
    /* CHECK: PARSE */
    /* clearing junk bits left behind the moved chunk of data */
    else
    {
      if( destBitPos != 0U )
      {
        buffer[destBytePos] =
          buffer[destBytePos] & (uint8)(0xFFU >> (DCM_BYTE_BITS - (uint8)destBitPos));
      }
      /* CHECK: NOPARSE */
      else
      {
        DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      }
      /* CHECK: PARSE */
    }
  }
  DBG_DCM_DSPINTERNAL_SHIFTBITSINBUFFER_EXIT(buffer, fromPosition, toPosition, noOfBits,
  clearJunkBits);
}

#endif /* DCM_DSP_USE_SERVICE_0X31 == STD_ON  */

#if ((DCM_DSP_USE_SERVICE_0X2E == STD_ON) || (DCM_DSP_USE_SERVICE_0X22 == STD_ON) || \
 (DCM_DSP_USE_SERVICE_0X86 == STD_ON) || (DCM_DSP_USE_SERVICE_0X31 == STD_ON) || \
 (DCM_DSP_USE_SERVICE_0X2F == STD_ON) || (DCM_DSP_USE_SERVICE_0X2A == STD_ON) || \
 (DCM_DSP_USE_SERVICE_0X2C == STD_ON) || (DCM_DSP_USE_SERVICE_0X24 == STD_ON) || \
 (((DCM_DSP_USE_SERVICE_0X01 == STD_ON) || \
   (DCM_DSP_USE_SERVICE_0X02 == STD_ON)) && \
  (DCM_NUM_PID_CFG > 0U)))

/* The function for copying bits from one buffer to another */
FUNC(void, DCM_CODE) Dcm_DspInternal_CopySignalData(
  P2CONST(uint8, AUTOMATIC, DCM_VAR) SourceBuffer,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) DestBuffer,
  Dcm_MsgLenType SourceStartPosition,
  Dcm_MsgLenType DestStartPosition,
  Dcm_MsgLenType SizeToCopy,
  Dcm_DataCopyMethodType CopyMethod,
  Dcm_ByteAdvanceDirectionType ByteAdvanceDirection
  )
{
  uint8 SourceDataMask = 0x00U;
  uint8 DestByteSpace = 0x00U;
  Dcm_MsgLenType Size = SizeToCopy;

  /* Initialize required bits to extract into Source data mask */
  uint8 SourceMaskBitsReq = 0x00U;

  /* Initialize the byte and bit position of source buffer from which data needs to be copied */
  /* The first 3 bits(Bit position 0..2) indicates the bit position and the rest of the bits
   * indicate the byte index (Bit position 3..15) */
  uint8 SourceBitPos = (uint8)(SourceStartPosition & DCM_BIT_POSITION_MASK);
  Dcm_MsgLenType SourceBytePos = (Dcm_MsgLenType)(SourceStartPosition >> DCM_BYTE_POSITION_SHIFTS);

  /* Initialize the byte and bit position of destination buffer to which data needs to be copied */
  /* The first 3 bits(Bit position 0..2) indicates the bit position and the rest of the bits
   * indicate the byte index (Bit position 3..15) */
  uint8 DestBitPos = (uint8)(DestStartPosition & DCM_BIT_POSITION_MASK);
  Dcm_MsgLenType DestBytePos = (Dcm_MsgLenType)(DestStartPosition >> DCM_BYTE_POSITION_SHIFTS);

  DBG_DCM_DSPINTERNAL_COPYSIGNALDATA_ENTRY(
    SourceBuffer,DestBuffer,SourceStartPosition, DestStartPosition, Size, CopyMethod,ByteAdvanceDirection);

  /* Process the buffer until all data bits are copied */
  while(Size > 0U)
  {
    /* Extract bits required for the source data mask */
    if(Size < ((Dcm_MsgLenType)DCM_BYTE_BITS - DestBitPos))
    {
      SourceMaskBitsReq = (uint8)Size;
    }
    else
    {
      SourceMaskBitsReq = DCM_BYTE_BITS - DestBitPos;
    }

    /* Extract required bits from the Source Buffer and store in Data mask */
    SourceDataMask = SourceBuffer[SourceBytePos] >> SourceBitPos;
    if(SourceMaskBitsReq > (DCM_BYTE_BITS - SourceBitPos))
    {
      Dcm_MsgLenType ByteInsource = Dcm_DspInternal_DetermineNextByteInSourceBuffer(SourceBytePos, CopyMethod, ByteAdvanceDirection);

      SourceDataMask = SourceDataMask |
                      (uint8)(SourceBuffer[ByteInsource] << (DCM_BYTE_BITS - SourceBitPos));
    }

    /* Check the current destination bit position is the start of the current destination byte */
    if(DestBitPos == 0U)
    {
      /* Check whether the number of bits to transfer is more than byte size */
      if(Size >= DCM_BYTE_BITS)
      {
        /* Do one byte transfer */
        DestBuffer[DestBytePos] = SourceDataMask;
        /* Update the byte positions depending on the endianness and copy direction */
        SourceBytePos = Dcm_DspInternal_DetermineNextByteInSourceBuffer(SourceBytePos, CopyMethod, ByteAdvanceDirection);
        DestBytePos = Dcm_DspInternal_DetermineNextByteInDestBuffer(DestBytePos, CopyMethod, ByteAdvanceDirection);

        /* Decrement the number of bits to be copied by 8 */
        Size = Size - DCM_BYTE_BITS;
      }
      else
      {
        /* Clear the bits that has to transfer in destination byte */
        DestBuffer[DestBytePos] = (DestBuffer[DestBytePos] & (uint8)(DCM_MASK_BYTE << Size));
        /* Transfer the number of remaining bits to current Dest byte */
        DestBuffer[DestBytePos] = DestBuffer[DestBytePos] |
                                  (SourceDataMask & (DCM_MASK_BYTE >> (DCM_BYTE_BITS-Size)));
        break;
      }
    }
    /* If current Destination Bit Position is not start of current byte */
    else
    {
      /* Count space in current destination byte to data storage */
      DestByteSpace = DCM_BYTE_BITS - DestBitPos;
      if( Size <= DestByteSpace)
      {
        /* Clear the bits that has to transfer in destination buffer */
        DestBuffer[DestBytePos] = DestBuffer[DestBytePos] &
                                  (uint8)(~((uint8)((uint8)(DCM_MASK_BYTE
                                  << (DCM_BYTE_BITS - Size)) >> (DestByteSpace - Size))));
        /* Transfer the number of remaining bits to current DestBuffer */
        DestBuffer[DestBytePos] = DestBuffer[DestBytePos] |
                                  ((uint8)(SourceDataMask <<DestBitPos)
                                  & (DCM_MASK_BYTE >> (DestByteSpace - Size)));

        /* Deviation MISRAC2012-2 */
        /* Deviation MISRA-3 */
        break;
      }
      else
      {
        /* Clear the bits that has to transfer in destination buffer */
        DestBuffer[DestBytePos] = DestBuffer[DestBytePos] &
                                                  (uint8)(~(uint8)(DCM_MASK_BYTE << DestBitPos));
        /* Fill the current byte of DestBuffer using SourceDataMask*/
        DestBuffer[DestBytePos] = DestBuffer[DestBytePos] | (uint8)(SourceDataMask <<DestBitPos);

        /* Update the source Byte and Bit Positions according to endianness and copy direction */
        if(DestByteSpace > (DCM_BYTE_BITS - SourceBitPos))
        {
          SourceBytePos = Dcm_DspInternal_DetermineNextByteInSourceBuffer(SourceBytePos, CopyMethod, ByteAdvanceDirection);
          SourceBitPos = (SourceBitPos - DestBitPos) - 1U;
        }
        else if(DestByteSpace < (DCM_BYTE_BITS - SourceBitPos))
        {
          SourceBitPos = SourceBitPos + DestByteSpace;
        }
        else
        {
          SourceBytePos = Dcm_DspInternal_DetermineNextByteInSourceBuffer(SourceBytePos, CopyMethod, ByteAdvanceDirection);
          SourceBitPos = 0U;
        }

        /* Updated the Destination Byte and Bit Positions accordingly */
        DestBytePos = Dcm_DspInternal_DetermineNextByteInDestBuffer(DestBytePos, CopyMethod, ByteAdvanceDirection);
        DestBitPos = 0U;
        /* Decrement the Size by DestByteSpace */
        Size = Size - DestByteSpace;
      }
    }
  }

  DBG_DCM_DSPINTERNAL_COPYSIGNALDATA_EXIT(
    SourceBuffer, DestBuffer, SourceStartPosition, DestStartPosition, Size, CopyMethod,ByteAdvanceDirection);
}
#endif /*#if ((DCM_DSP_USE_SERVICE_0X2E == STD_ON) || (DCM_DSP_USE_SERVICE_0X22 == STD_ON) || \
     (DCM_DSP_USE_SERVICE_0X86 == STD_ON) || (DCM_DSP_USE_SERVICE_0X31 == STD_ON) || \
     (DCM_DSP_USE_SERVICE_0X2F == STD_ON) || (DCM_DSP_USE_SERVICE_0X2A == STD_ON) || \
     (DCM_DSP_USE_SERVICE_0X2C == STD_ON) || (DCM_DSP_USE_SERVICE_0X24 == STD_ON) || \
     (((DCM_DSP_USE_SERVICE_0X01 == STD_ON) || \
       (DCM_DSP_USE_SERVICE_0X02 == STD_ON)) && \
      (DCM_NUM_PID_CFG > 0U)))*/

/* From this paragraph below you can find implementation of OBD Service Handlers.
 * Please, if you put other stuff below this paragraph, mention that in a comment */
#if(DCM_DSP_USE_OBDDATASERVICES == STD_ON)
/* enable OBD services */

/* !LINKSTO Dcm.dsn.OBDSupportFunctions.0002,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_OBDSupport_UpdateAvailableInfoWithID(
  Dcm_GenericUint8IdType id,
  Dcm_GenericUint8IdType availabilityId,
  P2VAR(uint8, AUTOMATIC, DCM_VAR) availabilityData)
{
  Std_ReturnType retVal = DCM_E_NOT_OK;

  DBG_DCM_OBDSUPPORT_UPDATEAVAILABLEINFOWITHID_ENTRY(id, availabilityId, availabilityData);
  /* parameter check:
     id != 0x00, 0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0, 0xE0
     availabilityId == 0x00, 0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0, 0xE0
     availabilityData != NULL_PTR */
  if ((DCM_IS_AVAILABILITY_OBDSUBID(id) == TRUE) ||
      (DCM_IS_AVAILABILITY_OBDSUBID(availabilityId) == FALSE) ||
      (availabilityData == NULL_PTR))
  {
    retVal = DCM_E_PARAM;
  }
  else if (id > availabilityId)
  {
    /* Set the bit indicating that in some subsequent range(s) there is availability information.
     * The ECU shall not respond to unsupported PID ranges unless subsequent ranges have a
     * supported non-availability PID/MID/... */

    /* !LINKSTO Dcm.ISO-15031-5_8.1.2.2_1,1 */
    /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.PID,1 */
    if (id > (availabilityId+DCM_OBD_AVAILABILITY_RANGE))
    {
      availabilityData[3U] |= 0x01U; /* set bit 0 of byte D */
    }
    else
    {
      /* NOTE: the range is 0x01 to 0x1F not 0x00 to 0x20 */
      availabilityData[((id - 1U) % DCM_OBD_AVAILABILITY_RANGE) / 8U] |= (uint8)(0x80U >> ((id - 1U) % 8U));
    }
    retVal = DCM_E_OK;
  }
  else
  {
    /* Do Nothing */
  }
  DBG_DCM_OBDSUPPORT_UPDATEAVAILABLEINFOWITHID_EXIT(retVal, id, availabilityId, availabilityData);

  return retVal;
}

/* Deviation MISRAC2012-1 <+3>*/
/* Deviation MISRA-2 <+3> */
FUNC(Std_ReturnType, DCM_CODE) Dcm_OBDSupport_CheckRequestCombinedIds(
    P2VAR(uint8, AUTOMATIC, DCM_VAR)  reqData,
    Dcm_MsgLenType  reqDataLen)
{
  Std_ReturnType retVal = DCM_E_NOT_OK;
  boolean availabilityReq = FALSE;
  boolean nonAvailabilityReq = FALSE;
  uint16_least idIdx = 0U;

  DBG_DCM_OBDSUPPORT_CHECKREQUESTCOMBINEDIDS_ENTRY(reqData, reqDataLen);
  if (reqDataLen != 0U)
  {
    for (idIdx=0U; idIdx < reqDataLen; idIdx++)
    {
      /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.OBDMID,1 */
      /* !LINKSTO Dcm.EB.ReadDataById.DefaultInterface.Enabled.UnsupportedDid.InfoType,1 */
      if (DCM_IS_AVAILABILITY_OBDSUBID(reqData[idIdx]) == TRUE)
      {
        availabilityReq = TRUE;
      }
      else
      {
        nonAvailabilityReq = TRUE;
      }
    }

    /* !LINKSTO Dcm.dsn.OBDSupportFunctions.0001,1 */
    if ((availabilityReq != TRUE) || (nonAvailabilityReq != TRUE))
    {
      retVal = DCM_E_OK;
    }
  }
  DBG_DCM_OBDSUPPORT_CHECKREQUESTCOMBINEDIDS_EXIT(retVal, reqData, reqDataLen);

  return retVal;
}
#endif /* DCM_DSP_USE_OBDDATASERVICES */

#if(DCM_DSP_USE_SERVICE_0X03 == STD_ON)
/* enable OBD service 0x03 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_RequestConfirmedDTC_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U) &&\
     (DCM_POSTCONDITION_ASSERT_ENABLED == STD_ON))
   Std_ReturnType DcmFacadeLockStatus = DCM_E_RESOURCE_LOCKED;
#endif

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSPINTERNAL_REQUESTCONFIRMEDDTC_SVCH_ENTRY(OpStatus, pMsgContext);

  /* !LINKSTO SWS_Dcm_00245,1 */
  switch (OpStatus)
  {
    /* first call of service function */
    case DCM_INITIAL:
    {
      /* check the request data length */
      if (pMsgContext->reqDataLen != 0U)
      {
        /* !LINKSTO Dcm.EB.OBD.AllSvc.RequestInvalid,1 */
        serviceProcessStatus = DCM_E_INHIBIT;
      }
      else
      {
        P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
#if (DCM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
        /* !LINKSTO Dcm.EB.OBD.0x03Svc.DemFacade.Locking,1 */
        DcmFacadeLockStatus = Dcm_Dsp_DemFacade_LockClient(DemClientId) ;
#else
        (void)Dcm_Dsp_DemFacade_LockClient(DemClientId) ;
#endif /* #if (DCM_POSTCONDITION_ASSERT_ENABLED == STD_ON) */

        /* The above locking operation should never fail */
        DCM_POSTCONDITION_ASSERT(DcmFacadeLockStatus != DCM_E_RESOURCE_LOCKED, DCM_INTERNAL_API_ID);
#endif /* #if((DCM_HAS_ROE_TYPE2 == STD_ON)&&(DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U)) */

        /* Buffer/page handling
         * Note: One byte is consumed by the response SID */
        ClientContextPtr->SizeOfOnePage = pMsgContext->resMaxDataLen + 1U;
        ClientContextPtr->AmountOfDataWrittenOnCurrentPage = 1U;

        /* Since the OBD services are using common code from the Read DTC Information service
         * implementation, these contexts must also be correctly initialized */
        ClientContextPtr->ServiceID = DCM_DSP_OBD_SERVICE_0X03;

        /* This service has no sub-service */
        ClientContextPtr->SubServiceID = DCM_DSP_READDTCINFORMATION_UNSPECIFIC_ID;

        /* Requested/service-specific parameter */
        /* !LINKSTO Dcm.OBD.0x03Svc.SetDTCFilter,1 */
        ClientContextPtr->DTCFormat = DEM_DTC_FORMAT_OBD;
        ClientContextPtr->MemoryOrigin = DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY;
        ClientContextPtr->RequestedStatusMask = DCM_REPORT_CONFIRMEDDTC;
        ClientContextPtr->RequestedStatusBits = 0xFFU;
        ClientContextPtr->FilterForFaultDetectionCounter = FALSE;
        /* Initialize page buffering flag */
        ClientContextPtr->PageBufferingFlag = FALSE;
        ClientContextPtr->FirstPageSent = FALSE;
        /* Flow Control parameter
         * Data Supplier Switches */
        ClientContextPtr->MemoryOriginState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
        ClientContextPtr->AvailabilityMaskState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
        ClientContextPtr->DtcFormatIdState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
        /* !LINKSTO Dcm.EB.OBD.0x03Svc.ServiceSupportedWithDTCsStored,1 */
        /* !LINKSTO Dcm.EB.OBD.0x03Svc.ServiceSupportedWithoutDTCsStored,1 */
        ClientContextPtr->FilteredDtcsCountState = DCM_DATA_SUPPLIER_REQUIRED;
        ClientContextPtr->FilteredDtcsState = DCM_DATA_SUPPLIER_REQUIRED;

        /* Start the service handling */
        serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoByFilter_Process(DemClientId, OpStatus, pMsgContext);
      }
    }
    break;
    case DCM_PENDING:
    {
      /* Proceed the service handling */
      serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoByFilter_Process(DemClientId, OpStatus, pMsgContext);
    }
    break;

    /* action performed on failure of repeating pending */
    case DCM_CANCEL:
    {
      serviceProcessStatus = DCM_E_DONE;
    }
    break;

    /* actions performed on successful sending of the response */
    case DCM_CONFIRMED_OK:
    /* actions performed on failure in sending of the response */
    case DCM_CONFIRMED_NOK:
    {
      serviceProcessStatus = DCM_E_DONE;
    }
    break;

    /* CHECK: NOPARSE */
    default:
    {
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SVCID_GETFILTEREDDTC);
    }
    break;
    /* CHECK: PARSE */
  }

  if (serviceProcessStatus == DCM_E_INHIBIT)
  {
    /* !LINKSTO Dcm.EB.OBD.0x03Svc.FailedProcessing,1 */
    pMsgContext->msgAddInfo.suppressPosResponse = TRUE;
    serviceProcessStatus = DCM_E_DONE;
  }

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
  if (( (serviceProcessStatus == DCM_E_OK)
      ||(serviceProcessStatus == DCM_E_NOT_OK)
      ||(serviceProcessStatus == DCM_E_ABORT)
      ||(serviceProcessStatus == DCM_E_DONE)
     ) && (OpStatus != DCM_CONFIRMED_OK) && (OpStatus != DCM_CONFIRMED_NOK) &&
      (pMsgContext->reqDataLen == 0U))
  {
    /* Unlock DemFacade */
    Dcm_Dsp_DemFacade_UnlockClient(DemClientId);
  }
#endif

  DBG_DCM_DSPINTERNAL_REQUESTCONFIRMEDDTC_SVCH_EXIT(serviceProcessStatus, OpStatus, pMsgContext);

  return serviceProcessStatus;
}
#endif /* #if(DCM_DSP_USE_SERVICE_0X03 == STD_ON) */

#if(DCM_DSP_USE_SERVICE_0X04 == STD_ON)
/* !LINKSTO SWS_Dcm_00246,1 */
/* This is the generic service handler for OBD service 0x04 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ClearEmissionDiagInfo_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  Dcm_NegativeResponseCodeType Nrc = E_OK;
#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U) &&\
     (DCM_POSTCONDITION_ASSERT_ENABLED == STD_ON))
   Std_ReturnType DcmFacadeLockStatus = DCM_E_RESOURCE_LOCKED;
#endif

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));

  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSPINTERNAL_CLEAREMISSIONDIAGINFO_SVCH_ENTRY(OpStatus,pMsgContext);

  switch (OpStatus)
  {

    case DCM_INITIAL:
      /* Check the request data length */
      if (pMsgContext->reqDataLen != 0U)
      {
        /* Set suppress positive response information */
        /* !LINKSTO Dcm.EB.OBD.ClearEmissioRelatedDiagInfo.WrongMsgLength,1 */
        pMsgContext->msgAddInfo.suppressPosResponse = TRUE;
      }
      else
      {
#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
#if (DCM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
         /* !LINKSTO Dcm.EB.OBD.0x04Svc.DemFacade.Locking,2 */
         DcmFacadeLockStatus = Dcm_Dsp_DemFacade_LockClient(DemClientId);
#else
         (void)Dcm_Dsp_DemFacade_LockClient(DemClientId);
#endif /* #if (DCM_POSTCONDITION_ASSERT_ENABLED == STD_ON) */

         /* The above locking operation should never fail */
         DCM_POSTCONDITION_ASSERT(DcmFacadeLockStatus != DCM_E_RESOURCE_LOCKED, DCM_INTERNAL_API_ID);

#endif /* #if((DCM_HAS_ROE_TYPE2 == STD_ON)&&(DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U)) */
         /* !LINKSTO SWS_Dcm_00004,1 */
         serviceProcessStatus = Dcm_Dsp_DemFacade_ClearDTCInfo_Setup(DemClientId,
                                             DEM_DTC_GROUP_ALL_DTCS, DEM_DTC_FORMAT_OBD,
                                             DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY, &Nrc);
         if (E_OK == Nrc)
         {
           serviceProcessStatus = Dcm_Dsp_DemFacade_ClearDTCInfo_Process(DemClientId, &Nrc);
         }
       }
      break;
    case DCM_PENDING:
      serviceProcessStatus = Dcm_Dsp_DemFacade_ClearDTCInfo_Process(DemClientId, &Nrc);
      break;

    case DCM_CANCEL:
      /* !LINKSTO Dcm.EB.ClearResetEmissionRelatedDiagInfo.Canceling,1 */
      serviceProcessStatus = Dcm_Dsp_DemFacade_ClearDTCInfo_Cancel(DemClientId);
      break;

    /* CHECK: NOPARSE */
    default:
    {
      /* No need of any processing for all the remaining operation states */
      serviceProcessStatus = DCM_E_DONE;
      break;
    }
    /* CHECK: PARSE */
  }
  if ((E_OK != Nrc) && (serviceProcessStatus == DCM_E_DONE))
  {
    if (DCM_E_CONDITIONSNOTCORRECT == Nrc)
    {
      /* Set negative response code */
      Dcm_ExternalSetNegResponse(pMsgContext, Nrc);
    }
    else
    {
      /* No response will be send */
      pMsgContext->msgAddInfo.suppressPosResponse = TRUE;
    }
  }

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
  if (( (serviceProcessStatus == DCM_E_OK)
      ||(serviceProcessStatus == DCM_E_NOT_OK)
      ||(serviceProcessStatus == DCM_E_ABORT)
      ||(serviceProcessStatus == DCM_E_DONE)
     ) && (OpStatus != DCM_CONFIRMED_OK) && (OpStatus != DCM_CONFIRMED_NOK) &&
      (pMsgContext->reqDataLen == 0U))
  {
    /* Unlock DemFacade */
    Dcm_Dsp_DemFacade_UnlockClient(DemClientId);
  }
#endif

  DBG_DCM_DSPINTERNAL_CLEAREMISSIONDIAGINFO_SVCH_EXIT(serviceProcessStatus,OpStatus,pMsgContext);

  return serviceProcessStatus;
}

#endif /* (DCM_DSP_USE_SERVICE_0X04 == STD_ON) */

#if(DCM_DSP_USE_SERVICE_0X07 == STD_ON)
/* enable OBD service 0x07 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_RequestPendingDTC_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U) &&\
     (DCM_POSTCONDITION_ASSERT_ENABLED == STD_ON))
   Std_ReturnType DcmFacadeLockStatus = DCM_E_RESOURCE_LOCKED;
#endif

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSPINTERNAL_REQUESTPENDINGDTC_SVCH_ENTRY(OpStatus, pMsgContext);

  /* !LINKSTO SWS_Dcm_00410,1 */
  switch (OpStatus)
  {
    /* first call of service function */
    case DCM_INITIAL:
    {
      /* check the request data length */
      if (pMsgContext->reqDataLen != 0U)
      {
        /* !LINKSTO Dcm.EB.OBD.AllSvc.RequestInvalid,1 */
        serviceProcessStatus = DCM_E_INHIBIT;
      }
      else
      {
        P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
#if (DCM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
        /* !LINKSTO Dcm.EB.OBD.0x07Svc.DemFacade.Locking,1 */
        DcmFacadeLockStatus = Dcm_Dsp_DemFacade_LockClient(DemClientId) ;
#else
        (void)Dcm_Dsp_DemFacade_LockClient(DemClientId) ;
#endif /* #if (DCM_POSTCONDITION_ASSERT_ENABLED == STD_ON) */

        /* The above locking operation should never fail */
        DCM_POSTCONDITION_ASSERT(DcmFacadeLockStatus != DCM_E_RESOURCE_LOCKED, DCM_INTERNAL_API_ID);

#endif /* #if((DCM_HAS_ROE_TYPE2 == STD_ON)&&(DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U)) */

        /* Buffer/page handling
         * Note: One byte is consumed by the response SID */
        ClientContextPtr->SizeOfOnePage = pMsgContext->resMaxDataLen + 1U;
        ClientContextPtr->AmountOfDataWrittenOnCurrentPage = 1U;

        /* Since the OBD services are using common code from the Read DTC Information service
         * implementation, these contexts must also be correctly initialized */
        ClientContextPtr->ServiceID = DCM_DSP_OBD_SERVICE_0X07;

        /* This service has no sub-service */
        ClientContextPtr->SubServiceID = DCM_DSP_READDTCINFORMATION_UNSPECIFIC_ID;

        /* Requested/service-specific parameter */
        /* !LINKSTO Dcm.OBD.0x07Svc.SetDTCFilter,1 */
        ClientContextPtr->DTCFormat = DEM_DTC_FORMAT_OBD;
        ClientContextPtr->MemoryOrigin = DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY;
        ClientContextPtr->RequestedStatusMask = DCM_REPORT_PENDINGDTC;
        ClientContextPtr->RequestedStatusBits = 0xFFU;
        ClientContextPtr->FilterForFaultDetectionCounter = FALSE;
        /* Initialize page buffering flag */
        ClientContextPtr->PageBufferingFlag = FALSE;
        ClientContextPtr->FirstPageSent = FALSE;
        /* Flow Control parameter
         * Data Supplier Switches */
        ClientContextPtr->MemoryOriginState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
        ClientContextPtr->AvailabilityMaskState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
        ClientContextPtr->DtcFormatIdState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
        /* !LINKSTO Dcm.EB.OBD.0x07Svc.ServiceSupportedWithDTCsStored,1 */
        /* !LINKSTO Dcm.EB.OBD.0x07Svc.ServiceSupportedWithoutDTCsStored,1 */
        ClientContextPtr->FilteredDtcsCountState = DCM_DATA_SUPPLIER_REQUIRED;
        ClientContextPtr->FilteredDtcsState = DCM_DATA_SUPPLIER_REQUIRED;

        /* Start the service handling */
        serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoByFilter_Process(DemClientId, OpStatus, pMsgContext);
      }
    }
    break;
    case DCM_PENDING:
    {
      /* Start the service handling */
      serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoByFilter_Process(DemClientId, OpStatus, pMsgContext);
    }
    break;

    /* action preformed on failure of repeating pending */
    case DCM_CANCEL:
    {
      serviceProcessStatus = DCM_E_DONE;
    }
    break;

    /* actions performed on successful sending of the response */
    case DCM_CONFIRMED_OK:
    /* actions performed on failure in sending of the response */
    case DCM_CONFIRMED_NOK:
    {
      serviceProcessStatus = DCM_E_DONE;
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

  if (serviceProcessStatus == DCM_E_INHIBIT)
  {
    /* !LINKSTO Dcm.EB.OBD.0x07Svc.FailedProcessing,1 */
    pMsgContext->msgAddInfo.suppressPosResponse = TRUE;
    serviceProcessStatus = DCM_E_DONE;
  }

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
  if (( (serviceProcessStatus == DCM_E_OK)
      ||(serviceProcessStatus == DCM_E_NOT_OK)
      ||(serviceProcessStatus == DCM_E_ABORT)
      ||(serviceProcessStatus == DCM_E_DONE)
     ) && (OpStatus != DCM_CONFIRMED_OK) && (OpStatus != DCM_CONFIRMED_NOK) &&
      (pMsgContext->reqDataLen == 0U))
  {
    /* Unlock DemFacade */
    Dcm_Dsp_DemFacade_UnlockClient(DemClientId);
  }
#endif

  DBG_DCM_DSPINTERNAL_REQUESTPENDINGDTC_SVCH_EXIT(serviceProcessStatus, OpStatus, pMsgContext);

  return serviceProcessStatus;
}
#endif /* #if(DCM_DSP_USE_SERVICE_0X07 == STD_ON) */

#if(DCM_DSP_USE_SERVICE_0X0A == STD_ON)
/* enable OBD service 0x0A */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_RequestPermanentDTC_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U) &&\
     (DCM_POSTCONDITION_ASSERT_ENABLED == STD_ON))
   Std_ReturnType DcmFacadeLockStatus = DCM_E_RESOURCE_LOCKED;
#endif

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSPINTERNAL_REQUESTPERMANENTDTC_SVCH_ENTRY(OpStatus, pMsgContext);

  /* !LINKSTO SWS_Dcm_00411,1 */
  switch (OpStatus)
  {
    /* first call of service function */
    case DCM_INITIAL:
    {
      /* check the request data length */
      if (pMsgContext->reqDataLen != 0U)
      {
        /* !LINKSTO Dcm.EB.OBD.AllSvc.RequestInvalid,1 */
        serviceProcessStatus = DCM_E_INHIBIT;
      }
      else
      {
        P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
#if (DCM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
        /* !LINKSTO Dcm.EB.OBD.0x0ASvc.DemFacade.Locking,1 */
        DcmFacadeLockStatus = Dcm_Dsp_DemFacade_LockClient(DemClientId) ;
#else
        (void)Dcm_Dsp_DemFacade_LockClient(DemClientId) ;
#endif /* #if (DCM_POSTCONDITION_ASSERT_ENABLED == STD_ON) */

        /* The above locking operation should never fail */
        DCM_POSTCONDITION_ASSERT(DcmFacadeLockStatus != DCM_E_RESOURCE_LOCKED, DCM_INTERNAL_API_ID);

#endif /* #if((DCM_HAS_ROE_TYPE2 == STD_ON)&&(DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U)) */

        /* Buffer/page handling
         * Note: One byte is consumed by the response SID */
        ClientContextPtr->SizeOfOnePage = pMsgContext->resMaxDataLen + 1U;
        ClientContextPtr->AmountOfDataWrittenOnCurrentPage = 1U;

        /* Since the OBD services are using common code from the Read DTC Information service
         * implementation, these contexts must also be correctly initialized */
        ClientContextPtr->ServiceID = DCM_DSP_OBD_SERVICE_0X0A;

        /* This service has no sub-service */
        ClientContextPtr->SubServiceID = DCM_DSP_READDTCINFORMATION_UNSPECIFIC_ID;

        /* Requested/service-specific parameter */
        /* !LINKSTO Dcm.OBD.0x0ASvc.SetDTCFilter,1 */
        ClientContextPtr->DTCFormat = DEM_DTC_FORMAT_OBD;
        ClientContextPtr->MemoryOrigin = DEM_DTC_ORIGIN_PERMANENT_MEMORY;
        ClientContextPtr->RequestedStatusMask = DCM_REPORT_ALLSUPPORTEDDTC;
        ClientContextPtr->RequestedStatusBits = 0xFFU;
        ClientContextPtr->FilterForFaultDetectionCounter = FALSE;
        /* Initialize page buffering flag */
        ClientContextPtr->PageBufferingFlag = FALSE;
        ClientContextPtr->FirstPageSent = FALSE;
        /* Flow Control parameter
         * Data Supplier Switches */
        ClientContextPtr->MemoryOriginState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
        ClientContextPtr->AvailabilityMaskState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
        ClientContextPtr->DtcFormatIdState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
        /* !LINKSTO Dcm.EB.OBD.0x0ASvc.ServiceSupportedWithDTCsStored,1 */
        /* !LINKSTO Dcm.EB.OBD.0x0ASvc.ServiceSupportedWithoutDTCsStored,1 */
        ClientContextPtr->FilteredDtcsCountState = DCM_DATA_SUPPLIER_REQUIRED;
        ClientContextPtr->FilteredDtcsState = DCM_DATA_SUPPLIER_REQUIRED;

        /* Start the service handling */
        serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoByFilter_Process(DemClientId, OpStatus, pMsgContext);
      }
    }
    break;
    case DCM_PENDING:
    {
      /* Start the service handling */
      serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoByFilter_Process(DemClientId, OpStatus, pMsgContext);
    }
    break;

    /* action preformed on failure of repeating pending */
    case DCM_CANCEL:
    {
      serviceProcessStatus = DCM_E_DONE;
    }
    break;

    /* actions performed on successful sending of the response */
    case DCM_CONFIRMED_OK:
    /* actions performed on failure in sending of the response */
    case DCM_CONFIRMED_NOK:
    {
      serviceProcessStatus = DCM_E_DONE;
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

  if (serviceProcessStatus == DCM_E_INHIBIT)
  {
    /* !LINKSTO Dcm.EB.OBD.0x0ASvc.FailedProcessing,1 */
    pMsgContext->msgAddInfo.suppressPosResponse = TRUE;
    serviceProcessStatus = DCM_E_DONE;
  }

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
  if (( (serviceProcessStatus == DCM_E_OK)
      ||(serviceProcessStatus == DCM_E_NOT_OK)
      ||(serviceProcessStatus == DCM_E_ABORT)
      ||(serviceProcessStatus == DCM_E_DONE)
     ) && (OpStatus != DCM_CONFIRMED_OK) && (OpStatus != DCM_CONFIRMED_NOK) &&
      (pMsgContext->reqDataLen == 0U))
  {
    /* Unlock DemFacade */
    Dcm_Dsp_DemFacade_UnlockClient(DemClientId);
  }
#endif

  DBG_DCM_DSPINTERNAL_REQUESTPERMANENTDTC_SVCH_EXIT(serviceProcessStatus, OpStatus, pMsgContext);

  return serviceProcessStatus;
}

#endif /* #if(DCM_DSP_USE_SERVICE_0X0A == STD_ON) */

FUNC(void, DCM_CODE) Dcm_Dsp_ProtocolTransmissionTimedOutWaitingForData(
  Dcm_BufferUserType ProtocolId,
  Dcm_BufferUserType TxConnectionId
)
{
  /* For now, a Protocol may have only one active transmission
     session at a time*/
  TS_PARAM_UNUSED(TxConnectionId);
  TS_PARAM_UNUSED(ProtocolId);

  DBG_DCM_DSP_PROTOCOLTRANSMISSIONTIMEDOUTWAITINGFORDATA_ENTRY(ProtocolId, TxConnectionId);
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
  Dcm_DspInternal_CancelPagedBufferProcessing();
#endif /* #if (DCM_PAGEDBUFFER_ENABLED == STD_ON) */
  DBG_DCM_DSP_PROTOCOLTRANSMISSIONTIMEDOUTWAITINGFORDATA_EXIT(ProtocolId, TxConnectionId);
}

FUNC (void, DCM_CODE) Dcm_Dsp_ProtocolProvideNextTxBuffer(
  Dcm_BufferUserType ProtocolId,
  Dcm_BufferUserType TxConnectionId
)
{
  Dcm_BufferStateType NewBufferState = DCM_BUFFER_IDLE;

#if (DCM_NUM_PROTOCOL > 1U)
  const Dcm_ProtocolIndexType ProtocolIndex = DCM_PROTOCOL_INDEX_FROM_ID(ProtocolId);
#endif /* #if (DCM_NUM_PROTOCOL > 1U) */

  DBG_DCM_DSP_PROTOCOLPROVIDENEXTTXBUFFER_ENTRY(ProtocolId, TxConnectionId);
  /* For now, a Protocol may have only one active transmission
     session at a time*/
  TS_PARAM_UNUSED(TxConnectionId);

      DCM_PRECONDITION_ASSERT(
  (DCM_IS_NORMAL_REQUEST(DCM_HSM_INFO(ProtocolIndex).RequestType)),
      DCM_INTERNAL_API_ID);

  NewBufferState = DCM_BUFFER_PROCESSING_NORMAL;

  /* The state of the TxBuffer is changed to reflect a processing in progress. */
  (void)Dcm_Dsl_BufferChangeState(ProtocolId,
                                 NewBufferState,
                                 DCM_HSM_INFO(ProtocolIndex).TxBuffer.BufferId);

  DCM_PRECONDITION_ASSERT(
    (DCM_HSM_INFO(ProtocolIndex).sentLength <
    (DCM_HSM_INFO(ProtocolIndex).msgContext.resDataLen + 1U)), DCM_INTERNAL_API_ID);

  /* A new buffer can be provided by the protocol as not all the data it was intending to send has
     been passed to the TxConnection. */
  (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm,
                                DCM_INST(ProtocolIndex),
                                DCM_HSM_DCM_EV_PROVIDE_TX_BUF);

  DBG_DCM_DSP_PROTOCOLPROVIDENEXTTXBUFFER_EXIT(ProtocolId, TxConnectionId);
}

FUNC (void, DCM_CODE) Dcm_Dsp_ProtocolTransmissionFinished(
  Dcm_BufferUserType ProtocolId,
  Dcm_TxResultType TransmissionResult,
  Dcm_BufferUserType TxConnectionId
)
{
#if (DCM_NUM_PROTOCOL > 1U)
  const Dcm_ProtocolIndexType ProtocolIndex = DCM_PROTOCOL_INDEX_FROM_ID(ProtocolId);
#endif /* #if (DCM_NUM_PROTOCOL == 1U) */

  DBG_DCM_DSP_PROTOCOLTRANSMISSIONFINISHED_ENTRY(ProtocolId,TransmissionResult,TxConnectionId);

  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  if (DCM_INVALID_REQUEST_TYPE != DCM_HSM_INFO(ProtocolIndex).NextJob.JobRequestType)
  {
    Dcm_Dsp_ProtocolTransmissionFinishedBeforeHSMStarted(ProtocolId ,TransmissionResult,
                                                         TxConnectionId);
  }
  else
  {
    Dcm_Dsp_ProtocolTransmissionFinishedAfterHSMStarted(ProtocolId ,TransmissionResult,
                                                        TxConnectionId);
  }
  /* EXIT critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  DBG_DCM_DSP_PROTOCOLTRANSMISSIONFINISHED_EXIT(ProtocolId, TransmissionResult,TxConnectionId);
}

FUNC(void, DCM_CODE) Dcm_Dsp_ProtocolStart(
  Dcm_BufferUserType ProtocolId,
  Dcm_BufferUserType RxConnectionId,
  Dcm_BufferUserType TxConnectionId,
  Dcm_BufferType RequestBuffer,
  Dcm_RequestTypeType RequestType,
  Dcm_ProtocolRCRRPTransmissionType ProtocolRCRRPTransmission,
  uint16 rpCntr,
  uint16 rpCntrConfirmed
  )
{
  /* Set up the relevant protocol's state machine */
  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo =
    &DCM_HSM_INFO(DCM_PROTOCOL_INDEX_FROM_ID(ProtocolId));
  Dcm_RxPduIdInfoType RxPduIdInfo;

  Dcm_HsmEventType EventToDispatchToProtocolHSM = DCM_HSM_DCM_EV_INVALID;

  DBG_DCM_DSP_PROTOCOLSTART_ENTRY(ProtocolId, RxConnectionId, TxConnectionId,
    RequestBuffer, RequestType);

#if (DCM_NUM_PROTOCOL == 1U)
  TS_PARAM_UNUSED(ProtocolId);
#endif

  /* Update current RxPduId as well as corresponding TxPduId in HSM information */

  /* Periodic requests shall have, as RxPduId, the RxPduId of the request that started them. */
  curHsmInfo->NextJob.JobRxConnectionId = RxConnectionId;

  /* Get the channel id */
  Dcm_Dsl_RxConnectionGetRxPduIdInformation(RxConnectionId, &RxPduIdInfo);

  /* The protocol shall need to know the ComM ChannelId in the context of which the transmission
     is being executed */
  curHsmInfo->NextJob.JobComMChannelId = RxPduIdInfo.ComMChannelId;

  /* The TxPduId/ConfirmationTxPduId pair was determined previously from the type of transmission
     which was requested. */
  curHsmInfo->NextJob.JobTxConnectionId = TxConnectionId;

  curHsmInfo->NextJob.JobTesterSourceAddress =
    Dcm_MainConnectionConfig[Dcm_RxPduIdConfig[RxConnectionId].MainConnectionIndex]
    .TesterSourceAddress;

  /* Ownership of the buffer has already been passed to the relevant protocol, therefore the
     protocol receives the "package" containing the data of the buffer to process. */
  curHsmInfo->NextJob.JobRxBuffer.BufferId = RequestBuffer.BufferId;
  curHsmInfo->NextJob.JobRxBuffer.FilledLength = RequestBuffer.FilledLength;
  curHsmInfo->NextJob.JobRxBuffer.Size = RequestBuffer.Size;
  curHsmInfo->NextJob.JobRxBuffer.BufferPtr = RequestBuffer.BufferPtr;

  curHsmInfo->NextJob.JobRequestType = RequestType;

  switch(RequestType)
  {
    case DCM_NORMAL_PHYSICAL_REQUEST:
    case DCM_NORMAL_FUNCTIONAL_REQUEST:

      if (ProtocolRCRRPTransmission == DCM_PROTOCOL_NO_RCRRP_ON_ENTRY)
      {
        /* Trigger the DCM_HSM_DCM_EV_REQUEST_RECEIVED event in the relevant state machine */
        EventToDispatchToProtocolHSM = DCM_HSM_DCM_EV_REQUEST_RECEIVED;
      }
      else
      {
        /* Trigger the DCM_HSM_DCM_EV_REQUEST_RECEIVED_RCRRP event in the relevant state machine */
        EventToDispatchToProtocolHSM = DCM_HSM_DCM_EV_REQUEST_RECEIVED_RCRRP;

        if (ProtocolRCRRPTransmission == DCM_PROTOCOL_TRANSMITTING_RCRRP_ON_ENTRY)
        {
          /* From now on confirmation will be reported to protocol */
          (void)Dcm_Dsl_TxConnectionGive(DCM_SUPERVISOR_ID, ProtocolId, TxConnectionId);
        }
      }

      /* For Protocols processing external requests, stop the S3Timer if it's running. It will
         be restarted at the end of processing should the session be different from the
         default session. */


    break;

    /* At first glance this might seem awkward, but there is a reason for differentiating
       between functional and physical ROE, even though it's highly unlikely that we will
       ever get ROE configured on a functional RxPduId */

#if (DCM_DSP_USE_ROESERVICES == STD_ON)
    case DCM_ROE_TYPE1_PHYSICAL_REQUEST:
    case DCM_ROE_TYPE1_FUNCTIONAL_REQUEST:
    case DCM_ROE_TYPE2_PHYSICAL_REQUEST:
    case DCM_ROE_TYPE2_FUNCTIONAL_REQUEST:

      /* Trigger the DCM_HSM_DCM_EV_ROE event in the relevant state machine */
      EventToDispatchToProtocolHSM = DCM_HSM_DCM_EV_ROE;
    break;
#endif /* #if (STD_ON == DCM_DSP_USE_ROESERVICES) */

#if (DCM_PERIODIC_USED == STD_ON)
    case DCM_PERIODIC_TYPE1_REQUEST:
      EventToDispatchToProtocolHSM = DCM_HSM_DCM_EV_RDBPI_TYPE1;
    break;

    case DCM_PERIODIC_TYPE2_REQUEST:
      EventToDispatchToProtocolHSM = DCM_HSM_DCM_EV_RDBPI_TYPE2;
      break;
#endif /* #if (STD_ON == DCM_PERIODIC_USED) */

    case DCM_JUMP_FROM_BL_RESPONSE_REQUEST:
    case DCM_JUMP_FROM_BL_NO_RESPONSE_REQUEST:

      /* Emit event so that the Dcm shall call ComM_DCM_ActiveDiagnostic() to request the
         ComManager for full communication mode */
      EventToDispatchToProtocolHSM = DCM_HSM_DCM_EV_RETURN_FROM_BL;

      break;

    /* CHECK: NOPARSE */
    default:
      /* Unreachable default case mandated by MISRA */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE */
  }

  curHsmInfo->NextJob.JobRpCntr = rpCntr;
  curHsmInfo->NextJob.JobRpCntrConfirmed = rpCntrConfirmed;

  if (DCM_HSM_DCM_EV_REQUEST_RECEIVED_RCRRP == EventToDispatchToProtocolHSM)
  {
    curHsmInfo->NextJob.JobResponsePendingTxStatus = DCM_RP_PENDING_CONF;
  }
  else
  {
    curHsmInfo->NextJob.JobResponsePendingTxStatus = DCM_RP_NO_DATA;
  }

  /* Dispatch the job to the intended Protocol HSM. */
  (void)DCM_HSMEMITINST_WRAPPER(
    &Dcm_HsmScDcm,
    DCM_CUR_INST_ID(DCM_PROTOCOL_INDEX_FROM_ID(ProtocolId)),
    EventToDispatchToProtocolHSM);

  DBG_DCM_DSP_PROTOCOLSTART_EXIT(ProtocolId, RxConnectionId, TxConnectionId,
    RequestBuffer, RequestType);
}

FUNC (void, DCM_CODE) Dcm_Dsp_ProtocolCancel(
  Dcm_BufferUserType ProtocolId)
{
#if (DCM_NUM_PROTOCOL > 1U)
  const Dcm_ProtocolIndexType ProtocolIndex = DCM_PROTOCOL_INDEX_FROM_ID(ProtocolId);
#else /* #if (DCM_NUM_PROTOCOL > 1U) */
  TS_PARAM_UNUSED (ProtocolId);
#endif /* #if (DCM_NUM_PROTOCOL > 1U) */

  DBG_DCM_DSP_PROTOCOLCANCEL_ENTRY(ProtocolId);

  (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(ProtocolIndex),
                                 DCM_HSM_DCM_EV_CANCEL_PROCESSING);
  DBG_DCM_DSP_PROTOCOLCANCEL_EXIT(ProtocolId);
}

FUNC (void, DCM_CODE) Dcm_Dsp_ProtocolSetUpCancellation(
  Dcm_CancellationType CancellationType,
  Dcm_ProtocolIndexType ProtocolIndex)
{
#if (DCM_NUM_PROTOCOL == 1U)
  TS_PARAM_UNUSED (ProtocolIndex);
#endif /* #if (DCM_NUM_PROTOCOL == 1U) */
  DBG_DCM_DSP_PROTOCOLSETUPCANCELLATION_ENTRY(CancellationType, ProtocolIndex);

  DCM_HSM_INFO(ProtocolIndex).CancellationConditions.CancellationType = CancellationType;

  DBG_DCM_DSP_PROTOCOLSETUPCANCELLATION_EXIT(CancellationType, ProtocolIndex);
}

FUNC (void, DCM_CODE) Dcm_Dsp_CancellationDone(
  Dcm_CancellationConfirmationType CancellationConfirmation,
  Dcm_ProtocolIndexType ProtocolIndex
)
{
  boolean CancellationFinished = FALSE;

  DBG_DCM_DSP_CANCELLATIONDONE_ENTRY(CancellationConfirmation, ProtocolIndex);

#if (DCM_NUM_PROTOCOL == 1U)
  TS_PARAM_UNUSED (ProtocolIndex);
#endif /* #if (DCM_NUM_PROTOCOL == 1U) */

  if (DCM_SERVICE_CANCELLATION == CancellationConfirmation)
  {
    DCM_HSM_INFO(ProtocolIndex).CancellationConditions.ServiceProcessingDone = TRUE;

    if ((DCM_CANCEL_ROE_PROCESSING ==
           DCM_HSM_INFO(ProtocolIndex).CancellationConditions.CancellationType) ||
        (DCM_CANCEL_NORMAL_PROCESSING_NO_TX ==
           DCM_HSM_INFO(ProtocolIndex).CancellationConditions.CancellationType) ||
        (DCM_CANCEL_NORMAL_PROCESSING_TX ==
           DCM_HSM_INFO(ProtocolIndex).CancellationConditions.CancellationType)
       )
    {
      /* In these cancellation cases, the service handler processing is now finished.
         The service handler will not be called again in this processing session and can
         be freed so that another protocol may use it. In the case of cancelling a Page-Buffered
         processing, the service processor shall be called after cancellation one more
         time in order to confirm the failed transmission. */
      Dcm_Dsp_ServiceProcessorReset(ProtocolIndex);
    }
  }
  else
  {
    DCM_HSM_INFO(ProtocolIndex).CancellationConditions.FinalTransmissionDone = TRUE;
  }

  switch (DCM_HSM_INFO(ProtocolIndex).CancellationConditions.CancellationType)
  {
    case DCM_CANCEL_NORMAL_SENDING:
      DCM_PRECONDITION_ASSERT(
      (TRUE == DCM_HSM_INFO(ProtocolIndex).CancellationConditions.FinalTransmissionDone),
      DCM_INTERNAL_API_ID);

      CancellationFinished = TRUE;
      break;

    case DCM_CANCEL_ROE_PROCESSING:
    case DCM_CANCEL_NORMAL_PROCESSING_NO_TX:
      DCM_PRECONDITION_ASSERT(
      (TRUE == DCM_HSM_INFO(ProtocolIndex).CancellationConditions.ServiceProcessingDone),
      DCM_INTERNAL_API_ID);

      CancellationFinished = TRUE;
      break;

    case DCM_CANCEL_PG_BUF_SENDING:
    case DCM_CANCEL_NORMAL_PROCESSING_TX:
      if ((TRUE == DCM_HSM_INFO(ProtocolIndex).CancellationConditions.ServiceProcessingDone) &&
          (TRUE == DCM_HSM_INFO(ProtocolIndex).CancellationConditions.FinalTransmissionDone))
      {
        CancellationFinished = TRUE;
      }
      break;

    /* CHECK: NOPARSE*/
    default:
      /* Unreachable default case mandated by MISRA. */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_INTERNAL_API_ID);
      break;
    /* CHECK: PARSE*/
  }

  if (TRUE == CancellationFinished)
  {
    /* Release rx and tx buffers */
    Dcm_Dsl_BufferFreeRxTxNRC(DCM_PROTOCOL_ID_FROM_INDEX(DCM_CUR_INST_ID(ProtocolIndex)),
                              DCM_HSM_INFO(DCM_CUR_INST_ID(ProtocolIndex)).RxBuffer.BufferId,
                              DCM_HSM_INFO(DCM_CUR_INST_ID(ProtocolIndex)).TxBuffer.BufferId,
                              DCM_HSM_INFO(DCM_CUR_INST_ID(ProtocolIndex)).NRCBuffer.BufferId);

    Dcm_Dsl_Supervisor_ProtocolFree(
      DCM_PROTOCOL_ID_FROM_INDEX(DCM_CUR_INST_ID(ProtocolIndex)));

    (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, DCM_CUR_INST_ID(ProtocolIndex),
                                  DCM_HSM_DCM_EV_CANCELLATION_DONE);
  }
  DBG_DCM_DSP_CANCELLATIONDONE_EXIT(CancellationConfirmation, ProtocolIndex);
}

/* Deviation MISRAC2012-4 */
FUNC_P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_CODE) Dcm_Dsp_GetCurrentHsmInfo(void)
{
  P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) CurrentHsmInfoPtr;
  const Dcm_ProtocolIndexType ProtocolIdx = DCM_PROTOCOL_INDEX_FROM_ID(Dcm_Dsl_Supervisor_GetLatestExternalUDSProtocol());


  if (DCM_INVALID_PROTOCOL_INDEX == ProtocolIdx)
  {
    /* functions calling this function do not expect to receive a NULL pointer. This can only
       happen when no protocol has actually been started yet. Basically we return a pointer to
       the first protocol (since it will be in a defaulted state anyway). */
    CurrentHsmInfoPtr = &DCM_HSM_INFO(0U);
  }
  else
  {
    CurrentHsmInfoPtr = &DCM_HSM_INFO(ProtocolIdx);
  }

  return CurrentHsmInfoPtr;
}

STATIC FUNC (void, DCM_CODE) Dcm_Dsp_ServiceProcessorContextInit(
    Dcm_ServiceProcessingContext_PtrType ServiceProcessingContextPtr)
{
  ServiceProcessingContextPtr->ServiceFunction = NULL_PTR;
  ServiceProcessingContextPtr->curMsgContext = NULL_PTR;
  ServiceProcessingContextPtr->OpStatus = DCM_OPSTATUS_INVALID;
  ServiceProcessingContextPtr->RetVal = DCM_E_NOT_OK;
  ServiceProcessingContextPtr->ServiceHandlerType = DCM_INTERNAL_SERVICE_HANDLER;
  ServiceProcessingContextPtr->DcmInstanceIndex = 0U;
  ServiceProcessingContextPtr->ServiceProcessingInProgress = FALSE;
#if (DCM_ASYNC_SERVICE_HANDLER == STD_ON)
  if (ServiceProcessingContextPtr->ServiceExecutionType == DCM_ASYNC_EXECUTION)
  {
    Dcm_AsynchronousProtocol = DCM_INVALID_ASYNCHRONOUS;
  }

  ServiceProcessingContextPtr->ServiceExecutionType = DCM_SYNC_EXECUTION;
  ServiceProcessingContextPtr->AsynchronousProcessingState = DCM_ASYNC_PROCESSING_IDLE;
#endif /* #if (DCM_ASYNC_SERVICE_HANDLER == STD_ON) */
}

#if ((DCM_DSP_USE_SERVICE_0X2E == STD_ON) || (DCM_DSP_USE_SERVICE_0X22 == STD_ON) || \
 (DCM_DSP_USE_SERVICE_0X86 == STD_ON) || (DCM_DSP_USE_SERVICE_0X31 == STD_ON) || \
 (DCM_DSP_USE_SERVICE_0X2F == STD_ON) || (DCM_DSP_USE_SERVICE_0X2A == STD_ON) || \
 (DCM_DSP_USE_SERVICE_0X2C == STD_ON) || (DCM_DSP_USE_SERVICE_0X24 == STD_ON) || \
 (((DCM_DSP_USE_SERVICE_0X01 == STD_ON) || \
   (DCM_DSP_USE_SERVICE_0X02 == STD_ON)) && \
  (DCM_NUM_PID_CFG > 0U)))

STATIC FUNC(Dcm_MsgLenType, DCM_CODE) Dcm_DspInternal_DetermineNextByteInSourceBuffer(
  Dcm_MsgLenType CurrentByteInSourceBuffer,
  Dcm_DataCopyMethodType CopyMethod,
  Dcm_ByteAdvanceDirectionType ByteAdvanceDirection)
{
  Dcm_MsgLenType ByteInSource = 0U;

  switch(CopyMethod)
  {
    case DCM_COPY_FROM_INBUFFER_BIGENDIAN:
    {
      /* If copying from a buffer big endian, go to the previous byte in source */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.NextSourceByte.BigEndian.USE_DATA_SENDER_RECEIVER,1 */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.NextSourceByte.BigEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
      ByteInSource = CurrentByteInSourceBuffer - 1U;
      break;
    }
    case DCM_COPY_FROM_INBUFFER_LITTLEENDIAN:
    case DCM_COPY_FROM_INBUFFER_OPAQUE:
    {
      /* If copying from a buffer little or opaque endian, go to the next byte in source */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.NextSourceByte.LittleEndian.USE_DATA_SENDER_RECEIVER,1 */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.NextSourceByte.LittleEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.NextSourceByte.Opaque.USE_DATA_SENDER_RECEIVER,1 */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.NextSourceByte.Opaque.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
      ByteInSource = CurrentByteInSourceBuffer + 1U;
      break;
    }
    case DCM_COPY_TO_OUTBUFFER_OPAQUE:
    {
      /* If copying from a variable opaque endian:
       * - if on little endian CPU the copy starts from least significant byte and goes in the order of significance (byte ++)
       * - if on big endian CPU the copy starts from most significant byte and goes opposite to the order of significance (meaning also byte ++)
       *   (to have the same order of bytes in the variable and in the buffer)
       * */
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.NextSourceByte.OpaqueOnLittleEndianCPU.USE_DATA_SENDER_RECEIVER,1 */
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.NextSourceByte.OpaqueOnBigEndianCPU.USE_DATA_SENDER_RECEIVER,1 */
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.NextSourceByte.OpaqueOnLittleEndianCPU.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.NextSourceByte.OpaqueOnBigEndianCPU.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
      ByteInSource = CurrentByteInSourceBuffer + 1U;
      break;
    }
    case DCM_COPY_TO_OUTBUFFER_LITTLEENDIAN:
    case DCM_COPY_TO_OUTBUFFER_BIGENDIAN:
    {
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.NextSourceByte.LittleEndian.USE_DATA_SENDER_RECEIVER,1 */
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.NextSourceByte.BigEndian.USE_DATA_SENDER_RECEIVER,1 */
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.NextSourceByte.LittleEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.NextSourceByte.BigEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
      /* If copying from a variable (little or big endian) keep count depending on the cpu byte order */
      if (ByteAdvanceDirection == DCM_BYTE_DIRECTION_INCREMENT)
      {
        ByteInSource = CurrentByteInSourceBuffer + 1U;
      }
      else
      {

        ByteInSource = CurrentByteInSourceBuffer - 1U;
      }
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

  return ByteInSource;
}

STATIC FUNC(Dcm_MsgLenType, DCM_CODE) Dcm_DspInternal_DetermineNextByteInDestBuffer(
  Dcm_MsgLenType CurrentByteInDestBuffer,
  Dcm_DataCopyMethodType CopyMethod,
  Dcm_ByteAdvanceDirectionType ByteAdvanceDirection)
{
  Dcm_MsgLenType ByteInDest = 0U;

  switch(CopyMethod)
  {
    case DCM_COPY_FROM_INBUFFER_LITTLEENDIAN:
    case DCM_COPY_FROM_INBUFFER_BIGENDIAN:
    {
      /* If copying little or big endian to a variable keep count depending on the cpu byte order */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.NextDestByte.LittleEndian.USE_DATA_SENDER_RECEIVER,1 */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.NextDestByte.LittleEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.NextDestByte.BigEndian.USE_DATA_SENDER_RECEIVER,1 */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.NextDestByte.BigEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
      if (ByteAdvanceDirection == DCM_BYTE_DIRECTION_INCREMENT)
      {
        ByteInDest = CurrentByteInDestBuffer + 1U;
      }
      else
      {

        ByteInDest = CurrentByteInDestBuffer - 1U;
      }
      break;
    }
    case DCM_COPY_FROM_INBUFFER_OPAQUE:
    {
      /* If copying to a variable opaque endian:
       * - if on little endian CPU the copy starts from least significant byte and goes in the order of significance (byte ++)
       * - if on big endian CPU the copy starts from most significant byte and goes opposite to the order of significance (meaning also byte ++)
       *   (to have the same order of bytes in the variable and in the buffer)
       * */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.NextDestByte.OpaqueOnLittleEndianCPU.USE_DATA_SENDER_RECEIVER,1 */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.NextDestByte.OpaqueOnLittleEndianCPU.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.NextDestByte.OpaqueOnBigEndianCPU.USE_DATA_SENDER_RECEIVER,1 */
      /* !LINKSTO Dcm.EB.WriteDataByIdentifier.EndiannessInterpretation.NextDestByte.OpaqueOnBigEndianCPU.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
      ByteInDest = CurrentByteInDestBuffer + 1U;
      break;
    }
    case DCM_COPY_TO_OUTBUFFER_BIGENDIAN:
    {
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.NextDestByte.BigEndian.USE_DATA_SENDER_RECEIVER,1 */
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.NextDestByte.BigEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
      /* If copying to a buffer big endian, go to the previous byte in destination */
      ByteInDest = CurrentByteInDestBuffer - 1U;
      break;
    }
    case DCM_COPY_TO_OUTBUFFER_LITTLEENDIAN:
    case DCM_COPY_TO_OUTBUFFER_OPAQUE:
    {
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.NextDestByte.LittleEndian.USE_DATA_SENDER_RECEIVER,1 */
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.NextDestByte.LittleEndian.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.NextDestByte.Opaque.USE_DATA_SENDER_RECEIVER,1 */
      /* !LINKSTO Dcm.EB.ReadDataByIdentifier.EndiannessInterpretation.NextDestByte.Opaque.USE_DATA_SENDER_RECEIVER_AS_SERVICE,1 */
      /* If copying to a buffer little or opaque endian, go to the next byte in destination */
      ByteInDest = CurrentByteInDestBuffer + 1U;
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

  return ByteInDest;
}
#endif /*#if ((DCM_DSP_USE_SERVICE_0X2E == STD_ON) || (DCM_DSP_USE_SERVICE_0X22 == STD_ON) || \
 (DCM_DSP_USE_SERVICE_0X86 == STD_ON) || (DCM_DSP_USE_SERVICE_0X31 == STD_ON) || \
 (DCM_DSP_USE_SERVICE_0X2F == STD_ON) || (DCM_DSP_USE_SERVICE_0X2A == STD_ON) || \
 (DCM_DSP_USE_SERVICE_0X2C == STD_ON) || (DCM_DSP_USE_SERVICE_0X24 == STD_ON) || \
 (((DCM_DSP_USE_SERVICE_0X01 == STD_ON) || \
   (DCM_DSP_USE_SERVICE_0X02 == STD_ON)) && \
  (DCM_NUM_PID_CFG > 0U)))*/

FUNC (void, DCM_CODE) Dcm_Dsp_ServiceProcessorInit(void)
{
  Dcm_AsynchronousProtocol = DCM_INVALID_ASYNCHRONOUS;

  Dcm_Dsp_ServiceProcessorContextInit(&Dcm_ServiceProcessingContext_OBD);

  Dcm_Dsp_ServiceProcessorContextInit(&Dcm_ServiceProcessingContext_UDS);

#if (DCM_HAS_ROE_TYPE2 == STD_ON)
  Dcm_Dsp_ServiceProcessorContextInit(&Dcm_ServiceProcessingContext_ROE_TYPE2);
#endif
}

FUNC (void, DCM_CODE) Dcm_Dsp_ServiceProcessorReset(Dcm_ProtocolIndexType ProtocolIdx)
{
  Dcm_ServiceProcessingContext_PtrType ServiceProcessingContextPtr =
    Dcm_Dsp_GetServiceProcessingContext(ProtocolIdx);

  DBG_DCM_DSP_SERVICEPROCESSORINIT_ENTRY();

  Dcm_Dsp_ServiceProcessorContextInit(ServiceProcessingContextPtr);

  DBG_DCM_DSP_SERVICEPROCESSORINIT_EXIT();
}

FUNC (void, DCM_CODE) Dcm_Dsp_Protocol_P2TimerTimeout(Dcm_ProtocolIndexType ProtocolIndex)
{

  DBG_DCM_DSP_PROTOCOL_P2TIMERTIMEOUT_ENTRY(ProtocolIndex);

#if (DCM_HSM_INST_MULTI_ENABLED == STD_OFF)
  TS_PARAM_UNUSED (ProtocolIndex);
#endif /* #if (DCM_HSM_INST_MULTI_ENABLED == STD_OFF) */

  /* emit the event TIMEOUT2 */
  /*!LINKSTO Dcm.EB.P2(*)Server.Timeout.Actions.Protocol,1 */
  (void)DCM_HSMEMITINST_WRAPPER(&Dcm_HsmScDcm, ProtocolIndex, DCM_HSM_DCM_EV_TIMEOUT2);

  DBG_DCM_DSP_PROTOCOL_P2TIMERTIMEOUT_EXIT();
}

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[end of file]================================================================*/
