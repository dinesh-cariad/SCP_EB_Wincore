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

#ifndef ETHTSYN_INT_H
#define ETHTSYN_INT_H

/*==================[inclusions]============================================*/
#include <EthIf.h> /* Module public API */
#include <EthTSyn_Version.h> /* Module version declarations */

/*==================[macros]================================================*/

#if (defined ETHTSYN_NANOSEC_MAX)
#error ETHTSYN_NANOSEC_MAX is already defined
#endif
#define ETHTSYN_NANOSEC_MAX (uint32) 999999999U /* Maximum Pdelay shall be 10^9ns-1. */

#if (defined ETHTSYN_32BIT_MAX)
#error ETHTSYN_32BIT_MAX is already defined
#endif
#define ETHTSYN_32BIT_MAX (uint32) 0xFFFFFFFFU

#if (defined ETHTSYN_16BIT_MAX)
#error ETHTSYN_16BIT_MAX is already defined
#endif
#define ETHTSYN_16BIT_MAX (uint16) 0xFFFFU

#if (defined ETHTSYN_16BIT_SIGNED_MAX)
#error ETHTSYN_16BIT_SIGNED_MAX is already defined
#endif
#define ETHTSYN_16BIT_SIGNED_MAX (uint16) 0x7FFFU

#if (defined ETHTSYN_MAX_DROPPED_SYNCFUP_PAIRS)
#error ETHTSYN_MAX_DROPPED_SYNCFUP_PAIRS is already defined
#endif
#define ETHTSYN_MAX_DROPPED_SYNCFUP_PAIRS 12U

#if (defined ETHTSYN_MAX_DROPPED_PDELAYRESPPDELAYFUP_PAIRS)
#error ETHTSYN_MAX_DROPPED_PDELAYRESPPDELAYFUP_PAIRS is already defined
#endif
#define ETHTSYN_MAX_DROPPED_PDELAYRESPPDELAYFUP_PAIRS 6U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#define ETHTSYN_START_SEC_VAR_INIT_UNSPECIFIED
#include <EthTSyn_MemMap.h>

/** \brief Stores the number of mainfunction() calls */
extern VAR(uint32, ETHTSYN_VAR) EthTSyn_GeneralCounter;

#define ETHTSYN_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <EthTSyn_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#define ETHTSYN_START_SEC_CODE
#include <EthTSyn_MemMap.h>

/**
 * \brief Service to add two time stamps.
 *
 * This service adds two time stamps and returns the result in the first time stamp
 * parameter if successful. In case that addition causes an overflow all struct members of the
 * result are set to zero.
 *
 * \param[in/out] TimeStamp1Ptr - Pointer to a time stamp for addition. It will return the result
 *                                if service is successful or set all struct members to zero if
 *                                overflow occurs.
 * \param[in] TimeStamp2Ptr - Pointer to a time stamp for addition.
 *
 */
extern FUNC(void, ETHTSYN_CODE) EthTSyn_AddTimeStamps
(
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStamp1Ptr,
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) TimeStamp2Ptr
);

/**
 * \brief Service to process incoming SequenceId.
 *
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] DataPtr - Pointer to the payload of the frame.
 *
 * \return E_OK: Success
 *         E_NOT_OK: Sequence counter validation failed.
 */
extern FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_CheckSC
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr
);
#define ETHTSYN_STOP_SEC_CODE
#include <EthTSyn_MemMap.h>

/*==================[internal function definitions]=========================*/

#endif /* ifndef ETHTSYN_INT_H */
/*==================[end of file]===========================================*/
