#
# \file
#
# \brief AUTOSAR Xcp
#
# This file contains the implementation of the AUTOSAR
# module Xcp.
#
# \version 2.12.0
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

Xcp_src_FILES      := $(Xcp_CORE_PATH)/src/Xcp.c \
                      $(Xcp_CORE_PATH)/src/Xcp_CommandProcessor.c \
                      $(Xcp_CORE_PATH)/src/Xcp_EventProcessor.c \
                      $(Xcp_CORE_PATH)/src/Xcp_EventReceiver.c \
                      $(Xcp_CORE_PATH)/src/Xcp_ReceiveProcessor.c \
                      $(Xcp_CORE_PATH)/src/Xcp_StimProcessor.c \
                      $(Xcp_CORE_PATH)/src/Xcp_TransmitProcessor.c \
                      $(Xcp_CORE_PATH)/src/Xcp_MemoryProxy.c \
                      $(Xcp_OUTPUT_PATH)/src/Xcp_Cfg.c \
                      $(Xcp_OUTPUT_PATH)/src/Xcp_Runnables.c

LIBRARIES_TO_BUILD += Xcp_src

Xcp_Wrapper_src_FILES      := $(Xcp_Wrapper_OUTPUT_PATH)/src/Xcp_Wrapper.c

LIBRARIES_TO_BUILD += Xcp_Wrapper_src
#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES




