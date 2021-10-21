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

#ifndef ETHIF_TRACE_H
#define ETHIF_TRACE_H
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


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_MAINFUNCTIONTX_ENTRY
/** \brief Entry point of function EthIf_MainFunctionTx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_MAINFUNCTIONTX_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_MAINFUNCTIONTX_EXIT
/** \brief Exit point of function EthIf_MainFunctionTx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_MAINFUNCTIONTX_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function EthIf_IsValidConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_ISVALIDCONFIG_ENTRY(voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_ISVALIDCONFIG_EXIT
/** \brief Exit point of function EthIf_IsValidConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_ISVALIDCONFIG_EXIT(Retval,voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_INIT_ENTRY
/** \brief Entry point of function EthIf_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_INIT_ENTRY(CfgPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_INIT_EXIT
/** \brief Exit point of function EthIf_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_INIT_EXIT(CfgPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SETCONTROLLERMODE_ENTRY
/** \brief Entry point of function EthIf_SetControllerMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_SETCONTROLLERMODE_ENTRY(CtrlIdx,CtrlMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SETCONTROLLERMODE_EXIT
/** \brief Exit point of function EthIf_SetControllerMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_SETCONTROLLERMODE_EXIT(Retval,CtrlIdx,CtrlMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETCONTROLLERMODE_ENTRY
/** \brief Entry point of function EthIf_GetControllerMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_GETCONTROLLERMODE_ENTRY(CtrlIdx,CtrlModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETCONTROLLERMODE_EXIT
/** \brief Exit point of function EthIf_GetControllerMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_GETCONTROLLERMODE_EXIT(Retval,CtrlIdx,CtrlModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETPHYSADDR_ENTRY
/** \brief Entry point of function EthIf_GetPhysAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_GETPHYSADDR_ENTRY(CtrlIdx,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETPHYSADDR_EXIT
/** \brief Exit point of function EthIf_GetPhysAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_GETPHYSADDR_EXIT(CtrlIdx,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_PROVIDETXBUFFER_ENTRY
/** \brief Entry point of function EthIf_ProvideTxBuffer()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_PROVIDETXBUFFER_ENTRY(CtrlIdx,FrameType,Priority,BufIdxPtr,BufPtr,LenBytePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_PROVIDETXBUFFER_EXIT
/** \brief Exit point of function EthIf_ProvideTxBuffer()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_PROVIDETXBUFFER_EXIT(Retval,CtrlIdx,FrameType,Priority,BufIdxPtr,BufPtr,LenBytePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_TRANSMIT_ENTRY
/** \brief Entry point of function EthIf_Transmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_TRANSMIT_ENTRY(CtrlIdx,BufIdx,FrameType,TxConfirmation,LenByte,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_TRANSMIT_EXIT
/** \brief Exit point of function EthIf_Transmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_TRANSMIT_EXIT(Retval,CtrlIdx,BufIdx,FrameType,TxConfirmation,LenByte,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_RETRANSMIT_ENTRY
/** \brief Entry point of function EthIf_Retransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_RETRANSMIT_ENTRY(CtrlIdx,OrigCtrlIdx,BufIdxPtr,DataPtr,FrameType,LenByte,RetransmitInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_RETRANSMIT_EXIT
/** \brief Exit point of function EthIf_Retransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_RETRANSMIT_EXIT(Retval,CtrlIdx,OrigCtrlIdx,BufIdxPtr,DataPtr,FrameType,LenByte,RetransmitInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETVERSIONINFO_ENTRY
/** \brief Entry point of function EthIf_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_GETVERSIONINFO_ENTRY(VersionInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETVERSIONINFO_EXIT
/** \brief Exit point of function EthIf_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_GETVERSIONINFO_EXIT(VersionInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SETPHYSADDR_ENTRY
/** \brief Entry point of function EthIf_SetPhysAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_SETPHYSADDR_ENTRY(CtrlIdx,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SETPHYSADDR_EXIT
/** \brief Exit point of function EthIf_SetPhysAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_SETPHYSADDR_EXIT(CtrlIdx,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_UPDATEPHYSADDRFILTER_ENTRY
/** \brief Entry point of function EthIf_UpdatePhysAddrFilter()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_UPDATEPHYSADDRFILTER_ENTRY(CtrlIdx,PhysAddrPtr,Action)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_UPDATEPHYSADDRFILTER_EXIT
/** \brief Exit point of function EthIf_UpdatePhysAddrFilter()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_UPDATEPHYSADDRFILTER_EXIT(Retval,CtrlIdx,PhysAddrPtr,Action)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_CBK_RXINDICATION_ENTRY
/** \brief Entry point of function EthIf_Cbk_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_CBK_RXINDICATION_ENTRY(CtrlIdx,DataPtr,LenByte)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_CBK_RXINDICATION_EXIT
/** \brief Exit point of function EthIf_Cbk_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_CBK_RXINDICATION_EXIT(CtrlIdx,DataPtr,LenByte)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_RXINDICATION_ENTRY
/** \brief Entry point of function EthIf_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_RXINDICATION_ENTRY(CtrlIdx,FrameType,isBroadcast,PhysAddrPtr,DataPtr,LenByte)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_RXINDICATION_EXIT
/** \brief Exit point of function EthIf_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_RXINDICATION_EXIT(CtrlIdx,FrameType,isBroadcast,PhysAddrPtr,DataPtr,LenByte)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_CBK_TXCONFIRMATION_ENTRY
/** \brief Entry point of function EthIf_Cbk_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_CBK_TXCONFIRMATION_ENTRY(CtrlIdx,BufIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_CBK_TXCONFIRMATION_EXIT
/** \brief Exit point of function EthIf_Cbk_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_CBK_TXCONFIRMATION_EXIT(CtrlIdx,BufIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

#if (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430)
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_TXCONFIRMATION_ENTRY
/** \brief Entry point of function EthIf_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_TXCONFIRMATION_ENTRY(CtrlIdx,BufIdx,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_TXCONFIRMATION_EXIT
/** \brief Exit point of function EthIf_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_TXCONFIRMATION_EXIT(CtrlIdx,BufIdx,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

#else
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_TXCONFIRMATION_ENTRY
/** \brief Entry point of function EthIf_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_TXCONFIRMATION_ENTRY(CtrlIdx,BufIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_TXCONFIRMATION_EXIT
/** \brief Exit point of function EthIf_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_TXCONFIRMATION_EXIT(CtrlIdx,BufIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

#endif /* (ETHIF_ETH_SUPPORT_API == ETHIF_ETH_SUPPORT_API_ASR430) */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_MAINFUNCTIONRX_ENTRY
/** \brief Entry point of function EthIf_MainFunctionRx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_MAINFUNCTIONRX_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_MAINFUNCTIONRX_EXIT
/** \brief Exit point of function EthIf_MainFunctionRx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_MAINFUNCTIONRX_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_MAINFUNCTIONRXPRIO_ENTRY
/** \brief Entry point of function EthIf_MainFunctionRxPrio()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_MAINFUNCTIONRXPRIO_ENTRY(PCtrlIngrFifoIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_MAINFUNCTIONRXPRIO_EXIT
/** \brief Exit point of function EthIf_MainFunctionRxPrio()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_MAINFUNCTIONRXPRIO_EXIT(PCtrlIngrFifoIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_MAINFUNCTIONSTATE_ENTRY
/** \brief Entry point of function EthIf_MainFunctionState()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_MAINFUNCTIONSTATE_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_MAINFUNCTIONSTATE_EXIT
/** \brief Exit point of function EthIf_MainFunctionState()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_MAINFUNCTIONSTATE_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETCURRENTTIME_ENTRY
/** \brief Entry point of function EthIf_GetCurrentTime()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_GETCURRENTTIME_ENTRY(CtrlIdx,timeQualPtr,timeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETCURRENTTIME_EXIT
/** \brief Exit point of function EthIf_GetCurrentTime()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_GETCURRENTTIME_EXIT(Retval,CtrlIdx,timeQualPtr,timeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_ENABLEEGRESSTIMESTAMP_ENTRY
/** \brief Entry point of function EthIf_EnableEgressTimeStamp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_ENABLEEGRESSTIMESTAMP_ENTRY(CtrlIdx,BufIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_ENABLEEGRESSTIMESTAMP_EXIT
/** \brief Exit point of function EthIf_EnableEgressTimeStamp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_ENABLEEGRESSTIMESTAMP_EXIT(CtrlIdx,BufIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETEGRESSTIMESTAMP_ENTRY
/** \brief Entry point of function EthIf_GetEgressTimeStamp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_GETEGRESSTIMESTAMP_ENTRY(CtrlIdx,BufIdx,timeQualPtr,timeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETEGRESSTIMESTAMP_EXIT
/** \brief Exit point of function EthIf_GetEgressTimeStamp()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_GETEGRESSTIMESTAMP_EXIT(Retval,CtrlIdx,BufIdx,timeQualPtr,timeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETINGRESSTIMESTAMP_ENTRY
/** \brief Entry point of function EthIf_GetIngressTimeStamp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_GETINGRESSTIMESTAMP_ENTRY(CtrlIdx,DataPtr,timeQualPtr,timeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETINGRESSTIMESTAMP_EXIT
/** \brief Exit point of function EthIf_GetIngressTimeStamp()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_GETINGRESSTIMESTAMP_EXIT(Retval,CtrlIdx,DataPtr,timeQualPtr,timeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SETCORRECTIONTIME_ENTRY
/** \brief Entry point of function EthIf_SetCorrectionTime()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_SETCORRECTIONTIME_ENTRY(CtrlIdx,timeOffsetPtr,rateRatioPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SETCORRECTIONTIME_EXIT
/** \brief Exit point of function EthIf_SetCorrectionTime()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_SETCORRECTIONTIME_EXIT(CtrlIdx,timeOffsetPtr,rateRatioPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SETGLOBALTIME_ENTRY
/** \brief Entry point of function EthIf_SetGlobalTime()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_SETGLOBALTIME_ENTRY(CtrlIdx,timeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SETGLOBALTIME_EXIT
/** \brief Exit point of function EthIf_SetGlobalTime()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_SETGLOBALTIME_EXIT(Retval,CtrlIdx,timeStampPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SETPHYLOOPBACKMODE_ENTRY
/** \brief Entry point of function EthIf_SetPhyLoopbackMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_SETPHYLOOPBACKMODE_ENTRY(TrcvIdx,Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SETPHYLOOPBACKMODE_EXIT
/** \brief Exit point of function EthIf_SetPhyLoopbackMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_SETPHYLOOPBACKMODE_EXIT(Retval,TrcvIdx,Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SETPHYTXMODE_ENTRY
/** \brief Entry point of function EthIf_SetPhyTxMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_SETPHYTXMODE_ENTRY(TrcvIdx,Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SETPHYTXMODE_EXIT
/** \brief Exit point of function EthIf_SetPhyTxMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_SETPHYTXMODE_EXIT(Retval,TrcvIdx,Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETTRCVSIGNALQUALITY_ENTRY
/** \brief Entry point of function EthIf_GetTrcvSignalQuality()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_GETTRCVSIGNALQUALITY_ENTRY(TrcvIdx,ResultPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETTRCVSIGNALQUALITY_EXIT
/** \brief Exit point of function EthIf_GetTrcvSignalQuality()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_GETTRCVSIGNALQUALITY_EXIT(Retval,TrcvIdx,ResultPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_CLEARTRCVSIGNALQUALITY_ENTRY
/** \brief Entry point of function EthIf_ClearTrcvSignalQuality()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_CLEARTRCVSIGNALQUALITY_ENTRY(TrcvIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_CLEARTRCVSIGNALQUALITY_EXIT
/** \brief Exit point of function EthIf_ClearTrcvSignalQuality()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_CLEARTRCVSIGNALQUALITY_EXIT(Retval,TrcvIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETSWITCHPORTSIGNALQUALITY_ENTRY
/** \brief Entry point of function EthIf_GetSwitchPortSignalQuality()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_GETSWITCHPORTSIGNALQUALITY_ENTRY(SwitchIdx,SwitchPortIdx,ResultPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETSWITCHPORTSIGNALQUALITY_EXIT
/** \brief Exit point of function EthIf_GetSwitchPortSignalQuality()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_GETSWITCHPORTSIGNALQUALITY_EXIT(Retval,SwitchIdx,SwitchPortIdx,ResultPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_CLEARSWITCHPORTSIGNALQUALITY_ENTRY
/** \brief Entry point of function EthIf_ClearSwitchPortSignalQuality()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_CLEARSWITCHPORTSIGNALQUALITY_ENTRY(SwitchIdx,SwitchPortIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_CLEARSWITCHPORTSIGNALQUALITY_EXIT
/** \brief Exit point of function EthIf_ClearSwitchPortSignalQuality()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_CLEARSWITCHPORTSIGNALQUALITY_EXIT(Retval,SwitchIdx,SwitchPortIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETCABLEDIAGNOSTICSRESULT_ENTRY
/** \brief Entry point of function EthIf_GetCableDiagnosticsResult()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_GETCABLEDIAGNOSTICSRESULT_ENTRY(TrcvIdx,ResultPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETCABLEDIAGNOSTICSRESULT_EXIT
/** \brief Exit point of function EthIf_GetCableDiagnosticsResult()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_GETCABLEDIAGNOSTICSRESULT_EXIT(Retval,TrcvIdx,ResultPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETPHYIDENTIFIER_ENTRY
/** \brief Entry point of function EthIf_GetPhyIdentifier()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_GETPHYIDENTIFIER_ENTRY(TrcvIdx,OrgUniqueIdPtr,ModelNrPtr,RevisionNrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETPHYIDENTIFIER_EXIT
/** \brief Exit point of function EthIf_GetPhyIdentifier()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_GETPHYIDENTIFIER_EXIT(Retval,TrcvIdx,OrgUniqueIdPtr,ModelNrPtr,RevisionNrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SETPHYTESTMODE_ENTRY
/** \brief Entry point of function EthIf_SetPhyTestMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_SETPHYTESTMODE_ENTRY(TrcvIdx,Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SETPHYTESTMODE_EXIT
/** \brief Exit point of function EthIf_SetPhyTestMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_SETPHYTESTMODE_EXIT(Retval,TrcvIdx,Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_UP_RXINDICATIONDUMMY_ENTRY
/** \brief Entry point of function EthIf_Up_RxIndicationDummy()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_UP_RXINDICATIONDUMMY_ENTRY(CtrlIdx,FrameType,IsBroadcast,PhysAddrPtr,DataPtr,LenByte)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_UP_RXINDICATIONDUMMY_EXIT
/** \brief Exit point of function EthIf_Up_RxIndicationDummy()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_UP_RXINDICATIONDUMMY_EXIT(CtrlIdx,FrameType,IsBroadcast,PhysAddrPtr,DataPtr,LenByte)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_UP_TXCONFIRMATIONDUMMY_ENTRY
/** \brief Entry point of function EthIf_Up_TxConfirmationDummy()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_UP_TXCONFIRMATIONDUMMY_ENTRY(CtrlIdx,BufIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_UP_TXCONFIRMATIONDUMMY_EXIT
/** \brief Exit point of function EthIf_Up_TxConfirmationDummy()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_UP_TXCONFIRMATIONDUMMY_EXIT(CtrlIdx,BufIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_UP_TRCVLINKSTATECHGDUMMY_ENTRY
/** \brief Entry point of function EthIf_Up_TrcvLinkStateChgDummy()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_UP_TRCVLINKSTATECHGDUMMY_ENTRY(CtrlIdx,TransceiverLinkState)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_UP_TRCVLINKSTATECHGDUMMY_EXIT
/** \brief Exit point of function EthIf_Up_TrcvLinkStateChgDummy()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_UP_TRCVLINKSTATECHGDUMMY_EXIT(CtrlIdx,TransceiverLinkState)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETFRAMETYPEOWNER_ENTRY
/** \brief Entry point of function EthIf_GetFrameTypeOwner()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_GETFRAMETYPEOWNER_ENTRY(FrameType)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETFRAMETYPEOWNER_EXIT
/** \brief Exit point of function EthIf_GetFrameTypeOwner()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_GETFRAMETYPEOWNER_EXIT(Retval,FrameType)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_READMII_ENTRY
/** \brief Entry point of function EthIf_ReadMii()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_READMII_ENTRY(CtrlIdx,RegIdx,RegValPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_READMII_EXIT
/** \brief Exit point of function EthIf_ReadMii()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_READMII_EXIT(Retval,CtrlIdx,RegIdx,RegValPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_WRITEMII_ENTRY
/** \brief Entry point of function EthIf_WriteMii()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_WRITEMII_ENTRY(CtrlIdx,RegIdx,RegVal)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_WRITEMII_EXIT
/** \brief Exit point of function EthIf_WriteMii()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_WRITEMII_EXIT(Retval,CtrlIdx,RegIdx,RegVal)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETPORTMACADDR_ENTRY
/** \brief Entry point of function EthIf_GetPortMacAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_GETPORTMACADDR_ENTRY(MacAddrPtr,SwitchIdxPtr,PortIdxPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETPORTMACADDR_EXIT
/** \brief Exit point of function EthIf_GetPortMacAddr()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_GETPORTMACADDR_EXIT(Retval,MacAddrPtr,SwitchIdxPtr,PortIdxPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETARLTABLE_ENTRY
/** \brief Entry point of function EthIf_GetArlTable()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_GETARLTABLE_ENTRY(SwitchIdx,numberOfElements,arlTableListPointer)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETARLTABLE_EXIT
/** \brief Exit point of function EthIf_GetArlTable()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_GETARLTABLE_EXIT(Retval,SwitchIdx,numberOfElements,arlTableListPointer)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETBUFFERLEVEL_ENTRY
/** \brief Entry point of function EthIf_GetBufferLevel()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_GETBUFFERLEVEL_ENTRY(SwitchIdx,SwitchBufferLevelPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETBUFFERLEVEL_EXIT
/** \brief Exit point of function EthIf_GetBufferLevel()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_GETBUFFERLEVEL_EXIT(Retval,SwitchIdx,SwitchBufferLevelPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SWTGETCOUNTERVALUES_ENTRY
/** \brief Entry point of function EthIf_SwtGetCounterValues()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_SWTGETCOUNTERVALUES_ENTRY(SwitchIdx,SwitchPortIdx,CounterPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SWTGETCOUNTERVALUES_EXIT
/** \brief Exit point of function EthIf_SwtGetCounterValues()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_SWTGETCOUNTERVALUES_EXIT(Retval,SwitchIdx,SwitchPortIdx,CounterPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_STORECONFIGURATION_ENTRY
/** \brief Entry point of function EthIf_StoreConfiguration()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_STORECONFIGURATION_ENTRY(SwitchIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_STORECONFIGURATION_EXIT
/** \brief Exit point of function EthIf_StoreConfiguration()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_STORECONFIGURATION_EXIT(Retval,SwitchIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_RESETCONFIGURATION_ENTRY
/** \brief Entry point of function EthIf_ResetConfiguration()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_RESETCONFIGURATION_ENTRY(SwitchIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_RESETCONFIGURATION_EXIT
/** \brief Exit point of function EthIf_ResetConfiguration()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_RESETCONFIGURATION_EXIT(Retval,SwitchIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SETTRANSCEIVERWAKEUPMODE_ENTRY
/** \brief Entry point of function EthIf_SetTransceiverWakeupMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_SETTRANSCEIVERWAKEUPMODE_ENTRY(TrcvIdx,TrcvWakeupMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SETTRANSCEIVERWAKEUPMODE_EXIT
/** \brief Exit point of function EthIf_SetTransceiverWakeupMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_SETTRANSCEIVERWAKEUPMODE_EXIT(Retval,TrcvIdx,TrcvWakeupMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETTRANSCEIVERWAKEUPMODE_ENTRY
/** \brief Entry point of function EthIf_GetTransceiverWakeupMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_GETTRANSCEIVERWAKEUPMODE_ENTRY(TrcvIdx,TrcvWakeupModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETTRANSCEIVERWAKEUPMODE_EXIT
/** \brief Exit point of function EthIf_GetTransceiverWakeupMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_GETTRANSCEIVERWAKEUPMODE_EXIT(Retval,TrcvIdx,TrcvWakeupModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_CHECKWAKEUP_ENTRY
/** \brief Entry point of function EthIf_CheckWakeup()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_CHECKWAKEUP_ENTRY(WakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_CHECKWAKEUP_EXIT
/** \brief Exit point of function EthIf_CheckWakeup()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_CHECKWAKEUP_EXIT(Retval,WakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETCTRLIDXLIST_ENTRY
/** \brief Entry point of function EthIf_GetCtrlIdxList()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_GETCTRLIDXLIST_ENTRY(NumberOfCtrlIdx,CtrlIdxListPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETCTRLIDXLIST_EXIT
/** \brief Exit point of function EthIf_GetCtrlIdxList()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_GETCTRLIDXLIST_EXIT(Retval,NumberOfCtrlIdx,CtrlIdxListPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETVLANID_ENTRY
/** \brief Entry point of function EthIf_GetVlanId()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_GETVLANID_ENTRY(CtrlIdx,VlanIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETVLANID_EXIT
/** \brief Exit point of function EthIf_GetVlanId()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_GETVLANID_EXIT(Retval,CtrlIdx,VlanIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETANDRESETMEASUREMENTDATA_ENTRY
/** \brief Entry point of function EthIf_GetAndResetMeasurementData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_GETANDRESETMEASUREMENTDATA_ENTRY(MeasurementIdx,MeasurementResetNeeded,MeasurementDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_GETANDRESETMEASUREMENTDATA_EXIT
/** \brief Exit point of function EthIf_GetAndResetMeasurementData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_GETANDRESETMEASUREMENTDATA_EXIT(Retval,MeasurementIdx,MeasurementResetNeeded,MeasurementDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SWITCHPORTGROUPREQUESTMODE_ENTRY
/** \brief Entry point of function EthIf_SwitchPortGroupRequestMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_SWITCHPORTGROUPREQUESTMODE_ENTRY(PortGroupIdx,PortMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SWITCHPORTGROUPREQUESTMODE_EXIT
/** \brief Exit point of function EthIf_SwitchPortGroupRequestMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_SWITCHPORTGROUPREQUESTMODE_EXIT(Retval,PortGroupIdx,PortMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_STARTALLPORTS_ENTRY
/** \brief Entry point of function EthIf_StartAllPorts()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_STARTALLPORTS_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_STARTALLPORTS_EXIT
/** \brief Exit point of function EthIf_StartAllPorts()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_STARTALLPORTS_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_VERIFYCONFIG_ENTRY
/** \brief Entry point of function EthIf_VerifyConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_VERIFYCONFIG_ENTRY(SwitchIdx,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_VERIFYCONFIG_EXIT
/** \brief Exit point of function EthIf_VerifyConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_VERIFYCONFIG_EXIT(Retval,SwitchIdx,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SETFORWARDINGMODE_ENTRY
/** \brief Entry point of function EthIf_SetForwardingMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_SETFORWARDINGMODE_ENTRY(SwitchIdx,mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SETFORWARDINGMODE_EXIT
/** \brief Exit point of function EthIf_SetForwardingMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_SETFORWARDINGMODE_EXIT(Retval,SwitchIdx,mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_CTRLMODEINDICATION_ENTRY
/** \brief Entry point of function EthIf_CtrlModeIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_CTRLMODEINDICATION_ENTRY(CtrlIdx,CtrlMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_CTRLMODEINDICATION_EXIT
/** \brief Exit point of function EthIf_CtrlModeIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_CTRLMODEINDICATION_EXIT(CtrlIdx,CtrlMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_TRCVMODEINDICATION_ENTRY
/** \brief Entry point of function EthIf_TrcvModeIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_TRCVMODEINDICATION_ENTRY(TrcvIdx,TrcvMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_TRCVMODEINDICATION_EXIT
/** \brief Exit point of function EthIf_TrcvModeIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_TRCVMODEINDICATION_EXIT(TrcvIdx,TrcvMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SWITCHMGMTINFOINDICATION_ENTRY
/** \brief Entry point of function EthIf_SwitchMgmtInfoIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_SWITCHMGMTINFOINDICATION_ENTRY(CtrlIdx,DataPtr,MgmtInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_SWITCHMGMTINFOINDICATION_EXIT
/** \brief Exit point of function EthIf_SwitchMgmtInfoIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_SWITCHMGMTINFOINDICATION_EXIT(CtrlIdx,DataPtr,MgmtInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_ENABLERELATEDETHIFCTRLS_ENTRY
/** \brief Entry point of function EthIf_EnableRelatedEthIfCtrls()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_ENABLERELATEDETHIFCTRLS_ENTRY(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_ENABLERELATEDETHIFCTRLS_EXIT
/** \brief Exit point of function EthIf_EnableRelatedEthIfCtrls()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_ENABLERELATEDETHIFCTRLS_EXIT(Retval,CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_DISABLERELATEDETHIFCTRLS_ENTRY
/** \brief Entry point of function EthIf_DisableRelatedEthIfCtrls()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ETHIF_DISABLERELATEDETHIFCTRLS_ENTRY(CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ETHIF_DISABLERELATEDETHIFCTRLS_EXIT
/** \brief Exit point of function EthIf_DisableRelatedEthIfCtrls()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ETHIF_DISABLERELATEDETHIFCTRLS_EXIT(Retval,CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */



/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef ETHIF_TRACE_H */
/*==================[end of file]===========================================*/
