#
# \file
#
# \brief AUTOSAR SoAd
#
# This file contains the implementation of the AUTOSAR
# module SoAd.
#
# \version 1.8.16
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

SoAd_CORE_PATH      ?= $(PLUGINS_BASE)/SoAd_$(SoAd_VARIANT)

SoAd_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

SoAd_DEPENDENT_PLUGINS      := base_make tresos
SoAd_VERSION                := 1.00.00
SoAd_DESCRIPTION            := SoAd Basic Software Makefile PlugIn for Autosar
CC_INCLUDE_PATH            += $(SoAd_CORE_PATH)/include \
                              $(SoAd_CORE_PATH)/src \
                              $(SoAd_OUTPUT_PATH)/include

