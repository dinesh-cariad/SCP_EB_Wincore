/**
 * \file
 *
 * \brief AUTOSAR SoAd
 *
 * This file contains the implementation of the AUTOSAR
 * module SoAd.
 *
 * \version 1.8.16
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef SOAD_TRACE_H
#define SOAD_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <SoAd_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_GETVERSIONINFO_ENTRY
/** \brief Entry point of function SoAd_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_GETVERSIONINFO_ENTRY(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_GETVERSIONINFO_EXIT
/** \brief Exit point of function SoAd_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_GETVERSIONINFO_EXIT(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_INIT_ENTRY
/** \brief Entry point of function SoAd_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_INIT_ENTRY(SoAdConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_INIT_EXIT
/** \brief Exit point of function SoAd_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_INIT_EXIT(SoAdConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function SoAd_IsValidConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_ISVALIDCONFIG_ENTRY(SoAdConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_ISVALIDCONFIG_EXIT
/** \brief Exit point of function SoAd_IsValidConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_ISVALIDCONFIG_EXIT(Retval,SoAdConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_IFTRANSMIT_ENTRY
/** \brief Entry point of function SoAd_IfTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_IFTRANSMIT_ENTRY(SoAdSrcPduId,SoAdSrcPduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_IFTRANSMIT_EXIT
/** \brief Exit point of function SoAd_IfTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_IFTRANSMIT_EXIT(Retval,SoAdSrcPduId,SoAdSrcPduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_IFROUTINGGROUPTRANSMIT_ENTRY
/** \brief Entry point of function SoAd_IfRoutingGroupTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_IFROUTINGGROUPTRANSMIT_ENTRY(id)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_IFROUTINGGROUPTRANSMIT_EXIT
/** \brief Exit point of function SoAd_IfRoutingGroupTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_IFROUTINGGROUPTRANSMIT_EXIT(Retval,id)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_IFSPECIFICROUTINGGROUPTRANSMIT_ENTRY
/** \brief Entry point of function SoAd_IfSpecificRoutingGroupTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_IFSPECIFICROUTINGGROUPTRANSMIT_ENTRY(id,SoConId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_IFSPECIFICROUTINGGROUPTRANSMIT_EXIT
/** \brief Exit point of function SoAd_IfSpecificRoutingGroupTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_IFSPECIFICROUTINGGROUPTRANSMIT_EXIT(Retval,id,SoConId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_ISCONNECTIONREADY_ENTRY
/** \brief Entry point of function SoAd_IsConnectionReady()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_ISCONNECTIONREADY_ENTRY(SoConId,RemoteAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_ISCONNECTIONREADY_EXIT
/** \brief Exit point of function SoAd_IsConnectionReady()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_ISCONNECTIONREADY_EXIT(Retval,SoConId,RemoteAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_TPTRANSMIT_ENTRY
/** \brief Entry point of function SoAd_TpTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_TPTRANSMIT_ENTRY(SoAdSrcPduId,SoAdSrcPduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_TPTRANSMIT_EXIT
/** \brief Exit point of function SoAd_TpTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_TPTRANSMIT_EXIT(Retval,SoAdSrcPduId,SoAdSrcPduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_TPCANCELTRANSMIT_ENTRY
/** \brief Entry point of function SoAd_TpCancelTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_TPCANCELTRANSMIT_ENTRY(PduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_TPCANCELTRANSMIT_EXIT
/** \brief Exit point of function SoAd_TpCancelTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_TPCANCELTRANSMIT_EXIT(Retval,PduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_TPCANCELRECEIVE_ENTRY
/** \brief Entry point of function SoAd_TpCancelReceive()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_TPCANCELRECEIVE_ENTRY(PduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_TPCANCELRECEIVE_EXIT
/** \brief Exit point of function SoAd_TpCancelReceive()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_TPCANCELRECEIVE_EXIT(Retval,PduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_GETSOCONID_ENTRY
/** \brief Entry point of function SoAd_GetSoConId()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_GETSOCONID_ENTRY(TxPduId,SoConIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_GETSOCONID_EXIT
/** \brief Exit point of function SoAd_GetSoConId()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_GETSOCONID_EXIT(Retval,TxPduId,SoConIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_GETSOCONMODE_ENTRY
/** \brief Entry point of function SoAd_GetSoConMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_GETSOCONMODE_ENTRY(SoConId,ModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_GETSOCONMODE_EXIT
/** \brief Exit point of function SoAd_GetSoConMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_GETSOCONMODE_EXIT(Retval,SoConId,ModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_OPENSOCON_ENTRY
/** \brief Entry point of function SoAd_OpenSoCon()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_OPENSOCON_ENTRY(SoConId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_OPENSOCON_EXIT
/** \brief Exit point of function SoAd_OpenSoCon()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_OPENSOCON_EXIT(Retval,SoConId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_CLOSESOCON_ENTRY
/** \brief Entry point of function SoAd_CloseSoCon()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_CLOSESOCON_ENTRY(SoConId,Abort)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_CLOSESOCON_EXIT
/** \brief Exit point of function SoAd_CloseSoCon()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_CLOSESOCON_EXIT(Retval,SoConId,Abort)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_REQUESTIPADDRASSIGNMENT_ENTRY
/** \brief Entry point of function SoAd_RequestIpAddrAssignment()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_REQUESTIPADDRASSIGNMENT_ENTRY(SoConId,Type,LocalIpAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_REQUESTIPADDRASSIGNMENT_EXIT
/** \brief Exit point of function SoAd_RequestIpAddrAssignment()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_REQUESTIPADDRASSIGNMENT_EXIT(Retval,SoConId,Type,LocalIpAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_RELEASEIPADDRASSIGNMENT_ENTRY
/** \brief Entry point of function SoAd_ReleaseIpAddrAssignment()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_RELEASEIPADDRASSIGNMENT_ENTRY(SoConId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_RELEASEIPADDRASSIGNMENT_EXIT
/** \brief Exit point of function SoAd_ReleaseIpAddrAssignment()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_RELEASEIPADDRASSIGNMENT_EXIT(Retval,SoConId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_GETLOCALADDR_ENTRY
/** \brief Entry point of function SoAd_GetLocalAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_GETLOCALADDR_ENTRY(SoConId,LocalAddrPtr,NetmaskPtr,DefaultRouterPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_GETLOCALADDR_EXIT
/** \brief Exit point of function SoAd_GetLocalAddr()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_GETLOCALADDR_EXIT(Retval,SoConId,LocalAddrPtr,NetmaskPtr,DefaultRouterPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_GETPHYSADDR_ENTRY
/** \brief Entry point of function SoAd_GetPhysAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_GETPHYSADDR_ENTRY(SoConId,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_GETPHYSADDR_EXIT
/** \brief Exit point of function SoAd_GetPhysAddr()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_GETPHYSADDR_EXIT(Retval,SoConId,PhysAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_ENABLEROUTING_ENTRY
/** \brief Entry point of function SoAd_EnableRouting()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_ENABLEROUTING_ENTRY(id)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_ENABLEROUTING_EXIT
/** \brief Exit point of function SoAd_EnableRouting()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_ENABLEROUTING_EXIT(Retval,id)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_ENABLESPECIFICROUTING_ENTRY
/** \brief Entry point of function SoAd_EnableSpecificRouting()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_ENABLESPECIFICROUTING_ENTRY(id,SoConId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_ENABLESPECIFICROUTING_EXIT
/** \brief Exit point of function SoAd_EnableSpecificRouting()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_ENABLESPECIFICROUTING_EXIT(Retval,id,SoConId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_DISABLEROUTING_ENTRY
/** \brief Entry point of function SoAd_DisableRouting()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_DISABLEROUTING_ENTRY(id)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_DISABLEROUTING_EXIT
/** \brief Exit point of function SoAd_DisableRouting()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_DISABLEROUTING_EXIT(Retval,id)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_DISABLESPECIFICROUTING_ENTRY
/** \brief Entry point of function SoAd_DisableSpecificRouting()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_DISABLESPECIFICROUTING_ENTRY(id,SoConId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_DISABLESPECIFICROUTING_EXIT
/** \brief Exit point of function SoAd_DisableSpecificRouting()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_DISABLESPECIFICROUTING_EXIT(Retval,id,SoConId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_SETREMOTEADDR_ENTRY
/** \brief Entry point of function SoAd_SetRemoteAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_SETREMOTEADDR_ENTRY(SoConId,IpAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_SETREMOTEADDR_EXIT
/** \brief Exit point of function SoAd_SetRemoteAddr()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_SETREMOTEADDR_EXIT(Retval,SoConId,IpAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_SETUNIQUEREMOTEADDR_ENTRY
/** \brief Entry point of function SoAd_SetUniqueRemoteAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_SETUNIQUEREMOTEADDR_ENTRY(SoConId,RemoteAddrPtr,AssignedSoConIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_SETUNIQUEREMOTEADDR_EXIT
/** \brief Exit point of function SoAd_SetUniqueRemoteAddr()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_SETUNIQUEREMOTEADDR_EXIT(Retval,SoConId,RemoteAddrPtr,AssignedSoConIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_TPCHANGEPARAMETER_ENTRY
/** \brief Entry point of function SoAd_TpChangeParameter()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_TPCHANGEPARAMETER_ENTRY(id,parameter,value)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_TPCHANGEPARAMETER_EXIT
/** \brief Exit point of function SoAd_TpChangeParameter()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_TPCHANGEPARAMETER_EXIT(Retval,id,parameter,value)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_CHANGEPARAMETER_ENTRY
/** \brief Entry point of function SoAd_ChangeParameter()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_CHANGEPARAMETER_ENTRY(SoConId,ParameterId,ParameterValuePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_CHANGEPARAMETER_EXIT
/** \brief Exit point of function SoAd_ChangeParameter()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_CHANGEPARAMETER_EXIT(Retval,SoConId,ParameterId,ParameterValuePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_READDHCPHOSTNAMEOPTION_ENTRY
/** \brief Entry point of function SoAd_ReadDhcpHostNameOption()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_READDHCPHOSTNAMEOPTION_ENTRY(SoConId,length,data)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_READDHCPHOSTNAMEOPTION_EXIT
/** \brief Exit point of function SoAd_ReadDhcpHostNameOption()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_READDHCPHOSTNAMEOPTION_EXIT(Retval,SoConId,length,data)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_WRITEDHCPHOSTNAMEOPTION_ENTRY
/** \brief Entry point of function SoAd_WriteDhcpHostNameOption()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_WRITEDHCPHOSTNAMEOPTION_ENTRY(SoConId,length,data)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_WRITEDHCPHOSTNAMEOPTION_EXIT
/** \brief Exit point of function SoAd_WriteDhcpHostNameOption()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_WRITEDHCPHOSTNAMEOPTION_EXIT(Retval,SoConId,length,data)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_RELEASEREMOTEADDR_ENTRY
/** \brief Entry point of function SoAd_ReleaseRemoteAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_RELEASEREMOTEADDR_ENTRY(SoConId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_RELEASEREMOTEADDR_EXIT
/** \brief Exit point of function SoAd_ReleaseRemoteAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_RELEASEREMOTEADDR_EXIT(SoConId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_MAINFUNCTION_ENTRY
/** \brief Entry point of function SoAd_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_MAINFUNCTION_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_MAINFUNCTION_EXIT
/** \brief Exit point of function SoAd_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_MAINFUNCTION_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_MAINFUNCTIONTX_ENTRY
/** \brief Entry point of function SoAd_MainFunctionTx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_MAINFUNCTIONTX_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_MAINFUNCTIONTX_EXIT
/** \brief Exit point of function SoAd_MainFunctionTx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_MAINFUNCTIONTX_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_GETREMOTEADDR_ENTRY
/** \brief Entry point of function SoAd_GetRemoteAddr()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_GETREMOTEADDR_ENTRY(SoConId,IpAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_GETREMOTEADDR_EXIT
/** \brief Exit point of function SoAd_GetRemoteAddr()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_GETREMOTEADDR_EXIT(Retval,SoConId,IpAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_RXINDICATION_ENTRY
/** \brief Entry point of function SoAd_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_RXINDICATION_ENTRY(SocketId,RemoteAddrPtr,BufPtr,Length)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_RXINDICATION_EXIT
/** \brief Exit point of function SoAd_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_RXINDICATION_EXIT(SocketId,RemoteAddrPtr,BufPtr,Length)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_COPYTXDATA_ENTRY
/** \brief Entry point of function SoAd_CopyTxData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_COPYTXDATA_ENTRY(SocketId,BufPtr,BufLength)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_COPYTXDATA_EXIT
/** \brief Exit point of function SoAd_CopyTxData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_COPYTXDATA_EXIT(Retval,SocketId,BufPtr,BufLength)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_TXCONFIRMATION_ENTRY
/** \brief Entry point of function SoAd_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_TXCONFIRMATION_ENTRY(SocketId,Length)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_TXCONFIRMATION_EXIT
/** \brief Exit point of function SoAd_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_TXCONFIRMATION_EXIT(SocketId,Length)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_TCPACCEPTED_ENTRY
/** \brief Entry point of function SoAd_TcpAccepted()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_TCPACCEPTED_ENTRY(SocketId,SocketIdConnected,RemoteAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_TCPACCEPTED_EXIT
/** \brief Exit point of function SoAd_TcpAccepted()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_TCPACCEPTED_EXIT(Retval,SocketId,SocketIdConnected,RemoteAddrPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_TCPTLSACCEPTED_ENTRY
/** \brief Entry point of function SoAd_TcpTlsAccepted()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_TCPTLSACCEPTED_ENTRY(SocketIdConnected)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_TCPTLSACCEPTED_EXIT
/** \brief Exit point of function SoAd_TcpTlsAccepted()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_TCPTLSACCEPTED_EXIT(Retval,SocketIdConnected)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_TCPCONNECTED_ENTRY
/** \brief Entry point of function SoAd_TcpConnected()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_TCPCONNECTED_ENTRY(SocketId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_TCPCONNECTED_EXIT
/** \brief Exit point of function SoAd_TcpConnected()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_TCPCONNECTED_EXIT(SocketId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_TCPIPEVENT_ENTRY
/** \brief Entry point of function SoAd_TcpIpEvent()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_TCPIPEVENT_ENTRY(SocketId,Event)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_TCPIPEVENT_EXIT
/** \brief Exit point of function SoAd_TcpIpEvent()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_TCPIPEVENT_EXIT(SocketId,Event)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_LOCALIPADDRASSIGNMENTCHG_ENTRY
/** \brief Entry point of function SoAd_LocalIpAddrAssignmentChg()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_LOCALIPADDRASSIGNMENTCHG_ENTRY(IpAddrId,State)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_LOCALIPADDRASSIGNMENTCHG_EXIT
/** \brief Exit point of function SoAd_LocalIpAddrAssignmentChg()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_LOCALIPADDRASSIGNMENTCHG_EXIT(IpAddrId,State)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_GETANDRESETMEASUREMENTDATA_ENTRY
/** \brief Entry point of function SoAd_GetAndResetMeasurementData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SOAD_GETANDRESETMEASUREMENTDATA_ENTRY(MeasurementIdx,MeasurementResetNeeded,MeasurementDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SOAD_GETANDRESETMEASUREMENTDATA_EXIT
/** \brief Exit point of function SoAd_GetAndResetMeasurementData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_SOAD_GETANDRESETMEASUREMENTDATA_EXIT(Retval,MeasurementIdx,MeasurementResetNeeded,MeasurementDataPtr)
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

#endif /* ifndef SOAD_TRACE_H */
/*==================[end of file]===========================================*/
