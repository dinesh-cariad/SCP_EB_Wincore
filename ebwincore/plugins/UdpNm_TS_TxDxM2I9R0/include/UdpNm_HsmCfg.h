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

#ifndef UDPNM_HSMCFG_H
#define UDPNM_HSMCFG_H

/* configuration options for the hsm */

/*==================[inclusions]============================================*/

#include <UdpNm_Cfg.h> /* number of state machines */
#include <UdpNm_Adaptive.h>
#if (UDPNM_ADAPTIVE == STD_ON)
#include <AdaptiveUdpNm_Cfg.h>
#endif
/*==================[macros]================================================*/

#if (defined UDPNM_HSM_UDPNM_NUM_INST)
#error UDPNM_HSM_UDPNM_NUM_INST already defined
#endif

/** \brief Number of instances of hsm UdpNm
 *
 * Definition is only needed if UDPNM_HSM_INST_MULTI_ENABLED==STD_ON */
#define UDPNM_HSM_UDPNM_NUM_INST          UDPNM_NUMBER_OF_CHANNELS



/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif
/*==================[end of file]===========================================*/
