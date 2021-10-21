/**
 * \file
 *
 * \brief AUTOSAR DoIP
 *
 * This file contains the implementation of the AUTOSAR
 * module DoIP.
 *
 * \version 1.1.21
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef DOIP_CUSTOMPAYLOADTYPE_H
#define DOIP_CUSTOMPAYLOADTYPE_H

/*==================[includes]==============================================*/

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/
#define DOIP_START_SEC_CODE
#include <DoIP_MemMap.h>

#if (DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON)
/** \brief Internal service for handling of custom message generic header
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] PayloadType - Received payload type.
 * \param[in] PayloadLength - Received payload length.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
FUNC(void, DOIP_CODE) DoIP_HandleCustomMsg
(
  DoIP_TcpConIdxType TcpConIdx,
  uint16 PayloadType,
  uint32 PayloadLength
);

/** \brief Internal service to fill custom message generic header into provided buffer
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[out] SduDataPtr - Pointer where message should be filled.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
FUNC(void, DOIP_CODE) DoIP_FillCustomMsgHdr
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) SduDataPtr
);

/** \brief Internal service to fill custom message payload into provided buffer
 *
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[out] PduInfoPtr - Pointer where Tx message payload shall be stored.
 * \param[out] AvailableDataPtr - Pointer to the remaining number of bytes available in PduR.

 * \return BufReq_ReturnType
 * \retval BUFREQ_OK - Copy was successful
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_FillCustomMsgPld
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr,
  P2VAR(PduLengthType, AUTOMATIC, DOIP_APPL_DATA) AvailableDataPtr
);

/** \brief Internal service for handling of custom message payload
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] PduInfoPtr - Pointer to the diagnostic message.
 *
 * \return BufReq_ReturnType
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
FUNC(BufReq_ReturnType, DOIP_CODE) DoIP_CopyCustomMsgPayload
(
  DoIP_TcpConIdxType TcpConIdx,
  P2CONST(PduInfoType, AUTOMATIC, DOIP_APPL_DATA) PduInfoPtr
);

/** \brief Internal service to initialize custom channels when Tcp connection gets established
 *
 * \param[in] TcpConIdx - Index of Tcp connection that got established.
 * \param[in] DoIPSoAdPduId - DoIPSoAdPduId of established Tcp connection.
 *
*/
FUNC(void, DOIP_CODE) DoIP_PrepareTcpConCustom
(
  DoIP_TcpConIdxType TcpConIdx,
  PduIdType DoIPSoAdPduId
);

/** \brief Internal service to handle custom channel during Tcp connection reset
 *
 * \param[in] TcpConIdx - Index of Tcp connection being reset.
 *
*/
FUNC(void, DOIP_CODE) DoIP_ResetTcpConCustom
(
  DoIP_TcpConIdxType TcpConIdx
);

#endif /* DOIP_CUSTOM_PAYLOAD_TYPE_ENABLED == STD_ON */

/** \brief Internal service to clear specified buffer
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
FUNC(void, DOIP_CODE) DoIP_ClearBuffer
(
  DoIP_TcpConIdxType TcpConIdx
);

/** \brief Internal service to release (free) buffer
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
FUNC(void, DOIP_CODE) DoIP_ReleaseBuffer
(
  DoIP_TcpConIdxType TcpConIdx
);

/** \brief Internal service to copy data into buffer
 *
 *  Specified data block is coped into buffer and buffer write pointer is updated accordingly.
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] DataPtr - Address of a data that should be copied into buffer.
 * \param[in] Size - Size of a data that should be copied.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
FUNC(void, DOIP_CODE) DoIP_WriteBuffer
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) DataPtr,
  PduLengthType Size
);

/** \brief Internal service to copy data from buffer
 *
 *  Requested block of data is coped from buffer into specified memory location, and
 *  buffer read pointer is updated accordingly.
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 * \param[in] DataPtr - Address where data should be copied.
 * \param[in] Size - Size of a data that should be copied.
 *
 * \return PduLengthType - Remaining size of data in buffer after reading.
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
FUNC(PduLengthType, DOIP_CODE) DoIP_ReadBuffer
(
  DoIP_TcpConIdxType TcpConIdx,
  P2VAR(uint8, AUTOMATIC, DOIP_APPL_DATA) DataPtr,
  PduLengthType Size
);

/** \brief Internal service to return size of a data stored in the buffer
 *
 * \param[in] TcpConIdx - Index of the used TCP connection.
 *
 * \return PduLengthType - Size of data stored in the buffer.
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
FUNC(PduLengthType, DOIP_CODE) DoIP_GetBufferDataSize
(
  DoIP_TcpConIdxType TcpConIdx
);

/** \brief Internal service to return a pointer to a data in buffer
 *
 * \param[in] TcpConIdx - Id of a buffer to be accessed.
 *
 * \return Address of data in the buffer
 *
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
*/
FUNC_P2VAR(uint8, DOIP_APPL_DATA, DOIP_CODE) DoIP_GetBufferDataPtr
(
  DoIP_TcpConIdxType TcpConIdx
);

#define DOIP_STOP_SEC_CODE
#include <DoIP_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#endif /* DOIP_CUSTOMPAYLOADTYPE_H */
/*==================[end of file]===========================================*/

