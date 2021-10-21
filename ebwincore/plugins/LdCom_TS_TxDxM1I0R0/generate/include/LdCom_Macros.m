[!/**
 * \file
 *
 * \brief AUTOSAR LdCom
 *
 * This file contains the implementation of the AUTOSAR
 * module LdCom.
 *
 * \version 1.0.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!NOCODE!]
[!IF "not(var:defined('LDCOM_MACROS_M'))"!]
[!VAR "LDCOM_MACROS_M"="'true'"!]
[!/*
****************************************************************************************************
* Create three lists of PDU names and the corresponding handle IDs.
* - SrcPduRefToHandleIdList will hold all source PDU IDs of all PduR routing paths.
* - DestPduRefToHandleIdList will hold all destination PDU IDs of all PduR routing paths.
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
[!// Iterate over all routing paths to collect the source and destination PDU-IDs
[!LOOP "as:modconf('PduR')[1]/PduRRoutingTables/*[1]/PduRRoutingTable/*[1]/PduRRoutingPath/*"!]
    [!// Get the source PDU-ID of a routing path
    [!VAR "SrcHandleId" = "./PduRSrcPdu/PduRSourcePduHandleId"!]
    [!IF "num:isnumber($SrcHandleId) and node:refexists(./PduRSrcPdu/PduRSrcPduRef)"!]
        [!VAR "SrcPduRef" = "node:path(node:ref(./PduRSrcPdu/PduRSrcPduRef))"!]
        [!VAR "SrcPduRefToHandleIdList"!][!"$SrcPduRefToHandleIdList"!];[!"$SrcPduRef"!]:[!"$SrcHandleId"!][!ENDVAR!]
    [!ENDIF!]
[!ENDLOOP!]


[!/*
****************************************************************************************************
* MACRO to get the PduR source PDU ID the PduR has to be called with. The macro parameter
* "LdComPduRef" must reference this PDU. The PDU-ID is stored in the global variable "PduID".
****************************************************************************************************
*/!]
[!MACRO "GetOutputPduId","LdComPduRef"!][!NOCODE!]
    [!VAR "LdComPduRefPath" = "node:path(node:ref($LdComPduRef))"!]
    [!VAR "FoundIds" = "false()"!]
    [!LOOP "text:split($SrcPduRefToHandleIdList,';')"!]
        [!IF "string(text:split(.,':')[1]) = $LdComPduRefPath"!]
            [!VAR "PduID" = "text:split(.,':')[2]"!]
            [!VAR "FoundIds" = "true()"!]
        [!ENDIF!]
    [!ENDLOOP!]
    [!IF "$FoundIds = 'false'"!]
        [!ERROR!] The PduR does not reference the PDU '[!"node:path(node:ref($LdComPduRef))"!]' [!ENDERROR!]
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
[!MACRO "GetSymbolName","ShortNameRef"!][!NOCODE!]
  [!VAR "SymbolName" = "asc:getShortName($ShortNameRef)"!]
  [!IF "$SymbolName = ''"!]
    [!ERROR!]The LdCom module does not reference the PDU '[!"$ShortNameRef"!]'.[!ENDERROR!]
  [!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to generate all symbolic name values for an ID
****************************************************************************************************
*/!]
[!MACRO "GenSymbols","SymbolicPrefix","SymbolicName","SymbolicIdName","SymbolicValue","PduName"!][!/*
*/!]/*------------------------------------------------------------------------------------------------*/
/* [!"node:path($SymbolicValue)"!] */
#if (defined [!"$SymbolicPrefix"!][!"$SymbolicName"!])
#error [!"$SymbolicPrefix"!][!"$SymbolicName"!] is already defined
#endif
/** \brief Export symbolic name value for [!"$SymbolicIdName"!] */
#define [!"$SymbolicPrefix"!][!"$SymbolicName"!] [!"node:value($SymbolicValue)"!]U /* [!"$PduName"!] */[!//
[!ENDMACRO!]

[!ENDIF!]
[!ENDNOCODE!][!//