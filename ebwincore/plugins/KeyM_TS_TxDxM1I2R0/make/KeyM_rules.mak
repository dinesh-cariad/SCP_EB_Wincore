#
# \file
#
# \brief AUTOSAR KeyM
#
# This file contains the implementation of the AUTOSAR
# module KeyM.
#
# \version 1.2.11
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#================================================================
# DEFINITIONS

#================================================================
# REGISTRY

KeyM_lib_FILES                      +=

KeyM_src_FILES                      += \
  $(KeyM_CORE_PATH)/src/KeyM.c         \
  $(KeyM_CORE_PATH)/src/KeyM_X509.c    \
  $(KeyM_CORE_PATH)/src/KeyM_Der.c     \
  $(KeyM_OUTPUT_PATH)/src/KeyM_Int_Cfg.c

LIBRARIES_TO_BUILD                  += KeyM_src
DIRECTORIES_TO_CREATE               += $(KeyM_lib_LIB_OUTPUT_PATH)

CC_FILES_TO_BUILD                   +=
CPP_FILES_TO_BUILD                  +=
ASM_FILES_TO_BUILD                  +=

MAKE_CLEAN_RULES                    +=
MAKE_GENERATE_RULES                 +=
MAKE_COMPILE_RULES                  +=
MAKE_CONFIG_RULES                   +=
#MAKE_ADD_RULES                      +=
#MAKE_DEBUG_RULES                    +=

define defineKeyMLibOutputPATH
$(1)_OBJ_OUTPUT_PATH                := $(KeyM_lib_LIB_OUTPUT_PATH)
endef
$(foreach SRC,$(basename $(notdir $(subst \,/,$(KeyM_lib_FILES)))),$(eval $(call defineKeyMLibOutputPATH,$(SRC))))

#================================================================
# DEPENDENCIES (only for assembler files)
#

#================================================================
# RULES

