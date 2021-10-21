#
# \file
#
# \brief AUTOSAR E2EPRN
#
# This file contains the implementation of the AUTOSAR
# module E2EPRN.
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

E2EPRN_CORE_PATH         ?= $(PLUGINS_BASE)/E2EPRN_$(E2EPRN_VARIANT)

E2EPRN_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

E2EPRN_DEPENDENT_PLUGINS := base_make tresos
E2EPRN_VERSION           := 2.00.00
E2EPRN_DESCRIPTION       := E2EPRN Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       +=    \
   $(E2EPRN_CORE_PATH)/include
