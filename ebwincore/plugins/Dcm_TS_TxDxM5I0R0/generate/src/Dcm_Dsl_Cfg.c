/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=================================================================*/
[!AUTOSPACING!]
#include <ComStack_Types.h>                               /* AUTOSAR standard types */
#include <Dcm_Int.h>
[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmComMMultiCoreSupport = 'true'"!]
#include <SchM_Dcm.h>
[!ENDIF!]
[!INDENT "0"!]
/*==================[macros]=====================================================================*/
[!/* EcuC References of Dcm Rx PDUs */!][!//
[!VAR "DcmRxPduCount" = "num:integer( count( DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*/DcmDslProtocolRx/*/DcmDslProtocolRxPduRef))"!][!//
[!//
[!/* EcuC References of Dcm Tx PDUs */!][!//
[!VAR "DcmTxPduCount" = "num:integer( count( DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*))"!][!//
[!//
[!/*Activation period of main function */!][!//
[!VAR "DcmTaskTime" = "DcmConfigSet/*[1]/DcmGeneral/DcmTaskTime"!][!//
[!//
[!/* Verify and increase the buffer size with the numbers of extra bytes required for routine control service */!][!//
[!VAR "buffersRequiredExraByte" = "':'"!][!//
[!VAR "extraBufferSpaceForRoutineControl" = "0"!][!//
[!VAR "currenttxbuffername" = "''"!][!//
[!VAR "currentrxbuffername" = "''"!][!//
[!VAR "CurrentGenericConnectionIndex" = "0"!][!//
[!/* Loop through all the routines configured */!][!//
[!LOOP "  DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*"!][!//
  [!/* Verify if any start in or stop in or requestresults in signals are variable length and not byte aligned */!][!//
  [!IF "node:exists(node:ref(DcmDspRoutineInfoRef)/DcmDspStartRoutineIn/DcmDspStartRoutineInSignal/*[DcmDspRoutineSignalType = 'VARIABLE_LENGTH' and ((DcmDspRoutineSignalPos mod 8) != 0)])  or node:exists(node:ref(DcmDspRoutineInfoRef)/DcmDspRoutineStopIn/DcmDspRoutineStopInSignal/*[DcmDspRoutineSignalType = 'VARIABLE_LENGTH' and ((DcmDspRoutineSignalPos mod 8) != 0)]) or node:exists(node:ref(DcmDspRoutineInfoRef)/DcmDspRoutineRequestResIn/DcmDspRoutineRequestResInSignal/*[DcmDspRoutineSignalType = 'VARIABLE_LENGTH' and ((DcmDspRoutineSignalPos mod 8) != 0)])"!][!//
    [!/* Loop through all the protocols configured */!][!//
    [!LOOP "../../../DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*"!][!//
      [!/* Check that 0x31 service is enabled and using internal service handler */!][!//
      [!IF "node:exists(node:ref(DcmDslProtocolSIDTable)/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x31')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!][!//
        [!/* Loop through all Rx channels configured */!][!//
        [!LOOP "DcmDslConnection/*/DcmDslProtocolRx/*"!][!//
          [!/* Extract the Rxbuffer name of this protocol */!][!//
          [!VAR "currentrxbuffername" = "node:name(node:ref(DcmDslProtocolRxBufferID))"!][!//
          [!/* Add into buffersRequiredExraByte list */!][!//
          [!IF "not( contains( $buffersRequiredExraByte, concat( ':', $currentrxbuffername, ':')))"!][!//
            [!VAR "buffersRequiredExraByte" = "concat( $buffersRequiredExraByte, $currentrxbuffername, ':')"!][!//
            [!/* Increment extraBufferSpaceForRoutineControl whenever new buffer is added to the list */!][!//
            [!VAR "extraBufferSpaceForRoutineControl" = "$extraBufferSpaceForRoutineControl +1"!][!//
          [!ENDIF!][!//
        [!ENDLOOP!][!/* End of Rx channels loop */!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!/* End of protocols loop */!][!//
  [!ENDIF!][!//
  [!/* Verify if any start out or stop out or routine results out signals are variable length and byte aligned */!][!//
  [!IF "node:exists(node:ref(DcmDspRoutineInfoRef)/DcmDspStartRoutineOut/DcmDspStartRoutineOutSignal/*[DcmDspRoutineSignalType = 'VARIABLE_LENGTH' and ((DcmDspRoutineSignalPos mod 8) != 0)])  or node:exists(node:ref(DcmDspRoutineInfoRef)/DcmDspRoutineStopOut/DcmDspRoutineStopOutSignal/*[DcmDspRoutineSignalType = 'VARIABLE_LENGTH' and ((DcmDspRoutineSignalPos mod 8) != 0)]) or node:exists(node:ref(DcmDspRoutineInfoRef)/DcmDspRoutineRequestResOut/DcmDspRoutineRequestResOutSignal/*[DcmDspRoutineSignalType = 'VARIABLE_LENGTH' and ((DcmDspRoutineSignalPos mod 8) != 0)])"!][!//
    [!/* Loop through all the protocols configured */!][!//
    [!LOOP "../../../DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*"!][!//
      [!/* Check that 0x31 service is enabled and using internal service handler */!][!//
      [!IF "node:exists(node:ref(DcmDslProtocolSIDTable)/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x31')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!][!//
        [!/* Extract the Txbuffer name of this protocol */!][!//
        [!VAR "currenttxbuffername" = "node:name(node:ref(DcmDslProtocolTxBufferID))"!][!//
        [!/* Add into buffersRequiredExraByte list */!][!//
        [!IF "not( contains( $buffersRequiredExraByte, concat( ':', $currenttxbuffername, ':')))"!][!//
          [!VAR "buffersRequiredExraByte" = "concat( $buffersRequiredExraByte, $currenttxbuffername, ':')"!][!//
          [!/* Increment extraBufferSpaceForRoutineControl whenever new buffer is added to the list */!][!//
          [!VAR "extraBufferSpaceForRoutineControl" = "$extraBufferSpaceForRoutineControl +1"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!/* End of protocols loop */!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!/* End of routines loop */!][!//
[!//
[!/*ChannelID holds the different 'DcmDslProtocolRxChannelId' configured. This will be used for the
generation of the array Dcm_RxComMChannelIDs[] and also for generating the index in Dcm_RxPduIdConfig*/!][!//
[!VAR "ChannelID" =  "' '"!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*/DcmDslProtocolRx/*"!][!//
  [!IF "not( contains( $ChannelID, concat( ' ', ./DcmDslProtocolRxChannelId,' ')))"!][!//
    [!VAR "ChannelID" = "concat( $ChannelID,' ',./DcmDslProtocolRxChannelId,' ')"!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!//

/*==================[type definitions]===========================================================*/

/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/

/*==================[internal constants]=========================================================*/

/*==================[internal data]==============================================================*/

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/* The configuration array for RxPduIDs.
 *
 * A Dcm Connection basically receives data from a number of RxPduIDs and transmits it to only ONE TxPduId.
 * In order to identify which Connection to use we will always need to use the RxPduId of the incoming
 * reception and from this infer all the data which we need to further handle reception, processing and transmission.
 *
 */

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.BufferManager.ConfigurationDataElements,1 */
/* Initialize global array holding the configuration for each buffer which is referenced from anywhere */
CONST(Dcm_BufferConfigType, DCM_CONST) Dcm_BufferConfig[DCM_NUM_CONFIGURED_BUFFERS] =
{
[!VAR "BufferPos" = "0"!]
[!VAR "DefaultBufferPos" = "0"!]
[!LOOP "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslBuffer/*"!]
[!IF "node:contains(node:refs(../../DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*/DcmDslProtocolRx/*/DcmDslProtocolRxBufferID) | node:refs(../../DcmDslProtocol/DcmDslProtocolRow/*/DcmDslProtocolTxBufferID), node:current())"!]
  [!VAR "LastBufferPos" = "$BufferPos"!] /* The index used for pointer to buffer */
  [!INDENT "2"!]
  {
    [!INDENT "4"!]
    [!IF "( contains( $buffersRequiredExraByte, concat( ':', node:name(.), ':')))"!]
      [!VAR "BufferPos" = "$BufferPos + DcmDslBufferSize + 1"!]
      /* Size of buffer - Used extra 1 byte for 0x31 service */
      [!"num:integer(DcmDslBufferSize)"!]U,
    [!ELSE!]
      [!VAR "BufferPos" = "$BufferPos + DcmDslBufferSize "!]
      /* Size of buffer */
      [!"num:integer(DcmDslBufferSize)"!]U,
    [!ENDIF!]
     /* Pointer to buffer */
    &Dcm_Buffers[[!"num:integer( $LastBufferPos)"!]],
    [!IF "../../DcmDslEnableTxConfirmationNotification = 'true'"!]
      [!IF "node:contains(node:refs(../../DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*/DcmDslProtocolRx/*/DcmDslProtocolRxBufferID), node:current())"!]
        /* Pointer to default buffer */
        &Dcm_DefaultBuffers[[!"num:integer( $DefaultBufferPos)"!]]
        [!VAR "DefaultBufferPos" = "$DefaultBufferPos + DcmDslBufferSize"!]
      [!ELSE!]
        /* Pointer to default buffer */
        NULL_PTR
      [!ENDIF!]
    [!ENDIF!]
    [!ENDINDENT!]
  },
  [!ENDINDENT!]
[!ENDIF!]
[!ENDLOOP!]
};

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.RxConnection.ConfigurationDataElements,1 */
/* Initialize global array holding the configuration for each RxPduId */
CONST(Dcm_RxPduIdConfigType, DCM_CONST) Dcm_RxPduIdConfig[DCM_NUM_RX_PDU_ID] =
{
[!VAR "i" = "0"!]
[!FOR "RxPduId" = "0" TO "$DcmRxPduCount - 1"!]
  [!SELECT "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*/DcmDslProtocolRx/*[DcmDslProtocolRxPduId = $RxPduId]"!]
    [!FOR "i" = "1" TO "count(text:split($ChannelID))"!]
      [!IF "DcmDslProtocolRxChannelId = text:split($ChannelID)[position() = $i]"!]
      [!BREAK!]
    [!ENDIF!]
    [!ENDFOR!]
    [!VAR "ChannelIdIndex" = "num:i($i - 1)"!][!//
    [!VAR "BufferIdx" = "0"!]
    [!VAR "CurrentIdx" = "num:integer( as:ref( DcmDslProtocolRxBufferID)/@index)"!]
    [!LOOP "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslBuffer/*[@index < $CurrentIdx]"!]
      [!IF "node:contains(node:refs(../../DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*/DcmDslProtocolRx/*/DcmDslProtocolRxBufferID) | node:refs(../../DcmDslProtocol/DcmDslProtocolRow/*/DcmDslProtocolTxBufferID), node:current())"!]
        [!VAR "BufferIdx" = "$BufferIdx + 1"!]
      [!ENDIF!]
    [!ENDLOOP!]
    [!VAR "BufferId" = "num:i($BufferIdx)"!][!//
    [!VAR "ConnId" = "-1"!]
    [!VAR "ConnId_i" = "0"!]
    [!LOOP "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[.='DcmDslMainConnection']"!]
      [!IF "node:exists(./DcmDslProtocolRx/*/DcmDslProtocolRxPduId[.=number($RxPduId)])"!]
        [!VAR "ConnId" = "$ConnId_i"!]
        [!BREAK!]
      [!ENDIF!]
      [!VAR "ConnId_i" = "$ConnId_i+1"!]
    [!ENDLOOP!]
    [!INDENT "2"!]
  {
      [!INDENT "4"!]
    /* This is the configuration of RxPduId [!"$RxPduId"!] */
    [!IF "node:value(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmGeneral/DcmGenericConnectionUsage) = 'true'"!]
      [!IF "node:exists(node:refs(DcmDslProtocolRxPduRef)/MetaDataTypeRef)"!]
        [!VAR "MetaDataRef" = "node:refs(DcmDslProtocolRxPduRef)/MetaDataTypeRef"!]
        [!VAR "hasSourceAddress" = "text:contains(node:refs($MetaDataRef)/*/*/MetaDataItemType, 'SOURCE_ADDRESS_16')"!]
        [!VAR "hasTargetAddress" = "text:contains(node:refs($MetaDataRef)/*/*/MetaDataItemType, 'TARGET_ADDRESS_16')"!]
        [!IF "node:exists($MetaDataRef) and $hasSourceAddress = 'true' and $hasTargetAddress = 'true'"!]
          [!"num:i(node:ref(DcmDslProtocolRxPduRef)/PduId)"!]U, /* Rx Ecu PduId */
        [!ELSE!]
          DCM_NOT_CONFIGURED_ECUC_PDUID, /* Ecu PduId not configured */
        [!ENDIF!]
      [!ELSE!]
        DCM_NOT_CONFIGURED_ECUC_PDUID, /* Metadata not configured for this main connection */
      [!ENDIF!]
    [!ELSE!]
      DCM_NOT_CONFIGURED_ECUC_PDUID, /* GenericConnectionUsage is disabled */
    [!ENDIF!]
    [!"DcmDslProtocolRxAddrType"!], /* AddressingType */
    [!"$BufferId"!]U, /* BufferIndex */
    [!"num:i($ConnId)"!]U, /* MainConnectionIndex */
    [!"$ChannelIdIndex"!]U  /* ChannelIdConfigIndex */
      [!ENDINDENT!]
  },
    [!ENDINDENT!]
  [!ENDSELECT!]
[!ENDFOR!]
};
[!//
[!VAR "RoeConnectionsPresent" = "'FALSE'"!]
[!VAR "PeriodicTransmissionsPresent" = "'FALSE'"!]
[!//
[!IF "(node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[(.='DcmDslMainConnection') and (node:refvalid(./DcmDslPeriodicTransmissionConRef))]))"!]
  [!VAR "PeriodicTransmissionsPresent" = "'TRUE'"!]
[!ENDIF!]
[!//
[!IF "(node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[(.='DcmDslMainConnection') and (node:refvalid(./DcmDslROEConnectionRef))]))"!]
  [!VAR "RoeConnectionsPresent" = "'TRUE'"!]
[!ENDIF!]

/* !LINKSTO Dcm.Dsn.DiagnosticSessionLayer.ConfigurationDataElements,1 */
/* Initialize global array holding the protocol configuration */
CONST(Dcm_ProtocolConfigType, DCM_CONST) Dcm_ProtocolConfig[DCM_NUM_PROTOCOLS] =
{
  [!LOOP "DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*"!][!//
  [!WS "2"!]
  {
    [!INDENT "4"!]
      [!"num:integer(floor(DcmTimStrP2ServerAdjust div $DcmTaskTime))"!]U, /*P2ServerAdjustTime*/
      [!"num:integer(floor(DcmTimStrP2StarServerAdjust div $DcmTaskTime))"!]U, /*P2StarServerAdjustTime */
      [!"num:integer(ceiling(DcmDslProtocolPreemptTimeout div $DcmTaskTime))"!]U, /* Protocol preemption timeout */
      [!VAR "BufferIdx" = "0"!]
      [!VAR "CurrentIdx" = "num:integer( as:ref( DcmDslProtocolTxBufferID)/@index)"!]
      [!LOOP "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslBuffer/*[@index < $CurrentIdx]"!]
        [!IF "node:contains(node:refs(../../DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*/DcmDslProtocolRx/*/DcmDslProtocolRxBufferID) | node:refs(../../DcmDslProtocol/DcmDslProtocolRow/*/DcmDslProtocolTxBufferID), node:current())"!]
          [!VAR "BufferIdx" = "$BufferIdx + 1"!]
        [!ENDIF!]
      [!ENDLOOP!]
      [!"num:integer($BufferIdx)"!]U, /* Tx buffer Index */
      [!"DcmDslProtocolID"!], /* Protocol ID */
      [!"DcmDslProtocolPriority"!]U, /* Protocol priority */
      [!"DcmDslProtocolStackNumber"!]U, /* Protocol stack number */
      /* !LINKSTO Dcm.LinktoServiceTable.ProtocolInit,2 */
      [!"num:integer( as:ref( DcmDslProtocolSIDTable)/DcmDsdSidTabId)"!]U, /* Dcm_SidTabConfigType */
      [!"text:toupper(DcmDslProtocolEndiannessConvEnabled)"!], /* endianness Conversion */
      [!"text:toupper(DcmSendRespPendOnTransToBoot)"!], /* Send RP OnTransToBoot */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x00,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x01,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x02,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x03,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x04,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x06,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x07,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x08,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x09,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x0A,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.SupressNRC.OBD_0x0B-0x0F,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x00,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x01,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x02,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x03,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x04,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x06,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x07,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x08,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x09,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x0A,1 */
      /* !LINKSTO Dcm.EB.ObdOptionalNrc.DoNotSupressNRC.OBD_0x0B-0x0F,1 */
      [!IF "node:exists(./DcmDemClientRef)"!]
        [!"text:toupper(DcmDslObdNRCResponseSupression)"!], /* Supress Service Not Supported NRC */
        [!"as:ref(DcmDemClientRef)/DemClientId"!]U /* DemClientId */
      [!ELSE!]
        [!"text:toupper(DcmDslObdNRCResponseSupression)"!] /* Supress Service Not Supported NRC */
      [!ENDIF!]
    [!ENDINDENT!]
  [!WS "2"!]
  },
[!ENDLOOP!][!//
};

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.TxConnection.ConfigurationDataElements,1 */
/* !LINKSTO Dcm.Eb.MainConnectionConfig,1 */
/* !LINKSTO Dcm.Eb.Virtual.TxConnectionPduId,1 */
/* Initialize global array holding the configuration for each DcmDslMainConnection */
CONST(Dcm_MainConnectionConfigType, DCM_CONST) Dcm_MainConnectionConfig[DCM_NUM_MAIN_CONNECTIONS] =
{
[!VAR "TotalRealMainConnectionTxConnectionPduId" = "num:i(count(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[(.='DcmDslMainConnection') and (node:exists(DcmDslProtocolTx))]))"!]
[!VAR "TotalRealPeriodicTxConnectionPduId" = "num:i(count(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[.='DcmDslPeriodicTransmission']/DcmDslPeriodicConnection/*))"!]
[!VAR "TotalRealROETxConnectionPduId" = "num:i(count(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[(.='DcmDslResponseOnEvent') and (node:exists(./DcmDslRoeTxPduRef)) and (node:refvalid(./DcmDslRoeTxPduRef)) ]))"!]
[!VAR "TotalNumberOfRealConnections" = "num:i($TotalRealMainConnectionTxConnectionPduId + $TotalRealPeriodicTxConnectionPduId + $TotalRealROETxConnectionPduId)"!]
[!VAR "VirtualTxConnectionPduId" = "num:integer($TotalNumberOfRealConnections)"!]
[!LOOP "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[.='DcmDslMainConnection']"!]
  [!INDENT "2"!]
  {
    [!INDENT "4"!]
      [!/* This is the configuration of DcmDslMainConnection  [!"as:name(.)"!] of DcmDslProtocol [!"as:name(../../.)"!] */!][!//
      [!IF "(node:exists(DcmDslProtocolTx))"!]
        [!"asc:getPduId( 'PduR','ForUpperLayer','Tp','Tx', asc:getPdus('PduR','ForUpperLayer','Tp','Tx',1,./DcmDslProtocolTx/DcmDslProtocolTxPduRef  )[1]) "!]U, /* TxPduId */
        [!"./DcmDslProtocolTx/DcmDslTxConfirmationPduId"!]U, /* TxConfirmationPduId*/
      [!ELSE!]/* !LINKSTO Dcm.Dsn.DiagnosticSessionLayer.VirtualTX,1 */
        DCM_VIRTUAL_TX_CONNECTION, /* This value is not used for virtual connections. */
        [!"num:integer($VirtualTxConnectionPduId)"!]U, /* TxConfirmationPduId*/
        [!VAR "VirtualTxConnectionPduId" = "$VirtualTxConnectionPduId + 1"!]
      [!ENDIF!]
      [!IF "node:value(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmGeneral/DcmGenericConnectionUsage) = 'true'"!]
        [!IF "node:exists(node:refs(DcmDslProtocolRx/*[1]/DcmDslProtocolRxPduRef)/MetaDataTypeRef)"!]
          [!VAR "MetaDataRef" = "node:refs(DcmDslProtocolRx/*[1]/DcmDslProtocolRxPduRef)/MetaDataTypeRef"!]
          [!VAR "hasSourceAddress" = "text:contains(node:refs($MetaDataRef)/*/*/MetaDataItemType, 'SOURCE_ADDRESS_16')"!]
          [!VAR "hasTargetAddress" = "text:contains(node:refs($MetaDataRef)/*/*/MetaDataItemType, 'TARGET_ADDRESS_16')"!]
          [!IF "node:exists($MetaDataRef) and $hasSourceAddress = 'true' and $hasTargetAddress = 'true'"!]
            [!"num:i(node:ref(./DcmDslProtocolTx/DcmDslProtocolTxPduRef)/PduId)"!]U, /* Tx Ecu PduId */
          [!ELSE!]
            DCM_NOT_CONFIGURED_ECUC_PDUID, /* Ecu PduId not configured */
          [!ENDIF!]
        [!ELSE!]
          DCM_NOT_CONFIGURED_ECUC_PDUID, /* Metadata not configured for this main connection */
        [!ENDIF!]
      [!ELSE!]
       DCM_NOT_CONFIGURED_ECUC_PDUID, /* GenericConnectionUsage is disabled */
     [!ENDIF!]
      [!"num:i(./DcmDslProtocolRxTesterSourceAddr)"!], /* TesterSourceAddress */
      [!"num:i(../../@index)"!], /* ProtocolIndex */
      [!IF "$RoeConnectionsPresent = 'TRUE'"!]
        [!IF "(node:exists(DcmDslROEConnectionRef)) and (node:refvalid(DcmDslROEConnectionRef))"!]
          [!VAR "CurrentRoeConnectionRefPath" = "as:path(node:ref(DcmDslROEConnectionRef))"!]
          [!VAR "CurrentRoeConnectionIndex" = "0"!]
          [!/* [!VAR "CurrentRoeConnectionName" = "as:name(node:ref(DcmDslROEConnectionRef))"!] */!][!//
          [!/* [!VAR "CurrentRoeConnectionProtocolName" = "as:name(node:ref(DcmDslROEConnectionRef)/../../.)"!] */!][!//
          [!LOOP "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[.='DcmDslResponseOnEvent']"!]
[!//
              [!IF "as:path(node:current()) = $CurrentRoeConnectionRefPath"!]
                [!"num:i($CurrentRoeConnectionIndex)"!], /* RoeConnectionIndex in Dcm_RoeConnectionConfig[] */
                [!/* RoeConnectionIndex, index of DcmDslResponseOnEvent connection [!"$CurrentRoeConnectionName"!] of DcmDslProtocol [!"$CurrentRoeConnectionProtocolName"!] */!][!//
              [!ELSE!]
                [!VAR "CurrentRoeConnectionIndex" = "$CurrentRoeConnectionIndex + 1"!]
              [!ENDIF!]
[!//
          [!ENDLOOP!]
        [!ELSE!]
    DCM_INVALID_ROE_CONNECTION, /* RoeConnectionIndex, invalid, as this connection does not reference a DcmDslResponseOnEvent connection */
        [!ENDIF!]
      [!ENDIF!]
      [!IF "$PeriodicTransmissionsPresent = 'TRUE'"!]
        [!IF "(node:exists(DcmDslPeriodicTransmissionConRef)) and (node:refvalid(DcmDslPeriodicTransmissionConRef))"!]
          [!VAR "CurrentPeriodicTransmissionRefPath" = "as:path(node:ref(DcmDslPeriodicTransmissionConRef))"!]
          [!VAR "CurrentPeriodicTransmissionIndex" = "0"!]
          [!/*[!VAR "CurrentPeriodicTransmissionName" = "as:name(node:ref(DcmDslPeriodicTransmissionConRef))"!]*/!][!//
          [!/*[!VAR "CurrentPeriodicTransmissionProtocolName" = "as:name(node:ref(DcmDslPeriodicTransmissionConRef)/../../.)"!]*/!][!//
          [!LOOP "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[.='DcmDslPeriodicTransmission']"!]
            [!IF "as:path(node:current()) = $CurrentPeriodicTransmissionRefPath"!]
              [!"num:i($CurrentPeriodicTransmissionIndex)"!], /* PeriodicTransmissionIndex in Dcm_PeriodicTransmissionConfig[] */
              [!/* PeriodicTransmissionIndex, index of DcmDslPeriodicTransmission connection [!"$CurrentPeriodicTransmissionName"!] of DcmDslProtocol [!"$CurrentPeriodicTransmissionProtocolName"!] */!][!//
            [!ELSE!]
              [!VAR "CurrentPeriodicTransmissionIndex" = "$CurrentPeriodicTransmissionIndex + 1"!]
            [!ENDIF!]
          [!ENDLOOP!]
        [!ELSE!]
    DCM_INVALID_PERIODIC_TRANSMISSION, /* PeriodicTransmissionIndex, invalid, as this connection does not reference a DcmDslPeriodicTransmission connection */
        [!ENDIF!]
      [!ENDIF!]
      [!IF "node:value(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmGeneral/DcmGenericConnectionUsage) = 'true'"!]
        [!IF "node:exists(node:refs(DcmDslProtocolRx/*[1]/DcmDslProtocolRxPduRef)/MetaDataTypeRef)"!]
          [!VAR "MetaDataRef" = "node:refs(DcmDslProtocolRx/*[1]/DcmDslProtocolRxPduRef)/MetaDataTypeRef"!]
          [!VAR "hasSourceAddress" = "text:contains(node:refs($MetaDataRef)/*/*/MetaDataItemType, 'SOURCE_ADDRESS_16')"!]
          [!VAR "hasTargetAddress" = "text:contains(node:refs($MetaDataRef)/*/*/MetaDataItemType, 'TARGET_ADDRESS_16')"!]
          [!IF "node:exists($MetaDataRef) and $hasSourceAddress = 'true' and $hasTargetAddress = 'true'"!]
            [!"num:i($CurrentGenericConnectionIndex)"!]U, /* Generic Connection index in Dcm_PduInfo[]*/
            [!VAR "CurrentGenericConnectionIndex" = "$CurrentGenericConnectionIndex + 1"!]
            TRUE /* Metadata is configured for this main connection. */
          [!ELSE!]
            DCM_NUM_GENERIC_CONNECTIONS,
            FALSE /* Metadata is not configured for this main connection. */
          [!ENDIF!]
        [!ELSE!]
          DCM_NUM_GENERIC_CONNECTIONS,
          FALSE /* Metadata is not configured for this main connection. */
        [!ENDIF!]
      [!ELSE!]
        DCM_NUM_GENERIC_CONNECTIONS,
        FALSE /* GenericConnectionUsage is disabled. */
      [!ENDIF!]
    [!ENDINDENT!]
  },
  [!ENDINDENT!]
[!ENDLOOP!]
};

[!IF "$RoeConnectionsPresent = 'TRUE'"!]
#if (STD_ON == DCM_DSP_USE_ROESERVICES)
/* Initialize global array holding the configuration for each DcmDslResponseOnEvent connection which is also referenced from a DcmDslMainConnection */
CONST(Dcm_RoeConnectionConfigType, DCM_CONST) Dcm_RoeConnectionConfig[DCM_NUM_ROE_CONNECTIONS] =
{
  [!LOOP "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[.='DcmDslResponseOnEvent']"!]
    [!INDENT "2"!]
    [!IF "node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[(.='DcmDslMainConnection') and (node:refexists(DcmDslROEConnectionRef)) and (as:path(node:ref(DcmDslROEConnectionRef)) = as:path(node:current()))])"!]
      [!/* DcmDslResponseOnEvent connection [!"as:name(.)"!] of DcmDslProtocol [!"as:name(../../.)"!] */!][!//
  {
        [!INDENT "4"!]
        [!IF "node:refexists(DcmDslRoeTxPduRef)"!]
          TRUE, /* The values of DcmDslRoeTxPduRef and DcmDslRoeTxConfirmationPduId are valid */
    [!"asc:getPduId( 'PduR','ForUpperLayer','Tp','Tx', asc:getPdus('PduR','ForUpperLayer','Tp','Tx',1,DcmDslRoeTxPduRef)[1])"!]U, /* TxPduId */
    [!"DcmDslRoeTxConfirmationPduId"!]U, /* ConfirmationTxPduId */
    [!IF "node:exists(node:refs(DcmDslRoeTxPduRef)/MetaDataTypeRef)"!]
      [!VAR "MetaDataRef" = "node:refs(DcmDslRoeTxPduRef)/MetaDataTypeRef"!]
      [!VAR "hasSourceAddress" = "text:contains(node:refs($MetaDataRef)/*/*/MetaDataItemType, 'SOURCE_ADDRESS_16')"!]
      [!VAR "hasTargetAddress" = "text:contains(node:refs($MetaDataRef)/*/*/MetaDataItemType, 'TARGET_ADDRESS_16')"!]
      [!IF "$hasSourceAddress = 'true' and $hasTargetAddress = 'true'"!]
        [!"num:i($CurrentGenericConnectionIndex)"!]U, /* Generic Connection index in Dcm_PduInfo[]*/
        [!VAR "CurrentGenericConnectionIndex" = "$CurrentGenericConnectionIndex + 1"!]
      [!ELSE!]
        0U,
      [!ENDIF!]
    [!ELSE!]
      0U, /* Invalid, as this DcmDslRoesponseOnEvent has no MetaDataConfigured */
    [!ENDIF!]
        [!ELSE!]
          FALSE,  /* The values of DcmDslRoeTxPduRef and DcmDslRoeTxConfirmationPduId are NOT valid */
    0U, /* Invalid, as this DcmDslRoesponseOnEvent is referenced by a MainConnection using TYPE1 ROE */
    0U, /* Invalid, as this DcmDslRoesponseOnEvent is referenced by a MainConnection using TYPE1 ROE */
    0U, /* Invalid, as this DcmDslRoesponseOnEvent is referenced by a MainConnection using TYPE1 ROE */
        [!ENDIF!]
    [!"num:i(../../@index)"!]U /* ProtocolIndex */
        [!ENDINDENT!]
  },
      [!ENDIF!]
    [!ENDINDENT!]
  [!ENDLOOP!]
};
#endif /* #if (STD_ON == DCM_DSP_USE_ROESERVICES) */
[!ENDIF!]

[!IF "$PeriodicTransmissionsPresent = 'TRUE'"!]
/* !LINKSTO Dcm.EB.PeriodicTransmission.TYPE2.TxPduId,1 */
  [!VAR "CurrentPeriodicConnectionIndex" = "0"!]
[!//
/* Initialize global array holding the configuration for each DcmDslPeriodicTransmission which is also referenced from a DcmDslMainConnection */
CONST(Dcm_PeriodicTransmissionConfigType, DCM_CONST) Dcm_PeriodicTransmissionConfig[DCM_NUM_PERIODIC_TRANSMISSIONS] =
{
  [!LOOP "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[.='DcmDslPeriodicTransmission']"!]
    [!INDENT "2"!]
      [!IF "node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[(.='DcmDslMainConnection') and (node:refexists(DcmDslPeriodicTransmissionConRef)) and (as:path(node:ref(DcmDslPeriodicTransmissionConRef)) = as:path(node:current()))])"!]
      [!/* DcmDslPeriodicTransmission [!"as:name(.)"!] of DcmDslProtocol [!"as:name(../../.)"!] */!][!//
  {
        [!INDENT "4"!]
          [!IF "node:exists(./DcmDslPeriodicConnection/*)"!]
    [!"num:i(count(./DcmDslPeriodicConnection/*))"!]U, /* NumberOfPeriodicConnections */
    [!"num:i($CurrentPeriodicConnectionIndex)"!]U, /* IndexOfFirstPeriodicConnection */
            [!VAR "CurrentPeriodicConnectionIndex" = "$CurrentPeriodicConnectionIndex + num:i(count(./DcmDslPeriodicConnection/*))"!]
          [!ELSE!]
    0U, /* Invalid, as this DcmDslPeriodicTransmission is referenced by a MainConnection using TYPE1 Periodic responses */
    0U, /* Invalid, as this DcmDslPeriodicTransmission is referenced by a MainConnection using TYPE1 Periodic responses */
          [!ENDIF!]
    [!"num:i(../../@index)"!]U /* ProtocolIndex */
        [!ENDINDENT!]
  },
      [!ENDIF!]
    [!ENDINDENT!]
  [!ENDLOOP!]
};

#if (DCM_NUM_PERIODIC_CONNECTIONS > 0)
/* Initialize global array holding the configuration for each DcmDslPeriodicConnection connection which belongs to a
  DcmDslPeriodicTransmission connection referenced from a DcmDslMainConnection */
CONST(Dcm_PeriodicConnectionConfigType, DCM_CONST) Dcm_PeriodicConnectionConfig[DCM_NUM_PERIODIC_CONNECTIONS] =
{
  [!LOOP "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[.='DcmDslPeriodicTransmission']/DcmDslPeriodicConnection/*"!]
    [!INDENT "2"!]
      [!IF "node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[(.='DcmDslMainConnection') and (node:refexists(DcmDslPeriodicTransmissionConRef)) and (as:path(node:ref(DcmDslPeriodicTransmissionConRef)) = as:path((node:current())/../../.))])"!]
      [!/* DcmDslPeriodicConnection [!"as:name(.)"!] of
           DcmDslPeriodicTransmission connection [!"as:name(./../.)"!] belonging to
           DcmDslProtocol [!"as:name(./../../../.)"!] */!][!//
  {
        [!INDENT "4"!]
    [!"asc:getPduId( 'PduR','ForUpperLayer','Tp','Tx', asc:getPdus('PduR','ForUpperLayer','Tp','Tx',1,DcmDslPeriodicTxPduRef)[1])"!]U, /* TxPduId */
    [!"DcmDslPeriodicTxConfirmationPduId"!]U, /* ConfirmationTxPduId */
    [!IF "node:exists(node:refs(DcmDslPeriodicTxPduRef)/MetaDataTypeRef)"!]
      [!VAR "MetaDataRef" = "node:refs(DcmDslPeriodicTxPduRef)/MetaDataTypeRef"!]
      [!VAR "hasSourceAddress" = "text:contains(node:refs($MetaDataRef)/*/*/MetaDataItemType, 'SOURCE_ADDRESS_16')"!]
      [!VAR "hasTargetAddress" = "text:contains(node:refs($MetaDataRef)/*/*/MetaDataItemType, 'TARGET_ADDRESS_16')"!]
      [!IF "$hasSourceAddress = 'true' and $hasTargetAddress = 'true'"!]
        [!"num:i($CurrentGenericConnectionIndex)"!]U /* Generic Connection index in Dcm_PduInfo[]*/
       [!VAR "CurrentGenericConnectionIndex" = "$CurrentGenericConnectionIndex + 1"!]
      [!ELSE!]
        0U /* Invalid, as this DcmDslPeriodicConnection has no MetaDataConfigured */
      [!ENDIF!]
    [!ELSE!]
      0U /* Invalid, as this DcmDslPeriodicConnection has no MetaDataConfigured */
    [!ENDIF!]
        [!ENDINDENT!]
  },
      [!ENDIF!]
    [!ENDINDENT!]
  [!ENDLOOP!]
};
#endif /* #if (DCM_NUM_PERIODIC_CONNECTIONS > 0) */
[!ENDIF!]
[!//
#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

#define DCM_START_SEC_CONST_8
#include <Dcm_MemMap.h>

/* Array holding the channel identifiers configured and the current state the channel is in */
CONST(NetworkHandleType, DCM_CONST) Dcm_RxComMChannelIDs[DCM_NUM_RX_COMM_CHANNELS] =
{
[!FOR "i" = "1" TO "count(text:split($ChannelID))"!]
  [!WS "2"!][!"text:split($ChannelID)[position() = $i]"!]U,
[!ENDFOR!]
};

#define DCM_STOP_SEC_CONST_8
#include <Dcm_MemMap.h>
/*==================[external constants]=========================================================*/

/*==================[external data]==============================================================*/

[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmComMMultiCoreSupport = 'true'"!]
#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

CONST(Dcm_DiagnosticNotificationLookUpType, DCM_CONST) Dcm_DiagnosticNotificationLookUpTable[DCM_NUM_RX_COMM_CHANNELS] =
{
[!LOOP "node:order(node:refs(as:modconf('Dcm')[1]/DcmConfigSet/*/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*/DcmDslProtocolRx/*/DcmDslProtocolRxComMChannelRef), 'ComMChannelId')"!]
  [!INDENT "2"!]
  {
    [!INDENT "4"!]
    [!"ComMChannelId"!],
    &SchM_Call_Dcm_RequiredCSEntry_ComM_DCM_ActiveDiagnostic_[!"node:name(.)"!],
    &SchM_Call_Dcm_RequiredCSEntry_ComM_DCM_InactiveDiagnostic_[!"node:name(.)"!],
    [!ENDINDENT!]
  },
  [!ENDINDENT!]
[!ENDLOOP!]
};

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>
[!ENDIF!]
[!//
[!IF "node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/./DcmDemClientRef)"!]
#define DCM_START_SEC_CONST_8
#include <Dcm_MemMap.h>

/* Array holding the Dem Client Ids referenced by Dcm */
[!VAR "DemClientId" =  "' '"!]
[!//
[!LOOP "DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*"!]
[!//
  [!IF "not( contains( $DemClientId, concat( ' ', node:ref(DcmDemClientRef)/DemClientId,' ')))"!]
[!//
    [!VAR "DemClientId" = "concat( $DemClientId,' ',node:ref(DcmDemClientRef)/DemClientId,' ')"!]
[!//
  [!ENDIF!][!//
[!ENDLOOP!][!//
CONST(uint8, DCM_CONST) Dcm_DemClientIds[DCM_NUM_DEM_CLIENTS] =
{
[!FOR "i" = "1" TO "count(text:split($DemClientId))"!]
  [!WS "2"!][!"text:split($DemClientId)[position() = $i]"!]U,
[!ENDFOR!]
};

#define DCM_STOP_SEC_CONST_8
#include <Dcm_MemMap.h>
[!ENDIF!]
[!//

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/
[!ENDINDENT!]

/*==================[end of file]================================================================*/
