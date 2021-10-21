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
 * \brief Com_ReceptionDMControl - enables or disables Rx I-PDU
 * Deadline Monitoring
 *
 * Function to enables/disables every RxDM of every Rx-IPdu according to the
 * passed states of the ComIpduGroups in the parameter ipduGroupVector.
 * Preconditions:
 * - COM must be initialized
 *
 * \param[in] ipduGroupVector - I-PDU group vector containing the activation
 * of RxDM (disable = 0/ enable = 1) for all Rx-Pdus.
 *
 * \ServiceID{0x06}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
FUNC(void, COM_CODE)  Com_ReceptionDMControl
(
   Com_IpduGroupVector ipduGroupVector
)
{
    DBG_COM_RECEPTIONDMCONTROL_ENTRY(ipduGroupVector);

#if (COM_DEV_ERROR_DETECT == STD_ON)

    /* check whether COM is initialized */

    if (Com_InitStatus == COM_EB_UNINIT)
    {
        COM_DET_REPORTERROR(COM_E_UNINIT, COMServiceId_ReceptionDMControl);
    }
    /* check ipduGroupVector */
    else if (ipduGroupVector == NULL_PTR)
    {
        COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COMServiceId_ReceptionDMControl);
    }
    /* check if the vector has valid value */
    else if (E_OK != Com_EB_CheckIpduGroupVector(ipduGroupVector))
    {
        COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_ReceptionDMControl);
    }
    else

#endif /* COM_DEV_ERROR_DETECT */

    {
        Com_EB_HandlePduGroupRx(COM_IPDUGROUP_RXDM_CONTROL, ipduGroupVector, FALSE);
    }

    DBG_COM_RECEPTIONDMCONTROL_EXIT(ipduGroupVector);
}                               /* Com_ReceptionDMControl */

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
