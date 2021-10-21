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
#ifndef ATOMICS_USER_FUNCTIONS_H
#define ATOMICS_USER_FUNCTIONS_H

#include <Os.h>
#include <Std_Types.h>
#include <Atomics_user_functions_customizations.h>

/** \def ATOMICS_USER_MULTICORE_CASE
 ** \brief User switch to select between single- and multi-core cases.
 **
 ** The generic implementations for the lock and unlock macros
 ** ATOMICS_USER_LOCK_OBJECT and ATOMICS_USER_UNLOCK_OBJECT, rely on AUTOSAR
 ** mechansims. This macro selects between the single- and multi-core case. In
 ** the first case, all interrupts are disabled, when atomic objects are accessed.
 ** In the second case, an AUTOSAR spinlock is used, which must be provided by you.
 ** The macro ATOMICS_USER_AUTOSAR_SPINLOCK must expand to the id of this
 ** AUTOSAR spinlock.
 **
 ** This also means, that in the first case the functions DisableAllInterrupts()
 ** and EnableAllInterrupts() are used, while in the second case the functions
 ** GetSpinlock() and ReleaseSpinlock() are used. The call environment must
 ** permit the use of these functions respectively.
 **
 ** If the generic implementation is not used, this macro can be removed.
 */
#ifndef ATOMICS_USER_MULTICORE_CASE
#error "Please provide a definition for the macro ATOMICS_USER_MULTICORE_CASE."
#endif

/** \def ATOMICS_USER_AUTOSAR_SPINLOCK
 ** \brief Id of user-provided AUTOSAR spinlock.
 **
 ** When the macro ATOMICS_USER_MULTICORE_CASE expands to 1, this macro must
 ** expand to the id of the AUTOSAR spinlock to use to facilitate atomic access
 ** to atomic objects. This AUTOSAR spinlock must be provided by you and must
 ** also disable all interrupts.
 **
 ** If ATOMICS_USER_MULTICORE_CASE expands to 0, this macro has no effect and
 ** may be empty.
 */
#if ATOMICS_USER_MULTICORE_CASE
#ifndef ATOMICS_USER_AUTOSAR_SPINLOCK
#define ATOMICS_USER_AUTOSAR_SPINLOCK Atomics_Spinlock
#endif
#endif /* ATOMICS_USER_MULTICORE_CASE */

/** \def ATOMICS_USER_OBJECT_INITIALIZER
 ** \brief Expands into an initializer for an atomic object.
 **
 ** See ATOMICS_OBJECT_INITIALIZER for further details.
 ** The initializer must be consistent with ATOMICS_USER_ATOMIC_T.
 */
#ifndef ATOMICS_USER_OBJECT_INITIALIZER
#define ATOMICS_USER_OBJECT_INITIALIZER(initValue) (initValue)
#endif

/** \def ATOMICS_USER_OBJECT_INIT
 ** \brief Initializes the atomic object with the given value.
 **
 ** The initialization must be consistent with ATOMICS_USER_ATOMIC_T.
 **
 ** \param[in,out] objaddr The address of the atomic object to initialize.
 ** \param[in] initValue The initial value.
 */
#ifndef ATOMICS_USER_OBJECT_INIT
#define ATOMICS_USER_OBJECT_INIT(objaddr, initValue)\
    do {\
        *(objaddr) = (initValue);\
    } while (0)
#endif

/** \def ATOMICS_USER_THREAD_FENCE
 ** \brief User callout for thread fence.
 **
 ** This macro is called by Atomics_ThreadFence() as well as other Atomics_*()
 ** functions to establish a sequentially consistent memory barrier.
 **
 ** The generic implementation of this macro is empty, as it is not necessary for
 ** the generic implementations of the other Atomics_*() functions. Note that this
 ** means, that Atomics_ThreadFence(), which is based on this macro, does actually
 ** not implement a memory barrier.
 **
 ** If you adapt one of ATOMICS_USER_THREAD_FENCE, ATOMICS_USER_LOCK_OBJECT or
 ** ATOMICS_USER_UNLOCK_OBJECT, make sure that these three macros are consistent
 ** and provide the intended functionality.
 */
#ifndef ATOMICS_USER_THREAD_FENCE
#define ATOMICS_USER_THREAD_FENCE() \
    do {\
    } while (0)
#endif

/** \def ATOMICS_USER_LOCK_OBJECT
 ** \brief User callout to lock an atomic object.
 **
 ** This callout is used by all Atomic_*() and TS_AtomicSet/ClearBit_*()
 ** functions, which are passed an atomic object. This object may be of type
 ** Atomic_t, uint8, uint16, uint32, or uint64.
 ** It must lock the passed atomic object to prevent concurrent accesses
 ** to it. The necessary measures depend on the group of accessors, but usually,
 ** interrupts must be disabled and, in case of accessors on multiple
 ** cores, an AUTOSAR spinlock must be acquired beforehand. See also the
 ** macro ATOMICS_USER_MULTICORE_CASE for further details about these cases.
 **
 ** To identify the atomic object to be locked, its address is passed to this
 ** macro. This allows to be more specific about how to lock a specific atomic
 ** object.
 **
 ** The success indicator is an output parameter of this macro and tells the
 ** caller, whether the atomic object can be safely accessed.
 **
 ** If you adapt one of ATOMICS_USER_THREAD_FENCE, ATOMICS_USER_LOCK_OBJECT or
 ** ATOMICS_USER_UNLOCK_OBJECT, make sure that these three macros are consistent
 ** and provide the intended functionality.
 **
 ** \param[in] objaddr The address of the Atomic_t, uint8, uint16, uint32, or
 **                    uint64 atomic object to be locked.
 ** \param[out] successaddr The address of the success indicator set by this
 **                        function. If set to TRUE, the atomic object may be
 **                        accessed. If set to FALSE, the atomic object may not
 **                        be accessed.
 */
#if ATOMICS_USER_MULTICORE_CASE
#ifndef ATOMICS_USER_LOCK_OBJECT
#define ATOMICS_USER_LOCK_OBJECT(objaddr, successaddr)\
    do {\
        StatusType const stat = GetSpinlock(ATOMICS_USER_AUTOSAR_SPINLOCK);\
        if (stat != E_OK)\
        {\
            *(successaddr) = FALSE;\
        }\
        else\
        {\
            *(successaddr) = TRUE;\
        }\
    } while (0)
#endif

#else /* !ATOMICS_USER_MULTICORE_CASE */

#ifndef ATOMICS_USER_LOCK_OBJECT
#define ATOMICS_USER_LOCK_OBJECT(objaddr, successaddr)\
    do {\
        SuspendAllInterrupts();\
        *(successaddr) = TRUE;\
    } while (0)
#endif
#endif /* ATOMICS_USER_MULTICORE_CASE */

/** \def ATOMICS_USER_UNLOCK_OBJECT
 ** \brief User callout to unlock an atomic object.
 **
 ** This callout is used by all Atomic_*() and TS_AtomicSet/ClearBit_*()
 ** functions, which are passed an atomic object. This object may be of type
 ** Atomic_t, uint8, uint16, uint32, or uint64.
 ** It is the inverse function of ATOMICS_USER_LOCK_OBJECT. It shall
 ** unlock the given atomic object, so that other accessors get a chance
 ** to work on it. It has the same parameters as ATOMICS_USER_LOCK_OBJECT.
 **
 ** If you adapt one of ATOMICS_USER_THREAD_FENCE, ATOMICS_USER_LOCK_OBJECT or
 ** ATOMICS_USER_UNLOCK_OBJECT, make sure that these three macros are consistent
 ** and provide the intended functionality.
 **
 ** \param[in] objaddr The address of the Atomic_t, uint8, uint16, uint32, or
 **                    uint64 atomic object to be locked.
 ** \param[out] successaddr The address of the success indicator set by this
 **                         function. If set to TRUE, the atomic object may be
 **                         accessed. If set to FALSE, the atomic object may not
 **                         be accessed.
 */
#if ATOMICS_USER_MULTICORE_CASE
#ifndef ATOMICS_USER_UNLOCK_OBJECT
#define ATOMICS_USER_UNLOCK_OBJECT(objaddr, successaddr)\
    do {\
        StatusType const stat = ReleaseSpinlock(ATOMICS_USER_AUTOSAR_SPINLOCK);\
        if (stat != E_OK)\
        {\
            *(successaddr) = FALSE;\
        }\
        else\
        {\
            *(successaddr) = TRUE;\
        }\
    } while (0)
#endif

#else /* !ATOMICS_USER_MULTICORE_CASE */

#ifndef ATOMICS_USER_UNLOCK_OBJECT
#define ATOMICS_USER_UNLOCK_OBJECT(objaddr, successaddr)\
    do {\
        ResumeAllInterrupts();\
        *(successaddr) = TRUE;\
    } while (0)
#endif
#endif /* ATOMICS_USER_MULTICORE_CASE */

/** \def ATOMICS_USER_GET_VALUE
 ** \brief User callout to get the value of an atomic object.
 **
 ** \param[in] objaddr The address of the Atomic_t object of which its value
 **                    shall be returned.
 **
 * \return The value of the atomic object at \a *objaddr
 */
#define ATOMICS_USER_GET_VALUE(objaddr) (*(objaddr))

/** \def ATOMICS_USER_SET_VALUE
 ** \brief User callout to set the value of an atomic object.
 **
 ** \param[in] objaddr The address of the Atomic_t object of which its value
 **                    shall be returned.
 ** \param[in] value The new value to be assigned to the atomic object at
 **                  \a *objaddr.
 */
#define ATOMICS_USER_SET_VALUE(objaddr, value)\
    do {\
        *(objaddr) = (value);\
    } while (0)

#endif /* ATOMICS_USER_FUNCTIONS_H */
