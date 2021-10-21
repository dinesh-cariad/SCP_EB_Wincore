#
# \file
#
# \brief AUTOSAR FrTSyn
#
# This file contains the implementation of the AUTOSAR
# module FrTSyn.
#
# \version 2.0.9
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

LIBRARIES_TO_BUILD     += FrTSyn_src

FrTSyn_src_FILES      := \
    $(FrTSyn_CORE_PATH)/src/FrTSyn.c \
    $(FrTSyn_OUTPUT_PATH)/src/FrTSyn_Cfg.c \

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES