#
# \file
#
# \brief AUTOSAR CanNm
#
# This file contains the implementation of the AUTOSAR
# module CanNm.
#
# \version 6.19.7
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

CanNm_src_FILES      := \
    $(CanNm_CORE_PATH)/src/CanNm.c \
    $(CanNm_CORE_PATH)/src/CanNm_Hsm.c \
    $(CanNm_CORE_PATH)/src/CanNm_HsmCanNmData.c \
    $(CanNm_CORE_PATH)/src/CanNm_HsmCanNmFnct.c \
    $(CanNm_OUTPUT_PATH)/src/CanNm_Cfg.c \
    $(CanNm_OUTPUT_PATH)/src/CanNm_Lcfg.c

LIBRARIES_TO_BUILD   += CanNm_src

# Fill the list with post build configuration files needed to build the post build binary.
CanNm_pbconfig_FILES := $(wildcard $(CanNm_OUTPUT_PATH)/src/CanNm_PBcfg.c)

# only add CanNm_pbconfig to the Pbcfg libraries to build if there
# are really post-build config files present
ifneq ($(strip $(CanNm_pbconfig_FILES)),)
# this is used by asc_Make
LIBRARIES_PBCFG_TO_BUILD += CanNm_pbconfig
# asc_Make can only separately generate module or Pb libraries, but not both
# in the default case (only generate module libs), we also want to 
# generate the Pb libraries, for proper testing.
LIBRARIES_TO_BUILD += CanNm_pbconfig
endif

define defineCanNmLibOutputPATH
$(1)_OBJ_OUTPUT_PATH    := $(CanNm_lib_LIB_OUTPUT_PATH)
endef
$(foreach SRC,$(basename $(notdir $(subst \,/,$(CanNm_lib_FILES)))),$(eval $(call defineCanNmLibOutputPATH,$(SRC))))

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
