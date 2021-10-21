/**
 * \file
 *
 * \brief AUTOSAR WdgM
 *
 * This file contains the implementation of the AUTOSAR
 * module WdgM.
 *
 * \version 6.1.39
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!INCLUDE "WdgM_Cfg.m"!][!//
[!CODE!][!//
#ifndef WDGM_BSW_TYPES_H
#define WDGM_BSW_TYPES_H

/*==================[inclusions]=================================================================*/
/* !LINKSTO WDGM.EB.Design.IncludeFileStructure,1 */
#include <WdgM_Types.h>

/*==================[macros]=====================================================================*/

/*==================[type definitions]===========================================================*/
[!IF "$WdgMMulticoreEnabled = 'false'"!][!//

/** \brief Global Init status
 *
 * This type is used to represent the global initialization status.
 * of the Watchdog Manager. */
typedef uint32 WdgM_EB_GlobalInitStatusType;

/** \brief Mode id
 *
 * This type distinguishes the different modes that were configured for the
 * Watchdog Manager. */
typedef uint8 WdgM_EB_ModeType;

/** \brief Local status
 *
 * This type is used to represent the local supervision status of the
 * individual supervised entities.
 */
typedef uint8 WdgM_EB_LocalStatusType;

/** \brief Global status
 *
 * This type is used to represent the global supervision status
 * of the Watchdog Manager. */
typedef uint8 WdgM_EB_GlobalStatusType;

/** \brief the first expired SEId
 *
 * Holding the SEId and the bit-inverse value */
typedef uint16 WdgM_EB_ExpiredSEIDType;

[!ELSE!][!/* IF "$WdgMMulticoreEnabled = 'false'"*/!][!//

/** \brief Global Init status
 *
 * This type is used to represent the global initialization status.
 * of the Watchdog Manager. */
typedef Atomic_t WdgM_EB_GlobalInitStatusType;

/** \brief Mode id
 *
 * This type distinguishes the different modes that were configured for the
 * Watchdog Manager. */
typedef Atomic_value_t WdgM_EB_ModeType;

/** \brief Local status
 *
 * This type is used to represent the local supervision status of the
 * individual supervised entities.
 */
typedef Atomic_value_t WdgM_EB_LocalStatusType;

/** \brief Global status
 *
 * This type is used to represent the global supervision status
 * of the Watchdog Manager. */
typedef Atomic_value_t WdgM_EB_GlobalStatusType;

/** \brief the first expired SEId
 *
 * Holding the SEId and the bit-inverse value */
typedef Atomic_value_t WdgM_EB_ExpiredSEIDType;

[!ENDIF!][!/* IF "$WdgMMulticoreEnabled = 'false'"*/!][!//
/*==================[external function declarations]=============================================*/

/*==================[internal function declarations]=============================================*/

/*==================[external constants]=========================================================*/

/*==================[internal constants]=========================================================*/

/*==================[external data]==============================================================*/

/*==================[internal data]==============================================================*/

/*==================[external function definitions]==============================================*/

/*==================[internal function definitions]==============================================*/

#endif /* ifndef WDGM_BSW_TYPES_H */
/*==================[end of file]================================================================*/
[!ENDCODE!][!//
