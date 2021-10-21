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
[!IF "not(var:defined('IPDUM_TYPES_M'))"!]
[!VAR "IPDUM_TYPES_M"="'true'"!]
[/* Contains type definitions */]

[!INCLUDE "IpduM_GenParams.m"!]
[!INCLUDE "IpduM_PBNumericVars.m"!]

[!IF "var:defined('postBuildVariant')"!]
[!VAR "IPDUM_VARIANT_SUFFIX"="concat('_',$postBuildVariant,'_')"!]
[!VAR "initPredefinedCfgName"="concat('IpduM_Config','_',$postBuildVariant)"!]
[!ELSE!]
[!VAR "IPDUM_VARIANT_SUFFIX"="'_'"!]
[!VAR "initPredefinedCfgName"="'IpduM_Config'"!]
[!ENDIF!]
[!VAR "IPDUM_PBCFG_C_VARIANT"="concat('IpduM',$IPDUM_VARIANT_SUFFIX,'PBcfg.c')"!]
[!VAR "IPDUM_PBCFG_H_VARIANT"="concat('IpduM',$IPDUM_VARIANT_SUFFIX,'PBcfg.h')"!]
[!VAR "IPDUM_CONFIG_LAYOUT_VARIANT"="concat('IpduM',$IPDUM_VARIANT_SUFFIX,'ConfigLayout')"!]
[!VAR "IPDUM_CONFIGLAYOUTTYPE_VARIANT"="concat('IpduM',$IPDUM_VARIANT_SUFFIX,'ConfigLayoutType')"!]
[!VAR "IPDUM_CONSTCONFIGLAYOUTTYPE_VARIANT"="concat('IpduM',$IPDUM_VARIANT_SUFFIX,'ConstConfigLayoutType')"!]

[/* Externals */]
[!/* getDataTypeSize and getDataTypeAlignment return empty strings for the Generic tests. Workaround. TBD: Why ? */!]
[!IF "'' != asc:getDataTypeSize('uint32')"!]
[!VAR "PduIdType_SIZE" = "asc:getDataTypeSize(text:tolower(as:modconf('EcuC')/EcucPduCollection/PduIdTypeEnum))"!]
[!VAR "PduIdType_ALG" = "asc:getDataTypeAlignment(text:tolower(as:modconf('EcuC')/EcucPduCollection/PduIdTypeEnum))"!]
[!VAR "PduLengthType_SIZE" = "asc:getDataTypeSize(text:tolower(as:modconf('EcuC')/EcucPduCollection/PduLengthTypeEnum))"!]
[!VAR "PduLengthType_ALG" = "asc:getDataTypeAlignment(text:tolower(as:modconf('EcuC')/EcucPduCollection/PduLengthTypeEnum))"!]
[!ELSE!]
[!VAR "PduIdType_SIZE" = "2"!]
[!VAR "PduIdType_ALG" = "2"!]
[!VAR "PduLengthType_SIZE" = "2"!]
[!VAR "PduLengthType_ALG" = "2"!]
[!ENDIF!]
[!/* Variables for the other types exist using the same naming scheme but they're constructed */!]
[/* Semicolon separated list of all the types' names */]
[!/* Types which need their sizes determined */!]
[!VAR "IpduMTypeRegistry_RAM" = "'IpduM_DataOffsetType;IpduM_TxTimeoutType;?IpduM_QueueEntryType;IpduM_TxDataType;IpduM_RxCntrInstType;IpduM_CntrQueueType;IpduM_TxCntrRAMDataType;IpduM_NumCntdType;IpduM_CntrTxInstType'"!]

[!/* Types used in the config */!]
[!VAR "IpduMTypeRegistry_CONF" = "'IpduM_BitfieldType;IpduM_BitfieldArraySizeType;IpduM_SelectorType;IpduM_MainFuncIdxType;IpduM_RxPartType;IpduM_RxPathWayType;IpduM_TxPartType;IpduM_TxPathWayType;IpduM_PathWayLenType;IpduM_CopyBitFieldType;IpduM_JitDataType;IpduM_TxOffsetType;IpduM_ContainerNumType;IpduM_ContainedNumType;?IpduM_ContainedRxPduType;?IpduM_StaticContainedRxPduType;?IpduM_ContainerRxPduType;IpduM_ContainerTxPduType;IpduM_ContainedTxPduType;IpduM_ConfigType'"!]

[!/* Used for the PB struct */!]
[!VAR "IpduMTypeRegistry_CONFSTRUCT" = "$IPDUM_CONFIGLAYOUTTYPE_VARIANT"!]

[!VAR "IpduM_BitfieldType_DEF" = "'uint16'"!]
[!VAR "IpduM_BitfieldArraySizeType_DEF" = "'uint16'"!]
[!VAR "IpduM_SelectorType_DEF" = "'uint8'"!]
[!VAR "IpduM_MainFuncIdxType_DEF" = "'PduIdType'"!]
[!/* Structs are denoted by {}. Nesting is not supported, typedef when if needed */!]
[!VAR "IpduM_CopyBitFieldType_DEF" = "'{}StartBit:IpduM_BitfieldType|EndBit:IpduM_BitfieldType|DestinationBit:IpduM_BitfieldType'"!]
[!VAR "IpduM_DataOffsetType_DEF" = "'uint32'"!]
[!VAR "IpduM_TxTimeoutType_DEF" = "'uint32'"!]
[!VAR "IpduM_PathWayLenType_DEF" = "'uint16'"!]
[!VAR "IpduM_TxOffsetType_DEF" = "'uint16'"!]
[!VAR "IpduM_ContainerNumType_DEF" = "'uint16'"!]
[!VAR "IpduM_ContainedNumType_DEF" = "'uint16'"!]
[!VAR "IpduM_NumCntdType_DEF" = "node:when($PduLengthType_SIZE > 2, 'uint16', 'uint8')"!]

[!/* !LINKSTO IPDUM.EB.IPDUM902,2 */!]
[!VAR "IpduM_QueueEntryType_DEF"!]{}TxPduId:PduIdType|PduRTxConfirmationPduId:PduIdType|DynamicPriority:uint16|SduData:[[!"num:i($IpduMTxRequestMaxSduLength)"!]]uint8[!ENDVAR!]
[!VAR "IpduM_QueueEntryType_COND" = "'TRUE:IpduMDynamicPartQueue'"!]

[!VAR "IpduM_TxDataType_DEF" = "'{}ConfirmationTimeoutCounter:IpduM_TxTimeoutType|PduRTxConfirmationPduId:PduIdType|?DynamicPartIndex:PduIdType|?PduRTriggerTxPduId:PduIdType|?QueuePos:uint8'"!]
[!VAR "DynamicPartIndex_COND" = "'TRUE:IpduMEnableJitUpdate|TRUE:IpduMInitializationBySignalValue'"!]
[!VAR "PduRTriggerTxPduId_COND" = "'TRUE:IpduMEnableJitUpdate|TRUE:IpduMInitializationBySignalValue'"!]
[!VAR "QueuePos_COND" = "'TRUE:IpduMDynamicPartQueue'"!]

[!VAR "IpduM_RxCntrInstType_DEF" = "'{}PduLength:PduLengthType|State:uint8'"!]
[!VAR "IpduM_CntrTxInstType_DEF" = "'{}PduLength:PduLengthType|NumCntd:IpduM_NumCntdType|State:uint8|InstLock:uint8'"!]
[!VAR "IpduM_CntrQueueType_DEF" = "'{}ReadIdx:uint8|WriteIdx:uint8'"!]

[!VAR "IpduM_TxCntrRAMDataType_DEF" = "'{}SendTimeout:uint16|TxConfTimeout:uint16'"!]

[!VAR "IpduM_RxPartType_DEF" = "'{}CopyBitField:*IpduM_CopyBitFieldType|OutgoingHandleId:PduIdType|CopyBitFieldArraySize:IpduM_BitfieldArraySizeType|SelectorValue:IpduM_SelectorType'"!]

[!VAR "IpduM_TxPartType_DEF" = "'{}CopyBitField:*IpduM_CopyBitFieldType|CopyBitFieldArraySize:IpduM_BitfieldArraySizeType|PduRTxConfirmationPduId:PduIdType|?PduRTriggerTxPduId:PduIdType|TxConfirmationPduId:PduIdType|TxOutgoingPduId:PduIdType|?JitUpdate:boolean|?DynamicPriority:uint8|?SelectorValue:IpduM_SelectorType'"!]
[!VAR "PduRTriggerTxPduId_COND" = "'TRUE:IpduMEnableJitUpdate|TRUE:IpduMInitializationBySignalValue'"!]
[!VAR "JitUpdate_COND" = "'TRUE:IpduMEnableJitUpdate'"!]
[!VAR "DynamicPriority_COND" = "'TRUE:IpduMDynamicPartQueue'"!]
[!VAR "SelectorValue_COND" = "'TRUE:IpduMTxAutomaticSelector'"!]

[!VAR "IpduM_ContainedRxPduType_DEF" = "'{}ContainedPduHeaderId:uint32|OutgoingPduId:PduIdType'"!]
[!VAR "IpduM_ContainedRxPduType_COND" = "'TRUE:IpduMContainerPduHandlingEnable'"!]

[!VAR "IpduM_StaticContainedRxPduType_DEF" = "'{}IpduMContainedPduOffset:PduLengthType|IpduMUpdateBitBytePosition:PduLengthType|PduLength:PduLengthType|OutgoingPduId:PduIdType|IpduMUpdateBitMask:uint8|HasIpduMUpdateBitPositionSet:boolean'"!]
[!VAR "IpduM_StaticContainedRxPduType_COND" = "'TRUE:IpduMContainerPduHandlingEnable;TRUE:IpduMStaticContainerPduHandlingEnable'"!]

[!VAR "IpduM_ContainerRxPduType_DEF" = "'{}ContainedRxPduFirst:uint32|ContainedRxPduNum:uint32|?QueueRAMOffset:IpduM_DataOffsetType|InstRAMOffset:IpduM_DataOffsetType|PduLength:PduLengthType|InstFirstIdx:uint16|NumInstances:uint8|HeaderSize:uint8'"!]
[!VAR "IpduM_ContainerRxPduType_COND" = "'TRUE:IpduMContainerPduHandlingEnable'"!]
[!VAR "QueueRAMOffset_COND" = "'TRUE:IpduMContainerQueuingRxEnable'"!]

[!VAR "IpduM_RxPathWayType_DEF" = "'{}?StaticPart:*IpduM_RxPartType|DynamicPart:*IpduM_RxPartType|DynamicPduArraySize:uint16|StartBit:IpduM_BitfieldType|EndBit:IpduM_BitfieldType'"!]
[!VAR "StaticPart_COND" = "'TRUE:IpduMStaticPartExists'"!]

[!VAR "IpduM_JitDataType_DEF" = "'{}TxPduId:PduIdType|TxPartIndex:PduIdType'"!]

[!VAR "IpduM_TxPathWayType_DEF" = "'{}?BufferOffset:IpduM_DataOffsetType|Size:PduLengthType|ConfirmationTimeout:IpduM_TxTimeoutType|?StaticPartTxConfHandleId:PduIdType|?StaticPartTrigTxHandleId:PduIdType|?StaticPartIndex:PduIdType|?InitialDynamicPartTrigTxHandleId:PduIdType|?InitialDynamicPartIndex:PduIdType|?QueueOffset:IpduM_DataOffsetType|StartBit:IpduM_BitfieldType|EndBit:IpduM_BitfieldType|DynamicConfirmArraySize:IpduM_SelectorType|UnusedAreasDefault:uint8|TriggerMode:uint8|?QueueSize:uint8'"!]
[!VAR "BufferOffset_COND" = "'FALSE:IpduMZeroCopy'"!]
[!VAR "StaticPartTxConfHandleId_COND" = "'TRUE:IpduMStaticPartExists'"!]
[!VAR "StaticPartTrigTxHandleId_COND" = "'TRUE:IpduMStaticPartExists+TRUE:IpduMEnableJitUpdate|TRUE:IpduMInitializationBySignalValue'"!]
[!VAR "StaticPartIndex_COND" = "'TRUE:IpduMStaticPartExists+TRUE:IpduMEnableJitUpdate|TRUE:IpduMInitializationBySignalValue'"!]
[!VAR "InitialDynamicPartTrigTxHandleId_COND" = "'TRUE:IpduMEnableJitUpdate|TRUE:IpduMInitializationBySignalValue'"!]
[!VAR "InitialDynamicPartIndex_COND" = "'TRUE:IpduMEnableJitUpdate|TRUE:IpduMInitializationBySignalValue'"!]
[!VAR "QueueOffset_COND" = "'TRUE:IpduMDynamicPartQueue'"!]
[!VAR "QueueSize_COND" = "'TRUE:IpduMDynamicPartQueue'"!]

[!VAR "IpduM_ContainerTxPduType_DEF" = "'{}QueueRAMOffset:IpduM_DataOffsetType|CntrDataRAMOffset:IpduM_DataOffsetType|InstRAMOffset:IpduM_DataOffsetType|TxConfListOffset:IpduM_DataOffsetType|IpduMContainerTxSizeThreshold:PduLengthType|PduLength:PduLengthType|IpduMContainerTxHandleId:PduIdType|IpduMContainerTxConfirmationTimeout:uint16|NumCntdMax:uint16|InstFirstIdx:uint16|NumInstances:uint8|?IpduMUnusedAreasDefault:uint8|HeaderSize:uint8|CfgFlags:uint8'"!]

[!VAR "IpduMUnusedAreasDefault_COND" = "'TRUE:IpduMStaticContainerPduHandlingEnable'"!]

[!VAR "IpduM_ContainedTxPduType_DEF" = "'{}IpduMContainedPduHeaderId:uint32|?IpduMContainedPduOffset:PduLengthType|?IpduMUpdateBitBytePosition:PduLengthType|PduLength:PduLengthType|IpduMContainedTxPduHandleId:PduIdType|IpduMContainerTxIndex:PduIdType|IpduMContainedTxPduSendTimeout:uint16|?IpduMUpdateBitMask:uint8|?IpduMContainedTxPduPriority:uint8|CfgFlags:uint8'"!]
[!VAR "IpduMContainedPduOffset_COND" = "'TRUE:IpduMStaticContainerPduHandlingEnable'"!]
[!VAR "IpduMUpdateBitBytePosition_COND" = "'TRUE:IpduMStaticContainerPduHandlingEnable'"!]
[!VAR "IpduMUpdateBitMask_COND" = "'TRUE:IpduMStaticContainerPduHandlingEnable'"!]
[!VAR "IpduMContainedTxPduPriority_COND" = "'TRUE:IpduMContainedTxPduPriorityHandlingEnable'"!]

[!/* PC config struct */!]
[!VAR "IpduM_ConfigType_DEF" = "'{}PlatformSignature:uint32|LcfgSignature:uint32|CfgSignature:uint32|PublicInfoSignature:uint32|?MFTxLookupPathway:*PduIdType|?MFTxLookup:*PduIdType|?RxMFLookup:*PduIdType|?RxCntrInst:*uint32|?TxCntrInst:*uint32|RxHandlerIndices:*uint8|RxPathWay:*IpduM_RxPathWayType|TxPathWay:*IpduM_TxPathWayType|TxPart:*IpduM_TxPartType|?ContainerRxPdu:*IpduM_ContainerRxPduType|?ContainedRxPdu:*IpduM_ContainedRxPduType|?StaticContainedRxPdu:*IpduM_StaticContainedRxPduType|?ContainerTxPdu:*IpduM_ContainerTxPduType|?ContainedTxPdu:*IpduM_ContainedTxPduType|TxContainerPduNum:IpduM_ContainerNumType|TxContainedPduNum:IpduM_ContainedNumType|RxPathWayLen:IpduM_PathWayLenType|TxPathWayLen:IpduM_PathWayLenType|TxPartLen:IpduM_PathWayLenType|TxDynamicPartOffset:IpduM_TxOffsetType|?RxContainerPduNum:IpduM_ContainerNumType|?RxStaticCntdPduNum:IpduM_ContainedNumType|?RxContainerProcessingDeferredNum:IpduM_ContainerNumType|?RxCntrOffset:IpduM_DataOffsetType|?TxCntrOffset:IpduM_DataOffsetType|DataMemMinLen:IpduM_DataOffsetType'"!]

[!VAR "MFTxLookupPathway_COND" = "'TRUE:IpduMMultiMF'"!]
[!VAR "MFTxLookup_COND" = "'TRUE:IpduMMultiMF'"!]
[!VAR "RxMFLookup_COND" = "'TRUE:IpduMMultiMF'"!]
[!VAR "RxCntrInst_COND" = "'TRUE:IpduMContainerPduHandlingEnable'"!]
[!VAR "ContainerRxPdu_COND" = "'TRUE:IpduMContainerPduHandlingEnable'"!]
[!VAR "ContainedRxPdu_COND" = "'TRUE:IpduMContainerPduHandlingEnable'"!]
[!VAR "StaticContainedRxPdu_COND" = "'TRUE:IpduMStaticContainerPduHandlingEnable'"!]
[!VAR "ContainerTxPdu_COND" = "'TRUE:IpduMContainerPduHandlingEnable'"!]
[!VAR "ContainedTxPdu_COND" = "'TRUE:IpduMContainerPduHandlingEnable'"!]
[!VAR "TxCntrInst_COND" = "'TRUE:IpduMContainerPduHandlingEnable'"!]
[!VAR "TxContainerPduNum_COND" = "'TRUE:IpduMContainerPduHandlingEnable'"!]
[!VAR "TxCntrOffset_COND" = "'TRUE:IpduMContainerPduHandlingEnable'"!]
[!VAR "TxContainedPduNum_COND" = "'TRUE:IpduMContainerPduHandlingEnable'"!]
[!VAR "RxContainerPduNum_COND" = "'TRUE:IpduMContainerPduHandlingEnable'"!]
[!VAR "RxStaticCntdPduNum_COND" = "'TRUE:IpduMStaticContainerPduHandlingEnable'"!]
[!VAR "RxContainerProcessingDeferredNum_COND" = "'TRUE:IpduMContainerPduHandlingEnable'"!]
[!VAR "RxCntrOffset_COND" = "'TRUE:IpduMContainerPduHandlingEnable'"!]
[!VAR "TxCntrOffset_COND" = "'TRUE:IpduMContainerPduHandlingEnable'"!]

[!SELECT "IpduMConfig/*[1]"!]

[!/* In order to simplify the definition of IpduM_ConfigLayoutType, establish the CopyBitFields separately */!]
[!VAR "RxPrtFieldStr" = "''"!]
[!LOOP "node:order(IpduMRxPathway/*, 'node:IpduMRxIndication/IpduMRxHandleId')"!]
[!VAR "HandleId" = "IpduMRxIndication/IpduMRxHandleId"!]
[!IF "(node:exists(IpduMRxIndication/IpduMRxStaticPart))"!]
[!VAR "NrOfStaticCopyBitFields" = "num:i(count(IpduMRxIndication/IpduMRxStaticPart/IpduMSegment/*))"!]
[!VAR "RxPrtFieldStr"!][!"$RxPrtFieldStr"!]|IpduM_Rx_StaticPart_CopyBitField_[!"$HandleId"!]:[[!"$NrOfStaticCopyBitFields"!]]IpduM_CopyBitFieldType|IpduM_Rx_StaticPart_[!"$HandleId"!]:IpduM_RxPartType|[!ENDVAR!]
[!ENDIF!]
[!VAR "NrOfDynamicCopyBitFields" = "num:i(count(IpduMRxIndication/IpduMRxDynamicPart/*/IpduMSegment/*))"!]
[!VAR "RxPrtFieldStr"!][!"$RxPrtFieldStr"!]|IpduM_Rx_DynamicPart_CopyBitField_[!"$HandleId"!]:[[!"$NrOfDynamicCopyBitFields"!]]IpduM_CopyBitFieldType|IpduM_Rx_DynamicPart_[!"$HandleId"!]:[[!"num:i(count(IpduMRxIndication/IpduMRxDynamicPart/*))"!]]IpduM_RxPartType|[!ENDVAR!]
[!ENDLOOP!]

[!/* End of CopyBitFields */!]

[!VAR "IpduM_ConfigLayoutType_DEF"!]{}IpduM_RootConfig:IpduM_ConfigType|?IpduM_MFTxLookupPathway:[[!"num:i($numTxPathway + (count(as:modconf('IpduM')[1]/IpduMGeneral/IpduMTxProcessing/*) + 1) * 2)"!]]PduIdType|?IpduM_MFTxLookup:[[!"num:i($numTxContainer)"!]]PduIdType|?IpduM_RxMFLookup:[[!"$NoOfDeferredContainers"!]]PduIdType|?IpduM_RxCntrInstanceBuffs:[[!"$NumRxCntrInst"!]]uint32|?IpduM_CntrTxInstanceBuffs:[[!"$numCntrTxBuffs"!]]uint32|IpduM_RxIndHandlerIdx:[[!"node:when($numRxTotal != 0, $numRxTotal, 1)"!]]uint8|[!"$RxPrtFieldStr"!]?IpduM_RxPathWay:[[!"$numRxPathway"!]]IpduM_RxPathWayType|?IpduM_TxCopyBitField:[[!"num:i(count(IpduMTxPathway/*/IpduMTxRequest/IpduMTxStaticPart/IpduMSegment/*) + count(IpduMTxPathway/*/IpduMTxRequest/IpduMTxDynamicPart/*/IpduMSegment/*))"!]]IpduM_CopyBitFieldType|?IpduM_TxPart:[[!"num:i($DynamicIdMax + 1)"!]]IpduM_TxPartType|?IpduM_TxPathWay:[[!"$numTxPathway"!]]IpduM_TxPathWayType|?IpduM_ContainerRxPdu:[[!"num:i($NrOfRxContainers)"!]]IpduM_ContainerRxPduType|?IpduM_ContainedRxPdu:[[!"num:i(node:when($RxContainerAcceptAllNoRefOnly = 'true', $NrOfRxContained - $NumStaticRxCntdPdus, $NrOfRxContained - $NumStaticRxCntdPdus + $NumCntdRxAcceptConf))"!]]IpduM_ContainedRxPduType|?IpduM_StaticContainedRxPdu:[[!"$NumStaticRxCntdPdus"!]]IpduM_StaticContainedRxPduType|?IpduM_ContainerTxPdu:[[!"$numTxContainer"!]]IpduM_ContainerTxPduType|?IpduM_ContainedTxPdu:[[!"$numContainedTx"!]]IpduM_ContainedTxPduType|[!ENDVAR!]
[!"var:set(concat($IPDUM_CONFIGLAYOUTTYPE_VARIANT, '_DEF'), $IpduM_ConfigLayoutType_DEF)"!]

[!VAR "IpduM_MFTxLookupPathway_COND"!]TRUE:IpduMMultiMF+TRUE:[!"$numTxPathway > 0"!][!ENDVAR!]
[!VAR "IpduM_MFTxLookup_COND"!]TRUE:IpduMMultiMF+TRUE:[!"$numTxContainer > 0"!][!ENDVAR!]
[!VAR "IpduM_RxMFLookup_COND"!]TRUE:IpduMMultiMF+TRUE:[!"$NoOfDeferredContainers > 0"!][!ENDVAR!]
[!VAR "IpduM_RxPathWay_COND"!]TRUE:[!"$numRxPathway > 0"!][!ENDVAR!]
[!VAR "IpduM_RxCntrInstanceBuffs_COND"!]TRUE:[!"$NumRxCntrInst > 0"!][!ENDVAR!]
[!VAR "IpduM_CntrTxInstanceBuffs_COND"!]TRUE:[!"$numCntrTxBuffs > 0"!][!ENDVAR!]
[!VAR "IpduM_TxCopyBitField_COND"!]TRUE:[!"$numTxPathway > 0"!][!ENDVAR!]
[!VAR "IpduM_TxPart_COND"!]TRUE:[!"$numTxPathway > 0"!][!ENDVAR!]
[!VAR "IpduM_TxPathWay_COND"!]TRUE:[!"$numTxPathway > 0"!][!ENDVAR!]
[!VAR "IpduM_ContainerRxPdu_COND"!]TRUE:IpduMContainerPduHandlingEnable+TRUE:[!"$NrOfRxContainers > 0"!][!ENDVAR!]
[!VAR "IpduM_ContainedRxPdu_COND"!]TRUE:IpduMContainerPduHandlingEnable+TRUE:[!"num:i($NrOfRxContained - $NumStaticRxCntdPdus) > num:i(0)"!][!ENDVAR!]
[!VAR "IpduM_StaticContainedRxPdu_COND"!]TRUE:IpduMStaticContainerPduHandlingEnable+TRUE:[!"$NumStaticRxCntdPdus > 0"!][!ENDVAR!]
[!VAR "IpduM_ContainerTxPdu_COND"!]TRUE:[!"$numTxContainer > 0"!][!ENDVAR!]
[!VAR "IpduM_ContainedTxPdu_COND"!]TRUE:[!"$numContainedTx > 0"!][!ENDVAR!]

[!ENDSELECT!][!// IpduM Root Config node

[!/* Values are from IpduM_PBNumericVars.m */!]


[!ENDIF!][!// multiple inclusion protection
[!ENDNOCODE!][!//
