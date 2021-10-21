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
 * \brief  Converts nanoseconds into macroticks according the configuration of a
 *         specific FlexRay Controller
 *
 * \param FrIf_CtrlIdx (in)       FlexRay controller index.
 *
 * \retval  *   Number of Macroticks
 */

FUNC(uint16,FRIF_CODE) FrIf_GetMacrotickDuration
    (
        uint8 FrIf_CtrlIdx
    )
{
    uint16 RetValue = 0U;

    DBG_FRIF_GETMACROTICKDURATION_ENTRY(FrIf_CtrlIdx);

/* check if development error detection is disabled */
#if (FRIF_DEV_ERROR_DETECT == STD_OFF)

    TS_PARAM_UNUSED(FrIf_CtrlIdx);

#else /* FRIF_DEV_ERROR_DETECT == STD_OFF */
    /* check for successfully initialized module */
    /* Report to DET and return Error in case module was not initialized before */
    if(FrIf_ModuleInitialized == FRIF_UNINIT)
    {
        /* report error to DET */
        FrIf_Det_ReportError(FRIF_GETMACROTICKDURATION_SERVICE_ID, FRIF_E_NOT_INITIALIZED);
    }

#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_ON)

    /* check that controller index is 0 */
    else if (FrIf_CtrlIdx != (uint8) 0U)

#else /* FRIF_SINGLE_CTRL_OPT_ENABLE */

    /* check whether configuration has an entry for this FlexRay CC Idx */
    else if(FRIF_CTRLIDX >= FrIf_RootConfigPtr->FrIf_CtrlNum)

#endif  /* FRIF_SINGLE_CTRL_OPT_ENABLE */
    {

        /* Report to DET */
        FrIf_Det_ReportError(FRIF_GETMACROTICKDURATION_SERVICE_ID, FRIF_E_INV_CTRL_IDX);
    }

    else

#endif  /* FRIF_DEV_ERROR_DETECT == STD_OFF */
    {
        /* get the macroticks per cycle for that controller */
        RetValue = FRIF_GET_CONFIG_ADDR(FrIf_CtrlType, FrIf_RootConfigPtr->FrIf_CtrlRef)
                [FRIF_CTRLIDX].NanoPerMT;
    }

    DBG_FRIF_GETMACROTICKDURATION_EXIT(RetValue,FrIf_CtrlIdx);
    return RetValue;
}


/* stop code section declaration */
#if (TS_MERGED_COMPILE == STD_OFF)
#define FRIF_STOP_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */
#endif /* TS_MERGED_COMPILE */

