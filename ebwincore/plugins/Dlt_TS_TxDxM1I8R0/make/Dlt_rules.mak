#
# \file
#
# \brief AUTOSAR Dlt
#
# This file contains the implementation of the AUTOSAR
# module Dlt.
#
# \version 1.8.6
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

Dlt_src_FILES          := \
     $(Dlt_CORE_PATH)/src/Dlt.c \
     $(Dlt_CORE_PATH)/src/Dlt_Control.c \
     $(Dlt_CORE_PATH)/src/Dlt_Core.c \
     $(Dlt_CORE_PATH)/src/Dlt_ReceiveProcessor.c \
     $(Dlt_CORE_PATH)/src/Dlt_Rte.c \
     $(Dlt_OUTPUT_PATH)/src/Dlt_Cfg.c \
     $(wildcard $(Dlt_OUTPUT_PATH)/src/Dlt_SessionMapping.c) \
     $(wildcard $(Dlt_OUTPUT_PATH)/src/Dlt_Rte_hook.c)

LIBRARIES_TO_BUILD     += Dlt_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
