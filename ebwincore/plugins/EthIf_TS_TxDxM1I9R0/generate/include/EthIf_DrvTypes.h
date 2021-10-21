/**
 * \file
 *
 * \brief AUTOSAR EthIf
 *
 * This file contains the implementation of the AUTOSAR
 * module EthIf.
 *
 * \version 1.9.18
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!CODE!]
[!AUTOSPACING!]

#ifndef ETHIF_DRVTYPES_H
#define ETHIF_DRVTYPES_H

[!INCLUDE "../../generate_macros/EthIf_Cfg.m"!][!//
#include <EthIf_Cfg.h>

[!/* *** include Eth driver(s) header(s) *** */!][!//
[!IF "$EthIf_CtrlBswRefNr > 0"!][!//
[!FOR "i"="1" TO "$EthIf_CtrlBswRefNr"!][!//
  [!VAR "driverVendorId" = "asc:getVendorId(as:path(node:ref(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfEthControllerType/*[num:i($i)]/EthIfEthControllerBswmdImplementationRefs)))"!][!//
  [!VAR "driverApiInfix" = "asc:getVendorApiInfix(as:path(node:ref(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfEthControllerType/*[num:i($i)]/EthIfEthControllerBswmdImplementationRefs)))"!][!//
  [!VAR "driverExtension" = "concat($driverVendorId,'_', $driverApiInfix)"!][!//
#include <Eth_[!"$driverExtension"!].h>
[!ENDFOR!][!//
[!ELSE!][!//
#include <Eth.h>              /* AUTOSAR Eth controller driver header file inclusion */
[!ENDIF!][!//

[!/* *** include Eth transceiver(s) header(s) *** */!][!//
[!IF "$EthIf_TrcvBswRefNr > 0"!][!//
[!FOR "i"="1" TO "$EthIf_TrcvBswRefNr"!][!//
  [!VAR "trcvVendorId" = "asc:getVendorId(as:path(node:ref(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfEthTrcvType/*[num:i($i)]/EthIfEthTrcvBswmdImplementationRefs)))"!][!//
  [!VAR "trcvApiInfix" = "asc:getVendorApiInfix(as:path(node:ref(as:modconf('EthIf')[1]/EthIfConfigSet/EthIfEthTrcvType/*[num:i($i)]/EthIfEthTrcvBswmdImplementationRefs)))"!][!//
  [!VAR "trcvExtension" = "concat($trcvVendorId,'_', $trcvApiInfix)"!][!//
#include <EthTrcv_[!"$trcvExtension"!].h>
[!ENDFOR!][!//
[!ELSEIF "$EthIf_TrcvSupportEnable"!][!//
#include <EthTrcv.h>              /* AUTOSAR Eth transceiver header file inclusion */
[!ELSE!][!//
[!ENDIF!][!//

[!/* *** include Eth switch) header(s) *** */!][!//
[!IF "$EthIf_SwtBswRefNr > 0"!][!//
#include <EthSwt_[!"$swtExtension"!].h>
[!ELSEIF "$EthIf_SwtSupportEnable"!][!//
#include <EthSwt.h>              /* AUTOSAR Eth switch header file inclusion */
[!ELSE!][!//
[!ENDIF!][!//

/*==================[macros]=================================================*/

/* --- Eth controller function definitions---------------------------------- */

#if (defined ETHIF_ETH_GETCONTROLLERMODE)
#error ETHIF_ETH_GETCONTROLLERMODE is already defined
#endif
[!IF "$EthIf_CtrlBswRefNr = 0"!][!//
#define ETHIF_ETH_GETCONTROLLERMODE(PhyCtrlPtr, CtrlModePtr) \
Eth_GetControllerMode((PhyCtrlPtr)->EthCtrlIdx, (CtrlModePtr))
[!ELSEIF "$EthIf_CtrlBswRefNr = 1"!][!//
#define ETHIF_ETH_GETCONTROLLERMODE(PhyCtrlPtr, CtrlModePtr) \
Eth_[!"$driverExtension"!]_GetControllerMode((PhyCtrlPtr)->EthCtrlIdx, (CtrlModePtr))
[!ELSE!][!//
#define ETHIF_ETH_GETCONTROLLERMODE(PhyCtrlPtr, CtrlModePtr) \
EthIf_EthFuncConfig[(PhyCtrlPtr)->EthCtrlApiIdx].GetControllerMode((PhyCtrlPtr)->EthCtrlIdx, \
(CtrlModePtr))
[!ENDIF!][!//

#if (defined ETHIF_ETH_GETPHYSADDR)
#error ETHIF_ETH_GETPHYSADDR is already defined
#endif
[!IF "$EthIf_CtrlBswRefNr = 0"!][!//
#define ETHIF_ETH_GETPHYSADDR(PhyCtrlPtr, PhysAddrPtr) \
Eth_GetPhysAddr((PhyCtrlPtr)->EthCtrlIdx, (PhysAddrPtr))
[!ELSEIF "$EthIf_CtrlBswRefNr = 1"!][!//
#define ETHIF_ETH_GETPHYSADDR(PhyCtrlPtr, PhysAddrPtr) \
Eth_[!"$driverExtension"!]_GetPhysAddr((PhyCtrlPtr)->EthCtrlIdx, (PhysAddrPtr))
[!ELSE!][!//
#define ETHIF_ETH_GETPHYSADDR(PhyCtrlPtr, PhysAddrPtr) \
EthIf_EthFuncConfig[(PhyCtrlPtr)->EthCtrlApiIdx].GetPhysAddr((PhyCtrlPtr)->EthCtrlIdx,(PhysAddrPtr))
[!ENDIF!][!//

#if (defined ETHIF_ETH_PROVIDETXBUFFER)
#error ETHIF_ETH_PROVIDETXBUFFER is already defined
#endif
[!IF "$EthIf_CtrlBswRefNr = 0"!][!//
#define ETHIF_ETH_PROVIDETXBUFFER(PhyCtrlPtr, Priority, BufIdxPtr, BufPtr, LenBytePtr) \
Eth_ProvideTxBuffer((PhyCtrlPtr)->EthCtrlIdx, (Priority), (BufIdxPtr), (BufPtr), (LenBytePtr))
[!ELSEIF "$EthIf_CtrlBswRefNr = 1"!][!//
#define ETHIF_ETH_PROVIDETXBUFFER(PhyCtrlPtr, Priority, BufIdxPtr, BufPtr, LenBytePtr)\
Eth_[!"$driverExtension"!]_ProvideTxBuffer((PhyCtrlPtr)->EthCtrlIdx, (Priority), (BufIdxPtr), \
(BufPtr), (LenBytePtr))
[!ELSE!][!//
#define ETHIF_ETH_PROVIDETXBUFFER(PhyCtrlPtr, Priority, BufIdxPtr, BufPtr, LenBytePtr) \
EthIf_EthFuncConfig[(PhyCtrlPtr)->EthCtrlApiIdx].ProvideTxBuffer((PhyCtrlPtr)->EthCtrlIdx, \
(Priority), (BufIdxPtr), (BufPtr), (LenBytePtr))
[!ENDIF!][!//

#if (defined ETHIF_ETH_READMII)
#error ETHIF_ETH_READMII is already defined
#endif
[!IF "$EthIf_CtrlBswRefNr = 0"!][!//
#define ETHIF_ETH_READMII(PhyCtrlPtr, TrcvIdx, RegIdx, RegValPtr) \
Eth_ReadMii((PhyCtrlPtr)->EthCtrlIdx, (TrcvIdx), (RegIdx), (RegValPtr))
[!ELSEIF "$EthIf_CtrlBswRefNr = 1"!][!//
#define ETHIF_ETH_READMII(PhyCtrlPtr, TrcvIdx, RegIdx, RegValPtr) \
Eth_[!"$driverExtension"!]_ReadMii((PhyCtrlPtr)->EthCtrlIdx, (TrcvIdx), (RegIdx), (RegValPtr))
[!ELSE!][!//
#define ETHIF_ETH_READMII(PhyCtrlPtr, TrcvIdx, RegIdx, RegValPtr) \
EthIf_EthFuncConfig[(PhyCtrlPtr)->EthCtrlApiIdx].ReadMii((PhyCtrlPtr)->EthCtrlIdx, \
(TrcvIdx), (RegIdx), (RegValPtr))
[!ENDIF!][!//

#if (defined ETHIF_ETH_WRITEMII)
#error ETHIF_ETH_WRITEMII is already defined
#endif
[!IF "$EthIf_CtrlBswRefNr = 0"!][!//
#define ETHIF_ETH_WRITEMII(PhyCtrlPtr, TrcvIdx, RegIdx, RegVal) \
Eth_WriteMii((PhyCtrlPtr)->EthCtrlIdx, (TrcvIdx), (RegIdx), (RegVal))
[!ELSEIF "$EthIf_CtrlBswRefNr = 1"!][!//
#define ETHIF_ETH_WRITEMII(PhyCtrlPtr, TrcvIdx, RegIdx, RegVal) \
Eth_[!"$driverExtension"!]_WriteMii((PhyCtrlPtr)->EthCtrlIdx, (TrcvIdx), (RegIdx), (RegVal))
[!ELSE!][!//
#define ETHIF_ETH_WRITEMII(PhyCtrlPtr, TrcvIdx, RegIdx, RegVal) \
EthIf_EthFuncConfig[(PhyCtrlPtr)->EthCtrlApiIdx].WriteMii((PhyCtrlPtr)->EthCtrlIdx, \
(TrcvIdx), (RegIdx), (RegVal))
[!ENDIF!][!//

#if (defined ETHIF_ETH_RECEIVE)
#error ETHIF_ETH_RECEIVE is already defined
#endif
[!IF "$EthIf_CtrlBswRefNr = 0"!][!//
#define ETHIF_ETH_RECEIVE(PhyCtrlPtr, FifoIdx, RxStatus) \
Eth_Receive((PhyCtrlPtr)->EthCtrlIdx, (FifoIdx), (RxStatus))
[!ELSEIF "$EthIf_CtrlBswRefNr = 1"!][!//
#define ETHIF_ETH_RECEIVE(PhyCtrlPtr, FifoIdx, RxStatus) \
Eth_[!"$driverExtension"!]_Receive((PhyCtrlPtr)->EthCtrlIdx, (FifoIdx), (RxStatus))
[!ELSE!][!//
#define ETHIF_ETH_RECEIVE(PhyCtrlPtr, FifoIdx, RxStatus) \
EthIf_EthFuncConfig[(PhyCtrlPtr)->EthCtrlApiIdx].Receive((PhyCtrlPtr)->EthCtrlIdx, (FifoIdx), \
(RxStatus))
[!ENDIF!][!//

#if (defined ETHIF_ETH_SETCONTROLLERMODE)
#error ETHIF_ETH_SETCONTROLLERMODE is already defined
#endif
[!IF "$EthIf_CtrlBswRefNr = 0"!][!//
#define ETHIF_ETH_SETCONTROLLERMODE(PhyCtrlPtr, CtrlMode) \
Eth_SetControllerMode((PhyCtrlPtr)->EthCtrlIdx, (CtrlMode))
[!ELSEIF "$EthIf_CtrlBswRefNr = 1"!][!//
#define ETHIF_ETH_SETCONTROLLERMODE(PhyCtrlPtr, CtrlMode) \
Eth_[!"$driverExtension"!]_SetControllerMode((PhyCtrlPtr)->EthCtrlIdx, (CtrlMode))
[!ELSE!][!//
#define ETHIF_ETH_SETCONTROLLERMODE(PhyCtrlPtr, CtrlMode) \
EthIf_EthFuncConfig[(PhyCtrlPtr)->EthCtrlApiIdx].SetControllerMode((PhyCtrlPtr)->EthCtrlIdx, \
(CtrlMode))
[!ENDIF!][!//

#if (defined ETHIF_ETH_TRANSMIT)
#error ETHIF_ETH_TRANSMIT is already defined
#endif
[!IF "$EthIf_CtrlBswRefNr = 0"!][!//
#define ETHIF_ETH_TRANSMIT(PhyCtrlPtr, BufIdx, FrameType, TxConfirmation, LenByte, PhysAddrPtr) \
Eth_Transmit((PhyCtrlPtr)->EthCtrlIdx, (BufIdx), (FrameType), (TxConfirmation), (LenByte), \
PhysAddrPtr)
[!ELSEIF "$EthIf_CtrlBswRefNr = 1"!][!//
#define ETHIF_ETH_TRANSMIT(PhyCtrlPtr, BufIdx, FrameType, TxConfirmation, LenByte, PhysAddrPtr)\
Eth_[!"$driverExtension"!]_Transmit((PhyCtrlPtr)->EthCtrlIdx, (BufIdx), (FrameType), \
(TxConfirmation), (LenByte), PhysAddrPtr)
[!ELSE!][!//
#define ETHIF_ETH_TRANSMIT(PhyCtrlPtr, BufIdx, FrameType, TxConfirmation, LenByte, PhysAddrPtr) \
EthIf_EthFuncConfig[(PhyCtrlPtr)->EthCtrlApiIdx].Transmit((PhyCtrlPtr)->EthCtrlIdx, \
(BufIdx), (FrameType), (TxConfirmation), (LenByte), PhysAddrPtr)
[!ENDIF!][!//

#if (defined ETHIF_ETH_TXCONFIRMATON)
#error ETHIF_ETH_TXCONFIRMATON is already defined
#endif
[!IF "$EthIf_CtrlBswRefNr = 0"!][!//
#define ETHIF_ETH_TXCONFIRMATON(PhyCtrlPtr) \
Eth_TxConfirmation((PhyCtrlPtr)->EthCtrlIdx)
[!ELSEIF "$EthIf_CtrlBswRefNr = 1"!][!//
#define ETHIF_ETH_TXCONFIRMATON(PhyCtrlPtr) \
Eth_[!"$driverExtension"!]_TxConfirmation((PhyCtrlPtr)->EthCtrlIdx)
[!ELSE!][!//
#define ETHIF_ETH_TXCONFIRMATON(PhyCtrlPtr) \
EthIf_EthFuncConfig[(PhyCtrlPtr)->EthCtrlApiIdx].TxConfirmation((PhyCtrlPtr)->EthCtrlIdx)
[!ENDIF!][!//

#if (defined ETHIF_ETH_SETPHYADDR)
#error ETHIF_ETH_SETPHYADDR is already defined
#endif
[!IF "$EthIf_CtrlBswRefNr = 0"!][!//
#define ETHIF_ETH_SETPHYADDR(PhyCtrlPtr, PhysAddrPtr) \
Eth_SetPhysAddr((PhyCtrlPtr)->EthCtrlIdx, (PhysAddrPtr))
[!ELSEIF "$EthIf_CtrlBswRefNr = 1"!][!//
#define ETHIF_ETH_SETPHYADDR(PhyCtrlPtr, PhysAddrPtr) \
Eth_[!"$driverExtension"!]_SetPhysAddr((PhyCtrlPtr)->EthCtrlIdx, (PhysAddrPtr))
[!ELSE!][!//
#define ETHIF_ETH_SETPHYADDR(PhyCtrlPtr, PhysAddrPtr) \
EthIf_EthFuncConfig[(PhyCtrlPtr)->EthCtrlApiIdx].SetPhysAddr((PhyCtrlPtr)->EthCtrlIdx,(PhysAddrPtr))
[!ENDIF!][!//

#if (defined ETHIF_ETH_UPDATEPHYADDRFILTER)
#error ETHIF_ETH_UPDATEPHYADDRFILTER is already defined
#endif
[!IF "$EthIf_CtrlBswRefNr = 0"!][!//
#define ETHIF_ETH_UPDATEPHYADDRFILTER(PhyCtrlPtr, PhysAddrPtr, Action) \
Eth_UpdatePhysAddrFilter((PhyCtrlPtr)->EthCtrlIdx, (PhysAddrPtr), (Action))
[!ELSEIF "$EthIf_CtrlBswRefNr = 1"!][!//
#define ETHIF_ETH_UPDATEPHYADDRFILTER(PhyCtrlPtr, PhysAddrPtr, Action) \
Eth_[!"$driverExtension"!]_UpdatePhysAddrFilter((PhyCtrlPtr)->EthCtrlIdx, (PhysAddrPtr), (Action))
[!ELSE!][!//
#define ETHIF_ETH_UPDATEPHYADDRFILTER(PhyCtrlPtr, PhysAddrPtr, Action) \
EthIf_EthFuncConfig[(PhyCtrlPtr)->EthCtrlApiIdx].UpdatePhysAddrFilter((PhyCtrlPtr)->EthCtrlIdx,\
(PhysAddrPtr), (Action))
[!ENDIF!][!//

#if (defined ETHIF_ETH_GETCURRENTTIME)
#error ETHIF_ETH_GETCURRENTTIME is already defined
#endif
[!IF "$EthIf_CtrlBswRefNr = 0"!][!//
#define ETHIF_ETH_GETCURRENTTIME(PhyCtrlPtr, TimeQualPtr, TimeStampPtr) \
Eth_GetCurrentTime((PhyCtrlPtr)->EthCtrlIdx, (TimeQualPtr), (TimeStampPtr))
[!ELSEIF "$EthIf_CtrlBswRefNr = 1"!][!//
#define ETHIF_ETH_GETCURRENTTIME(PhyCtrlPtr, TimeQualPtr, TimeStampPtr) \
Eth_[!"$driverExtension"!]_GetCurrentTime((PhyCtrlPtr)->EthCtrlIdx, (TimeQualPtr), (TimeStampPtr))
[!ELSE!][!//
#define ETHIF_ETH_GETCURRENTTIME(PhyCtrlPtr, TimeQualPtr, TimeStampPtr) \
EthIf_EthFuncConfig[(PhyCtrlPtr)->EthCtrlApiIdx].GetCurrentTime((PhyCtrlPtr)->EthCtrlIdx,\
(TimeQualPtr), (TimeStampPtr))
[!ENDIF!][!//

#if (defined ETHIF_ETH_ENABLEEGRESSTIMESTAMP)
#error ETHIF_ETH_ENABLEEGRESSTIMESTAMP is already defined
#endif
[!IF "$EthIf_CtrlBswRefNr = 0"!][!//
#define ETHIF_ETH_ENABLEEGRESSTIMESTAMP(PhyCtrlPtr, BufIdx) \
Eth_EnableEgressTimeStamp((PhyCtrlPtr)->EthCtrlIdx, (BufIdx))
[!ELSEIF "$EthIf_CtrlBswRefNr = 1"!][!//
#define ETHIF_ETH_ENABLEEGRESSTIMESTAMP(PhyCtrlPtr, BufIdx) \
Eth_[!"$driverExtension"!]_EnableEgressTimeStamp((PhyCtrlPtr)->EthCtrlIdx, (BufIdx))
[!ELSE!][!//
#define ETHIF_ETH_ENABLEEGRESSTIMESTAMP(PhyCtrlPtr, BufIdx) \
EthIf_EthFuncConfig[(PhyCtrlPtr)->EthCtrlApiIdx].EnableEgressTimeStamp((PhyCtrlPtr)->EthCtrlIdx,\
(BufIdx))
[!ENDIF!][!//

#if (defined ETHIF_ETH_GETEGRESSTIMESTAMP)
#error ETHIF_ETH_GETEGRESSTIMESTAMP is already defined
#endif
[!IF "$EthIf_CtrlBswRefNr = 0"!][!//
#define ETHIF_ETH_GETEGRESSTIMESTAMP(PhyCtrlPtr, BufIdx, TimeQualPtr, TimeStampPtr) \
Eth_GetEgressTimeStamp((PhyCtrlPtr)->EthCtrlIdx, (BufIdx), (TimeQualPtr), (TimeStampPtr))
[!ELSEIF "$EthIf_CtrlBswRefNr = 1"!][!//
#define ETHIF_ETH_GETEGRESSTIMESTAMP(PhyCtrlPtr, BufIdx, TimeQualPtr, TimeStampPtr) \
Eth_[!"$driverExtension"!]_GetEgressTimeStamp((PhyCtrlPtr)->EthCtrlIdx, (BufIdx), (TimeQualPtr), \
(TimeStampPtr))
[!ELSE!][!//
#define ETHIF_ETH_GETEGRESSTIMESTAMP(PhyCtrlPtr, BufIdx, TimeQualPtr, TimeStampPtr) \
EthIf_EthFuncConfig[(PhyCtrlPtr)->EthCtrlApiIdx].GetEgressTimeStamp((PhyCtrlPtr)->EthCtrlIdx,\
(BufIdx),(TimeQualPtr), (TimeStampPtr))
[!ENDIF!][!//

#if (defined ETHIF_ETH_GETINGRESSTIMESTAMP)
#error ETHIF_ETH_GETINGRESSTIMESTAMP is already defined
#endif
[!IF "$EthIf_CtrlBswRefNr = 0"!][!//
#define ETHIF_ETH_GETINGRESSTIMESTAMP(PhyCtrlPtr, DataPtr, TimeQualPtr, TimeStampPtr) \
Eth_GetIngressTimeStamp((PhyCtrlPtr)->EthCtrlIdx, (DataPtr), (TimeQualPtr), (TimeStampPtr))
[!ELSEIF "$EthIf_CtrlBswRefNr = 1"!][!//
#define ETHIF_ETH_GETINGRESSTIMESTAMP(PhyCtrlPtr, DataPtr, TimeQualPtr, TimeStampPtr) \
Eth_[!"$driverExtension"!]_GetIngressTimeStamp((PhyCtrlPtr)->EthCtrlIdx, (DataPtr), (TimeQualPtr), \
(TimeStampPtr))
[!ELSE!][!//
#define ETHIF_ETH_GETINGRESSTIMESTAMP(PhyCtrlPtr, DataPtr, TimeQualPtr, TimeStampPtr) \
EthIf_EthFuncConfig[(PhyCtrlPtr)->EthCtrlApiIdx].GetIngressTimeStamp((PhyCtrlPtr)->EthCtrlIdx,\
(DataPtr),(TimeQualPtr), (TimeStampPtr))
[!ENDIF!][!//

#if (defined ETHIF_ETH_SETCORRECTIONTIME)
#error ETHIF_ETH_SETCORRECTIONTIME is already defined
#endif
[!IF "$EthIf_CtrlBswRefNr = 0"!][!//
#define ETHIF_ETH_SETCORRECTIONTIME(PhyCtrlPtr, TimeOffsetPtr, RateRatioPtr) \
Eth_SetCorrectionTime((PhyCtrlPtr)->EthCtrlIdx, (TimeOffsetPtr), (RateRatioPtr))
[!ELSEIF "$EthIf_CtrlBswRefNr = 1"!][!//
#define ETHIF_ETH_SETCORRECTIONTIME(PhyCtrlPtr, TimeOffsetPtr, RateRatioPtr) \
Eth_[!"$driverExtension"!]_SetCorrectionTime((PhyCtrlPtr)->EthCtrlIdx, (TimeOffsetPtr), (RateRatioPtr))
[!ELSE!][!//
#define ETHIF_ETH_SETCORRECTIONTIME(PhyCtrlPtr, TimeOffsetPtr, RateRatioPtr) \
EthIf_EthFuncConfig[(PhyCtrlPtr)->EthCtrlApiIdx].SetCorrectionTime((PhyCtrlPtr)->EthCtrlIdx,\
(TimeOffsetPtr), (RateRatioPtr))
[!ENDIF!][!//

#if (defined ETHIF_ETH_SETGLOBALTIME)
#error ETHIF_ETH_SETGLOBALTIME is already defined
#endif
[!IF "$EthIf_CtrlBswRefNr = 0"!][!//
#define ETHIF_ETH_SETGLOBALTIME(PhyCtrlPtr, TimeStampPtr) \
Eth_SetGlobalTime((PhyCtrlPtr)->EthCtrlIdx, (TimeStampPtr))
[!ELSEIF "$EthIf_CtrlBswRefNr = 1"!][!//
#define ETHIF_ETH_SETGLOBALTIME(PhyCtrlPtr, TimeStampPtr) \
Eth_[!"$driverExtension"!]_SetGlobalTime((PhyCtrlPtr)->EthCtrlIdx, (TimeStampPtr))
[!ELSE!][!//
#define ETHIF_ETH_SETGLOBALTIME(PhyCtrlPtr, TimeStampPtr) \
EthIf_EthFuncConfig[(PhyCtrlPtr)->EthCtrlApiIdx].SetGlobalTime((PhyCtrlPtr)->EthCtrlIdx,\
(TimeStampPtr))
[!ENDIF!][!//

#if (defined ETHIF_ETH_RETRANSMIT)
#error ETHIF_ETH_RETRANSMIT is already defined
#endif
[!IF "$EthIf_CtrlBswRefNr = 0"!][!//
#define ETHIF_ETH_RETRANSMIT(PhyCtrlPtr, OrigCtrlIdx, BufIdxPtr, DataPtr, FrameType, \
LenByte, RetransmitInfoPtr) Eth_Retransmit((PhyCtrlPtr)->EthCtrlIdx,(OrigCtrlIdx), (BufIdxPtr), \
(DataPtr), (FrameType), (LenByte), (RetransmitInfoPtr))
[!ELSEIF "$EthIf_CtrlBswRefNr = 1"!][!//
#define ETHIF_ETH_RETRANSMIT(PhyCtrlPtr,OrigCtrlIdx, BufIdxPtr, DataPtr, FrameType, LenByte,\
RetransmitInfoPtr) Eth_[!"$driverExtension"!]_Retransmit((PhyCtrlPtr)->EthCtrlIdx, (OrigCtrlIdx), \
(BufIdxPtr), (DataPtr), (FrameType), (LenByte), (RetransmitInfoPtr))
[!ELSE!][!//
#define ETHIF_ETH_RETRANSMIT(PhyCtrlPtr,  OrigCtrlIdx, BufIdxPtr, DataPtr, FrameType, LenByte, \
RetransmitInfoPtr) EthIf_EthFuncConfig[(PhyCtrlPtr)->EthCtrlApiIdx].Retransmit\
((PhyCtrlPtr)->EthCtrlIdx,(OrigCtrlIdx),(BufIdxPtr), (DataPtr), (FrameType), (LenByte), \
(RetransmitInfoPtr))
[!ENDIF!][!//

/* --- Eth transceiver function definitions---------------------------------- */

#if (defined ETHIF_ETHTRCV_SETPHYLOOPBACKMODE)
#error ETHIF_ETHTRCV_SETPHYLOOPBACKMODE is already defined
#endif
[!IF "$EthIf_TrcvBswRefNr = 0"!][!//
#define ETHIF_ETHTRCV_SETPHYLOOPBACKMODE(EthIfTrvcPtr, Mode) \
EthTrcv_SetPhyLoopbackMode((EthIfTrvcPtr)->EthTrcvIdx, (Mode))
[!ELSEIF "$EthIf_TrcvBswRefNr = 1"!][!//
#define ETHIF_ETHTRCV_SETPHYLOOPBACKMODE(EthIfTrvcPtr, Mode) \
EthTrcv_[!"$trcvExtension"!]_SetPhyLoopbackMode((EthIfTrvcPtr)->EthTrcvIdx, (Mode))
[!ELSE!][!//
#define ETHIF_ETHTRCV_SETPHYLOOPBACKMODE(EthIfTrvcPtr, Mode) \
EthIf_EthTrcvFuncConfig[(EthIfTrvcPtr)->EthTrcvApiIdx].SetPhyLoopbackMode((EthIfTrvcPtr)->EthTrcvIdx,\
(Mode))
[!ENDIF!][!//

#if (defined ETHIF_ETHTRCV_SETPHYTXMODE)
#error ETHIF_ETHTRCV_SETPHYTXMODE is already defined
#endif
[!IF "$EthIf_TrcvBswRefNr = 0"!][!//
#define ETHIF_ETHTRCV_SETPHYTXMODE(EthIfTrvcPtr, Mode) \
EthTrcv_SetPhyTxMode((EthIfTrvcPtr)->EthTrcvIdx, (Mode))
[!ELSEIF "$EthIf_TrcvBswRefNr = 1"!][!//
#define ETHIF_ETHTRCV_SETPHYTXMODE(EthIfTrvcPtr, Mode) \
EthTrcv_[!"$trcvExtension"!]_SetPhyTxMode((EthIfTrvcPtr)->EthTrcvIdx, (Mode))
[!ELSE!][!//
#define ETHIF_ETHTRCV_SETPHYTXMODE(EthIfTrvcPtr, Mode) \
EthIf_EthTrcvFuncConfig[(EthIfTrvcPtr)->EthTrcvApiIdx].SetPhyTxMode((EthIfTrvcPtr)->EthTrcvIdx, (Mode))
[!ENDIF!][!//

#if (defined ETHIF_ETHTRCV_GETPHYSIGQUAL)
#error ETHIF_ETHTRCV_GETPHYSIGQUAL is already defined
#endif
[!IF "$EthIf_TrcvBswRefNr = 0"!][!//
#define ETHIF_ETHTRCV_GETPHYSIGQUAL(EthIfTrvcPtr, SignalQualityPtr) \
EthTrcv_GetPhySignalQuality((EthIfTrvcPtr)->EthTrcvIdx, (SignalQualityPtr))
[!ELSEIF "$EthIf_TrcvBswRefNr = 1"!][!//
#define ETHIF_ETHTRCV_GETPHYSIGQUAL(EthIfTrvcPtr, SignalQualityPtr) \
EthTrcv_[!"$trcvExtension"!]_GetPhySignalQuality((EthIfTrvcPtr)->EthTrcvIdx, (SignalQualityPtr))
[!ELSE!][!//
#define ETHIF_ETHTRCV_GETPHYSIGQUAL(EthIfTrvcPtr, SignalQualityPtr) \
EthIf_EthTrcvFuncConfig[(EthIfTrvcPtr)->EthTrcvApiIdx].GetPhySignalQuality((EthIfTrvcPtr)->EthTrcvIdx, \
(SignalQualityPtr))
[!ENDIF!][!//

#if (defined ETHIF_ETHTRCV_GETCABLEDIAGRESULT)
#error ETHIF_ETHTRCV_GETCABLEDIAGRESULT is already defined
#endif
[!IF "$EthIf_TrcvBswRefNr = 0"!][!//
#define ETHIF_ETHTRCV_GETCABLEDIAGRESULT(EthIfTrvcPtr, ResultPtr) \
EthTrcv_GetCableDiagnosticsResult((EthIfTrvcPtr)->EthTrcvIdx, (ResultPtr))
[!ELSEIF "$EthIf_TrcvBswRefNr = 1"!][!//
#define ETHIF_ETHTRCV_GETCABLEDIAGRESULT(EthIfTrvcPtr, ResultPtr) \
EthTrcv_[!"$trcvExtension"!]_GetCableDiagnosticsResult((EthIfTrvcPtr)->EthTrcvIdx, (ResultPtr))
[!ELSE!][!//
#define ETHIF_ETHTRCV_GETCABLEDIAGRESULT(EthIfTrvcPtr, ResultPtr) \
EthIf_EthTrcvFuncConfig[(EthIfTrvcPtr)->EthTrcvApiIdx].GetCableDiagnosticsResult(\
(EthIfTrvcPtr)->EthTrcvIdx, (ResultPtr))
[!ENDIF!][!//

#if (defined ETHIF_ETHTRCV_GETPHYIDENTIFIER)
#error ETHIF_ETHTRCV_GETPHYIDENTIFIER is already defined
#endif
[!IF "$EthIf_TrcvBswRefNr = 0"!][!//
#define ETHIF_ETHTRCV_GETPHYIDENTIFIER(EthIfTrvcPtr, OrgUniqueIdPtr, ModelNrPtr, RevisionNrPtr) \
EthTrcv_GetPhyIdentifier((EthIfTrvcPtr)->EthTrcvIdx, (OrgUniqueIdPtr), (ModelNrPtr), (RevisionNrPtr))
[!ELSEIF "$EthIf_TrcvBswRefNr = 1"!][!//
#define ETHIF_ETHTRCV_GETPHYIDENTIFIER(EthIfTrvcPtr, OrgUniqueIdPtr, ModelNrPtr, RevisionNrPtr) \
EthTrcv_[!"$trcvExtension"!]_GetPhyIdentifier((EthIfTrvcPtr)->EthTrcvIdx, (OrgUniqueIdPtr),(ModelNrPtr)\
,(RevisionNrPtr))
[!ELSE!][!//
#define ETHIF_ETHTRCV_GETPHYIDENTIFIER(EthIfTrvcPtr, OrgUniqueIdPtr, ModelNrPtr, RevisionNrPtr) \
EthIf_EthTrcvFuncConfig[(EthIfTrvcPtr)->EthTrcvApiIdx].GetPhyIdentifier(\
(EthIfTrvcPtr)->EthTrcvIdx, (OrgUniqueIdPtr),(ModelNrPtr), (RevisionNrPtr))
[!ENDIF!][!//

#if (defined ETHIF_ETHTRCV_SETPHYTESTMODE)
#error ETHIF_ETHTRCV_SETPHYTESTMODE is already defined
#endif
[!IF "$EthIf_TrcvBswRefNr = 0"!][!//
#define ETHIF_ETHTRCV_SETPHYTESTMODE(EthIfTrvcPtr, Mode) \
EthTrcv_SetPhyTestMode((EthIfTrvcPtr)->EthTrcvIdx, (Mode))
[!ELSEIF "$EthIf_TrcvBswRefNr = 1"!][!//
#define ETHIF_ETHTRCV_SETPHYTESTMODE(EthIfTrvcPtr, Mode) \
EthTrcv_[!"$trcvExtension"!]_SetPhyTestMode((EthIfTrvcPtr)->EthTrcvIdx, (Mode))
[!ELSE!][!//
#define ETHIF_ETHTRCV_SETPHYTESTMODE(EthIfTrvcPtr, Mode) \
EthIf_EthTrcvFuncConfig[(EthIfTrvcPtr)->EthTrcvApiIdx].SetPhyTestMode((EthIfTrvcPtr)->EthTrcvIdx, \
(Mode))
[!ENDIF!][!//

#if (defined ETHIF_ETHTRCV_SETTRCVWAKEUPMODE)
#error ETHIF_ETHTRCV_SETTRCVWAKEUPMODE is already defined
#endif
[!IF "$EthIf_TrcvBswRefNr = 0"!][!//
#define ETHIF_ETHTRCV_SETTRCVWAKEUPMODE(EthIfTrvcPtr, TrcvWakeupMode) \
EthTrcv_SetTransceiverWakeupMode((EthIfTrvcPtr)->EthTrcvIdx, (TrcvWakeupMode))
[!ELSEIF "$EthIf_TrcvBswRefNr = 1"!][!//
#define ETHIF_ETHTRCV_SETTRCVWAKEUPMODE(EthIfTrvcPtr, TrcvWakeupMode) \
EthTrcv_[!"$trcvExtension"!]_SetTransceiverWakeupMode((EthIfTrvcPtr)->EthTrcvIdx, (TrcvWakeupMode))
[!ELSE!][!//
#define ETHIF_ETHTRCV_SETTRCVWAKEUPMODE(EthIfTrvcPtr, TrcvWakeupMode) \
EthIf_EthTrcvFuncConfig[(EthIfTrvcPtr)->EthTrcvApiIdx].SetTransceiverWakeupMode(\
(EthIfTrvcPtr)->EthTrcvIdx, (TrcvWakeupMode))
[!ENDIF!][!//

#if (defined ETHIF_ETHTRCV_GETTRCVWAKEUPMODE)
#error ETHIF_ETHTRCV_GETTRCVWAKEUPMODE is already defined
#endif
[!IF "$EthIf_TrcvBswRefNr = 0"!][!//
#define ETHIF_ETHTRCV_GETTRCVWAKEUPMODE(EthIfTrvcPtr, TrcvWakeupModePtr) \
EthTrcv_GetTransceiverWakeupMode((EthIfTrvcPtr)->EthTrcvIdx, (TrcvWakeupModePtr))
[!ELSEIF "$EthIf_TrcvBswRefNr = 1"!][!//
#define ETHIF_ETHTRCV_GETTRCVWAKEUPMODE(EthIfTrvcPtr, TrcvWakeupModePtr) \
EthTrcv_[!"$trcvExtension"!]_GetTransceiverWakeupMode((EthIfTrvcPtr)->EthTrcvIdx, (TrcvWakeupModePtr))
[!ELSE!][!//
#define ETHIF_ETHTRCV_GETTRCVWAKEUPMODE(EthIfTrvcPtr, TrcvWakeupModePtr) \
EthIf_EthTrcvFuncConfig[(EthIfTrvcPtr)->EthTrcvApiIdx].GetTransceiverWakeupMode(\
(EthIfTrvcPtr)->EthTrcvIdx, (TrcvWakeupModePtr))
[!ENDIF!][!//

#if (defined ETHIF_ETHTRCV_CHECKWAKEUP)
#error ETHIF_ETHTRCV_CHECKWAKEUP is already defined
#endif
[!IF "$EthIf_TrcvBswRefNr = 0"!][!//
#define ETHIF_ETHTRCV_CHECKWAKEUP(EthIfTrvcPtr) \
EthTrcv_CheckWakeup((EthIfTrvcPtr)->EthTrcvIdx)
[!ELSEIF "$EthIf_TrcvBswRefNr = 1"!][!//
#define ETHIF_ETHTRCV_CHECKWAKEUP(EthIfTrvcPtr) \
EthTrcv_[!"$trcvExtension"!]_CheckWakeup((EthIfTrvcPtr)->EthTrcvIdx)
[!ELSE!][!//
#define ETHIF_ETHTRCV_CHECKWAKEUP(EthIfTrvcPtr) \
EthIf_EthTrcvFuncConfig[(EthIfTrvcPtr)->EthTrcvApiIdx].CheckWakeup((EthIfTrvcPtr)->EthTrcvIdx)
[!ENDIF!][!//

#if (defined ETHIF_ETHTRCV_SETTRCVMODE)
#error ETHIF_ETHTRCV_SETTRCVMODE is already defined
#endif
[!IF "$EthIf_TrcvBswRefNr = 0"!][!//
#define ETHIF_ETHTRCV_SETTRCVMODE(EthIfTrvcPtr, CtrlMode) \
EthTrcv_SetTransceiverMode((EthIfTrvcPtr)->EthTrcvIdx, (CtrlMode))
[!ELSEIF "$EthIf_TrcvBswRefNr = 1"!][!//
#define ETHIF_ETHTRCV_SETTRCVMODE(EthIfTrvcPtr, CtrlMode) \
EthTrcv_[!"$trcvExtension"!]_SetTransceiverMode((EthIfTrvcPtr)->EthTrcvIdx, (CtrlMode))
[!ELSE!][!//
#define ETHIF_ETHTRCV_SETTRCVMODE(EthIfTrvcPtr, CtrlMode) \
EthIf_EthTrcvFuncConfig[(EthIfTrvcPtr)->EthTrcvApiIdx].SetTransceiverMode((EthIfTrvcPtr)->EthTrcvIdx, \
(CtrlMode))
[!ENDIF!][!//

#if (defined ETHIF_ETHTRCV_GETTRCVMODE)
#error ETHIF_ETHTRCV_GETTRCVMODE is already defined
#endif
[!IF "$EthIf_TrcvBswRefNr = 0"!][!//
#define ETHIF_ETHTRCV_GETTRCVMODE(EthIfTrvcPtr, TrcvModePtr) \
EthTrcv_GetTransceiverMode((EthIfTrvcPtr)->EthTrcvIdx, (TrcvModePtr))
[!ELSEIF "$EthIf_TrcvBswRefNr = 1"!][!//
#define ETHIF_ETHTRCV_GETTRCVMODE(EthIfTrvcPtr, TrcvModePtr) \
EthTrcv_[!"$trcvExtension"!]_GetTransceiverMode((EthIfTrvcPtr)->EthTrcvIdx, (TrcvModePtr))
[!ELSE!][!//
#define ETHIF_ETHTRCV_GETTRCVMODE(EthIfTrvcPtr, TrcvModePtr) \
EthIf_EthTrcvFuncConfig[(EthIfTrvcPtr)->EthTrcvApiIdx].GetTransceiverMode((EthIfTrvcPtr)->EthTrcvIdx, \
(TrcvModePtr))
[!ENDIF!][!//

#if (defined ETHIF_ETHTRCV_GETLINKSTATE)
#error ETHIF_ETHTRCV_GETLINKSTATE is already defined
#endif
[!IF "$EthIf_TrcvBswRefNr = 0"!][!//
#define ETHIF_ETHTRCV_GETLINKSTATE(EthIfTrvcPtr, LinkStatePtr) \
EthTrcv_GetLinkState((EthIfTrvcPtr)->EthTrcvIdx, (LinkStatePtr))
[!ELSEIF "$EthIf_TrcvBswRefNr = 1"!][!//
#define ETHIF_ETHTRCV_GETLINKSTATE(EthIfTrvcPtr, LinkStatePtr) \
EthTrcv_[!"$trcvExtension"!]_GetLinkState((EthIfTrvcPtr)->EthTrcvIdx, (LinkStatePtr))
[!ELSE!][!//
#define ETHIF_ETHTRCV_GETLINKSTATE(EthIfTrvcPtr, LinkStatePtr) \
EthIf_EthTrcvFuncConfig[(EthIfTrvcPtr)->EthTrcvApiIdx].GetLinkState((EthIfTrvcPtr)->EthTrcvIdx, \
(LinkStatePtr))
[!ENDIF!][!//

#if (defined ETHIF_ETHTRCV_RUNCABLEDIAG)
#error ETHIF_ETHTRCV_RUNCABLEDIAG is already defined
#endif
[!IF "$EthIf_TrcvBswRefNr = 0"!][!//
#define ETHIF_ETHTRCV_RUNCABLEDIAG(EthIfTrvcPtr) \
EthTrcv_RunCableDiagnostic((EthIfTrvcPtr)->EthTrcvIdx)
[!ELSEIF "$EthIf_TrcvBswRefNr = 1"!][!//
#define ETHIF_ETHTRCV_RUNCABLEDIAG(EthIfTrvcPtr) \
EthTrcv_[!"$trcvExtension"!]_RunCableDiagnostic((EthIfTrvcPtr)->EthTrcvIdx)
[!ELSE!][!//
#define ETHIF_ETHTRCV_RUNCABLEDIAG(EthIfTrvcPtr) \
EthIf_EthTrcvFuncConfig[(EthIfTrvcPtr)->EthTrcvApiIdx].RunCableDiagnostic((EthIfTrvcPtr)->EthTrcvIdx)
[!ENDIF!][!//

/* --- Eth switch function definitions---------------------------------- */

#if (defined ETHIF_ETHSWT_TXADAPTFRAMETYPEBFLEN)
#error ETHIF_ETHSWT_TXADAPTFRAMETYPEBFLEN is already defined
#endif
[!IF "$EthIf_SwtBswRefNr = 0"!][!//
#define ETHIF_ETHSWT_TXADAPTFRAMETYPEBFLEN(CtrlIdx, FrameTypePtr, LenBytePtr) \
EthSwt_EthIfTxAdaptFrameTypeBufferLength((CtrlIdx), (FrameTypePtr), (LenBytePtr))
[!ELSE!][!//
#define ETHIF_ETHSWT_TXADAPTFRAMETYPEBFLEN(CtrlIdx, FrameTypePtr, LenBytePtr) \
EthSwt_[!"$swtExtension"!]_EthIfTxAdaptFrameTypeBufferLength((CtrlIdx), (FrameTypePtr), (LenBytePtr))
[!ENDIF!][!//

#if (defined ETHIF_ETHSWT_PREPROCESSRXFRAME)
#error ETHIF_ETHSWT_PREPROCESSRXFRAME is already defined
#endif
[!IF "$EthIf_SwtBswRefNr = 0"!][!//
#define ETHIF_ETHSWT_PREPROCESSRXFRAME(CtrlIdx, DataPtrPtr, LenPtr, PhysAddrPtr, FrameTypePtr) \
EthSwt_EthIfPreProcessRxFrame((CtrlIdx), (DataPtrPtr), (LenPtr), (PhysAddrPtr), (FrameTypePtr))
[!ELSE!][!//
#define ETHIF_ETHSWT_PREPROCESSRXFRAME(CtrlIdx, DataPtrPtr, LenPtr, PhysAddrPtr, FrameTypePtr) \
EthSwt_[!"$swtExtension"!]_EthIfPreProcessRxFrame((CtrlIdx), (DataPtrPtr), (LenPtr), (PhysAddrPtr),\
(FrameTypePtr))
[!ENDIF!][!//

#if (defined ETHIF_ETHSWT_PREPROCESSTXFRAME)
#error ETHIF_ETHSWT_PREPROCESSTXFRAME is already defined
#endif
[!IF "$EthIf_SwtBswRefNr = 0"!][!//
#define ETHIF_ETHSWT_PREPROCESSTXFRAME(CtrlIdx, FrameType, Priority, BufIdx, BufPtrPtr, \
LenBytePtr) EthSwt_EthIfPreProcessTxFrame((CtrlIdx), (FrameType), (Priority), (BufIdx), \
(BufPtrPtr), (LenBytePtr))
[!ELSE!][!//
#define ETHIF_ETHSWT_PREPROCESSTXFRAME(CtrlIdx, FrameType, Priority, BufIdx, BufPtrPtr,\
LenBytePtr) EthSwt_[!"$swtExtension"!]_EthIfPreProcessTxFrame((CtrlIdx), (FrameType), (Priority), \
(BufIdx), (BufPtrPtr), (LenBytePtr))
[!ENDIF!][!//

#if (defined ETHIF_ETHSWT_GETPORTSIGNALQUALITY)
#error ETHIF_ETHSWT_GETPORTSIGNALQUALITY is already defined
#endif
[!IF "$EthIf_SwtBswRefNr = 0"!][!//
#define ETHIF_ETHSWT_GETPORTSIGNALQUALITY(SwitchIdx, SwitchPortIdx, SignalQualityPtr) \
EthSwt_GetPortSignalQuality((SwitchIdx), (SwitchPortIdx), (SignalQualityPtr))
[!ELSE!][!//
#define ETHIF_ETHSWT_GETPORTSIGNALQUALITY(SwitchIdx, SwitchPortIdx, SignalQualityPtr) \
EthSwt_[!"$swtExtension"!]_GetPortSignalQuality((SwitchIdx), (SwitchPortIdx), (SignalQualityPtr))
[!ENDIF!][!//

#if (defined ETHIF_ETHSWT_GETARLTABLE)
#error ETHIF_ETHSWT_GETARLTABLE is already defined
#endif
[!IF "$EthIf_SwtBswRefNr = 0"!][!//
#define ETHIF_ETHSWT_GETARLTABLE(SwitchIdx, numberOfElements, ArlTable) \
EthSwt_GetArlTable((SwitchIdx), (numberOfElements), (ArlTable))
[!ELSE!][!//
#define ETHIF_ETHSWT_GETARLTABLE(SwitchIdx, numberOfElements, ArlTable) \
EthSwt_[!"$swtExtension"!]_GetArlTable((SwitchIdx), (numberOfElements), (ArlTable))
[!ENDIF!][!//

#if (defined ETHIF_ETHSWT_GETBUFFLEVEL)
#error ETHIF_ETHSWT_GETBUFFLEVEL is already defined
#endif
[!IF "$EthIf_SwtBswRefNr = 0"!][!//
#define ETHIF_ETHSWT_GETBUFFLEVEL(SwitchIdx, SwitchBufferLevelPtr) \
EthSwt_GetBufferLevel((SwitchIdx), (SwitchBufferLevelPtr))
[!ELSE!][!//
#define ETHIF_ETHSWT_GETBUFFLEVEL(SwitchIdx, SwitchBufferLevelPtr) \
EthSwt_[!"$swtExtension"!]_GetBufferLevel((SwitchIdx), (SwitchBufferLevelPtr))
[!ENDIF!][!//

#if (defined ETHIF_ETHSWT_GETCOUNTERVALUES)
#error ETHIF_ETHSWT_GETCOUNTERVALUES is already defined
#endif
[!IF "$EthIf_SwtBswRefNr = 0"!][!//
#define ETHIF_ETHSWT_GETCOUNTERVALUES(SwitchIdx, SwitchPortIdx, CounterPtr) \
EthSwt_GetCounterValues((SwitchIdx), (SwitchPortIdx), (CounterPtr))
[!ELSE!][!//
#define ETHIF_ETHSWT_GETCOUNTERVALUES(SwitchIdx, SwitchPortIdx, CounterPtr) \
EthSwt_[!"$swtExtension"!]_GetCounterValues((SwitchIdx), (SwitchPortIdx), (CounterPtr))
[!ENDIF!][!//

#if (defined ETHIF_ETHSWT_STORECONFIG)
#error ETHIF_ETHSWT_STORECONFIG is already defined
#endif
[!IF "$EthIf_SwtBswRefNr = 0"!][!//
#define ETHIF_ETHSWT_STORECONFIG(SwitchIdx) EthSwt_StoreConfiguration((SwitchIdx))
[!ELSE!][!//
#define ETHIF_ETHSWT_STORECONFIG(SwitchIdx) \
EthSwt_[!"$swtExtension"!]_StoreConfiguration((SwitchIdx))
[!ENDIF!][!//

#if (defined ETHIF_ETHSWT_RESETCONFIG)
#error ETHIF_ETHSWT_RESETCONFIG is already defined
#endif
[!IF "$EthIf_SwtBswRefNr = 0"!][!//
#define ETHIF_ETHSWT_RESETCONFIG(SwitchIdx) EthSwt_ResetConfiguration((SwitchIdx))
[!ELSE!][!//
#define ETHIF_ETHSWT_RESETCONFIG(SwitchIdx) \
EthSwt_[!"$swtExtension"!]_ResetConfiguration((SwitchIdx))
[!ENDIF!][!//

#if (defined ETHIF_ETHSWT_SETSWITCHPORTMODE)
#error ETHIF_ETHSWT_SETSWITCHPORTMODE is already defined
#endif
[!IF "$EthIf_SwtBswRefNr = 0"!][!//
#define ETHIF_ETHSWT_SETSWITCHPORTMODE(SwitchIdx, SwitchPortIdx, PortMode) \
EthSwt_SetSwitchPortMode((SwitchIdx), (SwitchPortIdx), (PortMode))
[!ELSE!][!//
#define ETHIF_ETHSWT_SETSWITCHPORTMODE(SwitchIdx, SwitchPortIdx, PortMode) \
EthSwt_[!"$swtExtension"!]_SetSwitchPortMode((SwitchIdx), (SwitchPortIdx), (PortMode))
[!ENDIF!][!//

#if (defined ETHIF_ETHSWT_GETLINKSTATE)
#error ETHIF_ETHSWT_GETLINKSTATE is already defined
#endif
[!IF "$EthIf_SwtBswRefNr = 0"!][!//
#define ETHIF_ETHSWT_GETLINKSTATE(SwitchIdx, SwitchPortIdx, LinkStatePtr) \
EthSwt_GetLinkState((SwitchIdx), (SwitchPortIdx), (LinkStatePtr))
[!ELSE!][!//
#define ETHIF_ETHSWT_GETLINKSTATE(SwitchIdx, SwitchPortIdx, LinkStatePtr) \
EthSwt_[!"$swtExtension"!]_GetLinkState((SwitchIdx), (SwitchPortIdx), (LinkStatePtr))
[!ENDIF!][!//

#if (defined ETHIF_ETHSWT_PORTRUNCABLEDIAG)
#error ETHIF_ETHSWT_PORTRUNCABLEDIAG is already defined
#endif
[!IF "$EthIf_SwtBswRefNr = 0"!][!//
#define ETHIF_ETHSWT_PORTRUNCABLEDIAG(SwitchIdx, PortIdx) \
EthSwt_PortRunCableDiagnostic((SwitchIdx), (PortIdx))
[!ELSE!][!//
#define ETHIF_ETHSWT_PORTRUNCABLEDIAG(SwitchIdx, PortIdx) \
EthSwt_[!"$swtExtension"!]_PortRunCableDiagnostic((SwitchIdx), (PortIdx))
[!ENDIF!][!//

/*==================[type definitions]=======================================*/

[!IF "$EthIf_CtrlBswRefNr > 1"!][!//

/** \brief definition of pointer to Eth_DataType */
typedef Eth_DataType* Eth_DataPtrType;

/* --- Eth controller function definitions --------------------------------- */

/** \brief Pointer to function Eth_GetControllerMode()
 **
 ** Function pointer type for the Eth controller function Eth_GetControllerMode().
 */
typedef P2FUNC(Std_ReturnType, ETH_CODE, EthIf_EthGetControllerModeFuncPtrType)
(
  uint8 CtrlIdx,
  P2VAR(Eth_ModeType, AUTOMATIC, ETH_APPL_DATA) CtrlModePtr
);

/** \brief Pointer to function Eth_GetPhysAddr()
 **
 ** Function pointer type for the Eth controller function Eth_GetPhysAddr().
 */
typedef P2FUNC(void, ETH_CODE, EthIf_EthGetPhysAddrFuncPtrType)
(
  uint8 CtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETH_APPL_DATA) PhysAddrPtr
);

/** \brief Pointer to function Eth_ProvideTxBuffer()
 **
 ** Function pointer type for the Eth controller function Eth_ProvideTxBuffer().
 */
typedef P2FUNC(BufReq_ReturnType, ETH_CODE, EthIf_EthProvideTxBufferFuncPtrType)
(
  uint8 CtrlIdx,
  uint8 Priority,
  P2VAR(Eth_BufIdxType, AUTOMATIC, ETH_APPL_DATA) BufIdxPtr,
  P2VAR(Eth_DataPtrType, AUTOMATIC, ETH_APPL_DATA) BufPtr,
  P2VAR(uint16, AUTOMATIC, ETH_APPL_DATA) LenBytePtr
);

[!IF "$EthIf_MiiApiEnable = 'true'"!]
/** \brief Pointer to function Eth_ReadMii()
 **
 ** Function pointer type for the Eth controller function Eth_ReadMii().
 */
typedef P2FUNC(Eth_ReturnType, ETH_CODE, EthIf_EthReadMiiFuncPtrType)
(
  uint8 CtrlIdx,
  uint8 TrcvIdx,
  uint8 RegIdx,
  P2VAR(uint16, AUTOMATIC, ETH_APPL_DATA) RegValPtr
);

/** \brief Pointer to function Eth_WriteMii()
 **
 ** Function pointer type for the Eth controller function Eth_WriteMii().
 */
typedef P2FUNC(Eth_ReturnType, ETH_CODE, EthIf_EthWriteMiiFuncPtrType)
(
  uint8 CtrlIdx,
  uint8 TrcvIdx,
  uint8 RegIdx,
  uint16 RegVal
);
[!ENDIF!][!// EthIf_MiiApiEnable

/** \brief Pointer to function Eth_Receive()
 **
 ** Function pointer type for the Eth controller function Eth_Receive().
 */
typedef P2FUNC(void, ETH_CODE, EthIf_EthReceiveFuncPtrType)
(
  uint8 CtrlIdx,
  uint8 FifoIdx,
  P2VAR(Eth_RxStatusType, AUTOMATIC, ETH_APPL_DATA) RxStatus
);

/** \brief Pointer to function Eth_SetControllerMode()
 **
 ** Function pointer type for the Eth controller function Eth_SetControllerMode().
 */
typedef P2FUNC(Std_ReturnType, ETH_CODE, EthIf_EthSetControllerModeFuncPtrType)
(
  uint8 CtrlIdx,
  Eth_ModeType CtrlMode
);

/** \brief Pointer to function Eth_Transmit()
 **
 ** Function pointer type for the Eth controller function Eth_Transmit().
 */
typedef P2FUNC(Std_ReturnType, ETH_CODE, EthIf_EthTransmitFuncPtrType)
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx,
  Eth_FrameType FrameType,
  boolean TxConfirmation,
  uint16 LenByte,
  P2CONST(uint8, AUTOMATIC, ETH_APPL_DATA) PhysAddrPtr
);

/** \brief Pointer to function Eth_TxConfirmation()
 **
 ** Function pointer type for the Eth controller function Eth_TxConfirmation().
 */
typedef P2FUNC(void, ETH_CODE, EthIf_EthTxConfirmationFuncPtrType)
(
  uint8 CtrlIdx
);

/** \brief Pointer to function Eth_SetPhysAddr()
 **
 ** Function pointer type for the Eth controller function Eth_SetPhysAddr().
 */
typedef P2FUNC(void, ETH_CODE, EthIf_EthSetPhysAddrFuncPtrType)
(
  uint8 CtrlIdx,
  P2CONST(uint8, AUTOMATIC, EBSTUBS_APPL_CONST) PhysAddrPtr
);

[!IF "$EthIf_UpdatePhysAddrFilterApiEnable = 'true'"!]
/** \brief Pointer to function Eth_UpdatePhysAddrFilter()
 **
 ** Function pointer type for the Eth controller function Eth_UpdatePhysAddrFilter().
 */
typedef P2FUNC(Std_ReturnType, ETH_CODE, EthIf_EthUpdatePhysAddrFilterFuncPtrType)
(
  uint8 CtrlIdx,
  P2CONST(uint8, AUTOMATIC, EBSTUBS_APPL_CONST) PhysAddrPtr,
  Eth_FilterActionType Action
);
[!ENDIF!][!//

[!IF "$EthIf_GlobalTimeApiEnable = 'true'"!]
/** \brief Pointer to function Eth_GetCurrentTime()
 **
 ** Function pointer type for the Eth controller function Eth_GetCurrentTime().
 */
typedef P2FUNC(Std_ReturnType, ETH_CODE, EthIf_EthGetCurrentTimeFuncPtrType)
(
  uint8 CtrlIdx,
  P2VAR(Eth_TimeStampQualType, AUTOMATIC, ETH_APPL_DATA) TimeQualPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETH_APPL_DATA) TimeStampPtr
);

/** \brief Pointer to function Eth_EnableEgressTimeStamp()
 **
 ** Function pointer type for the Eth controller function Eth_EnableEgressTimeStamp().
 */
typedef P2FUNC(void, ETH_CODE, EthIf_EthEnableEgressTimeStampFuncPtrType)
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx
);

/** \brief Pointer to function Eth_GetEgressTimeStamp()
 **
 ** Function pointer type for the Eth controller function Eth_GetEgressTimeStamp().
 */
typedef P2FUNC(void, ETH_CODE, EthIf_EthGetEgressTimeStampFuncPtrType)
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx,
  P2VAR(Eth_TimeStampQualType, AUTOMATIC, ETH_APPL_DATA) TimeQualPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETH_APPL_DATA) TimeStampPtr
);

/** \brief Pointer to function Eth_GetIngressTimeStamp()
 **
 ** Function pointer type for the Eth controller function Eth_GetIngressTimeStamp().
 */
typedef P2FUNC(void, ETH_CODE, EthIf_EthGetIngressTimeStampFuncPtrType)
(
  uint8 CtrlIdx,
  P2CONST(Eth_DataType, AUTOMATIC, ETH_APPL_CONST) DataPtr,
  P2VAR(Eth_TimeStampQualType, AUTOMATIC, ETH_APPL_DATA) TimeQualPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETH_APPL_DATA) TimeStampPtr
);

/** \brief Pointer to function Eth_SetCorrectionTime()
 **
 ** Function pointer type for the Eth controller function Eth_SetCorrectionTime().
 */
typedef P2FUNC(void, ETH_CODE, EthIf_EthSetCorrectionTimeFuncPtrType)
(
  uint8 CtrlIdx,
  P2CONST(Eth_TimeIntDiffType, AUTOMATIC, ETH_APPL_DATA) TimeOffsetPtr,
  P2CONST(Eth_RateRatioType, AUTOMATIC, ETH_APPL_DATA) RateRatioPtr
);

/** \brief Pointer to function Eth_SetGlobalTime()
 **
 ** Function pointer type for the Eth controller function Eth_SetGlobalTime().
 */
typedef P2FUNC(Std_ReturnType, ETH_CODE, EthIf_EthSetGlobalTimeFuncPtrType)
(
  uint8 CtrlIdx,
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETH_APPL_DATA) TimeStampPtr
);
[!ENDIF!][!//

[!IF "$EthIf_EthIfRetransmitApiEnable = 'true'"!][!//
/** \brief Pointer to function Eth_Retransmit()
 **
 ** Function pointer type for the Eth controller function Eth_Retransmit().
 */
typedef P2FUNC(Std_ReturnType, ETH_CODE, EthIf_EthRetransmitFuncPtrType)
(
  uint8 CtrlIdx,
  uint8 OrigCtrlIdx,
  P2VAR(Eth_BufIdxType, AUTOMATIC, ETH_APPL_DATA) BufIdxPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  Eth_FrameType FrameType,
  uint16 LenByte,
  P2CONST(Eth_RetransmitInfoType, AUTOMATIC, ETH_APPL_DATA) RetransmitInfoPtr
);
[!ENDIF!][!//
[!ENDIF!][!//

[!IF "$EthIf_TrcvBswRefNr > 1"!][!//

/* --- Eth transceiver function definitions---------------------------------- */

/** \brief Pointer to function EthTrcv_SetPhyLoopbackMode()
 **
 ** Function pointer type for the Eth transceiver function EthTrcv_SetPhyLoopbackMode().
 */
typedef P2FUNC(Std_ReturnType, ETHTRCV_CODE, EthIf_TrcvSetPhyLoopbackModeFuncPtrType)
(
  uint8 TrcvIdx,
  EthTrcv_PhyLoopbackModeType Mode
);

/** \brief Pointer to function EthTrcv_SetPhyTxMode()
 **
 ** Function pointer type for the Eth transceiver function EthTrcv_SetPhyTxMode().
 */
typedef P2FUNC(Std_ReturnType, ETHTRCV_CODE, EthIf_TrcvSetPhyTxModeFuncPtrType)
(
  uint8 TrcvIdx,
  EthTrcv_PhyTxModeType Mode
);

/** \brief Pointer to function EthTrcv_GetPhySignalQuality()
 **
 ** Function pointer type for the Eth transceiver function EthTrcv_GetPhySignalQuality().
 */
typedef P2FUNC(Std_ReturnType, ETHTRCV_CODE, EthIf_TrcvGetPhySignalQualityFuncPtrType)
(
  uint8 TrcvIdx,
  P2VAR(uint32, AUTOMATIC, ETHTRCV_APPL_DATA) SignalQualityPtr
);

/** \brief Pointer to function EthTrcv_GetCableDiagnosticsResult()
 **
 ** Function pointer type for the Eth transceiver function EthTrcv_GetCableDiagnosticsResult().
 */
typedef P2FUNC(Std_ReturnType, ETHTRCV_CODE, EthIf_TrcvGetCableDiagnosticsResultFuncPtrType)
(
  uint8 TrcvIdx,
  P2VAR(EthTrcv_CableDiagResultType, AUTOMATIC, ETHTRCV_APPL_DATA) ResultPtr
);

/** \brief Pointer to function EthTrcv_GetPhyIdentifier()
 **
 ** Function pointer type for the Eth transceiver function EthTrcv_GetPhyIdentifier().
 */
typedef P2FUNC(Std_ReturnType, ETHTRCV_CODE, EthIf_TrcvGetPhyIdentifierFuncPtrType)
(
  uint8 TrcvIdx,
  P2VAR(uint32, AUTOMATIC, ETHTRCV_APPL_DATA) OrgUniqueIdPtr,
  P2VAR(uint8, AUTOMATIC, ETHTRCV_APPL_DATA) ModelNrPtr,
  P2VAR(uint8, AUTOMATIC, ETHTRCV_APPL_DATA) RevisionNrPtr
);

/** \brief Pointer to function EthTrcv_SetPhyTestMode()
 **
 ** Function pointer type for the Eth transceiver function EthTrcv_SetPhyTestMode().
 */
typedef P2FUNC(Std_ReturnType, ETHTRCV_CODE, EthIf_TrcvSetPhyTestModeFuncPtrType)
(
  uint8 TrcvIdx,
  EthTrcv_PhyTestModeType Mode
);

[!IF "$EthIf_WakeUpSupportApi = 'true'"!][!//
/** \brief Pointer to function EthTrcv_SetTransceiverWakeupMode()
 **
 ** Function pointer type for the Eth transceiver function EthTrcv_SetTransceiverWakeupMode().
 */
typedef P2FUNC(Std_ReturnType, ETHTRCV_CODE, EthIf_TrcvSetTransceiverWakeupModeFuncPtrType)
(
  uint8 TrcvIdx,
  EthTrcv_WakeupModeType TrcvWakeupMode
);

/** \brief Pointer to function EthTrcv_CheckWakeup()
 **
 ** Function pointer type for the Eth transceiver function EthTrcv_CheckWakeup().
 */
typedef P2FUNC(Std_ReturnType, ETHTRCV_CODE, EthIf_TrcvCheckWakeupFuncPtrType)
(
  uint8 TrcvIdx
);

[!IF "$EthIf_GetTransceiverWakeupModeApi = 'true'"!]
/** \brief Pointer to function EthTrcv_GetTransceiverWakeupMode()
 **
 ** Function pointer type for the Eth transceiver function EthTrcv_GetTransceiverWakeupMode().
 */
typedef P2FUNC(Std_ReturnType, ETHTRCV_CODE, EthIf_TrcvGetTransceiverWakeupModeFuncPtrType)
(
  uint8 TrcvIdx,
  P2VAR(EthTrcv_WakeupModeType, AUTOMATIC, ETHTRCV_APPL_DATA) TrcvWakeupModePtr
);
[!ENDIF!][!// EthIf_GetTransceiverWakeupModeApi
[!ENDIF!][!// EthIf_WakeUpSupportApi

/** \brief Pointer to function EthTrcv_SetTransceiverMode()
 **
 ** Function pointer type for the Eth transceiver function EthTrcv_SetTransceiverMode().
 */
typedef P2FUNC(Std_ReturnType, ETHTRCV_CODE, EthIf_TrcvSetTransceiverModeFuncPtrType)
(
  uint8 TrcvIdx,
  EthTrcv_ModeType CtrlMode
);

/** \brief Pointer to function EthTrcv_GetTransceiverMode()
 **
 ** Function pointer type for the Eth transceiver function EthTrcv_GetTransceiverMode().
 */
typedef P2FUNC(Std_ReturnType, ETHTRCV_CODE, EthIf_TrcvGetTransceiverModeFuncPtrType)
(
  uint8 TrcvIdx,
  P2VAR(EthTrcv_ModeType, AUTOMATIC, ETHTRCV_APPL_DATA) TrcvModePtr
);

/** \brief Pointer to function EthTrcv_GetLinkState()
 **
 ** Function pointer type for the Eth transceiver function EthTrcv_GetLinkState().
 */
typedef P2FUNC(Std_ReturnType, ETHTRCV_CODE, EthIf_TrcvGetLinkStateFuncPtrType)
(
  uint8 TrcvIdx,
  P2VAR(EthTrcv_LinkStateType, AUTOMATIC, ETHTRCV_APPL_DATA) LinkStatePtr
);
[!ENDIF!][!// EthIf_TrcvBswRefNr > 1

[!IF "$EthIf_CtrlBswRefNr > 1"!][!//
/** \brief Eth function pointers
 **/
typedef struct
{
  EthIf_EthGetControllerModeFuncPtrType GetControllerMode;
  EthIf_EthGetPhysAddrFuncPtrType GetPhysAddr;
  EthIf_EthProvideTxBufferFuncPtrType ProvideTxBuffer;
[!IF "$EthIf_MiiApiEnable = 'true'"!][!//
  EthIf_EthReadMiiFuncPtrType ReadMii;
  EthIf_EthWriteMiiFuncPtrType WriteMii;
[!ENDIF!][!//
  EthIf_EthReceiveFuncPtrType Receive;
  EthIf_EthSetControllerModeFuncPtrType SetControllerMode;
  EthIf_EthTransmitFuncPtrType Transmit;
  EthIf_EthTxConfirmationFuncPtrType TxConfirmation;
  EthIf_EthSetPhysAddrFuncPtrType SetPhysAddr;
[!IF "$EthIf_UpdatePhysAddrFilterApiEnable = 'true'"!][!//
  EthIf_EthUpdatePhysAddrFilterFuncPtrType UpdatePhysAddrFilter;
[!ENDIF!][!//
[!IF "$EthIf_GlobalTimeApiEnable = 'true'"!][!//
  EthIf_EthGetCurrentTimeFuncPtrType GetCurrentTime;
  EthIf_EthEnableEgressTimeStampFuncPtrType EnableEgressTimeStamp;
  EthIf_EthGetEgressTimeStampFuncPtrType GetEgressTimeStamp;
  EthIf_EthGetIngressTimeStampFuncPtrType GetIngressTimeStamp;
  EthIf_EthSetCorrectionTimeFuncPtrType SetCorrectionTime;
  EthIf_EthSetGlobalTimeFuncPtrType SetGlobalTime;
[!ENDIF!][!//
[!IF "$EthIf_EthIfRetransmitApiEnable = 'true'"!][!//
  EthIf_EthRetransmitFuncPtrType Retransmit;
[!ENDIF!][!//
} EthIf_EthFuncConfigType;
[!ENDIF!][!//

[!IF "$EthIf_TrcvBswRefNr > 1"!][!//
/** \brief EthTrcv function pointers
**/
typedef struct
{
  EthIf_TrcvSetPhyLoopbackModeFuncPtrType SetPhyLoopbackMode;
  EthIf_TrcvSetPhyTxModeFuncPtrType SetPhyTxMode;
  EthIf_TrcvGetPhySignalQualityFuncPtrType GetPhySignalQuality;
  EthIf_TrcvGetCableDiagnosticsResultFuncPtrType GetCableDiagnosticsResult;
  EthIf_TrcvGetPhyIdentifierFuncPtrType GetPhyIdentifier;
  EthIf_TrcvSetPhyTestModeFuncPtrType SetPhyTestMode;
[!IF "$EthIf_WakeUpSupportApi = 'true'"!][!//
  EthIf_TrcvSetTransceiverWakeupModeFuncPtrType SetTransceiverWakeupMode;
  EthIf_TrcvCheckWakeupFuncPtrType CheckWakeup;
[!IF "$EthIf_GetTransceiverWakeupModeApi = 'true'"!][!//
  EthIf_TrcvGetTransceiverWakeupModeFuncPtrType GetTransceiverWakeupMode;
[!ENDIF!][!//
[!ENDIF!][!//
  EthIf_TrcvSetTransceiverModeFuncPtrType SetTransceiverMode;
  EthIf_TrcvGetTransceiverModeFuncPtrType GetTransceiverMode;
  EthIf_TrcvGetLinkStateFuncPtrType GetLinkState;
} EthIf_EthTrcvFuncConfigType;
[!ENDIF!][!//

#endif /* ifndef ETHIF_DRVTYPES_H */
/*==================[end of file]============================================*/
[!ENDCODE!]