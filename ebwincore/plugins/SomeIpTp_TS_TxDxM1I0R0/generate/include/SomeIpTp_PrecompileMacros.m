[!/**
 * \file
 *
 * \brief AUTOSAR SomeIpTp
 *
 * This file contains the implementation of the AUTOSAR
 * module SomeIpTp.
 *
 * \version 1.0.28
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!NOCODE!]
[!// multiple inclusion protection
[!IF "not(var:defined('SOMEIPTP_PRECOMPILEMACROS_M'))"!]
[!VAR "SOMEIPTP_PRECOMPILEMACROS_M"="'true'"!][!/*

=== Size of the SomeIpTp Channel list ===
*/!][!VAR "SomeIpTp_ChannelListSize" = "num:i(count(as:modconf('SomeIpTp')[1]/SomeIpTpChannel/*))"!][!/*

=== Size of the SomeIpTp TxNsdu list ===
*/!][!VAR "SomeIpTp_TxNsduListSize" = "num:i(count(as:modconf('SomeIpTp')[1]/SomeIpTpChannel/*/SomeIpTpTxNSdu/*))"!][!/*

=== Size of the SomeIpTp RxNsdu list ===
*/!][!VAR "SomeIpTp_RxNsduListSize" = "num:i(count(as:modconf('SomeIpTp')[1]/SomeIpTpChannel/*/SomeIpTpRxNSdu/*))"!][!/*

=== Macro to translate the configured TxSeparationTime/TxConfirmationTimeout float into number of main function Tx cycles. ===      
*/!][!MACRO "TxTimeToMFCTX", "TxValue"!][!NOCODE!]
    [!VAR "mTxValue" = "$TxValue div node:value(as:modconf('SomeIpTp')[1]/SomeIpTpGeneral/SomeIpTpTxMainFunctionPeriod)"!]
    [!VAR "MFCTx" = "num:i(ceiling($mTxValue))"!]
[!ENDNOCODE!][!ENDMACRO!][!/*

=== Macro to translate the configured RxTimeoutTime float into number of main function Rx cycles. ===      
*/!][!MACRO "RxTimeoutTimeToMFCRX", "RxValue"!][!NOCODE!]
    [!VAR "mRxValue" = "$RxValue div node:value(as:modconf('SomeIpTp')[1]/SomeIpTpGeneral/SomeIpTpRxMainFunctionPeriod)"!]
    [!VAR "MFCRx" = "num:i(ceiling($mRxValue))"!]
[!ENDNOCODE!][!ENDMACRO!][!/*

=== Macro to translate SrcPdu in pdur for TxNPdu transmission path. ===      
*/!][!MACRO "TxNPduToSrcPdu", "value"!][!NOCODE!]
      [!LOOP "as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/PduRRoutingPath/*"!]
        [!IF "(node:path(node:ref(./PduRSrcPdu/PduRSrcPduRef))) = $value"!]
          [!VAR "SrcPduId" = "num:i(./PduRSrcPdu/PduRSourcePduHandleId)"!]
          <!-- !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00004,1 -->
          [!VAR "ActualTxNPduLength" = "num:i(node:ref(./PduRSrcPdu/PduRSrcPduRef)/PduLength)"!]
        [!ENDIF!]
      [!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!][!/*

=== Macro to translate DestPdu in pdur for TxNSdu confirmation path. ===      
*/!][!MACRO "TxNSduToDestPdu", "value"!][!NOCODE!]
      [!LOOP "as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/PduRRoutingPath/*/PduRDestPdu/*"!]
        [!IF "(node:path(node:ref(./PduRDestPduRef))) = $value"!]
          [!ASSERT "node:exists(./PduRDestPduHandleId)"!]
            <PduRDestPduHandleId shall be set>
          [!ENDASSERT!]
          [!VAR "DestPduId" = "num:i(./PduRDestPduHandleId)"!]
          [!VAR "ActualTxNSduLength" = "num:i(node:ref(./PduRDestPduRef)/PduLength)"!]
        [!ENDIF!]
      [!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!][!/*

=== Macro to translate DestPdu in pdur for RxNSdu indication path. ===      
*/!][!MACRO "RxNSduToDestPdu", "value"!][!NOCODE!]
      [!LOOP "as:modconf('PduR')[1]/PduRRoutingTables/*/PduRRoutingTable/*/PduRRoutingPath/*/PduRDestPdu/*"!]
        [!IF "(node:path(node:ref(./PduRDestPduRef))) = $value"!]
          [!ASSERT "node:exists(./PduRDestPduHandleId)"!]
            <PduRDestPduHandleId shall be set>
          [!ENDASSERT!]
          [!VAR "DstPduId" = "num:i(./PduRDestPduHandleId)"!]
        [!ENDIF!]
      [!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!][!/*

*/!][!ENDIF!][!// SOMEIPTP_PRECOMPILEMACROS_M

[!ENDNOCODE!][!//

