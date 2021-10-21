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

/* 
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different object type.
 *
 * Reason:
 * The cast is required and unavoidable since there is no way of knowing what the configured memory area points to.
 * This is also used for converting the uint8 pointers in the Xcp_OdtAddressTableRAM array to usize pointers in order
 * to be compatible with the OdtEntry field of the Xcp_OdtType structure.
 *
 * MISRAC2012-2) Deviated Rule: 11.6 (required)
 * A cast shall not be performed between pointer to void and an arithmetic type.
 *
 * Reason:
 * According to this rule, casting a pointer to void to an integer arithmetic type is not considered safe because the
 * the resulting value can be outside the allowed range for the type. However, the cast of the void pointer is made to
 * type usize which is guaranteed to hold a pointer.
 *
 * MISRAC2012-3) Deviated Rule: 11.4 (advisory)
 * A conversion should not be performed between a pointer to object and an integer type.
 *
 * Reason:
 * Required in order to determine if the pointer parameter of Xcp_GetOdtEntryData is in the address space of the Xcp_OdtAddressTableRAM
 */
[!INCLUDE "../../generate_macros/Xcp_Vars.m"!][!//
/*==================[inclusions]============================================*/

#include <Std_Types.h>         /* AUTOSAR standard types */
#include <Xcp.h>               /* Module public API */
#if ( (XCP_DEFAULT_XCP_MODULE_STATE == STD_ON) || (XCP_ENABLE_XCP_CONTROL_API == STD_ON) )
#include <Xcp_Int.h>           /* Module internal interface */
[!IF "normalize-space(XcpGeneral/XcpUserHeader) != ''"!][!//
#include <[!"normalize-space(XcpGeneral/XcpUserHeader)"!]> /* User-defined symbols */
[!ENDIF!][!//

#if (XCP_ON_CDD_ENABLED == STD_ON)
[!VAR "Index"="1"!][!//
[!LOOP "text:split($CddHeaderFileList, ' ')"!][!//
#include <[!"."!]>          /* AUTOSAR [!"text:split($CddShortNameList, ' ')[num:i($Index)]"!] callback header */
[!VAR "Index"="$Index + 1"!][!//
[!ENDLOOP!][!//
#endif
[!// Macro needs to be called only with container XcpConfig/*[1] selected
[!// AddrExtn BitOffset Length Address
[!/* Generates the non-dynamic ODT entry information based on the DAQ configuration.
When the 'member' parameter is 'Address', 'Length', or 'BitOffset' only the respective
information is generated and this can be used for the Xcp_Odt*TableROM arrays.
When the 'member' parameter is 'all' (default) all the above are generated and this can
be used for the Xcp_DaqListsDefault array.
*/!][!//
[!MACRO "XcpGenerateOdtEntries","member"="'all'"!][!//
[!IF "$member='all'"!][!//
  {
[!ENDIF!][!//
[!LOOP "node:order(XcpDaqList/*, 'XcpDaqListNumber')"!]
[!VAR "SelectedDaqListId" = "num:i(XcpDaqListNumber)"!]
[!IF "(XcpDaqListNumber < $XcpMinDaq)"!][!//
[!IF "XcpDaqListNumber = 0 and $member='all'"!][!//
    /* ------ ODT Entries for all predefined DAQ lists ------- */
[!ENDIF!][!//
[!LOOP "node:order(XcpOdt/*, 'XcpOdtNumber')"!]
[!VAR "SelectedOdtId" = "num:i(XcpOdtNumber)"!]
[!IF "$member='all'"!][!//
    /* -- ODT Entries List for DaqListId = [!"$SelectedDaqListId"!] and OdtId = [!"num:i(XcpOdtNumber)"!] -- */
[!ENDIF!][!//
        [!LOOP "node:order(XcpOdtEntry/*, 'XcpOdtEntryNumber')"!]
[!IF "$member='all'"!][!//
    /* ODT Entry configuration for the OdtEntryId = [!"num:i(XcpOdtEntryNumber)"!] */
    {
[!ENDIF!][!//
[!IF "$member='all' or $member='Address'"!][!//
[!IF "$member='all'"!][!WS "4"!][!ENDIF!][!//
  /* The address location of the ODT entry */
[!IF "XcpOdtEntryAddress = 0"!][!//
[!IF "$member='all'"!][!WS "4"!][!ENDIF!][!//
  NULL_PTR,
[!ELSE!][!//
[!IF "$member='all'"!][!WS "4"!][!ENDIF!][!//
[!WS "2"!]
(P2VAR(uint8, XCP_CONST, XCP_APPL_DATA))[!"XcpOdtEntryAddress"!],
[!ENDIF!][!//
[!ENDIF!][!//
[!IF "$member='all' or $member='Length'"!][!//
[!IF "$member='all'"!][!WS "4"!][!ENDIF!][!//
  /* Size of the element in AG */
[!IF "$member='all'"!][!WS "4"!][!ENDIF!][!//
  [!"XcpOdtEntryLength"!]U,
[!ENDIF!]
[!IF "$member='all' or $member='BitOffset'"!][!//
[!IF "$member='all'"!][!WS "4"!][!ENDIF!][!//
  /* Position of the Bit to be considered */
[!IF "node:exists(XcpOdtEntryBitOffset) = 'true'"!][!//
[!IF "$member='all'"!][!WS "4"!][!ENDIF!][!//
  [!"XcpOdtEntryBitOffset"!]U,
[!ELSE!][!/* !LINKSTO Xcp.RamOptimizations.XcpEnableBitOffset.False.Default,1 */!][!//
[!IF "$member='all'"!][!WS "4"!][!ENDIF!][!//
  0xFFU,
[!ENDIF!][!//
[!ENDIF!][!//
[!IF "$member='all'"!][!//
      /* Address extension of the ODT */
      0U,
    },
[!ENDIF!][!//
[!ENDLOOP!]
[!ENDLOOP!]
[!ELSE!][!//
[!IF "XcpDaqListNumber = $XcpMinDaq and $member='all'"!][!//
    /* -- ODT Entries for all static configurable DAQ lists -- */
[!ENDIF!][!//
[!FOR "x" = "1" TO "num:i(XcpMaxOdt)"!]
[!IF "(XcpMaxOdtEntries > $XcpAbsoluteMaxOdtEntrySize) and ($XcpTimestampIsEnabled = 'true') and ($x = 1)"!][!//
[!VAR "XcpRelativeOdtEntrySize" = "$XcpAbsoluteMaxOdtEntrySize"!]
[!ELSE!][!//
[!VAR "XcpRelativeOdtEntrySize" = "XcpMaxOdtEntries"!]
[!ENDIF!][!//
[!IF "$member='all'"!][!//
    /* -- ODT Entries List for DaqListId = [!"num:i(XcpDaqListNumber)"!] and OdtId = [!"num:i($x - 1)"!] -- */
[!ENDIF!][!//
        [!FOR "y" = "1" TO "$XcpRelativeOdtEntrySize"!]
[!IF "$member='all'"!][!//
    /* ODT Entry configuration for the OdtEntryId = [!"num:i($y - 1)"!] */
    {
[!ENDIF!][!//
[!IF "$member='all' or $member='Address'"!][!//
[!IF "$member='all'"!][!WS "4"!][!ENDIF!][!//
  /* The address location of the ODT entry */
[!IF "$member='all'"!][!WS "4"!][!ENDIF!][!//
  NULL_PTR,
[!ENDIF!][!//
[!IF "$member='all' or $member='Length'"!][!//
[!IF "$member='all'"!][!WS "4"!][!ENDIF!][!//
  /* Size of the element in AG */
[!IF "$member='all'"!][!WS "4"!][!ENDIF!][!//
  0U,
[!ENDIF!][!//
[!IF "$member='all' or $member='BitOffset'"!][!//
[!IF "$member='all'"!][!WS "4"!][!ENDIF!][!//
  /* Position of the Bit to be considered */
[!IF "$member='all'"!][!WS "4"!][!ENDIF!][!//
  0xFFU,
[!ENDIF!][!//
[!IF "$member='all'"!][!//
      /* Address extension of the ODT */
      0U,
    },
[!ENDIF!][!//
[!ENDFOR!]
[!ENDFOR!]
[!ENDIF!]
[!ENDLOOP!]
[!IF "$member='all'"!][!//
  },
[!ENDIF!][!//
[!ENDMACRO!][!//
[!IF "$XcpBswDistributionEnabled"!]
[!VAR "XcpConnectionOsAppRefNameList" = "''"!]
[!LOOP "(XcpConfig/*[1]/XcpConnectionCfg/*)"!]
  [!VAR "XcpConnectionOsAppRefName"="name(as:ref(./XcpConnectionApplicationRef))"!]
  [!IF "not(contains($XcpConnectionOsAppRefNameList,concat(' ',$XcpConnectionOsAppRefName,' ')))"!]
#include <SchM_Xcp_1_[!"$XcpConnectionOsAppRefName"!].h>
    [!VAR "XcpConnectionOsAppRefNameList" = "concat($XcpConnectionOsAppRefNameList,' ',$XcpConnectionOsAppRefName,' ')"!]
  [!ENDIF!]
[!ENDLOOP!]
[!AUTOSPACING!][!//
[!IF "$XcpMemoryAccessAreas"!]
#include <Xcp_MemoryProxy.h>


[!LOOP "text:split($XcpMemProxyOsAppRefNameList, ' ')"!]
  [!VAR "CurrentMemProxyPartition" = "."!][!//
  [!IF "$XcpConnectionInstancesCount = 1"!]
    [!LOOP "text:split($XcpConnectionOsAppRefNameList, ' ')"!]
      [!VAR "ConnectionPartition" = "."!][!//
    [!ENDLOOP!]
    [!IF "not(contains($CurrentMemProxyPartition,$ConnectionPartition))"!]
#include <SchM_Xcp_1_XcpMemoryProxy_[!"$CurrentMemProxyPartition"!].h>
    [!ENDIF!]
  [!ELSE!]
#include <SchM_Xcp_1_XcpMemoryProxy_[!"$CurrentMemProxyPartition"!].h>
  [!ENDIF!]
[!ENDLOOP!]

[!IF "($XcpDaqStimMemoryAccessAreas = 'true') or ($RAMCalloutProxyEnabled = 'true')"!][!//

[!LOOP "text:split($XcpMemProxyOsAppRefNameList, ' ')"!][!//
  [!VAR "CurrentMemProxyPartition" = "."!][!//
  [!LOOP "text:split($XcpConnectionOsAppRefNameList, ' ')"!][!//
    [!VAR "CurrentConnectionPartition" = "."!][!//
    [!IF "not(contains($CurrentConnectionPartition,$CurrentMemProxyPartition))"!]
#include <SchM_Xcp_1_XcpMemoryProxyResponse_[!"$CurrentConnectionPartition"!]_From_[!"$CurrentMemProxyPartition"!].h>
    [!ENDIF!][!//
  [!ENDLOOP!][!//
[!ENDLOOP!]
[!ENDIF!]
[!ENDIF!]


[!IF "count(text:split($XcpRemoteDestinations,' '))>0"!]
#include <SchM_Xcp_1_[!"$XcpMasterApplication"!].h>
[!VAR "AllPartitionsNeeded" = "$XcpConnectionOsAppRefNameList"!][!//
[!LOOP "text:split($XcpRemoteDestinations, ' ')"!]
  [!VAR "CurrentRemoteDestination" = "."!][!//
  [!IF "not(contains($AllPartitionsNeeded,concat(' ',$CurrentRemoteDestination,' ')))"!]
#include <SchM_Xcp_1_[!"."!].h>
    [!VAR "AllPartitionsNeeded" = "concat($AllPartitionsNeeded,' ',$CurrentRemoteDestination,' ')"!]
  [!ENDIF!]
[!ENDLOOP!]
[!ENDIF!]

#include <Os.h>
[!ENDIF!]

/*==================[macros]================================================*/


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/
[!SELECT "XcpConfig/*[1]"!][!//

[!IF "count(XcpEventChannel/*/XcpEventChannelTriggeredDaqListRef/*) > 0"!][!//
[!IF "../../XcpGeneral/XcpIdentificationFieldType = 'RELATIVE_BYTE'"!][!//
#define XCP_START_SEC_CONST_8
[!ELSE!][!//
#define XCP_START_SEC_CONST_16
[!ENDIF!][!//
#include <Xcp_MemMap.h>

/** \brief List of DAQ IDs in the order of events */
STATIC CONST(Xcp_DaqIdType, XCP_CONST) Xcp_DaqIdListConst[XCP_NUM_EVENT_CHANNEL_PRECFG] =
{
[!LOOP "node:order(XcpEventChannel/*, 'XcpEventChannelNumber')"!]
  /* Event channel: [!"XcpEventChannelNumber"!] */
  [!LOOP "node:order(XcpEventChannelTriggeredDaqListRef/*, '255 - as:ref(.)/XcpDaqListPriority')"!]
  [!"as:ref(.)/XcpDaqListNumber"!]U,
  [!ENDLOOP!]
[!ENDLOOP!]
};

[!IF "../../XcpGeneral/XcpIdentificationFieldType = 'RELATIVE_BYTE'"!][!//
#define XCP_STOP_SEC_CONST_8
[!ELSE!][!//
#define XCP_STOP_SEC_CONST_16
[!ENDIF!][!//
#include <Xcp_MemMap.h>

[!ENDIF!][!//

[!IF "count(XcpEventChannel/*) > 0"!]
#define XCP_START_SEC_CONST_8
#include <Xcp_MemMap.h>

[!LOOP "node:order(XcpEventChannel/*, 'XcpEventChannelNumber')"!]
/*  Event name of Event channel number : [!"XcpEventChannelNumber"!]  */
STATIC CONST(uint8, XCP_CONST) Xcp_EventName_[!"XcpEventChannelNumber"!][[!"num:i(string-length(name(.)))"!]] = {"[!"name(.)"!]"};
[!ENDLOOP!]

#define XCP_STOP_SEC_CONST_8
#include <Xcp_MemMap.h>

[!ENDIF!]

#define XCP_START_SEC_CONST_UNSPECIFIED
#include <Xcp_MemMap.h>

/** \brief List of Xcp Tx PDUs ordered by connection and by Xcp internal Id. */
STATIC CONST(PduIdType, XCP_CONST) Xcp_TxPduIdCfgList[XCP_NO_OF_TX_PDU_IDS] =
{
[!LOOP "node:order(XcpConnectionCfg/*,'XcpConnectionId')"!]
  /* Tx Pdu configuration for the [!"@name"!] connection */
  [!LOOP "node:order(XcpTxPduConnectionInfo/*,'node:ref(XcpMapTxPdu2Connection)/XcpTxPduId')"!]
  [!"node:ref(XcpMapTxPdu2Connection)/XcpTxPduId"!]U,
  [!ENDLOOP!]
[!ENDLOOP!]
};

#define XCP_STOP_SEC_CONST_UNSPECIFIED
#include <Xcp_MemMap.h>

/*==================[external constants]====================================*/

[!IF "$XcpMaxDaq > 0 and $XcpMaxDaq > $XcpDaqCount and $XcpNoOfNonDynamicODTEntries > 0 and ../../RamOptimizations/XcpEnableDaqOptimization='true'"!][!//
#define XCP_START_SEC_CONST_8
#include <Xcp_MemMap.h>

CONST(uint8, XCP_CONST) Xcp_OdtLengthTableROM[XCP_NO_OF_NON_DYNAMIC_ODTENTRIES] = 
{
[!CALL "XcpGenerateOdtEntries","member"="'Length'"!]
};

[!IF "../../RamOptimizations/XcpEnableBitOffset='true'"!][!//
CONST(uint8, XCP_CONST) Xcp_OdtBitOffsetTableROM[XCP_NO_OF_NON_DYNAMIC_ODTENTRIES] = 
{
[!CALL "XcpGenerateOdtEntries","member"="'BitOffset'"!]
};
[!ENDIF!][!//

#define XCP_STOP_SEC_CONST_8
#include <Xcp_MemMap.h>

[!ENDIF!][!//

#define XCP_START_SEC_CONST_UNSPECIFIED
#include <Xcp_MemMap.h>

/** \brief Configuration placeholder for Xcp_Init() call. */
CONST( Xcp_ConfigType, XCP_CONST ) [!"node:name(.)"!] = { 0U };

[!IF "../../XcpGeneral/XcpSupportForPDURoutingTable = 'true'"!]
/*-----------------------Xcp Pdu router Info--------------------------------*/
[!IF "(../../XcpGeneral/XcpOnCanEnabled = 'true') or (../../XcpGeneral/XcpOnCanFDEnabled = 'true')"!]
/** \brief Routing map for the incoming CanIf Pdus. */
CONST(Xcp_LPduIdRouteConfType, XCP_CONST )  Xcp_CanIfRxLPduIdRouteConf[XCP_NO_OF_CANIF_RX_PDU_IDS] =
{
[!LOOP "node:order(XcpPdu/*[@name='XcpRxPdu'], 'XcpRxPduId')"!][!//
  [!VAR "RxPduId" = "XcpRxPduId"!]
  [!SELECT "../../XcpConnectionCfg/*[count(XcpRxPduConnectionInfo/*[node:refvalid(XcpMapRxPdu2Connection) and (node:ref(XcpMapRxPdu2Connection)/XcpRxPduId = $RxPduId)]) > 0]"!]
    [!IF "(XcpConnectionInterfaceType = 'XcpConnectionOverCAN')or(XcpConnectionInterfaceType = 'XcpConnectionOverCANFD')"!]
      [!SELECT "XcpRxPduConnectionInfo/*[node:refvalid(XcpMapRxPdu2Connection) and (node:ref(XcpMapRxPdu2Connection)/XcpRxPduId = $RxPduId)]"!]
  {
    [!"num:i(XcpRxLowerLayerHandleId)"!]U, /* CanIf handle Id */
    [!"num:i($RxPduId)"!]U, /* Xcp internal handle Id */
  },
      [!ENDSELECT!]
    [!ENDIF!]
  [!ENDSELECT!]
[!ENDLOOP!]
};

/** \brief Routing map for the outgoing CanIf Pdus. */
CONST(Xcp_LPduIdRouteConfType, XCP_CONST )  Xcp_CanIfTxLPduIdRouteConf[XCP_NO_OF_CANIF_TX_PDU_IDS] =
{
[!LOOP "node:order(XcpPdu/*[@name='XcpTxPdu'], 'XcpTxPduId')"!][!//
  [!CALL "GetSymbolName","ShortNameRef"="'.'","OldName"="name(.)"!]
  [!VAR "TxPduName" = "node:ref(XcpTxPduRef)/@name"!]
  [!VAR "TxPduId" = "XcpTxPduId"!]
  [!VAR "TxPduCddTriggerTransmit" = "'false'"!]
  [!SELECT "../../XcpConnectionCfg/*[count(XcpTxPduConnectionInfo/*[node:refvalid(XcpMapTxPdu2Connection) and (node:ref(XcpMapTxPdu2Connection)/XcpTxPduId = $TxPduId)]) > 0]"!]
    [!IF "(XcpConnectionInterfaceType = 'XcpConnectionOverCAN')or(XcpConnectionInterfaceType = 'XcpConnectionOverCANFD')"!]
      [!CALL "GetTxPduInfo","ConnectionPath"="node:path(.)","TxPduName"="$TxPduName","TxPduId"="$TxPduId", "TxPduCddTriggerTransmit"="$TxPduCddTriggerTransmit"!]
  {
    [!"num:i($ComIfPduDestId)"!]U, /* CanIf handle Id */
    [!"num:i($TxPduId)"!]U, /* Xcp internal handle Id */
  },
    [!ENDIF!]
  [!ENDSELECT!]
[!ENDLOOP!]
};
[!ENDIF!]

[!IF "../../XcpGeneral/XcpOnFlexRayEnabled"!]
/** \brief Routing map for the incoming FrIf Pdus. */
CONST(Xcp_LPduIdRouteConfType, XCP_CONST )  Xcp_FrIfRxLPduIdRouteConf[XCP_NO_OF_FRIF_RX_PDU_IDS] =
{
[!LOOP "node:order(XcpPdu/*[@name='XcpRxPdu'], 'XcpRxPduId')"!][!//
  [!VAR "RxPduId" = "XcpRxPduId"!]
  [!SELECT "../../XcpConnectionCfg/*[count(XcpRxPduConnectionInfo/*[node:refvalid(XcpMapRxPdu2Connection) and (node:ref(XcpMapRxPdu2Connection)/XcpRxPduId = $RxPduId)]) > 0]"!]
    [!IF "XcpConnectionInterfaceType = 'XcpConnectionOverFlexRay'"!]
      [!SELECT "XcpRxPduConnectionInfo/*[node:refvalid(XcpMapRxPdu2Connection) and (node:ref(XcpMapRxPdu2Connection)/XcpRxPduId = $RxPduId)]"!]
  {
    [!"num:i(XcpRxLowerLayerHandleId)"!]U, /* FrIf handle Id */
    [!"num:i($RxPduId)"!]U, /* Xcp internal handle Id */
  },
      [!ENDSELECT!]
    [!ENDIF!]
  [!ENDSELECT!]
[!ENDLOOP!]
};

/** \brief Routing map for the outgoing FrIf Pdus. */
CONST(Xcp_LPduIdRouteConfType, XCP_CONST )  Xcp_FrIfTxLPduIdRouteConf[XCP_NO_OF_FRIF_TX_PDU_IDS] =
{
[!LOOP "node:order(XcpPdu/*[@name='XcpTxPdu'], 'XcpTxPduId')"!][!//
  [!CALL "GetSymbolName","ShortNameRef"="'.'","OldName"="name(.)"!]
  [!VAR "TxPduName" = "node:ref(XcpTxPduRef)/@name"!]
  [!VAR "TxPduId" = "XcpTxPduId"!]
  [!VAR "TxPduCddTriggerTransmit" = "'false'"!]
  [!SELECT "../../XcpConnectionCfg/*[count(XcpTxPduConnectionInfo/*[node:refvalid(XcpMapTxPdu2Connection) and (node:ref(XcpMapTxPdu2Connection)/XcpTxPduId = $TxPduId)]) > 0]"!]
    [!IF "XcpConnectionInterfaceType = 'XcpConnectionOverFlexRay'"!]
      [!CALL "GetTxPduInfo","ConnectionPath"="node:path(.)","TxPduName"="$TxPduName","TxPduId"="$TxPduId", "TxPduCddTriggerTransmit"="$TxPduCddTriggerTransmit"!]
  {
    [!"num:i($ComIfPduDestId)"!]U, /* FrIf handle Id */
    [!"num:i($TxPduId)"!]U, /* Xcp internal handle Id */
  },
    [!ENDIF!]
  [!ENDSELECT!]
[!ENDLOOP!]
};
[!ENDIF!]

[!IF "../../XcpGeneral/XcpOnEthernetEnabled"!]
/** \brief Routing map for the incoming SoAd Pdus. */
CONST(Xcp_LPduIdRouteConfType, XCP_CONST )  Xcp_SoAdRxLPduIdRouteConf[XCP_NO_OF_SOAD_RX_PDU_IDS] =
{
[!LOOP "node:order(XcpPdu/*[@name='XcpRxPdu'], 'XcpRxPduId')"!][!//
  [!VAR "RxPduId" = "XcpRxPduId"!]
  [!SELECT "../../XcpConnectionCfg/*[count(XcpRxPduConnectionInfo/*[node:refvalid(XcpMapRxPdu2Connection) and (node:ref(XcpMapRxPdu2Connection)/XcpRxPduId = $RxPduId)]) > 0]"!]
    [!IF "XcpConnectionInterfaceType = 'XcpConnectionOverEthernet'"!]
      [!SELECT "XcpRxPduConnectionInfo/*[node:refvalid(XcpMapRxPdu2Connection) and (node:ref(XcpMapRxPdu2Connection)/XcpRxPduId = $RxPduId)]"!]
  {
    [!"num:i(XcpRxLowerLayerHandleId)"!]U, /* SoAd handle Id */
    [!"num:i($RxPduId)"!]U, /* Xcp internal handle Id */
  },
      [!ENDSELECT!]
    [!ENDIF!]
  [!ENDSELECT!]
[!ENDLOOP!]
};

/** \brief Routing map for the outgoing SoAd Pdus. */
CONST(Xcp_LPduIdRouteConfType, XCP_CONST )  Xcp_SoAdTxLPduIdRouteConf[XCP_NO_OF_SOAD_TX_PDU_IDS] =
{
[!LOOP "node:order(XcpPdu/*[@name='XcpTxPdu'], 'XcpTxPduId')"!][!//
  [!CALL "GetSymbolName","ShortNameRef"="'.'","OldName"="name(.)"!]
  [!VAR "TxPduName" = "node:ref(XcpTxPduRef)/@name"!]
  [!VAR "TxPduId" = "XcpTxPduId"!]
  [!VAR "TxPduCddTriggerTransmit" = "'false'"!]
  [!SELECT "../../XcpConnectionCfg/*[count(XcpTxPduConnectionInfo/*[node:refvalid(XcpMapTxPdu2Connection) and (node:ref(XcpMapTxPdu2Connection)/XcpTxPduId = $TxPduId)]) > 0]"!]
    [!IF "XcpConnectionInterfaceType = 'XcpConnectionOverEthernet'"!]
      [!CALL "GetTxPduInfo","ConnectionPath"="node:path(.)","TxPduName"="$TxPduName","TxPduId"="$TxPduId", "TxPduCddTriggerTransmit"="$TxPduCddTriggerTransmit"!]
  {
    [!"num:i($ComIfPduDestId)"!]U, /* SoAd handle Id */
    [!"num:i($TxPduId)"!]U, /* Xcp internal handle Id */
  },
    [!ENDIF!]
  [!ENDSELECT!]
[!ENDLOOP!]
};
[!ENDIF!]

[!IF "../../XcpGeneral/XcpOnCddEnabled"!]
/** \brief Routing map for the incoming Cdd Pdus. */
CONST(Xcp_LPduIdRouteConfType, XCP_CONST )  Xcp_CddRxLPduIdRouteConf[XCP_NO_OF_CDD_RX_PDU_IDS] =
{
[!LOOP "node:order(XcpPdu/*[@name='XcpRxPdu'], 'XcpRxPduId')"!][!//
  [!VAR "RxPduId" = "XcpRxPduId"!]
  [!SELECT "../../XcpConnectionCfg/*[count(XcpRxPduConnectionInfo/*[node:refvalid(XcpMapRxPdu2Connection) and (node:ref(XcpMapRxPdu2Connection)/XcpRxPduId = $RxPduId)]) > 0]"!]
    [!IF "XcpConnectionInterfaceType = 'XcpConnectionOverCDD'"!]
      [!SELECT "XcpRxPduConnectionInfo/*[node:refvalid(XcpMapRxPdu2Connection) and (node:ref(XcpMapRxPdu2Connection)/XcpRxPduId = $RxPduId)]"!]
  {
    [!"num:i(XcpRxLowerLayerHandleId)"!]U, /* Cdd handle Id */
    [!"num:i($RxPduId)"!]U, /* Xcp internal handle Id */
  },
      [!ENDSELECT!]
    [!ENDIF!]
  [!ENDSELECT!]
[!ENDLOOP!]
};

/** \brief Routing map for the outgoing Cdd Pdus. */
CONST(Xcp_LPduIdRouteConfType, XCP_CONST )  Xcp_CddTxLPduIdRouteConf[XCP_NO_OF_CDD_TX_PDU_IDS] =
{
[!LOOP "node:order(XcpPdu/*[@name='XcpTxPdu'], 'XcpTxPduId')"!][!//
  [!CALL "GetSymbolName","ShortNameRef"="'.'","OldName"="name(.)"!]
  [!VAR "TxPduName" = "node:ref(XcpTxPduRef)/@name"!]
  [!VAR "TxPduId" = "XcpTxPduId"!]
  [!IF "node:exists(XcpTxPduSupportForCddTriggerTransmit) and node:value(XcpTxPduSupportForCddTriggerTransmit) = 'true'"!]
    [!VAR "TxPduCddTriggerTransmit" = "'true'"!]
  [!ELSE!]
    [!VAR "TxPduCddTriggerTransmit" = "'false'"!]
  [!ENDIF!]
  [!LOOP "../../XcpConnectionCfg/*/XcpTxPduConnectionInfo/*[../../XcpConnectionInterfaceType = 'XcpConnectionOverCDD' and node:refvalid(XcpMapTxPdu2Connection) and (node:ref(XcpMapTxPdu2Connection)/XcpTxPduId = $TxPduId)]"!]
    [!CALL "GetTxPduInfo","ConnectionPath"="node:path(../..)","TxPduName"="$TxPduName","TxPduId"="$TxPduId", "TxPduCddTriggerTransmit"="$TxPduCddTriggerTransmit","TxPduConnectionInfoPath"="node:path(.)"!]
  {
    [!"num:i($ComIfPduDestId)"!]U, /* Cdd handle Id */
    [!"num:i($TxPduId)"!]U, /* Xcp internal handle Id */
  },
  [!ENDLOOP!]
[!ENDLOOP!]
};
[!ENDIF!]

[!ENDIF!]

/*----------------------------TxPduInfo--------------------------------------*/

/** \brief Tx Pdu Ids configured by the transport layer.
 **
 ** This Ids are used store information regarding:
 ** * mapping PDU toward connection
 ** * destination PDU id into transport layer configuration
 ** * start position of the PDU data inside the Tx PDU buffer area
 ** * supported Xcp communication transfer object types. */
CONST(Xcp_TxLPduIdConfType, XCP_CONST )  Xcp_TxLPduIdConf[XCP_NO_OF_TX_PDU_IDS] =
{
[!LOOP "node:order(XcpPdu/*[@name='XcpTxPdu'], 'XcpTxPduId')"!][!//
  [!CALL "GetSymbolName","ShortNameRef"="'.'","OldName"="name(.)"!]
  [!VAR "TxPduName" = "node:ref(XcpTxPduRef)/@name"!]
  [!VAR "TxPduId" = "XcpTxPduId"!]
  [!IF "node:exists(XcpTxPduSupportForCddTriggerTransmit) and node:value(XcpTxPduSupportForCddTriggerTransmit) = 'true'"!]
    [!VAR "TxPduCddTriggerTransmit" = "'true'"!]
  [!ELSE!]
    [!VAR "TxPduCddTriggerTransmit" = "'false'"!]
  [!ENDIF!]
  [!LOOP "../../XcpConnectionCfg/*/XcpTxPduConnectionInfo/*[node:refvalid(XcpMapTxPdu2Connection) and (node:ref(XcpMapTxPdu2Connection)/XcpTxPduId = $TxPduId)]"!][!//
    [!CALL "GetTxPduInfo","ConnectionPath"="node:path(../..)","TxPduName"="$TxPduName","TxPduId"="$TxPduId", "TxPduCddTriggerTransmit"="$TxPduCddTriggerTransmit","TxPduConnectionInfoPath"="node:path(.)"!]
  { /* Mapping information for [!"$SymbolName"!] Pdu. */
    [!IF "(count(../../../*[XcpConnectionInterfaceType = 'XcpConnectionOverCAN']) > 0) or (count(../../../*[XcpConnectionInterfaceType = 'XcpConnectionOverCANFD']) > 0)"!]
      [!IF "$CanTxId != ''"!]
    [!"num:i($CanTxId)"!]U,  /* CAN/CANFD ID of the package */
      [!ELSE!]
    XCP_IGNORED_CAN_ID,
      [!ENDIF!]
    [!ENDIF!]
    [!"num:i(num:mul($XcpPackagePduSize,$PduMapIndex))"!]U,  /* LPdu data start position into Tx buffer data */
    [!"num:i($ComIfPduDestId)"!]U,  /* [!"$ComIf"!] destination PDU id */
    [!"num:i($PduMapIndex)"!]U,  /* associated internal buffer Id */
    [!"../../XcpConnectionId"!]U,  /* Xcp connection Id([!"@name"!]) */
    [!CALL "PrintPropertyFlags","PropertyList"="$TxFlags","CommentString"="'Properties and supported types for this PDU'","Indentation"="4"!]
  },
  [!ENDLOOP!]
[!ENDLOOP!]
};

/*----------------------------RxPduInfo--------------------------------------*/

/** \brief Rx Pdu Ids configured by the transport layer.
 **
 ** This Ids are used store information regarding:
 ** * mapping PDU toward connection
 ** * start position of the PDU data inside the Rx PDU buffer area
 ** * supported Xcp communication transfer object types. */
CONST(Xcp_RxLPduIdConfType, XCP_CONST )  Xcp_RxLPduIdConf[XCP_NO_OF_RX_PDU_IDS] =
{
[!LOOP "node:order(XcpPdu/*[@name='XcpRxPdu'], 'XcpRxPduId')"!][!//
  [!CALL "GetSymbolName","ShortNameRef"="'.'","OldName"="name(.)"!]
  [!VAR "RxPduId" = "XcpRxPduId"!]
  [!VAR "RxPduName" = "node:ref(XcpRxPduRef)/@name"!]
  [!SELECT "../../XcpConnectionCfg/*[count(XcpRxPduConnectionInfo/*[node:refvalid(XcpMapRxPdu2Connection) and (node:ref(XcpMapRxPdu2Connection)/XcpRxPduId = $RxPduId)]) > 0]"!]
    [!CALL "GetRxPduInfo","ConnectionPath"="node:path(.)","RxPduId"="$RxPduId","RxPduName"="$RxPduName"!]
  { /* Mapping information for [!"$SymbolName"!] Pdu. */
    [!IF "(count(../../XcpConnectionCfg/*[XcpConnectionInterfaceType = 'XcpConnectionOverCAN']) > 0) or (count(../../XcpConnectionCfg/*[XcpConnectionInterfaceType = 'XcpConnectionOverCANFD']) > 0)"!]
      [!IF "$CanRxId != ''"!]
    [!"num:i($CanRxId)"!]U,  /* CAN ID of the package */
      [!ELSE!]
    XCP_IGNORED_CAN_ID,
      [!ENDIF!]
    [!ENDIF!]
    [!"num:i(num:mul($XcpPackagePduSize,$PduMapIndex))"!]U,  /* LPdu data start position into Rx buffer data */
    [!"num:i($PduMapIndex)"!]U,  /* associated internal buffer Id */
    [!"XcpConnectionId"!]U,  /* Xcp connection Id([!"@name"!]) */
    [!CALL "PrintPropertyFlags","PropertyList"="$RxFlags","CommentString"="'Properties and supported types for this PDU'","Indentation"="4"!]
  },
  [!ENDSELECT!]
[!ENDLOOP!]
};

/*----------------------------XcpConnectionCfg--------------------------------------*/
[!IF "$SuportForSoAdSocketProtocolTcp = 'true'"!]
/** \brief List used to map TCP/IP socket connections towards Xcp connection Id. */
CONST(Xcp_TcpIpSocketConMapType, XCP_CONST)
      Xcp_TcpIpSocketConMapCfg[XCP_NO_OF_TCPIP_SOCKET_CON] =
{
  [!LOOP "node:order(XcpConnectionCfg/*[XcpConnectionInterfaceType = 'XcpConnectionOverEthernet'],'XcpConnectionId')"!]
    [!VAR "SelectedConnectionId" = "XcpConnectionId"!]
    [!VAR "SelectedConnectionName"="@name"!]
    [!VAR "TxPduName"="node:ref(node:ref(XcpTxPduConnectionInfo/*[1]/XcpMapTxPdu2Connection)/XcpTxPduRef)/@name"!]
    [!IF "node:refvalid(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)"!]
      [!IF "node:exists(as:modconf('SoAd')/SoAdConfig/*/SoAdPduRoute/*/SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)"!]
        [!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)/SoAdPduRoute/*[node:refvalid(SoAdTxPduRef) and (node:ref(SoAdTxPduRef)/@name = $TxPduName) and node:refvalid(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef) and (node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)/../../SoAdSocketProtocol = 'SoAdSocketTcp')]"!]
          [!SELECT "node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)"!]
            [!VAR "SoAdConnectionName" = "concat(../../@name,'_',@name)"!]
  {
    [!"SoAdSocketId"!]U, /* Id of the TcpIp socket connection
               ([!"$SoAdConnectionName"!]). */
    [!"$SelectedConnectionId"!]U, /* Xcp connection Id([!"$SelectedConnectionName"!]) */
  },
         [!ENDSELECT!]
       [!ENDSELECT!]
      [!ELSEIF "node:exists(as:modconf('SoAd')/SoAdConfig/*/SoAdPduRoute/*/SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)"!]
        [!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)/SoAdPduRoute/*[node:refvalid(SoAdTxPduRef) and (node:ref(SoAdTxPduRef)/@name = $TxPduName) and node:refvalid(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef) and (node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)/../../SoAdSocketProtocol = 'SoAdSocketTcp')]"!]
          [!SELECT "node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)"!]
            [!VAR "SoAdConnectionName" = "concat(../../@name,'_',@name)"!]
  {
    [!"SoAdSocketId"!]U, /* Id of the TcpIp socket connection
               ([!"$SoAdConnectionName"!]). */
    [!"$SelectedConnectionId"!]U, /* Xcp connection Id([!"$SelectedConnectionName"!]) */
  },
         [!ENDSELECT!]
       [!ENDSELECT!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!]
};
[!ENDIF!]

/** \brief Connection configuration info */
CONST(Xcp_ConnectionCfgType, XCP_CONST) Xcp_ConnectionCfg[XCP_NO_OF_CONNECTIONS] =
{
[!VAR "TxPduIndex" = "0"!]
[!VAR "PacketCounterId" = "0"!]
[!LOOP "node:order(XcpConnectionCfg/*,'XcpConnectionId')"!]
  [!VAR "ConnectionType"="''"!]
  [!VAR "ConnectionHasCounter"="'false'"!]
  [!VAR "TxPduName"="node:ref(node:ref(XcpTxPduConnectionInfo/*[1]/XcpMapTxPdu2Connection)/XcpTxPduRef)/@name"!]
  [!VAR "RxPduName"="node:ref(node:ref(XcpRxPduConnectionInfo/*[1]/XcpMapRxPdu2Connection)/XcpRxPduRef)/@name"!]
  [!IF "XcpConnectionInterfaceType = 'XcpConnectionOverCAN'"!]
    [!VAR "ConnectionType"="'CAN'"!]
    [!VAR "TransmitFunction"="'&CanIf_Transmit'"!]
  [!ELSEIF "XcpConnectionInterfaceType = 'XcpConnectionOverCANFD'"!]
    [!VAR "ConnectionType"="'CANFD'"!]
    [!VAR "TransmitFunction"="'&CanIf_Transmit'"!]
  [!ELSEIF "XcpConnectionInterfaceType = 'XcpConnectionOverFlexRay'"!]
    [!VAR "ConnectionType"="'FLEXRAY'"!]
    [!VAR "TransmitFunction"="'&FrIf_Transmit'"!]
  [!ELSEIF "XcpConnectionInterfaceType = 'XcpConnectionOverEthernet'"!]
    [!IF "node:exists(as:modconf('SoAd')/SoAdConfig/*/SoAdPduRoute/*/SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)"!]
      [!IF "count(node:ref(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)/SoAdPduRoute/*[node:refvalid(SoAdTxPduRef) and (node:ref(SoAdTxPduRef)/@name = $TxPduName) and node:refvalid(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef) and (node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)/../../SoAdSocketProtocol = 'SoAdSocketTcp')]) > 0"!]
        [!VAR "ConnectionType"="'TCPIP'"!] /* TCP */
      [!ELSEIF "count(node:ref(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)/SoAdPduRoute/*[node:refvalid(SoAdTxPduRef) and (node:ref(SoAdTxPduRef)/@name = $TxPduName) and node:refvalid(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef) and (node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)/../../SoAdSocketProtocol = 'SoAdSocketUdp')]) > 0"!]
        [!VAR "ConnectionType"="'UDPIP'"!] /* UDP */
      [!ENDIF!]
    [!ELSEIF "node:exists(as:modconf('SoAd')/SoAdConfig/*/SoAdPduRoute/*/SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)"!]
      [!IF "count(node:ref(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)/SoAdPduRoute/*[node:refvalid(SoAdTxPduRef) and (node:ref(SoAdTxPduRef)/@name = $TxPduName) and node:refvalid(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef) and (node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)/../../SoAdSocketProtocol = 'SoAdSocketTcp')]) > 0"!]
        [!VAR "ConnectionType"="'TCPIP'"!]
      [!ELSEIF "count(node:ref(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)/SoAdPduRoute/*[node:refvalid(SoAdTxPduRef) and (node:ref(SoAdTxPduRef)/@name = $TxPduName) and node:refvalid(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef) and (node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)/../../SoAdSocketProtocol = 'SoAdSocketUdp')]) > 0"!]
        [!VAR "ConnectionType"="'UDPIP'"!]
      [!ENDIF!]
    [!ENDIF!]
    [!VAR "TransmitFunction"="'&SoAd_IfTransmit'"!]
  [!ELSEIF "XcpConnectionInterfaceType = 'XcpConnectionOverCDD'"!]
    [!VAR "ConnectionType"="'CDD'"!]
    [!VAR "TransmitFunction"="concat('&',XcpConnectionInterfaceType/XcpCddInformation/XcpCddTransmitFunctionName)"!]
  [!ENDIF!]
  [!CALL "GetRxPduInfo","ConnectionPath"="node:path(.)","RxPduId"="node:ref(XcpRxPduConnectionInfo/*[1]/XcpMapRxPdu2Connection)/XcpRxPduId","RxPduName"="node:ref(node:ref(XcpRxPduConnectionInfo/*[1]/XcpMapRxPdu2Connection)/XcpRxPduRef)/@name"!]
  [!VAR "ConnectionFlags"="''"!]
  [!IF "$PackMultiMsgInOneFrame = 'true'"!]
    [!VAR "ConnectionFlags"="concat($ConnectionFlags,'XCP_MULTI_PDUS_IN_FRAME_FLAG',' ')"!]
  [!ENDIF!]
  [!IF "$OpenSoAdCon = 'true'"!]
    [!VAR "ConnectionFlags"="concat($ConnectionFlags,'XCP_AUTO_OPEN_SOCON_FLAG',' ')"!]
  [!ENDIF!]
  [!IF "$SequenceCorrection = 'true'"!]
    [!VAR "ConnectionFlags"="concat($ConnectionFlags,'XCP_SEQUENCE_CORRECTION_FLAG',' ')"!]
  [!ENDIF!]
  [!IF "$MaxDlcRequired = 'true'"!]
    [!VAR "ConnectionFlags"="concat($ConnectionFlags,'XCP_MAX_DLC_REQUIRED',' ')"!]
  [!ENDIF!]
  /* configuration information for [!"@name"!] connection(Xcp over [!"$ConnectionType"!]). */
  {
    &Xcp_TxPduIdCfgList[[!"num:i($TxPduIndex)"!]U], /* start position for the mapped Tx Pdu list */
    [!"$TransmitFunction"!], /* pointer to transmit function */
  [!IF "XcpConnectionInterfaceType = 'XcpConnectionOverCANFD' and XcpConnectionInterfaceType/XcpCanFdMaxDlcRequired = 'true'"!]
    [!"XcpConnectionInterfaceType/XcpCanFdMaxDlc"!]U, /* PDU maximum length */
  [!ELSE!]
    [!"num:i($XcpPackagePduSize)"!]U, /* PDU maximum length */
  [!ENDIF!]
    [!"num:i($XcpPackageHeaderSize)"!]U, /* PDU header size */
    [!"num:i(count(XcpTxPduConnectionInfo/*))"!]U, /* number of mapped Tx PDUs towards connection */
    [!"num:i(count(XcpRxPduConnectionInfo/*))"!]U, /* number of mapped Rx PDUs towards connection */
  [!IF "(count(../../XcpConnectionCfg/*[XcpConnectionInterfaceType = 'XcpConnectionOverCAN']) > 0) or (count(../../XcpConnectionCfg/*[XcpConnectionInterfaceType = 'XcpConnectionOverCANFD']) > 0) or (count(../../XcpConnectionCfg/*[XcpConnectionInterfaceType = 'XcpConnectionOverCDD']) > 0) "!]
    [!IF "XcpConnectionInterfaceType = 'XcpConnectionOverCAN'"!]
    [!"node:ref(node:ref(XcpConnectionInterfaceType/XcpCtoSlavePduRef)/XcpMapRxPdu2Connection)/XcpRxPduId"!]U, /* Xcp PDU channel, used to receive all CMD's but GET_SLAVE_ID. */
    [!"node:ref(node:ref(XcpConnectionInterfaceType/XcpBroadcastPduRef)/XcpMapRxPdu2Connection)/XcpRxPduId"!]U, /* Xcp PDU channel, used to receive the broadcasted GET_SLAVE_ID CMD. */
    [!ELSEIF "XcpConnectionInterfaceType = 'XcpConnectionOverCANFD'"!]
    [!"node:ref(node:ref(XcpConnectionInterfaceType/XcpCanFdCtoSlavePduRef)/XcpMapRxPdu2Connection)/XcpRxPduId"!]U, /* Xcp PDU channel, used to receive all CMD's but GET_SLAVE_ID. */
    [!"node:ref(node:ref(XcpConnectionInterfaceType/XcpCanFdBroadcastPduRef)/XcpMapRxPdu2Connection)/XcpRxPduId"!]U, /* Xcp PDU channel, used to receive the broadcasted GET_SLAVE_ID CMD. */
    [!ELSEIF "XcpConnectionInterfaceType = 'XcpConnectionOverCDD'"!]
    [!"node:ref(node:ref(XcpConnectionInterfaceType/XcpCddCtoSlavePduRef)/XcpMapRxPdu2Connection)/XcpRxPduId"!]U, /* Xcp PDU channel, used to receive all CMD's but GET_SLAVE_ID. */
    [!"node:ref(node:ref(XcpConnectionInterfaceType/XcpCddBroadcastPduRef)/XcpMapRxPdu2Connection)/XcpRxPduId"!]U, /* Xcp PDU channel, used to receive the broadcasted GET_SLAVE_ID CMD. */
    [!ELSE!]
    (PduIdType)XCP_IGNORED_PDU_ID, /* Xcp PDU channel, used to receive all CMD's but GET_SLAVE_ID. */
    (PduIdType)XCP_IGNORED_PDU_ID, /* Xcp PDU channel, used to receive the broadcasted GET_SLAVE_ID CMD. */
    [!ENDIF!]
    [!IF "XcpConnectionInterfaceType = 'XcpConnectionOverCAN' or XcpConnectionInterfaceType = 'XcpConnectionOverCANFD' or XcpConnectionInterfaceType = 'XcpConnectionOverCDD'"!]
      [!IF "(../../../../XcpGeneral/XcpStimSupported = 'true') and (../../../../XcpGeneral/XcpSupportForGetDaqId = 'true')"!]
    [!"node:ref(XcpRxPduConnectionInfo/*[XcpPduSupportForStimDtoType = 'true']/XcpMapRxPdu2Connection)/XcpRxPduId"!]U, /* Xcp PDU channel, used to receive STIM DTOs. */
      [!ENDIF!]
    [!ELSE!]
      [!IF "(../../../../XcpGeneral/XcpStimSupported = 'true') and (../../../../XcpGeneral/XcpSupportForGetDaqId = 'true')"!]
    (PduIdType)XCP_IGNORED_PDU_ID, /* Xcp PDU channel, used to receive STIM DTOs. */
      [!ENDIF!]
    [!ENDIF!]
    [!IF "XcpConnectionInterfaceType = 'XcpConnectionOverCAN' or XcpConnectionInterfaceType = 'XcpConnectionOverCANFD' or XcpConnectionInterfaceType = 'XcpConnectionOverCDD'"!]
      [!IF "(../../../../XcpGeneral/XcpDaqSupported = 'true') and (../../../../XcpGeneral/XcpSupportForGetDaqId = 'true')"!]
    [!"node:ref(XcpTxPduConnectionInfo/*[XcpPduSupportForDaqDtoType = 'true']/XcpMapTxPdu2Connection)/XcpTxPduId"!]U, /* Xcp PDU channel, used to transmit Daq DTOs. */
      [!ENDIF!]
    [!ELSE!]
      [!IF "(../../../../XcpGeneral/XcpDaqSupported = 'true') and (../../../../XcpGeneral/XcpSupportForGetDaqId = 'true')"!]
    (PduIdType)XCP_IGNORED_PDU_ID, /* Xcp PDU channel, used to transmit Daq DTOs. */
      [!ENDIF!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "count(../../XcpConnectionCfg/*[XcpConnectionInterfaceType = 'XcpConnectionOverEthernet']) > 0"!]
    [!IF "XcpConnectionInterfaceType = 'XcpConnectionOverEthernet'"!]
      [!IF "node:exists(as:modconf('SoAd')/SoAdConfig/*/SoAdPduRoute/*/SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)"!]
        [!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)/SoAdPduRoute/*[node:refvalid(SoAdTxPduRef) and (node:ref(SoAdTxPduRef)/@name = $TxPduName) and (count(SoAdPduRouteDest/*) > 0)]"!]
          [!IF "not(node:refvalid(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)"!]
            [!ERROR "Invalid value for node SoAdTxSocketConnectionRef: Invalid reference."!]
          [!ENDIF!]
          [!VAR "SoAdConnectionName" = "node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)/@name"!]
          [!VAR "SoAdConnectionGroupName" = "node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)/../../@name"!]
    [!"node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)/SoAdSocketId"!]U, /* Tx socket connection Id([!"$SoAdConnectionGroupName"!]_[!"$SoAdConnectionName"!]) */
        [!ENDSELECT!]
      [!ELSEIF "node:exists(as:modconf('SoAd')/SoAdConfig/*/SoAdPduRoute/*/SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)"!]
        [!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)/SoAdPduRoute/*[node:refvalid(SoAdTxPduRef) and (node:ref(SoAdTxPduRef)/@name = $TxPduName) and (count(SoAdPduRouteDest/*) > 0)]"!]
          [!IF "not(node:refvalid(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef))"!]
            /* !LINKSTO Xcp.Configuration.XcpConfig.XcpConnectionCfg.XcpConnectionInterfaceType.XcpConnectionOverEthernet.XcpConnectionSoAdConfigRef.SoAdTxSocketConnOrSocketConnBundleRef,1 */
            [!ERROR "Invalid value for node SoAdTxSocketConnOrSocketConnBundleRef: Invalid reference."!]
          [!ENDIF!]
          [!VAR "SoAdConnectionName" = "node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)/@name"!]
          [!VAR "SoAdConnectionGroupName" = "node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)/../../@name"!]
    [!"node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)/SoAdSocketId"!]U, /* Tx socket connection Id([!"$SoAdConnectionGroupName"!]_[!"$SoAdConnectionName"!]) */
        [!ENDSELECT!]
      [!ENDIF!]
      [!IF "$SuportForSoAdSocketProtocolUdp = 'true'"!]
        [!IF "node:exists(as:modconf('SoAd')/SoAdConfig/*/SoAdSocketRoute/*/SoAdRxSocketConnectionRef)"!]
          [!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)/SoAdSocketRoute/*[(count(SoAdSocketRouteDest/*) > 0) and node:refvalid(SoAdSocketRouteDest/*/SoAdRxPduRef) and (node:ref(SoAdSocketRouteDest/*/SoAdRxPduRef)/@name = $RxPduName)]"!]
            [!IF "not(node:refvalid(SoAdRxSocketConnectionRef))"!]
              [!ERROR "Invalid value for node SoAdRxSocketConnectionRef: Invalid reference."!]
            [!ENDIF!]
            [!VAR "SoAdConnectionName" = "node:ref(SoAdRxSocketConnectionRef)/@name"!]
            [!VAR "SoAdConnectionGroupName" = "node:ref(SoAdRxSocketConnectionRef)/../../@name"!]
    [!"node:ref(SoAdRxSocketConnectionRef)/SoAdSocketId"!]U, /* Rx socket connection Id([!"$SoAdConnectionGroupName"!]_[!"$SoAdConnectionName"!]) */
          [!ENDSELECT!]
        [!ELSEIF "node:exists(as:modconf('SoAd')/SoAdConfig/*/SoAdSocketRoute/*/SoAdRxSocketConnOrSocketConnBundleRef)"!]
          [!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)/SoAdSocketRoute/*[(count(SoAdSocketRouteDest/*) > 0) and node:refvalid(SoAdSocketRouteDest/*/SoAdRxPduRef) and (node:ref(SoAdSocketRouteDest/*/SoAdRxPduRef)/@name = $RxPduName)]"!]
            [!IF "not(node:refvalid(SoAdRxSocketConnOrSocketConnBundleRef))"!]
              /* !LINKSTO Xcp.Configuration.XcpConfig.XcpConnectionCfg.XcpConnectionInterfaceType.XcpConnectionOverEthernet.XcpConnectionSoAdConfigRef.SoAdRxSocketConnOrSocketConnBundleRef,1 */
              [!ERROR "Invalid value for node SoAdRxSocketConnOrSocketConnBundleRef: Invalid reference."!]
            [!ENDIF!]
            [!VAR "SoAdConnectionName" = "node:ref(SoAdRxSocketConnOrSocketConnBundleRef)/@name"!]
            [!VAR "SoAdConnectionGroupName" = "node:ref(SoAdRxSocketConnOrSocketConnBundleRef)/../../@name"!]
    [!"node:ref(SoAdRxSocketConnOrSocketConnBundleRef)/SoAdSocketId"!]U, /* Rx socket connection Id([!"$SoAdConnectionGroupName"!]_[!"$SoAdConnectionName"!]) */
          [!ENDSELECT!]
        [!ENDIF!]
      [!ENDIF!]
    [!ELSE!]
    (SoAd_SoConIdType)XCP_IGNORED_SOCON_ID,
      [!IF "$SuportForSoAdSocketProtocolUdp = 'true'"!]
    (SoAd_SoConIdType)XCP_IGNORED_SOCON_ID,
      [!ENDIF!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "count(../../XcpConnectionCfg/*[XcpConnectionInterfaceType = 'XcpConnectionOverEthernet']) > 0"!]
    [!IF "XcpConnectionInterfaceType = 'XcpConnectionOverEthernet'"!]
      [!VAR "ConnectionHasCounter" = "'true'"!]
    [!ENDIF!]
  [!ENDIF!]   
  [!IF "count(../../XcpConnectionCfg/*[XcpConnectionInterfaceType = 'XcpConnectionOverFlexRay']) > 0"!]
    [!IF "(XcpConnectionInterfaceType = 'XcpConnectionOverFlexRay') and ($SequenceCorrection = 'true')"!]
      [!VAR "ConnectionHasCounter" = "'true'"!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "$ConnectionHasCounter = 'true'"!]
    [!"num:i($PacketCounterId)"!]U, /* Id of the packet counter */
    [!VAR "PacketCounterId" = "$PacketCounterId + 1"!]
  [!ELSE!]
    XCP_IGNORED_CNT_ID,  
  [!ENDIF!]
  [!IF "count(../../XcpConnectionCfg/*[XcpConnectionInterfaceType = 'XcpConnectionOverCANFD']) > 0"!]
    [!IF "XcpConnectionInterfaceType = 'XcpConnectionOverCANFD'"!]
    [!"XcpConnectionInterfaceType/XcpCanFdMaxDlc"!]U,  /* MAX_DLC value */
    [!"XcpConnectionInterfaceType/XcpCanFdFillValue"!]U,  /* Fill byte value */
    [!ELSE!]
    0U,  /* MAX_DLC Value */
    0U,  /* Fill byte value */
    [!ENDIF!]
  [!ENDIF!]
  [!IF "count(../../XcpConnectionCfg/*[XcpConnectionInterfaceType = 'XcpConnectionOverFlexRay']) > 0"!]
    [!IF "XcpConnectionInterfaceType = 'XcpConnectionOverFlexRay'"!]
    [!"XcpConnectionInterfaceType/XcpFlxNodeAddress"!]U, /* configured NAX address for the slave */
    [!"num:i($XcpPackageAlignment)"!]U,  /* Package Alignment */
    [!ELSE!]
    XCP_IGNORED_NAX_ID,
    XCP_IGNORED_PACKAGE_ALIGNMENT,
    [!ENDIF!]
  [!ENDIF!]
    [!CALL "PrintPropertyFlags","PropertyList"="$ConnectionFlags","CommentString"="'connection flags'","Indentation"="4"!]
    XCP_CONNECTION_OVER_[!"$ConnectionType"!], /* connection type */
    [!"XcpMaxCto"!]U, /* connection specific MAX_CTO */
    [!"XcpMaxDto"!]U, /* connection specific MAX_DTO */
[!IF "$XcpTimestampIsEnabled = 'true'"!]
    [!"text:toupper(XcpTimestampEnabled)"!], /* flag indicating if DTO timestamps are enabled for this connection */
[!ENDIF!]
[!IF "../../../../XcpGeneral/XcpMasterBlockModeSupported = 'true'"!]
    [!IF "../../../../XcpGeneral/XcpAddressGranularity = 'BYTE'"!]
      [!VAR "XcpMaxBs" = "num:integer(ceiling(255 div (XcpMaxCto - 2)))"!]
    [!ELSEIF "../../../../XcpGeneral/XcpAddressGranularity = 'WORD'"!]
      [!VAR "XcpMaxBs" = "num:integer(ceiling(255 div (floor((XcpMaxCto - 2) div 2 ))))"!]
    [!ELSE!]
      [!VAR "XcpMaxBs" = "num:integer(ceiling(255 div (floor((XcpMaxCto - 4) div 4 ))))"!]
    [!ENDIF!]
    [!IF "XcpMaxBS < $XcpMaxBs"!]
      [!VAR "XcpMaxBs" = "XcpMaxBS"!] [!/* Allow the user to configure a smaller value than the maximum possible value which would not result in a waste of resources */!]
    [!ENDIF!]
    [!"$XcpMaxBs"!]U, /* connection specific maximum block size */
[!ENDIF!]
[!IF "../../../../XcpGeneral/XcpPgmSupported = 'true'"!]
    [!"XcpMaxCtoPgm"!]U, /* connection specific MAX_CTO for flash programming */
[!IF "../../../../XcpGeneral/XcpMasterBlockModePgmSupported"!]
    [!IF "../../../../XcpGeneral/XcpAddressGranularity = 'BYTE'"!]
    [!VAR "XcpMaxBSPgmVar" = "num:integer(ceiling(255 div (XcpMaxCtoPgm - 2)))"!]
    [!ELSEIF "../../../../XcpGeneral/XcpAddressGranularity = 'WORD'"!]
    [!VAR "XcpMaxBSPgmVar" = "num:integer(ceiling(255 div (floor((XcpMaxCtoPgm - 2) div 2 ))))"!]
    [!ELSE!]
    [!VAR "XcpMaxBSPgmVar" = "num:integer(ceiling(255 div (floor((XcpMaxCtoPgm - 4) div 4 ))))"!]
    [!ENDIF!]
    [!//
    [!IF "XcpMaxBSPgm < $XcpMaxBSPgmVar"!]
    [!VAR "XcpMaxBSPgmVar" = "XcpMaxBSPgm"!] [!/* Allow the user to configure a smaller value than the maximum possible value which would not result in a waste of resources */!]
    [!ENDIF!]
    [!"$XcpMaxBSPgmVar"!]U, /* connection specific maximum block size for flash programming */
[!ENDIF!]
[!ENDIF!] /* XcpPgmSupported = 'true' */
[!IF "../../../../XcpGeneral/XcpDaqSupported = 'true'"!]
    [!IF "(XcpMaxDto - $XcpPidSize) < ../../../../XcpGeneral/XcpOdtEntrySizeDaq"!]
    [!"num:i(XcpMaxDto - $XcpPidSize)"!]U, /* connection specific maximum odt entry size for direction DAQ */
    [!ELSE!]
    [!"../../../../XcpGeneral/XcpOdtEntrySizeDaq"!]U, /* connection specific maximum odt entry size for direction DAQ */
    [!ENDIF!]
[!IF "../../../../XcpGeneral/XcpDaqConfigType = 'DAQ_DYNAMIC'"!]
    [!IF "(XcpMaxDto - $XcpPidSize) < ../../../../XcpGeneral/XcpOdtEntriesCount"!]
    [!"num:i(XcpMaxDto - $XcpPidSize)"!]U, /* connection specific maximum odt entries for dynamic DAQ */
    [!ELSE!]
    [!"../../../../XcpGeneral/XcpOdtEntriesCount"!]U, /* connection specific maximum odt entries for dynamic DAQ */
    [!ENDIF!]
[!IF "../../../../XcpGeneral/XcpTimestampType != 'NO_TIME_STAMP'"!]
    [!IF "(XcpMaxDto - $XcpPidSize - $XcpTimestampSize) < ../../../../XcpGeneral/XcpOdtEntriesCount"!]
    [!"num:i(XcpMaxDto - $XcpPidSize - $XcpTimestampSize)"!]U, /* connection specific maximum odt entries for dynamic DAQ with timestamps */
    [!ELSE!]
    [!"../../../../XcpGeneral/XcpOdtEntriesCount"!]U, /* connection specific maximum odt entries for dynamic DAQ with timestamps */
    [!ENDIF!]
[!ENDIF!] /* XcpTimestampType != 'NO_TIME_STAMP' */
[!ENDIF!] /* XcpDaqConfigType = 'DAQ_DYNAMIC' */
[!IF "../../../../XcpGeneral/XcpStimSupported = 'true'"!]
    [!IF "(XcpMaxDto - $XcpPidSize) < ../../../../XcpGeneral/XcpOdtEntrySizeStim"!]
    [!"num:i(XcpMaxDto - $XcpPidSize)"!]U /* connection specific maximum odt entry size for direction STIM */
    [!ELSE!]
    [!"../../../../XcpGeneral/XcpOdtEntrySizeStim"!]U /* connection specific maximum odt entry size for direction STIM */
    [!ENDIF!]
[!ENDIF!]
[!ENDIF!] /* XcpDaqSupported = 'true' */
  },
  [!VAR "TxPduIndex" = "$TxPduIndex + count(XcpTxPduConnectionInfo/*)"!]
[!ENDLOOP!]
};

[!VAR "XcpEventChannelMFEnabled"="(../../XcpGeneral/XcpEventTriggerMainFunc = 'true')"!]
[!VAR "EventChannelMFIndex" = "0"!]

[!IF "$XcpMaxEventChannel > 0"!]
[!VAR "Counter" = "0"!]
/** \brief Configured event properties used to restore after a session is started */
CONST(Xcp_EventInfoType, XCP_CONST) Xcp_EventInfo[XCP_MAX_EVENT_CHANNEL] =
{
[!LOOP "node:order(XcpEventChannel/*, 'XcpEventChannelNumber')"!]
  {
    /* The event channel cycle counter maximum value */
    [!IF "XcpEventChannelTimeCycle > 0"!]
    [!CALL "EVENT_CHANNEL_TIME_CYCLE_COUNTER_MAX", "time_unit" = "XcpEventChannelTimeUnit"!]U,
    [!ELSE!][!/*
    */!]    0U,
    [!ENDIF!]
    /* The name of the event */
    Xcp_EventName_[!"XcpEventChannelNumber"!],
    [!IF "count(XcpEventChannelTriggeredDaqListRef/*) > 0"!]
    /* Pointer to the list of DAQ IDs of this event */
    &Xcp_DaqIdListConst[[!"num:i($Counter)"!]],
    /* The number of DAQ Lists of this event */
    [!"num:i(count(XcpEventChannelTriggeredDaqListRef/*))"!]U,
    [!ELSE!]
    /* Pointer to the list of DAQ IDs of this event */
    NULL_PTR,
    /* The number of DAQ Lists of this event */
    0U,
    [!ENDIF!]
    /* The length of the event name */
    [!"num:i(string-length(name(.)))"!]U,
    /* Consistency and STIM/DAQ, both direction combine configured for this event */
    XCP_EVENT_CHANNEL_DIRECTION_[!"XcpEventChannelType"!] | XCP_EVENT_CHANNEL_CONSISTENCY_[!"XcpEventChannelConsistency"!],
    /* Maximum number of DAQ supported for this event */
    [!"XcpEventChannelMaxDaqList"!]U,
    /* The priority of this event */
    [!"XcpEventChannelPriority"!]U,
    /* The sampling period of this event */
    [!"XcpEventChannelTimeCycle"!]U,
    [!IF "XcpEventChannelTimeCycle > 0"!]
    /* The unit of the event channel time cycle */
    XCP_[!"XcpEventChannelTimeUnit"!]_MASK,
    [!ELSE!][!/*
    */!]    /* The unit of the event channel time cycle */
    XCP_TIME_UNIT_DONT_CARE_MASK, /* DON'T CARE value because XcpEventChannelTimeCycle = 0 */
    [!ENDIF!]
    [!IF "$XcpEventChannelMFEnabled"!]
      [!IF "XcpEventChannelTimeCycle > 0"!]
    /* Index in the flags holding whether the event MF is currently processing */
    [!"num:i($EventChannelMFIndex)"!]U,
        [!VAR "EventChannelMFIndex" = "$EventChannelMFIndex + 1"!]
      [!ELSE!]
    /* No index in the flags holding whether the event MF is currently processing */
    XCP_NO_EVENT_MAIN_FUNCTION_IDX,
      [!ENDIF!]

    [!ENDIF!]
  },
[!VAR "Counter" = "$Counter + count(XcpEventChannelTriggeredDaqListRef/*)"!]
[!ENDLOOP!]
};

[!IF "(count(XcpEventChannel/*) > 0) and ((count(XcpEventChannel/*[XcpEventChannelTimeCycle = 0]) > 0) or (../../XcpGeneral/XcpEventTriggerMainFunc = 'false'))"!]
/** \brief Configuration of Event Ids which  processed by the MainFunction,
 **        in the priority order */
CONST(uint16, XCP_CONST) Xcp_MFEventCfg[XCP_MAX_MF_PROCESSED_EVENTS] =
{
[!LOOP "node:order(XcpEventChannel/*,'number(255 - XcpEventChannelPriority)','XcpEventChannelNumber')"!]
  [!IF "(../../../../XcpGeneral/XcpEventTriggerMainFunc = 'false') or (XcpEventChannelTimeCycle = 0)"!]
  [!"XcpEventChannelNumber"!]U,  /* event [!"@name"!] */
  [!ENDIF!]
[!ENDLOOP!]
};
[!ENDIF!]

[!IF "$CyclicEventCount > 0 and $XcpMaxDaq > 0"!]
/** \brief Cyclic Events Ids which are set by MainFunction */
CONST(uint16, XCP_CONST) Xcp_MFCyclicEventCfg[XCP_MAX_CYCLIC_EVENT_CHANNEL] =
{
[!LOOP "XcpEventChannel/*[XcpEventChannelTimeCycle > 0]"!]
  [!"XcpEventChannelNumber"!]U,  /* event [!"@name"!] */
[!ENDLOOP!]
};
[!ENDIF!]

[!ENDIF!]

[!IF "$XcpMaxDaq > 0 and $XcpMaxDaq > $XcpDaqCount"!][!/* We have static or predefined DAQ lists */!]

[!VAR "IdentificationFieldType" = "../../XcpGeneral/XcpIdentificationFieldType"!]
[!VAR "CounterODT" = "0"!]
[!VAR "XcpDaqListName" = "0"!]
[!VAR "CounterODTEntry" = "0"!]
[!VAR "MirrorPos" = "0"!]

[!IF "$XcpNoOfNonDynamicODTEntries > 0 and ../../RamOptimizations/XcpEnableDaqOptimization='true'"!][!//

CONSTP2VAR(uint8, XCP_CONST, XCP_APPL_DATA) Xcp_OdtAddressTableROM[XCP_NO_OF_NON_DYNAMIC_ODTENTRIES] =
{
[!CALL "XcpGenerateOdtEntries","member"="'Address'"!]
};

[!ENDIF!][!//

/** \brief All Non-dynamic DAQ lists together with ODTs and ODT entries
 ** This information is used to restore all non-dynamic DAQ lists with default values
 ** for initialization/recovery purposes.
 */
CONST(Xcp_NonDynamicDaqListsType, XCP_CONST) Xcp_DaqListsDefault =
{
  /* ------------- Xcp_Daq list configuration section: ---------------------------------------- */
  {
  [!LOOP "node:order(XcpDaqList/*, 'XcpDaqListNumber')"!]
    [!IF "XcpDaqListNumber < $XcpMinDaq"!]
      [!IF "XcpDaqListNumber = 0"!]
    /* ---- DAQ List property structure for all predefined DAQ Lists. ---- */
      [!ENDIF!]
    [!ELSE!]
      [!IF "XcpDaqListNumber = $XcpMinDaq"!]
    /* ---- DAQ List property structure for all Static DAQ Lists. -------- */
      [!ENDIF!]
    [!ENDIF!]
    /* DaqList configuration for the DaqListId = [!"num:i(XcpDaqListNumber)"!] */
    {
      /* Pointer to the ODT Array or DAQ List */
      &Xcp_DaqLists.Xcp_NonDynamicDaqLists.Xcp_Odt[[!"num:i($CounterODT)"!]],
      /* Event channel number for this DAQ */
      [!VAR "XcpDaqListName" = "name(.)"!][!//
      [!VAR "XcpEventPresent" = "0"!]
      [!VAR "XcpDirectionSet" = "0"!]
      [!LOOP "../../XcpEventChannel/*/XcpEventChannelTriggeredDaqListRef/*[name(as:ref(.)) = $XcpDaqListName]"!]
      [!"../../XcpEventChannelNumber"!]U,
      [!VAR "XcpEventPresent" = "1"!]
      [!ENDLOOP!]
      [!IF "$XcpEventPresent = 0"!]
      XCP_INVALID_EVENT_CHANNEL,
      [!ENDIF!]
      /* Current  mode information of the DAQ List */
      [!IF "node:exists(XcpDaqListType) and (XcpDaqListType = 'DAQ' or XcpDaqListType = 'STIM')"!]
      XCP_MASK_DAQLIST_DIRECTION_[!"XcpDaqListType"!],
      [!VAR "XcpDirectionSet" = "1"!]
      [!ELSE!]
      0U,
      [!ENDIF!]
      /* Priority of this DAQ */
      [!IF "(node:exists(./XcpDaqListPriority) = 'true')"!]
      [!"./XcpDaqListPriority "!]U,
      [!ELSE!]
      0U,
      [!ENDIF!]
      [!IF "$IdentificationFieldType = 'ABSOLUTE'"!]
      /* The first PID only, when using absolute ODT number */
      [!"num:i($CounterODT)"!]U,
      [!ENDIF!]
      /* Number of ODTs in this DAQ list*/
      [!IF "XcpDaqListNumber <  $XcpMinDaq"!]
      [!"num:i(count(./XcpOdt/*))"!]U,
      [!ELSE!]
      [!"./XcpMaxOdt"!]U,
      [!ENDIF!]
      /* Maximum number of ODT entries in an ODT for this DAQ list */
      [!IF "XcpDaqListNumber <  $XcpMinDaq"!]
      [!VAR "ODTEntryCntMax" = "0"!]
      [!VAR "ODTEntryCnt" = "0"!]
      [!LOOP "./XcpOdt/*"!]
        [!VAR "ODTEntryCnt" = "count(./XcpOdtEntry/*)"!]
        [!IF "$ODTEntryCntMax < $ODTEntryCnt"!]
          [!VAR "ODTEntryCntMax" = "$ODTEntryCnt"!]
        [!ENDIF!]
      [!ENDLOOP!]
      [!"num:i($ODTEntryCntMax)"!]U,
      [!ELSE!]
      [!"./XcpMaxOdtEntries"!]U,
      [!ENDIF!]
      [!IF "(node:exists(./XcpDaqListPrescaler) = 'true')"!]
        [!VAR "PrescalerVal" = "num:i(XcpDaqListPrescaler)"!]
      [!ELSE!]
        [!VAR "PrescalerVal" = "num:i(1)"!]
      [!ENDIF!]
      /* Preset value of the prescaler */
      [!"$PrescalerVal"!]U,
      /* Current value of the prescaler */
      [!"$PrescalerVal"!]U,
      /* Configuration properties of the DAQ List */
      [!IF "(XcpDaqListNumber < $XcpMinDaq)"!]
      XCP_DAQ_LIST_DIRECTION_[!"XcpDaqListType"!][!IF "text:tolower(./XcpDaqEventFixed) = 'true'"!] | XCP_MASK_DAQLIST_EVENT_FIXED[!ENDIF!] [!IF "XcpDaqListNumber < $XcpMinDaq"!] | XCP_DAQ_LIST_PREDEFINED[!ENDIF!],
      [!ELSE!]
        [!IF "($XcpStimSupported = 'true')"!]
      XCP_DAQ_LIST_DIRECTION_DAQ_STIM,
        [!ELSE!]
      XCP_DAQ_LIST_DIRECTION_DAQ,
        [!ENDIF!]
      [!ENDIF!]
      /* Additional configuration of the DAQ List */
      [!IF "(XcpDaqListNumber < $XcpMinDaq) and ($XcpEventPresent = 1) and ($XcpDirectionSet = 1)"!]
      XCP_MASK_DAQLIST_READY | XCP_MASK_DAQLIST_CONFIGURED,
      [!ELSE!]
      0U,
      [!ENDIF!]
    },
  [!IF "XcpDaqListNumber <  $XcpMinDaq"!]
    [!VAR "CounterODT" = "$CounterODT + count(XcpOdt/*)"!]
  [!ELSE!]
    [!VAR "CounterODT" = "$CounterODT + num:integer(XcpMaxOdt)"!]
  [!ENDIF!]
  [!ENDLOOP!]
  },
  /* --------------------- Xcp_Odt configuration section: ------------------------------------- */
  {
  [!LOOP "node:order(XcpDaqList/*, 'XcpDaqListNumber')"!]
    [!IF "(XcpDaqListNumber < $XcpMinDaq)"!]
      [!IF "XcpDaqListNumber = 0"!]
    /* ----- ODT configuration section for predefined ODTs ------- */
      [!ENDIF!]
    /* --- ODT configuration list for the DaqListId = [!"num:i(XcpDaqListNumber)"!] --- */
      [!LOOP "node:order(XcpOdt/*, 'XcpOdtNumber')"!]
    /* ODT configuration for the OdtID = [!"num:i(XcpOdtNumber)"!][!IF "($XcpTimestampIsEnabled = 'true') and (XcpOdtNumber = 0)"!] (the resulting DTO for this ODT contains [!"num:i($XcpTimestampSize)"!] byte[!IF "$XcpTimestampSize > 1"!]s[!ENDIF!][!CR!]
     *                                      with the time stamp data) [!ENDIF!]*/
    {
      /* Pointer to OdtEntry array or Object Descriptor table */
[!IF "as:modconf('Xcp')[1]/RamOptimizations/XcpEnableDaqOptimization='false'"!][!//
      &(Xcp_DaqLists.Xcp_NonDynamicDaqLists.Xcp_OdtEntry)[[!"num:i($CounterODTEntry)"!]],
[!ELSE!][!//
      /* Deviation MISRAC2012-1 */
      (usize*)&(Xcp_OdtAddressTableRAM[[!"num:i($CounterODTEntry)"!]]),
[!ENDIF!][!//
      [!VAR "DtoSize" = "0"!]
      [!LOOP "node:order(XcpOdtEntry/*, 'XcpOdtEntryNumber')"!]
        [!VAR "DtoSize" = "$DtoSize + XcpOdtEntryLength"!]
      [!ENDLOOP!]
      /* Size of data segment in DTO package.*/
      [!"num:integer($DtoSize)"!]U,
      /* Position index of data in OdtMirrorDataArray. */
      [!"num:i($MirrorPos)"!]U,

      /* Number of elements in the ODT */
      [!"num:i(count(XcpOdtEntry/*))"!]U
    },
      [!VAR "CounterODTEntry" = "$CounterODTEntry + count(XcpOdtEntry/*)"!]
      [!VAR "MirrorPos" = "$MirrorPos + $DtoSize"!]
      [!ENDLOOP!]
    [!ELSE!]
      [!IF "XcpDaqListNumber = $XcpMinDaq"!]
    /* ----- ODT configuration section for static ODTs ---------- */
      [!ENDIF!]
    /* --- ODT configuration list for the DaqListId = [!"num:i(XcpDaqListNumber)"!] --- */
      [!FOR "x" = "1" TO "num:i(XcpMaxOdt)"!]
    /* ODT configuration for the OdtID = [!"num:i($x - 1)"!][!IF "($XcpTimestampIsEnabled = 'true') and ($x = 1)"!] (the resulting DTO for this ODT contains [!"num:i($XcpTimestampSize)"!] byte[!IF "$XcpTimestampSize > 1"!]s[!ENDIF!][!CR!]
     *                                      with the time stamp data) [!ENDIF!]*/
       [!IF "(XcpMaxOdtEntries > $XcpAbsoluteMaxOdtEntrySize) and ($XcpTimestampIsEnabled = 'true') and ($x = 1)"!]
         [!VAR "XcpRelativeOdtEntrySize" = "$XcpAbsoluteMaxOdtEntrySize"!]
       [!ELSE!]
         [!VAR "XcpRelativeOdtEntrySize" = "XcpMaxOdtEntries"!]
       [!ENDIF!]
    {
      /* Pointer to OdtEntry array or Object Descriptor table */
[!IF "as:modconf('Xcp')[1]/RamOptimizations/XcpEnableDaqOptimization='false'"!][!//
      &(Xcp_DaqLists.Xcp_NonDynamicDaqLists.Xcp_OdtEntry)[[!"num:i($CounterODTEntry)"!]],
[!ELSE!][!//
      /* Deviation MISRAC2012-1 */
      (usize*)&(Xcp_OdtAddressTableRAM[[!"num:i($CounterODTEntry)"!]]),
[!ENDIF!][!//
      [!VAR "DtoSize" = "XcpMaxOdtEntries * $MaxSizeOdtEntry"!]
      /* Size of data segment in DTO package.*/
      0U,
      /* Position index of data in OdtMirrorDataArray. */
      [!"num:i($MirrorPos)"!]U,
      /* Number of elements in the ODT */
      [!"$XcpRelativeOdtEntrySize"!]U
    },
      [!VAR "CounterODTEntry" = "$CounterODTEntry + $XcpRelativeOdtEntrySize"!]
      [!VAR "MirrorPos" = "$MirrorPos + $DtoSize"!]
      [!ENDFOR!]
    [!ENDIF!]
  [!ENDLOOP!]
  },
[!IF "as:modconf('Xcp')[1]/RamOptimizations/XcpEnableDaqOptimization='false'"!][!//
  /* ------------------ Xcp_OdtEntry configuration section: ----------------------------------- */
  [!CALL "XcpGenerateOdtEntries"!]
[!ENDIF!][!//
};
[!ENDIF!] [!/* We have static or predefined DAQ lists */!]
[!IF "count(../../XcpUserCommand/*) > 0"!]
/** \brief Configuration of user defined commands */
CONST(Xcp_UserCommandConfType, XCP_CONST) Xcp_UserCommandConf[XCP_NUMBER_OF_USERCOMMANDS] =
{
  [!LOOP "node:order(../../XcpUserCommand/*, 'XcpSubCommandCode')"!]
  {
    [!IF "(not(node:empty(./XcpSubCommandCallout)))"!]
    &[!"./XcpSubCommandCallout"!], /* Pointer to user defined call-out function */
    [!ELSE!]
    NULL_PTR, /* Pointer to user defined call-out function */
    [!ENDIF!]
    [!"./XcpSubCommandCode"!]U, /* Configured Code for SubCommand */
    [!"./XcpSubCommandLength"!]U, /* Configured Length for SubCommand */
    [!IF "node:exists(./XcpSubCommandSeedSupport) = 'true'"!]
      [!IF "(./XcpSubCommandSeedSupport = 'RESOURCE_PGM')"!]
    XCP_RESOURCE_PGM_MASK, /* Configured resource: PGM */
      [!ELSEIF "(./XcpSubCommandSeedSupport = 'RESOURCE_STIM')"!]
    XCP_RESOURCE_STIM_MASK, /* Configured resource: STIM */
      [!ELSEIF "(./XcpSubCommandSeedSupport = 'RESOURCE_DAQ')"!]
    XCP_RESOURCE_DAQ_MASK, /* Configured resource: DAQ */
      [!ELSEIF "(./XcpSubCommandSeedSupport = 'RESOURCE_CAL_PAG')"!]
    XCP_RESOURCE_CAL_PAG_MASK, /* Configured resource: CAL_PAG */
      [!ELSE!]
    XCP_RESOURCE_UNLOCKED, /* Resource Unlocked */
      [!ENDIF!]
    [!ELSE!]
    XCP_RESOURCE_UNLOCKED, /* Seed and Key Disabled */
    [!ENDIF!]
    [!"./XcpSubCommandResponseLength"!]U /* Configured length for the response */
  },
  [!ENDLOOP!]
};
[!ENDIF!]


[!IF "num:i(count(XcpConnectionCfg/*/XcpConnectionInterfaceType/XcpFlexrayBufferCfg/*)) > 0"!]



/** \brief Configured Flexray Buffers */
CONST(Xcp_FlxBufType, XCP_CONST) Xcp_FlxBufConfig[XCP_NO_OF_FLX_BUF_CONFIGURED] =
{
[!LOOP "node:order(XcpConnectionCfg/*/XcpConnectionInterfaceType/XcpFlexrayBufferCfg/*, 'FLX_BUFCfg/XcpFlxBufId')"!]
[!VAR "XcpBufferFlags" = "''"!]
[!VAR "XcpBufferFlagsAtLeastOneIsSet" = "'false'"!]
[!IF "node:refvalid(FLX_BUFCfg/XcpFrIfLPDURef) and (count(node:ref(node:ref(node:ref(node:ref(FLX_BUFCfg/XcpFrIfLPDURef)/FrIfVBTriggeringRef)/FrIfFrameStructureRef)/FrIfPdusInFrame/*[1]/FrIfPduRef)/FrIfPduDirection/FrIfTxPduRef) > 0)"!]
  [!VAR "XcpBufferFlags" = "concat($XcpBufferFlags,'XCP_BUFFER_DIRECTION_TX_MASK',' ')"!]
  [!VAR "XcpBufferFlagsAtLeastOneIsSet" = "'true'"!]  
[!ENDIF!]
[!IF "((node:exists(FLX_BUFCfg/XcpFlxBufMaxLenInitValue) = 'true') and (node:exists(LPDU_IDCfg/XcpFlxSlotIdInitValue) = 'true') and (node:exists(LPDU_IDCfg/XcpFlxOffsetInitValue) = 'true')and (node:exists(LPDU_IDCfg/XcpFlxCycleRepetitionInitValue) = 'true')and (node:exists(LPDU_IDCfg/XcpFlxChannelInitValue) = 'true'))"!]
  [!VAR "XcpBufferFlags" = "concat($XcpBufferFlags,'XCP_BUFFER_PRECONFIGURED_MASK',' ')"!]
  [!VAR "XcpBufferFlagsAtLeastOneIsSet" = "'true'"!]  
[!ENDIF!]
[!IF "((FLX_BUFCfg/XcpFlxBufMaxLen = 'FIXED')  and (LPDU_IDCfg/XcpFlxSlotId = 'FIXED') and (LPDU_IDCfg/XcpFlxOffset = 'FIXED') and (LPDU_IDCfg/XcpFlxCycleRepetition = 'FIXED') and (LPDU_IDCfg/XcpFlxChannel = 'FIXED'))"!]
  [!VAR "XcpBufferFlags" = "concat($XcpBufferFlags,'XCP_BUFFER_FIXED_MASK',' ')"!]
  [!VAR "XcpBufferFlagsAtLeastOneIsSet" = "'true'"!]  
[!ENDIF!]
[!IF "$XcpBufferFlagsAtLeastOneIsSet = 'false'"!]
[!VAR "XcpBufferFlags" = "concat($XcpBufferFlags,'XCP_FLX_NO_BIT_SET_MASK',' ')"!]
[!ENDIF!]
[!VAR "pduIdxCount" = "0"!]
[!VAR "pduIdxToWrite" = "0"!]
[!IF "(node:exists(FLX_BUFCfg/XcpFlxRxConnectionInfoRef) = 'true')"!]
[!VAR "pduIdxToCompare" = "as:ref(as:ref(FLX_BUFCfg/XcpFlxRxConnectionInfoRef)/XcpMapRxPdu2Connection)/XcpRxPduId"!]
  [!LOOP "node:order(../../../../../XcpPdu/*[@name='XcpRxPdu'], 'XcpRxPduId')"!]
      [!IF "$pduIdxToCompare = XcpRxPduId"!]
        [!VAR "pduIdxToWrite" = "$pduIdxCount"!]
      [!ENDIF!]
      [!VAR "pduIdxCount" = "$pduIdxCount + 1"!]
  [!ENDLOOP!]
[!ENDIF!]
[!IF "(node:exists(FLX_BUFCfg/XcpFlxTxConnectionInfoRef) = 'true')"!]
[!VAR "pduIdxToCompare" = "as:ref(as:ref(FLX_BUFCfg/XcpFlxTxConnectionInfoRef)/XcpMapTxPdu2Connection)/XcpTxPduId"!]
  [!LOOP "node:order(../../../../../XcpPdu/*[@name='XcpTxPdu'], 'XcpTxPduId')"!]
      [!IF "$pduIdxToCompare = XcpTxPduId"!]
        [!VAR "pduIdxToWrite" = "$pduIdxCount"!]
      [!ENDIF!]
      [!VAR "pduIdxCount" = "$pduIdxCount + 1"!]
  [!ENDLOOP!]
[!ENDIF!]
  {
    [!"FLX_BUFCfg/XcpFlxBufId"!], /* ID of the FLX_BUF */
    [!"as:ref(FLX_BUFCfg/XcpFrIfLPDURef)/FrIfLPduIdx"!], /* LPDU_ID of the FLX_BUF */
    [!"num:i($pduIdxToWrite)"!], /* index in either Xcp_TxLPduIdConf or Xcp_RxLPduIdConf */
    [!"as:ref(FLX_BUFCfg/XcpFrIfLPDURef)/../../FrIfCtrlIdx"!], /* Controller ID corresponding LPDU_ID */
    XCP_FLX_BUF_VALUE_[!"FLX_BUFCfg/XcpFlxBufMaxLen"!], /* MAX_FLX_LEN_BUF properties */
[!IF "(node:exists(FLX_BUFCfg/XcpFlxBufMaxLenInitValue) = 'true')"!]
    [!"FLX_BUFCfg/XcpFlxBufMaxLenInitValue"!], /* MAX_FLX_LEN_BUF initial value */
[!ELSE!]
    XCP_FLX_BUF_NO_INIT_VALUE, /* MAX_FLX_LEN_BUF has no initial value */
[!ENDIF!]
    XCP_FLX_BUF_VALUE_[!"LPDU_IDCfg/XcpFlxSlotId"!], /* SLOT_ID properties */
[!IF "(node:exists(LPDU_IDCfg/XcpFlxSlotIdInitValue) = 'true')"!]
    [!"LPDU_IDCfg/XcpFlxSlotIdInitValue"!], /* SLOT_ID initial value */
[!ELSE!]
    XCP_FLX_BUF_NO_INIT_VALUE, /* SLOT_ID has no initial value */
[!ENDIF!]
    XCP_FLX_BUF_VALUE_[!"LPDU_IDCfg/XcpFlxOffset"!], /* OFFSET properties */
[!IF "(node:exists(LPDU_IDCfg/XcpFlxOffsetInitValue) = 'true')"!]
    [!"LPDU_IDCfg/XcpFlxOffsetInitValue"!], /* OFFSET initial value */
[!ELSE!]
    XCP_FLX_BUF_NO_INIT_VALUE, /* OFFSET has no initial value */
[!ENDIF!]
    XCP_FLX_BUF_VALUE_[!"LPDU_IDCfg/XcpFlxCycleRepetition"!], /* CYCLE_REPETITION properties */
[!IF "(node:exists(LPDU_IDCfg/XcpFlxCycleRepetitionInitValue) = 'true')"!]
    [!"LPDU_IDCfg/XcpFlxCycleRepetitionInitValue"!], /* CYCLE_REPETITION initial value */
[!ELSE!]
    XCP_FLX_BUF_NO_INIT_VALUE, /* CYCLE_REPETITION has no initial value */
[!ENDIF!]
    XCP_FLX_BUF_VALUE_[!"LPDU_IDCfg/XcpFlxChannel"!], /* CHANNEL properties */
[!IF "(node:exists(LPDU_IDCfg/XcpFlxChannelInitValue) = 'true')"!]
    XCP_FLX_BUF_VALUE_CHANNEL_[!"LPDU_IDCfg/XcpFlxChannelInitValue"!], /* CHANNEL initial value */
[!ELSE!]
    XCP_FLX_BUF_NO_INIT_VALUE, /* CHANNEL has no initial value */
[!ENDIF!]
[!CALL "PrintPropertyFlags","PropertyList"="$XcpBufferFlags","CommentString"="'Buffer flags'","Indentation"="4"!]
  },

[!ENDLOOP!]
};
[!ENDIF!]

[!SELECT "../../XcpGeneral"!]
[!IF "(XcpBlockWriteReadMemoryRAMMechanism = 'true')"!]
[!IF "((node:exists(XcpWriteMemoryRAMCallback)) and not(node:empty(XcpWriteMemoryRAMCallback)))"!]
  [!VAR "GlobalWriteCallback" = "XcpWriteMemoryRAMCallback"!]
[!ENDIF!]
[!IF "((node:exists(XcpReadMemoryRAMCallback)) and not(node:empty(XcpReadMemoryRAMCallback)))"!]
  [!VAR "GlobalReadCallback" = "XcpReadMemoryRAMCallback"!]
[!ENDIF!]
[!ENDIF!]
[!ENDSELECT!]

[!IF "num:i(count(XcpMemoryAccessArea/XcpMemoryAccessArea/*)) > 0"!]
/* Deviation MISRAC2012-1 <START> */
/** \brief Array holding the configured memory areas */
CONST(Xcp_MemoryAreaType, XCP_CONST) Xcp_MemoryAreas[XCP_NUMBER_OF_MEMORY_AREAS] =
{
[!LOOP "node:order(XcpMemoryAccessArea/XcpMemoryAccessArea/*)"!]
  {
   /* The memory area start address */
   (P2VAR(uint8, XCP_CONST, XCP_APPL_DATA))[!"XcpMemoryAreaStartAddress"!],
   /* The memory area length */
   [!"num:inttohex(XcpMemoryAreaLength)"!]U,
   /* The memory area access type (read or/and write) */
   XCP_MEMORY_AREA_TYPE_[!"XcpMemoryAreaAcccessType"!],
   /* The memory area scope (calibration and/or DAQ_STIM) */
   XCP_MEMORY_AREA_SCOPE_[!"XcpMemoryAreaAcccessScope"!],
[!IF "$XcpBswDistributionEnabled"!]
   [!"node:name(node:ref(XcpMemoryAccessAreaApplicationRef))"!],
[!ENDIF!]
[!IF "$RAMCalloutProxyEnabled='true'"!]
   {
[!IF "node:exists(XcpMemoryAccessAreaWriteRAMCallout)"!]
     &[!"XcpMemoryAccessAreaWriteRAMCallout"!],
[!ELSE!]
     &[!"$GlobalWriteCallback"!],
[!ENDIF!]
[!IF "node:exists(XcpMemoryAccessAreaReadRAMCallout)"!]
     &[!"XcpMemoryAccessAreaReadRAMCallout"!]
[!ELSE!]
     &[!"$GlobalReadCallback"!]
[!ENDIF!]
   }
[!ENDIF!]
  },
[!ENDLOOP!]
};
/* Deviation MISRAC2012-1 <STOP> */
[!ENDIF!]

#define XCP_STOP_SEC_CONST_UNSPECIFIED
#include <Xcp_MemMap.h>

#define XCP_START_SEC_CONST_8
#include <Xcp_MemMap.h>

#if (XCP_ON_ETHERNET_ENABLED == STD_ON)

/** \brief Array that holds information about which connections are on IPv6 */
CONST(uint8, XCP_CONST) Xcp_Connections_IPv6[XCP_CONNECTION_FLAG_SIZE] =
{
[!VAR "IPv6Flag" = "'0'"!]
[!LOOP "node:order(XcpConnectionCfg/*,'XcpConnectionId')"!]
  [!IF "node:refvalid(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)"!]
    [!VAR "TxPduName"="node:ref(node:ref(XcpTxPduConnectionInfo/*[1]/XcpMapTxPdu2Connection)/XcpTxPduRef)/@name"!]
    [!VAR "IsIPv6" = "'false'"!]
    [!IF "node:exists(as:modconf('SoAd')/SoAdConfig/*/SoAdPduRoute/*/SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)"!]
      [!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)/SoAdPduRoute/*[node:refvalid(SoAdTxPduRef) and (node:ref(SoAdTxPduRef)/@name = $TxPduName) and node:refvalid(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef) and node:refvalid(node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)/../../SoAdSocketLocalAddressRef) and node:ref(node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)/../../SoAdSocketLocalAddressRef)/TcpIpDomainType = 'TCPIP_AF_INET6']"!]
        [!VAR "IsIPv6" = "'true'"!]
        [!BREAK!]
      [!ENDSELECT!]
    [!ENDIF!]
    [!IF "node:exists(as:modconf('SoAd')/SoAdConfig/*/SoAdPduRoute/*/SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)"!]
      [!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)/SoAdPduRoute/*[node:refvalid(SoAdTxPduRef) and (node:ref(SoAdTxPduRef)/@name = $TxPduName) and node:refvalid(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef) and node:refvalid(node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)/../../SoAdSocketLocalAddressRef) and node:ref(node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)/../../SoAdSocketLocalAddressRef)/TcpIpDomainType  = 'TCPIP_AF_INET6']"!]
        [!VAR "IsIPv6" = "'true'"!]
        [!BREAK!]
      [!ENDSELECT!]
    [!ENDIF!]
    [!IF "$IsIPv6 = 'true'"!]
      [!VAR "IPv6Flag" = "bit:bitset(num:i($IPv6Flag), num:mod(XcpConnectionId, 8.0)[1])"!]
    [!ENDIF!]
  [!ENDIF!]
  [!IF "(num:mod(XcpConnectionId, 8.0)[1] = 7.0) and (XcpConnectionId < $NoOfConnections - 1.0)"!]
  [!"$IPv6Flag"!]U,
    [!VAR "IPv6Flag" = "'0'"!]
  [!ENDIF!]
[!ENDLOOP!]
  [!"$IPv6Flag"!]U
};

#endif

#define XCP_STOP_SEC_CONST_8
#include <Xcp_MemMap.h>

/*==================[internal data]=========================================*/

[!IF "($XcpMaxEventChannel > 0) and (num:i(sum(XcpEventChannel/*/XcpEventChannelMaxDaqList)) > 0)"!]

[!IF "../../XcpGeneral/XcpIdentificationFieldType = 'RELATIVE_BYTE'"!]
#define XCP_START_SEC_VAR_8
[!ELSE!]
#define XCP_START_SEC_VAR_16
[!ENDIF!]
#include <Xcp_MemMap.h>

/** \brief List of DAQ IDs in the order of events */
STATIC VAR(Xcp_DaqIdType, XCP_VAR) Xcp_DaqIdList[XCP_EVENT_CHANNEL_TOTAL_DAQ] =
{
[!LOOP "node:order(XcpEventChannel/*, 'XcpEventChannelNumber')"!]
  /* Daq list configuration for [!"@name"!]. */
  [!IF "XcpEventChannelMaxDaqList > 0"!]
    [!LOOP "node:order(XcpEventChannelTriggeredDaqListRef/*, '255 - node:ref(.)/XcpDaqListPriority')"!]
  [!"as:ref(.)/XcpDaqListNumber"!]U,
    [!ENDLOOP!]
    [!IF "count(./XcpEventChannelTriggeredDaqListRef/*) < XcpEventChannelMaxDaqList"!]
      [!FOR "x" = "1" TO "num:i(XcpEventChannelMaxDaqList - count(./XcpEventChannelTriggeredDaqListRef/*))"!]
  XCP_DUMMY_DAQ_LIST,
      [!ENDFOR!]
   [!ENDIF!]
  [!ENDIF!]
[!ENDLOOP!]
};

[!IF "../../XcpGeneral/XcpIdentificationFieldType = 'RELATIVE_BYTE'"!]
#define XCP_STOP_SEC_VAR_8
[!ELSE!]
#define XCP_STOP_SEC_VAR_16
[!ENDIF!]
#include <Xcp_MemMap.h>
[!ENDIF!]

/*-------------[Xcp BSW distribution internal data]-------------------------*/

[!IF "$XcpBswDistributionEnabled"!]
#define XCP_START_SEC_CONST_8
#include <Xcp_MemMap.h>

CONST(uint8, XCP_CONST) Xcp_ConnectionToPartitionMap[XCP_NO_OF_CONNECTIONS] = 
{
[!LOOP "node:order(as:modconf('Xcp')[1]/XcpConfig/*[1]/XcpConnectionCfg/*, 'XcpConnectionId')"!]
  [!"node:name(node:ref(XcpConnectionApplicationRef))"!],
[!ENDLOOP!]
};

#define XCP_STOP_SEC_CONST_8
#include <Xcp_MemMap.h>

#define XCP_START_SEC_CONST_UNSPECIFIED
#include <Xcp_MemMap.h>

CONST(XcpSchMExclusiveAreaStruct, XCP_CONST) Xcp_SchMExclusiveAreas[XCP_NUM_OF_OS_CORES] = 
{
[!FOR "core"="0" TO "num:i(as:modconf('Os')[1]/OsOS/OsNumberOfCores) - 1"!]
[!VAR "foundCore" = "''"!]
  {
[!LOOP "text:split($CorePartitionMap,' ')"!]
[!IF "text:split(.,':')[1] = $core"!]
[!VAR "foundCore" = "text:split(.,':')[2]"!]
[!ENDIF!]
[!ENDLOOP!]
[!IF "$foundCore = ''"!]
    NULL_PTR,
    NULL_PTR,
[!ELSE!]
    &SchM_Enter_Xcp_1_[!"$foundCore"!]_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS,
    &SchM_Exit_Xcp_1_[!"$foundCore"!]_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS,
[!ENDIF!]
  },
[!ENDFOR!]
};

#define XCP_STOP_SEC_CONST_UNSPECIFIED
#include <Xcp_MemMap.h>

[!ENDIF!]

[!IF "$XcpMemoryAccessAreas"!][!//
Xcp_SendMemoryProxyData_FuncPtr Xcp_SendMemoryProxyData[[!"$XcpNumOsApps"!]][[!"$XcpNumOsApps"!]] = {
  [!LOOP "(as:modconf('Os')[1]/OsApplication/*)"!][!//
  {
    [!VAR "XcpReceivingApplicationList" = "''"!][!//
    [!VAR "NeedsEntry" = "'false'"!][!//
    [!VAR "PartitionName" = "name(.)"!][!//
    [!LOOP "(as:modconf('Xcp')[1]/XcpConfig/*[1]/XcpConnectionCfg/*)"!][!//
      [!IF "name(as:ref(./XcpConnectionApplicationRef)) = $PartitionName"!][!//
        [!VAR "NeedsEntry" = "'true'"!][!//
        [!BREAK!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
    [!IF "$NeedsEntry = 'true'"!]
      [!FOR "Index" = "0" TO "$XcpNumOsApps - 1"!][!//
        [!VAR "NeedsEntry" = "'false'"!][!//
        [!LOOP "(as:modconf('Xcp')[1]/XcpConfig/*[1]/XcpMemoryAccessArea/XcpMemoryAccessArea/*)"!][!//
          [!VAR "XcpReceivingApplication" = "name(as:ref(./XcpMemoryAccessAreaApplicationRef))"!][!//
          [!IF "(node:ref(./XcpMemoryAccessAreaApplicationRef))/@index = $Index"!][!//
            [!IF "not(contains($XcpReceivingApplicationList,concat(' ',$XcpReceivingApplication,' ')))"!][!//
              [!IF "not(contains($PartitionName,$XcpReceivingApplication))"!][!//
  SchM_Send_Xcp_1_[!"$PartitionName"!]_MemoryProxyData_[!"$PartitionName"!]_To_[!"$XcpReceivingApplication"!],
                  [!VAR "XcpReceivingApplicationList" = "concat($XcpReceivingApplicationList,' ',$XcpReceivingApplication,' ')"!][!//
                  [!VAR "NeedsEntry" = "'true'"!][!//
                  [!BREAK!][!//
              [!ENDIF!][!//
            [!ENDIF!][!//
          [!ENDIF!][!//
        [!ENDLOOP!][!//
        [!IF "$NeedsEntry = 'false'"!]
  NULL_PTR,
        [!ENDIF!]
      [!ENDFOR!][!//
    [!ELSE!]
      [!FOR "Index" = "0" TO "$XcpNumOsApps - 1"!][!//
  NULL_PTR,
      [!ENDFOR!] 
    [!ENDIF!]
  },
  [!ENDLOOP!][!//
};
[!IF "($XcpDaqStimMemoryAccessAreas = 'true') or ($RAMCalloutProxyEnabled = 'true')"!][!//
Xcp_SendMemoryProxyNotification_FuncPtr Xcp_SendMemoryProxyNotification[[!"$XcpNumOsApps"!]][[!"$XcpNumOsApps"!]] = {
  [!LOOP "(as:modconf('Os')[1]/OsApplication/*)"!][!//
  {
    [!VAR "XcpReceivingApplicationList" = "''"!][!//
    [!VAR "NeedsEntry" = "'false'"!][!//
    [!VAR "PartitionName" = "name(.)"!][!//
    [!LOOP "(as:modconf('Xcp')[1]/XcpConfig/*[1]/XcpMemoryAccessArea/XcpMemoryAccessArea/*)"!][!//
      [!IF "name(as:ref(./XcpMemoryAccessAreaApplicationRef)) = $PartitionName"!][!//
        [!VAR "NeedsEntry" = "'true'"!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
    [!IF "$NeedsEntry = 'true'"!]
      [!FOR "Index" = "0" TO "$XcpNumOsApps - 1"!][!//
        [!VAR "NeedsEntry" = "'false'"!][!//
        [!LOOP "(as:modconf('Xcp')[1]/XcpConfig/*[1]/XcpConnectionCfg/*)"!][!//
          [!VAR "XcpReceivingApplication" = "name(as:ref(./XcpConnectionApplicationRef))"!][!//
          [!IF "(node:ref(./XcpConnectionApplicationRef))/@index = $Index"!][!//
            [!IF "not(contains($XcpReceivingApplicationList,concat(' ',$XcpReceivingApplication,' ')))"!][!//
              [!IF "not(contains($PartitionName,$XcpReceivingApplication))"!][!//
  SchM_Send_Xcp_1_XcpMemoryProxy_[!"$PartitionName"!]_EventId_[!"$PartitionName"!]_To_[!"$XcpReceivingApplication"!],
                  [!VAR "XcpReceivingApplicationList" = "concat($XcpReceivingApplicationList,' ',$XcpReceivingApplication,' ')"!][!//
                  [!VAR "NeedsEntry" = "'true'"!][!//
                  [!BREAK!][!//
              [!ENDIF!][!//
            [!ENDIF!][!//
          [!ENDIF!]
        [!ENDLOOP!][!//
        [!IF "$NeedsEntry = 'false'"!]
  NULL_PTR,
        [!ENDIF!]
      [!ENDFOR!][!//
    [!ELSE!][!//
      [!FOR "Index" = "0" TO "$XcpNumOsApps - 1"!][!//
  NULL_PTR,
      [!ENDFOR!]
    [!ENDIF!][!//
  },
  [!ENDLOOP!][!//
};
[!ENDIF!][!//
[!ENDIF!][!//

/*==================[external data]=========================================*/
#define XCP_START_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

[!SELECT "../../XcpGeneral"!]
[!IF "(XcpBlockWriteReadMemoryRAMMechanism = 'true')"!]
/* Variable to hold the current information for accessing memory with a callout*/
VAR(Xcp_DescriptorCallbackType, XCP_VAR) Xcp_DescriptorCallbackTypePtr =
{
[!IF "((node:exists(XcpWriteMemoryRAMCallback)) and not(node:empty(XcpWriteMemoryRAMCallback)))"!]
  &[!"XcpWriteMemoryRAMCallback"!],
[!ENDIF!]
[!IF "((node:exists(XcpReadMemoryRAMCallback)) and not(node:empty(XcpReadMemoryRAMCallback)))"!]
  &[!"XcpReadMemoryRAMCallback"!]
[!ENDIF!]
};
[!ENDIF!]
[!ENDSELECT!]
#define XCP_STOP_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>
[!IF "$XcpMaxDaq > 0"!]

#define XCP_START_SEC_VAR_SAVED_ZONE_UNSPECIFIED
#include <Xcp_MemMap.h>

/** \brief All runtime DAQ lists together with ODTs and ODT entries and admin data*/
VAR(Xcp_AllDaqListsType, XCP_VAR_POWER_ON_CLEARED) Xcp_DaqLists;

[!IF "$XcpNoOfNonDynamicODTEntries > 0 and ../../RamOptimizations/XcpEnableDaqOptimization='true'"!][!//
P2VAR(uint8, XCP_APPL_DATA, XCP_VAR_POWER_ON_CLEARED) Xcp_OdtAddressTableRAM[XCP_NO_OF_NON_DYNAMIC_ODTENTRIES];
[!ENDIF!][!//

#define XCP_STOP_SEC_VAR_SAVED_ZONE_UNSPECIFIED
#include <Xcp_MemMap.h>

[!IF "$XcpNoOfNonDynamicODTEntries > 0 and ../../RamOptimizations/XcpEnableDaqOptimization='true'"!][!//
#define XCP_START_SEC_VAR_SAVED_ZONE_UNSPECIFIED_8
#include <Xcp_MemMap.h>

VAR(uint8,XCP_VAR_POWER_ON_CLEARED) Xcp_OdtLengthTableRAM[XCP_NO_OF_NON_DYNAMIC_ODTENTRIES];

[!IF "../../RamOptimizations/XcpEnableBitOffset='true'"!][!//
VAR(uint8,XCP_VAR_POWER_ON_CLEARED) Xcp_OdtBitOffsetTableRAM[XCP_NO_OF_NON_DYNAMIC_ODTENTRIES];
[!ENDIF!][!//

#define XCP_STOP_SEC_VAR_SAVED_ZONE_UNSPECIFIED_8
#include <Xcp_MemMap.h>
[!ENDIF!][!//

#define XCP_START_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>

[!IF "$XcpMaxEventChannel > 0"!]
[!VAR "Counter" = "0"!]
/** \brief Event */
VAR(Xcp_EventType, XCP_VAR) Xcp_Event[XCP_MAX_EVENT_CHANNEL] =
{
[!LOOP "node:order(XcpEventChannel/*, 'XcpEventChannelNumber')"!]
  {
    /* Time Cycle Counter */
    1U,
  [!IF "XcpEventChannelMaxDaqList > 0"!]
    /* Pointer to the list of DAQ IDs of this event */
    &Xcp_DaqIdList[[!"num:i($Counter)"!]],
  [!ELSE!]
    /* Pointer to the list of DAQ IDs of this event */
    NULL_PTR,
  [!ENDIF!]
    /* Next DAQ index to be sampled */
    0U,
    /* Next ODT to be sampled */
    0U,
  [!IF "XcpEventChannelMaxDaqList > 0"!]
    /* The number of DAQ Lists of this event */
    [!"num:i(count(./XcpEventChannelTriggeredDaqListRef/*))"!],
  [!ELSE!]
     /* The number of DAQ Lists of this event */
     0U,
  [!ENDIF!]
    /* Flag signaling whether this event was set (cyclically or via Xcp_SetEvent()) */
    FALSE,
  [!IF "$XcpEventChannelMFEnabled"!]
    /* Number of DAQ lists which are currently running and are associated with this event */
    0U,
  [!ENDIF!]
  },
[!VAR "Counter" = "$Counter + XcpEventChannelMaxDaqList"!]
[!ENDLOOP!]
};
[!ENDIF!]

#define XCP_STOP_SEC_VAR_UNSPECIFIED
#include <Xcp_MemMap.h>
[!ENDIF!]

#define XCP_START_SEC_VAR_8
#include <Xcp_MemMap.h>

[!SELECT "../../XcpGeneral"!]
VAR(uint8, XCP_VAR) Xcp_ControlStateOfXcpModule =          [!//
[!IF "(XcpDefaultXcpModuleState = 'true') and (node:exists(XcpDefaultXcpModuleState))"!]
XCP_STATE_ACTIVE;
[!ENDIF!]
[!IF "(XcpDefaultXcpModuleState = 'false') and (node:exists(XcpDefaultXcpModuleState))"!]
XCP_STATE_INACTIVE;
[!ENDIF!]
[!ENDSELECT!]

#define XCP_STOP_SEC_VAR_8
#include <Xcp_MemMap.h>
/*==================[external function definitions]=========================*/

/*-------------[Xcp BSW distribution external function definitions]---------------*/

[!IF "$XcpBswDistributionEnabled"!]
[!VAR "XcpConnectionOsAppRefNameList" = "''"!]
[!LOOP "(XcpConnectionCfg/*)"!]
  [!VAR "XcpConnectionOsAppRefName"="name(as:ref(./XcpConnectionApplicationRef))"!]
  [!IF "not(contains($XcpConnectionOsAppRefNameList,concat(' ',$XcpConnectionOsAppRefName,' ')))"!]

[!IF "../../../../XcpGeneral/XcpServiceWrapper = 'false'"!]
#define XCP_1_[!"$XcpConnectionOsAppRefName"!]_START_SEC_CODE
#include <Xcp_1_[!"$XcpConnectionOsAppRefName"!]_MemMap.h>

FUNC(void, XCP_1_[!"$XcpConnectionOsAppRefName"!]_CODE) Xcp_1_[!"$XcpConnectionOsAppRefName"!]_MainFunction(void)
{
  Xcp_MainFunction();
}

#define XCP_1_[!"$XcpConnectionOsAppRefName"!]_STOP_SEC_CODE
#include <Xcp_1_[!"$XcpConnectionOsAppRefName"!]_MemMap.h>
[!ENDIF!]
  [!VAR "XcpConnectionOsAppRefNameList" = "concat($XcpConnectionOsAppRefNameList,' ',$XcpConnectionOsAppRefName,' ')"!]
  [!ENDIF!]
[!ENDLOOP!]
[!ENDIF!]

[!IF "$XcpMemoryAccessAreas"!]

[!VAR "XcpSendingApplicationIteratorList" = "''"!]
[!VAR "XcpReceivingApplicationList" = "''"!]
[!LOOP "(XcpConnectionCfg/*)"!]
  [!VAR "XcpSendingApplicationIterator" = "name(as:ref(./XcpConnectionApplicationRef))"!]
  [!IF "not(contains($XcpSendingApplicationIteratorList,concat(' ',$XcpSendingApplicationIterator,' ')))"!]

    [!VAR "XcpMemProxyOsAppRefNameList" = "''"!]
    [!LOOP "(../../XcpMemoryAccessArea/XcpMemoryAccessArea/*)"!]
      [!VAR "XcpMemProxyOsAppRefName"="name(as:ref(./XcpMemoryAccessAreaApplicationRef))"!]
      [!IF "not(contains($XcpMemProxyOsAppRefNameList,concat(' ',$XcpMemProxyOsAppRefName,' ')))"!]
        [!VAR "XcpReceivingApplication" = "name(as:ref(./XcpMemoryAccessAreaApplicationRef))"!]
        [!IF "not(contains($XcpReceivingApplicationList,concat(' ',$XcpReceivingApplication,' ')))"!]
          [!IF "not(contains($XcpSendingApplicationIterator,$XcpReceivingApplication))"!]
#define XCP_1_XcpMemoryProxy_[!"$XcpMemProxyOsAppRefName"!]_START_SEC_CODE
#include <Xcp_1_XcpMemoryProxy_[!"$XcpMemProxyOsAppRefName"!]_MemMap.h>

FUNC(void, XCP_1_XcpMemoryProxy_[!"$XcpReceivingApplication"!]_CODE) Xcp_1_XcpMemoryProxy_[!"$XcpReceivingApplication"!]_MemoryProxyHandler (void)
{
  Xcp_MemoryProxyDataType data;
  Atomic_value_t partition = Atomics_Load(&Xcp_PartitionOfActiveConnection);
  switch(partition)
  {
  [!LOOP "text:split($XcpConnectionOsAppRefNameList, ' ')"!]
    [!IF ". != $XcpReceivingApplication"!]
    case [!"."!]:
     (void) SchM_Receive_Xcp_1_XcpMemoryProxy_[!"$XcpReceivingApplication"!]_MemoryProxyData_[!"$XcpReceivingApplication"!]_From_[!"."!](&data);
     break;
    [!ENDIF!]
  [!ENDLOOP!]
    default:
      /* nothing to do */
      break;
  }
  Xcp_MemoryProxyHandler(&data);
}

#define XCP_1_XcpMemoryProxy_[!"$XcpMemProxyOsAppRefName"!]_STOP_SEC_CODE
#include <Xcp_1_XcpMemoryProxy_[!"$XcpMemProxyOsAppRefName"!]_MemMap.h>

            [!VAR "XcpMemProxyOsAppRefNameList" = "concat($XcpMemProxyOsAppRefNameList,' ',$XcpMemProxyOsAppRefName,' ')"!]
            [!VAR "XcpReceivingApplicationList" = "concat($XcpReceivingApplicationList,' ',$XcpReceivingApplication,' ')"!]
          [!ENDIF!]
        [!ENDIF!]
      [!ENDIF!]
    [!ENDLOOP!]

  [!ENDIF!]
[!ENDLOOP!]

[!IF "($XcpDaqStimMemoryAccessAreas = 'true') or ($RAMCalloutProxyEnabled = 'true')"!]
[!VAR "XcpSendingApplicationIteratorList" = "''"!]
[!VAR "XcpReceivingApplicationList" = "''"!]
[!LOOP "(XcpMemoryAccessArea/XcpMemoryAccessArea/*)"!]
  [!VAR "XcpSendingApplicationIterator" = "name(as:ref(./XcpMemoryAccessAreaApplicationRef))"!]
  [!IF "not(contains($XcpSendingApplicationIteratorList,concat(' ',$XcpSendingApplicationIterator,' ')))"!]

    [!VAR "XcpMemProxyOsAppRefNameList" = "''"!]
    [!LOOP "(../../../XcpConnectionCfg/*)"!]
      [!VAR "XcpMemProxyOsAppRefName"="name(as:ref(./XcpConnectionApplicationRef))"!]
      [!IF "not(contains($XcpMemProxyOsAppRefNameList,concat(' ',$XcpMemProxyOsAppRefName,' ')))"!]
        [!VAR "XcpReceivingApplication" = "name(as:ref(./XcpConnectionApplicationRef))"!]
        [!IF "not(contains($XcpReceivingApplicationList,concat(' ',$XcpReceivingApplication,' ')))"!]
        [!VAR "XcpSendingApplicationList" = "''"!]
        [!LOOP "(../../XcpMemoryAccessArea/XcpMemoryAccessArea/*)"!]
          [!VAR "XcpSendingApplication" = "name(as:ref(./XcpMemoryAccessAreaApplicationRef))"!]
          [!IF "not(contains($XcpSendingApplicationList,concat(' ',$XcpSendingApplication,' ')))"!]
            [!IF "not(contains($XcpSendingApplication,$XcpReceivingApplication))"!]
#define XCP_1_[!"$XcpReceivingApplication"!]_START_SEC_CODE
#include <Xcp_1_[!"$XcpReceivingApplication"!]_MemMap.h>

FUNC(void, XCP_1_XcpMemoryProxy_[!"$XcpReceivingApplication"!]_CODE) Xcp_1_XcpMemoryProxyResponse_[!"$XcpReceivingApplication"!]_From_[!"$XcpSendingApplication"!]_MemoryProxyResponseHandler (void)
{
  uint16 EventId;
  (void) SchM_Receive_Xcp_1_XcpMemoryProxyResponse_[!"$XcpReceivingApplication"!]_From_[!"$XcpSendingApplication"!]_EventId_[!"$XcpReceivingApplication"!]_From_[!"$XcpSendingApplication"!](&EventId);
  Xcp_MemoryProxyResponseHandler(&EventId);
}

#define XCP_1_[!"$XcpReceivingApplication"!]_STOP_SEC_CODE
#include <Xcp_1_[!"$XcpReceivingApplication"!]_MemMap.h>
              [!VAR "XcpSendingApplicationList" = "concat($XcpSendingApplicationList,' ',$XcpSendingApplication,' ')"!][!//
            [!ENDIF!]
          [!ENDIF!]
        [!ENDLOOP!]
        [!VAR "XcpMemProxyOsAppRefNameList" = "concat($XcpMemProxyOsAppRefNameList,' ',$XcpMemProxyOsAppRefName,' ')"!]
        [!VAR "XcpReceivingApplicationList" = "concat($XcpReceivingApplicationList,' ',$XcpReceivingApplication,' ')"!]
        [!ENDIF!]
      [!ENDIF!]
    [!ENDLOOP!]

  [!ENDIF!]
[!ENDLOOP!]
[!ENDIF!]
[!ENDIF!]

[!ENDSELECT!][!//
/*==================[external function definitions]=========================*/

[!IF "RamOptimizations/XcpEnableDaqOptimization = 'true' and ($XcpNoOfNonDynamicODTEntries > 0 or XcpGeneral/XcpDaqConfigType = 'DAQ_DYNAMIC')"!][!//

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

FUNC_P2VAR(void, XCP_VAR, XCP_CODE) Xcp_GetOdtEntryData
(
  P2VAR(void, AUTOMATIC, XCP_VAR) ptr,
  uint8 offset,
  uint8 field
)
{
  void* ret = NULL_PTR;
[!IF "$XcpNoOfNonDynamicODTEntries > 0 and XcpGeneral/XcpDaqConfigType = 'DAQ_DYNAMIC'"!][!//
  /* Deviation MISRAC2012-2 */
  /* Deviation MISRAC2012-3 */
  if (((usize)(ptr) >= (usize)&Xcp_OdtAddressTableRAM) && ((usize)(ptr) < ((usize)&Xcp_OdtAddressTableRAM + (usize)sizeof(Xcp_OdtAddressTableRAM))))
[!ENDIF!][!//
[!IF "$XcpNoOfNonDynamicODTEntries > 0"!][!//
  {
    /* Deviation MISRAC2012-2 */
    /* Deviation MISRAC2012-3 */
    usize pos = (((usize)(ptr) - (usize)&Xcp_OdtAddressTableRAM)/sizeof(usize)) + (offset);
    switch(field)
    {
      case XCP_ODT_ENTRY_FIELD_ADDRESS:
        ret = (void*) &Xcp_OdtAddressTableRAM[pos];
        break;
      case XCP_ODT_ENTRY_FIELD_LENGTH:
        ret = (void*) &Xcp_OdtLengthTableRAM[pos];
        break;
[!IF "RamOptimizations/XcpEnableBitOffset = 'true'"!]
      case XCP_ODT_ENTRY_FIELD_BITOFFSET:
        ret = (void*) &Xcp_OdtBitOffsetTableRAM[pos];
        break;
[!ENDIF!][!//
      default:
        /* unreachable code */
        break;
    }
  }
[!ENDIF!][!//
[!IF "$XcpNoOfNonDynamicODTEntries > 0 and XcpGeneral/XcpDaqConfigType = 'DAQ_DYNAMIC'"!][!//
  else
[!ENDIF!][!//
[!IF "XcpGeneral/XcpDaqConfigType = 'DAQ_DYNAMIC'"!][!//
  {
    usize pos, base_offset = 0U, field_size = XCP_GET_ODT_ENTRY_ADDRESS_SIZE;
    switch(field)
    {
      case XCP_ODT_ENTRY_FIELD_ADDRESS:
        /* the ODT entry addresses are stored first */
        base_offset = 0U;
        field_size = XCP_GET_ODT_ENTRY_ADDRESS_SIZE;
        break;
      case XCP_ODT_ENTRY_FIELD_LENGTH:
        /* the ODT entry lengths are stored after all the addresses */
        base_offset = (sizeof(uint32)* (Xcp_DynamicInfo.AllocatedOdtEntriesSoFar));
        field_size = XCP_GET_ODT_ENTRY_LENGTH_SIZE;
        break;
[!IF "RamOptimizations/XcpEnableBitOffset = 'true'"!]
      case XCP_ODT_ENTRY_FIELD_BITOFFSET:
        /* the ODT entry bit offsets are stored after all the lengths */
        base_offset = ((sizeof(uint32)+sizeof(uint8))*(Xcp_DynamicInfo.AllocatedOdtEntriesSoFar));
        field_size = XCP_GET_ODT_ENTRY_BITOFFSET_SIZE;
        break;
[!ENDIF!][!//
      default:
        /* unreachable code */
        break;
    }
    /* Deviation MISRAC2012-2 <START> */
    /* Deviation MISRAC2012-3 <START> */
    pos = (usize)Xcp_DynamicInfo.FirstDynamicOdtEntryAddr - (usize)&Xcp_DaqLists.Xcp_DynamicArea +
          base_offset +
          ((((usize)(ptr) - (usize)(Xcp_DynamicInfo.FirstDynamicOdtEntryAddr))/sizeof(usize)) * field_size) +
          ((offset) * field_size);
    /* Deviation MISRAC2012-2 <STOP> */
    /* Deviation MISRAC2012-3 <STOP> */
    ret = (void*) &Xcp_DaqLists.Xcp_DynamicArea[pos];
  }
[!ENDIF!][!//
  return ret;
}

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

[!ENDIF!][!//

[!IF "$XcpBswDistributionEnabled"!]
[!IF "text:tolower($StoreDaq) = 'true'"!]
#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>
[!IF "num:i(count(text:split($XcpConnectionOsAppRefNameList, ' '))) > 1"!]
FUNC(void, XCP_1_[!"$XcpMasterApplication"!]_CODE)  Xcp_1_[!"$XcpMasterApplication"!]_NvMRequestHandler(void)
{
  uint8 PartitionId;
  Atomic_value_t partition = Atomics_Load(&Xcp_PartitionOfActiveConnection);
  switch(partition)
  {
  [!LOOP "text:split($XcpRemoteDestinations, ' ')"!]
    case [!"."!]:
     (void) SchM_Receive_Xcp_1_[!"$XcpMasterApplication"!]_NvMRequest_Master_From_[!"."!](&PartitionId);
     break;
  [!ENDLOOP!]
    default:
      /* Nothing to do */
      break;
  }
  Xcp_NvMRequestHandler();
}

[!ENDIF!]

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

#define XCP_START_SEC_CONST_UNSPECIFIED
#include <Xcp_MemMap.h>

Xcp_SendNvMRequest_FuncPtr Xcp_SendNvMRequest[[!"$XcpNumOsApps"!]] = {
  [!LOOP "(as:modconf('Os')[1]/OsApplication/*)"!][!//
    [!VAR "PartitionName" = "name(.)"!][!//
    [!VAR "NeedsEntry" = "'false'"!][!//
    [!LOOP "(as:modconf('Xcp')[1]/XcpConfig/*[1]/XcpConnectionCfg/*)"!][!//
      [!IF "name(as:ref(./XcpConnectionApplicationRef)) = $PartitionName"!][!//
        [!VAR "NeedsEntry" = "'true'"!][!//
        [!BREAK!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
    [!IF "($NeedsEntry = 'true') and ($PartitionName != $XcpMasterApplication)"!]
      SchM_Send_Xcp_1_[!"$PartitionName"!]_NvMRequest_[!"$PartitionName"!]_To_Master,
    [!ELSE!]
      NULL_PTR,
    [!ENDIF!]
  [!ENDLOOP!]
};

#define XCP_STOP_SEC_CONST_UNSPECIFIED
#include <Xcp_MemMap.h>

[!ENDIF!]
[!ENDIF!]

[!IF "$XcpBswDistributionEnabled and count(text:split($XcpRemoteDestinations,' '))>0"!]
#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

FUNC(Std_ReturnType, XCP_CODE) Xcp_RemoteDispatch_Handler(Xcp_RemoteDispatchType data)
{
  Std_ReturnType ret = E_NOT_OK;
  Atomic_value_t partition = Atomics_Load(&Xcp_PartitionOfActiveConnection);
  uint8 applicationId = (uint8) GetApplicationID();

  if (applicationId == XCP_MASTER_APPLICATION)
  {
[!IF "num:i($NoOfConnections) = '1'"!]
[!VAR "Partition"="text:split($XcpConnectionOsAppRefNameList, ' ')[1]"!]
    if (data.type == XCP_REMOTE_DISPATCH_INIT)
    {
      ret = SchM_Send_Xcp_1_[!"$XcpMasterApplication"!]_RemoteDispatch_Master_To_[!"$Partition"!](&data);
    }
    else
[!ENDIF!]
    {
      switch(partition)
      {
  [!LOOP "text:split($XcpRemoteDestinations, ' ')"!]
        case [!"."!]:
          ret = SchM_Send_Xcp_1_[!"$XcpMasterApplication"!]_RemoteDispatch_Master_To_[!"."!](&data);
          break;
  [!ENDLOOP!]
        default:
          /* nothing to do */
          break;
      }
    }
  }
  else
  {
    uint8 sid;
    switch(data.type)
    {
      case XCP_REMOTE_DISPATCH_SETEVENT:
        sid = XCP_SID_SET_EVENT;
        break;
[!IF "XcpGeneral/XcpSuppressTxSupport = 'true'"!]
      case XCP_REMOTE_DISPATCH_SETTRANSMISSIONMODE:
        sid = XCP_SID_SET_TRANSMISSION_MODE;
        break;
[!ENDIF!]
[!IF "XcpGeneral/XcpEnableXcpControlApi = 'true'"!]
      case XCP_REMOTE_DISPATCH_CONTROL:
        sid = XCP_SID_STATE_CONTROL;
        break;
[!ENDIF!]
      default:
        /* unknown SID */
        sid = 0xFF;
        break;
    }
    (void) Det_ReportRuntimeError(XCP_MODULE_ID, (uint8) applicationId, sid, XCP_E_WRONG_CONTEXT);
  }
  return ret;
}

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

[!FOR "i"="1" TO "num:i(count(text:split($XcpRemoteDestinations, ' ')))"!]
[!VAR "Partition"="text:split($XcpRemoteDestinations, ' ')[position() = $i]"!]
#define XCP_1_[!"$Partition"!]_START_SEC_CODE
#include <Xcp_1_[!"$Partition"!]_MemMap.h>

FUNC(void, XCP_1_[!"$Partition"!]_CODE) Xcp_1_[!"$Partition"!]_RemoteDispatch_[!"$Partition"!] (void)
{
  Xcp_RemoteDispatchType data;
  Std_ReturnType ret;
  const uint8 sid = 0xFF;
  Atomic_value_t partition = Atomics_Load(&Xcp_PartitionOfActiveConnection);

  ret = SchM_Receive_Xcp_1_[!"$Partition"!]_RemoteDispatch_[!"$Partition"!]_From_Master(&data);

  if (ret == E_OK)
  {
    switch(data.type)
    {
      case XCP_REMOTE_DISPATCH_SETEVENT:
        (void) Xcp_SetEvent_Internal(data.p16[0]);
        break;
[!IF "XcpGeneral/XcpSuppressTxSupport = 'true'"!]
      case XCP_REMOTE_DISPATCH_SETTRANSMISSIONMODE:
        (void) Xcp_SetTransmissionMode_Internal(data.p8[0], data.p8[1]);
        break;
[!ENDIF!]
[!IF "XcpGeneral/XcpEnableXcpControlApi = 'true'"!]
      case XCP_REMOTE_DISPATCH_CONTROL:
        (void) Xcp_Control_Internal(data.p8[0]);
        break;
[!ENDIF!]
[!IF "(node:exists(XcpGeneral/XcpCalPagStoreSupported) = 'true') and (text:tolower(XcpGeneral/XcpCalPagStoreSupported) = 'true')"!]
      case XCP_REMOTE_DISPATCH_SETREQSTORECALREQCBK:
        (void) Xcp_SetReqStoreCalReqCbk_Internal(data.p8[0],data.p8[1]);
        break;
[!ENDIF!]
[!IF "num:i($NoOfConnections) = '1'"!]
      case XCP_REMOTE_DISPATCH_INIT:
        (void) Xcp_Init_OnSlave();
        break;
[!ENDIF!]
      default:
        /* Nothing to do */
        break;
    }
  }
  else
  {
    (void) Det_ReportRuntimeError(XCP_MODULE_ID, (uint8) partition, sid, XCP_E_DATA_LOST);
  }
}

#define XCP_1_[!"$Partition"!]_STOP_SEC_CODE
#include <Xcp_1_[!"$Partition"!]_MemMap.h>
[!ENDFOR!]
[!ENDIF!]

#endif /*( (XCP_DEFAULT_XCP_MODULE_STATE == STD_ON) || (XCP_ENABLE_XCP_CONTROL_API == STD_ON) )*/
/*==================[end of file]===========================================*/
