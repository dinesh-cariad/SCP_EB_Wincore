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


ABS_FILE_SUFFIX   =elf
MAP_FILE_SUFFIX   =map
HEX_FILE_SUFFIX   =mot

PROJECT_BINARY  = $(BIN_OUTPUT_PATH)/$(PROJECT).$(ABS_FILE_SUFFIX)
MAPFILE         = $(BIN_OUTPUT_PATH)/$(PROJECT).$(MAP_FILE_SUFFIX)
HEXFILE         = $(BIN_OUTPUT_PATH)/$(PROJECT).$(HEX_FILE_SUFFIX)

# override globale make default suffixes
ASM_FILE_SUFFIX   =asm

# ------ general files (used in common rules)
ifneq (,$(findstring diab-7.0.2.0,$(TOOLPATH_COMPILER)))
  COMPILER_DIR    = $(TOOLPATH_COMPILER)/WIN64/bin
else
  COMPILER_DIR    = $(TOOLPATH_COMPILER)/Win32/bin
endif
CC              = $(COMPILER_DIR)/dcc.exe
CCLIB           = $(COMPILER_DIR)/dar.exe
ASM             = $(COMPILER_DIR)/dcc.exe
LINK            = $(COMPILER_DIR)/dld.exe
HEX             = $(COMPILER_DIR)/ddump.exe
GCC             = $(PLUGINS_BASE)/Make_$(Make_VARIANT)/tools/gcc/bin/cpp.exe
