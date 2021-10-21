#
# \file
#
# \brief AUTOSAR E2EP04
#
# This file contains the implementation of the AUTOSAR
# module E2EP04.
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

E2EP04_CORE_PATH         ?= $(PLUGINS_BASE)/E2EP04_$(E2EP04_VARIANT)

E2EP04_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

E2EP04_DEPENDENT_PLUGINS := base_make tresos
E2EP04_VERSION           := 2.00.00
E2EP04_DESCRIPTION       := E2EP04 Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       +=    \
   $(E2EP04_CORE_PATH)/include
