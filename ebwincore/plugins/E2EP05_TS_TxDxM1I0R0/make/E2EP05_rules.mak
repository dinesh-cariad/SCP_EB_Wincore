#
# \file
#
# \brief AUTOSAR E2EP05
#
# This file contains the implementation of the AUTOSAR
# module E2EP05.
#
# \version 1.0.11
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

E2EP05_src_FILES      :=  \
  $(E2EP05_CORE_PATH)/src/E2E_P05.c \
  $(E2EP05_CORE_PATH)/src/E2E_P05_E2EXf.c

LIBRARIES_TO_BUILD += E2EP05_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
