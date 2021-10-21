#
# \file
#
# \brief AUTOSAR NvM
#
# This file contains the implementation of the AUTOSAR
# module NvM.
#
# \version 6.17.22
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

NvM_CORE_PATH     ?= $(PLUGINS_BASE)/NvM_$(NvM_VARIANT)

NvM_OUTPUT_PATH   ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

NvM_DEPENDENT_PLUGINS := base_make tresos
NvM_VERSION           := 2.00.00
NvM_DESCRIPTION       := NvM Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       += \
  $(NvM_CORE_PATH)/include \
  $(NvM_OUTPUT_PATH)/include
