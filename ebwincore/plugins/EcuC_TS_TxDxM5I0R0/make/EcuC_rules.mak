#
# \file
#
# \brief AUTOSAR EcuC
#
# This file contains the implementation of the AUTOSAR
# module EcuC.
#
# \version 5.0.21
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

#################################################################

EcuC_src_FILES_TP_API := $(EcuC_CORE_PATH)/src/EcuC.c \

EcuC_src_FILES_Meta_Data := $(EcuC_CORE_PATH)/src/EcuC_MetaData.c \

EcuC_Units ?= TP_API Meta_Data
EcuC_src_FILES += $(foreach unit,$(EcuC_Units),$(EcuC_src_FILES_$(unit)))

EcuC_src_FILES +=  $(EcuC_OUTPUT_PATH)/src/EcuC_Cfg.c

LIBRARIES_TO_BUILD   += EcuC_src
