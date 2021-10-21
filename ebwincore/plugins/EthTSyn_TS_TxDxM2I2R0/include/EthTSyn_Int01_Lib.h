/**
 * \file
 *
 * \brief AUTOSAR EthTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module EthTSyn.
 *
 * \version 2.2.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef ETHTSYN_INT01_LIB_H
#define ETHTSYN_INT01_LIB_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h> /* Specific standard types */
#include <ComStack_Types.h> /* Communication stack types */
#include <EthTSyn_Cfg.h> /* Module configuration */
#include <EthTSyn_Int.h>
#include <StbM.h> /* Module public API */
#include <EthIf.h> /* Module public API */


/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
#define ETHTSYN_START_SEC_CODE
#include <EthTSyn_MemMap.h>

#if((ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON) || (ETHTSYN_SWITCH_USED == STD_ON))
/**
 * \brief Service to add two virtual local times.
 *
 * \param[in/out] VirtualLocalTime1Ptr - Pointer to a virtual local time is the first term
 *                                       and returns the result of the addition.
 * \param[in] VirtualLocalTime2Ptr - Pointer to a virtual local time is the second term.
 *
 * \return E_OK if successful
 *         E_NOT_OK if overflow occurred in the result
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Int01_AddVirtualLocalTimes
(
  P2VAR(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) VirtualLocalTime1Ptr,
  P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) VirtualLocalTime2Ptr
);

/**
 * \brief Service convert Eth_TimeStampType into StbM_VirtualLocalTimeType
 *
 * \param[in] EthTimePtr - Pointer to Eth time to be converted.
 * \param[out] VirtualLocalTimePtr - Pointer which returns the converted virtual local time.

  * \return E_OK if successful
 *          E_NOT_OK if conversion result is not correct
 *
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Int01_ConvertEthTimeToVirtualTime
(
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) EthTimePtr,
  P2VAR(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) VirtualLocalTimePtr
);
#endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON || ETHTSYN_SWITCH_USED == STD_ON*/

/**
 * \brief Service to subtract virtual local times.
 *
 * This service subtracts two virtual local times and returns the result in the first time stamp
 * parameter if successful. In case that sign is negative the function will return E_NOT_OK and
 * parameters are not changed.
 *
 * Note: Overflow is not taken in account because the max value 2^64-1 will never be achieved.
 *
 * \param[in] VirtualLocalTime_MinuendPtr - Minuend pointer to a virtual local time for subtraction.
 * \param[in] VirtualLocalTime_SubtrahendPtr - Subtrahend pointer to a virtual local time for subtraction.
 * \param[in/out] VirtualLocalTime_ResultPtr - Result pointer provides a variable for the result of the
 *                                      subtraction.
 *
 * \return E_OK if TimeStamp1Ptr >= TimeStamp2Ptr
 *         E_NOT_OK if TimeStamp1Ptr < TimeStamp2Ptr
 */
TS_MOD_PRIV_DECL FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Int01_SubVirtualLocalTimes
(
  P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) VirtualLocalTime_MinuendPtr,
  P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) VirtualLocalTime_SubtrahendPtr,
  P2VAR(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) VirtualLocalTime_ResultPtr
);

/**
 * \brief Service to compare two virtual local times.
 *
 * This service compares two virtual local times to detect which one is bigger.
 *
 * \param[in] VirtualLocalTime1Ptr - First pointer to a virtual local time.
 * \param[in] VirtualLocalTime2Ptr - Second pointer to a virtual local time.
 *
 * \return TRUE if VirtualLocalTime1Ptr >= VirtualLocalTime2Ptr
 *         FALSE if VirtualLocalTime1Ptr < VirtualLocalTime2Ptr
 */

TS_MOD_PRIV_DECL FUNC(boolean, ETHTSYN_CODE) EthTSyn_Int01_VirtualLocalTime_IsGreaterOrEqual
(
  P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) VirtualLocalTime1Ptr,
  P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) VirtualLocalTime2Ptr
);

/**
 * \brief Service convert StbM_VirtualLocalTimeType into Eth_TimeStampType
 *
 * \param[in] VirtualLocalTimePtr - Pointer to virtual local time to be converted.
 * \param[out] EthTimePtr - Pointer which returns the converted Eth time.
 *
 */
TS_MOD_PRIV_DECL FUNC(void, ETHTSYN_CODE) EthTSyn_Int01_ConvertVirtualTimeToEthTime
(
  P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, ETHTSYN_APPL_DATA) VirtualLocalTimePtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) EthTimePtr
);

#define ETHTSYN_STOP_SEC_CODE
#include <EthTSyn_MemMap.h>

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef ETHTSYN_INT01_LIB_H */
/*==================[end of file]===========================================*/
