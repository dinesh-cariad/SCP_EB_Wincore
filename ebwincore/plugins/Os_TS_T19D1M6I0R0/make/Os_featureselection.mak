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
#
# This makefile is used from both the plugin build and the user build environment
# It must only include the feature set definitions

# List of all Os features with individual file selection
#  - CheckForDispatch: Provide the OS_CheckForDispatch function to the hw-specific part
#  - GenericLeaveKernel: Enable to include generic leave kernel function
#  - GenericReturnFromCall: Enable to include generic return from call implementation
#  - GenericTimestamp: enable the generic timestamp functionality of the kernel
#  - SharedTimestampAndCounterTimer: Enable if the timestamp and a hw counter can share a timer
#  - SharedTimestampAndExecTimer: Enable if the timestamp and execution timer can share a timer
#  - SoftwareCLZ: Use a software implementation of the count leading zeros operation
#  - CacheOperations: Enable if software operations to ensure cache coherency are needed. Disable
#                     if no cache is used on RAMs at all, or if the hardware takes care of coherency
#
#  - GenericMMU: generic MMU support functionality in the kernel
#  - GenericMMU_GetMem: with GenericMMU, use the plain Os_GetMem() function
#  - GenericMMU_GetMemAlign: with GenericMMU, use the OS_GetMemAligned() function
#  - GenericMMU_PageMap with GenericMMU, use the generic page mapper
#  - GenericMMU_PadReadOnly with GenericMMU, use the generic function for padding read-only RAM regions
#
#  - Multicore: Enabled on multicore processors
OS_FEATURELIST := \
	ARMSpinlocks PADecoratedStorage CheckForDispatch CacheOperations GenericLeaveKernel GenericReturnFromCall \
	GenericTimestamp GenericMMU GenericMMU_GetMem GenericMMU_GetMemAlign GenericMMU_PageMap \
	GenericMMU_PadReadOnly SharedTimestampAndCounterTimer SharedTimestampAndExecTimer SoftwareCLZ Multicore

# only needs to be there if the derivative needs to override the defaults defined below
-include $(OS_FEATURELIST_MAKEDIR)/$(TARGET)/$(OS_CPU)/Os_featureselection_$(TARGET)_$(OS_CPU).mak

# only needs to be there if the architecture needs to override the defaults defined below
-include $(OS_FEATURELIST_MAKEDIR)/$(TARGET)/Os_featureselection_$(TARGET).mak

DIRMERGE_SETTINGS_TAG_OsFeature_ARMSpinlocks ?= disabled
DIRMERGE_SETTINGS_TAG_OsFeature_PADecoratedStorage ?= disabled
DIRMERGE_SETTINGS_TAG_OsFeature_PAHasReservations ?= disabled

DIRMERGE_SETTINGS_TAG_OsFeature_GenericMMU ?= disabled
DIRMERGE_SETTINGS_TAG_OsFeature_GenericMMU_GetMem ?= disabled
DIRMERGE_SETTINGS_TAG_OsFeature_GenericMMU_GetMemAlign ?= disabled
DIRMERGE_SETTINGS_TAG_OsFeature_GenericMMU_PageMap ?= disabled
DIRMERGE_SETTINGS_TAG_OsFeature_GenericMMU_PadReadOnly ?= disabled

DIRMERGE_SETTINGS_TAG_OsFeature_CacheOperations ?= disabled
DIRMERGE_SETTINGS_TAG_OsFeature_CheckForDispatch ?= enabled
DIRMERGE_SETTINGS_TAG_OsFeature_GenericLeaveKernel ?= enabled
DIRMERGE_SETTINGS_TAG_OsFeature_GenericReturnFromCall ?= disabled
DIRMERGE_SETTINGS_TAG_OsFeature_GenericTimestamp ?= enabled
DIRMERGE_SETTINGS_TAG_OsFeature_SharedTimestampAndCounterTimer ?= enabled
DIRMERGE_SETTINGS_TAG_OsFeature_SharedTimestampAndExecTimer ?= enabled
DIRMERGE_SETTINGS_TAG_OsFeature_SoftwareCLZ ?= enabled

DIRMERGE_SETTINGS_TAG_OsFeature_Multicore ?= enabled

