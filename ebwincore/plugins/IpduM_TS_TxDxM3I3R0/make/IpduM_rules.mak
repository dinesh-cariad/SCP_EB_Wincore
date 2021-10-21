#
# \file
#
# \brief AUTOSAR IpduM
#
# This file contains the implementation of the AUTOSAR
# module IpduM.
#
# \version 3.3.40
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


LIBRARIES_TO_BUILD  += IpduM_src

IpduM_lib_FILES       +=

IpduM_src_FILES += $(IpduM_CORE_PATH)/src/IpduM.c \
                   $(IpduM_OUTPUT_PATH)/src/IpduM_IsValidConfig.c \
                       $(IpduM_CORE_PATH)/src/IpduM_ComCallout.c \
                       $(IpduM_OUTPUT_PATH)/src/IpduM_MultiMF.c \
                       $(IpduM_OUTPUT_PATH)/src/IpduM_Lcfg.c

CC_FILES_TO_BUILD   +=


CPP_FILES_TO_BUILD  +=
ASM_FILES_TO_BUILD  +=


MAKE_CLEAN_RULES    +=
MAKE_GENERATE_RULES +=
MAKE_COMPILE_RULES  +=
#MAKE_DEBUG_RULES    +=
MAKE_CONFIG_RULES   +=
#MAKE_ADD_RULES      +=


# Fill the list with post build configuration files needed to build the post build binary.
IpduM_pbconfig_FILES := $(wildcard $(IpduM_OUTPUT_PATH)/src/IpduM_PBcfg.c)

# only add IpduM_pbconfig to the libraries to build if there are really post-build config files
# present
ifneq ($(strip $(IpduM_pbconfig_FILES)),)
LIBRARIES_PBCFG_TO_BUILD += IpduM_pbconfig
LIBRARIES_TO_BUILD += IpduM_pbconfig
endif

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
