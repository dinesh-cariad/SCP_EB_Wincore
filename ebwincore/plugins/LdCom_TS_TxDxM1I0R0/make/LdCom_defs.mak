#
# \file
#
# \brief AUTOSAR LdCom
#
# This file contains the implementation of the AUTOSAR
# module LdCom.
#
# \version 1.0.26
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

LdCom_CORE_PATH      ?= $(PLUGINS_BASE)/LdCom_$(LdCom_VARIANT)

LdCom_OUTPUT_PATH    ?= $(AUTOSAR_BASE_OUTPUT_PATH)

#################################################################
# REGISTRY

LdCom_DEPENDENT_PLUGINS      := base_make tresos
LdCom_VERSION                := 1.00.00
LdCom_DESCRIPTION            := LdCom Basic Software Makefile PlugIn for AUTOSAR
CC_INCLUDE_PATH            += $(LdCom_CORE_PATH)/include \
                              $(LdCom_CORE_PATH)/src \
                              $(LdCom_OUTPUT_PATH)/include

