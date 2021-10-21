#
# \file
#
# \brief AUTOSAR Crc
#
# This file contains the implementation of the AUTOSAR
# module Crc.
#
# \version 6.11.13
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

Crc_CORE_PATH         ?= $(PLUGINS_BASE)/Crc_$(Crc_VARIANT)

Crc_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

Crc_DEPENDENT_PLUGINS := base_make tresos
Crc_VERSION           := 2.00.00
Crc_DESCRIPTION       := Crc Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       +=    \
   $(Crc_CORE_PATH)/include \
   $(Crc_OUTPUT_PATH)/include
