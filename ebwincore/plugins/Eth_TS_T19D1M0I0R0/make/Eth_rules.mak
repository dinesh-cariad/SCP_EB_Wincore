#
# \file
#
# \brief AUTOSAR Eth
#
# This file contains the implementation of the AUTOSAR
# module Eth.
#
# \version 0.0.8
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2011 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


Eth_src_FILES = $(Eth_CORE_PATH)/src/Eth.c

LIBRARIES_TO_BUILD += Eth_src

#################################################################
# REGISTRY
Eth_src_FILES      += \
    $(Eth_CORE_PATH)/src/Eth_Legacy.c \
    $(Eth_OUTPUT_PATH)/src/Eth_Lcfg.c

# Fill the list with post build configuration files needed to build the post build binary.
Eth_pbconfig_FILES := $(wildcard $(Eth_OUTPUT_PATH)/src/Eth_PBcfg.c)

# only add Eth_pbconfig to the libraries to build if there are really post-build config files
# present
ifneq ($(strip $(Eth_pbconfig_FILES)),)
LIBRARIES_PBCFG_TO_BUILD += Eth_pbconfig
LIBRARIES_TO_BUILD += Eth_pbconfig
endif
# WINDOWS WIN32X86 specific part
Eth_src_FILES += $(Eth_CORE_PATH)/src/Eth_WinPcap.c
