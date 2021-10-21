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
#ifndef COM_CORE_TXSIGNAL_H_
#define COM_CORE_TXSIGNAL_H_
/*==================[inclusions]============================================*/


#include <TSAutosar.h>          /* EB specific standard types */
#include <Com_Api.h>                /* get module interface */

#if (COM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                /* get development error tracer interface */
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */

/*==================[version check]=========================================*/

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#define COM_START_SEC_CODE
#include <Com_MemMap.h>

#if (COM_SENDUPONAPICALL == STD_ON)

TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_SendSignal_Transmit
(
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

#endif /* (COM_SENDUPONAPICALL == STD_ON) */

#if (COM_TX_DYN_LENGTH_IPDU_ENABLE == STD_ON)

/** \brief Com_EB_WriteDynSignal_ExitArea_0 - update the value and the
 *  length of a dynamic length ComIpdu and initiate a transmission.
 * Precondition:
 * - COM must be initialized
 * - Exclusive area 0 must be entered
 * \param[in] SignalDataPtr - reference to value of the source signal
 * \param[in] Com_EB_WritePropPtr - configuration of a signal
 * \param[in] IntDataPtr - pointer to internal data structures
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_WriteDynSignal_ExitArea_0
(
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SignalDataPtr,
   P2CONST(ComTxSignalType, AUTOMATIC, AUTOMATIC) Com_EB_WritePropPtr,
   P2VAR(Com_EB_WriteIntDataType, AUTOMATIC, AUTOMATIC) IntDataPtr
);

#endif /* (COM_TX_DYN_LENGTH_IPDU_ENABLE == STD_ON) */

TS_MOD_PRIV_DECL FUNC(uint8, COM_CODE) Com_GetSendSignalRetVal
(
   P2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr
);


#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>



#endif /* COM_CORE_TXSIGNAL_H_ */
/*==================[end of file]===========================================*/

