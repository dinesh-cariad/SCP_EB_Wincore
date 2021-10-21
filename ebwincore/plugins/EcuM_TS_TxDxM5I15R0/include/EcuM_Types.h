
#ifndef ECUM_TYPES_H
#define ECUM_TYPES_H

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

/*==================[inclusions]============================================*/

#include <EcuM_BSW_Types.h>         /* static module types */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

#ifndef RTE_TYPE_EcuM_StateType
#define RTE_TYPE_EcuM_StateType
/* !LINKSTO EcuM.EB.ASR32.EcuM105,1 */
/** \brief EcuM States Type
 *
 * Provide AUTOSAR 4.0 EcuM_StateType as default to other BSW modules
 */

typedef EcuM_ASR40_StateType EcuM_StateType;
#endif

#ifndef RTE_TYPE_EcuM_UserType
#define RTE_TYPE_EcuM_UserType
/* !LINKSTO EcuM.EB.ASR32.EcuM105,1 */
/** \brief EcuM Usere Type
 *
 * Provide AUTOSAR 4.0 EcuM_UserType as default to other BSW modules
 */
typedef EcuM_ASR40_UserType EcuM_UserType;
#endif

#ifndef RTE_TYPE_EcuM_ShutdownCauseType
#define RTE_TYPE_EcuM_ShutdownCauseType
/* !LINKSTO EcuM.EB.ASR32.EcuM105,1 */
/** \brief EcuM Shutdown Cause Type
 *
 * Provide AUTOSAR 4.0 EcuM_ShutdownCauseType as default to other BSW modules
 */
typedef EcuM_ASR40_ShutdownCauseType EcuM_ShutdownCauseType;
#endif

# ifndef RTE_TYPE_EcuM_BootTargetType
#  define RTE_TYPE_EcuM_BootTargetType
/* !LINKSTO EcuM.EB.ASR32.EcuM105,1 */
/** \brief Type for boot target
 **
 ** Stores the type of the next boot target.
 **
 ** All valid values for this type are defined as preprocessor macros. */
typedef EcuM_ASR40_BootTargetType EcuM_BootTargetType;
# endif

/* --------------EcuM Multi-Core synchronization types---------------------- */
typedef uint8 EcuM_SyncActionType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
#endif /* ifndef ECUM_TYPES_H */

