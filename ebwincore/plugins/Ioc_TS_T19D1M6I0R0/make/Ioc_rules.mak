# Ioc_rules.mak
#
# This file just contains rules to build the module.
#
# (c) Elektrobit Automotive GmbH
#
# $Id: Ioc_rules.mak 2482 2018-02-01 09:20:32Z olme8414 $

LIBRARIES_TO_BUILD     += $(IOC_KERNEL_LIB_NAME) $(IOC_USER_LIB_NAME) $(IOC_MIXED_LIB_NAME)

ifneq ($(OS_N_CORES),1)
LIBRARIES_TO_BUILD     += $(IOC_MULTICORE_LIB_NAME)
endif

CC_FILES_TO_BUILD += $(IOC_SRC_FILES)

ifeq ($(MISRA_MODULE),Ioc)
  # The MISRA-C check wants the list of files stored in 'CC_FILES_TO_BUILD' to
  # be in a variable prefixed with the module's name. This means,
  # 'CC_FILES_TO_BUILD' must be copied into 'Ioc_CC_FILES_TO_BUILD'.
  # This is essentially the same mechanism, which is already used with
  # 'LIBRARIES_TO_BUILD'. There, the list of files of each 'lib' is stored as
  # list in a variable called '$(lib)_FILES'.
  Ioc_CC_FILES_TO_BUILD += $(IOC_SRC_FILES)
endif
