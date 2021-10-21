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
#include <Sd_Cbk.h>           /* Module callback */
#include <Sd_Receive_Int.h>
#include <Sd_ServerService_Int.h>
#include <Sd_Send_Int.h>
#include <Sd_ClientService_Int.h>

/*==================[macros]================================================*/
#define SD_RECEIVE_INVALID_OPTION_LENGTH 0xFFFFU

/* macro to read the very first byte of */
#define READ_ENTRY_TYPE(EntryStartAddr)  (EntryStartAddr[SD_ENTRY_TYPE_OFFSET])

/*==================[type definitions]======================================*/

typedef P2CONST(uint8, AUTOMATIC, SD_APPL_DATA) SdOptionPtrType;





/*==================[external data]=========================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define SD_START_SEC_CODE
#include <Sd_MemMap.h>

STATIC FUNC(void, SD_CODE) Sd_Receive_processMessage
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  P2VAR(PduInfoType, AUTOMATIC, SD_APPL_DATA) pduInfoPtr,
  Sd_RemoteNodeIdType RemoteNodeIndex,
  uint8 InstanceIndex,
  boolean IsMulticast
);

STATIC FUNC_P2CONST(uint8, AUTOMATIC, SD_CODE) Sd_Receive_checkMessage
(
  CONSTP2CONST(PduInfoType, AUTOMATIC, SD_APPL_DATA) pduInfoPtr,
  CONSTP2VAR(uint16, AUTOMATIC, SD_APPL_DATA) entriesCount,
  CONSTP2VAR(uint16, AUTOMATIC, SD_APPL_DATA) optionInfoPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SD_APPL_DATA) optionCountPtr
);

STATIC FUNC(uint16, SD_CODE) Sd_Receive_checkOption
(
  P2CONST(uint8, AUTOMATIC, SD_APPL_DATA) optionPtr
);

STATIC FUNC(void, SD_CODE) Sd_Receive_getEntryOptions
(
  P2CONST(uint8, AUTOMATIC, SD_APPL_DATA) entryPtr,
  SdOptionPtrType OptionsStartAddr,
  P2CONST(uint16, AUTOMATIC, SD_APPL_DATA) optionInfoPtr,
  uint8 optionCount,
  P2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) entryOptionInfoPtr,
  uint8 InstanceIndex
);

STATIC FUNC(void, SD_CODE) Sd_Receive_processOptionIPv4run
(
  CONSTP2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) entryOptionInfoPtr,
  SdOptionPtrType OptionsStartAddr,
  CONSTP2CONST(uint16, AUTOMATIC, SD_APPL_DATA) optionInfoPtr,
  const uint8 optionIndex,
  const uint8 optionCount
);

STATIC FUNC(void, SD_CODE) Sd_Receive_processOptionIPv6run
(
  CONSTP2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) entryOptionInfoPtr,
  SdOptionPtrType OptionsStartAddr,
  CONSTP2CONST(uint16, AUTOMATIC, SD_APPL_DATA) optionInfoPtr,
  const uint8 optionIndex,
  const uint8 optionCount
);

STATIC FUNC(uint8, SD_CODE) Sd_Receive_readInetAddr
(
  P2CONST(uint8,AUTOMATIC,SD_APPL_DATA) OptionPtr,
  P2VAR(TcpIp_SockAddrInet6Type,AUTOMATIC,SD_APPL_DATA) InetAddrPtr
);

STATIC FUNC(uint8, SD_CODE) Sd_Receive_readInet6Addr
(
  P2CONST(uint8,AUTOMATIC,SD_APPL_DATA) OptionPtr,
  P2VAR(TcpIp_SockAddrInet6Type,AUTOMATIC,SD_APPL_DATA) InetAddrPtr
);

STATIC FUNC(uint8, SD_CODE) Sd_Receive_readConfigOption
(
  P2CONST(uint8,AUTOMATIC,SD_APPL_DATA) OptionPtr,
  P2VAR(SdConfigOptionPtrType,AUTOMATIC,SD_APPL_DATA) ConfigOptionPtr
);

#if( (SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DEM) || ((SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON)) || (SD_MALFORMED_MSG_CALLBACK == STD_ON) )
STATIC FUNC(uint8, SD_CODE) Sd_Receive_handleUnicastEntry
#else
STATIC FUNC(void, SD_CODE) Sd_Receive_handleUnicastEntry
#endif
(
  P2CONST(uint8, AUTOMATIC, SD_APPL_DATA) entryPtr,
  P2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfo,
  Sd_RemoteNodeIdType RemoteNodeIndex,
  uint8 SdInstanceIndex
);

#if( (SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DEM) || ((SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON)) || (SD_MALFORMED_MSG_CALLBACK == STD_ON) )
STATIC FUNC(uint8, SD_CODE) Sd_Receive_handleMulticastEntry
#else
STATIC FUNC(void, SD_CODE) Sd_Receive_handleMulticastEntry
#endif
(
  P2CONST(uint8, AUTOMATIC, SD_APPL_DATA) entryPtr,
  P2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfo,
  Sd_RemoteNodeIdType RemoteNodeIndex,
  uint8 SdInstanceIndex
);

STATIC FUNC(void, SD_CODE) Sd_Receive_handleEntryFindService
(
  P2CONST(uint8, AUTOMATIC, SD_APPL_DATA) entryPtr,
  P2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) entryOptionInfoPtr,
  Sd_RemoteNodeIdType RemoteNodeIndex,
  uint8 SdInstanceIndex
);

STATIC FUNC(void, SD_CODE) Sd_Receive_handleEntrySubscribeEventgroup
(
  P2CONST(uint8, AUTOMATIC, SD_APPL_DATA) entryPtr,
  P2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) entryOptionInfoPtr,
  Sd_RemoteNodeIdType RemoteNodeIndex,
  uint8 SdInstanceIndex
);

STATIC FUNC(void, SD_CODE) Sd_Receive_handleEntryOfferService
(
  CONSTP2CONST(uint8, AUTOMATIC, SD_APPL_DATA) entryPtr,
  CONSTP2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) entryOptionInfoPtr,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  const uint8 SdInstanceIndex,
  const boolean isMulticast
);

STATIC FUNC(void, SD_CODE) Sd_Receive_handleEntrySubscribeEventgroupAck
(
  P2CONST(uint8, AUTOMATIC, SD_APPL_DATA) entryPtr,
  P2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) entryOptionInfoPtr,
  Sd_RemoteNodeIdType RemoteNodeIndex,
  uint8 SdInstanceIndex
);



#define SD_STOP_SEC_CODE
#include <Sd_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define SD_START_SEC_CODE
#include <Sd_MemMap.h>



/* Module API functions */

FUNC(void, SD_CODE) Sd_RxIndication
(
  PduIdType RxPduId,
  P2VAR(PduInfoType, AUTOMATIC, SD_APPL_DATA) PduInfoPtr
)
{
  DBG_SD_RXINDICATION_ENTRY(RxPduId,PduInfoPtr);

#if (SD_DEV_ERROR_DETECT == STD_ON)

  if (Sd_Initialized == FALSE)
  {
    SD_DET_REPORT_ERROR(SD_RXINDICATION_SVCID,SD_E_NOT_INITIALIZED);
  }
  else if (PduInfoPtr == NULL_PTR)
  {
    SD_DET_REPORT_ERROR(SD_RXINDICATION_SVCID,SD_E_PARAM_POINTER);
  }
  else if (PduInfoPtr->SduDataPtr == NULL_PTR)
  {
    SD_DET_REPORT_ERROR(SD_RXINDICATION_SVCID,SD_E_PARAM_POINTER);
  }
  else
#endif
  {
    if ((RxPduId >> 1) < SdConfigPtr->SdInstanceCount)
    {
      TcpIp_SockAddrInet6Type RemoteAddr;
      const uint8 InstanceIndex = (uint8)(RxPduId >> 1);
      SoAd_SoConIdType SoConId;
      boolean IsMulticast;
      CONSTP2CONST(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr = &Sd_InstanceStates[InstanceIndex];
      P2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceConfigPtr = &SD_CFG_REF(SdInstance_t, SdInstancesRef, 0U);
      InstanceConfigPtr =  &InstanceConfigPtr[InstanceIndex];

      if ((RxPduId & 0x1U) != 0)
      {
        IsMulticast = FALSE;
        SoConId = InstanceConfigPtr->SdInstanceRxUniCastSoCon;
      }
      else
      {
        IsMulticast = TRUE;
        SoConId = InstanceConfigPtr->SdInstanceRxMultiCastSoCon;
      }

      if( (InstanceStatePtr->RxMultiCastIpAddrState == TCPIP_IPADDR_STATE_ASSIGNED) && (InstanceStatePtr->TxIpAddrState == TCPIP_IPADDR_STATE_ASSIGNED) )
      {
        RemoteAddr.domain = Sd_GetInstanceAddressDomain(InstanceIndex);

        /* Deviation MISRAC2012-1 */
        if (SoAd_GetRemoteAddr(SoConId,(P2VAR(TcpIp_SockAddrType, TYPEDEF, SD_APPL_DATA))&RemoteAddr) == E_OK)
        {
          Sd_RemoteNodeIdType RemoteNodeIndex = Sd_AssignRemoteNodeState(&RemoteAddr, InstanceConfigPtr);

          /* Maximum number of remote nodes exceeded; message is ignored */
          if(RemoteNodeIndex != SD_INVALID_REMOTENODE_ID)
          {
            Sd_Receive_processMessage(InstanceConfigPtr, PduInfoPtr, RemoteNodeIndex, InstanceIndex, IsMulticast);
          }
        }
      }
#if (SD_DEV_ERROR_DETECT == STD_ON)
      else
      { /* generate a development error to indicate that initialization of SD module has not been completed yet */
        SD_DET_REPORT_ERROR(SD_RXINDICATION_SVCID, SD_E_IPADDR_ASSIGNMENT_NOT_FINISHED);
      }
#endif
      /* perform reset to wildcard after processing the messages
       * -> allows to access the remote address when the message is processed */
       SoAd_ReleaseRemoteAddr(SoConId);
    }
#if (SD_DEV_ERROR_DETECT == STD_ON)
    else
    {
      SD_DET_REPORT_ERROR(SD_RXINDICATION_SVCID,SD_E_INV_ID);
    }
#endif
  }
  DBG_SD_RXINDICATION_EXIT(RxPduId,PduInfoPtr);
}


/*==================[internal function definitions]=========================*/



STATIC FUNC(void, SD_CODE) Sd_Receive_processMessage
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  P2VAR(PduInfoType, AUTOMATIC, SD_APPL_DATA) pduInfoPtr,
  Sd_RemoteNodeIdType RemoteNodeIndex,
  uint8 InstanceIndex,
  boolean IsMulticast
)
{
  uint16 EntriesCount;
  uint16 OptionInfos[SD_RECEIVE_MAXNUM_OPTIONS];
  uint8 OptionsCount = SD_RECEIVE_MAXNUM_OPTIONS;
#if( (SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DEM) || ((SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON)) || (SD_MALFORMED_MSG_CALLBACK == STD_ON) )
  uint8 DemErrorCode = 0U;
#endif
  SdOptionPtrType OptionStartPtr = Sd_Receive_checkMessage(pduInfoPtr, &EntriesCount, OptionInfos, &OptionsCount);

  if(OptionStartPtr != NULL_PTR)
  {
    P2CONST(uint8, AUTOMATIC, SD_APPL_DATA) DataPtr = pduInfoPtr->SduDataPtr;
    uint8 Flags;

    SD_READ(SD_HEADER_FLAGS,DataPtr,Flags);

    if((Flags & SD_FLAG_UNICAST) != 0U)
    {
#if( (SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DEM) || ((SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON)) || (SD_MALFORMED_MSG_CALLBACK == STD_ON) )
      P2FUNC(uint8, SD_CODE, Sd_Receive_handleEntry_PtrFunc)
#else
      P2FUNC(void, SD_CODE, Sd_Receive_handleEntry_PtrFunc)
#endif
       (P2CONST(uint8, AUTOMATIC, SD_APPL_DATA),
        P2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA),
        Sd_RemoteNodeIdType,
        uint8);
      {
        CONSTP2VAR(Sd_RemoteNodeStateType,AUTOMATIC,SD_APPL_DATA) RemoteNodeStatePtr = Sd_GetRemoteNodeState(RemoteNodeIndex);
        P2VAR(Sd_RebootStateType,AUTOMATIC,SD_APPL_DATA) RebootStatePtr;
        const boolean RemoteRebootFlag = ((Flags & SD_FLAG_REBOOT) != 0U) ? TRUE : FALSE;
        uint16 SessionID;

        SD_READ(SD_HEADER_SESSIONID,DataPtr, SessionID); /* this variable holds the session ID */

        if(IsMulticast != FALSE)
        {
          RebootStatePtr = &RemoteNodeStatePtr->RemoteMulticastRebootState;

          Sd_Receive_handleEntry_PtrFunc = Sd_Receive_handleMulticastEntry;
        }
        else /* if(IsMulticast == FALSE) */
        {
          RebootStatePtr = &RemoteNodeStatePtr->RemoteUnicastRebootState;

          Sd_Receive_handleEntry_PtrFunc = Sd_Receive_handleUnicastEntry;

          /* !LINKSTO Sd.EB.ConnectionReady.Flag,1 */
          RemoteNodeStatePtr->IsConnectionReady = TRUE;
          RemoteNodeStatePtr->IsConnectionReadyTimer = 0U;
        }
        /* !LINKSTO Sd.Design.Receive.RebootDetection.Unicast,1 */
        /* !LINKSTO Sd.Design.Receive.RebootDetection.Multicast,1 */
        if(((RemoteRebootFlag == TRUE) && (RebootStatePtr->RebootFlag == FALSE)) ||
           ((RemoteRebootFlag == TRUE) && (RebootStatePtr->SessionId >= SessionID))
          )
        {
          CONSTP2VAR(Sd_InstanceStateType, AUTOMATIC, SD_APPL_DATA) InstanceStatesPtr = &Sd_InstanceStates[InstanceIndex];

          Sd_ResetRemoteNodeRxState(InstanceStatesPtr, RemoteNodeIndex);

          if(InstanceStatesPtr->ResponseTimersActiveCount == 0U)
          {
            InstanceStatesPtr->ResponseTimersDelta = 0U;
            InstanceStatesPtr->ResponseTimersDeltaTimer = 0U;
          }
          Sd_ServerService_resetRemoteConnection(InstanceCfgPtr, RemoteNodeIndex);
          Sd_ClientService_resetRemoteConnection(InstanceCfgPtr, InstanceStatesPtr, RemoteNodeIndex);
        }
        RebootStatePtr->RebootFlag = RemoteRebootFlag;
        RebootStatePtr->SessionId = SessionID;

        Sd_Send_setupBuffer(&RemoteNodeStatePtr->TxState, InstanceIndex);
      }

      {
        Sd_EntryOptionInfoType EntryOptionData;
        uint16 EntryIndex;

        /* set pointer at the beginning of the first entry */
        DataPtr = &DataPtr[SD_HEADER_SIZE + SD_HEADER_ENTRYLEN_SIZE];

        /* this local variable holds now the index of the entry that is being processed */
        for(EntryIndex = 0; EntryIndex < EntriesCount; EntryIndex++)
        {
          Sd_Receive_getEntryOptions(DataPtr, OptionStartPtr, OptionInfos, OptionsCount, &EntryOptionData, InstanceIndex);
#if( (SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DEM) || ((SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON)) || (SD_MALFORMED_MSG_CALLBACK == STD_ON) )
          DemErrorCode |= Sd_Receive_handleEntry_PtrFunc(DataPtr, &EntryOptionData, RemoteNodeIndex, InstanceIndex);
#else
          Sd_Receive_handleEntry_PtrFunc(DataPtr, &EntryOptionData, RemoteNodeIndex, InstanceIndex);
#endif
          DataPtr = &DataPtr[SD_ENTRY_SIZE];
        }
      }
      Sd_Send_flushBuffer();
    }
  }
#if( (SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DEM) || ((SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON)) || (SD_MALFORMED_MSG_CALLBACK == STD_ON) )
  else
  {
    DemErrorCode = (uint8)DEM_MALFORMED_MSG_BITMAP;
  }
#endif

#if( (SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DEM) || ((SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON)) || (SD_MALFORMED_MSG_CALLBACK == STD_ON))
  if(DemErrorCode == (uint8)DEM_MALFORMED_MSG_BITMAP)
  {
#if(SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DEM)
    /* If OptionStartPtr is a NULL_PTR, it is a indication for an invalid SOME/IP-SD message.
     * Therefore, error shall be reported to DEM */
    SD_DEM_REPORT_ERROR_STATUS(InstanceCfgPtr->SdDemMalformedMsgId, DEM_EVENT_STATUS_FAILED);
#elif (SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON)
    SD_DET_REPORT_ERROR(SD_RXINDICATION_SVCID, SD_E_MALFORMED_MSG_DEMTODET);
#endif

#if(SD_MALFORMED_MSG_CALLBACK == STD_ON)
    /* !LINKSTO Sd.EB.CallBack.MalformedMsg,1 */
    Sd_User_Malformed_Msg_Fp();
#endif /* SD_MALFORMED_MSG_CALLBACK == STD_ON */
  }
#endif
}



STATIC FUNC_P2CONST(uint8, AUTOMATIC, SD_CODE) Sd_Receive_checkMessage
(
  CONSTP2CONST(PduInfoType, AUTOMATIC, SD_APPL_DATA) pduInfoPtr,
  CONSTP2VAR(uint16, AUTOMATIC, SD_APPL_DATA) entriesCount,
  CONSTP2VAR(uint16, AUTOMATIC, SD_APPL_DATA) optionInfoPtr,
  CONSTP2VAR(uint8, AUTOMATIC, SD_APPL_DATA) optionCountPtr
)
{
  SdOptionPtrType ValidMessagePtr;
  const uint32 PduLength = (uint32)pduInfoPtr->SduLength;

  if(PduLength < (SD_HEADER_SIZE + SD_HEADER_ENTRYLEN_SIZE + SD_HEADER_OPTSLEN_SIZE))
  {
    ValidMessagePtr = NULL_PTR;
  }
  else
  {
    CONSTP2CONST(uint8, AUTOMATIC, SD_APPL_DATA) DataPtr = pduInfoPtr->SduDataPtr;
    uint32 LengthOfEntriesArray;
    uint16 ClientId;
    uint8 ProtocolVersion;
    uint8 InterfaceVersion;
    uint8 MessageType;
    uint8 ReturnCode;

    SD_READ(SD_HEADER_CLIENTID,DataPtr,ClientId);
    SD_READ(SD_HEADER_PROTOCOLVERSION,DataPtr,ProtocolVersion);
    SD_READ(SD_HEADER_INTERFACEVERSION,DataPtr,InterfaceVersion);
    SD_READ(SD_HEADER_MESSAGETYPE,DataPtr,MessageType);
    SD_READ(SD_HEADER_RETURNCODE,DataPtr,ReturnCode);

    SD_READ32U(SD_HEADER_SIZE, DataPtr, LengthOfEntriesArray);
    *entriesCount = (uint16)(LengthOfEntriesArray / SD_ENTRY_SIZE);

    if(ClientId != SD_CLIENTID)
    {
      ValidMessagePtr = NULL_PTR;
    }
    else if(ProtocolVersion != SD_PROTOCOLVERSION)
    {
      ValidMessagePtr = NULL_PTR;
    }
    else if(InterfaceVersion != SD_INTERFACEVERSION)
    {
      ValidMessagePtr = NULL_PTR;
    }
    else if(MessageType != SD_MESSAGETYPE)
    {
      ValidMessagePtr = NULL_PTR;
    }
    else if(ReturnCode != SD_RETURNCODE)
    {
      ValidMessagePtr = NULL_PTR;
    }
    else if( LengthOfEntriesArray > (PduLength - (SD_HEADER_SIZE + SD_HEADER_ENTRYLEN_SIZE + SD_HEADER_OPTSLEN_SIZE)) )
    {
      ValidMessagePtr = NULL_PTR;
    }
    else if( LengthOfEntriesArray != ((uint32)(*entriesCount) * SD_ENTRY_SIZE) )
    {
      ValidMessagePtr = NULL_PTR;
    }
    else
    {
      uint32 LengthOfOptions;
      SD_READ32U(SD_HEADER_SIZE + SD_HEADER_ENTRYLEN_SIZE + LengthOfEntriesArray, DataPtr, LengthOfOptions);

      if(  LengthOfOptions > ( PduLength - (SD_HEADER_SIZE + SD_HEADER_ENTRYLEN_SIZE + LengthOfEntriesArray + SD_HEADER_OPTSLEN_SIZE) )  )
      {
        ValidMessagePtr = NULL_PTR;
      }
      else
      {
        CONSTP2CONST(uint8, AUTOMATIC, SD_APPL_DATA) OptionsPtr = &DataPtr[SD_HEADER_SIZE + SD_HEADER_ENTRYLEN_SIZE + LengthOfEntriesArray + SD_HEADER_OPTSLEN_SIZE];
        uint16 OptionStartIndex = 0U;
        uint8 optionCounter = 0U;

        while(  (optionCounter < *optionCountPtr) &&
                ( ((uint32)OptionStartIndex + SD_OPTIONCOMMON_SIZE) <= LengthOfOptions )
             )
        {
          uint16 OptionLengthTotal = Sd_Receive_checkOption(&OptionsPtr[OptionStartIndex]);

          if(  (OptionLengthTotal == SD_RECEIVE_INVALID_OPTION_LENGTH) ||
               ( ((uint32)OptionStartIndex + (uint32)OptionLengthTotal) > LengthOfOptions )
            )
          {
            break;
          }
          else
          {
            optionInfoPtr[optionCounter] = OptionStartIndex;
            ++optionCounter;
            OptionStartIndex += OptionLengthTotal;
          }
        }

        *optionCountPtr = optionCounter;
        ValidMessagePtr = OptionsPtr;
      }
    }
  }

  return ValidMessagePtr;
}



STATIC FUNC(uint16, SD_CODE) Sd_Receive_checkOption
(
    P2CONST(uint8, AUTOMATIC, SD_APPL_DATA) optionPtr
)
{
  uint16 OptionLengthTotal;
  uint16 OptionLength;
  uint8 OptionType;

  SD_READ(SD_OPTIONCOMMON_LENGTH,optionPtr,OptionLength);
  SD_READ(SD_OPTIONCOMMON_TYPE,optionPtr,OptionType);

  OptionLengthTotal = (uint16)(OptionLength + SD_OPTIONCOMMON_SIZE);

  switch(OptionType)
  {
    case SD_OPTIONTYPE_CONFIGURATION:
    {
      if(OptionLengthTotal < SD_OPTIONCONFIG_MINSIZE)
      {
        OptionLengthTotal = SD_RECEIVE_INVALID_OPTION_LENGTH;
      }
      break;
    }
    case SD_OPTIONTYPE_IPV4:
    case SD_OPTIONTYPE_IPV4MULTI:
    {
      if(OptionLengthTotal != SD_OPTIONIPV4_SIZE)
      {
        OptionLengthTotal = SD_RECEIVE_INVALID_OPTION_LENGTH;
      }
      break;
    }

    case SD_OPTIONTYPE_IPV6:
    case SD_OPTIONTYPE_IPV6MULTI:
    {
      if(OptionLengthTotal != SD_OPTIONIPV6_SIZE)
      {
        OptionLengthTotal = SD_RECEIVE_INVALID_OPTION_LENGTH;
      }
      break;
    }
    default: /* unknown option type */
    { /* initialize return value to an invalid length */
      OptionLengthTotal = SD_RECEIVE_INVALID_OPTION_LENGTH;
      break;
    }
  }

  return OptionLengthTotal;
}



STATIC FUNC(void, SD_CODE) Sd_Receive_getEntryOptions
(
  P2CONST(uint8, AUTOMATIC, SD_APPL_DATA) entryPtr,
  SdOptionPtrType OptionsStartAddr,
  P2CONST(uint16, AUTOMATIC, SD_APPL_DATA) optionInfoPtr,
  uint8 optionCount,
  P2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) entryOptionInfoPtr,
  uint8 InstanceIndex
)
{
  {
    uint8 i;

    entryOptionInfoPtr->MulticastOption.domain = TCPIP_AF_UNSPEC;
    entryOptionInfoPtr->UdpEndpointOption.domain = TCPIP_AF_UNSPEC;
    entryOptionInfoPtr->TcpEndpointOption.domain = TCPIP_AF_UNSPEC;
    for(i = 0U; i < SD_OPTIONCONFIG_ARY_SIZE; i++)
    {
      entryOptionInfoPtr->ConfigOptions[i] = NULL_PTR;
    }
  }

  {
    uint8 Index1stOpt;
    uint8 Index2ndOpt;
    uint8 Count1stOpts;
    uint8 Count2ndOpts;

    SD_READ(SD_ENTRY_INDEX1OP,entryPtr,Index1stOpt);
    SD_READ(SD_ENTRY_INDEX2OP,entryPtr,Index2ndOpt);
    SD_READ(SD_ENTRY_NUMOPTS,entryPtr,Count2ndOpts);
    Count1stOpts = (Count2ndOpts >> 4) & 0x0FU;
    Count2ndOpts = Count2ndOpts & 0x0FU;

    {
      const TcpIp_DomainType localDomain = Sd_GetInstanceAddressDomain(InstanceIndex);
      entryOptionInfoPtr->OptionStatus = SD_OPTION_STATUS_OK;

      /* check all options referenced by this entry exist */
      if(Count1stOpts > 0U)
      { /* if there are 1st options make sure the number of 1st options does not exceed the total number of options*/
        if((Index1stOpt + Count1stOpts) > optionCount)
        { /* impossible case, 1st options exceed the array of options this is an error */
          entryOptionInfoPtr->OptionStatus = SD_OPTION_STATUSFLAG_ERROR;
        }
        else
        {
          if (localDomain == TCPIP_AF_INET)
          {
            Sd_Receive_processOptionIPv4run(entryOptionInfoPtr, OptionsStartAddr, optionInfoPtr, Index1stOpt, Count1stOpts);
          }
          else /* if (localDomain == TCPIP_AF_INET6) */
          {
#if (SD_DEV_ERROR_DETECT == STD_ON)
            SD_PRECONDITION_ASSERT( (localDomain == TCPIP_AF_INET6), SD_RECEVIVEGETENTRYOPTIONS_SVCID );
#endif
            Sd_Receive_processOptionIPv6run(entryOptionInfoPtr, OptionsStartAddr, optionInfoPtr, Index1stOpt, Count1stOpts);
          }
        }
      }

      if(Count2ndOpts > 0U)
      { /* if there are 2nd options make sure the number of 1st options does not exceed the total number of options*/
        if((Index2ndOpt + Count2ndOpts) > optionCount)
        { /* impossible case, 1st options exceed the array of options this is an error */
          entryOptionInfoPtr->OptionStatus = SD_OPTION_STATUSFLAG_ERROR;
        }
        else
        {
          if (localDomain == TCPIP_AF_INET)
          {
             Sd_Receive_processOptionIPv4run(entryOptionInfoPtr, OptionsStartAddr, optionInfoPtr, Index2ndOpt, Count2ndOpts);
          }
          else /* if (localDomain == TCPIP_AF_INET6) */
          {
#if (SD_DEV_ERROR_DETECT == STD_ON)
            SD_PRECONDITION_ASSERT( (localDomain == TCPIP_AF_INET6), SD_RECEVIVEGETENTRYOPTIONS_SVCID );
#endif
            Sd_Receive_processOptionIPv6run(entryOptionInfoPtr, OptionsStartAddr, optionInfoPtr, Index2ndOpt, Count2ndOpts);
          }
        }
      }
    }
  }
}


STATIC FUNC(void, SD_CODE) Sd_Receive_processOptionIPv4run
(
  CONSTP2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) entryOptionInfoPtr,
  SdOptionPtrType OptionsStartAddr,
  CONSTP2CONST(uint16, AUTOMATIC, SD_APPL_DATA) optionInfoPtr,
  const uint8 optionIndex,
  const uint8 optionCount
)
{
  const uint8 OptionEnd = optionIndex + optionCount;
  uint8 optionIdx = optionIndex;

  for(; optionIdx < OptionEnd; optionIdx++)
  {
    CONSTP2CONST(uint8,AUTOMATIC,SD_APPL_DATA) OptionPtr = &OptionsStartAddr[optionInfoPtr[optionIdx]];
    uint8 OptionType;

    SD_READ(SD_OPTIONCOMMON_TYPE,OptionPtr,OptionType);

    if(OptionType == SD_OPTIONTYPE_CONFIGURATION)
    {
      entryOptionInfoPtr->OptionStatus |= Sd_Receive_readConfigOption(OptionPtr, entryOptionInfoPtr->ConfigOptions);
    }
    else if(OptionType == SD_OPTIONTYPE_IPV4)
    {
      uint8 Protocol;
      SD_READ(SD_OPTIONIPV4_PROTOCOL,OptionPtr,Protocol);

      if(Protocol == SD_DOMAIN_TCP)
      {
        entryOptionInfoPtr->OptionStatus |= Sd_Receive_readInetAddr(OptionPtr, &entryOptionInfoPtr->TcpEndpointOption);
      }
      else if(Protocol == SD_DOMAIN_UDP)
      {
        entryOptionInfoPtr->OptionStatus |= Sd_Receive_readInetAddr(OptionPtr, &entryOptionInfoPtr->UdpEndpointOption);
      }
      else
      {
        entryOptionInfoPtr->OptionStatus |= SD_OPTION_STATUSFLAG_INVALIDENDPOINT;
      }
    }
    else if(OptionType == SD_OPTIONTYPE_IPV4MULTI)
    {
      uint8 Protocol;
      uint8 Addr1stOctet;

      SD_READ(SD_OPTIONIPV4_PROTOCOL,OptionPtr,Protocol);
      SD_READ8U(SD_OPTIONIPV4_ADDRESS_OFFSET, OptionPtr, Addr1stOctet);

      if((Protocol == SD_DOMAIN_UDP) &&
        ((Addr1stOctet & SD_IPV4_MULTICAST_1STOCT_MASK) == SD_IPV4_MULTICAST_1STOCT_VALUE))
      {
        entryOptionInfoPtr->OptionStatus |= Sd_Receive_readInetAddr(OptionPtr, &entryOptionInfoPtr->MulticastOption);
      }
      else
      {
        /* Protocol for Multicast endpoint must be UDP and address in multicast IP range*/
        entryOptionInfoPtr->OptionStatus |= SD_OPTION_STATUSFLAG_INVALIDENDPOINT;
      }
    }
    else /* if( (OptionType == SD_OPTIONTYPE_IPV6) || (OptionType == SD_OPTIONTYPE_IPV6MULTI) ) */
    {
      /* within IPv4 domain a IPv6 domain option has been received */
      entryOptionInfoPtr->OptionStatus |= SD_OPTION_STATUSFLAG_INVALIDENDPOINT;
    }
  }
}


STATIC FUNC(void, SD_CODE) Sd_Receive_processOptionIPv6run
(
  CONSTP2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) entryOptionInfoPtr,
  SdOptionPtrType OptionsStartAddr,
  CONSTP2CONST(uint16, AUTOMATIC, SD_APPL_DATA) optionInfoPtr,
  const uint8 optionIndex,
  const uint8 optionCount
)
{
  const uint8 OptionEnd = optionIndex + optionCount;
  uint8 optionIdx = optionIndex;

  for(; optionIdx < OptionEnd; optionIdx++)
  {
    CONSTP2CONST(uint8,AUTOMATIC,SD_APPL_DATA) OptionPtr = &OptionsStartAddr[optionInfoPtr[optionIdx]];
    uint8 OptionType;

    SD_READ(SD_OPTIONCOMMON_TYPE,OptionPtr,OptionType);

    if(OptionType == SD_OPTIONTYPE_CONFIGURATION)
    {
      entryOptionInfoPtr->OptionStatus |= Sd_Receive_readConfigOption(OptionPtr, entryOptionInfoPtr->ConfigOptions);
    }
    else if(OptionType == SD_OPTIONTYPE_IPV6)
    {
      uint8 Protocol;
      SD_READ(SD_OPTIONIPV6_PROTOCOL,OptionPtr,Protocol);

      if(Protocol == SD_DOMAIN_TCP)
      {
        entryOptionInfoPtr->OptionStatus |= Sd_Receive_readInet6Addr(OptionPtr, &entryOptionInfoPtr->TcpEndpointOption);
      }
      else if(Protocol == SD_DOMAIN_UDP)
      {
        entryOptionInfoPtr->OptionStatus |= Sd_Receive_readInet6Addr(OptionPtr, &entryOptionInfoPtr->UdpEndpointOption);
      }
      else
      {
        entryOptionInfoPtr->OptionStatus |= SD_OPTION_STATUSFLAG_INVALIDENDPOINT;
      }
    }
    else if(OptionType == SD_OPTIONTYPE_IPV6MULTI)
    {
      uint8 Protocol;
      uint8 Addr1stOctet;

      SD_READ(SD_OPTIONIPV6_PROTOCOL,OptionPtr,Protocol);
      SD_READ8U(SD_OPTIONIPV6_ADDRESS_OFFSET, OptionPtr, Addr1stOctet);

      if((Protocol == SD_DOMAIN_UDP) &&
       ((Addr1stOctet & SD_IPV6_MULTICAST_1STWORD_MASK) == SD_IPV6_MULTICAST_1STWORD_VALUE))
      {
        entryOptionInfoPtr->OptionStatus |= Sd_Receive_readInet6Addr(OptionPtr, &entryOptionInfoPtr->MulticastOption);
      }
      else
      {
        /* Protocol for Multicast endpoint must be UDP and address in multicast IP range*/
        entryOptionInfoPtr->OptionStatus |= SD_OPTION_STATUSFLAG_INVALIDENDPOINT;
      }
    }
    else /* if( (OptionType == SD_OPTIONTYPE_IPV4) || (OptionType == SD_OPTIONTYPE_IPV4MULTI) ) */
    {
      /* within IPv6 domain a IPv4 domain option has been received */
      entryOptionInfoPtr->OptionStatus |= SD_OPTION_STATUSFLAG_INVALIDENDPOINT;
    }
  }
}



STATIC FUNC(uint8, SD_CODE) Sd_Receive_readInetAddr
(
    P2CONST(uint8,AUTOMATIC,SD_APPL_DATA) OptionPtr,
    P2VAR(TcpIp_SockAddrInet6Type,AUTOMATIC,SD_APPL_DATA) InetAddrPtr
)
{
  uint8 OptionStatus;

  if(InetAddrPtr->domain == TCPIP_AF_UNSPEC)
  {
    InetAddrPtr->domain = TCPIP_AF_INET;
    SD_READ32U_NE(SD_OPTIONIPV4_ADDRESS_OFFSET,OptionPtr,InetAddrPtr->addr[0]);
    SD_READ(SD_OPTIONIPV4_PORT,OptionPtr,InetAddrPtr->port);

    if( (InetAddrPtr->port != 0U) && (InetAddrPtr->addr[0] != 0U) )
    {
      OptionStatus = SD_OPTION_STATUS_OK;
    }
    else
    {
      OptionStatus = SD_OPTION_STATUSFLAG_INVALIDENDPOINT;
    }
  }
  else
  {
    OptionStatus = SD_OPTION_STATUSFLAG_CONFLICT;
  }
  return OptionStatus;
}

STATIC FUNC(uint8, SD_CODE) Sd_Receive_readInet6Addr
(
    P2CONST(uint8,AUTOMATIC,SD_APPL_DATA) OptionPtr,
    P2VAR(TcpIp_SockAddrInet6Type,AUTOMATIC,SD_APPL_DATA) InetAddrPtr
)
{
  uint8 OptionStatus;

  if(InetAddrPtr->domain == TCPIP_AF_UNSPEC)
  {
    InetAddrPtr->domain = TCPIP_AF_INET6;
    SD_READ128U_NE(SD_OPTIONIPV6_ADDRESS_OFFSET,OptionPtr,InetAddrPtr->addr[0]);
    SD_READ(SD_OPTIONIPV6_PORT,OptionPtr,InetAddrPtr->port);

    /* check that at least one byte is not zero, then its a valid IPv6 address) */
    if(   (   (InetAddrPtr->port != 0U) &&
          (  ( (InetAddrPtr->addr[0] != 0U) || (InetAddrPtr->addr[1] != 0U) || (InetAddrPtr->addr[2] != 0U) || (InetAddrPtr->addr[3] != 0U) ) != FALSE  )   )
      )
    {
      OptionStatus = SD_OPTION_STATUS_OK;
    }
    else
    {
      OptionStatus = SD_OPTION_STATUSFLAG_INVALIDENDPOINT;
    }
  }
  else
  {
    OptionStatus = SD_OPTION_STATUSFLAG_CONFLICT;
  }
  return OptionStatus;
}


STATIC FUNC(uint8, SD_CODE) Sd_Receive_readConfigOption
(
  P2CONST(uint8,AUTOMATIC,SD_APPL_DATA) OptionPtr,
  P2VAR(SdConfigOptionPtrType,AUTOMATIC,SD_APPL_DATA) ConfigOptionPtr
)
{
  uint16 OptionLength;
  uint8 OptionStatus = SD_OPTION_STATUSFLAG_ERROR;

  /* get the configuration string length (Option Length field value - sizeof(Reserved field) */
  SD_READ(SD_OPTIONCOMMON_LENGTH, OptionPtr, OptionLength);

  /* only one configuration option per entry is supported */
  if (ConfigOptionPtr[0] == NULL_PTR)
  {
    const uint32 OptionLengthTotal = (uint32)OptionLength + (uint32)SD_OPTIONCOMMON_SIZE;

    if (OptionPtr[OptionLengthTotal - 1U] == 0U)
    {
      uint16 ItemLenOffset;
      /* Check the sanity of config option string */
      for(ItemLenOffset = SD_OPTIONCONFIG_STRING_OFFSET;
          ItemLenOffset < OptionLengthTotal;
          ItemLenOffset += ((uint16)OptionPtr[ItemLenOffset] + SD_OPTIONCONFIG_STRINGLENGTH_LENGTH))
      {
        if(OptionPtr[ItemLenOffset] == 0U)
        {
          break;
        }
      }

      if(ItemLenOffset == (OptionLengthTotal - 1U))
      {
        ConfigOptionPtr[0] = &OptionPtr[SD_OPTIONCONFIG_STRING_OFFSET];
        OptionStatus = SD_OPTION_STATUS_OK;
      }
    }
  }

  return OptionStatus;
}


#if( (SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DEM) || ((SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON)) || (SD_MALFORMED_MSG_CALLBACK == STD_ON) )
STATIC FUNC(uint8, SD_CODE) Sd_Receive_handleMulticastEntry
#else
STATIC FUNC(void, SD_CODE) Sd_Receive_handleMulticastEntry
#endif
(
  P2CONST(uint8, AUTOMATIC, SD_APPL_DATA) entryPtr,
  P2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfo,
  Sd_RemoteNodeIdType RemoteNodeIndex,
  uint8 SdInstanceIndex
)
{
#if( (SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DEM) || ((SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON)) || (SD_MALFORMED_MSG_CALLBACK == STD_ON) )
  uint8 RetDemErrCode = 0U;
#endif
  if(EntryOptionInfo->OptionStatus == SD_OPTION_STATUS_OK)
  {
    /* read entry type value from entry's start address in the whole message's buffer */
    const uint8 EntryType = READ_ENTRY_TYPE(entryPtr);

    if(EntryType == SD_ENTRYTYPE_FINDSERVICE)
    {
      Sd_Receive_handleEntryFindService(entryPtr, EntryOptionInfo, RemoteNodeIndex, SdInstanceIndex);
    }
    else if(EntryType == SD_ENTRYTYPE_OFFERSERVICE)
    {
      Sd_Receive_handleEntryOfferService(entryPtr, EntryOptionInfo, RemoteNodeIndex, SdInstanceIndex, TRUE);
    }
    else if(EntryType == SD_ENTRYTYPE_SUBSCRIBEEVENTGROUP)
    {
      Sd_Receive_handleEntrySubscribeEventgroup(entryPtr, EntryOptionInfo, RemoteNodeIndex, SdInstanceIndex);
    }
#if( (SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DEM) || ((SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON)) || (SD_MALFORMED_MSG_CALLBACK == STD_ON) )
    else
    { /* unknown or not allowed entry type, should not happen, set malformed message code */
      RetDemErrCode = (uint8)DEM_MALFORMED_MSG_BITMAP;
    }
  }
  else
  { /* this entry refers to a wrong or mismatched option */
    RetDemErrCode = (uint8)DEM_MALFORMED_MSG_BITMAP;
  }
  return RetDemErrCode;
#else
    else
    { /* unknown or not allowed entry type, should not happen */
    }
  }
  else
  { /* this entry refers to a wrong or mismatched option */
  }
#endif
}



#if( (SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DEM) || ((SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON)) || (SD_MALFORMED_MSG_CALLBACK == STD_ON) )
STATIC FUNC(uint8, SD_CODE) Sd_Receive_handleUnicastEntry
#else
STATIC FUNC(void, SD_CODE) Sd_Receive_handleUnicastEntry
#endif
(
  P2CONST(uint8, AUTOMATIC, SD_APPL_DATA) entryPtr,
  P2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) EntryOptionInfo,
  Sd_RemoteNodeIdType RemoteNodeIndex,
  uint8 SdInstanceIndex
)
{
#if( (SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DEM) || ((SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON)) || (SD_MALFORMED_MSG_CALLBACK == STD_ON) )
  uint8 RetDemErrCode = 0U;
#endif
  if(EntryOptionInfo->OptionStatus == SD_OPTION_STATUS_OK)
  {
    /* read entry type value from entry's start address in the whole message's buffer */
    const uint8 EntryType = READ_ENTRY_TYPE(entryPtr);

    switch(EntryType)
    {
      case SD_ENTRYTYPE_OFFERSERVICE:
        Sd_Receive_handleEntryOfferService(entryPtr, EntryOptionInfo, RemoteNodeIndex, SdInstanceIndex, FALSE);
      break;

      case SD_ENTRYTYPE_SUBSCRIBEEVENTGROUP:
        Sd_Receive_handleEntrySubscribeEventgroup(entryPtr, EntryOptionInfo, RemoteNodeIndex, SdInstanceIndex);
      break;

      case SD_ENTRYTYPE_SUBSCRIBEEVENTGROUPACK:
        Sd_Receive_handleEntrySubscribeEventgroupAck(entryPtr, EntryOptionInfo, RemoteNodeIndex, SdInstanceIndex);
      break;

      case SD_ENTRYTYPE_FINDSERVICE:
        Sd_Receive_handleEntryFindService(entryPtr, EntryOptionInfo, RemoteNodeIndex, SdInstanceIndex);
      break;
      /* CHECK: NOPARSE */
      default:
      /* unknown or not allowed entry type, should not happen, set malformed message code */
#if( (SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DEM) || ((SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON)) || (SD_MALFORMED_MSG_CALLBACK == STD_ON) )
      RetDemErrCode = (uint8)DEM_MALFORMED_MSG_BITMAP;
#endif
      break;
      /* CHECK: PARSE */
    }
  }
  else
  {
    if(entryPtr[SD_ENTRY_TYPE_OFFSET] == SD_ENTRYTYPE_SUBSCRIBEEVENTGROUP)
    {
      uint32 TTL;
      SD_READ(SD_ENTRY_TTL,entryPtr, TTL);

      if(TTL != 0U)
      {
        uint16 EventgroupID;
        uint16 ServiceID;
        uint16 InstanceID;
        uint8  MajorVersion;

        /* this entry refers to a wrong or mismatched option */
        SD_READ(SD_ENTRY_SERVICEID, entryPtr, ServiceID);
        SD_READ(SD_ENTRY_INSTANCEID, entryPtr, InstanceID);
        SD_READ(SD_ENTRY_MAJORVERSION, entryPtr, MajorVersion);
        SD_READ(SD_ENTRYT2_EVENTGROUPID,entryPtr,EventgroupID);

        Sd_Send_queueSubscribeEventgroupNackUnknown(ServiceID, InstanceID, EventgroupID, 0U, MajorVersion);
      }
    }
#if( (SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DEM) || ((SD_DEM_DETECT_SD_E_MALFORMED_MSG == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON)) || (SD_MALFORMED_MSG_CALLBACK == STD_ON) )
    RetDemErrCode = (uint8)DEM_MALFORMED_MSG_BITMAP;
  }
  return RetDemErrCode;
#else
  }
#endif
}



STATIC FUNC(void, SD_CODE) Sd_Receive_handleEntryFindService
(
  P2CONST(uint8, AUTOMATIC, SD_APPL_DATA) entryPtr,
  P2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) entryOptionInfoPtr,
  Sd_RemoteNodeIdType RemoteNodeIndex,
  uint8 SdInstanceIndex
)
{
  Sd_ServiceInfoType ServiceInfo;
  uint32 MinorVersion;

  SD_READ(SD_ENTRY_SERVICEID,entryPtr,ServiceInfo.ServiceID);
  SD_READ(SD_ENTRY_INSTANCEID,entryPtr,ServiceInfo.InstanceID);
  SD_READ(SD_ENTRY_MAJORVERSION,entryPtr,ServiceInfo.MajorVersion);
  SD_READ(SD_ENTRYT1_MINORVERSION,entryPtr,MinorVersion);

  Sd_ServerService_eventFind(
      &ServiceInfo,
      entryOptionInfoPtr,
      MinorVersion,
      RemoteNodeIndex,
      SdInstanceIndex
  );
}



STATIC FUNC(void, SD_CODE) Sd_Receive_handleEntryOfferService
(
  CONSTP2CONST(uint8, AUTOMATIC, SD_APPL_DATA) entryPtr,
  CONSTP2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) entryOptionInfoPtr,
  const Sd_RemoteNodeIdType RemoteNodeIndex,
  const uint8 SdInstanceIndex,
  const boolean isMulticast
)
{
  Sd_ServiceInfoType ServiceInfo;
  uint32 TTL;
  uint32 MinorVersion;

  SD_READ(SD_ENTRY_SERVICEID,entryPtr,ServiceInfo.ServiceID);
  SD_READ(SD_ENTRY_INSTANCEID,entryPtr,ServiceInfo.InstanceID);
  SD_READ(SD_ENTRY_MAJORVERSION,entryPtr,ServiceInfo.MajorVersion);
  SD_READ(SD_ENTRY_TTL,entryPtr,TTL);
  SD_READ(SD_ENTRYT1_MINORVERSION,entryPtr,MinorVersion);

  if(TTL != 0U)
  {
#if(  (SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DEM) || \
      ( (SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON) )  )
    const uint8 ErrorBitmap = Sd_ClientService_eventOfferService(
              &ServiceInfo,
              entryOptionInfoPtr,
              TTL,
              MinorVersion,
              RemoteNodeIndex,
              SdInstanceIndex,
              isMulticast);

    if((ErrorBitmap & DEM_OUT_OF_RESOURCES_UDP_BITMAP) != 0U)
    {
#if(SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DEM)
      P2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr = &SD_CFG_REF(SdInstance_t, SdInstancesRef, 0U);
      SD_DEM_REPORT_ERROR_STATUS(
          InstanceCfgPtr[SdInstanceIndex].SdDemOutOfResId,
          DEM_EVENT_STATUS_FAILED
          );
#elif((SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON))
      SD_DET_REPORT_ERROR(SD_RXINDICATION_SVCID, SD_E_OUT_OF_RES_DEMTODET);
#endif
    }
    if((ErrorBitmap & DEM_OUT_OF_RESOURCES_TCP_BITMAP) != 0U)
    {
#if(SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DEM)
      P2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr = &SD_CFG_REF(SdInstance_t, SdInstancesRef, 0U);
      SD_DEM_REPORT_ERROR_STATUS(
          InstanceCfgPtr[SdInstanceIndex].SdDemOutOfResId,
          DEM_EVENT_STATUS_FAILED
        );
#elif((SD_DEM_DETECT_SD_E_OUT_OF_RES == TS_PROD_ERR_REP_TO_DET) && (SD_DEV_ERROR_DETECT == STD_ON))
      SD_DET_REPORT_ERROR(SD_RXINDICATION_SVCID, SD_E_OUT_OF_RES_DEMTODET);
#endif
    }
#else
    Sd_ClientService_eventOfferService(
      &ServiceInfo,
      entryOptionInfoPtr,
      TTL,
      MinorVersion,
      RemoteNodeIndex,
      SdInstanceIndex,
      isMulticast);
#endif
  }
  else
  {
    if(isMulticast != FALSE)
    { /* A STOP OFFER entry can only arrive over multicast */
      Sd_ClientService_eventStopOfferService(
        &ServiceInfo,
        entryOptionInfoPtr,
        MinorVersion,
        RemoteNodeIndex,
        SdInstanceIndex);
    }
  }
}


STATIC FUNC(void, SD_CODE) Sd_Receive_handleEntrySubscribeEventgroup
(
  P2CONST(uint8, AUTOMATIC, SD_APPL_DATA) entryPtr,
  P2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) entryOptionInfoPtr,
  Sd_RemoteNodeIdType RemoteNodeIndex,
  uint8 SdInstanceIndex
)
{
  Sd_ServiceInfoType ServiceInfo;
  uint32 TTL;
  uint16 ReservedField;
  uint16 EventgroupID;

  SD_READ(SD_ENTRY_SERVICEID,entryPtr,ServiceInfo.ServiceID);
  SD_READ(SD_ENTRY_INSTANCEID,entryPtr,ServiceInfo.InstanceID);
  SD_READ(SD_ENTRY_MAJORVERSION,entryPtr,ServiceInfo.MajorVersion);
  SD_READ(SD_ENTRY_TTL,entryPtr,TTL);
  SD_READ(SD_ENTRYT2_RESERVED2,entryPtr,ReservedField);
  SD_READ(SD_ENTRYT2_EVENTGROUPID,entryPtr,EventgroupID);

  if(TTL != 0U)
  {
    Sd_ServerService_eventSubscribe(
      &ServiceInfo,
      entryOptionInfoPtr,
      TTL,
      ReservedField,
      EventgroupID,
      RemoteNodeIndex,
      SdInstanceIndex);
  }
  else
  {
    Sd_ServerService_eventStopSubscribe(
      &ServiceInfo,
      entryOptionInfoPtr,
      ReservedField,
      EventgroupID,
      RemoteNodeIndex,
      SdInstanceIndex);
  }
}



STATIC FUNC(void, SD_CODE) Sd_Receive_handleEntrySubscribeEventgroupAck
(
  P2CONST(uint8, AUTOMATIC, SD_APPL_DATA) entryPtr,
  P2VAR(Sd_EntryOptionInfoType, AUTOMATIC, SD_APPL_DATA) entryOptionInfoPtr,
  Sd_RemoteNodeIdType RemoteNodeIndex,
  uint8 SdInstanceIndex
)
{
  Sd_ServiceInfoType ServiceInfo;
  uint32 TTL;
  uint32 MinorVersion;
  uint16 EventgroupID;

  SD_READ(SD_ENTRY_SERVICEID,entryPtr,ServiceInfo.ServiceID);
  SD_READ(SD_ENTRY_INSTANCEID,entryPtr,ServiceInfo.InstanceID);
  SD_READ(SD_ENTRY_MAJORVERSION,entryPtr,ServiceInfo.MajorVersion);
  SD_READ(SD_ENTRY_TTL,entryPtr,TTL);
  SD_READ(SD_ENTRYT2_EVENTGROUPID,entryPtr,EventgroupID);
  SD_READ(SD_ENTRYT1_MINORVERSION,entryPtr,MinorVersion);

  Sd_ClientService_eventSubscribeAck(&ServiceInfo, entryOptionInfoPtr, TTL, MinorVersion, EventgroupID, RemoteNodeIndex, SdInstanceIndex);
}



#define SD_STOP_SEC_CODE
#include <Sd_MemMap.h>


/*==================[end of file]===========================================*/
