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
[!NOCODE!][!//
[!IF "not(var:defined('IPDUM_PBNUMERICVARS_M'))"!]
[!VAR "IPDUM_PBNUMERICVARS_M"="'true'"!]

[!SELECT "IpduMConfig/*[1]"!]

[!/*TxDynamicPartOffset is used to distinguish the static PDU from the dynamic one */!]
[!VAR "TxDynamicPartOffset"= "num:i(count(IpduMTxPathway/*/IpduMTxRequest/IpduMTxStaticPart))"!]
[!VAR "StaticIdMax" = "num:i($TxDynamicPartOffset - 1)"!]
[!/* Offset in bytes */!]
[!VAR "QueueOffset"= "num:i(0)"!]
[!/* Offset in bytes */!]
[!VAR "BufferOffset"= "num:i(0)"!]
[!VAR "TxQueueTotalEntries" = "num:i(node:when($IpduMDynamicPartQueue = 'true', sum(IpduMTxPathway/*/IpduMTxRequest/IpduMQueueSize), 0))"!]

[!VAR "numTxPathway" = "num:i(count(IpduMTxPathway/*))"!]
[!VAR "numRxPathway" = "num:i(count(IpduMRxPathway/*))"!]
[!VAR "numTxContainer" = "num:i(count(IpduMContainerTxPdu/*))"!]
[!VAR "numContainedTx" = "num:i(count(IpduMContainedTxPdu/*))"!]
[!VAR "NrOfRxContained" = "num:i(count(IpduMContainedRxPdu/*))"!]
[!VAR "NrOfRxContainers" = "num:i(count(IpduMContainerRxPdu/*))"!]
[!VAR "NumCntdRxAcceptConf" = "num:i(count(IpduMContainedRxPdu/*[node:exists(IpduMContainedPduHeaderId) and node:exists(IpduMContainedRxInContainerPduRef)]))"!]
[!VAR "NumStaticRxCntdPdus" = "num:i(count(IpduMContainedRxPdu/* [node:exists(IpduMContainedRxInContainerPduRef) and as:ref(IpduMContainedRxInContainerPduRef)/IpduMContainerHeaderSize = 'IPDUM_HEADERTYPE_NONE']))"!]
[!VAR "UpperLimitRxCntdAll" = "node:when($RxContainerAcceptAllNoRefOnly = 'true', num:i($NrOfRxContained - $NumStaticRxCntdPdus - $NumCntdRxAcceptConf), num:i($NrOfRxContained - $NumStaticRxCntdPdus))"!]
[!VAR "NumRxCntrInst" = "num:i(count(IpduMContainerRxPdu/*[IpduMContainerPduProcessing = 'IPDUM_PROCESSING_DEFERRED']) + sum(IpduMContainerRxPdu/*[IpduMContainerPduProcessing = 'IPDUM_PROCESSING_DEFERRED' and node:exists(IpduMContainerQueueSize)]/IpduMContainerQueueSize))"!]
[!VAR "numRxTotal" = "num:i(count(IpduMRxPathway/*) + $NrOfRxContainers)"!]
[!VAR "DynamicIdMax" = "node:order(IpduMTxPathway/*/IpduMTxRequest/IpduMTxDynamicPart/*, 'node:IpduMTxDynamicHandleId')[last()]/IpduMTxDynamicHandleId"!]
[!VAR "FirstPduPos" = "num:i(0)"!]
[!VAR "NumRxCntdPdusRef" = "num:i(0)"!]

[!VAR "numCntrTxBuffs" = "num:i(sum(node:refs(IpduMContainedTxPdu/*[IpduMContainedTxPduCollectionSemantics = 'IPDUM_COLLECT_QUEUED']/IpduMContainedTxInContainerPduRef)/IpduMContainerQueueSize) + count(node:refs(IpduMContainedTxPdu/*[IpduMContainedTxPduCollectionSemantics = 'IPDUM_COLLECT_QUEUED']/IpduMContainedTxInContainerPduRef)) + count(node:filter(IpduMContainerTxPdu/*[IpduMContainerTxTriggerMode = 'IPDUM_DIRECT'], "'IPDUM_COLLECT_LAST_IS_BEST' = ../../IpduMContainedTxPdu/*[node:path(node:ref(IpduMContainedTxInContainerPduRef)) = node:path(node:current())][1]/IpduMContainedTxPduCollectionSemantics")))"!]
[!ENDSELECT!]

[!ENDIF!][!// multiple inclusion protection
[!ENDNOCODE!][!//
