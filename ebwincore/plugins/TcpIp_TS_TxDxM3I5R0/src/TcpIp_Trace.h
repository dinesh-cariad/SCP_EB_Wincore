/**
 * \file
 *
 * \brief AUTOSAR TcpIp
 *
 * This file contains the implementation of the AUTOSAR
 * module TcpIp.
 *
 * \version 3.5.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef TCPIP_TRACE_H
#define TCPIP_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif

#include <TcpIp_guards.h> /* after foreign/API/Cfg; before internal/private */

/* give tests a chance to disable the internal trace macros completely */
#ifndef TCPIP_DISABLE_INTERNAL_TRACE_MACROS_FOR_TESTING
#include <TcpIp_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function TcpIp_IsValidConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_ISVALIDCONFIG_ENTRY(voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_ISVALIDCONFIG_EXIT
/** \brief Exit point of function TcpIp_IsValidConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_ISVALIDCONFIG_EXIT(Retval,voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_INIT_ENTRY
/** \brief Entry point of function TcpIp_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_INIT_ENTRY(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_INIT_EXIT
/** \brief Exit point of function TcpIp_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_INIT_EXIT(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_MAINFUNCTION_ENTRY
/** \brief Entry point of function TcpIp_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_MAINFUNCTION_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_MAINFUNCTION_EXIT
/** \brief Exit point of function TcpIp_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_MAINFUNCTION_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_MAINFUNCTIONTX_ENTRY
/** \brief Entry point of function TcpIp_MainFunctionTx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_MAINFUNCTIONTX_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_MAINFUNCTIONTX_EXIT
/** \brief Exit point of function TcpIp_MainFunctionTx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_MAINFUNCTIONTX_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_GETVERSIONINFO_ENTRY
/** \brief Entry point of function TcpIp_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_GETVERSIONINFO_ENTRY(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_GETVERSIONINFO_EXIT
/** \brief Exit point of function TcpIp_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_GETVERSIONINFO_EXIT(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_GETANDRESETMEASUREMENTDATA_ENTRY
/** \brief Entry point of function TcpIp_GetAndResetMeasurementData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_GETANDRESETMEASUREMENTDATA_ENTRY(MeasurementIdx,MeasurementResetNeeded,MeasurementDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_GETANDRESETMEASUREMENTDATA_EXIT
/** \brief Exit point of function TcpIp_GetAndResetMeasurementData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_GETANDRESETMEASUREMENTDATA_EXIT(MeasurementIdx,MeasurementResetNeeded,MeasurementDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_RXINDICATION_ENTRY
/** \brief Entry point of function TcpIp_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_RXINDICATION_ENTRY(CtrlIdx,FrameType,IsBroadcast,PhysAddrPtr,DataPtr,LenByte)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_RXINDICATION_EXIT
/** \brief Exit point of function TcpIp_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_RXINDICATION_EXIT(CtrlIdx,FrameType,IsBroadcast,PhysAddrPtr,DataPtr,LenByte)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_DHCPREADOPTION_ENTRY
/** \brief Entry point of function TcpIp_DhcpReadOption()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_DHCPREADOPTION_ENTRY(LocalIpAddrId,Option,DataLength,DataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_DHCPREADOPTION_EXIT
/** \brief Exit point of function TcpIp_DhcpReadOption()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_DHCPREADOPTION_EXIT(Retval,LocalIpAddrId,Option,DataLength,DataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_DHCPWRITEOPTION_ENTRY
/** \brief Entry point of function TcpIp_DhcpWriteOption()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_DHCPWRITEOPTION_ENTRY(LocalIpAddrId,Option,DataLength,DataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_DHCPWRITEOPTION_EXIT
/** \brief Exit point of function TcpIp_DhcpWriteOption()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_DHCPWRITEOPTION_EXIT(Retval,LocalIpAddrId,Option,DataLength,DataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_ICMPTRANSMIT_ENTRY
/** \brief Entry point of function TcpIp_IcmpTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_ICMPTRANSMIT_ENTRY(LocalIpAddrId,RemoteAddrPtr,Ttl,Type,Code,DataLength,DataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_ICMPTRANSMIT_EXIT
/** \brief Exit point of function TcpIp_IcmpTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_ICMPTRANSMIT_EXIT(Retval,LocalIpAddrId,RemoteAddrPtr,Ttl,Type,Code,DataLength,DataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_ICMPTRANSMITERROR_ENTRY
/** \brief Entry point of function TcpIp_IcmpTransmitError()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_ICMPTRANSMITERROR_ENTRY(CtrlIdx,RemoteAddrPtr,Ttl,Type,Code,DataLength,DataPtr,SpecificValue)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_ICMPTRANSMITERROR_EXIT
/** \brief Exit point of function TcpIp_IcmpTransmitError()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_ICMPTRANSMITERROR_EXIT(Retval,CtrlIdx,RemoteAddrPtr,Ttl,Type,Code,DataLength,DataPtr,SpecificValue)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_REQUESTCOMMODE_ENTRY
/** \brief Entry point of function TcpIp_RequestComMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_REQUESTCOMMODE_ENTRY(CtrlIdx,State)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_REQUESTCOMMODE_EXIT
/** \brief Exit point of function TcpIp_RequestComMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_REQUESTCOMMODE_EXIT(Retval,CtrlIdx,State)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_REQUESTIPADDRASSIGNMENT_ENTRY
/** \brief Entry point of function TcpIp_RequestIpAddrAssignment()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_REQUESTIPADDRASSIGNMENT_ENTRY(LocalAddrId,Type,LocalIpAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_REQUESTIPADDRASSIGNMENT_EXIT
/** \brief Exit point of function TcpIp_RequestIpAddrAssignment()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_REQUESTIPADDRASSIGNMENT_EXIT(Retval,LocalAddrId,Type,LocalIpAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_RELEASEIPADDRASSIGNMENT_ENTRY
/** \brief Entry point of function TcpIp_ReleaseIpAddrAssignment()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_RELEASEIPADDRASSIGNMENT_ENTRY(LocalAddrId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_RELEASEIPADDRASSIGNMENT_EXIT
/** \brief Exit point of function TcpIp_ReleaseIpAddrAssignment()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_RELEASEIPADDRASSIGNMENT_EXIT(Retval,LocalAddrId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_GETIPADDR_ENTRY
/** \brief Entry point of function TcpIp_GetIpAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_GETIPADDR_ENTRY(LocalAddrId,IpAddrPtr,NetmaskPtr,DefaultRouterPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_GETIPADDR_EXIT
/** \brief Exit point of function TcpIp_GetIpAddr()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_GETIPADDR_EXIT(Retval,LocalAddrId,IpAddrPtr,NetmaskPtr,DefaultRouterPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_GETPHYSADDR_ENTRY
/** \brief Entry point of function TcpIp_GetPhysAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_GETPHYSADDR_ENTRY(LocalAddrId,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_GETPHYSADDR_EXIT
/** \brief Exit point of function TcpIp_GetPhysAddr()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_GETPHYSADDR_EXIT(Retval,LocalAddrId,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_GETCTRLIDX_ENTRY
/** \brief Entry point of function TcpIp_GetCtrlIdx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_GETCTRLIDX_ENTRY(LocalAddrId,CtrlIdxPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_GETCTRLIDX_EXIT
/** \brief Exit point of function TcpIp_GetCtrlIdx()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_GETCTRLIDX_EXIT(Retval,LocalAddrId,CtrlIdxPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_SETREMOTEPHYSADDR_ENTRY
/** \brief Entry point of function TcpIp_SetRemotePhysAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_SETREMOTEPHYSADDR_ENTRY(CtrlIdx,IpAddrPtr,PhysAddrPtr,State)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_SETREMOTEPHYSADDR_EXIT
/** \brief Exit point of function TcpIp_SetRemotePhysAddr()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_SETREMOTEPHYSADDR_EXIT(Retval,CtrlIdx,IpAddrPtr,PhysAddrPtr,State)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_GETREMOTEPHYSADDR_ENTRY
/** \brief Entry point of function TcpIp_GetRemotePhysAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_GETREMOTEPHYSADDR_ENTRY(CtrlIdx,IpAddrPtr,PhysAddrPtr,initRes)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_GETREMOTEPHYSADDR_EXIT
/** \brief Exit point of function TcpIp_GetRemotePhysAddr()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_GETREMOTEPHYSADDR_EXIT(Retval,CtrlIdx,IpAddrPtr,PhysAddrPtr,initRes)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_GETARPCACHEENTRIES_ENTRY
/** \brief Entry point of function TcpIp_GetArpCacheEntries()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_GETARPCACHEENTRIES_ENTRY(ctrlIdx,numberOfElements,entryListPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_GETARPCACHEENTRIES_EXIT
/** \brief Exit point of function TcpIp_GetArpCacheEntries()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_GETARPCACHEENTRIES_EXIT(Retval,ctrlIdx,numberOfElements,entryListPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_GETNDPCACHEENTRIES_ENTRY
/** \brief Entry point of function TcpIp_GetNdpCacheEntries()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_GETNDPCACHEENTRIES_ENTRY(ctrlIdx,numberOfElements,entryListPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_GETNDPCACHEENTRIES_EXIT
/** \brief Exit point of function TcpIp_GetNdpCacheEntries()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_GETNDPCACHEENTRIES_EXIT(Retval,ctrlIdx,numberOfElements,entryListPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_SOCKET_GETSOCKET_ENTRY
/** \brief Entry point of function TcpIp_Socket_getSocket()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_SOCKET_GETSOCKET_ENTRY(Domain,Protocol,SocketIdPtr,sockOwnerId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_SOCKET_GETSOCKET_EXIT
/** \brief Exit point of function TcpIp_Socket_getSocket()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_SOCKET_GETSOCKET_EXIT(Retval,Domain,Protocol,SocketIdPtr,sockOwnerId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_BIND_ENTRY
/** \brief Entry point of function TcpIp_Bind()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_BIND_ENTRY(SocketId,LocalAddrId,PortPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_BIND_EXIT
/** \brief Exit point of function TcpIp_Bind()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_BIND_EXIT(Retval,SocketId,LocalAddrId,PortPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_CLOSE_ENTRY
/** \brief Entry point of function TcpIp_Close()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_CLOSE_ENTRY(SocketId,Abort)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_CLOSE_EXIT
/** \brief Exit point of function TcpIp_Close()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_CLOSE_EXIT(Retval,SocketId,Abort)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_CHANGEPARAMETER_ENTRY
/** \brief Entry point of function TcpIp_ChangeParameter()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_CHANGEPARAMETER_ENTRY(SocketId,ParameterId,ParameterValue)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_CHANGEPARAMETER_EXIT
/** \brief Exit point of function TcpIp_ChangeParameter()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_CHANGEPARAMETER_EXIT(Retval,SocketId,ParameterId,ParameterValue)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_TCPCONNECT_ENTRY
/** \brief Entry point of function TcpIp_TcpConnect()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_TCPCONNECT_ENTRY(SocketId,RemoteAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_TCPCONNECT_EXIT
/** \brief Exit point of function TcpIp_TcpConnect()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_TCPCONNECT_EXIT(Retval,SocketId,RemoteAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_TCPLISTEN_ENTRY
/** \brief Entry point of function TcpIp_TcpListen()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_TCPLISTEN_ENTRY(SocketId,MaxChannels)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_TCPLISTEN_EXIT
/** \brief Exit point of function TcpIp_TcpListen()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_TCPLISTEN_EXIT(Retval,SocketId,MaxChannels)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_TCPRECEIVED_ENTRY
/** \brief Entry point of function TcpIp_TcpReceived()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_TCPRECEIVED_ENTRY(SocketId,Length)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_TCPRECEIVED_EXIT
/** \brief Exit point of function TcpIp_TcpReceived()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_TCPRECEIVED_EXIT(Retval,SocketId,Length)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_TCPTRANSMIT_ENTRY
/** \brief Entry point of function TcpIp_TcpTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_TCPTRANSMIT_ENTRY(SocketId,DataPtr,AvailableLength,ForceRetrieve)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_TCPTRANSMIT_EXIT
/** \brief Exit point of function TcpIp_TcpTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_TCPTRANSMIT_EXIT(Retval,SocketId,DataPtr,AvailableLength,ForceRetrieve)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_UDPTRANSMIT_ENTRY
/** \brief Entry point of function TcpIp_UdpTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_UDPTRANSMIT_ENTRY(SocketId,DataPtr,RemoteAddrPtr,TotalLength)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_UDPTRANSMIT_EXIT
/** \brief Exit point of function TcpIp_UdpTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_UDPTRANSMIT_EXIT(Retval,SocketId,DataPtr,RemoteAddrPtr,TotalLength)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_DHCPGETSTATUS_ENTRY
/** \brief Entry point of function TcpIp_DhcpGetStatus()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_DHCPGETSTATUS_ENTRY(localAddressId,statusPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_DHCPGETSTATUS_EXIT
/** \brief Exit point of function TcpIp_DhcpGetStatus()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_DHCPGETSTATUS_EXIT(Retval,localAddressId,statusPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_REQUESTIPSECMODE_ENTRY
/** \brief Entry point of function TcpIp_RequestIpSecMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_REQUESTIPSECMODE_ENTRY(conId,reqState)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_REQUESTIPSECMODE_EXIT
/** \brief Exit point of function TcpIp_RequestIpSecMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_REQUESTIPSECMODE_EXIT(Retval,conId,reqState)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_ISCONNECTIONREADY_ENTRY
/** \brief Entry point of function TcpIp_IsConnectionReady()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TCPIP_ISCONNECTIONREADY_ENTRY(SocketId,RemoteAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TCPIP_ISCONNECTIONREADY_EXIT
/** \brief Exit point of function TcpIp_IsConnectionReady()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TCPIP_ISCONNECTIONREADY_EXIT(Retval,SocketId,RemoteAddrPtr)
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
#else /* TCPIP_DISABLE_INTERNAL_TRACE_MACROS_FOR_TESTING */
#define TCPIP_TRACE_H_DISABLED
#endif /* TCPIP_DISABLE_INTERNAL_TRACE_MACROS_FOR_TESTING */
#endif /* ifndef TCPIP_TRACE_H */
/*==================[end of file]===========================================*/
