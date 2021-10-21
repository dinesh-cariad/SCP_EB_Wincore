#
# \file
#
# \brief AUTOSAR EthIf
#
# This file contains the implementation of the AUTOSAR
# module EthIf.
#
# \version 1.9.18
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

EthIf_src_FILES      := \
    $(EthIf_CORE_PATH)/src/EthIf.c \
    $(EthIf_OUTPUT_PATH)/src/EthIf_Lcfg.c \
    $(EthIf_OUTPUT_PATH)/src/EthIf_Cfg.c \


# Fill the list with post build configuration files needed to build the post build binary.
EthIf_pbconfig_FILES := $(wildcard $(EthIf_OUTPUT_PATH)/src/EthIf_PBcfg.c)

ifneq ($(strip $(EthIf_pbconfig_FILES)),)
LIBRARIES_PBCFG_TO_BUILD += EthIf_pbconfig
LIBRARIES_TO_BUILD += EthIf_pbconfig
endif

LIBRARIES_TO_BUILD     += EthIf_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
