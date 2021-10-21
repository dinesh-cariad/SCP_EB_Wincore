#ifndef ETH_H
#define ETH_H

/**
 * \file
 *
 * \brief AUTOSAR Eth
 *
 * This file contains the implementation of the AUTOSAR
 * module Eth.
 *
 * \version 0.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2011 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

/* NOTE: this if-clause is only possible if the <MODULENAME>_H itself doesn't depend on the
 * PreCompile configuration! */
#ifndef ETH_NO_CFG_REQUIRED
/* Always include pre-compile-time information - needed by own and foreign modules */
/* don't mix PBcfg or Lcfg symbolic name values into Cfg since those headerfiles are included
 * anyway right afterwards */
#define ETH_NO_CFGCLASSMIX_REQUIRED
#include <Eth_Cfg.h>              /* pre-compile-time configuration */
#endif /* ETH_NO_CFG_REQUIRED */

/* Exclude post-build-time configuration include file if requested to do so */
#ifndef ETH_NO_PBCFG_REQUIRED
#if (ETH_PBCFGM_SUPPORT_ENABLED == STD_OFF)
#include <Eth_PBcfg.h>     /* post build configuration of Eth */
#endif /* ETH_PBCFGM_SUPPORT_ENABLED == STD_OFF */
#endif /* ETH_NO_PBCFG_REQUIRED */

#undef ETH_NO_LCFG_REQUIRED
#undef ETH_NO_PBCFG_REQUIRED
#undef ETH_NO_CFG_REQUIRED

#include <Eth_Api.h>        /* Eth type definitions */

/*==================[macros]================================================*/

/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef ETH_H */
/*==================[end of file]===========================================*/

