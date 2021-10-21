#
# \file
#
# \brief AUTOSAR E2EXf
#
# This file contains the implementation of the AUTOSAR
# module E2EXf.
#
# \version 1.0.36
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

E2EXf_src_FILES      :=  \
  $(E2EXf_CORE_PATH)/src/E2EXf.c \
  $(wildcard $(E2EXf_OUTPUT_PATH)/src/E2EXf*_PartitionApi.c)

LIBRARIES_TO_BUILD += E2EXf_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
