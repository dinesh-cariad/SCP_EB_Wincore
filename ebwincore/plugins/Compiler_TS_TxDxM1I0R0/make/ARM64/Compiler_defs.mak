#
# \file
#
# \brief AUTOSAR Compiler
#
# This file contains the implementation of the AUTOSAR
# module Compiler.
#
# \version 1.0.8
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2017 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# this file just references the compiler_defs.mak makfiles
# in the TOOLCHAIN directory
include $(Compiler_CORE_PATH)/make/$(TARGET)/$(TOOLCHAIN)/Compiler$(DEFS_FILE_SUFFIX)

#################################################################
ifeq ($(DERIVATE),$(filter $(DERIVATE), S32V234 S32V234BE))
CONFIG_T32            = $(PROJECT_ROOT)/output/make/config.t32
CONFIG_T32_GENERATOR  = $(TRESOS_BASE)/plugins/Platforms_$(Platforms_VARIANT)/tools/PerlScripts/config32_generator.pl 
TRACE32_IMAGE         = $(BOARD_PROJECT_PATH)/binary.elf

#$(TRACE32_IMAGE):
#	CMD /C $(BOARD_PROJECT_PATH)/build.bat NOPAUSE

$(CONFIG_T32):
	$(EB_VERBOSE)$(PERL) $(CONFIG_T32_GENERATOR) $(TRACE32_PATH) > $(CONFIG_T32)

# Name of the debugger (initialization values)
DB_ARM32 ?= $(TRACE32_APP_PATH)/t32marm.exe -c $(CONFIG_T32)
DB_ARM64 ?= $(TRACE32_APP_PATH)/t32marm64.exe -c $(CONFIG_T32)

DEBUG_ARM32_OPT_FILE ?= $(BOARD_PROJECT_PATH)/$(DERIVATE)-CM4.cmm $(TRACE32_IMAGE)
DEBUG_ARM64_OPT_FILE ?= $(BOARD_PROJECT_PATH)/$(DERIVATE)-CA53.cmm $(PROJECT_ROOT)/output/bin/$(PROJECT).$(ABS_FILE_SUFFIX)

MAKE_INFO += DEBUG_SCRIPT_ARM32="$(BOARD_PROJECT_PATH)/$(DERIVATE)-CM4.cmm"
MAKE_INFO += DEBUG_SCRIPT_ARM64="$(BOARD_PROJECT_PATH)/$(DERIVATE)-CA53.cmm"

else
DB ?= $(TRACE32_APP_PATH)/t32marm64.exe
endif

#################################################################
# PROJECT_BINARY: path to the executable
#
# If not set, assume it to be identical to FIRST_BUILD_TARGET 
PROJECT_BINARY ?= $(FIRST_BUILD_TARGET)

