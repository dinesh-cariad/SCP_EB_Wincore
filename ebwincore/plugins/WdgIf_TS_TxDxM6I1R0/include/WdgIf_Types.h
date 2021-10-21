/**
 * \file
 *
 * \brief AUTOSAR WdgIf
 *
 * This file contains the implementation of the AUTOSAR
 * module WdgIf.
 *
 * \version 6.1.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef WDGIF_TYPES_H
#define WDGIF_TYPES_H
/*==================[inclusions]=============================================*/
#include <Std_Types.h>      /* AUTOSAR standard types */

/*==================[macros]=================================================*/

#ifdef WDGIF_OFF_MODE
#error WDGIF_OFF_MODE already defined!
#endif
/** \brief In this mode, the watchdog driver is disabled (switched off). */
#define WDGIF_OFF_MODE     0u

#ifdef WDGIF_SLOW_MODE
#error WDGIF_SLOW_MODE already defined!
#endif
/** \brief In this mode, the watchdog driver is set up for a long timeout period (slow triggering). */
#define WDGIF_SLOW_MODE     1u

#ifdef WDGIF_FAST_MODE
#error WDGIF_FAST_MODE already defined!
#endif
/** \brief In this mode, the watchdog driver is set up for a short timeout period (fast triggering). */
#define WDGIF_FAST_MODE     2u

/*==================[type definitions]=======================================*/

/** \brief Mode type of the WdgIf module
 **
 ** This uint8 type holds the mode types that are passed as parameters
 ** to Wdg_SetMode(). */
typedef uint8 WdgIf_ModeType;

/*==================[external function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

#endif /* ifndef WDGIF_TYPES_H */
/*==================[end of file]============================================*/
