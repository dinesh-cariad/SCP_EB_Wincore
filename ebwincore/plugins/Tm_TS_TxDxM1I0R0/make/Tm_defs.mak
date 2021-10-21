#
# \file
#
# \brief AUTOSAR Tm
#
# This file contains the implementation of the AUTOSAR
# module Tm.
#
# \version 1.0.4
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

Tm_CORE_PATH         ?= $(PLUGINS_BASE)/Tm_$(Tm_VARIANT)

Tm_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

Tm_DEPENDENT_PLUGINS := base_make tresos
Tm_VERSION           := 1.00.00
Tm_DESCRIPTION       := Tm Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       +=    \
   $(Tm_CORE_PATH)/include \
   $(Tm_OUTPUT_PATH)/include
