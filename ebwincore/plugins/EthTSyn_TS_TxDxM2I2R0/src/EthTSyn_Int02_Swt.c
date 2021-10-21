/**
 * \file
 *
 * \brief AUTOSAR EthTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module EthTSyn.
 *
 * \version 2.2.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]============================================*/
#include <EthTSyn_Int02_Swt.h> /* Unit internal header file. */
#include <EthTSyn.h>
#include <EthTSyn_Trace.h> /* Debug and trace */
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
#include <Det.h> /* DET support */
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */

/* !LINKSTO EthTSyn.EB.SwitchConfigured, 1 */
#if(ETHTSYN_SWITCH_USED == STD_ON)
#include <EthSwt.h> /* Module public API */
#endif

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/
#define ETHTSYN_START_SEC_CODE
#include <EthTSyn_MemMap.h>
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
/**
 * \brief Service to process incoming Sync frames from EthSwt.
 *
 * \param[in] IngressTimeStampPtr - Ingress timestamp, provided by the EthSwt module.
 * \param[in] DataPtr - Pointer to the payload of the frame.
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 *
 *
 * \return TRUE: Timestamp is required for T5(egressTS from the hostport).
 *         FALSE: No timestamp is required.
 */
 
STATIC FUNC(boolean, ETHTSYN_CODE) EthTSyn_Inc02_ProcessRxSyncSwtMsg
(
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) IngressTimeStampPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint8 EthTSynCtrlIdx,
  uint32 UniqueId
);

/**
 * \brief Service to process incoming Sync frames from EthSwt.
 *
 * \param[in] DataPtr - Pointer to the payload of the frame.
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] Len - Length of the payload.
 *
 *
 * \return
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_ProcessRxFupSyncSwtMsg
(
  P2VAR(Eth_DataType, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint8 EthTSynCtrlIdx,
  uint16 Len
);

/**
 * \brief Service to synchronize the time base
 *
 * \param[in] OtsPdelayCorrPtr - Time used for the simple bridge synchronization.
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_SynchronizeBridge
(
  P2VAR(Eth_TimeIntDiffType, AUTOMATIC, ETHTSYN_APPL_DATA) OtsPdelayCorrPtr,
  uint8 EthTSynCtrlIdx
);

/**
 * \brief Service to add valid OTS and correctionField values.
 *
 * \param[in] DataPtr - Pointer to the payload of the frame.
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_AddOtsCorr
(
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint8 EthTSynCtrlIdx
);

/**
 * \brief Service to process received sync frame.
 *
 * \param[in] DataPtr - Pointer to the payload of the frame.
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 *
 */
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_ProcessRxSyncFrame
(
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint8 EthTSynCtrlIdx
);
#endif
#if(ETHTSYN_SWITCH_USED == STD_ON)
/**
 * \brief Service to process incoming PdelayReq frames from EthSwt.
 *
 * \param[in] MgmtInfoPtr - Port information.
 * \param[in] IngressTimeStampPtr - Ingress timestamp, provided by the EthSwt module.
 * \param[in] DataPtr - Pointer to the payload of the frame.
 * \param[in] EthTSynCtrlIdx - EthTSyn controller index the frame belongs to.
 * \param[in] Len - Length of the payload.
 *
 */
 
STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_ProcessRxPdelayReqSwtMsg
(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHTSYN_APPL_DATA) MgmtInfoPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) IngressTimeStampPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint8 EthTSynCtrlIdx,
  uint16 Len
);
#endif/* ETHTSYN_SWITCH_USED == STD_ON */

#define ETHTSYN_STOP_SEC_CODE
#include <EthTSyn_MemMap.h>
/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/
#define ETHTSYN_START_SEC_CODE
#include <EthTSyn_MemMap.h>

#if(ETHTSYN_SWITCH_USED == STD_ON)

TS_MOD_PRIV_DEFN FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_TransmitSwtOnHostPortSyncFrame
(
  uint8 EthTSynCtrlIdx
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  EthTSyn_CtrlSyncTxStateType SyncTxState;
  Eth_BufIdxType BufIdx = 0U;
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) PTPPayloadPtr;
  EthTSyn_PTPHeaderType PTPHeader;
  uint8 Tmp_buffer[ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_SYNC_PAYLOADSIZE];

  const EthTSyn_IntPortIdxType IntHostPortIdx
    = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt;

  DBG_ETHTSYN_TRANSMITSWTSYNCHOSTPORTFRAME_ENTRY(EthTSynCtrlIdx);

  TS_AtomicAssign8
  (
    SyncTxState,
    EthTSyn_Ctrl[EthTSynCtrlIdx].SyncTxState
  );
  if(ETHTSYN_CTRL_SYN_TX_STATE_IDLE != SyncTxState)
  {
    EthTSyn_ResetSynTxStateMachine(EthTSynCtrlIdx);
  }
  
  PTPHeader.MsgLength = ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_SYNC_PAYLOADSIZE;
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SimpleBridge == FALSE)
  {
#endif
    /* Prepare time sync frame PTP header. */
    PTPHeader.Flags = ETHTSYN_PTP_HEADER_FLAG_TWOSTEP;
    PTPHeader.Flags |= ETHTSYN_PTP_HEADER_FLAG_PTPTIMESCALE;
    PTPHeader.MsgType = ETHTSYN_PTP_HEADER_MSGTYPE_SYNC;
    PTPHeader.Control = ETHTSYN_PTP_HEADER_CONTROL_SYNC;

    /* Set sequence number. */
    /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00189, 1 */
    TS_AtomicAssign16(PTPHeader.SequNumber, EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber);

    PTPHeader.LogMsgInterval = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SyncTxMsgLogInterval;

    /* Fill PTP header */
    EthTSyn_FillPTPHeader(EthTSynCtrlIdx, &PTPHeader, &Tmp_buffer[0U]);
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
  }
  else
  {
    /* If a TAB with GTM not as Mgm CPU is configured and EthTSyn is configured to handle the fowrding of the frames, copy the
     * received frame. */
    TS_MemCpy(&Tmp_buffer, &EthTSyn_Ctrl[EthTSynCtrlIdx].ReceivedSyncFrame, PTPHeader.MsgLength);
  }
#endif

#if(ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT == STD_OFF)
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SwtPortIdxInCorrField)
  {
    /* !LINKSTO EthTSyn.EB.SwitchPortIdx.StoredInCorrectionField, 1 */
    Tmp_buffer[ETHTSYN_PTP_HEADER_B14_CORRFIELD_6] =
        (uint8)EthTSyn_RootCfgPtr->SwitchConfig[IntHostPortIdx].EthSwtIdx;
    Tmp_buffer[ETHTSYN_PTP_HEADER_B15_CORRFIELD_7] =
        (uint8)EthTSyn_RootCfgPtr->SwitchConfig[IntHostPortIdx].EthSwtPortIdx;
  }
#endif /*(ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT == STD_OFF)*/

  /* Set PTP payload */
  /* All bytes are reserved. */
  TS_MemBZero(&Tmp_buffer[ETHTSYN_PTP_HEADER_SIZE], ETHTSYN_PTP_SYNC_PAYLOADSIZE);

  /* !LINKSTO EthTSyn.EB.EthSwt_TransmitSyncFrameOnHostPort, 1 */
  RetVal = EthTSyn_Inc02_GetSwtTxBuffer
     (EthTSynCtrlIdx, PTPHeader.MsgLength, IntHostPortIdx, &BufIdx, &PTPPayloadPtr);

  /* !LINKSTO EthTSyn.EB.EthSwt_Return_NotOk, 1 */
  if(RetVal == E_OK)
  {

    TS_MemCpy(PTPPayloadPtr, Tmp_buffer, PTPHeader.MsgLength);

    EthTSyn_Ctrl[EthTSynCtrlIdx].Sync_BufIdx = BufIdx;
#if((ETHTSYN_TIME_RECORDING == STD_ON) && (ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT == STD_ON))
    {
      /* If EthTSynSendSyncFrameOnlyOnHostPort is TRUE and time recording enabled, EthTSyn
         has to save the sequenceId, sourcePortId */
      uint8 Idx = 0U;
      boolean Found = FALSE;
      uint8 StartPort = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart + 1U;
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
      /* check only the master ports */
      StartPort = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart + 2U;
#endif /* ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON */
      for
      (
         Idx = StartPort;
        ((Idx < (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount +
                 EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart
                )
         )
            && (FALSE == Found)
        );
         Idx++
      )
      {
        if(EthTSyn_RootCfgPtr->SwitchConfig[Idx].TimeValidationEnabled == TRUE)
        {
          /* Save SyncSequNumber to be reported to StbM for time recording */
          TS_AtomicAssign16(EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sequenceId,
                                                            EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber);
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity =
               (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0]) << 56U);
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
               (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B21_SRCPORTIDENTITY_1]) << 48U);
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
               (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B22_SRCPORTIDENTITY_2]) << 40U);
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
               (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B23_SRCPORTIDENTITY_3]) << 32U);
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
               (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B24_SRCPORTIDENTITY_4]) << 24U);
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
               (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B25_SRCPORTIDENTITY_5]) << 16U);
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
               (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B26_SRCPORTIDENTITY_6]) << 8U);
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
               (uint64)((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B27_SRCPORTIDENTITY_7]);
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.portNumber =
               (uint16)((uint16) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8]);
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.portNumber |=
               (uint16)((uint16) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9]);
          /* Is enough to check that at least one port from the current timedomain has the timevalidation enabled */
          Found = TRUE;
        }
      }
    }
#endif
    RetVal = EthTSyn_Inc02_TransmitSwtFrame(EthTSynCtrlIdx, BufIdx, TRUE, IntHostPortIdx, TRUE, PTPHeader.MsgLength, FALSE);
  }

  /* !LINKSTO EthTSyn.EB.EthSwt_Return_NotOk, 1 */
  if(RetVal == E_OK)
  {
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
    if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SimpleBridge == FALSE)
#endif /* ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON */
    {
      /* !LINKSTO EthTSyn.Dsn.HostPortSynTxState_INIT-WAIT_INGRESS, 1 */
      TS_AtomicAssign8
      (
        EthTSyn_SwitchPorts[IntHostPortIdx].SyncTxState,
        ETHTSYN_PORT_SYN_TX_STATE_WAIT_INGRESS_TS
      );
      /* !LINKSTO EthTSyn.Dsn.CtrlSynTxState_IDLE-WAIT_EGRESS, 1 */
      TS_AtomicAssign8
      (
        EthTSyn_Ctrl[EthTSynCtrlIdx].SyncTxState,
        ETHTSYN_CTRL_SYN_TX_STATE_WAIT_FOR_ETH_EGRESS_TS
      );
    }
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
    else
    {
      uint8 Idx;
      /* Prepare the master ports for receiving the egressTS. */
      for(
          Idx = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart + 2U;
          Idx < (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount +
              EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart);
          Idx++
         )
      {
        TS_AtomicAssign8
        (
            EthTSyn_SwitchPorts[Idx].SyncTxState,
            ETHTSYN_PORT_SYN_TX_STATE_WAIT_EGRESS_TS
        );
      }
    }
#endif /* ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON */
  }

  DBG_ETHTSYN_TRANSMITSWTSYNCHOSTPORTFRAME_EXIT(EthTSynCtrlIdx);
}


TS_MOD_PRIV_DEFN FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_TransmitSwtSyncFramePerPort
(
  uint8 EthTSynCtrlIdx,
  uint8 PortIdx
)
{
  Std_ReturnType RetVal;
  Eth_BufIdxType BufIdx = 0U;
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) PTPPayloadPtr ;
  EthTSyn_PTPHeaderType PTPHeader;
  uint8 Tmp_buffer[ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_SYNC_PAYLOADSIZE];
  DBG_ETHTSYN_INC02_TRANSMITSWTSYNCFRAMEPERPORT_ENTRY(EthTSynCtrlIdx);

  PTPHeader.MsgLength = ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_SYNC_PAYLOADSIZE;

#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SimpleBridge == FALSE)
  {
#endif
    /* Prepare time sync frame PTP header. */
    PTPHeader.Flags = ETHTSYN_PTP_HEADER_FLAG_TWOSTEP;
    PTPHeader.Flags |= ETHTSYN_PTP_HEADER_FLAG_PTPTIMESCALE;
    PTPHeader.MsgType = ETHTSYN_PTP_HEADER_MSGTYPE_SYNC;
    PTPHeader.Control = ETHTSYN_PTP_HEADER_CONTROL_SYNC;

    /* Set sequence number. */
    /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00189, 1 */
    TS_AtomicAssign16(PTPHeader.SequNumber, EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber);

    PTPHeader.LogMsgInterval = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SyncTxMsgLogInterval;

    /* Fill PTP header */
    EthTSyn_FillPTPHeader(EthTSynCtrlIdx, &PTPHeader, &Tmp_buffer[0U]);

    /* Set PTP payload */
    /* All bytes are reserved. */
    TS_MemBZero(&Tmp_buffer[ETHTSYN_PTP_HEADER_SIZE], ETHTSYN_PTP_SYNC_PAYLOADSIZE);
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
  }
  else
  {
    /* If a TAB with GTM not as Mgm CPU is configured and EthTSyn is configured to handle the fowrding of the frames, copy the
     * received frame. */
    TS_MemCpy(&Tmp_buffer, &EthTSyn_Ctrl[EthTSynCtrlIdx].ReceivedSyncFrame, PTPHeader.MsgLength);
  }
#endif

  /* !LINKSTO EthTSyn.EB.EthSwt_TransmitSyncFrameOnPorts, 1 */
  /* !LINKSTO EthTSyn.EB.TransparentClock_EthTSynSendSync_FailedFw, 1 */
  RetVal = EthTSyn_Inc02_GetSwtTxBuffer
      (EthTSynCtrlIdx, PTPHeader.MsgLength, PortIdx, &BufIdx, &PTPPayloadPtr);

#if(ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT == STD_OFF)
  /* !LINKSTO EthTSyn.EB.EthSwt_Return_NotOk, 1 */
  if(E_OK == RetVal)
  {
    if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SwtPortIdxInCorrField)
    {
      Tmp_buffer[ETHTSYN_PTP_HEADER_B14_CORRFIELD_6] =
          (uint8)EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].EthSwtIdx;
      Tmp_buffer[ETHTSYN_PTP_HEADER_B15_CORRFIELD_7] =
          (uint8)EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].EthSwtPortIdx;
    }
  }
  /* If the EthTSyn portid is configured update the portid bytes form the sourceportid field. */
  if(EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].EthTSynSourcePortId != 0xFFFFU)
  {
    /* 16 bit port number */
    Tmp_buffer[ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8] =
      (uint8)((uint16)(EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].EthTSynSourcePortId & ETHTSYN_16BIT_MASK_MSB) >> 8U);
    Tmp_buffer[ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9] =
      (uint8)((uint16)(EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].EthTSynSourcePortId & ETHTSYN_16BIT_MASK_LSB));
  }
#endif
  TS_MemCpy(PTPPayloadPtr, &Tmp_buffer, PTPHeader.MsgLength);

  /* !LINKSTO EthTSyn.EB.EthSwt_Return_NotOk, 1 */
  if(E_OK == RetVal)
  {
#if(ETHTSYN_TIME_RECORDING == STD_ON)
    if(EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].TimeValidationEnabled == TRUE)
    {
      /* Save SyncSequNumber to be reported to StbM for time recording */
      /* The seqId is the same for all ports. */
      TS_AtomicAssign16(EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sequenceId,
                                                        EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber);
      EthTSyn_SwitchPorts[PortIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity =
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0]) << 56U);
      EthTSyn_SwitchPorts[PortIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B21_SRCPORTIDENTITY_1]) << 48U);
      EthTSyn_SwitchPorts[PortIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B22_SRCPORTIDENTITY_2]) << 40U);
      EthTSyn_SwitchPorts[PortIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B23_SRCPORTIDENTITY_3]) << 32U);
      EthTSyn_SwitchPorts[PortIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B24_SRCPORTIDENTITY_4]) << 24U);
      EthTSyn_SwitchPorts[PortIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B25_SRCPORTIDENTITY_5]) << 16U);
      EthTSyn_SwitchPorts[PortIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B26_SRCPORTIDENTITY_6]) << 8U);
      EthTSyn_SwitchPorts[PortIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
           (uint64)((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B27_SRCPORTIDENTITY_7]);
      EthTSyn_SwitchPorts[PortIdx].SyncMasterTimeRecording.sourcePortId.portNumber =
           (uint16)((uint16) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8]);
      EthTSyn_SwitchPorts[PortIdx].SyncMasterTimeRecording.sourcePortId.portNumber |=
           (uint16)((uint16) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9]);
    }
#endif
    /* !LINKSTO EthTSyn.EB.EthSwt_TransmitSyncFrameOnPorts, 1 */
    RetVal = EthTSyn_Inc02_TransmitSwtFrame(EthTSynCtrlIdx, BufIdx, TRUE, PortIdx, TRUE, PTPHeader.MsgLength, FALSE);

    /* !LINKSTO EthTSyn.EB.EthSwt_Return_NotOk, 1 */
    if(E_OK == RetVal)
    {
      /* !LINKSTO EthTSyn.Dsn.PortSynTxState_IDLE-WAIT_EGRESS, 1 */
      TS_AtomicAssign8
      (
        EthTSyn_SwitchPorts[PortIdx].SyncTxState,
        ETHTSYN_PORT_SYN_TX_STATE_WAIT_EGRESS_TS
      );
    }
  }
  DBG_ETHTSYN_INC02_TRANSMITSWTSYNCFRAMEPERPORT_EXIT(EthTSynCtrlIdx);
}

TS_MOD_PRIV_DEFN FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_TransmitSyncSwtFUpFramePerPort
(
  uint8 EthTSynCtrlIdx,
  EthTSyn_IntPortIdxType PortIdx
)
{
  EthTSyn_IntPortIdxType Idx;
  EthTSyn_IntPortIdxType PortIdleCounter = 0U;
  EthTSyn_PortSyncTxStateType PortSynTxState;
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
  uint8 Crc_Time_1;
#endif
  uint16 SubTlvSize = 0U;
  Std_ReturnType RetVal;
  Eth_BufIdxType BufIdx = 0U;

  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) PTPPayloadPtr;
  EthTSyn_PTPHeaderType PTPHeader;


  DBG_ETHTSYN_INC02_TRANSMITSYNCSWTFUPFRAMEPERPORT_ENTRY(EthTSynCtrlIdx);

  /* Prepare PTP header. */
  PTPHeader.Flags = ETHTSYN_PTP_HEADER_FLAG_PTPTIMESCALE;
  PTPHeader.MsgType = ETHTSYN_PTP_HEADER_MSGTYPE_FUP;
  PTPHeader.Control = ETHTSYN_PTP_HEADER_CONTROL_FUP;

  /* Set sequence number. */
  /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00190, 1 */
  TS_AtomicAssign16(PTPHeader.SequNumber, EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber);

  /* Same value as sync frame (See 802.1AS-2011 Ch 11.4.2.8) */
  PTPHeader.LogMsgInterval = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SyncTxMsgLogInterval;

#if(ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)

  /* Increment the frame size with 10 bytes */
  SubTlvSize += ETHTSYN_FUP_AUTOSAR_TLV_HEADER_LENGTH;

  /* If Time SubTLV is enabled, increment the frame size with 5 bytes */
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TLVFollowUpTimeSubTLV == TRUE)
  {
    SubTlvSize += ETHTSYN_FUP_SUBTLV_TIME_SIZE;
  }
  /* If Status SubTLV is enabled, increment the frame size with 4 bytes */
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TLVFollowUpStatusSubTLV == TRUE)
  {
    SubTlvSize += ETHTSYN_FUP_SUBTLV_STATUS_SIZE;
  }
  /* If UserData SubTLV is enabled, increment the frame size with 7 bytes */
  if(
      (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TLVFollowUpUserDataSubTLV == TRUE) &&
      (EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userDataLength  != 0U)
    )
  {
    SubTlvSize += ETHTSYN_FUP_SUBTLV_USERDATA_SIZE;
  }

  /* If OFS SubTLV is enabled, increment the frame size with 19 bytes */
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TLVFollowUpOFSSubTLV == TRUE)
  {
    SubTlvSize += ETHTSYN_FUP_SUBTLV_OFS_SIZE;
  }

  if(SubTlvSize == ETHTSYN_FUP_AUTOSAR_TLV_HEADER_LENGTH)
  {
    SubTlvSize = 0U;
  }
#endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_ON) */

  PTPHeader.MsgLength = ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_SYNC_FUP_PAYLOADSIZE + SubTlvSize;
  
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SimpleBridge == TRUE)
  {
    PTPHeader.MsgLength = EthTSyn_Ctrl[EthTSynCtrlIdx].ReceivedFupFrameLength;
  }
#endif /* (ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF) */

    /* !LINKSTO EthTSyn.EB.EthSwt_TransmitSyncFupFrameOnPorts, 1 */
    RetVal = EthTSyn_Inc02_GetSwtTxBuffer
      (EthTSynCtrlIdx, PTPHeader.MsgLength, PortIdx, &BufIdx, &PTPPayloadPtr);
    /* !LINKSTO EthTSyn.EB.EthSwt_Return_NotOk, 1 */
    /* !LINKSTO EthTSyn.EB.BridgeWithGTMnotMgmCPU_FailedSyncFUp, 1 */
    if(RetVal == E_OK)
    {
      uint8 PayloadIdx = 0U;
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
      if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SimpleBridge == FALSE)
      {
#endif
        /* Fill PTP header */
        EthTSyn_FillPTPHeader(EthTSynCtrlIdx, &PTPHeader, &PTPPayloadPtr[PayloadIdx]);
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
      }
      else
      {
        /* If a transparent clock is used copy the received frame */
        TS_MemCpy(PTPPayloadPtr, &EthTSyn_Ctrl[EthTSynCtrlIdx].ReceivedFupFrame, EthTSyn_Ctrl[EthTSynCtrlIdx].ReceivedFupFrameLength);
      }
#endif /* (ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF) */

#if(ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT == STD_OFF)
      /* If the EthTSyn portid is configured update the portid bytes form the sourceportid field. */
      if(EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].EthTSynSourcePortId != 0xFFFFU)
      {
        /* 16 bit port number */
        PTPPayloadPtr[ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8] =
          (uint8)((uint16)(EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].EthTSynSourcePortId & ETHTSYN_16BIT_MASK_MSB) >> 8U);
        PTPPayloadPtr[ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9] =
          (uint8)((uint16)(EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].EthTSynSourcePortId & ETHTSYN_16BIT_MASK_LSB));
      }
#endif
      /* update correction field PTP header */
      /* !LINKSTO EthTSyn.EB.EthSwt_UpdateCorrectionField, 1 */
      EthTSyn_Inc02_SwtCorrFieldPTPHeader(EthTSynCtrlIdx, &PTPPayloadPtr[PayloadIdx], PortIdx);

      PayloadIdx += ETHTSYN_PTP_HEADER_SIZE; /* Set payload index to next free space. */
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
      if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SimpleBridge == FALSE)
      {
#endif /* (ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF) */
        /* Set PTP payload */
        EthTSyn_TimeStampToArray
        (
          &EthTSyn_Ctrl[EthTSynCtrlIdx].OriginTimeStamp, &PTPPayloadPtr[PayloadIdx]
        );
        PayloadIdx += ETHTSYN_TIME_STAMP_SIZE; /* Set payload index to next free space. */
        /* Add IEEE TLV information. */
        EthTSyn_FillFUpTLV(&PTPPayloadPtr[PayloadIdx]);
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
      }
#endif /* (ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF) */

#if((ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) && (ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF))
     if(SubTlvSize > 0U)
     {
       /* The AUTOSAR TLV is used */
       EthTSyn_FillFUpAutosarTLV(EthTSynCtrlIdx, &PTPPayloadPtr[0U]);
     }
#endif /* (ETHTSYN_MESSAGE_COMPLIANCE == STD_ON) */
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
     if((PTPPayloadPtr[ETHTSYN_FUP_AUTOSAR_SUBTLV_START_ADDR] == ETHTSYN_FUP_SUBTLV_TIME_TYPE) &&
        ((PTPPayloadPtr
        [ETHTSYN_FUP_AUTOSAR_SUBTLV_START_ADDR + ETHTSYN_FUP_SUBTLV_TIME_B2_TIME_FLAGS] &
         ETHTSYN_CORRECTION_FIELD_BIT_MASK) != 0U
        )
       )
      {
        /* !LINKSTO EthTSyn.ASR151.PRS_TS_00209, 1 */
        EthTSyn_Inc02_BridgeCalculateCrcTime1(EthTSynCtrlIdx, &PTPPayloadPtr[0U], &Crc_Time_1);
        PTPPayloadPtr[ETHTSYN_FUP_AUTOSAR_SUBTLV_START_ADDR +
                      ETHTSYN_FUP_SUBTLV_TIME_B4_CRC_TIME_1] = Crc_Time_1;
      }
#endif
#if(ETHTSYN_TIME_RECORDING == STD_ON) 
      if(EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].TimeValidationEnabled == TRUE)
      {
#if(ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT == STD_OFF)
        /* update port specific members */
        EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId = 
            EthTSyn_SwitchPorts[PortIdx].SyncMasterTimeRecording.sourcePortId;
#endif
        EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.correctionField =
            EthTSyn_SwitchPorts[PortIdx].SyncMasterTimeRecording.correctionField;
        /* Save preciseOriginTimeStamp to be reported to StbM for time recording */
        EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.preciseOriginTimestamp.nanoseconds =
                                     EthTSyn_Ctrl[EthTSynCtrlIdx].OriginTimeStamp.nanoseconds;
        EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.preciseOriginTimestamp.seconds =
                                     EthTSyn_Ctrl[EthTSynCtrlIdx].OriginTimeStamp.seconds;
        EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.syncEgressTimestamp =
                         EthTSyn_SwitchPorts[PortIdx].SyncMasterTimeRecording.syncEgressTimestamp;
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
        if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SimpleBridge == TRUE)
        {
          Eth_TimeStampType OriginTimeStamp;
          
          OriginTimeStamp.nanoseconds = 0U;
          OriginTimeStamp.seconds = 0U;
          OriginTimeStamp.secondsHi = 0U;
          /* If a transparent clock is used, EthTSyn will extract the data from the received fup and this will be
             passed to the StbM */
          /* Save SyncSequNumber to be reported to StbM for time recording */
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sequenceId =
                                                     EthTSyn_GetPTPHeaderSequNumber(PTPPayloadPtr);
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity =
               (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0]) << 56U);
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
               (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B21_SRCPORTIDENTITY_1]) << 48U);
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
               (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B22_SRCPORTIDENTITY_2]) << 40U);
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
               (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B23_SRCPORTIDENTITY_3]) << 32U);
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
               (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B24_SRCPORTIDENTITY_4]) << 24U);
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
               (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B25_SRCPORTIDENTITY_5]) << 16U);
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
               (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B26_SRCPORTIDENTITY_6]) << 8U);
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.clockIdentity |=
               (uint64)((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B27_SRCPORTIDENTITY_7]);
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.portNumber =
               (uint16)((uint16) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8]);
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.sourcePortId.portNumber |=
               (uint16)((uint16) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9]);
          
          /* Get origin time stamp of the master from follow up frame payload. */
          EthTSyn_ArrayToTimeStamp(&OriginTimeStamp, &PTPPayloadPtr[ETHTSYN_PTP_HEADER_SIZE]);
          /* Save preciseOriginTimeStamp to be reported to StbM for time recording */
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.preciseOriginTimestamp.nanoseconds =
                                                                       OriginTimeStamp.nanoseconds;
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording.preciseOriginTimestamp.seconds =
                                                                       OriginTimeStamp.seconds;
        }
#endif /* (ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF) */              
        (void)StbM_EthSetMasterTimingData
          (
              EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId,
              /* !LINKSTO EthTSyn.ASR20-11.SWS_EthTSyn_00215, 1 */
              &EthTSyn_Ctrl[EthTSynCtrlIdx].SyncMasterTimeRecording
          );
      }
#endif /* ETHTSYN_TIME_RECORDING == STD_ON */
      /* !LINKSTO EthTSyn.EB.EthSwt_TransmitSyncFupFrameOnPorts, 1 */
      RetVal = EthTSyn_Inc02_TransmitSwtFrame
                (EthTSynCtrlIdx, BufIdx, FALSE, PortIdx, FALSE, PTPHeader.MsgLength, FALSE);
      /* !LINKSTO EthTSyn.EB.EthSwt_Return_NotOk, 1 */
      if(E_OK == RetVal)
      {
        /* !LINKSTO EthTSyn.Dsn.PortSynTxState_READY_FUP-IDLE, 1 */
        TS_AtomicAssign8
        (
          EthTSyn_SwitchPorts[PortIdx].SyncTxState,
          ETHTSYN_PORT_SYN_TX_STATE_IDLE
        );
      }
    }

  for(
       Idx =  EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart;
       Idx < (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount
           +  EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart);
       Idx++
     )
  {
    TS_AtomicAssign8
    (
        PortSynTxState,
        EthTSyn_SwitchPorts[Idx].SyncTxState
    );

    if(ETHTSYN_PORT_SYN_TX_STATE_IDLE == PortSynTxState)
    {
      PortIdleCounter++;
    }
  }

  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount == PortIdleCounter)
  {
    /* !LINKSTO EthTSyn.Dsn.CtrlSynTxState_PORT_READY_FUP-IDLE, 1 */
    /* !LINKSTO EthTSyn.Dsn.CtrlSynBridge_PORT_READY-IDLE, 1 */
    EthTSyn_ResetSynTxStateMachine(EthTSynCtrlIdx);
  }

  DBG_ETHTSYN_INC02_TRANSMITSYNCSWTFUPFRAMEPERPORT_EXIT(EthTSynCtrlIdx);
}

TS_MOD_PRIV_DEFN FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_TransmitSwtPdelayReqFramePerPort
(
  uint8 EthTSynCtrlIdx,
  uint8 PortIdx
)
{
  Std_ReturnType TmpRet;
  Eth_BufIdxType BufIdx = 0U;
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) PTPPayloadPtr;
  EthTSyn_PTPHeaderType PTPHeader;
  uint16 PdelayReqSequNumber;

  DBG_ETHTSYN_INC02_TRANSMITSWTPDELAYREQFRAMEPERPORT_ENTRY(EthTSynCtrlIdx, PortIdx);

  TS_AtomicAssign16(PdelayReqSequNumber, EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReqSequNumber);
  /* Increment sequence number. The Sequence Counter shall wrap around at 65535 to 0 again. */
  /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00188, 1 */
  if(ETHTSYN_SEQ_NUM_MAX == PdelayReqSequNumber)
  {
    /* The first transmission of the Pdelay_Req frame shall contain seqId 0.*/
    /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00187_PdelayReq, 1 */
    PdelayReqSequNumber = 0U;
  }
  else
  {
    PdelayReqSequNumber++;
  }
  TS_AtomicAssign16(EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReqSequNumber, PdelayReqSequNumber);

  /* Prepare PTP header. */
  PTPHeader.Flags = ETHTSYN_PTP_HEADER_NOFLAGS;
  PTPHeader.Flags |= ETHTSYN_PTP_HEADER_FLAG_PTPTIMESCALE;
  PTPHeader.MsgType = ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_REQ;
  PTPHeader.Control = ETHTSYN_PTP_HEADER_CONTROL_PDELAY;

  /* Set sequence number. */
  PTPHeader.SequNumber = PdelayReqSequNumber;

  PTPHeader.LogMsgInterval
    = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PdelayTxMsgLogInterval;

  PTPHeader.MsgLength = ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_PDELAY_REQ_PAYLOADSIZE;

  /* !LINKSTO EthTSyn.EB.TransparentClock_PdelayReqTransmission_FailedBuffer, 1 */
  TmpRet = EthTSyn_Inc02_GetSwtTxBuffer
      (EthTSynCtrlIdx, PTPHeader.MsgLength, PortIdx, &BufIdx, &PTPPayloadPtr);

  if(E_OK == TmpRet)
  {
    /* Fill PTP header */
    EthTSyn_FillPTPHeader(EthTSynCtrlIdx, &PTPHeader, &PTPPayloadPtr[0U]);

    /* Set PTP payload */
    /* All bytes are reserved. */
    TS_MemBZero(&PTPPayloadPtr[ETHTSYN_PTP_HEADER_SIZE], ETHTSYN_PTP_PDELAY_REQ_PAYLOADSIZE);
#if(ETHTSYN_MAX_SLAVE != 0U)
      /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */
#if(ETHTSYN_TIME_RECORDING == STD_ON)
      if(EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].TimeValidationEnabled == TRUE)
      {
        StbM_TimeStampType StbMTimeStamp;
        StbM_UserDataType UserData;
        /* Save the value of the Virtual Local Time of the reference Global Time Tuple
         *  - referenceLocalTimestamp*/
        /* Save the value of the local instance of the Global Time of the reference
         *  Global Time Tuple referenceGlobalTimestamp */
        /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00223, 1 */
        (void)StbM_BusGetCurrentTime
        (
            EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId, /* timeBaseId */
            &StbMTimeStamp,
            &EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.referenceLocalTimestamp,
            &UserData
        );
        EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.referenceGlobalTimestamp.
          nanoseconds = StbMTimeStamp.nanoseconds;
        EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.referenceGlobalTimestamp.
          seconds     = StbMTimeStamp.seconds;
        /* Save the Sequence Id of sent Pdelay_Req frame */
       TS_AtomicAssign8
        (
        EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.sequenceId,
        PdelayReqSequNumber
        );

       /* Save the sourcePortId of sent Pdelay_Req frame*/
       EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.clockIdentity =
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0]) << 56U);
       EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.clockIdentity |=
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B21_SRCPORTIDENTITY_1]) << 48U);
       EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.clockIdentity |=
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B22_SRCPORTIDENTITY_2]) << 40U);
       EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.clockIdentity |=
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B23_SRCPORTIDENTITY_3]) << 32U);
       EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.clockIdentity |=
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B24_SRCPORTIDENTITY_4]) << 24U);
       EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.clockIdentity |=
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B25_SRCPORTIDENTITY_5]) << 16U);
       EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.clockIdentity |=
           (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B26_SRCPORTIDENTITY_6]) << 8U);
       EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.clockIdentity |=
           (uint64)((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B27_SRCPORTIDENTITY_7]);
       EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.portNumber =
           (uint16)((uint16) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8]);
       EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestPortId.portNumber |=
           (uint16)((uint16) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9]);
      }
#endif/* ETHTSYN_TIME_RECORDING == STD_ON */
#endif/* ETHTSYN_MAX_SLAVE != 0U */
    /* !LINKSTO EthTSyn.EB.TransparentClock_PdelayReqTransmission_FailedTransmission, 1 */
    TmpRet = EthTSyn_Inc02_TransmitSwtFrame(EthTSynCtrlIdx, BufIdx, TRUE, PortIdx, TRUE, PTPHeader.MsgLength, TRUE);

    if(E_OK == TmpRet)
    {
      /* Set source port identity to find corresponding Pdelay response and Pdelay response follow
         up frames. */
      SchM_Enter_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_0();
      TS_MemCpy(
                 &EthTSyn_Ctrl[EthTSynCtrlIdx].Pdelay_Tx_SrcPortIdentity[0U],
                 &PTPPayloadPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0],
                 ETHTSYN_SOURCEPORTIDENTITY_SIZE
               );
      SchM_Exit_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_0();

      EthTSyn_Ctrl[EthTSynCtrlIdx].PdelayReq_BufIdx = BufIdx;

      /* Set wait timeout for frame to transmit. */
      EthTSyn_Ctrl[EthTSynCtrlIdx].SendNextPdelayReqFrame_Timeout
        = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PdelayReqTxInterval;
#if(ETHTSYN_MAX_SLAVE != 0U)
      /* A pdelay calculation is started */
      TS_AtomicSetBit_8
      (
        &EthTSyn_Slave[EthTSynCtrlIdx].CalculatePdelay,
        ETHTSYN_BITPOS_PDELAYREQ_SENT
      );
#endif /* (ETHTSYN_MAX_SLAVE != 0U) */
      
    }
  }
  if(E_OK == TmpRet)
  {
    /* !LINKSTO EthTSyn.EB.SwitchPdelay.PdelayResp_WaitEgress, 1 */
    TS_AtomicAssign8
    (
      EthTSyn_SwitchPorts[PortIdx].PdelayState,
      ETHTSYN_PORT_PDELAY_WAIT_EGRESS_TS
    );
  }
  else
  {
    /* Reset the state machine if something happened with swt driver (TmpRet != E_OK)*/
    /* !LINKSTO EthTSyn.EB.EthSwt_Return_NotOk, 1 */
    EthTSyn_Inc02_ResetPortPdelayVars(PortIdx);
  }
  DBG_ETHTSYN_INC02_TRANSMITSWTPDELAYREQFRAMEPERPORT_EXIT(EthTSynCtrlIdx, PortIdx);
}

TS_MOD_PRIV_DEFN FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_TransmitSwtPdelayRespFramePerPort
(
  uint8 EthTSynCtrlIdx,
  uint8 PortIdx
)
{
  Std_ReturnType TmpRet;
  Eth_BufIdxType BufIdx = 0U;
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) PTPPayloadPtr;
  EthTSyn_PTPHeaderType PTPHeader;

  DBG_ETHTSYN_INC02_TRANSMITSWTPDELAYRESPFRAMEPERPORT_ENTRY(EthTSynCtrlIdx, PortIdx);

  /* Prepare PTP header. */
  PTPHeader.Flags = ETHTSYN_PTP_HEADER_FLAG_TWOSTEP;
  PTPHeader.Flags |= ETHTSYN_PTP_HEADER_FLAG_PTPTIMESCALE;
  PTPHeader.MsgType = ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_RESP;
  PTPHeader.Control = ETHTSYN_PTP_HEADER_CONTROL_PDELAY;

  /* Set sequence number. */
  /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00191_PdelayResp, 1 */
  TS_AtomicAssign16(
                     PTPHeader.SequNumber,
                     EthTSyn_SwitchPorts[PortIdx].Ingress_PdelayReqSequNumber
                   );
  /* Reserved value (See 802.1AS-2011 Ch 11.4.2.8) */
  PTPHeader.LogMsgInterval = ETHTSYN_PTP_HEADER_LOGMSGINTERVAL_RESERVED;

  PTPHeader.MsgLength = ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_PDELAY_RESP_PAYLOADSIZE;

  /* !LINKSTO EthTSyn.EB.SwitchPdelay.PdelayResp_Transmit, 1 */
  TmpRet = EthTSyn_Inc02_GetSwtTxBuffer
      (EthTSynCtrlIdx, PTPHeader.MsgLength, PortIdx, &BufIdx, &PTPPayloadPtr);

  /* !LINKSTO EthTSyn.EB.EthSwt_Return_NotOk, 1 */
  if(E_OK == TmpRet)
  {
    /* Fill PTP header */
    EthTSyn_FillPTPHeader(EthTSynCtrlIdx, &PTPHeader, &PTPPayloadPtr[0U]);

#if(ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT == STD_OFF)
    /* If the EthTSyn portid is configured update the portid bytes form the sourceportid field. */
    if(EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].EthTSynSourcePortId != 0xFFFFU)
    {
      /* 16 bit port number */
      PTPPayloadPtr[ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8] =
        (uint8)((uint16)(EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].EthTSynSourcePortId & ETHTSYN_16BIT_MASK_MSB) >> 8U);
      PTPPayloadPtr[ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9] =
        (uint8)((uint16)(EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].EthTSynSourcePortId & ETHTSYN_16BIT_MASK_LSB));
    }
#endif
    /* Set PTP payload */
    EthTSyn_TimeStampToArray(
                              &EthTSyn_SwitchPorts[PortIdx].IngPdelayFrameTimeStamp,
                              &PTPPayloadPtr[ETHTSYN_PTP_HEADER_SIZE]
                            );

    TS_MemCpy(
               &PTPPayloadPtr[ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_TIME_STAMP_SIZE],
               &EthTSyn_SwitchPorts[PortIdx].Pdelay_Rx_SrcPortIdentity[0U],
               ETHTSYN_SOURCEPORTIDENTITY_SIZE
             );
#if(ETHTSYN_TIME_RECORDING == STD_ON)
    if(EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].TimeValidationEnabled == TRUE)
    {
        StbM_UserDataType UserData;
        StbM_TimeStampType StbMTimeStamp;

        (void)StbM_BusGetCurrentTime
          (
            EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId, /* timeBaseId */
            &StbMTimeStamp, /* timeStampPtr */
            &EthTSyn_SwitchPorts[PortIdx].PdelayResponderTimeRecording.referenceLocalTimestamp, /* localTimePtr */
            &UserData /* userDataPtr */
          );

        EthTSyn_SwitchPorts[PortIdx].PdelayResponderTimeRecording.referenceGlobalTimestamp.
            nanoseconds = StbMTimeStamp.nanoseconds;
        EthTSyn_SwitchPorts[PortIdx].PdelayResponderTimeRecording.referenceGlobalTimestamp.
            seconds = StbMTimeStamp.seconds;
        EthTSyn_SwitchPorts[PortIdx].PdelayResponderTimeRecording.responsePortId.clockIdentity =
             (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0]) << 56U);
        EthTSyn_SwitchPorts[PortIdx].PdelayResponderTimeRecording.responsePortId.clockIdentity |=
             (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B21_SRCPORTIDENTITY_1]) << 48U);
        EthTSyn_SwitchPorts[PortIdx].PdelayResponderTimeRecording.responsePortId.clockIdentity |=
             (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B22_SRCPORTIDENTITY_2]) << 40U);
        EthTSyn_SwitchPorts[PortIdx].PdelayResponderTimeRecording.responsePortId.clockIdentity |=
             (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B23_SRCPORTIDENTITY_3]) << 32U);
        EthTSyn_SwitchPorts[PortIdx].PdelayResponderTimeRecording.responsePortId.clockIdentity |=
             (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B24_SRCPORTIDENTITY_4]) << 24U);
        EthTSyn_SwitchPorts[PortIdx].PdelayResponderTimeRecording.responsePortId.clockIdentity |=
             (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B25_SRCPORTIDENTITY_5]) << 16U);
        EthTSyn_SwitchPorts[PortIdx].PdelayResponderTimeRecording.responsePortId.clockIdentity |=
             (uint64)(((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B26_SRCPORTIDENTITY_6]) << 8U);
        EthTSyn_SwitchPorts[PortIdx].PdelayResponderTimeRecording.responsePortId.clockIdentity |=
             (uint64)((uint64) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B27_SRCPORTIDENTITY_7]);
        EthTSyn_SwitchPorts[PortIdx].PdelayResponderTimeRecording.responsePortId.portNumber =
             (uint16)((uint16) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8]);
        EthTSyn_SwitchPorts[PortIdx].PdelayResponderTimeRecording.responsePortId.portNumber |=
             (uint16)((uint16) PTPPayloadPtr[ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9]);
    }
#endif /* ETHTSYN_TIME_RECORDING == STD_ON */
    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00012, 1 */
    TmpRet = EthTSyn_Inc02_TransmitSwtFrame(EthTSynCtrlIdx, BufIdx, TRUE, PortIdx, TRUE, PTPHeader.MsgLength, TRUE);
  }
  if(E_OK == TmpRet)
  {
    /* !LINKSTO EthTSyn.EB.SwitchPdelay.PdelayResp_WaitEgress, 1 */
    /* !LINKSTO EthTSyn.Dsn.PortPdelayState_IDLE-WAIT_EGRESS, 1 */
    TS_AtomicAssign8
    (
      EthTSyn_SwitchPorts[PortIdx].PdelayState,
      ETHTSYN_PORT_PDELAY_WAIT_EGRESS_TS
    );
  }
  else
  {
    /* Reset the state machine if something happened with swt driver (TmpRet != E_OK)*/
    /* !LINKSTO EthTSyn.EB.EthSwt_Return_NotOk, 1 */
    EthTSyn_Inc02_ResetPortPdelayVars(PortIdx);
  }

  DBG_ETHTSYN_INC02_TRANSMITSWTPDELAYRESPFRAMEPERPORT_EXIT(EthTSynCtrlIdx, PortIdx);
}

TS_MOD_PRIV_DEFN FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_TransmitSwtPdelayRespFupFramePerPort
(
  uint8 EthTSynCtrlIdx,
  uint8 PortIdx
)
{
  Std_ReturnType TmpRet;
  Eth_BufIdxType BufIdx = 0U;
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) PTPPayloadPtr;
  EthTSyn_PTPHeaderType PTPHeader;

  DBG_ETHTSYN_INC02_TRANSMITSWTPDELAYRESPFUPFRAMEPERPORT_ENTRY(EthTSynCtrlIdx, PortIdx);

  /* Prepare PTP header. */
  PTPHeader.Flags = ETHTSYN_PTP_HEADER_NOFLAGS;
  PTPHeader.Flags |= ETHTSYN_PTP_HEADER_FLAG_PTPTIMESCALE;
  PTPHeader.MsgType = ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_FUP;
  PTPHeader.Control = ETHTSYN_PTP_HEADER_CONTROL_PDELAY;

  /* Set sequence number. */
  /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00191_PdelayRespFup, 1 */
  TS_AtomicAssign16(
                     PTPHeader.SequNumber,
                     EthTSyn_SwitchPorts[PortIdx].Ingress_PdelayReqSequNumber
                   );
  /* Reserved value (See 802.1AS-2011 Ch 11.4.2.8) */
  PTPHeader.LogMsgInterval = ETHTSYN_PTP_HEADER_LOGMSGINTERVAL_RESERVED;

  PTPHeader.MsgLength = ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_PDELAY_RESP_FUP_PAYLOADSIZE;

  /* !LINKSTO EthTSyn.EB.SwitchPdelay.PdelayRespFup_Transmit, 1 */
  TmpRet = EthTSyn_Inc02_GetSwtTxBuffer
      (EthTSynCtrlIdx, PTPHeader.MsgLength, PortIdx, &BufIdx, &PTPPayloadPtr);

  /* !LINKSTO EthTSyn.EB.EthSwt_Return_NotOk, 1 */
  if(E_OK == TmpRet)
  {
    /* Fill PTP header */
    EthTSyn_FillPTPHeader(EthTSynCtrlIdx, &PTPHeader, &PTPPayloadPtr[0U]);

#if(ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT == STD_OFF)
    /* If the EthTSyn portid is configured update the portid bytes form the sourceportid field. */
    if(EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].EthTSynSourcePortId != 0xFFFFU)
    {
      /* 16 bit port number */
      PTPPayloadPtr[ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8] =
        (uint8)((uint16)(EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].EthTSynSourcePortId & ETHTSYN_16BIT_MASK_MSB) >> 8U);
      PTPPayloadPtr[ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9] =
        (uint8)((uint16)(EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].EthTSynSourcePortId & ETHTSYN_16BIT_MASK_LSB));
    }
#endif

    /* Set PTP payload */
    EthTSyn_TimeStampToArray(
                              &EthTSyn_SwitchPorts[PortIdx].EgressPdelayFrameTimeStamp,
                              &PTPPayloadPtr[ETHTSYN_PTP_HEADER_SIZE]
                            );

    TS_MemCpy(
               &PTPPayloadPtr[ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_TIME_STAMP_SIZE],
               &EthTSyn_SwitchPorts[PortIdx].Pdelay_Rx_SrcPortIdentity[0U],
               ETHTSYN_SOURCEPORTIDENTITY_SIZE
             );

    /* Send frame: Ignore return value since no retry possible. */
    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00012, 1 */
    (void) EthTSyn_Inc02_TransmitSwtFrame(EthTSynCtrlIdx, BufIdx, TRUE, PortIdx, FALSE, PTPHeader.MsgLength, FALSE);
    /* Pass the responder data to the StbM. */
#if(ETHTSYN_TIME_RECORDING == STD_ON)
    if(EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].TimeValidationEnabled == TRUE)
    {
      /* Convert the egress timestamp. */
      /* !LINKSTO EthTSyn.ASR20-11.SWS_EthTSyn_00225, 1 */
      EthTSyn_Int01_ConvertEthTimeToVirtualTime(&EthTSyn_SwitchPorts[PortIdx].EgressPdelayFrameTimeStamp,
         &EthTSyn_SwitchPorts[PortIdx].PdelayResponderTimeRecording.responseOriginTimestamp);

      (void)StbM_EthSetPdelayResponderData
          (
              EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId,
              /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00220, 1 */
              &EthTSyn_SwitchPorts[PortIdx].PdelayResponderTimeRecording
          );
    }
#endif /* ETHTSYN_TIME_RECORDING == STD_ON */
  
  }

  /* Reset port pdelay variables */
  /* !LINKSTO EthTSyn.Dsn.PortPdelayState_WAIT_EGRESS-IDLE, 1 */
  EthTSyn_Inc02_ResetPortPdelayVars(PortIdx);

  DBG_ETHTSYN_INC02_TRANSMITSWTPDELAYRESPFUPFRAMEPERPORT_EXIT(EthTSynCtrlIdx, PortIdx);
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Inc02_GetSwtTxBuffer
(
  uint8 EthTSynCtrlIdx,
  uint16 PayloadLength,
  EthTSyn_IntPortIdxType IntPortIdx,
  P2VAR(Eth_BufIdxType, AUTOMATIC, ETHTSYN_APPL_DATA) BufIdxPtr,
  P2VAR(EthTSyn_PtrUint8, AUTOMATIC, ETHIF_APPL_DATA) BufPtr
)
{
  uint16 BufferSize = PayloadLength;
  Std_ReturnType RetVal = E_NOT_OK;
  BufReq_ReturnType RetVal_BufReq;
  EthSwt_MgmtInfoType MgmtInfo;

  DBG_ETHTSYN_INC02_GETSWTTXBUFFER_ENTRY(EthTSynCtrlIdx, PayloadLength, IntPortIdx, BufIdxPtr, BufPtr);

  MgmtInfo.SwitchIdx = (uint8)EthTSyn_RootCfgPtr->SwitchConfig[IntPortIdx].EthSwtIdx;
  MgmtInfo.SwitchPortIdx = (uint8)EthTSyn_RootCfgPtr->SwitchConfig[IntPortIdx].EthSwtPortIdx;

  /* !LINKSTO EthTSyn.EB.Master.FrameType, 1 */
  RetVal_BufReq = EthSwt_ProvideTxBuffer
  (
    EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx, /* EthIf CtrlIdx */
    ETHTSYN_PTP_FRAMETYPE, /* FrameType */
    EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].FramePriority, /* Priority */
    &MgmtInfo, /* EthSwt_MgmtInfoType */
    BufIdxPtr, /* BufIdxPtr */
    BufPtr, /* BufPtr */
    &BufferSize /* LenBytePtr */
  );

  /* !LINKSTO EthTSyn.EB.EthSwt_ProvideTxBuffer.NULL_PTR, 1 */
  if((BUFREQ_OK == RetVal_BufReq) && (*BufPtr != NULL_PTR))
  {
    RetVal = E_OK;
  }

  DBG_ETHTSYN_INC02_GETSWTTXBUFFER_EXIT(RetVal, EthTSynCtrlIdx, PayloadLength, IntPortIdx,
                                  BufIdxPtr, BufPtr);

  return RetVal;
}

TS_MOD_PRIV_DEFN FUNC(Std_ReturnType, ETHTSYN_CODE) EthTSyn_Inc02_TransmitSwtFrame
(
  uint8 EthTSynCtrlIdx,
  Eth_BufIdxType BufIdx,
  boolean RequestTimeStamp,
  EthTSyn_IntPortIdxType IntPortIdx,
  boolean ActivateTimeStamp,
  uint16 LenByte,
  boolean IsPdelayResp
)
{
  uint8 DestMacAddr[ETHTSYN_MAC_ADD_SIZE];
  Std_ReturnType RetVal;
  uint32 UniqueId;
  EthSwt_MgmtInfoType MgmtInfo;

  DBG_ETHTSYN_INC02_TRANSMITSWTFRAME_ENTRY(EthTSynCtrlIdx, BufIdx, RequestTimeStamp, IntPortIdx, ActivateTimeStamp, LenByte, IsPdelayResp);

  /* !LINKSTO EthTSyn.ASR44.ECUC_EthTSyn_00058_Conf, 1 */
  TS_MemCpy(DestMacAddr, EthTSyn_DestMacAddr, ETHTSYN_MAC_ADD_SIZE);

#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
  if(ActivateTimeStamp == TRUE)
  {
    /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00202, 1 */
    EthIf_EnableEgressTimeStamp
    (
      EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx,
      BufIdx
    );
  }
#else
  TS_PARAM_UNUSED(ActivateTimeStamp);
#endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */

  MgmtInfo.SwitchIdx = (uint8)EthTSyn_RootCfgPtr->SwitchConfig[IntPortIdx].EthSwtIdx;
  MgmtInfo.SwitchPortIdx = (uint8)EthTSyn_RootCfgPtr->SwitchConfig[IntPortIdx].EthSwtPortIdx;

  /* !LINKSTO EthTSyn.EB.Master.FrameType, 1 */
  RetVal = EthSwt_PtpTransmit
  (
    EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx, /* EthIf CtrlIdx */
    BufIdx, /* BufIdx */
    &MgmtInfo,
    &UniqueId,
    RequestTimeStamp, /* RequestTimeStamp */
    ETHTSYN_PTP_FRAMETYPE, /* FrameType */
    &DestMacAddr[0U], /* PhysAddrPtr */
    LenByte /* Length of frame in bytes */
  );

  if((E_OK == RetVal) && (RequestTimeStamp == TRUE))
  {
    if(IsPdelayResp == FALSE)
    {
#if(ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT == STD_OFF)
      EthTSyn_SwitchPorts[IntPortIdx].UniqueId = UniqueId;
#else
      uint8 Idx;
      for(
          Idx = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart;
          Idx < (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount +
              EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart);
          Idx++
         )
      {
        /* If EthTSynSendSyncFrameOnlyOnHostPort set to true the uniqueid returned
           from transmission of sync frame on host port will be added to all other
           non host ports. */
         EthTSyn_SwitchPorts[Idx].UniqueId = UniqueId;
      }
#endif/* (ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT == STD_OFF) */
    }
    else
    {
      /* Save the uniqueId to identify the egress timestamp */
      /* !LINKSTO EthTSyn.EB.SwitchPdelay.PdelayResp_SaveUniqueId, 1 */
      EthTSyn_SwitchPorts[IntPortIdx].PdelayUniqueId = UniqueId;
    }
  }

  DBG_ETHTSYN_INC02_TRANSMITSWTFRAME_EXIT(RetVal, EthTSynCtrlIdx, BufIdx, RequestTimeStamp, IntPortIdx, ActivateTimeStamp, LenByte, IsPdelayResp);

  return RetVal;
}

TS_MOD_PRIV_DEFN FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_SwtCorrFieldPTPHeader
(
  uint8 EthTSynCtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) BufPtr,
  uint8 PortIdx
)
{
  uint32 NsLow = 0U;
  Eth_TimeIntDiffType TimeCorrection;
  uint8 Idx;
  uint8 SwitchIdx = 0U;
  uint8 SwitchPortIdx   = 0U;

  EthTSyn_SubTimeStamps
  (
    &EthTSyn_SwitchPorts[PortIdx].SyncTimeStamp,
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF)
    &EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt].SyncTimeStamp,
#else
    &EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave].SyncTimeStamp,
#endif
    &TimeCorrection
  );
  if(
      (TimeCorrection.diff.nanoseconds <= ETHTSYN_NANOSEC_MAX)&&
      (TimeCorrection.diff.secondsHi == 0U)&&
      (TimeCorrection.sign == TRUE)
    )
  {
    /* conversion2ns */
    if(TimeCorrection.diff.seconds < 4U)
    {
      for(Idx = 0U; Idx < TimeCorrection.diff.seconds; Idx++)
      {
        NsLow += ETHTSYN_NANOSEC_MAX+1U;
      }
      NsLow += TimeCorrection.diff.nanoseconds;
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
      /* If a transparent clock is used, add the pathdelay to the correction field. */
      if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SimpleBridge == TRUE)
      {
        /* add to the correction field tha calculated pathDelay between the GTM and the switch. */
        NsLow += EthTSyn_Slave[EthTSynCtrlIdx].Pdelay;
      }
#endif
    }
    else
    {
      /* Only relevant for feature bridging */
    }
  }
  else
  {
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    ETHTSYN_PRECONDITION_ASSERT(TimeCorrection.sign == FALSE, ETHTSYN_INTERNAL_SVCID);
#endif /* ETHTSYN_DEV_ERROR_DETECT == STD_ON */
  }

  /* if TimeCorrection.diff.seconds >= 4seconds */
  BufPtr[ETHTSYN_PTP_HEADER_B8_CORRFIELD_0] = 0x00U;
  BufPtr[ETHTSYN_PTP_HEADER_B9_CORRFIELD_1] = 0x00U;
  /* ns integer part */
  BufPtr[ETHTSYN_PTP_HEADER_B10_CORRFIELD_2] = (uint8)((NsLow >> 24U) & 0xFFU);
  BufPtr[ETHTSYN_PTP_HEADER_B11_CORRFIELD_3] = (uint8)((NsLow >> 16U) & 0xFFU);
  BufPtr[ETHTSYN_PTP_HEADER_B12_CORRFIELD_4] = (uint8)((NsLow >>  8U) & 0xFFU);
  BufPtr[ETHTSYN_PTP_HEADER_B13_CORRFIELD_5] = (uint8)((NsLow       ) & 0xFFU);

  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SwtPortIdxInCorrField)
  {
    SwitchIdx       = (uint8)EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].EthSwtIdx;
    SwitchPortIdx   = (uint8)EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].EthSwtPortIdx;
  }
  /* ns fractional part */

  /* !LINKSTO EthTSyn.EB.SwitchPortIdx.StoredInCorrectionField, 1 */
  BufPtr[ETHTSYN_PTP_HEADER_B14_CORRFIELD_6] = SwitchIdx;
  BufPtr[ETHTSYN_PTP_HEADER_B15_CORRFIELD_7] = SwitchPortIdx;
#if(ETHTSYN_TIME_RECORDING == STD_ON) 
  if(EthTSyn_RootCfgPtr->SwitchConfig[PortIdx].TimeValidationEnabled == TRUE)
  {
    /* If timevalidation enabled, save the correction */
    EthTSyn_SwitchPorts[PortIdx].SyncMasterTimeRecording.correctionField = NsLow;
  }
#endif /* ETHTSYN_TIME_RECORDING == STD_ON */
}

TS_MOD_PRIV_DEFN FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_SwtSynStateHandling
(
  uint8 EthTSynCtrlIdx
)
{
  EthTSyn_CtrlSyncTxStateType SyncTxState;
  TS_AtomicAssign8
  (
    SyncTxState,
    EthTSyn_Ctrl[EthTSynCtrlIdx].SyncTxState
  );

  switch(SyncTxState)
  {
    case ETHTSYN_CTRL_SYN_TX_STATE_IDLE:
    case ETHTSYN_CTRL_SYN_TX_STATE_WAIT_FOR_ETH_EGRESS_TS:
    {
      /* Nothing to do in main function. */
      break;
    }
    case ETHTSYN_CTRL_SYN_TX_STATE_READY_FOR_SYN_TO_PORT:
    {
      uint8 Idx;
      ETHTSYN_SET_TXFLAG
       (&EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit, ETHTSYN_BITPOS_TX_SWT, TRUE);

      for(
            Idx = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart;
            Idx < (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount +
                  EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart);
            Idx++
         )
      {
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
        if((Idx != EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt) &&
           (Idx != EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave)
          )
#else
        if(Idx != EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt)
#endif
        {
          TS_AtomicSetBit_8
          (
            &EthTSyn_SwitchPorts[Idx].Frame2Transmit,
             ETHTSYN_BITPOS_TX_SYNC
          );
        }
      }

      /* No retry performed if sync Tx is not okay for a single port. */
      /* !LINKSTO EthTSyn.Dsn.CtrlSynTxState_READY_SYN_TO_PORT-WAIT_SWT_INGRESS, 1 */
      /* !LINKSTO EthTSyn.Dsn.CtrlSynBridge_READY-WAIT_INGRESS, 1 */
      TS_AtomicAssign8
      (
        EthTSyn_Ctrl[EthTSynCtrlIdx].SyncTxState,
        ETHTSYN_CTRL_SYN_TX_STATE_WAIT_FOR_SWT_INGRESS_TS
      );
      break;
    }
    case ETHTSYN_CTRL_SYN_TX_STATE_WAIT_FOR_SWT_INGRESS_TS:
    {
      EthTSyn_PortSyncTxStateType PortSynTxState;

      TS_AtomicAssign8
      (
        PortSynTxState,
        EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt].SyncTxState
      );

      if(ETHTSYN_PORT_SYN_TX_STATE_VALID_INGRESS_TS == PortSynTxState)
      {
        /* !LINKSTO EthTSyn.Dsn.HostPortSynTxState_VALID_INGRESS-IDLE, 1 */
        TS_AtomicAssign8
        (
          EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt].SyncTxState,
          ETHTSYN_PORT_SYN_TX_STATE_IDLE
        );
        /* !LINKSTO EthTSyn.Dsn.CtrlSynTxState_VALID_INGRESS-READY_FUP, 1 */
        /* !LINKSTO EthTSyn.Dsn.HostPortSynTxState_VALID_INGRESS-IDLE, 1 */
        /* !LINKSTO EthTSyn.Dsn.CtrlSynBridge_VALID_INGRESS-READY_FUP, 1 */
        TS_AtomicAssign8
        (
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncTxState,
          ETHTSYN_CTRL_SYN_TX_STATE_READY_FOR_FUP
        );
      }
      break;
    }
    case ETHTSYN_CTRL_SYN_TX_STATE_READY_FOR_FUP:
    {
      EthTSyn_PortSyncTxStateType Idx;
      EthTSyn_PortSyncTxStateType PortSynTxState;

      for(
            Idx = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart + 1U;
            Idx < (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount +
                  EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart);
            Idx++
         )
      {
        TS_AtomicAssign8
        (
          PortSynTxState,
          EthTSyn_SwitchPorts[Idx].SyncTxState
        );
        /* !LINKSTO EthTSyn.Dsn.CtrlSynTxState_CTRL_READY_FUP-PORT_READY_FUP, 1 */
        /* !LINKSTO EthTSyn.EB.BoundaryClock_TransmitSyncFUpOnMasterPorts, 1 */
        if(PortSynTxState == ETHTSYN_PORT_SYN_TX_STATE_READY_FOR_FUP)
        {
          ETHTSYN_SET_TXFLAG
            (&EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit, ETHTSYN_BITPOS_TX_SWT, TRUE);
          TS_AtomicSetBit_8
          (
            &EthTSyn_SwitchPorts[Idx].Frame2Transmit,
            ETHTSYN_BITPOS_TX_FUP
          );
        }
      }
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
      /* If the egress timestamp on the host port was received, we can proceed with the synchronization */
      if(EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt].SyncTxState == ETHTSYN_PORT_READY_FOR_TIME_SYNC)
      {
        uint8 PortIdx;
        uint8 PortIdleCounter = 0U;
        
        /* Sync the slave */
        /* !LINKSTO EthTSyn.EB.TransparentClock_Synchronize_EgressHostPortAfterSyncFUp, 1 */
        /* !LINKSTO EthTSyn.EB.TransparentClock_Synchronize_EgressHostPortAfterSyncFUpFW, 1 */
        EthTSyn_Inc02_SynchronizeBridge(&EthTSyn_Slave[EthTSynCtrlIdx].OriginTimeStampCorr, EthTSynCtrlIdx);
        
        /* Sync performed, don't retry on the mainfunction, reset the state for the host port */
        TS_AtomicAssign8
        (
            EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt].SyncTxState,
            ETHTSYN_PORT_SYN_TX_STATE_IDLE
        );
        
        /* If the timestamp for the host port was provided after the transmission of the fup frame, the ctrl SM must be reset */
        for(
             PortIdx =  EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart;
             PortIdx < (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount
                 +  EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart);
             PortIdx++
           )
        {
          TS_AtomicAssign8
          (
              PortSynTxState,
              EthTSyn_SwitchPorts[PortIdx].SyncTxState
          );

          if(ETHTSYN_PORT_SYN_TX_STATE_IDLE == PortSynTxState)
          {
            PortIdleCounter++;
          }
        }

        if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount == PortIdleCounter)
        {
          /* !LINKSTO EthTSyn.Dsn.CtrlSynTxState_PORT_READY_FUP-IDLE, 1 */
          /* !LINKSTO EthTSyn.Dsn.CtrlSynBridge_PORT_READY-IDLE, 1 */
          EthTSyn_ResetSynTxStateMachine(EthTSynCtrlIdx);
        }
      }
#endif /* ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON */
      break;
    }
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
    case ETHTSYN_CTRL_SYN_TX_STATE_READY_FOR_BRIDGE_SYNC:
    {
      /* If the egress timestamp on the host port was received, we can proceed with the synchronization */
      if(EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt].SyncTxState == ETHTSYN_PORT_READY_FOR_TIME_SYNC)
      {
        /* Sync the slave */
        EthTSyn_Inc02_SynchronizeBridge(&EthTSyn_Slave[EthTSynCtrlIdx].OriginTimeStampCorr, EthTSynCtrlIdx);
        EthTSyn_ResetSynTxStateMachine(EthTSynCtrlIdx);
      }
      break;
    }
#endif /* ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON */
    /* CHECK: NOPARSE */
    default:
    {
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
      ETHTSYN_UNREACHABLE_CODE_ASSERT(ETHTSYN_INTERNAL_SVCID);
#endif
      break;
    }
  /* CHECK: PARSE */
  }
}

TS_MOD_PRIV_DEFN FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_TransmitSwtFrameWithDebounce
(
  uint8 EthTSynCtrlIdx,
  uint8 PortIdx
)
{
  if((EthTSyn_SwitchPorts[PortIdx].Frame2Transmit & ETHTSYN_DEBOUNCE_SWT_TX_ON_HOST_MASK) != 0U)
  {
    /* !LINKSTO EthTSyn.ASR151.PRS_TS_00186, 1 */
    EthTSyn_Inc02_TransmitSwtOnHostPortSyncFrame(EthTSynCtrlIdx);

    /* clear the flag */
    TS_AtomicClearBit_8
    (
      &EthTSyn_SwitchPorts[PortIdx].Frame2Transmit,
      ETHTSYN_BITPOS_TX_SYNC_ON_HOST
    );
  }
  else if((EthTSyn_SwitchPorts[PortIdx].Frame2Transmit & ETHTSYN_DEBOUNCE_TX_SYNC_MASK) != 0U)
  {
    EthTSyn_Inc02_TransmitSwtSyncFramePerPort(EthTSynCtrlIdx, PortIdx);

    /* clear the flag */
    TS_AtomicClearBit_8
    (
      &EthTSyn_SwitchPorts[PortIdx].Frame2Transmit,
      ETHTSYN_BITPOS_TX_SYNC
    );
  }
  /* CHECK: NOPARSE */
  else if((EthTSyn_SwitchPorts[PortIdx].Frame2Transmit & ETHTSYN_DEBOUNCE_TX_FUP_MASK) != 0U)
  {
    EthTSyn_Inc02_TransmitSyncSwtFUpFramePerPort(EthTSynCtrlIdx, PortIdx);
    /* clear the flag */
    TS_AtomicClearBit_8
    (
      &EthTSyn_SwitchPorts[PortIdx].Frame2Transmit,
      ETHTSYN_BITPOS_TX_FUP
    );
  }
  else if((EthTSyn_SwitchPorts[PortIdx].Frame2Transmit & ETHTSYN_DEBOUNCE_PDELAYREQ_MASK) != 0U)
  {
    EthTSyn_Inc02_TransmitSwtPdelayReqFramePerPort(EthTSynCtrlIdx, PortIdx);
    /* clear the flag */
    TS_AtomicClearBit_8
    (
      &EthTSyn_SwitchPorts[PortIdx].Frame2Transmit,
      ETHTSYN_BITPOS_PDELAYREQ
    );
  }
  else if((EthTSyn_SwitchPorts[PortIdx].Frame2Transmit & ETHTSYN_DEBOUNCE_PDELAYRESP_MASK) != 0U)
  {
    EthTSyn_Inc02_TransmitSwtPdelayRespFramePerPort(EthTSynCtrlIdx, PortIdx);
    /* clear the flag */
    TS_AtomicClearBit_8
    (
      &EthTSyn_SwitchPorts[PortIdx].Frame2Transmit,
      ETHTSYN_BITPOS_PDELAYRESP
    );
  }
  else if((EthTSyn_SwitchPorts[PortIdx].Frame2Transmit & ETHTSYN_DEBOUNCE_PDELAYRESP_FUP_MASK) != 0U)
  {
    EthTSyn_Inc02_TransmitSwtPdelayRespFupFramePerPort(EthTSynCtrlIdx, PortIdx);
    /* clear the flag */
    TS_AtomicClearBit_8
    (
      &EthTSyn_SwitchPorts[PortIdx].Frame2Transmit,
      ETHTSYN_BITPOS_PDELAYRESP_FUP
    );
  }
  else
  {
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)
    ETHTSYN_UNREACHABLE_CODE_ASSERT(ETHTSYN_INTERNAL_SVCID);
    /* nothing to do */
#endif
  }
  /* CHECK: PARSE */
}

TS_MOD_PRIV_DEFN FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_ResetPortPdelayVars
(
  uint8 PortIdx
)
{

  TS_AtomicAssign8
  (
    EthTSyn_SwitchPorts[PortIdx].PdelayState,
    ETHTSYN_PORT_PDELAY_STATE_IDLE
  );
  EthTSyn_SwitchPorts[PortIdx].EgressPdelayFrameTimeStamp.nanoseconds = 0U;
  EthTSyn_SwitchPorts[PortIdx].EgressPdelayFrameTimeStamp.seconds = 0U;
  EthTSyn_SwitchPorts[PortIdx].EgressPdelayFrameTimeStamp.secondsHi = 0U;

  EthTSyn_SwitchPorts[PortIdx].IngPdelayFrameTimeStamp.nanoseconds = 0U;
  EthTSyn_SwitchPorts[PortIdx].IngPdelayFrameTimeStamp.seconds = 0U;
  EthTSyn_SwitchPorts[PortIdx].IngPdelayFrameTimeStamp.secondsHi = 0U;
  EthTSyn_SwitchPorts[PortIdx].Ingress_PdelayReqSequNumber = 0U;
  EthTSyn_SwitchPorts[PortIdx].PdelayUniqueId = ETHTSYN_UNIQUE_ID_INVALID;
  TS_MemBZero(
               &EthTSyn_SwitchPorts[PortIdx].Pdelay_Rx_SrcPortIdentity[0U],
               ETHTSYN_SOURCEPORTIDENTITY_SIZE
             );
}

#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
TS_MOD_PRIV_DEFN FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_BridgeCalculateCrcTime1
(
  uint8 EthTSynCtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) Crc_Time_1
)
{
  uint8 Crc_1;
  uint8 SeqNr = (DataPtr[ETHTSYN_PTP_HEADER_B31_SEQUNUMBER_0]) % 16U;

  /* Calculate CRC_Time_1 based using the value of CRC_Time_Flags */
  Crc_1 = Crc_CalculateCRC8H2F(&EthTSyn_Ctrl[EthTSynCtrlIdx].Crc_Time_Flags, 1U, 0U, TRUE);

  /* Calculate CRC_Time_1 based using the length of the message if CRC_Time_Flags has Bitmask 0x01 */
  if((DataPtr[ETHTSYN_FUP_AUTOSAR_SUBTLV_START_ADDR + ETHTSYN_FUP_SUBTLV_TIME_B2_TIME_FLAGS] & ETHTSYN_MESSAGE_LENGTH_BIT_MASK) != 0U)
  {
    Crc_CalculateCRC8H2F(&DataPtr[ETHTSYN_PTP_HEADER_B2_MSGLENGTH_1], 2U, Crc_1, FALSE);
  }

  /* Calculate CRC_Time_1 based using the correctionField */
  Crc_1 = Crc_CalculateCRC8H2F(&DataPtr[ETHTSYN_PTP_HEADER_B8_CORRFIELD_0], 8U, Crc_1, FALSE);

  /* Calculate CRC_Time_1 based using the CRCsequenceId if CRC_Time_Flags has Bitmask 0x10 */
  if((DataPtr[ETHTSYN_FUP_AUTOSAR_SUBTLV_START_ADDR + ETHTSYN_FUP_SUBTLV_TIME_B2_TIME_FLAGS] & ETHTSYN_SEQUENCE_ID_BIT_MASK) != 0U)
  {
    Crc_1 = Crc_CalculateCRC8H2F(&DataPtr[ETHTSYN_PTP_HEADER_B30_SEQUNUMBER_1], 2U, Crc_1, FALSE);
  }

  /* Calculate CRC_Time_1 based using the DataId */
  Crc_1 = Crc_CalculateCRC8H2F
            (
              &EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].DataIdList[SeqNr],
              1U, Crc_1, FALSE
            );

  *Crc_Time_1 = Crc_1;
}

#endif /* (ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON) */
TS_MOD_PRIV_DEFN FUNC(boolean, ETHTSYN_CODE) EthTSyn_Inc02_ProcessRxSwtMsg
(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHTSYN_APPL_DATA) MgmtInfoPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) IngressTimeStampPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint8 EthTSynCtrlIdx,
  uint16 Len,
  uint32 UniqueId
)
{
  boolean TimeStampRequired = FALSE;
  
  /* Extract message type from PTP header. */
  const uint8 MsgType = DataPtr[ETHTSYN_PTP_HEADER_B0_LN_MSGTYPE] & ETHTSYN_8BIT_MASK_LN;
  switch(MsgType)
  {
    case ETHTSYN_PTP_HEADER_MSGTYPE_SYNC:
    {
      /* !LINKSTO EthTSyn.EB.EthSwt_ReceiveSyncFrameOnSlavePort, 1 */
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
      /* !LINKSTO EthTSyn.EB.EthSwtRx_Sync_IncorrectLength, 1 */
      if(Len == ETHTSYN_SYNC_FRAME_SIZE)
      {
        /* !LINKSTO EthTSyn.EB.EthSwtPtpRxIndication_Incorrect_MgmtInfo, 1 */
        /* Sync frame can be received only on the slave port */
        if(
            (
              EthTSyn_RootCfgPtr->SwitchConfig[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave].
              EthSwtIdx == MgmtInfoPtr->SwitchIdx
            )
          &&
            (
              EthTSyn_RootCfgPtr->SwitchConfig[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave].
              EthSwtPortIdx == MgmtInfoPtr->SwitchPortIdx
            )
         )
         {
#if((ETHTSYN_DEV_ERROR_DETECT == STD_ON) && (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON))
          Std_ReturnType TmpRet = E_NOT_OK;
          /* The validation is done if the simple bridge is configured to validate the sync frame or
             if a boundary clock is used. */
          if((EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SimpleBridgeValidateSync == TRUE) ||
            (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SimpleBridge == FALSE))
          {
            if(EthTSyn_Slave[EthTSynCtrlIdx].SaveSrcPortId == TRUE)
            {
              /* When the first sync frame is received, save the sourceportid, in order to be able to detect
                 parallel masters. */
              TS_MemCpy(
                 &EthTSyn_Slave[EthTSynCtrlIdx].Sync_Rx_SrcPortIdentity[0U],
                 &DataPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0],
                 ETHTSYN_SOURCEPORTIDENTITY_SIZE
               );
               EthTSyn_Slave[EthTSynCtrlIdx].SaveSrcPortId = FALSE;
            }

            /* Check source port identity */
            TmpRet = TS_MemCmp(
                      &EthTSyn_Slave[EthTSynCtrlIdx].Sync_Rx_SrcPortIdentity[0U],
                      &DataPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0],
                      ETHTSYN_SOURCEPORTIDENTITY_SIZE
                    );
          }
          else
          {
            TmpRet = E_OK;
          }
          if(TmpRet == E_OK)
          {
#endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) && (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON) */
             /* If a new sync frame is received, reset the state and the previous values.*/
             EthTSyn_ResetSynTxStateMachine(EthTSynCtrlIdx);
             /* If a sync frame is received on the slave port we can proceed with processing the frame */
             TimeStampRequired = EthTSyn_Inc02_ProcessRxSyncSwtMsg(IngressTimeStampPtr, DataPtr, EthTSynCtrlIdx, UniqueId);
#if((ETHTSYN_DEV_ERROR_DETECT == STD_ON) && (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON))
          }
          else
          {
            /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00146, 1 */
            /* !LINKSTO EthTSyn.EB.TransparentClock_ValidateSync_ScrPortId, 1 */
            ETHTSYN_DET_REPORT_ERROR
            (
              ETHTSYN_SID_ETHSWTPTPRXINDICATION,
              ETHTSYN_E_TSCONFLICT
            );
          }
#endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) && (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON) */
         }
      }
      /* If a sync message is received on the TAB with GTM as Mgm CPU, report an error if configured */
#elif(ETHTSYN_PROD_ERR_HANDLING_UNEXPECTED_SYNC == TS_PROD_ERR_REP_TO_DEM)
      /* !LINKSTO EthTSyn.EB.Diagnostic_UnexpectedSync_DEM, 1 */
      Dem_ReportErrorStatus
      (
        EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EventIdUnexpectedSync,
        DEM_EVENT_STATUS_FAILED
      );
#endif /* ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON */

#if((ETHTSYN_DEV_ERROR_DETECT == STD_ON) && (ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF))
     /* Report errors (if configured) if a sync frame is received on a time aware with GTM as MGM cpu */
#if(ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON)
      /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00145, 1 */
      ETHTSYN_DET_REPORT_ERROR
      (
        ETHTSYN_SID_ETHSWTPTPRXINDICATION,
        ETHTSYN_E_TMCONFLICT
      );
#endif /* (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON) */

#if(ETHTSYN_PROD_ERR_HANDLING_UNEXPECTED_SYNC == TS_PROD_ERR_REP_TO_DET)
      /* !LINKSTO EthTSyn.EB.Diagnostic_UnexpectedSync_DET, 1 */
      ETHTSYN_DET_REPORT_ERROR
      (
        ETHTSYN_SID_ETHSWTPTPRXINDICATION,
        ETHTSYN_E_TMCONFLICT_CONFIGURED
      );
#endif /* (ETHTSYN_PROD_ERR_HANDLING_UNEXPECTED_SYNC == TS_PROD_ERR_REP_TO_DET) */

#endif /* (ETHTSYN_DEV_ERROR_DETECT == STD_ON) && (ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON) */
      break;
    }
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
    case ETHTSYN_PTP_HEADER_MSGTYPE_FUP:
    {
      /* !LINKSTO EthTSyn.EB.EthSwtRx_SyncFUp_IncorrectLength, 1 */
      if((ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_SYNC_FUP_PAYLOADSIZE) <= Len)
      {
        /* !LINKSTO EthTSyn.EB.EthSwtPtpRxIndication_Incorrect_MgmtInfo, 1 */
        /* SyncFup frame can be received only on the slave port */
        if(
            (
              EthTSyn_RootCfgPtr->SwitchConfig[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave].
              EthSwtIdx == MgmtInfoPtr->SwitchIdx
            )
          &&
            (
              EthTSyn_RootCfgPtr->SwitchConfig[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave].
              EthSwtPortIdx == MgmtInfoPtr->SwitchPortIdx
            )
         )
         {
            /* If a Fup frame is received on the slave port we can proceed with processing the frame */
            EthTSyn_Inc02_ProcessRxFupSyncSwtMsg(DataPtr, EthTSynCtrlIdx, Len);
            /* No timestamp is required for the Fup frame. */
            TimeStampRequired = FALSE;
         }
        }
      break;
    }
    
    case ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_RESP:
    {
      /* !LINKSTO EthTSyn.EB.EthSwtRx_PdelayResp_IncorrectLength, 1 */
      if((ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_PDELAY_RESP_PAYLOADSIZE) <= Len)
      {
        /* !LINKSTO EthTSyn.EB.TransparentClock_InvalidPdelayRespMgmtInfo, 1 */
        if(
            (
              EthTSyn_RootCfgPtr->SwitchConfig[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave].
              EthSwtIdx == MgmtInfoPtr->SwitchIdx
            )
          &&
            (
              EthTSyn_RootCfgPtr->SwitchConfig[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave].
              EthSwtPortIdx == MgmtInfoPtr->SwitchPortIdx
            )
          &&
            (
              EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PdelayReqTxInterval != 0U
            )
         )
          {
            EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t4.nanoseconds =  IngressTimeStampPtr->nanoseconds;
            EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t4.seconds =  IngressTimeStampPtr->seconds;
            EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t4.secondsHi =  IngressTimeStampPtr->secondsHi;
            EthTSyn_ProcessRxPdelayRespFrame(EthTSynCtrlIdx, DataPtr);
#if(ETHTSYN_TIME_RECORDING == STD_ON)
            if(EthTSyn_RootCfgPtr->SwitchConfig[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave].TimeValidationEnabled == TRUE)
            {
              /* requestReceiptTimeStamp as received in pdelay_Response message */
              EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestReceiptTimestamp.
                  seconds = EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t2.seconds;
              EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.requestReceiptTimestamp.
                  nanoseconds = EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t2.nanoseconds;
              
              /* Converted value of the responseReceiptTimestamp */
              (void)EthTSyn_Int01_ConvertEthTimeToVirtualTime
                  (
                      &EthTSyn_Slave[EthTSynCtrlIdx].Pdelay_TimeStamp_t4,
                      &EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responseReceiptTimestamp
                  );

              /* Save the sourcePortId of sent Pdelay_Req frame*/
              EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.clockIdentity
                     = (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0]) << 56U);
              EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.clockIdentity
                    |= (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B21_SRCPORTIDENTITY_1]) << 48U);
              EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.clockIdentity
                    |= (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B22_SRCPORTIDENTITY_2]) << 40U);
              EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.clockIdentity
                    |= (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B23_SRCPORTIDENTITY_3]) << 32U);
              EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.clockIdentity
                    |= (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B24_SRCPORTIDENTITY_4]) << 24U);
              EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.clockIdentity
                    |= (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B25_SRCPORTIDENTITY_5]) << 16U);
              EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.clockIdentity
                    |= (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B26_SRCPORTIDENTITY_6]) << 8U);
              EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.clockIdentity
                    |= (uint64)((uint64) DataPtr[ETHTSYN_PTP_HEADER_B27_SRCPORTIDENTITY_7]);
              EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.portNumber =
                   (uint16)((uint16) DataPtr[ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8]);
              EthTSyn_Slave[EthTSynCtrlIdx].PdelayInitiatorTimeRecording.responsePortId.portNumber |=
                   (uint16)((uint16) DataPtr[ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9]);

            }
#endif
            /* Update PdelayRespAndRespFollowUpTimeout(waiting for Pdelay_Resp) */
            /* !LINKSTO EthTSyn.ASR151.PRS_TS_00164.PdelayRespTimeout, 1 */
            EthTSyn_Slave[EthTSynCtrlIdx].PdelayFollowUpTimeout
              = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PdelayRespAndRespFollowUpTimeout;
          }
      }
      break;
    }
    case ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_FUP:
    {
      /* !LINKSTO EthTSyn.EB.EthSwtRx_PdelayRespFUp_IncorrectLength, 1 */
      if((ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_PDELAY_RESP_FUP_PAYLOADSIZE) <= Len)
      {
        /* !LINKSTO EthTSyn.EB.TransparentClock_InvalidPdelayRespFUpMgmtInfo, 1 */
        if(
            (
              EthTSyn_RootCfgPtr->SwitchConfig[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave].
              EthSwtIdx == MgmtInfoPtr->SwitchIdx
            )
          &&
            (
              EthTSyn_RootCfgPtr->SwitchConfig[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave].
              EthSwtPortIdx == MgmtInfoPtr->SwitchPortIdx
            )
          &&
            (
              EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PdelayReqTxInterval != 0U
            )
         )
        {
          /* !LINKSTO EthTSyn.EB.Master.RxIndication, 1 */
          EthTSyn_ProcessRxPdelayRespFUpFrame(EthTSynCtrlIdx, DataPtr);
        }
      }
      break;
    }
#endif/* ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON */
    case ETHTSYN_PTP_HEADER_MSGTYPE_PDELAY_REQ:
    {
      /* !LINKSTO EthTSyn.EB.EthSwtRx_PdelayReq_IncorrectLength, 1 */
      if((ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_PDELAY_REQ_PAYLOADSIZE) <= Len)
      {
        EthTSyn_Inc02_ProcessRxPdelayReqSwtMsg(MgmtInfoPtr, IngressTimeStampPtr, DataPtr, EthTSynCtrlIdx, Len);
        TS_PARAM_UNUSED(UniqueId);
        TimeStampRequired = FALSE;
      }
      break;
    }
    
    default:
    {
      /* !LINKSTO EthTSyn.EB.EthSwtPtpRxIndication_UnknownFrameType, 1 */
      /* Unknown message type: Ignore */
      break;
    }
  }
  return TimeStampRequired;
}
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
STATIC FUNC(boolean, ETHTSYN_CODE) EthTSyn_Inc02_ProcessRxSyncSwtMsg
(
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) IngressTimeStampPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint8 EthTSynCtrlIdx,
  uint32 UniqueId
)
{
  uint8 Idx = 0U;
  boolean TimeStampRequired = FALSE;
  /* Save the ingress timestamp for the slave port */
  /* Save the ingress timestamp in order to calculate the origintimestamp. */
  EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave].
                     SyncTimeStamp.nanoseconds = IngressTimeStampPtr->nanoseconds;
  EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave].
                     SyncTimeStamp.seconds = IngressTimeStampPtr->seconds;
  EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave].
                     SyncTimeStamp.secondsHi = IngressTimeStampPtr->secondsHi;

  /* Process received sync frame */
  EthTSyn_Inc02_ProcessRxSyncFrame(DataPtr, EthTSynCtrlIdx);
  /* The sync message was successfully processed*/
  if(EthTSyn_Slave[EthTSynCtrlIdx].Sync_SyncFrameReceived == TRUE)
  {
    /* If a transparent clock is used, and the sync message was successfully processed */
    if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SimpleBridge == TRUE)
    {
      /* EthSwt will handle the fowarding of the frames */
      if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SimpleBridgeTSynSendSync == FALSE)
      {
        /* Save the uniqueid for all master ports for the current timedomain. */
        for(
            Idx = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart + 2U;
            Idx < (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount +
                EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart);
            Idx++
           )
        {
          /* Use the uniqueid to identify the egress timestamps for the master ports. */
          EthTSyn_SwitchPorts[Idx].UniqueId = UniqueId;
          /* If the sync was received, the EthSwt already transmitted the sync to the
             master ports, therefore we can wait for the egress timestamps.*/
          TS_AtomicAssign8
          (
              EthTSyn_SwitchPorts[Idx].SyncTxState,
              ETHTSYN_PORT_SYN_TX_STATE_WAIT_EGRESS_TS
          );
        }
        /* Wait for the egress timestamps. */
        /* !LINKSTO EthTSyn.Dsn.CtrlSynBridge_IDLE-READY, 1 */
        TS_AtomicAssign8
        (
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncTxState,
          ETHTSYN_CTRL_SYN_TX_STATE_WAIT_FOR_ETH_EGRESS_TS
        );
      }
      else
      {
        /* !LINKSTO EthTSyn.EB.TransparentClock_EthTSynSendSync, 1 */
        TS_MemCpy(&EthTSyn_Ctrl[EthTSynCtrlIdx].ReceivedSyncFrame[0], &DataPtr[0], ETHTSYN_SYNC_FRAME_SIZE);
        if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].GlobalTimeDebounceTime != 0U)
        {

#if(ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT == STD_OFF)
          uint8 PortIdx;
         /* Transmit sync frames to the connected slaves on the mainfunction */
          for(
              PortIdx = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart + 2U;
              PortIdx < (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount +
                  EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart);
              PortIdx++
             )
          {
            TS_AtomicSetBit_8
            (
              &EthTSyn_SwitchPorts[PortIdx].Frame2Transmit,
              ETHTSYN_BITPOS_TX_SYNC
            );
          }
          
#else
          /* !LINKSTO EthTSyn.EB.TransparentClock_SyncOnHost, 1 */
          TS_AtomicSetBit_8
          (
            &EthTSyn_SwitchPorts
               [EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt].Frame2Transmit,
            ETHTSYN_BITPOS_TX_SYNC_ON_HOST
          );
#endif/* ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT == STD_OFF */
         /* The transmission will be done on the mainfunction */
         ETHTSYN_SET_TXFLAG
            (&EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit, ETHTSYN_BITPOS_TX_SWT, TRUE);
        }
        else
        {
#if(ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT == STD_OFF)
          /* If the debounce is 0 transmit the sync */
          for(
              Idx = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart + 2U;
              Idx < (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount +
                  EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart);
              Idx++
             )
          {
            /* Transmit the sync to the connected slave ports */
            EthTSyn_Inc02_TransmitSwtSyncFramePerPort(EthTSynCtrlIdx, Idx);
          }
#else
          EthTSyn_Inc02_TransmitSwtOnHostPortSyncFrame(EthTSynCtrlIdx);
#endif/* ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT == STD_OFF */
        }
      }
      /* If the sync is needed and the sync frame was processed successfully proceed with the sync */
      if(
          (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SynchronizeSimpleBridge == TRUE) &&
          (EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt].SyncTxState == ETHTSYN_PORT_SYN_TX_STATE_WAIT_EGRESS_TS)
        )
      {
        /* Use the uniqueid to identify the egress timestamps for the host port. */
        EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt].UniqueId = UniqueId;
        /* If synchronization of the simplebridge is configured, the egress
           timestamp T5(figure 5.7, PRS 1.5.1) is needed.*/
        TimeStampRequired = TRUE;
      }
    }
    else
    {
      /* Boundary clock */
      /* Ready to wait the egress timestamp from the host port. */
      TS_AtomicAssign8
      (
        EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt].SyncTxState,
        ETHTSYN_PORT_SYN_TX_STATE_WAIT_EGRESS_TS
      );
      /* Use the uniqueid to identify the egress timestamps for the host port. */
      EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt].UniqueId = UniqueId;
      TimeStampRequired = TRUE;
    }
  }
  return TimeStampRequired;
}

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_ProcessRxFupSyncSwtMsg
(
  P2VAR(Eth_DataType, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint8 EthTSynCtrlIdx,
  uint16 Len
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  const uint16 RxSequNumber = EthTSyn_GetPTPHeaderSequNumber(DataPtr);
  /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00196, 1 */
  if(
      (RxSequNumber == EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber) &&
      (EthTSyn_Slave[EthTSynCtrlIdx].Sync_SyncFrameReceived)
     )
  {
#if(ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF)

    /* If an EthTSynTimeDomain is configured as a transparent clock, CRC configuration won't be used
      (data from received message shall be used when validating the SyncFUp message) -
      - If received SyncFUp message lenght is greater than 76, the message contains AUTOSAR Sub-TLV. */
    if(Len > (ETHTSYN_PTP_HEADER_SIZE + ETHTSYN_PTP_SYNC_FUP_PAYLOADSIZE))
    {
      /* !LINKSTO EthTSyn.ASR151.PRS_TS_00208, 1 */
      RetVal = EthTSyn_VerifyAutosarTlv(EthTSynCtrlIdx, &DataPtr[0U], Len);
    }
    else
    {
      RetVal = E_OK;
    }

    if(RetVal == E_OK)
#endif
    {
      /* !LINKSTO EthTSyn.EB.Slave.LostSyncFUPFrame, 1 */
      TS_AtomicAssign8(EthTSyn_Slave[EthTSynCtrlIdx].Sync_SyncFrameReceived, FALSE);
       /* If a transparent clock is used */
      if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SimpleBridge == TRUE)
      {
        /* !LINKSTO EthTSyn.EB.EthSwt_ReceiveSyncFupFrameOnSlavePort, 1 */
        TS_MemCpy(&EthTSyn_Ctrl[EthTSynCtrlIdx].ReceivedFupFrame[0], &DataPtr[0], Len);
        EthTSyn_Ctrl[EthTSynCtrlIdx].ReceivedFupFrameLength = Len;

        /* !LINKSTO EthTSyn.EB.EthSwt_PassFupFrameToMasterPorts, 1 */
        /* Trigger the transmission of syncFup on all master ports. */
        /* !LINKSTO EthTSyn.ASR151.PRS_TS_00209, 1 */
        TS_AtomicAssign8
        (
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncTxState,
          ETHTSYN_CTRL_SYN_TX_STATE_READY_FOR_FUP
        );
      }
      if(
          (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SynchronizeSimpleBridge == TRUE) ||
          (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SimpleBridge == FALSE)
        )
      {
        /* Extract and calculate OTS + Correction */
        EthTSyn_Inc02_AddOtsCorr(DataPtr, EthTSynCtrlIdx);
        /* If the egress timestamp on the host port was received, we can proceed with the synchronization */
        if(EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt].SyncTxState == ETHTSYN_PORT_READY_FOR_TIME_SYNC)
        {
          /* Sync the slave */
          EthTSyn_Inc02_SynchronizeBridge(&EthTSyn_Slave[EthTSynCtrlIdx].OriginTimeStampCorr, EthTSynCtrlIdx);
          
          /* Sync performed, don't retry on the mainfunction, reset the state for the host port */
          TS_AtomicAssign8
          (
              EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt].SyncTxState,
              ETHTSYN_PORT_SYN_TX_STATE_IDLE
          );
        }
        else if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SimpleBridge == FALSE)
        {
            /* The sync frame and the fup are received and processed. */
            TS_AtomicAssign8
            (
              EthTSyn_Ctrl[EthTSynCtrlIdx].SyncTxState,
              ETHTSYN_CTRL_SYN_TX_STATE_READY_FOR_BRIDGE_SYNC
            );
        }
      }
#if(ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED == TS_PROD_ERR_REP_TO_DEM)
      /* Heal dem event */
      /* !LINKSTO EthTSyn.EB.Diagnostic_SyncOK_DEM, 1 */
      Dem_ReportErrorStatus
      (
          EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EventIdSyncFailed,
          DEM_EVENT_STATUS_PREPASSED
      );
#elif (ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED == TS_PROD_ERR_REP_TO_DET)
      /* Sync/Fup pair was received, reset the counter */
      EthTSyn_Slave[EthTSynCtrlIdx].SyncFupDroppedPairsCt = 0U;
#endif
#if(ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED != TS_PROD_ERR_DISABLE)
      /* Reload the value of timeout */
      EthTSyn_Slave[EthTSynCtrlIdx].SyncFupPairTimeout =
        EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].ReceiveSyncFupPairTimeout;
      /* Fup frame was successfully processed */
      RetVal = E_OK;
#endif
    }
#if((ETHTSYN_MESSAGE_COMPLIANCE == STD_OFF) && (ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED != TS_PROD_ERR_DISABLE))
    else
    {
      /* Sub-TLV validation failed */
      RetVal = E_NOT_OK;
    }
#endif
  }
#if (ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED != TS_PROD_ERR_DISABLE)
  else
  {
    /* SeqCounter not equal or sync frame was not received */
    RetVal = E_NOT_OK;
  }
  if(E_OK != RetVal)
  {
    /* Sync frame is missing or there where an issue on processing the Fup frame */
    EthTSyn_SyncFupDropped(EthTSynCtrlIdx);
  }
#else
    TS_PARAM_UNUSED(RetVal);
#endif
}

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_SynchronizeBridge
(
  P2VAR(Eth_TimeIntDiffType, AUTOMATIC, ETHTSYN_APPL_DATA) OtsPdelayCorrPtr,
  uint8 EthTSynCtrlIdx
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  /* Measure Data */
  StbM_MeasurementType MeasureData;
  Eth_TimeStampType TmpTimeStamp;
  Eth_TimeStampType TmpTimeStamp_T8;

  /* Default value for the SYNC_TO_GATEWAY bit in timeBaseStatus */
  EthTSyn_SyncToGatewayBit = 0U;

  TmpTimeStamp.nanoseconds = EthTSyn_Slave[EthTSynCtrlIdx].Pdelay;
  TmpTimeStamp.seconds = 0U;
  TmpTimeStamp.secondsHi = 0U;
  TmpTimeStamp_T8.nanoseconds = 0U;
  TmpTimeStamp_T8.seconds = 0U;
  TmpTimeStamp_T8.secondsHi = 0U;

  /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00150,1 */
  MeasureData.pathDelay = EthTSyn_Slave[EthTSynCtrlIdx].Pdelay;

  if(OtsPdelayCorrPtr->sign == TRUE) /* Proceed with sync, only if no wrong time stamp detected. */
  {
    /* Follow up frame correspond to latest received sync frame. */
    const Eth_TimeStampType Sync_ActualIngressTimeStamp
      = EthTSyn_Slave[EthTSynCtrlIdx].Sync_ActualIngressTimeStamp;
    StbM_VirtualLocalTimeType VirtualLocalTimeT8;
#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
    Eth_TimeStampQualType TimeQuality;
#endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */

    VirtualLocalTimeT8.nanosecondsLo = 0U;
    VirtualLocalTimeT8.nanosecondsHi = 0U;

#if(ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON)
    if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].VltStbMIsEthFreerunningCounter
                                                                                   == TRUE)
    {
      SchM_Enter_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_0();
      /* !LINKSTO EthTSyn.EB.Slave.HWTimpStamp.GatewaySync, 1 */
      /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00024, 1 */
      /* !LINKSTO EthTSyn.EB.TransparentClock_InvalidTimeStamp, 1 */
      RetVal = EthIf_GetCurrentTime
        (
          EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx,
          &TimeQuality,
          &TmpTimeStamp_T8
        );
      SchM_Exit_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_0();
      RetVal |=
        EthTSyn_Int01_ConvertEthTimeToVirtualTime(&TmpTimeStamp_T8, &VirtualLocalTimeT8);
    }
    else
    {
      SchM_Enter_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_1();
      /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00024, 1 */
      RetVal = EthIf_GetCurrentTime
        (
          EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].EthIfCtrlIdx,
          &TimeQuality,
          &TmpTimeStamp_T8
        );
      RetVal |= StbM_GetCurrentVirtualLocalTime
        (
          EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId,
          &VirtualLocalTimeT8
        );
      SchM_Exit_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_1();
    }

    /* !LINKSTO EthTSyn.EB.Slave.InvHWTimpStamp.GatewaySync, 1 */
    if(TimeQuality != ETH_VALID)
    {
      RetVal = E_NOT_OK;
    }

#else /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */
    SchM_Enter_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_1();
    RetVal = StbM_GetCurrentVirtualLocalTime
      (
         EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId,
         &VirtualLocalTimeT8
      );
    SchM_Exit_EthTSyn_SCHM_ETHTSYN_EXCLUSIVE_AREA_1();
    /* Convert virtual time to ethtime */
    EthTSyn_Int01_ConvertVirtualTimeToEthTime(&VirtualLocalTimeT8, &TmpTimeStamp_T8);
#endif /* ETHTSYN_HW_TIMESTAMP_SUPPORT == STD_ON */

    /* !LINKSTO EthTSyn.EB.Slave.StbMSync.GetCurrentTime.NotOk, 1 */
    if(E_OK == RetVal)
    {
      StbM_UserDataType UserData;
      StbM_TimeStampType StbMTimeStamp;
      Eth_TimeIntDiffType TimeStampDiff;

      /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00080, 1 */
      UserData.userDataLength = EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userDataLength;
      UserData.userByte0 = EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte0;
      UserData.userByte1 = EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte1;
      UserData.userByte2 = EthTSyn_Ctrl[EthTSynCtrlIdx].StbMUserData.userByte2;
      /* T8 - T6 */
      EthTSyn_SubTimeStamps(
                     &TmpTimeStamp_T8, /* T8 */
                     &Sync_ActualIngressTimeStamp, /* T6 */
                     &TimeStampDiff /* (T8 - T6 ) */
                   );
      /* OriginTimeStamp + correctionField + Pdelay */
      EthTSyn_AddTimeStamps(
                 &TmpTimeStamp,
                 &OtsPdelayCorrPtr->diff /* OriginTimeStamp + correctionField  */
               );
      /* OriginTimeStamp + correctionField + Pdelay + (T8 - T6) */
      EthTSyn_AddTimeStamps(
                   &TmpTimeStamp, /* OriginTimeStamp + correctionField + Pdelay */
                   &TimeStampDiff.diff /* T8 - T6  */
                 );
      /* OriginTimeStamp + correctionField + Pdelay + (T8 - T6) + (T5 - T4)*/
      EthTSyn_AddTimeStamps(
                   &TmpTimeStamp, /* OriginTimeStamp + correctionField + Pdelay + (T8 - T6) */
                   &EthTSyn_Slave[EthTSynCtrlIdx].TimeDiffHostEgressSlaveIngress.diff /* T5 - T4  */
                 );
      StbMTimeStamp.nanoseconds = TmpTimeStamp.nanoseconds;
      StbMTimeStamp.seconds = TmpTimeStamp.seconds;
      StbMTimeStamp.secondsHi = TmpTimeStamp.secondsHi;
      StbMTimeStamp.timeBaseStatus = EthTSyn_SyncToGatewayBit;

      /* Return value ignored: No further action possible if E_NOT_OK returned. */
      /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00129, 1 */
      /* !LINKSTO EthTSyn.EB.Slave.HWTimpStamp.GatewaySync, 1 */
      /* !LINKSTO EthTSyn.EB.Slave.StbMSync, 1 */
      /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00052, 1 */
      /* !LINKSTO EthTSyn.EB.TransparentClock_Synchronize, 1 */
      (void) StbM_BusSetGlobalTime
        (
          EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId, /* timeBaseId */
          &StbMTimeStamp, /* timeStampPtr */
          &UserData, /* userDataPtr */
          &MeasureData, /* measureDataPtr */
          &VirtualLocalTimeT8 /* localTimePtr */
        );
        
      /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */
#if(ETHTSYN_TIME_RECORDING == STD_ON)
      if(EthTSyn_RootCfgPtr->SwitchConfig[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave].TimeValidationEnabled == TRUE)
      {
        /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */
        EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.pDelay =
                                                      EthTSyn_Slave[EthTSynCtrlIdx].Pdelay;
        /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00216, 1 */
        (void) StbM_EthSetSlaveTimingData
          (
            EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].TimeBaseId, /* timeBaseId */
            /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00217, 1 */
            &EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording /* measureDataPtr */
          );
      }
#endif /* ETHTSYN_TIME_RECORDING == STD_ON */

      if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SimpleBridge == FALSE)
      {
        uint32 ConfiguredTxPeriod = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].BridgeTxPeriod;
        uint32 TransmitSync = 0U;

        if(ConfiguredTxPeriod % 2U == 0U)
        {
          TransmitSync = ConfiguredTxPeriod / 2U;
        }
        else
        {
          TransmitSync = (ConfiguredTxPeriod / 2U) + 1U;
        }
        if(
            ((EthTSyn_GeneralCounter - EthTSyn_Slave[EthTSynCtrlIdx].BridgeLastSyncSentTime) >= TransmitSync) ||
            (EthTSyn_Slave[EthTSynCtrlIdx].BridgeLastSyncSentTime == 0U)
          )
        {
          /* !LINKSTO EthTSyn.EB.BoundaryClock_TransmitSyncOnHostPort, 1 */
          /* !LINKSTO EthTSyn.EB.BoundaryClock_TransmitSyncOnHostPort_SecondTx_Less, 1 */
          /* !LINKSTO EthTSyn.EB.BoundaryClock_TransmitSyncOnHostPort_SecondTx_More, 1 */
          /* ready to transmit, on the next mainfunction, the sync to the connected slaves */
          /* Boundary clock transmit sync frames . */
          EthTSyn_Slave[EthTSynCtrlIdx].TriggerTxOnBridge = TRUE;
        }

        TS_AtomicAssign8
        (
          EthTSyn_Ctrl[EthTSynCtrlIdx].SyncTxState,
          ETHTSYN_CTRL_SYN_TX_STATE_IDLE
        );
      }
      /* After sync we can reset the variables */
      EthTSyn_Slave[EthTSynCtrlIdx].OriginTimeStampCorr.diff.nanoseconds = 0U;
      EthTSyn_Slave[EthTSynCtrlIdx].OriginTimeStampCorr.diff.seconds = 0U;
      EthTSyn_Slave[EthTSynCtrlIdx].OriginTimeStampCorr.diff.secondsHi = 0U;
      EthTSyn_Slave[EthTSynCtrlIdx].OriginTimeStampCorr.sign = FALSE;
      EthTSyn_Slave[EthTSynCtrlIdx].TimeDiffHostEgressSlaveIngress.diff.nanoseconds = 0U;
      EthTSyn_Slave[EthTSynCtrlIdx].TimeDiffHostEgressSlaveIngress.diff.seconds = 0U;
      EthTSyn_Slave[EthTSynCtrlIdx].TimeDiffHostEgressSlaveIngress.diff.secondsHi = 0U;
      EthTSyn_Slave[EthTSynCtrlIdx].TimeDiffHostEgressSlaveIngress.sign = FALSE;
    }
  }
}

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_AddOtsCorr
(
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint8 EthTSynCtrlIdx
)
{
  Std_ReturnType RetVal = E_NOT_OK;
  Eth_TimeStampType OriginTimeStamp;

  OriginTimeStamp.nanoseconds = 0U;
  OriginTimeStamp.seconds = 0U;
  OriginTimeStamp.secondsHi = 0U;


  /* Get origin time stamp of the master from follow up frame payload. */
  EthTSyn_ArrayToTimeStamp(&OriginTimeStamp, &DataPtr[ETHTSYN_PTP_HEADER_SIZE]);

  if(ETHTSYN_NANOSEC_MAX < OriginTimeStamp.nanoseconds)
  {
    /* !LINKSTO EthTSyn.EB.TransparentClock_SyncFUp_NanoSecMax, 1 */
    /* !LINKSTO EthTSyn.EB.Nanoseconds, 1 */
    RetVal = E_NOT_OK;
  }
  else
  {
    /* !LINKSTO EthTSyn.EB.NanosecondsLimit, 1 */
    RetVal = E_OK;
  }

  if(E_OK == RetVal)
  {
    Eth_TimeStampType TmpTimeStamp;
    /* Calculate time offset between master and slave. */
    /* OriginTimeStampSync[FUP] */
    TmpTimeStamp.secondsHi = 0U;
    TmpTimeStamp.seconds = 0U;
    TmpTimeStamp.nanoseconds = 0U;

    RetVal = EthTSyn_AddCorrField
    (
      EthTSynCtrlIdx,
      &DataPtr[ETHTSYN_PTP_HEADER_B8_CORRFIELD_0],
      &TmpTimeStamp
    );

    if(E_OK == RetVal) /* Only perform a correction if no wrong time stamp detected. */
    {
#if(ETHTSYN_TIME_RECORDING == STD_ON)
      if(EthTSyn_RootCfgPtr->SwitchConfig[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave].TimeValidationEnabled == TRUE)
      {
        /* Save PreciseOriginTimestamp as received in the Followup Message from master */
        EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.preciseOriginTimestamp.
                                                 nanoseconds = OriginTimeStamp.nanoseconds;
        EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.preciseOriginTimestamp.
                                                 seconds = OriginTimeStamp.seconds;
                                                 
      }
#endif
      EthTSyn_AddTimeStamps(&TmpTimeStamp, &OriginTimeStamp);
      EthTSyn_Slave[EthTSynCtrlIdx].OriginTimeStampCorr.diff.nanoseconds = TmpTimeStamp.nanoseconds;
      EthTSyn_Slave[EthTSynCtrlIdx].OriginTimeStampCorr.diff.seconds = TmpTimeStamp.seconds;
      EthTSyn_Slave[EthTSynCtrlIdx].OriginTimeStampCorr.diff.secondsHi = TmpTimeStamp.secondsHi;
      EthTSyn_Slave[EthTSynCtrlIdx].OriginTimeStampCorr.sign = TRUE;
      /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */
    }
  }
  if(RetVal != E_OK)
  {
    EthTSyn_Slave[EthTSynCtrlIdx].OriginTimeStampCorr.diff.nanoseconds = 0U;
    EthTSyn_Slave[EthTSynCtrlIdx].OriginTimeStampCorr.diff.seconds = 0U;
    EthTSyn_Slave[EthTSynCtrlIdx].OriginTimeStampCorr.diff.secondsHi = 0U;
    EthTSyn_Slave[EthTSynCtrlIdx].OriginTimeStampCorr.sign = FALSE;
  }
}

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_ProcessRxSyncFrame
(
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint8 EthTSynCtrlIdx
)
{
  Std_ReturnType TmpRet = E_NOT_OK;
  Eth_TimeStampType Sync_IngressTimeStamp;

  Sync_IngressTimeStamp.nanoseconds = 0U;
  Sync_IngressTimeStamp.seconds = 0U;
  Sync_IngressTimeStamp.secondsHi = 0U;

  /* If a boundary clock is used, take the ingress timestamp*/
  if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SimpleBridge == FALSE)
  {
    /* !LINKSTO EthTSyn.ASR20-11.PRS_TS_00199_CheckSC, 1 */
    /* !LINKSTO EthTSyn.EB.TransparentClock_ValidateSync, 1 */
    /* !LINKSTO EthTSyn.EB.TimeAwareBridge_DEM_FailedSync_Ingress_SC, 1 */
    if(E_OK == EthTSyn_CheckSC(EthTSynCtrlIdx, DataPtr))
    {
      /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00128, 1 */
      TmpRet = EthTSyn_GetIngressTimeStamp(EthTSynCtrlIdx, DataPtr, &Sync_IngressTimeStamp);
      if(TmpRet == E_OK)
      {
        EthTSyn_Slave[EthTSynCtrlIdx].Sync_ActualIngressTimeStamp = Sync_IngressTimeStamp;
      }
    }
  }
  else
  {
    if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SynchronizeSimpleBridge == TRUE)
    {
      Std_ReturnType RetValChecks = E_NOT_OK;
      /* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00128, 1 */
      /* !LINKSTO EthTSyn.EB.TransparentClock_SynchronizeFailIngress, 1 */
      RetValChecks = EthTSyn_GetIngressTimeStamp(EthTSynCtrlIdx, DataPtr, &Sync_IngressTimeStamp);
      if(RetValChecks == E_OK)
      {
        EthTSyn_Slave[EthTSynCtrlIdx].Sync_ActualIngressTimeStamp = Sync_IngressTimeStamp;
        /* Wait egress timestamp from the host port. */
        TS_AtomicAssign8
        (
          EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt].SyncTxState,
          ETHTSYN_PORT_SYN_TX_STATE_WAIT_EGRESS_TS
        );
      }
      else
      {
        /* Something is not ok, Skip the synchronization. */
        TS_AtomicAssign8
        (
          EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt].SyncTxState,
          ETHTSYN_PORT_SYN_TX_STATE_IDLE
        );
        EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortMgt].UniqueId = ETHTSYN_UNIQUE_ID_INVALID;
      }
    }
    if(EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].SimpleBridgeValidateSync == TRUE)
    {
      if(E_OK != EthTSyn_CheckSC(EthTSynCtrlIdx, DataPtr))
      {
        TmpRet = E_NOT_OK;
      }
      else
      {
        TmpRet = E_OK;
      }
    }
    else
    {
      /* Save the seqId. */
      TS_AtomicAssign16(EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber, EthTSyn_GetPTPHeaderSequNumber(DataPtr));
      /* If transparent clock is used, proceed with processing the sync frame */ 
      TmpRet = E_OK;
    }
  }

#if (ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED != TS_PROD_ERR_DISABLE)
  if((EthTSyn_Slave[EthTSynCtrlIdx].Sync_SyncFrameReceived == TRUE) || (E_OK != TmpRet))
  {
    /* Fup frame is missing or there where an issue on processing the sync frame */
    EthTSyn_SyncFupDropped(EthTSynCtrlIdx);
  }
#endif

  /* !LINKSTO EthTSyn.EB.Slave.LostSyncFUPFrame, 1 */
  if(E_OK == TmpRet)
  {
    TS_AtomicAssign8(EthTSyn_Slave[EthTSynCtrlIdx].Sync_SyncFrameReceived, TRUE);
    /* Timeout value of the Follow_Up message (of the subsequent Sync message).
       A value of 0 deactivates this timeout observation.*/
    EthTSyn_Slave[EthTSynCtrlIdx].GlobalTimeFollowUpTimeout =
            EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].GlobalTimeFollowUpTimeout;
    /* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00213, 1 */
#if(ETHTSYN_TIME_RECORDING == STD_ON)
    if(EthTSyn_RootCfgPtr->SwitchConfig[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave].TimeValidationEnabled == TRUE)
    {

      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.clockIdentity =
           (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0]) << 56U);
      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.clockIdentity |=
           (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B21_SRCPORTIDENTITY_1]) << 48U);
      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.clockIdentity |=
           (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B22_SRCPORTIDENTITY_2]) << 40U);
      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.clockIdentity |=
           (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B23_SRCPORTIDENTITY_3]) << 32U);
      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.clockIdentity |=
           (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B24_SRCPORTIDENTITY_4]) << 24U);
      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.clockIdentity |=
           (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B25_SRCPORTIDENTITY_5]) << 16U);
      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.clockIdentity |=
           (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B26_SRCPORTIDENTITY_6]) << 8U);
      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.clockIdentity |=
           (uint64)((uint64) DataPtr[ETHTSYN_PTP_HEADER_B27_SRCPORTIDENTITY_7]);
      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.portNumber =
           (uint16)((uint16) DataPtr[ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8]);
      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sourcePortId.portNumber |=
           (uint16)((uint16) DataPtr[ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9]);
      EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.sequenceId =
                                                EthTSyn_Ctrl[EthTSynCtrlIdx].SyncSequNumber;
        /* Save the ingress timestamp to be passed to the StbM. */
        /* Convert the ingress timestamp. */
        EthTSyn_Int01_ConvertEthTimeToVirtualTime(&EthTSyn_SwitchPorts[EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortSlave].SyncTimeStamp,
           &EthTSyn_Slave[EthTSynCtrlIdx].SyncSlaveTimeRecording.syncIngressTimestamp);

    }
#endif
  }
  else
  {
    /* Reset the state of the received sync frame*/
    TS_AtomicAssign8(EthTSyn_Slave[EthTSynCtrlIdx].Sync_SyncFrameReceived, FALSE);
  }

}
#endif /* ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON */

STATIC FUNC(void, ETHTSYN_CODE) EthTSyn_Inc02_ProcessRxPdelayReqSwtMsg
(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHTSYN_APPL_DATA) MgmtInfoPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) IngressTimeStampPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint8 EthTSynCtrlIdx,
  uint16 Len
)
{
  boolean Found = FALSE;
  EthTSyn_IntPortIdxType Idx;
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF)
  Idx = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart;
#else
  /* The pdelay_req frame is not processed for the slave port. */
  Idx = EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart + 2U;
#endif
  for
  (;((Idx < (EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortCount +
            EthTSyn_RootCfgPtr->EthTSynCtrl[EthTSynCtrlIdx].PortStart))
        && (FALSE == Found));
     Idx++
  )
  {
    /* !LINKSTO EthTSyn.EB.SwitchPdelay.PdelayReqReceived_Incorrect_MgmtInfo, 1 */
    if(MgmtInfoPtr->SwitchIdx == EthTSyn_RootCfgPtr->SwitchConfig[Idx].EthSwtIdx)
    {
      if(MgmtInfoPtr->SwitchPortIdx == EthTSyn_RootCfgPtr->SwitchConfig[Idx].EthSwtPortIdx)
      {
        /* !LINKSTO EthTSyn.ASR151.PRS_TS_00143, 1 */
        if(EthTSyn_RootCfgPtr->SwitchConfig[Idx].PdelayRespEnable == TRUE)
        {
          Found = TRUE;
          /* Store serial number to be used for Pdelay_resp and Pdelay_resp_FUp. */
          /* !LINKSTO EthTSyn.EB.SwitchPdelay.PdelayReqReceived_SaveData, 1 */
          TS_AtomicAssign16(
                             EthTSyn_SwitchPorts[Idx].Ingress_PdelayReqSequNumber,
                             EthTSyn_GetPTPHeaderSequNumber(DataPtr)
                           );
          TS_MemCpy(
               &EthTSyn_SwitchPorts[Idx].Pdelay_Rx_SrcPortIdentity[0U],
               &DataPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0],
               ETHTSYN_SOURCEPORTIDENTITY_SIZE
             );
          /* !LINKSTO EthTSyn.EB.SwitchPdelay.PdelayReq_SaveIngressTS, 1 */
          EthTSyn_SwitchPorts[Idx].IngPdelayFrameTimeStamp.nanoseconds = IngressTimeStampPtr->nanoseconds;
          EthTSyn_SwitchPorts[Idx].IngPdelayFrameTimeStamp.seconds = IngressTimeStampPtr->seconds;
          EthTSyn_SwitchPorts[Idx].IngPdelayFrameTimeStamp.secondsHi = IngressTimeStampPtr->secondsHi;

          TS_PARAM_UNUSED(Len);
          /* Make sure that the transmit flag is set to true
           * (this can be already set if a sync cycle is in progress) */
          /* !LINKSTO EthTSyn.EB.SwitchPdelay.PdelayResp_Transmit, 1 */
          ETHTSYN_SET_TXFLAG
            (&EthTSyn_Ctrl[EthTSynCtrlIdx].Frame2Transmit, ETHTSYN_BITPOS_TX_SWT, TRUE);

          /* In the next mainfunction the pdelayresp frame can be transmitted. */
          /* !LINKSTO EthTSyn.EB.TimeAwareBridge_MasterReceivePdelayReq, 1 */
          TS_AtomicSetBit_8
          (
            &EthTSyn_SwitchPorts[Idx].Frame2Transmit,
            ETHTSYN_BITPOS_PDELAYRESP
          );
#if(ETHTSYN_TIME_RECORDING == STD_ON)
          if(EthTSyn_RootCfgPtr->SwitchConfig[Idx].TimeValidationEnabled == TRUE)
          {

            /* If timevalidation is enabled save the relevant data. */
            EthTSyn_SwitchPorts[Idx].PdelayResponderTimeRecording.sequenceId =
                               EthTSyn_SwitchPorts[Idx].Ingress_PdelayReqSequNumber;
            
            EthTSyn_SwitchPorts[Idx].PdelayResponderTimeRecording.requestPortId.clockIdentity =
                 (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B20_SRCPORTIDENTITY_0]) << 56U);
            EthTSyn_SwitchPorts[Idx].PdelayResponderTimeRecording.requestPortId.clockIdentity |=
                 (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B21_SRCPORTIDENTITY_1]) << 48U);
            EthTSyn_SwitchPorts[Idx].PdelayResponderTimeRecording.requestPortId.clockIdentity |=
                 (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B22_SRCPORTIDENTITY_2]) << 40U);
            EthTSyn_SwitchPorts[Idx].PdelayResponderTimeRecording.requestPortId.clockIdentity |=
                 (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B23_SRCPORTIDENTITY_3]) << 32U);
            EthTSyn_SwitchPorts[Idx].PdelayResponderTimeRecording.requestPortId.clockIdentity |=
                 (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B24_SRCPORTIDENTITY_4]) << 24U);
            EthTSyn_SwitchPorts[Idx].PdelayResponderTimeRecording.requestPortId.clockIdentity |=
                 (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B25_SRCPORTIDENTITY_5]) << 16U);
            EthTSyn_SwitchPorts[Idx].PdelayResponderTimeRecording.requestPortId.clockIdentity |=
                 (uint64)(((uint64) DataPtr[ETHTSYN_PTP_HEADER_B26_SRCPORTIDENTITY_6]) << 8U);
            EthTSyn_SwitchPorts[Idx].PdelayResponderTimeRecording.requestPortId.clockIdentity |=
                 (uint64)((uint64) DataPtr[ETHTSYN_PTP_HEADER_B27_SRCPORTIDENTITY_7]);
            EthTSyn_SwitchPorts[Idx].PdelayResponderTimeRecording.requestPortId.portNumber =
                 (uint16)((uint16) DataPtr[ETHTSYN_PTP_HEADER_B28_SRCPORTIDENTITY_8]);
            EthTSyn_SwitchPorts[Idx].PdelayResponderTimeRecording.requestPortId.portNumber |=
                 (uint16)((uint16) DataPtr[ETHTSYN_PTP_HEADER_B29_SRCPORTIDENTITY_9]);

            (void)EthTSyn_Int01_ConvertEthTimeToVirtualTime
                (
                    &EthTSyn_SwitchPorts[Idx].IngPdelayFrameTimeStamp,
                    &EthTSyn_SwitchPorts[Idx].PdelayResponderTimeRecording.requestReceiptTimestamp
                );
          }
#endif /* ETHTSYN_TIME_RECORDING == STD_ON */
        }
      }
    }
  }
}
#endif /* (ETHTSYN_SWITCH_USED == STD_ON) */


#define ETHTSYN_STOP_SEC_CODE
#include <EthTSyn_MemMap.h>
/*==================[end of file]===========================================*/
