#
# \file
#
# \brief AUTOSAR FrTp
#
# This file contains the implementation of the AUTOSAR
# module FrTp.
#
# \version 4.4.26
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS
FrTp_CORE_PATH    ?= $(PLUGINS_BASE)/FrTp_$(FrTp_VARIANT)

FrTp_OUTPUT_PATH  ?= $(AUTOSAR_BASE_OUTPUT_PATH)

FrTp_GEN_FILES =                                  \
    $(FrTp_OUTPUT_PATH)/include/FrTp_Cfg.h   \
    $(FrTp_OUTPUT_PATH)/include/FrTp_PBcfg.h \
    $(FrTp_OUTPUT_PATH)/src/FrTp_PBcfg.c

TRESOS_GEN_FILES     += $(FrTp_GEN_FILES)

#################################################################
# REGISTRY

FrTp_DEPENDENT_PLUGINS := base_make tresos
FrTp_VERSION           := 1.00.00
FrTp_DESCRIPTION       := FrTp Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH       += $(FrTp_CORE_PATH)/include \
                         $(FrTp_CORE_PATH)/src \
                         $(FrTp_OUTPUT_PATH)/include \
                         $(FrTp_OUTPUT_PATH)/src

ASM_INCLUDE_PATH      +=
CPP_INCLUDE_PATH      +=
