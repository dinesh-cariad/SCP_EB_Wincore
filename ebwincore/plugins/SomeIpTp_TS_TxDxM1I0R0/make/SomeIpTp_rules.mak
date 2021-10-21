#
# \file
#
# \brief AUTOSAR SomeIpTp
#
# This file contains the implementation of the AUTOSAR
# module SomeIpTp.
#
# \version 1.0.28
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

LIBRARIES_TO_BUILD     += SomeIpTp_src

SomeIpTp_src_FILES      := \
    $(SomeIpTp_CORE_PATH)/src/SomeIpTp.c \
    $(SomeIpTp_OUTPUT_PATH)/src/SomeIpTp_Cfg.c \

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES