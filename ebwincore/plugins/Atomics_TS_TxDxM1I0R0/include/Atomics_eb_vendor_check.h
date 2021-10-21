/**
 * \file
 *
 * \brief AUTOSAR Atomics
 *
 * This file contains the implementation of the AUTOSAR
 * module Atomics.
 *
 * \version 1.0.15
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2017 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef ATOMICS_EB_VENDOR_CHECK_H
#define ATOMICS_EB_VENDOR_CHECK_H

#include <Os.h>

#ifndef OS_VENDOR_ID
#error "Please specify vendor of OS by defining OS_VENDOR_ID appropriately."
#endif

#if !defined(OS_VENDOR_ID_3SOFT) || (OS_VENDOR_ID != OS_VENDOR_ID_3SOFT)
#error "You must use EB tresos AutoCore OS, when ATOMICS_USE_GENERIC_IMPL is set to 0."
#endif

#endif /* ATOMICS_EB_VENDOR_CHECK_H */
