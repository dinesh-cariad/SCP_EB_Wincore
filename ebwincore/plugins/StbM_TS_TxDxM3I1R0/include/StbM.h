#ifndef STBM_H
#define STBM_H

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

#include <StbM_Version.h>
#include <StbM_Cfg.h>

/* !LINKSTO StbM.dsn.MulticoreDistribution,1 */
#if (STBM_MULTICORE_SUPPORTED == STD_ON)
#include <StbM_Partitions.h>
#else /* (STBM_MULTICORE_SUPPORTED == STD_OFF) */
#if (STBM_RTE_USED == STD_ON)
#include <Rte_StbM_Type.h>
#endif /* (STBM_RTE_USED == STD_ON) */
#endif /* (STBM_MULTICORE_SUPPORTED == STD_ON) */

#include <StbM_Types.h>

/*==[Macros]==================================================================*/

/* API service IDs */

#if (defined STBM_API_INIT)
#error STBM_API_INIT is already defined.
#endif
/** \brief Service ID for StbM_Init() function.*/
/* !LINKSTO StbM.ASR44.SWS_StbM_00052,1 */
#define STBM_API_INIT                           0x00U

#if (defined STBM_API_GETVERSIONINFO)
#error STBM_API_GETVERSIONINFO is already defined.
#endif
/** \brief Service ID for StbM_GetVersionInfo() function.*/
/* !LINKSTO StbM.ASR44.SWS_StbM_00066,1 */
#define STBM_API_GETVERSIONINFO                 0x05U

#if (defined STBM_API_GETCURRENTTIME)
#error STBM_API_GETCURRENTTIME is already defined.
#endif
/** \brief Service ID for StbM_GetCurrentTime() function.*/
/* !LINKSTO StbM.ASR44.SWS_StbM_00195,1 */
#define STBM_API_GETCURRENTTIME                 0x07U

#if (defined STBM_API_GETCURRENTTIMEEXTENDED)
#error STBM_API_GETCURRENTTIMEEXTENDED is already defined.
#endif
/** \brief Service ID for StbM_GetCurrentTimeExtended() function.*/
/* !LINKSTO StbM.ASR44.SWS_StbM_00200,1 */
#define STBM_API_GETCURRENTTIMEEXTENDED                 0x08U

#if (defined STBM_API_GETCURRENTVIRTUALLOCALTIME)
#error STBM_API_GETCURRENTVIRTUALLOCALTIME is already defined.
#endif
/** \brief Service ID for StbM_GetCurrentVirtualLocalTime() function.*/
/* !LINKSTO StbM.ASR44.SWS_StbM_91006,1 */
#define STBM_API_GETCURRENTVIRTUALLOCALTIME     0x1EU

#if (defined STBM_API_BUSGETCURRENTTIME)
#error STBM_API_BUSGETCURRENTTIME is already defined.
#endif
/** \brief Service ID for StbM_BusGetCurrentTime() function.*/
/* !LINKSTO StbM.ASR19-11.SWS_StbM_91005,1 */
#define STBM_API_BUSGETCURRENTTIME              0x1FU


#if (defined STBM_API_GETMASTERCONFIG)
#error STBM_API_GETMASTERCONFIG is already defined.
#endif
/** \brief Service ID for StbM_GetMasterConfig() function.*/
/* !LINKSTO StbM.ASR44.SWS_StbM_91002,1 */
#define STBM_API_GETMASTERCONFIG               0x1DU


#if (defined STBM_API_SETGLOBALTIME)
#error STBM_API_SETGLOBALTIME is already defined.
#endif
/** \brief Service ID for StbM_SetGlobalTime() function.*/
/* !LINKSTO StbM.ASR44.SWS_StbM_00213,1 */
#define STBM_API_SETGLOBALTIME                  0x0bU

#if (defined STBM_API_UPDATEGLOBALTIME)
#error STBM_API_UPDATEGLOBALTIME is already defined.
#endif
/** \brief Service ID for StbM_UpdateGlobalTime() function.*/
/* !LINKSTO StbM.ASR44.SWS_StbM_00385,1 */
#define STBM_API_UPDATEGLOBALTIME               0x10U

#if (defined STBM_API_SETUSERDATA)
#error STBM_API_SETUSERDATA is already defined.
#endif
/** \brief Service ID for StbM_SetUserData() function.*/
/* !LINKSTO StbM.ASR44.SWS_StbM_00218,1 */
#define STBM_API_SETUSERDATA                    0x0cU


#if (defined STBM_API_STARTTIMER)
#error STBM_API_STARTTIMER is already defined.
#endif
/** \brief Service ID for StbM_StartTimer() function.*/
/* !LINKSTO StbM.ASR19-11.SWS_StbM_00272,1 */
#define STBM_API_STARTTIMER                    0x15U


#if (defined STBM_API_SETOFFSET)
#error STBM_API_SETOFFSET is already defined.
#endif
/** \brief Service ID for StbM_SetOffset() function.*/
/* !LINKSTO StbM.ASR44.SWS_StbM_00223,1 */
#define STBM_API_SETOFFSET                      0x0dU

#if (defined STBM_API_GETOFFSET)
#error STBM_API_GETOFFSET is already defined.
#endif
/** \brief Service ID for StbM_GetOffset() function.*/
/* !LINKSTO StbM.ASR44.SWS_StbM_00228,1 */
#define STBM_API_GETOFFSET                      0x0eU

#if (defined STBM_API_BUSSETGLOBALTIME)
#error STBM_API_BUSSETGLOBALTIME is already defined.
#endif
/** \brief Service ID for StbM_BusSetGlobalTime() function.*/
/* !LINKSTO StbM.ASR44.SWS_StbM_00233,1 */
#define STBM_API_BUSSETGLOBALTIME               0x0fU

#if (defined STBM_API_TRIGGERTIMETRANSMISSION)
#error STBM_API_TRIGGERTIMETRANSMISSION is already defined.
#endif
/** \brief Service ID for StbM_TriggerTimeTransmission() function.*/
/* !LINKSTO StbM.ASR44.SWS_StbM_00346,1 */
#define STBM_API_TRIGGERTIMETRANSMISSION        0x1cU

#if (defined STBM_API_GETTIMEBASEUPDATECOUNTER)
#error STBM_API_GETTIMEBASEUPDATECOUNTER is already defined.
#endif
/** \brief Service ID for StbM_GetTimeBaseUpdateCounter() function.*/
/* !LINKSTO StbM.ASR44.SWS_StbM_00347,1 */
#define STBM_API_GETTIMEBASEUPDATECOUNTER       0x1bU

#if (defined STBM_API_GETTIMEBASESTATUS)
#error STBM_API_GETTIMEBASESTATUS is already defined.
#endif
/** \brief Service ID for StbM_GetTimeBaseStatus() function.*/
/* !LINKSTO StbM.ASR44.SWS_StbM_00263,1 */
#define STBM_API_GETTIMEBASESTATUS              0x14U

#if (defined STBM_API_GETRATEDEVIATION)
#error STBM_API_GETRATEDEVIATION is already defined.
#endif
/** \brief Service ID for StbM_GetRateDeviation() function.*/
/* !LINKSTO StbM.ASR44.SWS_StbM_00378,1 */
#define STBM_API_GETRATEDEVIATION              0x11U

#if (defined STBM_API_SETRATECORRECTION)
#error STBM_API_SETRATECORRECTION is already defined.
#endif
/** \brief Service ID for StbM_SetRateCorrection() function.*/
/* !LINKSTO StbM.ASR44.SWS_StbM_00390,1 */
#define STBM_API_SETRATECORRECTION              0x12U

#if (defined STBM_API_GETSYNCTIMERECORDHEAD)
#error STBM_API_GETSYNCTIMERECORDHEAD is already defined.
#endif
/** \brief Service ID for StbM_GetSyncTimeRecordHead() function.*/
/* !LINKSTO StbM.ASR44.SWS_StbM_00319,1 */
#define STBM_API_GETSYNCTIMERECORDHEAD          0x16U

#if (defined STBM_API_GETOFFSETTIMERECORDHEAD)
#error STBM_API_GETOFFSETTIMERECORDHEAD is already defined.
#endif
/** \brief Service ID for StbM_GetOffsetTimeRecordHead() function.*/
/* !LINKSTO StbM.ASR44.SWS_StbM_00325,1 */
#define STBM_API_GETOFFSETTIMERECORDHEAD        0x17U

#if (defined STBM_API_GETTIMELEAP)
#error STBM_API_GETTIMELEAP is already defined.
#endif
/** \brief Service ID for StbM_GetTimeLeap() function.*/
/* !LINKSTO StbM.ASR44.SWS_StbM_00267,1 */
#define STBM_API_GETTIMELEAP                    0x13U

/* DET macros */

#if (defined STBM_E_PARAM)
#error STBM_E_PARAM is already defined.
#endif
/** \brief StbM DET error code for API call with invalid parameter value.*/
/* !LINKSTO StbM.ASR19-11.SWS_StbM_00041.STBM_E_PARAM,1 */
#define STBM_E_PARAM                            0x0aU

#if (defined STBM_E_UNINIT)
#error STBM_E_UNINIT is already defined.
#endif
/** \brief StbM DET error code for API call before module initialization.*/
#define STBM_E_UNINIT                           0x0bU

#if (defined STBM_E_PARAM_POINTER)
#error STBM_E_PARAM_POINTER is already defined.
#endif
/** \brief StbM DET error code for API call with invalid pointer address.*/
/* !LINKSTO StbM.ASR19-11.SWS_StbM_00041.STBM_E_PARAM_POINTER,1 */
#define STBM_E_PARAM_POINTER                    0x10U

#if (defined STBM_E_INIT_FAILED)
#error STBM_E_INIT_FAILED is already defined.
#endif
/** \brief StbM DET error code for initalization failure during StbM_Init.*/
#define STBM_E_INIT_FAILED                      0x11U

#if (defined STBM_E_SERVICE_DISABLED)
#error STBM_E_SERVICE_DISABLED is already defined.
#endif
/** \brief StbM DET error code for API disabled by configuration.*/
#define STBM_E_SERVICE_DISABLED                 0x12U

#if (defined STBM_E_PARAM_TIMESTAMP)
#error STBM_E_PARAM_TIMESTAMP is already defined.
#endif
/** \brief StbM DET error code for API call with invalid timestamp.*/
/* !LINKSTO StbM.ASR19-11.SWS_StbM_00041.STBM_E_PARAM_TIMESTAMP,1 */
#define STBM_E_PARAM_TIMESTAMP                  0x25U

#if (defined STBM_E_PARAM_USERDATA)
#error STBM_E_PARAM_USERDATA is already defined.
#endif
/** \brief StbM DET error code for API call with invalid user data.*/
/* !LINKSTO StbM.ASR19-11.SWS_StbM_00041.STBM_E_PARAM_USERDATA,1 */
#define STBM_E_PARAM_USERDATA                   0x26U


/*==[Types]===================================================================*/

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/* !LINKSTO StbM.EB.StbM_TimeStamps,1 */
#if (STBM_NVM_USED == STD_ON)
#define STBM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <StbM_MemMap.h>

/** \brief Actual time tuple of all synchronized and offset time bases */
extern VAR(StbM_TimeTupleType, STBM_VAR) StbM_TimeStamps
        [STBM_NUMBER_OF_SYNC_TIMEBASES + STBM_NUMBER_OF_OFFSET_TIMEBASES + STBM_NUMBER_OF_PURE_TIMEBASES];

#define STBM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <StbM_MemMap.h>
#endif /* STBM_NVM_USED == STD_ON */

/*==[Definition of functions with external linkage]===========================*/

#define STBM_START_SEC_CODE
#include <StbM_MemMap.h>

/** \brief Initialize the StbM module
 **
 ** This function initializes the StbM module
 **
 ** \param[in] ConfigPtr Pointer to the selected configuration set.
 **
 ** \ServiceID{0x00}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, STBM_CODE) StbM_Init
(
  P2CONST(StbM_ConfigType, AUTOMATIC, STBM_APPL_DATA)ConfigPtr
);

#if (STBM_VERSION_INFO_API == STD_ON)
/** \brief Get module version information.
 **
 ** Returns the version information of the StbM module
 **
 ** \param[out] versioninfo Version information of the StbM module
 **
 ** \ServiceID{0x05}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void,STBM_CODE) StbM_GetVersionInfo
(
   P2VAR(Std_VersionInfoType,AUTOMATIC,STBM_APPL_DATA) versioninfo
);
#endif /* #if (STBM_VERSION_INFO_API == STD_ON) */


/* !LINKSTO StbM.ASR44.SWS_StbM_00233,1 */
/** \brief Service to set global time by bus
 **
 ** Allows the Time Provider Modules to set the new Global Time tuple (i.e. the
 ** Received Time Tuple) to the StbM, which has been received on a bus.
 **
 ** \param[in] timeBaseId - ID of the synchronized time-base.
 ** \param[in] globalTimePtr - New Global Time value.
 ** \param[in] userDataPtr - User data of the time base.
 ** \param[in] measureDataPtr - New measurement data.
 ** \param[in] localTimePtr - Value of the Virtual Local Time associated to the new
 **                           Global Time.
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x0f}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_BusSetGlobalTime
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) globalTimePtr,
  P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userDataPtr,
  P2CONST(StbM_MeasurementType, AUTOMATIC, STBM_APPL_DATA) measureDataPtr,
  P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, STBM_APPL_DATA) localTimePtr
);

/** \brief Service to return the Virtual Local Time
 **
 ** Returns the Virtual Local Time of the referenced Time Base.
 **
 ** \param[in] timeBaseId - ID of the synchronized time-base.
 ** \param[out] localTimePtr - Current Virtual Local Time value.
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x09}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentVirtualLocalTime
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_VirtualLocalTimeType, AUTOMATIC, STBM_APPL_DATA) localTimePtr
);


/* !LINKSTO StbM.ASR19-11.SWS_StbM_91005,1 */
/** \brief Service to get current Time Tuple
 **
 ** Returns the Time Tuple (Global Time, Virtual Local Time) taken when
 ** the function is called.
 **
 ** \param[in] timeBaseId - ID of the synchronized time-base.
 ** \param[out] globalTimePtr - Value of the local instance of the Global
 **                            Time, which is sampled when the function is
 **                            called.
 ** \param[out] localTimePtr - Value of the Virtual Local Time, which is
 **                            sampled when the function is called.
 ** \param[out] userData - User data of the time base.
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x0A}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_BusGetCurrentTime
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) globalTimePtr,
  P2VAR(StbM_VirtualLocalTimeType, AUTOMATIC, STBM_APPL_DATA) localTimePtr,
  P2VAR(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData
);

/* !LINKSTO StbM.ASR44.SWS_StbM_00347,1 */
/** \brief Service to get the counter value of the time base
 **
 ** Allows the TimeSync Modules to detect, whether a Time Base should be
 ** transmitted immediately in the subsequent [Bus]TSyn_MainFunction() cycle.
 **
 ** \param[in] timeBaseId - ID of the synchronized time-base.
 **
 ** \return Counter value belonging to the Time Base, that indicates a Time
 **         Base update to the TimeSync Modules.
 **
 ** \ServiceID{0x1b}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(uint8, STBM_CODE) StbM_GetTimeBaseUpdateCounter
(
  StbM_SynchronizedTimeBaseType timeBaseId
);


/* !LINKSTO StbM.ASR44.SWS_StbM_00228,1 */
/** \brief Service to get the value of a offset time base
 **
 ** Allows the Timebase Provider Modules to get the currentoffset time.
 **
 ** \param[in] timeBaseId - ID of the synchronized time-base.
 ** \param[out] timeStamp - Time stamp containing the current offset time.
 ** \param[out] userData - The current user data.
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x0e}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetOffset
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStamp,
  P2VAR(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData
);





/* !LINKSTO StbM.EB.Prototypes.Rte,1 */
#if ((STBM_RTE_USED == STD_OFF) || (!defined(STBM_INTERNAL_USAGE)))

/** \brief StbM Mainfunction.
 **
 ** This function will be called cyclically by a task body provided
 ** by the BSW Scheduler.
 ** It will invoke the triggered customers and synchronize the referenced OS
 ** ScheduleTables.
 **
 ** \ServiceID{0x04}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 ** Timing: FIXED_CYCLIC.
 */
extern FUNC(void, STBM_CODE) StbM_MainFunction(void);


/* !LINKSTO StbM.ASR44.SWS_StbM_00195,1 */
/** \brief Service to get current time
 **
 ** Returns a time value - Local Time derived from Global Time.
 **
 ** \param[in] timeBaseId - ID of the synchronized time-base.
 ** \param[out] timeStamp - Time stamp containing the current time.
 ** \param[out] userData - User data of the time base.
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x07}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentTime
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStamp,
  P2VAR(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData
);


#if (STBM_GETCURRENTTIMEEXTENDED_USED == STD_ON)
/* !LINKSTO StbM.ASR44.SWS_StbM_00200,1 */
/** \brief Service to get current time
 **
 ** Returns an extended time value - Local Time derived from Global Time.
 **
 ** \param[in] timeBaseId - ID of the synchronized time-base.
 ** \param[out] timeStamp - extended Time stamp containing the current time.
 ** \param[out] userData - User data of the time base.
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x08}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentTimeExtended
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_TimeStampExtendedType, AUTOMATIC, STBM_APPL_DATA) timeStamp,
  P2VAR(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData
);
#endif /* (STBM_GETCURRENTTIMEEXTENDED_USED == STD_ON) */



#if (STBM_GET_MASTERCONFIG_SUPPORTED == STD_ON)
/* !LINKSTO StbM.ASR44.SWS_StbM_00408,1 */
/** \brief Service to get the value of StbMIsSystemWideGlobalTimeMaster parameter of the time base
 **
 ** Indicates if the functionality for a system wide master (e.g. StbM_SetGlobalTime) for a given Time Base is available or not.
 **
 ** \param[in] timeBaseId - Time Base reference
 ** \param[out] masterConfig - Indicates if system wide master functionality is supported
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x1d}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
FUNC(Std_ReturnType, STBM_CODE) StbM_GetMasterConfig
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_MasterConfigType, AUTOMATIC, STBM_APPL_DATA) masterConfig
);
#endif /* (STBM_GET_MASTERCONFIG_SUPPORTED == STD_ON) */


#if (STBM_MASTER_OR_GATEWAY_TIME_BASE_CONFIGURED == STD_ON)
/* !LINKSTO StbM.ASR44.SWS_StbM_00213,1 */
/** \brief Service to set global time
 **
 ** Allows the Customers to set the new global time that has to be valid for the
 ** system, which will be sent to the busses and modify HW registers behind the
 ** providers, if supported. This function will be used if a Time Master is present in
 ** this ECU.
 **
 ** \param[in] timeBaseId - ID of the synchronized time-base.
 ** \param[in] timeStamp - Time stamp containing the current time.
 ** \param[in] userData - User data of the time base.
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x0b}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_SetGlobalTime
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStamp,
  P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData
);


/* !LINKSTO StbM.ASR44.SWS_StbM_00385,1 */
/** \brief Service to update global time
 **
 ** Allows the Customers to set the Global Time that will be sent to the buses.
 ** This function will be used if a Time Master is present in this ECU. Using
 ** UpdateGlobalTime will not lead to an immediate transmission of the Global Time.
 **
 ** \param[in] timeBaseId - ID of the synchronized time-base.
 ** \param[in] timeStamp - Time stamp containing the current time.
 ** \param[in] userData - User data of the time base.
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x10}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_UpdateGlobalTime
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStamp,
  P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData
);


/* !LINKSTO StbM.ASR44.SWS_StbM_00218,1 */
/** \brief Service to set user specific data
 **
 ** Allows the Customers to set the new user data that has to be valid for the system,
 ** which will be sent to the busses.
 **
 ** \param[in] timeBaseId - ID of the synchronized time-base.
 ** \param[in] userData - User data of the time base.
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x0c}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_SetUserData
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData
);
#endif /* (STBM_MASTER_OR_GATEWAY_TIME_BASE_CONFIGURED == STD_ON) */


#if (STBM_SETOFFSET_SUPPORTED == STD_ON)
/* !LINKSTO StbM.ASR44.SWS_StbM_00223,1 */
/** \brief Service to set the value of a offset time base
 **
 ** Allows the Customers and the Timebase Provider Modules to set the offset time
 ** that has to be valid for the system.
 **
 ** \param[in] timeBaseId - ID of the synchronized time-base.
 ** \param[in] timeStamp - Time stamp containing the current offset time.
 ** \param[in] userData - User data of the time base.
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x0d}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_SetOffset
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStamp,
  P2CONST(StbM_UserDataType, AUTOMATIC, STBM_APPL_DATA) userData
);
#endif /* (STBM_SETOFFSET_SUPPORTED == STD_ON) */


#if (STBM_TRIGGERTIMETRANSMISSION_SUPPORTED == STD_ON)
/* !LINKSTO StbM.ASR44.SWS_StbM_00346,1 */
/** \brief Service to force the TimeSync Modules to transmit the current Time Base
 **
 ** Called by the [Upper Layer] to force the TimeSync Modules to transmit the
 ** current Time Base again due to an incremented timeBaseUpdateCounter[timeBaseId].
 **
 ** \param[in] timeBaseId - ID of the synchronized time-base.
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x1c}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_TriggerTimeTransmission
(
  StbM_SynchronizedTimeBaseType timeBaseId
);
#endif /* (STBM_TRIGGERTIMETRANSMISSION_SUPPORTED == STD_ON) */



#if (STBM_NOTIFICATION_OF_CUSTOMERS_USED == STD_ON)
/* !LINKSTO StbM.ASR19-11.SWS_StbM_00272,1 */
/** \brief Service to set a time value, which the Time Base value is compared against
 **
 ** Allows the Customers to set an expire time (through StartTimer C-S Interface),
 ** which the Time Base value is compared against.
 **
 ** \param[in] timeBaseId - Time Base reference
 ** \param[in] customerId - Status of the Synchronized Time Base
 ** \param[in] expireTime - Time value relative to current Time Base value of the Notification Customer, when the Timer shall expire
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x15}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_StartTimer
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  StbM_CustomerIdType customerId,
  P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) expireTime
);
#endif /* (STBM_NOTIFICATION_OF_CUSTOMERS_USED == STD_ON) */


/* !LINKSTO StbM.ASR44.SWS_StbM_00263,1 */
/** \brief Service to return the default status of the time base
 **
 ** Returns the detailed status of the Time Base. For Offset Time Bases the status of
 ** the Offset Time Base itself and the status of the underlying Synchronized Time
 ** Base is returned.
 **
 ** \param[in] timeBaseId - ID of the synchronized time-base.
 ** \param[out] syncTimeBaseStatus - Status of the Synchronized Time Base.
 ** \param[out] offsetTimeBaseStatus - Status of the Offset Time Base.
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x14}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetTimeBaseStatus
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_TimeBaseStatusType, AUTOMATIC, STBM_APPL_DATA) syncTimeBaseStatus,
  P2VAR(StbM_TimeBaseStatusType, AUTOMATIC, STBM_APPL_DATA) offsetTimeBaseStatus
);

/* !LINKSTO StbM.ASR44.SWS_StbM_00378,1 */
/** \brief Service to return the value of the current rate deviation of a Time Base
 **
 ** Returns value of the current rate deviation of a Time Base
 **
 ** \param[in] timeBaseId - Time Base reference.
 ** \param[out] rateDeviation - Value of the current rate deviation of a Time Base.
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x11}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetRateDeviation
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_RateDeviationType, AUTOMATIC, STBM_APPL_DATA) rateDeviation
);

/* !LINKSTO StbM.ASR44.SWS_StbM_00390,1 */
/* !LINKSTO StbM.EB.StbM_SetRateCorrection.API.Availability,1 */
#if(STBM_MASTER_RATE_CORRECTION_USED == STD_ON)
/** \brief Service to set the Rate Correction for a Time Base
 **
 ** Allows to set the rate of a Synchronized Time Base (being either a Pure
 ** Local Time Base or not).
 **
 ** \param[in] timeBaseId - Time Base reference.
 ** \param[in] rateDeviation - Value of the applied rate deviation.
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x12}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_SetRateCorrection
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  StbM_RateDeviationType rateDeviation
);
#endif /* (STBM_MASTER_RATE_CORRECTION_USED == STD_ON) */

#if(STBM_SYNC_SLAVE_TIMEBASES_CONFIGURED == STD_ON)
/* !LINKSTO StbM.ASR44.SWS_StbM_00267,1 */
/** \brief Service to return the value of the current time leap of a Time Base
 **
 ** Returns value of the current time leap of a Time Base
 **
 ** \param[in] timeBaseId - Time Base reference.
 ** \param[out] timeJump  - Time leap value.
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 **
 ** \ServiceID{0x13}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetTimeLeap
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_TimeDiffType, AUTOMATIC, STBM_APPL_DATA) timeJump
);
#endif /* STBM_SYNC_SLAVE_TIMEBASES_CONFIGURED == STD_ON */

/* If Rte usage is enabled in the StbM configuration the declaration of these
 * functions are provided by Rte_StbM.h. */

#if ((STBM_SYNC_TIME_RECORDING_USED == STD_ON) || (STBM_OFFSET_TIME_RECORDING_USED == STD_ON))

#if (STBM_SYNC_TIME_RECORDING_USED == STD_ON)
/* !LINKSTO StbM.ASR44.SWS_StbM_00319,1 */
/** \brief Service to get Sync Header Time Record information
 **
 ** Accesses to the recorded snapshot data Header of the table belonging
 ** to the Synchronized Time Base.
 **
 ** \param[in] timeBaseId - Time Base reference.
 ** \param[in] syncRecordTableHead - Header of the table.
 **
 ** \return the success/failure of the function call
 ** \retval E_OK Table access done.
 ** \retval E_NOT_OK Table contains no data or access invalid.
 **
 ** \ServiceID{0x16}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetSyncTimeRecordHead
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_SyncRecordTableHeadType, AUTOMATIC, STBM_APPL_DATA) syncRecordTableHead
);
#endif /* (STBM_SYNC_TIME_RECORDING_USED == STD_ON) */


#if (STBM_OFFSET_TIME_RECORDING_USED == STD_ON)
/* !LINKSTO StbM.ASR44.SWS_StbM_00325,1 */
/** \brief Service to get Offset Header Time Record information
 **
 ** Accesses to the recorded snapshot data Header of the table belonging
 ** to the Offset Time Base.
 **
 ** \param[in] timeBaseId - Time Base reference.
 ** \param[in] offsetRecordTableHead - Header of the table.
 **
 ** \return the success/failure of the function call
 ** \retval E_OK Table access done.
 ** \retval E_NOT_OK Table contains no data or access invalid.
 **
 ** \ServiceID{0x17}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetOffsetTimeRecordHead
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_OffsetRecordTableHeadType, AUTOMATIC, STBM_APPL_DATA) offsetRecordTableHead
);
#endif /* (STBM_OFFSET_TIME_RECORDING_USED == STD_ON) */

#endif /* ((STBM_SYNC_TIME_RECORDING_USED == STD_ON) || (STBM_OFFSET_TIME_RECORDING_USED == STD_ON)) */


#endif /* (STBM_RTE_USED == STD_OFF) || (!defined(STBM_INTERNAL_USAGE)) */

#define STBM_STOP_SEC_CODE
#include <StbM_MemMap.h>

/*==================[end of file]===========================================*/


#endif /* ifndef STBM_H */
