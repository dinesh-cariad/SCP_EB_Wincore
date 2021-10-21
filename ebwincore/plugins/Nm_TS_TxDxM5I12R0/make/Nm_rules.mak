#
# \file
#
# \brief AUTOSAR Nm
#
# This file contains the implementation of the AUTOSAR
# module Nm.
#
# \version 5.12.8
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

Nm_src_FILES       := \
 $(Nm_CORE_PATH)/src/Nm.c \
 $(Nm_OUTPUT_PATH)/src/Nm_Cfg.c

LIBRARIES_TO_BUILD += Nm_src

#################################################################
# RULES
