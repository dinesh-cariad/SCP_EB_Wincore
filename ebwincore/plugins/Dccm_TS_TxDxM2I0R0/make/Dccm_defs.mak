#
# \file
#
# \brief AUTOSAR Dccm
#
# This file contains the implementation of the AUTOSAR
# module Dccm.
#
# \version 2.0.6
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.



#################################################################
# DEFINITIONS

ifeq ($(BUILD_MODE),LIB)
ifeq ($(MODULE),Dccm)
LIB_VARIANTS += Dccm_BASE
endif
endif

Dccm_CORE_PATH           := $(PLUGINS_BASE)\Dccm_$(Dccm_VARIANT)

Dccm_OUTPUT_PATH         := $(AUTOSAR_BASE_OUTPUT_PATH)

Dccm_lib_LIB_OUTPUT_PATH := $(Dccm_CORE_PATH)\lib

#################################################################
# REGISTRY
SSC_PLUGINS           += Dccm
Dccm_DEPENDENT_PLUGINS := base_make tresos
Dccm_VERSION           := 2.00.00
Dccm_DESCRIPTION       := Dccm Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH       += \
   $(Dccm_CORE_PATH)\include \
   $(Dccm_OUTPUT_PATH)\include

ASM_INCLUDE_PATH      +=
CPP_INCLUDE_PATH      +=
