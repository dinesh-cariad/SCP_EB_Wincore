#
# \file
#
# \brief AUTOSAR E2EXf
#
# This file contains the implementation of the AUTOSAR
# module E2EXf.
#
# \version 1.0.36
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

E2EXf_CORE_PATH         ?= $(PLUGINS_BASE)/E2EXf_$(E2EXf_VARIANT)

E2EXf_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

E2EXf_DEPENDENT_PLUGINS := base_make tresos
E2EXf_VERSION           := 2.00.00
E2EXf_DESCRIPTION       := E2EXf Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       +=    \
   $(E2EXf_CORE_PATH)/include \
   $(E2EXf_OUTPUT_PATH)/include
