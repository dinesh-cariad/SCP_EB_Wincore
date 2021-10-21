#
# \file
#
# \brief AUTOSAR Mirror
#
# This file contains the implementation of the AUTOSAR
# module Mirror.
#
# \version 1.0.17
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

Mirror_CORE_PATH   ?= $(PLUGINS_BASE)/Mirror_$(Mirror_VARIANT)

Mirror_OUTPUT_PATH ?= $(AUTOSAR_BASE_OUTPUT_PATH)

Mirror_GEN_FILES    = \
                       $(Mirror_OUTPUT_PATH)/include/Mirror_Cfg.h \
                       $(Mirror_OUTPUT_PATH)/include/Mirror_Types.h \
                       $(Mirror_OUTPUT_PATH)/include/Mirror_DefProg_Cfg.h \
                       $(Mirror_OUTPUT_PATH)/src/Mirror_Cfg.c \

TRESOS_GEN_FILES     += $(Mirror_GEN_FILES)


#################################################################
# REGISTRY


Mirror_DEPENDENT_PLUGINS := base_make tresos
Mirror_VERSION           := 1.00.00
Mirror_DESCRIPTION       := Mirror Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH            += \
  $(Mirror_CORE_PATH)/include \
  $(Mirror_OUTPUT_PATH)/include
ASM_INCLUDE_PATH           +=