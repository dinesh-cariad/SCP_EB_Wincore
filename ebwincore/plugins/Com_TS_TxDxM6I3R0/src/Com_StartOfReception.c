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

/** \brief Com_StartOfReception
 * returns the pointer to the size of the AUTOSAR COM module's internal
 * receive buffer for the I-PDU with ID ComRxPduId.
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * - the Pdu must be started before
 * \param[in] ComRxPduId - ID of Tp I-PDU to be received.
 * \param[in] TpSduLength - complete length of the TP I-PDU to be received.
 * \param[out] RxBufferSizePtr - Pointer to the size of internal TP-receive buffer
 * \retval BUFREQ_OK - Connection has been accepted. RxBufferSizePtr indicates the
 *                     available receive buffer.
 * \retval BUFREQ_E_NOT_OK - Connection has been rejected. RxBufferSizePtr remains
 *                           unchanged.
 * \retval BUFREQ_E_OVFL - In case the configured buffer size as specified via
 *                         ComPduIdRef.PduLength is smaller than TpSduLength.
 * \retval BUFREQ_E_BUSY - In case the reception buffer is actually not available for
 *                        a new reception (implementation specific).
 */
FUNC(BufReq_ReturnType, COM_CODE) Com_StartOfReception
(
   PduIdType ComRxPduId,
   PduLengthType TpSduLength,
   P2VAR(PduLengthType, AUTOMATIC, COM_APPL_DATA) RxBufferSizePtr
)
{
  BufReq_ReturnType RetVal = BUFREQ_E_NOT_OK;
  DBG_COM_STARTOFRECEPTION_ENTRY(ComRxPduId,TpSduLength,RxBufferSizePtr);

#if (COM_RX_TPAPI_ENABLE == STD_ON)

#if (COM_DEV_ERROR_DETECT == STD_ON)

  /* check whether Com is initialized */
  if (Com_InitStatus == COM_EB_UNINIT)
  {
     COM_DET_REPORTERROR(COM_E_UNINIT, COMServiceId_StartOfReception);
  }
  /* Check if the PduId is within the range of the ComTxIPduArraySize */
  else if (Com_gConfigPtr->ComRxIPduArraySize <= ComRxPduId)
  {
     COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_StartOfReception);
  }
  else if (E_NOT_OK == EcuC_StartOfReceptionDet(TpSduLength, RxBufferSizePtr))
  {
     COM_DET_REPORTERROR(COM_E_PARAM_POINTER, COMServiceId_StartOfReception);
  }
  else
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */
  {

    /* pointer to the configuration of the IPdu to which the signal belongs */
    CONSTP2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr =
        COM_GET_CONFIG_ELEMENT(ComRxIPduType, Com_gConfigPtr->ComRxIPduRef, ComRxPduId);

#if (COM_DEV_ERROR_DETECT == STD_ON)
      if (IPduPtr->ComRxIpduTPControlInfoRef == COM_IPDU_ARRAY_SIZE_MAX)
      {
        /* enters also for dummy pdu, isvalidconfig check ensure array size max value */
        COM_DET_REPORTERROR(COM_E_PARAM, COMServiceId_StartOfReception);
      }
      else
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */
      {

        PduInfoType pduinfo;

        /* get TP Control array of struct base */
        CONSTP2VAR(EcuC_RxControlInfoType, AUTOMATIC, COM_VAR_CLEARED) TpRxContPtrBase =
            COM_GET_MEMPTR(EcuC_RxControlInfoType, Com_gConfigPtr->ComRxIpduTPControlInfoBase);

        /* get TP Control struct value pointer */
        CONSTP2VAR(EcuC_RxControlInfoType, AUTOMATIC, COM_VAR_CLEARED) TpRxContPtrValPtr =
            &TpRxContPtrBase[IPduPtr->ComRxIpduTPControlInfoRef];

        /* pointer to byte with status flags of the Pdu */
        CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) IPduFlagBytePtr =
            &Com_gDataMemPtr[(Com_gConfigPtr->ComRxIPduStatusBase + ComRxPduId)];

        PduLengthType RxBufferSizeLocal = *RxBufferSizePtr;

        CONSTP2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) pduShadowDataPtr =
            (P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED))
            & Com_gDataMemPtr[((IPduPtr->ComIPduValueRef) + (IPduPtr->ComIPduSizeMax))];

        /* IPdu is started */
        if( Com_IsBitSet_8(IPduFlagBytePtr, COM_RXPDU_STARTED) )
        {

          pduinfo.SduDataPtr = pduShadowDataPtr;
          pduinfo.SduLength = ((PduLengthType) IPduPtr->ComIPduSizeMax);

          /* initialize TP Control struct before initiating an TP Rx session */
          EcuC_InitRx(TpRxContPtrValPtr);

          RetVal = EcuC_StartOfReception(
              TpRxContPtrValPtr,
              &pduinfo,
              TpSduLength,
              &RxBufferSizeLocal
          );

          if(RetVal == BUFREQ_OK)
          {
            *RxBufferSizePtr = RxBufferSizeLocal;
          }

        }
        else
        {
          /* COM721 */
          RetVal = BUFREQ_E_NOT_OK;
        }

      }

  }

#else
  TS_PARAM_UNUSED(ComRxPduId);
  TS_PARAM_UNUSED(TpSduLength);
  TS_PARAM_UNUSED(RxBufferSizePtr);
#endif /* (COM_RX_TPAPI_ENABLE == STD_ON) */

  DBG_COM_STARTOFRECEPTION_EXIT(RetVal,ComRxPduId,TpSduLength,RxBufferSizePtr);
  return RetVal;
}

#if (TS_MERGED_COMPILE == STD_OFF)
#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>
#endif


/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
