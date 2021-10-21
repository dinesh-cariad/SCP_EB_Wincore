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
#ifndef COM_CORE_RXUPDATEBIT_H_
#define COM_CORE_RXUPDATEBIT_H_
/*==================[inclusions]============================================*/


#include <TSAutosar.h>          /* EB specific standard types */
#include <Com_Api.h>                /* get module interface */
#include <Com_Lcfg_Static.h>    /* declaration of the callback arrays */

#if (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)

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

#if (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)

/** \brief Com_EB_CheckRxSignalGoupUpdateBitAndDM - handles update bit and DM of
 * Rx signals groups
 * this function evaluates the update bit and DM of Rx signals groups and
 * returns whether the value has to be replaced or not
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] SignalGPtr - reference to the description of the signal group
 * \param[in] IPduPtr - reference to the description of the signal group
 * \param[in] pduLength - length of Pdu
 * \return Function execution success status
 * \retval TRUE if the signal was updated, else FALSE
 */
TS_MOD_PRIV_DECL FUNC(boolean, COM_CODE) Com_EB_CheckRxSignalGoupUpdateBitAndDM
(
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   uint32 pduLength
);

#endif /* (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */

/** \brief Com_EB_CheckRxSignalUpdateBitAndDM - handles update bit and DM of
 * Rx signals this function evaluates the update bit and DM of Rx signals and
 * returns whether the value has to be replaced or not
 * Preconditions:
 * - prior to this call, the COM must be initialized.
 * \param[in] SignalPtr - reference to the description of the signal
 * \param[in] IPduPtr - reference to the description of the signal
 * \param[in,out] passFilter_MASKED_NEW_DIFFERS_MASKED_OLD -
 *         in: indicates if the Pdu base RxDM has been just restarted
 *         out: If signal based RxDM has just been restarted after a time out, the value is set to 1.
                If signal based RxDM did not timed out before, value is set to 0.
                If no update bit is configured, value is not modified.
 * \param[in] pduLength - received length of the PDU
 * \return Function execution success status
 * \retval TRUE if the signal was updated, else FALSE
 */

TS_MOD_PRIV_DECL FUNC(boolean, COM_CODE) Com_EB_CheckRxSignalUpdateBitAndDM
(
   P2CONST(ComRxSignalType, AUTOMATIC, COM_APPL_CONST) SignalPtr,
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   P2VAR(uint8, AUTOMATIC, AUTOMATIC) passFilter_MASKED_NEW_DIFFERS_MASKED_OLD,
   uint32 pduLength
);


#if (COM_RX_SIGNAL_BASED_DM == STD_ON)


/** \brief Com_EB_RxDMSignalGroupQueue - "rewind" the RxDM timeout for a
 * signal group
 * This function starts a timeout for the Rx deadline monitoring of a
 * signal group
 * Preconditions:
 * - must only be invoked within a critical section
 * - prior to this call, the COM must be initialized.
 * \param[in] SignalGPtr pointer to configuration of the Rx signal group to
 * start deadline monitoring
 * \param[in] TimeOutOffset - value which shall be used for "rewind"
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_RxDMSignalGroupQueue
(
   P2CONST(ComRxSignalGroupType, AUTOMATIC, COM_APPL_CONST) SignalGPtr,
   Com_RxDMNextDueTimeType TimeOutOffset
#if (COM_TIMEBASE == STD_OFF)
   , Com_MainFunctionRx_IdxType ComMainFunctionsRx_Idx
#endif /* (COM_TIMEBASE == STD_OFF) */
);



/** \brief Com_EB_RxDMSignalQueue - "rewind" the RxDM timeout for a signal
 * This function starts a timeout for the Rx deadline monitoring of a signal
 * Preconditions:
 * - must only be invoked within a critical section
 * - prior to this call, the COM must be initialized.
 * \param[in] SignalExtPtr pointer to extended configuration of the Rx signal to start
 * deadline monitoring
 * \param[in] TimeOutOffset - value which shall be used for "rewind"
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_RxDMSignalQueue
(
   P2CONST(ComRxSignalExtType, AUTOMATIC, COM_APPL_CONST) SignalExtPtr,
   Com_RxDMNextDueTimeType TimeOutOffset
#if (COM_TIMEBASE == STD_OFF)
   , Com_MainFunctionRx_IdxType ComMainFunctionsRx_Idx
#endif /* (COM_TIMEBASE == STD_OFF) */
);

/** \brief Com_EB_EnableSignalBasedDMFlag - sets up signal based DM
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * \param[in] IPduPtr - configuration of the I-Pdu
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_EnableSignalBasedDMFlag
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
);


/** \brief Com_EB_EnableSignalGroupBasedDMFlag - sets up signal group based DM
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * \param[in] IPduPtr - configuration of the I-Pdu
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_EnableSignalGroupBasedDMFlag
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr
);


/** \brief Com_EB_DisableSignalAndSignalGroupBasedDMFlag - disables
 * signal(group) based DM
 * Preconditions:
 * - prior to this call, the COM module shall be initialized
 * \param[in] IPduPtr - configuration of the I-Pdu
 * \param[in] Initialize - defines if timeout flag shall be cleared
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_DisableSignalAndSignalGroupBasedDMFlag
(
   P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
   boolean Initialize
);

#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */

#if (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON)

/** \brief Com_EB_HandleShortPdu_HandleUB - copies an update bit from external buffer to the Rx I-PDU buffer
 * \param[in] IPduPtr - configuration of the I-Pdu
 * \param[in] SduDataPtr - pointer to external buffer
 * \param[in] SduLength - length of external buffer
 * \param[in] ComUpdateBitPositionInBit - position of the update bit
 * \param[in] IsReceived - shall be TRUE if the signal / signal group has be fully received, else FALSE
 */
TS_MOD_PRIV_DECL FUNC(void, COM_CODE) Com_EB_HandleShortPdu_HandleUB
(
    P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) IPduPtr,
    P2VAR(uint8, AUTOMATIC, COM_VAR_CLEARED) SduDataPtr,
    PduLengthType SduLength,
    ComSignalDescBitType ComUpdateBitPositionInBit,
    uint8 IsReceived
);

#endif /*  (COM_HANDLESMALLERRXPDUS_CPY_SIG == STD_ON) */

#define COM_STOP_SEC_CODE
#include <Com_MemMap.h>

#endif /* (COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) */


#endif /* COM_CORE_RXUPDATEBIT_H_ */
/*==================[end of file]===========================================*/

