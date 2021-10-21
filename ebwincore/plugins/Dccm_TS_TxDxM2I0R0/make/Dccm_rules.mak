#
# \file
#
# \brief AUTOSAR Dccm
#
# This file contains the implementation of the AUTOSAR
# module Dccm.
#
# \version 2.0.6
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

Dccm_lib_FILES      :=


Dccm_src_FILES := \
  $(Dccm_CORE_PATH)\src\Dccm_Cbk.c \
  $(Dccm_CORE_PATH)\src\Dccm_MainFunction.c \
  $(Dccm_CORE_PATH)\src\Dccm_Service.c \
  $(Dccm_CORE_PATH)\src\Dccm_Validation.c \
  $(Dccm_OUTPUT_PATH)\src\Dccm_Cfg.c 
  
LIBRARIES_TO_BUILD     += Dccm_src

DIRECTORIES_TO_CREATE += $(Dccm_lib_LIB_OUTPUT_PATH)

CC_FILES_TO_BUILD       +=
CPP_FILES_TO_BUILD      +=
ASM_FILES_TO_BUILD      +=

MAKE_CLEAN_RULES        +=
MAKE_GENERATE_RULES     +=
MAKE_COMPILE_RULES      +=
#MAKE_DEBUG_RULES       +=
MAKE_CONFIG_RULES       +=
#MAKE_ADD_RULES         +=


define defineDccmLibOutputPATH
$(1)_OBJ_OUTPUT_PATH    := $(Dccm_lib_LIB_OUTPUT_PATH)
endef
$(foreach SRC,$(basename $(notdir $(subst \,/,$(Dccm_lib_FILES)))),\
$(eval $(call defineDccmLibOutputPATH,$(SRC))))

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
