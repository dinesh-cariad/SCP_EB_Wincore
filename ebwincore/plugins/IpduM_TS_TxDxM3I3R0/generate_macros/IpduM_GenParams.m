[!/**
 * \file
 *
 * \brief AUTOSAR IpduM
 *
 * This file contains the implementation of the AUTOSAR
 * module IpduM.
 *
 * \version 3.3.40
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!NOCODE!]
[!IF "not(var:defined('IPDUM_GENPARAMS_M'))"!]
[!VAR "IPDUM_GENPARAMS_M"="'true'"!]

[!// Avoid executing the validator just because of these
****************************************************************************
* Get all the general parameters in variables.
****************************************************************************
*/!][!//
[!SELECT "as:modconf('IpduM')[1]/IpduMGeneral"!][!//
[!VAR "IpduMRxTimeBase" = "node:value(./IpduMRxTimeBase)"!][!//
[!VAR "IpduMTxTimeBase" = "node:value(./IpduMTxTimeBase)"!][!//
[!VAR "IpduMDevErrorDetect" = "node:value(./IpduMDevErrorDetect)"!][!//
[!VAR "IpduMZeroCopy" = "node:value(./IpduMZeroCopy)"!][!//
[!VAR "IpduMByteCopy" = "node:value(./IpduMByteCopy)"!][!//
[!VAR "IpduMDynamicPartQueue" = "node:value(./IpduMDynamicPartQueue)"!][!//
[!VAR "IpduMTxRequestMaxSduLength" = "node:value(./IpduMTxRequestMaxSduLength)"!]
[!VAR "IpduMTxAutomaticSelector" = "node:value(./IpduMTxAutomaticSelector)"!][!//
[!/* Written like this in order to avoid handling the empty string in the node:when, node:fallback chain */!]
[!VAR "IpduMDataMemSize"!][!IF "not(node:empty(./IpduMDataMemSize))"!][!"node:value(./IpduMDataMemSize)"!][!ELSE!][!"num:i(0)"!][!ENDIF!][!ENDVAR!][!//
[!VAR "IpduMInitializationBySignalValue" = "node:value(./IpduMInitializationBySignalValue)"!][!//
[!VAR "IpduMEnableJitUpdate" = "node:value(./IpduMEnableJitUpdate)"!][!//
[!VAR "IpduMStaticPartExists" = "node:value(./IpduMStaticPartExists)"!][!//
[!VAR "IpduMMultiMF" = "./IpduMDedicatedIpduProcessingSupport = 'true'"!][!//
[!VAR "IpduMContainerPduHandlingEnable" = "node:value(./IpduMContainerPduHandlingEnable)"!][!//
[!VAR "IpduMStaticContainerPduHandlingEnable" = "node:fallback("->node:value(./IpduMStaticContainerPduHandling)", false())"!][!//
[!VAR "IpduMVersionInfoApi" = "node:value(./IpduMVersionInfoApi)"!][!//
[!VAR "IpduMContainerQueuingRxEnable" = "node:fallback("->node:value(./IpduMContainerQueuingRx)", false())"!][!//
[!VAR "IpduMContainedCollectQueuedTxEnable" = "node:fallback("->node:value(./IpduMContainedCollectQueuedTx)", false())"!][!//
[!VAR "IpduMMaxContainerRxLength" = "node:fallback("->node:value(./IpduMMaxContainerRxLength)", num:i(0))"!][!//
[!VAR "IpduMContainedTxPduPriorityHandlingEnable" = "node:fallback("->node:value(./IpduMContainedTxPduPriorityHandling)", false())"!][!//
[!VAR "RxContainerAcceptAllNoRefOnly" = "node:fallback("->node:value(./IpduMRxContainerAcceptAllNoRefOnly)", false())"!][!//
[!IF "not(node:empty(./IpduMAtomicValueSizeOverride))"!]
[!VAR "IpduMAtomicValueSizeOverride" = "num:i(node:value(./IpduMAtomicValueSizeOverride))"!]
[!ENDIF!]

[!ENDSELECT!][!//
[!// Not a general parameter, still needed
[!VAR "NumIpduMQTxContainers" = "num:i(count(as:modconf('IpduM')/IpduMConfig/*[1]/IpduMContainerTxPdu/*[node:exists(IpduMContainerQueueSize)]))"!][!//
[!VAR "NumIpduMRxQContainers" = "num:i(count(as:modconf('IpduM')/IpduMConfig/*[1]/IpduMContainerRxPdu/*[node:exists(IpduMContainerQueueSize)]))"!][!//
[!VAR "NoOfDeferredContainers" ="num:i(count(as:modconf('IpduM')/IpduMConfig/*[1]/IpduMContainerRxPdu/*[IpduMContainerPduProcessing = 'IPDUM_PROCESSING_DEFERRED']))"!][!//

[!ENDIF!][!// multiple inclusion protection
[!ENDNOCODE!][!//
