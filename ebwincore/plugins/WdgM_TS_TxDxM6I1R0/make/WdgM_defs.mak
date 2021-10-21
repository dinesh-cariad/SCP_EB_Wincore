#
# \file
#
# \brief AUTOSAR WdgM
#
# This file contains the implementation of the AUTOSAR
# module WdgM.
#
# \version 6.1.39
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

WdgM_CORE_PATH      ?= $(PLUGINS_BASE)/WdgM_$(WdgM_VARIANT)

WdgM_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

WdgM_DEPENDENT_PLUGINS := base_make tresos
WdgM_VERSION           := 2.00.00
WdgM_DESCRIPTION       := WdgM Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH        += \
   $(WdgM_CORE_PATH)/include \
   $(WdgM_OUTPUT_PATH)/include
