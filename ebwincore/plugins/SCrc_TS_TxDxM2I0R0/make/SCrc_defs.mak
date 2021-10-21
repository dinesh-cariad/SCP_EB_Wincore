#
# \file
#
# \brief AUTOSAR SCrc
#
# This file contains the implementation of the AUTOSAR
# module SCrc.
#
# \version 2.0.11
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

SCrc_CORE_PATH         ?= $(PLUGINS_BASE)/SCrc_$(SCrc_VARIANT)

SCrc_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

SCrc_DEPENDENT_PLUGINS := base_make tresos
SCrc_VERSION           := 2.00.00
SCrc_DESCRIPTION       := Crc Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH       +=    \
   $(SCrc_CORE_PATH)/include
