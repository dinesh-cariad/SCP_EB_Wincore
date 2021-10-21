#
# \file
#
# \brief AUTOSAR E2EP01
#
# This file contains the implementation of the AUTOSAR
# module E2EP01.
#
# \version 2.2.17
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

E2EP01_CORE_PATH         ?= $(PLUGINS_BASE)/E2EP01_$(E2EP01_VARIANT)

E2EP01_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

E2EP01_DEPENDENT_PLUGINS := base_make tresos
E2EP01_VERSION           := 2.00.00
E2EP01_DESCRIPTION       := E2EP01 Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       +=    \
   $(E2EP01_CORE_PATH)/include
