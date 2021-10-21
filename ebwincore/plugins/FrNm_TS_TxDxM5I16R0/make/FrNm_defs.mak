#
# \file
#
# \brief AUTOSAR FrNm
#
# This file contains the implementation of the AUTOSAR
# module FrNm.
#
# \version 5.16.7
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

FrNm_CORE_PATH      ?= $(PLUGINS_BASE)/FrNm_$(FrNm_VARIANT)

FrNm_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

FrNm_GEN_FILES =                                  \
    $(FrNm_OUTPUT_PATH)/include/FrNm_Cfg.h   \
    $(FrNm_OUTPUT_PATH)/include/FrNm_PBcfg.h \
    $(FrNm_OUTPUT_PATH)/src/FrNm_PBcfg.c

TRESOS_GEN_FILES     += $(FrNm_GEN_FILES)

#################################################################
# REGISTRY

FrNm_DEPENDENT_PLUGINS := base_make tresos
FrNm_VERSION           := 2.00.00
FrNm_DESCRIPTION       := FrNm Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       += $(FrNm_CORE_PATH)/include \
                         $(FrNm_CORE_PATH)/src \
                         $(FrNm_OUTPUT_PATH)/include \
                         $(FrNm_OUTPUT_PATH)/src
