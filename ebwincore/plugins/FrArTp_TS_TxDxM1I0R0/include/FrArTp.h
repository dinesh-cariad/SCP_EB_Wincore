/**
 * \file
 *
 * \brief AUTOSAR FrArTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrArTp.
 *
 * \version 1.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef FRARTP_H
#define FRARTP_H

/*==================[inclusions]============================================*/

#include <FrArTp_Api.h>            /* general public API */

/* Exclude post-build-time config include file if requested to do so */
#if (!defined FRARTP_NO_PBCFG_REQUIRED) && (!defined FRARTP_NO_CFG_REQUIRED)
#if (FRARTP_PBCFGM_SUPPORT_ENABLED == STD_OFF)
#include <FrArTp_PBcfg.h>         /* get FrArTp post build configuration */
#endif /* FRARTP_PBCFGM_SUPPORT_ENABLED == STD_OFF */
#endif /* FRARTP_NO_PBCFG_REQUIRED, FRARTP_NO_CFG_REQUIRED */


#undef FRARTP_NO_PBCFG_REQUIRED
#undef FRARTP_NO_CFG_REQUIRED

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef FRARTP_H */
/*==================[end of file]===========================================*/
