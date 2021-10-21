/**
 * \file
 *
 * \brief AUTOSAR CanSM
 *
 * This file contains the implementation of the AUTOSAR
 * module CanSM.
 *
 * \version 3.7.5
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CANSM_SCHM_H
#define CANSM_SCHM_H

 /* mgcc62 Deviation List
 *
 * MGCC62-1) Deviated Rule: redundant-decls
 * warning: redundant redeclaration of 'CanSM_MainFunction' [-Wredundant-decls]
 *
 * Reason:
 * MainFunction needs to be exported through <module>_SchM.h for Rte usage.
 * Rte also exports the function in SchM_<module>.h, redeclaring the function.
 */

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>    /* AUTOSAR communication stack types */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function declarations]========================*/

#define CANSM_START_SEC_CODE
#include <CanSM_MemMap.h>

/** \brief This function handles asynchronous events, such as mode changes.
 **
 **
 ** This is the main function of the CanSM. It handles asynchronous
 ** events, such as mode changes. It is called cyclically with a fixed period
 ** from the BSW Scheduler.
 **
 ** \ProductionError{CANSM_E_BUS_OFF, thrown\, if there is a notification of
 **                  a bus-off event on a CAN controller}
 **
 ** \ServiceID{5}
 */
/* Deviation MGCC62-1 */
extern FUNC(void, CANSM_CODE) CanSM_MainFunction (
  void
);

#define CANSM_STOP_SEC_CODE
#include <CanSM_MemMap.h>

#endif /* ifndef CANSM_SCHM_H */
/*==================[end of file]===========================================*/

