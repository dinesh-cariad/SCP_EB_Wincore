#
# \file
#
# \brief AUTOSAR Ea
#
# This file contains the implementation of the AUTOSAR
# module Ea.
#
# \version 5.12.14
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

Ea_src_FILES       := \
    $(Ea_CORE_PATH)/src/Ea.c \
    $(Ea_OUTPUT_PATH)/src/Ea_Cfg.c

LIBRARIES_TO_BUILD += Ea_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
