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

FUNC(Com_StatusType, COM_CODE) Com_GetStatus
(
   void
)
{

   Com_StatusType statusRetVal = COM_UNINIT;

   DBG_COM_GETSTATUS_ENTRY();

   switch(Com_InitStatus)
   {
   case COM_EB_UNINIT:
   {
      statusRetVal = COM_UNINIT;
   }
   break;
   case COM_EB_INIT:
   {
      statusRetVal = COM_INIT;
   }
   break;
   /* CHECK: NOPARSE */
   default:
   {
       COM_UNREACHABLE_CODE_ASSERT(COMServiceId_GetStatus);
   }
   break;
   /* CHECK: PARSE */
   }

   DBG_COM_GETSTATUS_EXIT(statusRetVal);
   return statusRetVal;
}

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
