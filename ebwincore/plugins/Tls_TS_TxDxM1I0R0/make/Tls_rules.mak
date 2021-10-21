# \file
#
# \brief AUTOSAR Crc
#
# This file contains the implementation of the AUTOSAR
# module Crc.
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2017 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.

#################################################################
# DEFINITIONS

#################################################################
# REGISTRY

# TODO: support for merged compile?

Tls_src_FILES_Encode := \
    $(Tls_CORE_PATH)/src/Tls_Encode.c \

Tls_src_FILES_Decode := \
    $(Tls_CORE_PATH)/src/Tls_Decode.c \

Tls_src_FILES := \
    $(Tls_CORE_PATH)/src/Tls.c \
    $(Tls_OUTPUT_PATH)/src/Tls_Cfg.c \
    $(Tls_OUTPUT_PATH)/src/Tls_Lcfg.c \
    $(Tls_CORE_PATH)/src/Tls_Debug.c

Tls_Units ?= Encode Decode

Tls_src_FILES += $(foreach unit,$(Tls_Units),$(Tls_src_FILES_$(unit)))

LIBRARIES_TO_BUILD += Tls_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
