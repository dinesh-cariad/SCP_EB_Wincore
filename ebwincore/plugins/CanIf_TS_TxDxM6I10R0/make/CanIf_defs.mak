#
# \file
#
# \brief AUTOSAR CanIf
#
# This file contains the implementation of the AUTOSAR
# module CanIf.
#
# \version 6.10.15
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

CanIf_CORE_PATH           ?= $(PLUGINS_BASE)/CanIf_$(CanIf_VARIANT)

CanIf_OUTPUT_PATH         ?= $(AUTOSAR_BASE_OUTPUT_PATH)

CanIf_GEN_FILES      = \
                       $(CanIf_OUTPUT_PATH)/include/CanIf_Cfg.h \
                       $(CanIf_OUTPUT_PATH)/include/CanIf_Int_Cfg.h \
                       $(CanIf_OUTPUT_PATH)/include/CanIf_PBcfg.h \
                       $(CanIf_OUTPUT_PATH)/include/CanIf_SymbolicNames_PBcfg.h \
                       $(CanIf_OUTPUT_PATH)/src/CanIf_Cfg.c \
                       $(CanIf_OUTPUT_PATH)/src/CanIf_PBcfg.c \

TRESOS_GEN_FILES     += $(CanIf_GEN_FILES)


#################################################################
# REGISTRY


CanIf_DEPENDENT_PLUGINS := base_make tresos
CanIf_VERSION           := 2.00.00
CanIf_DESCRIPTION       := CanIf Basic Software Makefile PlugIn for Autosar


CC_INCLUDE_PATH      += $(CanIf_CORE_PATH)/include

ASM_INCLUDE_PATH     +=
