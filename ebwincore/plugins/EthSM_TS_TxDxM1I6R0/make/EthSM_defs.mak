#
# \file
#
# \brief AUTOSAR EthSM
#
# This file contains the implementation of the AUTOSAR
# module EthSM.
#
# \version 1.6.14
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

EthSM_CORE_PATH      ?= $(PLUGINS_BASE)/EthSM_$(EthSM_VARIANT)

EthSM_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

EthSM_GEN_FILES      := $(EthSM_OUTPUT_PATH)/include/EthSM_Cfg.h \
                        $(EthSM_OUTPUT_PATH)/include/EthSM_PBcfg.h \
                        $(EthSM_OUTPUT_PATH)/include/EthSM_Lcfg.h \
                        $(EthSM_OUTPUT_PATH)/src/EthSM_PBcfg.c \
                        $(EthSM_OUTPUT_PATH)/src/EthSM_Lcfg.c \

TRESOS_GEN_FILES        += $(EthSM_GEN_FILES)

#################################################################
# REGISTRY

EthSM_DEPENDENT_PLUGINS      := base_make tresos
EthSM_VERSION                := 1.00.00
EthSM_DESCRIPTION            := EthSM Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH            += $(EthSM_CORE_PATH)/include \
                              $(EthSM_CORE_PATH)/src \
                              $(EthSM_OUTPUT_PATH)/include

