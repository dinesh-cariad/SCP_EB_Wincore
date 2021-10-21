/**
 * \file
 *
 * \brief AUTOSAR Com
 *
 * This file contains the implementation of the AUTOSAR
 * module Com.
 *
 * \version 6.3.50
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */


/*==================[inclusions]=============================================*/

#include <Com_Trace.h>
#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR standard types */
#include <Com_Api_Static.h>     /* module public static API */
#include <Com_Priv.h>           /* module internal stuff */
#include <TSMem.h>              /* TS_MemCpy */

#if (COM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                /* Det API */
#endif

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_START_SEC_CODE
#include <Com_MemMap.h>
#endif

FUNC(void, COM_CODE) Com_ReceiveShadowSignal
(
   Com_SignalIdType SignalId,
   P2VAR(void, AUTOMATIC, COM_APPL_DATA) SignalDataPtr
)
{
   DBG_COM_RECEIVESHADOWSIGNAL_ENTRY(SignalId,SignalDataPtr);

#if ((COM_GENERATED_RCV_SIG_ENABLE == STD_ON) && \
(COM_MAP_RECEIVE_SIGNAL == COM_RECEIVE_SIGNAL_GENERATED))

   (void)Com_ReceiveSignalGeneric(SignalId, SignalDataPtr);

#else

   (void)Com_ReceiveSignal(SignalId, SignalDataPtr);

#endif /* #if ((COM_GENERATED_RCV_SIG_ENABLE == STD_ON) && \
(COM_MAP_RECEIVE_SIGNAL == COM_RECEIVE_SIGNAL_GENERATED)) */

   DBG_COM_RECEIVESHADOWSIGNAL_EXIT(SignalId,SignalDataPtr);
}

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/

