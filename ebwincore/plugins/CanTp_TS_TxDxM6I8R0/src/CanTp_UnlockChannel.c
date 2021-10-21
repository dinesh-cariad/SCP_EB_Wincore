/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/

#include <CanTp_Trace.h>
#include <TSAutosar.h>         /* EB specific standard types */
#include <ComStack_Types.h>    /* typedefs for AUTOSAR com stack */
#include <CanTp_Internal.h>    /* internal macros and variables */
#include <CanTp_Cbk.h>         /* CanTp callback declarations */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/
#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)
/** \brief Handles Timeouts
 **
 ** This function handles postponed Rx timeout conditions and notifies PduR with
 ** corresponding timeout errors by calling CanTp_TimeoutHandling() function
 **
 ** \param[in] ChannelPtr  pointer to channel data structure to simplify lookups
 */
STATIC FUNC(void, CANTP_CODE) CanTp_PostponedTimeoutHandling
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */
/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_START_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)
FUNC(void, CANTP_CODE) CanTp_HandlePostponedFlags
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
)
{

  boolean PostponedFlags = FALSE;

  do
  {
    CanTp_PostponedTimeoutHandling(ChannelPtr);

    CANTP_ENTER_CRITICAL_SECTION();

    if(ChannelPtr->PostFlag)
    {
      PostponedFlags = TRUE;
    }
    else
    {
      /* No postponed tasks left */
      PostponedFlags = FALSE;
    }

    CANTP_LEAVE_CRITICAL_SECTION();

    if(PostponedFlags)
    {
      /* Process Tx confirmation before Rx indication.
         This shall avoid that Frame is rejected because channel is not in the
         proper state (waiting for the pending Tx confirmation).
       */
      /* lock channel */
      CANTP_LOCK_CHANNEL(ChannelPtr);

      CanTp_PostTxConfirmation(ChannelPtr);

      /* unlock channel */
      CANTP_UNLOCK_CHANNEL(ChannelPtr);
#if(CANTP_STALLHANDLING_SUPPORT == STD_ON)
      CanTp_PostRxIndication(ChannelPtr);
#endif
      CanTp_PostSTminCallback(ChannelPtr);
    }
  } while(PostponedFlags); /* Loop until all postponed tasks are done */
}

STATIC FUNC(void, CANTP_CODE) CanTp_PostponedTimeoutHandling
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
)
{
  CanTp_ChStateType ChannelState;
  boolean TimeoutDetected = FALSE;

  TS_AtomicAssign8(ChannelState, ChannelPtr->State);

  /* Check if timeout occurred during channel lock */
  if(ChannelState != CANTP_CH_IDLE)
  {
    CANTP_ENTER_CRITICAL_SECTION();
    /* if post ticks are available */
    if(ChannelPtr->PostTicks > 0U)
    {
      /* shutdown channel, if Postticks already exceeded Ticks */
      if(ChannelPtr->PostTicks >= ChannelPtr->Ticks)
      {
        ChannelPtr->Ticks = 0U;
        /* Timeout detected */
        TimeoutDetected = TRUE;
      }
      else
      {
        /* count down the Postticks */
        ChannelPtr->Ticks -= ChannelPtr->PostTicks;
        ChannelPtr->PostTicks = 0U;
      }
    }
    CANTP_LEAVE_CRITICAL_SECTION();
  }

  if(TimeoutDetected)
  {
    /* Perform timeout task if required. */
    CanTp_TimeoutHandling(ChannelPtr);
  }
}
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
