#
# \file
#
# \brief AUTOSAR E2EP06
#
# This file contains the implementation of the AUTOSAR
# module E2EP06.
#
# \version 1.0.11
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

E2EP06_CORE_PATH         ?= $(PLUGINS_BASE)/E2EP06_$(E2EP06_VARIANT)

E2EP06_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

E2EP06_DEPENDENT_PLUGINS := base_make tresos
E2EP06_VERSION           := 2.00.00
E2EP06_DESCRIPTION       := E2EP06 Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       +=    \
   $(E2EP06_CORE_PATH)/include
