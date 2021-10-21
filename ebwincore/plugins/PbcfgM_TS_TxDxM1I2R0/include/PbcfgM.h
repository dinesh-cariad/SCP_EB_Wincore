/**
 * \file
 *
 * \brief AUTOSAR PbcfgM
 *
 * This file contains the implementation of the AUTOSAR
 * module PbcfgM.
 *
 * \version 1.2.22
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef PBCFGM_H
#define PBCFGM_H

/*==================[inclusions]============================================*/

#include <PbcfgM_Api.h>

/* Exclude post-build-time configuration include file if requested to do so */
#if (!defined PBCFGM_NO_PBCFG_REQUIRED) && (!defined PBCFGM_NO_CFG_REQUIRED)
#include <PbcfgM_PBcfg.h>     /* post build configuration of PbcfgM */
#endif /* PBCFGM_NO_PBCFG_REQUIRED */

#undef PBCFGM_NO_PBCFG_REQUIRED

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

#endif /* ifndef PBCFGM_H */
/*==================[end of file]===========================================*/
