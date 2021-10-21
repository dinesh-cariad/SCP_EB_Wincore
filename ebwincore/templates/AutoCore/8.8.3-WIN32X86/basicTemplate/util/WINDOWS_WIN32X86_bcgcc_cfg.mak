#
# \file
#
# \brief AUTOSAR ApplTemplates
#
# This file contains the implementation of the AUTOSAR
# module ApplTemplates.
#
# \version 6.8.0
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# Toolchain settings

#################################################################
# TOOLPATH_COMPILER:
# Path to the cygwin compiler
ifneq ($(AC_CYGWIN_DOS),)
TOOLPATH_COMPILER ?= $(AC_CYGWIN_DOS)\usr\local
else
TOOLPATH_COMPILER       ?= C:\cygwin\usr\local
endif

#################################################################
# WIN32_LIB_PATH:
# Path to the win32 library for this compiler
# Usually set to the lib path of the compiler, for example
# $(TOOLPATH_COMPILER)\..\lib
#
# The library is used for instance by MCAL modules for Windows
# platform
WIN32_LIB_PATH ?= $(TOOLPATH_COMPILER)\lib\w32api

#################################################################
# OPT_LEVEL:
# Optimization level
# By default the compiler optimizes for size
# The optimization level is treated special as it is often
# necessary to disable optimization for easier debugging.
# NOTE: Optimization switched off due to possible compiler bugs
OPT_LEVEL ?= -O0


ifndef CC_OPT
#################################################################
# Define the options for the compiler

##### Options controlling the kind of output

# Compile or assemble the source files, but do not link
CC_OPT += -c

# Use pipes rather than temporary files
CC_OPT += -pipe

# pass all the source files to the compiler at once
# deactivated, but very interesting for the future,
# one compiler call for _all_ c files
# enables checks across translation units
# CC_OPT += -combine

##### C standard options

# ISO C90 as modified in amendment 1
CC_OPT += -std=iso9899:199409
# Assert that compilation takes place in a freestanding environment
CC_OPT += -ffreestanding

##### Options to Request or Suppress Warnings

# Issue all the warnings demanded by strict ISO C
# reject all programs that use forbidden extensions
CC_OPT += -pedantic

# This enables all the warnings about constructions that some users
# consider questionable
CC_OPT += -Wall

# This enables some extra warning flags that are not enabled by -Wall
CC_OPT += -Wextra

# Warn about uninitialized variables which are initialized with themselves
# hint: works only when -Wuninitialized is used, which is activated
#   with -Wall or -Wextra
CC_OPT += -Winit-self

# Warn if a user-supplied include directory does not exist
# Disabled: the application currently has the USER_INCLUDE_DIRS
#   defined which includes non-existent directories -> too much noise.
# CC_OPT += -Wmissing-include-dirs

# Warn whenever a switch statement does not have a default case.
CC_OPT += -Wswitch-default

# Warn whenever a switch statement has an index of enumerated type and
# lacks a case for one or more of the named codes of that enumeration.
# case labels outside the enumeration range also provoke warnings when
# this option is used.
# Disabled: this warning is also thrown if the switch statement has
#  a default branch -> produces lots of noise.
# CC_OPT += -Wswitch-enum

# Warn whenever a function parameter is unused aside from its declaration
CC_OPT += -Wunused-parameter

# Warn when a #pragma directive is encountered which is not understood by GCC
CC_OPT += -Wunknown-pragmas

# Warn if floating point values are used in equality comparisons
CC_OPT += -Wfloat-equal

# Warn when a declaration is found after a statement in a block
CC_OPT += -Wdeclaration-after-statement

# Warn if an undefined identifier is evaluated in an #if directive
CC_OPT += -Wundef

# Do not warn whenever an #else or an #endif are followed by text.
CC_OPT += -Wno-endif-labels

# arn whenever a local variable shadows another local variable, parameter
# or global variable or whenever a built-in function is shadowed
CC_OPT += -Wshadow

# Warn whenever a function call is cast to a non-matching type
CC_OPT += -Wbad-function-cast

# Warn about ISO C constructs that are outside of the common subset of
# ISO C and ISO C++
# Disabled: the only thing reported using this options are implicit conversions
#  from 'void *' to 'xy_type *', which are allowed in C-90 and not anymore
#  in ISO-C++.
#  However, introducing a vast array of extra casts makes code harder to
#  understand and besides that we don't really aim for C++ compatibility.
# CC_OPT += -Wc++-compat

# Warn whenever a pointer is cast so as to remove a type qualifier from
# the target type
#  disabled for bcgcc: this produces an insane amount of warnings and it seems
#    that the bounds patches did some serious problem in gcc
#CC_OPT += -Wcast-qual

# Warn whenever a pointer is cast such that the required alignment of the
# target is increased
CC_OPT += -Wcast-align

# When compiling C, give string constants the type const char[length] so
# that copying the address of one into a non-const char * pointer will get
#  a warning; when compiling C++, warn about the deprecated conversion from
# string literals to char *
CC_OPT += -Wwrite-strings

# Warn for implicit conversions that may alter a value
# disabled: causes lots of warnings if parameters are larger (e.g. int)
#   than the prototype of the function (e.g. uint8).
#   This also applies to e.g. constants and values.
# CC_OPT += -Wconversion

# Warn if a function is declared or defined without specifying the
# argument types
CC_OPT += -Wstrict-prototypes

# Warn if an old-style function definition is used. A warning is given
# even if there is a previous prototype
# Disabled: this only applies to function definitions where the prototype is
#   available and that have no parameters.
#   In the definition the 'void' parameter list has been forgotten.
# CC_OPT += -Wold-style-definition

# Warn if a global function is defined without a previous prototype declaration
CC_OPT += -Wmissing-prototypes

# Warn if a global function is defined without a previous declaration
CC_OPT += -Wmissing-declarations

# Warn if anything is declared more than once in the same scope, even in
# cases where multiple declaration is valid and changes nothing.
CC_OPT += -Wredundant-decls

# Warn if an extern declaration is encountered within a function
CC_OPT += -Wnested-externs

# Warn if the compiler detects that code will never be executed
# Disabled: following the decision for '-Wlogical-op', this is also disabled.
#   It nearly always warns if the configuration switches off code during
#   compile time.
# CC_OPT += -Wunreachable-code

# warns about expressions such as (!x | y) and (!x & y). Using explicit parentheses,
# such as in ((!x) | y), silences this warning
# remark:
#   brings it more inline with MISRA
CC_OPT += -Wparentheses

# inhibit warning about usage of long long as it is required for the uint64/sint64 types.
CC_OPT += -Wno-long-long

##### Options for Debugging Your Program or GCC

# Request debugging information and also use level to specify how much
# information
CC_OPT += -g

##### Options That Control Optimization

# Set optimization level
CC_OPT += $(OPT_LEVEL)

# Disable strict aliasing optimization
CC_OPT += -fno-strict-aliasing

##### Options Controlling the Preprocessor

CC_OPT += -DNOGDI -D_X86_ -D_WIN32X86_C_GCC_

##### Options for Code Generation Conventions

# This option generates traps for signed overflow on addition, subtraction,
# multiplication operations
# CC_OPT += -ftrapv

##### Options for the bounds checking patch

# enable bounds checking
CC_OPT += -fbounds-checking

endif

# On the test systems the STDOUT of the debugger is currently thrown
# away, so we need to write the output to a file.
CC_OPTS += -output-file=bounds_checking_gcc.log


##### other options

ifndef ASM_OPT
#################################################################
# Define the default options for the assembler
ASM_OPT                 += -c

endif


ifndef LINK_OPT
#################################################################
# Define the options for the linker
LINK_OPT                += -Wl,-Map,$(BIN_OUTPUT_PATH)\$(PROJECT).map

endif


#################################################################
# define output files
MAP_FILE                = $(BIN_DIR)\$(PROJECT).map
OUT_FILE                = $(BUILD_DIR)\$(PROJECT).out
DEBUG                   = NO
