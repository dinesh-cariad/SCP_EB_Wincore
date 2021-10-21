#
# \file
#
# \brief AUTOSAR SoAd
#
# This file contains the implementation of the AUTOSAR
# module SoAd.
#
# \version 1.8.16
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

ifndef TS_SOAD_MERGED_COMPILE
TS_SOAD_MERGED_COMPILE := $(TS_MERGED_COMPILE)
endif

ifeq ($(TS_SOAD_MERGED_COMPILE),TRUE)

SoAd_src_FILES +=                              \
          $(SoAd_CORE_PATH)/src/SoAd_Merged.c \

else

SoAd_src_FILES += \
    $(SoAd_CORE_PATH)/src/SoAd_02_SocketCon.c \
    $(SoAd_CORE_PATH)/src/SoAd_03_Lib.c \
    $(SoAd_CORE_PATH)/src/SoAd_Int_05_TxTp.c \
    $(SoAd_CORE_PATH)/src/SoAd_Int_06_TxIf.c \
    $(SoAd_CORE_PATH)/src/SoAd_Int_07_Rx.c \
    $(SoAd_CORE_PATH)/src/SoAd_Int_08_MainFunction.c \

SoAd_src_FILES_Udp := \
    $(SoAd_CORE_PATH)/src/SoAd_01_UdpSM.c \
    $(SoAd_CORE_PATH)/src/SoAd_01_UdpSM_User.c \

SoAd_src_FILES_Tcp := \
    $(SoAd_CORE_PATH)/src/SoAd_04_TcpSM.c \
    $(SoAd_CORE_PATH)/src/SoAd_04_TcpSM_User.c \


SoAd_Units ?=  Udp Tcp
SoAd_src_FILES += $(foreach unit,$(SoAd_Units),$(SoAd_src_FILES_$(unit)))

endif

SoAd_src_FILES +=  $(SoAd_OUTPUT_PATH)/src/SoAd_Lcfg.c

LIBRARIES_TO_BUILD   += SoAd_src

# Fill the list with post build configuration files needed to build the post build binary.
SoAd_pbconfig_FILES := $(wildcard $(SoAd_OUTPUT_PATH)/src/SoAd_PBcfg.c)

ifneq ($(strip $(SoAd_pbconfig_FILES)),)
LIBRARIES_PBCFG_TO_BUILD += SoAd_pbconfig
LIBRARIES_TO_BUILD += SoAd_pbconfig
endif


#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
