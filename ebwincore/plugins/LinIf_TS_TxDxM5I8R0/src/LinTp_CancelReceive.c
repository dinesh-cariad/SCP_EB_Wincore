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

/*==================[inclusions]=============================================*/

#include <LinIf_Trace.h>

#include <LinIf_Cfg.h>        /* Generated configuration */
#include <TSAutosar.h>        /* EB specific standard types */

#if (LINIF_TP_SUPPORTED == STD_ON)

#define LINIF_NO_PBCFG_REQUIRED
#include <LinIf.h>            /* Module library API */
#include <LinIf_Int.h>        /* LinIf internal data */
#include <LinTp_Int.h>        /* LinTp internal data */

#if ((LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON))
#include <Det.h>              /* Default Error Tracer */
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON) */
#include <SchM_LinIf.h>            /* for protecting exclusive areas */

/*==================[external function definitions]==========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - start section for code */
#define LINIF_START_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

/** \brief Cancel receive request */
FUNC(Std_ReturnType, LINIF_CODE) LinTp_CancelReceive
(
    PduIdType LinTpRxSduId
)
{
    Std_ReturnType result = E_NOT_OK;

    DBG_LINTP_CANCELRECEIVE_ENTRY(LinTpRxSduId);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    /* check DET errors first */
    if(LINTP_UNINIT_ACCESS)
    {
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_E_UNINIT,1 */
        (void)Det_ReportError( LINIF_MODULE_ID,
                               LINIF_INSTANCE_ID,
                               LINTP_API_CANCELRXREQUEST,
                               LINIF_E_UNINIT
                             );
    }

    /* check for valid N-SDU ID */
    else if( LinTpRxSduId >= LinTp_ConfigPtr->LinTpNumberOfRxNSdu )
    {
        (void)Det_ReportError( LINIF_MODULE_ID,
                               LINIF_INSTANCE_ID,
                               LINTP_API_CANCELRXREQUEST,
                               LINIF_E_PARAMETER
                             );
    }

    else

#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */

    {

        const uint8 Channel =
            LINTP_CFG_GET_RXNSDU[LinTpRxSduId].LinIfChannel;

        CONSTP2VAR(LinTp_ChannelInfoType, AUTOMATIC, LINIF_VAR) pTpData =
            &LinTp_ChannelInfo[Channel];

        SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();
        {
            const PduIdType currentRxSduId = pTpData->PhysChannelInfo.SduId;
            uint8 ChannelFlags;
            boolean CancelReceiveFlag, CancelReceiveAllowed;

            TS_AtomicAssign8(ChannelFlags, pTpData->ChannelFlags);
            CancelReceiveFlag = (ChannelFlags & LINTP_FLAG_CANCEL_RECEIVE_REQUESTED) ==
                LINTP_FLAG_CANCEL_RECEIVE_REQUESTED;
            CancelReceiveAllowed = (ChannelFlags & LINTP_FLAG_CANCEL_RECEIVE_ALLOWED) ==
                LINTP_FLAG_CANCEL_RECEIVE_ALLOWED;;

            /* Check if reception is ongoing and if requested cancel
               sdu equals the sdu of the current reception*/
            if((CancelReceiveAllowed && (!CancelReceiveFlag)) &&
               (currentRxSduId == LinTpRxSduId)
              )
            {
                /* indicates the main function to cancel a reception */
                ChannelFlags |= LINTP_FLAG_CANCEL_RECEIVE_REQUESTED;
                TS_AtomicAssign8(pTpData->ChannelFlags, ChannelFlags);
                result = E_OK;
            }
        }
        SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();
    }

    DBG_LINTP_CANCELRECEIVE_EXIT(result,LinTpRxSduId);

    return result;
}

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - end section for code */
#define LINIF_STOP_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

#else /* (LINIF_TP_SUPPORTED == STD_ON) */

/* prevent empty translation unit in case LinIf_Merged.c is not used */
#if (TS_MERGED_COMPILE == STD_OFF)

#include <TSCompiler.h>
TS_PREVENTEMPTYTRANSLATIONUNIT

#endif /* TS_MERGED_COMPILE */

#endif /* (LINIF_TP_SUPPORTED == STD_ON) */
/*==================[end of file]============================================*/
