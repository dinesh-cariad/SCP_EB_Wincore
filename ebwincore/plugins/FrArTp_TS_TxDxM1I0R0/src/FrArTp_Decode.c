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


/* ****************************************************************************
 **                      Include Section                                     **
 *****************************************************************************/

#include <FrArTp_Int.h>           /* get module internal generic interfaces */
#include <FrArTp_CfgAccess_Int.h> /* get module internal CfgAccess unit interfaces */
#include <FrArTp_SM_Int.h>

/* ****************************************************************************
 **                      Local Macros                                        **
 *****************************************************************************/

#define FRARTP_MAXSIZE_PDULENGTHTYPE_UINT16              0xFFFFU
#define FRARTP_DECODE_DEFAULT_STMIN_VALUE_0X7F             0x7FU

/* 1byte addresses, PCI; 1 byte after PCI (data, DL, BS, ...) */
#define FRARTP_DECODE_FRAME_MIN_SIZE                       0x04U

#define FRARTP_DECODE_VALID_RESERVED                       0X00U
#define FRARTP_DECODE_MIN_DL_SF_ISO                        0X01U
#define FRARTP_DECODE_MAX_DL_SF_ISO                        0X07U
#define FRARTP_DECODE_MIN_DL_SF_EXT                        0X01U
#define FRARTP_DECODE_MAX_DL_AND_ADDRSIZE_SF_EXT           0XFCU
#define FRARTP_DECODE_MIN_DL_FF_ISO                        0X08U

/* Flow control frame flow status values*/
#define FRARTP_FT_FLOW_CONTROL_FS_CTS              0x0U
#define FRARTP_FT_FLOW_CONTROL_FS_WT               0x1U
#define FRARTP_FT_FLOW_CONTROL_FS_OVFLW            0x2U
/* extracts the PCI out of a frame */
#define FRARTP_DECODE_GETDECODE_FT(pci) ((pci)&0xF0U)>>4U

#define FRARTP_DECODE_GETHIGHNIBBLE(pci)  ((pci)&0xF0U)
#define FRARTP_DECODE_GETLOWERNIBBLE(pci) ((pci)&0x0FU)
#define FRARTP_DECODE_MAX_PDU_SF_ISO(aiLen) (((PduLengthType) aiLen) + 0x08U)

/* ****************************************************************************
**                      Variable Definitions                                 **
******************************************************************************/
#if FRARTP_LOWLEVELROUTING_ENABLE == STD_ON

#define FRARTP_START_SEC_CONST_UNSPECIFIED
#include <FrArTp_MemMap.h>

typedef Std_ReturnType (*FrArTp_RxIndicationFctPtrType)
(
  PduIdType FrRxPduId,
  const PduInfoType *PduInfoPtr
);

STATIC CONST(FrArTp_RxIndicationFctPtrType,AUTOMATIC) FrArTp_RxIndicationFctPtr =
    &FRARTP_LLR_FCTNAME(FRARTP_LOWLEVELROUTING_PRAEFIX,LLR_FrIf_FrTp_RxIndication);

#define FRARTP_STOP_SEC_CONST_UNSPECIFIED
#include <FrArTp_MemMap.h>

#endif /* FRARTP_LOWLEVELROUTING_ENABLE == STD_ON */

/*
 * Start code section declaration
 */
#define FRARTP_START_SEC_CODE
#include <FrArTp_MemMap.h>

/* ****************************************************************************
**                      Local Function Prototypes                            **
******************************************************************************/

STATIC FUNC(void,FRARTP_CODE) FrArTp_IntRxIndication
(
  FrArTp_RxPduIdxType RxPduId,
  P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr
);


STATIC FUNC(FrArTp_ConnIdxType,FRARTP_CODE) FrArTp_IntDecodeConnection
(
  FrArTp_RxPduIdxType RxPduId,
  P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr,
  P2VAR(uint8,AUTOMATIC,FRARTP_APPL_DATA) PciOffsetPtr
);

STATIC FUNC(void,FRARTP_CODE) FrArTp_IntDecodeSF
(
  FrArTp_ConnIdxType connectionIdx,
  P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr,
  uint8 PCIOffset
);

STATIC FUNC(void,FRARTP_CODE) FrArTp_IntDecodeFF
(
  FrArTp_ConnIdxType connectionIdx,
  P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr,
  uint8 PCIOffset
);

STATIC FUNC(void,FRARTP_CODE) FrArTp_IntDecodeCF
(
  FrArTp_ConnIdxType connectionIdx,
  P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr,
  uint8 PCIOffset
);

STATIC FUNC(void,FRARTP_CODE) FrArTp_IntDecodeSFExt
(
  FrArTp_ConnIdxType connectionIdx,
  P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr,
  uint8 PCIOffset
);

STATIC FUNC(void,FRARTP_CODE) FrArTp_IntDecodeFFExt
(
  FrArTp_ConnIdxType connectionIdx,
  P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr,
  uint8 PCIOffset
);

STATIC FUNC(void,FRARTP_CODE) FrArTp_IntDecodeFC
(
  FrArTp_ConnIdxType connectionIdx,
  P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr,
  uint8 PCIOffset
);

/* ****************************************************************************
**                      Local Function Definitions                           **
******************************************************************************/
STATIC FUNC(void,FRARTP_CODE) FrArTp_IntDecodeSF
(
  FrArTp_ConnIdxType connectionIdx,
  P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr,
  uint8 PCIOffset
)
{
  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00023,1 */
  const uint8 dataLength = FRARTP_DECODE_GETLOWERNIBBLE(PduInfoPtr->SduDataPtr[PCIOffset]);
  CONSTP2CONST(FrArTp_ChannelCfgType, AUTOMATIC, FRARTP_APPL_CONST)
    pChannelCfg = FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connectionIdx);

  /* check if DL is within allowed range */
  if(
     /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00083.15,1 */
     (pChannelCfg->format != FRARTP_FRARTPLM_L4G) &&
     /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00028.2,1 */
     (dataLength <= (PduInfoPtr->SduLength - PCIOffset - FRARTP_PCI_SF_LENGTH) &&
     /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00025.3,1,
                 FrArTp.ASR42.SWS_FrArTp_00028.1,1 */
     (dataLength >= FRARTP_DECODE_MIN_DL_SF_ISO) && (dataLength <= FRARTP_DECODE_MAX_DL_SF_ISO) &&
     /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00025.1,1 */
     (PduInfoPtr->SduLength <= FRARTP_DECODE_MAX_PDU_SF_ISO(PCIOffset))))
  {
    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00023,1 */
    const uint8 payloadOffset = PCIOffset + FRARTP_PCI_SF_LENGTH;

    FrArTp_SM_receivedData(connectionIdx, (uint32) dataLength, dataLength, 0U,
                           &PduInfoPtr->SduDataPtr[payloadOffset]);
  }
}

STATIC FUNC(void,FRARTP_CODE) FrArTp_IntDecodeFF
(
  FrArTp_ConnIdxType connectionIdx,
  P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr,
  uint8 PCIOffset
)
{
  CONSTP2CONST(FrArTp_ChannelCfgType, AUTOMATIC, FRARTP_APPL_CONST)
    pChannelCfg = FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connectionIdx);
  CONSTP2CONST(FrArTp_ConnectionCfgType,AUTOMATIC,FRARTP_APPL_CONST)
    pConnectionCfg = FrArTp_CfgAccess_getConnectionCfgPtr(connectionIdx);

  if
  (
     /* Channel is ISO */
     /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00083.15,1 */
     (pChannelCfg->format != FRARTP_FRARTPLM_L4G) &&
     /* PDU is large enough for AI and PCI */
     /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00039.2,1 */
     (PduInfoPtr->SduLength >= (((PduLengthType) PCIOffset) + FRARTP_PCI_FF_LENGTH)) &&
     /* Connection is not 1:n */
     /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00086.4,1 */
     (pConnectionCfg->isMultRec == FALSE)
  )
  {
    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00237,1,
                FrArTp.ASR42.SWS_FrArTp_00299,1 */
    /* Extract DL 12 bit: 4 + 8 bit */
    const uint8 u8dl_msb = FRARTP_DECODE_GETLOWERNIBBLE(PduInfoPtr->SduDataPtr[PCIOffset]);
    const uint8 u8dl_lsb = PduInfoPtr->SduDataPtr[PCIOffset + 1U];
    const uint32 dataLength = (((uint32) u8dl_msb) << 8U) | ((uint32) u8dl_lsb);

    /* Check if DL has minimum value */
    if
    (
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00039.1,1 */
      (dataLength >= FRARTP_DECODE_MIN_DL_FF_ISO) &&
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00039.2,1 */
      (((PduLengthType) dataLength) >
        (PduInfoPtr->SduLength - ((PduLengthType) PCIOffset + FRARTP_PCI_FF_LENGTH)))
    )
    {
      const uint8 payloadOffset = PCIOffset + FRARTP_PCI_FF_LENGTH;
      /* !LINKSTO FrArTp.EB.FFI.PAYLOAD_LENGTH,1 */
      const uint8 payloadLength = (uint8)(PduInfoPtr->SduLength - payloadOffset);

      FrArTp_SM_receivedData(connectionIdx, dataLength, payloadLength, 0U,
                            &PduInfoPtr->SduDataPtr[payloadOffset]);
    }
  }
}

STATIC FUNC(void,FRARTP_CODE) FrArTp_IntDecodeCF
(
  FrArTp_ConnIdxType connectionIdx,
  P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr,
  uint8 PCIOffset
)
{
  CONSTP2CONST(FrArTp_ConnectionCfgType,AUTOMATIC,FRARTP_APPL_CONST)
    pConnectionCfg = FrArTp_CfgAccess_getConnectionCfgPtr(connectionIdx);

  /* If connection is not 1:n */
  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00086.6,1 */
  if(pConnectionCfg->isMultRec == FALSE)
  {
    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00239,1 */
    const uint8 pci_length = FRARTP_PCI_CF_LENGTH;
    const uint8 payloadOffset = PCIOffset + pci_length;
    const uint8 payloadLength = (uint8)(PduInfoPtr->SduLength - payloadOffset);
    const uint8 sequenceNumber = FRARTP_DECODE_GETLOWERNIBBLE(PduInfoPtr->SduDataPtr[PCIOffset]);

    FrArTp_SM_receivedData(connectionIdx, 0U, payloadLength, sequenceNumber,
                          &PduInfoPtr->SduDataPtr[payloadOffset]);
  }
}

/* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00029,1 */
STATIC FUNC(void,FRARTP_CODE) FrArTp_IntDecodeSFExt
(
  FrArTp_ConnIdxType connectionIdx,
  P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr,
  uint8 PCIOffset
)
{
  CONSTP2CONST(FrArTp_ChannelCfgType, AUTOMATIC, FRARTP_APPL_CONST)
  pChannelCfg = FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connectionIdx);

  /* Ignore SF-E if ISO or ISO6 is configured and if PCI is not within frame */
  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00283.15,1 */
  if((pChannelCfg->format == FRARTP_FRARTPLM_L4G) &&
     (PduInfoPtr->SduLength >= (((PduLengthType) PCIOffset) + FRARTP_PCI_SF_E_LENGTH)))
  {
    const uint8 PCI = PduInfoPtr->SduDataPtr[PCIOffset];

    /* Ignore SF-E if it has unexpected values in the reserved nibble */
    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00287.3,1 */
    if(FRARTP_DECODE_GETLOWERNIBBLE(PCI) == FRARTP_DECODE_VALID_RESERVED)
    {
      const uint8 dataLength = PduInfoPtr->SduDataPtr[PCIOffset + 1U];

      if
      (
        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00286.1,1 */
        (dataLength >= (FRARTP_DECODE_MIN_DL_SF_EXT)) &&
        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00286.2,1,
                    FrArTp.ASR42.SWS_FrArTp_00286.3,1 */
        (dataLength <= ((FRARTP_DECODE_MAX_DL_AND_ADDRSIZE_SF_EXT - PCIOffset)))
      )
      {
        const uint8 pci_length = FRARTP_PCI_SF_E_LENGTH;

        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00287.2,1 */
        if(((uint32) PduInfoPtr->SduLength) >=
           ((uint32) dataLength + ((uint32) PCIOffset + (uint32) pci_length)))
        {
          const uint8 payloadOffset = PCIOffset + pci_length;

          FrArTp_SM_receivedData(connectionIdx, (uint32) dataLength, dataLength, 0U,
                                 &PduInfoPtr->SduDataPtr[payloadOffset]);
        }
      }
    }
  }
}

STATIC FUNC(void,FRARTP_CODE) FrArTp_IntDecodeFFExt
(
  FrArTp_ConnIdxType connectionIdx,
  P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr,
  uint8 PCIOffset
)
{
  CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST)
    pChannelCfg = FrArTp_CfgAccess_getChannelCfgPtr_byConnIdx(connectionIdx);
  CONSTP2CONST(FrArTp_ConnectionCfgType,AUTOMATIC,FRARTP_APPL_CONST)
    pConnectionCfg = FrArTp_CfgAccess_getConnectionCfgPtr(connectionIdx);

  if
  (
    /* Channel is L4G */
    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00283.15,1 */
    (pChannelCfg->format == FRARTP_FRARTPLM_L4G) &&
    /* Connection is not 1:n */
    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00086.5,1 */
    (pConnectionCfg->isMultRec == FALSE)
  )
  {
    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00056,1 */
    if(FRARTP_DECODE_GETLOWERNIBBLE(PduInfoPtr->SduDataPtr[PCIOffset]) == FRARTP_DECODE_VALID_RESERVED)
    {
      /* Check if PCI is within frame */
      /* !LINKSTO FrArTp.EB.FFE.FRAME_LENGTH.1,1,
                  FrArTp.EB.FFE.FRAME_LENGTH.2,1 */
      if(PduInfoPtr->SduLength >= ((PduLengthType)PCIOffset + FRARTP_PCI_FF_E_LENGTH))
      {
        /* DL field 32 bit */
        /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00238,1 */
        const uint8 PCI_DLByte1 = PduInfoPtr->SduDataPtr[PCIOffset + 1U];
        const uint8 PCI_DLByte2 = PduInfoPtr->SduDataPtr[PCIOffset + 2U];
        const uint8 PCI_DLByte3 = PduInfoPtr->SduDataPtr[PCIOffset + 3U];
        const uint8 PCI_DLByte4 = PduInfoPtr->SduDataPtr[PCIOffset + 4U];
        uint32 dataLength = 0U;

        dataLength  = (uint32)(((uint32)PCI_DLByte1) << 24U);
        dataLength |= (uint32)(((uint32)PCI_DLByte2) << 16U);
        dataLength |= (uint32)(((uint32)PCI_DLByte3) << 8U);
        dataLength |= (uint32)(((uint32)PCI_DLByte4) << 0U);

        /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00152.FRARTP_E_INVALID_PARAMETER,1 */
#if (TS_SIZE_PduLengthType == TS_SIZE_uint16)
        /* Check for SDU length out of range (e.g in case of PduLengthType UINT16) */
        if (dataLength > (uint32) FRARTP_MAXSIZE_PDULENGTHTYPE_UINT16)
        {
            /* If activated, make a DET call */
            FRARTP_REPORT_ERROR(FRARTP_RXINDICATION_SERVICE_ID, FRARTP_E_INVALID_PARAMETER);
        }
        else
#endif /*TS_SIZE_PduLengthType */
        {
          /* Check if this should be FF-E according to the data length. */
          /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00057.1,1 */
          if(((PduLengthType) dataLength) > (PduInfoPtr->SduLength - ((PduLengthType) PCIOffset) - FRARTP_PCI_SF_E_LENGTH))
          {
            const uint8 payloadOffset = PCIOffset + FRARTP_PCI_FF_E_LENGTH;
            const uint8 payloadLength = (uint8)(PduInfoPtr->SduLength - payloadOffset);

            FrArTp_SM_receivedData(connectionIdx, dataLength, payloadLength, 0U, &PduInfoPtr->SduDataPtr[payloadOffset]);
          }
        }

      }
    }
  }
}

STATIC FUNC(void,FRARTP_CODE) FrArTp_IntDecodeFC
(
  FrArTp_ConnIdxType connectionIdx,
  P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr,
  uint8 PCIOffset
)
{
  CONSTP2CONST(FrArTp_ConnectionCfgType,AUTOMATIC,FRARTP_APPL_CONST)
    pConnectionCfg = FrArTp_CfgAccess_getConnectionCfgPtr(connectionIdx);

  /* If connection is not 1:n */
  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00087.1,1 */
  if(pConnectionCfg->isMultRec == FALSE)
  {
    /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00240,1 */
    const uint8 flowStatus = FRARTP_DECODE_GETLOWERNIBBLE(PduInfoPtr->SduDataPtr[PCIOffset]);
    switch(flowStatus)
    {
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00067.1,1 */
      case FRARTP_FT_FLOW_CONTROL_FS_CTS:
      {
        /* !LINKSTO FrArTp.EB.FC.FRAME_LENGTH,1 */
        if(PduInfoPtr->SduLength >= (((PduLengthType) PCIOffset) + FRARTP_PCI_FC_LENGTH))
        {
          /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00068.3,1 */
          const uint8 blockSize = PduInfoPtr->SduDataPtr[PCIOffset + FRARTP_PCI_FC_OFFSET_BS];
          uint8 stMin = PduInfoPtr->SduDataPtr[PCIOffset + FRARTP_PCI_FC_OFFSET_STMIN];

          /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00245,1 */
          if (((stMin >= FRARTP_INVALID_STMIN_VALUES_0X80) &&
              (stMin <= FRARTP_INVALID_STMIN_VALUES_0XF0)) ||
              ((stMin >= FRARTP_INVALID_STMIN_VALUES_0XFA)))
          {
            stMin = FRARTP_DECODE_DEFAULT_STMIN_VALUE_0X7F;
          }

          /* Clear To Send: The sender can continue transmitting the message. */
          FrArTp_SM_receivedCTS(connectionIdx,blockSize,stMin);
        }
        break;
      }
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00067.2,1 */
      case FRARTP_FT_FLOW_CONTROL_FS_WT:
      {
        /* Wait: The sender shall wait for another FC frame. */
        FrArTp_SM_receivedWT(connectionIdx);
        break;
      }
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00067.4,1 */
      case FRARTP_FT_FLOW_CONTROL_FS_OVFLW:
      {
        /* Overflow: The transmission shall be aborted. */
        FrArTp_SM_receivedOVFLW(connectionIdx);
        break;
      }
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00285.1,1 */
      default:
      {
        /* Invalid Flow Status: Abort when FC was expected. Otherwise, ignore it. */
        FrArTp_SM_receivedInvalidFS(connectionIdx);
        break;
      }
    }
  }
}

STATIC FUNC(FrArTp_ConnIdxType,FRARTP_CODE) FrArTp_IntDecodeConnection
(
    VAR(FrArTp_RxPduIdxType,AUTOMATIC) RxPduId,
    P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr,
    P2VAR(uint8,AUTOMATIC,FRARTP_APPL_DATA) PciOffsetPtr
)
{
  FrArTp_ConnIdxType connectionIdx = FRARTP_CONN_INVALID;

  /* minimum Pdu length necessary to decode anything received */
  if(PduInfoPtr->SduLength >= FRARTP_DECODE_FRAME_MIN_SIZE)
  {
    uint16 localAddress;
    uint16 remoteAddress;
    uint8 pciOffset;

    CONSTP2CONST(FrArTp_ChannelCfgType,AUTOMATIC,FRARTP_APPL_CONST) pChannelCfg =
        FrArTp_CfgAccess_getChannelCfgPtr_byRxPduIdx(RxPduId);

    if(pChannelCfg->addrSize == 1U)
    {
      localAddress = PduInfoPtr->SduDataPtr[0];
      remoteAddress = PduInfoPtr->SduDataPtr[1];
      pciOffset = 2U;
    }
    else
    {
      localAddress = (uint16)(((uint16)PduInfoPtr->SduDataPtr[0])<<8U)|PduInfoPtr->SduDataPtr[1];
      remoteAddress = (uint16)(((uint16)PduInfoPtr->SduDataPtr[2])<<8U)|PduInfoPtr->SduDataPtr[3];
      pciOffset = 4U;
    }

    if(PduInfoPtr->SduLength >= ((PduLengthType)pciOffset + FRARTP_PCI_SF_LENGTH))
    {
      connectionIdx = FrArTp_CfgAccess_getConnectionIdx_byAddress((FrArTp_RxPduIdxType)RxPduId,localAddress,remoteAddress);
      *PciOffsetPtr = pciOffset;
    }
  }

  return connectionIdx;
}

STATIC FUNC(void,FRARTP_CODE) FrArTp_IntRxIndication
(
    VAR(FrArTp_RxPduIdxType,AUTOMATIC) RxPduId,
    P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr
)
{
  uint8 PCIOffset = 0U;

  /* Check if addresses are configured and the received Pdu is long enough to contain the PCI */
  const FrArTp_ConnIdxType ConnectionIdx = FrArTp_IntDecodeConnection(RxPduId,PduInfoPtr, &PCIOffset);

  /* If a connection has been found */
  /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00261,1 */
  if(ConnectionIdx != FRARTP_CONN_INVALID)
  {
    /* analyze frame type */
    switch (FRARTP_DECODE_GETDECODE_FT(PduInfoPtr->SduDataPtr[PCIOffset]))
    {
      /* ISO 15765-2 single frame */
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00024,1 */
      case FRARTP_FT_SINGLE_FRAME_ID:
      {
        FrArTp_IntDecodeSF(ConnectionIdx,PduInfoPtr,PCIOffset);
        break;
      }
      /* ISO 15765-2 first frame */
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00037,1 */
      case FRARTP_FT_FIRST_FRAME_ID:
      {
        FrArTp_IntDecodeFF(ConnectionIdx,PduInfoPtr,PCIOffset);
        break;
      }
      /* ISO 15765-2 consecutive frame */
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00060.1,1 */
      case FRARTP_FT_CONSECUTIVE_FRAME_ID:
      {
        FrArTp_IntDecodeCF(ConnectionIdx,PduInfoPtr,PCIOffset);
        break;
      }
      /* Extended Single Frame */
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00030.1,1 */
      case FRARTP_FT_EXT_SINGLE_FRAME_ID:
      {
        FrArTp_IntDecodeSFExt(ConnectionIdx,PduInfoPtr,PCIOffset);
        break;
      }
      /* Extended First Frame */
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00055,1 */
      case FRARTP_FT_EXT_FIRST_FRAME_ID:
      {
        FrArTp_IntDecodeFFExt(ConnectionIdx,PduInfoPtr,PCIOffset);
        break;
      }
      /* ISO 15765-2 flow control frame */
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00064.2,1,
                  FrArTp.ASR42.SWS_FrArTp_00066,1 */
      case FRARTP_FT_FLOW_CONTROL_ID:
      {
        FrArTp_IntDecodeFC(ConnectionIdx, PduInfoPtr, PCIOffset);
        break;
      }
      /* !LINKSTO FrArTp.ASR42.SWS_FrArTp_00082.1,1 */
      default:
      {
         break;
      }
    }
  }
}

/* ****************************************************************************
**                      Function Definitions                                 **
******************************************************************************/

/* !LINKSTO FrArTp.EB.FrArTp_RxIndication,1,
            FrArTp.ASR42.SWS_FrArTp_00108,1,
            FrArTp.ASR42.SWS_FrArTp_00114,1 */
FUNC(void,FRARTP_CODE) FrArTp_RxIndication
(
    VAR(PduIdType,AUTOMATIC) RxPduId,
    P2VAR(PduInfoType,AUTOMATIC,FRARTP_APPL_DATA) PduInfoPtr
)
{

  DBG_FRARTP_RXINDICATION_ENTRY(RxPduId,PduInfoPtr);

  /* !LINKSTO FrArTp.VAG.AUDI_ASR_FrTP_054,1,
              FrArTp.VAG.AUDI_ASR_FrTP_055,1.1,
              FrArTp.VAG.AUDI_ASR_FrTP_056,1,
              FrArTp.VAG.AUDI_ASR_FrTP_057,1,
              FrArTp.VAG.AUDI_ASR_FrTP_058,1.1 */
#if FRARTP_LOWLEVELROUTING_ENABLE == STD_ON
  if(FrArTp_RxIndicationFctPtr(RxPduId, PduInfoPtr) != E_NOT_OK)
#endif /* FRARTP_LOWLEVELROUTING_ENABLE == STD_ON */
  {
    /* check if development error detection is enabled */
#if (FRARTP_DEV_ERROR_DETECT == STD_ON)

    /* check for successfully initialized module */
    /* Report to DET and return Error in case module was not initialized before */
    /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00152.E_UNINIT,1 */
    if (FrArTp_InitStatus == FALSE)
    {
      FRARTP_REPORT_ERROR(FRARTP_RXINDICATION_SERVICE_ID, FRARTP_E_UNINIT);
    }
    /* check whether RxPduId is valid */
    /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00152.E_INVALID_PDU_SDU_ID,1 */
    else if(FrArTp_CfgAccess_isValid_RxPduIdx(RxPduId) == FALSE)
    {
        /* Invalid PDU ID parameter has been passed, report to DET */
        FRARTP_REPORT_ERROR(FRARTP_RXINDICATION_SERVICE_ID, FRARTP_E_INVALID_PDU_SDU_ID);
    }
    /* check whether PduInfoPtr or SduDataPtr is a NULL_PTR */
    /* !LINKSTO FrArTp.EB.ASR42.SWS_FrArTp_00152.E_PARAM_POINTER,1 */
    else if( (PduInfoPtr == NULL_PTR)
           ||(PduInfoPtr->SduDataPtr == NULL_PTR)
           )
    {
        /* found a NULL_PTR, report to DET */
        FRARTP_REPORT_ERROR(FRARTP_RXINDICATION_SERVICE_ID, FRARTP_E_PARAM_POINTER);
    }
    else
#endif /* FRARTP_DEV_ERROR_DETECT */
    {
      FrArTp_IntRxIndication((FrArTp_RxPduIdxType)RxPduId,PduInfoPtr);
    }
  }
  DBG_FRARTP_RXINDICATION_EXIT(RxPduId,PduInfoPtr);
}

/*
 * Stop code section declaration
 */
#define FRARTP_STOP_SEC_CODE
#include <FrArTp_MemMap.h>
