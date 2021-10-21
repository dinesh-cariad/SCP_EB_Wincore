#
# \file
#
# \brief AUTOSAR Can
#
# This file contains the implementation of the AUTOSAR
# module Can.
#
# \version 3.1.4
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

LIBRARIES_TO_BUILD    += Can_src

Can_src_FILES      := \
    $(Can_CORE_PATH)/src/Can.c \
    $(Can_CORE_PATH)/src/Can_Irq.c \
    $(Can_CORE_PATH)/src/Can_WinApi.c \
    $(Can_OUTPUT_PATH)/src/Can_Cfg.c \
    $(Can_CORE_PATH)/vb_src/VirtualBusLib.c

LIB_INCLUDE_PATH+=$(Platforms_CORE_PATH)/lib

LIBRARIES_LINK_ONLY +=cif
LIBRARIES_LINK_ONLY +=EB_tresos_TAL
LIBRARIES_LINK_ONLY +=PCANBasic
LIBRARIES_LINK_ONLY +=winmm
LIBRARIES_LINK_ONLY +=vxlapi

LIB_PCAN_DLL=PCANBasic.dll
LIB_TAL_DLL=EB_tresos_TAL.dll
LIB_VECTOR_DLL=vxlapi.dll

# append one dependency to the target for the project binary (created by build-exe-file)
$(PROJECT_BINARY): $(BIN_OUTPUT_PATH)/$(LIB_VECTOR_DLL)
$(PROJECT_BINARY): $(BIN_OUTPUT_PATH)/$(LIB_PCAN_DLL)
$(PROJECT_BINARY): $(BIN_OUTPUT_PATH)/$(LIB_TAL_DLL)

# copy the shared lib from the compiler installation beside the project binary
$(BIN_OUTPUT_PATH)/$(LIB_VECTOR_DLL): $(Platforms_CORE_PATH)/lib/$(LIB_VECTOR_DLL)
	$(EB_VERBOSE)$(COPY) $(subst /,\,$< $@)

$(BIN_OUTPUT_PATH)/$(LIB_PCAN_DLL): $(Platforms_CORE_PATH)/lib/$(LIB_PCAN_DLL)
	$(EB_VERBOSE)$(COPY) $(subst /,\,$< $@)
                                        
$(BIN_OUTPUT_PATH)/$(LIB_TAL_DLL): $(Platforms_CORE_PATH)/lib/$(LIB_TAL_DLL)
	$(EB_VERBOSE)$(COPY) $(subst /,\,$< $@)

ifndef CAN_OMIT_STANDARD_VBUS_LIB

ifndef WIN32_LIB_PATH
ifeq ($(TOOLCHAIN), vc)
  $(error WIN32_LIB_PATH must be defined and point to the \
      folder containing WSock32.Lib if the standard VirtualBus \
      library is in use)
else
  $(error WIN32_LIB_PATH must be defined and point to the \
      folder containing libwsock32.a if the standard VirtualBus \
      library is in use)
endif # TOOLCHAIN = vc
endif # WIN32_LIB_PATH undefined

Can_src_FILES += $(Can_CORE_PATH)/vb_src/VirtualBusLib.c
endif

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
