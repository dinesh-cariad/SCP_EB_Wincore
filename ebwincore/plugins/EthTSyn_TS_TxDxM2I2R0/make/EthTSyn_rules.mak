#
# \file
#
# \brief AUTOSAR EthTSyn
#
# This file contains the implementation of the AUTOSAR
# module EthTSyn.
#
# \version 2.2.4
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

ifndef TS_MERGED_COMPILE
TS_MERGED_COMPILE := TRUE
endif

ifndef TS_ETHTSYN_MERGED_COMPILE
TS_ETHTSYN_MERGED_COMPILE := $(TS_MERGED_COMPILE)
endif

ifeq ($(TS_ETHTSYN_MERGED_COMPILE),TRUE)

EthTSyn_src_FILES +=                              \
          $(EthTSyn_CORE_PATH)/src/EthTSyn_Merged.c \

else

EthTSyn_src_FILES      := \
    $(EthTSyn_CORE_PATH)/src/EthTSyn.c \
    $(EthTSyn_OUTPUT_PATH)/src/EthTSyn_Cfg.c \
    $(EthTSyn_CORE_PATH)/src/EthTSyn_Int01_Lib.c

endif

LIBRARIES_TO_BUILD     += EthTSyn_src


#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
