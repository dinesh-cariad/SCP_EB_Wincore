#ifndef FEE_CBK_H
#define FEE_CBK_H
/**
 * \file
 *
 * \brief AUTOSAR Fee
 *
 * This file contains the implementation of the AUTOSAR
 * module Fee.
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2017 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <Fee_Cfg.h>              /* External API declarations  */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
#define FEE_START_SEC_CODE
#include <MemMap.h>

#if FEE_POLLING_MODE == STD_OFF
/** \brief Callback function to notify the successful completion of a flash job
 **
 ** This is called by the underlying flash driver to report
 ** the successful completion of an asynchronous operation.
 **
 ** \ServiceID{0x10}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous} */
/* !LINKSTO FEE069,1 */
extern FUNC(void,FEE_CODE) Fee_JobEndNotification(void);

/** \brief Callback function to notify the failure of a flash job
 **
 ** This is called by the underlying flash driver to report
 ** the failure of an asynchronous operation.
 **
 ** \ServiceID{0x11}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous} */
/* !LINKSTO FEE069,1 */
extern FUNC(void,FEE_CODE) Fee_JobErrorNotification(void);

#endif
#define FEE_STOP_SEC_CODE
#include <MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
#endif /* ifndef FEE_CBK_H */
