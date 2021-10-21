#
# \file
#
# \brief AUTOSAR ComXf
#
# This file contains the implementation of the AUTOSAR
# module ComXf.
#
# \version 1.0.37
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

ComXf_CORE_PATH      ?= $(PLUGINS_BASE)/ComXf_$(ComXf_VARIANT)

ComXf_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

ComXf_DEPENDENT_PLUGINS      := base_make tresos
ComXf_VERSION                := 1.00.00
ComXf_DESCRIPTION            := ComXf Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       +=       \
   $(ComXf_OUTPUT_PATH)/include \
   $(ComXf_CORE_PATH)/include

