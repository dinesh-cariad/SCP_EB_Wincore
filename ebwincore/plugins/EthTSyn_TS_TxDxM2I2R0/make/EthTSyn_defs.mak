#
# \file
#
# \brief AUTOSAR EthTSyn
#
# This file contains the implementation of the AUTOSAR
# module EthTSyn.
#
# \version 2.2.4
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

EthTSyn_CORE_PATH      ?= $(PLUGINS_BASE)/EthTSyn_$(EthTSyn_VARIANT)

EthTSyn_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

EthTSyn_GEN_FILES      := \
        $(EthTSyn_OUTPUT_PATH)/include/EthTSyn_Cfg.h \
        $(EthTSyn_OUTPUT_PATH)/src/EthTSyn_Cfg.c \

TRESOS_GEN_FILES        += $(EthTSyn_GEN_FILES)

#################################################################
# REGISTRY

EthTSyn_DEPENDENT_PLUGINS := base_make tresos
EthTSyn_VERSION           := 2.00.00
EthTSyn_DESCRIPTION       := EthTSyn Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH            += \
  $(EthTSyn_CORE_PATH)/include \
  $(EthTSyn_CORE_PATH)/src \
  $(EthTSyn_OUTPUT_PATH)/include \
  $(EthTSyn_OUTPUT_PATH)/src

ASM_INCLUDE_PATH           +=
