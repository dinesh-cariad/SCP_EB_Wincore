/**
 * \file
 *
 * \brief AUTOSAR CanNm
 *
 * This file contains the implementation of the AUTOSAR
 * module CanNm.
 *
 * \version 6.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CANNM_H
#define CANNM_H

/*==================[inclusions]============================================*/

#ifndef CANNM_NO_CFG_REQUIRED
#define CANNM_NO_CFGCLASSMIX_REQUIRED
#include <CanNm_Cfg.h>           /* Module configuration */
#endif /* CANNM_NO_CFG_REQUIRED */

#if (!defined CANNM_NO_LCFG_REQUIRED) && (!defined CANNM_NO_CFG_REQUIRED)
#include <CanNm_Lcfg.h>          /* Link-Time configuration */
#endif /* CANNM_NO_LCFG_REQUIRED */

/* Exclude post-build-time configuration include file if requested to do so */
#ifndef CANNM_NO_PBCFG_REQUIRED
#if (CANNM_PBCFGM_SUPPORT_ENABLED == STD_OFF)
#include <CanNm_PBcfg.h>     /* post build configuration of CanNm */
#endif /* CANNM_PBCFGM_SUPPORT_ENABLED == STD_OFF */
#endif /* CANNM_NO_PBCFG_REQUIRED */

#undef CANNM_NO_CFG_REQUIRED
#undef CANNM_NO_LCFG_REQUIRED
#undef CANNM_NO_PBCFG_REQUIRED

#include <CanNm_Api.h>

/*==================[macros]================================================*/
/* !LINKSTO CanNm.EB.EB_CANNM_Define,1 */
#if (defined EB_CANNM)
#error EB_CANNM is already defined
#endif
#define EB_CANNM

#endif /* ifndef CANNM_H */
/*==================[end of file]===========================================*/
