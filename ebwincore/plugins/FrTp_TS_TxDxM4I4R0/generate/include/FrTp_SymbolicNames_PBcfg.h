/**
 * \file
 *
 * \brief AUTOSAR FrTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrTp.
 *
 * \version 4.4.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!CODE!]
#ifndef FRTP_SYMBOLICNAMES_PBCFG_H
#define FRTP_SYMBOLICNAMES_PBCFG_H

/*==================[macros]=================================================*/
[!INCLUDE "FrTp_Variables.m"!]

/*------------------[symbolic name definitions]------------------------------*/
[!AUTOSPACING!]

/* generate macros for FrTpTxSduId */
[!LOOP "as:modconf('FrTp')[1]/FrTpMultipleConfig/*[1]/FrTpConnection/*"!]
[!IF "node:exists(./FrTpTxSdu)"!]
[!CALL "GetSymbolName","ShortNameRef"="'FrTpTxSdu'","OldName"="name(./FrTpTxSdu)"!]
/*---------------------------[!"$SymbolName"!]-----------------------------------*/
[!CALL "GuardedDefine", "Comment"="'Export symbolic name value AUTOSAR version = 4.0 rev3'",
  "Name"="concat('FrTpConf_', $SymbolName)"!] [!"./FrTpTxSdu/FrTpTxSduId"!]U
#if (defined FRTP_PROVIDE_LEGACY_SYMBOLIC_NAMES)
[!CALL "GuardedDefine", "Comment"="'Export symbolic name value with module abbreviation as prefix only (AUTOSAR 4.0 rev2 <= AUTOSAR version)'",
  "Name"="concat('FrTp_', @name,'_FrTpTxSdu')"!] [!"./FrTpTxSdu/FrTpTxSduId"!]U
#endif /* defined FRTP_PROVIDE_LEGACY_SYMBOLIC_NAMES */

[!ENDIF!]
[!ENDLOOP!]

/* generate macros for FrTpRxNPdus */
[!LOOP "as:modconf('FrTp')[1]/FrTpMultipleConfig/*[1]/FrTpRxPduPool/*/FrTpRxPdu/*"!]
[!CALL "GetSymbolName","ShortNameRef"="'.'","OldName"="name(..)"!]
/*-----------------------[!"$SymbolName"!]----------------------------*/
[!CALL "GuardedDefine", "Comment"="'Export symbolic name value AUTOSAR version = 4.0 rev3'",
  "Name"="concat('FrTpConf_', $SymbolName)"!] [!"FrTpRxPduId"!]U
#if (defined FRTP_PROVIDE_LEGACY_SYMBOLIC_NAMES)
[!CALL "GuardedDefine", "Comment"="'Export symbolic name value with module abbreviation as prefix only (AUTOSAR 4.0 rev2 <= AUTOSAR version)'",
  "Name"="concat('FrTp_', ../../@name, '_', @name)"!] [!"FrTpRxPduId"!]U
#endif /* defined FRTP_PROVIDE_LEGACY_SYMBOLIC_NAMES */

[!ENDLOOP!]

/* generate macros for FrTpTxConfirmationPduId */
[!LOOP "FrTpMultipleConfig/*[1]/FrTpTxPduPool/*/FrTpTxPdu/*"!]
[!CALL "GetSymbolName","ShortNameRef"="'.'","OldName"="name(..)"!]
/*-----------------------[!"$SymbolName"!]----------------------------*/
[!CALL "GuardedDefine", "Comment"="'Export symbolic name value AUTOSAR version = 4.0 rev3'",
  "Name"="concat('FrTpConf_', $SymbolName)"!] [!"FrTpTxConfirmationPduId"!]U
#if (defined FRTP_PROVIDE_LEGACY_SYMBOLIC_NAMES)
[!CALL "GuardedDefine", "Comment"="'Export symbolic name value with module abbreviation as prefix only (AUTOSAR 4.0 rev2 <= AUTOSAR version)'",
  "Name"="concat('FrTp_', ../../@name, '_', @name)"!] [!"FrTpTxConfirmationPduId"!]U
#endif /* defined FRTP_PROVIDE_LEGACY_SYMBOLIC_NAMES */

[!ENDLOOP!]

/* generate macros for FrTpRxSduId */
[!LOOP "as:modconf('FrTp')[1]/FrTpMultipleConfig/*[1]/FrTpConnection/*"!]
[!IF "node:exists(./FrTpRxSdu)"!]
[!CALL "GetSymbolName","ShortNameRef"="'FrTpRxSdu'","OldName"="name(./FrTpRxSdu)"!]
/*---------------------------[!"$SymbolName"!]-----------------------------------*/
[!CALL "GuardedDefine", "Comment"="'Export symbolic name value AUTOSAR version = 4.0 rev3'",
  "Name"="concat('FrTpConf_', $SymbolName)"!] [!"./FrTpRxSdu/FrTpRxSduId"!]U
#if (defined FRTP_PROVIDE_LEGACY_SYMBOLIC_NAMES)
[!CALL "GuardedDefine", "Comment"="'Export symbolic name value with module abbreviation as prefix only (AUTOSAR 4.0 rev2 <= AUTOSAR version)'",
  "Name"="concat('FrTp_', @name, '_FrTpRxSdu')"!] [!"./FrTpRxSdu/FrTpRxSduId"!]U
#endif /* defined FRTP_PROVIDE_LEGACY_SYMBOLIC_NAMES */

[!ENDIF!]
[!ENDLOOP!]

#endif /* FRTP_SYMBOLICNAMES_PBCFG_H */
[!ENDCODE!]
