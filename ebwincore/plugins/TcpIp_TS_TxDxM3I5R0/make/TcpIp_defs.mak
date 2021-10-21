#
# \file
#
# \brief AUTOSAR TcpIp
#
# This file contains the implementation of the AUTOSAR
# module TcpIp.
#
# \version 3.5.13
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.



#################################################################
# DEFINITIONS

TcpIp_CORE_PATH           ?= $(PLUGINS_BASE)/TcpIp_$(TcpIp_VARIANT)

TcpIp_OUTPUT_PATH         ?= $(AUTOSAR_BASE_OUTPUT_PATH)

TcpIp_GEN_FILES      = \
                       $(TcpIp_OUTPUT_PATH)/include/TcpIp_Cfg.h \
                       $(TcpIp_OUTPUT_PATH)/include/TcpIp_PBcfg.h \
                       $(TcpIp_OUTPUT_PATH)/include/TcpIp_SymbolicNames_PBcfg.h \
                       $(TcpIp_OUTPUT_PATH)/src/TcpIp_PBcfg.c \
                       $(TcpIp_OUTPUT_PATH)/src/TcpIp_Lcfg.c \

TRESOS_GEN_FILES     += $(TcpIp_GEN_FILES)

#################################################################
# REGISTRY

TcpIp_DEPENDENT_PLUGINS := base_make tresos
TcpIp_VERSION           := 1.00.00
TcpIp_DESCRIPTION       := TcpIp Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH       += \
   $(TcpIp_CORE_PATH)/include \
   $(TcpIp_CORE_PATH)/src \
   $(TcpIp_OUTPUT_PATH)/include
