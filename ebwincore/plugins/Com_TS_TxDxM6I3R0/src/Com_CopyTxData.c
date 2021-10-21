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

/*==================[inclusions]============================================*/

#include <Com_Trace.h>
#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR Standard types */
#include <Com_Api_Static.h>     /* Module public API prototypes */

#include <Com_Priv.h>           /* Module private API */

#if (COM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                /* Det API */
#endif

#if (COM_TX_TPAPI_ENABLE == STD_ON)
#include <EcuC.h>
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_START_SEC_CODE
#include <Com_MemMap.h>
#endif

/** \brief Com_CopyTxData
 * function which copy the requested transmit data of the large IPDU
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * - the Pdu must be started before
 * \param[in] PduId - ID of Tp I-PDU to be transmitted
 * \param[in] PduInfoPtr - Pointer to a PduInfoType, which indicates the number
 *            of bytes to be copied (SduLength) and the location where the data
 *            have to be copied to (SduDataPtr). An SduLength of 0 is possible in
 *            order to poll the available transmit data count. In this case no
 *            data are to be copied and SduDataPtr might be invalid.
 * \param[in] RetryInfoPtr - The COM module ignores the value of this pointer, since
 *            it always keeps the complete buffer until the transmission of a large
 *            I-PDU is either confirmed or aborted.
 * \param[out] TxDataCntPtr - Remaining Tx data after successful completion of this call
 *             (Com_CopyTxData returns BUFREQ_OK otherwise out put parameter TxDataCntPtr
 *             does not change).
 * \retval BUFREQ_OK - Data has been copied to the transmit buffer completely as requested.
 * \retval BUFREQ_E_BUSY - The transmission buffer is actually not available (implementation specific).
 * \retval BUFREQ_E_NOT_OK - Data has not been copied. Request failed, in case the corresponding I-PDU was stopped.
 */
FUNC(BufReq_ReturnType, COM_CODE) Com_CopyTxData
(
   PduIdType PduId,
   P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr,
   P2VAR(RetryInfoType, AUTOMATIC, COM_APPL_DATA) RetryInfoPtr,
   P2VAR(PduLengthType, AUTOMATIC, COM_APPL_DATA) TxDataCntPtr
)
{

  BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;
  DBG_COM_COPYTXDATA_ENTRY(PduId,PduInfoPtr,RetryInfoPtr,TxDataCntPtr);

#if (COM_TX_TPAPI_ENABLE == STD_ON)

#if (COM_DEV_ERROR_DETECT == STD_ON)

  /* check whether Com is initialized */
  if (Com_InitStatus == COM_EB_UNINIT)
  {
     COM_DET_REPORTERROR(COM_E_UNINIT, COMServiceId_CopyTxData);
  }
  /* Check if the PduId is within the range of the ComTxIPduArraySize */
  else if (Com_gConfigPtr->ComTxIPduArraySize <= PduId)
  {
     COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_CopyTxData);
  }
  else if (PduInfoPtr == NULL_PTR)
  {
     COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COMServiceId_CopyTxData);
  }
  else if (TxDataCntPtr == NULL_PTR)
  {
     COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COMServiceId_CopyTxData);
  }
  else

#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */

  {

    CONSTP2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
        COM_GET_CONFIG_ELEMENT(ComTxIPduType, Com_gConfigPtr->ComTxIPduRef, PduId);

    CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr =
        &Com_gDataMemPtr[(Com_gConfigPtr->ComTxIPduStatusBase + PduId)];

#if (COM_DEV_ERROR_DETECT == STD_ON)
    if (IPduPtr->ComTxIpduTPControlInfoRef == COM_IPDU_ARRAY_SIZE_MAX)
    {
      /* enters also for dummy pdu, isvalidconfig check ensure array size max value */
      COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_CopyTxData);
    }
    else
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */
    {

      /* ref to TP Control struct exists for this IPdu &&
       * IPdu is started */
      if( Com_IsBitSet_8(IPduFlagBytePtr, COM_TXPDU_STARTED) )
      {
        /* get TP Control array of struct base */
        CONSTP2VAR(EcuC_TxControlInfoType, AUTOMATIC, COM_VAR_CLEARED) TpTxContPtrBase =
            COM_GET_MEMPTR(EcuC_TxControlInfoType, Com_gConfigPtr->ComTxIpduTPControlInfoBase);

        /* get TP Control struct value pointer */
        CONSTP2VAR(EcuC_TxControlInfoType, AUTOMATIC, COM_VAR_CLEARED) TpTxContPtrValPtr =
            &TpTxContPtrBase[IPduPtr->ComTxIpduTPControlInfoRef];

        PduLengthType TxDataCntLocal = *TxDataCntPtr;

        PduInfoType pduinfo;

        CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) pdudataptr =
            (P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED))
            & Com_gDataMemPtr[(IPduPtr->ComIPduValueRef)];

        pduinfo.SduDataPtr = pdudataptr;
        COM_AtomicAssign_PduLengthType(pduinfo.SduLength, COM_GET_TX_IPDU_SIZE(PduId,IPduPtr));

        /* call EcuC_CopyTxData
         * expected retval:
         * BUFREQ_OK: copied data
         * BUFREQ_E_BUSY: number of bytes to copy is greater than remaining length
         * BUFREQ_E_NOT_OK: no copy operation was done
         */
        RetVal = EcuC_CopyTxData(
            TpTxContPtrValPtr,
            &pduinfo,
            PduInfoPtr,
            RetryInfoPtr,
            &TxDataCntLocal);

        if(RetVal == BUFREQ_OK)
        {
          /* assign local var to out param */
          *TxDataCntPtr = TxDataCntLocal;
        }
      }
      else
      {
        /* COM783 */
        RetVal = BUFREQ_E_NOT_OK;
      }

    }
  }
#else
  TS_PARAM_UNUSED(PduId);
  TS_PARAM_UNUSED(PduInfoPtr);
  TS_PARAM_UNUSED(RetryInfoPtr);
  TS_PARAM_UNUSED(TxDataCntPtr);
#endif /* (COM_TX_TPAPI_ENABLE == STD_ON) */

  DBG_COM_COPYTXDATA_EXIT(RetVal,PduId,PduInfoPtr,RetryInfoPtr,TxDataCntPtr);
  return RetVal;
}

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
