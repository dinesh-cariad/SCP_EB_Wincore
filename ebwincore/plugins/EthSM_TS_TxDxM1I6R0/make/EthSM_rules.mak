#
# \file
#
# \brief AUTOSAR EthSM
#
# This file contains the implementation of the AUTOSAR
# module EthSM.
#
# \version 1.6.14
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

EthSM_src_FILES      := $(EthSM_CORE_PATH)/src/EthSM.c \
                        $(EthSM_OUTPUT_PATH)/src/EthSM_Lcfg.c \

LIBRARIES_TO_BUILD     += EthSM_src

# Fill the list with post build configuration files needed to build the post build binary.
EthSM_pbconfig_FILES := $(wildcard $(EthSM_OUTPUT_PATH)/src/EthSM_PBcfg.c)

ifneq ($(strip $(EthSM_pbconfig_FILES)),)
LIBRARIES_PBCFG_TO_BUILD += EthSM_pbconfig
LIBRARIES_TO_BUILD += EthSM_pbconfig
endif

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
