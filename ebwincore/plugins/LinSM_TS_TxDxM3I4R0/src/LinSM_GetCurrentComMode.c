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

/*==================[inclusions]============================================*/

#include <LinSM_Trace.h>
#include <LinSM_Internal.h>

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
#define LINSM_START_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */
#endif /* TS_MERGED_COMPILE */

FUNC(Std_ReturnType, LINSM_CODE) LinSM_GetCurrentComMode
(
  NetworkHandleType  network,
  P2VAR(ComM_ModeType, AUTOMATIC, LINSM_APPL_DATA) mode
)
{
  LinSM_ChannelIdType id;
  Std_ReturnType retval = E_NOT_OK;

  DBG_LINSM_GETCURRENTCOMMODE_ENTRY(network,mode);

#if (LINSM_DEV_ERROR_DETECT == STD_ON)
  if ( LinSM_GlobalState == LINSM_GLOBAL_UNINIT )
  {
    LINSM_DET(LINSM_SID_GETCURRENTCOMMODE, LINSM_E_UNINIT);
    retval = E_NOT_OK;
  }

  else if ( mode == NULL_PTR )
  {
    LINSM_DET(LINSM_SID_GETCURRENTCOMMODE, LINSM_E_PARAMETER_POINTER);
    retval = E_NOT_OK;
  }
  else
#endif

  {
#if (LINSM_DEV_ERROR_DETECT == STD_ON)
    if ( ! (LINSM_COMM_NW_IS_VALID(network)) )
    {
      LINSM_DET(LINSM_SID_GETCURRENTCOMMODE, LINSM_E_NONEXISTENT_NETWORK);
      retval = E_NOT_OK;
    }
    else
#endif
    {
      uint8 channel_state;
      id = LINSM_COMM_NW_TO_ID(network);
      retval = E_OK;
      TS_AtomicAssign8(channel_state, LINSM_CHANNEL_STATE(id));
      switch (channel_state)
      {
        case LINSM_FULL_COM:
        case LINSM_GOTO_SLEEP:
          *mode = COMM_FULL_COMMUNICATION;
          break;

        case LINSM_NO_COM:
        case LINSM_WAKEUP:
          *mode = COMM_NO_COMMUNICATION;
          break;
        /* CHECK: NOPARSE */

        /* Unreachable */
        default: /* default case intended to be empty */
#if (LINSM_DEV_ERROR_DETECT == STD_ON)
          LINSM_UNREACHABLE_CODE_ASSERT(LINSM_SID_GETCURRENTCOMMODE);
#endif
          break;
        /* CHECK: PARSE */
      }
    }
  }

  DBG_LINSM_GETCURRENTCOMMODE_EXIT(retval,network,mode);
  return retval;
}

#if (TS_MERGED_COMPILE == STD_OFF)
#define LINSM_STOP_SEC_CODE
#include <LinSM_MemMap.h> /* !LINKSTO LinSM.ASR40.LINSM014,1 */
#endif /* TS_MERGED_COMPILE */

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/

