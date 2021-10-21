#
# \file
#
# \brief AUTOSAR PduR
#
# This file contains the implementation of the AUTOSAR
# module PduR.
#
# \version 5.3.46
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

PduR_CORE_PATH           ?= $(PLUGINS_BASE)/PduR_$(PduR_VARIANT)

PduR_OUTPUT_PATH         ?= $(AUTOSAR_BASE_OUTPUT_PATH)


#################################################################
# REGISTRY

PduR_DEPENDENT_PLUGINS := base_make tresos
PduR_VERSION           := 2.00.00
PduR_DESCRIPTION       := PduR Basic Software Makefile PlugIn for AUTOSAR


CC_INCLUDE_PATH       +=       \
   $(PduR_CORE_PATH)/include   \
   $(PduR_CORE_PATH)/src       \
   $(PduR_OUTPUT_PATH)/include

ASM_INCLUDE_PATH     +=
