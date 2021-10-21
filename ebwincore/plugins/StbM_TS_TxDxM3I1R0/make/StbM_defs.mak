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

StbM_CORE_PATH      ?= $(PLUGINS_BASE)/StbM_$(StbM_VARIANT)

StbM_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

StbM_GEN_FILES      := \
        $(StbM_OUTPUT_PATH)/include/StbM_Cfg.h \
        $(StbM_OUTPUT_PATH)/include/StbM_Cfg_Int.h \
        $(StbM_OUTPUT_PATH)/include/StbM_Partitions.h \
        $(StbM_OUTPUT_PATH)/src/StbM_Cfg.c \
        $(StbM_OUTPUT_PATH)/src/StbM_SharedData_Int.c

TRESOS_GEN_FILES        += $(StbM_GEN_FILES)

CC_INCLUDE_PATH            += \
        $(StbM_CORE_PATH)/include \
        $(StbM_OUTPUT_PATH)/include

ASM_INCLUDE_PATH           +=

#################################################################
# REGISTRY

