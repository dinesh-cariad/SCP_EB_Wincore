#
# \file
#
# \brief AUTOSAR Tls
#
# This file contains the implementation of the AUTOSAR
# module Tls.
#
# \version 1.0.8
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

Tls_CORE_PATH         := $(PLUGINS_BASE)/Tls_$(Tls_VARIANT)

Tls_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

Tls_GEN_FILES      = \
                     $(Tls_OUTPUT_PATH)/include/Tls_Cfg.h \
                     $(Tls_OUTPUT_PATH)/src/Tls_Cfg.c \
                     $(Tls_OUTPUT_PATH)/include/Tls_Lcfg.h \
                     $(Tls_OUTPUT_PATH)/src/Tls_Lcfg.c \

TRESOS_GEN_FILES     += $(Tls_GEN_FILES)

#################################################################
# REGISTRY

Tls_DEPENDENT_PLUGINS := base_make tresos
Tls_VERSION           := 1.00.00
Tls_DESCRIPTION       := Tls Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       +=    \
   $(Tls_CORE_PATH)/include \
   $(Tls_CORE_PATH)/src \
   $(Tls_OUTPUT_PATH)/include
