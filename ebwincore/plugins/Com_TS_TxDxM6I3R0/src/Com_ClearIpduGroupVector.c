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
 * \brief Com_ClearIpduGroupVector - sets all bits of the given
 * Com_IpduGroupVector to 0.
 *
 * \param[in] ipduGroupVector - I-PDU group vector to be cleared
 *
 * \ServiceID{0x1c}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */
FUNC(void, COM_CODE)  Com_ClearIpduGroupVector
(
   Com_IpduGroupVector ipduGroupVector
)
{
   DBG_COM_CLEARIPDUGROUPVECTOR_ENTRY(ipduGroupVector);
#if (COM_DEV_ERROR_DETECT == STD_ON)

   /* check whether COM is initialized */
   if (Com_InitStatus == COM_EB_UNINIT)
   {
       COM_DET_REPORTERROR(COM_E_UNINIT, COMServiceId_ClearIpduGroupVector);
   }
   /* check ipduGroupVector */
   else if (ipduGroupVector == NULL_PTR)
   {
       COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COMServiceId_ClearIpduGroupVector);
   }
   else

#endif /* COM_DEV_ERROR_DETECT */

   {
       uint16 iPduGroupVectorByte;
       /* loop over all bytes of the vector */
       for (iPduGroupVectorByte = 0U;
             iPduGroupVectorByte < COM_IPDU_GROUP_VECTOR_NUM_BYTES; iPduGroupVectorByte++)
       {
          ipduGroupVector[iPduGroupVectorByte] = 0U;
       }
   }

   DBG_COM_CLEARIPDUGROUPVECTOR_EXIT(ipduGroupVector);
}

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
