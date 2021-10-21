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
FrSM_CORE_PATH    ?= $(PLUGINS_BASE)/FrSM_$(FrSM_VARIANT)

FrSM_OUTPUT_PATH  ?= $(AUTOSAR_BASE_OUTPUT_PATH)

FrSM_GEN_FILES =                                  \
    $(FrSM_OUTPUT_PATH)/include/FrSM_Cfg.h   \
    $(FrSM_OUTPUT_PATH)/include/FrSM_Lcfg.h \
    $(FrSM_OUTPUT_PATH)/src/FrSM_Lcfg.c

TRESOS_GEN_FILES     += $(FrIf_GEN_FILES)

#################################################################
# REGISTRY

FrSM_DEPENDENT_PLUGINS := base_make tresos
FrSM_VERSION           := 3.00.00
FrSM_DESCRIPTION       := FrSM Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH       +=           \
    $(FrSM_CORE_PATH)/include      \
    $(FrSM_OUTPUT_PATH)/include    \
    $(FrSM_CORE_PATH)/src          \
    $(FrSM_OUTPUT_PATH)/src

ASM_INCLUDE_PATH     +=
CPP_INCLUDE_PATH      +=
