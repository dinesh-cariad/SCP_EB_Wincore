#
# \file
#
# \brief AUTOSAR Dem
#
# This file contains the implementation of the AUTOSAR
# module Dem.
#
# \version 6.4.1
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# DEFINITIONS

#################################################################
# REGISTRY

Dem_src_FILES          := \
     $(Dem_CORE_PATH)/src/Dem.c \
     $(Dem_CORE_PATH)/src/Dem_DcmIfStatusInfo.c \
     $(Dem_CORE_PATH)/src/Dem_DcmIfFFandED.c \
     $(Dem_CORE_PATH)/src/Dem_DcmIfAccess.c \
     $(Dem_CORE_PATH)/src/Dem_DcmIfOBD.c \
     $(Dem_CORE_PATH)/src/Dem_Core.c \
     $(Dem_CORE_PATH)/src/Dem_J1939Dcm.c \
     $(Dem_CORE_PATH)/src/Dem_J1939FF.c \
     $(Dem_OUTPUT_PATH)/src/Dem_Cfg.c

LIBRARIES_TO_BUILD     += Dem_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
