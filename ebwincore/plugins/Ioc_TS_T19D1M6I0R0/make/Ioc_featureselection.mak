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
#
# This makefile is used from both the plugin build and the user build environment
# It must only include the feature set definitions
# $Id: Ioc_featureselection.mak 2964 2018-11-14 14:05:00Z olme8414 $

IOC_CPUFAMILY ?= $(TARGET)

-include $(WORK_BASE)/asc_Os/config/$(TARGET).mk
ifneq ($(OS_CPU),)
IOC_CPU ?= $(OS_CPU)
endif

# $(DERIVATE) is the default, if $(OS_CPU) is not set by config/$(TARGET).mk
IOC_CPU ?= $(DERIVATE)

# Check presence of variables.
ifeq ($(IOC_CPUFAMILY),)
$(error IOC_CPUFAMILY is not set)
endif
ifeq ($(IOC_CPU),)
$(error IOC_CPU is not set)
endif

# List of all features with individual file selection
#  - Multicore: Enabled on multicore processors
#
IOC_FEATURELIST := Multicore


# only needs to be there if the derivative needs to override the defaults defined below
-include $(IOC_FEATURELIST_MAKEDIR)/$(IOC_CPUFAMILY)/$(IOC_CPU)/Ioc_featureselection_$(IOC_CPUFAMILY)_$(IOC_CPU).mak

# only needs to be there if the architecture needs to override the defaults defined below
-include $(IOC_FEATURELIST_MAKEDIR)/$(IOC_CPUFAMILY)/Ioc_featureselection_$(IOC_CPUFAMILY).mak


IOC_FEATURE_Multicore ?= enabled


DIRMERGE_SETTINGS_TAG_IocFeature_Multicore := $(IOC_FEATURE_Multicore)
