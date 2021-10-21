/**
 * \file
 *
 * \brief AUTOSAR FrArTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrArTp.
 *
 * \version 1.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef FRARTP_SDUMNG_INT_H
#define FRARTP_SDUMNG_INT_H


 /*==================[includes]================================================*/

#include <FrArTp_Int.h>            /* FrArTp generic interfaces */

/*==================[macros]================================================*/

/*==================[type definitions]=========================*/

/*==================[external data declarations]=========================*/

/*==================[external function declarations]=========================*/

/*
 * Start code section declaration
 */
#define FRARTP_START_SEC_CODE
#include <FrArTp_MemMap.h>

/**
 * \brief indicate start of reception
 *
 * \param[in] connIdx         Connection index
 * \param[in] TpSduLength     Total length of Sdu to receive
 * \param[out] BufferSizePtr  Address to write the current buffer length to
 *
 * \return    function call result
 */
TS_MOD_PRIV_DECL FUNC(BufReq_ReturnType, FRARTP_CODE) FrArTp_SduMng_startOfReception
(
  FrArTp_ConnIdxType connIdx,
  PduLengthType TpSduLength,
  P2VAR(PduLengthType, AUTOMATIC, FRARTP_APPL_DATA) BufferSizePtr
);



/**
 * \brief copy received data
 *
 * \param[in] connIdx         Connection index
 * \param[in] PduInfoPtr      Information about received data
 * \param[out] BufferSizePtr  Address to write the current buffer length to
 *
 * \return    function call result
 */
TS_MOD_PRIV_DECL FUNC(BufReq_ReturnType, FRARTP_CODE) FrArTp_SduMng_copyRxData
(
  FrArTp_ConnIdxType connIdx,
  P2CONST(PduInfoType, AUTOMATIC, FRARTP_APPL_CONST) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, FRARTP_APPL_DATA) BufferSizePtr
);



/**
 * \brief receive result indication
 *
 * \param[in] connIdx     Connection index
 * \param[in] Result      Result of reception
 */
TS_MOD_PRIV_DECL FUNC(void, FRARTP_CODE) FrArTp_SduMng_rxIndication
(
  FrArTp_ConnIdxType connIdx,
  NotifResultType Result
);



/**
 * \brief get data for transmission
 *
 * \param[in] connIdx             Connection index
 * \param[in/out] PduInfoPtr      Information where to put transmit data to/amount of data to transmit
 * \param[in] RetryInfoPtr        Transmission retry information (wind back)
 * \param[out] AvailableDataPtr   Address to write the number of bytes available in transmit buffer to
 */
TS_MOD_PRIV_DECL FUNC(BufReq_ReturnType, FRARTP_CODE) FrArTp_SduMng_copyTxData
(
  FrArTp_ConnIdxType connIdx,
  P2VAR(PduInfoType, AUTOMATIC, FRARTP_APPL_DATA) PduInfoPtr,
  P2VAR(RetryInfoType, AUTOMATIC, FRARTP_APPL_DATA) RetryInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, FRARTP_APPL_DATA) AvailableDataPtr
);



/**
 * \brief transmission result confirmation indication
 *
 * \param[in] connIdx       Connection index
 * \param[in] Result        Transmission result information
 */
TS_MOD_PRIV_DECL FUNC(void, FRARTP_CODE) FrArTp_SduMng_txConfirmation
(
  FrArTp_ConnIdxType connIdx,
  NotifResultType Result
);

/*
 * Stop code section declaration
 */
#define FRARTP_STOP_SEC_CODE
#include <FrArTp_MemMap.h>


#endif /* ifndef FRARTP_SDUMNG_INT_H */
/*==================[end of file]===========================================*/
