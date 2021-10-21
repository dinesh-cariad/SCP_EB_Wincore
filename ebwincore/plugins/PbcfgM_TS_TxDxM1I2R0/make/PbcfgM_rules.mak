#
# \file
#
# \brief AUTOSAR PbcfgM
#
# This file contains the implementation of the AUTOSAR
# module PbcfgM.
#
# \version 1.2.22
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

PbcfgM_src_FILES += \
    $(PbcfgM_CORE_PATH)/src/PbcfgM.c \

# Add generated files to the list of source files
PbcfgM_src_FILES += $(PbcfgM_OUTPUT_PATH)/src/PbcfgM_Lcfg.c

LIBRARIES_TO_BUILD += PbcfgM_src

# Fill the list with post build configuration files needed to build the post build binary.
PbcfgM_pbconfig_FILES := $(wildcard $(PbcfgM_OUTPUT_PATH)/src/PbcfgM_PBcfg.c)

ifneq ($(strip $(PbcfgM_pbconfig_FILES)),)
LIBRARIES_PBCFG_TO_BUILD += PbcfgM_pbconfig
LIBRARIES_TO_BUILD += PbcfgM_pbconfig
endif

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
