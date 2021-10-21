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

/*==================[inclusions]============================================*/

#include <TSMem.h>                  /* Memory functions */
#include <TcpIp_guards.h>           /* After foreign/API/Cfg; before internal/private */
#include <TcpIp_Tcp_Priv.h>         /* Own SW-unit interface file */

/*==================[macros]================================================*/

#if(TCPIP_TCP_OUTOFORDER_RECEPTION_ENABLED == STD_ON)

#define TCPIP_TCP_RINGBUFFER_OFFSET_TO_INDEX_WRAP(offset, baseOffset)                              \
  ((((offset) - (baseOffset)) >= TCPIP_TCP_RINGBUFFER_SIZE)                                        \
     ? ((offset) - (baseOffset) -TCPIP_TCP_RINGBUFFER_SIZE)                                        \
     : ((offset) - (baseOffset)))

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxOutOfOrder_passToUpperLayer
(
  TcpIp_SocketIdType socketId,
  P2CONST(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) directDataPtr,
  uint16 directDataLen,
  uint32 directSequenceNum
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxOutOfOrder_removeHole
(
  uint16 holeListIndex,
  uint8 holeIndex,
  uint8 holePredecessorIndex
);

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_rxOutOfOrder_addHole
(
  uint16 holeListIndex,
  uint32 startOffset,
  uint32 endOffset
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxOutOfOrder_bufferSegment
(
  uint16 holeListIndex,
  uint32 startOffset,
  uint32 endOffset,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr
);

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxOutOfOrder_copyToBuffer
(
  uint32 startOffset,
  uint32 endOffset,
  uint32 baseOffset,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) bufferDataPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) srcDataPtr
);

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

/*==================[internal constants]====================================*/

/*==================[internal data]=========================================*/

#define TCPIP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

STATIC VAR(TcpIp_Tcp_RingBufferInfoType,
           TCPIP_VAR) TcpIp_Tcp_ringBufferInfo[TCPIP_TCP_RINGBUFFER_COUNT];
STATIC VAR(TcpIp_Tcp_RingBufferType, TCPIP_VAR) TcpIp_Tcp_ringBuffer[TCPIP_TCP_RINGBUFFER_COUNT];
STATIC VAR(TcpIp_Tcp_HoleListInfoType,
           TCPIP_VAR) TcpIp_Tcp_holeListInfo[TCPIP_TCP_RINGBUFFER_COUNT];
STATIC VAR(TcpIp_Tcp_HoleListType, TCPIP_VAR) TcpIp_Tcp_holeList[TCPIP_TCP_RINGBUFFER_COUNT];

#define TCPIP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <TcpIp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function definitions]=========================*/

#define TCPIP_START_SEC_CODE
#include <TcpIp_MemMap.h>

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxOutOfOrder_Init
(
  void
)
{
  /* Iterate all hole lists */
  uint16 holeListIndex = 0U;
  do
  {
    /* Link the entries */
    uint8 holeIndex;
    for(holeIndex = 0U; holeIndex < (TCPIP_TCP_HOLE_LIST_SIZE - 1U); ++holeIndex)
    {
      TcpIp_Tcp_holeList[holeListIndex].entries[holeIndex].nextHoleIndex = holeIndex + 1U;
    }
    TcpIp_Tcp_holeList[holeListIndex].entries[TCPIP_TCP_HOLE_LIST_SIZE - 1U].nextHoleIndex =
      TCPIP_TCP_HOLE_LIST_INDEX_INVALID;

    /* Set the head indices of the lists */
    TcpIp_Tcp_holeListInfo[holeListIndex].freeHoleListHeadIndex = 0U;
    TcpIp_Tcp_holeListInfo[holeListIndex].usedHoleListHeadIndex = TCPIP_TCP_HOLE_LIST_INDEX_INVALID;
    ++holeListIndex;
  } while(holeListIndex < TCPIP_TCP_RINGBUFFER_COUNT);
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxOutOfOrder_Receive
(
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr,
  uint16 dataLen,
  uint32 sequenceNum,
  TcpIp_SocketIdType socketId,
  P2CONST(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr
)
{
  uint32 const sequenceNumNext = ctrlBlockPtr->recv_nxt;
  uint16 const recvWndSize = ctrlBlockPtr->recv_wnd;
  uint16 const limitingSize =
    (TCPIP_TCP_RINGBUFFER_SIZE > recvWndSize) ? recvWndSize : TCPIP_TCP_RINGBUFFER_SIZE;
  uint32 const limitingEndOffset = sequenceNumNext + limitingSize - 1U;
  uint16 toBufferDataLen = dataLen;
  uint32 endOffset = sequenceNum + dataLen - 1U;

  /* If data does not completely fit into the receive window or out-of-order buffer */
  if(TCPIP_TCP_IS_GREATER_THAN(endOffset, limitingEndOffset))
  {
    /* Truncate at the end */
    toBufferDataLen -= (uint16)(endOffset - limitingEndOffset);
    endOffset = limitingEndOffset;
  }

  /* Check if this is the first out-of-order segment */
  if(ctrlBlockPtr->ringbuffer_info_index == TCPIP_TCP_RINGBUFFER_INFO_INDEX_INVALID)
  {
    /* If buffer size is large enough */
    if(TCPIP_TCP_IS_LOWER_THAN(sequenceNum, sequenceNumNext + TCPIP_TCP_RINGBUFFER_SIZE))
    {
      /* Search for a free buffer */
      uint16 bufferIndex = 0U;
      do
      {
        /* If we found a free buffer */
        if(TcpIp_Tcp_holeListInfo[bufferIndex].usedHoleListHeadIndex ==
           TCPIP_TCP_HOLE_LIST_INDEX_INVALID)
        {
          /* Initialize buffer info */
          uint16 const copyIndex = (uint16)(sequenceNum - sequenceNumNext);
          CONSTP2VAR(TcpIp_Tcp_RingBufferInfoType, AUTOMATIC, TCPIP_APPL_DATA) bufferInfoPtr =
            &TcpIp_Tcp_ringBufferInfo[bufferIndex];
          bufferInfoPtr->writeOffset = sequenceNum + toBufferDataLen;
          bufferInfoPtr->baseOffset = sequenceNumNext;

          TCPIP_PRECONDITION_ASSERT(copyIndex < TCPIP_TCP_RINGBUFFER_SIZE, TCPIP_INTERNAL_API_ID);

          /* Acquire buffer */
          ctrlBlockPtr->ringbuffer_info_index = bufferIndex;

          /* Add hole to hole list */
          (void) TcpIp_Tcp_rxOutOfOrder_addHole(bufferIndex, sequenceNumNext, sequenceNum - 1U);

          /* Copy data to buffer */
          TS_MemCpy(
            &(TcpIp_Tcp_ringBuffer[bufferIndex].data[copyIndex]), dataPtr, toBufferDataLen);
          break;
        }
        ++bufferIndex;
      } while(bufferIndex < TCPIP_TCP_RINGBUFFER_COUNT);
    }
  }
  /* If there is already data buffered */
  else
  {
    uint16 const bufferIndex = ctrlBlockPtr->ringbuffer_info_index;
    uint32 startOffset;

    TCPIP_PRECONDITION_ASSERT( bufferIndex < TCPIP_TCP_RINGBUFFER_COUNT, TCPIP_INTERNAL_API_ID );

    /* Only consider data not already passed to upper layer */
    if(TCPIP_TCP_IS_LOWER_THAN(sequenceNum, sequenceNumNext))
    {
      uint16 const difference = (uint16)(sequenceNumNext - sequenceNum);
      startOffset = sequenceNumNext;
      dataPtr += difference;
      sequenceNum += difference;
      dataLen -= difference;
    }
    else
    {
      startOffset = sequenceNum;
    }

    /* Only proceed if there is data to process */
    if(TCPIP_TCP_IS_GREATER_OR_EQUAL(endOffset, startOffset))
    {
      /* Update hole list and copy missing data to buffer */
      TcpIp_Tcp_rxOutOfOrder_bufferSegment(bufferIndex, startOffset, endOffset, dataPtr);

      /* If first byte of lowest hole is filled */
      if(sequenceNum == sequenceNumNext)
      {
        /* If data length exceeds window size, truncate at the end */
        dataLen = (dataLen > recvWndSize) ? recvWndSize : dataLen;

        /* Pass buffered data to upper layer */
        TcpIp_Tcp_rxOutOfOrder_passToUpperLayer(
          socketId, socketPtr, ctrlBlockPtr, dataPtr, dataLen, sequenceNum);
      }
    }
  }
}

TS_MOD_PRIV_DEFN FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxOutOfOrder_reset
(
  uint16 bufferIndex
)
{
  uint8 holeIndex;

  /* Function is only called with valid buffer index */
  TCPIP_PRECONDITION_ASSERT((bufferIndex != TCPIP_TCP_RINGBUFFER_INFO_INDEX_INVALID),
                            TCPIP_INTERNAL_API_ID);

  /* Reset link of the entries */
  for(holeIndex = 0U; holeIndex < (TCPIP_TCP_HOLE_LIST_SIZE - 1U); ++holeIndex)
  {
    TcpIp_Tcp_holeList[bufferIndex].entries[holeIndex].nextHoleIndex = holeIndex + 1U;
  }
  TcpIp_Tcp_holeList[bufferIndex].entries[TCPIP_TCP_HOLE_LIST_SIZE - 1U].nextHoleIndex =
    TCPIP_TCP_HOLE_LIST_INDEX_INVALID;

  /* Set the head indices of the lists */
  TcpIp_Tcp_holeListInfo[bufferIndex].freeHoleListHeadIndex = 0U;
  TcpIp_Tcp_holeListInfo[bufferIndex].usedHoleListHeadIndex = TCPIP_TCP_HOLE_LIST_INDEX_INVALID;
}

/*==================[internal function definitions]=========================*/

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxOutOfOrder_passToUpperLayer
(
  TcpIp_SocketIdType socketId,
  P2CONST(TcpIp_Socket_TCP_Generic_Type, AUTOMATIC, TCPIP_APPL_DATA) socketPtr,
  P2VAR(TcpIp_TcpCtrlBlockType, AUTOMATIC, TCPIP_APPL_DATA) ctrlBlockPtr,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) directDataPtr,
  uint16 directDataLen,
  uint32 directSequenceNum
)
{
  /* Determine what data can be passed to upper layer */
  CONSTP2CONST(TcpIp_SockAddrType, AUTOMATIC, TCPIP_APPL_DATA) remoteAddr = &socketPtr->remote;
  uint8 const sockOwnderId = socketPtr->generic.aligned.sockOwnerId;
  uint16 const bufferIndex = ctrlBlockPtr->ringbuffer_info_index;
  uint8 holeIndex = TcpIp_Tcp_holeListInfo[bufferIndex].usedHoleListHeadIndex;
  CONSTP2VAR(TcpIp_Tcp_RingBufferInfoType, AUTOMATIC, TCPIP_VAR)
  bufferInfoPtr = &TcpIp_Tcp_ringBufferInfo[bufferIndex];
  CONSTP2VAR(uint8, AUTOMATIC, TCPIP_VAR) bufferDataPtr = TcpIp_Tcp_ringBuffer[bufferIndex].data;
  uint32 const startOffset = ctrlBlockPtr->recv_nxt;
  uint32 endOffset;
  uint16 dataLen;
  uint16 totalDataLen;
  uint16 passDirectDataLen = 0;


  TCPIP_PRECONDITION_ASSERT((sockOwnderId < TcpIp_NumConfSockOwner), TCPIP_INTERNAL_API_ID);
  /* If there are no holes left */
  if(holeIndex == TCPIP_TCP_HOLE_LIST_INDEX_INVALID)
  {
    uint32 endDirectOffset = directSequenceNum + directDataLen - 1U;
    endOffset = bufferInfoPtr->writeOffset - 1U;

    /* Free buffer */
    ctrlBlockPtr->ringbuffer_info_index = TCPIP_TCP_RINGBUFFER_INFO_INDEX_INVALID;

    if(TCPIP_TCP_IS_GREATER_THAN(endDirectOffset, endOffset))
    {
      passDirectDataLen = (uint16)(endDirectOffset - endOffset);
    }
  }
  else
  {
    P2VAR(TcpIp_Tcp_HoleListEntryType, AUTOMATIC, TCPIP_APPL_DATA) holeListEntriesPtr =
      TcpIp_Tcp_holeList[bufferIndex].entries;

    /* Iterate holes to find the lowest ones */
    while(holeListEntriesPtr[holeIndex].nextHoleIndex != TCPIP_TCP_HOLE_LIST_INDEX_INVALID)
    {
      holeIndex = holeListEntriesPtr[holeIndex].nextHoleIndex;
    }
    endOffset = holeListEntriesPtr[holeIndex].startOffset - 1U;
  }
  dataLen = (uint16)(endOffset - startOffset + 1U);

  /* Update receive window, next expected seq number, unacknowledged counter */
  totalDataLen = dataLen + passDirectDataLen;
  ctrlBlockPtr->recv_wnd -= totalDataLen;
  ctrlBlockPtr->recv_nxt += totalDataLen;
  ctrlBlockPtr->num_unack += totalDataLen;

  /* If data does not wrap around in buffer */
  if(TCPIP_TCP_IS_LOWER_THAN(endOffset, bufferInfoPtr->baseOffset + TCPIP_TCP_RINGBUFFER_SIZE))
  {
    /* Call upper layer RxIndication */
    TcpIp_RxIndicationAPI[sockOwnderId](
      socketId, remoteAddr, &bufferDataPtr[startOffset - bufferInfoPtr->baseOffset], dataLen);
  }
  else
  {
    uint16 const startIndexEnd = (uint16)(startOffset - bufferInfoPtr->baseOffset);
    uint16 const dataLenEnd = TCPIP_TCP_RINGBUFFER_SIZE - startIndexEnd;

    /* Call upper layer RxIndication twice (end and beginning) and update the base offset */
    TcpIp_RxIndicationAPI[sockOwnderId](
      socketId, remoteAddr, &bufferDataPtr[startIndexEnd], dataLenEnd);
    TcpIp_RxIndicationAPI[sockOwnderId](
      socketId, remoteAddr, &bufferDataPtr[0], dataLen - dataLenEnd);
    bufferInfoPtr->baseOffset += TCPIP_TCP_RINGBUFFER_SIZE;
  }

  /* If data exceeding buffer can be passed directly */
  if(passDirectDataLen > 0)
  {
    TcpIp_RxIndicationAPI[sockOwnderId](
      socketId, remoteAddr, &directDataPtr[directDataLen - passDirectDataLen], passDirectDataLen);
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxOutOfOrder_removeHole
(
  uint16 holeListIndex,
  uint8 holeIndex,
  uint8 holePredecessorIndex
)
{
  TCPIP_PRECONDITION_ASSERT(holeIndex < TCPIP_TCP_HOLE_LIST_SIZE, TCPIP_INTERNAL_API_ID);
  TCPIP_PRECONDITION_ASSERT(holeListIndex < TCPIP_TCP_RINGBUFFER_COUNT, TCPIP_INTERNAL_API_ID);
  {
    CONSTP2VAR(TcpIp_Tcp_HoleListInfoType, AUTOMATIC, TCPIP_VAR) holeListInfoPtr =
      &TcpIp_Tcp_holeListInfo[holeListIndex];
    CONSTP2VAR(TcpIp_Tcp_HoleListEntryType, AUTOMATIC, TCPIP_VAR) holeListEntriesPtr =
      TcpIp_Tcp_holeList[holeListIndex].entries;


    /* Remove from hole list */
    if (holePredecessorIndex == TCPIP_TCP_HOLE_LIST_INDEX_INVALID)
    {
      TCPIP_PRECONDITION_ASSERT( holeListInfoPtr->usedHoleListHeadIndex < TCPIP_TCP_HOLE_LIST_SIZE, TCPIP_INTERNAL_API_ID );
      holeListInfoPtr->usedHoleListHeadIndex =
        holeListEntriesPtr[holeListInfoPtr->usedHoleListHeadIndex].nextHoleIndex;
    }
    else
    {
      TCPIP_PRECONDITION_ASSERT(holePredecessorIndex < TCPIP_TCP_HOLE_LIST_SIZE, TCPIP_INTERNAL_API_ID);
      holeListEntriesPtr[holePredecessorIndex].nextHoleIndex =
      holeListEntriesPtr[holeIndex].nextHoleIndex;
    }

    /* Insert into free list*/
    holeListEntriesPtr[holeIndex].nextHoleIndex = holeListInfoPtr->freeHoleListHeadIndex;
    holeListInfoPtr->freeHoleListHeadIndex = holeIndex;
  }
}

STATIC FUNC(Std_ReturnType, TCPIP_CODE) TcpIp_Tcp_rxOutOfOrder_addHole
(
  uint16 holeListIndex,
  uint32 startOffset,
  uint32 endOffset
)
{
  Std_ReturnType retVal = E_NOT_OK;
  TCPIP_PRECONDITION_ASSERT( holeListIndex < TCPIP_TCP_RINGBUFFER_COUNT, TCPIP_INTERNAL_API_ID );
  {
    /* NOTE: Function could be further optimized by supplying insert position as parameter */
    CONSTP2VAR(TcpIp_Tcp_HoleListInfoType, AUTOMATIC, TCPIP_VAR) holeListInfoPtr = &TcpIp_Tcp_holeListInfo[holeListIndex];
    CONSTP2VAR(TcpIp_Tcp_HoleListEntryType, AUTOMATIC, TCPIP_VAR) holeListEntriesPtr = TcpIp_Tcp_holeList[holeListIndex].entries;

    /* Check if free list is not empty, i.e. hole list is not full */
    if(holeListInfoPtr->freeHoleListHeadIndex != TCPIP_TCP_HOLE_LIST_INDEX_INVALID)
    {
      uint8 successorIndex;
      uint8 predecessorIndex;

      /* Get unused element */
      uint8 const oldFreeHeadIndex = holeListInfoPtr->freeHoleListHeadIndex;
      TCPIP_PRECONDITION_ASSERT( oldFreeHeadIndex < TCPIP_TCP_HOLE_LIST_SIZE, TCPIP_INTERNAL_API_ID );
      holeListInfoPtr->freeHoleListHeadIndex = holeListEntriesPtr[oldFreeHeadIndex].nextHoleIndex;

      /* Update hole data */
      holeListEntriesPtr[oldFreeHeadIndex].startOffset = startOffset;
      holeListEntriesPtr[oldFreeHeadIndex].endOffset = endOffset;

      /* Search for position in list where to insert new hole */
      successorIndex = holeListInfoPtr->usedHoleListHeadIndex;

      TCPIP_PRECONDITION_ASSERT( (successorIndex == TCPIP_TCP_HOLE_LIST_INDEX_INVALID)||
                                 (successorIndex < TCPIP_TCP_HOLE_LIST_SIZE),
                                 TCPIP_INTERNAL_API_ID
                                );
      predecessorIndex = TCPIP_TCP_HOLE_LIST_INDEX_INVALID;
      while((successorIndex != TCPIP_TCP_HOLE_LIST_INDEX_INVALID) &&
            TCPIP_TCP_IS_GREATER_THAN(holeListEntriesPtr[successorIndex].startOffset, startOffset))
      {
        predecessorIndex = successorIndex;
        successorIndex = holeListEntriesPtr[successorIndex].nextHoleIndex;
      }

      /* Insert element into list */
      if(holeListInfoPtr->usedHoleListHeadIndex == successorIndex)
      {
        holeListInfoPtr->usedHoleListHeadIndex = oldFreeHeadIndex;
      }
      else
      {
        TCPIP_PRECONDITION_ASSERT( predecessorIndex < TCPIP_TCP_HOLE_LIST_SIZE, TCPIP_INTERNAL_API_ID );
        holeListEntriesPtr[predecessorIndex].nextHoleIndex = oldFreeHeadIndex;
      }
      holeListEntriesPtr[oldFreeHeadIndex].nextHoleIndex = successorIndex;

      retVal = E_OK;
    }
  }
  return retVal;
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxOutOfOrder_bufferSegment
(
  uint16 holeListIndex,
  uint32 startOffset,
  uint32 endOffset,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) dataPtr
)
{
  TCPIP_PRECONDITION_ASSERT( holeListIndex < TCPIP_TCP_RINGBUFFER_COUNT, TCPIP_INTERNAL_API_ID );
  {
    Std_ReturnType status = E_OK;
    CONSTP2VAR(TcpIp_Tcp_RingBufferInfoType, AUTOMATIC, TCPIP_VAR)
    bufferInfoPtr = &TcpIp_Tcp_ringBufferInfo[holeListIndex];
    CONSTP2VAR(uint8, AUTOMATIC, TCPIP_VAR) bufferDataPtr = TcpIp_Tcp_ringBuffer[holeListIndex].data;

    /* Check for overlap with implicit hole which starts at writeOffset of the buffer info */
    if(TCPIP_TCP_IS_GREATER_OR_EQUAL(endOffset, bufferInfoPtr->writeOffset))
    {
      /* If new data not adjacent */
      if(TCPIP_TCP_IS_GREATER_THAN(startOffset, bufferInfoPtr->writeOffset))
      {
        /* Add a new hole */
        status =
          TcpIp_Tcp_rxOutOfOrder_addHole(holeListIndex, bufferInfoPtr->writeOffset, startOffset - 1U);
      }

      if(status == E_OK)
      {
        /* Determine copy parameters and copy to buffer */
        uint32 const copyStartOffset =
          TCPIP_TCP_IS_LOWER_THAN(startOffset, bufferInfoPtr->writeOffset)
            ? bufferInfoPtr->writeOffset
            : startOffset;
        uint16 const copyStartIndexSrc = (uint16)(copyStartOffset - startOffset);
        TcpIp_Tcp_rxOutOfOrder_copyToBuffer(copyStartOffset,
                                            endOffset,
                                            bufferInfoPtr->baseOffset,
                                            bufferDataPtr,
                                            &dataPtr[copyStartIndexSrc]);
        /* Update write offset */
        bufferInfoPtr->writeOffset = endOffset + 1U;
      }
    }

    {
      CONSTP2VAR(TcpIp_Tcp_HoleListEntryType, AUTOMATIC, TCPIP_VAR) holeListEntriesPtr =
        TcpIp_Tcp_holeList[holeListIndex].entries;
      uint8 linkIndex = TcpIp_Tcp_holeListInfo[holeListIndex].usedHoleListHeadIndex;
      uint8 prevLinkIndex = TCPIP_TCP_HOLE_LIST_INDEX_INVALID;
      uint8 nextLinkIndex = TCPIP_TCP_HOLE_LIST_INDEX_INVALID;

      /* Iterate explicitly modeled holes, i.e. the hole list (ordered by offset in descending order)
          as long as:
          the index is valid AND
          current hole ends above the new filled interval AND
          previous hole additions were successful */
      while((linkIndex != TCPIP_TCP_HOLE_LIST_INDEX_INVALID) &&
            TCPIP_TCP_IS_GREATER_OR_EQUAL(holeListEntriesPtr[linkIndex].endOffset, startOffset) &&
            (status == E_OK))
      {
        nextLinkIndex = holeListEntriesPtr[linkIndex].nextHoleIndex;

        /* If there is an overlap with segment and current hole */
        if(TCPIP_TCP_IS_GREATER_OR_EQUAL(endOffset, holeListEntriesPtr[linkIndex].startOffset))
        {
          uint32 const holeStartOffset = holeListEntriesPtr[linkIndex].startOffset;
          uint32 const holeEndOffset = holeListEntriesPtr[linkIndex].endOffset;

          /* If segment completely eliminates hole */
          if(TCPIP_TCP_IS_LOWER_OR_EQUAL(startOffset, holeStartOffset) &&
             TCPIP_TCP_IS_GREATER_OR_EQUAL(endOffset, holeEndOffset))
          {
            TcpIp_Tcp_rxOutOfOrder_removeHole(holeListIndex, linkIndex, prevLinkIndex);

            /* Skip this entry as prevLinkIndex in the next iteration */
            linkIndex = prevLinkIndex;
          }
          /* If segment splits hole in two */
          else if(TCPIP_TCP_IS_GREATER_THAN(startOffset, holeStartOffset) &&
                TCPIP_TCP_IS_LOWER_THAN(endOffset, holeEndOffset))
          {
            status = TcpIp_Tcp_rxOutOfOrder_addHole(holeListIndex, endOffset + 1U, holeEndOffset);
            if(status == E_OK)
            {
              holeListEntriesPtr[linkIndex].endOffset = startOffset - 1U;
            }
          }
          /* If hole gets shrunk at the end */
          else if(TCPIP_TCP_IS_GREATER_THAN(startOffset, holeStartOffset))
          {
            holeListEntriesPtr[linkIndex].endOffset = startOffset - 1U;
          }
          /* If hole gets shrunk at the start, corresponds to (endOffset < holeEndOffset) */
          else
          {
            holeListEntriesPtr[linkIndex].startOffset = endOffset + 1U;
          }

          /* If we did not run out of holes */
          if(status == E_OK)
          {
            uint32 const copyStartOffset =
              TCPIP_TCP_IS_GREATER_THAN(startOffset, holeStartOffset) ? startOffset : holeStartOffset;
            uint32 const copyEndOffset =
              TCPIP_TCP_IS_LOWER_THAN(endOffset, holeEndOffset) ? endOffset : holeEndOffset;
            uint16 const copyStartIndexSrc = (uint16)(copyStartOffset - startOffset);

            /* Copy data from overlap to buffer */
            TcpIp_Tcp_rxOutOfOrder_copyToBuffer(copyStartOffset,
                                                copyEndOffset,
                                                bufferInfoPtr->baseOffset,
                                                bufferDataPtr,
                                                &dataPtr[copyStartIndexSrc]);
          }
        }
        prevLinkIndex = linkIndex;
        linkIndex = nextLinkIndex;
      }
    }
  }
}

STATIC FUNC(void, TCPIP_CODE) TcpIp_Tcp_rxOutOfOrder_copyToBuffer
(
  uint32 startOffset,
  uint32 endOffset,
  uint32 baseOffset,
  P2VAR(uint8, AUTOMATIC, TCPIP_APPL_DATA) bufferDataPtr,
  P2CONST(uint8, AUTOMATIC, TCPIP_APPL_DATA) srcDataPtr
)
{
  uint16 const startIndex = (uint16)(TCPIP_TCP_RINGBUFFER_OFFSET_TO_INDEX_WRAP(startOffset, baseOffset));
  uint16 const endIndex = (uint16)(TCPIP_TCP_RINGBUFFER_OFFSET_TO_INDEX_WRAP(endOffset, baseOffset));
  uint16 const copyLen = (uint16)(endOffset - startOffset + 1U);

  /* If copy is either not wrapping around or wrapping around completely */
  if(endIndex >= startIndex)
  {
    TS_MemCpy(&bufferDataPtr[startIndex], srcDataPtr, copyLen);
  }
  /* If copy is wrapping wrapped around partially */
  else
  {
    uint16 const endCopyLen = TCPIP_TCP_RINGBUFFER_SIZE - startIndex;
    uint16 const beginCopyLen = copyLen - endCopyLen;
    TS_MemCpy(&bufferDataPtr[startIndex], srcDataPtr, endCopyLen);
    TS_MemCpy(&bufferDataPtr[0], &srcDataPtr[endCopyLen], beginCopyLen);
  }
}

#define TCPIP_STOP_SEC_CODE
#include <TcpIp_MemMap.h>

#else
TS_PREVENTEMPTYTRANSLATIONUNIT
#endif /* (TCPIP_TCP_OUTOFORDER_RECEPTION_ENABLED == STD_ON) */

/*==================[end of file]===========================================*/
