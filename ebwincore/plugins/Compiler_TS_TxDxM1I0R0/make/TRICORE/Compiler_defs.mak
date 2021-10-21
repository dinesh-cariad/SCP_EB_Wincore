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
# Name of the debugger (initialization values)
DB ?= $(TRACE32_APP_PATH)/t32mtc.exe 

#################################################################
# PROJECT_BINARY: path to the executable
#
# Unlike FIRST_BUILD_TARGET, this is the .elf file
PROJECT_BINARY = $(BIN_OUTPUT_PATH)/$(PROJECT).$(ABS_FILE_SUFFIX)

