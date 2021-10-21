#
# \file
#
# \brief AUTOSAR Crypto
#
# This file contains the implementation of the AUTOSAR
# module Crypto.
#
# \version 1.7.42
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

Crypto_xVIx_xAIx_lib_FILES          +=
Crypto_xVIx_xAIx_src_FILES          += \
	$(subst /,\,$(wildcard $(subst \,/,$(Crypto_xVIx_xAIx_OUTPUT_PATH))/instance/$(Crypto_xVIx_xAIx_INSTANCE)/src/Crypto_*.c)) \
	$(subst /,\,$(wildcard $(subst \,/,$(Crypto_xVIx_xAIx_OUTPUT_PATH))/src/Crypto_xVIx_xAIx*.c)) \
	$(subst /,\,$(wildcard $(subst \,/,$(Crypto_xVIx_xAIx_OUTPUT_PATH))/lib_src/*.c))

LIBRARIES_TO_BUILD                  += Crypto_xVIx_xAIx_src
DIRECTORIES_TO_CREATE               += $(Crypto_xVIx_xAIx_lib_LIB_OUTPUT_PATH)

CC_FILES_TO_BUILD                   +=
CPP_FILES_TO_BUILD                  +=
ASM_FILES_TO_BUILD                  +=

MAKE_CLEAN_RULES                    +=
MAKE_GENERATE_RULES                 +=
MAKE_COMPILE_RULES                  +=
MAKE_CONFIG_RULES                   +=

define defineCryptoLibOutputPATH
$(1)_OBJ_OUTPUT_PATH                := $(Crypto_xVIx_xAIx_lib_LIB_OUTPUT_PATH)
endef
$(foreach SRC,$(basename $(notdir $(subst \,/,$(Crypto_xVIx_xAIx_lib_FILES)))),$(eval $(call defineCryptoLibOutputPATH,$(SRC))))

#================================================================
# DEPENDENCIES (only for assembler files)
#

#================================================================
# RULES

