#
# \file
#
# \brief AUTOSAR E2EPRN
#
# This file contains the implementation of the AUTOSAR
# module E2EPRN.
#
# \version 1.0.12
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

E2EPRN_src_FILES      :=  \
  $(E2EPRN_CORE_PATH)/src/E2E_PRN.c \
  $(E2EPRN_CORE_PATH)/src/E2E_PRN_E2EXf.c

LIBRARIES_TO_BUILD += E2EPRN_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
