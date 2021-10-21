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

#################################################################
# REGISTRY

Base_src_FILES      := \
  $(Base_CORE_PATH)/src/TSMem.c \
  $(Base_CORE_PATH)/src/TSPBConfig.c \
  $(Base_CORE_PATH)/src/ComStack_Helpers.c

LIBRARIES_TO_BUILD   += Base_src

#################################################################
# RULES
