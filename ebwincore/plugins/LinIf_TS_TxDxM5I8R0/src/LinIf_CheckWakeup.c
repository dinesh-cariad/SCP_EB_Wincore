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
#include <SchM_LinIf.h>       /* for protecting exclusive areas */

#if ((LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON))
#include <Det.h>              /* Default Error Tracer */
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON) */

#ifdef LINIF_WAKEUP_REV2_NOT_USED
#include <EcuM.h>            /* EcuM_WakeupSourceType */
#endif /* LINIF_WAKEUP_REV2_NOT_USED */

/*==================[external function definitions]==========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - start section for code */
#define LINIF_START_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

#if (LINIF_CHECKWAKEUP_SUPPORTED == STD_ON)
#ifndef LINIF_USE_LINAPI_REV2
/*==================[AUTOSAR 4.0 REV3 IMPLEMENTATION]=========================*/
FUNC(Std_ReturnType, LINIF_CODE) LinIf_CheckWakeup
(
   EcuM_WakeupSourceType WakeupSource
)
{
    Std_ReturnType retVal = E_NOT_OK;

    DBG_LINIF_CHECKWAKEUP_ENTRY(WakeupSource);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    if(LINIF_UNINIT_ACCESS)
    {
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_E_UNINIT,1 */
        (void)Det_ReportError(LINIF_MODULE_ID,
                              LINIF_INSTANCE_ID,
                              LINIF_API_CHECKWAKEUP,
                              LINIF_E_UNINIT
                             );
    }
    else
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
    {
        NetworkHandleType channelIdx;

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
      boolean invalidWakeupSource = TRUE;
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */

        /* iterate over all LIN channels */
        for( channelIdx = 0U ; channelIdx < LinIf_ConfigPtr->NumChannelsMaster ; channelIdx++ )
        {
            /* check if a valid EcuMWakeupSourceId is defined for this channel */
            if( LINIF_INVALIDECUMWAKEUPSOURCEID != LINIF_CFG_GET_CHANNEL[channelIdx].LinWakeupSourceId )
            {
                const uint32 WakeupSourceMask = (uint32)((uint32)1UL << LINIF_CFG_GET_CHANNEL[channelIdx].LinWakeupSourceId);

                if( (WakeupSource & WakeupSourceMask) == WakeupSourceMask )
                {
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
                    invalidWakeupSource = FALSE;
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
                    if( E_OK == LinIf_LinDriverConfig[LINIF_GET_LIN_DRIVER_INDEX(channelIdx)].checkWakeup(LINIF_GET_LIN_CHANNEL(channelIdx)) )
                    {

/* !LINKSTO LinIf.EB.LinIf_CheckWakeup.NOT_OPERATIONAL.42DRV,1 */
/* !LINKSTO LinIf.EB.LinIf_CheckWakeup.NOT_OPERATIONAL.40DRV,1 */
#ifndef LINIF_WAKEUP_LINAPI_42
                        DBG_LINIF_CFSM_GRP(Channel,
                                           (LinIf_ChannelInfo[channelIdx].cFSM),
                                           (LINIF_CHANNEL_OPERATIONAL)
                                          );

                        TS_AtomicAssign8(LinIf_ChannelInfo[channelIdx].cFSM, LINIF_CHANNEL_OPERATIONAL);
#endif /* LINIF_WAKEUP_LINAPI_42 */

                        /* at least one Lin_CheckWakeup returned E_OK */
                        retVal = E_OK;
                    }
                }
            }

#if (LINIF_TRCV_SUPPORTED == STD_ON)
            if( LINIF_INVALIDECUMWAKEUPSOURCEID != LINIF_GET_TRCV_WUSID(channelIdx) )
            {
              const uint32 WakeupSourceMask = (uint32)((uint32)1UL << LINIF_GET_TRCV_WUSID(channelIdx));

              if( (WakeupSource & WakeupSourceMask) == WakeupSourceMask )
              {
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
                invalidWakeupSource = FALSE;
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */

                retVal = LinIf_TrcvFuncPtrs[LINIF_GET_TRCV_FUNCIDX(channelIdx)].CheckWakeup(LINIF_GET_TRCV_CHID(channelIdx));
              }
            }
#endif
        } /* end loop */

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
        /* in case the given WakeupSource does not match any of the possible Lin WakeupSources,
           report to LINIF_E_PARAM_WAKEUPSOURCE DET */
        if(invalidWakeupSource)
        {
            (void)Det_ReportError(LINIF_MODULE_ID,
                                  LINIF_INSTANCE_ID,
                                  LINIF_API_CHECKWAKEUP,
                                  LINIF_E_PARAM_WAKEUPSOURCE
                                 );
        }
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */

    }

    DBG_LINIF_CHECKWAKEUP_EXIT(retVal, WakeupSource);

    return retVal;
}
#else /* ifndef LINIF_USE_LINAPI_REV2 */
/*==================[AUTOSAR 4.0 REV2 IMPLEMENTATION]=========================*/
/** \brief Check wakeup function */
FUNC(void, LINIF_CODE) LinIf_CheckWakeup
(
    NetworkHandleType Channel
)
{
    DBG_LINIF_CHECKWAKEUP_ENTRY(Channel);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    /* check DET errors first */
    if (LINIF_UNINIT_ACCESS)
    {
        (void)Det_ReportError(LINIF_MODULE_ID,
                              LINIF_INSTANCE_ID,
                              LINIF_API_CHECKWAKEUP,
                              LINIF_E_UNINIT
                             );
    }
    else
    {
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */

      Channel = LINIF_TRANSLATE_COMM_CHANNEL(Channel);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
      if ( Channel >= LinIf_ConfigPtr->NumChannelsMaster )
      {
          (void)Det_ReportError(LINIF_MODULE_ID,
                                LINIF_INSTANCE_ID,
                                LINIF_API_CHECKWAKEUP,
                                LINIF_E_NONEXISTENT_CHANNEL
                               );
      }
      else
      {
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */

          /* call Lin wakeup validation function */
          LinIf_LinDriverConfig[LINIF_GET_LIN_DRIVER_INDEX(Channel)].wakeupValidation();

          DBG_LINIF_CFSM_GRP(Channel,
                             (LinIf_ChannelInfo[Channel].cFSM),
                             (LINIF_CHANNEL_OPERATIONAL)
                            );

          TS_AtomicAssign8(LinIf_ChannelInfo[Channel].cFSM, LINIF_CHANNEL_OPERATIONAL);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
      }
    }
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */

    DBG_LINIF_CHECKWAKEUP_EXIT(Channel);
}
#endif /* LINIF_USE_LINAPI_REV2 == STD_OFF */
#endif /* LINIF_CHECKWAKEUP_SUPPORTED == STD_ON */

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - end section for code */
#define LINIF_STOP_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

/*==================[end of file]============================================*/
