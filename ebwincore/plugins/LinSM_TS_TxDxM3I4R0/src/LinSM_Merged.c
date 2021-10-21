/**
 * \file
 *
 * \brief AUTOSAR LinSM
 *
 * This file contains the implementation of the AUTOSAR
 * module LinSM.
 *
 * \version 3.4.19
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#if (defined TS_MERGED_COMPILE) /* to prevent double declaration */
#error TS_MERGED_COMPILE already defined
#endif /* if (defined TS_MERGED_COMPILE) */

/* set macro that enables merged compilation macros */
#define TS_MERGED_COMPILE STD_ON

/* First of all include all header files, the C-files depend on.
 * This prevent us for overlapping memory sections because all included files are protected for
 * further inclusion. */
#include <LinSM_Internal.h>   /* Module private API */
#include <LinSM_Cfg.h>        /* generated config */
#include <BswM_LinSM.h>

/* All source files which contains sections other than *_SEC_CODE (or no sections) have to be
 * included before the memory mapping in this file to protect them for overlapping memory
 * sections. */
#include "LinSM.c"

/* list of files that include only memory abstraction CODE segments */
/* start code section declaration */
#define LINSM_START_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

#include "LinSM_GetCurrentComMode.c"
#include "LinSM_GetVersionInfo.c"
#include "LinSM_GotoSleepConfirmation.c"
#include "LinSM_GotoSleepIndication.c"
#include "LinSM_Init.c"
#include "LinSM_MainFunction.c"
#include "LinSM_RequestComMode.c"
#include "LinSM_ScheduleRequest.c"
#include "LinSM_ScheduleRequestConfirmation.c"
#include "LinSM_WakeupConfirmation.c"

/* stop code section declaration */
#define LINSM_STOP_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

