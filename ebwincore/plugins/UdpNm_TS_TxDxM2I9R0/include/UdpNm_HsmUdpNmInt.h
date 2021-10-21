/**
 * \file
 *
 * \brief AUTOSAR UdpNm
 *
 * This file contains the implementation of the AUTOSAR
 * module UdpNm.
 *
 * \version 2.9.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef UDPNM_HSMUDPNMINT_H
#define UDPNM_HSMUDPNMINT_H

/* Internal interface for the UdpNm state
 * machine.
 *
 * This header must only be included by
 * UdpNm_HsmUdpNmData.c and UdpNm_HsmUdpNmFnct.c.
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
#include <UdpNm_Hsm.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/* *** State indices and IDs for states *** */
typedef enum
{
  UDPNM_HSM_UDPNM_SIDX_TOP,
  UDPNM_HSM_UDPNM_SIDX_BUSSLEEPMODE,
  UDPNM_HSM_UDPNM_SIDX_NETWORKMODE,
  UDPNM_HSM_UDPNM_SIDX_READYSLEEPSTATE,
  UDPNM_HSM_UDPNM_SIDX_READYSLEEPREMOTEACTIVITY,
  UDPNM_HSM_UDPNM_SIDX_READYSLEEPREMOTESLEEP,
  UDPNM_HSM_UDPNM_SIDX_SENDINGSUBMODE,
  UDPNM_HSM_UDPNM_SIDX_NORMALOPERATIONSTATE,
  UDPNM_HSM_UDPNM_SIDX_NORMALOPERATIONREMOTEACTIVITY,
  UDPNM_HSM_UDPNM_SIDX_NORMALOPERATIONREMOTESLEEP,
  UDPNM_HSM_UDPNM_SIDX_REPEATMESSAGESTATE,
  UDPNM_HSM_UDPNM_SIDX_PREPAREBUSSLEEPMODE,
  UDPNM_HSM_UDPNM_NO_OF_STATES
} UdpNm_HsmUdpNmStateIdType;
/* typedefed type only used for debugging */

/* *** IDs for entry/axit and transition actions *** */
typedef enum
{
  UDPNM_HSM_UDPNM_AIDX_TOP_ENTRY,
  UDPNM_HSM_UDPNM_AIDX_BUSSLEEPMODE_ENTRY,
  UDPNM_HSM_UDPNM_AIDX_BUSSLEEPMODE_ACTION_1,
  UDPNM_HSM_UDPNM_AIDX_NETWORKMODE_ENTRY,
  UDPNM_HSM_UDPNM_AIDX_NETWORKMODE_ACTION_1,
  UDPNM_HSM_UDPNM_AIDX_NETWORKMODE_ACTION_2,
  UDPNM_HSM_UDPNM_AIDX_NETWORKMODE_ACTION_3,
  UDPNM_HSM_UDPNM_AIDX_READYSLEEPSTATE_ENTRY,
  UDPNM_HSM_UDPNM_AIDX_READYSLEEPREMOTEACTIVITY_ENTRY,
  UDPNM_HSM_UDPNM_AIDX_READYSLEEPREMOTEACTIVITY_ACTION_1,
  UDPNM_HSM_UDPNM_AIDX_READYSLEEPREMOTESLEEP_ACTION_1,
  UDPNM_HSM_UDPNM_AIDX_READYSLEEPREMOTESLEEP_ACTION_2,
  UDPNM_HSM_UDPNM_AIDX_READYSLEEPREMOTESLEEP_ACTION_3,
  UDPNM_HSM_UDPNM_AIDX_SENDINGSUBMODE_EXIT,
  UDPNM_HSM_UDPNM_AIDX_SENDINGSUBMODE_ACTION_1,
  UDPNM_HSM_UDPNM_AIDX_SENDINGSUBMODE_ACTION_2,
  UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONSTATE_ENTRY,
  UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ENTRY,
  UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ACTION_1,
  UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONREMOTEACTIVITY_ACTION_4,
  UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONREMOTESLEEP_ENTRY,
  UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONREMOTESLEEP_ACTION_2,
  UDPNM_HSM_UDPNM_AIDX_NORMALOPERATIONREMOTESLEEP_ACTION_3,
  UDPNM_HSM_UDPNM_AIDX_REPEATMESSAGESTATE_ENTRY,
  UDPNM_HSM_UDPNM_AIDX_REPEATMESSAGESTATE_EXIT,
  UDPNM_HSM_UDPNM_AIDX_PREPAREBUSSLEEPMODE_ENTRY,
  UDPNM_HSM_UDPNM_AIDX_PREPAREBUSSLEEPMODE_ACTION_3,
  UDPNM_HSM_UDPNM_NO_OF_ACTIONS
} UdpNm_HsmUdpNmActionIdType;
/* typedefed type only used for debugging */

/* *** IDs for guards *** */
typedef enum
{
  UDPNM_HSM_UDPNM_GIDX_BUSSLEEPMODE_GUARD_3,
  UDPNM_HSM_UDPNM_GIDX_NETWORKMODE_GUARD_4,
  UDPNM_HSM_UDPNM_GIDX_READYSLEEPSTATE_GUARD_1,
  UDPNM_HSM_UDPNM_GIDX_READYSLEEPREMOTEACTIVITY_GUARD_1,
  UDPNM_HSM_UDPNM_GIDX_READYSLEEPREMOTESLEEP_GUARD_1,
  UDPNM_HSM_UDPNM_GIDX_NORMALOPERATIONREMOTEACTIVITY_GUARD_2,
  UDPNM_HSM_UDPNM_GIDX_NORMALOPERATIONREMOTEACTIVITY_GUARD_4,
  UDPNM_HSM_UDPNM_GIDX_NORMALOPERATIONREMOTESLEEP_GUARD_1,
  UDPNM_HSM_UDPNM_GIDX_REPEATMESSAGESTATE_GUARD_1,
  UDPNM_HSM_UDPNM_GIDX_REPEATMESSAGESTATE_GUARD_2,
  UDPNM_HSM_UDPNM_GIDX_PREPAREBUSSLEEPMODE_GUARD_2,
  UDPNM_HSM_UDPNM_NO_OF_GUARDS
} UdpNm_HsmUdpNmGuardIdType;
/* typedefed type only used for debugging */

/*==================[external function declarations]========================*/

#define UDPNM_START_SEC_CODE
#include <UdpNm_MemMap.h>

/* function declarations of state entry, exit, guard, action functions
 * defined in UdpNm_fnct.c */

/* Deviation MISRAC2012-1 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfTOPEntry(
  UDPNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfBusSleepModeEntry(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements action 'Nm_NetworkStartIndication(); #ifdef (...) Nm_PduRxIndication(); #endif --> SWS_UdpNm_00127; SWS_UdpNm_00037; SWS_UdpNm_00164;' */
/* Deviation MISRAC2012-1 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfBusSleepModeAction1(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements guard 'NetworkRequested==TRUE' */
/* Deviation MISRAC2012-1 */
extern FUNC(boolean, UDPNM_CODE)
 UdpNm_HsmUdpNmSfBusSleepModeGuard3(
  UDPNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfNetworkModeEntry(
  UDPNM_PDL_SF(const uint8 instIdx));
/* implements action '#if (UDPNM_COM_CONTROL_ENABLED == STD_ON) UdpNm_HandleComControl() -> SWS_UdpNm_00176; SWS_UdpNm_00170;' */
/* Deviation MISRAC2012-1 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfNetworkModeAction1(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements action 'HandleRxIndicationCommon() --> SWS_UdpNm_00037; SWS_UdpNm_00098;SWS_UdpNm_00164;' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfNetworkModeAction2(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements action 'HandleTxConfirmation(); --> SWS_UdpNm_00099;' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfNetworkModeAction3(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements guard 'PnHandleMultipleNetworkRequests==TRUE && NetworkRequested==TRUE' */
/* Deviation MISRAC2012-1 */
extern FUNC(boolean, UDPNM_CODE)
 UdpNm_HsmUdpNmSfNetworkModeGuard4(
  UDPNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfReadySleepStateEntry(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements guard 'NmTimer == 0' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, UDPNM_CODE)
 UdpNm_HsmUdpNmSfReadySleepStateGuard1(
  UDPNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfReadySleepRemoteActivityEntry(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements guard 'PnHandleMultipleNetworkRequests==FALSE && NetworkRequested==TRUE' */
/* Deviation MISRAC2012-1 */
extern FUNC(boolean, UDPNM_CODE)
 UdpNm_HsmUdpNmSfReadySleepRemoteActivityGuard1(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements action 'HandleTransmit()' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfReadySleepRemoteActivityAction1(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements guard 'PnHandleMultipleNetworkRequests==FALSE && NetworkRequested==TRUE' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, UDPNM_CODE)
 UdpNm_HsmUdpNmSfReadySleepRemoteSleepGuard1(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements action 'HandleTransmit()' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfReadySleepRemoteSleepAction1(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements action 'Nm_RemoteSleepCancellation(); */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfReadySleepRemoteSleepAction2(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements action 'Nm_RemoteSleepCancellation(); HandleRxIndicationCommon();' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfReadySleepRemoteSleepAction3(
  UDPNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfSendingSubModeExit(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements action 'if (NmTimeout == 0) {HandleNmTimeout();} -->SWS_UdpNm_00193; SWS_UdpNm_00194; SWS_UdpNm_00101; SWS_UdpNm_00117; SWS_UdpNm_00206;' */
/* Deviation MISRAC2012-1 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfSendingSubModeAction1(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements action 'UdpNm_HandleTransmit() --> SWS_UdpNm_00032;' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfSendingSubModeAction2(
  UDPNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfNormalOperationStateEntry(
  UDPNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfNormalOperationRemoteActivityEntry(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements action 'HandleRxIndication_NOState_RA(); --> SWS_UdpNm_00037;SWS_UdpNm_00164;' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfNormalOperationRemoteActivityAction1(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements guard 'NetworkRequested==FALSE' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, UDPNM_CODE)
 UdpNm_HsmUdpNmSfNormalOperationRemoteActivityGuard2(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements guard '(!(ChanStatus & UDPNM_COM_DISABLED)) && (UniTimer == 0)' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, UDPNM_CODE)
 UdpNm_HsmUdpNmSfNormalOperationRemoteActivityGuard4(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements action 'Nm_RemoteSleepIndication();' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfNormalOperationRemoteActivityAction4(
  UDPNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfNormalOperationRemoteSleepEntry(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements guard 'NetworkRequested==FALSE' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, UDPNM_CODE)
 UdpNm_HsmUdpNmSfNormalOperationRemoteSleepGuard1(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements action 'Nm_RemoteSleepCancellation();' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfNormalOperationRemoteSleepAction2(
  UDPNM_PDL_SF(const uint8 instIdx));
/* implements action 'UdpNm_HandleRxIndicationCommon() RSI timer is started by entry action /' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfNormalOperationRemoteSleepAction3(
  UDPNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfRepeatMessageStateEntry(
  UDPNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfRepeatMessageStateExit(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements guard '(NetworkRequested==FALSE) && (RMS timer == 0))' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, UDPNM_CODE)
 UdpNm_HsmUdpNmSfRepeatMessageStateGuard1(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements guard '((NetworkRequested==TRUE) && (RMS timer == 0))' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, UDPNM_CODE)
 UdpNm_HsmUdpNmSfRepeatMessageStateGuard2(
  UDPNM_PDL_SF(const uint8 instIdx));

/* Deviation MISRAC2012-1 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfPrepareBusSleepModeEntry(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements guard 'NetworkRequested==TRUE' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(boolean, UDPNM_CODE)
 UdpNm_HsmUdpNmSfPrepareBusSleepModeGuard2(
  UDPNM_PDL_SF(const uint8 instIdx));

/* implements action '#ifdef (...) Nm_PduRxIndication(); #endif' */
/* Deviation MISRAC2012-1, MISRAC2012-2 */
extern FUNC(void, UDPNM_CODE)
 UdpNm_HsmUdpNmSfPrepareBusSleepModeAction3(
  UDPNM_PDL_SF(const uint8 instIdx));


#define UDPNM_STOP_SEC_CODE
#include <UdpNm_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* defined UDPNM_HSMUDPNMINT_H */
/*==================[end of file]===========================================*/
