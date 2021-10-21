#
# \file
#
# \brief AUTOSAR SomeIpTp
#
# This file contains the implementation of the AUTOSAR
# module SomeIpTp.
#
# \version 1.0.28
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

SomeIpTp_CORE_PATH   ?= $(PLUGINS_BASE)/SomeIpTp_$(SomeIpTp_VARIANT)

SomeIpTp_OUTPUT_PATH ?= $(AUTOSAR_BASE_OUTPUT_PATH)

SomeIpTp_GEN_FILES    = \
                       $(SomeIpTp_OUTPUT_PATH)/include/SomeIpTp_Cfg.h \
                       $(SomeIpTp_OUTPUT_PATH)/src/SomeIpTp_Cfg.c \

TRESOS_GEN_FILES     += $(SomeIpTp_GEN_FILES)


#################################################################
# REGISTRY


SomeIpTp_DEPENDENT_PLUGINS := base_make tresos
SomeIpTp_VERSION           := 1.00.00
SomeIpTp_DESCRIPTION       := SomeIpTp Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH            += \
  $(SomeIpTp_CORE_PATH)/include \
  $(SomeIpTp_OUTPUT_PATH)/include
ASM_INCLUDE_PATH           +=
