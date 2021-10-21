#
# \file
#
# \brief AUTOSAR FiM
#
# This file contains the implementation of the AUTOSAR
# module FiM.
#
# \version 2.5.3
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

FiM_CORE_PATH         ?= $(PLUGINS_BASE)/FiM_$(FiM_VARIANT)

FiM_OUTPUT_PATH       ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

CC_INCLUDE_PATH       +=    \
   $(FiM_CORE_PATH)/include \
   $(FiM_OUTPUT_PATH)/include
