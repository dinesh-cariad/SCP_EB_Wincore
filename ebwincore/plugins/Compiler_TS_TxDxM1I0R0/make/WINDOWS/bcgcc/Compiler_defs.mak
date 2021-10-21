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


PROJECT_BINARY  = $(BIN_OUTPUT_PATH)/$(PROJECT).exe
FIRST_BUILD_TARGET := $(PROJECT_BINARY)
MAPFILE         = $(BIN_OUTPUT_PATH)/$(PROJECT).map

CC              = $(TOOLPATH_COMPILER)/bin/bcgcc.exe
CCLIB           = $(TOOLPATH_COMPILER)/bin/ar.exe
LINK            = $(TOOLPATH_COMPILER)/bin/bcgcc.exe
ASM             = $(TOOLPATH_COMPILER)/bin/bcgcc.exe
GCC             = $(MAKE_ROOT)/tools/gcc/bin/cpp.exe
COMPILER_DIR    = $(TOOLPATH_COMPILER)
# bcgcc compiler is located in /usr/local of cygwin,
# but uses objcopy and debugger of gcc which is located in /bin
OBJCOPY        ?= $(TOOLPATH_COMPILER)/../../bin/objcopy.exe
DEBUGGER       ?= $(TOOLPATH_COMPILER)/../../bin/gdb.exe

ifeq (,$(realpath $(GCC)))
  $(error FILE DOES NOT EXIST GCC $(GCC))
endif