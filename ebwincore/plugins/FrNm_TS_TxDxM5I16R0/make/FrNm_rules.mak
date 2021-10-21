#
# \file
#
# \brief AUTOSAR FrNm
#
# This file contains the implementation of the AUTOSAR
# module FrNm.
#
# \version 5.16.7
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

FrNm_src_FILES      := \
    $(FrNm_CORE_PATH)/src/FrNm.c \
    $(FrNm_CORE_PATH)/src/FrNm_Hsm.c \
    $(FrNm_CORE_PATH)/src/FrNm_HsmFrNmData.c \
    $(FrNm_CORE_PATH)/src/FrNm_HsmFrNmFnct.c \
    $(FrNm_OUTPUT_PATH)/src/FrNm_Cfg.c \
    $(FrNm_OUTPUT_PATH)/src/FrNm_Lcfg.c

LIBRARIES_TO_BUILD   += FrNm_src

# Fill the list with post build configuration files needed to build the post build binary.
FrNm_pbconfig_FILES := $(wildcard $(FrNm_OUTPUT_PATH)/src/FrNm_PBcfg.c)

# only add FrNm_pbconfig to the Pbcfg libraries to build if there
# are really post-build config files present
ifneq ($(strip $(FrNm_pbconfig_FILES)),)
# this is used by asc_Make
LIBRARIES_PBCFG_TO_BUILD += FrNm_pbconfig
# asc_Make can only separately generate module or Pb libraries, but not both
# in the default case (only generate module libs), we also want to 
# generate the Pb libraries, for proper testing.
LIBRARIES_TO_BUILD += FrNm_pbconfig
endif

define defineFrNmLibOutputPATH
$(1)_OBJ_OUTPUT_PATH    := $(FrNm_lib_LIB_OUTPUT_PATH)
endef
$(foreach SRC,$(basename $(notdir $(subst \,/,$(FrNm_lib_FILES)))),$(eval $(call defineFrNmLibOutputPATH,$(SRC))))

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
