#
# \file
#
# \brief AUTOSAR CanSM
#
# This file contains the implementation of the AUTOSAR
# module CanSM.
#
# \version 3.7.5
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

CanSM_CORE_PATH      ?= $(PLUGINS_BASE)/CanSM_$(CanSM_VARIANT)

CanSM_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

CanSM_GEN_FILES      := $(CanSM_OUTPUT_PATH)/include/CanSM_Cfg.h \
                        $(CanSM_OUTPUT_PATH)/include/CanSM_PBcfg.h \
                        $(CanSM_OUTPUT_PATH)/include/CanSM_SymbolicNames_PBcfg.h \
                        $(CanSM_OUTPUT_PATH)/src/CanSM_Cfg.c \
                        $(CanSM_OUTPUT_PATH)/src/CanSM_PBcfg.c \

TRESOS_GEN_FILES        += $(CanSM_GEN_FILES)

#################################################################
# REGISTRY

CanSM_DEPENDENT_PLUGINS := base_make tresos
CanSM_VERSION           := 1.0.0
CanSM_DESCRIPTION       := CanSM Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH            += $(CanSM_CORE_PATH)/include \
                              $(CanSM_CORE_PATH)/src \
                              $(CanSM_OUTPUT_PATH)/include \

ASM_INCLUDE_PATH           +=

