#
# \file
#
# \brief AUTOSAR ComM
#
# This file contains the implementation of the AUTOSAR
# module ComM.
#
# \version 5.19.7
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

ComM_CORE_PATH      ?= $(PLUGINS_BASE)/ComM_$(ComM_VARIANT)

ComM_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

ComM_GEN_FILES =                                  \
    $(ComM_OUTPUT_PATH)/include/ComM_PBcfg.h \
    $(ComM_OUTPUT_PATH)/src/ComM_PBcfg.c

TRESOS_GEN_FILES     += $(ComM_GEN_FILES)

#################################################################
# REGISTRY

ComM_DEPENDENT_PLUGINS := base_make tresos
ComM_VERSION           := 2.00.00
ComM_DESCRIPTION       := ComM Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH        += \
   $(ComM_CORE_PATH)/include \
   $(ComM_CORE_PATH)/src \
   $(ComM_OUTPUT_PATH)/include \
   $(ComM_OUTPUT_PATH)/src
