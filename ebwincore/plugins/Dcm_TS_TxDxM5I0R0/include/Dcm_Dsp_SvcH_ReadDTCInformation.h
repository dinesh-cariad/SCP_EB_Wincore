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

#ifndef DCM_DSP_SVCH_READDTCINFORMATION_H
#define DCM_DSP_SVCH_READDTCINFORMATION_H

/* !LINKSTO Dcm.Dsn.File.RDTCI.PublicApi,1 */
/* This file contains the public API of the implementation of the Diagnostic
 * Service ReadDTCInformation. */

/*==================[inclusions]==================================================================*/

#include <Std_Types.h>         /* AUTOSAR standard types */
#include <Dcm_Int.h>

#include <Dcm_Cfg.h>

#if ( (DCM_DSP_USE_SERVICE_0X19 == STD_ON) || (DCM_DSP_USE_SERVICE_0XAF == STD_ON) )

#include <Dcm_Dsp_DemFacade.h>

#if (DCM_DEM_CLIENT_REF_USED == STD_ON)
#include <Dem.h>
#endif /* #if (DCM_DEM_CLIENT_REF_USED == STD_ON) */

/*==================[macro definitions]========================================================== */
/*==================[type declarations]===========================================================*/

/** \brief This type contains the information needed while processing service
 * ReadDTCInforamtion(0x19) */
typedef struct
{
  /** \brief Count of number of DTCs processed */
  uint16 DTCCount;

#if ((DCM_HAS_ROE_TYPE2 == STD_ON) && (DCM_ROE_NUM_ROEONDTCSTATUSCHANGE > 0U))
  /** \brief This flag indicate if the DemFacade is locked or not */
  boolean Dcm_ReadDTCInformationHasFacade;
#endif
}
Dcm_ReadDTCInfoContextType;

/*==================[external constant declarations]==============================================*/

/*==================[external data declarations]==================================================*/

/*==================[external function declarations]==============================================*/

#define DCM_START_SEC_CODE
#include <Dcm_MemMap.h>

#if(DCM_DSP_USE_SERVICE_0X19 == STD_ON)
/** \brief Implement DEM access service 0x19-dispatcher
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReadDTCInformation_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if(DCM_DSP_USE_SERVICE_0X19 == STD_ON) */

#if(DCM_DSP_USE_SERVICE_0XAF == STD_ON)
/** \brief Implement DEM access service 0xAF-dispatcher
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReadGenericInformation_SvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* (DCM_DSP_USE_SERVICE_0XAF == STD_ON) */

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x01 == STD_ON)
/** \brief UDS service 0x19-0x01 (reportNumberOfDTCByStatusMask)
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportNumberOfDTCByStatusMask_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif/*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x01 == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x02 == STD_ON)
/** \brief UDS service 0x19-0x02 (reportDTCByStatusMask)
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportDTCByStatusMask_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif/*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x02 == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x03 == STD_ON)
/** \brief UDS service 0x19-0x03 (reportDtcSnapshotRecordIdentification)
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportDtcSnapshotRecordIdentification_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif/*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x03 == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x04 == STD_ON)
/** \brief UDS service 0x19-0x04 (reportDtcSnapshotRecordByDtcNumber)
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportDtcSnapshotRecordByDtcNumber_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif/*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x04 == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x06 == STD_ON)
/** \brief UDS service 0x19-0x06 (reportDtcExtendedDataRecordByDtcNumber)
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportDtcExtendedDataRecordByDtcNumber_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif/*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x06 == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x07 == STD_ON)
/** \brief UDS service 0x19-0x07 (reportNumberOfDTCBySeverityMaskRecord)
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDTCInformation_ReportNumberOfDTCBySeverityMaskRecord_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif/*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x07 == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08 == STD_ON)
/** \brief UDS service 0x19-0x08 (reportDTCBySeverityMaskRecord)
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDTCInformation_ReportDTCBySeverityMaskRecord_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif/*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x08 == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON)
/** \brief UDS service 0x19-0x09 (reportSeverityInformationOfDTC)
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDTCInformation_ReportSeverityInformationOfDTC_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif/*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x09 == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0A == STD_ON)
/** \brief UDS service 0x19-0x0A (reportSupportedDtcs)
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportSupportedDtcs_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0A == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0B == STD_ON)
/** \brief UDS service 0x19-0x0B (reportFirstTestFailedDTC)
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDTCInformation_ReportFirstTestFailedDTC_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0B == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0C == STD_ON)
/** \brief UDS service 0x19-0x0C (reportFirstConfirmedDTC)
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDTCInformation_ReportFirstConfirmedDTC_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0C == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0D == STD_ON)
/** \brief UDS service 0x19-0x0D (reportMostRecentTestFailedDTC)
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDTCInformation_ReportMostRecentTestFailedDTC_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0D == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0E == STD_ON)
/** \brief UDS service 0x19-0x0E (reportMostRecentConfirmedDTC)
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_Dsp_ReadDTCInformation_ReportMostRecentConfirmedDTC_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x0E == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x13 == STD_ON)
/** \brief UDS service 0x19-0x13 (reportEmissionsRelatedOBDDTCByStatusMask)
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
extern FUNC(Std_ReturnType, DCM_CODE)
  Dcm_DspInternal_ReportEmissionsRelatedOBDDTCByStatusMask_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x13 == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x14 == STD_ON)
/** \brief UDS service 0x19-0x14 (reportEmissionsRelatedOBDDTCByStatusMask)
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportDTCFaultDetectionCounter_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x14 == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x15 == STD_ON)
/** \brief UDS service 0x19-0x15 (reportDTCWithPermanentStatus)
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportDTCWithPermanentStatus_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x15 == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x17 == STD_ON)
/** \brief UDS service 0x19-0x17 (reportUserDefMemoryDTCByStatusMask)
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
extern FUNC(Std_ReturnType, DCM_CODE) Dcm_DspInternal_ReportUserDefMemoryDTCByStatusMask_SSvcH(
  Dcm_OpStatusType OpStatus,
  P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif/*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x17 == STD_ON)*/

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x18 == STD_ON)
/** \brief UDS service 0x19-0x18 (reportUserDefMemoryDtcSnapshotRecordByDtcNumber)
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
extern FUNC(Std_ReturnType, DCM_CODE)
  Dcm_DspInternal_ReportUserDefMemoryDtcSnapshotRecordByDtcNumber_SSvcH(
    Dcm_OpStatusType OpStatus,
    P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif /* #if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x18 == STD_ON) */

#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x19 == STD_ON)
/** \brief UDS service 0x19-0x19 (reportUserDefMemoryDtcExtendedDataRecordByDtcNumber)
 **
 ** \param[in] OpStatus Operation state of the function.
 ** \param[inout] pMsgContext Message related information for one diagnostic protocol identifier.
 **
 ** \retval DCM_E_DONE         Operation completed
 ** \retval DCM_E_PENDING      Operation did not complete, it must be called again
 ** \retval DCM_E_PROCESS_PAGE One page is ready to be given to the PduR
 */
extern FUNC(Std_ReturnType, DCM_CODE)
  Dcm_DspInternal_ReportUserDefMemoryDtcExtendedDataRecordByDtcNumber_SSvcH(
    Dcm_OpStatusType OpStatus,
    P2VAR(Dcm_MsgContextType, AUTOMATIC, DCM_VAR) pMsgContext);
#endif/*#if (DCM_DSP_SUBSERVICE_READDTCINFORMATION_0x19 == STD_ON)*/

#define DCM_STOP_SEC_CODE
#include <Dcm_MemMap.h>

#endif /* #if ( (DCM_DSP_USE_SERVICE_0X19 == STD_ON) || (DCM_DSP_USE_SERVICE_0XAF == STD_ON) ) */

#endif /* ifndef DCM_DSP_SVCH_READDTCINFORMATION_H */
/*==================[end of file]=================================================================*/
