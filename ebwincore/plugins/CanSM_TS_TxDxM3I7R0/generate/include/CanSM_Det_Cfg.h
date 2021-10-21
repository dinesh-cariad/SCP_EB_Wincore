/**
 * \file
 *
 * \brief AUTOSAR CanSM
 *
 * This file contains the implementation of the AUTOSAR
 * module CanSM.
 *
 * \version 3.7.5
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CANSM_DET_CFG_H
#define CANSM_DET_CFG_H
/*==================[includes]==============================================*/

/*==================[version check]=========================================*/

/*==================[macros]================================================*/

#if (defined CANSM_DEV_ERROR_DETECT)
#error CANSM_DEV_ERROR_DETECT is already defined
#endif /* #if (defined CANSM_DEV_ERROR_DETECT) */

/** \brief Switch for DET usage */
[!IF "as:modconf('CanSM')[1]/CanSMGeneral/CanSMDevErrorDetect = 'true'"!][!//
#define CANSM_DEV_ERROR_DETECT   STD_ON
[!ELSE!][!//
#define CANSM_DEV_ERROR_DETECT   STD_OFF
[!ENDIF!][!//

/*==================[type definitions]======================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function declarations]========================*/

#endif /* ifndef CANSM_DET_CFG_H */
/*==================[end of file]===========================================*/

