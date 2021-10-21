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
#include <Com_Core_TxIPdu.h>    /* Header TxIPdu functions */

#if (COM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>
#endif

#if (COM_TX_TPAPI_ENABLE == STD_ON)
#include <EcuC.h>
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */

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
 ** The function triggers sending of an IPDU
 ** Preconditions:
 ** - The COM must be initialized
 ** - The function must only be called from callouts
 ** \param[in] ComTxPduId ID of the PDU to be sent
 */
FUNC(void, COM_CODE) Com_TriggerIPDUSend
(
   PduIdType ComTxPduId
)
{
    DBG_COM_TRIGGERIPDUSEND_ENTRY(ComTxPduId);

#if (COM_DEV_ERROR_DETECT == STD_ON)

    /* check whether Com is initialized */
    if (Com_InitStatus == COM_EB_UNINIT)
    {
        COM_DET_REPORTERROR(COM_E_UNINIT, COMServiceId_TriggerIPDUSend);
    }
    /* check if ComTxPduId is valid */
    else if (Com_gConfigPtr->ComTxIPduArraySize <= ComTxPduId)
    {
        COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_TriggerIPDUSend);
    }
    else

#endif /* COM_DEV_ERROR_DETECT */

    {

#if ( ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) && (COM_DEV_ERROR_DETECT == STD_ON) ) \
      || (COM_DEFERTX2MAINFUNC == STD_OFF) )

      CONSTP2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
          COM_GET_CONFIG_ELEMENT(ComTxIPduType, Com_gConfigPtr->ComTxIPduRef, ComTxPduId);

#endif /* ( ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) && (COM_DEV_ERROR_DETECT == STD_ON) ) \
      || (COM_DEFERTX2MAINFUNC == STD_OFF) ) */

#if ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) && (COM_DEV_ERROR_DETECT == STD_ON) )

      if( TRUE == COM_GET_COM_TX_IPDU_DUMMY(IPduPtr) )
      {
        COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_TriggerIPDUSend);
      }
      else
#endif /* ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) && (COM_DEV_ERROR_DETECT == STD_ON) ) */
      {

#if (COM_DEFERTX2MAINFUNC == STD_OFF)
        /* If the PDU cannot be sent now (because the minimum delay time has not
         * passed by yet)... */
#if (COM_MDT_QUEUE_AVAILABLE == STD_ON)
        if ( Com_IsBitSet_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + ComTxPduId)],
            COM_TXPDU_MDTQUEUED) )
        {
          /* If Mdt-queued, schedule for sending asap. */
          COM_AtomicSetBit_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + ComTxPduId)],
              COM_TXPDU_SENDIMMEDIATELY);
        }
        else
#endif /* (COM_MDT_QUEUE_AVAILABLE == STD_ON) */
        {
          /* send the Pdu out */
          Com_EB_Send_Pdu((ComIPduRefType)ComTxPduId, IPduPtr);
        }
#else
        /* defer each send request into main function */
        COM_AtomicSetBit_8(&Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + ComTxPduId)],
            COM_TXPDU_SENDIMMEDIATELY);
#endif /* (COM_DEFERTX2MAINFUNC == STD_OFF) */

      }

    }

    DBG_COM_TRIGGERIPDUSEND_EXIT(ComTxPduId);
}                               /* Com_TriggerIPDUSend */

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
