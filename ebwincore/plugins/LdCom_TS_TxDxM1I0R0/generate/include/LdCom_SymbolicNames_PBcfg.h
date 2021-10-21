/**
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
 */
#ifndef LDCOM_SYMBOLIC_NAMES_PBCFG_H
#define LDCOM_SYMBOLIC_NAMES_PBCFG_H

[!INCLUDE "LdCom_Macros.m"!]

/*==================[inclusions]============================================*/

/*==================[macros]================================================*/

/*****************************************/
/* Symbolic name values of LdComHandleId */
/*****************************************/
[!INDENT "0"!][!//
  [!LOOP "LdComConfig/LdComIPdu/*"!]
    /*################################################################################################*/
    /* I-PDU: [!"name(.)"!] */
    [!CALL "GetSymbolName",
      "ShortNameRef"="'.'"!][!//
    [!CALL "GenSymbols",
      "SymbolicPrefix"="'LdComConf_LdComIPdu_'",
      "SymbolicName"="$SymbolName",
      "SymbolicIdName"="'LdComHandleId'",
      "SymbolicValue"="node:path(./LdComHandleId)",
      "PduName"="name(node:ref(./LdComPduRef))"!]
  [!ENDLOOP!]
[!ENDINDENT!][!//

/*********************************/
/* Source PDU-IDs of PduR module */
/*********************************/
#if (defined LDCOM_PROVIDE_ADJACENT_MODULE_VENDOR_SYMBOLIC_NAMES)
[!INDENT "0"!][!//
  [!LOOP "LdComConfig/LdComIPdu/*[./LdComIPduDirection = 'LDCOM_SEND']"!]
    /*################################################################################################*/
    /* I-PDU: [!"name(.)"!] */[!/*
    */!][!CALL "GetOutputPduId","LdComPduRef"="./LdComPduRef"!]
    #if (defined PDUR_SOURCE_PDUID_[!"name(./..)"!]_[!"name(.)"!])
    #error PDUR_SOURCE_PDUID_[!"name(./..)"!]_[!"name(.)"!] is already defined
    #endif
    /* [!"node:path(LdComPduRef)"!] */
    #define PDUR_SOURCE_PDUID_[!"name(./..)"!]_[!"name(.)"!] [!"num:i($PduID)"!]U /* [!"name(node:ref(LdComPduRef))"!] */
  [!ENDLOOP!][!//
[!ENDINDENT!][!//
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef LDCOM_SYMBOLIC_NAMES_PBCFG_H */
/*==================[end of file]===========================================*/
