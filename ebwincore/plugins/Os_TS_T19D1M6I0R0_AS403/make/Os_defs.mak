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

# define release variant for conditionals in generic makefiles
OS_RELEASE_SUFFIX := AS403

# redirect AS403 to common OS plugin
include $(SSC_ROOT)\Os_TS_T19D1M6I0R0\make\Os_defs.mak
