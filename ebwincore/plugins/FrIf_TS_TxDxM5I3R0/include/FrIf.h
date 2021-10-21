/**
 * \file
 *
 * \brief AUTOSAR FrIf
 *
 * This file contains the implementation of the AUTOSAR
 * module FrIf.
 *
 * \version 5.3.25
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef FRIF_H_
#define FRIF_H_

/*======================[Include Section]=====================================*/

#include <FrIf_Api.h>           /* get AUTOSAR FrIf prototypes               */
#include <FrIf_Lcfg.h>          /* get AUTOSAR FrIf link-time configuration  */

/* Exclude post-build-time config include file if requested to do so */
#if (!defined FRIF_NO_PBCFG_REQUIRED) && (!defined FRIF_NO_CFG_REQUIRED)
#if (FRIF_PBCFGM_SUPPORT_ENABLED == STD_OFF)
#include <FrIf_PBcfg.h>         /* get AUTOSAR FrIf post build configuration */
#endif /* FRIF_PBCFGM_SUPPORT_ENABLED == STD_OFF */
#endif /* FRIF_NO_PBCFG_REQUIRED, FRIF_NO_CFG_REQUIRED */


#undef FRIF_NO_PBCFG_REQUIRED
#undef FRIF_NO_CFG_REQUIRED

/*======================[Global Macros]=======================================*/

/*======================[Global Data Types]===================================*/

/*======================[Global Data]=========================================*/

/*======================[Global Function Declarations]========================*/

#endif /* FRIF_H_ */

