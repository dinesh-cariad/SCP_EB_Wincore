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

/*==================[inclusions]============================================*/

#include <CanSM_Trace.h>
#include <TSAutosar.h>              /* EB specific standard types */
#include <CanSM_Cfg.h>              /* DET configuration */
#include <CanSM_Api.h>              /* function declarations */
#include <ComM.h>                   /* Communication Manager types */
#include <CanSM_Dcm.h>              /* Module API prototype */
#include <CanSM_Internal.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#if (CANSM_SET_BAUDRATE_API == STD_ON)

#define CANSM_START_SEC_CODE
#include <CanSM_MemMap.h>

FUNC(Std_ReturnType, CANSM_CODE) CanSM_SetBaudrate
(
  NetworkHandleType  network,
  uint16 BaudRateConfigID
)
{
  Std_ReturnType retval = E_NOT_OK;

  DBG_CANSM_SETBAUDRATE_ENTRY(network,BaudRateConfigID);

  /*SWS_CanSM_00530*/
  if ( CanSM_GlobalInit == CANSM_GLOBAL_UNINIT )
  {
#if (CANSM_DEV_ERROR_DETECT == STD_ON)
    CANSM_DET(CANSM_SID_SETBAUDRATE, CANSM_E_UNINIT);
#endif
  }
  /*SWS_CanSM_00502*/
  else if ( ! (CANSM_NW_HANDLE_IS_VALID(network)) )
  {
#if (CANSM_DEV_ERROR_DETECT == STD_ON)
    CANSM_DET(CANSM_SID_SETBAUDRATE, CANSM_E_INVALID_NETWORK_HANDLE);
#endif
  }
  else
  {
    const CanSM_NetworkIndexType nw = CANSM_NW_IDX_FROM_HANDLE(network);

    /* decide if change baudrate request can be done
     *
     * Check and set TransitionOngoing flag in critical sections to avoid side effects with
     * other functions also updating the internal state (RequestComMode, TxTimeoutException,
     * MainFunction).
     */
    CANSM_ENTER_CRITICAL_SECTION();
    {
      /* caches if a transition is already ongoing */
      boolean TransitionOngoing = CANSM_NW_INFO_TRANSITION_ONGOING(nw);

      /* CanSM_SetBaudrate is only accepted in state NO_BUS_OFF */
      if ( (TransitionOngoing == FALSE)
           &&
           (CANSM_S_FUCO_NO_BUS_OFF == CANSM_NW_INFO_INTERNALSTATE(nw))
         )
      {
        /* protect the synchronous transition against other API functions */
        CANSM_NW_INFO_TRANSITION_ONGOING(nw) = TRUE;
        CANSM_NW_INFO_BAUDRATEID(nw) = BaudRateConfigID;
        retval = E_OK;
      }
    }
    CANSM_EXIT_CRITICAL_SECTION();

    if (retval == E_OK)
    {
      /* do synchronous transition now */
      CanSM_DoTransitionSequence (nw, CANSM_T_FUCO_CHBR, FALSE);
      /* synchronous part of transition done, unlock protection */
      TS_AtomicAssign8(CANSM_NW_INFO_TRANSITION_ONGOING(nw), FALSE);
    }
  }

  DBG_CANSM_SETBAUDRATE_EXIT(retval,network,BaudRateConfigID);
  return retval;
} /* CanSM_SetBaudrate */



#define CANSM_STOP_SEC_CODE
#include <CanSM_MemMap.h>

#endif /* CANSM_SET_BAUDRATE_API */

/*==================[end of file]===========================================*/

