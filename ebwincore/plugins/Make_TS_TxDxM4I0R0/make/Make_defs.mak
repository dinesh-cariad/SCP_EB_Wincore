#
# \file
#
# \brief AUTOSAR Make
#
# This file contains the implementation of the AUTOSAR
# module Make.
#
# \version 4.0.27
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.

ifndef (_MAKE_DEFS)
_MAKE_DEFS=1

# set shell for Make - unix is fine, it's user's choice or Windows that need to be taken care of ...
ifeq (,$(USERBUILD_SHELL))
  # look what's there
  ifeq ($(OS),Windows_NT)
    USERBUILD_SHELL:=$(COMSPEC)
  endif
endif
ifneq (,$(USERBUILD_SHELL))
  SHELL := $(subst \,/,$(USERBUILD_SHELL))
#  export USERBUILD_SHELL
  MAKEOVERRIDES+=SHELL=$(USERBUILD_SHELL)
endif
# flag if Make is running on Windows, check if shell is cmd.exe
# (the make variable OS is overwritten by included plugin makefiles)
IS_DOS_SHELL := $(findstring cmd.exe,$(SHELL))
ifneq (,$(IS_DOS_SHELL))
  .SHELLFLAGS= /x /c
endif
# verify Make version
ifneq (4.0,$(firstword $(sort 4.0 $(MAKE_VERSION))))
  $(error make version $(MAKE_VERSION) detected! expected Make version >= 4.0)
endif
# check if realpath works
ifeq (,$(realpath $(wildcard ../*)))
  $(error realpath returned empty string! ensure make in version at least 3.82 is installed)
endif
EB_VERBOSE ?= @
ifneq (,$(EB_VERBOSE))
  MAKEFLAGS+= -s
  override EB_VERBOSE:=
  log_verbose=
else
  log_verbose=$(info # $(CURDIR): $1)
endif
MAKEOVERRIDES+=EB_VERBOSE=$(EB_VERBOSE)

# sanitize input paths (backslashes are evil -> quoting hell)
# TRESOS_BASE and PROJECT_ROOT are already sanitized by common.mak - but did we already fix all overrides, yet?
override PROJECT_ROOT		:= $(subst \,/,$(PROJECT_ROOT))
override TRESOS_BASE		:= $(subst \,/,$(TRESOS_BASE))
override PLUGINS_BASE		:= $(subst \,/,$(PLUGINS_BASE))
override GEN_OUTPUT_PATH	:= $(subst \,/,$(GEN_OUTPUT_PATH))
override TRESOS2_OUTPUT_PATH	:= $(GEN_OUTPUT_PATH)
# empty for WIN32X86 & redefined by WIN32X86/Compiler_defs.mak
ifneq (,$(findstring \,$(TOOLPATH_COMPILER)))
  TOOLPATH_COMPILER	:= $(subst \,/,$(TOOLPATH_COMPILER))
endif

# sensible default. Also used as uniq identifier like $$ in bash.
PROJECT_NAME?= $(lastword $(subst /, ,$(PROJECT_ROOT)))

# legacy aliases
SSC_ROOT?= $(PLUGINS_BASE)

# macros for special characters
EMPTY	:=
BLANK	:= $(EMPTY) $(EMPTY)
SPACE	:= $(BLANK)
COMMA	:= ,
SEMICOL	:= ;
LBRACK	:= (
RBRACK	:= )
DOLLAR	:= $$
HASH	:= \#
# macro for newline, use as $(\n) within make functions to insert a line break
define \n


endef

# macro for tabulator, use as $(\t) within make functions to insert a tab
define \t
	$(EMPTY)
endef

# don't evaluate CC_FILES_TO_BUILD -> $(wildcard caching problem for generated files

# include configuration makefile
-include $(PROJECT_ROOT)/util/$(TARGET)_$(DERIVATE)_$(TOOLCHAIN)_cfg.mak

#################################################################
# Define the suffix for all files which implement the interfaces:
# Compiler Makefile Interface
# Board Makefile Interface
# Basic Software Makefile Interface
# Utility Makefile Interface
MAK_FILE_SUFFIX		?= mak
DEFS_FILE_SUFFIX	?= _defs.$(MAK_FILE_SUFFIX)
RULES_FILE_SUFFIX	?= _rules.$(MAK_FILE_SUFFIX)
CHECK_FILE_SUFFIX	?= _check.$(MAK_FILE_SUFFIX)
CFG_FILE_SUFFIX		?= _cfg.$(MAK_FILE_SUFFIX)

# one of the few paths in backslash-notation, because it's solely used to execute the tresos_cmd.bat
TRESOS_BIN		:= $(subst /,\,$(abspath $(subst \,/,$(TRESOS_BASE)/bin)))
# we need 'override' here or we couldn't sanitize backslashes passed on the make command line
override PLUGINS_BASE	:= $(abspath $(subst \,/,$(PLUGINS_BASE)))
override FORWARD_PLUGINS_BASE = $(error Use of obsolete variable FORWARD_PLUGINS_BASE - use PLUGINS_BASE instead)
MAKE_ROOT		:= $(PLUGINS_BASE)/Make_$(Make_VARIANT)
BOARD_PROJECT_PATH	?= $(PROJECT_ROOT)/source/boards/$(BOARD)

# ***  tool path tests
# check the tool paths for existence - no use starting a build with any of these missing
$(foreach p,PROJECT_ROOT TRESOS_BASE TRESOS_BIN MAKE_ROOT\
	,$(if $(realpath $(subst \,/,$($(p)))),,$(error path not found: $(p)=$($(p)))))

# ------------  tools  --------------------------------------------------------------
# include GNU Make standard library
include $(MAKE_ROOT)/tools/gmsl/gmsl

# helpers to avoid 'undefined variable' warnings
# parameter 1: the variable to be used, if defined
# parameter 2:  the default value to use if undefined: may be another $(call optional,fallback-variable)
optional=$(if $(findstring undefined,$(origin $(1))),$(2),$($(1)))

# original gmsl implementation using single-element divide & conquer using tail-recursion
_uniq1 = $(if $1,$(firstword $1) $(call _uniq1,$(filter-out $(firstword $1),$1)))

# emit & reduce
# parameter 1: original list
# parameter 2: de-duplicated sublist
_reduce = $(if $2,$2$(call _split,$(filter-out $2,$1)))

# split the next batch, make unique and hand to reduce stage
# parameter 1: the remaining list
_split = $(if $1,$(call _reduce,$1,$(call _uniq1,$(wordlist 1,20,$1))))

# less stack-hungry implementation than gmsl using batch-subprocessing and 'unrolling' by a factor of 20
uniq = $(strip $(call _split,$1))

# define variables as shortcuts for tools
TRACE32_DIR		?= $(TOOLS_BASE)/T32
TRACE32_BIN		?= $(TRACE32_DIR)/bin/windows/t32mppc.exe
TRACE32_CONFIG		?= $(EOCM_PATH)/EOCM_Base/config.t32

# std unix tools & respective DOS/Windows ports + a few macros & functions form a GNU tools abstraction layer
# fixCmdPaths: IF-conversion: do the correct path separator fixup for DOS/cmd.exe & linux/bash
# handles lists of paths and even full command lines, but doesn't handle commas in the input, unless quoted as $(COMMA)
ifeq (,$(IS_DOS_SHELL))
  fixCmdPaths		= $(subst \,/,$(1))
  ifexist		= [[ ! -e '$1' ]] || $2
  ifnotexist		= [[ -e '$1' ]] || $2
  DEVNULL		:= /dev/null
  STATUS		?= $$?
  MKDIR			?= mkdir -p
  RM			?= rm -f
  MV			?= mv
  COPY			?= cp
  CP			?= cp
  RMTREE		?= rm -Rf
  RMDIR			?= rm -Rf
  DOS2UNIX		?= dos2unix
  PERL_EXE		?= perl
  PERL			?= perl
  ECHO			?= echo
  WHICH			?= which
  CPP			?= cpp
else
  fixCmdPaths		= $(subst /,\,$(1))
  ifexist		= if exist "$(subst /,\,$1)" $2
  ifnotexist		= if not exist "$(subst /,\,$1)" $2
  DEVNULL		:= nul:
  STATUS		:= %ERRORLEVEL%
  # gnu & unix tools compiled for DOS can take forward-slash paths and can take unexpanded wildcards, but cmd.exe needs a backslash path to find them
  _MAKE_TOOLS		 = $(subst /,\,$(MAKE_ROOT))\tools
  _SFK_EXE		 = $(_MAKE_TOOLS)\sfk\sfk.exe
  _MKDIR_EXE		 = $(_MAKE_TOOLS)\GNU_Utils\mkdir.exe
  MKDIR			:= $(_MKDIR_EXE) -p
  _RM_EXE		 = $(_MAKE_TOOLS)\GNU_Utils\rm.exe
  RM			:= $(_RM_EXE) -f
  _MV_EXE		 = $(_MAKE_TOOLS)\GNU_Utils\mv.exe
  _MOVE_EXE		 = $(_MV_EXE)
  MV			:= $(_MV_EXE) -f
  MOVE			:= $(MV)
  _CP_EXE		 = $(_MAKE_TOOLS)\GNU_Utils\cp.exe
  CP			:= $(_CP_EXE)
# DEPRECATED
  # we use Windows/DOS copy because if you pass a "path/*.ext" parameter
  # to regular tools an invalid "path*.ext" pattern would be forwarded.
  # ... to be fixed
  COPY			:= copy /y
  RMTREE		:= $(_RM_EXE) -Rf
  # deprecated - use RMTREE instead!
  RMDIR			:= rmdir /s /q
  _DOS2UNIX_EXE		 = $(_MAKE_TOOLS)\dos2unix\dos2unix.exe
  DOS2UNIX		:= $(_DOS2UNIX_EXE) -q
  _PERL_EXE		 = $(_MAKE_TOOLS)\strawberryperl\perl.exe
  PERL			 = $(_PERL_EXE) -I$(MAKE_ROOT)/tools/strawberryperl
  PERL_EXE		 = $(PERL)
  ECHO			:= $(_SFK_EXE) echo -spat
  WHICH			:= $(_SFK_EXE) pathfind
  _CPP_EXE		 = $(_MAKE_TOOLS)\gcc\bin\cpp.exe
  CPP			:= $(_CPP_EXE)

  # ***  tool path tests
  # check the tool paths for existence - no use starting a build with any of these missing
  $(foreach p,_MAKE_TOOLS _RM_EXE _MV_EXE _CP_EXE _MKDIR_EXE _SFK_EXE _DOS2UNIX_EXE _PERL_EXE\
  	,$(if $(realpath $(subst \,/,$($(p)))),,$(error path not found: $(p)=$($(p)))))
endif

CHECK_DUPS		:= $(MAKE_ROOT)/tools/scripts/check_dups.pl
CPP_DEPS		:= $(MAKE_ROOT)/tools/scripts/cppDeps.pl

# commandline compression:
# cd to that directory and strip that directory prefix from all paths
# allows use of relative and absolute paths
# cd is an internal command in all shells, so doesn't add to execution time
# && is recognized by bash & cmd.exe
# used with fixCmdPaths, this is shell-agnostic
# Note: for the replacement to work, the directory has to include the suitable trailing slash
# This is not required, if only relative paths are used in the actual command
# as always, commas that need to be passed WITHIN an argument must be passed as $(COMMA)
runIn=cd $(call fixCmdPaths,$(1)) && $(subst $(1),,$(2))

################################################################
# helper to execute a command in a foreach loop or recursively
# note: the 3rd line (empty) is really needed to serve as a cmd-separator.
#
# args: 1 - cmd(s) to perform
define SC_CMD_AR_HELPER
	$(1)

endef
define SC_CMD_AR2_HELPER
	$(1)
	$(2)

endef

################################################################
# recursive helper to perform a task on chunks of a list
# note: do not place a space in front of argument 5 of the recursive call,
#       ( $(4),<here>$(wordlist... )). This would result in infinite recursion.
#
# args: 1 - variable-name that contains commands to perform
#       2 - additional arguments to $(1)
#       3 - max. number of elements in a chunk
#       4 - obsolete - was "$3+1 (sorry, cannot perform arith in make)"
#       5 - list
SC_CMD_LISTPERFORM = $(if $(5),\
  $(call $(1),$(2) $(wordlist 1,$(3),$(5)))\
  $(call SC_CMD_LISTPERFORM,$(1),$(2),$(3),_,$(filter-out $(wordlist 1,$(3),$(5)),$(5))))

# ------------  Project Directories  ---------------------------------------------------
# define paths relative to PROJECT_ROOT
# they help with DOS/Win/CygWin/linux agnostic code and small DOS commandline
REL_OUT_DIR			:= output
REL_BIN_DIR			:= $(REL_OUT_DIR)/bin
REL_DEP_DIR			:= $(REL_OUT_DIR)/depend
REL_GEN_DIR			:= $(REL_OUT_DIR)/generated
REL_LIB_DIR			:= $(REL_OUT_DIR)/lib
REL_MAKE_DIR			:= $(REL_OUT_DIR)/make
REL_OBJ_DIR			:= $(REL_OUT_DIR)/obj
REL_LIST_DIR			:= $(REL_OUT_DIR)/list
REL_LOG_DIR			:= $(REL_OUT_DIR)/temp

# now define absolute paths
OUTPUT_DIR			:= $(PROJECT_ROOT)/$(REL_OUT_DIR)
BIN_OUTPUT_PATH			:= $(PROJECT_ROOT)/$(REL_BIN_DIR)
DEPEND_DIR			:= $(PROJECT_ROOT)/$(REL_DEP_DIR)
LIB_DIR				:= $(PROJECT_ROOT)/$(REL_LIB_DIR)
MK_TMP_DIR			:= $(PROJECT_ROOT)/$(REL_MAKE_DIR)
OBJ_DIR				:= $(PROJECT_ROOT)/$(REL_OBJ_DIR)
LIST_DIR			:= $(PROJECT_ROOT)/$(REL_LIST_DIR)
LOG_DIR				:= $(PROJECT_ROOT)/$(REL_LOG_DIR)

OBJ_OUTPUT_PATH			:= $(OBJ_DIR)
GENERATED_DIR			 = $(subst \,/,$(GEN_OUTPUT_PATH))
GEN_OUTPUT_PATHS		?= $(GENERATED_DIR)
override GEN_OUTPUT_PATHS	:= $(subst \,/,$(GEN_OUTPUT_PATHS))
AUTOSAR_BASE_OUTPUT_PATH	?= $(OUTPUT_DIR)/generated
override AUTOSAR_BASE_OUTPUT_PATH	:= $(subst \,/,$(AUTOSAR_BASE_OUTPUT_PATH))

# add generated library directory to list of library include paths
LIB_INCLUDE_PATH		+= $(LIB_DIR)

# ------------  compiler  --------------------------------------------------------------
MAKE_INC_DEFAULT		 = $(MK_TMP_DIR)/compiler.inc
MAKE_INC			 = $(if $($(basename $(notdir $@))_OPTMAP),$($(basename $(notdir $@))_OPTMAP),$(MAKE_INC_DEFAULT))
MAKE_INCPP			 = $(MK_TMP_DIR)/compiler.incpp
ASM_INC				 = $(MK_TMP_DIR)/assembler.inc
DEPEND_INC			 = $(MK_TMP_DIR)/depend.inc
LNK_INC				 = $(MK_TMP_DIR)/lnk.inc
LIB_TO_OBJ			 = $(MK_TMP_DIR)/lib_to_obj.mak
OBJ_TO_CC			 = $(MK_TMP_DIR)/obj_to_cc.mak
PREP_TO_CC			 = $(MK_TMP_DIR)/prep_to_cc.mak
MAKE_INFO_FILE			 = $(MK_TMP_DIR)/make.info
PLUGIN_INC			 = $(MK_TMP_DIR)/plugins.mak
OPT_MAP				 = $(MK_TMP_DIR)/opt.map
SRC_LIST			 = $(MK_TMP_DIR)/src_list.txt
HDR_LIST			 = $(MK_TMP_DIR)/hdr_list.txt
GLOBAL_LIB_DIR_NAME		:= global_lib_dir.$(TOOLCHAIN)
GLOBAL_LIB_DIR			?= $(PLUGINS_BASE)/$(GLOBAL_LIB_DIR_NAME)
GLOBAL_LIB_DIR			:= $(subst \,/,$(GLOBAL_LIB_DIR))

# define suffixes if not overridden by Compiler_defs.mak
ASM_FILE_SUFFIX			?= asm
CPP_FILE_SUFFIX			?= cpp
CC_FILE_SUFFIX			?= c
OBJ_FILE_SUFFIX			?= o
LIB_FILE_SUFFIX			?= a
PREP_FILE_SUFFIX		?= p
DEP_FILE_SUFFIX			?= d

#.SUFFIXES:
.SUFFIXES: .$(CC_FILE_SUFFIX) .$(CPP_FILE_SUFFIX) .$(PREP_FILE_SUFFIX) .$(ASM_FILE_SUFFIX) .$(OBJ_FILE_SUFFIX) .$(LIB_FILE_SUFFIX) .$(BIN_FILE_SUFFIX)

CC_FILTER_LIST			?= %.$(CC_FILE_SUFFIX)
ASM_FILTER_LIST			?= %.$(ASM_FILE_SUFFIX)
CPP_FILTER_LIST			?= %.$(CPP_FILE_SUFFIX)

# generate preprocessor defines from key-value-pairs
cppGetPreProcessorDefines	 = $(foreach def,$(PREPROCESSOR_DEFINES),-D$($(def)_KEY)=$(subst \,/,$($(def)_VALUE)))

# this is for handling the TS_BUILD_POST_BUILD_BINARY=TRUE commandline trigger
# normally you can just type "make postbuild" but this does not (yet) work for the legacy test cases
ifneq (,$(filter TRUE,$(TS_BUILD_POST_BUILD_BINARY))$(filter postbuild,$(MAKECMDGOALS)))
  ALL_PREREQUISITES		 = postbuild
else
  ALL_PREREQUISITES		 = $(PROJECT_BINARY) $(MAKE_INFO_FILE)
endif

# ------------  first rules  ------------------------------------------------------------
.PHONY: all run

ifneq (,$(filter all,$(MAKECMDGOALS)))
  ifneq (,$(filter run%,$(MAKECMDGOALS)))
$(filter run%,$(MAKECMDGOALS)): all

  endif
endif

endif # ifndef (_MAKE_DEFS)

