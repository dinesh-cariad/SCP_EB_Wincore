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

#if (TS_MERGED_COMPILE == STD_OFF)
#define FRIF_START_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */
#endif /* TS_MERGED_COMPILE */

/**
 * \brief  This API returns the configured time of the configuration parameter "GdCycle"
 *         in nanoseconds for the FlexRay controller with index FrIf_CtrlIdx.
 *
 * \param FrIf_CtrlIdx (in)     Index of the FlexRay CC to address.
 *
 * \retval                      Time in unit of nanoseconds
 *
 * \ServiceID{0x3A}
 * \Reentrancy{Re-entrant for different values of FrIf_TxPduId only}
 * \Synchronicity{Synchronous}
 */
FUNC(uint32,FRIF_CODE) FrIf_GetCycleLength
    (
        uint8 FrIf_CtrlIdx
    )
{
    uint32 RetValue = 0U;

    DBG_FRIF_GETCYCLELENGTH_ENTRY(FrIf_CtrlIdx);

/* check if development error detection is enabled */
#if (FRIF_DEV_ERROR_DETECT == STD_ON)

    /* check for successfully initialized module */
    /* Report to DET and return Error in case module was not initialized before */
    if(FrIf_ModuleInitialized == FRIF_UNINIT)
    {
        /* Report to DET */
        FrIf_Det_ReportError(FRIF_GETCYCLELENGTH_SERVICE_ID, FRIF_E_NOT_INITIALIZED);
    }

#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_ON)

    /* check that controller index is 0 if single controller optimization is enabled */
    else if (FrIf_CtrlIdx != (uint8) 0U)

#else /* FRIF_SINGLE_CTRL_OPT_ENABLE */

    /* otherwise check that the index is not out of configuration-bounds */
    else if(FrIf_CtrlIdx >= FrIf_RootConfigPtr->FrIf_CtrlNum)

#endif /* FRIF_SINGLE_CTRL_OPT_ENABLE */
    {
        /* Report to DET */
        FrIf_Det_ReportError(FRIF_GETCYCLELENGTH_SERVICE_ID, FRIF_E_INV_CTRL_IDX);
    }

    /* all DET checks passed */
    else
#endif  /* FRIF_DEV_ERROR_DETECT */
    {

#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_ON)
        TS_PARAM_UNUSED(FrIf_CtrlIdx);
#endif /* FRIF_SINGLE_CTRL_OPT_ENABLE */

        /* get the nanoseconds per cycle for that controller */
        RetValue = FRIF_GET_CONFIG_ADDR(FrIf_CtrlType, FrIf_RootConfigPtr->FrIf_CtrlRef)
                      [FRIF_CTRLIDX].NanoPerCycle;
    }

    /* return result */

    DBG_FRIF_GETCYCLELENGTH_EXIT(RetValue,FrIf_CtrlIdx);
    return RetValue;
}

/* stop code section declaration */
#if (TS_MERGED_COMPILE == STD_OFF)
#define FRIF_STOP_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */
#endif /* TS_MERGED_COMPILE */

