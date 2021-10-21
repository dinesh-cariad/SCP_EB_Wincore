[!/**
 * \file
 *
 * \brief AUTOSAR CanIf
 *
 * This file contains the implementation of the AUTOSAR
 * module CanIf.
 *
 * \version 6.10.15
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!NOCODE!]
[!IF "not(var:defined('CANIF_DECOUPLEDPROCESSING_M'))"!]
[!VAR "CANIF_DECOUPLEDPROCESSING_M"="'true'"!]

[!// Decoupled Rx support
[!VAR "decoupledRxSupported" = "node:exists(CanIfPublicCfg/CanIfRxProcessing/*/CanIfRxPduProcessingRef)"!]
[!VAR "numRxMFs" = "node:when($decoupledRxSupported, num:i(count(CanIfPublicCfg/CanIfRxProcessing/*)), num:i(0))"!]
[!VAR "canifRxPduQueueSize" = "node:when($decoupledRxSupported, num:i(sum(CanIfPublicCfg/CanIfRxProcessing/*/CanIfRxPduQueueSize)), num:i(0))"!]
[!VAR "canifRxQOffsetType" = "node:when($canifRxPduQueueSize < 256, 'uint8', 'uint16')"!]
[!VAR "canifMaxPayloadQueueSize" = "node:when($decoupledRxSupported, num:i(sum(CanIfPublicCfg/CanIfRxProcessing/*/CanIfPublicMaxPayloadQueueSize)), num:i(0))"!]
[!VAR "canifMaxPayloadSizeType" = "node:when(num:max(CanIfPublicCfg/CanIfRxProcessing/*/CanIfPublicMaxPayloadQueueSize) < 256 , 'uint8', 'uint16')"!]
[!VAR "canifMaxPayloadSizeTotalType" = "node:when($canifMaxPayloadQueueSize < 256, 'uint8', 'uint16')"!]
[!VAR "canifRxPduQueueSizeType" = "node:when(num:max(CanIfPublicCfg/CanIfRxProcessing/*/CanIfRxPduQueueSize) < 256, 'uint8', 'uint16')"!]
[!// Decoupled Tx support
[!VAR "txDecoupledSupport" = "node:exists(CanIfPublicCfg/CanIfTxProcessing/*/CanIfTxPduProcessingRef)"!]
[!VAR "numTxMFs" = "node:when($txDecoupledSupport, num:i(count(CanIfPublicCfg/CanIfTxProcessing/*)), num:i(0))"!]
[!VAR "canifTxPduQueueSize" = "node:when($txDecoupledSupport, num:i(sum(CanIfPublicCfg/CanIfTxProcessing/*/CanIfTxPduQueueSize)), num:i(0))"!]
[!VAR "canifTxQOffsetType" = "node:when($canifTxPduQueueSize < 256, 'uint8', 'uint16')"!]
[!/*
Macro creates a string-map of RxPdu-MainFunctionRx allocation.
Attributes are separated by pound sign(#), attribute list elements by pipe (|) 
and entries by semicolon (;).
*/!]
[!MACRO "createRxMFAlloc"!]
  [!IF "not(var:defined('mfRxAlloc'))"!]
    [!VAR "mfRxAlloc" = "''"!]
    [!LOOP "as:modconf('CanIf')[1]/CanIfPublicCfg/CanIfRxProcessing/*"!]
      [!VAR "mfRxAlloc" = "concat($mfRxAlloc, ';', node:pos(.), '#', node:name(.), '#CanIf_MainFunctionRx_', node:name(.), '#', text:join(./CanIfRxPduProcessingRef/*,'|'))"!]
    [!ENDLOOP!]
  [!ENDIF!]
[!ENDMACRO!]
[!/*
Macro creates a string-map of TxPdu-MainFunctionTx allocation.
Attributes are separated by pound sign(#), attribute list elements by pipe (|)
and entries by semicolon (;).
*/!]
[!MACRO "createTxMFAlloc"!]
  [!IF "not(var:defined('mfTxAlloc'))"!]
    [!VAR "mfTxAlloc" = "''"!]
    [!LOOP "as:modconf('CanIf')[1]/CanIfPublicCfg/CanIfTxProcessing/*"!]
      [!VAR "pos" = "node:pos(.)"!]
      [!VAR "mfTxAlloc" = "concat($mfTxAlloc, ';', node:pos(.), '#', node:name(.), '#CanIf_MainFunctionTx_', node:name(.), '#', text:join(./CanIfTxPduProcessingRef/*,'|'))"!]
    [!ENDLOOP!]
  [!ENDIF!]
[!ENDMACRO!]

[!ENDIF!] [!// CANIF_DECOUPLEDPROCESSING_M
[!ENDNOCODE!]