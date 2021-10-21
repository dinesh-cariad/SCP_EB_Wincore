#
# \file
#
# \brief AUTOSAR Mirror
#
# This file contains the implementation of the AUTOSAR
# module Mirror.
#
# \version 1.0.17
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

LIBRARIES_TO_BUILD     += Mirror_src

Mirror_src_FILES      := \
    $(Mirror_CORE_PATH)/src/Mirror.c \
    $(Mirror_CORE_PATH)/src/Mirror_Operations.c \
    $(Mirror_OUTPUT_PATH)/src/Mirror_Cfg.c \

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES