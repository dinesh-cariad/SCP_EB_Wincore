#
# \file
#
# \brief AUTOSAR E2EPJLR
#
# This file contains the implementation of the AUTOSAR
# module E2EPJLR.
#
# \version 1.0.7
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

E2EPJLR_src_FILES      :=  \
  $(E2EPJLR_CORE_PATH)/src/E2E_PJLR.c \
  $(E2EPJLR_CORE_PATH)/src/E2E_PJLR_E2EXf.c

LIBRARIES_TO_BUILD += E2EPJLR_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
