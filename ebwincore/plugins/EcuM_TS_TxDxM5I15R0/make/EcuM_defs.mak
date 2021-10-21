#
# \file
#
# \brief AUTOSAR EcuM
#
# This file contains the implementation of the AUTOSAR
# module EcuM.
#
# \version 5.15.7
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

EcuM_CORE_PATH    ?= $(PLUGINS_BASE)/EcuM_$(EcuM_VARIANT)

EcuM_OUTPUT_PATH  ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY


EcuM_DEPENDENT_PLUGINS := base_make tresos
EcuM_VERSION           := 2.00.00
EcuM_DESCRIPTION       := EcuM Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH        += \
   $(EcuM_CORE_PATH)/include \
   $(EcuM_OUTPUT_PATH)/include
