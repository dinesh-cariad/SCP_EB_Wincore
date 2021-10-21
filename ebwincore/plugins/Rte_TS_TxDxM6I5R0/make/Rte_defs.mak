#
# \file
#
# \brief AUTOSAR Rte
#
# This file contains the implementation of the AUTOSAR
# module Rte.
#
# \version 6.5.3
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

Rte_CORE_PATH   ?= $(PLUGINS_BASE)/Rte_$(Rte_VARIANT)

Rte_OUTPUT_PATH ?= $(AUTOSAR_BASE_OUTPUT_PATH)

Rte_GEN_FILES    = $(wildcard $(Rte_OUTPUT_PATH)/src/Rte*.c) \
                   $(wildcard $(Rte_OUTPUT_PATH)/include/Rte_*.h)

Rte_OPTIONS += $(Rte_SYSD_OPTIONS)


#################################################################
# REGISTRY

Rte_DEPENDENT_PLUGINS = base_make
Rte_VERSION           = 2.00.00
Rte_DESCRIPTION       = Rte Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH            += \
   $(Rte_CORE_PATH)/include \
   $(Rte_OUTPUT_PATH)/include

