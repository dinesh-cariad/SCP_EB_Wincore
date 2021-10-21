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
# Path to the MinGw compiler
# In case you don't want to use the compiler provided with the
# Platforms plugin set the path to the mingw compiler here
# TOOLPATH_COMPILER := C:\MinGw\bin

#################################################################
# WIN32_LIB_PATH:
# Path to the win32 library for this compiler
# Usually set to the lib path of the compiler, for example
# $(TOOLPATH_COMPILER)\..\lib
#
# The library is used for instance by MCAL modules for Windows
# platform
WIN32_LIB_PATH ?= $(TOOLPATH_COMPILER)\..\lib

#################################################################
# OPT_LEVEL:
# Optimization level
# By default the compiler optimizes for size
# The optimization level is treated special as it is often
# necessary to disable optimization for easier debugging.
COMPILE_FOR_DEBUG       ?= NO
COMPILE_FOR_PROFILE     ?= NO

ifeq ($(strip $(COMPILE_FOR_PROFILE)),YES)
ifeq ($(strip $(COMPILE_FOR_DEBUG)),YES)
$(warning Both COMPILE_FOR_PROFILE and COMPILE_FOR_DEBUG are set to YES)
endif
endif

ifeq ($(strip $(COMPILE_FOR_DEBUG)),YES)
OPT_LEVEL ?= -O0
else
# Note: changed from -Os to -O3 as customers like maximum optimizations
OPT_LEVEL ?= -O3
endif

ifndef CC_OPT
#################################################################
# Define the options for the compiler

##### Options controlling the kind of output

# Compile or assemble the source files, but do not link
CC_OPT += -c

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
CC_OPT += -Wpedantic

# This enables all the warnings about constructions that some users
# consider questionable
CC_OPT += -Wall

# This enables some extra warning flags that are not enabled by -Wall
CC_OPT += -Wextra

##### Note: the following list of settings depends on the choice of general
#####       warnings above. Changing pedantic, all or extra needs re-evaluation

# Give a warning when a value of type float is implicitly promoted to double
CC_OPT += -Wdouble-promotion

# Warn if the compiler detects paths that trigger erroneous or undefined behavior
# due to dereferencing a null pointer.
CC_OPT += -Wnull-dereference

# Warn if a user-supplied include directory does not exist
# Disabled: the application currently has the USER_INCLUDE_DIRS
#   defined which includes non-existent directories -> too much noise.
# CC_OPT += -Wmissing-include-dirs

# Warn if left shifting a negative value.
CC_OPT += -Wshift-negative-value

# Warn about left shift overflows.
CC_OPT += -Wshift-overflow

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

# Warn whenever a constant static variable is unused aside from its declaration.
CC_OPT += -Wunused-const-variable=2

# Do not warn about unused local typedefs
CC_OPT += -Wno-unused-local-typedefs

# Warn if an automatic variable is used without first being initialized or if a
# variable may be clobbered by a setjmp call.
CC_OPT += -Wuninitialized

# Warn when a #pragma directive is encountered which is not understood by GCC
# Note: using this option such pragmas are also reported for system header files
CC_OPT += -Wunknown-pragmas

# It warns about cases where the compiler optimizes based on the assumption
# that signed overflow does not occur
CC_OPT += -Wstrict-overflow=2

# It warns about subscripts to arrays that are always out of bounds
# This option is only active when -ftree-vrp  is active (default for -O2 and
# above)
# Note: -Warray-bounds == -Warray-bounds=1 is enabled by -Wall
#       Setting this to 2 could give a large number of false positives
CC_OPT += -Warray-bounds=1

# Warn about duplicated conditions in an if-else-if chain.
CC_OPT += -Wduplicated-cond

# Warn about trampolines generated for pointers to nested functions.
# For security reasons we want to set the stacks to non-executable.
CC_OPT += -Wtrampolines

# Warn if floating point values are used in equality comparisons
CC_OPT += -Wfloat-equal

# Warn when a declaration is found after a statement in a block
# Note: when switching to ISO-C:99, this is legal and the warning should be removed
CC_OPT += -Wdeclaration-after-statement

# Warn if an undefined identifier is evaluated in an #if directive
CC_OPT += -Wundef

# Do not warn whenever an #else or an #endif are followed by text.
CC_OPT += -Wno-endif-labels

# Warn whenever a local variable shadows another local variable, parameter
# or global variable or whenever a built-in function is shadowed
CC_OPT += -Wshadow

# Report stack usage of all functions into individual files
CC_OPT += -fstack-usage

# Warn whenever a function call is cast to a non-matching type
CC_OPT += -Wbad-function-cast

# Warn about features not present in ISO C90, but present in ISO C99
# Note: experimental, needs to be checked for false positives
CC_OPT += -Wc90-c99-compat

# Warn about features not present in ISO C99, but present in ISO C11.
# Note: experimental, needs to be checked for false positives
CC_OPT += -Wc99-c11-compat

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
CC_OPT += -Wcast-qual

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

# Warn when macros __TIME__, __DATE__ or __TIMESTAMP__ are encountered.
CC_OPT += -Wdate-time

# warns about cases where a goto or switch skips the initialization of a variable.
# This sort of branch is an error in C++ but not in C.
CC_OPT += -Wjump-misses-init

# Warn for implicit conversions that may change the sign of an integer value.
# disabled: causes lots of warnings if parameters are larger (e.g. int)
#   than the prototype of the function (e.g. uint8).
#   This also applies to e.g. constants and values
# CC_OPT += -Wsign-conversion

# Warn for implicit conversions that reduce the precision of a real value.
CC_OPT += -Wfloat-conversion

# Warn if in a loop with constant number of iterations the compiler detects
# undefined behavior in some statement during one or more of the iterations. 
CC_OPT += -Wno-aggressive-loop-optimizations

# Warn about suspicious uses of logical operators in expressions. This
# includes using logical operators in contexts where a bit-wise operator
# is likely to be expected.
# Disabled: all cases that have been warned about using this option are
#  configuration optimizations, e.g. a check is intentionally always '0'
#  or '!= 0' when compiling.
# CC_OPT += -Wlogical-op

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

# Warn if variable length array is used in the code
CC_OPT += -Wvla

# inhibit warning about usage of long long as it is required for the uint64/sint64 types.
CC_OPT += -Wno-long-long

# Variadic macros are used: OS_WINDOWSLog() in Os_kernel_WINDOWS.h for example.
# Note, this option has been disabled again until further notice, because of the special build system.
#CC_OPT += -Wno-variadic-macros

# Do not emit separate sections with GCC ident strings
# - they make problems with objcopy / PBconfig data link/locate/objcopy step
CC_OPT += -fno-ident

# Do not recover in case the sanitizer has found anything. This is important, because
# we want detected problems to show up in the test protocol. Without this option, we
# would need an extra parser for the sanitizer output.
CC_OPT += -fno-sanitize-recover

##### Options for Debugging Your Program or GCC

# Request debugging information and also use level to specify how much information.
# It seems to be better to disable debugging information during profiling
# (see http://www.cs.duke.edu/~ola/courses/programming/gprof.html).
ifeq ($(strip $(COMPILE_FOR_PROFILE)),NO)
  ifeq ($(strip $(COMPILE_FOR_DEBUG)),YES)
    CC_OPT += -g3
  else
    CC_OPT += -g
  endif
endif

# Generate extra code to write profile information suitable for the analysis program gprof.
# See http://gcc.gnu.org/onlinedocs/gcc-4.7.2/gcc/Debugging-Options.html#Debugging-Options
ifeq ($(strip $(COMPILE_FOR_PROFILE)),YES)
CC_OPT += -pg
endif

##### Options that control optimization

# Set optimization level
CC_OPT += $(OPT_LEVEL)

# Disable strict aliasing optimization
CC_OPT += -fno-strict-aliasing

##### Options that control the undefined behavior sanitizer
# Note: currently all are commented

# Enable UndefinedBehaviorSanitizer, a fast undefined behavior detector.
# Note: it is unclear which ones of the suboptions are enabled by this one.
CC_OPT += -fsanitize=undefined

##### Suboptions

# This option enables checking that the result of a shift operation is not undefined.
# CC_OPT += -fsanitize=shift

# Detect integer division by zero.
# CC_OPT += -fsanitize=integer-divide-by-zero

# This option instructs the compiler to check that the size of a variable length array
# is positive.
# CC_OPT += -fsanitize=vla-bound

# This option enables pointer checking.
# CC_OPT += -fsanitize=null

# This option enables signed integer overflow checking.
# CC_OPT += -fsanitize=signed-integer-overflow

# This option enables instrumentation of array bounds.
# CC_OPT += -fsanitize=bounds

# This option enables strict instrumentation of array bounds.
# CC_OPT += -fsanitize=bounds-strict

# This option enables checking of alignment of pointers
# CC_OPT += -fsanitize=alignment

# This option enables instrumentation of memory references
# CC_OPT += -fsanitize=object-size

# Detect floating-point division by zero.
# CC_OPT += -fsanitize=float-divide-by-zero

# This option enables floating-point type to integer conversion checking.
# CC_OPT += -fsanitize=float-cast-overflow

# Note: -fsanitize=nonnull-attribute and -fsanitize=returns-nonnull-attribute are
#       not used as we currently don't instrument with 'nonnull'

# This option enables instrumentation of loads from bool.
# CC_OPT += -fsanitize=bool

# Note: -fsanitize=enum is not used as test cases use it for out of bounds checking

# Report a __builtin_trap rather than a libubsan library routing.
# Note: could be interesting as we are in a freestanding environment.
# CC_OPT += -fsanitize-undefined-trap-on-error

##### Options that control the stack checker
# Note: checking all functions is overkill, the 'strong' option has shown to
#       be a good balance
# Note: as it is genuinely a good idea, we need to see how much we change our
#       object code with -fsanitize and -fstack-protector
# CC_OPT += -fstack-protector-strong

##### Options that control the preprocessor

CC_OPT += -DNOGDI -D_X86_ -D_WIN32X86_C_GCC_

##### Options for Code Generation Conventions

# This option generates traps for signed overflow on addition, subtraction,
# multiplication operations
# CC_OPT += -ftrapv

endif

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

# Generate extra code to write profile information suitable for the analysis program gprof.
# See http://gcc.gnu.org/onlinedocs/gcc-4.7.2/gcc/Debugging-Options.html#Debugging-Options
ifeq ($(strip $(COMPILE_FOR_PROFILE)),YES)
LINK_OPT                += -pg
endif

LINK_OPT_POST += -lubsan

#################################################################
# define output files
MAP_FILE                = $(BIN_DIR)\$(PROJECT).map
OUT_FILE                = $(BUILD_DIR)\$(PROJECT).out
DEBUG                   = NO

#################################################################
# Exclude Mirosoft OLE 2.0 and its companion Component Object Model (COM)
# declarations. The reason to exclude these declarations is to avoid
# name clashes with AUTOSAR-defined types. For example, if this is not done,
# then the inclusion of 'windows.h' results in conflicting declarations
# of the type 'ApplicationType'.
# Note, it's safe to exclude OLE 2.0, because these things will never be used
# in this context.
PREPROCESSOR_DEFINES += EB_COM_NO_WINDOWS_H
EB_COM_NO_WINDOWS_H_KEY = COM_NO_WINDOWS_H
EB_COM_NO_WINDOWS_H_VALUE = 1
