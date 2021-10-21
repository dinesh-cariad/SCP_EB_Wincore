[!AUTOSPACING!][!//
[!/**
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
 */!][!//
[!/* List of all AUTOSAR types provided as property */!][!//
[!VAR "AutosarTypeList"="'boolean,sint8,sint16,sint32,uint8,uint16,uint32,sint8_least,sint16_least,sint32_least,uint8_least,uint16_least,uint32_least,float32,float64'"!][!//
[!IF "node:exists(as:modconf('Base')[1]/BaseTypes/BaseTypes64bit) and (as:modconf('Base')[1]/BaseTypes/BaseTypes64bit = 'true')"!][!//
[!VAR "AutosarTypeList"="concat($AutosarTypeList,',sint64,uint64')"!][!//
[!ENDIF!][!//
[!/* List of bit width of all available AUTOSAR types as property */!][!//
[!VAR "BitWidthList"="'8,16,32'"!][!//
[!IF "node:exists(as:modconf('Base')[1]/BaseTypes/BaseTypes64bit) and (as:modconf('Base')[1]/BaseTypes/BaseTypes64bit = 'true')"!][!//
[!VAR "BitWidthList"="concat($BitWidthList,',64')"!][!//
[!ENDIF!][!//
[!/* List of all C90 types provided as property */!][!//
[!VAR "CTypeList"="'char,short,int,long,float,double'"!][!//
[!IF "node:exists(as:modconf('Base')[1]/BaseTypes/BaseTypes64bit) and (as:modconf('Base')[1]/BaseTypes/BaseTypes64bit = 'true')"!][!//
[!VAR "CTypeList"="concat($CTypeList,',long long')"!][!//
[!ENDIF!][!//
[!/* List of all complex types provided as property */!][!//
[!VAR "ComplexTypeList"="'struct,array'"!][!//
[!/* List of all complex types provided as property */!][!//
[!VAR "DerivedTypeList"="'TS_MaxAlignedType'"!][!//