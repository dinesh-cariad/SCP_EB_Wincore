#
# \file
#
# \brief AUTOSAR FrIf
#
# This file contains the implementation of the AUTOSAR
# module FrIf.
#
# \version 5.3.25
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS
FrIf_CORE_PATH    ?= $(PLUGINS_BASE)/FrIf_$(FrIf_VARIANT)

FrIf_OUTPUT_PATH  ?= $(AUTOSAR_BASE_OUTPUT_PATH)

FrIf_GEN_FILES =                                  \
    $(FrIf_OUTPUT_PATH)/include/FrIf_Cfg.h   \
    $(FrIf_OUTPUT_PATH)/include/FrIf_PBcfg.h \
    $(FrIf_OUTPUT_PATH)/include/FrIf_Lcfg.h \
    $(FrIf_OUTPUT_PATH)/src/FrIf_PBcfg.c     \
    $(FrIf_OUTPUT_PATH)/src/FrIf_Lcfg.c     \

TRESOS_GEN_FILES     += $(FrIf_GEN_FILES)

#################################################################
# REGISTRY

FrIf_DEPENDENT_PLUGINS := base_make tresos
FrIf_VERSION           := 3.00.00
FrIf_DESCRIPTION       := FrIf Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH       +=           \
    $(FrIf_CORE_PATH)/include      \
    $(FrIf_CORE_PATH)/src          \

ASM_INCLUDE_PATH     +=
CPP_INCLUDE_PATH      +=
