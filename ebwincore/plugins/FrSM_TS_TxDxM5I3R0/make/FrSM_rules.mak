#
# \file
#
# \brief AUTOSAR FrSM
#
# This file contains the implementation of the AUTOSAR
# module FrSM.
#
# \version 5.3.18
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

LIBRARIES_TO_BUILD   += FrSM_src

FrSM_lib_FILES +=                                                \

FrSM_src_FILES +=                               \
    $(FrSM_CORE_PATH)/src/FrSM.c                \
    $(FrSM_OUTPUT_PATH)/src/FrSM_Lcfg.c         \


define defineFrSMLibOutputPATH
$(1)_OBJ_OUTPUT_PATH    := $(FrSM_lib_LIB_OUTPUT_PATH)
endef
$(foreach SRC,$(basename $(notdir $(subst \,/,$(FrSM_lib_FILES)))),$(eval $(call defineFrSMLibOutputPATH,$(SRC))))

# Fill the list with post build configuration files needed to build the post build binary.
FrSM_pbconfig_FILES := $(wildcard $(FrSM_OUTPUT_PATH)/src/FrSM*_PBcfg.c)

ifneq ($(strip $(FrSM_pbconfig_FILES)),)
LIBRARIES_PBCFG_TO_BUILD += FrSM_pbconfig
LIBRARIES_TO_BUILD += FrSM_pbconfig
endif

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES




