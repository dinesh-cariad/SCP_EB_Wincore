/**
 * \file
 *
 * \brief AUTOSAR Xcp
 *
 * This file contains the implementation of the AUTOSAR
 * module Xcp.
 *
 * \version 2.12.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/*
 *  MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 13.5 (required)
 * The right hand operand of a logical && or || operator shall not contain persistent side effects.
 *
 * Reason:
 * The functions which are called to calculate the operands do not
 * contain side effects.
 */

/*==================[inclusions]============================================*/

#include <Xcp.h>               /* Module public API */
#if ( (XCP_DEFAULT_XCP_MODULE_STATE == STD_ON) || (XCP_ENABLE_XCP_CONTROL_API == STD_ON) )
#include <TSAutosar.h>         /* EB specific standard types */
#include <TSMem.h>             /* EB memory functions */
#include <SchM_Xcp.h>          /* Needed for exclusive area definition */
#include <Xcp_Trace.h>

#include <Xcp_Int.h>           /* Internal API */

#include <Xcp_Cbk.h>           /* Callbacks of Xcp */

/*==================[macros]================================================*/

#if (defined XCP_ACT_IGNORE)
# error XCP_ACT_IGNORE already defined
#endif
/** \brief Action indicates that the received package is ignored */
#define XCP_ACT_IGNORE                       0U

#if (defined XCP_ACT_DATA_STIM)
# error XCP_ACT_DATA_STIM already defined
#endif
/** \brief Action indicates that a STIM data package is received */
#define XCP_ACT_DATA_STIM                    1U

#if (defined XCP_ACT_Q_CMD)
# error XCP_ACT_Q_CMD already defined
#endif
/** \brief Action indicates that the received command package will be queued */
#define XCP_ACT_Q_CMD                        2U

#if (defined XCP_ACT_UNQ_CMD_SYNC)
# error XCP_ACT_UNQ_CMD_SYNC already defined
#endif
/** \brief Action indicates the reception of sync command package. */
#define XCP_ACT_UNQ_CMD_SYNC                 3U

#if (defined XCP_ACT_UNQ_CMD_DISCONNECT)
# error XCP_ACT_UNQ_CMD_DISCONNECT already defined
#endif
/** \brief Action indicates the reception of disconnect command package. */
#define XCP_ACT_UNQ_CMD_DISCONNECT           4U

#if (defined XCP_ACT_UNQ_CMD_ABORTBUSY)
# error XCP_ACT_UNQ_CMD_ABORTBUSY already defined
#endif
/** \brief Action indicates that the received command cannot be queued due to
 **        BUSY state of Xcp command processor. */
#define XCP_ACT_UNQ_CMD_ABORTBUSY            5U

#if (defined XCP_PACKAGE_OK)
# error XCP_PACKAGE_OK already defined
#endif
/** \brief The received package is accepted. */
#define XCP_PACKAGE_OK                       0U

#if (defined XCP_PACKAGE_IGNORE)
# error XCP_PACKAGE_IGNORE already defined
#endif
/** \brief The received package is ignored because is not supported
 **        to be received via selected Pdu. */
#define XCP_PACKAGE_IGNORE                   1U

#if (defined XCP_PACKAGE_ABORT)
# error XCP_PACKAGE_ABORT already defined
#endif
/** \brief The length validation is not successful and the unpacking
 **        has to be aborted. */
#define XCP_PACKAGE_ABORT                    2U

#if ((XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON) ||\
     (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON))

#if (defined XCP_IS_SYNC_DISCONNECT_CMD)
#error XCP_IS_SYNC_DISCONNECT_CMD already defined
#endif
/** \brief Verify whether the command is SYNC or DISCONNECT. */
#define XCP_IS_SYNC_DISCONNECT_CMD(command) \
   (( ((command) == XCP_CMD_SYNCH_PID) || \
      ((command) == XCP_CMD_DISCONNECT_PID) \
     )? TRUE:FALSE)

#if (defined XCP_IS_MASTER_BLOCK_MODE_ACTIVE)
#error XCP_IS_MASTER_BLOCK_MODE_ACTIVE already defined
#endif
/** \brief Verify whether the block mode is active. */
#if((XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON) &&\
    (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON))
/* Deviation MISRAC2012-1 <START> */
#define XCP_IS_MASTER_BLOCK_MODE_ACTIVE() \
  ( ((Xcp_IsDownloadInBlockMode()) || (Xcp_IsProgrammingBlockMode())) )
/* Deviation MISRAC2012-1 <STOP> */
#elif (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON)
#define XCP_IS_MASTER_BLOCK_MODE_ACTIVE()  (Xcp_IsDownloadInBlockMode())
#elif (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON)
#define XCP_IS_MASTER_BLOCK_MODE_ACTIVE()  (Xcp_IsProgrammingBlockMode())
#endif /* ((XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON) &&\
        *  (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON)) */

#if (defined XCP_IS_MASTER_BLOCK_MODE_CMD)
#error XCP_IS_MASTER_BLOCK_MODE_CMD already defined
#endif
/** \brief Verify whether the block mode Next command is received. */
#if ((XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON) &&\
     (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON))
/* Deviation MISRAC2012-1 <START> */
#define XCP_IS_MASTER_BLOCK_MODE_CMD(command) \
  (( (((command) == XCP_CMD_DOWNLOAD_NEXT_PID) && \
      (Xcp_IsDownloadInBlockMode() == TRUE)) || \
     (((command) == XCP_CMD_PROGRAM_NEXT_PID) && \
      (Xcp_IsProgrammingBlockMode() == TRUE))  \
    ) ? TRUE : FALSE )
/* Deviation MISRAC2012-1 <STOP> */
#elif (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON)
#define XCP_IS_MASTER_BLOCK_MODE_CMD(command) \
  (( ((command) == XCP_CMD_DOWNLOAD_NEXT_PID) ) ? TRUE : FALSE)
#elif (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON)
#define XCP_IS_MASTER_BLOCK_MODE_CMD(command) \
  (( ((command) == XCP_CMD_PROGRAM_NEXT_PID) ) ? TRUE : FALSE)
#endif /* ((XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON) &&\
        *  (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON)) */

#endif /* ((XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON) ||\
        *  (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON)) */

#if (defined XCP_GET_INC_RX_PDU_INFO_LENGTH)
#error XCP_GET_INC_RX_PDU_INFO_LENGTH already defined
#endif
/** \brief Get received Pdu length from receive buffer */
#define XCP_GET_INC_RX_PDU_INFO_LENGTH(IntBufId) \
  (Xcp_RxLPduData.LPduLength[(IntBufId)])

#if (defined XCP_SET_INC_RX_PDU_INFO_LENGTH)
#error XCP_SET_INC_RX_PDU_INFO_LENGTH already defined
#endif
/** \brief Set received Pdu length into receive buffer */
#define XCP_SET_INC_RX_PDU_INFO_LENGTH(IntBufId, PduLength) \
  do{                                                       \
      Xcp_RxLPduData.LPduLength[(IntBufId)] = (PduLength);  \
  }while(0)

/*==================[type definitions]======================================*/

/** \brief Definition of the length validation result for the incoming PDU.*/
typedef uint8 Xcp_PackageReturnType;

/** \brief Definition of Xcp received Pdu data type */
typedef struct
{
  /* ------------- Rx Pdu buffer data ------------- */
  /* Length of Rx Pdu */
  PduLengthType LPduLength[XCP_MAX_RX_PDU_PER_CON];
  /* store the Xcp RxPduId of the stored data */
  PduIdType LPduIntId[XCP_MAX_RX_PDU_PER_CON];
  /* Rx LPdu buffer used to contingently store the Rx PDU data, for all available PDUs */
  uint8 LPduBuffer[XCP_RX_BUFFER_SIZE];
#if (XCP_DYNAMIC_PDU_SUPPORTED == STD_ON)
  /* Semaphores: bit flag indicating if PDU is enabled */
  uint8 IsLPduEnabled[XCP_RX_PDU_FLAG_SIZE];
#endif
#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
  uint16 LPduStartPos[XCP_MAX_RX_PDU_PER_CON]; /* runtime info about where to store the
                                              payload of this PDU. Can be changed by FLX_ASSIGN */
  uint8 LPduMaxLenBuf[XCP_MAX_RX_PDU_PER_CON]; /* MAX_FLX_LEN_BUF_x. Can be changed by FLX_ASSIGN */

#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
  /* Semaphores: bit flag indicating if STIM is enabled for the selected PDU */
  uint8 LPDUPacketStatusStim[XCP_RX_PDU_FLAG_SIZE];
#endif
#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON) */
  /* Semaphores: bit flag indicating if the buffer has new data in it.
   * It is used to prevent overwrite of the old data of each Rx Pdu buffer */
  uint8 IsBufferFullFlag[XCP_RX_PDU_FLAG_SIZE];
}Xcp_RxLPduDataType;

/** \brief Definition of Xcp received action type */
typedef uint8 Xcp_RcvActionType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

/** \brief Function used to unpack the the XCP Pdus from the Rx Pdu buffer.
 **
 ** \param[in]      RxBufferPduId  Id of the Rx buffer from where the
 **                 data will be unpacked.
 ** \param[in]      SidId The Function Id from where the function is called
 **/
STATIC FUNC(void, XCP_CODE) Xcp_UnpackRxBufferData
(
  PduIdType RxBufferId,
  uint8 SidId
);


/** \brief Function for processing a received XCP PDU.
 **
 ** Use this function to process the received PDU depending on the type of
 ** information inside the PDU.
 **
 ** \param[in]      SduLength  Length of the received pointer.
 ** \param[in]      SduDataPtr  Address of the received pointer.
 ** \param[inout]   IsFirstCmdUnpackedPtr Indicates if at least one CMD
 **                 was unpacked from the processed Rx Pdu data
 **/
STATIC FUNC(void, XCP_CODE) Xcp_ProcessRxPdu
(
  PduLengthType SduLength,
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) SduDataPtr,
  P2VAR(boolean, AUTOMATIC, XCP_APPL_DATA) IsFirstCmdUnpackedPtr
);

/** \brief Function to get the length of the XCP data segment.
 **
 ** \param[in]  IsFirstCmdUnpacked  flag indicating if a command was already unpacked.
 ** \param[in]  RemainingRxSpace    remaining frame space to unpack inside I-PDU.
 ** \param[in]  RxPduId             Received PDU Id.
 ** \param[in]  SduDataPtr          pointer to I-PDU.
 ** \param[out] DataLengthPtr       length of the XCP data segment.
 ** \param[out] TailLengthPtr       length of the XCP tail.
 ** \param[in ] HeaderLength        length of the XCP header.
 ** \return     If the package length has been successfully retrieved
 ** \retval     E_OK the package length has been successfully retrieved
 ** \retval     E_NOT_OK the package length could not be retrieved
 **
 **/
STATIC FUNC(Xcp_PackageReturnType, XCP_CODE) Xcp_GetPduDataLength
(
  boolean IsFirstCmdUnpacked,
  PduLengthType RemainingRxSpace,
  PduIdType RxPduId,
  P2CONST(uint8, AUTOMATIC, AUTOSAR_COMSTACKDATA) SduDataPtr,
  P2VAR(PduLengthType, AUTOMATIC, AUTOMATIC) DataLengthPtr,
  P2VAR(PduLengthType, AUTOMATIC, AUTOMATIC) TailLengthPtr,
  PduLengthType HeaderLength
);

#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
/** \brief Function to determine if the PDU can accept the incoming
 ** STIM packet
 **
 ** \param[in] RxPduId - the PDU Id
 ** \param[in] XcpRxSduPtr Pointer to the DTO data
 ** \param[in] DataLength Length of the DTO data
 ** \return whether the PDU can accept the STIM package type
 ** \retval TRUE the PDU can accept the STIM package type
 ** \retval FALSE the PDU cannot accept the STIM package type
 */
STATIC FUNC(boolean, XCP_CODE) Xcp_IsRxStimPacketAllowed
(
  PduIdType RxPduId,
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) XcpRxSduPtr,
  PduLengthType DataLength
);
#endif /* (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)*/

/** \brief Function to get the length of the XCP data segment, packed in frame.
 **
 ** \param[in]  FrameRemainingRxSpace     Available unpacking space.
 ** \param[in]  SduDataPtr                pointer to I-PDU.
 ** \param[out] PduDataLengthPtr          length of the PDU, in XCP frame.
 ** \param[in ] HeaderLength              length of the XCP header.
 **
 ** \return the success of the operation or an error code, for the insuccess.
 ** \retval XCP_PACKAGE_OK:     The length of the PDU was successfully returned
 ** \retval XCP_PACKAGE_IGNORE: The length of the PDU is returned, but the package
 **                             should be ignored.
 ** \retval XCP_PACKAGE_ABORT:  The length validation failed due to length size error,
 **                             the unpacking operation, of the XCP frame, is aborted.
 **/
STATIC FUNC(void, XCP_CODE) Xcp_GetPduInFrameSize
(
  PduLengthType FrameRemainingRxSpace,
  P2CONST(uint8, AUTOMATIC, AUTOSAR_COMSTACKDATA) SduDataPtr,
  CONSTP2VAR(PduLengthType, AUTOMATIC, XCP_APPL_DATA) PduDataLengthPtr,
  PduLengthType HeaderLength
);

/** \brief Function to Validate the length of the PDU, against the range margins.
 **
 ** \param[in]    RangeMinVal     Lower limit of the length range.
 ** \param[in]    RangeMaxVal     Upper limit of the length range.
 ** \param[inout] DataLengthPtr   The PDU length which is validated.
 ** \param[out]   TailLengthPtr   The tail of the incoming PDU.
 **
 ** \return the following:
 ** \retval TRUE:   The length of the PDU is valid
 ** \retval FALSE:  The length of the PDU is invalid.
 **/
STATIC FUNC(boolean, XCP_CODE) Xcp_ValidatePduLength
(
  PduLengthType RangeMinVal,
  PduLengthType RangeMaxVal,
  P2VAR(PduLengthType, AUTOMATIC, XCP_APPL_DATA) DataLengthPtr,
  P2VAR(PduLengthType, AUTOMATIC, XCP_APPL_DATA) TailLengthPtr
);


/** \brief Function to extract the action from the received PDU.
 **
 ** The action extracted at this point is processed by:
 ** - Xcp_ProcessRcvCmdAction() for the case when a CTO is received;
 ** - Xcp_ProcessRcvDataAction() for the case when a DTO is received.
 **
 ** \param[in]    SduDataPtr Pointer to the received PDU data.
 ** \param[inout] IsFirstCmdUnpackedPtr Indicates if at least one CMD
 **                 was unpacked from the processed Rx Pdu data
 ** \return      The action which is taken with the received package.
 ** \retval      XCP_ACT_IGNORE              Ignore PDU and don't set any action;
 ** \retval      XCP_ACT_DATA_STIM           Treat the PDU as a data STIM package;
 ** \retval      XCP_ACT_Q_CMD               Queue the command package;
 ** \retval      XCP_ACT_UNQ_CMD_SYNC        Treat the PDU as a SYNC command;
 ** \retval      XCP_ACT_UNQ_CMD_DISCONNECT  Treat the PDU as a DISCONNECT command;
 ** \retval      XCP_ACT_UNQ_CMD_ABORTBUSY   Ignore the package and set the action to
 **                                          abort the busy command;
 ** \retval      XCP_ACT_UNQ_CMD_SYNTAXERR   Ignore the package and set the action to
 **                                          send an ERR_CMD_SYNTAX response packet.
 */
STATIC FUNC(Xcp_RcvActionType, XCP_CODE) Xcp_GetRcvAction
(
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) SduDataPtr,
  P2VAR(boolean, AUTOMATIC, XCP_APPL_DATA) IsFirstCmdUnpackedPtr
);

/** \brief Function to process the received CTO with the action returned by Xcp_GetRcvAction().
 **
 ** \param[in]   SduLength Data length of the received PDU.
 ** \param[in]   SduDataPtr Pointer to the received PDU data.
 ** \param[in]   RcvAction Action which has to be done with the received CTO.
 */
STATIC FUNC(void, XCP_CODE) Xcp_ProcessRcvAction
(
  PduLengthType SduLength,
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) SduDataPtr,
  Xcp_RcvActionType RcvAction
);

/** \brief Function to return whether the command is allowed to be queued or not.
 ** There are two situations when the command is allowed:
 ** 1. If there is an ongoing master block mode, the function checks:
 ** - if the command is related to the ongoing master block mode
 **   (i.e. PROGRAM_NEXT or DOWNLOAD_NEXT)
 ** or
 ** - if the received command is SYNC or DISCONNECT
 ** and returns false if not so.
 ** 2. If multiple XCP messages in one frame are allowed, the function checks if the
 ** unpacked interleave command is the first detected one and returns false if not so.
 **
 ** \param[in] Pid Package Id
 ** \param[in] IsFirstCmdUnpacked Semaphore indicating if in the selected Pdu
 **                               another CMD package was already unpacked.
 ** \return TRUE the command can be queued, FALSE otherwise
 **/
STATIC FUNC(boolean, XCP_CODE)  Xcp_IsCmdAllowed
(
  uint8 Pid,
  boolean IsFirstCmdUnpacked
);

/** \brief Function to verify that the CTO package is expected to
 *         be received via selected Rx PDU channel.
 ** \param[in]   RxPduId    The ID of the I-PDU channel.
 ** \param[in]   SduDataPtr I-PDU data
 ** \param[in]   DataLength Length of the I-PDU data
 ** \return  If the Pdu is received via the expected PDU channel.
 */
STATIC FUNC(boolean, XCP_CODE) Xcp_IsCtoPduAccepted
(
  PduIdType RxPduId,
  P2CONST(uint8, AUTOMATIC, AUTOSAR_COMSTACKDATA) SduDataPtr,
  PduLengthType DataLength
);

/** \brief Function to initially validate the minimum length of command which
 ** are accessed afterwards (e.g. to access the byte that holds the information regarding
 ** the necessary full command length)
 ** \param[in]   SduDataPtr I-PDU data
 ** \param[in]   DataLength Length of the I-PDU data
 ** \return  If the Length is OK or not.
 */
STATIC FUNC(boolean, XCP_CODE) Xcp_InitialMinimumCmdLengthIsValid
(
  P2CONST(uint8, AUTOMATIC, AUTOSAR_COMSTACKDATA) SduDataPtr,
  PduLengthType DataLength
);

/** \brief Function to get the PDU start position inside the Rx buffer.
 ** \param[in]   IntPduId  The internal PDU ID.
 ** \return  A pointer to the buffer where this PDU can write received data
 */
STATIC FUNC_P2VAR(uint8, XCP_VAR_CLEARED, XCP_CODE) XcpGetIncRxPduInfoPtr
(
  PduIdType IntPduId
);



#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#if (XCP_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON)
#define XCP_START_SEC_VAR_8
#include <Xcp_MemMap.h>
/** \brief Flag to signal that a SoAd UDP socket configuration is needed */
STATIC VAR(boolean, XCP_VAR) Xcp_ConfigureSocket = FALSE;
#define XCP_STOP_SEC_VAR_8
#include <Xcp_MemMap.h>
#endif /* (XCP_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON) */

#define XCP_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <Xcp_MemMap.h>

STATIC VAR(Xcp_RxLPduDataType, XCP_VAR_CLEARED) Xcp_RxLPduData;

#define XCP_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <Xcp_MemMap.h>

/*==================[external function definitions]=========================*/

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

/*-----------------------------[Xcp_InitReceiveProcessor]------------------*/

FUNC(void, XCP_CODE) Xcp_InitReceiveProcessor(void)
{
  PduIdType Iterator;

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
  uint16 startAddress  = 0;
#endif

  DBG_XCP_INITRECEIVEPROCESSOR_ENTRY();

  for(Iterator = 0U; Iterator < XCP_MAX_RX_PDU_PER_CON; Iterator++)
  {
    /* --- clear Rx pdu buffer data --- */
    XCP_CLEAR_BYTE_FLAG(Iterator, Xcp_RxLPduData.IsBufferFullFlag);
    Xcp_RxLPduData.LPduLength[Iterator] = 0U;
  }

#if(XCP_NO_OF_CONNECTIONS == 1U)
  /* Initialize runtime flags from configuration: if the PDU is dynamic and which packet is supported.
  In case multiple configurations are configured, this initialization occurs at Rx Indication time, depending
  on the needed configuration  */
  {
    Xcp_InitRxPduFlags(0U);
  }
#endif

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
  /* initialize max buffer length (MAX_FLX_LEN_BUF_x_init) */
  for (Iterator=0; Iterator<XCP_NO_OF_FLX_BUF_CONFIGURED; Iterator++)
  {
    if(XCP_IS_FLAG_SET(XCP_BUFFER_DIRECTION_TX_MASK, Xcp_FlxBufConfig[Iterator].Flags,uint8) == FALSE)
    {
      Xcp_RxLPduData.LPduStartPos[Xcp_RxLPduIdConf[Xcp_FlxBufConfig[Iterator].FlxPduIdConfIdx].IntPduBufId] = startAddress;

      Xcp_RxLPduData.LPduMaxLenBuf[Xcp_RxLPduIdConf[Xcp_FlxBufConfig[Iterator].FlxPduIdConfIdx].IntPduBufId] =
         Xcp_FlxBufConfig[Iterator].FlxBufLengthInitValue;
      startAddress += Xcp_FlxBufConfig[Iterator].FlxBufLengthInitValue;

    }
  }
#endif /*(XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)*/

  DBG_XCP_INITRECEIVEPROCESSOR_EXIT();

}

/*-----------------------------[Xcp_ReceiveRxData]------------------*/

FUNC(void, XCP_CODE) Xcp_ReceiveRxData
(
  PduIdType XcpRxPduId,
  P2CONST(PduInfoType, AUTOMATIC, XCP_APPL_DATA) XcpRxPduPtr
)
{
#if (XCP_PROD_ERR_HANDLING_PDU_BUFFER_FULL !=TS_PROD_ERR_DISABLE)
  boolean reportDET = FALSE;
#endif
#if (XCP_DYNAMIC_PDU_SUPPORTED == STD_ON)
  boolean PduLocalSts = FALSE;
#endif
  CONSTP2CONST(Xcp_RxLPduIdConfType, AUTOMATIC, XCP_APPL_CONST) RxLPduIdConfPtr =
      &Xcp_RxLPduIdConf[XcpRxPduId];
  /* get the ID of the Rx LPdu buffer */
  const PduIdType IntPduId = RxLPduIdConfPtr->IntPduBufId;

  DBG_XCP_RECEIVERXDATA_ENTRY(XcpRxPduId, XcpRxPduPtr);

#if (XCP_BSW_DISTRIBUTION_ENABLED == STD_ON)
  if (Xcp_ConnectionToPartitionMap[RxLPduIdConfPtr->IntConnectionId] != (uint8)GetApplicationID())
  {
    /* reject package */
    return;
  }
  else
  {
    Atomics_Store(&Xcp_ActiveConnectionId, RxLPduIdConfPtr->IntConnectionId);
  }
#endif

#if(XCP_NO_OF_CONNECTIONS > 1U)
  /* If the connection was changed and there was no TP command that changed the PDUs configuration in the meantime,
   * re-initialize the PDU flags with pre-configured values */
  if (Xcp_ConnectionStatus.ConnectionId != RxLPduIdConfPtr->IntConnectionId)
  {
    /* Enter critical section */
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

    /* !LINKSTO Xcp.Communication.MultipleConnection.PDUInfo.ReInitialization,1 */
    if (XCP_IS_FLAG_SET(XCP_TP_CONFIGURATION_DONE_FLAG, Xcp_ConnectionStatus.Flags, uint8) == FALSE)
    {
      /* The connection is changed, we need to re-initialize flags from the
           configuration */
      Xcp_InitRxPduFlags(RxLPduIdConfPtr->IntConnectionId);
      Xcp_InitTxPduFlags(RxLPduIdConfPtr->IntConnectionId);
    }
    /* Exit critical section */
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
  }
  /* apply the I'th level lock of the connection resources */
  Xcp_ConnectionStatus.ConnectionId = RxLPduIdConfPtr->IntConnectionId;
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */

#if (XCP_DYNAMIC_PDU_SUPPORTED == STD_ON)
  /* if PDU is dynamic and enabled or PDU is not dynamic */
  if ((TRUE == XCP_IS_FLAG_SET(XCP_DYNAMIC_TX_PDU,
             Xcp_RxLPduIdConf[XcpRxPduId].Properties,uint8)) &&
      (XCP_IS_BYTE_FLAG_SET(IntPduId, Xcp_RxLPduData.IsLPduEnabled) == TRUE))
  {
    PduLocalSts = TRUE;
  }
  else if (FALSE == XCP_IS_FLAG_SET(XCP_DYNAMIC_TX_PDU,
                  Xcp_RxLPduIdConf[XcpRxPduId].Properties,uint8))
  {
    PduLocalSts = TRUE;
  }
  else
  {
    /* nothing to do */
  }
  if (PduLocalSts == TRUE)
#endif /* (XCP_DYNAMIC_PDU_SUPPORTED == STD_ON) */
  {
    /* Enter critical section */
    SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
    /* if the buffer is not full set it to full and lock the access */
    /* !LINKSTO Xcp.BswDistribution.Communication.Rx.Buffers.CoreLocal,1 */
    if( XCP_IS_BYTE_FLAG_SET(IntPduId, Xcp_RxLPduData.IsBufferFullFlag) == FALSE)
    {
      P2VAR(uint8, AUTOMATIC, XCP_VAR_CLEARED) RxBufferPtr;
      /* mark the buffer as full */
      XCP_SET_BYTE_FLAG(IntPduId, Xcp_RxLPduData.IsBufferFullFlag);
      /* get PDU start position inside the LPdu buffer */
      RxBufferPtr = XcpGetIncRxPduInfoPtr(XcpRxPduId);
      /* copy received data, of the incoming PDU, into assigned Rx buffer */
      TS_MemCpy(RxBufferPtr, XcpRxPduPtr->SduDataPtr, XcpRxPduPtr->SduLength);
      /* store length of the incoming PDU */
      XCP_SET_INC_RX_PDU_INFO_LENGTH(IntPduId, XcpRxPduPtr->SduLength);
      /* store the mapped Rx Pdu Id */
      Xcp_RxLPduData.LPduIntId[IntPduId] = XcpRxPduId;
    }
#if (XCP_PROD_ERR_HANDLING_PDU_BUFFER_FULL !=TS_PROD_ERR_DISABLE)
    else
    {
      reportDET = TRUE;
    }
#endif
    /* Exit critical section */
    SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

#if (XCP_RECEIVE_FROM_INDICATION == STD_ON)
    /* check if receive from Rx indication is enabled for this PDU */
    if (TRUE == XCP_IS_FLAG_SET(XCP_SUPPORT_RX_FROM_INDICATION,
                RxLPduIdConfPtr->Properties,
                uint8))
    {
      /* unpack data from the selected Rx buffer */
      Xcp_UnpackRxBufferData(IntPduId, XCP_SID_IF_RX_INDICATION);
    }
#endif
  }
#if (XCP_PROD_ERR_HANDLING_PDU_BUFFER_FULL !=TS_PROD_ERR_DISABLE)
  if(reportDET == TRUE)
  {
    /* A message was lost because the corresponding LPDU buffer is full.
     * Therefore, a DET notification is issued to signal this situation. */
    XCP_REPORT_PDU_BUFFER_FULL(XCP_SID_IF_RX_INDICATION);
  }
#endif
  DBG_XCP_RECEIVERXDATA_EXIT();
}

/*-----------------------------[Xcp_ProcessReception]------------------*/

FUNC(void, XCP_CODE) Xcp_ProcessReception
(
  uint8 SidId
)
{
  PduIdType RxPduId;
  P2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_APPL_CONST) ConnectionCfgPtr;

  DBG_XCP_PROCESSRECEPTION_ENTRY();
#if (XCP_COM_MAINFUNCTION_SUPPORT == STD_ON)
  if (SidId != XCP_SID_MAIN_FUNCTION)
#endif
  {
#if (XCP_NO_OF_CONNECTIONS > 1U)
    if(Xcp_ConnectionStatus.ConnectionId != XCP_INVALID_CONNECTION_ID)
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
    {
      /* select connection configuration */
      /* !LINKSTO Xcp.BswDistribution.Communication.ConnectionData.CoreLocal,1 */
      ConnectionCfgPtr = &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];
      /* search through all Rx buffers to see if there is any data available. */
      for(RxPduId = 0U; RxPduId < ConnectionCfgPtr->RxPduMax; RxPduId++)
      {
#if (XCP_DYNAMIC_PDU_SUPPORTED == STD_ON)
        boolean PduLocalSts = FALSE;
        /* if PDU is dynamic and enabled or PDU is not dynamic */
        if ((TRUE == XCP_IS_FLAG_SET(XCP_DYNAMIC_RX_PDU,
                   Xcp_RxLPduIdConf[Xcp_RxLPduData.LPduIntId[RxPduId]].Properties,uint8)) &&
             (XCP_IS_BYTE_FLAG_SET(RxPduId, Xcp_RxLPduData.IsLPduEnabled) == TRUE))
        {
          PduLocalSts = TRUE;
        }
        else if (FALSE == XCP_IS_FLAG_SET(XCP_DYNAMIC_RX_PDU,
                   Xcp_RxLPduIdConf[Xcp_RxLPduData.LPduIntId[RxPduId]].Properties,uint8))
        {
          PduLocalSts = TRUE;
        }
        else
        {
          /* nothing to do */
        }
        if (PduLocalSts == TRUE)
#endif /* (XCP_DYNAMIC_PDU_SUPPORTED == STD_ON) */
        {
          /* unpack data from the selected Rx buffer */
          Xcp_UnpackRxBufferData(RxPduId, SidId);
        }
      }
    }
  }

  DBG_XCP_PROCESSRECEPTION_EXIT();
}


#if (XCP_DYNAMIC_PDU_SUPPORTED == STD_ON)
/*-----------------------------[Xcp_EnableRxCommunicationChannel]--------------------------*/
FUNC(uint8, XCP_CODE) Xcp_EnableRxCommunicationChannel
(
  P2CONST( uint8, AUTOMATIC, XCP_APPL_DATA ) ParamPtr
)
{
  uint8 RetVal = E_NOT_OK;
  PduIdType RxPduId = ParamPtr[XCP_DYNAMIC_PDU_CMD_PDUID];
  /* pointer used to select the locked connection resource */
  P2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_APPL_CONST) ConnectionCfgPtr;

  DBG_XCP_ENABLE_RX_COMMUNICATION_CHANNEL_ENTRY(ParamPtr);
#if (XCP_NO_OF_CONNECTIONS > 1U)
  /* CHECK: NOPARSE */
  if(Xcp_ConnectionStatus.ConnectionId != XCP_INVALID_CONNECTION_ID)
  /* CHECK: PARSE */
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
  {
    /* select the connection configuration */
    ConnectionCfgPtr = &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];
    /* Check if requested PduId is valid */
    if (RxPduId >  ConnectionCfgPtr->RxPduMax)
    {
      /* return E_NOT_OK */
      RetVal = E_NOT_OK;
    }
    else if (ParamPtr[XCP_DYNAMIC_PDU_CMD_STATE] == XCP_DYNAMIC_PDU_CMD_STATE_ENABLED)
    {
      /* enable the requested PDU */
      XCP_SET_BYTE_FLAG(Xcp_RxLPduIdConf[RxPduId].IntPduBufId, Xcp_RxLPduData.IsLPduEnabled);
      RetVal = E_OK;
    }
    else
    {
      /* disable the requested PDU */
      XCP_CLEAR_BYTE_FLAG(Xcp_RxLPduIdConf[RxPduId].IntPduBufId, Xcp_RxLPduData.IsLPduEnabled);
      RetVal = E_OK;
    }
  }
#if(XCP_NO_OF_CONNECTIONS > 1U)
  /* CHECK: NOPARSE */
  else
  {
    /* In case of an invalid Xcp_ConnectionStatus.ConnectionId (XCP_INVALID_CONNECTION_ID) the command will not be inserted
    into the command queue because the described condition is verified before in Xcp_ProcessReception() in case the
    reception of the command is made from Xcp_MainFunction or in case the XcpPduSupportRxFromRxIndication
    is enabled the Xcp_ConnectionStatus.ConnectionId is updated to the RxLPdus internal ConnectionId. */
    XCP_UNREACHABLE_CODE_ASSERT(XCP_SID_MAIN_FUNCTION);
  }
  /* CHECK: PARSE */
#endif

  DBG_XCP_ENABLE_RX_COMMUNICATION_CHANNEL_EXIT(RetVal, ParamPtr);
  return RetVal;
}
#endif /* (XCP_DYNAMIC_PDU_SUPPORTED == STD_ON) */

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
FUNC(void, XCP_CODE) Xcp_SetPacketEnableStatusStim
(
  PduIdType FlagID,
  boolean ShouldBeEnabled
)
{
  /* Function must be called within Xcp critical section */

  if (ShouldBeEnabled == TRUE)
  {
    /* enable STIM packet type */
    XCP_SET_BYTE_FLAG(FlagID, Xcp_RxLPduData.LPDUPacketStatusStim);
  }
  else
  {
    /* disable STIM packet type */
    XCP_CLEAR_BYTE_FLAG(FlagID, Xcp_RxLPduData.LPDUPacketStatusStim);
  }
}
#endif /* XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK*/


FUNC(void, XCP_CODE) Xcp_SetRxFlxBufferLength(uint8 bufferId, uint8 newLength)
{
  /* Function must be called within Xcp critical section */
  uint8 Iterator;
  uint16 startAddress  = 0U;

  /* Reset all Rx buffers so that unexpected data is not being sent during reconfiguration */
  TS_MemSet(Xcp_RxLPduData.IsBufferFullFlag, 0U, XCP_RX_PDU_FLAG_SIZE);

  /* set new length */
  Xcp_RxLPduData.LPduMaxLenBuf[Xcp_RxLPduIdConf[Xcp_FlxBufConfig[bufferId].FlxPduIdConfIdx].IntPduBufId] = newLength;

  /* adjust all buffers starting points with the same direction based on the new length */
  for (Iterator=0; Iterator<XCP_NO_OF_FLX_BUF_CONFIGURED; Iterator++)
  {
    if(XCP_IS_FLAG_SET(XCP_BUFFER_DIRECTION_TX_MASK, Xcp_FlxBufConfig[Iterator].Flags,uint8) == FALSE)
    {
      /* set new start address in the continuous memory area that contains all buffers */
      Xcp_RxLPduData.LPduStartPos[Xcp_RxLPduIdConf[Xcp_FlxBufConfig[Iterator].FlxPduIdConfIdx].IntPduBufId] = startAddress;
      /* increment the start address */
      startAddress += Xcp_RxLPduData.LPduMaxLenBuf[Xcp_RxLPduIdConf[Xcp_FlxBufConfig[Iterator].FlxPduIdConfIdx].IntPduBufId];
    }
  }
}

FUNC(uint8, XCP_CODE) Xcp_GetRxFlxBufferLength(uint8 bufferId)
{
   return Xcp_RxLPduData.LPduMaxLenBuf[Xcp_RxLPduIdConf[Xcp_FlxBufConfig[bufferId].FlxPduIdConfIdx].IntPduBufId];
}

#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON) */

FUNC(PduLengthType, XCP_CODE) Xcp_GetPduLength(PduIdType pduId)
{
  /* Get the value from configuration */
  PduLengthType retValue = Xcp_ConnectionCfg[Xcp_RxLPduIdConf[pduId].IntConnectionId].PduLengthMax;

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
  if (Xcp_ConnectionCfg[Xcp_RxLPduIdConf[pduId].IntConnectionId].ConnectionType == XCP_CONNECTION_OVER_FLEXRAY)
  {
     /* Get the value at runtime */
     retValue = Xcp_RxLPduData.LPduMaxLenBuf[Xcp_RxLPduIdConf[pduId].IntPduBufId];
  }
#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)*/
  return retValue;
}

/*-----------------------------[Xcp_InitRxPduFlags]------------------*/
FUNC(void, XCP_CODE) Xcp_InitRxPduFlags
(
  uint8 ConnectionId
)
{
  PduIdType Iterator;

  for(Iterator = 0U; Iterator < XCP_NO_OF_RX_PDU_IDS; Iterator++)
  {
    if (Xcp_RxLPduIdConf[Iterator].IntConnectionId == ConnectionId)
    {
#if (XCP_DYNAMIC_PDU_SUPPORTED == STD_ON)
        /* If PDU is dynamic and default state is enabled update runtime properties of the pdu */
      if ((TRUE == XCP_IS_FLAG_SET
                    (XCP_DYNAMIC_RX_PDU, Xcp_RxLPduIdConf[Iterator].Properties,uint8)) &&
          (TRUE == XCP_IS_FLAG_SET
                    (XCP_ENABLE_STATE_DYNAMIC_RX_PDU, Xcp_RxLPduIdConf[Iterator].Properties,uint8)))
      {
        XCP_SET_BYTE_FLAG(Xcp_RxLPduIdConf[Iterator].IntPduBufId, Xcp_RxLPduData.IsLPduEnabled);
      }
#endif /* (XCP_DYNAMIC_PDU_SUPPORTED = STD_ON) */

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))

#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
      if (Xcp_ConnectionCfg[ConnectionId].ConnectionType == XCP_CONNECTION_OVER_FLEXRAY)
      {
        if (XCP_IS_FLAG_SET(XCP_SUPPORT_FOR_STIM_DTO_PDU, Xcp_RxLPduIdConf[Iterator].Properties, uint8) == TRUE)
        {
          /* Enable STIM packet for this PDU */
          XCP_SET_BYTE_FLAG(Xcp_RxLPduIdConf[Iterator].IntPduBufId, Xcp_RxLPduData.LPDUPacketStatusStim);
        }
        else
        {
          /* Disable STIM packet for this PDU */
          XCP_CLEAR_BYTE_FLAG(Xcp_RxLPduIdConf[Iterator].IntPduBufId, Xcp_RxLPduData.LPDUPacketStatusStim);
        }
      }
#endif
#endif /*(XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)*/
    }
  }
}

#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>

/*==================[internal function definitions]=========================*/

#define XCP_START_SEC_CODE
#include <Xcp_MemMap.h>

/*-----------------------------[Xcp_GetPduInFrameSize]------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_GetPduInFrameSize
(
  PduLengthType FrameRemainingRxSpace,
  P2CONST(uint8, AUTOMATIC, AUTOSAR_COMSTACKDATA) SduDataPtr,
  CONSTP2VAR(PduLengthType, AUTOMATIC, XCP_APPL_DATA) PduDataLengthPtr,
  PduLengthType HeaderLength
)
{
#if (XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
#if (XCP_NO_OF_CONNECTIONS > 1U)
  P2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_APPL_CONST) ConnectionCfgPtr =
      &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];
#endif
#if (XCP_ON_FLEXRAY_ENABLED == STD_OFF)
  TS_PARAM_UNUSED(HeaderLength);
#endif
#endif/* (XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) */

#if ((XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_OFF) || (XCP_ON_FLEXRAY_ENABLED == STD_OFF))
  TS_PARAM_UNUSED(HeaderLength);
#endif

  DBG_XCP_GETPDUINFRAMESIZE_ENTRY(FrameRemainingRxSpace, SduDataPtr);

  /* initialize data length return value with default value */
  *PduDataLengthPtr = FrameRemainingRxSpace;

#if (XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
#if (XCP_NO_OF_CONNECTIONS > 1U)
  if(XCP_IS_FLAG_SET(XCP_MULTI_PDUS_IN_FRAME_FLAG,
                     ConnectionCfgPtr->Properties,
                     uint8) == TRUE)
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
  {
#if (XCP_ON_FLEXRAY_ENABLED == STD_ON)
#if (XCP_NO_OF_CONNECTIONS > 1U)
    if(ConnectionCfgPtr->ConnectionType == XCP_CONNECTION_OVER_FLEXRAY)
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
    {
      /* return calculated value for FlexRay bus */
      *PduDataLengthPtr = (PduLengthType)(SduDataPtr[HeaderLength - 1U]);
    }
#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) */

#if (XCP_ON_ETHERNET_ENABLED == STD_ON)
#if (XCP_NO_OF_CONNECTIONS > 1U)
    if((ConnectionCfgPtr->ConnectionType == XCP_CONNECTION_OVER_TCPIP) ||
       (ConnectionCfgPtr->ConnectionType == XCP_CONNECTION_OVER_UDPIP))
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
    {
      /* Return calculated value of the request length in endianness order as received on Ethernet bus.*/
      *PduDataLengthPtr = (PduLengthType)(((PduLengthType)SduDataPtr[1U]) << 8U);
      *PduDataLengthPtr = *PduDataLengthPtr | (PduLengthType)(SduDataPtr[0U]);
    }
#endif /* (XCP_ON_ETHERNET_ENABLED == STD_ON) */
  }
#else
  TS_PARAM_UNUSED(SduDataPtr);
#endif /* (XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) */

  DBG_XCP_GETPDUINFRAMESIZE_EXIT(PduDataLengthPtr);
}

/*-----------------------------[Xcp_ValidatePduLength]------------------*/

STATIC FUNC(boolean, XCP_CODE) Xcp_ValidatePduLength
(
  PduLengthType RangeMinVal,
  PduLengthType RangeMaxVal,
  P2VAR(PduLengthType, AUTOMATIC, XCP_APPL_DATA) DataLengthPtr,
  P2VAR(PduLengthType, AUTOMATIC, XCP_APPL_DATA) TailLengthPtr
)
{
#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON))
  /* get the number of bytes  of alignment (the same as header length )*/
  const PduLengthType NumberOfBytesAllignment = Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].FlexRayPackageAlignment;
#endif /* ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)) */

  const PduLengthType DataLength = *DataLengthPtr;
  boolean RetVal = FALSE;


  DBG_XCP_VALIDATEPDULENGTH_ENTRY(RangeMinVal, RangeMaxVal, DataLengthPtr);

#if (XCP_ON_FLEXRAY_ENABLED == STD_ON)
#if (XCP_NO_OF_CONNECTIONS > 1U)
  if(Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].ConnectionType == XCP_CONNECTION_OVER_FLEXRAY)
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
#if (XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
  {
    if(XCP_IS_FLAG_SET(XCP_MULTI_PDUS_IN_FRAME_FLAG,
                       Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].Properties,
                       uint8) == TRUE)
    {
      /* remove tail from the package, if it was added for alignment purposes*/
      PduLengthType MessageUnalignedBytes = DataLength % NumberOfBytesAllignment;
      if (MessageUnalignedBytes > 0U)
      {
        *TailLengthPtr = NumberOfBytesAllignment - MessageUnalignedBytes;
        /* Maximum range increased as the Message also has a tail byte(s) */
        RangeMaxVal += MessageUnalignedBytes;
      }
    }
  }
#else
  {
    TS_PARAM_UNUSED(TailLengthPtr);
    /* check if message is odd or if maximum is odd (MAX_CTO or MAX_CTO_PGM)*/
    if (((DataLength % 2U) != 0U) || ((RangeMaxVal % 2U) != 0U) )
    {
      /* check if the message exceeds the MAX_CTO or MAX_CTO_PGM by 1, meaning that a tail byte was added */
      if (DataLength == (RangeMaxVal + 1U))
      {
        /* remove the tail from the message */
        *DataLengthPtr-=1U;
        /* Maximum range increased as the Message also has a tail byte */
        RangeMaxVal += 1U;
      }
    }
  }
#endif /* XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON */
#else
  TS_PARAM_UNUSED(TailLengthPtr);
#endif /*(XCP_ON_FLEXRAY_ENABLED == STD_ON)*/

  if((DataLength >= RangeMinVal) &&  (DataLength <= RangeMaxVal))
  {
    /* the length is valid */
    RetVal = TRUE;
  }

  DBG_XCP_VALIDATEPDULENGTH_EXIT(RetVal, DataLengthPtr, TailLengthPtr);

  return RetVal;
}

/*-----------------------------[Xcp_GetPduDataLength]------------------*/

STATIC FUNC(Xcp_PackageReturnType, XCP_CODE) Xcp_GetPduDataLength
(
  boolean IsFirstCmdUnpacked,
  PduLengthType RemainingRxSpace,
  PduIdType RxPduId,
  P2CONST(uint8, AUTOMATIC, AUTOSAR_COMSTACKDATA) SduDataPtr,
  P2VAR(PduLengthType, AUTOMATIC, AUTOMATIC) DataLengthPtr,
  P2VAR(PduLengthType, AUTOMATIC, AUTOMATIC) TailLengthPtr,
  PduLengthType HeaderLength
)
{
  /* Function must be called within Xcp critical section */

  /* initialize return value with the worst case scenario */
  Xcp_PackageReturnType RetVal;
  /* initialize with a value which doesn't represent a CMD package */
  uint8 Pid = 0U;
  /* local variable used to retrieve the package data length */
  PduLengthType DataLength = 0U;
  /* Local variable to hold the expected data length */
  PduLengthType PduMinLength = 0U;
  PduLengthType PduMaxLength = 0U;

  DBG_XCP_GETPDUDATALENGTH_ENTRY(RxPduId, RemainingRxSpace, SduDataPtr);

  if(RemainingRxSpace <= HeaderLength)
  {
    /* the unpacking operation will be aborted */
    RetVal = XCP_PACKAGE_ABORT;
  }
  else
  {
    /* update remaining space. strip-out the header */
    RemainingRxSpace -= HeaderLength;
    /* retrieve package data length.
     * If tail is detected by the Xcp_GetCommandLength then it
     * will be updated by the Xcp_GetCommandLength function */
    Xcp_GetPduInFrameSize(RemainingRxSpace, SduDataPtr, &DataLength, HeaderLength);
    /* select the address where data segment begins */
    SduDataPtr = &SduDataPtr[HeaderLength];
    /* get package id */
    Pid = SduDataPtr[0U];
#if(XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
    /* check that the given package data is not out of range */
    if (DataLength == 0U)
    {
      /* in case the first PDU has length 0, send an ERR_CMD_SYNTAX response packet. */
      if((XCP_PID_IS_CMD(Pid)) && (IsFirstCmdUnpacked == FALSE))
      {
        /* The data packet is not valid, send an ERR_CMD_SYNTAX response packet. */
        Xcp_SendCmdErrorPacket(Pid, XCP_ERR_CMD_SYNTAX);
      }
      /* the unpacking operation will be aborted */
      RetVal = XCP_PACKAGE_ABORT;
    }
    else if(DataLength > RemainingRxSpace)
    {
      /* Invalid Pdu length has been detected: the data length is higher than the
       * remaining space in the received Xcp data payload. */
      if(XCP_PID_IS_CMD(Pid))
      {
        /* The data packet is not valid, send an ERR_CMD_SYNTAX response packet. */
        Xcp_SendCmdErrorPacket(Pid, XCP_ERR_CMD_SYNTAX);
      }
      /* the unpacking operation will be aborted */
      RetVal = XCP_PACKAGE_ABORT;
    }
    else
#else
    TS_PARAM_UNUSED(IsFirstCmdUnpacked);
#endif
    {
      /* get the expected package length */
      if (XCP_PID_IS_CMD(Pid))
      {
        /* check if the CMD packet is received via expected PDU channel */
        if(Xcp_IsCtoPduAccepted(RxPduId, SduDataPtr, DataLength) == FALSE)
        {
          /* Skip the package processing */
          RetVal = XCP_PACKAGE_IGNORE;
        }
        else
        {
          if(Xcp_IsCmdImplemented(Pid) == TRUE)
          {
            /* get the data length range for the command package */
            Xcp_GetCmdLength(SduDataPtr, &PduMinLength, &PduMaxLength);
            /* check if the data length is in range */
            if(Xcp_ValidatePduLength(PduMinLength, PduMaxLength,
                                     &DataLength, TailLengthPtr) == TRUE)
            {
              /* the received package can be processed */
              RetVal = XCP_PACKAGE_OK;
            }
            else
            {
              /* The data packet is not valid, send an ERR_CMD_SYNTAX response packet. */
              Xcp_SendCmdErrorPacket(Pid, XCP_ERR_CMD_SYNTAX);
              /* the length is invalid, so unpack operation will be aborted. */
              RetVal = XCP_PACKAGE_ABORT;
            }
          }
          else
          {
            /* The command is not implemented, send an ERR_CMD_UNKNOWN response packet. */
            Xcp_SendCmdErrorPacket(Pid, XCP_ERR_CMD_UNKNOWN);
            /* the command is not implemented, so unpack operation will be aborted. */
            RetVal = XCP_PACKAGE_ABORT;
          }
        }
      }
      else
      {
#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
        if (Xcp_IsRxStimPacketAllowed(RxPduId, SduDataPtr, DataLength) == TRUE)
        {
          /* get the data length range for the stim package */
          Xcp_GetStimLength(SduDataPtr, &PduMinLength, &PduMaxLength);
          /* check if the data length is in range */
          if(Xcp_ValidatePduLength(PduMinLength, PduMaxLength,
                                   &DataLength, TailLengthPtr) == TRUE)
          {
            /* the received package can be processed */
            RetVal = XCP_PACKAGE_OK;
          }
          else
          {
            /* Skip the package processing */
            RetVal = XCP_PACKAGE_IGNORE;
          }
        }
        else
        {
          /* Skip the package processing */
          RetVal = XCP_PACKAGE_IGNORE;
        }
#else
        /* Skip the package processing */
        RetVal = XCP_PACKAGE_IGNORE;
#endif
      }
    }
  }

  switch(RetVal)
  {
  case XCP_PACKAGE_OK:
  case XCP_PACKAGE_IGNORE:
    /* return package data length */
    *DataLengthPtr = DataLength;
    break;
  case XCP_PACKAGE_ABORT:
    /* the same action shall be taken as for unknown behavior
     * which represents the worst case scenario. */
  default:
#if (XCP_PGM_SUPPORTED == STD_ON)
    /* abort PGM session, if active */
    Xcp_SetAbortProgrammingSession();
#else
    /* nothing to do */
#endif
    break;
  }

  DBG_XCP_GETPDUDATALENGTH_EXIT(RetVal, DataLengthPtr, TailLengthPtr);

  return RetVal;
}

#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
/*-----------------------------[Xcp_IsRxStimPacketAllowed]------------------*/
STATIC FUNC(boolean, XCP_CODE) Xcp_IsRxStimPacketAllowed
(
  PduIdType RxPduId,
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) XcpRxSduPtr,
  PduLengthType DataLength
)
{
  /* Function must be called within Xcp critical section */

  boolean retValue = FALSE;
  uint8 minimumStimLength = XCP_PID_LENGTH;

  /* Check the minimum required length beforehand */
  if (DataLength >= minimumStimLength)
  {
#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
    if (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].ConnectionType ==
        XCP_CONNECTION_OVER_FLEXRAY)
    {
      /* Check if the packet STIM is allowed (read from runtime bitfield) */
      if(XCP_IS_BYTE_FLAG_SET(Xcp_RxLPduIdConf[RxPduId].IntPduBufId,
                              Xcp_RxLPduData.LPDUPacketStatusStim) == TRUE)
      {
        uint8 OdtId;
        Xcp_DaqIdType DaqID;
        /* Get the DAQ Id and the ODT from the DTO packet */
        Xcp_GetDaqIDFromDto(XcpRxSduPtr, &DaqID, &OdtId);
        retValue = Xcp_IsFlxDaqIdAllowedForTheBuffer(RxPduId, XCP_SUPPORT_FOR_STIM_DTO_PDU, DaqID);
      }
    }
    else
#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)*/
    {
      TS_PARAM_UNUSED(XcpRxSduPtr);
      /* Check if the packet STIM is allowed (read from configuration) */
      retValue = (XCP_IS_FLAG_SET(XCP_SUPPORT_FOR_STIM_DTO_PDU,
                                  Xcp_RxLPduIdConf[RxPduId].Properties, uint8) == TRUE);
    }
  }
  return retValue;
}
#endif /* (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK) */
/*-----------------------------[Xcp_UnpackRxBufferData]------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_UnpackRxBufferData
(
  PduIdType RxBufferId,
  uint8 SidId
)
{
  P2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_APPL_CONST) ConnectionCfgPtr =
      &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];
  const PduIdType RxPduId = Xcp_RxLPduData.LPduIntId[RxBufferId];
  PduLengthType HeaderLength = ConnectionCfgPtr->PduHeaderLength;
  /* select the Rx buffer */
  P2VAR(uint8, AUTOMATIC, XCP_VAR_CLEARED) RxBufferPtr;
#if (XCP_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON)
  SoAd_SoConIdType DstSoConId;
  boolean LocalBufferFullSts = FALSE;
#endif /* (XCP_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON) */
  /* Xcp data length and tail */
  PduLengthType DataLength = 0U;
  PduLengthType TailLength = 0U;
  /* no command was unpacked, yet */
  boolean IsFirstCmdUnpacked = FALSE;
#if(XCP_PROD_ERR_HANDLING_PDU_LOST != TS_PROD_ERR_DISABLE)
  boolean ReportPduLostErr = FALSE;
#endif

  /* get the usable Pdu length*/
  PduLengthType RemainingRxSpace;
  Xcp_PackageReturnType GetLengthResult;
  /* get the Pdu pointer */
  RxBufferPtr = XcpGetIncRxPduInfoPtr(RxPduId);

  DBG_XCP_UNPACKRXBUFFERDATA_ENTRY(RxBufferId);

  /* Enter critical section */
  SchM_Enter_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();

  RemainingRxSpace = XCP_GET_INC_RX_PDU_INFO_LENGTH(RxBufferId);

  if( XCP_IS_BYTE_FLAG_SET(RxBufferId, Xcp_RxLPduData.IsBufferFullFlag) == TRUE)
  {

#if (XCP_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON)
    LocalBufferFullSts = TRUE;
#endif

    /* unpack PDUs */
    /* !LINKSTO Xcp.BswDistribution.Communication.Rx.CoreLocal,1 */
    /* !LINKSTO Xcp.BswDistribution.Communication.PduData.CoreLocal,1 */
    do
    {
      /* Reset tail length */
      TailLength = 0U;
      /* get data segment length */
      GetLengthResult = Xcp_GetPduDataLength(IsFirstCmdUnpacked,
                                             RemainingRxSpace,
                                             RxPduId, RxBufferPtr,
                                             &DataLength, &TailLength,
                                             HeaderLength);

      if (XCP_PACKAGE_ABORT != GetLengthResult)
      {
        /* select the first address of the data segment into Rx package */
        RxBufferPtr = &RxBufferPtr[HeaderLength];
        if(XCP_PACKAGE_OK == GetLengthResult)
        {
          /* process received data by storing the received COMMAND/STIM in
           * corresponding queue and preparing it for processing. */
          Xcp_ProcessRxPdu(DataLength , RxBufferPtr, &IsFirstCmdUnpacked);
        }
#if(XCP_PROD_ERR_HANDLING_PDU_LOST != TS_PROD_ERR_DISABLE)
        else
        {
          /* a package is ignored resulting in data loss */
          ReportPduLostErr = TRUE;
        }
#endif
        /* select the address of the next Rx PDU into package */
        RxBufferPtr = &RxBufferPtr[(DataLength + TailLength)];
        /* Safety measure to catch an overflow, should not happen */
        /* CHECK: NOPARSE */
        if ((HeaderLength + DataLength + TailLength) > RemainingRxSpace)
        {
          RemainingRxSpace = 0U;
        }
        /* CHECK: PARSE */
        else
        {
          /* update remaining space */
          RemainingRxSpace -= (HeaderLength + DataLength + TailLength);
        }
      }
      else
      {
        /* take action to abort unpacking */
        RemainingRxSpace = 0U;
      }
#if ((XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) && (XCP_ON_FLEXRAY_ENABLED == STD_ON))
#if(XCP_NO_OF_CONNECTIONS > 1U)
      if((ConnectionCfgPtr->ConnectionType == XCP_CONNECTION_OVER_FLEXRAY) &&
         (XCP_IS_FLAG_SET(XCP_MULTI_PDUS_IN_FRAME_FLAG,
                        ConnectionCfgPtr->Properties,
                        uint8) == TRUE))
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
      {
        /* for subsequent FlexRay concatenated messages the length is only one byte containing the LEN field */
        HeaderLength = 1U;
      }
#endif /* ((XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) &&\
           (XCP_ON_FLEXRAY_ENABLED == STD_ON))*/
    }
    while(RemainingRxSpace > HeaderLength); /* while we reach the end of packed PDUs */
    /* Data has been unpacked - mark buffer as empty */
    XCP_CLEAR_BYTE_FLAG(RxBufferId, Xcp_RxLPduData.IsBufferFullFlag);
  }
  /* Exit critical section */
  SchM_Exit_Xcp_SCHM_XCP_EXCLUSIVE_AREA_XCP_INTERNALS();
#if (XCP_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON)
#if (XCP_NO_OF_CONNECTIONS > 1U)
  if(ConnectionCfgPtr->ConnectionType == XCP_CONNECTION_OVER_UDPIP)
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
  {
    if(LocalBufferFullSts == TRUE)
    {
      /* If flag is set, configure the socket */
      if (Xcp_ConfigureSocket == TRUE)
      {
        DstSoConId = ConnectionCfgPtr->SoAdTxConId;
      }
      else
      {
        /* Reconfigure the RX socket for each reception to have the remote port 0 (wild card
           * - accept all ports). Master can change its port but not the IP. */
        DstSoConId = ConnectionCfgPtr->SoAdRxConId;
      }
        /* If the command is CONNECT, we must configure the socket for transmission with the IP and
         * port received on the reception socket */
        Xcp_CopySoAdRemoteAddress(ConnectionCfgPtr->SoAdRxConId,
                                 DstSoConId,
                                 Xcp_ConfigureSocket);
      /* clear flag */
      Xcp_ConfigureSocket = FALSE;
    }
  }
#endif /* XCP_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON */
#if(XCP_PROD_ERR_HANDLING_PDU_LOST != TS_PROD_ERR_DISABLE)
  if(ReportPduLostErr == TRUE)
  {
    /* a package is ignored resulting in data loss.
     * a DET error notification will be issued */
    XCP_REPORT_PDU_LOST(SidId);
  }
  else
#endif
  {
    TS_PARAM_UNUSED(SidId);
  }

  DBG_XCP_UNPACKRXBUFFERDATA_EXIT();
}

/*-----------------------------[Xcp_ProcessRxPdu]------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_ProcessRxPdu
(
  PduLengthType SduLength,
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) SduDataPtr,
  P2VAR(boolean, AUTOMATIC, XCP_APPL_DATA) IsFirstCmdUnpackedPtr
)
{

  /* Variable to determine the Xcp action response */
  Xcp_RcvActionType RcvAction;

  DBG_XCP_PROCESSRXPDU_ENTRY(SduLength,SduDataPtr,IsFirstCmdUnpackedPtr);

  /* determine what to do with the incoming message */
  RcvAction = Xcp_GetRcvAction(SduDataPtr,IsFirstCmdUnpackedPtr);
  /* process CTO incoming message */
  Xcp_ProcessRcvAction(SduLength,SduDataPtr,RcvAction);

  DBG_XCP_PROCESSRXPDU_EXIT(IsFirstCmdUnpackedPtr);
}

/*-----------------------------[Xcp_GetRcvAction]------------------*/

STATIC FUNC(Xcp_RcvActionType, XCP_CODE) Xcp_GetRcvAction
(
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) SduDataPtr,
  P2VAR(boolean, AUTOMATIC, XCP_APPL_DATA) IsFirstCmdUnpackedPtr
)
{
  /* Local variable to hold the data packet PID (the first byte) */
  uint8 Pid = SduDataPtr[XCP_RES_PID_INDEX];
  /* Local variable to hold the Xcp_GetRcvAction() return value.
   * Initialize variable to ignore action. Change the value when a defined
   * type of action in certain conditions is received. */
  Xcp_RcvActionType RcvAction = XCP_ACT_IGNORE;

  DBG_XCP_GETRCVACTION_ENTRY(SduDataPtr,IsFirstCmdUnpackedPtr);

#if(XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_OFF)
  TS_PARAM_UNUSED(IsFirstCmdUnpackedPtr);
#endif

  /* If the PID corresponds to CMD packet */
  if (XCP_PID_IS_CMD(Pid))
  {
    if(Xcp_IsCmdAllowed(Pid,*IsFirstCmdUnpackedPtr))
    {
      switch (Xcp_ConnectionStatus.State)
      {
#if (XCP_STORE_DAQ_SUPPORTED == STD_ON)
        case XCP_STATE_RESUME: /* RESUME mode accepts the same commands like DISCONNECT mode, fall through*/
#endif /* (XCP_STORE_DAQ_SUPPORTED == STD_ON) */
        case XCP_STATE_DISCONNECTED:
        {
          /* only one command is accepted at this time */
          if (Xcp_CommandQueueIsEmpty() == TRUE)
          {
            /* accept only CONNECT or auto detection when the slave and master are disconnected */
            if( (XCP_PID_IS_CONNECT(Pid)) || (XCP_PID_IS_TRANSPORT_LAYER(Pid)) )
            {
              /* Set action to queue command */
              RcvAction = XCP_ACT_Q_CMD;
            }
          }
          break;
        }
        case XCP_STATE_CONNECTED:
        {
          /* If the PID corresponds to SYNCH command packet */
          if (XCP_PID_IS_SYNCH(Pid))
          {
            /* Set action to process the SYNC command */
            RcvAction = XCP_ACT_UNQ_CMD_SYNC;
          }
          /* If the PID corresponds to DISCONNECT command packet*/
          else if (XCP_PID_IS_DISCONNECT(Pid))
          {
            /* Set action to process the DISCONNECT command */
            RcvAction = XCP_ACT_UNQ_CMD_DISCONNECT;
          }
          /* Normal command (not SYNCH or DISCONNECT) was received */
          /* If the state is BUSY, the command is ignored and the busy command is aborted */
          else if (Xcp_GetCommandProcessorState() == XCP_CMDPROCESSOR_BUSY)
          {
            /* Set action to abort the BUSY command */
            RcvAction = XCP_ACT_UNQ_CMD_ABORTBUSY;
          }
          /* if the command queue is full, ignore the command, else queue command */
          else if (Xcp_CommandQueueIsFull() == FALSE)
          {
            /* Set action to queue command */
            RcvAction = XCP_ACT_Q_CMD;
          }
          else
          {
            /* Nothing to do. Just ignore the command */
          }
          break;
        }
        default:
          /* Ignore the packet */
          break;
      }
#if(XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
      /* signal that the first command has been unpacked */
      *IsFirstCmdUnpackedPtr = TRUE;
#endif
    }
  }
#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
  else /* This is a DTO packet */
  {
    /* Set action to STIM DATA direction */
    RcvAction = XCP_ACT_DATA_STIM;
  }
#endif

  DBG_XCP_GETRCVACTION_EXIT(RcvAction,IsFirstCmdUnpackedPtr);

  /* Return action */
  return RcvAction;
}

/*-----------------------------[Xcp_ProcessRcvAction]------------------*/

STATIC FUNC(void, XCP_CODE) Xcp_ProcessRcvAction
(
  PduLengthType SduLength,
  P2CONST(uint8, AUTOMATIC, XCP_APPL_DATA) SduDataPtr,
  Xcp_RcvActionType RcvAction
)
{
#if ( (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON) ||\
      (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON) ||\
      (XCP_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON) )
  /* Local variable to hold the data packet PID (the first byte) */
  uint8 Pid = SduDataPtr[XCP_RES_PID_INDEX];
#endif

  DBG_XCP_PROCESSRCVACTION_ENTRY(SduLength,SduDataPtr,RcvAction);

  switch(RcvAction)
  {
  case XCP_ACT_Q_CMD:
#if (XCP_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON)
#if (XCP_NO_OF_CONNECTIONS > 1U)
    if(Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].ConnectionType == XCP_CONNECTION_OVER_UDPIP)
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
    {
      if (XCP_PID_IS_CONNECT(Pid))
      {
        /* Set flag to change the SoAD */
        Xcp_ConfigureSocket = TRUE;
      }
    }
#endif /* (XCP_SOAD_PROTOCOL_UDP_SUPPORT == STD_ON) */

    /* Queue the command */
    Xcp_InsertCommand(SduLength, SduDataPtr);

#if (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON)
    if (Pid == XCP_CMD_PROGRAM_PID)
    {
      /* if the command is PROGRAM we set the programming in block mode transfer.
       * From now on we accept only PROGRAM_NEXT commands in the command queue until the
       * queue is emptied */
      Xcp_SetProgrammingBlockMode();
    }
#endif /* (XCP_MASTER_BLOCK_MODE_PGM_SUPPORTED == STD_ON) */

#if (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON)
    if (Pid == XCP_CMD_DOWNLOAD_PID)
    {
      /* if the command is DOWNLOAD we set the download in block mode flag.
       * From now on we accept only DOWNLOAD_NEXT commands in the command queue until the
       * queue is emptied */
      Xcp_SetDownloadInBlockMode();
    }
#endif /* (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON) */
    break;
  case XCP_ACT_UNQ_CMD_SYNC:
    /* Set a flag to signal that SYNCH needs to be processed */
    Xcp_SetSynchCmdPending();
    break;
  case XCP_ACT_UNQ_CMD_DISCONNECT:
    /* Set a flag to signal that DISCONNECT needs to be processed */
    Xcp_SetDisconnectCmdPending();
    break;
  case XCP_ACT_UNQ_CMD_ABORTBUSY:
    /* Abort any BUSY command, set the command processor state to IDLE */
    Xcp_AbortBusyCommand();
    break;
#if (XCP_RESOURCE_STIM == XCP_RESOURCE_STIM_MASK)
  case XCP_ACT_DATA_STIM:
    /* Insert DTO packet into the STIM Buffer */
    Xcp_InsertSTIM(SduDataPtr);
    break;
#endif
  default: /* XCP_ACT_IGNORE or any other value */
    /* Ignore the packet */
    break;
  }

  DBG_XCP_PROCESSRCVACTION_EXIT();
}

/*-----------------------------[Xcp_IsCmdAllowed]------------------*/

STATIC FUNC(boolean, XCP_CODE)  Xcp_IsCmdAllowed
(
  uint8 Pid,
  boolean IsFirstCmdUnpacked
)
{
  /* The return value */
  boolean RetValue = TRUE;
  DBG_XCP_ISCMDALLOWED_ENTRY(Pid,IsFirstCmdUnpacked);
 #if (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON)
  if(XCP_IS_MASTER_BLOCK_MODE_ACTIVE() == TRUE)
  {
    /* If there is a master block mode sequence, always accept block mode *NEXT command */
    /* !LINKSTO Xcp.ASAM.DOWNLOADMAX006,1 */
    /* !LINKSTO Xcp.ASAM.SHORTDOWNLOAD006,1 */
    RetValue = XCP_IS_MASTER_BLOCK_MODE_CMD(Pid);
    if(RetValue == FALSE)
    {
#if (XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
      if(IsFirstCmdUnpacked == TRUE)
      {
        /* ignore all interleave commands if the first command from the incoming
         * package was already unpacked */
        RetValue = FALSE;
      }
      else
#endif /* (XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON) */
      {
        /* If there is a master block mode sequence ignore all interleaved commands
         * except SYNC and DISCONNECT */
         RetValue =XCP_IS_SYNC_DISCONNECT_CMD(Pid);
      }
    }
  }
  else
  {
#if(XCP_MULTI_PDUS_IN_FRAME_SUPPORT == STD_ON)
    if(IsFirstCmdUnpacked == TRUE)
    {
      /* accept to unpack only the first detected interleave command */
      RetValue = FALSE;
    }
#endif
  }
#endif /* (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON) */

#if (XCP_MASTER_BLOCK_MODE_SUPPORTED == STD_ON)
#if (XCP_MULTI_PDUS_IN_FRAME_SUPPORT != STD_ON)
  TS_PARAM_UNUSED(IsFirstCmdUnpacked);
#endif
#else /* (XCP_MASTER_BLOCK_MODE_SUPPORTED != STD_ON) */
  TS_PARAM_UNUSED(IsFirstCmdUnpacked);
  TS_PARAM_UNUSED(Pid);
#endif


  DBG_XCP_ISCMDALLOWED_EXIT(RetValue);

   return RetValue;
}

/*-----------------------------[Xcp_InitialMinimumCmdLengthIsValid]------------------*/

STATIC FUNC(boolean, XCP_CODE) Xcp_InitialMinimumCmdLengthIsValid
(
  P2CONST(uint8, AUTOMATIC, AUTOSAR_COMSTACKDATA) SduDataPtr,
  PduLengthType DataLength
)
{
  boolean RetVal = TRUE;
  /* command ID */
  const uint8 CommandId = SduDataPtr[XCP_CMD_BYTE0_INDEX];

  /* Validate length of the TRANSPORT_LAYER_CMD */
  if(XCP_PID_IS_TRANSPORT_LAYER(CommandId))
  {
    if (DataLength < XCP_TRANSPORT_LAYER_CMD_MIN_LENGTH)
    {
      RetVal = FALSE;
    }
#if((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON) || \
    (XCP_ON_ETHERNET_ENABLED == STD_ON))
    else
    {
      if((Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].ConnectionType == XCP_CONNECTION_OVER_CAN) ||
         (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].ConnectionType == XCP_CONNECTION_OVER_CANFD))
      {
        if(XCP_SUBPID_IS_GET_SLAVE_ID(SduDataPtr[1U]))
        {
          if (DataLength < XCP_SUBCMD_GET_SLAVE_ID_ON_CAN_LENGTH)
          {
            RetVal = FALSE;
          }
        }
      }
      else if(Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].ConnectionType == XCP_CONNECTION_OVER_UDPIP)
      {
        if(XCP_SUBPID_IS_GET_SLAVE_ID(SduDataPtr[1U]))
        {
          if (DataLength < XCP_SUBCMD_GET_SLAVE_ID_ON_ETH_LENGTH)
          {
            RetVal = FALSE;
          }
        }
      }
      else
      {
        /* nothing to do */
      }
    }
#endif /* (XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON)) || (XCP_ON_ETHERNET_ENABLED == STD_ON) */
  }
  else if ( CommandId == XCP_CMD_UNLOCK_PID )
  {
    if (DataLength < XCP_UNLOCK_CMD_MIN_LENGTH)
    {
      RetVal = FALSE;
    }
  }
  else if (TRUE == IS_CMD_MASTER_BLOCK_MODE(CommandId))
  {
    if (DataLength < XCP_BLOCK_MODE_CMD_MIN_LENGTH)
    {
      RetVal = FALSE;
    }
  }
  else
  {
    RetVal = TRUE;
  }
  return RetVal;
}

/*-----------------------------[Xcp_IsCtoPduAccepted]------------------*/

STATIC FUNC(boolean, XCP_CODE) Xcp_IsCtoPduAccepted
(
  PduIdType RxPduId,
  P2CONST(uint8, AUTOMATIC, AUTOSAR_COMSTACKDATA) SduDataPtr,
  PduLengthType DataLength
)
{
#if((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON))
  P2CONST(Xcp_ConnectionCfgType, AUTOMATIC, XCP_APPL_CONST) ConnectionCfgPtr =
      &Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId];
#endif /* ((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON)) */
  boolean RetVal;

  DBG_XCP_ISCTOPDUACCEPTED_ENTRY(RxPduId, SduDataPtr);

  if(XCP_IS_FLAG_SET(XCP_SUPPORT_FOR_CMD_CTO_PDU,
                     Xcp_RxLPduIdConf[RxPduId].Properties,
                     uint8) == TRUE)
  {
    if (Xcp_InitialMinimumCmdLengthIsValid(SduDataPtr, DataLength) == FALSE)
    {
      /* The data packet length is not valid, send an ERR_CMD_SYNTAX response packet. */
      Xcp_SendCmdErrorPacket(SduDataPtr[0U], XCP_ERR_CMD_SYNTAX);
      RetVal = FALSE;
    }
    else
    {
#if((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON))
#if(XCP_NO_OF_CONNECTIONS > 1U)
      if((ConnectionCfgPtr->ConnectionType != XCP_CONNECTION_OVER_CAN) &&
         (ConnectionCfgPtr->ConnectionType != XCP_CONNECTION_OVER_CANFD))
      {
        /* this check doesn't apply for other bus types except CAN. */
        RetVal = TRUE;
      }
      else
#endif /* (XCP_NO_OF_CONNECTIONS > 1U) */
      {
        if(XCP_PID_IS_TRANSPORT_LAYER(SduDataPtr[0U]))
        {
          if(XCP_SUBPID_IS_GET_SLAVE_ID(SduDataPtr[1U]))
          {
            /* the GET_SLAVE_ID is allowed to be received only via the configured
             * CAN Id used for broadcasting this command*/
            if(ConnectionCfgPtr->BroadcastPduId == RxPduId)
            {
              RetVal = TRUE;
            }
            else
            {
              RetVal = FALSE;
            }
          }
          /* all other transport layer specific commands, excepting the GET_SLAVE_ID,
           * are allowed to be received only via the configured CAN id which
           * identifies the Xcp Slave */
          else if (ConnectionCfgPtr->CtoSlavePduId == RxPduId)
          {
            RetVal = TRUE;
          }
          else
          {
            RetVal = FALSE;
          }
        }
        /* all non transport layer specific commands are allowed to be received
         * only via the configured CAN id which identifies the Xcp Slave */
        else if(ConnectionCfgPtr->CtoSlavePduId == RxPduId)
        {
          RetVal = TRUE;
        }
        else
        {
          RetVal = FALSE;
        }
      }
#else
    TS_PARAM_UNUSED(SduDataPtr);
    RetVal = TRUE;
#endif /* ((XCP_ON_CAN_ENABLED == STD_ON) || (XCP_ON_CANFD_ENABLED == STD_ON)) */
    }
  }
  else
  {
    RetVal = FALSE;
  }

  DBG_XCP_ISCTOPDUACCEPTED_EXIT(RetVal);

  return RetVal;
}

/*-----------------------------[XcpGetIncRxPduInfoPtr]------------------*/
STATIC FUNC_P2VAR(uint8, XCP_VAR_CLEARED, XCP_CODE) XcpGetIncRxPduInfoPtr
(
  PduIdType IntPduId
)
{
  /* Get the value from configuration */
  P2VAR(uint8, AUTOMATIC, XCP_VAR_CLEARED) retValue =
    &(Xcp_RxLPduData.LPduBuffer[Xcp_RxLPduIdConf[IntPduId].LPduStartPos]);

#if ((XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON))
  if (Xcp_ConnectionCfg[Xcp_ConnectionStatus.ConnectionId].ConnectionType == XCP_CONNECTION_OVER_FLEXRAY)
  {
     /* Get the value at runtime */
     retValue = &(Xcp_RxLPduData.LPduBuffer[Xcp_RxLPduData.LPduStartPos[Xcp_RxLPduIdConf[IntPduId].IntPduBufId]]);
  }
#endif /* (XCP_ON_FLEXRAY_ENABLED == STD_ON) && (XCP_FLEXRAY_TP_COMMANDS_SUPPORTED == STD_ON)*/
  return retValue;
}


#define XCP_STOP_SEC_CODE
#include <Xcp_MemMap.h>
#endif /* ( (XCP_DEFAULT_XCP_MODULE_STATE == STD_ON) || (XCP_ENABLE_XCP_CONTROL_API == STD_ON) ) */

/*==================[end of file]===========================================*/
