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

#ifndef DCM_HSMDCM_H
#define DCM_HSMDCM_H

/* Public interface for the Dcm
 * state machine.
 *
 * This file defines the public symbols and functions to use the
 * Dcm state machine.
 */

/*==================[inclusions]============================================*/

#include <Std_Types.h>                            /* AUTOSAR standard types */

/* include state machine driver interface */
#include <Dcm_Hsm.h>

/*==================[macros]================================================*/

/** \brief Events/signal defined in the Dcm state machine. */
#if (defined DCM_HSM_DCM_EV_BL_TX_RESPONSE)
  #error "DCM_HSM_DCM_EV_BL_TX_RESPONSE is already defined"
#endif
#define DCM_HSM_DCM_EV_BL_TX_RESPONSE 0U

#if (defined DCM_HSM_DCM_EV_REQUEST_RECEIVED_RCRRP)
  #error "DCM_HSM_DCM_EV_REQUEST_RECEIVED_RCRRP is already defined"
#endif
#define DCM_HSM_DCM_EV_REQUEST_RECEIVED_RCRRP 1U

#if (defined DCM_HSM_DCM_EV_RETURN_FROM_BL)
  #error "DCM_HSM_DCM_EV_RETURN_FROM_BL is already defined"
#endif
#define DCM_HSM_DCM_EV_RETURN_FROM_BL 2U

#if (defined DCM_HSM_DCM_EV_REQUEST_RECEIVED)
  #error "DCM_HSM_DCM_EV_REQUEST_RECEIVED is already defined"
#endif
#define DCM_HSM_DCM_EV_REQUEST_RECEIVED 3U

#if (defined DCM_HSM_DCM_EV_ROE)
  #error "DCM_HSM_DCM_EV_ROE is already defined"
#endif
#define DCM_HSM_DCM_EV_ROE 4U

#if (defined DCM_HSM_DCM_EV_RDBPI_TYPE1)
  #error "DCM_HSM_DCM_EV_RDBPI_TYPE1 is already defined"
#endif
#define DCM_HSM_DCM_EV_RDBPI_TYPE1 5U

#if (defined DCM_HSM_DCM_EV_RDBPI_TYPE2)
  #error "DCM_HSM_DCM_EV_RDBPI_TYPE2 is already defined"
#endif
#define DCM_HSM_DCM_EV_RDBPI_TYPE2 6U

#if (defined DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT)
  #error "DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT is already defined"
#endif
#define DCM_HSM_DCM_EV_PERIODIC_UUDT_NEXT 7U

#if (defined DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE)
  #error "DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE is already defined"
#endif
#define DCM_HSM_DCM_EV_PERIODIC_UUDT_DONE 8U

#if (defined DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM)
  #error "DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM is already defined"
#endif
#define DCM_HSM_DCM_EV_PERIODIC_UUDT_CONFIRM 9U
#if (defined DCM_HSM_DCM_EV_PROC_ABORT)
  #error "DCM_HSM_DCM_EV_PROC_ABORT is already defined"
#endif
#define DCM_HSM_DCM_EV_PROC_ABORT 10U

#if (defined DCM_HSM_DCM_EV_RETRY)
  #error "DCM_HSM_DCM_EV_RETRY is already defined"
#endif
#define DCM_HSM_DCM_EV_RETRY 11U

#if (defined DCM_HSM_DCM_EV_PROC_DONE)
  #error "DCM_HSM_DCM_EV_PROC_DONE is already defined"
#endif
#define DCM_HSM_DCM_EV_PROC_DONE 12U

#if (defined DCM_HSM_DCM_EV_FORCE_RCRRP)
  #error "DCM_HSM_DCM_EV_FORCE_RCRRP is already defined"
#endif
#define DCM_HSM_DCM_EV_FORCE_RCRRP 13U

#if (defined DCM_HSM_DCM_EV_TIMEOUT2)
  #error "DCM_HSM_DCM_EV_TIMEOUT2 is already defined"
#endif
#define DCM_HSM_DCM_EV_TIMEOUT2 14U

#if (defined DCM_HSM_DCM_EV_CANCELLATION_DONE)
  #error "DCM_HSM_DCM_EV_CANCELLATION_DONE is already defined"
#endif
#define DCM_HSM_DCM_EV_CANCELLATION_DONE 15U

#if (defined DCM_HSM_DCM_EV_TX_CONF_NOT_OK)
  #error "DCM_HSM_DCM_EV_TX_CONF_NOT_OK is already defined"
#endif
#define DCM_HSM_DCM_EV_TX_CONF_NOT_OK 16U

#if (defined DCM_HSM_DCM_EV_TX_CONF_OK)
  #error "DCM_HSM_DCM_EV_TX_CONF_OK is already defined"
#endif
#define DCM_HSM_DCM_EV_TX_CONF_OK 17U

#if (defined DCM_HSM_DCM_EV_CANCEL_PROCESSING)
  #error "DCM_HSM_DCM_EV_CANCEL_PROCESSING is already defined"
#endif
#define DCM_HSM_DCM_EV_CANCEL_PROCESSING 18U

#if (defined DCM_HSM_DCM_EV_PROVIDE_TX_BUF)
  #error "DCM_HSM_DCM_EV_PROVIDE_TX_BUF is already defined"
#endif
#define DCM_HSM_DCM_EV_PROVIDE_TX_BUF 19U

#if (defined DCM_HSM_DCM_EV_PROCESS_PAGE)
  #error "DCM_HSM_DCM_EV_PROCESS_PAGE is already defined"
#endif
#define DCM_HSM_DCM_EV_PROCESS_PAGE 20U

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

#define DCM_START_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/** \brief Initialization data for the statechart statechart Dcm */
extern CONST(Dcm_HsmStatechartType, DCM_CONST)
 Dcm_HsmScDcm;

#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include <Dcm_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* defined DCM_HSMDCM_H */
/*==================[end of file]===========================================*/
