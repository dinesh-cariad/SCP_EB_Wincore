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

#include <Com_Core_IPduGroup.h>    /* Header IPduGroup functions */

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

/**
 * \brief Com_IpduGroupControl - starts/stops  I-PDU
 *
 * Function to start/stop every Rx-Ipdu and Tx-Ipdu according to the passed
 * states of the ComIpduGroups in the parameter ipduGroupVector.
 * Preconditions:
 * - COM must be initialized
 *
 * \param[in] ipduGroupVector - I-PDU group vector containing the activation
 * state (stopped = 0/ started = 1) for all IPdus.
 * \param[in] Initialize - flag to request initialization of the I-PDUs which
 * are newly started
 */
FUNC(void, COM_CODE) Com_IpduGroupControl
(
   Com_IpduGroupVector ipduGroupVector,
   boolean Initialize
)
{
   DBG_COM_IPDUGROUPCONTROL_ENTRY(ipduGroupVector,Initialize);
#if (COM_DEV_ERROR_DETECT == STD_ON)

   /* check whether COM is initialized */
   if (Com_InitStatus == COM_EB_UNINIT)
   {
      COM_DET_REPORTERROR(COM_E_UNINIT, COMServiceId_IpduGroupControl);
   }
   /* check ipduGroupVector */
   else if (ipduGroupVector == NULL_PTR)
   {
      COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COMServiceId_IpduGroupControl);
   }
   /* check if the vector has valid value */
   else if (E_OK != Com_EB_CheckIpduGroupVector(ipduGroupVector))
   {
      COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_IpduGroupControl);
   }
   else

#endif /* COM_DEV_ERROR_DETECT */

   {
      Com_EB_HandlePduGroupRx(COM_IPDUGROUP_CONTROL, ipduGroupVector, Initialize);
      Com_EB_HandlePduGroupTx(ipduGroupVector, Initialize);
   }

   DBG_COM_IPDUGROUPCONTROL_EXIT(ipduGroupVector,Initialize);
}                               /* Com_IpduGroupControl */

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
