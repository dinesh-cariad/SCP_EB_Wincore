[!AUTOSPACING!]
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
#ifndef STBM_EXTERNALS_H
#define STBM_EXTERNALS_H

/** \addtogroup Synchronized Time Base Manager
 ** @{ */
 
/*==[Includes]================================================================*/

[!INCLUDE "StbM_Vars.m"!]

#include <StbM_Int_Cfg.h>
#include <TSAutosar.h>

/*==[Macros]==================================================================*/

/*==[Types]===================================================================*/

/*==[Declaration of functions with internal linkage]==========================*/

/*==[Constants with internal linkage]=========================================*/

/*==[Variables with internal linkage]=========================================*/

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/


[!IF "($StbMTimeBaseSyncTimeRecord = 'true') or ($StbMTimeBaseOffsetTimeRecord = 'true') or ($StbMTimeBaseNotificationStatus = 'true') or ($StbMNotificationOfCustomersUsed = 'true')"!]

#define STBM_START_SEC_CODE
#include <StbM_MemMap.h>

[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*, './StbMSynchronizedTimeBaseIdentifier')"!]
/* [!"node:name(.)"!] */

[!IF "$StbMTimeBaseNotificationStatus = 'true'"!]
    [!IF "((node:value(StbMNotificationInterface) = 'CALLBACK_AND_SR_INTERFACE') or (node:value(StbMNotificationInterface) = 'CALLBACK'))"!]
      [!CALL "GetTimeBaseCallbackName", "timeBaseNode" = "node:path(.)", "functionName" = "node:value(StbMStatusNotificationCallback)"!]
  extern Std_ReturnType [!"$callbackName"!](uint32 eventNotification);
    [!ELSE!]
  /* Status Notification Callback not needed for [!"node:name(.)"!] time base */
    [!ENDIF!]
[!ENDIF!]

[!IF "$StbMTimeBaseSyncTimeRecord = 'true'"!]
    [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
    [!IF "((($isSlaveConnected = 'isSlave') or ($isMasterSlaveConnected = 'true')) and (node:exists(StbMTimeRecording/StbMSyncTimeRecordBlockCallback)))"!]
      [!CALL "GetTimeBaseCallbackName", "timeBaseNode" = "node:path(.)", "functionName" = "node:value(StbMTimeRecording/StbMSyncTimeRecordBlockCallback)"!]
  extern Std_ReturnType [!"$callbackName"!](const StbM_SyncRecordTableBlockType* syncRecordTableBlock);
    [!ELSE!]
  /* Sync Time Recording Callback not needed for [!"node:name(.)"!] time base */
    [!ENDIF!]
[!ENDIF!]

[!IF "$StbMTimeBaseOffsetTimeRecord = 'true'"!]
    [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
    [!IF "((($isSlaveConnected = 'isSlave') or ($isMasterSlaveConnected = 'true')) and (node:exists(StbMTimeRecording/StbMOffsetTimeRecordBlockCallback)))"!]
      [!CALL "GetTimeBaseCallbackName", "timeBaseNode" = "node:path(.)", "functionName" = "node:value(StbMTimeRecording/StbMOffsetTimeRecordBlockCallback)"!]
  extern Std_ReturnType [!"$callbackName"!](const StbM_OffsetRecordTableBlockType* offsetRecordTableBlock);
    [!ELSE!]
  /* Offset Time Recording Callback not needed for [!"node:name(.)"!] time base */
    [!ENDIF!]
[!ENDIF!]

[!IF "$StbMNotificationOfCustomersUsed = 'true'"!]
    [!IF "(count(StbMNotificationCustomer/*) > 0)"!]
      [!LOOP "node:order(StbMNotificationCustomer/*, './StbMNotificationCustomerId')"!]
        [!IF "node:exists(StbMTimeNotificationCallback) = 'true'"!]
          [!CALL "GetTimeNotificationCallbackName", "customer" = "node:path(.)"!]
  extern Std_ReturnType [!"$timeNotificationCallbackName"!](StbM_TimeDiffType deviationTime);
        [!ELSE!]
  /* Customer Notification Callback not needed for [!"node:name(.)"!] time base */
        [!ENDIF!]
      [!ENDLOOP!]
    [!ENDIF!]
[!ENDIF!]
[!ENDLOOP!]

#define STBM_STOP_SEC_CODE
#include <StbM_MemMap.h>

[!ENDIF!]

/*==[Definition of functions with internal linkage]===========================*/

/** @} doxygen end group definition */

#endif /* ifndef STBM_EXTERNALS_H */
/*==================[end of file]===========================================*/

