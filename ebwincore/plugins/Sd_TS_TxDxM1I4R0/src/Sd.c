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
#include <Sd_Cbk.h>           /* Module public API */
#include <SchM_Sd.h>          /* SchM symbols for critical sections */

#include <Sd_ServerService_Int.h> /* SW-Unit ServerService interface */
#include <Sd_ClientService_Int.h> /* SW-Unit ClientService interface */
#include <Sd_Send_Int.h>          /* SW-Unit Send interface */

#include <TSMem.h>
#include <TSPBConfig_Signature.h> /* relocatable post-build macros */
#if (SD_PBCFGM_SUPPORT_ENABLED == STD_ON)
#include <PbcfgM.h>     /* Post build configuration manager API */
#endif /* SD_PBCFGM_SUPPORT_ENABLED */

/*==================[version check]=========================================*/
/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef SD_VENDOR_ID /* configuration check */
#error SD_VENDOR_ID must be defined
#endif

#if (SD_VENDOR_ID != 1U) /* vendor check */
#error SD_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef SD_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error SD_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (SD_AR_RELEASE_MAJOR_VERSION != 4U)
#error SD_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef SD_AR_RELEASE_MINOR_VERSION /* configuration check */
#error SD_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (SD_AR_RELEASE_MINOR_VERSION != 2U )
#error SD_AR_RELEASE_MINOR_VERSION wrong (!= 2U)
#endif

#ifndef SD_AR_RELEASE_REVISION_VERSION /* configuration check */
#error SD_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (SD_AR_RELEASE_REVISION_VERSION != 1U )
#error SD_AR_RELEASE_REVISION_VERSION wrong (!= 1U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef SD_SW_MAJOR_VERSION /* configuration check */
#error SD_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (SD_SW_MAJOR_VERSION != 1U)
#error SD_SW_MAJOR_VERSION wrong (!= 1U)
#endif

#ifndef SD_SW_MINOR_VERSION /* configuration check */
#error SD_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (SD_SW_MINOR_VERSION < 4U)
#error SD_SW_MINOR_VERSION wrong (< 4U)
#endif

#ifndef SD_SW_PATCH_VERSION /* configuration check */
#error SD_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (SD_SW_PATCH_VERSION < 9U)
#error SD_SW_PATCH_VERSION wrong (< 9U)
#endif

/*==================[macros]================================================*/
#define SD_RANDOM_MULTIPLIER    1664525U
#define SD_RANDOM_INCREMENT  1013904223U


/*==================[type definitions]======================================*/

/*==================[external data]=========================================*/
#define SD_START_SEC_VAR_INIT_8
#include <Sd_MemMap.h>

VAR(uint8, SD_VAR) Sd_Initialized = SD_UNINITED;

#define SD_STOP_SEC_VAR_INIT_8
#include <Sd_MemMap.h>

#define SD_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Sd_MemMap.h>

P2CONST(Sd_ConfigType,SD_VAR_CLEARED, SD_APPL_CONST) SdConfigPtr;

VAR(Sd_InstanceStateType, SD_VAR_CLEARED) Sd_InstanceStates[SD_MAX_INSTANCE_COUNT];

#define SD_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Sd_MemMap.h>

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#define SD_START_SEC_CODE
#include <Sd_MemMap.h>

/** \brief resets the state information for remote nodes of a SD instance
 **
 ** \param[in] InstanceIndex Index of ServiceDiscovery instance
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
STATIC FUNC(void,SD_CODE) Sd_ResetRemoteNodeStates
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr
);

STATIC FUNC(void,SD_CODE) Sd_HandleResponseTimers
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr,
  uint8 InstanceIdx
);

#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
STATIC FUNC(void,SD_CODE) Sd_HandleRetryTimers
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr,
  uint8 InstanceIdx
);
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */

STATIC FUNC(boolean, SD_CODE) Sd_CompareAddress
(
  CONSTP2CONST(TcpIp_SockAddrInet6Type,AUTOMATIC,SD_APPL_CODE) AddrPtr1,
  CONSTP2CONST(TcpIp_SockAddrInet6Type,AUTOMATIC,SD_APPL_CODE) AddrPtr2
);

#define SD_STOP_SEC_CODE
#include <Sd_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/
#define SD_START_SEC_VAR_INIT_32
#include <Sd_MemMap.h>

STATIC VAR(uint32, SD_VAR) Sd_RandomState = 1U;

#define SD_STOP_SEC_VAR_INIT_32
#include <Sd_MemMap.h>


#define SD_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Sd_MemMap.h>

STATIC VAR(Sd_RemoteNodeStateType, SD_VAR) Sd_RemoteNodeStates[SD_MAXNUM_REMOTENODES];

#define SD_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Sd_MemMap.h>


/*==================[external function definitions]=========================*/

#define SD_START_SEC_CODE
#include <Sd_MemMap.h>


/* !LINKSTO Sd.ASR40.SWS_SD_00126,1 */
#if (SD_VERSION_INFO_API == STD_ON)
FUNC(void, SD_CODE) Sd_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, SD_APPL_DATA) Sd_VersionInfo
)
{
  DBG_SD_GETVERSIONINFO_ENTRY(Sd_VersionInfo);
#if (SD_DEV_ERROR_DETECT == STD_ON)
  if(Sd_VersionInfo == NULL_PTR)
  {
    SD_DET_REPORT_ERROR(SD_GETVERSIONINFO_SVCID, SD_E_PARAM_POINTER);
  }
  else
#endif /* SD_DEV_ERROR_DETECT == STD_ON */
  {
    /* assign logistic values to structure members */
    Sd_VersionInfo->vendorID = (uint16) SD_VENDOR_ID;
    Sd_VersionInfo->moduleID = (uint16) SD_MODULE_ID;
    Sd_VersionInfo->sw_major_version = (uint8) SD_SW_MAJOR_VERSION;
    Sd_VersionInfo->sw_minor_version = (uint8) SD_SW_MINOR_VERSION;
    Sd_VersionInfo->sw_patch_version = (uint8) SD_SW_PATCH_VERSION;
  }
  DBG_SD_GETVERSIONINFO_EXIT(Sd_VersionInfo);
}
#endif /* SD_VERSION_INFO_API == STD_ON */


FUNC(Std_ReturnType, SD_CODE) Sd_IsValidConfig
(
  P2CONST(void, AUTOMATIC, SD_APPL_CONST) voidConfigPtr
)
{
  /* Assume an invalid configuration. */
  Std_ReturnType RetVal = E_NOT_OK;
  P2CONST(Sd_ConfigType, AUTOMATIC, SD_APPL_CONST) ConfigPtr = voidConfigPtr;

  DBG_SD_ISVALIDCONFIG_ENTRY(voidConfigPtr);

  if(ConfigPtr != NULL_PTR)
  {
    /* Check if the configuration fits to the platform. */
    if(TS_PlatformSigIsValid(ConfigPtr->PlatformSignature) == TRUE)
    {
      /* Validate the post build configuration against the compile time configuration. */
      if(SD_CFG_SIGNATURE == ConfigPtr->CfgSignature)
      {
        /* Validate the post build configuration against the compile time published information configuration. */
        if(SD_PUBLIC_INFO_SIGNATURE == ConfigPtr->PublicInfoSignature)
        {
          /* Indicate that the configuration is valid. */
          RetVal = E_OK;
        }
      }
    }
  }

  DBG_SD_ISVALIDCONFIG_EXIT(RetVal, voidConfigPtr);
  return RetVal;
}


FUNC(void, SD_CODE) Sd_Init
(
  P2CONST(Sd_ConfigType, AUTOMATIC, SD_APPL_CONST) ConfigPtr
)
{
  P2CONST(Sd_ConfigType, AUTOMATIC, SD_APPL_CONST) LocalConfigPtr = ConfigPtr;

  DBG_SD_INIT_ENTRY(ConfigPtr);

#if (SD_PBCFGM_SUPPORT_ENABLED == STD_ON)
  /* If the initialization function is called with a null pointer get the configuration from the
   * post build configuration manager. */
  if(LocalConfigPtr == NULL_PTR)
  {
    PbcfgM_ModuleConfigPtrType ModuleConfig = NULL_PTR;
    if(E_OK == PbcfgM_GetConfig(
                                 SD_MODULE_ID,
                                 SD_INSTANCE_ID,
                                 &ModuleConfig)
                               )
    {
      LocalConfigPtr = (P2CONST(Sd_ConfigType, AUTOMATIC, SD_APPL_CONST)) ModuleConfig;
    }
  }
#endif /* SD_PBCFGM_SUPPORT_ENABLED == STD_OFF */

#if (SD_DEV_ERROR_DETECT == STD_ON)
  if(LocalConfigPtr == NULL_PTR)
  {
    SD_DET_REPORT_ERROR(SD_INIT_SVCID, SD_E_PARAM_POINTER);
  }
  /* Check that configuration pointer is valid. */
  else if(E_OK != Sd_IsValidConfig(LocalConfigPtr))
  {
    SD_DET_REPORT_ERROR(SD_INIT_SVCID, SD_E_INVALID_ARG);
  }
  else
#endif
  {
    /* !LINKSTO Sd.ASR40.SWS_SD_00121,1 */
    SdConfigPtr = LocalConfigPtr;
    {
      P2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr = &SD_CFG_REF(SdInstance_t, SdInstancesRef, 0U);
      P2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr = &Sd_InstanceStates[0];
      uint8 InstanceIndex;

      for (InstanceIndex = 0U; InstanceIndex < SdConfigPtr->SdInstanceCount; ++InstanceIndex)
      { /* Open TxCon*/
        /* The return value can be ignored as the only possible return value is E_OK */
        (void)SoAd_OpenSoCon(InstanceCfgPtr->SdInstanceTxSoCon);
        /* open RxCons*/
        /* The return value can be ignored as the only possible return value is E_OK */
        (void)SoAd_OpenSoCon(InstanceCfgPtr->SdInstanceRxMultiCastSoCon);
        /* The return value can be ignored as the only possible return value is E_OK */
        (void)SoAd_OpenSoCon(InstanceCfgPtr->SdInstanceRxUniCastSoCon);

        InstanceStatePtr->TxIpAddrState = TCPIP_IPADDR_STATE_UNASSIGNED;
        InstanceStatePtr->RxMultiCastIpAddrState = TCPIP_IPADDR_STATE_UNASSIGNED;
        InstanceStatePtr->TxState.Address.domain = TCPIP_AF_UNSPEC;
        InstanceStatePtr->ServersDeltaTTL = SD_TTL_OFF;
        InstanceStatePtr->ServersDeltaTTLtimer = SD_TTL_OFF;
        InstanceStatePtr->ClientsDeltaTTL = SD_TTL_OFF;
        InstanceStatePtr->ClientsDeltaTTLtimer = SD_TTL_OFF;
        InstanceStatePtr->ClientServicesEventsCount = 0U;
        InstanceStatePtr->ClientsDelta = 0U;
        InstanceStatePtr->ClientsDeltaTimer = 0U;
        InstanceStatePtr->ServersDelta = 0U;
        InstanceStatePtr->ServerDeltaTimer = 0U;
        InstanceStatePtr->ResponseTimersDelta = 0U;
        InstanceStatePtr->ResponseTimersDeltaTimer = 0U;
        InstanceStatePtr->ServerServicesTimersActive = 0U;
        InstanceStatePtr->ClientServicesTimersActive = 0U;
        InstanceStatePtr->ServerServicesEventsCount = 0U;
        InstanceStatePtr->ResponseTimersActiveCount = 0U;
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
        InstanceStatePtr->RetryTimersDelta = 0U;
        InstanceStatePtr->RetryTimersDeltaTimer = 0U;
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
        InstanceStatePtr->Halted = FALSE;

        ++InstanceCfgPtr;
        ++InstanceStatePtr;
      }
    }

    Sd_Send_init();
    /* !LINKSTO Sd.ASR40.SWS_SD_00120,1 */
    Sd_ServerService_init();
    Sd_ClientService_init();

    {
      Sd_RemoteNodeIdType RemoteNodeIndex;

      for(RemoteNodeIndex = 0U; RemoteNodeIndex < SD_MAXNUM_REMOTENODES; ++RemoteNodeIndex)
      {
        /* Init remote nodes */
        CONSTP2VAR(Sd_RemoteNodeStateType,AUTOMATIC,SD_APPL_DATA) RemoteNodeStatesPtr = &Sd_RemoteNodeStates[RemoteNodeIndex];

        RemoteNodeStatesPtr->RemoteUnicastRebootState.SessionId = 0U;
        RemoteNodeStatesPtr->RemoteUnicastRebootState.RebootFlag = TRUE;

        RemoteNodeStatesPtr->RemoteMulticastRebootState.SessionId = 0U;
        RemoteNodeStatesPtr->RemoteMulticastRebootState.RebootFlag = TRUE;

        RemoteNodeStatesPtr->ResponseTimer = 0U;
        RemoteNodeStatesPtr->IsConnectionReadyTimer = 0U;
        RemoteNodeStatesPtr->ServerServicesFindsReceived = 0U;
        RemoteNodeStatesPtr->ClientServicesOffersReceived = 0U;
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
        RemoteNodeStatesPtr->ClientServicesRetriesToProcess = 0U;
        RemoteNodeStatesPtr->RetryTimer = 0U;
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
        RemoteNodeStatesPtr->IsConnectionReady = FALSE;
      }
    }

    Sd_Initialized = SD_INITED;
  }
  DBG_SD_INIT_EXIT(ConfigPtr);
}



FUNC(void, SD_CODE) Sd_LocalIpAddrAssignmentChg
(
  SoAd_SoConIdType SoConId,
  TcpIp_IpAddrStateType State
)
{
  DBG_SD_LOCALIPADDRASSIGNMENTCHG_ENTRY(SoConId,State);

#if (SD_DEV_ERROR_DETECT == STD_ON)
  if (Sd_Initialized == SD_UNINITED)
  {
    SD_DET_REPORT_ERROR(SD_LOCALIPADDRASSIGNMENTCHG_SVCID,SD_E_NOT_INITIALIZED);
  }
  else if((State != TCPIP_IPADDR_STATE_ASSIGNED) &&
          (State != TCPIP_IPADDR_STATE_ONHOLD) &&
          (State != TCPIP_IPADDR_STATE_UNASSIGNED))
  {
    SD_DET_REPORT_ERROR(SD_LOCALIPADDRASSIGNMENTCHG_SVCID,SD_E_INV_MODE);
  }
  else
#endif
  {
    P2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr = &SD_CFG_REF(SdInstance_t, SdInstancesRef, 0U);
    P2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr = &Sd_InstanceStates[0];
    uint8 InstanceIndex;
    const uint8 InstanceIndexEnd = SdConfigPtr->SdInstanceCount;
    boolean SoConIdFound = FALSE;

    for(InstanceIndex = 0U; InstanceIndex < InstanceIndexEnd; ++InstanceIndex)
    {
      uint8 NetMask;

      if(InstanceCfgPtr->SdInstanceRxMultiCastSoCon == SoConId)
      {
        SoConIdFound = TRUE;

        if(InstanceStatePtr->RxMultiCastIpAddrState != State)
        {
          /* !LINKSTO Sd.ASR40.SWS_SD_00697,1 */
          if (State == TCPIP_IPADDR_STATE_ASSIGNED)
          {
              TcpIp_SockAddrType LocalAddress, LocalGateway;
              /* set domain to unspecified to indicate GetLocalAddr function in SoAd module that
               * the domain is unknown, so that it can be set */
              LocalAddress.domain = TCPIP_AF_UNSPEC;

              /* The return value can be ignored as the only possible return value is E_OK */
              (void)SoAd_GetLocalAddr(InstanceCfgPtr->SdInstanceRxMultiCastSoCon,
                                      &LocalAddress,
                                      &NetMask,
                                      &LocalGateway);

              /* the domain has been set whithin the GetLocalAddr function, it can know be set
               * into the instance state variable */
              InstanceStatePtr->TxState.Address.domain = LocalAddress.domain;

            /* depending on the domain the Socket Address structure to be used is different, that is why
             * a different local structure with the correct type (INET6 or INET) is passed to the SoAd
             * GetLocalAddress to avoid side effects on casting one structure to the other  */
            if (InstanceStatePtr->TxState.Address.domain == TCPIP_AF_INET6)
            {
              TcpIp_SockAddrInet6Type LocalSocket6, LocalGateway6;

              LocalSocket6.domain = TCPIP_AF_INET6;
              LocalGateway6.domain = TCPIP_AF_INET6;

              /* The return value can be ignored as the only possible return value is E_OK */
              (void)SoAd_GetLocalAddr(InstanceCfgPtr->SdInstanceRxMultiCastSoCon,
                                      /* Deviation MISRAC2012-1 */
                                      (TcpIp_SockAddrType*)&LocalSocket6,
                                      &NetMask,
                                      /* Deviation MISRAC2012-1 */
                                      (TcpIp_SockAddrType*)&LocalGateway6);

              InstanceStatePtr->TxState.Address.addr[0] = LocalSocket6.addr[0];
              InstanceStatePtr->TxState.Address.addr[1] = LocalSocket6.addr[1];
              InstanceStatePtr->TxState.Address.addr[2] = LocalSocket6.addr[2];
              InstanceStatePtr->TxState.Address.addr[3] = LocalSocket6.addr[3];
              InstanceStatePtr->TxState.Address.port = LocalSocket6.port;
            }
            else
            {
              TcpIp_SockAddrInetType LocalSocket, LocalSockGateway;

              LocalSocket.domain = TCPIP_AF_INET;
              LocalSockGateway.domain = TCPIP_AF_INET;

              /* The return value can be ignored as the only possible return value is E_OK */
              (void)SoAd_GetLocalAddr(InstanceCfgPtr->SdInstanceRxMultiCastSoCon,
                                      /* Deviation MISRAC2012-1 */
                                      (TcpIp_SockAddrType*)&LocalSocket,
                                      &NetMask,
                                      /* Deviation MISRAC2012-1 */
                                      (TcpIp_SockAddrType*)&LocalSockGateway);

              InstanceStatePtr->TxState.Address.addr[0] = LocalSocket.addr[0];
              InstanceStatePtr->TxState.Address.port = LocalSocket.port;
            }

            SchM_Enter_Sd_SCHM_SD_EXCLUSIVE_AREA_0();
            InstanceStatePtr->RxMultiCastIpAddrState = TCPIP_IPADDR_STATE_ASSIGNED;

            if(InstanceStatePtr->TxIpAddrState == TCPIP_IPADDR_STATE_ASSIGNED)
            {
              Sd_ResetRemoteNodeStates(InstanceCfgPtr, InstanceStatePtr);
              Sd_ServerService_startServices(InstanceCfgPtr, InstanceStatePtr);
              Sd_ClientService_startServices(InstanceCfgPtr, InstanceStatePtr);
            }

            SchM_Exit_Sd_SCHM_SD_EXCLUSIVE_AREA_0();
          }
          else
          {
            SchM_Enter_Sd_SCHM_SD_EXCLUSIVE_AREA_0();
            InstanceStatePtr->RxMultiCastIpAddrState = State;

            Sd_ServerService_haltServices(InstanceCfgPtr, InstanceStatePtr);
            Sd_ClientService_haltServices(InstanceCfgPtr, InstanceStatePtr);

            Sd_ClientServices_init_after_halt(InstanceCfgPtr);
            Sd_ServerServices_init_after_halt(InstanceCfgPtr);

            InstanceStatePtr->ServerServicesTimersActive = 0U;
            InstanceStatePtr->ClientServicesTimersActive = 0U;
            InstanceStatePtr->ServerDeltaTimer = 0U;
            InstanceStatePtr->ClientsDeltaTimer = 0U;
            InstanceStatePtr->ResponseTimersDeltaTimer = 0U;
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
            InstanceStatePtr->RetryTimersDeltaTimer = 0U;
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
            InstanceStatePtr->ResponseTimersActiveCount = 0U;
            InstanceStatePtr->Halted = TRUE;
            SchM_Exit_Sd_SCHM_SD_EXCLUSIVE_AREA_0();
          }
        }
      }
      else
      {
        if(InstanceCfgPtr->SdInstanceTxSoCon == SoConId)
        {
          SoConIdFound = TRUE;

          if(InstanceStatePtr->TxIpAddrState != State)
          {
            SchM_Enter_Sd_SCHM_SD_EXCLUSIVE_AREA_0();
            InstanceStatePtr->TxIpAddrState = State;
            if(State == TCPIP_IPADDR_STATE_ASSIGNED)
            {
              if(InstanceStatePtr->RxMultiCastIpAddrState == TCPIP_IPADDR_STATE_ASSIGNED)
              {
                Sd_ResetRemoteNodeStates(InstanceCfgPtr, InstanceStatePtr);
                Sd_ServerService_startServices(InstanceCfgPtr, InstanceStatePtr);
                Sd_ClientService_startServices(InstanceCfgPtr, InstanceStatePtr);
              }
            }
            else
            {
              Sd_ServerService_haltServices(InstanceCfgPtr, InstanceStatePtr);
              Sd_ClientService_haltServices(InstanceCfgPtr, InstanceStatePtr);

              Sd_ClientServices_init_after_halt(InstanceCfgPtr);
              Sd_ServerServices_init_after_halt(InstanceCfgPtr);

              InstanceStatePtr->ServerServicesTimersActive = 0U;
              InstanceStatePtr->ClientServicesTimersActive = 0U;
              InstanceStatePtr->ServerDeltaTimer = 0U;
              InstanceStatePtr->ClientsDeltaTimer = 0U;
              InstanceStatePtr->ResponseTimersDeltaTimer = 0U;
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
              InstanceStatePtr->RetryTimersDeltaTimer = 0U;
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
              InstanceStatePtr->ResponseTimersActiveCount = 0U;
              InstanceStatePtr->Halted = TRUE;
            }
            SchM_Exit_Sd_SCHM_SD_EXCLUSIVE_AREA_0();
          }
        }
      }
      if(SoConIdFound)
      {
        break;
      }
      ++InstanceCfgPtr;
      ++InstanceStatePtr;
    }

#if (SD_DEV_ERROR_DETECT == STD_ON)
    if(!SoConIdFound)
    {
      SD_DET_REPORT_ERROR(SD_LOCALIPADDRASSIGNMENTCHG_SVCID,SD_E_INV_ID);
    }
#endif
  }
  DBG_SD_LOCALIPADDRASSIGNMENTCHG_EXIT(SoConId,State);
}

/* !LINKSTO Sd.Design.Sd_MainFunction,1 */
FUNC(void, SD_CODE) Sd_MainFunction(void)
{
  DBG_SD_MAINFUNCTION_ENTRY();

  /* Main functions shall not call DET if called before initialization. */
  if (Sd_Initialized == SD_INITED)
  { /* SD has been initialized, go through all configured instances... */
    P2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr = &SD_CFG_REF(SdInstance_t, SdInstancesRef, 0U);
    P2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr = &Sd_InstanceStates[0U];
    uint16_least InstanceIndex;
    const uint16_least InstanceIndexEnd = (uint16_least)SdConfigPtr->SdInstanceCount;

    for(InstanceIndex = 0U; InstanceIndex < InstanceIndexEnd; ++InstanceIndex)
    {
      /* cyclic main function pro instance */
      Sd_Instance_MainFunction((CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA))InstanceCfgPtr, (CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA))InstanceStatePtr, (uint8)InstanceIndex);

      /* defensive action, if this instance has been halted, once processed and all its services (clients and servers)
        have been halted, the general flags are initialized here */
      if(InstanceStatePtr->Halted != FALSE)
      {
        InstanceStatePtr->ServersDeltaTTL = SD_TTL_OFF;
        InstanceStatePtr->ServersDeltaTTLtimer = SD_TTL_OFF;
        InstanceStatePtr->ClientsDeltaTTL = SD_TTL_OFF;
        InstanceStatePtr->ClientsDeltaTTLtimer = SD_TTL_OFF;

        SchM_Enter_Sd_SCHM_SD_EXCLUSIVE_AREA_0();
        InstanceStatePtr->ServerServicesEventsCount = 0U;
        InstanceStatePtr->ClientServicesEventsCount = 0U;
        SchM_Exit_Sd_SCHM_SD_EXCLUSIVE_AREA_0();

        InstanceStatePtr->ServersDelta = 0U;
        InstanceStatePtr->ClientsDelta = 0U;
        InstanceStatePtr->Halted = FALSE;
      }
      /* increase pointer to the next instance */
      ++InstanceCfgPtr;
      ++InstanceStatePtr;
    }
  }
  DBG_SD_MAINFUNCTION_EXIT();
}



/* SW-Unit API functions */

TS_MOD_PRIV_DEFN FUNC(boolean, SD_CODE) Sd_IsInstanceAddressAssigned(const uint8 instanceIdx)
{
  boolean result;
  if((Sd_InstanceStates[instanceIdx].RxMultiCastIpAddrState == TCPIP_IPADDR_STATE_ASSIGNED) &&
     (Sd_InstanceStates[instanceIdx].TxIpAddrState == TCPIP_IPADDR_STATE_ASSIGNED) )
  {
    result = TRUE;
  }
  else
  {
    result = FALSE;
  }
  return result;
}


FUNC(void, SD_CODE) Sd_StartRandom
(
  uint32 Seed
)
{
  DBG_SD_STARTRANDOM_ENTRY(Seed);
  Sd_RandomState = Seed;
  DBG_SD_STARTRANDOM_EXIT(Seed);
}


/* SW-Unit API functions */

TS_MOD_PRIV_DEFN FUNC(uint32, SD_CODE) Sd_GetRandom(const uint32 MinLimit, const uint32 MaxLimit)
{
  uint32 Result = MinLimit;
  /* increment maximum value to make sure the whole range MinLimit - MaxLimit is covered */
  const uint32 MaxValue = MaxLimit + 1U;

#if (SD_DEV_ERROR_DETECT == STD_ON)
  /* CHECK: NOPARSE */
  /* this defensive check covers these two impossible cases that should never happen:       *
     - Value of MaxLimit is samller than value of MinLimit                                  *
     - Value of MaxLimit overflows after +1 operation is executed                           */
  if (MaxValue <= MinLimit)
  { /* should not happen */
    SD_DET_REPORT_ERROR(SD_GETRANDOM_SVCID, SD_E_DIVISOR_IS_NEGATIVE);
  }
  /* CHECK: PARSE */
  else
#endif
  {
    Sd_RandomState = (Sd_RandomState * SD_RANDOM_MULTIPLIER) + SD_RANDOM_INCREMENT;

  /* modular operation yields a value between 0 and ( (MaxLimit+1) - MinLimit - 1 ),              *
   * Result range goes from  (MinLimit + 0)                             = MinLimit           *
   *                   to    (MinLimit + (MaxLimit + 1) - MinLimit - 1) = MaxLimit           */
    Result += ( Sd_RandomState % (MaxValue - MinLimit) );
  }

  return Result;
}



TS_MOD_PRIV_DEFN FUNC(Sd_RemoteNodeIdType, SD_CODE) Sd_AssignRemoteNodeState
(
  CONSTP2CONST(TcpIp_SockAddrInet6Type,AUTOMATIC,SD_APPL_CODE) RemoteAddrPtr,
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr
)
{
  boolean RemoteNodeFound = FALSE;
  Sd_RemoteNodeIdType RemoteNodeIndex = InstanceCfgPtr->RemoteNode;
  const Sd_RemoteNodeIdType RemoteNodeIndexEnd = RemoteNodeIndex + InstanceCfgPtr->RemoteNodeCount;
  P2VAR(Sd_RemoteNodeStateType,AUTOMATIC,SD_APPL_DATA) RemoteNodeStatesPtr = &Sd_RemoteNodeStates[RemoteNodeIndex];

  while(RemoteNodeIndex < RemoteNodeIndexEnd)
  {
    if(RemoteNodeStatesPtr->TxState.Address.domain != TCPIP_AF_UNSPEC)
    {
      if(Sd_CompareAddress(&RemoteNodeStatesPtr->TxState.Address, RemoteAddrPtr) != FALSE)
      {
        RemoteNodeFound = TRUE;
      }
    }
    else
    {
      RemoteNodeStatesPtr->TxState.Address = *RemoteAddrPtr;

      RemoteNodeStatesPtr->TxState.RebootState.SessionId = 1U;
      RemoteNodeStatesPtr->TxState.RebootState.RebootFlag = TRUE;

      RemoteNodeStatesPtr->RemoteUnicastRebootState.SessionId = 0U;
      RemoteNodeStatesPtr->RemoteUnicastRebootState.RebootFlag = TRUE;

      RemoteNodeStatesPtr->RemoteMulticastRebootState.SessionId = 0U;
      RemoteNodeStatesPtr->RemoteMulticastRebootState.RebootFlag = TRUE;
      RemoteNodeStatesPtr->ServerServicesFindsReceived = 0U;
      RemoteNodeStatesPtr->ClientServicesOffersReceived = 0U;
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
      RemoteNodeStatesPtr->ClientServicesRetriesToProcess = 0U;
      RemoteNodeStatesPtr->RetryTimer = 0U;
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
      RemoteNodeStatesPtr->ResponseTimer = 0U;
      RemoteNodeStatesPtr->IsConnectionReadyTimer = 0U;
      RemoteNodeStatesPtr->IsConnectionReady = FALSE;

      RemoteNodeFound = TRUE;
    }

    if(RemoteNodeFound != FALSE)
    {
      break;
    }
    ++RemoteNodeIndex;
    ++RemoteNodeStatesPtr;
  }

  if(RemoteNodeFound == FALSE)
  {
    RemoteNodeIndex = SD_INVALID_REMOTENODE_ID;
  }
  return RemoteNodeIndex;
}



TS_MOD_PRIV_DEFN FUNC_P2VAR(Sd_RemoteNodeStateType, AUTOMATIC, SD_CODE) Sd_GetRemoteNodeState(const Sd_RemoteNodeIdType RemoteNodeIndex)
{
  return &Sd_RemoteNodeStates[RemoteNodeIndex];
}



TS_MOD_PRIV_DEFN FUNC(TcpIp_DomainType, SD_CODE) Sd_GetInstanceAddressDomain(const uint8 InstanceIndex)
{
  return Sd_InstanceStates[InstanceIndex].TxState.Address.domain;
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_ResetRemoteNodeRxState
(
  CONSTP2VAR(Sd_InstanceStateType, AUTOMATIC, SD_APPL_DATA) InstanceStatesPtr,
  const Sd_RemoteNodeIdType RemoteNodeIndex
)
{
  CONSTP2VAR(Sd_RemoteNodeStateType,AUTOMATIC,SD_APPL_DATA) RemoteNodeStatesPtr = &Sd_RemoteNodeStates[RemoteNodeIndex];


  RemoteNodeStatesPtr->RemoteUnicastRebootState.SessionId = 0U;
  RemoteNodeStatesPtr->RemoteUnicastRebootState.RebootFlag = TRUE;

  RemoteNodeStatesPtr->RemoteMulticastRebootState.SessionId = 0U;
  RemoteNodeStatesPtr->RemoteMulticastRebootState.RebootFlag = TRUE;

  RemoteNodeStatesPtr->ServerServicesFindsReceived = 0U;
  RemoteNodeStatesPtr->IsConnectionReady = FALSE;
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
  RemoteNodeStatesPtr->ClientServicesRetriesToProcess = 0U;
  RemoteNodeStatesPtr->RetryTimer = 0U;
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */

  if(RemoteNodeStatesPtr->ResponseTimer != 0U)
  {
    RemoteNodeStatesPtr->ResponseTimer = 0U;
    RemoteNodeStatesPtr->IsConnectionReadyTimer = 0U;
#if (SD_DEV_ERROR_DETECT == STD_ON)
    /* CHECK: NOPARSE */
    /* this defensive check covers the case of an underflow in the variable ResponseTimersActiveCount */
    SD_PRECONDITION_ASSERT( (InstanceStatesPtr->ResponseTimersActiveCount > 0U), SD_RESETREMOTENODERXSTATE_SVCID );
    /* CHECK: PARSE */
#endif
    --InstanceStatesPtr->ResponseTimersActiveCount;
  }
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_RestartNodeResponseTimer
(
  const Sd_TimerType timerValue,
  const Sd_RemoteNodeIdType nodeIndex,
  const uint8 SdInstanceIndex
)
{
  CONSTP2VAR(Sd_RemoteNodeStateType,AUTOMATIC,SD_VAR) RemoteNodeStatePtr = &Sd_RemoteNodeStates[nodeIndex];
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr = &Sd_InstanceStates[SdInstanceIndex];

  const boolean NewResponseTimer = (RemoteNodeStatePtr->ResponseTimer == 0U);
  const boolean oldTimerinRange = (timerValue < RemoteNodeStatePtr->ResponseTimer);

  if(NewResponseTimer != FALSE)
  {
    /* This nodes response timer was not previously active, increase the counter that keeps the total number of response timer active within this instance */
    ++InstanceStatePtr->ResponseTimersActiveCount;
  }

  if(   (  (NewResponseTimer != FALSE) && ( (InstanceStatePtr->ResponseTimersDeltaTimer == 0U) || (timerValue < InstanceStatePtr->ResponseTimersDeltaTimer) )  )
     || ( (oldTimerinRange != FALSE) && (timerValue < InstanceStatePtr->ResponseTimersDeltaTimer) )   )
  {
    RemoteNodeStatePtr->ResponseTimer = timerValue;
    InstanceStatePtr->ResponseTimersDeltaTimer = timerValue;
    InstanceStatePtr->ResponseTimersDelta = timerValue;
  }
  else
  {
    if( (NewResponseTimer != FALSE) || (oldTimerinRange != FALSE) )
    {
      RemoteNodeStatePtr->ResponseTimer = timerValue + (InstanceStatePtr->ResponseTimersDelta - InstanceStatePtr->ResponseTimersDeltaTimer);
    }
  }
}



TS_MOD_PRIV_DEFN FUNC(boolean, SD_CODE) Sd_CapabilityRecordStrCmp
(
  CONSTP2CONST(SdConfigOptionPtrType, AUTOMATIC, SD_APPL_DATA) receivedConfigStringArray,
  CONSTP2CONST(uint8, AUTOMATIC, SD_APPL_CONST) configuredConfigString,
  const uint16 configuredConfigStringLength
)
{
  uint16 Index;
  boolean ReturnValue = FALSE;

  if((receivedConfigStringArray[0] == NULL_PTR) &&
     (configuredConfigStringLength == 0U))
  {
    /* No config string received and no config string configured */
    /* Match */
    ReturnValue = TRUE;
  }
  else if((receivedConfigStringArray[0] != NULL_PTR) &&
          (configuredConfigStringLength > 0U) &&
          (receivedConfigStringArray[1] == NULL_PTR)
         )
  {
    /* One config string received and config string configured */
    /* Compare config strings */
    CONSTP2CONST(uint8, AUTOMATIC, SD_APPL_DATA) receivedConfigString = receivedConfigStringArray[0];
    const uint16 StringLengthToCompare = configuredConfigStringLength - 1U;
    for(Index = 0U;
        Index < StringLengthToCompare;
        ++Index)
    {
      if(receivedConfigString[Index] != configuredConfigString[Index])
      {
        break;
      }
    }

    if(Index == StringLengthToCompare)
    {
      ReturnValue = TRUE;
    }
  }
  else
  {
    /* No match */
  }

  return ReturnValue;
}



TS_MOD_PRIV_DEFN FUNC(void, SD_CODE) Sd_Instance_MainFunction
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr,
  uint8 InstanceIndex
)
{
  boolean FlushSendBuffer = FALSE;

  /* -----------------  SERVER SERVICES ------------------------------ */
  if(InstanceStatePtr->ServerServicesEventsCount != 0U)
  {
    uint16 InstanceServerServicesEventsCount;

    /* Setup buffer for multicast transmission for this Sd instance */
    Sd_Send_setupBuffer(&InstanceStatePtr->TxState, InstanceIndex);
    FlushSendBuffer = TRUE;

    SchM_Enter_Sd_SCHM_SD_EXCLUSIVE_AREA_0();
    InstanceServerServicesEventsCount = InstanceStatePtr->ServerServicesEventsCount;
    InstanceStatePtr->ServerServicesEventsCount = 0U;
    SchM_Exit_Sd_SCHM_SD_EXCLUSIVE_AREA_0();

    Sd_ServerService_mainFunction(InstanceCfgPtr, InstanceStatePtr, InstanceServerServicesEventsCount);
  }

  /* if there is any running timer in this instance */
  if(InstanceStatePtr->ServerDeltaTimer != 0U)
  {
    if(FlushSendBuffer == FALSE)
    { /* Setup buffer for multicast transmission for this Sd instance */
      Sd_Send_setupBuffer(&InstanceStatePtr->TxState, InstanceIndex);
      FlushSendBuffer = TRUE;
    }

    --InstanceStatePtr->ServerDeltaTimer;
    if(InstanceStatePtr->ServerDeltaTimer == 0U)
    {
      Sd_ServerService_mainTimersFunction(InstanceCfgPtr, InstanceStatePtr);

      InstanceStatePtr->ServersDelta = InstanceStatePtr->ServerDeltaTimer;
    }
  }
  /* if there is any TTL running in this instance */
  if(InstanceStatePtr->ServersDeltaTTLtimer > SD_TTL_ALWAYS_ON)
  {
    --InstanceStatePtr->ServersDeltaTTLtimer;
    if(InstanceStatePtr->ServersDeltaTTLtimer == SD_TTL_TIMED_OUT)
    {
      Sd_ServerService_handleTTL(InstanceCfgPtr, InstanceStatePtr);

      InstanceStatePtr->ServersDeltaTTL = InstanceStatePtr->ServersDeltaTTLtimer;
    }
  }

  /* ------------------------  CLIENT SERVICES -------------------------------- */
  if(InstanceStatePtr->ClientServicesEventsCount != 0U)
  {
    uint16 InstanceClientServicesOffersSubsAcksCount = (uint16)(InstanceStatePtr->ClientServicesEventsCount >> 16);
    uint16 InstanceClientServicesEventsCount;

    SchM_Enter_Sd_SCHM_SD_EXCLUSIVE_AREA_0();
    InstanceClientServicesEventsCount = (uint16)InstanceStatePtr->ClientServicesEventsCount;
    InstanceStatePtr->ClientServicesEventsCount = 0U;
    SchM_Exit_Sd_SCHM_SD_EXCLUSIVE_AREA_0();

    Sd_ClientService_mainFunction(InstanceCfgPtr, InstanceStatePtr, InstanceClientServicesEventsCount, InstanceClientServicesOffersSubsAcksCount);
  }
  /* else, nothing to do here, neither BswM events nor SD events have been recorded since last main cycle */

  /* if there is any running timer in this instance */
  if(InstanceStatePtr->ClientsDeltaTimer != 0U)
  {
    if(FlushSendBuffer == FALSE)
    { /* Setup buffer for multicast transmission for this Sd instance */
      Sd_Send_setupBuffer(&InstanceStatePtr->TxState, InstanceIndex);
      FlushSendBuffer = TRUE;
    }

    --InstanceStatePtr->ClientsDeltaTimer;
    if(InstanceStatePtr->ClientsDeltaTimer == 0U)
    {
      Sd_ClientService_mainTimersFunction(InstanceCfgPtr, InstanceStatePtr);
      InstanceStatePtr->ClientsDelta = InstanceStatePtr->ClientsDeltaTimer;
    }
  }

  /* if there is any TTL running in this instance */
  if(InstanceStatePtr->ClientsDeltaTTLtimer > SD_TTL_ALWAYS_ON)
  {
    --InstanceStatePtr->ClientsDeltaTTLtimer;
    if(InstanceStatePtr->ClientsDeltaTTLtimer == SD_TTL_TIMED_OUT)
    {
      Sd_ClientService_mainTTL(InstanceCfgPtr, InstanceStatePtr);

      InstanceStatePtr->ClientsDeltaTTL = InstanceStatePtr->ClientsDeltaTTLtimer;
    }
  }

  /* -------------------------  TRANSMISSION BUFFER IS CHECKED ------------------------------ */
  if(FlushSendBuffer != FALSE)
  {
    Sd_Send_flushBuffer();
  }

  /* --------------------------- HANDLE RESPONSE AND RETRY TIMERS ------------------------------------- */
   Sd_HandleResponseTimers(InstanceCfgPtr, InstanceStatePtr, InstanceIndex);
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
   Sd_HandleRetryTimers(InstanceCfgPtr, InstanceStatePtr, InstanceIndex);
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
}



/*==================[internal function definitions]=========================*/
STATIC FUNC(void,SD_CODE) Sd_HandleResponseTimers
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr,
  uint8 InstanceIdx
)
{
  /* if there is any active response timer in this instance */
  if(InstanceStatePtr->ResponseTimersDeltaTimer != 0U)
  {
    --InstanceStatePtr->ResponseTimersDeltaTimer;
    if(InstanceStatePtr->ResponseTimersDeltaTimer == 0U)
    {
      uint16_least RespTimerActCntrProcessed = (uint16_least)InstanceStatePtr->ResponseTimersActiveCount;
      Sd_RemoteNodeIdType RemoteNodeIndex = InstanceCfgPtr->RemoteNode;
      const Sd_RemoteNodeIdType RemoteNodeIndexEnd = RemoteNodeIndex + InstanceCfgPtr->RemoteNodeCount;
      /* if the number of active response timers is different to 0, this means that there is at least one remote node configured for this instance,
       that's why a do-while loop here doesn't carries the risk of "RemoteNodeStatePtr" being a NULL pointer */
      P2VAR(Sd_RemoteNodeStateType,AUTOMATIC,SD_VAR) RemoteNodeStatePtr = &Sd_RemoteNodeStates[RemoteNodeIndex];

      do
      { /* check if this remote node response timer is active */
        if(RemoteNodeStatePtr->ResponseTimer > 0U)
        {
#if (SD_DEV_ERROR_DETECT == STD_ON)
          /* CHECK: NOPARSE */
          /* this defensive check covers the case of an underflow in the variable TimeOutDelta */
          SD_PRECONDITION_ASSERT( (RemoteNodeStatePtr->ResponseTimer >= InstanceStatePtr->ResponseTimersDelta), SD_INSTANCEMAINFUNCTION_SVCID );
          /* CHECK: PARSE */
#endif
          /* active remote node, decrement response timer and also the number of remode nodes timers to be processed */
          RemoteNodeStatePtr->ResponseTimer -= InstanceStatePtr->ResponseTimersDelta;
          /* once response timer has been decremented check if its time is out */
          if(RemoteNodeStatePtr->ResponseTimer == 0U)
          { /* Setup buffer for unicast transmission to remote node */
            /* if connection to this remote node is not ready yet */
            if(RemoteNodeStatePtr->IsConnectionReady == FALSE)
            { /* check current connection state against this remote node */
              TcpIp_ReturnType ConnectionState =
                /* Deviation MISRAC2012-1 */
                SoAd_IsConnectionReady(InstanceCfgPtr->SdInstanceTxSoCon, (P2VAR(TcpIp_SockAddrType, TYPEDEF, SD_APPL_DATA))&RemoteNodeStatePtr->TxState.Address);

              if(ConnectionState == TCPIP_OK)
              { /* !LINKSTO Sd.EB.ConnectionReady.Flag,1 */
                RemoteNodeStatePtr->IsConnectionReady = TRUE;
                RemoteNodeStatePtr->IsConnectionReadyTimer = 0U;
              }
              else if(ConnectionState == TCPIP_E_PENDING)
              { /* !LINKSTO Sd.EB.ConnectionReady.Pending,1 */
                /* connection is pending, it means that its state is not ready, yet, the unicast response won't be send in this cycle.
                  it will be tried in the next cycle after connections state is checked again */
                RemoteNodeStatePtr->ResponseTimer = 1U;
                InstanceStatePtr->ResponseTimersDeltaTimer = 1U;
#if (SD_DEV_ERROR_DETECT == STD_ON)
                /* this defensive check covers the case of an underflow in the variable IsConnectionReadyTimer */
                SD_PRECONDITION_ASSERT( (RemoteNodeStatePtr->IsConnectionReadyTimer > 0U), SD_HANDLERESPONSETIMERS_SVCID );
#endif
                /* security timer with the maximum time allowed for a connection to be ready is decremented */
                --RemoteNodeStatePtr->IsConnectionReadyTimer;
                if(RemoteNodeStatePtr->IsConnectionReadyTimer == 0U)
                { /* time to set up a connection is finished, there is a problem outside of SD module, messages are sent out anyway for security
                   * and a Det Error log is generated to signal this exceptional error event */
                  /* !LINKSTO Sd.EB.ConnectionReady.Timer,1 */
                  RemoteNodeStatePtr->IsConnectionReady = TRUE;
                  /* this is a configuration error, no transmission at all to this remote node is allowed */
#if (SD_DEV_ERROR_DETECT == STD_ON)
                  SD_DET_REPORT_ERROR(SD_HANDLERESPONSETIMERS_SVCID, SD_CONNECTION_SETTING_FAILED);
#endif
                }
              }
              else
              { /* !LINKSTO Sd.EB.ConnectionReady.NOT_OK,1 */
                /* this is a configuration error, no transmission at all to this remote node is allowed */
#if (SD_DEV_ERROR_DETECT == STD_ON)
                SD_DET_REPORT_ERROR(SD_HANDLERESPONSETIMERS_SVCID, SD_CONTROL_SOCKET_IMPOSSIBLE);
#endif
              }
            }
            /* if connection state to this remote node is ready, unicast response(s) are sent normally */
            if(RemoteNodeStatePtr->IsConnectionReady != FALSE)
            { /* !LINKSTO Sd.EB.ConnectionReady.OK,1 */
              Sd_Send_setupBuffer(&RemoteNodeStatePtr->TxState, InstanceIdx);

              if(RemoteNodeStatePtr->ServerServicesFindsReceived != 0U)
              {
                Sd_ServerService_sendEntriesUnicast(InstanceCfgPtr, RemoteNodeIndex, RemoteNodeStatePtr->ServerServicesFindsReceived);
                RemoteNodeStatePtr->ServerServicesFindsReceived = 0U;
              }
              if(RemoteNodeStatePtr->ClientServicesOffersReceived > 0U)
              {
#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
                /* if counter indicates how many client services pro remote node have just been sent and thus
                  have not been acknowledged yet is bigger than zero */
                const uint16 ClientServicesRetriesToProcessPrev = RemoteNodeStatePtr->ClientServicesRetriesToProcess;
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */

                Sd_ClientService_sendEntriesUnicast(InstanceCfgPtr, RemoteNodeStatePtr, RemoteNodeIndex);

#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
                /* if counter indicates how many client services pro remote node have just been sent and thus
                  have not been acknowledged yet is bigger than zero */
                if(RemoteNodeStatePtr->ClientServicesRetriesToProcess > ClientServicesRetriesToProcessPrev)
                { /* this remote node is going to send retries of subscriptions, so its retry timer is initialized to the configured retry timer for this instance */
                  RemoteNodeStatePtr->RetryTimer = (Sd_TimerType)InstanceCfgPtr->SdSubscribeEventgroupRetryDelay + 1U;

                  if(InstanceStatePtr->RetryTimersDeltaTimer == 0U)
                  { /* if retry timer delta of this instance has not been initialized yet, then it is done here */
                    InstanceStatePtr->RetryTimersDeltaTimer = RemoteNodeStatePtr->RetryTimer;
                    /* absolute retry delta timer of this instance contains the next (shorter) remote node retry timer */
                    InstanceStatePtr->RetryTimersDelta = InstanceStatePtr->RetryTimersDeltaTimer;
                  }
                  else
                  { /* if there is already a retry timer running on this instance then the retry timer of this remote node has to be synchronized to it */
                    RemoteNodeStatePtr->RetryTimer += ((InstanceCfgPtr->SdSubscribeEventgroupRetryDelay - InstanceStatePtr->RetryTimersDeltaTimer) + 1U);
                  }
                }
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */
              }
              Sd_Send_flushBuffer();
              /* a response timeout has been processed, so counter with the number of active response timers in this instance can be decremented */
              --InstanceStatePtr->ResponseTimersActiveCount;
            }
          }
          else
          {
            if( (InstanceStatePtr->ResponseTimersDeltaTimer == 0U) || (RemoteNodeStatePtr->ResponseTimer < InstanceStatePtr->ResponseTimersDeltaTimer) )
            {
              InstanceStatePtr->ResponseTimersDeltaTimer = RemoteNodeStatePtr->ResponseTimer;
            }
          }
          --RespTimerActCntrProcessed;
        } /* else timer value equal to 0 do nothing, timer for this remote node is not running */
        ++RemoteNodeIndex;
        ++RemoteNodeStatePtr;
      } /* loop continues until all active response timers in this instance have been processed */
      while( (RespTimerActCntrProcessed > 0U) && (RemoteNodeIndex < RemoteNodeIndexEnd) );

      InstanceStatePtr->ResponseTimersDelta = InstanceStatePtr->ResponseTimersDeltaTimer;
    }
  }
}


#if (SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON)
STATIC FUNC(void,SD_CODE) Sd_HandleRetryTimers
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr,
  uint8 InstanceIdx
)
{
  /* if there is any retry timer active */
  if(InstanceStatePtr->RetryTimersDeltaTimer != 0U)
  {
    --InstanceStatePtr->RetryTimersDeltaTimer;
    /* check if the next shorter remote node retry timer has been reached */
    if(InstanceStatePtr->RetryTimersDeltaTimer == 0U)
    {
      Sd_RemoteNodeIdType RemoteNodeIndex = InstanceCfgPtr->RemoteNode;
      const Sd_RemoteNodeIdType RemoteNodeIndexEnd = RemoteNodeIndex + InstanceCfgPtr->RemoteNodeCount;
      /* if the number of active response timers is different to 0, this means that there is at least one remote node configured for this instance,
       that's why a do-while loop here doesn't carries the risk of "RemoteNodeStatePtr" being a NULL pointer */
      P2VAR(Sd_RemoteNodeStateType,AUTOMATIC,SD_VAR) RemoteNodeStatePtr = &Sd_RemoteNodeStates[RemoteNodeIndex];

      do
      { /* loop along all configured remote nodes of this instance to check which one of them has an active retry timer */
        if(RemoteNodeStatePtr->RetryTimer > 0U)
        { /* active remote node, decrement retry timer with the retry delta value */
          RemoteNodeStatePtr->RetryTimer -= InstanceStatePtr->RetryTimersDelta;
          /* once response timer has been decremented check if its time is out */
          if(RemoteNodeStatePtr->RetryTimer == 0U)
          {
            Sd_Send_setupBuffer(&RemoteNodeStatePtr->TxState, InstanceIdx);
            /* retry timer of this remote node is over, send all due retries */
            RemoteNodeStatePtr->ClientServicesRetriesToProcess =
            Sd_ClientService_sendRetriesOfSubscribeEventGroups(InstanceCfgPtr, RemoteNodeIndex, RemoteNodeStatePtr->ClientServicesRetriesToProcess);
            /* if there are still retries to be sent out to this remote node... */
            if(RemoteNodeStatePtr->ClientServicesRetriesToProcess > 0U)
            { /* ... then retry timer will have to be initialized again to the configured retry timer */
              RemoteNodeStatePtr->RetryTimer = InstanceCfgPtr->SdSubscribeEventgroupRetryDelay;
            }
            Sd_Send_flushBuffer();
          }
          /* set the next shorter retry timer to a remote node that will trigger retries to be sent to this remote node */
          if( (InstanceStatePtr->RetryTimersDeltaTimer == 0U) || (RemoteNodeStatePtr->RetryTimer < InstanceStatePtr->RetryTimersDeltaTimer) )
          {
            InstanceStatePtr->RetryTimersDeltaTimer = RemoteNodeStatePtr->RetryTimer;
          }
        }
        ++RemoteNodeIndex;
        ++RemoteNodeStatePtr;
      } /* loop continues until all active response timers in this instance have been processed */
      while(RemoteNodeIndex < RemoteNodeIndexEnd);

      /* absolute retry delta timer of this instance contains the next (shorter) remote node retry timer */
      InstanceStatePtr->RetryTimersDelta = InstanceStatePtr->RetryTimersDeltaTimer;
    }
  }
}
#endif /* SD_SUBSCRIBE_EVENTGROUP_RETRY_ENABLED == STD_ON */



STATIC FUNC(void,SD_CODE) Sd_ResetRemoteNodeStates
(
  CONSTP2CONST(SdInstance_t, AUTOMATIC, SD_APPL_DATA) InstanceCfgPtr,
  CONSTP2VAR(Sd_InstanceStateType,AUTOMATIC,SD_APPL_DATA) InstanceStatePtr
)
{
  Sd_RemoteNodeIdType RemoteNodeIndex = InstanceCfgPtr->RemoteNode;
  const Sd_RemoteNodeIdType RemoteNodeIndexEnd = RemoteNodeIndex + InstanceCfgPtr->RemoteNodeCount;

  while(RemoteNodeIndex < RemoteNodeIndexEnd)
  {
    Sd_RemoteNodeStates[RemoteNodeIndex].TxState.Address.domain = TCPIP_AF_UNSPEC;
    Sd_RemoteNodeStates[RemoteNodeIndex].ResponseTimer = 0U;
    ++RemoteNodeIndex;
  }
  /* Reset Multicast-TX-RebootState of this instance */
  InstanceStatePtr->TxState.RebootState.SessionId = 1U;
  InstanceStatePtr->TxState.RebootState.RebootFlag = TRUE;
}



STATIC FUNC(boolean, SD_CODE) Sd_CompareAddress
(
  CONSTP2CONST(TcpIp_SockAddrInet6Type,AUTOMATIC,SD_APPL_CODE) AddrPtr1,
  CONSTP2CONST(TcpIp_SockAddrInet6Type,AUTOMATIC,SD_APPL_CODE) AddrPtr2
)
{
  boolean RetVal = FALSE;
#if (SD_DEV_ERROR_DETECT == STD_ON)
  SD_PRECONDITION_ASSERT( (AddrPtr1->domain == AddrPtr2->domain), SD_COMPAREADDRESS_SVCID );
#endif

  {
    if(AddrPtr1->domain == TCPIP_AF_INET)
    {
      if( (AddrPtr1->addr[0] == AddrPtr2->addr[0]) &&
          (AddrPtr1->port == AddrPtr2->port) )
      {
        RetVal = TRUE;
      }
    }
    else
    {
      if( (TS_MemCmp(AddrPtr1->addr, AddrPtr2->addr, 16U) == E_OK) &&
          (AddrPtr1->port == AddrPtr2->port) )
      {
        RetVal = TRUE;
      }
    }
  }
  return RetVal;
}

#define SD_STOP_SEC_CODE
#include <Sd_MemMap.h>

/*==================[end of file]===========================================*/
