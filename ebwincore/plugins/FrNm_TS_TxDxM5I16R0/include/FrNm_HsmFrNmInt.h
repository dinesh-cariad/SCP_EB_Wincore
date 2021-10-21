/**
 * \file
 *
 * \brief AUTOSAR FrNm
 *
 * This file contains the implementation of the AUTOSAR
 * module FrNm.
 *
 * \version 5.16.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef FRNM_HSMFRNMINT_H
#define FRNM_HSMFRNMINT_H

/* Internal interface for the FrNm state
 * machine.
 *
 * This header must only be included by
 * FrNm_HsmFrNmData.c and FrNm_HsmFrNmFnct.c.
 */

/* CHECK: RULE 301 OFF (this file is generated, some lines may be longer then
 * 100 characters) */

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 5.1 (required)
 * External identifiers shall be distinct.
 *
 * Reason:
 * The last characters of external identifiers are significant characters. Better and faster
 * understanding of functions names.
 *
 * MISRAC2012-2) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be enclosed in parentheses.
 *
 * Reason:
 * The macro is used in function parameter declarations and definitions or as structure member
 * where the number of parentheses matter.
 */

/*==================[inclusions]============================================*/

/* include state machine driver interface */
#include <FrNm_Hsm.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/* *** State indices and IDs for states *** */
typedef enum
{
  FRNM_HSM_FRNM_SIDX_TOP,
  FRNM_HSM_FRNM_SIDX_BUSSLEEPMODE,
  FRNM_HSM_FRNM_SIDX_NETWORKMODE,
  FRNM_HSM_FRNM_SIDX_READYSLEEPSTATE,
  FRNM_HSM_FRNM_SIDX_READYSLEEPREMOTEACTIVITY,
  FRNM_HSM_FRNM_SIDX_READYSLEEPREMOTESLEEP,
  FRNM_HSM_FRNM_SIDX_SENDINGSUBMODE,
  FRNM_HSM_FRNM_SIDX_NORMALOPERATIONSTATE,
  FRNM_HSM_FRNM_SIDX_NORMALOPERATIONREMOTEACTIVITY,
  FRNM_HSM_FRNM_SIDX_NORMALOPERATIONREMOTESLEEP,
  FRNM_HSM_FRNM_SIDX_REPEATMESSAGESTATE,
  FRNM_HSM_FRNM_SIDX_SYNCHRONIZEMODE,
  FRNM_HSM_FRNM_NO_OF_STATES
} FrNm_HsmFrNmStateIdType;
/* typedefed type only used for debugging */

/* *** IDs for entry/axit and transition actions *** */
typedef enum
{
  FRNM_HSM_FRNM_AIDX_TOP_ENTRY,
  FRNM_HSM_FRNM_AIDX_TOP_ACTION_1,
  FRNM_HSM_FRNM_AIDX_BUSSLEEPMODE_ENTRY,
  FRNM_HSM_FRNM_AIDX_BUSSLEEPMODE_ACTION_1,
  FRNM_HSM_FRNM_AIDX_BUSSLEEPMODE_ACTION_2,
  FRNM_HSM_FRNM_AIDX_NETWORKMODE_ENTRY,
  FRNM_HSM_FRNM_AIDX_NETWORKMODE_ACTION_1,
  FRNM_HSM_FRNM_AIDX_NETWORKMODE_ACTION_2,
  FRNM_HSM_FRNM_AIDX_NETWORKMODE_ACTION_3,
  FRNM_HSM_FRNM_AIDX_READYSLEEPSTATE_ENTRY,
  FRNM_HSM_FRNM_AIDX_READYSLEEPSTATE_EXIT,
  FRNM_HSM_FRNM_AIDX_READYSLEEPSTATE_ACTION_1,
  FRNM_HSM_FRNM_AIDX_READYSLEEPREMOTEACTIVITY_ENTRY,
  FRNM_HSM_FRNM_AIDX_READYSLEEPREMOTEACTIVITY_ACTION_1,
  FRNM_HSM_FRNM_AIDX_READYSLEEPREMOTESLEEP_ENTRY,
  FRNM_HSM_FRNM_AIDX_READYSLEEPREMOTESLEEP_ACTION_1,
  FRNM_HSM_FRNM_AIDX_READYSLEEPREMOTESLEEP_ACTION_3,
  FRNM_HSM_FRNM_AIDX_SENDINGSUBMODE_ENTRY,
  FRNM_HSM_FRNM_AIDX_SENDINGSUBMODE_EXIT,
  FRNM_HSM_FRNM_AIDX_SENDINGSUBMODE_ACTION_1,
  FRNM_HSM_FRNM_AIDX_SENDINGSUBMODE_ACTION_2,
  FRNM_HSM_FRNM_AIDX_SENDINGSUBMODE_ACTION_3,
  FRNM_HSM_FRNM_AIDX_SENDINGSUBMODE_ACTION_4,
  FRNM_HSM_FRNM_AIDX_NORMALOPERATIONSTATE_ENTRY,
  FRNM_HSM_FRNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ENTRY,
  FRNM_HSM_FRNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ACTION_1,
  FRNM_HSM_FRNM_AIDX_NORMALOPERATIONREMOTESLEEP_ENTRY,
  FRNM_HSM_FRNM_AIDX_NORMALOPERATIONREMOTESLEEP_ACTION_1,
  FRNM_HSM_FRNM_AIDX_NORMALOPERATIONREMOTESLEEP_ACTION_3,
  FRNM_HSM_FRNM_AIDX_REPEATMESSAGESTATE_ENTRY,
  FRNM_HSM_FRNM_AIDX_REPEATMESSAGESTATE_EXIT,
  FRNM_HSM_FRNM_AIDX_REPEATMESSAGESTATE_ACTION_1,
  FRNM_HSM_FRNM_AIDX_SYNCHRONIZEMODE_ENTRY,
  FRNM_HSM_FRNM_AIDX_SYNCHRONIZEMODE_ACTION_1,
  FRNM_HSM_FRNM_NO_OF_ACTIONS
} FrNm_HsmFrNmActionIdType;
/* typedefed type only used for debugging */

/* *** IDs for guards *** */
typedef enum
{
  FRNM_HSM_FRNM_GIDX_READYSLEEPSTATE_GUARD_1,
  FRNM_HSM_FRNM_GIDX_READYSLEEPREMOTEACTIVITY_GUARD_2,
  FRNM_HSM_FRNM_GIDX_READYSLEEPREMOTEACTIVITY_GUARD_3,
  FRNM_HSM_FRNM_GIDX_READYSLEEPREMOTESLEEP_GUARD_1,
  FRNM_HSM_FRNM_GIDX_READYSLEEPREMOTESLEEP_GUARD_2,
  FRNM_HSM_FRNM_GIDX_NORMALOPERATIONREMOTEACTIVITY_GUARD_2,
  FRNM_HSM_FRNM_GIDX_NORMALOPERATIONREMOTEACTIVITY_GUARD_3,
  FRNM_HSM_FRNM_GIDX_NORMALOPERATIONREMOTEACTIVITY_GUARD_4,
  FRNM_HSM_FRNM_GIDX_NORMALOPERATIONREMOTESLEEP_GUARD_1,
  FRNM_HSM_FRNM_GIDX_NORMALOPERATIONREMOTESLEEP_GUARD_2,
  FRNM_HSM_FRNM_GIDX_REPEATMESSAGESTATE_GUARD_2,
  FRNM_HSM_FRNM_GIDX_REPEATMESSAGESTATE_GUARD_3,
  FRNM_HSM_FRNM_GIDX_SYNCHRONIZEMODE_GUARD_2,
  FRNM_HSM_FRNM_GIDX_SYNCHRONIZEMODE_GUARD_3,
  FRNM_HSM_FRNM_NO_OF_GUARDS
} FrNm_HsmFrNmGuardIdType;
/* typedefed type only used for debugging */

/*==================[external function declarations]========================*/

#define FRNM_START_SEC_CODE
#include <FrNm_MemMap.h>

/* function declarations of state entry, exit, guard, action functions
 * defined in FrNm_fnct.c */

/* Deviation MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfTOPEntry(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements action '#if (FRNM_PDU_RX_INDIATION_ENABLED == STD_ON) Nm_PduRxIndication(); #endif =>FRNM189 FRNM190' */
/* Deviation MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfTOPAction1(
  FRNM_PDL_SF(const uint8 instIdx));
/* Deviation MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfBusSleepModeEntry(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements action 'Nm_NetworkStartIndication(); => FRNM175' */
/* Deviation MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfBusSleepModeAction1(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements action 'if(ChanStatus & NETWORK_REQUESTED) SetActiveWakeUpFlag = true' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfBusSleepModeAction2(
  FRNM_PDL_SF(const uint8 instIdx));
/* Deviation MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfNetworkModeEntry(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements action 'FrNm_HandleSynchError();' */
/* Deviation MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfNetworkModeAction1(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements action 'InitInternalVar();' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfNetworkModeAction2(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements action 'ClearActiveWakeupBit()' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfNetworkModeAction3(
  FRNM_PDL_SF(const uint8 instIdx));
/* Deviation MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfReadySleepStateEntry(
  FRNM_PDL_SF(const uint8 instIdx));
/* Deviation MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfReadySleepStateExit(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements guard 'ReadySleepTimer == 0' */
/* Deviation MISRAC2012-2 */
extern FUNC(boolean, FRNM_CODE)
 FrNm_HsmFrNmSfReadySleepStateGuard1(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements action 'ClearActiveWakeupBit()' */
/* Deviation MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfReadySleepStateAction1(
  FRNM_PDL_SF(const uint8 instIdx));
/* Deviation MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfReadySleepRemoteActivityEntry(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements action 'ReadySleepTimerStart(READY_SLEEP_COUNT);' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfReadySleepRemoteActivityAction1(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements guard 'IsTransitionRmsAllowed() == true' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, FRNM_CODE)
 FrNm_HsmFrNmSfReadySleepRemoteActivityGuard2(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements guard 'IsTransitionNoOpAllowed() == true' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, FRNM_CODE)
 FrNm_HsmFrNmSfReadySleepRemoteActivityGuard3(
  FRNM_PDL_SF(const uint8 instIdx));
/* Deviation MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfReadySleepRemoteSleepEntry(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements guard 'IsTransitionRmsAllowed() == true' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, FRNM_CODE)
 FrNm_HsmFrNmSfReadySleepRemoteSleepGuard1(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements action 'Nm_RemoteSleepCancellation();' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfReadySleepRemoteSleepAction1(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements guard 'IsTransitionNoOpAllowed() == true' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, FRNM_CODE)
 FrNm_HsmFrNmSfReadySleepRemoteSleepGuard2(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements action 'Nm_RemoteSleepCancellation(); ChanStatus &= ~FRNM_REMOTE_SLEEP_INDICATION;' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfReadySleepRemoteSleepAction3(
  FRNM_PDL_SF(const uint8 instIdx));
/* Deviation MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfSendingSubModeEntry(
  FRNM_PDL_SF(const uint8 instIdx));
/* Deviation MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfSendingSubModeExit(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements action '#if ((PASSIVE_MODE == STD_OFF)) if ((ChanStatus == COM_ENABLED) && (ChanProperty == NM_DATA_ENABLED)) {HandleDataCycle();}#endif' */
/* Deviation MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfSendingSubModeAction1(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements action '#if ((PASSIVE_MODE == STD_OFF)) if ((ChanStatus == FRNM_COM_ENABLED) {HandleVoteCycle();}#endif' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfSendingSubModeAction2(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements action 'Restart TimeoutTimer;' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfSendingSubModeAction3(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements action 'Nm_TxTimeoutException();' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfSendingSubModeAction4(
  FRNM_PDL_SF(const uint8 instIdx));
/* Deviation MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfNormalOperationStateEntry(
  FRNM_PDL_SF(const uint8 instIdx));
/* Deviation MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfNormalOperationRemoteActivityEntry(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements action '#if (RSI == STD_ON) UniTimerStart(FRNM_REMOTE_SLEEP_IND_TIME); #endif' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfNormalOperationRemoteActivityAction1(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements guard 'ChanStatus & REPEAT_MESSAGE_REQUESTED' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, FRNM_CODE)
 FrNm_HsmFrNmSfNormalOperationRemoteActivityGuard2(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements guard '(ChanStatus.UniversalTimer == 0) && ChanConfig.RemoteSleepIndTime != 0)' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, FRNM_CODE)
 FrNm_HsmFrNmSfNormalOperationRemoteActivityGuard3(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements guard '(! ChanStatus & NETWORK_REQUESTED) && (! ChanStatus & REPEAT_MSG_REQUESTED)' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, FRNM_CODE)
 FrNm_HsmFrNmSfNormalOperationRemoteActivityGuard4(
  FRNM_PDL_SF(const uint8 instIdx));
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfNormalOperationRemoteSleepEntry(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements guard 'ChanStatus & REPEAT_MSG_REQUESTED' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, FRNM_CODE)
 FrNm_HsmFrNmSfNormalOperationRemoteSleepGuard1(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements action 'Nm_RemoteSleepCancellation(); => FRNM230' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfNormalOperationRemoteSleepAction1(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements guard '(! ChanStatus & NETWORK_REQUESTED) && (! ChanStatus & REPEAT_MSG_REQUESTED)' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, FRNM_CODE)
 FrNm_HsmFrNmSfNormalOperationRemoteSleepGuard2(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements action 'Uni timer (RSI) is started by entry action' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfNormalOperationRemoteSleepAction3(
  FRNM_PDL_SF(const uint8 instIdx));
/* Deviation MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfRepeatMessageStateEntry(
  FRNM_PDL_SF(const uint8 instIdx));
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfRepeatMessageStateExit(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements action 'ChanStatus |= UNI_TIMEOUT_PASSED;' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfRepeatMessageStateAction1(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements guard '(ChanStatus & NETWORK_REQUESTED) && (ChanStatus & UNI_TIMEOUT_PASSED)' */
/* Deviation MISRAC2012-1, MISRAC2012-2*/
extern FUNC(boolean, FRNM_CODE)
 FrNm_HsmFrNmSfRepeatMessageStateGuard2(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements guard '( !(ChanStatus & NETWORK_REQUESTED) && (ChanStatus & UNI_TIMEOUT_PASSED) )' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, FRNM_CODE)
 FrNm_HsmFrNmSfRepeatMessageStateGuard3(
  FRNM_PDL_SF(const uint8 instIdx));
/* Deviation MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfSynchronizeModeEntry(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements action 'InitInternalVar();' */
/* Deviation MISRAC2012-2 */
extern FUNC(void, FRNM_CODE)
 FrNm_HsmFrNmSfSynchronizeModeAction1(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements guard '(FrNm_SyncLoss == false)' */
/* Deviation MISRAC2012-2 */
extern FUNC(boolean, FRNM_CODE)
 FrNm_HsmFrNmSfSynchronizeModeGuard2(
  FRNM_PDL_SF(const uint8 instIdx));
/* implements guard '! (ChanStatus & NETWORK_REQUESTED)' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, FRNM_CODE)
 FrNm_HsmFrNmSfSynchronizeModeGuard3(
  FRNM_PDL_SF(const uint8 instIdx));


#define FRNM_STOP_SEC_CODE
#include <FrNm_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* defined FRNM_HSMFRNMINT_H */
/*==================[end of file]===========================================*/
