#
# \file
#
# \brief AUTOSAR E2EPW
#
# This file contains the implementation of the AUTOSAR
# module E2EPW.
#
# \version 2.3.20
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

E2EPW_gen_FILES         += $(wildcard $(E2EPW_OUTPUT_PATH)/src/E2EPW*.c)
E2EPW_src_FILES      += $(wildcard $(E2EPW_CORE_PATH)/src/E2EPW*.c) \
                        $(E2EPW_gen_FILES)

LIBRARIES_TO_BUILD += E2EPW_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
