#
# \file
#
# \brief AUTOSAR WdgM
#
# This file contains the implementation of the AUTOSAR
# module WdgM.
#
# \version 6.1.39
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

WdgM_src_FILES      := \
    $(WdgM_CORE_PATH)/src/WdgM.c \
    $(WdgM_OUTPUT_PATH)/src/WdgM_Rte.c \
    $(WdgM_OUTPUT_PATH)/src/WdgM_BSW_Lcfg.c \
    $(wildcard $(WdgM_OUTPUT_PATH)/src/WdgM_Rte_*Lcfg.c)

LIBRARIES_TO_BUILD   += WdgM_src

#################################################################
# RULES
