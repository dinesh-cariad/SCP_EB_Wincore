#ifndef DCCM_TYPES_H
#define DCCM_TYPES_H

/***************************************************************************
 *
 * \file Dccm_Types.h
 *
 * \brief Diagnostic Client Communication Manager implementation of Dccm
 *
 * Copyright 2020 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* MISRA-C:2012 Deviation List
 *
 */

/*==================[inclusions]=============================================*/
#include <Std_Types.h>                            /* AUTOSAR standard types */

/**
 * \brief This type contains all Dccm Diagnostic Protocol result values,
 * which can be reported via the callback method.
 */
typedef uint8 Dccm_DiagProtocolResponseCodeType;

/**
 * \brief This type is used to identify the diagnostic protocol,
 */
typedef uint8 Dccm_ProtocolIdType;

/**
 *
 * \brief Status of a diagnostic protocol.
 *
 */
typedef uint8 Dccm_DiagnosticProtocolStatusType;

/**
 *
 * \brief Type for timeout counter.
 *
 */
typedef uint32 Dccm_TimeoutType;

/*==================[type definitions]=======================================*/

/**
 *
 * \brief Diagnostic Protocol Callback type to notify SW-Manager.
 *
 * \param[in]  ProtocolId Signalize the diagnostic protocol for what the response was
 *         received.
 * \param[in]  ResponseCode Signalize the status of the requested service.
 *
 */

typedef P2FUNC(void, DCCM_CODE, Dccm_CallbackType)
(
  Dccm_ProtocolIdType ProtocolId,
  Dccm_DiagProtocolResponseCodeType ResponseCode
);

/**
 *
 * \brief Diagnostic Protocol Callback type to request next chunk of data in case Buffer Streaming is enabled.
 *
 * \param[in]      ProtocolId The protocol id of the buffer request
 * \param[out]     Buffer Buffer pointing to the next chunk of data.
 * \param[in]      StartLocation The requested start of the data chuck.
 * \param[in,out]  AvailableDataPtr I/O parameter. Input: the requested size of data, Output: the length of the next chunk of data.
 * \param[in]      uint8 RetryInformation Can have the values from the enum TpDataStateType: TP_DATARETRY, TP_DATACONF or TP_CONFPENDING.
 *                 If the parameter RetryInfoPtr of the function Dccm_CopyTxData() is NULL_PTR, this member will have
 *                 the value DCCM_RETRY_INFO_NULL.
 */
typedef P2FUNC(Std_ReturnType, DCCM_CODE, Dccm_BufferStreamingCallbackType)
(
  Dccm_ProtocolIdType ProtocolId,
  P2VAR(uint8, AUTOMATIC, DCCM_APPL_DATA) Buffer,
  uint16 StartLocation,
  P2VAR(PduLengthType, AUTOMATIC, DCCM_APPL_DATA) AvailableDataPtr,
  uint8 RetryInformation
);



#endif /* DCCM_H */
/*==================[end of file]============================================*/
