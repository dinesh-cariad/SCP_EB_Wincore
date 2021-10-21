#ifndef BSWM_TYPES_H
#define BSWM_TYPES_H
/**
 * \file
 *
 * \brief AUTOSAR BswM
 *
 * This file contains the implementation of the AUTOSAR
 * module BswM.
 *
 * \version 1.15.6
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <Std_Types.h>

/*==================[type definitions]=======================================*/

/* !LINKSTO BswM.Impl.BswM_ModeType,1 */
/** \brief BswM_ModeType is used by the BswM_RequestMode API */
typedef uint16 BswM_ModeType;

/* !LINKSTO BswM.Impl.BswM_UserType,1 */
/** \brief BswM_UserType is used by the BswM_RequestMode API */
typedef uint16 BswM_UserType;

/*==================[macros]=================================================*/

/*==================[end of file]============================================*/
#endif /* ifndef BSWM_TYPES_H */
