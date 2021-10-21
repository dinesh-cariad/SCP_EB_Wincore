#ifndef STBM_INT_STC_H
#define STBM_INT_STC_H

/**
 * \file
 *
 * \brief AUTOSAR StbM
 *
 * This file contains the implementation of the AUTOSAR
 * module StbM.
 *
 * \version 3.1.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */


/*==[Includes]================================================================*/

#include <StbM_Types.h>
#include <StbM_TimeConversionMacros.h>

/*==[Macros]==================================================================*/

#if (defined STBM_TIME_BASE_USES_OS)
#error STBM_TIME_BASE_USES_OS is already defined.
#endif
/** \brief indicates that the time base parameter StbMLocalTimeHardware points to the Os
 */
#define STBM_TIME_BASE_USES_OS   255U


#if (defined STBM_OFFSET_TIME_BASE_NO_ETHTSYN_AND_OS_USED)
#error STBM_OFFSET_TIME_BASE_NO_ETHTSYN_AND_OS_USED is already defined.
#endif
/** \brief indicates that the time base parameter StbMLocalTimeHardware does not point to EthTSyn or Os
 */
#define STBM_OFFSET_TIME_BASE_NO_ETHTSYN_AND_OS_USED  255U

#if (defined STBM_TIME_BASE_DOES_NOT_USE_SHARED_DATA)
#error STBM_TIME_BASE_DOES_NOT_USE_SHARED_DATA is already defined.
#endif
/** \brief indicates that the time base parameter StbMProvideDataInSharedMemory is false
 */
#define STBM_TIME_BASE_DOES_NOT_USE_SHARED_DATA   255U

/* ---- Indicates the time base role ---- */

#if (defined STBM_ROLE_MASTER)
#error STBM_ROLE_MASTER is already defined.
#endif
/** \brief indicates that a time base is a master time base
 */
#define STBM_ROLE_MASTER    0U

#if (defined STBM_ROLE_SLAVE)
#error STBM_ROLE_SLAVE is already defined.
#endif
/** \brief indicates that a time base is a slave time base
 */
#define STBM_ROLE_SLAVE     1U

#if (defined STBM_ROLE_GATEWAY)
#error STBM_ROLE_GATEWAY is already defined.
#endif
/** \brief indicates that a time base is a gateway time base
 */
#define STBM_ROLE_GATEWAY   2U

#if (defined STBM_DUMMY_ROLE_OFFSET_TIMEBASE)
#error STBM_DUMMY_ROLE_OFFSET_TIMEBASE is already defined.
#endif
/** \brief dummy role used for offset timebases
 */
#define STBM_DUMMY_ROLE_OFFSET_TIMEBASE   3U


#if (defined STBM_DUMMY_GPT_MAXCHANNELTICKVALUE_OFFSET_TIMEBASE)
#error STBM_DUMMY_GPT_MAXCHANNELTICKVALUE_OFFSET_TIMEBASE is already defined.
#endif
/** \brief dummy gpt max channel tick value used for offset timebases
 */
#define STBM_DUMMY_GPT_MAXCHANNELTICKVALUE_OFFSET_TIMEBASE   0U

/* ---- Indicates the sign of the stored offset ---- */

#if (defined STBM_SIGN_IS_NEGATIVE)
#error STBM_SIGN_IS_NEGATIVE is already defined.
#endif
/** \brief indicates that an offset has a positive sign
 */
#define STBM_SIGN_IS_NEGATIVE     0U

#if (defined STBM_SIGN_IS_POSITIVE)
#error STBM_SIGN_IS_POSITIVE is already defined.
#endif
/** \brief indicates that an offset has a negative sign
 */
#define STBM_SIGN_IS_POSITIVE     1U

#if (defined STBM_SYSTEM_WIDE_MASTER_ENABLED)
#error STBM_SYSTEM_WIDE_MASTER_ENABLED is already defined.
#endif
/** \brief Global Time Master time base acts as a system-wide source of time
 */
#define STBM_SYSTEM_WIDE_MASTER_ENABLED     1U

#if (defined STBM_SYSTEM_WIDE_MASTER_DISABLED)
#error STBM_SYSTEM_WIDE_MASTER_DISABLED is already defined.
#endif
/** \brief Global Time Master time base doesn't act as a system-wide source of time
 */
#define STBM_SYSTEM_WIDE_MASTER_DISABLED     0U

#if (defined STBM_RATE_DEVIATION_DEFAULT_VALUE)
#error STBM_RATE_DEVIATION_DEFAULT_VALUE is already defined.
#endif
/** \brief indicates the default value for the Rate Deviation
 */
#define STBM_RATE_DEVIATION_DEFAULT_VALUE     0U

#if (defined STBM_RATE_CORRECTION_DEFAULT_VALUE)
#error STBM_RATE_CORRECTION_DEFAULT_VALUE is already defined.
#endif
/** \brief indicates the default value for the Rate Correction(1U to Q format)
 */
#define STBM_RATE_CORRECTION_DEFAULT_VALUE     268435456U

#if (defined STBM_Q_FRACTION)
#error STBM_Q_FRACTION is already defined.
#endif
/** \brief indicates the number of bits for the fractional part
 */
#define STBM_Q_FRACTION     28U

#if (defined STBM_Q_INTEGRAL)
#error STBM_Q_INTEGRAL is already defined.
#endif
/** \brief indicates the number of bits for the integral part
 */
#define STBM_Q_INTEGRAL     4U

#if (defined STBM_FACTOR_Q)
#error STBM_FACTOR_Q is already defined.
#endif
/** \brief indicates the Q factor which in our case is 2^28
 */
#define STBM_FACTOR_Q     268435456U

#if (defined STBM_FACTOR_PPM)
#error STBM_FACTOR_PPM is already defined.
#endif
/** \brief indicates the multiplier to get ppm
 */
#define STBM_FACTOR_PPM     1000000U

#if (defined STBM_FACTOR_MORE_PRECISION)
#error STBM_FACTOR_MORE_PRECISION is already defined.
#endif
/** \brief indicates the multiplicator to get ppm
 */
#define STBM_FACTOR_MORE_PRECISION     1000000000U

#if (defined STBM_NSECS_PER_SEC)
#error STBM_NSECS_PER_SEC is already defined.
#endif
/** \brief indicates number of nanoseconds in a second
 */
#define STBM_NSECS_PER_SEC     1000000000U

#if (defined STBM_RATE_DEVIATION_MAX_VALUE)
#error STBM_RATE_DEVIATION_MAX_VALUE is already defined.
#endif
/** \brief indicates the maximum value abs(RateDeviation) can have
 */
#define STBM_RATE_DEVIATION_MAX_VALUE 32000U

#if (defined STBM_RATE_DEVIATION_INVALID_VALUE)
#error STBM_RATE_DEVIATION_INVALID_VALUE is already defined.
#endif
/** \brief indicates an invalid value for rate deviation
 */
#define STBM_RATE_DEVIATION_INVALID_VALUE 32001U

#if (defined STBM_INVALID_INDEX)
#error STBM_INVALID_INDEX is already defined.
#endif
/** \brief indicates an invalid index from configuration
 */
#define STBM_INVALID_INDEX 255U

#if (defined STBM_NANOSECONDS_HI_MAX_ALLOWED)
#error STBM_NANOSECONDS_HI_MAX_ALLOWED is already defined.
#endif
/** \brief indicates max nanosecondsHi accepted for the Virtual Local Time
 */
#define STBM_NANOSECONDS_HI_MAX_ALLOWED 3U

#if (defined STBM_NANOSECONDS_LO_MAX_ALLOWED)
#error STBM_NANOSECONDS_LO_MAX_ALLOWED is already defined.
#endif
/** \brief indicates max nanosecondsHi accepted for the Virtual Local Time
 */
#define STBM_NANOSECONDS_LO_MAX_ALLOWED 3115098114U

#if (defined STBM_NANOSECONDS_LO_MAX)
#error STBM_NANOSECONDS_LO_MAX is already defined.
#endif
/** \brief indicates max nanosecondsHi accepted for the Virtual Local Time
 */
#define STBM_NANOSECONDS_LO_MAX 4294967295U

#if (defined STBM_NUMBER_OF_SECONDS_IN_A_NANOSECONDS_HI)
#error STBM_NUMBER_OF_SECONDS_IN_A_NANOSECONDS_HI is already defined.
#endif
/** \brief indicates number of seonds in a nanosecondsHi unit
 */
#define STBM_NUMBER_OF_SECONDS_IN_A_NANOSECONDS_HI 4U

#if (defined STBM_NUMBER_OF_NANOSECONDS_LEFT_IN_A_NANOSECONDS_HI)
#error STBM_NUMBER_OF_NANOSECONDS_LEFT_IN_A_NANOSECONDS_HI is already defined.
#endif
/** \brief indicates number of seonds in a nanosecondsHi unit
 */
#define STBM_NUMBER_OF_NANOSECONDS_LEFT_IN_A_NANOSECONDS_HI 294967295U

#if (defined STBM_SYNC_TO_GATEWAY_MASK)
#error STBM_SYNC_TO_GATEWAY_MASK is already defined.
#endif
/** \brief StbM_TimeBaseStatusType SYNC_TO_GATEWAY mask
 */
#define STBM_SYNC_TO_GATEWAY_MASK 0x04U

#if (defined STBM_NO_RESTART_NEEDED)
#error STBM_NO_RESTART_NEEDED is already defined.
#endif
/** \brief indicates that rate correction measurements do not need restart
 */
#define STBM_NO_RESTART_NEEDED 0U

#if (defined STBM_RESTART_NEEDED)
#error STBM_RESTART_NEEDED is already defined.
#endif
/** \brief indicates that rate correction measurements need restart
 */
#define STBM_RESTART_NEEDED 1U

/*==[Types]===================================================================*/

/** \brief Function pointer to set global time
 **
 ** This type defines a function pointer, pointing to the API serivce
 ** for setting the global time.
 ** In case an Os counter is used, this pointer is used to pointer to
 ** an internal StbM function.
 */
typedef P2FUNC (Std_ReturnType, STBM_APPL_CODE, StbM_TimeSetterFctPtrType)
(
  VAR(StbM_SynchronizedTimeBaseType, STBM_VAR) timeBaseId,
  P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStampPtr,
  P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, STBM_APPL_DATA) localTimePtr
);

/** \brief Time Recording information
 **
 ** This type contains runtime information regarding
 ** time recording
 */
typedef struct
{
  VAR(uint32, TYPEDEF) counter;  /**< block counter */
  VAR(boolean, TYPEDEF) wasCounterWrapped;  /**< let the next StbM_BusSetGlobalTime API, knows that the counter was wrapped */
} StbM_TimeRecordingInfoType;

/*==[Declaration of functions with internal linkage]==========================*/

/*==[Constants with internal linkage]=========================================*/

/*==[Variables with internal linkage]=========================================*/

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/

/*==[Definition of functions with internal linkage]===========================*/

/*==================[end of file]===========================================*/


#endif /* ifndef STBM_INT_STC_H */
