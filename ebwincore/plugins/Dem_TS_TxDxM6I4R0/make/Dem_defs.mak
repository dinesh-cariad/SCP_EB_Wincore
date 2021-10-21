#
# \file
#
# \brief AUTOSAR Dem
#
# This file contains the implementation of the AUTOSAR
# module Dem.
#
# \version 6.4.1
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

Dem_CORE_PATH      ?= $(PLUGINS_BASE)/Dem_$(Dem_VARIANT)

Dem_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY
Dem_DEPENDENT_PLUGINS := base_make tresos2
Dem_VERSION           := 2.00.00
Dem_DESCRIPTION       := Dem Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       += \
   $(Dem_CORE_PATH)/include \
   $(Dem_OUTPUT_PATH)/include
