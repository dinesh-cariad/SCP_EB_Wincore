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

#define LINIF_NO_PBCFG_REQUIRED
#include <LinIf.h>            /* Module library API */
#include <LinIf_Int.h>        /* LinIf internal data */

#if ((LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON))
#include <Det.h>              /* Default Error Tracer */
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON) */

/*==================[external function definitions]==========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - start section for code */
#define LINIF_START_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

/** \brief Schedule transmission of a sporadic frame.
 **
 ** This function schedules sporadic frames for transmission.
 **
 ** \param[in] LinTxPduId The PDU Id of the sporadic frame.
 ** \param[in] PduInfoPtr Not used.
 **
 */
FUNC(Std_ReturnType, LINIF_CODE) LinIf_Transmit
(
    PduIdType LinTxPduId,
    P2CONST(PduInfoType, AUTOMATIC, LINIF_APPL_DATA) PduInfoPtr
)
{
    Std_ReturnType retValue = E_NOT_OK;

    DBG_LINIF_TRANSMIT_ENTRY(LinTxPduId,PduInfoPtr);

    /* Parameter PduInfoPtr is not used (only within parameter check,
       when DET is enabled) */
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    /* check DET errors first */

    if (LINIF_UNINIT_ACCESS)
    {
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_E_UNINIT,1 */
        (void)Det_ReportError(LINIF_MODULE_ID,
                              LINIF_INSTANCE_ID,
                              LINIF_API_TRANSMIT,
                              LINIF_E_UNINIT
                             );
    }
    else if ( PduInfoPtr == NULL_PTR )
    {
        (void)Det_ReportError(LINIF_MODULE_ID,
                              LINIF_INSTANCE_ID,
                              LINIF_API_TRANSMIT,
                              LINIF_E_PARAMETER_POINTER
                             );
    }
    else if (LinTxPduId >= LinIf_ConfigPtr->NumTxPdus)
    {
        (void)Det_ReportError(LINIF_MODULE_ID,
                              LINIF_INSTANCE_ID,
                              LINIF_API_TRANSMIT,
                              LINIF_E_PARAMETER
                             );
    }
    /* Note: there is no check whether the frame requested
             is actually a sporadic frame (master)/event triggered(slave) or not.
             This has 2 reasons:
             1. It depends on the schedule table currently running
             2. In gateway mode LinIf_Transmit is called from PduR for
                every frame, because PduR cannot know which frames are
                sporadic/event triggered and which are not. */
    else
#else
    TS_PARAM_UNUSED(PduInfoPtr);
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */

    {
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
        LINIF_PRECONDITION_ASSERT( (LINIF_MAX_PDUS_FLAGS * 8U) > LinTxPduId, LINIF_API_TRANSMIT);
#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */
      {
        CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
              &LinIf_ChannelInfo[(LINIF_CFG_GET_TXFRAME[LinTxPduId].LinIfChannelId)];

        /* !LINKSTO LinIf.EB.LinIf_Transmit.CHANNEL_NOT_OPERATIONAL,1 */
        if(pIfChannel->cFSM == LINIF_CHANNEL_OPERATIONAL)
        {
          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00106.Master,1,
                      LinIf.ASR20-11.SWS_LinIf_00106.Slave,1,
                      LinIf.ASR20-11.SWS_LinIf_00341.Master,1,
                      LinIf.ASR20-11.SWS_LinIf_00341.Slave,1
           */
          /* now set event flag */
          LINIF_SET_BIT_ATOMIC(LinIf_FrameEventFlags,
              (PduIdType)(LinTxPduId));

          /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00700.Master,1,
                      LinIf.ASR20-11.SWS_LinIf_00700.Slave,1
           */
          retValue = E_OK;
        }
      }
    }

    DBG_LINIF_TRANSMIT_EXIT(retValue,LinTxPduId,PduInfoPtr);

    return retValue;
}

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - end section for code */
#define LINIF_STOP_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

/*==================[end of file]============================================*/
