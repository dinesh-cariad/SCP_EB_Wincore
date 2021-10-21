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

#ifndef COMM_HSMCOMMPNCINT_H
#define COMM_HSMCOMMPNCINT_H

/* Internal interface for the ComMPnc state
 * machine.
 *
 * This header must only be included by
 * ComM_HsmComMPncData.c and ComM_HsmComMPncFnct.c.
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
 * The macro is used in function parameter declarations and definitions of structure member where
 * the number of parentheses matter.
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
  COMM_HSM_COMMPNC_SIDX_PNCTOP,
  COMM_HSM_COMMPNC_SIDX_PNCFULLCOM,
  COMM_HSM_COMMPNC_SIDX_PNCPREPARESLEEP,
  COMM_HSM_COMMPNC_SIDX_PNCREADYSLEEP,
  COMM_HSM_COMMPNC_SIDX_PNCREQUESTED,
  COMM_HSM_COMMPNC_SIDX_PNCNOCOM,
  COMM_HSM_COMMPNC_SIDX_PNCNOCOMINIT,
  COMM_HSM_COMMPNC_SIDX_PNCNOCOMNOTIFY,
  COMM_HSM_COMMPNC_NO_OF_STATES
} ComM_HsmComMPncStateIdType;
/* typedefed type only used for debugging */

/* *** IDs for entry/do/exit and transition actions *** */
typedef enum
{
  COMM_HSM_COMMPNC_AIDX_PNCTOP_ACTION_1,
  COMM_HSM_COMMPNC_AIDX_PNCFULLCOM_ENTRY,
  COMM_HSM_COMMPNC_AIDX_PNCPREPARESLEEP_ENTRY,
  COMM_HSM_COMMPNC_AIDX_PNCPREPARESLEEP_ACTION_1,
  COMM_HSM_COMMPNC_AIDX_PNCPREPARESLEEP_ACTION_2,
  COMM_HSM_COMMPNC_AIDX_PNCPREPARESLEEP_ACTION_3,
  COMM_HSM_COMMPNC_AIDX_PNCREADYSLEEP_ENTRY,
  COMM_HSM_COMMPNC_AIDX_PNCREQUESTED_ENTRY,
  COMM_HSM_COMMPNC_AIDX_PNCREQUESTED_ACTION_1,
  COMM_HSM_COMMPNC_AIDX_PNCNOCOM_ENTRY,
  COMM_HSM_COMMPNC_AIDX_PNCNOCOM_ACTION_1,
  COMM_HSM_COMMPNC_AIDX_PNCNOCOM_ACTION_3,
  COMM_HSM_COMMPNC_AIDX_PNCNOCOM_ACTION_4,
  COMM_HSM_COMMPNC_AIDX_PNCNOCOMNOTIFY_ENTRY,
  COMM_HSM_COMMPNC_NO_OF_ACTIONS
} ComM_HsmComMPncActionIdType;
/* typedefed type only used for debugging */

/* *** IDs for guards *** */
typedef enum
{
  COMM_HSM_COMMPNC_GIDX_PNCPREPARESLEEP_GUARD_1,
  COMM_HSM_COMMPNC_GIDX_PNCREADYSLEEP_GUARD_1,
  COMM_HSM_COMMPNC_GIDX_PNCREQUESTED_GUARD_1,
  COMM_HSM_COMMPNC_GIDX_PNCNOCOM_GUARD_1,
  COMM_HSM_COMMPNC_GIDX_PNCNOCOM_GUARD_3,
  COMM_HSM_COMMPNC_NO_OF_GUARDS
} ComM_HsmComMPncGuardIdType;
/* typedefed type only used for debugging */

/*==================[external function declarations]========================*/

#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>

/* function declarations of state entry, exit, guard, action functions
 * defined in ComMPnc_fnct.c */

/* implements action 'UpdateERAPassive();' */
/* Deviation MISRAC2012-1 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMPncSfPncTopAction1(
  COMM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMPncSfPncFullComEntry(
  COMM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMPncSfPncPrepareSleepEntry(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard 'ComM_PncGetEIRA() == 1' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMPncSfPncPrepareSleepGuard1(
  COMM_PDL_SF(const uint8 instIdx));

/* implements action 'ComM_PncStopTimer(PNC);' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMPncSfPncPrepareSleepAction1(
  COMM_PDL_SF(const uint8 instIdx));

/* implements action 'ComM_PncStopTimer(PNC);' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMPncSfPncPrepareSleepAction2(
  COMM_PDL_SF(const uint8 instIdx));

/* implements action 'if(COMM_NO_INTERNAL_AND_EXTERNAL_REQUESTS) ComM_PNC_RequestComMode (COMM_NO_COMMUNICATION);' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMPncSfPncPrepareSleepAction3(
  COMM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMPncSfPncReadySleepEntry(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard 'ComM_PncGetEIRA() == 0' */
/* Deviation MISRAC2012-1 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMPncSfPncReadySleepGuard1(
  COMM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMPncSfPncRequestedEntry(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard 'ComM_PncGetERAUnfiltered() == 0 && NO_COM' */
/* Deviation MISRAC2012-1 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMPncSfPncRequestedGuard1(
  COMM_PDL_SF(const uint8 instIdx));

/* implements action 'ComM_TxEiraEraActive(PNC_ID) = 0; ComM_PncSendSignal(PNC); if(COMM_NO_INTERNAL_REQUESTS) ComM_PNC_RequestComMode (COMM_NO_COMMUNICATION);' */
/* Deviation MISRAC2012-1 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMPncSfPncRequestedAction1(
  COMM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMPncSfPncNoComEntry(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard 'ComM_PncGetEIRA() == 1' */
/* Deviation MISRAC2012-1 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMPncSfPncNoComGuard1(
  COMM_PDL_SF(const uint8 instIdx));

/* implements action 'BswM_ComM_CurrentPNCMode(PNC_ID, PNC_FULL_COMMUNICATION);' */
/* Deviation MISRAC2012-1 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMPncSfPncNoComAction1(
  COMM_PDL_SF(const uint8 instIdx));

/* implements guard 'ComMSynchronousWakeUp == TRUE' */
/* Deviation MISRAC2012-1 */
extern FUNC(boolean, COMM_CODE)
 ComM_HsmComMPncSfPncNoComGuard3(
  COMM_PDL_SF(const uint8 instIdx));

/* implements action 'BswM_ComM_CurrentPNCMode(PNC_ID, PNC_FULL_COMMUNICATION);' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMPncSfPncNoComAction3(
  COMM_PDL_SF(const uint8 instIdx));

/* implements action 'BswM_ComM_CurrentPNCMode(PNC_ID, PNC_FULL_COMMUNICATION);' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMPncSfPncNoComAction4(
  COMM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, COMM_CODE)
 ComM_HsmComMPncSfPncNoComNotifyEntry(
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

#endif /* defined COMM_HSMCOMMPNCINT_H */
/*==================[end of file]===========================================*/
