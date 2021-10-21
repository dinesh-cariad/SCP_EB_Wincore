#
# \file
#
# \brief AUTOSAR LinSM
#
# This file contains the implementation of the AUTOSAR
# module LinSM.
#
# \version 3.4.19
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

LinSM_CORE_PATH      ?= $(PLUGINS_BASE)/LinSM_$(LinSM_VARIANT)

LinSM_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

LinSM_GEN_FILES      := \
        $(LinSM_OUTPUT_PATH)/include/LinSM_Cfg.h \
        $(LinSM_OUTPUT_PATH)/include/LinSM_InternalCfg.h \
        $(LinSM_OUTPUT_PATH)/src/LinSM_Cfg.c

TRESOS_GEN_FILES        += $(LinSM_GEN_FILES)


#################################################################
# REGISTRY

LinSM_DEPENDENT_PLUGINS := base_make tresos
LinSM_VERSION           := 1.0.0
LinSM_DESCRIPTION       := LinSM Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH            += \
   $(LinSM_CORE_PATH)/include \
   $(LinSM_CORE_PATH)/src \
   $(LinSM_OUTPUT_PATH)/include \
   $(LinSM_OUTPUT_PATH)/src \

ASM_INCLUDE_PATH           +=

