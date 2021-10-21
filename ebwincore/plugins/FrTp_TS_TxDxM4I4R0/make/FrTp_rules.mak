#
# \file
#
# \brief AUTOSAR FrTp
#
# This file contains the implementation of the AUTOSAR
# module FrTp.
#
# \version 4.4.26
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


ifndef TS_MERGED_COMPILE
TS_MERGED_COMPILE := TRUE
endif

ifndef TS_FRTP_MERGED_COMPILE
TS_FRTP_MERGED_COMPILE := $(TS_MERGED_COMPILE)
endif

ifeq ($(TS_FRTP_MERGED_COMPILE),TRUE)

FrTp_src_FILES += \
    $(FrTp_CORE_PATH)/src/FrTp_Merged.c \
    $(FrTp_OUTPUT_PATH)/src/FrTp_Lcfg.c

else

FrTp_src_FILES += \
    $(FrTp_CORE_PATH)/src/FrTp.c \
    $(FrTp_CORE_PATH)/src/FrTp_LL.c \
    $(FrTp_CORE_PATH)/src/FrTp_TxStateMachine.c \
    $(FrTp_CORE_PATH)/src/FrTp_RxStateMachine.c \
    $(FrTp_CORE_PATH)/src/FrTp_GetVersionInfo.c \
    $(FrTp_OUTPUT_PATH)/src/FrTp_Lcfg.c

endif

LIBRARIES_TO_BUILD   += FrTp_src

# Fill the list with post build configuration files needed to build the post build binary.
FrTp_pbconfig_FILES := $(wildcard $(FrTp_OUTPUT_PATH)/src/FrTp_PBcfg.c)

# only add FrTp_pbconfig to the libraries to build if there are really post-build config files
# present
ifneq ($(strip $(FrTp_pbconfig_FILES)),)
LIBRARIES_PBCFG_TO_BUILD += FrTp_pbconfig
LIBRARIES_TO_BUILD += FrTp_pbconfig
endif

define defineFrTpLibOutputPATH
$(1)_OBJ_OUTPUT_PATH    := $(FrTp_lib_LIB_OUTPUT_PATH)
endef
$(foreach SRC,$(basename $(notdir $(subst \,/,$(FrTp_lib_FILES)))),$(eval $(call defineFrTpLibOutputPATH,$(SRC))))

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
