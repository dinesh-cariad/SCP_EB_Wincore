#
# \file
#
# \brief AUTOSAR LdCom
#
# This file contains the implementation of the AUTOSAR
# module LdCom.
#
# \version 1.0.26
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

LdCom_src_FILES += \
    $(LdCom_CORE_PATH)/src/LdCom.c \
    $(LdCom_OUTPUT_PATH)/src/LdCom_Lcfg.c

LIBRARIES_TO_BUILD     += LdCom_src

# Fill the list with post build configuration files needed to build the post build binary.
LdCom_pbconfig_FILES := $(wildcard $(LdCom_OUTPUT_PATH)/src/LdCom_PBcfg.c)

# only add LdCom_pbconfig to the libraries to build if there are really post-build config files
# present
ifneq ($(strip $(LdCom_pbconfig_FILES)),)
LIBRARIES_PBCFG_TO_BUILD += LdCom_pbconfig
LIBRARIES_TO_BUILD += LdCom_pbconfig
endif

define defineLdComLibOutputPATH
$(1)_OBJ_OUTPUT_PATH    := $(LdCom_lib_LIB_OUTPUT_PATH)
endef

$(foreach SRC,$(basename $(notdir $(subst \,/,$(LdCom_lib_FILES)))),$(eval $(call defineLdComLibOutputPATH,$(SRC))))

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
