#
# \file
#
# \brief AUTOSAR FrArTp
#
# This file contains the implementation of the AUTOSAR
# module FrArTp.
#
# \version 1.0.8
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.



#################################################################
# DEFINITIONS

FrArTp_CORE_PATH           ?= $(PLUGINS_BASE)/FrArTp_$(FrArTp_VARIANT)

FrArTp_OUTPUT_PATH         ?= $(AUTOSAR_BASE_OUTPUT_PATH)

FrArTp_GEN_FILES      = \
                       $(FrArTp_OUTPUT_PATH)/include/FrArTp_Cfg.h \
                       $(FrArTp_OUTPUT_PATH)/include/FrArTp_PBcfg.h \
                       $(FrArTp_OUTPUT_PATH)/include/FrArTp_SymbolicNames_PBcfg.h \
                       $(FrArTp_OUTPUT_PATH)/src/FrArTp_PBcfg.c \

TRESOS_GEN_FILES     += $(FrArTp_GEN_FILES)

#################################################################
# REGISTRY

FrArTp_DEPENDENT_PLUGINS := base_make tresos
FrArTp_VERSION           := 1.00.00
FrArTp_DESCRIPTION       := FrArTp Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH       += \
   $(FrArTp_CORE_PATH)/include \
   $(FrArTp_CORE_PATH)/src \
   $(FrArTp_OUTPUT_PATH)/include
