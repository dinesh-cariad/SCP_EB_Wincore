#
# \file
#
# \brief AUTOSAR StbM
#
# This file contains the implementation of the AUTOSAR
# module StbM.
#
# \version 3.1.7
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

LIBRARIES_TO_BUILD    += StbM_src

StbM_src_FILES      := \
        $(StbM_CORE_PATH)/src/StbM.c \
        $(StbM_OUTPUT_PATH)/src/StbM_Cfg.c \
        $(StbM_OUTPUT_PATH)/src/StbM_SharedData_Int.c \
        $(StbM_OUTPUT_PATH)/src/StbM_Partitions.c
