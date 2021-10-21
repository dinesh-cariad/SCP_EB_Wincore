#
# \file
#
# \brief AUTOSAR LinSM
#
# This file contains the implementation of the AUTOSAR
# module LinSM.
#
# \version 3.4.19
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

LIBRARIES_TO_BUILD   += LinSM_src

LinSM_lib_FILES      :=


ifndef TS_MERGED_COMPILE
TS_MERGED_COMPILE := TRUE
endif

ifndef TS_LINSM_MERGED_COMPILE
TS_LINSM_MERGED_COMPILE := $(TS_MERGED_COMPILE)
endif

ifeq ($(TS_LINSM_MERGED_COMPILE),TRUE)

LinSM_src_FILES +=                              \
    $(LinSM_CORE_PATH)/src/LinSM_Merged.c       \

else

LinSM_src_FILES      := \
    $(LinSM_CORE_PATH)/src/LinSM.c \
    $(LinSM_CORE_PATH)/src/LinSM_GetCurrentComMode.c \
    $(LinSM_CORE_PATH)/src/LinSM_GetVersionInfo.c \
    $(LinSM_CORE_PATH)/src/LinSM_GotoSleepConfirmation.c \
    $(LinSM_CORE_PATH)/src/LinSM_GotoSleepIndication.c \
    $(LinSM_CORE_PATH)/src/LinSM_Init.c \
    $(LinSM_CORE_PATH)/src/LinSM_MainFunction.c \
    $(LinSM_CORE_PATH)/src/LinSM_RequestComMode.c \
    $(LinSM_CORE_PATH)/src/LinSM_ScheduleRequest.c \
    $(LinSM_CORE_PATH)/src/LinSM_ScheduleRequestConfirmation.c \
    $(LinSM_CORE_PATH)/src/LinSM_WakeupConfirmation.c \

endif

LinSM_src_FILES +=                              \
    $(LinSM_OUTPUT_PATH)/src/LinSM_Cfg.c        \


define defineLinSMLibOutputPATH
$(1)_OBJ_OUTPUT_PATH := $(LinSM_lib_LIB_OUTPUT_PATH)
endef
$(foreach SRC,$(basename $(notdir $(subst \,/,$(LinSM_lib_FILES)))),$(eval $(call defineLinSMLibOutputPATH,$(SRC))))

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
