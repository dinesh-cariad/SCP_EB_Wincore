/**
 * \file
 *
 * \brief AUTOSAR FrNm
 *
 * This file contains the implementation of the AUTOSAR
 * module FrNm.
 *
 * \version 5.16.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef FRNM_H
#define FRNM_H

/*==================[inclusions]=========================================*/

#ifndef FRNM_NO_CFG_REQUIRED
#define FRNM_NO_CFGCLASSMIX_REQUIRED
#include <FrNm_Cfg.h>           /* Module configuration */
#endif /* FRNM_NO_CFG_REQUIRED */

#if (!defined FRNM_NO_LCFG_REQUIRED) && (!defined FRNM_NO_CFG_REQUIRED)
#include <FrNm_Lcfg.h>          /* Link-Time configuration */
#endif /* FRNM_NO_LCFG_REQUIRED */

/* Exclude post-build-time configuration include file if requested to do so */
#ifndef FRNM_NO_PBCFG_REQUIRED
#if (FRNM_PBCFGM_SUPPORT_ENABLED == STD_OFF)
#include <FrNm_PBcfg.h>     /* post build configuration of FrNm */
#endif /* FRNM_PBCFGM_SUPPORT_ENABLED == STD_OFF */
#endif /* FRNM_NO_PBCFG_REQUIRED */

#undef FRNM_NO_CFG_REQUIRED
#undef FRNM_NO_LCFG_REQUIRED
#undef FRNM_NO_PBCFG_REQUIRED

#include <FrNm_Api.h>

/*==================[macros]=================================================*/
/* !LINKSTO FrNm.EB.EB_FrNm_Define,1 */
#if (defined EB_FRNM)
#error EB_FRNM is already defined
#endif
#define EB_FRNM

/*==================[type definitions]=======================================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef FRNM_H */
/*==================[end of file]========================================*/
