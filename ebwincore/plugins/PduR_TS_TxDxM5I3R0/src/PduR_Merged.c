/**
 * \file
 *
 * \brief AUTOSAR PduR
 *
 * This file contains the implementation of the AUTOSAR
 * module PduR.
 *
 * \version 5.3.46
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
#include <PduR_Internal_Static.h> /* Internal API (static part) */
#include <PduR_Api_Depend.h>      /* Public API (dependent part) */
#include <PduR_Version.h>         /* Version Information */
#include <TSMem.h>                /* TS_MemCpy */
#include <PduR_Lcfg.h>            /* Link-time configurable data */
#include <TSPBConfig_Signature.h> /* TS_PlatformSigIsValid */

#if ((PDUR_ZERO_COST_OPERATION_FULL == STD_OFF) && \
     ((PDUR_MULTICAST_UPTOLOTP_SUPPORT == STD_ON) || \
      (PDUR_MULTICAST_LOTPTOUP_SUPPORT == STD_ON) || \
      (PDUR_FIFO_TX_BUFFER_SUPPORT == STD_ON) || \
      (PDUR_SB_TX_BUFFER_SUPPORT == STD_ON) || \
      (PDUR_TPGATEWAY_SUPPORT == STD_ON) || \
      (PDUR_ROUTINGPATHGROUPS_SUPPORT == STD_ON) || \
      (PDUR_MULTICAST_TOIF_SUPPORT == STD_ON)))
#include <SchM_PduR.h>            /* Schedule Manager for module PduR */
#endif /* (PDUR_ZERO_COST_OPERATION_FULL == STD_OFF) &&  */

/* list of files that include memory abstractions other than the CODE segment */
#include "PduR.c"
#include "PduR_Init.c"
#include "PduR_IsValidConfig.c"


/* list of files that include only memory abstraction CODE segments */
/* start code section declaration */
#define PDUR_START_SEC_CODE
#include <PduR_MemMap.h>

 /* "" shall be used for inclusion of PduR's subordinate c-files within PduR_Merged.c */
#include "PduR_GateIfRxIndicationDf.c"
#include "PduR_GateIfRxIndicationNb.c"
#include "PduR_GateIfRxIndicationSb.c"
#include "PduR_GateIfRxIndicationTf.c"
#include "PduR_GateIfTriggerTransmitSb.c"
#include "PduR_GateIfTriggerTransmitTf.c"
#include "PduR_GateIfTxConfirmationDf.c"
#include "PduR_GateIfTxConfirmationTf.c"
#include "PduR_GateIfRxIndicationDfDynPyld.c"
#include "PduR_GateIfRxIndicationSbDynPyld.c"
#include "PduR_GateIfRxIndicationSbMCoreDynPyld.c"
#include "PduR_GateIfRxIndicationTfDynPyld.c"
#include "PduR_GateIfRxIndicationDfMCoreDynPyld.c"
#include "PduR_GateIfTriggerTransmitSbDynPyld.c"
#include "PduR_GateIfTriggerTransmitTfDynPyld.c"
#include "PduR_GateIfTxConfirmationDfDynPyld.c"
#include "PduR_GateIfTxConfirmationTfDynPyld.c"
#include "PduR_GateIfTxConfirmationDfMCoreDynPyld.c"
#include "PduR_GateIfRxIndicationMcUpSbDynPyld.c"
#include "PduR_GateTp.c"
#include "PduR_GateTpUpReception.c"
#include "PduR_GateTpStartOfReception.c"
#include "PduR_GateTpCopyRxData.c"
#include "PduR_GateTpRxIndication.c"
#include "PduR_GateTpCopyTxData.c"
#include "PduR_GateTpTxConfirmation.c"
#include "PduR_Nto1GateTpStartOfReception.c"
#include "PduR_Nto1GateTpCopyRxData.c"
#include "PduR_Nto1GateTpRxIndication.c"
#include "PduR_Nto1GateTpCopyTxData.c"
#include "PduR_Nto1GateTpTxConfirmation.c"
#include "PduR_GenericIfRxIndication.c"
#include "PduR_GenericIfTransmit.c"
#include "PduR_GenericIfTriggerTransmit.c"
#include "PduR_GenericIfTxConfirmation.c"
#include "PduR_GenericTpChangeParameter.c"
#include "PduR_GenericTpCancelTransmit.c"
#include "PduR_GenericTpCancelReceive.c"
#include "PduR_GenericTpStartOfReception.c"
#include "PduR_GenericTpCopyRxData.c"
#include "PduR_GenericTpRxIndication.c"
#include "PduR_GenericTpCopyTxData.c"
#include "PduR_GenericTpTxConfirmation.c"
#include "PduR_GenericTpTransmit.c"
#include "PduR_GetConfigurationId.c"
#include "PduR_GetVersionInfo.c"
#include "PduR_MultiIf.c"
#include "PduR_MultiIfRxIndication.c"
#include "PduR_MultiUpIfTransmit.c"
#include "PduR_MultiUpMCoreIfTransmit.c"
#include "PduR_MultiIfTriggerTransmit.c"
#include "PduR_MultiIfTxConfirmation.c"
#include "PduR_MultiTp.c"
#include "PduR_MultiTpCopyTxData.c"
#include "PduR_MultiTpTxConfirmation.c"
#include "PduR_MultiTpTransmit.c"
#include "PduR_WrapASR32.c"
#include "PduR_EnableRouting.c"
#include "PduR_DisableRouting.c"
#include "PduR_RPathGroups.c"

/* stop code section declaration */
#define PDUR_STOP_SEC_CODE
#include <PduR_MemMap.h>
