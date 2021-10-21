/**
 * \file
 *
 * \brief AUTOSAR FrNm
 *
 * This file contains the implementation of the AUTOSAR
 * module FrNm.
 *
 * \version 5.16.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef FRNM_HSMCFG_H
#define FRNM_HSMCFG_H

/* Configuration options for the hsm */

/*==================[inclusions]============================================*/
#include <FrNm_Cfg.h> /* number of state machines */
/*==================[macros]================================================*/

/** \brief Number of instances of hsm <HSM_NAME>
 *
 * Definition is only needed if FRNM_HSM_INST_MULTI_ENABLED==STD_ON */
#if (defined FRNM_HSM_FRNM_NUM_INST)
#error FRNM_HSM_FRNM_NUM_INST already defined
#endif
#define FRNM_HSM_FRNM_NUM_INST         FRNM_NUMBER_OF_CHANNELS


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
