[!AUTOSPACING!]
#if (!defined STBM_PARTITIONS_H)
#define STBM_PARTITIONS_H

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
/** \addtogroup Synchronized Time Base Manager
 ** @{ */

/*==[Includes]================================================================*/

[!INCLUDE "StbM_Vars.m"!]
#include <StbM_Gen_Common.h>

/* include Rte_StbM_Type.h file before the StbM_Types.h file inclusion,
   in order to assure that re-definitions of StbM types in case Non-EB Rte is used, will not happen */
/* !LINKSTO StbM.dsn.MulticoreDistribution,1 */
[!LOOP "text:split($ListOfEcucPartitionNames, ';')"!]

[!VAR "EcuCPartitionName" = "."!]
[!IF "$EcuCPartitionName != '_NoPartition'"!]
#include <Rte_StbM[!"$EcuCPartitionName"!]_Type.h>
[!ENDIF!]

[!ENDLOOP!]

/*==[Macros]==================================================================*/

/*==[Types]===================================================================*/

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
extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetMasterConfig[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_MasterConfigType, AUTOMATIC, STBM_APPL_DATA) masterConfig
);
#endif /* (STBM_GET_MASTERCONFIG_SUPPORTED == STD_ON) */

#if (STBM_MASTER_OR_GATEWAY_TIME_BASE_CONFIGURED == STD_ON)
extern FUNC(Std_ReturnType, STBM_CODE) StbM_SetGlobalTime[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStamp,
  P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData
);
#endif /* (STBM_MASTER_OR_GATEWAY_TIME_BASE_CONFIGURED == STD_ON) */

#if (STBM_MASTER_OR_GATEWAY_TIME_BASE_CONFIGURED == STD_ON)
extern FUNC(Std_ReturnType, STBM_CODE) StbM_SetUserData[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData
);
#endif /* (STBM_MASTER_OR_GATEWAY_TIME_BASE_CONFIGURED == STD_ON) */

#if (STBM_TRIGGERTIMETRANSMISSION_SUPPORTED == STD_ON)
extern FUNC(Std_ReturnType, STBM_CODE) StbM_TriggerTimeTransmission[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId
);
#endif /* (STBM_TRIGGERTIMETRANSMISSION_SUPPORTED == STD_ON) */

#if (STBM_MASTER_OR_GATEWAY_TIME_BASE_CONFIGURED == STD_ON)
extern FUNC(Std_ReturnType, STBM_CODE) StbM_UpdateGlobalTime[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStamp,
  P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData
);
#endif /* (STBM_MASTER_OR_GATEWAY_TIME_BASE_CONFIGURED == STD_ON) */

#if (STBM_SETOFFSET_SUPPORTED == STD_ON)
extern FUNC(Std_ReturnType, STBM_CODE) StbM_SetOffset[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStamp,
  P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData
);
#endif /* (STBM_SETOFFSET_SUPPORTED == STD_ON) */

#if(STBM_MASTER_RATE_CORRECTION_USED == STD_ON)
extern FUNC(Std_ReturnType, STBM_CODE) StbM_SetRateCorrection[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  StbM_RateDeviationType rateDeviation
);
#endif /* (STBM_MASTER_RATE_CORRECTION_USED == STD_ON) */


/** \brief StbM_GetCurrentTime_EcucPartition_0 API. */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentTime[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStamp,
  P2VAR(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData
);

#if (STBM_GETCURRENTTIMEEXTENDED_USED == STD_ON)
extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentTimeExtended[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_TimeStampExtendedType, AUTOMATIC, STBM_APPL_DATA) timeStamp,
  P2VAR(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData
);
#endif /* (STBM_GETCURRENTTIMEEXTENDED_USED == STD_ON) */

extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetRateDeviation[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_RateDeviationType, AUTOMATIC, STBM_APPL_DATA) rateDeviation
);

#if ((STBM_SYNC_TIME_RECORDING_USED == STD_ON) || (STBM_OFFSET_TIME_RECORDING_USED == STD_ON))
#if (STBM_SYNC_TIME_RECORDING_USED == STD_ON)
extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetSyncTimeRecordHead[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_SyncRecordTableHeadType, AUTOMATIC, STBM_APPL_DATA) syncRecordTableHead
);
#endif /* (STBM_SYNC_TIME_RECORDING_USED == STD_ON) */

#if (STBM_OFFSET_TIME_RECORDING_USED == STD_ON)
extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetOffsetTimeRecordHead[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_OffsetRecordTableHeadType, AUTOMATIC, STBM_APPL_DATA) offsetRecordTableHead
);
#endif /* (STBM_OFFSET_TIME_RECORDING_USED == STD_ON) */
#endif /* ((STBM_SYNC_TIME_RECORDING_USED == STD_ON) || (STBM_OFFSET_TIME_RECORDING_USED == STD_ON)) */

extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetTimeBaseStatus[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_TimeBaseStatusType, AUTOMATIC, STBM_APPL_DATA) syncTimeBaseStatus,
  P2VAR(StbM_TimeBaseStatusType, AUTOMATIC, STBM_APPL_DATA) offsetTimeBaseStatus
);

#if(STBM_SYNC_SLAVE_TIMEBASES_CONFIGURED == STD_ON)
extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetTimeLeap[!"$EcuCPartitionName"!]
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_TimeDiffType, AUTOMATIC, STBM_APPL_DATA) timeJump
);
#endif /* (STBM_SYNC_SLAVE_TIMEBASES_CONFIGURED == STD_ON) */

[!ENDIF!]

[!ENDLOOP!]



#define STBM_STOP_SEC_CODE
#include <StbM_MemMap.h>

/** @} doxygen end group definition */
/*==================[end of file]===========================================*/


#endif /* if !defined(STBM_PARTITIONS_H) */
