#
# \file
#
# \brief AUTOSAR FiM
#
# This file contains the implementation of the AUTOSAR
# module FiM.
#
# \version 2.5.3
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

FiM_src_FILES      := $(FiM_CORE_PATH)/src/FiM.c \
                      $(FiM_OUTPUT_PATH)/src/FiM_Cfg.c

LIBRARIES_TO_BUILD += FiM_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES




