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

/*
 *  MISRA-C:2012 Deviations:
 *
 *  MISRAC2012-1) Deviated Rule: 8.5 (Required)
 *   An external object or function shall be declared once in one and only one file.
 *
 *    Reason:
 *    The header associated with the link time configuration isn't included by
 *    the source / header files of the module with the exception of LinIf_Init.c
 *    and only for the purpose of signature verification. The symbols aren't
 *    exposed through a header.
 *
 */

[!AUTOSPACING!]
[!INCLUDE "../include/LinIf_CddSupport.m"!]
/* !LINKSTO LinIf.ASR40.LINIF241,1 */

/*==================[inclusions]============================================*/

#include <LinIf_Lcfg.h>       /* Generated configuration */
[!IF "$endOfSchedNotifSupported = 'true' or $ulCddSupported = 'true'"!][!//
/* !LINKSTO LinIf.ASR40.LINIF669,2 */
[!CALL "GetCDDHeaders"!][!//
#include <LinIf_Types.h>            /* Needed for func ptr definition */
[!"$IncludeCDDs"!]
[!ENDIF!]
/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/* start data section declaration */
#define LINIF_START_SEC_CONST_32
#include <LinIf_MemMap.h>

/* value used to validate post build configuration against link time configuration */
CONST(uint32, LINIF_CONST) LinIf_LcfgSignature = [!"asc:getConfigSignature(as:modconf('LinIf')[1]//*[not(child::*) and (node:configclass() = 'Link')])"!]U;

/* stop data section declaration */
#define LINIF_STOP_SEC_CONST_32
#include <LinIf_MemMap.h>

[!IF "$endOfSchedNotifSupported = 'true'"!]
[!CALL "EnumarateEOSCallouts"!]
#define LINIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <LinIf_MemMap.h>
/* Deviation MISRAC2012-1 */
extern CONST(LinIf_EndOfSchedNotifFctPtrType, LINIF_CONST) LinIf_EOSNotifCallouts[[!"num:i(1 + $numEosCallouts)"!]U];
CONST(LinIf_EndOfSchedNotifFctPtrType, LINIF_CONST) LinIf_EOSNotifCallouts[[!"num:i(1 + $numEosCallouts)"!]U] =
{
  NULL_PTR,
  [!LOOP "text:split($eosCallouts, ';')"!]
    [!WS "2"!]&[!"text:split(., '|')[2]"!],
  [!ENDLOOP!]
};
#define LINIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <LinIf_MemMap.h>
[!ENDIF!]

[!IF "$ulCddSupported = 'true'"!]
[!CALL "EnumerateStateHandleFuncs"!]
[!CALL "EnumeratePduHandleFuncs"!]
#define LINIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <LinIf_MemMap.h>
/* Deviation MISRAC2012-1 <START> */
[!CALL "GetGotoSleepConfFuncs"!]
/* !LINKSTO LinIf.ASR40.LINIF601,2 */
extern CONST(LinIf_SlpWuConfFuncPtrType, LINIF_CONST) LinIf_SleepConfFuncs[[!"$numLinIf_GotoSleepConfFuncs"!]U];
CONST(LinIf_SlpWuConfFuncPtrType, LINIF_CONST) LinIf_SleepConfFuncs[[!"$numLinIf_GotoSleepConfFuncs"!]U] =
{[!"$LinIf_GotoSleepConfFuncs"!]};

[!IF "slaveSupported = 'true'"!]
[!CALL "GetGotoSleepIndFuncs"!]
/* !LINKSTO LinIf.EB.GotoSleepIndicationFunc,1 */
extern const(LinIf_SlpIndicationFuncPtrType, LINIF_CONST) LinIf_SleepIndFuncs[[!"$numLinIf_GotoSleepIndFuncs"!]U];
CONST(LinIf_SlpIndicationFuncPtrType, LINIF_CONST) LinIf_SleepIndFuncs[[!"$numLinIf_GotoSleepIndFuncs"!]U] =
{[!"$LinIf_GotoSleepIndFuncs"!]};
[!ENDIF!]

[!CALL "GetWuConfFuncs"!]
/* !LINKSTO LinIf.ASR40.LINIF602,2 */
extern CONST(LinIf_SlpWuConfFuncPtrType, LINIF_CONST) LinIf_WuConfFuncs[[!"$numLinIf_WuConfFuncs"!]U];
CONST(LinIf_SlpWuConfFuncPtrType, LINIF_CONST) LinIf_WuConfFuncs[[!"$numLinIf_WuConfFuncs"!]U] =
{[!"$LinIf_WuConfFuncs"!]};

[!CALL "GetSchedReqConfFuncs"!]
/* !LINKSTO LinIf.ASR40.LINIF600,2 */
extern CONST(LinIf_SchedReqConfFuncPtrType, LINIF_CONST) LinIf_SchedReqConfFuncs[[!"$numLinIf_SchedReqConfFuncs"!]U];
CONST(LinIf_SchedReqConfFuncPtrType, LINIF_CONST) LinIf_SchedReqConfFuncs[[!"$numLinIf_SchedReqConfFuncs"!]U] =
{[!"$LinIf_SchedReqConfFuncs"!]};

[!CALL "GetRxIndFuncs"!]
/* !LINKSTO LinIf.ASR40.LINIF610,2 */
extern CONST(LinIf_RxIndFuncPtrType, LINIF_CONST) LinIf_ULRxIndFuncs[[!"$numLinIf_RxIndFuncs"!]U];
CONST(LinIf_RxIndFuncPtrType, LINIF_CONST) LinIf_ULRxIndFuncs[[!"$numLinIf_RxIndFuncs"!]U] =
{[!"$LinIf_RxIndFuncs"!]};

[!CALL "GetTrgTxFuncs"!]
/* !LINKSTO LinIf.ASR40.LINIF608,2 */
extern CONST(LinIf_TrgTxFuncPtrType, LINIF_CONST) LinIf_ULTrgTxFuncs[[!"$numLinIf_TrgTxFuncs"!]U];
CONST(LinIf_TrgTxFuncPtrType, LINIF_CONST) LinIf_ULTrgTxFuncs[[!"$numLinIf_TrgTxFuncs"!]U] =
{[!"$LinIf_TrgTxFuncs"!]};

[!CALL "GetTxConfFuncs"!]
/* !LINKSTO LinIf.ASR40.LINIF609,2 */
extern CONST(LinIf_TxConfFuncPtrType, LINIF_CONST) LinIf_ULTxConfFuncs[[!"$numLinIf_TxConfFuncs"!]U];
CONST(LinIf_TxConfFuncPtrType, LINIF_CONST) LinIf_ULTxConfFuncs[[!"$numLinIf_TxConfFuncs"!]U] =
{[!"$LinIf_TxConfFuncs"!]};
/* Deviation MISRAC2012-1 <STOP> */
#define LINIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <LinIf_MemMap.h>
[!ENDIF!]

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
