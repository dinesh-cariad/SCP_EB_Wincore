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
#ifndef ATOMICS_PLATFORM_TYPES_H
#define ATOMICS_PLATFORM_TYPES_H

#include <Std_Types.h>

/* The macro CPU_TYPE_64 is only defined in Platform_Types.h for 64-bit
 * architectures. It is always needed, though, because 64-bit functions
 * are only defined by this module, when CPU_TYPE is equal to CPU_TYPE_64
 * and hence, CPU_TYPE_64 is always used by this module. If it was not
 * always defined, you'd get compiler warnings.
 */
#ifndef CPU_TYPE_64
#define CPU_TYPE_64 64U
#endif

#endif /* ATOMICS_PLATFORM_TYPES_H */
