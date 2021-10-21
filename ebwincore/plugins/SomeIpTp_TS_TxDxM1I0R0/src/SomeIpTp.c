/**
 * \file
 *
 * \brief AUTOSAR SomeIpTp
 *
 * This file contains the implementation of the AUTOSAR
 * module SomeIpTp.
 *
 * \version 1.0.28
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */


/* tasking Deviation List
 *
 * TASKING-1) Deviated Rule: W549
 * W549: condition is always true
 *
 * Reason:
 * False-positive from the Tasking v5.0r2 compiler (TCVX-41885)
 */

/*==================[inclusions]============================================*/

#include <TSAutosar.h>        /* Specific standard types */
#include <TSMem.h>            /* TS_MemCpy(), TS_MemSet() */
#include <SomeIpTp.h>         /* Module public API */
#include <SomeIpTp_Cbk.h>     /* Module call back public API */
#include <SomeIpTp_Types.h>   /* Module specific types */
#include <SomeIpTp_Trace.h>   /* Debug and trace */
#include <SomeIpTp_Version.h> /* Module version declarations */
#include <SchM_SomeIpTp.h>    /* SchM interface header */
#include <PduR_SomeIpTp.h>    /* PduR public Api for SomeIpTp */

/*==================[version check]=========================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef SOMEIPTP_VENDOR_ID /* configuration check */
#error SOMEIPTP_VENDOR_ID must be defined
#endif

#if (SOMEIPTP_VENDOR_ID != 1U) /* vendor check */
#error SOMEIPTP_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef SOMEIPTP_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error SOMEIPTP_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (SOMEIPTP_AR_RELEASE_MAJOR_VERSION != 4U)
#error SOMEIPTP_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef SOMEIPTP_AR_RELEASE_MINOR_VERSION /* configuration check */
#error SOMEIPTP_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (SOMEIPTP_AR_RELEASE_MINOR_VERSION != 3U )
#error SOMEIPTP_AR_RELEASE_MINOR_VERSION wrong (!= 3U)
#endif

#ifndef SOMEIPTP_AR_RELEASE_REVISION_VERSION /* configuration check */
#error SOMEIPTP_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (SOMEIPTP_AR_RELEASE_REVISION_VERSION != 0U )
#error SOMEIPTP_AR_RELEASE_REVISION_VERSION wrong (!= 0U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef SOMEIPTP_SW_MAJOR_VERSION /* configuration check */
#error SOMEIPTP_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (SOMEIPTP_SW_MAJOR_VERSION != 1U)
#error SOMEIPTP_SW_MAJOR_VERSION wrong (!= 1U)
#endif

#ifndef SOMEIPTP_SW_MINOR_VERSION /* configuration check */
#error SOMEIPTP_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (SOMEIPTP_SW_MINOR_VERSION < 0U)
#error SOMEIPTP_SW_MINOR_VERSION wrong (< 0U)
#endif

#ifndef SOMEIPTP_SW_PATCH_VERSION /* configuration check */
#error SOMEIPTP_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (SOMEIPTP_SW_PATCH_VERSION < 28U)
#error SOMEIPTP_SW_PATCH_VERSION wrong (< 28U)
#endif

/*==================[macros]================================================*/

#if(defined SOMEIPTP_INITIALIZED)
#error SOMEIPTP_INITIALIZED already defined!
#endif /* #if(defined SOMEIPTP_INITIALIZED)*/
#define SOMEIPTP_INITIALIZED TRUE

#if(defined SOMEIPTP_NOT_INITIALIZED)
#error SOMEIPTP_NOT_INITIALIZED already defined!
#endif /* #if(defined SOMEIPTP_NOT_INITIALIZED)*/
#define SOMEIPTP_NOT_INITIALIZED FALSE

#if(defined SOMEIPTP_MSG_TYPE_INDEX)
#error SOMEIPTP_MSG_TYPE_INDEX already defined!
#endif /* #if(defined SOMEIPTP_MSG_TYPE_INDEX)*/
#define SOMEIPTP_MSG_TYPE_INDEX 6U

#if(defined SOMEIPTP_HEADER_INDEX)
#error SOMEIPTP_HEADER_INDEX already defined!
#endif /* #if(defined SOMEIPTP_HEADER_INDEX)*/
#define SOMEIPTP_HEADER_INDEX 8U

#if(defined SOMEIPTP_SET_TP_FLAG)
#error SOMEIPTP_SET_TP_FLAG already defined!
#endif /* #if(defined SOMEIPTP_SET_TP_FLAG)*/
#define SOMEIPTP_SET_TP_FLAG(u8msgType) (u8msgType)|=(1U<<5U)

#if(defined SOMEIPTP_CLEAR_TP_FLAG)
#error SOMEIPTP_CLEAR_TP_FLAG already defined!
#endif /* #if(defined SOMEIPTP_CLEAR_TP_FLAG)*/
#define SOMEIPTP_CLEAR_TP_FLAG(u8msgType) (u8msgType)&=(~(1U<<5U))

#if(defined SOMEIPTP_GET_TP_FLAG)
#error SOMEIPTP_GET_TP_FLAG already defined!
#endif /* #if(defined SOMEIPTP_GET_TP_FLAG)*/
#define SOMEIPTP_GET_TP_FLAG(u8msgType) (((u8msgType)&(1U<<5U))>>5U)

#if(defined SOMEIPTP_GET_MORE_FLAG)
#error SOMEIPTP_GET_MORE_FLAG already defined!
#endif /* #if(SOMEIPTP_GET_MORE_FLAG)*/
#define SOMEIPTP_GET_MORE_FLAG(u8ptr) (((u8ptr)[11U])&0x01U)

#if(defined SOMEIP_HEADER_LENGTH)
#error SOMEIP_HEADER_LENGTH already defined!
#endif /* #if(defined SOMEIP_HEADER_LENGTH)*/
#define SOMEIP_HEADER_LENGTH 8U

#if(defined SOMEIPTP_HEADER_LENGTH)
#error SOMEIPTP_HEADER_LENGTH already defined!
#endif /* #if(defined SOMEIPTP_HEADER_LENGTH)*/
#define SOMEIPTP_HEADER_LENGTH 12U

#if(defined SOMEIPTP_RES_M_LENGTH)
#error SOMEIPTP_RES_M_LENGTH already defined!
#endif /* #if(defined SOMEIPTP_RES_M_LENGTH)*/
#define SOMEIPTP_RES_M_LENGTH 4U

#if(defined SOMEIPTP_PDULENGTH_DIVIDE)
#error SOMEIPTP_PDULENGTH_DIVIDE already defined!
#endif /* #if(SOMEIPTP_PDULENGTH_DIVIDE)*/
#define SOMEIPTP_PDULENGTH_DIVIDE(a,b) (PduLengthType)((PduLengthType)(a)/(PduLengthType)(b))

#if(defined SOMEIPTP_PDULENGTH_MODULUS)
#error SOMEIPTP_PDULENGTH_MODULUS already defined!
#endif /* #if(SOMEIPTP_PDULENGTH_MODULUS)*/
#define SOMEIPTP_PDULENGTH_MODULUS(a,b) (PduLengthType)((PduLengthType)(a)%(PduLengthType)(b))

#if(defined SOMEIPTP_GET_OFFSET)
#error SOMEIPTP_GET_OFFSET already defined!
#endif /* #if(SOMEIPTP_GET_OFFSET)*/
#define SOMEIPTP_GET_OFFSET(u8ptr) ((PduLengthType)(((uint32)((u8ptr)[SOMEIP_HEADER_LENGTH])<<24U) | \
                                             ((uint32)((u8ptr)[SOMEIP_HEADER_LENGTH+1])<<16U) | \
                                             ((uint32)((u8ptr)[SOMEIP_HEADER_LENGTH+2])<<8U) | \
                                             ((uint32)((u8ptr)[SOMEIP_HEADER_LENGTH+3])))>>4U)

#if(defined SOMEIPTP_CALC_OFFSET)
#error SOMEIPTP_CALC_OFFSET already defined!
#endif /* #if(SOMEIPTP_CALC_OFFSET) */
#define SOMEIPTP_CALC_OFFSET(length) ((PduLengthType)(length)>>4U) /* Calculate offset as number of received 16 bytes block data */

#if(defined SOMEIPTP_PAYLOAD_MODULUS_BY16)
#error SOMEIPTP_PAYLOAD_MODULUS_BY16 already defined!
#endif /* #if(SOMEIPTP_PAYLOAD_MODULUS_BY16)*/
#define SOMEIPTP_PAYLOAD_MODULUS_BY16(length) ((PduLengthType)(length)%16U)

#if(defined SOMEIPTP_FIRST_BYTLE_BIG)
#error SOMEIPTP_FIRST_BYTLE_BIG already defined!
#endif /* #if(SOMEIPTP_FIRST_BYTLE_BIG)*/
#define SOMEIPTP_FIRST_BYTLE_BIG(b) ((uint8)((uint32)(b)>>24U))

#if(defined SOMEIPTP_SECOND_BYTLE_BIG)
#error SOMEIPTP_SECOND_BYTLE_BIG already defined!
#endif /* #if(SOMEIPTP_SECOND_BYTLE_BIG)*/
#define SOMEIPTP_SECOND_BYTLE_BIG(b) ((uint8)(((uint32)(b)>>16U) & 0x000000FFU))

#if(defined SOMEIPTP_THIRD_BYTLE_BIG)
#error SOMEIPTP_THIRD_BYTLE_BIG already defined!
#endif /* #if(SOMEIPTP_THIRD_BYTLE_BIG)*/
#define SOMEIPTP_THIRD_BYTLE_BIG(b) ((uint8)(((uint32)(b)>>8U)  & 0x000000FFU))

#if(defined SOMEIPTP_FOURTH_BYTLE_BIG)
#error SOMEIPTP_FOURTH_BYTLE_BIG already defined!
#endif /* #if(SOMEIPTP_FOURTH_BYTLE_BIG)*/
#define SOMEIPTP_FOURTH_BYTLE_BIG(b) ((uint8)((uint32)(b) & 0x000000FFU))

#if(defined SOMEIPTP_TWO_SEGMENTS)
#error SOMEIPTP_TWO_SEGMENTS already defined!
#endif /* #if(SOMEIPTP_TWO_SEGMENTS)*/
#define SOMEIPTP_TWO_SEGMENTS        2U

#if (defined SOMEIPTP_CHECK_IF_TXCONF_STATE)
#error SOMEIPTP_CHECK_IF_TXCONF_STATE is already defined.
#endif
/** \brief Checks if TxConfirmationTimeout should be checked
 **
 **
 ** \param[in] state - current ongoing state
 */
#define SOMEIPTP_CHECK_IF_TXCONF_STATE(state)           \
        (                                               \
              ( (state) == SOMEIPTP_SINGLE_CONF )         \
           || ( (state) == SOMEIPTP_FIRST_CONF )          \
           || ( (state) == SOMEIPTP_CONSECUTIVE_CONF )    \
           || ( (state) == SOMEIPTP_LAST_CONF )           \
        )

#define SOMEIPTP_CHECK_IF_INIT_STATE_USED           \
        (                                           \
             (SOMEIPTP_NO_OF_TXNSDUS > 0U )         \
          || (SOMEIPTP_NO_OF_RXNSDUS > 0U )         \
          || (SOMEIPTP_DEV_ERROR_DETECT == STD_ON)  \
        )

/*==================[internal function declarations]=======================*/

#define SOMEIPTP_START_SEC_CODE
#include <SomeIpTp_MemMap.h>

#if(SOMEIPTP_NO_OF_TXNSDUS > 0U )
/** \brief SomeIpTp_ResetTxState()
 **
 ** This function reset the state buffer of TxNsdu
 **
 ** \param[in] TxPduId  Identifier of the Tx PDU.
 */
STATIC FUNC(void, SOMEIPTP_CODE) SomeIpTp_ResetTxState
(
  PduIdType TxPduId
);

/** \brief SomeIpTp_TransmitSingleSegment()
 ** This function processes transmission of the single segment:
 **
 ** - Call PduR_SomeIpTpCopyTxData() to copy single segment data to PduInfoPtr->SduDataPtr.
 **
 ** Preconditions:
 ** - PDU ID must be in state SOMEIPTP_SINGLE.
 **
 ** \param[in] TxPduId  Identifier of the Tx PDU.
 ** \param[inout] PduInfoPtr - pointer to a buffer to where the SDU data shall be copied,
 **                 and the available buffer size in SduLengh.
 **                 On return, the service will indicate the length of
 **                 the copied SDU data in SduLength.
 **               SkipWithoutInterruptionFlag - pointer to boolean flag reporting if errors happened
 **                 due to unexpected behaviour inside the function.
 **                   TRUE means no errors
 **                   FALSE means error happened
 */
STATIC FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_TransmitSingleSegment
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr,
  P2VAR(boolean, AUTOMATIC, SOMEIPTP_APPL_DATA) SkipWithoutInterruptionFlag
);

/** \brief SomeIpTp_TransmitFirstSegment()
 ** This function processes transmission of the first segment:
 **
 ** - Call PduR_SomeIpTpCopyTxData() to copy Tx Header to PduInfoPtr->SduDataPtr.
 ** - Call PduR_SomeIpTpCopyTxData() to copy Tx data to PduInfoPtr->SduDataPtr.
 ** - Set Tp-flag.
 ** - Create SomeIpTp header.
 **
 ** Preconditions:
 ** - PDU ID must be in state SOMEIPTP_FIRST.
 **
 ** \param[in] TxPduId  Identifier of the Tx PDU.
 ** \param[inout] PduInfoPtr - pointer to a buffer to where the SDU data shall be copied,
 **                 and the available buffer size in SduLengh.
 **                 On return, the service will indicate the length of
 **                 the copied SDU data in SduLength.
 **               SkipWithoutInterruptionFlag - pointer to boolean flag reporting if errors happened
 **                 due to unexpected behaviour inside the function.
 **                   TRUE means no errors
 **                   FALSE means error happened
 */
STATIC FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_TransmitFirstSegment
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr,
  P2VAR(boolean, AUTOMATIC, SOMEIPTP_APPL_DATA) SkipWithoutInterruptionFlag
);

/** \brief SomeIpTp_TransmitConsecutiveSegment()
 ** This function processes transmission of the Consecutive segment:
 **
 ** - Call PduR_SomeIpTpCopyTxData() to copy Tx data to PduInfoPtr->SduDataPtr.
 ** - Set Header from that of first segment.
 ** - Set Tp-flag.
 ** - Create SomeIpTp header.
 **
 ** Preconditions:
 ** - PDU ID must be in state SOMEIPTP_CONSECUTIVE.
 **
 ** \param[in] TxPduId  Identifier of the Tx PDU.
 ** \param[inout] PduInfoPtr - pointer to a buffer to where the SDU data shall be copied,
 **                 and the available buffer size in SduLengh.
 **                 On return, the service will indicate the length of
 **                 the copied SDU data in SduLength.
 **               SkipWithoutInterruptionFlag - pointer to boolean flag reporting if errors happened
 **                 due to unexpected behaviour inside the function.
 **                   TRUE means no errors
 **                   FALSE means error happened
 */
STATIC FUNC(boolean, SOMEIPTP_CODE) SomeIpTp_TransmitConsecutiveSegment
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr,
  P2VAR(boolean, AUTOMATIC, SOMEIPTP_APPL_DATA) SkipWithoutInterruptionFlag
);

/** \brief SomeIpTp_TransmitLastSegment()
 ** This function processes transmission of the Last segment:
 **
 ** - Call PduR_SomeIpTpCopyTxData() to copy Tx data to PduInfoPtr->SduDataPtr.
 ** - Set Header from that of first segment.
 ** - Set Tp-flag.
 ** - Create SomeIpTp header.
 **
 ** Preconditions:
 ** - PDU ID must be in state SOMEIPTP_LAST.
 **
 ** \param[in] TxPduId  Identifier of the Tx PDU.
 ** \param[inout] PduInfoPtr - pointer to a buffer to where the SDU data shall be copied,
 **                 and the available buffer size in SduLengh.
 **                 On return, the service will indicate the length of
 **                 the copied SDU data in SduLength.
 **               SkipWithoutInterruptionFlag - pointer to boolean flag reporting if errors happened
 **                 due to unexpected behaviour inside the function.
 **                   TRUE means no errors
 **                   FALSE means error happened
 */
STATIC FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_TransmitLastSegment
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr,
  P2VAR(boolean, AUTOMATIC, SOMEIPTP_APPL_DATA) SkipWithoutInterruptionFlag
);
#endif /* SOMEIPTP_NO_OF_TXNSDUS > 0U  */

#if(SOMEIPTP_NO_OF_RXNSDUS > 0U )
/** \brief SomeIpTp_InterruptAssemblyProcess()
 **
 ** This function Interrupt the Assembly process
 **
 ** Preconditions:
 ** - Rx timeout time defined by SomeIpTpRxTimeoutTime expires.
 ** - or API SomeIpTp_RxIndication() is called with Offset > 0 while no ongoing session is running.
 ** - or Module detects an inconsistency of the received SOME/IP TP headers.
 ** - or API SomeIpTp_RxIndication() is called with Tp-flag = 0 while ongoing session is running.
 ** - or The received payload bytes are not dividable by 16 for all but last.
 ** - or The Offset Value in units of 16 bytes does not match to the sum of the received
 **   Payload bytes of the previous SOME/IP segments.
 ** - or The received Offset Value equals '0' while the received Payload bytes of the previous
 **   SOME/IP segments is greater than '0'.
 ** - or The bufferSizePtr provided by the API PduR_SomeIpTpStartOfReception() or
 **   PduR_SomeIpTpCopyRxData() is smaller than the sum of the received.
 ** - or API PduR_SomeIpTpCopyRxData() or PduR_SomeIpTpStartOfReception() returns something else
 **   than BUFREQ_OK.
 **
 ** \param[in] RxPduId  Identifier of the Rx PDU.
 ** \param[in] ApiId  Service Identifier of the API which calls this function.
 ** \param[in] ErrorId  Error Identifier of the case that leads to an error.
 */
STATIC FUNC(void, SOMEIPTP_CODE) SomeIpTp_InterruptAssemblyProcess
(
  PduIdType RxPduId,
  uint8 ApiId,
  uint8 ErrorId
);

/** \brief SomeIpTp_ResetRxState()
 **
 ** This function reset the state buffer of RxNsdu
 **
 ** \param[in] RxPduId  Identifier of the Rx PDU.
 */
STATIC FUNC(void, SOMEIPTP_CODE) SomeIpTp_ResetRxState
(
  PduIdType RxPduId
);

/** \brief SomeIpTp_ReceiveSingleMessage()
 **
 ** This function processes a received Single segment:
 ** - Call PduR_SomeIpTpStartOfReception to start reception.
 ** - Call PduR_SomeIpTpCopyRxData() to copy received data.
 ** - Call PduR_SomeIpTpRxIndication() to notify that the data has been received successfully.
 **
 ** Preconditions:
 ** - PDU ID must be in state SOMEIPTP_IDLE.
 **
 ** \param[in] RxPduId  Identifier of the Rx PDU.
 ** \param[in] PduInfoPtr  Contains the length (SduLength) of the received PDU, a pointer to
 **            a buffer (SduDataPtr) containing the PDU, and the MetaData related to this PDU.
 */
STATIC FUNC(SomeIpTp_ErrorType, SOMEIPTP_CODE) SomeIpTp_ReceiveSingleMessage
(
  PduIdType RxPduId,
  P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_CONST) PduInfoPtr
);

/** \brief SomeIpTp_ReceiveFirstSegment()
 **
 ** This function processes a received first segment:
 ** - Check that the received payload is dividable by 16.
 ** - Store SOME\IP Header.
 ** - Clear Tpflag.
 ** - Start RxTimeout.
 ** - Call PduR_SomeIpTpStartOfReception to start reception.
 ** - Call PduR_SomeIpTpCopyRxData() to copy received data.
 **
 ** Preconditions:
 ** - PDU ID must be in state SOMEIPTP_IDLE.
 **
 ** \param[in] RxPduId  Identifier of the Rx PDU.
 ** \param[in] PduInfoPtr  Contains the length (SduLength) of the received PDU, a pointer to
 **            a buffer (SduDataPtr) containing the PDU, and the MetaData related to this PDU.
 */
STATIC FUNC(SomeIpTp_ErrorType, SOMEIPTP_CODE) SomeIpTp_ReceiveFirstSegment
(
  PduIdType RxPduId,
  P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_CONST) PduInfoPtr
);

/** \brief SomeIpTp_ReceiveConsecutiveOrLastSegment()
 **
 ** This function processes a received Consecutive or last segment:
 ** - Check Header consistency with the one for the first segment received.
 ** - Restart RxTimeout.
 ** - Check that the received payload is dividable by 16.
 ** - Check that the received offset is equal to the calculated offset.
 ** - Check that AvailBufLength from last call of CopyRxData is larger or equal to the received payload.
 ** - Call PduR_SomeIpTpCopyRxData() to copy received data.
 ** - If last segment is received, Call PduR_SomeIpTpRxIndication() to notify that the data has been received successfully.
 **
 ** Preconditions:
 ** - PDU ID must be in state SOMEIPTP_RX_STARTED.
 **
 ** \param[in] RxPduId  Identifier of the Rx PDU.
 ** \param[in] PduInfoPtr  Contains the length (SduLength) of the received PDU, a pointer to
 **            a buffer (SduDataPtr) containing the PDU, and the MetaData related to this PDU.
 */
 STATIC FUNC(SomeIpTp_ErrorType, SOMEIPTP_CODE) SomeIpTp_ReceiveConsecutiveOrLastSegment
(
  PduIdType RxPduId,
  P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_CONST) PduInfoPtr,
  uint8 More
);
#endif /* SOMEIPTP_NO_OF_RXNSDUS > 0U  */

#define SOMEIPTP_STOP_SEC_CODE
#include <SomeIpTp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#define SOMEIPTP_START_SEC_VAR_INIT_8
#include <SomeIpTp_MemMap.h>

#if(SOMEIPTP_CHECK_IF_INIT_STATE_USED)
/** \brief Variable to indicate that module was initialized. */
STATIC VAR(boolean, SOMEIPTP_VAR) SomeIpTp_InitState = SOMEIPTP_NOT_INITIALIZED;
#endif /* SOMEIPTP_CHECK_IF_INIT_STATE_USED */

#define SOMEIPTP_STOP_SEC_VAR_INIT_8
#include <SomeIpTp_MemMap.h>

/*==================[external function definitions]=========================*/
#define SOMEIPTP_START_SEC_CODE
#include <SomeIpTp_MemMap.h>

/* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00044,1 */
#if(SOMEIPTP_VERSION_INFO_API == STD_ON)
FUNC(void, SOMEIPTP_CODE) SomeIpTp_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) VersionInfo
)
{
  DBG_SOMEIPTP_GETVERSIONINFO_ENTRY(VersionInfo);

#if(SOMEIPTP_DEV_ERROR_DETECT == STD_ON)
  /* check if the VersionInfo pointer is a Null Pointer */
  /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00066,1 */
  if(NULL_PTR == VersionInfo)
  {
    SOMEIPTP_DET_REPORT_ERROR(SOMEIPTP_SID_GETVERSIONINFO, SOMEIPTP_E_PARAM_POINTER);
  }
  else
#endif /* SOMEIPTP_DEV_ERROR_DETECT == STD_ON */
  {
    /* Return version information */
    VersionInfo->vendorID = (uint16)SOMEIPTP_VENDOR_ID;
    VersionInfo->moduleID = (uint16)SOMEIPTP_MODULE_ID;
    VersionInfo->sw_major_version = (uint8)SOMEIPTP_SW_MAJOR_VERSION;
    VersionInfo->sw_minor_version = (uint8)SOMEIPTP_SW_MINOR_VERSION;
    VersionInfo->sw_patch_version = (uint8)SOMEIPTP_SW_PATCH_VERSION;
  }

  DBG_SOMEIPTP_GETVERSIONINFO_EXIT(VersionInfo);
}
#endif /* SOMEIPTP_VERSION_INFO_API */

/* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00046,1 */
FUNC(void, SOMEIPTP_CODE) SomeIpTp_Init
(
  P2CONST(SomeIpTp_ConfigType, AUTOMATIC, SOMEIPTP_APPL_CONST) config
)
{
#if(SOMEIPTP_NO_OF_TXNSDUS > 0U )
  PduIdType txNsduId = 0U;
#endif /* SOMEIPTP_NO_OF_TXNSDUS > 0U  */

#if(SOMEIPTP_NO_OF_RXNSDUS > 0U )
  PduIdType rxNsduId = 0U;
#endif /* SOMEIPTP_NO_OF_RXNSDUS > 0U  */

  DBG_SOMEIPTP_INIT_ENTRY(config);

  TS_PARAM_UNUSED(config);

#if(SOMEIPTP_NO_OF_TXNSDUS > 0U )
  for(txNsduId=0U;
      /* Deviation TASKING-1 */
      txNsduId<SOMEIPTP_NO_OF_TXNSDUS;
      txNsduId++)
  {
    SomeIpTp_ResetTxState(txNsduId);
  }
#endif /* SOMEIPTP_NO_OF_TXNSDUS > 0U  */

#if(SOMEIPTP_NO_OF_RXNSDUS > 0U )
  for(rxNsduId=0U;
      /* Deviation TASKING-1 */
      rxNsduId<SOMEIPTP_NO_OF_RXNSDUS;
      rxNsduId++)
  {
    SomeIpTp_ResetRxState(rxNsduId);
  }
#endif /* SOMEIPTP_NO_OF_RXNSDUS > 0U  */

#if(SOMEIPTP_CHECK_IF_INIT_STATE_USED)
  SomeIpTp_InitState = SOMEIPTP_INITIALIZED;
#endif /* SOMEIPTP_CHECK_IF_INIT_STATE_USED */

  DBG_SOMEIPTP_INIT_EXIT(config);
}

/* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00058,1 */
FUNC(void, SOMEIPTP_CODE) SomeIpTp_MainFunctionTx
(
  void
)
{
#if(SOMEIPTP_NO_OF_TXNSDUS > 0U )
  PduIdType txNsduId = 0U;
  PduInfoType pduInfo;
  Std_ReturnType RetVal_PduRSomeIpTpTrans = E_OK;
  boolean reportError = FALSE;
#endif /* SOMEIPTP_NO_OF_TXNSDUS > 0U  */

  DBG_SOMEIPTP_MAINFUNCTIONTX_ENTRY();

#if(SOMEIPTP_NO_OF_TXNSDUS > 0U )
  /* check if the module was initialized */
  /* If not initialized return */
  if(SOMEIPTP_INITIALIZED == SomeIpTp_InitState)
  {
    /* Start the MainFunctionTx */
    for(txNsduId=0U;
        /* Deviation TASKING-1 */
        txNsduId<SOMEIPTP_NO_OF_TXNSDUS;
        txNsduId++)
    {
      pduInfo.SduDataPtr = NULL_PTR;
      RetVal_PduRSomeIpTpTrans = E_OK;
      reportError = FALSE;
      switch(SomeIpTp_TxNSduState[txNsduId].ongoingState)
      {
        case SOMEIPTP_SINGLE_TO_BE_TRANSMITTED:
        {
          /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00017.First_Segment_Size_Calc,1 */
          pduInfo.SduLength = SomeIpTp_TxNSduState[txNsduId].lastSegLength;
          SomeIpTp_TxNSduState[txNsduId].ongoingState = SOMEIPTP_SINGLE;
          /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00017.Pdu_ID,1 */
          /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00017.call_PduR_SomeIpTpTransmit,1 */
          RetVal_PduRSomeIpTpTrans = PduR_SomeIpTpTransmit(SomeIpTp_TxNsduElements[txNsduId].NPduTxId, &pduInfo);
          break;
        }
        case SOMEIPTP_FIRST_TO_BE_TRANSMITTED:
        {
          /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00017.First_Segment_Size_Calc,1 */
          pduInfo.SduLength = SomeIpTp_TxNsduElements[txNsduId].MaxNpduLength;
          SomeIpTp_TxNSduState[txNsduId].ongoingState = SOMEIPTP_FIRST;
          /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00017.Pdu_ID,1 */
          /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00017.call_PduR_SomeIpTpTransmit,1 */
          RetVal_PduRSomeIpTpTrans = PduR_SomeIpTpTransmit(SomeIpTp_TxNsduElements[txNsduId].NPduTxId, &pduInfo);
          break;
        }
        case SOMEIPTP_NEXT_TO_BE_CONSECUTIVE:
        {
          /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00020,1 */
          if(0U == SomeIpTp_TxNSduState[txNsduId].txSeparationMFC)
          {
            pduInfo.SduLength = SomeIpTp_TxNsduElements[txNsduId].MaxNpduLength;
            SomeIpTp_TxNSduState[txNsduId].ongoingState = SOMEIPTP_CONSECUTIVE;
            RetVal_PduRSomeIpTpTrans = PduR_SomeIpTpTransmit(SomeIpTp_TxNsduElements[txNsduId].NPduTxId, &pduInfo);
          }
          else
          {
            SomeIpTp_TxNSduState[txNsduId].txSeparationMFC--;
          }
          break;
        }
        case SOMEIPTP_NEXT_TO_BE_LAST:
        {
          /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00020,1 */
          if(0U == SomeIpTp_TxNSduState[txNsduId].txSeparationMFC)
          {
            pduInfo.SduLength = SomeIpTp_TxNSduState[txNsduId].lastSegLength + SOMEIPTP_HEADER_LENGTH;
            SomeIpTp_TxNSduState[txNsduId].ongoingState = SOMEIPTP_LAST;
            RetVal_PduRSomeIpTpTrans = PduR_SomeIpTpTransmit(SomeIpTp_TxNsduElements[txNsduId].NPduTxId, &pduInfo);
          }
          else
          {
            SomeIpTp_TxNSduState[txNsduId].txSeparationMFC--;
          }
          break;
        }
        default:
        {
          SchM_Enter_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();

          if(TRUE == SOMEIPTP_CHECK_IF_TXCONF_STATE(SomeIpTp_TxNSduState[txNsduId].ongoingState))
          {
            /* Check if the TxConfirmation timeout has expired */
            /* !LINKSTO SomeIpTp.EB.SomeIpTpTxConfirmationTimeout.Monitor,1 */
            if(SomeIpTp_TxNSduState[txNsduId].txConfTimeoutMFC == 0U )
            {
              /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00023.SomeIpTpTxConfirmationTimeout.StopAssembly,1 */
              reportError = TRUE;
            }
            else
            {
              SomeIpTp_TxNSduState[txNsduId].txConfTimeoutMFC--;
            }
          }

          SchM_Exit_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();

          break;
        }
      }

      /* Report error in case the PduR_SomeIpTpTransmit() returned E_NOT_OK */
      if(E_OK != RetVal_PduRSomeIpTpTrans)
      {
        SchM_Enter_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();

        /* Check if no API have reset the ongoing state to Idle, called the Tx confirmation and reported Det_ReportRuntimeError.
        This API can be SomeIpTp_TriggerTransmit() or SomeIpTp_Transmit(). */
        /* Get sure that the ongoing state is not idle due to SomeIpTp_Transmit() call, since SomeIpTp_Transmit() will
        be accepted in case the ongoing state is reset to Idle.
        SomeIpTp_Transmit() will change the state to SOMEIPTP_FIRST_TO_BE_TRANSMITTED or SOMEIPTP_SINGLE_TO_BE_TRANSMITTED
        These states will not be changed till the next SomeIpTp_MainFunctionTx() */
        if((SomeIpTp_TxNSduState[txNsduId].ongoingState != SOMEIPTP_IDLE)
        && (SomeIpTp_TxNSduState[txNsduId].ongoingState != SOMEIPTP_FIRST_TO_BE_TRANSMITTED)
        && (SomeIpTp_TxNSduState[txNsduId].ongoingState != SOMEIPTP_SINGLE_TO_BE_TRANSMITTED))
        {
          /* !LINKSTO SomeIpTp.EB.LowerLayerTransmit.NOT_OK.Cancel,1 */
          reportError = TRUE;
        }

        SchM_Exit_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();
      }
      else
      {
        /* Nothing to do here, just let the state machine continue */
      }

      if(TRUE == reportError)
      {
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00023.SomeIpTpTxConfirmationTimeout.DET,2 */
        /* !LINKSTO SomeIpTp.EB.LowerLayerTransmit.NOT_OK.ReportRuntimeError,1 */
        SOMEIPTP_DET_REPORT_RUNTIME_ERROR(SOMEIPTP_SID_MAINFUNCTIONTX, SOMEIPTP_E_DISASSEMBLY_INTERRUPT);

        /* !LINKSTO SomeIpTp.EB.AcceptTransmit.From.PduR_SomeIpTpTxConfirmation,1 */
        SomeIpTp_ResetTxState(txNsduId);
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00023.SomeIpTpTxConfirmationTimeout.Confirmation,1 */
        /* !LINKSTO SomeIpTp.EB.LowerLayerTransmit.NOT_OK.TxConfirmation,1 */
        PduR_SomeIpTpTxConfirmation(SomeIpTp_TxNsduElements[txNsduId].NSduTxId, NTFRSLT_E_NOT_OK);
      }
      else
      {
        /* Nothing to do here, just let the state machine continue */
      }
    }
  }
  else
  {
    /* MainFunctionTx will simply return if not initialized  */
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00059,1 */
  }
#endif /* SOMEIPTP_NO_OF_TXNSDUS > 0U  */

  DBG_SOMEIPTP_MAINFUNCTIONTX_EXIT();
}

/* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00069,1 */
FUNC(void, SOMEIPTP_CODE) SomeIpTp_MainFunctionRx
(
  void
)
{
#if(SOMEIPTP_NO_OF_RXNSDUS > 0U )
  PduIdType rxNsduId = 0U;
#endif /* SOMEIPTP_NO_OF_RXNSDUS > 0U  */

  DBG_SOMEIPTP_MAINFUNCTIONRX_ENTRY();

#if(SOMEIPTP_NO_OF_RXNSDUS > 0U )
  /* check if the module was initialized */
  if(SOMEIPTP_INITIALIZED == SomeIpTp_InitState)
  {
    /* Start the MainFunctionRx */
    for(rxNsduId=0U;
        /* Deviation TASKING-1 */
        rxNsduId<SOMEIPTP_NO_OF_RXNSDUS;
        rxNsduId++)
    {
      /* !LINKSTO SomeIpTp.dsn.ExclusiveArea_0,1 */
      SchM_Enter_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_0();
      if(SOMEIPTP_IDLE != SomeIpTp_RxNSduState[rxNsduId].ongoingState)
      {
        if(SomeIpTp_RxNSduState[rxNsduId].rxTimeoutMFC == 0U )
        {
          /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00041.ReportError,2 */
          /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00054.Result_RDUR403,1 */
          SomeIpTp_InterruptAssemblyProcess(rxNsduId,
                                   SOMEIPTP_SID_MAINFUNCTIONRX,
                                   SOMEIPTP_E_ASSEMBLY_INTERRUPT);
        }
        else
        {
          SomeIpTp_RxNSduState[rxNsduId].rxTimeoutMFC--;
        }
      }
      else
      {
        /* if IDLE, Do Nothing */
      }
      /* !LINKSTO SomeIpTp.dsn.ExclusiveArea_0,1 */
      SchM_Exit_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_0();
    }
  }
  else
  {
    /* MainFunctionRx will simply return if not initialized  */
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00070,1 */
  }
#endif /* SOMEIPTP_NO_OF_RXNSDUS > 0U  */

  DBG_SOMEIPTP_MAINFUNCTIONRX_EXIT();
}

/* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_91001.NoResult_RDUR403,2 */
FUNC(void, SOMEIPTP_CODE) SomeIpTp_TxConfirmation
(
  PduIdType TxPduId
)
{
#if(SOMEIPTP_NO_OF_TXNSDUS > 0U )
  uint16 chnId = 0U;
#else
  TS_PARAM_UNUSED(TxPduId);
#endif /* SOMEIPTP_NO_OF_TXNSDUS > 0U  */

  DBG_SOMEIPTP_TXCONFIRMATION_ENTRY(TxPduId);

#if(SOMEIPTP_DEV_ERROR_DETECT == STD_ON)
  /* check if the module was initialized */
  /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00067,1 */
  if(SOMEIPTP_INITIALIZED != SomeIpTp_InitState)
  {
    SOMEIPTP_DET_REPORT_ERROR(SOMEIPTP_SID_TXCONFIRMATION, SOMEIPTP_E_NOTINIT);
  }
#if(SOMEIPTP_NO_OF_TXNSDUS > 0U )
  /* check if the function was called with valid PduId */
  /* !LINKSTO SomeIpTp.EB.TxConfirmation_Det_E_PARAM,1 */
  else if(TxPduId >= SOMEIPTP_NO_OF_TXNSDUS)
  {
    SOMEIPTP_DET_REPORT_ERROR(SOMEIPTP_SID_TXCONFIRMATION, SOMEIPTP_E_PARAM);
  }
#endif /* SOMEIPTP_NO_OF_TXNSDUS > 0U  */
  /* Start the Tx confirmation function */
  else
#endif /* SOMEIPTP_DEV_ERROR_DETECT == STD_ON */
  {
#if(SOMEIPTP_NO_OF_TXNSDUS > 0U )
    switch(SomeIpTp_TxNSduState[TxPduId].ongoingState)
    {
      case SOMEIPTP_SINGLE_CONF:
      case SOMEIPTP_LAST_CONF:
      {
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00021.PDUR403,1 */
        /* !LINKSTO SomeIpTp.EB.AcceptTransmit.From.PduR_SomeIpTpTxConfirmation,1 */
        SomeIpTp_ResetTxState(TxPduId);
        PduR_SomeIpTpTxConfirmation(SomeIpTp_TxNsduElements[TxPduId].NSduTxId, NTFRSLT_OK);
        break;
      }
      case SOMEIPTP_FIRST_CONF:
      case SOMEIPTP_CONSECUTIVE_CONF:
      {
        chnId = SomeIpTp_TxNsduElements[TxPduId].ChannelId;
        SomeIpTp_TxNSduState[TxPduId].txSeparationMFC =
                                      SomeIpTp_ChannelElements[chnId].NPduSeparationTimeMFC;

        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.FirstSegment_Payload_Retry,1 */
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.Consecutive_or_last_Segment_Retry,1 */
        SomeIpTp_TxNSduState[TxPduId].remSegments--;
        SomeIpTp_TxNSduState[TxPduId].retryState = TP_DATACONF;

        if(1U == SomeIpTp_TxNSduState[TxPduId].remSegments)
        {
          SomeIpTp_TxNSduState[TxPduId].ongoingState = SOMEIPTP_NEXT_TO_BE_LAST;
        }
        else
        {
          SomeIpTp_TxNSduState[TxPduId].ongoingState = SOMEIPTP_NEXT_TO_BE_CONSECUTIVE;
        }
        break;
      }
      default:
      {
        /* Skip all other non awaiting Confirmation states */
        break;
      }
    }
#endif /* SOMEIPTP_NO_OF_TXNSDUS > 0U  */
  }

  DBG_SOMEIPTP_TXCONFIRMATION_EXIT(TxPduId);
}

/* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00053,1 */
FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_TriggerTransmit
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr
)
{
  Std_ReturnType RetVal = E_NOT_OK;
#if(SOMEIPTP_NO_OF_TXNSDUS > 0U )
  boolean SkipWithoutInterruption = TRUE;
  boolean ValidSduLength = TRUE;
#else
  TS_PARAM_UNUSED(TxPduId);
  TS_PARAM_UNUSED(PduInfoPtr);
#endif /* SOMEIPTP_NO_OF_TXNSDUS > 0U  */

  DBG_SOMEIPTP_TRIGGERTRANSMIT_ENTRY(TxPduId, PduInfoPtr);

#if(SOMEIPTP_DEV_ERROR_DETECT == STD_ON)
  /* check if the module was initialized */
  /* !LINKSTO SomeIpTp.EB.SWS.00054.2,1 */
  if(SOMEIPTP_INITIALIZED != SomeIpTp_InitState)
  {
    SOMEIPTP_DET_REPORT_ERROR(SOMEIPTP_SID_TRIGGERTRANSMIT, SOMEIPTP_E_NOTINIT);
  }
  /* check if PduInfoPtr is not equal to NULL_PTR */
  /* !LINKSTO SomeIpTp.EB.NULLPTR_TriggerTransmit,1 */
  else if(NULL_PTR == PduInfoPtr)
  {
    SOMEIPTP_DET_REPORT_ERROR(SOMEIPTP_SID_TRIGGERTRANSMIT, SOMEIPTP_E_PARAM_POINTER);
  }
#if(SOMEIPTP_NO_OF_TXNSDUS > 0U )
  /* check if the function was called with valid PduId */
  /* !LINKSTO SomeIpTp.EB.TriggerTransmit_Det_E_PARAM,1 */
  else if(TxPduId >= SOMEIPTP_NO_OF_TXNSDUS)
  {
    SOMEIPTP_DET_REPORT_ERROR(SOMEIPTP_SID_TRIGGERTRANSMIT, SOMEIPTP_E_PARAM);
  }
#endif /* SOMEIPTP_NO_OF_TXNSDUS > 0U  */
  /* Start the trigger transmit function */
  else
#endif /* SOMEIPTP_DEV_ERROR_DETECT == STD_ON */
  {
#if(SOMEIPTP_NO_OF_TXNSDUS > 0U )
    /* Check if provided data length is larger than or equal to the actual Npdu-length
       except for last */
    switch(SomeIpTp_TxNSduState[TxPduId].ongoingState)
    {
      case SOMEIPTP_SINGLE:
      {
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00055.SINGLE,1 */
        if(PduInfoPtr->SduLength < SomeIpTp_TxNSduState[TxPduId].lastSegLength)
        {
          ValidSduLength = FALSE;
        }
        break;
      }
      case SOMEIPTP_LAST:
      {
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00055.Last,1 */
        if(PduInfoPtr->SduLength < (SomeIpTp_TxNSduState[TxPduId].lastSegLength + SOMEIPTP_HEADER_LENGTH))
        {
          ValidSduLength = FALSE;
        }
        break;
      }
      case SOMEIPTP_FIRST:
      case SOMEIPTP_CONSECUTIVE:
      {
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00055.All_But_Last,1 */
        if(PduInfoPtr->SduLength < SomeIpTp_TxNsduElements[TxPduId].MaxNpduLength)
        {
          ValidSduLength = FALSE;
        }
        break;
      }
      default:
      {
        /* Skip all other non awaiting segments states */
        break;
      }
    }

    if(TRUE == ValidSduLength)
    {
      switch(SomeIpTp_TxNSduState[TxPduId].ongoingState)
      {
        case SOMEIPTP_SINGLE:
        case SOMEIPTP_SINGLE_CONF:
        {
          RetVal = SomeIpTp_TransmitSingleSegment(TxPduId, PduInfoPtr, &SkipWithoutInterruption);
          break;
        }
        case SOMEIPTP_FIRST:
        case SOMEIPTP_FIRST_CONF:
        {
          RetVal = SomeIpTp_TransmitFirstSegment(TxPduId, PduInfoPtr, &SkipWithoutInterruption);
          break;
        }
        case SOMEIPTP_CONSECUTIVE:
        case SOMEIPTP_CONSECUTIVE_CONF:
        {
          RetVal = SomeIpTp_TransmitConsecutiveSegment(TxPduId, PduInfoPtr, &SkipWithoutInterruption);
          break;
        }
        case SOMEIPTP_LAST:
        case SOMEIPTP_LAST_CONF:
        {
          RetVal = SomeIpTp_TransmitLastSegment(TxPduId, PduInfoPtr, &SkipWithoutInterruption);
          break;
        }
        default:
        {
          /* Skip all other non awaiting segments states */
          SkipWithoutInterruption = TRUE;
          break;
        }
      }
      if(FALSE == SkipWithoutInterruption)
      {
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00024.ExpectedForAllButLast_Result_RDUR403_ReportError,2 */
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00025.Result_RDUR403_ReportError,2 */
        SOMEIPTP_DET_REPORT_RUNTIME_ERROR(SOMEIPTP_SID_TRIGGERTRANSMIT, SOMEIPTP_E_DISASSEMBLY_INTERRUPT);
        /* !LINKSTO SomeIpTp.EB.AcceptTransmit.From.PduR_SomeIpTpTxConfirmation,1 */
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00024.ExpectedForAllButLast_Result_RDUR403_CancelDisassembly,1 */
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00025.Result_RDUR403_CancelDisassembly,1 */
        SomeIpTp_ResetTxState(TxPduId);
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00024.ExpectedForAllButLast_Result_RDUR403_CallTxConfirmation,1 */
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00025.Result_RDUR403_CallTxConfirmation,1 */
        PduR_SomeIpTpTxConfirmation(SomeIpTp_TxNsduElements[TxPduId].NSduTxId, NTFRSLT_E_NOT_OK);
      }
      else
      {
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00055.All_But_Last,1 */
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00055.Last,1 */
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00055.SINGLE,1 */
        /* Just return E_NOT_OK without interruption */
      }
    }
    else
    {
      /* Shall not copy any data and return E_NOT_OK*/
    }
#endif /* SOMEIPTP_NO_OF_TXNSDUS > 0U  */
  }

  DBG_SOMEIPTP_TRIGGERTRANSMIT_EXIT(TxPduId, PduInfoPtr);

  return RetVal;
}

/* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00056.ASR403,1 */
FUNC(void, SOMEIPTP_CODE) SomeIpTp_RxIndication
(
  PduIdType RxPduId,
  P2VAR(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr
)
{
#if(SOMEIPTP_NO_OF_RXNSDUS > 0U )
  SomeIpTp_ErrorType errorChunk = SOMEIPTP_NO_RX_ERROR;
  PduLengthType offset = 0U;
  uint8 More = 0U;
  uint8 TpFlag = 0U;
  boolean assemblyOngoing = FALSE;
  boolean refuseNewAssembly = FALSE;
#else
  TS_PARAM_UNUSED(RxPduId);
  TS_PARAM_UNUSED(PduInfoPtr);
#endif /* SOMEIPTP_NO_OF_RXNSDUS > 0U  */

  DBG_SOMEIPTP_RXINDICATION_ENTRY(RxPduId, PduInfoPtr);

#if(SOMEIPTP_DEV_ERROR_DETECT == STD_ON)
  /* check if the module was initialized */
  /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00057,1 */
  if(SOMEIPTP_INITIALIZED != SomeIpTp_InitState)
  {
    SOMEIPTP_DET_REPORT_ERROR(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_NOTINIT);
  }
  /* check if PduInfoPtr is not equal to NULL_PTR */
  /* !LINKSTO SomeIpTp.EB.NULLPTR_RxIndication,1 */
  else if(NULL_PTR == PduInfoPtr)
  {
    SOMEIPTP_DET_REPORT_ERROR(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_PARAM_POINTER);
  }
#if(SOMEIPTP_NO_OF_RXNSDUS > 0U )
  /* check if the function was called with valid PduId */
  /* !LINKSTO SomeIpTp.EB.RxIndication_Det_E_PARAM,1 */
  else if(RxPduId >= SOMEIPTP_NO_OF_RXNSDUS)
  {
    SOMEIPTP_DET_REPORT_ERROR(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_PARAM);
  }
#endif /* SOMEIPTP_NO_OF_RXNSDUS > 0U  */
  /* Start Rx indication */
  else
#endif /* SOMEIPTP_DEV_ERROR_DETECT == STD_ON */
  {
#if(SOMEIPTP_NO_OF_RXNSDUS > 0U )
    if(PduInfoPtr->SduLength >= SOMEIP_HEADER_LENGTH)
    {
      TpFlag = SOMEIPTP_GET_TP_FLAG(PduInfoPtr->SduDataPtr[SOMEIPTP_MSG_TYPE_INDEX]);
      /* Check that the received pdu have header and payload */
      if(PduInfoPtr->SduLength >= SOMEIPTP_HEADER_LENGTH)
      {
        /* Get the offset and More bit out of the payload */
        offset = SOMEIPTP_GET_OFFSET(PduInfoPtr->SduDataPtr);
        More = SOMEIPTP_GET_MORE_FLAG(PduInfoPtr->SduDataPtr);
      }
      else
      {
        /* !LINKSTO SomeIpTp.EB.Zero_Payload,1 */
        /* Do nothing with the offset and the More bit, just leave them
           zero so that the segment is interpreted as a single segment */
        /* Check if the TP-Flag is set and Report error in case the
           Tp-flag is set and there is no Tp Header in the message */
        if(1U == TpFlag)
        {
          errorChunk = SOMEIPTP_E_MISSING_SOMEIPTP_HEADER;
        }
      }

      /* Single segment message handling, i.e. TP-Flag is zero so it is not
         a TP message, also if TP-Flag = 1 and the More bit =0 and it is not
         the last segment since the offset indicate that there were no
         previously received segments, so this is the first received segment
         and will not be followed by more segments (single segment message)*/
      if((0U == TpFlag) || ((0U == offset) && (0U == More)))
      {
        /* !LINKSTO SomeIpTp.dsn.ExclusiveArea_0,1 */
        SchM_Enter_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_0();
        if(SOMEIPTP_IDLE != SomeIpTp_RxNSduState[RxPduId].ongoingState)
        {
          /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00054.Result_RDUR403,1 */
          SomeIpTp_RxNSduState[RxPduId].ongoingState = SOMEIPTP_IDLE;
          /* Indicate that there were an ongoing assembly process */
          assemblyOngoing = TRUE;
        }
        /* !LINKSTO SomeIpTp.dsn.ExclusiveArea_0,1 */
        SchM_Exit_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_0();

        /* Check if there were an ongoing assembly process, if so then determine waht
           error should be reported */
        if(TRUE == assemblyOngoing)
        {
          /* If the TP-Flag is set to zero then report an error in the message type */
          if(0U == TpFlag)
          {
            /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00045.AllButFirst_ReportError,2 */
            errorChunk = SOMEIPTP_E_MESSAGE_TYPE;

          }
          /* else if the TP-Flag is set to 1, and the error is an error other than
             SOMEIPTP_E_MISSING_SOMEIPTP_HEADER, then it is an inconsistent sequence
             i.e. single segment is received while it was not expected as we were in
             an already ongoing process */
          else if(SOMEIPTP_E_MISSING_SOMEIPTP_HEADER != errorChunk)
          {
            /* !LINKSTO SomeIpTp.EB.00085.Report_Det_Error,3 */
            errorChunk = SOMEIPTP_E_INCONSISTENT_SEQUENCE;
          }
          else
          {
            /* This is the case where TP-Flag = 1 and the
            errorChunk = SOMEIPTP_E_MISSING_SOMEIPTP_HEADER*/
            /* Just leave the errorChunk assigned to the value
               SOMEIPTP_E_MISSING_SOMEIPTP_HEADER to be handled
               later*/
          }
        }

        /* Check if any errors existed */
        if(SOMEIPTP_NO_RX_ERROR != errorChunk)
        {
          /* If an error exist while there is an ongoing assembly process,
          interrupt it and report a DET error, also send the Rx indication
          to upper layer */
          if(TRUE == assemblyOngoing)
          {
            /* SomeIpTpRxTimeoutTime cancelled for this pduId */
            /* !LINKSTO SomeIpTp.EB.00085.Interrupt_Assembly,2 */
            /* !LINKSTO SomeIpTp.EB.Check_SomeIpTp_Header.ReportRuntimeError,1 */
            /* !LINKSTO SomeIpTp.EB.Check_SomeIpTp_Header.call_RxIndication,1 */
            /* !LINKSTO SomeIpTp.EB.Check_SomeIpTp_Header.Cancel_TimeOut,1 */
            SomeIpTp_InterruptAssemblyProcess(RxPduId,
                                              SOMEIPTP_SID_RXINDICATION,
                                              errorChunk);
          }
          /* This is the case where there were not an ongoing assembly
             process, and errorChunk = SOMEIPTP_E_MISSING_SOMEIPTP_HEADER,
             then just report a DET error, nothing more is needed to be done
             here, since there is no assembly process (ongoing/started) */
          else
          {
            /* !LINKSTO SomeIpTp.EB.Check_SomeIpTp_Header.ReportRuntimeError,1 */
            SOMEIPTP_DET_REPORT_RUNTIME_ERROR(SOMEIPTP_SID_RXINDICATION, errorChunk);
          }

          /* For both errors SOMEIPTP_E_MESSAGE_TYPE & SOMEIPTP_E_MISSING_SOMEIPTP_HEADER the
             reception shall not be processed any more, since these errors can't be recovered
             i.e. the received messages can not be received */
          /* Only error that can be recovered is the SOMEIPTP_E_INCONSISTENT_SEQUENCE error, the
             received single segment shall not be stopped and considered as a new single segment
             message */
          if(SOMEIPTP_E_INCONSISTENT_SEQUENCE == errorChunk)
          {
            refuseNewAssembly = FALSE;
          }
          /* case of SOMEIPTP_E_MESSAGE_TYPE or SOMEIPTP_E_MISSING_SOMEIPTP_HEADER */
          else
          {
            refuseNewAssembly = TRUE;
          }
        }
        else
        {
          /* No Assembly process for multiple segments is already running */
          /* No other errors occured and NSdu is ready for single segment reception */
        }
        /* Process single segment*/
        /* The case were the TP-Flag is set to 0 while there is an ongoing assembly process
           should be treated as an error in the message type field and the segment/message
           should not be processed*/
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00031,1 */
        /* !LINKSTO SomeIpTp.EB.00084,2 */
        /* !LINKSTO SomeIpTp.EB.00085.Call_StartOfReception,2 */
        if(FALSE == refuseNewAssembly)
        {
          errorChunk = SomeIpTp_ReceiveSingleMessage(RxPduId, PduInfoPtr);
        }
        else
        {
          /* indicate no more errors to process since the errors are already processed */
          errorChunk = SOMEIPTP_NO_RX_ERROR;
        }
      }
      /* Tp message */
      else
      {
        if(0U == offset)
        {
          /* first segment */
          /* offset = 0, M = 1*/
          errorChunk = SomeIpTp_ReceiveFirstSegment(RxPduId, PduInfoPtr);
        }
        else
        {
          /* Consecutive or Last segment */
          /* offset > 0, M = 1 : Consecutive */
          /* offset > 0, M = 0 : Last */
          errorChunk = SomeIpTp_ReceiveConsecutiveOrLastSegment(RxPduId, PduInfoPtr, More);
        }
      }
    }
    else
    {
      /* No SomeIp header found, Report an error */
      /* !LINKSTO SomeIpTp.dsn.ExclusiveArea_0,1 */
      SchM_Enter_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_0();
      if(SOMEIPTP_IDLE != SomeIpTp_RxNSduState[RxPduId].ongoingState)
      {
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00054.Result_RDUR403,1 */
        SomeIpTp_RxNSduState[RxPduId].ongoingState = SOMEIPTP_IDLE;
        assemblyOngoing = TRUE;
      }
      /* !LINKSTO SomeIpTp.dsn.ExclusiveArea_0,1 */
      SchM_Exit_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_0();

      /* If an error exist while there is an ongoing assembly process,
          interrupt it and report a DET error, also send the Rx indication
          to upper layer */
      if(TRUE == assemblyOngoing)
      {
        /* !LINKSTO SomeIpTp.EB.Check_SomeIp_Header.call_RxIndication,1 */
        /* !LINKSTO SomeIpTp.EB.Check_SomeIp_Header.Cancel_TimeOut,1 */
        /* !LINKSTO SomeIpTp.EB.Check_SomeIp_Header.ReportRuntimeError,1 */
        errorChunk = SOMEIPTP_E_INCONSISTENT_HEADER;
      }
      /* This is the case where there were not an ongoing assembly process,
         then just report a DET error, nothing more is needed to be done
         here, since there is no assembly process (ongoing/started) */
      else
      {
        /* !LINKSTO SomeIpTp.EB.Check_SomeIp_Header.ReportRuntimeError,1 */
        SOMEIPTP_DET_REPORT_RUNTIME_ERROR(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_INCONSISTENT_HEADER);
      }
    }

    /* Handle the errors caused due to reception of (single/first/consecutive/last) segment */
    if(SOMEIPTP_NO_RX_ERROR != errorChunk)
    {
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00054.Result_RDUR403,1 */
      SomeIpTp_InterruptAssemblyProcess(RxPduId,
                               SOMEIPTP_SID_RXINDICATION,
                               errorChunk);
    }
    else
    {
      /* It is received with no errors */
    }
#endif /* SOMEIPTP_NO_OF_RXNSDUS > 0U  */
  }

  DBG_SOMEIPTP_RXINDICATION_EXIT(RxPduId, PduInfoPtr);
}

/* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00047,1 */
FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_Transmit
(
  PduIdType TxPduId,
  P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_CONST) PduInfoPtr
)
{
  Std_ReturnType retVal = E_NOT_OK;
#if(SOMEIPTP_NO_OF_TXNSDUS > 0U )
  PduLengthType ModValue = 0;
  PduLengthType DivValue = 0;
  boolean ProcessIntFlag = FALSE;
#else
  TS_PARAM_UNUSED(TxPduId);
  TS_PARAM_UNUSED(PduInfoPtr);
#endif /* SOMEIPTP_NO_OF_TXNSDUS > 0U  */

  DBG_SOMEIPTP_TRANSMIT_ENTRY(TxPduId,PduInfoPtr);

#if(SOMEIPTP_DEV_ERROR_DETECT == STD_ON)
  /* check if the module was initialized */
  /* !LINKSTO SomeIpTp.EB.SWS.00052.2,1 */
  if(SOMEIPTP_INITIALIZED != SomeIpTp_InitState)
  {
    /* !LINKSTO SomeIpTp.EB.SWS.00048.2,1 */
    SOMEIPTP_DET_REPORT_ERROR(SOMEIPTP_SID_TRANSMIT, SOMEIPTP_E_NOTINIT);
  }
#if(SOMEIPTP_NO_OF_TXNSDUS > 0U )
  /* check if the function was called with valid PduId */
  /* !LINKSTO SomeIpTp.EB.SWS.00049.2,1 */
  else if(TxPduId >= SOMEIPTP_NO_OF_TXNSDUS)
  {
    SOMEIPTP_DET_REPORT_ERROR(SOMEIPTP_SID_TRANSMIT, SOMEIPTP_E_PARAM);
  }
  /* check if the function was called without NULL_PTR PduInfoPtr */
  /* !LINKSTO SomeIpTp.EB.SWS.00050.2,1 */
  else if(NULL_PTR == PduInfoPtr)
  {
    SOMEIPTP_DET_REPORT_ERROR(SOMEIPTP_SID_TRANSMIT, SOMEIPTP_E_PARAM_POINTER);
  }
  /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00016,1 */
  /* !LINKSTO SomeIpTp.ASR44.SWS_SomeIpTp_00022.ReportError,2 */
#endif /* SOMEIPTP_NO_OF_TXNSDUS > 0U */
  else
#endif /* SOMEIPTP_DEV_ERROR_DETECT == STD_ON */
  {
#if(SOMEIPTP_NO_OF_TXNSDUS > 0U )
    /* !LINKSTO SomeIpTp.dsn.ExclusiveArea_1,1 */
    SchM_Enter_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();
    if(SOMEIPTP_IDLE != SomeIpTp_TxNSduState[TxPduId].ongoingState)
    {
      SomeIpTp_TxNSduState[TxPduId].ongoingState = SOMEIPTP_IDLE;
      ProcessIntFlag = TRUE;
    }
    /* !LINKSTO SomeIpTp.dsn.ExclusiveArea_1,1 */
    SchM_Exit_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();
    if(FALSE == ProcessIntFlag) /* There wasn't an ongoing process */
    {
      if(PduInfoPtr->SduLength <= SomeIpTp_TxNsduElements[TxPduId].MaxNpduLength)
      {
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00017.Pdu_Length,1 */
        SomeIpTp_TxNSduState[TxPduId].lastSegLength = PduInfoPtr->SduLength;
        retVal = E_OK;
        SomeIpTp_TxNSduState[TxPduId].ongoingState = SOMEIPTP_SINGLE_TO_BE_TRANSMITTED;
      }
      else
      {
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00001,1 */
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00002,1 */
        ModValue = SOMEIPTP_PDULENGTH_MODULUS((PduInfoPtr->SduLength - SOMEIP_HEADER_LENGTH),
                                              (SomeIpTp_TxNsduElements[TxPduId].MaxNpduLength - SOMEIPTP_HEADER_LENGTH));
        DivValue = SOMEIPTP_PDULENGTH_DIVIDE((PduInfoPtr->SduLength - SOMEIP_HEADER_LENGTH),
                                             (SomeIpTp_TxNsduElements[TxPduId].MaxNpduLength - SOMEIPTP_HEADER_LENGTH));
        if(0U != ModValue)
        {
          /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00017.Pdu_Length,1 */
          SomeIpTp_TxNSduState[TxPduId].lastSegLength = ModValue;
          SomeIpTp_TxNSduState[TxPduId].remSegments = DivValue + 1U;
        }
        else
        {
          /* The last segment length is the same like Consecutive, dividable by 16 */
          /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00017.Pdu_Length,1 */
          SomeIpTp_TxNSduState[TxPduId].lastSegLength = (SomeIpTp_TxNsduElements[TxPduId].MaxNpduLength - SOMEIPTP_HEADER_LENGTH);
          SomeIpTp_TxNSduState[TxPduId].remSegments = DivValue;
        }
        retVal = E_OK;
        SomeIpTp_TxNSduState[TxPduId].ongoingState = SOMEIPTP_FIRST_TO_BE_TRANSMITTED;
      }
    }
    else /* There was an ongoing process and interrupted */
    {
      SOMEIPTP_DET_REPORT_RUNTIME_ERROR(SOMEIPTP_SID_TRANSMIT, SOMEIPTP_E_DISASSEMBLY_INTERRUPT);

      /* !LINKSTO SomeIpTp.ASR44.SWS_SomeIpTp_00022.Confirmation,1 */
      /* !LINKSTO SomeIpTp.EB.AcceptTransmit.From.PduR_SomeIpTpTxConfirmation,1 */
      SomeIpTp_ResetTxState(TxPduId);
      PduR_SomeIpTpTxConfirmation(SomeIpTp_TxNsduElements[TxPduId].NSduTxId, NTFRSLT_E_NOT_OK);
    }
#endif /* SOMEIPTP_NO_OF_TXNSDUS > 0U  */
  }

  DBG_SOMEIPTP_TRANSMIT_EXIT(TxPduId,PduInfoPtr);

  return retVal;
}

#define SOMEIPTP_STOP_SEC_CODE
#include <SomeIpTp_MemMap.h>

/*==================[internal function definitions]=========================*/

#define SOMEIPTP_START_SEC_CODE
#include <SomeIpTp_MemMap.h>

#if(SOMEIPTP_NO_OF_TXNSDUS > 0U )
STATIC FUNC(void, SOMEIPTP_CODE) SomeIpTp_ResetTxState
(
  PduIdType TxPduId
)
{
  SomeIpTp_TxNSduState[TxPduId].txSeparationMFC = 0U;
  SomeIpTp_TxNSduState[TxPduId].txConfTimeoutMFC = 0U;
  SomeIpTp_TxNSduState[TxPduId].offset = 0U;
  SomeIpTp_TxNSduState[TxPduId].remSegments = 0U;
  SomeIpTp_TxNSduState[TxPduId].lastSegLength = 0U;
  TS_MemBZero(SomeIpTp_TxNsduElements[TxPduId].MetaData, SOMEIP_HEADER_LENGTH);
  SomeIpTp_TxNSduState[TxPduId].retryState = TP_CONFPENDING;
  SomeIpTp_TxNSduState[TxPduId].ongoingState = SOMEIPTP_IDLE;
}

STATIC FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_TransmitSingleSegment
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr,
  P2VAR(boolean, AUTOMATIC, SOMEIPTP_APPL_DATA) SkipWithoutInterruptionFlag
)
{
  PduLengthType RemainingLength = 0U;
  Std_ReturnType RetVal = E_NOT_OK;
  SomeIpTp_RetryStateType retry;
  RetryInfoType retryInfo;

  /*Send the whole length with no change at all in the first call of
  PduR_SomeIpTpCopyTxData, i.e: no change in header or of course in payload */
  PduInfoPtr->SduLength = SomeIpTp_TxNSduState[TxPduId].lastSegLength;

  SchM_Enter_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();
  retry = SomeIpTp_TxNSduState[TxPduId].retryState;
  SchM_Exit_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();

  if(TP_CONFPENDING == retry)
  {
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.SingleSegment,1 */
    retryInfo.TpDataState = TP_CONFPENDING;
    retryInfo.TxTpDataCnt = 0U;
  }
  else /* TP_DATARETRY == retry */
  {
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.SingleSegment_Retry,1 */
    retryInfo.TpDataState = TP_DATARETRY;
    retryInfo.TxTpDataCnt = PduInfoPtr->SduLength;
  }

  /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.SingleSegment,1 */
  /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.SingleSegment_Retry,1 */
  /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00009.SDU_Fitting,1 */
  if(BUFREQ_OK == PduR_SomeIpTpCopyTxData(SomeIpTp_TxNsduElements[TxPduId].NSduTxId,
                          PduInfoPtr,
                          &retryInfo,
                          &RemainingLength))
  {
    uint16 chnId = SomeIpTp_TxNsduElements[TxPduId].ChannelId;

    if(TP_CONFPENDING == retry)
    {
      SchM_Enter_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();
      SomeIpTp_TxNSduState[TxPduId].retryState = TP_DATARETRY;
      SomeIpTp_TxNSduState[TxPduId].txConfTimeoutMFC = SomeIpTp_ChannelElements[chnId].TxConfTimeoutTimeMFC;
      SchM_Exit_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();
    }

    RetVal = E_OK;
    SomeIpTp_TxNSduState[TxPduId].ongoingState = SOMEIPTP_SINGLE_CONF;
  }
  else
  {
    *SkipWithoutInterruptionFlag = FALSE;
  }

  return RetVal;
}

STATIC FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_TransmitFirstSegment
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr,
  P2VAR(boolean, AUTOMATIC, SOMEIPTP_APPL_DATA) SkipWithoutInterruptionFlag
)
{
  PduInfoType pduInfo;
  PduLengthType RemainingLength = 0U;
  Std_ReturnType RetVal = E_NOT_OK;
  SomeIpTp_RetryStateType retry;
  RetryInfoType retryInfo;
  boolean headerCopied = FALSE;

  PduInfoPtr->SduLength = SomeIpTp_TxNsduElements[TxPduId].MaxNpduLength;
  pduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
  pduInfo.SduLength = SOMEIP_HEADER_LENGTH;

  SchM_Enter_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();
  retry = SomeIpTp_TxNSduState[TxPduId].retryState;
  SchM_Exit_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();

  if(TP_CONFPENDING == retry)
  {
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.FirstSegment_header,1 */
    if((BUFREQ_OK == PduR_SomeIpTpCopyTxData(SomeIpTp_TxNsduElements[TxPduId].NSduTxId,
                            &pduInfo,
                            NULL_PTR,
                            &RemainingLength)) &&
       (RemainingLength >= (SomeIpTp_TxNsduElements[TxPduId].MaxNpduLength - SOMEIPTP_HEADER_LENGTH)))
    {
      /* storing the message header from upper layer to the Pdu Metadata */
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00007,1 */
      /* !LINKSTO SomeIpTp.ASR44.SWS_SomeIpTp_00008,1 */
      TS_MemCpy(SomeIpTp_TxNsduElements[TxPduId].MetaData, pduInfo.SduDataPtr, SOMEIP_HEADER_LENGTH);

      headerCopied = TRUE;
    }
  }

  if((TP_CONFPENDING != retry) ||
     /* Deviation TASKING-1 */
     ((TP_CONFPENDING == retry) && (TRUE == headerCopied)))
  {
    /*Copying the message header to the first segment */
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00003,1 */
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00007,1 */
    TS_MemCpy(PduInfoPtr->SduDataPtr, SomeIpTp_TxNsduElements[TxPduId].MetaData, SOMEIP_HEADER_LENGTH);

    pduInfo.SduDataPtr = &(PduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_LENGTH]);
    pduInfo.SduLength = (SomeIpTp_TxNsduElements[TxPduId].MaxNpduLength - SOMEIPTP_HEADER_LENGTH);

    if(TP_CONFPENDING == retry)
    {
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.FirstSegment_payload,1 */
      retryInfo.TpDataState = TP_CONFPENDING;
      retryInfo.TxTpDataCnt = 0U;
    }
    else /* TP_DATARETRY == retry */
    {
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.FirstSegment_Payload_Retry,1 */
      retryInfo.TpDataState = TP_DATARETRY;
      retryInfo.TxTpDataCnt = pduInfo.SduLength;
    }

    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.FirstSegment_payload,1 */
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.FirstSegment_Payload_Retry,1 */
    if((BUFREQ_OK == PduR_SomeIpTpCopyTxData(SomeIpTp_TxNsduElements[TxPduId].NSduTxId,
                          &pduInfo,
                          &retryInfo,
                          &RemainingLength)) &&
       (
        ((SomeIpTp_TxNSduState[TxPduId].remSegments > SOMEIPTP_TWO_SEGMENTS) &&
         (RemainingLength >= (SomeIpTp_TxNsduElements[TxPduId].MaxNpduLength - SOMEIPTP_HEADER_LENGTH))) ||
        ((SOMEIPTP_TWO_SEGMENTS == SomeIpTp_TxNSduState[TxPduId].remSegments) &&
         (RemainingLength >= SomeIpTp_TxNSduState[TxPduId].lastSegLength))
       ))
    {
      uint16 chnId = SomeIpTp_TxNsduElements[TxPduId].ChannelId;
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00003,1 */
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00010,1 */
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00009.Message_Type_Set_Tp_Flag,1 */
      SOMEIPTP_SET_TP_FLAG(PduInfoPtr->SduDataPtr[SOMEIPTP_MSG_TYPE_INDEX]);
      /* offset = 0, RES = 000, M = 1 in order of bigendian */
      /* !LINKSTO SomeIpTp.EB.TpHeader.BIGENDIAN,1 */
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00011,1 */
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00013,1 */
      PduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_INDEX] = 0U;
      PduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_INDEX+1U] = 0U;
      PduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_INDEX+2U] = 0U;
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00014,1 */
      PduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_INDEX+3U] = 1U;

      if(TP_CONFPENDING == retry)
      {
        SchM_Enter_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();
        SomeIpTp_TxNSduState[TxPduId].retryState = TP_DATARETRY;
        SomeIpTp_TxNSduState[TxPduId].txConfTimeoutMFC = SomeIpTp_ChannelElements[chnId].TxConfTimeoutTimeMFC;
        SchM_Exit_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();
      }
      RetVal = E_OK;
      SomeIpTp_TxNSduState[TxPduId].ongoingState = SOMEIPTP_FIRST_CONF;
    }
    else
    {
      *SkipWithoutInterruptionFlag = FALSE;
    }
  }
  else
  {
    *SkipWithoutInterruptionFlag = FALSE;
  }

  return RetVal;
}

STATIC FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_TransmitConsecutiveSegment
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr,
  P2VAR(boolean, AUTOMATIC, SOMEIPTP_APPL_DATA) SkipWithoutInterruptionFlag
)
{
  PduInfoType pduInfo;
  PduLengthType offset = 0;
  PduLengthType RemainingLength = 0U;
  Std_ReturnType RetVal = E_NOT_OK;
  SomeIpTp_RetryStateType retry;
  RetryInfoType retryInfo;

  PduInfoPtr->SduLength = SomeIpTp_TxNsduElements[TxPduId].MaxNpduLength;
  pduInfo.SduDataPtr = &(PduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_LENGTH]);
  pduInfo.SduLength = (SomeIpTp_TxNsduElements[TxPduId].MaxNpduLength - SOMEIPTP_HEADER_LENGTH);

  SchM_Enter_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();
  retry = SomeIpTp_TxNSduState[TxPduId].retryState;
  SchM_Exit_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();

  if(TP_DATACONF == retry)
  {
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.Consecutive_or_last_Segment,1 */
    retryInfo.TpDataState = TP_DATACONF;
    retryInfo.TxTpDataCnt = 0U;
  }
  else /* TP_DATARETRY == retry */
  {
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.Consecutive_or_last_Segment_Retry,1 */
    retryInfo.TpDataState = TP_DATARETRY;
    retryInfo.TxTpDataCnt = pduInfo.SduLength;
  }

  /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.Consecutive_or_last_Segment,1 */
  /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.Consecutive_or_last_Segment_Retry,1 */
  if((BUFREQ_OK == PduR_SomeIpTpCopyTxData(SomeIpTp_TxNsduElements[TxPduId].NSduTxId,
                        &pduInfo,
                        &retryInfo,
                        &RemainingLength)) &&
     (
      ((SomeIpTp_TxNSduState[TxPduId].remSegments > SOMEIPTP_TWO_SEGMENTS) &&
       (RemainingLength >= (SomeIpTp_TxNsduElements[TxPduId].MaxNpduLength - SOMEIPTP_HEADER_LENGTH))) ||
      ((SOMEIPTP_TWO_SEGMENTS == SomeIpTp_TxNSduState[TxPduId].remSegments) &&
       (RemainingLength >= SomeIpTp_TxNSduState[TxPduId].lastSegLength))
     ))
  {
    uint16 chnId = SomeIpTp_TxNsduElements[TxPduId].ChannelId;
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00003,1 */
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00007,1 */
    TS_MemCpy(PduInfoPtr->SduDataPtr, SomeIpTp_TxNsduElements[TxPduId].MetaData, SOMEIP_HEADER_LENGTH);

    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00010,1 */
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00009.Message_Type_Set_Tp_Flag,1 */
    SOMEIPTP_SET_TP_FLAG(PduInfoPtr->SduDataPtr[SOMEIPTP_MSG_TYPE_INDEX]);

    offset = SOMEIPTP_CALC_OFFSET(SomeIpTp_TxNsduElements[TxPduId].MaxNpduLength - SOMEIPTP_HEADER_LENGTH);
    if(TP_DATACONF == retry)
    {
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00012,1 */
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.Consecutive_or_last_Segment_Retry,1 */
      /* offset value Shall be incremented once with the first time to copy data */
      SomeIpTp_TxNSduState[TxPduId].offset += offset;
    }
    offset = SomeIpTp_TxNSduState[TxPduId].offset;
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00013,1 */
    offset = offset<<SOMEIPTP_RES_M_LENGTH;
    /* offset += currentpayload/16, RES = 000, M = 1 */
    /* !LINKSTO SomeIpTp.EB.TpHeader.BIGENDIAN,1 */
    PduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_INDEX] = SOMEIPTP_FIRST_BYTLE_BIG(offset);
    PduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_INDEX+1U] = SOMEIPTP_SECOND_BYTLE_BIG(offset);
    PduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_INDEX+2U] = SOMEIPTP_THIRD_BYTLE_BIG(offset);
    PduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_INDEX+3U] = SOMEIPTP_FOURTH_BYTLE_BIG(offset);
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00014,1 */
    PduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_INDEX+3U] |= 0x01U;

    if(TP_DATACONF == retry)
    {
      SchM_Enter_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();
      SomeIpTp_TxNSduState[TxPduId].retryState = TP_DATARETRY;
      SomeIpTp_TxNSduState[TxPduId].txConfTimeoutMFC = SomeIpTp_ChannelElements[chnId].TxConfTimeoutTimeMFC;
      SchM_Exit_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();
    }
    RetVal = E_OK;
    SomeIpTp_TxNSduState[TxPduId].ongoingState = SOMEIPTP_CONSECUTIVE_CONF;
  }
  else
  {
    *SkipWithoutInterruptionFlag = FALSE;
  }

  return RetVal;
}

STATIC FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_TransmitLastSegment
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr,
  P2VAR(boolean, AUTOMATIC, SOMEIPTP_APPL_DATA) SkipWithoutInterruptionFlag
)
{
  PduInfoType pduInfo;
  PduLengthType offset = 0;
  PduLengthType RemainingLength = 0U;
  Std_ReturnType RetVal = E_NOT_OK;
  SomeIpTp_RetryStateType retry;
  RetryInfoType retryInfo;

  PduInfoPtr->SduLength =
              SOMEIPTP_HEADER_LENGTH + SomeIpTp_TxNSduState[TxPduId].lastSegLength;
  pduInfo.SduDataPtr = &(PduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_LENGTH]);
  pduInfo.SduLength = SomeIpTp_TxNSduState[TxPduId].lastSegLength;

  SchM_Enter_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();
  retry = SomeIpTp_TxNSduState[TxPduId].retryState;
  SchM_Exit_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();

  if(TP_DATACONF == retry)
  {
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.Consecutive_or_last_Segment,1 */
    retryInfo.TpDataState = TP_DATACONF;
    retryInfo.TxTpDataCnt = 0U;
  }
  else /* TP_DATARETRY == retry */
  {
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.Consecutive_or_last_Segment_Retry,1 */
    retryInfo.TpDataState = TP_DATARETRY;
    retryInfo.TxTpDataCnt = pduInfo.SduLength;
  }

  /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.Consecutive_or_last_Segment,1 */
  /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.Consecutive_or_last_Segment_Retry,1 */
  if(BUFREQ_OK == PduR_SomeIpTpCopyTxData(SomeIpTp_TxNsduElements[TxPduId].NSduTxId,
                        &pduInfo,
                        &retryInfo,
                        &RemainingLength))
  {
    uint16 chnId = SomeIpTp_TxNsduElements[TxPduId].ChannelId;
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00003,1 */
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00007,1 */
    TS_MemCpy(PduInfoPtr->SduDataPtr, SomeIpTp_TxNsduElements[TxPduId].MetaData, SOMEIP_HEADER_LENGTH);

    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00010,1 */
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00009.Message_Type_Set_Tp_Flag,1 */
    SOMEIPTP_SET_TP_FLAG(PduInfoPtr->SduDataPtr[SOMEIPTP_MSG_TYPE_INDEX]);

    offset = SOMEIPTP_CALC_OFFSET(SomeIpTp_TxNsduElements[TxPduId].MaxNpduLength - SOMEIPTP_HEADER_LENGTH);
    if(TP_DATACONF == retry)
    {
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00012,1 */
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00018.Consecutive_or_last_Segment_Retry,1 */
      /* offset value Shall be incremented once with the first time to copy data */
      SomeIpTp_TxNSduState[TxPduId].offset += offset;
    }
    offset = SomeIpTp_TxNSduState[TxPduId].offset;
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00013,1 */
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00015,1 */
    offset = offset<<SOMEIPTP_RES_M_LENGTH;
    /* offset += currentpayload/16, RES = 000, M = 0 */
    /* !LINKSTO SomeIpTp.EB.TpHeader.BIGENDIAN,1 */
    PduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_INDEX] = SOMEIPTP_FIRST_BYTLE_BIG(offset);
    PduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_INDEX+1U] = SOMEIPTP_SECOND_BYTLE_BIG(offset);
    PduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_INDEX+2U] = SOMEIPTP_THIRD_BYTLE_BIG(offset);
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00015,1 */
    PduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_INDEX+3U] = SOMEIPTP_FOURTH_BYTLE_BIG(offset);

    if(TP_DATACONF == retry)
    {
      SchM_Enter_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();
      SomeIpTp_TxNSduState[TxPduId].retryState = TP_DATARETRY;
      SomeIpTp_TxNSduState[TxPduId].txConfTimeoutMFC = SomeIpTp_ChannelElements[chnId].TxConfTimeoutTimeMFC;
      SchM_Exit_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_1();
    }
    RetVal = E_OK;
    SomeIpTp_TxNSduState[TxPduId].ongoingState = SOMEIPTP_LAST_CONF;
  }
  else
  {
    *SkipWithoutInterruptionFlag = FALSE;
  }

  return RetVal;
}
#endif /* SOMEIPTP_NO_OF_TXNSDUS > 0U  */

#if(SOMEIPTP_NO_OF_RXNSDUS > 0U )
STATIC FUNC(void, SOMEIPTP_CODE) SomeIpTp_InterruptAssemblyProcess
(
  PduIdType RxPduId,
  uint8 ApiId,
  uint8 ErrorId
)
{
  /* InterruptAssemblyProcess */
  /* 1. Call PduR_SomeIpTpRxIndication() with pduId, NTFRSLT_E_NOT_OK if needed, i.e., start of reception accepted */
  /* PduR_SomeIpTpRxIndication is not called in case the start of reception was not successful, i.e. no reception started */
  /* SOMEIPTP_E_ASSEMBLY_INTERRUPT_SOR is the case when start of reception fail, so we do not send PduR_SomeIpTpRxIndication */
  if(ErrorId != SOMEIPTP_E_ASSEMBLY_INTERRUPT_SOR)
  {
    /* !LINKSTO SomeIpTp.EB.SWS.00083.call_RxIndication,1 */
    PduR_SomeIpTpRxIndication(SomeIpTp_RxNsduElements[RxPduId].NSduRxIndId, NTFRSLT_E_NOT_OK);
  }
  else
  {
    /* assign the correct runtime error ID for failing start of reception instead of SOMEIPTP_E_ASSEMBLY_INTERRUPT_SOR*/
    ErrorId = SOMEIPTP_E_ASSEMBLY_INTERRUPT;
  }
  /* 2. SomeIpTpRxTimeoutTime cancelled for this pduId */
  /* !LINKSTO SomeIpTp.EB.SWS.00083.Cancel_TimeOut,1 */
  SomeIpTp_ResetRxState(RxPduId);

  SOMEIPTP_DET_REPORT_RUNTIME_ERROR(ApiId, ErrorId);
}

STATIC FUNC(void, SOMEIPTP_CODE) SomeIpTp_ResetRxState
(
  PduIdType RxPduId
)
{
  SomeIpTp_RxNSduState[RxPduId].rxTimeoutMFC = 0U;
  SomeIpTp_RxNSduState[RxPduId].expOffest = 0U;
  SomeIpTp_RxNSduState[RxPduId].AvailBufLength = 0U;
  TS_MemBZero(SomeIpTp_RxNsduElements[RxPduId].MetaData, SOMEIP_HEADER_LENGTH);
  SomeIpTp_RxNSduState[RxPduId].ongoingState = SOMEIPTP_IDLE;
}

STATIC FUNC(SomeIpTp_ErrorType, SOMEIPTP_CODE) SomeIpTp_ReceiveSingleMessage
(
  PduIdType RxPduId,
  P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_CONST) PduInfoPtr
)
{
  SomeIpTp_ErrorType errorChunk = SOMEIPTP_NO_RX_ERROR;
  PduLengthType AvailBufLength = 0U;
  PduInfoType PduInfo;
  BufReq_ReturnType startOfRecResult;

  PduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
  if(1U == SOMEIPTP_GET_TP_FLAG(PduInfoPtr->SduDataPtr[SOMEIPTP_MSG_TYPE_INDEX]))
  {
    /* !LINKSTO SomeIpTp.EB.00084,2 */
    PduInfo.SduLength = SOMEIP_HEADER_LENGTH;
  }
  else
  {
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00031,1 */
    PduInfo.SduLength = PduInfoPtr->SduLength;
  }
  /* Single segment is received */
    startOfRecResult= PduR_SomeIpTpStartOfReception(SomeIpTp_RxNsduElements[RxPduId].NSduRxIndId,
                                PduInfoPtr->SduLength,
                                &AvailBufLength);
    if((BUFREQ_OK == startOfRecResult) &&
     (AvailBufLength >= PduInfo.SduLength))
  {
    if(1U == SOMEIPTP_GET_TP_FLAG(PduInfoPtr->SduDataPtr[SOMEIPTP_MSG_TYPE_INDEX]))
    {
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00034.CopyRxDataForHeader,1 */
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00030.Forward_MetaData,1 */
      if((BUFREQ_OK == PduR_SomeIpTpCopyRxData(SomeIpTp_RxNsduElements[RxPduId].NSduRxIndId,
                                &PduInfo,
                                &AvailBufLength)) &&
         (AvailBufLength >= (PduInfoPtr->SduLength - SOMEIPTP_HEADER_LENGTH)))
      {
        /* No errors */
      }
      else
      {
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00050.ReportError,2 */
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00049.bufferSizePtr_CopyRxData,2 */
        /* !BUFREQ_OK or AvailBufLength < payload length "copyRxData" for SOME/IP header */
        errorChunk = SOMEIPTP_E_ASSEMBLY_INTERRUPT;
      }
      PduInfo.SduDataPtr = &PduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_LENGTH];
      PduInfo.SduLength = PduInfoPtr->SduLength - SOMEIPTP_HEADER_LENGTH;
    }
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00034.CopyRxDataForPayload,1 */
    if(SOMEIPTP_NO_RX_ERROR == errorChunk)
    {
      if(BUFREQ_OK == PduR_SomeIpTpCopyRxData(SomeIpTp_RxNsduElements[RxPduId].NSduRxIndId,
                                  &PduInfo,
                                  &AvailBufLength))
      {
        PduR_SomeIpTpRxIndication(SomeIpTp_RxNsduElements[RxPduId].NSduRxIndId, NTFRSLT_OK);
      }
      else
      {
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00050.ReportError,2 */
        /* !BUFREQ_OK "copyRxData" for received single segment data */
        errorChunk = SOMEIPTP_E_ASSEMBLY_INTERRUPT;
      }
    }
  }
  else
  {
    /* if start of reception BUFREQ_OK then the issue is in the AvailBufLength < SOME/IP header length */
    if(BUFREQ_OK == startOfRecResult)
    {
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00049.bufferSizePtr_StartOfReception,2 */
      errorChunk = SOMEIPTP_E_ASSEMBLY_INTERRUPT;
    }
    /* else then the start of reception is not BUFREQ_OK then report runtime error */
    else
    {
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00051.ReportError,2 */
      errorChunk = SOMEIPTP_E_ASSEMBLY_INTERRUPT_SOR;
    }
  }
  return errorChunk;
}

STATIC FUNC(SomeIpTp_ErrorType, SOMEIPTP_CODE) SomeIpTp_ReceiveFirstSegment
(
  PduIdType RxPduId,
  P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_CONST) PduInfoPtr
)
{
  uint16 chnId = 0U;
  PduLengthType AvailBufLength = 0U;
  SomeIpTp_ErrorType errorChunk = SOMEIPTP_NO_RX_ERROR;
  PduInfoType PduInfo;
  BufReq_ReturnType startOfRecResult;

  /* !LINKSTO SomeIpTp.dsn.ExclusiveArea_0,1 */
  SchM_Enter_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_0();
  if(SOMEIPTP_IDLE != SomeIpTp_RxNSduState[RxPduId].ongoingState)
  {
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00048.ReportError,2 */
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00054.Result_RDUR403,1 */
    SomeIpTp_RxNSduState[RxPduId].ongoingState = SOMEIPTP_IDLE;
    errorChunk = SOMEIPTP_E_INCONSISTENT_SEQUENCE;
  }
  else
  {
    /* No Assembly process for multiple segments is already running */
    /* Start reception of first segment */
  }
  /* !LINKSTO SomeIpTp.dsn.ExclusiveArea_0,1 */
  SchM_Exit_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_0();
  if(SOMEIPTP_NO_RX_ERROR != errorChunk)
  {
    SomeIpTp_InterruptAssemblyProcess(RxPduId,
                                      SOMEIPTP_SID_RXINDICATION,
                                      errorChunk);
    errorChunk = SOMEIPTP_NO_RX_ERROR;
  }
  if(0U == SOMEIPTP_PAYLOAD_MODULUS_BY16(PduInfoPtr->SduLength - SOMEIPTP_HEADER_LENGTH))
  {
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00026,1 */
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00030.Buffer_MetaData,1 */
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00032,1 */
    TS_MemCpy(SomeIpTp_RxNsduElements[RxPduId].MetaData, PduInfoPtr->SduDataPtr, SOMEIP_HEADER_LENGTH);

    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00035,1 */
    SOMEIPTP_CLEAR_TP_FLAG(PduInfoPtr->SduDataPtr[SOMEIPTP_MSG_TYPE_INDEX]);
    chnId = SomeIpTp_RxNsduElements[RxPduId].ChannelId;
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00033.RxTimeout_Start,1 */
    SomeIpTp_RxNSduState[RxPduId].rxTimeoutMFC = SomeIpTp_ChannelElements[chnId].RxTimeoutTimeMFC;
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00033.StartOfReception_Call,1 */
    startOfRecResult= PduR_SomeIpTpStartOfReception(SomeIpTp_RxNsduElements[RxPduId].NSduRxIndId,
                                  0U,
                                  &AvailBufLength);
    if((BUFREQ_OK == startOfRecResult) &&
      (AvailBufLength >= SOMEIP_HEADER_LENGTH))
    {
      PduInfo.SduLength = SOMEIP_HEADER_LENGTH;
      PduInfo.SduDataPtr = PduInfoPtr->SduDataPtr;
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00034.CopyRxDataForHeader,1 */
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00030.Forward_MetaData,1 */
      if((BUFREQ_OK == PduR_SomeIpTpCopyRxData(SomeIpTp_RxNsduElements[RxPduId].NSduRxIndId,
                                  &PduInfo,
                                  &AvailBufLength)) &&
        (AvailBufLength >= (PduInfoPtr->SduLength - SOMEIPTP_HEADER_LENGTH)))
      {
        PduInfo.SduDataPtr = &PduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_LENGTH];
        PduInfo.SduLength = (PduLengthType)(PduInfoPtr->SduLength - SOMEIPTP_HEADER_LENGTH);

        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00034.CopyRxDataForPayload,1 */
        if(BUFREQ_OK == PduR_SomeIpTpCopyRxData(SomeIpTp_RxNsduElements[RxPduId].NSduRxIndId,
                                    &PduInfo,
                                    &AvailBufLength))
        {
          SomeIpTp_RxNSduState[RxPduId].AvailBufLength = AvailBufLength;
          /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00027,1 */
          /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00028,1 */
          /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00036,1 */
          SomeIpTp_RxNSduState[RxPduId].expOffest = SOMEIPTP_CALC_OFFSET(PduInfo.SduLength);
          SomeIpTp_RxNSduState[RxPduId].ongoingState = SOMEIPTP_RX_STARTED;
        }
        else
        {
          /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00050.ReportError,2 */
          /* !BUFREQ_OK "CopyRxData" for received segment payload */
          errorChunk = SOMEIPTP_E_ASSEMBLY_INTERRUPT;
        }
      }
      else
      {
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00050.ReportError,2 */
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00049.bufferSizePtr_CopyRxData,2 */
        /* !BUFREQ_OK or AvailBufLength < payload length "CopyRxData" for SOME/IP header */
        errorChunk = SOMEIPTP_E_ASSEMBLY_INTERRUPT;
      }
    }
    else
    {
      /* if start of reception BUFREQ_OK then the issue is in the AvailBufLength < SOME/IP header length */
      if(BUFREQ_OK == startOfRecResult)
      {
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00049.bufferSizePtr_StartOfReception,2 */
        errorChunk = SOMEIPTP_E_ASSEMBLY_INTERRUPT;
      }
      /* else then the start of reception is not BUFREQ_OK then report runtime error */
      else
      {
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00051.ReportError,2 */
        errorChunk = SOMEIPTP_E_ASSEMBLY_INTERRUPT_SOR;
      }
    }
  }
  else
  {
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00063.ReportError,2 */
    /* Received Payload not dividable by 16 */
    SOMEIPTP_DET_REPORT_RUNTIME_ERROR(SOMEIPTP_SID_RXINDICATION, SOMEIPTP_E_ASSEMBLY_INTERRUPT);
  }
  return errorChunk;
}

STATIC FUNC(SomeIpTp_ErrorType, SOMEIPTP_CODE) SomeIpTp_ReceiveConsecutiveOrLastSegment
(
  PduIdType RxPduId,
  P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_CONST) PduInfoPtr,
  uint8 More
)
{
  uint8 index = 0;
  uint16 chnId = 0U;
  PduInfoType pduInfo;
  PduLengthType offset = 0;
  PduLengthType AvailBufLength = 0U;
  SomeIpTp_ErrorType errorChunk = SOMEIPTP_NO_RX_ERROR;


  /* !LINKSTO SomeIpTp.dsn.ExclusiveArea_0,1 */
  SchM_Enter_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_0();
  if(SOMEIPTP_IDLE != SomeIpTp_RxNSduState[RxPduId].ongoingState)
  {
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00037,2 */
    for(index=0U; index<SOMEIP_HEADER_LENGTH; index++)
    {
      if(SomeIpTp_RxNsduElements[RxPduId].MetaData[index] != PduInfoPtr->SduDataPtr[index])
      {
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00062.ReportError,2 */
        SomeIpTp_RxNSduState[RxPduId].ongoingState = SOMEIPTP_IDLE;
        errorChunk = SOMEIPTP_E_INCONSISTENT_HEADER;
        break;
      }
      else
      {
        /* Header Data is consistent */
        /* Continue */
      }
    }
    if(SOMEIPTP_NO_RX_ERROR == errorChunk)
    {
      /* restart timeout */
      chnId = SomeIpTp_RxNsduElements[RxPduId].ChannelId;
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00037,2 */
      SomeIpTp_RxNSduState[RxPduId].rxTimeoutMFC = SomeIpTp_ChannelElements[chnId].RxTimeoutTimeMFC;
    }
  }
  /* !LINKSTO SomeIpTp.dsn.ExclusiveArea_0,1 */
  SchM_Exit_SomeIpTp_SCHM_SOMEIPTP_EXCLUSIVE_AREA_0();
  if((SOMEIPTP_IDLE == SomeIpTp_RxNSduState[RxPduId].ongoingState) &&
     (SOMEIPTP_NO_RX_ERROR == errorChunk))
  {
    /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00042.ReportError,2 */
    SOMEIPTP_DET_REPORT_RUNTIME_ERROR(SOMEIPTP_SID_RXINDICATION,
                                      SOMEIPTP_E_INCONSISTENT_SEQUENCE);
  }
  else if(SOMEIPTP_NO_RX_ERROR == errorChunk)
  {
    if((1U == More) && (0U != SOMEIPTP_PAYLOAD_MODULUS_BY16(PduInfoPtr->SduLength - SOMEIPTP_HEADER_LENGTH)))
    {
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00063.ReportError,2 */
      errorChunk = SOMEIPTP_E_ASSEMBLY_INTERRUPT;
      /* Received Payload not dividable by 16 */
    }
    else
    {
      offset = SOMEIPTP_GET_OFFSET(PduInfoPtr->SduDataPtr);
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00039,1 */
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00063.ReportError,2 */
      if(offset != SomeIpTp_RxNSduState[RxPduId].expOffest)
      {
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00064.ReportError,2 */
        errorChunk = SOMEIPTP_E_INCONSISTENT_SEQUENCE;
      }
      /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00039,1 */
      else if(SomeIpTp_RxNSduState[RxPduId].AvailBufLength <
                                            (PduInfoPtr->SduLength - SOMEIPTP_HEADER_LENGTH))
      {
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00049.bufferSizePtr_CopyRxData,2 */
        errorChunk = SOMEIPTP_E_ASSEMBLY_INTERRUPT;
      }
      else
      {
        pduInfo.SduDataPtr = &(PduInfoPtr->SduDataPtr[SOMEIPTP_HEADER_LENGTH]);
        pduInfo.SduLength = PduInfoPtr->SduLength - SOMEIPTP_HEADER_LENGTH;
        /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00039,1 */
        if(BUFREQ_OK == PduR_SomeIpTpCopyRxData(SomeIpTp_RxNsduElements[RxPduId].NSduRxIndId,
                                    &pduInfo,
                                    &AvailBufLength))
        {
          SomeIpTp_RxNSduState[RxPduId].AvailBufLength = AvailBufLength;
          if(1U == More) /*Consecutive*/
          {
            /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00027,1 */
            /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00028,1 */
            /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00038,1 */
            SomeIpTp_RxNSduState[RxPduId].expOffest += SOMEIPTP_CALC_OFFSET(pduInfo.SduLength);
          }
          else /*last*/
          {
            /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00040.RxIndication_Call,1 */
            PduR_SomeIpTpRxIndication(SomeIpTp_RxNsduElements[RxPduId].NSduRxIndId,
                                      NTFRSLT_OK);
            /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00040.RxTimeout_Cancel,1 */
            SomeIpTp_ResetRxState(RxPduId);
          }
        }
        else
        {
          /* !LINKSTO SomeIpTp.ASR43.SWS_SomeIpTp_00050.ReportError,2 */
          errorChunk = SOMEIPTP_E_ASSEMBLY_INTERRUPT;
          /* !BUFREQ_OK "CopyRxData" for payload */
        }
      }
    }
  }
  else
  {
    /* SOMEIPTP_E_INCONSISTENT_HEADER occured */
  }
  return errorChunk;
}
#endif /* SOMEIPTP_NO_OF_RXNSDUS > 0U  */

/*==================[external dummy PduR functions definitions]=========================*/
/* dummy function for integration with PduR */
FUNC(Std_ReturnType, SOMEIPTP_CODE) SomeIpTp_PduRIfTransmit
(
  PduIdType TxPduId,
  P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_CONST) PduInfoPtr
)
{

  TS_PARAM_UNUSED(TxPduId);
  TS_PARAM_UNUSED(PduInfoPtr);

#if(SOMEIPTP_DEV_ERROR_DETECT == STD_ON)
  SOMEIPTP_DET_REPORT_ERROR(SOMEIPTP_SID_PDURIFTRANSMIT, SOMEIPTP_E_INVALID_CALL);
#endif /* SOMEIPTP_DEV_ERROR_DETECT == STD_ON */

  return E_NOT_OK;
}


FUNC(void, SOMEIPTP_CODE ) SomeIpTp_PduRTpTxConfirmation
(
    PduIdType PduId,
    NotifResultType Result
)
{
  TS_PARAM_UNUSED(PduId);
  TS_PARAM_UNUSED(Result);

#if(SOMEIPTP_DEV_ERROR_DETECT == STD_ON)
  SOMEIPTP_DET_REPORT_ERROR(SOMEIPTP_SID_PDURTPTXCONFIRMATION, SOMEIPTP_E_INVALID_CALL);
#endif /* SOMEIPTP_DEV_ERROR_DETECT == STD_ON */

  return;
}


FUNC(BufReq_ReturnType, SOMEIPTP_CODE) SomeIpTp_PduRTpCopyTxData
(
   PduIdType PduId,
   P2VAR(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPtr,
   P2VAR(RetryInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) RetryInfoPtr,
   P2VAR(PduLengthType, AUTOMATIC, SOMEIPTP_APPL_DATA) TxDataCntPtr
)
{
  TS_PARAM_UNUSED(PduId);
  TS_PARAM_UNUSED(PduInfoPtr);
  TS_PARAM_UNUSED(RetryInfoPtr);
  TS_PARAM_UNUSED(TxDataCntPtr);

#if(SOMEIPTP_DEV_ERROR_DETECT == STD_ON)
  SOMEIPTP_DET_REPORT_ERROR(SOMEIPTP_SID_PDURTPCOPYTXDATA, SOMEIPTP_E_INVALID_CALL);
#endif /* SOMEIPTP_DEV_ERROR_DETECT == STD_ON */

  return BUFREQ_E_NOT_OK;
}


FUNC(BufReq_ReturnType, SOMEIPTP_CODE) SomeIpTp_PduRTpStartOfReception
(
   PduIdType PduId,
   PduLengthType PduLength,
   P2VAR(PduLengthType, AUTOMATIC, SOMEIPTP_APPL_DATA) RxBufferSizePtr
)
{
  TS_PARAM_UNUSED(PduId);
  TS_PARAM_UNUSED(PduLength);
  TS_PARAM_UNUSED(RxBufferSizePtr);

#if(SOMEIPTP_DEV_ERROR_DETECT == STD_ON)
  SOMEIPTP_DET_REPORT_ERROR(SOMEIPTP_SID_PDURTPSTARTOFRECEPTION, SOMEIPTP_E_INVALID_CALL);
#endif /* SOMEIPTP_DEV_ERROR_DETECT == STD_ON */

  return BUFREQ_E_NOT_OK;
}


FUNC(BufReq_ReturnType, SOMEIPTP_CODE) SomeIpTp_PduRTpCopyRxData
(
   PduIdType PduId,
   P2CONST(PduInfoType, AUTOMATIC, SOMEIPTP_APPL_DATA) PduInfoPointer,
   P2VAR(PduLengthType, AUTOMATIC, SOMEIPTP_APPL_DATA) RxBufferSizePtr
)
{
  TS_PARAM_UNUSED(PduId);
  TS_PARAM_UNUSED(PduInfoPointer);
  TS_PARAM_UNUSED(RxBufferSizePtr);

#if(SOMEIPTP_DEV_ERROR_DETECT == STD_ON)
  SOMEIPTP_DET_REPORT_ERROR(SOMEIPTP_SID_PDURTPCOPYRXDATA, SOMEIPTP_E_INVALID_CALL);
#endif /* SOMEIPTP_DEV_ERROR_DETECT == STD_ON */

  return BUFREQ_E_NOT_OK;
}


FUNC(void, SOMEIPTP_CODE) SomeIpTp_PduRTpRxIndication
(
    PduIdType RxPduId,
    NotifResultType Result
)
{
  TS_PARAM_UNUSED(RxPduId);
  TS_PARAM_UNUSED(Result);

#if(SOMEIPTP_DEV_ERROR_DETECT == STD_ON)
  SOMEIPTP_DET_REPORT_ERROR(SOMEIPTP_SID_PDURTPRXINDICATION, SOMEIPTP_E_INVALID_CALL);
#endif /* SOMEIPTP_DEV_ERROR_DETECT == STD_ON */

  return;
}


#define SOMEIPTP_STOP_SEC_CODE
#include <SomeIpTp_MemMap.h>

/*==================[end of file]===========================================*/
