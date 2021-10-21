#
# \file
#
# \brief AUTOSAR Atomics
#
# This file contains the implementation of the AUTOSAR
# module Atomics.
#
# \version 1.0.15
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2017 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


Atomics_src_FILES := \
	$(Atomics_CORE_PATH)/src/Atomics.c \
	$(Atomics_CORE_PATH)/src/Atomics_TSPlatforms.c

ifneq ($(ATOMICS_USE_GENERIC_IMPL),0)
# The generic implementation shall be used and not the specialized one provided
# by EB tresos AutoCore OS. The generic implementation is contained in the files
# in the list Atomics_src_FILES.
LIBRARIES_TO_BUILD += Atomics_src

ifeq ($(MISRA_MODULE),Atomics)
# The Makefiles in asc_Testing for Polysapce expect the list of files to check
# in the Make variable Atomics_CC_FILES_TO_BUILD.
Atomics_CC_FILES_TO_BUILD += $(Atomics_src_FILES)
endif # $(MISRA_MODULE) == "Atomics"
endif # $(ATOMICS_USE_GENERIC_IMPL) != 0
