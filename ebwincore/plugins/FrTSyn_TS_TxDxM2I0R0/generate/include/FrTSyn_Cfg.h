/**
 * \file
 *
 * \brief AUTOSAR FrTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module FrTSyn.
 *
 * \version 2.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!CODE!]
[!AUTOSPACING!]

#ifndef FRTSYN_CFG_H
#define FRTSYN_CFG_H

/*==================[includes]==============================================*/

[!INCLUDE "FrTSyn_PrecompileMacros.m"!][!//

#include <TSAutosar.h>              /* global configuration */
#include <FrTSyn_Types.h>
/*==================[macros]=================================================*/

/** \brief Switch for DET usage */
#if (defined FRTSYN_DEV_ERROR_DETECT)
#error FRTSYN_DEV_ERROR_DETECT already defined!
#endif /* #if (defined FRTSYN_DEV_ERROR_DETECT)*/
#define FRTSYN_DEV_ERROR_DETECT        STD_[!IF "FrTSynGeneral/FrTSynDevErrorDetect = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


/** \brief Support of FrTSyn_GetVersionInfo()
  ** This constant defines whether the FrTSyn_GetVersionInfo is
  ** provided.
  ** STD_ON    Function is provided
  ** STD_OFF   Function is not provided
*/
/* !LINKSTO FrTSyn.ASR44.ECUC_FrTSyn_00019,1 */
#if (defined FRTSYN_VERSION_INFO_API)
#error FRTSYN_VERSION_INFO_API already defined!
#endif /* #if (defined FRTSYN_VERSION_INFO_API)*/
#define FRTSYN_VERSION_INFO_API       STD_[!IF "FrTSynGeneral/FrTSynVersionInfoApi = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

[!IF "num:i($FrTSyn_TimeDomainSlavesSize) > num:i(0)"!][!//
/** \brief Number of the Time Domain Slaves*/
#if (defined FRTSYN_NUMBER_OF_TIME_SLAVES)
#error FRTSYN_NUMBER_OF_TIME_SLAVES already defined!
#endif /* #if (defined FRTSYN_NUMBER_OF_TIME_SLAVES)*/
#define FRTSYN_NUMBER_OF_TIME_SLAVES        [!"num:i($FrTSyn_TimeDomainSlavesSize)"!]U
[!ENDIF!][!//

#if (defined FRTSYN_IS_SLAVE_FUNC_USED)
#error FRTSYN_IS_SLAVE_FUNC_USED already defined!
#endif /* #if (defined FRTSYN_IS_SLAVE_FUNC_USED)*/
#define FRTSYN_IS_SLAVE_FUNC_USED     STD_[!IF "num:i($FrTSyn_TimeDomainSlavesSize) > num:i(0)"!]ON[!ELSE!]OFF[!ENDIF!]

[!IF "num:i($FrTSyn_TimeDomainMastersSize) > num:i(0)"!][!//
/** \brief Number of the Time Domain Masters*/
#if (defined FRTSYN_NUMBER_OF_TIME_MASTERS)
#error FRTSYN_NUMBER_OF_TIME_MASTERS already defined!
#endif /* #if (defined FRTSYN_NUMBER_OF_TIME_MASTERS)*/
#define FRTSYN_NUMBER_OF_TIME_MASTERS        [!"num:i($FrTSyn_TimeDomainMastersSize)"!]U
[!ENDIF!][!//

#if (defined FRTSYN_IS_MASTER_FUNC_USED)
#error FRTSYN_IS_MASTER_FUNC_USED already defined!
#endif /* #if (defined FRTSYN_IS_MASTER_FUNC_USED)*/
#define FRTSYN_IS_MASTER_FUNC_USED     STD_[!IF "num:i($FrTSyn_TimeDomainMastersSize) > num:i(0)"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined FRTSYN_IS_ITS_MASTER_FUNC_USED)
#error FRTSYN_IS_ITS_MASTER_FUNC_USED already defined!
#endif /* #if (defined FRTSYN_IS_ITS_MASTER_FUNC_USED)*/
#define FRTSYN_IS_ITS_MASTER_FUNC_USED     STD_[!IF "$immediateTS = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined FRTSYN_CRC_STARTVAL)
#error FRTSYN_CRC_STARTVAL already defined!
#endif /* #if (defined FRTSYN_CRC_STARTVAL)*/
#define FRTSYN_CRC_STARTVAL   0xFFU

[!/* These shall be removed / modified, with the static code once PB is supported */!][!//
#if (defined FRTSYN_CRC_CHECK_USED)
#error FRTSYN_CRC_CHECK_USED already defined!
#endif /* #if (defined FRTSYN_CRC_CHECK_USED)*/
#define FRTSYN_CRC_CHECK_USED   STD_[!IF "$FrTSyn_NumGTDILEs > 0"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined FRTSYN_TXTYPE_PERIODIC_USED)
#error FRTSYN_TXTYPE_PERIODIC_USED already defined!
#endif /* #if (defined FRTSYN_TXTYPE_PERIODIC_USED)*/
#if (defined FRTSYN_TX_PERIODIC_LUT_NAME)
#error FRTSYN_TX_PERIODIC_LUT_NAME already defined!
#endif /* #if (defined FRTSYN_TX_PERIODIC_LUT_NAME)*/
[!IF "bit:and($usedTxTypes, $TX_PERIODIC) > 0"!]
#define FRTSYN_TXTYPE_PERIODIC_USED   STD_ON
#define FRTSYN_TX_PERIODIC_LUT_NAME   FrTSyn_TxCondFunc_Periodic
[!ELSE!]
#define FRTSYN_TXTYPE_PERIODIC_USED   STD_OFF
#define FRTSYN_TX_PERIODIC_LUT_NAME   NULL_PTR
[!ENDIF!]

#if (defined FRTSYN_TXTYPE_IMMEDIATE_USED)
#error FRTSYN_TXTYPE_IMMEDIATE_USED already defined!
#endif /* #if (defined FRTSYN_TXTYPE_IMMEDIATE_USED)*/
#if (defined FRTSYN_TX_IMMEDIATE_LUT_NAME)
#error FRTSYN_TX_IMMEDIATE_LUT_NAME already defined!
#endif /* #if (defined FRTSYN_TX_IMMEDIATE_LUT_NAME)*/
[!IF "bit:and($usedTxTypes, $TX_IMMEDIATE) > 0"!]
#define FRTSYN_TXTYPE_IMMEDIATE_USED   STD_ON
#define FRTSYN_TX_IMMEDIATE_LUT_NAME   FrTSyn_TxCondFunc_Immediate
[!ELSE!]
#define FRTSYN_TXTYPE_IMMEDIATE_USED   STD_OFF
#define FRTSYN_TX_IMMEDIATE_LUT_NAME   NULL_PTR
[!ENDIF!]

#if (defined FRTSYN_TXTYPE_BOTH_USED)
#error FRTSYN_TXTYPE_BOTH_USED already defined!
#endif /* #if (defined FRTSYN_TXTYPE_BOTH_USED)*/
#if (defined FRTSYN_TX_BOTH_LUT_NAME)
#error FRTSYN_TX_BOTH_LUT_NAME already defined!
#endif /* #if (defined FRTSYN_TX_BOTH_LUT_NAME)*/
[!IF "bit:and($usedTxTypes, $TX_BOTH) > 0"!]
#define FRTSYN_TXTYPE_BOTH_USED   STD_ON
#define FRTSYN_TX_BOTH_LUT_NAME   FrTSyn_TxCondFunc_Both
[!ELSE!]
#define FRTSYN_TXTYPE_BOTH_USED   STD_OFF
#define FRTSYN_TX_BOTH_LUT_NAME   NULL_PTR
[!ENDIF!]

#if (defined FRTSYN_TIME_VALIDATION_ENABLED)
#error FRTSYN_TIME_VALIDATION_ENABLED already defined !
#endif /* (defined FRTSYN_TIME_VALIDATION_ENABLED) */
#define FRTSYN_TIME_VALIDATION_ENABLED   STD_[!IF "$FrTSyn_TimeValidationEnabled = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined FRTSYN_FRIF_TX_MODE_IMMEDIATE)
#error FRTSYN_FRIF_TX_MODE_IMMEDIATE already defined !
#endif /* (defined FRTSYN_FRIF_TX_MODE_IMMEDIATE) */
#define FRTSYN_FRIF_TX_MODE_IMMEDIATE   0U

#if (defined FRTSYN_FRIF_TX_MODE_DECOUPLED)
#error FRTSYN_FRIF_TX_MODE_DECOUPLED already defined !
#endif /* (defined FRTSYN_FRIF_TX_MODE_DECOUPLED) */
#define FRTSYN_FRIF_TX_MODE_DECOUPLED   1U

#if (defined FRTSYN_TIME_VALIDATION_IS_ENABLED)
#error FRTSYN_TIME_VALIDATION_IS_ENABLED already defined !
#endif /* (defined FRTSYN_TIME_VALIDATION_IS_ENABLED) */
#define FRTSYN_TIME_VALIDATION_IS_ENABLED   TRUE

#if (defined FRTSYN_TIME_VALIDATION_NOT_ENABLED)
#error FRTSYN_TIME_VALIDATION_NOT_ENABLED already defined !
#endif /* (defined FRTSYN_TIME_VALIDATION_NOT_ENABLED) */
#define FRTSYN_TIME_VALIDATION_NOT_ENABLED   FALSE
/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

#define FRTSYN_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <FrTSyn_MemMap.h>

[!IF "num:i($FrTSyn_TimeDomainSlavesSize) > num:i(0)"!][!//
/** \brief FrTSyn Slave configuration
 **
 ** This is the FrTSyn Slave configuration.
 ** Please note that the name of this element is configuration dependent.
 */

extern CONST(FrTSyn_SlaveTimeDomainConfigType, FRTSYN_APPL_CONST) FrTSyn_SlaveConfig[[!"num:i($FrTSyn_TimeDomainSlavesSize)"!]U];
[!ENDIF!][!//

[!IF "num:i($FrTSyn_TimeDomainMastersSize) > num:i(0)"!][!//
/** \brief FrTSyn Master configuration
 **
 ** This is the FrTSyn Master configuration.
 ** Please note that the name of this element is configuration dependent.
 */
extern CONST(FrTSyn_MasterTimeDomainConfigType, FRTSYN_APPL_CONST) FrTSyn_MasterConfig[[!"num:i($FrTSyn_TimeDomainMastersSize)"!]U];
[!ENDIF!][!//

#define FRTSYN_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <FrTSyn_MemMap.h>

[!IF "$FrTSyn_NumGTDILEs > 0"!]
#define FRTSYN_START_SEC_CONFIG_DATA_8
#include <FrTSyn_MemMap.h>
/** \brief FrTSyn DataIDList configuration
 **
 ** This is the FrTSyn DataIDList configuration.
 ** Please note that the name of this element is configuration dependent.
 */
extern CONST(FrTSyn_DataIDType, FRTSYN_APPL_CONST) FrTSyn_GlobalTimeDataIDListElements[[!"$FrTSyn_NumGTDILEs"!]U];
#define FRTSYN_STOP_SEC_CONFIG_DATA_8
#include <FrTSyn_MemMap.h>
[!ENDIF!]
/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef FRTSYN_CFG_H */

/*==================[end of file]============================================*/

[!ENDCODE!]


