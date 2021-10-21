/**
 * \file
 *
 * \brief AUTOSAR Eth
 *
 * This file contains the implementation of the AUTOSAR
 * module Eth.
 *
 * \version 0.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2011 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef ETH_TRACE_H
#define ETH_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif

#include <Eth_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function Eth_IsValidConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ISVALIDCONFIG_ENTRY(voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ISVALIDCONFIG_EXIT
/** \brief Exit point of function Eth_IsValidConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_ISVALIDCONFIG_EXIT(Retval,voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_INIT_ENTRY
/** \brief Entry point of function Eth_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_INIT_ENTRY(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_INIT_EXIT
/** \brief Exit point of function Eth_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_INIT_EXIT(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_CONTROLLERINITASR422_ENTRY
/** \brief Entry point of function Eth_ControllerInitAsr422()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_CONTROLLERINITASR422_ENTRY(CtrlIdx,CfgIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_CONTROLLERINITASR422_EXIT
/** \brief Exit point of function Eth_ControllerInitAsr422()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_CONTROLLERINITASR422_EXIT(Result,CtrlIdx,CfgIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_MIIINITASR430B_ENTRY
/** \brief Entry point of function Eth_MiiInitAsr430b()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_MIIINITASR430B_ENTRY(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_MIIINITASR430B_EXIT
/** \brief Exit point of function Eth_MiiInitAsr430b()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_MIIINITASR430B_EXIT(Result,CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_CONTROLLERINITASR430B_ENTRY
/** \brief Entry point of function Eth_ControllerInitAsr430b()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_CONTROLLERINITASR430B_ENTRY(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_CONTROLLERINITASR430B_EXIT
/** \brief Exit point of function Eth_ControllerInitAsr430b()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_CONTROLLERINITASR430B_EXIT(Result,CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SETCONTROLLERMODE_ENTRY
/** \brief Entry point of function Eth_SetControllerMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SETCONTROLLERMODE_ENTRY(CtrlIdx,CtrlMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SETCONTROLLERMODE_EXIT
/** \brief Exit point of function Eth_SetControllerMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_SETCONTROLLERMODE_EXIT(Result,CtrlIdx,CtrlMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETCONTROLLERMODE_ENTRY
/** \brief Entry point of function Eth_GetControllerMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_GETCONTROLLERMODE_ENTRY(CtrlIdx,CtrlModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETCONTROLLERMODE_EXIT
/** \brief Exit point of function Eth_GetControllerMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_GETCONTROLLERMODE_EXIT(Result,CtrlIdx,CtrlModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETPHYSADDR_ENTRY
/** \brief Entry point of function Eth_GetPhysAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_GETPHYSADDR_ENTRY(CtrlIdx,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETPHYSADDR_EXIT
/** \brief Exit point of function Eth_GetPhysAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_GETPHYSADDR_EXIT(CtrlIdx,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SETPHYSADDR_ENTRY
/** \brief Entry point of function Eth_SetPhysAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SETPHYSADDR_ENTRY(CtrlIdx,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SETPHYSADDR_EXIT
/** \brief Exit point of function Eth_SetPhysAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SETPHYSADDR_EXIT(CtrlIdx,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_UPDATEPHYSADDRFILTER_ENTRY
/** \brief Entry point of function Eth_UpdatePhysAddrFilter()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_UPDATEPHYSADDRFILTER_ENTRY(CtrlIdx,PhysAddrPtr,Action)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_UPDATEPHYSADDRFILTER_EXIT
/** \brief Exit point of function Eth_UpdatePhysAddrFilter()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_UPDATEPHYSADDRFILTER_EXIT(Result,CtrlIdx,PhysAddrPtr,Action)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_WRITEMII_ENTRY
/** \brief Entry point of function Eth_WriteMii()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_WRITEMII_ENTRY(CtrlIdx,TrcvIdx,RegIdx,RegVal)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_WRITEMII_EXIT
/** \brief Exit point of function Eth_WriteMii()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_WRITEMII_EXIT(Result,CtrlIdx,TrcvIdx,RegIdx,RegVal)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_READMII_ENTRY
/** \brief Entry point of function Eth_ReadMii()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_READMII_ENTRY(CtrlIdx,TrcvIdx,RegIdx,RegValPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_READMII_EXIT
/** \brief Exit point of function Eth_ReadMii()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_READMII_EXIT(Result,CtrlIdx,TrcvIdx,RegIdx,RegValPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETCOUNTERSTATE_ENTRY
/** \brief Entry point of function Eth_GetCounterState()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_GETCOUNTERSTATE_ENTRY(CtrlIdx,CtrlOffs,CtrlValPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETCOUNTERSTATE_EXIT
/** \brief Exit point of function Eth_GetCounterState()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_GETCOUNTERSTATE_EXIT(CtrlIdx,CtrlOffs,CtrlValPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETCOUNTERVALUES_ENTRY
/** \brief Entry point of function Eth_GetCounterValues()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_GETCOUNTERVALUES_ENTRY(CtrlIdx,CounterPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETCOUNTERVALUES_EXIT
/** \brief Exit point of function Eth_GetCounterValues()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_GETCOUNTERVALUES_EXIT(Result,CtrlIdx,CounterPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETRXSTATS_ENTRY
/** \brief Entry point of function Eth_GetRxStats()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_GETRXSTATS_ENTRY(CtrlIdx,RxStats)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETRXSTATS_EXIT
/** \brief Exit point of function Eth_GetRxStats()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_GETRXSTATS_EXIT(Result,CtrlIdx,RxStats)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETTXSTATS_ENTRY
/** \brief Entry point of function Eth_GetTxStats()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_GETTXSTATS_ENTRY(CtrlIdx,TxStats)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETTXSTATS_EXIT
/** \brief Exit point of function Eth_GetTxStats()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_GETTXSTATS_EXIT(Result,CtrlIdx,TxStats)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETTXERRORCOUNTERVALUES_ENTRY
/** \brief Entry point of function Eth_GetTxErrorCounterValues()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_GETTXERRORCOUNTERVALUES_ENTRY(CtrlIdx,TxErrorCounterValues)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETTXERRORCOUNTERVALUES_EXIT
/** \brief Exit point of function Eth_GetTxErrorCounterValues()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_GETTXERRORCOUNTERVALUES_EXIT(Result,CtrlIdx,TxErrorCounterValues)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_PROVIDETXBUFFERASR422_ENTRY
/** \brief Entry point of function Eth_ProvideTxBufferAsr422()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_PROVIDETXBUFFERASR422_ENTRY(CtrlIdx,BuIdxPtr,BufPtr, LenBytePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_PROVIDETXBUFFERASR422_EXIT
/** \brief Exit point of function Eth_ProvideTxBufferAsr422()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_PROVIDETXBUFFERASR422_EXIT(BufReq,CtrlIdx,BuIdxPtr,BufPtr, LenBytePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_PROVIDETXBUFFER_ENTRY
/** \brief Entry point of function Eth_ProvideTxBuffer()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_PROVIDETXBUFFER_ENTRY(CtrlIdx,Priority,BufIdxPtr, BufPtr,LenBytePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_PROVIDETXBUFFER_EXIT
/** \brief Exit point of function Eth_ProvideTxBuffer()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_PROVIDETXBUFFER_EXIT(BufReq,CtrlIdx,Priority,BufIdxPtr, BufPtr,LenBytePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_TRANSMIT_ENTRY
/** \brief Entry point of function Eth_Transmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_TRANSMIT_ENTRY(CtrlIdx,BufIdx,FrameType,TxConfirmation,LenByte,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_TRANSMIT_EXIT
/** \brief Exit point of function Eth_Transmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_TRANSMIT_EXIT(Result,CtrlIdx,BufIdx,FrameType,TxConfirmation,LenByte,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_RETRANSMIT_ENTRY
/** \brief Entry point of function Eth_Retransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_RETRANSMIT_ENTRY(CtrlIdx,OrigCtrlIdx,BufIdxPtr,DataPtr,FrameType,LenByte,RetransmitInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_RETRANSMIT_EXIT
/** \brief Exit point of function Eth_Retransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_RETRANSMIT_EXIT(Result,CtrlIdx,OrigCtrlIdx,BufIdxPtr,DataPtr,FrameType,LenByte,RetransmitInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_TRANSMITBUFLIST_ENTRY
/** \brief Entry point of function Eth_TransmitBufList()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_TRANSMITBUFLIST_ENTRY(CtrlIdx,Priority,BufListPtr,BufListNo,UserPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_TRANSMITBUFLIST_EXIT
/** \brief Exit point of function Eth_TransmitBufList()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_TRANSMITBUFLIST_EXIT(Result,CtrlIdx,Priority,BufListPtr,BufListNo,UserPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_RECEIVEASR422_ENTRY
/** \brief Entry point of function Eth_ReceiveAsr422()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_RECEIVEASR422_ENTRY(CtrlIdx,RxStatus)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_RECEIVEASR422_EXIT
/** \brief Exit point of function Eth_ReceiveAsr422()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_RECEIVEASR422_EXIT(CtrlIdx,RxStatus)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_RECEIVEASR430_ENTRY
/** \brief Entry point of function Eth_ReceiveAsr430()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_RECEIVEASR430_ENTRY(CtrlIdx,FifoIdx,RxStatus)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_RECEIVEASR430_EXIT
/** \brief Exit point of function Eth_ReceiveAsr430()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_RECEIVEASR430_EXIT(CtrlIdx,FifoIdx,RxStatus)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_TXCONFIRMATION_ENTRY
/** \brief Entry point of function Eth_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_TXCONFIRMATION_ENTRY(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_TXCONFIRMATION_EXIT
/** \brief Exit point of function Eth_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_TXCONFIRMATION_EXIT(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETVERSIONINFO_ENTRY
/** \brief Entry point of function Eth_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_GETVERSIONINFO_ENTRY(VersionInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETVERSIONINFO_EXIT
/** \brief Exit point of function Eth_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_GETVERSIONINFO_EXIT(VersionInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETCURRENTTIME_ENTRY
/** \brief Entry point of function Eth_GetCurrentTime()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_GETCURRENTTIME_ENTRY(CtrlIdx,timeQualPtr,timeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETCURRENTTIME_EXIT
/** \brief Exit point of function Eth_GetCurrentTime()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_GETCURRENTTIME_EXIT(FunctionSuccess,CtrlIdx,timeQualPtr,timeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ENABLEEGRESSTIMESTAMP_ENTRY
/** \brief Entry point of function Eth_EnableEgressTimeStamp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ENABLEEGRESSTIMESTAMP_ENTRY(CtrlIdx,BufIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ENABLEEGRESSTIMESTAMP_EXIT
/** \brief Exit point of function Eth_EnableEgressTimeStamp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ENABLEEGRESSTIMESTAMP_EXIT(CtrlIdx,BufIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETEGRESSTIMESTAMP_ENTRY
/** \brief Entry point of function Eth_GetEgressTimeStamp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_GETEGRESSTIMESTAMP_ENTRY(CtrlIdx,DataPtr,timeQualPtr,timeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETEGRESSTIMESTAMP_EXIT
/** \brief Exit point of function Eth_GetEgressTimeStamp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_GETEGRESSTIMESTAMP_EXIT(CtrlIdx,DataPtr,timeQualPtr,timeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETINGRESSTIMESTAMP_ENTRY
/** \brief Entry point of function Eth_GetIngressTimeStamp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_GETINGRESSTIMESTAMP_ENTRY(CtrlIdx,DataPtr,timeQualPtr,timeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_GETINGRESSTIMESTAMP_EXIT
/** \brief Exit point of function Eth_GetIngressTimeStamp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_GETINGRESSTIMESTAMP_EXIT(CtrlIdx,DataPtr,timeQualPtr,timeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SETCORRECTIONTIME_ENTRY
/** \brief Entry point of function Eth_SetCorrectionTime()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SETCORRECTIONTIME_ENTRY(CtrlIdx,timeOffsetPtr,rateRatioPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SETCORRECTIONTIME_EXIT
/** \brief Exit point of function Eth_SetCorrectionTime()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SETCORRECTIONTIME_EXIT(CtrlIdx,timeOffsetPtr,rateRatioPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SETGLOBALTIME_ENTRY
/** \brief Entry point of function Eth_SetGlobalTime()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SETGLOBALTIME_ENTRY(CtrlIdx,timeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SETGLOBALTIME_EXIT
/** \brief Exit point of function Eth_SetGlobalTime()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_SETGLOBALTIME_EXIT(RetVal,CtrlIdx,timeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_TIMESTAMPMAINFUNCTION_ENTRY
/** \brief Entry point of function Eth_TimeStampMainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_TIMESTAMPMAINFUNCTION_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_TIMESTAMPMAINFUNCTION_EXIT
/** \brief Exit point of function Eth_TimeStampMainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_TIMESTAMPMAINFUNCTION_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_TXDISPATCHER_ENTRY
/** \brief Entry point of function Eth_TxDispatcher()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_TXDISPATCHER_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_TXDISPATCHER_EXIT
/** \brief Exit point of function Eth_TxDispatcher()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_TXDISPATCHER_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_PROVIDEPRIORITYTXBUFFER_ENTRY
/** \brief Entry point of function Eth_ProvidePriorityTxBuffer()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_PROVIDEPRIORITYTXBUFFER_ENTRY(CtrlIdx,Priority,BufIdxPtr,BufPtr,LenBytePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_PROVIDEPRIORITYTXBUFFER_EXIT
/** \brief Exit point of function Eth_ProvidePriorityTxBuffer()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_PROVIDEPRIORITYTXBUFFER_EXIT(BufReq,CtrlIdx,Priority,BufIdxPtr,BufPtr,LenBytePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_PRIORITYRECEIVE_ENTRY
/** \brief Entry point of function Eth_PriorityReceive()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_PRIORITYRECEIVE_ENTRY(CtrlIdx,Priority,FilterPriority,RxStatusPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_PRIORITYRECEIVE_EXIT
/** \brief Exit point of function Eth_PriorityReceive()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_PRIORITYRECEIVE_EXIT(CtrlIdx,Priority,FilterPriority,RxStatusPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_MAINFUNCTION_ENTRY
/** \brief Entry point of function Eth_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_MAINFUNCTION_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_MAINFUNCTION_EXIT
/** \brief Exit point of function Eth_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_MAINFUNCTION_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SETRXINTERRUPTUSERCALLOUTMODE_ENTRY
/** \brief Entry point of function Eth_SetRxInterruptUsercalloutMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SETRXINTERRUPTUSERCALLOUTMODE_ENTRY(CtrlIdx,FifoIdx,Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SETRXINTERRUPTUSERCALLOUTMODE_EXIT
/** \brief Exit point of function Eth_SetRxInterruptUsercalloutMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SETRXINTERRUPTUSERCALLOUTMODE_EXIT(CtrlIdx,FifoIdx,Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SETTXINTERRUPTUSERCALLOUTMODE_ENTRY
/** \brief Entry point of function Eth_SetTxInterruptUsercalloutMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SETTXINTERRUPTUSERCALLOUTMODE_ENTRY(CtrlIdx,FifoIdx,Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SETTXINTERRUPTUSERCALLOUTMODE_EXIT
/** \brief Exit point of function Eth_SetTxInterruptUsercalloutMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SETTXINTERRUPTUSERCALLOUTMODE_EXIT(CtrlIdx,FifoIdx,Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */



/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef ETH_TRACE_H */
/*==================[end of file]===========================================*/

