#
# \file
#
# \brief AUTOSAR Eth
#
# This file contains the implementation of the AUTOSAR
# module Eth.
#
# \version 0.0.8
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2011 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


Eth_CORE_PATH      ?= $(PLUGINS_BASE)/Eth_$(Eth_VARIANT)

Eth_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

Eth_GEN_FILES      := $(Eth_OUTPUT_PATH)/include/Eth_Cfg.h \
                      $(Eth_OUTPUT_PATH)/src/Eth_Cfg.c

TRESOS_GEN_FILES   += $(Eth_GEN_FILES)

CC_INCLUDE_PATH    += $(Eth_CORE_PATH)/include \
                      $(Eth_CORE_PATH)/src \
                      $(Eth_OUTPUT_PATH)/include

# WINDOWS\WIN32X86 specific part

ifeq ($(TOOLCHAIN),mgcc62)
LINK_OPT_POST += -lwpcap
LIB_INCLUDE_PATH += $(Eth_CORE_PATH)/src
else
LIBRARIES_LINK_ONLY += $(Eth_CORE_PATH)/src/libpacket.a \
                       $(Eth_CORE_PATH)/src/libwpcap.a
endif
