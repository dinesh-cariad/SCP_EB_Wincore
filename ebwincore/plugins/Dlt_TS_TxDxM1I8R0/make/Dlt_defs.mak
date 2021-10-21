#
# \file
#
# \brief AUTOSAR Dlt
#
# This file contains the implementation of the AUTOSAR
# module Dlt.
#
# \version 1.8.6
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

Dlt_CORE_PATH      ?= $(PLUGINS_BASE)/Dlt_$(Dlt_VARIANT)

Dlt_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

Dlt_DEPENDENT_PLUGINS := base_make tresos2
Dlt_VERSION           := 1.00.00
Dlt_DESCRIPTION       := Dlt Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       += \
   $(Dlt_CORE_PATH)/include \
   $(Dlt_OUTPUT_PATH)/include
