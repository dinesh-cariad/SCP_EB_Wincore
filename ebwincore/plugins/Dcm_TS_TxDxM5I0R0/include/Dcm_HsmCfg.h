/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef DCM_HSMCFG_H
#define DCM_HSMCFG_H

/*==================[inclusions]============================================*/

#include <Dcm_Types.h> /* DCM type definition */
#include <Dcm_Cfg.h> /* number of state machines */

/*==================[macros]================================================*/

/** \brief Enable multiple instances of the same statemachine */
#if (DCM_NUM_PROTOCOL > 1U)
#if (defined DCM_HSM_INST_MULTI_ENABLED)
  #error "DCM_HSM_INST_MULTI_ENABLED is already defined"
#endif
#define DCM_HSM_INST_MULTI_ENABLED      STD_ON
#else
#if (defined DCM_HSM_INST_MULTI_ENABLED)
  #error "DCM_HSM_INST_MULTI_ENABLED is already defined"
#endif
#define DCM_HSM_INST_MULTI_ENABLED      STD_OFF
#endif

/* *** Individual settings for each Hsm model *** */

/** \brief Number of instances of hsm DCM */
#if (defined DCM_HSM_DCM_NUM_INST)
  #error "DCM_HSM_DCM_NUM_INST is already defined"
#endif
#define DCM_HSM_DCM_NUM_INST            DCM_NUM_PROTOCOL

/** \brief Number of elements in the event queue for HSM DCM. */
#if (defined DCM_HSM_DCM_EV_QUEUE_SIZE)
  #error "DCM_HSM_DCM_EV_QUEUE_SIZE is already defined"
#endif
#define DCM_HSM_DCM_EV_QUEUE_SIZE       10U

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
