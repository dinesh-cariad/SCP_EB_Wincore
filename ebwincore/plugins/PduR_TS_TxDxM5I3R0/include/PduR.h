/**
 * \file
 *
 * \brief AUTOSAR PduR
 *
 * This file contains the implementation of the AUTOSAR
 * module PduR.
 *
 * \version 5.3.46
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef PDUR_H
#define PDUR_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h>     /* EB specific standard types */
#include <PduR_Api.h>      /* Module public API */

/* Exclude post-build-time configuration include file if requested to do so */
#if (!defined PDUR_NO_PBCFG_REQUIRED) && (!defined PDUR_NO_CFG_REQUIRED)
#if (PDUR_PBCFGM_SUPPORT_ENABLED == STD_OFF)
#include <PduR_PBcfg.h>    /* get name of PduR post build configuration */
#endif /* PDUR_PBCFGM_SUPPORT_ENABLED == STD_OFF */
#endif /* PDUR_NO_PBCFG_REQUIRED */

#undef PDUR_NO_PBCFG_REQUIRED
#undef PDUR_NO_CFG_REQUIRED

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

#endif /* ifndef PDUR_H */
/*==================[end of file]===========================================*/
