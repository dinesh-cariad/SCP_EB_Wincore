/**
 * \file
 *
 * \brief AUTOSAR TcpIp
 *
 * This file contains the implementation of the AUTOSAR
 * module TcpIp.
 *
 * \version 3.5.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*
 * Misra-C:2012 Deviations:
 *
 * MISRAC2012-3) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * 1) Init time allocation requires a cast to the type of the allocated object.
 * 2) Keeping Reassembly details from the generic buffer allocation functions requires a
 *    cast-to-generic-pointer.
 * 3) Using the lowest bit of the pointer as an available-flag saves space and makes
 * unused pointers unaligned (signalling error).
 * We cast the pointer to uint8* (char*), so we can increment & decrement the pointer to
 * set & reset the lowest bit.
 *
 * MISRAC2012-5) Deviated Rule: 11.4 (advisory)
 * A conversion should not be performed between a pointer to object and an integer type
 *
 * Reason:
 * To check alignment, the pointer has to be converted to integer, so we can inspect the lower bits
 * using bit arithmetic.
 *
 * MISRAC2012-6) Deviated Rule: 11.6 (required)
 * A cast shall not be performed between pointer to void and an arithmetic type.
 *
 * Reason:
 * Using the lowest bit of the pointer as an available-flag saves space and makes
 * unused pointers unaligned (signalling error).
 * We cast the pointer to uint8* (char*), so we can increment & decrement the pointer to
 * set & reset the bit.
 *
 * MISRAC2012-9) Deviated Rule: 20.7 (required)
 * Expressions resulting from the expansion of macro parameters shall be enclosed in
 * parentheses.
 *
 * Reason:
 * 1) Parenthesis defeat the purpose of a compile-time-if macro: e.g. it could not be used
 * within a parameter list, where the additional parenthesis are harmful.
 *
 */

/*==================[inclusions]============================================*/
#include <TSMem.h>                /* memory functions */
#include <TcpIp_CfgTypes.h>       /* TcpIp pre-compile-time dependent configuration types */
#include <SchM_TcpIp.h>

#include <TcpIp_guards.h>         /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Ip_Priv.h>
#include <TcpIp_Trace.h>          /* debug and trace macros; must be last */

/*==================[macros]================================================*/

#if (TCPIP_IPV6_ENABLED == STD_ON) || (TCPIP_IPV4_ENABLED == STD_ON)

/** \brief Test, if a pointer was marked as available (LSB set).
 * Used internally by \a TcpIp_Ip_Frag_findAvailableBuffer().
 * odd (misaligned) pointer means available.
 * Even if the conversion from pointer to uint32 loses significant digits,
 * we still test the correct bit. */
  /* Deviation MISRAC2012-6 */
#define TCPIP_IP_FRAG_IS_AVAILABLE(ptr)     ((((uint32)(ptr))&1u)==1u)  /* TS_IsBitSet(ptr, 0u) */

/** \brief Mark the pointer as unavailable.
 * Used internally by \a TcpIp_Ip_Frag_findAvailableBuffer().
 * We decrement the pointer, which was casted to uint8* (char*) to avoid
 * pointer-to-integer-to-pointer conversions with potential loss of significant bits.
 */
#define TCPIP_IP_FRAG_MAKE_UNAVAILABLE(ptr) (--*( (uint8**)&(ptr) )) /* TS_AtomicClearBit_32(ptr, 0u) */

/** \brief Mark the pointer as available.
 * Must be used to free a buffer allocated by \a TcpIp_Ip_Frag_findAvailableBuffer().
 * We increment the pointer, which was casted to uint8* (char*) to avoid
 * pointer-to-integer-to-pointer conversions with potential loss of significant bits.
 */
#define TCPIP_IP_FRAG_MAKE_AVAILABLE(ptr)   (++*( (uint8**)&(ptr) )) /* TS_AtomicSetBit_32(ptr, 0u) */

/*==================[type definitions]======================================*/

#if ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_OUTOFORDER )

/** \brief Structure containing information to copy from upper layer (out of order) */
typedef struct
{
  /**< \brief fragmentation internal use: the accumulated checksum over all payload fragments */
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) payloadSumPtr;
  /**< \brief Start of the upper layer payload (e.g. UDP) */
  uint16  upLayerDataOffset;
  /**< \brief Length of the upper layer payload*/
  uint16  upLayerPayloadLength;
} TcpIp_Ip_Frag_UpLayerInfoType;

#endif /* ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_OUTOFORDER ) */

#if ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_INORDER )

/** \brief Structure containing information to copy from upper layer (In order)*/
typedef struct
{
  /**< \brief fragmentation internal use: the accumulated checksum over all payload fragments */
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) payloadSumPtr;
  /**< \brief Start of the upper layer payload (e.g. UDP) */
  uint16 upLayerDataOffset;
  /**< \brief Number of fragments to transmit */
  uint16 fragmentCount;
  /**< \brief Length of the last fragment to transmit */
  uint16 fragmetLastLength;
  /**< \brief Length of the fragment to transmit */
  uint16 fragmentLength;
} TcpIp_Ip_Frag_CopyInfoType;

#endif /* ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_INORDER ) */

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

#if ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_INORDER )

/** \brief Find next pointer that is marked as free in an array of pointers (\a buffers), limited by \a max.
 * The LSB is used as free(1) / in-use(0) marker, hence only 16bit aligned buffers can be handled.
 * \param buffers[in] Pointer to the array of buffer pointers.
 * \param max[in]     Length of the array of buffer pointers.
 * \return            The index of the reserved buffer.
 * \par
 * potential improvement: ringbuffer
 * keep last found as start of next search -> typical O(1) search, not degenerating.
 * (-) harder to test coverage
 */
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_Ip_Frag_findAvailableBuffer
(
  P2VAR(TcpIp_VoidPtrType, AUTOMATIC, TCPIP_APPL_DATA) buffers,
  uint16 max
);
#endif /* TCPIP_IP_FRAGMENT_TX_ENABLED == STD_ON */

/** \brief Copy the data into the transmit buffer.
 *
 *  This function copies Udp payload data into the transmit buffer.
 *  If \a
 *  header and invokes the transmission of the Udp frame over Ip.
 *  Please note that the Udp payload is not part of this function but must be copied into the buffer
 *  between the calls to
 *
 * \param[in] copyDataFunctorPtr The copyData functor.
 * \param[in,out] payloadSumPtr  accumulated checksum over all payload fragments
 * \param[in,out] ethTxHandlePtr The data is copied to this buffer area.
 * \param[in,out] dataPtr        The source data buffer. If NULL_PTR, the the copxTxData interface is used.
 * \param[in] dataOffset         Byte offset of the area to copy, if dataPtr != NULL. i.e. start at dataPtr[dataOffset]
 * \param[in] calculateChecksum  Indicates if checksum shall be calculated of ther upper layer data
 *
 * \return Result of operation
 * \retval TCPIP_OK             everything ok
 * \retval TCPIP_E_NOT_OK       copxTxData interface returned an error code != BUFREQ_OK.
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ip_Frag_copyTxData
(
  P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) copyDataFunctorPtr,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) payloadSumPtr,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataOffset,
  boolean calculateChecksum
);

#if ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_OUTOFORDER )

/** \brief Performs fragmentation for Out-Of-Order transmission.
 *
 * This functions transmits the payload fragments.
 *
 * \param[in] copyDataFunctorPtr      The copyData functor.
 * \param[in] ipParameterPtr          The IP parameter configuration block.
 * \param[in] fragInfoPtr             Assorted fragmentation information.
 * \param[in] remoteAddrPtr           Points to a structure containing the remote address information
 * \param[in] dataPtr                 The source data buffer. If NULL_PTR, the copyTxData interface is used.
 * \param[in] localAddrId             Local address identifier.
 * \param[in] remoteAddrHandle        Indicates remote link layer address index
 *
 * \return Result of operation
 * \retval TCPIP_OK               A buffer has of sufficient size has been provided.
 * \retval TCPIP_E_NOT_OK         Requesting the buffer failed.
 * \retval TCPIP_E_PHYS_ADDR_MISS Requesting buffer failed due to missing physical remote address.
 *                                Instead an ARP request was transmitted.
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ip_Frag_transmitPayloadFrags
(
  P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) copyDataFunctorPtr,
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2VAR(TcpIp_Ip_Frag_UpLayerInfoType, AUTOMATIC, TCPIP_APPL_DATA) fragInfoPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  TcpIp_LocalAddrIdType localAddrId,
  uint16 remoteAddrHandle
);

#endif /* ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_OUTOFORDER ) */

#if ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_INORDER )

/** \brief Retrieve buffer and copy data
 *
 * This functions retrieves hardware buffer for all the required ip fragments and copies the
 * data from the upper layer into these buffers
 *
 * \param[in] copyDataFunctorPtr      The copyData functor.
 * \param[in] ipParameterPtr          The IP parameter configuration block.
 * \param[in] fragInfoPtr             Assorted fragmentation information.
 * \param[in] ethTxHandlePtr          Points to a structure containing the hardware buffers
 * \param[in] addrPtr                 Local and remote ip address
 * \param[in] dataPtr                 The source data buffer. If NULL_PTR, the copyTxData interface is used.
 * \param[out] numFragToTransmit      Indicates the number of fragment buffers that have been allocated
 *                                    and need to be transmitted
 *
 * \return Result of operation
 * \retval TCPIP_OK               A buffer has of sufficient size has been provided.
 * \retval TCPIP_E_NOT_OK         Requesting the buffer failed.
 */
STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ip_Frag_copyDataInOrder
(
  P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) copyDataFunctorPtr,
  P2VAR(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2VAR(TcpIp_Ip_Frag_CopyInfoType, AUTOMATIC, TCPIP_APPL_DATA) fragInfoPtr,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  P2CONST(TcpIp_Ip_AddrType, AUTOMATIC, TCPIP_APPL_DATA) addrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) numFragToTransmitPtr
);

#endif /* ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_INORDER ) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

#if ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_INORDER )

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/** \brief The buffers used for INORDER fragmented-datagram transmission. */
STATIC P2VAR(TcpIp_Ip_EthTxHandlePtrType, TCPIP_VAR_CLEARED, TCPIP_APPL_DATA) TcpIp_Ip_Frag_TxBuffers;

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

#endif /* TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_INORDER */


/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

#if ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF )

TS_MOD_PRIV_DEFN FUNC(uint32, TCPIP_CODE) TcpIp_Ip_Frag_init
(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_Ip_Frag_MemReserved,
  uint32 startOffset
)
{
  uint32 n= startOffset;
  /* Deviation MISRAC2012-5 <+2> */
  TCPIP_PRECONDITION_ASSERT(
      TCPIP_ISALIGNED(&TcpIp_Ip_Frag_MemReserved[n], sizeof(uint32)),
      TCPIP_SID_INIT);

#if ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_OUTOFORDER )
#elif ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_INORDER )
  {
    uint16 i=0;
    uint32 bufSize= TcpIp_PBcfgPtr->ipFragConfig.txFragmentSegmentCount * sizeof(TcpIp_Ip_EthTxHandleType);
    {
      /* Deviation MISRAC2012-3 */
      TcpIp_Ip_Frag_TxBuffers=
        (P2VAR(TcpIp_Ip_EthTxHandlePtrType, AUTOMATIC, TCPIP_APPL_DATA)) &TcpIp_Ip_Frag_MemReserved[n];
      n+= TcpIp_PBcfgPtr->ipFragConfig.txFragmentBufferCount * sizeof(TcpIp_Ip_EthTxHandlePtrType);

      for (i=0; i < TcpIp_PBcfgPtr->ipFragConfig.txFragmentBufferCount; ++i)
      {
        /* Deviation MISRAC2012-3 */
        TcpIp_Ip_Frag_TxBuffers[i]= (TcpIp_Ip_EthTxHandlePtrType) &TcpIp_Ip_Frag_MemReserved[n];
        TCPIP_IP_FRAG_MAKE_AVAILABLE(TcpIp_Ip_Frag_TxBuffers[i]);
        n+= bufSize;
      }
    }
    /* Deviation MISRAC2012-5 <+2> */
    TCPIP_POSTCONDITION_ASSERT(
        TCPIP_ISALIGNED(&TcpIp_Ip_Frag_MemReserved[n], sizeof(uint32)),
        TCPIP_SID_INIT);
  }
#endif /* TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF */

  TCPIP_POSTCONDITION_ASSERT(n<=TCPIP_IP_FRAGMENT_MEMRESERVED, TCPIP_SID_INIT);  /* TCPIP_E_NOBUFS */
  return n;
}
#endif /* ( TCPIP_IP_FRAGMENT_TX_ENABLED != STD_OFF ) */


#if ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_INORDER )
TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ip_transmitIp /*IO*/
(
  P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) copyDataFunctorPtr,
  P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) updateHeaderFunctorPtr,
  P2VAR(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2CONST(TcpIp_Ip_AddrType, AUTOMATIC, TCPIP_APPL_DATA) addrPtr,
  uint16 remoteHandle,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 headerLength,
  uint16 upperLayerPayloadLength
)
{
  TcpIp_ReturnType retVal= TCPIP_E_NOT_OK;
  const uint16 maxPayload = ipParameterPtr->maxIpPayloadLength;
  uint16 payloadSum = 0U;
  uint16 frgCnt;
  uint16 frgLastLen;
#if(TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
  /* additional header like AH must be considered during transmission */
  uint8 addHeaderLength = ipParameterPtr->addHeaderLength;
#else /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
  uint8 addHeaderLength = 0U;
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
#if (TCPIP_IP_DUALSTACK == STD_ON)
  uint8 const page = TCPIP_IP_DOMAIN_GET(addrPtr->remoteAddrPtr->domain);
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */

  const uint16 upLayerDataOffset = headerLength + addHeaderLength;
  const uint16 ipPayloadLength = upLayerDataOffset + upperLayerPayloadLength;
  uint16 fragmentLength;
  boolean IPisFragment;

  DBG_TCPIP_IP_TRANSMITIP_ENTRY
    (copyDataFunctorPtr, updateHeaderFunctorPtr, ipParameterPtr,
     addrPtr, remoteHandle, dataPtr, headerLength, upperLayerPayloadLength);

  /* UDP header is included in totalLength, IpVx (+fragment) header was subtracted from maxPayload by IpVxProvideTxBuffer */
  if ((upperLayerPayloadLength + headerLength) > maxPayload)
  {
    /* Deviation MISRAC2012-9 */
    TcpIp_Ip_getFragmentSize_FctPtrType const fragmentSize = TCPIP_IP_GETFRAGMENTSIZE(page);
    /* Fragmentation limit: any non-last fragment must be a multiple of 8 bytes
     * (the UDP header itself is 8 bytes) */
    fragmentLength = fragmentSize(addrPtr->localAddrId);
    fragmentLength &= 0xFFF8U; /* only modified for the last loop iteration */
    IPisFragment = TRUE;

    /* make sure frgLastLen is not 0, while last iteration handles partial (or full) buffer */
    frgCnt = (((ipPayloadLength - 1U) / fragmentLength) + 1U);
    frgLastLen = (((ipPayloadLength - 1U) % fragmentLength) + 1U);
  }
  else
  {
    fragmentLength = ipPayloadLength;
    frgCnt = 1U;
    frgLastLen = ipPayloadLength;
    IPisFragment = FALSE;
  }

  {
#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
    if (frgCnt > TcpIp_PBcfgPtr->ipFragConfig.txFragmentSegmentCount)
    {
      TCPIP_REPORT_ERROR(DET_INTERNAL_API_ID, TCPIP_E_MSGSIZE);
    }
    else
#endif /* ( TCPIP_DEV_ERROR_DETECT == STD_ON ) */
    {
      uint16 const freeBufferIdx=
          TcpIp_Ip_Frag_findAvailableBuffer
            (
             /* Deviation MISRAC2012-3 */
             (P2VAR(TcpIp_VoidPtrType, AUTOMATIC, TCPIP_APPL_DATA))TcpIp_Ip_Frag_TxBuffers,
             TcpIp_PBcfgPtr->ipFragConfig.txFragmentBufferCount
            );

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
      if (freeBufferIdx >= TcpIp_PBcfgPtr->ipFragConfig.txFragmentBufferCount)
      {
        TCPIP_REPORT_ERROR(DET_INTERNAL_API_ID, TCPIP_E_NOBUFS);
      }
      else
#endif /* ( TCPIP_DEV_ERROR_DETECT == STD_ON ) */
      {
        /*
         * If In-Order Transmission is needed while using the SoAd copyTxData interface (dataPtr==NULL_PTR)
         * to avoid double-buffering, we store all data in Ethernet transmission buffers, reserving space
         * for the header, which is constructed as soon as the checksum has been computed.
         * This way we only have to store the TX buffer handles for all fragments.
         * While the worst case is 8kB, it can be reduced considerably via configuration.
         *
         * for In-Order transmission we use a
         * 1st loop to collect all data in Ethernet transmit buffers:
         *    provideTxBuffer
         *    copyData
         *    aggregate the checksum while copying,
         * then insert the header into the 1st packet and use a
         * 2nd loop to
         *    transmit all packets in this 2nd loop in-order
         *
         * With max packet size 64k and min MTU 68B, we cannot store the txLengthInfo structs and
         * must recompute the same values with synchronous loops.
         * We must however store the dllHandles of the allocated buffers, and we add one byte of
         * actual length information to those (so far unused padding):
         *   this is sufficient as 256 x 8 (Fragment granularity) = 2K > 1600 (max Eth MTU)
         *   the last fragment's length (not a multiple of 8) can be recomputed by adding totalLength & 7
         * the size of the array is configuration-dependent:
         *   sizeof(*ethTxHandlePtr) (=8)  * maxPacketsize (up to 64kB) / min_IP_Fragment_Payload (Min-TU)
         * this will be a compile-time configuration, derived from the corresponding settings.
         */
        P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) updateHeaderFctPtr = updateHeaderFunctorPtr;
        P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr= TcpIp_Ip_Frag_TxBuffers[freeBufferIdx];
        /* Deviation MISRAC2012-9 <+2> */
        TcpIp_Ip_transmit_FctPtrType const transmit= TCPIP_IP_TRANSMIT(page);
        TcpIp_Ip_Frag_TxHeaderType fragHdr;
        TcpIp_Ip_Frag_CopyInfoType fragCopyHdr;
        uint16 frgIdx = 0U;

        /* initialize data for copying */
        fragCopyHdr.fragmentLength = fragmentLength;
        fragCopyHdr.fragmetLastLength = frgLastLen;
        fragCopyHdr.fragmentCount = frgCnt;
        fragCopyHdr.payloadSumPtr = &payloadSum;
        fragCopyHdr.upLayerDataOffset = upLayerDataOffset;

        /* allocate all necessary buffers and copy data from upper layer to buffers */
        retVal =
          TcpIp_Ip_Frag_copyDataInOrder
            (copyDataFunctorPtr, ipParameterPtr, &fragCopyHdr, ethTxHandlePtr, addrPtr, dataPtr, &frgCnt);

        /* initialize fragmentation parameter for ip header,
           first fragment has offset 0 and more flags set to true */
        fragHdr.IPoffset = 0U;
        fragHdr.IPmoreFlag = TRUE;
        fragHdr.IPisFragment = IPisFragment;

        for ( frgIdx = 0U; frgIdx<frgCnt; ++frgIdx )
        {
          if ((retVal == TCPIP_OK) && (frgIdx == 0U))
          {
            /* add upper layer (UDP, TCP, ICMP) and attach ip header */
            /* Deviation MISRAC2012-9 */
            retVal = TCPIP_IP_UPDATEHEADER(page)( updateHeaderFctPtr,
                                                  ipParameterPtr,
                                                  addrPtr->remoteAddrPtr,
                                                  ethTxHandlePtr,
                                                  addrPtr->localAddrId,
                                                  payloadSum,
                                                  ipPayloadLength,
                                                  frgCnt
                                                );
          }

          if (retVal == TCPIP_OK)
          {
            /* last iteration */
            if (frgIdx==(frgCnt-1U))
            {
              fragmentLength = frgLastLen;
              fragHdr.IPmoreFlag = FALSE;
            }
          }
          else
          {
            /* Even with retVal != TCPIP_OK, the transmission cannot be aborted, because there is no API to
             * discard the TxBuffer. The only way to solve this situation is to send a dummy fragment packet.
             */
            /* No UDP packet, just IP header */
            fragHdr.IPisFragment= FALSE;
            fragHdr.IPmoreFlag = FALSE;
            fragHdr.IPoffset = 0U;
            ethTxHandlePtr[frgIdx].bufferLength = 0U;
          }

          {
            /* Transmit needs to be called even in case of error to free allocates TxBuffers
             * Hence we just make sure to return the 1st error that occurred. */
            TcpIp_ReturnType result =
                transmit
                    (
                      ipParameterPtr,
                      &fragHdr,
                      &ethTxHandlePtr[frgIdx],
                      remoteHandle,
                      addrPtr->remoteAddrPtr,
                      addrPtr->localAddrId
                    );

              if(retVal == TCPIP_OK)
              {
                retVal = result;
              }
          }

          fragHdr.IPoffset += fragmentLength; /* IP Fragmentation offset includes the UDP header */
          updateHeaderFctPtr= NULL_PTR; /* no further upper layer (UDP/ICMP) header & checksum updates */
        }
        TCPIP_INVARIANT_ASSERT((fragHdr.IPoffset == ipPayloadLength) || (retVal != TCPIP_OK),
                               TCPIP_INTERNAL_API_ID);
        /* no atomic section needed: only freed, after pointer is written back */
        TCPIP_IP_FRAG_MAKE_AVAILABLE(TcpIp_Ip_Frag_TxBuffers[freeBufferIdx]);
      }
    }
  }

  DBG_TCPIP_IP_TRANSMITIP_EXIT
    (retVal, copyDataFunctorPtr, updateHeaderFunctorPtr, ipParameterPtr,
     addrPtr, remoteHandle, dataPtr, headerLength, upperLayerPayloadLength);

  return retVal;
}

#else

TS_MOD_PRIV_DEFN FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ip_transmitIp/*OOO*/
(
  P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) copyDataFunctorPtr,
  P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) updateHeaderFunctorPtr,
  P2VAR(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2CONST(TcpIp_Ip_AddrType, AUTOMATIC, TCPIP_APPL_DATA) addrPtr,
  uint16 remoteHandle,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 headerLength,
  uint16 upperLayerPayloadLength
)
{
  TcpIp_ReturnType retVal= TCPIP_E_NOT_OK;
#if ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_OUTOFORDER )
  TcpIp_Ip_Frag_TxHeaderType fragHdr;
#endif /* ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_OUTOFORDER ) */
#if(TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC)
  /* additional header like AH must be considered during transmission */
  uint8 addHeaderLength = ipParameterPtr->addHeaderLength;
#else /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */
  uint8 addHeaderLength = 0U;
#endif /* (TCPIP_SECURITY_MODE >= TCPIP_STATIC_IPSEC) */

  const uint16 upLayerDataOffset = headerLength + addHeaderLength;
  const uint16 ipPayloadLength = upLayerDataOffset + upperLayerPayloadLength;
  uint16 transmitLength = ipPayloadLength;
  uint16 payloadSum = 0U;

  DBG_TCPIP_IP_TRANSMITIP_ENTRY
    (copyDataFunctorPtr, updateHeaderFunctorPtr, ipParameterPtr,
     addrPtr, remoteHandle, dataPtr, headerLength, upperLayerPayloadLength);
  {
    /*
     * Because TCPIP might need data from SoAd (if dataPtr==NULL_PTR), it cannot
     * calculate the checksum until all data is fetched. However, it is also not
     * practical to copy all data from SoAd at once then calculate the checksum.
     *
     * Therefore, a workaround is implemented with the assumption:
     *
     *    UDP Fragments are allowed to be received out of order.
     *
     * The implementation divides the UDP packet to fragments like:
     *
     * Fragment 0 only contains UDP header
     * Fragment 1 contains the 1. part of data
     * Fragment 2 contains the 2. part of data
     * ..........
     * Fragment N contains the N. part of data
     *
     * These fragments are sent with the order: 1, 2, ..., N, 0
     * So that the checksum can be calculated at the very end
     */
#if ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_OUTOFORDER )

    /* UDP header is included in totalLength, IpVx (+fragment) header was subtracted
       from maxPayload by IpVxProvideTxBuffer */
    if(ipPayloadLength > ipParameterPtr->maxIpPayloadLength)
    {
      TcpIp_Ip_Frag_UpLayerInfoType fragUpLayerInfo;

      fragUpLayerInfo.payloadSumPtr = &payloadSum;
      fragUpLayerInfo.upLayerDataOffset = upLayerDataOffset;
      fragUpLayerInfo.upLayerPayloadLength = upperLayerPayloadLength;

      retVal =
        TcpIp_Ip_Frag_transmitPayloadFrags
          (copyDataFunctorPtr, ipParameterPtr, &fragUpLayerInfo, addrPtr->remoteAddrPtr, dataPtr, addrPtr->localAddrId, remoteHandle);

      /* header is sent at last fragment */
      fragHdr.IPisFragment = TRUE;
      fragHdr.IPmoreFlag = TRUE;
      fragHdr.IPoffset = 0U;
      transmitLength = headerLength;
    }
    else
    {
      fragHdr.IPisFragment = FALSE;
      fragHdr.IPmoreFlag = FALSE;
      fragHdr.IPoffset = 0U;
      transmitLength = ipPayloadLength;
      retVal= TCPIP_OK;
    }

    if (retVal == TCPIP_OK)
#endif /* TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_OUTOFORDER */
    {
      TcpIp_Ip_EthTxHandleType ethTxHandle = TCPIP_IP_ETHTXHANDLE_INITIALIZER;
#if (TCPIP_IP_DUALSTACK == STD_ON)
      uint8 const page = TCPIP_IP_DOMAIN_GET(addrPtr->remoteAddrPtr->domain);
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
      /* Deviation MISRAC2012-9 <+2> */
      TcpIp_Ip_provideTxBuffer_FctPtrType const provideTxBuffer= TCPIP_IP_PROVIDETXBUFFER(page);
      TcpIp_Ip_transmit_FctPtrType const transmit= TCPIP_IP_TRANSMIT(page);

      retVal= provideTxBuffer
               (
                ipParameterPtr,
                &ethTxHandle,
                transmitLength, /* request header+total, get actual */
                addrPtr->localAddrId,
                TCPIP_IP_IF_FRAGMENT_TX(fragHdr.IPisFragment, FALSE)
               );

      if(retVal == TCPIP_OK)
      {
        P2VAR(TcpIp_Ip_UpdateHeaderFunctorType, AUTOMATIC, TCPIP_APPL_DATA) updateHeaderFctPtr = updateHeaderFunctorPtr;

        TCPIP_INVARIANT_ASSERT(ethTxHandle.bufferLength == transmitLength, TCPIP_INTERNAL_API_ID);

#if ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_OUTOFORDER )
        if(!fragHdr.IPisFragment)
#endif /* ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_OUTOFORDER ) */
        {
          TcpIp_Ip_EthTxHandleType copyEthTxHandle;

          copyEthTxHandle.bufferPtr = &(ethTxHandle.bufferPtr[upLayerDataOffset]);
          copyEthTxHandle.bufferLength = ethTxHandle.bufferLength - upLayerDataOffset;

          retVal = TcpIp_Ip_Frag_copyTxData
                      (
                        copyDataFunctorPtr,
                        &payloadSum,
                        &copyEthTxHandle,
                        dataPtr,
                        0U,
                        ipParameterPtr->calculateChecksum
                      );
        }

        if (retVal == TCPIP_OK)
        {
          /* Deviation MISRAC2012-9 */
          retVal = TCPIP_IP_UPDATEHEADER(page)( updateHeaderFctPtr,
                                                ipParameterPtr,
                                                addrPtr->remoteAddrPtr,
                                                &ethTxHandle,
                                                addrPtr->localAddrId,
                                                payloadSum,
                                                ipPayloadLength,
                                                1U
                                              );
        }

        if (retVal != TCPIP_OK)
        {
          /* oh dear, something went wrong - but we can't stop transmission now with the
           * existing Autosar APIs. indicate this by passing a zero payload length
           * and no UDP header, transmitting just the IP header without any payload.
           */
#if ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_OUTOFORDER )
          fragHdr.IPisFragment= FALSE;
          fragHdr.IPmoreFlag = FALSE;
          fragHdr.IPoffset = 0U;
#endif /* ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_OUTOFORDER ) */
          ethTxHandle.bufferLength = 0U;
        }

        {
          /* transmit needs to be called even in case of error to free allocated TxBuffers
           * Hence we need to make sure to return the 1st error that occurred. */
          TcpIp_ReturnType result =
            transmit /* TcpIp_IpV4_transmit, TcpIp_IpV6_transmit */
                    (
                      ipParameterPtr,
                      TCPIP_IP_IF_FRAGMENT_TX(&fragHdr, NULL_PTR),
                      &ethTxHandle,
                      remoteHandle,
                      addrPtr->remoteAddrPtr,
                      addrPtr->localAddrId
                    );

          if(retVal == TCPIP_OK)
          {
            retVal = result;
          }
        }
      }
    } /* handle header fragment for OOO or standalone -unfragmented- packet */
  }

  DBG_TCPIP_IP_TRANSMITIP_EXIT
    (retVal, copyDataFunctorPtr, updateHeaderFunctorPtr, ipParameterPtr,
     addrPtr, remoteHandle, dataPtr, headerLength, upperLayerPayloadLength);

  return retVal;
}
#endif /* TCPIP_IP_FRAGMENT_TX_ENABLED != TCPIP_IP_FRAGMENT_INORDER */

/*==================[internal function definitions]=========================*/

#if ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_INORDER )
STATIC FUNC(uint16, TCPIP_CODE) TcpIp_Ip_Frag_findAvailableBuffer
(
  P2VAR(TcpIp_VoidPtrType, AUTOMATIC, TCPIP_APPL_DATA) buffers,
  uint16 max
)
{
  boolean found= FALSE;
  uint16 i;
  for (i= 0u; i < max; ++i)
  {
    /* atomic start (would be nice to have an atomic TAS operation */
    /* !LINKSTO TcpIp.Design.IpFrag.EA4.InOrderTx,1 */
    SchM_Enter_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_4();

    if (TCPIP_IP_FRAG_IS_AVAILABLE(buffers[i]))
    {

      found= TRUE;
      TCPIP_IP_FRAG_MAKE_UNAVAILABLE(buffers[i]);

    }

    /* !LINKSTO TcpIp.Design.IpFrag.EA4.InOrderTx,1 */
    SchM_Exit_TcpIp_SCHM_TCPIP_EXCLUSIVE_AREA_4();
    /* atomic end */

    if (found)
    {
      break;
    }
  }
  return i;
}
#endif /* TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_INORDER */


STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ip_Frag_copyTxData
(
  P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) copyDataFunctorPtr,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) payloadSumPtr,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataOffset,
  boolean calculateChecksum
)
{
  TcpIp_ReturnType retVal = TCPIP_E_NOT_OK;
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) bufferPtr = ethTxHandlePtr->bufferPtr;
  const uint16 copyLength = ethTxHandlePtr->bufferLength;

  {
    if(dataPtr == NULL_PTR)
    {
      /* wouldn't it be nice, if we could compute the checksum during copying?
       * The CPU core is running circles around the memory IF, doing nothing.
       * + the buffer area is potentially uncached due to HW access via DMA,
       * so rereading that is a very bad idea! */
      if(copyDataFunctorPtr->copyDataFctPtr(copyDataFunctorPtr,bufferPtr, copyLength) == BUFREQ_OK)
      {
        if(calculateChecksum)
        {
          *payloadSumPtr= TcpIp_CalcSum16Bit(bufferPtr, *payloadSumPtr, copyLength);
        }
        retVal = TCPIP_OK;
      }
    }
    else
    {
      TS_MemCpy(bufferPtr, &dataPtr[dataOffset], copyLength);
      /* avoid re-reading the potentially uncached (DMA, ...) Eth buffer area, if we can reresd the source! */
      if(calculateChecksum)
      {
        *payloadSumPtr= TcpIp_CalcSum16Bit(&dataPtr[dataOffset], *payloadSumPtr, copyLength);
      }
      retVal = TCPIP_OK;
    }
  }
  return retVal;
}

#if ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_OUTOFORDER )

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ip_Frag_transmitPayloadFrags
(
  P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) copyDataFunctorPtr,
  P2CONST(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2VAR(TcpIp_Ip_Frag_UpLayerInfoType, AUTOMATIC, TCPIP_APPL_DATA) fragInfoPtr,
  P2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  TcpIp_LocalAddrIdType localAddrId,
  uint16 remoteAddrHandle
)
{
  TcpIp_ReturnType retVal= TCPIP_OK;
  TcpIp_Ip_EthTxHandleType ethTxHandle = TCPIP_IP_ETHTXHANDLE_INITIALIZER;
  const uint16 payloadLength= fragInfoPtr->upLayerPayloadLength;
#if (TCPIP_IP_DUALSTACK == STD_ON)
  uint8 const page = TCPIP_IP_DOMAIN_GET(remoteAddrPtr->domain);
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  /* Deviation MISRAC2012-9 <+3> */
  TcpIp_Ip_provideTxBuffer_FctPtrType const provideTxBuffer= TCPIP_IP_PROVIDETXBUFFER(page);
  TcpIp_Ip_transmit_FctPtrType const transmit= TCPIP_IP_TRANSMIT(page);
  TcpIp_Ip_getFragmentSize_FctPtrType const fragmentSize = TCPIP_IP_GETFRAGMENTSIZE(page);
  TcpIp_Ip_Frag_TxHeaderType fragHdr;

  /* Fragmentation limitation: any non-last fragment must be a multiple of 8 bytes
   * (the UDP header itself is 8 bytes) */
  uint16 fragmentLength = fragmentSize(localAddrId);

  fragmentLength &= (uint16)~((uint16)7u); /* only modified for the last loop iteration */
  fragHdr.IPisFragment=  TRUE;
  fragHdr.IPmoreFlag= TRUE;
  {
    /* make sure frgLastLen is > 0, while last iteration handles partial (or full) buffer */
    uint16 const frgCnt =     (((payloadLength - 1U) / fragmentLength) + 1U);
    uint16 const frgLastLen = (((payloadLength - 1U) % fragmentLength) + 1U);
    uint16 copyOffset= 0U;
    uint16 frgIdx= 0U;
    fragHdr.IPoffset= fragInfoPtr->upLayerDataOffset;
    /* Send the payload first */
    do  /* we always have at least 1 fragment to send (see the -1 / +1 for frgCnt) */
    {
      if (frgIdx==(frgCnt-1U)) /* last iteration */
      {
        fragmentLength = frgLastLen; /* to be allocated/copied/transmitted */
        fragHdr.IPmoreFlag = FALSE;
      }

      retVal = provideTxBuffer
                (
                    ipParameterPtr,
                    &ethTxHandle,
                    fragmentLength, /* request fragmentLength, get IPpayload */
                    localAddrId,
                    TRUE
                );

      if(retVal == TCPIP_OK)
      {
        /* returned buffer size must be equal to requested buffer size */
        TCPIP_INVARIANT_ASSERT(ethTxHandle.bufferLength == fragmentLength, TCPIP_INTERNAL_API_ID);

        retVal = TcpIp_Ip_Frag_copyTxData
                    (
                        copyDataFunctorPtr, fragInfoPtr->payloadSumPtr,
                        &ethTxHandle, dataPtr, copyOffset,
                        ipParameterPtr->calculateChecksum
                    );
        /* Even with retVal != TCPIP_OK, the transmission cannot be aborted, because there is no API to
         * discard the TxBuffer. The only way to solve this situation is to send a dummy fragment packet.
         */
        if (retVal != TCPIP_OK)
        {
          /* No UDP packet, just IP header */
          fragHdr.IPisFragment= FALSE;
          fragHdr.IPmoreFlag = FALSE;
          fragHdr.IPoffset = 0U;
          ethTxHandle.bufferLength = 0U;
        }
        {
          /* payload fragments are actually raw data over IP */
          /* only overwrite retVal, if no prior error -> return 1st error */
          TcpIp_ReturnType result =
            transmit /*TcpIp_IpV4_transmit,TcpIp_IpV6_transmit*/
                (
                  ipParameterPtr,
                  &fragHdr,
                  &ethTxHandle,
                  remoteAddrHandle,
                  remoteAddrPtr,
                  localAddrId
                );

          if(retVal == TCPIP_OK)
          {
            retVal = result;
          }
        }
      }
      copyOffset += fragmentLength;  /* for copying, we exclude the UDP header */
      fragHdr.IPoffset += fragmentLength; /* for transmission, we include the UDP header */
      ++frgIdx;
    } while ((retVal == TCPIP_OK) && (frgIdx<frgCnt));
  }

  return retVal;
}

#endif /* ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_OUTOFORDER ) */

#if ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_INORDER )

STATIC FUNC(TcpIp_ReturnType, TCPIP_CODE) TcpIp_Ip_Frag_copyDataInOrder
(
  P2VAR(TcpIp_Ip_CopyDataFunctorType, AUTOMATIC, TCPIP_APPL_DATA) copyDataFunctorPtr,
  P2VAR(TcpIp_Ip_IpParameterType, AUTOMATIC, TCPIP_APPL_DATA) ipParameterPtr,
  P2VAR(TcpIp_Ip_Frag_CopyInfoType, AUTOMATIC, TCPIP_APPL_DATA) fragInfoPtr,
  P2VAR(TcpIp_Ip_EthTxHandleType, AUTOMATIC, TCPIP_APPL_DATA) ethTxHandlePtr,
  P2CONST(TcpIp_Ip_AddrType, AUTOMATIC, TCPIP_APPL_DATA) addrPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  P2VAR(uint16, AUTOMATIC, TCPIP_APPL_DATA) numFragToTransmitPtr
)
{
#if (TCPIP_IP_DUALSTACK == STD_ON)
  uint8 const page = TCPIP_IP_DOMAIN_GET(addrPtr->remoteAddrPtr->domain);
#endif /* (TCPIP_IP_DUALSTACK == STD_ON) */
  /* Deviation MISRAC2012-9 <+2> */
  TcpIp_Ip_provideTxBuffer_FctPtrType const provideTxBuffer= TCPIP_IP_PROVIDETXBUFFER(page);
  TcpIp_ReturnType retVal= TCPIP_E_NOT_OK;
  uint16 frgIdx = 0u;
  /* for copying, we need to be aware of the UDP header
   * for transmission we need to keep track of the fragment offset to be set in the IP header
     (the UDP header is included there) */
  uint16 startOffset = fragInfoPtr->upLayerDataOffset; /* leave space for the header in 1st fragment */
  uint16 copyOffset = 0U;
  uint16 fragmentLength = fragInfoPtr->fragmentLength;
  boolean IPisFragment = (fragInfoPtr->fragmentCount > 1) ? TRUE : FALSE;

  do
  {
    if (frgIdx==(fragInfoPtr->fragmentCount - 1U)) /* last iteration */
    {
      /* to be allocated/copied/transmitted */
      fragmentLength = fragInfoPtr->fragmetLastLength;
    }

    /* payload fragments are actually raw data over IP */
    retVal = provideTxBuffer /* TcpIp_IpV4_provideTxBuffer, TcpIp_IpV6_provideTxBuffer */
              (
                  ipParameterPtr,
                  &ethTxHandlePtr[frgIdx],
                  fragmentLength, /* request fragmentLength, get totalLength */
                  addrPtr->localAddrId,
                  IPisFragment
              );

    if(retVal == TCPIP_OK)
    {
      TcpIp_Ip_EthTxHandleType copyEthTxHandle;

      /* returned buffer size must be equal to requested buffer size */
      TCPIP_POSTCONDITION_ASSERT
         (ethTxHandlePtr[frgIdx].bufferLength == fragmentLength, TCPIP_INTERNAL_API_ID);

      copyEthTxHandle.bufferPtr = &(ethTxHandlePtr[frgIdx].bufferPtr[startOffset]);
      copyEthTxHandle.bufferLength = ethTxHandlePtr[frgIdx].bufferLength - startOffset;

      retVal = TcpIp_Ip_Frag_copyTxData
                (
                    copyDataFunctorPtr, fragInfoPtr->payloadSumPtr, /* sets totalLength= #copied */
                    &copyEthTxHandle, dataPtr,
                    copyOffset,
                    ipParameterPtr->calculateChecksum
                );
      TCPIP_INVARIANT_ASSERT((retVal!=TCPIP_OK) ||
          ((((copyEthTxHandle.bufferLength % 8u) == 0u) || (frgIdx == (fragInfoPtr->fragmentCount-1U))) &&
           (copyEthTxHandle.bufferLength == (ethTxHandlePtr[frgIdx].bufferLength - startOffset))),
          TCPIP_INTERNAL_API_ID);

      ++frgIdx;  /* indicate that this 'provided' buffer still needs to be sent ICE */
    }
    copyOffset += fragmentLength - startOffset; /* for copying, we exclude the UDP header */
    startOffset= 0;                                         /* but only in the 1st fragment */
  } while ((retVal==TCPIP_OK) && (frgIdx < fragInfoPtr->fragmentCount));

  /* if we didn't allocate all buffers due to errors, we only need to 'transmit' those allocated */
  *numFragToTransmitPtr = frgIdx;

  return retVal;
}

#endif /* ( TCPIP_IP_FRAGMENT_TX_ENABLED == TCPIP_IP_FRAGMENT_TX_INORDER ) */

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* (TCPIP_IPV6_ENABLED == STD_ON) || (TCPIP_IPV4_ENABLED == STD_ON) */

/*==================[end of file]===========================================*/
