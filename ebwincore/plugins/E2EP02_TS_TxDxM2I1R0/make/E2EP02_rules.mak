#
# \file
#
# \brief AUTOSAR E2EP02
#
# This file contains the implementation of the AUTOSAR
# module E2EP02.
#
# \version 2.1.17
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

E2EP02_src_FILES      :=  \
  $(E2EP02_CORE_PATH)/src/E2E_P02.c \
  $(E2EP02_CORE_PATH)/src/E2E_P02_E2EXf.c

LIBRARIES_TO_BUILD += E2EP02_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
