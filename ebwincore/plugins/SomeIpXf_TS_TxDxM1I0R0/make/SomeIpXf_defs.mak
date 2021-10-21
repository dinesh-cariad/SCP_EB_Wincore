#
# \file
#
# \brief AUTOSAR SomeIpXf
#
# This file contains the implementation of the AUTOSAR
# module SomeIpXf.
#
# \version 1.0.47
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

SomeIpXf_CORE_PATH      ?= $(PLUGINS_BASE)/SomeIpXf_$(SomeIpXf_VARIANT)

SomeIpXf_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

SomeIpXf_DEPENDENT_PLUGINS      := base_make tresos
SomeIpXf_VERSION                := 1.00.00
SomeIpXf_DESCRIPTION            := SomeIpXf Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       +=       \
   $(SomeIpXf_OUTPUT_PATH)/include \
   $(SomeIpXf_CORE_PATH)/include

