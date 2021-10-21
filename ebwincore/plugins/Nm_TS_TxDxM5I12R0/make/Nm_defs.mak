#
# \file
#
# \brief AUTOSAR Nm
#
# This file contains the implementation of the AUTOSAR
# module Nm.
#
# \version 5.12.8
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

Nm_CORE_PATH      ?= $(PLUGINS_BASE)/Nm_$(Nm_VARIANT)

Nm_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

Nm_DEPENDENT_PLUGINS := base_make tresos
Nm_VERSION           := 2.00.00
Nm_DESCRIPTION       := Nm Basic Software Makefile PlugIn for Autosar

CC_INCLUDE_PATH      += \
   $(Nm_CORE_PATH)/include \
   $(Nm_OUTPUT_PATH)/include
