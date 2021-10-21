/**
 * \file
 *
 * \brief AUTOSAR LinSM
 *
 * This file contains the implementation of the AUTOSAR
 * module LinSM.
 *
 * \version 3.4.19
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef LINSM_CBK_H
#define LINSM_CBK_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h>         /* EB specific standard types */
#include <ComStack_Types.h>    /* AUTOSAR communication stack types */
#include <LinSM_Version.h>     /* this module's version declaration */
#include <LinSM_Cfg.h>         /* this module's configuration */
#include <LinIf.h>             /* LinIf declarations */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function declarations]========================*/

#define LINSM_START_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

#if (LINSM_MASTER_NODES_ENABLED == STD_ON)

/** \brief Confirmation callout for schedule table changes
 **
 ** The LinIf module will call this callback when the new requested schedule
 ** table is active.
 **
 ** \param[in] network   Identification of the LIN channel
 ** \param[in] schedule  Index of the new active Schedule table
 **
 ** \ServiceID{0x20}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, LINSM_CODE) LinSM_ScheduleRequestConfirmation
(
  NetworkHandleType network,
  LinIf_SchHandleType schedule
);

#endif


/** \brief Confirmation callout for WakeUp
 **
 ** This callout must be called by the LinIf after a wakeup request has been
 ** received using LinIf_Wakeup. It signals if the wakeup request was
 ** successful.
 **
 ** Note that the LinIf has to call this function in any case if the call to
 ** LinIf_Wakeup has returned E_OK. That means, even if there is no wakeup
 ** request carried out on the bus (because the LinIf channel is already
 ** awake), the confirmation must be called nonetheless.
 **
 ** \param[in] network  Identification of the LIN channel (LinSMChannelIndex)
 ** \param[in] success  True if wakeup was successfully sent, false otherwise
 **
 ** \ServiceID{0x21}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, LINSM_CODE) LinSM_WakeupConfirmation
(
  NetworkHandleType network,
  boolean success
);


/** \brief Confirmation callout for GotoSleep transition
 **
 ** The LinIf will call this callback when the go to sleep command is sent
 ** successfully or not sent successfully on the network.
 **
 ** \param[in] network  Identification of the LIN channel
 ** \param[in] success  True if goto sleep was successfully sent, false
 **                      otherwise
 **
 ** \ServiceID{0x22}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, LINSM_CODE) LinSM_GotoSleepConfirmation
(
  NetworkHandleType network,
  boolean success
);


#if (LINSM_SLAVE_NODES_ENABLED == STD_ON)
/** \brief Indication callout for GotoSleep transition
 **
 ** The LinIf will call this callback when the go to sleep command
 ** is received on the network or a bus idle timeout occurs.
 ** Only applicable for LIN slave nodes.
 **
 ** \param[in] Channel  Identification of the LIN channel
 **
 ** \ServiceID{0x03}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, LINSM_CODE) LinSM_GotoSleepIndication
(
  NetworkHandleType Channel
);
#endif


#define LINSM_STOP_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */

#endif /* ifndef LINSM_CBK_H */
/*==================[end of file]===========================================*/

