#
# \file
#
# \brief AUTOSAR CanNm
#
# This file contains the implementation of the AUTOSAR
# module CanNm.
#
# \version 6.19.7
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

CanNm_CORE_PATH      ?= $(PLUGINS_BASE)/CanNm_$(CanNm_VARIANT)

CanNm_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

CanNm_GEN_FILES =                                  \
    $(CanNm_OUTPUT_PATH)/include/CanNm_Cfg.h   \
    $(CanNm_OUTPUT_PATH)/include/CanNm_PBcfg.h \
    $(CanNm_OUTPUT_PATH)/src/CanNm_PBcfg.c

TRESOS_GEN_FILES     += $(CanNm_GEN_FILES)

#################################################################
# REGISTRY

CanNm_DEPENDENT_PLUGINS := base_make tresos
CanNm_VERSION           := 2.00.00
CanNm_DESCRIPTION       := CanNm Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH       += $(CanNm_CORE_PATH)/include \
                         $(CanNm_CORE_PATH)/src \
                         $(CanNm_OUTPUT_PATH)/include \
                         $(CanNm_OUTPUT_PATH)/src
