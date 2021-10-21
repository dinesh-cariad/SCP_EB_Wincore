#
# \file
#
# \brief AUTOSAR Compiler
#
# This file contains the implementation of the AUTOSAR
# module Compiler.
#
# \version 1.0.8
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2017 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


#################################################################
# this file just references the compiler_rules.mak makfiles
# in the TOOLCHAIN directory
include $(Compiler_CORE_PATH)/make/$(TARGET)/$(TOOLCHAIN)/Compiler$(RULES_FILE_SUFFIX)

#####################################################################
# debug : starts BUILD_ALL and in case of success starts the TRACE32
# from Lauterbach to debug the application.
.PHONY : debug

ifeq ($(DERIVATE),$(filter $(DERIVATE), S32V234 S32V234BE))
ALTERNATIVE_DEBUG_RULES=yes
debug: $(CONFIG_T32) $(TRACE32_PATH) all
	$(DB_ARM32), $(DEBUG_ARM32_OPT_FILE)
	$(DB_ARM64), $(DEBUG_ARM64_OPT_FILE)

start_debug: $(CONFIG_T32) $(TRACE32_PATH)
	$(DB_ARM32), $(DEBUG_ARM32_OPT_FILE)
	$(DB_ARM64), $(DEBUG_ARM64_OPT_FILE)

endif
