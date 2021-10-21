/**
 * \file
 *
 * \brief AUTOSAR UdpNm
 *
 * This file contains the implementation of the AUTOSAR
 * module UdpNm.
 *
 * \version 2.9.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef UDPNM_TRACE_H
#define UDPNM_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <UdpNm_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_PASSIVESTARTUP_ENTRY
/** \brief Entry point of function UdpNm_PassiveStartUp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_PASSIVESTARTUP_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_PASSIVESTARTUP_EXIT
/** \brief Exit point of function UdpNm_PassiveStartUp()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_UDPNM_PASSIVESTARTUP_EXIT(Retval,nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_NETWORKREQUEST_ENTRY
/** \brief Entry point of function UdpNm_NetworkRequest()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_NETWORKREQUEST_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_NETWORKREQUEST_EXIT
/** \brief Exit point of function UdpNm_NetworkRequest()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_UDPNM_NETWORKREQUEST_EXIT(Retval,nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_NETWORKGWERAREQUEST_ENTRY
/** \brief Entry point of function UdpNm_NetworkGwEraRequest()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_NETWORKGWERAREQUEST_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_NETWORKGWERAREQUEST_EXIT
/** \brief Exit point of function UdpNm_NetworkGwEraRequest()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_UDPNM_NETWORKGWERAREQUEST_EXIT(Retval,nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_NETWORKRELEASE_ENTRY
/** \brief Entry point of function UdpNm_NetworkRelease()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_NETWORKRELEASE_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_NETWORKRELEASE_EXIT
/** \brief Exit point of function UdpNm_NetworkRelease()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_UDPNM_NETWORKRELEASE_EXIT(Retval,nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_DISABLECOMMUNICATION_ENTRY
/** \brief Entry point of function UdpNm_DisableCommunication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_DISABLECOMMUNICATION_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_DISABLECOMMUNICATION_EXIT
/** \brief Exit point of function UdpNm_DisableCommunication()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_UDPNM_DISABLECOMMUNICATION_EXIT(Retval,nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_ENABLECOMMUNICATION_ENTRY
/** \brief Entry point of function UdpNm_EnableCommunication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_ENABLECOMMUNICATION_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_ENABLECOMMUNICATION_EXIT
/** \brief Exit point of function UdpNm_EnableCommunication()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_UDPNM_ENABLECOMMUNICATION_EXIT(Retval,nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_REPEATMESSAGEREQUEST_ENTRY
/** \brief Entry point of function UdpNm_RepeatMessageRequest()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_REPEATMESSAGEREQUEST_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_REPEATMESSAGEREQUEST_EXIT
/** \brief Exit point of function UdpNm_RepeatMessageRequest()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_UDPNM_REPEATMESSAGEREQUEST_EXIT(Retval,nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_GETSTATE_ENTRY
/** \brief Entry point of function UdpNm_GetState()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_GETSTATE_ENTRY(nmChannelHandle,nmStatePtr,nmModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_GETSTATE_EXIT
/** \brief Exit point of function UdpNm_GetState()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_UDPNM_GETSTATE_EXIT(Retval,nmChannelHandle,nmStatePtr,nmModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_REQUESTBUSSYNCHRONIZATION_ENTRY
/** \brief Entry point of function UdpNm_RequestBusSynchronization()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_REQUESTBUSSYNCHRONIZATION_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_REQUESTBUSSYNCHRONIZATION_EXIT
/** \brief Exit point of function UdpNm_RequestBusSynchronization()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_UDPNM_REQUESTBUSSYNCHRONIZATION_EXIT(Retval,nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_CHECKREMOTESLEEPINDICATION_ENTRY
/** \brief Entry point of function UdpNm_CheckRemoteSleepIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_CHECKREMOTESLEEPINDICATION_ENTRY(nmChannelHandle,nmRemoteSleepIndPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_CHECKREMOTESLEEPINDICATION_EXIT
/** \brief Exit point of function UdpNm_CheckRemoteSleepIndication()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_UDPNM_CHECKREMOTESLEEPINDICATION_EXIT(Retval,nmChannelHandle,nmRemoteSleepIndPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_INIT_ENTRY
/** \brief Entry point of function UdpNm_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_INIT_ENTRY(udpnmConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_INIT_EXIT
/** \brief Exit point of function UdpNm_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_INIT_EXIT(udpnmConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_GETNODEIDENTIFIER_ENTRY
/** \brief Entry point of function UdpNm_GetNodeIdentifier()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_GETNODEIDENTIFIER_ENTRY(nmChannelHandle,nmNodeIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_GETNODEIDENTIFIER_EXIT
/** \brief Exit point of function UdpNm_GetNodeIdentifier()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_UDPNM_GETNODEIDENTIFIER_EXIT(Retval,nmChannelHandle,nmNodeIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_GETLOCALNODEIDENTIFIER_ENTRY
/** \brief Entry point of function UdpNm_GetLocalNodeIdentifier()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_GETLOCALNODEIDENTIFIER_ENTRY(nmChannelHandle,nmNodeIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_GETLOCALNODEIDENTIFIER_EXIT
/** \brief Exit point of function UdpNm_GetLocalNodeIdentifier()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_UDPNM_GETLOCALNODEIDENTIFIER_EXIT(Retval,nmChannelHandle,nmNodeIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_GETUSERDATA_ENTRY
/** \brief Entry point of function UdpNm_GetUserData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_GETUSERDATA_ENTRY(nmChannelHandle,nmUserDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_GETUSERDATA_EXIT
/** \brief Exit point of function UdpNm_GetUserData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_UDPNM_GETUSERDATA_EXIT(Retval,nmChannelHandle,nmUserDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_SETUSERDATA_ENTRY
/** \brief Entry point of function UdpNm_SetUserData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_SETUSERDATA_ENTRY(nmChannelHandle,nmUserDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_SETUSERDATA_EXIT
/** \brief Exit point of function UdpNm_SetUserData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_UDPNM_SETUSERDATA_EXIT(Retval,nmChannelHandle,nmUserDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_TRANSMIT_ENTRY
/** \brief Entry point of function UdpNm_Transmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_TRANSMIT_ENTRY(UdpNmTxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_TRANSMIT_EXIT
/** \brief Exit point of function UdpNm_Transmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_UDPNM_TRANSMIT_EXIT(Retval,UdpNmTxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_GETPDUDATA_ENTRY
/** \brief Entry point of function UdpNm_GetPduData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_GETPDUDATA_ENTRY(nmChannelHandle,nmPduDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_GETPDUDATA_EXIT
/** \brief Exit point of function UdpNm_GetPduData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_UDPNM_GETPDUDATA_EXIT(Retval,nmChannelHandle,nmPduDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_GETVERSIONINFO_ENTRY
/** \brief Entry point of function UdpNm_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_GETVERSIONINFO_ENTRY(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_GETVERSIONINFO_EXIT
/** \brief Exit point of function UdpNm_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_GETVERSIONINFO_EXIT(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_SOADIFTXCONFIRMATION_ENTRY
/** \brief Entry point of function UdpNm_SoAdIfTxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_SOADIFTXCONFIRMATION_ENTRY(TxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_SOADIFTXCONFIRMATION_EXIT
/** \brief Exit point of function UdpNm_SoAdIfTxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_SOADIFTXCONFIRMATION_EXIT(TxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_SOADIFRXINDICATION_ENTRY
/** \brief Entry point of function UdpNm_SoAdIfRxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_SOADIFRXINDICATION_ENTRY(RxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_UDPNM_SOADIFRXINDICATION_EXIT
/** \brief Exit point of function UdpNm_SoAdIfRxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_UDPNM_SOADIFRXINDICATION_EXIT(RxPduId,PduInfoPtr)
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

#endif /* ifndef UDPNM_TRACE_H */
/*==================[end of file]===========================================*/
