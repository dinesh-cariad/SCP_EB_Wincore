#
# \file
#
# \brief AUTOSAR FrTSyn
#
# This file contains the implementation of the AUTOSAR
# module FrTSyn.
#
# \version 2.0.9
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

FrTSyn_CORE_PATH           ?= $(PLUGINS_BASE)/FrTSyn_$(FrTSyn_VARIANT)

FrTSyn_OUTPUT_PATH         := $(AUTOSAR_BASE_OUTPUT_PATH)

FrTSyn_GEN_FILES      = \
                       $(FrTSyn_OUTPUT_PATH)/include/FrTSyn_Cfg.h \
                       $(FrTSyn_OUTPUT_PATH)/src/FrTSyn_Cfg.c \

TRESOS_GEN_FILES     += $(FrTSyn_GEN_FILES)


#################################################################
# REGISTRY


FrTSyn_DEPENDENT_PLUGINS := base_make tresos
FrTSyn_VERSION           := 2.00.00
FrTSyn_DESCRIPTION       := FrTSyn Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH            += \
  $(FrTSyn_CORE_PATH)/include \
  $(FrTSyn_OUTPUT_PATH)/include
ASM_INCLUDE_PATH           +=
