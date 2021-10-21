/**
 * \file
 *
 * \brief AUTOSAR LinIf
 *
 * This file contains the implementation of the AUTOSAR
 * module LinIf.
 *
 * \version 5.8.27
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef LINIF_CBK_H
#define LINIF_CBK_H

/*==================[inclusions]=============================================*/

#include <ComStack_Types.h>        /* AUTOSAR standard types */
#include <EcuM.h>                  /* EcuM_WakeupSourceType */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]========================*/

#define LINIF_START_SEC_CODE
#include <LinIf_MemMap.h>

/**
 ** \brief Wakeup Confirmation function
 **
 ** This service is called by the LIN Driver or by the LIN Tranciever Driver
 ** to report the wake up source after the successful wakeup detection
 ** during CheckWakeup or after power on by bus.
 **
 ** \param[in] WakeupSource Source device which initiated the
 **                         wakeup event: LIN controller or LIN transceiver
 **
 ** \ServiceID{0x61}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, LINIF_CODE) LinIf_WakeupConfirmation
(
  EcuM_WakeupSourceType WakeupSource
);

#define LINIF_STOP_SEC_CODE
#include <LinIf_MemMap.h>

#endif /* ifndef LINIF_CBK_H */
/*==================[end of file]============================================*/
