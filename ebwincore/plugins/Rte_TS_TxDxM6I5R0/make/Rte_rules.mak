#
# \file
#
# \brief AUTOSAR Rte
#
# This file contains the implementation of the AUTOSAR
# module Rte.
#
# \version 6.5.3
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# REGISTRY

Rte_src_FILES      := $(wildcard $(Rte_OUTPUT_PATH)/src/Rte*.c)

LIBRARIES_TO_BUILD += Rte_src
