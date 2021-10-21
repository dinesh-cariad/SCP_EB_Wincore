#
# \file
#
# \brief AUTOSAR Dcm
#
# This file contains the implementation of the AUTOSAR
# module Dcm.
#
# \version 5.0.9
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

Dcm_CORE_PATH      ?= $(PLUGINS_BASE)/Dcm_$(Dcm_VARIANT)

Dcm_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

Dcm_DEPENDENT_PLUGINS := base_make tresos
Dcm_VERSION           := 3.01.00
Dcm_DESCRIPTION       := Dcm Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       += \
   $(Dcm_CORE_PATH)/include \
   $(Dcm_OUTPUT_PATH)/include

