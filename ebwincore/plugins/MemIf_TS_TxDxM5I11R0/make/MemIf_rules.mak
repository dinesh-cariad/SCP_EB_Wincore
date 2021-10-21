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

#################################################################
# REGISTRY

MemIf_src_FILES    := $(MemIf_CORE_PATH)/src/MemIf.c

LIBRARIES_TO_BUILD += MemIf_src

#################################################################
# RULES
