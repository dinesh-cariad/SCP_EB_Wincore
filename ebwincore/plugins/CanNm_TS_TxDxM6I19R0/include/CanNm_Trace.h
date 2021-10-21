/**
 * \file
 *
 * \brief AUTOSAR CanNm
 *
 * This file contains the implementation of the AUTOSAR
 * module CanNm.
 *
 * \version 6.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef CANNM_TRACE_H
#define CANNM_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <CanNm_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function CanNm_IsValidConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_ISVALIDCONFIG_ENTRY(voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_ISVALIDCONFIG_EXIT
/** \brief Exit point of function CanNm_IsValidConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANNM_ISVALIDCONFIG_EXIT(Retval,voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_PASSIVESTARTUP_ENTRY
/** \brief Entry point of function CanNm_PassiveStartUp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_PASSIVESTARTUP_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_PASSIVESTARTUP_EXIT
/** \brief Exit point of function CanNm_PassiveStartUp()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANNM_PASSIVESTARTUP_EXIT(Retval,nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_NETWORKREQUEST_ENTRY
/** \brief Entry point of function CanNm_NetworkRequest()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_NETWORKREQUEST_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_NETWORKREQUEST_EXIT
/** \brief Exit point of function CanNm_NetworkRequest()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANNM_NETWORKREQUEST_EXIT(Retval,nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_NETWORKGWERAREQUEST_ENTRY
/** \brief Entry point of function CanNm_NetworkGwEraRequest()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_NETWORKGWERAREQUEST_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_NETWORKGWERAREQUEST_EXIT
/** \brief Exit point of function CanNm_NetworkGwEraRequest()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANNM_NETWORKGWERAREQUEST_EXIT(Retval,nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_NETWORKRELEASE_ENTRY
/** \brief Entry point of function CanNm_NetworkRelease()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_NETWORKRELEASE_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_NETWORKRELEASE_EXIT
/** \brief Exit point of function CanNm_NetworkRelease()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANNM_NETWORKRELEASE_EXIT(Retval,nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_DISABLECOMMUNICATION_ENTRY
/** \brief Entry point of function CanNm_DisableCommunication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_DISABLECOMMUNICATION_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_DISABLECOMMUNICATION_EXIT
/** \brief Exit point of function CanNm_DisableCommunication()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANNM_DISABLECOMMUNICATION_EXIT(Retval,nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_ENABLECOMMUNICATION_ENTRY
/** \brief Entry point of function CanNm_EnableCommunication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_ENABLECOMMUNICATION_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_ENABLECOMMUNICATION_EXIT
/** \brief Exit point of function CanNm_EnableCommunication()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANNM_ENABLECOMMUNICATION_EXIT(Retval,nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_REPEATMESSAGEREQUEST_ENTRY
/** \brief Entry point of function CanNm_RepeatMessageRequest()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_REPEATMESSAGEREQUEST_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_REPEATMESSAGEREQUEST_EXIT
/** \brief Exit point of function CanNm_RepeatMessageRequest()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANNM_REPEATMESSAGEREQUEST_EXIT(Retval,nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_GETSTATE_ENTRY
/** \brief Entry point of function CanNm_GetState()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_GETSTATE_ENTRY(nmChannelHandle,nmStatePtr,nmModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_GETSTATE_EXIT
/** \brief Exit point of function CanNm_GetState()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANNM_GETSTATE_EXIT(Retval,nmChannelHandle,nmStatePtr,nmModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_REQUESTBUSSYNCHRONIZATION_ENTRY
/** \brief Entry point of function CanNm_RequestBusSynchronization()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_REQUESTBUSSYNCHRONIZATION_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_REQUESTBUSSYNCHRONIZATION_EXIT
/** \brief Exit point of function CanNm_RequestBusSynchronization()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANNM_REQUESTBUSSYNCHRONIZATION_EXIT(Retval,nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_CHECKREMOTESLEEPINDICATION_ENTRY
/** \brief Entry point of function CanNm_CheckRemoteSleepIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_CHECKREMOTESLEEPINDICATION_ENTRY(nmChannelHandle,nmRemoteSleepIndPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_CHECKREMOTESLEEPINDICATION_EXIT
/** \brief Exit point of function CanNm_CheckRemoteSleepIndication()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANNM_CHECKREMOTESLEEPINDICATION_EXIT(Retval,nmChannelHandle,nmRemoteSleepIndPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_INIT_ENTRY
/** \brief Entry point of function CanNm_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_INIT_ENTRY(cannmConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_INIT_EXIT
/** \brief Exit point of function CanNm_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_INIT_EXIT(cannmConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_MAINFUNCTION_ENTRY
/** \brief Entry point of function CanNm_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_MAINFUNCTION_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_MAINFUNCTION_EXIT
/** \brief Exit point of function CanNm_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_MAINFUNCTION_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_GETNODEIDENTIFIER_ENTRY
/** \brief Entry point of function CanNm_GetNodeIdentifier()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_GETNODEIDENTIFIER_ENTRY(nmChannelHandle,nmNodeIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_GETNODEIDENTIFIER_EXIT
/** \brief Exit point of function CanNm_GetNodeIdentifier()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANNM_GETNODEIDENTIFIER_EXIT(Retval,nmChannelHandle,nmNodeIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_GETLOCALNODEIDENTIFIER_ENTRY
/** \brief Entry point of function CanNm_GetLocalNodeIdentifier()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_GETLOCALNODEIDENTIFIER_ENTRY(nmChannelHandle,nmNodeIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_GETLOCALNODEIDENTIFIER_EXIT
/** \brief Exit point of function CanNm_GetLocalNodeIdentifier()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANNM_GETLOCALNODEIDENTIFIER_EXIT(Retval,nmChannelHandle,nmNodeIdPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_GETUSERDATA_ENTRY
/** \brief Entry point of function CanNm_GetUserData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_GETUSERDATA_ENTRY(nmChannelHandle,nmUserDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_GETUSERDATA_EXIT
/** \brief Exit point of function CanNm_GetUserData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANNM_GETUSERDATA_EXIT(Retval,nmChannelHandle,nmUserDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_SETUSERDATA_ENTRY
/** \brief Entry point of function CanNm_SetUserData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_SETUSERDATA_ENTRY(nmChannelHandle,nmUserDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_SETUSERDATA_EXIT
/** \brief Exit point of function CanNm_SetUserData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANNM_SETUSERDATA_EXIT(Retval,nmChannelHandle,nmUserDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_TRANSMIT_ENTRY
/** \brief Entry point of function CanNm_Transmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_TRANSMIT_ENTRY(CanTxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_TRANSMIT_EXIT
/** \brief Exit point of function CanNm_Transmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANNM_TRANSMIT_EXIT(Retval,CanTxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_GETPDUDATA_ENTRY
/** \brief Entry point of function CanNm_GetPduData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_GETPDUDATA_ENTRY(nmChannelHandle,nmPduDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_GETPDUDATA_EXIT
/** \brief Exit point of function CanNm_GetPduData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANNM_GETPDUDATA_EXIT(Retval,nmChannelHandle,nmPduDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_CONFIRMPNAVAILABILITY_ENTRY
/** \brief Entry point of function CanNm_ConfirmPnAvailability()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_CONFIRMPNAVAILABILITY_ENTRY(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_CONFIRMPNAVAILABILITY_EXIT
/** \brief Exit point of function CanNm_ConfirmPnAvailability()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_CONFIRMPNAVAILABILITY_EXIT(nmChannelHandle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_GETVERSIONINFO_ENTRY
/** \brief Entry point of function CanNm_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_GETVERSIONINFO_ENTRY(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_GETVERSIONINFO_EXIT
/** \brief Exit point of function CanNm_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_GETVERSIONINFO_EXIT(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_TXCONFIRMATION_ENTRY
/** \brief Entry point of function CanNm_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_TXCONFIRMATION_ENTRY(TxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_TXCONFIRMATION_EXIT
/** \brief Exit point of function CanNm_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_TXCONFIRMATION_EXIT(TxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_RXINDICATION_ENTRY
/** \brief Entry point of function CanNm_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_RXINDICATION_ENTRY(RxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANNM_RXINDICATION_EXIT
/** \brief Exit point of function CanNm_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANNM_RXINDICATION_EXIT(RxPduId,PduInfoPtr)
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

#endif /* ifndef CANNM_TRACE_H */
/*==================[end of file]===========================================*/

