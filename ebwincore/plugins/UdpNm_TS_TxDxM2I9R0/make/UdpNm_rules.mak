#
# \file
#
# \brief AUTOSAR UdpNm
#
# This file contains the implementation of the AUTOSAR
# module UdpNm.
#
# \version 2.9.7
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

UdpNm_src_FILES      := \
    $(UdpNm_CORE_PATH)/src/UdpNm.c \
    $(UdpNm_CORE_PATH)/src/UdpNm_Hsm.c \
    $(UdpNm_CORE_PATH)/src/UdpNm_HsmUdpNmData.c \
    $(UdpNm_CORE_PATH)/src/UdpNm_HsmUdpNmFnct.c \
    $(UdpNm_OUTPUT_PATH)/src/UdpNm_Cfg.c \
    $(UdpNm_OUTPUT_PATH)/src/UdpNm_Lcfg.c

LIBRARIES_TO_BUILD   += UdpNm_src

# Fill the list with post build configuration files needed to build the post build binary.
UdpNm_pbconfig_FILES := $(wildcard $(UdpNm_OUTPUT_PATH)/src/UdpNm_PBcfg.c)

# only add UdpNm_pbconfig to the Pbcfg libraries to build if there
# are really post-build config files present
ifneq ($(strip $(UdpNm_pbconfig_FILES)),)
# this is used by asc_Make
LIBRARIES_PBCFG_TO_BUILD += UdpNm_pbconfig
# asc_Make can only separately generate module or Pb libraries, but not both
# in the default case (only generate module libs), we also want to 
# generate the Pb libraries, for proper testing.
LIBRARIES_TO_BUILD += UdpNm_pbconfig
endif

define defineUdpNmLibOutputPATH
$(1)_OBJ_OUTPUT_PATH    := $(UdpNm_lib_LIB_OUTPUT_PATH)
endef
$(foreach SRC,$(basename $(notdir $(subst \,/,$(UdpNm_lib_FILES)))),$(eval $(call defineUdpNmLibOutputPATH,$(SRC))))

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
