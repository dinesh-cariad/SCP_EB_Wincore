#
# \file
#
# \brief AUTOSAR FrArTp
#
# This file contains the implementation of the AUTOSAR
# module FrArTp.
#
# \version 1.0.8
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

ifndef TS_FRARTP_MERGED_COMPILE
TS_FRARTP_MERGED_COMPILE := $(TS_MERGED_COMPILE)
endif

ifeq ($(TS_FRARTP_MERGED_COMPILE),TRUE)

FrArTp_src_FILES +=                              \
          $(FrArTp_CORE_PATH)/src/FrArTp_Merged.c \

else


FrArTp_src_FILES += $(FrArTp_CORE_PATH)/src/FrArTp.c \

FrArTp_src_FILES_Encode += $(FrArTp_CORE_PATH)/src/FrArTp_Encode.c \

FrArTp_src_FILES_Decode += $(FrArTp_CORE_PATH)/src/FrArTp_Decode.c \

FrArTp_src_FILES_CfgAccess += $(FrArTp_CORE_PATH)/src/FrArTp_CfgAccess.c \

FrArTp_src_FILES_SM += $(FrArTp_CORE_PATH)/src/FrArTp_SM.c \
                       $(FrArTp_CORE_PATH)/src/FrArTp_SM_Rx.c \
                       $(FrArTp_CORE_PATH)/src/FrArTp_SM_Tx.c \

FrArTp_src_FILES_SduMng += $(FrArTp_CORE_PATH)/src/FrArTp_SduMng.c \

FrArTp_Units ?= Encode Decode CfgAccess SM SduMng
FrArTp_src_FILES += $(foreach unit,$(FrArTp_Units),$(FrArTp_src_FILES_$(unit)))

endif

FrArTp_src_FILES +=

LIBRARIES_TO_BUILD += FrArTp_src

# Fill the list with post build configuration files needed to build the post build binary.
FrArTp_pbconfig_FILES := $(wildcard $(FrArTp_OUTPUT_PATH)/src/FrArTp_PBcfg.c)

ifneq ($(strip $(FrArTp_pbconfig_FILES)),)
LIBRARIES_PBCFG_TO_BUILD += FrArTp_pbconfig
LIBRARIES_TO_BUILD += FrArTp_pbconfig
endif
