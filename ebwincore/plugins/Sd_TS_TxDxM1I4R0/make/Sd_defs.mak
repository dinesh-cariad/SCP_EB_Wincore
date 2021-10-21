#
# \file
#
# \brief AUTOSAR Sd
#
# This file contains the implementation of the AUTOSAR
# module Sd.
#
# \version 1.4.9
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

Sd_CORE_PATH      ?= $(PLUGINS_BASE)/Sd_$(Sd_VARIANT)

Sd_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

Sd_GEN_FILES      := $(Sd_OUTPUT_PATH)/include/Sd_Cfg.h \
                     $(Sd_OUTPUT_PATH)/src/Sd_Cfg.c \

TRESOS_GEN_FILES        += $(Sd_GEN_FILES)

#################################################################
# REGISTRY

Sd_DEPENDENT_PLUGINS      := base_make tresos
Sd_VERSION                := 1.00.00
Sd_DESCRIPTION            := Sd Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH            += $(Sd_CORE_PATH)/include \
                              $(Sd_CORE_PATH)/src \
                              $(Sd_OUTPUT_PATH)/include

