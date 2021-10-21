/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/

#include <CanTp_Trace.h>
#include <TSAutosar.h>         /* EB specific standard types */
#include <ComStack_Types.h>    /* typedefs for AUTOSAR com stack */
#include <CanTp_Internal.h>    /* internal macros and variables */
#include <TSMem.h>             /* TS_MemCpy */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_START_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)

FUNC(void, CANTP_CODE) CanTp_PreparexF
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
)
{
  const PduIdType NSduId = ChannelPtr->NSduId;
  uint8 DataIndex; /* index in CAN frame */

  /* Channel State */
  CanTp_ChStateType ChannelState;

  /* init */
  DataIndex = 0U;

  /* set N_TA/N_AE value on extended/mixed frame */
  /* no N_TA/N_AE in standard/normal fixed addressing case */
  if(!CANTP_IS_NORMAL_ADDRESSING(CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).AddressingFormat))
  {
    ChannelPtr->CanIfData[DataIndex] = CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).N_TA_AE;
    DataIndex = 1U;
  }

  /* atomic assign */
  TS_AtomicAssign8(ChannelState, ChannelPtr->State);

  /* write frame dependent header */
  switch (ChannelState)
  {
    case CANTP_CH_TX_BUFREQ_SF:
    {
#if(CANTP_CANFD_ENABLED == STD_ON)
      /* Since it is a single frame, the NPduLength is smaller than 255. */
      uint8 NPduLength = DataIndex + CANTP_SF_NPCI_SIZE + (uint8)ChannelPtr->TotalLength;

      if(NPduLength > CANTP_NPDU_LENGTH_8)
      {
        /* Use CAN FD SF format
         * 1. N_PCI byte: 0U
         * 2. N_PCI byte: payload length
         */
        ChannelPtr->CanIfData[DataIndex] = 0U;
        DataIndex++;
        ChannelPtr->CanIfData[DataIndex] = (uint8) ChannelPtr->TotalLength;
      }
      else
#endif /* CANTP_CANFD_ENABLED == STD_ON */
      {
        /* set N_PCI data: (CANTP_N_PCI_TYPE_SF is 0, lower 4 bits: length */
        ChannelPtr->CanIfData[DataIndex] = (uint8) (ChannelPtr->TotalLength & 0xFU);
      }
      DataIndex++;

      break;
    }
    case CANTP_CH_TX_BUFREQ_FF:
    {
      if(CANTP_12BIT_MAX_MESSAGE_LENGTH < ChannelPtr->TotalLength)
      {
        const uint32 TotalLength = ChannelPtr->TotalLength;

        /* set N_PCI data: 4 bit frame type = FF  + 12 bit length = 0 for big data support. */
        ChannelPtr->CanIfData[DataIndex] = CANTP_N_PCI_TYPE_FF;
        DataIndex++;
        ChannelPtr->CanIfData[DataIndex] = 0U;
        DataIndex++;
        ChannelPtr->CanIfData[DataIndex] = (uint8)(TotalLength >> 24U);
        DataIndex++;
        ChannelPtr->CanIfData[DataIndex] = (uint8)(TotalLength >> 16U);
        DataIndex++;
        ChannelPtr->CanIfData[DataIndex] = (uint8)(TotalLength >> 8U);
        DataIndex++;
        ChannelPtr->CanIfData[DataIndex] = (uint8)TotalLength;
        DataIndex++;
      }
      else
      {
        const uint16 TotalLength = (uint16) ChannelPtr->TotalLength;
        /* set N_PCI data: 4 bit frame type + 12 bit length */
        ChannelPtr->CanIfData[DataIndex]
          = (uint8)(CANTP_N_PCI_TYPE_FF | (uint8)(TotalLength >> 8U));
        DataIndex++;
        ChannelPtr->CanIfData[DataIndex] = (uint8)TotalLength;
        DataIndex++;
      }

      /* Set ProcessedBlockFrames and BlockSize to same value for FF. */
      ChannelPtr->ProcessedBlockFrames = 1U;
      ChannelPtr->BlockSize = 1U;
      break;
    }
    case CANTP_CH_TX_BUFREQ_CF:
    {
      /* set N_PCI data: 4 bit type + 4 bit sequence number */
      ChannelPtr->CanIfData[DataIndex] = ChannelPtr->SequenceNumber
                                         | CANTP_N_PCI_TYPE_CF;
      DataIndex++;

      /* handle sequence number and processed block count */
      ChannelPtr->SequenceNumber++;
      ChannelPtr->SequenceNumber &= CANTP_N_PCI_CF_SN_MASK;
      ChannelPtr->ProcessedBlockFrames++;
      break;
    }
    /* CHECK: NOPARSE */
    default:
    {
      /* unexpected state - should never be reached */
      CANTP_UNREACHABLE_CODE_ASSERT(CANTP_INTERNAL_API_ID);
      break;
    }
    /* CHECK: PARSE */
  }

  /* Set DLC to the header size. */
  ChannelPtr->CanIfDLC = DataIndex;

}

FUNC(void, CANTP_CODE) CanTp_PreparePadding
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  uint8 Padding,
  uint8 CanIdType,
  P2VAR(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) FCBufferPtr
)
{
  uint8_least Idx;
  PduLengthType NPduSize;
#if(CANTP_CANFD_ENABLED == STD_ON)
  PduLengthType FrameLength;
#endif
  uint8 PaddingByte = CANTP_PADDING_BYTE;

  if(FCBufferPtr != NULL_PTR)
  {
    /* If frame is a FC, use FCBufferPtr data */
#if(CANTP_CANFD_ENABLED == STD_ON)
    FrameLength = FCBufferPtr->SduLength;
#endif
    NPduSize = FCBufferPtr->SduLength;
  }
  else
  {
    /* If not, use ChannelPtr data */
#if(CANTP_CANFD_ENABLED == STD_ON)
    FrameLength = ChannelPtr->CanIfDLC;
#endif
    NPduSize = ChannelPtr->CanIfDLC;
  }

#if(CANTP_CANFD_ENABLED == STD_ON)
  if((CANTP_TX_STANDARD_FD_CAN == CanIdType) || (CANTP_TX_EXTENDED_FD_CAN == CanIdType))
  {
    PaddingByte = CANTP_PADDING_BYTE_CANFD;
    if(CANTP_ON_CAN_CAN_FD == Padding)
    {
      /* Padding required */
      NPduSize = CANTP_FD_NPDU_LENGTH_64;
    }
    else
    {
      if(CANTP_NPDU_LENGTH_8 >= FrameLength)
      {
         if(CANTP_ON == Padding)
         {
           /* Padding required */
           NPduSize = CANTP_NPDU_LENGTH_8;
         }
      }
      else if(CANTP_FD_NPDU_LENGTH_12 >= FrameLength)
      {
        /* Mandatory Padding */
        NPduSize = CANTP_FD_NPDU_LENGTH_12;
      }
      else if(CANTP_FD_NPDU_LENGTH_16 >= FrameLength)
      {
        /* Mandatory Padding */
        NPduSize = CANTP_FD_NPDU_LENGTH_16;
      }
      else if(CANTP_FD_NPDU_LENGTH_20 >= FrameLength)
      {
        /* Mandatory Padding */
        NPduSize = CANTP_FD_NPDU_LENGTH_20;
      }
      else if(CANTP_FD_NPDU_LENGTH_24 >= FrameLength)
      {
        /* Mandatory Padding */
        NPduSize = CANTP_FD_NPDU_LENGTH_24;
      }
      else if(CANTP_FD_NPDU_LENGTH_32 >= FrameLength)
      {
        /* Mandatory Padding */
        NPduSize = CANTP_FD_NPDU_LENGTH_32;
      }
      else if(CANTP_FD_NPDU_LENGTH_48 >= FrameLength)
      {
        /* Mandatory Padding */
        NPduSize = CANTP_FD_NPDU_LENGTH_48;
      }
      else
      {
        /* Mandatory Padding */
        NPduSize = CANTP_FD_NPDU_LENGTH_64;
      }
    }
  }
  else
#else
  TS_PARAM_UNUSED(CanIdType);
#endif /* CANTP_CANFD_ENABLED == STD_ON */
  {
    if(CANTP_OFF != Padding)
    {
      /* Padding required */
      NPduSize = CANTP_NPDU_LENGTH_8;
    }
  }

  if(FCBufferPtr != NULL_PTR)
  {
    /* If frame is a FC, use FCBufferPtr data */
    Idx = FCBufferPtr->SduLength;
    FCBufferPtr->SduLength = NPduSize;
  }
  else
  {
    /* If not, use ChannelPtr data */
    Idx = ChannelPtr->CanIfDLC;
    ChannelPtr->CanIfDLC = (uint8)NPduSize;
  }

  for(;Idx < NPduSize;Idx++)
  {
    if(FCBufferPtr != NULL_PTR)
    {
      /* If frame is a FC, add padding bytes to SduDataPtr */
      FCBufferPtr->SduDataPtr[Idx] = PaddingByte;
    }
    else
    {
      /* If not, add padding bytes to CanIfData */
      ChannelPtr->CanIfData[Idx] = PaddingByte;
    }
  }
}

#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
