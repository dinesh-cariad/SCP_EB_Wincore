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

#################################################################
# DEFINITIONS


#################################################################
# REGISTRY

ASM_FILES_TO_BUILD +=

ifeq ($(BUILD_MODE),LIB)
ifeq ($(MODULE),Os)
#################################################################
# lib build mode settings
LIBRARIES_TO_BUILD += $(OS_KERNEL_LIB_NAME) $(OS_USER_LIB_NAME) $(OS_ERROR_LIB_NAME)
Os_lib_LIB_VARIANT        = $($(LIB_VARIANT)_VARIANT)
OS_CONFIG_EXTENDED_STATUS = $($(LIB_VARIANT)_STATUS)
endif
else
LIBRARIES_TO_BUILD += Os_src
ifeq ($(OS_LIB_SRC_AVAILABLE),1)
LIBRARIES_TO_BUILD += $(OS_KERNEL_LIB_NAME) $(OS_ERROR_LIB_NAME) $(OS_USER_LIB_NAME)
else
LIBRARIES_LINK_ONLY += $(OS_LIB_OUTPUT_BASEPATH)\$(OS_KERNEL_LIB_NAME).$(LIB_FILE_SUFFIX)
LIBRARIES_LINK_ONLY += $(OS_LIB_OUTPUT_BASEPATH)\$(OS_USER_LIB_NAME).$(LIB_FILE_SUFFIX)
LIBRARIES_LINK_ONLY += $(OS_LIB_OUTPUT_BASEPATH)\$(OS_ERROR_LIB_NAME).$(LIB_FILE_SUFFIX)
endif
endif

MAKE_CLEAN_LIB_RULES  += AutosarOS_lib_clean

#################################################################
# RULES

AutosarOS_lib_clean:
ifeq ($(OS_LIB_SRC_AVAILABLE),1)
	$(SEPARATOR) [clean] 	AutosarOS libraries
	$(RM) "$(OS_LIB_OUTPUT_BASEPATH)\*.*"
	$(RM) "$(OS_LIB_OUTPUT_BASEPATH)\$(OS_ERROR_LIB_NAME)\*.*"
	$(RM) "$(OS_LIB_OUTPUT_BASEPATH)\$(OS_KERNEL_LIB_NAME)\*.*"
	$(RM) "$(OS_LIB_OUTPUT_BASEPATH)\$(OS_USER_LIB_NAME)\*.*"
endif

