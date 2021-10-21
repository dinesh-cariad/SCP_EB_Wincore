# \file
#
# \brief AUTOSAR Os
#
# This file contains the implementation of the AUTOSAR
# module Os.
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 1998 - 2020 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.

# This file defines the used lib suffixes.
# Each flag has a global prepocessor define and one upper case letter in
# the lib suffix.

OS_LIB_EMPTY=
OS_LIB_SPACE=$(OS_LIB_EMPTY) $(OS_LIB_EMPTY)

#############################################
# common lib flag definitions
#############################################
OS_COMMON_LIB_FLAG_DEFAULT=o
OS_COMMON_LIB_FLAGS = \
    os_kerneltype \
    os_trace \
    os_devsanity \
    os_memmap \
    os_ncores \
    os_scheduling_algorithm \
    os_initcore \
    os_alci

#############
# kernel interface
#
# syscall, no syscall, MicroOs
os_kerneltype_KEY=OS_KERNEL_TYPE
ifeq ($(OS_KERNEL_TYPE),MICROKERNEL)
os_kerneltype_VALUE=OS_MICROKERNEL
os_kerneltype_FLAG=M
else
ifeq ($(OS_KERNEL_TYPE),SYSTEM_CALL)
os_kerneltype_VALUE=OS_SYSTEM_CALL
os_kerneltype_FLAG=S
else
ifeq ($(OS_KERNEL_TYPE),FUNCTION_CALL)
os_kerneltype_VALUE=OS_FUNCTION_CALL
os_kerneltype_FLAG=F
else
ifneq ($(OS_KERNEL_TYPE),)
$(error "supported OS_KERNEL_TYPE are MICROKERNEL, SYSTEM_CALL or FUNCTION_CALL but is currently $(OS_KERNEL_TYPE)")
endif
endif
endif
endif

#############
# trace
#
os_trace_KEY=OS_USE_TRACE
ifeq ($(OS_TRACE),TRUE)
os_trace_VALUE =1
os_trace_FLAG  =T
else
os_trace_VALUE =0
os_trace_FLAG  =$(OS_COMMON_LIB_FLAG_DEFAULT)
endif

#######################
# scheduling algorithm
#
os_scheduling_algorithm_KEY=OS_SCHEDULING_ALGORITHM
ifeq ($(OS_SCHEDULING_ALGORITHM),CLZ_QUEUE)
os_scheduling_algorithm_VALUE=OS_CLZ_QUEUE
os_scheduling_algorithm_FLAG=R
else
ifeq ($(OS_SCHEDULING_ALGORITHM),LINKED_LIST)
os_scheduling_algorithm_VALUE=OS_LINKED_LIST
os_scheduling_algorithm_FLAG=L
else
ifneq ($(OS_SCHEDULING_ALGORITHM),)
$(error "supported OS_SCHEDULING_ALGORITHM are CLZ_QUEUE or LINKED_LIST but is currently $(OS_SCHEDULING_ALGORITHM)")
endif
endif
endif

#############
# devsanity
#
os_devsanity_KEY =OS_DEVELOPMENT_SANITY_CHECKS
ifeq ($(OS_DEVELOPMENT_SANITY_CHECKS),TRUE)
os_devsanity_VALUE =1
os_devsanity_FLAG  =S
else
os_devsanity_VALUE =0
os_devsanity_FLAG  =$(OS_COMMON_LIB_FLAG_DEFAULT)
endif

#############
# memmap
#
os_memmap_KEY =OS_MEMMAP

ifeq ($(OS_INCLUDE_MEMMAP_SUPPORT),TRUE)
os_memmap_VALUE =1
os_memmap_FLAG =M
else
os_memmap_VALUE = 0
os_memmap_FLAG =$(OS_COMMON_LIB_FLAG_DEFAULT)
endif

#############
# multicore
#
os_ncores_FLAG =C$(OS_N_CORES)
ifneq ($(OS_N_CORES),1)
OS_AUTOSAR_SPINLOCKS=TRUE
endif

#######################################################
# Init core flag
#
os_initcore_KEY = OS_INITCORE
ifneq ($(OS_INITCORE),)
os_initcore_FLAG = $(OS_INITCORE)
os_initcore_VALUE = $(OS_INITCORE)
else
os_initcore_FLAG = 0
os_initcore_VALUE = 0
endif

#############
# Advacend logical core interface
#
os_alci_KEY=OS_USE_LOGICAL_CORE_IDS
ifeq ($(OS_ALCI),TRUE)
os_alci_VALUE =1
os_alci_FLAG  =A
else
os_alci_VALUE =0
os_alci_FLAG  =$(OS_COMMON_LIB_FLAG_DEFAULT)
endif

#######################################################
# get architecture specific lib flags
#
include $(AutosarOS_CORE_PATH)/make/$(OS_ARCH)/Os_libsuffix_$(OS_ARCH).mak


#######################################################
# prepare all defaults
#
define OS_DefineDefaultLibFlag
$(1)_FLAG?=$(OS_COMMON_LIB_FLAG_DEFAULT)
endef
define OS_GetLibFlag
$($(1)_FLAG)
endef

$(foreach flag,$(OS_COMMON_LIB_FLAGS),$(eval $(call OS_DefineDefaultLibFlag,$(flag))))
$(foreach flag,$(OS_ARCH_LIB_FLAGS),$(eval $(call OS_DefineDefaultLibFlag,$(flag))))

#######################################################
# FINALLY: construct the flag string and provide one global preprocessor
#          define for each used flag
#
OS_COMMON_LIB_FLAG_STRING:=$(subst $(OS_LIB_SPACE),$(OS_LIB_EMPTY),$(foreach flag,$(OS_COMMON_LIB_FLAGS),$(call OS_GetLibFlag,$(flag))))
OS_ARCH_LIB_FLAG_STRING:=$(subst $(OS_LIB_SPACE),$(OS_LIB_EMPTY),$(foreach flag,$(OS_ARCH_LIB_FLAGS),$(call OS_GetLibFlag,$(flag))))

ifneq ($(OS_ARCH_LIB_FLAG_STRING),)
OS_ARCH_LIB_FLAG_STRING:=_$(OS_ARCH_LIB_FLAG_STRING)
endif

OS_LIB_SUFFIX=$(OS_COMMON_LIB_FLAG_STRING)$(OS_ARCH_LIB_FLAG_STRING)

# publish preprocessor defines
PREPROCESSOR_DEFINES += $(OS_COMMON_LIB_FLAGS) $(OS_ARCH_LIB_FLAGS)

#########################
# Editor settings: DO NOT DELETE
# vi:set ts=4:
