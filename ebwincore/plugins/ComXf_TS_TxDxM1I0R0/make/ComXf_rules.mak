#
# \file
#
# \brief AUTOSAR ComXf
#
# This file contains the implementation of the AUTOSAR
# module ComXf.
#
# \version 1.0.37
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

ComXf_src_FILES += \
    $(ComXf_CORE_PATH)/src/ComXf.c \
    $(wildcard $(ComXf_OUTPUT_PATH)/src/ComXf*_PartitionApi.c) \
    $(wildcard $(ComXf_OUTPUT_PATH)/src/ComXf_S*_PartitionApi.c)

LIBRARIES_TO_BUILD     += ComXf_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
