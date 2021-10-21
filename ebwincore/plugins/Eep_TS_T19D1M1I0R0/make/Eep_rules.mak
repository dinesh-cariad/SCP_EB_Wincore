#
# \file
#
# \brief AUTOSAR Eep
#
# This file contains the implementation of the AUTOSAR
# module Eep.
#
# \version 1.0.9
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

LIBRARIES_TO_BUILD += Eep_src

Eep_src_FILES      := \
          $(Eep_CORE_PATH)/src/Eep.c \
          $(Eep_OUTPUT_PATH)/src/Eep_PBcfg.c

#################################################################
# RULES
