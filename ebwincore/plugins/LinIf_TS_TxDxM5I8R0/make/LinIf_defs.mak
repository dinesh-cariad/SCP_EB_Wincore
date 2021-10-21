#
# \file
#
# \brief AUTOSAR LinIf
#
# This file contains the implementation of the AUTOSAR
# module LinIf.
#
# \version 5.8.27
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS


LinIf_CORE_PATH      ?= $(PLUGINS_BASE)/LinIf_$(LinIf_VARIANT)

LinIf_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

LinIf_GEN_FILES      = \
                    $(LinIf_OUTPUT_PATH)/include/LinIf_Cfg.h \
                    $(LinIf_OUTPUT_PATH)/include/LinIf_PBcfg.h \
                    $(LinIf_OUTPUT_PATH)/include/LinTp_PBcfg.h \
                    $(LinIf_OUTPUT_PATH)/src/LinIf_Cfg.c     \
                    $(LinIf_OUTPUT_PATH)/src/LinIf_PBcfg.c     \
                    $(LinIf_OUTPUT_PATH)/src/LinTp_PBcfg.c \
                   
TRESOS_GEN_FILES += LinIf_GEN_FILES

#################################################################
# REGISTRY

LinIf_DEPENDENT_PLUGINS = base_make tresos
LinIf_VERSION           = 2.0.0
LinIf_DESCRIPTION       = LinIf Basic Software Makefile PlugIn for Autosar


CC_INCLUDE_PATH      += $(LinIf_OUTPUT_PATH)/include \
                        $(LinIf_CORE_PATH)/include \
                        $(LinIf_CORE_PATH)/src \

ASM_INCLUDE_PATH     +=
