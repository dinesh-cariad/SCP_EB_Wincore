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

/*======================[Include Section]=====================================*/

#include <FrIf_guards.h> /* Include define guards */
#include <FrIf_Trace.h>
#include <FrIf_Priv.h>

/*======================[Local Macros]========================================*/

/*======================[Local Data Types]====================================*/

/*======================[Local Data]==========================================*/

/*======================[Local Functions]=====================================*/

/*======================[Global Functions]====================================*/

/* only available if Fr-integration is disabled */
#if (FRIF_FR_INTEGRATION_ENABLE == STD_OFF)

/* !LINKSTO FrIf.ASR44.SWS_FrIf_05727,1 */
#if (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON)

#if (TS_MERGED_COMPILE == STD_OFF)
#define FRIF_START_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */
#endif /* TS_MERGED_COMPILE */

/**
 * \brief  Enables or disables mirroring for all FlexRay controllers connected to the addressed FlexRay cluster.
 *
 * \param FrIf_ClstIdx (in)                 Index of the FlexRay cluster to address.
 * \param FrIf_MirroringActive (in)         TRUE: Mirror_ReportFlexRayFrame will be called for each
 *                                                frame received or transmitted on the addressed FlexRay CC.
 *                                          FALSE: Mirror_ReportFlexRayFrame will not be called for
 *                                                 the addressed FlexRay CC.
 *
 * \retval  E_OK                Mirroring mode was changed.
 * \retval  E_NOT_OK            Wrong FrIf_CtrlIdx, or mirroring is globally disabled (see FrIfBusMirroringSupport).
 *
 * \ServiceID{0x4B}
 * \Reentrancy{Re-entrant}
 * \Synchronicity{Synchronous}
 */
FUNC(Std_ReturnType,FRIF_CODE) FrIf_EnableBusMirroring
    (
        uint8 FrIf_ClstIdx,
        boolean FrIf_MirroringActive
    )
{
    Std_ReturnType RetValue = E_NOT_OK;

    DBG_FRIF_ENABLEBUSMIRRORING_ENTRY(FrIf_ClstIdx,FrIf_MirroringActive);

#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_ON)
    TS_PARAM_UNUSED(FrIf_ClstIdx);
#endif /* FRIF_SINGLE_CLST_OPT_ENABLE */

/* check if development error detection is enabled */
#if (FRIF_DEV_ERROR_DETECT == STD_ON)

    /* check for successfully initialized module */
    /* Report to DET and return Error in case module was not initialized before */
    if(FrIf_ModuleInitialized == FRIF_UNINIT)
    {
        /* Report to DET */
        FrIf_Det_ReportError(FRIF_ENABLEBUSMIRRORING_SERVICE_ID, FRIF_E_NOT_INITIALIZED);
    }

#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_ON)

    /* check that cluster index is 0 */
    else if (FrIf_ClstIdx != (uint8) 0U)

#else /* FRIF_SINGLE_CLST_OPT_ENABLE */

    /* check whether configuration has an entry for this FlexRay Cluster Idx */
    else if(FRIF_CLSTIDX >= FrIf_RootConfigPtr->FrIf_ClstNum)

#endif  /* FRIF_SINGLE_CLST_OPT_ENABLE */
    {
        /* !LINKSTO FrIf.EB.BusmirrorSupport.FrIfEnableBusMirroring.InvalidClstIdx,1 */
        /* Report to DET */
        FrIf_Det_ReportError(FRIF_ENABLEBUSMIRRORING_SERVICE_ID, FRIF_E_INV_CLST_IDX);
    }

    /* all DET checks passed */
    else
#endif  /* FRIF_DEV_ERROR_DETECT */
    {
        /* set BusmirroringEnabled of Cluster to requested state and return E_OK*/
        FrIf_ClstRuntimeData[FRIF_CLSTIDX].BusmirroringEnabled = FrIf_MirroringActive;
        RetValue = E_OK;
    }

    DBG_FRIF_ENABLEBUSMIRRORING_EXIT(RetValue,FrIf_ClstIdx,FrIf_MirroringActive);
    return RetValue;
}

/* stop code section declaration */
#if (TS_MERGED_COMPILE == STD_OFF)
#define FRIF_STOP_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */
#endif /* TS_MERGED_COMPILE */

#endif /* FRIF_BUSMIRRORINGSUPPORT_ENABLE */

#endif /* FRIF_FR_INTEGRATION_ENABLE */

