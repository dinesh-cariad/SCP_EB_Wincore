#
# \file
#
# \brief AUTOSAR Xcp
#
# This file contains the implementation of the AUTOSAR
# module Xcp.
#
# \version 2.12.0
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

Xcp_CORE_PATH             ?= $(PLUGINS_BASE)/Xcp_$(Xcp_VARIANT)

Xcp_OUTPUT_PATH           ?= $(AUTOSAR_BASE_OUTPUT_PATH)
Xcp_Wrapper_OUTPUT_PATH   ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

Xcp_DEPENDENT_PLUGINS := base_make tresos
Xcp_VERSION           := 1.00.00
Xcp_DESCRIPTION       := Xcp Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       +=    \
   $(Xcp_CORE_PATH)/include \
   $(Xcp_OUTPUT_PATH)/include
