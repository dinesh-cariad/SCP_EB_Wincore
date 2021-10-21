#
# \file
#
# \brief AUTOSAR PbcfgM
#
# This file contains the implementation of the AUTOSAR
# module PbcfgM.
#
# \version 1.2.22
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

PbcfgM_CORE_PATH    ?= $(PLUGINS_BASE)/PbcfgM_$(PbcfgM_VARIANT)

PbcfgM_OUTPUT_PATH  ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY


PbcfgM_DEPENDENT_PLUGINS := base_make tresos
PbcfgM_VERSION           := 2.00.00
PbcfgM_DESCRIPTION       = PbcfgM Basic Software Makefile PlugIn

CC_INCLUDE_PATH        += \
   $(PbcfgM_OUTPUT_PATH)/include \
   $(PbcfgM_CORE_PATH)/include \
   $(PbcfgM_CORE_PATH)/src
