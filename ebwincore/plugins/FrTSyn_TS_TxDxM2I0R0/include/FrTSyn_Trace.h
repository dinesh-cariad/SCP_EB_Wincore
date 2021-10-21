/**
 * \file
 *
 * \brief AUTOSAR FrTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module FrTSyn.
 *
 * \version 2.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef FRTSYN_TRACE_H
#define FRTSYN_TRACE_H


/*==================[macros]================================================*/
/* ====== FRTSYN public APIs ====== */

#ifndef DBG_FRTSYN_GETVERSIONINFO_ENTRY
/** \brief Entry point of function FrTSyn_GetVersionInfo() */
#define DBG_FRTSYN_GETVERSIONINFO_ENTRY(a)
#endif

#ifndef DBG_FRTSYN_GETVERSIONINFO_EXIT
/** \brief Exit point of function FrTSyn_GetVersionInfo() */
#define DBG_FRTSYN_GETVERSIONINFO_EXIT(a)
#endif

#ifndef DBG_FRTSYN_INIT_ENTRY
/** \brief Entry point of function FrTSyn_Init() */
#define DBG_FRTSYN_INIT_ENTRY(a)
#endif

#ifndef DBG_FRTSYN_INIT_EXIT
/** \brief Exit point of function FrTSyn_Init() */
#define DBG_FRTSYN_INIT_EXIT(a)
#endif

#ifndef DBG_FRTSYN_MAINFUNCTION_ENTRY
/** \brief Entry point of function FrTSyn_MainFunction() */
#define DBG_FRTSYN_MAINFUNCTION_ENTRY()
#endif

#ifndef DBG_FRTSYN_MAINFUNCTION_EXIT
/** \brief Exit point of function FrTSyn_MainFunction() */
#define DBG_FRTSYN_MAINFUNCTION_EXIT()
#endif

#ifndef DBG_FRTSYN_RXINDICATION_ENTRY
/** \brief Entry point of function FrTSyn_RxIndication() */
#define DBG_FRTSYN_RXINDICATION_ENTRY(a,b)
#endif

#ifndef DBG_FRTSYN_RXINDICATION_EXIT
/** \brief Exit point of function FrTSyn_RxIndication() */
#define DBG_FRTSYN_RXINDICATION_EXIT(a,b)
#endif

#ifndef DBG_FRTSYN_CHECKPDUIDVALIDITY_ENTRY
/** \brief Entry point of function FrTSyn_CheckPduIdValidity() */
#define DBG_FRTSYN_CHECKPDUIDVALIDITY_ENTRY(a, b)
#endif

#ifndef DBG_FRTSYN_CHECKPDUIDVALIDITY_EXIT
/** \brief Exit point of function FrTSyn_CheckPduIdValidity() */
#define DBG_FRTSYN_CHECKPDUIDVALIDITY_EXIT(a, b)
#endif

#ifndef DBG_FRTSYN_SUBSTRACTFROMTIMESTAMP_ENTRY
/** \brief Entry point of function FrTSyn_SubtractFromTimeStamp() */
#define DBG_FRTSYN_SUBSTRACTFROMTIMESTAMP_ENTRY(a,b)
#endif

#ifndef DBG_FRTSYN_SUBSTRACTFROMTIMESTAMP_EXIT
/** \brief Exit point of function FrTSyn_SubtractFromTimeStamp() */
#define DBG_FRTSYN_SUBSTRACTFROMTIMESTAMP_EXIT(a,b)
#endif

#ifndef DBG_FRTSYN_SENDTOSTBM_ENTRY
/** \brief Entry point of function FrTSyn_SendToStbM() */
#define DBG_FRTSYN_SENDTOSTBM_ENTRY(a,b,d,e,f)
#endif

#ifndef DBG_FRTSYN_SENDTOSTBM_EXIT
/** \brief Exit point of function FrTSyn_SendToStbM() */
#define DBG_FRTSYN_SENDTOSTBM_EXIT(a,b,d,e,f)
#endif

#ifndef DBG_FRTSYN_ADDTOTIMESTAMP_ENTRY
/** \brief Entry point of function FrTSyn_AddToTimeStamp() */
#define DBG_FRTSYN_ADDTOTIMESTAMP_ENTRY(a,b)
#endif

#ifndef DBG_FRTSYN_ADDTOTIMESTAMP_EXIT
/** \brief Exit point of function FrTSyn_AddToTimeStamp() */
#define DBG_FRTSYN_ADDTOTIMESTAMP_EXIT(a,b)
#endif

#ifndef DBG_FRTSYN_SETTRANSMISSIONMODE_ENTRY
/** \brief Entry point of function FrTSyn_SetTransmissionMode() */
#define DBG_FRTSYN_SETTRANSMISSIONMODE_ENTRY(a,b)
#endif

#ifndef DBG_FRTSYN_SETTRANSMISSIONMODE_EXIT
/** \brief Exit point of function FrTSyn_SetTransmissionMode() */
#define DBG_FRTSYN_SETTRANSMISSIONMODE_EXIT(a,b)
#endif

#ifndef DBG_FRTSYN_ADDDATATOARRAY_ENTRY
/** \brief Entry point of function FrTSyn_AddDataToArray() */
#define DBG_FRTSYN_ADDDATATOARRAY_ENTRY(a,b,c)
#endif

#ifndef DBG_FRTSYN_ADDDATATOARRAY_EXIT
/** \brief Exit point of function FrTSyn_AddDataToArray() */
#define DBG_FRTSYN_ADDDATATOARRAY_EXIT(a,b,c)
#endif

#ifndef DBG_FRTSYN_TRIGGERTRANSMIT_ENTRY
/** \brief Entry point of function FrTSyn_TriggerTransmit() */
#define DBG_FRTSYN_TRIGGERTRANSMIT_ENTRY(a,b)
#endif

#ifndef DBG_FRTSYN_TRIGGERTRANSMIT_EXIT
/** \brief Exit point of function FrTSyn_TriggerTransmit() */
#define DBG_FRTSYN_TRIGGERTRANSMIT_EXIT(a,b)
#endif

#ifndef DBG_FRTSYN_CHECKTXPDUIDVALIDITY_ENTRY
/** \brief Entry point of function FrTSyn_CheckTxPduIdValidity() */
#define DBG_FRTSYN_CHECKTXPDUIDVALIDITYT_ENTRY(a,b)
#endif

#ifndef DBG_FRTSYN_CHECKTXPDUIDVALIDITY_EXIT
/** \brief Exit point of function FrTSyn_CheckTxPduIdValidity() */
#define DBG_FRTSYN_CHECKTXPDUIDVALIDITY_EXIT(a,b)
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


#endif /* ifndef FRTSYN_TRACE_H */
