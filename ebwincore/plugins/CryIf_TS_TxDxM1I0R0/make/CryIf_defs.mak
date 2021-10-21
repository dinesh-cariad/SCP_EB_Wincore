#
# \file
#
# \brief AUTOSAR CryIf
#
# This file contains the implementation of the AUTOSAR
# module CryIf.
#
# \version 1.0.24
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

ifeq ($(BUILD_MODE),LIB)
ifeq ($(MODULE),CryIf)
LIB_VARIANTS                  += CryIf_BASE
endif
endif

CryIf_CORE_PATH                 ?= $(PLUGINS_BASE)/CryIf_$(CryIf_VARIANT)
CryIf_OUTPUT_PATH               ?= $(AUTOSAR_BASE_OUTPUT_PATH)

CryIf_GEN_FILES                 :=                                    \
  $(CryIf_CORE_PATH)/generate/include/CryIf_Cfg.h \
  $(CryIf_CORE_PATH)/generate/src/CryIf_Cfg.c
  
CryIf_lib_LIB_OUTPUT_PATH       := $(CryIf_CORE_PATH)/lib

#################################################################
# REGISTRY


CryIf_DEPENDENT_PLUGINS         := base_make tresos
CryIf_VERSION                   := 1.00.00
CryIf_DESCRIPTION               := CryIf Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH               += \
  $(CryIf_CORE_PATH)/include     \
  $(CryIf_CORE_PATH)/lib_include \
  $(CryIf_OUTPUT_PATH)/include

ASM_INCLUDE_PATH              +=
CPP_INCLUDE_PATH              +=

