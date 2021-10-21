# \file
#
# \brief AUTOSAR Ioc
#
# This file contains the implementation of the AUTOSAR
# module Ioc.
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2018 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.

# This file defines the used lib suffixes.
# Each flag has a global prepocessor define and one upper case letter in the lib suffix.

#############################################
# Get the libcfg variables (if they reside at the expected position).
#############################################
-include $(Ioc_OUTPUT_MAKE)/Ioc_gen_libcfg.mak

#############################################
# Empty strings and space.
#############################################
IOC_LIB_EMPTY=
IOC_LIB_SPACE=$(IOC_LIB_EMPTY) $(IOC_LIB_EMPTY)

#############################################
# common lib flag definitions
#############################################
IOC_COMMON_LIB_FLAG_DEFAULT=o
IOC_COMMON_LIB_FLAGS = \
    ioc_kerneltype


#############
# kernel interface
#
ioc_kerneltype_KEY=IOC_LCFG_KERNEL_TYPE
ifeq ($(IOC_LCFG_KERNEL_TYPE),IOC_MICROKERNEL)
ioc_kerneltype_VALUE=IOC_MICROKERNEL
ioc_kerneltype_FLAG=M
endif
ifeq ($(IOC_LCFG_KERNEL_TYPE),IOC_AUTOCOREOS)
ioc_kerneltype_VALUE=IOC_AUTOCOREOS
ioc_kerneltype_FLAG=Q
endif
ifeq ($(ioc_kerneltype_VALUE),)
ioc_kerneltype_VALUE=ERROR_UNEXPECTED_IOC_LCFG_KERNEL_TYPE_VALUE
ioc_kerneltype_FLAG=X
endif

#######################################################
# prepare all defaults
#
define IOC_DefineDefaultLibFlag
$(1)_FLAG?=$(IOC_COMMON_LIB_FLAG_DEFAULT)
endef
define IOC_GetLibFlag
$($(1)_FLAG)
endef

$(foreach flag,$(IOC_COMMON_LIB_FLAGS),$(eval $(call IOC_DefineDefaultLibFlag,$(flag))))

#######################################################
# FINALLY: construct the flag string and provide one global preprocessor
#          define for each used flag
#
IOC_COMMON_LIB_FLAG_STRING:=$(subst $(IOC_LIB_SPACE),$(IOC_LIB_EMPTY),$(foreach flag,$(IOC_COMMON_LIB_FLAGS),$(call IOC_GetLibFlag,$(flag))))

IOC_LIB_SUFFIX=$(IOC_COMMON_LIB_FLAG_STRING)

# publish preprocessor defines
PREPROCESSOR_DEFINES += $(IOC_COMMON_LIB_FLAGS)

#########################
# Editor settings: DO NOT DELETE
# vi:set ts=4:
