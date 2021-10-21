#
# \file
#
# \brief AUTOSAR E2EP01
#
# This file contains the implementation of the AUTOSAR
# module E2EP01.
#
# \version 2.2.17
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

E2EP01_src_FILES      :=  \
  $(E2EP01_CORE_PATH)/src/E2E_P01.c \
  $(E2EP01_CORE_PATH)/src/E2E_P01_E2EXf.c

LIBRARIES_TO_BUILD += E2EP01_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
