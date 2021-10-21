#ifndef EA_CFG_H
#define EA_CFG_H
/**
 * \file
 *
 * \brief AUTOSAR Ea
 *
 * This file contains the implementation of the AUTOSAR
 * module Ea.
 *
 * \version 5.12.14
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[includes]==============================================*/
/* !LINKSTO EA083,1, EA113,1  */
#include <MemIf_Types.h>         /* Memory Abstraction Interface Types */
#include <Eep.h>                 /* EEPROM Header */
[!IF "((node:exists(EaGeneral/EaNvmJobErrorNotification)) and
(EaGeneral/EaNvmJobErrorNotification != '') and (EaGeneral/EaNvmJobErrorNotification != 'NULL_PTR'))
or ((node:exists(EaGeneral/EaNvmJobEndNotification)) and 
(EaGeneral/EaNvmJobEndNotification != '') and (EaGeneral/EaNvmJobEndNotification != 'NULL_PTR'))"!]
#include <NvM_Cbk.h>            /* NVRAM Manager callback API */
[!ENDIF!]
/*==================[macros]================================================*/

#if (defined EA_DEV_ERROR_DETECT)
#error EA_DEV_ERROR_DETECT is already defined
#endif
/** \brief Flag to en/disable development error detection for the Ea */
#define EA_DEV_ERROR_DETECT      [!//
[!IF "EaGeneral/EaDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined EA_VERSION_INFO_API)
#error EA_VERSION_INFO_API is already defined
#endif
/** \brief Provide API function Ea_GetVersionInfo() */
#define EA_VERSION_INFO_API      [!//
[!IF "EaGeneral/EaVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined EA_POLLING_MODE)
#error EA_POLLING_MODE is already defined
#endif
/** \brief Indicates if polling is enabled or not. */
#define EA_POLLING_MODE          [!//
[!IF "EaGeneral/EaPollingMode = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined EA_VIRTUAL_PAGE_SIZE)
#error EA_VIRTUAL_PAGE_SIZE is already defined
#endif
/** \brief size of virtual page in bytes. */
#define EA_VIRTUAL_PAGE_SIZE     [!"EaGeneral/EaVirtualPageSize"!]U

#if (defined EA_SET_MODE_SUPPORTED)
#error EA_SET_MODE_SUPPORTED is already defined
#endif
/** \brief Indicates if Ea_SetMode() is enabled or not. */
#define EA_SET_MODE_SUPPORTED    [!//
[!IF "EaGeneral/EaSetModeSupported = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!NOCODE!][!//
--- Get general configuration parameters
[!VAR "thisVirtualPageSize" = "EaGeneral/EaVirtualPageSize"!]
---  Generate internal buffer size
[!IF "node:exists(EaGeneral/EaBuffer) and (num:i(EaGeneral/EaBuffer) >= num:i(2*$thisVirtualPageSize))"!]
  [!VAR "thisTempBufferLength" = "num:i(EaGeneral/EaBuffer)"!]
[!ELSE!]
  [!VAR "thisTempBufferLength" = "num:i(2*$thisVirtualPageSize)"!]
[!ENDIF!]
--- Minimum buffer size is: 2 * 1 Byte (VLEC) + 1 Byte (IF) = 3 Byte
[!IF "$thisTempBufferLength < 3"!]
  [!VAR "thisTempBufferLength" = "num:i(3)"!]
[!ENDIF!]
[!ENDNOCODE!][!//

#if (defined EA_TEMPBUFFER_LENGTH)
#error EA_TEMPBUFFER_LENGTH is already defined
#endif
/** \brief Temporary buffer length */
#define EA_TEMPBUFFER_LENGTH     [!"$thisTempBufferLength"!]U

#if (defined EA_NUMBEROFBLOCKS)
#error EA_NUMBEROFBLOCKS is already defined
#endif
/** \brief Number of blocks configured. */
#define EA_NUMBEROFBLOCKS        [!"num:i(count(EaBlockConfiguration/*))"!]U

#if (defined EA_EEP_ERASE_VALUE)
#error EA_EEP_ERASE_VALUE is already defined
#endif
/** \brief Erase pattern as published by the Eep driver */
#define EA_EEP_ERASE_VALUE       [!//
[!"as:modconf('Eep')[1]/EepPublishedInformation/EepEraseValue"!]U

/* -----------[Callback function names to upper layer module]-------------- */

#if (defined EA_JOB_END_NFY_CBK)
#error EA_JOB_END_NFY_CBK is already defined
#endif
/** \brief Callback function in upper layer to indicate successful job
 * completion. */
#define EA_JOB_END_NFY_CBK()     [!//
[!IF "node:exists(EaGeneral/EaNvmJobEndNotification) and
(EaGeneral/EaNvmJobEndNotification != '') and (EaGeneral/EaNvmJobEndNotification != 'NULL_PTR')"!][!//
[!"EaGeneral/EaNvmJobEndNotification"!]()[!ELSE!]do {} while (0)[!ENDIF!]

#if (defined EA_JOB_ERROR_NTY_CBK)
#error EA_JOB_ERROR_NTY_CBK is already defined
#endif
/** \brief Callback function in upper layer to indicate unsuccessful job
 * completion. */
#define EA_JOB_ERROR_NTY_CBK()   [!//
[!IF "node:exists(EaGeneral/EaNvmJobErrorNotification) and
(EaGeneral/EaNvmJobErrorNotification != '') and (EaGeneral/EaNvmJobErrorNotification != 'NULL_PTR')"!][!//
[!"EaGeneral/EaNvmJobErrorNotification"!]()[!ELSE!]do {} while (0)[!ENDIF!]

/* -----------[Symbolic names for the Ea blocks]--------------------------- */

[!LOOP "EaBlockConfiguration/*"!][!//
#if (defined EaConf_EaBlockConfiguration_[!"name(.)"!])
#error EaConf_EaBlockConfiguration_[!"name(.)"!] is already defined
#endif
/** \brief Symbolic name for the Ea Block Number [!"EaBlockNumber"!] */
#define EaConf_EaBlockConfiguration_[!"name(.)"!]   [!"EaBlockNumber"!]U

#ifndef EA_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
/** \brief Export symbolic name value without prefix (AUTOSAR version <= 3.1 rev4) */
#if (defined [!"name(.)"!])
#error [!"name(.)"!] is already defined
#endif
#define [!"name(.)"!]   [!"EaBlockNumber"!]U
/** \brief Export symbolic name value with module abbreviation as prefix
 ** only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#if (defined Ea_[!"@name"!])
#error Ea_[!"@name"!] is already defined
#endif
#define Ea_[!"@name"!] [!"EaBlockNumber"!]U
#endif /* ifndef EA_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!][!//

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef EA_CFG_H */
/*==================[end of file]===========================================*/
