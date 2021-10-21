#
# \file
#
# \brief AUTOSAR E2EP02
#
# This file contains the implementation of the AUTOSAR
# module E2EP02.
#
# \version 2.1.17
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

E2EP02_CORE_PATH         ?= $(PLUGINS_BASE)/E2EP02_$(E2EP02_VARIANT)

E2EP02_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

E2EP02_DEPENDENT_PLUGINS := base_make tresos
E2EP02_VERSION           := 2.00.00
E2EP02_DESCRIPTION       := E2EP02 Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       +=    \
   $(E2EP02_CORE_PATH)/include
