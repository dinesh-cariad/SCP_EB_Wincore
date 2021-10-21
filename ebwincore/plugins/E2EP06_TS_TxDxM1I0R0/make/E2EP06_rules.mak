#
# \file
#
# \brief AUTOSAR E2EP06
#
# This file contains the implementation of the AUTOSAR
# module E2EP06.
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

E2EP06_src_FILES      :=  \
  $(E2EP06_CORE_PATH)/src/E2E_P06.c \
  $(E2EP06_CORE_PATH)/src/E2E_P06_E2EXf.c

LIBRARIES_TO_BUILD += E2EP06_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
