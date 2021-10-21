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
[!IF "not(var:defined('IPDUM_HELPERMACROS_M'))"!]
[!VAR "IPDUM_HELPERMACROS_M"="'true'"!]
[/* Contains type definitions */]

[!/*
****************************************************************************************************
* Helper macro for the MACRO EvalCond
****************************************************************************************************
*/!]
[!MACRO "EvalCondNoNesting","ParamCond"!]
[!VAR "evalCondAnd" = "num:i(0)"!]
[!VAR "evalCondRet" = "num:i(0)"!]
[!LOOP "text:split($ParamCond, '|')"!]
  [!LOOP "text:split(., ';')"!]
    [!VAR "cond1" = "text:tolower(text:split(.,':')[1])"!]
    [!VAR "cond2" = "text:split(.,':')[2]"!]
    [!IF "var:defined($cond2)"!]
      [!VAR "evalCondAnd" = "node:foreach($cond1, 'bool_value', concat('$bool_value = $', $cond2))[1]"!]
    [!ELSE!]
      [!VAR "evalCondAnd" = "$cond1 = $cond2"!]
    [!ENDIF!]
    [!IF "'true' != $evalCondAnd"!]
      [!BREAK!]
    [!ENDIF!]
  [!ENDLOOP!]
  [!VAR "evalCondRet" = "node:when($evalCondRet = 'true', num:i(1), num:i(0)) or node:when($evalCondAnd = 'true', num:i(1), num:i(0))"!]
[!ENDLOOP!]
[!ENDMACRO!]


[!/*
****************************************************************************************************
* Evaluates conditions in <type>_COND variables.
*
* Conditions start with either TRUE/FALSE, followed by a colon and may have the form:
*   Simple: TRUE:<variable name>, ex: TRUE:$asdEnabled
*   And: TRUE:<variable name>;FALSE:<other variable name>
*   Or:  TRUE:<variable name>|FALSE:<other variable name>
*   Guarding condition: TRUE:<variable name>+TRUE:<variable name>;TRUE:<other variable name> (and/or supported)
*  Note: <variable name> is expected to store 'true' or 'false'. Used as such in order not to further hinder
*     readability
*   Pre-evaluated: TRUE:["$asd < 15"] (which might be TRUE:true)
* Parameters:
*   Cond : Condition as text (string)
****************************************************************************************************
*/!]
[!MACRO "EvalCond","Cond"!]
[!IF "text:contains(string($Cond), '+')"!]
  [!CALL "EvalCondNoNesting", "ParamCond" = "text:split($Cond,'+')[1]"!]
  [!VAR "retEvalCond" = "$evalCondRet"!]
  [!IF "'false' != $retEvalCond"!]
    [!CALL "EvalCondNoNesting", "ParamCond" = "text:split($Cond,'+')[2]"!]
    [!VAR "retEvalCond" = "node:when($retEvalCond = 'true', num:i(1), num:i(0)) and node:when($evalCondRet = 'true', num:i(1), num:i(0))"!]
  [!ENDIF!]
[!ELSE!]
  [!CALL "EvalCondNoNesting", "ParamCond"="$Cond"!]
  [!VAR "retEvalCond" = "$evalCondRet"!]
[!ENDIF!]
[!ENDMACRO!]

[!/*
****************************************************************************************************
* Retrieves the value of a variable and stores in the the variable named "VarVal" (cast to num int),
*   if possible
* Parameters:
*   VarName: Name of the variable
****************************************************************************************************
*/!]
[!MACRO "IpduMGetVar","VarName"!]
[!VAR "VarVal" = "node:foreach($VarName, 'dummy', concat('$',string($VarName)))[1]"!]
[!/* Most params are int, try to recast if possible */!]
[!VAR "VarVal" = "node:fallback("->num:i($VarVal)", string($VarVal))"!]
[!ENDMACRO!]

[!/*
****************************************************************************************************
* Helper variable for IpduMGetTypeSizeAlg since asc:getDataTypeSize() fails ungracefully if the
* supplied string does not exist within the properties file
****************************************************************************************************
*/!]
[!VAR "IpduMPrimitiveTypes" = "'boolean#uint8#uint16#uint32#sint32#sint8#sint16'"!]

[!/*
   * Results are in the variables: IpduMTypeSize, IpduMTypeAlignment
*/!]

[!/*
****************************************************************************************************
* Calculates the struct alignment based on it's size.
* Parses ComplexType_Alignment_struct from Base which should have the ecu prop
*   Complextypes.struct.Alignment as value (number or string like 1->1,2->2,3->2,4->2,5->2,6->2,7->2,4)
* Parameters:
*   structSize: Size of struct in bytes
* Result is in the variable: structAlignment
****************************************************************************************************
*/!]
[!MACRO "GetStructAlg","structSize"!][!NOCODE!]
[!IF "not(var:defined('structAlignConf'))"!]
[!/* This is due to Generic tests. No Base , no ecu props, can't wrap them with node:fallback to fail gracefully */!]
[!VAR "structAlignConf" = "''"!]
[!IF "node:exists(as:modconf('Base')[1]/BaseTypes/ComplexType_Alignment_struct)"!]
  [!VAR "structAlignConf" = "as:modconf('Base')[1]/BaseTypes/ComplexType_Alignment_struct"!]
[!ENDIF!]
  [!IF "$structAlignConf = ''"!]
    [!/* Set because of Generic tests. No asc:getStructAlignment() either. (empty string) */!]
    [!VAR "structAlignConf" = "num:i(4)"!]
  [!ENDIF!]
  [!IF "num:isnumber($structAlignConf)"!]
    [!VAR "structAlignment" = "num:i($structAlignConf)"!]
    [!VAR "structAlignFixed" = "true()"!]
  [!ELSE!]
    [!VAR "structAlignFixed" = "false()"!]
  [!ENDIF!]
[!ENDIF!]
[!IF "structAlignFixed != 'true'"!]
  [!IF "not(var:defined('byteAlgLimits'))"!]
    [!VAR "prevLimit" = "num:i(0)"!]
    [!VAR "prevSize" = "num:i(1)"!]
    [!VAR "byteAlgLimits" = "''"!]
    [!LOOP "text:split($structAlignConf,',')"!]
      [!IF "count(text:split(., '->')) = 2"!]
        [!VAR "limit" = "text:split(., '->')[1]"!]
        [!VAR "size" = "text:split(., '->')[2]"!]
      [!ELSE!]
        [!VAR "limit" = "num:i(0)"!]
        [!VAR "size" = "text:split(., '->')[1]"!]
      [!ENDIF!]
      [!IF "($limit = 0) or ($size > $prevSize)"!]
        [!VAR "byteAlgLimits" = "concat($byteAlgLimits, $prevLimit, ':', $prevSize, ';')"!]
        [!VAR "prevSize" = "$size"!]
      [!ENDIF!]
      [!IF "$limit > $prevLimit"!]
        [!VAR "prevLimit" = "$limit"!]
      [!ENDIF!]
    [!ENDLOOP!]
    [!/* Prepend last one, may have limit, may be missing */!]
    [!VAR "byteAlgLimits" = "concat($byteAlgLimits, $prevLimit + 1, ':', $size)"!][!/* has semicolon from loop */!]
  [!ENDIF!]
  [!VAR "numAlgBuckets" = "num:i(count(text:split($byteAlgLimits, ';')))"!]
  [!FOR "bcktCnt" = "1" TO "$numAlgBuckets"!]
    [!VAR "bckt" = "text:split($byteAlgLimits, ';')[number($bcktCnt)]"!]
    [!IF "($bcktCnt = $numAlgBuckets) or (num:i($structSize) <= num:i(text:split($bckt, ':')[1]))"!]
      [!VAR "structAlignment" = "num:i(text:split($bckt, ':')[2])"!]
      [!BREAK!]
    [!ENDIF!]
  [!ENDFOR!]
[!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* Calculates the size and alignment for types
* Parameters:
*   Type: Type name
*   TypeSep: Optional, separator for struct field name and type name
*   FieldSep: Optional, separator for struct fields
* The calculated size and alignments are stored in the variables IpduMTypeSize and IpduMTypeAlignment
****************************************************************************************************
*/!]
[!MACRO "IpduMGetTypeSizeAlg", "Type", "TypeSep"="':'", "FieldSep" = "'|'"!]
[!/* This has to be the first, as variable names can't start with "*" */!]
[!IF "text:match($Type, '^\*\w+')"!]
[!/* We're not going to track near/far pointers in this fine template generator */!]
  [!VAR "IpduMTypeSize" = "num:i(asc:getDataTypeSize('nearpointer'))"!]
  [!VAR "IpduMTypeAlignment" = "num:i(asc:getDataTypeAlignment('nearpointer'))"!]
[!ELSEIF "var:defined(concat($Type, '_SIZE'))"!]
  [!CALL "IpduMGetVar", "VarName" = "concat($Type, '_SIZE')"!]
  [!VAR "IpduMTypeSize" = "$VarVal"!]
  [!CALL "IpduMGetVar", "VarName" = "concat($Type, '_ALG')"!]
  [!VAR "IpduMTypeAlignment" = "$VarVal"!]
[!/* Branching is necessary because node:fallback can't handle when getDataTypeSize fails */!]
[!ELSEIF "text:match($Type, '^(?:Atomic_t|TS_MaxAlignedType)$')"!]
[!/* Note: Atomic_t is mapped to greatest native int with atomic access, not double like TS_MaxAlignedType */!]
  [!VAR "IpduMTypeSize" = "num:i(node:when(var:defined('IpduMAtomicValueSizeOverride'), $IpduMAtomicValueSizeOverride, asc:getDataTypeSize(concat('uint', ecu:get('Cpu.Type')))))"!]
  [!/* Since the available doc is unclear, always going with TS_MaxAlignedType */!]
  [!VAR "IpduMTypeAlignment" = "num:i(asc:getDataTypeAlignment('TS_MaxAlignedType'))"!]
[!ELSEIF "text:contains(text:split($IpduMPrimitiveTypes, '#'), text:tolower($Type))"!]
  [!/* getDataTypeSize and getDataTypeAlignment return empty string in the case of Generic tests. Workaround. TBD: Why ? */!]
  [!VAR "bts" = "asc:getDataTypeSize(text:tolower($Type))"!]
  [!VAR "IpduMTypeSize" = "num:i(node:when($bts != '', $bts, 4))"!]
  [!VAR "IpduMTypeAlignment" = "num:i(node:when($bts != '', asc:getDataTypeAlignment(text:tolower($Type)), 4))"!]
[!ELSEIF "text:match($Type, '^\[\d+\]\w+$')"!]
  [!/* In theory this should be asc:getArrayAlignment() but IMHO that should accept a type string and return the alignment of that */!]
  [!VAR "a_quantity" = "substring-before(substring-after($Type, '['), ']')"!]
  [!CALL "IpduMGetTypeSizeAlg", "Type" = "substring-after($Type, ']')"!]
  [!/* No assignment to IpduMTypeAlignment, it's set by the prev call */!]
  [!VAR "IpduMTypeSize" = "num:i($IpduMTypeSize * $a_quantity)"!]
[!ELSE!]
  [!IF "starts-with($Type, '{}')"!]
    [!VAR "TDef" = "substring($Type, 3)"!]
    [!VAR "sizeCumulator" = "num:i(0)"!]
    [!VAR "Align" = "num:i(2)"!]
    [!FOR "t" = "1" TO "count(text:split($TDef, $FieldSep))"!]
      [!/* This will always be max 1 level deep */!]
      [!CALL "IpduMGetTypeSizeAlg", "Type" = "substring-after(text:split($TDef, $FieldSep)[position() = $t], $TypeSep)"!]
      [!VAR "Align" = "node:when($Align < $IpduMTypeAlignment, $IpduMTypeAlignment, $Align)"!]
      [!VAR "Padding" = "$IpduMTypeSize mod $IpduMTypeAlignment"!]
      [!VAR "sizeCumulator" = "num:i($sizeCumulator + ($IpduMTypeSize + $Padding) )"!]
      [!IF "$bts != ''"!]
        [!/* Only when sizes could truly be established */!]
        [!ASSERT "$IpduMTypeAlignment <= $Align"!]First member of struct [!"$Type"!] is not the one with greatest alignment[!ENDASSERT!]
      [!ENDIF!]
    [!ENDFOR!]
    [!CALL "GetStructAlg", "structSize" = "$sizeCumulator"!]
    [!VAR "Align" = "node:when($structAlignment >= $Align, $structAlignment, $Align)"!]
    [!VAR "sizeAlgRem" = "$sizeCumulator mod $Align"!]
    [!IF "$sizeAlgRem != 0"!]
      [!VAR "IpduMTypeSize" = "num:i($sizeCumulator + ($Align - $sizeAlgRem))"!]
    [!ELSE!]
      [!VAR "IpduMTypeSize" = "num:i($sizeCumulator)"!]
    [!ENDIF!]
    [!VAR "IpduMTypeAlignment" = "num:i($Align)"!]
  [!ELSE!]
    [!/* Only 1 level of redirection is supported, it's presumed that basic types are established first */!]
    [!CALL "IpduMGetTypeSizeAlg", "Type" = "substring-after($Type, $TypeSep)"!]
    [!/* No assignment, the var with the same name is set in the recursive call */!]
  [!ENDIF!]
[!ENDIF!]
[!ENDMACRO!]

[!ENDIF!][!// multiple inclusion protection
[!ENDNOCODE!][!//
