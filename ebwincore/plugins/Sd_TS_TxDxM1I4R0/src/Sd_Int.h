#ifndef SD_INT_H_
#define SD_INT_H_

/**
 * \file
 *
 * \brief AUTOSAR Sd
 *
 * This file contains the implementation of the AUTOSAR
 * module Sd.
 *
 * \version 1.4.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*  MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 *  The # and ## preprocessor operators should not be used.
 *
 *   Reason:
 *   The ## preprocessor operator is used to generate read and write macros
 *   for message handling.
 *
 *  MISRAC2012-2) Deviated Rule: 12.2 (required)
 *  The right hand operand of a shift operator shall lie in the range zero to one less
 *  than the width in bits of the essential type of the left hand operand.
 *
 *   Reason:
 *   Temporarily justified due to a bug in Polyspace 2017b.
 */

/*==================[inclusions]============================================*/
#include <Sd_Cfg.h>
#include <Sd_Types.h>
#include <Sd_Trace.h>

#include <TSAutosar.h>

/*==================[macros]================================================*/

#if (defined SD_UNINITED)
#error SD_UNINITED is already defined
#endif

#define SD_UNINITED 0U


#if (defined SD_INITED)
#error SD_INITED is already defined
#endif

#define SD_INITED 1U

#define SD_OPTION_STATUS_OK 0x00U

#define SD_OPTION_STATUSFLAG_ERROR           0x01U
#define SD_OPTION_STATUSFLAG_CONFLICT        0x02U
#define SD_OPTION_STATUSFLAG_INVALIDENDPOINT 0x04U

#define SD_TIMER_IMMEDIATE 1U

#define SD_TTL_OFF                               0U
#define SD_TTL_ALWAYS_ON                         1U
#define SD_TTL_TIMED_OUT                         SD_TTL_ALWAYS_ON

#define SD_INCREASE_CLIENT_OFFER_EVENT  0x00010000U

#define DEM_ENTRY_NOT_FOUND_BITMAP            0x01U
#define DEM_OUT_OF_RESOURCES_UDP_BITMAP       0x02U
#define DEM_OUT_OF_RESOURCES_TCP_BITMAP       0x04U
#define DEM_MALFORMED_MSG_BITMAP              0x08U

#if (SD_DEV_ERROR_DETECT == STD_ON)

#include <Det.h>            /* Det API */

/** \brief Macro to report DET error with ApiId and Error Id */
#define SD_DET_REPORT_ERROR(ApiId, ErrorId)                      \
  (void) Det_ReportError(SD_MODULE_ID, 0U, (ApiId), (ErrorId))

#else

/** \brief Macro to report DET error with ApiId and Error Id */
#define SD_DET_REPORT_ERROR(ApiId, ErrorId)      \
  do {} while (0)

#endif /* (SD_DEV_ERROR_DETECT == STD_ON) */

#if (SD_DEM_DETECT == STD_ON)
/* !LINKSTO Sd.ASR40.SWS_SD_00003,1 */
/* !LINKSTO Sd.ASR40.SWS_SD_00106,1 */
#include <Dem.h>

/** \brief Macro to report DET error with ApiId and Error Id */
#define SD_DEM_REPORT_ERROR_STATUS(EventId, EventStatus)                       \
  do                                                                           \
  {                                                                            \
    if((EventId) != SD_INVALID_DEM_EVENTID)                                      \
    {                                                                          \
      Dem_ReportErrorStatus((EventId), (EventStatus));                         \
    }                                                                          \
  }while(0)

#endif /* SD_DEM_DETECT == STD_ON */

#if (SD_RELOCATABLE_CFG_ENABLE == STD_ON)

#if (defined TS_RELOCATABLE_CFG_ENABLE)
#error TS_RELOCATABLE_CFG_ENABLE is already defined
#endif
#define TS_RELOCATABLE_CFG_ENABLE STD_ON

#include <TSPBConfig.h>        /* post-build config access macros */

#if (defined SD_CFG)
#error SD_CFG is already defined
#endif
#define SD_CFG(type, name, id) \
    (TS_UNCHECKEDGETCFG(SdConfigPtr, type, SD, SdConfigPtr->name) [(id)])

#if (defined SD_CFG_REF)
#error SD_CFG_REF is already defined
#endif
#define SD_CFG_REF(type, name, id) \
    (TS_UNCHECKEDGETCFG(SdConfigPtr, type, SD, SdConfigPtr->name) [0])

#else

#if (defined SD_CFG)
#error SD_CFG is already defined
#endif
#define SD_CFG(type, name, id)  ((const type*)SdConfigPtr->name)[(id)]

#if (defined SD_CFG_REF)
#error SD_CFG_REF is already defined
#endif
#define SD_CFG_REF(type, name, id)  SdConfigPtr->name[0]

#endif /* SD_RELOCATABLE_CFG_ENABLE */

#define SD_FLAG_REBOOT                0x80U
#define SD_FLAG_UNICAST               0x40U

#define SD_EXACT_OR_ANY_MINOR_VERSION 0U
#define SD_MINIMUM_MINOR_VERSION      1U

#define SD_WILDCARD_SERVICEID         0xFFFFU
#define SD_WILDCARD_INSTANCEID        0xFFFFU
#define SD_WILDCARD_MAJORVERSION      0xFFU
#define SD_WILDCARD_MINORVERSION      0xFFFFFFFFU
#define SD_WILDCARD_EVENTGROUPID      0xFFFFU

#define SD_OPTIONTYPE_CONFIGURATION   0x01U
#define SD_OPTIONTYPE_IPV4            0x04U
#define SD_OPTIONTYPE_IPV6            0x06U
#define SD_OPTIONTYPE_IPV4MULTI       0x14U
#define SD_OPTIONTYPE_IPV6MULTI       0x16U

#define SD_DOMAIN_TCP                 0x06U
#define SD_DOMAIN_UDP                 0x11U

#define SD_CLIENTID                   0x00U
#define SD_PROTOCOLVERSION            0x01U
#define SD_INTERFACEVERSION           0x01U
#define SD_MESSAGETYPE                0x02U
#define SD_RETURNCODE                 0x00U

#define SD_ENTRYTYPE_FINDSERVICE             0x00U
#define SD_ENTRYTYPE_OFFERSERVICE            0x01U
#define SD_ENTRYTYPE_SUBSCRIBEEVENTGROUP     0x06U
#define SD_ENTRYTYPE_SUBSCRIBEEVENTGROUPACK  0x07U

#define SD_ENTRYTTL_INFINITE          0xFFFFFFU

/* Header */
#define SD_HEADER_SIZE 12U

#define SD_HEADER_ENTRYLEN_SIZE 4U
#define SD_HEADER_OPTSLEN_SIZE 4U

#define SD_ENTRY_SIZE 16U


#define SD_HEADER_CLIENTID_OFFSET 0U
#define SD_HEADER_CLIENTID_LENGTH 16U

#define SD_HEADER_SESSIONID_OFFSET 2U
#define SD_HEADER_SESSIONID_LENGTH 16U

#define SD_HEADER_PROTOCOLVERSION_OFFSET 4U
#define SD_HEADER_PROTOCOLVERSION_LENGTH 8U

#define SD_HEADER_INTERFACEVERSION_OFFSET 5U
#define SD_HEADER_INTERFACEVERSION_LENGTH 8U

#define SD_HEADER_MESSAGETYPE_OFFSET 6U
#define SD_HEADER_MESSAGETYPE_LENGTH 8U

#define SD_HEADER_RETURNCODE_OFFSET 7U
#define SD_HEADER_RETURNCODE_LENGTH 8U

#define SD_HEADER_FLAGS_OFFSET 8U
#define SD_HEADER_FLAGS_LENGTH 8U

#define SD_HEADER_RESERVED_OFFSET 9U
#define SD_HEADER_RESERVED_LENGTH 24U

/* Common Entry */
#define SD_ENTRY_TYPE_OFFSET 0U
#define SD_ENTRY_TYPE_LENGTH 8U

#define SD_ENTRY_INDEX1OP_OFFSET 1U
#define SD_ENTRY_INDEX1OP_LENGTH 8U

#define SD_ENTRY_INDEX2OP_OFFSET 2U
#define SD_ENTRY_INDEX2OP_LENGTH 8U

#define SD_ENTRY_NUMOPTS_OFFSET 3U
#define SD_ENTRY_NUMOPTS_LENGTH 8U

#define SD_ENTRY_SERVICEID_OFFSET 4U
#define SD_ENTRY_SERVICEID_LENGTH 16U

#define SD_ENTRY_INSTANCEID_OFFSET 6U
#define SD_ENTRY_INSTANCEID_LENGTH 16U

#define SD_ENTRY_MAJORVERSION_OFFSET 8U
#define SD_ENTRY_MAJORVERSION_LENGTH 8U

#define SD_ENTRY_TTL_OFFSET 9U
#define SD_ENTRY_TTL_LENGTH 24U

/* Type 1 entry */
#define SD_ENTRYT1_MINORVERSION_OFFSET 12U
#define SD_ENTRYT1_MINORVERSION_LENGTH 32U

/* Type 2 entry */
#define SD_ENTRYT2_RESERVED2_OFFSET 12U
#define SD_ENTRYT2_RESERVED2_LENGTH 16U

#define SD_ENTRYT2_COUNTER_MASK   0x0FU

#define SD_ENTRYT2_EVENTGROUPID_OFFSET 14U
#define SD_ENTRYT2_EVENTGROUPID_LENGTH 16U

/* Common Option */
#define SD_OPTIONCOMMON_SIZE 3U

#define SD_OPTIONCOMMON_LENGTH_OFFSET 0U
#define SD_OPTIONCOMMON_LENGTH_LENGTH 16U

#define SD_OPTIONCOMMON_TYPE_OFFSET 2U
#define SD_OPTIONCOMMON_TYPE_LENGTH 8U

/* IPv4 Option */
#define SD_OPTIONIPV4_SIZE 12U

#define SD_OPTIONIPV4_RESERVED1_OFFSET 3U
#define SD_OPTIONIPV4_RESERVED1_LENGTH 8U

#define SD_OPTIONIPV4_ADDRESS_OFFSET 4U
#define SD_OPTIONIPV4_ADDRESS_LENGTH 32U

#define SD_OPTIONIPV4_RESERVED2_OFFSET 8U
#define SD_OPTIONIPV4_RESERVED2_LENGTH 8U

#define SD_OPTIONIPV4_PROTOCOL_OFFSET 9U
#define SD_OPTIONIPV4_PROTOCOL_LENGTH 8U

#define SD_OPTIONIPV4_PORT_OFFSET 10U
#define SD_OPTIONIPV4_PORT_LENGTH 16U

/* IPv6 Option */
#define SD_OPTIONIPV6_SIZE 24U

#define SD_OPTIONIPV6_RESERVED1_OFFSET 3U
#define SD_OPTIONIPV6_RESERVED1_LENGTH 8U

#define SD_OPTIONIPV6_ADDRESS_OFFSET 4U
#define SD_OPTIONIPV6_ADDRESS_LENGTH 128U

#define SD_OPTIONIPV6_RESERVED2_OFFSET 20U
#define SD_OPTIONIPV6_RESERVED2_LENGTH 8U

#define SD_OPTIONIPV6_PROTOCOL_OFFSET 21U
#define SD_OPTIONIPV6_PROTOCOL_LENGTH 8U

#define SD_OPTIONIPV6_PORT_OFFSET 22U
#define SD_OPTIONIPV6_PORT_LENGTH 16U

/* Config Option */
#define SD_OPTIONCONFIG_COMMONSIZE 4U
#define SD_OPTIONCONFIG_MINSIZE 5U

#define SD_OPTIONCONFIG_RESERVED_OFFSET 3U
#define SD_OPTIONCONFIG_RESERVED_LENGTH 8U

#define SD_OPTIONCONFIG_STRING_OFFSET 4U
#define SD_OPTIONCONFIG_STRINGLENGTH_LENGTH 1U

#define SD_OPTIONCONFIG_SIZE(StringLength) (StringLength) + SD_OPTIONCONFIG_COMMONSIZE

/* Multicast IPv4-Address verification values */
#define SD_IPV4_MULTICAST_1STOCT_MASK  0xF0U
#define SD_IPV4_MULTICAST_1STOCT_VALUE 0xE0U


/* Multicast IPv6-Address verification values */
#define SD_IPV6_MULTICAST_1STWORD_MASK  0xFFU
#define SD_IPV6_MULTICAST_1STWORD_VALUE 0xFFU

/* A single configuration option per entry is supported */
#define SD_OPTIONCONFIG_ARY_SIZE 2U

/* Deviation MISRAC2012-1 */
#define SD_ENTRY_OFFSET(entry) entry##_OFFSET
#define SD_ENTRY_LENGTH(entry) entry##_LENGTH


/* Write macros */
#define SD_WRITE(entry,target,value)  SD_WRITEINT(SD_ENTRY_OFFSET(entry),target,SD_ENTRY_LENGTH(entry),value)

#define SD_WRITEINT(offset,target,length,value) SD_WRITEINT2(offset,target,length,value)
/* Deviation MISRAC2012-1 */
#define SD_WRITEINT2(offset,target,length,value) SD_WRITE##length((offset),target,value)

#define SD_WRITE8U(offset,target,value) do { \
  (target)[(offset)] = (uint8)(value);                \
}                                            \
while (0)

/* Deviation MISRAC2012-2 */
#define SD_WRITE16U(offset,target,value) do {       \
  (target)[(offset) + 0U] =  (uint8)(((value) >>  8) & 0xFFU);    \
  (target)[(offset) + 1U] =  (uint8)(((value) >>  0) & 0xFFU);    \
}                                           \
while (0)

#define SD_WRITE24U(offset,target,value) do {       \
  (target)[(offset) + 0U] =  (uint8)(((value) >> 16) & 0xFFU);    \
  (target)[(offset) + 1U] =  (uint8)(((value) >>  8) & 0xFFU);    \
  (target)[(offset) + 2U] =  (uint8)(((value) >>  0) & 0xFFU);    \
}                                           \
while (0)

#define SD_WRITE32U(offset,target,value) do {       \
  (target)[(offset) + 0U] =  (uint8)(((value) >> 24) & 0xFFU);    \
  (target)[(offset) + 1U] =  (uint8)(((value) >> 16) & 0xFFU);    \
  (target)[(offset) + 2U] =  (uint8)(((value) >>  8) & 0xFFU);    \
  (target)[(offset) + 3U] =  (uint8)(((value) >>  0) & 0xFFU);    \
}                                           \
while (0)

#define SD_WRITE32U_NE(offset,target,value) do {       \
  CONSTP2VAR(uint8,AUTOMATIC,SD_VAR) SourcePtr = (uint8*)&(value); \
  (target)[(offset) + 0U] = SourcePtr[0U];        \
  (target)[(offset) + 1U] = SourcePtr[1U];        \
  (target)[(offset) + 2U] = SourcePtr[2U];        \
  (target)[(offset) + 3U] = SourcePtr[3U];        \
}                                           \
while (0)

#define SD_WRITE128U_NE(offset,target,value) do {       \
  CONSTP2VAR(uint8,AUTOMATIC,SD_VAR) SourcePtr = (uint8*)&(value); \
  (target)[(offset) + 0U] = SourcePtr[0U];        \
  (target)[(offset) + 1U] = SourcePtr[1U];        \
  (target)[(offset) + 2U] = SourcePtr[2U];        \
  (target)[(offset) + 3U] = SourcePtr[3U];        \
  (target)[(offset) + 4U] = SourcePtr[4U];        \
  (target)[(offset) + 5U] = SourcePtr[5U];        \
  (target)[(offset) + 6U] = SourcePtr[6U];        \
  (target)[(offset) + 7U] = SourcePtr[7U];        \
  (target)[(offset) + 8U] = SourcePtr[8U];        \
  (target)[(offset) + 9U] = SourcePtr[9U];        \
  (target)[(offset) + 10U] = SourcePtr[10U];        \
  (target)[(offset) + 11U] = SourcePtr[11U];        \
  (target)[(offset) + 12U] = SourcePtr[12U];        \
  (target)[(offset) + 13U] = SourcePtr[13U];        \
  (target)[(offset) + 14U] = SourcePtr[14U];        \
  (target)[(offset) + 15U] = SourcePtr[15U];        \
}                                           \
while (0)

/* Read macros */
#define SD_READ(entry,source,value)  SD_READINT(SD_ENTRY_OFFSET(entry),(source),SD_ENTRY_LENGTH(entry),(value))

#define SD_READINT(offset,source,length,value) SD_READINT2(offset,source,length,value)
/* Deviation MISRAC2012-1 */
#define SD_READINT2(offset,source,length,value) SD_READ##length((offset),source,value)

#define SD_READ8U(offset,source,value) do {        \
  (value) = (source)[(offset)];                    \
}                                                  \
while (0)

#define SD_READ16U(offset,source,value) do {        \
  (value) =                                         \
   (uint16)((uint16)((source)[(offset) + 0U]) << 8) \
   |        (uint16)((source)[(offset) + 1U]);      \
}                                                   \
while (0)

#define SD_READ24U(offset,source,value) do {         \
  (value) =                                          \
   (uint32)((uint32)((source)[(offset) + 0U]) << 16) \
  |(uint32)((uint32)((source)[(offset) + 1U]) <<  8) \
  |         (uint32)((source)[(offset) + 2U]);       \
}                                                    \
while (0)

#define SD_READ32U(offset,source,value) do {         \
  (value) =                                          \
   (uint32)((uint32)((source)[(offset) + 0U]) << 24) \
  |(uint32)((uint32)((source)[(offset) + 1U]) << 16) \
  |(uint32)((uint32)((source)[(offset) + 2U]) <<  8) \
  |         (uint32)((source)[(offset) + 3U]);       \
}                                                    \
while (0)

#define SD_READ32U_NE(offset,source,value) do {      \
  CONSTP2VAR(uint8,AUTOMATIC,SD_VAR) TargetPtr = (uint8*)&(value); \
  TargetPtr[0] = (source)[(offset) + 0U];       \
  TargetPtr[1] = (source)[(offset) + 1U];       \
  TargetPtr[2] = (source)[(offset) + 2U];       \
  TargetPtr[3] = (source)[(offset) + 3U];       \
}                                                    \
while (0)


#define SD_READ128U_NE(offset,source,value) do {      \
  CONSTP2VAR(uint8,AUTOMATIC,SD_VAR) TargetPtr = (uint8*)&(value); \
  TargetPtr[0] = (source)[(offset) + 0U];       \
  TargetPtr[1] = (source)[(offset) + 1U];       \
  TargetPtr[2] = (source)[(offset) + 2U];       \
  TargetPtr[3] = (source)[(offset) + 3U];       \
  TargetPtr[4] = (source)[(offset) + 4U];       \
  TargetPtr[5] = (source)[(offset) + 5U];       \
  TargetPtr[6] = (source)[(offset) + 6U];       \
  TargetPtr[7] = (source)[(offset) + 7U];       \
  TargetPtr[8] = (source)[(offset) + 8U];       \
  TargetPtr[9] = (source)[(offset) + 9U];       \
  TargetPtr[10] = (source)[(offset) + 10U];       \
  TargetPtr[11] = (source)[(offset) + 11U];       \
  TargetPtr[12] = (source)[(offset) + 12U];       \
  TargetPtr[13] = (source)[(offset) + 13U];       \
  TargetPtr[14] = (source)[(offset) + 14U];       \
  TargetPtr[15] = (source)[(offset) + 15U];       \
}                                                    \
while (0)


#define Sd_StartTimer(timerPtr, value) (*(timerPtr)) = (value)
#define Sd_StopTimer(timerPtr) Sd_StartTimer((timerPtr),0U)

/* helper macro to ensure all arrays have a size > 0 */
#define SD_ARRAY_SIZE(COUNT) ((COUNT)?(COUNT):1U)

#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
#define SD_INFINITE_RETRIES  255U
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */

/*==================[type definitions]======================================*/
/* !LINKSTO Sd.Design.Data.Runtime,1 */
typedef struct
{
  uint16 SessionId;
  boolean RebootFlag;
}Sd_RebootStateType;

typedef struct
{
  TcpIp_SockAddrInet6Type Address;
  Sd_RebootStateType RebootState;
}Sd_TxStateType;

typedef struct
{
  Sd_TxStateType TxState;
  Sd_RebootStateType RemoteUnicastRebootState;
  Sd_RebootStateType RemoteMulticastRebootState;
  Sd_TimerType ResponseTimer;
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
  Sd_TimerType RetryTimer;
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
  Sd_TimerType IsConnectionReadyTimer;
  uint16 ServerServicesFindsReceived;
  uint16 ClientServicesOffersReceived;
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
  /* this counter indicates how many client services pro remote node still have
    have consumed event group subscribes sent which have not been acknowledged yet */
  uint16 ClientServicesRetriesToProcess;
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
  boolean IsConnectionReady;
}Sd_RemoteNodeStateType;

typedef struct
{
  uint16 ServiceID;
  uint16 InstanceID;
  uint8 MajorVersion;
}Sd_ServiceInfoType;

typedef P2CONST(uint8, AUTOMATIC, SD_APPL_DATA) SdConfigOptionPtrType;

typedef struct
{
  TcpIp_SockAddrInet6Type MulticastOption;
  TcpIp_SockAddrInet6Type UdpEndpointOption;
  TcpIp_SockAddrInet6Type TcpEndpointOption;
  SdConfigOptionPtrType  ConfigOptions[SD_OPTIONCONFIG_ARY_SIZE];
  uint8 OptionStatus;
}Sd_EntryOptionInfoType;

typedef struct
{
  TcpIp_IpAddrStateType RxMultiCastIpAddrState;
  TcpIp_IpAddrStateType TxIpAddrState;
  Sd_TxStateType TxState;
  uint32 ServersDeltaTTL;
  uint32 ServersDeltaTTLtimer;
  uint32 ClientsDeltaTTL;
  uint32 ClientsDeltaTTLtimer;
  uint32 ClientServicesEventsCount;
  Sd_TimerType ServersDelta;
  Sd_TimerType ServerDeltaTimer;
  Sd_TimerType ClientsDelta;
  Sd_TimerType ClientsDeltaTimer;
  Sd_TimerType ResponseTimersDelta;
  Sd_TimerType ResponseTimersDeltaTimer;
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
  Sd_TimerType RetryTimersDelta;
  Sd_TimerType RetryTimersDeltaTimer;
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
  uint16 ServerServicesTimersActive;
  uint16 ClientServicesTimersActive;
  uint16 ServerServicesEventsCount;
  uint16 ResponseTimersActiveCount;
  boolean Halted;
}Sd_InstanceStateType;

/* !LINKSTO Sd.Design.Data.Runtime,1 */

/*==================[external function declarations]========================*/

#define SD_START_SEC_CODE
#include <Sd_MemMap.h>

TS_MOD_PRIV_DECL FUNC(boolean, SD_CODE) Sd_IsInstanceAddressAssigned(const uint8 instanceIdx);

TS_MOD_PRIV_DECL FUNC(uint32, SD_CODE) Sd_GetRandom(const uint32 MinLimit, const uint32 MaxLimit);

TS_MOD_PRIV_DECL FUNC(Sd_RemoteNodeIdType, SD_CODE) Sd_AssignRemoteNodeState
(
  CONSTP2CONST(TcpIp_SockAddrInet6Type,AUTOMATIC,SD_APPL_CODE) RemoteAddrPtr,
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr
);

TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_ResetRemoteNodeRxState
(
  CONSTP2VAR(Sd_InstanceStateType, AUTOMATIC, SD_APPL_DATA) InstanceStatesPtr,
  const Sd_RemoteNodeIdType RemoteNodeIndex
);

TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_RestartNodeResponseTimer
(
  const Sd_TimerType timerValue,
  const Sd_RemoteNodeIdType nodeIndex,
  const uint8 SdInstanceIndex
);

/** \brief Compare a received configuration string with the configured configuration string
 **
 ** \param[in] receivedConfigStringArray    array of receive configuration strings
 ** \param[in] configuredConfigString       configured configuration string
 ** \param[in] configuredConfigStringLength length of configured configuration
 **                                         string including zero length-terminator
 **
 ** \return Result of function call
 ** \retval TRUE The configuration strings match
 ** \retval FALSE The configuration strings do not match
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
TS_MOD_PRIV_DECL FUNC(boolean, SD_CODE) Sd_CapabilityRecordStrCmp
(
  CONSTP2CONST(SdConfigOptionPtrType, AUTOMATIC, SD_APPL_DATA) receivedConfigStringArray,
  CONSTP2CONST(uint8, AUTOMATIC, SD_APPL_CONST) configuredConfigString,
  const uint16 configuredConfigStringLength
);

/** \brief returns the state information of a remote node
 **
 ** \param[in] RemoteNodeIndex  index of the remote node
 **
 ** \return pointer to state of remote node
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
TS_MOD_PRIV_DECL FUNC_P2VAR(Sd_RemoteNodeStateType, AUTOMATIC, SD_CODE) Sd_GetRemoteNodeState(const Sd_RemoteNodeIdType RemoteNodeIndex);

TS_MOD_PRIV_DECL FUNC(TcpIp_DomainType, SD_CODE) Sd_GetInstanceAddressDomain(const uint8 InstanceIndex);
/** \brief Mainfunction for one Sd instance
 **
 ** \param[in] InstanceIndex Index of ServiceDiscovery instance
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
TS_MOD_PRIV_DECL FUNC(void, SD_CODE) Sd_Instance_MainFunction
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr,
  uint8 InstanceIndex
);

#define SD_STOP_SEC_CODE
#include <Sd_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#define SD_START_SEC_VAR_INIT_8
#include <Sd_MemMap.h>

extern VAR(uint8, SD_VAR) Sd_Initialized;

#define SD_STOP_SEC_VAR_INIT_8
#include <Sd_MemMap.h>



#define SD_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Sd_MemMap.h>

extern P2CONST(Sd_ConfigType,SD_VAR_CLEARED, SD_APPL_CONST) SdConfigPtr;

extern VAR(Sd_InstanceStateType, SD_VAR_CLEARED) Sd_InstanceStates[SD_MAX_INSTANCE_COUNT];

#define SD_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Sd_MemMap.h>


/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* SD_INT_H_ */
