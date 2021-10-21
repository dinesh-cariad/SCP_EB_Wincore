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

/* MISRA-C:2012 Deviation List
 *
 *
 * MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 * A conversion should not be performed from pointer to
 * void into pointer to object.
 *
 * Reason:
 * The memory routines are optimized for dealing with aligned
 * memory sections.
 *
 */

/*==================[inclusions]=============================================*/

#include <Com_Trace.h>
#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR standard types */
#include <Com_Rules.h>          /* Generated configuration + derived rules */
#include <Com_Api_Static.h>     /* module public static API */
#include <Com_Priv.h>           /* public intra module API */

#include <Com_Core_TxIPdu.h>    /* Header TxIPdu functions */
#include <Com_Core_RxIPdu.h>    /* Header RxIPdu functions */

#if (COM_PBCFGM_SUPPORT_ENABLED == STD_ON)
#include <PbcfgM_Api.h>             /* Post build configuration manager */
#endif /* (COM_PBCFGM_SUPPORT_ENABLED == STD_ON) */


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


/* start code section declaration */
#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_START_SEC_CODE
#include <Com_MemMap.h>
#endif

/** \brief Com_Init - initialize the COM
 * function to initialize the COM; checks the configuration and sets
 * everything back to start positions
 * \param[in] config - pointer to configurations structure
 * \remarks This function stops all PDU-groups, re-initializes the send-
 * and minimum-delay-time-"queues", and sets the initial values for all
 ** internal and external signals.
 */
FUNC(void, COM_CODE) Com_Init
(
   P2CONST(Com_ConfigType, AUTOMATIC,COM_APPL_CONST) Com_ConfigPtr
)
{

   ComIPduRefType pdu;

   /*
    * local configuration pointer
    */
   P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST) TmpCfgPtr =
         /*
          * check for constant configuration address
          */
#if (COM_CONST_CFG_ADDRESS_ENABLE == STD_OFF)
         Com_ConfigPtr;

   DBG_COM_INIT_ENTRY(Com_ConfigPtr);

#if (COM_PBCFGM_SUPPORT_ENABLED == STD_ON)

   /* If the initialization function is called with an null pointer get the configuration from the
    * post build configuration manager */
   if (TmpCfgPtr == NULL_PTR)
   {
      PbcfgM_ModuleConfigPtrType PbcfgMConfigPtr = NULL_PTR;

      (void)PbcfgM_GetConfig(
            COM_MODULE_ID,
            0U,
            &PbcfgMConfigPtr);

      TmpCfgPtr = (P2CONST(Com_ConfigType, AUTOMATIC, COM_APPL_CONST)) PbcfgMConfigPtr;

   }

#endif /* (COM_PBCFGM_SUPPORT_ENABLED == STD_ON) */

#else /* COM_CONST_CFG_ADDRESS_ENABLE == STD_OFF */
         Com_gConfigPtr;

   DBG_COM_INIT_ENTRY(Com_ConfigPtr);

   TS_PARAM_UNUSED(Com_ConfigPtr);
#endif /* COM_CONST_CFG_ADDRESS_ENABLE */

#if (COM_DEV_ERROR_DETECT == STD_ON)

   /* check that configuration pointer is valid */
   if (E_OK != Com_IsValidConfig(TmpCfgPtr))
   {
      COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COMServiceId_Init);
   }
   else

#endif /* COM_DEV_ERROR_DETECT */

   {

#if (COM_CONST_CFG_ADDRESS_ENABLE == STD_OFF)
      /* save configuration pointer */
      Com_gConfigPtr = TmpCfgPtr;
#endif /* COM_CONST_CONFIG_ADDRESS */

      /* Initialize Rx-Pdus */
      for (pdu = 0; (pdu < TmpCfgPtr->ComRxIPduArraySize); pdu++)
      {
         Com_EB_InitRxIPdu(pdu);
      }

#if ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) && (COM_TMS_ENABLE == STD_ON) )
      /* Initialize dummy ComTxSignalGroup flag bytes */
      {
        ComIPduRefType SignalGId;

        for(SignalGId = 0U; SignalGId < TmpCfgPtr->ComTxSignalGroupArraySize; SignalGId++)
        {
          Com_EB_ClearTxSGTMSState(SignalGId);
        }
      }
#endif /* ( (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) && (COM_TMS_ENABLE == STD_ON) ) */

      /* Initialize Tx-Pdus */
      for (pdu = 0; (pdu < TmpCfgPtr->ComTxIPduArraySize); pdu++)
      {
         Com_EB_InitTxIPdu(pdu);
      }

#if (COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE == STD_ON)
      {
          ComRAMIdxType i;
          ComRAMIdxType numBytes =
                 (((ComRAMIdxType)Com_gConfigPtr->ComCbkTxAckDeferredListMax + 7U) / 8U);

         /* Check for deferred confirmations */
         for (i = Com_gConfigPtr->ComCbkTxAckDeferredStatusBase;
               i < (Com_gConfigPtr->ComCbkTxAckDeferredStatusBase + numBytes);
               i++)
         {
            /* clear the byte */
            Com_gDataMemPtr[i] = 0U;
         }
      }
#endif /* (COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE == STD_ON) */

#if (COM_SIGNAL_GW_ENABLE == STD_ON)
      {
         ComRAMIdxType ii;
         /* Deviation MISRAC2012-1 <+2> */
         CONSTP2VAR(ComSignalGroupRefType, AUTOMATIC, COM_VAR_CLEARED) TxSignalGroupQueueBase =
                  COM_GET_MEMPTR(ComSignalGroupRefType, Com_gConfigPtr->ComSigGWTxSignalGroupQueueBase);

        /* reset queue */
        for (ii = 0U; ii < Com_gConfigPtr->ComSigGWTxSignalGroupQueueMax; ii++)
        {
           /* clear the byte */
           TxSignalGroupQueueBase[ii] = COM_SIGNAL_GROUP_INVALID;
        }
      }
#endif
      Com_InitStatus = COM_EB_INIT;
   }

   DBG_COM_INIT_EXIT(Com_ConfigPtr);
}                               /* Com_Init */


/* stop code section declaration */
#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif


/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/

