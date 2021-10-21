#
# \file
#
# \brief AUTOSAR KeyM
#
# This file contains the implementation of the AUTOSAR
# module KeyM.
#
# \version 1.2.11
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#================================================================
# DEFINITIONS

ifeq ($(BUILD_MODE),LIB)
ifeq ($(MODULE),KeyM)
LIB_VARIANTS                        += KeyM_BASE
endif
endif

KeyM_CORE_PATH                      ?= $(PLUGINS_BASE)/KeyM_$(KeyM_VARIANT)
KeyM_OUTPUT_PATH                    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

KeyM_GEN_FILES                      := \
  $(KeyM_CORE_PATH)/generate/include/KeyM_Types.h \
  $(KeyM_CORE_PATH)/generate/include/KeyM_Cfg.h

KeyM_lib_LIB_OUTPUT_PATH            := $(KeyM_CORE_PATH)/lib

#================================================================
# REGISTRY

SSC_PLUGINS                         += KeyM
KeyM_DEPENDENT_PLUGINS              := base_make tresos
KeyM_VERSION                        := 2.00.00
KeyM_DESCRIPTION                    := KeyM Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH                     += \
  $(KeyM_CORE_PATH)/include     \
  $(KeyM_CORE_PATH)/lib_include \
  $(KeyM_OUTPUT_PATH)/include

ASM_INCLUDE_PATH                    +=
CPP_INCLUDE_PATH                    +=

