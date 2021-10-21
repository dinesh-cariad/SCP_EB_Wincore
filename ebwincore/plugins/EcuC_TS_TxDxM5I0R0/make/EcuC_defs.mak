#
# \file
#
# \brief AUTOSAR EcuC
#
# This file contains the implementation of the AUTOSAR
# module EcuC.
#
# \version 5.0.21
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.



#################################################################
# DEFINITIONS

EcuC_CORE_PATH           ?= $(PLUGINS_BASE)/EcuC_$(EcuC_VARIANT)

EcuC_OUTPUT_PATH         ?= $(AUTOSAR_BASE_OUTPUT_PATH)

EcuC_GEN_FILES      = \
                       $(EcuC_OUTPUT_PATH)/include/EcuC_Cfg.h \
                       $(EcuC_OUTPUT_PATH)/include/EcuC_SymbolicNames_Cfg.h \
                       $(EcuC_OUTPUT_PATH)/src/EcuC_Cfg.c \

#################################################################
# REGISTRY
SSC_PLUGINS             += EcuC
EcuC_DEPENDENT_PLUGINS := base_make tresos
EcuC_VERSION           := 1.00.00
EcuC_DESCRIPTION       := EcuC Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH       += \
   $(EcuC_CORE_PATH)/src \
   $(EcuC_CORE_PATH)/include
