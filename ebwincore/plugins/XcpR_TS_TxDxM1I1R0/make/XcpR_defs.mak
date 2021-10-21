#
# \file
#
# \brief AUTOSAR XcpR
#
# This file contains the implementation of the AUTOSAR
# module XcpR.
#
# \version 1.1.0
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

XcpR_CORE_PATH             ?= $(PLUGINS_BASE)/XcpR_$(XcpR_VARIANT)

XcpR_OUTPUT_PATH           ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

XcpR_DEPENDENT_PLUGINS := base_make tresos
XcpR_VERSION           := 1.00.00
XcpR_DESCRIPTION       := Xcp Router Basic Software Makefile PlugIn
CC_INCLUDE_PATH        +=   \
   $(XcpR_CORE_PATH)/include \
   $(XcpR_OUTPUT_PATH)/include
