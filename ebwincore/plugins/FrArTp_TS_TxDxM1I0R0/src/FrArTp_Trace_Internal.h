/**
 * \file
 *
 * \brief AUTOSAR FrArTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrArTp.
 *
 * \version 1.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef FRARTP_TRACE_INTERNAL_H
#define FRARTP_TRACE_INTERNAL_H
/*==================[inclusions]============================================*/

/*==================[macros]================================================*/
/* Place internal debug instrumentation macros in this file, i.e. all
   macros that are not generated for the public api instrumentation.
*/

#ifndef DBG_FRARTP_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function FrArTp_IsValidConfig() */
#define DBG_FRARTP_ISVALIDCONFIG_ENTRY(a)
#endif

#ifndef DBG_FRARTP_ISVALIDCONFIG_EXIT
/** \brief Exit point of function FrArTp_IsValidConfig() */
#define DBG_FRARTP_ISVALIDCONFIG_EXIT(a,b)
#endif

#ifndef DBG_FRARTP_INIT_ENTRY
/** \brief Entry point of function FrArTp_Init() */
#define DBG_FRARTP_INIT_ENTRY(a)
#endif

#ifndef DBG_FRARTP_INIT_EXIT
/** \brief Exit point of function FrArTp_Init() */
#define DBG_FRARTP_INIT_EXIT(a)
#endif

#ifndef DBG_FRARTP_GETVERSIONINFO_ENTRY
/** \brief Entry point of function FrArTp_GetVersionInfo() */
#define DBG_FRARTP_GETVERSIONINFO_ENTRY(a)
#endif

#ifndef DBG_FRARTP_GETVERSIONINFO_EXIT
/** \brief Exit point of function FrArTp_GetVersionInfo() */
#define DBG_FRARTP_GETVERSIONINFO_EXIT(a)
#endif

#ifndef DBG_FRARTP_TRANSMIT_ENTRY
/** \brief Exit point of function FrArTp_Transmit() */
#define DBG_FRARTP_TRANSMIT_ENTRY(a,b)
#endif

#ifndef DBG_FRARTP_TRANSMIT_EXIT
/** \brief Exit point of function FrArTp_Transmit() */
#define DBG_FRARTP_TRANSMIT_EXIT(a,b,c)
#endif

#ifndef DBG_FRARTP_CANCELTRANSMIT_ENTRY
/** \brief Exit point of function FrArTp_CancelTransmit() */
#define DBG_FRARTP_CANCELTRANSMIT_ENTRY(a)
#endif

#ifndef DBG_FRARTP_CANCELTRANSMIT_EXIT
/** \brief Exit point of function FrArTp_CancelTransmit() */
#define DBG_FRARTP_CANCELTRANSMIT_EXIT(a,b)
#endif

#ifndef DBG_FRARTP_CANCELRECEIVE_ENTRY
/** \brief Exit point of function FrArTp_CancelReceive() */
#define DBG_FRARTP_CANCELRECEIVE_ENTRY(a)
#endif

#ifndef DBG_FRARTP_CANCELRECEIVE_EXIT
/** \brief Exit point of function FrArTp_CancelReceive() */
#define DBG_FRARTP_CANCELRECEIVE_EXIT(a,b)
#endif

#ifndef DBG_FRARTP_CHANGEPARAMETER_ENTRY
/** \brief Entry point of function FrArTp_ChangeParameter() */
#define DBG_FRARTP_CHANGEPARAMETER_ENTRY(a,b,c)
#endif

#ifndef DBG_FRARTP_CHANGEPARAMETER_EXIT
/** \brief Exit point of function FrArTp_ChangeParameter() */
#define DBG_FRARTP_CHANGEPARAMETER_EXIT(a,b,c,d)
#endif

#ifndef DBG_FRARTP_TRIGGERTRANSMIT_ENTRY
/** \brief Entry point of function FrArTp_TriggerTransmit() */
#define DBG_FRARTP_TRIGGERTRANSMIT_ENTRY(a,b)
#endif

#ifndef DBG_FRARTP_TRIGGERTRANSMIT_EXIT
/** \brief Exit point of function FrArTp_TriggerTransmit() */
#define DBG_FRARTP_TRIGGERTRANSMIT_EXIT(a,b,c)
#endif

#ifndef DBG_FRARTP_RXINDICATION_ENTRY
/** \brief Entry point of function FrArTp_RxIndication() */
#define DBG_FRARTP_RXINDICATION_ENTRY(a,b)
#endif

#ifndef DBG_FRARTP_RXINDICATION_EXIT
/** \brief Exit point of function FrArTp_RxIndication() */
#define DBG_FRARTP_RXINDICATION_EXIT(a,b)
#endif

#ifndef DBG_FRARTP_TXCONFIRMATION_ENTRY
/** \brief Entry point of function FrArTp_TxConfirmation() */
#define DBG_FRARTP_TXCONFIRMATION_ENTRY(a)
#endif

#ifndef DBG_FRARTP_TXCONFIRMATION_EXIT
/** \brief Exit point of function FrArTp_TxConfirmation() */
#define DBG_FRARTP_TXCONFIRMATION_EXIT(a)
#endif

#ifndef DBG_FRARTP_MAINFUNCTION_ENTRY
/** \brief Entry point of function FrArTp_MainFunction() */
#define DBG_FRARTP_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_FRARTP_MAINFUNCTION_EXIT
/** \brief Exit point of function FrArTp_MainFunction() */
#define DBG_FRARTP_MAINFUNCTION_EXIT()
#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef FRARTP_TRACE_INTERNAL_H */
/*==================[end of file]===========================================*/
