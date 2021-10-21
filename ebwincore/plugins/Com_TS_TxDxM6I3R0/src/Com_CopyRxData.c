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

#if (COM_RX_TPAPI_ENABLE == STD_ON)
#include <EcuC.h>
#endif /* (COM_RX_TPAPI_ENABLE == STD_ON) */

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

/** \brief Com_CopyRxData
 * Called once upon reception of each segment. Within this call, the received
 * data is copied to the receive TP buffer. The function must only be called
 * if the connection has been accepted by an initial call to Com_StartOfReception.
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * - the Pdu must be started before
 * \param[in] PduId - ID of Tp I-PDU to be transmitted
 * \param[in] PduInfoPointer - Pointer to a PduInfoType, which indicates the number
 *            of bytes to be copied (SduLength) and the location of the source
 *            data (SduDataPtr).
 *            An SduLength of 0 is possible in order to poll the available receive
 *            buffer size. In this case no data are to be copied and PduInfoPtr
 *            might be invalid.
 * \param[out] RxBufferSizePtr - Remaining receive buffer after successful completion of this call
 *             (Com_CopyRxData returns BUFREQ_OK otherwise out put parameter RxBufferSizePtr does
 *             not change).
 * \retval BUFREQ_OK - Data has been copied to the receive buffer completely as requested.
 * \retval BUFREQ_E_BUSY - The receive buffer is actually not available (implementation specific).
 * \retval BUFREQ_E_NOT_OK - Data has not been copied. Request failed.
 */
FUNC(BufReq_ReturnType, COM_CODE) Com_CopyRxData
(
   PduIdType PduId,
   P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPointer,
   P2VAR(PduLengthType, AUTOMATIC, COM_APPL_DATA) RxBufferSizePtr
)
{

  BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;
  DBG_COM_COPYRXDATA_ENTRY(PduId,PduInfoPointer,RxBufferSizePtr);

#if (COM_RX_TPAPI_ENABLE == STD_ON)

#if (COM_DEV_ERROR_DETECT == STD_ON)

  /* check whether Com is initialized */
  if (Com_InitStatus == COM_EB_UNINIT)
  {
     COM_DET_REPORTERROR(COM_E_UNINIT, COMServiceId_CopyRxData);
  }
  /* Check if the PduId is within the range of the ComTxIPduArraySize */
  else if (Com_gConfigPtr->ComRxIPduArraySize <= PduId)
  {
     COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_CopyRxData);
  }
  else if (E_NOT_OK == EcuC_CopyRxDataDet(PduInfoPointer, RxBufferSizePtr) )
  {
     COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COMServiceId_CopyRxData);
  }
  else
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */
  {
    PduInfoType pduinfo;

    CONSTP2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
        COM_GET_CONFIG_ELEMENT(ComRxIPduType, Com_gConfigPtr->ComRxIPduRef, PduId);

    PduLengthType rxBufferSizeLocal = *RxBufferSizePtr;

#if (COM_DEV_ERROR_DETECT == STD_ON)
    if (IPduPtr->ComRxIpduTPControlInfoRef == COM_IPDU_ARRAY_SIZE_MAX)
    {
      /* enters also for dummy pdu, isvalidconfig check ensure array size max value */
      COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_CopyRxData);
    }
    else
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */
    {

      ComIPduSizeType pdulength = IPduPtr->ComIPduSizeMax;

      CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr =
          &Com_gDataMemPtr[(Com_gConfigPtr->ComRxIPduStatusBase + PduId)];

      /* get TP Control array of struct base */
      CONSTP2VAR(EcuC_RxControlInfoType, AUTOMATIC, COM_VAR_CLEARED) TpRxContPtrBase =
          COM_GET_MEMPTR(EcuC_RxControlInfoType, Com_gConfigPtr->ComRxIpduTPControlInfoBase);

      /* get TP Control struct value pointer */
      CONSTP2VAR(EcuC_RxControlInfoType, AUTOMATIC, COM_VAR_CLEARED) TpRxContPtrValPtr =
          &TpRxContPtrBase[IPduPtr->ComRxIpduTPControlInfoRef];

      /* set data pointer to Rx Tp shadow Pdu buffer */
      CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) pduShadowDataPtr =
          (P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED))
          & Com_gDataMemPtr[((IPduPtr->ComIPduValueRef) + (IPduPtr->ComIPduSizeMax))];

      /* IPdu is started */
      if( Com_IsBitSet_8(IPduFlagBytePtr, COM_RXPDU_STARTED) )
      {

        pduinfo.SduDataPtr = pduShadowDataPtr;
        /* length info actually not used -> was informed through Com_StartOfReception */
        COM_AtomicAssign_PduLengthType(pduinfo.SduLength, pdulength);

        RetVal = EcuC_CopyRxData
            (
                TpRxContPtrValPtr,
                &pduinfo,
                PduInfoPointer,
                &rxBufferSizeLocal
            );

        if(RetVal == BUFREQ_OK)
        {
          /* assign local var to out param */
          *RxBufferSizePtr = rxBufferSizeLocal;
        }

      }
      else
      {
        /* COM782 */
        RetVal = BUFREQ_E_NOT_OK;
      }

    }

  }
#else
  TS_PARAM_UNUSED(PduId);
  TS_PARAM_UNUSED(PduInfoPointer);
  TS_PARAM_UNUSED(RxBufferSizePtr);
#endif /* (COM_RX_TPAPI_ENABLE == STD_ON) */

  DBG_COM_COPYRXDATA_EXIT(RetVal,PduId,PduInfoPointer,RxBufferSizePtr);
  return RetVal;
}

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif


/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
