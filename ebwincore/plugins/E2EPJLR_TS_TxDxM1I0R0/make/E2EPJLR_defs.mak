#
# \file
#
# \brief AUTOSAR E2EPJLR
#
# This file contains the implementation of the AUTOSAR
# module E2EPJLR.
#
# \version 1.0.7
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

E2EPJLR_CORE_PATH         ?= $(PLUGINS_BASE)/E2EPJLR_$(E2EPJLR_VARIANT)

E2EPJLR_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

E2EPJLR_DEPENDENT_PLUGINS := base_make tresos
E2EPJLR_VERSION           := 2.00.00
E2EPJLR_DESCRIPTION       := E2EPJLR Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       +=    \
   $(E2EPJLR_CORE_PATH)/include
