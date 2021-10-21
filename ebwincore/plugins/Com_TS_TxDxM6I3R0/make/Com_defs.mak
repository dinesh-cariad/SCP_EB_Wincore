#
# \file
#
# \brief AUTOSAR Com
#
# This file contains the implementation of the AUTOSAR
# module Com.
#
# \version 6.3.50
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS
Com_CORE_PATH      ?= $(PLUGINS_BASE)/Com_$(Com_VARIANT)

Com_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

Com_DEPENDENT_PLUGINS = base_make tresos
Com_VERSION           = 2.00.00
Com_DESCRIPTION       = Com Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH      += \
   $(Com_CORE_PATH)/include \
   $(Com_CORE_PATH)/src \
   $(Com_OUTPUT_PATH)/include

ASM_INCLUDE_PATH     +=
