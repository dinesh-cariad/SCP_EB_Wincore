#
# \file
#
# \brief AUTOSAR XcpR
#
# This file contains the implementation of the AUTOSAR
# module XcpR.
#
# \version 1.1.0
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

XcpR_src_FILES      := $(XcpR_CORE_PATH)/src/XcpR.c \
                       $(XcpR_CORE_PATH)/src/XcpR_SourceProcessor.c \
                       $(XcpR_CORE_PATH)/src/XcpR_DestinationProcessor.c \
                       $(XcpR_OUTPUT_PATH)/src/XcpR_Cfg.c \

LIBRARIES_TO_BUILD += XcpR_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES




