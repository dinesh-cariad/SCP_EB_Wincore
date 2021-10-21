/**
 * \file
 *
 * \brief AUTOSAR UdpNm
 *
 * This file contains the implementation of the AUTOSAR
 * module UdpNm.
 *
 * \version 2.9.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef UDPNM_H
#define UDPNM_H
/* !LINKSTO EB_SWS_UdpNm_00082_3,1, EB_SWS_UdpNm_00044_1,1 */

/*==================[inclusions]============================================*/
#include <UdpNm_Adaptive.h>

#if (UDPNM_ADAPTIVE == STD_OFF)
#ifndef UDPNM_NO_CFG_REQUIRED
#define UDPNM_NO_CFGCLASSMIX_REQUIRED
#include <UdpNm_Cfg.h>           /* Module configuration */
#endif /* UDPNM_NO_CFG_REQUIRED */

#if (!defined UDPNM_NO_LCFG_REQUIRED) && (!defined UDPNM_NO_CFG_REQUIRED)
#include <UdpNm_Lcfg.h>          /* Link-Time configuration */
#endif /* UDPNM_NO_LCFG_REQUIRED */

/* Exclude post-build-time configuration include file if requested to do so */
#ifndef UDPNM_NO_PBCFG_REQUIRED
#if (UDPNM_PBCFGM_SUPPORT_ENABLED == STD_OFF)
#include <UdpNm_PBcfg.h>     /* post build configuration of UdpNm */
#endif /* UDPNM_PBCFGM_SUPPORT_ENABLED == STD_OFF */
#endif /* UDPNM_NO_PBCFG_REQUIRED */

#undef UDPNM_NO_CFG_REQUIRED
#undef UDPNM_NO_LCFG_REQUIRED
#undef UDPNM_NO_PBCFG_REQUIRED
/*==================[macros]================================================*/
#endif /* #include <UdpNm_Adaptive.h> */
#include <UdpNm_Api.h>

/* !LINKSTO UdpNm.EB.EB_UDPNM_Define,1 */
#if (defined EB_UDPNM)
#error EB_UDPNM is already defined
#endif
#define EB_UDPNM

#endif /* ifndef UDPNM_H */

/*==================[end of file]===========================================*/
