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

[!INCLUDE "IpduM_GenParams.m"!]
[!INCLUDE "IpduM_HelperMacros.m"!]

[!NOCODE!]
[!IF "not(var:defined('IPDUM_MACROS_M'))"!]
[!VAR "IPDUM_MACROS_M"="'true'"!]

[!/* ComStack_FindNextOne() needs a terminator bit set */!]
[!VAR "IpduM_ExtraBitsForSearchAlgo" = "num:i(1)"!]
[!VAR "uint32_ALG" = "asc:getDataTypeAlignment('uint32')"!]
[!IF "$uint32_ALG = ''"!]
  [!VAR "uint32_ALG" = "4"!]
[!ENDIF!]

[!/*
****************************************************************************************************
* Helper macro calculates the necessary bitfield size in uint32s
* Suitable to be used for ComStack_FindNextOne() searchable bitfields.
* The result is in "BFSizeU32"
****************************************************************************************************
*/!]
[!MACRO "GetBFSizeInUINT32", "numElems"!][!NOCODE!]
[!VAR "BFSizeU32" = "num:i(($numElems + $IpduM_ExtraBitsForSearchAlgo + 31) div 32)"!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* Helper macro that facilitates the alignment of RAM data.
* The result resides in "valPadded".
****************************************************************************************************
*/!]
[!MACRO "IpduMHelpAlign","valToExtend","Alignment"="$uint32_ALG"!][!NOCODE!]
[!VAR "valPadded" = "num:i($valToExtend)"!]
[!VAR "padVal" = "num:i($Alignment - ($valToExtend mod $Alignment))"!]
[!IF "$padVal != $Alignment"!]
  [!VAR "valPadded" = "num:i($valToExtend + $padVal)"!]
[!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO establishes the IpduM_DataMem slice allocated to TxPathways
* Within this file it's not grouped with the others because it's used by IpduMCalculateRxQueueSlices
****************************************************************************************************
*/!]
[!MACRO "GetTxPathwaySize"!][!NOCODE!]
[!VAR "IpduMDataMemPathwaySize" = "num:i(0)"!]
[!SELECT "as:modconf('IpduM')/IpduMConfig/*[1]"!]
[!IF "node:exists(IpduMTxPathway/*)"!]
  [!VAR "TxPathWayTotalCount" = "num:i(count(IpduMTxPathway/*))"!]
  [!IF "$IpduMDynamicPartQueue = 'true'"!]
    [!VAR "TxQueueTotalEntries" = "num:i(sum(IpduMTxPathway/*/IpduMTxRequest/IpduMQueueSize))"!]
  [!ELSE!]
    [!VAR "TxQueueTotalEntries" = "num:i(0)"!]
  [!ENDIF!]
  [!IF "$IpduMZeroCopy = 'true'"!]
    [!VAR "TxTotalBufferSize" = "num:i(0)"!]
  [!ELSE!]
    [!CALL "IpduMHelpAlign","valToExtend"="num:i(sum(node:refs(./IpduMTxPathway/*/IpduMTxRequest/IpduMOutgoingPduRef)/PduLength))"!]
    [!VAR "TxTotalBufferSize" = "$valPadded"!]
  [!ENDIF!]
[!IF "not(var:defined('IpduM_TxDataType_SIZE'))"!]
  [!VAR "IpduM_TxDataType_SIZE" = "0"!]
[!ENDIF!]
[!IF "not(var:defined('IpduM_QueueEntryType_SIZE'))"!]
  [!VAR "IpduM_QueueEntryType_SIZE" = "0"!]
[!ENDIF!]
[!VAR "IpduMDataMemPathwaySize" = "num:i(($IpduM_TxDataType_SIZE * $TxPathWayTotalCount) + ($IpduM_QueueEntryType_SIZE * $TxQueueTotalEntries) + $TxTotalBufferSize)"!]
[!ENDIF!][!//IpduMTxPathway
[!ENDSELECT!]
[!VAR "IpduMDataMemMinLenCalc" = "$IpduMDataMemPathwaySize"!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* Macro used to established whether byte swapping the header id is necessary
****************************************************************************************************
*/!]
[!MACRO "IpduMIsHeaderSwapNeeded"!][!NOCODE!]
[!VAR "IpduMHeaderSwapNeeded" = "false()"!]
[!VAR "IHBO" = "'LE'"!]
[!IF "node:exists(as:modconf('IpduM')[1]/IpduMGeneral/IpduMHeaderByteOrder) and node:value(as:modconf('IpduM')[1]/IpduMGeneral/IpduMHeaderByteOrder) = 'IPDUM_BIG_ENDIAN'"!]
  [!VAR "IHBO" = "'BE'"!]
[!ENDIF!]
[!IF "asc:getEndianness() != $IHBO"!]
  [!VAR "IpduMHeaderSwapNeeded" = "true()"!]
[!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* Macro swaps an uint32 (byte reversal)
* The output is stored in the variable IpduMHeaderID
****************************************************************************************************
*/!]
[!MACRO "IpduMGetHeaderID", "headerIDVal"!][!NOCODE!]
[!IF "not(var:defined('IpduMHeaderSwapNeeded'))"!]
  [!CALL "IpduMIsHeaderSwapNeeded"!]
[!ENDIF!]
[!IF "$IpduMHeaderSwapNeeded = 'false'"!]
  [!VAR "IpduMHeaderID" = "num:i($headerIDVal)"!]
[!ELSE!]
  [!VAR "IpduMHeaderID" = "num:i(bit:or(bit:or(bit:or(bit:shr(bit:and($headerIDVal,4278190080),24), bit:shr(bit:and($headerIDVal, 16711680),8)), bit:shl(bit:and($headerIDVal, 65280),8)), bit:shl(bit:and($headerIDVal, 255), 24)))"!]
[!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* Calculates the offsets of the memory allocated for DEFERRED Rx PDUs
* Result is in the variable "rxQLookupStr"
* Additionally sets the variables
*   IpduMRxBFSizeU32: Size of bitfield for the containers
*   IPDUM_RX_DATA_OFFSET: Start offset for Rx Containers
*   rxPduBuffLookupStr: for instances PDU storage allocation
*   IpduMDataMemMinLenCalc: Size of RAM with Rx Containers considered (+ TxPathways)
****************************************************************************************************
*/!]
[!MACRO "IpduMCalculateRxQInstOffsets"!][!NOCODE!]
[!VAR "IPDUM_RX_DATA_OFFSET" = "num:i(0)"!]
[!IF "$NumRxCntrInst > 0"!]
[!VAR "rxQLookupStr" = "''"!]
[!VAR "rxPduBuffLookupStr" = "''"!]
[!CALL "GetBFSizeInUINT32", "numElems"="$NoOfDeferredContainers"!]
[!VAR "IpduMRxBFSizeU32" = "$BFSizeU32"!]
[!VAR "defListSize" = "num:i(asc:getDataTypeSize('uint32') * ($IpduMRxBFSizeU32 * (1 + count(as:modconf('IpduM')[1]/IpduMGeneral/IpduMRxProcessing/*))))"!]
[!/* IpduMDataMemMinLenCalc at this point holds the value necessary for TxPathways */!]
[!CALL "IpduMHelpAlign", "valToExtend" = "$IpduMDataMemMinLenCalc"!]
[!VAR "IPDUM_RX_DATA_OFFSET" = "num:i($valPadded)"!]
[!VAR "rxBaseAddr" = "$IPDUM_RX_DATA_OFFSET + $defListSize"!]
[!LOOP "node:order(as:modconf('IpduM')[1]/IpduMConfig/*[1]/IpduMContainerRxPdu/*[IpduMContainerPduProcessing='IPDUM_PROCESSING_DEFERRED'], 'node:IpduMContainerRxHandleId')"!]
  [!IF "node:exists(./IpduMContainerQueueSize)"!]
    [!/* !LINKSTO IpduM.EB.CntrRx.QueuingNumInstances,1 */!]
    [!VAR "nInst" = "num:i(node:value(./IpduMContainerQueueSize) + 1)"!]
    [!CALL "IpduMHelpAlign", "valToExtend" = "$rxBaseAddr", "Alignment" = "$IpduM_CntrQueueType_ALG"!]
    [!VAR "qOfs" = "$valPadded"!]
    [!CALL "IpduMHelpAlign", "valToExtend" = "$qOfs + $IpduM_CntrQueueType_SIZE", "Alignment" = "$IpduM_RxCntrInstType_ALG"!]
    [!VAR "instOfs" = "$valPadded"!]
    [!VAR "rxBaseAddr" = "$instOfs + ($IpduM_RxCntrInstType_SIZE * $nInst)"!]
  [!ELSE!]
    [!VAR "nInst" = "num:i(1)"!]
    [!CALL "IpduMHelpAlign", "valToExtend" = "$rxBaseAddr", "Alignment" = "$IpduM_RxCntrInstType_ALG"!]
    [!VAR "instOfs" = "$valPadded"!]
    [!VAR "qOfs" = "0"!]
    [!VAR "rxBaseAddr" = "$instOfs + $IpduM_RxCntrInstType_SIZE"!]
  [!ENDIF!]
  [!VAR "rxQLookupStr" = "concat($rxQLookupStr, node:name(.), ':', num:i($qOfs), ':', num:i($instOfs), '|')"!]
  [!CALL "IpduMGetCntrLength", "cntrPath" = "as:path(.)"!]
  [!VAR "rxPduBuffLookupStr" = "concat($rxPduBuffLookupStr, node:name(.), ':', $nInst, ';', $IpduMCntrLen, '|')"!]
[!ENDLOOP!]
[!VAR "rxPduBuffLookupStrTmp" = "''"!]
[!VAR "pduOfs" = "$rxBaseAddr"!]
[!LOOP "text:split($rxPduBuffLookupStr, '|')"!]
  [!VAR "rxPduBuffLookupStrTmp" = "concat($rxPduBuffLookupStrTmp, text:split(., ':')[1], ':')"!]
  [!VAR "nrInst" = "text:split(text:split(., ':')[2], ';')[1]"!]
  [!VAR "pduLen" = "text:split(text:split(., ':')[2], ';')[2]"!]
  [!VAR "instPduOfs" = "''"!]
  [!FOR "x" = "1" TO "$nrInst"!]
    [!CALL "IpduMHelpAlign", "valToExtend" = "$pduOfs"!]
    [!VAR "instPduOfs" = "concat($instPduOfs, $valPadded, ';')"!]
    [!VAR "pduOfs" = "$valPadded + $pduLen"!]
  [!ENDFOR!]
  [!VAR "rxPduBuffLookupStrTmp" = "concat($rxPduBuffLookupStrTmp, $instPduOfs, '|')"!]
[!ENDLOOP!]
[!VAR "rxPduBuffLookupStr" = "$rxPduBuffLookupStrTmp"!]
[!CALL "IpduMGetCntrLength", "cntrPath" = "as:path(node:order(as:modconf('IpduM')[1]/IpduMConfig/*[1]/IpduMContainerRxPdu/*[IpduMContainerPduProcessing='IPDUM_PROCESSING_DEFERRED'], 'node:IpduMContainerRxHandleId')[last()])"!]
[!VAR "IpduMDataMemMinLenCalc" = "num:i(text:split(substring-after(text:split($rxPduBuffLookupStr, '|')[last()], ':'),';')[last()] + $IpduMCntrLen)"!]
[!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* Sorts the Tx Containers because node:order() and node:when() can't
****************************************************************************************************
*/!]
[!MACRO "GetIpduMTxContainersLst"!][!NOCODE!]
[!VAR "IpduMTxContainersLst" = "''"!]
[!VAR "IpduMCntrIdx" = "num:i(0)"!]
[!LOOP "node:order(as:modconf('IpduM')/IpduMConfig/*[1]/IpduMContainerTxPdu/*[node:exists(IpduMContainerTxHandleId)], 'node:IpduMContainerTxHandleId')"!]
  [!VAR "IpduMTxContainersLst"!][!"$IpduMTxContainersLst"!];[!"as:path(.)"!]:[!"$IpduMCntrIdx"!][!ENDVAR!]
  [!VAR "IpduMCntrIdx" = "num:i($IpduMCntrIdx + 1)"!]
[!ENDLOOP!]
[!LOOP "node:order(as:modconf('IpduM')/IpduMConfig/*[1]/IpduMContainerTxPdu/*[not(node:exists(IpduMContainerTxHandleId))], 'node:name(.)')"!]
  [!VAR "IpduMTxContainersLst"!][!"$IpduMTxContainersLst"!];[!"as:path(.)"!]:[!"$IpduMCntrIdx"!][!ENDVAR!]
  [!VAR "IpduMCntrIdx" = "num:i($IpduMCntrIdx + 1)"!]
[!ENDLOOP!]
[!VAR "IpduMTxContainersLst" = "concat($IpduMTxContainersLst, ';')"!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* Gets the Contained collections semantics based on Container referenced
* (Gets backward reference from Container to Contained)
****************************************************************************************************
*/!]
[!MACRO "GetIpduMCntrCollectSemantics", "Container"!][!NOCODE!]
[!VAR "parentNode" = "node:path($Container)"!]
[!LOOP "../../IpduMContainedTxPdu/*"!]
  [!IF "node:path(node:value(./IpduMContainedTxInContainerPduRef))=$parentNode"!]
    [!VAR "CollectionSemantics" = "node:value(./IpduMContainedTxPduCollectionSemantics)"!]
    [!BREAK!]
  [!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* Gets the number of containeds that have different priorities.
* (If we have containeds with different priorities then it makes sense to sort them)
****************************************************************************************************
*/!]
[!MACRO "GetIpduMCntdTxPrioFlag", "Container"!][!NOCODE!]
[!VAR "prios" = "text:join((../../IpduMContainedTxPdu/*[node:exists(IpduMContainedTxPduPriority) and node:path(node:value(./IpduMContainedTxInContainerPduRef)) = $Container]/IpduMContainedTxPduPriority), ':')"!]
[!IF "text:match($prios, '^\d+')"!]
  [!VAR "prioFlag" = "node:when(num:min(text:split($prios, ':')) != num:max(text:split($prios, ':')), num:i(1), num:i(0))"!]
[!ELSE!]
  [!VAR "prioFlag" = "num:i(0)"!]
[!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* Sets the variable IpduMCntdInCntrIdx to the index of the container obtained by the sorting
* of the containers using the MACRO GetIpduMTxContainersLst.
* The supplied parameter is expected to be the path of a node
****************************************************************************************************
*/!]
[!MACRO "GetIpduMCntdInCntrIdx", "ContainerPath"!][!NOCODE!]
[!VAR "IpduMCntdInCntrIdx" = "num:i(-1)"!]
[!VAR "entry" = "substring-before(substring-after($IpduMTxContainersLst , concat(';',$ContainerPath,':')) , ';')"!]
[!IF "$entry != ''"!]
  [!VAR "IpduMCntdInCntrIdx" = "num:i($entry)"!]
[!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]


[!/*
****************************************************************************************************
* Calculates the offsets of the memory allocated for Tx Containers.
* Result is in the variable "txLookupStr"
* Additionally sets the variables:
*   IpduMBFSizeTxU32: Size of bitfield for the containers
*   txBuffLookupStr: Lookup string for
*   IPDUM_TX_DATA_OFFSET: Start offset for Tx Containers
*   IpduMDataMemMinLenCalc: Size of RAM, final (Pathways + Rx Containers + Tx Containers)
****************************************************************************************************
*/!]
[!MACRO "IpduMCalculateTxRAMDataOffsets"!]
[!VAR "IPDUM_TX_DATA_OFFSET" = "num:i(0)"!]
[!IF "$numTxContainer > 0"!]
[!IF "not(var:defined('IpduMTxContainersLst'))"!][!CALL "GetIpduMTxContainersLst"!][!ENDIF!]
[!CALL "GetBFSizeInUINT32", "numElems" = "$numTxContainer"!]
[!VAR "IpduMBFSizeTxU32" = "$BFSizeU32"!]
[!VAR "txMFChangedBFSize" = "num:i(asc:getDataTypeSize('uint32') * ($IpduMBFSizeTxU32 * (1 + count(as:modconf('IpduM')[1]/IpduMGeneral/IpduMTxProcessing/*))))"!]
[!/* This comes just after the RX PDUs */!]
[!CALL "IpduMHelpAlign", "valToExtend" = "$IpduMDataMemMinLenCalc"!]
[!VAR "IPDUM_TX_DATA_OFFSET" = "$valPadded"!]
[!CALL "IpduMHelpAlign", "valToExtend" = "$IPDUM_TX_DATA_OFFSET + $txMFChangedBFSize", "Alignment" = "$uint32_ALG"!]
[!VAR "txBaseAddr" = "$valPadded"!]
[!VAR "txLookupStr" = "''"!]
[!VAR "txBuffLookupStr" = "''"!]
[!VAR "txConfListLookupStr" = "''"!]
[!VAR "nInst" = "0"!]
[!FOR "c" = "0" TO "$numTxContainer - 1"!]
[!SELECT "as:ref(text:split(string(text:split($IpduMTxContainersLst,';')[position()-1 = $c]),':')[1])"!]
  [!CALL "IpduMGetCntrLength", "cntrPath" = "as:path(.)"!]
  [!VAR "pduBuffLen" = "$IpduMCntrLen"!]
  [!IF "node:exists(./IpduMContainerQueueSize)"!]
    [!VAR "nInst" = "1 + node:value(./IpduMContainerQueueSize)"!]
  [!ELSE!]
    [!VAR "nInst" = "1"!]
  [!ENDIF!]
  [!CALL "IpduMHelpAlign", "valToExtend" = "$txBaseAddr", "Alignment" = "$IpduM_CntrQueueType_ALG"!]
  [!VAR "qOfs" = "$valPadded"!]
  [!CALL "IpduMHelpAlign", "valToExtend" = "$qOfs + $IpduM_CntrQueueType_SIZE", "Alignment" = "$IpduM_TxCntrRAMDataType_ALG"!]
  [!VAR "ramDataOfs" = "$valPadded"!]
  [!CALL "IpduMHelpAlign", "valToExtend" = "$ramDataOfs + $IpduM_TxCntrRAMDataType_SIZE", "Alignment" = "$IpduM_CntrTxInstType_ALG"!]
  [!VAR "instOfs" = "$valPadded"!]
  [!VAR "txBaseAddr" = "$instOfs + ($nInst * $IpduM_CntrTxInstType_SIZE)"!]
  [!VAR "nBuffInst" = "$nInst"!]
  [!IF "'IPDUM_COLLECT_QUEUED' = ../../IpduMContainedTxPdu/*[node:path(node:ref(IpduMContainedTxInContainerPduRef)) = node:path(node:current())][1]/IpduMContainedTxPduCollectionSemantics"!]
      [!IF "./IpduMContainerHeaderSize = 'IPDUM_HEADERTYPE_SHORT'"!]
      [!VAR "txCnfNumElem" = "num:i(ceiling($IpduMCntrLen div num:i(5)) + 1)"!]
    [!ELSE!]
      [!VAR "txCnfNumElem" = "num:i(ceiling($IpduMCntrLen div num:i(9)) + 1)"!]
    [!ENDIF!]
  [!ELSE!]
  [!/* LAST_IS_BEST or HEADERTYPE_NONE */!]
    [!VAR "txCnfNumElem" = "num:i(count(../../IpduMContainedTxPdu/*[node:path(node:ref(IpduMContainedTxInContainerPduRef)) = node:path(node:current())]))"!]
      [!/* LAST_IS_BEST TT is assembled on the buffer provided by PduR */!]
    [!IF "./IpduMContainerTxTriggerMode = 'IPDUM_DIRECT'"!]
      [!VAR "nBuffInst" = "1"!]
    [!ELSE!]
      [!VAR "nBuffInst" = "0"!]
      [!VAR "pduBuffLen" = "0"!]
    [!ENDIF!]
  [!ENDIF!]
  [!VAR "txLookupStr" = "concat($txLookupStr, node:name(.), ':', $qOfs, ':', $ramDataOfs, ':', $instOfs, ':', $txCnfNumElem, ':', num:i($nInst), ':', num:i($nBuffInst), '|')"!]
  [!/* All containers have an entry otherwise can't check what grep returns */!]
  [!VAR "txBuffLookupStr" = "concat($txBuffLookupStr, node:name(.), ':', $pduBuffLen, ':', $nBuffInst, '|')"!]
  [!VAR "txConfListLookupStr" = "concat($txConfListLookupStr, node:name(.), ':', num:i($nInst * $txCnfNumElem), '|')"!]
[!ENDSELECT!]
[!ENDFOR!]
[!CALL "IpduMHelpAlign", "valToExtend" = "$txBaseAddr", "Alignment" = "$PduIdType_ALG"!]
[!VAR "txConfOfs" = "$valPadded"!]
[!VAR "txConfListLookupStrTmp" = "''"!]
[!LOOP "text:split($txConfListLookupStr, '|')"!]
  [!VAR "txConfListLookupStrTmp" = "concat($txConfListLookupStrTmp, text:split(., ':')[1], ':', $txConfOfs, '|')"!]
  [!CALL "IpduMHelpAlign", "valToExtend" = "$txConfOfs + ($PduIdType_SIZE * text:split(., ':')[2])", "Alignment" = "$PduIdType_ALG"!]
  [!VAR "txConfOfs" = "$valPadded"!]
[!ENDLOOP!]
[!VAR "txConfListLookupStr" = "$txConfListLookupStrTmp"!]
[!VAR "txBuffLookupStrTmp" = "''"!]
[!CALL "IpduMHelpAlign", "valToExtend" = "$txConfOfs"!]
[!VAR "buffOfs" = "$valPadded"!]
[!LOOP "text:split($txBuffLookupStr, '|')"!]
  [!VAR "bLen" = "text:split(., ':')[2]"!]
  [!VAR "txBuffLookupStrTmp" = "concat($txBuffLookupStrTmp, text:split(., ':')[1], ':')"!]
  [!IF "$bLen > 0"!]
    [!/* Align all the buffers to u32 */!]
    [!VAR "bStr" = "''"!]
    [!FOR "b" = "1" TO "text:split(., ':')[3]"!]
      [!CALL "IpduMHelpAlign", "valToExtend" = "$buffOfs"!]
      [!VAR "bStr" = "concat($bStr, $valPadded, ';')"!]
      [!VAR "buffOfs" = "$valPadded + $bLen"!]
    [!ENDFOR!]
  [!ENDIF!]
  [!VAR "txBuffLookupStrTmp" = "concat($txBuffLookupStrTmp, node:when($bLen > 0, $bStr, num:i(0)), '|')"!]
[!ENDLOOP!]
[!VAR "txBuffLookupStr" = "$txBuffLookupStrTmp"!]
[!VAR "IpduMDataMemMinLenCalc" = "num:i($buffOfs)"!]
[!ENDIF!]
[!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO establishes IpduM_DateMem, the memory required for the configuration.
* IpduM_DataMem is made up of 3 chunks, Pathway, ContainerRxPdu and ContainerTxPdu related data.
* The offsets and length are set in the 4 associated variables.
* The values are strings.
* If the required memory is greater than the configured IpduMDataMemSize an error is issued.
****************************************************************************************************
*/!]

[!MACRO "GetDataMemSlices"!][!NOCODE!]
[!IF "not(var:defined('IpduMDataMemMinLenCalc'))"!]
  [!CALL "GetTxPathwaySize"!]
  [!CALL "IpduMCalculateRxQInstOffsets"!]
  [!CALL "IpduMCalculateTxRAMDataOffsets"!]
[/* !LINKSTO IPDUM.EB.IPDUM562,2 */]
[!IF "($IpduMDataMemSize > 0) and ($IpduMDataMemMinLenCalc > $IpduMDataMemSize)"!]
  [!ERROR!]The memory required is [!"$IpduMDataMemMinLenCalc"!] bytes, IpduMDataMemSize is set to [!"$IpduMDataMemSize"!][!ENDERROR!]
[!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* Create three lists of PDU names and the corresponding handle IDs.
* - SrcPduRefToHandleIdList will hold all source PDU IDs of all PduR routing paths.
* - DestPduRefToHandleIdList will hold all destination PDU IDs of all PduR routing paths.
* - ComPduRefToHandleIdList will hold the ComHandleIds of the ComIpdus(configured to sent normal Ipdus)
*   which references the same global PDU that is referenced by request messages.
* Each list is realized as continuous string. The string contains tuples separated by ";". Each
* tuple contains two elements separated by ":". First element of tuple is the PDU reference. The
* Second element of the tuple is the corresponding handle ID.
* A list, for example, looks like this: Pdu_0:321;Pdu_1:322;Pdu_3:323;
* The location of the PDU AUTOSAR/TOP-LEVEL-PACKAGES/EcuC/ELEMENTS/EcuC/EcucPduCollection/Pdu/ is
* not shown in the example.
****************************************************************************************************
*/!]
[!VAR "SrcPduRefToHandleIdList"="''"!]
[!VAR "DestPduRefToHandleIdList"="''"!]
[!VAR "ComPduRefToHandleIdList"="''"!]
[!// Iterate over all routing paths to collect the source and destination PDU-IDs
[!LOOP "as:modconf('PduR')[1]/PduRRoutingTables/*[1]/PduRRoutingTable/*[1]/PduRRoutingPath/*"!]
    [!// Get the source PDU-ID of a routing path
    [!VAR "SrcHandleId" = "./PduRSrcPdu/PduRSourcePduHandleId"!]
    [!IF "num:isnumber($SrcHandleId) and node:refexists(./PduRSrcPdu/PduRSrcPduRef)"!]
        [!VAR "SrcPduRef" = "node:path(node:ref(./PduRSrcPdu/PduRSrcPduRef))"!]
        [!VAR "SrcPduRefToHandleIdList"!][!"$SrcPduRefToHandleIdList"!];[!"$SrcPduRef"!]:[!"$SrcHandleId"!][!ENDVAR!]
    [!ENDIF!]
    [!// Get the destination PDU-IDs of a routing path
    [!LOOP "./PduRDestPdu/*"!]
        [!IF "node:exists(./PduRDestPduHandleId)"!]
            [!VAR "DestHandleId" = "./PduRDestPduHandleId"!]
            [!IF "num:isnumber($DestHandleId) and node:refexists(./PduRDestPduRef)"!]
                [!VAR "DestPduRef" = "node:path(node:ref(./PduRDestPduRef))"!]
                [!VAR "DestPduRefToHandleIdList"!][!"$DestPduRefToHandleIdList"!];[!"$DestPduRef"!]:[!"$DestHandleId"!][!ENDVAR!]
            [!ENDIF!]
        [!ENDIF!]
    [!ENDLOOP!]
[!ENDLOOP!]

[!LOOP "as:modconf('Com')[1]/ComConfig/*[1]/ComIPdu/*[ComIPduDirection = 'SEND' and ComIPduType = 'NORMAL']"!]
    [!IF "node:exists(./ComIPduHandleId)"!]
            [!VAR "ComHandleId" = "./ComIPduHandleId"!]
            [!IF "num:isnumber($ComHandleId) and node:refexists(./ComPduIdRef)"!]
                [!VAR "ComPduRef" = "node:path(node:ref(./ComPduIdRef))"!]
                [!VAR "ComPduRefToHandleIdList"!][!"$ComPduRefToHandleIdList"!];[!"$ComPduRef"!]:[!"$ComHandleId"!][!ENDVAR!]
            [!ENDIF!]
    [!ENDIF!]
[!ENDLOOP!]

[!/*
****************************************************************************************************
* MACRO to get the ComIPduHandleId with which Com_TriggerIPDUSend() has to be called. The macro parameter
* "IpduMRequestMessagePduRef" must reference this PDU. The PDU-ID is stored in the global variable "PduID".
*
* GetOutputPduId is called to check whether the Pdu referenced by IpduMRequestMessage is
* also referenced by any of the PduRSrcPdu.
****************************************************************************************************
*/!]
[!MACRO "GetComHandleId","IpduMRequestMessagePduRef"!][!NOCODE!]
    [!VAR "IpduMRequestMsgPduRefPath" = "node:path(node:ref($IpduMRequestMessagePduRef))"!]
    [!CALL "GetOutputPduId","IpduMOutgoingPduRef"="$IpduMRequestMessagePduRef"!]
    [!IF "$FoundIds ='true'"!]
      [!VAR "FoundIds" = "false()"!]
      [!LOOP "text:split($ComPduRefToHandleIdList,';')"!]
          [!IF "string(text:split(.,':')[1]) = $IpduMRequestMsgPduRefPath"!]
              [!VAR "PduID" = "text:split(.,':')[2]"!]
              [!VAR "FoundIds" = "true()"!]
          [!ENDIF!]
      [!ENDLOOP!]
      [!IF "$FoundIds = 'false'"!]
          [!ERROR!] The Com module does not reference the PDU '[!"$IpduMRequestMsgPduRefPath"!]' [!ENDERROR!]
      [!ENDIF!]
    [!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to get the PduR source PDU ID the PduR has to be called with. The macro parameter
* "IpduMOutgoingPduRef" must reference this PDU. The PDU-ID is stored in the global variable "PduID".
****************************************************************************************************
*/!]
[!MACRO "GetOutputPduId","IpduMOutgoingPduRef"!][!NOCODE!]
    [!VAR "IpduMOutgoingPduRefPath" = "node:path(node:ref($IpduMOutgoingPduRef))"!]
    [!VAR "FoundIds" = "false()"!]
    [!LOOP "text:split($SrcPduRefToHandleIdList,';')"!]
        [!IF "string(text:split(.,':')[1]) = $IpduMOutgoingPduRefPath"!]
            [!VAR "PduID" = "text:split(.,':')[2]"!]
            [!VAR "FoundIds" = "true()"!]
            [!BREAK!]
        [!ENDIF!]
    [!ENDLOOP!]
    [!IF "$FoundIds = 'false'"!]
        [!ERROR!] The PduR does not reference the PDU '[!"node:path(node:ref($IpduMOutgoingPduRef))"!]' [!ENDERROR!]
    [!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to get the PduR's destination PDU-ID referenced by global PDU "IpduMPduRef". The PDU-ID is
* stored in the global variable "ResponsePduID". The variable "FoundReponseId" inherently indicate if
* the TxConfirmation is enabled or disabled by the PduR. The macro shall only be applied if the
* considered TxConfirmation of the IpduM is enabled.
****************************************************************************************************
*/!]
[!MACRO "GetResponsePduId","IpduMPduRef"!][!NOCODE!]
    [!VAR "IpduMPduRefPath" = "node:path(node:ref($IpduMPduRef))"!]
    [!VAR "FoundReponseId" = "false()"!]
    [!LOOP "text:split($DestPduRefToHandleIdList,';')"!]
        [!IF "string(text:split(.,':')[1]) = $IpduMPduRefPath"!]
            [!VAR "ResponsePduID" = "text:split(.,':')[2]"!]
            [!VAR "FoundReponseId" = "true()"!]
            [!BREAK!]
        [!ENDIF!]
    [!ENDLOOP!]
    [!IF "$FoundReponseId = 'false'"!]
      /* refs not shared, needed for Symbolic names */
      /* no direct call in order to preserve err msg */
      [!LOOP "text:split($SrcPduRefToHandleIdList,';')"!]
        [!IF "string(text:split(.,':')[1]) = $IpduMPduRefPath"!]
          [!VAR "ResponsePduID" = "text:split(.,':')[2]"!]
          [!VAR "FoundReponseId" = "true()"!]
          [!BREAK!]
        [!ENDIF!]
      [!ENDLOOP!]
        [!IF "$FoundReponseId = 'false'"!]
          [!ERROR!] Either PDU '[!"node:path(node:ref($IpduMPduRef))"!]' is not referenced by the PduR module or associated parameter 'PduRDestPduHandleId' is disabled.[!ENDERROR!]
        [!ENDIF!]
    [!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to get the string for the symbolic name value. Sets variable "SymbolName" holding either the
* short name of the referenced container or (if not available) the provided "Oldname".
* "ShortNameRef" must reference the container holding the short name. (AUTOSAR short name)
* "OldName" must hold a unique string for the case that the specification
* conform short name does not exist. This is supplied to be backward compatible.
****************************************************************************************************
*/!]
[!MACRO "GetSymbolName","ShortNameRef","OldName"!][!NOCODE!]
  [!VAR "SymbolName" = "asc:getShortName($ShortNameRef)"!]
    [!IF "$SymbolName = ''"!]
      [!VAR "SymbolName" = "$OldName"!]
    [!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to generate all symbolic name values for an ID
****************************************************************************************************
*/!]
[!MACRO "GenSymbols","SymbolicPrefix","SymbolicName","SymbolicIdName","SymbolicValue","PduName","VariantSuffix","Legacy"="true()"!][!/*
*/!]/*------------------------------------------------------------------------------------------------*/
/* [!"node:path($SymbolicValue)"!] */
[!VAR "SymbolName" = "concat($SymbolicPrefix, $SymbolicName, $VariantSuffix)"!][!//
#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] is already defined
#endif
/** \brief Export symbolic name value for [!"$SymbolicIdName"!] */
#define [!"$SymbolName"!] [!"node:value($SymbolicValue)"!]U /* [!"$PduName"!] */

[!IF "$Legacy != 'false'"!][!//
[!VAR "SymbolName" = "concat('IpduM_', $SymbolicName, $VariantSuffix)"!][!//
#if (defined IPDUM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined [!"$SymbolName"!])
#error [!"$SymbolName"!] is already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix only (AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define [!"$SymbolName"!] [!"node:value($SymbolicValue)"!]U
#endif /* defined IPDUM_PROVIDE_LEGACY_SYMBOLIC_NAMES */[!//
[!ENDIF!]
[!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to convert Timeouts values to MainFunctionTx cycles. Value is rounded down
* Instead of string matching workaround two parameters are used,
* nodePath is the path to the container, while
* valPathNode is the path to the value.
* NOTE: valPathNode isn't relative to nodePath because of the SendTimeouts
* NOTE2: nodePath is expected to be a SHORT-NAME-path
* NOTE3: if valPathNode is empty, directValue is mandatory (default value used otherwise)
****************************************************************************************************
*/!]
[!MACRO "TxTimeoutToMFC","nodePath","valPathNode","asPathChildrenWorkaround"="1", "directValue"="1.008"!][!NOCODE!]
[!IF "$IpduMMultiMF = 'true'"!]
  [!IF "not(var:defined('mfTxAllocTiming'))"!][!CALL "createTXMFAlloc"!][!ENDIF!]
  [!LOOP "text:split($mfTxAllocTiming, ';')"!]
    [!IF "contains(., $nodePath)"!]
      [!VAR "txTBase" = "text:split(.,':')[2]"!]
      [!BREAK!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ELSE!]
  [!VAR "txTBase" = "as:modconf('IpduM')[1]/IpduMGeneral/IpduMTxTimeBase"!]
[!ENDIF!]
[!IF "$valPathNode != ''"!]
  [!/* as:ref() workaround, it doesn't resolve IpduMTxRequest for ex */!]
  [!VAR "Children" = "''"!]
  [!FOR "k" = "0" TO "$asPathChildrenWorkaround - 1"!]
    [!IF "$k = 0"!]
      [!VAR "pathSep"="''"!]
    [!ELSE!]
      [!VAR "pathSep"="'/'"!]
    [!ENDIF!]
    [!VAR "Children" = "concat(text:split($valPathNode,'/')[last() - $k],$pathSep,$Children)"!]
  [!ENDFOR!]
  [!/* There would be no need for this if as:value would exist */!]
  [!VAR "Parents" = "substring-before($valPathNode, $Children)"!]
  [!/* When obtaining */!]
  [!VAR "shortNameToXPath" = "concat(node:path(as:ref($Parents)),'/',$Children)"!]
  [!VAR "mVal" = "node:value($shortNameToXPath) div $txTBase"!]
[!ELSE!]
  [!VAR "mVal" = "$directValue div $txTBase"!]
[!ENDIF!]
[!IF "text:match(string($mVal),'\d+\.9{15}')"!]
  [!VAR "mVal" = "num:i(ceiling($mVal))"!]
[!ENDIF!]
[!VAR "exponent" = "0"!]
[!IF "num:i($mVal) != $mVal"!]
  [!VAR "exponent" = "string-length(substring-after(string($mVal), '.'))"!]
[!ENDIF!]
[!FOR "i" = "1" TO "$exponent"!]
  [!VAR "mVal" = "$mVal * 10"!]
[!ENDFOR!]
[!VAR "mVal" = "num:i($mVal)"!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to get the TxSendTimeout, this being that of the Container or the Contained, depending
* which is lesser.
* The parameter is expected to be that of type "IpduMContainedTxPdu"
****************************************************************************************************
*/!]
[!MACRO "GetMinTxSendTimeout", "ValPath1"!][!NOCODE!]
[!VAR "TxSContainedTimeout" = "num:i(0)"!]
[!VAR "TxSTimeoutContainer" = "num:i(0)"!]
[!VAR "assocCntrPath" = "as:path(as:ref(as:ref($ValPath1)/IpduMContainedTxInContainerPduRef))"!]
[!IF "node:exists(as:ref($ValPath1)/IpduMContainedTxPduSendTimeout)"!]
  [!CALL "TxTimeoutToMFC", "nodePath"="$assocCntrPath", "valPathNode"="concat($ValPath1, '/IpduMContainedTxPduSendTimeout')"!]
  [!VAR "TxSContainedTimeout" = "$mVal"!]
[!ENDIF!]
[!IF "node:exists(as:ref($assocCntrPath)/IpduMContainerTxSendTimeout)"!]
  [!CALL "TxTimeoutToMFC", "nodePath"="$assocCntrPath", "valPathNode"="concat($assocCntrPath, '/IpduMContainerTxSendTimeout')"!]
  [!VAR "TxSTimeoutContainer" = "$mVal"!]
[!ENDIF!]
[!IF "$TxSContainedTimeout = 0"!]
  [!VAR "MinTxSendTimeout" = "$TxSTimeoutContainer"!]
[!ELSEIF "$TxSTimeoutContainer = 0"!]
  [!VAR "MinTxSendTimeout" = "$TxSContainedTimeout"!]
[!ELSE!]
  [!IF "$TxSTimeoutContainer < $TxSContainedTimeout"!]
    [!VAR "MinTxSendTimeout" = "$TxSTimeoutContainer"!]
  [!ELSE!]
    [!VAR "MinTxSendTimeout" = "$TxSContainedTimeout"!]
  [!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* Macro enumerates the RX MainFunctions, the generic one being at the beginning.
* Entries are separated by semicolon (;), attributes by pound sign(#).
****************************************************************************************************
*/!]
[!MACRO "createRXMFFuncNames"!][!NOCODE!]
[!VAR "mfRXalloc"!];[!"num:i(0)"!]# #IpduM_MainFunctionRx#[!ENDVAR!]
[!LOOP "as:modconf('IpduM')[1]/IpduMGeneral/IpduMRxProcessing/*"!]
  [!VAR "mfRXalloc"!][!"$mfRXalloc"!];[!"num:i(node:pos(.) + 1)"!]#[!"node:name(.)"!]#IpduM_MainFunctionRx_[!"node:name(.)"!][!ENDVAR!]
[!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]
[!/*
****************************************************************************************************
* Macro creates a string-map of pathway-MainFunctionTX allocation
* Entries of mfTXalloc are separated by semicolon (;), attributes  by pound sign(#) and attribute list elements by pipe (|).
* Entries of mfTxAllocTiming are separated by semicolon (;), attributes by colon(:), list elements by pipe (|).
****************************************************************************************************
*/!]
[!MACRO "createTXMFAlloc"!][!NOCODE!]
[!VAR "mfTxPathsMFGenPWs" = "text:join(text:difference(as:paths(as:modconf('IpduM')[1]/IpduMConfig/*[1]/IpduMTxPathway/*), as:paths(node:refs(as:modconf('IpduM')[1]/IpduMGeneral/IpduMTxProcessing/*/IpduMTxPduRef/*))),'|')"!]
[!VAR "mfTXalloc"!];[!"num:i(0)"!]# #IpduM_MainFunctionTx# [!"$mfTxPathsMFGenPWs"!][!ENDVAR!]
[!VAR "mfTxAllocTiming" = "concat($mfTxPathsMFGenPWs, '|', text:join(text:difference(as:paths(as:modconf('IpduM')[1]/IpduMConfig/*[1]/IpduMContainerTxPdu/*), as:paths(node:refs(as:modconf('IpduM')[1]/IpduMGeneral/IpduMTxProcessing/*/IpduMTxPduRef/*))),'|'), ':', as:modconf('IpduM')[1]/IpduMGeneral/IpduMTxTimeBase)"!]
[!/* Note: Space is important before the text:replaceAll() concatenation, otherwise if there are no references the selector goes to the next field */!]
[!LOOP "as:modconf('IpduM')[1]/IpduMGeneral/IpduMTxProcessing/*"!]
  [!VAR "mfTXalloc"!][!"$mfTXalloc"!];[!"num:i(node:pos(.) + 1)"!]#[!"node:name(.)"!]#IpduM_MainFunctionTx_[!"node:name(.)"!]# [!"text:replaceAll(text:join(node:filter(./IpduMTxPduRef/*, "text:split(as:path(as:dtos(as:ref(.))), '/')[last()] = 'IpduMTxPathway'"),'|'),'ASPath\:','')"!][!ENDVAR!]
  [!/* Can't filter directly in xgen for IpduM_MFTxLookupPathway on strlist with node:filter(), hence the query */!]
  [!VAR "mfTxAllocTiming"!][!"$mfTxAllocTiming"!];[!"text:replaceAll(text:join(./IpduMTxPduRef/*, '|'),'ASPath\:','')"!]:[!"./IpduMTimeBase"!][!ENDVAR!]
[!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]
[!/*
****************************************************************************************************
* Macro creates section / segment strings for memory mapping
* Results are in "secSTART", "secSTOP", "segCODE".
****************************************************************************************************
*/!]
[!MACRO "createSegmentStr","shortName"!][!NOCODE!]
[!IF "not(var:defined('mipStr'))"!]
  [!//maybe at some point join would work with varargs as well
  [!VAR "mipStr" = "'IPDUM'"!]
  [!VAR "startStr" = "'START'"!]
  [!VAR "stopStr" = "'STOP'"!]
  [!VAR "sectionStr" = "'SEC'"!]
  [!VAR "segmentStr" = "'CODE'"!]
  [!VAR "scope" = "'LOCAL'"!]
  [!VAR "delimiter" = "'_'"!]
[!ENDIF!]
[!/*
[!IF "$shortName = ' '"!][!//single empty space because of tokenization
*/!]
  [!VAR "secSTART" = "concat($mipStr, $delimiter, $startStr, $delimiter, $sectionStr, $delimiter, $segmentStr)"!]
  [!VAR "secSTOP" = "concat($mipStr, $delimiter, $stopStr, $delimiter, $sectionStr, $delimiter, $segmentStr)"!]
  [!VAR "segCODE" = "concat($mipStr, $delimiter, $segmentStr)"!]
[!/*
[!ELSE!]
  [!VAR "secSTART" = "concat($mipStr, $delimiter, text:toupper($shortName), $delimiter, $startStr, $delimiter, $sectionStr, $delimiter, $segmentStr, $delimiter, $scope)"!]
  [!VAR "secSTOP" = "concat($mipStr, $delimiter, text:toupper($shortName), $delimiter, $stopStr, $delimiter, $sectionStr, $delimiter, $segmentStr, $delimiter, $scope)"!]
  [!VAR "segCODE" = "concat($mipStr, $delimiter, text:toupper($shortName), $delimiter, $segmentStr)"!]
[!ENDIF!]
*/!]
[!ENDNOCODE!][!ENDMACRO!]
[!/*
****************************************************************************************************
* MACRO used to get internal RxIndication handles function index.
* Note: Needs to be in sync with IpduM_RxIndication.c
****************************************************************************************************
*/!]
[!MACRO "IpduMGetRxIndHandlerFuncIdx","cfgItemPath"!][!NOCODE!]
  [!IF "contains($cfgItemPath, 'IpduMRxPathway')"!]
    [!VAR "IpduMRxIndHandlerFuncIdx" = "num:i(0)"!]
  [!ELSE!]
    [!SELECT "node:ref($cfgItemPath)"!]
      [!IF "./IpduMContainerPduProcessing = 'IPDUM_PROCESSING_IMMEDIATE'"!]
        [!VAR "IpduMRxIndHandlerFuncIdx" = "num:i(1)"!]
      [!ELSE!]
        [!VAR "IpduMRxIndHandlerFuncIdx" = "num:i(2)"!]
      [!ENDIF!]
    [!ENDSELECT!]
  [!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* Establishes whether the declared types are enabled from the supplied type definitions.
* During type generation it's expected to be the first type related macro called.
* See IpduM_Types.m for examples.
* All type definitions are supplied in <type name>_DEF string variables.
* Optional type definitions start with a "?" and have to have a <type>_COND variable
* Structure definitions start with "{}"
* Pointer/reference fields start with "*"
* Arrays have their size specified within "[" and "]", preceding the type. Ex: a:[5]uint8
* For the types for which the <type>_COND variables are evaluated as true, their names are appended
* to the <$TypeStrName>_ENABLEDTYPES variable.

* Parameters:
*   TypeStrName : Name of the variable (NOTE: Not the content)
*   TypeStrSep: Optional; Type separator character
*   TypeSep: Optional, struct field names and types separator character
*   StructFieldSep: Optional, structure field separator character
****************************************************************************************************
*/!]
[!MACRO "IpduMRegisterTypes","TypeStrName","TypeStrSep"="';'", "TypeSep"="':'", "StructFieldSep"="'|'"!][!NOCODE!]
[!CALL "IpduMGetVar", "VarName" = "$TypeStrName"!]
[!VAR "TypeStr" = "$VarVal"!]
[!VAR "TypesEnabledStrName" = "concat($TypeStrName, '_ENABLEDTYPES')"!]
[!IF "not(var:defined($TypesEnabledStrName))"!]
  [!"var:set($TypesEnabledStrName, '')"!]
[!ENDIF!]
[!LOOP "text:split($TypeStr, $TypeStrSep)"!]
  [!VAR "Optional" = "false()"!]
  [!VAR "TypeName" = "string(.)"!]
  [!IF "starts-with(.,'?')"!]
    [!VAR "Optional" = "true()"!]
    [!VAR "TypeName" = "substring($TypeName, 2)"!]
  [!ENDIF!]
  [!IF "$Optional = 'true'"!]
    [!CALL "IpduMGetVar","VarName" = "concat($TypeName, '_COND')"!]
    [!CALL "EvalCond","Cond" = "$VarVal"!]
    [!VAR "TypeEnabled" = "$retEvalCond"!]
  [!ELSE!]
    [!VAR "TypeEnabled" = "true()"!]
  [!ENDIF!]
  [!IF "$TypeEnabled = 'true'"!]
    [!CALL "IpduMGetVar","VarName" = "$TypesEnabledStrName"!]
    [!"var:set($TypesEnabledStrName, concat($VarVal, $TypeStrSep, $TypeName))"!]
    [!CALL "IpduMGetVar","VarName" = "concat($TypeName, '_DEF')"!]
    [!VAR "TypeDef" = "$VarVal"!]
    [!IF "starts-with($TypeDef, '{}')"!]
      [!VAR "TypeDefStructFieldsEnabled" = "'{}'"!]
      [!LOOP "text:split(substring($TypeDef, 3), $StructFieldSep)"!]
        [!/* grep can't return matched groups */!]
        [!IF "starts-with(., '?')"!]
          [!VAR "FieldName" = "substring(substring-before(., $TypeSep), 2)"!]
          [!/* If it's a pointer/ref it starts with "*" */!]
          [!CALL "IpduMGetVar", "VarName"="concat($FieldName, '_COND')"!]
          [!CALL "EvalCond", "Cond"="$VarVal"!]
          [!IF "$retEvalCond = 'true'"!]
            [!VAR "FieldType" = "substring-after(., $TypeSep)"!]
            [!VAR "TypeDefStructFieldsEnabled" = "concat($TypeDefStructFieldsEnabled, $FieldName, $TypeSep, $FieldType, $StructFieldSep)"!]
          [!ENDIF!]
        [!ELSE!]
          [!/* Consider it enabled */!]
          [!VAR "TypeDefStructFieldsEnabled" = "concat($TypeDefStructFieldsEnabled, ., $StructFieldSep)"!]
        [!ENDIF!]
      [!ENDLOOP!]
      [!/* Overwrite original definition with just the enabled ones */!]
      [!"var:set(concat($TypeName, '_DEF'), $TypeDefStructFieldsEnabled)"!]
    [!ENDIF!]
  [!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* Iterates over the string created by the MACRO IpduMRegisterTypes.
* The optional parameters are the same with the exception of isVar.
* Parameters:
*   TypeStr: Value of the string with the enabled types
*   isVar: Optional, if true, the size and alignment is calculated for the variables and
*     stored in the <type name>_SIZE and <type name>_ALG variables respectively.
* <type name>_EN variables are defined suitable for the XGen (to check if a field is enabled).
****************************************************************************************************
*/!]
[!MACRO "IpduMEstablishTypes", "TypeStr", "isVar" = "false()", "TypeStrSeparator"="';'", "TypeSep"="':'", "StructFieldSep"="'|'"!][!NOCODE!]
[!FOR "i" = "1" TO "count(text:split($TypeStr, $TypeStrSeparator))"!]
  [!VAR "Name" = "text:split($TypeStr, $TypeStrSeparator)[position() = $i]"!]
  [!IF "not(var:defined(concat($Name, '_SIZE')))"!]
    [!CALL "IpduMGetVar", "VarName" = "concat($Name, '_DEF')"!]
    [!IF "$isVar != 'false'"!]
      [!CALL "IpduMGetTypeSizeAlg", "Type" = "$VarVal"!]
      [!"var:set(concat($Name, '_SIZE'), $IpduMTypeSize)"!]
      [!"var:set(concat($Name, '_ALG'), $IpduMTypeAlignment)"!]
    [!ENDIF!]
    [!"var:set(concat($Name, '_EN'), true())"!]
    [!IF "starts-with($VarVal, '{}')"!]
      [!LOOP "text:split(substring-after($VarVal, '{}'), $StructFieldSep)"!]
        [!/* Again, no group match */!]
        [!VAR "FieldName" = "substring-before(., $TypeSep)"!]
        [!"var:set(concat($Name, '_', $FieldName, '_EN'), true())"!]
      [!ENDLOOP!]
    [!ENDIF!]
  [!ENDIF!]
[!ENDFOR!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* Iterates over a type definition string and generates xml fragments, suitable for XGen.
* The optional parameters are the same as for the MACRO IpduMRegisterTypes, with the exception of DeclInd
* Note: Output is not stored but printed to the location of the call.
* Parameters:
*   TypeStr: Value of the type string
*   DeclInd: Optional, indentation level, default is 4
****************************************************************************************************
*/!]
[!MACRO "IpduMDeclareTypes", "TypeStr", "DeclInd" = "4", "TypeStrSeparator"="';'", "TypeSep"="':'", "StructFieldSep"="'|'"!][!NOCODE!]
[!LOOP "text:split($TypeStr, $TypeStrSeparator)"!]
  [!VAR "varName" = "."!]
  [!CALL "IpduMGetVar", "VarName" = "concat($varName, '_DEF')"!]
  [!VAR "def" = "$VarVal"!]
  [!INDENT "$DeclInd"!]
  [!IF "not(starts-with($def, '{}'))"!]
    [!CODE!]
    <type name="[!"$varName"!]">
    [!WS "2"!]<reference type="[!"$def"!]">
    [!WS "4"!]<compiler-abstraction>
    [!WS "6"!]<var memory-class="TYPEDEF"/>
    [!WS "4"!]</compiler-abstraction>
    [!WS "2"!]</reference>
    </type>
    [!ENDCODE!]
  [!ELSE!]
  [!/* Besides basic types everything else is encapsulated in a struct */!]
    [!CODE!]
    <type name="[!"$varName"!]">
    [!WS "2"!]<struct>
    [!ENDCODE!][!//
    [!LOOP "text:split(substring-after($def, '{}'), $StructFieldSep)"!]
      [!VAR "memberName" = "substring-before(., $TypeSep)"!]
      [!VAR "memberTypeStr" = "substring-after(., $TypeSep)"!]
      [!/* Substring returns an empty string if there's no match. Also, no group match */!]
      [!VAR "memberType" = "node:when(starts-with($memberTypeStr, '*'), substring($memberTypeStr, 2), $memberTypeStr)"!]
      [!/* not to nest node:whens */!]
      [!VAR "memberType" = "node:when(starts-with($memberTypeStr, '['), substring-after($memberTypeStr, ']'), $memberType)"!]
      [!VAR "memberCount" = "node:when(starts-with($memberTypeStr, '['), node:fallback("->num:i(substring-after(substring-before($memberTypeStr, ']') , '['))", string('')), string(''))"!]
      [!CODE!][!//
      [!WS "4"!]<member name="[!"$memberName"!]" type="[!"$memberType"!]"[!"node:when($memberCount != '', concat(' count="', $memberCount, '"') , string(''))"!]>
      [!WS "6"!]<compiler-abstraction>
      [!WS "8"!][!"node:when(starts-with($memberTypeStr, '*'), '<ref2cfg/>', concat('<', node:when($memberName != 'IpduM_RootConfig', 'var', 'const'), ' memory-class="TYPEDEF"/>'))"!]
      [!WS "6"!]</compiler-abstraction>
      [!WS "4"!]</member>
      [!ENDCODE!][!//
    [!ENDLOOP!]
    [!CODE!][!//
    [!WS "2"!]</struct>
    </type>
    [!ENDCODE!]
  [!ENDIF!]
  [!ENDINDENT!]
[!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* Stores and returns container length in bytes
* Used in order to eliminate IPDUM_HEADERTYPE_NONE underallocation
* Note: Prints an INFO message if a container has a greater length configured than the one used
* Parameters:
*   cntrPath: SHORT-NAME path of the container
* Result is in "IpduMCntrLen"
****************************************************************************************************
*/!]
[!MACRO "IpduMGetCntrLength", "cntrPath"!][!NOCODE!]
[!VAR "dir" = "node:when('IpduMContainerTxPdu' = text:split(as:path(as:dtos(string($cntrPath))),'/')[last()], 'Tx', 'Rx')"!]
[!VAR "cntrLookupVarName" = "concat('IpduM', $dir, 'CntrLengths')"!]
[!IF "not(var:defined($cntrLookupVarName))"!]
[!"var:set($cntrLookupVarName, '')"!]
[!LOOP "as:ref($cntrPath)/../*"!]
  [!VAR "confCntrLen" = "node:ref(node:ref(concat('./IpduMContainer', $dir, 'PduRef')))/PduLength"!]
  [!IF "'IPDUM_HEADERTYPE_NONE' != ./IpduMContainerHeaderSize"!]
    [!VAR "IpduMCntrLen" = "$confCntrLen"!]
  [!ELSE!]
    [!VAR "maxOfs" = "num:max(node:ref(concat('../../IpduMContained', $dir, 'Pdu'))/*[node:refvalid(concat('IpduMContained', $dir, 'InContainerPduRef')) and (node:path(node:ref(node:ref(concat('IpduMContained', $dir, 'InContainerPduRef')))) = node:path(node:current()))]/IpduMContainedPduOffset)"!]
    [!/* IpduM_checks enforces that there is a single one */!]
    [!VAR "lenMaxOfs" = "node:ref(node:ref(concat(node:path(node:ref(concat('../../IpduMContained', $dir, 'Pdu'))/*[node:refvalid(concat('IpduMContained', $dir, 'InContainerPduRef')) and (node:path(node:ref(node:ref(concat('IpduMContained', $dir, 'InContainerPduRef')))) = node:path(node:current())) and (IpduMContainedPduOffset = $maxOfs)]), '/IpduMContained', $dir, 'PduRef')))/PduLength"!]
    [!VAR "IpduMCntrLen" = "num:i($maxOfs + $lenMaxOfs)"!]
    [!IF "$confCntrLen > $IpduMCntrLen"!]
      [!INFO!]The container [!"node:name(.)"!] has a configured length of [!"$confCntrLen"!] bytes and only [!"$IpduMCntrLen"!] allocated. Using the value of [!"$IpduMCntrLen"!] as length.[!ENDINFO!]
    [!ENDIF!]
  [!ENDIF!]
  [!CALL "IpduMGetVar", "VarName" = "$cntrLookupVarName"!]
  [!"var:set($cntrLookupVarName, concat($VarVal, node:name(.), ':', $IpduMCntrLen, ';'))"!]
[!ENDLOOP!]
[!ENDIF!]
[!CALL "IpduMGetVar", "VarName" = "$cntrLookupVarName"!]
[!VAR "IpduMCntrLen" = "num:i(text:split(text:grep(text:split($VarVal, ';'), concat('^', text:split($cntrPath, '/')[last()], '\:\d+$'))[1], ':')[2])"!]
[!ENDNOCODE!][!ENDMACRO!]

[!ENDIF!][!// multiple inclusion protection
[!ENDNOCODE!][!//
