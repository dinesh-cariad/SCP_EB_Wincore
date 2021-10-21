#
# \file
#
# \brief AUTOSAR BswM
#
# This file contains the implementation of the AUTOSAR
# module BswM.
#
# \version 1.15.6
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

BswM_CORE_PATH         ?= $(PLUGINS_BASE)/BswM_$(BswM_VARIANT)

BswM_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

BswM_DEPENDENT_PLUGINS := base_make tresos
BswM_VERSION           := 1.00.00
BswM_DESCRIPTION       := BswM Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH        +=    \
   $(BswM_CORE_PATH)/include \
   $(BswM_OUTPUT_PATH)/include
