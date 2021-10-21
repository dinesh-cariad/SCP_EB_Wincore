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
 * MISRAC2012-2) Deviated Rule: 11.3 (required)
 * A cast shall not be performed between a pointer to object type and a pointer to a different
 * object type.
 *
 * Reason:
 * 1)Init time allocation requires a cast to the type of the allocated object.
 * 2)In order to implement the algorithm without extra bookkeeping data as described in RFC 815,
 * the hole descriptors have to lie within the buffer, thus the type cast is necessary.
 *
 * MISRAC2012-7) Deviated Rule: 11.4 (required)
 * A conversion should not be performed between a pointer to object and an integer type.
 *
 * Reason:
 * To check alignment, the pointer has to be converted to integer, so we can inspect the lower bits
 * using bit arithmetic.
 *
 */

/*==================[inclusions]============================================*/

#include <TSMem.h>                /* memory functions */
#include <SchM_TcpIp.h>

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

#include <TcpIp_Cfg.h>    /* based on configuration, we may skip the rest */
#if (TCPIP_IPV6_ENABLED == STD_ON) || (TCPIP_IPV4_ENABLED == STD_ON)
#if ( TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON )
#include <TcpIp_Ip_Priv.h>
#include <TcpIp_Trace.h>          /* debug and trace macros; must be last */

/*==================[macros]================================================*/

/* \brief Check if timeout has passed. */
#define TCPIP_IP_FRAG_CHECK_TIMEOUT(timer, timestamp) \
  (((timer) - (timestamp)) > TcpIp_PBcfgPtr->ipFragConfig.reassTimeout)

#define TCPIP_IP_FRAG_EOL ((uint32)~((uint32)0u))

/** \brief clear (deallocate) a reassembly buffer.
 * \param[in,out] bufferPtr The pointer to the reassembly buffer.
 * */
#define TcpIp_Ip_Frag_bufferClear(bufferPtr)                             \
  do                                                                     \
  {                                                                      \
    P2VAR(TcpIp_Ip_Frag_RxBufferType, AUTOMATIC, TCPIP_APPL_DATA) bPtr=  \
      (bufferPtr);                                                       \
    bPtr->bUsed= FALSE;                                                  \
  } while(0u)

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

/** \brief allocate a reassembly buffer.
 * \param[in,out] bufferPtr The pointer to the reassembly buffer.
 * \param[in] channelIdPtr  Pointer to the channel ID (srcIp,dstIp,IpID)
 *                          For IpV4, IpId includes the protocol in the high bits16-23.
 * \param[in] timeStamp     The timestamp of the 'reception' of the fragment.
 */
STATIC FUNC(void, TCPIP_CODE) TcpIp_Ip_Frag_bufferAllocate
(
  P2VAR(TcpIp_Ip_Frag_RxBufferType, AUTOMATIC, TCPIP_APPL_DATA) bufferPtr,
  P2CONST(TcpIp_Ip_Frag_ChannelIdType, AUTOMATIC, TCPIP_APPL_DATA) channelIdPtr,
  uint32 timeStamp
);

/** \brief update the hole list for insertion of the current fragment.
 * \param[in] bufferDataPtr   The buffer data, where the hole descriptors reside
 *                            within the first 8 bytes of each hole.
 * \param[in] moreFrags       Is the current fragment the last one? Add hole descriptor aterwards?
 * \param[in] iFragmentFirst  First byte's offset within \a bufferDataPtr.
 * \param[in] iFragmentLast   Last byte's offset within \a bufferDataPtr.
 * \param[in] prevHolePtr     Pointer to the previous hole (or dummy @start of list).
 * \param[in] currentHolePtr  Pointer to the current hole, which will be filled.
 * \param[in] nextHoleInPtr   Pointer to the following hole or \a NULL_PTR, if non-existant.
 */
STATIC FUNC(void, TCPIP_CODE)  TcpIp_Frag_adjustHoles
(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) bufferDataPtr,
  boolean moreFrags,
  uint16 iFragmentFirst,
  uint16 iFragmentLast,
  P2VAR(TcpIp_Ip_Frag_HoleDescriptorType, AUTOMATIC, TCPIP_APPL_DATA) prevHolePtr,
  P2VAR(TcpIp_Ip_Frag_HoleDescriptorType, AUTOMATIC, TCPIP_APPL_DATA) currentHolePtr,
  P2VAR(TcpIp_Ip_Frag_HoleDescriptorType, AUTOMATIC, TCPIP_APPL_DATA) nextHoleInPtr
);

/** \brief insert fragment described by \a frag into the re-assembly buffer \a currentBuffer.
 *
 * SWS_TCPIP_00054 The TcpIp shall be able to reassemble incoming datagrams that are fragmented according to IETF RFC 815
 * DEVIATIONS: according to IPV6's RFC5722, overlaps are disallowed.
 *
 * \param bufferPtr           Reassembly buffer, i.e.: where to insert
 * \param fragPtr             Fragment header abstraction
 *
 * \return                  TCPIP standard return code.
 * \retval TCPIP_IP_RX_OK               Success, reassembly buffer complete
 * \retval TCPIP_IP_RX_NOT_OK           Reassembly buffer incomplete
 * \retval TCPIP_IP_RX_FRAGMENT_OFFSET  Offset too large for configured reassembly buffer size
 * \retval TCPIP_IP_RX_FRAGMENT_LENGTH  Length not 0 modulo 8 for non-tail fragment
 * \retval TCPIP_IP_RX_FRAGMENT_OVERLAP Overlapping fragments
 *
 */
STATIC FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE) TcpIp_Ip_Frag_insert
(
  P2VAR(TcpIp_Ip_Frag_RxBufferType, AUTOMATIC, TCPIP_APPL_DATA) bufferPtr,
  P2CONST(TcpIp_Ip_Frag_RxHeaderType, AUTOMATIC, TCPIP_APPL_DATA) fragPtr
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/


#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/** \brief The buffers used for fragmented-datagram reassembly. */
STATIC P2VAR(TcpIp_Ip_Frag_RxBufferType, TCPIP_VAR_CLEARED, TCPIP_APPL_DATA) TcpIp_Ip_Frag_RxBuffers;

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

#define TCPIP_START_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

STATIC VAR(uint32, TCPIP_VAR) TcpIp_Ip_Frag_timer = 0U;

#define TCPIP_STOP_SEC_VAR_INIT_32
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/


#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC(uint32, TCPIP_CODE) TcpIp_Ip_Reass_init
(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) TcpIp_Ip_Frag_MemReserved,
  uint32 startOffset
)
{
  uint32 n= startOffset;
  /* Deviation MISRAC2012-7 <+2> */
  TCPIP_PRECONDITION_ASSERT(
      TCPIP_ISALIGNED(&TcpIp_Ip_Frag_MemReserved[n], sizeof(uint32)),
      TCPIP_SID_INIT);
  {
    uint16 i=0;
    uint32 const bufSize= TCPIP_ROUNDUP(TCPIP_IP_MAX_HEADERS_LENGTH + TcpIp_PBcfgPtr->ipFragConfig.reassBufferSize, sizeof(uint32));
    /* Deviation MISRAC2012-2 */
    TcpIp_Ip_Frag_RxBuffers= (TcpIp_Ip_Frag_RxBufferPtrType) &TcpIp_Ip_Frag_MemReserved[n];
    n+= TcpIp_PBcfgPtr->ipFragConfig.reassBufferCount * sizeof(TcpIp_Ip_Frag_RxBufferType);
    /* Deviation MISRAC2012-7 <+2> */
    TCPIP_POSTCONDITION_ASSERT(
        TCPIP_ISALIGNED(&TcpIp_Ip_Frag_MemReserved[n], sizeof(uint32)),
        TCPIP_SID_INIT);
    for (i=0; i < TcpIp_PBcfgPtr->ipFragConfig.reassBufferCount; ++i)
    {
      TcpIp_Ip_Frag_RxBuffers[i].bufferPtr= &TcpIp_Ip_Frag_MemReserved[n];
      n+= bufSize;
      TCPIP_POSTCONDITION_ASSERT(n<=TCPIP_IP_FRAGMENT_MEMRESERVED, TCPIP_SID_INIT);  /* TCPIP_E_NOBUFS */
      TcpIp_Ip_Frag_bufferClear(&(TcpIp_Ip_Frag_RxBuffers[i]));
    }
  }
  return n;
}


TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ip_Frag_mainFunction(void)
{
  ++TcpIp_Ip_Frag_timer;
}


TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Ip_Frag_returnBuffer
(
  P2VAR(TcpIp_Ip_Frag_RxBufferType, AUTOMATIC, TCPIP_APPL_DATA) bufferPtr
)
{
  DBG_TCPIP_IP_FRAG_RETURNBUFFER_ENTRY(bufferPtr);
  TcpIp_Ip_Frag_bufferClear(bufferPtr);
  DBG_TCPIP_IP_FRAG_RETURNBUFFER_EXIT(bufferPtr);
}


TS_MOD_PRIV_DEFN FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE)  TcpIp_Ip_Frag_reassemble
(
  P2VAR(TcpIp_Ip_Frag_RxBufferPtrType, AUTOMATIC, TCPIP_APPL_DATA) bufferPtrPtr,
  P2VAR(TcpIp_Ip_Frag_RxHeaderType, AUTOMATIC, TCPIP_APPL_DATA) frag
)
{
  TcpIp_Ip_RxReturnType result= TCPIP_IP_RX_NOT_OK;
  /* grab a local non-volatile copy (~reception time) of the volatile timer counter */
  uint32 const timer= TcpIp_Ip_Frag_timer;
  uint16 maxBuffers= TcpIp_PBcfgPtr->ipFragConfig.reassBufferCount;
  uint16 iPos = 0u;
  *bufferPtrPtr = NULL_PTR;

  DBG_TCPIP_IP_FRAG_REASSEMBLE_ENTRY(bufferPtrPtr, frag);

  /* RFC6946: complete (atomic) fragments are handled as packets -> no collision, skip search_existing */
  if ((frag->offset==0U) && (frag->moreFrags==FALSE))
  {
    iPos= maxBuffers;
  }

  /* search allocated buffer */
  for ( ; iPos < maxBuffers; ++iPos)
  {
    if (TcpIp_Ip_Frag_RxBuffers[iPos].bUsed == TRUE)
    {
      if (TS_MemCmp32_NoCheck(&TcpIp_Ip_Frag_RxBuffers[iPos].channelId,
                              &frag->channelId,
                              sizeof(TcpIp_Ip_Frag_ChannelIdType)) == E_OK)
      {
        /* Ignore timeout for now - we found the buffer to insert the current fragment.
         * By inserting the fragment, we have more data to send back with the ICMP time_exceeded msg.
         * Plus we don't leave a stale fragment. */
        break;
      }
    }
  }

  /* 2nd chance: search for free or timed-out buffer & reuse */
  if (iPos >= maxBuffers)
  {
    /* new fragment */
    /* now check for free buffers */
    /* !LINKSTO TcpIp.rfc815.sec7.pf1,1 */
    for (iPos = 0; iPos < maxBuffers; ++iPos)
    {
      uint32 const timestamp= TcpIp_Ip_Frag_RxBuffers[iPos].timestamp;
      /* !LINKSTO TcpIp.rfc815.sec7.pf2,1 */
      if (TCPIP_IP_FRAG_CHECK_TIMEOUT(timer, timestamp))
      {
        /* free the outdated buffer & reuse it immediately */
        TcpIp_Ip_Frag_bufferClear(&TcpIp_Ip_Frag_RxBuffers[iPos]);
        /* fall through to free buffer (re-)allocation. */
      }
      /* reusing a free or timed-out buffer */
      if ((TcpIp_Ip_Frag_RxBuffers[iPos].bUsed) == FALSE)
      {
        TcpIp_Ip_Frag_bufferAllocate(&TcpIp_Ip_Frag_RxBuffers[iPos], &frag->channelId, timer);
        break;
      }
    }
    /* found free buffer if loop unfinished, i.e.: iPos < reassBufferCount */
  }

  if (iPos >= maxBuffers)
  {
    /* neither pre-existing, nor timed-out, nor free buffer left */
    result= TCPIP_IP_RX_FRAGMENT_OUT_OF_BUFFERS;

#if ( TCPIP_DEV_ERROR_DETECT == STD_ON )
    TCPIP_REPORT_ERROR(TCPIP_SID_RXINDICATION, TCPIP_E_NOBUFS);
#endif /* ( TCPIP_DEV_ERROR_DETECT == STD_ON ) */
  }
  else
  { /* we get here with found nonempty buffer, or (found free or (timed-out & cleared)),
       then (re-)allocated buffer */
    P2VAR(TcpIp_Ip_Frag_RxBufferType, AUTOMATIC, TCPIP_APPL_DATA) bufferPtr=
      &TcpIp_Ip_Frag_RxBuffers[iPos];

    /* we do have a new or possibly-timed-out old buffer */

    /* !LINKSTO TcpIp.rfc815.sec3.pf2,1 */
    *bufferPtrPtr= bufferPtr;
    /* insert still useful fragment */
    result= TcpIp_Ip_Frag_insert(bufferPtr, frag);

    /* We inserted the fragment -> now check if it's a timeout */
    /* !LINKSTO TcpIp.rfc1122.InitialFragmentTimeout.Fixed,1 */
    if (TCPIP_IP_FRAG_CHECK_TIMEOUT(timer, bufferPtr->timestamp))
    {
      /* we return the (partially) reassembled buffer,
       * but make sure it will be discarded after handling the timeout */
      result= TCPIP_IP_RX_FRAGMENT_TIMEOUT;
    }
  }

  DBG_TCPIP_IP_FRAG_REASSEMBLE_EXIT(result, bufferPtrPtr, frag);
  return result;
}


/*==================[internal function definitions]=========================*/

STATIC FUNC(void, TCPIP_CODE)  TcpIp_Frag_adjustHoles
(
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) bufferDataPtr,
  boolean moreFrags,
  uint16 iFragmentFirst,
  uint16 iFragmentLast,
  P2VAR(TcpIp_Ip_Frag_HoleDescriptorType, AUTOMATIC, TCPIP_APPL_DATA) prevHolePtr,
  P2VAR(TcpIp_Ip_Frag_HoleDescriptorType, AUTOMATIC, TCPIP_APPL_DATA) currentHolePtr,
  P2VAR(TcpIp_Ip_Frag_HoleDescriptorType, AUTOMATIC, TCPIP_APPL_DATA) nextHoleInPtr
)
{
  boolean bHoleInFront = FALSE;
  boolean bHoleInBack = FALSE;
  P2VAR(TcpIp_Ip_Frag_HoleDescriptorType, AUTOMATIC, TCPIP_APPL_DATA) nextHolePtr= nextHoleInPtr;
  const uint16 iHoleLast = currentHolePtr->iLast;  /* remember */

  if (iFragmentFirst > currentHolePtr->iFirst) /* !LINKSTO TcpIp.rfc815.sec3.st5,1 */
  {
    /* reusing (truncating) the current hole implicitly deletes it while creating the new one */
    /* new hole in front of fragment */
    /* !LINKSTO TcpIp.rfc815.sec3.st1,1 */
    bHoleInFront = TRUE;
    /* adapt new last byte */
    currentHolePtr->iLast = iFragmentFirst - 1u;
  }
  if ((iFragmentLast < iHoleLast) && (moreFrags == TRUE)) /* !LINKSTO TcpIp.rfc815.sec3.st6,1 */
  {
    /* new hole after fragment, except if moreFrags said 'no' */
    /* either, there is already a hole behind this position
     * ->  store its start to reconnect after creation of the new hole
     * otherwise store end-of-list indicator */
    uint32 iStartPrevNextHole = (nextHolePtr != NULL_PTR) ? ((uint32) nextHolePtr->iFirst) : TCPIP_IP_FRAG_EOL;
    bHoleInBack = TRUE;

    /* Create the new hole */
    /* Deviation MISRAC2012-2 */
    nextHolePtr =
      (P2VAR(TcpIp_Ip_Frag_HoleDescriptorType, AUTOMATIC, TCPIP_APPL_DATA))
        &(bufferDataPtr[iFragmentLast + 1u]);
    nextHolePtr->iFirst = iFragmentLast + 1u;
    nextHolePtr->iLast = iHoleLast;

    /* append remaining hole-list */
    nextHolePtr->iNext = iStartPrevNextHole;

    /* re-chain the holes */
    if (bHoleInFront == TRUE)
    {
      /* connect the hole in front with the current one */
      currentHolePtr->iNext = nextHolePtr->iFirst;
    }
    else
    {
      /* delete current hole by chaining predecessor & successor */
      /* !LINKSTO TcpIp.rfc815.sec3.st4,1 */
      prevHolePtr->iNext = nextHolePtr->iFirst;
    }
  }
  if (!((bHoleInFront == TRUE) || (bHoleInBack == TRUE)))
  {
    /* filled a complete gap */
    if (nextHolePtr != NULL_PTR)
    {
      /* there is another gap later on */
      /* chain to previous hole */
      prevHolePtr->iNext = nextHolePtr->iFirst;
    }
    else
    { /* no following gap */
      /* Delete pointer to next */
      prevHolePtr->iNext = TCPIP_IP_FRAG_EOL;
    }
  }
}


STATIC FUNC(TcpIp_Ip_RxReturnType, TCPIP_CODE)  TcpIp_Ip_Frag_insert
(
  P2VAR(TcpIp_Ip_Frag_RxBufferType, AUTOMATIC, TCPIP_APPL_DATA) bufferPtr,
  P2CONST(TcpIp_Ip_Frag_RxHeaderType, AUTOMATIC, TCPIP_APPL_DATA) fragPtr
)
{
  TcpIp_Ip_RxReturnType result = TCPIP_IP_RX_NOT_OK;

  TCPIP_PRECONDITION_ASSERT((fragPtr->offset & 3u) == 0u, TCPIP_INTERNAL_API_ID);

  /* !LINKSTO TcpIp.rfc1122.Reassembly.EMTU_R,1 */
  if ((((uint32)fragPtr->offset)+fragPtr->lenDataByte) >= TcpIp_PBcfgPtr->ipFragConfig.reassBufferSize)
  {
    result= TCPIP_IP_RX_FRAGMENT_OFFSET;
  }
  else if (( (fragPtr->moreFrags==TRUE) && ((fragPtr->lenDataByte % 8u) != 0u) ) ||
           (fragPtr->lenDataByte == 0u))  /* zero length is also disallowed */
  {
    result= TCPIP_IP_RX_FRAGMENT_LENGTH;
  }
  else
  {
    P2VAR(TcpIp_Ip_Frag_HoleDescriptorType, AUTOMATIC, TCPIP_APPL_DATA) prevHolePtr =
      &bufferPtr->first_hole;
    P2VAR(TcpIp_Ip_Frag_HoleDescriptorType, AUTOMATIC, TCPIP_APPL_DATA) currentHolePtr = NULL_PTR;
    boolean bFound = FALSE;
    /* get fragment's first and last position  */
    uint16 const iFragmentFirst = fragPtr->offset;
    uint16 const iFragmentLast  = iFragmentFirst + (fragPtr->lenDataByte - 1u);
    /* plain payload data w/o header */
    P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) bufferDataPtr=
      &bufferPtr->bufferPtr[TCPIP_IP_MAX_HEADERS_LENGTH];

    /* !LINKSTO TcpIp.rfc815.sec3.st7,1 */
    while (prevHolePtr->iNext != TCPIP_IP_FRAG_EOL)
    {
      /* get first/next hole */
      /* Deviation MISRAC2012-2 */   /* !LINKSTO TcpIp.rfc815.sec3.st1,1 */
      currentHolePtr =
        (P2VAR(TcpIp_Ip_Frag_HoleDescriptorType, AUTOMATIC, TCPIP_APPL_DATA))
          &bufferDataPtr[prevHolePtr->iNext];

      /* since the deviation doesn't allow overlaps, steps 2 & 3 spell out as is_contained */
      if ( (iFragmentFirst >= currentHolePtr->iFirst) &&     /* !LINKSTO TcpIp.EB.rfc815.sec3.st2,2 */
           (iFragmentLast <= currentHolePtr->iLast)      )   /* !LINKSTO TcpIp.EB.rfc815.sec3.st3,2 */
      {
        /* found the right hole */
        bFound = TRUE;
        break;
      }
      /* hole didn't match, get next hole */
      prevHolePtr = currentHolePtr;
    }

    if (bFound)
    {
      P2VAR(TcpIp_Ip_Frag_HoleDescriptorType, AUTOMATIC, TCPIP_APPL_DATA) nextHolePtr = NULL_PTR;

      /* We have a valid fitting fragment: adjust total size with last pos of fragment */
      if (bufferPtr->size <= iFragmentLast)
      {
        bufferPtr->size = ((uint32) iFragmentLast) + 1u;
      }

      /* if available already get next hole */
      if (currentHolePtr->iNext != TCPIP_IP_FRAG_EOL)
      {
        /* Deviation MISRAC2012-2 */
        nextHolePtr =
          (P2VAR(TcpIp_Ip_Frag_HoleDescriptorType, AUTOMATIC, TCPIP_APPL_DATA))
            &bufferDataPtr[currentHolePtr->iNext];
      }

      TcpIp_Frag_adjustHoles(bufferDataPtr, fragPtr->moreFrags, iFragmentFirst, iFragmentLast,
                             prevHolePtr, currentHolePtr, nextHolePtr);
      if (bufferPtr->first_hole.iNext == TCPIP_IP_FRAG_EOL) /* no more gaps */
      {
        /* assert(bufferPtr->first_hole->iNext = bufferPtr->first_hole->iFirst); */
        /* no next hole, no previous hole */
        /* ready to return buffer */
        /* !LINKSTO TcpIp.rfc815.sec3.st8,1 */
        result = TCPIP_IP_RX_OK;
      }

      /* copy current payload into reassembling buffer, the correct header will be copied later. */
      /* this potentially overwrites the current hole indicator */
      /* !LINKSTO TcpIp.rfc815.sec4.pf3,1 */
      TS_MemCpy(&bufferDataPtr[iFragmentFirst], fragPtr->dataPtr, fragPtr->lenDataByte);
    }
    else
    { /* !bFound */
      /* we ran out of holes with no one fitting: frag must have been overlapping (rfc5722) */
      /* signal an error to the concrete IP layer, to be handled there. */
      result= TCPIP_IP_RX_FRAGMENT_OVERLAP;
    }
  }
  return result;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Ip_Frag_bufferAllocate
(
  P2VAR(TcpIp_Ip_Frag_RxBufferType, AUTOMATIC, TCPIP_APPL_DATA) bufferPtr,
  P2CONST(TcpIp_Ip_Frag_ChannelIdType, AUTOMATIC, TCPIP_APPL_DATA) channelIdPtr,
  uint32 timeStamp
)
{
  /* Deviation MISRAC2012-2 */
  P2VAR(TcpIp_Ip_Frag_HoleDescriptorType, AUTOMATIC, TCPIP_APPL_DATA) first_hole=
    (P2VAR(TcpIp_Ip_Frag_HoleDescriptorType, AUTOMATIC, TCPIP_APPL_DATA))
       &bufferPtr->bufferPtr[TCPIP_IP_MAX_HEADERS_LENGTH];

  bufferPtr->bUsed= TRUE;
  bufferPtr->size= 0u;
  bufferPtr->timestamp= timeStamp;
  bufferPtr->channelId= *channelIdPtr;
  bufferPtr->first_hole.iFirst= 0u;
  bufferPtr->first_hole.iLast= 0u;
  bufferPtr->first_hole.iNext= 0u;
  bufferPtr->headerStart = 0u;
  first_hole->iFirst= 0u;

  /* !LINKSTO TcpIp.rfc1122.Reassembly.EMTU_R,1 */
  first_hole->iLast= (uint16) (TcpIp_PBcfgPtr->ipFragConfig.reassBufferSize - 1u);
  first_hole->iNext= TCPIP_IP_FRAG_EOL;
}

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#endif /* TCPIP_IP_FRAGMENT_RX_ENABLED == STD_ON */

#else
  TS_PREVENTEMPTYTRANSLATIONUNIT
#endif /* (TCPIP_IPV6_ENABLED == STD_ON) || (TCPIP_IPV4_ENABLED == STD_ON) */

/*==================[end of file]===========================================*/
