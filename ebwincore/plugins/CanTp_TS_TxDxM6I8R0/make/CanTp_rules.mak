#
# \file
#
# \brief AUTOSAR CanTp
#
# This file contains the implementation of the AUTOSAR
# module CanTp.
#
# \version 6.8.42
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

ifndef TS_CANTP_MERGED_COMPILE
TS_CANTP_MERGED_COMPILE := $(TS_MERGED_COMPILE)
endif

ifeq ($(TS_CANTP_MERGED_COMPILE),TRUE)

CanTp_src_FILES       += $(CanTp_CORE_PATH)/src/CanTp_Merged.c                 \

else

CanTp_src_FILES       += $(CanTp_CORE_PATH)/src/CanTp_MainFunction.c           \
                         $(CanTp_CORE_PATH)/src/CanTp_RxBufferDependentSend.c  \
                         $(CanTp_CORE_PATH)/src/CanTp_RxIndication.c           \
                         $(CanTp_CORE_PATH)/src/CanTp_TxBufferDependentSend.c  \
                         $(CanTp_CORE_PATH)/src/CanTp_StopChannel.c            \
                         $(CanTp_CORE_PATH)/src/CanTp_TransmitFrame.c          \
                         $(CanTp_CORE_PATH)/src/CanTp_CancelTransmit.c         \
                         $(CanTp_CORE_PATH)/src/CanTp_TxConfirmation.c         \
                         $(CanTp_CORE_PATH)/src/CanTp_STminCallback.c          \
                         $(CanTp_CORE_PATH)/src/CanTp_Transmit.c               \
                         $(CanTp_CORE_PATH)/src/CanTp_UnlockChannel.c          \
                         $(CanTp_CORE_PATH)/src/CanTp_PrepareFC.c              \
                         $(CanTp_CORE_PATH)/src/CanTp_PreparexF.c              \
                         $(CanTp_CORE_PATH)/src/CanTp_GetVersionInfo.c         \
                         $(CanTp_CORE_PATH)/src/CanTp_SetNSa.c                 \
                         $(CanTp_CORE_PATH)/src/CanTp_GetNSa.c                 \
                         $(CanTp_CORE_PATH)/src/CanTp_Init.c                   \
                         $(CanTp_CORE_PATH)/src/CanTp_ChangeParameter.c        \
                         $(CanTp_CORE_PATH)/src/CanTp_ReadParameter.c          \
                         $(CanTp_CORE_PATH)/src/CanTp_CancelReceive.c          \
                         $(CanTp_CORE_PATH)/src/CanTp.c                        \

endif

CanTp_src_FILES       += $(CanTp_OUTPUT_PATH)/src/CanTp_Cfg.c                  \
                         $(CanTp_OUTPUT_PATH)/src/CanTp_Lcfg.c                  \
                         $(CanTp_OUTPUT_PATH)/src/CanTp_EntryTable.c           \
                         $(CanTp_OUTPUT_PATH)/src/CanTp_ExitTable.c            \

LIBRARIES_TO_BUILD     += CanTp_src

# Fill the list with post build configuration files needed to build the post build binary.
CanTp_pbconfig_FILES := $(wildcard $(CanTp_OUTPUT_PATH)/src/CanTp_PBcfg.c)

ifneq ($(strip $(CanTp_pbconfig_FILES)),)
LIBRARIES_PBCFG_TO_BUILD += CanTp_pbconfig
LIBRARIES_TO_BUILD += CanTp_pbconfig
endif

CPP_FILES_TO_BUILD   +=
ASM_FILES_TO_BUILD   +=

MAKE_CLEAN_RULES     +=
MAKE_GENERATE_RULES  +=
MAKE_COMPILE_RULES   +=
MAKE_CONFIG_RULES    +=

define defineCanTpLibOutputPATH
$(1)_OBJ_OUTPUT_PATH += $(CanTp_lib_LIB_OUTPUT_PATH)
endef

$(foreach SRC,$(basename $(notdir $(subst \,/,$(CanTp_lib_FILES)))),          \
              $(eval $(call defineCanTpLibOutputPATH,$(SRC))))



#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
