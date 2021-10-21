#
# \file
#
# \brief AUTOSAR CanTp
#
# This file contains the implementation of the AUTOSAR
# module CanTp.
#
# \version 6.8.42
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS


CanTp_CORE_PATH           ?= $(PLUGINS_BASE)/CanTp_$(CanTp_VARIANT)

CanTp_OUTPUT_PATH         ?= $(AUTOSAR_BASE_OUTPUT_PATH)

CanTp_GEN_FILES           := $(CanTp_OUTPUT_PATH)/include/CanTp_InternalCfg.h \
                             $(CanTp_OUTPUT_PATH)/include/CanTp_Cfg.h \
                             $(CanTp_OUTPUT_PATH)/include/CanTp_EntryTable.h \
                             $(CanTp_OUTPUT_PATH)/include/CanTp_PBcfg.h \
                             $(CanTp_OUTPUT_PATH)/src/CanTp_Cfg.c \
                             $(CanTp_OUTPUT_PATH)/src/CanTp_EntryTable.c \
                             $(CanTp_OUTPUT_PATH)/src/CanTp_ExitTable.c \
                             $(CanTp_OUTPUT_PATH)/src/CanTp_PBcfg.c \

TRESOS_GEN_FILES          += $(CanTp_GEN_FILES)

#################################################################
# REGISTRY

CanTp_DEPENDENT_PLUGINS  = base_make tresos
CanTp_VERSION            = 2.00.00
CanTp_DESCRIPTION        = CanTp Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH         += $(CanTp_OUTPUT_PATH)/include \
                           $(CanTp_CORE_PATH)/include \
                           $(CanTp_CORE_PATH)/src \
                           
ASM_INCLUDE_PATH        +=
