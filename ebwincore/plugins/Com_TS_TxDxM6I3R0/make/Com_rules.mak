#
# \file
#
# \brief AUTOSAR Com
#
# This file contains the implementation of the AUTOSAR
# module Com.
#
# \version 6.3.50
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

ifndef TS_COM_MERGED_COMPILE
TS_COM_MERGED_COMPILE := $(TS_MERGED_COMPILE)
endif



Com_lib_FILES_Core    = \
                      $(Com_CORE_PATH)/src/Com_DeInit.c \
                      $(Com_CORE_PATH)/src/Com_GetStatus.c \


Com_lib_FILES_Core_PreCompile    = \
                      $(Com_CORE_PATH)/src/Com.c \
                      $(Com_CORE_PATH)/src/Com_Core_IPduGroup.c \
                      $(Com_CORE_PATH)/src/Com_Core_RxIPdu.c \
                      $(Com_CORE_PATH)/src/Com_Core_TxIPdu.c \
                      $(Com_CORE_PATH)/src/Com_Core_TxSignal.c \
                      $(Com_CORE_PATH)/src/Com_Core_TxSignalGroup.c \
                      $(Com_CORE_PATH)/src/Com_IpduGroupControl.c \
                      $(Com_CORE_PATH)/src/Com_ReceptionDMControl.c \
                      $(Com_CORE_PATH)/src/Com_GetConfigurationId.c \
                      $(Com_CORE_PATH)/src/Com_Init.c \
                      $(Com_CORE_PATH)/src/Com_IsValidConfig.c \
                      $(Com_CORE_PATH)/src/Com_ClearIpduGroupVector.c \
                      $(Com_CORE_PATH)/src/Com_SetIpduGroup.c \
                      $(Com_CORE_PATH)/src/Com_MainFunctionRx.c \
                      $(Com_CORE_PATH)/src/Com_MainFunctionTx.c \
                      $(Com_CORE_PATH)/src/Com_ReceiveShadowSignal.c \
                      $(Com_CORE_PATH)/src/Com_ReceiveSignal.c \
                      $(Com_CORE_PATH)/src/Com_ReceiveSignalGroup.c \
                      $(Com_CORE_PATH)/src/Com_RxIndication.c \
                      $(Com_CORE_PATH)/src/Com_SendSignal.c \
                      $(Com_CORE_PATH)/src/Com_SendSignalGroup.c \
                      $(Com_CORE_PATH)/src/Com_SwitchIpduTxMode.c \
                      $(Com_CORE_PATH)/src/Com_TriggerIPDUSend.c \
                      $(Com_CORE_PATH)/src/Com_TriggerTransmit.c \
                      $(Com_CORE_PATH)/src/Com_TxConfirmation.c \
                      $(Com_CORE_PATH)/src/Com_UpdateShadowSignal.c \
                      $(Com_CORE_PATH)/src/Com_GetVersionInfo.c \
                      $(Com_CORE_PATH)/src/Com_Filter.c \
                      $(Com_CORE_PATH)/src/Com_Core_Read.c \
                      $(Com_CORE_PATH)/src/Com_Core_Write.c \
                      $(Com_CORE_PATH)/src/Com_CopyTxData.c \
                      $(Com_CORE_PATH)/src/Com_CopyRxData.c \
                      $(Com_CORE_PATH)/src/Com_StartOfReception.c \


Com_lib_FILES_TransmissionMode      =  \
                      $(Com_CORE_PATH)/src/Com_Core_TM.c

Com_lib_FILES_SigGW      =  \
                      $(Com_CORE_PATH)/src/Com_MainFunctionRouteSignals.c \


Com_lib_FILES_UpdateBit      =  \
                      $(Com_CORE_PATH)/src/Com_Core_RxUpdateBit.c

Com_gen_API_FILES    = $(Com_OUTPUT_PATH)/src/Com_GenApi.c

Com_linkconfig_FILES   = $(Com_OUTPUT_PATH)/src/Com_Lcfg.c

Com_src_FILES      += $(Com_gen_API_FILES)
Com_src_FILES      += $(Com_linkconfig_FILES)

ifeq ($(TS_COM_MERGED_COMPILE),TRUE)

Com_src_FILES +=                             \
    $(Com_CORE_PATH)/src/Com_Merged.c        \

else

Com_src_FILES      += $(Com_lib_FILES_Core_PreCompile)
Com_src_FILES      += $(Com_lib_FILES_TransmissionMode)
Com_src_FILES      += $(Com_lib_FILES_SigGW)
Com_src_FILES      += $(Com_lib_FILES_UpdateBit)
Com_src_FILES      += $(Com_lib_FILES_Core)


endif

LIBRARIES_TO_BUILD     += Com_src

# Fill the list with post build configuration files needed to build the post build binary.
Com_pbconfig_FILES := $(wildcard $(Com_OUTPUT_PATH)/src/Com_PBcfg.c)

# only add Com_pbconfig to the libraries to build if there are really post-build config files
# present
ifneq ($(strip $(Com_pbconfig_FILES)),)
LIBRARIES_PBCFG_TO_BUILD += Com_pbconfig
LIBRARIES_TO_BUILD += Com_pbconfig
endif

define defineComLibOutputPATH
$(1)_OBJ_OUTPUT_PATH    := $(Com_lib_LIB_OUTPUT_PATH)
endef
$(foreach SRC,$(basename $(notdir $(subst \,/,$(Com_lib_FILES)))),$(eval $(call defineComLibOutputPATH,$(SRC))))

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES

