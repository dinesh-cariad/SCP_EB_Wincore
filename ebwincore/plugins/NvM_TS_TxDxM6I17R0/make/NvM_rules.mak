#
# \file
#
# \brief AUTOSAR NvM
#
# This file contains the implementation of the AUTOSAR
# module NvM.
#
# \version 6.17.22
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

NvM_src_FILES :=  \
    $(NvM_CORE_PATH)/src/NvM.c \
    $(NvM_CORE_PATH)/src/NvM_Rte.c \
    $(NvM_CORE_PATH)/src/NvM_ASR32_Rte.c \
    $(NvM_CORE_PATH)/src/NvM_ASR40_Rte.c \
    $(NvM_CORE_PATH)/src/NvM_ASR42_Rte.c \
    $(NvM_CORE_PATH)/src/NvM_CalcCrc.c \
    $(NvM_CORE_PATH)/src/NvM_DataIntern.c \
    $(NvM_CORE_PATH)/src/NvM_Queue.c \
    $(NvM_CORE_PATH)/src/NvM_ReadAll.c \
    $(NvM_CORE_PATH)/src/NvM_ReadBlock.c \
    $(NvM_CORE_PATH)/src/NvM_RestoreBlockDefaults.c \
    $(NvM_CORE_PATH)/src/NvM_StateMachine.c \
    $(NvM_CORE_PATH)/src/NvM_ValidateAll.c \
    $(NvM_CORE_PATH)/src/NvM_WriteAll.c \
    $(NvM_CORE_PATH)/src/NvM_WriteBlock.c \
    $(NvM_CORE_PATH)/src/NvM_Wrapper.c \
    $(NvM_CORE_PATH)/src/NvM_BlockCheck.c \
    $(NvM_CORE_PATH)/src/NvM_FirstInitAll.c \
    $(NvM_OUTPUT_PATH)/src/NvM_Cfg.c

LIBRARIES_TO_BUILD  += NvM_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
