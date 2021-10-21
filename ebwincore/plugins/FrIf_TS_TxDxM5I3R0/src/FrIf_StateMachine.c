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

/*
* Misra-C:2012 Deviations:
*
* MISRAC2012-1) Deviated Rule: 11.5 (required)
* A conversion should not be performed from pointer to void into pointer to object.
*
* Reason:
* For efficiency reasons a uint32 array is allocated while certain operatioons access
* it byte-wise. In these cases there is a cast to void* in order to
* satisfy the compilers.
*
*/

/*======================[Include Section]=====================================*/

#include <TSAtomic_Assign.h>
#include <TSAutosar.h>
#include <SchM_FrIf.h>
#include <FrIf_guards.h> /* Include define guards */
#include <FrIf_Trace.h>
#include <FrIf_Priv.h>

/*======================[Local Macros]========================================*/

/*======================[Local Types]=========================================*/

/*======================[Local Data]==========================================*/

#if (TS_MERGED_COMPILE == STD_OFF)
#define FRIF_START_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */
#endif /* TS_MERGED_COMPILE */

/*======================[Local Functions]=====================================*/

/**
 * \brief  This API services for the initial JobList-Entry to execute
 *         and programs the related absolute timer.
 *
 * \param FrIf_ClstIdx (in)       FlexRay cluster index.
 * \param FrIf_CtrlIdx (in)       FlexRay controller index.
 * \param ClstPtr (in)            Cluster configuration pointer.
 *
 */
STATIC FUNC(void,FRIF_CODE) FrIf_SyncJobList
    (
        uint8 FrIf_ClstIdx,
        uint8 FrIf_CtrlIdx,
        P2CONST(FrIf_ClstType,AUTOMATIC, FRIF_APPL_CONST) ClstPtr,
        uint8 CurrentCycle,
        uint16 CurrentOffset
    );


/**
 * \brief  This API services resets all Tx flags to it's initial value
 *  (no transmission requests pending, no confirmations pending).
 *
 * \param FrIf_ClstIdx (in)       FlexRay cluster index.
 *
 */
STATIC FUNC(void,FRIF_CODE) FrIf_ResetTxFlags
    (
        uint8 FrIf_ClstIdx
    );

#if (FRIF_DECOUPLED_RX_ENABLE == STD_ON)

/**
 * \brief  This API services resets all Rx flags to it's initial value
 *  (no valid received (non indicated) frame available).
 *
 * \param FrIf_ClstIdx (in)       FlexRay cluster index.
 *
 */
STATIC FUNC(void,FRIF_CODE) FrIf_ResetRxFlags
    (
        uint8 FrIf_ClstIdx
    );

#endif /* FRIF_DECOUPLED_RX_ENABLE */

#if ( (FRIF_CHANNEL_STATUS_REPORTING == STD_ON) || (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON) )
/**
 * \brief  This API services checks the channel status information of the specified controller
 *         and reports to Dem, Det or Mirror.
 *
 * \param FrIf_ClstIdx (in)       FlexRay cluster index.
 * \param FrIf_CtrlIdx (in)       FlexRay controller index.
 *
 */
STATIC FUNC(void,FRIF_CODE) FrIf_ReportChannelStatus
(
    uint8 FrIf_ClstIdx
);
#endif /* (FRIF_CHANNEL_STATUS_REPORTING == STD_ON) || (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON) */

STATIC FUNC(void,FRIF_CODE) FrIf_ResetTxFlags
    (
        uint8 FrIf_ClstIdx
    )
{

#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF)

    /* get a cluster configuration pointer */
    CONSTP2CONST(FrIf_ClstType,AUTOMATIC, FRIF_APPL_CONST) ClstPtr =
        &FRIF_GET_CONFIG_ADDR(FrIf_ClstType, FrIf_RootConfigPtr->FrIf_ClstRef)[FRIF_CLSTIDX];

    /* get pointer to list of Tx-Pdu indices */
    CONSTP2CONST(FrIf_PduIdxType,AUTOMATIC, FRIF_APPL_CONST) TxPduIdxPtr =
        FRIF_GET_CONFIG_ADDR(FrIf_PduIdxType, ClstPtr->ClstTxPduIdxRef);

#endif /* FRIF_SINGLE_CLST_OPT_ENABLE */

    /* create Pdu iterator variable */
    uint16_least clstPduNum = FRIF_GET_CLSTPDUNUM(ClstPtr);

    /* create Pdu iterator variable */
    uint16_least iPdu;

#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_ON)

    TS_PARAM_UNUSED(FrIf_ClstIdx);

#endif /* FRIF_SINGLE_CLST_OPT_ENABLE */

    /* iterate over Pdus */
    for(iPdu = 0U; iPdu < clstPduNum; iPdu++)
    {

        /* clear Tx pending flag and TxCounter */
#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF)

        /* Deviation MISRAC2012-1 */
        ((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[TxPduIdxPtr[iPdu]] = 0x00U;

#else /* FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFN */

        /* Deviation MISRAC2012-1 */
        ((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[iPdu] = 0x00U;

#endif /* FRIF_SINGLE_CLST_OPT_ENABLE */

    }
}

#if (FRIF_DECOUPLED_RX_ENABLE == STD_ON)

STATIC FUNC(void,FRIF_CODE) FrIf_ResetRxFlags
    (
        uint8 FrIf_ClstIdx
    )
{
    /* get a cluster configuration pointer */
    CONSTP2CONST(FrIf_ClstType,AUTOMATIC, FRIF_APPL_CONST) ClstPtr =
        &FRIF_GET_CONFIG_ADDR(FrIf_ClstType, FrIf_RootConfigPtr->FrIf_ClstRef)[FRIF_CLSTIDX];

    /* get pointer to list of Rx-LPdu PB-configuration references */
    CONSTP2CONST(FrIf_DecRxLPduRef2CfgType,AUTOMATIC,FRIF_APPL_CONST) ClstDecRxLPduRef =
        FRIF_GET_CONFIG_ADDR(FrIf_DecRxLPduRef2CfgType, ClstPtr->ClstDecRxLPduRefRef);

    /* create Pdu iterator variable */
    uint16_least iPdu;

#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_ON)

    TS_PARAM_UNUSED(FrIf_ClstIdx);

#endif /* FRIF_SINGLE_CLST_OPT_ENABLE */

    /* iterate over Pdus */
    for(iPdu = 0U; iPdu < (uint16_least)ClstPtr->ClstDecRxLPduNum; iPdu++)
    {
        /* obtain pointer to Pdu configuration */
        CONSTP2CONST(FrIf_DecRxLPduType,AUTOMATIC, FRIF_APPL_CONST) RxPduPtr =
            FRIF_GET_CONFIG_ADDR(FrIf_DecRxLPduType, ClstDecRxLPduRef[iPdu]);

        /* clear valid date field for decoupled Rx Pdus */
        /* Deviation MISRAC2012-1 */
        ((P2VAR(uint8,AUTOMATIC,FRIF_VAR))(P2VAR(void,AUTOMATIC,FRIF_VAR)) FrIf_Mem)[RxPduPtr->PBRamBufRef] = 0x00U;
    }
}

#endif /* FRIF_DECOUPLED_RX_ENABLE */

STATIC FUNC(void,FRIF_CODE) FrIf_SyncJobList
    (
        uint8 FrIf_ClstIdx,
        uint8 FrIf_CtrlIdx,
        P2CONST(FrIf_ClstType,AUTOMATIC,FRIF_APPL_CONST) ClstPtr,
        uint8 CurrentCycle,
        uint16 CurrentOffset
    )
{
   /* get absolute timer index for joblist execution */
    const uint8 FrIfAbsTimerIdx = ClstPtr->JLE_FrIf_AbsTimerIdx;

    /* get Number of Jobs */
    const uint16_least NumJobs = ClstPtr->JobNum;

#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_ON)

    TS_PARAM_UNUSED(FrIf_ClstIdx);

#endif /* FRIF_SINGLE_CLST_OPT_ENABLE */

#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_ON)

    TS_PARAM_UNUSED(FrIf_CtrlIdx);

#endif /* FRIF_SINGLE_CTRL_OPT_ENABLE */

    if(NumJobs > 0U)
    {
        uint16_least iJob;

        /* get pointer to start of JobList */
        CONSTP2CONST(FrIf_JobType,AUTOMATIC, FRIF_APPL_CONST) JobStartPtr =
            FRIF_GET_CONFIG_ADDR(FrIf_JobType, ClstPtr->JobRef);

        /* get Joblist Repetition Mask */
        const uint8 JobListRepetition = ClstPtr->JobListCycleRepetition;

        /* get Joblist Repetition Mask */
        const uint8 JobListRepetitionMask = JobListRepetition - 1U;

        /* get FrIfSafetyMargin (in MacroTicks) */
        const uint16 SafetyMargin = ClstPtr->SafetyMargin;

        /* get number of MacroTicks per Cycle */
        const uint16 MTperCycle =  FRIF_GET_CONFIG_ADDR(FrIf_CtrlType,
            FrIf_RootConfigPtr->FrIf_CtrlRef)[FRIF_CTRLIDX].MTperCycle;

        /* variables used to calculate next save job execution */
        uint8 SafetyCurrentCycle = CurrentCycle;
        uint16 SafetyCurrentOffset;
        uint8 CycleBase;
        uint8 CycleOffset;

        /* if the SafetyMargin exceeds the current cycle */
        if( (CurrentOffset + SafetyMargin) >= MTperCycle )
        {
            /* wrap around to the next cycle */
            SafetyCurrentCycle = (SafetyCurrentCycle + 1U) & 0x3FU;

            /* and the calculate new cycle offset */
            SafetyCurrentOffset = ((CurrentOffset + SafetyMargin) - MTperCycle);
        }
        else
        {
            /* otherwise just add the safety margin to the current offset */
            SafetyCurrentOffset = CurrentOffset + SafetyMargin;
        }

        /* calculate the Cycle base */
        CycleBase = (SafetyCurrentCycle&(0xFFU^JobListRepetitionMask));

        /* calculate the Cycle offset */
        CycleOffset = (SafetyCurrentCycle&JobListRepetitionMask);

        /* search for the next appropriate job-list entry */
        for(iJob = 0U; iJob < NumJobs; iJob++)
        {
            if(((JobStartPtr[iJob].Cycle == CycleOffset) &&
                (JobStartPtr[iJob].Offset >= SafetyCurrentOffset)) ||
                (JobStartPtr[iJob].Cycle > CycleOffset))
            {
                /* found an valid entry */
                break;
            }
        }

        /* if we didn't find any appropriate entry in this cycle
         * set to first entry - wrap around!
         */
        if( iJob == NumJobs )
        {
            /* set to very first job-list entry */
            iJob = 0U;

            /* since we wrap around the table, we have to also wrap around the cycle base */
            CycleBase = (CycleBase + JobListRepetition) & 0x3FU;
        }

        /* store the job to execute next */
        FrIf_ClstRuntimeData[FRIF_CLSTIDX].NextJob = (uint16)iJob;

        /* store the cycle base for that job */
        FrIf_ClstRuntimeData[FRIF_CLSTIDX].JobListCycleBase = CycleBase;

        {
            /* get a pointer to the current Job */
            P2CONST(FrIf_JobType,AUTOMATIC,FRIF_APPL_CONST) JobPtr = &JobStartPtr[iJob];

            /* program absolute timer for initial job */
            (void)FrIf_SetAbsoluteTimer(FRIF_CTRLIDX, FrIfAbsTimerIdx, (uint8)(CycleBase + JobPtr->Cycle), JobPtr->Offset);
        }

        /* enable absolute timer interrupt */
        (void)FrIf_EnableAbsoluteTimerIRQ(FRIF_CTRLIDX, FrIfAbsTimerIdx);

    }

}


/**
 * \brief  Performs the MainFunction execution for a particular cluster
 *
 * \param FrIf_ClstIdx (in)       FlexRay cluster index.
 *
 */

FUNC(void,FRIF_CODE) FrIf_MainFunction
    (
        uint8 FrIf_ClstIdx
    )
{
    DBG_FRIF_MAINFUNCTION_ENTRY(FrIf_ClstIdx);

#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_ON)

    TS_PARAM_UNUSED(FrIf_ClstIdx);

#endif /* FRIF_SINGLE_CLST_OPT_ENABLE */

    /* check for successfully initialized module */
    if(FrIf_ModuleInitialized != FRIF_UNINIT)
    /* monitor joblist execution */
    {

        /* get cluster configuration pointer */
        CONSTP2CONST(FrIf_ClstType,AUTOMATIC, FRIF_APPL_CONST) ClstPtr =
            &FRIF_GET_CONFIG_ADDR(FrIf_ClstType,FrIf_RootConfigPtr->FrIf_ClstRef)[FRIF_CLSTIDX];

#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_OFF)

        /* get the controller index responsible for this cluster's joblist */
        const uint8 FrIf_CtrlIdx = ClstPtr->JLE_FrIf_CtrlIdx;

#endif /* FRIF_SINGLE_CTRL_OPT_ENABLE */

        /* temporary variable for evaluating whether controller driving the
           joblist got synchronized since last invocation */
        boolean TransitionToSync = FALSE;

        /* variables for getting FlexRay time */
        uint8 CurrentCycle;
        uint16 CurrentOffset;
        uint8 JobListTimeoutCounter;

        /* read JobListTimeoutCounter in an atomic fashion */
        TS_AtomicAssign8(JobListTimeoutCounter,FrIf_ClstRuntimeData[FRIF_CLSTIDX].JobListTimeoutCounter);

        /* report DEM-Error only in case of joblist-problems */
        if(JobListTimeoutCounter == ClstPtr->JobListTimeoutLimit)
        {
            /* forward counter to avoid another Dem reporting */
            /* no critical section requried, because concurrent access is seperated by
               previous state JobListTimeoutCounter */
            TS_AtomicAssign8(FrIf_ClstRuntimeData[FRIF_CLSTIDX].JobListTimeoutCounter,ClstPtr->JobListTimeoutLimit + 1U);

#if (FRIF_PROD_ERR_HANDLING_JLE_SYNC == TS_PROD_ERR_REP_TO_DEM)

            /* check if DEM reporting is enabled for this cluster */
            if (ClstPtr->DemEventId_JLE_SYNC != 0U)
            {
              /* Report a Joblist sync error to the DEM */
              Dem_ReportErrorStatus(ClstPtr->DemEventId_JLE_SYNC , FRIF_JLE_SYNC_FAILED);
            }

#elif (FRIF_PROD_ERR_HANDLING_JLE_SYNC == TS_PROD_ERR_REP_TO_DET)
#if (FRIF_REPORT_TO_DET_ENABLE == STD_ON)

            /* Report a Joblist sync error to the DET */
            (void)Det_ReportError( FRIF_MODULE_ID,
                                   FrIf_ClstIdx,
                                   FRIF_MAINFUNCTION_SERVICE_ID,
                                   FRIF_E_DEMTODET_JLE_SYNC);

#endif /* (FRIF_REPORT_TO_DET_ENABLE == STD_ON) */
#endif /* (FRIF_PROD_ERR_HANDLING_JLE_SYNC == TS_PROD_ERR_REP_TO_DET) */
        }

        /* read synchronization state of the controller */
        if( FrIf_GetGlobalTime(FRIF_CTRLIDX, &CurrentCycle, &CurrentOffset) == E_OK )
        {
            uint8 FrIf_SyncState;
            TS_AtomicAssign8(FrIf_SyncState,FrIf_ClstRuntimeData[FRIF_CLSTIDX].FrIf_SyncState);
            if(FrIf_SyncState == FRIF_SYNCSTATE_ASYNC)
            {
                /* remember that transistion */
                TransitionToSync = TRUE;
            }

            /* check if a Job list execution timeout occured OR a transition from ASYNC to SYNC */
            if((JobListTimeoutCounter >= ClstPtr->JobListTimeoutLimit) || (TransitionToSync != FALSE))
            {
                /* resync the joblist */
                FrIf_SyncJobList(FRIF_CLSTIDX,FRIF_CTRLIDX,ClstPtr,CurrentCycle,CurrentOffset);

                /* reset timeout counter */
                /* no critical section requried, because concurrent access is seperated by
                   previous state of JobListTimeoutCounter */
                TS_AtomicAssign8(FrIf_ClstRuntimeData[FRIF_CLSTIDX].JobListTimeoutCounter,0U);

                /* switch to new internal state - we are synchronized */
                DBG_FRIF_SYNCSTATE_GRP(FRIF_CLSTIDX, (FrIf_ClstRuntimeData[FRIF_CLSTIDX].FrIf_SyncState), (FRIF_SYNCSTATE_SYNC));
                TS_AtomicAssign8(FrIf_ClstRuntimeData[FRIF_CLSTIDX].FrIf_SyncState,FRIF_SYNCSTATE_SYNC);
            }
            else
            {
                SchM_Enter_FrIf_SCHM_FRIF_JOBLIST_EXECUTION();

                /* read again timout counter and increment within critical section */
                TS_AtomicAssign8(JobListTimeoutCounter, FrIf_ClstRuntimeData[FRIF_CLSTIDX].JobListTimeoutCounter);
                JobListTimeoutCounter++;
                TS_AtomicAssign8(FrIf_ClstRuntimeData[FRIF_CLSTIDX].JobListTimeoutCounter, JobListTimeoutCounter);

                SchM_Exit_FrIf_SCHM_FRIF_JOBLIST_EXECUTION();
            }

            /* check if the at least one channel status needs to be reported */
#if ( (FRIF_CHANNEL_STATUS_REPORTING == STD_ON) || (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON) )
            FrIf_ReportChannelStatus(FRIF_CLSTIDX);
#endif
        }
        else
        {
            DBG_FRIF_SYNCSTATE_GRP(FRIF_CLSTIDX, (FrIf_ClstRuntimeData[FRIF_CLSTIDX].FrIf_SyncState), (FRIF_SYNCSTATE_ASYNC));
            TS_AtomicAssign8(FrIf_ClstRuntimeData[FRIF_CLSTIDX].FrIf_SyncState,FRIF_SYNCSTATE_ASYNC);
        }
    }

    DBG_FRIF_MAINFUNCTION_EXIT(FrIf_ClstIdx);
}

#if (FRIF_CHANNEL_STATUS_REPORTING == STD_ON)

#if (FRIF_PROD_ERR_HANDLING_NIT_CH_A != TS_PROD_ERR_DISABLE)
STATIC FUNC(void,FRIF_CODE) FrIf_CheckReport_NIT_ChA
    (
        uint8 FrIf_ClstIdx,
        uint16 FrIf_ChannelStatus
    );

STATIC FUNC(void,FRIF_CODE) FrIf_CheckReport_NIT_ChA
    (
        uint8 FrIf_ClstIdx,
        uint16 FrIf_ChannelStatus
    )
{
#if (FRIF_PROD_ERR_HANDLING_NIT_CH_A == TS_PROD_ERR_REP_TO_DEM)
    /* get cluster configuration pointer */
    CONSTP2CONST(FrIf_ClstType,AUTOMATIC, FRIF_APPL_CONST) ClstPtr =
        &FRIF_GET_CONFIG_ADDR(FrIf_ClstType,FrIf_RootConfigPtr->FrIf_ClstRef)[FrIf_ClstIdx];
#elif (FRIF_PROD_ERR_HANDLING_NIT_CH_A == TS_PROD_ERR_REP_TO_DET)
#if (FRIF_REPORT_TO_DET_ENABLE != STD_ON)
    TS_PARAM_UNUSED(FrIf_ClstIdx);
#endif /* (FRIF_REPORT_TO_DET_ENABLE != STD_ON) */
#endif /* (FRIF_PROD_ERR_HANDLING_NIT_CH_A == TS_PROD_ERR_REP_TO_DEM) */

    /* --- FRIF_E_NIT_CH_A --- */
    if((FrIf_ChannelStatus & FRIF_ERRBITMASK_NIT_CH) != 0x0000U )
    {
#if (FRIF_PROD_ERR_HANDLING_NIT_CH_A == TS_PROD_ERR_REP_TO_DEM)

        /* check if DEM reporting is enabled for this cluster */
        if (ClstPtr->DemEventId_NIT_CH_A != 0U)
        {
          /* Report a NIT channel A error to the DEM */
          Dem_ReportErrorStatus(ClstPtr->DemEventId_NIT_CH_A, FRIF_NIT_CH_A_FAILED);
        }

#elif (FRIF_PROD_ERR_HANDLING_NIT_CH_A == TS_PROD_ERR_REP_TO_DET)
#if (FRIF_REPORT_TO_DET_ENABLE == STD_ON)

        /* Report a NIT channel A error to the DET */
        (void)Det_ReportError( FRIF_MODULE_ID,
                               FrIf_ClstIdx,
                               FRIF_MAINFUNCTION_SERVICE_ID,
                               FRIF_E_DEMTODET_NIT_CH_A);

#endif /* (FRIF_REPORT_TO_DET_ENABLE == STD_ON) */
#endif /* (FRIF_PROD_ERR_HANDLING_NIT_CH_A == TS_PROD_ERR_REP_TO_DET) */
    }
    else
    {
#if (FRIF_PROD_ERR_HANDLING_NIT_CH_A == TS_PROD_ERR_REP_TO_DEM)

        /* check if DEM reporting is enabled for this cluster */
        if (ClstPtr->DemEventId_NIT_CH_A != 0U)
        {
          /* Report NIT channel A status to the DEM */
          Dem_ReportErrorStatus(ClstPtr->DemEventId_NIT_CH_A, FRIF_NIT_CH_A_PASSED);
        }
#endif /* (FRIF_PROD_ERR_HANDLING_NIT_CH_A == TS_PROD_ERR_REP_TO_DEM) */
    }
}
#endif /* (FRIF_PROD_ERR_HANDLING_NIT_CH_A != TS_PROD_ERR_DISABLE) */

#if (FRIF_PROD_ERR_HANDLING_NIT_CH_B != TS_PROD_ERR_DISABLE)
STATIC FUNC(void,FRIF_CODE) FrIf_CheckReport_NIT_ChB
    (
        uint8 FrIf_ClstIdx,
        uint16 FrIf_ChannelStatus
    );
STATIC FUNC(void,FRIF_CODE) FrIf_CheckReport_NIT_ChB
    (
        uint8 FrIf_ClstIdx,
        uint16 FrIf_ChannelStatus
    )
{
#if (FRIF_PROD_ERR_HANDLING_NIT_CH_B == TS_PROD_ERR_REP_TO_DEM)
    /* get cluster configuration pointer */
    CONSTP2CONST(FrIf_ClstType,AUTOMATIC, FRIF_APPL_CONST) ClstPtr =
        &FRIF_GET_CONFIG_ADDR(FrIf_ClstType,FrIf_RootConfigPtr->FrIf_ClstRef)[FrIf_ClstIdx];
#elif (FRIF_PROD_ERR_HANDLING_NIT_CH_B == TS_PROD_ERR_REP_TO_DET)
#if (FRIF_REPORT_TO_DET_ENABLE != STD_ON)
    TS_PARAM_UNUSED(FrIf_ClstIdx);
#endif /* (FRIF_REPORT_TO_DET_ENABLE != STD_ON) */
#endif /* (FRIF_PROD_ERR_HANDLING_NIT_CH_B == TS_PROD_ERR_REP_TO_DEM) */

    /* --- FRIF_E_NIT_CH_B --- */
    if((FrIf_ChannelStatus & FRIF_ERRBITMASK_NIT_CH) != 0x0000U )
    {
#if (FRIF_PROD_ERR_HANDLING_NIT_CH_B == TS_PROD_ERR_REP_TO_DEM)

        /* check if DEM reporting is enabled for this cluster */
        if (ClstPtr->DemEventId_NIT_CH_B != 0U)
        {
          /* Report a NIT channel B error to the DEM */
          Dem_ReportErrorStatus(ClstPtr->DemEventId_NIT_CH_B, FRIF_NIT_CH_B_FAILED);
        }

#elif (FRIF_PROD_ERR_HANDLING_NIT_CH_B == TS_PROD_ERR_REP_TO_DET)
#if (FRIF_REPORT_TO_DET_ENABLE == STD_ON)

        /* Report a NIT channel B error to the DET */
        (void)Det_ReportError( FRIF_MODULE_ID,
                               FrIf_ClstIdx,
                               FRIF_MAINFUNCTION_SERVICE_ID,
                               FRIF_E_DEMTODET_NIT_CH_B);

#endif /* (FRIF_REPORT_TO_DET_ENABLE == STD_ON) */
#endif /* (FRIF_PROD_ERR_HANDLING_NIT_CH_B == TS_PROD_ERR_REP_TO_DET) */
    }
    else
    {
#if (FRIF_PROD_ERR_HANDLING_NIT_CH_B == TS_PROD_ERR_REP_TO_DEM)

        /* check if DEM reporting is enabled for this cluster */
        if (ClstPtr->DemEventId_NIT_CH_B != 0U)
        {
          /* Report NIT channel B status to the DEM */
          Dem_ReportErrorStatus(ClstPtr->DemEventId_NIT_CH_B, FRIF_NIT_CH_B_PASSED);
        }
#endif /* (FRIF_PROD_ERR_HANDLING_NIT_CH_B == TS_PROD_ERR_REP_TO_DEM) */
    }
}
#endif /* (FRIF_PROD_ERR_HANDLING_NIT_CH_B != TS_PROD_ERR_DISABLE) */

#if (FRIF_PROD_ERR_HANDLING_SW_CH_A != TS_PROD_ERR_DISABLE)
STATIC FUNC(void,FRIF_CODE) FrIf_CheckReport_SW_ChA
    (
        uint8 FrIf_ClstIdx,
        uint16 FrIf_ChannelStatus
    );

STATIC FUNC(void,FRIF_CODE) FrIf_CheckReport_SW_ChA
    (
        uint8 FrIf_ClstIdx,
        uint16 FrIf_ChannelStatus
    )
{
#if (FRIF_PROD_ERR_HANDLING_SW_CH_A == TS_PROD_ERR_REP_TO_DEM)
    /* get cluster configuration pointer */
    CONSTP2CONST(FrIf_ClstType,AUTOMATIC, FRIF_APPL_CONST) ClstPtr =
        &FRIF_GET_CONFIG_ADDR(FrIf_ClstType,FrIf_RootConfigPtr->FrIf_ClstRef)[FrIf_ClstIdx];
#elif (FRIF_PROD_ERR_HANDLING_SW_CH_A == TS_PROD_ERR_REP_TO_DET)
#if (FRIF_REPORT_TO_DET_ENABLE != STD_ON)
    TS_PARAM_UNUSED(FrIf_ClstIdx);
#endif /* (FRIF_REPORT_TO_DET_ENABLE != STD_ON) */
#endif /* (FRIF_PROD_ERR_HANDLING_SW_CH_A == TS_PROD_ERR_REP_TO_DEM) */

    /* --- FRIF_E_SW_CH_A --- */
    if((FrIf_ChannelStatus & FRIF_ERRBITMASK_SW_CH) != 0x0000U )
    {
#if (FRIF_PROD_ERR_HANDLING_SW_CH_A == TS_PROD_ERR_REP_TO_DEM)

        /* check if DEM reporting is enabled for this cluster */
        if (ClstPtr->DemEventId_SW_CH_A != 0U)
        {
          /* Report a SW channel A error to the DEM */
          Dem_ReportErrorStatus(ClstPtr->DemEventId_SW_CH_A, FRIF_SW_CH_A_FAILED);
        }

#elif (FRIF_PROD_ERR_HANDLING_SW_CH_A == TS_PROD_ERR_REP_TO_DET)
#if (FRIF_REPORT_TO_DET_ENABLE == STD_ON)

        /* Report a SW channel A error to the DET */
        (void)Det_ReportError( FRIF_MODULE_ID,
                               FrIf_ClstIdx,
                               FRIF_MAINFUNCTION_SERVICE_ID,
                               FRIF_E_DEMTODET_SW_CH_A);

#endif /* (FRIF_REPORT_TO_DET_ENABLE == STD_ON) */
#endif /* (FRIF_PROD_ERR_HANDLING_SW_CH_A == TS_PROD_ERR_REP_TO_DET) */
    }
    else
    {
#if (FRIF_PROD_ERR_HANDLING_SW_CH_A == TS_PROD_ERR_REP_TO_DEM)

        /* check if DEM reporting is enabled for this cluster */
        if (ClstPtr->DemEventId_SW_CH_A != 0U)
        {
          /* Report SW channel A status to the DEM */
          Dem_ReportErrorStatus(ClstPtr->DemEventId_SW_CH_A, FRIF_SW_CH_A_PASSED);
        }
#endif /* (FRIF_PROD_ERR_HANDLING_SW_CH_A == TS_PROD_ERR_REP_TO_DEM) */
    }

}
#endif /* (FRIF_PROD_ERR_HANDLING_SW_CH_A != TS_PROD_ERR_DISABLE) */

#if (FRIF_PROD_ERR_HANDLING_SW_CH_B != TS_PROD_ERR_DISABLE)
STATIC FUNC(void,FRIF_CODE) FrIf_CheckReport_SW_ChB
    (
        uint8 FrIf_ClstIdx,
        uint16 FrIf_ChannelStatus
    );

STATIC FUNC(void,FRIF_CODE) FrIf_CheckReport_SW_ChB
    (
        uint8 FrIf_ClstIdx,
        uint16 FrIf_ChannelStatus
    )
{
#if (FRIF_PROD_ERR_HANDLING_SW_CH_B == TS_PROD_ERR_REP_TO_DEM)
    /* get cluster configuration pointer */
    CONSTP2CONST(FrIf_ClstType,AUTOMATIC, FRIF_APPL_CONST) ClstPtr =
        &FRIF_GET_CONFIG_ADDR(FrIf_ClstType,FrIf_RootConfigPtr->FrIf_ClstRef)[FrIf_ClstIdx];
#elif (FRIF_PROD_ERR_HANDLING_SW_CH_B == TS_PROD_ERR_REP_TO_DET)
#if (FRIF_REPORT_TO_DET_ENABLE != STD_ON)
    TS_PARAM_UNUSED(FrIf_ClstIdx);
#endif /* (FRIF_REPORT_TO_DET_ENABLE != STD_ON) */
#endif /* (FRIF_PROD_ERR_HANDLING_SW_CH_B == TS_PROD_ERR_REP_TO_DEM) */

    /* --- FRIF_E_SW_CH_B --- */
    if((FrIf_ChannelStatus & FRIF_ERRBITMASK_SW_CH) != 0x0000U )
    {
#if (FRIF_PROD_ERR_HANDLING_SW_CH_B == TS_PROD_ERR_REP_TO_DEM)

        /* check if DEM reporting is enabled for this cluster */
        if (ClstPtr->DemEventId_SW_CH_B != 0U)
        {
          /* Report a SW channel B error to the DEM */
          Dem_ReportErrorStatus(ClstPtr->DemEventId_SW_CH_B, FRIF_SW_CH_B_FAILED);
        }

#elif (FRIF_PROD_ERR_HANDLING_SW_CH_B == TS_PROD_ERR_REP_TO_DET)
#if (FRIF_REPORT_TO_DET_ENABLE == STD_ON)

        /* Report a SW channel B error to the DET */
        (void)Det_ReportError( FRIF_MODULE_ID,
                               FrIf_ClstIdx,
                               FRIF_MAINFUNCTION_SERVICE_ID,
                               FRIF_E_DEMTODET_SW_CH_B);

#endif /* (FRIF_REPORT_TO_DET_ENABLE == STD_ON) */
#endif /* (FRIF_PROD_ERR_HANDLING_SW_CH_B == TS_PROD_ERR_REP_TO_DET) */
    }
    else
    {
#if (FRIF_PROD_ERR_HANDLING_SW_CH_B == TS_PROD_ERR_REP_TO_DEM)

        /* check if DEM reporting is enabled for this cluster */
        if (ClstPtr->DemEventId_SW_CH_B != 0U)
        {
          /* Report a SW channel B status to the DEM */
          Dem_ReportErrorStatus(ClstPtr->DemEventId_SW_CH_B, FRIF_SW_CH_B_PASSED);
        }
#endif /* (FRIF_PROD_ERR_HANDLING_SW_CH_B == TS_PROD_ERR_REP_TO_DEM) */
    }

}
#endif /* (FRIF_PROD_ERR_HANDLING_SW_CH_B != TS_PROD_ERR_DISABLE) */

#if (FRIF_PROD_ERR_HANDLING_ACS_CH_A != TS_PROD_ERR_DISABLE)
STATIC FUNC(void,FRIF_CODE) FrIf_CheckReport_ACS_ChA
    (
        uint8 FrIf_ClstIdx,
        uint16 FrIf_ChannelStatus
    );

STATIC FUNC(void,FRIF_CODE) FrIf_CheckReport_ACS_ChA
    (
        uint8 FrIf_ClstIdx,
        uint16 FrIf_ChannelStatus
    )
{
#if (FRIF_PROD_ERR_HANDLING_ACS_CH_A == TS_PROD_ERR_REP_TO_DEM)
    /* get cluster configuration pointer */
    CONSTP2CONST(FrIf_ClstType,AUTOMATIC, FRIF_APPL_CONST) ClstPtr =
        &FRIF_GET_CONFIG_ADDR(FrIf_ClstType,FrIf_RootConfigPtr->FrIf_ClstRef)[FrIf_ClstIdx];
#elif (FRIF_PROD_ERR_HANDLING_ACS_CH_A == TS_PROD_ERR_REP_TO_DET)
#if (FRIF_REPORT_TO_DET_ENABLE != STD_ON)
    TS_PARAM_UNUSED(FrIf_ClstIdx);
#endif /* (FRIF_REPORT_TO_DET_ENABLE != STD_ON) */
#endif /* (FRIF_PROD_ERR_HANDLING_ACS_CH_A == TS_PROD_ERR_REP_TO_DEM) */

    /* --- FRIF_E_ACS_CH_A --- */
    if((FrIf_ChannelStatus & FRIF_ERRBITMASK_ACS_CH) != 0x0000U )
    {
#if (FRIF_PROD_ERR_HANDLING_ACS_CH_A == TS_PROD_ERR_REP_TO_DEM)

        /* check if DEM reporting is enabled for this cluster */
        if (ClstPtr->DemEventId_ACS_CH_A != 0U)
        {
          /* Report a ACS channel A error to the DEM */
          Dem_ReportErrorStatus(ClstPtr->DemEventId_ACS_CH_A, FRIF_ACS_CH_A_FAILED);
        }

#elif (FRIF_PROD_ERR_HANDLING_ACS_CH_A == TS_PROD_ERR_REP_TO_DET)
#if (FRIF_REPORT_TO_DET_ENABLE == STD_ON)

        /* Report a ACS channel A error to the DET */
        (void)Det_ReportError( FRIF_MODULE_ID,
                               FrIf_ClstIdx,
                               FRIF_MAINFUNCTION_SERVICE_ID,
                               FRIF_E_DEMTODET_ACS_CH_A);

#endif /* (FRIF_REPORT_TO_DET_ENABLE == STD_ON) */
#endif /* (FRIF_PROD_ERR_HANDLING_ACS_CH_A == TS_PROD_ERR_REP_TO_DET) */
    }
    else
    {
#if (FRIF_PROD_ERR_HANDLING_ACS_CH_A == TS_PROD_ERR_REP_TO_DEM)

        /* check if DEM reporting is enabled for this cluster */
        if (ClstPtr->DemEventId_ACS_CH_A != 0U)
        {
          /* Report ACS channel A status to the DEM */
          Dem_ReportErrorStatus(ClstPtr->DemEventId_ACS_CH_A, FRIF_ACS_CH_A_PASSED);
        }
#endif /* (FRIF_PROD_ERR_HANDLING_ACS_CH_A == TS_PROD_ERR_REP_TO_DEM) */
    }
}
#endif /* (FRIF_PROD_ERR_HANDLING_ACS_CH_A != TS_PROD_ERR_DISABLE) */

#if (FRIF_PROD_ERR_HANDLING_ACS_CH_B != TS_PROD_ERR_DISABLE)
STATIC FUNC(void,FRIF_CODE) FrIf_CheckReport_ACS_ChB
    (
        uint8 FrIf_ClstIdx,
        uint16 FrIf_ChannelStatus
    );

STATIC FUNC(void,FRIF_CODE) FrIf_CheckReport_ACS_ChB
    (
        uint8 FrIf_ClstIdx,
        uint16 FrIf_ChannelStatus
    )
{
#if (FRIF_PROD_ERR_HANDLING_ACS_CH_B == TS_PROD_ERR_REP_TO_DEM)
    /* get cluster configuration pointer */
    CONSTP2CONST(FrIf_ClstType,AUTOMATIC, FRIF_APPL_CONST) ClstPtr =
        &FRIF_GET_CONFIG_ADDR(FrIf_ClstType,FrIf_RootConfigPtr->FrIf_ClstRef)[FrIf_ClstIdx];
#elif (FRIF_PROD_ERR_HANDLING_ACS_CH_B == TS_PROD_ERR_REP_TO_DET)
#if (FRIF_REPORT_TO_DET_ENABLE != STD_ON)
    TS_PARAM_UNUSED(FrIf_ClstIdx);
#endif /* (FRIF_REPORT_TO_DET_ENABLE != STD_ON) */
#endif /* (FRIF_PROD_ERR_HANDLING_ACS_CH_B == TS_PROD_ERR_REP_TO_DEM) */

    /* --- FRIF_E_ACS_CH_B --- */
    if((FrIf_ChannelStatus & FRIF_ERRBITMASK_ACS_CH) != 0x0000U )
    {
#if (FRIF_PROD_ERR_HANDLING_ACS_CH_B == TS_PROD_ERR_REP_TO_DEM)

        /* check if DEM reporting is enabled for this cluster */
        if (ClstPtr->DemEventId_ACS_CH_B != 0U)
        {
          /* Report a ACS channel B error to the DEM */
          Dem_ReportErrorStatus(ClstPtr->DemEventId_ACS_CH_B, FRIF_ACS_CH_B_FAILED);
        }

#elif (FRIF_PROD_ERR_HANDLING_ACS_CH_B == TS_PROD_ERR_REP_TO_DET)
#if (FRIF_REPORT_TO_DET_ENABLE == STD_ON)

        /* Report a ACS channel B error to the DET */
        (void)Det_ReportError( FRIF_MODULE_ID,
                               FrIf_ClstIdx,
                               FRIF_MAINFUNCTION_SERVICE_ID,
                               FRIF_E_DEMTODET_ACS_CH_B);

#endif /* (FRIF_REPORT_TO_DET_ENABLE == STD_ON) */
#endif /* (FRIF_PROD_ERR_HANDLING_ACS_CH_B == TS_PROD_ERR_REP_TO_DET) */
    }
    else
    {
#if (FRIF_PROD_ERR_HANDLING_ACS_CH_B == TS_PROD_ERR_REP_TO_DEM)

        /* check if DEM reporting is enabled for this cluster */
        if (ClstPtr->DemEventId_ACS_CH_B != 0U)
        {
          /* Report ACS channel B status to the DEM */
          Dem_ReportErrorStatus(ClstPtr->DemEventId_ACS_CH_B, FRIF_ACS_CH_B_PASSED);
        }
#endif /* (FRIF_PROD_ERR_HANDLING_ACS_CH_B == TS_PROD_ERR_REP_TO_DEM) */
    }
}
#endif /* (FRIF_PROD_ERR_HANDLING_ACS_CH_B != TS_PROD_ERR_DISABLE) */

#endif /* (FRIF_CHANNEL_STATUS_REPORTING == STD_ON) */

#if ( (FRIF_CHANNEL_STATUS_REPORTING == STD_ON) || (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON) )
STATIC FUNC(void,FRIF_CODE) FrIf_ReportChannelStatus
(
    uint8 FrIf_ClstIdx
)
{
    uint16 FrIf_ChannelAStatus = 0x0000U;
    uint16 FrIf_ChannelBStatus = 0x0000U;
    Std_ReturnType RetCode = E_NOT_OK;

    /* variable for iterating FrIf controllers */
    uint8_least FrIf_CtrlIdx = 0U;

    /* temporary channel status variables */
    uint16 TmpChannelAStatus;
    uint16 TmpChannelBStatus;

#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_ON)
    TS_PARAM_UNUSED(FrIf_ClstIdx);
#endif /* (FRIF_SINGLE_CLST_OPT_ENABLE == STD_ON) */

#if ( (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON) && (FRIF_CHANNEL_STATUS_REPORTING == STD_OFF) )

    /* If status reports are only enabled for Mirror, check if this cluster was enabled to report */
    if(FrIf_ClstRuntimeData[FRIF_CLSTIDX].BusmirroringEnabled == TRUE)
#endif /* (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON) && (FRIF_CHANNEL_STATUS_REPORTING == STD_OFF) */
    {
#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_OFF)

      /* iterate over all controllers of this cluster */
      for(FrIf_CtrlIdx = 0U; FrIf_CtrlIdx < FrIf_RootConfigPtr->FrIf_CtrlNum; FrIf_CtrlIdx++)
      {
#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF)
          if ( FRIF_GET_CONFIG_ADDR(FrIf_CtrlType,
              FrIf_RootConfigPtr->FrIf_CtrlRef)[FrIf_CtrlIdx].ClstIdx == FRIF_CLSTIDX)
          {
#endif /* (FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF) */
#endif /* FRIF_SINGLE_CTRL_OPT_ENABLE ==  STD_OFF */

              /* FrIf_CtrlIdx is uint8_least for performance reasons, but will never exceed 255 */
              RetCode = FrIf_GetChannelStatus((uint8)FrIf_CtrlIdx, &TmpChannelAStatus, &TmpChannelBStatus);

              if( RetCode == E_OK )
              {
                  FrIf_ChannelAStatus |= TmpChannelAStatus;
                  FrIf_ChannelBStatus |= TmpChannelBStatus;
              }

#if (FRIF_SINGLE_CTRL_OPT_ENABLE ==  STD_OFF)
#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF)
          }
#endif /* (FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF) */
      }
#endif /* FRIF_SINGLE_CTRL_OPT_ENABLE ==  STD_OFF */
    }

    {

#if (FRIF_PROD_ERR_HANDLING_NIT_CH_A != TS_PROD_ERR_DISABLE)
        /* --- FRIF_E_NIT_CH_A --- */
        FrIf_CheckReport_NIT_ChA(FRIF_CLSTIDX, FrIf_ChannelAStatus);
#endif /* (FRIF_PROD_ERR_HANDLING_NIT_CH_A != TS_PROD_ERR_DISABLE) */

#if (FRIF_PROD_ERR_HANDLING_NIT_CH_B != TS_PROD_ERR_DISABLE)
        /* --- FRIF_E_NIT_CH_B --- */
        FrIf_CheckReport_NIT_ChB(FRIF_CLSTIDX, FrIf_ChannelBStatus);
#endif /* (FRIF_PROD_ERR_HANDLING_NIT_CH_B != TS_PROD_ERR_DISABLE) */

#if (FRIF_PROD_ERR_HANDLING_SW_CH_A != TS_PROD_ERR_DISABLE)
        /* --- FRIF_E_SW_CH_A --- */
        FrIf_CheckReport_SW_ChA(FRIF_CLSTIDX, FrIf_ChannelAStatus);
#endif /* (FRIF_PROD_ERR_HANDLING_SW_CH_A != TS_PROD_ERR_DISABLE) */

#if (FRIF_PROD_ERR_HANDLING_SW_CH_B != TS_PROD_ERR_DISABLE)
        /* --- FRIF_E_SW_CH_B --- */
        FrIf_CheckReport_SW_ChB(FRIF_CLSTIDX, FrIf_ChannelBStatus);
#endif /* (FRIF_PROD_ERR_HANDLING_SW_CH_B != TS_PROD_ERR_DISABLE) */

#if (FRIF_PROD_ERR_HANDLING_ACS_CH_A != TS_PROD_ERR_DISABLE)
        /* --- FRIF_E_ACS_CH_A --- */
        FrIf_CheckReport_ACS_ChA(FRIF_CLSTIDX, FrIf_ChannelAStatus);
#endif /* (FRIF_PROD_ERR_HANDLING_ACS_CH_A != TS_PROD_ERR_DISABLE) */

#if (FRIF_PROD_ERR_HANDLING_ACS_CH_B != TS_PROD_ERR_DISABLE)
        /* --- FRIF_E_ACS_CH_B --- */
        FrIf_CheckReport_ACS_ChB(FRIF_CLSTIDX, FrIf_ChannelBStatus);
#endif /* (FRIF_PROD_ERR_HANDLING_ACS_CH_B != TS_PROD_ERR_DISABLE) */

#if (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON)
        /* --- FRIF_BUSMIRRORING --- */
        /* If Busmirroring is enabled for this Cluster, report the status to BusMirror module*/
        if(FrIf_ClstRuntimeData[FRIF_CLSTIDX].BusmirroringEnabled == TRUE)
        {
            /* !LINKSTO FrIf.ASR44.SWS_FrIf_01124,1 */
            Mirror_ReportFlexRayChannelStatus
                (
                    FRIF_CLSTIDX,
                    FrIf_ChannelAStatus,
                    FrIf_ChannelBStatus
                );
        }
#endif /* (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON) */
    }
}

#endif /* (FRIF_CHANNEL_STATUS_REPORTING == STD_ON) || (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON) */

/**
 * \brief  Read the current FrIf state of a FlexRay cluster
 *
 * \param Fr_ClstIdx (in)           FlexRay cluster index.
 * \param FrIf_StatePtr (out)       Address the current state is written to.
 *
 * \retval  E_OK               Function serviced successfully.
 * \retval  E_NOT_OK           Function execution failed.
 *
 */
FUNC(Std_ReturnType,FRIF_CODE) FrIf_GetState
    (
        uint8 FrIf_ClstIdx,
        P2VAR(FrIf_StateType,AUTOMATIC,FRIF_APPL_DATA) FrIf_StatePtr
    )
{
    Std_ReturnType RetValue = E_NOT_OK;

    DBG_FRIF_GETSTATE_ENTRY(FrIf_ClstIdx,FrIf_StatePtr);

/* check if development error detection is disabled */
#if (FRIF_DEV_ERROR_DETECT == STD_OFF)

    TS_PARAM_UNUSED(FrIf_ClstIdx);

#else /* FRIF_DEV_ERROR_DETECT == STD_OFF */
    /* check for successfully initialized module */
    /* Report to DET and return Error in case module was not initialized before */
    if(FrIf_ModuleInitialized == FRIF_UNINIT)
    {
        /* Report to DET */
        FrIf_Det_ReportError(FRIF_GETSTATE_SERVICE_ID, FRIF_E_NOT_INITIALIZED);
    }

#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_ON)

    /* check that controller index is 0 */
    else if (FrIf_ClstIdx != (uint8) 0U)

#else /* FRIF_SINGLE_CTRL_OPT_ENABLE */

    /* check whether configuration has an entry for this FlexRay CC Idx */
    else if(FRIF_CLSTIDX >= FrIf_RootConfigPtr->FrIf_ClstNum)

#endif  /* FRIF_SINGLE_CTRL_OPT_ENABLE */
    {
        /* Report to DET */
        FrIf_Det_ReportError(FRIF_GETSTATE_SERVICE_ID, FRIF_E_INV_CLST_IDX);
    }

    else if(FrIf_StatePtr == NULL_PTR)
    {

        /* Report to DET */
        FrIf_Det_ReportError(FRIF_GETSTATE_SERVICE_ID, FRIF_E_INV_POINTER);
    }

    else

#endif  /* FRIF_DEV_ERROR_DETECT == STD_OFF */
    {
        /* store state in an atomic fashion to a local variable */
        uint8 State;
        TS_AtomicAssign8(State,FrIf_ClstRuntimeData[FRIF_CLSTIDX].FrIf_State);

        /* translate internal state into API state */
        *FrIf_StatePtr = (FrIf_StateType)State;
        RetValue = E_OK;
    }

    /* return result */

    DBG_FRIF_GETSTATE_EXIT(RetValue,FrIf_ClstIdx,FrIf_StatePtr);
    return RetValue;
}


/**
 * \brief  Set a FrIf state of the addressed FlexRay cluster
 *
 * \param Fr_ClstIdx (in)           FlexRay controller index.
 * \param FrIf_StateTransition (in) Requested transition.
  *
 * \retval  E_OK               Function serviced successfully.
 * \retval  E_NOT_OK           Function execution failed.
 *
 */

FUNC(Std_ReturnType,FRIF_CODE) FrIf_SetState
    (
        uint8 FrIf_ClstIdx,
        FrIf_StateTransitionType FrIf_StateTransition
    )
{

    Std_ReturnType RetValue = E_NOT_OK;

    DBG_FRIF_SETSTATE_ENTRY(FrIf_ClstIdx,FrIf_StateTransition);

/* check if development error detection is disabled */
#if (FRIF_DEV_ERROR_DETECT == STD_OFF)

    TS_PARAM_UNUSED(FrIf_ClstIdx);

#else /* FRIF_DEV_ERROR_DETECT == STD_OFF */
    /* check for successfully initialized module */
    /* Report to DET and return Error in case module was not initialized before */
    if(FrIf_ModuleInitialized == FRIF_UNINIT)
    {
        /* Report to DET */
        FrIf_Det_ReportError(FRIF_SETSTATE_SERVICE_ID, FRIF_E_NOT_INITIALIZED);
    }

#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_ON)

    /* check that cluster index is 0 */
    else if (FrIf_ClstIdx != (uint8) 0U)

#else /* FRIF_SINGLE_CLST_OPT_ENABLE */

    /* check whether configuration has an entry for this FlexRay Cluster Idx */
    else if(FRIF_CLSTIDX >= FrIf_RootConfigPtr->FrIf_ClstNum)

#endif  /* FRIF_SINGLE_CLST_OPT_ENABLE */
    {
        /* Report to DET */
        FrIf_Det_ReportError(FRIF_SETSTATE_SERVICE_ID, FRIF_E_INV_CLST_IDX);
    }

    else

#endif  /* FRIF_DEV_ERROR_DETECT == STD_OFF */
    {
        /* check if online state is requested */
        if (FrIf_StateTransition == FRIF_GOTO_ONLINE)
        {
            /* get current state */
            FrIf_StateType CurrentState;

            uint8 State;

            /* convert state in an atomic fashion */
            TS_AtomicAssign8(State,FrIf_ClstRuntimeData[FRIF_CLSTIDX].FrIf_State);

            /* convert state to enum */
            CurrentState = (FrIf_StateType)State;

            /*
             * if the state is switched from offline to online,
             * initialize all Rx-Flags and Tx-Flags
             */
            if (CurrentState == FRIF_STATE_OFFLINE)
            {
                /* reset pending flags */

#if (FRIF_DECOUPLED_RX_ENABLE == STD_ON)

                FrIf_ResetRxFlags(FRIF_CLSTIDX);

#endif /* FRIF_DECOUPLED_RX_ENABLE  */

                FrIf_ResetTxFlags(FRIF_CLSTIDX);

#if (FRIF_EXTENDED_RXFIFO_ENABLE == STD_ON)
                {
                    /* get cluster configuration pointer */
                    CONSTP2CONST(FrIf_CtrlType,AUTOMATIC, FRIF_APPL_CONST) CtrlPtr =
                        FRIF_GET_CONFIG_ADDR(FrIf_CtrlType,FrIf_RootConfigPtr->FrIf_CtrlRef);

#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_OFF)

                    /* variable for iterating FrIf controllers */
                    uint8_least FrIf_CtrlIdx;

                    /* iterate over all controllers */
                    for(FrIf_CtrlIdx = 0U; FrIf_CtrlIdx < FrIf_RootConfigPtr->FrIf_CtrlNum; FrIf_CtrlIdx++)
#endif /* FRIF_SINGLE_CTRL_OPT_ENABLE */
                    {
#if (FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF)
                        /* search for controller belonging to the current cluster */
                        if(CtrlPtr[FRIF_CTRLIDX].ClstIdx == FRIF_CLSTIDX)
#endif /* (FRIF_SINGLE_CLST_OPT_ENABLE == STD_OFF) */
                        {
                            /* variable for iterating RxFIFOs of the controller */
                            uint8_least iRxFIFO;

                            /* iterate over all RxFIFOs of the controller */
                            for(iRxFIFO = 0U; iRxFIFO < CtrlPtr[FRIF_CTRLIDX].RxFIFONum; iRxFIFO++)
                            {
                                /* flush all FIFOs of the repesctive controller */
                                FrIf_Call_Fr_Arg2(FRIF_FR_INSTANCE_IDX,FlushRxFIFO,
                                        FRIF_FR_CTRLIDX,
                                        (uint8)iRxFIFO
                                    );
                            }
                        }
                    }
                }
#endif /* FRIF_EXTENDED_RXFIFO_ENABLE */

            }

            /* store the reqested target state */
            DBG_FRIF_STATE_GRP(FRIF_CLSTIDX, (FrIf_ClstRuntimeData[FRIF_CLSTIDX].FrIf_State), (FRIF_STATE_ONLINE));
            TS_AtomicAssign8(FrIf_ClstRuntimeData[FRIF_CLSTIDX].FrIf_State,(uint8)FRIF_STATE_ONLINE);
        }
        else /* offline state is requested */
        {
            /* store the reqested target state */
            DBG_FRIF_STATE_GRP(FRIF_CLSTIDX, (FrIf_ClstRuntimeData[FRIF_CLSTIDX].FrIf_State), (FRIF_STATE_OFFLINE));

            TS_AtomicAssign8(FrIf_ClstRuntimeData[FRIF_CLSTIDX].FrIf_State,(uint8)FRIF_STATE_OFFLINE);
        }
        RetValue = E_OK;
    }

    /* return result */

    DBG_FRIF_SETSTATE_EXIT(RetValue,FrIf_ClstIdx,FrIf_StateTransition);
    return RetValue;
}


/* stop code section declaration */
#if (TS_MERGED_COMPILE == STD_OFF)
#define FRIF_STOP_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */
#endif /* TS_MERGED_COMPILE */

