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

#ifndef DCM_DSL_CFG_H
#define DCM_DSL_CFG_H

/*==================[includes]===================================================================*/

[!AUTOSPACING!]
#include <Std_Types.h>
[!INCLUDE "Dcm_common.m"!]

/*==================[macros]=====================================================================*/

/* Allocate memory of buffers configured for the Dcm module */
[!/* Verify and increase the buffer size with the numbers of extra bytes required for routine control service */!][!//
[!INDENT "0"!]
[!VAR "buffersRequiredExraByte" = "':'"!]
[!VAR "extraBufferSpaceForRoutineControl" = "0"!]
[!VAR "currenttxbuffername" = "''"!]
[!VAR "currentrxbuffername" = "''"!]
[!//
[!/* Loop through all the routines configured */!][!//
[!LOOP "  DcmConfigSet/*[1]/DcmDsp/DcmDspRoutine/*"!]
  [!/* Verify if any start in or stop in or requestresults in signals are variable length and not byte aligned */!][!//
  [!IF "node:exists(node:ref(DcmDspRoutineInfoRef)/DcmDspStartRoutineIn/DcmDspStartRoutineInSignal/*[DcmDspRoutineSignalType = 'VARIABLE_LENGTH' and ((DcmDspRoutineSignalPos mod 8) != 0)])  or node:exists(node:ref(DcmDspRoutineInfoRef)/DcmDspRoutineStopIn/DcmDspRoutineStopInSignal/*[DcmDspRoutineSignalType = 'VARIABLE_LENGTH' and ((DcmDspRoutineSignalPos mod 8) != 0)]) or node:exists(node:ref(DcmDspRoutineInfoRef)/DcmDspRoutineRequestResIn/DcmDspRoutineRequestResInSignal/*[DcmDspRoutineSignalType = 'VARIABLE_LENGTH' and ((DcmDspRoutineSignalPos mod 8) != 0)])"!]
    [!/* Loop through all the protocols configured */!][!//
    [!LOOP "../../../DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*"!]
      [!/* Check that 0x31 service is enabled and using internal service handler */!][!//
      [!IF "node:exists(node:ref(DcmDslProtocolSIDTable)/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x31')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]
        [!/* Loop through all Rx channels configured */!][!//
        [!LOOP "DcmDslConnection/*/DcmDslProtocolRx/*"!]
          [!/* Extract the Rxbuffer name of this protocol */!][!//
          [!VAR "currentrxbuffername" = "node:name(node:ref(DcmDslProtocolRxBufferID))"!]
          [!/* Add into buffersRequiredExraByte list */!][!//
          [!IF "not( contains( $buffersRequiredExraByte, concat( ':', $currentrxbuffername, ':')))"!]
            [!VAR "buffersRequiredExraByte" = "concat( $buffersRequiredExraByte, $currentrxbuffername, ':')"!]
            [!/* Increment extraBufferSpaceForRoutineControl whenever new buffer is added to the list */!][!//
            [!VAR "extraBufferSpaceForRoutineControl" = "$extraBufferSpaceForRoutineControl +1"!]
          [!ENDIF!]
        [!ENDLOOP!][!/* End of Rx channels loop */!][!//
      [!ENDIF!]
    [!ENDLOOP!][!/* End of protocols loop */!][!//
  [!ENDIF!]
  [!/* Verify if any start out or stop out or routine results out signals are variable length and byte aligned */!][!//
  [!IF "node:exists(node:ref(DcmDspRoutineInfoRef)/DcmDspStartRoutineOut/DcmDspStartRoutineOutSignal/*[DcmDspRoutineSignalType = 'VARIABLE_LENGTH' and ((DcmDspRoutineSignalPos mod 8) != 0)])  or node:exists(node:ref(DcmDspRoutineInfoRef)/DcmDspRoutineStopOut/DcmDspRoutineStopOutSignal/*[DcmDspRoutineSignalType = 'VARIABLE_LENGTH' and ((DcmDspRoutineSignalPos mod 8) != 0)]) or node:exists(node:ref(DcmDspRoutineInfoRef)/DcmDspRoutineRequestResOut/DcmDspRoutineRequestResOutSignal/*[DcmDspRoutineSignalType = 'VARIABLE_LENGTH' and ((DcmDspRoutineSignalPos mod 8) != 0)])"!]
    [!/* Loop through all the protocols configured */!][!//
    [!LOOP "../../../DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*"!]
      [!/* Check that 0x31 service is enabled and using internal service handler */!][!//
      [!IF "node:exists(node:ref(DcmDslProtocolSIDTable)/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x31')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]
        [!/* Extract the Txbuffer name of this protocol */!][!//
        [!VAR "currenttxbuffername" = "node:name(node:ref(DcmDslProtocolTxBufferID))"!]
        [!/* Add into buffersRequiredExraByte list */!][!//
        [!IF "not( contains( $buffersRequiredExraByte, concat( ':', $currenttxbuffername, ':')))"!]
          [!VAR "buffersRequiredExraByte" = "concat( $buffersRequiredExraByte, $currenttxbuffername, ':')"!]
          [!/* Increment extraBufferSpaceForRoutineControl whenever new buffer is added to the list */!][!//
          [!VAR "extraBufferSpaceForRoutineControl" = "$extraBufferSpaceForRoutineControl +1"!]
        [!ENDIF!]
      [!ENDIF!]
    [!ENDLOOP!][!/* End of protocols loop */!][!//
  [!ENDIF!]
[!ENDLOOP!][!/* End of routines loop */!][!//
[!ENDINDENT!]

#if (defined DCM_NUM_RX_PDU_ID) /* To prevent double declaration */
#error DCM_NUM_RX_PDU_ID already defined
#endif /* if (defined DCM_NUM_RX_PDU_ID) */

/** \brief Definition of number of reception(Rx) Pdu Ids */
#define DCM_NUM_RX_PDU_ID                   [!//
[!"num:i( count( DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*/DcmDslProtocolRx/*))"!]U
[!//
[!VAR "NumUsedPeriodicTransmissions" = "0"!]
[!//
[!LOOP "node:order(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[.='DcmDslPeriodicTransmission'])"!]
  [!IF "node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[(.='DcmDslMainConnection') and (node:refexists(DcmDslPeriodicTransmissionConRef)) and (as:path(node:ref(DcmDslPeriodicTransmissionConRef)) = as:path(node:current()))])"!]
    [!VAR "NumUsedPeriodicTransmissions" = "num:integer($NumUsedPeriodicTransmissions + 1)"!]
  [!ENDIF!]
[!ENDLOOP!]
[!//
[!VAR "NumUsedPeriodicConnections" = "0"!]
[!//
[!LOOP "node:order(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[.='DcmDslPeriodicTransmission']/DcmDslPeriodicConnection/*)"!]
      [!IF "node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[(.='DcmDslMainConnection') and (node:refexists(DcmDslPeriodicTransmissionConRef)) and (as:path(node:ref(DcmDslPeriodicTransmissionConRef)) = as:path((node:current())/../../.))])"!]
    [!VAR "NumUsedPeriodicConnections" = "num:integer($NumUsedPeriodicConnections + 1)"!]
  [!ENDIF!]
[!ENDLOOP!]
[!//
[!VAR "NumUsedROEConnections" = "0"!]
[!//
[!LOOP "node:order(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[.='DcmDslResponseOnEvent'])"!]
  [!IF "node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[(.='DcmDslMainConnection') and (node:refexists(DcmDslROEConnectionRef)) and (as:path(node:ref(DcmDslROEConnectionRef)) = as:path(node:current()))])"!]
    [!VAR "NumUsedROEConnections" = "num:integer($NumUsedROEConnections + 1)"!]
  [!ENDIF!]
[!ENDLOOP!]
[!//
[!VAR "NumMainConnectionTxPduIds" = "num:i(count(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[.='DcmDslMainConnection']))"!]
[!VAR "NumPeriodicConnectionTxPduIds" = "num:i(count(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[.='DcmDslPeriodicTransmission']/DcmDslPeriodicConnection/*))"!]
[!VAR "NumROEConnectionTxPduIds" = "num:i(count(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[(.='DcmDslResponseOnEvent') and (node:exists(./DcmDslRoeTxPduRef)) and (node:refvalid(./DcmDslRoeTxPduRef)) ]))"!]

#if (defined DCM_NUM_TX_PDU_ID) /* To prevent double declaration */
#error DCM_NUM_TX_PDU_ID already defined
#endif /* if (defined DCM_NUM_TX_PDU_ID) */

/** \brief Definition of number of Transmission(Tx) Pdu Ids */
/* !LINKSTO Dcm.Eb.NumberOf.TxConnections,1 */
/* Amount of TX connections is calculated from the total of ROE Connections, Periodic
   connections and Main (real and virtual) connections. */
#define DCM_NUM_TX_PDU_ID                   [!//
[!"num:integer($NumMainConnectionTxPduIds + $NumROEConnectionTxPduIds + $NumPeriodicConnectionTxPduIds)"!]U

#if (defined DCM_NUM_CONFIRMATION_TX_PDU_ID) /* To prevent double declaration */
#error DCM_NUM_CONFIRMATION_TX_PDU_ID already defined
#endif /* if (defined DCM_NUM_CONFIRMATION_TX_PDU_ID) */

/** \brief Definition of number of Transmission(Tx) Confirmation Pdu Ids */
#define DCM_NUM_CONFIRMATION_TX_PDU_ID      DCM_NUM_TX_PDU_ID

#if defined DCM_TOTAL_CONFIGURED_BUFFER_SIZE /* To prevent double declaration */
#error DCM_TOTAL_CONFIGURED_BUFFER_SIZE already defined
#endif /* if defined DCM_TOTAL_CONFIGURED_BUFFER_SIZE */

#define DCM_TOTAL_CONFIGURED_BUFFER_SIZE    [!//
[!"num:integer(sum(node:refs(DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*/DcmDslProtocolRx/*/DcmDslProtocolRxBufferID)/DcmDslBufferSize | node:refs(DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslProtocolTxBufferID)/DcmDslBufferSize) + $extraBufferSpaceForRoutineControl)"!]U

#if defined DCM_TOTAL_RX_CONFIGURED_BUFFER_SIZE /* To prevent double declaration */
#error DCM_TOTAL_RX_CONFIGURED_BUFFER_SIZE already defined
#endif /* if defined DCM_TOTAL_RX_CONFIGURED_BUFFER_SIZE */

#define DCM_TOTAL_RX_CONFIGURED_BUFFER_SIZE    [!//
[!"num:i(sum(node:refs(DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*/DcmDslProtocolRx/*/DcmDslProtocolRxBufferID)/DcmDslBufferSize))"!]U

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.Types,2 */
#if (defined DCM_NUM_CONFIGURED_BUFFERS) /* To prevent double declaration */
#error DCM_NUM_CONFIGURED_BUFFERS already defined
#endif /* if (defined DCM_NUM_CONFIGURED_BUFFERS) */

/** \brief Definition of number of configured buffers */
#define DCM_NUM_CONFIGURED_BUFFERS          [!//
[!"num:i(count(node:refs(DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*/DcmDslProtocolRx/*/DcmDslProtocolRxBufferID) | node:refs(DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslProtocolTxBufferID)))"!]U

#if defined DCM_NUM_MAIN_CONNECTIONS /* To prevent double declaration */
#error DCM_NUM_MAIN_CONNECTIONS already defined
#endif /* if defined DCM_NUM_MAIN_CONNECTIONS */

#define DCM_NUM_MAIN_CONNECTIONS            [!//
[!"num:i(count(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[.='DcmDslMainConnection']))"!]U

#if defined DCM_NUM_GENERIC_CONNECTIONS /* To prevent double declaration */
#error DCM_NUM_GENERIC_CONNECTIONS already defined
#endif /* if defined DCM_NUM_GENERIC_CONNECTIONS */

[!VAR "GenericConnectionCount" = "0"!]
[!IF "node:value(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmGeneral/DcmGenericConnectionUsage) = 'true'"!]
  [!LOOP "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[.='DcmDslMainConnection']/DcmDslProtocolRx/*[1]/DcmDslProtocolRxPduRef"!]
    [!IF "node:exists(node:refs(node:current())/MetaDataTypeRef)"!]
      [!VAR "MetaDataRef" = "node:refs(node:current())/MetaDataTypeRef"!]
      [!VAR "hasSourceAddress" = "text:contains(node:refs($MetaDataRef)/*/*/MetaDataItemType, 'SOURCE_ADDRESS_16')"!]
      [!VAR "hasTargetAddress" = "text:contains(node:refs($MetaDataRef)/*/*/MetaDataItemType, 'TARGET_ADDRESS_16')"!]
      [!IF "node:exists($MetaDataRef) and $hasSourceAddress = 'true' and $hasTargetAddress = 'true'"!]
        [!VAR "GenericConnectionCount" = "$GenericConnectionCount + 1"!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!]
  [!LOOP "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[.='DcmDslResponseOnEvent']/DcmDslRoeTxPduRef"!]
    [!IF "node:exists(node:refs(node:current())/MetaDataTypeRef)"!]
      [!VAR "MetaDataRef" = "node:refs(node:current())/MetaDataTypeRef"!]
      [!VAR "hasSourceAddress" = "text:contains(node:refs($MetaDataRef)/*/*/MetaDataItemType, 'SOURCE_ADDRESS_16')"!]
      [!VAR "hasTargetAddress" = "text:contains(node:refs($MetaDataRef)/*/*/MetaDataItemType, 'TARGET_ADDRESS_16')"!]
      [!IF "node:exists($MetaDataRef) and $hasSourceAddress = 'true' and $hasTargetAddress = 'true'"!]
       [!VAR "GenericConnectionCount" = "$GenericConnectionCount + 1"!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!]
  [!LOOP "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[.='DcmDslPeriodicTransmission']/DcmDslPeriodicConnection/*/DcmDslPeriodicTxPduRef"!]
    [!IF "node:exists(node:refs(node:current())/MetaDataTypeRef)"!]
      [!VAR "MetaDataRef" = "node:refs(node:current())/MetaDataTypeRef"!]
      [!VAR "hasSourceAddress" = "text:contains(node:refs($MetaDataRef)/*/*/MetaDataItemType, 'SOURCE_ADDRESS_16')"!]
      [!VAR "hasTargetAddress" = "text:contains(node:refs($MetaDataRef)/*/*/MetaDataItemType, 'TARGET_ADDRESS_16')"!]
      [!IF "node:exists($MetaDataRef) and $hasSourceAddress = 'true' and $hasTargetAddress = 'true'"!]
       [!VAR "GenericConnectionCount" = "$GenericConnectionCount + 1"!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDIF!]
#define DCM_NUM_GENERIC_CONNECTIONS            [!"num:i($GenericConnectionCount)"!]U

#if defined DCM_NUM_PROTOCOLS /* To prevent double declaration */
#error DCM_NUM_PROTOCOLS already defined
#endif /* if defined DCM_NUM_PROTOCOLS */

#define DCM_NUM_PROTOCOLS                   [!//
[!"num:integer(count(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*))"!]U

#if defined DCM_DEM_CLIENT_REF_USED /* To prevent double declaration */
#error DCM_DEM_CLIENT_REF_USED already defined
#endif /* if defined DCM_DEM_CLIENT_REF_USED */
[!IF "node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/./DcmDemClientRef)"!]
#define DCM_DEM_CLIENT_REF_USED             STD_ON
[!ELSE!]
#define DCM_DEM_CLIENT_REF_USED             STD_OFF
[!ENDIF!]
[!IF "node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/./DcmDemClientRef)"!]
#if defined DCM_NUM_DEM_CLIENTS /* To prevent double declaration */
#error DCM_NUM_DEM_CLIENTS already defined
#endif /* if defined DCM_NUM_DEM_CLIENTS */

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
#define DCM_NUM_DEM_CLIENTS                 [!//
[!"num:i(count(text:split($DemClientId)))"!]U
[!ENDIF!][!//

#if defined DCM_NUM_STACKS
#error DCM_NUM_STACKS already defined
#endif

#define DCM_NUM_STACKS           [!//
[!"num:i(num:max(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslProtocolStackNumber) + 1)"!]U

/* !LINKSTO Dcm.Dsn.LowerLayerInterfaces.PduRInterface.Buffer.Types,2 */
#if (defined DCM_NUM_NRC_BUFFERS) /* To prevent double declaration */
#error DCM_NUM_NRC_BUFFERS already defined
#endif /* if (defined DCM_NUM_NRC_BUFFERS) */

/** \brief Definition of number of NRC buffers */
#define DCM_NUM_NRC_BUFFERS                 DCM_NUM_TX_PDU_ID

#if (defined DCM_NUM_DEFAULT_BUFFERS) /* To prevent double declaration */
#error DCM_NUM_DEFAULT_BUFFERS already defined
#endif /* if (DCM_NUM_DEFAULT_BUFFERS) */

/** \brief Definition of number of default 2-byte buffers */
#define DCM_NUM_DEFAULT_BUFFERS             DCM_NUM_RX_PDU_ID
[!//
[!VAR "PeriodicEnabled" = "'FALSE'"!]
[!//
[!IF "(node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[(.='DcmDslMainConnection') and (node:refvalid(./DcmDslPeriodicTranmissionConRef))]))"!]
  [!VAR "PeriodicEnabled" = "'TRUE'"!]
[!ENDIF!]
[!IF "(node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[(.='DcmDslPeriodicTransmission')]))"!]
  [!VAR "PeriodicEnabled" = "'TRUE'"!]
[!ENDIF!]
[!IF "($DCM_DSP_USE_SERVICE_0X2A = 'STD_ON')"!]
  [!VAR "PeriodicEnabled" = "'TRUE'"!]
[!ENDIF!]

#if defined DCM_PERIODIC_USED /* To prevent double declaration */
#error DCM_PERIODIC_USED already defined
#endif /* if defined DCM_PERIODIC_USED */

[!IF "$PeriodicEnabled = 'TRUE'"!]
#define DCM_PERIODIC_USED                   STD_ON
[!ELSE!]
#define DCM_PERIODIC_USED                   STD_OFF
[!ENDIF!]

#if defined DCM_NUM_ROE_CONNECTIONS /* To prevent double declaration */
#error DCM_NUM_ROE_CONNECTIONS already defined
#endif /* if defined DCM_NUM_ROE_CONNECTIONS */

#define DCM_NUM_ROE_CONNECTIONS             [!//
[!"num:i($NumUsedROEConnections)"!]U

#if defined DCM_NUM_PERIODIC_TRANSMISSIONS /* To prevent double declaration */
#error DCM_NUM_PERIODIC_TRANSMISSIONS already defined
#endif /* if defined DCM_NUM_PERIODIC_TRANSMISSIONS */

#define DCM_NUM_PERIODIC_TRANSMISSIONS      [!//
[!"num:i($NumUsedPeriodicTransmissions)"!]U

#if defined DCM_NUM_PERIODIC_CONNECTIONS /* To prevent double declaration */
#error DCM_NUM_PERIODIC_CONNECTIONS already defined
#endif /* if defined DCM_NUM_PERIODIC_CONNECTIONS */

#define DCM_NUM_PERIODIC_CONNECTIONS        [!//
[!"num:i($NumUsedPeriodicConnections)"!]U

#if defined DCM_VIRTUAL_TX_CONNECTION
#error DCM_VIRTUAL_TX_CONNECTION already defined
#endif

#define DCM_VIRTUAL_TX_CONNECTION   0xffffU

#if defined DCM_TOTAL_NUMBER_OF_REAL_TX_CONNECTIONS
#error DCM_TOTAL_NUMBER_OF_REAL_TX_CONNECTIONS already defined
#endif

#define DCM_TOTAL_NUMBER_OF_REAL_TX_CONNECTIONS         [!//
[!VAR "TotalRealMainConnectionTxConnectionPduId" = "num:i(count(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[(.='DcmDslMainConnection') and (node:exists(DcmDslProtocolTx))]))"!]
[!VAR "TotalRealPeriodicTxConnectionPduId" = "num:i(count(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[.='DcmDslPeriodicTransmission']/DcmDslPeriodicConnection/*))"!]
[!VAR "TotalRealROETxConnectionPduId" = "num:i(count(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[(.='DcmDslResponseOnEvent') and (node:exists(./DcmDslRoeTxPduRef)) and (node:refvalid(./DcmDslRoeTxPduRef)) ]))"!]
[!VAR "FirstVirtualPduId" = "num:i($TotalRealMainConnectionTxConnectionPduId + $TotalRealPeriodicTxConnectionPduId + $TotalRealROETxConnectionPduId)"!]
[!"num:i($FirstVirtualPduId)"!]U

#if defined DCM_VIRTUAL_TX_CONNECTIONS_PRESENT
#error DCM_VIRTUAL_TX_CONNECTIONS_PRESENT already defined
#endif
[!VAR "AmountOfVirtualConnections" = "0"!]
[!LOOP "as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*[.='DcmDslMainConnection']"!]
  [!IF "(node:exists(DcmDslProtocolTx))"!]
  [!ELSE!]
    [!VAR "AmountOfVirtualConnections" = "$AmountOfVirtualConnections + 1"!]
  [!ENDIF!]
[!ENDLOOP!]
[!IF "$AmountOfVirtualConnections = 0"!]
#define DCM_VIRTUAL_TX_CONNECTIONS_PRESENT         STD_OFF
[!ELSE!]
#define DCM_VIRTUAL_TX_CONNECTIONS_PRESENT         STD_ON
[!ENDIF!]

[!/*ChannelID holds the different 'DcmDslProtocolRxChannelId' configured. This will use for the generation of the macro DCM_NUM_RX_COMM_CHANNELS*/!][!//
[!VAR "ChannelID" =  "' '"!][!//
[!LOOP "DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/DcmDslConnection/*/DcmDslProtocolRx/*"!][!//
  [!IF "not( contains( $ChannelID, concat( ' ', ./DcmDslProtocolRxChannelId,' ')))"!][!//
    [!VAR "ChannelID" = "concat( $ChannelID,' ',./DcmDslProtocolRxChannelId,' ')"!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!//
[!//
/** \brief Definition of number of channel Identifiers configured. */
#if (defined DCM_NUM_RX_COMM_CHANNELS)
  #error "DCM_NUM_RX_COMM_CHANNELS is already defined"
#endif
#define DCM_NUM_RX_COMM_CHANNELS                     [!"num:i(count(text:split($ChannelID)))"!]U

#if (defined DCM_DSL_APPLICATION_DIAGNOSTIC_REQUESTS) /* To prevent double declaration */
#error DCM_DSL_APPLICATION_DIAGNOSTIC_REQUESTS already defined
#endif /* if (defined DCM_DSL_APPLICATION_DIAGNOSTIC_REQUESTS) */

/** \brief Application Diagnostic Request is enabled or not */
#define DCM_DSL_APPLICATION_DIAGNOSTIC_REQUESTS                [!//
[!IF "node:exists(DcmConfigSet/*[1]/DcmDsl/DcmDslApplicationDiagnosticRequests)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!IF "node:exists(DcmConfigSet/*[1]/DcmDsl/DcmDslApplicationDiagnosticRequests)"!]
#if (defined DCM_MAX_INJECTION_BUFFER_SIZE) /* To prevent double declaration */
#error DCM_MAX_INJECTION_BUFFER_SIZE already defined
#endif /* if (defined DCM_MAX_INJECTION_BUFFER_SIZE) */

/** \brief Definition of Maximum buffer size for Application Diagnostic Requests */
#define DCM_MAX_INJECTION_BUFFER_SIZE                       [!//
[!"num:i(DcmConfigSet/*[1]/DcmDsl/DcmDslApplicationDiagnosticRequests/DcmDslApplicationDiagnosticRequestBufferSize)"!]U
[!ENDIF!][!//

/*==================[type definitions]===========================================================*/

[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmComMMultiCoreSupport = 'true'"!]
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_SchMCallDiagnosticNotificationFunctionType)(uint8 ComMChannelId);

typedef struct
{
  uint8 ComMChannelId;
  Dcm_SchMCallDiagnosticNotificationFunctionType ActiveDiagnostic;
  Dcm_SchMCallDiagnosticNotificationFunctionType InactiveDiagnostic;
}
Dcm_DiagnosticNotificationLookUpType;
[!ENDIF!]

/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[external data]==============================================================*/

[!IF "DcmConfigSet/*[1]/DcmGeneral/DcmComMMultiCoreSupport = 'true'"!]
#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

extern CONST(Dcm_DiagnosticNotificationLookUpType, DCM_CONST)
                          Dcm_DiagnosticNotificationLookUpTable[DCM_NUM_RX_COMM_CHANNELS];

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>
[!ENDIF!]
[!//
[!IF "node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/./DcmDemClientRef)"!]
#define DCM_START_SEC_CONST_8
#include <Dcm_MemMap.h>

extern CONST(uint8, DCM_CONST) Dcm_DemClientIds[DCM_NUM_DEM_CLIENTS];

#define DCM_STOP_SEC_CONST_8
#include <Dcm_MemMap.h>

[!ENDIF!]
[!//
/*==================[internal data]==============================================================*/

#define DCM_START_SEC_CONST_8
#include <Dcm_MemMap.h>

extern CONST(NetworkHandleType, DCM_CONST) Dcm_RxComMChannelIDs[DCM_NUM_RX_COMM_CHANNELS];

#define DCM_STOP_SEC_CONST_8
#include <Dcm_MemMap.h>

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef DCM_DSL_CFG_H */
/*==================[end of file]================================================================*/
