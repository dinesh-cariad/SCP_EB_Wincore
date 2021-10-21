/**
 * \file
 *
 * \brief AUTOSAR LinIf
 *
 * This file contains the implementation of the AUTOSAR
 * module LinIf.
 *
 * \version 5.8.27
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef LINIF_TRACE_H
#define LINIF_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <LinIf_Cfg.h>
#include <LinIf_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_GETVERSIONINFO_ENTRY
/** \brief Entry point of function LinIf_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_GETVERSIONINFO_ENTRY(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_GETVERSIONINFO_EXIT
/** \brief Exit point of function LinIf_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_GETVERSIONINFO_EXIT(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_ENABLEBUSMIRRORING_ENTRY
/** \brief Entry point of function LinIf_EnableBusMirroring()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_ENABLEBUSMIRRORING_ENTRY(Channel,MirroringActive)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_ENABLEBUSMIRRORING_EXIT
/** \brief Exit point of function LinIf_EnableBusMirroring()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_ENABLEBUSMIRRORING_EXIT(Retval,Channel,MirroringActive)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function LinIf_IsValidConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_ISVALIDCONFIG_ENTRY(voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_ISVALIDCONFIG_EXIT
/** \brief Exit point of function LinIf_IsValidConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINIF_ISVALIDCONFIG_EXIT(Retval,voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_INIT_ENTRY
/** \brief Entry point of function LinIf_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_INIT_ENTRY(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_INIT_EXIT
/** \brief Exit point of function LinIf_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_INIT_EXIT(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_TRANSMIT_ENTRY
/** \brief Entry point of function LinIf_Transmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_TRANSMIT_ENTRY(LinTxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_TRANSMIT_EXIT
/** \brief Exit point of function LinIf_Transmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINIF_TRANSMIT_EXIT(Retval,LinTxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_SCHEDULEREQUEST_ENTRY
/** \brief Entry point of function LinIf_ScheduleRequest()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_SCHEDULEREQUEST_ENTRY(Channel,ScheduleTable)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_SCHEDULEREQUEST_EXIT
/** \brief Exit point of function LinIf_ScheduleRequest()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINIF_SCHEDULEREQUEST_EXIT(Retval,Channel,ScheduleTable)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_GOTOSLEEP_ENTRY
/** \brief Entry point of function LinIf_GotoSleep()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_GOTOSLEEP_ENTRY(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_GOTOSLEEP_EXIT
/** \brief Exit point of function LinIf_GotoSleep()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINIF_GOTOSLEEP_EXIT(Retval,Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_WAKEUP_ENTRY
/** \brief Entry point of function LinIf_Wakeup()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_WAKEUP_ENTRY(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_WAKEUP_EXIT
/** \brief Exit point of function LinIf_Wakeup()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINIF_WAKEUP_EXIT(Retval,Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

#if (LINIF_TRCV_SUPPORTED == STD_ON)

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_SETTRCVMODE_ENTRY
/** \brief Entry point of function LinIf_SetTrcvMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_SETTRCVMODE_ENTRY(Channel,TransceiverMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_SETTRCVMODE_EXIT
/** \brief Exit point of function LinIf_SetTrcvMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINIF_SETTRCVMODE_EXIT(Retval,Channel,TransceiverMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_GETTRCVMODE_ENTRY
/** \brief Entry point of function LinIf_GetTrcvMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_GETTRCVMODE_ENTRY(Channel,TransceiverModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_GETTRCVMODE_EXIT
/** \brief Exit point of function LinIf_GetTrcvMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINIF_GETTRCVMODE_EXIT(Retval,Channel,TransceiverModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_GETTRCVWAKEUPREASON_ENTRY
/** \brief Entry point of function LinIf_GetTrcvWakeupReason()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_GETTRCVWAKEUPREASON_ENTRY(Channel,TrcvWuReasonPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_GETTRCVWAKEUPREASON_EXIT
/** \brief Exit point of function LinIf_GetTrcvWakeupReason()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINIF_GETTRCVWAKEUPREASON_EXIT(Retval,Channel,TrcvWuReasonPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_SETTRCVWAKEUPMODE_ENTRY
/** \brief Entry point of function LinIf_SetTrcvWakeupMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_SETTRCVWAKEUPMODE_ENTRY(Channel,LinTrcvWakeupMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_SETTRCVWAKEUPMODE_EXIT
/** \brief Exit point of function LinIf_SetTrcvWakeupMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINIF_SETTRCVWAKEUPMODE_EXIT(Retval,Channel,LinTrcvWakeupMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
#endif /* LINIF_TRCV_SUPPORTED == STD_ON */

#if (LINIF_CHECKWAKEUP_SUPPORTED == STD_ON)
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_WAKEUPCONFIRMATION_ENTRY
/** \brief Entry point of function LinIf_WakeupConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_WAKEUPCONFIRMATION_ENTRY(WakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_WAKEUPCONFIRMATION_EXIT
/** \brief Exit point of function LinIf_WakeupConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_WAKEUPCONFIRMATION_EXIT(WakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
#endif

#ifndef LINIF_USE_LINAPI_REV2
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_CHECKWAKEUP_ENTRY
/** \brief Entry point of function LinIf_CheckWakeup()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_CHECKWAKEUP_ENTRY(WakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_CHECKWAKEUP_EXIT
/** \brief Exit point of function LinIf_CheckWakeup()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINIF_CHECKWAKEUP_EXIT(Retval,WakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
#else
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_CHECKWAKEUP_ENTRY
/** \brief Entry point of function LinIf_CheckWakeup()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_CHECKWAKEUP_ENTRY(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_CHECKWAKEUP_EXIT
/** \brief Exit point of function LinIf_CheckWakeup()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_CHECKWAKEUP_EXIT(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
#endif

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_GETPIDTABLE_ENTRY
/** \brief Entry point of function LinIf_GetPIDTable()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_GETPIDTABLE_ENTRY(Channel,PidBufferPtr,PidBufferLengthPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_GETPIDTABLE_EXIT
/** \brief Exit point of function LinIf_GetPIDTable()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINIF_GETPIDTABLE_EXIT(Retval,Channel,PidBufferPtr,PidBufferLengthPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_SETPIDTABLE_ENTRY
/** \brief Entry point of function LinIf_SetPIDTable()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_SETPIDTABLE_ENTRY(Channel,PidBufferPtr,PidBufferLengthPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_SETPIDTABLE_EXIT
/** \brief Exit point of function LinIf_SetPIDTable()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINIF_SETPIDTABLE_EXIT(Retval,Channel,PidBufferPtr,PidBufferLengthPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_GETCONFIGUREDNAD_ENTRY
/** \brief Entry point of function LinIf_GetConfiguredNAD()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_GETCONFIGUREDNAD_ENTRY(Channel,NadPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_GETCONFIGUREDNAD_EXIT
/** \brief Exit point of function LinIf_GetConfiguredNAD()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINIF_GETCONFIGUREDNAD_EXIT(Retval,Channel,NadPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_SETCONFIGUREDNAD_ENTRY
/** \brief Entry point of function LinIf_SetConfiguredNAD()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_SETCONFIGUREDNAD_ENTRY(Channel,Nad)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_SETCONFIGUREDNAD_EXIT
/** \brief Exit point of function LinIf_SetConfiguredNAD()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINIF_SETCONFIGUREDNAD_EXIT(Retval,Channel,Nad)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_HEADERINDICATION_ENTRY
/** \brief Entry point of function LinIf_HeaderIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_HEADERINDICATION_ENTRY(Channel,PduPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_HEADERINDICATION_EXIT
/** \brief Exit point of function LinIf_HeaderIndication()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINIF_HEADERINDICATION_EXIT(Retval,Channel,PduPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_RXINDICATION_ENTRY
/** \brief Entry point of function LinIf_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_RXINDICATION_ENTRY(Channel,Lin_SduPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_RXINDICATION_EXIT
/** \brief Exit point of function LinIf_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_RXINDICATION_EXIT(Channel,Lin_SduPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_TXCONFIRMATION_ENTRY
/** \brief Entry point of function LinIf_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_TXCONFIRMATION_ENTRY(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_TXCONFIRMATION_EXIT
/** \brief Exit point of function LinIf_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_TXCONFIRMATION_EXIT(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_LINERRORINDICATION_ENTRY
/** \brief Entry point of function LinIf_LinErrorIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_LINERRORINDICATION_ENTRY(Channel,ErrorStatus)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_LINERRORINDICATION_EXIT
/** \brief Exit point of function LinIf_LinErrorIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_LINERRORINDICATION_EXIT(Channel,ErrorStatus)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_MAINFUNCTION_ENTRY
/** \brief Entry point of function LinIf_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_MAINFUNCTION_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINIF_MAINFUNCTION_EXIT
/** \brief Exit point of function LinIf_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINIF_MAINFUNCTION_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINTP_GETVERSIONINFO_ENTRY
/** \brief Entry point of function LinTp_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINTP_GETVERSIONINFO_ENTRY(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINTP_GETVERSIONINFO_EXIT
/** \brief Exit point of function LinTp_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINTP_GETVERSIONINFO_EXIT(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINTP_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function LinTp_IsValidConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINTP_ISVALIDCONFIG_ENTRY(voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINTP_ISVALIDCONFIG_EXIT
/** \brief Exit point of function LinTp_IsValidConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINTP_ISVALIDCONFIG_EXIT(Retval,voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINTP_INIT_ENTRY
/** \brief Entry point of function LinTp_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINTP_INIT_ENTRY(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINTP_INIT_EXIT
/** \brief Exit point of function LinTp_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINTP_INIT_EXIT(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINTP_TRANSMIT_ENTRY
/** \brief Entry point of function LinTp_Transmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINTP_TRANSMIT_ENTRY(LinTpTxSduId,LinTpTxInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINTP_TRANSMIT_EXIT
/** \brief Exit point of function LinTp_Transmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINTP_TRANSMIT_EXIT(Retval,LinTpTxSduId,LinTpTxInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINTP_CANCELTRANSMIT_ENTRY
/** \brief Entry point of function LinTp_CancelTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINTP_CANCELTRANSMIT_ENTRY(LinTpTxSduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINTP_CANCELTRANSMIT_EXIT
/** \brief Exit point of function LinTp_CancelTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINTP_CANCELTRANSMIT_EXIT(Retval,LinTpTxSduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINTP_CANCELRECEIVE_ENTRY
/** \brief Entry point of function LinTp_CancelReceive()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINTP_CANCELRECEIVE_ENTRY(LinTpRxSduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINTP_CANCELRECEIVE_EXIT
/** \brief Exit point of function LinTp_CancelReceive()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINTP_CANCELRECEIVE_EXIT(Retval,LinTpRxSduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINTP_CHANGEPARAMETER_ENTRY
/** \brief Entry point of function LinTp_ChangeParameter()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LINTP_CHANGEPARAMETER_ENTRY(id,parameter,value)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LINTP_CHANGEPARAMETER_EXIT
/** \brief Exit point of function LinTp_ChangeParameter()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LINTP_CHANGEPARAMETER_EXIT(Retval,id,parameter,value)
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

#endif /* ifndef LINIF_TRACE_H */
/*==================[end of file]===========================================*/
