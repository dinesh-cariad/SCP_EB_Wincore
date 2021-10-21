#
# \file
#
# \brief AUTOSAR Crypto
#
# This file contains the implementation of the AUTOSAR
# module Crypto.
#
# \version 1.7.42
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#================================================================
# DEFINITIONS

Crypto_xVIx_xAIx_INSTANCE                      := Crypto_xVIx_xAIx

ifeq ($(BUILD_MODE),LIB)
ifeq ($(MODULE),Crypto)
LIB_VARIANTS += Crypto_xVIx_xAIx_BASE
endif
endif

Crypto_xVIx_xAIx_CORE_PATH           ?=
Crypto_xVIx_xAIx_OUTPUT_PATH         ?= $(AUTOSAR_BASE_OUTPUT_PATH)

Crypto_xVIx_xAIx_GEN_FILES           := \
  $(Crypto_xVIx_xAIx_CORE_PATH)/generate/include/Crypto_Cfg.h

Crypto_xVIx_xAIx_lib_LIB_OUTPUT_PATH := $(Crypto_xVIx_xAIx_CORE_PATH)/lib

#================================================================
# REGISTRY

SSC_PLUGINS                          += Crypto_xVIx_xAIx
Crypto_xVIx_xAIx_DEPENDENT_PLUGINS   := base_make tresos
Crypto_xVIx_xAIx_DESCRIPTION         := Crypto Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH += \
  $(Crypto_xVIx_xAIx_OUTPUT_PATH)/instance/$(Crypto_xVIx_xAIx_INSTANCE)/include \
  $(Crypto_xVIx_xAIx_OUTPUT_PATH)/include


