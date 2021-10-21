#
# \file
#
# \brief AUTOSAR CryIf
#
# This file contains the implementation of the AUTOSAR
# module CryIf.
#
# \version 1.0.24
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

CryIf_lib_FILES                 :=

CryIf_src_FILES                 +=                         \
  $(CryIf_CORE_PATH)/src/CryIf.c

LIBRARIES_TO_BUILD            += CryIf_src

DIRECTORIES_TO_CREATE         += $(CryIf_lib_LIB_OUTPUT_PATH)

CC_FILES_TO_BUILD             += $(CryIf_CORE_PATH)/src/CryIf.c $(CryIf_OUTPUT_PATH)/src/CryIf_Cfg.c
CPP_FILES_TO_BUILD            +=
ASM_FILES_TO_BUILD            +=

MAKE_CLEAN_RULES              +=
MAKE_GENERATE_RULES           +=
MAKE_COMPILE_RULES            +=
MAKE_CONFIG_RULES             +=
#MAKE_ADD_RULES               +=
#MAKE_DEBUG_RULES             +=

define defineCryIfLibOutputPATH
$(1)_OBJ_OUTPUT_PATH          := $(CryIf_lib_LIB_OUTPUT_PATH)
endef
$(foreach SRC,$(basename $(notdir $(subst \,/,$(CryIf_lib_FILES)))),\
$(eval $(call defineCryIfLibOutputPATH,$(SRC))))

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES

