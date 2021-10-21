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
 *  MISRAC2012-1) Deviated Rule: 11.3 (required)
 *  A cast shall not be performed between a pointer to object type and a pointer to a different object type.
 *
 *   Reason:
 *   Pointer must be cast to more generic type
 *
 */

/*==================[inclusions]============================================*/

#include <Sd.h>               /* Module public API */
#include <Sd_Send_Int.h>
#include <SchM_Sd.h>     /* SchM symbols for crit. sections */

/*==================[macros]================================================*/

#define SD_OPTIONQUEUETYPE_SERVERSERVICE_CONFIGURATIONOPTION 0x01
#define SD_OPTIONQUEUETYPE_CLIENTSERVICE_CONFIGURATIONOPTION 0x02
#define SD_OPTIONQUEUETYPE_UDP_ENDPOINTOPTION           0x03
#define SD_OPTIONQUEUETYPE_TCP_ENDPOINTOPTION           0x04
#define SD_OPTIONQUEUETYPE_MULTICAST_ENDPOINTOPTION     0x05

#define SD_OPTIONQUEUE_HASH(Type, Id) (((uint32)(Type) << 24U) | (uint32)(Id))
#define SD_OPTIONQUEUE_HASH_Id(Hash) ((Hash) & 0xFFFFFFU)
#define SD_OPTIONQUEUE_HASH_Type(Hash) ((Hash) >> 24U)
#define SD_OPTIONQUEUE_HASH_INVALID 0U


#define SD_SEND_OPTIONINDEX_INVALID 0xFFU

/*==================[type definitions]======================================*/
/* !LINKSTO Sd.Design.Data.Runtime,1 */
typedef struct
{
  Sd_TxStateType* TxStatePtr;
  uint32 Options[SD_SEND_MAXNUM_OPTIONS];
  uint16 DataEnd;
  uint16 OptionSize;
  uint8 InstanceIndex;
  uint8 OptionEnd;
  uint8 Data[SD_SEND_MAXPAYLOAD];
}Sd_SendBuffer_t;


/*==================[external data]=========================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define SD_START_SEC_CODE
#include <Sd_MemMap.h>


STATIC FUNC(void, SD_CODE) Sd_Send_resetBuffer(void);

STATIC FUNC(void, SD_CODE) Sd_Send_queueOptions(void);

STATIC FUNC(void, SD_CODE) Sd_Send_multicastEndpointOption
(
  TcpIp_DomainType LocalDomain,
  CONSTP2VAR(uint8,AUTOMATIC,SD_APPL_DATA) DataPtr,
  const uint8 Type,
  SoAd_SoConIdType SoConId
);

STATIC FUNC(void, SD_CODE) Sd_Send_writeIpv4EndpointOption
(
  TcpIp_SockAddrInetType *Ip,
  CONSTP2VAR(uint8,AUTOMATIC,SD_APPL_DATA) DataPtr,
  const uint8 Type,
  SoAd_SoConIdType SoConId
);

STATIC FUNC(void, SD_CODE) Sd_Send_writeIpv6EndpointOption
(
  TcpIp_SockAddrInet6Type *Ip,
  CONSTP2VAR(uint8,AUTOMATIC,SD_APPL_DATA) DataPtr,
  const uint8 Type,
  SoAd_SoConIdType SoConId
);

STATIC FUNC(void, SD_CODE) Sd_Send_checkBufferFillLevel(uint16 NumberOfEndpointoptions, uint16 ConfigStringLength);

STATIC FUNC(void, SD_CODE) Sd_Send_appendOption
(
  const uint32 OptionHash,
  const uint16 OptionSize
);

STATIC FUNC(void, SD_CODE) Sd_Send_addOption
(
  CONSTP2VAR(uint8,AUTOMATIC,SD_APPL_DATA) OptionIndexPtr,
  const uint32 Hash,
  const uint16 Size
);

STATIC FUNC(void, SD_CODE) Sd_Send_addOptions
(
  CONSTP2VAR(uint8,AUTOMATIC,SD_APPL_DATA) OptionIndexPtr,
  CONSTP2VAR(uint8,AUTOMATIC,SD_APPL_DATA) OptionCountPtr,
  const uint32 Hash1,
  const uint32 Hash2,
  const uint16 Size
);

STATIC FUNC(void, SD_CODE) Sd_Send_queueSubscribeEventgroupAckNack
(
  const uint32 TTL,
  const uint16 serviceIdx,
  const uint16 eventGroupId,
  const uint16 reservedField,
  const SoAd_SoConIdType MulticastEventSoCon
);



#define SD_STOP_SEC_CODE
#include <Sd_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/
#define SD_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Sd_MemMap.h>

STATIC VAR(Sd_SendBuffer_t, SD_VAR) SendBuffer;

#define SD_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Sd_MemMap.h>

/*==================[external function definitions]=========================*/

#define SD_START_SEC_CODE
#include <Sd_MemMap.h>

/* Module API functions */

/* SW-Unit API functions */

TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_Send_init(void)
{
  SD_WRITE(SD_HEADER_CLIENTID,SendBuffer.Data, (uint16)SD_CLIENTID);
  SD_WRITE(SD_HEADER_PROTOCOLVERSION,SendBuffer.Data, SD_PROTOCOLVERSION);
  SD_WRITE(SD_HEADER_INTERFACEVERSION,SendBuffer.Data, SD_INTERFACEVERSION);
  SD_WRITE(SD_HEADER_MESSAGETYPE,SendBuffer.Data, SD_MESSAGETYPE);
  SD_WRITE(SD_HEADER_RETURNCODE,SendBuffer.Data, SD_RETURNCODE);
  SD_WRITE(SD_HEADER_RESERVED,SendBuffer.Data, 0UL);
  Sd_Send_resetBuffer();
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_Send_setupBuffer
(
  CONSTP2VAR(Sd_TxStateType, AUTOMATIC, SD_APPL_DATA) txStatePtr,
  uint8 instanceIdx
)
{
  SendBuffer.TxStatePtr = txStatePtr;
  SendBuffer.InstanceIndex = instanceIdx;
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_Send_flushBuffer(void)
{
  PduInfoType PduInfo;
  Std_ReturnType RetVal;

  if(SendBuffer.DataEnd > (SD_HEADER_SIZE + SD_HEADER_ENTRYLEN_SIZE))
  {
    P2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr = &SD_CFG_REF(SdInstance_t, SdInstancesRef, 0U);
    CONSTP2VAR(Sd_TxStateType, AUTOMATIC, SD_APPL_DATA) CurrTxStatePtr = SendBuffer.TxStatePtr;
    SoAd_SoConIdType SoConId;

    InstanceCfgPtr =  &InstanceCfgPtr[SendBuffer.InstanceIndex];
    /* write size of entries array */
    {
      uint32 EntriesLength = (uint32)SendBuffer.DataEnd - (SD_HEADER_SIZE + SD_HEADER_ENTRYLEN_SIZE);
      SD_WRITE32U(SD_HEADER_SIZE, SendBuffer.Data, EntriesLength);
    }

    Sd_Send_queueOptions();

    /* write reboot data */
    SD_WRITE(SD_HEADER_SESSIONID, SendBuffer.Data, SendBuffer.TxStatePtr->RebootState.SessionId);
    SD_WRITE(SD_HEADER_FLAGS, SendBuffer.Data, ((SendBuffer.TxStatePtr->RebootState.RebootFlag ? SD_FLAG_REBOOT : 0U) | SD_FLAG_UNICAST));

    PduInfo.SduDataPtr = SendBuffer.Data;
    PduInfo.SduLength = SendBuffer.DataEnd;

    /* Deviation MISRAC2012-1 */
    RetVal = SoAd_SetUniqueRemoteAddr(InstanceCfgPtr->SdInstanceTxSoCon, (TcpIp_SockAddrType*)&CurrTxStatePtr->Address, &SoConId);

    if(RetVal == E_OK)
    { /* return value can be ignored as the only possible return value is E_OK */
      (void)SoAd_IfTransmit(InstanceCfgPtr->SdInstanceTxPduId, &PduInfo);

      CurrTxStatePtr->RebootState.SessionId++;
      if (CurrTxStatePtr->RebootState.SessionId == 0U)
      {
        CurrTxStatePtr->RebootState.SessionId = 0x0001U;
        CurrTxStatePtr->RebootState.RebootFlag = FALSE;
      }
      /* socket connection is released, that is set to the configurated values */
      SoAd_ReleaseRemoteAddr(SoConId);
    }
    else
    {
#if (SD_DEV_ERROR_DETECT == STD_ON)
      SD_DET_REPORT_ERROR(SD_SEND_FLUSHBUFFER_SVCID, SD_CONTROL_SOCKET_NOT_RELEASED);
#endif
      /* This is only a defensive action, this ensures that the configured socket connection is released,
       * because the SD control socket connection group has to contain only one socket connection
       * and it always has to be release after receiving and processing a SOME/IP SD control frame
       * and also after sending a SOME/IP SD control frame */
      SoAd_ReleaseRemoteAddr(InstanceCfgPtr->SdInstanceTxSoCon);
    }
    Sd_Send_resetBuffer();
  }
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_Send_queueFindService
(
  CONSTP2CONST(SdClientService_t,AUTOMATIC,SD_APPL_DATA) ServicePtr,
  uint16 serviceIdx
)
{
  Sd_Send_checkBufferFillLevel(0U, ServicePtr->SdClientServiceConfigurationStringLength);

  {
    CONSTP2VAR(uint8,AUTOMATIC,SD_APPL_DATA) EntryPtr = &SendBuffer.Data[SendBuffer.DataEnd];
    CONSTP2CONST(SdClientTimer_t, AUTOMATIC, SD_APPL_DATA) TimerCfgPtr = &SD_CFG_REF(SdClientTimer_t, SdClientTimersRef, 0U);
    uint8 OptionIndex2 = 0U;
    uint8 OptionCount2 = 0U;

    if(ServicePtr->SdClientServiceConfigurationStringLength > 0U)
    {
      const uint32 Hash = SD_OPTIONQUEUE_HASH(SD_OPTIONQUEUETYPE_CLIENTSERVICE_CONFIGURATIONOPTION, serviceIdx);
      Sd_Send_addOption(&OptionIndex2,
                        Hash,
                        SD_OPTIONCONFIG_SIZE(ServicePtr->SdClientServiceConfigurationStringLength));
      OptionCount2 = 1U;
    }

    /* write entry */
    SD_WRITE(SD_ENTRY_TYPE, EntryPtr, SD_ENTRYTYPE_FINDSERVICE);
    SD_WRITE(SD_ENTRY_INDEX1OP,EntryPtr, 0U);
    SD_WRITE(SD_ENTRY_INDEX2OP,EntryPtr, OptionIndex2);
    SD_WRITE(SD_ENTRY_NUMOPTS,EntryPtr, OptionCount2);

    SD_WRITE(SD_ENTRY_SERVICEID,EntryPtr,ServicePtr->SdClientServiceID);
    SD_WRITE(SD_ENTRY_INSTANCEID,EntryPtr,ServicePtr->SdClientServiceInstanceId);

    SD_WRITE(SD_ENTRY_MAJORVERSION,EntryPtr,ServicePtr->SdClientServiceMajorVersion);
    SD_WRITE(SD_ENTRY_TTL,EntryPtr, TimerCfgPtr[ServicePtr->SdClientServiceTimerIndex].SdClientTimerTTL);
    if(ServicePtr->SdVersionDrivenFindBehavior == SD_EXACT_OR_ANY_MINOR_VERSION)
    { /* !LINKSTO Sd.ASR40.SWS_SD_00503,1 */
      SD_WRITE(SD_ENTRYT1_MINORVERSION,EntryPtr, ServicePtr->SdClientServiceMinorVersion);
    }
    else
    { /* !LINKSTO Sd.ASR40.SWS_SD_10503,1 */
      SD_WRITE(SD_ENTRYT1_MINORVERSION,EntryPtr, SD_WILDCARD_MINORVERSION);
    }
    SendBuffer.DataEnd += SD_ENTRY_SIZE;
  }
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_Send_queueOfferService
(
  CONSTP2CONST(SdServerService_t,AUTOMATIC,SD_APPL_DATA) ServicePtr,
  const uint16 serviceIdx,
  const boolean isStopOffer
)
{
  Sd_Send_checkBufferFillLevel(2U, ServicePtr->SdServerServiceConfigurationStringLength);

  {
    CONSTP2VAR(uint8,AUTOMATIC,SD_APPL_DATA) EntryPtr = &SendBuffer.Data[SendBuffer.DataEnd];
    CONSTP2CONST(SdSocketConnectionGroupType, AUTOMATIC, SD_APPL_DATA) SdSoConGroupsCfgRefPtr = &SD_CFG_REF(SdSocketConnectionGroupType, SdSoConGroupsRef, 0U);
    CONSTP2CONST(SdServerTimer_t, AUTOMATIC, SD_APPL_DATA) TimerCfgPtr = &SD_CFG_REF(SdServerTimer_t, SdServerTimersRef, 0U);
    uint32 UdpHash, TcpHash;
    const TcpIp_DomainType LocalDomain = Sd_GetInstanceAddressDomain(SendBuffer.InstanceIndex);
    uint8 OptionIndex1, OptionIndex2;
    uint8 OptionCount1, OptionCount2;

    if(ServicePtr->SoConGroupUdp != SD_INVALID_SOCONGROUP_ID)
    {
      UdpHash = SD_OPTIONQUEUE_HASH(SD_OPTIONQUEUETYPE_UDP_ENDPOINTOPTION, SdSoConGroupsCfgRefPtr[ServicePtr->SoConGroupUdp].GroupStart);

      /* !LINKSTO Sd.ASR40.SWS_SD_00656.TCP,1 */
      /* if there is an unicast UDP server service reference, then there might or might not be a TCP reference */
      if(ServicePtr->SoConGroupTcp != SD_INVALID_SOCONGROUP_ID)
      {
        TcpHash = SD_OPTIONQUEUE_HASH(SD_OPTIONQUEUETYPE_TCP_ENDPOINTOPTION, SdSoConGroupsCfgRefPtr[ServicePtr->SoConGroupTcp].GroupStart);
      }
      else
      {
        TcpHash = SD_OPTIONQUEUE_HASH_INVALID;
      }
    }
    else /* if there is not an unicast UDP server service reference, then there has to be a TCP reference */
    { /* !LINKSTO Sd.ASR40.SWS_SD_00656.TCP,1 */
      UdpHash = SD_OPTIONQUEUE_HASH_INVALID;
      TcpHash = SD_OPTIONQUEUE_HASH(SD_OPTIONQUEUETYPE_TCP_ENDPOINTOPTION, SdSoConGroupsCfgRefPtr[ServicePtr->SoConGroupTcp].GroupStart);
    }

    if(LocalDomain == TCPIP_AF_INET)
    {
      Sd_Send_addOptions(&OptionIndex1, &OptionCount1, UdpHash, TcpHash, SD_OPTIONIPV4_SIZE);
    }
    else
    {

#if (SD_DEV_ERROR_DETECT == STD_ON)
      SD_PRECONDITION_ASSERT( (LocalDomain == TCPIP_AF_INET6), SD_SENDQUEUEOPTIONS_SVCID );
#endif

      Sd_Send_addOptions(&OptionIndex1, &OptionCount1, UdpHash, TcpHash, SD_OPTIONIPV6_SIZE);
    }

    if(ServicePtr->SdServerServiceConfigurationStringLength > 0U)
    {
      const uint32 Hash = SD_OPTIONQUEUE_HASH(SD_OPTIONQUEUETYPE_SERVERSERVICE_CONFIGURATIONOPTION, serviceIdx);
      Sd_Send_addOption(&OptionIndex2,
                        Hash,
                        SD_OPTIONCONFIG_SIZE(ServicePtr->SdServerServiceConfigurationStringLength));
      OptionCount2 = 1U;
    }
    else
    {
      OptionCount2 = 0U;
      /* initializing this index is not strictly necessary but it is done for security and coherence */
      OptionIndex2 = 0U;
    }

    /* write entry */
    SD_WRITE(SD_ENTRY_TYPE, EntryPtr, SD_ENTRYTYPE_OFFERSERVICE);
    SD_WRITE(SD_ENTRY_INDEX1OP,EntryPtr, OptionIndex1);
    SD_WRITE(SD_ENTRY_INDEX2OP,EntryPtr, OptionIndex2);
    SD_WRITE(SD_ENTRY_NUMOPTS,EntryPtr, ((OptionCount1 << 4) + OptionCount2));

    SD_WRITE(SD_ENTRY_SERVICEID,EntryPtr,ServicePtr->SdServerServiceID);
    SD_WRITE(SD_ENTRY_INSTANCEID,EntryPtr,ServicePtr->SdServerServiceInstanceId);

    SD_WRITE(SD_ENTRY_MAJORVERSION,EntryPtr,ServicePtr->SdServerServiceMajorVersion);
    if(isStopOffer)
    {
      SD_WRITE(SD_ENTRY_TTL,EntryPtr, 0UL);
    }
    else
    {
      SD_WRITE(SD_ENTRY_TTL,EntryPtr, TimerCfgPtr[ServicePtr->SdServerServiceTimerIndex].SdServerTimerTTL);
    }

    SD_WRITE(SD_ENTRYT1_MINORVERSION,EntryPtr,ServicePtr->SdServerServiceMinorVersion);
    SendBuffer.DataEnd += SD_ENTRY_SIZE;
  }
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_Send_queueSubscribeEventgroup
(
  const uint16 eventGroupIdx,
  const SoAd_SoConIdType UniUdpSoConId,
  const SoAd_SoConIdType UniTcpSoConId,
  const boolean isStopSubscribe
)
{
  P2CONST(SdConsumedEventGroup_t,AUTOMATIC,SD_APPL_DATA) EventGroupCfgPtr = &SD_CFG(SdConsumedEventGroup_t, SdConsumedEventGroupsRef, eventGroupIdx);
  P2CONST(SdClientService_t,AUTOMATIC,SD_APPL_DATA) ServiceCfgPtr = &SD_CFG(SdClientService_t, SdClientServicesRef, EventGroupCfgPtr->ServiceHandleId);

  Sd_Send_checkBufferFillLevel(2U, ServiceCfgPtr->SdClientServiceConfigurationStringLength);

  {
    CONSTP2VAR(uint8,AUTOMATIC,SD_APPL_DATA) EntryPtr = &SendBuffer.Data[SendBuffer.DataEnd];

    uint32 UdpHash, TcpHash;
    const TcpIp_DomainType LocalDomain = Sd_GetInstanceAddressDomain(SendBuffer.InstanceIndex);
    uint8 OptionIndex1 = 0U, OptionCount1 = 0U;
    uint8 OptionIndex2, OptionCount2;

    /* !LINKSTO Sd.ASR40.SWS_SD_00655,1 */
    /* !LINKSTO Sd.ASR40.SWS_SD_00658.UDP,1 */
    /* A UDP end-point option is added if there is an UDP unicast data path configured for this event group OR
     * if there is an UDP reference configured in the client service and the received OFFER has a UDP entry attached to it */
    if(UniUdpSoConId != SOAD_INVALID_SOCON_ID)
    { /* !LINKSTO Sd.ASR40.SWS_SD_00701.UDP,1 */
      UdpHash = SD_OPTIONQUEUE_HASH(SD_OPTIONQUEUETYPE_UDP_ENDPOINTOPTION, UniUdpSoConId);
    }
    else
    {
      UdpHash = SD_OPTIONQUEUE_HASH_INVALID;
    }
    /* !LINKSTO Sd.ASR40.SWS_SD_00655,1 */
    /* !LINKSTO Sd.ASR40.SWS_SD_00658.TCP,1 */
    /* A TCP end-point option is added if there is an TCP unicast data path configured for this event group OR
     * if there is an TCP reference configured in the client service and the received OFFER has a TCP entry attached to it */
    if(UniTcpSoConId != SOAD_INVALID_SOCON_ID)
    { /* !LINKSTO Sd.ASR40.SWS_SD_00701.TCP,1 */
      TcpHash = SD_OPTIONQUEUE_HASH(SD_OPTIONQUEUETYPE_TCP_ENDPOINTOPTION, UniTcpSoConId);
    }
    else
    {
      TcpHash = SD_OPTIONQUEUE_HASH_INVALID;
    }

    if(LocalDomain == TCPIP_AF_INET)
    {
      Sd_Send_addOptions(&OptionIndex1, &OptionCount1, UdpHash, TcpHash, SD_OPTIONIPV4_SIZE);
    }
    else
    {

#if (SD_DEV_ERROR_DETECT == STD_ON)
      SD_PRECONDITION_ASSERT( (LocalDomain == TCPIP_AF_INET6), SD_SENDQUEUEOPTIONS_SVCID );
#endif

      Sd_Send_addOptions(&OptionIndex1, &OptionCount1, UdpHash, TcpHash, SD_OPTIONIPV6_SIZE);
    }

    if(ServiceCfgPtr->SdClientServiceConfigurationStringLength > 0U)
    {
      const uint32 Hash = SD_OPTIONQUEUE_HASH(SD_OPTIONQUEUETYPE_CLIENTSERVICE_CONFIGURATIONOPTION, EventGroupCfgPtr->ServiceHandleId);
      Sd_Send_addOption(&OptionIndex2,
                        Hash,
                        SD_OPTIONCONFIG_SIZE(ServiceCfgPtr->SdClientServiceConfigurationStringLength));
      OptionCount2 = 1U;
    }
    else
    {
      OptionCount2 = 0U;
      /* initializing this index is not strictly necessary but it is done for security and coherence */
      OptionIndex2 = 0U;
    }

    /* !LINKSTO Sd.ASR40.SWS_SD_00701.UDP,1 */
    /* !LINKSTO Sd.ASR40.SWS_SD_00701.TCP,1 */
    /* write entry */
    SD_WRITE(SD_ENTRY_TYPE,EntryPtr,SD_ENTRYTYPE_SUBSCRIBEEVENTGROUP);
    SD_WRITE(SD_ENTRY_INDEX1OP,EntryPtr, OptionIndex1);
    SD_WRITE(SD_ENTRY_INDEX2OP,EntryPtr, OptionIndex2);
    SD_WRITE(SD_ENTRY_NUMOPTS,EntryPtr, ((uint8)(OptionCount1 << 4U) + OptionCount2));

    SD_WRITE(SD_ENTRY_SERVICEID,EntryPtr, ServiceCfgPtr->SdClientServiceID);
    SD_WRITE(SD_ENTRY_INSTANCEID,EntryPtr, ServiceCfgPtr->SdClientServiceInstanceId);
    SD_WRITE(SD_ENTRY_MAJORVERSION,EntryPtr, ServiceCfgPtr->SdClientServiceMajorVersion);
    if(isStopSubscribe)
    {
      SD_WRITE(SD_ENTRY_TTL,EntryPtr, 0UL);
    }
    else
    {
      SD_WRITE(SD_ENTRY_TTL,EntryPtr, EventGroupCfgPtr->SdClientTimerTTL);
    }
    SD_WRITE(SD_ENTRYT2_RESERVED2,EntryPtr, (uint16)0);
    SD_WRITE(SD_ENTRYT2_EVENTGROUPID,EntryPtr, EventGroupCfgPtr->SdConsumedEventGroupID);

    SendBuffer.DataEnd += SD_ENTRY_SIZE;
  }
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_Send_queueSubscribeEventgroupAck
(
  const uint32 TTL,
  const uint16 serviceIdx,
  const uint16 eventGroupIdx,
  const uint16 reservedField
)
{
  P2CONST(SdEventHandler_t,AUTOMATIC,SD_APPL_DATA) EventCfgPtr = &SD_CFG(SdEventHandler_t, SdEventHandlersRef, eventGroupIdx);

  Sd_Send_queueSubscribeEventgroupAckNack(TTL,
                                          serviceIdx,
                                          EventCfgPtr->SdEventHandlerEventGroupId,
                                          reservedField,
                                          EventCfgPtr->SdMulticastEventSoConRef
                                         );
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_Send_queueSubscribeEventgroupNack
(
  const uint16 serviceIdx,
  const uint16 reservedField,
  const uint16 eventGroupId
)
{
  Sd_Send_queueSubscribeEventgroupAckNack(0U,
                                          serviceIdx,
                                          eventGroupId,
                                          reservedField,
                                          SOAD_INVALID_SOCON_ID
                                         );
}

TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_Send_queueSubscribeEventgroupNackUnknown
(
  uint16 serviceID,
  uint16 instanceId,
  uint16 eventGroupId,
  uint16 reservedField,
  uint8 majorVersion
)
{
  Sd_Send_checkBufferFillLevel(0U, 0U);

  {
    CONSTP2VAR(uint8,AUTOMATIC,SD_APPL_DATA) EntryPtr = &SendBuffer.Data[SendBuffer.DataEnd];

    /* write entry */
    SD_WRITE(SD_ENTRY_TYPE,EntryPtr,SD_ENTRYTYPE_SUBSCRIBEEVENTGROUPACK);
    SD_WRITE(SD_ENTRY_INDEX1OP,EntryPtr, 0U);
    SD_WRITE(SD_ENTRY_INDEX2OP,EntryPtr, 0U);
    SD_WRITE(SD_ENTRY_NUMOPTS,EntryPtr, 0U);

    SD_WRITE(SD_ENTRY_SERVICEID,EntryPtr,serviceID);
    SD_WRITE(SD_ENTRY_INSTANCEID,EntryPtr,instanceId);
    SD_WRITE(SD_ENTRY_MAJORVERSION,EntryPtr,majorVersion);
    SD_WRITE(SD_ENTRY_TTL,EntryPtr,0UL);
    SD_WRITE(SD_ENTRYT2_RESERVED2,EntryPtr,reservedField);
    SD_WRITE(SD_ENTRYT2_EVENTGROUPID,EntryPtr,eventGroupId);

    SendBuffer.DataEnd += SD_ENTRY_SIZE;
  }
}



/*==================[internal function definitions]=========================*/



STATIC FUNC(void, SD_CODE) Sd_Send_resetBuffer(void)
{
  SendBuffer.OptionSize = 0U;
  SendBuffer.DataEnd = SD_HEADER_SIZE + SD_HEADER_ENTRYLEN_SIZE;
  SendBuffer.OptionEnd = 0U;
}



STATIC FUNC(void, SD_CODE) Sd_Send_queueOptions(void)
{
  uint8 Index;
  CONSTP2VAR(uint8,AUTOMATIC,SD_APPL_DATA) OptionsLengthPtr = &SendBuffer.Data[SendBuffer.DataEnd];
  uint16 OptionsBegin;
  SendBuffer.DataEnd += SD_HEADER_OPTSLEN_SIZE;
  OptionsBegin = SendBuffer.DataEnd;

  for(Index = 0; Index < SendBuffer.OptionEnd; Index++)
  {
    CONSTP2VAR(uint8,AUTOMATIC,SD_APPL_DATA) DataPtr = &SendBuffer.Data[SendBuffer.DataEnd];
    const uint8 Type = (uint8)SD_OPTIONQUEUE_HASH_Type(SendBuffer.Options[Index]);
    const uint32 Idx = SD_OPTIONQUEUE_HASH_Id(SendBuffer.Options[Index]);

    switch(Type)
    {
    case SD_OPTIONQUEUETYPE_UDP_ENDPOINTOPTION:
    case SD_OPTIONQUEUETYPE_TCP_ENDPOINTOPTION:
    case SD_OPTIONQUEUETYPE_MULTICAST_ENDPOINTOPTION:
    {
      SoAd_SoConIdType SoConId = (SoAd_SoConIdType)Idx;
      TcpIp_DomainType LocalDomain = Sd_GetInstanceAddressDomain(SendBuffer.InstanceIndex);

      Sd_Send_multicastEndpointOption(LocalDomain, DataPtr, Type, SoConId);

      break;
    }
    case SD_OPTIONQUEUETYPE_SERVERSERVICE_CONFIGURATIONOPTION:
    case SD_OPTIONQUEUETYPE_CLIENTSERVICE_CONFIGURATIONOPTION:
    {
      SdConfigOptionPtrType ConfigStringPtr = &SD_CFG_REF(uint8, SdConfigStringRef, 0U);
      uint16 ConfigStringLength;

      if(Type == SD_OPTIONQUEUETYPE_SERVERSERVICE_CONFIGURATIONOPTION)
      {
        P2CONST(SdServerService_t,AUTOMATIC,SD_APPL_DATA) ServicePtr = &SD_CFG(SdServerService_t, SdServerServicesRef, Idx);

        ConfigStringPtr = &ConfigStringPtr[ServicePtr->SdConfigurationStringIndex];
        ConfigStringLength = ServicePtr->SdServerServiceConfigurationStringLength;
      }
      else
      {
        P2CONST(SdClientService_t,AUTOMATIC,SD_APPL_DATA) ServicePtr = &SD_CFG(SdClientService_t, SdClientServicesRef, Idx);

        ConfigStringPtr = &ConfigStringPtr[ServicePtr->SdConfigurationStringIndex];
        ConfigStringLength = ServicePtr->SdClientServiceConfigurationStringLength;
      }

      SD_WRITE(SD_OPTIONCOMMON_LENGTH,DataPtr,((uint16)SD_OPTIONCONFIG_COMMONSIZE - (uint16)SD_OPTIONCOMMON_SIZE) + ConfigStringLength);
      SD_WRITE(SD_OPTIONCOMMON_TYPE,DataPtr,SD_OPTIONTYPE_CONFIGURATION);
      SD_WRITE(SD_OPTIONCONFIG_RESERVED,DataPtr,0U);

      {
        uint16 StringIndex;
        for(StringIndex = 0U; StringIndex < ConfigStringLength; StringIndex++)
        {
          SD_WRITE8U(SD_OPTIONCONFIG_STRING_OFFSET + StringIndex, DataPtr, ConfigStringPtr[StringIndex]);
        }
      }

      SendBuffer.DataEnd += SD_OPTIONCONFIG_COMMONSIZE + ConfigStringLength;

      break;
    }
    /* CHECK: NOPARSE */
    default:
	/* should never happen */
#if (SD_DEV_ERROR_DETECT == STD_ON)
      SD_UNREACHABLE_CODE_ASSERT(SD_SENDQUEUEOPTIONS_SVCID);
#endif /* SD_DEV_ERROR_DETECT == STD_ON */
      break;
    /* CHECK: PARSE */
    }
  }
  {
    uint32 OptionsLength = (uint32)SendBuffer.DataEnd - (uint32)OptionsBegin;
    /* write size of options array */
    SD_WRITE32U(0U, OptionsLengthPtr, OptionsLength);
  }
}



STATIC FUNC(void, SD_CODE) Sd_Send_multicastEndpointOption
(
  TcpIp_DomainType LocalDomain,
  CONSTP2VAR(uint8,AUTOMATIC,SD_APPL_DATA) DataPtr,
  const uint8 Type,
  SoAd_SoConIdType SoConId
)
{
  if(LocalDomain == TCPIP_AF_INET)
  {
    TcpIp_SockAddrInetType Ip;
    Ip.domain = TCPIP_AF_INET;

    /* write ipv4 endpoint option */
    Sd_Send_writeIpv4EndpointOption(&Ip, DataPtr, Type, SoConId);

    SD_WRITE(SD_OPTIONIPV4_RESERVED1,DataPtr,0U);
    SD_WRITE32U_NE(SD_OPTIONIPV4_ADDRESS_OFFSET,DataPtr,Ip.addr[0]);
    SD_WRITE(SD_OPTIONIPV4_RESERVED2,DataPtr,0U);
    if(Type == SD_OPTIONQUEUETYPE_TCP_ENDPOINTOPTION)
    { /* !LINKSTO Sd.ASR40.SWS_SD_00701.TCP,1 */
      SD_WRITE(SD_OPTIONIPV4_PROTOCOL,DataPtr,TCPIP_IPPROTO_TCP);
    }
    else
    { /* !LINKSTO Sd.ASR40.SWS_SD_00701.UDP,1 */
      SD_WRITE(SD_OPTIONIPV4_PROTOCOL,DataPtr,TCPIP_IPPROTO_UDP);
    }
    SD_WRITE(SD_OPTIONIPV4_PORT,DataPtr,Ip.port);
    SendBuffer.DataEnd += SD_OPTIONIPV4_SIZE;
  }
  else
  {
    TcpIp_SockAddrInet6Type Ip;

  #if (SD_DEV_ERROR_DETECT == STD_ON)
    SD_PRECONDITION_ASSERT( (LocalDomain == TCPIP_AF_INET6), SD_SENDQUEUEOPTIONS_SVCID );
  #endif
    Ip.domain = TCPIP_AF_INET6;

    /* write ipv6 endpoint option */
    Sd_Send_writeIpv6EndpointOption(&Ip, DataPtr, Type, SoConId);

    SD_WRITE(SD_OPTIONIPV6_RESERVED1,DataPtr,0U);
    SD_WRITE128U_NE(SD_OPTIONIPV6_ADDRESS_OFFSET,DataPtr,Ip.addr[0]);
    SD_WRITE(SD_OPTIONIPV6_RESERVED2,DataPtr,0U);
    if(Type == SD_OPTIONQUEUETYPE_TCP_ENDPOINTOPTION)
    { /* !LINKSTO Sd.ASR40.SWS_SD_00701.TCP,1 */
      SD_WRITE(SD_OPTIONIPV6_PROTOCOL,DataPtr,TCPIP_IPPROTO_TCP);
    }
    else
    { /* !LINKSTO Sd.ASR40.SWS_SD_00701.UDP,1 */
      SD_WRITE(SD_OPTIONIPV6_PROTOCOL,DataPtr,TCPIP_IPPROTO_UDP);
    }
    /* !LINKSTO Sd.ASR40.SWS_SD_00222,1 */
    /* !LINKSTO Sd.ASR40.SWS_SD_00415,1 */
    SD_WRITE(SD_OPTIONIPV6_PORT,DataPtr,Ip.port);
    SendBuffer.DataEnd += SD_OPTIONIPV6_SIZE;
  }
}



STATIC FUNC(void, SD_CODE) Sd_Send_writeIpv4EndpointOption
(
  TcpIp_SockAddrInetType *Ip,
  CONSTP2VAR(uint8,AUTOMATIC,SD_APPL_DATA) DataPtr,
  const uint8 Type,
  SoAd_SoConIdType SoConId
)
{
  SD_WRITE( SD_OPTIONCOMMON_LENGTH, DataPtr, ((uint16)SD_OPTIONIPV4_SIZE - (uint16)SD_OPTIONCOMMON_SIZE) ); /* length is 3 bytes less (headerpart) than real */
  if(Type == SD_OPTIONQUEUETYPE_MULTICAST_ENDPOINTOPTION)
  {
    /* The return value can be ignored as the only possible return value is E_OK */
    /* Deviation MISRAC2012-1 */
    (void)SoAd_GetRemoteAddr(SoConId,(P2VAR(TcpIp_SockAddrType, TYPEDEF, SD_APPL_DATA))Ip);
    SD_WRITE(SD_OPTIONCOMMON_TYPE,DataPtr,SD_OPTIONTYPE_IPV4MULTI);
  }
  else
  {
    TcpIp_SockAddrInetType Gateway;
    uint8 NetMask;
    Gateway.domain = TCPIP_AF_INET;

    /* !LINKSTO Sd.ASR40.SWS_SD_00701.UDP,1 */
    /* !LINKSTO Sd.ASR40.SWS_SD_00701.TCP,1 */
    /* The return value can be ignored as the only possible return value is E_OK */
    /* Deviation MISRAC2012-1 */
    (void)SoAd_GetLocalAddr(SoConId,(TcpIp_SockAddrType*)Ip,&NetMask,(TcpIp_SockAddrType*)&Gateway);
    SD_WRITE(SD_OPTIONCOMMON_TYPE,DataPtr,SD_OPTIONTYPE_IPV4);
  }
}



STATIC FUNC(void, SD_CODE) Sd_Send_writeIpv6EndpointOption
(
  TcpIp_SockAddrInet6Type *Ip,
  CONSTP2VAR(uint8,AUTOMATIC,SD_APPL_DATA) DataPtr,
  const uint8 Type,
  SoAd_SoConIdType SoConId
)
{
  SD_WRITE( SD_OPTIONCOMMON_LENGTH, DataPtr, ((uint16)SD_OPTIONIPV6_SIZE - (uint16)SD_OPTIONCOMMON_SIZE) ); /* length is 3 bytes less (headerpart) than real */
  if(Type == SD_OPTIONQUEUETYPE_MULTICAST_ENDPOINTOPTION)
  {
    /* The return value can be ignored as the only possible return value is E_OK */
    /* Deviation MISRAC2012-1 */
    (void)SoAd_GetRemoteAddr(SoConId,(P2VAR(TcpIp_SockAddrType, TYPEDEF, SD_APPL_DATA))Ip);
    SD_WRITE(SD_OPTIONCOMMON_TYPE,DataPtr,SD_OPTIONTYPE_IPV6MULTI);
  }
  else
  {
    TcpIp_SockAddrInet6Type Gateway;
    uint8 NetMask;
    Gateway.domain = TCPIP_AF_INET6;

    /* The return value can be ignored as the only possible return value is E_OK */
    /* Deviation MISRAC2012-1 */
    (void)SoAd_GetLocalAddr(SoConId,(TcpIp_SockAddrType*)Ip,&NetMask,(TcpIp_SockAddrType*)&Gateway);
    SD_WRITE(SD_OPTIONCOMMON_TYPE,DataPtr,SD_OPTIONTYPE_IPV6);
  }
}



STATIC FUNC(void, SD_CODE) Sd_Send_checkBufferFillLevel(uint16 NumberOfEndpointoptions, uint16 ConfigStringLength)
{
  uint16 NumberOfBytes;
  TcpIp_DomainType LocalDomain = Sd_GetInstanceAddressDomain(SendBuffer.InstanceIndex);

  if(LocalDomain == TCPIP_AF_INET)
  {
    NumberOfBytes = SD_ENTRY_SIZE + (NumberOfEndpointoptions * SD_OPTIONIPV4_SIZE);
  }
  else
  {
#if (SD_DEV_ERROR_DETECT == STD_ON)
    SD_PRECONDITION_ASSERT( (LocalDomain == TCPIP_AF_INET6), SD_CHECKBUFFERFILLLEVEL_SVCID );
#endif

    NumberOfBytes = SD_ENTRY_SIZE + (NumberOfEndpointoptions * SD_OPTIONIPV6_SIZE);
  }

  if(ConfigStringLength != 0U)
  {
    ConfigStringLength += SD_OPTIONCONFIG_COMMONSIZE;
  }

  if(((SendBuffer.DataEnd + SD_HEADER_OPTSLEN_SIZE + SendBuffer.OptionSize + NumberOfBytes + ConfigStringLength) > SD_SEND_MAXPAYLOAD) ||
     ((SendBuffer.OptionEnd + 3U) > SD_SEND_MAXNUM_OPTIONS))
  {
    Sd_Send_flushBuffer();
  }
}



STATIC FUNC(void, SD_CODE) Sd_Send_appendOption
(
  const uint32 OptionHash,
  const uint16 OptionSize
)
{
  SendBuffer.Options[SendBuffer.OptionEnd] = OptionHash;
  SendBuffer.OptionEnd++;
  SendBuffer.OptionSize += OptionSize;
}



STATIC FUNC(void, SD_CODE) Sd_Send_addOption
(
  CONSTP2VAR(uint8,AUTOMATIC,SD_APPL_DATA) OptionIndexPtr,
  const uint32 Hash,
  const uint16 Size
)
{
  uint8 Index;

  for(Index = 0U; Index < SendBuffer.OptionEnd; Index++)
  {
    if(SendBuffer.Options[Index] == Hash)
    {
      break;
    }
  }

  if(Index == SendBuffer.OptionEnd)
  {
    Sd_Send_appendOption(Hash, Size);
  }

  *OptionIndexPtr = Index;
}



STATIC FUNC(void, SD_CODE) Sd_Send_addOptions
(
  CONSTP2VAR(uint8,AUTOMATIC,SD_APPL_DATA) OptionIndexPtr,
  CONSTP2VAR(uint8,AUTOMATIC,SD_APPL_DATA) OptionCountPtr,
  const uint32 Hash1,
  const uint32 Hash2,
  const uint16 Size
)
{
  *OptionCountPtr = 0U;
  /* First of all check if this entry has both an UDP and a TCP option attached to it */
  if( (Hash1 != SD_OPTIONQUEUE_HASH_INVALID) && (Hash2 != SD_OPTIONQUEUE_HASH_INVALID) )
  {
    uint8 Index;

    for(Index = 0U; Index < SendBuffer.OptionEnd; Index++)
    {
      if(SendBuffer.Options[Index] == Hash1)
      {
        /* Hash1 found */
        if((Index + 1U) == SendBuffer.OptionEnd)
        {
          /* Hash1 is last option, append second option to the end */
          Sd_Send_appendOption(Hash2, Size);
          *OptionIndexPtr = Index;
          *OptionCountPtr = 2U;
        }
        else if(SendBuffer.Options[Index + 1U] == Hash2)
        {
          /* first option is already followed by second option, nothing to add */
          *OptionIndexPtr = Index;
          *OptionCountPtr = 2U;
        }
        else
        {
          /* Found option is not reusable */
        }
      }
      if(*OptionCountPtr == 2U)
      {
        break;
      }
    }

    if(*OptionCountPtr != 2U)
    {
      /* No reusable option, append both options to the end of the list */
      Sd_Send_appendOption(Hash1, Size);
      Sd_Send_appendOption(Hash2, Size);
      *OptionIndexPtr = Index;
      *OptionCountPtr = 2U;
    }
  }
  /* else, this entry has either a UPD option or a TCP option or otherwise no option attached to it */
  else
  { /* check Hash1 is being used */
    if(Hash1 != SD_OPTIONQUEUE_HASH_INVALID)
    { /* Only Hash1 is used to attach an UDP option */
      Sd_Send_addOption(OptionIndexPtr, Hash1, Size);
      *OptionCountPtr = 1U;
    }
    /* else check Hash2 is being used */
    else if(Hash2 != SD_OPTIONQUEUE_HASH_INVALID)
    { /* Only Hash2 is used to attach an TCP option */
      Sd_Send_addOption(OptionIndexPtr, Hash2, Size);
      *OptionCountPtr = 1U;
    }
    else
    {
      /* no option at all is attached to this entry */
    }
  }
}



STATIC FUNC(void, SD_CODE) Sd_Send_queueSubscribeEventgroupAckNack
(
  const uint32 TTL,
  const uint16 serviceIdx,
  const uint16 eventGroupId,
  const uint16 reservedField,
  const SoAd_SoConIdType MulticastEventSoCon
)
{
  P2CONST(SdServerService_t,AUTOMATIC,SD_APPL_DATA) ServiceCfgPtr = &SD_CFG(SdServerService_t, SdServerServicesRef, serviceIdx);

  if(MulticastEventSoCon != SOAD_INVALID_SOCON_ID)
  {
    Sd_Send_checkBufferFillLevel(1U, ServiceCfgPtr->SdServerServiceConfigurationStringLength);
  }
  else
  {
    Sd_Send_checkBufferFillLevel(0U, ServiceCfgPtr->SdServerServiceConfigurationStringLength);
  }

  {
    CONSTP2VAR(uint8,AUTOMATIC,SD_APPL_DATA) EntryPtr = &SendBuffer.Data[SendBuffer.DataEnd];

    uint8 OptionIndex1, OptionIndex2;
    uint8 OptionCount1, OptionCount2;

    if(MulticastEventSoCon != SOAD_INVALID_SOCON_ID)
    {
      const uint32 McHash = SD_OPTIONQUEUE_HASH(SD_OPTIONQUEUETYPE_MULTICAST_ENDPOINTOPTION,
                                          MulticastEventSoCon);
      TcpIp_DomainType LocalDomain = Sd_GetInstanceAddressDomain(SendBuffer.InstanceIndex);

      if(LocalDomain == TCPIP_AF_INET)
      {
        Sd_Send_addOption(&OptionIndex1, McHash, SD_OPTIONIPV4_SIZE);
      }
      else
      {

#if (SD_DEV_ERROR_DETECT == STD_ON)
        SD_PRECONDITION_ASSERT( (LocalDomain == TCPIP_AF_INET6), SD_SENDQUEUEOPTIONS_SVCID );
#endif

        Sd_Send_addOption(&OptionIndex1, McHash, SD_OPTIONIPV6_SIZE);
      }

      OptionCount1 = 1U;
    }
    else
    {
      OptionCount1 = 0U;
      OptionIndex1 = 0U;
    }

    if(ServiceCfgPtr->SdServerServiceConfigurationStringLength > 0U)
    {
      const uint32 Hash = SD_OPTIONQUEUE_HASH(SD_OPTIONQUEUETYPE_SERVERSERVICE_CONFIGURATIONOPTION, serviceIdx);
      Sd_Send_addOption(&OptionIndex2,
                        Hash,
                        SD_OPTIONCONFIG_SIZE(ServiceCfgPtr->SdServerServiceConfigurationStringLength));
      OptionCount2 = 1U;
    }
    else
    {
      OptionCount2 = 0U;
      OptionIndex2 = 0U;
    }

    /* write entry */
    SD_WRITE(SD_ENTRY_TYPE,EntryPtr,SD_ENTRYTYPE_SUBSCRIBEEVENTGROUPACK);
    SD_WRITE(SD_ENTRY_INDEX1OP,EntryPtr, OptionIndex1);
    SD_WRITE(SD_ENTRY_INDEX2OP,EntryPtr, OptionIndex2);
    SD_WRITE(SD_ENTRY_NUMOPTS,EntryPtr, ((uint8)(OptionCount1 << 4U) + OptionCount2));

    SD_WRITE(SD_ENTRY_SERVICEID,EntryPtr,ServiceCfgPtr->SdServerServiceID);
    SD_WRITE(SD_ENTRY_INSTANCEID,EntryPtr,ServiceCfgPtr->SdServerServiceInstanceId);
    SD_WRITE(SD_ENTRY_MAJORVERSION,EntryPtr,ServiceCfgPtr->SdServerServiceMajorVersion);
    SD_WRITE(SD_ENTRY_TTL,EntryPtr,TTL);
    SD_WRITE(SD_ENTRYT2_RESERVED2,EntryPtr,reservedField);
    SD_WRITE(SD_ENTRYT2_EVENTGROUPID,EntryPtr,eventGroupId);

    SendBuffer.DataEnd += SD_ENTRY_SIZE;
  }
}







#define SD_STOP_SEC_CODE
#include <Sd_MemMap.h>

/*==================[end of file]===========================================*/

