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

EthIf_CORE_PATH      ?= $(PLUGINS_BASE)/EthIf_$(EthIf_VARIANT)

EthIf_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

EthIf_GEN_FILES      := \
                        $(EthIf_OUTPUT_PATH)/include/EthIf_Cfg.h \
                        $(EthIf_OUTPUT_PATH)/include/EthIf_PBcfg.h \
                        $(EthIf_OUTPUT_PATH)/include/EthIf_Lcfg.h \
                        $(EthIf_OUTPUT_PATH)/include/EthIf_SymbolicNames_PBcfg.h \
                        $(EthIf_OUTPUT_PATH)/src/EthIf_PBcfg.c \
                        $(EthIf_OUTPUT_PATH)/src/EthIf_Lcfg.c \
                        $(EthIf_OUTPUT_PATH)/src/EthIf_Cfg.c \

TRESOS_GEN_FILES        += $(EthIf_GEN_FILES)

EthIf_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

EthIf_DEPENDENT_PLUGINS      := base_make tresos
EthIf_VERSION                := 1.00.00
EthIf_DESCRIPTION            := EthIf Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH            += $(EthIf_CORE_PATH)/include \
                              $(EthIf_CORE_PATH)/src \
                              $(EthIf_OUTPUT_PATH)/include

