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
/* !LINKSTO ComM463_Refine_ComMh,1 */
#ifndef COMM_H
#define COMM_H

/*==================[inclusions]============================================*/

#include <ComM_Types.h>                                /* ComM's common API */
/* !LINKSTO ComM280,1 */
#include <ComM_BSW.h>                                  /* ComM's common API */

#include <ComM_Types_Int.h>                            /* PostBuild types */

/* Exclude post-build-time configuration file if requested to do so */
#ifndef COMM_NO_PBCFG_REQUIRED
#if (COMM_PBCFGM_SUPPORT_ENABLED == STD_OFF)
#include <ComM_PBcfg.h>     /* post build configuration of ComM */
#endif /* COMM_PBCFGM_SUPPORT_ENABLED == STD_OFF */
#endif /* COMM_NO_PBCFG_REQUIRED */

#if (COMM_PBCFGM_SUPPORT_ENABLED == STD_ON)
#include <PbcfgM_Api.h>         /* Post build configuration manager */
#endif /* COMM_PBCFGM_SUPPORT_ENABLED */

#include <ComM_Lcfg.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
#define COMM_START_SEC_CODE
#include <ComM_MemMap.h>

/** \brief Validates the post-build configuration data structure
 **
 ** This function validates the post-build configuration data structure
 ** passed to the ComM_Init function.
 **
 ** \param[in] voidConfigPtr - pointer to a ComM post-build data structure.  If a
 ** NULL_PTR is passed, the ComM will attempt to retrieve the ComM post-build
 ** configuration from the PbcfgM module.
 **
 ** \return Standard Return Code
 ** \retval E_OK When the pre-compile, link-time and platform hash values stored
 ** within the post-build structure correspond to the hash values of the
 ** compiled source files.
 ** \retval E_NOT_OK Otherwise, E_NOT_OK will be returned.
 **
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous} */
 FUNC(Std_ReturnType, COMM_CODE) ComM_IsValidConfig
(
  P2CONST(void, AUTOMATIC, COMM_APPL_CONST) voidConfigPtr
);

#define COMM_STOP_SEC_CODE
#include <ComM_MemMap.h>

/* hide declaration in own header if included by ComM itself and RTE is
 * enabled */
#if ((!defined(COMM_INTERNAL_USAGE)) || (COMM_INCLUDE_RTE == STD_OFF))

#if ((COMM_MODE_LIMITATION_ENABLED == STD_ON) || (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON))

/** \brief
 * Wrapping macro for ComM_GetInhibitionStatus to provide AUTOSAR 4.0 API as
 * default to other BSW modules
 */
#if (defined ComM_GetInhibitionStatus)
#error ComM_GetInhibitionStatus already defined
#endif
#define ComM_GetInhibitionStatus ComM_ASR40_GetInhibitionStatus

#endif

/** \brief
 * Wrapping macro for ComM_RequestComMode to provide AUTOSAR 4.0 API as
 * default to other BSW modules
 */
#if (defined ComM_RequestComMode)
#error ComM_RequestComMode already defined
#endif
#define ComM_RequestComMode ComM_ASR40_RequestComMode

/** \brief
 * Wrapping macro for ComM_GetMaxComMode to provide AUTOSAR 4.0 API as
 * default to other BSW modules
 */
#if (defined ComM_GetMaxComMode)
#error ComM_GetMaxComMode already defined
#endif
#define ComM_GetMaxComMode ComM_ASR40_GetMaxComMode

/** \brief
 * Wrapping macro for ComM_GetRequestedComMode to provide AUTOSAR 4.0 API as
 * default to other BSW modules
 */
#if (defined ComM_GetRequestedComMode)
#error ComM_GetRequestedComMode already defined
#endif
#define ComM_GetRequestedComMode ComM_ASR40_GetRequestedComMode

/** \brief
 * Wrapping macro for ComM_GetCurrentComMode to provide AUTOSAR 4.0 API as
 * default to other BSW modules
 */
#if (defined ComM_GetCurrentComMode)
#error ComM_GetCurrentComMode already defined
#endif
#define ComM_GetCurrentComMode ComM_ASR40_GetCurrentComMode

/** \brief
 * Wrapping macro for ComM_GetCurrentPNCComMode to provide AUTOSAR 4.0 API as
 * default to other BSW modules
 */
#if (defined ComM_GetCurrentPNCComMode)
#error ComM_GetCurrentPNCComMode already defined
#endif
#define ComM_GetCurrentPNCComMode ComM_ASR40_GetCurrentPNCComMode


#if (COMM_WAKEUP_INHIBITION_ENABLED == STD_ON)
/** \brief
 * Wrapping macro for ComM_PreventWakeUp to provide AUTOSAR 4.0 API as
 * default to other BSW modules
 */
#if (defined ComM_PreventWakeUp)
#error ComM_PreventWakeUp already defined
#endif
#define ComM_PreventWakeUp ComM_ASR40_PreventWakeUp

#endif

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
/** \brief
 * Wrapping macro for ComM_LimitChannelToNoComMode to provide AUTOSAR 4.0 API as
 * default to other BSW modules
 */
#if (defined ComM_LimitChannelToNoComMode)
#error ComM_LimitChannelToNoComMode already defined
#endif
#define ComM_LimitChannelToNoComMode ComM_ASR40_LimitChannelToNoComMode

#endif

#if (COMM_MODE_LIMITATION_ENABLED == STD_ON)
/** \brief
 * Wrapping macro for ComM_LimitECUToNoComMode to provide AUTOSAR 4.0 API as
 * default to other BSW modules
 */
#if (defined ComM_LimitECUToNoComMode)
#error ComM_LimitECUToNoComMode already defined
#endif
#define ComM_LimitECUToNoComMode ComM_ASR40_LimitECUToNoComMode

#endif

#if ((COMM_WAKEUP_INHIBITION_ENABLED == STD_ON) || (COMM_MODE_LIMITATION_ENABLED == STD_ON))
/** \brief
 * Wrapping macro for ComM_ReadInhibitCounter to provide AUTOSAR 4.0 API as
 * default to other BSW modules
 */
#if (defined ComM_ReadInhibitCounter)
#error ComM_ReadInhibitCounter already defined
#endif
#define ComM_ReadInhibitCounter ComM_ASR40_ReadInhibitCounter

/** \brief
 * Wrapping macro for ComM_ResetInhibitCounter to provide AUTOSAR 4.0 API as
 * default to other BSW modules
 */
#if (defined ComM_ResetInhibitCounter)
#error ComM_ResetInhibitCounter already defined
#endif
#define ComM_ResetInhibitCounter ComM_ASR40_ResetInhibitCounter

/** \brief
 * Wrapping macro for ComM_SetECUGroupClassification to provide AUTOSAR 4.0 API as
 * default to other BSW modules
 */
#if (defined ComM_SetECUGroupClassification)
#error ComM_SetECUGroupClassification already defined
#endif
#define ComM_SetECUGroupClassification ComM_ASR40_SetECUGroupClassification

#endif

#endif /* ((!defined(COMM_INTERNAL_USAGE)) || (COMM_INCLUDE_RTE == STD_OFF)) */

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef COMM_H */
/*==================[end of file]===========================================*/
