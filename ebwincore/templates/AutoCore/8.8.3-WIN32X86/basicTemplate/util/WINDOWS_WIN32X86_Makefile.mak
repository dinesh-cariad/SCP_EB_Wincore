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
# BOARD:
# This variable defines the used hardware (it is called board because
# of the usage of evaluation boards).
#
# For example:
# BOARD = eva168_2
BOARD       ?= WIN32X86

#################################################################
# TARGET:
# default target of this project
TARGET      ?= WINDOWS
DERIVATE    ?= WIN32X86

#################################################################
# TOOLCHAIN:
# The build environment allows to switch the compiler by
# changing the used toolchain. A toolchain references a set
# of makefiles that implement the compiler specific configuration
# for the current target as used by the Make plugin.
#
# These specific makefiles are:
# $(PROJECT_ROOT)\util\<TARGET>_<DERIVATE>_<TOOLCHAIN>_cfg.mak
# $(SSC_ROOT)\<Compiler plugin>\make\<TARGET>\<TOOLCHAIN>\compiler_defs.mak
# $(SSC_ROOT)\<compiler plugin>\make\<TARGET>\<TOOLCHAIN>\compiler_rules.mak
# $(SSC_ROOT)\<Compiler plugin>\make\<TARGET>\<TOOLCHAIN>\compiler_check.mak
#
# Supported toolchains:
# TOOLCHAIN ?= mgcc62
# TOOLCHAIN ?= bcgcc
#
TOOLCHAIN    ?= mgcc62
COMPILER     ?= ${TARGET}_${DERIVATE}_${TOOLCHAIN}

default: all
