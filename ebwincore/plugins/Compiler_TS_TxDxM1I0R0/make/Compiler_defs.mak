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


##################################################################
# CONFIGURATION
Compiler_CORE_PATH           ?= $(PLUGINS_BASE)/Compiler_$(Compiler_VARIANT)
Compiler_OUTPUT_PATH         ?= $(AUTOSAR_BASE_OUTPUT_PATH)
Compiler_lib_LIB_OUTPUT_PATH := $(Compiler_CORE_PATH)/lib

#################################################################
# REGISTRY


# include architecture-specific Compiler_defs makefile
include $(Compiler_CORE_PATH)/make/$(TARGET)/Compiler$(DEFS_FILE_SUFFIX)

# add architecture- and toolchain-specific header files 
ifeq ($(TARGET),WINDOWS)
  CC_INCLUDE_PATH      += $(Compiler_CORE_PATH)/include/$(TARGET)
else
  CC_INCLUDE_PATH      += $(Compiler_CORE_PATH)/include
  CC_INCLUDE_PATH      += $(Compiler_CORE_PATH)/include/$(TARGET)/$(TOOLCHAIN)
endif

#################################################################
# TRACE32_PATH (default)
# Path to the Windows Trace32 Debugger

TRACE32_PATH ?= C:/tools/T32
# old T32
#TRACE32_APP_PATH ?= $(TRACE32_PATH)
# new T32 (which runs on e.g. windows and linux) has a different path 
TRACE32_APP_PATH ?= $(TRACE32_PATH)/bin/windows64


#####################################################################
# debug : starts BUILD_ALL and in case of success starts the TRACE32
# from Lauterbach to debug the application.
# syntax of command line T32 format is:
#   t32m<arch>[.<x>] [<options>] [-c <configfile> [<c_args>]] [-s <startup_script> [<s_args>]]
# more information: http://www2.lauterbach.com/pdf/debugger_user.pdf

# DEBUG_CFG_FILE = -c <configfile> [<c_args>]
DEBUG_CFG_FILE ?= -c $(TRACE32_PATH)/config.t32
# DEBUG_OPT_FILE = -s <startup_script> [<s_args>]
DEBUG_OPT_FILE ?= -s $(BOARD_PROJECT_PATH)/$(DERIVATE).cmm ../output/bin/$(PROJECT).$(ABS_FILE_SUFFIX)


#################################################################
# checks if compiler was set correctly
ifeq (,$(PROJECT_BINARY))
  $(error No PROJECT_BINARY defined, check Compiler_defs.mak in Compiler_$(Compiler_VARIANT) \
  or verify that plugin Compiler_$(Compiler_VARIANT) is compatible with plugin Make_$(Make_VARIANT))
endif

ifeq (,$(realpath $(subst \,/,$(CC))))
  $(error FILE DOES NOT EXIST CC $(CC))
endif
ifeq (,$(realpath $(subst \,/,$(CCLIB))))
  $(error FILE DOES NOT EXIST CCLIB $(CCLIB))
endif
ifeq (,$(realpath $(subst \,/,$(LINK))))
  $(error FILE DOES NOT EXIST LINK $(LINK))
endif
ifeq (,$(realpath $(subst \,/,$(COMPILER_DIR))))
  $(error DIRECTORY DOES NOT EXIST COMPILER_DIR $(COMPILER_DIR))
endif
