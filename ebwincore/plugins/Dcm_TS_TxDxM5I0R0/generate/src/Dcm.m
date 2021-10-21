[!/**
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
 */!][!//

[!//
[!/*=== The code generating the internal service handler  name ===*/!][!//
[!//
[!MACRO "DCM_SERVICE_HANDLER_NAME"!][!/*
*/!][!IF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x10'"!][!/*
*/!]Dcm_DspInternal_DiagnosticSessionControl_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x11'"!][!/*
*/!]Dcm_DspInternal_ECUReset_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x14'"!][!/*
*/!]Dcm_DspInternal_ClearDiagnosticInformation_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x19'"!][!/*
*/!]Dcm_DspInternal_ReadDTCInformation_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x22'"!][!/*
*/!]Dcm_Dsp_ReadDataByIdentifier_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x23'"!][!/*
*/!]Dcm_Dsp_ReadMemoryByAddress_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x24'"!][!/*
*/!]Dcm_Dsp_ReadScalingDataByIdentifier_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x27'"!][!/*
*/!]Dcm_Dsp_SecurityAccess_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x28'"!][!/*
*/!]Dcm_Dsp_CommunicationControl_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x2a'"!][!/*
*/!]Dcm_Dsp_ReadDataByPeriodicIdentifier_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x2c'"!][!/*
*/!]Dcm_Dsp_DynamicallyDefineDataIdentifier_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x2e'"!][!/*
*/!]Dcm_Dsp_WriteDataByIdentifier_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x2f'"!][!/*
*/!]Dcm_Dsp_InputOutputControlByIdentifier_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x31'"!][!/*
*/!]Dcm_Dsp_RoutineControl_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x34'"!][!/*
*/!]Dcm_Dsp_RequestDownload_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x35'"!][!/*
*/!]Dcm_Dsp_RequestUpload_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x36'"!][!/*
*/!]Dcm_Dsp_TransferData_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x37'"!][!/*
*/!]Dcm_Dsp_RequestTransferExit_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x3d'"!][!/*
*/!]Dcm_Dsp_WriteMemoryByAddress_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x3e'"!][!/*
*/!]Dcm_Dsp_TesterPresent_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x85'"!][!/*
*/!]Dcm_Dsp_ControlDTCSetting_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x86'"!][!/*
*/!]Dcm_Dsp_ResponseOnEvent_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x87'"!][!/*
*/!]Dcm_Dsp_LinkControl_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0xaf'"!][!/*
*/!]Dcm_DspInternal_ReadGenericInformation_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x01'"!][!/*
*/!]Dcm_Dsp_RequestCurrentPowertrainDiagnosticData_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x02'"!][!/*
*/!]Dcm_Dsp_RequestPowertrainFreezeFrameData_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x03'"!][!/*
*/!]Dcm_DspInternal_RequestConfirmedDTC_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x04'"!][!/*
*/!]Dcm_DspInternal_ClearEmissionDiagInfo_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x06'"!][!/*
*/!]Dcm_Dsp_RequestOnBoardMonitoringTestResults_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x07'"!][!/*
*/!]Dcm_DspInternal_RequestPendingDTC_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x08'"!][!/*
*/!]Dcm_DspInternal_RequestControlOfOnBoardSysTstComp_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x09'"!][!/*
*/!]Dcm_DspInternal_RequestVehicleInformation_SvcH[!/*
*/!][!ELSEIF "num:inttohex( DcmDsdSidTabServiceId, 2) = '0x0a'"!][!/*
*/!]Dcm_DspInternal_RequestPermanentDTC_SvcH[!/*
*/!][!ENDIF!][!/*
*/!][!ENDMACRO!][!//
[!/*=== The code generating the internal Sub service handler  name ===*/!][!//
[!MACRO "DCM_SUB_SERVICE_HANDLER_NAME"!][!/*
*/!][!IF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x10'"!][!/*
*/!]Dcm_DspInternal_DiagnosticSessionControlCommon_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x19' and num:inttohex( DcmDsdSubServiceId, 2) = '0x01'"!][!/*
*/!]Dcm_DspInternal_ReportNumberOfDTCByStatusMask_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x19' and num:inttohex( DcmDsdSubServiceId, 2) = '0x02'"!][!/*
*/!]Dcm_DspInternal_ReportDTCByStatusMask_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x19' and num:inttohex( DcmDsdSubServiceId, 2) = '0x03'"!][!/*
*/!]Dcm_DspInternal_ReportDtcSnapshotRecordIdentification_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x19' and num:inttohex( DcmDsdSubServiceId, 2) = '0x04'"!][!/*
*/!]Dcm_DspInternal_ReportDtcSnapshotRecordByDtcNumber_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x19' and num:inttohex( DcmDsdSubServiceId, 2) = '0x06'"!][!/*
*/!]Dcm_DspInternal_ReportDtcExtendedDataRecordByDtcNumber_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x19' and num:inttohex( DcmDsdSubServiceId, 2) = '0x07'"!][!/*
*/!]Dcm_Dsp_ReadDTCInformation_ReportNumberOfDTCBySeverityMaskRecord_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x19' and num:inttohex( DcmDsdSubServiceId, 2) = '0x08'"!][!/*
*/!]Dcm_Dsp_ReadDTCInformation_ReportDTCBySeverityMaskRecord_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x19' and num:inttohex( DcmDsdSubServiceId, 2) = '0x09'"!][!/*
*/!]Dcm_Dsp_ReadDTCInformation_ReportSeverityInformationOfDTC_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x19' and num:inttohex( DcmDsdSubServiceId, 2) = '0x0a'"!][!/*
*/!]Dcm_DspInternal_ReportSupportedDtcs_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x19' and num:inttohex( DcmDsdSubServiceId, 2) = '0x0b'"!][!/*
*/!]Dcm_Dsp_ReadDTCInformation_ReportFirstTestFailedDTC_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x19' and num:inttohex( DcmDsdSubServiceId, 2) = '0x0c'"!][!/*
*/!]Dcm_Dsp_ReadDTCInformation_ReportFirstConfirmedDTC_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x19' and num:inttohex( DcmDsdSubServiceId, 2) = '0x0d'"!][!/*
*/!]Dcm_Dsp_ReadDTCInformation_ReportMostRecentTestFailedDTC_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x19' and num:inttohex( DcmDsdSubServiceId, 2) = '0x0e'"!][!/*
*/!]Dcm_Dsp_ReadDTCInformation_ReportMostRecentConfirmedDTC_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x19' and num:inttohex( DcmDsdSubServiceId, 2) = '0x13'"!][!/*
*/!]Dcm_DspInternal_ReportEmissionsRelatedOBDDTCByStatusMask_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x19' and num:inttohex( DcmDsdSubServiceId, 2) = '0x14'"!][!/*
*/!]Dcm_DspInternal_ReportDTCFaultDetectionCounter_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x19' and num:inttohex( DcmDsdSubServiceId, 2) = '0x15'"!][!/*
*/!]Dcm_DspInternal_ReportDTCWithPermanentStatus_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x19' and num:inttohex( DcmDsdSubServiceId, 2) = '0x17'"!][!/*
*/!]Dcm_DspInternal_ReportUserDefMemoryDTCByStatusMask_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x19' and num:inttohex( DcmDsdSubServiceId, 2) = '0x19'"!][!/*
*/!]Dcm_DspInternal_ReportUserDefMemoryDtcExtendedDataRecordByDtcNumber_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x19' and num:inttohex( DcmDsdSubServiceId, 2) = '0x18'"!][!/*
*/!]Dcm_DspInternal_ReportUserDefMemoryDtcSnapshotRecordByDtcNumber_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x85' and num:inttohex( DcmDsdSubServiceId, 2) = '0x01'"!][!/*
*/!]Dcm_Dsp_ControlDTCSetting_On_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x85' and num:inttohex( DcmDsdSubServiceId, 2) = '0x02'"!][!/*
*/!]Dcm_Dsp_ControlDTCSetting_Off_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x28' and num:inttohex( DcmDsdSubServiceId, 2) = '0x00'"!][!/*
*/!]Dcm_Dsp_CommunicationControl_EnableRxAndTx_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x28' and num:inttohex( DcmDsdSubServiceId, 2) = '0x01'"!][!/*
*/!]Dcm_Dsp_CommunicationControl_EnableRxAndDisableTx_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x28' and num:inttohex( DcmDsdSubServiceId, 2) = '0x02'"!][!/*
*/!]Dcm_Dsp_CommunicationControl_DisableRxAndEnableTx_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x28' and num:inttohex( DcmDsdSubServiceId, 2) = '0x03'"!][!/*
*/!]Dcm_Dsp_CommunicationControl_DisableRxAndTx_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x28' and num:inttohex( DcmDsdSubServiceId, 2) = '0x04'"!][!/*
*/!]Dcm_Dsp_CommunicationControl_EnableRxAndDisableTxWithEnhancedAddrInfo_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x28' and num:inttohex( DcmDsdSubServiceId, 2) = '0x05'"!][!/*
*/!]Dcm_Dsp_CommunicationControl_EnableRxAndTxWithEnhancedAddrInfo_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x3e' and num:inttohex( DcmDsdSubServiceId, 2) = '0x00'"!][!/*
*/!]Dcm_Dsp_TesterPresent_ZeroSubFunction_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x2c' and num:inttohex( DcmDsdSubServiceId, 2) = '0x01'"!][!/*
*/!]Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByDid_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x2c' and num:inttohex( DcmDsdSubServiceId, 2) = '0x02'"!][!/*
*/!]Dcm_Dsp_DynamicallyDefineDataIdentifierAddSrcByMem_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x2c' and num:inttohex( DcmDsdSubServiceId, 2) = '0x03'"!][!/*
*/!]Dcm_Dsp_DynamicallyDefineDataIdentifierClear_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x86' and num:inttohex( DcmDsdSubServiceId, 2) = '0x05'"!][!/*
*/!]Dcm_Dsp_ResponseOnEvent_StartResponseOnEventNonPersistent_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x86' and num:inttohex( DcmDsdSubServiceId, 2) = '0x45'"!][!/*
*/!]Dcm_Dsp_ResponseOnEvent_StartResponseOnEventPersistent_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x86' and num:inttohex( DcmDsdSubServiceId, 2) = '0x00'"!][!/*
*/!]Dcm_Dsp_ResponseOnEvent_StopResponseOnEventNonPersistent_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x86' and num:inttohex( DcmDsdSubServiceId, 2) = '0x40'"!][!/*
*/!]Dcm_Dsp_ResponseOnEvent_StopResponseOnEventPersistent_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x86' and num:inttohex( DcmDsdSubServiceId, 2) = '0x01'"!][!/*
*/!]Dcm_Dsp_ResponseOnEvent_OnDTCStatusChangeNonPersistent_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x86' and num:inttohex( DcmDsdSubServiceId, 2) = '0x41'"!][!/*
*/!]Dcm_Dsp_ResponseOnEvent_OnDTCStatusChangePersistent_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x86' and num:inttohex( DcmDsdSubServiceId, 2) = '0x03'"!][!/*
*/!]Dcm_Dsp_ResponseOnEvent_OnChangeOfDataIdentifierNonPersistent_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x86' and num:inttohex( DcmDsdSubServiceId, 2) = '0x43'"!][!/*
*/!]Dcm_Dsp_ResponseOnEvent_OnChangeOfDataIdentifierPersistent_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x86' and num:inttohex( DcmDsdSubServiceId, 2) = '0x04'"!][!/*
*/!]Dcm_Dsp_ResponseOnEvent_ReportActivatedEvents_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x86' and num:inttohex( DcmDsdSubServiceId, 2) = '0x06'"!][!/*
*/!]Dcm_Dsp_ResponseOnEvent_ClearResponseOnEvent_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x11' and num:inttohex( DcmDsdSubServiceId, 2) = '0x01'"!][!/*
*/!]Dcm_DspInternal_HardReset_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x11' and num:inttohex( DcmDsdSubServiceId, 2) = '0x02'"!][!/*
*/!]Dcm_DspInternal_KeyOffOnReset_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x11' and num:inttohex( DcmDsdSubServiceId, 2) = '0x03'"!][!/*
*/!]Dcm_DspInternal_SoftReset_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x11' and num:inttohex( DcmDsdSubServiceId, 2) = '0x04'"!][!/*
*/!]Dcm_DspInternal_EnableRapidPowerShutDown_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x11' and num:inttohex( DcmDsdSubServiceId, 2) = '0x05'"!][!/*
*/!]Dcm_DspInternal_DisableRapidPowerShutDown_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x87' and num:inttohex( DcmDsdSubServiceId, 2) = '0x01'"!][!/*
*/!]Dcm_Dsp_LinkControl_FixedBaudrateVerification_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x87' and num:inttohex( DcmDsdSubServiceId, 2) = '0x02'"!][!/*
*/!]Dcm_Dsp_LinkControl_SpecificBaudrateVerification_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0x87' and num:inttohex( DcmDsdSubServiceId, 2) = '0x03'"!][!/*
*/!]Dcm_Dsp_LinkControl_TransitionBaudrate_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0xaf' and num:inttohex( DcmDsdSubServiceId, 2) = '0x04'"!][!/*
*/!]Dcm_DspInternal_ReportDtcSnapshotRecordByDtcNumber_SSvcH[!/*
*/!][!ELSEIF "num:inttohex(../.././DcmDsdSidTabServiceId,2) ='0xaf' and num:inttohex( DcmDsdSubServiceId, 2) = '0x06'"!][!/*
*/!]Dcm_DspInternal_ReportDtcExtendedDataRecordByDtcNumber_SSvcH[!/*
*/!][!ENDIF!][!/*
*/!][!ENDMACRO!][!//
[!//
[!MACRO "GENERATE_IO_CONTROL_FUNC", "GenerateFunctions", "FuncPointer", "Mask", "WriteComment"!][!//
  [!IF "$GenerateFunctions = 'TRUE'"!][!//
    [!INDENT "2"!][!//
    [!"$WriteComment"!]
    {(Dcm_GeneralDidOperationFuncType)&[!"$FuncPointer"!]},
    [!ENDINDENT!][!//
  [!ELSE!][!//
    [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
    [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$Mask)"!][!//
  [!ENDIF!][!//
[!ENDMACRO!][!//
[!//
[!MACRO "GENERATE_IO_CONTROL_CS", "GenerateFunctions", "Operation", "Mask", "WriteComment"!][!//
  [!IF "$GenerateFunctions = 'TRUE'"!][!//
    [!INDENT "2"!][!//
    [!"$WriteComment"!]
    {(Dcm_GeneralDidOperationFuncType)&Rte_Call_DataServices_[!"$DataName"!]_[!"$Operation"!]},
    [!ENDINDENT!][!//
  [!ELSE!][!//
    [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
    [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$Mask)"!][!//
  [!ENDIF!][!//
[!ENDMACRO!][!//
[!//
[!VAR "MacUseBlockIDAccess" = "num:hextoint('0x0000')"!][!//
[!VAR "MacAMacSyncCSAccess" = "num:hextoint('0x0001')"!][!//
[!VAR "MacAMacSyncFncAccess" = "num:hextoint('0x0002')"!][!//
[!VAR "MacSRAccess" = "num:hextoint('0x0003')"!][!//
[!VAR "MacSyncCSAccess" = "num:hextoint('0x0004')"!][!//
[!VAR "MacSyncFncAccess" = "num:hextoint('0x0005')"!][!//
[!VAR "MacECUSignalAccess" = "num:hextoint('0x0006')"!][!//
[!VAR "MacFixedLengthMask" = "num:hextoint('0x0008')"!][!//
[!VAR "MacDataTypeBoolMask" = "num:hextoint('0x0000')"!][!//
[!VAR "MacDataTypeSint16Mask" = "num:hextoint('0x0010')"!][!//
[!VAR "MacDataTypeSint16NMask" = "num:hextoint('0x0020')"!][!//
[!VAR "MacDataTypeSint32Mask" = "num:hextoint('0x0030')"!][!//
[!VAR "MacDataTypeSint32NMask" = "num:hextoint('0x0040')"!][!//
[!VAR "MacDataTypeSint8Mask" = "num:hextoint('0x0050')"!][!//
[!VAR "MacDataTypeSint8NMask" = "num:hextoint('0x0060')"!][!//
[!VAR "MacDataTypeUint16Mask" = "num:hextoint('0x0070')"!][!//
[!VAR "MacDataTypeUint16NMask" = "num:hextoint('0x0080')"!][!//
[!VAR "MacDataTypeUint32Mask" = "num:hextoint('0x0090')"!][!//
[!VAR "MacDataTypeUint32NMask" = "num:hextoint('0x00A0')"!][!//
[!VAR "MacDataTypeUint8Mask" = "num:hextoint('0x00B0')"!][!//
[!VAR "MacDataTypeUint8NMask" = "num:hextoint('0x00C0')"!][!//
[!VAR "MacReadDataMask" = "num:hextoint('0x0100')"!][!//
[!VAR "MacReadDataLengthMask" = "num:hextoint('0x0200')"!][!//
[!VAR "MacCondCheckReadMask" = "num:hextoint('0x0400')"!][!//
[!VAR "MacWriteDataMask" = "num:hextoint('0x0800')"!][!//
[!VAR "MacShTermAdjMask" = "num:hextoint('0x1000')"!][!//
[!VAR "MacIoCtrlFreeze" = "num:hextoint('0x2000')"!][!//
[!VAR "MacIoCtrlReset" = "num:hextoint('0x4000')"!][!//
[!VAR "MacIoCtrlReturn" = "num:hextoint('0x8000')"!][!//
[!VAR "MacSRReadDataMask" = "num:hextoint('0x0100')"!][!//
[!VAR "MacSRWriteDataMask" = "num:hextoint('0x0200')"!][!//
[!VAR "MacSREndiannessBigEndianMask" = "num:hextoint('0x2000')"!][!//
[!VAR "MacSREndiannessLittleEndianMask" = "num:hextoint('0x4000')"!][!//
[!VAR "MacSREndiannessOpaqueEndianMask" = "num:hextoint('0x6000')"!][!//
[!VAR "MacReadScalingDataMask" = "num:hextoint('0x10000')"!][!//
[!//
[!VAR "DcmDspDataReadAccessList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidRead)]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
[!VAR "DcmDspDataWriteAccessList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidWrite)]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
[!VAR "DcmDspDataControlAccessList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidControl) and ((as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidResetToDefault = 'true') or (as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidFreezeCurrentState = 'true') or  (as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidReturnControlToEcu = 'true') or (as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidShortTermAdjustment = 'true'))]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
[!VAR "DcmDspDataFreezeCurrentStateList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidControl) and (as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidFreezeCurrentState = 'true')]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
[!VAR "DcmDspDataResetToDefaultList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidControl) and (as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidResetToDefault = 'true')]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
[!VAR "DcmDspDataShortTermAdjList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidControl) and (as:ref(DcmDspDidInfoRef)/*/DcmDspDidControl/DcmDspDidShortTermAdjustment = 'true')]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
[!VAR "DcmDspDataReadScalingDataAccessList" = "text:join(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[node:exists(node:ref(DcmDspDidInfoRef)/DcmDspDidAccess/DcmDspDidReadScalingData)]/DcmDspDidSignal/*/DcmDspDidDataRef)"!][!//
[!//
[!MACRO "HANDLE_DID_DATA", "GenerateFunctions", "DidDataPosition"!][!//
[!INDENT "0"!][!//
  [!VAR "MacDidDataDescriptor" = "num:hextoint('0x00')"!][!//
  [!IF "$GenerateFunctions = 'FALSE'"!][!//
    [!WS "2"!]{
    [!INDENT "4"!][!//
      [!IF "not(DcmDspDataUsePort = 'USE_BLOCK_ID')"!][!//
        /* Index in Dcm_DidDataTable */
        [!"num:i($DidDataPosition)"!]U,
      [!ELSE!][!//
        /* Index in Dcm_DidDataNvMBlockConfig */
        [!"node:ref(DcmDspDataBlockIdRef)/NvMNvramBlockIdentifier"!]U,
      [!ENDIF!][!//
    [!ENDINDENT!][!//
  [!ENDIF!][!//
  [!VAR "ReadAccess" = "'FALSE'"!][!//
  [!VAR "WriteAccess" = "'FALSE'"!][!//
  [!VAR "ControlAccess" = "'FALSE'"!][!//
  [!VAR "ReadScalingDataAccess" = "'FALSE'"!][!//
  [!VAR "CurrentNode_aspath" = "as:path(.,true())"!][!//
  [!IF "text:match($DcmDspDataReadAccessList,'ASPath')"!][!//
    [!IF "text:contains(text:split($DcmDspDataReadAccessList), $CurrentNode_aspath)"!][!//
      [!VAR "ReadAccess" = "'TRUE'"!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
  [!/* Check that DspData element is refered in a Did, which has the ReadScalinData access*/!][!//
  [!IF "text:match($DcmDspDataReadScalingDataAccessList,'ASPath')"!][!//
    [!IF "text:contains(text:split($DcmDspDataReadScalingDataAccessList), $CurrentNode_aspath)"!][!//
      [!VAR "ReadScalingDataAccess" = "'TRUE'"!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
  [!/* Check that DspData element is refered in a Did, which has the write access*/!][!//
  [!IF "text:match($DcmDspDataWriteAccessList,'ASPath')"!][!//
    [!IF "text:contains(text:split($DcmDspDataWriteAccessList), $CurrentNode_aspath)"!][!//
      [!VAR "WriteAccess" = "'TRUE'"!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
  [!/* Check that DspData element is refered in a Did, which has the Control access*/!][!//
  [!IF "text:match($DcmDspDataControlAccessList,'ASPath')"!][!//
    [!IF "text:contains(text:split($DcmDspDataControlAccessList), $CurrentNode_aspath)"!][!//
      [!VAR "ControlAccess" = "'TRUE'"!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
  [!IF "(DcmDspDataUsePort = 'USE_DATA_SENDER_RECEIVER') or (DcmDspDataUsePort = 'USE_DATA_SENDER_RECEIVER_AS_SERVICE')"!][!//
    [!IF "$GenerateFunctions = 'TRUE'"!]
    [!INDENT "2"!][!//
    /* Available function pointers for [!"node:name(.)"!] */
      [!IF "$ReadAccess = 'TRUE'"!][!//
        /* SenderReceiver Read operation */
        {(Dcm_GeneralDidOperationFuncType)&Rte_Read_DataServices_[!"node:name(.)"!]_data},
      [!ENDIF!][!//
      [!IF "$WriteAccess = 'TRUE'"!][!//
        /* SenderReceiver Write operation */
        {(Dcm_GeneralDidOperationFuncType)&Rte_Write_DataServices_[!"node:name(.)"!]_data},
      [!ENDIF!][!//
    [!ENDINDENT!][!//
    [!ELSE!][!//
      [!IF "$ReadAccess = 'TRUE'"!][!//
        [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
        [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacSRReadDataMask)"!][!//
      [!ENDIF!][!//
      [!IF "$WriteAccess = 'TRUE'"!][!//
        [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
        [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacSRWriteDataMask)"!][!//
      [!ENDIF!][!//
      [!IF "node:exists(DcmDspDataEndianness)"!][!//
        [!IF "DcmDspDataEndianness = 'BIG_ENDIAN'"!][!//
          [!/* !LINKSTO Dcm.EB.EndiannessInterpretation.DataEndianness.BigEndian,1 */!][!//
          [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacSREndiannessBigEndianMask)"!][!//
        [!ELSEIF "DcmDspDataEndianness = 'LITTLE_ENDIAN'"!][!//
          [!/* !LINKSTO Dcm.EB.EndiannessInterpretation.DataEndianness.LittleEndian,1 */!][!//
          [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacSREndiannessLittleEndianMask)"!][!//
        [!ELSE!][!//
          [!/* !LINKSTO Dcm.EB.EndiannessInterpretation.OpaqueDataEndianness.CpuLittleEndian,1 */!][!//
          [!/* !LINKSTO Dcm.EB.EndiannessInterpretation.OpaqueDataEndianness.CpuBigEndian,1 */!][!//
          [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacSREndiannessOpaqueEndianMask)"!][!//
        [!ENDIF!][!//
      [!ELSE!][!//
        [!VAR "DefaultEndianness" = "../../../DcmGeneral/DcmDspDataDefaultEndianness"!][!//
        [!IF "$DefaultEndianness = 'BIG_ENDIAN'"!][!//
          [!/* !LINKSTO Dcm.EB.EndiannessInterpretation.DefaultEndianness.BigEndian,1 */!][!//
          [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacSREndiannessBigEndianMask)"!][!//
        [!ELSEIF "$DefaultEndianness = 'LITTLE_ENDIAN'"!][!//
          [!/* !LINKSTO Dcm.EB.EndiannessInterpretation.DefaultEndianness.LittleEndian,1 */!][!//
          [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacSREndiannessLittleEndianMask)"!][!//
        [!ELSE!][!//
          [!/* !LINKSTO Dcm.EB.EndiannessInterpretation.OpaqueDefaultEndianness.CpuLittleEndian,1 */!][!//
          [!/* !LINKSTO Dcm.EB.EndiannessInterpretation.OpaqueDefaultEndianness.CpuBigEndian,1 */!][!//
          [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacSREndiannessOpaqueEndianMask)"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ELSE!][!//
    [!/* !LINKSTO Dcm.EB.ServiceAPI.DataServices,1 */!][!//
    [!IF "contains ($DefaultASRVersion, 'AUTOSAR_42')"!][!//
      [!VAR "RteConnectedASRVersion" = "'DCM_USE_AUTOSAR_42'"!][!//
    [!ELSE!][!//
      [!VAR "RteConnectedASRVersion" = "'DCM_USE_AUTOSAR_40'"!][!//
    [!ENDIF!][!//
    [!//
      [!IF "$GenerateFunctions = 'TRUE'"!]
      [!WS "2"!]/* Available function pointers for [!"node:name(.)"!] */
      [!ENDIF!][!//
      [!INDENT "2"!][!//
      [!/*---ConditionCheckRead------*/!][!//
      [!IF "DcmDspDataUsePort = 'USE_DATA_SYNCH_FNC'"!][!//
        [!IF "node:exists(DcmDspDataConditionCheckReadFnc) and DcmDspDataConditionCheckReadFncUsed = 'true'"!][!//
          [!IF "$GenerateFunctions = 'TRUE'"!][!//
            /* Ptr to ConditionCheckRead() for user sync function */
            {(Dcm_GeneralDidOperationFuncType)&[!"DcmDspDataConditionCheckReadFnc"!]},
          [!ELSE!][!//
            [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
            [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacCondCheckReadMask)"!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
      [!ELSEIF "DcmDspDataUsePort = 'USE_DATA_ASYNCH_FNC'"!][!//
        [!IF "node:exists(DcmDspDataConditionCheckReadFnc) and DcmDspDataConditionCheckReadFncUsed = 'true'"!][!//
          [!IF "$GenerateFunctions = 'TRUE'"!][!//
            /* Ptr to ConditionCheckRead() for user async function */
            {(Dcm_GeneralDidOperationFuncType)&[!"DcmDspDataConditionCheckReadFnc"!]},
          [!ELSE!][!//
            [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
            [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacCondCheckReadMask)"!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
      [!ELSEIF "(DcmDspDataUsePort = 'USE_DATA_SYNCH_CLIENT_SERVER') and (($ReadAccess = 'TRUE') or ($ControlAccess = 'TRUE')) and (DcmDspDataConditionCheckReadFncUsed = 'true')"!][!//
        [!IF "$GenerateFunctions = 'TRUE'"!][!//
          /* Ptr to ConditionCheckRead() for sync operation client-server */
          {(Dcm_GeneralDidOperationFuncType)&Rte_Call_DataServices_[!"node:name(.)"!]_ConditionCheckRead},
        [!ELSE!][!//
          [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
          [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacCondCheckReadMask)"!][!//
        [!ENDIF!][!//
      [!ELSEIF "(DcmDspDataUsePort = 'USE_DATA_ASYNCH_CLIENT_SERVER') and (($ReadAccess = 'TRUE') or ($ControlAccess = 'TRUE')) and (DcmDspDataConditionCheckReadFncUsed = 'true')"!][!//
        [!IF "$GenerateFunctions = 'TRUE'"!][!//
          /* ptr to ConditionCheckRead() for async operation client server */
          {(Dcm_GeneralDidOperationFuncType)&Rte_Call_DataServices_[!"node:name(.)"!]_ConditionCheckRead},
        [!ELSE!][!//
          [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
          [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacCondCheckReadMask)"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!/*---ReadDataLength------*/!][!//
    [!IF "DcmDspDataUsePort = 'USE_DATA_SYNCH_FNC'"!][!//
      [!IF "node:exists(DcmDspDataReadDataLengthFnc)"!][!//
        [!IF "$GenerateFunctions = 'TRUE'"!][!//
          /* ReadDataLength sync function */
          {(Dcm_GeneralDidOperationFuncType)&[!"DcmDspDataReadDataLengthFnc"!]},
        [!ELSE!][!//
          [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
          [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacReadDataLengthMask)"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ELSEIF "DcmDspDataUsePort = 'USE_DATA_ASYNCH_FNC'"!][!//
      [!IF "node:exists(DcmDspDataReadDataLengthFnc)"!][!//
        [!IF "contains ($RteConnectedASRVersion, 'DCM_USE_AUTOSAR_42')"!][!//
          [!IF "$GenerateFunctions = 'TRUE'"!][!//
            /* ReadDataLength - DCM_USE_AUTOSAR_42 True*/
            {(Dcm_GeneralDidOperationFuncType)&[!"DcmDspDataReadDataLengthFnc"!]},
          [!ELSE!][!//
            [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
            [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacReadDataLengthMask)"!][!//
          [!ENDIF!][!//
        [!ELSE!][!//
          [!IF "$GenerateFunctions = 'TRUE'"!][!//
            /* ReadDataLength - DCM_USE_AUTOSAR_42 False */
            {(Dcm_GeneralDidOperationFuncType)&[!"DcmDspDataReadDataLengthFnc"!]},
          [!ELSE!][!//
            [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
            [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacReadDataLengthMask)"!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ELSEIF "(DcmDspDataUsePort = 'USE_DATA_SYNCH_CLIENT_SERVER') and (node:ref(DcmDspDataInfoRef)/DcmDspDataFixedLength = 'false') and (($ReadAccess = 'TRUE') or ($ControlAccess = 'TRUE'))"!][!//
      [!IF "$GenerateFunctions = 'TRUE'"!][!//
        /* ReadDataLength sync c-s */
        {(Dcm_GeneralDidOperationFuncType)&Rte_Call_DataServices_[!"node:name(.)"!]_ReadDataLength},
      [!ELSE!][!//
        [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
        [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacReadDataLengthMask)"!][!//
      [!ENDIF!][!//
    [!ELSEIF "(DcmDspDataUsePort = 'USE_DATA_ASYNCH_CLIENT_SERVER') and (node:ref(DcmDspDataInfoRef)/DcmDspDataFixedLength = 'false') and (($ReadAccess = 'TRUE') or ($ControlAccess = 'TRUE'))"!][!//
      [!IF "contains ($RteConnectedASRVersion, 'DCM_USE_AUTOSAR_42')"!][!//
        [!IF "$GenerateFunctions = 'TRUE'"!][!//
          /* ReadDataLength async c-s - DCM_USE_AUTOSAR_42 True*/
          {(Dcm_GeneralDidOperationFuncType)&Rte_Call_DataServices_[!"node:name(.)"!]_ReadDataLength},
        [!ELSE!]
          [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
          [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacReadDataLengthMask)"!][!//
        [!ENDIF!][!//
      [!ELSE!][!//
        [!IF "$GenerateFunctions = 'TRUE'"!][!//
          /* ReadDataLength async c-s - DCM_USE_AUTOSAR_42 False */
          {(Dcm_GeneralDidOperationFuncType)&Rte_Call_DataServices_[!"node:name(.)"!]_ReadDataLength},
        [!ELSE!][!//
          [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
          [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacReadDataLengthMask)"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
    [!/*---ReadDataFnc------*/!][!//
    [!IF "DcmDspDataUsePort = 'USE_DATA_SYNCH_FNC'"!][!//
      [!IF "node:exists(DcmDspDataReadFnc)"!][!//
        [!IF "$GenerateFunctions = 'TRUE'"!][!//
          /* ReadData sync function */
          {(Dcm_GeneralDidOperationFuncType)&[!"DcmDspDataReadFnc"!]},
        [!ELSE!][!//
          [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
          [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacReadDataMask)"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ELSEIF "(DcmDspDataUsePort = 'USE_DATA_SYNCH_CLIENT_SERVER') and (($ReadAccess = 'TRUE') or ($ControlAccess = 'TRUE'))"!][!//
      [!IF "$GenerateFunctions = 'TRUE'"!][!//
        /* ReadData sync c-s */
        {(Dcm_GeneralDidOperationFuncType)&Rte_Call_DataServices_[!"node:name(.)"!]_ReadData},
      [!ELSE!][!//
        [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
        [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacReadDataMask)"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
    [!/*---CallReadData ------*/!][!//
    [!IF "DcmDspDataUsePort = 'USE_DATA_ASYNCH_FNC'"!][!//
      [!IF "node:exists(DcmDspDataReadFnc)"!][!//
        [!IF "$GenerateFunctions = 'TRUE'"!][!//
          /* CallReadData async function */
          {(Dcm_GeneralDidOperationFuncType)&[!"DcmDspDataReadFnc"!]},
        [!ELSE!][!//
          [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
          [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacReadDataMask)"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ELSEIF "(DcmDspDataUsePort = 'USE_DATA_ASYNCH_CLIENT_SERVER') and (($ReadAccess = 'TRUE') or ($ControlAccess = 'TRUE'))"!][!//
      [!IF "$GenerateFunctions = 'TRUE'"!][!//
        /* CallReadData async c-s */
        {(Dcm_GeneralDidOperationFuncType)&Rte_Call_DataServices_[!"node:name(.)"!]_ReadData},
      [!ELSE!][!//
        [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
        [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacReadDataMask)"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
    [!/*---WriteFixedData ------*/!][!//
    [!IF "node:ref(DcmDspDataInfoRef)/DcmDspDataFixedLength = 'true'"!][!//
      [!IF "DcmDspDataUsePort = 'USE_DATA_SYNCH_FNC'"!][!//
        [!IF "node:exists(DcmDspDataWriteFnc)"!][!//
          [!IF "$GenerateFunctions = 'TRUE'"!][!//
            /* Write sync fixed length */
            {(Dcm_GeneralDidOperationFuncType)&[!"DcmDspDataWriteFnc"!]},
          [!ELSE!][!//
            [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
            [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacWriteDataMask)"!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
      [!ELSEIF "(DcmDspDataUsePort = 'USE_DATA_SYNCH_CLIENT_SERVER') and ($WriteAccess = 'TRUE')"!][!//
        [!IF "$GenerateFunctions = 'TRUE'"!][!//
          /* Write sync c-s fixed length */
          {(Dcm_GeneralDidOperationFuncType)&Rte_Call_DataServices_[!"node:name(.)"!]_WriteData},
        [!ELSE!][!//
          [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
          [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacWriteDataMask)"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
    [!/*--- WriteVariableData------*/!][!//
    [!IF "node:ref(DcmDspDataInfoRef)/DcmDspDataFixedLength = 'false'"!][!//
      [!IF "DcmDspDataUsePort = 'USE_DATA_SYNCH_FNC'"!][!//
        [!IF "node:exists(DcmDspDataWriteFnc)"!][!//
          [!IF "$GenerateFunctions = 'TRUE'"!][!//
            /* Write sync variable length */
            {(Dcm_GeneralDidOperationFuncType)&[!"DcmDspDataWriteFnc"!]},
          [!ELSE!][!//
            [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
            [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacWriteDataMask)"!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
      [!ELSEIF "(DcmDspDataUsePort = 'USE_DATA_SYNCH_CLIENT_SERVER') and ($WriteAccess = 'TRUE')"!][!//
        [!IF "$GenerateFunctions = 'TRUE'"!][!//
          /* Write c-s variable length */
          {(Dcm_GeneralDidOperationFuncType)&Rte_Call_DataServices_[!"node:name(.)"!]_WriteData},
        [!ELSE!][!//
          [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
          [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacWriteDataMask)"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
    [!/*---CallWriteFixedLength, CallWriteVarLength , ResultWriteData------*/!][!//
    [!IF "(DcmDspDataUsePort = 'USE_DATA_ASYNCH_CLIENT_SERVER')  and ($WriteAccess = 'TRUE')"!][!//
      [!IF "node:ref(DcmDspDataInfoRef)/DcmDspDataFixedLength = 'true'"!][!//
        [!IF "$GenerateFunctions = 'TRUE'"!][!//
          /* Write async c-s fixed length */
          {(Dcm_GeneralDidOperationFuncType)&Rte_Call_DataServices_[!"node:name(.)"!]_WriteData},
        [!ELSE!][!//
          [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
          [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacWriteDataMask)"!][!//
        [!ENDIF!][!//
      [!ELSE!][!//
        [!IF "$GenerateFunctions = 'TRUE'"!][!//
          /* Write async c-s variable length */
          {(Dcm_GeneralDidOperationFuncType)&Rte_Call_DataServices_[!"node:name(.)"!]_WriteData},
        [!ELSE!][!//
          [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
          [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacWriteDataMask)"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!/*--- Rte_Result_DataServices_[!"node:name(.)"!]_WriteData, -----*/!][!//
    [!ELSEIF "DcmDspDataUsePort = 'USE_DATA_ASYNCH_FNC'"!][!//
      [!IF "node:exists(DcmDspDataWriteFnc)"!][!//
        [!IF "node:ref(DcmDspDataInfoRef)/DcmDspDataFixedLength = 'true'"!][!//
          [!IF "$GenerateFunctions = 'TRUE'"!][!//
            /* Write async fixed length */
            {(Dcm_GeneralDidOperationFuncType)&[!"DcmDspDataWriteFnc"!]},
          [!ELSE!][!//
            [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
            [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacWriteDataMask)"!][!//
          [!ENDIF!][!//
        [!ELSE!][!//
          [!IF "$GenerateFunctions = 'TRUE'"!][!//
            /* Write async variable length */
            {(Dcm_GeneralDidOperationFuncType)&[!"DcmDspDataWriteFnc"!]},
          [!ELSE!][!//
            [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
            [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacWriteDataMask)"!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!/*--- Rte_Result_DataServices_[!"node:name(.)"!]_WriteData, -----*/!][!//
    [!ENDIF!][!//
[!IF "$DCM_DSP_USE_SERVICE_0X24 = 'STD_ON'"!][!//
    [!IF "DcmDspDataUsePort = 'USE_DATA_SYNCH_FNC'"!][!//
      [!IF "node:exists(DcmDspDataGetScalingInfoFnc)"!][!//
        [!IF "node:exists(node:ref(DcmDspDataInfoRef)/DcmDspDataScalingInfoSize)"!][!//
          [!IF "$GenerateFunctions = 'TRUE'"!][!//
            /* GetScalingInfoFnc sync function */
            {(Dcm_GeneralDidOperationFuncType)&[!"DcmDspDataGetScalingInfoFnc"!]},
          [!ELSE!][!//
            [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
            [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacReadScalingDataMask)"!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ELSEIF "DcmDspDataUsePort = 'USE_DATA_ASYNCH_FNC'"!][!//
      [!IF "node:exists(DcmDspDataGetScalingInfoFnc)"!][!//
        [!IF "node:exists(node:ref(DcmDspDataInfoRef)/DcmDspDataScalingInfoSize)"!][!//
          [!IF "$GenerateFunctions = 'TRUE'"!][!//
            /* GetScalingInfoFnc async function */
            {(Dcm_GeneralDidOperationFuncType)&[!"DcmDspDataGetScalingInfoFnc"!]},
          [!ELSE!][!//
            [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
            [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacReadScalingDataMask)"!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ELSEIF "(DcmDspDataUsePort = 'USE_DATA_SYNCH_CLIENT_SERVER') and ($ReadScalingDataAccess = 'TRUE')"!][!//
        [!IF "node:exists(node:ref(DcmDspDataInfoRef)/DcmDspDataScalingInfoSize)"!][!//
          [!IF "$GenerateFunctions = 'TRUE'"!][!//
            /* ReadScalingData sync c-s */
            {(Dcm_GeneralDidOperationFuncType)&Rte_Call_DataServices_[!"node:name(.)"!]_GetScalingInformation},
          [!ELSE!][!//
            [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
            [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacReadScalingDataMask)"!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
    [!ELSEIF "(DcmDspDataUsePort = 'USE_DATA_ASYNCH_CLIENT_SERVER') and ($ReadScalingDataAccess = 'TRUE')"!][!//
        [!IF "node:exists(node:ref(DcmDspDataInfoRef)/DcmDspDataScalingInfoSize)"!][!//
          [!IF "$GenerateFunctions = 'TRUE'"!][!//
            /* ReadScalingData async c-s */
            {(Dcm_GeneralDidOperationFuncType)&Rte_Call_DataServices_[!"node:name(.)"!]_GetScalingInformation},
          [!ELSE!][!//
            [!VAR "DidDataPosition" = "$DidDataPosition + 1"!][!//
            [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacReadScalingDataMask)"!][!//
          [!ENDIF!][!//
        [!ENDIF!][!//
    [!ENDIF!][!//
[!ENDIF!][!//
[!IF "$DCM_DSP_USE_SERVICE_0X2F = 'STD_ON'"!][!//
  [!VAR "DataName" = "node:name(.)"!][!//
  [!VAR "DataUsePort" = "DcmDspDataUsePort"!][!//
  [!IF "text:match($DcmDspDataFreezeCurrentStateList,'ASPath')"!][!//
    [!IF "text:contains(text:split($DcmDspDataFreezeCurrentStateList), $CurrentNode_aspath)"!][!//
      [!IF "contains($DataUsePort, '_FNC')"!][!//
        [!CALL "GENERATE_IO_CONTROL_FUNC", "GenerateFunctions" = "$GenerateFunctions", "FuncPointer" = "./DcmDspDataFreezeCurrentStateFnc", "Mask" = "$MacIoCtrlFreeze", "WriteComment" = "'/* Freeze function */'"!][!//
      [!ELSEIF "contains($DataUsePort, '_CLIENT_SERVER')"!][!//
        [!CALL "GENERATE_IO_CONTROL_CS", "GenerateFunctions" = "$GenerateFunctions", "Operation" = "'FreezeCurrentState'", "Mask" = "$MacIoCtrlFreeze", "WriteComment" = "'/* Freeze c-s */'"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
  [!IF "text:match($DcmDspDataShortTermAdjList,'ASPath')"!][!//
    [!IF "text:contains(text:split($DcmDspDataShortTermAdjList), $CurrentNode_aspath)"!][!//
      [!IF "contains($DataUsePort, '_FNC')"!][!//
        [!CALL "GENERATE_IO_CONTROL_FUNC", "GenerateFunctions" = "$GenerateFunctions", "FuncPointer" = "./DcmDspDataShortTermAdjustmentFnc", "Mask" = "$MacShTermAdjMask", "WriteComment" = "'/* Short term adj function */'"!][!//
      [!ELSEIF "contains($DataUsePort, '_CLIENT_SERVER')"!][!//
        [!CALL "GENERATE_IO_CONTROL_CS", "GenerateFunctions" = "$GenerateFunctions", "Operation" = "'ShortTermAdjustment'", "Mask" = "$MacShTermAdjMask", "WriteComment" = "'/* Short term adj c-s */'"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
  [!IF "text:match($DcmDspDataResetToDefaultList,'ASPath')"!][!//
    [!IF "text:contains(text:split($DcmDspDataResetToDefaultList), $CurrentNode_aspath)"!][!//
      [!IF "contains($DataUsePort, '_FNC')"!][!//
        [!CALL "GENERATE_IO_CONTROL_FUNC", "GenerateFunctions" = "$GenerateFunctions", "FuncPointer" = "./DcmDspDataResetToDefaultFnc", "Mask" = "$MacIoCtrlReset", "WriteComment" = "'/* Reset to default function */'"!][!//
      [!ELSEIF "contains($DataUsePort, '_CLIENT_SERVER')"!][!//
        [!CALL "GENERATE_IO_CONTROL_CS", "GenerateFunctions" = "$GenerateFunctions", "Operation" = "'ResetToDefault'", "Mask" = "$MacIoCtrlReset", "WriteComment" = "'/* Reset to default c-s */'"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
  [!IF "text:match($DcmDspDataControlAccessList,'ASPath')"!][!//
    [!IF "text:contains(text:split($DcmDspDataControlAccessList), $CurrentNode_aspath)"!][!//
      [!IF "contains($DataUsePort, '_FNC')"!][!//
        [!CALL "GENERATE_IO_CONTROL_FUNC", "GenerateFunctions" = "$GenerateFunctions", "FuncPointer" = "./DcmDspDataReturnControlToEcuFnc", "Mask" = "$MacIoCtrlReturn", "WriteComment" = "'/* ReturnControlToEcu function */'"!][!//
      [!ELSEIF "contains($DataUsePort, '_CLIENT_SERVER')"!][!//
        [!CALL "GENERATE_IO_CONTROL_CS", "GenerateFunctions" = "$GenerateFunctions", "Operation" = "'ReturnControlToECU'", "Mask" = "$MacIoCtrlReturn", "WriteComment" = "'/* ReturnControlToEcu c-s */'"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
[!ENDIF!][!//
  [!ENDINDENT!][!//
[!ENDIF!][!//
[!ENDINDENT!][!//
  [!/*---DataType------*/!][!//
  [!IF "node:exists(DcmDspDataType)"!][!//
    [!IF "DcmDspDataType ='BOOLEAN'"!][!//
      [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacDataTypeBoolMask)"!][!//
    [!ELSEIF "DcmDspDataType ='UINT8'"!][!//
      [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacDataTypeUint8Mask)"!][!//
    [!ELSEIF "DcmDspDataType ='SINT8'"!][!//
      [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacDataTypeSint8Mask)"!][!//
    [!ELSEIF "DcmDspDataType ='UINT8_N'"!][!//
      [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacDataTypeUint8NMask)"!][!//
    [!ELSEIF "DcmDspDataType ='SINT8_N'"!][!//
      [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacDataTypeSint8NMask)"!][!//
    [!ELSEIF "DcmDspDataType ='UINT16'"!][!//
      [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacDataTypeUint16Mask)"!][!//
    [!ELSEIF "DcmDspDataType ='SINT16'"!][!//
      [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacDataTypeSint16Mask)"!][!//
    [!ELSEIF "DcmDspDataType ='UINT16_N'"!][!//
      [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacDataTypeUint16NMask)"!][!//
    [!ELSEIF "DcmDspDataType ='SINT16_N'"!][!//
      [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacDataTypeSint16NMask)"!][!//
    [!ELSEIF "DcmDspDataType ='UINT32'"!][!//
      [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacDataTypeUint32Mask)"!][!//
    [!ELSEIF "DcmDspDataType ='SINT32'"!][!//
      [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacDataTypeSint32Mask)"!][!//
    [!ELSEIF "DcmDspDataType ='UINT32_N'"!][!//
      [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacDataTypeUint32NMask)"!][!//
    [!ELSEIF "DcmDspDataType ='SINT32_N'"!][!//
      [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacDataTypeSint32NMask)"!][!//
    [!ENDIF!][!//
  [!ELSE!][!//
    [!/* !LINKSTO Dcm.EB.Config.DcmDspDataType.Default,1 */!][!//
    [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacDataTypeUint8Mask)"!][!//
  [!ENDIF!][!//
  [!/*---AccessInterface------*/!][!//
  [!IF "DcmDspDataUsePort ='USE_BLOCK_ID'"!][!//
    [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacUseBlockIDAccess)"!][!//
  [!ELSEIF "DcmDspDataUsePort ='USE_DATA_ASYNCH_CLIENT_SERVER'"!][!//
    [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacAMacSyncCSAccess)"!][!//
  [!ELSEIF "DcmDspDataUsePort ='USE_DATA_ASYNCH_FNC'"!][!//
    [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacAMacSyncFncAccess)"!][!//
  [!ELSEIF "(DcmDspDataUsePort ='USE_DATA_SENDER_RECEIVER') or (DcmDspDataUsePort ='USE_DATA_SENDER_RECEIVER_AS_SERVICE')"!][!//
    [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacSRAccess)"!][!//
  [!ELSEIF "DcmDspDataUsePort ='USE_DATA_SYNCH_CLIENT_SERVER'"!][!//
    [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacSyncCSAccess)"!][!//
  [!ELSEIF "DcmDspDataUsePort ='USE_DATA_SYNCH_FNC'"!][!//
    [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacSyncFncAccess)"!][!//
  [!ELSEIF "DcmDspDataUsePort ='USE_ECU_SIGNAL'"!][!//
    [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacSyncFncAccess)"!][!//
  [!ENDIF!][!//
  [!/*---FixedLenth------*/!][!//
  [!IF "node:ref(DcmDspDataInfoRef)/DcmDspDataFixedLength = 'true'"!][!//
    [!VAR "MacDidDataDescriptor" = "bit:or($MacDidDataDescriptor,$MacFixedLengthMask)"!][!//
  [!ENDIF!][!//
  [!//
  [!IF "$GenerateFunctions = 'FALSE'"!][!//
   [!INDENT "4"!][!//
   /* Data descriptor */
   [!"num:inttohex($MacDidDataDescriptor)"!]U,
   [!ENDINDENT!][!//
   [!WS "2"!]},
  [!ENDIF!][!//
[!ENDMACRO!][!//
[!//
