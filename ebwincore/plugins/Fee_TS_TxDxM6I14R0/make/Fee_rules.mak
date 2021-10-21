#
# \file
#
# \brief AUTOSAR Fee
#
# This file contains the implementation of the AUTOSAR
# module Fee.
#
# \version 6.14.13
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

Fee_src_FILES           := \
    $(Fee_CORE_PATH)/src/Fee.c \
    $(Fee_CORE_PATH)/src/Fee_Extra.c \
	$(Fee_CORE_PATH)/src/Fee_Jobs.c \
    $(Fee_CORE_PATH)/src/Fee_Initialization.c \
    $(Fee_CORE_PATH)/src/Fee_Internal.c \
    $(Fee_CORE_PATH)/src/Fee_Sections.c \
    $(Fee_CORE_PATH)/src/Fee_SectionsSwitch.c \
    $(Fee_OUTPUT_PATH)/src/Fee_Cfg.c

LIBRARIES_TO_BUILD     += Fee_src

#################################################################
# DEPENDENCIES (only for assembler files)
#

#################################################################
# RULES
