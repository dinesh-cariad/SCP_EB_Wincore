#
# \file
#
# \brief AUTOSAR DoIP
#
# This file contains the implementation of the AUTOSAR
# module DoIP.
#
# \version 1.1.21
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

LIBRARIES_TO_BUILD   += DoIP_src

DoIP_lib_FILES +=                                               \

DoIP_src_FILES +=                                               \
    $(DoIP_CORE_PATH)/src/DoIP.c                                \
    $(DoIP_CORE_PATH)/src/DoIP_CustomPayloadType.c              \
    $(DoIP_OUTPUT_PATH)/src/DoIP_Lcfg.c


# Fill the list with post build configuration files needed to build the post build binary.
DoIP_pbconfig_FILES := $(wildcard $(DoIP_OUTPUT_PATH)/src/DoIP_PBcfg.c)

ifneq ($(strip $(DoIP_pbconfig_FILES)),)
LIBRARIES_PBCFG_TO_BUILD += DoIP_pbconfig
LIBRARIES_TO_BUILD += DoIP_pbconfig
endif

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
