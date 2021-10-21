#ifndef STBM_TYPES_H
#define STBM_TYPES_H

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

#include <Std_Types.h>
#include <StbM_Gen_Common.h>

/*==[Macros]==================================================================*/

#if (defined STBM_TIMEOUT_FLAG)
#error STBM_TIMEOUT_FLAG is already defined.
#endif
/** \brief TIMEOUT bit (Bit 0) in StbM_TimeBaseStatusType
 **
 ** 0x00: No Timeout on receiving Synchronization Messages
 ** 0x01: Timeout on receiving Synchronization Messages
 */
#define STBM_TIMEOUT_FLAG           0x1U

#if (defined STBM_TIME_LEAP_FUTURE_FLAG)
#error STBM_TIME_LEAP_FUTURE_FLAG is already defined.
#endif
/** \brief TIMELEAP_FUTURE bit (Bit 4) in StbM_TimeBaseStatusType
 **
 ** 0x00:  No leap within the received time
 ** 0x10:  Leap within the received time that exceeds a configured threshold in time leap future
 */
#define STBM_TIME_LEAP_FUTURE_FLAG             0x10U

#if (defined STBM_TIME_LEAP_PAST_FLAG)
#error STBM_TIME_LEAP_PAST_FLAG is already defined.
#endif
/** \brief TIMELEAP_PAST bit (Bit 5) in StbM_TimeBaseStatusType
 **
 ** 0x00:  No leap within the received time
 ** 0x20:  Leap within the received time that exceeds a configured threshold in time leap past
 */
#define STBM_TIME_LEAP_PAST_FLAG               0x20U

#if (defined STBM_ALL_TIME_LEAP_FLAGS_MASK)
#error STBM_ALL_TIME_LEAP_FLAGS_MASK is already defined.
#endif
/** \brief TIMELEAP_FUTURE bit (Bit 4) and TIMELEAP_PAST bit (Bit 5) in StbM_TimeBaseStatusType
 **
 ** 0x30:  Mask for both the time leap future and time leap past bits set
 */
#define STBM_ALL_TIME_LEAP_FLAGS_MASK       0x30U

#if (defined STBM_SYNC_TO_GATEWAY_FLAG)
#error STBM_SYNC_TO_GATEWAY_FLAG is already defined.
#endif
/** \brief SYNC_TO_GATEWAY bit (Bit 2) in StbM_TimeBaseStatusType
 **
 ** 0x00: Local Time Base is synchronous to Global Time Master
 ** 0x01: Local Time Base updates are based on a Time Gateway below the Global Time Master
 */
#define STBM_SYNC_TO_GATEWAY_FLAG   0x4U

#if (defined STBM_GLOBAL_TIME_BASE_FLAG)
#error STBM_GLOBAL_TIME_BASE_FLAG is already defined.
#endif
/** \brief GLOBAL_TIME_BASE bit (Bit 3) in StbM_TimeBaseStatusType
 **
 ** 0x00: Local Time Base is based on Local Time Base reference clock only (never
 **       synchronized with Global Time Base)
 ** 0x01: Local Time Base was at least synchronized with Global Time Base one time
 */
#define STBM_GLOBAL_TIME_BASE_FLAG  0x8U

/*
 * Status notification events
 */
#if (defined STBM_EV_GLOBAL_TIME)
#error STBM_EV_GLOBAL_TIME is already defined.
#endif
/** \brief Status notification event EV_GLOBAL_TIME
 **
 ** Bit 0 (LSB):
 ** 0: synchronization to global
 ** time master not changed
 ** 1: GLOBAL_TIME_BASE in
 ** StbM_TimeBaseStatusType
 ** has changed from 0 to 1
 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00284.EV_GLOBAL_TIME_BASE,1 */
#define STBM_EV_GLOBAL_TIME              0x01U

#if (defined STBM_EV_TIMEOUT_OCCURRED)
#error STBM_EV_TIMEOUT_OCCURRED is already defined.
#endif
/** \brief Status notification event EV_TIMEOUT_OCCURRED
 **
 ** Bit 1:
 ** 1: TIMEOUT bit in
 ** timeBaseStatus has changed
 ** from 0 to 1
 ** 0: otherwise
 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00284.EV_TIMEOUT_OCCURED,1 */
#define STBM_EV_TIMEOUT_OCCURRED         0x02U

#if (defined STBM_EV_TIMEOUT_REMOVED)
#error STBM_EV_TIMEOUT_REMOVED is already defined.
#endif
/** \brief Status notification event EV_TIMEOUT_REMOVED
 **
 ** Bit 2
 ** 1: TIMEOUT bit in
 ** timeBaseStatus has changed
 ** from 1 to 0
 ** 0: otherwise
 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00284.EV_TIMEOUT_REMOVED,1 */
#define STBM_EV_TIMEOUT_REMOVED          0x04U

#if (defined STBM_EV_TIMELEAP_FUTURE)
#error STBM_EV_TIMELEAP_FUTURE is already defined.
#endif
/** \brief Status notification event EV_TIMELEAP_FUTURE
 **
 ** Bit 3
 ** 1: TIMELEAP_FUTURE bit in
 ** timeBaseStatus has changed
 ** from 0 to 1
 ** 0: otherwise
 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00284.EV_TIMELEAP_FUTURE,1 */
#define STBM_EV_TIMELEAP_FUTURE          0x08U

#if (defined STBM_EV_TIMELEAP_FUTURE_REMOVED)
#error STBM_EV_TIMELEAP_FUTURE_REMOVED is already defined.
#endif
/** \brief Status notification event EV_TIMELEAP_FUTURE_REMOVED
 **
 ** Bit 4
 ** 1: TIMELEAP_FUTURE bit in
 ** timeBaseStatus has changed
 ** from 1 to 0
 ** 0: otherwise
 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00284.EV_TIMELEAP_FUTURE_REMOVED,1 */
#define STBM_EV_TIMELEAP_FUTURE_REMOVED  0x10U

#if (defined STBM_EV_TIMELEAP_PAST)
#error STBM_EV_TIMELEAP_PAST is already defined.
#endif
/** \brief Status notification event EV_TIMELEAP_PAST
 **
 ** Bit 5
 ** 1: TIMELEAP_PAST bit in
 ** timeBaseStatus has changed
 ** from 0 to 1
 ** 0: otherwise
 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00284.EV_TIMELEAP_PAST,1 */
#define STBM_EV_TIMELEAP_PAST            0x20U

#if (defined STBM_EV_TIMELEAP_PAST_REMOVED)
#error STBM_EV_TIMELEAP_PAST_REMOVED is already defined.
#endif
/** \brief Status notification event EV_TIMELEAP_PAST_REMOVED
 **
 ** Bit 6
 ** 1: TIMELEAP_PAST bit in
 ** timeBaseStatus has changed
 ** from 1 to 0
 ** 0: otherwise
 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00284.EV_TIMELEAP_PAST_REMOVED,1 */
#define STBM_EV_TIMELEAP_PAST_REMOVED    0x40U

#if (defined STBM_EV_SYNC_TO_SUBDOMAIN)
#error STBM_EV_SYNC_TO_SUBDOMAIN is already defined.
#endif
/** \brief Status notification event EV_SYNC_TO_SUBDOMAIN
 **
 ** Bit 7
 ** 1: SYNC_TO_GATEWAY bit
 ** in timeBaseStatus has
 ** changed from 0 to 1
 ** 0: otherwise
 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00284.EV_SYNC_TO_SUBDOMAIN,1 */
#define STBM_EV_SYNC_TO_SUBDOMAIN        0x80U

#if (defined STBM_EV_SYNC_TO_GLOBAL_MASTER)
#error STBM_EV_SYNC_TO_GLOBAL_MASTER is already defined.
#endif
/** \brief Status notification event EV_SYNC_TO_GLOBAL_MASTER
 **
 ** Bit 8
 ** 1: SYNC_TO_GATEWAY bit
 ** of Time Domain changes
 ** from 1 to 0
 ** 0: otherwise
 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00284.EV_SYNC_TO_GLOBAL_MASTER,1 */
#define STBM_EV_SYNC_TO_GLOBAL_MASTER    0x100U


#if (defined STBM_EV_RESYNC)
#error STBM_EV_RESYNC is already defined.
#endif
/** \brief Status notification event STBM_EV_RESYNC
 **
 ** Bit 9
 ** 1: A synchronization of the local time to the valid Global Time value has occurred
 ** 0: No resynchronization event occurred
 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00284.EV_RESYNC,1 */
#define STBM_EV_RESYNC    0x0200U

#if (defined STBM_EV_RATECORRECTION)
#error STBM_EV_RATECORRECTION is already defined.
#endif
/** \brief Status notification event STBM_EV_RATECORRECTION
 **
 ** Bit 10
 ** 1: A valid rate correction has been calculated (not beyond limits)
 ** 0: No rate correction calculated
 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00284.EV_RATECORRECTION.Master,1 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00284.EV_RATECORRECTION.Normal.SlaveRateCorrection,1 */
#define STBM_EV_RATECORRECTION    0x0400U


/*==[Types]===================================================================*/

/*-- Rte related types -------------------------------------------------------*/

#ifndef RTE_TYPE_StbM_SynchronizedTimeBaseType
/** \brief Enables the use of StbM's own definition for "StbM_SynchronizedTimeBaseType" type.*/
#define RTE_TYPE_StbM_SynchronizedTimeBaseType

/** \brief Represent the kind of synchronized time-base
 **
 ** Variables of this type are used to represent the kind of
 ** synchronized time-base.
 */
 /* !LINKSTO StbM.ASR44.SWS_StbM_00142,1 */
typedef VAR(uint16, TYPEDEF) StbM_SynchronizedTimeBaseType;

#endif /* RTE_TYPE_StbM_SynchronizedTimeBaseType */

#ifndef RTE_TYPE_StbM_TimeBaseStatusType
/** \brief Enables the use of StbM's own definition for "StbM_TimeBaseStatusType" type.*/
#define RTE_TYPE_StbM_TimeBaseStatusType

/** \brief Represents the time base status
 **
 ** Bit 6 and 7 are always 0 (reserved for future usage)
 **
 ** Variables of this type are used to express if and how a Local Time Base is
 ** synchronized to the Global Time Master. The type is a bit field of individual status bits,
 ** although not every combination is possible, i.e. any of the bits TIMEOUT,
 ** TIMELEAP_FUTURE, TIMELEAP_PAST and SYNC_TO_GATEWAY can only be set if
 ** the GLOBAL_TIME_BASE bit is set.
 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00239,1 */
typedef VAR(uint8, TYPEDEF) StbM_TimeBaseStatusType;

#endif /* RTE_TYPE_StbM_TimeBaseStatusType */

#ifndef RTE_TYPE_StbM_TimeStampType
/** \brief Enables the use of StbM's own definition for "StbM_TimeStampType" type.*/
#define RTE_TYPE_StbM_TimeStampType

/** \brief Type to represent long time intervals
 **
 ** Variables of this type are used for expressing time stamps including relative
 ** time and absolute calendar time.
 ** The absolute time starts from 1970-01-01 acc. to "[17], Annex C/C1" as specified for PTP.
 ** 0 to 281474976710655s == 3257812230d [0xFFFF FFFF FFFF]
 ** 0 to 999999999ns
 ** [0x3B9A C9FF]
 ** invalid value in nanoseconds: [0x3B9A CA00] to [0x3FFF FFFF]
 ** Bit 30 and 31 reserved, default: 0
 */
 /* !LINKSTO StbM.ASR44.SWS_StbM_00241,1 */
typedef struct
{
  VAR(StbM_TimeBaseStatusType, TYPEDEF) timeBaseStatus;  /**< status of the time base */
  VAR(uint32, TYPEDEF) nanoseconds;  /**< Nanoseconds part of the time */
  VAR(uint32, TYPEDEF) seconds;      /**< 32 bit LSB of the 48 bits Seconds part of the time */
  VAR(uint16, TYPEDEF) secondsHi;    /**< 16 bit MSB of the 48 bits Seconds part of the time */
} StbM_TimeStampType;

#endif /* RTE_TYPE_StbM_TimeStampType */


#if (STBM_GETCURRENTTIMEEXTENDED_USED == STD_ON)
#ifndef RTE_TYPE_StbM_TimeStampExtendedType
/** \brief Enables the use of StbM's own definition for "StbM_TimeStampExtendedType" type.*/
#define RTE_TYPE_StbM_TimeStampExtendedType

/** \brief Type to represent long time intervals
 **
 ** Variables of this type are used for expressing time stamps including relative time and absolute calendar time.
 ** The absolute time starts from 1970-01-01.
 ** Start of absolute time (1970-01-01) is according to [17], Annex C/C1 (refer to parameter "approximate epoch" for PTP)
 ** 0 to 999999999ns
 ** 0 to 18446744073709551615s == 21350398233460d [0xFFFF FFFF FFFF FFFF]
 ** [0x3B9A C9FF]
 ** invalid value in nanoseconds: [0x3B9A CA00] to [0x3FFF FFFF]
 ** Bit 30 and 31 reserved, default: 0
 */
 /* !LINKSTO StbM.ASR44.SWS_StbM_00242,1 */
typedef struct
{
  VAR(StbM_TimeBaseStatusType, TYPEDEF) timeBaseStatus;  /**< status of the time base */
  VAR(uint32, TYPEDEF) nanoseconds;  /**< Nanoseconds part of the time */
  VAR(uint64, TYPEDEF) seconds;      /**< 48 bit Seconds part of the time */
} StbM_TimeStampExtendedType;

#endif /* RTE_TYPE_StbM_TimeStampExtendedType */
#endif /* (STBM_GETCURRENTTIMEEXTENDED_USED == STD_ON) */


#ifndef RTE_TYPE_StbM_UserDataType
/** \brief Enables the use of StbM's own definition for "StbM_UserDataType" type.*/
#define RTE_TYPE_StbM_UserDataType

/** \brief Current user data of the time base */
/* !LINKSTO StbM.ASR44.SWS_StbM_00243,1 */
typedef struct
{
  VAR(uint8, TYPEDEF) userDataLength;  /**< User Data Length in bytes */
  VAR(uint8, TYPEDEF) userByte0;       /**< User Byte 0 */
  VAR(uint8, TYPEDEF) userByte1;       /**< User Byte 1 */
  VAR(uint8, TYPEDEF) userByte2;       /**< User Byte 2 */
} StbM_UserDataType;

#endif /* RTE_TYPE_StbM_UserDataType */

#ifndef RTE_TYPE_StbM_RateDeviationType
/** \brief Enables the use of StbM's own definition for "StbM_RateDeviationType" type.*/
#define RTE_TYPE_StbM_RateDeviationType

/* !LINKSTO StbM.ASR44.SWS_StbM_00301,1 */
/** \brief Variables of this type are used to express a rate deviation in ppm.*/
typedef VAR(sint16, TYPEDEF) StbM_RateDeviationType;

#endif /* RTE_TYPE_StbM_RateDeviationType */

/** \brief StbM configuration data type */
/* !LINKSTO StbM.ASR44.SWS_StbM_00249,1 */
typedef struct
{
  VAR(uint8, TYPEDEF) ConfigSet;
}StbM_ConfigType;


#ifndef RTE_TYPE_StbM_TimeBaseNotificationType
/** \brief Enables the use of StbM's own definition for "StbM_TimeBaseNotificationType" type.*/
#define RTE_TYPE_StbM_TimeBaseNotificationType

/** \brief Represents the time base notification type
 **
 ** Variables of this type are used to represent the number of global time related
 ** events. The type definition is used for storing the events in the status variable
 ** NotificationEvents and for setting the mask variable NotificationMask which defines a
 ** subset of events for which an interrupt request shall be raised.
 */
/* !LINKSTO StbM.ASR44.SWS_StbM_00287,1 */
typedef VAR(uint32, TYPEDEF) StbM_TimeBaseNotificationType;

#endif /* RTE_TYPE_StbM_TimeBaseNotificationType */



#ifndef RTE_TYPE_StbM_SyncRecordTableHeadType
/** \brief Enables the use of StbM's own definition for "StbM_SyncRecordTableHeadType" type.*/
#define RTE_TYPE_StbM_SyncRecordTableHeadType

/** \brief Synchronized Time Base Record Table Header */
/* !LINKSTO StbM.ASR44.SWS_StbM_00331,1 */
typedef struct
{
  VAR(uint8, TYPEDEF) SynchronizedTimeDomain;    /**< Time Domain 0..15 */
  VAR(uint32, TYPEDEF) HWfrequency;              /**< HW Frequency in Hz */
  VAR(uint32, TYPEDEF) HWprescaler;              /**< Prescaler value */
}StbM_SyncRecordTableHeadType;

#endif /* RTE_TYPE_StbM_SyncRecordTableHeadType */


#ifndef RTE_TYPE_StbM_SyncRecordTableBlockType
/** \brief Enables the use of StbM's own definition for "StbM_SyncRecordTableBlockType" type.*/
#define RTE_TYPE_StbM_SyncRecordTableBlockType

/** \brief Synchronized Time Base Record Table Block */
/* !LINKSTO StbM.ASR44.SWS_StbM_00332,1 */
typedef struct
{
  VAR(uint32, TYPEDEF) GlbSeconds;                       /**< Seconds of the Local Time Base
                                                              directly after synchronization with
                                                              the Global Time Base */
  VAR(uint32, TYPEDEF) GlbNanoSeconds;                   /**< Nanoseconds of the Local Time Base
                                                              directly after synchronization with
                                                              the Global Time Base */
  VAR(StbM_TimeBaseStatusType, TYPEDEF) TimeBaseStatus;  /**< Time Base Status of the Local Time
                                                              Base directly after synchronization
                                                              with the Global Time Base */
  VAR(uint32, TYPEDEF) VirtualLocalTimeLow;              /**< HW counter reference value directly
                                                              after synchronization with the Global
                                                              Time Base */
  VAR(StbM_RateDeviationType, TYPEDEF) RateDeviation;    /**< Calculated Rate Deviation directly
                                                              after rate deviation measurement */
  VAR(uint32, TYPEDEF) LocSeconds;                       /**< Seconds of the Local Time Base
                                                              directly before synchronization with
                                                              the Global Time Base */
  VAR(uint32, TYPEDEF) LocNanoSeconds;                   /**< Nanoseconds of the Local Time Base
                                                              directly before synchronization with
                                                              the Global Time Base */
  VAR(uint32, TYPEDEF) PathDelay;                        /**< Current propagation delay in
                                                              nanoseconds */
}StbM_SyncRecordTableBlockType;

#endif /* RTE_TYPE_StbM_SyncRecordTableBlockType */




#ifndef RTE_TYPE_StbM_OffsetRecordTableHeadType
/** \brief Enables the use of StbM's own definition for "StbM_OffsetRecordTableHeadType" type.*/
#define RTE_TYPE_StbM_OffsetRecordTableHeadType

/** \brief Offset Time Base Record Table Header */
/* !LINKSTO StbM.ASR44.SWS_StbM_00333,1 */
typedef struct
{
  VAR(uint8, TYPEDEF) OffsetTimeDomain;         /**< Time Domain 16..31 */
}StbM_OffsetRecordTableHeadType;

#endif /* RTE_TYPE_StbM_OffsetRecordTableHeadType */


#ifndef RTE_TYPE_StbM_OffsetRecordTableBlockType
/** \brief Enables the use of StbM's own definition for "StbM_OffsetRecordTableBlockType" type.*/
#define RTE_TYPE_StbM_OffsetRecordTableBlockType

/** \brief Offset Time Base Record Table Block */
/* !LINKSTO StbM.ASR44.SWS_StbM_00334,1 */
typedef struct
{
  VAR(uint32, TYPEDEF) GlbSeconds;                       /**< Seconds of the Offset Time Base */
  VAR(uint32, TYPEDEF) GlbNanoSeconds;                   /**< Nanoseconds of the Offset Time
                                                              Base */
  VAR(StbM_TimeBaseStatusType, TYPEDEF) TimeBaseStatus;  /**< Time Base Status of the Local Time
                                                              Base directly after synchronization
                                                              with the Global Time Base */
}StbM_OffsetRecordTableBlockType;

#endif /* RTE_TYPE_StbM_OffsetRecordTableBlockType */




/** \brief Structure which contains additional measurement data */
/* !LINKSTO StbM.ASR44.SWS_StbM_00384,1 */
typedef struct
{
  VAR(uint32, TYPEDEF) pathDelay;         /**< Propagation delay in nanoseconds */
}StbM_MeasurementType;


#ifndef RTE_TYPE_StbM_VirtualLocalTimeType
/** \brief Enables the use of StbM's own definition for "StbM_VirtualLocalTimeType" type.*/
#define RTE_TYPE_StbM_VirtualLocalTimeType
/** \brief Represent the kind of synchronized time-base
 **
 ** Variables of this type store time stamps of the Virtual
 ** Local Time. The unit is nanoseconds.
 */
 /* !LINKSTO StbM.ASR44.SWS_StbM_91003,1 */
typedef struct
{
  VAR(uint32, TYPEDEF) nanosecondsLo;  /**< Least significant 32 bits of the 64 bit Virtual Local Time */
  VAR(uint32, TYPEDEF) nanosecondsHi;  /**< Most significant 32 bits of the 64 bit Virtual Local Time */
} StbM_VirtualLocalTimeType;

#endif /* RTE_TYPE_StbM_VirtualLocalTimeType */

/** \brief Represent a Time Tuple
 **
 ** Variables of this type store a Time Tuple containing the Global Time and
 ** the associated Virtual Local Time.
 */
typedef struct
{
  VAR(StbM_TimeStampType, TYPEDEF) globalTime;  /**< Global Time */
  VAR(StbM_VirtualLocalTimeType, TYPEDEF) virtualLocalTime;  /**< Associated Virtual Local Time */
} StbM_TimeTupleType;

#ifndef RTE_TYPE_StbM_TimeDiffType
/** \brief Enables the use of StbM's own definition for "StbM_TimeDiffType" type.*/
#define RTE_TYPE_StbM_TimeDiffType

/** \brief Offset Time Base Record Table Header */
/* !LINKSTO StbM.ASR19-11.SWS_StbM_00300,1 */
typedef sint32 StbM_TimeDiffType;

#endif /* RTE_TYPE_StbM_TimeDiffType */

#ifndef RTE_TYPE_StbM_CustomerIdType
/** \brief Enables the use of StbM's own definition for "StbM_CustomerIdType" type.*/
#define RTE_TYPE_StbM_CustomerIdType

/** \brief Unique identifier of a notification customer */
/* !LINKSTO StbM.ASR19-11.SWS_StbM_00288,1 */
typedef uint16 StbM_CustomerIdType;

#endif /* RTE_TYPE_StbM_CustomerIdType */




#ifndef RTE_TYPE_StbM_MasterConfigType
/** \brief Enables the use of StbM's own definition for "StbM_MasterConfigType" type. */
#define RTE_TYPE_StbM_MasterConfigType

/** \brief MasterConfigType */
/* !LINKSTO StbM.ASR44.SWS_StbM_91001,1 */
typedef uint8 StbM_MasterConfigType;

#endif /* RTE_TYPE_StbM_MasterConfigType */


/*==[Declaration of functions with internal linkage]==========================*/

/*==[Constants with internal linkage]=========================================*/

/*==[Variables with internal linkage]=========================================*/

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/

/*==[Definition of functions with internal linkage]===========================*/

/*==================[end of file]===========================================*/


#endif /* ifndef STBM_TYPES_H */
