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
 * \brief Com_SetIpduGroup -  sets the value of a bit in an I-PDU group vector.
 *
 * Preconditions:
 * - COM must be initialized
 *
 * \param[in] ipduGroupVector - I-PDU group vector to be modified
 * \param[in] ipduGroupId - identifies the corresponding bit in the I-PDU
 * group vector
 * \param[in] bitval - new value of the corresponding bit
 *
 * \ServiceID{0x1d}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
FUNC(void, COM_CODE)  Com_SetIpduGroup
(
   Com_IpduGroupVector ipduGroupVector,
   Com_IpduGroupIdType ipduGroupId,
   boolean bitval
)
{

    DBG_COM_SETIPDUGROUP_ENTRY(ipduGroupVector,ipduGroupId,bitval);

#if (COM_DEV_ERROR_DETECT == STD_ON)

    /* check whether COM is initialized */

    if (Com_InitStatus == COM_EB_UNINIT)
    {
        COM_DET_REPORTERROR(COM_E_UNINIT, COMServiceId_SetIpduGroup);
    }
   /* check ipduGroupVector */
    else if (ipduGroupVector == NULL_PTR)
    {
        COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COMServiceId_SetIpduGroup);
    }
   /* Check if the ipduGroupId is valid */
    else if (Com_gConfigPtr->ComIPduGroupsNum <= ipduGroupId)
    {
        COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_SetIpduGroup);
    }
    else

#endif /* COM_DEV_ERROR_DETECT */

    {
        if (0U == bitval)
        {
            /* clear the bit */
            Com_ClearBitUInt8(&ipduGroupVector[ipduGroupId/ 8U], ipduGroupId % 8U);
        }
        else
        {
            /* set the bit */
            Com_SetBitUInt8(&ipduGroupVector[ipduGroupId/ 8U], ipduGroupId % 8U);
        }
    }

    DBG_COM_SETIPDUGROUP_EXIT(ipduGroupVector,ipduGroupId,bitval);
}

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
