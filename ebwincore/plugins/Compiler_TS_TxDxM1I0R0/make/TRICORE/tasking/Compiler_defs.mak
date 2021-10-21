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


ABS_FILE_SUFFIX=elf
ASM_FILE_SUFFIX=s
HEX_FILE_SUFFIX=mot

PROJECT_BINARY  = $(BIN_OUTPUT_PATH)/$(PROJECT).$(ABS_FILE_SUFFIX)
MAPFILE         = $(BIN_OUTPUT_PATH)/$(PROJECT).map
HEXFILE         = $(BIN_OUTPUT_PATH)/$(PROJECT).$(HEX_FILE_SUFFIX)

# ------ general files (used in common rules)
COMPILER_DIR    = $(TOOLPATH_COMPILER)/bin
CC              = $(COMPILER_DIR)/cctc.exe
CCLIB           = $(COMPILER_DIR)/artc.exe
ASM             = $(COMPILER_DIR)/astc.exe
LINK            = $(COMPILER_DIR)/ltc.exe
ifeq (,$(realpath $(CPP_DEPS)))
  GCC             = $(PLUGINS_BASE)/Make_$(Make_VARIANT)/tools/gcc/bin/cpp.exe
else
  GCC             = $(PERL) $(CPP_DEPS)
endif

# ------ compiler specific files (and the corresponding checks)
CPP             = $(COMPILER_DIR)/cptc.exe
CTC             = $(COMPILER_DIR)/ctc.exe

ifeq (,$(strip $(Compiler_CORE_PATH)))
  DEP_CONVERTER   = $(PLUGINS_BASE)/Compiler_$(Compiler_VARIANT)/make/$(TARGET)/$(TOOLCHAIN)/convert_tasking_dep_3.5.pl
else
  DEP_CONVERTER   = $(Compiler_CORE_PATH)/make/$(TARGET)/$(TOOLCHAIN)/convert_tasking_dep_3.5.pl
endif

ifeq (,$(strip $(Compiler_CORE_PATH)))
  GET_LICENSE_SCRIPT   = $(PLUGINS_BASE)/Compiler_$(Compiler_VARIANT)/make/$(TARGET)/$(TOOLCHAIN)/get_license.pl
else
  GET_LICENSE_SCRIPT   = $(Compiler_CORE_PATH)/make/$(TARGET)/$(TOOLCHAIN)/get_license.pl
endif

ifeq (,$(realpath $(CPP)))
  $(error FILE DOES NOT EXIST CPP $(CPP) $(realpath $(CPP)))
endif

ifeq (,$(realpath $(CTC)))
  $(error FILE DOES NOT EXIST CTC $(CTC))
endif

ifeq (,$(realpath $(DEP_CONVERTER)))
  $(error FILE DOES NOT EXIST DEP_CONVERTER $(DEP_CONVERTER))
endif

ifeq (,$(realpath $(GET_LICENSE_SCRIPT)))
  $(error FILE DOES NOT EXIST GET_LICENSE_SCRIPT $(GET_LICENSE_SCRIPT))
endif

ifeq (,$(realpath $(firstword $(PERL))))
  $(error FILE DOES NOT EXIST PERL $(firstword $(PERL)))
endif

# call the converter script every time make resolves it's variables
# in this way the tasking depend files are always converted before being
# included by make
CONVERTER_OUTPUT := $(shell $(PERL) $(DEP_CONVERTER) $(DEPEND_DIR) $(OBJ_DIR))

WAIT_FOR_LICENSE := $(shell $(PERL) $(GET_LICENSE_SCRIPT) $(COMPILER_DIR)/ctc.exe $(MAKECMDGOALS))

