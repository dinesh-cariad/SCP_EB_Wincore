#
# \file
#
# \brief AUTOSAR MemIf
#
# This file contains the implementation of the AUTOSAR
# module MemIf.
#
# \version 5.11.11
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

MemIf_CORE_PATH      ?= $(PLUGINS_BASE)/MemIf_$(MemIf_VARIANT)

MemIf_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

CC_INCLUDE_PATH      += \
   $(MemIf_CORE_PATH)/include \
   $(MemIf_OUTPUT_PATH)/include
