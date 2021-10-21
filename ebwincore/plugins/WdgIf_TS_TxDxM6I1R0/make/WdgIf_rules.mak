#
# \file
#
# \brief AUTOSAR WdgIf
#
# This file contains the implementation of the AUTOSAR
# module WdgIf.
#
# \version 6.1.26
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

WdgIf_src_FILES    := $(WdgIf_CORE_PATH)/src/WdgIf.c

LIBRARIES_TO_BUILD += WdgIf_src

#################################################################
# RULES
