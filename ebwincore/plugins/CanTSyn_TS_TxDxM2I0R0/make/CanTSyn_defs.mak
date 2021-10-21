#
# \file
#
# \brief AUTOSAR CanTSyn
#
# This file contains the implementation of the AUTOSAR
# module CanTSyn.
#
# \version 2.0.8
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

CanTSyn_CORE_PATH           ?= $(PLUGINS_BASE)/CanTSyn_$(CanTSyn_VARIANT)

CanTSyn_OUTPUT_PATH         ?= $(AUTOSAR_BASE_OUTPUT_PATH)

CanTSyn_GEN_FILES      = \
                       $(CanTSyn_OUTPUT_PATH)/include/CanTSyn_Cfg.h \
                       $(CanTSyn_OUTPUT_PATH)/include/CanTSyn_Types.h \
                       $(CanTSyn_OUTPUT_PATH)/src/CanTSyn_Cfg.c \

TRESOS_GEN_FILES     += $(CanTSyn_GEN_FILES)


#################################################################
# REGISTRY


CanTSyn_DEPENDENT_PLUGINS := base_make tresos
CanTSyn_VERSION           := 2.00.00
CanTSyn_DESCRIPTION       := CanTSyn Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH            += \
  $(CanTSyn_CORE_PATH)/include \
  $(CanTSyn_OUTPUT_PATH)/include
ASM_INCLUDE_PATH           +=
