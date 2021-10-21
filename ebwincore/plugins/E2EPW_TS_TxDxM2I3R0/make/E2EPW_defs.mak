#
# \file
#
# \brief AUTOSAR E2EPW
#
# This file contains the implementation of the AUTOSAR
# module E2EPW.
#
# \version 2.3.20
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

E2EPW_CORE_PATH         ?= $(PLUGINS_BASE)/E2EPW_$(E2EPW_VARIANT)

E2EPW_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

E2EPW_DEPENDENT_PLUGINS := base_make tresos
E2EPW_VERSION           := 2.00.00
E2EPW_DESCRIPTION       := E2EPW Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       +=    \
   $(E2EPW_CORE_PATH)/include \
   $(E2EPW_OUTPUT_PATH)/include
