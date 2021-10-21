#
# \file
#
# \brief AUTOSAR CanSM
#
# This file contains the implementation of the AUTOSAR
# module CanSM.
#
# \version 3.7.5
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

LIBRARIES_TO_BUILD   += CanSM_src


ifndef TS_MERGED_COMPILE
TS_MERGED_COMPILE := TRUE
endif

ifndef TS_CANSM_MERGED_COMPILE
TS_CANSM_MERGED_COMPILE := $(TS_MERGED_COMPILE)
endif

ifeq ($(TS_CANSM_MERGED_COMPILE),TRUE)

CanSM_src_FILES +=                              \
    $(CanSM_CORE_PATH)/src/CanSM_Merged.c       \

else

CanSM_src_FILES      := \
    $(CanSM_CORE_PATH)/src/CanSM.c \
    $(CanSM_CORE_PATH)/src/CanSM_Cbk.c \
    $(CanSM_CORE_PATH)/src/CanSM_GetCurrentComMode.c \
    $(CanSM_CORE_PATH)/src/CanSM_Init.c \
    $(CanSM_CORE_PATH)/src/CanSM_MainFunction.c \
    $(CanSM_CORE_PATH)/src/CanSM_RequestComMode.c \
    $(CanSM_CORE_PATH)/src/CanSM_StateMachine.c \
    $(CanSM_CORE_PATH)/src/CanSM_GetVersionInfo.c \
    $(CanSM_CORE_PATH)/src/CanSM_SetBaudrate.c \

endif

# Fill the list with post build configuration files needed to build the post build binary.
CanSM_pbconfig_FILES := $(wildcard $(CanSM_OUTPUT_PATH)/src/CanSM_PBcfg.c)

ifneq ($(strip $(CanSM_pbconfig_FILES)),)
LIBRARIES_PBCFG_TO_BUILD += CanSM_pbconfig
LIBRARIES_TO_BUILD += CanSM_pbconfig
endif

CanSM_src_FILES +=                              \
    $(CanSM_OUTPUT_PATH)/src/CanSM_Cfg.c        \


define defineCanSMLibOutputPATH
$(1)_OBJ_OUTPUT_PATH := $(CanSM_lib_LIB_OUTPUT_PATH)
endef
$(foreach SRC,$(basename $(notdir $(subst \,/,$(CanSM_lib_FILES)))),$(eval $(call defineCanSMLibOutputPATH,$(SRC))))

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
