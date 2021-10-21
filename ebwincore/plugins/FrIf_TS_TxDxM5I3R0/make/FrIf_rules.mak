#
# \file
#
# \brief AUTOSAR FrIf
#
# This file contains the implementation of the AUTOSAR
# module FrIf.
#
# \version 5.3.25
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

ifndef TS_MERGED_COMPILE
TS_MERGED_COMPILE := TRUE
endif

ifndef TS_FRIF_MERGED_COMPILE
TS_FRIF_MERGED_COMPILE := $(TS_MERGED_COMPILE)
endif

ifeq ($(TS_FRIF_MERGED_COMPILE),TRUE)

FrIf_src_FILES +=                              \
    $(FrIf_CORE_PATH)/src/FrIf_Merged.c        \

else

FrIf_src_FILES +=                                          \
    $(FrIf_CORE_PATH)/src/FrIf_AbortCommunication.c        \
    $(FrIf_CORE_PATH)/src/FrIf_AckAbsoluteTimerIRQ.c       \
    $(FrIf_CORE_PATH)/src/FrIf_AckIRQ.c                    \
    $(FrIf_CORE_PATH)/src/FrIf_AckRelativeTimerIRQ.c       \
    $(FrIf_CORE_PATH)/src/FrIf_AllowColdstart.c            \
    $(FrIf_CORE_PATH)/src/FrIf_AllSlots.c                  \
    $(FrIf_CORE_PATH)/src/FrIf_CancelAbsoluteTimer.c       \
    $(FrIf_CORE_PATH)/src/FrIf_CancelRelativeTimer.c       \
    $(FrIf_CORE_PATH)/src/FrIf_CheckWakeupByTransceiver.c  \
    $(FrIf_CORE_PATH)/src/FrIf_CheckMTS.c                  \
    $(FrIf_CORE_PATH)/src/FrIf_ClearTransceiverWakeup.c    \
    $(FrIf_CORE_PATH)/src/FrIf_ControllerInit.c            \
    $(FrIf_CORE_PATH)/src/FrIf_DisableAbsoluteTimerIRQ.c   \
    $(FrIf_CORE_PATH)/src/FrIf_DisableIRQ.c                \
    $(FrIf_CORE_PATH)/src/FrIf_DisableLPdu.c               \
    $(FrIf_CORE_PATH)/src/FrIf_DisableRelativeTimerIRQ.c   \
    $(FrIf_CORE_PATH)/src/FrIf_DisableTransceiverBranch.c  \
    $(FrIf_CORE_PATH)/src/FrIf_EnableAbsoluteTimerIRQ.c    \
    $(FrIf_CORE_PATH)/src/FrIf_EnableBusMirroring.c        \
    $(FrIf_CORE_PATH)/src/FrIf_EnableIRQ.c                 \
    $(FrIf_CORE_PATH)/src/FrIf_EnableRelativeTimerIRQ.c    \
    $(FrIf_CORE_PATH)/src/FrIf_EnableTransceiverBranch.c   \
    $(FrIf_CORE_PATH)/src/FrIf_GetAbsoluteTimerIRQStatus.c \
    $(FrIf_CORE_PATH)/src/FrIf_GetChannelStatus.c          \
    $(FrIf_CORE_PATH)/src/FrIf_GetClockCorrection.c        \
    $(FrIf_CORE_PATH)/src/FrIf_GetControllerErrorStatus.c  \
    $(FrIf_CORE_PATH)/src/FrIf_GetCycleLength.c            \
    $(FrIf_CORE_PATH)/src/FrIf_GetGlobalTime.c             \
    $(FrIf_CORE_PATH)/src/FrIf_GetIRQStatus.c              \
    $(FrIf_CORE_PATH)/src/FrIf_GetNmVector.c               \
    $(FrIf_CORE_PATH)/src/FrIf_GetNumOfStartupFrames.c     \
    $(FrIf_CORE_PATH)/src/FrIf_GetPOCStatus.c              \
    $(FrIf_CORE_PATH)/src/FrIf_GetRelativeTimerIRQStatus.c \
    $(FrIf_CORE_PATH)/src/FrIf_GetSyncFrameList.c          \
    $(FrIf_CORE_PATH)/src/FrIf_GetSyncState.c              \
    $(FrIf_CORE_PATH)/src/FrIf_GetTransceiverError.c       \
    $(FrIf_CORE_PATH)/src/FrIf_GetTransceiverMode.c        \
    $(FrIf_CORE_PATH)/src/FrIf_GetTransceiverWUReason.c    \
    $(FrIf_CORE_PATH)/src/FrIf_GetVersionInfo.c            \
    $(FrIf_CORE_PATH)/src/FrIf_GetWakeupRxStatus.c         \
    $(FrIf_CORE_PATH)/src/FrIf_HaltCommunication.c         \
    $(FrIf_CORE_PATH)/src/FrIf_Init.c                      \
    $(FrIf_CORE_PATH)/src/FrIf_JobListExecution.c          \
    $(FrIf_CORE_PATH)/src/FrIf_ReadCCConfig.c              \
    $(FrIf_CORE_PATH)/src/FrIf_ReconfigLPdu.c              \
    $(FrIf_CORE_PATH)/src/FrIf_SendMTS.c                   \
    $(FrIf_CORE_PATH)/src/FrIf_SendWUP.c                   \
    $(FrIf_CORE_PATH)/src/FrIf_SetExtSync.c                \
    $(FrIf_CORE_PATH)/src/FrIf_SetAbsoluteTimer.c          \
    $(FrIf_CORE_PATH)/src/FrIf_SetRelativeTimer.c          \
    $(FrIf_CORE_PATH)/src/FrIf_SetTransceiverMode.c        \
    $(FrIf_CORE_PATH)/src/FrIf_SetWakeupChannel.c          \
    $(FrIf_CORE_PATH)/src/FrIf_StartCommunication.c        \
    $(FrIf_CORE_PATH)/src/FrIf_StateMachine.c              \
    $(FrIf_CORE_PATH)/src/FrIf_TimeConversion.c            \
    $(FrIf_CORE_PATH)/src/FrIf_Transmit.c                  \
   
endif

FrIf_src_FILES += $(FrIf_OUTPUT_PATH)/src/FrIf_Lcfg.c

LIBRARIES_TO_BUILD += FrIf_src

# Fill the list with post build configuration files needed to build the post build binary.
FrIf_pbconfig_FILES := $(wildcard $(FrIf_OUTPUT_PATH)/src/FrIf_PBcfg.c)

ifneq ($(strip $(FrIf_pbconfig_FILES)),)
LIBRARIES_PBCFG_TO_BUILD += FrIf_pbconfig
LIBRARIES_TO_BUILD += FrIf_pbconfig
endif



define defineFrIfLibOutputPATH
$(1)_OBJ_OUTPUT_PATH    := $(FrIf_lib_LIB_OUTPUT_PATH)
endef
$(foreach SRC,$(basename $(notdir $(subst \,/,$(FrIf_lib_FILES)))),$(eval $(call defineFrIfLibOutputPATH,$(SRC))))

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES




