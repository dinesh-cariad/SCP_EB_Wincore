#
# \file
#
# \brief AUTOSAR Det
#
# This file contains the implementation of the AUTOSAR
# module Det.
#
# \version 6.5.6
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

Det_CORE_PATH         ?= $(PLUGINS_BASE)/Det_$(Det_VARIANT)

Det_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

CC_INCLUDE_PATH       += \
   $(Det_CORE_PATH)/include \
   $(Det_OUTPUT_PATH)/include
