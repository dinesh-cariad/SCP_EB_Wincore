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
#ifndef DCM_TRACE_INTERNAL_H
#define DCM_TRACE_INTERNAL_H
/*==================[inclusions]============================================*/

/*==================[macros]================================================*/
/* Place internal debug instrumentation macros in this file, i.e. all
   macros that are not generated for the public api instrumentation.
*/

#ifndef DCM_DBG_ENABLED
/** \brief Dbg module enable status for Dcm */
#define DCM_DBG_ENABLED STD_OFF
#endif

#ifndef DBG_DCM_CURPROTOCOLID
/** \brief Change of Dcm_CurProtocolId */
#define DBG_DCM_CURPROTOCOLID(a,b)
#endif

#ifndef DBG_DCM_REQPROCESSING
/** \brief Change of Dcm_ReqProcessing */
#define DBG_DCM_REQPROCESSING(a,b)
#endif

#ifndef DBG_DCM_ROEPROCESSING
/** \brief Change of Dcm_ROEProcessing */
#define DBG_DCM_ROEPROCESSING(a,b)
#endif

#ifndef DBG_DCM_ROEENQUEUEEVENT
/** \brief Change of Dcm_ROEEnqueueEvent */
#define DBG_DCM_ROEENQUEUEEVENT(a,b)
#endif

#ifndef DBG_DCM_DSL_TXCONNECTIONPROCESSTXCONNECTIONS_ENTRY
/** \brief Entry point of function Dcm_Dsl_TxConnectionProcessTxConnections() */
#define DBG_DCM_DSL_TXCONNECTIONPROCESSTXCONNECTIONS_ENTRY()
#endif

#ifndef DBG_DCM_DSL_TXCONNECTIONPROCESSTXCONNECTIONS_EXIT
/** \brief Exit point of function Dcm_Dsl_TxConnectionProcessTxConnections() */
#define DBG_DCM_DSL_TXCONNECTIONPROCESSTXCONNECTIONS_EXIT()
#endif

#ifndef DBG_DCM_DSDINTERNAL_DISPATCHSVCREQ_ENTRY
/** \brief Entry point of function Dcm_DsdInternal_DispatchSvcReq() */
#define DBG_DCM_DSDINTERNAL_DISPATCHSVCREQ_ENTRY(a)
#endif

#ifndef DBG_DCM_DSDINTERNAL_DISPATCHSVCREQ_EXIT
/** \brief Exit point of function Dcm_DsdInternal_DispatchSvcReq() */
#define DBG_DCM_DSDINTERNAL_DISPATCHSVCREQ_EXIT(a)
#endif

#ifndef DBG_DCM_DSDINTERNAL_STARTPAGEDPROCESSING_ENTRY
/** \brief Entry point of function Dcm_DsdInternal_StartPagedProcessing() */
#define DBG_DCM_DSDINTERNAL_STARTPAGEDPROCESSING_ENTRY(a)
#endif

#ifndef DBG_DCM_DSDINTERNAL_STARTPAGEDPROCESSING_EXIT
/** \brief Entry point of function Dcm_DsdInternal_StartPagedProcessing() */
#define DBG_DCM_DSDINTERNAL_STARTPAGEDPROCESSING_EXIT(a)
#endif

#ifndef DBG_DCM_DSDINTERNAL_PROCESSPAGE_ENTRY
/** \brief Entry point of function Dcm_DsdInternal_ProcessPage() */
#define DBG_DCM_DSDINTERNAL_PROCESSPAGE_ENTRY(a)
#endif

#ifndef DBG_DCM_DSDINTERNAL_PROCESSPAGE_EXIT
/** \brief Entry point of function Dcm_DsdInternal_ProcessPage() */
#define DBG_DCM_DSDINTERNAL_PROCESSPAGE_EXIT(a)
#endif

#ifndef DBG_DCM_DSPINTERNAL_SVCFUNC_ENTRY
/** \brief Entry point of function Dcm_DspInternal_SvcFunc() */
#define DBG_DCM_DSPINTERNAL_SVCFUNC_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_SVCFUNC_EXIT
/** \brief Exit point of function Dcm_DspInternal_SvcFunc() */
#define DBG_DCM_DSPINTERNAL_SVCFUNC_EXIT(a,b)
#endif

#ifndef DBG_DCM_APICALLER_GENERICSVCHIMPLEMENTATION_ENTRY
/** \brief Entry point of function Dcm_APICaller_GenericSvcHImplementation() */
#define DBG_DCM_APICALLER_GENERICSVCHIMPLEMENTATION_ENTRY(a,b)
#endif

#ifndef DBG_DCM_APICALLER_GENERICSVCHIMPLEMENTATION_EXIT
/** \brief Exit point of function Dcm_APICaller_GenericSvcHImplementation() */
#define DBG_DCM_APICALLER_GENERICSVCHIMPLEMENTATION_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_DIAGNOSTICSESSIONCONTROL_SVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_DiagnosticSessionControl_SvcH() */
#define DBG_DCM_DSPINTERNAL_DIAGNOSTICSESSIONCONTROL_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_DIAGNOSTICSESSIONCONTROL_SVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_DiagnosticSessionControl_SvcH() */
#define DBG_DCM_DSPINTERNAL_DIAGNOSTICSESSIONCONTROL_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_DIAGNOSTICSESSIONCONTROLCOMMON_SSVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_DiagnosticSessionControlCommon_SSvcH() */
#define DBG_DCM_DSPINTERNAL_DIAGNOSTICSESSIONCONTROLCOMMON_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_DIAGNOSTICSESSIONCONTROLCOMMON_SSVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_DiagnosticSessionControlCommon_SSvcH() */
#define DBG_DCM_DSPINTERNAL_DIAGNOSTICSESSIONCONTROLCOMMON_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_ECURESET_SVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ECUReset_SvcH() */
#define DBG_DCM_DSPINTERNAL_ECURESET_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_ECURESET_SVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_ECUReset_SvcH() */
#define DBG_DCM_DSPINTERNAL_ECURESET_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_HARDRESET_SSVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_HardReset_SSvcH() */
#define DBG_DCM_DSPINTERNAL_HARDRESET_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_HARDRESET_SSVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_HardReset_SSvcH() */
#define DBG_DCM_DSPINTERNAL_HARDRESET_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_KEYOFFONRESET_SSVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_KeyOffOnReset_SSvcH() */
#define DBG_DCM_DSPINTERNAL_KEYOFFONRESET_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_KEYOFFONRESET_SSVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_KeyOffOnReset_SSvcH() */
#define DBG_DCM_DSPINTERNAL_KEYOFFONRESET_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_SOFTRESET_SSVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_SoftReset_SSvcH() */
#define DBG_DCM_DSPINTERNAL_SOFTRESET_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_SOFTRESET_SSVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_SoftReset_SSvcH() */
#define DBG_DCM_DSPINTERNAL_SOFTRESET_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_ENABLERAPIDPOWERSHUTDOWN_SSVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_EnableRapidPowerShutDown_SSvcH() */
#define DBG_DCM_DSPINTERNAL_ENABLERAPIDPOWERSHUTDOWN_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_ENABLERAPIDPOWERSHUTDOWN_SSVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_EnableRapidPowerShutDown_SSvcH() */
#define DBG_DCM_DSPINTERNAL_ENABLERAPIDPOWERSHUTDOWN_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_DISABLERAPIDPOWERSHUTDOWN_SSVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_DisableRapidPowerShutDown_SSvcH() */
#define DBG_DCM_DSPINTERNAL_DISABLERAPIDPOWERSHUTDOWN_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_DISABLERAPIDPOWERSHUTDOWN_SSVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_DisableRapidPowerShutDown_SSvcH() */
#define DBG_DCM_DSPINTERNAL_DISABLERAPIDPOWERSHUTDOWN_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_CLEARDIAGNOSTICINFORMATION_SVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ClearDiagnosticInformation_SvcH() */
#define DBG_DCM_DSPINTERNAL_CLEARDIAGNOSTICINFORMATION_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_CLEARDIAGNOSTICINFORMATION_SVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_ClearDiagnosticInformation_SvcH() */
#define DBG_DCM_DSPINTERNAL_CLEARDIAGNOSTICINFORMATION_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_CLEARDTCINFO_SVCSTART_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ClearDTCInfo_SvcStart() */
#define DBG_DCM_DSPINTERNAL_CLEARDTCINFO_SVCSTART_ENTRY(a)
#endif

#ifndef DBG_DCM_DSPINTERNAL_CLEARDTCINFO_SVCSTART_EXIT
/** \brief Exit point of function Dcm_DspInternal_ClearDTCInfo_SvcStart() */
#define DBG_DCM_DSPINTERNAL_CLEARDTCINFO_SVCSTART_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_READDTCINFORMATION_SVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ReadDTCInformation_SvcH() */
#define DBG_DCM_DSPINTERNAL_READDTCINFORMATION_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_READDTCINFORMATION_SVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_ReadDTCInformation_SvcH() */
#define DBG_DCM_DSPINTERNAL_READDTCINFORMATION_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTNUMBEROFDTCBYSTATUSMASK_SSVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ReportNumberOfDTCByStatusMask_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTNUMBEROFDTCBYSTATUSMASK_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTNUMBEROFDTCBYSTATUSMASK_SSVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_ReportNumberOfDTCByStatusMask_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTNUMBEROFDTCBYSTATUSMASK_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTDTCFAULTDETECTIONCOUNTER_SSVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_reportDTCFaultDetectionCounter_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTDTCFAULTDETECTIONCOUNTER_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTDTCFAULTDETECTIONCOUNTER_SSVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_reportDTCFaultDetectionCounter_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTDTCFAULTDETECTIONCOUNTER_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTNUMBEROFDTCBYSTATUSMASK_PROCESS_ENTRY
/** \brief Entry point of function Dcm_DspInternal_reportNumberOfDTCByStatusMask_Process() */
#define DBG_DCM_DSPINTERNAL_REPORTNUMBEROFDTCBYSTATUSMASK_PROCESS_ENTRY(a)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTNUMBEROFDTCBYSTATUSMASK_PROCESS_EXIT
/** \brief Exit point of function Dcm_DspInternal_reportNumberOfDTCByStatusMask_Process() */
#define DBG_DCM_DSPINTERNAL_REPORTNUMBEROFDTCBYSTATUSMASK_PROCESS_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTDTCBYSTATUSMASK_SSVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ReportDTCByStatusMask_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTDTCBYSTATUSMASK_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTDTCBYSTATUSMASK_SSVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_ReportDTCByStatusMask_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTDTCBYSTATUSMASK_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTDTCSNAPSHOTRECORDIDENTIFICATION_SSVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ReportDtcSnapshotRecordIdentification_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTDTCSNAPSHOTRECORDIDENTIFICATION_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTDTCSNAPSHOTRECORDIDENTIFICATION_SSVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_ReportDtcSnapshotRecordIdentification_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTDTCSNAPSHOTRECORDIDENTIFICATION_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTDTCSNAPSHOTRECORDBYDTCNUMBER_SSVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ReportDtcSnapshotRecordByDtcNumber_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTDTCSNAPSHOTRECORDBYDTCNUMBER_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTDTCSNAPSHOTRECORDBYDTCNUMBER_SSVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_ReportDtcSnapshotRecordByDtcNumber_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTDTCSNAPSHOTRECORDBYDTCNUMBER_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTDTCEXTENDEDDATARECORDBYDTCNUMBER_SSVCH_ENTRY
#define DBG_DCM_DSPINTERNAL_REPORTDTCEXTENDEDDATARECORDBYDTCNUMBER_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTDTCEXTENDEDDATARECORDBYDTCNUMBER_SSVCH_EXIT
#define DBG_DCM_DSPINTERNAL_REPORTDTCEXTENDEDDATARECORDBYDTCNUMBER_SSVCH_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTUSERDEFMDTCEXTENDEDDATARECORDBYDTCNUMBER_SSVCH_ENTRY
/** \brief Entry point of function
    Dcm_DspInternal_ReportUserDefMemoryDtcExtendedDataRecordByDtcNumber_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTUSERDEFMDTCEXTENDEDDATARECORDBYDTCNUMBER_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTUSERDEFMDTCEXTENDEDDATARECORDBYDTCNUMBER_SSVCH_EXIT
/** \brief Exit point of function
    Dcm_DspInternal_ReportUserDefMemoryDtcExtendedDataRecordByDtcNumber_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTUSERDEFMDTCEXTENDEDDATARECORDBYDTCNUMBER_SSVCH_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTSUPPORTEDDTCS_SSVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ReportSupportedDtcs_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTSUPPORTEDDTCS_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTSUPPORTEDDTCS_SSVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_ReportSupportedDtcs_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTSUPPORTEDDTCS_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_READDTCINFORMATION_REPORTNUMBEROFDTCBYSEVERITYMASKRECORD_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDTCInformation_ReportNumberOfDTCBySeverityMaskRecord_SSvcH() */
#define DBG_DCM_DSP_READDTCINFORMATION_REPORTNUMBEROFDTCBYSEVERITYMASKRECORD_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_READDTCINFORMATION_REPORTNUMBEROFDTCBYSEVERITYMASKRECORD_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDTCInformation_ReportNumberOfDTCBySeverityMaskRecord_SSvcH() */
#define DBG_DCM_DSP_READDTCINFORMATION_REPORTNUMBEROFDTCBYSEVERITYMASKRECORD_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_READDTCINFORMATION_REPORTDTCBYSEVERITYMASKRECORD_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDTCInformation_ReportDTCBySeverityMaskRecord_SSvcH() */
#define DBG_DCM_DSP_READDTCINFORMATION_REPORTDTCBYSEVERITYMASKRECORD_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_READDTCINFORMATION_REPORTDTCBYSEVERITYMASKRECORD_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDTCInformation_ReportDTCBySeverityMaskRecord_SSvcH() */
#define DBG_DCM_DSP_READDTCINFORMATION_REPORTDTCBYSEVERITYMASKRECORD_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_READDTCINFORMATION_REPORTSEVERITYINFORMATIONOFDTC_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDTCInformation_ReportSeverityInformationOfDTC_SSvcH() */
#define DBG_DCM_DSP_READDTCINFORMATION_REPORTSEVERITYINFORMATIONOFDTC_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_READDTCINFORMATION_REPORTSEVERITYINFORMATIONOFDTC_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDTCInformation_ReportSeverityInformationOfDTC_SSvcH() */
#define DBG_DCM_DSP_READDTCINFORMATION_REPORTSEVERITYINFORMATIONOFDTC_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_CLEAREMISSIONDIAGINFO_SVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ClearEmissionDiagInfo_SvcH() */
#define DBG_DCM_DSPINTERNAL_CLEAREMISSIONDIAGINFO_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_CLEAREMISSIONDIAGINFO_SVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_ClearEmissionDiagInfo_SvcH() */
#define DBG_DCM_DSPINTERNAL_CLEAREMISSIONDIAGINFO_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTEMISSIONSRELATEDOBDDTCBYSTATUSMASK_SSVCH_ENTRY
/** \brief Entry point of function
    Dcm_DspInternal_ReportEmissionsRelatedOBDDTCByStatusMask_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTEMISSIONSRELATEDOBDDTCBYSTATUSMASK_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTEMISSIONSRELATEDOBDDTCBYSTATUSMASK_SSVCH_EXIT
/** \brief Exit point of function
    Dcm_DspInternal_ReportEmissionsRelatedOBDDTCByStatusMask_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTEMISSIONSRELATEDOBDDTCBYSTATUSMASK_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTDTCWITHPERMANENTSTATUS_SSVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ReportDTCWithPermanentStatus_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTDTCWITHPERMANENTSTATUS_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTDTCWITHPERMANENTSTATUS_SSVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_ReportDTCWithPermanentStatus_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTDTCWITHPERMANENTSTATUS_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_READDATABYIDENTIFIER_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDataByIdentifier_Init() */
#define DBG_DCM_DSP_READDATABYIDENTIFIER_INIT_ENTRY()
#endif

#ifndef DBG_DCM_DSP_READDATABYIDENTIFIER_INIT_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDataByIdentifier_Init() */
#define DBG_DCM_DSP_READDATABYIDENTIFIER_INIT_EXIT()
#endif

#ifndef DBG_DCM_DSP_READDATABYIDENTIFIER_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDataByIdentifier_SvcH() */
#define DBG_DCM_DSP_READDATABYIDENTIFIER_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_READDATABYIDENTIFIER_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDataByIdentifier_SvcH() */
#define DBG_DCM_DSP_READDATABYIDENTIFIER_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_READSCALINGDATABYIDENTIFIER_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadScalingDataByIdentifier_SvcH() */
#define DBG_DCM_DSP_READSCALINGDATABYIDENTIFIER_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_READSCALINGDATABYIDENTIFIER_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadScalingDataByIdentifier_SvcH() */
#define DBG_DCM_DSP_READSCALINGDATABYIDENTIFIER_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_SvcH() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_SvcH() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_HSMDCMSFPERIODICRESPONSEPROCESSINGENTRY_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfPeriodicResponseProcessingEntry() */
#define DBG_DCM_HSMDCMSFPERIODICRESPONSEPROCESSINGENTRY_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFPERIODICRESPONSEPROCESSINGENTRY_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfPeriodicResponseProcessingEntry() */
#define DBG_DCM_HSMDCMSFPERIODICRESPONSEPROCESSINGENTRY_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_SECURITYACCESS_SVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_SecurityAccess_SvcH() */
#define DBG_DCM_DSP_SECURITYACCESS_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_SECURITYACCESS_SVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_SecurityAccess_SvcH() */
#define DBG_DCM_DSP_SECURITYACCESS_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_CommunicationControl_SvcH() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_CommunicationControl_SvcH() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_SVCSTART_ENTRY
/** \brief Entry point of function Dcm_Dsp_CommunicationControl_SvcStart() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_SVCSTART_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_SVCSTART_EXIT
/** \brief Exit point of function Dcm_Dsp_CommunicationControl_SvcStart() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_SVCSTART_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_SVCCONT_ENTRY
/** \brief Entry point of function Dcm_Dsp_CommunicationControl_SvcCont() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_SVCCONT_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_SVCCONT_EXIT
/** \brief Exit point of function Dcm_Dsp_CommunicationControl_SvcCont() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_SVCCONT_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDTX_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_CommunicationControl_EnableRxAndTx_SSvcH() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDTX_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDTX_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_CommunicationControl_EnableRxAndTx_SSvcH() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDTX_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDDISABLETX_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_CommunicationControl_EnableRxAndDisableTx_SSvcH() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDDISABLETX_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDDISABLETX_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_CommunicationControl_EnableRxAndDisableTx_SSvcH() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDDISABLETX_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_DISABLERXANDENABLETX_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_CommunicationControl_DisableRxAndEnableTx_SSvcH() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_DISABLERXANDENABLETX_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_DISABLERXANDENABLETX_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_CommunicationControl_DisableRxAndEnableTx_SSvcH() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_DISABLERXANDENABLETX_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_DISABLERXANDTX_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_CommunicationControl_DisableRxAndTx_SSvcH() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_DISABLERXANDTX_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_DISABLERXANDTX_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_CommunicationControl_DisableRxAndTx_SSvcH() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_DISABLERXANDTX_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDDISABLETXWITHENHANCEDADDRINFO_SSVCH_ENTRY
/** \brief Entry point of function
 **        Dcm_Dsp_CommunicationControl_EnableRxAndDisableTxWithEnhancedAddrInfo_SSvcH()
 */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDDISABLETXWITHENHANCEDADDRINFO_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDDISABLETXWITHENHANCEDADDRINFO_SSVCH_EXIT
/** \brief Exit point of function
 **        Dcm_Dsp_CommunicationControl_EnableRxAndDisableTxWithEnhancedAddrInfo_SSvcH()
 */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDDISABLETXWITHENHANCEDADDRINFO_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDTXWITHENHANCEDADDRINFO_SSVCH_ENTRY
/** \brief Entry point of function
 **        Dcm_Dsp_CommunicationControl_EnableRxAndTxWithEnhancedAddrInfo_SSvcH()
 */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDTXWITHENHANCEDADDRINFO_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDTXWITHENHANCEDADDRINFO_SSVCH_EXIT
/** \brief Exit point of function
 **        Dcm_Dsp_CommunicationControl_EnableRxAndTxWithEnhancedAddrInfo_SSvcH()
 */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_ENABLERXANDTXWITHENHANCEDADDRINFO_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_SETCONFIRMEDCOMMUNICATIONMODE_ENTRY
/** \brief Entry point of function Dcm_Dsp_CommunicationControl_SetConfirmedCommunicationMode() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_SETCONFIRMEDCOMMUNICATIONMODE_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_SETCONFIRMEDCOMMUNICATIONMODE_EXIT
/** \brief Exit point of function Dcm_Dsp_CommunicationControl_SetConfirmedCommunicationMode() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_SETCONFIRMEDCOMMUNICATIONMODE_EXIT()
#endif

#ifndef DBG_DCM_DSPINTERNAL_DYNAMICALLYDEFINEDATAIDENTIFIER_SVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_DynamicallyDefineDataIdentifier_SvcH() */
#define DBG_DCM_DSPINTERNAL_DYNAMICALLYDEFINEDATAIDENTIFIER_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_DYNAMICALLYDEFINEDATAIDENTIFIER_SVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_DynamicallyDefineDataIdentifier_SvcH() */
#define DBG_DCM_DSPINTERNAL_DYNAMICALLYDEFINEDATAIDENTIFIER_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYMEM_SSVCH_ENTRY
/** \brief Entry point of function
    Dcm_DspInternal_DynamicallyDefineDataIdentifierAddSrcByMem_SSvcH() */
#define DBG_DCM_DSPINTERNAL_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYMEM_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYMEM_SSVCH_EXIT
/** \brief Exit point of function
    Dcm_DspInternal_DynamicallyDefineDataIdentifierAddSrcByMem_SSvcH() */
#define DBG_DCM_DSPINTERNAL_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYMEM_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDMEMSRCTODDDID_ENTRY
/** \brief Entry point of function Dcm_Dsp_DynamicallyDefineDataIdentifierAddMemSrcToDDDID() */
#define DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDMEMSRCTODDDID_ENTRY(a,b,c,d)
#endif

#ifndef DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDMEMSRCTODDDID_EXIT
/** \brief Exit point of function Dcm_Dsp_DynamicallyDefineDataIdentifierAddMemSrcToDDDID() */
#define DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDMEMSRCTODDDID_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_DYNAMICALLYDEFINEDATAIDENTIFIERCLEAR_SSVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_DynamicallyDefineDataIdentifierClear_SSvcH() */
#define DBG_DCM_DSPINTERNAL_DYNAMICALLYDEFINEDATAIDENTIFIERCLEAR_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_DYNAMICALLYDEFINEDATAIDENTIFIERCLEAR_SSVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_DynamicallyDefineDataIdentifierClear_SSvcH() */
#define DBG_DCM_DSPINTERNAL_DYNAMICALLYDEFINEDATAIDENTIFIERCLEAR_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_DDDIDWRITETONVM_ENTRY
/** \brief Entry point of function Dcm_DspInternal_DDDIDWriteToNvM() */
#define DBG_DCM_DSPINTERNAL_DDDIDWRITETONVM_ENTRY()
#endif

#ifndef DBG_DCM_DSPINTERNAL_DDDIDWRITETONVM_EXIT
/** \brief Exit point of function Dcm_DspInternal_DDDIDWriteToNvM() */
#define DBG_DCM_DSPINTERNAL_DDDIDWRITETONVM_EXIT(a)
#endif

#ifndef DBG_DCM_DSPINTERNAL_VALIDATEDDDID_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ValidateDDDid() */
#define DBG_DCM_DSPINTERNAL_VALIDATEDDDID_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_VALIDATEDDDID_EXIT
/** \brief Exit point of function Dcm_DspInternal_ValidateDDDid() */
#define DBG_DCM_DSPINTERNAL_VALIDATEDDDID_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_CLEAR_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_DynamicDid_Clear() */
#define DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_CLEAR_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_CLEAR_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_DynamicDid_Clear() */
#define DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_CLEAR_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_DynamicDid_Init() */
#define DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_INIT_ENTRY()
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_INIT_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_DynamicDid_Init() */
#define DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_INIT_EXIT()
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_INITPERSISTENT_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_DynamicDid_InitPersistent() */
#define DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_INITPERSISTENT_ENTRY()
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_INITPERSISTENT_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_DynamicDid_InitPersistent() */
#define DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_INITPERSISTENT_EXIT()
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_INITIALIZED_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_DynamicDid_Initialized() */
#define DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_INITIALIZED_ENTRY()
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_INITIALIZED_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_DynamicDid_Initialized() */
#define DBG_DCM_DSP_DIDSERVICES_DYNAMICDID_INITIALIZED_EXIT(a)
#endif

#ifndef DBG_DCM_DSPINTERNAL_WRITEDATABYIDENTIFIER_SVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_WriteDataByIdentifier_SvcH() */
#define DBG_DCM_DSPINTERNAL_WRITEDATABYIDENTIFIER_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_WRITEDATABYIDENTIFIER_SVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_WriteDataByIdentifier_SvcH() */
#define DBG_DCM_DSPINTERNAL_WRITEDATABYIDENTIFIER_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsp_InputOutputControlByIdentifier_Init() */
#define DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_INIT_ENTRY()
#endif

#ifndef DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_INIT_EXIT
/** \brief Exit point of function Dcm_Dsp_InputOutputControlByIdentifier_Init() */
#define DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_INIT_EXIT()
#endif

#ifndef DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_GETRETURNCONTROLTOECUTIMEOUTCOUNT_ENTRY
/** \brief Entry point of function Dcm_Dsp_InputOutputControlByIdentifier_GetReturnControlToEcuTimeoutCount() */
#define DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_GETRETURNCONTROLTOECUTIMEOUTCOUNT_ENTRY()
#endif

#ifndef DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_GETRETURNCONTROLTOECUTIMEOUTCOUNT_EXIT
/** \brief Exit point of function Dcm_Dsp_InputOutputControlByIdentifier_GetReturnControlToEcuTimeoutCount() */
#define DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_GETRETURNCONTROLTOECUTIMEOUTCOUNT_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_GETRETURNCONTROLTOECUSTATUS_ENTRY
/** \brief Entry point of function Dcm_Dsp_InputOutputControlByIdentifier_GetReturnControlToEcuStatus() */
#define DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_GETRETURNCONTROLTOECUSTATUS_ENTRY()
#endif

#ifndef DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_GETRETURNCONTROLTOECUSTATUS_EXIT
/** \brief Exit point of function Dcm_Dsp_InputOutputControlByIdentifier_GetReturnControlToEcuStatus() */
#define DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_GETRETURNCONTROLTOECUSTATUS_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SETRETURNCONTROLTOECUSTATUS_ENTRY
/** \brief Entry point of function Dcm_Dsp_InputOutputControlByIdentifier_SetReturnControlToEcuStatus() */
#define DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SETRETURNCONTROLTOECUSTATUS_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SETRETURNCONTROLTOECUSTATUS_EXIT
/** \brief Exit point of function Dcm_Dsp_InputOutputControlByIdentifier_SetReturnControlToEcuStatus() */
#define DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SETRETURNCONTROLTOECUSTATUS_EXIT()
#endif

#ifndef DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_GETRETURNCONTROLTOECUSTATUS_DEFAULTINTERFACE_ENTRY
/** \brief Entry point of function Dcm_Dsp_InputOutputControlByIdentifier_GetReturnControlToEcuDefaultStatus() */
#define DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_GETRETURNCONTROLTOECUSTATUS_DEFAULTINTERFACE_ENTRY()
#endif

#ifndef DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_GETRETURNCONTROLTOECUSTATUS_DEFAULTINTERFACE_EXIT
/** \brief Exit point of function Dcm_Dsp_InputOutputControlByIdentifier_GetReturnControlToEcuDefaultStatus() */
#define DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_GETRETURNCONTROLTOECUSTATUS_DEFAULTINTERFACE_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SETRETURNCONTROLTOECUSTATUS_DEFAULTINTERFACE_ENTRY
/** \brief Entry point of function Dcm_Dsp_InputOutputControlByIdentifier_SetReturnControlToEcuDefaultStatus() */
#define DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SETRETURNCONTROLTOECUSTATUS_DEFAULTINTERFACE_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SETRETURNCONTROLTOECUSTATUS_DEFAULTINTERFACE_EXIT
/** \brief Exit point of function Dcm_Dsp_InputOutputControlByIdentifier_SetReturnControlToEcuDefaultStatus() */
#define DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SETRETURNCONTROLTOECUSTATUS_DEFAULTINTERFACE_EXIT()
#endif

#ifndef DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_InputOutputControlByIdentifier_SvcH() */
#define DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_InputOutputControlByIdentifier_SvcH() */
#define DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_RETURNCONTROLTOECUFORALLSIGNALSTRYLOCK_ENTRY
/** \brief Entry point of function Dcm_Dsp_InputOutputControlByIdentifier_ReturnControlToEcuForAllSignalsTryLock() */
#define DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_RETURNCONTROLTOECUFORALLSIGNALSTRYLOCK_ENTRY()
#endif

#ifndef DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_RETURNCONTROLTOECUFORALLSIGNALSTRYLOCK_EXIT
/** \brief Exit point of function Dcm_Dsp_InputOutputControlByIdentifier_ReturnControlToEcuForAllSignalsTryLock() */
#define DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_RETURNCONTROLTOECUFORALLSIGNALSTRYLOCK_EXIT()
#endif

#ifndef DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_RETURNCONTROLTOECUFORALLSIGNALS_ENTRY
/** \brief Entry point of function Dcm_Dsp_InputOutputControlByIdentifier_ReturnControlToEcuForAllSignals() */
#define DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_RETURNCONTROLTOECUFORALLSIGNALS_ENTRY()
#endif

#ifndef DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_RETURNCONTROLTOECUFORALLSIGNALS_EXIT
/** \brief Exit point of function Dcm_Dsp_InputOutputControlByIdentifier_ReturnControlToEcuForAllSignals() */
#define DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_RETURNCONTROLTOECUFORALLSIGNALS_EXIT()
#endif

#ifndef DBG_DCM_DSP_ROUTINECONTROL_STARTROUTINE_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_RoutineControl_StartRoutine_SvcH() */
#define DBG_DCM_DSP_ROUTINECONTROL_STARTROUTINE_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_ROUTINECONTROL_STARTROUTINE_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_RoutineControl_StartRoutine_SvcH() */
#define DBG_DCM_DSP_ROUTINECONTROL_STARTROUTINE_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_ROUTINECONTROL_STOPTROUTINE_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_RoutineControl_StopRoutine_SvcH() */
#define DBG_DCM_DSP_ROUTINECONTROL_STOPROUTINE_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_ROUTINECONTROL_STOPROUTINE_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_RoutineControl_StopRoutine_SvcH() */
#define DBG_DCM_DSP_ROUTINECONTROL_STOPROUTINE_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_ROUTINECONTROL_REQUESTRESULTS_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_RoutineControl_RequestResults_SvcH() */
#define DBG_DCM_DSP_ROUTINECONTROL_REQUESTRESULTS_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_ROUTINECONTROL_REQUESTRESULTS_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_RoutineControl_RequestResults_SvcH() */
#define DBG_DCM_DSP_ROUTINECONTROL_REQUESTRESULTS_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_ROUTINECONTROL_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_RoutineControl_SvcH() */
#define DBG_DCM_DSP_ROUTINECONTROL_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_ROUTINECONTROL_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_RoutineControl_SvcH() */
#define DBG_DCM_DSP_ROUTINECONTROL_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_TESTERPRESENT_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_TesterPresent_SvcH() */
#define DBG_DCM_DSP_TESTERPRESENT_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_TESTERPRESENT_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_TesterPresent_SvcH() */
#define DBG_DCM_DSP_TESTERPRESENT_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_TESTERPRESENT_SVCSTART_ENTRY
/** \brief Entry point of function Dcm_Dsp_TesterPresent_SvcStart() */
#define DBG_DCM_DSP_TESTERPRESENT_SVCSTART_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_TESTERPRESENT_SVCSTART_EXIT
/** \brief Exit point of function Dcm_Dsp_TesterPresent_SvcStart() */
#define DBG_DCM_DSP_TESTERPRESENT_SVCSTART_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSP_TESTERPRESENT_SVCCONT_ENTRY
/** \brief Entry point of function Dcm_Dsp_TesterPresent_SvcCont() */
#define DBG_DCM_DSP_TESTERPRESENT_SVCCONT_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_TESTERPRESENT_SVCCONT_EXIT
/** \brief Exit point of function Dcm_Dsp_TesterPresent_SvcCont() */
#define DBG_DCM_DSP_TESTERPRESENT_SVCCONT_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSP_TESTERPRESENT_ZEROSUBFUNCTION_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_TesterPresent_ZeroSubFunction_SSvcH() */
#define DBG_DCM_DSP_TESTERPRESENT_ZEROSUBFUNCTION_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_TESTERPRESENT_ZEROSUBFUNCTION_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_TesterPresent_ZeroSubFunction_SSvcH() */
#define DBG_DCM_DSP_TESTERPRESENT_ZEROSUBFUNCTION_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_CONTROLDTCSETTING_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ControlDTCSetting_SvcH() */
#define DBG_DCM_DSP_CONTROLDTCSETTING_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_CONTROLDTCSETTING_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ControlDTCSetting_SvcH() */
#define DBG_DCM_DSP_CONTROLDTCSETTING_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ResponseOnEvent_SvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ResponseOnEvent_SvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_ONDTCSTATUSCHANGENONPERSISTENT_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ResponseOnEvent_OnDTCStatusChangeNonPersistent_SSvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_ONDTCSTATUSCHANGENONPERSISTENT_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_ONDTCSTATUSCHANGENONPERSISTENT_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ResponseOnEvent_OnDTCStatusChangeNonPersistent_SSvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_ONDTCSTATUSCHANGENONPERSISTENT_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_ONDTCSTATUSCHANGEPERSISTENT_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ResponseOnEvent_OnDTCStatusChangePersistent_SSvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_ONDTCSTATUSCHANGEPERSISTENT_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_ONDTCSTATUSCHANGEPERSISTENT_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ResponseOnEvent_OnDTCStatusChangePersistent_SSvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_ONDTCSTATUSCHANGEPERSISTENT_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_ONCHANGEOFDATAIDENTIFIERNONPERSISTENT_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ResponseOnEvent_OnChangeOfDataIdentifierNonPersistent_SSvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_ONCHANGEOFDATAIDENTIFIERNONPERSISTENT_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_ONCHANGEOFDATAIDENTIFIERNONPERSISTENT_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ResponseOnEvent_OnChangeOfDataIdentifierNonPersistent_SSvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_ONCHANGEOFDATAIDENTIFIERNONPERSISTENT_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_ONCHANGEOFDATAIDENTIFIERPERSISTENT_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ResponseOnEvent_OnChangeOfDataIdentifierPersistent_SSvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_ONCHANGEOFDATAIDENTIFIERPERSISTENT_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_ONCHANGEOFDATAIDENTIFIERPERSISTENT_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ResponseOnEvent_OnChangeOfDataIdentifierPersistent_SSvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_ONCHANGEOFDATAIDENTIFIERPERSISTENT_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_STARTRESPONSEONEVENTNONPERSISTENT_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ResponseOnEvent_StartResponseOnEventNonPersistent_SSvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_STARTRESPONSEONEVENTNONPERSISTENT_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_STARTRESPONSEONEVENTNONPERSISTENT_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ResponseOnEvent_StartResponseOnEventNonPersistent_SSvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_STARTRESPONSEONEVENTNONPERSISTENT_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_STARTRESPONSEONEVENTPERSISTENT_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ResponseOnEvent_StartResponseOnEventPersistent_SSvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_STARTRESPONSEONEVENTPERSISTENT_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_STARTRESPONSEONEVENTPERSISTENT_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ResponseOnEvent_StartResponseOnEventPersistent_SSvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_STARTRESPONSEONEVENTPERSISTENT_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_STOPRESPONSEONEVENTNONPERSISTENT_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ResponseOnEvent_StopResponseOnEventNonPersistent_SSvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_STOPRESPONSEONEVENTNONPERSISTENT_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_STOPRESPONSEONEVENTNONPERSISTENT_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ResponseOnEvent_StopResponseOnEventNonPersistent_SSvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_STOPRESPONSEONEVENTNONPERSISTENT_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_STOPRESPONSEONEVENTPERSISTENT_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ResponseOnEvent_StopResponseOnEventPersistent_SSvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_STOPRESPONSEONEVENTPERSISTENT_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_STOPRESPONSEONEVENTPERSISTENT_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ResponseOnEvent_StopResponseOnEventPersistent_SSvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_STOPRESPONSEONEVENTPERSISTENT_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_CLEARRESPONSEONEVENT_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ResponseOnEvent_ClearResponseOnEvent_SSvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_CLEARRESPONSEONEVENT_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_CLEARRESPONSEONEVENT_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ResponseOnEvent_ClearResponseOnEvent_SSvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_CLEARRESPONSEONEVENT_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_REPORTACTIVATEDEVENTS_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ResponseOnEvent_ReportActivatedEvents_SSvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_REPORTACTIVATEDEVENTS_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_RESPONSEONEVENT_REPORTACTIVATEDEVENTS_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ResponseOnEvent_ReportActivatedEvents_SSvcH() */
#define DBG_DCM_DSP_RESPONSEONEVENT_REPORTACTIVATEDEVENTS_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_UPDATEPAGE_ENTRY
/** \brief Entry point of function Dcm_DspInternal_UpdatePage() */
#define DBG_DCM_DSPINTERNAL_UPDATEPAGE_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_UPDATEPAGE_EXIT
/** \brief Entry point of function Dcm_DspInternal_UpdatePage() */
#define DBG_DCM_DSPINTERNAL_UPDATEPAGE_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_CANCELPAGEDBUFFERPROCESSING_ENTRY
/** \brief Entry point of function Dcm_DspInternal_CancelPagedBufferProcessing() */
#define DBG_DCM_DSPINTERNAL_CANCELPAGEDBUFFERPROCESSING_ENTRY()
#endif

#ifndef DBG_DCM_DSPINTERNAL_CANCELPAGEDBUFFERPROCESSING_EXIT
/** \brief Exit point of function Dcm_DspInternal_CancelPagedBufferProcessing() */
#define DBG_DCM_DSPINTERNAL_CANCELPAGEDBUFFERPROCESSING_EXIT()
#endif

#ifndef DBG_DCM_DSPINTERNAL_CANCELPROCESSING_ENTRY
/** \brief Entry point of function Dcm_DspInternal_CancelProcessing() */
#define DBG_DCM_DSPINTERNAL_CANCELPROCESSING_ENTRY()
#endif

#ifndef DBG_DCM_DSPINTERNAL_CANCELPROCESSING_EXIT
/** \brief Exit point of function Dcm_DspInternal_CancelProcessing() */
#define DBG_DCM_DSPINTERNAL_CANCELPROCESSING_EXIT()
#endif

#ifndef DBG_DCM_DSPINTERNAL_DCMCONFIRMATION_ENTRY
/** \brief Entry point of function Dcm_DspInternal_DcmConfirmation() */
#define DBG_DCM_DSPINTERNAL_DCMCONFIRMATION_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_DCMCONFIRMATION_EXIT
/** \brief Entry point of function Dcm_DspInternal_DcmConfirmation() */
#define DBG_DCM_DSPINTERNAL_DCMCONFIRMATION_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_ENABLEPROCESSPAGE_ENTRY
/** \brief Entry point of function Dcm_DspInternal_EnableProcessPage() */
#define DBG_DCM_DSPINTERNAL_ENABLEPROCESSPAGE_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_ENABLEPROCESSPAGE_EXIT
/** \brief Exit point of function Dcm_DspInternal_EnableProcessPage() */
#define DBG_DCM_DSPINTERNAL_ENABLEPROCESSPAGE_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_PROCESSPAGEDTCSNAPSHOTRECORD_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ProcessPageDtcSnapshotRecord() */
#define DBG_DCM_DSPINTERNAL_PROCESSPAGEDTCSNAPSHOTRECORD_ENTRY(a)
#endif

#ifndef DBG_DCM_DSPINTERNAL_PROCESSPAGEDTCSNAPSHOTRECORD_EXIT
/** \brief Exit point of function Dcm_DspInternal_ProcessPageDtcSnapshotRecord() */
#define DBG_DCM_DSPINTERNAL_PROCESSPAGEDTCSNAPSHOTRECORD_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_PROCESSPAGEDTCEXTDATARECORD_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ProcessPageDtcExtDataRecord() */
#define DBG_DCM_DSPINTERNAL_PROCESSPAGEDTCEXTDATARECORD_ENTRY(a)
#endif

#ifndef DBG_DCM_DSPINTERNAL_PROCESSPAGEDTCEXTDATARECORD_EXIT
/** \brief Exit point of function Dcm_DspInternal_ProcessPageDtcExtDataRecord() */
#define DBG_DCM_DSPINTERNAL_PROCESSPAGEDTCEXTDATARECORD_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_GETFILTEREDDTC_ENTRY
/** \brief Entry point of function Dcm_DspInternal_GetFilteredDTC() */
#define DBG_DCM_DSPINTERNAL_GETFILTEREDDTC_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_GETFILTEREDDTC_EXIT
/** \brief Exit point of function Dcm_DspInternal_GetFilteredDTC() */
#define DBG_DCM_DSPINTERNAL_GETFILTEREDDTC_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_READDID_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ReadDid() */
#define DBG_DCM_DSPINTERNAL_READDID_ENTRY(a,b,c,d)
#endif

#ifndef DBG_DCM_DSPINTERNAL_READDATAOFSINGLEDID_EXECUTE_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ReadSingleDid() */
#define DBG_DCM_DSPINTERNAL_READDATAOFSINGLEDID_EXECUTE_ENTRY(a,b,c,d)
#endif

#ifndef DBG_DCM_DSPINTERNAL_READDATAOFSINGLEDID_EXECUTE_EXIT
/** \brief Exit point of function Dcm_DspInternal_ReadSingleDid() */
#define DBG_DCM_DSPINTERNAL_READDATAOFSINGLEDID_EXECUTE_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_DCM_DSPINTERNAL_READDID_SIGNAL_CONDITIONCHECK_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ReadDid_Signal_ConditionCheck() */
#define DBG_DCM_DSPINTERNAL_READDID_SIGNAL_CONDITIONCHECK_ENTRY(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_READDID_SIGNAL_CONDITIONCHECK_EXIT
/** \brief Exit point of function Dcm_DspInternal_ReadDid_Signal_ConditionCheck() */
#define DBG_DCM_DSPINTERNAL_READDID_SIGNAL_CONDITIONCHECK_EXIT(a,b,c,d)
#endif

#ifndef DBG_DCM_DSPINTERNAL_READDID_SIGNAL_COPYTOBUFFER_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ReadDid_Signal_CopyToBuffer() */
#define DBG_DCM_DSPINTERNAL_READDID_SIGNAL_COPYTOBUFFER_ENTRY(a,b,c,d)
#endif

#ifndef DBG_DCM_DSPINTERNAL_READDID_SIGNAL_COPYTOBUFFER_EXIT
/** \brief Exit point of function Dcm_DspInternal_ReadDid_Signal_CopyToBuffer() */
#define DBG_DCM_DSPINTERNAL_READDID_SIGNAL_COPYTOBUFFER_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_DCM_DSPINTERNAL_READDIDSIGNALDATALENGTH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ReadDidSignalDataLength() */
#define DBG_DCM_DSPINTERNAL_READDIDSIGNALDATALENGTH_ENTRY(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_READDIDSIGNALDATALENGTH_EXIT
/** \brief Exit point of function Dcm_DspInternal_ReadDidSignalDataLength() */
#define DBG_DCM_DSPINTERNAL_READDIDSIGNALDATALENGTH_EXIT(a,b,c,d)
#endif

#ifndef DBG_DCM_DSPINTERNAL_READDIDSIGNALDATA_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ReadDidSignalData() */
#define DBG_DCM_DSPINTERNAL_READDIDSIGNALDATA_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_DCM_DSPINTERNAL_READDIDSIGNALDATA_EXIT
/** \brief Exit point of function Dcm_DspInternal_ReadDidSignalData() */
#define DBG_DCM_DSPINTERNAL_READDIDSIGNALDATA_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_GETDIDINDEX_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_GetDidIndex() */
#define DBG_DCM_DSP_DIDSERVICES_GETDIDINDEX_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_GETDIDINDEX_EXIT
/** \brief Entry point of function Dcm_Dsp_DidServices_GetDidIndex() */
#define DBG_DCM_DSP_DIDSERVICES_GETDIDINDEX_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_Init() */
#define DBG_DCM_DSP_DIDSERVICES_INIT_ENTRY()
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_INIT_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_Init() */
#define DBG_DCM_DSP_DIDSERVICES_INIT_EXIT()
#endif

#ifndef DBG_DCM_DSP_MIDSERVICES_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsp_MidServices_Init() */
#define DBG_DCM_DSP_MIDSERVICES_INIT_ENTRY()
#endif

#ifndef DBG_DCM_DSP_MIDSERVICES_INIT_EXIT
/** \brief Exit point of function Dcm_Dsp_MidServices_Init() */
#define DBG_DCM_DSP_MIDSERVICES_INIT_EXIT()
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_MEMORYCONDITIONCHECKS_ENTRY
/** \brief Entry point of function Dcm_DspInternal_MemoryConditionChecks() */
#define DBG_DCM_DSP_MEMORYSERVICES_MEMORYCONDITIONCHECKS_ENTRY(a,b,c)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_MEMORYCONDITIONCHECKS_EXIT
/** \brief Exit point of function Dcm_DspInternal_MemoryConditionChecks() */
#define DBG_DCM_DSP_MEMORYSERVICES_MEMORYCONDITIONCHECKS_EXIT(a,b,c,d)
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVEFROMSCHEDULERTABLE_ENTRY
/** \brief Entry point of function
    Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveFromSchedulerTable() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVEFROMSCHEDULERTABLE_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVEFROMSCHEDULERTABLE_EXIT
/** \brief Exit point of function
    Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveFromSchedulerTable() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVEFROMSCHEDULERTABLE_EXIT()
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVESECUREDFROMSCHEDULERTABLE_ENTRY
/** \brief Entry point of function
    Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveSecuredFromSchedulerTable() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVESECUREDFROMSCHEDULERTABLE_ENTRY()
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVESECUREDFROMSCHEDULERTABLE_EXIT
/** \brief Exit point of function
    Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveSecuredFromSchedulerTable() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVESECUREDFROMSCHEDULERTABLE_EXIT()
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVESAMPLE_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveSample() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVESAMPLE_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVESAMPLE_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveSample() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVESAMPLE_EXIT()
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_PROCESSTIMERSANDSAMPLEPDIDS_ENTRY
/** \brief Entry point of function
    Dcm_Dsp_ReadDataByPeriodicIdentifier_ProcessTimersAndSamplePDIDs() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_PROCESSTIMERSANDSAMPLEPDIDS_ENTRY()
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_TRANSMIT_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_Transmit() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_TRANSMIT_ENTRY()
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_TRANSMIT_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_Transmit() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_TRANSMIT_EXIT()
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_PROCESSPDID_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_ProcessPDID() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_PROCESSPDID_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_PROCESSPDID_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_ProcessPDID() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_PROCESSPDID_EXIT()
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_UUDTSETUP_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_UUDTSetup() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_UUDTSETUP_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_UUDTSETUP_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_UUDTSetup() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_UUDTSETUP_EXIT()
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_DUUDTSENDNEXTPDID_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_UUDTSendNextPDID() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_DUUDTSENDNEXTPDID_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_DUUDTSENDNEXTPDID_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_UUDTSendNextPDID() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_DUUDTSENDNEXTPDID_EXIT()
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVEUUDTSAMPLEONCONFIRMATION_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveUUDTSampleOnConfirmation() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVEUUDTSAMPLEONCONFIRMATION_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVEUUDTSAMPLEONCONFIRMATION_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_RemoveUUDTSampleOnConfirmation() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_REMOVEUUDTSAMPLEONCONFIRMATION_EXIT()
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_DIDCONDITIONCHECKSSESLEVEL_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_DidConditionChecksSesLevel() */
#define DBG_DCM_DSP_DIDSERVICES_DIDCONDITIONCHECKSSESLEVEL_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_DIDCONDITIONCHECKSSESLEVEL_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_DidConditionChecksSesLevel() */
#define DBG_DCM_DSP_DIDSERVICES_DIDCONDITIONCHECKSSESLEVEL_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_DIDCONDITIONCHECKSSECLEVEL_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_DidConditionChecksSecLevel() */
#define DBG_DCM_DSP_DIDSERVICES_DIDCONDITIONCHECKSSECLEVEL_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_DIDCONDITIONCHECKSSECLEVEL_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_DidConditionChecksSecLevel() */
#define DBG_DCM_DSP_DIDSERVICES_DIDCONDITIONCHECKSSECLEVEL_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_COPYSIGNALDATA_ENTRY
/** \brief Entry point of function Dcm_DspInternal_CopySignalData() */
#define DBG_DCM_DSPINTERNAL_COPYSIGNALDATA_ENTRY(a,b,c,d,e,f,g)
#endif

#ifndef DBG_DCM_DSPINTERNAL_COPYSIGNALDATA_EXIT
/** \brief Exit point of function Dcm_DspInternal_CopySignalData() */
#define DBG_DCM_DSPINTERNAL_COPYSIGNALDATA_EXIT(a,b,c,d,e,f,g)
#endif

#ifndef DBG_DCM_DSPINTERNAL_WRITEDIDREQUESTLENGTHCHK_ENTRY
/** \brief Entry point of function Dcm_DspInternal_WriteDidRequestLengthChk() */
#define DBG_DCM_DSPINTERNAL_WRITEDIDREQUESTLENGTHCHK_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_WRITEDIDREQUESTLENGTHCHK_EXIT
/** \brief Exit point of function Dcm_DspInternal_WriteDidRequestLengthChk() */
#define DBG_DCM_DSPINTERNAL_WRITEDIDREQUESTLENGTHCHK_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_WRITEDID_ENTRY
/** \brief Entry point of function Dcm_DspInternal_WriteDid() */
#define DBG_DCM_DSPINTERNAL_WRITEDID_ENTRY(a,b,c,d)
#endif

#ifndef DBG_DCM_DSPINTERNAL_WRITEDID_EXIT
/** \brief Exit point of function Dcm_DspInternal_WriteDid() */
#define DBG_DCM_DSPINTERNAL_WRITEDID_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_DCM_DSPINTERNAL_WRITEDIDSIGNAL_ENTRY
/** \brief Entry point of function Dcm_DspInternal_WriteDidSignal() */
#define DBG_DCM_DSPINTERNAL_WRITEDIDSIGNAL_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_DCM_DSPINTERNAL_WRITEDIDSIGNAL_EXIT
/** \brief Exit point of function Dcm_DspInternal_WriteDidSignal() */
#define DBG_DCM_DSPINTERNAL_WRITEDIDSIGNAL_EXIT(a,b,c,d,e,f,g)
#endif

#ifndef DBG_DCM_DSPINTERNAL_WRITEDIDASYNCCLIENTSERVER_ENTRY
/** \brief Entry point of function Dcm_DspInternal_WriteDidAsyncClientServer() */
#define DBG_DCM_DSPINTERNAL_WRITEDIDASYNCCLIENTSERVER_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_DCM_DSPINTERNAL_WRITEDIDASYNCCLIENTSERVER_EXIT
/** \brief Exit point of function Dcm_DspInternal_WriteDidAsyncClientServer() */
#define DBG_DCM_DSPINTERNAL_WRITEDIDASYNCCLIENTSERVER_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_DCM_DSPINTERNAL_WRITEDIDSYNCCLIENTSERVER_ENTRY
/** \brief Entry point of function Dcm_DspInternal_WriteDidSyncClientServer() */
#define DBG_DCM_DSPINTERNAL_WRITEDIDSYNCCLIENTSERVER_ENTRY(a,b,c,d)
#endif

#ifndef DBG_DCM_DSPINTERNAL_WRITEDIDSYNCCLIENTSERVER_EXIT
/** \brief Exit point of function Dcm_DspInternal_WriteDidSyncClientServer() */
#define DBG_DCM_DSPINTERNAL_WRITEDIDSYNCCLIENTSERVER_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_DCM_DSPINTERNAL_WRITEDIDSYNCFUNC_ENTRY
/** \brief Entry point of function Dcm_DspInternal_WriteDidSyncFunc() */
#define DBG_DCM_DSPINTERNAL_WRITEDIDSYNCFUNC_ENTRY(a,b,c,d)
#endif

#ifndef DBG_DCM_DSPINTERNAL_WRITEDIDSYNCFUNC_EXIT
/** \brief Exit point of function Dcm_DspInternal_WriteDidSyncFunc() */
#define DBG_DCM_DSPINTERNAL_WRITEDIDSYNCFUNC_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_DCM_DSPINTERNAL_WRITEDIDASYNCFUNC_ENTRY
/** \brief Entry point of function Dcm_DspInternal_WriteDidAsyncFunc() */
#define DBG_DCM_DSPINTERNAL_WRITEDIDASYNCFUNC_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_DCM_DSPINTERNAL_WRITEDIDASYNCFUNC_EXIT
/** \brief Exit point of function Dcm_DspInternal_WriteDidAsyncFunc() */
#define DBG_DCM_DSPINTERNAL_WRITEDIDASYNCFUNC_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_DCM_DSPINTERNAL_WRITENVMDATA_ENTRY
/** \brief Entry point of function Dcm_DspInternal_WriteNvmData() */
#define DBG_DCM_DSPINTERNAL_WRITENVMDATA_ENTRY(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_WRITENVMDATA_EXIT
/** \brief Exit point of function Dcm_DspInternal_WriteNvmData() */
#define DBG_DCM_DSPINTERNAL_WRITENVMDATA_EXIT(a,b,c,d)
#endif

#ifndef DBG_DCM_DSP_DIDSIGNALS_ENDIANNESS_CONVERSION_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidSignals_Endianness_Conversion() */
#define DBG_DCM_DSP_DIDSIGNALS_ENDIANNESS_CONVERSION_ENTRY(a,b,c)
#endif

#ifndef DBG_DCM_DSP_DIDSIGNALS_ENDIANNESS_CONVERSION_EXIT
/** \brief Exit point of function Dcm_Dsp_DidSignals_Endianness_Conversion() */
#define DBG_DCM_DSP_DIDSIGNALS_ENDIANNESS_CONVERSION_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_REQUESTDOWNLOAD_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_RequestDownload_SvcH() */
#define DBG_DCM_DSP_REQUESTDOWNLOAD_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_REQUESTDOWNLOAD_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_RequestDownload_SvcH() */
#define DBG_DCM_DSP_REQUESTDOWNLOAD_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_REQUESTUPLOAD_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_RequestUpload_SvcH() */
#define DBG_DCM_DSP_REQUESTUPLOAD_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_REQUESTUPLOAD_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_RequestUpload_SvcH() */
#define DBG_DCM_DSP_REQUESTUPLOAD_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_TRANSFERDATA_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_TransferData_SvcH() */
#define DBG_DCM_DSP_TRANSFERDATA_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_TRANSFERDATA_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_TransferData_SvcH() */
#define DBG_DCM_DSP_TRANSFERDATA_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_LINKCONTROL_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_LinkControl_SvcH() */
#define DBG_DCM_DSP_LINKCONTROL_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_LINKCONTROL_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_LinkControl_SvcH() */
#define DBG_DCM_DSP_LINKCONTROL_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_LINKCONTROL_SVCSTART_ENTRY
/** \brief Entry point of function Dcm_Dsp_LinkControl_SvcStart() */
#define DBG_DCM_DSP_LINKCONTROL_SVCSTART_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_LINKCONTROL_FIXEDBAUDRATEVERIFICATION_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_LinkControl_FixedBaudrateVerification_SSvcH() */
#define DBG_DCM_DSP_LINKCONTROL_FIXEDBAUDRATEVERIFICATION_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_LINKCONTROL_FIXEDBAUDRATEVERIFICATION_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_LinkControl_FixedBaudrateVerification_SSvcH() */
#define DBG_DCM_DSP_LINKCONTROL_FIXEDBAUDRATEVERIFICATION_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_LINKCONTROL_SPECIFICBAUDRATEVERIFICATION_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_LinkControl_SpecificBaudrateVerification_SSvcH() */
#define DBG_DCM_DSP_LINKCONTROL_SPECIFICBAUDRATEVERIFICATION_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_LINKCONTROL_SPECIFICBAUDRATEVERIFICATION_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_LinkControl_SpecificBaudrateVerification_SSvcH() */
#define DBG_DCM_DSP_LINKCONTROL_SPECIFICBAUDRATEVERIFICATION_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_LINKCONTROL_TRANSITIONBAUDRATE_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_LinkControl_TransitionBaudrate_SSvcH() */
#define DBG_DCM_DSP_LINKCONTROL_TRANSITIONBAUDRATE_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_LINKCONTROL_TRANSITIONBAUDRATE_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_LinkControl_TransitionBaudrate_SSvcH() */
#define DBG_DCM_DSP_LINKCONTROL_TRANSITIONBAUDRATE_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_READMEMORYBYADDRESS_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadMemoryByAddress_SvcH() */
#define DBG_DCM_DSP_READMEMORYBYADDRESS_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_READMEMORYBYADDRESS_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadMemoryByAddress_SvcH() */
#define DBG_DCM_DSP_READMEMORYBYADDRESS_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_GETRANGE_ENTRY
/** \brief Entry point of function Dcm_Dsp_MemoryServices_ReadMemory_GetRange() */
#define DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_GETRANGE_ENTRY(a,b,c,d)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_GETRANGE_EXIT
/** \brief Exit point of function Dcm_Dsp_MemoryServices_ReadMemory_GetRange() */
#define DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_GETRANGE_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_DCM_DSP_WRITEMEMORYBYADDRESS_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_WriteMemoryByAddress_SvcH() */
#define DBG_DCM_DSP_WRITEMEMORYBYADDRESS_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_WRITEMEMORYBYADDRESS_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_WriteMemoryByAddress_SvcH() */
#define DBG_DCM_DSP_WRITEMEMORYBYADDRESS_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_CANCEL_ENTRY
/** \brief Entry point of function Dcm_Dsp_MemoryServices_WriteMemory_Cancel() */
#define DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_CANCEL_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_CANCEL_EXIT
/** \brief Entry point of function Dcm_Dsp_MemoryServices_WriteMemory_Cancel() */
#define DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_CANCEL_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_CONFIRMFORCEDRCRRP_ENTRY
/** \brief Entry point of function Dcm_Dsp_MemoryServices_WriteMemory_ConfirmForcedRCRRP() */
#define DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_CONFIRMFORCEDRCRRP_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_CONFIRMFORCEDRCRRP_EXIT
/** \brief Entry point of function Dcm_Dsp_MemoryServices_WriteMemory_ConfirmForcedRCRRP() */
#define DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_CONFIRMFORCEDRCRRP_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_EXECUTE_ENTRY
/** \brief Entry point of function Dcm_Dsp_MemoryServices_WriteMemory_Execute() */
#define DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_EXECUTE_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_EXECUTE_EXIT
/** \brief Entry point of function Dcm_Dsp_MemoryServices_WriteMemory_Execute() */
#define DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_EXECUTE_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_GETRESULTS_ENTRY
/** \brief Entry point of function Dcm_Dsp_MemoryServices_WriteMemory_GetResults() */
#define DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_GETRESULTS_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_GETRESULTS_EXIT
/** \brief Entry point of function Dcm_Dsp_MemoryServices_WriteMemory_GetResults() */
#define DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_GETRESULTS_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_MEMORY_SERVICES_WRITEMEMORY_SETUP_ENTRY
/** \brief Entry point of function Dcm_Dsp_MemoryServices_WriteMemory_Setup() */
#define DBG_DCM_DSP_MEMORY_SERVICES_WRITEMEMORY_SETUP_ENTRY(a, b, c)
#endif

#ifndef DBG_DCM_DSP_MEMORY_SERVICES_WRITEMEMORY_SETUP_EXIT
/** \brief Entry point of function Dcm_Dsp_MemoryServices_WriteMemory_Setup() */
#define DBG_DCM_DSP_MEMORY_SERVICES_WRITEMEMORY_SETUP_EXIT()
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_GETRANGE_ENTRY
/** \brief Entry point of function Dcm_Dsp_MemoryServices_WriteMemory_GetRange() */
#define DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_GETRANGE_ENTRY(a,b,c,d)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_GETRANGE_EXIT
/** \brief Exit point of function Dcm_Dsp_MemoryServices_WriteMemory_GetRange() */
#define DBG_DCM_DSP_MEMORYSERVICES_WRITEMEMORY_GETRANGE_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_GETMEMORYADDRESSANDSIZE_ENTRY
/** \brief Entry point of function Dcm_Dsp_MemoryServices_GetMemoryAddressAndSize() */
#define DBG_DCM_DSP_MEMORYSERVICES_GETMEMORYADDRESSANDSIZE_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_GETMEMORYADDRESSANDSIZE_EXIT
/** \brief Exit point of function Dcm_Dsp_MemoryServices_GetMemoryAddressAndSize() */
#define DBG_DCM_DSP_MEMORYSERVICES_GETMEMORYADDRESSANDSIZE_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_CHECKSECURITY_ENTRY
/** \brief Entry point of function Dcm_Dsp_MemoryServices_CheckSecurity() */
#define DBG_DCM_DSP_MEMORYSERVICES_CHECKSECURITY_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_CHECKSECURITY_EXIT
/** \brief Exit point of function Dcm_Dsp_MemoryServices_CheckSecurity() */
#define DBG_DCM_DSP_MEMORYSERVICES_CHECKSECURITY_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_ADDRESSLENGTHANDFORMAT_VERIFICATION_ENTRY
/** \brief Entry point of function Dcm_Dsp_MemoryServices_AddressLengthAndFormat_Verification() */
#define DBG_DCM_DSP_MEMORYSERVICES_ADDRESSLENGTHANDFORMAT_VERIFICATION_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_ADDRESSLENGTHANDFORMAT_VERIFICATION_EXIT
/** \brief Exit point of function Dcm_Dsp_MemoryServices_AddressLengthAndFormat_Verification() */
#define DBG_DCM_DSP_MEMORYSERVICES_ADDRESSLENGTHANDFORMAT_VERIFICATION_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSP_REQUESTONBOARDMONITORINGTESTRESULTS_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_RequestOnBoardMonitoringTestResults_SvcH() */
#define DBG_DCM_DSP_REQUESTONBOARDMONITORINGTESTRESULTS_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_REQUESTONBOARDMONITORINGTESTRESULTS_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_RequestOnBoardMonitoringTestResults_SvcH() */
#define DBG_DCM_DSP_REQUESTONBOARDMONITORINGTESTRESULTS_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REQUESTCONFIRMEDDTC_SVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_RequestConfirmedDTC_SvcH() */
#define DBG_DCM_DSPINTERNAL_REQUESTCONFIRMEDDTC_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REQUESTCONFIRMEDDTC_SVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_RequestConfirmedDTC_SvcH() */
#define DBG_DCM_DSPINTERNAL_REQUESTCONFIRMEDDTC_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REQUESTPENDINGDTC_SVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_RequestPendingDTC_SvcH() */
#define DBG_DCM_DSPINTERNAL_REQUESTPENDINGDTC_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REQUESTPENDINGDTC_SVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_RequestPendingDTC_SvcH() */
#define DBG_DCM_DSPINTERNAL_REQUESTPENDINGDTC_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REQUESTPERMANENTDTC_SVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_RequestPermanentDTC_SvcH() */
#define DBG_DCM_DSPINTERNAL_REQUESTPERMANENTDTC_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REQUESTPERMANENTDTC_SVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_RequestPermanentDTC_SvcH() */
#define DBG_DCM_DSPINTERNAL_REQUESTPERMANENTDTC_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_HSMINIT_ENTRY
/** \brief Entry point of function Dcm_HsmInit() */
#define DBG_DCM_HSMINIT_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMINIT_EXIT
/** \brief Exit point of function Dcm_HsmInit() */
#define DBG_DCM_HSMINIT_EXIT(a)
#endif

#ifndef DBG_DCM_HSMINITINST_ENTRY
/** \brief Entry point of function DCM_HSMINITINST() */
#define DBG_DCM_HSMINITINST_ENTRY(a,b)
#endif

#ifndef DBG_DCM_HSMINITINST_EXIT
/** \brief Exit point of function DCM_HSMINITINST() */
#define DBG_DCM_HSMINITINST_EXIT(a,b)
#endif

#ifndef DBG_DCM_HSMEMITINST_ENTRY
/** \brief Entry point of function DCM_HSMEMITINST() */
#define DBG_DCM_HSMEMITINST_ENTRY(a,b,c)
#endif

#ifndef DBG_DCM_HSMEMITINST_EXIT
/** \brief Exit point of function DCM_HSMEMITINST() */
#define DBG_DCM_HSMEMITINST_EXIT(a,b,c,d)
#endif

#ifndef DBG_DCM_HSMEMITTOSELFINST_ENTRY
/** \brief Entry point of function DCM_HSMEMITTOSELFINST() */
#define DBG_DCM_HSMEMITTOSELFINST_ENTRY(a,b,c)
#endif

#ifndef DBG_DCM_HSMEMITTOSELFINST_EXIT
/** \brief Exit point of function DCM_HSMEMITTOSELFINST() */
#define DBG_DCM_HSMEMITTOSELFINST_EXIT(a,b,c,d)
#endif

#ifndef DBG_DCM_HSMMAIN_ENTRY
/** \brief Entry point of function Dcm_HsmMain() */
#define DBG_DCM_HSMMAIN_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMMAIN_EXIT
/** \brief Exit point of function Dcm_HsmMain() */
#define DBG_DCM_HSMMAIN_EXIT(a)
#endif

#ifndef DBG_DCM_HSMMAININST_ENTRY
/** \brief Entry point of function DCM_HSMMAININST() */
#define DBG_DCM_HSMMAININST_ENTRY(a,b)
#endif

#ifndef DBG_DCM_HSMMAININST_EXIT
/** \brief Exit point of function DCM_HSMMAININST() */
#define DBG_DCM_HSMMAININST_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_HSMSETTRACING_ENTRY
/** \brief Entry point of function Dcm_HsmSetTracing() */
#define DBG_DCM_HSMSETTRACING_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMSETTRACING_EXIT
/** \brief Exit point of function Dcm_HsmSetTracing() */
#define DBG_DCM_HSMSETTRACING_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFTOPENTRY_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfTopEntry() */
#define DBG_DCM_HSMDCMSFTOPENTRY_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFTOPENTRY_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfTopEntry() */
#define DBG_DCM_HSMDCMSFTOPENTRY_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORFULLCOMGUARD1_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfWaitForFullComGuard1() */
#define DBG_DCM_HSMDCMSFWAITFORFULLCOMGUARD1_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORFULLCOMGUARD1_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfWaitForFullComGuard1() */
#define DBG_DCM_HSMDCMSFWAITFORFULLCOMGUARD1_EXIT(a, b)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORFULLCOMGUARD2_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfWaitForFullComGuard2() */
#define DBG_DCM_HSMDCMSFWAITFORFULLCOMGUARD2_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORFULLCOMGUARD2_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfWaitForFullComGuard2() */
#define DBG_DCM_HSMDCMSFWAITFORFULLCOMGUARD2_EXIT(a, b)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORFULLCOMACTION1_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfWaitForFullComAction1() */
#define DBG_DCM_HSMDCMSFWAITFORFULLCOMACTION1_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORFULLCOMACTION1_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfWaitForFullComAction1() */
#define DBG_DCM_HSMDCMSFWAITFORFULLCOMACTION1_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORFULLCOMACTION2_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfWaitForFullComAction2() */
#define DBG_DCM_HSMDCMSFWAITFORFULLCOMACTION2_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORFULLCOMACTION2_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfWaitForFullComAction2() */
#define DBG_DCM_HSMDCMSFWAITFORFULLCOMACTION2_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEENTRY_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfOutOfServiceEntry() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEENTRY_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEENTRY_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfOutOfServiceEntry() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEENTRY_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEACTION2_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfOutOfServiceAction2() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEACTION2_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEACTION2_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfOutOfServiceAction2() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEACTION2_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEACTION4_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfOutOfServiceAction4() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEACTION4_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEACTION4_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfOutOfServiceAction4() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEACTION4_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEACTION4_EXIT
/** \brief Entry point of function Dcm_HsmDcmSfOutOfServiceGuard4() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEACTION4_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD4_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfOutOfServiceGuard4() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD4_EXIT(a, b)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEACTION7_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfOutOfServiceAction7() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEACTION7_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEACTION7_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfOutOfServiceAction7() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEACTION7_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEACTION8_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfOutOfServiceAction8() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEACTION8_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEACTION8_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfOutOfServiceAction8() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEACTION8_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOOSINITACTION1_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfOosInitAction1() */
#define DBG_DCM_HSMDCMSFOOSINITACTION1_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOOSINITACTION1_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfOosInitAction1() */
#define DBG_DCM_HSMDCMSFOOSINITACTION1_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOOSNOTIFIEDENTRY_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfOosNotifiedEntry() */
#define DBG_DCM_HSMDCMSFOOSNOTIFIEDENTRY_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOOSNOTIFIEDENTRY_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfOosNotifiedEntry() */
#define DBG_DCM_HSMDCMSFOOSNOTIFIEDENTRY_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFINSERVICEENTRY_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfInServiceEntry() */
#define DBG_DCM_HSMDCMSFINSERVICEENTRY_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFINSERVICEENTRY_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfInServiceEntry() */
#define DBG_DCM_HSMDCMSFINSERVICEENTRY_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFINSERVICEACTION1_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfInServiceAction1() */
#define DBG_DCM_HSMDCMSFINSERVICEACTION1_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFINSERVICEACTION1_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfInServiceAction1() */
#define DBG_DCM_HSMDCMSFINSERVICEACTION1_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFCONFIRMINGNOKENTRY_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfConfirmingNOkEntry() */
#define DBG_DCM_HSMDCMSFCONFIRMINGNOKENTRY_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFCONFIRMINGNOKENTRY_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfConfirmingNOkEntry() */
#define DBG_DCM_HSMDCMSFCONFIRMINGNOKENTRY_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFCONFIRMINGOKENTRY_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfConfirmingOkEntry() */
#define DBG_DCM_HSMDCMSFCONFIRMINGOKENTRY_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFCONFIRMINGOKENTRY_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfConfirmingOkEntry() */
#define DBG_DCM_HSMDCMSFCONFIRMINGOKENTRY_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFSENDINGENTRY_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfSendingEntry() */
#define DBG_DCM_HSMDCMSFSENDINGENTRY_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFSENDINGENTRY_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfSendingEntry() */
#define DBG_DCM_HSMDCMSFSENDINGENTRY_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFPGBUFDATAAVAILABLEACTION1_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfPgBufDataAvailableAction1() */
#define DBG_DCM_HSMDCMSFPGBUFDATAAVAILABLEACTION1_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFPGBUFDATAAVAILABLEACTION1_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfPgBufDataAvailableAction1() */
#define DBG_DCM_HSMDCMSFPGBUFDATAAVAILABLEACTION1_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFSENDRPENTRY_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfSendRpEntry() */
#define DBG_DCM_HSMDCMSFSENDRPENTRY_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFSENDRPENTRY_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfSendRpEntry() */
#define DBG_DCM_HSMDCMSFSENDRPENTRY_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFSENDRPEXIT_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfSendRpExit() */
#define DBG_DCM_HSMDCMSFSENDRPEXIT_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFSENDRPEXIT_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfSendRpExit() */
#define DBG_DCM_HSMDCMSFSENDRPEXIT_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFSENDRPFORCEDACTION1_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfSendRpForcedAction1() */
#define DBG_DCM_HSMDCMSFSENDRPFORCEDACTION1_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFSENDRPFORCEDACTION1_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfSendRpForcedAction1() */
#define DBG_DCM_HSMDCMSFSENDRPFORCEDACTION1_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_REQUESTCURRENTPOWERTRAINDIAGNOSTICDATA_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_RequestCurrentPowertrainDiagnosticData_SvcH() */
#define DBG_DCM_DSP_REQUESTCURRENTPOWERTRAINDIAGNOSTICDATA_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_REQUESTCURRENTPOWERTRAINDIAGNOSTICDATA_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_RequestCurrentPowertrainDiagnosticData_SvcH() */
#define DBG_DCM_DSP_REQUESTCURRENTPOWERTRAINDIAGNOSTICDATA_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_REQUESTPOWERTRAINFREEZEFRAMEDATA_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_RequestPowertrainFreezeFrameData_SvcH() */
#define DBG_DCM_DSP_REQUESTPOWERTRAINFREEZEFRAMEDATA_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_REQUESTPOWERTRAINFREEZEFRAMEDATA_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_RequestPowertrainFreezeFrameData_SvcH() */
#define DBG_DCM_DSP_REQUESTPOWERTRAINFREEZEFRAMEDATA_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_REQUESTVEHICLEINFORMATION_SERVICEEXECUTION_ENTRY
/** \brief Entry point of function Dcm_Dsp_RequestVehicleInformation_ServiceExecution() */
#define DBG_DCM_DSP_REQUESTVEHICLEINFORMATION_SERVICEEXECUTION_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_REQUESTVEHICLEINFORMATION_SERVICEEXECUTION_EXIT
/** \brief Exit point of function Dcm_Dsp_RequestVehicleInformation_ServiceExecution() */
#define DBG_DCM_DSP_REQUESTVEHICLEINFORMATION_SERVICEEXECUTION_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_REQUESTVEHICLEINFORMATION_SVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_RequestVehicleInformation_SvcH() */
#define DBG_DCM_DSP_REQUESTVEHICLEINFORMATION_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_REQUESTVEHICLEINFORMATION_SVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_RequestVehicleInformation_SvcH() */
#define DBG_DCM_DSP_REQUESTVEHICLEINFORMATION_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_REQUESTVEHICLEINFORMATION_HELPERINIT_ENTRY
/** \brief Entry point of function Dcm_RequestVehicleInformation_HelperInit() */
#define DBG_DCM_REQUESTVEHICLEINFORMATION_HELPERINIT_ENTRY(a,b,c)
#endif

#ifndef DBG_DCM_REQUESTVEHICLEINFORMATION_HELPERINIT_EXIT
/** \brief Exit point of function Dcm_RequestVehicleInformation_HelperInit() */
#define DBG_DCM_REQUESTVEHICLEINFORMATION_HELPERINIT_EXIT(a,b,c,d)
#endif

#ifndef DBG_DCM_REQUESTVEHICLEINFORMATION_HELPEREXECUTE_ENTRY
/** \brief Entry point of function Dcm_RequestVehicleInformation_HelperExecute() */
#define DBG_DCM_REQUESTVEHICLEINFORMATION_HELPEREXECUTE_ENTRY(a,b,c)
#endif

#ifndef DBG_DCM_REQUESTVEHICLEINFORMATION_HELPEREXECUTE_EXIT
/** \brief Exit point of function Dcm_RequestVehicleInformation_HelperExecute() */
#define DBG_DCM_REQUESTVEHICLEINFORMATION_HELPEREXECUTE_EXIT(a,b,c,d)
#endif

#ifndef DBG_DCM_REQUESTVEHICLEINFORMATION_HELPEREXECUTEAVAILABLEINFOTYPE_ENTRY
/** \brief Entry point of function
    Dcm_RequestVehicleInformation_HelperExecuteAvailableInfoType() */
#define DBG_DCM_REQUESTVEHICLEINFORMATION_HELPEREXECUTEAVAILABLEINFOTYPE_ENTRY(a,b)
#endif

#ifndef DBG_DCM_REQUESTVEHICLEINFORMATION_HELPEREXECUTEAVAILABLEINFOTYPE_EXIT
/** \brief Exit point of function
    Dcm_RequestVehicleInformation_HelperExecuteAvailableInfoType() */
#define DBG_DCM_REQUESTVEHICLEINFORMATION_HELPEREXECUTEAVAILABLEINFOTYPE_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_REQUESTVEHICLEINFORMATION_HELPEREXECUTENONAVAILABLEINFOTYPE_ENTRY
/** \brief Entry point of function
    Dcm_RequestVehicleInformation_HelperExecuteNonAvailableInfoType() */
#define DBG_DCM_REQUESTVEHICLEINFORMATION_HELPEREXECUTENONAVAILABLEINFOTYPE_ENTRY(a,b)
#endif

#ifndef DBG_DCM_REQUESTVEHICLEINFORMATION_HELPEREXECUTENONAVAILABLEINFOTYPE_EXIT
/** \brief Exit point of function
    Dcm_RequestVehicleInformation_HelperExecuteNonAvailableInfoType() */
#define DBG_DCM_REQUESTVEHICLEINFORMATION_HELPEREXECUTENONAVAILABLEINFOTYPE_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_REQUESTVEHICLEINFORMATION_HELPERCANCEL_ENTRY
/** \brief Entry point of function Dcm_RequestVehicleInformation_HelperCancel() */
#define DBG_DCM_REQUESTVEHICLEINFORMATION_HELPERCANCEL_ENTRY(a)
#endif

#ifndef DBG_DCM_REQUESTVEHICLEINFORMATION_HELPERCANCEL_EXIT
/** \brief Exit point of function Dcm_RequestVehicleInformation_HelperCancel() */
#define DBG_DCM_REQUESTVEHICLEINFORMATION_HELPERCANCEL_EXIT(a)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REQUESTCONTROLOFONBOARDSYSTSTCOMP_SVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_RequestControlOfOnBoardSysTstComp_SvcH() */
#define DBG_DCM_DSPINTERNAL_REQUESTCONTROLOFONBOARDSYSTSTCOMP_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REQUESTCONTROLOFONBOARDSYSTSTCOMP_SVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_RequestControlOfOnBoardSysTstComp_SvcH() */
#define DBG_DCM_DSPINTERNAL_REQUESTCONTROLOFONBOARDSYSTSTCOMP_SVCH_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REQCTRLVIAROUTINECTRL_UDSSHELL_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ReqCtrlViaRoutineCtrl_UDSShell() */
#define DBG_DCM_DSPINTERNAL_REQCTRLVIAROUTINECTRL_UDSSHELL_ENTRY(a)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REQCTRLVIAROUTINECTRL_UDSSHELL_EXIT
/** \brief Exit point of function Dcm_DspInternal_ReqCtrlViaRoutineCtrl_UDSShell() */
#define DBG_DCM_DSPINTERNAL_REQCTRLVIAROUTINECTRL_UDSSHELL_EXIT()
#endif

#ifndef DBG_DCM_DSP_MIDSERVICES_GETAVAILABILITYMID_ENTRY
/** \brief Entry point of function Dcm_Dsp_MidServices_GetAvailabilityMid() */
#define DBG_DCM_DSP_MIDSERVICES_GETAVAILABILITYMID_ENTRY(a, b, c, d, e)
#endif

#ifndef DBG_DCM_DSP_MIDSERVICES_GETAVAILABILITYMID_EXIT
/** \brief Exit point of function Dcm_Dsp_MidServices_GetAvailabilityMid() */
#define DBG_DCM_DSP_MIDSERVICES_GETAVAILABILITYMID_EXIT(a, b, c, d, e, f)
#endif

#ifndef DBG_DCM_DSP_DCM_DSP_MIDSERVICES_GETNONAVAILABILITYMID_ENTRY
/** \brief Entry point of function Dcm_Dsp_MidServices_GetNonAvailabilityMid() */
#define DBG_DCM_DSP_DCM_DSP_MIDSERVICES_GETNONAVAILABILITYMID_ENTRY(a, b, c, d, e, f)
#endif

#ifndef DBG_DCM_DSP_DCM_DSP_MIDSERVICES_GETNONAVAILABILITYMID_EXIT
/** \brief Exit point of function Dcm_Dsp_MidServices_GetNonAvailabilityMid() */
#define DBG_DCM_DSP_DCM_DSP_MIDSERVICES_GETNONAVAILABILITYMID_EXIT(a, b, c, d, e, f, g)
#endif



#ifndef DBG_DCM_DSL_RXCONNECTIONGETRXPDUIDINFORMATION_ENTRY
/** \brief Entry point of function Dcm_Dsl_RxConnectionGetRxPduIdInformation() */
#define DBG_DCM_DSL_RXCONNECTIONGETRXPDUIDINFORMATION_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSL_RXCONNECTIONGETRXPDUIDINFORMATION_EXIT
/** \brief Exit point of function Dcm_Dsl_RxConnectionGetRxPduIdInformation() */
#define DBG_DCM_DSL_RXCONNECTIONGETRXPDUIDINFORMATION_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_REQUESTACTIVEDIAGNOSTIC_ENTRY
/** \brief Entry point of function Dcm_Dsl_ComMChannelStateManager_RequestActiveDiagnostic() */
#define DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_REQUESTACTIVEDIAGNOSTIC_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_REQUESTACTIVEDIAGNOSTIC_EXIT
/** \brief Exit point of function Dcm_Dsl_ComMChannelStateManager_RequestActiveDiagnostic() */
#define DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_REQUESTACTIVEDIAGNOSTIC_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_REQUESTINACTIVEDIAGNOSTIC_ENTRY
/** \brief Entry point of function Dcm_Dsl_ComMChannelStateManager_RequestInactiveDiagnostic() */
#define DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_REQUESTINACTIVEDIAGNOSTIC_ENTRY()
#endif

#ifndef DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_REQUESTINACTIVEDIAGNOSTIC_EXIT
/** \brief Exit point of function Dcm_Dsl_ComMChannelStateManager_RequestInactiveDiagnostic() */
#define DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_REQUESTINACTIVEDIAGNOSTIC_EXIT()
#endif

#ifndef DBG_DCM_DSP_CONTEXT_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsp_Context_Init() */
#define DBG_DCM_DSP_CONTEXT_INIT_ENTRY(a, b, c)
#endif

#ifndef DBG_DCM_DSP_CONTEXT_INIT_EXIT
/** \brief Exit point of function Dcm_Dsp_Context_Init() */
#define DBG_DCM_DSP_CONTEXT_INIT_EXIT(a, b, c)
#endif

#ifndef DBG_DCM_DSP_CONTEXT_EXECUTE_ENTRY
/** \brief Entry point of function Dcm_Dsp_Context_Execute() */
#define DBG_DCM_DSP_CONTEXT_EXECUTE_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_CONTEXT_EXECUTE_EXIT
/** \brief Exit point of function Dcm_Dsp_Context_Execute() */
#define DBG_DCM_DSP_CONTEXT_EXECUTE_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_CONTEXT_CANCEL_ENTRY
/** \brief Entry point of function Dcm_Dsp_Context_Cancel() */
#define DBG_DCM_DSP_CONTEXT_CANCEL_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_CONTEXT_CANCEL_EXIT
/** \brief Exit point of function Dcm_Dsp_Context_Cancel() */
#define DBG_DCM_DSP_CONTEXT_CANCEL_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsp_CommunicationControl_Init() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_INIT_ENTRY()
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_INIT_EXIT
/** \brief Exit point of function Dcm_Dsp_CommunicationControl_Init() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_INIT_EXIT()
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsl_Supervisor_Init() */
#define DBG_DCM_DSL_SUPERVISOR_INIT_ENTRY()
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_INIT_EXIT
/** \brief Exit point of function Dcm_Dsl_Supervisor_Init() */
#define DBG_DCM_DSL_SUPERVISOR_INIT_EXIT()
#endif

#ifndef DBG_DCM_DSL_TXCONNECTIONPROCESSNEXTTXBUFFER_ENTRY
/** \brief Entry point of function Dcm_Dsl_TxConnectionProcessNextTxBuffer() */
#define DBG_DCM_DSL_TXCONNECTIONPROCESSNEXTTXBUFFER_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSL_TXCONNECTIONPROCESSNEXTTXBUFFER_EXIT
/** \brief Exit point of function Dcm_Dsl_TxConnectionProcessNextTxBuffer() */
#define DBG_DCM_DSL_TXCONNECTIONPROCESSNEXTTXBUFFER_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSL_GETDEMCLIENTID_ENTRY
/** \brief Entry point of function Dcm_Dsl_GetDemClientId() */
#define DBG_DCM_DSL_GETDEMCLIENTID_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_GETDEMCLIENTID_EXIT
/** \brief Exit point of function Dcm_Dsl_GetDemClientId() */
#define DBG_DCM_DSL_GETDEMCLIENTID_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsl_ComMChannelStateManager_Init() */
#define DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_INIT_ENTRY()
#endif

#ifndef DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_INIT_EXIT
/** \brief Exit point of function Dcm_Dsl_ComMChannelStateManager_Init() */
#define DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_INIT_EXIT()
#endif

#ifndef DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_ENTERMODE_ENTRY
/** \brief Entry point of function Dcm_Dsl_ComMChannelStateManager_EnterMode() */
#define DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_ENTERMODE_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_ENTERMODE_EXIT
/** \brief Exit point of function Dcm_Dsl_ComMChannelStateManager_EnterMode() */
#define DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_ENTERMODE_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_TRANSMISSIONFINISHED_ENTRY
/** \brief Entry point of function Dcm_Dsl_Supervisor_TransmissionFinished() */
#define DBG_DCM_DSL_SUPERVISOR_TRANSMISSIONFINISHED_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_TRANSMISSIONFINISHED_EXIT
/** \brief Exit point of function Dcm_Dsl_Supervisor_TransmissionFinished() */
#define DBG_DCM_DSL_SUPERVISOR_TRANSMISSIONFINISHED_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_EXTRACTSUBSERVICEHANDLER_ENTRY
/** \brief Entry point of function Dcm_Dsp_ExtractSubServiceHandler() */
#define DBG_DCM_DSP_EXTRACTSUBSERVICEHANDLER_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSP_EXTRACTSUBSERVICEHANDLER_EXIT
/** \brief Exit point of function Dcm_Dsp_ExtractSubServiceHandler() */
#define DBG_DCM_DSP_EXTRACTSUBSERVICEHANDLER_EXIT(a, b, c)
#endif

#ifndef DBG_DCM_DSP_CANCELLATIONDONE_ENTRY
/** \brief Entry point of function Dcm_Dsp_CancellationDone() */
#define DBG_DCM_DSP_CANCELLATIONDONE_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSP_CANCELLATIONDONE_EXIT
/** \brief Exit point of function Dcm_Dsp_CancellationDone() */
#define DBG_DCM_DSP_CANCELLATIONDONE_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSL_RXCONNECTIONRXINDICATION_ENTRY
/** \brief Entry point of function Dcm_Dsl_RxConnectionRxIndication() */
#define DBG_DCM_DSL_RXCONNECTIONRXINDICATION_ENTRY(a, b, c)
#endif

#ifndef DBG_DCM_DSL_RXCONNECTIONRXINDICATION_EXIT
/** \brief Exit point of function Dcm_Dsl_RxConnectionRxIndication() */
#define DBG_DCM_DSL_RXCONNECTIONRXINDICATION_EXIT(a, b, c)
#endif

#ifndef DBG_DCM_DSL_RXCONNECTIONCANCELRECEIVE_ENTRY
/** \brief Entry point of function Dcm_Dsl_RxConnectionCancelReceive() */
#define DBG_DCM_DSL_RXCONNECTIONCANCELRECEIVE_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_RXCONNECTIONCANCELRECEIVE_EXIT
/** \brief Exit point of function Dcm_Dsl_RxConnectionCancelReceive() */
#define DBG_DCM_DSL_RXCONNECTIONCANCELRECEIVE_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_REQUESTRXPERMISSION_ENTRY
/** \brief Entry point of function Dcm_Dsl_Supervisor_RequestRxPermission() */
#define DBG_DCM_DSL_SUPERVISOR_REQUESTRXPERMISSION_ENTRY(a, b, c)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_REQUESTRXPERMISSION_EXIT
/** \brief Exit point of function Dcm_Dsl_Supervisor_RequestRxPermission() */
#define DBG_DCM_DSL_SUPERVISOR_REQUESTRXPERMISSION_EXIT(a, b, c, d)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_PROCESSREQUEST_ENTRY
/** \brief Entry point of function Dcm_Dsl_Supervisor_ProcessRequest() */
#define DBG_DCM_DSL_SUPERVISOR_PROCESSREQUEST_ENTRY(a, b, c)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_PROCESSREQUEST_EXIT
/** \brief Exit point of function Dcm_Dsl_Supervisor_ProcessRequest() */
#define DBG_DCM_DSL_SUPERVISOR_PROCESSREQUEST_EXIT(a, b, c)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_PROTOCOLProtocolFree_ENTRY
/** \brief Entry point of function Dcm_Dsl_Supervisor_ProtocolProtocolFree() */
#define DBG_DCM_DSL_SUPERVISOR_PROTOCOLProtocolFree_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_PROTOCOLProtocolFree_EXIT
/** \brief Exit point of function Dcm_Dsl_Supervisor_ProtocolProtocolFree() */
#define DBG_DCM_DSL_SUPERVISOR_PROTOCOLProtocolFree_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_GETCURRENTPROTOCOL_ENTRY
/** \brief Entry point of function Dcm_Dsl_Supervisor_GetCurrentProtocol() */
#define DBG_DCM_DSL_SUPERVISOR_GETCURRENTPROTOCOL_ENTRY()
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_GETCURRENTPROTOCOL_EXIT
/** \brief Exit point of function Dcm_Dsl_Supervisor_GetCurrentProtocol() */
#define DBG_DCM_DSL_SUPERVISOR_GETCURRENTPROTOCOL_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_GETPROTOCOLINDEXFROMRXPDUIDANDREQUESTTYPE_ENTRY
/** \brief Entry point of function Dcm_Dsl_Supervisor_GetProtocolIndexFromRxPduIdAndRequestType() */
#define DBG_DCM_DSL_SUPERVISOR_GETPROTOCOLINDEXFROMRXPDUIDANDREQUESTTYPE_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_GETPROTOCOLINDEXFROMRXPDUIDANDREQUESTTYPE_EXIT
/** \brief Exit point of function Dcm_Dsl_Supervisor_GetProtocolIndexFromRxPduIdAndRequestType() */
#define DBG_DCM_DSL_SUPERVISOR_GETPROTOCOLINDEXFROMRXPDUIDANDREQUESTTYPE_EXIT(a, b, c)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_INCOMINGRECEPTION_ENTRY
/** \brief Entry point of function Dcm_Dsl_Supervisor_IncomingReception() */
#define DBG_DCM_DSL_SUPERVISOR_INCOMINGRECEPTION_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_INCOMINGRECEPTION_EXIT
/** \brief Exit point of function Dcm_Dsl_Supervisor_IncomingReception() */
#define DBG_DCM_DSL_SUPERVISOR_INCOMINGRECEPTION_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSL_TXCONNECTIONCANCELTRANSMIT_ENTRY
/** \brief Entry point of function Dcm_Dsl_TxConnectionCancelTransmit() */
#define DBG_DCM_DSL_TXCONNECTIONCANCELTRANSMIT_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_TXCONNECTIONCANCELTRANSMIT_EXIT
/** \brief Exit point of function Dcm_Dsl_TxConnectionCancelTransmit() */
#define DBG_DCM_DSL_TXCONNECTIONCANCELTRANSMIT_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsl_Init() */
#define DBG_DCM_DSL_INIT_ENTRY()
#endif

#ifndef DBG_DCM_DSL_INIT_EXIT
/** \brief Exit point of function Dcm_Dsl_Init() */
#define DBG_DCM_DSL_INIT_EXIT()
#endif

#ifndef DBG_DCM_DSL_TXCONNECTIONTRANSMIT_ENTRY
/** \brief Entry point of function Dcm_Dsl_TxConnectionTransmit() */
#define DBG_DCM_DSL_TXCONNECTIONTRANSMIT_ENTRY(a, b, c, d, e, f, g)
#endif

#ifndef DBG_DCM_DSL_TXCONNECTIONTRANSMIT_EXIT
/** \brief Exit point of function Dcm_Dsl_TxConnectionTransmit() */
#define DBG_DCM_DSL_TXCONNECTIONTRANSMIT_EXIT(a, b, c, d, e, f, g)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_MAINFUNCTION_ENTRY
/** \brief Entry point of function Dcm_Dsl_Supervisor_MainFunction() */
#define DBG_DCM_DSL_SUPERVISOR_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_MAINFUNCTION_EXIT
/** \brief Exit point of function Dcm_Dsl_Supervisor_MainFunction() */
#define DBG_DCM_DSL_SUPERVISOR_MAINFUNCTION_EXIT()
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_GETCURRENTPROTOCOLSTATE_ENTRY
/** \brief Entry point of function Dcm_Dsl_Supervisor_GetCurrentProtocolState() */
#define DBG_DCM_DSL_SUPERVISOR_GETCURRENTPROTOCOLSTATE_ENTRY()
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_GETCURRENTPROTOCOLSTATE_EXIT
/** \brief Exit point of function Dcm_Dsl_Supervisor_GetCurrentProtocolState() */
#define DBG_DCM_DSL_SUPERVISOR_GETCURRENTPROTOCOLSTATE_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_INCOMINGRECEPTIONFINISHED_ENTRY
/** \brief Entry point of function Dcm_Dsl_Supervisor_IncomingReceptionFinished() */
#define DBG_DCM_DSL_SUPERVISOR_INCOMINGRECEPTIONFINISHED_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_INCOMINGRECEPTIONFINISHED_EXIT
/** \brief Exit point of function Dcm_Dsl_Supervisor_IncomingReceptionFinished() */
#define DBG_DCM_DSL_SUPERVISOR_INCOMINGRECEPTIONFINISHED_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIER_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsp_DynamicallyDefineDataIdentifier_Init() */
#define DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIER_INIT_ENTRY()
#endif

#ifndef DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIER_INIT_EXIT
/** \brief Exit point of function Dcm_Dsp_DynamicallyDefineDataIdentifier_Init() */
#define DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIER_INIT_EXIT()
#endif

#ifndef DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIER_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_DynamicallyDefineDataIdentifier_SvcH() */
#define DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIER_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIER_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_DynamicallyDefineDataIdentifier_SvcH() */
#define DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIER_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYDID_SSVCCONTINUE_ENTRY
/** \brief Entry point of function
    Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByDid_SSvcContinue() */
#define DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYDID_SSVCCONTINUE_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYDID_SSVCCONTINUE_EXIT
/** \brief Exit point of function
    Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByDid_SSvcContinue() */
#define DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYDID_SSVCCONTINUE_EXIT(a, b, c)
#endif

#ifndef DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYDID_SSVCSTART_ENTRY
/** \brief Entry point of function
    Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByDid_SSvcStart() */
#define DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYDID_SSVCSTART_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYDID_SSVCSTART_EXIT
/** \brief Exit point of function
    Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByDid_SSvcStart() */
#define DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYDID_SSVCSTART_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYDID_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByDid_SSvcH() */
#define DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYDID_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYDID_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByDid_SSvcH() */
#define DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYDID_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_ALLOWROETOPOLLDIDDATA_ENTRY
/** \brief Entry point of function Dcm_AllowROEToPollDIDData() */
#define DBG_DCM_ALLOWROETOPOLLDIDDATA_ENTRY()
#endif

#ifndef DBG_DCM_ALLOWROETOPOLLDIDDATA_EXIT
/** \brief Exit point of function Dcm_AllowROEToPollDIDData() */
#define DBG_DCM_ALLOWROETOPOLLDIDDATA_EXIT()
#endif

#ifndef DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYMEM_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByMem_SSvcH() */
#define DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYMEM_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYMEM_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByMem_SSvcH() */
#define DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERADDSRCBYMEM_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERCLEAR_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_DynamicallyDefineDataIdentifierClear_SSvcH() */
#define DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERCLEAR_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERCLEAR_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_DynamicallyDefineDataIdentifierClear_SSvcH() */
#define DBG_DCM_DSP_DYNAMICALLYDEFINEDATAIDENTIFIERCLEAR_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_CONFIRMANDCHECKPENDINGCONFIRMATIONS_ENTRY
/** \brief Entry point of function
    Dcm_Dsp_ReadDataByPeriodicIdentifier_ConfirmAndCheckPendingConfirmations() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_CONFIRMANDCHECKPENDINGCONFIRMATIONS_ENTRY()
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_CONFIRMANDCHECKPENDINGCONFIRMATIONS_EXIT
/** \brief Exit point of function
    Dcm_Dsp_ReadDataByPeriodicIdentifier_ConfirmAndCheckPendingConfirmations() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_CONFIRMANDCHECKPENDINGCONFIRMATIONS_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_Init() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_INIT_ENTRY()
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_INIT_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_Init() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_INIT_EXIT()
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_FORCEREVALIDATE_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_ForceReValidate() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_FORCEREVALIDATE_ENTRY()
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_FORCEREVALIDATE_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_ForceReValidate() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_FORCEREVALIDATE_EXIT()
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_ISPERIODICTXCHANNELENABLED_ENTRY
/** \brief Entry point of function
    Dcm_Dsp_ReadDataByPeriodicIdentifier_IsPeriodicTxChannelEnabled() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_ISPERIODICTXCHANNELENABLED_ENTRY()
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_ISPERIODICTXCHANNELENABLED_EXIT
/** \brief Exit point of function
    Dcm_Dsp_ReadDataByPeriodicIdentifier_IsPeriodicTxChannelEnabled() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_ISPERIODICTXCHANNELENABLED_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_ISTYPE2PERIODICTXCONNECTION_ENTRY
/** \brief Entry point of function Dcm_Dsl_IsTYPE2PeriodicTxConnection() */
#define DBG_DCM_DSL_ISTYPE2PERIODICTXCONNECTION_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_ISTYPE2PERIODICTXCONNECTION_EXIT
/** \brief Exit point of function Dcm_Dsl_IsTYPE2PeriodicTxConnection() */
#define DBG_DCM_DSL_ISTYPE2PERIODICTXCONNECTION_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_READDID_CANCEL_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_ReadDid_Cancel() */
#define DBG_DCM_DSP_DIDSERVICES_READDID_CANCEL_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_READDID_CANCEL_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_ReadDid_Cancel() */
#define DBG_DCM_DSP_DIDSERVICES_READDID_CANCEL_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_GETDIDINFO_EXECUTE_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_GetDidInfo_Execute() */
#define DBG_DCM_DSP_DIDSERVICES_GETDIDINFO_EXECUTE_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_GETDIDINFO_EXECUTE_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_GetDidInfo_Execute() */
#define DBG_DCM_DSP_DIDSERVICES_GETDIDINFO_EXECUTE_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_GETDIDINFO_CANCEL_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_GetDidInfo_Cancel() */
#define DBG_DCM_DSP_DIDSERVICES_GETDIDINFO_CANCEL_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_GETDIDINFO_CANCEL_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_GetDidInfo_Cancel() */
#define DBG_DCM_DSP_DIDSERVICES_GETDIDINFO_CANCEL_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_READDID_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_ReadDid_Init() */
#define DBG_DCM_DSP_DIDSERVICES_READDID_INIT_ENTRY(a, b, c, d, e, f, g)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_READDID_INIT_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_ReadDid_Init() */
#define DBG_DCM_DSP_DIDSERVICES_READDID_INIT_EXIT(a, b, c, d, e, f, g)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_READSIGNAL_GETRESULTS_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_ReadSignal_GetResults() */
#define DBG_DCM_DSP_DIDSERVICES_READSIGNAL_GETRESULTS_ENTRY(a, b, c)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_READSIGNAL_GETRESULTS_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_ReadSignal_GetResults() */
#define DBG_DCM_DSP_DIDSERVICES_READSIGNAL_GETRESULTS_EXIT(a, b, c, d)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_WRITEDID_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_WriteDid_Init() */
#define DBG_DCM_DSP_DIDSERVICES_WRITEDID_INIT_ENTRY(a, b, c, d, e)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_WRITEDID_INIT_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_WriteDid_Init() */
#define DBG_DCM_DSP_DIDSERVICES_WRITEDID_INIT_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_WRITEDID_EXECUTE_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_WriteDid_Execute() */
#define DBG_DCM_DSP_DIDSERVICES_WRITEDID_EXECUTE_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_WRITEDID_EXECUTE_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_WriteDid_Execute() */
#define DBG_DCM_DSP_DIDSERVICES_WRITEDID_EXECUTE_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_WRITEDID_CANCEL_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_WriteDid_Cancel() */
#define DBG_DCM_DSP_DIDSERVICES_WRITEDID_CANCEL_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_WRITEDID_CANCEL_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_WriteDid_Cancel() */
#define DBG_DCM_DSP_DIDSERVICES_WRITEDID_CANCEL_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_LOCKSERVICE_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_LockService() */
#define DBG_DCM_DSP_DIDSERVICES_LOCKSERVICE_ENTRY()
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_LOCKSERVICE_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_LockService() */
#define DBG_DCM_DSP_DIDSERVICES_LOCKSERVICE_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_UNLOCKSERVICE_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_UnlockService() */
#define DBG_DCM_DSP_DIDSERVICES_UNLOCKSERVICE_ENTRY()
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_UNLOCKSERVICE_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_UnlockService() */
#define DBG_DCM_DSP_DIDSERVICES_UNLOCKSERVICE_EXIT()
#endif

#ifndef DBG_DCM_DSP_MIDSERVICES_LOCKSERVICES_ENTRY
/** \brief Entry point of function Dcm_Dsp_MidServices_LockServices() */
#define DBG_DCM_DSP_MIDSERVICES_LOCKSERVICES_ENTRY()
#endif

#ifndef DBG_DCM_DSP_MIDSERVICES_LOCKSERVICES_EXIT
/** \brief Exit point of function Dcm_Dsp_MidServices_LockServices() */
#define DBG_DCM_DSP_MIDSERVICES_LOCKSERVICES_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_MIDSERVICES_UNLOCKSERVICES_ENTRY
/** \brief Entry point of function Dcm_Dsp_MidServices_UnlockService() */
#define DBG_DCM_DSP_MIDSERVICES_UNLOCKSERVICES_ENTRY()
#endif

#ifndef DBG_DCM_DSP_MIDSERVICES_UNLOCKSERVICES_EXIT
/** \brief Exit point of function Dcm_Dsp_MidServices_UnlockServices() */
#define DBG_DCM_DSP_MIDSERVICES_UNLOCKSERVICES_EXIT()
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_GETDTCINFOBYSELECT_PROCESS_ENTRY
/** \brief Entry point of function Dcm_Dsp_DemFacade_GetDTCInfoBySelect_Process() */
#define DBG_DCM_DSP_DEMFACADE_GETDTCINFOBYSELECT_PROCESS_ENTRY(a, b, c)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_GETDTCINFOBYSELECT_PROCESS_EXIT
/** \brief Exit point of function Dcm_Dsp_DemFacade_GetDTCInfoBySelect_Process() */
#define DBG_DCM_DSP_DEMFACADE_GETDTCINFOBYSELECT_PROCESS_EXIT(a,b,c,d)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_GETDTCSEVERITYINFOBYSELECT_PROCESS_ENTRY
/** \brief Entry point of function Dcm_Dsp_DemFacade_GetDTCSeverityInfoBySelect_Process() */
#define DBG_DCM_DSP_DEMFACADE_GETDTCSEVERITYINFOBYSELECT_PROCESS_ENTRY(a, b, c)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_GETDTCSEVERITYINFOBYSELECT_PROCESS_EXIT
/** \brief Exit point of function Dcm_Dsp_DemFacade_GetDTCSeverityInfoBySelect_Process() */
#define DBG_DCM_DSP_DEMFACADE_GETDTCSEVERITYINFOBYSELECT_PROCESS_EXIT(a,b,c,d)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_GETDTCINFOBYFILTER_PROCESS_ENTRY
/** \brief Entry point of function Dcm_Dsp_DemFacade_GetDTCInfoByFilter_Process() */
#define DBG_DCM_DSP_DEMFACADE_GETDTCINFOBYFILTER_PROCESS_ENTRY(a,b,c)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_GETDTCINFOBYFILTER_PROCESS_EXIT
/** \brief Exit point of function Dcm_Dsp_DemFacade_GetDTCInfoByFilter_Process() */
#define DBG_DCM_DSP_DEMFACADE_GETDTCINFOBYFILTER_PROCESS_EXIT(a,b,c,d)
#endif

#ifndef DBG_DCM_INTERNAL_GETLOCK_ENTRY
/** \brief Entry point of function Dcm_Internal_GetLock() */
#define DBG_DCM_INTERNAL_GETLOCK_ENTRY(a)
#endif

#ifndef DBG_DCM_INTERNAL_GETLOCK_EXIT
/** \brief Exit point of function Dcm_Internal_GetLock() */
#define DBG_DCM_INTERNAL_GETLOCK_EXIT(a)
#endif

#ifndef DBG_DCM_INTERNAL_RELEASELOCK_ENTRY
/** \brief Entry point of function Dcm_Internal_ReleaseLock() */
#define DBG_DCM_INTERNAL_RELEASELOCK_ENTRY(a)
#endif

#ifndef DBG_DCM_INTERNAL_RELEASELOCK_EXIT
/** \brief Exit point of function Dcm_Internal_ReleaseLock() */
#define DBG_DCM_INTERNAL_RELEASELOCK_EXIT(a)
#endif

#ifndef DBG_DCM_DSPINTERNAL_UPDATEAMOUNTOFFILLEDDATAONRESPONSEBUFFER_ENTRY
/** \brief Entry point of function Dcm_DspInternal_UpdateAmountOfFilledDataOnResponseBuffer() */
#define DBG_DCM_DSPINTERNAL_UPDATEAMOUNTOFFILLEDDATAONRESPONSEBUFFER_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_UPDATEAMOUNTOFFILLEDDATAONRESPONSEBUFFER_EXIT
/** \brief Exit point of function Dcm_DspInternal_UpdateAmountOfFilledDataOnResponseBuffer() */
#define DBG_DCM_DSPINTERNAL_UPDATEAMOUNTOFFILLEDDATAONRESPONSEBUFFER_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_SETUP_ENTRY
/** \brief Entry point of function Dcm_Dsp_MemoryServices_ReadMemory_Setup() */
#define DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_SETUP_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_SETUP_EXIT
/** \brief Exit point of function Dcm_Dsp_MemoryServices_ReadMemory_Setup() */
#define DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_SETUP_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_EXECUTE_ENTRY
/** \brief Entry point of function Dcm_Dsp_MemoryServices_ReadMemory_Execute() */
#define DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_EXECUTE_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_EXECUTE_EXIT
/** \brief Exit point of function Dcm_Dsp_MemoryServices_ReadMemory_Execute() */
#define DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_EXECUTE_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_GETRESULTS_ENTRY
/** \brief Entry point of function Dcm_Dsp_MemoryServices_ReadMemory_GetResults() */
#define DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_GETRESULTS_ENTRY(a, b, c)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_GETRESULTS_EXIT
/** \brief Exit point of function Dcm_Dsp_MemoryServices_ReadMemory_GetResults() */
#define DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_GETRESULTS_EXIT(a, b, c, d)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_CANCEL_ENTRY
/** \brief Entry point of function Dcm_Dsp_MemoryServices_ReadMemory_Cancel() */
#define DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_CANCEL_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_CANCEL_EXIT
/** \brief Exit point of function Dcm_Dsp_MemoryServices_ReadMemory_Cancel() */
#define DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_CANCEL_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_CONFIRMFORCEDRCRRP_ENTRY
/** \brief Entry point of function Dcm_Dsp_MemoryServices_ReadMemory_ConfirmForcedRCRRP() */
#define DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_CONFIRMFORCEDRCRRP_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_CONFIRMFORCEDRCRRP_EXIT
/** \brief Exit point of function Dcm_Dsp_MemoryServices_ReadMemory_ConfirmForcedRCRRP() */
#define DBG_DCM_DSP_MEMORYSERVICES_READMEMORY_CONFIRMFORCEDRCRRP_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_PIDSERVICES_EXECUTE_ENTRY
/** \brief Entry point of function Dcm_Dsp_PidServices_Execute() */
#define DBG_DCM_DSP_PIDSERVICES_EXECUTE_ENTRY(a,b,c)
#endif

#ifndef DBG_DCM_DSP_PIDSERVICES_EXECUTE_EXIT
/** \brief Exit point of function Dcm_Dsp_PidServices_Execute() */
#define DBG_DCM_DSP_PIDSERVICES_EXECUTE_EXIT(a,b,c,d)
#endif

#ifndef DBG_DCM_POSTPONEROEFROMPOLLINGDIDDATA_ENTRY
/** \brief Entry point of function Dcm_PostponeROEFromPollingDIDData() */
#define DBG_DCM_POSTPONEROEFROMPOLLINGDIDDATA_ENTRY(a)
#endif

#ifndef DBG_DCM_POSTPONEROEFROMPOLLINGDIDDATA_EXIT
/** \brief Exit point of function Dcm_PostponeROEFromPollingDIDData() */
#define DBG_DCM_POSTPONEROEFROMPOLLINGDIDDATA_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_PROCESSMODERULES_ENTRY
/** \brief Entry point of function Dcm_Dsp_CommunicationControl_ProcessModeRules() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_PROCESSMODERULES_ENTRY()
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_PROCESSMODERULES_EXIT
/** \brief Exit point of function Dcm_Dsp_CommunicationControl_ProcessModeRules() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_PROCESSMODERULES_EXIT()
#endif

#ifndef DBG_DCM_UPLOADDOWNLOADINIT_ENTRY
/** \brief Entry point of function Dcm_UploadDownloadInit() */
#define DBG_DCM_UPLOADDOWNLOADINIT_ENTRY()
#endif

#ifndef DBG_DCM_UPLOADDOWNLOADINIT_EXIT
/** \brief Exit point of function Dcm_UploadDownloadInit() */
#define DBG_DCM_UPLOADDOWNLOADINIT_EXIT()
#endif

#ifndef DBG_DCM_DSP_LINKCONTROL_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsp_LinkControl_Init() */
#define DBG_DCM_DSP_LINKCONTROL_INIT_ENTRY()
#endif

#ifndef DBG_DCM_DSP_LINKCONTROL_INIT_EXIT
/** \brief Exit point of function Dcm_Dsp_LinkControl_Init() */
#define DBG_DCM_DSP_LINKCONTROL_INIT_EXIT()
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_ALLOWNEWTRANSMISSION_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_AllowNewTransmission() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_ALLOWNEWTRANSMISSION_ENTRY()
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_ALLOWNEWTRANSMISSION_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_AllowNewTransmission() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_ALLOWNEWTRANSMISSION_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_UUDTSENDNEXTPDID_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_UUDTSendNextPDID() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_UUDTSENDNEXTPDID_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_UUDTSENDNEXTPDID_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_UUDTSendNextPDID() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_UUDTSENDNEXTPDID_EXIT()
#endif

#ifndef DBG_DCM_DSPINTERNAL_READGENERICINFORMATION_SVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ReadGenericInformation_SvcH() */
#define DBG_DCM_DSPINTERNAL_READGENERICINFORMATION_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_READGENERICINFORMATION_SVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_ReadGenericInformation_SvcH() */
#define DBG_DCM_DSPINTERNAL_READGENERICINFORMATION_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTUSERDEFMEMORYDTCBYSTATUSMASK_SSVCH_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ReportUserDefMemoryDTCByStatusMask_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTUSERDEFMEMORYDTCBYSTATUSMASK_SSVCH_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTUSERDEFMEMORYDTCBYSTATUSMASK_SSVCH_EXIT
/** \brief Exit point of function Dcm_DspInternal_ReportUserDefMemoryDTCByStatusMask_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTUSERDEFMEMORYDTCBYSTATUSMASK_SSVCH_EXIT(a, b, c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTUSERDEFMEMORYDTCSNAPSHOTRECORDBYDTCNUMBER_SSVCH_ENTRY
/** \brief Entry point of function
    Dcm_DspInternal_ReportUserDefMemoryDtcSnapshotRecordByDtcNumber_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTUSERDEFMEMORYDTCSNAPSHOTRECORDBYDTCNUMBER_SSVCH_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTUSERDEFMEMORYDTCSNAPSHOTRECORDBYDTCNUMBER_SSVCH_EXIT
/** \brief Exit point of function
    Dcm_DspInternal_ReportUserDefMemoryDtcSnapshotRecordByDtcNumber_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTUSERDEFMEMORYDTCSNAPSHOTRECORDBYDTCNUMBER_SSVCH_EXIT(a, b, c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTUSERDEFMEMORYDTCEXTENDEDDATARECORDBYDTCNUMBER_SSVCH_ENTRY
/** \brief Entry point of function
    Dcm_DspInternal_ReportUserDefMemoryDtcExtendedDataRecordByDtcNumber_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTUSERDEFMEMORYDTCEXTENDEDDATARECORDBYDTCNUMBER_SSVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSPINTERNAL_REPORTUSERDEFMEMORYDTCEXTENDEDDATARECORDBYDTCNUMBER_SSVCH_EXIT
/** \brief Exit point of function
    Dcm_DspInternal_ReportUserDefMemoryDtcExtendedDataRecordByDtcNumber_SSvcH() */
#define DBG_DCM_DSPINTERNAL_REPORTUSERDEFMEMORYDTCEXTENDEDDATARECORDBYDTCNUMBER_SSVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_REQUESTTRANSFEREXIT_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_RequestTransferExit_SvcH() */
#define DBG_DCM_DSP_REQUESTTRANSFEREXIT_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_REQUESTTRANSFEREXIT_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_RequestTransferExit_SvcH() */
#define DBG_DCM_DSP_REQUESTTRANSFEREXIT_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_DSP_WRITEDATABYIDENTIFIER_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsp_WriteDataByIdentifier_Init() */
#define DBG_DCM_DSP_WRITEDATABYIDENTIFIER_INIT_ENTRY()
#endif

#ifndef DBG_DCM_DSP_WRITEDATABYIDENTIFIER_INIT_EXIT
/** \brief Exit point of function Dcm_Dsp_WriteDataByIdentifier_Init() */
#define DBG_DCM_DSP_WRITEDATABYIDENTIFIER_INIT_EXIT()
#endif

#ifndef DBG_DCM_DSP_WRITEDATABYIDENTIFIER_SVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_WriteDataByIdentifier_SvcH() */
#define DBG_DCM_DSP_WRITEDATABYIDENTIFIER_SVCH_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_WRITEDATABYIDENTIFIER_SVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_WriteDataByIdentifier_SvcH() */
#define DBG_DCM_DSP_WRITEDATABYIDENTIFIER_SVCH_EXIT(a,b,c)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD1_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfOutOfServiceGuard1() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD1_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD1_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfOutOfServiceGuard1() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD1_EXIT(a, b)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD2_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfOutOfServiceGuard2() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD2_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD2_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfOutOfServiceGuard2() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD2_EXIT(a, b)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD3_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfOutOfServiceAction3() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD3_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD3_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfOutOfServiceAction3() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD3_EXIT(a, b)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD4_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfOutOfServiceAction4() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD4_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD4_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfOutOfServiceAction4() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD4_EXIT(a, b)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD7_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfOutOfServiceGuard7() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD7_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD7_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfOutOfServiceGuard7() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD7_EXIT(a, b)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD8_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfOutOfServiceGuard8() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD8_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD8_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfOutOfServiceGuard8() */
#define DBG_DCM_HSMDCMSFOUTOFSERVICEGUARD8_EXIT(a, b)
#endif

#ifndef DBG_DCM_HSMDCMSFPERIODICPROCESSINGACTION1_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfPeriodicProcessingAction1() */
#define DBG_DCM_HSMDCMSFPERIODICPROCESSINGACTION1_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFPERIODICPROCESSINGACTION1_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfPeriodicProcessingAction1() */
#define DBG_DCM_HSMDCMSFPERIODICPROCESSINGACTION1_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFPERIODICPROCESSINGUUDTACTION1_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfPeriodicProcessingUUDTAction1() */
#define DBG_DCM_HSMDCMSFPERIODICPROCESSINGUUDTACTION1_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFPERIODICPROCESSINGUUDTACTION1_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfPeriodicProcessingUUDTAction1() */
#define DBG_DCM_HSMDCMSFPERIODICPROCESSINGUUDTACTION1_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTENTRY_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfPeriodicSendingUUDTEntry() */
#define DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTENTRY_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTENTRY_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfPeriodicSendingUUDTEntry() */
#define DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTENTRY_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTACTION1_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfPeriodicSendingUUDTAction1() */
#define DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTACTION1_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTACTION1_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfPeriodicSendingUUDTAction1() */
#define DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTACTION1_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTFINISHINGACTION1_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfPeriodicSendingUUDTFinishingAction1() */
#define DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTFINISHINGACTION1_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTFINISHINGACTION1_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfPeriodicSendingUUDTFinishingAction1() */
#define DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTFINISHINGACTION1_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTFINISHINGACTION2_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfPeriodicSendingUUDTFinishingAction2() */
#define DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTFINISHINGACTION2_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTFINISHINGACTION2_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfPeriodicSendingUUDTFinishingAction2() */
#define DBG_DCM_HSMDCMSFPERIODICSENDINGUUDTFINISHINGACTION2_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFROEPROCESSINGACTION3_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfROEProcessingAction3() */
#define DBG_DCM_HSMDCMSFROEPROCESSINGACTION3_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFROEPROCESSINGACTION3_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfROEProcessingAction3() */
#define DBG_DCM_HSMDCMSFROEPROCESSINGACTION3_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFROERESPONSEPROCESSINGENTRY_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfROEResponseProcessingEntry() */
#define DBG_DCM_HSMDCMSFROERESPONSEPROCESSINGENTRY_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFROERESPONSEPROCESSINGENTRY_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfROEResponseProcessingEntry() */
#define DBG_DCM_HSMDCMSFROERESPONSEPROCESSINGENTRY_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFREQUESTPROCESSINGENTRY_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfRequestProcessingEntry() */
#define DBG_DCM_HSMDCMSFREQUESTPROCESSINGENTRY_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFREQUESTPROCESSINGENTRY_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfRequestProcessingEntry() */
#define DBG_DCM_HSMDCMSFREQUESTPROCESSINGENTRY_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFREQUESTPROCESSINGACTION3_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfRequestProcessingAction3() */
#define DBG_DCM_HSMDCMSFREQUESTPROCESSINGACTION3_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFREQUESTPROCESSINGACTION3_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfRequestProcessingAction3() */
#define DBG_DCM_HSMDCMSFREQUESTPROCESSINGACTION3_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFREQUESTCANCELLINGENTRY_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfRequestCancellingEntry() */
#define DBG_DCM_HSMDCMSFREQUESTCANCELLINGENTRY_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFREQUESTCANCELLINGENTRY_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfRequestCancellingEntry() */
#define DBG_DCM_HSMDCMSFREQUESTCANCELLINGENTRY_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORROEPROCESSINGCANCELLATIONENTRY_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfWaitForRoeProcessingCancellationEntry() */
#define DBG_DCM_HSMDCMSFWAITFORROEPROCESSINGCANCELLATIONENTRY_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORROEPROCESSINGCANCELLATIONENTRY_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfWaitForRoeProcessingCancellationEntry() */
#define DBG_DCM_HSMDCMSFWAITFORROEPROCESSINGCANCELLATIONENTRY_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORPROTOCOLENTRY_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfWaitForProtocolEntry() */
#define DBG_DCM_HSMDCMSFWAITFORPROTOCOLENTRY_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORPROTOCOLENTRY_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfWaitForProtocolEntry() */
#define DBG_DCM_HSMDCMSFWAITFORPROTOCOLENTRY_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFCANCELWITHGENERALREJECTENTRY_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfCancelWithGeneralRejectEntry() */
#define DBG_DCM_HSMDCMSFCANCELWITHGENERALREJECTENTRY_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFCANCELWITHGENERALREJECTENTRY_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfCancelWithGeneralRejectEntry() */
#define DBG_DCM_HSMDCMSFCANCELWITHGENERALREJECTENTRY_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFCANCELWITHGENERALREJECTACTION1_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfCancelWithGeneralRejectAction1() */
#define DBG_DCM_HSMDCMSFCANCELWITHGENERALREJECTACTION1_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFCANCELWITHGENERALREJECTACTION1_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfCancelWithGeneralRejectAction1() */
#define DBG_DCM_HSMDCMSFCANCELWITHGENERALREJECTACTION1_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORTXANDPROTOCOLENTRY_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfWaitForTxAndProtocolEntry() */
#define DBG_DCM_HSMDCMSFWAITFORTXANDPROTOCOLENTRY_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORTXANDPROTOCOLENTRY_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfWaitForTxAndProtocolEntry() */
#define DBG_DCM_HSMDCMSFWAITFORTXANDPROTOCOLENTRY_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFNORMALSENDINGCANCELLINGENTRY_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfNormalSendingCancellingEntry() */
#define DBG_DCM_HSMDCMSFNORMALSENDINGCANCELLINGENTRY_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFNORMALSENDINGCANCELLINGENTRY_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfNormalSendingCancellingEntry() */
#define DBG_DCM_HSMDCMSFNORMALSENDINGCANCELLINGENTRY_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFPGBUFSENDINGCANCELLINGENTRY_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfPgBufSendingCancellingEntry() */
#define DBG_DCM_HSMDCMSFPGBUFSENDINGCANCELLINGENTRY_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFPGBUFSENDINGCANCELLINGENTRY_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfPgBufSendingCancellingEntry() */
#define DBG_DCM_HSMDCMSFPGBUFSENDINGCANCELLINGENTRY_EXIT(a)
#endif

#ifndef DBG_DCM_HSMDCMSFPGBUFDATAREQUESTEDACTION1_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfPgBufDataRequestedAction1() */
#define DBG_DCM_HSMDCMSFPGBUFDATAREQUESTEDACTION1_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFPGBUFDATAREQUESTEDACTION1_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfPgBufDataRequestedAction1() */
#define DBG_DCM_HSMDCMSFPGBUFDATAREQUESTEDACTION1_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_GETRXSTATE_ENTRY
/** \brief Entry point of function Dcm_Dsl_ComMChannelStateManager_GetRxState() */
#define DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_GETRXSTATE_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_GETRXSTATE_EXIT
/** \brief Exit point of function Dcm_Dsl_ComMChannelStateManager_GetRxState() */
#define DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_GETRXSTATE_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_GETTXSTATE_ENTRY
/** \brief Entry point of function Dcm_Dsl_ComMChannelStateManager_GetTxState() */
#define DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_GETTXSTATE_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_GETTXSTATE_EXIT
/** \brief Exit point of function Dcm_Dsl_ComMChannelStateManager_GetTxState() */
#define DBG_DCM_DSL_COMMCHANNELSTATEMANAGER_GETTXSTATE_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSL_TXCONNECTIONGETSTATE_ENTRY
/** \brief Entry point of function Dcm_Dsl_TxConnectionGetState() */
#define DBG_DCM_DSL_TXCONNECTIONGETSTATE_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_TXCONNECTIONGETSTATE_EXIT
/** \brief Exit point of function Dcm_Dsl_TxConnectionGetState() */
#define DBG_DCM_DSL_TXCONNECTIONGETSTATE_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSL_RXCONNECTIONGETSTATE_ENTRY
/** \brief Entry point of function Dcm_Dsl_RxConnectionGetState() */
#define DBG_DCM_DSL_RXCONNECTIONGETSTATE_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_RXCONNECTIONGETSTATE_EXIT
/** \brief Exit point of function Dcm_Dsl_RxConnectionGetState() */
#define DBG_DCM_DSL_RXCONNECTIONGETSTATE_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_SESSIONMANAGER_GETDIAGNOSTICSESSION_ENTRY
/** \brief Entry point of function Dcm_Dsl_Supervisor_SessionManager_GetDiagnosticSession() */
#define DBG_DCM_DSL_SUPERVISOR_SESSIONMANAGER_GETDIAGNOSTICSESSION_ENTRY()
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_SESSIONMANAGER_GETDIAGNOSTICSESSION_EXIT
/** \brief Exit point of function Dcm_Dsl_Supervisor_SessionManager_GetDiagnosticSession() */
#define DBG_DCM_DSL_SUPERVISOR_SESSIONMANAGER_GETDIAGNOSTICSESSION_EXIT(a)
#endif

#ifndef DBG_DCM_PROCESSSERVICEASYNC_ENTRY
/** \brief Entry point of function Dcm_ProcessServiceAsync() */
#define DBG_DCM_PROCESSSERVICEASYNC_ENTRY()
#endif

#ifndef DBG_DCM_PROCESSSERVICEASYNC_EXIT
/** \brief Exit point of function Dcm_ProcessServiceAsync() */
#define DBG_DCM_PROCESSSERVICEASYNC_EXIT()
#endif

#ifndef DBG_DCM_DSPINTERNAL_GETSIDCONFIG_ENTRY
/** \brief Entry point of function Dcm_DspInternal_GetSidConfig() */
#define DBG_DCM_DSPINTERNAL_GETSIDCONFIG_ENTRY(a, b, c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_GETSIDCONFIG_EXIT
/** \brief Exit point of function Dcm_DspInternal_GetSidConfig() */
#define DBG_DCM_DSPINTERNAL_GETSIDCONFIG_EXIT(a, b, c, d)
#endif

#ifndef DBG_DCM_DSP_PROTOCOLCANCELPROCESSING_ENTRY
/** \brief Entry point of function Dcm_Dsp_ProtocolCancelProcessing() */
#define DBG_DCM_DSP_PROTOCOLCANCELPROCESSING_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_PROTOCOLCANCELPROCESSING_EXIT
/** \brief Exit point of function Dcm_Dsp_ProtocolCancelProcessing() */
#define DBG_DCM_DSP_PROTOCOLCANCELPROCESSING_EXIT(a)
#endif

#ifndef DBG_DCM_DSPINTERNAL_SHIFTBITSINBUFFER_ENTRY
/** \brief Entry point of function Dcm_DspInternal_ShiftBitsInBuffer() */
#define DBG_DCM_DSPINTERNAL_SHIFTBITSINBUFFER_ENTRY(a, b, c, d, e)
#endif

#ifndef DBG_DCM_DSPINTERNAL_SHIFTBITSINBUFFER_EXIT
/** \brief Exit point of function Dcm_DspInternal_ShiftBitsInBuffer() */
#define DBG_DCM_DSPINTERNAL_SHIFTBITSINBUFFER_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DCM_OBDSUPPORT_UPDATEAVAILABLEINFOWITHID_ENTRY
/** \brief Entry point of function Dcm_OBDSupport_UpdateAvailableInfoWithID() */
#define DBG_DCM_OBDSUPPORT_UPDATEAVAILABLEINFOWITHID_ENTRY(a, b, c)
#endif

#ifndef DBG_DCM_OBDSUPPORT_UPDATEAVAILABLEINFOWITHID_EXIT
/** \brief Exit point of function Dcm_OBDSupport_UpdateAvailableInfoWithID() */
#define DBG_DCM_OBDSUPPORT_UPDATEAVAILABLEINFOWITHID_EXIT(a, b, c, d)
#endif

#ifndef DBG_DCM_OBDSUPPORT_CHECKREQUESTCOMBINEDIDS_ENTRY
/** \brief Entry point of function Dcm_OBDSupport_CheckRequestCombinedIds() */
#define DBG_DCM_OBDSUPPORT_CHECKREQUESTCOMBINEDIDS_ENTRY(a, b)
#endif

#ifndef DBG_DCM_OBDSUPPORT_CHECKREQUESTCOMBINEDIDS_EXIT
/** \brief Exit point of function Dcm_OBDSupport_CheckRequestCombinedIds() */
#define DBG_DCM_OBDSUPPORT_CHECKREQUESTCOMBINEDIDS_EXIT(a, b, c)
#endif

#ifndef DBG_DCM_DSP_PROTOCOLTRANSMISSIONTIMEDOUTWAITINGFORDATA_ENTRY
/** \brief Entry point of function Dcm_Dsp_ProtocolTransmissionTimedOutWaitingForData() */
#define DBG_DCM_DSP_PROTOCOLTRANSMISSIONTIMEDOUTWAITINGFORDATA_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSP_PROTOCOLTRANSMISSIONTIMEDOUTWAITINGFORDATA_EXIT
/** \brief Exit point of function Dcm_Dsp_ProtocolTransmissionTimedOutWaitingForData() */
#define DBG_DCM_DSP_PROTOCOLTRANSMISSIONTIMEDOUTWAITINGFORDATA_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_PROTOCOLPROVIDENEXTTXBUFFER_ENTRY
/** \brief Entry point of function Dcm_Dsp_ProtocolProvideNextTxBuffer() */
#define DBG_DCM_DSP_PROTOCOLPROVIDENEXTTXBUFFER_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSP_PROTOCOLPROVIDENEXTTXBUFFER_EXIT
/** \brief Exit point of function Dcm_Dsp_ProtocolProvideNextTxBuffer() */
#define DBG_DCM_DSP_PROTOCOLPROVIDENEXTTXBUFFER_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_PROTOCOLTRANSMISSIONFINISHED_ENTRY
/** \brief Entry point of function Dcm_Dsp_ProtocolTransmissionFinished() */
#define DBG_DCM_DSP_PROTOCOLTRANSMISSIONFINISHED_ENTRY(a, b, c)
#endif

#ifndef DBG_DCM_DSP_PROTOCOLTRANSMISSIONFINISHED_EXIT
/** \brief Exit point of function Dcm_Dsp_ProtocolTransmissionFinished() */
#define DBG_DCM_DSP_PROTOCOLTRANSMISSIONFINISHED_EXIT(a, b, c)
#endif

#ifndef DBG_DCM_DSP_PROTOCOLSTART_ENTRY
/** \brief Entry point of function Dcm_Dsp_ProtocolStart() */
#define DBG_DCM_DSP_PROTOCOLSTART_ENTRY(a, b, c, d, e)
#endif

#ifndef DBG_DCM_DSP_PROTOCOLSTART_EXIT
/** \brief Exit point of function Dcm_Dsp_ProtocolStart() */
#define DBG_DCM_DSP_PROTOCOLSTART_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DCM_DSP_PROTOCOLCANCEL_ENTRY
/** \brief Entry point of function Dcm_Dsp_ProtocolCancel() */
#define DBG_DCM_DSP_PROTOCOLCANCEL_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_PROTOCOLCANCEL_EXIT
/** \brief Exit point of function Dcm_Dsp_ProtocolCancel() */
#define DBG_DCM_DSP_PROTOCOLCANCEL_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_PROTOCOLSETUPCANCELLATION_ENTRY
/** \brief Entry point of function Dcm_Dsp_ProtocolSetUpCancellation() */
#define DBG_DCM_DSP_PROTOCOLSETUPCANCELLATION_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSP_PROTOCOLSETUPCANCELLATION_EXIT
/** \brief Exit point of function Dcm_Dsp_ProtocolSetUpCancellation() */
#define DBG_DCM_DSP_PROTOCOLSETUPCANCELLATION_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_SERVICEPROCESSORINIT_ENTRY
/** \brief Entry point of function Dcm_Dsp_ServiceProcessorInit() */
#define DBG_DCM_DSP_SERVICEPROCESSORINIT_ENTRY()
#endif

#ifndef DBG_DCM_DSP_SERVICEPROCESSORINIT_EXIT
/** \brief Exit point of function Dcm_Dsp_ServiceProcessorInit() */
#define DBG_DCM_DSP_SERVICEPROCESSORINIT_EXIT()
#endif

#ifndef DBG_DCM_DSP_PROTOCOL_P2TIMERTIMEOUT_ENTRY
/** \brief Entry point of function Dcm_Dsp_Protocol_P2TimerTimeout() */
#define DBG_DCM_DSP_PROTOCOL_P2TIMERTIMEOUT_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_PROTOCOL_P2TIMERTIMEOUT_EXIT
/** \brief Exit point of function Dcm_Dsp_Protocol_P2TimerTimeout() */
#define DBG_DCM_DSP_PROTOCOL_P2TIMERTIMEOUT_EXIT()
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_GETDIDINFO_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_GetDidInfo_Init() */
#define DBG_DCM_DSP_DIDSERVICES_GETDIDINFO_INIT_ENTRY(a,b,c,d)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_GETDIDINFO_INIT_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_GetDidInfo_Init() */
#define DBG_DCM_DSP_DIDSERVICES_GETDIDINFO_INIT_EXIT(a,b,c,d)
#endif

#ifndef DBG_DCM_DSLINTERNAL_CONNECTIONWAITINGFORDATA_ENTRY
/** \brief Entry point to connection timeout functionality() */
#define DBG_DCM_DSLINTERNAL_CONNECTIONWAITINGFORDATA_ENTRY()
#endif

#ifndef DBG_DCM_DSLINTERNAL_CONNECTIONWAITINGFORDATA_EXIT
/** \brief Exit point of connection timeout functionality() */
#define DBG_DCM_DSLINTERNAL_CONNECTIONWAITINGFORDATA_EXIT()
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORAPPLGUARD2_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfWaitForApplGuard2() */
#define DBG_DCM_HSMDCMSFWAITFORAPPLGUARD2_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORAPPLGUARD2_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfWaitForApplGuard2() */
#define DBG_DCM_HSMDCMSFWAITFORAPPLGUARD2_EXIT(a, b)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORAPPLGUARD3_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfWaitForApplGuard2() */
#define DBG_DCM_HSMDCMSFWAITFORAPPLGUARD3_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORAPPLGUARD3_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfWaitForApplGuard2() */
#define DBG_DCM_HSMDCMSFWAITFORAPPLGUARD3_EXIT(a, b)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORAPPLGUARD4_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfWaitForApplGuard4() */
#define DBG_DCM_HSMDCMSFWAITFORAPPLGUARD4_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORAPPLGUARD4_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfWaitForApplGuard4() */
#define DBG_DCM_HSMDCMSFWAITFORAPPLGUARD4_EXIT(a, b)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORAPPLGUARD5_ENTRY
/** \brief Entry point of function Dcm_HsmDcmSfWaitForApplGuard2() */
#define DBG_DCM_HSMDCMSFWAITFORAPPLGUARD5_ENTRY(a)
#endif

#ifndef DBG_DCM_HSMDCMSFWAITFORAPPLGUARD5_EXIT
/** \brief Exit point of function Dcm_HsmDcmSfWaitForApplGuard2() */
#define DBG_DCM_HSMDCMSFWAITFORAPPLGUARD5_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSDINTERNAL_PGBUFDATAREQUESTED_ENTRY
/** \brief Entry point of function Dcm_DsdInternal_ProcessPage() */
#define DBG_DCM_DSDINTERNAL_PGBUFDATAREQUESTED_ENTRY()
#endif

#ifndef DBG_DCM_DSDINTERNAL_PGBUFDATAREQUESTED_EXIT
/** \brief Entry point of function Dcm_DsdInternal_ProcessPage() */
#define DBG_DCM_DSDINTERNAL_PGBUFDATAREQUESTED_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_READDID_EXECUTE_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_ReadDid_Execute() */
#define DBG_DCM_DSP_DIDSERVICES_READDID_EXECUTE_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_READDID_EXECUTE_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_ReadDid_Execute() */
#define DBG_DCM_DSP_DIDSERVICES_READDID_EXECUTE_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_READDATAOFSINGLEDID_EXECUTE_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_ReadDataOfSingleDid_Execute() */
#define DBG_DCM_DSP_DIDSERVICES_READDATAOFSINGLEDID_EXECUTE_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_READDATAOFSINGLEDID_EXECUTE_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_ReadDataOfSingleDid_Execute() */
#define DBG_DCM_DSP_DIDSERVICES_READDATAOFSINGLEDID_EXECUTE_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_GETACCESSINFO_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_GetAccessInfo() */
#define DBG_DCM_DSP_DIDSERVICES_GETACCESSINFO_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_GETACCESSINFO_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_GetAccessInfo() */
#define DBG_DCM_DSP_DIDSERVICES_GETACCESSINFO_EXIT(a, b, c)
#endif

#ifndef DBG_DCM_DSPINTERNAL_GETCOMMONDIDSIGNALMASK_ENTRY
/** \brief Entry point of function Dcm_DspInternal_GetCommonDidSignalMask() */
#define DBG_DCM_DSPINTERNAL_GETCOMMONDIDSIGNALMASK_ENTRY(a)
#endif

#ifndef DBG_DCM_DSPINTERNAL_GETCOMMONDIDSIGNALMASK_EXIT
/** \brief Exit point of function Dcm_DspInternal_GetCommonDidSignalMask() */
#define DBG_DCM_DSPINTERNAL_GETCOMMONDIDSIGNALMASK_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_GETMAXIMUMDIDLENGTH_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_GetMaximumDidLength() */
#define DBG_DCM_DSP_DIDSERVICES_GETMAXIMUMDIDLENGTH_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_GETMAXIMUMDIDLENGTH_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_GetMaximumDidLength() */
#define DBG_DCM_DSP_DIDSERVICES_GETMAXIMUMDIDLENGTH_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_VALIDATEDDDIDSOURCES_ENTRY
/** \brief Entry point of function Dcm_Dsp_DidServices_ValidateDDDIDSources() */
#define DBG_DCM_DSP_DIDSERVICES_VALIDATEDDDIDSOURCES_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_VALIDATEDDDIDSOURCES_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_ValidateDDDIDSources() */
#define DBG_DCM_DSP_DIDSERVICES_VALIDATEDDDIDSOURCES_EXIT(a, b, c)
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_SCHEDULE_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_Schedule() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_SCHEDULE_ENTRY()
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_SCHEDULE_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_Schedule() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_SCHEDULE_EXIT()
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_MAINFUNCTION_ENTRY
/** \brief Entry point of function Dcm_Dsl_RoeServices_MainFunction() */
#define DBG_DCM_DSL_ROESERVICES_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_MAINFUNCTION_EXIT
/** \brief Entry point of function Dcm_Dsl_RoeServices_MainFunction() */
#define DBG_DCM_DSL_ROESERVICES_MAINFUNCTION_EXIT()
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_GETRXPDUID_ENTRY
/** \brief Entry point of function Dcm_Dsl_RoeServices_GetRxPduId() */
#define DBG_DCM_DSL_ROESERVICES_GETRXPDUID_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_GETRXPDUID_EXIT
/** \brief Entry point of function Dcm_Dsl_RoeServices_GetRxPduId() */
#define DBG_DCM_DSL_ROESERVICES_GETRXPDUID_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_SESSIONCHANGE_ENTRY
/** \brief Entry point of function Dcm_Dsl_RoeServices_SessionChange() */
#define DBG_DCM_DSL_ROESERVICES_SESSIONCHANGE_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_SESSIONCHANGE_EXIT
/** \brief Entry point of function Dcm_Dsl_RoeServices_SessionChange() */
#define DBG_DCM_DSL_ROESERVICES_SESSIONCHANGE_EXIT()
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_TRIGGERONEVENT_ENTRY
/** \brief Entry point of function Dcm_Dsl_RoeServices_TriggerOnEvent() */
#define DBG_DCM_DSL_ROESERVICES_TRIGGERONEVENT_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_TRIGGERONEVENT_EXIT
/** \brief Entry point of function Dcm_Dsl_RoeServices_TriggerOnEvent() */
#define DBG_DCM_DSL_ROESERVICES_TRIGGERONEVENT_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_TRIGGERONDTCSTATUS_ENTRY
/** \brief Entry point of function Dcm_Dsl_RoeServices_TriggerOnEvent() */
#define DBG_DCM_DSL_ROESERVICES_TRIGGERONDTCSTATUS_ENTRY(a, b, c)
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_TRIGGERONDTCSTATUS_EXIT
/** \brief Entry point of function Dcm_Dsl_RoeServices_TriggerOnEvent() */
#define DBG_DCM_DSL_ROESERVICES_TRIGGERONDTCSTATUS_EXIT(a, b, c, d)
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_STOPRESPONSEONEVENT_ENTRY
/** \brief Entry point of function Dcm_Dsl_RoeServices_StopResponseOnEvent() */
#define DBG_DCM_DSL_ROESERVICES_STOPRESPONSEONEVENT_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_STOPRESPONSEONEVENT_EXIT
/** \brief Entry point of function Dcm_Dsl_RoeServices_StopResponseOnEvent() */
#define DBG_DCM_DSL_ROESERVICES_STOPRESPONSEONEVENT_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_REPORTACTIVATEDEVENTS_ENTRY
/** \brief Entry point of function Dcm_Dsl_RoeServices_ReportActivatedEvents() */
#define DBG_DCM_DSL_ROESERVICES_REPORTACTIVATEDEVENTS_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_REPORTACTIVATEDEVENTS_EXIT
/** \brief Entry point of function Dcm_Dsl_RoeServices_ReportActivatedEvents() */
#define DBG_DCM_DSL_ROESERVICES_REPORTACTIVATEDEVENTS_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_STARTRESPONSEONEVENT_ENTRY
/** \brief Entry point of function Dcm_Dsl_RoeServices_StartResponseOnEvent() */
#define DBG_DCM_DSL_ROESERVICES_STARTRESPONSEONEVENT_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_STARTRESPONSEONEVENT_EXIT
/** \brief Entry point of function Dcm_Dsl_RoeServices_StartResponseOnEvent() */
#define DBG_DCM_DSL_ROESERVICES_STARTRESPONSEONEVENT_EXIT(a, b, c)
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_SETUPEVENT_DTCSTATUSCHANGE_ENTRY
/** \brief Entry point of function Dcm_Dsl_RoeServices_StartResponseOnEvent() */
#define DBG_DCM_DSL_ROESERVICES_SETUPEVENT_DTCSTATUSCHANGE_ENTRY(a, b, c)
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_SETUPEVENT_DTCSTATUSCHANGE_EXIT
/** \brief Entry point of function Dcm_Dsl_RoeServices_StartResponseOnEvent() */
#define DBG_DCM_DSL_ROESERVICES_SETUPEVENT_DTCSTATUSCHANGE_EXIT(a, b, c, d)
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsl_RoeServices_Init() */
#define DBG_DCM_DSL_ROESERVICES_INIT_ENTRY()
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_INIT_EXIT
/** \brief Entry point of function Dcm_Dsl_RoeServices_Init() */
#define DBG_DCM_DSL_ROESERVICES_INIT_EXIT()
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_ISINITIALIZED_ENTRY
/** \brief Entry point of function Dcm_Dsl_RoeServices_IsInitialized() */
#define DBG_DCM_DSL_ROESERVICES_ISINITIALIZED_ENTRY()
#endif

#ifndef DBG_DCM_DSL_ROESERVICES_ISINITIALIZED_EXIT
/** \brief Entry point of function Dcm_Dsl_RoeServices_IsInitialized() */
#define DBG_DCM_DSL_ROESERVICES_ISINITIALIZED_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_BUFFERGETINFO_ENTRY

#define DBG_DCM_DSL_BUFFERGETINFO_ENTRY(Requester,Use,Buffer)
#endif

#ifndef DBG_DCM_DSL_BUFFERGETINFO_EXIT

#define DBG_DCM_DSL_BUFFERGETINFO_EXIT(Requester,Use,Buffer)
#endif

#ifndef DBG_DCM_DSL_BUFFERFREE_ENTRY

#define DBG_DCM_DSL_BUFFERFREE_ENTRY(Requester,BufferId)
#endif

#ifndef DBG_DCM_DSL_BUFFERFREE_EXIT

#define DBG_DCM_DSL_BUFFERFREE_EXIT(Retval,Requester,BufferId)
#endif

#ifndef DBG_DCM_DSL_BUFFERCHANGESTATE_ENTRY

#define DBG_DCM_DSL_BUFFERCHANGESTATE_ENTRY(CurrentUser,NewState,BufferId)
#endif

#ifndef DBG_DCM_DSL_BUFFERCHANGESTATE_EXIT

#define DBG_DCM_DSL_BUFFERCHANGESTATE_EXIT(Retval,CurrentUser,NewState,BufferId)
#endif

#ifndef DBG_DCM_DSL_BUFFERALLOCATE_ENTRY

#define DBG_DCM_DSL_BUFFERALLOCATE_ENTRY(Requester,Use,Size,Buffer)
#endif

#ifndef DBG_DCM_DSL_BUFFERALLOCATE_EXIT

#define DBG_DCM_DSL_BUFFERALLOCATE_EXIT(Retval,Requester,Use,Size,Buffer)
#endif

#ifndef DBG_DCM_DSL_BUFFERGIVE_ENTRY

#define DBG_DCM_DSL_BUFFERGIVE_ENTRY(Provider,Receiver,BufferId)
#endif

#ifndef DBG_DCM_DSL_BUFFERGIVE_EXIT

#define DBG_DCM_DSL_BUFFERGIVE_EXIT(Retval,Provider,Receiver,BufferId)
#endif

#ifndef DBG_DCM_DSL_BUFFERINIT_ENTRY

#define DBG_DCM_DSL_BUFFERINIT_ENTRY()
#endif

#ifndef DBG_DCM_DSL_BUFFERINIT_EXIT

#define DBG_DCM_DSL_BUFFERINIT_EXIT()
#endif

#ifndef DBG_DCM_DSL_BUFFERFREERXTXNRC_ENTRY
/** \brief Entry point of function Dcm_Dsl_BufferFreeRxTxNRC() */
#define DBG_DCM_DSL_BUFFERFREERXTXNRC_ENTRY(a, b, c, d)
#endif

#ifndef DBG_DCM_DSL_BUFFERFREERXTXNRC_EXIT
/** \brief Exit point of function Dcm_Dsl_BufferFreeRxTxNRC() */
#define DBG_DCM_DSL_BUFFERFREERXTXNRC_EXIT()
#endif

#ifndef DBG_DCM_DSL_SECURITYLEVELHANDLER_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsl_SecurityLevelHandler_InitSecurityLevel() */
#define DBG_DCM_DSL_SECURITYLEVELHANDLER_INIT_ENTRY()
#endif

#ifndef DBG_DCM_DSL_SECURITYLEVELHANDLER_INIT_EXIT
/** \brief Exit point of function Dcm_Dsl_SecurityLevelHandler_InitSecurityLevel() */
#define DBG_DCM_DSL_SECURITYLEVELHANDLER_INIT_EXIT()
#endif

#ifndef DBG_DCM_DSL_SECURITYLEVELHANDLER_SETSECURITYLEVEL_ENTRY
/** \brief Entry point of function Dcm_Dsl_SecurityLevelHandler_SetSecurityLevel() */
#define DBG_DCM_DSL_SECURITYLEVELHANDLER_SETSECURITYLEVEL_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_SECURITYLEVELHANDLER_SETSECURITYLEVEL_EXIT
/** \brief Exit point of function Dcm_Dsl_SecurityLevelHandler_SetSecurityLevel() */
#define DBG_DCM_DSL_SECURITYLEVELHANDLER_SETSECURITYLEVEL_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_SECURITYLEVELHANDLER_GETSECURITYLEVEL_ENTRY
/** \brief Entry point of function Dcm_Dsl_SecurityLevelHandler_GetSecurityLevel() */
#define DBG_DCM_DSL_SECURITYLEVELHANDLER_GETSECURITYLEVEL_ENTRY()
#endif

#ifndef DBG_DCM_DSL_SECURITYLEVELHANDLER_GETSECURITYLEVEL_EXIT
/** \brief Exit point of function Dcm_Dsl_SecurityLevelHandler_GetSecurityLevel() */
#define DBG_DCM_DSL_SECURITYLEVELHANDLER_GETSECURITYLEVEL_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsp_DemFacade_Init() */
#define DBG_DCM_DSP_DEMFACADE_INIT_ENTRY()
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_INIT_EXIT
/** \brief Exit point of function Dcm_Dsp_DemFacade_Init() */
#define DBG_DCM_DSP_DEMFACADE_INIT_EXIT()
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_LOCKCLIENT_ENTRY
/** \brief Entry point of function Dcm_Dsp_DemFacade_LockClient() */
#define DBG_DCM_DSP_DEMFACADE_LOCKCLIENT_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_LOCKCLIENT_EXIT
/** \brief Exit point of function Dcm_Dsp_DemFacade_LockClient() */
#define DBG_DCM_DSP_DEMFACADE_LOCKCLIENT_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_UNLOCKCLIENT_ENTRY
/** \brief Entry point of function Dcm_Dsp_DemFacade_UnlockClient() */
#define DBG_DCM_DSP_DEMFACADE_UNLOCKCLIENT_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_UNLOCKCLIENT_EXIT
/** \brief Exit point of function Dcm_Dsp_DemFacade_UnlockClient() */
#define DBG_DCM_DSP_DEMFACADE_UNLOCKCLIENT_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_GETCLIENT_ENTRY
/** \brief Entry point of function Dcm_Dsp_DemFacade_GetClient() */
#define DBG_DCM_DSP_DEMFACADE_GETCLIENT_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_GETCLIENT_EXIT
/** \brief Exit point of function Dcm_Dsp_DemFacade_GetClient() */
#define DBG_DCM_DSP_DEMFACADE_GETCLIENT_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_READDATAOFOBDFREEZEFRAME_ENTRY
/** \brief Entry point of function Dcm_Dsp_DemFacade_ReadDataOfOBDFreezeFrame() */
#define DBG_DCM_DSP_DEMFACADE_READDATAOFOBDFREEZEFRAME_ENTRY(a, b, c, d)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_READDATAOFOBDFREEZEFRAME_EXIT
/** \brief Exit point of function Dcm_Dsp_DemFacade_ReadDataOfOBDFreezeFrame() */
#define DBG_DCM_DSP_DEMFACADE_READDATAOFOBDFREEZEFRAME_EXIT(a, b, c, d, e)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_GETDTCOFOBDFREEZEFRAME_ENTRY
/** \brief Entry point of function Dcm_Dsp_DemFacade_GetDTCOfOBDFreezeFrame() */
#define DBG_DCM_DSP_DEMFACADE_GETDTCOFOBDFREEZEFRAME_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_GETDTCOFOBDFREEZEFRAME_EXIT
/** \brief Exit point of function Dcm_Dsp_DemFacade_GetDTCOfOBDFreezeFrame() */
#define DBG_DCM_DSP_DEMFACADE_GETDTCOFOBDFREEZEFRAME_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_GETAVAILABLEOBDMIDS_ENTRY
/** \brief Entry point of function Dcm_Dsp_DemFacade_GetAvailableOBDMIDs() */
#define DBG_DCM_DSP_DEMFACADE_GETAVAILABLEOBDMIDS_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_GETAVAILABLEOBDMIDS_EXIT
/** \brief Exit point of function Dcm_Dsp_DemFacade_GetAvailableOBDMIDs() */
#define DBG_DCM_DSP_DEMFACADE_GETAVAILABLEOBDMIDS_EXIT(a, b, c)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_GETNUMTIDSOFOBDMID_ENTRY
/** \brief Entry point of function Dcm_Dsp_DemFacade_GetNumTIDsOfOBDMID() */
#define DBG_DCM_DSP_DEMFACADE_GETNUMTIDSOFOBDMID_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_GETNUMTIDSOFOBDMID_EXIT
/** \brief Exit point of function Dcm_Dsp_DemFacade_GetNumTIDsOfOBDMID() */
#define DBG_DCM_DSP_DEMFACADE_GETNUMTIDSOFOBDMID_EXIT(a, b, c)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_GETDTRDATA_ENTRY
/** \brief Entry point of function Dcm_Dsp_DemFacade_GetDTRData() */
#define DBG_DCM_DSP_DEMFACADE_GETDTRDATA_ENTRY(a, b, c, d, e, f, g)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_GETDTRDATA_EXIT
/** \brief Exit point of function Dcm_Dsp_DemFacade_GetDTRData() */
#define DBG_DCM_DSP_DEMFACADE_GETDTRDATA_EXIT(a, b, c, d, e, f, g, h)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_GETDTCINFOBYOCCURRENCE_PROCESS_ENTRY
/** \brief Entry point of function Dcm_Dsp_DemFacade_GetDTCInfoByOccurrence_Process() */
#define DBG_DCM_DSP_DEMFACADE_GETDTCINFOBYOCCURRENCE_PROCESS_ENTRY(a, b, c, d, e, f)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_GETDTCINFOBYOCCURRENCE_PROCESS_EXIT
/** \brief Exit point of function Dcm_Dsp_DemFacade_GetDTCInfoByOccurrence_Process() */
#define DBG_DCM_DSP_DEMFACADE_GETDTCINFOBYOCCURRENCE_PROCESS_EXIT(a, b, c, d, e, f, g)
#endif

#ifndef DBG_DCM_DSP_READDTCINFORMATION_REPORTFIRSTTESTFAILEDDTC_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDTCInformation_ReportFirstTestFailedDTC_SSvcH() */
#define DBG_DCM_DSP_READDTCINFORMATION_REPORTFIRSTTESTFAILEDDTC_SSVCH_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSP_READDTCINFORMATION_REPORTFIRSTTESTFAILEDDTC_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDTCInformation_ReportFirstTestFailedDTC_SSvcH() */
#define DBG_DCM_DSP_READDTCINFORMATION_REPORTFIRSTTESTFAILEDDTC_SSVCH_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_READDTCINFORMATION_REPORTFIRSTCONFIRMEDDTC_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDTCInformation_ReportFirstConfirmedDTC_SSvcH() */
#define DBG_DCM_DSP_READDTCINFORMATION_REPORTFIRSTCONFIRMEDDTC_SSVCH_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSP_READDTCINFORMATION_REPORTFIRSTCONFIRMEDDTC_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDTCInformation_ReportFirstConfirmedDTC_SSvcH() */
#define DBG_DCM_DSP_READDTCINFORMATION_REPORTFIRSTCONFIRMEDDTC_SSVCH_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_READDTCINFORMATION_REPORTMOSTRECENTTESTFAILEDDTC_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDTCInformation_ReportMostRecentTestFailedDTC_SSvcH()*/
#define DBG_DCM_DSP_READDTCINFORMATION_REPORTMOSTRECENTTESTFAILEDDTC_SSVCH_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSP_READDTCINFORMATION_REPORTMOSTRECENTTESTFAILEDDTC_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDTCInformation_ReportMostRecentTestFailedDTC_SSvcH()*/
#define DBG_DCM_DSP_READDTCINFORMATION_REPORTMOSTRECENTTESTFAILEDDTC_SSVCH_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_READDTCINFORMATION_REPORTMOSTRECENTCONFIRMEDDTC_SSVCH_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDTCInformation_ReportMostRecentConfirmedDTC_SSvcH()*/
#define DBG_DCM_DSP_READDTCINFORMATION_REPORTMOSTRECENTCONFIRMEDDTC_SSVCH_ENTRY(a, b)
#endif

#ifndef DBG_DCM_DSP_READDTCINFORMATION_REPORTMOSTRECENTCONFIRMEDDTC_SSVCH_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDTCInformation_ReportMostRecentConfirmedDTC_SSvcH() */
#define DBG_DCM_DSP_READDTCINFORMATION_REPORTMOSTRECENTCONFIRMEDDTC_SSVCH_EXIT(a, b)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_CONTROLDTCSETTING_ENABLE_ENTRY
/** \brief Entry point of function Dcm_Dsp_DemFacade_ControlDTCSetting_Enable() */
#define DBG_DCM_DSP_DEMFACADE_CONTROLDTCSETTING_ENABLE_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_CONTROLDTCSETTING_ENABLE_EXIT
/** \brief Exit point of function Dcm_Dsp_DemFacade_ControlDTCSetting_Enable() */
#define DBG_DCM_DSP_DEMFACADE_CONTROLDTCSETTING_ENABLE_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_CONTROLDTCSETTING_DISABLE_ENTRY
/** \brief Entry point of function Dcm_Dsp_DemFacade_ControlDTCSetting_Disable() */
#define DBG_DCM_DSP_DEMFACADE_CONTROLDTCSETTING_DISABLE_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_CONTROLDTCSETTING_DISABLE_EXIT
/** \brief Exit point of function Dcm_Dsp_DemFacade_ControlDTCSetting_Disable() */
#define DBG_DCM_DSP_DEMFACADE_CONTROLDTCSETTING_DISABLE_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_CONTROLDTCSETTINGSTATE_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsp_ControlDTCSettingState_Init() */
#define DBG_DCM_DSP_CONTROLDTCSETTINGSTATE_INIT_ENTRY()
#endif

#ifndef DBG_DCM_DSP_CONTROLDTCSETTINGSTATE_INIT_EXIT
/** \brief Exit point of function Dcm_Dsp_ControlDTCSettingState_Init() */
#define DBG_DCM_DSP_CONTROLDTCSETTINGSTATE_INIT_EXIT()
#endif

#ifndef DBG_DCM_DSP_CONTROLDTCSETTING_REENABLE_ENTRY
/** \brief Entry point of function Dcm_Dsp_ControlDTCSetting_SetState() */
#define DBG_DCM_DSP_CONTROLDTCSETTING_REENABLE_ENTRY()
#endif

#ifndef DBG_DCM_DSP_CONTROLDTCSETTING_REENABLE_EXIT
/** \brief Exit point of function Dcm_Dsp_ControlDTCSetting_SetState() */
#define DBG_DCM_DSP_CONTROLDTCSETTING_REENABLE_EXIT()
#endif

#ifndef DBG_DCM_DSP_CONTROLDTCSETTING_GETSTATE_ENTRY
/** \brief Entry point of function Dcm_Dsp_ControlDTCSetting_GetState() */
#define DBG_DCM_DSP_CONTROLDTCSETTING_GETSTATE_ENTRY()
#endif

#ifndef DBG_DCM_DSP_CONTROLDTCSETTING_GETSTATE_EXIT
/** \brief Exit point of function Dcm_Dsp_ControlDTCSetting_GetState() */
#define DBG_DCM_DSP_CONTROLDTCSETTING_GETSTATE_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_ENDIANNESSINTERPRETATION_COPYDATA_ENTRY
/** \brief Entry point of function Dcm_Dsp_EndiannessInterpretation_CopyData() */
#define DBG_DCM_DSP_ENDIANNESSINTERPRETATION_COPYDATA_ENTRY(a,b,c,d,e,f,g)
#endif

#ifndef DBG_DCM_DSP_ENDIANNESSINTERPRETATION_COPYDATA_EXIT
/** \brief Exit point of function Dcm_Dsp_EndiannessInterpretation_CopyData() */
#define DBG_DCM_DSP_ENDIANNESSINTERPRETATION_COPYDATA_EXIT(a,b,c,d,e,f,g)
#endif

#ifndef DBG_DCM_GETSESCTRLTYPEEXT_ENTRY
/** \brief Entry point of function Dcm_GetSesCtrlTypeExt() */
#define DBG_DCM_GETSESCTRLTYPEEXT_ENTRY(a)
#endif

#ifndef DBG_DCM_GETSESCTRLTYPEEXT_EXIT
/** \brief Exit point of function Dcm_GetSesCtrlTypeExt() */
#define DBG_DCM_GETSESCTRLTYPEEXT_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_CLEARDTCINFO_SETUP_ENTRY
/** \brief Entry point of function Dcm_Dsp_DemFacade_ClearDTCInfo_Setup() */
#define DBG_DCM_DSP_DEMFACADE_CLEARDTCINFO_SETUP_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_CLEARDTCINFO_SETUP_EXIT
/** \brief Exit point of function Dcm_Dsp_DemFacade_ClearDTCInfo_Setup() */
#define DBG_DCM_DSP_DEMFACADE_CLEARDTCINFO_SETUP_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_CLEARDTCINFO_PROCESS_ENTRY
/** \brief Entry point of function Dcm_Dsp_DemFacade_ClearDTCInfo_Process() */
#define DBG_DCM_DSP_DEMFACADE_CLEARDTCINFO_PROCESS_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_CLEARDTCINFO_PROCESS_EXIT
/** \brief Exit point of function Dcm_Dsp_DemFacade_ClearDTCInfo_Process() */
#define DBG_DCM_DSP_DEMFACADE_CLEARDTCINFO_PROCESS_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_CLEARDTCINFO_CANCEL_ENTRY
/** \brief Entry point of function Dcm_Dsp_DemFacade_ClearDTCInfo_Cancel() */
#define DBG_DCM_DSP_DEMFACADE_CLEARDTCINFO_CANCEL_ENTRY(a)
#endif

#ifndef DBG_DCM_DSP_DEMFACADE_CLEARDTCINFO_CANCEL_EXIT
/** \brief Exit point of function Dcm_Dsp_DemFacade_ClearDTCInfo_Cancel() */
#define DBG_DCM_DSP_DEMFACADE_CLEARDTCINFO_CANCEL_EXIT()
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_GETDIDGENERALBUFFER_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_GetDidGeneralBuffer() */
#define DBG_DCM_DSP_DIDSERVICES_GETDIDGENERALBUFFER_EXIT(a)
#endif

#ifndef DBG_DCM_DSP_DIDSERVICES_GETDIDSIGNALBUFFER_EXIT
/** \brief Exit point of function Dcm_Dsp_DidServices_GetDidSignalBuffer() */
#define DBG_DCM_DSP_DIDSERVICES_GETDIDSIGNALBUFFER_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_INIT_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosicSessionHandler_Init() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_INIT_ENTRY()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_INIT_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosicSessionHandler_Init() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_INIT_EXIT()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_P2TIMERHANDLINGMAINFUNCTION_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosicSessionHandler_P2TimerHandlingMainFunction() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_P2TIMERHANDLINGMAINFUNCTION_ENTRY()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_P2TIMERHANDLINGMAINFUNCTION_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosicSessionHandler_P2TimerHandlingMainFunction() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_P2TIMERHANDLINGMAINFUNCTION_EXIT()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_S3TIMERHANDLINGMAINFUNCTION_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosicSessionHandler_S3TimerHandlingMainFunction() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_S3TIMERHANDLINGMAINFUNCTION_ENTRY()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_S3TIMERHANDLINGMAINFUNCTION_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosicSessionHandler_S3TimerHandlingMainFunction() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_S3TIMERHANDLINGMAINFUNCTION_EXIT()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETSESSIONMAXRCRRP_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosicSessionHandler_GetSessionMaxRcrrp() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETSESSIONMAXRCRRP_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETSESSIONMAXRCRRP_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosicSessionHandler_GetSessionMaxRcrrp() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETSESSIONMAXRCRRP_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETCURRENTSESSION_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosicSessionHandler_GetCurrentSession() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETCURRENTSESSION_ENTRY()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETCURRENTSESSION_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosicSessionHandler_GetCurrentSession() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETCURRENTSESSION_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETNONDEFAULTSESSIONPROTOCOLINDEX_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosticSessionHandler_GetNonDefaultSessionProtocolIndex() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETNONDEFAULTSESSIONPROTOCOLINDEX_ENTRY()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETNONDEFAULTSESSIONPROTOCOLINDEX_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosticSessionHandler_GetNonDefaultSessionProtocolIndex() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETNONDEFAULTSESSIONPROTOCOLINDEX_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETNONDEFAULTSESSIONMAINCONNECTION_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosticSessionHandler_GetNonDefaultSessionMainConnection() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETNONDEFAULTSESSIONMAINCONNECTION_ENTRY()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETNONDEFAULTSESSIONMAINCONNECTION_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosticSessionHandler_GetNonDefaultSessionMainConnection() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETNONDEFAULTSESSIONMAINCONNECTION_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_RESETTODEFAULTSESSION_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosicSessionHandler_ResetToDefaultSession() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_RESETTODEFAULTSESSION_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_RESETTODEFAULTSESSION_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosicSessionHandler_ResetToDefaultSession() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_RESETTODEFAULTSESSION_EXIT()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_SETSESSION_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosicSessionHandler_SetSession() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_SETSESSION_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_SETSESSION_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosicSessionHandler_SetSession() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_SETSESSION_EXIT()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_SWITCHCURRENTSESSION_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosicSessionHandler_SwitchCurrentSession() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_SWITCHCURRENTSESSION_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_SWITCHCURRENTSESSION_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosicSessionHandler_SwitchCurrentSession() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_SWITCHCURRENTSESSION_EXIT()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_DOSWITCH_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosicSessionHandler_DoSwitch() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_DOSWITCH_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_DOSWITCH_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosicSessionHandler_DoSwitch() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_DOSWITCH_EXIT()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_UPONSESSIONCHANGENOTIFY_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosticSessionHandler_UponSessionChangeNotify() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_UPONSESSIONCHANGENOTIFY_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_UPONSESSIONCHANGENOTIFY_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosticSessionHandler_UponSessionChangeNotify() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_UPONSESSIONCHANGENOTIFY_EXIT()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_STARTP2TIMER_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosicSessionHandler_StartP2Timer() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_STARTP2TIMER_ENTRY(a,b,c)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_STARTP2TIMER_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosicSessionHandler_StartP2Timer() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_STARTP2TIMER_EXIT()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_STARTS3TIMER_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosicSessionHandler_StartS3Timer() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_STARTS3TIMER_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_STARTS3TIMER_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosicSessionHandler_StartS3Timer() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_STARTS3TIMER_EXIT()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_STOPTIMER_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosicSessionHandler_StopTimer() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_STOPTIMER_ENTRY(a,b,c)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_STOPTIMER_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosicSessionHandler_StopTimer() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_STOPTIMER_EXIT()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GIVEP2TIMER_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosicSessionHandler_GiveP2Timer() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GIVEP2TIMER_ENTRY(a,b,c)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GIVEP2TIMER_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosicSessionHandler_GiveP2Timer() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GIVEP2TIMER_EXIT()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_ALLOCATEP2TIMER_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosicSessionHandler_AllocateP2Timer() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_ALLOCATEP2TIMER_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_ALLOCATEP2TIMER_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosicSessionHandler_AllocateP2Timer() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_ALLOCATEP2TIMER_EXIT()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_FREEP2TIMER_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosicSessionHandler_FreeP2Timer() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_FREEP2TIMER_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_FREEP2TIMER_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosicSessionHandler_FreeP2Timer() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_FREEP2TIMER_EXIT()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETTIMERVALUE_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosicSessionHandler_GetTimerValue() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETTIMERVALUE_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETTIMERVALUE_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosicSessionHandler_GetTimerValue() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETTIMERVALUE_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETP2STARMAXTIME_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosticSessionHandler_GetP2StarMaxTime() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETP2STARMAXTIME_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETP2STARMAXTIME_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosticSessionHandler_GetP2StarMaxTime() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETP2STARMAXTIME_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETP2MAXTIME_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosticSessionHandler_GetP2MaxTime() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETP2MAXTIME_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETP2MAXTIME_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosticSessionHandler_GetP2MaxTime() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETP2MAXTIME_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_CLEARRESETTODEFAULTSESSIONOCCURED_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosticSessionHandler_ClearResetToDefaultSessionOccured() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_CLEARRESETTODEFAULTSESSIONOCCURED_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_CLEARRESETTODEFAULTSESSIONOCCURED_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosticSessionHandler_ClearResetToDefaultSessionOccured() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_CLEARRESETTODEFAULTSESSIONOCCURED_EXIT()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETPREEMPTIONOCCURED_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosticSessionHandler_GetPreemptionOccured() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETPREEMPTIONOCCURED_ENTRY()
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETPREEMPTIONOCCURED_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosticSessionHandler_GetPreemptionOccured() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETPREEMPTIONOCCURED_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETRESETTODEFAULTSESSIONOCCURED_ENTRY
/** \brief Entry point of function Dcm_Dsl_DiagnosticSessionHandler_GetResetToDefaultSessionOccured() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETRESETTODEFAULTSESSIONOCCURED_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETRESETTODEFAULTSESSIONOCCURED_EXIT
/** \brief Exit point of function Dcm_Dsl_DiagnosticSessionHandler_GetResetToDefaultSessionOccured() */
#define DBG_DCM_DSL_DIAGNOSTICSESSIONHANDLER_GETRESETTODEFAULTSESSIONOCCURED_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_SECURITYACCESS_SESSIONCHANGE_ENTRY
/** \brief Entry point of function Dcm_Dsp_SecurityAccess_SessionChange() */
#define DBG_DCM_DSL_SECURITYACCESS_SESSIONCHANGE_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSL_SECURITYACCESS_SESSIONCHANGE_EXIT
/** \brief Exit point of function Dcm_Dsp_SecurityAccess_SessionChange() */
#define DBG_DCM_DSL_SECURITYACCESS_SESSIONCHANGE_EXIT()
#endif

#ifndef DBG_DCM_DSL_SECURITYLEVEL_SESSIONCHANGE_ENTRY
/** \brief Entry point of function Dcm_Dsl_SecurityLevelHandler_SessionChange() */
#define DBG_DCM_DSL_SECURITYLEVEL_SESSIONCHANGE_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSL_SECURITYLEVEL_SESSIONCHANGE_EXIT
/** \brief Exit point of function Dcm_Dsl_SecurityLevelHandler_SessionChange() */
#define DBG_DCM_DSL_SECURITYLEVEL_SESSIONCHANGE_EXIT()
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_SESSIONCHANGE_ENTRY
/** \brief Entry point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_SessionChange() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_SESSIONCHANGE_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_SESSIONCHANGE_EXIT
/** \brief Exit point of function Dcm_Dsp_ReadDataByPeriodicIdentifier_SessionChange() */
#define DBG_DCM_DSP_READDATABYPERIODICIDENTIFIER_SESSIONCHANGE_EXIT()
#endif

#ifndef DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SESSIONCHANGE_ENTRY
/** \brief Entry point of function Dcm_Dsp_InputOutputControlByIdentifier_SessionChange() */
#define DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SESSIONCHANGE_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SESSIONCHANGE_EXIT
/** \brief Exit point of function Dcm_Dsp_InputOutputControlByIdentifier_SessionChange() */
#define DBG_DCM_DSP_INPUTOUTPUTCONTROLBYIDENTIFIER_SESSIONCHANGE_EXIT()
#endif

#ifndef DBG_DCM_RESETDTCSETTINGSTATUSONSESSIONCHANGE_ENTRY
/** \brief Entry point of function Dcm_ResetDTCSettingStatusOnSessionChange() */
#define DBG_DCM_RESETDTCSETTINGSTATUSONSESSIONCHANGE_ENTRY(a)
#endif

#ifndef DBG_DCM_RESETDTCSETTINGSTATUSONSESSIONCHANGE_EXIT
/** \brief Exit point of function Dcm_ResetDTCSettingStatusOnSessionChange() */
#define DBG_DCM_RESETDTCSETTINGSTATUSONSESSIONCHANGE_EXIT()
#endif

#ifndef DBG_DCM_DSP_CONTROLDTCSETTING_SESSIONCHANGE_ENTRY
/** \brief Entry point of function Dcm_Dsp_ControlDTCSetting_SessionChange() */
#define DBG_DCM_DSP_CONTROLDTCSETTING_SESSIONCHANGE_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_CONTROLDTCSETTING_SESSIONCHANGE_EXIT
/** \brief Exit point of function Dcm_Dsp_ControlDTCSetting_SessionChange() */
#define DBG_DCM_DSP_CONTROLDTCSETTING_SESSIONCHANGE_EXIT()
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_RESETONSESSIONCHANGE_ENTRY
/** \brief Entry point of function Dcm_Dsp_CommunicationControl_ResetOnSessionChange() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_RESETONSESSIONCHANGE_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_RESETONSESSIONCHANGE_EXIT
/** \brief Exit point of function Dcm_Dsp_CommunicationControl_ResetOnSessionChange() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_RESETONSESSIONCHANGE_EXIT()
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_SESSIONCHANGE_ENTRY
/** \brief Entry point of function Dcm_Dsp_CommunicationControl_SessionChange() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_SESSIONCHANGE_ENTRY(a,b)
#endif

#ifndef DBG_DCM_DSP_COMMUNICATIONCONTROL_SESSIONCHANGE_EXIT
/** \brief Exit point of function Dcm_Dsp_CommunicationControl_SessionChange() */
#define DBG_DCM_DSP_COMMUNICATIONCONTROL_SESSIONCHANGE_EXIT()
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_PROTOCOLHANDLING_GETNUMBEROFCONFIRMEDRCRRP_ENTRY
/** \brief Entry point of function Dcm_Dsl_Supervisor_ProtocolHandling_GetNumberOfConfirmedRCRRP() */
#define DBG_DCM_DSL_SUPERVISOR_PROTOCOLHANDLING_GETNUMBEROFCONFIRMEDRCRRP_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_PROTOCOLHANDLING_GETNUMBEROFCONFIRMEDRCRRP_EXIT
/** \brief Exit point of function Dcm_Dsl_Supervisor_ProtocolHandling_GetNumberOfConfirmedRCRRP() */
#define DBG_DCM_DSL_SUPERVISOR_PROTOCOLHANDLING_GETNUMBEROFCONFIRMEDRCRRP_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_PROTOCOLHANDLING_RCRRPSENT_ENTRY
/** \brief Entry point of function Dcm_Dsl_Supervisor_ProtocolHandling_RCRRPSent() */
#define DBG_DCM_DSL_SUPERVISOR_PROTOCOLHANDLING_RCRRPSENT_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_PROTOCOLHANDLING_RCRRPSENT_EXIT
/** \brief Exit point of function Dcm_Dsl_Supervisor_ProtocolHandling_RCRRPSent() */
#define DBG_DCM_DSL_SUPERVISOR_PROTOCOLHANDLING_RCRRPSENT_EXIT(a)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_PROTOCOLHANDLING_RCRRPTRANSMISSIONFINISHED_ENTRY
/** \brief Entry point of function Dcm_Dsl_Supervisor_ProtocolHandling_RCRRPTransmissionFinished() */
#define DBG_DCM_DSL_SUPERVISOR_PROTOCOLHANDLING_RCRRPTRANSMISSIONFINISHED_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_SUPERVISOR_PROTOCOLHANDLING_RCRRPTRANSMISSIONFINISHED_EXIT
/** \brief Exit point of function Dcm_Dsl_Supervisor_ProtocolHandling_RCRRPTransmissionFinished() */
#define DBG_DCM_DSL_SUPERVISOR_PROTOCOLHANDLING_RCRRPTRANSMISSIONFINISHED_EXIT(a,b)
#endif

#ifndef DBG_DCM_DSL_GETECUCMETADATAINFO_ENTRY
/** \brief Entry point of function Dcm_Dsl_GetEcuCMetaDataInfo() */
#define DBG_DCM_DSL_GETECUCMETADATAINFO_ENTRY(a)
#endif

#ifndef DBG_DCM_DSL_GETECUCMETADATAINFO_EXIT
/** \brief Exit point of function Dcm_Dsl_GetEcuCMetaDataInfo() */
#define DBG_DCM_DSL_GETECUCMETADATAINFO_EXIT()
#endif

#ifndef DBG_DCM_DSL_SETECUCMETADATAINFO_ENTRY
/** \brief Entry point of function Dcm_Dsl_SetEcuCMetaDataInfo() */
#define DBG_DCM_DSL_SETECUCMETADATAINFO_ENTRY(a,b,c)
#endif

#ifndef DBG_DCM_DSL_SETECUCMETADATAINFO_EXIT
/** \brief Exit point of function Dcm_Dsl_SetEcuCMetaDataInfo() */
#define DBG_DCM_DSL_SETECUCMETADATAINFO_EXIT()
#endif

#ifndef DBG_DCM_MAINFUNCTION_PART1_ENTRY
/** \brief Entry point of function Dcm_MainFunction_Part1() */
#define DBG_DCM_MAINFUNCTION_PART1_ENTRY()
#endif

#ifndef DBG_DCM_MAINFUNCTION_PART1_EXIT
/** \brief Exit point of function Dcm_MainFunction_Part1() */
#define DBG_DCM_MAINFUNCTION_PART1_EXIT()
#endif

#ifndef DBG_DCM_MAINFUNCTION_PART2_ENTRY
/** \brief Entry point of function Dcm_MainFunction_Part2() */
#define DBG_DCM_MAINFUNCTION_PART2_ENTRY()
#endif

#ifndef DBG_DCM_MAINFUNCTION_PART2_EXIT
/** \brief Exit point of function Dcm_MainFunction_Part2() */
#define DBG_DCM_MAINFUNCTION_PART2_EXIT()
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef DCM_TRACE_INTERNAL_H */
/*==================[end of file]===========================================*/
