#
# \file
#
# \brief AUTOSAR E2E
#
# This file contains the implementation of the AUTOSAR
# module E2E.
#
# \version 2.0.18
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

E2E_src_FILES      :=  \
  $(E2E_CORE_PATH)/src/E2E.c \
  $(E2E_CORE_PATH)/src/E2E_Mem.c \
  $(E2E_CORE_PATH)/src/E2E_PXX_E2EXf.c \
  $(E2E_CORE_PATH)/src/E2E_No_Check_E2EXf.c
  
LIBRARIES_TO_BUILD += E2E_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
