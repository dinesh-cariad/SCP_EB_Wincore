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

/*==================[inclusions]=============================================*/

#include <CanTp_Trace.h>
#include <TSAutosar.h>         /* EB specific standard types */
#include <TSAtomic_Assign.h>   /* Header for atomic assignment macros */
#include <ComStack_Types.h>    /* typedefs for AUTOSAR com stack */
#include <CanTp_Api.h>         /* Module public API */
#include <CanTp_Cbk.h>         /* CanTp callback declarations */
#include <CanTp_Internal.h>    /* internal macros and variables */
#if(CANTP_JUMPTABLE_SUPPORT == STD_ON)
#include <CanTp_EntryTable.h>  /* CanTp entry jumptable */
#endif /* CANTP_JUMPTABLE_SUPPORT == STD_ON */
#include <TSMem.h>             /* TS_MemCpy */

#if(CANTP_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>
#endif

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/
#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_START_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)

/** \brief CanTp_ProcessReceivedSF()
 **
 ** This function processes a received SF from CanIf:
 ** - Frame length is checked.
 ** - PduR_LoTpStartOfReception() called to start reception.
 ** - PduR_LoTpCopyRxData() called to copy received data.
 ** - Received data will be stored in temporary buffer if upper layer returns busy.
 **
 ** Preconditions:
 ** - channel must be in state CANTP_CH_IDLE and reserved
 ** - frame must be a SF and addressing must be correct (no check again)
 ** - ChannelPtr must correspond to the incoming frame
 ** - parameters must be valid
 ** - N-Pdu length must be within the minimum and maximum range
 ** - N-Pdu length must be the configured NPDU length from ECUC if padding is enabled
 **
 ** \param[in] CanTpRxPduPtr  pointer to received data and length
 ** \param[in] ChannelPtr  pointer to channel data structure in RAM
 */
STATIC FUNC(void, CANTP_CODE) CanTp_ProcessReceivedSF
(
  P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpRxPduPtr,
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);

/** \brief CanTp_ProcessReceivedFF()
 **
 ** This function processes a received FF from CanIf
 ** - Frame length is checked.
 ** - PduR_LoTpStartOfReception() called to start reception.
 ** - PduR_LoTpCopyRxData() called to copy received data.
 ** - Received data will be stored in temporary buffer if upper layer returns busy.
 **
 ** Preconditions:
 ** - channel must be in state CANTP_CH_IDLE and reserved
 ** - frame must be a FF and addressing must be correct (no check again)
 ** - ChannelPtr must correspond to the incoming frame
 ** - parameters must be valid
 ** - N-Pdu length must be within the minimum and maximum range
 **
 ** \param[in] CanTpRxPduPtr  pointer to received data and length
 ** \param[in] ChannelPtr  pointer to channel data structure in RAM
 */
STATIC FUNC(void, CANTP_CODE) CanTp_ProcessReceivedFF
(
  P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpRxPduPtr,
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);

/** \brief CanTp_ProcessReceivedCF()
 **
 ** This function processes a CF from CanIf
 ** - Sequence number is checked.
 ** - Frame length is checked.
 ** - PduR_LoTpCopyRxData() called to copy received data.
 ** - FC (CTS) will be transmitted in case that a whole block was received.
 **
 ** Preconditions:
 ** - channel must be in state CANTP_CH_RX_RECEIVE_CF
 ** - frame must be a CF and addressing must be correct
 ** - ChannelPtr must correspond to the incoming frame
 ** - parameters must be valid
 ** - N-Pdu length must be within the minimum and maximum range
 ** - N-Pdu length must be the configured NPDU length if padding is enabled
 **
 ** \param[in] CanTpRxPduPtr  pointer to received data and length
 ** \param[in] ChannelPtr  pointer to channel data structure in RAM
 */
STATIC FUNC(void, CANTP_CODE) CanTp_ProcessReceivedCF
(
  P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpRxPduPtr,
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);

/** \brief CanTp_ProcessReceivedIntermediateCF()
 **
 ** Part of CanTp_ProcessReceivedCF that handles intermediate consecutive frames.
 **
 ** \param[in] CanTpRxPduPtr  pointer to received data and length
 ** \param[in] ChannelPtr     pointer to channel data structure in RAM
 */
STATIC FUNC(void, CANTP_CODE) CanTp_ProcessReceivedIntermediateCF
(
  P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpRxPduPtr,
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);

/** \brief CanTp_ProcessReceivedLastCF()
 **
 ** Part of CanTp_ProcessReceivedCF that handles final consecutive frames.
 **
 ** \param[in] CanTpRxPduPtr  pointer to received data and length
 ** \param[in] ChannelPtr     pointer to channel data structure in RAM
 ** \param[in] DataLength     Remaining bytes of this reception (Data length in last CF)
 */
STATIC FUNC(void, CANTP_CODE) CanTp_ProcessReceivedLastCF
(
  P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpRxPduPtr,
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  uint8 DataLength
);

/** \brief CanTp_ProcessReceivedFC()
 **
 ** This function receives a FC from CanIf
 ** - length code will be checked
 ** - on CTS: take BS, STmin and continues with next CF.
 ** - on WT: count down and close channel on timeout
 ** - on OVFL: close channel
 **
 ** Preconditions:
 ** - channel must be in state CANTP_CH_TX_RECEIVE_FC
 ** - frame must be a FC and addressing must be correct
 ** - ChannelPtr must correspond to the incoming frame
 ** - parameters must be valid
 ** - N-Pdu length must be within the minimum and maximum range
 ** - N-Pdu length must be 8 if padding is enabled
 **
 ** \param[in] CanTpRxPduPtr  pointer to received data and length
 ** \param[in] ChannelPtr  pointer to channel data structure in RAM
 */
STATIC FUNC(void, CANTP_CODE) CanTp_ProcessReceivedFC
(
  P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpRxPduPtr,
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);

/** \brief CanTp_FindNSduId()
 **
 ** Searched for an N-Sdu matching the received addressing information (N-Pdu Id
 ** for standard addressing, N-Pdu Id and N_SA byte for extended addressing).
 **
 ** \param[in] CanTpRxPduId  Id of N-Pdu received
 ** \param[in] CanDataPtr  pointer to received data
 ** \param[out] NSduId  N-Sdu Id found
 ** \param[out] FrameType  Type of frame form N_PCI byte found
 **
 ** \retval FALSE: no matching N-Sdu found, output parameters are invalid
 ** \retval TRUE: N-Sdu found, corresponding NSduId and FrameType set
 */
STATIC FUNC(boolean, CANTP_CODE) CanTp_FindNSduId
(
  PduIdType CanTpRxPduId,
  P2CONST(uint8, AUTOMATIC, CANTP_APPL_DATA) CanDataPtr,
  P2VAR(PduIdType, AUTOMATIC, CANTP_APPL_DATA) NSduId,
  P2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) FrameType
);

/** \brief CanTp_RxChannelCleanup()
 **
 ** Tries to free the requested channel. In case of usage, a current receive
 ** will be terminated.
 **
 ** \param[in] ChannelPtr  pointer to channel data structure in RAM
 */
STATIC FUNC(void, CANTP_CODE) CanTp_RxChannelCleanup
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
);

/** \brief CanTp_ProcessRxFrame()
 **
 ** Handles the processing of Received frame.
 **
 ** \param[in] ChannelPtr  pointer to channel data structure in RAM
 ** \param[in] CanTpRxPduPtr  pointer to received data and length
 ** \param[in] NSduId  corresponding N-Sdu of the incoming frame
 ** \param[in] FrameType  contains the information of frame type from N_PCI
 **
 */
STATIC FUNC(void, CANTP_CODE) CanTp_ProcessRxFrame
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpRxPduPtr,
  PduIdType NSduId,
  uint8 FrameType
);

#if(CANTP_STALLHANDLING_SUPPORT == STD_ON)
/** \brief CanTp_PostponeRxFrame()
 **
 ** Handles the reception when the channel is locked.
 **
 ** Preconditions:
 ** - CanTpRxPduPtr->SduLength shall be < 256
 **
 ** \param[in] ChannelPtr  pointer to channel data structure in RAM
 ** \param[in] CanTpRxPduPtr  pointer to received data and length
 ** \param[in] NSduId  corresponding N-Sdu of the incoming frame
 ** \param[in] FrameType  contains the information of frame type from N_PCI
 **
 */
STATIC FUNC(void, CANTP_CODE) CanTp_PostponeRxFrame
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpRxPduPtr,
  PduIdType NSduId,
  uint8 FrameType
);
#endif

/** \brief CanTp_CheckNPduLength()
 **
 ** Internal function to verify the N-PDU length of a received frame. This is relevant for
 ** SF and last CF since the N-PDU size can vary.
 ** If Padding is enabled, the function checks that N-PDU is not below 8.
 ** If CAN FD support is enabled, the function checks that N-PDU length > 8 has the value
 ** 12, 16, 20, 24, 32, 48 or 64.
 **
 ** Preconditions:
 ** - The N-PDU length shall not exceed the PduLength in EcuC.
 **
 ** \param[in] NPduLength - Length to be checked
 ** \param[in] Padding - Parameter indicates if padding is enabled and shall be considered.

 ** \retval TRUE: NPduLength is valid.
 ** \retval FALSE: NPduLength is invalid.
 */
STATIC FUNC(boolean, CANTP_CODE) CanTp_CheckNPduLength
(
  PduLengthType NPduLength,
  uint8 Padding,
  uint8 MaxLength
);

/** \brief CanTp_ValidateFCLength()
 **
 ** Internal function to validate FC length
 **
 **
 ** \param[in] NSduId           - ID of the NSdu.
 ** \param[in] SduLength        - Parameter indicates Sdu Length.
 ** \param[in] AddressingFormat - Parameter indicates used addressing format.

 ** \retval TRUE: FCLength is valid.
 */
STATIC FUNC(boolean, CANTP_CODE) CanTp_ValidateFCLength
(
  PduIdType NSduId,
  PduLengthType SduLength,
  const uint8  AddressingFormat
);

#if(CANTP_DYNAMIC_ID_SUPPORT == STD_ON)
/** \brief CanTp_ProcessReceivedMetaData()
 **
 ** Internal function to process the MetaData received via a FF/SF.
 **
 **
 ** \param[in] NSduId        - Id of received N-Sdu.
 ** \param[in] NSaCfg        - defines the configured source address to be set
 **                            for the upper layer.
 ** \param[in] FrameType     - defines if it is a SF, FF, CF or a FC.
 ** \param[in] NAIPtr        - pointer to N_AI containing the N_TA and N_SA.

 ** \retval TRUE: The received MetaData is valid.
 */
STATIC FUNC(boolean, CANTP_CODE) CanTp_ProcessReceivedMetaData
(
  PduIdType NSduId,
  uint8 NSaCfg,
  uint8 FrameType,
  P2VAR(uint16, AUTOMATIC, CANTP_APPL_DATA) NAIPtr
);

#endif /* CANTP_DYNAMIC_ID_SUPPORT == STD_ON */

/** \brief CanTp_ValidateNAI()
 **
 ** Internal function to process the MetaData received via a FF/SF.
 **
 **
 ** \param[in] ChannelPtr    - pointer to channel data structure in RAM.
 ** \param[in] NSduId        - Id of received N-Sdu.
 ** \param[in] CanDataNAI    - N_AI received in the first data byte for
 **                            extended/mixed addressing format.
 ** \param[in] FrameType     - defines if it is a SF, FF, CF or a FC.

 ** \retval TRUE: The received MetaData is valid.
 */
STATIC FUNC(boolean, CANTP_CODE) CanTp_ValidateNAI
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  PduIdType NSduId,
  PduIdType CanDataNAI,
  uint8 FrameType
);
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

#if(CANTP_PROVIDE_API_FUNCTIONS == STD_ON)

/** \brief CanTp_RxIndication()
 **
 ** This function receives any N-PDU from CanIf
 ** - checks if a configured N-SDU exists for this N-PDU
 ** - For SF or FF it verifies that channel is available and starts reception
 ** - For CF or FC it verifies that channel is waiting for this N-PDU
 **   and processes the reception
 **
 ** \param[in] CanTpRxPduId, N-Pdu Id that was received
 ** \param[in] CanTpRxPduPtr, pointer to received data and length
 */
FUNC(void, CANTP_CODE) CanTp_RxIndication
(
  PduIdType CanTpRxPduId,
  P2VAR(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpRxPduPtr
)
#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)
{
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr;

  PduIdType               NSduId;           /* affected N-Sdu Id */
  uint8                   FrameType;        /* frame type from N_PCI frame header */

  DBG_CANTP_RXINDICATION_ENTRY(CanTpRxPduId, CanTpRxPduPtr);

#if(CANTP_DEV_ERROR_DETECT == STD_ON)
  /* check CanTp state - possible DET and return on error */
  if(CanTp_InitVars.State != CANTP_ON)
  {
    CANTP_DET_REPORT_ERROR(
        CANTP_MODULE_ID,
        CANTP_INSTANCE_ID,
        CANTP_API_RXINDICATION,
        CANTP_E_UNINIT
    );
  }

  /* check null pointer - possible DET and return on error */
  else if((CanTpRxPduPtr == NULL_PTR) || (CanTpRxPduPtr->SduDataPtr == NULL_PTR))
  {
    /* call DET with error ERRORCODE */
    CANTP_DET_REPORT_ERROR(
        CANTP_MODULE_ID,
        CANTP_INSTANCE_ID,
        CANTP_API_RXINDICATION,
        CANTP_E_PARAM_POINTER
    );
  }

  /* check N-PDU Id range - possible DET and return on error */
  else if(CanTpRxPduId >= CanTp_CfgPtr->NumberRxNPdus)
  {
    /* call DET with error ERRORCODE */
    CANTP_DET_REPORT_ERROR(
        CANTP_MODULE_ID,
        CANTP_INSTANCE_ID,
        CANTP_API_RXINDICATION,
        CANTP_E_INVALID_RX_ID
    );
  }

  /* Length must be at least 2 bytes */
  else if(CANTP_NPDU_LENGTH_MIN > CanTpRxPduPtr->SduLength)
  {
    CANTP_DET_REPORT_ERROR(
        CANTP_MODULE_ID,
        CANTP_INSTANCE_ID,
        CANTP_API_RXINDICATION,
        CANTP_E_INVALID_RX_LENGTH
    );
  }
  else
#endif
{
    /* Initialize NSduId and NPCIIndex to prevent false uninitialized warnings
       from code verification tools and compilers - values are overwritten with
       the correct values by CanTp_FindNSduId() if the function succeeds.
     */
    NSduId = 0U;
    FrameType = 0U;

    /* search for the corresponding NSdu */
    if(CanTp_FindNSduId(CanTpRxPduId, CanTpRxPduPtr->SduDataPtr, &NSduId, &FrameType))
    {
      P2CONST(CanTp_RxNPduTableType, AUTOMATIC, CANTP_APPL_CONST)
                RxNPduTableEntry = &(CANTP_CFG_GET_RXNPDU_TABLE(CANTP_CFG_GET_RXNPDU_LOOKUPTABLE(CanTpRxPduId)));
      boolean RxProcessFlag = FALSE;

      PduLengthType NPduMaxLength = CANTP_NPDU_LENGTH_8;
      boolean FCValidLength = FALSE;

      CanTp_ChType Channel; /* counter for channels */
      Std_ReturnType Result = E_NOT_OK;

      /* If an NSdu for an FC was found it must be Tx, otherwise it's Rx */
      if(FrameType == CANTP_N_PCI_TYPE_FC)
      {
        CANTP_GET_LOCKED_TX_CH_INDEX(Channel, NSduId, Result);
      }
      else
      {
        CANTP_GET_RX_CH_INDEX(Channel, NSduId, Result);
      }

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)
      if(E_OK == Result)
#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */
      {
        ChannelPtr = &CanTp_Channel[Channel];

        /* If an NSdu for an FC was found it must be Tx, otherwise it's Rx */
        if(FrameType == CANTP_N_PCI_TYPE_FC)
        {
          /* addressing format */
          const uint8 AddressingFormat = CANTP_GET_ADDR_FORMAT(RxNPduTableEntry);

          FCValidLength = CanTp_ValidateFCLength(NSduId, CanTpRxPduPtr->SduLength, AddressingFormat);
        }
        else
        {
          NPduMaxLength = CANTP_CFG_GET_RXNSDU_CONFIG(NSduId).NPduMaxLength;
        }

#if(CANTP_DEV_ERROR_DETECT == STD_ON)
        /* Check that SduLength is at least 3 bytes for FF with extended or mixed addressing. */
        if(FrameType == CANTP_N_PCI_TYPE_FF && (CANTP_GET_ADDR_FORMAT(RxNPduTableEntry) == CANTP_EXTENDED || CANTP_GET_ADDR_FORMAT(RxNPduTableEntry) == CANTP_MIXED) && CANTP_EXT_MIX_FF_MIN_LENGTH > CanTpRxPduPtr->SduLength)
        {
          CANTP_DET_REPORT_ERROR(
          CANTP_MODULE_ID,
          CANTP_INSTANCE_ID,
          CANTP_API_RXINDICATION,
          CANTP_E_INVALID_RX_LENGTH
          );
        }
        else
#endif /* (CANTP_DEV_ERROR_DETECT == STD_ON) */

        /* Check that SduLength is below the configured limit. */
        if(((FrameType == CANTP_N_PCI_TYPE_FC) && (FCValidLength == TRUE)) || ((FrameType != CANTP_N_PCI_TYPE_FC) && (CanTpRxPduPtr->SduLength <= NPduMaxLength)))
        {
          P2CONST(uint8, AUTOMATIC, CANTP_APPL_CONST) SduDataPtr = CanTpRxPduPtr->SduDataPtr;
          boolean IgnoreMsgFlag = CanTp_ValidateNAI(ChannelPtr, NSduId, (PduIdType)SduDataPtr[0U], FrameType);

          if(IgnoreMsgFlag == (boolean)FALSE)
          {
#if(CANTP_STALLHANDLING_SUPPORT == STD_ON)
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
            boolean OverwrittenCF = FALSE;
#endif /* (CANTP_DEV_ERROR_DETECT == STD_ON) */
#endif /* CANTP_STALLHANDLING_SUPPORT == STD_ON */

            CANTP_ENTER_CRITICAL_SECTION();

            /* lock channel if possible */
            if(
                !((CANTP_IS_CHANNEL_LOCKED(ChannelPtr)) &&
                (((FrameType == CANTP_N_PCI_TYPE_FC) || (FrameType == CANTP_N_PCI_TYPE_CF)) ||
                ((FrameType == CANTP_N_PCI_TYPE_SF) && ((CANTP_CHECK_FLAG(ChannelPtr->ConnectionFlags,CANTP_TXLASTFRAME_MASK)) != (boolean)FALSE))))
            )
            {
              CANTP_LOCK_CHANNEL(ChannelPtr);
              /* Setting RxProcessFlag to process the received frame */
              RxProcessFlag = TRUE;
            }
            else
            {
#if(CANTP_STALLHANDLING_SUPPORT == STD_ON)
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
              if((FrameType == CANTP_N_PCI_TYPE_CF) && ((CANTP_CHECK_FLAG(ChannelPtr->PostFlag,CANTP_POST_RX_IND_MASK)) != (boolean)FALSE))
              {
                OverwrittenCF = TRUE;
              }
#endif /* (CANTP_DEV_ERROR_DETECT == STD_ON) */

              /* Postponing Rx Frame when channel is locked */
              CanTp_PostponeRxFrame(
                  ChannelPtr,
                  CanTpRxPduPtr,
                  NSduId,
                  FrameType
              );
#endif /* CANTP_STALLHANDLING_SUPPORT == STD_ON */
            }

            CANTP_LEAVE_CRITICAL_SECTION();

#if(CANTP_STALLHANDLING_SUPPORT == STD_ON)
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
            if(OverwrittenCF == (boolean)TRUE)
            {
              /* Frame is overwritten - call DET and stop channel */
              CANTP_DET_REPORT_ERROR(
                  CANTP_MODULE_ID,
                  CANTP_INSTANCE_ID,
                  CANTP_API_RXINDICATION,
                  CANTP_E_OVERWRITE_FRAME
              );
            }
#endif /* (CANTP_DEV_ERROR_DETECT == STD_ON) */
#endif /* CANTP_STALLHANDLING_SUPPORT == STD_ON */

            /* Checking RxProcessFlag to process the received frame */
            if(RxProcessFlag == (boolean)TRUE)
            {
              /* Processing the received frame. */
              CanTp_ProcessRxFrame(
                  ChannelPtr,
                  CanTpRxPduPtr,
                  NSduId,
                  FrameType
              );
              CanTp_HandlePostponedFlags(ChannelPtr);

              /*unlock channel*/
              CANTP_UNLOCK_CHANNEL(ChannelPtr);
            }
          }
        }
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
        else
        {
          /* length is wrong - call DET and ignore frame*/
          CANTP_DET_REPORT_ERROR(
              CANTP_MODULE_ID,
              CANTP_INSTANCE_ID,
              CANTP_API_RXINDICATION,
              CANTP_E_INVALID_RX_LENGTH
          );
        }
#endif
      }
    } /* if a configured N-SDU was found */
}

  DBG_CANTP_RXINDICATION_EXIT(CanTpRxPduId, CanTpRxPduPtr);

} /* CanTp_RxIndication() */
#else /* => CANTP_JUMPTABLE_MODE == CANTP_JUMPTABLE_CLIENT */
{

  DBG_CANTP_RXINDICATION_ENTRY(CanTpRxPduId, CanTpRxPduPtr);
  (CanTp_EntryTablePtr->RxIndication)(CanTpRxPduId, CanTpRxPduPtr);

  DBG_CANTP_RXINDICATION_EXIT(CanTpRxPduId, CanTpRxPduPtr);
}
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#endif /* CANTP_PROVIDE_API_FUNCTIONS == STD_ON */

#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)
#if(CANTP_STALLHANDLING_SUPPORT == STD_ON)
FUNC(void, CANTP_CODE) CanTp_PostRxIndication
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
)
{
  PduInfoType CanTpNPdu; /* to store NPdu data pointer and length */
  uint8 NPduData[CANTP_NPDU_LENGTH_MAX] = {0U}; /* to store NPdu data */
  PduIdType NSduId;
  uint8 FrameType; /* frame type of postponed frame */
  uint8_least Loop;
  boolean PendingFrame = FALSE; /* indicates if a frame is postponed */

  /* Initialization required to avoid "usage of uninitialized variables" warning. */
  NSduId = 0U;
  FrameType = 0U;
  CanTpNPdu.SduDataPtr = NULL_PTR;
  CanTpNPdu.SduLength = 0U;

  CANTP_ENTER_CRITICAL_SECTION();

  if((CANTP_CHECK_FLAG(ChannelPtr->PostFlag,CANTP_POST_RX_IND_MASK)) != (boolean)FALSE)
  {
    /* Copy the postponed frame data to local variables. */
    for(Loop = 0U; Loop < ChannelPtr->PostRxIndLength; Loop++)
    {
      NPduData[Loop] = ChannelPtr->PostRxIndData[Loop];
    }
    CanTpNPdu.SduDataPtr = &NPduData[0U];
    CanTpNPdu.SduLength = ChannelPtr->PostRxIndLength;
    NSduId = ChannelPtr->PostRxIndNSduId;
    FrameType = ChannelPtr->PostRxIndFrameType;

    /* Free postponed frame memory. */
    CANTP_CLEAR_FLAG(ChannelPtr->PostFlag,CANTP_POST_RX_IND_MASK);
    PendingFrame = TRUE;
  }
  CANTP_LEAVE_CRITICAL_SECTION();

  if(PendingFrame)
  {
    /* lock channel */
    CANTP_LOCK_CHANNEL(ChannelPtr);

    CanTp_ProcessRxFrame(
                          ChannelPtr,
                          &CanTpNPdu,
                          NSduId,
                          FrameType
                        );

    /* unlock channel */
    CANTP_UNLOCK_CHANNEL(ChannelPtr);
  }
}
#endif /*(CANTP_STALLHANDLING_SUPPORT == STD_ON)*/
#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

/*==================[internal function definitions]==========================*/

#if(CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT)
#if(CANTP_STALLHANDLING_SUPPORT == STD_ON)
STATIC FUNC(void, CANTP_CODE) CanTp_PostponeRxFrame
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpRxPduPtr,
  PduIdType NSduId,
  uint8 FrameType
)
{
  /* memorize Rx indication */
  TS_MemCpy(ChannelPtr->PostRxIndData, CanTpRxPduPtr->SduDataPtr, CanTpRxPduPtr->SduLength);
  CANTP_SET_FLAG(ChannelPtr->PostFlag,CANTP_POST_RX_IND_MASK);
  ChannelPtr->PostRxIndNSduId = NSduId;
  ChannelPtr->PostRxIndLength = (uint8) CanTpRxPduPtr->SduLength;
  ChannelPtr->PostRxIndFrameType = FrameType;
}
#endif

STATIC FUNC(void, CANTP_CODE) CanTp_ProcessRxFrame
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpRxPduPtr,
  PduIdType NSduId,
  uint8 FrameType
)
{
  CanTp_ChStateType ChannelState;

  switch(FrameType)
  {
    case CANTP_N_PCI_TYPE_SF: /* single frame handling */
    {
      CanTp_RxChannelCleanup(ChannelPtr);

      /* atomic assign after cleanup */
      TS_AtomicAssign8(ChannelState, ChannelPtr->State);

      if(ChannelState == CANTP_CH_IDLE)
      {
        ChannelPtr->NSduId = NSduId; /* take this channel */
        CANTP_INC_NON_IDLE_CHANNELS();  /* increment the used channels counter */
        CanTp_ProcessReceivedSF(CanTpRxPduPtr, ChannelPtr);
      }
      break;
    }
    case CANTP_N_PCI_TYPE_FF: /* First frame handling */
    {
      CanTp_RxChannelCleanup(ChannelPtr);

      /* atomic assign after cleanup */
      TS_AtomicAssign8(ChannelState, ChannelPtr->State);

      if(ChannelState == CANTP_CH_IDLE)
      {
        ChannelPtr->NSduId = NSduId; /* take this channel */
        CANTP_INC_NON_IDLE_CHANNELS();  /* increment the used channels counter */
        CanTp_ProcessReceivedFF(CanTpRxPduPtr, ChannelPtr);
      }
      break;
    }
    case CANTP_N_PCI_TYPE_CF: /* Consecutive frame handling */
    {
      TS_AtomicAssign8(ChannelState, ChannelPtr->State);

      /* Check NSdu id & actual channel mode, ignore frame if not correct */
      if((ChannelPtr->NSduId == NSduId)
         && (CANTP_CH_MODE_RX == CANTP_CH_GET_MODE(ChannelState))
        )
      {
        CanTp_ProcessReceivedCF(CanTpRxPduPtr, ChannelPtr);
      }
      break;
    }
    case CANTP_N_PCI_TYPE_FC: /* Flow control frame handling */
    {
      /* Check NSdu id, ignore frame if not correct */
      if(ChannelPtr->NSduId == NSduId)
      {
        CanTp_ProcessReceivedFC(CanTpRxPduPtr, ChannelPtr);
      }
      break;
    }
    default:
    {
      /* ignore unknown frame type */
      break;
    }
  }
}

STATIC FUNC(void, CANTP_CODE) CanTp_RxChannelCleanup
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
)
{

  CanTp_ChStateType ChannelState;
  TS_AtomicAssign8(ChannelState, ChannelPtr->State);

  /* CANTP290: Terminate current Rx but continue Tx */
  switch(CANTP_CH_GET_MODE(ChannelState))
  {
    case CANTP_CH_MODE_TX:
    {
      /* don't interrupt when channel is in Tx mode */
      break;
    }
    case CANTP_CH_MODE_IDLE:
    {
      /* channel is already idle -> Nothing to do. */
      break;
    }
    case CANTP_CH_MODE_RX:
    {
      if(
          CANTP_GET_CH_MODE(&CANTP_CFG_GET_RXNSDU_CONFIG(ChannelPtr->NSduId))
          == CANTP_MODE_FULL_DUPLEX
        )
      {
        /* !LINKSTO CanTp.ASR40.CANTP057_FullDuplex_6, 1, CanTp.ASR40.CANTP057_FullDuplex_7, 1 */
        /* Stop channel as SF/FF arrived while a reception is already going on in FULL DUPLEX channel
           and notify upper layer
         */
        CanTp_StopChannel(
                           ChannelPtr,
                           CANTP_ULIND_STATUS,
                           NTFRSLT_E_NOT_OK
                         );
      }
      else
      {
        /* !LINKSTO CanTp.ASR40.CANTP057_HalfDuplex_6, 2, CanTp.ASR40.CANTP057_HalfDuplex_7, 2 */
        /* Stop channel as SF/FF arrived while a reception is already going on in HALF DUPLEX channel
           and notify upper layer
         */
        CanTp_StopChannel(
                           ChannelPtr,
                           CANTP_ULIND_STATUS,
                           NTFRSLT_E_UNEXP_PDU
                         );
      }
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
}

STATIC FUNC(boolean, CANTP_CODE) CanTp_FindNSduId
(
  PduIdType CanTpRxPduId,
  P2CONST(uint8, AUTOMATIC, CANTP_APPL_DATA) CanDataPtr,
  P2VAR(PduIdType, AUTOMATIC, CANTP_APPL_DATA) NSduId,
  P2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) FrameType
)
{
  P2CONST(CanTp_RxNPduTableType, AUTOMATIC, CANTP_APPL_CONST)
    RxNPduTableEntry = &(CANTP_CFG_GET_RXNPDU_TABLE(CANTP_CFG_GET_RXNPDU_LOOKUPTABLE(CanTpRxPduId)));
  boolean FoundFlag;        /* found something? */
  uint8  SearchDirection;  /* N-Sdu direction */
  /* addressing format */
  const uint8  AddressingFormat= CANTP_GET_ADDR_FORMAT(RxNPduTableEntry);
  /* index of first N_PCI byte */
  const uint8  FirstNPCIByte = CANTP_GET_FIRST_N_PCI_BYTE(RxNPduTableEntry);
  uint8  N_SA_AE;          /* N_SA (extended adr.) or N_AE (mixed adr.) */
#if(CANTP_DYNAMIC_NSA_ENABLED == STD_ON)
  CanTp_RxNPduLookupEntryType  Index;   /* Index for CanTp_NSaValues[] */
#endif

  FoundFlag = FALSE;

  /* find out the direction of the search (based on frame type) */

#if(CANTP_DYNAMIC_NSA_ENABLED == STD_ON)
  /* Get first index out of Lookup Table */
  Index = CANTP_CFG_GET_RXNPDU_LOOKUPTABLE(CanTpRxPduId);
#endif

  /* a FC frame belongs to an Tx-Connection */
  if(CANTP_IS_N_PCI_TYPE_FC(CanDataPtr[FirstNPCIByte]))
  {
    SearchDirection = CANTP_PDU_DIR_TRANSMIT;
  }
  else
  {
    SearchDirection = CANTP_PDU_DIR_RECEIVE;
  }

  /* find the configured N-SDU */
  do
  {
    /* if direction is correct */
    if(RxNPduTableEntry->Direction == SearchDirection)
    {
      /* Get N_SA/N_AE value */
#if(CANTP_DYNAMIC_NSA_ENABLED == STD_ON)
      if(AddressingFormat == CANTP_EXTENDED)
      {
        /* get N_SA value which was set at runtime */
        TS_AtomicAssign8(N_SA_AE,CanTp_NSaValues[Index]);
      }
      else
      {
        /* get dummy N_SA/N_AE (standard adr.) or N_AE (mixed adr.) value */
        N_SA_AE = RxNPduTableEntry->N_SA_AE;
      }
#else
      /* get configured N_SA/N_AE value */
      N_SA_AE = RxNPduTableEntry->N_SA_AE;
#endif

      /* if addressing format is standard or
       * addressing format is normal fixed or
       * addressing format is extended and N_SA (first byte) is correct or
       * addressing format is mixed and N_AE (first byte) is correct
       */
      if((CANTP_IS_NORMAL_ADDRESSING(AddressingFormat)) || (CanDataPtr[0] == N_SA_AE))
      {
        /* this is the correct NSdu */
        FoundFlag = TRUE;
        *NSduId = RxNPduTableEntry->Index;
        *FrameType = CANTP_GET_N_PCI_TYPE(CanDataPtr[FirstNPCIByte]);
      }
    }

    /* get next element */
    if(RxNPduTableEntry->Next != 0U)
    {
#if(CANTP_DYNAMIC_NSA_ENABLED == STD_ON)
      /* get next index */
      Index = RxNPduTableEntry->Next;
#endif
      RxNPduTableEntry = &(CANTP_CFG_GET_RXNPDU_TABLE(RxNPduTableEntry->Next));
    }
    else
    {
      /* no more matching N-PDUs - quit the search */
      break;
    }
  } while(FoundFlag == (boolean)FALSE);

#if(CANTP_DEV_ERROR_DETECT == STD_ON)
  if((CANTP_IS_NORMAL_ADDRESSING(AddressingFormat)) && (FoundFlag == (boolean)FALSE))
  {
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_RXINDICATION,
                            CANTP_E_INVALID_RX_ID
                          );
  }
#endif

  return FoundFlag;
}


STATIC FUNC(void, CANTP_CODE) CanTp_ProcessReceivedSF
(
  P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpRxPduPtr,
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
)
{
  /* Pointer  to N-Sdu config */
  CONSTP2CONST(CanTp_RxNSduConfigType, AUTOMATIC, CANTP_APPL_CONST) RxNSduCfgPtr
                            = &CANTP_CFG_GET_RXNSDU_CONFIG(ChannelPtr->NSduId);

  /* Start position of first N_PCI byte within the NPdu. */
  const uint8 StartPosition_SF_N_PCI = CANTP_GET_FIRST_N_PCI_BYTE(RxNSduCfgPtr);

  /* Encoded TP length from N_PCI data field of the Can frame: */
  uint8 DataLength = 0U;
  uint8 NPciHeaderSize = 0U;

  boolean ValidFrameLength = FALSE; /* Indication for valid the data length. */

  /* CAN payload smaller or equal than 8 */
  if(CanTpRxPduPtr->SduLength <= CANTP_NPDU_LENGTH_8)
  {
    /* Data length is contained in the first byte lower nibble */
    DataLength = CanTpRxPduPtr->SduDataPtr[StartPosition_SF_N_PCI] & CANTP_N_PCI_SF_DL_MASK;
    NPciHeaderSize = CANTP_SF_NPCI_SIZE;

    /* Check for a valid SF_DL */
    if(DataLength == 0U)
    {
      /* if SF_DL equal to zero, the frame shall be ignored */
      ValidFrameLength = FALSE;
    }
    else if(DataLength < CANTP_NPDU_LENGTH_8) /* SFs with SF_DL < 8 */
    {
      /* for STANDARD/NORMAL_FIXED addressing, SF_DL shall be smaller or equal than CAN_DL-1 (see Table 12 from ISO 15765-2:2016) */
      if(StartPosition_SF_N_PCI == 0U)
      {
      /* minimum SF_DL value not checked because it is checked in the beginning of the CanTp_RxIndication() */
        if(DataLength <= (CanTpRxPduPtr->SduLength - 1U))
        {
          ValidFrameLength = TRUE;
        }
        else
        {
          ValidFrameLength = FALSE;
        }
      }
      else /* for EXTENDED addressing, SF_DL shall be smaller or equal than CAN_DL-2 (see Table 12 from ISO 15765-2:2016) */
      {
        if((CanTpRxPduPtr->SduLength >= CANTP_CAN20_SINGLEFRAME_MIN_SDULENGTH_EXTENDED_MIXED) && (DataLength <= (CanTpRxPduPtr->SduLength - 2U)))
        {
          ValidFrameLength = TRUE;
        }
        else
        {
          ValidFrameLength = FALSE;
        }
      }
    }
    else
    {
      /* SF_DL greater than 8, not possible if CAN_DL <= 8 */
      ValidFrameLength = FALSE;
    }

  }
#if(CANTP_CANFD_ENABLED == STD_ON)
  else if((CanTpRxPduPtr->SduDataPtr[StartPosition_SF_N_PCI] & CANTP_N_PCI_SF_DL_MASK) == 0U) /* low nibble SF_DL is 0 and CAN_DL > 8 => CAN FD single frame */
  {
    /* Data length is next byte => CAN FD format is used */
    DataLength = CanTpRxPduPtr->SduDataPtr[StartPosition_SF_N_PCI + 1U];
    NPciHeaderSize = CANTP_SF_FD_NPCI_SIZE;

    if(DataLength == 0U)
    {
      /* if the CAN FD SF has SF_DL equal to zero, the frame shall be ignored */
      ValidFrameLength = FALSE;
    }
    else /* SF_DL greater or equal than 8 => CAN FD single frame format */
    {

      /* for STANDARD/NORMAL_FIXED addressing, SF_DL shall be smaller or equal than CAN_DL-2 (see Table 13 from ISO 15765-2:2016) */
      if(StartPosition_SF_N_PCI == 0U)
      {
        /* Respect minimum and maximum values for SF_DL (see Table 13) */
        if(DataLength <= (CanTpRxPduPtr->SduLength - 2U))
        {
          ValidFrameLength = TRUE;
        }
        else
        {
          ValidFrameLength = FALSE;
        }
      }
      else /* for EXTENDED addressing, SF_DL shall be smaller or equal than CAN_DL-3 (see Table 13 from ISO 15765-2:2016) */
      {
        /* Respect minimum and maximum values for SF_DL (see Table 13) */
        if(DataLength <= (CanTpRxPduPtr->SduLength - 3U))
        {
          ValidFrameLength = TRUE;
        }
        else
        {
          ValidFrameLength = FALSE;
        }
      }
    }
  }
#endif
  else
  {
    /* neither CAN 2.0 single frame format, nor CAN FD single frame format is used => invalid SF format */
    ValidFrameLength = FALSE;
  }


  if(ValidFrameLength)
  {
    if(!CanTp_CheckNPduLength(CanTpRxPduPtr->SduLength, RxNSduCfgPtr->PaddingActivation, RxNSduCfgPtr->NPduMaxLength))
    {
      ValidFrameLength = FALSE;
    }
    else
    {
      ValidFrameLength = TRUE; /* all checks passed */
    }
  }

  /* quick check if length is as expected */
  if(ValidFrameLength)
  {
    /* Data length is okay */
    Std_ReturnType Ret;
    /* Pointer to upper layer data */
    CONSTP2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) NSduDataPtr
      = &(CanTpRxPduPtr->SduDataPtr[StartPosition_SF_N_PCI + NPciHeaderSize]);

    /* setup channel and request buffer */
    ChannelPtr->TotalLength = DataLength;
    ChannelPtr->Ticks = CANTP_GET_RX_PDU_N_BR(RxNSduCfgPtr);

    DBG_CANTP_CHANNEL_STATE_GRP(
                                DBG_CANTP_GET_CHANNEL_INDEX(ChannelPtr),
                                ChannelPtr->State,
                                CANTP_CH_RX_BUFREQ_SF
                               );

    /* set to first state */
    TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_RX_BUFREQ_SF);

     Ret = CanTp_ProcessStartOfReception(ChannelPtr);

     if(Ret == E_OK)
     {
       /* Reception started. Try to copy FF data. */
       BufReq_ReturnType Ret_ReqRxBuffer;

       Ret_ReqRxBuffer = CanTp_ProcessStartOfReceptionData
                                  (
                                    ChannelPtr,
                                    NSduDataPtr,
                                    DataLength
                                  );
       switch(Ret_ReqRxBuffer)
       {
         case BUFREQ_OK:
         {
           /* single frame reception completed. The whole message was copied to
              upper layer successfully.
            */
           CanTp_StopChannel(ChannelPtr, CANTP_ULIND_STATUS, NTFRSLT_OK);

           break;
         }
         case BUFREQ_E_BUSY:
         {
           /* Store data and wait in state CANTP_CH_RX_BUFREQ_SF (already set) until
              buffer is available or timeout occurs.
            */
           TS_MemCpy(ChannelPtr->CanIfData, NSduDataPtr, DataLength);
           ChannelPtr->CanIfDLC = DataLength;

           /* no other reception accepted on same channel, with different N_AI
            * until ongoing reception ends or is aborted */
           CANTP_SET_FLAG(ChannelPtr->ConnectionFlags, CANTP_RXCHANN_LOCK_MASK);

           break;
         }
         default:
         {
           /* Unexpected return value. Close channel. */
           CanTp_StopChannel(ChannelPtr, CANTP_ULIND_STATUS, NTFRSLT_E_NOT_OK);
           break;
         }
       }
     }
  }
  else /* N-PDU has an invalid rx length */
  {
    /* Data length is not okay -> Free channel */
    CanTp_StopChannel(ChannelPtr, CANTP_ULIND_NONE, NTFRSLT_E_NOT_OK);

#if(CANTP_DEV_ERROR_DETECT == STD_ON)
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_RXINDICATION,
                            CANTP_E_INVALID_RX_LENGTH
                          );
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */
  }

}/* CanTp_ProcessReceivedSF() */

STATIC FUNC(void, CANTP_CODE) CanTp_ProcessReceivedFF
(
  P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpRxPduPtr,
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
)
{
  /* Pointer to N-Sdu config */
  CONSTP2CONST(CanTp_RxNSduConfigType, AUTOMATIC, CANTP_APPL_CONST) RxNSduCfgPtr =
      &CANTP_CFG_GET_RXNSDU_CONFIG(ChannelPtr->NSduId);

  boolean ValidFirstFrame; /* Indication for valid first frame. */

  uint16 MessageLength;
  uint8 FirstFrame_NPCI_Size = CANTP_FF_NPCI_SIZE;

  /* Start position of first N_PCI byte within the NPdu. */
  const uint8 StartPosition_FF_N_PCI = CANTP_GET_FIRST_N_PCI_BYTE(RxNSduCfgPtr);

  /* For checking whether the data can fit in a SF */
  uint8 NPciSize_SF;
  uint8 MaxUnSegMsgLength;


#if(CANTP_CANFD_ENABLED == STD_ON)
  if(CanTpRxPduPtr->SduLength > CANTP_NPDU_LENGTH_8)
  {
    NPciSize_SF = CANTP_SF_FD_NPCI_SIZE;
  }
  else
#endif /* CANTP_CANFD_ENABLED == STD_ON */
  {
    NPciSize_SF = CANTP_SF_NPCI_SIZE;
  }

  /* For checking whether data will fit in a SF. */
  MaxUnSegMsgLength = (uint8)CanTpRxPduPtr->SduLength
                      - NPciSize_SF
                      - CANTP_GET_FIRST_N_PCI_BYTE(RxNSduCfgPtr);

  /* Calculate the total length of the whole message.
     Total length: 12 bits (lower 4 bits of N_PCI byte 0 + all
     8 bits of N_PCI byte 1) The position of N_PCI depends on the addressing format.
   */
  MessageLength = ((uint16) (CanTpRxPduPtr->SduDataPtr[StartPosition_FF_N_PCI]) &
                             CANTP_N_PCI_FF_DL_MASK
                  );
  MessageLength = MessageLength << 8U;
  MessageLength = MessageLength
                  | ((uint16) (CanTpRxPduPtr->SduDataPtr[StartPosition_FF_N_PCI + 1U]));

#if(CANTP_CANFD_ENABLED == STD_ON)
  switch(CanTpRxPduPtr->SduLength)
  {
  case CANTP_NPDU_LENGTH_8:
  case CANTP_FD_NPDU_LENGTH_12:
  case CANTP_FD_NPDU_LENGTH_16:
  case CANTP_FD_NPDU_LENGTH_20:
  case CANTP_FD_NPDU_LENGTH_24:
  case CANTP_FD_NPDU_LENGTH_32:
  case CANTP_FD_NPDU_LENGTH_48:
  case CANTP_FD_NPDU_LENGTH_64:
#endif /* CANTP_CANFD_ENABLED == STD_ON */
  {
    if(RxNSduCfgPtr->Tatype != CANTP_PHYSICAL)
    {
      /* FF is only allowed for physical addressing (functional addressing is only allowed for SF) */
      ValidFirstFrame = FALSE;
    }
#if(CANTP_CANFD_ENABLED == STD_OFF)
    else if(CanTpRxPduPtr->SduLength != CANTP_NPDU_LENGTH_8)
    {
      ValidFirstFrame = FALSE;
    }
#endif /* (CANTP_CANFD_ENABLED == STD_OFF) */
     else if (MessageLength == 0U)
    {
      /* Big data FF format detected. */
      uint32 MessageLength_BigData = 0U;
      /* N_PCI header:
       * Upper nibble of 1st byte contains frame type (FF).
       * Lower nibble of 1st byte is 0.
       * 2nd byte is 0.
       * 3rd till 6th byte contain the data length (without padding bytes).
       * The offset of the message length is 2.
       */
      const PduLengthType PduLenghTypeMaxMask = 0U;
      /* Returns the maximum of PduLengthType */
      const PduLengthType PduLenghTypeMax = (PduLengthType)(~PduLenghTypeMaxMask);

      const uint8 StartPosition_FF_N_PCI_Length = StartPosition_FF_N_PCI + 2U;

      MessageLength_BigData |=
        (uint32)(((uint32)CanTpRxPduPtr->SduDataPtr[StartPosition_FF_N_PCI_Length]) << 24U);
      MessageLength_BigData |=
        (uint32)(((uint32)CanTpRxPduPtr->SduDataPtr[StartPosition_FF_N_PCI_Length + 1U]) << 16U);
      MessageLength_BigData |=
        (uint32)(((uint32)CanTpRxPduPtr->SduDataPtr[StartPosition_FF_N_PCI_Length + 2U]) << 8U);
      MessageLength_BigData |= (uint32)CanTpRxPduPtr->SduDataPtr[StartPosition_FF_N_PCI_Length + 3U];

      if(PduLenghTypeMax >= MessageLength_BigData)
      {
        ChannelPtr->TotalLength = (PduLengthType) MessageLength_BigData;
        FirstFrame_NPCI_Size = CANTP_FF_BIGDATA_NPCI_SIZE;
        ValidFirstFrame = TRUE;
      }
      else
      {
        /* N-SDU size in the FF is greater as allowed by PduLength of EcuC. */
        ValidFirstFrame = FALSE;
      }
    }
    else
    {
      /* Store calculated message length. */
      ChannelPtr->TotalLength = MessageLength;

      /* All data and message length checks passed. */
      ValidFirstFrame = TRUE;
    }

    if(ChannelPtr->TotalLength <= MaxUnSegMsgLength)
    {
      /* The total message length would fit into a single frame, no need of FF. */
      ValidFirstFrame = FALSE;
    }
  }
#if(CANTP_CANFD_ENABLED == STD_ON)
    break;
  default:
    ValidFirstFrame = FALSE;
    break;
  }
#endif /* CANTP_CANFD_ENABLED == STD_ON */


  /* check if length is as expected and communication is not functional */
  if(ValidFirstFrame)
  {
    Std_ReturnType Ret;

    /* NSdu data length of first frame */
    const uint8 DataLength_FF = (uint8)CanTpRxPduPtr->SduLength
                                - StartPosition_FF_N_PCI
                                - FirstFrame_NPCI_Size;
    /* Pointer to upper layer data */
    CONSTP2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) NSduDataPtr
        = &(CanTpRxPduPtr->SduDataPtr[StartPosition_FF_N_PCI + FirstFrame_NPCI_Size]);

    /* setup channel and request for buffer */
    ChannelPtr->ProcessedLength = 0U;
    ChannelPtr->WftCounter = CANTP_GET_RX_PDU_WFT_MAX(RxNSduCfgPtr);
    /* N_Br is the waiting time for the FC(WT) frame, N_Ar is the timeout,
       so buffer request and transmission must be finished after N_Br+N_Ar
     */
    ChannelPtr->Ticks = (uint32) CANTP_GET_RX_PDU_N_BR(RxNSduCfgPtr) +
                        CANTP_GET_RX_PDU_N_AR(RxNSduCfgPtr);

    /* Initialize block size (required for FC(wait) and FC(overflow)). */
    ChannelPtr->BlockSize = RxNSduCfgPtr->BS;

    DBG_CANTP_CHANNEL_STATE_GRP(
                                DBG_CANTP_GET_CHANNEL_INDEX(ChannelPtr),
                                ChannelPtr->State,
                                CANTP_CH_RX_BUFREQ_FF
                               );

    /* atomic assign */
    TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_RX_BUFREQ_FF);

#if(CANTP_CANFD_ENABLED == STD_ON)
    /* save the SduLength of the received FF, needed afterwards on intermediate and last CF reception */
    ChannelPtr->ReceivedSduLength = (uint8)CanTpRxPduPtr->SduLength;
#endif /* CANTP_CANFD_ENABLED */

    Ret = CanTp_ProcessStartOfReception(ChannelPtr);

    if(Ret == E_OK)
    {
      /* Reception started. Try to copy FF data. */
      BufReq_ReturnType Ret_ReqRxBuffer;

      /* no other reception accepted on same channel, with different N_AI
       * until ongoing reception ends or is aborted */
      CANTP_SET_FLAG(ChannelPtr->ConnectionFlags, CANTP_RXCHANN_LOCK_MASK);

      Ret_ReqRxBuffer = CanTp_ProcessStartOfReceptionData(
                                                           ChannelPtr,
                                                           NSduDataPtr,
                                                           DataLength_FF
                                                         );

      switch(Ret_ReqRxBuffer)
      {
        case BUFREQ_OK:
        {
          Std_ReturnType BSCalc_Ok; /* Indication of the actual BS calculation */

          /* Data of FF processed */
          ChannelPtr->ProcessedLength = DataLength_FF;

          /* Only calculate actual block size if FF data already processed. */
          BSCalc_Ok = CanTp_CalculateBlockSize(ChannelPtr, &(ChannelPtr->BlockSize));

          if(BSCalc_Ok == E_OK)
          {
            CanTp_TryToSendFC(ChannelPtr, CANTP_CH_FRAME_FC_CTS);
          }
          else
          {
            /* Wait in state CANTP_CH_RX_BUFREQ_FF (already set) until
               enough buffer for BS is available or timeout occurs.
            */
            /* Send wait frame if timeout already elapsed */
            CanTp_CheckAndHandleWaitFrameTimeOut(ChannelPtr);
          }
          break;
        }
        case BUFREQ_E_BUSY:
        {
          /* Store data and wait in state CANTP_CH_RX_BUFREQ_FF (already set) until
             buffer is available or timeout occurs.
           */
          TS_MemCpy(ChannelPtr->CanIfData, NSduDataPtr, DataLength_FF);
          ChannelPtr->CanIfDLC = DataLength_FF;

          /* no other reception accepted on same channel, with different N_AI
           * until ongoing reception ends or is aborted */
          CANTP_SET_FLAG(ChannelPtr->ConnectionFlags, CANTP_RXCHANN_LOCK_MASK);

          /* Send wait frame if timeout already elapsed */
          CanTp_CheckAndHandleWaitFrameTimeOut(ChannelPtr);
          break;
        }
        default:
        {
          /* Unexpected return value. Close channel. */
          CanTp_StopChannel(ChannelPtr, CANTP_ULIND_STATUS, NTFRSLT_E_NOT_OK);
          break;
        }
      }
    }
  }
  else
  {
    /* FF is invalid */
    CanTp_StopChannel(ChannelPtr, CANTP_ULIND_NONE, NTFRSLT_E_NOT_OK);

#if(CANTP_DEV_ERROR_DETECT == STD_ON)
    if(RxNSduCfgPtr->Tatype == CANTP_FUNCTIONAL)
    {
      /* wrong TA Type */
      CANTP_DET_REPORT_ERROR(
                              CANTP_MODULE_ID,
                              CANTP_INSTANCE_ID,
                              CANTP_API_RXINDICATION,
                              CANTP_E_INVALID_TATYPE
                            );
    }
    else
    {
      /* wrong length */
      CANTP_DET_REPORT_ERROR(
                              CANTP_MODULE_ID,
                              CANTP_INSTANCE_ID,
                              CANTP_API_RXINDICATION,
                              CANTP_E_INVALID_RX_LENGTH
                            );
    }
#endif /* CANTP_DEV_ERROR_DETECT == STD_ON */
  }

} /* CanTp_ProcessReceivedFF() */

STATIC FUNC(void, CANTP_CODE) CanTp_ProcessReceivedCF
(
  P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpRxPduPtr,
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
)
{
  CanTp_ChStateType ChannelState;
  TS_AtomicAssign8(ChannelState, ChannelPtr->State);

  /* Check for expected state and ignore frame if its state is not expected. */
  if((ChannelState == CANTP_CH_RX_RECEIVE_CF) || (ChannelState == CANTP_CH_RX_CONFIRM_FC_CTS))
  {
    /* ptr to N-Sdu config */
    CONSTP2CONST(CanTp_RxNSduConfigType, AUTOMATIC, CANTP_APPL_CONST) RxNSduCfgPtr
        = &CANTP_CFG_GET_RXNSDU_CONFIG(ChannelPtr->NSduId);

    /* Start position of first N_PCI byte within the NPdu. */
    const uint8 StartPosition_CF_N_PCI = CANTP_GET_FIRST_N_PCI_BYTE(RxNSduCfgPtr);

    /* if this is the wrong sequence number - stop channel */
    if(
         ChannelPtr->SequenceNumber !=
         (CanTpRxPduPtr->SduDataPtr[StartPosition_CF_N_PCI] & CANTP_N_PCI_CF_SN_MASK)
       )
    {
      /* CanTp.ASR40.CANTP314: check correctness of SN, abort with
         NTFRSLT_E_WRONG_SN if wrong.
       */
      CanTp_StopChannel(
                         ChannelPtr,
                         CANTP_ULIND_STATUS,
                         NTFRSLT_E_WRONG_SN
                       );
    }
    else
    {
      /* calculate remaining bytes / Delta needed for this N-Sdu */
      const PduLengthType Delta = ChannelPtr->TotalLength - ChannelPtr->ProcessedLength;
#if(CANTP_CANFD_ENABLED == STD_ON)
      const uint8 DataLen = ChannelPtr->ReceivedSduLength;
#else
      const uint8 DataLen = CANTP_NPDU_LENGTH_8;
#endif /* (CANTP_CANFD_ENABLED == STD_ON) */
      const uint8 SingleFrameLength = ( DataLen
                                        - StartPosition_CF_N_PCI
                                        - CANTP_SF_NPCI_SIZE );

      /* is this the last CF for the N-Sdu / does the Delta fit in a frame? */
      if(Delta <= SingleFrameLength)
      {
        CANTP_SET_FLAG(ChannelPtr->ConnectionFlags,CANTP_TXLASTFRAME_MASK);
        CanTp_ProcessReceivedLastCF(
                                     CanTpRxPduPtr,
                                     ChannelPtr,
                                     (uint8) Delta
                                   );
      }
      else
      {
        CanTp_ProcessReceivedIntermediateCF(
                                             CanTpRxPduPtr,
                                             ChannelPtr
                                           );
      }
    }
  }
  else
  {
    /* According to ISO15765 table 18 ignore unexpected CF */
  }
} /* CanTp_ProcessReceivedCF() */

STATIC FUNC(void, CANTP_CODE) CanTp_ProcessReceivedIntermediateCF
(
  P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpRxPduPtr,
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
)
{
  /* Pointer  to N-SDU config */
  CONSTP2CONST(CanTp_RxNSduConfigType, AUTOMATIC, CANTP_APPL_CONST) RxNSduCfgPtr
                            = &CANTP_CFG_GET_RXNSDU_CONFIG(ChannelPtr->NSduId);
#if(CANTP_CANFD_ENABLED == STD_ON)
  const uint8 DataLen = ChannelPtr->ReceivedSduLength;
#else
  const uint8 DataLen = CANTP_NPDU_LENGTH_8;
#endif /* (CANTP_CANFD_ENABLED == STD_ON) */


  /* This is an intermediate CF. The length must be the same as FF length and must not exceed configured PduLength */
  if(CanTpRxPduPtr->SduLength == DataLen)
  {
    BufReq_ReturnType Ret_CopyRxData;

    /* Start position of first N_PCI byte within the NPdu. */
    const uint8 StartPosition_CF_N_PCI = CANTP_GET_FIRST_N_PCI_BYTE(RxNSduCfgPtr);

    /* Pointer to upper layer data */
    CONSTP2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) NSduDataPtr
        = &(CanTpRxPduPtr->SduDataPtr[StartPosition_CF_N_PCI + CANTP_CF_NPCI_SIZE]);

    const uint8 DataLength = ( DataLen
                              - CANTP_CF_NPCI_SIZE
                              - StartPosition_CF_N_PCI );

    /* valid frame - increment sequence number to be correct for next CF */
    ChannelPtr->SequenceNumber++;

    /* wrap around sequence number */
    ChannelPtr->SequenceNumber &= CANTP_N_PCI_CF_SN_MASK;

    Ret_CopyRxData = CanTp_RxProcessData(ChannelPtr, NSduDataPtr, DataLength);

    if(BUFREQ_OK == Ret_CopyRxData)
    {
      /* one more frame of the actual block received successfully */
      ChannelPtr->ProcessedLength += (uint16) DataLength;

      if(0U == ChannelPtr->BlockSize)
      {
        /* Message will be sent within a big single block. No block checking required */
        /* Set ChannelPtr->ProcessedBlockFrames to a value != 0U (end of block indication)
            to avoid sending wait frames while waiting for a buffer.
         */
        ChannelPtr->ProcessedBlockFrames = 0x0FU;
      }
      else
      {
        ChannelPtr->ProcessedBlockFrames++;
      }

      if(ChannelPtr->ProcessedBlockFrames == ChannelPtr->BlockSize)
      {
        boolean ContinueWithNextBlock;

        /* this was the last block in this session */
        ChannelPtr->ProcessedBlockFrames = 0U;

        ContinueWithNextBlock = CanTp_VerifyBlockSize(ChannelPtr);

        if(ContinueWithNextBlock)
        {
          CanTp_TryToSendFC(ChannelPtr, CANTP_CH_FRAME_FC_CTS);
        }
        else
        {
          /* Not enough buffer for the next block available. Request for buffer at next
             main function call.
           */

          /* N_Br is the waiting time for the FC(WT) frame, N_Ar is the
             timeout, so buffer request and transmission must be finished
             after N_Br+N_Ar
           */
          ChannelPtr->Ticks =
            (uint32) CANTP_GET_RX_PDU_N_BR(&CANTP_CFG_GET_RXNSDU_CONFIG(ChannelPtr->NSduId))
            + CANTP_GET_RX_PDU_N_AR(&CANTP_CFG_GET_RXNSDU_CONFIG(ChannelPtr->NSduId));

          /* reset FC(WT) counter */
          ChannelPtr->WftCounter =
            CANTP_GET_RX_PDU_WFT_MAX(&CANTP_CFG_GET_RXNSDU_CONFIG(ChannelPtr->NSduId));

          DBG_CANTP_CHANNEL_STATE_GRP(
                                      DBG_CANTP_GET_CHANNEL_INDEX(ChannelPtr),
                                      ChannelPtr->State,
                                      CANTP_CH_RX_BUFREQ_CF
                                     );

          /* Set state to request buffer for next block. */
          TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_RX_BUFREQ_CF);

          /* Send wait frame if timeout already elapsed */
          CanTp_CheckAndHandleWaitFrameTimeOut(ChannelPtr);
        }
      }
      else
      {
        /* CF within a block received. Wait for next CF */
        ChannelPtr->Ticks = CANTP_GET_RX_PDU_N_CR(&CANTP_CFG_GET_RXNSDU_CONFIG(ChannelPtr->NSduId));

        DBG_CANTP_CHANNEL_STATE_GRP(
                                    DBG_CANTP_GET_CHANNEL_INDEX(ChannelPtr),
                                    ChannelPtr->State,
                                    CANTP_CH_RX_RECEIVE_CF
                                   );

        TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_RX_RECEIVE_CF);
      }
    }
    else
    {
      /* Error during processing of CF data. */
      CanTp_StopChannel(ChannelPtr, CANTP_ULIND_STATUS, NTFRSLT_E_NOT_OK);
    }
  }
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
  else /* if SDU length was wrong */
  {
    /* length is invalid -> ignore frame but DET */
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_RXINDICATION,
                            CANTP_E_INVALID_RX_LENGTH
                          );
  }
#endif
}

STATIC FUNC(void, CANTP_CODE) CanTp_ProcessReceivedLastCF
(
  P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpRxPduPtr,
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  uint8 DataLength
)
{
  boolean ValidFrameLength; /* Indication if last CF length is in valid range. */

  /* ptr to N-Sdu config */
  CONSTP2CONST(CanTp_RxNSduConfigType, AUTOMATIC, CANTP_APPL_CONST) RxNSduCfgPtr
      = &CANTP_CFG_GET_RXNSDU_CONFIG(ChannelPtr->NSduId);

  /* Start position of first N_PCI byte within the NPdu. */
  const uint8 StartPosition_CF_N_PCI = CANTP_GET_FIRST_N_PCI_BYTE(RxNSduCfgPtr);

  /* Minimum required N_SDU length to hold header and data */
  const uint8 MinReqSduLength = StartPosition_CF_N_PCI + CANTP_CF_NPCI_SIZE + DataLength;

  /* Function shall never be called with nod data. This indicates a bug. */
  CANTP_PRECONDITION_ASSERT((DataLength > 0U), CANTP_INTERNAL_API_ID);

    if(CanTpRxPduPtr->SduLength < MinReqSduLength)
    {
      /* SduLength shall be at least big enough to hold the
       header and the encoded data length of N_PCI
       */
      ValidFrameLength = FALSE;
    }
    else if(!CanTp_CheckNPduLength(CanTpRxPduPtr->SduLength, RxNSduCfgPtr->PaddingActivation, RxNSduCfgPtr->NPduMaxLength))
    {
      ValidFrameLength = FALSE;
    }
    else
    {
      ValidFrameLength = TRUE; /* all checks passed */
    }

  /* Only process if last CF length is valid */
  if(ValidFrameLength)
  {
    BufReq_ReturnType Ret_CopyRxData;

    /* Pointer to upper layer data */
    CONSTP2VAR(uint8, AUTOMATIC, CANTP_APPL_DATA) NSduDataPtr
        = &(CanTpRxPduPtr->SduDataPtr[StartPosition_CF_N_PCI + CANTP_CF_NPCI_SIZE]);

    Ret_CopyRxData = CanTp_RxProcessData(ChannelPtr, NSduDataPtr, DataLength);

    if(BUFREQ_OK == Ret_CopyRxData)
    {
      /* Message reception completed. The whole message was copied to
         upper layer successfully.
       */
      CanTp_StopChannel(ChannelPtr, CANTP_ULIND_STATUS, NTFRSLT_OK);
    }
    else
    {
      /* Error during processing of last CF data. */
      CanTp_StopChannel(ChannelPtr, CANTP_ULIND_STATUS, NTFRSLT_E_NOT_OK);
    }

    CANTP_CLEAR_FLAG(ChannelPtr->ConnectionFlags,CANTP_TXLASTFRAME_MASK);
  }
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
  else
  {
    /* length is invalid -> ignore frame but DET */
    CANTP_DET_REPORT_ERROR(
                            CANTP_MODULE_ID,
                            CANTP_INSTANCE_ID,
                            CANTP_API_RXINDICATION,
                            CANTP_E_INVALID_RX_LENGTH
                          );
  }
#endif
}

STATIC FUNC(void, CANTP_CODE) CanTp_ProcessReceivedFC
(
  P2CONST(PduInfoType, AUTOMATIC, CANTP_APPL_DATA) CanTpRxPduPtr,
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr
)
{
  /* ptr to N-Sdu config */
  CONSTP2CONST(CanTp_TxNSduConfigType, AUTOMATIC, CANTP_APPL_CONST) TxNSduCfgPtr
      = &CANTP_CFG_GET_TXNSDU_CONFIG(ChannelPtr->NSduId);

  /* Start position of first N_PCI byte within the NPdu. */
  const uint8 StartPosition_FC_N_PCI = CANTP_GET_FIRST_N_PCI_BYTE(TxNSduCfgPtr);

  CanTp_ChStateType ChannelState;
  TS_AtomicAssign8(ChannelState, ChannelPtr->State);

  /* check for correct channel state and ignore frame if it is not */
  switch(ChannelState)
  {
    case CANTP_CH_TX_RECEIVE_FC: /* Fall through */
    case CANTP_CH_TX_CONFIRM_FF: /* Fall through */
    case CANTP_CH_TX_CONFIRM_CF:
    {
      boolean ExpectedFrame = FALSE; /* Verify if frame is expected */

      if(ChannelPtr->BlockSize == 0U)
      {
        ExpectedFrame = FALSE; /* No FC needed. */
      }
      else if(ChannelPtr->BlockSize != ChannelPtr->ProcessedBlockFrames)
      {
        ExpectedFrame = FALSE; /* Not at the end of a block or after a FF. */
      }
      else if(ChannelPtr->TotalLength == ChannelPtr->ProcessedLength)
      {
        ExpectedFrame = FALSE; /* All data are processed. */
      }
      else
      {
        ExpectedFrame = TRUE; /* All checks passed. */
      }

      /* Only process if FC length is valid */
      if(ExpectedFrame)
      {
        /* ptr to can data */
        CONSTP2CONST(uint8, AUTOMATIC, CANTP_APPL_DATA) CanDataPtr
            = CanTpRxPduPtr->SduDataPtr;

        /* check flow status */
        switch(CanDataPtr[StartPosition_FC_N_PCI])
        {
        case CANTP_N_PCI_FC_CTS:
        {
          uint8 DataIndex = StartPosition_FC_N_PCI;  /* Can data index */

#if(CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON)
          uint8 StMinValue;

          TS_AtomicAssign8(StMinValue, CanTp_TxNSduStMinValues[ChannelPtr->NSduId]);
#endif /* (CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON) */

          ChannelPtr->ProcessedBlockFrames = 0U; /* Reset frame counter within a block */

          TS_AtomicAssign8(ChannelState, ChannelPtr->State);
          /* !LINKSTO CanTp.ISO2016.DynamicStminBS, 1 */
          /* BS shall be updated from each FC (CTS) received */
          DataIndex++; /* next byte after N_PCI is BS */
          ChannelPtr->BlockSize = CanDataPtr[DataIndex];

          DataIndex++; /* next byte after BS is StMin*/

#if(CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON)
          /* check if the STmin was changed before by CanTp_ChangeTxParameter() */
          if(0xFFU != StMinValue)
          {
            ChannelPtr->StMin = StMinValue;
          }
          else
#endif /* (CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON) */
          {
            /* if this is the first FC, indicated by StMin = 0xFF (reserved) */
            ChannelPtr->StMin = CanDataPtr[DataIndex];
          }

#if(CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON)
          /* Store the received STmin */
          ChannelPtr->LastStMinFC = CanDataPtr[DataIndex];

          /* if LastStMin is not valid */
          if(
              (
                  (ChannelPtr->LastStMinFC >= 0x80U)
                  && (ChannelPtr->LastStMinFC <= 0xF0U)
              )
              || (ChannelPtr->LastStMinFC >= 0xFAU)
          )
          {
            /* set LastStMin to max value as specified by ISO */
            ChannelPtr->LastStMinFC = 0x7FU;
          }
#endif /* (CANTP_CHANGE_TX_PARAMETER_REQ_API == STD_ON) */

          /* if StMin is not valid */
          if(
              (
                  (ChannelPtr->StMin >= 0x80U)
                  && (ChannelPtr->StMin <= 0xF0U)
              )
              || (ChannelPtr->StMin >= 0xFAU)
          )
          {
            /* set StMin to max value as specified by ISO */
            ChannelPtr->StMin = 0x7FU;
          }

          /* Check if StMin is used. */
          if (ChannelPtr->StMin != 0u)
          {
            /* Set the flag to round up the STmin between the first CF and the second. */
            CANTP_SET_FLAG(ChannelPtr->ConnectionFlags,CANTP_STMIN_ROUNDUP_MASK);
          }

          ChannelPtr->Ticks = CANTP_GET_TX_PDU_N_CS(TxNSduCfgPtr);

          /* Send next frame immediately if FC(CTS) is received. In case that
               Tx confirmation is outstanding, delay the transmission until it arrives. */
          if(ChannelState == CANTP_CH_TX_RECEIVE_FC)
          {
            DBG_CANTP_CHANNEL_STATE_GRP(
                DBG_CANTP_GET_CHANNEL_INDEX(ChannelPtr),
                ChannelPtr->State,
                CANTP_CH_TX_BUFREQ_CF
            );

            TS_AtomicAssign8(ChannelPtr->State, CANTP_CH_TX_BUFREQ_CF);

            CanTp_TxStateHandling(ChannelPtr,FALSE);
          }
          else
          {
            /* Remember that the FC has been received. */
            CANTP_SET_FLAG(ChannelPtr->ConnectionFlags, CANTP_FCRECEIVED_MASK);
          }
          break;
        }
        case CANTP_N_PCI_FC_WT:
        {
          /* wait; restart N_Bs timeout */
          ChannelPtr->Ticks = CANTP_GET_TX_PDU_N_BS(TxNSduCfgPtr);
          break;
        }
        case CANTP_N_PCI_FC_OVFLW:
        {
          /* CanTp.ASR40.CANTP309: FC OVFLW received -> report to PduR
               with result NTFRSLT_E_NO_BUFFER
           */
          CanTp_StopChannel(
              ChannelPtr,
              CANTP_ULIND_STATUS,
              NTFRSLT_E_NO_BUFFER
          );
          break;
        }
        default:
        {
          /* CanTp.ASR40.CANTP317: on invalid flow control status, stop channel
               and report result NTFRSLT_E_INVALID_FS
           */
          CanTp_StopChannel(
              ChannelPtr,
              CANTP_ULIND_STATUS,
              NTFRSLT_E_INVALID_FS
          );
          break;
        }
        }
      }
      break;
    }
    default:
    {
      /* Ignore FC frame if not expected. */
      break;
    }
  }
}

STATIC FUNC(boolean, CANTP_CODE) CanTp_CheckNPduLength
(
  PduLengthType NPduLength,
  uint8 Padding,
  uint8 MaxLength
)
{
  boolean ValidNPduLength;

#if(CANTP_CANFD_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(MaxLength);
#else
  if ( Padding == CANTP_ON_CAN_CAN_FD )
  {
    if(NPduLength == MaxLength)
    {
      ValidNPduLength = TRUE;
    }
    else
    {
      ValidNPduLength = FALSE;
    }
  }
  else /* Padding == CANTP_ON or Padding == CANTP_OFF */
#endif /* (CANTP_CANFD_ENABLED == STD_ON) */
{
  if(CANTP_NPDU_LENGTH_8 > NPduLength)
  {
    if(Padding == CANTP_ON)
    {
      ValidNPduLength = FALSE;
    }
    else
    {
      ValidNPduLength = TRUE;
    }
  }
  else /* NPduLength is greater or equal than eight */
  {
    if(CANTP_NPDU_LENGTH_8 == NPduLength)
    {
      ValidNPduLength = TRUE;
    }
#if(CANTP_CANFD_ENABLED == STD_ON)
    else if(CANTP_FD_NPDU_LENGTH_12 == NPduLength)
    {
      ValidNPduLength = TRUE;
    }
    else if(CANTP_FD_NPDU_LENGTH_16 == NPduLength)
    {
      ValidNPduLength = TRUE;
    }
    else if(CANTP_FD_NPDU_LENGTH_20 == NPduLength)
    {
      ValidNPduLength = TRUE;
    }
    else if(CANTP_FD_NPDU_LENGTH_24 == NPduLength)
    {
      ValidNPduLength = TRUE;
    }
    else if(CANTP_FD_NPDU_LENGTH_32 == NPduLength)
    {
      ValidNPduLength = TRUE;
    }
    else if(CANTP_FD_NPDU_LENGTH_48 == NPduLength)
    {
      ValidNPduLength = TRUE;
    }
    else if(CANTP_FD_NPDU_LENGTH_64 == NPduLength)
    {
      ValidNPduLength = TRUE;
    }
#endif /* (CANTP_CANFD_ENABLED == STD_ON) */
    else
    {
      ValidNPduLength = FALSE;
    }
  }
}

  return ValidNPduLength;
}

#if(CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON)

/** \brief Get the locked channel or the first released one
 **
 ** This function returns the index of the channel.
 **
 ** \param[in]  RxNSduId      Currently received RxNSduId
 ** \param[out] ChannelIdxPtr Address to write the channel index to
 **
 ** \return Result of the function
 ** \retval E_OK The first released channel or the locked one corresponding to NSDUId,
 **               was found and written to the address
 ** \retval E_NOT_OK No Rx channel was found
 */
FUNC(Std_ReturnType, CANTP_CODE) CanTp_GetRxChannelIndex
(
  P2VAR(CanTp_ChType, AUTOMATIC, AUTOMATIC) ChannelIdxPtr,
  PduIdType RxNSduId
)
{
  Std_ReturnType Result = E_NOT_OK;
  CanTp_ChType idx;
  CanTp_ChStateType CurrentState;
  CanTp_ChType NoOfChannels;

  NoOfChannels = CANTP_MAX_TX_CHANNELS;

  for(idx = CANTP_MAX_TX_CHANNELS; idx < CANTP_MAX_PARALLEL_CHANNELS; idx++)
  {
    P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr;
    ChannelPtr = &CanTp_Channel[idx];

    TS_AtomicAssign8(CurrentState, (CanTp_ChStateType) (ChannelPtr)->State);

    if(CANTP_CH_GET_MODE(CurrentState) == CANTP_CH_MODE_RX)
    {
      /* Check if locked channels, were already locked for the current RxNSduId */
      if(ChannelPtr->NSduId == RxNSduId)
      {
        /* channel was found */
        Result = E_OK;
        break;
      }
    }
    /* count channels */
    NoOfChannels++;
  }

  /* search the first free channel */
  if(E_NOT_OK == Result)
  {
    NoOfChannels = CANTP_MAX_TX_CHANNELS;

    for(idx = CANTP_MAX_TX_CHANNELS; idx < CANTP_MAX_PARALLEL_CHANNELS; idx++)
    {
      P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr;
      ChannelPtr = &CanTp_Channel[idx];

      TS_AtomicAssign8(CurrentState, (CanTp_ChStateType) (ChannelPtr)->State);

      if(CANTP_CH_GET_MODE(CurrentState) != CANTP_CH_MODE_RX)
      {
        /* channel was found */
        Result = E_OK;
        break;
      }

      /* count channels */
      NoOfChannels++;
    }
  }

  if(Result == E_OK)
  {
    /* save channel's index */
    *ChannelIdxPtr = NoOfChannels;
  }

  return Result;
}


/** \brief Get the Rx channel for the corresponding N-SDU
 **
 ** This function returns the index of the free channel.
 **
 ** \param[in]  RxNSduId          RxNSduId for the current RxNSdu that shall be transmitted
 ** \param[out] ChannelIdxPtr     Address to write the channel index to
 **
 ** \return Result of the function
 ** \retval E_OK     The locked channel of the N-SDU was found and written to the address
 ** \retval E_NOT_OK No channel involved in a reception with the RxNSduId was found
 */
FUNC(Std_ReturnType, CANTP_CODE) CanTp_FindRxNsduChannelIndex
(
  P2VAR(CanTp_ChType, AUTOMATIC, AUTOMATIC) ChannelIdxPtr,
  PduIdType RxNSduId
)
{
  Std_ReturnType Result = E_NOT_OK;
  CanTp_ChType idx;
  CanTp_ChStateType CurrentState;
  CanTp_ChType NoOfChannels = CANTP_MAX_TX_CHANNELS;
  boolean NSduFound = FALSE;

  /* search through all rx channels */
  for(idx = CANTP_MAX_TX_CHANNELS; idx < CANTP_MAX_PARALLEL_CHANNELS; idx++)
  {
    P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr;
    ChannelPtr = &CanTp_Channel[idx];

    TS_AtomicAssign8(CurrentState, (CanTp_ChStateType) (ChannelPtr)->State);

    /* check if there was a locked Rx Channel */
    if(CANTP_CH_GET_MODE(CurrentState) == CANTP_CH_MODE_RX)
    {
      /* Check if locked channels, were already locked for the current RxNSduId */
      if(ChannelPtr->NSduId == RxNSduId)
      {
        /* channel was found */
        NSduFound = TRUE;
        break;
      }
      /* count channels */
      NoOfChannels++;
    }
  }

  /* store the channel index only if no N-SDU was found */
  if((boolean)FALSE == NSduFound)
  {
    /* save channel's index */
    *ChannelIdxPtr = NoOfChannels;

    Result = E_OK;
  }

  /* result shall remain E_NOT_OK, because the parameter should not be changed,
     if the reception is in progress */
  return Result;
}


/** \brief Get the Rx channel for the corresponding N-SDU
 **
 ** This function returns the index of the locked channel.
 **
 ** \param[in]  RxNSduId          RxNSduId for the current RxNSdu that shall be transmitted
 ** \param[out] ChannelIdxPtr     Address to write the channel index to
 **
 ** \return Result of the function
 ** \retval E_OK     The locked channel of the N-SDU was found and written to the address
 ** \retval E_NOT_OK No channel involved in a reception with the RxNSduId was found
 */
FUNC(Std_ReturnType, CANTP_CODE) CanTp_FindLockedRxChannelIndex
(
  P2VAR(CanTp_ChType, AUTOMATIC, AUTOMATIC) ChannelIdxPtr,
  PduIdType RxNSduId
)
{
  Std_ReturnType Result = E_NOT_OK;
  CanTp_ChType idx;
  CanTp_ChStateType CurrentState;
  CanTp_ChType NoOfChannels = CANTP_MAX_TX_CHANNELS;
  boolean NSduFound = FALSE;

  for(idx = CANTP_MAX_TX_CHANNELS; idx < CANTP_MAX_PARALLEL_CHANNELS; idx++)
  {
    P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr;
    ChannelPtr = &CanTp_Channel[idx];

    TS_AtomicAssign8(CurrentState, (CanTp_ChStateType) (ChannelPtr)->State);

    if(CANTP_CH_GET_MODE(CurrentState) == CANTP_CH_MODE_RX)
    {
      /* Check if locked channels, were already locked for the current RxNSduId */
      if(ChannelPtr->NSduId == RxNSduId)
      {
        /* channel was found */
        NSduFound = TRUE;
        break;
      }
      /* count channels */
      NoOfChannels++;
    }
  }

  /* store the channel index only if no N-SDU was found */
  if((boolean)TRUE == NSduFound)
  {
    /* save channel's index */
    *ChannelIdxPtr = NoOfChannels;

    Result = E_OK;
  }

  return Result;
}

#endif /* CANTP_PARALLEL_CHANNELS_ENABLED == STD_ON */

/** \brief CanTp_ValidateFCLength()
 **
 ** Internal function to validate FC length
 **
 **
 ** \param[in] NSduId           - ID of the NSdu.
 ** \param[in] SduLength        - Parameter indicates Sdu Length.
 ** \param[in] AddressingFormat - Parameter indicates used addressing format.

 ** \retval TRUE: FCLength is valid.
 */
STATIC FUNC(boolean, CANTP_CODE) CanTp_ValidateFCLength
(
  PduIdType NSduId,
  PduLengthType SduLength,
  const uint8  AddressingFormat
)
{
  boolean FCValidLength = FALSE;
  PduLengthType NPduMaxLength = CANTP_NPDU_LENGTH_8;
  if(CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).PaddingActivation == CANTP_OFF)
  {
    PduLengthType FCMinLength;
    if(CANTP_IS_NORMAL_ADDRESSING(AddressingFormat))
    {
       /* N-PDU length must be the address byte (if exist) + FC N_PCI header */
       FCMinLength = CANTP_FC_NPCI_SIZE;
    }
     else
    {
      /* N-PDU length must be the address byte (if exist) + FC N_PCI header */
      FCMinLength = 1U + CANTP_FC_NPCI_SIZE;
    }
    if((SduLength >= FCMinLength) && (SduLength <= NPduMaxLength))
    {
      FCValidLength = TRUE;
    }
  }
#if (CANTP_CANFD_ENABLED == STD_OFF)
  else
  {
    if(SduLength == NPduMaxLength)
    {
      FCValidLength = TRUE;
    }
  }
#else
  else if(CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).PaddingActivation == CANTP_ON)
  {
    if(SduLength > CANTP_NPDU_LENGTH_8)
    {
      /* Padding required */
      NPduMaxLength = CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).LSduMaxLength;
    }
    if(SduLength == NPduMaxLength)
    {
      FCValidLength = TRUE;
    }
  }
  else /* CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).PaddingActivation == CANTP_ON_CAN_CAN_FD */
  {
    if((CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).CanIdType == CANTP_TX_STANDARD_FD_CAN) || (CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).CanIdType == CANTP_TX_EXTENDED_FD_CAN))
    {
      /* Padding required */
      NPduMaxLength = CANTP_FD_NPDU_LENGTH_64;
    }
    if(SduLength == NPduMaxLength)
    {
      FCValidLength = TRUE;
    }
  }
#endif /* (CANTP_CANFD_ENABLED == STD_OFF) */

  return FCValidLength;
}

#if(CANTP_DYNAMIC_ID_SUPPORT == STD_ON)
STATIC FUNC(boolean, CANTP_CODE) CanTp_ProcessReceivedMetaData
(
  PduIdType NSduId,
  uint8 NSaCfg,
  uint8 FrameType,
  P2VAR(uint16, AUTOMATIC, CANTP_APPL_DATA) NAIPtr
)
{
  boolean IsValidMetaData = FALSE;
  PduIdType MetaDataLLPduId;
  PduIdType MetaDataULPduId;
  uint32 Metadata = 0U;
  uint8 TAtype;
  uint8 TATypeCfg;
  uint8 NTaCfg;
  PduInfoType CanTpRxPdu;

  if(FrameType == CANTP_N_PCI_TYPE_FC)
  {
     /* Pointer to N-Sdu config */
     CONSTP2CONST(CanTp_TxNSduConfigType, AUTOMATIC, CANTP_APPL_CONST) TxNSduCfgPtr
                               = &CANTP_CFG_GET_TXNSDU_CONFIG(NSduId);

    MetaDataULPduId = TxNSduCfgPtr->MetadataULPduId;
    MetaDataLLPduId = TxNSduCfgPtr->MetadataLLFcPduId;
    TATypeCfg = TxNSduCfgPtr->Tatype;
    NTaCfg = TxNSduCfgPtr->N_TA_AE;
  }
  else
  {
    /* Pointer to N-Sdu config */
    CONSTP2CONST(CanTp_RxNSduConfigType, AUTOMATIC, CANTP_APPL_CONST) RxNSduCfgPtr
                        = &CANTP_CFG_GET_RXNSDU_CONFIG(NSduId);

    MetaDataULPduId = RxNSduCfgPtr->MetadataULPduId;
    MetaDataLLPduId = RxNSduCfgPtr->MetadataLLPduId;
    TATypeCfg = RxNSduCfgPtr->Tatype;
    NTaCfg = RxNSduCfgPtr->N_TA_AE;
  }

  /* !LINKSTO CanTp.EB.MetaData.GetRxNTaFromCanId, 1 */
  /* Get the metadata from the CanId */
  (void)EcuC_GetMetaDataCanId(MetaDataLLPduId, &CanTpRxPdu, &Metadata);

  /* get the TAtype */
  TAtype = (uint8)((Metadata >> 16U) & 0x01U);

  /* check the configured TaType against the received value from the CanId */
  if(TATypeCfg == CANTP_PHYSICAL)
  {
    /* check if the N_TAtype set in the CanId indicates a physical message */
    if(TAtype == CANTP_PHYSICAL)
    {
      IsValidMetaData = TRUE;
    }
  }
  else
  {
    /* check if the N_TAtype set in the CanId indicates a functional message */
    if(TAtype == CANTP_FUNCTIONAL)
    {
      IsValidMetaData = TRUE;
    }
  }

  /* if the TAtype is valid, continue to process the TA and SA */
  if(IsValidMetaData)
  {
    PduInfoType MetadataPduInfo;
    uint16 ReceivedCanIdNSa;
    uint16 ReceivedCanIdNTa;

    /* extract the SA and TA */
    ReceivedCanIdNSa = CANTP_GET_RX_METADATA_N_SA(FrameType, Metadata);

    /* !LINKSTO CanTp.EB.MetaData.GetRxNTaFromCanId, 1 */
    /* get the target address from the CanId */
    ReceivedCanIdNTa = CANTP_GET_RX_METADATA_N_TA(FrameType, Metadata);

    switch(FrameType)
    {
      case CANTP_N_PCI_TYPE_FF:
      case CANTP_N_PCI_TYPE_SF:
      {
#if(CANTP_GENERIC_CONNECTION_SUPPORT == STD_OFF)
        /* !LINKSTO CanTp.EB.MetaData.GetRxNSaGenConnectionOff, 1 */
        if((ReceivedCanIdNSa == NSaCfg) && (ReceivedCanIdNTa == NTaCfg))
#else
        TS_PARAM_UNUSED(NSaCfg);

        /* !LINKSTO CanTp.EB.MetaData.GetRxNSaGenConnectionOn, 1 */
        if(ReceivedCanIdNTa == NTaCfg)
#endif /* CANTP_GENERIC_CONNECTION_SUPPORT == STD_OFF */
        {
          /* set the Address info for SF/FF for later N-PDU arrivals handling */
          *NAIPtr = (uint16)(Metadata & 0xFFFFU);

          /* Set the Target address and Source address */
          /* !LINKSTO CanTp.EB.MetaData.SetRxNSa, 1 */
          (void)EcuC_SetMetaDataSourceAddr(MetaDataULPduId, &MetadataPduInfo, ReceivedCanIdNSa);
          /* !LINKSTO CanTp.EB.MetaData.SetRxNTa,1 */
          (void)EcuC_SetMetaDataTargetAddr(MetaDataULPduId, &MetadataPduInfo, ReceivedCanIdNTa);
        }
        else
        {
          IsValidMetaData = FALSE;
        }
        break;
      }
      case CANTP_N_PCI_TYPE_CF:
      case CANTP_N_PCI_TYPE_FC:
      {
        uint16 NSa;
        uint16 NTa;

        /* Get the Target address and Source address set at FF reception */
        (void)EcuC_GetMetaDataSourceAddr(MetaDataULPduId, &MetadataPduInfo, &NSa);

        (void)EcuC_GetMetaDataTargetAddr(MetaDataULPduId, &MetadataPduInfo, &NTa);

        /* check if the SA and TA match to the FF values */
        /* !LINKSTO CanTp.EB.MetaData.CheckRxNTa, 1 */
        /* !LINKSTO CanTp.EB.MetaData.CheckRxFcNTa, 1 */
        /* !LINKSTO CanTp.EB.MetaData.CheckRxNSa, 1 */
        /* !LINKSTO CanTp.EB.MetaData.CheckRxFcNSa, 1 */
        if((NSa != ReceivedCanIdNSa) || (NTa != ReceivedCanIdNTa))
        {
          IsValidMetaData = FALSE;
        }
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
  }

  return IsValidMetaData;
}

#endif /* CANTP_DYNAMIC_ID_SUPPORT == STD_ON */

STATIC FUNC(boolean, CANTP_CODE) CanTp_ValidateNAI
(
  P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChannelPtr,
  PduIdType NSduId,
  PduIdType CanDataNAI,
  uint8 FrameType
)
{
  uint8 AddressingFormat;
  PduIdType NPduId;
  boolean IgnoreMsgFlag = FALSE;

  /* If an NSdu for an FC was found it must be Tx, otherwise it's Rx */
  if(FrameType == CANTP_N_PCI_TYPE_FC)
  {
    NPduId = CANTP_CFG_GET_TXNSDU_CONFIG(NSduId).NPduIdFc;
    AddressingFormat = CANTP_GET_ADDR_FORMAT(&(CANTP_CFG_GET_RXNPDU_TABLE(CANTP_CFG_GET_RXNPDU_LOOKUPTABLE(NPduId))));

  }
  else
  {
    NPduId = CANTP_CFG_GET_RXNSDU_CONFIG(NSduId).NPduIdxF;
    AddressingFormat = CANTP_GET_ADDR_FORMAT(&CANTP_CFG_GET_RXNSDU_CONFIG(NSduId));
  }

  if((FrameType == CANTP_N_PCI_TYPE_FF) || (FrameType == CANTP_N_PCI_TYPE_SF))
  {
    PduIdType N_AI = 0U;

    /* save the N_AI based on the addressing format */
    if(AddressingFormat == CANTP_STANDARD)
    {
      N_AI = NPduId;
    }
#if(CANTP_DYNAMIC_ID_SUPPORT == STD_ON)
    else if(AddressingFormat == CANTP_NORMALFIXED)
    {
      uint16 AddrInfo;
      P2CONST(CanTp_RxNPduTableType, AUTOMATIC, CANTP_APPL_CONST) RxNPduTableEntry =
                                &(CANTP_CFG_GET_RXNPDU_TABLE(CANTP_CFG_GET_RXNPDU_LOOKUPTABLE(NPduId)));
      boolean IsValidMetaData = CanTp_ProcessReceivedMetaData(NSduId, RxNPduTableEntry->N_SA_AE, FrameType, &AddrInfo);

      if(IsValidMetaData == (boolean)FALSE)
      {
        /* ignore message */
        IgnoreMsgFlag = TRUE;
      }
      else
      {
        N_AI = AddrInfo;
      }
    }
#endif /* CANTP_DYNAMIC_ID_SUPPORT == STD_ON */
    else
    {
      N_AI = CanDataNAI;
    }

#if(CANTP_DYNAMIC_ID_SUPPORT == STD_ON)
    if(IgnoreMsgFlag == (boolean)FALSE)
#endif /* CANTP_DYNAMIC_ID_SUPPORT == STD_ON */
    {
      if((CANTP_CHECK_FLAG(ChannelPtr->ConnectionFlags, CANTP_RXCHANN_LOCK_MASK)) == (boolean)FALSE)
      {
        /* save the addressing information of the ongoing reception */
        ChannelPtr->RxNAILock = N_AI;
      }
      else
      {
        if(ChannelPtr->RxNAILock != N_AI)
        {
          /* ignore message */
          IgnoreMsgFlag = TRUE;
        }
      }
    }
  }
#if(CANTP_DYNAMIC_ID_SUPPORT == STD_ON)
  else
  {
    if(AddressingFormat == CANTP_NORMALFIXED)
    {
      uint16 AddrInfo;
      P2CONST(CanTp_RxNPduTableType, AUTOMATIC, CANTP_APPL_CONST) RxNPduTableEntry =
                          &(CANTP_CFG_GET_RXNPDU_TABLE(CANTP_CFG_GET_RXNPDU_LOOKUPTABLE(NPduId)));
      boolean IsValidMetaData = CanTp_ProcessReceivedMetaData(NSduId, RxNPduTableEntry->N_SA_AE, FrameType, &AddrInfo);

      if(!IsValidMetaData)
      {
        /* ignore message */
        IgnoreMsgFlag = TRUE;
      }
    }
  }
#endif /* CANTP_DYNAMIC_ID_SUPPORT == STD_ON */

  return IgnoreMsgFlag;
}

#endif /* CANTP_JUMPTABLE_MODE != CANTP_JUMPTABLE_CLIENT */

#if(TS_MERGED_COMPILE == STD_OFF)
#define CANTP_STOP_SEC_CODE
#include <CanTp_MemMap.h>
#endif /* TS_MERGED_COMPILE == STD_OFF */

/*==================[end of file]============================================*/
