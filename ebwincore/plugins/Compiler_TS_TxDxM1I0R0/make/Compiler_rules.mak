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


include $(Compiler_CORE_PATH)/make/$(TARGET)/Compiler$(RULES_FILE_SUFFIX)

#####################################################################
# debug : starts BUILD_ALL and in case of success starts the TRACE32
# from Lauterbach to debug the application.
# syntax of command line T32 format is:
#   t32m<arch>[.<x>] [<options>] [-c <configfile> [<c_args>]] [-s <startup_script> [<s_args>]]
# more information: http://www2.lauterbach.com/pdf/debugger_user.pdf
# makefile variables consist of:
# DB = t32m<arch>[.<x>] [<options>]
# DEBUG_CFG_FILE = -c <configfile> [<c_args>]
# DEBUG_OPT_FILE = -s <startup_script> [<s_args>]

.PHONY : debug start_debug

ifneq ($(ALTERNATIVE_DEBUG_RULES),yes)
# default debug rules

debug: $(TRACE32_PATH) all
	$(EB_VERBOSE)$(subst /,\,$(DB) $(DEBUG_CFG_FILE) $(DEBUG_OPT_FILE))

start_debug: $(TRACE32_PATH)
	$(EB_VERBOSE)$(subst /,\,$(DB) $(DEBUG_CFG_FILE) $(DEBUG_OPT_FILE))

endif
