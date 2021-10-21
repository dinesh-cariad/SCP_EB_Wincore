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

#ifndef LDCOM_CFG_H
#define LDCOM_CFG_H

/*==================[inclusions]============================================*/
[!INCLUDE "../include/LdCom_Vars.m"!][!//
#include <TSAutosar.h>              /* EB specific standard types */
#include <ComStack_Types.h>         /* AUTOSAR standard types (ON/OFF) */

/*==========================================================================*/

/*==================[macros]================================================*/
#if (defined LDCOM_VERSION_INFO_API)
#error LDCOM_VERSION_INFO_API already defined
#endif
/** \brief Macro for version info api */
#define LDCOM_VERSION_INFO_API [!//
[!IF "LdComGeneral/LdComVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined LDCOM_DEV_ERROR_DETECT)
#error LDCOM_DEV_ERROR_DETECT already defined
#endif
/** \brief Macro for DET usage */
#define LDCOM_DEV_ERROR_DETECT [!//
[!IF "LdComGeneral/LdComDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined LDCOM_TPRXAPIS_AVAILABLE)
#error LDCOM_TPRXAPIS_AVAILABLE already defined
#endif
/** \brief APIs LdCom_StartOfReception, LdCom_CopyRxData and LdCom_TpRxIndication are available */
#define LDCOM_TPRXAPIS_AVAILABLE [!//
[!IF "$VarTpRxCount > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined LDCOM_TPTXAPIS_AVAILABLE)
#error LDCOM_TPTXAPIS_AVAILABLE already defined
#endif
/** \brief APIs LdCom_CopyTxData, LdCom_TpTxConfirmation and LdCom_TpTransmit are available */
#define LDCOM_TPTXAPIS_AVAILABLE [!//
[!IF "$VarTpTxCount > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined LDCOM_IFRXAPIS_AVAILABLE)
#error LDCOM_IFRXAPIS_AVAILABLE already defined
#endif
/** \brief API LdCom_RxIndication are available */
#define LDCOM_IFRXAPIS_AVAILABLE [!//
[!IF "$VarIfRxCount > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined LDCOM_IFTXAPIS_AVAILABLE)
#error LDCOM_IFTXAPIS_AVAILABLE already defined
#endif
/** \brief APIs LdCom_TriggerTransmit, LdCom_TxConfirmation and LdCom_IfTransmit are available 
** in general */
#define LDCOM_IFTXAPIS_AVAILABLE [!//
[!IF "$VarIfTxCount > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined LDCOM_TRIGGERTRANSMIT_AVAILABLE)
#error LDCOM_TRIGGERTRANSMIT_AVAILABLE already defined
#endif
/** \brief At least a single LdCom_TriggerTransmit is available */
#define LDCOM_TRIGGERTRANSMIT_AVAILABLE [!//
[!IF "$VarTrigTxCount > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined LDCOM_TXCONFIRMATION_AVAILABLE)
#error LDCOM_TXCONFIRMATION_AVAILABLE already defined
#endif
/** \brief At least a single LdCom_TxConfirmation is available */
#define LDCOM_TXCONFIRMATION_AVAILABLE [!//
[!IF "$VarTxConfCount > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!//

[!SELECT "VendorSpecific"!]

#if (defined LDCOM_PBCFGM_SUPPORT_ENABLED)
#error LDCOM_PBCFGM_SUPPORT_ENABLED already defined
#endif
/** \brief Indicates PbcfgM support for LdCom */
#define LDCOM_PBCFGM_SUPPORT_ENABLED [!IF "node:contains(node:refs(as:modconf('PbcfgM')/PbcfgMBswModules/*/PbcfgMBswModuleRef), as:modconf('LdCom')) = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


/* ----- Pre-processor switch to enable/disable relocatable postbuild config ----- */
#if (LDCOM_PBCFGM_SUPPORT_ENABLED == STD_ON)

#if (defined LDCOM_RELOCATABLE_CFG_ENABLE)
#error LDCOM_RELOCATABLE_CFG_ENABLE already defined
#endif
#define LDCOM_RELOCATABLE_CFG_ENABLE [!IF "as:modconf('PbcfgM')/PbcfgMGeneral/PbcfgMRelocatableCfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#else

#if (defined LDCOM_RELOCATABLE_CFG_ENABLE)
#error LDCOM_RELOCATABLE_CFG_ENABLE already defined
#endif
#define LDCOM_RELOCATABLE_CFG_ENABLE [!IF "LdComRelocatablePbcfgEnable = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#endif
[!IF "not(node:exists(LdComRelocatablePbcfgEnable))"!]
[!ERROR!]Parameter LdComRelocatablePbcfgEnable can not be found in current path. Use SELECT to specify the path.[!ENDERROR!]
[!ENDIF!]
[!ENDSELECT!][!//

#if (defined LDCOM_CFG_SIGNATURE)
#error LDCOM_CFG_SIGNATURE already defined
#endif
/** \brief Compile time verification value */
#define LDCOM_CFG_SIGNATURE [!"asc:getConfigSignature(node:difference(as:modconf('LdCom')[1]//*[not(child::*) and (node:configclass() = 'PreCompile') and not (name(.) = 'LdComUpperLayerHeaderFile')], as:modconf('LdCom')[1]/VendorSpecific/LdComUpperLayerHeaderFile/*))"!]U

#if (defined LDCOM_PUBLIC_INFO_SIGNATURE)
#error LDCOM_PUBLIC_INFO_SIGNATURE already defined
#endif
/** \brief Compile time Published information verfication value */
#define LDCOM_PUBLIC_INFO_SIGNATURE  [!"asc:getConfigSignature(node:difference(as:modconf('LdCom')[1]/CommonPublishedInformation//*[not(child::*) and (node:configclass() = 'PublishedInformation') ], as:modconf('LdCom')[1]/CommonPublishedInformation/Release))"!]U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef LDCOM_CFG_H */
/*==================[end of file]===========================================*/
