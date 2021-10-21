#
# \file
#
# \brief AUTOSAR Csm
#
# This file contains the implementation of the AUTOSAR
# module Csm.
#
# \version 3.1.13
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#================================================================
# DEFINITIONS

ifeq ($(BUILD_MODE),LIB)
ifeq ($(MODULE),Csm)
LIB_VARIANTS                        += Csm_BASE
endif
endif

Csm_CORE_PATH                       ?= $(PLUGINS_BASE)/Csm_$(Csm_VARIANT)
Csm_OUTPUT_PATH                     ?= $(AUTOSAR_BASE_OUTPUT_PATH)

Csm_GEN_FILES                       := \
  $(Csm_CORE_PATH)/generate/include/Csm_Types.h   \
  $(Csm_CORE_PATH)/generate/include/Csm_Cfg.h     \
  $(Csm_CORE_PATH)/generate/include/Csm_Int_Cfg.h

Csm_lib_LIB_OUTPUT_PATH             := $(Csm_CORE_PATH)/lib

#================================================================
# REGISTRY

Csm_DEPENDENT_PLUGINS               := base_make tresos
Csm_VERSION                         := 2.00.00
Csm_DESCRIPTION                     := Csm Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH                     += \
  $(Csm_CORE_PATH)/include     \
  $(Csm_CORE_PATH)/lib_include \
  $(Csm_OUTPUT_PATH)/include

ASM_INCLUDE_PATH                    +=
CPP_INCLUDE_PATH                    +=

