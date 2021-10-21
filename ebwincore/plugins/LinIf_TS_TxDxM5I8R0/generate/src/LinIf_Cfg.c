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
[!AUTOSPACING!]

[!NOCODE!][!INCLUDE "../include/LinIf_Macros.m"!][!ENDNOCODE!]

/* !LINKSTO LinIf.ASR40.LINIF241,1 */
/*==================[inclusions]=============================================*/

#include <LinIf_Cfg.h>          /* DET + TP ON/OFF configuration */
#include <LinIf_Types.h>        /* LinIf module types */
#include <LinTp_Types.h>        /* LinTp module types */
#include <LinIf_Int.h>          /* LinIf internal data */
#include <LinTp_Int.h>          /* LinTp internal data */

/*==================[external data]==========================================*/

#define LINIF_START_SEC_VAR_INIT_8
#include <LinIf_MemMap.h>

VAR(uint8, LINIF_VAR) LinIf_FrameEventFlags[LINIF_MAX_PDUS_FLAGS] = {0U}; /* [!"num:i(LinIfGeneral/LinIfMaxTxPdus)"!] bits used */
[!IF "$numChannelsMaster != 0"!][!//
VAR(uint8, LINIF_VAR) LinIf_EventTriggeredCollision[LINIF_MAX_EV_TRGD] = {0U}; /* [!WS!][!IF "LinIfGeneral/LinIfMaxEventTriggeredFrames = 0"!]1[!ELSE!][!"LinIfGeneral/LinIfMaxEventTriggeredFrames"!][!ENDIF!][!WS!] event triggered collision frames */
[!ENDIF!][!//

#define LINIF_STOP_SEC_VAR_INIT_8
#include <LinIf_MemMap.h>

[!IF "$numSlaveChannels != 0"!]
#define LINIF_START_SEC_VAR_CLEARED_8
#include <LinIf_MemMap.h>

VAR(uint8, LINIF_VAR) LinIf_PidTable[LINIF_MAX_SLAVE_FRAMES];

#define LINIF_STOP_SEC_VAR_CLEARED_8
#include <LinIf_MemMap.h>
[!ENDIF!][!//

#define LINIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <LinIf_MemMap.h>

VAR(LinIf_ChannelInfoType, LINIF_VAR) LinIf_ChannelInfo[LINIF_MAX_CHANNELS];
[!IF "$numChannelsMaster != 0"!][!//
VAR(LinIf_MasterChannelInfoType, LINIF_VAR) LinIf_MasterChannelInfo[LINIF_MAX_CHANNELS];
[!ENDIF!][!//
[!IF "$numSlaveChannels != 0"!][!//
VAR(LinIf_SlaveChannelInfoType, LINIF_VAR) LinIf_SlaveChannelInfo[LINIF_MAX_CHANNELS];
[!ENDIF!][!//
[!IF "LinIfGeneral/LinIfTpSupported = 'true'"!]
VAR(LinTp_ChannelInfoType, LINIF_VAR) LinTp_ChannelInfo[LINIF_MAX_CHANNELS];[!/*
*/!][!ENDIF!]

#define LINIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <LinIf_MemMap.h>

#define LINIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <LinIf_MemMap.h>

/** \brief Configuration of Lin Driver function pointers */
CONST( LinIf_LinDriverConfigType, LINIF_APPL_CONST )
  LinIf_LinDriverConfig[[!"num:i($numberOfDrivers)"!]] =
{
[!FOR "i"="0" TO "$numberOfDrivers - 1"!][!//
  [!IF "$useDriverInfix = 'true'"!][!//
    [!VAR "driverInfix"!][!CALL "getDriverInfo","driver" = "$currentDriver", "onlyInfix" = "true()", "index" = "$i"!]_[!ENDVAR!][!//
    [!IF "$driverInfix = '__'"!][!//
      [!VAR "driverInfix" = "''"!][!//
    [!ENDIF!][!//
  [!ELSE!][!//
    [!VAR "driverInfix" = "''"!][!//
  [!ENDIF!][!//
  {
[!IF "$numChannelsMaster > 0"!][!//
    &Lin_[!"$driverInfix"!]SendFrame,
    &Lin_[!"$driverInfix"!]GetStatus,
[!ENDIF!][!//
[!IF "LinIfGeneral/LinIfCheckWakeupSupported = 'true'"!][!//
[!IF "LinIfGeneral/LinIfLinDriverAPI != 'REV2'"!][!//
    &Lin_[!"$driverInfix"!]CheckWakeup,
[!ELSE!][!//
    &Lin_[!"$driverInfix"!]WakeupValidation,
[!ENDIF!][!//
[!ENDIF!][!//
[!IF "$numChannelsMaster > 0"!][!//
    &Lin_[!"$driverInfix"!]GoToSleep,
[!ENDIF!][!//
    &Lin_[!"$driverInfix"!]GoToSleepInternal,
    &Lin_[!"$driverInfix"!]Wakeup,
[!IF "LinIfGeneral/LinIfLinDriverAPI = 'REV42' or LinIfGeneral/LinIfLinDriverAPI = 'REV431' or LinIfGeneral/LinIfLinDriverAPI = 'REV44'"!][!//
    &Lin_[!"$driverInfix"!]WakeupInternal,
[!ENDIF!][!//
  },
[!ENDFOR!]
};

[!IF "$numTrcvs > 0"!][!//
[!CALL "EnumerateTrcvs"!][!//
CONST(LinIf_LinTrcvFuncPtrType, LINIF_APPL_CONST) LinIf_TrcvFuncPtrs[[!"$numTrcvs"!]U] =
{
[!NOCODE!]
[!LOOP "text:split($trcvLst, ';')"!]
[!VAR "apiPrefix" = "'&LinTrcv'"!]
[!IF "$trcvDoApiInfix = 'true'"!]
  [!VAR "apiPrefix" = "concat($apiPrefix, text:split(.,',')[4], text:split(.,',')[5])"!]
[!ENDIF!]
[!CODE!][!//
  {
    [!"concat($apiPrefix, '_SetOpMode')"!],
    [!"concat($apiPrefix, '_GetOpMode')"!],
    [!"concat($apiPrefix, '_GetBusWuReason')"!],
    [!"concat($apiPrefix, '_SetWakeupMode')"!],[!//
[!IF "$cwuNeeded = 'true'"!][!//
[!IF "text:split(.,',')[3] = 'true'"!]
    [!"concat($apiPrefix, '_CheckWakeup')"!]
[!ELSE!]
    NULL_PTR
[!ENDIF!][!//
[!ENDIF!][!//
  },
[!ENDCODE!]
[!ENDLOOP!]
[!ENDNOCODE!][!//
};
[!ENDIF!]

#define LINIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <LinIf_MemMap.h>

/*==================[end of file]============================================*/
