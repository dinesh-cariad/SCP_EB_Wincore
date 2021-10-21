/**
 * \file
 *
 * \brief AUTOSAR EcuM
 *
 * This file contains the implementation of the AUTOSAR
 * module EcuM.
 *
 * \version 5.15.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef ECUM_INT_CFG_H
#define ECUM_INT_CFG_H

/*==================[includes]==============================================*/

/*==================[macros]================================================*/

#if (defined ECUM_STORED_SHUTDOWN_OPERATIONS)
#error ECUM_STORED_SHUTDOWN_OPERATIONS already defined
#endif
/** \brief Size of the array which contains all the recent
 shutdown information. Used to implement EcuM_GetNextRecentShutdown() */
#define ECUM_STORED_SHUTDOWN_OPERATIONS   [!//
[!"EcuMFlexGeneral/EcuMStoredShutdownOperations"!]U

/*------------------[EcuM Multi-Core implementation macros]-----------------------*/

[!VAR "MULTICORE_ENABLED" = "node:exists(as:modconf('Os')[1]/OsOS/OsNumberOfCores) and node:value(as:modconf('Os')/OsOS/OsNumberOfCores) > 1 and node:value(as:modconf('EcuM')/EcuMGeneral/EcuMEnableMulticore) = 'true'"!]
[!IF "$MULTICORE_ENABLED"!]
[!VAR "MULTICORE_NUM_CORES" = "node:value(as:modconf('Os')[1]/OsOS/OsNumberOfCores)"!]
[!ELSE!][!//
[!VAR "MULTICORE_NUM_CORES" = "1"!]
[!ENDIF!]

[!IF "node:exists(as:modconf('EcuM')/EcuMGeneral/EcuMMasterCoreId)"!][!//
[!VAR "ECUM_MASTER_CORE_ID" = "node:value(as:modconf('EcuM')/EcuMGeneral/EcuMMasterCoreId)"!][!//
[!ELSE!][!//
[!VAR "ECUM_MASTER_CORE_ID" = "'0'"!][!//
[!ENDIF!][!//

[!IF "node:exists(as:modconf('EcuM')/EcuMGeneral/EcuMInitialCoreId)"!][!//
[!VAR "ECUM_INITIAL_CORE_ID" = "node:value(as:modconf('EcuM')/EcuMGeneral/EcuMInitialCoreId)"!][!//
[!ENDIF!][!//

[!SELECT "EcuMConfiguration/*[1]/EcuMCommonConfiguration"!][!//
[!IF "node:exists(EcuMDriverInitListZero)"!]
#define ECUM_AL_DRIVERINITZERO() EcuM_AL_DriverInitZero()
[!ELSE!]
#define ECUM_AL_DRIVERINITZERO()
[!ENDIF!]

[!IF "node:exists(EcuMDriverInitListOne)"!]
#define ECUM_AL_DRIVERINITONE() EcuM_AL_DriverInitOne(EcuM_ConfigPtr)
[!ELSE!]
#define ECUM_AL_DRIVERINITONE()
[!ENDIF!]
[!ENDSELECT!][!//

#if (defined ECUM_NUM_CORES)
#error ECUM_NUM_CORES already defined
#endif
/** \brief Preprocessor macro defining the number of cores configured. */
#define ECUM_NUM_CORES [!"num:i($MULTICORE_NUM_CORES)"!]U

#if (defined ECUM_CORE_ID_MASTER)
#error ECUM_CORE_ID_MASTER already defined
#endif
/** \brief Preprocessor macro defining the core configured as master core. */
#define ECUM_CORE_ID_MASTER [!"num:i($ECUM_MASTER_CORE_ID)"!]U

[!IF "$MULTICORE_ENABLED"!][!//
#if (defined ECUM_CORE_ID_INITIAL)
#error ECUM_CORE_ID_INITIAL already defined
#endif
/** \brief Preprocessor macro defining the initially active core . */
[!IF "node:exists(as:modconf('EcuM')/EcuMGeneral/EcuMInitialCoreId)"!][!//
#define ECUM_CORE_ID_INITIAL [!"num:i($ECUM_INITIAL_CORE_ID)"!]U
[!ELSE!][!//
#define ECUM_CORE_ID_INITIAL OS_CORE_ID_MASTER
[!ENDIF!][!//

#if (defined ECUM_SLAVE_SYNC_MASK)
#error ECUM_SLAVE_SYNC_MASK already defined
#endif
/** \brief a bit mask  for the slave core Ids. */
#define ECUM_SLAVE_SYNC_MASK \
  ( \
[!FOR "Index" = "0" TO "$MULTICORE_NUM_CORES - 1"!][!IF "$Index != $ECUM_MASTER_CORE_ID"!]((uint32)1U << OS_CORE_ID_[!"num:i($Index)"!]) | [!ENDIF!][!ENDFOR!]\
  0U)
[!ENDIF!]

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
[!IF "$MULTICORE_ENABLED"!]
#define ECUM_START_SEC_CODE
#include <EcuM_MemMap.h>
/** \brief Starts all Os cores except the designated initial core. */
extern FUNC(void, ECUM_CODE) EcuM_StartAllCores(void);
#define ECUM_STOP_SEC_CODE
#include <EcuM_MemMap.h>
[!ENDIF!]
/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef ECUM_INT_CFG_H */
/*==================[end of file]===========================================*/
