/**
 * \file
 *
 * \brief AUTOSAR ComM
 *
 * This file contains the implementation of the AUTOSAR
 * module ComM.
 *
 * \version 5.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef COMM_TYPES_H
#define COMM_TYPES_H

/*==================[inclusions]============================================*/

#include <ComM_BSW_Types.h>/* Types required in own module's implementation */
#if (COMM_INCLUDE_RTE == STD_ON)
#include <Rte_ComM_Type.h>       /* Include Symbols defined by RTE */
#endif /* #if (COMM_INCLUDE_RTE == STD_ON) */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))

/* Provide the typedef for ComM_InhibitionStatusType only if it
 *  was not provided by RTE */
#ifndef RTE_TYPE_ComM_InhibitionStatusType
#define RTE_TYPE_ComM_InhibitionStatusType
/** \brief Inhibition Status Type
 *
 * Provide AUTOSAR 4.0 InhibitionStatusType as default to other BSW modules
 */
/* !LINKSTO ComM.Types_Implicit2,1 */
typedef ComM_ASR40_InhibitionStatusType ComM_InhibitionStatusType;
#endif
#endif /* COMM_MODE_LIMITATION_ENABLED ... */

/* Provide the typedef for ComM_UserHandleType only if it
 *  was not provided by RTE */
#ifndef RTE_TYPE_ComM_UserHandleType
#define RTE_TYPE_ComM_UserHandleType
/** \brief User Handle Type
 *
 * Provide AUTOSAR 4.0 UserHandleType as default to other BSW modules
 */
/* !LINKSTO ComM.Types_Implicit3,1 */
typedef ComM_ASR40_UserHandleType ComM_UserHandleType;
#endif

/* Provide the typedef for ComM_ModeType only if it
 *  was not provided by RTE */
#ifndef RTE_TYPE_ComM_ModeType
#define RTE_TYPE_ComM_ModeType
/** \brief Mode Type
 *
 * Provide AUTOSAR 4.0 ComM_ModeType as default to other BSW modules
 */
/* !LINKSTO ComM.Types_Implicit4,1 */
typedef ComM_ASR40_ModeType ComM_ModeType;
#endif

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef COMM_TYPES_H */
/*==================[end of file]===========================================*/
