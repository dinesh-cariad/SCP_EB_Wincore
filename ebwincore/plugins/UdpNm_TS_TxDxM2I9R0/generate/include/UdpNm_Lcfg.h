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
#ifndef UDPNM_LCFG_H
#define UDPNM_LCFG_H

[!AUTOSPACING!]
#include <UdpNm_Adaptive.h>
/*==================[includes]==============================================*/
#if (UDPNM_ADAPTIVE == STD_ON)
#include <AdaptiveUdpNm_Cfg.h>
#else
#include <TSAutosar.h>              /* global configuration */

/*==================[macros]================================================*/
/* !LINKSTO UdpNm.EB.PostBuildRamSize_1,1 */
#define UDPNM_DATA_MEM_SIZE [!"num:i(as:modconf('UdpNm')[1]/UdpNmGlobalConfig/*[1]/UdpNmPostBuildRamSize)"!]
/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/* start data section declaration */
#define UDPNM_START_SEC_CONST_32
#include <UdpNm_MemMap.h>

/* value used to validate post build configuration against link time configuration */
extern CONST(uint32, UDPNM_CONST) UdpNm_LcfgSignature;

/* stop data section declaration */
#define UDPNM_STOP_SEC_CONST_32
#include <UdpNm_MemMap.h>

#endif /* UDPNM_ADAPTIVE == STD_OFF */
/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef UDPNM_LCFG_H */
/*==================[end of file]===========================================*/
