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

#ifndef ETH_TRACE_INTERNAL_H
#define ETH_TRACE_INTERNAL_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif


/*==================[macros]================================================*/
/* Place internal debug instrumentation macros in this file, i.e. all
   macros that are not generated for the public api instrumentation.
*/

/* == Trace macros for sub module AddrFilt == */
/* -- Trace macros for sub module LocLst -- */
/* Trace macros for file Eth_LocLst.c */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_UPDATEPHYSADDRFILTER_ENTRY
/** \brief Entry point of function Eth_Arch_UpdatePhysAddrFilter()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_UPDATEPHYSADDRFILTER_ENTRY(CtrlIdx,PhysAddrPtr,Action)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_UPDATEPHYSADDRFILTER_EXIT
/** \brief Exit point of function Eth_Arch_UpdatePhysAddrFilter()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_ARCH_UPDATEPHYSADDRFILTER_EXIT(Ret,CtrlIdx,PhysAddrPtr,Action)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_LOCLSTADDADDR_ENTRY
/** \brief Entry point of function Eth_LocLstAddAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_LOCLSTADDADDR_ENTRY(CtrlIdx,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_LOCLSTADDADDR_EXIT
/** \brief Exit point of function Eth_LocLstAddAddr()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_LOCLSTADDADDR_EXIT(Ret,CtrlIdx,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_LOCLSTREMOVEADDR_ENTRY
/** \brief Entry point of function Eth_LocLstRemoveAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_LOCLSTREMOVEADDR_ENTRY(CtrlIdx,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_LOCLSTREMOVEADDR_EXIT
/** \brief Exit point of function Eth_LocLstRemoveAddr()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_LOCLSTREMOVEADDR_EXIT(Ret,CtrlIdx,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_LOCLSTFLUSH_ENTRY
/** \brief Entry point of function Eth_LocLstFlush()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_LOCLSTFLUSH_ENTRY(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_LOCLSTFLUSH_EXIT
/** \brief Exit point of function Eth_LocLstFlush()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_LOCLSTFLUSH_EXIT(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_LOCLSTENABLEPROMISCUOUS_ENTRY
/** \brief Entry point of function Eth_LocLstEnablePromiscuous()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_LOCLSTENABLEPROMISCUOUS_ENTRY(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_LOCLSTENABLEPROMISCUOUS_EXIT
/** \brief Exit point of function Eth_LocLstEnablePromiscuous()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_LOCLSTENABLEPROMISCUOUS_EXIT(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_LOCLSTDISABLEPROMISCUOUS_ENTRY
/** \brief Entry point of function Eth_LocLstDisablePromiscuous()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_LOCLSTDISABLEPROMISCUOUS_ENTRY(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_LOCLSTDISABLEPROMISCUOUS_EXIT
/** \brief Exit point of function Eth_LocLstDisablePromiscuous()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_LOCLSTDISABLEPROMISCUOUS_EXIT(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_LOCLSTCHECKPHYSADDRPOOL_ENTRY
/** \brief Entry point of function Eth_LocLstCheckPhysAddrPool()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_LOCLSTCHECKPHYSADDRPOOL_ENTRY(CtrlIdx,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_LOCLSTCHECKPHYSADDRPOOL_EXIT
/** \brief Exit point of function Eth_LocLstCheckPhysAddrPool()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_LOCLSTCHECKPHYSADDRPOOL_EXIT(Ret,CtrlIdx,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_LOCLSTPHYSADDRFILTERINIT_ENTRY
/** \brief Entry point of function Eth_LocLstPhysAddrFilterInit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_LOCLSTPHYSADDRFILTERINIT_ENTRY(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_LOCLSTPHYSADDRFILTERINIT_EXIT
/** \brief Exit point of function Eth_LocLstPhysAddrFilterInit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_LOCLSTPHYSADDRFILTERINIT_EXIT(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/* == Trace macros for sub module Frame == */
/* -- Trace macros for sub module BDR -- */
/* Trace macros for file Eth_Bdr.c */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_INIT_ENTRY
/** \brief Entry point of function Eth_Arch_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_INIT_ENTRY(CfgPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_INIT_EXIT
/** \brief Exit point of function Eth_Arch_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_INIT_EXIT(CfgPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_MIIINIT_ENTRY
/** \brief Entry point of function Eth_Arch_MiiInit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_MIIINIT_ENTRY(CfgPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_MIIINIT_EXIT
/** \brief Exit point of function Eth_Arch_MiiInit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_ARCH_MIIINIT_EXIT(Res,CfgPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_CONTROLLERINIT_ENTRY
/** \brief Entry point of function Eth_Arch_ControllerInit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_CONTROLLERINIT_ENTRY(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_CONTROLLERINIT_EXIT
/** \brief Exit point of function Eth_Arch_ControllerInit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_ARCH_CONTROLLERINIT_EXIT(Res,CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_SETCONTROLLERMODE_ENTRY
/** \brief Entry point of function Eth_Arch_SetControllerMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_SETCONTROLLERMODE_ENTRY(CtrlIdx,CtrlMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_SETCONTROLLERMODE_EXIT
/** \brief Exit point of function Eth_Arch_SetControllerMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_ARCH_SETCONTROLLERMODE_EXIT(Result,CtrlIdx,CtrlMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_GETPHYSADDR_ENTRY
/** \brief Entry point of function Eth_Arch_GetPhysAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_GETPHYSADDR_ENTRY(CtrlIdx,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_GETPHYSADDR_EXIT
/** \brief Exit point of function Eth_Arch_GetPhysAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_GETPHYSADDR_EXIT(CtrlIdx,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_SETPHYSADDR_ENTRY
/** \brief Entry point of function Eth_Arch_SetPhysAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_SETPHYSADDR_ENTRY(CtrlIdx,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_SETPHYSADDR_EXIT
/** \brief Exit point of function Eth_Arch_SetPhysAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_SETPHYSADDR_EXIT(CtrlIdx,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_GETCOUNTERSTATE_ENTRY
/** \brief Entry point of function Eth_Arch_GetCounterState()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_GETCOUNTERSTATE_ENTRY(CtrlIdx,CtrOffs,CtrValPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_GETCOUNTERSTATE_EXIT
/** \brief Exit point of function Eth_Arch_GetCounterState()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_GETCOUNTERSTATE_EXIT(CtrlIdx,CtrOffs,CtrValPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_WRITEMII_ENTRY
/** \brief Entry point of function Eth_Arch_WriteMii()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_WRITEMII_ENTRY(CtrlIdx,TrcvIdx,RegIdx,RegVal)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_WRITEMII_EXIT
/** \brief Exit point of function Eth_Arch_WriteMii()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_ARCH_WRITEMII_EXIT(Ret,CtrlIdx,TrcvIdx,RegIdx,RegVal)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_READMII_ENTRY
/** \brief Entry point of function Eth_Arch_ReadMii()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_READMII_ENTRY(CtrlIdx,TrcvIdx,RegIdx,RegVal)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_READMII_EXIT
/** \brief Exit point of function Eth_Arch_ReadMii()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_ARCH_READMII_EXIT(Ret,CtrlIdx,TrcvIdx,RegIdx,RegVal)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/* Trace macros for file Eth_BdrRx.c */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_RECEIVE_ENTRY
/** \brief Entry point of function Eth_Arch_Receive()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_RECEIVE_ENTRY(CtrlIdx,QueRxIdx,RxStatus,Irq)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_RECEIVE_EXIT
/** \brief Exit point of function Eth_Arch_Receive()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_RECEIVE_EXIT(CtrlIdx,QueRxIdx,RxStatus,Irq)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRINITBDRX_ENTRY
/** \brief Entry point of function Eth_BdrInitBdRx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRINITBDRX_ENTRY(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRINITBDRX_EXIT
/** \brief Exit point of function Eth_BdrInitBdRx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRINITBDRX_EXIT(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRINITBDRXQUEUE_ENTRY
/** \brief Entry point of function Eth_BdrInitBdRxQueue()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRINITBDRXQUEUE_ENTRY(CtrlIdx,QueRxIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRINITBDRXQUEUE_EXIT
/** \brief Exit point of function Eth_BdrInitBdRxQueue()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRINITBDRXQUEUE_EXIT(CtrlIdx,QueRxIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRRXGETBUFADDRBYBDRXIDX_ENTRY
/** \brief Entry point of function Eth_BdrRxGetBufAddrByBdRxIdx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRRXGETBUFADDRBYBDRXIDX_ENTRY(CtrlIdx,QueRxIdx,BdRxIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRRXGETBUFADDRBYBDRXIDX_EXIT
/** \brief Exit point of function Eth_BdrRxGetBufAddrByBdRxIdx()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_BDRRXGETBUFADDRBYBDRXIDX_EXIT(BufAddr,CtrlIdx,QueRxIdx,BdRxIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRRXGETBUFADDRBYBUFRXIDX_ENTRY
/** \brief Entry point of function Eth_BdrRxGetBufAddrByBufRxIdx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRRXGETBUFADDRBYBUFRXIDX_ENTRY(CtrlIdx,QueRxIdx,BufRxIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRRXGETBUFADDRBYBUFRXIDX_EXIT
/** \brief Exit point of function Eth_BdrRxGetBufAddrByBufRxIdx()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_BDRRXGETBUFADDRBYBUFRXIDX_EXIT(BufAddr,CtrlIdx,QueRxIdx,BufRxIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_FRAMEBUFRXINGRESSTSGETRXBDIDX_ENTRY
/** \brief Entry point of function Eth_FrameBufRxIngressTsGetRxBdIdx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_FRAMEBUFRXINGRESSTSGETRXBDIDX_ENTRY(CtrlIdx,DataPtr,QueRxIdxPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_FRAMEBUFRXINGRESSTSGETRXBDIDX_EXIT
/** \brief Exit point of function Eth_FrameBufRxIngressTsGetRxBdIdx()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_FRAMEBUFRXINGRESSTSGETRXBDIDX_EXIT(RxBdIdx,CtrlIdx,DataPtr,QueRxIdxPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRPROCESSRXFRAME_ENTRY
/** \brief Entry point of function Eth_BdrProcessRxFrame()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRPROCESSRXFRAME_ENTRY(CtrlIdx,QueRxIdx,pRxInfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRPROCESSRXFRAME_EXIT
/** \brief Exit point of function Eth_BdrProcessRxFrame()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_BDRPROCESSRXFRAME_EXIT(RetVal,CtrlIdx,QueRxIdx,pRxInfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRASSERTPHYSADDRRXFRAME_ENTRY
/** \brief Entry point of function Eth_BdrAssertPhysAddrRxFrame()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRASSERTPHYSADDRRXFRAME_ENTRY(CtrlIdx,pRxInfo,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRASSERTPHYSADDRRXFRAME_EXIT
/** \brief Exit point of function Eth_BdrAssertPhysAddrRxFrame()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_BDRASSERTPHYSADDRRXFRAME_EXIT(Ret,CtrlIdx,pRxInfo,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_SETRXINTERRUPTUSERCALLOUTMODE_ENTRY
/** \brief Entry point of function Eth_Arch_SetRxInterruptUsercalloutMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_SETRXINTERRUPTUSERCALLOUTMODE_ENTRY(CtrlIdx,QueRxIdx,Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_SETRXINTERRUPTUSERCALLOUTMODE_EXIT
/** \brief Exit point of function Eth_Arch_SetRxInterruptUsercalloutMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_SETRXINTERRUPTUSERCALLOUTMODE_EXIT(CtrlIdx,QueRxIdx,Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/* Trace macros for file Eth_BdrTx.c */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_PROVIDETXBUFFER_ENTRY
/** \brief Entry point of function Eth_Arch_ProvideTxBuffer()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_PROVIDETXBUFFER_ENTRY(CtrlIdx,Priority,BufIdxPtr,BufPtr,LenBytePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_PROVIDETXBUFFER_EXIT
/** \brief Exit point of function Eth_Arch_ProvideTxBuffer()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_ARCH_PROVIDETXBUFFER_EXIT(Ret,CtrlIdx,Priority,BufIdxPtr,BufPtr,LenBytePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_TRANSMIT_ENTRY
/** \brief Entry point of function Eth_Arch_Transmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_TRANSMIT_ENTRY(CtrlIdx,BufId,FrameType,TxConfirmation,LenByte,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_TRANSMIT_EXIT
/** \brief Exit point of function Eth_Arch_Transmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_ARCH_TRANSMIT_EXIT(Ret,CtrlIdx,BufId,FrameType,TxConfirmation,LenByte,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_TXCONFIRMATION_ENTRY
/** \brief Entry point of function Eth_Arch_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_TXCONFIRMATION_ENTRY(CtrlIdx,Irq)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_TXCONFIRMATION_EXIT
/** \brief Exit point of function Eth_Arch_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_TXCONFIRMATION_EXIT(CtrlIdx,Irq)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXCONFIRMATION_ENTRY
/** \brief Entry point of function Eth_BdrTxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXCONFIRMATION_ENTRY(CtrlIdx,QueTxBdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXCONFIRMATION_EXIT
/** \brief Exit point of function Eth_BdrTxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXCONFIRMATION_EXIT(CtrlIdx,QueTxBdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXBDCHECKTRANSMITEDUNLINK_ENTRY
/** \brief Entry point of function Eth_BdrTxBdCheckTransmitedUnlink()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXBDCHECKTRANSMITEDUNLINK_ENTRY(CtrlIdx,QueTxBdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXBDCHECKTRANSMITEDUNLINK_EXIT
/** \brief Exit point of function Eth_BdrTxBdCheckTransmitedUnlink()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXBDCHECKTRANSMITEDUNLINK_EXIT(CtrlIdx,QueTxBdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRINITBDTX_ENTRY
/** \brief Entry point of function Eth_BdrInitBdTx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRINITBDTX_ENTRY(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRINITBDTX_EXIT
/** \brief Exit point of function Eth_BdrInitBdTx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRINITBDTX_EXIT(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRINITBUFTXQUEUE_ENTRY
/** \brief Entry point of function Eth_BdrInitBufTxQueue()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRINITBUFTXQUEUE_ENTRY(CtrlIdx,QueTxBufIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRINITBUFTXQUEUE_EXIT
/** \brief Exit point of function Eth_BdrInitBufTxQueue()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRINITBUFTXQUEUE_EXIT(CtrlIdx,QueTxBufIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRINITBDTXQUEUE_ENTRY
/** \brief Entry point of function Eth_BdrInitBdTxQueue()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRINITBDTXQUEUE_ENTRY(CtrlIdx,QueTxBdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRINITBDTXQUEUE_EXIT
/** \brief Exit point of function Eth_BdrInitBdTxQueue()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRINITBDTXQUEUE_EXIT(CtrlIdx,QueTxBdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_FRAMEBUFBEFORTRANS_ENTRY
/** \brief Entry point of function Eth_FrameBufBeforTrans()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_FRAMEBUFBEFORTRANS_ENTRY(CtrlIdx,BufTxIdxInt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_FRAMEBUFBEFORTRANS_EXIT
/** \brief Exit point of function Eth_FrameBufBeforTrans()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_FRAMEBUFBEFORTRANS_EXIT(BeforeTrans,CtrlIdx,BufTxIdxInt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_FRAMEBUFTXSETEGRESS_ENTRY
/** \brief Entry point of function Eth_FrameBufTxSetEgress()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_FRAMEBUFTXSETEGRESS_ENTRY(CtrlIdx,BufTxIdxInt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_FRAMEBUFTXSETEGRESS_EXIT
/** \brief Exit point of function Eth_FrameBufTxSetEgress()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_FRAMEBUFTXSETEGRESS_EXIT(CtrlIdx,BufTxIdxInt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_FRAMEBUFREADYFOREGRESS_ENTRY
/** \brief Entry point of function Eth_FrameBufReadyForEgress()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_FRAMEBUFREADYFOREGRESS_ENTRY(CtrlIdx,BufTxIdxInt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_FRAMEBUFREADYFOREGRESS_EXIT
/** \brief Exit point of function Eth_FrameBufReadyForEgress()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_FRAMEBUFREADYFOREGRESS_EXIT(ReadyForEgress,CtrlIdx,BufTxIdxInt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_FRAMETRANSMIT_ENTRY
/** \brief Entry point of function Eth_FrameTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_FRAMETRANSMIT_ENTRY(CtrlIdx,BufIdxInt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_FRAMETRANSMIT_EXIT
/** \brief Exit point of function Eth_FrameTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_FRAMETRANSMIT_EXIT(CtrlIdx,BufIdxInt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_FRAMEBUFGETFRAMELEN_ENTRY
/** \brief Entry point of function Eth_FrameBufGetFrameLen()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_FRAMEBUFGETFRAMELEN_ENTRY(CtrlIdx,BufTxIdxInt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_FRAMEBUFGETFRAMELEN_EXIT
/** \brief Exit point of function Eth_FrameBufGetFrameLen()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_FRAMEBUFGETFRAMELEN_EXIT(FrameLen,CtrlIdx,BufTxIdxInt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_RETRANSMIT_ENTRY
/** \brief Entry point of function Eth_Arch_Retransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_RETRANSMIT_ENTRY(CtrlIdx,OrigCtrlIdx,pBufId,pData,FrameType,LenByte,pReInfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_RETRANSMIT_EXIT
/** \brief Exit point of function Eth_Arch_Retransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_ARCH_RETRANSMIT_EXIT(Result,CtrlIdx,OrigCtrlIdx,pBufId,pData,FrameType,LenByte,pReInfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_TRANSMITBUFLIST_ENTRY
/** \brief Entry point of function Eth_Arch_TransmitBufList()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_TRANSMITBUFLIST_ENTRY(CtrlIdx,Priority,pBufLst,BufLstNo,pUser)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_TRANSMITBUFLIST_EXIT
/** \brief Exit point of function Eth_Arch_TransmitBufList()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_ARCH_TRANSMITBUFLIST_EXIT(Result,CtrlIdx,Priority,pBufLst,BufLstNo,pUser)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_SETTXINTERRUPTUSERCALLOUTMODE_ENTRY
/** \brief Entry point of function Eth_Arch_SetTxInterruptUsercalloutMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_SETTXINTERRUPTUSERCALLOUTMODE_ENTRY(CtrlIdx,QueTxBufIdx,Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_SETTXINTERRUPTUSERCALLOUTMODE_EXIT
/** \brief Exit point of function Eth_Arch_SetTxInterruptUsercalloutMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_SETTXINTERRUPTUSERCALLOUTMODE_EXIT(CtrlIdx,QueTxBufIdx,Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/* Trace macros for file Eth_BdrTx.c - internal functions */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXGETQUETXIDXBYPRIO_ENTRY
/** \brief Entry point of function Eth_BdrTxGetQueTxIdxByPrio()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXGETQUETXIDXBYPRIO_ENTRY(CtrlIdx,Priority,pQueTxBufIdx,pQueTxBdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXGETQUETXIDXBYPRIO_EXIT
/** \brief Exit point of function Eth_BdrTxGetQueTxIdxByPrio()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXGETQUETXIDXBYPRIO_EXIT(CtrlIdx,Priority,pQueTxBufIdx,pQueTxBdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRPROVIDETXBUFFER_ENTRY
/** \brief Entry point of function Eth_BdrProvideTxBuffer()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRPROVIDETXBUFFER_ENTRY(CtrlIdx,BufIdxPtr,BufPtr,LenBytePtr,QueTxBufIdx,QueTxBdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRPROVIDETXBUFFER_EXIT
/** \brief Exit point of function Eth_BdrProvideTxBuffer()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_BDRPROVIDETXBUFFER_EXIT(Ret,CtrlIdx,BufIdxPtr,BufPtr,LenBytePtr,QueTxBufIdx,QueTxBdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXWRTETHHEAD_ENTRY
/** \brief Entry point of function Eth_BdrTxWrtEthHead()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXWRTETHHEAD_ENTRY(CtrlIdx,pBuf,FrameType,pPhysAddr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXWRTETHHEAD_EXIT
/** \brief Exit point of function Eth_BdrTxWrtEthHead()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXWRTETHHEAD_EXIT(CtrlIdx,pBuf,FrameType,pPhysAddr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXTRANSMIT2_ENTRY
/** \brief Entry point of function Eth_BdrTxTransmit2()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXTRANSMIT2_ENTRY(CtrlIdx,BufIdxInt,pBuf,TxConfirmation,LenByte)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXTRANSMIT2_EXIT
/** \brief Exit point of function Eth_BdrTxTransmit2()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_BDRTXTRANSMIT2_EXIT(Ret,CtrlIdx,BufIdxInt,pBuf,TxConfirmation,LenByte)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXGETBUFADDR_ENTRY
/** \brief Entry point of function Eth_BdrTxGetBufAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXGETBUFADDR_ENTRY(CtrlIdx,BufIdxInt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXGETBUFADDR_EXIT
/** \brief Exit point of function Eth_BdrTxGetBufAddr()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_BDRTXGETBUFADDR_EXIT(pBuf,CtrlIdx,BufIdxInt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXLNKBDTX_ENTRY
/** \brief Entry point of function Eth_BdrTxLnkBdTx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXLNKBDTX_ENTRY(CtrlIdx,QueTxBufIdx,BufIdxInt,pBuf,ErrForcFree)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXLNKBDTX_EXIT
/** \brief Exit point of function Eth_BdrTxLnkBdTx()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_BDRTXLNKBDTX_EXIT(Ret,CtrlIdx,QueTxBufIdx,BufIdxInt,pBuf,ErrForcFree)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXUNLINKBDINFO_ENTRY
/** \brief Entry point of function Eth_BdrTxUnlinkBdInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXUNLINKBDINFO_ENTRY(CtrlIdx,QueTxBdIdx,BufIdxInt,BdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXUNLINKBDINFO_EXIT
/** \brief Exit point of function Eth_BdrTxUnlinkBdInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXUNLINKBDINFO_EXIT(CtrlIdx,QueTxBdIdx,BufIdxInt,BdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXUNLINKBUFINFO_ENTRY
/** \brief Entry point of function Eth_BdrTxUnlinkBufInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXUNLINKBUFINFO_ENTRY(CtrlIdx,BufIdxInt,BdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXUNLINKBUFINFO_EXIT
/** \brief Exit point of function Eth_BdrTxUnlinkBufInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXUNLINKBUFINFO_EXIT(CtrlIdx,BufIdxInt,BdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXBDUNLINKCHECKREL_ENTRY
/** \brief Entry point of function Eth_BdrTxBdUnlinkCheckRel()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXBDUNLINKCHECKREL_ENTRY(CtrlIdx,QueTxBdIdx,TxBdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXBDUNLINKCHECKREL_EXIT
/** \brief Exit point of function Eth_BdrTxBdUnlinkCheckRel()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXBDUNLINKCHECKREL_EXIT(CtrlIdx,QueTxBdIdx,TxBdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXBUFFREE_ENTRY
/** \brief Entry point of function Eth_BdrTxBufFree()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXBUFFREE_ENTRY(CtrlIdx,BufTxIdxInt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXBUFFREE_EXIT
/** \brief Exit point of function Eth_BdrTxBufFree()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXBUFFREE_EXIT(CtrlIdx,BufTxIdxInt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXRETXCHECKTXBUF_ENTRY
/** \brief Entry point of function Eth_BdrTxRetxCheckTxBuf()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXRETXCHECKTXBUF_ENTRY(BufIdxIntOrig)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXRETXCHECKTXBUF_EXIT
/** \brief Exit point of function Eth_BdrTxRetxCheckTxBuf()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_BDRTXRETXCHECKTXBUF_EXIT(BufOk,BufIdxIntOrig)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXNOFREETXBD_ENTRY
/** \brief Entry point of function Eth_BdrTxNoFreeTxBd()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXNOFREETXBD_ENTRY(CtrlIdx,QueTxBufIdx,TxBufIdxInt,ForcFree)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRTXNOFREETXBD_EXIT
/** \brief Exit point of function Eth_BdrTxNoFreeTxBd()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRTXNOFREETXBD_EXIT(CtrlIdx,QueTxBufIdx,TxBufIdxInt,ForcFree)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRMEMTXREQUEST_ENTRY
/** \brief Entry point of function Eth_BdrMemTxRequest()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRMEMTXREQUEST_ENTRY(CtrlIdx,QueTxBufIdx,MemIdPtr,MemPtr,LenBytePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRMEMTXREQUEST_EXIT
/** \brief Exit point of function Eth_BdrMemTxRequest()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_BDRMEMTXREQUEST_EXIT(Ret,CtrlIdx,QueTxBufIdx,MemIdPtr,MemPtr,LenBytePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRMEMTXFREE_ENTRY
/** \brief Entry point of function Eth_BdrMemTxFree()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRMEMTXFREE_ENTRY(CtrlIdx,QueTxBufIdx,MemId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRMEMTXFREE_EXIT
/** \brief Exit point of function Eth_BdrMemTxFree()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRMEMTXFREE_EXIT(CtrlIdx,QueTxBufIdx,MemId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRBUFTXCONFPENDQUEMPTY_ENTRY
/** \brief Entry point of function Eth_BdrBufTxConfPendQuEmpty()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRBUFTXCONFPENDQUEMPTY_ENTRY(CtrlIdx,QueTxBdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRBUFTXCONFPENDQUEMPTY_EXIT
/** \brief Exit point of function Eth_BdrBufTxConfPendQuEmpty()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRBUFTXCONFPENDQUEMPTY_EXIT(CtrlIdx,QueTxBdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRBUFTXCONFPENDQUADD_ENTRY
/** \brief Entry point of function Eth_BdrBufTxConfPendQuAdd()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRBUFTXCONFPENDQUADD_ENTRY(CtrlIdx,QueTxBdIdx,BufIdxInt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRBUFTXCONFPENDQUADD_EXIT
/** \brief Exit point of function Eth_BdrBufTxConfPendQuAdd()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRBUFTXCONFPENDQUADD_EXIT(CtrlIdx,QueTxBdIdx,BufIdxInt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRBUFTXCONFPENDQUNXT_ENTRY
/** \brief Entry point of function Eth_BdrBufTxConfPendQuNxt()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRBUFTXCONFPENDQUNXT_ENTRY(CtrlIdx,QueTxBdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_BDRBUFTXCONFPENDQUNXT_EXIT
/** \brief Exit point of function Eth_BdrBufTxConfPendQuNxt()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_BDRBUFTXCONFPENDQUNXT_EXIT(CtrlIdx,QueTxBdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/* == Trace macros for sub module MAC == */










/* -- Trace macros for sub module QoS -- */
/* Trace macros for file Eth_Qos.c */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_QOSGETQUETXBUFIDXBYPRIO_ENTRY
/** \brief Entry point of function Eth_QosGetQueTxBufIdxByPrio()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_QOSGETQUETXBUFIDXBYPRIO_ENTRY(CtrlIdx,Priority)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_QOSGETQUETXBUFIDXBYPRIO_EXIT
/** \brief Exit point of function Eth_QosGetQueTxBufIdxByPrio()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_QOSGETQUETXBUFIDXBYPRIO_EXIT(Res,CtrlIdx,Priority)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_QOSTXQUEADD_ENTRY
/** \brief Entry point of function Eth_QosTxQueAdd()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_QOSTXQUEADD_ENTRY(CtrlIdx,QueTxBufIdx,BufIdxInt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_QOSTXQUEADD_EXIT
/** \brief Exit point of function Eth_QosTxQueAdd()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_QOSTXQUEADD_EXIT(CtrlIdx,QueTxBufIdx,BufIdxInt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_TXDISPATCHER_ENTRY
/** \brief Entry point of function Eth_Arch_TxDispatcher()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_TXDISPATCHER_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_TXDISPATCHER_EXIT
/** \brief Exit point of function Eth_Arch_TxDispatcher()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_TXDISPATCHER_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_QOSQUERYNXTSTRM_ENTRY
/** \brief Entry point of function Eth_QosQueryNxtStrm()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_QOSQUERYNXTSTRM_ENTRY(CtrlIdx,StrmTreeIdx,CycleSeqAct,pCycleSeqNxt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_QOSQUERYNXTSTRM_EXIT
/** \brief Exit point of function Eth_QosQueryNxtStrm()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_QOSQUERYNXTSTRM_EXIT(BufIdx,CtrlIdx,StrmTreeIdx,CycleSeqAct,pCycleSeqNxt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_QOSTXCONFIRMATION_ENTRY
/** \brief Entry point of function Eth_QosTxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_QOSTXCONFIRMATION_ENTRY(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_QOSTXCONFIRMATION_EXIT
/** \brief Exit point of function Eth_QosTxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_QOSTXCONFIRMATION_EXIT(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_PRIORITYRECEIVE_ENTRY
/** \brief Entry point of function Eth_Arch_PriorityReceive()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_PRIORITYRECEIVE_ENTRY(CtrlIdx,Priority,FilterPriority,RxStatusPtr,Irq)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_PRIORITYRECEIVE_EXIT
/** \brief Exit point of function Eth_Arch_PriorityReceive()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_PRIORITYRECEIVE_EXIT(CtrlIdx,Priority,FilterPriority,RxStatusPtr,Irq)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_QOSINIT_ENTRY
/** \brief Entry point of function Eth_QosInit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_QOSINIT_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_QOSINIT_EXIT
/** \brief Exit point of function Eth_QosInit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_QOSINIT_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_QOSINITPRIOBDRX_ENTRY
/** \brief Entry point of function Eth_QosInitPrioBdRx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_QOSINITPRIOBDRX_ENTRY(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_QOSINITPRIOBDRX_EXIT
/** \brief Exit point of function Eth_QosInitPrioBdRx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_QOSINITPRIOBDRX_EXIT(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_QOSINITPRIOBDTX_ENTRY
/** \brief Entry point of function Eth_QosInitPrioBdTx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_QOSINITPRIOBDTX_ENTRY(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_QOSINITPRIOBDTX_EXIT
/** \brief Exit point of function Eth_QosInitPrioBdTx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_QOSINITPRIOBDTX_EXIT(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_QOSINITPRIOBUFTXQUEINFO_ENTRY
/** \brief Entry point of function Eth_QosInitPrioBufTxQueInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_QOSINITPRIOBUFTXQUEINFO_ENTRY(CtrlIdx,QueTxBufIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_QOSINITPRIOBUFTXQUEINFO_EXIT
/** \brief Exit point of function Eth_QosInitPrioBufTxQueInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_QOSINITPRIOBUFTXQUEINFO_EXIT(CtrlIdx,QueTxBufIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_QOSCHECKPRIO_ENTRY
/** \brief Entry point of function Eth_QosCheckPrio()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_QOSCHECKPRIO_ENTRY(CtrlIdx,QueTxBufIdx,RxBdIdx,PrioQue)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_QOSCHECKPRIO_EXIT
/** \brief Exit point of function Eth_QosCheckPrio()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_QOSCHECKPRIO_EXIT(FramePrioQue,CtrlIdx,QueTxBufIdx,RxBdIdx,PrioQue)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_QOSVSWTBEQUENXT_ENTRY
/** \brief Entry point of function Eth_QosVswtBeQueNxt()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_QOSVSWTBEQUENXT_ENTRY(CtrlIdx,StrmTreeIdx,CycleSeqAct,pCycleSeqNxt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_QOSVSWTBEQUENXT_EXIT
/** \brief Exit point of function Eth_QosVswtBeQueNxt()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_QOSVSWTBEQUENXT_EXIT(Ret,CtrlIdx,StrmTreeIdx,CycleSeqAct,pCycleSeqNxt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/* == Trace macros for sub module TSN == */
/* Trace macros for file Eth_Tsn.c */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_GETCURRENTTIME_ENTRY
/** \brief Entry point of function Eth_Arch_GetCurrentTime()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_GETCURRENTTIME_ENTRY(CtrlIdx,TimeQualPtr,TimeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_GETCURRENTTIME_EXIT
/** \brief Exit point of function Eth_Arch_GetCurrentTime()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_ARCH_GETCURRENTTIME_EXIT(Ret,CtrlIdx,TimeQualPtr,TimeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_SETGLOBALTIME_ENTRY
/** \brief Entry point of function Eth_Arch_SetGlobalTime()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_SETGLOBALTIME_ENTRY(CtrlIdx,TimeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_SETGLOBALTIME_EXIT
/** \brief Exit point of function Eth_Arch_SetGlobalTime()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_ARCH_SETGLOBALTIME_EXIT(Ret,CtrlIdx,TimeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_SETCORRECTIONTIME_ENTRY
/** \brief Entry point of function Eth_Arch_SetCorrectionTime()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_SETCORRECTIONTIME_ENTRY(CtrlIdx,TimeOffsetPtr,RateRatioPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_SETCORRECTIONTIME_EXIT
/** \brief Exit point of function Eth_Arch_SetCorrectionTime()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_SETCORRECTIONTIME_EXIT(CtrlIdx,TimeOffsetPtr,RateRatioPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_SETGLOBALTIME_ENTRY
/** \brief Entry point of function Eth_Arch_SetGlobalTime()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_SETGLOBALTIME_ENTRY(CtrlIdx,TimeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_SETGLOBALTIME_EXIT
/** \brief Exit point of function Eth_Arch_SetGlobalTime()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_ARCH_SETGLOBALTIME_EXIT(Ret,CtrlIdx,TimeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_SETCORRECTIONTIME_ENTRY
/** \brief Entry point of function Eth_Arch_SetCorrectionTime()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_SETCORRECTIONTIME_ENTRY(CtrlIdx,TimeOffsetPtr,RateRatioPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_SETCORRECTIONTIME_EXIT
/** \brief Exit point of function Eth_Arch_SetCorrectionTime()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_SETCORRECTIONTIME_EXIT(CtrlIdx,TimeOffsetPtr,RateRatioPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_ENABLEEGRESSTIMESTAMP_ENTRY
/** \brief Entry point of function Eth_Arch_EnableEgressTimeStamp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_ENABLEEGRESSTIMESTAMP_ENTRY(CtrlIdx,BufId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_ENABLEEGRESSTIMESTAMP_EXIT
/** \brief Exit point of function Eth_Arch_EnableEgressTimeStamp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_ENABLEEGRESSTIMESTAMP_EXIT(CtrlIdx,BufId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_GETEGRESSTIMESTAMP_ENTRY
/** \brief Entry point of function Eth_Arch_GetEgressTimeStamp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_GETEGRESSTIMESTAMP_ENTRY(CtrlIdx,BufId,TimeQualPtr,TimeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_GETEGRESSTIMESTAMP_EXIT
/** \brief Exit point of function Eth_Arch_GetEgressTimeStamp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_GETEGRESSTIMESTAMP_EXIT(CtrlIdx,BufId,TimeQualPtr,TimeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_GETINGRESSTIMESTAMP_ENTRY
/** \brief Entry point of function Eth_Arch_GetIngressTimeStamp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_GETINGRESSTIMESTAMP_ENTRY(CtrlIdx,pRxInfo,pTimeQual,pTimeStamp)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_GETINGRESSTIMESTAMP_EXIT
/** \brief Exit point of function Eth_Arch_GetIngressTimeStamp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_GETINGRESSTIMESTAMP_EXIT(CtrlIdx,pRxInfo,pTimeQual,pTimeStamp)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_TSNTIMESTAMPADD_ENTRY
/** \brief Entry point of function Eth_TsnTimeStampAdd()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_TSNTIMESTAMPADD_ENTRY(TimeStamp1Ptr,TimeStamp2Ptr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_TSNTIMESTAMPADD_EXIT
/** \brief Exit point of function Eth_TsnTimeStampAdd()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_TSNTIMESTAMPADD_EXIT(retval,TimeStamp1Ptr,TimeStamp2Ptr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_TSNTIMESTAMPSUB_ENTRY
/** \brief Entry point of function Eth_TsnTimeStampSub()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_TSNTIMESTAMPSUB_ENTRY(TimeStampMinuendPtr,TimeStampSubtrahendPtr,TimeStampDifferencePt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_TSNTIMESTAMPSUB_EXIT
/** \brief Exit point of function Eth_TsnTimeStampSub()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_TSNTIMESTAMPSUB_EXIT(retval,TimeStampMinuendPtr,TimeStampSubtrahendPtr,TimeStampDifferencePt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_TSNTIMESTAMPISGREATEROREQUAL_ENTRY
/** \brief Entry point of function Eth_TsnTimeStampIsGreaterOrEqual()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_TSNTIMESTAMPISGREATEROREQUAL_ENTRY(TimeStamp1Ptr,TimeStamp2Ptr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_TSNTIMESTAMPISGREATEROREQUAL_EXIT
/** \brief Exit point of function Eth_TsnTimeStampIsGreaterOrEqual()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_TSNTIMESTAMPISGREATEROREQUAL_EXIT(Sign,TimeStamp1Ptr,TimeStamp2Ptr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/* == Trace macros for sub module TSN == */
/* -- Trace macros for sub module SWC -- */
/* Trace macros for file Eth_Swc.c */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SWCINIT_ENTRY
/** \brief Entry point of function Eth_SwcInit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SWCINIT_ENTRY(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SWCINIT_EXIT
/** \brief Exit point of function Eth_SwcInit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SWCINIT_EXIT(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SWCINITQUEUE_ENTRY
/** \brief Entry point of function Eth_SwcInitQueue()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SWCINITQUEUE_ENTRY(CtrlIdx,QueTxBdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SWCINITQUEUE_EXIT
/** \brief Exit point of function Eth_SwcInitQueue()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SWCINITQUEUE_EXIT(CtrlIdx,QueTxBdIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SWCGETCURRENTTIME_ENTRY
/** \brief Entry point of function Eth_SwcGetCurrentTime()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SWCGETCURRENTTIME_ENTRY(CtrlIdx,TimeQualPtr,TimeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SWCGETCURRENTTIME_EXIT
/** \brief Exit point of function Eth_SwcGetCurrentTime()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SWCGETCURRENTTIME_EXIT(CtrlIdx,TimeQualPtr,TimeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SWCTXTIMESTAMPSAVE_ENTRY
/** \brief Entry point of function Eth_SwcTxTimeStampSave()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SWCTXTIMESTAMPSAVE_ENTRY(CtrlIdx,QueTxBdIdx,BdTxIdx,BufTxIdxInt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SWCTXTIMESTAMPSAVE_EXIT
/** \brief Exit point of function Eth_SwcTxTimeStampSave()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SWCTXTIMESTAMPSAVE_EXIT(CtrlIdx,QueTxBdIdx,BdTxIdx,BufTxIdxInt)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SWCGETEGRESSTIMESTAMP_ENTRY
/** \brief Entry point of function Eth_SwcGetEgressTimeStamp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SWCGETEGRESSTIMESTAMP_ENTRY(CtrlIdx,BufTxIdxInt,TimeQualPtr,TimeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SWCGETEGRESSTIMESTAMP_EXIT
/** \brief Exit point of function Eth_SwcGetEgressTimeStamp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SWCGETEGRESSTIMESTAMP_EXIT(CtrlIdx,BufTxIdxInt,TimeQualPtr,TimeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SWCGETINGRESSTIMESTAMP_ENTRY
/** \brief Entry point of function Eth_SwcGetIngressTimeStamp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SWCGETINGRESSTIMESTAMP_ENTRY(CtrlIdx,pRxInfo,pTimeQual,pTimeStamp)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SWCGETINGRESSTIMESTAMP_EXIT
/** \brief Exit point of function Eth_SwcGetIngressTimeStamp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SWCGETINGRESSTIMESTAMP_EXIT(CtrlIdx,pRxInfo,pTimeQual,pTimeStamp)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SWCSETGLOBALTIME_ENTRY
/** \brief Entry point of function Eth_SwcSetGlobalTime()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_SWCSETGLOBALTIME_ENTRY(CtrlIdx,TimeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_SWCSETGLOBALTIME_EXIT
/** \brief Exit point of function Eth_SwcSetGlobalTime()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETH_SWCSETGLOBALTIME_EXIT(RetVal,CtrlIdx,TimeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_TIMESTAMPMAINFUNCTION_ENTRY
/** \brief Entry point of function Eth_Arch_TimeStampMainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_TIMESTAMPMAINFUNCTION_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_TIMESTAMPMAINFUNCTION_EXIT
/** \brief Exit point of function Eth_Arch_TimeStampMainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_TIMESTAMPMAINFUNCTION_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_MAINFUNCTION_ENTRY
/** \brief Entry point of function Eth_Arch_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_MAINFUNCTION_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETH_ARCH_MAINFUNCTION_EXIT
/** \brief Exit point of function Eth_Arch_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETH_ARCH_MAINFUNCTION_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */





/*==================[type definitions]======================================*/

/*=================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef ETH_TRACE_INTERNAL_H */
/*==================[end of file]===========================================*/


