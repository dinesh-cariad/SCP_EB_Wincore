/**
 * \file
 *
 * \brief AUTOSAR Tls
 *
 * This file contains the implementation of the AUTOSAR
 * module Tls.
 *
 * \version 1.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* This file contains the implementation of the Transport Layer Security Module. */

/*
 * Misra-C:2012 Deviations:
 *
 * MISRAC2012-1) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.

 * Reason:
 * 1) AUTOSAR defines a generic type which dependent on a member of the generic type
 * implements an concrete type. Thus for usage of this concrete type, a cast is necessary.
 */

/*===============================[includes]======================================================*/

#include <TcpIp.h>
/* !LINKSTO Tls.API.Callout.Upperlayer,1 */
#include <TcpIp_Lcfg.h>
#include <TSMem.h>
#include <Tls_Int.h>
#include <Tls_Cfg.h>
#include <Tls_Lcfg.h>
#include <Tls_Debug.h>
#include <Tls_Encode_Int.h>
#include <Tls_Decode_Int.h>

#if(TLS_DEV_ERROR_DETECT == STD_ON)
#include <Det.h> /* Det API */
#endif /* TLS_DEV_ERROR_DETECT == STD_ON */

/*===========================[macro definitions]=================================================*/

#if TLS_DEV_ERROR_DETECT == STD_ON
#define TLS_DET_REPORT_ERROR(ApiId, ErrorId) \
    ((void)Det_ReportError(TLS_MODULE_ID, 0u, (ApiId), (ErrorId)))
#endif /* TLS_DEV_ERROR_DETECT == STD_ON */

/** \brief Length of the "key expansion" string */
#define TLS_KEYEXPANSION_STRING_LENGTH                     (13U)
/** \brief Length of the "master secret" string */
#define TLS_MASTERSECRET_STRING_LENGTH                     (13U)

/** \brief Size of the nonce used for AEAD encryption and decryption. This includes explicit and implicit nonce. */
#define TLS_AEAD_NONCE_LENGTH                              (12U)
/** \brief Size of the explicit nonce prepended to the record payload for AEAD encrypted record */
#define TLS_AEAD_EXPLICIT_NONCE_LENGTH                      (8U)

/** \brief Number of cipher suites in the Tls_CipherSuites array */
#define TLS_CIPHERSUITE_COUNT                               (4U)

/** \brief MAC key length for cipher suite TLS_PSK_WITH_AES_128_GCM_SHA256. Does not have a MAC key. */
#define TLS_PSK_WITH_AES_128_GCM_SHA256_MAC_KEY_LENGTH      (0u)
/* !LINKSTO Tls.Ciphers.TLS_PSK_WITH_AES_128_GCM_SHA256.MAC,1 */
/** \brief Record MAC length for cipher suite TLS_PSK_WITH_AES_128_GCM_SHA256 */
#define TLS_PSK_WITH_AES_128_GCM_SHA256_MAC_LENGTH         (16u)
/** \brief Fixed IV length for cipher suite TLS_PSK_WITH_AES_128_GCM_SHA256 */
#define TLS_PSK_WITH_AES_128_GCM_SHA256_IV_LENGTH           (4u)
/* !LINKSTO Tls.Ciphers.TLS_PSK_WITH_AES_128_GCM_SHA256.ENCKEY,1 */
/** \brief Encryption key length for cipher suite TLS_PSK_WITH_AES_128_GCM_SHA256 */
#define TLS_PSK_WITH_AES_128_GCM_SHA256_KEY_LENGTH         (16u)

/* !LINKSTO Tls.Ciphers.TLS_PSK_WITH_NULL_SHA256.MACKEY,1 */
/** \brief MAC key length for cipher suite TLS_PSK_WITH_NULL_SHA256 */
#define TLS_PSK_WITH_NULL_SHA256_MAC_KEY_LENGTH            (32u)
/* !LINKSTO Tls.Ciphers.TLS_PSK_WITH_NULL_SHA256.MAC,1 */
/** \brief Record MAC length for cipher suite TLS_PSK_WITH_NULL_SHA256 */
#define TLS_PSK_WITH_NULL_SHA256_MAC_LENGTH                (32u)
/** \brief Fixed IV length for cipher suite TLS_PSK_WITH_NULL_SHA256. Does not have an IV. */
#define TLS_PSK_WITH_NULL_SHA256_IV_LENGTH                  (0u)
/** \brief Encryption key length for cipher suite TLS_PSK_WITH_NULL_SHA256. Does not have an encryption key. */
#define TLS_PSK_WITH_NULL_SHA256_KEY_LENGTH                 (0u)

/** \brief MAC key length for cipher suite TLS_ECDHE_ECDSA_WITH_NULL_SHA */
#define TLS_ECDHE_ECDSA_WITH_NULL_SHA_MAC_KEY_LENGTH        (20U)
/** \brief Record MAC length for cipher suite TLS_ECDHE_ECDSA_WITH_NULL_SHA */
#define TLS_ECDHE_ECDSA_WITH_NULL_SHA_MAC_LENGTH            (20U)
/** \brief Fixed IV length for cipher suite TLS_ECDHE_ECDSA_WITH_NULL_SHA. Does not have an IV. */
#define TLS_ECDHE_ECDSA_WITH_NULL_SHA_IV_LENGTH             (0U)
/** \brief Encryption key length for cipher suite TLS_ECDHE_ECDSA_WITH_NULL_SHA. Does not have an encryption key. */
#define TLS_ECDHE_ECDSA_WITH_NULL_SHA_KEY_LENGTH            (0U)

/** \brief MAC key length for cipher suite TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256. Does not have a MAC key. */
#define TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256_MAC_KEY_LENGTH (0U)
/** \brief Record MAC length for cipher suite TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 */
#define TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256_MAC_LENGTH     (16U)
/** \brief Fixed IV length for cipher suite TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 */
#define TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256_IV_LENGTH      (4U)
/** \brief Encryption key length for cipher suite TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256 */
#define TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256_KEY_LENGTH     (16U)

/** \brief Minimum length of a TLS ClientHello payload */
#define TLS_CLIENTHELLO_MIN_LENGTH (/*version*/2u + TLS_RANDOM_SIZE + /*session id*/1u + /*cipher suites*/2u+2u + /*compression*/1u)

/** \brief Minimum length of a DTLS ClientHello payload */
#define TLS_DTLS_CLIENTHELLO_MIN_LENGTH (TLS_CLIENTHELLO_MIN_LENGTH + 1u)

/** \brief Minimum length of a ServerHello payload */
#define TLS_SERVERHELLO_MIN_LENGTH (/*version*/2u + TLS_RANDOM_SIZE + /*session id*/1u + /*cipher suite*/2u + /*compression*/1u)

/** \brief Length of an Alert record */
#define TLS_ALERT_LENGTH                                    (2u)

/** \brief Size of Handshake Protocol fields */
/** \brief size of TLS/DTLS version */
#define TLS_VERSION_SIZE                                    (2u)
/** \brief Size of ciphersuite Length field */
#define TLS_CIPHERSUITE_LENGTH_SIZE                         (2u)
/** \brief Size of Compression Method length fields */
#define TLS_COMPRESSIONMETHOD_LENGTH_SIZE                   (1u)
/** \brief Size of Extensions header field */
#define TLS_EXTENSIONS_HEADER_SIZE                          (2u)
/** \brief Size of Extension Type field */
#define TLS_EXTENSION_TYPE_SIZE                             (2u)
/** \brief Size of Extension Length field */
#define TLS_EXTENSION_LENGTH_SIZE                           (2u)
/** \brief Size of extension: Record_size_limit */
#define TLS_EXTENSION_RECORDSIZELIMIT_SIZE                  (2u)

/** \brief Get the Extension type. */
#define TLS_GET_EXTENSION_TYPE(payloadPtr, idx) \
  ( (uint16)(((uint16)(payloadPtr)[(idx)] << 8u) + (uint16)(payloadPtr)[(idx) + 1u]) );

/** \brief Get the Extension length. */
#define TLS_GET_EXTENSION_LENGTH(payloadPtr, idx) \
  ( (uint16)(((uint16)(payloadPtr)[(idx) + 2u] << 8u) + (uint16)(payloadPtr)[(idx) + 3u]) );

/** \brief Get the Extension pointer. */
#define TLS_GET_EXTENSION_BUFF_PTR(payloadPtr, idx) \
  ( &(payloadPtr)[(idx) + 4u] );

/** \brief Compression method: NULL */
#define TLS_COMPRESSION_METHOD_NULL                         (0u)

/** \brief Extension: Record_size_limit */
#define TLS_EXTENSION_RECORDSIZELIMIT_ID               (0x001CU)

/** \brief record size limits */
#define TLS_MIN_RECORD_SIZE                                (64U)
#define TLS_MAX_RECORD_SIZE                             (16384U)

/** \brief HSM algorithm length */
#define TLS_HSM_ALGORITHM_LEN                               (1U)

/** \brief HSM algorithm */
#define TLS_CSM_KEYDERIVE_ALGO_INVALID                      (0xFFU)
#define TLS_CSM_KEYDERIVE_ALGO_AES_128_GCM_SHA256           (0x00U)
#define TLS_CSM_KEYDERIVE_ALGO_NULL_SHA256                  (0x01U)

/** \brief Crypto elements for Key derivation */
#define TLS_CRYPTO_KE_KEYDERIVATION_TLS_PSK_RANDOM_CLIENT   (1101U)
#define TLS_CRYPTO_KE_KEYDERIVATION_TLS_PSK_RANDOM_SERVER   (1102U)

/*=====================[external constants declarations]=========================================*/

/*=====================[internal constants declarations]=========================================*/
#define TLS_START_SEC_CONST_UNSPECIFIED
#include <Tls_MemMap.h>

STATIC CONST(Tls_CipherSuiteParametersType, TLS_CONST) Tls_CipherSuites[TLS_CIPHERSUITE_COUNT] =
{
    /* !LINKSTO Tls.TLS_PSK_WITH_NULL_SHA256,1 */
    {
        TLS_PSK_WITH_NULL_SHA256, TLS_NULL_CIPHER, TLS_PSK_WITH_NULL_SHA256_MAC_KEY_LENGTH,
        TLS_PSK_WITH_NULL_SHA256_MAC_LENGTH, TLS_PSK_WITH_NULL_SHA256_IV_LENGTH,
        TLS_PSK_WITH_NULL_SHA256_KEY_LENGTH
    },
    /* !LINKSTO Tls.TLS_PSK_WITH_AES_128_GCM_SHA256,1 */
    {
        TLS_PSK_WITH_AES_128_GCM_SHA256, TLS_AEAD_CIPHER, TLS_PSK_WITH_AES_128_GCM_SHA256_MAC_KEY_LENGTH,
        TLS_PSK_WITH_AES_128_GCM_SHA256_MAC_LENGTH, TLS_PSK_WITH_AES_128_GCM_SHA256_IV_LENGTH,
        TLS_PSK_WITH_AES_128_GCM_SHA256_KEY_LENGTH
    },
    {
        /* cipher */ TLS_ECDHE_ECDSA_WITH_NULL_SHA,
        /* cipherType */ TLS_NULL_CIPHER,
        /* MACKeyLength */ TLS_ECDHE_ECDSA_WITH_NULL_SHA_MAC_KEY_LENGTH,
        /* MACLength */ TLS_ECDHE_ECDSA_WITH_NULL_SHA_MAC_LENGTH,
        /* IVLength */ TLS_ECDHE_ECDSA_WITH_NULL_SHA_IV_LENGTH,
        /* encKeyLength */ TLS_ECDHE_ECDSA_WITH_NULL_SHA_KEY_LENGTH
    },
    {
        /* cipher */ TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256,
        /* cipherType */ TLS_AEAD_CIPHER,
        /* MACKeyLength */ TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256_MAC_KEY_LENGTH,
        /* MACLength */ TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256_MAC_LENGTH,
        /* IVLength */ TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256_IV_LENGTH,
        /* encKeyLength */ TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256_KEY_LENGTH
    }
};

#define TLS_STOP_SEC_CONST_UNSPECIFIED
#include <Tls_MemMap.h>

#define TLS_START_SEC_CONST_8
#include <Tls_MemMap.h>

/** \brief "server finished" string */
STATIC CONST(uint8, TLS_CONST) Tls_String_ServerFinished[TLS_SERVERFINISHED_STRING_LENGTH] =
  {'s', 'e', 'r', 'v', 'e', 'r', ' ', 'f', 'i', 'n', 'i', 's', 'h', 'e', 'd'};
/** \brief "client finished" string */
STATIC CONST(uint8, TLS_CONST) Tls_String_ClientFinished[TLS_CLIENTFINISHED_STRING_LENGTH] =
  {'c', 'l', 'i', 'e', 'n', 't', ' ', 'f', 'i', 'n', 'i', 's', 'h', 'e', 'd'};

#if( TLS_CSM_KEYDERIVE_SUPPORT == STD_OFF )
/** \brief The "master secret" string */
STATIC CONST(uint8, TLS_CONST) Tls_String_MasterSecret[TLS_MASTERSECRET_STRING_LENGTH] =
  {'m', 'a', 's', 't', 'e', 'r', ' ', 's', 'e', 'c', 'r', 'e', 't'};
/** \brief The "key expansion" string */
STATIC CONST(uint8, TLS_CONST) Tls_String_KeyExpansion[TLS_KEYEXPANSION_STRING_LENGTH] =
  {'k', 'e', 'y', ' ', 'e', 'x', 'p', 'a', 'n', 's', 'i', 'o', 'n'};
#endif /* TLS_CSM_KEYDERIVE_SUPPORT == STD_OFF */

#define TLS_STOP_SEC_CONST_8
#include <Tls_MemMap.h>

/*==================[internal function declarations]=============================================*/
#define TLS_START_SEC_CODE
#include <Tls_MemMap.h>

/** \brief Find the TLS connection context for a socket
 *
 *
 * \param[in] SocketId     The SocketId for which the TLS context shall be searched
 *
 * \return Pointer to the connection context or NULL_PTR if none was found
 */
STATIC FUNC(Tls_PtrConnection, TLS_CODE) Tls_FindConnection(TcpIp_SocketIdType SocketId);

/** \brief Callback for completion of the Csm_RandomGenerate job
 *
 *
 * \param[in] ConnectionPtr     Pointer to the TLS connection context
 * \param[in] result            The result value of the job
 *
 */
STATIC FUNC(void, TLS_CODE) Tls_RandomCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result);

/** \brief Copies an IP address
 *
 * Copies the given IP address to connection context to use it as remote address.
 *
 * \param[in] ConnectionPtr     Pointer to the TLS connection context
 * \param[in] RemoteAddrPtr     The address to copy
 *
 * \return E_OK if successful, E_NOT_OK otherwise
 */
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_CopyIpAddress(Tls_PtrConnection ConnectionPtr, TcpIp_ConstSockAddrPtrType RemoteAddrPtr);

/** \brief Wrapper for TcpIp_GetSocketOwner
 *
 *
 * \param[in] ConnectionPtr     Pointer to the TLS connection context
 *
 * \return The socket owner index of the upper layer
 */
STATIC FUNC(uint8, TLS_CODE) Tls_GetSocketOwner(Tls_PtrConnection ConnectionPtr);

/** \brief Wrapper for TcpIp_CopyTxDataAPI
 *
 *
 * \param[in] ConnectionPtr     Pointer to the TLS connection context
 * \param[in] BufPtr            Pointer to the buffer to fill with data
 * \param[in] BufLength         Size of the buffer
 *
 * \return E_OK if successful, E_NOT_OK otherwise
 */
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_UL_CopyTxData(Tls_PtrConnection ConnectionPtr, P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) BufPtr, uint16 BufLength);

/** \brief Wrapper for TcpIp_TcpIpEventAPI
 *
 *
 * \param[in] ConnectionPtr     Pointer to the TLS connection context
 * \param[in] Event             The TcpIp event
 *
 */
STATIC FUNC(void, TLS_CODE) Tls_UL_TcpIpEvent(Tls_PtrConnection ConnectionPtr, TcpIp_EventType Event);

/** \brief Close notify upper layer
 *
 * Tells the upper layer that it can close the socket
 *
 * \param[in] ConnectionPtr     Pointer to the TLS connection context
 *
 */
STATIC FUNC(void, TLS_CODE) Tls_UL_Closed(Tls_PtrConnection ConnectionPtr);

/** \brief Wrapper for TcpIp_TxConfirmationAPI
 *
 *
 * \param[in] ConnectionPtr     Pointer to the TLS connection context
 * \param[in] Length            Size of the buffer
 *
 */
STATIC FUNC(void, TLS_CODE) Tls_UL_TxConfirmation(Tls_PtrConnection ConnectionPtr, uint16 Length);

/** \brief Get the TLS version of a connection
 *
 * Returns 0x0303 for TLS(1.2) connections and 0xFEFD for DTLS(1.2) connections.
 *
 * \param[in] ConnectionPtr     Pointer to the TLS connection context
 *
 * \return The TLS/DTLS version
 */
STATIC FUNC(uint16, TLS_CODE) Tls_GetVersion(Tls_PtrConstConnection ConnectionPtr);

 /** \brief Get the length of a TLS/DTLS record
  *
  * Reads the length field of a TLS/DTLS record.
  *
  * \param[in] ConnectionPtr     Pointer to the TLS connection context
  * \param[in] record            Pointer to the beginning of TLS/DTLS record header
  *
  * \return The length in bytes of the payload excluding the header
  */
STATIC FUNC(uint16, TLS_CODE) Tls_GetRecordLength(Tls_PtrConstConnection ConnectionPtr, Tls_PtrConstUint8 record);

 /** \brief Set the length of a TLS/DTLS record
  *
  * Sets the length field of a TLS/DTLS record.
  *
  * \param[in]  ConnectionPtr    Pointer to the TLS connection context
  * \param[out] record           Pointer to the beginning of TLS/DTLS record header
  * \param[in]  length           Length to set
  *
  */
STATIC FUNC(void, TLS_CODE) Tls_SetRecordLength(Tls_PtrConstConnection ConnectionPtr, Tls_PtrUint8 record, uint16 length);

 /** \brief Get the version of a TLS/DTLS record
  *
  * Gets the version field of a TLS/DTLS record.
  *
  * \param[in] record           Pointer to the beginning of TLS/DTLS record header
  *
  * \return The version field
  */
STATIC FUNC(uint16, TLS_CODE) Tls_GetRecordVersion(Tls_PtrConstUint8 record);

 /** \brief Get the type of a TLS/DTLS record
  *
  * Gets the type field of a TLS/DTLS record.
  *
  * \param[in] record           Pointer to the beginning of TLS/DTLS record header
  *
  * \return The type field
  */
STATIC FUNC(uint8, TLS_CODE) Tls_GetRecordType(Tls_PtrConstUint8 record);

 /** \brief Get the overhead of a TLS/DTLS record
  *
  * Gets the overhead in bytes that is added to payload data for each record.
  * This includes the header and overhead that is added by the cipher suite like (explicit) IV and MAC.
  *
  * \param[in] ConnectionPtr    Pointer to the TLS connection context
  *
  * \return The overhead in bytes
  */
 STATIC FUNC(uint16, TLS_CODE) Tls_GetRecordOverhead(Tls_PtrConnection ConnectionPtr);

 /** \brief Changes the state of the main internal state machine
  *
  *
  * \param[in] ConnectionPtr    Pointer to the TLS connection context
  * \param[in] NewState         The new state to set
  *
  */
STATIC FUNC(void, TLS_CODE) Tls_SetState(Tls_PtrConnection ConnectionPtr, Tls_StateType NewState);

/** \brief Writes a DTLS record header
 *
 * Writes a DTLS record header to the given buffer.
 *
 * \param[out] dest             Pointer to the beginning of the DTLS record header
 * \param[in] type              Value for the type field
 * \param[in] version           Value for the version field
 * \param[in] length            Value for the length field
 * \param[in] epoch             Value for the epoch field
 * \param[in] seq_num           Value for the seq_num field
 *
 */
STATIC FUNC(void, TLS_CODE) Tls_Dtls_WriteRecordHeader(Tls_PtrUint8 dest, uint8 type, uint16 version, uint32 length, uint16 epoch, Tls_Uint64Type seq_num);

/** \brief Checks if there is enough space in the TxConfirmation buffer
 *
 *
 * \param[in] ConnectionPtr    Pointer to the TLS connection context
 * \param[in] count            Number of free entries required
 *
 * \return TRUE if there is enough space, FALSE otherwise
 */
STATIC FUNC(boolean, TLS_CODE) Tls_TxConfirmation_Check(Tls_PtrConnection ConnectionPtr, uint8 count);

/** \brief Queues a record item to the TxConfirmation buffer
 *
 *
 * \param[in] ConnectionPtr    Pointer to the TLS connection context
 * \param[in] size             Total length of the transmitted record
 * \param[in] type             Type of the transmitted record
 *
 */
STATIC FUNC(void, TLS_CODE) Tls_TxConfirmation_Enqueue(Tls_PtrConnection ConnectionPtr, uint16 size, uint8 type);

/** \brief Dequeues bytes from the TxConfirmation buffer
 *
 * The TxConfirmation buffer contains a list of records that where sent.
 * This function counts the number of bytes to dequeue and returns the size of
 * the record if the counter is greater or equal than the size of the first record in the queue.
 *
 * \param[in] ConnectionPtr    Pointer to the TLS connection context
 * \param[in] size             Number of bytes to dequeue
 *
 * \return Size of the record that was dequeued or 0 if no record was complete
 */
STATIC FUNC(uint16, TLS_CODE) Tls_TxConfirmation_Dequeue(Tls_PtrConnection ConnectionPtr, uint16 size);

/** \brief Transmits data from the encryption buffer
 *
 * Passes data to TcpIp_TcpTransmit for TLS and TcpIp_UdpTransmit for DTLS.
 *
 * \param[in] ConnectionPtr    Pointer to the TLS connection context
 * \param[in] AvailableLength  Number of bytes to transmit
 *
 * \return Result from TcpIp_TcpTransmit or TcpIp_UdpTransmit or E_NOT_OK if something went wrong before calling TcpIp
 */
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_TcpIp_Transmit(Tls_PtrConnection ConnectionPtr, uint32 AvailableLength);

/** \brief Writes a TLS record header
 *
 *
 * \param[out] dest            Pointer to the beginning of the TLS record to be written
 * \param[in] type             Value for the type field
 * \param[in] version          Value for the version field
 * \param[in] length           Value for the length field
 *
 */
STATIC FUNC(void, TLS_CODE) Tls_WriteRecordHeader(Tls_PtrUint8 dest, uint8 type, uint16 version, uint32 length);

/** \brief Initializes a Tls_BufferType
 *
 * Initializes the buffer using the given memory pointer and length.
 *
 * \param[out] BufferPtr       Pointer to the Tls_BufferType
 * \param[in] BufPtr           Pointer to the buffer to use as storage
 * \param[in] Length           Length of the storage
 *
 */
STATIC FUNC(void, TLS_CODE) Tls_Buffer_Init(Tls_PtrBuffer BufferPtr, P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) BufPtr, uint16 Length);

/** \brief Appends data to a buffer
 *
 *
 * \param[out] BufferPtr       Pointer to the Tls_BufferType
 * \param[in] BufPtr           Pointer to the data to append
 * \param[in] Length           Length of the data
 *
 * \return E_OK if successfull, E_NOT_OK otherwise
 */
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Buffer_Append(Tls_PtrBuffer BufferPtr, P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) BufPtr, uint16 Length);

/** \brief Gets the number of free bytes in a buffer
 *
 *
 * \param[in] BufferPtr        Pointer to the Tls_BufferType
 *
 * \return The number of free bytes
 */
STATIC FUNC(uint16, TLS_CODE) Tls_Buffer_GetFreeBytes(Tls_PtrBuffer BufferPtr);

/** \brief Copies data from the buffer
 *
 *
 * \param[in] BufferPtr        Pointer to the source Tls_BufferType
 * \param[out] BufPtr          The destination of the copy operation
 * \param[in] Start            The start position inside the buffer
 * \param[in] Length           The length of data to be copied
 *
 * \return E_OK if successfull, E_NOT_OK otherwise
 */
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Buffer_Copy(Tls_PtrBuffer BufferPtr, Tls_PtrUint8 BufPtr, uint16 Start, uint16 Length);

/** \brief Consumes bytes from a buffer
 *
 * Removes the given number of bytes from beginning of the buffer.
 *
 * \param[in,out] BufferPtr    Pointer to the source Tls_BufferType
 * \param[in] Length           The number of bytes to remove
 *
 * \return E_OK if successfull, E_NOT_OK otherwise
 */
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Buffer_Consume(Tls_PtrBuffer BufferPtr, uint16 Length);

/** \brief Consumes a record from a buffer
 *
 * Removes one record from the beginning of the given buffer.
 *
 * \param[in] ConnectionPtr    Pointer to the TLS connection context. Used to determine TLS/DTLS.
 * \param[in,out] BufferPtr    Pointer to the Tls_BufferType
 *
 * \return E_OK if successfull, E_NOT_OK otherwise
 */
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_ConsumeRecordFromBuffer(Tls_PtrConnection ConnectionPtr, P2VAR(Tls_BufferType, AUTOMATIC, TLS_APPL_DATA) BufferPtr);

/** \brief Callback for asynchronous TLS pseudorandom function
 *
 * This function is called when an asynchronous PRF Csm_MacGenerate job finishes.
 * It updates the PRF state machine.
 *
 * \param[in] ConnectionPtr    Pointer to the TLS connection context
 * \param[in] result           Result of Csm_MacGenerate
 *
 */
STATIC FUNC(void, TLS_CODE) Tls_PRFCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result);

/** \brief Called when ChangeCipherSpec is received
 *
 * Updates epoch and resets sequence number
 *
 * \param[in] ConnectionPtr    Pointer to the TLS connection context
 *
 */
STATIC FUNC(void, TLS_CODE) Tls_OnReceiveChangeCipherSpec(Tls_PtrConnection ConnectionPtr);

/** \brief Called when ChangeCipherSpec is sent
 *
 * Updates epoch and resets sequence number
 *
 * \param[in,out] ConnectionPtr  Pointer to the TLS connection context
 *
 */
STATIC FUNC(void, TLS_CODE) Tls_OnSendChangeCipherSpec(Tls_PtrConnection ConnectionPtr);

/** \brief PRF MainFunction
 *
 * Updates the state machine for TLS pseudo random function
 *
 * \param[in,out] ConnectionPtr  Pointer to the TLS connection context
 *
 */
STATIC FUNC(void, TLS_CODE) Tls_PRF_MainFunction(Tls_PtrConnection ConnectionPtr);

/** \brief Wrapper for Csm_KeyElementSet
 *
 *
 * \param[in] ConnectionPtr    Pointer to the TLS connection context
 *
 * \return Result of Csm_KeyElementSet
 */
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Csm_KeyElementSet(uint32 KeyId, uint32 KeyElementId, P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) KeyPtr, uint16 KeyLength);

/** \brief Gets the associated data for AEAD operation
 *
 *
 * \param[in] seq_num          Sequence number
 * \param[in] type             Record type
 * \param[in] version          TLS version
 * \param[in] length           Length of the record
 * \param[out] dataPtr         Pointer to associated data buffer
 *
 * \return E_OK if successfull, E_NOT_OK otherwise
 */
STATIC FUNC(void, TLS_CODE) Tls_AEAD_GetAssociatedData(Tls_Uint64Type seq_num, uint8 type,
    uint16 version, uint16 length, P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) dataPtr);

/** \brief Gets the explicit nonce for AEAD operation
 *
 *
 * \param[in] seq_num          Sequence number
 * \param[out] dataPtr         Pointer to associated data buffer
 *
 */
STATIC FUNC(void, TLS_CODE) Tls_AEAD_GetExplicitNonce(Tls_Uint64Type seq_num, P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) dataPtr);

/** \brief Checks if there is a complete record in the transmit buffer
 *
 *
 * \param[in] ConnectionPtr    Pointer to the TLS connection context
 *
 * \return E_OK if there is a record, E_NOT_OK otherwise
 */
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_CheckTransmitBuffer(Tls_PtrConnection ConnectionPtr);

/** \brief Wrapper for the TcpIp ProvideTxBuffer API
 *
 * Calls TcpIp_Udp_provideTxBuffer for UDP connections and TcpIp_Tcp_provideTxBuffer for TCP connections.
 *
 * \param[in] ConnectionPtr    Pointer to the TLS connection context
 *
 * \return E_OK if successful, E_NOT_OK otherwise
 */
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_ProvideTxBuffer(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Encrypt(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_EncryptCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Decrypt(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_DecryptCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result);

STATIC FUNC(void, TLS_CODE) Tls_CloseInternal(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_ChangeCipherSpec_Write(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_MacMakeHeader(Tls_PtrUint8 buffer, uint32 length, uint8 type, uint16 version, Tls_Uint64Type seq_num);

STATIC FUNC(void, TLS_CODE) Tls_MacVerifyCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result);

STATIC FUNC(void, TLS_CODE) Tls_MacGenerateCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result);

#if TLS_CERTIFICATE_SUPPORT == STD_ON
STATIC FUNC(void, TLS_CODE) Tls_VerifyCertificateCallback(Tls_PtrConnection ConnectionPtr, KeyM_CertificateStatusType result);

STATIC FUNC(void, TLS_CODE) Tls_SignatureGenerateCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result);

STATIC FUNC(void, TLS_CODE) Tls_SignatureVerifyCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result);

STATIC FUNC(void, TLS_CODE) Tls_JobKeyExchangeCalcPubValCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result);

STATIC FUNC(void, TLS_CODE) Tls_JobKeyExchangeCalcSecretCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result);
#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */

STATIC FUNC(void, TLS_CODE) Tls_Handshake_WriteFinished(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(uint32, TLS_CODE) Tls_Handshake_WriteClientKeyExchangePsk(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_SetDefaults(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(uint32, TLS_CODE) Tls_Handshake_WriteClientHello(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 random32byte);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_GenerateVerifyDataStart(Tls_PtrConnection ConnectionPtr,
    P2CONST(uint8, AUTOMATIC, TLS_CONST) label, uint32 label_length);

STATIC FUNC(void, TLS_CODE) Tls_HashCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result);

STATIC FUNC(void, TLS_CODE) Tls_Handshake_GenerateClientVerifyDataStart(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_Handshake_GenerateServerVerifyDataStart(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(uint8, TLS_CODE) Tls_Handshake_GetType(Tls_PtrConstUint8 handshake);

STATIC FUNC(uint32, TLS_CODE) Tls_Dtls_Handshake_GetFragmentLength(Tls_PtrConstUint8 handshake);

STATIC FUNC(uint32, TLS_CODE) Tls_Dtls_Handshake_GetFragmentOffset(Tls_PtrConstUint8 handshake);

STATIC FUNC(uint16, TLS_CODE) Tls_Dtls_Handshake_GetMessageSequence(Tls_PtrConstUint8 handshakeHeaderPtr);

STATIC FUNC(uint16, TLS_CODE) Tls_Handshake_Hello_GetVersion(Tls_PtrConstUint8 handshake);

STATIC FUNC(Tls_PtrConstUint8, TLS_CODE) Tls_Handshake_Hello_GetRandom(Tls_PtrConstUint8 handshake);

STATIC FUNC(void, TLS_CODE) Tls_Handshake_Append(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 handshake_data, uint32 length);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_CheckVerifyData(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(Tls_Uint64Type, TLS_CODE) Tls_Dtls_GetEpochAndSeqNum(Tls_PtrConstUint8 record);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_RandomGenerate(Tls_PtrConnection ConnectionPtr, Tls_PtrUint8 output);

STATIC FUNC(void, TLS_CODE) Tls_Dtls_Handshake_StartRetransmitTimer(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_Dtls_Handshake_StopRetransmitTimer(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_Dtls_Handshake_RetransmitTimerExpired(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_Handshake_Start(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Dtls_Handshake_HandleHelloVerifyRequest(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 handshake);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_StartMacGenerate(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Dtls_GenerateCookie(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Dtls_CheckCookie(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 CookiePtr, uint8 CookieLength);

STATIC FUNC(void, TLS_CODE) Tls_Dtls_Handshake_WriteHelloVerifyRequest(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(uint32, TLS_CODE) Tls_Handshake_WriteServerHelloDone(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(uint32, TLS_CODE) Tls_Handshake_WriteServerHello(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(uint32, TLS_CODE) Tls_Handshake_WritePskServerKeyExchange(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_ReplyClientHello(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleClientHello(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 DataPtr, uint16 Length);

STATIC FUNC(void, TLS_CODE) Tls_Connected(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleFinished(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 DataPtr, uint16 Length);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleServer(Tls_PtrConnection ConnectionPtr, Tls_PtrUint8 DataPtr, uint16 Length);

STATIC FUNC(void, TLS_CODE) Tls_StartKeyDerivation(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleServerHello(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 DataPtr, uint16 Length);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleClient(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 DataPtr, uint16 Length);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_Handle(Tls_PtrConnection ConnectionPtr, Tls_Uint8PtrType DataPtr, uint16 Length);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Dtls_Handshake_HandleFragment(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 HandshakePtr, uint16 Length);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleFragment(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 DataPtr, uint16 Length);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_ChangeCipherSpec_Handle(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_ApplicationData_Handle(Tls_PtrConnection ConnectionPtr, Tls_PtrUint8 PayloadPtr, uint16 Length);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Alert_Handle(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 PayloadPtr, uint16 Length);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Transmit(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 BufPtr, uint32 AvailableLength, boolean ForceRetrieve);

STATIC FUNC(void, TLS_CODE) Tls_Dtls_UpdateReceiveWindow(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 record);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Dtls_CheckSeqNum(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 record);

STATIC FUNC(void, TLS_CODE) Tls_RecordDecrypted(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_RecordReceived(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(boolean, TLS_CODE) Tls_CheckRecordType(uint8 Type);

STATIC FUNC(void, TLS_CODE) Tls_HandleReceiveBuffer(Tls_PtrConnection ConnectionPtr);

#if TLS_CERTIFICATE_SUPPORT == STD_ON
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_TransmitCertificateRequest(Tls_PtrConnection ConnectionPtr);
#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_TransmitServerHelloDone(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_HandleTransmitBuffer(Tls_PtrConnection ConnectionPtr);


#if( TLS_CSM_KEYDERIVE_SUPPORT == STD_ON )
STATIC FUNC(uint8, TLS_CODE) Tls_GetCsmKeyDeriveAlgorithm(uint16 ciphersuite);
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_CsmKeyDerive(Tls_PtrConnection ConnectionPtr);
#else /* TLS_CSM_KEYDERIVE_SUPPORT == STD_OFF */
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_KeyDeriveSetCsmKeys(Tls_PtrConnection ConnectionPtr);
#endif /* TLS_CSM_KEYDERIVE_SUPPORT == STD_ON */

STATIC FUNC(void, TLS_CODE) Tls_KeyDeriveMainFunction(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_ProcessVerifyData(Tls_PtrConnection ConnectionPtr,
    P2VAR(Tls_VerifyDataStateType, AUTOMATIC, TLS_VAR) statePtr,
    Tls_PtrUint8 verifyDataPtr, P2CONST(uint8, AUTOMATIC, TLS_CONST) label, uint32 labelLength);

STATIC FUNC(void, TLS_CODE) Tls_ProcessMacGeneration(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_ProcessEncryption(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_ProcessRandom(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_Dtls_ProcessCookie(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_ProcessMacVerification(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_ProcessDecryption(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_Dtls_ProcessTimer(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_UL_Accepted(Tls_PtrConnection ConnectionPtr);

#if TLS_CERTIFICATE_SUPPORT == STD_ON
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_SignatureGenerate(Tls_PtrConnection ConnectionPtr, uint8 *sigInPtr , uint32 sigInLen, Crypto_OperationModeType mode, Tls_SigGenStateType sigGenState);
#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */

STATIC FUNC(void, TLS_CODE) Tls_ProcessServerState(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_ProcessClientState(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_ProcessState(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(void, TLS_CODE) Tls_ConnectionMainFunction(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_ReadVar8Array(Tls_PtrConstUint8 DataPtr, uint32 Length,
    P2VAR(uint32, AUTOMATIC, TLS_APPL_DATA) Idx,
    Tls_PtrUint8 ArrayLengthPtr, P2VAR(Tls_PtrConstUint8, AUTOMATIC, TLS_APPL_DATA) ArrayDataPtr);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_ConsumeRecordFromReceiveBuffer(Tls_PtrConnection ConnectionPtr);

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_ConsumeRecordFromTransmitBuffer(Tls_PtrConnection ConnectionPtr);

#if TLS_CFG_CSM_PROCESSING == TLS_CSM_ASYNCHRONOUS
FUNC(void, CSM_APPL_CODE) Tls_CsmRandomCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result);

FUNC(void, CSM_APPL_CODE) Tls_CsmPRFCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result);

FUNC(void, CSM_APPL_CODE) Tls_CsmEncryptCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result);

FUNC(void, CSM_APPL_CODE) Tls_CsmHashCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result);

FUNC(void, CSM_APPL_CODE) Tls_CsmDecryptCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result);

FUNC(void, CSM_APPL_CODE) Tls_CsmMacGenerateCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result);

FUNC(void, CSM_APPL_CODE) Tls_CsmMacVerifyCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result);

FUNC(void, CSM_APPL_CODE) Tls_CsmSignatureGenerateCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result);

FUNC(void, CSM_APPL_CODE) Tls_CsmSignatureVerifyCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result);

FUNC(void, CSM_APPL_CODE) Tls_CsmJobKeyExchangeCalcPubValCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result);

FUNC(void, CSM_APPL_CODE) Tls_CsmJobKeyExchangeCalcSecretCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result);
#endif /* TLS_CFG_CSM_PROCESSING == TLS_CSM_ASYNCHRONOUS */

#if (TLS_CERTIFICATE_SUPPORT == STD_ON)
/* TODO: isn't this declared in KeyM_Externals.h (according to SWS)? */
extern FUNC(Std_ReturnType, TLS_APPL_CODE) Tls_KeyMVerifyCertificateCallback(KeyM_CertificateIdType CertId, KeyM_CertificateStatusType Result);
#endif /* (TLS_CERTIFICATE_SUPPORT == STD_ON) */

#define TLS_STOP_SEC_CODE
#include <Tls_MemMap.h>

/*=======================[external data declarations]============================================*/

/*=======================[internal data declarations]============================================*/

#define TLS_START_SEC_VAR_INIT_BOOLEAN
#include <Tls_MemMap.h>

/** \brief  Variable that represents whether the Tls module is fully initialized.
 **/
#if(TLS_DEV_ERROR_DETECT == STD_ON)
STATIC VAR(boolean, TLS_VAR) Tls_Initialized = FALSE;
#endif

#define TLS_STOP_SEC_VAR_INIT_BOOLEAN
#include <Tls_MemMap.h>

#define TLS_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Tls_MemMap.h>

/* !LINKSTO Tls.Config.Connections,1 */
STATIC Tls_ConnectionType Tls_Connections[TLS_CFG_CONNECTION_COUNT];

#define TLS_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Tls_MemMap.h>

/*==================[internal function definitions]==============================================*/
#define TLS_START_SEC_CODE
#include <Tls_MemMap.h>

STATIC FUNC(uint8, TLS_CODE) Tls_GetSocketOwner(Tls_PtrConnection ConnectionPtr)
{
  /* !LINKSTO Tls.API.Callout.TcpIp_GetSocketOwner,1 */
  return TcpIp_GetSocketOwner(ConnectionPtr->socket);
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_UL_CopyTxData(Tls_PtrConnection ConnectionPtr, P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) BufPtr, uint16 BufLength)
{
  Std_ReturnType retVal = E_NOT_OK;
  /* !LINKSTO Tls.API.Callout.CopyTxData,1 */
  if(TcpIp_CopyTxDataAPI[Tls_GetSocketOwner(ConnectionPtr)](ConnectionPtr->socket, BufPtr, BufLength) == BUFREQ_OK)
  {
    retVal = E_OK;
  }

  return retVal;
}

STATIC FUNC(void, TLS_CODE) Tls_UL_TcpIpEvent(Tls_PtrConnection ConnectionPtr, TcpIp_EventType Event)
{
  /* !LINKSTO Tls.API.Callout.Closed,1 */
  TcpIp_TcpIpEventAPI[Tls_GetSocketOwner(ConnectionPtr)](ConnectionPtr->socket, Event);
}

STATIC FUNC(void, TLS_CODE) Tls_UL_TcpAccepted(Tls_PtrConnection ConnectionPtr, TcpIp_SocketIdType SocketId, TcpIp_SocketIdType SocketIdConnected, P2CONST(TcpIp_SockAddrType, AUTOMATIC, TLS_APPL_DATA) RemoteAddrPtr)
{
  (void)SocketId;
  (void)RemoteAddrPtr;
  /* !LINKSTO Tls.API.Callout.TcpAccepted,1 */
  TcpIp_TcpTlsAcceptedAPI[Tls_GetSocketOwner(ConnectionPtr)](SocketIdConnected);
}

STATIC FUNC(void, TLS_CODE) Tls_UL_Closed(Tls_PtrConnection ConnectionPtr)
{
  /* Tell SoAd to close the socket */
  /* !LINKSTO Tls.API.Callout.Closed,1 */
  if(ConnectionPtr->config->dtls == FALSE)
  {
    Tls_UL_TcpIpEvent(ConnectionPtr, TCPIP_TCP_FIN_RECEIVED);
  }
  else
  {
    Tls_UL_TcpIpEvent(ConnectionPtr, TCPIP_UDP_CLOSED);
  }
}

FUNC(Std_ReturnType, TLS_CODE) Tls_TcpAccepted(TcpIp_SocketIdType SocketId, TcpIp_SocketIdType SocketIdConnected,
       TcpIp_ConstSockAddrPtrType RemoteAddrPtr)
{
  Std_ReturnType retVal = E_NOT_OK;
  Tls_PtrConnection ConnectionPtr;

#if(TLS_DEV_ERROR_DETECT == STD_ON)
  if (Tls_Initialized == FALSE)
  {
    /* !LINKSTO Tls.API.Tls_TcpAccepted.Det.NoInit,1 */
    TLS_DET_REPORT_ERROR(TLS_TCPACCEPTED_SVCID, TLS_E_UNINIT);
  }
  else
#endif
  {
    ConnectionPtr = Tls_FindConnection(SocketIdConnected);
    if(ConnectionPtr != NULL_PTR)
    {
      ConnectionPtr->listenSocket = SocketId;
      /* Will return an error if something is wrong with the address. */
      retVal = Tls_CopyIpAddress(ConnectionPtr, RemoteAddrPtr);
    }
  }
  return retVal;
}

STATIC FUNC(void, TLS_CODE) Tls_UL_TxConfirmation(Tls_PtrConnection ConnectionPtr, uint16 Length)
{
  TLS_PRINTF(("Tls_UL_TxConfirmation: %d bytes\n", Length));
  /* !LINKSTO Tls.API.Callout.TxConfirmation,1 */
  TcpIp_TxConfirmationAPI[Tls_GetSocketOwner(ConnectionPtr)](ConnectionPtr->socket, Length);
}

STATIC FUNC(void, TLS_CODE) Tls_UL_RxIndication
(
    Tls_PtrConnection ConnectionPtr,
    Tls_PtrUint8 BufPtr,
    uint16 Length
)
{
  uint8 owner = Tls_GetSocketOwner(ConnectionPtr);
  /* !LINKSTO Tls.API.Callout.RxIndication,1 */
  TcpIp_RxIndicationFpType ptr = TcpIp_RxIndicationAPI[owner];
  /* Deviation MISRAC2012-1 */
  ptr(ConnectionPtr->socket, (TcpIp_ConstSockAddrPtrType)&ConnectionPtr->remoteAddr, BufPtr, Length);
}

STATIC FUNC(uint16, TLS_CODE) Tls_GetVersion(Tls_PtrConstConnection ConnectionPtr)
{
  uint16 version;

  if(ConnectionPtr->config->dtls)
  {
    version = TLS_DTLS_VERSION;
  }
  else
  {
    version = TLS_VERSION;
  }

  return version;
}

STATIC FUNC(Tls_Uint64Type, TLS_CODE) Tls_GetWriteSeqNum(Tls_PtrConstConnection ConnectionPtr)
{
  Tls_Uint64Type WriteseqNum;

  if(ConnectionPtr->config->dtls)
  {
    WriteseqNum = ((Tls_Uint64Type)(ConnectionPtr->writeSeqNum) | (Tls_Uint64Type)((Tls_Uint64Type)(ConnectionPtr->dtls.writeEpoch)<<48) );
  }
  else
  {
    WriteseqNum = ConnectionPtr->writeSeqNum;
  }

  return WriteseqNum;
}

STATIC FUNC(Tls_Uint64Type, TLS_CODE) Tls_GetReadSeqNum(Tls_PtrConstConnection ConnectionPtr)
{
  Tls_Uint64Type ReadseqNum;

  if(ConnectionPtr->config->dtls)
  {
    ReadseqNum = Tls_Dtls_GetEpochAndSeqNum(ConnectionPtr->receiveBuffer.buffer);
  }
  else
  {
    ReadseqNum = (Tls_Uint64Type)ConnectionPtr->readSeqNum;
  }

  return ReadseqNum;
}

STATIC FUNC(uint16, TLS_CODE) Tls_GetRecordLength(Tls_PtrConstConnection ConnectionPtr, Tls_PtrConstUint8 record)
{
  uint8 header_size = Tls_GetRecordHeaderSize(ConnectionPtr);

  uint16 length = ((uint16)record[header_size - 2u] << 8) | (uint16)record[header_size - 1u];

  return length;
}

STATIC FUNC(void, TLS_CODE) Tls_SetRecordLength(Tls_PtrConstConnection ConnectionPtr, Tls_PtrUint8 record, uint16 length)
{
  uint8 header_size = Tls_GetRecordHeaderSize(ConnectionPtr);

  record[header_size - 2u] = (uint8)((uint8)(length >> 8) & 0xFFu);
  record[header_size - 1u] = (uint8)((uint8)(length >> 0) & 0xFFu);
}

STATIC FUNC(uint16, TLS_CODE) Tls_GetRecordVersion(Tls_PtrConstUint8 record)
{
  return ((uint16)record[1u] << 8) | (uint16)record[2u];
}

STATIC FUNC(uint8, TLS_CODE) Tls_GetRecordType(Tls_PtrConstUint8 record)
{
  return record[0u];
}

STATIC FUNC(uint16, TLS_CODE) Tls_GetRecordOverhead(Tls_PtrConnection ConnectionPtr)
{
  uint16 overhead = Tls_GetRecordHeaderSize(ConnectionPtr);
  if(ConnectionPtr->cipherParams->cipherType == TLS_AEAD_CIPHER)
  {
    overhead += (uint16)TLS_AEAD_EXPLICIT_NONCE_LENGTH + (uint16)ConnectionPtr->cipherParams->MACLength;
  }
  else
  {
    overhead += ConnectionPtr->cipherParams->MACLength;
  }
  return overhead;
}

STATIC FUNC(void, TLS_CODE) Tls_SetState(Tls_PtrConnection ConnectionPtr, Tls_StateType NewState)
{
  TLS_PRINTF(("TLS State Change: "));
  TLS_DEBUG_PRINTSTATE(ConnectionPtr);
  TLS_PRINTF((" -> "));
  ConnectionPtr->state = NewState;
  TLS_DEBUG_PRINTSTATE(ConnectionPtr);
  TLS_PRINTF(("\n"));
}

/* !LINKSTO Dtls.RFC.Record.Format,1 */
STATIC FUNC(void, TLS_CODE) Tls_Dtls_WriteRecordHeader(Tls_PtrUint8 dest, uint8 type, uint16 version, uint32 length, uint16 epoch, Tls_Uint64Type seq_num)
{
  dest[0] = type;
  dest[1] = (uint8)((uint8)(version >> 8) & 0xFFu);
  dest[2] = (uint8)(version & 0xFFu);
  dest[3] = (uint8)((uint8)(epoch >> 8) & 0xFFu);
  dest[4] = (uint8)((uint8)(epoch >> 0) & 0xFFu);
  dest[5] = (uint8)((uint8)(seq_num >> 40) & 0xFFu);
  dest[6] = (uint8)((uint8)(seq_num >> 32) & 0xFFu);
  dest[7] = (uint8)((uint8)(seq_num >> 24) & 0xFFu);
  dest[8] = (uint8)((uint8)(seq_num >> 16) & 0xFFu);
  dest[9] = (uint8)((uint8)(seq_num >> 8) & 0xFFu);
  dest[10] = (uint8)((uint8)(seq_num >> 0) & 0xFFu);
  dest[11] = (uint8)((uint8)(length >> 8) & 0xFFu);
  dest[12] = (uint8)((uint8)(length >> 0) & 0xFFu);
}

STATIC FUNC(boolean, TLS_CODE) Tls_TxConfirmation_Check(Tls_PtrConnection ConnectionPtr, uint8 count)
{
  return (TLS_TXCONFIRMATION_SIZE - ConnectionPtr->txConfirmations.count) >= count;
}

STATIC FUNC(void, TLS_CODE) Tls_TxConfirmation_Enqueue(Tls_PtrConnection ConnectionPtr, uint16 size, uint8 type)
{
  uint8 pos;

  /* Assume there is enough space in the buffer. Call Tls_TxConfirmation_Check before. */
  pos = (ConnectionPtr->txConfirmations.start + ConnectionPtr->txConfirmations.count) % TLS_TXCONFIRMATION_SIZE;
  ConnectionPtr->txConfirmations.buffer[pos] = size;
  ConnectionPtr->txConfirmations.types[pos] = type;
  ConnectionPtr->txConfirmations.count++;
}

STATIC FUNC(uint16, TLS_CODE) Tls_TxConfirmation_Dequeue(Tls_PtrConnection ConnectionPtr, uint16 size)
{
  uint16 recordSize;
  uint8 recordType;
  uint16 ret = 0u;
  Tls_PtrTxConfirmationBuffer txConf = &ConnectionPtr->txConfirmations;

  txConf->confirmed += size;

  while(txConf->count > 0u)
  {
    recordSize = txConf->buffer[txConf->start];
    recordType = txConf->types[txConf->start];

    if(txConf->confirmed >= recordSize)
    {
      /* Record is completely confirmed. Check if record type is application data. */
      if(recordType == TLS_RECORD_APPLICATIONDATA)
      {
        /* return the size of the confirmed record */
        ret = recordSize;
      }
      txConf->confirmed -= recordSize;
      txConf->start = (txConf->start + 1u) % TLS_TXCONFIRMATION_SIZE;
      txConf->count--;
    }
    else
    {
      break;
    }
  }
  return ret;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_TcpIp_Transmit(Tls_PtrConnection ConnectionPtr, uint32 AvailableLength)
{
  Std_ReturnType ret = E_NOT_OK;
  boolean appended = FALSE;
  Tls_PtrHandshakeData data;
  TcpIp_Socket_Generic_PtrType SocketPtr;

  TLS_DEBUG_PRINTBYTES("Transmit", ConnectionPtr->encryptBuffer, AvailableLength);

  if (ConnectionPtr->config->dtls == TRUE)
  {
#if (TCPIP_UDP_ENABLED == STD_ON)
    data = &ConnectionPtr->handshakeData;
    SocketPtr = TcpIp_Socket_getSocketPtr(ConnectionPtr->socket);
    /* Append retransmit data before calling TcpIp transmit. Pointer may not be valid after the call anymore. */
    if(ConnectionPtr->state != TLS_STATE_OK)
    {
      /* Check if there is enough space in the buffer. Should always be true. */
      /* CHECK: NOPARSE */
      if(TLS_DTLS_RETRANSMIT_BUFFER_SIZE >= (AvailableLength + data->dtls_retransmitBufferCount))
      /* CHECK: PARSE */
      {
        /* Copy data to retransmit buffer */
        TS_MemCpy(&data->dtls_retransmitBuffer[data->dtls_retransmitBufferCount], ConnectionPtr->encryptBuffer, AvailableLength);
        data->dtls_retransmitBufferCount += AvailableLength;
        appended = TRUE;
      }
    }

    /* !LINKSTO Tls.API.Callout.TcpIp_Udp_transmit,1 */
    /* Deviation MISRAC2012-1 */
    ret = TcpIp_Udp_transmit
          (
            ConnectionPtr->socket,
            255 /*TODO define for: invalid SA index*/,
            SocketPtr,
            ConnectionPtr->encryptBuffer,
            (TcpIp_ConstSockAddrPtrType) &ConnectionPtr->remoteAddr,
            SocketPtr->aligned.local.addrId,
            SocketPtr->aligned.local.port,
            AvailableLength
          );

#endif /* (TCPIP_UDP_ENABLED == STD_ON) */
    TLS_PRINTF(("TcpIp_Udp_transmit: %d\n", ret));

    if(ret != E_OK)
    {
      if(appended == TRUE)
      {
        /* Transmit did not work. Remove data from the buffer. */
        data->dtls_retransmitBufferCount -= AvailableLength;
      }

      if( (ConnectionPtr->state == TLS_STATE_OK) || (ret != TCPIP_E_PHYS_ADDR_MISS) )
      {
        ConnectionPtr->transmissionError = TRUE;
      }
    }
  }
  else
  {
#if (TCPIP_TCP_ENABLED == STD_ON)
    /* Check if there is enough space to buffer data for the TxConfirmation */
    if(Tls_TxConfirmation_Check(ConnectionPtr,1u) == TRUE)
    {
      /* !LINKSTO Tls.API.Callout.TcpIp_Tcp_transmit,1 */
      ret = TcpIp_Tcp_transmit(ConnectionPtr->socket, AvailableLength);
      TLS_PRINTF(("TcpIp_Tls_TcpTransmit: %d\n", ret));
      if(ret != E_OK)
      {
        ConnectionPtr->transmissionError = TRUE;
      }
    }
    else
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */
    {
      ret = E_NOT_OK;
    }
  }

  if(ret == E_OK)
  {
    if(ConnectionPtr->config->dtls == FALSE)
    {
      /* Queue TxConfirmation only for TCP */
      Tls_TxConfirmation_Enqueue(ConnectionPtr, AvailableLength, Tls_GetRecordType(ConnectionPtr->encryptBuffer));
    }

    ConnectionPtr->encryptBuffer = NULL_PTR;
    ConnectionPtr->encryptBufferLength = 0u;
    ConnectionPtr->encryptBufferCapacity = 0u;
  }

  return ret;
}

STATIC FUNC(void, TLS_CODE) Tls_WriteRecordHeader(Tls_PtrUint8 dest, uint8 type, uint16 version, uint32 length)
{
  /* !LINKSTO Tls.RFC.Record.Format,1 */
  dest[0] = type;
  dest[1] = (uint8)((uint8)(version >> 8) & 0xFFu);
  dest[2] = (uint8)(version & 0xFFu);
  dest[3] = (uint8)((uint8)(length >> 8) & 0xFFu);
  dest[4] = (uint8)((uint8)(length >> 0) & 0xFFu);
}

STATIC FUNC(void, TLS_CODE) Tls_Buffer_Init(Tls_PtrBuffer BufferPtr, P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) BufPtr, uint16 Length)
{
  BufferPtr->size = Length;
  BufferPtr->buffer = BufPtr;
  BufferPtr->count = 0u;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Buffer_Append(Tls_PtrBuffer BufferPtr, P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) BufPtr, uint16 Length)
{
  Std_ReturnType ret = E_OK;

  /* Defensive check. Should not happen. */
  /* CHECK: NOPARSE */
  if ((Length + BufferPtr->count) > BufferPtr->size)
  {
    /* Does not fit */
    ret = E_NOT_OK;
  }
  else
  /* CHECK: PARSE */
  {
    TS_MemCpy(&BufferPtr->buffer[BufferPtr->count], BufPtr, Length);
    BufferPtr->count += Length;
  }
  return ret;
}

STATIC FUNC(uint16, TLS_CODE) Tls_Buffer_GetFreeBytes(Tls_PtrBuffer BufferPtr)
{
  return BufferPtr->size - BufferPtr->count;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Buffer_Copy(Tls_PtrBuffer BufferPtr, Tls_PtrUint8 BufPtr, uint16 Start, uint16 Length)
{
  Std_ReturnType ret = E_OK;

  /* This is a defensive check. The buffer should not be accessed outside its bounds. */
  /* CHECK: NOPARSE */
  if ((Start + Length) > BufferPtr->count)
  {
      ret = E_NOT_OK;
  }
  else
  /* CHECK: PARSE */
  {
    TS_MemCpy(BufPtr, &BufferPtr->buffer[Start], Length);
  }

  return ret;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Buffer_Consume(Tls_PtrBuffer BufferPtr, uint16 Length)
{
  Std_ReturnType ret = E_OK;

  /* This is a defensive check. The buffer should not be accessed outside its bounds. */
  /* CHECK: NOPARSE */
  if(Length > BufferPtr->count)
  {
    ret = E_NOT_OK;
  }
  else
  /* CHECK: PARSE */
  {
    BufferPtr->count -= Length;
    TS_MemCpy(BufferPtr->buffer, &BufferPtr->buffer[Length], BufferPtr->count);
  }

  return ret;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_ConsumeRecordFromBuffer(Tls_PtrConnection ConnectionPtr, P2VAR(Tls_BufferType, AUTOMATIC, TLS_APPL_DATA) BufferPtr)
{
  Std_ReturnType ret = E_OK;
  uint8 headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);
  uint32 length;

  TLS_PRINTF(("Tls_ConsumeRecordFromBuffer\n"));

  /* This is a defensive check. This function is only called if there is a record in the buffer. */
  /* CHECK: NOPARSE */
  if (BufferPtr->count >= headerSize)
  {
    length = Tls_GetRecordLength(ConnectionPtr, BufferPtr->buffer);
    ret = Tls_Buffer_Consume(BufferPtr, length + headerSize);
  }
  else
  {
    ret = E_NOT_OK;
  }
  /* CHECK: PARSE */

  return ret;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_ConsumeRecordFromReceiveBuffer(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret;

  TLS_ENTER_EXCLUSIVE_AREA_RX();
  ret = Tls_ConsumeRecordFromBuffer(ConnectionPtr, &ConnectionPtr->receiveBuffer);
  TLS_EXIT_EXCLUSIVE_AREA_RX();

  return ret;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_ConsumeRecordFromTransmitBuffer(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret;

  TLS_ENTER_EXCLUSIVE_AREA_TX();
  ret = Tls_ConsumeRecordFromBuffer(ConnectionPtr, &ConnectionPtr->transmitBuffer);
  TLS_EXIT_EXCLUSIVE_AREA_TX();

  return ret;
}

/* !LINKSTO Tls.RFC.PRF,1 */
STATIC FUNC(void, TLS_CODE) Tls_PRFCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result)
{
  if(result == E_OK)
  {
    switch(ConnectionPtr->handshakeData.prf.state)
    {
    case TLS_PRF_STATE_BUSY_1:
      ConnectionPtr->handshakeData.prf.state = TLS_PRF_STATE_BUSY_2;
      TLS_DEBUG_PRF_PRINTSTATE(&ConnectionPtr->handshakeData.prf);
      break;
    case TLS_PRF_STATE_BUSY_2B:
      ConnectionPtr->handshakeData.prf.state = TLS_PRF_STATE_BUSY_2C;
      TLS_DEBUG_PRF_PRINTSTATE(&ConnectionPtr->handshakeData.prf);
      break;
    case TLS_PRF_STATE_BUSY_2D:
      ConnectionPtr->handshakeData.prf.state = TLS_PRF_STATE_BUSY_2E;
      TLS_DEBUG_PRF_PRINTSTATE(&ConnectionPtr->handshakeData.prf);
      break;
    default:
      /* Do nothing: The callback was called but the Tls module did not call MacGenerate before. */
      TLS_PRINTF(("Tls_PRF_Callback not expected.\n"));
      break;
    }
  }
  else
  {
    ConnectionPtr->handshakeData.prf.state = TLS_PRF_STATE_ERROR;
    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
  }
}

STATIC FUNC(void, TLS_CODE) Tls_OnReceiveChangeCipherSpec(Tls_PtrConnection ConnectionPtr)
{
  TLS_PRINTF(("Tls_OnReceiveChangeCipherSpec\n"));

  /* !LINKSTO Dtls.RFC.Record.SequenceNumber.Reset,1 */
  ConnectionPtr->readSeqNum = 0u;
  if(ConnectionPtr->config->dtls)
  {
    /* !LINKSTO Dtls.RFC.Record.Epoch.Increment,1 */
    ConnectionPtr->dtls.readEpoch++;
    ConnectionPtr->dtls.receiveWindow = 0u;
    TLS_PRINTF(("New readEpoch: %d, readSeqNum=%d\n", ConnectionPtr->dtls.readEpoch, ConnectionPtr->readSeqNum));
  }
}

STATIC FUNC(void, TLS_CODE) Tls_OnSendChangeCipherSpec(Tls_PtrConnection ConnectionPtr)
{
  TLS_PRINTF(("Tls_OnSendChangeCipherSpec\n"));

  /* !LINKSTO Dtls.RFC.Record.SequenceNumber.Reset,1 */
  ConnectionPtr->writeSeqNum = 0u;
  if(ConnectionPtr->config->dtls)
  {
    /* !LINKSTO Dtls.RFC.Record.Epoch.Increment,1 */
    ConnectionPtr->dtls.writeEpoch++;
    TLS_PRINTF(("New writeEpoch: %d, writeSeqNum=%d\n", ConnectionPtr->dtls.writeEpoch, ConnectionPtr->writeSeqNum));
  }
}

/* !LINKSTO Tls.RFC.PRF,1 */
STATIC FUNC(void, TLS_CODE) Tls_PRF_MainFunction(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret = E_OK;
  Tls_PtrPRF_Context PRFPtr = &ConnectionPtr->handshakeData.prf;

  switch(PRFPtr->state)
  {
  case TLS_PRF_STATE_START:
    {
      PRFPtr->pos = 0u;
      PRFPtr->len = TLS_PRF_BLOCK_SIZE;
      /* A(1)= HMAC_hash(secret, seed ) */
      ret = Csm_MacGenerate(PRFPtr->jobId, CRYPTO_OPERATIONMODE_SINGLECALL, PRFPtr->seed, PRFPtr->seedLength, PRFPtr->tmp, &PRFPtr->len);
      if(ret == E_OK)
      {
        PRFPtr->state = TLS_PRF_STATE_BUSY_1;
        TLS_DEBUG_PRF_PRINTSTATE(PRFPtr);
#if TLS_CFG_CSM_PROCESSING == TLS_CSM_SYNCHRONOUS
        /* !LINKSTO Tls.Async,1 */
        Tls_PRFCallback(ConnectionPtr, E_OK);
#endif
      }
    }
    break;
  case TLS_PRF_STATE_BUSY_2:
    {
      if(PRFPtr->len != TLS_PRF_BLOCK_SIZE)
      {
        ret = E_NOT_OK;
      }
      else
      {
        /* tmp = A(n) + seed */
        TS_MemCpy(&PRFPtr->tmp[TLS_PRF_BLOCK_SIZE], PRFPtr->seed, PRFPtr->seedLength);

        PRFPtr->state = TLS_PRF_STATE_BUSY_2A;
        TLS_DEBUG_PRF_PRINTSTATE(PRFPtr);
      }
    }
    break;
  case TLS_PRF_STATE_BUSY_2A:
    {
      if(PRFPtr->pos >= PRFPtr->resultLength)
      {
        PRFPtr->state = TLS_PRF_STATE_READY;
        TLS_DEBUG_PRF_PRINTSTATE(PRFPtr);
      }
      else
      {
        /* r = HMAC_hash(secret, A(n) + seed) */
        ret = Csm_MacGenerate(PRFPtr->jobId, CRYPTO_OPERATIONMODE_SINGLECALL, PRFPtr->tmp, PRFPtr->seedLength + TLS_PRF_BLOCK_SIZE, PRFPtr->r, &PRFPtr->len);
        if(ret == E_OK)
        {
          PRFPtr->state = TLS_PRF_STATE_BUSY_2B;
          TLS_DEBUG_PRF_PRINTSTATE(PRFPtr);
#if TLS_CFG_CSM_PROCESSING == TLS_CSM_SYNCHRONOUS
          /* !LINKSTO Tls.Async,1 */
          Tls_PRFCallback(ConnectionPtr, E_OK);
#endif
        }
      }
    }
    break;
  case TLS_PRF_STATE_BUSY_2C:
    {
      if(PRFPtr->len != TLS_PRF_BLOCK_SIZE)
      {
        ret = E_NOT_OK;
      }
      else
      {
        /* tmp2 = A(n) = HMAC_hash(secret, A(n-1)) */
        ret = Csm_MacGenerate(PRFPtr->jobId, CRYPTO_OPERATIONMODE_SINGLECALL, PRFPtr->tmp, TLS_PRF_BLOCK_SIZE, PRFPtr->tmp2, &PRFPtr->len);
        if(ret == E_OK)
        {
          PRFPtr->state = TLS_PRF_STATE_BUSY_2D;
          TLS_DEBUG_PRF_PRINTSTATE(PRFPtr);
#if TLS_CFG_CSM_PROCESSING == TLS_CSM_SYNCHRONOUS
          /* !LINKSTO Tls.Async,1 */
        Tls_PRFCallback(ConnectionPtr, E_OK);
#endif
        }
      }
    }
    break;
  case TLS_PRF_STATE_BUSY_2E:
    {
      if(PRFPtr->len != TLS_PRF_BLOCK_SIZE)
      {
        ret = E_NOT_OK;
      }
      else
      {
        TS_MemCpy(PRFPtr->tmp, PRFPtr->tmp2, TLS_PRF_BLOCK_SIZE);

        if((PRFPtr->pos + TLS_PRF_BLOCK_SIZE) < PRFPtr->resultLength)
        {
          TS_MemCpy(&PRFPtr->result[PRFPtr->pos], PRFPtr->r, TLS_PRF_BLOCK_SIZE);
        }
        else
        {
          TS_MemCpy(&PRFPtr->result[PRFPtr->pos], PRFPtr->r, PRFPtr->resultLength - PRFPtr->pos);
        }
        PRFPtr->pos += TLS_PRF_BLOCK_SIZE;
        PRFPtr->state = TLS_PRF_STATE_BUSY_2A;
        TLS_DEBUG_PRF_PRINTSTATE(PRFPtr);
      }
    }
    break;
  default:
    /* Do nothing */
    break;
  }

  if(ret != E_OK)
  {
    TLS_PRINTF(("Tls_PRF_MainFunction error.\n"));
    PRFPtr->state = TLS_PRF_STATE_ERROR;
    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
  }
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Csm_KeyElementSet(uint32 KeyId, uint32 KeyElementId, P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) KeyPtr, uint16 KeyLength)
{
  Std_ReturnType ret = E_OK;

  ret = Csm_KeyElementSet(KeyId, KeyElementId, KeyPtr, KeyLength);
  if(ret != E_OK)
  {
    TLS_PRINTF(("Csm_KeyElementSet: %d\n", ret));
  }
  else
  {
    ret = Csm_KeySetValid(KeyId);
    if(ret != E_OK)
    {
      TLS_PRINTF(("Csm_KeySetValid: %d\n", ret));
    }
  }
  return ret;
}

/* !LINKSTO Tls.RFC.AEAD.AssociatedData,1 */
STATIC FUNC(void, TLS_CODE) Tls_AEAD_GetAssociatedData(Tls_Uint64Type seq_num, uint8 type,
    uint16 version, uint16 length, P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) dataPtr)
{
  dataPtr[0] = (uint8)((uint8)(seq_num >> 56) & 0xFFu);
  dataPtr[1] = (uint8)((uint8)(seq_num >> 48) & 0xFFu);
  dataPtr[2] = (uint8)((uint8)(seq_num >> 40) & 0xFFu);
  dataPtr[3] = (uint8)((uint8)(seq_num >> 32) & 0xFFu);
  dataPtr[4] = (uint8)((uint8)(seq_num >> 24) & 0xFFu);
  dataPtr[5] = (uint8)((uint8)(seq_num >> 16) & 0xFFu);
  dataPtr[6] = (uint8)((uint8)(seq_num >> 8) & 0xFFu);
  dataPtr[7] = (uint8)((uint8)(seq_num >> 0) & 0xFFu);

  dataPtr[8] = type;

  dataPtr[9] = (uint8)((version >> 8) & 0xFFu);
  dataPtr[10] = (uint8)((version >> 0)& 0xFFu);

  dataPtr[11] = (uint8)((length >> 8) & 0xFFu);
  dataPtr[12] = (uint8)((length >> 0) & 0xFFu);
}

STATIC FUNC(void, TLS_CODE) Tls_AEAD_GetExplicitNonce(Tls_Uint64Type seq_num, P2VAR(uint8, AUTOMATIC, TLS_APPL_DATA) dataPtr)
{
  dataPtr[0] = (uint8)((uint8)(seq_num >> 56) & 0xFFu);
  dataPtr[1] = (uint8)((uint8)(seq_num >> 48) & 0xFFu);
  dataPtr[2] = (uint8)((uint8)(seq_num >> 40) & 0xFFu);
  dataPtr[3] = (uint8)((uint8)(seq_num >> 32) & 0xFFu);
  dataPtr[4] = (uint8)((uint8)(seq_num >> 24) & 0xFFu);
  dataPtr[5] = (uint8)((uint8)(seq_num >> 16) & 0xFFu);
  dataPtr[6] = (uint8)((uint8)(seq_num >> 8) & 0xFFu);
  dataPtr[7] = (uint8)((uint8)(seq_num >> 0) & 0xFFu);
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_CheckTransmitBuffer(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret = E_OK;
  uint8 headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);
  uint16 recordLength = Tls_GetRecordLength(ConnectionPtr, ConnectionPtr->transmitBuffer.buffer);

  if(ConnectionPtr->transmitBuffer.count < (headerSize + recordLength))
  {
    /* record is not complete */
    ret = E_NOT_OK;
  }

  return ret;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_ProvideTxBuffer(Tls_PtrConnection ConnectionPtr)
{
  TcpIp_ReturnType ret = E_OK;

  if(ConnectionPtr->encryptBuffer == NULL_PTR)
  {
    ret = E_NOT_OK;
    ConnectionPtr->encryptBufferCapacity = ConnectionPtr->config->maxRecordSize;
    if(ConnectionPtr->config->dtls)
    {
#if (TCPIP_UDP_ENABLED == STD_ON)
      ConnectionPtr->dtls.ethTxHandle.bufferPtr = NULL_PTR;
      ConnectionPtr->dtls.ethTxHandle.bufferLength = ConnectionPtr->encryptBufferCapacity;
      ConnectionPtr->dtls.ethTxHandle.bufferHandle = 0u;

      /* Set encrypt Buffer */
      ConnectionPtr->encryptBuffer = ConnectionPtr->config->dtlsEncryptBufferPtr;
      ret = E_OK;
#endif /* (TCPIP_UDP_ENABLED == STD_ON) */
    }
    else
    {
#if (TCPIP_TCP_ENABLED == STD_ON)
      /* !LINKSTO Tls.API.Callout.TcpIp_Tcp_provideTxBuffer,1 */
      ret = TcpIp_Tcp_provideTxBuffer(&ConnectionPtr->encryptBuffer, &ConnectionPtr->encryptBufferCapacity, ConnectionPtr->socket);
#endif /* (TCPIP_TCP_ENABLED == STD_ON) */
    }
  }

  if(ret != E_OK)
  {
    ConnectionPtr->encryptBufferCapacity = 0u;
  }

  return ret;
}

/* !LINKSTO Tls.RFC.AEAD.Format,1 */
/* !LINKSTO Tls.RFC.Record.Cipher,1 */
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Encrypt(Tls_PtrConnection ConnectionPtr)
{
#if( TLS_CSM_KEYDERIVE_SUPPORT == STD_OFF )
  uint8 nonce[TLS_AEAD_NONCE_LENGTH];
#endif /* TLS_CSM_KEYDERIVE_SUPPORT == STD_OFF */
  Std_ReturnType ret = E_OK;
  uint8 headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);
  uint16 recordLength = Tls_GetRecordLength(ConnectionPtr, ConnectionPtr->transmitBuffer.buffer);
  uint8 explicitNonceLength = TLS_AEAD_NONCE_LENGTH - ConnectionPtr->cipherParams->IVLength;
  uint32 recordStart;

  TLS_PRINTF(("Tls_Encrypt begin\n"));

  TLS_DEBUG_PRINTBYTES("Record", ConnectionPtr->transmitBuffer.buffer, headerSize + recordLength);

  Tls_AEAD_GetAssociatedData(Tls_GetWriteSeqNum(ConnectionPtr), Tls_GetRecordType(ConnectionPtr->transmitBuffer.buffer),
      Tls_GetVersion(ConnectionPtr), recordLength, ConnectionPtr->encryptAssociatedData);

  TLS_DEBUG_PRINTBYTES("AssociatedData", ConnectionPtr->encryptAssociatedData, TLS_AEAD_ASSOCIATED_DATA_LENGTH);

#if( TLS_CSM_KEYDERIVE_SUPPORT == STD_OFF )
  TS_MemCpy(nonce, ConnectionPtr->writeIV, ConnectionPtr->cipherParams->IVLength);
  TS_MemCpy(&nonce[ConnectionPtr->cipherParams->IVLength], ConnectionPtr->encryptAssociatedData, explicitNonceLength);

  TLS_DEBUG_PRINTBYTES("Nonce", nonce, TLS_AEAD_NONCE_LENGTH);

  /* !LINKSTO Tls.RFC.AEAD.IV,1 */
  ret = Tls_Csm_KeyElementSet(ConnectionPtr->config->encryptKeyId, CRYPTO_KE_CIPHER_IV, nonce, TLS_AEAD_NONCE_LENGTH);
#else /* TLS_CSM_KEYDERIVE_SUPPORT == STD_ON */
  /* !LINKSTO Tls.Design.CsmKeyDerivation.AEADEncrypt,1 */
  ret = Tls_Csm_KeyElementSet(ConnectionPtr->config->encryptKeyId, CRYPTO_KE_CIPHER_IV, ConnectionPtr->encryptAssociatedData, explicitNonceLength);
#endif /* TLS_CSM_KEYDERIVE_SUPPORT == STD_OFF */

  if(ret == E_OK)
  {
    ret = Tls_ProvideTxBuffer(ConnectionPtr);
  }

  if(ret == E_OK)
  {
    recordStart = headerSize;

    ConnectionPtr->encryptMACLength = ConnectionPtr->cipherParams->MACLength;
    ConnectionPtr->encryptLength = recordLength;

    /* !LINKSTO Tls.RFC.AEAD.Encrypt,1 */
    ret = Csm_AEADEncrypt(ConnectionPtr->config->encryptJobId, CRYPTO_OPERATIONMODE_SINGLECALL,
      &ConnectionPtr->transmitBuffer.buffer[recordStart], recordLength,
      ConnectionPtr->encryptAssociatedData, TLS_AEAD_ASSOCIATED_DATA_LENGTH,
      &ConnectionPtr->encryptBuffer[headerSize + explicitNonceLength], &ConnectionPtr->encryptLength,
      ConnectionPtr->encryptMAC, &ConnectionPtr->encryptMACLength
    );
  }

  if(ret == E_OK)
  {
    TLS_PRINTF(("Tls_Encrypt success\n"));
  }
  else if(ret == CRYPTO_E_BUSY)
  {
    TLS_PRINTF(("Tls_Encrypt busy\n"));
  }
  else
  {
    TLS_PRINTF(("Tls_Encrypt fail\n"));
  }
  return ret;
}

STATIC FUNC(void, TLS_CODE) Tls_EncryptCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result)
{
  uint8 headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);
  uint16 explicitNonceLength = (uint16)TLS_AEAD_NONCE_LENGTH - (uint16)ConnectionPtr->cipherParams->IVLength;

  TLS_PRINTF(("Tls_EncryptCallback\n"));

  if(result == E_OK)
  {
    /* !LINKSTO Tls.RFC.AEAD.Format,1 */
    /* Encryption operation is ready */
    ConnectionPtr->encryptState = TLS_ENCRYPT_STATE_READY;
    /* Copy explicit nonce to the encrypted record. After header, before encrypted payload. */
    TS_MemCpy(&ConnectionPtr->encryptBuffer[headerSize], ConnectionPtr->encryptAssociatedData, explicitNonceLength);
    /* Copy header to encrypted record. The transmit buffer contains the plaintext record. */
    TS_MemCpy(ConnectionPtr->encryptBuffer, ConnectionPtr->transmitBuffer.buffer, headerSize);
    /* Change length of encrypted record as it also contains explicit nonce and MAC */
    Tls_SetRecordLength(ConnectionPtr, ConnectionPtr->encryptBuffer, ConnectionPtr->encryptLength + (uint32)explicitNonceLength + ConnectionPtr->encryptMACLength);
    /* Copy MAC to encrypted record */
    TS_MemCpy(&ConnectionPtr->encryptBuffer[(uint32)headerSize + (uint32)explicitNonceLength + ConnectionPtr->encryptLength], ConnectionPtr->encryptMAC, ConnectionPtr->encryptMACLength);
    /* Set total size of the encryption buffer */
    ConnectionPtr->encryptBufferLength = (uint16)headerSize + explicitNonceLength + (uint16)ConnectionPtr->encryptLength + (uint16)ConnectionPtr->encryptMACLength;
    TLS_PRINTF(("Csm_AEADEncrypt success, Encrypted record size: %d\n", ConnectionPtr->encryptBufferLength));
    /* !LINKSTO Dtls.RFC.Record.SequenceNumber,1 */
    ConnectionPtr->writeSeqNum++;
  }
  else
  {
    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
    ConnectionPtr->encryptState = TLS_ENCRYPT_STATE_ERROR;
    TLS_PRINTF(("Csm_AEADEncrypt failed\n"));
  }
}

/* !LINKSTO Tls.RFC.AEAD.Format,1 */
/* !LINKSTO Tls.RFC.Record.Cipher,1 */
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Decrypt(Tls_PtrConnection ConnectionPtr)
{
#if( TLS_CSM_KEYDERIVE_SUPPORT == STD_OFF )
  uint8 nonce[TLS_AEAD_NONCE_LENGTH];
#endif /* TLS_CSM_KEYDERIVE_SUPPORT == STD_OFF */
  Std_ReturnType ret = E_OK;
  Tls_PtrUint8 mac;
  uint32 macLength = ConnectionPtr->cipherParams->MACLength;
  Tls_PtrConstUint8 header;
  uint8 headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);
  uint8 explicitNonceLength = TLS_AEAD_NONCE_LENGTH - ConnectionPtr->cipherParams->IVLength;
  uint32 recordLength;
  Tls_PtrConstUint8 input;
  uint16 inputLength;

  TLS_PRINTF(("Tls_Decrypt begin\n"));

  /* Assume there is a complete record in the buffer */
  header = ConnectionPtr->receiveBuffer.buffer;
  recordLength = Tls_GetRecordLength(ConnectionPtr, header);

  if(recordLength < (macLength + explicitNonceLength))
  {
    TLS_PRINTF(("Record too short for decryption (%d bytes)\n", recordLength));
    /* !LINKSTO Tls.RFC.Alert.DecodeError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_DECODE_ERROR);
    ret = E_NOT_OK;
  }

  if(ret == E_OK)
  {
    uint8 explicitNonce[TLS_AEAD_EXPLICIT_NONCE_LENGTH];

    Tls_AEAD_GetAssociatedData(Tls_GetReadSeqNum(ConnectionPtr), Tls_GetRecordType(header),
                               Tls_GetRecordVersion(header), recordLength - explicitNonceLength - macLength , ConnectionPtr->decryptAssociatedData);

    Tls_AEAD_GetExplicitNonce(Tls_GetReadSeqNum(ConnectionPtr), explicitNonce);

    /* Get the MAC from the record. */
    mac = &ConnectionPtr->receiveBuffer.buffer[(headerSize + recordLength) - macLength];

    input = &ConnectionPtr->receiveBuffer.buffer[headerSize + explicitNonceLength];
    inputLength = recordLength - explicitNonceLength - macLength;

#if( TLS_CSM_KEYDERIVE_SUPPORT == STD_OFF )
    /* !LINKSTO Tls.RFC.AEAD.Format,1 */
    TS_MemCpy(nonce, ConnectionPtr->readIV, ConnectionPtr->cipherParams->IVLength);
    TS_MemCpy(&nonce[ConnectionPtr->cipherParams->IVLength], &ConnectionPtr->receiveBuffer.buffer[headerSize], explicitNonceLength);

    /* !LINKSTO Tls.RFC.AEAD.IV,1 */
    ret = Tls_Csm_KeyElementSet(ConnectionPtr->config->decryptKeyId, CRYPTO_KE_CIPHER_IV, nonce, TLS_AEAD_NONCE_LENGTH);

#else /* TLS_CSM_KEYDERIVE_SUPPORT == STD_ON */
  /* !LINKSTO Tls.Design.CsmKeyDerivation.AEADDecrypt,1 */
  ret = Tls_Csm_KeyElementSet(ConnectionPtr->config->decryptKeyId, CRYPTO_KE_CIPHER_IV, explicitNonce, explicitNonceLength);
#endif /* TLS_CSM_KEYDERIVE_SUPPORT == STD_OFF */

    if(ret != E_OK)
    {
      TLS_PRINTF(("Tls_Csm_KeyElementSet fail: %d\n", ret));
    }
    else
    {
      TLS_PRINTF(("RecordLength: %d\n", recordLength));
      TLS_PRINTF(("HeaderSize: %d\n", headerSize));
      TLS_DEBUG_PRINTBYTES("MAC", mac, macLength);
      TLS_DEBUG_PRINTBYTES("CipherText", input, inputLength);
      TLS_DEBUG_PRINTBYTES("Nonce", nonce, TLS_AEAD_NONCE_LENGTH);
      TLS_DEBUG_PRINTBYTES("AssociatedData", ConnectionPtr->decryptAssociatedData, TLS_AEAD_ASSOCIATED_DATA_LENGTH);


      ConnectionPtr->decryptLength = ConnectionPtr->decryptBuffer.size;

      /* !LINKSTO Tls.RFC.AEAD.Decrypt,1 */
      /* Start decryption. */
      ret = Csm_AEADDecrypt(ConnectionPtr->config->decryptJobId, CRYPTO_OPERATIONMODE_SINGLECALL,
                            input, inputLength,
                            ConnectionPtr->decryptAssociatedData, TLS_AEAD_ASSOCIATED_DATA_LENGTH,
                            mac, macLength,
                            &ConnectionPtr->decryptBuffer.buffer[headerSize], &ConnectionPtr->decryptLength,
                            &ConnectionPtr->decryptVerify
                           );

      if(ret != E_OK)
      {
        TLS_PRINTF(("Decrypt start failed: %d.\n", ret));
      }
      else
      {
        TLS_PRINTF(("Decrypt start success.\n"));
      }
    }
  }

  TLS_PRINTF(("Tls_Decrypt end\n"));

  return ret;
}

STATIC FUNC(void, TLS_CODE) Tls_DecryptCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result)
{
  uint8 headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);

  TLS_PRINTF(("Tls_DecryptCallback\n"));

  if(result == E_OK)
  {
    TLS_PRINTF(("Decrypt finish output length: %d\n", ConnectionPtr->decryptLength));
    TLS_DEBUG_PRINTBYTES("PlainText", &ConnectionPtr->decryptBuffer.buffer[headerSize], ConnectionPtr->decryptLength);
    ConnectionPtr->decryptState = TLS_DECRYPT_STATE_READY;
    if(ConnectionPtr->decryptVerify == CRYPTO_E_VER_OK)
    {
      TS_MemCpy(ConnectionPtr->decryptBuffer.buffer, ConnectionPtr->receiveBuffer.buffer, headerSize);
      Tls_SetRecordLength(ConnectionPtr, ConnectionPtr->decryptBuffer.buffer, ConnectionPtr->decryptLength);
      ConnectionPtr->decryptBuffer.count = ConnectionPtr->decryptLength + headerSize;
      TLS_PRINTF(("Tls_Decrypt verify ok.\n"));
    }
    else
    {
      TLS_PRINTF(("Tls_Decrypt verify NOT ok.\n"));

      if(ConnectionPtr->config->dtls == TRUE)
      {
        /* !LINKSTO Dtls.RFC.Record.Check,1 */
        /* Silently discard */
      }
      else
      {
        /* !LINKSTO Tls.RFC.Alert.BadRecordMAC,1 */
        Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_BAD_RECORD_MAC);
      }

      ConnectionPtr->decryptBuffer.count = 0u;
      ConnectionPtr->decryptState = TLS_DECRYPT_STATE_IDLE;
      (void)Tls_ConsumeRecordFromReceiveBuffer(ConnectionPtr);
    }
  }
  else
  {
    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
    ConnectionPtr->decryptState = TLS_DECRYPT_STATE_ERROR;
    ConnectionPtr->decryptBuffer.count = 0u;
  }
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_CopyIpAddress(Tls_PtrConnection ConnectionPtr, TcpIp_ConstSockAddrPtrType RemoteAddrPtr)
{
  Std_ReturnType ret = E_OK;

  /* Check for NULL pointer */
  if(RemoteAddrPtr == NULL_PTR)
  {
    ret = E_NOT_OK;
  }
  else
  {
    /* Pointer is valid. Check domain field. */
    if(RemoteAddrPtr->domain == TCPIP_AF_INET)
    {
      /* IPv4 address */
      /* Deviation MISRAC2012-1 */
      *((P2VAR(TcpIp_SockAddrInetType, AUTOMATIC, TLS_VAR))&ConnectionPtr->remoteAddr) = *(TcpIp_PtrConstSockAddrInet)RemoteAddrPtr;
    }
    else if(RemoteAddrPtr->domain == TCPIP_AF_INET6)
    {
      /* IPv6 address */
      /* Deviation MISRAC2012-1 */
      ConnectionPtr->remoteAddr = *(TcpIp_PtrConstSockAddrInet6)RemoteAddrPtr;
    }
    else
    {
      /* Unknown domain. Address cant be copied. */
      ret = E_NOT_OK;
    }
  }
  return ret;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_CheckIpAddress(Tls_PtrConnection ConnectionPtr, TcpIp_ConstSockAddrPtrType RemoteAddrPtr)
{
  Std_ReturnType ret = E_OK;
  P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TLS_VAR) ipv4;
  TcpIp_PtrConstSockAddrInet ipv4r;
  P2CONST(TcpIp_SockAddrInet6Type, AUTOMATIC, TLS_VAR) ipv6;
  TcpIp_PtrConstSockAddrInet6 ipv6r;

  /* Check domain */
  if(ConnectionPtr->remoteAddr.domain != RemoteAddrPtr->domain)
  {
    ret = E_NOT_OK;
  }
  else
  {
    /* Check address */
    if(RemoteAddrPtr->domain == TCPIP_AF_INET)
    {
      /* Deviation MISRAC2012-1 */
      ipv4 = (P2CONST(TcpIp_SockAddrInetType, AUTOMATIC, TLS_VAR))&ConnectionPtr->remoteAddr;
      /* Deviation MISRAC2012-1 */
      ipv4r = (TcpIp_PtrConstSockAddrInet)RemoteAddrPtr;

      if((ipv4->port != ipv4r->port) ||
          (ipv4->addr[0] != ipv4r->addr[0])
        )
      {
        ret = E_NOT_OK;
      }
    }
    else
    {
      /* RemoteAddrPtr->domain is TCPIP_AF_INET6 */

      ipv6 = &ConnectionPtr->remoteAddr;
      /* Deviation MISRAC2012-1 */
      ipv6r = (TcpIp_PtrConstSockAddrInet6)RemoteAddrPtr;

      if((ipv6->port != ipv6r->port) ||
          (ipv6->addr[0] != ipv6r->addr[0]) ||
          (ipv6->addr[1] != ipv6r->addr[1]) ||
          (ipv6->addr[2] != ipv6r->addr[2]) ||
          (ipv6->addr[3] != ipv6r->addr[3])
        )
      {
        ret = E_NOT_OK;
      }
    }
  }
  return ret;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_TransmitRecord(Tls_PtrConnection ConnectionPtr)
{
  Tls_PtrConstUint8 header;
  uint8 header_size = Tls_GetRecordHeaderSize(ConnectionPtr);
  Std_ReturnType ret = E_OK;
  uint16 length;

  /* This can not fail as TransmitRecord is only called with complete record in the buffer. */
  header = ConnectionPtr->transmitBuffer.buffer;
  length = Tls_GetRecordLength(ConnectionPtr, header);

  if((((ConnectionPtr->state == TLS_STATE_OK) || (ConnectionPtr->state == TLS_STATE_CHANGECIPHERSPEC_SENT))) &&
      ((ConnectionPtr->cipherParams->cipherType != TLS_NULL_CIPHER) || (ConnectionPtr->cipherParams->MACKeyLength > 0))
    )
  {
    /* Nothing to do. Record must be encrypted first. */
    TLS_PRINTF(("Tls_TransmitRecord: Wait for encryption\n"));
  }
  else
  {
    TLS_PRINTF(("Tls_TransmitRecord: Send without encryption\n"));
    /* No encryption necessary. Send immediately. */
    ret = Tls_ProvideTxBuffer(ConnectionPtr);
    if(ret == E_OK)
    {
      /* !LINKSTO Dtls.RFC.Record.Mapping,1 */
      /* Copy record to encryption buffer */
      (void)Tls_Buffer_Copy(&ConnectionPtr->transmitBuffer, ConnectionPtr->encryptBuffer, 0u, length + header_size);

      /* Remove record from transmit buffer */
      (void)Tls_ConsumeRecordFromTransmitBuffer(ConnectionPtr);
      /* Start transmission */
      ret = Tls_TcpIp_Transmit(ConnectionPtr, (uint32)length + (uint32)header_size);
      if(ret == E_OK)
      {
        ConnectionPtr->writeSeqNum++;
      }
    }
    else
    {
      /* Transmission failed because of TcpIp error */
      ConnectionPtr->transmissionError = TRUE;
    }
  }

  return ret;
}

STATIC FUNC(void, TLS_CODE) Tls_CloseInternal(Tls_PtrConnection ConnectionPtr)
{
  ConnectionPtr->closeRequest = TRUE;
}

STATIC FUNC(void, TLS_CODE) Tls_ChangeCipherSpec_Write(Tls_PtrConnection ConnectionPtr)
{
  uint8 headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);

  /* Write header */
  Tls_WriteRecordHeaderToTransmitBuffer(ConnectionPtr, ConnectionPtr->transmitBuffer.count, 1u, TLS_RECORD_CHANGECIPHERSPEC);
  ConnectionPtr->transmitBuffer.count += headerSize;

  /* Write data */
  (void)Tls_Buffer_AppendByte(&ConnectionPtr->transmitBuffer, 1u);
}

/* !LINKSTO Tls.RFC.HelloExtensions.Format,1 */
TS_MOD_PRIV_DEFN FUNC(uint16, TLS_CODE) Tls_Handshake_WriteExtension(Tls_PtrUint8 dest, uint16 extension_type, uint32 extension_len, uint8 * extension_payload)
{
  /* Extension type */
  TLS_SET16(dest, 0U, extension_type);
  dest += TLS_EXTENSION_TYPE_SIZE;

  /* Extension length */
  TLS_SET16(dest, 0U, extension_len);
  dest += TLS_EXTENSION_LENGTH_SIZE;

  if( TLS_IS_POINTER_VALID(extension_payload) )
  {
    /* Extension payload */
    TS_MemCpy(dest, extension_payload, extension_len);
  }
  else
  {
    /* should not happen */
    TLS_PRINTF(("Invalid extension payload\n"));
    TLS_UNREACHABLE_CODE_ASSERT(TLS_INTERNAL_SVCID);
  }

  return TLS_EXTENSION_TYPE_SIZE + TLS_EXTENSION_LENGTH_SIZE + extension_len;
}

TS_MOD_PRIV_DEFN FUNC(uint32, TLS_CODE) Tls_GetExtensionsLength(Tls_PtrConnection ConnectionPtr)
{
  uint16 length=0U;

  /* Extension: recordSizeLimit */
  if (ConnectionPtr->config->RecordSizeLimitExtensionEnabled == TRUE)
  {
    length += TLS_EXTENSION_TYPE_SIZE + TLS_EXTENSION_LENGTH_SIZE + TLS_EXTENSION_RECORDSIZELIMIT_SIZE;
  }

  /* Add extensions header size if at least one extension is used */
  if(length > 0U)
  {
    length += TLS_EXTENSIONS_HEADER_SIZE;
  }

  return length;
}

TS_MOD_PRIV_DEFN FUNC(uint16, TLS_CODE) Tls_Handshake_WriteExtension_RecordSizeLimit(Tls_PtrUint8 BufferPtr, uint16 recordSize)
{
  uint8 recordSizeLimitPayload[2];
  uint16 extensionSize;

  TLS_SET16(recordSizeLimitPayload, 0U, recordSize);

  extensionSize = Tls_Handshake_WriteExtension
                  (
                    BufferPtr,
                    TLS_EXTENSION_RECORDSIZELIMIT_ID,
                    TLS_EXTENSION_RECORDSIZELIMIT_SIZE,
                    recordSizeLimitPayload
                  );

  return extensionSize;
}

STATIC FUNC(void, TLS_CODE) Tls_MacMakeHeader(Tls_PtrUint8 buffer, uint32 length, uint8 type, uint16 version, Tls_Uint64Type seq_num)
{
  /*MAC(MAC_write_key, seq_num +
      TLSCompressed.type +
      TLSCompressed.version +
      TLSCompressed.length +
      TLSCompressed.fragment);*/

  buffer[0] = (uint8)((uint8)(seq_num >> 56) & 0xFFu);
  buffer[1] = (uint8)((uint8)(seq_num >> 48) & 0xFFu);
  buffer[2] = (uint8)((uint8)(seq_num >> 40) & 0xFFu);
  buffer[3] = (uint8)((uint8)(seq_num >> 32) & 0xFFu);
  buffer[4] = (uint8)((uint8)(seq_num >> 24) & 0xFFu);
  buffer[5] = (uint8)((uint8)(seq_num >> 16) & 0xFFu);
  buffer[6] = (uint8)((uint8)(seq_num >> 8) & 0xFFu);
  buffer[7] = (uint8)((uint8)(seq_num >> 0) & 0xFFu);
  buffer[8] = type;
  buffer[9] = (uint8)((uint8)(version >> 8) & 0xFFu);
  buffer[10] = (uint8)((uint8)(version >> 0) & 0xFFu);
  buffer[11] = (uint8)((uint8)(length >> 8) & 0xFFu);
  buffer[12] = (uint8)((uint8)(length >> 0) & 0xFFu);

  TLS_PRINTF(("Tls_MacMakeHeader: length=%d, seqnum=%d\n", length, (uint32)seq_num));
}

STATIC FUNC(void, TLS_CODE) Tls_MacVerifyCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result)
{
  /* TODO: ConnectionPtr->decryptVerify does not need to be evaluated here??? */
  if(result == E_OK)
  {
    switch(ConnectionPtr->macVerifyState)
    {
    case TLS_MAC_STATE_HEADER_BUSY:
      ConnectionPtr->macVerifyState = TLS_MAC_STATE_HEADER_READY;
      TLS_PRINTF(("macVerifyState = TLS_MAC_STATE_HEADER_READY\n"));
      break;
    case TLS_MAC_STATE_PAYLOAD_BUSY:
      ConnectionPtr->macVerifyState = TLS_MAC_STATE_PAYLOAD_READY;
      TLS_PRINTF(("macVerifyState = TLS_MAC_STATE_PAYLOAD_READY\n"));
      break;
    case TLS_MAC_STATE_FINISH_BUSY:
      ConnectionPtr->macVerifyState = TLS_MAC_STATE_READY;
      TLS_PRINTF(("macVerifyState = TLS_MAC_STATE_READY\n"));
      break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TLS_PRINTF(("Tls_MacVerifyCallback invalid state: %d\n", ConnectionPtr->macVerifyState));
      TLS_UNREACHABLE_CODE_ASSERT(TLS_INTERNAL_SVCID);
      break;
    /* CHECK: PARSE */
    }
  }
  else
  {
    /* Crypto job failed */
    /* An error occured, remove record from receive buffer. */
    (void)Tls_ConsumeRecordFromReceiveBuffer(ConnectionPtr);
    ConnectionPtr->macVerifyState = TLS_MAC_STATE_ERROR;
    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
  }
}

STATIC FUNC(void, TLS_CODE) Tls_MacGenerateCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result)
{
  if(result == E_OK)
  {
    switch(ConnectionPtr->macGenerateState)
    {
    case TLS_MAC_STATE_HEADER_BUSY:
      ConnectionPtr->macGenerateState = TLS_MAC_STATE_HEADER_READY;
      TLS_PRINTF(("macGenerateState = TLS_MAC_STATE_HEADER_READY\n"));
      break;
    case TLS_MAC_STATE_PAYLOAD_BUSY:
      ConnectionPtr->macGenerateState = TLS_MAC_STATE_PAYLOAD_READY;
      TLS_PRINTF(("macGenerateState = TLS_MAC_STATE_PAYLOAD_READY\n"));
      break;
    case TLS_MAC_STATE_FINISH_BUSY:
      ConnectionPtr->macGenerateState = TLS_MAC_STATE_READY;
      TLS_PRINTF(("macGenerateState = TLS_MAC_STATE_READY\n"));
      break;
    /* CHECK: NOPARSE */
    default:
      /* should not happen */
      TLS_PRINTF(("Tls_MacGenerateCallback invalid state: %d\n", ConnectionPtr->macGenerateState));
      TLS_UNREACHABLE_CODE_ASSERT(TLS_INTERNAL_SVCID);
      break;
    /* CHECK: PARSE */
    }
  }
  else
  {
    /* Crypto job failed */
    ConnectionPtr->macGenerateState = TLS_MAC_STATE_ERROR;
    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
  }
}

#if TLS_CERTIFICATE_SUPPORT == STD_ON
STATIC FUNC(void, TLS_CODE) Tls_VerifyCertificateCallback(Tls_PtrConnection ConnectionPtr, KeyM_CertificateStatusType result)
{
  if(result == KEYM_CERTIFICATE_VALID)
  {
    switch(ConnectionPtr->handshakeData.verifyCertState)
    {
      case TLS_VERIFY_CERT_STATE_BUSY:
        ConnectionPtr->handshakeData.verifyCertState = TLS_VERIFY_CERT_STATE_READY;
        TLS_PRINTF(("verifyCertState = TLS_VERIFY_CERT_STATE_READY\n"));
        break;
      /* CHECK: NOPARSE */
      default:
        /* Unreachable code */
        TLS_PRINTF(("Tls_VerifyCertificateCallback invalid state: %d\n", ConnectionPtr->handshakeData.verifyCertState));
        TLS_UNREACHABLE_CODE_ASSERT(TLS_INTERNAL_SVCID);
        break;
      /* CHECK: PARSE */
    }
  }
  else
  {
    /* KeyM_VerifyCertificate failed */
    ConnectionPtr->handshakeData.verifyCertState = TLS_VERIFY_CERT_STATE_ERROR;
    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    /* TODO: probably not the correct alert */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_BAD_CERTIFICATE);
  }
}

STATIC FUNC(void, TLS_CODE) Tls_SignatureGenerateCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result)
{
  if(result == E_OK)
  {
    TLS_PRECONDITION_ASSERT(ConnectionPtr->handshakeData.sigGenOutputLength == TLS_SIGNATURE_ED25519_SIZE, TLS_INTERNAL_SVCID);

    switch(ConnectionPtr->handshakeData.sigGenState)
    {
      case TLS_SIG_GEN_STATE_CLIENT_RANDOM_BUSY:
      {
        ConnectionPtr->handshakeData.sigGenState = TLS_SIG_GEN_STATE_CLIENT_RANDOM_READY;
        TLS_PRINTF(("genSigState = TLS_SIG_GEN_STATE_CLIENT_RANDOM_READY\n"));
        break;
      }
      case TLS_SIG_GEN_STATE_SERVER_RANDOM_BUSY:
      {
        ConnectionPtr->handshakeData.sigGenState = TLS_SIG_GEN_STATE_SERVER_RANDOM_READY;
        TLS_PRINTF(("genSigState = TLS_SIG_GEN_STATE_SERVER_RANDOM_READY\n"));
        break;
      }
      case TLS_SIG_GEN_STATE_SKEX_PARAMS_BUSY:
      {
        ConnectionPtr->handshakeData.sigGenState = TLS_SIG_GEN_STATE_SKEX_PARAMS_READY;
        TLS_PRINTF(("genSigState = TLS_SIG_GEN_STATE_SKEX_PARAMS_READY\n"));
        break;
      }
      case TLS_SIG_GEN_STATE_FINISH_BUSY:
      {
        ConnectionPtr->handshakeData.sigGenState = TLS_SIG_GEN_STATE_READY;
        TLS_PRINTF(("genSigState = TLS_SIG_GEN_STATE_READY\n"));
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
        /* Unreachable code */
        TLS_PRINTF(("Tls_SignatureGenerateCallback invalid state: %d\n", ConnectionPtr->handshakeData.sigGenState));
        TLS_UNREACHABLE_CODE_ASSERT(TLS_INTERNAL_SVCID);
        break;
      }
      /* CHECK: PARSE */
    }
  }
  else
  {
    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
    ConnectionPtr->handshakeData.sigGenState = TLS_SIG_GEN_STATE_ERROR;
  }
}

STATIC FUNC(void, TLS_CODE) Tls_SignatureVerifyCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result)
{
  if(result == E_OK)
  {
    switch(ConnectionPtr->handshakeData.sigVerState)
    {
      case TLS_SIG_VER_STATE_BUSY:
        if(ConnectionPtr->handshakeData.sigVerifyResult == CRYPTO_E_VER_OK)
        {
          ConnectionPtr->handshakeData.sigVerState = TLS_SIG_VER_STATE_READY;
          TLS_PRINTF(("verSigState = TLS_SIG_VER_STATE_READY\n"));
        }
        else
        {
          /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
          Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
          ConnectionPtr->handshakeData.sigVerState = TLS_SIG_VER_STATE_ERROR;
        }
        break;
      /* CHECK: NOPARSE */
      default:
        /* Unreachable code */
        TLS_PRINTF(("Tls_SignatureVerifyCallback invalid state: %d\n", ConnectionPtr->handshakeData.sigVerState));
        TLS_UNREACHABLE_CODE_ASSERT(TLS_INTERNAL_SVCID);
        break;
      /* CHECK: PARSE */
    }
  }
  else
  {
    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
    ConnectionPtr->handshakeData.sigVerState = TLS_SIG_VER_STATE_ERROR;
  }
}

STATIC FUNC(void, TLS_CODE) Tls_JobKeyExchangeCalcPubValCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result)
{
  if(result == E_OK)
  {
    TLS_PRECONDITION_ASSERT(ConnectionPtr->handshakeData.keCalcPubValLength == TLS_KEY_ED25519_SIZE, TLS_INTERNAL_SVCID);

    switch(ConnectionPtr->handshakeData.keCalcPubValState)
    {
      case TLS_KE_CALC_PUB_VAL_STATE_BUSY:
      {
        ConnectionPtr->handshakeData.keCalcPubValState = TLS_KE_CALC_PUB_VAL_STATE_READY;
        TLS_PRINTF(("keCalcPubValState = TLS_KE_CALC_PUB_VAL_STATE_READY\n"));
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
        /* Unreachable code */
        TLS_PRINTF(("Tls_JobKeyExchangeCalcPubValCallback invalid state: %d\n", ConnectionPtr->handshakeData.keCalcPubValState));
        TLS_UNREACHABLE_CODE_ASSERT(TLS_INTERNAL_SVCID);
        break;
      }
      /* CHECK: PARSE */
    }
  }
  else
  {
    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
    ConnectionPtr->handshakeData.keCalcPubValState = TLS_KE_CALC_PUB_VAL_STATE_ERROR;
  }
}

STATIC FUNC(void, TLS_CODE) Tls_JobKeyExchangeCalcSecretCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result)
{
  if(result == E_OK)
  {
    switch(ConnectionPtr->handshakeData.keCalcSecretState)
    {
      case TLS_KE_CALC_SECRET_STATE_BUSY:
      {
        ConnectionPtr->handshakeData.keCalcSecretState = TLS_KE_CALC_SECRET_STATE_READY;
        TLS_PRINTF(("keCalcSecretState = TLS_KE_CALC_SECRET_STATE_READY\n"));
        break;
      }
      /* CHECK: NOPARSE */
      default:
      {
        /* Unreachable code */
        TLS_PRINTF(("Tls_JobKeyExchangeCalcSecretCallback invalid state: %d\n", ConnectionPtr->handshakeData.keCalcSecretState));
        TLS_UNREACHABLE_CODE_ASSERT(TLS_INTERNAL_SVCID);
        break;
      }
      /* CHECK: PARSE */
    }
  }
  else
  {
    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
    ConnectionPtr->handshakeData.keCalcSecretState = TLS_KE_CALC_SECRET_STATE_ERROR;
  }
}
#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */

STATIC FUNC(void, TLS_CODE) Tls_Handshake_WriteFinished(Tls_PtrConnection ConnectionPtr)
{
  Tls_PtrConstUint8 verify_data = (ConnectionPtr->config->entity == TLS_CLIENT) ?
      ConnectionPtr->handshakeData.clientVerifyData : ConnectionPtr->handshakeData.serverVerifyData;
  uint8 handshakeHeaderSize = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  uint8 headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);

  TLS_ENTER_EXCLUSIVE_AREA_TX();

  /* Write record header */
  Tls_WriteRecordHeaderToTransmitBuffer(ConnectionPtr, ConnectionPtr->transmitBuffer.count,
      (uint16)TLS_VERIFY_DATA_LENGTH + (uint16)handshakeHeaderSize, TLS_RECORD_HANDSHAKE);
  ConnectionPtr->transmitBuffer.count += headerSize;

  /* Write handshake header */
  Tls_Handshake_WriteHeader(ConnectionPtr, &ConnectionPtr->transmitBuffer.buffer[ConnectionPtr->transmitBuffer.count],
      TLS_HANDSHAKE_FINISHED, TLS_VERIFY_DATA_LENGTH);
  ConnectionPtr->transmitBuffer.count += handshakeHeaderSize;

  TLS_EXIT_EXCLUSIVE_AREA_TX();

  /* Write verify data */
  (void)Tls_Buffer_Append(&ConnectionPtr->transmitBuffer, verify_data, TLS_VERIFY_DATA_LENGTH);
}

/* !LINKSTO Tls.RFC.Handshake.ClientKeyExchange,1 */
STATIC FUNC(uint32, TLS_CODE) Tls_Handshake_WriteClientKeyExchangePsk(Tls_PtrConnection ConnectionPtr)
{
  uint32 ret = 0u;
  uint32 handshakeLength = (uint32)2u + ConnectionPtr->config->identityLength;
  uint8 handshakeHeaderSize = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  uint8 headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);

  TLS_ENTER_EXCLUSIVE_AREA_TX();

  /* Write record header */
  Tls_WriteRecordHeaderToTransmitBuffer(ConnectionPtr, ConnectionPtr->transmitBuffer.count,
      handshakeLength + handshakeHeaderSize, TLS_RECORD_HANDSHAKE);
  ConnectionPtr->transmitBuffer.count += headerSize;

  /* Write handshake header */
  Tls_Handshake_WriteHeader(ConnectionPtr, &ConnectionPtr->transmitBuffer.buffer[ConnectionPtr->transmitBuffer.count],
      TLS_HANDSHAKE_CLIENTKEYEXCHANGE, handshakeLength);
  ConnectionPtr->transmitBuffer.count += handshakeHeaderSize;

  /*identity length*/
  (void)Tls_Buffer_AppendByte(&ConnectionPtr->transmitBuffer, (ConnectionPtr->config->identityLength >> 8) & 0xFFu);
  (void)Tls_Buffer_AppendByte(&ConnectionPtr->transmitBuffer, ConnectionPtr->config->identityLength & 0xFFu);

  /* identity */
  (void)Tls_Buffer_Append(&ConnectionPtr->transmitBuffer, ConnectionPtr->config->identity, ConnectionPtr->config->identityLength);

  TLS_EXIT_EXCLUSIVE_AREA_TX();

  /* Return the length of the record payload as it is needed to compute verify data */
  ret = handshakeLength + handshakeHeaderSize;


  return ret;
}

STATIC FUNC(void, TLS_CODE) Tls_SetDefaults(Tls_PtrConnection ConnectionPtr)
{
  /* Set all values to default */
  /* !LINKSTO Dtls.RFC.Record.SequenceNumber.Reset,1 */
  ConnectionPtr->readSeqNum = 0u;
  ConnectionPtr->writeSeqNum = 0u;
  ConnectionPtr->errorFlag = FALSE;
  ConnectionPtr->state = TLS_STATE_DEFAULT;
  ConnectionPtr->active = FALSE;
  ConnectionPtr->cipher = 0u;
  ConnectionPtr->cipherParams = NULL_PTR;
  ConnectionPtr->config = NULL_PTR;
  TS_MemSet(ConnectionPtr->decryptAssociatedData, 0u, TLS_AEAD_ASSOCIATED_DATA_LENGTH);
  ConnectionPtr->decryptBuffer.buffer = NULL_PTR;
  ConnectionPtr->decryptBuffer.count = 0u;
  ConnectionPtr->decryptBuffer.size = 0u;
  ConnectionPtr->decryptLength = 0u;
  ConnectionPtr->decryptState = TLS_DECRYPT_STATE_IDLE;
  /* !LINKSTO Dtls.RFC.Record.Epoch.Init,1 */
  ConnectionPtr->dtls.readEpoch = 0u;
  ConnectionPtr->dtls.writeEpoch = 0u;
  ConnectionPtr->dtls.ethTxHandle.bufferPtr = NULL_PTR;
  ConnectionPtr->dtls.ethTxHandle.bufferHandle = 0u;
  ConnectionPtr->dtls.ethTxHandle.bufferLength = 0u;
  ConnectionPtr->dtls.receiveWindow = 0u;
  ConnectionPtr->remoteAddr.addr[0u] = 0u;
  ConnectionPtr->remoteAddr.addr[1u] = 0u;
  ConnectionPtr->remoteAddr.addr[2u] = 0u;
  ConnectionPtr->remoteAddr.addr[3u] = 0u;
  ConnectionPtr->remoteAddr.domain = TCPIP_AF_UNSPEC;
  ConnectionPtr->remoteAddr.port = 0u;
  TS_MemSet(ConnectionPtr->encryptAssociatedData, 0u, TLS_AEAD_ASSOCIATED_DATA_LENGTH);
  ConnectionPtr->encryptBuffer = NULL_PTR;
  ConnectionPtr->encryptLength = 0u;
  ConnectionPtr->encryptState = TLS_ENCRYPT_STATE_IDLE;
  ConnectionPtr->encryptBufferCapacity = 0u;
  ConnectionPtr->encryptBufferLength = 0u;
  ConnectionPtr->macGenerateState = TLS_MAC_STATE_IDLE;
  ConnectionPtr->macVerifyState = TLS_MAC_STATE_IDLE;
  TS_MemSet(ConnectionPtr->encryptMAC, 0u, TLS_AEAD_MAX_MAC_LENGTH);
  ConnectionPtr->encryptMACLength = 0u;
  TS_MemSet(ConnectionPtr->handshakeData.clientRandom, 0u, TLS_RANDOM_SIZE);
  TS_MemSet(ConnectionPtr->handshakeData.clientVerifyData, 0u, TLS_VERIFY_DATA_LENGTH);
  ConnectionPtr->handshakeData.clientVerifyDataState = TLS_VERIFYDATA_STATE_IDLE;
  TS_MemSet(ConnectionPtr->handshakeData.dtls_cookie, 0u, TLS_DTLS_MAX_COOKIE_LENGTH);
  ConnectionPtr->handshakeData.dtls_cookieLength = 0u;
  ConnectionPtr->handshakeData.dtls_cookieState = TLS_RANDOM_STATE_IDLE;
  /* !LINKSTO Dtls.RFC.Handshake.MessageSeq.Init,1 */
  ConnectionPtr->handshakeData.dtls_handshakeSeq = 0u;
  ConnectionPtr->handshakeData.dtls_nextReceiveSeq = 0u;
  ConnectionPtr->handshakeData.dtls_helloCount = 0u;
  TS_MemSet(ConnectionPtr->handshakeData.dtls_retransmitBuffer, 0u, TLS_DTLS_RETRANSMIT_BUFFER_SIZE);
  ConnectionPtr->handshakeData.dtls_retransmitBufferCount = 0u;
  ConnectionPtr->handshakeData.dtls_retransmitCount = 0u;
  ConnectionPtr->handshakeData.dtls_retransmitTimer = 0u;
  TS_MemSet(ConnectionPtr->handshakeData.handshakeFragmentBuffer, 0u, TLS_HANDSHAKE_FRAGMENT_BUFFER_SIZE);
  ConnectionPtr->handshakeData.handshakeFragmentBufferLength = 0u;
  TS_MemSet(ConnectionPtr->handshakeData.handshakeMessages, 0u, TLS_HANDSHAKE_VERIFYDATA_BUFFER_SIZE);
  ConnectionPtr->handshakeData.handshakeMessagesLength = 0u;
  ConnectionPtr->handshakeData.handshakeMessagesOverflow = FALSE;
  ConnectionPtr->handshakeData.hashLength = 0u;
  ConnectionPtr->handshakeData.keyDeriveState = TLS_KEYDERIVE_STATE_IDLE;
  ConnectionPtr->handshakeData.prf.jobId = 0u;
  ConnectionPtr->handshakeData.prf.len = 0u;
  ConnectionPtr->handshakeData.prf.pos = 0u;
  TS_MemSet(ConnectionPtr->handshakeData.prf.r, 0u, TLS_PRF_BLOCK_SIZE);
  TS_MemSet(ConnectionPtr->handshakeData.prf.result, 0u, TLS_PRF_RESULT_MAX_LENGTH);
  ConnectionPtr->handshakeData.prf.resultLength = 0u;
  TS_MemSet(ConnectionPtr->handshakeData.prf.seed, 0u, TLS_PRF_SEED_MAX_LENGTH);
  ConnectionPtr->handshakeData.prf.seedLength = 0u;
  ConnectionPtr->handshakeData.prf.state = TLS_PRF_STATE_IDLE;
  TS_MemSet(ConnectionPtr->handshakeData.prf.tmp, 0u, TLS_PRF_BLOCK_SIZE + TLS_PRF_SEED_MAX_LENGTH);
  TS_MemSet(ConnectionPtr->handshakeData.prf.tmp2, 0u, TLS_PRF_BLOCK_SIZE);
  ConnectionPtr->handshakeData.randomSize = 0u;
  ConnectionPtr->handshakeData.randomState = TLS_RANDOM_STATE_IDLE;
  TS_MemSet(ConnectionPtr->handshakeData.receivedVerifyData, 0u, TLS_VERIFY_DATA_LENGTH);
  TS_MemSet(ConnectionPtr->handshakeData.serverRandom, 0u, TLS_RANDOM_SIZE);
  TS_MemSet(ConnectionPtr->handshakeData.serverVerifyData, 0u, TLS_VERIFY_DATA_LENGTH);
  ConnectionPtr->handshakeData.serverVerifyDataState = TLS_VERIFYDATA_STATE_IDLE;
  TS_MemSet(ConnectionPtr->handshakeData.verifyDataSeed, 0u, TLS_SHA256_SIZE + TLS_FINISHEDLABEL_MAX_LENGTH);
  ConnectionPtr->handshakeData.verifyCertState = TLS_VERIFY_CERT_STATE_IDLE;
  ConnectionPtr->handshakeData.sigGenState = TLS_SIG_GEN_STATE_IDLE;
  ConnectionPtr->handshakeData.sigVerState = TLS_SIG_VER_STATE_IDLE;
  ConnectionPtr->handshakeData.keCalcPubValState = TLS_KE_CALC_PUB_VAL_STATE_IDLE;
  ConnectionPtr->handshakeData.keCalcSecretState = TLS_KE_CALC_SECRET_STATE_IDLE;
  ConnectionPtr->txConfirmations.confirmed = 0u;
  ConnectionPtr->txConfirmations.count = 0u;
  ConnectionPtr->txConfirmations.start = 0u;
  ConnectionPtr->closeRequest = FALSE;
  ConnectionPtr->transmissionError = FALSE;
  ConnectionPtr->transmitRecordSizeLimit = 0u;
}

FUNC(Std_ReturnType, TLS_CODE) Tls_Enable(Tls_ConnectionIdType ConnectionId, TcpIp_SocketIdType SocketId)
{
  Std_ReturnType ret = E_OK;
  Tls_PtrConnection ConnectionPtr = NULL_PTR;
  uint32 i;

#if(TLS_DEV_ERROR_DETECT == STD_ON)
  if (Tls_Initialized == FALSE)
  {
    /* !LINKSTO Tls.API.Tls_Enable.Det.NoInit,1 */
    TLS_DET_REPORT_ERROR(TLS_ENABLE_SVCID, TLS_E_UNINIT);
    ret = E_NOT_OK;
  }
  else
#endif
  {
    if (ConnectionId >= TLS_CFG_CONNECTION_COUNT)
    {
      ret = E_NOT_OK;
    }

    if (ret == E_OK)
    {
      ConnectionPtr = &Tls_Connections[ConnectionId];
      if ((ConnectionPtr->active == TRUE) && (ConnectionPtr->socket != SocketId))
      {
        /* ConnectionPtr is in use */
        ret = E_NOT_OK;
      }
    }

    if((ret == E_OK) && (ConnectionPtr->active == TRUE) && (ConnectionPtr->state != TLS_STATE_ERROR))
    {
      /* !LINKSTO Tls.API.Tls_Enable.InUse,1 */
      ret = E_NOT_OK;
    }

    if (ret == E_OK)
    {
      Tls_SetDefaults(ConnectionPtr);
      ConnectionPtr->config = &Tls_Config.ConnectionConfigs[ConnectionId];

      ret = E_NOT_OK;
      for(i=0u;i<TLS_CIPHERSUITE_COUNT;++i)
      {
        if(Tls_CipherSuites[i].cipher == ConnectionPtr->config->cipher)
        {
          ret = E_OK;
          ConnectionPtr->cipherParams = &Tls_CipherSuites[i];
          break;
        }
      }
    }

    if (ret == E_OK)
    {
      ConnectionPtr->socket = SocketId;
      ConnectionPtr->active = TRUE;
      ConnectionPtr->handshakeData.randomState = TLS_RANDOM_STATE_START;

      ConnectionPtr->transmitRecordSizeLimit = ConnectionPtr->config->maxRecordSize;

      Tls_Buffer_Init(&ConnectionPtr->transmitBuffer, ConnectionPtr->config->transmitBufferPtr, ConnectionPtr->config->transmitBufferSize);
      Tls_Buffer_Init(&ConnectionPtr->receiveBuffer, ConnectionPtr->config->receiveBufferPtr, ConnectionPtr->config->receiveBufferSize);
      Tls_Buffer_Init(&ConnectionPtr->decryptBuffer, ConnectionPtr->config->decryptBufferPtr, ConnectionPtr->config->decryptBufferSize);
    }

    if(ret != E_OK)
    {
      TLS_PRINTF(("Tls_Enable failed.\n"));
    }
    else
    {
      TLS_PRINTF(("Tls_Enable success.\n"));
    }
  }
  return ret;
}

/* !LINKSTO Tls.RFC.Handshake.ClientHello,1 */
STATIC FUNC(uint32, TLS_CODE) Tls_Handshake_WriteClientHello(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 random32byte)
{
  uint32 ret = 0u;
  uint16 version = Tls_GetVersion(ConnectionPtr);
  uint32 handshakeLength = (uint32)9u + TLS_RANDOM_SIZE;
  uint8 handshakeHeaderSize = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  uint8 headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);
  Tls_PtrConstConnectionConfig configPtr = ConnectionPtr->config;

  if (configPtr->dtls)
  {
    handshakeLength += (uint32)1u + ConnectionPtr->handshakeData.dtls_cookieLength;
  }

  /* Reserve space for extensions */
  handshakeLength += Tls_GetExtensionsLength(ConnectionPtr);

  TLS_ENTER_EXCLUSIVE_AREA_TX();

  /* Write record header */
  Tls_WriteRecordHeaderToTransmitBuffer(ConnectionPtr, ConnectionPtr->transmitBuffer.count,
      handshakeLength + handshakeHeaderSize, TLS_RECORD_HANDSHAKE);
  ConnectionPtr->transmitBuffer.count += headerSize;

  /* Write handshake header */
  Tls_Handshake_WriteHeader(ConnectionPtr, &ConnectionPtr->transmitBuffer.buffer[ConnectionPtr->transmitBuffer.count],
      TLS_HANDSHAKE_CLIENTHELLO, handshakeLength);
  ConnectionPtr->transmitBuffer.count += handshakeHeaderSize;

  (void)Tls_Buffer_AppendByte(&ConnectionPtr->transmitBuffer, (version >> 8) & 0xFFu);
  (void)Tls_Buffer_AppendByte(&ConnectionPtr->transmitBuffer, version & 0xFFu);

  (void)Tls_Buffer_Append(&ConnectionPtr->transmitBuffer, random32byte, TLS_RANDOM_SIZE);

  /* !LINKSTO Tls.SessionResumption,1 */
  /*Session id length (No session resumption) */
  (void)Tls_Buffer_AppendByte(&ConnectionPtr->transmitBuffer, 0u);

  /* !LINKSTO Dtls.RFC.Handshake.ClientHello.Format,1 */
  if (ConnectionPtr->config->dtls)
  {
    /* cookie length */
    (void)Tls_Buffer_AppendByte(&ConnectionPtr->transmitBuffer, ConnectionPtr->handshakeData.dtls_cookieLength);

    /* cookie */
    (void)Tls_Buffer_Append(&ConnectionPtr->transmitBuffer, ConnectionPtr->handshakeData.dtls_cookie, ConnectionPtr->handshakeData.dtls_cookieLength);
  }

  /* cipher suites length */
  (void)Tls_Buffer_AppendByte(&ConnectionPtr->transmitBuffer, 0u);
  (void)Tls_Buffer_AppendByte(&ConnectionPtr->transmitBuffer, 2u);

  /* Cipher Suite: */
  /* !LINKSTO Tls.RFC.Handshake.ClientHello.Ciphersuite,1 */
  (void)Tls_Buffer_AppendByte(&ConnectionPtr->transmitBuffer, ConnectionPtr->config->cipher >> 8u);
  (void)Tls_Buffer_AppendByte(&ConnectionPtr->transmitBuffer, ConnectionPtr->config->cipher & 0xFFu);

  /* Compression Methods Length: 1 */
  (void)Tls_Buffer_AppendByte(&ConnectionPtr->transmitBuffer, 1u);
  /* Compression Method: null (0) */
  (void)Tls_Buffer_AppendByte(&ConnectionPtr->transmitBuffer, 0u);

  /* Handle extensions */
  /* !LINKSTO Tls.RFC.Handshake.ClientHello.Extensions,1 */
  {
    uint16 extensionsSize = 0U;
    /* Reserve space for extensions total size */
    uint16 BufferPos = ConnectionPtr->transmitBuffer.count + TLS_EXTENSIONS_HEADER_SIZE;

    /* Extension: record_size_limit */
    if( configPtr->RecordSizeLimitExtensionEnabled )
    {
      extensionsSize += Tls_Handshake_WriteExtension_RecordSizeLimit
                            (
                              &ConnectionPtr->transmitBuffer.buffer[BufferPos],
                              ConnectionPtr->config->maxRecordSize
                            );
    }

    /* If at least one extension is used, add the extensions payload to the clientHello record */
    if(extensionsSize > 0U)
    {
      /* Set total length of all extensions */
      TLS_SET16(&ConnectionPtr->transmitBuffer.buffer[ConnectionPtr->transmitBuffer.count], 0U, extensionsSize);

      /* Increment buffer count */
      ConnectionPtr->transmitBuffer.count += extensionsSize + TLS_EXTENSIONS_HEADER_SIZE;
    }
  }

  TLS_EXIT_EXCLUSIVE_AREA_TX();

  /* Return the length of the record payload as it is needed to compute verify data */
  ret = handshakeLength + handshakeHeaderSize;

  return ret;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_GenerateVerifyDataStart
(
    Tls_PtrConnection ConnectionPtr,
    P2CONST(uint8, AUTOMATIC, TLS_CONST) label, uint32 label_length
)
{
  /* hash all handshake messages
     PRF(master_secret, finished_label, Hash(handshake_messages))[0..verify_data_length - 1];
  */
  Std_ReturnType ret = E_OK;
  TLS_PRINTF(("Tls_Handshake_GenerateVerifyDataStart\n"));

  if(ConnectionPtr->handshakeData.handshakeMessagesOverflow == TRUE)
  {
    ret = E_NOT_OK;
  }
  else
  {
    ConnectionPtr->handshakeData.hashLength = TLS_SHA256_SIZE;

    TS_MemCpy(ConnectionPtr->handshakeData.verifyDataSeed, label, label_length);

    TLS_DEBUG_PRINTBYTES("Csm_Hash data", ConnectionPtr->handshakeData.handshakeMessages, ConnectionPtr->handshakeData.handshakeMessagesLength);
    ret = Csm_Hash(ConnectionPtr->config->hashJobId, CRYPTO_OPERATIONMODE_SINGLECALL,
                   ConnectionPtr->handshakeData.handshakeMessages, ConnectionPtr->handshakeData.handshakeMessagesLength,
                   &ConnectionPtr->handshakeData.verifyDataSeed[label_length], &ConnectionPtr->handshakeData.hashLength
                  );
    TLS_PRINTF(("Csm_Hash return: %d.\n", ret));
  }

  return ret;
}

/* TODO: Check if clientVerifyDataState and serverVerifyDataState SM can be started in parallel? */
STATIC FUNC(void, TLS_CODE) Tls_HashCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result)
{
  if(ConnectionPtr->handshakeData.clientVerifyDataState == TLS_VERIFYDATA_STATE_HASH_BUSY)
  {
    TLS_PRINTF(("clientVerifyData Hash ready.\n"));
    if(result == E_OK)
    {
      ConnectionPtr->handshakeData.clientVerifyDataState = TLS_VERIFYDATA_STATE_HASH_READY;
    }
    else
    {
      ConnectionPtr->handshakeData.clientVerifyDataState = TLS_VERIFYDATA_STATE_ERROR;
    }
  }
  else
  {
    /* Assume serverVerifyDataState == TLS_VERIFYDATA_STATE_HASH_BUSY */
    TLS_PRINTF(("serverVerifyData Hash ready.\n"));
    if(result == E_OK)
    {
      ConnectionPtr->handshakeData.serverVerifyDataState = TLS_VERIFYDATA_STATE_HASH_READY;
    }
    else
    {
      ConnectionPtr->handshakeData.serverVerifyDataState = TLS_VERIFYDATA_STATE_ERROR;
    }
  }

  if(result != E_OK)
  {
    /* Hash operation failed. Close connection. */
    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
  }
}

STATIC FUNC(void, TLS_CODE) Tls_Handshake_GenerateClientVerifyDataStart(Tls_PtrConnection ConnectionPtr)
{
  if(ConnectionPtr->handshakeData.clientVerifyDataState == TLS_VERIFYDATA_STATE_IDLE)
  {
    TLS_PRINTF(("Tls_Handshake_GenerateClientVerifyDataStart\n"));
    ConnectionPtr->handshakeData.clientVerifyDataState = TLS_VERIFYDATA_STATE_START;
  }
}

STATIC FUNC(void, TLS_CODE) Tls_Handshake_GenerateServerVerifyDataStart(Tls_PtrConnection ConnectionPtr)
{
  /* Defensive check. serverVerifyDataState should be always IDLE at this point. */
  /* CHECK: NOPARSE */
  if(ConnectionPtr->handshakeData.serverVerifyDataState == TLS_VERIFYDATA_STATE_IDLE)
  {
    TLS_PRINTF(("Tls_Handshake_GenerateServerVerifyDataStart\n"));
    ConnectionPtr->handshakeData.serverVerifyDataState = TLS_VERIFYDATA_STATE_START;
  }
  /* CHECK: PARSE */
}

STATIC FUNC(uint8, TLS_CODE) Tls_Handshake_GetType(Tls_PtrConstUint8 handshake)
{
  return handshake[0];
}

STATIC FUNC(uint32, TLS_CODE) Tls_Dtls_Handshake_GetFragmentLength(Tls_PtrConstUint8 handshake)
{
  return ((uint32)handshake[9] << 16) | ((uint32)handshake[10] << 8) | ((uint32)handshake[11] << 0);
}

STATIC FUNC(uint32, TLS_CODE) Tls_Dtls_Handshake_GetFragmentOffset(Tls_PtrConstUint8 handshake)
{
  return ((uint32)handshake[6] << 16) | ((uint32)handshake[7] << 8) | ((uint32)handshake[8] << 0);
}

STATIC FUNC(uint16, TLS_CODE) Tls_Dtls_Handshake_GetMessageSequence(Tls_PtrConstUint8 handshakeHeaderPtr)
{
  return ((uint16)handshakeHeaderPtr[4] << 8) | ((uint16)handshakeHeaderPtr[5] << 0);
}

STATIC FUNC(uint16, TLS_CODE) Tls_Handshake_Hello_GetVersion(Tls_PtrConstUint8 handshake)
{
  uint16 version = (uint16)(((uint16)handshake[0] << 8) | (uint16)handshake[1]);
  return version;
}

STATIC FUNC(Tls_PtrConstUint8, TLS_CODE) Tls_Handshake_Hello_GetRandom(Tls_PtrConstUint8 handshake)
{
  return &handshake[2];
}

STATIC FUNC(void, TLS_CODE) Tls_Handshake_Append(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 handshake_data, uint32 length)
{
  Tls_PtrHandshakeData data = &ConnectionPtr->handshakeData;
  TLS_PRINTF(("Append handshake: "));
  TLS_DEBUG_PRINTHANDSHAKETYPE(Tls_Handshake_GetType(handshake_data));
  TLS_PRINTF(("\n"));

  if((data->handshakeMessagesLength + length) > TLS_HANDSHAKE_VERIFYDATA_BUFFER_SIZE)
  {
    TLS_PRINTF(("Handshake buffer full."));
    data->handshakeMessagesOverflow = TRUE;
  }
  else
  {
    TS_MemCpy(&data->handshakeMessages[data->handshakeMessagesLength], handshake_data, length);
    data->handshakeMessagesLength += length;
  }
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_CheckVerifyData(Tls_PtrConnection ConnectionPtr)
{
  Tls_PtrHandshakeData data = &ConnectionPtr->handshakeData;
  Std_ReturnType ret = E_OK;

  Tls_PtrConstUint8 verify_data = (ConnectionPtr->config->entity == TLS_CLIENT) ? data->serverVerifyData : data->clientVerifyData;

  if (TS_MemCmp32(data->receivedVerifyData, verify_data, TLS_VERIFY_DATA_LENGTH) == 0u)
  {
    TLS_PRINTF(("Verify data ok.\n"));
  }
  else
  {
    TLS_PRINTF(("Verify data fail.\n"));
    TLS_DEBUG_PRINTBYTES("verify_data", verify_data, TLS_VERIFY_DATA_LENGTH);
    TLS_DEBUG_PRINTBYTES("received data", data->receivedVerifyData, TLS_VERIFY_DATA_LENGTH);
    ret = E_NOT_OK;
  }
  return ret;
}

STATIC FUNC(uint16, TLS_CODE) Tls_Dtls_GetEpoch(Tls_PtrConstUint8 record)
{
    return (((uint16)(record[3] << 8)) | ((uint16)record[4] << 0));
}

STATIC FUNC(Tls_Uint64Type, TLS_CODE) Tls_Dtls_GetSeqNum(Tls_PtrConstUint8 record)
{
    return (((Tls_Uint64Type)record[5] << 40) | ((Tls_Uint64Type)record[6] << 32) | ((Tls_Uint64Type)record[7] << 24) |
         ((Tls_Uint64Type)record[8] << 16) | ((Tls_Uint64Type)record[9] << 8) | ((Tls_Uint64Type)record[10] << 0));
}

STATIC FUNC(Tls_Uint64Type, TLS_CODE) Tls_Dtls_GetEpochAndSeqNum(Tls_PtrConstUint8 record)
{
    return ((Tls_Uint64Type)record[3] << 56) | ((Tls_Uint64Type)record[4] << 48) | ((Tls_Uint64Type)record[5] << 40) | ((Tls_Uint64Type)record[6] << 32) |
        ((Tls_Uint64Type)record[7] << 24) | ((Tls_Uint64Type)record[8] << 16) | ((Tls_Uint64Type)record[9] << 8) | ((Tls_Uint64Type)record[10] << 0);
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_RandomGenerate(Tls_PtrConnection ConnectionPtr, Tls_PtrUint8 output)
{
  Std_ReturnType ret;
  ConnectionPtr->handshakeData.randomSize = TLS_RANDOM_SIZE;

  ret = Csm_RandomGenerate(ConnectionPtr->config->randomJobId, output, &ConnectionPtr->handshakeData.randomSize);
  TLS_PRINTF(("Csm_RandomGenerate: %d.\n", ret));

  return ret;
}

STATIC FUNC(void, TLS_CODE) Tls_Dtls_Handshake_StartRetransmitTimer(Tls_PtrConnection ConnectionPtr)
{
  TLS_PRINTF(("Tls_Dtls_Handshake_StartRetransmitTimer\n"));
  ConnectionPtr->handshakeData.dtls_retransmitTimer = TLS_DTLS_HANDSHAKE_INITIAL_TIMEOUT;
  ConnectionPtr->handshakeData.dtls_retransmitBufferCount = 0u;
  ConnectionPtr->handshakeData.dtls_retransmitCount = 0u;
}

STATIC FUNC(void, TLS_CODE) Tls_Dtls_Handshake_StopRetransmitTimer(Tls_PtrConnection ConnectionPtr)
{
  TLS_PRINTF(("Tls_Dtls_Handshake_StopRetransmitTimer\n"));
  ConnectionPtr->handshakeData.dtls_retransmitBufferCount = 0u;
  ConnectionPtr->handshakeData.dtls_retransmitTimer = 0u;
}

/* !LINKSTO Dtls.RFC.Handshake.Retransmission,1 */
STATIC FUNC(void, TLS_CODE) Tls_Dtls_Handshake_RetransmitTimerExpired(Tls_PtrConnection ConnectionPtr)
{
  uint16 tmp;
  Tls_PtrHandshakeData data = &ConnectionPtr->handshakeData;
  Std_ReturnType ret;

  TLS_PRINTF(("Tls_Dtls_Handshake_RetransmitTimerExpired\n"));

  /* Retransmit all data from the retransmit buffer */
  ret = Tls_ProvideTxBuffer(ConnectionPtr);
  if(ret == E_OK)
  {
    TS_MemCpy(ConnectionPtr->encryptBuffer, data->dtls_retransmitBuffer, data->dtls_retransmitBufferCount);
    tmp = data->dtls_retransmitBufferCount;
    ret = Tls_TcpIp_Transmit(ConnectionPtr, data->dtls_retransmitBufferCount);
    /* Remove data from the retransmit buffer because this was a retransmit. */
    data->dtls_retransmitBufferCount = tmp;
  }

  if(ret != E_OK)
  {
    /* Something went wrong, but this is not a fatal error */
    TLS_PRINTF(("Retransmission failed.\n"));
  }

  /* !LINKSTO Dtls.RFC.Handshake.RetransmitLimit,1 */
  if(data->dtls_retransmitCount < TLS_DTLS_RETRANSMIT_LIMIT)
  {
    data->dtls_retransmitCount++;
    /* Restart the timer */
    /* !LINKSTO Dtls.RFC.Handshake.RetransmitTimeout,1 */
    data->dtls_retransmitTimer = (uint32)( (uint32)(TLS_DTLS_HANDSHAKE_INITIAL_TIMEOUT) << (data->dtls_retransmitCount) );
  }
  else
  {
    /* Maximum number of retransmits reached */
    TLS_PRINTF(("Maximum number of retransmits reached.\n"));
    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
  }
}

STATIC FUNC(void, TLS_CODE) Tls_Handshake_Start(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret = E_OK;
  uint32 pos;

  /* Check if clientRandom was generated */
  if(ConnectionPtr->handshakeData.randomState != TLS_RANDOM_STATE_READY)
  {
    TLS_PRINTF(("Random not ready.\n"));
  }
  else
  {
    /* Start the retransmit timer for the current DTLS flight */
    Tls_Dtls_Handshake_StartRetransmitTimer(ConnectionPtr);

    /* Write handshake message */
    pos = Tls_Handshake_WriteClientHello(ConnectionPtr, ConnectionPtr->handshakeData.clientRandom);

    if((ConnectionPtr->config->dtls == FALSE) || (ConnectionPtr->handshakeData.dtls_helloCount > 0u))
    {
      /* Append message to handshake buffer */
      Tls_Handshake_Append(ConnectionPtr,
          &ConnectionPtr->transmitBuffer.buffer[(uint32)ConnectionPtr->transmitBuffer.count - pos], pos);
    }

    ret = Tls_TransmitRecord(ConnectionPtr);
    if(ret == E_OK)
    {
      if(ConnectionPtr->config->dtls == TRUE)
      {
        /* LINKSTO Dtls.RFC.Handshake.MessageSeq.Increment,1 */
        ConnectionPtr->handshakeData.dtls_handshakeSeq++;
        ConnectionPtr->handshakeData.dtls_helloCount++;
      }

      if((ConnectionPtr->config->dtls == FALSE) || (ConnectionPtr->handshakeData.dtls_helloCount > 1u))
      {
        /* Mark random value as used */
        /* When using DTLS, the first two ClientHellos contain the same random. */
        ConnectionPtr->handshakeData.randomState = TLS_RANDOM_STATE_IDLE;
      }
      Tls_SetState(ConnectionPtr, TLS_STATE_CLIENTHELLO_DONE);
    }
    else
    {
      if(ret != TCPIP_E_PHYS_ADDR_MISS)
      {
        /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
        Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
      }
    }
  }
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Dtls_Handshake_HandleHelloVerifyRequest(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 handshake)
{
  Std_ReturnType ret = E_OK;
  uint8 HeaderLength = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  uint8 cookieLength;
  uint32 handshakeLength = Tls_Dtls_Handshake_GetFragmentLength(handshake);

  /* Check minimum size (without cookie) */
  if(handshakeLength < (/*version*/2u + /*cookie length*/1u))
  {
    ret = E_NOT_OK;
  }
  else
  {
    /* Read the cookie length from the message */
    cookieLength = handshake[HeaderLength + 2u];
  }

  if(ret == E_OK)
  {
#if TLS_DTLS_MAX_COOKIE_LENGTH < 0xFFu
    /* This code is not used as TLS_DTLS_MAX_COOKIE_LENGTH is 255 by default */
    /* CHECK: NOPARSE */
    /* Check if cookie fits in the cookie buffer */
    if(cookieLength > TLS_DTLS_MAX_COOKIE_LENGTH)
    {
      ret = E_NOT_OK;
    }
    else
    /* CHECK: PARSE */
#endif
    {
      /* Check if cookie fits exactly in the handshake message. */
      if(handshakeLength != ((uint32)cookieLength + /*version*/2u + /*cookie length*/1u))
      {
        ret = E_NOT_OK;
      }
    }
  }

  if(ret == E_OK)
  {
    /* Copy cookie to the cookie buffer */
    TS_MemCpy(ConnectionPtr->handshakeData.dtls_cookie, &handshake[HeaderLength + 3u], cookieLength);
    ConnectionPtr->handshakeData.dtls_cookieLength = cookieLength;

    Tls_Handshake_Start(ConnectionPtr);
  }
  else
  {
    TLS_PRINTF(("HelloVerifyRequest format check failed.\n"));
    /* !LINKSTO Tls.RFC.Alert.DecodeError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_DECODE_ERROR);
  }

  return ret;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_StartMacGenerate(Tls_PtrConnection ConnectionPtr)
{
  uint8 headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);
  uint16 length = Tls_GetRecordLength(ConnectionPtr, ConnectionPtr->transmitBuffer.buffer);
  uint8 type = Tls_GetRecordType(ConnectionPtr->transmitBuffer.buffer);
  Std_ReturnType ret = E_OK;

  /* Get a transmit buffer from TcpIp */
  ret = Tls_ProvideTxBuffer(ConnectionPtr);
  if(ret == E_OK)
  {
    /* Generate the header for the MAC generation */
    Tls_MacMakeHeader(ConnectionPtr->macGenerateHeader, length, type, Tls_GetVersion(ConnectionPtr), ConnectionPtr->writeSeqNum | ((Tls_Uint64Type)ConnectionPtr->dtls.writeEpoch << 48));
    ConnectionPtr->macGenerateLength = ConnectionPtr->cipherParams->MACLength;
    /* Start MAC generation */
    ret = Csm_MacGenerate(ConnectionPtr->config->MACGenerateJobId, CRYPTO_OPERATIONMODE_STREAMSTART,
        ConnectionPtr->macGenerateHeader, TLS_MAC_HEADER_SIZE,
        /*NULL_PTR*/&ConnectionPtr->encryptBuffer[headerSize + Tls_GetRecordLength(ConnectionPtr, ConnectionPtr->transmitBuffer.buffer)],
        /*NULL_PTR*/&ConnectionPtr->macGenerateLength);
  }
  if(ret == E_OK)
  {
    TLS_PRINTF(("Csm_MacGenerate success\n"));
  }
  else
  {
    TLS_PRINTF(("Csm_MacGenerate fail: %d\n", ret));
  }

  return ret;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Dtls_GenerateCookie(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret;

  ConnectionPtr->handshakeData.dtls_cookieLength = 0u;
  ret = Tls_RandomGenerate(ConnectionPtr, ConnectionPtr->handshakeData.dtls_cookie);
  if(ret == E_OK)
  {
    TLS_PRINTF(("cookieState = TLS_RANDOM_STATE_BUSY\n"));
    ConnectionPtr->handshakeData.dtls_cookieState = TLS_RANDOM_STATE_BUSY;
#if TLS_CFG_CSM_PROCESSING == TLS_CSM_SYNCHRONOUS
    /* !LINKSTO Tls.Async,1 */
    Tls_RandomCallback(ConnectionPtr, E_OK);
#endif
  }
  else
  {
    TLS_PRINTF(("cookieState = TLS_RANDOM_STATE_ERROR\n"));
    ConnectionPtr->handshakeData.dtls_cookieState = TLS_RANDOM_STATE_ERROR;
  }

  return ret;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Dtls_CheckCookie(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 CookiePtr, uint8 CookieLength)
{
  Std_ReturnType ret = E_OK;

  if ((ConnectionPtr->handshakeData.dtls_cookieLength != CookieLength) || (CookieLength == 0u))
  {
    ret = E_NOT_OK;
  }
  else if(TS_MemCmp32(ConnectionPtr->handshakeData.dtls_cookie, CookiePtr, ConnectionPtr->handshakeData.dtls_cookieLength) != 0u)
  {
    ret = E_NOT_OK;
  }
  else
  {
    /* Cookie ok */
  }

  return ret;
}

/* !LINKSTO Dtls.RFC.Handshake.HelloVerifyRequest.Format,1 */
/* !LINKSTO Dtls.RFC.Handshake.HelloVerifyRequest.Parameters,1 */
/* !LINKSTO Dtls.RFC.Handshake.HelloVerifyRequest.Version,1 */
STATIC FUNC(void, TLS_CODE) Tls_Dtls_Handshake_WriteHelloVerifyRequest(Tls_PtrConnection ConnectionPtr)
{
  Tls_PtrUint8 data;
  uint8 handshakeHeaderSize = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  uint16 handshakeLength = (uint32)2u + 1u + ConnectionPtr->handshakeData.dtls_cookieLength;
  uint8 headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);

  /* Write record header */
  Tls_WriteRecordHeaderToTransmitBuffer(ConnectionPtr, ConnectionPtr->transmitBuffer.count,
      handshakeLength + handshakeHeaderSize, TLS_RECORD_HANDSHAKE);
  ConnectionPtr->transmitBuffer.count += headerSize;

  /* Write handshake header */
  Tls_Handshake_WriteHeader(ConnectionPtr, &ConnectionPtr->transmitBuffer.buffer[ConnectionPtr->transmitBuffer.count],
      TLS_HANDSHAKE_HELLOVERIFYREQUEST, handshakeLength);
  ConnectionPtr->transmitBuffer.count += handshakeHeaderSize;

  /* Start of payload */
  data = &ConnectionPtr->transmitBuffer.buffer[ConnectionPtr->transmitBuffer.count];

  /* Version */
  data[0] = (uint8)((uint8)(TLS_DTLS_VERSION >> 8) & 0xFFu);
  data[1] = (uint8)((uint8)(TLS_DTLS_VERSION >> 0) & 0xFFu);

  /* Cookie length */
  data[2] = ConnectionPtr->handshakeData.dtls_cookieLength;

  /* Cookie */
  TS_MemCpy(&data[3u], ConnectionPtr->handshakeData.dtls_cookie, ConnectionPtr->handshakeData.dtls_cookieLength);

  /* Update buffer count */
  ConnectionPtr->transmitBuffer.count += (uint16)ConnectionPtr->handshakeData.dtls_cookieLength + 3u;
}

/* !LINKSTO Tls.RFC.Handshake.ServerHello,1 */
STATIC FUNC(uint32, TLS_CODE) Tls_Handshake_WriteServerHelloDone(Tls_PtrConnection ConnectionPtr)
{
  uint8 handshakeHeaderSize = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  uint8 headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);

  TLS_ENTER_EXCLUSIVE_AREA_TX();

  /* Write record header. There is no payload. */
  Tls_WriteRecordHeaderToTransmitBuffer(ConnectionPtr, ConnectionPtr->transmitBuffer.count,
      handshakeHeaderSize, TLS_RECORD_HANDSHAKE);
  ConnectionPtr->transmitBuffer.count += headerSize;

  /* Write handshake header */
  Tls_Handshake_WriteHeader(ConnectionPtr, &ConnectionPtr->transmitBuffer.buffer[ConnectionPtr->transmitBuffer.count],
      TLS_HANDSHAKE_SERVERHELLODONE, 0u);
  ConnectionPtr->transmitBuffer.count += handshakeHeaderSize;

  TLS_EXIT_EXCLUSIVE_AREA_TX();

  /* Return length of data required for verify data. Only header, no payload. */
  return handshakeHeaderSize;
}

STATIC FUNC(uint32, TLS_CODE) Tls_Handshake_WriteServerHello(Tls_PtrConnection ConnectionPtr)
{
  uint32 ret;
  uint32 handshakeLength = /*version*/2u + TLS_RANDOM_SIZE + /* session length*/1u + /*cipher*/2u + /*compression*/1u + TLS_EXTENSIONS_HEADER_SIZE;
  uint8 handshakeHeaderSize = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  Tls_PtrUint8 data;
  Tls_PtrConstConnectionConfig configPtr = ConnectionPtr->config;
  uint16 dataPos=0u;
  uint16 version = Tls_GetVersion(ConnectionPtr);
  uint8 headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);

  TLS_ENTER_EXCLUSIVE_AREA_TX();

  /* Reserve space for Extensions */
  if( configPtr->RecordSizeLimitExtensionEnabled == TRUE )
  {
    handshakeLength += TLS_EXTENSION_TYPE_SIZE +
                       TLS_EXTENSION_LENGTH_SIZE + TLS_EXTENSION_RECORDSIZELIMIT_SIZE;
  }

  /* Write record header */
  Tls_WriteRecordHeaderToTransmitBuffer(ConnectionPtr, ConnectionPtr->transmitBuffer.count,
      handshakeLength + handshakeHeaderSize, TLS_RECORD_HANDSHAKE);
  ConnectionPtr->transmitBuffer.count += headerSize;

  /* Write handshake header */
  Tls_Handshake_WriteHeader(ConnectionPtr, &ConnectionPtr->transmitBuffer.buffer[ConnectionPtr->transmitBuffer.count],
      TLS_HANDSHAKE_SERVERHELLO, handshakeLength);
  ConnectionPtr->transmitBuffer.count += handshakeHeaderSize;

  /* Start of payload */
  data = &ConnectionPtr->transmitBuffer.buffer[ConnectionPtr->transmitBuffer.count];

  /* Version */
  data[dataPos] = (uint8)((uint8)(version >> 8) & 0xFFu);
  data[dataPos+1u] = (uint8)((uint8)(version >> 0) & 0xFFu);
  dataPos += TLS_VERSION_SIZE;

  /* Random */
  TS_MemCpy(&data[dataPos], ConnectionPtr->handshakeData.serverRandom, TLS_RANDOM_SIZE);
  dataPos += TLS_RANDOM_SIZE;

  /* Session id length */
  /* !LINKSTO Tls.SessionResumption,1 */
  data[dataPos++] = 0u;
  /* Session id */
  /* Nothing */

  /* Cipher */
  data[dataPos] = (uint8)((uint8)(ConnectionPtr->config->cipher >> 8) & 0xFFu);
  data[dataPos+1U] = (uint8)((uint8)(ConnectionPtr->config->cipher >> 0) & 0xFFu);
  dataPos += TLS_CIPHERSUITE_LENGTH_SIZE;

  /* Compression */
  data[dataPos] = 0u;
  dataPos+= TLS_COMPRESSIONMETHOD_LENGTH_SIZE;

  /* Extensions*/
  {
    /* Reserve space for extensions total size */
    uint16 Tls_ExtensionsSize = 0U;
    uint16 BufferPos = dataPos + TLS_EXTENSIONS_HEADER_SIZE;

    if( configPtr->RecordSizeLimitExtensionEnabled == TRUE )
    {
      Tls_ExtensionsSize += Tls_Handshake_WriteExtension_RecordSizeLimit(&data[BufferPos+Tls_ExtensionsSize], ConnectionPtr->config->maxRecordSize);
    }

    /* Set total length of all extensions */
    TLS_SET16(&(data[dataPos]), 0U, Tls_ExtensionsSize);
  }

  /* Update buffer count */
  ConnectionPtr->transmitBuffer.count += handshakeLength;

  TLS_EXIT_EXCLUSIVE_AREA_TX();

  /* Return length of data required for verify data */
  ret = handshakeLength + handshakeHeaderSize;

  return ret;
}

/* !LINKSTO Tls.RFC.Handshake.ServerKeyExchange,1 */
STATIC FUNC(uint32, TLS_CODE) Tls_Handshake_WritePskServerKeyExchange(Tls_PtrConnection ConnectionPtr)
{
  uint32 ret;
  uint32 handshakeLength = (uint32)2u + ConnectionPtr->config->identityLength;
  uint8 handshakeHeaderSize = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  Tls_PtrUint8 data;
  uint8 headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);

  /* Write record header */
  Tls_WriteRecordHeaderToTransmitBuffer(ConnectionPtr, ConnectionPtr->transmitBuffer.count,
      handshakeLength + handshakeHeaderSize, TLS_RECORD_HANDSHAKE);
  ConnectionPtr->transmitBuffer.count += headerSize;

  /* Write handshake header */
  Tls_Handshake_WriteHeader(ConnectionPtr, &ConnectionPtr->transmitBuffer.buffer[ConnectionPtr->transmitBuffer.count],
      TLS_HANDSHAKE_SERVERKEYEXCHANGE, handshakeLength);
  ConnectionPtr->transmitBuffer.count += handshakeHeaderSize;

  /* Start of payload */
  data = &ConnectionPtr->transmitBuffer.buffer[ConnectionPtr->transmitBuffer.count];

  /* hint length */
  data[0] = (uint8)((uint8)(ConnectionPtr->config->identityLength >> 8) & 0xFFu);
  data[1] = (uint8)(ConnectionPtr->config->identityLength & 0xFFu);

  /* hint */
  TS_MemCpy(&data[2], ConnectionPtr->config->identity, ConnectionPtr->config->identityLength);

  /* Update buffer count */
  ConnectionPtr->transmitBuffer.count += handshakeLength;

  /* Return length of data required for verify data */
  ret = handshakeLength + handshakeHeaderSize;

  return ret;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_ReplyClientHello(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret = E_OK;
  uint32 size;

  Tls_SetState(ConnectionPtr, TLS_STATE_CLIENTHELLO_DONE);

  Tls_Dtls_Handshake_StartRetransmitTimer(ConnectionPtr);

  /* Transmit ServerHello */
  size = Tls_Handshake_WriteServerHello(ConnectionPtr);
  /* Append handshake for verify_data */
  Tls_Handshake_Append(ConnectionPtr,
      &ConnectionPtr->transmitBuffer.buffer[(uint32)ConnectionPtr->transmitBuffer.count - size], size);
  /* Transmit record */
  ret = Tls_TransmitRecord(ConnectionPtr);
  if(ret == E_OK)
  {
    /* !LINKSTO Dtls.RFC.Handshake.MessageSeq.Increment,1 */
    ConnectionPtr->handshakeData.dtls_handshakeSeq++;
    Tls_SetState(ConnectionPtr, TLS_STATE_SERVERHELLO_DONE);
  }

  /* If configured transmit Certificate from server */
#if TLS_CERTIFICATE_SUPPORT == STD_ON
  if((ret == E_OK) && (ConnectionPtr->config->localCertChainIdPtr != NULL_PTR))
  {
    Tls_ConstUint8PtrType recordPtr;
    size = Tls_Handshake_WriteCertificate(ConnectionPtr);

    /* Append handshake for verify_data */
    recordPtr = &ConnectionPtr->transmitBuffer.buffer[(uint32)ConnectionPtr->transmitBuffer.count - size];
    Tls_Handshake_Append(ConnectionPtr, recordPtr, size);

    /* Transmit record */
    ret = Tls_TransmitRecord(ConnectionPtr);
    if(ret == E_OK)
    {
      /* LINKSTO Dtls.RFC.Handshake.MessageSeq.Increment,1 */
      ConnectionPtr->handshakeData.dtls_handshakeSeq++;
      Tls_SetState(ConnectionPtr, TLS_STATE_SERVERCERTIFICATE_DONE);
    }
  }
#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */

  /* Transmit ServerKeyExchange */
  if(ret == E_OK)
  {
#if TLS_CERTIFICATE_SUPPORT == STD_ON
    /* If ECDHE is used for the key exchange */
    if(TLS_CIPHER_USES_CERTS(ConnectionPtr->config->cipher))
    {
      size = Tls_Handshake_WriteEcdhServerKeyExchange(ConnectionPtr);

      if(size != 0U)
      {
        ConnectionPtr->handshakeData.sigGenState = TLS_SIG_GEN_STATE_CLIENT_RANDOM_BUSY;

#if TLS_CFG_CSM_PROCESSING == TLS_CSM_SYNCHRONOUS
        /* !LINKSTO Tls.Async,1 */
        Tls_SignatureGenerateCallback(ConnectionPtr, E_OK);
#endif

        ConnectionPtr->handshakeData.keCalcPubValState = TLS_KE_CALC_PUB_VAL_STATE_BUSY;

#if TLS_CFG_CSM_JOB_BASED_KEYEXCHANGE == STD_OFF
        Tls_JobKeyExchangeCalcPubValCallback(ConnectionPtr, E_OK);
#endif

      }
      else
      {
        ConnectionPtr->handshakeData.sigGenState = TLS_SIG_GEN_STATE_ERROR;
        ConnectionPtr->handshakeData.keCalcPubValState = TLS_KE_CALC_PUB_VAL_STATE_ERROR;
      }
    }
    /* Otherwise PSK is used */
    else
#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */
    {
      if(ConnectionPtr->config->identityLength > 0u)
      {
        size = Tls_Handshake_WritePskServerKeyExchange(ConnectionPtr);

        /* Append handshake for verify_data */
        Tls_Handshake_Append(ConnectionPtr,
            &ConnectionPtr->transmitBuffer.buffer[(uint32)ConnectionPtr->transmitBuffer.count - size], size);

        /* Transmit record */
        ret = Tls_TransmitRecord(ConnectionPtr);

        if(ret == E_OK)
        {
          /* LINKSTO Dtls.RFC.Handshake.MessageSeq.Increment,1 */
          ConnectionPtr->handshakeData.dtls_handshakeSeq++;
        }
      }
      if(ret == E_OK)
      {
        Tls_SetState(ConnectionPtr, TLS_STATE_SERVERKEYEXCHANGE_DONE);

#if TLS_CERTIFICATE_SUPPORT == STD_ON
        ret = Tls_TransmitCertificateRequest(ConnectionPtr);

        if(ret == E_OK)
#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */
        {
          ret = Tls_TransmitServerHelloDone(ConnectionPtr);
        }
      }

      /* Mark server random as used */
      ConnectionPtr->handshakeData.randomState = TLS_RANDOM_STATE_IDLE;
    }
  }

  return ret;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_ReadVar8Array(Tls_PtrConstUint8 DataPtr, uint32 Length,
    P2VAR(uint32, AUTOMATIC, TLS_APPL_DATA) Idx,
    Tls_PtrUint8 ArrayLengthPtr, P2VAR(Tls_PtrConstUint8, AUTOMATIC, TLS_APPL_DATA) ArrayDataPtr)
{
  Std_ReturnType ret = E_OK;
  uint8 lengthField;

  /* Check if length is in range. This always true if used correctly. */
  /* CHECK: NOPARSE */
  if(*Idx < Length)
  /* CHECK: PARSE */
  {
    lengthField = DataPtr[*Idx];
    /* Check if the complete array is in range */
    if((*Idx + 1u + lengthField) < Length)
    {
      if(ArrayLengthPtr != NULL_PTR)
      {
        *ArrayLengthPtr = lengthField;
      }
      if(ArrayDataPtr != NULL_PTR)
      {
        *ArrayDataPtr = &DataPtr[*Idx + 1u];
      }
      /* Set Idx to data behind the array */
      *Idx = 1u + *Idx + lengthField;
    }
    else
    {
      ret = E_NOT_OK;
    }
  }
  /* CHECK: NOPARSE */
  else
  {
    ret = E_NOT_OK;
  }
  /* CHECK: PARSE */

  return ret;
}

/* !LINKSTO Tls.RFC.Handshake.ClientHello,1 */
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleClientHello(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 DataPtr, uint16 Length)
{
  Std_ReturnType ret = E_OK;
  Tls_PtrConstUint8 cookiePtr = NULL_PTR;
  Tls_PtrConstUint8 payloadPtr;
  uint32 handshakeLength;
  uint32 minLength;
  uint32 idx;
  uint8 cookieLength = 0u;
  uint8 headerSize = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  boolean skipReply = FALSE;

  TS_PARAM_UNUSED(Length);

  /* Get the length of payload from the header. This is the same for DTLS and TLS. */
  handshakeLength = Tls_Handshake_GetLength(DataPtr);

  /* Get minimum length for the message */
  if(ConnectionPtr->config->dtls == TRUE)
  {
    minLength = TLS_DTLS_CLIENTHELLO_MIN_LENGTH;
  }
  else
  {
    minLength = TLS_CLIENTHELLO_MIN_LENGTH;
  }

  /* Check minimum length*/
  if(handshakeLength < minLength)
  {
    ret = E_NOT_OK;
  }
  else
  {
    payloadPtr = &DataPtr[headerSize];
    idx = TLS_VERSION_SIZE + TLS_RANDOM_SIZE;
    /* Skip session id */
    ret = Tls_Handshake_ReadVar8Array(payloadPtr, handshakeLength, &idx, NULL_PTR, NULL_PTR);
    if(ret != E_OK)
    {
      /* Session id does not fit */
      TLS_PRINTF(("SessionId format error.\n"));
    }
    else
    {
      /* Session id is ok */
      if(ConnectionPtr->config->dtls == TRUE)
      {
        /* Check cookie */
        ret = Tls_Handshake_ReadVar8Array(payloadPtr, handshakeLength, &idx, &cookieLength, &cookiePtr);
        if(ret != E_OK)
        {
          /* Cookie does not fit */
          TLS_PRINTF(("Cookie format error.\n"));
        }
      }
    }
  }

  if(ret == E_OK)
  {
    uint16 cipherLength;

    /* Check cipher suites */
    cipherLength = (uint16)(((uint16)payloadPtr[idx] << 8) + (uint16)payloadPtr[idx + 1u]);
    if((idx + cipherLength + TLS_CIPHERSUITE_LENGTH_SIZE) > handshakeLength)
    {
      /* Cipher suites does not fit */
      TLS_PRINTF(("Cipher suites format error: cipherLength=%d, idx=%d, handshakeLength=%d\n", cipherLength, idx, handshakeLength));
      ret = E_NOT_OK;
    }
    else
    {
      /* Cipher suites is ok, skip it. */
      idx += (uint32)(TLS_CIPHERSUITE_LENGTH_SIZE + (uint32)(cipherLength) );
    }
  }

  if(ret == E_OK)
  {
    uint16 CompressionMethodLength;

    /* Compression method */
    CompressionMethodLength = (uint16)payloadPtr[idx];

    if((idx + CompressionMethodLength + TLS_COMPRESSIONMETHOD_LENGTH_SIZE) > handshakeLength)
    {
      /* Compression method does not fit */
      TLS_PRINTF(("Compression method format error: compressionLength=%d, idx=%d, handshakeLength=%d\n", CompressionMethodLength, idx, handshakeLength));
      ret = E_NOT_OK;
    }
    else
    {
      /* Set index to the first compression method */
      idx += TLS_COMPRESSIONMETHOD_LENGTH_SIZE;

      /* Consider only the first compression method */
      if( payloadPtr[idx] != TLS_COMPRESSION_METHOD_NULL )
      {
        /* Compression method is not supported */
        ret = E_NOT_OK;
      }
      else
      {
        /* Compression method is ok */
        idx += (uint32)CompressionMethodLength;
      }
    }
  }

  if(ret == E_OK)
  {
    uint16 extensionsTotalLength;

    /* Check Extensions */
    /* !LINKSTO Tls.RFC.HelloExtensions,1 */
    if( (idx + TLS_EXTENSION_LENGTH_SIZE ) < handshakeLength )
    {
      /* Extensions length */
      extensionsTotalLength = (uint16)(((uint16)payloadPtr[idx] << 8u) + (uint16)payloadPtr[idx + 1u]);
      idx += TLS_EXTENSION_LENGTH_SIZE;

      if((idx + extensionsTotalLength) > handshakeLength)
      {
        /* Extensions do not fit */
        TLS_PRINTF(("Extensions format error: extensionLength=%d, idx=%d, handshakeLength=%d\n", extensionsTotalLength, idx, handshakeLength));
        ret = E_NOT_OK;
      }
      else
      {
        /* Handle extensions */
        /* Ignore extensions that are not supported */
        ret = Tls_Handshake_HandleExtensions(ConnectionPtr, &payloadPtr[idx], extensionsTotalLength);
      }
    }
  }

  if(ret == E_OK)
  {
    /* Check if serverRandom was generated. Should not happen. */
    /* CHECK: NOPARSE */
    if(ConnectionPtr->handshakeData.randomState != TLS_RANDOM_STATE_READY)
    {
      TLS_PRINTF(("Random not ready.\n"));
      TLS_UNREACHABLE_CODE_ASSERT(TLS_INTERNAL_SVCID);
    }
    /* CHECK: PARSE */
  }
  else
  {
    /* Message could not be properly decoded */
    /* !LINKSTO Tls.RFC.Alert.DecodeError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_DECODE_ERROR);
  }

  /* Check if client_version is supported */
  if(ret == E_OK)
  {
    if(Tls_Handshake_Hello_GetVersion(payloadPtr) < TLS_VERSION)
    {
      /* The client version is too low */
      TLS_PRINTF(("Client version too low.\n"));
      /* !LINKSTO Tls.RFC.Alert.ProtocolVersion,1 */
      Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_PROTOCOL_VERSION);
      ret = E_NOT_OK;
    }
  }

  if(ret == E_OK)
  {
    if (ConnectionPtr->config->dtls)
    {
      /* !LINKSTO Dtls.RFC.Handshake.InvalidCookie,1 */
      if (Tls_Dtls_CheckCookie(ConnectionPtr, cookiePtr, cookieLength) != E_OK)
      {
        /* No cookie given or cookie is invalid */
        /* This should always be TRUE */
        /* CHECK: NOPARSE */
        if(ConnectionPtr->handshakeData.dtls_cookieState == TLS_RANDOM_STATE_IDLE)
        /* CHECK: PARSE */
        {
          /* Start cookie generation */
          ConnectionPtr->handshakeData.dtls_cookieState = TLS_RANDOM_STATE_START;
          /* Do not reply to this ClientHello. Will reply with HelloVerifyRequest when the cookie is ready. */
          skipReply = TRUE;
          /* !LINKSTO Dtls.RFC.Handshake.HelloVerifyRequest.SequenceNumber,1 */
          /* Set the sequence number of the HelloVerifyRequest to sequence number of the ClientHello */
          ConnectionPtr->writeSeqNum = Tls_Dtls_GetSeqNum(ConnectionPtr->decryptBuffer.buffer);
          TLS_PRINTF(("Setting writeSeqNum to %d for HelloVerifyRequest\n", ConnectionPtr->writeSeqNum));

          /* Reset receive window.
           * All ClientHellos with wrong or empty cookie shall not affect the state of the receive window,
           * but it is updated before handshake message is parsed.
           */
          ConnectionPtr->readSeqNum = 0u;
          ConnectionPtr->dtls.receiveWindow = 0u;
          ConnectionPtr->handshakeData.dtls_nextReceiveSeq = 0u;
        }
        /* CHECK: NOPARSE */
        else
        {
          /* Should not happen */
          TLS_PRINTF(("Received ClientHello but cookieState is not IDLE.\n"));
          TLS_UNREACHABLE_CODE_ASSERT(TLS_INTERNAL_SVCID);
        }
        /* CHECK: PARSE */
      }
      else
      {
        /* Cookie is ok. */
        /* Set message sequence number to 1 for ServerHello */
        ConnectionPtr->handshakeData.dtls_handshakeSeq = 1u;
      }
    }

    if(skipReply == FALSE)
    {
      /* Save client random */
      TS_MemCpy(ConnectionPtr->handshakeData.clientRandom, Tls_Handshake_Hello_GetRandom(payloadPtr), TLS_RANDOM_SIZE);
      /* Append ClientHello for verify data */
      Tls_Handshake_Append(ConnectionPtr, DataPtr, Length);

      /* Reply to the ClientHello */
      ret = Tls_Handshake_ReplyClientHello(ConnectionPtr);
    }
  }

  return ret;
}

STATIC FUNC(void, TLS_CODE) Tls_Connected(Tls_PtrConnection ConnectionPtr)
{
  /* !LINKSTO Tls.API.Callout.TcpConnected,1 */
  TcpIp_TcpConnectedAPI[Tls_GetSocketOwner(ConnectionPtr)](ConnectionPtr->socket);
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleFinished(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 DataPtr, uint16 Length)
{
  Std_ReturnType ret = E_OK;
  uint8 headerSize = Tls_Handshake_GetHeaderSize(ConnectionPtr);

  /* Check length of the message */
  if(Length != ((uint16)TLS_VERIFY_DATA_LENGTH + headerSize))
  {
    ret = E_NOT_OK;
  }
  else
  {
    /* Save verify data. Local generation of verify data might not be ready yet as it might run asynchronously. */
    TS_MemCpy(ConnectionPtr->handshakeData.receivedVerifyData, &DataPtr[headerSize], TLS_VERIFY_DATA_LENGTH);
  }

  if(ret != E_OK)
  {
    /* !LINKSTO Tls.RFC.Alert.DecodeError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_DECODE_ERROR);
  }

  return ret;
}

#if TLS_CERTIFICATE_SUPPORT == STD_ON
STATIC FUNC(boolean, TLS_CODE) canReceiveClientKeyExchange(Tls_PtrConnection ConnectionPtr)
{
  /* If client Certificate processed if configured, or ServerHelloDone processed */
  return
      ((ConnectionPtr->config->remoteCertChainIdPtr == NULL_PTR) &&
       (ConnectionPtr->state == TLS_STATE_SERVERHELLODONE_DONE)) ||
      (ConnectionPtr->state == TLS_STATE_CLIENTCERTIFICATE_DONE);
}
#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleServer(Tls_PtrConnection ConnectionPtr, Tls_PtrUint8 DataPtr, uint16 Length)
{
  uint8 type = Tls_Handshake_GetType(DataPtr);
  Std_ReturnType ret = E_OK;
  boolean unexpected = FALSE;

  switch (type)
  {
  case TLS_HANDSHAKE_CLIENTHELLO:
    if(ConnectionPtr->state == TLS_STATE_OK)
    {
      /* Connection is already established. Send no_renegotiation alert. */
      /* !LINKSTO Tls.RFC.Alert.NoRenegotiation,1 */
      Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_WARNING, TLS_ALERT_DESC_NO_RENEGOTIATION);
    }
    else if(ConnectionPtr->state == TLS_STATE_DEFAULT)
    {
      Tls_Dtls_Handshake_StopRetransmitTimer(ConnectionPtr);
      ret = Tls_Handshake_HandleClientHello(ConnectionPtr, DataPtr, Length);
    }
    else
    {
      /* Wrong state. Did not expect ClientHello. */
      TLS_PRINTF(("Received ClientHello in wrong state.\n"));
      ret = E_NOT_OK;
      unexpected = TRUE;
    }
    break;
#if TLS_CERTIFICATE_SUPPORT == STD_ON
  case TLS_HANDSHAKE_CERTIFICATE:
  {
    if
    (
      TLS_CIPHER_USES_CERTS(ConnectionPtr->cipherParams->cipher) &&
      (ConnectionPtr->state == TLS_STATE_SERVERHELLODONE_DONE) &&
      (ConnectionPtr->config->remoteCertChainIdPtr != NULL_PTR)
    )
    {
      ret = Tls_Handshake_HandleCertificate(ConnectionPtr, DataPtr, Length);
    }
    else
    {
      /* Unexpected message */
      ret = E_NOT_OK;
      unexpected = TRUE;
    }
    break;
  }
#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */
  case TLS_HANDSHAKE_CLIENTKEYEXCHANGE:
  {
#if TLS_CERTIFICATE_SUPPORT == STD_ON
    /* If Certificate cipher suite */
    if(TLS_CIPHER_USES_CERTS(ConnectionPtr->cipherParams->cipher))
    {
      if(canReceiveClientKeyExchange(ConnectionPtr))
      {
        ret = Tls_Handshake_HandleEcdhClientKeyExchange(ConnectionPtr, DataPtr, Length);

        if(ret == E_OK)
        {
          ConnectionPtr->handshakeData.keCalcSecretState = TLS_KE_CALC_SECRET_STATE_BUSY;
          TLS_PRINTF(("keCalcSecretState = TLS_KE_CALC_SECRET_STATE_BUSY\n"));

#if TLS_CFG_CSM_JOB_BASED_KEYEXCHANGE == STD_OFF
          Tls_JobKeyExchangeCalcSecretCallback(ConnectionPtr, E_OK);
#endif
        }
      }
      else
      {
        /* Wrong state. Did not expect ClientKeyExchange. */
        ret = E_NOT_OK;
        unexpected = TRUE;
      }
    }
    /* If PSK cipher suite */
    else
#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */
    {
      if(ConnectionPtr->state == TLS_STATE_SERVERHELLODONE_DONE)
      {
        /* Ignore contents as the key is preconfigured */
        Tls_SetState(ConnectionPtr, TLS_STATE_CLIENTKEYEXCHANGE_DONE);
      }
      else
      {
        /* Wrong state. Did not expect ClientKeyExchange. */
        ret = E_NOT_OK;
        unexpected = TRUE;
      }
    }
    break;
  }
#if TLS_CERTIFICATE_SUPPORT == STD_ON
  case TLS_HANDSHAKE_CERTIFICATEVERIFY:
  {
    /* If Certificate cipher suite and correct state */
    if
    (
      TLS_CIPHER_USES_CERTS(ConnectionPtr->cipherParams->cipher) &&
      (ConnectionPtr->config->remoteCertChainIdPtr != NULL_PTR) &&
      (ConnectionPtr->state == TLS_STATE_CLIENTKEYEXCHANGE_DONE)
    )
    {
      ret = Tls_Handshake_HandleCertificateVerify(ConnectionPtr, DataPtr, Length);

      if(ret == E_OK)
      {
        ConnectionPtr->handshakeData.sigVerState = TLS_SIG_VER_STATE_BUSY;
        TLS_PRINTF(("sigVerState = TLS_SIG_VER_STATE_BUSY\n"));
#if TLS_CFG_CSM_PROCESSING == TLS_CSM_SYNCHRONOUS
        /* !LINKSTO Tls.Async,1 */
        Tls_SignatureVerifyCallback(ConnectionPtr, E_OK);
#endif
      }
    }
    else
    {
      /* CertificateVerify was not expected. */
      ret = E_NOT_OK;
      unexpected = TRUE;
    }
    break;
  }
#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */
  case TLS_HANDSHAKE_FINISHED:
    {
      if (ConnectionPtr->state == TLS_STATE_CHANGECIPHERSPEC_RECEIVED)
      {
        Tls_Dtls_Handshake_StopRetransmitTimer(ConnectionPtr);
        Tls_SetState(ConnectionPtr, TLS_STATE_FINISHED_RECEIVED);

        ret = Tls_Handshake_HandleFinished(ConnectionPtr, DataPtr, Length);
        if(ret == E_OK)
        {
          /* Append handshake data without verifying the data */
          Tls_Handshake_Append(ConnectionPtr, DataPtr, Length);
        }
      }
      else
      {
        /* Wrong state. Did not expect Finished. */
        ret = E_NOT_OK;
        unexpected = TRUE;
      }
    }
  break;
  default:
    /* Received wrong handshake message. */
    TLS_PRINTF(("Received handshake message with wrong type.\n"));
    ret = E_NOT_OK;
    unexpected = TRUE;
    break;
  }

  if(unexpected == TRUE)
  {
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_UNEXPECTED_MESSAGE);
  }

  return ret;
}

STATIC FUNC(void, TLS_CODE) Tls_StartKeyDerivation(Tls_PtrConnection ConnectionPtr)
{
  ConnectionPtr->handshakeData.keyDeriveState = TLS_KEYDERIVE_STATE_START;
}

/* !LINKSTO Tls.RFC.HelloExtensions.Format,1 */
TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleExtensions(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 DataPtr, uint16 Length)
{
  uint32 idx = 0u; /* type uint32 used to avoid endless while loop */
  uint16 extensionType;
  uint16 extensionLength;
  const uint8 * extensionPtr;
  Std_ReturnType ret = E_OK;

  /* Read each extension */
  while( idx < ((uint32)Length) )
  {
    extensionType   = TLS_GET_EXTENSION_TYPE(DataPtr, idx);
    extensionLength = TLS_GET_EXTENSION_LENGTH(DataPtr, idx);
    extensionPtr    = TLS_GET_EXTENSION_BUFF_PTR(DataPtr, idx);

    switch (extensionType)
    {
      case TLS_EXTENSION_RECORDSIZELIMIT_ID:
      {
        if( ConnectionPtr->config->RecordSizeLimitExtensionEnabled )
        {
          if( Tls_Handshake_HandleExtension_RecordSizeLimit(ConnectionPtr, extensionPtr, extensionLength) == E_NOT_OK )
          {
            ret = E_NOT_OK;
          }
        }
      }
      break;

      default:
      {
#if (TLS_EXTENSIONS_DETECTION_FCT_ENABLE == STD_ON)
        /* Notify use of unsupported extension */
        Tls_ExtensionsDetectionAPI
        (
          (TcpIp_ConstSockAddrPtrType)&ConnectionPtr->remoteAddr,
          extensionType
        );
#endif /* (TLS_EXTENSIONS_DETECTION_FCT_ENABLE == STD_ON) */

        if (ConnectionPtr->config->entity == TLS_CLIENT)
        {
          /* The remote server sends only extensions requested by the client.
           * Any unsupported extension received by the client leads to abort the handshake */
          ret = E_NOT_OK;
        }
        else
        {
          /* Ignore unsupported extensions */
        }
      }
      break;
    }

    /* Go to the next field */
    idx += TLS_EXTENSION_TYPE_SIZE + TLS_EXTENSION_LENGTH_SIZE + (uint32)extensionLength;
  }

  return ret;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleExtension_RecordSizeLimit(Tls_PtrConnection ConnectionPtr, const uint8 * extensionPtr, uint16 extensionLength)
{
  Std_ReturnType ret = E_NOT_OK;

  if( extensionLength == TLS_EXTENSION_RECORDSIZELIMIT_SIZE )
  {
    uint16 recordSizeLimit = TLS_GET16(extensionPtr);

    if( (recordSizeLimit < TLS_MIN_RECORD_SIZE) || (recordSizeLimit > TLS_MAX_RECORD_SIZE) )
    {
      /* Invalid size limit received. Abort handshake */
      /* !LINKSTO Tls.RFC.HelloExtensions.RecordSizeLimit.minumumValue,1 */
      /* !LINKSTO Tls.RFC.HelloExtensions.RecordSizeLimit.maximumValue,1 */
      ret = E_NOT_OK;
      Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_ILLEGAL_PARAMETER);
    }
    else if( recordSizeLimit < ConnectionPtr->config->maxRecordSize )
    {
      /* Limit the Tx buffer size to the size requested by the remote host */
      ConnectionPtr->transmitRecordSizeLimit = recordSizeLimit;
      ret = E_OK;
    }
    else
    {
      /* Set the buffer size to the maximum capacity */
      ConnectionPtr->transmitRecordSizeLimit = ConnectionPtr->config->maxRecordSize;
      ret = E_OK;
    }
  }
  else
  {
    ret = E_NOT_OK;
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
  }

  return ret;
}

/* !LINKSTO Tls.RFC.Handshake.ServerHello,1 */
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleServerHello(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 DataPtr, uint16 Length)
{
  Std_ReturnType ret = E_OK;
  Tls_PtrConstUint8 payloadPtr;
  uint32 handshakeLength;
  uint8 headerSize = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  uint32 idx;
  uint16 cipher;
  uint8 compression;

  TS_PARAM_UNUSED(Length);

  /* Get the length of payload from the header */
  handshakeLength = Tls_Handshake_GetLength(DataPtr);

  /* Check minimum length*/
  if(handshakeLength < TLS_SERVERHELLO_MIN_LENGTH)
  {
    ret = E_NOT_OK;
  }
  else
  {
    payloadPtr = &DataPtr[headerSize];
    idx = TLS_VERSION_SIZE + TLS_RANDOM_SIZE;

    /* Skip session id */
    ret = Tls_Handshake_ReadVar8Array(payloadPtr, handshakeLength, &idx, NULL_PTR, NULL_PTR);
    if(ret != E_OK)
    {
      /* Session id does not fit */
      TLS_PRINTF(("SessionId format error.\n"));
    }
    else
    {
      /* Check remaining length */
      if((idx + TLS_CIPHERSUITE_LENGTH_SIZE + TLS_COMPRESSIONMETHOD_LENGTH_SIZE) > handshakeLength)
      {
        ret = E_NOT_OK;
      }
    }
  }

  if(ret == E_OK)
  {
    /* Read the cipher suite selected by the server (only one possible) */
    cipher = ((uint16)payloadPtr[idx]<<8) | (uint16)payloadPtr[idx + 1u];
    idx += TLS_CIPHERSUITE_LENGTH_SIZE;

    /* Read the compression method selected by the server (only one possible) */
    compression = payloadPtr[idx];
    idx += TLS_COMPRESSIONMETHOD_LENGTH_SIZE;

    if(compression != TLS_COMPRESSION_METHOD_NULL)
    {
      TLS_PRINTF(("Compression not supported.\n"));
      ret = E_NOT_OK;
    }
    else
    {
      /* !LINKSTO Dtls.RFC.Handshake.ServerHello.VersionCheck,1 */
      /* Check if server_version is supported */
      if(Tls_Handshake_Hello_GetVersion(payloadPtr) != Tls_GetVersion(ConnectionPtr))
      {
        /* The server version does not match */
        TLS_PRINTF(("Server version is not supported.\n"));
        /* !LINKSTO Tls.RFC.Alert.ProtocolVersion,1 */
        Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_PROTOCOL_VERSION);
        ret = E_NOT_OK;
      }
    }
  }

  if(ret == E_OK)
  {
    /* Check extensions */
    if((idx + TLS_EXTENSION_LENGTH_SIZE) < handshakeLength)
    {
      uint16 extensionsTotalLength = (uint16)(((uint16)payloadPtr[idx] << 8u) + (uint16)payloadPtr[idx + 1u]);
      idx += TLS_EXTENSION_LENGTH_SIZE;

      if((idx + extensionsTotalLength) == handshakeLength)
      {
        /* Handle Tls extensions */
        ret = Tls_Handshake_HandleExtensions(ConnectionPtr, &payloadPtr[idx], extensionsTotalLength);

        if(ret != E_OK)
        {
          /* Extension is not supported */
          TLS_PRINTF(("Extensions are not supported.\n"));
          /* !LINKSTO Tls.RFC.Alert.UnsupportedExtension,1 */
          Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_UNSUPPORTED_EXTENSION);
        }
      }
      else
      {
        /* Extensions do not fit */
        TLS_PRINTF(("Extensions format error: extensionLength=%d, idx=%d, handshakeLength=%d\n", extensionsTotalLength, idx, handshakeLength));
        /* !LINKSTO Tls.RFC.Alert.DecodeError,1 */
        Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_DECODE_ERROR);

        ret = E_NOT_OK;
      }

    }
  }
  else
  {
    /* Message could not be properly decoded */
    /* !LINKSTO Tls.RFC.Alert.DecodeError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_DECODE_ERROR);
  }

  if(ret == E_OK)
  {
    TS_MemCpy(ConnectionPtr->handshakeData.serverRandom, Tls_Handshake_Hello_GetRandom(payloadPtr), TLS_RANDOM_SIZE);
    ConnectionPtr->cipher = cipher;
    if (ConnectionPtr->cipher != ConnectionPtr->config->cipher)
    {
      /* !LINKSTO Tls.RFC.Alert.HandshakeFailure,1 */
      TLS_PRINTF(("wrong cipher!"));
      Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_HANDSHAKE_FAILURE);
      ret = E_NOT_OK;
    }
    else
    {
      Tls_SetState(ConnectionPtr, TLS_STATE_SERVERHELLO_DONE);
    }
  }

  return ret;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleClient(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 DataPtr, uint16 Length)
{
  uint8 type = Tls_Handshake_GetType(DataPtr);
  uint32 size;
  Std_ReturnType ret = E_OK;
  boolean unexpected = FALSE;

  switch (type)
  {
  case TLS_HANDSHAKE_SERVERHELLO:
    {
      if (ConnectionPtr->state == TLS_STATE_CLIENTHELLO_DONE)
      {
        ret = Tls_Handshake_HandleServerHello(ConnectionPtr, DataPtr, Length);
      }
      else
      {
        ret = E_NOT_OK;
        unexpected = TRUE;
      }
    }
    break;
  case TLS_HANDSHAKE_SERVERKEYEXCHANGE:
    {
      if (ConnectionPtr->state == TLS_STATE_SERVERHELLO_DONE)
      {
          Tls_SetState(ConnectionPtr, TLS_STATE_SERVERKEYEXCHANGE_DONE);
      }
      else
      {
          ret = E_NOT_OK;
          unexpected = TRUE;
      }
    }
    break;
  case TLS_HANDSHAKE_SERVERHELLODONE:
    {
      /* !LINKSTO Tls.RFC.Handshake.ServerKeyExchange.PSKIdentityHint,1 */
      if( (ConnectionPtr->state == TLS_STATE_SERVERKEYEXCHANGE_DONE) ||
          (ConnectionPtr->state == TLS_STATE_SERVERHELLO_DONE) )
      {
        Tls_Dtls_Handshake_StartRetransmitTimer(ConnectionPtr);

        Tls_SetState(ConnectionPtr, TLS_STATE_SERVERHELLODONE_DONE);

        /* Send ClientKeyExchange */
        size = Tls_Handshake_WriteClientKeyExchangePsk(ConnectionPtr);
        /* Append handshake for verify_data */
        Tls_Handshake_Append(ConnectionPtr,
            &ConnectionPtr->transmitBuffer.buffer[(uint32)ConnectionPtr->transmitBuffer.count - size], size);
        /* Transmit record */
        ret = Tls_TransmitRecord(ConnectionPtr);
        if(ret == E_OK)
        {
          /* LINKSTO Dtls.RFC.Handshake.MessageSeq.Increment,1 */
          ConnectionPtr->handshakeData.dtls_handshakeSeq++;
          Tls_SetState(ConnectionPtr, TLS_STATE_CLIENTKEYEXCHANGE_DONE);
        }

        /* Send ChangeCipherSpec */
        if(ret == E_OK)
        {
          Tls_ChangeCipherSpec_Write(ConnectionPtr);
          ret = Tls_TransmitRecord(ConnectionPtr);
        }

        if(ret == E_OK)
        {
          Tls_OnSendChangeCipherSpec(ConnectionPtr);

          Tls_SetState(ConnectionPtr, TLS_STATE_CHANGECIPHERSPEC_SENT);

          /* Start generation of key material */
          Tls_StartKeyDerivation(ConnectionPtr);
        }
      }
      else
      {
        ret = E_NOT_OK;
        unexpected = TRUE;
      }
    }
    break;
  case TLS_HANDSHAKE_FINISHED:
    {
      if (ConnectionPtr->state == TLS_STATE_CHANGECIPHERSPEC_RECEIVED)
      {
        Tls_Dtls_Handshake_StopRetransmitTimer(ConnectionPtr);

        Tls_SetState(ConnectionPtr, TLS_STATE_FINISHED_RECEIVED);

        ret = Tls_Handshake_HandleFinished(ConnectionPtr, DataPtr, Length);
      }
      else
      {
        ret = E_NOT_OK;
        unexpected = TRUE;
      }
    }
    break;
  case TLS_HANDSHAKE_HELLOVERIFYREQUEST:
    /* !LINKSTO Dtls.RFC.Handshake.Flow,1 */
    if((ConnectionPtr->state == TLS_STATE_DEFAULT) || (ConnectionPtr->state == TLS_STATE_CLIENTHELLO_DONE))
    {
      Tls_Dtls_Handshake_StopRetransmitTimer(ConnectionPtr);
      ret = Tls_Dtls_Handshake_HandleHelloVerifyRequest(ConnectionPtr, DataPtr);
    }
    else
    {
      ret = E_NOT_OK;
      unexpected = TRUE;
    }
    break;
  case TLS_HANDSHAKE_HELLOREQUEST:
    if(ConnectionPtr->state == TLS_STATE_OK)
    {
      /* Got renegotiation request */
      /* !LINKSTO Tls.Renegotiation,1 */
      /* !LINKSTO Tls.RFC.Alert.NoRenegotiation,1 */
      Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_WARNING, TLS_ALERT_DESC_NO_RENEGOTIATION);
    }
    else
    {
      /* Connection not established. Ignore the message. */
      /* !LINKSTO Tls.RFC.HelloRequest.Ignore,1 */
    }
    break;
  default:
    /* Received wrong handshake message. */
    TLS_PRINTF(("Received handshake message with wrong type.\n"));
    ret = E_NOT_OK;
    unexpected = TRUE;
    break;
  }

  if(unexpected == TRUE)
  {
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_UNEXPECTED_MESSAGE);
  }

  return ret;
}

/* !LINKSTO Tls.RFC.Handshake.FullFlow,1 */
/* !LINKSTO Dtls.RFC.Handshake.Flow,1 */
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_Handle(Tls_PtrConnection ConnectionPtr, Tls_Uint8PtrType DataPtr, uint16 Length)
{
  uint8 type = Tls_Handshake_GetType(DataPtr);
  Std_ReturnType ret;

  TLS_PRINTF(("Handshake received: "));
  TLS_DEBUG_PRINTHANDSHAKETYPE(type);
  TLS_PRINTF(("\n"));

  /* !LINKSTO Dtls.RFC.Handshake.VerifyData,1 */
  if ((type != TLS_HANDSHAKE_CLIENTHELLO) && (type != TLS_HANDSHAKE_FINISHED) &&
      (type != TLS_HANDSHAKE_HELLOVERIFYREQUEST) && (type != TLS_HANDSHAKE_HELLOREQUEST))
  {
    /* Append received handshake message for verify data */
    Tls_Handshake_Append(ConnectionPtr, DataPtr, Length);
  }

  if (ConnectionPtr->config->entity == TLS_SERVER)
  {
    ret = Tls_Handshake_HandleServer(ConnectionPtr, DataPtr, Length);
  }
  else
  {
    ret = Tls_Handshake_HandleClient(ConnectionPtr, DataPtr, Length);
  }

  if(ret != E_OK)
  {
    TLS_PRINTF(("Tls_HandleHandshake fail.\n"));
  }


  return ret;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Dtls_Handshake_HandleFragment(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 HandshakePtr, uint16 Length)
{
  /* !LINKSTO Dtls.RFC.Handshake.Defragmentation,1 */

  Std_ReturnType ret = E_OK;
  uint8 handshakeHeaderSize = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  Tls_PtrHandshakeData data = &ConnectionPtr->handshakeData;
  Tls_PtrUint8 handshakeDataBuffer = &data->handshakeFragmentBuffer[handshakeHeaderSize];
  uint16 messageSeq = Tls_Dtls_Handshake_GetMessageSequence(HandshakePtr);
  uint32 fragmentOffset = Tls_Dtls_Handshake_GetFragmentOffset(HandshakePtr);
  uint32 fragmentLength = Tls_Dtls_Handshake_GetFragmentLength(HandshakePtr);
  uint32 messageLength = Tls_Handshake_GetLength(HandshakePtr);

  TLS_PRINTF(("DTLS handshake fragment received: fragment_offset = %d, fragment_length = %d, message_seq = %d, message_length = %d\n", fragmentOffset, fragmentLength, messageSeq, messageLength));
  if(messageSeq == data->dtls_nextReceiveSeq)
  {
    /* Check if length field of handshake header is valid */
    if((fragmentLength + handshakeHeaderSize) != Length)
    {
      TLS_PRINTF(("fragmentLength = %d is too big for record payload length = %d.\n", fragmentLength, Length));
      ret = E_NOT_OK;
    }

    if(ret == E_OK)
    {
      /* Check if fragment_length and fragment_offset fields in handshake header are valid */
      if((fragmentLength + fragmentOffset) > messageLength)
      {
        TLS_PRINTF(("fragment_length = %d and fragment_offset = %d invalid for message_length = %d.\n", fragmentLength, fragmentOffset, messageLength));
        ret = E_NOT_OK;
      }
    }

    if(ret != E_OK)
    {
      /* Something is wrong with the length fields */
      /* !LINKSTO Tls.RFC.Alert.DecodeError,1 */
      Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_DECODE_ERROR);
    }

    if(ret == E_OK)
    {
      /* Check if message fits in fragment buffer. The fragment buffer includes the handshake header. */
      if((messageLength + handshakeHeaderSize) > TLS_HANDSHAKE_FRAGMENT_BUFFER_SIZE)
      {
        TLS_PRINTF(("Handshake message does not fit in fragment buffer: message_length = %d\n", messageLength));
        ret = E_NOT_OK;

        /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
        Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
      }
    }

    if(ret == E_OK)
    {
      if(data->handshakeFragmentBufferLength == 0u)
      {
        /* Write header if this is the start of a handshake message */
        Tls_Handshake_WriteHeader(ConnectionPtr, data->handshakeFragmentBuffer, Tls_Handshake_GetType(HandshakePtr), messageLength);

        /* Set message sequence */
        data->handshakeFragmentBuffer[4] = (uint8)((uint8)(ConnectionPtr->handshakeData.dtls_nextReceiveSeq >> 8) & 0xFFu);
        data->handshakeFragmentBuffer[5] = (uint8)((uint8)(ConnectionPtr->handshakeData.dtls_nextReceiveSeq >> 0) & 0xFFu);

        /* Set handshake header size */
        data->handshakeFragmentBufferLength = handshakeHeaderSize;
      }

      /* Check if the received fragment can be joined to the fragment buffer. */
      if((fragmentOffset + handshakeHeaderSize) <= data->handshakeFragmentBufferLength)
      {
        /* Copy fragment into fragment buffer */
        TS_MemCpy(&handshakeDataBuffer[fragmentOffset], &HandshakePtr[handshakeHeaderSize], fragmentLength);

        data->handshakeFragmentBufferLength = (uint16)handshakeHeaderSize + (uint16)(fragmentOffset + fragmentLength);

        /* Check if the handshake message is complete */
        if(data->handshakeFragmentBufferLength == ((uint16)messageLength + (uint16)handshakeHeaderSize))
        {
          /* Message is complete */
          ret = Tls_Handshake_Handle(ConnectionPtr, data->handshakeFragmentBuffer, data->handshakeFragmentBufferLength);
          data->handshakeFragmentBufferLength = 0u;
          data->dtls_nextReceiveSeq++;
        }
      }
      else
      {
        /* Discard fragment. This may drop valid fragments that would cause a gap in the fragment buffer.
         * This implementation does not allow gaps to save memory.
         * The lost message will be retransmitted. */
        TLS_PRINTF(("DTLS handshake fragment dropped.\n"));
      }
    }
  }
  else
  {
    if((messageSeq + 1u) == data->dtls_nextReceiveSeq)
    {
      /* This is a retransmit of the previous flight. Retransmit last flight. */
      /* !LINKSTO Dtls.RFC.Handshake.Retransmit,1 */
      TLS_PRINTF(("Retransmitted or duplicate handshake message received: messageSeq=%d, nextReceiveSeq=%d\n", messageSeq, data->dtls_nextReceiveSeq));
      Tls_Dtls_Handshake_RetransmitTimerExpired(ConnectionPtr);
    }
    /* Discard message */
    /* !LINKSTO Dtls.RFC.Handshake.MessageSeq.Check,1 */
    TLS_PRINTF(("DTLS handshake message discarded because of wrong sequence number.\n"));
  }

  return ret;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Handshake_HandleFragment(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 DataPtr, uint16 Length)
{
  Std_ReturnType ret = E_OK;
  uint8 handshakeHeaderSize = Tls_Handshake_GetHeaderSize(ConnectionPtr);
  uint32 handshakeLength;
  Tls_PtrHandshakeData handshake = &ConnectionPtr->handshakeData;

  /* Check if the fragment buffer is large enough */
  if((handshake->handshakeFragmentBufferLength + Length) > TLS_HANDSHAKE_FRAGMENT_BUFFER_SIZE)
  {
    TLS_PRINTF(("Handshake buffer full.\n"));
    ret = E_NOT_OK;

    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
  }

  if(ret == E_OK)
  {
    /* copy data to buffer */
    TS_MemCpy(&handshake->handshakeFragmentBuffer[handshake->handshakeFragmentBufferLength], DataPtr, Length);
    handshake->handshakeFragmentBufferLength += Length;

    /* There may be more than one handshake message */
    while((ret == E_OK) && (handshake->handshakeFragmentBufferLength >= handshakeHeaderSize))
    {
      /* enough data to parse header*/
      handshakeLength = Tls_Handshake_GetLength(handshake->handshakeFragmentBuffer);
      if(handshake->handshakeFragmentBufferLength >= (handshakeLength + handshakeHeaderSize))
      {
        /* At least one handshake message is complete */
        ret = Tls_Handshake_Handle(ConnectionPtr, handshake->handshakeFragmentBuffer, handshakeLength + handshakeHeaderSize);
        /* If there is more data, move it to the beginning of the buffer */
        TS_MemMove(handshake->handshakeFragmentBuffer,
            &handshake->handshakeFragmentBuffer[handshakeLength + handshakeHeaderSize],
            handshake->handshakeFragmentBufferLength - handshakeLength - handshakeHeaderSize);
        handshake->handshakeFragmentBufferLength -= handshakeLength + handshakeHeaderSize;
      }
      else
      {
        /* Header is complete, but there is not enough data to parse the whole message */
        break;
      }
    }

  }

  return ret;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_ChangeCipherSpec_Handle(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret = E_OK;

  if (ConnectionPtr->config->entity == TLS_CLIENT)
  {
    if (ConnectionPtr->state == TLS_STATE_FINISHED_SENT)
    {
      Tls_SetState(ConnectionPtr, TLS_STATE_CHANGECIPHERSPEC_RECEIVED);
      Tls_Handshake_GenerateServerVerifyDataStart(ConnectionPtr);
      ConnectionPtr->cipher = ConnectionPtr->config->cipher;
      TLS_PRINTF(("changed cipher.\n"));
      Tls_OnReceiveChangeCipherSpec(ConnectionPtr);
    }
    else
    {
      ret = E_NOT_OK;
    }
  }
  else /* TLS_SERVER */
  {
    if
    (
#if TLS_CERTIFICATE_SUPPORT == STD_ON
        ((ConnectionPtr->config->remoteCertChainIdPtr != NULL_PTR) &&
         (ConnectionPtr->state == TLS_STATE_CERTIFICATE_VERIFY_DONE)) ||
#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */
        (ConnectionPtr->state == TLS_STATE_CLIENTKEYEXCHANGE_DONE)
    )
    {
      Tls_SetState(ConnectionPtr, TLS_STATE_CHANGECIPHERSPEC_RECEIVED);
      ConnectionPtr->cipher = ConnectionPtr->config->cipher;
      Tls_OnReceiveChangeCipherSpec(ConnectionPtr);
      TLS_PRINTF(("changed cipher.\n"));

      Tls_StartKeyDerivation(ConnectionPtr);
    }
    else if (ConnectionPtr->state == TLS_STATE_FINISHED_RECEIVED)
    {
      ret = E_OK;
    }
    else if (ConnectionPtr->state == TLS_STATE_CHANGECIPHERSPEC_SENT)
    {
      ret = E_OK;
    }
    else
    {
      ret = E_NOT_OK;
    }
  }

  if(ret != E_OK)
  {
    /* !LINKSTO Tls.RFC.Alert.UnexpectedMessage,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_UNEXPECTED_MESSAGE);
  }

  return ret;
}

STATIC FUNC(Tls_PtrConnection, TLS_CODE) Tls_FindConnection(TcpIp_SocketIdType SocketId)
{
  uint16 i;
  Tls_PtrConnection ConnectionPtr = NULL_PTR;

  for(i=0u;i<TLS_CFG_CONNECTION_COUNT;++i)
  {
    if ((Tls_Connections[i].socket == SocketId) && (Tls_Connections[i].active == TRUE))
    {
      ConnectionPtr = &Tls_Connections[i];
      break;
    }
  }

  if (ConnectionPtr == NULL_PTR)
  {
    TLS_PRINTF(("No ConnectionPtr for socket %d.\n", SocketId));
  }

  return ConnectionPtr;
}

FUNC(Std_ReturnType, TLS_CODE) Tls_GetConnectionState(TcpIp_SocketIdType SocketId)
{
  Std_ReturnType ret = E_NOT_OK;
  Tls_PtrConstConnection ConnectionPtr = Tls_FindConnection(SocketId);
  if(ConnectionPtr == NULL_PTR)
  {
    ret = TLS_E_NO_TLS_SOCKET;
  }
  else
  {
    if(ConnectionPtr->state == TLS_STATE_OK)
    {
      ret = E_OK;
    }
    else
    {
      ret = TLS_E_NOT_CONNECTED;
    }
  }
  return ret;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_ApplicationData_Handle(Tls_PtrConnection ConnectionPtr, Tls_PtrUint8 PayloadPtr, uint16 Length)
{
  Std_ReturnType ret = E_OK;

  if (ConnectionPtr->state == TLS_STATE_OK)
  {
    TLS_DEBUG_PRINTBYTES("ApplicationData", PayloadPtr, Length);

    Tls_UL_RxIndication(ConnectionPtr, PayloadPtr, Length);
  }
  else
  {
    TLS_PRINTF(("Tls_HandleApplicationData state != TLS_STATE_OK"));
    /* !LINKSTO Tls.RFC.Alert.UnexpectedMessage,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_UNEXPECTED_MESSAGE);
    ret = E_NOT_OK;
  }

  return ret;
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Alert_Handle(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 PayloadPtr, uint16 Length)
{
  Std_ReturnType ret = E_OK;
  uint8 level;
  uint8 desc;

  if(Length != TLS_ALERT_LENGTH)
  {
    /* !LINKSTO Tls.RFC.Alert.DecodeError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_DECODE_ERROR);
    ret = E_NOT_OK;
  }
  else
  {
    level = PayloadPtr[0];
    desc = PayloadPtr[1];

    TLS_PRINTF(("TLS Alert Level: %d\n", level));
    TLS_PRINTF(("TLS Alert Desc: %d\n", desc));

    /* !LINKSTO Tls.RFC.Alert.Received,1 */
    if(desc == TLS_ALERT_DESC_CLOSE_NOTIFY)
    {
      /* Answer close_notify */
      Tls_CloseInternal(ConnectionPtr);
    }
    else if(level == TLS_ALERT_LEVEL_FATAL)
    {
      /* Close on fatal alerts */
      Tls_SetState(ConnectionPtr, TLS_STATE_CLOSING);
      ConnectionPtr->errorFlag = TRUE;
    }
    else
    {
      /* Ignore warnings */
    }
  }
  return ret;
}

/* !LINKSTO Tls.Internal.Tls_TcpTransmit.CopyAndQueue,1 */
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Transmit(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 BufPtr, uint32 AvailableLength, boolean ForceRetrieve)
{
  Std_ReturnType ret = E_OK;
  uint8 headerSize;
  uint16 overhead;
  uint32 parts;
  uint32 partPayload;
  uint32 totalBytes;
  uint32 length;
  uint32 i;
  uint32 inPos;
  BufReq_ReturnType bufRet;
  uint32 saveTxBufferCount;

  /* Always copy all data */
  TS_PARAM_UNUSED(ForceRetrieve);

  if (ConnectionPtr->state != TLS_STATE_OK)
  {
    /* !LINKSTO Tls.API.Tls_TcpTransmit.State,1 */
    ret = TLS_E_NOT_CONNECTED;
  }

  if(ret == E_OK)
  {
    headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);
    overhead = Tls_GetRecordOverhead(ConnectionPtr);

    /* !LINKSTO Tls.RFC.HelloExtensions.RecordSizeLimit,1 */
    if((AvailableLength + overhead) > ConnectionPtr->transmitRecordSizeLimit)
    {
      /* Fragmentation necessary */
      partPayload = (uint32)ConnectionPtr->transmitRecordSizeLimit - overhead;
      parts = (AvailableLength / partPayload) + 1u;
      totalBytes = ((parts - 1u) * (partPayload + headerSize)) + (AvailableLength % partPayload) + headerSize;
      TLS_PRINTF(("Fragmentation: %d parts, %d bytes total in transmit buffer\n", parts, totalBytes));
    }
    else
    {
      /* Only one part */
      parts = 1u;
      partPayload = AvailableLength;
      totalBytes = AvailableLength + headerSize;
    }
  }

  /* Lock TX buffer for writing. It is also used in Tls_ProcessEncryption and Tls_ProcessMacGeneration. */
  TLS_ENTER_EXCLUSIVE_AREA_TX();

  /* Save the current tx buffer count to restore it if anything goes wrong. */
  saveTxBufferCount = ConnectionPtr->transmitBuffer.count;

  if(ret == E_OK)
  {
    /* Check if there is enough space in the transmit buffer */
    if(Tls_Buffer_GetFreeBytes(&ConnectionPtr->transmitBuffer) < totalBytes)
    {
      TLS_PRINTF(("Transmit buffer full\n"));
      ret = TLS_E_BUSY;
    }
  }

  if(ret == E_OK)
  {
    inPos = 0u;
    /* Copy the data chunks to individual records */
    for(i=0u;i<parts;++i)
    {
      if((i == (parts - 1u)) && (parts > 1u))
      {
        length = AvailableLength % partPayload;
      }
      else
      {
        length = partPayload;
      }

      Tls_WriteRecordHeaderToTransmitBuffer(ConnectionPtr, ConnectionPtr->transmitBuffer.count, length, TLS_RECORD_APPLICATIONDATA);
      ConnectionPtr->transmitBuffer.count += headerSize;

      if(BufPtr == NULL_PTR)
      {
        bufRet = Tls_UL_CopyTxData(ConnectionPtr, &ConnectionPtr->transmitBuffer.buffer[ConnectionPtr->transmitBuffer.count], length);
        if(bufRet != BUFREQ_OK)
        {
          TLS_PRINTF(("Tls_UL_CopyTxData failed: %d\n", bufRet));
          ret = E_NOT_OK;
        }
      }
      else
      {
        TS_MemCpy(&ConnectionPtr->transmitBuffer.buffer[ConnectionPtr->transmitBuffer.count], &BufPtr[inPos], length);
      }
      ConnectionPtr->transmitBuffer.count += length;
      inPos += length;
    }
  }

  if(ret != E_OK)
  {
    /* Restore buffer */
    ConnectionPtr->transmitBuffer.count = saveTxBufferCount;
  }

  TLS_EXIT_EXCLUSIVE_AREA_TX();

  if(ret == E_OK)
  {
    ret = Tls_TransmitRecord(ConnectionPtr);
  }

  TLS_PRINTF(("Tls_Transmit return %d\n", ret));

  return ret;
}

FUNC(Std_ReturnType, TLS_CODE) Tls_UdpTransmit(TcpIp_SocketIdType SocketId, P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) DataPtr,
       TcpIp_ConstSockAddrPtrType RemoteAddrPtr, uint16 TotalLength)
{
  Std_ReturnType ret = E_NOT_OK;
  Tls_PtrConnection ConnectionPtr = Tls_FindConnection(SocketId);
  TLS_PRINTF(("Tls_UdpTransmit\n"));
#if(TLS_DEV_ERROR_DETECT == STD_ON)
  if (Tls_Initialized == FALSE)
  {
    /* !LINKSTO Tls.API.Tls_UdpTransmit.Det.NoInit,1 */
    TLS_DET_REPORT_ERROR(TLS_UDPTRANSMIT_SVCID, TLS_E_UNINIT);
    ret = E_NOT_OK;
  }
  else
#endif
  {
    if(ConnectionPtr != NULL_PTR)
    {
      /* Check if this is a DTLS/UDP socket */
      if(ConnectionPtr->config->dtls)
      {
        if(ConnectionPtr->config->entity == TLS_CLIENT)
        {
          /* Take the first remote address as remote endpoint */
          if(ConnectionPtr->remoteAddr.domain == TCPIP_AF_UNSPEC)
          {
            ret = Tls_CopyIpAddress(ConnectionPtr, RemoteAddrPtr);
          }
          else
          {
            ret = E_OK;
          }
          if(ret == E_OK)
          {
            /* Start handshake */
            if(ConnectionPtr->state == TLS_STATE_DEFAULT)
            {
              Tls_SetState(ConnectionPtr, TLS_STATE_START);

              /* TODO: When starting the Tls handshake, the first udp message is lost.
               * Return TCPIP_E_PHYS_ADDR_MISS so that SoAd can buffer the message
               * and retry to send in the next mainfunction.
               * This is a temporary solution until SoAd supports TLS_E_NOT_CONNECTED return value */
              ret = TCPIP_E_PHYS_ADDR_MISS;
            }
          }
        }
        else
        {
          ret = E_OK;
        }

        if(ret == E_OK)
        {
          ret = Tls_Transmit(ConnectionPtr, DataPtr, TotalLength, TRUE);
        }
      }
    }
    else
    {
      ret = TLS_E_NO_TLS_SOCKET;
    }
  }
  return ret;
}

FUNC(Std_ReturnType, TLS_CODE) Tls_TcpTransmit(TcpIp_SocketIdType SocketId, P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) BufPtr,
    uint32 AvailableLength, boolean ForceRetrieve)
{
  Std_ReturnType ret = E_NOT_OK;
  Tls_PtrConnection ConnectionPtr;
  TLS_PRINTF(("Tls_TcpTransmit\n"));

#if(TLS_DEV_ERROR_DETECT == STD_ON)
  if (Tls_Initialized == FALSE)
  {
    TLS_DET_REPORT_ERROR(TLS_TCPTRANSMIT_SVCID, TLS_E_UNINIT);
    /* !LINKSTO Tls.API.Tls_TcpTransmit.Det.NoInit,1 */
  }
  else
#endif
  {
    ConnectionPtr = Tls_FindConnection(SocketId);
    if (ConnectionPtr == NULL_PTR)
    {
      ret = TLS_E_NO_TLS_SOCKET;
    }
    else
    {
      /* Check if this is a TLS/TCP socket */
      if(ConnectionPtr->config->dtls == FALSE)
      {
        ret = Tls_Transmit(ConnectionPtr, BufPtr, AvailableLength, ForceRetrieve);
      }
    }
  }

  return ret;
}

/* !LINKSTO Dtls.RFC.Record.AntiReplay,1 */
STATIC FUNC(void, TLS_CODE) Tls_Dtls_UpdateReceiveWindow(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 record)
{
  Tls_Uint64Type seq = Tls_Dtls_GetSeqNum(record);
  Tls_Uint64Type epoch = Tls_Dtls_GetEpoch(record);
  uint8 index;
  TLS_PRINTF(("Tls_Dtls_UpdateReceiveWindow BEGIN: receiveWindow=0x%X, readSeqNum=%d, seq=%d\n", ConnectionPtr->dtls.receiveWindow, ConnectionPtr->readSeqNum, seq ));

  if(epoch < ConnectionPtr->dtls.readEpoch)
  {
    /* Record is older than current windows. Do not update the seqNum */
  }
  else if (seq >= (ConnectionPtr->readSeqNum + TLS_DTLS_RECEIVE_WINDOW_SIZE))
  {
    /* Receive window is outdated */
    /* Set only one bit of receive window */
    ConnectionPtr->dtls.receiveWindow = 1u;
    ConnectionPtr->readSeqNum = seq;
  }
  else if(seq >= ConnectionPtr->readSeqNum)
  {
    /* Shift receive window */
    index = (uint8)(seq - ConnectionPtr->readSeqNum);
    ConnectionPtr->dtls.receiveWindow <<= index;
    ConnectionPtr->dtls.receiveWindow |= 1u;
    ConnectionPtr->readSeqNum = seq;
  }
  else
  {
    /* seq < ConnectionPtr->readSeqNum */
    index = (uint8)(ConnectionPtr->readSeqNum - seq);
    /* Record is older than newest record but in receive window: index < TLS_DTLS_RECEIVE_WINDOW_SIZE.
     * Otherwise it would not have passed Tls_Dtls_CheckSeqNum.  */
    /* Set bit in receive window */
    ConnectionPtr->dtls.receiveWindow |= (uint32)((uint32)1u << index);
  }

  TLS_PRINTF(("Tls_Dtls_UpdateReceiveWindow END: receiveWindow=0x%X, readSeqNum=%d\n", ConnectionPtr->dtls.receiveWindow, ConnectionPtr->readSeqNum));
}

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_Dtls_CheckSeqNum(Tls_PtrConnection ConnectionPtr, Tls_PtrConstUint8 record)
{
  Tls_Uint64Type seq = Tls_Dtls_GetSeqNum(record);
  uint16 epoch = Tls_Dtls_GetEpoch(record);

  Std_ReturnType ret = E_OK;
  uint8 index;

  /* !LINKSTO Dtls.RFC.Record.Epoch.Check,1 */
  if(seq <= ConnectionPtr->readSeqNum)
  {
    /* !LINKSTO Dtls.RFC.Record.AntiReplay,1 */
    if( (ConnectionPtr->config->dtls) && (epoch < ConnectionPtr->dtls.readEpoch) )
    {
      /* Old epoch number received */
      TLS_PRINTF(("Old epoch message received: readSeqNum=%d, seq=%d, readEpoch=%d, epoch=%d\n", ConnectionPtr->readSeqNum, seq, ConnectionPtr->dtls.readEpoch, epoch));
      ret = E_NOT_OK;
    }
    else if ((ConnectionPtr->readSeqNum - seq) > TLS_DTLS_RECEIVE_WINDOW_SIZE)
    {
      /* seq_num is outside receive window */
      TLS_PRINTF(("seq_num is outside receive window: readSeqNum=%d, seq=%d\n", ConnectionPtr->readSeqNum, seq));
      ret = E_NOT_OK;
    }
    else
    {
      /* seq_num is in receive window. Check against bit pattern. */
      index = (uint8)(ConnectionPtr->readSeqNum - seq);
      if((ConnectionPtr->dtls.receiveWindow & (uint32)((uint32)1u<<index)) != 0u)
      {
        /* Record with that sequence number was already received */
        TLS_PRINTF(("seq_num is in receive window but was already received: readSeqNum=%d, seq=%d, receiveWindow=%X\n", ConnectionPtr->readSeqNum, seq, ConnectionPtr->dtls.receiveWindow));
        ret = E_NOT_OK;
      }
    }
  }
  else
  {
    /* This is a new record */
  }

  if (ret != E_OK)
  {
    TLS_PRINTF(("Record Sequence check failed: %d != %d\n", seq, ConnectionPtr->readSeqNum));
  }

  return ret;
}

STATIC FUNC(void, TLS_CODE) Tls_RecordDecrypted(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret = E_OK;
  uint16 length = Tls_GetRecordLength(ConnectionPtr, ConnectionPtr->decryptBuffer.buffer);
  uint32 upperLayerAcceptedLength = 0U;
  uint8 headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);
  uint8 type = Tls_GetRecordType(ConnectionPtr->decryptBuffer.buffer);
  Tls_PtrUint8 payload = &ConnectionPtr->decryptBuffer.buffer[headerSize];

  TLS_PRINTF(("Tls_RecordDecrypted type=%d, length=%d\n", type, length));
  TLS_DEBUG_PRINTBYTES("Received (decrypted) TLS Record", ConnectionPtr->decryptBuffer.buffer, length + headerSize);

  /* Update the receive window for DTLS. MAC check is complete at this point. */
  if (ConnectionPtr->config->dtls)
  {
    Tls_Dtls_UpdateReceiveWindow(ConnectionPtr, ConnectionPtr->decryptBuffer.buffer);
  }

  switch (type)
  {
    case TLS_RECORD_HANDSHAKE:
    {
      if(ConnectionPtr->config->dtls)
      {
        ret = Tls_Dtls_Handshake_HandleFragment(ConnectionPtr, payload, length);
      }
      else
      {
        ret = Tls_Handshake_HandleFragment(ConnectionPtr, payload, length);
      }
      break;
    }
    case TLS_RECORD_CHANGECIPHERSPEC:
    {
      ret = Tls_ChangeCipherSpec_Handle(ConnectionPtr);
      break;
    }
    case TLS_RECORD_APPLICATIONDATA:
    {
      ret = Tls_ApplicationData_Handle(ConnectionPtr, payload, length);
      upperLayerAcceptedLength = length;
      break;
    }
    case TLS_RECORD_ALERT:
    {
      ret = Tls_Alert_Handle(ConnectionPtr, payload, length);
      break;
    }
    /* CHECK: NOPARSE */
    default:
    {
      /* Should not happen, type is checked before decryption */
      TLS_PRINTF(("Tls_RecordReceived type unknown: %d.\n", type));
      TLS_UNREACHABLE_CODE_ASSERT(TLS_INTERNAL_SVCID);
      break;
    }
  /* CHECK: PARSE */
  }

  if(ret == E_OK)
  {
    if(ConnectionPtr->config->dtls == FALSE)
    {
      if (type != TLS_RECORD_CHANGECIPHERSPEC)
      {
        TLS_PRINTF(("readSeqNum++\n"));
        ConnectionPtr->readSeqNum++;
      }

      TcpIp_TcpReceived(ConnectionPtr->socket, ConnectionPtr->receivedRecordSize - upperLayerAcceptedLength);
    }
  }
  else
  {
    /* Go to error state if something failed and connection is still open */
    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
  }
}

STATIC FUNC(void, TLS_CODE) Tls_RecordReceived(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret = E_OK;
  uint32 length = Tls_GetRecordLength(ConnectionPtr, ConnectionPtr->receiveBuffer.buffer);

  TLS_PRINTF(("TLS Record received: "));
  TLS_DEBUG_PRINTRECORDTYPE(Tls_GetRecordType(ConnectionPtr->receiveBuffer.buffer));
  TLS_PRINTF(("\n"));
  TLS_DEBUG_PRINTBYTES("Received (encrypted) TLS Record", ConnectionPtr->receiveBuffer.buffer, length + Tls_GetRecordHeaderSize(ConnectionPtr));

  if ((ret == E_OK) && (ConnectionPtr->config->dtls == TRUE))
  {
    if (Tls_Dtls_CheckSeqNum(ConnectionPtr, ConnectionPtr->receiveBuffer.buffer) != E_OK)
    {
      ret = E_NOT_OK;
      /* Remove the record from the buffer */
      (void)Tls_ConsumeRecordFromReceiveBuffer(ConnectionPtr);
    }
  }

  if(ret == E_OK)
  {
    ConnectionPtr->receivedRecordSize = length + Tls_GetRecordHeaderSize(ConnectionPtr);

    if ((ConnectionPtr->state == TLS_STATE_OK)  || (ConnectionPtr->state == TLS_STATE_CHANGECIPHERSPEC_RECEIVED))
    {
      if(ConnectionPtr->cipherParams->cipherType == TLS_AEAD_CIPHER)
      {
        /* Start decryption */
        ConnectionPtr->decryptState = TLS_DECRYPT_STATE_START;
      }
      else
      {
        /* MAC check only */
        ConnectionPtr->macVerifyState = TLS_MAC_STATE_START;
        TLS_PRINTF(("macVerifyState = TLS_MAC_STATE_START\n"));
      }
    }
    else
    {
      /* No cipher and no MAC check */
      /* TODO: do not throw away return values */
      (void)Tls_Buffer_Copy(&ConnectionPtr->receiveBuffer, ConnectionPtr->decryptBuffer.buffer, 0u, length + Tls_GetRecordHeaderSize(ConnectionPtr));
      (void)Tls_ConsumeRecordFromReceiveBuffer(ConnectionPtr);
      Tls_RecordDecrypted(ConnectionPtr);
    }
  }
}

STATIC FUNC(boolean, TLS_CODE) Tls_CheckRecordType(uint8 Type)
{
  boolean ret = FALSE;

  switch (Type)
  {
    case TLS_RECORD_HANDSHAKE:
    case TLS_RECORD_CHANGECIPHERSPEC:
    case TLS_RECORD_APPLICATIONDATA:
    case TLS_RECORD_ALERT:
    {
      ret = TRUE;
      break;
    }
    default:
    {
      break;
    }
  }

  return ret;
}

/* !LINKSTO Tls.RFC.Record.Format,1 */
STATIC FUNC(void, TLS_CODE) Tls_HandleReceiveBuffer(Tls_PtrConnection ConnectionPtr)
{
  uint32 length;
  uint8 headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);
  Std_ReturnType ret = E_OK;
  uint16 recordVersion;
  uint16 version;
  uint8 recordType;

  /* Check if header is complete and all relevant asynchronous operations are idle */
  while
  (
    (ConnectionPtr->receiveBuffer.count >= headerSize) &&
    (ConnectionPtr->macVerifyState == TLS_MAC_STATE_IDLE) &&
    (ConnectionPtr->decryptState == TLS_DECRYPT_STATE_IDLE) &&
    (ConnectionPtr->handshakeData.dtls_cookieState == TLS_RANDOM_STATE_IDLE) &&
    (ConnectionPtr->handshakeData.randomState != TLS_RANDOM_STATE_BUSY) &&
    ((ConnectionPtr->handshakeData.keyDeriveState == TLS_KEYDERIVE_STATE_READY) || (ConnectionPtr->handshakeData.keyDeriveState == TLS_KEYDERIVE_STATE_IDLE)) &&
    (ConnectionPtr->handshakeData.verifyCertState == TLS_VERIFY_CERT_STATE_IDLE) &&
    (ConnectionPtr->handshakeData.sigGenState == TLS_SIG_GEN_STATE_IDLE) &&
    (ConnectionPtr->handshakeData.sigVerState == TLS_SIG_VER_STATE_IDLE) &&
    (ConnectionPtr->handshakeData.keCalcPubValState == TLS_KE_CALC_PUB_VAL_STATE_IDLE) &&
    (ConnectionPtr->handshakeData.keCalcSecretState == TLS_KE_CALC_SECRET_STATE_IDLE) &&
    (ConnectionPtr->state != TLS_STATE_ERROR) &&
    (ConnectionPtr->state != TLS_STATE_CLOSED) &&
    (ConnectionPtr->state != TLS_STATE_CLOSING)
  )
  {
    ret = E_OK;

    /* Header is complete. Get the payload length and record type. */
    length = Tls_GetRecordLength(ConnectionPtr, ConnectionPtr->receiveBuffer.buffer);
    recordType = Tls_GetRecordType(ConnectionPtr->receiveBuffer.buffer);

    /* Check record length */
    /* !LINKSTO Tls.RFC.HelloExtensions.RecordSizeLimit.recordoverflow,1 */
    if ((length + (uint32)headerSize) > (uint32)ConnectionPtr->config->maxRecordSize)
    {
      /* Record too long */
      TLS_PRINTF(("Record too long.\n"));
      /* !LINKSTO Tls.RFC.Alert.RecordOverflow,1 */
      Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_RECORD_OVERFLOW);
      ret = E_NOT_OK;
    }

    if(ret == E_OK)
    {
      /* !LINKSTO Dtls.RFC.Record.Major.Version.Check,1 */
      /* Check record version */
      recordVersion = Tls_GetRecordVersion(ConnectionPtr->receiveBuffer.buffer);
      version = Tls_GetVersion(ConnectionPtr);
      if((ConnectionPtr->state == TLS_STATE_DEFAULT) && (ConnectionPtr->config->entity == TLS_SERVER))
      {
        /* The TLS server may accept lower version numbers. Only compare the high byte. */
        /* !LINKSTO Dtls.RFC.Record.Minor.Version.Check,1 */
        if((recordVersion >> 8) != (version >> 8))
        {
          ret = E_NOT_OK;
        }
      }
      else if((ConnectionPtr->state == TLS_STATE_CLIENTHELLO_DONE) && (ConnectionPtr->config->entity == TLS_CLIENT) && \
              (recordType == TLS_RECORD_HANDSHAKE)
             )
      {
        Tls_PtrConstUint8 payload = &ConnectionPtr->receiveBuffer.buffer[headerSize];

        if(Tls_Handshake_GetType(payload) == TLS_HANDSHAKE_HELLOVERIFYREQUEST)
        {
          /* The DTLS client may accept lower version numbers. Only compare the high byte. */
          /* !LINKSTO Dtls.RFC.Record.Minor.Version.Check,1 */
          if((recordVersion >> 8) != (version >> 8))
          {
            ret = E_NOT_OK;
          }
        }
        else
        {
          if (recordVersion != version)
          {
            ret = E_NOT_OK;
          }
        }
      }
      else if (recordVersion != version)
      {
        ret = E_NOT_OK;
      }
      else
      {
        /* For MISRA checker */
      }
      if(ret == E_NOT_OK)
      {
        /* Wrong version field */
        TLS_PRINTF(("Wrong record version: %04x\n", Tls_GetRecordVersion(ConnectionPtr->receiveBuffer.buffer)));
        /* !LINKSTO Tls.RFC.Alert.DecodeError,1 */
        Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_DECODE_ERROR);
      }
    }

    if(ret == E_OK)
    {
      /* Check record type */
      if(Tls_CheckRecordType(recordType) == FALSE)
      {
        /* Wrong type field */
        TLS_PRINTF(("Wrong record type.\n"));
        /* !LINKSTO Tls.RFC.Alert.DecodeError,1 */
        Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_DECODE_ERROR);
        ret = E_NOT_OK;
      }
    }

    /* Check if record is complete */
    if(ConnectionPtr->receiveBuffer.count < (length + headerSize))
    {
      /* record is not complete yet */
      break;
    }

    if(ret == E_OK)
    {
      Tls_RecordReceived(ConnectionPtr);
    }
  }
}

#if TLS_CERTIFICATE_SUPPORT == STD_ON
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_TransmitCertificateRequest(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret = E_OK;
  uint32 size;
  Tls_ConstUint8PtrType recordPtr;

  /* If configured transmit CertificateRequest */
  if(ConnectionPtr->config->remoteCertChainIdPtr != NULL_PTR)
  {
    size = Tls_Handshake_WriteServerCertificateRequest(ConnectionPtr);

    /* Append handshake for verify_data */
    recordPtr = &ConnectionPtr->transmitBuffer.buffer[(uint32)ConnectionPtr->transmitBuffer.count - size];
    Tls_Handshake_Append(ConnectionPtr, recordPtr, size);

    /* Transmit record */
    ret = Tls_TransmitRecord(ConnectionPtr);
    if(ret == E_OK)
    {
      /* LINKSTO Dtls.RFC.Handshake.MessageSeq.Increment,1 */
      ConnectionPtr->handshakeData.dtls_handshakeSeq++;
      Tls_SetState(ConnectionPtr, TLS_STATE_CERTIFICATE_REQUEST_DONE);
    }
  }

  return ret;
}
#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */

STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_TransmitServerHelloDone(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret;
  uint32 size;

  /* Build ServerHelloDone */
  size = Tls_Handshake_WriteServerHelloDone(ConnectionPtr);
  /* Append handshake for verify_data */
  Tls_Handshake_Append(ConnectionPtr,
      &ConnectionPtr->transmitBuffer.buffer[(uint32)ConnectionPtr->transmitBuffer.count - size], size);
  /* Transmit record */
  ret = Tls_TransmitRecord(ConnectionPtr);
  if(ret == E_OK)
  {
    /* LINKSTO Dtls.RFC.Handshake.MessageSeq.Increment,1 */
    ConnectionPtr->handshakeData.dtls_handshakeSeq++;
    Tls_SetState(ConnectionPtr, TLS_STATE_SERVERHELLODONE_DONE);
  }

  return ret;
}

FUNC(void, TLS_CODE) Tls_RxIndication
(
    TcpIp_SocketIdType SocketId,
    TcpIp_ConstSockAddrPtrType RemoteAddrPtr,
    P2CONST(uint8, AUTOMATIC, TLS_APPL_DATA) BufPtr, uint16 Length
)
{
  Tls_PtrConnection ConnectionPtr = NULL_PTR;
  Std_ReturnType ret = E_OK;

#if(TLS_DEV_ERROR_DETECT == STD_ON)
  if (Tls_Initialized == FALSE)
  {
    /* !LINKSTO Tls.API.Tls_RxIndication.Det.NoInit,1 */
    TLS_DET_REPORT_ERROR(TLS_RXINDICATION_SVCID, TLS_E_UNINIT);
  }
  else if (RemoteAddrPtr == NULL_PTR)
  {
    /* !LINKSTO Tls.API.Tls_RxIndication.Det.InvArg.RemoteAddrPtr,1 */
    TLS_DET_REPORT_ERROR(TLS_RXINDICATION_SVCID, TLS_E_INV_ARG);
  }
  else if (BufPtr == NULL_PTR)
  {
    /* !LINKSTO Tls.API.Tls_RxIndication.Det.InvArg.BufPtr,1 */
    TLS_DET_REPORT_ERROR(TLS_RXINDICATION_SVCID, TLS_E_INV_ARG);
  }
  else
#endif
  {
    ConnectionPtr = Tls_FindConnection(SocketId);
    if (ConnectionPtr == NULL_PTR)
    {
      ret = TLS_E_NO_TLS_SOCKET;
    }

    if(ret == E_OK)
    {
      if((ConnectionPtr->state == TLS_STATE_CLOSING) ||
          (ConnectionPtr->state == TLS_STATE_CLOSED) ||
          (ConnectionPtr->state == TLS_STATE_ERROR))
      {
        ret = E_NOT_OK;
        TLS_PRINTF(("RxIndication: Data dropped because of wrong state.\n"));
      }
    }

    if(ret == E_OK)
    {
      if(ConnectionPtr->config->dtls == TRUE)
      {
        if(ConnectionPtr->config->entity == TLS_SERVER)
        {
          if(ConnectionPtr->state == TLS_STATE_DEFAULT)
          {
            /* Copy the address if no connection or handshake is running */
            ret = Tls_CopyIpAddress(ConnectionPtr, RemoteAddrPtr);
          }
          else
          {
            /* Check if the packet comes from the right remote address */
            ret = Tls_CheckIpAddress(ConnectionPtr, RemoteAddrPtr);
          }
        }
        else /* TLS_CLIENT */
        {
          /* Check if the packet comes from the right remote address */
          ret = Tls_CheckIpAddress(ConnectionPtr, RemoteAddrPtr);
        }
      }
    }

    if(ret == E_OK)
    {
      TLS_ENTER_EXCLUSIVE_AREA_RX();
      /* Copy data to the receive buffer */
      ret = Tls_Buffer_Append(&ConnectionPtr->receiveBuffer, BufPtr, Length);
      TLS_EXIT_EXCLUSIVE_AREA_RX();

      if(ret != E_OK)
      {
        /* Receive buffer full */
        /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
        Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
      }
    }
  }
}

FUNC(Std_ReturnType, TLS_CODE) Tls_TcpConnected(TcpIp_SocketIdType SocketId, TcpIp_ConstSockAddrPtrType RemoteAddrPtr)
{
  Std_ReturnType ret = E_OK;
  Tls_PtrConnection ConnectionPtr = NULL_PTR;

  TLS_PRINTF(("Tls_TcpConnected\n"));

#if(TLS_DEV_ERROR_DETECT == STD_ON)
  if (Tls_Initialized == FALSE)
  {
    /* !LINKSTO Tls.API.Tls_TcpConnected.Det.NoInit,1 */
    TLS_DET_REPORT_ERROR(TLS_TCPCONNECTED_SVCID, TLS_E_UNINIT);
    ret = E_NOT_OK;
  }
  else
#endif
  {
    ConnectionPtr = Tls_FindConnection(SocketId);
    if (ConnectionPtr != NULL_PTR)
    {
      if(ConnectionPtr->state == TLS_STATE_DEFAULT)
      {
        ret = Tls_CopyIpAddress(ConnectionPtr, RemoteAddrPtr);
        if(ret == E_OK)
        {
          Tls_SetState(ConnectionPtr, TLS_STATE_START);
        }
      }
      else
      {
        /* TcpConnected was called while not in default state */
        ret = E_NOT_OK;
        TLS_PRINTF(("TcpConnected failed: Not in default state.\n"));
      }
    }
    else
    {
      ret = TLS_E_NO_TLS_SOCKET;
    }
  }
  return ret;
}


STATIC FUNC(void, TLS_CODE) Tls_HandleTransmitBuffer(Tls_PtrConnection ConnectionPtr)
{
  uint8 header_size = Tls_GetRecordHeaderSize(ConnectionPtr);
  Std_ReturnType ret = E_OK;
  uint16 length;

  if (ConnectionPtr->state == TLS_STATE_ERROR)
  {
    ret = E_NOT_OK;
  }

  if(ret == E_OK)
  {
    if(ConnectionPtr->encryptBufferLength <= header_size)
    {
      ret = E_NOT_OK;
    }
  }

  if(ret == E_OK)
  {
    length = Tls_GetRecordLength(ConnectionPtr, ConnectionPtr->encryptBuffer);
  }

  if (ret == E_OK)
  {
    TLS_PRINTF(("Tls_HandleTransmitBuffer: Transmit %d bytes\n", ConnectionPtr->encryptBufferLength));
    ret = Tls_TcpIp_Transmit(ConnectionPtr, (uint32)length + (uint32)header_size);
    if(ret == E_OK)
    {
      ConnectionPtr->encryptBufferLength = 0u;
    }
  }
}

#if( TLS_CSM_KEYDERIVE_SUPPORT == STD_OFF )
/* !LINKSTO Tls.RFC.KeyCalculation,1 */
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_KeyDeriveSetCsmKeys(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret = E_OK;
  uint32 pos;
  Tls_PtrHandshakeData data = &ConnectionPtr->handshakeData;
  Tls_PtrConstCipherSuiteParameters params = ConnectionPtr->cipherParams;
  Tls_PtrPRF_Context prf = &data->prf;

  uint32 clientWriteMacKeyId;
  uint32 serverWriteMacKeyId;
  uint32 clientWriteKeyId;
  uint32 serverWriteKeyId;

  if(ConnectionPtr->config->entity == TLS_CLIENT)
  {
    clientWriteMacKeyId = ConnectionPtr->config->MACGenerateKeyId;
    serverWriteMacKeyId = ConnectionPtr->config->MACVerifyKeyId;
    clientWriteKeyId = ConnectionPtr->config->encryptKeyId;
    serverWriteKeyId = ConnectionPtr->config->decryptKeyId;
  }
  else
  {
    clientWriteMacKeyId = ConnectionPtr->config->MACVerifyKeyId;
    serverWriteMacKeyId = ConnectionPtr->config->MACGenerateKeyId;
    clientWriteKeyId = ConnectionPtr->config->decryptKeyId;
    serverWriteKeyId = ConnectionPtr->config->encryptKeyId;
  }

  pos = 0u;
  if(params->MACKeyLength > 0u)
  {
    TLS_DEBUG_PRINTBYTES("ClientMacKey", &prf->result[pos], params->MACKeyLength);
    if(Tls_Csm_KeyElementSet(clientWriteMacKeyId, CRYPTO_KE_MAC_KEY, &prf->result[pos], params->MACKeyLength) != E_OK)
    {
      ret = E_NOT_OK;
    }
    pos += params->MACKeyLength;

    TLS_DEBUG_PRINTBYTES("ServerMacKey", &prf->result[pos], params->MACKeyLength);
    if(Tls_Csm_KeyElementSet(serverWriteMacKeyId, CRYPTO_KE_MAC_KEY, &prf->result[pos], params->MACKeyLength) != E_OK)
    {
      ret = E_NOT_OK;
    }
    pos += params->MACKeyLength;
  }

  if(params->encKeyLength > 0u)
  {
    TLS_DEBUG_PRINTBYTES("ClientWriteKey", &prf->result[pos], params->encKeyLength);
    if(Tls_Csm_KeyElementSet(clientWriteKeyId, CRYPTO_KE_CIPHER_KEY, &prf->result[pos], params->encKeyLength) != E_OK)
    {
      ret = E_NOT_OK;
    }
    pos += params->encKeyLength;

    TLS_DEBUG_PRINTBYTES("ServerWriteKey", &prf->result[pos], params->encKeyLength);
    if(Tls_Csm_KeyElementSet(serverWriteKeyId, CRYPTO_KE_CIPHER_KEY, &prf->result[pos], params->encKeyLength) != E_OK)
    {
      ret = E_NOT_OK;
    }
    pos += params->encKeyLength;
  }

  if(params->IVLength > 0u)
  {
    TLS_DEBUG_PRINTBYTES("ClientWriteIV", &prf->result[pos], params->IVLength);
    if(ConnectionPtr->config->entity == TLS_CLIENT)
    {
      TS_MemCpy(ConnectionPtr->writeIV, &prf->result[pos], params->IVLength);
    }
    else
    {
      TS_MemCpy(ConnectionPtr->readIV, &prf->result[pos], params->IVLength);
    }

    pos += params->IVLength;

    TLS_DEBUG_PRINTBYTES("ServerWriteIV", &prf->result[pos], params->IVLength);
    if(ConnectionPtr->config->entity == TLS_CLIENT)
    {
      TS_MemCpy(ConnectionPtr->readIV, &prf->result[pos], params->IVLength);
    }
    else
    {
      TS_MemCpy(ConnectionPtr->writeIV, &prf->result[pos], params->IVLength);
    }

    pos += params->IVLength;
  }

  return ret;
}
#endif /* TLS_CSM_KEYDERIVE_SUPPORT == STD_OFF */

#if( TLS_CSM_KEYDERIVE_SUPPORT == STD_ON )
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_CsmKeyDerive(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret = E_OK;
  Tls_PtrHandshakeData data = &ConnectionPtr->handshakeData;
  uint32 keyId = ConnectionPtr->config->PreMasterSecretKeyId;
  uint8 algorithm = Tls_GetCsmKeyDeriveAlgorithm(ConnectionPtr->config->cipher);

  /* !LINKSTO Tls.Design.CsmKeyDerivation,1 */
  ret = Csm_KeyElementSet(keyId, TLS_CRYPTO_KE_KEYDERIVATION_TLS_PSK_RANDOM_CLIENT, data->clientRandom, TLS_RANDOM_SIZE);

  if(ret == E_OK)
  {
    ret = Csm_KeyElementSet(keyId, TLS_CRYPTO_KE_KEYDERIVATION_TLS_PSK_RANDOM_SERVER, data->serverRandom, TLS_RANDOM_SIZE);
  }

  if(ret == E_OK)
  {
    ret = Csm_KeyElementSet(keyId, CRYPTO_KE_KEYDERIVATION_ALGORITHM, &algorithm, TLS_HSM_ALGORITHM_LEN);
  }

  if(ret == E_OK)
  {
    ret = Csm_KeySetValid(keyId);
  }

  if(ret == E_OK)
  {
    if( TLS_CIPHER_USES_ENCRYPTION(ConnectionPtr) )
    {
      ret = Csm_KeyDerive(keyId, ConnectionPtr->config->encryptKeyId);
    }
    else
    {
      ret = Csm_KeyDerive(keyId, ConnectionPtr->config->MACGenerateKeyId);
    }
  }

  if(ret == E_OK)
  {
    if( TLS_CIPHER_USES_AUTHENTICATION(ConnectionPtr) )
    {
      if(Csm_KeySetValid(ConnectionPtr->config->MACGenerateKeyId) != E_OK)
      {
        ret = E_NOT_OK;
      }
      if(Csm_KeySetValid(ConnectionPtr->config->MACVerifyKeyId) != E_OK)
      {
        ret = E_NOT_OK;
      }
    }

    if( TLS_CIPHER_USES_ENCRYPTION(ConnectionPtr) )
    {
      if(Csm_KeySetValid(ConnectionPtr->config->encryptKeyId) != E_OK)
      {
        ret = E_NOT_OK;
      }

      if(Csm_KeySetValid(ConnectionPtr->config->decryptKeyId) != E_OK)
      {
        ret = E_NOT_OK;
      }
    }
  }

  return ret;
}
#endif /* TLS_CSM_KEYDERIVE_SUPPORT == STD_ON */

STATIC FUNC(void, TLS_CODE) Tls_KeyDeriveMainFunction(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret = E_OK;
  Tls_PtrHandshakeData data = &ConnectionPtr->handshakeData;

#if( TLS_CSM_KEYDERIVE_SUPPORT == STD_OFF )
  uint32 keyBlockSize;
  Tls_PtrConstCipherSuiteParameters params = ConnectionPtr->cipherParams;
  Tls_PtrPRF_Context prf = &data->prf;
#endif /* TLS_CSM_KEYDERIVE_SUPPORT == STD_OFF */

  if(data->keyDeriveState == TLS_KEYDERIVE_STATE_START)
  {

#if( TLS_CSM_KEYDERIVE_SUPPORT == STD_ON )
  ret = Tls_CsmKeyDerive(ConnectionPtr);

  if(ret == E_OK)
  {
    data->keyDeriveState = TLS_KEYDERIVE_STATE_READY;
    TLS_PRINTF("Keys Ready.\n");
  }
  else
  {
    data->keyDeriveState = TLS_KEYDERIVE_STATE_ERROR;
    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);

#if(TLS_DEV_ERROR_DETECT == STD_ON)
    TLS_DET_REPORT_ERROR(TLS_INTERNAL_SVCID, TLS_E_CSM_KEY_INVALID);
#endif
  }

#else /* TLS_CSM_KEYDERIVE_SUPPORT == STD_OFF */
    /* !LINKSTO Tls.RFC.Handshake.MasterSecret,1 */
    data->keyDeriveState = TLS_KEYDERIVE_STATE_BUSY;

    prf->state = TLS_PRF_STATE_START;
    prf->jobId = ConnectionPtr->config->PRFPreMasterSecretJobId;
    prf->resultLength = TLS_MASTER_SECRET_SIZE;
    prf->seedLength = (2u * TLS_RANDOM_SIZE) + TLS_MASTERSECRET_STRING_LENGTH;

    TS_MemCpy(prf->seed, Tls_String_MasterSecret, TLS_MASTERSECRET_STRING_LENGTH);
    TS_MemCpy(&prf->seed[TLS_MASTERSECRET_STRING_LENGTH], data->clientRandom, TLS_RANDOM_SIZE);
    TS_MemCpy(&prf->seed[TLS_MASTERSECRET_STRING_LENGTH + TLS_RANDOM_SIZE], data->serverRandom, TLS_RANDOM_SIZE);

    (void)Csm_KeySetValid(ConnectionPtr->config->PreMasterSecretKeyId);

#endif /* TLS_CSM_KEYDERIVE_SUPPORT == STD_ON */
  }

#if( TLS_CSM_KEYDERIVE_SUPPORT == STD_OFF )
  if(data->keyDeriveState == TLS_KEYDERIVE_STATE_BUSY)
  {
    if(prf->state == TLS_PRF_STATE_READY)
    {
      /* Start mastersecret generation */
      prf->state = TLS_PRF_STATE_IDLE;
      TLS_PRINTF(("Master key ready.\n"));
      TLS_DEBUG_PRINTBYTES("ClientRandom", data->clientRandom, TLS_RANDOM_SIZE);
      TLS_DEBUG_PRINTBYTES("ServerRandom", data->serverRandom, TLS_RANDOM_SIZE);
      TLS_DEBUG_PRINTBYTES("MasterSecret", prf->result, TLS_MASTER_SECRET_SIZE);

      ret = Csm_KeyElementSet(ConnectionPtr->config->MasterSecretKeyId, CRYPTO_KE_MAC_KEY, prf->result, TLS_MASTER_SECRET_SIZE);
      if(ret != E_OK)
      {
        data->keyDeriveState = TLS_KEYDERIVE_STATE_ERROR;
        /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
        Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
#if(TLS_DEV_ERROR_DETECT == STD_ON)
        TLS_DET_REPORT_ERROR(TLS_INTERNAL_SVCID, TLS_E_CSM_KEY_INVALID);
#endif
        TLS_PRINTF(("Csm_KeyElementSet: %d\n", ret));
      }
      else
      {
        data->keyDeriveState = TLS_KEYDERIVE_STATE_BUSY_KEYS;
        (void)Csm_KeySetValid(ConnectionPtr->config->MasterSecretKeyId);
      }
    }
  }

  if(data->keyDeriveState == TLS_KEYDERIVE_STATE_BUSY_KEYS)
  {
    if(prf->state == TLS_PRF_STATE_IDLE)
    {
      TLS_PRINTF(("Generate keys.\n"));
      /* Start key derivation */
      keyBlockSize = ((uint32)2u * params->MACKeyLength) +
                     ((uint32)2u * params->encKeyLength) +
                     ((uint32)2u * params->IVLength);

      prf->state = TLS_PRF_STATE_START;
      prf->jobId = ConnectionPtr->config->PRFMasterSecretJobId;
      prf->resultLength = keyBlockSize;
      prf->seedLength = TLS_KEYEXPANSION_STRING_LENGTH + (2u * TLS_RANDOM_SIZE);

      TS_MemCpy(prf->seed, Tls_String_KeyExpansion, TLS_KEYEXPANSION_STRING_LENGTH);
      TS_MemCpy(&prf->seed[TLS_KEYEXPANSION_STRING_LENGTH], data->serverRandom, TLS_RANDOM_SIZE);
      TS_MemCpy(&prf->seed[TLS_KEYEXPANSION_STRING_LENGTH + TLS_RANDOM_SIZE], data->clientRandom, TLS_RANDOM_SIZE);

    }

    if(prf->state == TLS_PRF_STATE_READY)
    {
      TLS_PRINTF(("Keys Ready.\n"));
      prf->state = TLS_PRF_STATE_IDLE;

      ret = Tls_KeyDeriveSetCsmKeys(ConnectionPtr);

      if(ret == E_OK)
      {
        data->keyDeriveState = TLS_KEYDERIVE_STATE_READY;
      }
      else
      {
        data->keyDeriveState = TLS_KEYDERIVE_STATE_ERROR;
        /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
        Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
#if(TLS_DEV_ERROR_DETECT == STD_ON)
        TLS_DET_REPORT_ERROR(TLS_INTERNAL_SVCID, TLS_E_CSM_KEY_INVALID);
#endif
      }
    }
  }
#endif /* TLS_CSM_KEYDERIVE_SUPPORT == STD_OFF */
}

STATIC FUNC(void, TLS_CODE) Tls_ProcessVerifyData
(
  Tls_PtrConnection ConnectionPtr,
  P2VAR(Tls_VerifyDataStateType, AUTOMATIC, TLS_VAR) statePtr,
  Tls_PtrUint8 verifyDataPtr,
  P2CONST(uint8, AUTOMATIC, TLS_CONST) label,
  uint32 labelLength
)
{
  Tls_PtrHandshakeData data = &ConnectionPtr->handshakeData;
  Tls_PtrPRF_Context prf = &data->prf;
  Std_ReturnType ret = E_OK;

  if((*statePtr) == TLS_VERIFYDATA_STATE_HASH_READY)
  {
    /* PRF state should be always IDLE at this point. */
    /* CHECK: NOPARSE */
    if(prf->state == TLS_PRF_STATE_IDLE)
    /* CHECK: PARSE */
    {
      TLS_PRINTF(("VerifyData start PRF.\n"));

      prf->state = TLS_PRF_STATE_START;
      prf->jobId = ConnectionPtr->config->PRFMasterSecretJobId;
      prf->resultLength = TLS_VERIFY_DATA_LENGTH;
      prf->seedLength = TLS_SHA256_SIZE + labelLength;
      TS_MemCpy(prf->seed, data->verifyDataSeed, TLS_SHA256_SIZE + labelLength);

      *statePtr = TLS_VERIFYDATA_STATE_PRF_BUSY;
    }
  }

  if((*statePtr) == TLS_VERIFYDATA_STATE_PRF_BUSY)
  {
    if(prf->state == TLS_PRF_STATE_READY)
    {
      TS_MemCpy(verifyDataPtr, prf->result, TLS_VERIFY_DATA_LENGTH);
      prf->state = TLS_PRF_STATE_IDLE;
      *statePtr = TLS_VERIFYDATA_STATE_READY;
      TLS_PRINTF(("VerifyData PRF ready.\n"));
    }
  }

  if((*statePtr) == TLS_VERIFYDATA_STATE_START)
  {
    /* TODO: check if necessary, should be handled by Crypto */
    if( ( (ConnectionPtr->macGenerateState == TLS_MAC_STATE_IDLE) || (ConnectionPtr->macGenerateState == TLS_MAC_STATE_READY) ) &&
        ( (ConnectionPtr->macVerifyState == TLS_MAC_STATE_IDLE) || (ConnectionPtr->macVerifyState == TLS_MAC_STATE_READY) )
      )
    {
      ret = Tls_Handshake_GenerateVerifyDataStart(ConnectionPtr, label, labelLength);
      if(ret == E_OK)
      {
        TLS_PRINTF(("VerifyDataState = TLS_VERIFYDATA_STATE_HASH_BUSY\n"));
        *statePtr = TLS_VERIFYDATA_STATE_HASH_BUSY;
#if TLS_CFG_CSM_PROCESSING == TLS_CSM_SYNCHRONOUS
        /* !LINKSTO Tls.Async,1 */
        Tls_HashCallback(ConnectionPtr, E_OK);
#endif
      }
      else
      {
        *statePtr = TLS_VERIFYDATA_STATE_ERROR;
        /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
        Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
      }
    }
  }
}

/* !LINKSTO Tls.RFC.Record.MAC,1 */
STATIC FUNC(void, TLS_CODE) Tls_ProcessMacGeneration(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret = E_OK;
  uint8 headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);
  uint8 macLength = ConnectionPtr->cipherParams->MACLength;

  if(Tls_CheckTransmitBuffer(ConnectionPtr) == E_OK)
  {
    /* TODO: check if necessary, should be handled by Crypto */
    if( (ConnectionPtr->cipherParams->cipherType == TLS_NULL_CIPHER) &&
         ((ConnectionPtr->state == TLS_STATE_CHANGECIPHERSPEC_SENT) ||
          (ConnectionPtr->state == TLS_STATE_OK) ||
          (ConnectionPtr->state == TLS_STATE_CLOSING)
         )
      )
    {
      /* MAC only */
      if(ConnectionPtr->macGenerateState == TLS_MAC_STATE_IDLE)
      {
        /* Start MAC generation */
        TLS_PRINTF(("macGenerateState = TLS_MAC_STATE_START\n"));
        ConnectionPtr->macGenerateState = TLS_MAC_STATE_START;
      }
    }
  }

  if(ConnectionPtr->macGenerateState == TLS_MAC_STATE_START)
  {
    /* start MAC generation */
    ret = Tls_StartMacGenerate(ConnectionPtr);
    if(ret == E_OK)
    {
      ConnectionPtr->macGenerateState = TLS_MAC_STATE_HEADER_BUSY;
#if TLS_CFG_CSM_PROCESSING == TLS_CSM_SYNCHRONOUS
      /* !LINKSTO Tls.Async,1 */
      /* Call Csm callback if the MacGenerate job is synchronous */
      Tls_MacGenerateCallback(ConnectionPtr, E_OK);
#endif
    }
  }

  if((ret == E_OK) && (ConnectionPtr->macGenerateState == TLS_MAC_STATE_HEADER_READY))
  {
    /* Header ready, start payload processing */
    ret = Csm_MacGenerate(ConnectionPtr->config->MACGenerateJobId, CRYPTO_OPERATIONMODE_UPDATE,
                          &ConnectionPtr->transmitBuffer.buffer[headerSize],
                          Tls_GetRecordLength(ConnectionPtr, ConnectionPtr->transmitBuffer.buffer),
                          /*NULL_PTR*/&ConnectionPtr->encryptBuffer[headerSize + Tls_GetRecordLength(ConnectionPtr, ConnectionPtr->transmitBuffer.buffer)],
                          /*NULL_PTR*/&ConnectionPtr->macGenerateLength
                         );
    if(ret == E_OK)
    {
      ConnectionPtr->macGenerateState = TLS_MAC_STATE_PAYLOAD_BUSY;
#if TLS_CFG_CSM_PROCESSING == TLS_CSM_SYNCHRONOUS
      /* !LINKSTO Tls.Async,1 */
      /* Call Csm callback if the MacGenerate job is synchronous */
      Tls_MacGenerateCallback(ConnectionPtr, E_OK);
#endif
    }
  }

  if((ret == E_OK) && (ConnectionPtr->macGenerateState == TLS_MAC_STATE_PAYLOAD_READY))
  {
    /* Payload ready, start finish operation */
    ret = Csm_MacGenerate(ConnectionPtr->config->MACGenerateJobId, CRYPTO_OPERATIONMODE_FINISH,
                          &ConnectionPtr->transmitBuffer.buffer[headerSize], 0u,
                          &ConnectionPtr->encryptBuffer[headerSize + Tls_GetRecordLength(ConnectionPtr, ConnectionPtr->transmitBuffer.buffer)],
                          &ConnectionPtr->macGenerateLength
                         );
    if(ret == E_OK)
    {
      ConnectionPtr->macGenerateState = TLS_MAC_STATE_FINISH_BUSY;
#if TLS_CFG_CSM_PROCESSING == TLS_CSM_SYNCHRONOUS
      /* !LINKSTO Tls.Async,1 */
      /* Call Csm callback if the MacGenerate job is synchronous */
      Tls_MacGenerateCallback(ConnectionPtr, E_OK);
#endif
    }
  }

  if((ret == E_OK) && (ConnectionPtr->macGenerateState == TLS_MAC_STATE_READY))
  {
    TLS_PRINTF(("macGenerateState = TLS_MAC_STATE_IDLE\n"));
    /* CHECK: NOPARSE */
    if(ConnectionPtr->macGenerateLength != macLength)
    {
      /* should not happen */
      TLS_PRINTF(("Csm_MacGenerate returned wrong MAC length: %d\n", ConnectionPtr->macGenerateLength));
      TLS_UNREACHABLE_CODE_ASSERT(TLS_INTERNAL_SVCID);
      ret = E_NOT_OK;
    }
    else
    /* CHECK: PARSE */
    {
      /* Copy payload and header to encrypt buffer as there is no encryption */
      ConnectionPtr->encryptBufferLength = Tls_GetRecordLength(ConnectionPtr, ConnectionPtr->transmitBuffer.buffer) + headerSize + ConnectionPtr->cipherParams->MACLength;
      (void)Tls_Buffer_Copy(&ConnectionPtr->transmitBuffer, ConnectionPtr->encryptBuffer, 0u, ConnectionPtr->encryptBufferLength - ConnectionPtr->cipherParams->MACLength);
      Tls_SetRecordLength(ConnectionPtr, ConnectionPtr->encryptBuffer, ConnectionPtr->encryptBufferLength - headerSize);

      /* MAC complete, send record */
      TLS_PRINTF(("MAC ready, send record\n"));
      ret = Tls_TcpIp_Transmit(ConnectionPtr, ConnectionPtr->encryptBufferLength);
      ConnectionPtr->encryptBufferLength = 0u;
      if(ret == E_OK)
      {
        (void)Tls_ConsumeRecordFromTransmitBuffer(ConnectionPtr);
        /* !LINKSTO Dtls.RFC.Record.SequenceNumber,1 */
        ConnectionPtr->writeSeqNum++;
        /* Only set IDLE state if transmit was successfull. Otherwise we will try again. */
        ConnectionPtr->macGenerateState = TLS_MAC_STATE_IDLE;
      }
    }
  }

  if(ret != E_OK)
  {
    ConnectionPtr->macGenerateState = TLS_MAC_STATE_ERROR;
    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
  }
}

STATIC FUNC(void, TLS_CODE) Tls_ProcessEncryption(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret = E_OK;

  /* Encryption */
  if(ConnectionPtr->encryptState == TLS_ENCRYPT_STATE_READY)
  {
    /* Record was encrypted. */
    (void)Tls_ConsumeRecordFromTransmitBuffer(ConnectionPtr);
    ConnectionPtr->encryptState = TLS_ENCRYPT_STATE_IDLE;
  }

  if(Tls_CheckTransmitBuffer(ConnectionPtr) == E_OK)
  {
    if(ConnectionPtr->cipherParams->cipherType == TLS_AEAD_CIPHER)
    {
      if((ConnectionPtr->encryptState != TLS_ENCRYPT_STATE_BUSY) &&
        (ConnectionPtr->encryptBufferLength == 0u) &&
        (ConnectionPtr->cipher != 0u)
        )
      {
        /* Start encryption */
        ConnectionPtr->encryptState = TLS_ENCRYPT_STATE_BUSY;
        /* !LINKSTO Tls.API.Tls_TcpTransmit.Cipher,1 */
        ret = Tls_Encrypt(ConnectionPtr);
        if(ret == E_OK)
        {
#if TLS_CFG_CSM_PROCESSING == TLS_CSM_SYNCHRONOUS
          /* !LINKSTO Tls.Async,1 */
          Tls_EncryptCallback(ConnectionPtr, E_OK);
#endif
        }
        else if(ret == CRYPTO_E_BUSY)
        {
          ConnectionPtr->encryptState = TLS_ENCRYPT_STATE_START;
        }
        else
        {
          ConnectionPtr->encryptState = TLS_ENCRYPT_STATE_ERROR;
          /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
          Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
        }
      }
    }
  }
}

STATIC FUNC(void, TLS_CODE) Tls_RandomCallback(Tls_PtrConnection ConnectionPtr, Std_ReturnType result)
{
  Tls_PtrHandshakeData data = &ConnectionPtr->handshakeData;

  if(data->dtls_cookieState == TLS_RANDOM_STATE_BUSY)
  {
    /* Cookie was generated */
    if((result == E_OK) && (data->randomSize == TLS_RANDOM_SIZE))
    {
      /* Random values where properly generated */
      TLS_PRINTF(("Cookie ready.\n"));
      data->dtls_cookieState = TLS_RANDOM_STATE_READY;
      data->dtls_cookieLength = (uint8)data->randomSize;
    }
    else
    {
      /* Something went wrong */
      TLS_PRINTF(("Cookie fail.\n"));
      data->dtls_cookieState = TLS_RANDOM_STATE_ERROR;
      /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
      Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
    }
  }
  else
  {
    /* Client/Server random was generated */
    if((result == E_OK) && (data->randomSize == TLS_RANDOM_SIZE))
    {
      /* Random values where properly generated */
      TLS_PRINTF(("Random ready.\n"));
      data->randomState = TLS_RANDOM_STATE_READY;
    }
    else
    {
      /* Something went wrong */
      TLS_PRINTF(("Random fail.\n"));
      data->randomState = TLS_RANDOM_STATE_ERROR;
      /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
      Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
    }
  }
}

STATIC FUNC(void, TLS_CODE) Tls_ProcessRandom(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret = E_OK;
  Tls_PtrHandshakeData data = &ConnectionPtr->handshakeData;

  if(data->randomState == TLS_RANDOM_STATE_START)
  {
    if(ConnectionPtr->config->entity == TLS_CLIENT)
    {
      ret = Tls_RandomGenerate(ConnectionPtr, data->clientRandom);
    }
    else
    {
      ret = Tls_RandomGenerate(ConnectionPtr, data->serverRandom);
    }
    if(ret == E_OK)
    {
      data->randomState = TLS_RANDOM_STATE_BUSY;
#if TLS_CFG_CSM_PROCESSING == TLS_CSM_SYNCHRONOUS
      /* !LINKSTO Tls.Async,1 */
      Tls_RandomCallback(ConnectionPtr, E_OK);
#endif
    }
    else
    {
      data->randomState = TLS_RANDOM_STATE_ERROR;
      /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
      Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
    }
  }
}

/* !LINKSTO Dtls.Handshake.CookieGeneration,1 */
STATIC FUNC(void, TLS_CODE) Tls_Dtls_ProcessCookie(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret = E_OK;
  Tls_PtrHandshakeData data = &ConnectionPtr->handshakeData;

  if(data->dtls_cookieState == TLS_RANDOM_STATE_START)
  {
    ret = Tls_Dtls_GenerateCookie(ConnectionPtr);
  }

  if(data->dtls_cookieState == TLS_RANDOM_STATE_READY)
  {
    /* Send HelloVerifyRequest */
    Tls_Dtls_Handshake_WriteHelloVerifyRequest(ConnectionPtr);

    ret = Tls_TransmitRecord(ConnectionPtr);
    if(ret == E_OK)
    {
      TLS_PRINTF(("cookieState = TLS_RANDOM_STATE_IDLE\n"));
      data->dtls_cookieState = TLS_RANDOM_STATE_IDLE;
    }
  }

  if(ret != E_OK)
  {
    data->dtls_cookieState = TLS_RANDOM_STATE_ERROR;
    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
  }
}

/* !LINKSTO Tls.RFC.Record.MAC,1 */
STATIC FUNC(void, TLS_CODE) Tls_ProcessMacVerification(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret = E_OK;
  uint8 headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);
  uint16 recordLength = Tls_GetRecordLength(ConnectionPtr, ConnectionPtr->receiveBuffer.buffer);
  uint8 macLength = ConnectionPtr->cipherParams->MACLength;

  if(ConnectionPtr->macVerifyState == TLS_MAC_STATE_START)
  {
    /* Check length of the record. Must include the MAC. */
    if(recordLength < (uint16)macLength)
    {
      TLS_PRINTF(("Record too short for MAC verification\n"));
      /* !LINKSTO Tls.RFC.Alert.DecodeError,1 */
      Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_DECODE_ERROR);
      ret = E_NOT_OK;
    }
    else
    {
      Tls_MacMakeHeader(ConnectionPtr->macVerifyHeader,
          (uint32)recordLength - (uint32)macLength,
          Tls_GetRecordType(ConnectionPtr->receiveBuffer.buffer), Tls_GetRecordVersion(ConnectionPtr->receiveBuffer.buffer),
          ConnectionPtr->config->dtls ? Tls_Dtls_GetEpochAndSeqNum(ConnectionPtr->receiveBuffer.buffer) : ConnectionPtr->readSeqNum);

      ret = Csm_MacVerify(ConnectionPtr->config->MACVerifyJobId, CRYPTO_OPERATIONMODE_STREAMSTART,
          ConnectionPtr->macVerifyHeader, TLS_MAC_HEADER_SIZE,
          &ConnectionPtr->receiveBuffer.buffer[((uint16)headerSize + recordLength) - macLength], macLength,
          /*NULL_PTR*/&ConnectionPtr->decryptVerify);
    }
    if(ret == E_OK)
    {
      ConnectionPtr->macVerifyState = TLS_MAC_STATE_HEADER_BUSY;
      TLS_PRINTF(("macVerifyState = TLS_MAC_STATE_HEADER_BUSY\n"));
#if TLS_CFG_CSM_PROCESSING == TLS_CSM_SYNCHRONOUS
      /* !LINKSTO Tls.Async,1 */
      /* Call Csm callback if the MacVerify job is synchronous */
      Tls_MacVerifyCallback(ConnectionPtr, E_OK);
#endif
    }
  }
  if((ret == E_OK) && (ConnectionPtr->macVerifyState == TLS_MAC_STATE_HEADER_READY))
  {
    ret = Csm_MacVerify(ConnectionPtr->config->MACVerifyJobId, CRYPTO_OPERATIONMODE_UPDATE,
                        &ConnectionPtr->receiveBuffer.buffer[headerSize],
                        (uint32)recordLength - (uint32)macLength,
                        &ConnectionPtr->receiveBuffer.buffer[((uint16)headerSize + recordLength) - macLength],
                        macLength,
                        &ConnectionPtr->decryptVerify
                       );

    if(ret == E_OK)
    {
      ConnectionPtr->macVerifyState = TLS_MAC_STATE_PAYLOAD_BUSY;
      TLS_PRINTF(("macVerifyState = TLS_MAC_STATE_PAYLOAD_BUSY\n"));
#if TLS_CFG_CSM_PROCESSING == TLS_CSM_SYNCHRONOUS
      /* !LINKSTO Tls.Async,1 */
      /* Call Csm callback if the MacVerify job is synchronous */
      Tls_MacVerifyCallback(ConnectionPtr, E_OK);
#endif
    }
  }
  if((ret == E_OK) && (ConnectionPtr->macVerifyState == TLS_MAC_STATE_PAYLOAD_READY))
  {
    /* Payload ready, start finish operation */
    ret = Csm_MacVerify(ConnectionPtr->config->MACVerifyJobId, CRYPTO_OPERATIONMODE_FINISH,
                        &ConnectionPtr->receiveBuffer.buffer[headerSize], 0u,
                        &ConnectionPtr->receiveBuffer.buffer[((uint16)headerSize + recordLength) - macLength], macLength,
                        &ConnectionPtr->decryptVerify
                       );
    if(ret == E_OK)
    {
      ConnectionPtr->macVerifyState = TLS_MAC_STATE_FINISH_BUSY;
      TLS_PRINTF(("macVerifyState = TLS_MAC_STATE_FINISH_BUSY\n"));
#if TLS_CFG_CSM_PROCESSING == TLS_CSM_SYNCHRONOUS
      /* !LINKSTO Tls.Async,1 */
      /* Call Csm callback if the MacVerify job is synchronous */
      Tls_MacVerifyCallback(ConnectionPtr, E_OK);
#endif
    }
  }

  if((ret == E_OK) && (ConnectionPtr->macVerifyState == TLS_MAC_STATE_READY))
  {
    ConnectionPtr->macVerifyState = TLS_MAC_STATE_IDLE;
    TLS_PRINTF(("macVerifyState = TLS_MAC_STATE_IDLE\n"));
    /* MAC verification ready */
    if(ConnectionPtr->decryptVerify == CRYPTO_E_VER_OK)
    {
      /* verification OK */
      TLS_PRINTF(("MAC verify OK.\n"));

      (void)Tls_Buffer_Copy(&ConnectionPtr->receiveBuffer, ConnectionPtr->decryptBuffer.buffer, 0u, ((uint16)headerSize + recordLength) - macLength);
      Tls_SetRecordLength(ConnectionPtr, ConnectionPtr->decryptBuffer.buffer, recordLength - macLength);
      ConnectionPtr->decryptBuffer.count = ((uint16)headerSize + recordLength) - macLength;
      Tls_RecordDecrypted(ConnectionPtr);
      (void)Tls_ConsumeRecordFromReceiveBuffer(ConnectionPtr);
    }
    else
    {
      /* verification failed */
      TLS_PRINTF(("MAC verify failed.\n"));
      if(ConnectionPtr->config->dtls == TRUE)
      {
        /* !LINKSTO Dtls.RFC.Record.Check,1 */
        /* Silently discard */
      }
      else
      {
        /* !LINKSTO Tls.RFC.Alert.BadRecordMAC,1 */
        Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_BAD_RECORD_MAC);
      }
      (void)Tls_ConsumeRecordFromReceiveBuffer(ConnectionPtr);
    }
  }

  if(ret != E_OK)
  {
    TLS_PRINTF(("Csm_MacVerify fail: %d\n", ret));
    /* An error occured, remove record from receive buffer. */
    (void)Tls_ConsumeRecordFromReceiveBuffer(ConnectionPtr);
    ConnectionPtr->macVerifyState = TLS_MAC_STATE_ERROR;
    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
  }
}

STATIC FUNC(void, TLS_CODE) Tls_ProcessDecryption(Tls_PtrConnection ConnectionPtr)
{
  if(ConnectionPtr->decryptState == TLS_DECRYPT_STATE_READY)
  {
    /* Record was decrypted. */
    Tls_RecordDecrypted(ConnectionPtr);
    (void)Tls_ConsumeRecordFromReceiveBuffer(ConnectionPtr);
    ConnectionPtr->decryptState = TLS_DECRYPT_STATE_IDLE;
  }

  if((ConnectionPtr->decryptState == TLS_DECRYPT_STATE_START) &&
     /* TODO: check if it is sufficient to check if != BUSY */
     ((ConnectionPtr->handshakeData.clientVerifyDataState == TLS_VERIFYDATA_STATE_IDLE) || (ConnectionPtr->handshakeData.clientVerifyDataState == TLS_VERIFYDATA_STATE_READY) || (ConnectionPtr->handshakeData.clientVerifyDataState == TLS_VERIFYDATA_STATE_ERROR)) &&
     ((ConnectionPtr->handshakeData.serverVerifyDataState == TLS_VERIFYDATA_STATE_IDLE) || (ConnectionPtr->handshakeData.serverVerifyDataState == TLS_VERIFYDATA_STATE_READY) || (ConnectionPtr->handshakeData.serverVerifyDataState == TLS_VERIFYDATA_STATE_ERROR))
    )
  {
    /* Start decryption */
    ConnectionPtr->decryptState = TLS_DECRYPT_STATE_BUSY;
    if(Tls_Decrypt(ConnectionPtr) == E_OK)
    {
#if TLS_CFG_CSM_PROCESSING == TLS_CSM_SYNCHRONOUS
      /* !LINKSTO Tls.Async,1 */
      Tls_DecryptCallback(ConnectionPtr, E_OK);
#endif
    }
    else
    {
      ConnectionPtr->decryptState = TLS_DECRYPT_STATE_ERROR;
      /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
      Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
    }
  }
}

STATIC FUNC(void, TLS_CODE) Tls_UL_Accepted(Tls_PtrConnection ConnectionPtr)
{
  /* Deviation MISRAC2012-1 */
  Tls_UL_TcpAccepted(ConnectionPtr, ConnectionPtr->listenSocket, ConnectionPtr->socket, (TcpIp_ConstSockAddrPtrType)&ConnectionPtr->remoteAddr);
}

STATIC FUNC(void, TLS_CODE) Tls_Dtls_ProcessTimer(Tls_PtrConnection ConnectionPtr)
{
  /* !LINKSTO Dtls.RFC.Handshake.RetransmitTimer,1 */
  /* Update DTLS retransmit timer */
  if(ConnectionPtr->handshakeData.dtls_retransmitTimer > 0u)
  {
    /* Do not update the counter if crypto operations are busy as there are some cases
     * where it can take some time for messages to be sent/received:
     * - Tls starts the timer and flushes the retransmit buffer when sending the ClienKeyExchange message.
     *   But the Finished message of the same DTLS flight has to wait for generation of VerifyData and encryption,
     * - If the encrypted Finished message is received, it may take some time to decode it. During this time the timer does not get updated.
     */
    if(((ConnectionPtr->handshakeData.keyDeriveState == TLS_KEYDERIVE_STATE_IDLE) || (ConnectionPtr->handshakeData.keyDeriveState == TLS_KEYDERIVE_STATE_READY)) &&
        ((ConnectionPtr->handshakeData.clientVerifyDataState == TLS_VERIFYDATA_STATE_IDLE) || (ConnectionPtr->handshakeData.clientVerifyDataState == TLS_VERIFYDATA_STATE_READY)) &&
        ((ConnectionPtr->macGenerateState == TLS_MAC_STATE_IDLE) || (ConnectionPtr->macGenerateState == TLS_MAC_STATE_READY)) &&
        ((ConnectionPtr->encryptState == TLS_ENCRYPT_STATE_IDLE) || (ConnectionPtr->encryptState == TLS_ENCRYPT_STATE_READY)) &&
        ((ConnectionPtr->macVerifyState == TLS_MAC_STATE_IDLE)   || (ConnectionPtr->macVerifyState == TLS_MAC_STATE_READY))   &&
        ((ConnectionPtr->decryptState == TLS_DECRYPT_STATE_IDLE) || (ConnectionPtr->decryptState == TLS_DECRYPT_STATE_READY)) &&
        ((ConnectionPtr->handshakeData.verifyCertState == TLS_VERIFY_CERT_STATE_IDLE) || (ConnectionPtr->handshakeData.verifyCertState == TLS_VERIFY_CERT_STATE_READY)) &&
        ((ConnectionPtr->handshakeData.sigGenState == TLS_SIG_GEN_STATE_IDLE) || (ConnectionPtr->handshakeData.sigGenState == TLS_SIG_GEN_STATE_READY)) &&
        ((ConnectionPtr->handshakeData.sigVerState == TLS_SIG_VER_STATE_IDLE) || (ConnectionPtr->handshakeData.sigVerState == TLS_SIG_VER_STATE_READY)) &&
        ((ConnectionPtr->handshakeData.keCalcPubValState == TLS_KE_CALC_PUB_VAL_STATE_IDLE) || (ConnectionPtr->handshakeData.keCalcPubValState == TLS_KE_CALC_PUB_VAL_STATE_READY)) &&
        ((ConnectionPtr->handshakeData.keCalcSecretState == TLS_KE_CALC_SECRET_STATE_IDLE) || (ConnectionPtr->handshakeData.keCalcSecretState == TLS_KE_CALC_SECRET_STATE_READY))
        )
    {
      /* Update the timer */
      if(ConnectionPtr->handshakeData.dtls_retransmitTimer == 1u)
      {
        Tls_Dtls_Handshake_RetransmitTimerExpired(ConnectionPtr);
      }
      ConnectionPtr->handshakeData.dtls_retransmitTimer--;
    }
  }
}

#if TLS_CERTIFICATE_SUPPORT == STD_ON
STATIC FUNC(Std_ReturnType, TLS_CODE) Tls_SignatureGenerate(Tls_PtrConnection ConnectionPtr, uint8 *sigInPtr , uint32 sigInLen, Crypto_OperationModeType mode, Tls_SigGenStateType sigGenState)
{
  /* ServerHello.Random */
  uint32 sigJobId = ConnectionPtr->config->localCertSigGenerateJobId;
  uint8 *sigOutPtr = ConnectionPtr->handshakeData.sigGenOutputDataPtr;
  uint32 *sigOutLenPtr = &(ConnectionPtr->handshakeData.sigGenOutputLength);
  Std_ReturnType status = Csm_SignatureGenerate(sigJobId, mode, sigInPtr, sigInLen, sigOutPtr, sigOutLenPtr);

  if(status == E_OK)
  {
    ConnectionPtr->handshakeData.sigGenState = sigGenState;
#if TLS_CFG_CSM_PROCESSING == TLS_CSM_SYNCHRONOUS
    /* !LINKSTO Tls.Async,1 */
    Tls_SignatureGenerateCallback(ConnectionPtr, status);
#endif
  }
  return status;
}
#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */

STATIC FUNC(void, TLS_CODE) Tls_ProcessServerState(Tls_PtrConnection ConnectionPtr)
{
  Std_ReturnType ret = E_OK;

#if TLS_CERTIFICATE_SUPPORT == STD_ON
  if(ConnectionPtr->state == TLS_STATE_SERVERCERTIFICATE_DONE)
  {
    Std_ReturnType status = E_OK;

    TLS_POSTCONDITION_ASSERT(TLS_CIPHER_USES_CERTS(ConnectionPtr->cipher), TLS_MAINFUNCTION_SVCID);
    /* TODO: This should probably not be in ProcessServerState but in its own function, ProcessSignatureGenerateState */

    if
    (
      (ConnectionPtr->handshakeData.sigGenState == TLS_SIG_GEN_STATE_READY) &&
      (ConnectionPtr->handshakeData.keCalcPubValState == TLS_KE_CALC_PUB_VAL_STATE_READY)
    )
    {
      /* Append handshake for verify_data */
      uint32 size = Tls_Handshake_GetSizeEcdhServerKeyExchange() + Tls_Handshake_GetHeaderSize(ConnectionPtr);
      Tls_Handshake_Append(ConnectionPtr,
          &ConnectionPtr->transmitBuffer.buffer[(uint32)ConnectionPtr->transmitBuffer.count - size], size);

      /* Transmit record */
      ret = Tls_TransmitRecord(ConnectionPtr);
      if(ret == E_OK)
      {
        /* LINKSTO Dtls.RFC.Handshake.MessageSeq.Increment,1 */
        ConnectionPtr->handshakeData.dtls_handshakeSeq++;
        Tls_SetState(ConnectionPtr, TLS_STATE_SERVERKEYEXCHANGE_DONE);

        if(ret == E_OK)
        {
          ret = Tls_TransmitCertificateRequest(ConnectionPtr);

          if(ret == E_OK)
          {
            ret = Tls_TransmitServerHelloDone(ConnectionPtr);
          }
        }
      }

      /* TODO: handle failures? */

      /* Mark server random as used */
      ConnectionPtr->handshakeData.randomState = TLS_RANDOM_STATE_IDLE;
      ConnectionPtr->handshakeData.sigGenState = TLS_SIG_GEN_STATE_IDLE;
      ConnectionPtr->handshakeData.keCalcPubValState = TLS_KE_CALC_PUB_VAL_STATE_IDLE;
    }
    else
    {
      switch(ConnectionPtr->handshakeData.sigGenState)
      {
        case TLS_SIG_GEN_STATE_CLIENT_RANDOM_READY:
        {
          /* ServerHello.Random */
          status = Tls_SignatureGenerate(ConnectionPtr, ConnectionPtr->handshakeData.serverRandom, TLS_RANDOM_SIZE, CRYPTO_OPERATIONMODE_UPDATE, TLS_SIG_GEN_STATE_SERVER_RANDOM_BUSY);
          break;
        }
        case TLS_SIG_GEN_STATE_SERVER_RANDOM_READY:
        {
          /* ServerKeyExchange.params */
          status = Tls_SignatureGenerate(ConnectionPtr, ConnectionPtr->handshakeData.sigGenInputDataPtr, ConnectionPtr->handshakeData.sigGenInputLength, CRYPTO_OPERATIONMODE_UPDATE, TLS_SIG_GEN_STATE_SKEX_PARAMS_BUSY);
          break;
        }
        case TLS_SIG_GEN_STATE_SKEX_PARAMS_READY:
        {
          /* Finish */
          status = Tls_SignatureGenerate(ConnectionPtr, ConnectionPtr->handshakeData.sigGenInputDataPtr, 0U, CRYPTO_OPERATIONMODE_FINISH, TLS_SIG_GEN_STATE_FINISH_BUSY);
          break;
        }
        default:
        {
          /* Do nothing */
          break;
        }
      }
    }

    /* Handle failed Csm_SigautureGenerate calls */
    if(status != E_OK)
    {
      ConnectionPtr->handshakeData.sigGenState = TLS_SIG_GEN_STATE_ERROR;
      /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
      Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
    }
  }

  if(canReceiveClientKeyExchange(ConnectionPtr) &&
      (ConnectionPtr->handshakeData.keCalcSecretState == TLS_KE_CALC_SECRET_STATE_READY))
  {
    /* Copy the shared secret to the PMS key */
    ret = Csm_KeyElementCopy
    (
      ConnectionPtr->config->keyExchangeEphemeralExchangeKeyId,
      CRYPTO_KE_KEYEXCHANGE_SHAREDVALUE,
      ConnectionPtr->config->PreMasterSecretKeyId,
      CRYPTO_KE_MAC_KEY
    );

    if(ret == E_OK)
    {
      Tls_SetState(ConnectionPtr, TLS_STATE_CLIENTKEYEXCHANGE_DONE);
      ConnectionPtr->handshakeData.keCalcSecretState = TLS_KE_CALC_SECRET_STATE_IDLE;
    }
  }

  if((ConnectionPtr->state == TLS_STATE_CLIENTKEYEXCHANGE_DONE) &&
      (ConnectionPtr->handshakeData.sigVerState == TLS_SIG_VER_STATE_READY))
  {
    TLS_POSTCONDITION_ASSERT(TLS_CIPHER_USES_CERTS(ConnectionPtr->cipher), TLS_MAINFUNCTION_SVCID);
    TLS_POSTCONDITION_ASSERT(ConnectionPtr->config->remoteCertChainIdPtr != NULL_PTR, TLS_MAINFUNCTION_SVCID);

    ConnectionPtr->handshakeData.sigVerState = TLS_SIG_VER_STATE_IDLE;
    Tls_SetState(ConnectionPtr, TLS_STATE_CERTIFICATE_VERIFY_DONE);
  }

  if((ConnectionPtr->state == TLS_STATE_SERVERHELLODONE_DONE) && (ConnectionPtr->handshakeData.verifyCertState == TLS_VERIFY_CERT_STATE_READY))
  {
    TLS_POSTCONDITION_ASSERT(TLS_CIPHER_USES_CERTS(ConnectionPtr->cipher), TLS_MAINFUNCTION_SVCID);
    TLS_POSTCONDITION_ASSERT(ConnectionPtr->config->remoteCertChainIdPtr != NULL_PTR, TLS_MAINFUNCTION_SVCID);

    ConnectionPtr->handshakeData.verifyCertState = TLS_VERIFY_CERT_STATE_IDLE;
    Tls_SetState(ConnectionPtr, TLS_STATE_CLIENTCERTIFICATE_DONE);
  }
#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */

  /* TODO: check if necessary, should be handled by Crypto */
  if(( (ConnectionPtr->state == TLS_STATE_CHANGECIPHERSPEC_RECEIVED) || (ConnectionPtr->state == TLS_STATE_FINISHED_RECEIVED)) && (ConnectionPtr->handshakeData.keyDeriveState == TLS_KEYDERIVE_STATE_READY) &&
     ((ConnectionPtr->macVerifyState == TLS_MAC_STATE_IDLE) || (ConnectionPtr->macVerifyState == TLS_MAC_STATE_READY))                           &&
      ((ConnectionPtr->macGenerateState == TLS_MAC_STATE_IDLE) || (ConnectionPtr->macGenerateState == TLS_MAC_STATE_READY))
    )
  {
    Tls_Handshake_GenerateClientVerifyDataStart(ConnectionPtr);
  }

  if((ConnectionPtr->state == TLS_STATE_CHANGECIPHERSPEC_SENT) && (ConnectionPtr->handshakeData.serverVerifyDataState == TLS_VERIFYDATA_STATE_READY))
  {
    Tls_Handshake_WriteFinished(ConnectionPtr);

    /* This wont fail because data must be encrypted first */
    (void)Tls_TransmitRecord(ConnectionPtr);

    Tls_SetState(ConnectionPtr, TLS_STATE_FINISHED_SENT);

    if(ConnectionPtr->config->dtls == TRUE)
    {
      /* LINKSTO Dtls.RFC.Handshake.MessageSeq.Increment,1 */
      ConnectionPtr->handshakeData.dtls_handshakeSeq++;
    }

    Tls_SetState(ConnectionPtr, TLS_STATE_OK);

    if( ConnectionPtr->config->dtls == FALSE )
    {
      Tls_UL_Accepted(ConnectionPtr);
    }
  }

  if((ConnectionPtr->state == TLS_STATE_FINISHED_RECEIVED) && (ConnectionPtr->handshakeData.clientVerifyDataState == TLS_VERIFYDATA_STATE_READY))
  {
    ConnectionPtr->handshakeData.clientVerifyDataState = TLS_VERIFYDATA_STATE_IDLE;
    if (Tls_Handshake_CheckVerifyData(ConnectionPtr) == E_OK)
    {
      /* Send ChangeCipherSpec */
      Tls_ChangeCipherSpec_Write(ConnectionPtr);
      ret = Tls_TransmitRecord(ConnectionPtr);
      if(ret == E_OK)
      {
        Tls_OnSendChangeCipherSpec(ConnectionPtr);

        Tls_SetState(ConnectionPtr, TLS_STATE_CHANGECIPHERSPEC_SENT);
        Tls_Handshake_GenerateServerVerifyDataStart(ConnectionPtr);
      }
    }
    else
    {
      /* !LINKSTO Tls.RFC.Alert.DecryptError,1 */
      Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_DECRYPT_ERROR);
    }
  }

  if(ret != E_OK)
  {
    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
  }
}

STATIC FUNC(void, TLS_CODE) Tls_ProcessClientState(Tls_PtrConnection ConnectionPtr)
{
  uint8 size;

  if((ConnectionPtr->state == TLS_STATE_CHANGECIPHERSPEC_SENT) && (ConnectionPtr->handshakeData.clientVerifyDataState == TLS_VERIFYDATA_STATE_READY))
  {
    /* Write finished message */
    Tls_Handshake_WriteFinished(ConnectionPtr);

    /* Copy handshake message to the handshake buffer for generation of VerifyData */
    size = TLS_VERIFY_DATA_LENGTH + Tls_Handshake_GetHeaderSize(ConnectionPtr);
    Tls_Handshake_Append(ConnectionPtr,
                         &ConnectionPtr->transmitBuffer.buffer[ConnectionPtr->transmitBuffer.count - size],
                         size
                        );

    /* This wont fail because data must be encrypted first */
    (void)Tls_TransmitRecord(ConnectionPtr);

    if(ConnectionPtr->config->dtls == TRUE)
    {
      /* LINKSTO Dtls.RFC.Handshake.MessageSeq.Increment,1 */
      ConnectionPtr->handshakeData.dtls_handshakeSeq++;
    }

    Tls_SetState(ConnectionPtr, TLS_STATE_FINISHED_SENT);
  }

  if((ConnectionPtr->state == TLS_STATE_CHANGECIPHERSPEC_SENT) && (ConnectionPtr->handshakeData.keyDeriveState == TLS_KEYDERIVE_STATE_READY))
  {
    ConnectionPtr->handshakeData.keyDeriveState = TLS_KEYDERIVE_STATE_IDLE;

    Tls_Handshake_GenerateClientVerifyDataStart(ConnectionPtr);
  }

  if((ConnectionPtr->state == TLS_STATE_FINISHED_RECEIVED) && (ConnectionPtr->handshakeData.serverVerifyDataState == TLS_VERIFYDATA_STATE_READY))
  {
    if (Tls_Handshake_CheckVerifyData(ConnectionPtr) == E_OK)
    {
      Tls_SetState(ConnectionPtr, TLS_STATE_OK);
      if( ConnectionPtr->config->dtls == FALSE )
      {
        Tls_Connected(ConnectionPtr);
      }
    }
    else
    {
      /* !LINKSTO Tls.RFC.Alert.DecryptError,1 */
      Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_DECRYPT_ERROR);
    }
  }

  if(ConnectionPtr->state == TLS_STATE_START)
  {
    Tls_Handshake_Start(ConnectionPtr);
  }
}

STATIC FUNC(void, TLS_CODE) Tls_ProcessState(Tls_PtrConnection ConnectionPtr)
{
  if(ConnectionPtr->closeRequest == TRUE)
  {
    TLS_PRINTF(("Close requested\n"));
    /* Just call Tls_Alert_Transmit. It will check if the connection is in the right state. */
    /* !LINKSTO Tls.RFC.Alert.CloseNotify,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_CLOSE_NOTIFY);
  }

  if(ConnectionPtr->transmissionError == TRUE)
  {
    /* Transmission failed because of TcpIp error. Set error flag and go to CLOSING state */
    /* !LINKSTO Tls.RFC.Alert.InternalError,1 */
    Tls_Alert_Transmit(ConnectionPtr, TLS_ALERT_LEVEL_FATAL, TLS_ALERT_DESC_INTERNAL_ERROR);
  }

  if(ConnectionPtr->state == TLS_STATE_CLOSING)
  {
    /* If all data was transmitted and all Csm jobs are stopped(ERROR, IDLE, READY), go to CLOSED state */
    TLS_PRINTF(("Waiting in CLOSING state:\n"));
    TLS_PRINTF(("transmitBuffer.count = %d\n", ConnectionPtr->transmitBuffer.count));
    TLS_PRINTF(("decryptState = %d\n", ConnectionPtr->decryptState));
    TLS_PRINTF(("encryptState = %d\n", ConnectionPtr->encryptState));
    TLS_PRINTF(("macGenerateState = %d\n", ConnectionPtr->macGenerateState));
    TLS_PRINTF(("macVerifyState = %d\n", ConnectionPtr->macVerifyState));
    TLS_PRINTF(("handshakeData.randomState = %d\n", ConnectionPtr->handshakeData.randomState));
    TLS_PRINTF(("transmissionError = %d\n", ConnectionPtr->transmissionError));

    if
    (
        /* Wait for transmit buffer empty. If encrypt, transmission or MAC generate failed, sending is not possible anymore. */
        ((ConnectionPtr->transmitBuffer.count == 0) || (ConnectionPtr->encryptState == TLS_ENCRYPT_STATE_ERROR) ||
            (ConnectionPtr->macGenerateState == TLS_MAC_STATE_ERROR) || (ConnectionPtr->transmissionError == TRUE))
        &&
        /* Wait for all decrypt callbacks */
        ((ConnectionPtr->decryptState == TLS_DECRYPT_STATE_IDLE) || (ConnectionPtr->decryptState == TLS_DECRYPT_STATE_ERROR) ||
            (ConnectionPtr->decryptState == TLS_DECRYPT_STATE_READY))
        &&
        /* Wait for all encrypt callbacks */
        ((ConnectionPtr->encryptState == TLS_ENCRYPT_STATE_IDLE) || (ConnectionPtr->encryptState == TLS_ENCRYPT_STATE_ERROR) ||
            (ConnectionPtr->encryptState == TLS_ENCRYPT_STATE_READY))
        &&
        /* Wait for all mac generate callbacks */
        ((ConnectionPtr->macGenerateState == TLS_MAC_STATE_IDLE) || (ConnectionPtr->macGenerateState == TLS_MAC_STATE_ERROR) ||
            (ConnectionPtr->macGenerateState == TLS_MAC_STATE_READY))
        &&
        /* Wait for all mac verify callbacks */
        ((ConnectionPtr->macVerifyState == TLS_MAC_STATE_IDLE) || (ConnectionPtr->macVerifyState == TLS_MAC_STATE_ERROR) ||
            (ConnectionPtr->macVerifyState == TLS_MAC_STATE_READY))
        &&
        /* Wait for all random callbacks */
        ((ConnectionPtr->handshakeData.randomState == TLS_RANDOM_STATE_IDLE) || (ConnectionPtr->handshakeData.randomState == TLS_RANDOM_STATE_ERROR) || (ConnectionPtr->handshakeData.randomState == TLS_RANDOM_STATE_READY))
    )
    {
      if(ConnectionPtr->errorFlag == TRUE)
      {
        Tls_SetState(ConnectionPtr, TLS_STATE_ERROR);
      }
      else
      {
        Tls_SetState(ConnectionPtr, TLS_STATE_CLOSED);
      }

      /* Notify upper layer */
      Tls_UL_Closed(ConnectionPtr);

      /* Close TcpIp socket */
      TcpIp_TlsClose(ConnectionPtr->socket);

      /* Mark as inactive. Connection can be reused. */
      ConnectionPtr->active = FALSE;
    }
  }
}

STATIC FUNC(void, TLS_CODE) Tls_ConnectionMainFunction(Tls_PtrConnection ConnectionPtr)
{
  if(ConnectionPtr->config->dtls)
  {
    /* Update DTLS retransmit timer */
    Tls_Dtls_ProcessTimer(ConnectionPtr);
    /* Process DTLS server cookie generation state */
    Tls_Dtls_ProcessCookie(ConnectionPtr);
  }

  if(ConnectionPtr->config->entity == TLS_SERVER)
  {
    /* Process server-only state */
    Tls_ProcessServerState(ConnectionPtr);
  }
  else
  {
    /* Process client-only state */
    Tls_ProcessClientState(ConnectionPtr);
  }

  /* Process client and server state */
  Tls_ProcessState(ConnectionPtr);

  /* Process key derivation state */
  Tls_KeyDeriveMainFunction(ConnectionPtr);

  /* Process verification data generation state */
  Tls_ProcessVerifyData(ConnectionPtr, &ConnectionPtr->handshakeData.serverVerifyDataState,
      ConnectionPtr->handshakeData.serverVerifyData, Tls_String_ServerFinished, TLS_SERVERFINISHED_STRING_LENGTH);
  Tls_ProcessVerifyData(ConnectionPtr, &ConnectionPtr->handshakeData.clientVerifyDataState,
      ConnectionPtr->handshakeData.clientVerifyData,Tls_String_ClientFinished, TLS_CLIENTFINISHED_STRING_LENGTH);

  /* Process decryption state */
  Tls_ProcessDecryption(ConnectionPtr);
  /* Process encryption state */
  Tls_ProcessEncryption(ConnectionPtr);

  /* Process MAC generation state for cipher suites with separate MAC */
  /* TODO: check if necessary, should be handled by Crypto */
  if( ((ConnectionPtr->handshakeData.serverVerifyDataState == TLS_VERIFYDATA_STATE_IDLE) ||
       (ConnectionPtr->handshakeData.serverVerifyDataState == TLS_VERIFYDATA_STATE_READY)) &&
       ((ConnectionPtr->handshakeData.clientVerifyDataState == TLS_VERIFYDATA_STATE_IDLE) ||
       (ConnectionPtr->handshakeData.clientVerifyDataState == TLS_VERIFYDATA_STATE_READY))
    )
  {
    Tls_ProcessMacGeneration(ConnectionPtr);
  }

  /* Process MAC verification state for cipher suites with separate MAC */
  Tls_ProcessMacVerification(ConnectionPtr);

  /* Process random state */
  Tls_ProcessRandom(ConnectionPtr);

  /* Process the pseudo random function state */
  Tls_PRF_MainFunction(ConnectionPtr);

  /* Handle the receive buffer */
  Tls_HandleReceiveBuffer(ConnectionPtr);
  /* Handle the transmit buffer */
  Tls_HandleTransmitBuffer(ConnectionPtr);
}

FUNC(void, TLS_CODE) Tls_MainFunction(void)
{
  uint8 i;

#if(TLS_DEV_ERROR_DETECT == STD_ON)
  if (Tls_Initialized == FALSE)
  {
    TLS_DET_REPORT_ERROR(TLS_MAINFUNCTION_SVCID, TLS_E_UNINIT);
  }
  else
#endif
  {
    /* Call main function for all active connections */
    for(i=0u;i<TLS_CFG_CONNECTION_COUNT;++i)
    {
      if((Tls_Connections[i].active == TRUE) && (Tls_Connections[i].state != TLS_STATE_ERROR))
      {
        Tls_ConnectionMainFunction(&Tls_Connections[i]);
      }
    }
  }
}

FUNC(void, TLS_CODE) Tls_Init(void)
{
  uint8 i;
#if(TLS_DEV_ERROR_DETECT == STD_ON)
  Tls_Initialized = TRUE;
#endif
  /* Initialize defaults for all connections */
  for(i=0u;i<TLS_CFG_CONNECTION_COUNT;++i)
  {
    Tls_SetDefaults(&Tls_Connections[i]);
  }
}

FUNC(Std_ReturnType, TLS_CODE) Tls_Close(TcpIp_SocketIdType SocketId, boolean Abort)
{
  Std_ReturnType ret = E_OK;
  Tls_PtrConnection ConnectionPtr = NULL_PTR;

#if(TLS_DEV_ERROR_DETECT == STD_ON)
  if (Tls_Initialized == FALSE)
  {
    /* !LINKSTO Tls.API.Tls_Close.Det.NoInit,1 */
    TLS_DET_REPORT_ERROR(TLS_CLOSE_SVCID, TLS_E_UNINIT);
    ret = E_NOT_OK;
  }
  else
#endif
  {
    ConnectionPtr = Tls_FindConnection(SocketId);

    if (ConnectionPtr != NULL_PTR)
    {
      if(Abort == TRUE)
      {
        Tls_SetState(ConnectionPtr, TLS_STATE_CLOSED);

        if(ConnectionPtr->config->dtls == FALSE)
        {
          /* Notify upper layer of TcpIp that the tcp connection is reset */
          Tls_UL_TcpIpEvent(ConnectionPtr, TCPIP_TCP_RESET);
        }
        else
        {
          /* Notify upper layer of TcpIp that the udp connection is closed */
          Tls_UL_TcpIpEvent(ConnectionPtr, TCPIP_UDP_CLOSED);
        }
        /* Mark as inactive. Connection can be reused. */
        ConnectionPtr->active = FALSE;
      }
      else
      {
        Tls_CloseInternal(ConnectionPtr);
      }
    }
    else
    {
      ret = E_NOT_OK;
    }
  }
  return ret;
}

#if( TLS_CSM_KEYDERIVE_SUPPORT == STD_ON )
STATIC FUNC(uint8, TLS_CODE) Tls_GetCsmKeyDeriveAlgorithm(uint16 ciphersuite)
{
  uint8 algorithm = TLS_CSM_KEYDERIVE_ALGO_INVALID;

  switch(ciphersuite)
  {
    case TLS_PSK_WITH_AES_128_GCM_SHA256:
    {
      algorithm = TLS_CSM_KEYDERIVE_ALGO_AES_128_GCM_SHA256;
    }
    break;

    case TLS_PSK_WITH_NULL_SHA256:
    {
      algorithm = TLS_CSM_KEYDERIVE_ALGO_NULL_SHA256;
    }
    break;

    default:
    {
      /* should not happen */
      TLS_PRINTF(("Unsupported Csm KeyDerive algorithm: %d\n", ciphersuite));
      TLS_UNREACHABLE_CODE_ASSERT(TLS_INTERNAL_SVCID);
    }
    break;
  }

  return algorithm;
}
#endif /* TLS_CSM_KEYDERIVE_SUPPORT == STD_ON */



#if TLS_CFG_CSM_PROCESSING == TLS_CSM_ASYNCHRONOUS

FUNC(void, CSM_APPL_CODE) Tls_CsmDecryptCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result)
{
  uint8 i;
  boolean ok = FALSE;

  TLS_PRINTF(("Tls_CsmDecryptCallback %d\n", job->jobId));

  for(i=0u;i<TLS_CFG_CONNECTION_COUNT;++i)
  {
    if((Tls_Config.ConnectionConfigs[i].decryptJobId == job->jobId) && (Tls_Connections[i].decryptState == TLS_DECRYPT_STATE_BUSY))
    {
      Tls_DecryptCallback(&Tls_Connections[i], result);
      ok = TRUE;
      break;
    }
  }

  if(!ok)
  {
    TLS_PRINTF(("Tls_CsmDecryptCallback FAIL\n"));
  }
}

FUNC(void, CSM_APPL_CODE) Tls_CsmEncryptCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result)
{
  uint8 i;
  boolean ok = FALSE;

  TLS_PRINTF(("Tls_CsmEncryptCallback JobId=%d\n", job->jobId));

  for(i=0u;i<TLS_CFG_CONNECTION_COUNT;++i)
  {
    if((Tls_Config.ConnectionConfigs[i].encryptJobId == job->jobId) && (Tls_Connections[i].encryptState == TLS_ENCRYPT_STATE_BUSY))
    {
      Tls_EncryptCallback(&Tls_Connections[i], result);
      ok = TRUE;
      break;
    }
  }

  if(!ok)
  {
    TLS_PRINTF(("Tls_CsmEncryptCallback ignored\n"));
  }
}

FUNC(void, CSM_APPL_CODE) Tls_CsmRandomCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result)
{
  uint8 i;
  boolean ok = FALSE;
  Tls_PtrConnection ConnectionPtr;

  TLS_PRINTF(("Tls_CsmRandomCallback %d\n", job->jobId));

  for(i=0u;i<TLS_CFG_CONNECTION_COUNT;++i)
  {
    ConnectionPtr = &Tls_Connections[i];
    /* Find connection with the given jobId and busy state */
    if((Tls_Config.ConnectionConfigs[i].randomJobId == job->jobId) &&
        ((ConnectionPtr->handshakeData.randomState == TLS_RANDOM_STATE_BUSY) || (ConnectionPtr->handshakeData.dtls_cookieState == TLS_RANDOM_STATE_BUSY))
      )
    {
      Tls_RandomCallback(ConnectionPtr, result);
      ok = TRUE;
      break;
    }
  }

  if(!ok)
  {
    TLS_PRINTF(("Tls_CsmRandomCallback ignored\n"));
  }
}

FUNC(void, CSM_APPL_CODE) Tls_CsmHashCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result)
{
  uint8 i;
  boolean ok = FALSE;

  TLS_PRINTF(("Tls_CsmHashCallback job Id=%d\n", job->jobId));

  for(i=0u;i<TLS_CFG_CONNECTION_COUNT;++i)
  {
    if
    (
      (Tls_Config.ConnectionConfigs[i].hashJobId == job->jobId) &&
      (
        (Tls_Connections[i].handshakeData.clientVerifyDataState == TLS_VERIFYDATA_STATE_HASH_BUSY) ||
        (Tls_Connections[i].handshakeData.serverVerifyDataState == TLS_VERIFYDATA_STATE_HASH_BUSY)
      )
    )
    {
      TLS_PRINTF(("Hash ready: %d\n", result));
      Tls_HashCallback(&Tls_Connections[i], result);
      ok = TRUE;
      break;
    }
  }

  if(!ok)
  {
    TLS_PRINTF(("Tls_CsmHashCallback ignored\n"));
  }
}

FUNC(void, CSM_APPL_CODE) Tls_CsmPRFCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result)
{
  uint8 i;
  boolean ok = FALSE;

  TLS_PRINTF(("Tls_CsmPRFCallback %d\n", job->jobId));

  for(i=0u;i<TLS_CFG_CONNECTION_COUNT;++i)
  {
    if
    (
      (
        (Tls_Connections[i].handshakeData.prf.state != TLS_PRF_STATE_IDLE) &&
        (Tls_Connections[i].handshakeData.prf.state != TLS_PRF_STATE_START) &&
        (Tls_Connections[i].handshakeData.prf.state != TLS_PRF_STATE_READY) &&
        (Tls_Connections[i].handshakeData.prf.state != TLS_PRF_STATE_ERROR)
      )
      && (Tls_Connections[i].handshakeData.prf.jobId == job->jobId)
    )
    {
      TLS_PRINTF(("PRF HMAC ready.\n"));
      Tls_PRFCallback(&Tls_Connections[i], result);
      ok = TRUE;
      break;
    }
  }

  if(!ok)
  {
    TLS_PRINTF(("Tls_CsmPRFCallback ignored\n"));
  }
}

FUNC(void, CSM_APPL_CODE) Tls_CsmMacGenerateCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result)
{
  uint8 i;
  boolean ok = FALSE;

  TLS_PRINTF(("Tls_CsmMacGenerateCallback %d\n", job->jobId));

  for(i=0u;i<TLS_CFG_CONNECTION_COUNT;++i)
  {
    if
    (
      (Tls_Config.ConnectionConfigs[i].MACGenerateJobId == job->jobId) &&
      (
        (Tls_Connections[i].macGenerateState == TLS_MAC_STATE_HEADER_BUSY) ||
        (Tls_Connections[i].macGenerateState == TLS_MAC_STATE_PAYLOAD_BUSY) ||
        (Tls_Connections[i].macGenerateState == TLS_MAC_STATE_FINISH_BUSY)
      )
    )
    {
      TLS_PRINTF(("MacGenerate ready: %d\n", result));
      Tls_MacGenerateCallback(&Tls_Connections[i], result);
      ok = TRUE;
      break;
    }
  }

  if(!ok)
  {
    TLS_PRINTF(("Tls_CsmMacGenerateCallback ignored\n"));
  }
}

FUNC(void, CSM_APPL_CODE) Tls_CsmMacVerifyCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result)
{
  uint8 i;
  boolean ok = FALSE;

  TLS_PRINTF(("Tls_CsmMacVerifyCallback %d\n", job->jobId));

  for(i=0u;i<TLS_CFG_CONNECTION_COUNT;++i)
  {
    if((Tls_Config.ConnectionConfigs[i].MACVerifyJobId == job->jobId) &&
      (
        (Tls_Connections[i].macVerifyState == TLS_MAC_STATE_HEADER_BUSY) ||
        (Tls_Connections[i].macVerifyState == TLS_MAC_STATE_PAYLOAD_BUSY) ||
        (Tls_Connections[i].macVerifyState == TLS_MAC_STATE_FINISH_BUSY)
      )
    )
    {
      TLS_PRINTF(("MacVerify ready: %d\n", result));
      Tls_MacVerifyCallback(&Tls_Connections[i], result);
      ok = TRUE;
      break;
    }
  }

  if(!ok)
  {
    TLS_PRINTF(("Tls_CsmMacVerifyCallback ignored\n"));
  }
}

#if TLS_CERTIFICATE_SUPPORT == STD_ON
FUNC(void, CSM_APPL_CODE) Tls_CsmSignatureGenerateCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result)
{
  uint8 i;
  boolean ok = FALSE;

  TLS_PRINTF(("Tls_CsmSignatureGenerateCallback %d\n", job->jobId));

  for(i=0u; i < TLS_CFG_CONNECTION_COUNT; ++i)
  {
    if((Tls_Config.ConnectionConfigs[i].localCertSigGenerateJobId == job->jobId) &&
       ((Tls_Connections[i].handshakeData.sigGenState == TLS_SIG_GEN_STATE_CLIENT_RANDOM_BUSY) ||
           (Tls_Connections[i].handshakeData.sigGenState == TLS_SIG_GEN_STATE_SERVER_RANDOM_BUSY) ||
           (Tls_Connections[i].handshakeData.sigGenState == TLS_SIG_GEN_STATE_SKEX_PARAMS_BUSY) ||
           (Tls_Connections[i].handshakeData.sigGenState == TLS_SIG_GEN_STATE_FINISH_BUSY))
    )
    {
      TLS_PRINTF(("SignatureGenerate ready: %d\n", result));
      Tls_SignatureGenerateCallback(&Tls_Connections[i], result);
      ok = TRUE;
      break;
    }
  }


  if(!ok)
  {
    TLS_PRINTF(("Tls_CsmSignatureGenerateCallback ignored\n"));
  }
}

FUNC(void, CSM_APPL_CODE) Tls_CsmSignatureVerifyCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result)
{
  uint8 i;
  boolean ok = FALSE;

  TLS_PRINTF(("Tls_CsmSignatureVerifyCallback %d\n", job->jobId));

  for(i=0u; i < TLS_CFG_CONNECTION_COUNT; ++i)
  {
    if((Tls_Config.ConnectionConfigs[i].remoteCertSigVerifyJobId == job->jobId) &&
       (Tls_Connections[i].handshakeData.sigVerState == TLS_SIG_VER_STATE_BUSY)
    )
    {
      TLS_PRINTF(("SignatureVerify ready: %d\n", result));
      Tls_SignatureVerifyCallback(&Tls_Connections[i], result);
      ok = TRUE;
      break;
    }
  }

  if(!ok)
  {
    TLS_PRINTF(("Tls_CsmSignatureVerifyCallback ignored\n"));
  }
}

FUNC(void, CSM_APPL_CODE) Tls_CsmJobKeyExchangeCalcPubValCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result)
{
  uint8 i;
  boolean ok = FALSE;

  TLS_PRINTF(("Tls_CsmJobKeyExchangeCalcPubValCallback %d\n", job->jobId));

  for(i=0u; i < TLS_CFG_CONNECTION_COUNT; ++i)
  {
    /* TODO: not KEY ID, job ID! */
    if((Tls_Config.ConnectionConfigs[i].keyExchangeEphemeralExchangeKeyId == job->jobId) &&
       (Tls_Connections[i].handshakeData.keCalcPubValState == TLS_KE_CALC_PUB_VAL_STATE_BUSY)
    )
    {
      TLS_PRINTF(("JobKeyExchangeCalcPubVal ready: %d\n", result));
      Tls_JobKeyExchangeCalcPubValCallback(&Tls_Connections[i], result);
      ok = TRUE;
      break;
    }
  }

  if(!ok)
  {
    TLS_PRINTF(("Tls_CsmJobKeyExchangeCalcPubValCallback ignored\n"));
  }
}

FUNC(void, CSM_APPL_CODE) Tls_CsmJobKeyExchangeCalcSecretCallback(P2CONST(Crypto_JobType, AUTOMATIC, CSM_APPL_DATA) job, Std_ReturnType result)
{
  uint8 i;
  boolean ok = FALSE;

  TLS_PRINTF(("Tls_CsmJobKeyExchangeCalcSecretCallback %d\n", job->jobId));

  for(i=0u; i < TLS_CFG_CONNECTION_COUNT; ++i)
  {
    /* TODO: not KEY ID, job ID! */
    if((Tls_Config.ConnectionConfigs[i].keyExchangeEphemeralExchangeKeyId == job->jobId) &&
       (Tls_Connections[i].handshakeData.keCalcSecretState == TLS_KE_CALC_SECRET_STATE_BUSY)
    )
    {
      /* TODO: assert for key ID */
      TLS_PRINTF(("JobKeyExchangeCalcSecret ready: %d\n", result));
      Tls_JobKeyExchangeCalcSecretCallback(&Tls_Connections[i], result);
      ok = TRUE;
      break;
    }
  }

  if(!ok)
  {
    TLS_PRINTF(("Tls_CsmJobKeyExchangeCalcSecretCallback ignored\n"));
  }
}
#endif /* TLS_CERTIFICATE_SUPPORT == STD_ON */

#endif /* TLS_CFG_CSM_PROCESSING == TLS_CSM_ASYNCHRONOUS */



#if (TLS_CERTIFICATE_SUPPORT == STD_ON)
FUNC(Std_ReturnType, TLS_APPL_CODE) Tls_KeyMVerifyCertificateCallback(KeyM_CertificateIdType CertId, KeyM_CertificateStatusType Result)
{
  uint8 connIdx;

  TLS_PRINTF(("Tls_KeyMVerifyCertificateCallback %d\n", CertId));

  for(connIdx = 0U; connIdx < TLS_CFG_CONNECTION_COUNT; connIdx++)
  {
    if((Tls_Config.ConnectionConfigs[connIdx].remoteCertChainIdPtr[0U] == CertId) && (Tls_Connections[connIdx].handshakeData.verifyCertState == TLS_VERIFY_CERT_STATE_BUSY))
    {
      TLS_PRINTF(("VerifyCertificate ready: %d\n", Result));
      Tls_VerifyCertificateCallback(&Tls_Connections[connIdx], Result);
      break;
    }
  }

  return E_OK;
}
#endif /* (TLS_CERTIFICATE_SUPPORT == STD_ON) */

FUNC(void, TLS_CODE) Tls_TxConfirmation(TcpIp_SocketIdType SocketId, uint16 Length)
{
  uint16 overhead;
  uint16 confirmed;
  Tls_PtrConnection ConnectionPtr = Tls_FindConnection(SocketId);

  TLS_PRINTF(("Tls_TxConfirmation: %d bytes\n", Length));
#if(TLS_DEV_ERROR_DETECT == STD_ON)
  if (Tls_Initialized == FALSE)
  {
    /* !LINKSTO Tls.API.Tls_TxConfirmation.Det.NoInit,1 */
    TLS_DET_REPORT_ERROR(TLS_TXCONFIRMATION_SVCID, TLS_E_UNINIT);
  }
  else
#endif
  {
    if(ConnectionPtr != NULL_PTR)
    {
      confirmed = Tls_TxConfirmation_Dequeue(ConnectionPtr, Length);
      if(confirmed > 0u)
      {
        overhead = Tls_GetRecordOverhead(ConnectionPtr);
        /* CHECK: NOPARSE */
        if(confirmed > overhead)
        /* CHECK: PARSE */
        {
          confirmed -= overhead;
          Tls_UL_TxConfirmation(ConnectionPtr, confirmed);
        }
        /* CHECK: NOPARSE */
        else
        {
          /* Confirmed less bytes than overhead of one record. Should not happen. */
          TLS_UNREACHABLE_CODE_ASSERT(TLS_INTERNAL_SVCID);
        }
        /* CHECK: PARSE */
      }
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(uint8, TLS_CODE) Tls_Handshake_GetHeaderSize(Tls_PtrConnection ConnectionPtr)
{
  uint8 ret;

  if(ConnectionPtr->config->dtls)
  {
    ret = TLS_HANDSHAKE_DTLS_HEADER_SIZE;
  }
  else
  {
    ret = TLS_HANDSHAKE_HEADER_SIZE;
  }

  return ret;
}

TS_MOD_PRIV_DEFN FUNC(uint8, TLS_CODE) Tls_GetRecordHeaderSize(Tls_PtrConstConnection ConnectionPtr)
{
  uint8 size;
  if(ConnectionPtr->config->dtls)
  {
    size = TLS_RECORD_DTLS_HEADER_SIZE;
  }
  else
  {
    size = TLS_RECORD_HEADER_SIZE;
  }

  return size;
}

/* !LINKSTO Tls.RFC.Handshake.Format,1 */
TS_MOD_PRIV_DEFN FUNC(void, TLS_CODE) Tls_Handshake_WriteHeader(Tls_PtrConnection ConnectionPtr, Tls_PtrUint8 dest, uint8 handshake_type, uint32 handshake_len)
{
  /* Type */
  dest[0] = handshake_type;

  /* Length */
  dest[1] = (uint8)((uint8)(handshake_len >> 16) & 0xFFu);
  dest[2] = (uint8)((uint8)(handshake_len >> 8) & 0xFFu);
  dest[3] = (uint8)((uint8)(handshake_len >> 0) & 0xFFu);

  /* !LINKSTO Dtls.RFC.Handshake.Format,1 */
  if (ConnectionPtr->config->dtls)
  {
    /* Sequence */
    dest[4] = (uint8)((uint8)(ConnectionPtr->handshakeData.dtls_handshakeSeq >> 8) & 0xFFu);
    dest[5] = (uint8)((uint8)(ConnectionPtr->handshakeData.dtls_handshakeSeq >> 0) & 0xFFu);

    /* !LINKSTO Dtls.RFC.Handshake.Fragmentation,1 */
    /* Fragment offset */
    dest[6] = 0u;
    dest[7] = 0u;
    dest[8] = 0u;

    /* Fragment length */
    dest[9] = (uint8)((uint8)(handshake_len >> 16) & 0xFFu);
    dest[10] = (uint8)((uint8)(handshake_len >> 8) & 0xFFu);
    dest[11] = (uint8)((uint8)(handshake_len >> 0) & 0xFFu);
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TLS_CODE) Tls_WriteRecordHeaderToTransmitBuffer(Tls_PtrConnection ConnectionPtr, uint32 pos, uint16 Length, uint8 type)
{
  if (ConnectionPtr->config->dtls)
  {
    /* !LINKSTO Dtls.RFC.Record.Version.Write,1 */
    Tls_Dtls_WriteRecordHeader(&ConnectionPtr->transmitBuffer.buffer[pos], type, TLS_DTLS_VERSION, Length, ConnectionPtr->dtls.writeEpoch, ConnectionPtr->writeSeqNum);
  }
  else
  {
    Tls_WriteRecordHeader(&ConnectionPtr->transmitBuffer.buffer[pos], type, TLS_VERSION, Length);
  }
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, TLS_CODE) Tls_Buffer_AppendByte(Tls_PtrBuffer BufferPtr, uint8 Byte)
{
  return Tls_Buffer_Append(BufferPtr, &Byte, 1U);
}

TS_MOD_PRIV_DEFN FUNC(void, TLS_CODE) Tls_Alert_Transmit(Tls_PtrConnection ConnectionPtr, uint8 Level, uint8 Desc)
{
  Std_ReturnType ret = E_OK;
  uint8 headerSize = Tls_GetRecordHeaderSize(ConnectionPtr);

  TLS_PRINTF(("Tls_Alert_Transmit\n"));

  /* Do not transmit in CLOSING or ERROR state */
  if((ConnectionPtr->state != TLS_STATE_CLOSING) &&
      (ConnectionPtr->state != TLS_STATE_CLOSED) &&
      (ConnectionPtr->state != TLS_STATE_ERROR)
    )
  {
    TLS_ENTER_EXCLUSIVE_AREA_TX();

    /* Check if there is enough space in the buffer */
    if(Tls_Buffer_GetFreeBytes(&ConnectionPtr->transmitBuffer) >= ((uint16)headerSize + 2u))
    {
      /* Write header to transmit buffer. It will fit as we checked before. */
      Tls_WriteRecordHeaderToTransmitBuffer(ConnectionPtr, ConnectionPtr->transmitBuffer.count, 2u, TLS_RECORD_ALERT);
      ConnectionPtr->transmitBuffer.count += headerSize;

      /* Write level to transmit buffer. It will fit as we checked before. */
      (void)Tls_Buffer_AppendByte(&ConnectionPtr->transmitBuffer, Level);
      /* Write description to transmit buffer. It will fit as we checked before. */
      (void)Tls_Buffer_AppendByte(&ConnectionPtr->transmitBuffer, Desc);

      TLS_EXIT_EXCLUSIVE_AREA_TX();

      ret = Tls_TransmitRecord(ConnectionPtr);
    }
    else
    {
      TLS_EXIT_EXCLUSIVE_AREA_TX();
      ret = E_NOT_OK;
    }

    if(ret == E_OK)
    {
      /* !LINKSTO Tls.RFC.Alert.Fatal,1 */
      /* If it was a fatal alert or something went wrong, go to CLOSING state */
      if(Level == TLS_ALERT_LEVEL_FATAL)
      {
        Tls_SetState(ConnectionPtr, TLS_STATE_CLOSING);
        if(Desc != TLS_ALERT_DESC_CLOSE_NOTIFY)
        {
          /* Set error flag in case this was not a normal close. Will go to ERROR state after close is complete. */
          ConnectionPtr->errorFlag = TRUE;
        }
      }
    }
    else
    {
      /* Something went wrong, close with error */
      Tls_SetState(ConnectionPtr, TLS_STATE_CLOSING);
      ConnectionPtr->errorFlag = TRUE;
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(uint32, TLS_CODE) Tls_Handshake_GetLength(Tls_PtrConstUint8 handshake)
{
  /* TODO: preprocessor offset */
  return TLS_GET24(handshake, 1U);
}

TS_MOD_PRIV_DEFN FUNC(Tls_PtrConnection, TLS_CODE) Tls_GetConnectionPtr(Tls_ConnectionIdType ConnectionId)
{
  return &Tls_Connections[ConnectionId];
}

#define TLS_STOP_SEC_CODE
#include <Tls_MemMap.h>
