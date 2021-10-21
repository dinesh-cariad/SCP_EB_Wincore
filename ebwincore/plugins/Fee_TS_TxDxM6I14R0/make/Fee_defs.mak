#
# \file
#
# \brief AUTOSAR Fee
#
# This file contains the implementation of the AUTOSAR
# module Fee.
#
# \version 6.14.13
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

Fee_CORE_PATH         ?= $(PLUGINS_BASE)/Fee_$(Fee_VARIANT)

Fee_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY
SSC_PLUGINS           += Fee
Fee_DEPENDENT_PLUGINS := base_make tresos
Fee_VERSION           := 2.00.00
Fee_DESCRIPTION       := Fee Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH       += \
   $(Fee_CORE_PATH)/include \
   $(Fee_OUTPUT_PATH)/include
