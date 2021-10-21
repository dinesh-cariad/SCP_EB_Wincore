# \file
#
# \brief AUTOSAR Os
#
# This file contains the implementation of the AUTOSAR
# module Os.
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 1998 - 2020 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.

#################################################################
# DEFINITIONS
ifeq ($(OS_ROOT),)
  AutosarOS_CORE_PATH    = $(SSC_ROOT)\Os_TS_T19D1M6I0R0
  AutosarOS_LIB_PATH     = $(SSC_ROOT)\Os_$(Os_VARIANT)\lib
else
  AutosarOS_CORE_PATH    = $(OS_ROOT)
  AutosarOS_LIB_PATH     = $(OS_ROOT)\lib
endif

OS_ARCH := WINDOWS
OS_CPU := WIN32X86

include $(AutosarOS_CORE_PATH)/make/Os_definitions.mak
#########################
# Editor settings: DO NOT DELETE
# vi:set ts=4:
