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
#ifndef COM_CORE_TM_H_
#define COM_CORE_TM_H_
/*==================[inclusions]============================================*/


#include <TSAutosar.h>          /* EB specific standard types */
#include <Com_Api.h>                /* get module interface */
#if ((COM_TM_AVAILABLE == STD_ON) || (COM_TMS_ENABLE == STD_ON))

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

/** \brief Com_EB_SendUponSendApiCall
 * Function which reacts on a send API call (SendSignal / SendSignalGroup)
 * \param[in] PduId - Id of the IPdu the API call was called for
 * \param[in] SendInMainFunctionTx - if TRUE, the Pdu is sent not now
 *            but deferred to the next main function
 * during the next Com_MainfunctionTx(), otherwise immediately
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_SendUponSendApiCall
(
   ComIPduRefType PduId
#if ( (COM_SIGNAL_GW_ENABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
   ,boolean SendInMainFunctionTx
#endif /* ( (COM_SIGNAL_GW_ENABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
);



/** \brief Com_EB_Request_Send_Pdu
 * Function which tries to send out an I-PDU, otherwise defers transmission to next main function
 * \param[in] PduId - Id of the IPdu the API call was called for
 * \param[in] IPduPtr - reference to the configuration of the I-Pdu
 * \param[in] PduStatusFlagsPtr - reference status of the I-Pdu
 * \param[in] SendInMainFunctionTx - if TRUE, the Pdu is sent not now but
 * during the next Com_MainfunctionTx(), otherwise immediately
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_Request_Send_Pdu
(
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
#if ((COM_MDT_QUEUE_AVAILABLE == STD_ON) || (COM_SIGNAL_GW_ENABLE == STD_ON))
   ,P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) PduStatusFlagsPtr
#endif
#if ( (COM_SIGNAL_GW_ENABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) )
   ,boolean SendInMainFunctionTx
#endif /* ( (COM_SIGNAL_GW_ENABLE == STD_ON) && (COM_DEFERTX2MAINFUNC == STD_OFF) ) */
);


#endif /* (COM_SENDUPONAPICALL == STD_ON) */


#if (COM_N_TIMES_QUEUE_AVAILABLE == STD_ON)
/** \brief Com_EB_NTimesSendQueue - "queue" an PDU for sending in an
 * amount of time
 * this function schedules the sending of a PDU in some amount of time
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * - Must only be called for IPdus which have a transmission mode with
 * ntimes sending part
 * \param[in] PduId - ID of the PDU to be sent
 * \param[in] Offset - Time (in COM ticks) to pass by before sending
 * \param[in] EntryInNTimesQueue - position in of the PDU in N-Times Queue
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_NTimesSendQueue
(
   ComIPduRefType PduId,
   ComTxModeRepetitionPeriodFactorType Offset,
   ComIPduRefType EntryInNTimesQueue
);
#endif /* (COM_N_TIMES_QUEUE_AVAILABLE == STD_ON) */


#if (COM_TMS_ENABLE == STD_ON)

/** \brief Com_EB_CalculateTM - calculates the TM of a Pdu
 * Preconditions:
 * - COM must be initialized
 * - should only be called in a critical section
 * \param[in] IPduPtr - reference to the configuration of the I-Pdu
 * \param[out] None
 * \return Function execution success status
 * \retval TRUE, TM is true; FALSE TM is false
 */
TS_MOD_PRIV_DECL FUNC(boolean, COM_CODE) Com_EB_CalculateTM
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
);

#endif /* (COM_TMS_ENABLE == STD_ON) */


#if (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON)
/** \brief Com_EB_PeriodicSendQueue - "queue" an PDU for sending in an
 * amount of time
 * this function schedules the sending of a PDU in some amount of time
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * - Must only be called for IPdus which have a transmission mode with
 * periodic sending part
 * \param[in] Offset Time (in ticks) to pass by before sending
  * \param[in] EntryInPeriodicQueue position in of the PDU in Periodic Queue
 * or stopped (false)
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_PeriodicSendQueue
(
   Com_PeriodicSendNextDueTimeType Offset,
   ComIPduRefType EntryInPeriodicQueue
#if (COM_TIMEBASE == STD_OFF)
   ,
   Com_MainFunctionTx_IdxType Com_MainFunctionTx_Idx
#endif /* (COM_TIMEBASE == STD_OFF) */
);

#endif /* (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON) */


#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>

#endif /* ((COM_TM_AVAILABLE == STD_ON) || (COM_TMS_ENABLE == STD_ON)) */


#endif /* COM_CORE_TM_H_ */
/*==================[end of file]===========================================*/

