
/* !LINKSTO EA114,1 */
#ifndef EA_CBK_H
#define EA_CBK_H
/**
 * \file
 *
 * \brief AUTOSAR Ea
 *
 * This file contains the implementation of the AUTOSAR
 * module Ea.
 *
 * \version 5.12.14
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <Ea_Version.h> /* the module's version information */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define EA_START_SEC_CODE
#include <Ea_MemMap.h>

/** \brief Called by the EEPROM driver when a job is successfully finished
 **
 ** This callback function is called by the underlying EEPROM Driver to
 ** report the successful end of an asynchronous operation. When a job
 ** given to the EEPROM Abstraction module is finished, a callback function
 ** will be called in the upper layer.
 **
 ** \ServiceID{16}
 ** \Reentrancy{non reentrant}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(void, EA_CODE) Ea_JobEndNotification(void);

/** \brief Called by the EEPROM driver when a job fails
 **
 ** This callback function is called by the underlying EEPROM Driver to
 ** report the failure of an asynchronous operation. When a job given to
 ** the EEPROM Abstraction module is finished, a callback function will be
 ** called in the upper layer.
 **
 ** \ServiceID{17}
 ** \Reentrancy{non reentrant}
 ** \Synchronicity{synchronous}
 **
 */
extern FUNC(void, EA_CODE) Ea_JobErrorNotification(void);

#define EA_STOP_SEC_CODE
#include <Ea_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef EA_CBK_H */
/*==================[end of file]===========================================*/
