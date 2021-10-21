#
# \file
#
# \brief AUTOSAR Dcm
#
# This file contains the implementation of the AUTOSAR
# module Dcm.
#
# \version 5.0.9
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

#################################################################
# REGISTRY

## Dcm core files ###############################################
#  - files which are always compiled in
Dcm_src_FILES := \
  $(Dcm_CORE_PATH)/src/Dcm_Dsp_Contexts.c \
  $(Dcm_OUTPUT_PATH)/src/Dcm_Cfg.c \
  $(Dcm_OUTPUT_PATH)/src/Dcm_Dsl_Cfg.c

## Dcm SwUnit files ###############################################
#
##     Core modules #
Dcm_src_FILES_Core := \
  $(Dcm_CORE_PATH)/src/Dcm.c \
  $(Dcm_CORE_PATH)/src/Dcm_Dsd.c \
  $(Dcm_CORE_PATH)/src/Dcm_Dsp.c

Dcm_src_FILES_Supervisor := \
  $(Dcm_CORE_PATH)/src/Dcm_Dsl_Supervisor.c \
  $(Dcm_CORE_PATH)/src/Dcm_Dsl_Supervisor_CommunicationSessionHandling.c \
  $(Dcm_CORE_PATH)/src/Dcm_Dsl_Supervisor_MainConnectionHandling.c \
  $(Dcm_CORE_PATH)/src/Dcm_Dsl_Supervisor_ProtocolHandling.c \

Dcm_src_FILES_DiagnosticSessionHandler:= \
  $(Dcm_CORE_PATH)/src/Dcm_Dsl_DiagnosticSessionHandler.c \

Dcm_src_FILES_PostBootOperationsHandler:= \
  $(Dcm_CORE_PATH)/src/Dcm_Dsl_PostBootOperationsHandler.c \

Dcm_src_FILES_ComMChannelStateManager:= \
  $(Dcm_CORE_PATH)/src/Dcm_Dsl_ComMChannelStateManager.c \

#
##     Diagnostic Service handlers #
Dcm_src_FILES_RoutineControl := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_RoutineControl.c\
                                $(Dcm_OUTPUT_PATH)/src/Dcm_RoutineControlConst_Cfg.c\
                                $(Dcm_OUTPUT_PATH)/src/Dcm_RoutineControlOperations_Cfg.c
Dcm_src_FILES_CommunicationControl := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_CommunicationControl.c \
                                      $(Dcm_OUTPUT_PATH)/src/Dcm_CommunicationControl_Cfg.c
Dcm_src_FILES_ControlDTCSetting := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_ControlDTCSetting.c
Dcm_src_FILES_InputOutputControlByIdentifier := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_InputOutputControlByIdentifier.c
Dcm_src_FILES_DynamicallyDefineDataIdentifier := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_DynamicallyDefineDataIdentifier.c
Dcm_src_FILES_LinkControl := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_LinkControl.c
Dcm_src_FILES_ReadDataByIdentifier := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_ReadDataByIdentifier.c
Dcm_src_FILES_ReadScalingDataByIdentifier := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_ReadScalingDataByIdentifier.c
Dcm_src_FILES_ReadDataByPeriodicIdentifier := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_ReadDataByPeriodicIdentifier.c
Dcm_src_FILES_ReadDTCInformation := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_ReadDTCInformation.c
Dcm_src_FILES_ReadMemoryByAddress := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_ReadMemoryByAddress.c
Dcm_src_FILES_RequestControlOfOnBoardSysTstComp := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_RequestControlOfOnBoardSysTstComp.c
Dcm_src_FILES_RequestDownload := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_RequestDownload.c
Dcm_src_FILES_RequestOnboardMonitoringTestResults := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_RequestOnboardMonitoringTestResults.c
Dcm_src_FILES_RequestTransferExit := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_RequestTransferExit.c
Dcm_src_FILES_RequestUpload := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_RequestUpload.c
Dcm_src_FILES_RequestVehicleInformation := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_RequestVehicleInformation.c \
                                           $(Dcm_OUTPUT_PATH)/src/Dcm_RequestVehicleInformationConst_Cfg.c
Dcm_src_FILES_ResponseOnEvent := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_ResponseOnEvent.c
Dcm_src_FILES_TransferData := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_TransferData.c
Dcm_src_FILES_WriteDataByIdentifier := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_WriteDataByIdentifier.c
Dcm_src_FILES_WriteMemoryByAddress := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_WriteMemoryByAddress.c
Dcm_src_FILES_SecurityAccess := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_SecurityAccess.c \
                                $(Dcm_OUTPUT_PATH)/src/Dcm_SecurityAccess_Cfg.c
Dcm_src_FILES_TesterPresent := $(Dcm_CORE_PATH)/src/Dcm_Dsp_SvcH_TesterPresent.c
#
##     Service libraries #
Dcm_src_FILES_DidSignals  := $(Dcm_CORE_PATH)/src/Dcm_Dsp_DidSignals.c
Dcm_src_FILES_DidServices := $(Dcm_CORE_PATH)/src/Dcm_Dsp_DidServices.c
Dcm_src_FILES_PidServices := $(Dcm_CORE_PATH)/src/Dcm_Dsp_PidServices.c
Dcm_src_FILES_MidServices := $(Dcm_CORE_PATH)/src/Dcm_Dsp_MidServices.c
Dcm_src_FILES_VinServices := $(Dcm_CORE_PATH)/src/Dcm_Dsp_VinServices.c
Dcm_src_FILES_RoeServices := $(Dcm_CORE_PATH)/src/Dcm_Dsl_RoeServices.c
Dcm_src_FILES_DataTransferServices := $(Dcm_CORE_PATH)/src/Dcm_Dsp_DataTransferServices.c
Dcm_src_FILES_MemoryServices := $(Dcm_CORE_PATH)/src/Dcm_Dsp_MemoryServices.c
Dcm_src_FILES_CommunicationServices := $(Dcm_CORE_PATH)/src/Dcm_Dsl_CommunicationServices.c \
                                       $(Dcm_CORE_PATH)/src/Dcm_Dsl_CommunicationServices_BufferManager.c \
                                       $(Dcm_CORE_PATH)/src/Dcm_Dsl_CommunicationServices_MetaData.c \
                                       $(Dcm_CORE_PATH)/src/Dcm_Dsl_CommunicationServices_RxConnections.c \
                                       $(Dcm_CORE_PATH)/src/Dcm_Dsl_CommunicationServices_TxConnections.c
Dcm_src_FILES_DemFacade := $(Dcm_CORE_PATH)/src/Dcm_Dsp_DemFacade.c
Dcm_src_FILES_SecurityLevelHandler := $(Dcm_CORE_PATH)/src/Dcm_Dsl_SecurityLevelHandler.c
Dcm_src_FILES_DiagnosticSessionHandler := $(Dcm_CORE_PATH)/src/Dcm_Dsl_DiagnosticSessionHandler.c
Dcm_src_FILES_EndiannessInterpretation := $(Dcm_CORE_PATH)/src/Dcm_Dsp_EndiannessInterpretation.c
Dcm_src_FILES_ApplicationDiagnosticRequests := $(Dcm_CORE_PATH)/src/Dcm_Dsp_ApplicationDiagnosticRequests.c
#
##     HSM #
Dcm_src_FILES_Hsm := \
  $(Dcm_CORE_PATH)/src/Dcm_Hsm.c \
  $(Dcm_CORE_PATH)/src/Dcm_HsmDcmFnct.c \
  $(Dcm_CORE_PATH)/src/Dcm_HsmDcmData.c \

Dcm_Units ?= \
  Core \
  Supervisor \
  DiagnosticSessionHandler \
  PostBootOperationsHandler \
  ComMChannelStateManager \
  Hsm \
  CommunicationControl \
  ControlDTCSetting \
  InputOutputControlByIdentifier \
  DidServices \
  DidSignals \
  MemoryServices \
  DynamicallyDefineDataIdentifier \
  LinkControl \
  ReadDataByIdentifier \
  ReadScalingDataByIdentifier\
  ReadDataByPeriodicIdentifier \
  ReadDTCInformation \
  ReadMemoryByAddress \
  RequestControlOfOnBoardSysTstComp \
  RequestDownload \
  RequestOnboardMonitoringTestResults \
  RequestTransferExit \
  RequestUpload \
  RequestVehicleInformation \
  ResponseOnEvent \
  RoutineControl \
  TransferData \
  WriteDataByIdentifier \
  WriteMemoryByAddress \
  PidServices \
  MidServices \
  VinServices \
  RoeServices \
  DataTransferServices \
  CommunicationServices \
  DemFacade \
  SecurityAccess \
  SecurityLevelHandler \
  DiagnosticSessionHandler \
  TesterPresent \
  ApplicationDiagnosticRequests \
  EndiannessInterpretation \

Dcm_src_FILES += $(foreach unit,$(Dcm_Units),$(Dcm_src_FILES_$(unit)))

LIBRARIES_TO_BUILD     += Dcm_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
