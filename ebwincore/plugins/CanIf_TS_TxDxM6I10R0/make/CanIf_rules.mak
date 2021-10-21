#
# \file
#
# \brief AUTOSAR CanIf
#
# This file contains the implementation of the AUTOSAR
# module CanIf.
#
# \version 6.10.15
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


##############################################################################
# DEFINITIONS


##############################################################################
# REGISTRY

CanIf_lib_FILES      +=

CanIf_src_FILES      += \
    $(CanIf_CORE_PATH)/src/CanIf.c \
    $(CanIf_OUTPUT_PATH)/src/CanIf_MultiMF.c \
    $(CanIf_OUTPUT_PATH)/src/CanIf_MultiDriver403.c \
    $(CanIf_OUTPUT_PATH)/src/CanIf_Lcfg.c \
    $(CanIf_OUTPUT_PATH)/src/CanIf_Cfg.c


LIBRARIES_TO_BUILD   += CanIf_src

# Fill the list with post build configuration files needed to build the post build binary.
CanIf_pbconfig_FILES := $(wildcard $(CanIf_OUTPUT_PATH)/src/CanIf_PBcfg.c)

ifneq ($(strip $(CanIf_pbconfig_FILES)),)
LIBRARIES_PBCFG_TO_BUILD += CanIf_pbconfig
LIBRARIES_TO_BUILD += CanIf_pbconfig
endif

CC_FILES_TO_BUILD    +=
CPP_FILES_TO_BUILD   +=
ASM_FILES_TO_BUILD   +=


MAKE_CLEAN_RULES     +=
MAKE_GENERATE_RULES  +=
MAKE_COMPILE_RULES   +=
#MAKE_DEBUG_RULES    +=
MAKE_CONFIG_RULES    +=
#MAKE_ADD_RULES      +=

define defineCanIfLibOutputPATH
$(1)_OBJ_OUTPUT_PATH += $(CanIf_lib_LIB_OUTPUT_PATH)
endef

$(foreach SRC,$(basename $(notdir $(subst \,/,$(CanIf_lib_FILES)))),$(eval $(call defineCanIfLibOutputPATH,$(SRC))))

##############################################################################
# DEPENDENCIES (only for assembler files)
#

##############################################################################
# RULES
