#ifndef AUTOSAR_CFG_H
#define AUTOSAR_CFG_H
[!AUTOSPACING!][!//

/**
 * \file
 *
 * \brief AUTOSAR Base
 *
 * This file contains the implementation of the AUTOSAR
 * module Base.
 *
 * \version 5.0.31
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[macros]=================================================*/

/*------------------[Macros for Atomic Assignment]---------------------------*/

[!INCLUDE "TypeLists.m"!]
[!/* MACRO GetCType:
     Get C type for a certain AUTOSAR type

     Parameter:
      - Prefix: prefix of the parameter to check; allowed: 'Base', 'Complex', 'Derived'
      - Type: the AUTOSAR type
      - Label: label for type to be printed in error message

     OUT Variables:
      - CType:  the C type corresponding to the AUTOSAR type
*/!][!//
[!MACRO "GetCType","Prefix","Type","Label"!][!//
  [!VAR "property"="concat($Prefix,'Type_',$Type)"!]
  [!IF "node:exists(as:modconf('Base')[1]/BaseTypes/*[node:name(.)=$property])"!]
    [!VAR "BaseType"="as:modconf('Base')[1]/BaseTypes/*[node:name(.)=$property]"!]
    [!VAR "CType"="normalize-space(text:replace($BaseType,'(un)?signed',''))"!]
  [!ELSE!]
    [!ERROR!]
      Base: No mapping to C type found for [!"$Label"!] type [!"$Type"!]
    [!ENDERROR!]
  [!ENDIF!]
[!ENDMACRO!]
[!VAR "BitWidthListLength"="count(text:split($BitWidthList,','))"!]
[!FOR "i"="1" TO "num:i($BitWidthListLength)"!]
  [!VAR "BitWidth"="text:split($BitWidthList,',')[num:i($i)]"!]
  [!VAR "Type"="concat('uint',$BitWidth)"!]
  [!CALL "GetCType","Prefix"="'Base'","Type"="$Type","Label"="'base'"!]
  [!VAR "property"="concat('BaseType_AtomicAccess_',text:replace($CType,' ',''))"!]
  [!IF "node:exists(as:modconf('Base')[1]/BaseTypes/*[node:name(.)=$property]) and (as:modconf('Base')[1]/BaseTypes/*[node:name(.)=$property] = 'true')"!]
#if (defined TS_AtomicAssign[!"$BitWidth"!])
#error TS_AtomicAssign[!"$BitWidth"!] already defined
#endif
/** \brief Assigns the [!"$BitWidth"!] bit entity \p from to \p to in an atomic fashion
 *
 * This macro assigns the [!"$BitWidth"!] bit entity \p from to \p to in an atomic fashion
 *
 * The parameters \p from and \p to thus have to be of type \p uint8 or \p sint8.
 *
 * Platforms Setting:
 * It is derived from the parameters in the Platforms plugin by
 * '[Basetypes|Derivedtypes].[!"$CType"!].AtomicAccess'
 *
 * \param[out] to    [!"$BitWidth"!] bit entity which is the destination of the assignment
 * \param[in]  from  [!"$BitWidth"!] bit entity which is the source of the assignment
 *
 * \remarks The parameters of this macro may be used in any way, especially
 *   they can be used more than once. They shall not contain side effects.
 */
#define TS_AtomicAssign[!"$BitWidth"!](to, from) ((to) = (from))

  [!ENDIF!]
[!ENDFOR!]
/*------------------[Macros for Litte / Big Endian]--------------------------*/

[!/* import GuardedDefine macro
*/!][!INCLUDE "CommonMacros.m"!][!/*

*/!][!CALL "GuardedDefine", "Comment"="'Selector for Big Endian / Little Endian expression variants'",
 "Name"="'TS_IF_BE_LE'", "Params"="'(BE,LE)'"!][!//
[!IF "as:modconf('Base')[1]/BaseCpuConfig/BaseCpuConfig_byteorder = 'LE'"!]LE[!ELSE!]BE[!ENDIF!]


/*------------------[Size of Autosar Standard Types]-------------------------*/
[!/* MACRO CreateSizeMacros:
     Create marcos for accessing the size for all types passed.

     Parameter:
      - List: list of types to create the macros
      - IsDerivedType: flag (true|false) if derived type or base type is processed
*/!]
[!MACRO "CreateSizeMacros","List","Prefix","Label"!]
  [!VAR "ListLength"="count(text:split($List,','))"!]
  [!FOR "i"="1" TO "num:i($ListLength)"!]
    [!VAR "Type"="text:split($List,',')[num:i($i)]"!]
    [!CALL "GetCType","Prefix"="$Prefix","Type"="$Type","Label"="$Label"!]
    [!VAR "property"="concat('BaseType_Size_',text:replace($CType,' ',''))"!]
    [!IF "node:exists(as:modconf('Base')[1]/BaseTypes/*[node:name(.)=$property])"!]
#if (defined TS_SIZE_[!"$Type"!])
#error TS_SIZE_[!"$Type"!] already defined
#endif
/** \brief Size of derived type [!"$Type"!]
 *
 * Platforms Setting:
 * It is derived from the parameters in the Platforms plugin by
 * '[Basetypes|Derivedtypes].[!"$CType"!].Size'
 */
#define TS_SIZE_[!"$Type"!] [!WS!][!"as:modconf('Base')[1]/BaseTypes/*[node:name(.)=$property]"!]U

    [!ENDIF!]
  [!ENDFOR!]
[!ENDMACRO!]
[!CALL "CreateSizeMacros","List"="$AutosarTypeList","Prefix"="'Base'","Label"="'base'"!]
[!CALL "CreateSizeMacros","List"="$DerivedTypeList","Prefix"="'Derived'","Label"="'derived'"!]
/*------------------[Alignment Autosar Standard Types]-----------------------*/
[!/* MACRO CreateAlignmentMacros:
     Create marcos for accessing the alignment for all types passed.

     Parameter:
      - List: list of types to create the macros
      - IsDerivedType: flag (true|false) if derived type or base type is processed
*/!][!//
[!MACRO "CreateAlignmentMacros","List","Prefix","Label"!][!//
  [!VAR "ListLength"="count(text:split($List,','))"!]
  [!FOR "i"="1" TO  "num:i($ListLength)"!]
    [!VAR "Type"="text:split($List,',')[num:i($i)]"!]
    [!CALL "GetCType","Prefix"="$Prefix","Type"="$Type","Label"="$Label"!]
    [!VAR "property"="concat('BaseType_Alignment_',text:replace($CType,' ',''))"!]
    [!IF "node:exists(as:modconf('Base')[1]/BaseTypes/*[node:name(.)=$property])"!]
#if (defined TS_ALIGNMENT_[!"$Type"!])
#error TS_ALIGNMENT_[!"$Type"!] already defined
#endif
/** \brief Alignment constraints for derived type [!"$Type"!]
 *
 * Platforms Setting:
 * It is derived from the parameters in the Platforms plugin by
 * '[Basetypes|Derivedtypes].[!"$CType"!].Alignment'
 */
#define TS_ALIGNMENT_[!"$Type"!] [!WS!][!"as:modconf('Base')[1]/BaseTypes/*[node:name(.)=$property]"!]U

    [!ENDIF!]
  [!ENDFOR!]
[!ENDMACRO!]
[!/* MACRO CreateComplexAlignmentMacros:
      Create marcos for accessing the alignment of complex types.

      Parameter:
       - Type: type to create alignment macros for (either 'array' or 'struct')
       - ListThresholds: list of thresholds of alignments of complex type
       - ListAlignments: list of alignments of complex type
       - DefaultAlignment: default alignment of complex type
 */!]
[!MACRO "CreateComplexAlignmentMacros","Type","ListThresholds","ListAlignments","DefaultAlignment"!]
  [!VAR "Thresholds"="text:replaceAll(substring($ListThresholds,2,string-length($ListThresholds)-2),',','')"!]
  [!VAR "Alignments"="text:replaceAll(substring($ListAlignments,2,string-length($ListAlignments)-2),',','')"!]
  [!VAR "Length"="num:i(count(text:split($Alignments)))"!]
  [!IF "$Length > 1"!]
#if (defined TS_ALIGNMENT_[!"$Type"!]_NUM_THRESHOLDS)
#error TS_ALIGNMENT_[!"$Type"!]_NUM_THRESHOLDS already defined
#endif
/** \brief Number of alignment constraints for [!"$Type"!]s
 *
 * Platforms Setting:
 * It is set in the Platforms plugin by
 * setting the parameter 'Complextypes.[!"$Type"!].Alignment'.
 */
#define TS_ALIGNMENT_[!"$Type"!]_NUM_THRESHOLDS [!WS!][!"$Length"!]U
    [!FOR "i"="1" TO "$Length"!]
      [!VAR "Threshold"="text:split($Thresholds)[num:i($i)]"!]
      [!VAR "Alignment"="text:split($Alignments)[num:i($i)]"!]
      [!VAR "Index"="num:i($i - 1)"!]
      [!IF "string-length($Threshold) > 0"!]
#if (defined TS_ALIGNMENT_[!"$Type"!]_THRESHOLD_[!"$Index"!])
#error TS_ALIGNMENT_[!"$Type"!]_THRESHOLD_[!"$Index"!][!WS!]already defined
#endif
/** \brief Threshold of [!"$i"!]. alignment constraint for [!"$Type"!]s */
#define TS_ALIGNMENT_[!"$Type"!]_THRESHOLD_[!"$Index"!][!WS!][!"$Threshold"!]U

      [!ENDIF!]
#if (defined TS_ALIGNMENT_[!"$Type"!]_[!"$Index"!])
#error TS_ALIGNMENT_[!"$Type"!]_[!"$Index"!][!WS!]already defined
#endif
/** \brief [!"$i"!]. alignment constraint for [!"$Type"!]s */
#define TS_ALIGNMENT_[!"$Type"!]_[!"$Index"!] [!WS!][!"$Alignment"!]U

    [!ENDFOR!]
  [!ENDIF!]
#if (defined TS_ALIGNMENT_[!"$Type"!])
#error TS_ALIGNMENT_[!"$Type"!] already defined
#endif
/** \brief Default alignment constraint for [!"$Type"!]s */
#define TS_ALIGNMENT_[!"$Type"!] [!WS!][!"$DefaultAlignment"!]U

[!ENDMACRO!]
[!CALL "CreateAlignmentMacros", "List"="$AutosarTypeList","Prefix"="'Base'","Label"="'base'"!]
[!CALL "CreateComplexAlignmentMacros", "Type"="'array'", "ListThresholds"="asc:getArrayAlignmentThresholds()", "ListAlignments"="asc:getArrayAlignments()", "DefaultAlignment"="asc:getArrayAlignment()"!]
[!CALL "CreateComplexAlignmentMacros", "Type"="'struct'", "ListThresholds"="asc:getStructAlignmentThresholds()", "ListAlignments"="asc:getStructAlignments()", "DefaultAlignment"="asc:getStructAlignment()"!]
[!CALL "CreateAlignmentMacros", "List"="$DerivedTypeList","Prefix"="'Derived'","Label"="'derived'"!]
/*==================[type definitions]======================================*/

[!VAR "ListLength"="count(text:split($DerivedTypeList,','))"!]
[!FOR "i"="1" TO  "num:i($ListLength)"!]
  [!VAR "DerivedType"="text:split($DerivedTypeList,',')[num:i($i)]"!]
  [!VAR "property"="concat('DerivedType_',$DerivedType)"!]
  [!IF "node:exists(as:modconf('Base')[1]/BaseTypes/*[node:name(.)=$property])"!]
/** \brief Type definition of derived type [!"$DerivedType"!]
 *
 * Platforms Setting:
 * It is set in Platforms plugin by setting the parameter 'Derivedtypes.[!"$DerivedType"!].Mapping'.
 */
typedef[!WS!][!"as:modconf('Base')[1]/BaseTypes/*[node:name(.)=$property]"!][!WS!][!"$DerivedType"!];

  [!ENDIF!]
[!ENDFOR!]
#endif /* ifndef AUTOSAR_CFG_H */
