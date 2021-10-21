#
# \file
#
# \brief AUTOSAR EcuM
#
# This file contains the implementation of the AUTOSAR
# module EcuM.
#
# \version 5.15.7
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

LIBRARIES_TO_BUILD  += EcuM_src

EcuM_src_FILES      := \
   $(EcuM_CORE_PATH)/src/EcuM.c \
   $(EcuM_CORE_PATH)/src/EcuM_BootTarget.c \
   $(EcuM_CORE_PATH)/src/EcuM_Rte.c \
   $(EcuM_CORE_PATH)/src/EcuM_ASR40_Rte.c \
   $(EcuM_CORE_PATH)/src/EcuM_ASR32_Rte.c \
   $(EcuM_OUTPUT_PATH)/src/EcuM_Cfg.c \
   $(EcuM_OUTPUT_PATH)/src/EcuM_PBcfg.c \
   $(EcuM_OUTPUT_PATH)/src/EcuM_Lcfg.c

##############################################################################
# DEPENDENCIES (only for assembler files)
#

##############################################################################
# RULES
