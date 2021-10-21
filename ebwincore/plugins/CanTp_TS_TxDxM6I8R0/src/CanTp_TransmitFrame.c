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
#include <CanTp_InternalLib.h> /* internal macros and variables */
#include <CanTp_Internal.h>    /* internal macros and variables */
#include <CanIf.h>             /* CanIf module types and API */

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#if(CANTP_DYNAMIC_ID_SUPPORT == STD_ON)
/** \brief CanTp_HandleTxMetadata()
 **
 ** Internal function to handle the Tx MetaData.
 **
 **
 ** \param[in] ChannelPtr    - pointer to channel data structure in RAM.
 ** \param[in] FCBufferPtr   - pointer to FC data structure in stack.

 ** \retval TRUE: The MetaData is valid.
 */
STATIC FUNC(boolean, CANTP_CODE) CanTp_HandleTxMetadata
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  P2VAR(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) FCBufferPtr
);
#endif /* CANTP_DYNAMIC_ID_SUPPORT == STD_ON */
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

FUNC(void, CANTP_CODE) CanTp_TransmitFrame
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  P2VAR(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) FCBufferPtr
)
{
  PduInfoType       PduInfo;  /* data to be transmitted */
  /* Id of affected N-SDU */
  const PduIdType NSduId = ChannelPtr->NSduId;
  PduIdType         LSduId;   /* LSduId to transmit via CanIf */
  Std_ReturnType    CanIfRet; /* return value of CanIf */
  CanTp_ChStateType ChannelState;
#if(CANTP_DYNAMIC_ID_SUPPORT == STD_ON)
  boolean IsValidAddInfo = TRUE;
#endif /* CANTP_DYNAMIC_ID_SUPPORT == STD_ON */

  /* CanIf might be in passive mode and respond within the CanIf_Transmit()
     call. Therefore the state will be set now.
   */
  TS_AtomicAssign8(ChannelState, ChannelPtr->State);

  /* get LSduId */
  if(CANTP_CH_GET_MODE(ChannelState) == CANTP_CH_MODE_RX)
  {
    LSduId = CANTP_CFG_GET_RXNSDU_CONFIG(NSduId).LSduIdFc;
  }
  else
  {
    /* isn't Rx -> must be Tx */
    LSduId = CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).LSduIdxF;
  }

#if(CANTP_DYNAMIC_ID_SUPPORT == STD_ON)
  IsValidAddInfo = CanTp_HandleTxMetadata(ChannelPtr, FCBufferPtr);

  /* check if valid Metadata set */
  if(IsValidAddInfo)
  {
#endif /* CANTP_DYNAMIC_ID_SUPPORT == STD_ON */

    if(FCBufferPtr!= NULL_PTR)
    {
      /* If frame is a FC, use FCBufferPtr data */
      PduInfo.SduDataPtr = FCBufferPtr->SduDataPtr;
      PduInfo.SduLength = FCBufferPtr->SduLength;
    }
    else
    {
      /* If not, use ChannelPtr data */
      PduInfo.SduLength  = ChannelPtr->CanIfDLC;
      PduInfo.SduDataPtr = (P2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA)) ChannelPtr->CanIfData;
    }

    /* switch to next state */
    CANTP_CH_SET_ACTION(ChannelPtr, CANTP_CH_ACTION_CONFIRM);

    /* now transmit data to CanIf */
    CanIfRet = CANTP_CANIF_TRANSMIT(LSduId, &PduInfo);

    if(CanIfRet == E_OK)
    {
      if(FCBufferPtr == NULL_PTR)
      {
        /* If sent frame was not a FC, reset send buffer */
        ChannelPtr->CanIfDLC = 0U;
      }
    }
    else
    {
      /* !LINKSTO CanTp.EB.11310, 2 */
      /* !LINKSTO CanTp.EB.11310_2, 1 */
      CanTp_StopChannel(ChannelPtr, CANTP_ULIND_STATUS, NTFRSLT_E_NOT_OK);
    }
#if(CANTP_DYNAMIC_ID_SUPPORT == STD_ON)
  }
  else
  {
    /* !LINKSTO CanTp.EB.MetaData.GetTxNTaGenConnectionOff, 1 */
    CanTp_StopChannel(ChannelPtr, CANTP_ULIND_STATUS, NTFRSLT_E_NOT_OK);
  }
#endif /* CANTP_DYNAMIC_ID_SUPPORT == STD_ON */
}

FUNC(void, CANTP_CODE) CanTp_TransmitPostponedFrame
(
    CanTp_TxChannelType TxChannelID
)
{
  CanTp_ChStateType TxChannelState;
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) Tx_ChannelPtr = NULL_PTR;

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_OFF)
  if(TxChannelID != INVALID_TX_CHANNEL_ID)
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_OFF */
  {
    Tx_ChannelPtr = &CanTp_Channel[TxChannelID];
    TS_AtomicAssign8(TxChannelState, Tx_ChannelPtr->State);
    if(CANTP_CH_GET_ACTION(TxChannelState) == CANTP_CH_ACTION_POSTPONE_FRAME)
    {
      /*clear CANTP_CH_ACTION_POSTPONE_DATA state and transmit data*/
      TxChannelState &= (~CANTP_CH_ACTION_POSTPONE_FRAME);
      TS_AtomicAssign8(Tx_ChannelPtr->State, TxChannelState);

      Tx_ChannelPtr->Ticks = CANTP_GET_TX_PDU_N_AS(&CANTP_CFG_GET_TXNSDU_CONFIG(Tx_ChannelPtr->NSduId));
      CANTP_CH_SET_ACTION(Tx_ChannelPtr, CANTP_CH_ACTION_CONFIRM);
      CanTp_TransmitFrame(Tx_ChannelPtr,NULL_PTR);
    }
  }
}

FUNC(void, CANTP_CODE) CanTp_TransmitPostponedFlowControl
(
  CanTp_ChType RxChannelID
)
{
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) Rx_ChannelPtr = NULL_PTR;

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_OFF)
  if(RxChannelID != INVALID_RX_CHANNEL_ID)
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_OFF */

  {
    Rx_ChannelPtr = &CanTp_Channel[RxChannelID];
    switch(Rx_ChannelPtr->State)
    {
    case CANTP_CH_RX_POSTPONED_FC_WT:
    {
      TS_AtomicAssign8(Rx_ChannelPtr->State, CANTP_CH_RX_TRANSMIT_FC_WT);
      CanTp_PrepareFC(Rx_ChannelPtr, CANTP_N_PCI_FC_WT);
      break;
    }
    case CANTP_CH_RX_POSTPONED_FC_CTS:
    {
      TS_AtomicAssign8(Rx_ChannelPtr->State, CANTP_CH_RX_TRANSMIT_FC_CTS);
      CanTp_PrepareFC(Rx_ChannelPtr, CANTP_N_PCI_FC_CTS);
      break;
    }
    case CANTP_CH_RX_POSTPONED_FC_OVFLW:
    {
      TS_AtomicAssign8(Rx_ChannelPtr->State, CANTP_CH_RX_TRANSMIT_FC_OVFLW);
      CanTp_PrepareFC(Rx_ChannelPtr, CANTP_N_PCI_FC_OVFLW);
      break;
    }
    default:
    {
      /*do nothing*/
      break;
    }
    }
  }
}

#if(CANTP_DYNAMIC_ID_SUPPORT == STD_ON)
STATIC FUNC(boolean, CANTP_CODE) CanTp_HandleTxMetadata
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  P2VAR(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) FCBufferPtr
)
{
  uint32 Metadata = 0U;
  PduInfoType MetadataPduInfo; /* Not used by EcuC but needed in the call */
  uint16 NSa;
  uint16 NTa;
  boolean IsValidAddInfo = TRUE;
  uint8 AddrFormat;
  uint8 TaType;
  PduIdType MetaDataULPduId;
  PduIdType MetaDataLLPduId;
  CanTp_ChStateType ChannelState;
  /* Id of affected N-SDU */
  const PduIdType NSduId = ChannelPtr->NSduId;

  TS_AtomicAssign8(ChannelState, ChannelPtr->State);

  /* get LSduId */
  if(CANTP_CH_GET_MODE(ChannelState) == CANTP_CH_MODE_RX)
  {
    AddrFormat = CANTP_CFG_GET_RXNSDU_CONFIG(NSduId).AddressingFormat;
    MetaDataULPduId = CANTP_CFG_GET_RXNSDU_CONFIG(NSduId).MetadataULPduId;
    MetaDataLLPduId = CANTP_CFG_GET_RXNSDU_CONFIG(NSduId).MetadataLLFcPduId;
    TaType = CANTP_CFG_GET_RXNSDU_CONFIG(NSduId).Tatype;
  }
  else
  {
    /* isn't Rx -> must be Tx */
    AddrFormat = CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).AddressingFormat;
    MetaDataULPduId = CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).MetadataULPduId;
    MetaDataLLPduId = CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).MetadataLLPduId;
    TaType = CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).Tatype;
  }

    /* Check if the Pdu has Metadata */
  if(AddrFormat == CANTP_NORMALFIXED)
  {
    /* !LINKSTO CanTp.EB.MetaData.GetNTaTxFc, 1 */
    /* !LINKSTO CanTp.EB.MetaData.GetTxNSa, 1 */
    /* get the N_SA set by the upper layer */
    (void)EcuC_GetMetaDataSourceAddr(MetaDataULPduId, &MetadataPduInfo, &NSa);

    /* !LINKSTO CanTp.EB.MetaData.GetNSaTxFc, 1 */
    /* !LINKSTO CanTp.EB.MetaData.GetTxNTaGenConnectionOn, 1 */
    /* get the N_TA set by the upper layer */
    (void)EcuC_GetMetaDataTargetAddr(MetaDataULPduId, &MetadataPduInfo, &NTa);

    /* check if not FC */
    if(FCBufferPtr == NULL_PTR)
    {
      /* compare the EcuC SA/TA value with the configured value */
#if(CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON)
      if(CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).N_SA != (uint8)NSa)
#else
      if((CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).N_SA != (uint8)NSa) ||
         (CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).N_TA_AE != (uint8)NTa))
#endif /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_ON */
      {
        IsValidAddInfo = FALSE;
      }
      else
      {
        /* The CanId (29 bits) consists of:
           - bits 0-7 represent the N_SA
           - bits 8-15 represent the N_TA
           - bits 16-28 are fixed, see ISO 15765-2:2016.
         */
        Metadata = CANTP_CALCULATE_CANID_METADATA(TaType, NSa, NTa);
      }
    }
    else
    {
      /* The CanId (29 bits) consists of:
         - bits 0-7 represent the N_SA
         - bits 8-15 represent the N_TA
         - bits 16-28 are fixed, see ISO 15765-2:2016.
       */
      Metadata = CANTP_CALCULATE_CANID_METADATA(TaType, NTa, NSa);
    }

    if(IsValidAddInfo)
    {
      /* !LINKSTO CanTp.EB.MetaData.SetTxMetaDataCanId, 1 */
      /* !LINKSTO CanTp.EB.MetaData.SetTxFcMetaDataCanId, 1 */
      (void)EcuC_SetMetaDataCanId(MetaDataLLPduId, &MetadataPduInfo, Metadata);
    }
  }

  return IsValidAddInfo;
}
#endif /* CANTP_DYNAMIC_ID_SUPPORT == STD_ON */
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
