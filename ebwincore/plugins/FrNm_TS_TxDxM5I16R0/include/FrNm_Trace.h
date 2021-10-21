/**
 * \file
 *
 * \brief AUTOSAR FrNm
 *
 * This file contains the implementation of the AUTOSAR
 * module FrNm.
 *
 * \version 5.16.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef FRNM_TRACE_H
#define FRNM_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <FrNm_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_INIT_ENTRY
/** \brief Entry point of function FrNm_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_INIT_ENTRY(nmConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_INIT_EXIT
/** \brief Exit point of function FrNm_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_INIT_EXIT(nmConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_PASSIVESTARTUP_ENTRY
/** \brief Entry point of function FrNm_PassiveStartUp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_PASSIVESTARTUP_ENTRY(NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_PASSIVESTARTUP_EXIT
/** \brief Exit point of function FrNm_PassiveStartUp()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRNM_PASSIVESTARTUP_EXIT(Retval,NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_NETWORKREQUEST_ENTRY
/** \brief Entry point of function FrNm_NetworkRequest()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_NETWORKREQUEST_ENTRY(NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_NETWORKREQUEST_EXIT
/** \brief Exit point of function FrNm_NetworkRequest()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRNM_NETWORKREQUEST_EXIT(Retval,NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_NETWORKGWERAREQUEST_ENTRY
/** \brief Entry point of function FrNm_NetworkGwEraRequest()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_NETWORKGWERAREQUEST_ENTRY(NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_NETWORKGWERAREQUEST_EXIT
/** \brief Exit point of function FrNm_NetworkGwEraRequest()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRNM_NETWORKGWERAREQUEST_EXIT(Retval,NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_NETWORKRELEASE_ENTRY
/** \brief Entry point of function FrNm_NetworkRelease()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_NETWORKRELEASE_ENTRY(NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_NETWORKRELEASE_EXIT
/** \brief Exit point of function FrNm_NetworkRelease()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRNM_NETWORKRELEASE_EXIT(Retval,NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_SETUSERDATA_ENTRY
/** \brief Entry point of function FrNm_SetUserData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_SETUSERDATA_ENTRY(NetworkHandle,nmUserDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_SETUSERDATA_EXIT
/** \brief Exit point of function FrNm_SetUserData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRNM_SETUSERDATA_EXIT(Retval,NetworkHandle,nmUserDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_GETUSERDATA_ENTRY
/** \brief Entry point of function FrNm_GetUserData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_GETUSERDATA_ENTRY(NetworkHandle,nmUserDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_GETUSERDATA_EXIT
/** \brief Exit point of function FrNm_GetUserData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRNM_GETUSERDATA_EXIT(Retval,NetworkHandle,nmUserDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_GETPDUDATA_ENTRY
/** \brief Entry point of function FrNm_GetPduData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_GETPDUDATA_ENTRY(NetworkHandle,nmPduData)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_GETPDUDATA_EXIT
/** \brief Exit point of function FrNm_GetPduData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRNM_GETPDUDATA_EXIT(Retval,NetworkHandle,nmPduData)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_REPEATMESSAGEREQUEST_ENTRY
/** \brief Entry point of function FrNm_RepeatMessageRequest()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_REPEATMESSAGEREQUEST_ENTRY(NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_REPEATMESSAGEREQUEST_EXIT
/** \brief Exit point of function FrNm_RepeatMessageRequest()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRNM_REPEATMESSAGEREQUEST_EXIT(Retval,NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_GETNODEIDENTIFIER_ENTRY
/** \brief Entry point of function FrNm_GetNodeIdentifier()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_GETNODEIDENTIFIER_ENTRY(NetworkHandle,nmNodeIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_GETNODEIDENTIFIER_EXIT
/** \brief Exit point of function FrNm_GetNodeIdentifier()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRNM_GETNODEIDENTIFIER_EXIT(Retval,NetworkHandle,nmNodeIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_GETLOCALNODEIDENTIFIER_ENTRY
/** \brief Entry point of function FrNm_GetLocalNodeIdentifier()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_GETLOCALNODEIDENTIFIER_ENTRY(NetworkHandle,nmNodeIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_GETLOCALNODEIDENTIFIER_EXIT
/** \brief Exit point of function FrNm_GetLocalNodeIdentifier()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRNM_GETLOCALNODEIDENTIFIER_EXIT(Retval,NetworkHandle,nmNodeIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_REQUESTBUSSYNCHRONIZATION_ENTRY
/** \brief Entry point of function FrNm_RequestBusSynchronization()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_REQUESTBUSSYNCHRONIZATION_ENTRY(NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_REQUESTBUSSYNCHRONIZATION_EXIT
/** \brief Exit point of function FrNm_RequestBusSynchronization()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRNM_REQUESTBUSSYNCHRONIZATION_EXIT(Retval,NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_CHECKREMOTESLEEPINDICATION_ENTRY
/** \brief Entry point of function FrNm_CheckRemoteSleepIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_CHECKREMOTESLEEPINDICATION_ENTRY(NetworkHandle,nmRemoteSleepIndPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_CHECKREMOTESLEEPINDICATION_EXIT
/** \brief Exit point of function FrNm_CheckRemoteSleepIndication()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRNM_CHECKREMOTESLEEPINDICATION_EXIT(Retval,NetworkHandle,nmRemoteSleepIndPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_GETSTATE_ENTRY
/** \brief Entry point of function FrNm_GetState()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_GETSTATE_ENTRY(NetworkHandle,nmStatePtr,nmModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_GETSTATE_EXIT
/** \brief Exit point of function FrNm_GetState()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRNM_GETSTATE_EXIT(Retval,NetworkHandle,nmStatePtr,nmModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_GETVERSIONINFO_ENTRY
/** \brief Entry point of function FrNm_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_GETVERSIONINFO_ENTRY(NmVerInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_GETVERSIONINFO_EXIT
/** \brief Exit point of function FrNm_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_GETVERSIONINFO_EXIT(NmVerInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_STARTUPERROR_ENTRY
/** \brief Entry point of function FrNm_StartupError()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_STARTUPERROR_ENTRY(NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_STARTUPERROR_EXIT
/** \brief Exit point of function FrNm_StartupError()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_STARTUPERROR_EXIT(NetworkHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_TRANSMIT_ENTRY
/** \brief Entry point of function FrNm_Transmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_TRANSMIT_ENTRY(FrNmTxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_TRANSMIT_EXIT
/** \brief Exit point of function FrNm_Transmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRNM_TRANSMIT_EXIT(Retval,FrNmTxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_ENABLECOMMUNICATION_ENTRY
/** \brief Entry point of function FrNm_EnableCommunication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_ENABLECOMMUNICATION_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_ENABLECOMMUNICATION_EXIT
/** \brief Exit point of function FrNm_EnableCommunication()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRNM_ENABLECOMMUNICATION_EXIT(Retval,nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_DISABLECOMMUNICATION_ENTRY
/** \brief Entry point of function FrNm_DisableCommunication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_DISABLECOMMUNICATION_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_DISABLECOMMUNICATION_EXIT
/** \brief Exit point of function FrNm_DisableCommunication()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRNM_DISABLECOMMUNICATION_EXIT(Retval,nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_SETSLEEPREADYBIT_ENTRY
/** \brief Entry point of function FrNm_SetSleepReadyBit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_SETSLEEPREADYBIT_ENTRY(nmChannelHandle,nmSleepReadyBit)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_SETSLEEPREADYBIT_EXIT
/** \brief Exit point of function FrNm_SetSleepReadyBit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRNM_SETSLEEPREADYBIT_EXIT(Retval,nmChannelHandle,nmSleepReadyBit)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function FrNm_IsValidConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_ISVALIDCONFIG_ENTRY(voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_ISVALIDCONFIG_EXIT
/** \brief Exit point of function FrNm_IsValidConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRNM_ISVALIDCONFIG_EXIT(Retval,voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_RXINDICATION_ENTRY
/** \brief Entry point of function FrNm_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_RXINDICATION_ENTRY(RxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_RXINDICATION_EXIT
/** \brief Exit point of function FrNm_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_RXINDICATION_EXIT(RxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_TRIGGERTRANSMIT_ENTRY
/** \brief Entry point of function FrNm_TriggerTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_TRIGGERTRANSMIT_ENTRY(TxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_TRIGGERTRANSMIT_EXIT
/** \brief Exit point of function FrNm_TriggerTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRNM_TRIGGERTRANSMIT_EXIT(Retval,TxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_TXCONFIRMATION_ENTRY
/** \brief Entry point of function FrNm_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_TXCONFIRMATION_ENTRY(TxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRNM_TXCONFIRMATION_EXIT
/** \brief Exit point of function FrNm_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRNM_TXCONFIRMATION_EXIT(TxPduId)
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

#endif /* ifndef FRNM_TRACE_H */
/*==================[end of file]===========================================*/
