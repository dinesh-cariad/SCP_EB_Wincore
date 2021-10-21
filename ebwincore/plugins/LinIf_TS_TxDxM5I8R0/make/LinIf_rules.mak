#
# \file
#
# \brief AUTOSAR LinIf
#
# This file contains the implementation of the AUTOSAR
# module LinIf.
#
# \version 5.8.27
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

ifndef TS_LINIF_MERGED_COMPILE
TS_LINIF_MERGED_COMPILE := $(TS_MERGED_COMPILE)
endif

ifeq ($(TS_LINIF_MERGED_COMPILE),TRUE)

LinIf_src_FILES +=                              \
    $(LinIf_CORE_PATH)/src/LinIf_Merged.c       \

else

LinIf_src_FILES      = \
                      $(LinIf_CORE_PATH)/src/LinIf_GotoSleep.c \
                      $(LinIf_CORE_PATH)/src/LinIf_Transmit.c \
                      $(LinIf_CORE_PATH)/src/LinIf_GetVersionInfo.c \
                      $(LinIf_CORE_PATH)/src/LinIf.c \
                      $(LinIf_CORE_PATH)/src/LinIf_MainFunction.c \
                      $(LinIf_CORE_PATH)/src/LinIf_Init.c \
                      $(LinIf_CORE_PATH)/src/LinIf_ScheduleRequest.c \
                      $(LinIf_CORE_PATH)/src/LinIf_Wakeup.c \
                      $(LinIf_CORE_PATH)/src/LinIf_CheckWakeup.c \
                      $(LinIf_CORE_PATH)/src/LinIf_Trcv.c \
                      $(LinIf_CORE_PATH)/src/LinIf_PIDTable.c \
                      $(LinIf_CORE_PATH)/src/LinIf_ConfiguredNAD.c \
                      $(LinIf_CORE_PATH)/src/LinIf_HeaderIndication.c \
                      $(LinIf_CORE_PATH)/src/LinIf_RxIndication.c \
                      $(LinIf_CORE_PATH)/src/LinIf_TxConfirmation.c \
                      $(LinIf_CORE_PATH)/src/LinIf_LinErrorIndication.c \
                      $(LinIf_CORE_PATH)/src/LinTp_Init.c \
                      $(LinIf_CORE_PATH)/src/LinTp_MainFunction.c \
                      $(LinIf_CORE_PATH)/src/LinTp_Transmit.c \
                      $(LinIf_CORE_PATH)/src/LinTp_ChangeParameter.c \
                      $(LinIf_CORE_PATH)/src/LinTp_CancelTransmit.c \
                      $(LinIf_CORE_PATH)/src/LinTp_CancelReceive.c \
                      $(LinIf_CORE_PATH)/src/LinTp_GetVersionInfo.c \

endif

LinIf_src_FILES      += \
                       $(LinIf_OUTPUT_PATH)/src/LinIf_Cfg.c \
                       $(LinIf_OUTPUT_PATH)/src/LinIf_Lcfg.c \
                       $(LinIf_OUTPUT_PATH)/src/LinTp_Lcfg.c \

LIBRARIES_TO_BUILD  += LinIf_src

# Fill the list with post build configuration files needed to build the post build binary.
LinIf_pbconfig_FILES := $(wildcard $(LinIf_OUTPUT_PATH)/src/LinIf_PBcfg.c) $(wildcard $(LinIf_OUTPUT_PATH)/src/LinTp_PBcfg.c)

ifneq ($(strip $(LinIf_pbconfig_FILES)),)
LIBRARIES_PBCFG_TO_BUILD += LinIf_pbconfig
LIBRARIES_TO_BUILD += LinIf_pbconfig
endif

define defineLinIfLibOutputPATH
$(1)_OBJ_OUTPUT_PATH += $(LinIf_lib_LIB_OUTPUT_PATH)
endef

$(foreach SRC,$(basename $(notdir $(subst \,/,$(LinIf_lib_FILES)))),$(eval $(call defineLinIfLibOutputPATH,$(SRC))))

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
