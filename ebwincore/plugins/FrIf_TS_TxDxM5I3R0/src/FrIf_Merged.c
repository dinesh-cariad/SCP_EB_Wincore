/**
 * \file
 *
 * \brief AUTOSAR FrIf
 *
 * This file contains the implementation of the AUTOSAR
 * module FrIf.
 *
 * \version 5.3.25
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* set macro that enables merged compilation macros */
#define TS_MERGED_COMPILE STD_ON

/* first of all include all header files, the C-files depend on */
#include <TSMem.h>              /* TS_memset */
#include <SchM_FrIf.h>
#include <FrIf_merged_includes.h>
/* checking for development error tracer (DET) configuration */

#include <FrIf_Cfg.h> /* defines FRIF_REPORT_TO_DET_ENABLE */
#if (FRIF_REPORT_TO_DET_ENABLE == STD_ON)
#include <Det.h> /* get development error tracer interface */
#endif   /* (FRIF_REPORT_TO_DET_ENABLE == STD_ON) */

#include <FrIf_Priv.h>
#include <FrIf_guards.h> /* Include define guards */

/* list of files that include only memory abstraction CODE segments */
/* start code section declaration */
#define FRIF_START_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */

#include <FrIf_AbortCommunication.c>
#include <FrIf_AckAbsoluteTimerIRQ.c>
#include <FrIf_AckIRQ.c>
#include <FrIf_AckRelativeTimerIRQ.c>
#include <FrIf_AllowColdstart.c>
#include <FrIf_AllSlots.c>
#include <FrIf_CancelAbsoluteTimer.c>
#include <FrIf_CancelRelativeTimer.c>
#include <FrIf_CheckWakeupByTransceiver.c>
#include <FrIf_CheckMTS.c>
#include <FrIf_ClearTransceiverWakeup.c>
#include <FrIf_ControllerInit.c>
#include <FrIf_DisableAbsoluteTimerIRQ.c>
#include <FrIf_DisableIRQ.c>
#include <FrIf_DisableLPdu.c>
#include <FrIf_DisableRelativeTimerIRQ.c>
#include <FrIf_DisableTransceiverBranch.c>
#include <FrIf_EnableAbsoluteTimerIRQ.c>
#include <FrIf_EnableBusMirroring.c>
#include <FrIf_EnableIRQ.c>
#include <FrIf_EnableRelativeTimerIRQ.c>
#include <FrIf_EnableTransceiverBranch.c>
#include <FrIf_GetAbsoluteTimerIRQStatus.c>
#include <FrIf_GetChannelStatus.c>
#include <FrIf_GetClockCorrection.c>
#include <FrIf_GetControllerErrorStatus.c>
#include <FrIf_GetCycleLength.c>
#include <FrIf_GetGlobalTime.c>
#include <FrIf_GetIRQStatus.c>
#include <FrIf_GetNmVector.c>
#include <FrIf_GetNumOfStartupFrames.c>
#include <FrIf_GetPOCStatus.c>
#include <FrIf_GetRelativeTimerIRQStatus.c>
#include <FrIf_GetSyncFrameList.c>
#include <FrIf_GetSyncState.c>
#include <FrIf_GetTransceiverError.c>
#include <FrIf_GetTransceiverMode.c>
#include <FrIf_GetTransceiverWUReason.c>
#include <FrIf_GetWakeupRxStatus.c>
#include <FrIf_GetVersionInfo.c>
#include <FrIf_HaltCommunication.c>
#include <FrIf_JobListExecution.c>
#include <FrIf_ReadCCConfig.c>
#include <FrIf_ReconfigLPdu.c>
#include <FrIf_SendMTS.c>
#include <FrIf_SendWUP.c>
#include <FrIf_SetAbsoluteTimer.c>
#include <FrIf_SetExtSync.c>
#include <FrIf_SetRelativeTimer.c>
#include <FrIf_SetTransceiverMode.c>
#include <FrIf_SetWakeupChannel.c>
#include <FrIf_StartCommunication.c>
#include <FrIf_StateMachine.c>
#include <FrIf_TimeConversion.c>
#include <FrIf_Transmit.c>

/* stop code section declaration */
#define FRIF_STOP_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */

#include <FrIf_Init.c>

