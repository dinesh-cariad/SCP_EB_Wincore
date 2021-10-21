#
# \file
#
# \brief AUTOSAR SomeIpXf
#
# This file contains the implementation of the AUTOSAR
# module SomeIpXf.
#
# \version 1.0.47
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

SomeIpXf_src_FILES += \
    $(SomeIpXf_CORE_PATH)/src/SomeIpXf.c \
    $(SomeIpXf_CORE_PATH)/src/SomeIpXf_S.c \
    $(SomeIpXf_CORE_PATH)/src/SomeIpXf_Int.c \
    $(SomeIpXf_CORE_PATH)/src/SomeIpXf_S_Int.c \
    $(wildcard $(SomeIpXf_OUTPUT_PATH)/src/SomeIpXf_Cfg.c) \
    $(wildcard $(SomeIpXf_OUTPUT_PATH)/src/SomeIpXf*_PartitionApi.c)

LIBRARIES_TO_BUILD     += SomeIpXf_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
