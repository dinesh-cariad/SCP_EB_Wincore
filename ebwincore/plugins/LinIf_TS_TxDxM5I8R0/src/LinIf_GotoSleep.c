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
#include <SchM_LinIf.h>            /* for protecting exclusive areas */

#if ((LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON))
#include <Det.h>              /* Default Error Tracer */
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON) */

/*==================[external function definitions]==========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - start section for code */
#define LINIF_START_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE  */

/** \brief Set channel to sleep
 **
 ** This function schedules a sleep request for execution. The channel will
 ** not enter sleep mode before the next schedule entry is due.
 **
 ** \param[in] Channel The LinIf channel to operate on.
 */
FUNC(Std_ReturnType, LINIF_CODE) LinIf_GotoSleep
(
    NetworkHandleType Channel
)
{
    Std_ReturnType retval = E_NOT_OK;

    DBG_LINIF_GOTOSLEEP_ENTRY(Channel);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    /* check DET errors first */
    if (LINIF_UNINIT_ACCESS)
    {
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_E_UNINIT,1 */
        (void)Det_ReportError(LINIF_MODULE_ID,
                              LINIF_INSTANCE_ID,
                              LINIF_API_GOTOSLEEP,
                              LINIF_E_UNINIT
                             );
    }
    else if ( Channel >= (LinIf_ConfigPtr->NumChannelsMaster + LinIf_ConfigPtr->NumSlaveChannels) )
    {
        (void)Det_ReportError(LINIF_MODULE_ID,
                              LINIF_INSTANCE_ID,
                              LINIF_API_GOTOSLEEP,
                              LINIF_E_NONEXISTENT_CHANNEL
                             );
    }
    else
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */

    {
        CONSTP2VAR(LinIf_ChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannel =
                &LinIf_ChannelInfo[Channel];

        uint8 SleepFlags;

        /* Lin_GoToSleep() is not called before the next entry is scheduled */
        /* enter critical region */
        SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();
        {
            TS_AtomicAssign8(SleepFlags, pIfChannel->SleepHandling);

            /* !LINKSTO LinIf.EB.GotoSleepDuringWakeup, 2*/
            /* !LINKSTO LinIf.EB.GotoSleepDuringWakeupInternal, 1*/
            /* sleep requests are only allowed if currently no other Sleep/Wakeup
               request is ongoing, except an internal Wakeup*/
            if(SleepFlags == LINIF_NO_SLEEP_REQUEST)
            {
#if(LINIF_SLAVE_SUPPORTED == STD_ON)
#if(LINIF_MASTER_SUPPORTED == STD_ON)
                if(Channel >= LinIf_ConfigPtr->NumChannelsMaster)
                {
                    /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00760_1,1 */
                    SleepFlags |= LINIF_SLEEP_ONGOING_FLAG;
                    TS_AtomicAssign8(pIfChannel->SleepHandling, SleepFlags);
                }
                else
                {
                    /* set a flag, goto sleep will be sent if next frame is due */
                    SleepFlags |= LINIF_SLEEP_REQUESTED_FLAG;
                    TS_AtomicAssign8(pIfChannel->SleepHandling, SleepFlags);
                }
#else /* LINIF_MASTER_SUPPORTED == STD_OFF */
                /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00760_1,1 */
                SleepFlags |= LINIF_SLEEP_ONGOING_FLAG;
                TS_AtomicAssign8(pIfChannel->SleepHandling, SleepFlags);
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */
#else /* LINIF_SLAVE_SUPPORTED == STD_OFF */
                /* set a flag, goto sleep will be sent if next frame is due */
                SleepFlags |= LINIF_SLEEP_REQUESTED_FLAG;
                TS_AtomicAssign8(pIfChannel->SleepHandling, SleepFlags);
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */
                retval = E_OK;
            }
        }
        /* leave critical region */
        SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

#if (LINIF_SLAVE_SUPPORTED == STD_ON)
#if(LINIF_MASTER_SUPPORTED == STD_ON)
        if((E_OK == retval) && (Channel >= LinIf_ConfigPtr->NumChannelsMaster))
#else
        if(E_OK == retval)
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */
        {
            /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00757,1 */
            retval = LinIf_LinDriverConfig[LINIF_GET_LIN_DRIVER_INDEX(Channel)].goToSleepInt(LINIF_GET_LIN_CHANNEL(Channel));

            /* !LINKSTO LinIf.EB.Slave.GoToSleepInternal.E_NOT_OK,1 */
            if(E_OK != retval)
            {
                SleepFlags &= ~(uint8)LINIF_SLEEP_ONGOING_FLAG;
                TS_AtomicAssign8(pIfChannel->SleepHandling, SleepFlags);
            }
            else
            {
              /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00758,1 */
              TS_AtomicAssign8(pIfChannel->SleepHandling, SleepFlags & (~(uint8)LINIF_WAKEUP_INTERNAL_ONGOING_FLAG));
            }
        }
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */
    }

    DBG_LINIF_GOTOSLEEP_EXIT(retval,Channel);

    return retval;
}

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - end section for code */
#define LINIF_STOP_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

/*==================[end of file]============================================*/
