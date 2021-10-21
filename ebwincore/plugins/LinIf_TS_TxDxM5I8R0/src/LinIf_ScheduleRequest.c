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
/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00858,1 */
#if (LINIF_MASTER_SUPPORTED == STD_ON)
#include <LinIf_Int.h>        /* LinIf internal data */
#include <SchM_LinIf.h>       /* for protecting exclusive areas */
#include <LinSM_Cbk.h>        /* LinSM callback functions */

#if ((LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON))
#include <Det.h>              /* Default Error Tracer */
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) || (LINIF_RUNTIME_ERROR_REPORTING == STD_ON) */

/*==================[external function definitions]==========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - start section for code */
#define LINIF_START_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */

/** \brief Request schedule table for execution
 **
 ** This function schedules a schedule table for execution. Note that when
 ** the NULL_SCHEDULE is requested, all previous requests are deleted.
 **
 ** \param[in] Channel The LinIf channel to operate on.
 ** \param[in] ScheduleTable The Id of the schedule requested.
 **
 */
FUNC(Std_ReturnType, LINIF_CODE) LinIf_ScheduleRequest
(
    NetworkHandleType Channel,
    LinIf_SchHandleType ScheduleTable
)
{
    uint16 TableIndex;
    Std_ReturnType RetVal = E_NOT_OK;

    DBG_LINIF_SCHEDULEREQUEST_ENTRY(Channel,ScheduleTable);

#if (LINIF_DEV_ERROR_DETECT == STD_ON)
    /* check DET errors first */
    if (LINIF_UNINIT_ACCESS)
    {
        /* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_E_UNINIT,1 */
        (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                  LINIF_API_SCHEDULEREQUEST, LINIF_E_UNINIT);
    }
    else if ( Channel >= LinIf_ConfigPtr->NumChannelsMaster )
    {
        (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                  LINIF_API_SCHEDULEREQUEST, LINIF_E_NONEXISTENT_CHANNEL);
    }
    else
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
    {
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
        uint8 LinIfState;

        TS_AtomicAssign8(LinIfState, LinIf_ChannelInfo[Channel].cFSM );

        if ( LinIfState != LINIF_CHANNEL_OPERATIONAL)
        {
          (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
              LINIF_API_SCHEDULEREQUEST, LINIF_E_SCHEDULE_REQUEST_ERROR);
        }
        else
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
        {
#if (LINIF_DEV_ERROR_DETECT == STD_ON)
            const uint8 NumChannels = LinIf_ConfigPtr->NumChannelsMaster;
            const uint16 NumSchTables = LinIf_ConfigPtr->NumSchTables;
            boolean ReportDet = FALSE;
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */

            TableIndex = LINIF_CFG_GET_CHANNEL_MASTER[Channel].SchTableInd +
                              (uint16) ScheduleTable;

#if (LINIF_DEV_ERROR_DETECT == STD_ON)

            /* check if schedule was requested for channel with highest index */
            if(Channel != (NumChannels - 1U))
            {
                /* verify that requested schedule belong to the channel */
                if (TableIndex >= LINIF_CFG_GET_CHANNEL_MASTER[Channel + 1U].SchTableInd)
                {
                    ReportDet = TRUE;
                }
            }
            else
            {
                /* verify that requested schedule belong to the channel */
                if(TableIndex >= NumSchTables)
                {
                    ReportDet = TRUE;
                }
            }

            /* report Det error if invalid schedule was selected */
            if (ReportDet == TRUE)
            {
                (void)Det_ReportError(LINIF_MODULE_ID, LINIF_INSTANCE_ID,
                          LINIF_API_SCHEDULEREQUEST, LINIF_E_SCHEDULE_REQUEST_ERROR);
            }

            else
#endif /* (LINIF_DEV_ERROR_DETECT == STD_ON) */
            {
                LinIf_InternalScheduleRequest(Channel, TableIndex);
                RetVal = E_OK;
            }
        }
    }

    DBG_LINIF_SCHEDULEREQUEST_EXIT(RetVal,Channel,ScheduleTable);

    return RetVal;
}


/** \brief This function handles new schedule request from internal
 *  or upper layers
 *
 *  \param internalChannel  The LinIf channel where the schedule request should
 *                          be operated
 *  \param TableIndex       Schedule table index, indicates which schedule
 *                          should be selected
 *
 */
FUNC(void, LINIF_CODE) LinIf_InternalScheduleRequest
(
    uint8 internalChannel,
    uint16 TableIndex
)
{
    CONSTP2VAR(LinIf_MasterChannelInfoType, AUTOMATIC, LINIF_VAR) pIfChannelMaster =
        &LinIf_MasterChannelInfo[internalChannel];

    if ( LINIF_CFG_GET_SCHTABLE[TableIndex].RunMode != LINIF_RUN_ONCE )
    {
        /* enter critical region */
        SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

        if ( TableIndex ==  LINIF_CFG_GET_CHANNEL_MASTER[internalChannel].SchTableInd )
        {
            /* force rescheduling to also abort a running RUN_ONCE table */
            pIfChannelMaster->ForceReschedule = TRUE;
            /* Reset the variable for the next call */
            pIfChannelMaster->SchROTable      = LINIF_NULL_SCHEDULE;
        }
        /* if the old continuous table is replaced ... */
        if( pIfChannelMaster->SchContTable != TableIndex )
        {
          /* ... reset resume point to start of the new table */
          pIfChannelMaster->ResumeSchEntry = LINIF_CFG_GET_SCHTABLE[TableIndex].EntryStartInd;

          pIfChannelMaster->SchContTable = TableIndex;
        }

        if( pIfChannelMaster->SchCurrentTable == TableIndex )
        {
          /* schedule already running, request restart of the table */
          pIfChannelMaster->RestartCurrentTable = TRUE;

          pIfChannelMaster->SchContTable = TableIndex;
        }

        pIfChannelMaster->ResumeSch = FALSE;

        /* leave critical region */
        SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();
    }
    else
    {
        /* enter critical region */
        SchM_Enter_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();

        pIfChannelMaster->ResumeSch = FALSE;

        /* store the TableIndex to be used in the LinIf_Schedule*/
        pIfChannelMaster->SchROTable = TableIndex;
        /* leave critical region */
        SchM_Exit_LinIf_SCHM_LINIF_EXCLUSIVE_AREA_0();
    }
}


#if (TS_MERGED_COMPILE == STD_OFF)
/* AUTOSAR Memory Mapping - end section for code */
#define LINIF_STOP_SEC_CODE
#include <LinIf_MemMap.h>
#endif /* TS_MERGED_COMPILE */
#else
#include <TSCompiler.h>
TS_PREVENTEMPTYTRANSLATIONUNIT
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

/*==================[end of file]============================================*/
