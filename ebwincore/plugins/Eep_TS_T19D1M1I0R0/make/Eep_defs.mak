#
# \file
#
# \brief AUTOSAR Eep
#
# This file contains the implementation of the AUTOSAR
# module Eep.
#
# \version 1.0.9
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

Eep_CORE_PATH         ?= $(PLUGINS_BASE)/Eep_$(Eep_VARIANT)

Eep_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

Eep_GEN_FILES      := \
        $(Eep_OUTPUT_PATH)/include/Eep_Cfg.h \
        $(Eep_OUTPUT_PATH)/src/Eep_PBcfg.c

TRESOS_GEN_FILES        += $(Eep_GEN_FILES)

#################################################################
# REGISTRY

CC_INCLUDE_PATH       += \
   $(Eep_CORE_PATH)/include \
   $(Eep_OUTPUT_PATH)/include
