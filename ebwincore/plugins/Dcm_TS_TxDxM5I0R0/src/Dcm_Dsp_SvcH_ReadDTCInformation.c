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

/* !LINKSTO Dcm.Dsn.File.RDTCI.Impl,1 */
/* This file contains the implementation of the implementation of the Diagnostic
 * Service ReadDTCInformation. */

/*  MISRA-C:2004 Deviation List
 *
 *  MISRA-1) Deviated Rule: 15.2 (required)
 *     "An unconditional break statement shall terminate every non-empty switch clause."
 *
 *     Reason:
 *     The following actions have to be executed in the same state so the break
 *     statement. The fallthrough allows shorter and non-duplicated code.
 *
 *  MISRA-2) Deviated Rule: 14.6 (required)
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
  *  MISRAC2012-1) Deviated Rule: 16.3 (required)
  *     "An unconditional break statement shall terminate every switch-clause."
  *
  *     Reason:
  *     The following actions have to be executed in the same state so the break
  *     statement is removed in order to have shorter code and avoid duplicated code.
  *
  *  MISRAC2012-2) Deviated Rule: 15.4 (advisory)
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

/*==================[includes]===================================================================*/

#include <Dcm_Trace.h>
#include <Std_Types.h>                                                 /* AUTOSAR standard types */
#include <TSAutosar.h>
#include <Dcm_Dsp_SvcH_ReadDTCInformation.h>
#include <Dcm_Dsl_Supervisor.h>

/* The functions provided and implemented by this unit are exclusively used for
 * UDS services 0x19 and 0xAF handling. */
#if ( (DCM_DSP_USE_SERVICE_0X19 == STD_ON) || (DCM_DSP_USE_SERVICE_0XAF == STD_ON) )

#include <SchM_Dcm.h>                                                  /* SchM interface for Dcm */

/*==================[macro definitions]==========================================================*/

#if (DCM_NUM_PROTOCOL > 1U)
#if (defined DCM_READ_DTC_INFO_CONTEXT)
  #error "DCM_READ_DTC_INFO_CONTEXT is already defined"
#endif
#define DCM_READ_DTC_INFO_CONTEXT(instIdx) Dcm_ReadDTCInfoContext[(instIdx)]
#else
#if (defined DCM_READ_DTC_INFO_CONTEXT)
  #error "DCM_READ_DTC_INFO_CONTEXT is already defined"
#endif
#define DCM_READ_DTC_INFO_CONTEXT(instIdx) Dcm_ReadDTCInfoContext[0U]
#endif /* #if (DCM_NUM_PROTOCOL > 1U) */

#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
#if (defined DCM_FIVE_BYTES_OF_HEADER_PER_DTC)
  #error "DCM_FIVE_BYTES_OF_HEADER_PER_DTC is already defined"
#endif
#define DCM_FIVE_BYTES_OF_HEADER_PER_DTC 5U
#endif /* #if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)*/

#if (defined DCM_DTC_ORIGIN_USER_MEMORY_BASE)
  #error "DCM_DTC_ORIGIN_USER_MEMORY_BASE is already defined"
#endif
#define DCM_DTC_ORIGIN_USER_MEMORY_BASE   (0x0100U)

/*==================[type declarations]==========================================================*/

/*==================[internal function declarations]=============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

#if (DCM_DSP_USE_SERVICE_0X19 == STD_ON)
/** \brief Internal start function for UDS service 0x19
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP  The DCM shall send an "response pending" immediately
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReadDTCInformation_SvcStart(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if (DCM_DSP_USE_SERVICE_0X19 == STD_ON) */

#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)

/** \brief UDS service 0xAF
 **
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP  The DCM shall send an "response pending" immediately
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReadGenericInformation_SvcStart(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#endif /* #if (DCM_DSP_USE_SERVICE_0XAF == STD_ON) */

/** \brief Internal execution function UDS services 0x19 and 0xAF
 **
 ** \param[in] OpStatus Status of the service processing
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_FORCE_RCRRP  The DCM shall send an "response pending" immediately
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReadDTCInformation_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x03 == STD_ON)
/** \brief UDS service 0x19-0x03
 **
 ** \param[in]    DemClientId This parameter defines the Dem Client.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportDtcSnapshotRecordIdent_Process(
  Dcm_DemClientIdType DemClientId,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x03 == STD_ON) */

#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)

#if (DCM_DSP_USE_SERVICE_0X19 == STD_ON)
/** \brief This function is used to update the response buffer pointer and the data length
 **
 ** \param[in] pMsgContext Message related information for one diagnostic protocol.
 **
 ** \retval None
 */
STATIC FUNC(void, DCM_CODE) Dcm_DspInternal_UpdateResponseBuffer(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if (DCM_DSP_USE_SERVICE_0X19 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x03 == STD_ON)
/** \brief UDS service 0x19-0x03
 **
 ** \param[in] DemClientId This parameter defines the Dem Client instance.
 ** \param[in] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ProcessPageDTCSnapshotByIdentification (
  Dcm_DemClientIdType DemClientId,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext
);
#endif
#if (DCM_DSP_USE_SERVICE_0X19 == STD_ON)
#if ((DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x06 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x19 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x18 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x04 ==STD_ON))
/** \brief UDS service 0x19-0x19 and 0x19-0x06
 **
 ** \param[in] DemClientId This parameter defines the Dem Client instance.
 ** \param[in] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_PENDING       Operation did not complete, it must be called again
 ** \retval DCM_E_PROCESS_PAGE  One page is ready to be given to the PduR
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ProcessPageDtcExtDataRecord
(
  Dcm_DemClientIdType DemClientId,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext
);
#endif/* #if ((DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x06 == STD_ON) || \
              (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x19 == STD_ON) || \
              (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x18 == STD_ON) || \
              (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x04 ==STD_ON)) */
#endif /* #if (DCM_DSP_USE_SERVICE_0X19 == STD_ON) */

#if ((DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x17 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x15 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x14 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x13 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0A == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x02 == STD_ON))
/* Page processing function of service report filtered DTCs*/
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ProcessPageFilteredDTCs
(
  Dcm_DemClientIdType DemClientId,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext
);
#endif

#endif /* #if (DCM_PAGEDBUFFER_ENABLED == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

/*==================[internal constant definitions]==============================================*/

/*==================[internal data definitions]==================================================*/

#define DCM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#if ((DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x03 == STD_ON) ||\
     ((DCM_DSP_USE_SERVICE_0X19 == STD_ON) && (DCM_HAS_ROE_TYPE2 == STD_ON) &&\
      (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U)))
/* Dcm_ReadDTCInfoContext is used to hold information regarding the processing of
 * the service ReadDTCInformation(0x19).
 */
/* Rationale for mapping Dcm_ReadDTCInfoContext to NOINIT section.
 *
 * Members of Dcm_ReadDTCInfoContext are initialized and accessed as described below:
 * DTCCount  : Initialized in Dcm_DspInternal_ReportDTCByStatusMask_SSvcH() before
 *             being accessed from the same function.
 * Dcm_ReadDTCInformationHasFacade: Initialized in Dcm_DspInternal_ReadDTCInformation_SvcH() before
 *             being accessed from the same function.
 */
/* Deviation MISRAC2012-3 */
STATIC VAR(Dcm_ReadDTCInfoContextType, DCM_VAR) Dcm_ReadDTCInfoContext[DCM_NUM_PROTOCOL];
#endif

STATIC VAR(Dcm_GenericServiceHandlerType, DCM_VAR) Dcm_ReadDTCInformation_SubServiceHandler;

#define DCM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Dcm_MemMap.h>

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U) &&\
     (DCM_DSP_USE_SERVICE_0XAF == STD_ON))
#define DCM_START_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>

/* This service cannot be executed from two protocols at the same time (it is not available as a
 * ServiceToRespondTo for ROE), therefore it is safe to use only one status variable for
 * whether the Facade has been locked by it */
/* Deviation MISRAC2012-3 */
STATIC VAR(boolean, DCM_VAR) Dcm_ReadGenericInformationHasFacade;

#define DCM_STOP_SEC_VAR_CLEARED_8
#include <Dcm_MemMap.h>
#endif

/*==================[external constant definitions]==============================================*/

/*==================[external data definitions]==================================================*/

/*==================[external function definitions]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

#if (DCM_DSP_USE_SERVICE_0X19 == STD_ON)
/* This is the generic service handler for UDS service 0x19 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReadDTCInformation_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

#if (((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U)) || \
     (DCM_PAGEDBUFFER_ENABLED == STD_ON))
  Dcm_ProtocolIndexType instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(instIdx);
#endif /* #if (((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U)) || \
               (DCM_PAGEDBUFFER_ENABLED == STD_ON)) */

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
  P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U)) */

#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)

  /* Get the current context info */
  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);

#if (DCM_DSP_SUBSERVICES_READDTCINFORMATION_EXTSVC_ENABLED == STD_OFF)
  STATIC CONST(Dcm_ProcessPageFuncType, DCM_CONST) Dcm_ReadDTC_ProcPageFunc[3U] =
  {
#if ((DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x02 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0A == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x13 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x14 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x15 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x17 == STD_ON))
    &Dcm_DspInternal_ProcessPageFilteredDTCs,
#else
    NULL_PTR,
#endif

#if ((DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x19 == STD_ON) || \
    (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x06 == STD_ON) || \
    (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x04 == STD_ON) || \
    (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x18 == STD_ON))
    &Dcm_DspInternal_ProcessPageDtcExtDataRecord,
#else
    NULL_PTR,
#endif

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x03 == STD_ON)
    &Dcm_DspInternal_ProcessPageDTCSnapshotByIdentification,
#else
    NULL_PTR,
#endif
  };
#endif /* #if (DCM_DSP_SUBSERVICES_READDTCINFORMATION_EXTSVC_ENABLED == STD_OFF) */

  DBG_DCM_DSPINTERNAL_READDTCINFORMATION_SVCH_ENTRY(OpStatus,pMsgContext);

#endif /* #if (DCM_PAGEDBUFFER_ENABLED == STD_ON) */

  switch (OpStatus)
  {
    /* first call of service function */
    case DCM_INITIAL:
    {
#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
      /* !LINKSTO Dcm.EB.ReadDTCInformation.DemFacade.Locking,1 */
      Std_ReturnType DcmFacadeLockStatus = Dcm_Dsp_DemFacade_LockClient(DemClientId) ;

      /* Set the ReadDTCInformation lock flag */
      DCM_READ_DTC_INFO_CONTEXT(instIdx).Dcm_ReadDTCInformationHasFacade = FALSE;

      /* If the DemFacade could NOT be locked because they are already locked by something else */
      if (DcmFacadeLockStatus == DCM_E_RESOURCE_LOCKED)
      {
        /* If the DemFacade could not be locked the service handler returns
         * NRC ConditionsNotCorrect (0x22)  */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
        serviceProcessStatus = DCM_E_DONE;
      }
      else
      {
        DCM_READ_DTC_INFO_CONTEXT(instIdx).Dcm_ReadDTCInformationHasFacade = TRUE;
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U)) */
        serviceProcessStatus = Dcm_DspInternal_ReadDTCInformation_SvcStart(pMsgContext);

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
      }
#endif
      /* first page of data is being filled from here, if paged processing has been enabled */

#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
      if (curHsmInfo->processPageFuncId != DCM_PROCESS_PAGE_ID_INVALID)
      {
        /* update the response buffer pointer and the response length */
        Dcm_DspInternal_UpdateResponseBuffer(pMsgContext);

#if (DCM_DSP_SUBSERVICES_READDTCINFORMATION_EXTSVC_ENABLED == STD_OFF)
        /* fill a page of data */
        serviceProcessStatus =
          Dcm_ReadDTC_ProcPageFunc[curHsmInfo->processPageFuncId](DemClientId, pMsgContext);
#endif
      }
#endif
    }
      break;

    /* consecutive call after the function returned PENDING before */
    case DCM_PENDING:
    case DCM_FORCE_RCRRP_OK:
      /* Depending on whether the paged processing is started or not,
         the corresponding service handler function or pagedprocessing function
         needs to be called from here */
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
      if (curHsmInfo->processPageFuncId != DCM_PROCESS_PAGE_ID_INVALID)
      {
#if (DCM_DSP_SUBSERVICES_READDTCINFORMATION_EXTSVC_ENABLED == STD_OFF)
        /* fill a page of data */
        serviceProcessStatus =
          Dcm_ReadDTC_ProcPageFunc[curHsmInfo->processPageFuncId](DemClientId, pMsgContext);
#endif
      }
      else
#endif
      {
        serviceProcessStatus = Dcm_DspInternal_ReadDTCInformation_SvcCont(OpStatus, pMsgContext);
        /* first page of data is being filled from here, if paged processing has been enabled */
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
        if (curHsmInfo->processPageFuncId != DCM_PROCESS_PAGE_ID_INVALID)
        {
          /* update the response buffer pointer and the response length */
          Dcm_DspInternal_UpdateResponseBuffer(pMsgContext);
#if (DCM_DSP_SUBSERVICES_READDTCINFORMATION_EXTSVC_ENABLED == STD_OFF)
          /* fill a page of data */
          serviceProcessStatus =
            Dcm_ReadDTC_ProcPageFunc[curHsmInfo->processPageFuncId](DemClientId, pMsgContext);
#endif
        }
#endif
      }
      break;

    case DCM_CANCEL:
      /* !LINKSTO Dcm.EB.DemIF.RequestCancellation, 1 */
      (void) Dcm_DspInternal_ReadDTCInformation_SvcCont(OpStatus, pMsgContext);
      serviceProcessStatus = DCM_E_DONE;
      break;

    case DCM_CONFIRMED_OK:
    case DCM_CONFIRMED_NOK:
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
    /* cancel any ongoing service processing, during paged processing */
      /* reset the paged buffer function id */
      curHsmInfo->processPageFuncId = DCM_PROCESS_PAGE_ID_INVALID;
#endif
      serviceProcessStatus = DCM_E_DONE;
      break;

#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
    /* update the next page in paged buffer processing */
    case DCM_UPDATE_PAGE:
      /* the process page id should be valid */
      DCM_PRECONDITION_ASSERT(curHsmInfo->processPageFuncId != DCM_PROCESS_PAGE_ID_INVALID,
                              DCM_SID_READ_DTC_INFORMATION);
#if (DCM_DSP_SUBSERVICES_READDTCINFORMATION_EXTSVC_ENABLED == STD_OFF)
      /* call the corresponding paged processing function */
      serviceProcessStatus = Dcm_ReadDTC_ProcPageFunc[curHsmInfo->processPageFuncId](DemClientId, pMsgContext);
#endif
      break;
#endif

    /* CHECK: NOPARSE */
    default:
      /* Defensive branch. Default case at the end of the switch statement */
      DCM_UNREACHABLE_CODE_ASSERT(DCM_SID_READ_DTC_INFORMATION);
      break;
    /* CHECK: PARSE */
  }

/* Unlock the Facade unit in the next situation:
 *   - the Facade unit is still locked by ReadDTCInformation service and
 *   - all amount of data was sent by page buffering mechanism or
 *   - a lower layer confirm the transition before all amount of data is sent or
 *   - the service handler is in a final state
 */
#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
  if((DCM_READ_DTC_INFO_CONTEXT(instIdx).Dcm_ReadDTCInformationHasFacade == TRUE) &&
     ((ClientContextPtr->PageBufferingDone == TRUE) ||
     (OpStatus == DCM_CONFIRMED_OK) ||
     (OpStatus == DCM_CONFIRMED_NOK) ||
     (serviceProcessStatus == DCM_E_DONE))
   )
  {
    Dcm_Dsp_DemFacade_UnlockClient(DemClientId);
    DCM_READ_DTC_INFO_CONTEXT(instIdx).Dcm_ReadDTCInformationHasFacade = FALSE;
  }
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U)) */

  DBG_DCM_DSPINTERNAL_READDTCINFORMATION_SVCH_EXIT(serviceProcessStatus,OpStatus,pMsgContext);

  return serviceProcessStatus;
}
#endif /* #if (DCM_DSP_USE_SERVICE_0X19 == STD_ON) */

#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
/* This is the service handler for Read Generic Information service 0xAF */
/* !LINKSTO Dcm.ReadGenericInformation.Implementation,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReadGenericInformation_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  Std_ReturnType ResutlFromGetNextDTC = DEM_NO_SUCH_ELEMENT;
  Dcm_OpStatusType OperationStatus;

  /* complete amount of data gathered from all requested DTCs */
  Dcm_MsgLenType AmountOfDataGathered = 0U;
#if (DCM_MAX_CONSEC_GET_EXT_DATA_REC != 0U)
  uint8 NumberOfIterations = 0;
#endif /* #if (DCM_MAX_CONSEC_GET_EXT_DATA_REC != 0U)*/
#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U) &&\
     (DCM_POSTCONDITION_ASSERT_ENABLED == STD_ON))
   Std_ReturnType DcmFacadeLockStatus = DCM_E_RESOURCE_LOCKED;
#endif

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);
  P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);

  DBG_DCM_DSPINTERNAL_READGENERICINFORMATION_SVCH_ENTRY(OpStatus, pMsgContext);

  OperationStatus = OpStatus;

  /* Deviation MISRAC2012-4 */
  switch (OperationStatus)
  {
    /* first call of service function */
    /* Deviation MISRAC2012-1 */
    /* Deviation MISRA-1 */
    case DCM_INITIAL:
    {
#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
      /* Set the ReadGenericInformation lock flag */
      Dcm_ReadGenericInformationHasFacade = FALSE;
#if (DCM_POSTCONDITION_ASSERT_ENABLED == STD_ON)
      /* !LINKSTO Dcm.EB.ReadGenericInformation.DemFacade.Locking,1 */
      DcmFacadeLockStatus = Dcm_Dsp_DemFacade_LockClient(DemClientId) ;
#else
      (void)Dcm_Dsp_DemFacade_LockClient(DemClientId) ;
#endif /* #if (DCM_POSTCONDITION_ASSERT_ENABLED == STD_ON) */

      /* The above locking operation should never fail */
      DCM_POSTCONDITION_ASSERT(DcmFacadeLockStatus != DCM_E_RESOURCE_LOCKED, DCM_INTERNAL_API_ID);

      Dcm_ReadGenericInformationHasFacade = TRUE;
#endif /* #if((DCM_HAS_ROE_TYPE2 == STD_ON)&&(DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U)) */

      /* setup of Service Context, set of Stored DTCs filter and obtain DTC from request.*/
      serviceProcessStatus = Dcm_DspInternal_ReadGenericInformation_SvcStart(pMsgContext);

      if (serviceProcessStatus == DCM_E_NOT_OK)
      {
        /* Negative response has been already set and processing has ended in this case.
         * Break processing */
        serviceProcessStatus = DCM_E_DONE;
        break;
      }
    }
      /* no break */
      /* consecutive call after the function returned PENDING before */
    case DCM_PENDING:
    case DCM_FORCE_RCRRP_OK:
      while (Dcm_ReadEventDataServicesContext.ReachedEndOfRequest == FALSE)
      {
#if (DCM_MAX_CONSEC_GET_EXT_DATA_REC != 0U)
        /* !LINKSTO Dcm.EB.ReadGenericInformationContainer,1 */
        /* !LINKSTO Dcm.EB.DcmDspMaxDemGetDiagnosticDataConsecutiveExecutions,1 */
        if(NumberOfIterations == DCM_MAX_CONSEC_GET_EXT_DATA_REC)
        {
          serviceProcessStatus = DCM_E_PENDING;
          /* Deviation MISRAC2012-2 */
          /* Deviation MISRA-2 */
          break;
        }
        NumberOfIterations++;
#endif /* #if (DCM_MAX_CONSEC_GET_EXT_DATA_REC != 0U)*/
        if((Dcm_ReadEventDataServicesContext.RequestAllDTCs == TRUE)&&
           (ClientContextPtr->CurrentDTCProcessingReady == TRUE))
        {
          ClientContextPtr->CurrentDTCProcessingReady = FALSE;
          /* Trick the sub-service handler into obtaining everything again */
          OperationStatus = DCM_INITIAL;
          /* !LINKSTO Dcm.ReadGenericInformation.ObtainStoredDTCs,2 */
          /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SetStoredDTCFilter.E_OK,1 */
          /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SetStoredDTCFilter.E_OK,1 */
          /* !LINKSTO Dcm.ReadGenericInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
          ResutlFromGetNextDTC = Dem_DcmGetNextFilteredStoredDTC(
                                  DemClientId,
                                  &(ClientContextPtr->DTCMask));
          if (E_OK == ResutlFromGetNextDTC)
          {
             /* If the Get next filter function does not return with not matching
             DTC on the very first run this flag indicates that. */
             Dcm_ReadEventDataServicesContext.AtLeastOneDTCExists = TRUE;
             Dcm_ReadEventDataServicesContext.AmountOfDTCsProcessed++;
          }
          else if (DEM_NO_SUCH_ELEMENT == ResutlFromGetNextDTC)
          {
             Dcm_ReadEventDataServicesContext.ReachedEndOfRequest = TRUE;
             /* Deviation MISRAC2012-2 */
             /* Deviation MISRA-2 */
             break;
          }
          else
          {
             /* Return code is INVALID or E_NOT_OK - send GeneralReject, DET (if error
             detection is enabled), and set the service process status to done. */

             /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextFilteredStoredDTC.E_NOT_OK.NRC,1 */
             /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextFilteredStoredDTC.E_NOT_OK.NRC,1 */
             /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextFilteredStoredDTC.UnsupportedError.NRC,1 */
             /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextFilteredStoredDTC.UnsupportedError.NRC,1 */
             Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);

             /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextFilteredStoredDTC.E_NOT_OK.DET,1 */
             /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextFilteredStoredDTC.E_NOT_OK.DET,1 */
             /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextFilteredStoredDTC.UnsupportedError.DET,1 */
             /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextFilteredStoredDTC.UnsupportedError.DET,1 */
#if (DCM_DEV_ERROR_DETECT == STD_ON)
             DCM_DET_REPORT_ERROR(DCM_SVCID_READDTCINFORMATION, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* if (DCM_DEV_ERROR_DETECT == STD_ON) */
             serviceProcessStatus = DCM_E_DONE;
            /* Deviation MISRAC2012-2 */
            /* Deviation MISRA-2 */
             break;
          }
        }
        if(Dcm_ReadEventDataServicesContext.RequestAllDTCs == FALSE)
        {
          Dcm_ReadEventDataServicesContext.AmountOfDTCsProcessed = 1U;
        }
        /* Execute the sub-service routine */
        /* !LINKSTO Dcm.ReadGenericInformation.ReportDtcExtendedDataRecordByDtcNumber.Implementation,1 */
        /* !LINKSTO Dcm.ReadGenericInformation.ReportDtcSnapshotRecordByDtcNumber.Implementation,1 */
        serviceProcessStatus = Dcm_DspInternal_ReadDTCInformation_SvcCont(OperationStatus, pMsgContext);
        if ((Dcm_ReadEventDataServicesContext.RequestAllDTCs == FALSE)&&
            (serviceProcessStatus != DCM_E_PENDING)&&
            (serviceProcessStatus != DCM_E_FORCE_RCRRP))
        {
          /* If only one DTC is requested, the loop is executed only one time */
          Dcm_ReadEventDataServicesContext.ReachedEndOfRequest = TRUE;
          Dcm_ReadEventDataServicesContext.AtLeastOneDTCExists = TRUE;
        }
        if ((serviceProcessStatus == DCM_E_NOT_OK)||
            (serviceProcessStatus == DCM_E_PENDING)||
            (serviceProcessStatus == DCM_E_FORCE_RCRRP))
        {
          if (serviceProcessStatus == DCM_E_NOT_OK)
          {
            /* Clear flag to indicate that the request has ended */
            Dcm_ReadEventDataServicesContext.ReachedEndOfRequest = TRUE;
            serviceProcessStatus = DCM_E_DONE;
          }
          /* Deviation MISRAC2012-2 */
          /* Deviation MISRA-2 */
          break;
        }

      }
      if(Dcm_ReadEventDataServicesContext.ReachedEndOfRequest == TRUE)
      {
        /* Calculate the amount of data written on the complete request */
        if(ClientContextPtr->DataKind == DCM_FACADE_FREEZE_FRAME_DATA)
        {
          AmountOfDataGathered = pMsgContext->resDataLen;
        }
        else
        {
          AmountOfDataGathered =
          ( pMsgContext->resDataLen - ((uint32)(DCM_FIVE_BYTES_OF_HEADER_PER_DTC) *
             (uint32)(Dcm_ReadEventDataServicesContext.AmountOfDTCsProcessed)) );
        }
        serviceProcessStatus = DCM_E_DONE;
      }
      if(((Dcm_ReadEventDataServicesContext.AtLeastOneDTCExists == FALSE)||
        (AmountOfDataGathered == 1U))&&
       (serviceProcessStatus != DCM_E_PENDING)&&
       (serviceProcessStatus != DCM_E_FORCE_RCRRP))
      {
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.GetNextExtendedDataRecord.DEM_NO_SUCH_ELEMENT.NoValue,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.GetNextFreezeFrameData.DEM_NO_SUCH_ELEMENT.NoValue,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_CONDITIONSNOTCORRECT);
        serviceProcessStatus = DCM_E_DONE;
      }
       break;
    /* Deviation MISRAC2012-1 */
    /* Deviation MISRA-1 */
    case DCM_CANCEL:
      /* !LINKSTO Dcm.EB.DemIF.RequestCancellation, 1 */
      (void) Dcm_DspInternal_ReadDTCInformation_SvcCont(OperationStatus, pMsgContext);
      /* no break */
    default:
      serviceProcessStatus = DCM_E_DONE;
      break;
  }

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
  if((Dcm_ReadGenericInformationHasFacade == TRUE) &&
     ((ClientContextPtr->PageBufferingDone == TRUE) ||
     (OperationStatus == DCM_CONFIRMED_OK) ||
     (OperationStatus == DCM_CONFIRMED_NOK) ||
     (serviceProcessStatus == DCM_E_DONE))
    )
  {
    Dcm_Dsp_DemFacade_UnlockClient(DemClientId);
    Dcm_ReadGenericInformationHasFacade = FALSE;
  }
#endif /* #if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U)) */

  DBG_DCM_DSPINTERNAL_READGENERICINFORMATION_SVCH_EXIT(serviceProcessStatus, OpStatus, pMsgContext);

  return serviceProcessStatus;
}

#endif /*#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x01 == STD_ON)
/* This is the UDS service 0x19-0x01 handler */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportNumberOfDTCByStatusMask_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSPINTERNAL_REPORTNUMBEROFDTCBYSTATUSMASK_SSVCH_ENTRY(OpStatus, pMsgContext);
  /* Check the length of the request */
  if (pMsgContext->reqDataLen == 1U)
  {
    if (OpStatus == DCM_INITIAL)
    {
      P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);

      ClientContextPtr->SubServiceID = DCM_DSP_READDTCINFORMATION_0x01_ID;
      ClientContextPtr->DTCFormat = DEM_DTC_FORMAT_UDS;
      ClientContextPtr->MemoryOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
      ClientContextPtr->FilterForFaultDetectionCounter = FALSE;
      /* Initialize page buffering flag */
      ClientContextPtr->PageBufferingFlag = FALSE;
      /* Initialize the size of a single page*/
      ClientContextPtr->SizeOfOnePage = pMsgContext->resMaxDataLen + 1U;
      ClientContextPtr->AmountOfDataWrittenOnCurrentPage = 2U;
      ClientContextPtr->RequestedStatusMask = pMsgContext->reqData[0];
      ClientContextPtr->RequestedStatusBits = pMsgContext->reqData[0];
      ClientContextPtr->FirstPageSent = FALSE;
      /* Data Supplier Switches */
      ClientContextPtr->MemoryOriginState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->AvailabilityMaskState = DCM_DATA_SUPPLIER_REQUIRED;
      ClientContextPtr->DtcFormatIdState = DCM_DATA_SUPPLIER_REQUIRED;
      ClientContextPtr->FilteredDtcsCountState = DCM_DATA_SUPPLIER_REQUIRED;
      ClientContextPtr->FilteredDtcsState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
    }
    serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoByFilter_Process(DemClientId, OpStatus, pMsgContext);
  }
  /* request data length is invalid */
  else
  {
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    serviceProcessStatus = DCM_E_DONE;
  }

  DBG_DCM_DSPINTERNAL_REPORTNUMBEROFDTCBYSTATUSMASK_SSVCH_EXIT(serviceProcessStatus, OpStatus,
    pMsgContext);

  return serviceProcessStatus;
}
#endif /* #if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x01 == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x02 == STD_ON)
/* This is the UDS service 0x19-0x02 handler */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportDTCByStatusMask_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSPINTERNAL_REPORTDTCBYSTATUSMASK_SSVCH_ENTRY(OpStatus, pMsgContext);
  /* Check the length of the request */
  if (pMsgContext->reqDataLen == 1U)
  {
    if (OpStatus == DCM_INITIAL)
    {
      P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);

      ClientContextPtr->SubServiceID = DCM_DSP_READDTCINFORMATION_0x02_ID;
      /* Explicit and implicit requested parameter */
      ClientContextPtr->DTCFormat = DEM_DTC_FORMAT_UDS;
      ClientContextPtr->MemoryOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
      ClientContextPtr->FilterForFaultDetectionCounter = FALSE;
      /* Initialize page buffering flag */
      ClientContextPtr->PageBufferingFlag = FALSE;
      /* Initialize the size of a single page*/
      ClientContextPtr->SizeOfOnePage = pMsgContext->resMaxDataLen + 1U;
      ClientContextPtr->AmountOfDataWrittenOnCurrentPage = 2U;
      ClientContextPtr->RequestedStatusMask = pMsgContext->reqData[0];
      ClientContextPtr->RequestedStatusBits = pMsgContext->reqData[0];
      ClientContextPtr->FirstPageSent = FALSE;
      /* Data Supplier Switches */
      ClientContextPtr->MemoryOriginState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->AvailabilityMaskState = DCM_DATA_SUPPLIER_REQUIRED;
      ClientContextPtr->DtcFormatIdState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->FilteredDtcsCountState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->FilteredDtcsState = DCM_DATA_SUPPLIER_REQUIRED;
    }
    serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoByFilter_Process(DemClientId, OpStatus, pMsgContext);
  }
  /* request data length is invalid */
  else
  {
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    serviceProcessStatus = DCM_E_DONE;
  }

  DBG_DCM_DSPINTERNAL_REPORTDTCBYSTATUSMASK_SSVCH_EXIT(serviceProcessStatus,
    OpStatus, pMsgContext);
  return serviceProcessStatus;
}
#endif  /* #if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x02 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x03 == STD_ON)
/* This is the UDS service 0x19-0x03 handler */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportDtcSnapshotRecordIdentification_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  Dcm_ProtocolIndexType instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(instIdx);

  DBG_DCM_DSPINTERNAL_REPORTDTCSNAPSHOTRECORDIDENTIFICATION_SSVCH_ENTRY(OpStatus,pMsgContext);

  if (OpStatus == DCM_INITIAL)
  {
    /* first call of service function */

    /* Check the request data length */
    if (pMsgContext->reqDataLen == 0U)
    {
#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
      /* Get the current context info */
      CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);
      P2VAR(Dcm_BufferSizeType, AUTOMATIC, DCM_VAR) FilledLengthPtr =
        &(curHsmInfo->TxBuffer.FilledLength);
#endif
      Std_ReturnType resultFreezeRecord;

      /* Get the number of filtered freeze frame records */
      /* !LINKSTO SWS_Dcm_00836,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotIdentification.GetNumberOfFreezeFrameRecords,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
      resultFreezeRecord =
        Dem_SetFreezeFrameRecordFilter(DemClientId,
                                       DEM_DTC_FORMAT_UDS,
                                       &DCM_READ_DTC_INFO_CONTEXT(instIdx).DTCCount);

#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)

      if (resultFreezeRecord == E_OK)
      {
        /* update the total response length for the filtered DTCs */
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotIdentification.PageBufferEnabled,1 */
        /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotIdentification.PageBufferEnabled.DTCZero,1 */
        pMsgContext->resDataLen +=
                  ((Dcm_MsgLenType)DCM_READ_DTC_INFO_CONTEXT(instIdx).DTCCount *
                  (Dcm_MsgLenType)DCM_DTC_SNAPSHOT_SNGL_RCD_LEN);
        serviceProcessStatus = DCM_E_DONE;
      }
      else if(E_NOT_OK == resultFreezeRecord)
      {
        /* !LINKSTO SWS_Dcm_01256,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
        serviceProcessStatus = DCM_E_DONE;
      }
      /* Getting the number of filtered freeze frame records failed */
      else
      {
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotIdentification.SetFreezeFrameRecordFilter.UnsupportedError.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotIdentification.SetFreezeFrameRecordFilter.UnsupportedError.DET,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
        serviceProcessStatus = DCM_E_DONE;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        DCM_DET_REPORT_ERROR(DCM_SVCID_READDTCINFORMATION, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
      }

      /* if no error occurred till now and filled length is less than total response length */
      if ((curHsmInfo->negativeResponseCode == DCM_E_OK) &&
          (*FilledLengthPtr < pMsgContext->resDataLen))
      {
        /* set the paged processing status to ON */
        Dcm_DspInternal_EnableProcessPage(pMsgContext,
                                          DCM_PROCESS_PAGE_DTC_SNAPSHOT_IDENTIFICATION);

        /* indicate start of paged processing */
        Dcm_DsdInternal_StartPagedProcessing(pMsgContext);

        /* set serviceProcessStatus to pending so that DSP state machine
         * will stay in state 'ApplQueried', so that CallStdService() will be
         * called to continue paged processing */
        serviceProcessStatus = DCM_E_PROCESS_PAGE;
      }
#else
      if (E_OK == resultFreezeRecord)
      {
        if(DCM_READ_DTC_INFO_CONTEXT(instIdx).DTCCount > 0U)
        {
          /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotIdentification.PageBufferDisabled,1 */
          serviceProcessStatus =
            Dcm_DspInternal_ReportDtcSnapshotRecordIdent_Process(DemClientId, pMsgContext);
        }
        else
        {
          serviceProcessStatus = DCM_E_DONE;
        }
      }
      /* Sets a freeze frame record filter failed */
      else if(E_NOT_OK == resultFreezeRecord)
      {
        /* !LINKSTO SWS_Dcm_01256,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
        serviceProcessStatus = DCM_E_DONE;
      }
      else
      {
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotIdentification.SetFreezeFrameRecordFilter.UnsupportedError.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotIdentification.SetFreezeFrameRecordFilter.UnsupportedError.DET,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
        serviceProcessStatus = DCM_E_DONE;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        DCM_DET_REPORT_ERROR(DCM_SVCID_READDTCINFORMATION, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
      }

#endif /* DCM_PAGEDBUFFER_ENABLED == STD_ON */
    }
    /* if diagnostic request data length is invalid */
    else
    {
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
      serviceProcessStatus = DCM_E_DONE;
    }
  }
  else if (OpStatus == DCM_PENDING)
  {
    /* consecutive call after the function returned PENDING before */
    serviceProcessStatus =
    Dcm_DspInternal_ReportDtcSnapshotRecordIdent_Process(DemClientId, pMsgContext);
  }
  /* Since this function has the potential of returning Pending to the client
   * it has to have a way of dealing with a cancellation request when the pending
   * conditions have reached the maximum limit */
  else
  {
    /* !LINKSTO Dcm.EB.DemIF.RequestCancellation, 1 */
    DCM_PRECONDITION_ASSERT((OpStatus == DCM_CANCEL), DCM_SID_READ_DTC_INFORMATION);
    serviceProcessStatus = DCM_E_DONE;
  }

  DBG_DCM_DSPINTERNAL_REPORTDTCSNAPSHOTRECORDIDENTIFICATION_SSVCH_EXIT(serviceProcessStatus,
    OpStatus,pMsgContext);

  return serviceProcessStatus;
}
#endif  /*  #if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x03 == STD_ON)  */

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x04 == STD_ON)
/* This is the UDS service 0x19-0x04 handler */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportDtcSnapshotRecordByDtcNumber_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSPINTERNAL_REPORTDTCSNAPSHOTRECORDBYDTCNUMBER_SSVCH_ENTRY(OpStatus, pMsgContext);
  if(OpStatus == DCM_INITIAL)
  {
    P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);

    /* Check that request length is Ok. */
    if ( pMsgContext->reqDataLen != 4U )
    {
      ClientContextPtr->AmountOfDataWrittenOnCurrentPage = 0U;
#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
      Dcm_ReadEventDataServicesContext.ReachedEndOfRequest = TRUE;
#endif
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
      serviceProcessStatus = DCM_E_DONE;
    }
#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
    else if ( (ClientContextPtr->ServiceID == DCM_DSP_SERVICE_0XAF) &&
              ( ((pMsgContext->resMaxDataLen - pMsgContext->resDataLen) < 3U) ||
                (pMsgContext->resMaxDataLen < pMsgContext->resDataLen) ) )
    {
      ClientContextPtr->AmountOfDataWrittenOnCurrentPage = 0U;
      Dcm_ReadEventDataServicesContext.ReachedEndOfRequest = TRUE;
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_RESPONSETOOLONG);
      serviceProcessStatus = DCM_E_DONE;
    }
#endif
    else
    {
      /* Keep the sub-service ID in the context */
      ClientContextPtr->SubServiceID = DCM_DSP_READDTCINFORMATION_0x04_ID;
      /* Pass kind of Data to retrieve */
      ClientContextPtr->DataKind = DCM_FACADE_FREEZE_FRAME_DATA;
      /* Obtain the DTC Format */
      ClientContextPtr->DTCFormat = DEM_DTC_FORMAT_UDS;
      /* Obtain the memory origin */
      ClientContextPtr->MemoryOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;

      /* fill initial bytes of response buffer with requested DTC mask */
      /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotRecordByDTCNumber.Response,1 */
      pMsgContext->resData[pMsgContext->resDataLen] =
          (uint8)(ClientContextPtr->DTCMask >> 16U);
      pMsgContext->resData[pMsgContext->resDataLen+1U] =
          (uint8)(ClientContextPtr->DTCMask >> 8U);
      pMsgContext->resData[pMsgContext->resDataLen+2U] =
          (uint8) ClientContextPtr->DTCMask;
#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
      Dcm_ReadEventDataServicesContext.HeaderData[0] =
          (uint8)(ClientContextPtr->DTCMask >> 16U);
      Dcm_ReadEventDataServicesContext.HeaderData[1] =
          (uint8)(ClientContextPtr->DTCMask >> 8U);
      Dcm_ReadEventDataServicesContext.HeaderData[2] =
          (uint8) ClientContextPtr->DTCMask;
      Dcm_ReadEventDataServicesContext.WriteHeaderDataBeforeRecordNumber = FALSE;
#endif /* #if (DCM_DSP_USE_SERVICE_0XAF == STD_ON) */
      pMsgContext->resDataLen += 3U;
      /* Initialize the counter of written bytes on the complete response. At this point:
       * SvcID + SSvcID + 3Bytes Mask */
      ClientContextPtr->AmountOfDataWrittenOnCurrentPage =
          pMsgContext->resDataLen + 1U;
      /* Initialize flag for reporting the write of the DTC status mask */
      ClientContextPtr->DTCStatusWrittenInResponseBuffer = FALSE;
      /* Initialize page buffering flag */
      ClientContextPtr->PageBufferingFlag = FALSE;
      /* Initialize the size of a single page*/
      ClientContextPtr->SizeOfOnePage = pMsgContext->resMaxDataLen + 1U;
      ClientContextPtr->FirstPageSent = FALSE;
    }
  }

  if (serviceProcessStatus != DCM_E_DONE)
  {
    serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoBySelect_Process(DemClientId, OpStatus, pMsgContext);
  }

  DBG_DCM_DSPINTERNAL_REPORTDTCSNAPSHOTRECORDBYDTCNUMBER_SSVCH_EXIT(serviceProcessStatus,
    OpStatus, pMsgContext);

  return serviceProcessStatus;
}
#endif /* #if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x04 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x06 == STD_ON)
/* This is the UDS service 0x19-0x06 handler */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportDtcExtendedDataRecordByDtcNumber_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSPINTERNAL_REPORTDTCEXTENDEDDATARECORDBYDTCNUMBER_SSVCH_ENTRY(OpStatus,pMsgContext);

  if(OpStatus == DCM_INITIAL)
  {
    P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);

    /* Check that request length is OK. */
    if ( pMsgContext->reqDataLen != 4U )
    {
      ClientContextPtr->AmountOfDataWrittenOnCurrentPage = 0U;
#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
      Dcm_ReadEventDataServicesContext.ReachedEndOfRequest = TRUE;
#endif
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
      serviceProcessStatus = DCM_E_DONE;
    }
#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
    else if ( (ClientContextPtr->ServiceID == DCM_DSP_SERVICE_0XAF) &&
              ( ((pMsgContext->resMaxDataLen - pMsgContext->resDataLen) < 3U) ||
                (pMsgContext->resMaxDataLen < pMsgContext->resDataLen) ) )
    {
      ClientContextPtr->AmountOfDataWrittenOnCurrentPage = 0U;
      Dcm_ReadEventDataServicesContext.ReachedEndOfRequest = TRUE;
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_RESPONSETOOLONG);
      serviceProcessStatus = DCM_E_DONE;
    }
#endif
    else
    {
      /* !LINKSTO Dcm.EB.UDSServices.reportDTCExtDataRecordByDTCNumber.SupportedRecordNumber.CollectExtendedDataRecord,1 */
      /* !LINKSTO Dcm.EB.UDSServices.reportGenericExtendedDataByDTCNumber.SupportedRecordNumber.CollectExtendedDataRecord,1 */
      /* Evaluate the requested ExtDataRecord */
      if ( /* Request out of regular range 0x01..0x0F */
           ( (ClientContextPtr->DataRecordNumber < 0x01U) ||
             (ClientContextPtr->DataRecordNumber > 0xF0U)
           ) && /* no request of ALL ExtDataRecords */
           (ClientContextPtr->DataRecordNumber != 0xFFU) )
      {
        /* !LINKSTO Dcm.EB.UDSServices.reportDTCExtDataRecordByDTCNumber.UnsupportedRecordNumber.NegativeResponse.NRC_ROOR,1*/
        /* !LINKSTO Dcm.EB.UDSServices.reportGenericExtendedDataByDTCNumber.UnsupportedRecordNumber.NegativeResponse.NRC_ROOR,1*/
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
        Dcm_ReadEventDataServicesContext.ReachedEndOfRequest = TRUE;
#endif
        serviceProcessStatus = DCM_E_DONE;
      }
      else
      {
        /* Keep the sub-service ID in the context */
        ClientContextPtr->SubServiceID = DCM_DSP_READDTCINFORMATION_0x06_ID;
        /* Pass kind of Data to retrieve */
        ClientContextPtr->DataKind = DCM_FACADE_EXTENDED_DATA_RECORDS;
        /* Obtain the DTC Format */
        ClientContextPtr->DTCFormat = DEM_DTC_FORMAT_UDS;
        /* Obtain the memory origin */
        ClientContextPtr->MemoryOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;

        /* fill initial bytes of response buffer with requested DTC mask */
        pMsgContext->resData[pMsgContext->resDataLen] =
            (uint8)(ClientContextPtr->DTCMask >> 16U);
        pMsgContext->resData[pMsgContext->resDataLen+1U] =
            (uint8)(ClientContextPtr->DTCMask >> 8U);
        pMsgContext->resData[pMsgContext->resDataLen+2U] =
            (uint8) ClientContextPtr->DTCMask;
        pMsgContext->resDataLen += 3U;

        /* Initialize the counter of written bytes on the complete response. At this point:
         * SvcID + SSvcID + 3Bytes Mask */
        ClientContextPtr->AmountOfDataWrittenOnCurrentPage =
            pMsgContext->resDataLen + 1U;
        /* Initialize flag for reporting the write of the DTC status mask */
        ClientContextPtr->DTCStatusWrittenInResponseBuffer = FALSE;
        /* Initialize page buffering flag */
        ClientContextPtr->PageBufferingFlag = FALSE;
        /* Initialize the size of a single page*/
        ClientContextPtr->SizeOfOnePage = pMsgContext->resMaxDataLen + 1U;
        ClientContextPtr->FirstPageSent = FALSE;
#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
        Dcm_ReadEventDataServicesContext.WriteHeaderDataBeforeRecordNumber = FALSE;
#endif /* #if (DCM_DSP_USE_SERVICE_0XAF == STD_ON) */
      }
    }
  }

  if (serviceProcessStatus != DCM_E_DONE)
  {
    serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoBySelect_Process(DemClientId, OpStatus, pMsgContext);
  }

  DBG_DCM_DSPINTERNAL_REPORTDTCEXTENDEDDATARECORDBYDTCNUMBER_SSVCH_EXIT(OpStatus, pMsgContext);

  return serviceProcessStatus;
}
#endif /* #if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x06 == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x07 == STD_ON)
/* This is the UDS service 0x19-0x07 handler */
/* !LINKSTO Dcm.Dsn.IB.ReadDTCInformation.ReportNumberOfDTCBySeverityMaskRecord.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDTCInformation_ReportNumberOfDTCBySeverityMaskRecord_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  const Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSP_READDTCINFORMATION_REPORTNUMBEROFDTCBYSEVERITYMASKRECORD_SSVCH_ENTRY(OpStatus, pMsgContext);

  /* Check if this is the initial call and perform setup. */
  if (OpStatus == DCM_INITIAL)
  {
    /* Check the length of the request */
    /* !LINKSTO Dcm.EB.ReadDTCInfo.reportNumberOfDTCBySeverityMaskRecord.MaxLenNRC,1 */
    if (pMsgContext->reqDataLen == 2U)
    {
      P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);

      ClientContextPtr->SubServiceID = DCM_DSP_READDTCINFORMATION_0x07_ID;
      /* !LINKSTO Dcm.ReadDTCInfo.reportNumberOfDTCBySeverityMaskRecord.Dem_SetDTCFilter.Parameters,1 */
      ClientContextPtr->DTCFormat = DEM_DTC_FORMAT_UDS;
      ClientContextPtr->MemoryOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
      ClientContextPtr->FilterForFaultDetectionCounter = FALSE;
      ClientContextPtr->RequestedSeverityMask  = pMsgContext->reqData[0];
      /* Initialize the size of a single page*/
      ClientContextPtr->SizeOfOnePage = pMsgContext->resMaxDataLen + 1U;
      ClientContextPtr->AmountOfDataWrittenOnCurrentPage = 2U;
      ClientContextPtr->RequestedStatusMask = pMsgContext->reqData[1];
      ClientContextPtr->RequestedStatusBits = pMsgContext->reqData[1];
      ClientContextPtr->FirstPageSent = FALSE;
      /* Data Supplier Switches */
      ClientContextPtr->MemoryOriginState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->AvailabilityMaskState = DCM_DATA_SUPPLIER_REQUIRED;
      ClientContextPtr->DtcFormatIdState = DCM_DATA_SUPPLIER_REQUIRED;
      ClientContextPtr->FilteredDtcsCountState = DCM_DATA_SUPPLIER_REQUIRED;
      ClientContextPtr->FilteredDtcsState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->PageBufferingFlag = FALSE;
    }
    else
    {
      /* request data length is invalid */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
      serviceProcessStatus = DCM_E_DONE;
    }
  }

  /* Continue if no negative response was set. */
  if (serviceProcessStatus != DCM_E_DONE)
  {
    serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoByFilter_Process(DemClientId, OpStatus, pMsgContext);
  }

  DBG_DCM_DSP_READDTCINFORMATION_REPORTNUMBEROFDTCBYSEVERITYMASKRECORD_SSVCH_EXIT(serviceProcessStatus, OpStatus,
    pMsgContext);

  return serviceProcessStatus;
}
#endif  /* #if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x07 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08 == STD_ON)
/* This is the UDS service 0x19-0x08 handler */
/* !LINKSTO Dcm.Dsn.IB.ReadDTCInformation.ReportDTCBySeverityMaskRecord.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDTCInformation_ReportDTCBySeverityMaskRecord_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  const Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSP_READDTCINFORMATION_REPORTDTCBYSEVERITYMASKRECORD_SSVCH_ENTRY(OpStatus, pMsgContext);

  /* Check if this is the initial call and perform setup. */
  if (OpStatus == DCM_INITIAL)
  {
    /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCBySeverityMaskRecord.MaxLenNRC,1 */
    /* Check the length of the request. */
    if (pMsgContext->reqDataLen == 2U)
    {
      P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);

      ClientContextPtr->SubServiceID = DCM_DSP_READDTCINFORMATION_0x08_ID;
      /* Explicit and implicit requested parameter */
      /* !LINKSTO SWS_Dcm_00380,1 */
      ClientContextPtr->DTCFormat = DEM_DTC_FORMAT_UDS;
      ClientContextPtr->MemoryOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
      ClientContextPtr->RequestedSeverityMask  = pMsgContext->reqData[0];
      ClientContextPtr->FilterForFaultDetectionCounter = FALSE;
      /* Initialize the size of a single page*/
      ClientContextPtr->SizeOfOnePage = pMsgContext->resMaxDataLen + 1U;
      ClientContextPtr->AmountOfDataWrittenOnCurrentPage = 2U;
      ClientContextPtr->RequestedStatusMask = pMsgContext->reqData[1];
      ClientContextPtr->RequestedStatusBits = pMsgContext->reqData[1];
      ClientContextPtr->FirstPageSent = FALSE;
      /* Data Supplier Switches */
      ClientContextPtr->MemoryOriginState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->AvailabilityMaskState = DCM_DATA_SUPPLIER_REQUIRED;
      ClientContextPtr->DtcFormatIdState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->FilteredDtcsCountState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->FilteredDtcsState = DCM_DATA_SUPPLIER_REQUIRED;
      ClientContextPtr->PageBufferingFlag = FALSE;
    }
    else
    {
      /* request data length is invalid */
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
      serviceProcessStatus = DCM_E_DONE;
    }
  }

  /* Continue if no negative response was set. */
  if (serviceProcessStatus != DCM_E_DONE)
  {
    serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoByFilter_Process(DemClientId, OpStatus, pMsgContext);
  }

  DBG_DCM_DSP_READDTCINFORMATION_REPORTDTCBYSEVERITYMASKRECORD_SSVCH_EXIT(serviceProcessStatus,
    OpStatus, pMsgContext);
  return serviceProcessStatus;
}
#endif  /* #if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON)
/* This is the UDS service 0x19-0x09 handler */
/* !LINKSTO Dcm.Dsn.IB.ReadDTCInformation.ReportSeverityInformationOfDTC.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDTCInformation_ReportSeverityInformationOfDTC_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  const Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSP_READDTCINFORMATION_REPORTSEVERITYINFORMATIONOFDTC_SSVCH_ENTRY(OpStatus, pMsgContext);

  /* Check if this is the initial call and perform setup. */
  if(OpStatus == DCM_INITIAL)
  {
    P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);
    /* Check that request length is Ok. */
    /* !LINKSTO Dcm.EB.ReadDTCInfo.reportSeverityInformationOfDTC.MaxLenNRC,1 */
    if ( pMsgContext->reqDataLen == 3U )
    {
      /* Keep the sub-service ID in the context */
      ClientContextPtr->SubServiceID = DCM_DSP_READDTCINFORMATION_0x09_ID;
      /* Obtain the memory origin */
      ClientContextPtr->MemoryOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
      /* Set DTC Format */
      ClientContextPtr->DTCFormat = DEM_DTC_FORMAT_UDS;
      /* fill initial bytes of response buffer with requested DTC mask */
      /* Initialize the counter of written bytes on the complete response. At this point:
      * SvcID + SSvcID */
      ClientContextPtr->AmountOfDataWrittenOnCurrentPage =
          pMsgContext->resDataLen + 1U;
      /* Initialize flag for reporting the write of the DTC status mask */
      ClientContextPtr->DTCStatusWrittenInResponseBuffer = FALSE;
      /* Initialize page buffering flag */
      ClientContextPtr->PageBufferingFlag = FALSE;
      /* Initialize the size of a single page*/
      ClientContextPtr->SizeOfOnePage = pMsgContext->resMaxDataLen + 1U;
      ClientContextPtr->FirstPageSent = FALSE;
    }
    else
    {
      ClientContextPtr->AmountOfDataWrittenOnCurrentPage = 0U;
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
      serviceProcessStatus = DCM_E_DONE;
    }
  }

  /* Continue if no negative response was set. */
  if(serviceProcessStatus != DCM_E_DONE)
  {
    serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCSeverityInfoBySelect_Process(DemClientId, OpStatus, pMsgContext);
  }

  DBG_DCM_DSP_READDTCINFORMATION_REPORTSEVERITYINFORMATIONOFDTC_SSVCH_EXIT(serviceProcessStatus,
    OpStatus, pMsgContext);

  return serviceProcessStatus;
}
#endif /* #if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON) */

 #if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0A == STD_ON)
/* This is the UDS service 0x19-0x0A handler */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportSupportedDtcs_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSPINTERNAL_REPORTSUPPORTEDDTCS_SSVCH_ENTRY(OpStatus, pMsgContext);
  /* Check the length of the request */
  if (pMsgContext->reqDataLen == 0U)
  {
    if (OpStatus == DCM_INITIAL)
    {
      P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);

      ClientContextPtr->SubServiceID = DCM_DSP_READDTCINFORMATION_0x0A_ID;
      /* Explicit and implicit requested parameter */
      ClientContextPtr->DTCFormat = DEM_DTC_FORMAT_UDS;
      ClientContextPtr->MemoryOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
      ClientContextPtr->FilterForFaultDetectionCounter = FALSE;
      ClientContextPtr->RequestedStatusMask = DCM_REPORT_ALLSUPPORTEDDTC;
      ClientContextPtr->RequestedStatusBits = 0xFFU;
      /* Initialize page buffering flag */
      ClientContextPtr->PageBufferingFlag = FALSE;
      /* Initialize the size of a single page*/
      ClientContextPtr->SizeOfOnePage = pMsgContext->resMaxDataLen + 1U;
      ClientContextPtr->FirstPageSent = FALSE;
      ClientContextPtr->AmountOfDataWrittenOnCurrentPage = 2U;
      /* Data Supplier Switches */
      ClientContextPtr->MemoryOriginState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->AvailabilityMaskState = DCM_DATA_SUPPLIER_REQUIRED;
      ClientContextPtr->DtcFormatIdState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->FilteredDtcsCountState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->FilteredDtcsState = DCM_DATA_SUPPLIER_REQUIRED;
    }
    serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoByFilter_Process(DemClientId, OpStatus, pMsgContext);
  }
  /* request data length is invalid */
  else
  {
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    serviceProcessStatus = DCM_E_DONE;
  }

  DBG_DCM_DSPINTERNAL_REPORTSUPPORTEDDTCS_SSVCH_EXIT(serviceProcessStatus, OpStatus, pMsgContext);

  return serviceProcessStatus;
}
#endif /*#if DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0A */

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x13 == STD_ON)
/* This is the UDS service 0x19-0x13 handler */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportEmissionsRelatedOBDDTCByStatusMask_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSPINTERNAL_REPORTEMISSIONSRELATEDOBDDTCBYSTATUSMASK_SSVCH_ENTRY(OpStatus, pMsgContext);
  /* Check the length of the request */
  if (pMsgContext->reqDataLen == 1U)
  {
    if (OpStatus == DCM_INITIAL)
    {
      P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);

      ClientContextPtr->SubServiceID = DCM_DSP_READDTCINFORMATION_0x13_ID;
      /* Explicit and implicit requested parameter */
      ClientContextPtr->DTCFormat = DEM_DTC_FORMAT_UDS;
      ClientContextPtr->MemoryOrigin = DEM_DTC_ORIGIN_OBD_RELEVANT_MEMORY;
      ClientContextPtr->FilterForFaultDetectionCounter = FALSE;
      ClientContextPtr->RequestedStatusMask = pMsgContext->reqData[0U];
      ClientContextPtr->RequestedStatusBits = pMsgContext->reqData[0U];
      /* Initialize page buffering flag */
      ClientContextPtr->PageBufferingFlag = FALSE;
      /* Initialize the size of a single page*/
      ClientContextPtr->SizeOfOnePage = pMsgContext->resMaxDataLen + 1U;
      ClientContextPtr->FirstPageSent = FALSE;
      ClientContextPtr->AmountOfDataWrittenOnCurrentPage = 2U;
      /* Data Supplier Switches */
      ClientContextPtr->MemoryOriginState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->AvailabilityMaskState = DCM_DATA_SUPPLIER_REQUIRED;
      ClientContextPtr->DtcFormatIdState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->FilteredDtcsCountState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->FilteredDtcsState = DCM_DATA_SUPPLIER_REQUIRED;
    }
    serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoByFilter_Process(DemClientId, OpStatus, pMsgContext);
  }
  /* request data length is invalid */
  else
  {
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    serviceProcessStatus = DCM_E_DONE;
  }

  DBG_DCM_DSPINTERNAL_REPORTEMISSIONSRELATEDOBDDTCBYSTATUSMASK_SSVCH_EXIT(serviceProcessStatus,
    OpStatus, pMsgContext);

  return serviceProcessStatus;
}
#endif /*#if DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x13 */

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x14 == STD_ON)
/* This is the UDS service 0x19-0x14 handler */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportDTCFaultDetectionCounter_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSPINTERNAL_REPORTDTCFAULTDETECTIONCOUNTER_SSVCH_ENTRY(OpStatus, pMsgContext);
  /* Check the length of the request */
  if (pMsgContext->reqDataLen == 0U)
  {
    if (OpStatus == DCM_INITIAL)
    {
      P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);

      ClientContextPtr->SubServiceID = DCM_DSP_READDTCINFORMATION_0x14_ID;
      /* Explicit and implicit requested parameter */
      ClientContextPtr->DTCFormat = DEM_DTC_FORMAT_UDS;
      ClientContextPtr->MemoryOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
      ClientContextPtr->FilterForFaultDetectionCounter = TRUE;
      ClientContextPtr->RequestedStatusMask = DCM_REPORT_ALLSUPPORTEDDTC;
      ClientContextPtr->RequestedStatusBits = 0xFFU;
      /* Initialize page buffering flag */
      ClientContextPtr->PageBufferingFlag = FALSE;
      /* Initialize the size of a single page*/
      ClientContextPtr->SizeOfOnePage = pMsgContext->resMaxDataLen + 1U;
      ClientContextPtr->FirstPageSent = FALSE;
      ClientContextPtr->AmountOfDataWrittenOnCurrentPage = 2U;

      /* Data Supplier Switches */
      ClientContextPtr->MemoryOriginState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->AvailabilityMaskState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->DtcFormatIdState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->FilteredDtcsCountState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->FilteredDtcsState = DCM_DATA_SUPPLIER_REQUIRED;
    }
    serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoByFilter_Process(DemClientId, OpStatus, pMsgContext);
  }
  /* request data length is invalid */
  else
  {
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    serviceProcessStatus = DCM_E_DONE;
  }

  DBG_DCM_DSPINTERNAL_REPORTDTCFAULTDETECTIONCOUNTER_SSVCH_EXIT(serviceProcessStatus,
    OpStatus, pMsgContext);

  return serviceProcessStatus;
}
#endif /*#if DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x14 */

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x15 == STD_ON)
/* This is the UDS service 0x19-0x15 handler */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportDTCWithPermanentStatus_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSPINTERNAL_REPORTDTCWITHPERMANENTSTATUS_SSVCH_ENTRY(OpStatus, pMsgContext);
  /* Check the length of the request */
  if (pMsgContext->reqDataLen == 0U)
  {
    if (OpStatus == DCM_INITIAL)
    {
      P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);

      ClientContextPtr->SubServiceID = DCM_DSP_READDTCINFORMATION_0x15_ID;
      /* Explicit and implicit requested parameter */
      ClientContextPtr->DTCFormat = DEM_DTC_FORMAT_UDS;
      ClientContextPtr->MemoryOrigin = DEM_DTC_ORIGIN_PERMANENT_MEMORY;
      ClientContextPtr->FilterForFaultDetectionCounter = FALSE;
      ClientContextPtr->RequestedStatusMask = DCM_REPORT_ALLSUPPORTEDDTC;
      ClientContextPtr->RequestedStatusBits = 0xFFU;
      /* Initialize page buffering flag */
      ClientContextPtr->PageBufferingFlag = FALSE;
      /* Initialize the size of a single page*/
      ClientContextPtr->SizeOfOnePage = pMsgContext->resMaxDataLen + 1U;
      ClientContextPtr->FirstPageSent = FALSE;
      ClientContextPtr->AmountOfDataWrittenOnCurrentPage = 2U;
      /* Data Supplier Switches */
      ClientContextPtr->MemoryOriginState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->AvailabilityMaskState = DCM_DATA_SUPPLIER_REQUIRED;
      ClientContextPtr->DtcFormatIdState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->FilteredDtcsCountState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->FilteredDtcsState = DCM_DATA_SUPPLIER_REQUIRED;
    }
    serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoByFilter_Process(DemClientId, OpStatus, pMsgContext);
  }
  /* request data length is invalid */
  else
  {
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    serviceProcessStatus = DCM_E_DONE;
  }

  DBG_DCM_DSPINTERNAL_REPORTDTCWITHPERMANENTSTATUS_SSVCH_EXIT(serviceProcessStatus, OpStatus,
    pMsgContext);

  return serviceProcessStatus;
}
#endif /*#if DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x15 */

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x17 == STD_ON)
/* This is the UDS service 0x19-0x17 handler */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportUserDefMemoryDTCByStatusMask_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSPINTERNAL_REPORTUSERDEFMEMORYDTCBYSTATUSMASK_SSVCH_ENTRY(OpStatus, pMsgContext);
  if (OpStatus == DCM_INITIAL)
  {
    P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);

    /* Make some checks of the request: */

    /* Whole length of the request */
    if (pMsgContext->reqDataLen != 2U)
    {
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
      serviceProcessStatus = DCM_E_DONE;
    }
    else
    {
      /* Read out the user defined request */
      ClientContextPtr->RequestedStatusMask = pMsgContext->reqData[0U];
      ClientContextPtr->RequestedStatusBits = pMsgContext->reqData[0U];
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCByStatusMask.Dem_DTCOriginType.MemorySelection,1 */
      ClientContextPtr->MemoryOrigin = (pMsgContext->reqData[1U] + DCM_DTC_ORIGIN_USER_MEMORY_BASE);

        /* Read out the user defined request */
      ClientContextPtr->SubServiceID = DCM_DSP_READDTCINFORMATION_0x17_ID;
      ClientContextPtr->FilterForFaultDetectionCounter = FALSE;
      ClientContextPtr->DTCFormat = DEM_DTC_FORMAT_UDS;
      /* Initialize the size of a single page; Add one byte for SID to the resMaxDataLen */
      ClientContextPtr->SizeOfOnePage = pMsgContext->resMaxDataLen + 1U;
      ClientContextPtr->FirstPageSent = FALSE;
      /* Two bytes already written into page by upper layer: SID (by Dsd) and SSID (by SvcH) */
      ClientContextPtr->AmountOfDataWrittenOnCurrentPage = 2U;
      /* Data supplier switches */
      ClientContextPtr->MemoryOriginState = DCM_DATA_SUPPLIER_REQUIRED;
      ClientContextPtr->AvailabilityMaskState = DCM_DATA_SUPPLIER_REQUIRED;
      ClientContextPtr->DtcFormatIdState = DCM_DATA_SUPPLIER_NOT_REQUIRED;
      ClientContextPtr->FilteredDtcsState = DCM_DATA_SUPPLIER_REQUIRED;
    }
  }
  /* If no error, proceed with the execution */
  if (serviceProcessStatus != DCM_E_DONE)
  {
    serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoByFilter_Process(DemClientId, OpStatus, pMsgContext);
  }

  DBG_DCM_DSPINTERNAL_REPORTUSERDEFMEMORYDTCBYSTATUSMASK_SSVCH_EXIT(serviceProcessStatus,
    OpStatus, pMsgContext);

  return serviceProcessStatus;
}
#endif  /* #if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x17 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x18 == STD_ON)
/* This is the UDS service 0x19-0x18 handler */
FUNC(Std_ReturnType, DCM_CODE)
  Dcm_DspInternal_ReportUserDefMemoryDtcSnapshotRecordByDtcNumber_SSvcH(
    Dcm_OpStatusType OpStatus,
    P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSPINTERNAL_REPORTUSERDEFMEMORYDTCSNAPSHOTRECORDBYDTCNUMBER_SSVCH_ENTRY(OpStatus,
    pMsgContext);
  if(OpStatus == DCM_INITIAL)
  {
    P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);

    /* Check that request length is OK. */
    if ( pMsgContext->reqDataLen != 5U )
    {
      ClientContextPtr->AmountOfDataWrittenOnCurrentPage = 0U;
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
      serviceProcessStatus = DCM_E_DONE;
    }
    else
    {
      /* Read out the user defined request */
      ClientContextPtr->DTCMask = DCM_CONSTRUCT_DTC(pMsgContext->reqData[0U],
                                                     pMsgContext->reqData[1U],
                                                     pMsgContext->reqData[2U]);
      ClientContextPtr->DataRecordNumber = pMsgContext->reqData[3U];
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.Dem_DTCOriginType.MemorySelection,1 */
      ClientContextPtr->MemoryOrigin = (pMsgContext->reqData[4U] + DCM_DTC_ORIGIN_USER_MEMORY_BASE);

      /* Keep the sub-service ID in the context */
      ClientContextPtr->SubServiceID = DCM_DSP_READDTCINFORMATION_0x18_ID;
      /* Pass kind of Data to retrieve */
      ClientContextPtr->DataKind = DCM_FACADE_FREEZE_FRAME_DATA;
      /* Obtain the DTC Format */
      ClientContextPtr->DTCFormat = DEM_DTC_FORMAT_UDS;

      /* fill initial bytes of response buffer with requested MemorySelection and DTC mask */
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCSnapshotRecordByDTCNumber.Response,1 */
      pMsgContext->resData[pMsgContext->resDataLen] =
          (uint8)(ClientContextPtr->MemoryOrigin);
      pMsgContext->resData[pMsgContext->resDataLen+1U] =
          (uint8)(ClientContextPtr->DTCMask >> 16U);
      pMsgContext->resData[pMsgContext->resDataLen+2U] =
          (uint8)(ClientContextPtr->DTCMask >> 8U);
      pMsgContext->resData[pMsgContext->resDataLen+3U] =
          (uint8) ClientContextPtr->DTCMask;
      pMsgContext->resDataLen += 4U;
      /* Initialize the counter of written bytes on the complete response. At this point:
       * SvcID + SSvcID + MemOrigin + 3Bytes Mask */
      ClientContextPtr->AmountOfDataWrittenOnCurrentPage =
          pMsgContext->resDataLen + 1U;
      /* Initialize flag for reporting the write of the DTC status mask */
      ClientContextPtr->DTCStatusWrittenInResponseBuffer = FALSE;
      /* Initialize page buffering flag */
      ClientContextPtr->PageBufferingFlag = FALSE;
      /* Initialize the size of a single page */
      ClientContextPtr->SizeOfOnePage = pMsgContext->resMaxDataLen + 1U;
      ClientContextPtr->FirstPageSent = FALSE;
    }
  }

  if (serviceProcessStatus != DCM_E_DONE)
  {
    serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoBySelect_Process(DemClientId, OpStatus, pMsgContext);
  }

  DBG_DCM_DSPINTERNAL_REPORTUSERDEFMEMORYDTCSNAPSHOTRECORDBYDTCNUMBER_SSVCH_EXIT(
    serviceProcessStatus, OpStatus, pMsgContext);

  return serviceProcessStatus;
}
#endif /*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x18 == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x19 == STD_ON)
/* This is the UDS service 0x19-0x19 handler */
FUNC(Std_ReturnType, DCM_CODE)
  Dcm_DspInternal_ReportUserDefMemoryDtcExtendedDataRecordByDtcNumber_SSvcH(
    Dcm_OpStatusType OpStatus,
    P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSPINTERNAL_REPORTUSERDEFMEMORYDTCEXTENDEDDATARECORDBYDTCNUMBER_SSVCH_ENTRY(OpStatus,
    pMsgContext);
  if(OpStatus == DCM_INITIAL)
  {
    P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);

    /* Check that request length is OK. */
    if ( pMsgContext->reqDataLen != 5U )
    {
      ClientContextPtr->AmountOfDataWrittenOnCurrentPage = 0U;
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
      serviceProcessStatus = DCM_E_DONE;
    }
    else
    {
      /* Read out the user defined request */
      ClientContextPtr->DTCMask = DCM_CONSTRUCT_DTC(pMsgContext->reqData[0U],
                                                     pMsgContext->reqData[1U],
                                                     pMsgContext->reqData[2U]);
      ClientContextPtr->DataRecordNumber = pMsgContext->reqData[3U];
      /* !LINKSTO Dcm.ReadDTCInfo.reportUserDefMemoryDTCExtDataRecordByDTCNumber.Dem_DTCOriginType.MemorySelection,1 */
      ClientContextPtr->MemoryOrigin = (pMsgContext->reqData[4U] + DCM_DTC_ORIGIN_USER_MEMORY_BASE);
      /* !LINKSTO Dcm.EB.UDSServices.reportUserDefMemoryDTCExtDataRecordByDTCNumber.SupportedRecordNumber.CollectExtendedDataRecord,1 */
      if( /* Requested ExtDataRecord invalid? */
          ( /* Requested ExtDataRecord out of regular range? */
            (ClientContextPtr->DataRecordNumber < 0x01U) ||
            (ClientContextPtr->DataRecordNumber > 0xF0U)
          ) && /* No request of all ExtDataRecords? */
             (ClientContextPtr->DataRecordNumber != 0xFFU)
        )
      {
        /* !LINKSTO Dcm.EB.UDSServices.reportUserDefMemoryDTCExtDataRecordByDTCNumber.UnsupportedRecordNumber.NegativeResponse.NRC_ROOR,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_REQUESTOUTOFRANGE);
        serviceProcessStatus = DCM_E_DONE;
      }
      else
      {
        /* Keep the sub-service ID in the context */
        ClientContextPtr->SubServiceID = DCM_DSP_READDTCINFORMATION_0x19_ID;
        /* Pass kind of Data to retrieve */
        ClientContextPtr->DataKind = DCM_FACADE_EXTENDED_DATA_RECORDS;
        /* Obtain the DTC Format */
        ClientContextPtr->DTCFormat = DEM_DTC_FORMAT_UDS;

        /* fill initial bytes of response buffer with requested DTC mask */
        pMsgContext->resData[pMsgContext->resDataLen] =
            (uint8)(ClientContextPtr->MemoryOrigin);
        pMsgContext->resData[pMsgContext->resDataLen+1U] =
            (uint8)(ClientContextPtr->DTCMask >> 16U);
        pMsgContext->resData[pMsgContext->resDataLen+2U] =
            (uint8)(ClientContextPtr->DTCMask >> 8U);
        pMsgContext->resData[pMsgContext->resDataLen+3U] =
            (uint8) ClientContextPtr->DTCMask;
        pMsgContext->resDataLen += 4U;

        /* Initialize the counter of written bytes on the complete response. At this point:
         * SvcID + SSvcID + MemOrigin + 3Bytes Mask */
        ClientContextPtr->AmountOfDataWrittenOnCurrentPage =
            pMsgContext->resDataLen + 1U;
        /* Initialize flag for reporting the write of the DTC status mask */
        ClientContextPtr->DTCStatusWrittenInResponseBuffer = FALSE;
        /* Initialize page buffering flag */
        ClientContextPtr->PageBufferingFlag = FALSE;
        /* Initialize the size of a single page*/
        ClientContextPtr->SizeOfOnePage = pMsgContext->resMaxDataLen + 1U;
        ClientContextPtr->FirstPageSent = FALSE;
      }
    }
  }

  if (serviceProcessStatus != DCM_E_DONE)
  {
    serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoBySelect_Process(DemClientId, OpStatus, pMsgContext);
  }

  DBG_DCM_DSPINTERNAL_REPORTUSERDEFMEMORYDTCEXTENDEDDATARECORDBYDTCNUMBER_SSVCH_EXIT(
    serviceProcessStatus, OpStatus, pMsgContext);

  return serviceProcessStatus;
}
#endif /*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x19 == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0B == STD_ON)
/* This is the UDS service 0x19-0x0B handler */
/* !LINKSTO Dcm.Dsn.IB.ReadDTCInformation.ReportFirstTestFailedDTC.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDTCInformation_ReportFirstTestFailedDTC_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  Dcm_NegativeResponseCodeType Nrc = DCM_E_GENERALREJECT;

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSP_READDTCINFORMATION_REPORTFIRSTTESTFAILEDDTC_SSVCH_ENTRY(OpStatus, pMsgContext);
  /* Whole length of the request */
  if (pMsgContext->reqDataLen != 0U)
  {
    Nrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    Dcm_ExternalSetNegResponse(pMsgContext, Nrc);
    ServiceProcessStatus = DCM_E_DONE;
  }
  /* !LINKSTO Dcm.EB.reportFirstTestFailedDTC.MaxLenNRC,1 */
  else
  {
    /* Call the DemFacade helper function */
    ServiceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoByOccurrence_Process(DemClientId,
                                                                            DEM_FIRST_FAILED_DTC,
                                                                            OpStatus,
                                                                            pMsgContext->resData,
                                                                            &(pMsgContext->resDataLen),
                                                                            &Nrc);
    /* If the request failed */
    if (ServiceProcessStatus == DCM_E_NOT_OK)
    {
      /* Set negative response code */
      Dcm_ExternalSetNegResponse(pMsgContext, Nrc);
      ServiceProcessStatus = DCM_E_DONE;
    }

  }
  DBG_DCM_DSP_READDTCINFORMATION_REPORTFIRSTTESTFAILEDDTC_SSVCH_EXIT(OpStatus, pMsgContext);
  return ServiceProcessStatus;
}
#endif /*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0B == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0C == STD_ON)
/* This is the UDS service 0x19-0x0C handler */
/* !LINKSTO Dcm.Dsn.IB.ReadDTCInformation.ReportFirstConfirmedDTC.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDTCInformation_ReportFirstConfirmedDTC_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  Dcm_NegativeResponseCodeType Nrc = DCM_E_GENERALREJECT;

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSP_READDTCINFORMATION_REPORTFIRSTCONFIRMEDDTC_SSVCH_ENTRY(OpStatus, pMsgContext);
  /* Whole length of the request */
  if (pMsgContext->reqDataLen != 0U)
  {
    Nrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    Dcm_ExternalSetNegResponse(pMsgContext, Nrc);
    ServiceProcessStatus = DCM_E_DONE;
  }
  /* !LINKSTO Dcm.EB.reportFirstConfirmedDTC.MaxLenNRC,1 */
  else
  {
    /* Call the DemFacade helper function */
    ServiceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoByOccurrence_Process(DemClientId,
                                                                            DEM_FIRST_DET_CONFIRMED_DTC,
                                                                            OpStatus,
                                                                            pMsgContext->resData,
                                                                            &(pMsgContext->resDataLen),
                                                                            &Nrc);
    /* If the request failed */
    if (ServiceProcessStatus == DCM_E_NOT_OK)
    {
      /* Set negative response code */
      Dcm_ExternalSetNegResponse(pMsgContext, Nrc);
      ServiceProcessStatus = DCM_E_DONE;
    }

  }
  DBG_DCM_DSP_READDTCINFORMATION_REPORTFIRSTCONFIRMEDDTC_SSVCH_EXIT(OpStatus, pMsgContext);
  return ServiceProcessStatus;
}
#endif /*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0C == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0D == STD_ON)
/* This is the UDS service 0x19-0x0D handler */
/* !LINKSTO Dcm.Dsn.IB.ReadDTCInformation.ReportMostRecentTestFailedDTC.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDTCInformation_ReportMostRecentTestFailedDTC_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  Dcm_NegativeResponseCodeType Nrc = DCM_E_GENERALREJECT;

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSP_READDTCINFORMATION_REPORTMOSTRECENTTESTFAILEDDTC_SSVCH_ENTRY(OpStatus, pMsgContext);
  /* Whole length of the request */
  if (pMsgContext->reqDataLen != 0U)
  {
    Nrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    Dcm_ExternalSetNegResponse(pMsgContext, Nrc);
    ServiceProcessStatus = DCM_E_DONE;
  }
  /* !LINKSTO Dcm.EB.reportMostRecentTestFailedDTC.MaxLenNRC,1 */
  else
  {
    /* Call the DemFacade helper function */
    ServiceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoByOccurrence_Process(DemClientId,
                                                                            DEM_MOST_RECENT_FAILED_DTC,
                                                                            OpStatus,
                                                                            pMsgContext->resData,
                                                                            &(pMsgContext->resDataLen),
                                                                            &Nrc);
    /* If the request failed */
    if (ServiceProcessStatus == DCM_E_NOT_OK)
    {
      /* Set negative response code */
      Dcm_ExternalSetNegResponse(pMsgContext, Nrc);
      ServiceProcessStatus = DCM_E_DONE;
    }

  }
  DBG_DCM_DSP_READDTCINFORMATION_REPORTMOSTRECENTTESTFAILEDDTC_SSVCH_EXIT(OpStatus, pMsgContext);
  return ServiceProcessStatus;
}
#endif /*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0D == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0E == STD_ON)
/* This is the UDS service 0x19-0x0E handler */
/* !LINKSTO Dcm.Dsn.IB.ReadDTCInformation.ReportMostRecentConfirmedDTC.SSvcH,1 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDTCInformation_ReportMostRecentConfirmedDTC_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType ServiceProcessStatus = DCM_E_ABORT;
  Dcm_NegativeResponseCodeType Nrc = DCM_E_GENERALREJECT;

  Dcm_ProtocolIndexType ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(ProtocolIndex);

  DBG_DCM_DSP_READDTCINFORMATION_REPORTMOSTRECENTCONFIRMEDDTC_SSVCH_ENTRY(OpStatus, pMsgContext);
  /* Whole length of the request */
  if (pMsgContext->reqDataLen != 0U)
  {
    Nrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
    Dcm_ExternalSetNegResponse(pMsgContext, Nrc);
    ServiceProcessStatus = DCM_E_DONE;
  }
  /* !LINKSTO Dcm.EB.reportMostRecentConfirmedDTC.MaxLenNRC,1 */
  else
  {
    /* Call the DemFacade helper function */
    ServiceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoByOccurrence_Process(DemClientId,
                                                                            DEM_MOST_REC_DET_CONFIRMED_DTC,
                                                                            OpStatus,
                                                                            pMsgContext->resData,
                                                                            &(pMsgContext->resDataLen),
                                                                            &Nrc);
    /* If the request failed */
    if (ServiceProcessStatus == DCM_E_NOT_OK)
    {
      /* Set negative response code */
      Dcm_ExternalSetNegResponse(pMsgContext, Nrc);
      ServiceProcessStatus = DCM_E_DONE;
    }

  }
  DBG_DCM_DSP_READDTCINFORMATION_REPORTMOSTRECENTCONFIRMEDDTC_SSVCH_EXIT(OpStatus, pMsgContext);
  return ServiceProcessStatus;
}
#endif /*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0E == STD_ON)*/

/*==================[internal function definitions]==============================================*/

#if (DCM_DSP_USE_SERVICE_0X19 == STD_ON)
/* This will be called internally for accessing the DEM via UDS service 0x19.
 * This function is used as dispatcher for the sub-services.
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReadDTCInformation_SvcStart(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
#if ( (DCM_NUM_PROTOCOL > 1U) || (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON) )
  /* get protocol instance index */
  /* This service can be invoked also from ROE, so ROE connection needs to be taken into account */
  Dcm_ProtocolIndexType instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
#endif /* #if ( (DCM_NUM_PROTOCOL > 1U) || (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) ) */

  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  /* get the context information current instance */
  P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);

#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
  /* Get the pointer to filled length in response buffer */
  P2VAR(Dcm_BufferSizeType, AUTOMATIC, DCM_VAR) FilledLengthPtr =
    &(curHsmInfo->TxBuffer.FilledLength);
#endif/* #if DCM_PAGEDBUFFER_ENABLED */

#if ((DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON))
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(instIdx);
  P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);
#endif /*#if (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) */

  /* No check if sub-function is requested,
   * already been checked in Dcm_DsdInternal_DispatchSvcReq */

  /* copy subfunction value to response buffer */
  pMsgContext->resData[0U] = curHsmInfo->subServiceId;
  pMsgContext->resDataLen = 1U;

#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)
  /* buffer is now filled with the subfunction ID;
     So buffer fill length is now 2 (response ID was already filled) */
  *FilledLengthPtr = 2U;
#endif
  /* retrieve sub-service handler */
  Dcm_ReadDTCInformation_SubServiceHandler =
                                          Dcm_Dsp_ExtractSubServiceHandler(DCM_INITIAL, pMsgContext);
  /* ------------------------ setup of service context --------------------- */
#if ((DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON))
  /* Construct the DTC from the request */
  ClientContextPtr->ServiceID = DCM_DSP_SERVICE_0X19;
  /* Because of sub-services 0x04 and 0x06 also used for service 0xAF
   * read the requested DTC already in the SvcH */
  if ( ( (curHsmInfo->subServiceId == DCM_DSP_READDTCINFORMATION_0x04_ID) ||
         (curHsmInfo->subServiceId == DCM_DSP_READDTCINFORMATION_0x06_ID) ||
         (curHsmInfo->subServiceId == DCM_DSP_READDTCINFORMATION_0x09_ID)
       ) && (pMsgContext->reqDataLen >= 3U) )
  {
    ClientContextPtr->DTCMask = DCM_CONSTRUCT_DTC(pMsgContext->reqData[0U],
                                                   pMsgContext->reqData[1U],
                                                   pMsgContext->reqData[2U]);
    ClientContextPtr->DataRecordNumber = pMsgContext->reqData[3U];
  }
#endif /* #if ((DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) || (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON)) */
  /* ------------------------ end of setup of service context --------------------- */
  serviceProcessStatus = Dcm_DspInternal_ReadDTCInformation_SvcCont(DCM_INITIAL, pMsgContext);

  return serviceProcessStatus;
}
#endif /* #if (DCM_DSP_USE_SERVICE_0X19 == STD_ON) */

#if (DCM_DSP_USE_SERVICE_0XAF == STD_ON)
/* This will be called internally for initializing the service context.
 */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReadGenericInformation_SvcStart(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType SetStoredDTCFilterResult = E_NOT_OK;
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
  uint32 CurrentDTC;

  Dcm_ProtocolIndexType instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
      DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
      (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
  Dcm_DemClientIdType DemClientId = Dcm_Dsl_GetDemClientId(instIdx);
  P2VAR(Dcm_DemClientContextType, AUTOMATIC, DCM_VAR) ClientContextPtr = Dcm_Dsp_DemFacade_GetClient(DemClientId);

  /* Check the length of the request */
  /* !LINKSTO Dcm.EB.ReadGenericInformation.NRC0x13,1 */
  if (pMsgContext->reqDataLen == 5U)
  {
    /* get the context information current instance */
    P2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);

    /* copy sub-function value to response buffer */
    pMsgContext->resData[pMsgContext->resDataLen] = curHsmInfo->subServiceId;
    pMsgContext->resDataLen++;
    /* retrieve sub-service handler */
    Dcm_ReadDTCInformation_SubServiceHandler =
                                            Dcm_Dsp_ExtractSubServiceHandler(DCM_INITIAL, pMsgContext);
    /* ------------------------ setup of service context --------------------- */
    CurrentDTC = DCM_CONSTRUCT_DTC(pMsgContext->reqData[0U],
                                   pMsgContext->reqData[1U],
                                   pMsgContext->reqData[2U]);
#if (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON)
    ClientContextPtr->ServiceID = DCM_DSP_SERVICE_0XAF;
    ClientContextPtr->DTCMask = CurrentDTC;
    ClientContextPtr->DataRecordNumber = pMsgContext->reqData[3U];
#endif /* #if (DCM_SUBSERVICES_EDR_AND_FF_GROUP == STD_ON) */
    /* !LINKSTO Dcm.ReadGenericInformation.RequestAllDtcs,1 */
    if(CurrentDTC == DCM_REQUEST_ALL_DTCS)
    {
      Dcm_ReadEventDataServicesContext.RequestAllDTCs = TRUE;
      Dcm_ReadEventDataServicesContext.ReachedEndOfRequest = FALSE;
      Dcm_ReadEventDataServicesContext.MemoryOrigin = DEM_DTC_ORIGIN_PRIMARY_MEMORY;
      if(curHsmInfo->subServiceId == DCM_DSP_READDTCINFORMATION_0x06_ID)
      {
        Dcm_ReadEventDataServicesContext.DataKind = DCM_FACADE_EXTENDED_DATA_RECORDS;
      }
      else
      {
        Dcm_ReadEventDataServicesContext.DataKind = DCM_FACADE_FREEZE_FRAME_DATA;
      }
      /* !LINKSTO Dcm.ReadGenericInformation.DataOriginDataKindInitialization,2 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.InitialDemAPIAllDTCs,1 */
      /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.InitialDemAPIAllDTCs,1 */
      /* !LINKSTO Dcm.ReadGenericInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
      SetStoredDTCFilterResult = Dem_DcmSetStoredDTCFilter(DemClientId,
                                   Dcm_ReadEventDataServicesContext.MemoryOrigin,
                                   Dcm_ReadEventDataServicesContext.DataKind);
      if (SetStoredDTCFilterResult != E_OK)
      {
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SetStoredDTCFilter.E_NOT_OK.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SetStoredDTCFilter.E_NOT_OK.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SetStoredDTCFilter.UnsupportedError.NRC,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SetStoredDTCFilter.UnsupportedError.NRC,1 */
        Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);

        serviceProcessStatus = DCM_E_NOT_OK;
        Dcm_ReadEventDataServicesContext.ReachedEndOfRequest = TRUE;

        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SetStoredDTCFilter.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SetStoredDTCFilter.E_NOT_OK.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericExtendedDataByDTCNumber.SetStoredDTCFilter.UnsupportedError.DET,1 */
        /* !LINKSTO Dcm.EB.ReadGenericInfo.reportGenericSnapshotByDTCNumber.SetStoredDTCFilter.UnsupportedError.DET,1 */
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        DCM_DET_REPORT_ERROR(DCM_SVCID_READDTCINFORMATION, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* if (DCM_DEV_ERROR_DETECT == STD_ON) */
      }
    }
    else
    {
      Dcm_ReadEventDataServicesContext.RequestAllDTCs = FALSE;
      Dcm_ReadEventDataServicesContext.ReachedEndOfRequest = FALSE;
      serviceProcessStatus = DCM_E_OK;
    }
    ClientContextPtr->CurrentDTCProcessingReady = TRUE;
    Dcm_ReadEventDataServicesContext.AtLeastOneDTCExists = FALSE;
    Dcm_ReadEventDataServicesContext.AmountOfDTCsProcessed = 0U;
    /* ------------------------ end of setup of service context --------------------- */
  }
  else
  {
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT);
    serviceProcessStatus = DCM_E_NOT_OK;
  }
  return serviceProcessStatus;
}
#endif /* #if(DCM_DSP_USE_SERVICE_0XAF == STD_ON) */

STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReadDTCInformation_SvcCont(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  /* Call sub-service handler. */
  serviceProcessStatus = Dcm_ReadDTCInformation_SubServiceHandler(OpStatus, pMsgContext);

  /* If sub-service handler returns E_OK, it must be converted to DCM_E_DONE. */
  if (serviceProcessStatus == E_OK)
  {
      serviceProcessStatus = DCM_E_DONE;
  }

  return serviceProcessStatus;
}

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x03 == STD_ON)
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportDtcSnapshotRecordIdent_Process(
  Dcm_DemClientIdType DemClientId,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;
#if (DCM_NUM_PROTOCOL > 1U)
  uint8 instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                    DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
                    (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
#endif
  /* Get the current context info */
  CONSTP2VAR(Dcm_HsmInfoType, AUTOMATIC, DCM_VAR) curHsmInfo = &DCM_HSM_INFO(instIdx);
  Std_ReturnType resultGetNextRecord = 0U;
  /* cache data pointer */
  Dcm_MsgType DataPtr = pMsgContext->resData;
  uint32 DataIdx = pMsgContext->resDataLen;
  uint32 reqDTC = 0U;
  uint8 dtcSnapShotRecordNum = 0U;


  /* loop until no further freeze frame record is returned or response buffer becomes too
   * small */
  do
  {
    /* check if buffer size is sufficient */
    if ((pMsgContext->resDataLen + (Dcm_MsgLenType)4U) <= pMsgContext->resMaxDataLen)
    {
      /* gather DTC record informations */
      /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotIdentification.SetFreezeFrameRecordFilter.E_OK,1 */
      /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
      resultGetNextRecord = Dem_GetNextFilteredRecord(DemClientId, &reqDTC, &dtcSnapShotRecordNum);
      if (resultGetNextRecord == E_OK)
      {
        /* copy DTC value to diagnostic response data */
        /* append 4 bytes to the response buffer */
        DataPtr[DataIdx] = (uint8)(reqDTC >> 16U);
        DataIdx++;
        DataPtr[DataIdx] = (uint8)(reqDTC >> 8U);
        DataIdx++;
        DataPtr[DataIdx] = (uint8)(reqDTC);
        DataIdx++;
        DataPtr[DataIdx] = (Dcm_MsgItemType)dtcSnapShotRecordNum;
        DataIdx++;

        /* update response length */
        pMsgContext->resDataLen += (Dcm_MsgLenType)DCM_DTC_SNAPSHOT_SNGL_RCD_LEN;
        /* update the remaining DTC count to be sent as response */
        DCM_READ_DTC_INFO_CONTEXT(instIdx).DTCCount--;
      }
    }
    else
    {
      Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
    }
  }
  while ((resultGetNextRecord == E_OK) &&
         (DCM_READ_DTC_INFO_CONTEXT(instIdx).DTCCount > 0)
          && (curHsmInfo->negativeResponseCode == DCM_E_OK));

  if (resultGetNextRecord == DEM_PENDING)
  {
    /* !LINKSTO Dcm.ReadDTCInfo.reportDTCSnapshotIdentification.GetNextFilteredRecord.DEM_PENDING,1 */
    serviceProcessStatus = DCM_E_PENDING;
  }
  else if((E_OK == resultGetNextRecord) || (DEM_NO_SUCH_ELEMENT == resultGetNextRecord))
  {
    /* !LINKSTO SWS_Dcm_01237,1 */
    /* !LINKSTO SWS_Dcm_01238,1 */
    serviceProcessStatus = DCM_E_DONE;
  }
  else
  {
    /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotIdentification.GetNextFilteredRecord.UnsupportedError.NRC,1 */
    /* !LINKSTO Dcm.EB.ReadDTCInfo.reportDTCSnapshotIdentification.GetNextFilteredRecord.UnsupportedError.DET,1 */
    Dcm_ExternalSetNegResponse(pMsgContext, DCM_E_GENERALREJECT);
    serviceProcessStatus = DCM_E_DONE;
#if (DCM_DEV_ERROR_DETECT == STD_ON)
        DCM_DET_REPORT_ERROR(DCM_SVCID_READDTCINFORMATION, DCM_E_INTERFACE_RETURN_VALUE);
#endif /* #if (DCM_DEV_ERROR_DETECT == STD_ON) */
  }

  return serviceProcessStatus;
}
#endif /* #if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x03 == STD_ON) */

#if (DCM_PAGEDBUFFER_ENABLED == STD_ON)

#if (DCM_DSP_USE_SERVICE_0X19 == STD_ON)
STATIC FUNC(void, DCM_CODE) Dcm_DspInternal_UpdateResponseBuffer(
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext)
{
  /* length of data already stored in buffer */
  Dcm_BufferSizeType curTxBufferLen;
#if (DCM_NUM_PROTOCOL > 1U)
  uint8 ProtocolIndex = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                          DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
                          (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
#endif

  /* ENTER critical section */
  SchM_Enter_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  /* get length of data already stored in buffer */
  curTxBufferLen = DCM_HSM_INFO(ProtocolIndex).TxBuffer.FilledLength;

  /* LEAVE critical section */
  SchM_Exit_Dcm_SCHM_DCM_EXCLUSIVE_AREA_0();

  /* Update the response data buffer with PagedBufPtr */
  pMsgContext->resData = &DCM_HSM_INFO(ProtocolIndex).TxBuffer.BufferPtr[curTxBufferLen];

  /* Update the response data buffer size */
  pMsgContext->resMaxDataLen = (Dcm_MsgLenType)(DCM_HSM_INFO(ProtocolIndex).
                                  TxBuffer.Size - curTxBufferLen);

}
#endif /* #if (DCM_DSP_USE_SERVICE_0X19 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x03 == STD_ON)
/* Page processing function of service report DTC Snapshot Record by Identification */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ProcessPageDTCSnapshotByIdentification(
  Dcm_DemClientIdType DemClientId,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext
)
{
  Dcm_BufferSizeType FilledLength = 0U;

#if (DCM_NUM_PROTOCOL > 1U)
  uint8 instIdx = Dcm_Dsl_Supervisor_GetProcessingProtocolIndex(
                    DCM_RX_CONNECTION_ID_FROM_RXPDUID(pMsgContext->dcmRxPduId),
                    (Dcm_RequestOriginType)(pMsgContext->msgAddInfo.requestOrigin));
#endif
  Std_ReturnType resultGetNextRecord = E_OK;
  uint8 dtcSnapShotRecordNum;
  uint32 reqDtc;
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  /* loop until no further freeze frame record is returned or response buffer becomes
     too small or maximum no. of filtered DTC count is reached */
  do
  {
    /* gather DTC informations */
    /* !LINKSTO Dcm.ReadDTCInfo.DcmDslProtocolRow.DcmDemClientRef,1 */
    resultGetNextRecord = Dem_GetNextFilteredRecord(DemClientId, &reqDtc, &dtcSnapShotRecordNum);

    if (resultGetNextRecord == E_OK)
    {
      /* append DTC(3 byte) and DTCStatus(1 byte) to the response buffer */
      pMsgContext->resData[FilledLength] = (uint8)(reqDtc >> 16U);
      FilledLength++;
      pMsgContext->resData[FilledLength] = (uint8)(reqDtc >> 8U);
      FilledLength++;
      pMsgContext->resData[FilledLength] = (uint8)(reqDtc);
      FilledLength++;
      pMsgContext->resData[FilledLength] = (Dcm_MsgItemType)dtcSnapShotRecordNum;
      FilledLength++;

      /* update the remaining DTC count to be sent as response */
      DCM_READ_DTC_INFO_CONTEXT(instIdx).DTCCount--;
    }
  }
  while ((resultGetNextRecord == E_OK) &&
         (DCM_READ_DTC_INFO_CONTEXT(instIdx).DTCCount > 0U) &&
         (((Dcm_MsgLenType)FilledLength + (Dcm_MsgLenType)DCM_DTC_SNAPSHOT_SNGL_RCD_LEN) <=
          pMsgContext->resMaxDataLen));
  if (resultGetNextRecord == DEM_PENDING)
  {
    serviceProcessStatus = DCM_E_PENDING;
  }
  else
  {
    /* if total response sent is less than actual response to be sent,
       then fill remaining bytes with 0 */
    if (resultGetNextRecord == DEM_NO_SUCH_ELEMENT)
    {
      while ((DCM_READ_DTC_INFO_CONTEXT(instIdx).DTCCount > 0U) &&
             (((Dcm_MsgLenType)FilledLength + (Dcm_MsgLenType)DCM_DTC_SNAPSHOT_SNGL_RCD_LEN) <=
             pMsgContext->resMaxDataLen))
      {
        pMsgContext->resData[FilledLength] = 0U;
        FilledLength++;
        pMsgContext->resData[FilledLength] = 0U;
        FilledLength++;
        pMsgContext->resData[FilledLength] = 0U;
        FilledLength++;
        pMsgContext->resData[FilledLength] = 0U;
        FilledLength++;
        DCM_READ_DTC_INFO_CONTEXT(instIdx).DTCCount--;
      }
    }

    /* At this point, the response buffer is always filled with data.
     * In case of E_OK, with data read out with Dem_GetNextFilteredRecord
     * or, in case of DEM_NO_SUCH_ELEMENT, with dummy 0x00,
     * because DTCCount was at least at start of this function != 0.
     * Note, that DEM_PENDING is handled in the path before. */
    DCM_PRECONDITION_ASSERT(FilledLength > 0U, DCM_SID_READ_DTC_INFORMATION);
    /* Indicate to start processing of the page, by passing the total number of bytes filled in
       this call of Dcm_DspInternal_ProcessPageDTCSnapshotByIdentification() */
    Dcm_DsdInternal_ProcessPage((Dcm_MsgLenType)FilledLength);
    serviceProcessStatus = DCM_E_PROCESS_PAGE;
  }

  return serviceProcessStatus;
}
#endif /* #if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x03 == STD_ON) */

#if (DCM_DSP_USE_SERVICE_0X19 == STD_ON)
#if ((DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x06 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x19 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x18 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x04 ==STD_ON))
/* Page processing function of service report DTC extended data record and
 * Freeze Frame data by DTC number */
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ProcessPageDtcExtDataRecord
(
  Dcm_DemClientIdType DemClientId,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext
)
{
  /* This function is called after the initial call of the service and sub-service handlers,
   * where the executers have been initialized.*/
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoBySelect_Process(DemClientId, DCM_UPDATE_PAGE, pMsgContext);

  return serviceProcessStatus;
}
#endif  /* #if ((DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x06 == STD_ON) || \
                (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x19 == STD_ON) || \
                (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x18 == STD_ON) || \
                (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x04 ==STD_ON))*/
#endif /* #if (DCM_DSP_USE_SERVICE_0X19 == STD_ON) */

#if ((DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x17 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x15 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x14 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x13 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0A == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08 == STD_ON) || \
     (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x02 == STD_ON))
/* Page processing function of service report filtered DTCs*/
STATIC FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ProcessPageFilteredDTCs
(
  Dcm_DemClientIdType DemClientId,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext
)
{
  /* This function is called after the initial call of the service and sub-service handlers,
     where the executers have been initialized.*/
  Std_ReturnType serviceProcessStatus = DCM_E_ABORT;

  serviceProcessStatus = Dcm_Dsp_DemFacade_GetDTCInfoByFilter_Process(DemClientId, DCM_UPDATE_PAGE, pMsgContext);

  return serviceProcessStatus;
}
#endif  /* #if ((DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x17 == STD_ON) || \
                (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x15 == STD_ON) || \
                (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x14 == STD_ON) || \
                (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0A == STD_ON) || \
                (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08 == STD_ON) || \
                (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x02 == STD_ON))*/

#endif /* #if (DCM_PAGEDBUFFER_ENABLED == STD_ON) */

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if ( (DCM_DSP_USE_SERVICE_0X19 == STD_ON) || (DCM_DSP_USE_SERVICE_0XAF == STD_ON) ) */

/*==================[end of file]================================================================*/
