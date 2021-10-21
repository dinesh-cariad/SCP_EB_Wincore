/**
 * \file
 *
 * \brief AUTOSAR LinIf
 *
 * This file contains the implementation of the AUTOSAR
 * module LinIf.
 *
 * \version 5.8.27
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* set macro that enables merged compilation macros */
#if (defined TS_MERGED_COMPILE)
#error TS_MERGED_COMPILE already defined!
#endif /* #if( defined TS_MERGED_COMPILE)*/
#define TS_MERGED_COMPILE STD_ON

/* First of all include all header files, the C-files depend on.
 * This prevent us for overlapping memory sections because all included files are protected for
 * further inclusion. */
#include <LinIf_Trace.h>      /* Debug and trace header file */
#include <TSAutosar.h>        /* EB specific standard types */
#include <TSCompiler.h>       /* EB Compiler Abstraction macros */
#include <ComStack_Types.h>   /* AUTOSAR comstack standard types */

#include <LinIf_Cfg.h>        /* Generated configuration */
#include <LinIf.h>            /* Module public API */
#include <LinIf_Types.h>      /* Module type definitions */
#include <LinIf_Int.h>        /* LinIf internal data */
#include <LinIf_Version.h>    /* Module version declarations */

#include <LinTp_Types.h>      /* Module type definitions */
#include <LinTp_Int.h>        /* LinTp internal data */

#include <LinSM_Cbk.h>        /* LinSM callback functions */
#include <SchM_LinIf.h>       /* for protecting exclusive areas */

#if(LINIF_SLAVE_SUPPORTED == STD_ON)
#include <LinIf_Externals.h>  /* for user callouts */
#include <Com.h>              /* for Com_SendSignal */
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */

#ifdef LINIF_WAKEUP_REV2_NOT_USED
#include <EcuM.h>             /* EcuM API */
#endif /* LINIF_WAKEUP_REV2_NOT_USED */

#ifdef LINIF_WAKEUP_LINAPI_42
#include <LinIf_Cbk.h>       /* LinIf callback functions */
#endif /* LINIF_WAKEUP_LINAPI_42 */

/* do not indirectly include PduR_SymbolicNames_PBcfg.h via PduR_LinIf/Tp.h */
#define PDUR_NO_PBCFG_REQUIRED
#include <PduR_LinIf.h>       /* PDU Router LIN Interface */
#if (LINIF_TP_SUPPORTED == STD_ON)
#include <PduR_LinTp.h>         /* PDU Router LIN-TP Interface */
#if (LINIF_MASTER_SUPPORTED == STD_ON)
#include <BswM_LinTp.h>         /* Base software manager header file */
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */
#endif /* (LINIF_TP_SUPPORTED == STD_ON) */
#if ((LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON))
#include <Det.h>              /* Default Error Tracer */
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON) */

/* All source files which contains sections other than *_SEC_CODE (or no sections) have to be
 * included before the memory mapping in this file to protect them for overlapping memory
 * sections. */
#include "LinIf_Init.c"
#include "LinIf_MainFunction.c"
#include "LinTp_Init.c"

/* list of files that include only memory abstraction CODE segments */
/* start code section declaration */
#define LINIF_START_SEC_CODE
#include <LinIf_MemMap.h>

#include "LinIf.c"
#include "LinIf_CheckWakeup.c"
#include "LinIf_GetVersionInfo.c"
#include "LinIf_GotoSleep.c"
#include "LinIf_ScheduleRequest.c"
#include "LinIf_Transmit.c"
#include "LinIf_Wakeup.c"

#if (LINIF_TRCV_SUPPORTED == STD_ON)
#include "LinIf_Trcv.c"
#endif /* LINIF_TRCV_SUPPORTED == STD_ON */

#if (LINIF_SLAVE_SUPPORTED == STD_ON)
#include "LinIf_ConfiguredNAD.c"
#include "LinIf_HeaderIndication.c"
#include "LinIf_LinErrorIndication.c"
#include "LinIf_PIDTable.c"
#include "LinIf_RxIndication.c"
#include "LinIf_TxConfirmation.c"
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */

#include "LinTp_CancelReceive.c"
#include "LinTp_CancelTransmit.c"
#include "LinTp_ChangeParameter.c"

#include "LinTp_GetVersionInfo.c"
#include "LinTp_Transmit.c"
#include "LinTp_MainFunction.c"


/* stop code section declaration */
#define LINIF_STOP_SEC_CODE
#include <LinIf_MemMap.h>
