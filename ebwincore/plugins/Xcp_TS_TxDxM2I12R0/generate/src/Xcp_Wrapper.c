/**
 * \file
 *
 * \brief AUTOSAR Xcp
 *
 * This file contains the implementation of the AUTOSAR
 * module Xcp.
 *
 * \version 2.12.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!INCLUDE "../../generate_macros/Xcp_Vars.m"!][!//
/*==================[inclusions]============================================*/

#include <Xcp_Wrapper_Trace.h>
#include <Std_Types.h>        /* AUTOSAR standard types */
#include <TSAutosar.h>        /* EB specific standard types */


/* include service wrapper header file */
#include <Xcp_Wrapper.h>

/* Xcp header file */
#include <Xcp.h>
#include <Xcp_Cbk.h>

#if (XCP_ON_CDD_ENABLED == STD_ON)
[!VAR "Index"="1"!][!//
[!LOOP "text:split($CddHeaderFileList, ' ')"!][!//
#include <[!"."!]>          /* AUTOSAR [!"text:split($CddShortNameList, ' ')[num:i($Index)]"!] on CDD callback header */
[!VAR "Index"="$Index + 1"!][!//
[!ENDLOOP!][!//
#endif

/* include user header file*/
[!IF "normalize-space(XcpGeneral/XcpUserHeader) != ''"!][!//
#include <[!"normalize-space(XcpGeneral/XcpUserHeader)"!]>   /*includes the prototype of App_Xcp_Wrapper_IsXcpUsed */
[!ENDIF!][!//

[!IF "$XcpBswDistributionEnabled"!][!//
[!IF "XcpGeneral/XcpServiceWrapper = 'true'"!][!//
[!VAR "XcpConnectionOsAppRefNameList" = "''"!][!//
[!LOOP "(XcpConfig/*[1]/XcpConnectionCfg/*)"!][!//
[!IF "node:exists(XcpConnectionApplicationRef)"!][!//
  [!VAR "XcpConnectionOsAppRefName"="name(as:ref(./XcpConnectionApplicationRef))"!][!//
  [!IF "not(contains($XcpConnectionOsAppRefNameList,concat(' ',$XcpConnectionOsAppRefName,' ')))"!][!//
#include <SchM_Xcp_1_[!"$XcpConnectionOsAppRefName"!].h>
    [!VAR "XcpConnectionOsAppRefNameList" = "concat($XcpConnectionOsAppRefNameList,' ',$XcpConnectionOsAppRefName,' ')"!][!//
  [!ENDIF!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDIF!][!//
[!ENDIF!][!//
/*==================[macros]================================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef XCP_VENDOR_ID /* configuration check */
#error XCP_VENDOR_ID must be defined
#endif

#if (XCP_VENDOR_ID != 1U) /* vendor check */
#error XCP_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef XCP_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error XCP_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (XCP_AR_RELEASE_MAJOR_VERSION != 4U)
#error XCP_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef XCP_AR_RELEASE_MINOR_VERSION /* configuration check */
#error XCP_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (XCP_AR_RELEASE_MINOR_VERSION != 0U )
#error XCP_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef XCP_AR_RELEASE_REVISION_VERSION /* configuration check */
#error XCP_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (XCP_AR_RELEASE_REVISION_VERSION != 3U )
#error XCP_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef XCP_SW_MAJOR_VERSION /* configuration check */
#error XCP_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (XCP_SW_MAJOR_VERSION != 2U)
#error XCP_SW_MAJOR_VERSION wrong (!= 2U)
#endif

#ifndef XCP_SW_MINOR_VERSION /* configuration check */
#error XCP_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (XCP_SW_MINOR_VERSION < 12U)
#error XCP_SW_MINOR_VERSION wrong (< 12U)
#endif

#ifndef XCP_SW_PATCH_VERSION /* configuration check */
#error XCP_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (XCP_SW_PATCH_VERSION < 0U)
#error XCP_SW_PATCH_VERSION wrong (< 0U)
#endif
/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/
#if (defined XCP_MODULE_DISABLED)
#error XCP_MODULE_DISABLED already defined
#endif
/** \brief Constant macrodefinition for checking the state of Xcp module */
#define XCP_MODULE_DISABLED                             0U

/*==================[external constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/
[!AUTOSPACING!][!//
[!VAR "XcpEventChannelMFEnabled"="(XcpGeneral/XcpEventTriggerMainFunc = 'true') and (node:exists(XcpConfig/*[1]/XcpEventChannel)) and (num:i(count(XcpConfig/*[1]/XcpEventChannel/*)) > 0)"!]
[!VAR "XcpCrcMFEnabled"="XcpGeneral/XcpBuildChecksumMainFunctionSupport = 'true'"!]
[!VAR "XcpComMFEnabled"="node:exists(XcpGeneral/XcpSupportComMainFunctions) = 'true'"!]
[!VAR "XcpWrapperEnabled"="XcpGeneral/XcpServiceWrapper"!]

[!IF "$XcpWrapperEnabled"!]
#define XCP_WRAPPER_START_SEC_WRAPPER_CODE
#include <Xcp_MemMap.h>

/*------------------[Xcp_Wrapper_MainFunction]-------------------------------------------*/
FUNC(void,XCP_WRAPPER_CODE) Xcp_Wrapper_MainFunction(void)
{
  DBG_XCP_WRAPPER_MAINFUNCTION_ENTRY();
  /* Check if Xcp module shall be called */
  /* !LINKSTO Xcp.Disable.Wrapper.Functionality,1 */
  if (XCP_MODULE_DISABLED != App_Xcp_Wrapper_IsXcpUsed())
  {
    Xcp_MainFunction();
  }
  else
  {
    /* Function shall return without any effect */
  }
  DBG_XCP_WRAPPER_MAINFUNCTION_EXIT();
}

[!IF "$XcpEventChannelMFEnabled"!]
  [!LOOP "XcpConfig/*[1]/XcpEventChannel/*"!]
    [!IF "XcpEventChannelTimeCycle > 0"!]
/*------------------[Xcp_Wrapper_MainFunctionW_EventChannel-------------------------------*/
FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_MainFunction_[!"@name"!](void)
{
  DBG_XCP_WRAPPER_MAINFUNCTION_[!"text:toupper(@name)"!]_ENTRY();
  /* Check if Xcp module shall be called */
    /* !LINKSTO Xcp.Disable.Wrapper.Functionality,1 */
  if (XCP_MODULE_DISABLED != App_Xcp_Wrapper_IsXcpUsed())
  {
    Xcp_MainFunction_[!"@name"!]();
  }
  else
  {
    /* Function shall return without any effect */
  }
  DBG_XCP_WRAPPER_MAINFUNCTION_[!"text:toupper(@name)"!]_EXIT();
}
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDIF!]

[!IF "$XcpCrcMFEnabled"!]
/*------------------[Xcp_Wrapper_CrcMainFunction]----------------------------------------*/
FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_CrcMainFunction(void)
{
  DBG_XCP_WRAPPER_CRCMAINFUNCTION_ENTRY();
  /* Check if Xcp module shall be called */
  /* !LINKSTO Xcp.Disable.Wrapper.Functionality,1 */
  if (XCP_MODULE_DISABLED != App_Xcp_Wrapper_IsXcpUsed())
  {
    Xcp_CrcMainFunction();
  }
  else
  {
    /* Function shall return without any effect */
  }

  DBG_XCP_WRAPPER_CRCMAINFUNCTION_EXIT();
}
[!ENDIF!]

[!IF "$XcpComMFEnabled = 'true'"!]
/*------------------[Xcp_Wrapper_TxMainFunction]-----------------------------------------*/
FUNC(void,XCP_WRAPPER_CODE) Xcp_Wrapper_TxMainFunction(void)
{
  DBG_XCP_WRAPPER_TXMAINFUNCTION_ENTRY();

  /* Check if Xcp module shall be called */
  /* !LINKSTO Xcp.Disable.Wrapper.Functionality,1 */
  if (XCP_MODULE_DISABLED != App_Xcp_Wrapper_IsXcpUsed())
  {
    Xcp_TxMainFunction();
  }
  else
  {
    /* Function shall return without any effect */
  }

  DBG_XCP_WRAPPER_TXMAINFUNCTION_EXIT();
}

/*------------------[Xcp_Wrapper_RxMainFunction]-----------------------------------------*/
FUNC(void,XCP_WRAPPER_CODE) Xcp_Wrapper_RxMainFunction(void)
{
  DBG_XCP_WRAPPER_RXMAINFUNCTION_ENTRY();

  /* Check if Xcp module shall be called */
  /* !LINKSTO Xcp.Disable.Wrapper.Functionality,1 */
  if (XCP_MODULE_DISABLED != App_Xcp_Wrapper_IsXcpUsed())
  {
    Xcp_RxMainFunction();
  }
  else
  {
    /* Function shall return without any effect */
  }

  DBG_XCP_WRAPPER_RXMAINFUNCTION_EXIT();
}
[!ENDIF!]

#if (XCP_VERSION_INFO_API == STD_ON)
/*------------------[Xcp_Wrapper_GetVersionInfo]-----------------------------------------*/
FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, XCP_APPL_DATA) VersionInfoPtr
)
{
  DBG_XCP_WRAPPER_GETVERSIONINFO_ENTRY(VersionInfoPtr);
  /* Check if Xcp module shall be called */
  /* !LINKSTO Xcp.Disable.Wrapper.Functionality,1 */
  if (XCP_MODULE_DISABLED != App_Xcp_Wrapper_IsXcpUsed())
  {
    Xcp_GetVersionInfo(VersionInfoPtr);
  }
  else
  {
    /* Function shall return without any effect */
  }
  DBG_XCP_WRAPPER_GETVERSIONINFO_EXIT(VersionInfoPtr);
}
#endif

/*------------------[Xcp_Wrapper_Init]---------------------------------------------------*/
FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_Init
(
  P2CONST(Xcp_ConfigType, AUTOMATIC, XCP_APPL_CONST) Xcp_ConfigPtr
)
{
  DBG_XCP_WRAPPER_INIT_ENTRY(Xcp_ConfigPtr);
  /* Check if Xcp module shall be called */
  /* !LINKSTO Xcp.Disable.Wrapper.Functionality,1 */
  if (XCP_MODULE_DISABLED != App_Xcp_Wrapper_IsXcpUsed())
  {
    Xcp_Init(Xcp_ConfigPtr);
  }
  else
  {
    /* Function shall return without any effect */
  }
  DBG_XCP_WRAPPER_INIT_EXIT(Xcp_ConfigPtr);
}

/*------------------[Xcp_Wrapper_SetEvent]-----------------------------------------------*/
FUNC(Xcp_ReturnType, XCP_WRAPPER_CODE) Xcp_Wrapper_SetEvent
(
  uint16 EventId
)
{
  Xcp_ReturnType ret_val = XCP_NOT_OK;
  DBG_XCP_WRAPPER_SETEVENT_ENTRY(EventId);
  /* Check if Xcp module shall be called */
  /* !LINKSTO Xcp.Disable.Wrapper.Functionality,1 */
  if (XCP_MODULE_DISABLED != App_Xcp_Wrapper_IsXcpUsed())
  {
    ret_val = Xcp_SetEvent(EventId);
  }
  else
  {
    /* Function shall return without any effect */
  }
  DBG_XCP_WRAPPER_SETEVENT_EXIT(ret_val, EventId);
  return ret_val;
}

#if ((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON))
/*------------------[Xcp_Wrapper_CanIfRxIndication]--------------------------------------*/
FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_CanIfRxIndication
(
  PduIdType XcpRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) XcpRxPduPtr
)
{
  DBG_XCP_WRAPPER_CANIFRXINDICATION_ENTRY(XcpRxPduId, XcpRxPduPtr);
  /* Check if Xcp module shall be called */
  /* !LINKSTO Xcp.Disable.Wrapper.Functionality,1 */
  if (XCP_MODULE_DISABLED != App_Xcp_Wrapper_IsXcpUsed())
  {
    Xcp_CanIfRxIndication(XcpRxPduId, XcpRxPduPtr);
  }
  else
  {
    /* Function shall return without any effect */
  }
  DBG_XCP_WRAPPER_CANIFRXINDICATION_EXIT(XcpRxPduId, XcpRxPduPtr);
}

/*------------------[Xcp_Wrapper_CanIfTxConfirmation]------------------------------------*/
FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_CanIfTxConfirmation
(
  PduIdType XcpTxPduId
)
{
  DBG_XCP_WRAPPER_CANIFTXCONFIRMATION_ENTRY(XcpTxPduId);
  /* Check if Xcp module shall be called */
  /* !LINKSTO Xcp.Disable.Wrapper.Functionality,1 */
  if (XCP_MODULE_DISABLED != App_Xcp_Wrapper_IsXcpUsed())
  {
    Xcp_CanIfTxConfirmation(XcpTxPduId);
  }
  else
  {
    /* Function shall return without any effect */
  }
  DBG_XCP_WRAPPER_CANIFTXCONFIRMATION_EXIT(XcpTxPduId);
}
#endif /* ((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON)) */

#if (XCP_ON_ETHERNET_ENABLED == STD_ON)
/*------------------[Xcp_Wrapper_SoAdIfRxIndication]-------------------------------------*/
FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_SoAdIfRxIndication
(
  PduIdType XcpRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) XcpRxPduPtr
)
{
  DBG_XCP_WRAPPER_SOADIFRXINDICATION_ENTRY(XcpRxPduId, XcpRxPduPtr);
  /* Check if Xcp module shall be called */
  /* !LINKSTO Xcp.Disable.Wrapper.Functionality,1 */
  if (XCP_MODULE_DISABLED != App_Xcp_Wrapper_IsXcpUsed())
  {
    Xcp_SoAdIfRxIndication(XcpRxPduId, XcpRxPduPtr);
  }
  else
  {
    /* Function shall return without any effect */
  }
  DBG_XCP_WRAPPER_SOADIFRXINDICATION_EXIT(XcpRxPduId, XcpRxPduPtr);
}

/*------------------[Xcp_Wrapper_SoAdIfTxConfirmation]-----------------------------------*/
FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_SoAdIfTxConfirmation
(
  PduIdType XcpTxPduId
)
{
  DBG_XCP_WRAPPER_SOADIFTXCONFIRMATION_ENTRY(XcpTxPduId);
  /* Check if Xcp module shall be called */
  /* !LINKSTO Xcp.Disable.Wrapper.Functionality,1 */
  if (XCP_MODULE_DISABLED != App_Xcp_Wrapper_IsXcpUsed())
  {
    Xcp_SoAdIfTxConfirmation(XcpTxPduId);
  }
  else
  {
    /* Function shall return without any effect */
  }
  DBG_XCP_WRAPPER_SOADIFTXCONFIRMATION_EXIT(XcpTxPduId);
}

#if (XCP_SOAD_PROTOCOL_TCP_SUPPORT == STD_ON)

/*------------------[Xcp_Wrapper_SoAdSoConModeChg]---------------------------------------*/
FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_SoAdSoConModeChg
(
  SoAd_SoConIdType SoConId,
  SoAd_SoConModeType Mode
)
{
  DBG_XCP_WRAPPER_SOADSOCONMODECHG_ENTRY(SoConId, Mode);
  /* Check if Xcp module shall be called */
  /* !LINKSTO Xcp.Disable.Wrapper.Functionality,1 */
  if (XCP_MODULE_DISABLED != App_Xcp_Wrapper_IsXcpUsed())
  {
    Xcp_SoAdSoConModeChg(SoConId, Mode);
  }
  else
  {
    /* Function shall return without any effect */
  }
  DBG_XCP_WRAPPER_SOADSOCONMODECHG_EXIT(SoConId, Mode)
}
#endif /* (XCP_SOAD_PROTOCOL_TCP_SUPPORT == STD_ON) */
#endif /* (XCP_ON_ETHERNET_ENABLED == STD_ON) */

#if (XCP_ON_FLEXRAY_ENABLED == STD_ON)
/*------------------[Xcp_Wrapper_FrIfRxIndication]---------------------------------------*/
FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_FrIfRxIndication
(
  PduIdType XcpRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) XcpRxPduPtr
)
{
  DBG_XCP_WRAPPER_FRIFRXINDICATION_ENTRY(XcpRxPduId, XcpRxPduPtr);
  /* Check if Xcp module shall be called */
  /* !LINKSTO Xcp.Disable.Wrapper.Functionality,1 */
  if (XCP_MODULE_DISABLED != App_Xcp_Wrapper_IsXcpUsed())
  {
    Xcp_FrIfRxIndication(XcpRxPduId, XcpRxPduPtr);
  }
  else
  {
    /* Function shall return without any effect */
  }
  DBG_XCP_WRAPPER_FRIFRXINDICATION_EXIT(XcpRxPduId, XcpRxPduPtr);
}

/*------------------[Xcp_Wrapper_FrIfTxConfirmation]-------------------------------------*/
FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_FrIfTxConfirmation
(
  PduIdType XcpTxPduId
)
{
  DBG_XCP_WRAPPER_FRIFTXCONFIRMATION_ENTRY(XcpTxPduId);
  /* Check if Xcp module shall be called */
  /* !LINKSTO Xcp.Disable.Wrapper.Functionality,1 */
  if (XCP_MODULE_DISABLED != App_Xcp_Wrapper_IsXcpUsed())
  {
    Xcp_FrIfTxConfirmation(XcpTxPduId);
  }
  else
  {
    /* Function shall return without any effect */
  }
  DBG_XCP_WRAPPER_FRIFTXCONFIRMATION_EXIT(XcpTxPduId);
}

#if (XCP_TRIGGER_TX_API_ENABLED == STD_ON)
/*------------------[Xcp_Wrapper_FrIfTriggerTransmit]-------------------------------------*/
FUNC(Std_ReturnType, XCP_WRAPPER_CODE) Xcp_Wrapper_FrIfTriggerTransmit
(
  PduIdType XcpTxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) PduInfoPtr
)
{
  Std_ReturnType ret_val = E_NOT_OK;
  DBG_XCP_WRAPPER_FRIFTRIGGERTRANSMIT_ENTRY(XcpTxPduId, PduInfoPtr);
  /* Check if Xcp module shall be called */
  /* !LINKSTO Xcp.Disable.Wrapper.Functionality,1 */
  if (XCP_MODULE_DISABLED != App_Xcp_Wrapper_IsXcpUsed())
  {
    ret_val = Xcp_FrIfTriggerTransmit(XcpTxPduId, PduInfoPtr);
  }
  else
  {
    /* Function shall return without any effect */
  }
  DBG_XCP_WRAPPER_FRIFTRIGGERTRANSMIT_EXIT(ret_val, XcpTxPduId, PduInfoPtr);
  return ret_val;
}
#endif /* (XCP_TRIGGER_TX_API_ENABLED == STD_ON) */
#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) */

#if (XCP_ON_CDD_ENABLED == STD_ON)
/*------------------[Xcp_Wrapper_CddRxIndication]---------------------------------------*/
FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_CddRxIndication
(
  PduIdType XcpRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) XcpRxPduPtr
)
{
  /* Check if Xcp module shall be called */
  /* !LINKSTO Xcp.Disable.Wrapper.Functionality,1 */
  if (XCP_MODULE_DISABLED != App_Xcp_Wrapper_IsXcpUsed())
  {
    Xcp_CddRxIndication(XcpRxPduId, XcpRxPduPtr);
  }
  else
  {
    /* Function shall return without any effect */
  }
}

/*------------------[Xcp_Wrapper_CddTxConfirmation]-------------------------------------*/
FUNC(void, XCP_WRAPPER_CODE) Xcp_Wrapper_CddTxConfirmation
(
  PduIdType XcpTxPduId
)
{
  /* Check if Xcp module shall be called */
  /* !LINKSTO Xcp.Disable.Wrapper.Functionality,1 */
  if (XCP_MODULE_DISABLED != App_Xcp_Wrapper_IsXcpUsed())
  {
    Xcp_CddTxConfirmation(XcpTxPduId);
  }
  else
  {
    /* Function shall return without any effect */
  }
}

#if (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON)
/*------------------[Xcp_Wrapper_CddTriggerTransmit]-------------------------------------*/
FUNC(Std_ReturnType, XCP_WRAPPER_CODE) Xcp_Wrapper_CddTriggerTransmit
(
  PduIdType XcpTxPduId,
  P2VAR(PduInfoType, AUTOMATIC, XCP_APPL_DATA) PduInfoPtr
)
{
  Std_ReturnType ret_val = E_NOT_OK;

  /* Check if Xcp module shall be called */
  /* !LINKSTO Xcp.Disable.Wrapper.Functionality,1 */
  if (XCP_MODULE_DISABLED != App_Xcp_Wrapper_IsXcpUsed())
  {
    ret_val = Xcp_CddTriggerTransmit(XcpTxPduId, PduInfoPtr);
  }
  else
  {
    /* Function shall return without any effect */
  }

  return ret_val;
}
#endif /* (XCP_CDD_TRIGGER_TX_API_ENABLED == STD_ON) */
#endif /* (XCP_ON_CDD_ENABLED == STD_ON) */

#if (XCP_SUPPRESS_TX_SUPPORT == STD_ON)

FUNC(void, XCP_CODE)  Xcp_Wrapper_SetTransmissionMode
(
  NetworkHandleType Channel,
  Xcp_TransmissionModeType Mode
)
{
  /* Check if Xcp module shall be called */
  /* !LINKSTO Xcp.Disable.Wrapper.Functionality,1 */
  if (XCP_MODULE_DISABLED != App_Xcp_Wrapper_IsXcpUsed())
  {
    Xcp_SetTransmissionMode(Channel, Mode);
  }
  else
  {
    /* Function shall return without any effect */
  }
}

#endif /* #if (XCP_SUPPRESS_TX_SUPPORT == STD_ON) */

#define XCP_WRAPPER_STOP_SEC_WRAPPER_CODE
#include <Xcp_MemMap.h>
[!ENDIF!]

[!IF "$XcpBswDistributionEnabled"!][!//
[!SELECT "XcpConfig/*[1]"!][!//

[!VAR "XcpConnectionOsAppRefNameList" = "''"!][!//
[!LOOP "(XcpConnectionCfg/*)"!][!//
  [!IF "node:exists(XcpConnectionApplicationRef)"!][!//
  [!VAR "XcpConnectionOsAppRefName"="name(as:ref(./XcpConnectionApplicationRef))"!][!//
  [!IF "not(contains($XcpConnectionOsAppRefNameList,concat(' ',$XcpConnectionOsAppRefName,' ')))"!][!//

[!IF "../../../../XcpGeneral/XcpServiceWrapper = 'true'"!][!//
/*-------------[Xcp BSW distribution external function definitions]---------------*/
#define XCP_1_[!"$XcpConnectionOsAppRefName"!]_START_SEC_CODE
#include <Xcp_1_[!"$XcpConnectionOsAppRefName"!]_MemMap.h>
[!ENDIF!][!//

[!IF "../../../../XcpGeneral/XcpServiceWrapper = 'true'"!][!//
FUNC(void, XCP_1_[!"$XcpConnectionOsAppRefName"!]_CODE) Xcp_1_[!"$XcpConnectionOsAppRefName"!]_Wrapper_MainFunction(void)
{
  /* Xcp_Wrapper_MainFunction(); */
}
[!ENDIF!][!//

[!IF "../../../../XcpGeneral/XcpEventTriggerMainFunc = 'true'"!][!//
  [!IF "(node:exists(../../XcpEventChannel)) and (num:i(count(../../XcpEventChannel/*)) > 0)"!][!//
    [!LOOP "../../XcpEventChannel/*"!][!//
      [!IF "./XcpEventChannelTimeCycle > 0"!][!//
        [!IF "../../../../XcpGeneral/XcpServiceWrapper = 'true'"!][!//
FUNC(void, XCP_1_[!"$XcpConnectionOsAppRefName"!]_CODE) Xcp_1_[!"$XcpConnectionOsAppRefName"!]_Wrapper_MainFunction_[!"@name"!](void)
{
  /* Xcp_Wrapper_MainFunction_[!"@name"!](); */
}
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
  [!ENDIF!][!//
[!ENDIF!][!//

[!IF "node:exists(../../../../XcpGeneral/XcpSupportComMainFunctions) = 'true'"!][!//
  [!IF "../../../../XcpGeneral/XcpServiceWrapper = 'true'"!][!//
FUNC(void, XCP_1_[!"$XcpConnectionOsAppRefName"!]_CODE) Xcp_1_[!"$XcpConnectionOsAppRefName"!]_Wrapper_RxMainFunction(void)
{
  /* Xcp_Wrapper_RxMainFunction(); */
}

FUNC(void, XCP_1_[!"$XcpConnectionOsAppRefName"!]_CODE) Xcp_1_[!"$XcpConnectionOsAppRefName"!]_Wrapper_TxMainFunction(void)
{
  /* Xcp_Wrapper_TxMainFunction(); */
}
  [!ENDIF!][!//
[!ENDIF!][!//

[!IF "../../../../XcpGeneral/XcpServiceWrapper = 'true'"!][!//
#define XCP_1_[!"$XcpConnectionOsAppRefName"!]_STOP_SEC_CODE
#include <Xcp_1_[!"$XcpConnectionOsAppRefName"!]_MemMap.h>
[!ENDIF!][!//

  [!VAR "XcpConnectionOsAppRefNameList" = "concat($XcpConnectionOsAppRefNameList,' ',$XcpConnectionOsAppRefName,' ')"!][!//
  [!ENDIF!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//

[!ENDSELECT!][!//
[!ENDIF!][!//
/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
