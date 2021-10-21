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
# Disclaimer:
# >>>  Use of an alternative build environment may  <<<
# >>> lead to non-executable or non-compilable code <<<
# 
# Use the build environment delivered with EB tresos AutoCore to
# execute and compile code. If you use an alternative build
# environment, your EB tresos AutoCore version may generate
# non-executable or non-compilable code.
#################################################################

# provide a default target, if nobody else does
.PHONY: default all
default: all
all:

#################################################################
# path sanitation & sensible default project name if none is specified (predefined or overridden later)
WORK_BASE		:= $(subst \,/,$(WORK_BASE))
TEST_BASE		:= $(subst \,/,$(TEST_BASE))
PROJECT_ROOT		:= $(subst \,/,$(PROJECT_ROOT))
PROJECT_NAME		?= $(lastword $(subst /, ,$(PROJECT_ROOT)))
GEN_OUTPUT_PATH		?= $(PROJECT_ROOT)/output/generated

#################################################################
# tresos settings
#
TRESOS2_WORKSPACE_DIR	 = $(abspath $(PROJECT_ROOT)/..)
TRESOS2_USER_OPTIONS	+= -data $(TRESOS2_WORKSPACE_DIR)
TRESOS2_PROJECT_NAME	 = $(PROJECT_NAME)
TRESOS2_USER_OPTIONS	+= -Dinfo=false -DmergeConfigs=true

#################################################################
# include merged makefile if they exist
-include $(PROJECT_ROOT)/util/Merged_Makefile.mak

#################################################################
# additional C source files to be built
#
CC_FILES_TO_BUILD += 

# We want project mode, not legacy.
USE_LEGACY_MAKECFG := false

-include common.mak

