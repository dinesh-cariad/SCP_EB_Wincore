/**
 * \file
 *
 * \brief AUTOSAR Nm
 *
 * This file contains the implementation of the AUTOSAR
 * module Nm.
 *
 * \version 5.12.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef NM_TRACE_H
#define NM_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <Nm_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_INIT_ENTRY
/** \brief Entry point of function Nm_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_INIT_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_INIT_EXIT
/** \brief Exit point of function Nm_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_INIT_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_PASSIVESTARTUP_ENTRY
/** \brief Entry point of function Nm_PassiveStartUp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_PASSIVESTARTUP_ENTRY(NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_PASSIVESTARTUP_EXIT
/** \brief Exit point of function Nm_PassiveStartUp()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_NM_PASSIVESTARTUP_EXIT(Retval,NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_NETWORKREQUEST_ENTRY
/** \brief Entry point of function Nm_NetworkRequest()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_NETWORKREQUEST_ENTRY(NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_NETWORKREQUEST_EXIT
/** \brief Exit point of function Nm_NetworkRequest()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_NM_NETWORKREQUEST_EXIT(Retval,NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_NETWORKRELEASE_ENTRY
/** \brief Entry point of function Nm_NetworkRelease()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_NETWORKRELEASE_ENTRY(NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_NETWORKRELEASE_EXIT
/** \brief Exit point of function Nm_NetworkRelease()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_NM_NETWORKRELEASE_EXIT(Retval,NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_NETWORKGWERAREQUEST_ENTRY
/** \brief Entry point of function Nm_NetworkGwEraRequest()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_NETWORKGWERAREQUEST_ENTRY(NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_NETWORKGWERAREQUEST_EXIT
/** \brief Exit point of function Nm_NetworkGwEraRequest()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_NM_NETWORKGWERAREQUEST_EXIT(Retval,NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_DISABLECOMMUNICATION_ENTRY
/** \brief Entry point of function Nm_DisableCommunication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_DISABLECOMMUNICATION_ENTRY(NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_DISABLECOMMUNICATION_EXIT
/** \brief Exit point of function Nm_DisableCommunication()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_NM_DISABLECOMMUNICATION_EXIT(Retval,NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_ENABLECOMMUNICATION_ENTRY
/** \brief Entry point of function Nm_EnableCommunication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_ENABLECOMMUNICATION_ENTRY(NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_ENABLECOMMUNICATION_EXIT
/** \brief Exit point of function Nm_EnableCommunication()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_NM_ENABLECOMMUNICATION_EXIT(Retval,NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_SETUSERDATA_ENTRY
/** \brief Entry point of function Nm_SetUserData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_SETUSERDATA_ENTRY(NetworkHandle,nmUserDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_SETUSERDATA_EXIT
/** \brief Exit point of function Nm_SetUserData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_NM_SETUSERDATA_EXIT(Retval,NetworkHandle,nmUserDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_GETUSERDATA_ENTRY
/** \brief Entry point of function Nm_GetUserData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_GETUSERDATA_ENTRY(NetworkHandle,nmUserDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_GETUSERDATA_EXIT
/** \brief Exit point of function Nm_GetUserData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_NM_GETUSERDATA_EXIT(Retval,NetworkHandle,nmUserDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_GETPDUDATA_ENTRY
/** \brief Entry point of function Nm_GetPduData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_GETPDUDATA_ENTRY(NetworkHandle,nmPduData)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_GETPDUDATA_EXIT
/** \brief Exit point of function Nm_GetPduData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_NM_GETPDUDATA_EXIT(Retval,NetworkHandle,nmPduData)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_REPEATMESSAGEREQUEST_ENTRY
/** \brief Entry point of function Nm_RepeatMessageRequest()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_REPEATMESSAGEREQUEST_ENTRY(NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_REPEATMESSAGEREQUEST_EXIT
/** \brief Exit point of function Nm_RepeatMessageRequest()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_NM_REPEATMESSAGEREQUEST_EXIT(Retval,NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_GETNODEIDENTIFIER_ENTRY
/** \brief Entry point of function Nm_GetNodeIdentifier()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_GETNODEIDENTIFIER_ENTRY(NetworkHandle,nmNodeIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_GETNODEIDENTIFIER_EXIT
/** \brief Exit point of function Nm_GetNodeIdentifier()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_NM_GETNODEIDENTIFIER_EXIT(Retval,NetworkHandle,nmNodeIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_GETLOCALNODEIDENTIFIER_ENTRY
/** \brief Entry point of function Nm_GetLocalNodeIdentifier()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_GETLOCALNODEIDENTIFIER_ENTRY(NetworkHandle,nmNodeIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_GETLOCALNODEIDENTIFIER_EXIT
/** \brief Exit point of function Nm_GetLocalNodeIdentifier()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_NM_GETLOCALNODEIDENTIFIER_EXIT(Retval,NetworkHandle,nmNodeIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_CHECKREMOTESLEEPINDICATION_ENTRY
/** \brief Entry point of function Nm_CheckRemoteSleepIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_CHECKREMOTESLEEPINDICATION_ENTRY(nmNetworkHandle,nmRemoteSleepIndPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_CHECKREMOTESLEEPINDICATION_EXIT
/** \brief Exit point of function Nm_CheckRemoteSleepIndication()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_NM_CHECKREMOTESLEEPINDICATION_EXIT(Retval,nmNetworkHandle,nmRemoteSleepIndPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_GETCOORDINATORSTATE_ENTRY
/** \brief Entry point of function Nm_GetCoordinatorState()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_GETCOORDINATORSTATE_ENTRY(nmCoordinatorId,nmStatePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_GETCOORDINATORSTATE_EXIT
/** \brief Exit point of function Nm_GetCoordinatorState()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_NM_GETCOORDINATORSTATE_EXIT(Retval,nmCoordinatorId,nmStatePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_GETSTATE_ENTRY
/** \brief Entry point of function Nm_GetState()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_GETSTATE_ENTRY(nmNetworkHandle,nmStatePtr,nmModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_GETSTATE_EXIT
/** \brief Exit point of function Nm_GetState()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_NM_GETSTATE_EXIT(Retval,nmNetworkHandle,nmStatePtr,nmModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_STATECHANGENOTIFICATION_ENTRY
/** \brief Entry point of function Nm_StateChangeNotification()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_STATECHANGENOTIFICATION_ENTRY(nmNetworkHandle,nmStatePtr,nmModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_STATECHANGENOTIFICATION_EXIT
/** \brief Exit point of function Nm_StateChangeNotification()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_NM_STATECHANGENOTIFICATION_EXIT(nmNetworkHandle,nmStatePtr,nmModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_MAINFUNCTION_ENTRY
/** \brief Entry point of function Nm_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_MAINFUNCTION_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_MAINFUNCTION_EXIT
/** \brief Exit point of function Nm_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_MAINFUNCTION_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_NETWORKMODE_ENTRY
/** \brief Entry point of function Nm_NetworkMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_NETWORKMODE_ENTRY(nmNetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_NETWORKMODE_EXIT
/** \brief Exit point of function Nm_NetworkMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_NETWORKMODE_EXIT(nmNetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_PREPAREBUSSLEEPMODE_ENTRY
/** \brief Entry point of function Nm_PrepareBusSleepMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_PREPAREBUSSLEEPMODE_ENTRY(nmNetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_PREPAREBUSSLEEPMODE_EXIT
/** \brief Exit point of function Nm_PrepareBusSleepMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_PREPAREBUSSLEEPMODE_EXIT(nmNetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_BUSSLEEPMODE_ENTRY
/** \brief Entry point of function Nm_BusSleepMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_BUSSLEEPMODE_ENTRY(nmNetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_BUSSLEEPMODE_EXIT
/** \brief Exit point of function Nm_BusSleepMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_BUSSLEEPMODE_EXIT(nmNetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_NETWORKSTARTINDICATION_ENTRY
/** \brief Entry point of function Nm_NetworkStartIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_NETWORKSTARTINDICATION_ENTRY(nmNetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_NETWORKSTARTINDICATION_EXIT
/** \brief Exit point of function Nm_NetworkStartIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_NETWORKSTARTINDICATION_EXIT(nmNetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_REMOTESLEEPINDICATION_ENTRY
/** \brief Entry point of function Nm_RemoteSleepIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_REMOTESLEEPINDICATION_ENTRY(nmNetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_REMOTESLEEPINDICATION_EXIT
/** \brief Exit point of function Nm_RemoteSleepIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_REMOTESLEEPINDICATION_EXIT(nmNetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_REMOTESLEEPCANCELLATION_ENTRY
/** \brief Entry point of function Nm_RemoteSleepCancellation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_REMOTESLEEPCANCELLATION_ENTRY(nmNetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_REMOTESLEEPCANCELLATION_EXIT
/** \brief Exit point of function Nm_RemoteSleepCancellation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_REMOTESLEEPCANCELLATION_EXIT(nmNetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_SYNCHRONIZATIONPOINT_ENTRY
/** \brief Entry point of function Nm_SynchronizationPoint()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_SYNCHRONIZATIONPOINT_ENTRY(nmNetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_SYNCHRONIZATIONPOINT_EXIT
/** \brief Exit point of function Nm_SynchronizationPoint()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_SYNCHRONIZATIONPOINT_EXIT(nmNetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_TXTIMEOUTEXCEPTION_ENTRY
/** \brief Entry point of function Nm_TxTimeoutException()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_TXTIMEOUTEXCEPTION_ENTRY(nmNetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_TXTIMEOUTEXCEPTION_EXIT
/** \brief Exit point of function Nm_TxTimeoutException()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_TXTIMEOUTEXCEPTION_EXIT(nmNetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_COORDREADYTOSLEEPINDICATION_ENTRY
/** \brief Entry point of function Nm_CoordReadyToSleepIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_COORDREADYTOSLEEPINDICATION_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_COORDREADYTOSLEEPINDICATION_EXIT
/** \brief Exit point of function Nm_CoordReadyToSleepIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_COORDREADYTOSLEEPINDICATION_EXIT(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_GETVERSIONINFO_ENTRY
/** \brief Entry point of function Nm_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_GETVERSIONINFO_ENTRY(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_GETVERSIONINFO_EXIT
/** \brief Exit point of function Nm_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_GETVERSIONINFO_EXIT(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_PDURXINDICATION_ENTRY
/** \brief Entry point of function Nm_PduRxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_PDURXINDICATION_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_PDURXINDICATION_EXIT
/** \brief Exit point of function Nm_PduRxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_PDURXINDICATION_EXIT(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_REPEATMESSAGEINDICATION_ENTRY
/** \brief Entry point of function Nm_RepeatMessageIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_REPEATMESSAGEINDICATION_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_REPEATMESSAGEINDICATION_EXIT
/** \brief Exit point of function Nm_RepeatMessageIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_REPEATMESSAGEINDICATION_EXIT(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_CARWAKEUPINDICATION_ENTRY
/** \brief Entry point of function Nm_CarWakeUpIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_CARWAKEUPINDICATION_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_NM_CARWAKEUPINDICATION_EXIT
/** \brief Exit point of function Nm_CarWakeUpIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_NM_CARWAKEUPINDICATION_EXIT(nmChannelHandle)
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

#endif /* ifndef NM_TRACE_H */
/*==================[end of file]===========================================*/
