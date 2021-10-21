/**
 * \file
 *
 * \brief AUTOSAR WdgM
 *
 * This file contains the implementation of the AUTOSAR
 * module WdgM.
 *
 * \version 6.1.39
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef WDGM_TRACE_STC_H
#define WDGM_TRACE_STC_H

/*==================[inclusions]============================================*/
/* !LINKSTO WDGM.EB.Dbg2,1 */
#include <WdgM_Trace.h>

/*==================[macros]================================================*/

#ifndef DBG_WDGM_INIT_ENTRY
/** \brief Entry point of function WdgM_Init() */
#define DBG_WDGM_INIT_ENTRY(a)
#endif

#ifndef DBG_WDGM_INIT_EXIT
/** \brief Exit point of function WdgM_Init() */
#define DBG_WDGM_INIT_EXIT(a)
#endif

#ifndef DBG_WDGM_DEINIT_ENTRY
/** \brief Entry point of function WdgM_DeInit() */
#define DBG_WDGM_DEINIT_ENTRY()
#endif

#ifndef DBG_WDGM_DEINIT_EXIT
/** \brief Exit point of function WdgM_DeInit() */
#define DBG_WDGM_DEINIT_EXIT()
#endif

#ifndef DBG_WDGM_GETVERSIONINFO_ENTRY
/** \brief Entry point of function WdgM_GetVersionInfo() */
#define DBG_WDGM_GETVERSIONINFO_ENTRY(a)
#endif

#ifndef DBG_WDGM_GETVERSIONINFO_EXIT
/** \brief Exit point of function WdgM_GetVersionInfo() */
#define DBG_WDGM_GETVERSIONINFO_EXIT(a)
#endif

#ifndef DBG_WDGM_SETMODE_ENTRY
/** \brief Entry point of function WdgM_SetMode() */
#define DBG_WDGM_SETMODE_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_SETMODE_EXIT
/** \brief Exit point of function WdgM_SetMode() */
#define DBG_WDGM_SETMODE_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_GETMODE_ENTRY
/** \brief Entry point of function WdgM_GetMode() */
#define DBG_WDGM_GETMODE_ENTRY(a)
#endif

#ifndef DBG_WDGM_GETMODE_EXIT
/** \brief Exit point of function WdgM_GetMode() */
#define DBG_WDGM_GETMODE_EXIT(a,b)
#endif

#ifndef DBG_WDGM_CHECKPOINTREACHED_ENTRY
/** \brief Entry point of function WdgM_ASR40_CheckpointReached() */
#define DBG_WDGM_CHECKPOINTREACHED_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_CHECKPOINTREACHED_EXIT
/** \brief Exit point of function WdgM_ASR40_CheckpointReached() */
#define DBG_WDGM_CHECKPOINTREACHED_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_UPDATEALIVECOUNTER_ENTRY
/** \brief Entry point of function WdgM_ASR40_UpdateAliveCounter() */
#define DBG_WDGM_UPDATEALIVECOUNTER_ENTRY(a)
#endif

#ifndef DBG_WDGM_UPDATEALIVECOUNTER_EXIT
/** \brief Exit point of function WdgM_ASR40_UpdateAliveCounter() */
#define DBG_WDGM_UPDATEALIVECOUNTER_EXIT(a,b)
#endif

#ifndef DBG_WDGM_GETLOCALSTATUS_ENTRY
/** \brief Entry point of function WdgM_GetLocalStatus() */
#define DBG_WDGM_GETLOCALSTATUS_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_GETLOCALSTATUS_EXIT
/** \brief Exit point of function WdgM_GetLocalStatus() */
#define DBG_WDGM_GETLOCALSTATUS_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_GETGLOBALSTATUS_ENTRY
/** \brief Entry point of function WdgM_GetGlobalStatus() */
#define DBG_WDGM_GETGLOBALSTATUS_ENTRY(a)
#endif

#ifndef DBG_WDGM_GETGLOBALSTATUS_EXIT
/** \brief Exit point of function WdgM_GetGlobalStatus() */
#define DBG_WDGM_GETGLOBALSTATUS_EXIT(a,b)
#endif

#ifndef DBG_WDGM_PERFORMRESET_ENTRY
/** \brief Entry point of function WdgM_PerformReset() */
#define DBG_WDGM_PERFORMRESET_ENTRY()
#endif

#ifndef DBG_WDGM_PERFORMRESET_EXIT
/** \brief Exit point of function WdgM_PerformReset() */
#define DBG_WDGM_PERFORMRESET_EXIT()
#endif

#ifndef DBG_WDGM_GETFIRSTEXPIREDSEID_ENTRY
/** \brief Entry point of function WdgM_GetFirstExpiredSEID() */
#define DBG_WDGM_GETFIRSTEXPIREDSEID_ENTRY(a)
#endif

#ifndef DBG_WDGM_GETFIRSTEXPIREDSEID_EXIT
/** \brief Exit point of function WdgM_GetFirstExpiredSEID() */
#define DBG_WDGM_GETFIRSTEXPIREDSEID_EXIT(a,b)
#endif

#ifndef DBG_WDGM_MAINFUNCTION_ENTRY
/** \brief Entry point of function WdgM_MainFunction() */
#define DBG_WDGM_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_WDGM_MAINFUNCTION_EXIT
/** \brief Exit point of function WdgM_MainFunction() */
#define DBG_WDGM_MAINFUNCTION_EXIT()
#endif

#ifndef DBG_WDGM_EB_INIT_ENTRY
/** \brief Entry point of function WdgM_EB_Init() */
#define DBG_WDGM_EB_INIT_ENTRY(a)
#endif

#ifndef DBG_WDGM_EB_INIT_EXIT
/** \brief Exit point of function WdgM_EB_Init() */
#define DBG_WDGM_EB_INIT_EXIT(a,b)
#endif

#ifndef DBG_WDGM_EB_DEINIT_ENTRY
/** \brief Entry point of function WdgM_EB_DeInit() */
#define DBG_WDGM_EB_DEINIT_ENTRY(a)
#endif

#ifndef DBG_WDGM_EB_DEINIT_EXIT
/** \brief Exit point of function WdgM_EB_DeInit() */
#define DBG_WDGM_EB_DEINIT_EXIT(a,b)
#endif

#ifndef DBG_WDGM_EB_SETMODE_ENTRY
/** \brief Entry point of function WdgM_EB_SetMode() */
#define DBG_WDGM_EB_SETMODE_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_EB_SETMODE_EXIT
/** \brief Exit point of function WdgM_EB_SetMode() */
#define DBG_WDGM_EB_SETMODE_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_EB_PERFORMWDGIFMODESWITCH_ENTRY
/** \brief Entry point of function WdgM_EB_PerformWdgIfModeSwitch() */
#define DBG_WDGM_EB_PERFORMWDGIFMODESWITCH_ENTRY(a)
#endif

#ifndef DBG_WDGM_EB_PERFORMWDGIFMODESWITCH_EXIT
/** \brief Exit point of function WdgM_EB_PerformWdgIfModeSwitch() */
#define DBG_WDGM_EB_PERFORMWDGIFMODESWITCH_EXIT(a,b)
#endif

#ifndef DBG_WDGM_EB_CHECKPOINTREACHEDINTLOGICAL_ENTRY
/** \brief Entry point of function WdgM_EB_CheckpointReachedIntLogical() */
#define DBG_WDGM_EB_CHECKPOINTREACHEDINTLOGICAL_ENTRY(a,b,c)
#endif

#ifndef DBG_WDGM_EB_CHECKPOINTREACHEDINTLOGICAL_EXIT
/** \brief Exit point of function WdgM_EB_CheckpointReachedIntLogical() */
#define DBG_WDGM_EB_CHECKPOINTREACHEDINTLOGICAL_EXIT(a,b,c,d)
#endif

#ifndef DBG_WDGM_EB_CHECKPOINTREACHEDEXTLOGICAL_ENTRY
/** \brief Entry point of function WdgM_EB_CheckpointReachedExtLogical() */
#define DBG_WDGM_EB_CHECKPOINTREACHEDEXTLOGICAL_ENTRY(a,b,c,d)
#endif

#ifndef DBG_WDGM_EB_CHECKPOINTREACHEDEXTLOGICAL_EXIT
/** \brief Exit point of function WdgM_EB_CheckpointReachedExtLogical() */
#define DBG_WDGM_EB_CHECKPOINTREACHEDEXTLOGICAL_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_WDGM_EB_CHECKPOINTREACHEDDEADLINE_ENTRY
/** \brief Entry point of function WdgM_EB_CheckpointReachedDeadline() */
#define DBG_WDGM_EB_CHECKPOINTREACHEDDEADLINE_ENTRY(a,b,c)
#endif

#ifndef DBG_WDGM_EB_CHECKPOINTREACHEDDEADLINE_EXIT
/** \brief Exit point of function WdgM_EB_CheckpointReachedDeadline() */
#define DBG_WDGM_EB_CHECKPOINTREACHEDDEADLINE_EXIT(a,b,c,d)
#endif

#ifndef DBG_WDGM_EB_STARTCHECKPOINTDEADLINE_ENTRY
/** \brief Entry point of function WdgM_EB_StartCheckpointDeadline() */
#define DBG_WDGM_EB_STARTCHECKPOINTDEADLINE_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_WDGM_EB_STARTCHECKPOINTDEADLINE_EXIT
/** \brief Exit point of function WdgM_EB_StartCheckpointDeadline() */
#define DBG_WDGM_EB_STARTCHECKPOINTDEADLINE_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_WDGM_EB_STOPCHECKPOINTDEADLINE_ENTRY
/** \brief Entry point of function WdgM_EB_StopCheckpointDeadline() */
#define DBG_WDGM_EB_STOPCHECKPOINTDEADLINE_ENTRY(a,b,c)
#endif

#ifndef DBG_WDGM_EB_STOPCHECKPOINTDEADLINE_EXIT
/** \brief Exit point of function WdgM_EB_StopCheckpointDeadline() */
#define DBG_WDGM_EB_STOPCHECKPOINTDEADLINE_EXIT(a,b,c,d)
#endif

#ifndef DBG_WDGM_EB_CHECKDEADLINEVIOLATION_ENTRY
/** \brief Entry point of function WdgM_EB_CheckDeadlineViolation() */
#define DBG_WDGM_EB_CHECKDEADLINEVIOLATION_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_EB_CHECKDEADLINEVIOLATION_EXIT
/** \brief Exit point of function WdgM_EB_CheckDeadlineViolation() */
#define DBG_WDGM_EB_CHECKDEADLINEVIOLATION_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_EB_CHECKPOINTREACHEDALIVE_ENTRY
/** \brief Entry point of function WdgM_EB_CheckpointReachedAlive() */
#define DBG_WDGM_EB_CHECKPOINTREACHEDALIVE_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_EB_CHECKPOINTREACHEDALIVE_EXIT
/** \brief Exit point of function WdgM_EB_CheckpointReachedAlive() */
#define DBG_WDGM_EB_CHECKPOINTREACHEDALIVE_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_EB_CHECKPREDCP_ENTRY
/** \brief Entry point of function WdgM_EB_CheckPredCP() */
#define DBG_WDGM_EB_CHECKPREDCP_ENTRY(a,b,c)
#endif

#ifndef DBG_WDGM_EB_CHECKPREDCP_EXIT
/** \brief Exit point of function WdgM_EB_CheckPredCP() */
#define DBG_WDGM_EB_CHECKPREDCP_EXIT(a,b,c,d)
#endif

#ifndef DBG_WDGM_EB_DETERMINEALIVERESULT_ENTRY
/** \brief Entry point of function WdgM_EB_DetermineAliveResult() */
#define DBG_WDGM_EB_DETERMINEALIVERESULT_ENTRY(a,b,c,d)
#endif

#ifndef DBG_WDGM_EB_DETERMINEALIVERESULT_EXIT
/** \brief Exit point of function WdgM_EB_DetermineAliveResult() */
#define DBG_WDGM_EB_DETERMINEALIVERESULT_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_WDGM_EB_DETERMINEDEADLINERESULT_ENTRY
/** \brief Entry point of function WdgM_EB_DetermineDeadlineResult() */
#define DBG_WDGM_EB_DETERMINEDEADLINERESULT_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_EB_DETERMINEDEADLINERESULT_EXIT
/** \brief Exit point of function WdgM_EB_DetermineDeadlineResult() */
#define DBG_WDGM_EB_DETERMINEDEADLINERESULT_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_EB_DETERMINELOGICALRESULT_ENTRY
/** \brief Entry point of function WdgM_EB_DetermineLogicalResult() */
#define DBG_WDGM_EB_DETERMINELOGICALRESULT_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_EB_DETERMINELOGICALRESULT_EXIT
/** \brief Exit point of function WdgM_EB_DetermineLogicalResult() */
#define DBG_WDGM_EB_DETERMINELOGICALRESULT_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_EB_SETLOCALSTATUS_ENTRY
/** \brief Entry point of function WdgM_EB_SetLocalStatus() */
#define DBG_WDGM_EB_SETLOCALSTATUS_ENTRY(a)
#endif

#ifndef DBG_WDGM_EB_SETLOCALSTATUS_EXIT
/** \brief Exit point of function WdgM_EB_SetLocalStatus() */
#define DBG_WDGM_EB_SETLOCALSTATUS_EXIT(a)
#endif

#ifndef DBG_WDGM_EB_SETSELOCALSTATUS_ENTRY
/** \brief Entry point of function WdgM_EB_SetSELocalStatus() */
#define DBG_WDGM_EB_SETSELOCALSTATUS_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_WDGM_EB_SETSELOCALSTATUS_EXIT
/** \brief Exit point of function WdgM_EB_SetSELocalStatus() */
#define DBG_WDGM_EB_SETSELOCALSTATUS_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_WDGM_EB_SETGLOBALSTATUS_ENTRY
/** \brief Entry point of function WdgM_EB_SetGlobalStatus() */
#define DBG_WDGM_EB_SETGLOBALSTATUS_ENTRY(a)
#endif

#ifndef DBG_WDGM_EB_SETGLOBALSTATUS_EXIT
/** \brief Exit point of function WdgM_EB_SetGlobalStatus() */
#define DBG_WDGM_EB_SETGLOBALSTATUS_EXIT(a)
#endif

#ifndef DBG_WDGM_EB_GETNEWGLOBALSTATUS_ENTRY
/** \brief Entry point of function WdgM_EB_GetNewGlobalStatus() */
#define DBG_WDGM_EB_GETNEWGLOBALSTATUS_ENTRY(a)
#endif

#ifndef DBG_WDGM_EB_GETNEWGLOBALSTATUS_EXIT
/** \brief Exit point of function WdgM_EB_GetNewGlobalStatus() */
#define DBG_WDGM_EB_GETNEWGLOBALSTATUS_EXIT(a,b)
#endif

#ifndef DBG_WDGM_EB_UPDATESUPERVISIONCYCLECOUNTER_ENTRY
/** \brief Entry point of function WdgM_EB_UpdateSupervisionCycleCounter() */
#define DBG_WDGM_EB_UPDATESUPERVISIONCYCLECOUNTER_ENTRY(a)
#endif

#ifndef DBG_WDGM_EB_UPDATESUPERVISIONCYCLECOUNTER_EXIT
/** \brief Exit point of function WdgM_EB_UpdateSupervisionCycleCounter() */
#define DBG_WDGM_EB_UPDATESUPERVISIONCYCLECOUNTER_EXIT(a)
#endif

#ifndef DBG_WDGM_EB_CHECKMAINFUNCTIONTIMINGVIOLATION_ENTRY
/** \brief Entry point of function WdgM_EB_CheckMainFunctionTimingViolation() */
#define DBG_WDGM_EB_CHECKMAINFUNCTIONTIMINGVIOLATION_ENTRY(a)
#endif

#ifndef DBG_WDGM_EB_CHECKMAINFUNCTIONTIMINGVIOLATION_EXIT
/** \brief Exit point of function WdgM_EB_CheckMainFunctionTimingViolation() */
#define DBG_WDGM_EB_CHECKMAINFUNCTIONTIMINGVIOLATION_EXIT(a)
#endif

#ifndef DBG_WDGM_EB_GETCHECKEDGRAPHDATA_ENTRY
/** \brief Entry point of function WdgM_EB_GetCheckedGraphData() */
#define DBG_WDGM_EB_GETCHECKEDGRAPHDATA_ENTRY(a,b,c,d)
#endif

#ifndef DBG_WDGM_EB_GETCHECKEDGRAPHDATA_EXIT
/** \brief Exit point of function WdgM_EB_GetCheckedGraphData() */
#define DBG_WDGM_EB_GETCHECKEDGRAPHDATA_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_WDGM_EB_STOREGRAPHDATA_ENTRY
/** \brief Entry point of function WdgM_EB_StoreGraphData() */
#define DBG_WDGM_EB_STOREGRAPHDATA_ENTRY(a,b,c,d)
#endif

#ifndef DBG_WDGM_EB_STOREGRAPHDATA_EXIT
/** \brief Exit point of function WdgM_EB_StoreGraphData() */
#define DBG_WDGM_EB_STOREGRAPHDATA_EXIT(a,b,c,d)
#endif

#ifndef DBG_WDGM_EB_GETCHECKEDSEQNUMBER_ENTRY
/** \brief Entry point of function WdgM_EB_GetCheckedGraphData() */
#define DBG_WDGM_EB_GETCHECKEDSEQNUMBER_ENTRY(a,b,c)
#endif

#ifndef DBG_WDGM_EB_GETCHECKEDSEQNUMBER_EXIT
/** \brief Exit point of function WdgM_EB_GetCheckedGraphData() */
#define DBG_WDGM_EB_GETCHECKEDSEQNUMBER_EXIT(a,b,c,d)
#endif

#ifndef DBG_WDGM_EB_STORESEQNUMBER_ENTRY
/** \brief Entry point of function WdgM_EB_StoreGraphData() */
#define DBG_WDGM_EB_STORESEQNUMBER_ENTRY(a,b,c)
#endif

#ifndef DBG_WDGM_EB_STORESEQNUMBER_EXIT
/** \brief Exit point of function WdgM_EB_StoreGraphData() */
#define DBG_WDGM_EB_STORESEQNUMBER_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_EB_CHECKCPALIVECOUNTERDATA_ENTRY
/** \brief Entry point of function WdgM_EB_CheckCPAliveCounterData() */
#define DBG_WDGM_EB_CHECKCPALIVECOUNTERDATA_ENTRY(a)
#endif

#ifndef DBG_WDGM_EB_CHECKCPALIVECOUNTERDATA_EXIT
/** \brief Exit point of function WdgM_EB_CheckCPAliveCounterData() */
#define DBG_WDGM_EB_CHECKCPALIVECOUNTERDATA_EXIT(a,b)
#endif

#ifndef DBG_WDGM_EB_INCREMENTCPALIVECOUNTERDATA_ENTRY
/** \brief Entry point of function WdgM_EB_IncrementCPAliveCounterData() */
#define DBG_WDGM_EB_INCREMENTCPALIVECOUNTERDATA_ENTRY(a)
#endif

#ifndef DBG_WDGM_EB_INCREMENTCPALIVECOUNTERDATA_EXIT
/** \brief Exit point of function WdgM_EB_IncrementCPAliveCounterData() */
#define DBG_WDGM_EB_INCREMENTCPALIVECOUNTERDATA_EXIT(a)
#endif

#ifndef DBG_WDGM_EB_RESETCPALIVECOUNTERDATA_ENTRY
/** \brief Entry point of function WdgM_EB_ResetCPAliveCounterData() */
#define DBG_WDGM_EB_RESETCPALIVECOUNTERDATA_ENTRY(a,b,c)
#endif

#ifndef DBG_WDGM_EB_RESETCPALIVECOUNTERDATA_EXIT
/** \brief Exit point of function WdgM_EB_ResetCPAliveCounterData() */
#define DBG_WDGM_EB_RESETCPALIVECOUNTERDATA_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_EB_GETCHECKEDCPALIVECOUNTERDATA_ENTRY
/** \brief Entry point of function WdgM_EB_GetCheckedCPAliveCounterData() */
#define DBG_WDGM_EB_GETCHECKEDCPALIVECOUNTERDATA_ENTRY(a,b,c,d)
#endif

#ifndef DBG_WDGM_EB_GETCHECKEDCPALIVECOUNTERDATA_EXIT
/** \brief Exit point of function WdgM_EB_GetCheckedCPAliveCounterData() */
#define DBG_WDGM_EB_GETCHECKEDCPALIVECOUNTERDATA_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_WDGM_EB_GETCHECKEDDMDATA_ENTRY
/** \brief Entry point of function WdgM_EB_GetCheckedDMData() */
#define DBG_WDGM_EB_GETCHECKEDDMDATA_ENTRY(a,b,c,d)
#endif

#ifndef DBG_WDGM_EB_GETCHECKEDDMDATA_EXIT
/** \brief Exit point of function WdgM_EB_GetCheckedDMData() */
#define DBG_WDGM_EB_GETCHECKEDDMDATA_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_WDGM_EB_STOREDMDATA_ENTRY
/** \brief Entry point of function WdgM_EB_StoreDMData() */
#define DBG_WDGM_EB_STOREDMDATA_ENTRY(a,b,c,d)
#endif

#ifndef DBG_WDGM_EB_STOREDMDATA_EXIT
/** \brief Exit point of function WdgM_EB_StoreDMData() */
#define DBG_WDGM_EB_STOREDMDATA_EXIT(a,b,c,d)
#endif

#ifndef DBG_WDGM_EB_STORECPDEADLINEDATA_ENTRY
/** \brief Entry point of function WdgM_EB_StoreCPDeadlineData() */
#define DBG_WDGM_EB_STORECPDEADLINEDATA_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_EB_STORECPDEADLINEDATA_EXIT
/** \brief Exit point of function WdgM_EB_StoreCPDeadlineData() */
#define DBG_WDGM_EB_STORECPDEADLINEDATA_EXIT(a,b)
#endif

#ifndef DBG_WDGM_EB_GETCHECKEDCPDEADLINEDATA_ENTRY
/** \brief Entry point of function WdgM_EB_GetCheckedCPDeadlineData() */
#define DBG_WDGM_EB_GETCHECKEDCPDEADLINEDATA_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_EB_GETCHECKEDCPDEADLINEDATA_EXIT
/** \brief Exit point of function WdgM_EB_GetCheckedCPDeadlineData() */
#define DBG_WDGM_EB_GETCHECKEDCPDEADLINEDATA_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_EB_STORECPLOGICALDATA_ENTRY
/** \brief Entry point of function WdgM_EB_StoreCPLogicalData() */
#define DBG_WDGM_EB_STORECPLOGICALDATA_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_EB_STORECPLOGICALDATA_EXIT
/** \brief Exit point of function WdgM_EB_StoreCPLogicalData() */
#define DBG_WDGM_EB_STORECPLOGICALDATA_EXIT(a,b)
#endif

#ifndef DBG_WDGM_EB_GETCHECKEDCPLOGICALDATA_ENTRY
/** \brief Entry point of function WdgM_EB_GetCheckedCPLogicalData() */
#define DBG_WDGM_EB_GETCHECKEDCPLOGICALDATA_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_EB_GETCHECKEDCPLOGICALDATA_EXIT
/** \brief Exit point of function WdgM_EB_GetCheckedCPLogicalData() */
#define DBG_WDGM_EB_GETCHECKEDCPLOGICALDATA_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_EB_RESETEXTGRAPHDATASEQ_ENTRY
/** \brief Entry point of function WdgM_EB_ResetExtGraphDataSeq() */
#define DBG_WDGM_EB_RESETEXTGRAPHDATASEQ_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_EB_RESETEXTGRAPHDATASEQ_EXIT
/** \brief Exit point of function WdgM_EB_ResetExtGraphDataSeq() */
#define DBG_WDGM_EB_RESETEXTGRAPHDATASEQ_EXIT(a,b)
#endif

#ifndef DBG_WDGM_EB_STOREEXPIREDSEID_ENTRY
/** \brief Entry point of function WdgM_EB_StoreExpiredSEID() */
#define DBG_WDGM_EB_STOREEXPIREDSEID_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_EB_STOREEXPIREDSEID_EXIT
/** \brief Exit point of function WdgM_EB_StoreExpiredSEID() */
#define DBG_WDGM_EB_STOREEXPIREDSEID_EXIT(a,b)
#endif

#ifndef DBG_WDGM_EB_GETCHECKEDEXPIREDSEID_ENTRY
/** \brief Entry point of function WdgM_EB_GetCheckedExpiredSEID() */
#define DBG_WDGM_EB_GETCHECKEDEXPIREDSEID_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_EB_GETCHECKEDEXPIREDSEID_EXIT
/** \brief Exit point of function WdgM_EB_GetCheckedExpiredSEID() */
#define DBG_WDGM_EB_GETCHECKEDEXPIREDSEID_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_EB_SEID2INTSEID_ENTRY
/** \brief Entry point of function WdgM_EB_SEId2IntSEId() */
#define DBG_WDGM_EB_SEID2INTSEID_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_EB_SEID2INTSEID_EXIT
/** \brief Exit point of function WdgM_EB_SEId2IntSEId() */
#define DBG_WDGM_EB_SEID2INTSEID_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_EB_SEISACTIVERESETDATA_ENTRY
/** \brief Entry point of function WdgM_EB_SEIsActiveResetData() */
#define DBG_WDGM_EB_SEISACTIVERESETDATA_ENTRY(a,b,c,d)
#endif

#ifndef DBG_WDGM_EB_SEISACTIVERESETDATA_EXIT
/** \brief Exit point of function WdgM_EB_SEIsActiveResetData() */
#define DBG_WDGM_EB_SEISACTIVERESETDATA_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_WDGM_EB_SERESETDATA_ENTRY
/** \brief Entry point of function WdgM_EB_SEResetData() */
#define DBG_WDGM_EB_SERESETDATA_ENTRY(a)
#endif

#ifndef DBG_WDGM_EB_SERESETDATA_EXIT
/** \brief Exit point of function WdgM_EB_SEResetData() */
#define DBG_WDGM_EB_SERESETDATA_EXIT(a)
#endif

#ifndef DBG_WDGM_EB_SERESETCPALIVEDATA_ENTRY
/** \brief Entry point of function WdgM_EB_SEResetCPAliveData() */
#define DBG_WDGM_EB_SERESETCPALIVEDATA_ENTRY(a,b,c,d)
#endif

#ifndef DBG_WDGM_EB_SERESETCPALIVEDATA_EXIT
/** \brief Exit point of function WdgM_EB_SEResetCPAliveData() */
#define DBG_WDGM_EB_SERESETCPALIVEDATA_EXIT(a,b,c,d)
#endif

#ifndef DBG_WDGM_EB_UPDATETRIGGERCONDITIONS_ENTRY
/** \brief Entry point of function WdgM_EB_UpdateTriggerConditions() */
#define DBG_WDGM_EB_UPDATETRIGGERCONDITIONS_ENTRY(a)
#endif

#ifndef DBG_WDGM_EB_UPDATETRIGGERCONDITIONS_EXIT
/** \brief Exit point of function WdgM_EB_UpdateTriggerConditions() */
#define DBG_WDGM_EB_UPDATETRIGGERCONDITIONS_EXIT(a)
#endif

#ifndef DBG_WDGM_EB_CHECKCALLERID_ENTRY
/** \brief Entry point of function WdgM_EB_CheckCallerID() */
#define DBG_WDGM_EB_CHECKCALLERID_ENTRY(a)
#endif

#ifndef DBG_WDGM_EB_CHECKCALLERID_EXIT
/** \brief Exit point of function WdgM_EB_CheckCallerID() */
#define DBG_WDGM_EB_CHECKCALLERID_EXIT(a,b)
#endif

#ifndef DBG_WDGM_EB_CHECKINITREQUEST_ENTRY
/** \brief Entry point of function WdgM_EB_CheckInitRequest() */
#define DBG_WDGM_EB_CHECKINITREQUEST_ENTRY(a)
#endif

#ifndef DBG_WDGM_EB_CHECKINITREQUEST_EXIT
/** \brief Exit point of function WdgM_EB_CheckInitRequest() */
#define DBG_WDGM_EB_CHECKINITREQUEST_EXIT(a,b)
#endif

#ifndef DBG_WDGM_EB_CHECKMODESWITCHREQUESTSYNCHRON_ENTRY
/** \brief Entry point of function WdgM_EB_CheckModeSwitchRequestSynchron() */
#define DBG_WDGM_EB_CHECKMODESWITCHREQUESTSYNCHRON_ENTRY(a)
#endif

#ifndef DBG_WDGM_EB_CHECKMODESWITCHREQUESTSYNCHRON_EXIT
/** \brief Exit point of function WdgM_EB_CheckModeSwitchRequestSynchron() */
#define DBG_WDGM_EB_CHECKMODESWITCHREQUESTSYNCHRON_EXIT(a,b)
#endif

#ifndef DBG_WDGM_EB_CHECKDEINITREQUEST_ENTRY
/** \brief Entry point of function WdgM_EB_CheckDeinitRequest() */
#define DBG_WDGM_EB_CHECKDEINITREQUEST_ENTRY(a)
#endif

#ifndef DBG_WDGM_EB_CHECKDEINITREQUEST_EXIT
/** \brief Exit point of function WdgM_EB_CheckDeinitRequest() */
#define DBG_WDGM_EB_CHECKDEINITREQUEST_EXIT(a,b)
#endif

#ifndef DBG_WDGM_EB_CHECKMODESWITCHREQUEST_ENTRY
/** \brief Entry point of function WdgM_EB_CheckModeSwitchRequest() */
#define DBG_WDGM_EB_CHECKMODESWITCHREQUEST_ENTRY(a)
#endif

#ifndef DBG_WDGM_EB_CHECKMODESWITCHREQUEST_EXIT
/** \brief Exit point of function WdgM_EB_CheckModeSwitchRequest() */
#define DBG_WDGM_EB_CHECKMODESWITCHREQUEST_EXIT(a,b)
#endif

#ifndef DBG_WDGM_ASR32_RTEGLOBALMODESWITCH_ENTRY
/** \brief Entry point of function WdgM_ASR32_RteGlobalModeSwitch() */
#define DBG_WDGM_ASR32_RTEGLOBALMODESWITCH_ENTRY(a)
#endif

#ifndef DBG_WDGM_ASR32_RTEGLOBALMODESWITCH_EXIT
/** \brief Exit point of function WdgM_ASR32_RteGlobalModeSwitch() */
#define DBG_WDGM_ASR32_RTEGLOBALMODESWITCH_EXIT(a)
#endif

#ifndef DBG_WDGM_ASR32_RTEINDIVIDUALMODESWITCH_ENTRY
/** \brief Entry point of function WdgM_ASR32_RteIndividualModeSwitch() */
#define DBG_WDGM_ASR32_RTEINDIVIDUALMODESWITCH_ENTRY(a,b,c)
#endif

#ifndef DBG_WDGM_ASR32_RTEINDIVIDUALMODESWITCH_EXIT
/** \brief Exit point of function WdgM_ASR32_RteIndividualModeSwitch() */
#define DBG_WDGM_ASR32_RTEINDIVIDUALMODESWITCH_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_ASR32_RTE_UPDATEALIVECOUNTER_ENTRY
/** \brief Entry point of function WdgM_ASR32_Rte_UpdateAliveCounter() */
#define DBG_WDGM_ASR32_RTE_UPDATEALIVECOUNTER_ENTRY(a)
#endif

#ifndef DBG_WDGM_ASR32_RTE_UPDATEALIVECOUNTER_EXIT
/** \brief Exit point of function WdgM_ASR32_Rte_UpdateAliveCounter() */
#define DBG_WDGM_ASR32_RTE_UPDATEALIVECOUNTER_EXIT(a,b)
#endif

#ifndef DBG_WDGM_ASR32_RTE_ACTIVATEALIVESUPERVISION_ENTRY
/** \brief Entry point of function WdgM_ASR32_Rte_ActivateAliveSupervision() */
#define DBG_WDGM_ASR32_RTE_ACTIVATEALIVESUPERVISION_ENTRY(a)
#endif

#ifndef DBG_WDGM_ASR32_RTE_ACTIVATEALIVESUPERVISION_EXIT
/** \brief Exit point of function WdgM_ASR32_Rte_ActivateAliveSupervision() */
#define DBG_WDGM_ASR32_RTE_ACTIVATEALIVESUPERVISION_EXIT(a,b)
#endif

#ifndef DBG_WDGM_ASR32_RTE_DEACTIVATEALIVESUPERVISION_ENTRY
/** \brief Entry point of function WdgM_ASR32_Rte_DeactivateAliveSupervision() */
#define DBG_WDGM_ASR32_RTE_DEACTIVATEALIVESUPERVISION_ENTRY(a)
#endif

#ifndef DBG_WDGM_ASR32_RTE_DEACTIVATEALIVESUPERVISION_EXIT
/** \brief Exit point of function WdgM_ASR32_Rte_DeactivateAliveSupervision() */
#define DBG_WDGM_ASR32_RTE_DEACTIVATEALIVESUPERVISION_EXIT(a,b)
#endif

#ifndef DBG_WDGM_ASR40_RTEGLOBALMODESWITCH_ENTRY
/** \brief Entry point of function WdgM_ASR40_RteGlobalModeSwitch() */
#define DBG_WDGM_ASR40_RTEGLOBALMODESWITCH_ENTRY(a)
#endif

#ifndef DBG_WDGM_ASR40_RTEGLOBALMODESWITCH_EXIT
/** \brief Exit point of function WdgM_ASR40_RteGlobalModeSwitch() */
#define DBG_WDGM_ASR40_RTEGLOBALMODESWITCH_EXIT(a)
#endif

#ifndef DBG_WDGM_ASR40_RTEINDIVIDUALMODESWITCH_ENTRY
/** \brief Entry point of function WdgM_ASR40_RteIndividualModeSwitch() */
#define DBG_WDGM_ASR40_RTEINDIVIDUALMODESWITCH_ENTRY(a,b,c)
#endif

#ifndef DBG_WDGM_ASR40_RTEINDIVIDUALMODESWITCH_EXIT
/** \brief Exit point of function WdgM_ASR40_RteIndividualModeSwitch() */
#define DBG_WDGM_ASR40_RTEINDIVIDUALMODESWITCH_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_ASR40_RTE_UPDATEALIVECOUNTER_ENTRY
/** \brief Entry point of function WdgM_ASR40_Rte_UpdateAliveCounter() */
#define DBG_WDGM_ASR40_RTE_UPDATEALIVECOUNTER_ENTRY(a)
#endif

#ifndef DBG_WDGM_ASR40_RTE_UPDATEALIVECOUNTER_EXIT
/** \brief Exit point of function WdgM_ASR40_Rte_UpdateAliveCounter() */
#define DBG_WDGM_ASR40_RTE_UPDATEALIVECOUNTER_EXIT(a,b)
#endif

#ifndef DBG_WDGM_ASR40_RTE_CHECKPOINTREACHED_ENTRY
/** \brief Entry point of function WdgM_ASR40_Rte_CheckpointReached() */
#define DBG_WDGM_ASR40_RTE_CHECKPOINTREACHED_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_ASR40_RTE_CHECKPOINTREACHED_EXIT
/** \brief Exit point of function WdgM_ASR40_Rte_CheckpointReached() */
#define DBG_WDGM_ASR40_RTE_CHECKPOINTREACHED_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_ASR43_RTEGLOBALMODESWITCH_ENTRY
/** \brief Entry point of function WdgM_ASR40_RteGlobalModeSwitch() */
#define DBG_WDGM_ASR43_RTEGLOBALMODESWITCH_ENTRY(a)
#endif

#ifndef DBG_WDGM_ASR43_RTEGLOBALMODESWITCH_EXIT
/** \brief Exit point of function WdgM_ASR40_RteGlobalModeSwitch() */
#define DBG_WDGM_ASR43_RTEGLOBALMODESWITCH_EXIT(a)
#endif

#ifndef DBG_WDGM_ASR43_RTEINDIVIDUALMODESWITCH_ENTRY
/** \brief Entry point of function WdgM_ASR40_RteIndividualModeSwitch() */
#define DBG_WDGM_ASR43_RTEINDIVIDUALMODESWITCH_ENTRY(a,b,c)
#endif

#ifndef DBG_WDGM_ASR43_RTEINDIVIDUALMODESWITCH_EXIT
/** \brief Exit point of function WdgM_ASR40_RteIndividualModeSwitch() */
#define DBG_WDGM_ASR43_RTEINDIVIDUALMODESWITCH_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_ASR43_RTE_CHECKPOINTREACHED_ENTRY
/** \brief Entry point of function WdgM_ASR40_Rte_CheckpointReached() */
#define DBG_WDGM_ASR43_RTE_CHECKPOINTREACHED_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_ASR43_RTE_CHECKPOINTREACHED_EXIT
/** \brief Exit point of function WdgM_ASR40_Rte_CheckpointReached() */
#define DBG_WDGM_ASR43_RTE_CHECKPOINTREACHED_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_RTE_UPDATEALIVECOUNTER_ENTRY
/** \brief Entry point of function WdgM_Rte_UpdateAliveCounter() */
#define DBG_WDGM_RTE_UPDATEALIVECOUNTER_ENTRY(a)
#endif

#ifndef DBG_WDGM_RTE_UPDATEALIVECOUNTER_EXIT
/** \brief Exit point of function WdgM_Rte_UpdateAliveCounter() */
#define DBG_WDGM_RTE_UPDATEALIVECOUNTER_EXIT(a,b)
#endif

#ifndef DBG_WDGM_RTE_ACTIVATEALIVESUPERVISION_ENTRY
/** \brief Entry point of function WdgM_Rte_ActivateAliveSupervision() */
#define DBG_WDGM_RTE_ACTIVATEALIVESUPERVISION_ENTRY(a)
#endif

#ifndef DBG_WDGM_RTE_ACTIVATEALIVESUPERVISION_EXIT
/** \brief Exit point of function WdgM_Rte_ActivateAliveSupervision() */
#define DBG_WDGM_RTE_ACTIVATEALIVESUPERVISION_EXIT(a,b)
#endif

#ifndef DBG_WDGM_RTE_DEACTIVATEALIVESUPERVISION_ENTRY
/** \brief Entry point of function WdgM_Rte_DeactivateAliveSupervision() */
#define DBG_WDGM_RTE_DEACTIVATEALIVESUPERVISION_ENTRY(a)
#endif

#ifndef DBG_WDGM_RTE_DEACTIVATEALIVESUPERVISION_EXIT
/** \brief Exit point of function WdgM_Rte_DeactivateAliveSupervision() */
#define DBG_WDGM_RTE_DEACTIVATEALIVESUPERVISION_EXIT(a,b)
#endif

#ifndef DBG_WDGM_RTEINDIVIDUALMODESWITCH_ENTRY
/** \brief Entry point of function WdgM_RteIndividualModeSwitch() */
#define DBG_WDGM_RTEINDIVIDUALMODESWITCH_ENTRY(a,b,c)
#endif

#ifndef DBG_WDGM_RTEINDIVIDUALMODESWITCH_EXIT
/** \brief Exit point of function WdgM_RteIndividualModeSwitch() */
#define DBG_WDGM_RTEINDIVIDUALMODESWITCH_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_RTEGLOBALMODESWITCH_ENTRY
/** \brief Entry point of function WdgM_RteGlobalModeSwitch() */
#define DBG_WDGM_RTEGLOBALMODESWITCH_ENTRY(a)
#endif

#ifndef DBG_WDGM_RTEGLOBALMODESWITCH_EXIT
/** \brief Exit point of function WdgM_RteGlobalModeSwitch() */
#define DBG_WDGM_RTEGLOBALMODESWITCH_EXIT(a)
#endif

#ifndef DBG_WDGM_RTE_CHECKPOINTREACHED_ENTRY
/** \brief Entry point of function WdgM_CheckpointReached() */
#define DBG_WDGM_RTE_CHECKPOINTREACHED_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_RTE_CHECKPOINTREACHED_EXIT
/** \brief Exit point of function WdgM_CheckpointReached() */
#define DBG_WDGM_RTE_CHECKPOINTREACHED_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_EB_DECMAXCYCLESDMDATA_ENTRY
/** \brief Entry point of function WdgM_EB_DecMaxCyclesDMData() */
#define DBG_WDGM_EB_DECMAXCYCLESDMDATA_ENTRY(a)
#endif

#ifndef DBG_WDGM_EB_DECMAXCYCLESDMDATA_EXIT
/** \brief Exit point of function WdgM_EB_DecMaxCyclesDMData() */
#define DBG_WDGM_EB_DECMAXCYCLESDMDATA_EXIT(a,b)
#endif

#ifndef DBG_WDGM_EB_CHECKMODESWITCHTIMEFRAME_ENTRY
/** \brief Entry point of function WdgM_EB_CheckModeSwitchTimeFrame() */
#define DBG_WDGM_EB_CHECKMODESWITCHTIMEFRAME_ENTRY()
#endif

#ifndef DBG_WDGM_EB_CHECKMODESWITCHTIMEFRAME_EXIT
/** \brief Exit point of function WdgM_EB_CheckModeSwitchTimeFrame() */
#define DBG_WDGM_EB_CHECKMODESWITCHTIMEFRAME_EXIT()
#endif

#ifndef DBG_WDGM_RTE_GETGLOBALSTATUS_ENTRY
/** \brief Entry point of function WdgM_Rte_GetGlobalStatus() */
#define DBG_WDGM_RTE_GETGLOBALSTATUS_ENTRY(a)
#endif

#ifndef DBG_WDGM_RTE_GETGLOBALSTATUS_EXIT
/** \brief Exit point of function WdgM_Rte_GetGlobalStatus() */
#define DBG_WDGM_RTE_GETGLOBALSTATUS_EXIT(a,b)
#endif

#ifndef DBG_WDGM_RTE_GETLOCALSTATUS_ENTRY
/** \brief Entry point of function WdgM_Rte_GetLocalStatus() */
#define DBG_WDGM_RTE_GETLOCALSTATUS_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_RTE_GETLOCALSTATUS_EXIT
/** \brief Exit point of function WdgM_Rte_GetLocalStatus() */
#define DBG_WDGM_RTE_GETLOCALSTATUS_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_RTE_GETFIRSTEXPIREDSEID_ENTRY
/** \brief Entry point of function WdgM_Rte_GetFirstExpiredSEID() */
#define DBG_WDGM_RTE_GETFIRSTEXPIREDSEID_ENTRY(a)
#endif

#ifndef DBG_WDGM_RTE_GETFIRSTEXPIREDSEID_EXIT
/** \brief Exit point of function WdgM_Rte_GetFirstExpiredSEID() */
#define DBG_WDGM_RTE_GETFIRSTEXPIREDSEID_EXIT(a,b)
#endif

#ifndef DBG_WDGM_RTE_GETMODE_ENTRY
/** \brief Entry point of function WdgM_Rte_GetMode() */
#define DBG_WDGM_RTE_GETMODE_ENTRY(a)
#endif

#ifndef DBG_WDGM_RTE_GETMODE_EXIT
/** \brief Exit point of function WdgM_Rte_GetMode() */
#define DBG_WDGM_RTE_GETMODE_EXIT(a,b)
#endif

#ifndef DBG_WDGM_RTE_SETMODE_ENTRY
/** \brief Entry point of function WdgM_Rte_SetMode() */
#define DBG_WDGM_RTE_SETMODE_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_RTE_SETMODE_EXIT
/** \brief Exit point of function WdgM_Rte_SetMode() */
#define DBG_WDGM_RTE_SETMODE_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_RTE_PERFORMRESETE_ENTRY
/** \brief Entry point of function WdgM_Rte_PerformReset() */
#define DBG_WDGM_RTE_PERFORMRESETE_ENTRY()
#endif

#ifndef DBG_WDGM_RTE_PERFORMRESET_EXIT
/** \brief Exit point of function WdgM_Rte_PerformReset() */
#define DBG_WDGM_RTE_PERFORMRESET_EXIT()
#endif

#ifndef DBG_WDGM_ASR43_RTE_GETGLOBALSTATUS_ENTRY
/** \brief Entry point of function WdgM_Rte_GetGlobalStatus() */
#define DBG_WDGM_ASR43_RTE_GETGLOBALSTATUS_ENTRY(a)
#endif

#ifndef DBG_WDGM_ASR43_RTE_GETGLOBALSTATUS_EXIT
/** \brief Exit point of function WdgM_Rte_GetGlobalStatus() */
#define DBG_WDGM_ASR43_RTE_GETGLOBALSTATUS_EXIT(a,b)
#endif

#ifndef DBG_WDGM_ASR43_RTE_GETLOCALSTATUS_ENTRY
/** \brief Entry point of function WdgM_Rte_GetLocalStatus() */
#define DBG_WDGM_ASR43_RTE_GETLOCALSTATUS_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_ASR43_RTE_GETLOCALSTATUS_EXIT
/** \brief Exit point of function WdgM_Rte_GetLocalStatus() */
#define DBG_WDGM_ASR43_RTE_GETLOCALSTATUS_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_ASR43_RTE_GETFIRSTEXPIREDSEID_ENTRY
/** \brief Entry point of function WdgM_Rte_GetFirstExpiredSEID() */
#define DBG_WDGM_ASR43_RTE_GETFIRSTEXPIREDSEID_ENTRY(a)
#endif

#ifndef DBG_WDGM_ASR43_RTE_GETFIRSTEXPIREDSEID_EXIT
/** \brief Exit point of function WdgM_Rte_GetFirstExpiredSEID() */
#define DBG_WDGM_ASR43_RTE_GETFIRSTEXPIREDSEID_EXIT(a,b)
#endif

#ifndef DBG_WDGM_ASR43_RTE_GETMODE_ENTRY
/** \brief Entry point of function WdgM_Rte_GetMode() */
#define DBG_WDGM_ASR43_RTE_GETMODE_ENTRY(a)
#endif

#ifndef DBG_WDGM_ASR43_RTE_GETMODE_EXIT
/** \brief Exit point of function WdgM_Rte_GetMode() */
#define DBG_WDGM_ASR43_RTE_GETMODE_EXIT(a,b)
#endif

#ifndef DBG_WDGM_ASR43_RTE_SETMODE_ENTRY
/** \brief Entry point of function WdgM_Rte_SetMode() */
#define DBG_WDGM_ASR43_RTE_SETMODE_ENTRY(a,b)
#endif

#ifndef DBG_WDGM_ASR43_RTE_SETMODE_EXIT
/** \brief Exit point of function WdgM_Rte_SetMode() */
#define DBG_WDGM_ASR43_RTE_SETMODE_EXIT(a,b,c)
#endif

#ifndef DBG_WDGM_ASR43_RTE_PERFORMRESETE_ENTRY
/** \brief Entry point of function WdgM_Rte_PerformReset() */
#define DBG_WDGM_ASR43_RTE_PERFORMRESETE_ENTRY()
#endif

#ifndef DBG_WDGM_ASR43_RTE_PERFORMRESET_EXIT
/** \brief Exit point of function WdgM_Rte_PerformReset() */
#define DBG_WDGM_ASR43_RTE_PERFORMRESET_EXIT()
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef WDGM_TRACE_STC_H */
/*==================[end of file]===========================================*/
