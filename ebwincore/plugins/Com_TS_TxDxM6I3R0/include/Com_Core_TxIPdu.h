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
#ifndef COM_CORE_TXIPDU_H_
#define COM_CORE_TXIPDU_H_
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


/** \brief Com_EB_Send_Pdu
 * Function which sends out the PDU (calls the I-Pdu callout if configured)
  * Preconditions:
 * - function does not check if MDT is running -> invoke only if MDT is not
 *   running or MDT shall be ignored.
 * \param[in] PduId - Id of the IPdu the API call was called for
 * \param[in] IPduPtr - reference to configuration of Pdu
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_Send_Pdu
(
   ComIPduRefType PduId,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
);


/** \brief Com_EB_HandleTxIPdu - handles a Tx IPdu
 * function which manages starting/stopping a Tx I-Pdu
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 *
 * \param[in] toBeActivated - state of start/stop
 * \param[in] IPduPtr - configuration of the I-Pdu
 * \param[in] Initialize - defines if the Pdu group should be initialized
 * in case of COM_START_GROUP
 * \param[in] IPduId - Id of the I-Pdu
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_HandleTxIPdu
(
   boolean toBeActivated,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   boolean Initialize,
   ComIPduRefType IPduId
);


/** \brief Com_EB_InitTxIPdu - initializes the specified Tx IPdu
 * \param[in] TxPduId -IPdu which should be initialized
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_InitTxIPdu
(
   ComIPduRefType TxPduId
);

#if (COM_TMS_ENABLE == STD_ON)
/** \brief Com_EB_InitTxIPdu - clear/initializes TxSGTMSState flag byte
 * \param[in] SignalGId -SignalGroupID where the TxSGTMSState flag byte
 * should be initialized
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_ClearTxSGTMSState
(
    ComIPduRefType SignalGId
);
#endif /* (COM_TMS_ENABLE == STD_ON) */


#if (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)

/** \brief Com_EB_TxDMQueue - "rewind" the TxDM timeout for an IPDU
 * This function starts a timeout for the Tx deadline monitoring of an IPDU
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] PduId ID of the Tx PDU to start deadline monitoring for
 * \param[in] Offset Time (in COM ticks) to pass by before sending
 * \param[in] EntryInTxDMcQueue position in of the PDU in TxDM Queue
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_TxDMQueue
(
   ComIPduRefType PduId,
   ComTxTimeoutFactorType Offset,
   ComIPduRefType EntryInTxDMcQueue
);

/** \brief Com_EB_TxDM_TMNone - "rewind" the TxDM timeout for an IPDU with
 * TM only one TM configured.
 * This function starts a timeout for the Tx deadline monitoring of an IPDU
 * for an I-Pdu which has TxDM configured and only one Transmission Mode.
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * - shall only be called in the current TM is TM_NONE
 * \param[in] IPduPtr pointer to the configuration of the PDU
 * \param[in] PduId ID of the Tx PDU to start deadline monitoring for
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_TxDM_TMNone
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   ComIPduRefType PduId
);

#endif /* (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */




#if (COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)
/** \brief Com_EB_TxConfirmations - calls all Tx notifications of the signals
 * which are within a PDU
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] IPduPtr pointer to the configuration of the PDU
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_TxConfirmations
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
);

#endif /* (COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */


#if (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)

/** \brief Com_EB_ClearTxUpdateBits - clears the update bit within the I-Pdu.
 * function which clears the update bit of the signals and signal groups
 * within the specified I-Pdu
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * - must not be called from any function but Com_TxConfirmation
 * - must only be called for IPdus which have MDT configured
 * \param[in] IPduPtr reference to the I-Pdu
 * or stopped (false)
 */
TS_MOD_PRIV_DECL FUNC(void, COM_APPL_CODE) Com_EB_ClearTxUpdateBits
(
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
);

#endif /* (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) */


#if (COM_MDT_QUEUE_AVAILABLE == STD_ON)

/** \brief Com_EB_MdtQueue - set a minimum delay time for a PDU.
 * function which blocks sending of a PDU for an amount of time to implement
 * the minimum delay time mechanism of the COM.
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * - must not be called from any function but Com_TxConfirmation
 * - must only be called for IPdus which have MDT configured
 * \param[in] PduId ID of the PDU to be blocked for its' minimum delay time
 * or stopped (false)
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_MdtQueue
(
   ComIPduRefType Pdu,
   P2CONST(ComTxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   ComTxIPduMinimumDelayTimeFactorType MDT
);

#endif /* (COM_MDT_QUEUE_AVAILABLE == STD_ON) */


#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>



#endif /* COM_CORE_TXIPDU_H_ */
/*==================[end of file]===========================================*/


