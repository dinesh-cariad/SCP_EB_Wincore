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

/*==================[includes]==============================================*/

#include <LdCom_Lcfg.h> /* Link time configuration header  */
[!SELECT "as:modconf('LdCom')[1]/VendorSpecific/LdComUpperLayerHeaderFile"!][!//
[!IF "count(./*) > 0"!][!//
[!LOOP "./*"!][!//
#include <[!"."!]> /* Upper layer header file */
[!ENDLOOP!][!//
[!ELSE!][!//
#include <Rte_Cbk.h> /* Rte callback header file */
[!ENDIF!][!//
[!ENDSELECT!][!//
#include <LdCom_DummyCbk.h> /* Dummy callback header file */
[!INCLUDE "../include/LdCom_Vars.m"!][!//
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/* start data section declaration */
#define LDCOM_START_SEC_CONST_32
#include <LdCom_MemMap.h>

/* value used to validate post build configuration against link time configuration */
CONST(uint32, LDCOM_CONST) LdCom_LcfgSignature = [!"asc:getConfigSignature(as:modconf('LdCom')[1]//*[not(child::*) and (node:configclass() = 'Link')])"!]U;

/* stop data section declaration */
#define LDCOM_STOP_SEC_CONST_32
#include <LdCom_MemMap.h>

/* start const section declaration */
#define LDCOM_START_SEC_CONST_UNSPECIFIED
#include <LdCom_MemMap.h>
[!IF "$VarTpRxCount > 0"!]
/* API function pointer table for Rx signals from a transport protocol module */
CONST(LdCom_ApiTpRxType, LDCOM_CONST) LdCom_TpRxApi[LDCOM_TPRX_FUNCTION_PTR_COUNT] =
{[!/*
*/!][!LOOP "node:order(as:modconf('LdCom')[1]/LdComConfig/LdComIPdu/*[./LdComApiType='LDCOM_TP'][./LdComIPduDirection='LDCOM_RECEIVE'], 'node:LdComHandleId')"!]
  { /* [[!"./LdComHandleId"!]] [!"node:name(.)"!] */[!INDENT "4"!]
  [!IF "node:exists(./LdComRxStartOfReception) and (node:value(./LdComRxStartOfReception) != '')"!]&[!"./LdComRxStartOfReception"!][!ELSE!][!"'&LdCom_DummyCbkStartOfReception'"!][!ENDIF!], /* StartOfReception */
  [!IF "node:exists(./LdComRxCopyRxData) and (node:value(./LdComRxCopyRxData) != '')"!]&[!"./LdComRxCopyRxData"!][!ELSE!][!"'&LdCom_DummyCbkCopyRxData'"!][!ENDIF!], /* CopyRxData */
  [!IF "node:exists(./LdComTpRxIndication) and (node:value(./LdComTpRxIndication) != '')"!]&[!"./LdComTpRxIndication"!][!ELSE!][!"'&LdCom_DummyCbkTpRxIndication'"!][!ENDIF!] /* TpRxIndication */[!/*
  */!][!ENDINDENT!]
  },[!/*
*/!][!ENDLOOP!]
};
[!ENDIF!]
[!IF "$VarTpTxCount > 0"!]
/* API function pointer table for Tx signals to a transport protocol module */
CONST(LdCom_ApiTpTxType, LDCOM_CONST) LdCom_TpTxApi[LDCOM_TPTX_FUNCTION_PTR_COUNT] =
{[!/*
*/!][!LOOP "node:order(as:modconf('LdCom')[1]/LdComConfig/LdComIPdu/*[./LdComApiType='LDCOM_TP'][./LdComIPduDirection='LDCOM_SEND'], 'node:LdComHandleId')"!]
  { /* [[!"./LdComHandleId"!]] [!"node:name(.)"!] */[!INDENT "4"!]
  [!IF "node:exists(./LdComTxCopyTxData) and (node:value(./LdComTxCopyTxData) != '')"!]&[!"./LdComTxCopyTxData"!][!ELSE!][!"'&LdCom_DummyCbkCopyTxData'"!][!ENDIF!], /* CopyTxData */
  [!IF "node:exists(./LdComTpTxConfirmation) and (node:value(./LdComTpTxConfirmation) != '')"!]&[!"./LdComTpTxConfirmation"!][!ELSE!][!"'&LdCom_DummyCbkTpTxConfirmation'"!][!ENDIF!] /* TpTxConfirmation */[!/*
  */!][!ENDINDENT!]
  },[!/*
*/!][!ENDLOOP!]
};
[!ENDIF!]
[!IF "$VarIfRxCount > 0"!]
/* API function pointer table for Rx signals from a communication interface module */
CONST(LdCom_ApiIfRxType, LDCOM_CONST) LdCom_IfRxApi[LDCOM_IFRX_FUNCTION_PTR_COUNT] =
{[!/*
*/!][!LOOP "node:order(as:modconf('LdCom')[1]/LdComConfig/LdComIPdu/*[./LdComApiType='LDCOM_IF'][./LdComIPduDirection='LDCOM_RECEIVE'], 'node:LdComHandleId')"!]
  { /* [[!"./LdComHandleId"!]] [!"node:name(.)"!] */[!INDENT "4"!]
  [!IF "node:exists(./LdComRxIndication) and (node:value(./LdComRxIndication) != '')"!]&[!"./LdComRxIndication"!][!ELSE!][!"'&LdCom_DummyCbkRxIndication'"!][!ENDIF!] /* RxIndication */[!/*
  */!][!ENDINDENT!]
  },[!/*
*/!][!ENDLOOP!]
};
[!ENDIF!]
[!IF "$VarIfTxCount > 0"!]
/* API function pointer table for Tx signals to a communication interface module */
CONST(LdCom_ApiIfTxType, LDCOM_CONST) LdCom_IfTxApi[LDCOM_IFTX_FUNCTION_PTR_COUNT] =
{[!/*
*/!][!LOOP "node:order(as:modconf('LdCom')[1]/LdComConfig/LdComIPdu/*[./LdComApiType='LDCOM_IF'][./LdComIPduDirection='LDCOM_SEND'], 'node:LdComHandleId')"!]
  { /* [[!"./LdComHandleId"!]] [!"node:name(.)"!] */[!INDENT "4"!]
  [!IF "node:exists(./LdComTxTriggerTransmit) and (node:value(./LdComTxTriggerTransmit) != '')"!]&[!"./LdComTxTriggerTransmit"!][!ELSE!][!"'&LdCom_DummyCbkTriggerTransmit'"!][!ENDIF!], /* TriggerTx */
  [!IF "node:exists(./LdComTxConfirmation) and (node:value(./LdComTxConfirmation) != '')"!]&[!"./LdComTxConfirmation"!][!ELSE!][!"'&LdCom_DummyCbkTxConfirmation'"!][!ENDIF!] /* TxConfirmation */[!/*
  */!][!ENDINDENT!]
  },[!/*
*/!][!ENDLOOP!]
};
[!ENDIF!]
/* stop const section declaration */
#define LDCOM_STOP_SEC_CONST_UNSPECIFIED
#include <LdCom_MemMap.h>

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
