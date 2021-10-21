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

#ifndef CANNM_HSMCFG_H
#define CANNM_HSMCFG_H

/* configuration options for the hsm */

/*==================[inclusions]============================================*/

#include <CanNm_Cfg.h> /* number of state machines */

/*==================[macros]================================================*/

/** \brief Number of instances of hsm CanNm
 *
 * Definition is only needed if CANNM_HSM_INST_MULTI_ENABLED==STD_ON */
#if (defined CANNM_HSM_CANNM_NUM_INST)
#error CANNM_HSM_CANNM_NUM_INST is already defined
#endif
#define CANNM_HSM_CANNM_NUM_INST          CANNM_NUMBER_OF_CHANNELS


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
