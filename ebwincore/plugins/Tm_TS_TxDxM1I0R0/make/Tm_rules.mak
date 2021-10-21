#
# \file
#
# \brief AUTOSAR Tm
#
# This file contains the implementation of the AUTOSAR
# module Tm.
#
# \version 1.0.4
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

Tm_src_FILES      := $(Tm_CORE_PATH)/src/Tm.c

LIBRARIES_TO_BUILD += Tm_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES

