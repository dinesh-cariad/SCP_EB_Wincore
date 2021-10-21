#
# \file
#
# \brief AUTOSAR E2E
#
# This file contains the implementation of the AUTOSAR
# module E2E.
#
# \version 2.0.18
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

E2E_CORE_PATH         ?= $(PLUGINS_BASE)/E2E_$(E2E_VARIANT)

E2E_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

E2E_DEPENDENT_PLUGINS := base_make tresos
E2E_VERSION           := 2.00.00
E2E_DESCRIPTION       := E2E Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       +=    \
   $(E2E_CORE_PATH)/include
