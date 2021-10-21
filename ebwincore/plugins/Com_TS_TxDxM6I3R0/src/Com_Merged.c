/**
 * \file
 *
 * \brief AUTOSAR Com
 *
 * This file contains the implementation of the AUTOSAR
 * module Com.
 *
 * \version 6.3.50
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
#include <Com_Priv.h>                /* public intra module API */
#include <Com_Api.h>                 /* Module public API */
#include <Com_Cbk_Static.h>          /* Static ""callbacks"" of the COM */
#include <Com_Core_IPduGroup.h>      /* Header IPduGroup functions */
#include <Com_Core_RxIPdu.h>         /* Header TxIPdu functions */
#include <Com_Core_RxUpdateBit.h>    /* UpdateBit Functions */
#include <Com_Core_TM.h>             /* Transmission Mode functions */
#include <Com_Core_TxIPdu.h>         /* Header TxIPdu functions */
#include <Com_Core_TxSignal.h>       /* signal functions */
#include <Com_Core_TxSignalGroup.h>  /* (group)signal(group) functions */
#include <Com_Defs.h>                /* COM_SERVICE_NOT_AVAILABLE */
#include <Com_Filter.h>              /* Filter functions */
#include <Com_Lcfg_Static.h>         /* declaration of the callback arrays */
#include <Com_Rules.h>               /* Generated configuration + derived rules */
#include <Com_Types.h>               /* types of configuration elements */
#include <Com_Version.h>             /* Module version declarations */
#include <ComStack_Types.h>          /* AUTOSAR standard types */
#include <Com_Trace.h>               /* Macros for Debug and Trace instrumentation */
#include <TSPBConfig_Signature.h>    /* TS_PlatformSigIsValid */
#include <Com_Core_Read.h>           /* de-serialization library */
#include <Com_Core_Write.h>          /* serialization library */

#if ((COM_TX_TPAPI_ENABLE == STD_ON) || (COM_RX_TPAPI_ENABLE == STD_ON))
#include <EcuC.h>
#endif /* ((COM_TX_TPAPI_ENABLE == STD_ON) || (COM_RX_TPAPI_ENABLE == STD_ON)) */

/* Com module does not require configuration of PduR, no need to re-build
 * Com module in case the post-build configuration of PduR changes */
#define PDUR_NO_PBCFG_REQUIRED
#include <PduR_Com.h>                /* For the PduR_ComTransmit-call */
#include <TSMem.h>                   /* TS_MemCpy */
#include <Com_ComXf.h>               /* APIs for EB ComXf */

#if (COM_PBCFGM_SUPPORT_ENABLED == STD_ON)
#include <PbcfgM_Api.h>             /* Post build configuration manager */
#endif /* (COM_PBCFGM_SUPPORT_ENABLED == STD_ON) */


/* list of files that include memory abstractions other than the CODE segment */
#include "Com.c"
#include "Com_IsValidConfig.c"
#include "Com_Core_Read.c"
#include "Com_Core_Write.c"


/* list of files that include only memory abstraction CODE segments */
/* start code section declaration */
#define COM_START_SEC_CODE
#include <Com_MemMap.h>

#include "Com_Core_IPduGroup.c"
#include "Com_Core_RxIPdu.c"
#include "Com_Core_RxUpdateBit.c"
#include "Com_Core_TM.c"
#include "Com_Core_TxIPdu.c"
#include "Com_Core_TxSignal.c"
#include "Com_Core_TxSignalGroup.c"
#include "Com_IpduGroupControl.c"
#include "Com_ReceptionDMControl.c"
#include "Com_Filter.c"
#include "Com_GetConfigurationId.c"
#include "Com_Init.c"
#include "Com_ClearIpduGroupVector.c"
#include "Com_SetIpduGroup.c"
#include "Com_MainFunctionRouteSignals.c"
#include "Com_MainFunctionRx.c"
#include "Com_MainFunctionTx.c"
#include "Com_ReceiveShadowSignal.c"
#include "Com_ReceiveSignal.c"
#include "Com_ReceiveSignalGroup.c"
#include "Com_RxIndication.c"
#include "Com_SendSignal.c"
#include "Com_SendSignalGroup.c"
#include "Com_SwitchIpduTxMode.c"
#include "Com_TriggerIPDUSend.c"
#include "Com_TriggerTransmit.c"
#include "Com_TxConfirmation.c"
#include "Com_UpdateShadowSignal.c"
#include "Com_GetVersionInfo.c"
#include "Com_DeInit.c"
#include "Com_GetStatus.c"
#include "Com_CopyTxData.c"
#include "Com_CopyRxData.c"
#include "Com_StartOfReception.c"

/* stop code section declaration */
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
