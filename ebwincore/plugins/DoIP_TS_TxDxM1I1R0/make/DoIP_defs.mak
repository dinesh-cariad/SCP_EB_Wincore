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
DoIP_CORE_PATH    ?= $(PLUGINS_BASE)/DoIP_$(DoIP_VARIANT)

DoIP_OUTPUT_PATH  ?= $(AUTOSAR_BASE_OUTPUT_PATH)

DoIP_GEN_FILES =                                  \
    $(DoIP_OUTPUT_PATH)/include/DoIP_Cfg.h

TRESOS_GEN_FILES     += $(DoIP_GEN_FILES)

#################################################################
# REGISTRY

DoIP_DEPENDENT_PLUGINS := base_make tresos
DoIP_VERSION           := 1.00.00
DoIP_DESCRIPTION       := DoIP Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH       +=             \
    $(DoIP_CORE_PATH)/include        \
    $(DoIP_OUTPUT_PATH)/include      \
    $(DoIP_CORE_PATH)/src

ASM_INCLUDE_PATH     +=
CPP_INCLUDE_PATH     +=
