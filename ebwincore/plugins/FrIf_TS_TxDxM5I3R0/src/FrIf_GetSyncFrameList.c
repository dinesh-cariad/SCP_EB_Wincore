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
#if (FRIF_GETSYNCFRAMELIST_API_ENABLE == STD_ON)

#if (TS_MERGED_COMPILE == STD_OFF)
#define FRIF_START_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */
#endif /* TS_MERGED_COMPILE */

/**
 * \brief   Returns a list of sync frames.
 *
 * This service writes a list of sync frames observed in the last even/odd communication cycle couple
 * into *FrIf_ChannelAEvenListPtr, *FrIf_ChannelBEvenListPtr, *FrIf_ChannelAOddListPtr and
 * *FrIf_ChannelBOddListPtr
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx                 FlexRay controller index.
 * \param[in] FrIf_ListSize                Size of list passed to the output pointers.
 * \param[out] FrIf_ChannelAEvenListPtr    Address to write the list of even sync frames of channel A.
 * \param[out] FrIf_ChannelBEvenListPtr    Address to write the list of even sync frames of channel B.
 * \param[out] FrIf_ChannelAOddListPtr     Address to write the list of odd sync frames of channel A.
 * \param[out] FrIf_ChannelBOddListPtr     Address to write the list of odd sync frames of channel B.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x2A}
 * \Reentrancy{Non Reentrant for same FlexRay controller}
 * \Synchronicity{Synchronous}
 */

FUNC(Std_ReturnType,FRIF_CODE) FrIf_GetSyncFrameList
    (
        uint8 FrIf_CtrlIdx,
        uint8 FrIf_ListSize,
        P2VAR(uint16,AUTOMATIC,FRIF_APPL_DATA) FrIf_ChannelAEvenListPtr,
        P2VAR(uint16,AUTOMATIC,FRIF_APPL_DATA) FrIf_ChannelBEvenListPtr,
        P2VAR(uint16,AUTOMATIC,FRIF_APPL_DATA) FrIf_ChannelAOddListPtr,
        P2VAR(uint16,AUTOMATIC,FRIF_APPL_DATA) FrIf_ChannelBOddListPtr
    )
{
    Std_ReturnType RetValue = E_NOT_OK;

    DBG_FRIF_GETSYNCFRAMELIST_ENTRY(FrIf_CtrlIdx,FrIf_ListSize,FrIf_ChannelAEvenListPtr,FrIf_ChannelBEvenListPtr,FrIf_ChannelAOddListPtr,FrIf_ChannelBOddListPtr);

/* check if development error detection is disabled */
#if (FRIF_DEV_ERROR_DETECT == STD_OFF)

#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_ON)

    /* if single controller optimization is enabled, parameter FrIf_CtrlIdx is not used */
    TS_PARAM_UNUSED(FrIf_CtrlIdx);

#endif /* FRIF_SINGLE_CTRL_OPT_ENABLE */

#else /* FRIF_DEV_ERROR_DETECT == STD_OFF */
    /* check for successfully initialized module */
    /* Report to DET and return Error in case module was not initialized before */
    if(FrIf_ModuleInitialized == FRIF_UNINIT)
    {
        /* Report to DET */
        FrIf_Det_ReportError(FRIF_GETSYNCFRAMELIST_SERVICE_ID, FRIF_E_NOT_INITIALIZED);
    }

#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_ON)
    /* check that controller index is 0 if single controller optimization is enabled*/
    else if (FrIf_CtrlIdx != (uint8) 0U)

#else /* FRIF_SINGLE_CTRL_OPT_ENABLE */
    /* otherwise check that the index is not out of configuration-bounds */
    else if(FRIF_CTRLIDX >= FrIf_RootConfigPtr->FrIf_CtrlNum)

#endif  /* FRIF_SINGLE_CTRL_OPT_ENABLE */
    {
        /* Report to DET */
        FrIf_Det_ReportError(FRIF_GETSYNCFRAMELIST_SERVICE_ID, FRIF_E_INV_CTRL_IDX);
    }

    else

#endif  /* FRIF_DEV_ERROR_DETECT == STD_OFF */
    {
        /* call wrapped Fr service */
        RetValue = FrIf_Call_Fr_Arg6(FRIF_FR_INSTANCE_IDX,GetSyncFrameList,
            FRIF_FR_CTRLIDX,
            FrIf_ListSize,
            FrIf_ChannelAEvenListPtr,
            FrIf_ChannelBEvenListPtr,
            FrIf_ChannelAOddListPtr,
            FrIf_ChannelBOddListPtr);
    }

    /* return result */

    DBG_FRIF_GETSYNCFRAMELIST_EXIT(RetValue,FrIf_CtrlIdx,FrIf_ListSize,FrIf_ChannelAEvenListPtr,FrIf_ChannelBEvenListPtr,FrIf_ChannelAOddListPtr,FrIf_ChannelBOddListPtr);
    return RetValue;
}

/* stop code section declaration */
#if (TS_MERGED_COMPILE == STD_OFF)
#define FRIF_STOP_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */
#endif /* TS_MERGED_COMPILE */

#endif /* FRIF_GETSYNCFRAMELIST_API_ENABLE */
#endif /* FRIF_FR_INTEGRATION_ENABLE */

