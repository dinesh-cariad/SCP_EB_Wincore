/**
 * \file
 *
 * \brief AUTOSAR CanNm
 *
 * This file contains the implementation of the AUTOSAR
 * module CanNm.
 *
 * \version 6.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef CANNM_HSMCANNMINT_H
#define CANNM_HSMCANNMINT_H

/* Internal interface for the CanNm state
 * machine.
 *
 * This header must only be included by
 * CanNm_HsmCanNmData.c and CanNm_HsmCanNmFnct.c.
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
 * External identifiers shall be distinct
 *
 * Reason:
 * The last characters of external identifiers could be significant characters.
 */

/*==================[inclusions]============================================*/

/* include state machine driver interface */
#include <CanNm_Hsm.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/* *** State indices and IDs for states *** */
typedef enum
{
  CANNM_HSM_CANNM_SIDX_TOP,
  CANNM_HSM_CANNM_SIDX_BUSSLEEPMODE,
  CANNM_HSM_CANNM_SIDX_NETWORKMODE,
  CANNM_HSM_CANNM_SIDX_READYSLEEPSTATE,
  CANNM_HSM_CANNM_SIDX_READYSLEEPREMOTEACTIVITY,
  CANNM_HSM_CANNM_SIDX_READYSLEEPREMOTESLEEP,
  CANNM_HSM_CANNM_SIDX_SENDINGSUBMODE,
  CANNM_HSM_CANNM_SIDX_NORMALOPERATIONSTATE,
  CANNM_HSM_CANNM_SIDX_NORMALOPERATIONREMOTEACTIVITY,
  CANNM_HSM_CANNM_SIDX_NORMALOPERATIONREMOTESLEEP,
  CANNM_HSM_CANNM_SIDX_REPEATMESSAGESTATE,
  CANNM_HSM_CANNM_SIDX_PREPAREBUSSLEEPMODE,
  CANNM_HSM_CANNM_NO_OF_STATES
} CanNm_HsmCanNmStateIdType;
/* typedefed type only used for debugging */

/* *** IDs for entry/axit and transition actions *** */
typedef enum
{
  CANNM_HSM_CANNM_AIDX_TOP_ENTRY,
  CANNM_HSM_CANNM_AIDX_BUSSLEEPMODE_ENTRY,
  CANNM_HSM_CANNM_AIDX_BUSSLEEPMODE_ACTION_1,
  CANNM_HSM_CANNM_AIDX_NETWORKMODE_ENTRY,
  CANNM_HSM_CANNM_AIDX_NETWORKMODE_ACTION_1,
  CANNM_HSM_CANNM_AIDX_NETWORKMODE_ACTION_2,
  CANNM_HSM_CANNM_AIDX_NETWORKMODE_ACTION_3,
  CANNM_HSM_CANNM_AIDX_READYSLEEPSTATE_ENTRY,
  CANNM_HSM_CANNM_AIDX_READYSLEEPREMOTEACTIVITY_ENTRY,
  CANNM_HSM_CANNM_AIDX_READYSLEEPREMOTEACTIVITY_ACTION_1,
  CANNM_HSM_CANNM_AIDX_READYSLEEPREMOTESLEEP_ACTION_1,
  CANNM_HSM_CANNM_AIDX_READYSLEEPREMOTESLEEP_ACTION_2,
  CANNM_HSM_CANNM_AIDX_READYSLEEPREMOTESLEEP_ACTION_3,
  CANNM_HSM_CANNM_AIDX_SENDINGSUBMODE_EXIT,
  CANNM_HSM_CANNM_AIDX_SENDINGSUBMODE_ACTION_1,
  CANNM_HSM_CANNM_AIDX_SENDINGSUBMODE_ACTION_2,
  CANNM_HSM_CANNM_AIDX_SENDINGSUBMODE_ACTION_3,
  CANNM_HSM_CANNM_AIDX_NORMALOPERATIONSTATE_ENTRY,
  CANNM_HSM_CANNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ENTRY,
  CANNM_HSM_CANNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ACTION_1,
  CANNM_HSM_CANNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ACTION_4,
  CANNM_HSM_CANNM_AIDX_NORMALOPERATIONREMOTESLEEP_ENTRY,
  CANNM_HSM_CANNM_AIDX_NORMALOPERATIONREMOTESLEEP_ACTION_2,
  CANNM_HSM_CANNM_AIDX_NORMALOPERATIONREMOTESLEEP_ACTION_3,
  CANNM_HSM_CANNM_AIDX_REPEATMESSAGESTATE_ENTRY,
  CANNM_HSM_CANNM_AIDX_REPEATMESSAGESTATE_EXIT,
  CANNM_HSM_CANNM_AIDX_PREPAREBUSSLEEPMODE_ENTRY,
  CANNM_HSM_CANNM_AIDX_PREPAREBUSSLEEPMODE_ACTION_3,
  CANNM_HSM_CANNM_NO_OF_ACTIONS
} CanNm_HsmCanNmActionIdType;
/* typedefed type only used for debugging */

/* *** IDs for guards *** */
typedef enum
{
  CANNM_HSM_CANNM_GIDX_BUSSLEEPMODE_GUARD_3,
  CANNM_HSM_CANNM_GIDX_NETWORKMODE_GUARD_4,
  CANNM_HSM_CANNM_GIDX_READYSLEEPSTATE_GUARD_1,
  CANNM_HSM_CANNM_GIDX_READYSLEEPREMOTEACTIVITY_GUARD_1,
  CANNM_HSM_CANNM_GIDX_READYSLEEPREMOTESLEEP_GUARD_1,
  CANNM_HSM_CANNM_GIDX_NORMALOPERATIONREMOTEACTIVITY_GUARD_2,
  CANNM_HSM_CANNM_GIDX_NORMALOPERATIONREMOTEACTIVITY_GUARD_4,
  CANNM_HSM_CANNM_GIDX_NORMALOPERATIONREMOTESLEEP_GUARD_1,
  CANNM_HSM_CANNM_GIDX_REPEATMESSAGESTATE_GUARD_1,
  CANNM_HSM_CANNM_GIDX_REPEATMESSAGESTATE_GUARD_2,
  CANNM_HSM_CANNM_GIDX_PREPAREBUSSLEEPMODE_GUARD_2,
  CANNM_HSM_CANNM_NO_OF_GUARDS
} CanNm_HsmCanNmGuardIdType;
/* typedefed type only used for debugging */

/*==================[external function declarations]========================*/

#define CANNM_START_SEC_CODE
#include <CanNm_MemMap.h>

/* function declarations of state entry, exit, guard, action functions
 * defined in CanNm_fnct.c */

/* Deviation MISRAC2012-1 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfTOPEntry(
  CANNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfBusSleepModeEntry(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements action 'Nm_NetworkStartIndication(); #ifdef (...) Nm_PduRxIndication; #endif #if(Det == STD_ON);DET_REPORT_ERROR();#endif --> CANNM127 CANNM037 CANNM336 CANNM337' */
/* Deviation MISRAC2012-1 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfBusSleepModeAction1(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements guard 'NetworkRequested==TRUE' */
/* Deviation MISRAC2012-1 */
extern FUNC(boolean, CANNM_CODE)
 CanNm_HsmCanNmSfBusSleepModeGuard3(
  CANNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfNetworkModeEntry(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements action '#if (CANNM_COM_CONTROL_ENABLED == STD_ON) HandleComControl()' */
/* Deviation MISRAC2012-1 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfNetworkModeAction1(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements action 'HandleRxIndicationCommon()' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfNetworkModeAction2(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements action 'HandleTxConfirmation();' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfNetworkModeAction3(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements guard 'PnHandleMultipleNetworkRequests==TRUE && NetworkRequested==TRUE' */
/* Deviation MISRAC2012-1 */
extern FUNC(boolean, CANNM_CODE)
 CanNm_HsmCanNmSfNetworkModeGuard4(
  CANNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfReadySleepStateEntry(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements guard 'NmTimer == 0' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, CANNM_CODE)
 CanNm_HsmCanNmSfReadySleepStateGuard1(
  CANNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfReadySleepRemoteActivityEntry(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements guard 'PnHandleMultipleNetworkRequests==FALSE && NetworkRequested==TRUE' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, CANNM_CODE)
 CanNm_HsmCanNmSfReadySleepRemoteActivityGuard1(
  CANNM_PDL_SF(const uint8 instIdx));
/* Deviation MISRAC2012-1, MISRAC2012-2 */
/* implements action 'CanNm_MsgCycleTimerStart(MsgCycleOffset)' */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfReadySleepRemoteActivityAction1(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements guard 'PnHandleMultipleNetworkRequests==FALSE && NetworkRequested==TRUE' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, CANNM_CODE)
 CanNm_HsmCanNmSfReadySleepRemoteSleepGuard1(
  CANNM_PDL_SF(const uint8 instIdx));
/* implements action 'CanNm_MsgCycleTimerStart(MsgCycleOffset)' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfReadySleepRemoteSleepAction1(
  CANNM_PDL_SF(const uint8 instIdx));
/* implements action 'Nm_RemoteSleepCancellation();' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfReadySleepRemoteSleepAction2(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements action 'Nm_RemoteSleepCancellation(); HandleRxIndicationCommon();' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfReadySleepRemoteSleepAction3(
  CANNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfSendingSubModeExit(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements action '#if ((PASSIVE_MODE == false) && (IMM_TRANCONF_ENBLD == false)) Nm_TxTimeoutException(); #if (CANNM_PN_ENABLED == STD_ON) CanSM_TxTimeoutException();' */
/* Deviation MISRAC2012-1 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfSendingSubModeAction1(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements action 'HandleMsgCycleTimeout() --> CANNM032' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfSendingSubModeAction2(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements action 'if (NmTimeout == 0) {HandleNmTimeout();}' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfSendingSubModeAction3(
  CANNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfNormalOperationStateEntry(
  CANNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfNormalOperationRemoteActivityEntry(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements action 'HandleRxIndication_NOState_RA();' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfNormalOperationRemoteActivityAction1(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements guard 'NetworkRequested==FALSE' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, CANNM_CODE)
 CanNm_HsmCanNmSfNormalOperationRemoteActivityGuard2(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements guard '(!(ChanStatus & CANNM_COM_DISABLED)) && (UniTimer == 0)' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, CANNM_CODE)
 CanNm_HsmCanNmSfNormalOperationRemoteActivityGuard4(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements action 'Nm_RemoteSleepIndication' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfNormalOperationRemoteActivityAction4(
  CANNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfNormalOperationRemoteSleepEntry(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements guard 'NetworkRequested==FALSE' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, CANNM_CODE)
 CanNm_HsmCanNmSfNormalOperationRemoteSleepGuard1(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements action 'Nm_RemoteSleepCancellation();' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfNormalOperationRemoteSleepAction2(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements action 'HandleRxIndication_NOState() RSI timer is started by entry action #ifdef (RSI) Nm_RemoteSleepCancellation' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfNormalOperationRemoteSleepAction3(
  CANNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfRepeatMessageStateEntry(
  CANNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfRepeatMessageStateExit(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements guard '(NetworkRequested==FALSE) && (RMS timer == 0))' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, CANNM_CODE)
 CanNm_HsmCanNmSfRepeatMessageStateGuard1(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements guard '((NetworkRequested==TRUE) && (RMS timer == 0))' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, CANNM_CODE)
 CanNm_HsmCanNmSfRepeatMessageStateGuard2(
  CANNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfPrepareBusSleepModeEntry(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements guard 'NetworkRequested==TRUE' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, CANNM_CODE)
 CanNm_HsmCanNmSfPrepareBusSleepModeGuard2(
  CANNM_PDL_SF(const uint8 instIdx));

/* implements action '#ifdef (...) Nm_PduRxIndication(); #endif --> CANNM037' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, CANNM_CODE)
 CanNm_HsmCanNmSfPrepareBusSleepModeAction3(
  CANNM_PDL_SF(const uint8 instIdx));


#define CANNM_STOP_SEC_CODE
#include <CanNm_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* defined CANNM_HSMCANNMINT_H */
/*==================[end of file]===========================================*/
