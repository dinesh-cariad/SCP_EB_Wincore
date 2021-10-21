/**
 * \file
 *
 * \brief AUTOSAR StbM
 *
 * This file contains the implementation of the AUTOSAR
 * module StbM.
 *
 * \version 3.1.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef STBM_TRACE_H
#define STBM_TRACE_H
/*==================[inclusions]============================================*/

[!IF "node:exists(as:modconf('Dbg'))"!]
#include <Dbg.h>
[!ENDIF!]

/*==================[macros]================================================*/

#ifndef DBG_STBM_INIT_ENTRY
/** \brief Entry point of function StbM_Init */
#define DBG_STBM_INIT_ENTRY(a)
#endif

#ifndef DBG_STBM_INIT_EXIT
/** \brief Exit point of function StbM_Init */
#define DBG_STBM_INIT_EXIT(a)
#endif


#ifndef DBG_STBM_COPYSYNCMEASUREMENTDATABLOCKBYBLOCK_ENTRY
/** \brief Entry point of function StbM_CopySyncMeasurementDataBlockByBlock */
#define DBG_STBM_COPYSYNCMEASUREMENTDATABLOCKBYBLOCK_ENTRY()
#endif

#ifndef DBG_STBM_COPYSYNCMEASUREMENTDATABLOCKBYBLOCK_EXIT
/** \brief Exit point of function StbM_CopySyncMeasurementDataBlockByBlock */
#define DBG_STBM_COPYSYNCMEASUREMENTDATABLOCKBYBLOCK_EXIT()
#endif

#ifndef DBG_STBM_COPYOFFSETMEASUREMENTDATABLOCKBYBLOCK_ENTRY
/** \brief Entry point of function StbM_CopyOffsetMeasurementDataBlockByBlock */
#define DBG_STBM_COPYOFFSETMEASUREMENTDATABLOCKBYBLOCK_ENTRY()
#endif

#ifndef DBG_STBM_COPYOFFSETMEASUREMENTDATABLOCKBYBLOCK_EXIT
/** \brief Exit point of function StbM_CopyOffsetMeasurementDataBlockByBlock */
#define DBG_STBM_COPYOFFSETMEASUREMENTDATABLOCKBYBLOCK_EXIT()
#endif



#ifndef DBG_STBM_COPYCANTSYNMASTERTIMINGDATABLOCKBYBLOCK_ENTRY
/** \brief Entry point of function StbM_CopyCanTSynMasterTimingDataBlockByBlock */
#define DBG_STBM_COPYCANTSYNMASTERTIMINGDATABLOCKBYBLOCK_ENTRY()
#endif

#ifndef DBG_STBM_COPYCANTSYNMASTERTIMINGDATABLOCKBYBLOCK_EXIT
/** \brief Exit point of function StbM_CopyCanTSynMasterTimingDataBlockByBlock */
#define DBG_STBM_COPYCANTSYNMASTERTIMINGDATABLOCKBYBLOCK_EXIT()
#endif


#ifndef DBG_STBM_COPYCANTSYNSLAVETIMINGDATABLOCKBYBLOCK_ENTRY
/** \brief Entry point of function StbM_CopyCanTSynSlaveTimingDataBlockByBlock */
#define DBG_STBM_COPYCANTSYNSLAVETIMINGDATABLOCKBYBLOCK_ENTRY()
#endif

#ifndef DBG_STBM_COPYCANTSYNSLAVETIMINGDATABLOCKBYBLOCK_EXIT
/** \brief Exit point of function StbM_CopyCanTSynSlaveTimingDataBlockByBlock */
#define DBG_STBM_COPYCANTSYNSLAVETIMINGDATABLOCKBYBLOCK_EXIT()
#endif


#ifndef DBG_STBM_COPYFRTSYNMASTERTIMINGDATABLOCKBYBLOCK_ENTRY
/** \brief Entry point of function StbM_CopyFrTSynMasterTimingDataBlockByBlock */
#define DBG_STBM_COPYFRTSYNMASTERTIMINGDATABLOCKBYBLOCK_ENTRY()
#endif

#ifndef DBG_STBM_COPYFRTSYNMASTERTIMINGDATABLOCKBYBLOCK_EXIT
/** \brief Exit point of function StbM_CopyFrTSynMasterTimingDataBlockByBlock */
#define DBG_STBM_COPYFRTSYNMASTERTIMINGDATABLOCKBYBLOCK_EXIT()
#endif


#ifndef DBG_STBM_COPYFRTSYNSLAVETIMINGDATABLOCKBYBLOCK_ENTRY
/** \brief Entry point of function StbM_CopyFrTSynSlaveTimingDataBlockByBlock */
#define DBG_STBM_COPYFRTSYNSLAVETIMINGDATABLOCKBYBLOCK_ENTRY()
#endif

#ifndef DBG_STBM_COPYFRTSYNSLAVETIMINGDATABLOCKBYBLOCK_EXIT
/** \brief Exit point of function StbM_CopyFrTSynSlaveTimingDataBlockByBlock */
#define DBG_STBM_COPYFRTSYNSLAVETIMINGDATABLOCKBYBLOCK_EXIT()
#endif


#ifndef DBG_STBM_COPYETHTSYNMASTERTIMINGDATABLOCKBYBLOCK_ENTRY
/** \brief Entry point of function StbM_CopyEthTSynMasterTimingDataBlockByBlock */
#define DBG_STBM_COPYETHTSYNMASTERTIMINGDATABLOCKBYBLOCK_ENTRY()
#endif

#ifndef DBG_STBM_COPYETHTSYNMASTERTIMINGDATABLOCKBYBLOCK_EXIT
/** \brief Exit point of function StbM_CopyEthTSynMasterTimingDataBlockByBlock */
#define DBG_STBM_COPYETHTSYNMASTERTIMINGDATABLOCKBYBLOCK_EXIT()
#endif


#ifndef DBG_STBM_COPYETHTSYNSLAVETIMINGDATABLOCKBYBLOCK_ENTRY
/** \brief Entry point of function StbM_CopyEthTSynSlaveTimingDataBlockByBlock */
#define DBG_STBM_COPYETHTSYNSLAVETIMINGDATABLOCKBYBLOCK_ENTRY()
#endif

#ifndef DBG_STBM_COPYETHTSYNSLAVETIMINGDATABLOCKBYBLOCK_EXIT
/** \brief Exit point of function StbM_CopyEthTSynSlaveTimingDataBlockByBlock */
#define DBG_STBM_COPYETHTSYNSLAVETIMINGDATABLOCKBYBLOCK_EXIT()
#endif


#ifndef DBG_STBM_COPYETHTSYNPDELAYRESPONDERTIMINGDATABLOCKBYBLOCK_ENTRY
/** \brief Entry point of function StbM_CopyEthTSynPdelayResponderTimingDataBlockByBlock */
#define DBG_STBM_COPYETHTSYNPDELAYRESPONDERTIMINGDATABLOCKBYBLOCK_ENTRY()
#endif

#ifndef DBG_STBM_COPYETHTSYNPDELAYRESPONDERTIMINGDATABLOCKBYBLOCK_EXIT
/** \brief Exit point of function StbM_CopyEthTSynPdelayResponderTimingDataBlockByBlock */
#define DBG_STBM_COPYETHTSYNPDELAYRESPONDERTIMINGDATABLOCKBYBLOCK_EXIT()
#endif


#ifndef DBG_STBM_COPYETHTSYNPDELAYINITIATORTIMINGDATABLOCKBYBLOCK_ENTRY
/** \brief Entry point of function StbM_CopyEthTSynPdelayInitiatorTimingDataBlockByBlock */
#define DBG_STBM_COPYETHTSYNPDELAYINITIATORTIMINGDATABLOCKBYBLOCK_ENTRY()
#endif

#ifndef DBG_STBM_COPYETHTSYNPDELAYINITIATORTIMINGDATABLOCKBYBLOCK_EXIT
/** \brief Exit point of function StbM_CopyEthTSynPdelayInitiatorTimingDataBlockByBlock */
#define DBG_STBM_COPYETHTSYNPDELAYINITIATORTIMINGDATABLOCKBYBLOCK_EXIT()
#endif




#ifndef DBG_STBM_SENDSYNCMEASUREMENTDATABLOCKBYBLOCK_ENTRY
/** \brief Entry point of function StbM_SendSyncMeasurementDataBlockByBlock */
#define DBG_STBM_SENDSYNCMEASUREMENTDATABLOCKBYBLOCK_ENTRY()
#endif

#ifndef DBG_STBM_SENDSYNCMEASUREMENTDATABLOCKBYBLOCK_EXIT
/** \brief Exit point of function StbM_SendSyncMeasurementDataBlockByBlock */
#define DBG_STBM_SENDSYNCMEASUREMENTDATABLOCKBYBLOCK_EXIT()
#endif

#ifndef DBG_STBM_SENDOFFSETMEASUREMENTDATABLOCKBYBLOCK_ENTRY
/** \brief Entry point of function StbM_SendOffsetMeasurementDataBlockByBlock */
#define DBG_STBM_SENDOFFSETMEASUREMENTDATABLOCKBYBLOCK_ENTRY()
#endif

#ifndef DBG_STBM_SENDOFFSETMEASUREMENTDATABLOCKBYBLOCK_EXIT
/** \brief Exit point of function StbM_SendOffsetMeasurementDataBlockByBlock */
#define DBG_STBM_SENDOFFSETMEASUREMENTDATABLOCKBYBLOCK_EXIT()
#endif


#ifndef DBG_STBM_CHECKIFNEWMEASUREMENTDATAISAVAILABLEANDSEND_ENTRY
/** \brief Entry point of function StbM_CheckIfNewMeasurementDataIsAvailableAndSend */
#define DBG_STBM_CHECKIFNEWMEASUREMENTDATAISAVAILABLEANDSEND_ENTRY()
#endif

#ifndef DBG_STBM_CHECKIFNEWMEASUREMENTDATAISAVAILABLEANDSEND_EXIT
/** \brief Exit point of function StbM_CheckIfNewMeasurementDataIsAvailableAndSend */
#define DBG_STBM_CHECKIFNEWMEASUREMENTDATAISAVAILABLEANDSEND_EXIT()
#endif



#ifndef DBG_STBM_SENDSYNCMEASUREMENTDATA_ENTRY
/** \brief Entry point of function StbM_SendSyncMeasurementData */
#define DBG_STBM_SENDSYNCMEASUREMENTDATA_ENTRY()
#endif

#ifndef DBG_STBM_SENDSYNCMEASUREMENTDATA_EXIT
/** \brief Exit point of function StbM_SendSyncMeasurementData */
#define DBG_STBM_SENDSYNCMEASUREMENTDATA_EXIT()
#endif

#ifndef DBG_STBM_SENDOFFSETMEASUREMENTDATA_ENTRY
/** \brief Entry point of function StbM_SendOffsetMeasurementData */
#define DBG_STBM_SENDOFFSETMEASUREMENTDATA_ENTRY()
#endif

#ifndef DBG_STBM_SENDOFFSETMEASUREMENTDATA_EXIT
/** \brief Exit point of function StbM_SendOffsetMeasurementData */
#define DBG_STBM_SENDOFFSETMEASUREMENTDATA_EXIT()
#endif



#ifndef DBG_STBM_SENDMEASUREMENTDATA_ENTRY
/** \brief Entry point of function StbM_SendMeasurementData */
#define DBG_STBM_SENDMEASUREMENTDATA_ENTRY()
#endif

#ifndef DBG_STBM_SENDMEASUREMENTDATA_EXIT
/** \brief Exit point of function StbM_SendMeasurementData */
#define DBG_STBM_SENDMEASUREMENTDATA_EXIT()
#endif



#ifndef DBG_STBM_SENDCANTSYNTIMINGDATA_ENTRY
/** \brief Entry point of function StbM_SendCanTSynTimingData */
#define DBG_STBM_SENDCANTSYNTIMINGDATA_ENTRY()
#endif

#ifndef DBG_STBM_SENDCANTSYNTIMINGDATA_EXIT
/** \brief Exit point of function StbM_SendCanTSynTimingData */
#define DBG_STBM_SENDCANTSYNTIMINGDATA_EXIT()
#endif


#ifndef DBG_STBM_SENDCANTSYNMASTERTIMINGDATA_ENTRY
/** \brief Entry point of function StbM_SendCanTSynMasterTimingData */
#define DBG_STBM_SENDCANTSYNMASTERTIMINGDATA_ENTRY()
#endif

#ifndef DBG_STBM_SENDCANTSYNMASTERTIMINGDATA_EXIT
/** \brief Exit point of function StbM_SendCanTSynMasterTimingData */
#define DBG_STBM_SENDCANTSYNMASTERTIMINGDATA_EXIT()
#endif


#ifndef DBG_STBM_SENDCANTSYNSLAVETIMINGDATA_ENTRY
/** \brief Entry point of function StbM_SendCanTSynSlaveTimingData */
#define DBG_STBM_SENDCANTSYNSLAVETIMINGDATA_ENTRY()
#endif

#ifndef DBG_STBM_SENDCANTSYNSLAVETIMINGDATA_EXIT
/** \brief Exit point of function StbM_SendCanTSynSlaveTimingData */
#define DBG_STBM_SENDCANTSYNSLAVETIMINGDATA_EXIT()
#endif



#ifndef DBG_STBM_SENDFRTSYNTIMINGDATA_ENTRY
/** \brief Entry point of function StbM_SendFrTSynTimingData */
#define DBG_STBM_SENDFRTSYNTIMINGDATA_ENTRY()
#endif

#ifndef DBG_STBM_SENDFRTSYNTIMINGDATA_EXIT
/** \brief Exit point of function StbM_SendFrTSynTimingData */
#define DBG_STBM_SENDFRTSYNTIMINGDATA_EXIT()
#endif


#ifndef DBG_STBM_SENDFRTSYNMASTERTIMINGDATA_ENTRY
/** \brief Entry point of function StbM_SendFrTSynMasterTimingData */
#define DBG_STBM_SENDFRTSYNMASTERTIMINGDATA_ENTRY()
#endif

#ifndef DBG_STBM_SENDFRTSYNMASTERTIMINGDATA_EXIT
/** \brief Exit point of function StbM_SendFrTSynMasterTimingData */
#define DBG_STBM_SENDFRTSYNMASTERTIMINGDATA_EXIT()
#endif


#ifndef DBG_STBM_SENDFRTSYNSLAVETIMINGDATA_ENTRY
/** \brief Entry point of function StbM_SendFrTSynSlaveTimingData */
#define DBG_STBM_SENDFRTSYNSLAVETIMINGDATA_ENTRY()
#endif

#ifndef DBG_STBM_SENDFRTSYNSLAVETIMINGDATA_EXIT
/** \brief Exit point of function StbM_SendFrTSynSlaveTimingData */
#define DBG_STBM_SENDFRTSYNSLAVETIMINGDATA_EXIT()
#endif




#ifndef DBG_STBM_SENDETHTSYNTIMINGDATA_ENTRY
/** \brief Entry point of function StbM_SendEthTSynTimingData */
#define DBG_STBM_SENDETHTSYNTIMINGDATA_ENTRY()
#endif

#ifndef DBG_STBM_SENDETHTSYNTIMINGDATA_EXIT
/** \brief Exit point of function StbM_SendEthTSynTimingData */
#define DBG_STBM_SENDETHTSYNTIMINGDATA_EXIT()
#endif


#ifndef DBG_STBM_SENDETHTSYNMASTERTIMINGDATA_ENTRY
/** \brief Entry point of function StbM_SendEthTSynMasterTimingData */
#define DBG_STBM_SENDETHTSYNMASTERTIMINGDATA_ENTRY()
#endif

#ifndef DBG_STBM_SENDETHTSYNMASTERTIMINGDATA_EXIT
/** \brief Exit point of function StbM_SendEthTSynMasterTimingData */
#define DBG_STBM_SENDETHTSYNMASTERTIMINGDATA_EXIT()
#endif


#ifndef DBG_STBM_SENDETHTSYNSLAVETIMINGDATA_ENTRY
/** \brief Entry point of function StbM_SendEthTSynSlaveTimingData */
#define DBG_STBM_SENDETHTSYNSLAVETIMINGDATA_ENTRY()
#endif

#ifndef DBG_STBM_SENDETHTSYNSLAVETIMINGDATA_EXIT
/** \brief Exit point of function StbM_SendEthTSynSlaveTimingData */
#define DBG_STBM_SENDETHTSYNSLAVETIMINGDATA_EXIT()
#endif




#ifndef DBG_STBM_SENDETHTSYNPDELAYTIMINGDATA_ENTRY
/** \brief Entry point of function StbM_SendEthTSynPdelayTimingData */
#define DBG_STBM_SENDETHTSYNPDELAYTIMINGDATA_ENTRY()
#endif

#ifndef DBG_STBM_SENDETHTSYNPDELAYTIMINGDATA_EXIT
/** \brief Exit point of function StbM_SendEthTSynPdelayTimingData */
#define DBG_STBM_SENDETHTSYNPDELAYTIMINGDATA_EXIT()
#endif


#ifndef DBG_STBM_SENDETHTSYNMASTERPDELAYTIMINGDATA_ENTRY
/** \brief Entry point of function StbM_SendEthTSynMasterPdelayTimingData */
#define DBG_STBM_SENDETHTSYNMASTERPDELAYTIMINGDATA_ENTRY()
#endif

#ifndef DBG_STBM_SENDETHTSYNMASTERPDELAYTIMINGDATA_EXIT
/** \brief Exit point of function StbM_SendEthTSynMasterPdelayTimingData */
#define DBG_STBM_SENDETHTSYNMASTERPDELAYTIMINGDATA_EXIT()
#endif


#ifndef DBG_STBM_SENDETHTSYNSLAVEPDELAYTIMINGDATA_ENTRY
/** \brief Entry point of function StbM_SendEthTSynSlavePdelayTimingData */
#define DBG_STBM_SENDETHTSYNSLAVEPDELAYTIMINGDATA_ENTRY()
#endif

#ifndef DBG_STBM_SENDETHTSYNSLAVEPDELAYTIMINGDATA_EXIT
/** \brief Exit point of function StbM_SendEthTSynSlavePdelayTimingData */
#define DBG_STBM_SENDETHTSYNSLAVEPDELAYTIMINGDATA_EXIT()
#endif



#ifndef DBG_STBM_TIMENOTIFICATIONDATAINIT_ENTRY
/** \brief Entry point of function StbM_TimeNotificationDataInit */
#define DBG_STBM_TIMENOTIFICATIONDATAINIT_ENTRY(a)
#endif

#ifndef DBG_STBM_TIMENOTIFICATIONDATAINIT_EXIT
/** \brief Exit point of function StbM_TimeNotificationDataInit */
#define DBG_STBM_TIMENOTIFICATIONDATAINIT_EXIT(a)
#endif


#ifndef DBG_STBM_SENDNOTIFICATIONTOCUSTOMER_ENTRY
/** \brief Entry point of function StbM_SendNotificationToCustomer */
#define DBG_STBM_SENDNOTIFICATIONTOCUSTOMER_ENTRY()
#endif

#ifndef DBG_STBM_SENDNOTIFICATIONTOCUSTOMER_EXIT
/** \brief Exit point of function StbM_SendNotificationToCustomer */
#define DBG_STBM_SENDNOTIFICATIONTOCUSTOMER_EXIT()
#endif


#ifndef DBG_STBM_CALCULATETIMEDEVIATIONFORIMMEDIATENOTIFICATION_ENTRY
/** \brief Entry point of function StbM_CalculateTimeDeviationForImmediateNotification */
#define DBG_STBM_CALCULATETIMEDEVIATIONFORIMMEDIATENOTIFICATION_ENTRY(a,b)
#endif

#ifndef DBG_STBM_CALCULATETIMEDEVIATIONFORIMMEDIATENOTIFICATION_EXIT
/** \brief Exit point of function StbM_CalculateTimeDeviationForImmediateNotification */
#define DBG_STBM_CALCULATETIMEDEVIATIONFORIMMEDIATENOTIFICATION_EXIT(a,b,c)
#endif


#ifndef DBG_STBM_TRYTOCALLGPTSTARTTIMER_ENTRY
/** \brief Entry point of function StbM_TryToCallGptStartTimer */
#define DBG_STBM_TRYTOCALLGPTSTARTTIMER_ENTRY(a,b)
#endif

#ifndef DBG_STBM_TRYTOCALLGPTSTARTTIMER_EXIT
/** \brief Exit point of function StbM_TryToCallGptStartTimer */
#define DBG_STBM_TRYTOCALLGPTSTARTTIMER_EXIT(a,b)
#endif

#ifndef DBG_STBM_GETMAXALLWEDLIMIT_ENTRY
/** \brief Entry point of function StbM_GetMaxAllowedLimit */
#define DBG_STBM_GETMAXALLWEDLIMIT_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_GETMAXALLWEDLIMIT_EXIT
/** \brief Exit point of function StbM_GetMaxAllowedLimit */
#define DBG_STBM_GETMAXALLWEDLIMIT_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_TRYTOCALLGPTSTARTTIMERBYARRAYINDEX_ENTRY
/** \brief Entry point of function StbM_TryToCallGptStartTimerByArrayIndex */
#define DBG_STBM_TRYTOCALLGPTSTARTTIMERBYARRAYINDEX_ENTRY(a,b)
#endif

#ifndef DBG_STBM_TRYTOCALLGPTSTARTTIMERBYARRAYINDEX_EXIT
/** \brief Exit point of function StbM_TryToCallGptStartTimerByArrayIndex */
#define DBG_STBM_TRYTOCALLGPTSTARTTIMERBYARRAYINDEX_EXIT(a,b)
#endif


#ifndef DBG_STBM_RATECORRECTIONDATAINIT_ENTRY
/** \brief Entry point of function StbM_RateCorrectionDataInit */
#define DBG_STBM_RATECORRECTIONDATAINIT_ENTRY()
#endif

#ifndef DBG_STBM_RATECORRECTIONDATAINIT_EXIT
/** \brief Exit point of function StbM_RateCorrectionDataInit */
#define DBG_STBM_RATECORRECTIONDATAINIT_EXIT()
#endif

#ifndef DBG_STBM_SLAVEDATAINIT_ENTRY
/** \brief Entry point of function StbM_SlaveDataInit */
#define DBG_STBM_SLAVEDATAINIT_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_SLAVEDATAINIT_EXIT
/** \brief Exit point of function StbM_SlaveDataInit */
#define DBG_STBM_SLAVEDATAINIT_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_SLAVEDATAREINITMEASUREMENTS_ENTRY
/** \brief Entry point of function StbM_SlaveDataReinitMeasurements */
#define DBG_STBM_SLAVEDATAREINITMEASUREMENTS_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_SLAVEDATAREINITMEASUREMENTS_EXIT
/** \brief Exit point of function StbM_SlaveDataReinitMeasurements */
#define DBG_STBM_SLAVEDATAREINITMEASUREMENTS_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_GETVERSIONINFO_ENTRY
/** \brief Entry point of function StbM_GetVersionInfo */
#define DBG_STBM_GETVERSIONINFO_ENTRY(a)
#endif

#ifndef DBG_STBM_GETVERSIONINFO_EXIT
/** \brief Exit point of function StbM_GetVersionInfo */
#define DBG_STBM_GETVERSIONINFO_EXIT(a)
#endif

#ifndef DBG_STBM_RESETOFFSETCORRECTIONVALUES_ENTRY
/** \brief Entry point of function StbM_ResetOffsetCorrectionValues */
#define DBG_STBM_RESETOFFSETCORRECTIONVALUES_ENTRY(a)
#endif

#ifndef DBG_STBM_RESETOFFSETCORRECTIONVALUES_EXIT
/** \brief Exit point of function StbM_ResetOffsetCorrectionValues */
#define DBG_STBM_RESETOFFSETCORRECTIONVALUES_EXIT(a)
#endif

#ifndef DBG_STBM_MAINFUNCTION_ENTRY
/** \brief Entry point of function StbM_MainFunction */
#define DBG_STBM_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_STBM_MAINFUNCTION_EXIT
/** \brief Exit point of function StbM_MainFunction */
#define DBG_STBM_MAINFUNCTION_EXIT()
#endif


#ifndef DBG_STBM_GETCURRENTTIMECOMMON_ENTRY
/** \brief Entry point of function StbM_GetCurrentTimeCommon */
#define DBG_STBM_GETCURRENTTIMECOMMON_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_GETCURRENTTIMECOMMON_EXIT
/** \brief Exit point of function StbM_GetCurrentTimeCommon */
#define DBG_STBM_GETCURRENTTIMECOMMON_EXIT(a,b,c,d)
#endif


#ifndef DBG_STBM_GETCURRENTTIME_ENTRY
/** \brief Entry point of function StbM_GetCurrentTime */
#define DBG_STBM_GETCURRENTTIME_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_GETCURRENTTIME_EXIT
/** \brief Exit point of function StbM_GetCurrentTime */
#define DBG_STBM_GETCURRENTTIME_EXIT(a,b,c,d)
#endif


#ifndef DBG_STBM_GETCURRENTTIMEEXTENDED_ENTRY
/** \brief Entry point of function StbM_GetCurrentTimeExtended */
#define DBG_STBM_GETCURRENTTIMEEXTENDED_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_GETCURRENTTIMEEXTENDED_EXIT
/** \brief Exit point of function StbM_GetCurrentTimeExtended */
#define DBG_STBM_GETCURRENTTIMEEXTENDED_EXIT(a,b,c,d)
#endif


#ifndef DBG_STBM_GETMASTERCONFIG_ENTRY
/** \brief Entry point of function StbM_GetMasterConfig */
#define DBG_STBM_GETMASTERCONFIG_ENTRY(a,b)
#endif

#ifndef DBG_STBM_GETMASTERCONFIG_EXIT
/** \brief Exit point of function StbM_GetMasterConfig */
#define DBG_STBM_GETMASTERCONFIG_EXIT(a,b,c)
#endif


#ifndef DBG_STBM_GETOFFSETTIMEBASETIME_ENTRY
/** \brief Entry point of function StbM_GetOffsetTimeBaseTime */
#define DBG_STBM_GETOFFSETTIMEBASETIME_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_STBM_GETOFFSETTIMEBASETIME_EXIT
/** \brief Exit point of function StbM_GetOffsetTimeBaseTime */
#define DBG_STBM_GETOFFSETTIMEBASETIME_EXIT(a,b,c,d,e,f,g)
#endif


#ifndef DBG_STBM_BUSGETCURRENTTIME_ENTRY
/** \brief Entry point of function StbM_BusGetCurrentTime */
#define DBG_STBM_BUSGETCURRENTTIME_ENTRY(a,b,c,d)
#endif

#ifndef DBG_STBM_BUSGETCURRENTTIME_EXIT
/** \brief Exit point of function StbM_BusGetCurrentTime */
#define DBG_STBM_BUSGETCURRENTTIME_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_STBM_GETCURRENTVIRTUALLOCALTIME_ENTRY
/** \brief Entry point of function StbM_GetCurrentVirtualLocalTime */
#define DBG_STBM_GETCURRENTVIRTUALLOCALTIME_ENTRY(a,b)
#endif

#ifndef DBG_STBM_GETCURRENTVIRTUALLOCALTIME_EXIT
/** \brief Exit point of function StbM_GetCurrentVirtualLocalTime */
#define DBG_STBM_GETCURRENTVIRTUALLOCALTIME_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_GETCURRENTVIRTUALLOCALTIMENOCHECKS_ENTRY
/** \brief Entry point of function StbM_GetCurrentVirtualLocalTimeNoChecks */
#define DBG_STBM_GETCURRENTVIRTUALLOCALTIMENOCHECKS_ENTRY(a,b)
#endif

#ifndef DBG_STBM_GETCURRENTVIRTUALLOCALTIMENOCHECKS_EXIT
/** \brief Exit point of function StbM_GetCurrentVirtualLocalTimeNoChecks */
#define DBG_STBM_GETCURRENTVIRTUALLOCALTIMENOCHECKS_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_GETCURRENTVIRTUALLOCALTIMENODET_ENTRY
/** \brief Entry point of function StbM_GetCurrentVirtualLocalTimeNoDet */
#define DBG_STBM_GETCURRENTVIRTUALLOCALTIMENODET_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_STBM_GETCURRENTVIRTUALLOCALTIMENODET_EXIT
/** \brief Exit point of function StbM_GetCurrentVirtualLocalTimeNoDet */
#define DBG_STBM_GETCURRENTVIRTUALLOCALTIMENODET_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_STBM_SETGLOBALTIME_ENTRY
/** \brief Entry point of function StbM_SetGlobalTime */
#define DBG_STBM_SETGLOBALTIME_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_SETGLOBALTIME_EXIT
/** \brief Exit point of function StbM_SetGlobalTime */
#define DBG_STBM_SETGLOBALTIME_EXIT(a,b,c,d)
#endif



#ifndef DBG_STBM_INITTIMEVALIDATIONINFOBLOCKS_ENTRY
/** \brief Entry point of function StbM_InitTimeValidationInfoBlocks */
#define DBG_STBM_INITTIMEVALIDATIONINFOBLOCKS_ENTRY()
#endif

#ifndef DBG_STBM_INITTIMEVALIDATIONINFOBLOCKS_EXIT
/** \brief Exit point of function StbM_InitTimeValidationInfoBlocks */
#define DBG_STBM_INITTIMEVALIDATIONINFOBLOCKS_EXIT()
#endif

#ifndef DBG_STBM_INITTIMEVALIDATIONINFO_ENTRY
/** \brief Entry point of function StbM_InitTimeValidationInfo */
#define DBG_STBM_INITTIMEVALIDATIONINFO_ENTRY()
#endif

#ifndef DBG_STBM_INITTIMEVALIDATIONINFO_EXIT
/** \brief Exit point of function StbM_InitTimeValidationInfo */
#define DBG_STBM_INITTIMEVALIDATIONINFO_EXIT()
#endif

#ifndef DBG_STBM_CANMASTERTIMINGDATAINIT_ENTRY
/** \brief Entry point of function StbM_CanMasterTimingDataInit */
#define DBG_STBM_CANMASTERTIMINGDATAINIT_ENTRY()
#endif

#ifndef DBG_STBM_CANMASTERTIMINGDATAINIT_EXIT
/** \brief Exit point of function StbM_CanMasterTimingDataInit */
#define DBG_STBM_CANMASTERTIMINGDATAINIT_EXIT()
#endif

#ifndef DBG_STBM_CANSLAVETIMINGDATAINIT_ENTRY
/** \brief Entry point of function StbM_CanSlaveTimingDataInit */
#define DBG_STBM_CANSLAVETIMINGDATAINIT_ENTRY()
#endif

#ifndef DBG_STBM_CANSLAVETIMINGDATAINIT_EXIT
/** \brief Exit point of function StbM_CanSlaveTimingDataInit */
#define DBG_STBM_CANSLAVETIMINGDATAINIT_EXIT()
#endif


#ifndef DBG_STBM_FRMASTERTIMINGDATAINIT_ENTRY
/** \brief Entry point of function StbM_FrMasterTimingDataInit */
#define DBG_STBM_FRMASTERTIMINGDATAINIT_ENTRY()
#endif

#ifndef DBG_STBM_FRMASTERTIMINGDATAINIT_EXIT
/** \brief Exit point of function StbM_FrMasterTimingDataInit */
#define DBG_STBM_FRMASTERTIMINGDATAINIT_EXIT()
#endif

#ifndef DBG_STBM_FRSLAVETIMINGDATAINIT_ENTRY
/** \brief Entry point of function StbM_FrSlaveTimingDataInit */
#define DBG_STBM_FRSLAVETIMINGDATAINIT_ENTRY()
#endif

#ifndef DBG_STBM_FRSLAVETIMINGDATAINIT_EXIT
/** \brief Exit point of function StbM_FrSlaveTimingDataInit */
#define DBG_STBM_FRSLAVETIMINGDATAINIT_EXIT()
#endif


#ifndef DBG_STBM_ETHMASTERTIMINGDATAINIT_ENTRY
/** \brief Entry point of function StbM_EthMasterTimingDataInit */
#define DBG_STBM_ETHMASTERTIMINGDATAINIT_ENTRY()
#endif

#ifndef DBG_STBM_ETHMASTERTIMINGDATAINIT_EXIT
/** \brief Exit point of function StbM_EthMasterTimingDataInit */
#define DBG_STBM_ETHMASTERTIMINGDATAINIT_EXIT()
#endif

#ifndef DBG_STBM_ETHSLAVETIMINGDATAINIT_ENTRY
/** \brief Entry point of function StbM_EthSlaveTimingDataInit */
#define DBG_STBM_ETHSLAVETIMINGDATAINIT_ENTRY()
#endif

#ifndef DBG_STBM_ETHSLAVETIMINGDATAINIT_EXIT
/** \brief Exit point of function StbM_EthSlaveTimingDataInit */
#define DBG_STBM_ETHSLAVETIMINGDATAINIT_EXIT()
#endif


#ifndef DBG_STBM_ETHMASTERPDELAYTIMINGDATAINIT_ENTRY
/** \brief Entry point of function StbM_EthMasterPdelayTimingDataInit */
#define DBG_STBM_ETHMASTERPDELAYTIMINGDATAINIT_ENTRY()
#endif

#ifndef DBG_STBM_ETHMASTERPDELAYTIMINGDATAINIT_EXIT
/** \brief Exit point of function StbM_EthMasterPdelayTimingDataInit */
#define DBG_STBM_ETHMASTERPDELAYTIMINGDATAINIT_EXIT()
#endif

#ifndef DBG_STBM_ETHSLAVEPDELAYTIMINGDATAINIT_ENTRY
/** \brief Entry point of function StbM_EthSlavePdelayTimingDataInit */
#define DBG_STBM_ETHSLAVEPDELAYTIMINGDATAINIT_ENTRY()
#endif

#ifndef DBG_STBM_ETHSLAVEPDELAYTIMINGDATAINIT_EXIT
/** \brief Exit point of function StbM_EthSlavePdelayTimingDataInit */
#define DBG_STBM_ETHSLAVEPDELAYTIMINGDATAINIT_EXIT()
#endif



#ifndef DBG_STBM_TIMEVALIDATIONDATAINIT_ENTRY
/** \brief Entry point of function StbM_CanSetMasterTimingData */
#define DBG_STBM_TIMEVALIDATIONDATAINIT_ENTRY()
#endif

#ifndef DBG_STBM_TIMEVALIDATIONDATAINIT_EXIT
/** \brief Exit point of function StbM_CanSetMasterTimingData */
#define DBG_STBM_TIMEVALIDATIONDATAINIT_EXIT()
#endif


#ifndef DBG_STBM_CANSETMASTERTIMINGDATA_ENTRY
/** \brief Entry point of function StbM_CanSetMasterTimingData */
#define DBG_STBM_CANSETMASTERTIMINGDATA_ENTRY(a,b)
#endif

#ifndef DBG_STBM_CANSETMASTERTIMINGDATA_EXIT
/** \brief Exit point of function StbM_CanSetMasterTimingData */
#define DBG_STBM_CANSETMASTERTIMINGDATA_EXIT(a,b,c)
#endif


#ifndef DBG_STBM_FRSETMASTERTIMINGDATA_ENTRY
/** \brief Entry point of function StbM_FrSetMasterTimingData */
#define DBG_STBM_FRSETMASTERTIMINGDATA_ENTRY(a,b)
#endif

#ifndef DBG_STBM_FRSETMASTERTIMINGDATA_EXIT
/** \brief Exit point of function StbM_FrSetMasterTimingData */
#define DBG_STBM_FRSETMASTERTIMINGDATA_EXIT(a,b,c)
#endif


#ifndef DBG_STBM_ETHSETMASTERTIMINGDATA_ENTRY
/** \brief Entry point of function StbM_EthSetMasterTimingData */
#define DBG_STBM_ETHSETMASTERTIMINGDATA_ENTRY(a,b)
#endif

#ifndef DBG_STBM_ETHSETMASTERTIMINGDATA_EXIT
/** \brief Exit point of function StbM_EthSetMasterTimingData */
#define DBG_STBM_ETHSETMASTERTIMINGDATA_EXIT(a,b,c)
#endif


#ifndef DBG_STBM_CANSETSLAVETIMINGDATA_ENTRY
/** \brief Entry point of function StbM_CanSetSlaveTimingData */
#define DBG_STBM_CANSETSLAVETIMINGDATA_ENTRY(a,b)
#endif

#ifndef DBG_STBM_CANSETSLAVETIMINGDATA_EXIT
/** \brief Exit point of function StbM_CanSetSlaveTimingData */
#define DBG_STBM_CANSETSLAVETIMINGDATA_EXIT(a,b,c)
#endif



#ifndef DBG_STBM_FRSETSLAVETIMINGDATA_ENTRY
/** \brief Entry point of function StbM_FrSetSlaveTimingData */
#define DBG_STBM_FRSETSLAVETIMINGDATA_ENTRY(a,b)
#endif

#ifndef DBG_STBM_FRSETSLAVETIMINGDATA_EXIT
/** \brief Exit point of function StbM_FrSetSlaveTimingData */
#define DBG_STBM_FRSETSLAVETIMINGDATA_EXIT(a,b,c)
#endif


#ifndef DBG_STBM_ETHSETSLAVETIMINGDATA_ENTRY
/** \brief Entry point of function StbM_EthSetSlaveTimingData */
#define DBG_STBM_ETHSETSLAVETIMINGDATA_ENTRY(a,b)
#endif

#ifndef DBG_STBM_ETHSETSLAVETIMINGDATA_EXIT
/** \brief Exit point of function StbM_EthSetSlaveTimingData */
#define DBG_STBM_ETHSETSLAVETIMINGDATA_EXIT(a,b,c)
#endif


#ifndef DBG_STBM_ETHSETPDELAYINITIATORDATA_ENTRY
/** \brief Entry point of function StbM_EthSetPdelayInitiatorData */
#define DBG_STBM_ETHSETPDELAYINITIATORDATA_ENTRY(a,b)
#endif

#ifndef DBG_STBM_ETHSETPDELAYINITIATORDATA_EXIT
/** \brief Exit point of function StbM_EthSetPdelayInitiatorData */
#define DBG_STBM_ETHSETPDELAYINITIATORDATA_EXIT(a,b,c)
#endif


#ifndef DBG_STBM_ETHSETPDELAYRESPONDERDATA_ENTRY
/** \brief Entry point of function StbM_EthSetPdelayResponderData */
#define DBG_STBM_ETHSETPDELAYRESPONDERDATA_ENTRY(a,b)
#endif

#ifndef DBG_STBM_ETHSETPDELAYRESPONDERDATA_EXIT
/** \brief Exit point of function StbM_EthSetPdelayResponderData */
#define DBG_STBM_ETHSETPDELAYRESPONDERDATA_EXIT(a,b,c)
#endif


#ifndef DBG_STBM_STARTTIMER_ENTRY
/** \brief Entry point of function StbM_StartTimer */
#define DBG_STBM_STARTTIMER_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_STARTTIMER_EXIT
/** \brief Exit point of function StbM_StartTimer */
#define DBG_STBM_STARTTIMER_EXIT(a,b,c,d)
#endif


#ifndef DBG_STBM_TIMERCALLBACK_ENTRY
/** \brief Entry point of function StbM_TimerCallback */
#define DBG_STBM_TIMERCALLBACK_ENTRY()
#endif

#ifndef DBG_STBM_TIMERCALLBACK_EXIT
/** \brief Exit point of function StbM_TimerCallback */
#define DBG_STBM_TIMERCALLBACK_EXIT()
#endif


#ifndef DBG_STBM_SENDNOTIFICATIONTOCUSTOMER_ENTRY
/** \brief Entry point of function StbM_StartTimer */
#define DBG_STBM_SENDNOTIFICATIONTOCUSTOMER_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_SENDNOTIFICATIONTOCUSTOMER_EXIT
/** \brief Exit point of function StbM_StartTimer */
#define DBG_STBM_SENDNOTIFICATIONTOCUSTOMER_EXIT(a,b,c)
#endif


#ifndef DBG_STBM_SENDNOTIFICATIONFOREXPIRYPOINTSWITHGPTTIMERALREADYEXPIRED_ENTRY
/** \brief Entry point of function StbM_SendNotificationForExpiryPointsWithGptTimerAlreadyExpired */
#define DBG_STBM_SENDNOTIFICATIONFOREXPIRYPOINTSWITHGPTTIMERALREADYEXPIRED_ENTRY()
#endif

#ifndef DBG_STBM_SENDNOTIFICATIONFOREXPIRYPOINTSWITHGPTTIMERALREADYEXPIRED_EXIT
/** \brief Exit point of function StbM_SendNotificationForExpiryPointsWithGptTimerAlreadyExpired */
#define DBG_STBM_SENDNOTIFICATIONFOREXPIRYPOINTSWITHGPTTIMERALREADYEXPIRED_EXIT()
#endif


#ifndef DBG_STBM_HANDLEEXPIRYPOINTSWITHPENDINGSTARTGPTTIMER_ENTRY
/** \brief Entry point of function StbM_HandleExpiryPointsWithPendingStartGptTimer */
#define DBG_STBM_HANDLEEXPIRYPOINTSWITHPENDINGSTARTGPTTIMER_ENTRY(a)
#endif

#ifndef DBG_STBM_HANDLEEXPIRYPOINTSWITHPENDINGSTARTGPTTIMER_EXIT
/** \brief Exit point of function StbM_HandleExpiryPointsWithPendingStartGptTimer */
#define DBG_STBM_HANDLEEXPIRYPOINTSWITHPENDINGSTARTGPTTIMER_EXIT(a)
#endif


#ifndef DBG_STBM_DIFFERENCEPASSEDOVERSTARTTHRESHOLD_ENTRY
/** \brief Entry point of function StbM_DifferencePassedOverStartThreshold */
#define DBG_STBM_DIFFERENCEPASSEDOVERSTARTTHRESHOLD_ENTRY(a)
#endif

#ifndef DBG_STBM_DIFFERENCEPASSEDOVERSTARTTHRESHOLD_EXIT
/** \brief Exit point of function StbM_DifferencePassedOverStartThreshold */
#define DBG_STBM_DIFFERENCEPASSEDOVERSTARTTHRESHOLD_EXIT(a)
#endif


#ifndef DBG_STBM_UPDATEGLOBALTIME_ENTRY
/** \brief Entry point of function StbM_UpdateGlobalTime */
#define DBG_STBM_UPDATEGLOBALTIME_ENTRY(a,b,c)
#endif


#ifndef DBG_STBM_SETUSERDATABYTES_ENTRY
/** \brief Entry point of function StbM_SetUserDataBytes */
#define DBG_STBM_SETUSERDATABYTES_ENTRY(a,b)
#endif

#ifndef DBG_STBM_SETUSERDATABYTES_EXIT
/** \brief Exit point of function StbM_SetUserDataBytes */
#define DBG_STBM_SETUSERDATABYTES_EXIT(a,b)
#endif


#ifndef DBG_STBM_SETTIME_ENTRY
/** \brief Entry point of function StbM_SetTime */
#define DBG_STBM_SETTIME_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_STBM_SETTIME_EXIT
/** \brief Exit point of function StbM_SetTime */
#define DBG_STBM_SETTIME_EXIT(a,b,c,d,e,f,g)
#endif

#ifndef DBG_STBM_UPDATEGLOBALTIME_EXIT
/** \brief Exit point of function StbM_UpdateGlobalTime */
#define DBG_STBM_UPDATEGLOBALTIME_EXIT(a,b,c,d)
#endif

#ifndef DBG_STBM_SETUSERDATA_ENTRY
/** \brief Entry point of function StbM_SetUserData */
#define DBG_STBM_SETUSERDATA_ENTRY(a,b)
#endif

#ifndef DBG_STBM_SETUSERDATA_EXIT
/** \brief Exit point of function StbM_SetUserData */
#define DBG_STBM_SETUSERDATA_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_SETOFFSET_ENTRY
/** \brief Entry point of function StbM_SetOffset */
#define DBG_STBM_SETOFFSET_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_SETOFFSET_EXIT
/** \brief Exit point of function StbM_SetOffset */
#define DBG_STBM_SETOFFSET_EXIT(a,b,c,d)
#endif

#ifndef DBG_STBM_VIRTUALTIMEISGREATER_ENTRY
/** \brief Entry point of function StbM_VirtualTimeIsGreater */
#define DBG_STBM_VIRTUALTIMEISGREATER_ENTRY(a,b)
#endif

#ifndef DBG_STBM_VIRTUALTIMEISGREATEROREQUAL_EXIT
/** \brief Exit point of function StbM_VirtualTimeIsGreater */
#define DBG_STBM_VIRTUALTIMEISGREATER_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_GETOFFSET_ENTRY
/** \brief Entry point of function StbM_GetOffset */
#define DBG_STBM_GETOFFSET_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_GETOFFSET_EXIT
/** \brief Exit point of function StbM_GetOffset */
#define DBG_STBM_GETOFFSET_EXIT(a,b,c,d)
#endif

#ifndef DBG_STBM_BUSSETGLOBALTIME_ENTRY
/** \brief Entry point of function StbM_BusSetGlobalTime */
#define DBG_STBM_BUSSETGLOBALTIME_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_STBM_BUSSETGLOBALTIME_EXIT
/** \brief Exit point of function StbM_BusSetGlobalTime */
#define DBG_STBM_BUSSETGLOBALTIME_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_STBM_BUSSETGLOBALTIME_DETCHECKS_ENTRY
/** \brief Entry point of function StbM_BusSetGlobalTime_DetChecks */
#define DBG_STBM_BUSSETGLOBALTIME_DETCHECKS_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_STBM_BUSSETGLOBALTIME_DETCHECKS_EXIT
/** \brief Exit point of function StbM_BusSetGlobalTime_DetChecks */
#define DBG_STBM_BUSSETGLOBALTIME_DETCHECKS_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_STBM_GETCURRENTTIMEINTERNAL_ENTRY
/** \brief Entry point of function StbM_GetCurrentTimeInternal */
#define DBG_STBM_GETCURRENTTIMEINTERNAL_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_STBM_GETCURRENTTIMEINTERNAL_EXIT
/** \brief Exit point of function StbM_GetCurrentTimeInternal */
#define DBG_STBM_GETCURRENTTIMEINTERNAL_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_STBM_SETGLOBALTIMEOFFSET_ENTRY
/** \brief Entry point of function StbM_SetGlobalTimeOffset */
#define DBG_STBM_SETGLOBALTIMEOFFSET_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_SETGLOBALTIMEOFFSET_EXIT
/** \brief Exit point of function StbM_SetGlobalTimeOffset */
#define DBG_STBM_SETGLOBALTIMEOFFSET_EXIT(a,b,c,d)
#endif

#ifndef DBG_STBM_SETGLOBALTIMEINTERNAL_ENTRY
/** \brief Entry point of function StbM_SetglobaltimeInternal */
#define DBG_STBM_SETGLOBALTIMEINTERNAL_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_STBM_SETGLOBALTIMEINTERNAL_EXIT
/** \brief Exit point of function StbM_SetglobaltimeInternal */
#define DBG_STBM_SETGLOBALTIMEINTERNAL_EXIT(a,b,c,d,e,f,g)
#endif

#ifndef DBG_STBM_CALCULATELOCALTIME_ENTRY
/** \brief Entry point of function StbM_CalculateLocalTime */
#define DBG_STBM_CALCULATELOCALTIME_ENTRY(a,b)
#endif

#ifndef DBG_STBM_CALCULATELOCALTIME_EXIT
/** \brief Exit point of function StbM_CalculateLocalTime */
#define DBG_STBM_CALCULATELOCALTIME_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_CALCULATELOCALTIMEANDUPDATE_ENTRY
/** \brief Entry point of function StbM_CalculateLocalTimeAndUpdate */
#define DBG_STBM_CALCULATELOCALTIMEANDUPDATE_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_CALCULATELOCALTIMEANDUPDATE_EXIT
/** \brief Exit point of function StbM_CalculateLocalTimeAndUpdate */
#define DBG_STBM_CALCULATELOCALTIMEANDUPDATE_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_VIRTUALLOCALTIMETOTIMESTAMP_ENTRY
/** \brief Entry point of function StbM_VirtualLocalTimeToTimeStamp */
#define DBG_STBM_VIRTUALLOCALTIMETOTIMESTAMP_ENTRY(a,b)
#endif

#ifndef DBG_STBM_VIRTUALLOCALTIMETOTIMESTAMP_EXIT
/** \brief Exit point of function StbM_VirtualLocalTimeToTimeStamp */
#define DBG_STBM_VIRTUALLOCALTIMETOTIMESTAMP_EXIT(a,b)
#endif

#ifndef DBG_STBM_TIMESTAMPTOVIRTUALLOCALTIME_ENTRY
/** \brief Entry point of function StbM_TimeStampToVirtualLocalTime */
#define DBG_STBM_TIMESTAMPTOVIRTUALLOCALTIME_ENTRY(a,b)
#endif

#ifndef DBG_STBM_TIMESTAMPTOVIRTUALLOCALTIME_EXIT
/** \brief Exit point of function StbM_TimeStampToVirtualLocalTime */
#define DBG_STBM_TIMESTAMPTOVIRTUALLOCALTIME_EXIT(a,b)
#endif

#ifndef DBG_STBM_GETTIME_ENTRY
/** \brief Entry point of function StbM_GetTime */
#define DBG_STBM_GETTIME_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_STBM_GETTIME_EXIT
/** \brief Exit point of function StbM_GetTime */
#define DBG_STBM_GETTIME_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_STBM_SETGLOBALTIMEBASE_ENTRY
/** \brief Entry point of function StbM_SetGlobalTimeBase */
#define DBG_STBM_SETGLOBALTIMEBASE_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_SETGLOBALTIMEBASE_EXIT
/** \brief Exit point of function StbM_SetGlobalTimeBase */
#define DBG_STBM_SETGLOBALTIMEBASE_EXIT(a,b,c,d)
#endif

#ifndef DBG_STBM_SUMTIMESTAMPS_ENTRY
/** \brief Entry point of function StbM_SumTimeStamps */
#define DBG_STBM_SUMTIMESTAMPS_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_SUMTIMESTAMPS_EXIT
/** \brief Exit point of function StbM_SumTimeStamps */
#define DBG_STBM_SUMTIMESTAMPS_EXIT(a,b,c,d)
#endif

#ifndef DBG_STBM_SUBTRACTTIMESTAMPS_ENTRY
/** \brief Entry point of function StbM_SubtractTimeStamps */
#define DBG_STBM_SUBTRACTTIMESTAMPS_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_SUBTRACTTIMESTAMPS_EXIT
/** \brief Exit point of function StbM_SubtractTimeStamps */
#define DBG_STBM_SUBTRACTTIMESTAMPS_EXIT(a,b,c,d)
#endif

#ifndef DBG_STBM_CALCULATENEWTIMETUPLE_ENTRY
/** \brief Entry point of function StbM_CalculateNewTimeTuple */
#define DBG_STBM_CALCULATENEWTIMETUPLE_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_STBM_CALCULATENEWTIMETUPLE_EXIT
/** \brief Exit point of function StbM_CalculateNewTimeTuple */
#define DBG_STBM_CALCULATENEWTIMETUPLE_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_STBM_SUBTRACTVIRTUALLOCALTIMES_ENTRY
/** \brief Entry point of function StbM_SubtractVirtualLocalTimes */
#define DBG_STBM_SUBTRACTVIRTUALLOCALTIMES_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_SUBTRACTVIRTUALLOCALTIMES_EXIT
/** \brief Exit point of function StbM_SubtractVirtualLocalTimes */
#define DBG_STBM_SUBTRACTVIRTUALLOCALTIMES_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_SUBTRACTVIRTUALLOCALTIMESTOTIMESTAMP_ENTRY
/** \brief Entry point of function StbM_SubtractVirtualLocalTimesToTimeStamp */
#define DBG_STBM_SUBTRACTVIRTUALLOCALTIMESTOTIMESTAMP_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_SUBTRACTVIRTUALLOCALTIMESTOTIMESTAMP_EXIT
/** \brief Exit point of function StbM_SubtractVirtualLocalTimesToTimeStamp */
#define DBG_STBM_SUBTRACTVIRTUALLOCALTIMESTOTIMESTAMP_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_SUMVIRTUALLOCALTIMES_ENTRY
/** \brief Entry point of function StbM_SumVirtualLocalTimes */
#define DBG_STBM_SUMVIRTUALLOCALTIMES_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_SUMVIRTUALLOCALTIMES_EXIT
/** \brief Exit point of function StbM_SumVirtualLocalTimes */
#define DBG_STBM_SUMVIRTUALLOCALTIMES_EXIT(a,b,c,d)
#endif

#ifndef DBG_STBM_SYNCTRIGGEREDCUSTOMER_ENTRY
/** \brief Entry point of function StbM_SyncTriggeredCustomer */
#define DBG_STBM_SYNCTRIGGEREDCUSTOMER_ENTRY(a,b)
#endif

#ifndef DBG_STBM_SYNCTRIGGEREDCUSTOMER_EXIT
/** \brief Exit point of function StbM_SyncTriggeredCustomer */
#define DBG_STBM_SYNCTRIGGEREDCUSTOMER_EXIT(a,b)
#endif

#ifndef DBG_STBM_TIMELEAPDETECTION_ENTRY
/** \brief Entry point of function StbM_TimeLeapDetection */
#define DBG_STBM_TIMELEAPDETECTION_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_TIMELEAPDETECTION_EXIT
/** \brief Exit point of function StbM_TimeLeapDetection */
#define DBG_STBM_TIMELEAPDETECTION_EXIT(a,b,c)
#endif



#ifndef DBG_STBM_STORENEWTIMEVALUESFORSLAVERATECORRECTION_ENTRY
/** \brief Entry point of function StbM_StoreNewTimeValuesForSlaveRateCorrection */
#define DBG_STBM_STORENEWTIMEVALUESFORSLAVERATECORRECTION_ENTRY(a,b,c,d)
#endif

#ifndef DBG_STBM_STORENEWTIMEVALUESFORSLAVERATECORRECTION_EXIT
/** \brief Exit point of function StbM_StoreNewTimeValuesForSlaveRateCorrection */
#define DBG_STBM_STORENEWTIMEVALUESFORSLAVERATECORRECTION_EXIT(a,b,c,d)
#endif


#ifndef DBG_STBM_HANDLEOFFSETCORRECTIONTIMERECORDINGTIMELEAP_ENTRY
/** \brief Entry point of function StbM_HandleTimeLeap */
#define DBG_STBM_HANDLEOFFSETCORRECTIONTIMERECORDINGTIMELEAP_ENTRY(a,b,c,d,e,f,g)
#endif

#ifndef DBG_STBM_HANDLEOFFSETCORRECTIONTIMERECORDINGTIMELEAP_EXIT
/** \brief Exit point of function StbM_HandleTimeLeap */
#define DBG_STBM_HANDLEOFFSETCORRECTIONTIMERECORDINGTIMELEAP_EXIT(a,b,c,d,e,f,g)
#endif


#ifndef DBG_STBM_HANDLETIMELEAP_ENTRY
/** \brief Entry point of function StbM_HandleTimeLeap */
#define DBG_STBM_HANDLETIMELEAP_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_HANDLETIMELEAP_EXIT
/** \brief Exit point of function StbM_HandleTimeLeap */
#define DBG_STBM_HANDLETIMELEAP_EXIT(a,b,c)
#endif


#ifndef DBG_STBM_HANDLEOFFSETCORRECTION_ENTRY
/** \brief Entry point of function StbM_HandleOffsetCorrection */
#define DBG_STBM_HANDLEOFFSETCORRECTION_ENTRY(a,b,c,d)
#endif

#ifndef DBG_STBM_HANDLEOFFSETCORRECTION_EXIT
/** \brief Exit point of function StbM_HandleOffsetCorrection */
#define DBG_STBM_HANDLEOFFSETCORRECTION_EXIT(a,b,c,d)
#endif


#ifndef DBG_STBM_HANDLETIMERECORDING_ENTRY
/** \brief Entry point of function StbM_HandleTimeRecording */
#define DBG_STBM_HANDLETIMERECORDING_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_STBM_HANDLETIMERECORDING_EXIT
/** \brief Exit point of function StbM_HandleTimeRecording */
#define DBG_STBM_HANDLETIMERECORDING_EXIT(a,b,c,d,e,f)
#endif


#ifndef DBG_STBM_STORETIMETUPLE_ENTRY
/** \brief Entry point of function StbM_StoreTimeTuple */
#define DBG_STBM_STORETIMETUPLE_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_STORETIMETUPLE_EXIT
/** \brief Exit point of function StbM_StoreTimeTuple */
#define DBG_STBM_STORETIMETUPLE_EXIT(a,b,c)
#endif



#ifndef DBG_STBM_TIMESTAMPISGREATEREQUAL_ENTRY
/** \brief Entry point of function StbM_TimeStampIsGreaterEqual */
#define DBG_STBM_TIMESTAMPISGREATEREQUAL_ENTRY(a,b)
#endif

#ifndef DBG_STBM_TIMESTAMPISGREATEREQUAL_EXIT
/** \brief Exit point of function StbM_TimeStampIsGreaterEqual */
#define DBG_STBM_TIMESTAMPISGREATEREQUAL_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_TRIGGERTIMETRANSMISSION_ENTRY
/** \brief Entry point of function StbM_TriggerTimeTransmission */
#define DBG_STBM_TRIGGERTIMETRANSMISSION_ENTRY(a)
#endif

#ifndef DBG_STBM_TRIGGERTIMETRANSMISSION_EXIT
/** \brief Entry point of function StbM_TriggerTimeTransmission */
#define DBG_STBM_TRIGGERTIMETRANSMISSION_EXIT(a,b)
#endif

#ifndef DBG_STBM_GETTIMEBASEUPDATECOUNTER_ENTRY
/** \brief Entry point of function StbM_GetTimeBaseUpdateCounter */
#define DBG_STBM_GETTIMEBASEUPDATECOUNTER_ENTRY(a)
#endif

#ifndef DBG_STBM_GETTIMEBASEUPDATECOUNTER_EXIT
/** \brief Entry point of function StbM_GetTimeBaseUpdateCounter */
#define DBG_STBM_GETTIMEBASEUPDATECOUNTER_EXIT(a,b)
#endif

#ifndef DBG_STBM_GETTIMEBASESTATUS_ENTRY
/** \brief Entry point of function StbM_GetTimeBaseStatus */
#define DBG_STBM_GETTIMEBASESTATUS_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_GETTIMEBASESTATUS_EXIT
/** \brief Entry point of function StbM_GetTimeBaseStatus */
#define DBG_STBM_GETTIMEBASESTATUS_EXIT(a,b,c,d)
#endif

#ifndef DBG_STBM_ETHTSYNGETCURRENTTIME_ENTRY
/** \brief Entry point of function StbM_EthTSynGetCurrentTime */
#define DBG_STBM_ETHTSYNGETCURRENTTIME_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_ETHTSYNGETCURRENTTIME_EXIT
/** \brief Entry point of function StbM_EthTSynGetCurrentTime */
#define DBG_STBM_ETHTSYNGETCURRENTTIME_EXIT(a,b,c,d)
#endif

#ifndef DBG_STBM_ETHTSYNSETGLOBALTIME_ENTRY
/** \brief Entry point of function StbM_EthTSynSetGlobalTime */
#define DBG_STBM_ETHTSYNSETGLOBALTIME_ENTRY(a,b)
#endif

#ifndef DBG_STBM_ETHTSYNSETGLOBALTIME_EXIT
/** \brief Entry point of function StbM_EthTSynSetGlobalTime */
#define DBG_STBM_ETHTSYNSETGLOBALTIME_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_GETCURRENTVIRTUALLOCALTIME_BASEDON_OSCOUNTER_ENTRY
/** \brief Entry point of function StbM_GetCurrentVirtualLocalTime_BasedOn_OsCounter */
#define DBG_STBM_GETCURRENTVIRTUALLOCALTIME_BASEDON_OSCOUNTER_ENTRY(a,b)
#endif

#ifndef DBG_STBM_GETCURRENTVIRTUALLOCALTIME_BASEDON_OSCOUNTER_EXIT
/** \brief Entry point of function StbM_GetCurrentVirtualLocalTime_BasedOn_OsCounter */
#define DBG_STBM_GETCURRENTVIRTUALLOCALTIME_BASEDON_OSCOUNTER_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_GETCURRENTVIRTUALLOCALTIME_USING_OSGETTIMESTAMP_ENTRY
/** \brief Entry point of function StbM_GetCurrentVirtualLocalTime_Using_OSGetTimeStamp */
#define DBG_STBM_GETCURRENTVIRTUALLOCALTIME_USING_OSGETTIMESTAMP_ENTRY(a,b)
#endif

#ifndef DBG_STBM_GETCURRENTVIRTUALLOCALTIME_USING_OSGETTIMESTAMP_EXIT
/** \brief Entry point of function StbM_GetCurrentVirtualLocalTime_Using_OSGetTimeStamp */
#define DBG_STBM_GETCURRENTVIRTUALLOCALTIME_USING_OSGETTIMESTAMP_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_GETCURRENTVIRTUALLOCALTIMEETHTSYN_ENTRY
/** \brief Entry point of function StbM_GetCurrentVirtualLocalTimeEthTSyn */
#define DBG_STBM_GETCURRENTVIRTUALLOCALTIMEETHTSYN_ENTRY(a,b)
#endif

#ifndef DBG_STBM_GETCURRENTVIRTUALLOCALTIMEETHTSYN_EXIT
/** \brief Entry point of function StbM_GetCurrentVirtualLocalTimeEthTSyn */
#define DBG_STBM_GETCURRENTVIRTUALLOCALTIMEETHTSYN_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_GETCURRENTVIRTUALLOCALTIMEGPT_ENTRY
/** \brief Entry point of function StbM_GetCurrentVirtualLocalTimeGpt */
#define DBG_STBM_GETCURRENTVIRTUALLOCALTIMEGPT_ENTRY(a,b)
#endif

#ifndef DBG_STBM_GETCURRENTVIRTUALLOCALTIMEGPT_EXIT
/** \brief Entry point of function StbM_GetCurrentVirtualLocalTimeGpt */
#define DBG_STBM_GETCURRENTVIRTUALLOCALTIMEGPT_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_CHECKINTERVALTIMEING_ENTRY
/** \brief Entry point of function StbM_CheckIntervalTiming */
#define DBG_STBM_CHECKINTERVALTIMEING_ENTRY(a,b)
#endif

#ifndef DBG_STBM_CHECKINTERVALTIMEING_EXIT
/** \brief Entry point of function StbM_CheckIntervalTiming */
#define DBG_STBM_CHECKINTERVALTIMEING_EXIT(a,b)
#endif

#ifndef DBG_STBM_SETRATECORRECTION_ENTRY
/** \brief Entry point of function StbM_SetRateCorrection */
#define DBG_STBM_SETRATECORRECTION_ENTRY(a,b)
#endif

#ifndef DBG_STBM_SETRATECORRECTION_EXIT
/** \brief Entry point of function StbM_SetRateCorrection */
#define DBG_STBM_SETRATECORRECTION_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_GETRATEDEVIATION_ENTRY
/** \brief Entry point of function StbM_GetRateDeviation */
#define DBG_STBM_GETRATEDEVIATION_ENTRY(a,b)
#endif

#ifndef DBG_STBM_GETRATEDEVIATION_EXIT
/** \brief Entry point of function StbM_GetRateDeviation */
#define DBG_STBM_GETRATEDEVIATION_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_TIMESTAMP_TO_Q_ENTRY
/** \brief Entry point of function StbM_timestamp_to_q */
#define DBG_STBM_TIMESTAMP_TO_Q_ENTRY(a)
#endif

#ifndef DBG_STBM_TIMESTAMP_TO_Q_EXIT
/** \brief Entry point of function StbM_timestamp_to_q */
#define DBG_STBM_TIMESTAMP_TO_Q_EXIT(a)
#endif

#ifndef DBG_STBM_Q_TO_TIMESTAMP_ENTRY
/** \brief Entry point of function StbM_Q_To_Timestamp */
#define DBG_STBM_Q_TO_TIMESTAMP_ENTRY(a,b)
#endif

#ifndef DBG_STBM_Q_TO_TIMESTAMP_EXIT
/** \brief Entry point of function StbM_Q_To_Timestamp */
#define DBG_STBM_Q_TO_TIMESTAMP_EXIT(a,b)
#endif

#ifndef DBG_STBM_DIV_Q_ENTRY
/** \brief Entry point of function StbM_div_q */
#define DBG_STBM_DIV_Q_ENTRY(a,b)
#endif

#ifndef DBG_STBM_DIV_Q_EXIT
/** \brief Entry point of function StbM_div_q */
#define DBG_STBM_DIV_Q_EXIT(a,b)
#endif

#ifndef DBG_STBM_MUL_Q_ENTRY
/** \brief Entry point of function StbM_mul_q */
#define DBG_STBM_MUL_Q_ENTRY(a,b)
#endif

#ifndef DBG_STBM_MUL_Q_EXIT
/** \brief Entry point of function StbM_mul_q */
#define DBG_STBM_MUL_Q_EXIT(a,b)
#endif

#ifndef DBG_STBM_UDIV64_32_ENTRY
/** \brief Entry point of function StbM_udiv64_32 */
#define DBG_STBM_UDIV64_32_ENTRY(a,b)
#endif

#ifndef DBG_STBM_UDIV64_32_EXIT
/** \brief Entry point of function StbM_udiv64_32 */
#define DBG_STBM_UDIV64_32_EXIT(a,b)
#endif

#ifndef DBG_STBM_UMUL32_64_ENTRY
/** \brief Entry point of function StbM_umul32_64 */
#define DBG_STBM_UMUL32_64_ENTRY(a,b)
#endif

#ifndef DBG_STBM_UMUL32_64_EXIT
/** \brief Entry point of function StbM_umul32_64 */
#define DBG_STBM_UMUL32_64_EXIT(a,b)
#endif

#ifndef DBG_STBM_UMUL32_WIDE_ENTRY
/** \brief Entry point of function StbM_umul32_wide */
#define DBG_STBM_UMUL32_WIDE_ENTRY(a,b,c,d)
#endif

#ifndef DBG_STBM_UMUL32_WIDE_EXIT
/** \brief Entry point of function StbM_umul32_wide */
#define DBG_STBM_UMUL32_WIDE_EXIT(a,b,c,d)
#endif

#ifndef DBG_STBM_CALCULATELOCALTIMEWITHRATE_ENTRY
/** \brief Entry point of function StbM_CalculateLocalTimeWithRate */
#define DBG_STBM_CALCULATELOCALTIMEWITHRATE_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_STBM_CALCULATELOCALTIMEWITHRATE_EXIT
/** \brief Entry point of function StbM_CalculateLocalTimeWithRate */
#define DBG_STBM_CALCULATELOCALTIMEWITHRATE_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_STBM_STATUSEVENTDETECTION_ENTRY
/** \brief Entry point of function StbM_StatusEventDetection */
#define DBG_STBM_STATUSEVENTDETECTION_ENTRY(a,b)
#endif

#ifndef DBG_STBM_STATUSEVENTDETECTION_EXIT
/** \brief Exit point of function StbM_StatusEventDetection */
#define DBG_STBM_STATUSEVENTDETECTION_EXIT(a,b)
#endif

#ifndef DBG_STBM_TIMEALREADYUPDATED_ENTRY
/** \brief Entry point of function StbM_TimeAlreadyUpdated */
#define DBG_STBM_TIMEALREADYUPDATED_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_TIMEALREADYUPDATED_EXIT
/** \brief Exit point of function StbM_TimeAlreadyUpdated */
#define DBG_STBM_TIMEALREADYUPDATED_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_PERFORMOFFSETCORRECTION_ENTRY
/** \brief Entry point of function StbM_PerformOffsetCorrection */
#define DBG_STBM_PERFORMOFFSETCORRECTION_ENTRY(a,b,c,d)
#endif

#ifndef DBG_STBM_PERFORMOFFSETCORRECTION_EXIT
/** \brief Exit point of function StbM_PerformOffsetCorrection */
#define DBG_STBM_PERFORMOFFSETCORRECTION_EXIT(a,b,c,d)
#endif

#ifndef DBG_STBM_PERFORMSLAVERATECORRECTION_ENTRY
/** \brief Entry point of function StbM_PerformSlaveRateCorrection */
#define DBG_STBM_PERFORMSLAVERATECORRECTION_ENTRY(a,b)
#endif

#ifndef DBG_STBM_PERFORMSLAVERATECORRECTION_EXIT
/** \brief Exit point of function StbM_PerformSlaveRateCorrection */
#define DBG_STBM_PERFORMSLAVERATECORRECTION_EXIT(a,b)
#endif

#ifndef DBG_STBM_CALCULATESLAVERATECORRECTION_ENTRY
/** \brief Entry point of function StbM_CalculateSlaveRateCorrection */
#define DBG_STBM_CALCULATESLAVERATECORRECTION_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_CALCULATESLAVERATECORRECTION_EXIT
/** \brief Exit point of function StbM_CalculateSlaveRateCorrection */
#define DBG_STBM_CALCULATESLAVERATECORRECTION_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_CHECKFLAGCHANGES_ENTRY
/** \brief Entry point of function StbM_CheckFlagChanges */
#define DBG_STBM_CHECKFLAGCHANGES_ENTRY(a)
#endif

#ifndef DBG_STBM_CHECKFLAGCHANGES_EXIT
/** \brief Exit point of function StbM_CheckFlagChanges */
#define DBG_STBM_CHECKFLAGCHANGES_EXIT(a)
#endif

#ifndef DBG_STBM_VALIDATERATECORRECTIONINTERVAL_ENTRY
/** \brief Entry point of function StbM_ValidateRateCorrectionInterval */
#define DBG_STBM_VALIDATERATECORRECTIONINTERVAL_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_VALIDATERATECORRECTIONINTERVAL_EXIT
/** \brief Exit point of function StbM_ValidateRateCorrectionInterval */
#define DBG_STBM_VALIDATERATECORRECTIONINTERVAL_EXIT(a,b,c,d)
#endif

#ifndef DBG_STBM_CHECKIFMEASUREMENTRESTARTISNEEDED_ENTRY
/** \brief Entry point of function StbM_CheckIfMeasurementRestartIsNeeded */
#define DBG_STBM_CHECKIFMEASUREMENTRESTARTISNEEDED_ENTRY(a,b,c,d,e)
#endif

#ifndef DBG_STBM_CHECKIFMEASUREMENTRESTARTISNEEDED_EXIT
/** \brief Exit point of function StbM_CheckIfMeasurementRestartIsNeeded */
#define DBG_STBM_CHECKIFMEASUREMENTRESTARTISNEEDED_EXIT(a,b,c,d,e)
#endif

#ifndef DBG_STBM_PERFORMTIMERECORDING_ENTRY
/** \brief Entry point of function StbM_PerformTimeRecording */
#define DBG_STBM_PERFORMTIMERECORDING_ENTRY(a,b,c,d,e,f)
#endif

#ifndef DBG_STBM_PERFORMTIMERECORDING_EXIT
/** \brief Exit point of function StbM_PerformTimeRecording */
#define DBG_STBM_PERFORMTIMERECORDING_EXIT(a,b,c,d,e,f)
#endif

#ifndef DBG_STBM_GETSYNCTIMERECORDHEAD_ENTRY
/** \brief Entry point of function StbM_GetSyncTimeRecordHead */
#define DBG_STBM_GETSYNCTIMERECORDHEAD_ENTRY(a,b)
#endif

#ifndef DBG_STBM_GETSYNCTIMERECORDHEAD_EXIT
/** \brief Exit point of function StbM_GetSyncTimeRecordHead */
#define DBG_STBM_GETSYNCTIMERECORDHEAD_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_GETOFFSETTIMERECORDHEAD_ENTRY
/** \brief Entry point of function StbM_GetOffsetTimeRecordHead */
#define DBG_STBM_GETOFFSETTIMERECORDHEAD_ENTRY(a,b)
#endif

#ifndef DBG_STBM_GETOFFSETTIMERECORDHEAD_EXIT
/** \brief Exit point of function StbM_GetOffsetTimeRecordHead */
#define DBG_STBM_GETOFFSETTIMERECORDHEAD_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_HANDLETIMELEAPCOUNTER_ENTRY
/** \brief Entry point of function StbM_HandleTimeLeapCounter */
#define DBG_STBM_HANDLETIMELEAPCOUNTER_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_HANDLETIMELEAPCOUNTER_EXIT
/** \brief Exit point of function StbM_HandleTimeLeapCounter */
#define DBG_STBM_HANDLETIMELEAPCOUNTER_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_UPDATESHAREDDATAINMEMORY_ENTRY
/** \brief Entry point of function StbM_UpdateSharedDataInMemory */
#define DBG_STBM_UPDATESHAREDDATAINMEMORY_ENTRY(a,b,c,d)
#endif

#ifndef DBG_STBM_UPDATESHAREDDATAINMEMORY_EXIT
/** \brief Exit point of function StbM_UpdateSharedDataInMemory */
#define DBG_STBM_UPDATESHAREDDATAINMEMORY_EXIT(a,b,c,d)
#endif

#ifndef DBG_STBM_UPDATESHAREDSTATUSINMEMORY_ENTRY
/** \brief Entry point of function StbM_UpdateSharedStatusInMemory */
#define DBG_STBM_UPDATESHAREDSTATUSINMEMORY_ENTRY(a)
#endif

#ifndef DBG_STBM_UPDATESHAREDSTATUSINMEMORY_EXIT
/** \brief Exit point of function StbM_UpdateSharedStatusInMemory */
#define DBG_STBM_UPDATESHAREDSTATUSINMEMORY_EXIT(a)
#endif

#ifndef DBG_STBM_UPDATESHAREDRATEDEVIATIONINMEMORY_ENTRY
/** \brief Entry point of function StbM_UpdateSharedRateDeviationInMemory */
#define DBG_STBM_UPDATESHAREDRATEDEVIATIONINMEMORY_ENTRY(a,b)
#endif

#ifndef DBG_STBM_UPDATESHAREDRATEDEVIATIONINMEMORY_EXIT
/** \brief Exit point of function StbM_UpdateSharedRateDeviationInMemory */
#define DBG_STBM_UPDATESHAREDRATEDEVIATIONINMEMORY_EXIT(a,b)
#endif


#ifndef DBG_STBM_FINDCURRENTBLOCKINDEX_ENTRY
/** \brief Entry point of function StbM_FindCurrentBlockIndex */
#define DBG_STBM_FINDCURRENTBLOCKINDEX_ENTRY(a)
#endif

#ifndef DBG_STBM_FINDCURRENTBLOCKINDEX_EXIT
/** \brief Entry point of function StbM_FindCurrentBlockIndex */
#define DBG_STBM_FINDCURRENTBLOCKINDEX_EXIT(a)
#endif


#ifndef DBG_STBM_CALCULATEANDRECORDRATECORRECTION_ENTRY
/** \brief Entry point of function StbM_CalculateAndRecordRateCorrection */
#define DBG_STBM_CALCULATEANDRECORDRATECORRECTION_ENTRY(a,b)
#endif

#ifndef DBG_STBM_CALCULATEANDRECORDRATECORRECTION_EXIT
/** \brief Entry point of function StbM_CalculateAndRecordRateCorrection */
#define DBG_STBM_CALCULATEANDRECORDRATECORRECTION_EXIT(a,b)
#endif

#ifndef DBG_STBM_HANDLESLAVERATECORRECTION_ENTRY
/** \brief Entry point of function StbM_HandleSlaveRateCorrection */
#define DBG_STBM_HANDLESLAVERATECORRECTION_ENTRY(a,b)
#endif

#ifndef DBG_STBM_HANDLESLAVERATECORRECTION_EXIT
/** \brief Entry point of function StbM_HandleSlaveRateCorrection */
#define DBG_STBM_HANDLESLAVERATECORRECTION_EXIT(a,b)
#endif



#ifndef DBG_STBM_GETINSERTIONINDEX_ENTRY
/** \brief Entry point of function StbM_GetInsertionIndex */
#define DBG_STBM_GETINSERTIONINDEX_ENTRY(a)
#endif

#ifndef DBG_STBM_GETINSERTIONINDEX_EXIT
/** \brief Entry point of function StbM_GetInsertionIndex */
#define DBG_STBM_GETINSERTIONINDEX_EXIT(a)
#endif


#ifndef DBG_STBM_GETLISTLENGTH_ENTRY
/** \brief Entry point of function StbM_GetListLength */
#define DBG_STBM_GETLISTLENGTH_ENTRY()
#endif

#ifndef DBG_STBM_GETLISTLENGTH_EXIT
/** \brief Entry point of function StbM_GetListLength */
#define DBG_STBM_GETLISTLENGTH_EXIT()
#endif



#ifndef DBG_STBM_INSERTINLIST_ENTRY
/** \brief Entry point of function StbM_InsertInList */
#define DBG_STBM_INSERTINLIST_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_INSERTINLIST_EXIT
/** \brief Exit point of function StbM_InsertInList */
#define DBG_STBM_INSERTINLIST_EXIT(a,b,c)
#endif


#ifndef DBG_STBM_INSERTATTHEBEGINNING_ENTRY
/** \brief Entry point of function StbM_InsertAtTheBeginning */
#define DBG_STBM_INSERTATTHEBEGINNING_ENTRY(a,b)
#endif

#ifndef DBG_STBM_INSERTATTHEBEGINNING_EXIT
/** \brief Entry point of function StbM_InsertAtTheBeginning */
#define DBG_STBM_INSERTATTHEBEGINNING_EXIT(a,b)
#endif


#ifndef DBG_STBM_INSERTATTHERIGHTPOSITION_ENTRY
/** \brief Entry point of function StbM_InsertAtTheRightPosition */
#define DBG_STBM_INSERTATTHERIGHTPOSITION_ENTRY(a,b)
#endif

#ifndef DBG_STBM_INSERTATTHERIGHTPOSITION_EXIT
/** \brief Entry point of function StbM_InsertAtTheRightPosition */
#define DBG_STBM_INSERTATTHERIGHTPOSITION_EXIT(a,b)
#endif


#ifndef DBG_STBM_INSERTAFTER_ENTRY
/** \brief Entry point of function StbM_InsertAfter */
#define DBG_STBM_INSERTAFTER_ENTRY(a,b,c)
#endif

#ifndef DBG_STBM_INSERTAFTER_EXIT
/** \brief Entry point of function StbM_InsertAfter */
#define DBG_STBM_INSERTAFTER_EXIT(a,b,c)
#endif

#ifndef DBG_STBM_ISDATACONTAINEDINLIST_ENTRY
/** \brief Entry point of function StbM_IsDataContainedInList */
#define DBG_STBM_ISDATACONTAINEDINLIST_ENTRY(a,b)
#endif

#ifndef DBG_STBM_ISDATACONTAINEDINLIST_EXIT
/** \brief Entry point of function StbM_IsDataContainedInList */
#define DBG_STBM_ISDATACONTAINEDINLIST_EXIT(a,b)
#endif



#ifndef DBG_STBM_DELETEFROMPOSITION_ENTRY
/** \brief Entry point of function StbM_DeleteFromPosition */
#define DBG_STBM_DELETEFROMPOSITION_ENTRY(a)
#endif

#ifndef DBG_STBM_DELETEFROMPOSITION_EXIT
/** \brief Entry point of function StbM_DeleteFromPosition */
#define DBG_STBM_DELETEFROMPOSITION_EXIT(a)
#endif

#ifndef DBG_STBM_GETTIMELEAP_ENTRY
/** \brief Entry point of function StbM_GetTimeLeap */
#define DBG_STBM_GETTIMELEAP_ENTRY(a,b)
#endif

#ifndef DBG_STBM_GETTIMELEAP_EXIT
/** \brief Exit point of function StbM_GetTimeLeap */
#define DBG_STBM_GETTIMELEAP_EXIT(a,b,c)
#endif
/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef STBM_TRACE_H */
/*==================[end of file]===========================================*/
