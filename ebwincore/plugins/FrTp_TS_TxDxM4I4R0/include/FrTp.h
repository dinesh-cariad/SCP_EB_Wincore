/**
 * \file
 *
 * \brief AUTOSAR FrTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrTp.
 *
 * \version 4.4.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/*  MISRA-C:2012 Deviation List
 *
 *  MISRAC2012-1) Deviated Rule: 20.5 (advisory)
 *   #undef shall not be used.
 *
 *   Reason:
 *   Macro FRTP_NO_PBCFG_REQUIRED may be defined in
 *   more than one instance which will cause compile
 *   warning.
 */

#ifndef FRTP_H
#define FRTP_H

/*=====================[Include Section]=====================================*/

#include <FrTp_Api.h>       /* static configuration of FrTp */

/* Exclude post-build-time configuration include file if requested to do so */
#ifndef FRTP_NO_PBCFG_REQUIRED
#if (FRTP_PBCFGM_SUPPORT_ENABLED == STD_OFF)
#include <FrTp_PBcfg.h>     /* post build configuration of FrTp */
#endif /* FRTP_PBCFGM_SUPPORT_ENABLED == STD_OFF */
#endif /* FRTP_NO_PBCFG_REQUIRED */

/* Deviation MISRAC2012-1 */
#undef FRTP_NO_PBCFG_REQUIRED

/*=====================[Global Macros]=======================================*/

/*=====================[Global Macro Checks]=================================*/

/*=====================[Global Data Types]===================================*/

/*=====================[Global Function Declarations]========================*/

#endif /* FRTP_H */

