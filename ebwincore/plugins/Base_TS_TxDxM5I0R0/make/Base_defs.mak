#
# \file
#
# \brief AUTOSAR Base
#
# This file contains the implementation of the AUTOSAR
# module Base.
#
# \version 5.0.31
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

Base_CORE_PATH         ?= $(PLUGINS_BASE)/Base_$(Base_VARIANT)

Base_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

CC_INCLUDE_PATH        += \
   $(Base_CORE_PATH)/include \
   $(Base_OUTPUT_PATH)/include
