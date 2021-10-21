#
# \file
#
# \brief AUTOSAR UdpNm
#
# This file contains the implementation of the AUTOSAR
# module UdpNm.
#
# \version 2.9.7
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

UdpNm_CORE_PATH      ?= $(PLUGINS_BASE)/UdpNm_$(UdpNm_VARIANT)

UdpNm_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

UdpNm_GEN_FILES =                                  \
    $(UdpNm_OUTPUT_PATH)/include/UdpNm_Cfg.h   \
    $(UdpNm_OUTPUT_PATH)/include/UdpNm_PBcfg.h \
    $(UdpNm_OUTPUT_PATH)/src/UdpNm_PBcfg.c

TRESOS_GEN_FILES     += $(UdpNm_GEN_FILES)

#################################################################
# REGISTRY

UdpNm_DEPENDENT_PLUGINS := base_make tresos
UdpNm_VERSION           := 2.00.00
UdpNm_DESCRIPTION       := UdpNm Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH       += $(UdpNm_CORE_PATH)/include \
                         $(UdpNm_CORE_PATH)/src \
                         $(UdpNm_OUTPUT_PATH)/include \
                         $(UdpNm_OUTPUT_PATH)/src
