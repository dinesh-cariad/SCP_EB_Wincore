#
# \file
#
# \brief AUTOSAR WdgIf
#
# This file contains the implementation of the AUTOSAR
# module WdgIf.
#
# \version 6.1.26
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

WdgIf_CORE_PATH      ?= $(PLUGINS_BASE)/WdgIf_$(WdgIf_VARIANT)

WdgIf_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

WdgIf_DEPENDENT_PLUGINS := base_make tresos
WdgIf_VERSION           := 2.00.00
WdgIf_DESCRIPTION       := WdgIf Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH         += \
   $(WdgIf_CORE_PATH)/include \
   $(WdgIf_OUTPUT_PATH)/include
