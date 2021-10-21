/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#if (defined TS_MERGED_COMPILE) /* to prevent double declaration */
#error TS_MERGED_COMPILE is already defined
#endif /* if (defined TS_MERGED_COMPILE) */

/* set macro that enables merged compilation macros */
#define TS_MERGED_COMPILE STD_ON

/* First of all include all header files, the C-files depend on.
 * This prevent us for overlapping memory sections because all included files are protected for
 * further inclusion. */
#include <TSAutosar.h>         /* EB specific standard types */
#include <TSAtomic_Assign.h>   /* Header for atomic assignment macros */
#include <TSMem.h>             /* TS_MemCpy */
#include <ComStack_Types.h>    /* AUTOSAR communication stack types */
#include "CanTp_Api.h"         /* Module public API */
#include "CanTp_Internal.h"    /* internal macros and variables */
#include "CanTp_InternalLib.h" /* internal macros and variables */
#include "CanTp_InternalCfg.h" /* CanTp internal configuration */
#include "CanTp_Cbk.h"         /* CanTp callback declarations */
#include "CanTp_Cfg.h"         /* configuration for transmit cancellation */
#include "CanTp_Version.h"     /* CanTp version declarations */

#if (defined PDUR_NO_PBCFG_REQUIRED) /* to prevent double declaration */
#error PDUR_NO_PBCFG_REQUIRED is already defined
#endif /* if (defined PDUR_NO_PBCFG_REQUIRED) */

/* do not indirectly include PduR_SymbolicNames_PBcfg.h via PduR_CanTp.h */
#define PDUR_NO_PBCFG_REQUIRED
#include <PduR_CanTp.h>        /* PduR module API */
#include <CanIf.h>             /* CanIf module types and API */
#if(CANTP_JUMPTABLE_SUPPORT == STD_ON)
#include "CanTp_EntryTable.h"  /* CanTp entry jumptable */
#endif /* CANTP_JUMPTABLE_SUPPORT == STD_ON */
#if(CANTP_USE_GPT == STD_ON)
#include <Gpt.h>               /* Gpt module types and API */
#endif /* CANTP_USE_GPT == STD_ON */
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */

/* All source files which contains sections other than *_SEC_CODE (or no sections) have to be
 * included before the memory mapping in this file to protect them for overlapping memory
 * sections. */
#include "CanTp.c"
#include "CanTp_Init.c"

/* list of files that include only memory abstraction CODE segments */
/* start code section declaration */
#define CANTP_START_SEC_CODE
#include <CanTp_MemMap.h>

#include "CanTp_CancelTransmit.c"
#include "CanTp_GetNSa.c"
#include "CanTp_GetVersionInfo.c"
#include "CanTp_MainFunction.c"
#include "CanTp_PrepareFC.c"
#include "CanTp_PreparexF.c"
#include "CanTp_RxBufferDependentSend.c"
#include "CanTp_RxIndication.c"
#include "CanTp_SetNSa.c"
#include "CanTp_STminCallback.c"
#include "CanTp_StopChannel.c"
#include "CanTp_Transmit.c"
#include "CanTp_TransmitFrame.c"
#include "CanTp_TxBufferDependentSend.c"
#include "CanTp_TxConfirmation.c"
#include "CanTp_UnlockChannel.c"
#include "CanTp_ChangeParameter.c"
#include "CanTp_ReadParameter.c"
#include "CanTp_CancelReceive.c"

/* stop code section declaration */
#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>

