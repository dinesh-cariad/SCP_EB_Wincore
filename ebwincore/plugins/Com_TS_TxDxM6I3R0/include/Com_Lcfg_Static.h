/**
 * \file
 *
 * \brief AUTOSAR Com
 *
 * This file contains the implementation of the AUTOSAR
 * module Com.
 *
 * \version 6.3.50
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef COM_LCFG_STATIC_H_
#define COM_LCFG_STATIC_H_

/*==================[inclusions]============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <Com_Cfg.h>            /* get module interface */
#include <Com_Cbk.h>             /* callback types */
#include <Com_Callout.h>             /* callback types */

/*==================[version check]=========================================*/

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define COM_START_SEC_APPL_CODE
#include <Com_MemMap.h>

extern FUNC(void, COM_APPL_CODE) Com_EB_TxDMDummyCbk(void);

#define COM_STOP_SEC_APPL_CODE
#include <Com_MemMap.h>

#define COM_START_SEC_CODE
#include <Com_MemMap.h>

#if (COM_TIMEBASE == STD_OFF)
/**
 * \brief Com_MainFunctionTx_Generic - Com_MainFunctionTx for I-Pdus which are assigned to
 * Tx main function with the Id ComMainFunctionsTx_Idx
 *
 * This function handles cyclic sending-related tasks such as minimum delay
 * time and cyclic sending for I-Pdus which are assigned to Tx main function with the Id
 * ComMainFunctionsTx_Idx.
 * Preconditions:
 * - COM must be initialized
 *
 * \param[in] ComMainFunctionsTx_Idx - Id of the main function
 *
 * \ServiceID{0x19}
 * \Reentrancy{Non re-entrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, COM_CODE) Com_MainFunctionTx_Generic
(
    Com_MainFunctionTx_IdxType ComMainFunctionsTx_Idx
);

/**
 * \brief Com_MainFunctionRx_Generic -  Com_MainFunctionRx for I-Pdus which are assigned to
 * Rx main function with the Id ComMainFunctionsRx_Idx
 *
 * This function handles cyclic receiving-related tasks like reception deadline
 * monitoringfor I-Pdus which are assigned to Rx main function with the Id
 * ComMainFunctionsRx_Idx.
 * Preconditions:
 * - COM must be initialized
 *
 * \param[in] ComMainFunctionsRx_Idx - Id of the main function
 *
 * \ServiceID{0x18}
 * \Reentrancy{Non re-entrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, COM_CODE) Com_MainFunctionRx_Generic
(
    Com_MainFunctionRx_IdxType ComMainFunctionsRx_Idx
);

#if (COM_SIGNAL_GW_ENABLE == STD_ON)
/**
 * \brief Com_MainFunctionRouteSignals_Src_Generic - handles source related part for signal gateway
 * for source I-Pdus which are assigned to Rx main function with the Id ComMainFunctionsRx_Idx
 *
 * This function copies values of the gated source I-Pdus to a signal gateway shadow buffer
 * Preconditions:
 * - COM must be initialized
 * - must not be interrupted by or interrupt Com_MainFunctionRouteSignals_Src_Dest_Generic
 *   with the same Rx main function Id ComMainFunctionsRx_Idx
 *
 * \param[in] ComMainFunctionsRx_Idx - Id of the main function
 *
 *
 * \ServiceID{0x18}
 * \Reentrancy{Non re-entrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, COM_CODE) Com_MainFunctionRouteSignals_Src_Generic
(

   Com_MainFunctionRx_IdxType ComMainFunctionsRx_Idx

);

/**
 * \brief Com_MainFunctionRouteSignals_Src_Dest_Generic - handles destination related part for
 * signal gateway for source I-Pdus which are assigned to Rx main function with the Id
 * ComMainFunctionsRx_Idx and destination I-Pdus which are assigned to Tx main function with the Id
 * ComMainFunctionsTx_Idx
 *
 * This function copies values from the source signal gateway shadow buffer to the destination
 * I-Pdus which are assigned to Tx main function with the Id ComMainFunctionsTx_Idx
 * Preconditions:
 * - COM must be initialized
 * - must not be interrupted by or interrupt Com_MainFunctionRouteSignals_Src_Generic
 *   with the same Rx main function Id ComMainFunctionsRx_Idx
 *
 * \param[in] ComMainFunctionsRx_Idx - Id of the main function
 * \param[in] ComMainFunctionsTx_Idx - Id of the main function
 *
 *
 * \ServiceID{0x18}
 * \Reentrancy{Non re-entrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, COM_CODE) Com_MainFunctionRouteSignals_Src_Dest_Generic
(

   Com_MainFunctionRx_IdxType ComMainFunctionsRx_Idx,
   Com_MainFunctionTx_IdxType ComMainFunctionsTx_Idx

);
#endif /* (COM_SIGNAL_GW_ENABLE == STD_ON) */


#endif /* (COM_TIMEBASE == STD_OFF) */

#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#define COM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <Com_MemMap.h>


#if (COM_CALL_OUT_FUNC_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
extern CONSTP2VAR(Com_TxCalloutType, AUTOMATIC, COM_APPL_CODE) Com_TxCallouts[];
extern CONSTP2VAR(Com_RxCalloutType, AUTOMATIC, COM_APPL_CODE) Com_RxCallouts[];
#endif /* (COM_CALL_OUT_FUNC_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */

#if (COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
extern CONSTP2VAR(Com_CbkTxAck_Type, AUTOMATIC, COM_RTE_CODE) Com_CbkTxAck_Array[];
#endif /* (COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */

#if (COM_CBK_TX_ERR_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
extern CONSTP2VAR(Com_CbkTxErr_Type, AUTOMATIC, COM_RTE_CODE) Com_CbkTxErr_Array[];
#endif /* (COM_CBK_TX_ERR_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */

#if (COM_CBK_TX_T_OUT_ARRAY_SIZE_MAX != COM_INDEX_NONE)
extern CONSTP2VAR(Com_CbkTxTOut_Type, AUTOMATIC, COM_RTE_CODE) Com_CbkTxTOut_Array[];
#endif /* (COM_CBK_TX_T_OUT_ARRAY_SIZE_MAX != COM_INDEX_NONE) */


#if (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
extern CONSTP2VAR(Com_CbkRxAck_Type, AUTOMATIC, COM_RTE_CODE) Com_CbkRxAck_Array[];
#endif /* (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */


#if (COM_CBK_RX_T_OUT_ARRAY_SIZE_MAX != COM_INDEX_NONE)
extern CONSTP2VAR(Com_CbkRxTOut_Type, AUTOMATIC, COM_RTE_CODE) Com_CbkRxTOut_Array[];
#endif /* (COM_CBK_RX_T_OUT_ARRAY_SIZE_MAX != COM_INDEX_NONE) */


#define COM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <Com_MemMap.h>


/* start data section declaration */
#define COM_START_SEC_CONST_UNSPECIFIED
#include <Com_MemMap.h>

/* CHECK: RULE 506 OFF */
/* Check_C disabled. Com_gDataMemPtr is not renamed because of readability and severity of impact to benefit ratio. */
/** \brief
 * Pointer to statically allocated RAM.
 */
extern CONSTP2VAR(uint8, COM_CONST, COM_VAR_CLEARED) Com_gDataMemPtr;
/* CHECK: RULE 506 ON */

/* stop data section declaration */
#define COM_STOP_SEC_CONST_UNSPECIFIED
#include <Com_MemMap.h>



/* start data section declaration */
#define COM_START_SEC_CONST_32
#include <Com_MemMap.h>

/** \brief
 * Variable holding link-time configuration
 */
extern CONST(uint32, COM_CONST) Com_LcfgSignature;

/* stop data section declaration */
#define COM_STOP_SEC_CONST_32
#include <Com_MemMap.h>



/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /*COM_LCFG_STATIC_H_ */
