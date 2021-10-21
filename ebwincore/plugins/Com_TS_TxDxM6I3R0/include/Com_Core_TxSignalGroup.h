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
#ifndef COM_CORE_TXSIGNALGROUP_H_
#define COM_CORE_TXSIGNALGROUP_H_
/*==================[inclusions]============================================*/


#include <TSAutosar.h>          /* EB specific standard types */
#include <Com_Api.h>                /* get module interface */

#if (COM_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>                /* get development error tracer interface */
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */

/*==================[version check]=========================================*/

/*==================[macros]================================================*/

/*==================[type definitions]======================================*/

typedef FUNC(uint8, COM_CODE) (Com_TxSignalGroupFPtrType)
(
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayPtr,
   uint16 SGArrayLength,
   P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
#if ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
   ,boolean SendInMainFunctionTx
#endif /* ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
);


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



/** \brief Com_EB_SendSignalGroup - internal function for sending a signal group
 * The service Com_EB_SendSignalGroup sends a signal group. It evaluates the
 * TMS flags of the group signals and calculates the TMS for the signal group
 * Preconditions:
 * - COM must be initialized.
 * \param[in] SGArrayPtr - not used
 * \param[in] SGArrayPtrLength - not used
 * \param[in] SignalGPtr - reference to configuration of signal group
 * \param[in] PduId - id of I-Pdu to which the signal group belongs
 * \param[in] IPduPtr - reference to the configuration of I-Pdu
 * \param[in] SendInMainFunctionTx - if TRUE, the Pdu is sent not now but
 * during the next Com_MainfunctionTx(), otherwise immediately
 * \return Function execution success status
 * \retval E_OK if signal group could be copied to the I-Pdu
 * \retval COM_SERVICE_NOT_AVAILABLE if corresponding I-PDU group was stopped
 * \retval COM_BUSY - The receive Tp buffer is actually not available
 * (or service failed due to development error)
 */
TS_MOD_PRIV_DECL FUNC(uint8, COM_CODE) Com_EB_SendSignalGroup
(
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayPtr,
   uint16 SGArrayPtrLength,
   P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
#if ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
   ,boolean SendInMainFunctionTx
#endif /* ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
);


#if (COM_BASED_TRANSFORMER_SUPPORT_TX == STD_ON)

/** \brief Com_EB_SendSignalGroupArray_DETCheck - internal function for updating and
 * sending a signal group and DET checks for signal group array
 * The service Com_EB_SendSignalGroupArray_DETCheck updates and sends a signal group.
 * It evaluates the
 * TMS flags of the group signals and calculates the TMS for the signal group
 * Performs DET checks for signal group array parameter
 * Preconditions:
 * - COM must be initialized.
 * \param[in] SGArrayPtr - Reference to the signal group array to be transmitted
 * \param[in] SGArrayLength - Length of the signal group array
 * \param[in] SignalGPtr - reference to configuration of signal group
 * \param[in] PduId - id of I-Pdu to which the signal group belongs
 * \param[in] IPduPtr - reference to the configuration of I-Pdu
 * \param[in] SendInMainFunctionTx - if TRUE, the Pdu is sent not now but
 * during the next Com_MainfunctionTx(), otherwise immediately
 * \return Function execution success status
 * \retval E_OK if signal group could be copied to the I-Pdu
 * \retval COM_SERVICE_NOT_AVAILABLE if corresponding I-PDU group was stopped
 * (or service failed due to development error)
 */
TS_MOD_PRIV_DECL FUNC(uint8, COM_CODE) Com_EB_SendSignalGroupArray_DETCheck
(
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayPtr,
   uint16 SGArrayLength,
   P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
#if ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
   ,boolean SendInMainFunctionTx
#endif /* ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
);

#endif /* (COM_BASED_TRANSFORMER_SUPPORT_TX == STD_ON) */


/** \brief Com_EB_SendSignalGroupArray - internal function for updating and
 * sending a signal group
 * The service Com_EB_SendSignalGroupArray updates and sends a signal group.
 * It evaluates the
 * TMS flags of the group signals and calculates the TMS for the signal group
 * Preconditions:
 * - COM must be initialized.
 * \param[in] SGArrayPtr - Reference to the signal group array to be transmitted
 * \param[in] SignalGPtr - reference to configuration of signal group
 * \param[in] PduId - id of I-Pdu to which the signal group belongs
 * \param[in] IPduPtr - reference to the configuration of I-Pdu
 * \param[in] SendInMainFunctionTx - if TRUE, the Pdu is sent not now but
 * during the next Com_MainfunctionTx(), otherwise immediately
 * \return Function execution success status
 * \retval E_OK if signal group could be copied to the I-Pdu
 * \retval COM_SERVICE_NOT_AVAILABLE if corresponding I-PDU group was stopped
 * \retval COM_BUSY - The receive Tp buffer is actually not available
 */
TS_MOD_PRIV_DECL FUNC(uint8, COM_CODE) Com_EB_SendSignalGroupArray
(
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayPtr,
   P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
#if ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
   ,boolean SendInMainFunctionTx
#endif /* ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
);


/** \brief Com_EB_InitSGBuffer - internal function initialization of a signal group array
 * The service Com_EB_InitSGBuffer copies the value of the IPdu buffer to the signal group array.
 * Preconditions:
 * - COM must be initialized.
 * \param[in] SGArrayPtr - Reference to the signal group array
 * \param[in] SignalGPtr - reference to configuration of signal group
 * \param[in] IPduPtr - reference to the configuration of I-Pdu
 * \return number of bytes which have been initialized
 * \retval number of bytes which have been initialized
 */
TS_MOD_PRIV_DECL FUNC(uint16, COM_CODE) Com_EB_InitSGBuffer
(
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayPtr,
   P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
);


#if (COM_TMS_ENABLE == STD_ON)
/** \brief Com_EB_CalculateGroupSignalTM - calculates the transmission mode of
 * a group signal
 * The service Com_EB_CalculateGroupSignalTM evaluates the filter of the group
 * signal and sets the TMS flags
 * Preconditions:
 * - COM must be initialized.
 * - signal type COM_UINT8_N not allowed
 * \param[in] GSignalPtr - reference to configuration of group signal
 * \param[in] SignalGPtr - reference to configuration of signal group to
 * which the group signal belongs
 * \param[in] TMSResult - result of filter
 * \param[in] SignalType - type of the group signal
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_CalculateGroupSignalTM
(
   P2CONST(ComTxSignalType, AUTOMATIC, COM_APPL_CONST) GSignalPtr,
   P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   boolean TMSResult
);

#endif /* (COM_TMS_ENABLE == STD_ON) */

/** \brief Com_SetupTxSignalGroup - retrieves configuration data for in order to copy
 * a signal group array into the appropriate I-PDU buffer and calls function to copy it into
 * the I-PDU buffer
 * Preconditions:
 * \param[out] SGArrayPtr - Reference to the signal group array to be transmitted;
 *             if function referred with Com_TxSignalGroupFPtr does not need info,
 *             it may be set to NULL_PTR
 * \param[in] SGArrayLength - Length of the signal group array
 *             if function referred with Com_TxSignalGroupFPtr does not need info,
 *             it may be set to 0
 * \param[in] Com_TxSignalGroupFPtr - function which shall be used to
 * copy the value (either copy shadow buffer or signal group array)
 * \param[in] SignalGPtr - reference to configuration of signal group
 * \param[in] retvalPtr - pointer where return value of Com_TxSignalGroupFPtr shall be stored
 * \param[in] SendInMainFunctionTx - if TRUE, the Pdu is sent not now but
 * during the next Com_MainfunctionTx(), otherwise immediately
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_SetupTxSignalGroup
(
   P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) SGArrayPtr,
   uint16 SGArrayLength,
   P2VAR(Com_TxSignalGroupFPtrType, AUTOMATIC, COM_CODE) Com_TxSignalGroupFPtr,
   P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2VAR(uint8, AUTOMATIC, COM_APPL_DATA) retvalPtr
#if ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
   ,boolean SendInMainFunctionTx
#endif /* ( (COM_SENDUPONAPICALL_AND_SIGGW == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
);

#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>


#endif /* COM_CORE_TXSIGNALGROUP_H_ */
/*==================[end of file]===========================================*/

