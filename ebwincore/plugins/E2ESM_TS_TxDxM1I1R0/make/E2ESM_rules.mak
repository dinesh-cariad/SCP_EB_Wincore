#
# \file
#
# \brief AUTOSAR E2ESM
#
# This file contains the implementation of the AUTOSAR
# module E2ESM.
#
# \version 1.1.10
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

E2ESM_src_FILES      :=  \
  $(E2ESM_CORE_PATH)/src/E2E_SM.c

LIBRARIES_TO_BUILD += E2ESM_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
