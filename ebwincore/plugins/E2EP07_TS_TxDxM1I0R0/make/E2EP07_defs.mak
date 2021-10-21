#
# \file
#
# \brief AUTOSAR E2EP07
#
# This file contains the implementation of the AUTOSAR
# module E2EP07.
#
# \version 1.0.12
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

E2EP07_CORE_PATH         ?= $(PLUGINS_BASE)/E2EP07_$(E2EP07_VARIANT)

E2EP07_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

E2EP07_DEPENDENT_PLUGINS := base_make tresos
E2EP07_VERSION           := 2.00.00
E2EP07_DESCRIPTION       := E2EP07 Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       +=    \
   $(E2EP07_CORE_PATH)/include
