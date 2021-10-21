#
# \file
#
# \brief AUTOSAR BswM
#
# This file contains the implementation of the AUTOSAR
# module BswM.
#
# \version 1.15.6
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

LIBRARIES_TO_BUILD   += BswM_src
LIBRARIES_PBCFG_TO_BUILD += BswM_pbconfig

ifndef TS_BSWM_COMPILE_WITH_POSTBUILD
TS_BSWM_COMPILE_WITH_POSTBUILD := TRUE
endif

ifndef TS_BUILD_POST_BUILD_BINARY
TS_BUILD_POST_BUILD_BINARY := FALSE
endif

BswM_src_FILES  := $(BswM_CORE_PATH)/src/BswM.c \
                   $(BswM_OUTPUT_PATH)/src/BswM_Cfg.c \
                   $(wildcard $(BswM_OUTPUT_PATH)/src/BswM_*Lcfg.c)

ifeq ($(TS_BSWM_COMPILE_WITH_POSTBUILD),TRUE)
# Compile with postbuild
BswM_src_FILES += $(BswM_OUTPUT_PATH)/src/BswM_PBcfg.c
endif

ifeq ($(TS_BUILD_POST_BUILD_BINARY),TRUE)
# If the post build binary shall be built do not compile any files other than the postbuild files.
BswM_src_FILES :=
endif

# Fill the list with post build configuration files needed to build the post build binary.
BswM_pbconfig_FILES := $(BswM_OUTPUT_PATH)/src/BswM_PBcfg.c

define defineBswMLibOutputPATH
$(1)_OBJ_OUTPUT_PATH    := $(BswM_lib_LIB_OUTPUT_PATH)
endef
$(foreach SRC,$(basename $(notdir $(subst \,/,$(BswM_lib_FILES)))),$(eval $(call defineBswMLibOutputPATH,$(SRC))))

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES




