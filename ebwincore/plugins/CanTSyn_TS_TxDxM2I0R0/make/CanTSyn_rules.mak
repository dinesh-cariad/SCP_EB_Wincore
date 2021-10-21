#
# \file
#
# \brief AUTOSAR CanTSyn
#
# This file contains the implementation of the AUTOSAR
# module CanTSyn.
#
# \version 2.0.8
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

LIBRARIES_TO_BUILD     += CanTSyn_src

CanTSyn_src_FILES      := \
    $(CanTSyn_CORE_PATH)/src/CanTSyn.c \
    $(CanTSyn_OUTPUT_PATH)/src/CanTSyn_Cfg.c \

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES