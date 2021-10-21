#ifndef ECUM_H
#define ECUM_H

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

#include <EcuM_Types.h>         /* EcuM's common module types */
#include <EcuM_BSW.h>           /* EcuM's common API and configuration */
#include <EcuM_Cbk.h>           /* EcuM's callout and callback interface */
#include <EcuM_Lcfg.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/* If the Rte interface and the multicore functionality are enabled, the prototype for EcuM_SelectShutdownTarget
   is exposed by SchM_EcuM.h since a BswSwcMapping is present in the EcuM BSW module description file.
   This BswSwcMapping is not present for the ASR40 and ASR32 Service interface. */
#if !((ECUM_MULTICORE_ENABLED == TRUE) && (ECUM_INCLUDE_RTE == STD_ON))
/* !LINKSTO EcuM.EB.ASR32.EcuM100,1 */
/**
* \brief Wrapping macro for EcuM_SelectShutdownTarget
*
* Provides an AUTOSAR 4.0 API as default to other BSW modules.
*/
#define EcuM_SelectShutdownTarget EcuM_ASR40_SelectShutdownTarget
#endif

/* !LINKSTO EcuM.EB.ASR32.EcuM100,1 */
/**
* \brief Wrapping macro for EcuM_GetShutdownTarget
*
* Provides an AUTOSAR 4.0 API as default to other BSW modules.
*/
#define EcuM_GetShutdownTarget EcuM_ASR40_GetShutdownTarget

/* !LINKSTO EcuM.EB.ASR32.EcuM100,1 */
/**
* \brief Wrapping macro for EcuM_GetLastShutdownTarget
*
* Provides an AUTOSAR 4.0 API as default to other BSW modules.
*/
#define EcuM_GetLastShutdownTarget EcuM_ASR40_GetLastShutdownTarget

/* !LINKSTO EcuM.EB.ASR32.EcuM100,1 */
/**
* \brief Wrapping macro for EcuM_SelectShutdownCause
*
* Provides an AUTOSAR 4.0 API as default to other BSW modules.
*/
#define EcuM_SelectShutdownCause EcuM_ASR40_SelectShutdownCause

/* !LINKSTO EcuM.EB.ASR32.EcuM100,1 */
/**
* \brief Wrapping macro for EcuM_GetShutdownCause
*
* Provides an AUTOSAR 4.0 API as default to other BSW modules.
*/
#define EcuM_GetShutdownCause EcuM_ASR40_GetShutdownCause

/* !LINKSTO EcuM.EB.ASR32.EcuM100,1 */
/**
* \brief Wrapping macro for EcuM_GetMostRecentShutdown
*
* Provides an AUTOSAR 4.0 API as default to other BSW modules.
*/
#define EcuM_GetMostRecentShutdown EcuM_ASR40_GetMostRecentShutdown

/* !LINKSTO EcuM.EB.ASR32.EcuM100,1 */
/**
* \brief Wrapping macro for EcuM_GetNextRecentShutdown
*
* Provides an AUTOSAR 4.0 API as default to other BSW modules.
*/
#define EcuM_GetNextRecentShutdown EcuM_ASR40_GetNextRecentShutdown

/* !LINKSTO EcuM.EB.ASR32.EcuM100,1 */
/**
* \brief Wrapping macro for EcuM_SelectBootTarget
*
* Provides an AUTOSAR 4.0 API as default to other BSW modules.
*/
#define EcuM_SelectBootTarget EcuM_ASR40_SelectBootTarget

/* !LINKSTO EcuM.EB.ASR32.EcuM100,1 */
/**
* \brief Wrapping macro for EcuM_GetBootTarget
*
* Provides an AUTOSAR 4.0 API as default to other BSW modules.
*/
#define EcuM_GetBootTarget EcuM_ASR40_GetBootTarget

/*==================[internal function declarations]========================*/
/* !LINKSTO EcuM.Impl.PBVariant.IsValid,1 */

/** \brief Validates the post-build configuration data structure used in the
 ** EcuM_Init function.
 **
 **
 ** \param voidConfigPtr The pointer to a EcuM post-build data structure.
 ** \retval E_OK When the pre-compile, link-time and platform hash values stored
 ** within the post-build structure correspond to the hash values of the
 ** compiled source files.  Otherwise, E_NOT_OK will be returned. */
FUNC(Std_ReturnType, ECUM_CODE) EcuM_IsValidConfig
(
  P2CONST(void, AUTOMATIC, ECUM_APPL_CONST) voidConfigPtr
);

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
#endif /* ifndef ECUM_H */

