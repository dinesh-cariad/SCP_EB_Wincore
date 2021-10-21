#
# \file
#
# \brief AUTOSAR E2EP05
#
# This file contains the implementation of the AUTOSAR
# module E2EP05.
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

E2EP05_CORE_PATH         ?= $(PLUGINS_BASE)/E2EP05_$(E2EP05_VARIANT)

E2EP05_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

E2EP05_DEPENDENT_PLUGINS := base_make tresos
E2EP05_VERSION           := 2.00.00
E2EP05_DESCRIPTION       := E2EP05 Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       +=    \
   $(E2EP05_CORE_PATH)/include
