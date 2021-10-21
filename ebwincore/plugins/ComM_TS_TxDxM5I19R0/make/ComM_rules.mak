#
# \file
#
# \brief AUTOSAR ComM
#
# This file contains the implementation of the AUTOSAR
# module ComM.
#
# \version 5.19.7
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

ComM_src_FILES      := \
   $(ComM_CORE_PATH)/src/ComM.c \
   $(ComM_CORE_PATH)/src/ComM_Hsm.c \
   $(ComM_CORE_PATH)/src/ComM_HsmComMData.c \
   $(ComM_CORE_PATH)/src/ComM_HsmComMFnct.c \
   $(ComM_CORE_PATH)/src/ComM_HsmComMPncData.c \
   $(ComM_CORE_PATH)/src/ComM_HsmComMPncFnct.c \
   $(ComM_CORE_PATH)/src/ComM_Rte.c \
   $(ComM_CORE_PATH)/src/ComM_ASR32_Rte.c \
   $(ComM_CORE_PATH)/src/ComM_ASR40_Rte.c \
   $(ComM_OUTPUT_PATH)/src/ComM_BSW_Cfg.c \
   $(ComM_OUTPUT_PATH)/src/ComM_Lcfg.c \
   $(ComM_OUTPUT_PATH)/src/ComM_Rte_Cfg.c \
   $(ComM_OUTPUT_PATH)/src/ComM_MainFunction.c

LIBRARIES_TO_BUILD   += ComM_src

# Fill the list with post build configuration files needed to build the post build binary.
ComM_pbconfig_FILES := $(wildcard $(ComM_OUTPUT_PATH)/src/ComM_PBcfg.c)

# only add ComM_pbconfig to the Pbcfg libraries to build if there
# are really post-build config files present
ifneq ($(strip $(ComM_pbconfig_FILES)),)
# this is used by asc_Make
LIBRARIES_PBCFG_TO_BUILD += ComM_pbconfig
# asc_Make can only separately generate module or Pb libraries, but not both
# in the default case (only generate module libs), we also want to
# generate the Pb libraries, for proper testing.
LIBRARIES_TO_BUILD += ComM_pbconfig
endif

define defineComMLibOutputPATH
$(1)_OBJ_OUTPUT_PATH    := $(ComM_lib_LIB_OUTPUT_PATH)
endef
$(foreach SRC,$(basename $(notdir $(subst \,/,$(ComM_lib_FILES)))),$(eval $(call defineComMLibOutputPATH,$(SRC))))

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
