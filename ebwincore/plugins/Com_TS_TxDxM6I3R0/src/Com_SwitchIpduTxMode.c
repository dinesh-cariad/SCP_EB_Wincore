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

#if (COM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>
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
/** \brief Send an IPDU
 * The function triggers sets the transmission mode of the I-PDU referenced by PduId to Mode.
 * Preconditions:
 * - The COM must be initialized
 * - should not be mixed with signal based TMS
 * \param[in] PduId - Id of the I-PDU of which the transmission mode shall be changed
 * \param[in] Mode - the transmission mode that shall be set
 */
FUNC(void, COM_CODE) Com_SwitchIpduTxMode
(
   PduIdType PduId,
   boolean Mode
)
{

   DBG_COM_SWITCHIPDUTXMODE_ENTRY(PduId,Mode);

#if (COM_TM_AVAILABLE == STD_ON)
#if (COM_DEV_ERROR_DETECT == STD_ON)

   /* check whether Com is initialized */
   if (Com_InitStatus == COM_EB_UNINIT)
   {
      COM_DET_REPORTERROR(COM_E_UNINIT, COMServiceId_SwitchIpduTxMode);
   }
   /* check if PduId is valid */
   else if (Com_gConfigPtr->ComTxIPduArraySize <= PduId)
   {
      COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_SwitchIpduTxMode);
   }
   /* check the range of Mode */
   else if ( (Mode != TRUE) && (Mode != FALSE) )
   {
      COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_SwitchIpduTxMode);
   }
   else
#endif /* COM_DEV_ERROR_DETECT */

   {

#if ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) && (COM_DEV_ERROR_DETECT == STD_ON) )

     CONSTP2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
         COM_GET_CONFIG_ELEMENT(ComTxIPduType, Com_gConfigPtr->ComTxIPduRef, PduId);

     if( TRUE == COM_GET_COM_TX_IPDU_DUMMY(IPduPtr) )
     {
       COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_SwitchIpduTxMode);
     }
     else
#endif /* ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) && (COM_DEV_ERROR_DETECT == STD_ON) ) */
     {

       CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) PduStatusFlagsPtr =
           &Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + PduId)];

       uint8 PduStatusByte;
       uint8 CurrentTM;

       /* enter the critical section for calculation the new TM */
       Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0();

       /* get the status byte of the Pdu */
       PduStatusByte = *PduStatusFlagsPtr;

       /* get the current TMS status of Pdu */
       TS_GetBit(&PduStatusByte, COM_TXPDU_TMSSTATE, uint8, CurrentTM);

       if (Mode != CurrentTM)
       {
         /* change the TM of the I-Pdu */
         if (Mode == TRUE)
         {
           Com_SetBitUInt8Var(PduStatusByte, COM_TXPDU_TMSSTATE);
         }
         else
         {
           Com_ClearBitUInt8Var(PduStatusByte, COM_TXPDU_TMSSTATE);
         }

         /* Set the changed flag of the I-Pdu */
         Com_SetBitUInt8Var(PduStatusByte, COM_TXPDU_TMSCHANGED);

         /* save the status byte again */
         *PduStatusFlagsPtr = PduStatusByte;

       }                         /* if (Mode != CurrentTM) */

       Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0();

     }

   }

#else
   TS_PARAM_UNUSED(PduId);
   TS_PARAM_UNUSED(Mode);
#endif /* (COM_TM_AVAILABLE == STD_ON) */

   DBG_COM_SWITCHIPDUTXMODE_EXIT(PduId,Mode);
}                               /* Com_SwitchIpduTxMode */

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
