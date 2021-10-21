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
#
# This makefile is used from both the plugin build and the user build environment
# It must only include the feature set definitions

# Only INTERNAL is supported for the OsTimestampTimer configuration option.
# Thus, there is only an internal time stamp timer.
DIRMERGE_SETTINGS_TAG_OsFeature_GenericTimestamp := disabled
DIRMERGE_SETTINGS_TAG_OsFeature_SharedTimestampAndCounterTimer := disabled
DIRMERGE_SETTINGS_TAG_OsFeature_SharedTimestampAndExecTimer := disabled
