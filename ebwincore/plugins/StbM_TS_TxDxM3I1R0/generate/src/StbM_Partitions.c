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

/*==[Includes]================================================================*/

[!INCLUDE "../include/StbM_Vars.m"!]

#include <TSAutosar.h>
#include <TSAtomic_Assign.h>

#include <StbM_Trace.h>
#include <StbM.h>
#include <SchM_StbM.h>         /* AUTOSAR BSW scheduler */

/*==[Macros]==================================================================*/

/*==[Types]===================================================================*/

/*==[Declaration of functions with internal linkage]==========================*/

/*==[Constants with internal linkage]=========================================*/

/*==[Variables with internal linkage]=========================================*/

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/

#define STBM_START_SEC_CODE
#include <StbM_MemMap.h>


/* !LINKSTO StbM.dsn.MulticoreDistribution,1 */
[!LOOP "text:split($ListOfEcucPartitionNames, ';')"!]
[!VAR "EcuCPartitionName" = "."!]
[!IF "$EcuCPartitionName != '_NoPartition'"!]

#if (STBM_GET_MASTERCONFIG_SUPPORTED == STD_ON)
FUNC(Std_ReturnType, STBM_CODE) StbM_GetMasterConfig[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_MasterConfigType, AUTOMATIC, STBM_APPL_DATA) masterConfig
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  
  DBG_STBM_GETMASTERCONFIG_ENTRY(timeBaseId, masterConfig);

  RetVal = StbM_GetMasterConfig(timeBaseId, masterConfig);

  DBG_STBM_GETMASTERCONFIG_EXIT(RetVal, timeBaseId, masterConfig);

  return RetVal;
}
#endif /* (STBM_GET_MASTERCONFIG_SUPPORTED == STD_ON) */

#if (STBM_MASTER_OR_GATEWAY_TIME_BASE_CONFIGURED == STD_ON)
FUNC(Std_ReturnType, STBM_CODE) StbM_SetGlobalTime[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStamp,
  P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  
  DBG_STBM_SETGLOBALTIME_ENTRY(timeBaseId, timeStamp, userData);

  RetVal = StbM_SetGlobalTime(timeBaseId, timeStamp, userData);

  DBG_STBM_SETGLOBALTIME_EXIT(RetVal, timeBaseId, timeStamp, userData);

  return RetVal;
}
#endif /* (STBM_MASTER_OR_GATEWAY_TIME_BASE_CONFIGURED == STD_ON) */

#if (STBM_MASTER_OR_GATEWAY_TIME_BASE_CONFIGURED == STD_ON)
FUNC(Std_ReturnType, STBM_CODE) StbM_SetUserData[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  
  DBG_STBM_SETUSERDATA_ENTRY(timeBaseId, userData);

  RetVal = StbM_SetUserData(timeBaseId,  userData);

  DBG_STBM_SETUSERDATA_EXIT(RetVal, timeBaseId, userData);

  return RetVal;
}
#endif /* (STBM_MASTER_OR_GATEWAY_TIME_BASE_CONFIGURED == STD_ON) */

#if (STBM_TRIGGERTIMETRANSMISSION_SUPPORTED == STD_ON)
FUNC(Std_ReturnType, STBM_CODE) StbM_TriggerTimeTransmission[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  
  DBG_STBM_TRIGGERTIMETRANSMISSION_ENTRY(timeBaseId);

  RetVal = StbM_TriggerTimeTransmission(timeBaseId);

  DBG_STBM_TRIGGERTIMETRANSMISSION_EXIT(RetVal, timeBaseId);

  return RetVal;
}
#endif /* (STBM_TRIGGERTIMETRANSMISSION_SUPPORTED == STD_ON) */

#if (STBM_MASTER_OR_GATEWAY_TIME_BASE_CONFIGURED == STD_ON)
FUNC(Std_ReturnType, STBM_CODE) StbM_UpdateGlobalTime[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStamp,
  P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  
  DBG_STBM_UPDATEGLOBALTIME_ENTRY(timeBaseId, timeStamp, userData);

  RetVal = StbM_UpdateGlobalTime(timeBaseId, timeStamp, userData);

  DBG_STBM_UPDATEGLOBALTIME_EXIT(RetVal, timeBaseId, timeStamp, userData);

  return RetVal;
}
#endif /* (STBM_MASTER_OR_GATEWAY_TIME_BASE_CONFIGURED == STD_ON) */

#if (STBM_SETOFFSET_SUPPORTED == STD_ON)
FUNC(Std_ReturnType, STBM_CODE) StbM_SetOffset[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStamp,
  P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  
  DBG_STBM_SETOFFSET_ENTRY(timeBaseId, timeStamp, userData);

  RetVal = StbM_SetOffset(timeBaseId, timeStamp, userData);

  DBG_STBM_SETOFFSET_EXIT(RetVal, timeBaseId, timeStamp, userData);

  return RetVal;
}
#endif /* (STBM_SETOFFSET_SUPPORTED == STD_ON) */

#if(STBM_MASTER_RATE_CORRECTION_USED == STD_ON)
FUNC(Std_ReturnType, STBM_CODE) StbM_SetRateCorrection[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  StbM_RateDeviationType rateDeviation
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  
  DBG_STBM_SETRATECORRECTION_ENTRY(timeBaseId, rateDeviation);

  RetVal = StbM_SetRateCorrection(timeBaseId, rateDeviation);

  DBG_STBM_SETRATECORRECTION_EXIT(RetVal, timeBaseId, rateDeviation);

  return RetVal;
}
#endif /* (STBM_MASTER_RATE_CORRECTION_USED == STD_ON) */




FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentTime[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStamp,
  P2VAR(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  
  DBG_STBM_GETCURRENTTIME_ENTRY(timeBaseId, timeStamp, userData);

  RetVal = StbM_GetCurrentTime(timeBaseId, timeStamp, userData);

  DBG_STBM_GETCURRENTTIME_EXIT(RetVal, timeBaseId, timeStamp, userData);

  return RetVal;
}


#if (STBM_GETCURRENTTIMEEXTENDED_USED == STD_ON)
FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentTimeExtended[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_TimeStampExtendedType, AUTOMATIC, STBM_APPL_DATA) timeStamp,
  P2VAR(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  
  DBG_STBM_GETCURRENTTIMEEXTENDED_ENTRY(timeBaseId, timeStamp, userData);

  RetVal = StbM_GetCurrentTimeExtended(timeBaseId, timeStamp, userData);

  DBG_STBM_GETCURRENTTIMEEXTENDED_EXIT(RetVal, timeBaseId, timeStamp, userData);

  return RetVal;
}
#endif /* (STBM_GETCURRENTTIMEEXTENDED_USED == STD_ON) */


FUNC(Std_ReturnType, STBM_CODE) StbM_GetRateDeviation[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_RateDeviationType, AUTOMATIC, STBM_APPL_DATA) rateDeviation
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  
  DBG_STBM_GETRATEDEVIATION_ENTRY(timeBaseId, rateDeviation);

  RetVal = StbM_GetRateDeviation(timeBaseId, rateDeviation);

  DBG_STBM_GETRATEDEVIATION_EXIT(RetVal, timeBaseId, rateDeviation);

  return RetVal;
}


#if ((STBM_SYNC_TIME_RECORDING_USED == STD_ON) || (STBM_OFFSET_TIME_RECORDING_USED == STD_ON))
#if (STBM_SYNC_TIME_RECORDING_USED == STD_ON)
FUNC(Std_ReturnType, STBM_CODE) StbM_GetSyncTimeRecordHead[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_SyncRecordTableHeadType, AUTOMATIC, STBM_APPL_DATA) syncRecordTableHead
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  
  DBG_STBM_GETSYNCTIMERECORDHEAD_ENTRY(timeBaseId, syncRecordTableHead);

  RetVal = StbM_GetSyncTimeRecordHead(timeBaseId, syncRecordTableHead);

  DBG_STBM_GETSYNCTIMERECORDHEAD_EXIT(RetVal, timeBaseId, syncRecordTableHead);

  return RetVal;
}
#endif /* (STBM_SYNC_TIME_RECORDING_USED == STD_ON) */

#if (STBM_OFFSET_TIME_RECORDING_USED == STD_ON)
FUNC(Std_ReturnType, STBM_CODE) StbM_GetOffsetTimeRecordHead[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_OffsetRecordTableHeadType, AUTOMATIC, STBM_APPL_DATA) offsetRecordTableHead
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  
  DBG_STBM_GETOFFSETTIMERECORDHEAD_ENTRY(timeBaseId, offsetRecordTableHead);

  RetVal = StbM_GetOffsetTimeRecordHead(timeBaseId, offsetRecordTableHead);

  DBG_STBM_GETOFFSETTIMERECORDHEAD_EXIT(RetVal, timeBaseId, offsetRecordTableHead);

  return RetVal;
}
#endif /* (STBM_OFFSET_TIME_RECORDING_USED == STD_ON) */
#endif /* ((STBM_SYNC_TIME_RECORDING_USED == STD_ON) || (STBM_OFFSET_TIME_RECORDING_USED == STD_ON)) */


FUNC(Std_ReturnType, STBM_CODE) StbM_GetTimeBaseStatus[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_TimeBaseStatusType, AUTOMATIC, STBM_APPL_DATA) syncTimeBaseStatus,
  P2VAR(StbM_TimeBaseStatusType, AUTOMATIC, STBM_APPL_DATA) offsetTimeBaseStatus
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  
  DBG_STBM_GETTIMEBASESTATUS_ENTRY(timeBaseId, syncTimeBaseStatus, offsetTimeBaseStatus);

  RetVal = StbM_GetTimeBaseStatus(timeBaseId, syncTimeBaseStatus, offsetTimeBaseStatus);

  DBG_STBM_GETTIMEBASESTATUS_EXIT(RetVal, timeBaseId, syncTimeBaseStatus, offsetTimeBaseStatus);

  return RetVal;
}


#if(STBM_SYNC_SLAVE_TIMEBASES_CONFIGURED == STD_ON)
FUNC(Std_ReturnType, STBM_CODE) StbM_GetTimeLeap[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_TimeDiffType, AUTOMATIC, STBM_APPL_DATA) timeJump
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  
  DBG_STBM_GETTIMELEAP_ENTRY(timeBaseId, timeJump);

  RetVal = StbM_GetTimeLeap(timeBaseId, timeJump);

  DBG_STBM_GETTIMELEAP_EXIT(RetVal, timeBaseId, timeJump);

  return RetVal;
}
#endif /* (STBM_SYNC_SLAVE_TIMEBASES_CONFIGURED == STD_ON) */

[!ENDIF!]
[!ENDLOOP!]


#define STBM_STOP_SEC_CODE
#include <StbM_MemMap.h>

/*==================[end of file]===========================================*/

