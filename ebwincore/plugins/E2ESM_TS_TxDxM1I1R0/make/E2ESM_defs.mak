#
# \file
#
# \brief AUTOSAR E2ESM
#
# This file contains the implementation of the AUTOSAR
# module E2ESM.
#
# \version 1.1.10
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

E2ESM_CORE_PATH         ?= $(PLUGINS_BASE)/E2ESM_$(E2ESM_VARIANT)

E2ESM_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

E2ESM_DEPENDENT_PLUGINS := base_make tresos
E2ESM_VERSION           := 2.00.00
E2ESM_DESCRIPTION       := E2ESM Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       +=    \
   $(E2ESM_CORE_PATH)/include
