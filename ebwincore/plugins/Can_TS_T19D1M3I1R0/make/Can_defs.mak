#
# \file
#
# \brief AUTOSAR Can
#
# This file contains the implementation of the AUTOSAR
# module Can.
#
# \version 3.1.4
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

Can_CORE_PATH      ?= $(PLUGINS_BASE)/Can_$(Can_VARIANT)

Can_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

Can_GEN_FILES      := \
        $(Can_OUTPUT_PATH)/include/Can_Cfg.h \
        $(Can_OUTPUT_PATH)/src/Can_Cfg.c

TRESOS_GEN_FILES        += $(Can_GEN_FILES)

#################################################################
# REGISTRY

Can_DEPENDENT_PLUGINS := base_make tresos
Can_VERSION           := 1.0.0
Can_DESCRIPTION       := Can Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH            += \
   $(Can_CORE_PATH)/include \
   $(Can_CORE_PATH)/include/$(CPU)/$(DERIVATE) \
   $(Can_OUTPUT_PATH)/include
ASM_INCLUDE_PATH           +=

