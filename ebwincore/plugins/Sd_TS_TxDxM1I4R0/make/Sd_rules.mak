#
# \file
#
# \brief AUTOSAR Sd
#
# This file contains the implementation of the AUTOSAR
# module Sd.
#
# \version 1.4.9
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

ifndef TS_SD_MERGED_COMPILE
TS_SD_MERGED_COMPILE := $(TS_MERGED_COMPILE)
endif

ifeq ($(TS_SD_MERGED_COMPILE),TRUE)

Sd_src_FILES +=                              \
          $(Sd_CORE_PATH)/src/Sd_Merged.c \

else

Sd_src_FILES_Receive := \
    $(Sd_CORE_PATH)/src/Sd_Receive.c \

Sd_src_FILES_Send := \
    $(Sd_CORE_PATH)/src/Sd_Send.c \

Sd_src_FILES_ServerService := \
    $(Sd_CORE_PATH)/src/Sd_ServerService.c \

Sd_src_FILES_ClientService := \
    $(Sd_CORE_PATH)/src/Sd_ClientService.c \
    
Sd_src_FILES := \
    $(Sd_CORE_PATH)/src/Sd.c \

Sd_Units ?= Receive Send ServerService ClientService
Sd_src_FILES += $(foreach unit,$(Sd_Units),$(Sd_src_FILES_$(unit)))

endif

Sd_src_FILES +=  $(Sd_OUTPUT_PATH)/src/Sd_Cfg.c

LIBRARIES_TO_BUILD     += Sd_src

# Fill the list with post build configuration files needed to build the post build binary.
Sd_pbconfig_FILES :=

ifneq ($(strip $(Sd_pbconfig_FILES)),)
LIBRARIES_PBCFG_TO_BUILD += Sd_pbconfig
LIBRARIES_TO_BUILD += Sd_pbconfig
endif

