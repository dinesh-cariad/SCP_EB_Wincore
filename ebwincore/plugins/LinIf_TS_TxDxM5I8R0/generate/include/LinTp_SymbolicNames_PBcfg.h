/**
 * \file
 *
 * \brief AUTOSAR LinIf
 *
 * This file contains the implementation of the AUTOSAR
 * module LinIf.
 *
 * \version 5.8.27
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef LINTP_SYMBOLICNAMES_PBCFG_H
#define LINTP_SYMBOLICNAMES_PBCFG_H

/*==================[includes]===============================================*/

/*==================[ Macros]=================================================*/

[!IF "LinIfGeneral/LinIfTpSupported = 'true'"!]
[!INCLUDE "LinIf_Macros.m"!][!//

/* generating macros for LinTp RxNSdus */[!/*
*/!][!LOOP "as:modconf('LinTp')[1]/LinTpGlobalConfig/*/LinTpRxNSdu/*"!][!/*
*/!][!CALL "GetSymbolName","ShortNameRef"="'.'","OldName"="node:name(../.)"!]
/** \brief Export symbolic name value for [!"@name"!] */
#if (defined LinTpConf_[!"$SymbolName"!])
#error LinTpConf_[!"$SymbolName"!] already defined!
#endif /* #if( defined LinTpConf_[!"$SymbolName"!])*/
#define LinTpConf_[!"$SymbolName"!] [!"LinTpRxNSduId"!]U[!/*
*/!]
[!ENDLOOP!]


/* generating macros for LinTp TxNSdus */[!/*
*/!][!LOOP "as:modconf('LinTp')[1]/LinTpGlobalConfig/*/LinTpTxNSdu/*"!][!/*
*/!][!CALL "GetSymbolName","ShortNameRef"="'.'","OldName"="node:name(../.)"!]
/** \brief Export symbolic name value for [!"@name"!] */
#if (defined LinTpConf_[!"$SymbolName"!])
#error LinTpConf_[!"$SymbolName"!] already defined!
#endif /* #if( defined LinTpConf_[!"$SymbolName"!])*/
#define LinTpConf_[!"$SymbolName"!] [!"LinTpTxNSduId"!]U[!/*
*/!]
[!ENDLOOP!]

[!ENDIF!]

#endif /* ifndef LINTP_SYMBOLICNAMES_PBCFG_H */
/*==================[end of file]============================================*/
