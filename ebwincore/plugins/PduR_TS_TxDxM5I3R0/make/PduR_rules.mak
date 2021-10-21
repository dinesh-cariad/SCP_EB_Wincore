#
# \file
#
# \brief AUTOSAR PduR
#
# This file contains the implementation of the AUTOSAR
# module PduR.
#
# \version 5.3.46
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

ifndef TS_MERGED_COMPILE
TS_MERGED_COMPILE := TRUE
endif

ifndef TS_PDUR_MERGED_COMPILE
TS_PDUR_MERGED_COMPILE := $(TS_MERGED_COMPILE)
endif

#################################################################
# REGISTRY

PduR_src_FILES_static      += \
   $(PduR_CORE_PATH)/src/PduR_GetVersionInfo.c \
   $(PduR_CORE_PATH)/src/PduR_GateIfRxIndicationDf.c \
   $(PduR_CORE_PATH)/src/PduR_GateIfTxConfirmationDf.c \
   $(PduR_CORE_PATH)/src/PduR_GateIfRxIndicationTf.c \
   $(PduR_CORE_PATH)/src/PduR_GateIfTriggerTransmitTf.c \
   $(PduR_CORE_PATH)/src/PduR_GateIfTxConfirmationTf.c \
   $(PduR_CORE_PATH)/src/PduR_GateIfRxIndicationSb.c \
   $(PduR_CORE_PATH)/src/PduR_GateIfTriggerTransmitSb.c \
   $(PduR_CORE_PATH)/src/PduR_GateIfRxIndicationNb.c \
   $(PduR_CORE_PATH)/src/PduR_GateTp.c \
   $(PduR_CORE_PATH)/src/PduR_GateTpUpReception.c \
   $(PduR_CORE_PATH)/src/PduR_GateTpStartOfReception.c \
   $(PduR_CORE_PATH)/src/PduR_GateTpCopyRxData.c \
   $(PduR_CORE_PATH)/src/PduR_GateTpRxIndication.c \
   $(PduR_CORE_PATH)/src/PduR_GateTpCopyTxData.c \
   $(PduR_CORE_PATH)/src/PduR_GateTpTxConfirmation.c \
   $(PduR_CORE_PATH)/src/PduR_Nto1GateTpStartOfReception.c \
   $(PduR_CORE_PATH)/src/PduR_Nto1GateTpCopyRxData.c \
   $(PduR_CORE_PATH)/src/PduR_Nto1GateTpRxIndication.c \
   $(PduR_CORE_PATH)/src/PduR_Nto1GateTpCopyTxData.c \
   $(PduR_CORE_PATH)/src/PduR_Nto1GateTpTxConfirmation.c \
   $(PduR_CORE_PATH)/src/PduR_MultiUpIfTransmit.c \
   $(PduR_CORE_PATH)/src/PduR_MultiUpMCoreIfTransmit.c \
   $(PduR_CORE_PATH)/src/PduR_MultiIfTriggerTransmit.c \
   $(PduR_CORE_PATH)/src/PduR_MultiIfTxConfirmation.c \
   $(PduR_CORE_PATH)/src/PduR_MultiIfRxIndication.c \
   $(PduR_CORE_PATH)/src/PduR_MultiIf.c \
   $(PduR_CORE_PATH)/src/PduR_MultiTpCopyTxData.c \
   $(PduR_CORE_PATH)/src/PduR_MultiTpTxConfirmation.c \
   $(PduR_CORE_PATH)/src/PduR_MultiTpTransmit.c \
   $(PduR_CORE_PATH)/src/PduR_MultiTp.c \
   $(PduR_CORE_PATH)/src/PduR_GenericIfRxIndication.c \
   $(PduR_CORE_PATH)/src/PduR_GenericIfTransmit.c \
   $(PduR_CORE_PATH)/src/PduR_GenericIfTriggerTransmit.c \
   $(PduR_CORE_PATH)/src/PduR_GenericIfTxConfirmation.c \
   $(PduR_CORE_PATH)/src/PduR_GenericTpChangeParameter.c \
   $(PduR_CORE_PATH)/src/PduR_GenericTpCancelTransmit.c \
   $(PduR_CORE_PATH)/src/PduR_GenericTpCancelReceive.c \
   $(PduR_CORE_PATH)/src/PduR_GenericTpStartOfReception.c \
   $(PduR_CORE_PATH)/src/PduR_GenericTpCopyRxData.c \
   $(PduR_CORE_PATH)/src/PduR_GenericTpRxIndication.c \
   $(PduR_CORE_PATH)/src/PduR_GenericTpCopyTxData.c \
   $(PduR_CORE_PATH)/src/PduR_GenericTpTxConfirmation.c \
   $(PduR_CORE_PATH)/src/PduR_GenericTpTransmit.c \
   $(PduR_CORE_PATH)/src/PduR_Init.c \
   $(PduR_CORE_PATH)/src/PduR_IsValidConfig.c \
   $(PduR_CORE_PATH)/src/PduR_WrapASR32.c \
   $(PduR_CORE_PATH)/src/PduR_GetConfigurationId.c \
   $(PduR_CORE_PATH)/src/PduR_EnableRouting.c \
   $(PduR_CORE_PATH)/src/PduR_DisableRouting.c \
   $(PduR_CORE_PATH)/src/PduR_RPathGroups.c


# Static source dynamic pay load
PduR_src_FILES_static += \
   $(PduR_CORE_PATH)/src/PduR_GateIfTriggerTransmitSbDynPyld.c \
   $(PduR_CORE_PATH)/src/PduR_GateIfRxIndicationSbDynPyld.c \
   $(PduR_CORE_PATH)/src/PduR_GateIfRxIndicationSbMCoreDynPyld.c \
   $(PduR_CORE_PATH)/src/PduR_GateIfTxConfirmationDfDynPyld.c \
   $(PduR_CORE_PATH)/src/PduR_GateIfRxIndicationDfDynPyld.c \
   $(PduR_CORE_PATH)/src/PduR_GateIfTriggerTransmitTfDynPyld.c \
   $(PduR_CORE_PATH)/src/PduR_GateIfTxConfirmationTfDynPyld.c \
   $(PduR_CORE_PATH)/src/PduR_GateIfTxConfirmationDfMCoreDynPyld.c \
   $(PduR_CORE_PATH)/src/PduR_GateIfRxIndicationTfDynPyld.c \
   $(PduR_CORE_PATH)/src/PduR_GateIfRxIndicationDfMCoreDynPyld.c \
   $(PduR_CORE_PATH)/src/PduR_GateIfRxIndicationMcUpSbDynPyld.c


ifeq ($(TS_PDUR_MERGED_COMPILE),TRUE)
# Merged file compile
PduR_src_FILES := $(PduR_CORE_PATH)/src/PduR_Merged.c
else
# Single file compile
PduR_src_FILES := $(PduR_CORE_PATH)/src/PduR.c
PduR_src_FILES += $(PduR_src_FILES_static)
endif

# Add generated files to the list of source files
PduR_src_FILES += $(PduR_OUTPUT_PATH)/src/PduR_AdjLayerApi.c
PduR_src_FILES += $(PduR_OUTPUT_PATH)/src/PduR_Lcfg.c

LIBRARIES_TO_BUILD += PduR_src

# Fill the list with post build configuration files needed to build the post build binary.
PduR_pbconfig_FILES := $(wildcard $(PduR_OUTPUT_PATH)/src/PduR_PBcfg.c)

# only add PduR_pbconfig to the libraries to build if there are really post-build config files
# present
ifneq ($(strip $(PduR_pbconfig_FILES)),)
LIBRARIES_PBCFG_TO_BUILD += PduR_pbconfig
LIBRARIES_TO_BUILD += PduR_pbconfig
endif

define definePduRLibOutputPATH
$(1)_OBJ_OUTPUT_PATH    := $(PduR_lib_LIB_OUTPUT_PATH)
endef

$(foreach SRC,$(basename $(notdir $(subst \,/,$(PduR_lib_FILES)))),$(eval $(call definePduRLibOutputPATH,$(SRC))))

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
