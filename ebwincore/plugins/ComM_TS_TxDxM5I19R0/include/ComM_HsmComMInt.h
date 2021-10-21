/**
 * \file
 *
 * \brief AUTOSAR ComM
 *
 * This file contains the implementation of the AUTOSAR
 * module ComM.
 *
 * \version 5.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef COMM_HSMCOMMINT_H
#define COMM_HSMCOMMINT_H

/* Internal interface for the ComM state
 * machine.
 *
 * This header must only be included by
 * ComM_HsmComMData.c and ComM_HsmComMFnct.c.
 */

/* CHECK: RULE 301 OFF (this file is generated, some lines may be longer then
 * 100 characters) */

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be enclosed in parentheses.
 *
 * Reason:
 * The macro is used in function parameter declarations and definitions where the number of
 * parentheses matter.
 *
 * MISRAC2012-2) Deviated Rule: 5.1 (required)
 * External identifiers shall be distinct.
 *
 * Reason:
 * The last characters of external identifiers could be significant characters.
 */

/*==================[inclusions]============================================*/

/* include state machine driver interface */
#include <ComM_Hsm.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/* *** State indices and IDs for states *** */
typedef enum
{
  COMM_HSM_COMM_SIDX_TOP,
  COMM_HSM_COMM_SIDX_FULL_COMMUNICATION,
  COMM_HSM_COMM_SIDX_FULL_COM_NETWORK_REQUESTED,
  COMM_HSM_COMM_SIDX_FULL_COM_NETWORK_REQUESTED_ENTRY,
  COMM_HSM_COMM_SIDX_FULL_COM_NETWORK_REQUESTED_PENDING,
  COMM_HSM_COMM_SIDX_FULL_COM_READY_SLEEP,
  COMM_HSM_COMM_SIDX_NO_COMMUNICATION,
  COMM_HSM_COMM_SIDX_NO_COM_NO_PENDING_REQUEST,
  COMM_HSM_COMM_SIDX_NO_COM_REQUEST_PENDING,
  COMM_HSM_COMM_SIDX_SILENT_COMMUNICATION,
  COMM_HSM_COMM_NO_OF_STATES
} ComM_HsmComMStateIdType;
/* typedefed type only used for debugging */

/* *** IDs for entry/do/exit and transition actions *** */
typedef enum
{
  COMM_HSM_COMM_AIDX_TOP_ACTION_1,
  COMM_HSM_COMM_AIDX_FULL_COMMUNICATION_ENTRY,
  COMM_HSM_COMM_AIDX_FULL_COM_NETWORK_REQUESTED_ENTRY,
  COMM_HSM_COMM_AIDX_FULL_COM_NETWORK_REQUESTED_ACTION_1,
  COMM_HSM_COMM_AIDX_FULL_COM_NETWORK_REQUESTED_ENTRY_ENTRY,
  COMM_HSM_COMM_AIDX_FULL_COM_NETWORK_REQUESTED_ENTRY_EXIT,
  COMM_HSM_COMM_AIDX_FULL_COM_READY_SLEEP_ENTRY,
  COMM_HSM_COMM_AIDX_FULL_COM_READY_SLEEP_EXIT,
  COMM_HSM_COMM_AIDX_FULL_COM_READY_SLEEP_ACTION_3,
  COMM_HSM_COMM_AIDX_NO_COMMUNICATION_ENTRY,
  COMM_HSM_COMM_AIDX_NO_COM_NO_PENDING_REQUEST_ENTRY,
  COMM_HSM_COMM_AIDX_NO_COM_NO_PENDING_REQUEST_ACTION_1,
  COMM_HSM_COMM_AIDX_NO_COM_REQUEST_PENDING_ENTRY,
  COMM_HSM_COMM_AIDX_NO_COM_REQUEST_PENDING_ACTION_2,
  COMM_HSM_COMM_AIDX_NO_COM_REQUEST_PENDING_ACTION_3,
  COMM_HSM_COMM_AIDX_SILENT_COMMUNICATION_ENTRY,
  COMM_HSM_COMM_NO_OF_ACTIONS
} ComM_HsmComMActionIdType;
/* typedefed type only used for debugging */

/* *** IDs for guards *** */
typedef enum
{
  COMM_HSM_COMM_GIDX_FULL_COMMUNICATION_GUARD_2,
  COMM_HSM_COMM_GIDX_FULL_COM_NETWORK_REQUESTED_GUARD_2,
  COMM_HSM_COMM_GIDX_FULL_COM_NETWORK_REQUESTED_GUARD_3,
  COMM_HSM_COMM_GIDX_FULL_COM_NETWORK_REQUESTED_ENTRY_GUARD_1,
  COMM_HSM_COMM_GIDX_FULL_COM_NETWORK_REQUESTED_ENTRY_GUARD_2,
  COMM_HSM_COMM_GIDX_FULL_COM_NETWORK_REQUESTED_ENTRY_GUARD_3,
  COMM_HSM_COMM_GIDX_FULL_COM_NETWORK_REQUESTED_ENTRY_GUARD_4,
  COMM_HSM_COMM_GIDX_FULL_COM_NETWORK_REQUESTED_PENDING_GUARD_1,
  COMM_HSM_COMM_GIDX_FULL_COM_READY_SLEEP_GUARD_1,
  COMM_HSM_COMM_GIDX_FULL_COM_READY_SLEEP_GUARD_2,
  COMM_HSM_COMM_GIDX_NO_COM_NO_PENDING_REQUEST_GUARD_2,
  COMM_HSM_COMM_GIDX_NO_COM_REQUEST_PENDING_GUARD_1,
  COMM_HSM_COMM_GIDX_NO_COM_REQUEST_PENDING_GUARD_2,
  COMM_HSM_COMM_GIDX_NO_COM_REQUEST_PENDING_GUARD_4,
  COMM_HSM_COMM_GIDX_NO_COM_REQUEST_PENDING_GUARD_5,
  COMM_HSM_COMM_GIDX_SILENT_COMMUNICATION_GUARD_1,
  COMM_HSM_COMM_GIDX_SILENT_COMMUNICATION_GUARD_2,
  COMM_HSM_COMM_GIDX_SILENT_COMMUNICATION_GUARD_3,
  COMM_HSM_COMM_NO_OF_GUARDS
} ComM_HsmComMGuardIdType;
/* typedefed type only used for debugging */

/*==================[external function declarations]========================*/

#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>

/* function declarations of state entry, exit, guard, action functions
 * defined in ComM_fnct.c */

/* implements action 'ComM_Notify(ComM_ChannelMode); ComM_ClearUserRequest();' */
/* Deviation MISRAC2012-1 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMSfTOPAction1(
  COMM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMSfFULL_COMMUNICATIONEntry(
  COMM_PDL_SF(const uint8 instIdx));
/* implements guard '(ComM_NmGetMode() == NM_MODE_BUS_SLEEP)' */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMSfFULL_COMMUNICATIONGuard2(
  COMM_PDL_SF(const uint8 instIdx));

extern FUNC(void, COMM_CODE)
 ComM_HsmComMSfFULL_COM_NETWORK_REQUESTEDEntry(
  COMM_PDL_SF(const uint8 instIdx));

/* implements action 'Nm_NetworkRequest();' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMSfFULL_COM_NETWORK_REQUESTEDAction1(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard '!ComM_IsFullComAllowed()' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMSfFULL_COM_NETWORK_REQUESTEDGuard2(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard '(ComMNmVariant == PASSIVE) && (ComM_NmGetMode() == NM_MODE_PREPARE_BUS_SLEEP)' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMSfFULL_COM_NETWORK_REQUESTEDGuard3(
  COMM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMSfFULL_COM_NETWORK_REQUESTED_ENTRYEntry(
  COMM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMSfFULL_COM_NETWORK_REQUESTED_ENTRYExit(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard 'ComM_IsFullComAllowed()' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMSfFULL_COM_NETWORK_REQUESTED_ENTRYGuard1(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard '(FULL|PASSIVE|SLAVE_ACTIVE|SLAVE_PASSIVE == ComMNmVariant) && (!ComM_IsFullComAllowed())' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMSfFULL_COM_NETWORK_REQUESTED_ENTRYGuard2(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard 'ComM_IsFullComAllowed()' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMSfFULL_COM_NETWORK_REQUESTED_ENTRYGuard3(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard 'ComM_IsFullComAllowed()' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMSfFULL_COM_NETWORK_REQUESTED_ENTRYGuard4(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard '!ComM_IsFullComAllowed()' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMSfFULL_COM_NETWORK_REQUESTED_PENDINGGuard1(
  COMM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMSfFULL_COM_READY_SLEEPEntry(
  COMM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMSfFULL_COM_READY_SLEEPExit(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard '(ComMNmVariant == FULL|PASSIVE) && (ComM_NmGetMode() == NM_MODE_PREPARE_BUS_SLEEP)' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMSfFULL_COM_READY_SLEEPGuard1(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard 'ComM_IsFullComAllowed()' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMSfFULL_COM_READY_SLEEPGuard2(
  COMM_PDL_SF(const uint8 instIdx));

/* implements action 'if(COMM_BUS_TYPE_INTERNAL== ComMBusType) {ComM_Notify(COMM_NO_COMMUNICATION); ComM_ClearUserRequest();}' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMSfFULL_COM_READY_SLEEPAction3(
  COMM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMSfNO_COMMUNICATIONEntry(
  COMM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMSfNO_COM_NO_PENDING_REQUESTEntry(
  COMM_PDL_SF(const uint8 instIdx));

/* implements action 'ComM_ChannelPassiveWakeup = TRUE' */
/* Deviation MISRAC2012-1 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMSfNO_COM_NO_PENDING_REQUESTAction1(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard 'ComM_IsWakeupAllowed()' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMSfNO_COM_NO_PENDING_REQUESTGuard2(
  COMM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMSfNO_COM_REQUEST_PENDINGEntry(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard 'ComM_Communication_Allowed == TRUE' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMSfNO_COM_REQUEST_PENDINGGuard1(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard 'ComM_IsLimitToNoCom' */
/* Deviation MISRAC2012-1 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMSfNO_COM_REQUEST_PENDINGGuard2(
  COMM_PDL_SF(const uint8 instIdx));

/* implements action 'ComM_ClearUserRequest' */
/* Deviation MISRAC2012-1 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMSfNO_COM_REQUEST_PENDINGAction2(
  COMM_PDL_SF(const uint8 instIdx));

/* implements action 'ComM_ChannelPassiveWakeup = TRUE' */
/* Deviation MISRAC2012-1 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMSfNO_COM_REQUEST_PENDINGAction3(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard 'ComM_IsNotRequested' */
/* Deviation MISRAC2012-1 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMSfNO_COM_REQUEST_PENDINGGuard4(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard 'ComM_IsWakeupInhibition' */
/* Deviation MISRAC2012-1 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMSfNO_COM_REQUEST_PENDINGGuard5(
  COMM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMSfSILENT_COMMUNICATIONEntry(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard 'ComM_NmGetMode() == NM_MODE_NETWORK' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMSfSILENT_COMMUNICATIONGuard1(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard 'ComM_NmGetMode() == NM_MODE_BUS_SLEEP' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMSfSILENT_COMMUNICATIONGuard2(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard 'ComM_IsWakeupAllowed()' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMSfSILENT_COMMUNICATIONGuard3(
  COMM_PDL_SF(const uint8 instIdx));


#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* defined COMM_HSMCOMMINT_H */
/*==================[end of file]===========================================*/
