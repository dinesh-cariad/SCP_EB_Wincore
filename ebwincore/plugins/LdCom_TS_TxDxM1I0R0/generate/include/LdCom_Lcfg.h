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
[!CODE!]
[!AUTOSPACING!]
#ifndef LDCOM_LCFG_H
#define LDCOM_LCFG_H

/*==================[inclusions]============================================*/
[!INCLUDE "../include/LdCom_Vars.m"!][!//
#include <TSAutosar.h>          /* EB specific standard types */
#include <LdCom_Cbk.h>           /* Module interface */
#include <LdCom_Cfg.h>           /* Module configurable parameters */

/*==================[macros]================================================*/
#if (defined LDCOM_TPTX_FUNCTION_PTR_COUNT)
#error LDCOM_TPTX_FUNCTION_PTR_COUNT already defined
#endif /* if (defined LDCOM_TPTX_FUNCTION_PTR_COUNT) */
#define LDCOM_TPTX_FUNCTION_PTR_COUNT [!"$VarTpTxCount"!]U
#if (defined LDCOM_TPRX_FUNCTION_PTR_COUNT)
#error LDCOM_TPRX_FUNCTION_PTR_COUNT already defined
#endif /* if (defined LDCOM_TPRX_FUNCTION_PTR_COUNT) */
#define LDCOM_TPRX_FUNCTION_PTR_COUNT [!"$VarTpRxCount"!]U
#if (defined LDCOM_IFTX_FUNCTION_PTR_COUNT)
#error LDCOM_IFTX_FUNCTION_PTR_COUNT already defined
#endif /* if (defined LDCOM_IFTX_FUNCTION_PTR_COUNT) */
#define LDCOM_IFTX_FUNCTION_PTR_COUNT [!"$VarIfTxCount"!]U
#if (defined LDCOM_IFRX_FUNCTION_PTR_COUNT)
#error LDCOM_IFRX_FUNCTION_PTR_COUNT already defined
#endif /* if (defined LDCOM_IFRX_FUNCTION_PTR_COUNT) */
#define LDCOM_IFRX_FUNCTION_PTR_COUNT [!"$VarIfRxCount"!]U
/*==================[Error codes]===========================================*/

/*==================[macro checks]==========================================*/

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
extern CONST(uint32, LDCOM_CONST) LdCom_LcfgSignature;

/* stop data section declaration */
#define LDCOM_STOP_SEC_CONST_32
#include <LdCom_MemMap.h>

/* start const section declaration */
#define LDCOM_START_SEC_CONST_UNSPECIFIED
#include <LdCom_MemMap.h>

[!IF "$VarTpRxCount > 0"!]
/* API function pointer table for Rx signals from a transport protocol module */
extern CONST(LdCom_ApiTpRxType, LDCOM_CONST) LdCom_TpRxApi[LDCOM_TPRX_FUNCTION_PTR_COUNT];
[!ENDIF!]

[!IF "$VarTpTxCount > 0"!]
/* API function pointer table for Tx signals to a transport protocol module */
extern CONST(LdCom_ApiTpTxType, LDCOM_CONST) LdCom_TpTxApi[LDCOM_TPTX_FUNCTION_PTR_COUNT];
[!ENDIF!]

[!IF "$VarIfRxCount > 0"!]
/* API function pointer table for Rx signals from a communication interface module */
extern CONST(LdCom_ApiIfRxType, LDCOM_CONST) LdCom_IfRxApi[LDCOM_IFRX_FUNCTION_PTR_COUNT];
[!ENDIF!]

[!IF "$VarIfTxCount > 0"!]
/* API function pointer table for Tx signals to a communication interface module */
extern CONST(LdCom_ApiIfTxType, LDCOM_CONST) LdCom_IfTxApi[LDCOM_IFTX_FUNCTION_PTR_COUNT];
[!ENDIF!]

/* stop const section declaration */
#define LDCOM_STOP_SEC_CONST_UNSPECIFIED
#include <LdCom_MemMap.h>
/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef LDCOM_LCFG_H */
/*==================[end of file]===========================================*/
[!ENDCODE!][!//

