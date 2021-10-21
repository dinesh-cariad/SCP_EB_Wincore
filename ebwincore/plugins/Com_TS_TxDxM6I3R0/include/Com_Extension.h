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

/* MISRA-C:2012 Deviation List
 *
 *
 * MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * ## operator is used for code generation via pre-processor.
 *
 */

#ifndef COM_EXTENSION_H
#define COM_EXTENSION_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <Com_Rules.h>          /* Generated configuration + derived rules */
#include <Com_Api.h>            /* get module interface */
#include <SchM_Com.h>           /* Header of Schedule Manager for Com */
#include <stddef.h>             /* ptrdiff_t */

/*==================[checks]================================================*/

/*==================[macros]================================================*/

#if (defined COM_GET_TM_TIME_OFFSET_FACTOR)        /* To prevent double declaration */
#error COM_GET_TM_TIME_OFFSET_FACTOR is already defined
#endif /* (defined COM_GET_TM_TIME_OFFSET_FACTOR) */

#if (COM_TX_MODE_TIME_OFFSET_FACTOR_SIZE == COM_SIZE_0_BIT)
#define COM_GET_TM_TIME_OFFSET_FACTOR(tmPtr) 0U
#else
#define COM_GET_TM_TIME_OFFSET_FACTOR(tmPtr) (tmPtr)->ComTxModeTimeOffsetFactor
#endif

#if (defined COM_CALC_MASK)        /* To prevent double declaration */
#error COM_CALC_MASK is already defined
#endif /* (defined COM_CALC_MASK) */

/** \brief Define COM_CALC_MASK generates a bit mask which is maximum
 * 32 bits
 * \param[in] type - the data type of the mask
 * \param[in] position - position of the mask
 * (LSB = 0, MSB = (sizeof(type)*8-1)
 * \param[in] num_bits - number of 1 in the mask
 * \return generated mask
 * \retval generated mask */
#define COM_CALC_MASK(type, position, num_bits) \
    ( (type) (((uint32) (UINT32_C(0xFFFFFFFF) >> ((32U) - (num_bits))) ) << (position) ))

#if (COM_RX_SIG_CONF_IMMEDIATE_ENABLE == STD_ON)
#if (COM_RX_SIG_CONF_DEFERRED_ENABLE == STD_ON)

#if (defined COM_GET_COM_RX_IPDU_SIGNAL_PROCESSING)        /* To prevent double declaration */
#error COM_GET_COM_RX_IPDU_SIGNAL_PROCESSING is already defined
#endif /* (defined COM_GET_COM_RX_IPDU_SIGNAL_PROCESSING) */
/** \brief Define COM_GET_COM_RX_IPDU_SIGNAL_PROCESSING returns the
 * processing of the indications of the signals within the I-Pdu
 * (COM_DEFERED/COM_IMMEDIATE)
 * \param[in] ipdu - reference to the Rx-Ipdu
 * \return processing flag
 * \retval processing flag (COM_DEFERED/COM_IMMEDIATE) */
#define COM_GET_COM_RX_IPDU_SIGNAL_PROCESSING(ipdu) \
   ((uint8)((uint8)((ipdu)->ComRxIPduFlags) & 0x1U ))
#endif /* (COM_RX_SIG_CONF_IMMEDIATE_ENABLE == STD_ON) */
#endif /* (COM_RX_SIG_CONF_DEFERRED_ENABLE == STD_ON) */

#if (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON)
#if (defined COM_GET_COM_RX_IPDU_DUMMY)        /* To prevent double declaration */
#error COM_GET_COM_RX_IPDU_DUMMY is already defined
#endif /* (defined COM_GET_COM_RX_IPDU_DUMMY) */
/** \brief Define COM_GET_COM_RX_IPDU_DUMMY returns if Rx-I-Pdu is a dummy Rx-I-Pdu or not.
 * \param[in] ipdu - reference to the Rx-Ipdu
 * \return dummy flag
 * \retval dummy flag */
#define COM_GET_COM_RX_IPDU_DUMMY(ipdu) \
   ((uint8)((uint8)((ipdu)->ComRxIPduFlags>>3U) & 0x1U ))
#endif /* (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) */

#if (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)

#if (defined COM_GET_COM_RX_IPDU_DM)        /* To prevent double declaration */
#error COM_GET_COM_RX_IPDU_DM is already defined
#endif /* (defined COM_GET_COM_RX_IPDU_DM) */
/** \brief Define COM_GET_COM_RX_IPDU_DM returns if deadline monitoring is
 * configured for the I-Pdu
 * \param[in] ipdu - reference to the Rx-Ipdu
 * \return deadline monitoring flag
 * \retval deadline monitoring flag (COM_RX_DM_ON/COM_RX_DM_OFF) */
#define COM_GET_COM_RX_IPDU_DM(ipdu) \
   ((uint8)((uint8)((ipdu)->ComRxIPduFlags>>1U) & 0x1U ))
#endif /* (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */

#if (defined COM_GET_COM_RX_IPDU_DYN)        /* To prevent double declaration */
#error COM_GET_COM_RX_IPDU_DYN is already defined
#endif /* (defined COM_GET_COM_RX_IPDU_DYN) */
/** \brief Define COM_GET_COM_RX_IPDU_DYN returns if Rx-I-Pdu is configured
 * with a dynamic length.
 * \param[in] ipdu - reference to the Rx-Ipdu
 * \return dynamic length flag
 * \retval dynamic length flag (COM_DYN_IPDU_ON/COM_DYN_IPDU_OFF) */
#define COM_GET_COM_RX_IPDU_DYN(ipdu) \
   ((uint8)((uint8)((ipdu)->ComRxIPduFlags>>2U) & 0x1U ))

#if (defined COM_GET_COM_TX_IPDU_SIGNAL_PROCESSING)        /* To prevent double declaration */
#error COM_GET_COM_TX_IPDU_SIGNAL_PROCESSING is already defined
#endif /* (defined COM_GET_COM_TX_IPDU_SIGNAL_PROCESSING) */
/** \brief Define COM_GET_COM_TX_IPDU_SIGNAL_PROCESSING returns the
 * processing of the indications of the signals within the I-Pdu
 * (COM_DEFERED/COM_IMMEDIATE)
 * \param[in] ipdu - reference to the Tx-Ipdu
 * \return processing flag
 * \retval processing flag (COM_DEFERED/COM_IMMEDIATE) */
#define COM_GET_COM_TX_IPDU_SIGNAL_PROCESSING(ipdu) \
   ((uint8)((uint8)((ipdu)->ComTxIPduFlags) & 0x1U ))

#if (defined COM_GET_COM_TX_IPDU_DM_AVAILABLE)        /* To prevent double declaration */
#error COM_GET_COM_TX_IPDU_DM_AVAILABLE is already defined
#endif /* (defined COM_GET_COM_TX_IPDU_DM_AVAILABLE) */
/** \brief Define COM_GET_COM_TX_IPDU_DM_AVAILABLE returns if deadline
 * monitoring is configured for the I-Pdu
 * \param[in] ipdu - reference to the Tx-Ipdu
 * \return deadline monitoring flag
 * \retval deadline monitoring flag (COM_TX_DM_ON/COM_TX_DM_OFF) */
#define COM_GET_COM_TX_IPDU_DM_AVAILABLE(ipdu) \
   ((uint8)((uint8)((ipdu)->ComTxIPduFlags>>1U) & 0x1U ))

#if (defined COM_GET_COM_TX_IPDU_INIT_TM)        /* To prevent double declaration */
#error COM_GET_COM_TX_IPDU_INIT_TM is already defined
#endif /* (defined COM_GET_COM_TX_IPDU_INIT_TM) */
/** \brief Define COM_GET_COM_TX_IPDU_INIT_TM returns the initial
 * transmission mode of the Tx-IPdu
 * \param[in] ipdu - reference to the Tx-Ipdu
 * \return initial transmission mode flag
 * \retval initial transmission mode flag (TM_TRUE/TM_FALSE) */
#define COM_GET_COM_TX_IPDU_INIT_TM(ipdu) \
   ((uint8)((uint8)((ipdu)->ComTxIPduFlags>>2U) & 0x1U ))

#if (defined COM_GET_COM_TX_IPDU_CLEAR_UPDATE_BIT)        /* To prevent double declaration */
#error COM_GET_COM_TX_IPDU_CLEAR_UPDATE_BIT is already defined
#endif /* (defined COM_GET_COM_TX_IPDU_CLEAR_UPDATE_BIT) */
/** \brief Define COM_GET_COM_TX_IPDU_CLEAR_UPDATE_BIT returns during which
 * function call the update bits shall be cleared
 * \param[in] ipdu - reference to the Tx-Ipdu
 * \return ClearUpdateBit
 * \retval ClearUpdateBit  (COM_CL_UB_TRANSMIT,
 * COM_CL_UB_TRIGGERTRANSMIT, COM_CL_UB_CONFIRMATION, COM_CL_UB_NEVER) */
#define COM_GET_COM_TX_IPDU_CLEAR_UPDATE_BIT(ipdu) \
   ((uint8)((uint8)((ipdu)->ComTxIPduFlags>>3U) & 0x3U ))

#if (defined COM_GET_COM_TX_IPDU_DYN)        /* To prevent double declaration */
#error COM_GET_COM_TX_IPDU_DYN is already defined
#endif /* (defined COM_GET_COM_TX_IPDU_DYN) */
/** \brief Define COM_GET_COM_TX_IPDU_DYN returns if Tx-I-Pdu is configured
 * with a dynamic length.
 * \param[in] ipdu - reference to the Tx-Ipdu
 * \return dynamic length flag
 * \retval dynamic length flag (COM_DYN_IPDU_ON/COM_DYN_IPDU_OFF) */
#define COM_GET_COM_TX_IPDU_DYN(ipdu) \
   ((uint8)((uint8)((ipdu)->ComTxIPduFlags>>5U) & 0x1U ))

#if (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON)
#if (defined COM_GET_COM_TX_IPDU_DUMMY)        /* To prevent double declaration */
#error COM_GET_COM_TX_IPDU_DUMMY is already defined
#endif /* (defined COM_GET_COM_TX_IPDU_DUMMY) */
/** \brief Define COM_GET_COM_TX_IPDU_DUMMY returns if Tx-I-Pdu is a dummy Tx-I-Pdu or not.
 * \param[in] ipdu - reference to the Tx-Ipdu
 * \return dummy flag
 * \retval dummy flag */
#define COM_GET_COM_TX_IPDU_DUMMY(ipdu) \
   ((uint8)((uint8)((ipdu)->ComTxIPduFlags>>6U) & 0x1U ))
#endif /* (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) */

#if (COM_TX_IPDU_MINIMUM_DELAY_TIME_FACTOR_SIZE != COM_SIZE_0_BIT)

#if (defined COM_GET_MDT)        /* To prevent double declaration */
#error COM_GET_MDT is already defined
#endif /* (defined COM_GET_MDT) */
/** \brief Define COM_GET_MDT gets the minimum delay time value of a I-Pdu
 * \param[in] ipduPtr - reference to the I-Pdu
 * \return value of MDT
 * \retval value of MDT
 */
#define COM_GET_MDT(ipduPtr) \
   ((ComTxIPduMinimumDelayTimeFactorType) (ipduPtr)->ComTxIPduMinimumDelayTimeFactorValue)

#endif /* (COM_TX_IPDU_MINIMUM_DELAY_TIME_FACTOR_SIZE != COM_SIZE_0_BIT) */


#if (COM_TM_AVAILABLE == STD_ON)

#if (defined COM_GET_COM_TX_MODE)        /* To prevent double declaration */
#error COM_GET_COM_TX_MODE is already defined
#endif /* (defined COM_GET_COM_TX_MODE) */
/** \brief Define COM_GET_COM_TX_MODE gets the transmission mode of a I-Pdu
 * \param[in] TM - TM which should be returned (TRUE/FALSE)
 * \param[in] ComTxModeModes - reference of the I-Pdu where the transmission
 * modes are stored
 * \return value of TM
 * \retval value of TM (COM_TX_MODE_MODE_NONE/COM_TX_MODE_MODE_PERIODIC/
 * COM_TX_MODE_MODE_DIRECT/COM_TX_MODE_MODE_MIXED_DIRECT/
 * COM_TX_MODE_MODE_MIXED_N/COM_TX_MODE_MODE_N_TIMES) */
#define COM_GET_COM_TX_MODE(TM, ComTxModeModes) \
   (((TM) == TRUE) ? (COM_GET_COM_TX_MODE_TRUE(ComTxModeModes)) : \
         (COM_GET_COM_TX_MODE_FALSE(ComTxModeModes)))

#if (defined COM_GET_COM_TX_MODE_INV)        /* To prevent double declaration */
#error COM_GET_COM_TX_MODE_INV is already defined
#endif /* (defined COM_GET_COM_TX_MODE_INV) */
/** \brief Define COM_GET_COM_TX_MODE_INV gets the inverted transmission mode
 * of a I-Pdu
 * \param[in] TM - TM which should be returned (if TRUE, TM FALSE is returned;
 * if FALSE, TM TRUE is returned)
 * \param[in] ComTxModeModes - reference of the I-Pdu where the transmission
 * modes are stored
 * \return value of TM
 * \retval value of TM (COM_TX_MODE_MODE_NONE/COM_TX_MODE_MODE_PERIODIC/
 * COM_TX_MODE_MODE_DIRECT/COM_TX_MODE_MODE_MIXED_DIRECT/
 * COM_TX_MODE_MODE_MIXED_N/COM_TX_MODE_MODE_N_TIMES) */
#define COM_GET_COM_TX_MODE_INV(TM, ComTxModeModes) \
   (((TM) == FALSE) ? (COM_GET_COM_TX_MODE_TRUE(ComTxModeModes)) : \
         (COM_GET_COM_TX_MODE_FALSE(ComTxModeModes)))

#if (defined COM_GET_COM_TX_MODE_TRUE)        /* To prevent double declaration */
#error COM_GET_COM_TX_MODE_TRUE is already defined
#endif /* (defined COM_GET_COM_TX_MO) */
/** \brief Define COM_GET_COM_TX_MODE_TRUE gets the transmission mode TRUE
 * of a I-Pdu
 * \param[in] ComTxModeModes - reference of the I-Pdu where the transmission
 * modes are stored
 * \return value of TM
 * \retval value of TM (COM_TX_MODE_MODE_NONE/COM_TX_MODE_MODE_PERIODIC/
 * COM_TX_MODE_MODE_DIRECT/COM_TX_MODE_MODE_MIXED_DIRECT/
 * COM_TX_MODE_MODE_MIXED_N/COM_TX_MODE_MODE_N_TIMES) */
#define COM_GET_COM_TX_MODE_TRUE(ComTxModeModes) \
   ((uint8)((ComTxModeModes) & (0xFU)))

#if (defined COM_GET_COM_TX_MODE_FALSE)        /* To prevent double declaration */
#error COM_GET_COM_TX_MODE_FALSE is already defined
#endif /* (defined COM_GET_COM_TX_MODE_FALSE) */
/** \brief Define COM_GET_COM_TX_MODE_TRUE gets the transmission mode FALSE
 * of a I-Pdu
 * \param[in] ComTxModeModes - reference of the I-Pdu where the transmission
 * modes are stored
 * \return value of TM
 * \retval value of TM (COM_TX_MODE_MODE_NONE/COM_TX_MODE_MODE_PERIODIC/
 * COM_TX_MODE_MODE_DIRECT/COM_TX_MODE_MODE_MIXED_DIRECT/
 * COM_TX_MODE_MODE_MIXED_N/COM_TX_MODE_MODE_N_TIMES)
 */
#define COM_GET_COM_TX_MODE_FALSE(ComTxModeModes) \
   ((uint8)((uint8)((ComTxModeModes)>>4U) & (0xFU)))

#if (defined COM_GET_COM_TX_MODE_REF_TRUE)        /* To prevent double declaration */
#error COM_GET_COM_TX_MODE_REF_TRUE is already defined
#endif /* (defined COM_GET_COM_TX_MODE_REF_TRUE) */
/** \brief Define COM_GET_COM_TX_MODE_REF_TRUE gets the reference of the
 * configuration of the transmission mode TRUE of a I-Pdu
 * \param[in] IPduPtr - reference ot the I-Pdu
 * \return reference to the TM TRUE
 * \retval reference to the TM TRUE
 */
#define COM_GET_COM_TX_MODE_REF_TRUE(IPduPtr) \
   ((ComTxModeIdxType)(IPduPtr)->ComTxModeTrueRef)

#if (defined COM_GET_COM_TX_MODE_REF_FALSE)        /* To prevent double declaration */
#error COM_GET_COM_TX_MODE_REF_FALSE is already defined
#endif /* (defined COM_GET_COM_TX_MODE_REF_FALSE) */
/** \brief Define COM_GET_COM_TX_MODE_REF_FALSE gets the reference of the
 * configuration of the transmission mode FALSE of a I-Pdu
 * \param[in] IPduPtr - reference ot the I-Pdu
 * \return reference to the TM FALSE
 * \retval reference to the TM FALSE (is always COM_TX_MODE_INVALID if TM
 * selection is disabled)
 */
#if (COM_TMS_ENABLE == STD_ON)
#define COM_GET_COM_TX_MODE_REF_FALSE(IPduPtr) \
         ((ComTxModeIdxType)(IPduPtr)->ComTxModeFalseRef)
#else
#define COM_GET_COM_TX_MODE_REF_FALSE(IPduPtr) COM_TX_MODE_INVALID
#endif /* (COM_TMS_ENABLE == STD_ON) */

#if (defined COM_GET_COM_TX_MODE_REF)        /* To prevent double declaration */
#error COM_GET_COM_TX_MODE_REF is already defined
#endif /* (defined COM_GET_COM_TX_MODE_REF) */
/** \brief Define COM_GET_COM_TX_MODE_REF gets the reference of the
 * configuration of the transmission mode of a I-Pdu
 * \param[in] TM - defines which reference should be returned (TRUE/FALSE)
 * \param[in] IPduPtr - reference ot the I-Pdu
 * \return reference to the TM specified in TM
 * \retval reference to the TM specified in TM (In case TM == FALSE and
 * TM selection is disabled COM_TX_MODE_INVALID is returned)
 */
#define COM_GET_COM_TX_MODE_REF(TM, IPduPtr) \
   (((TM) == TRUE)?(COM_GET_COM_TX_MODE_REF_TRUE(IPduPtr)) : \
         (COM_GET_COM_TX_MODE_REF_FALSE(IPduPtr)))

#endif /* (COM_TM_AVAILABLE == STD_ON) */



#if (COM_RX_DATA_TOUT_ACTION == COM_RX_DATA_TOUT_ACTION_CONFIG)

#if (defined COM_GET_RX_SIGNAL_GROUP_TIMEOUT_ACTION)        /* To prevent double declaration */
#error COM_GET_RX_SIGNAL_GROUP_TIMEOUT_ACTION is already defined
#endif /* (defined COM_GET_RX_SIGNAL_GROUP_TIMEOUT_ACTION) */
/** \brief Define COM_GET_RX_SIGNAL_GROUP_TIMEOUT_ACTION returns the defined
 * action in case of RxDM timeout
 * \param[in] signalExtPtr - reference to the signal extension config
 * \return timeout action of the signal group
 * \retval timeout action of the signal group (COM_RX_DATA_TOUT_ACTION_NONE/
 * COM_RX_DATA_TOUT_ACTION_REPLACE)
 */
#define COM_GET_RX_SIGNAL_GROUP_TIMEOUT_ACTION(signalExtPtr) \
   COM_GET_RX_SIGNAL_EXT_TIMEOUT_ACTION(signalExtPtr)
#endif /* (COM_RX_DATA_TOUT_ACTION == COM_RX_DATA_TOUT_ACTION_CONFIG) */

#if (COM_UPDATE_BIT_RX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL)

#if (defined COM_GET_RX_SIGNAL_GROUP_UPDATE_BIT)        /* To prevent double declaration */
#error COM_GET_RX_SIGNAL_GROUP_UPDATE_BIT is already defined
#endif /* (defined COM_GET_RX_SIGNAL_GROUP_UPDATE_BIT) */
/** \brief Define COM_GET_RX_SIGNAL_GROUP_UPDATE_BIT returns the presence of
 * an update bit for the signal group
 * \param[in] signalExtPtr - reference to the signal extension config
 * \return Function execution success status
 * \retval presence of an update bit for the signal group
 * (COM_UPDATE_BIT_ABSENT/COM_UPDATE_BIT_PRESENT)
 */
#define COM_GET_RX_SIGNAL_GROUP_UPDATE_BIT(signalExtPtr) \
   COM_GET_RX_SIGNAL_EXT_UPDATE_BIT(signalExtPtr)
#endif /* (COM_UPDATE_BIT_RX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL) */

#if (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON)
#if (defined COM_GET_RX_SIGNAL_GROUP_SIGDUMMY)        /* To prevent double declaration */
#error COM_GET_RX_SIGNAL_GROUP_SIGDUMMY is already defined
#endif /* (defined COM_GET_RX_SIGNAL_GROUP_SIGDUMMY) */
/** \brief Define COM_GET_RX_SIGNAL_GROUP_SIGDUMMY
 * returns if the signal group is a dummy signal group
 * \param[in] signalExtPtr - pointer to configuration of a signal group extension
 * \return TRUE if dummy signal group, FALSE if non dummy signal group
 * \retval TRUE if dummy signal group, FALSE if non dummy signal group
 */
#define COM_GET_RX_SIGNAL_GROUP_SIGDUMMY(signalExtPtr) \
   COM_GET_RX_SIGNAL_EXT_SIGDUMMY(signalExtPtr)
#endif /* (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) */

#if (COM_RX_SIGNAL_BASED_DM == STD_ON)

#if (defined COM_GET_RX_SIGNAL_GROUP_SIGDM)        /* To prevent double declaration */
#error COM_GET_RX_SIGNAL_GROUP_SIGDM is already defined
#endif /* (defined COM_GET_RX_SIGNAL_GROUP_SIGDM) */
/** \brief Define COM_GET_RX_SIGNAL_GROUP_SIGDM returns if
 * the a signal group based RxDM is configured for the signal group
 * \param[in] signalExtPtr - reference to the signal extension config
 * \return Function execution success status
 * \retval TRUE if invalid value equals init value, else FALSE
 * (COM_RX_DM_ON/COM_RX_DM_OFF)
 */
#define COM_GET_RX_SIGNAL_GROUP_SIGDM(signalExtPtr) \
   COM_GET_RX_SIGNAL_EXT_SIGDM(signalExtPtr)
#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */


/* calculate the RAM buffer of signal groups for signal base DM */
#if (COM_RX_SIGNAL_BASED_DM == STD_ON)

#if (defined COM_RX_SIGNAL_DM_TIMEOUT_BIT_POS_OFFSET)        /* To prevent double declaration */
#error COM_RX_SIGNAL_DM_TIMEOUT_BIT_POS_OFFSET is already defined
#endif /* (defined COM_RX_SIGNAL_DM_TIMEOUT_BIT_POS_OFFSET) */
#define COM_RX_SIGNAL_DM_TIMEOUT_BIT_POS_OFFSET 0U

#if (defined COM_RX_SIGNAL_DM_RUNNING_BIT_OFFSET)        /* To prevent double declaration */
#error COM_RX_SIGNAL_DM_RUNNING_BIT_OFFSET is already defined
#endif /* (defined COM_RX_SIGNAL_DM_RUNNING_BIT_OFFSET) */
#define COM_RX_SIGNAL_DM_RUNNING_BIT_OFFSET 1U

#if (defined COM_RX_SIGNAL_GROUP_BYTE_POS)        /* To prevent double declaration */
#error COM_RX_SIGNAL_GROUP_BYTE_POS is already defined
#endif /* (defined COM_RX_SIGNAL_GROUP_BYTE_POS) */
/** \brief Define COM_RX_SIGNAL_GROUP_BYTE_POS returns the byte position of the
 * flags of the signal group
 * \param[in] SignalGPtr - reference to the signal group
 * \return address of the byte where the flags of the signal group are stored
 * \retval address of the byte where the flags of the signal group are stored
 */
#define COM_RX_SIGNAL_GROUP_BYTE_POS(SignalGPtr) \
   (&Com_gDataMemPtr[(Com_gConfigPtr->ComRxDMInvSignalGroupStatusQueueBase + \
         ((ComRAMIdxType)(SignalGPtr)->RxSigDMQueueRef / 4U))])

#if (defined COM_GET_RX_SIGNAL_GROUP_BIT_POS)        /* To prevent double declaration */
#error COM_GET_RX_SIGNAL_GROUP_BIT_POS is already defined
#endif /* (defined COM_GET_RX_SIGNAL_GROUP_BIT_POS) */
/** \brief Define COM_GET_RX_SIGNAL_GROUP_BIT_POS returns the bit position of the
 * flags of the signal group
 * \param[in] SignalGPtr - reference to the signal group
 * \param[in] offset - offset of the bit relative to start of bits of the signal
 * \return position of the first flag of the signal group within a byte
 * \retval position of the first flag of the signal group within a byte
 */
#define COM_GET_RX_SIGNAL_GROUP_BIT_POS(SignalGPtr, offset) \
   (((((SignalGPtr)->RxSigDMQueueRef) % 4U) * 2U) + (offset))

/** \brief Define COM_RX_SIGNAL_GROUP_DM_FLAGS_BYTE_POS,
 * COM_RX_SIGNAL_GROUP_DM_TIMEOUT_BIT_POS,
 * COM_RX_SIGNAL_GROUP_DM_RUNNING_BIT_POS return the bit and byte position
 * of the DM timeout flag and the DM enable flag of a signal group
 * \param[in] SignalGPtr - reference to the signal group
 * \return position of the DM flags the signal group
 * \retval position of the DM flags the signal group
 */
#if (COM_RX_SIGNAL_BASED_DM == STD_ON)

#if (defined COM_RX_SIGNAL_GROUP_DM_FLAGS_BYTE_POS)        /* To prevent double declaration */
#error COM_RX_SIGNAL_GROUP_DM_FLAGS_BYTE_POS is already defined
#endif /* (defined COM_RX_SIGNAL_GROUP_DM_FLAGS_BYTE_POS) */
#define COM_RX_SIGNAL_GROUP_DM_FLAGS_BYTE_POS(SignalGPtr) \
   (COM_RX_SIGNAL_GROUP_BYTE_POS(SignalGPtr))

#if (defined COM_RX_SIGNAL_GROUP_DM_TIMEOUT_BIT_POS)        /* To prevent double declaration */
#error COM_RX_SIGNAL_GROUP_DM_TIMEOUT_BIT_POS is already defined
#endif /* (defined COM_RX_SIGNAL_GROUP_DM_TIMEOUT_BIT_POS) */
#define COM_RX_SIGNAL_GROUP_DM_TIMEOUT_BIT_POS(SignalGPtr) \
   (COM_RX_SIGNAL_DM_TIMEOUT_BIT_POS(SignalGPtr))

#if (defined COM_RX_SIGNAL_GROUP_DM_RUNNING_BIT_POS)        /* To prevent double declaration */
#error COM_RX_SIGNAL_GROUP_DM_RUNNING_BIT_POS is already defined
#endif /* (defined COM_RX_SIGNAL_GROUP_DM_RUNNING_BIT_POS) */
#define COM_RX_SIGNAL_GROUP_DM_RUNNING_BIT_POS(SignalGPtr) \
   COM_RX_SIGNAL_DM_RUNNING_BIT_POS(SignalGPtr)
#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */


#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */

#if (defined COM_GET_COM_TX_SIGNAL_GROUP_TRANSFER_PROPERTY)        /* To prevent double declaration */
#error COM_GET_COM_TX_SIGNAL_GROUP_TRANSFER_PROPERTY is already defined
#endif /* (defined COM_GET_COM_TX_SIGNAL_GROUP_TRANSFER_PROPERTY) */
#define COM_GET_COM_TX_SIGNAL_GROUP_TRANSFER_PROPERTY(signalGPtr) \
   ((uint8)((uint8)((signalGPtr)->ComTxSignalGroupFlags) & (0x3U)))

#if (defined COM_GET_COM_TX_SIGNAL_GROUP_TRANSFER_PROPERTY_WO_REP)        /* To prevent double declaration */
#error COM_GET_COM_TX_SIGNAL_GROUP_TRANSFER_PROPERTY_WO_REP is already defined
#endif /* (defined COM_GET_COM_TX_SIGNAL_GROUP_TRANSFER_PROPERTY_WO_REP) */
#define COM_GET_COM_TX_SIGNAL_GROUP_TRANSFER_PROPERTY_WO_REP(signalGPtr) \
   ((uint8)((uint8)(((signalGPtr)->ComTxSignalGroupFlags)>>2U) & (0x1U)))

#if (COM_UPDATE_BIT_TX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL)

#if (defined COM_GET_TX_SIGNAL_GROUP_UPDATE_BIT)        /* To prevent double declaration */
#error COM_GET_TX_SIGNAL_GROUP_UPDATE_BIT is already defined
#endif /* (defined COM_GET_TX_SIGNAL_GROUP_UPDATE_BIT) */
#define COM_GET_TX_SIGNAL_GROUP_UPDATE_BIT(signalGPtr) \
   ((uint8)((uint8)(((signalGPtr)->ComTxSignalGroupFlags)>>3U) & (0x1U)))
#endif

#if (defined COM_GET_TX_SIGNAL_GROUP_BUFFER_INIT)        /* To prevent double declaration */
#error COM_GET_TX_SIGNAL_GROUP_BUFFER_INIT is already defined
#endif /* (defined COM_GET_TX_SIGNAL_GROUP_BUFFER_INIT) */
#define COM_GET_TX_SIGNAL_GROUP_BUFFER_INIT(signalGPtr) \
   ((uint8)((uint8)(((signalGPtr)->ComTxSignalGroupFlags)>>4U) & (0x1U)))

#if (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON)
#if (defined COM_GET_TX_SIGNAL_GROUP_DUMMY)        /* To prevent double declaration */
#error COM_GET_TX_SIGNAL_GROUP_DUMMY is already defined
#endif /* (defined COM_GET_TX_SIGNAL_GROUP_DUMMY) */
#define COM_GET_TX_SIGNAL_GROUP_DUMMY(signalGPtr) \
   ((uint8)((uint8)(((signalGPtr)->ComTxSignalGroupFlags)>>5U) & (0x1U)))
#endif /* (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) */

#if (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)

#if (defined COM_GET_RX_TIMEOUT_FACTOR)        /* To prevent double declaration */
#error COM_GET_RX_TIMEOUT_FACTOR is already defined
#endif /* (defined COM_GET_RX_TIMEOUT_FACTOR) */
/** \brief COM_GET_RX_TIMEOUT_FACTOR
 * gets the Rx Timeout Factor of an IPdu
 * Preconditions:
 * - a Timeout Factor has to be configured for the IPdu
 * \param[in] IPduPtr configuration of the IPdu
 * \return Rx Timeout Factor
 * \retval Rx Timeout Factor
 */
#define COM_GET_RX_TIMEOUT_FACTOR(IPduPtr) \
   ((ComRxTimeoutFactorType)((IPduPtr)->ComRxTimeoutFactorValue))



#if (COM_RX_SIGNAL_BASED_DM == STD_ON)

#if (defined COM_GET_RX_SIGNAL_TIMEOUT_FACTOR)        /* To prevent double declaration */
#error COM_GET_RX_SIGNAL_TIMEOUT_FACTOR is already defined
#endif /* (defined COM_GET_RX_SIGNAL_TIMEOUT_FACTOR) */
/** \brief COM_GET_RX_SIGNAL_TIMEOUT_FACTOR
 * gets the Rx Timeout Factor of an Signal
 * Preconditions:
 * - a Timeout Factor has to be configured for the Signal
 * \param[in] signalPtr configuration of the signal
 * \return Rx Timeout Factor
 * \retval Rx Timeout Factor
 */
#define COM_GET_RX_SIGNAL_TIMEOUT_FACTOR(signalPtr) \
   ((ComRxTimeoutFactorType)((signalPtr)->ComRxTimeoutFactorValue))

#if (defined COM_GET_RX_SIGNAL_GROUP_TIMEOUT_FACTOR)        /* To prevent double declaration */
#error COM_GET_RX_SIGNAL_GROUP_TIMEOUT_FACTOR is already defined
#endif /* (defined COM_GET_RX_SIGNAL_GROUP_TIMEOUT_FACTOR) */
/** \brief COM_GET_RX_SIGNAL_GROUP_TIMEOUT_FACTOR
 * gets the Rx Timeout Factor of an Signal Group
 * Preconditions:
 * - a Timeout Factor has to be configured for the Signal Group
 * \param[in] signalGPtr configuration of the signal group
 * \return Rx Timeout Factor
 * \retval Rx Timeout Factor
 */
#define COM_GET_RX_SIGNAL_GROUP_TIMEOUT_FACTOR(signalGPtr) \
   ((ComRxTimeoutFactorType)((signalGPtr)->ComRxTimeoutFactorValue))

#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */

#endif /* (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */



#if (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)

#if (defined COM_GET_TX_TIMEOUT_FACTOR)        /* To prevent double declaration */
#error COM_GET_TX_TIMEOUT_FACTOR is already defined
#endif /* (defined COM_GET_TX_TIMEOUT_FACTOR) */
/** \brief COM_GET_TX_TIMEOUT_FACTOR
 * gets the Tx Timeout Factor of an IPdu
 * Preconditions:
 * - a Timeout Factor has to be configured for the IPdu
 * \param[in] IPduPtr configuration of the IPdu
 * \return Tx Timeout Factor
 * \retval Tx Timeout Factor
 */
#define COM_GET_TX_TIMEOUT_FACTOR(IPduPtr) \
   ((IPduPtr)->ComTxTimeoutFactorValue)

#endif /* (COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */

/* Deviation MISRAC2012-1 <START> */


#if (defined COM_CONCAT2) /* To prevent double declaration */
#error COM_CONCAT2 already defined
#endif /* if (defined COM_CONCAT2) */

/** \brief Helper macro for the concatenation of two arguments with a defined order */
#define COM_CONCAT2(param1,param2) param1 ## param2

#if (defined COM_CONCAT3) /* To prevent double declaration */
#error COM_CONCAT3 already defined
#endif /* if (defined COM_CONCAT3) */

/** \brief Helper macro for the concatenation of three arguments with a defined order */
#define COM_CONCAT3(param1,param2,param3) COM_CONCAT2(param1 ## param2, param3)

/* Deviation MISRAC2012-1 <STOP> */

#if (defined COM_GET_FLAG)        /* To prevent double declaration */
#error COM_GET_FLAG is already defined
#endif /* (defined COM_GET_FLAG) */
/** \brief COM_GET_FLAG
 * extracts flags from with a given size and offset
 * \param[in] flags - value where flag shall be extracted
 * \param[in] type - data type of the flags
 * \param[in] offset - offset in bits of the flag which shall be extracted
 * \param[in] size - size in bits of the flag which shall be extracted
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_FLAG(flags, type, offset, size) \
   (type)((type)((flags)>>(offset)) & (COM_CALC_MASK(type, 0U, (size))))

#if (defined COM_GET_RX_SIGNAL)        /* To prevent double declaration */
#error COM_GET_RX_SIGNAL is already defined
#endif /* (defined COM_GET_RX_SIGNAL) */
/** \brief COM_GET_RX_SIGNAL
 * helper macro for extraction of flags from a Rx signal configuration
 * \param[in] signalPtr - pointer to configuration of a signal
 * \param[in] FLAG - name of the flag which shall be extracted (is used for concatenation)
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_RX_SIGNAL(signalPtr, FLAG)\
      COM_GET_FLAG((signalPtr)->ComRxSignalFlags, uint8, COM_CONCAT3(COM_RX_SIGNAL_FLAG_, FLAG, _OFFSET), COM_CONCAT3(COM_RX_SIGNAL_FLAG_, FLAG, _SIZE))

#if (defined COM_GET_RX_SIGNAL_OPT_EXT)        /* To prevent double declaration */
#error COM_GET_RX_SIGNAL_OPT_EXT is already defined
#endif /* (defined COM_GET_RX_SIGNAL_OPT_EXT) */
/** \brief COM_GET_RX_SIGNAL_OPT_EXT
 * returns if the signal value extraction uses an optimized function
 * \param[in] signalPtr - pointer to configuration of a signal
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_RX_SIGNAL_OPT_EXT(signalPtr) COM_GET_RX_SIGNAL((signalPtr), OPT_EXT)

#if (defined COM_GET_RX_SIGNAL_SIGNAL_EXT)        /* To prevent double declaration */
#error COM_GET_RX_SIGNAL_SIGNAL_EXT is already defined
#endif /* (defined COM_GET_RX_SIGNAL_SIGNAL_EXT) */
/** \brief COM_GET_RX_SIGNAL_SIGNAL_EXT
 * returns if the signal has a extended configuration
 * \param[in] signalPtr - pointer to configuration of a signal
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_RX_SIGNAL_SIGNAL_EXT(signalPtr) COM_GET_RX_SIGNAL((signalPtr), SIGNAL_EXT)

#if (defined COM_GET_RX_SIGNAL_SIGNAL_TYPE)        /* To prevent double declaration */
#error COM_GET_RX_SIGNAL_SIGNAL_TYPE is already defined
#endif /* (defined COM_GET_RX_SIGNAL_SIGNAL_TYPE) */
/** \brief COM_GET_RX_SIGNAL_SIGNAL_TYPE
 * returns if the configured type of the signal
 * \param[in] signalPtr - pointer to configuration of a signal
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_RX_SIGNAL_SIGNAL_TYPE(signalPtr) COM_GET_RX_SIGNAL((signalPtr), SIGNAL_TYPE_CONF)

#if (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON)
#if (defined COM_IS_RX_DUMMY_SIGNAL)        /* To prevent double declaration */
#error COM_IS_RX_DUMMY_SIGNAL is already defined
#endif /* (defined COM_IS_RX_DUMMY_SIGNAL) */
/** \brief COM_IS_RX_DUMMY_SIGNAL
 * returns if the configured signal is a dummy signal
 * \param[in] signalPtr - pointer to configuration of a signal
 * \return TRUE if dummy signal, FALSE if non dummy signal
 * \retval TRUE if dummy signal, FALSE if non dummy signal
 */
#define COM_IS_RX_DUMMY_SIGNAL(signalPtr) ( (uint16)((signalPtr).ComRxSignalFlags & COM_CORE_READ_DUMMY_MASK) == COM_CORE_READ_DUMMY_MASK )
#endif /* (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) */

#if (defined COM_GET_RX_SIGNAL_ENDIANNESS)        /* To prevent double declaration */
#error COM_GET_RX_SIGNAL_ENDIANNESS is already defined
#endif /* (defined COM_GET_RX_SIGNAL_ENDIANNESS) */
/** \brief COM_GET_RX_SIGNAL_ENDIANNESS
 * returns the endianness of the signal
 * \param[in] signalPtr - pointer to configuration of a signal
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_RX_SIGNAL_ENDIANNESS(signalPtr) COM_GET_RX_SIGNAL((signalPtr), ENDIANNESS)

#if (defined COM_GET_RX_SIGNAL_EXT)        /* To prevent double declaration */
#error COM_GET_RX_SIGNAL_EXT is already defined
#endif /* (defined COM_GET_RX_SIGNAL_EXT) */
/** \brief COM_GET_RX_SIGNAL_EXT
 * helper macro for extraction of flags from a Rx signal extension configuration
 * \param[in] signalExtPtr - pointer to configuration of a signal extension
 * \param[in] FLAG - name of the flag which shall be extracted (is used for concatenation)
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_RX_SIGNAL_EXT(signalExtPtr, FLAG)\
      COM_GET_FLAG((signalExtPtr)->ComRxSignalExtFlags, uint8, COM_CONCAT3(COM_RX_SIGNAL_EXT_FLAG_, FLAG, _OFFSET), COM_CONCAT3(COM_RX_SIGNAL_EXT_FLAG_, FLAG, _SIZE))

#if (defined COM_GET_RX_SIGNAL_EXT_TIMEOUT_ACTION)        /* To prevent double declaration */
#error COM_GET_RX_SIGNAL_EXT_TIMEOUT_ACTION is already defined
#endif /* (defined COM_GET_RX_SIGNAL_EXT_TIMEOUT_ACTION) */
/** \brief COM_GET_RX_SIGNAL_EXT_TIMEOUT_ACTION
 * returns the timeout action of the signal
 * \param[in] signalExtPtr - pointer to configuration of a signal extension
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_RX_SIGNAL_EXT_TIMEOUT_ACTION(signalExtPtr) COM_GET_RX_SIGNAL_EXT((signalExtPtr), TIMEOUT_ACTION)

#if (defined COM_GET_RX_SIGNAL_EXT_UPDATE_BIT)        /* To prevent double declaration */
#error COM_GET_RX_SIGNAL_EXT_UPDATE_BIT is already defined
#endif /* (defined COM_GET_RX_SIGNAL_EXT_UPDATE_BIT) */
/** \brief COM_GET_RX_SIGNAL_EXT_UPDATE_BIT
 * returns if the signal has an update bit configured
 * \param[in] signalExtPtr - pointer to configuration of a signal extension
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_RX_SIGNAL_EXT_UPDATE_BIT(signalExtPtr) COM_GET_RX_SIGNAL_EXT((signalExtPtr), UPDATE_BIT)

#if (defined COM_GET_RX_SIGNAL_EXT_SIGDM)        /* To prevent double declaration */
#error COM_GET_RX_SIGNAL_EXT_SIGDM is already defined
#endif /* (defined COM_GET_RX_SIGNAL_EXT_SIGDM) */
/** \brief COM_GET_RX_SIGNAL_EXT_SIGDM
 * returns if the signal has signal based deadline monitoring configured
 * \param[in] signalExtPtr - pointer to configuration of a signal extension
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_RX_SIGNAL_EXT_SIGDM(signalExtPtr) COM_GET_RX_SIGNAL_EXT((signalExtPtr), SIGDM)

#if (defined COM_GET_RX_SIGNAL_EXT_PDUSIGDM)        /* To prevent double declaration */
#error COM_GET_RX_SIGNAL_EXT_PDUSIGDM is already defined
#endif /* (defined COM_GET_RX_SIGNAL_EXT_PDUSIGDM) */
/** \brief COM_GET_RX_SIGNAL_EXT_PDUSIGDM
 * returns if the signal has PDU based deadline monitoring configured
 * \param[in] signalExtPtr - pointer to configuration of a signal extension
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_RX_SIGNAL_EXT_PDUSIGDM(signalExtPtr) COM_GET_RX_SIGNAL_EXT((signalExtPtr), PDUSIGDM)

#if (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON)
#if (defined COM_GET_RX_SIGNAL_EXT_SIGDUMMY)        /* To prevent double declaration */
#error COM_GET_RX_SIGNAL_EXT_SIGDUMMY is already defined
#endif /* (defined COM_GET_RX_SIGNAL_EXT_SIGDUMMY) */
/** \brief COM_GET_RX_SIGNAL_EXT_SIGDUMMY
 * returns if the signal is a dummy signal
 * \param[in] signalExtPtr - pointer to configuration of a signal extension
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_RX_SIGNAL_EXT_SIGDUMMY(signalExtPtr) COM_GET_RX_SIGNAL_EXT((signalExtPtr), SIGDUMMY)
#endif /* (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) */

#if (COM_RX_SIGNAL_BASED_DM == STD_ON)

#if (defined COM_RX_SIGNAL_DM_FLAGS_BYTE_POS)        /* To prevent double declaration */
#error COM_RX_SIGNAL_DM_FLAGS_BYTE_POS is already defined
#endif /* (defined COM_RX_SIGNAL_DM_FLAGS_BYTE_POS) */
/** \brief COM_RX_SIGNAL_DM_FLAGS_BYTE_POS
 * returns the byte for a signal in which the status flags of signal based deadline monitoring
 * is stored
 * \param[in] signalPtr - pointer to configuration of a signal
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_RX_SIGNAL_DM_FLAGS_BYTE_POS(SignalPtr) \
   (&Com_gDataMemPtr[(Com_gConfigPtr->ComRxTimeoutSignalStatusQueueBase + \
         ((ComRAMIdxType)((SignalPtr)->RxSigDMQueueRef) / 4U))])

#if (defined COM_RX_SIGNAL_DM_TIMEOUT_BIT_POS)        /* To prevent double declaration */
#error COM_RX_SIGNAL_DM_TIMEOUT_BIT_POS is already defined
#endif /* (defined COM_RX_SIGNAL_DM_TIMEOUT_BIT_POS) */
/** \brief COM_RX_SIGNAL_DM_TIMEOUT_BIT_POS
 * returns position of the flag which indicates if the signal based deadline monitoring is timed out
 * \param[in] signalPtr - pointer to configuration of a signal
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_RX_SIGNAL_DM_TIMEOUT_BIT_POS(SignalPtr) \
   (COM_GET_RX_SIGNAL_GROUP_BIT_POS(SignalPtr, COM_RX_SIGNAL_DM_TIMEOUT_BIT_POS_OFFSET))

#if (defined COM_RX_SIGNAL_DM_RUNNING_BIT_POS)        /* To prevent double declaration */
#error COM_RX_SIGNAL_DM_RUNNING_BIT_POS is already defined
#endif /* (defined COM_RX_SIGNAL_DM_RUNNING_BIT_POS) */
/** \brief COM_RX_SIGNAL_DM_TIMEOUT_BIT_POS
 * returns position of the flag which indicates if the signal based deadline monitoring is running
 * \param[in] signalPtr - pointer to configuration of a signal
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_RX_SIGNAL_DM_RUNNING_BIT_POS(SignalPtr) \
   (COM_GET_RX_SIGNAL_GROUP_BIT_POS(SignalPtr, COM_RX_SIGNAL_DM_RUNNING_BIT_OFFSET))
#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */

#if (defined COM_GET_TX_SIGNAL)        /* To prevent double declaration */
#error COM_GET_TX_SIGNAL is already defined
#endif /* (defined COM_GET_TX_SIGNAL) */
/** \brief COM_GET_TX_SIGNAL
 * helper macro for extraction of flags from a Tx signal configuration
 * \param[in] signalPtr - pointer to configuration of a signal
 * \param[in] FLAG - name of the flag which shall be extracted (is used for concatenation)
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_TX_SIGNAL(signalPtr, FLAG)\
      COM_GET_FLAG((signalPtr)->ComTxSignalFlags, uint8, COM_CONCAT3(COM_TX_SIGNAL_FLAG_, FLAG, _OFFSET), COM_CONCAT3(COM_TX_SIGNAL_FLAG_, FLAG, _SIZE))

#if (defined COM_GET_TX_SIGNAL_SIGNAL_TYPE_OPT)        /* To prevent double declaration */
#error COM_GET_TX_SIGNAL_SIGNAL_TYPE_OPT is already defined
#endif /* (defined COM_GET_TX_SIGNAL_SIGNAL_TYPE_OPT) */
/** \brief COM_GET_TX_SIGNAL_SIGNAL_TYPE_OPT
 * returns the optimized type for value serialization
 * \param[in] signalPtr - pointer to configuration of a signal
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_TX_SIGNAL_SIGNAL_TYPE_OPT(signalPtr) COM_GET_TX_SIGNAL((signalPtr), SIGNAL_TYPE_OPT)

#if (defined COM_GET_TX_SIGNAL_SIGNAL_TYPE_SIGN)        /* To prevent double declaration */
#error COM_GET_TX_SIGNAL_SIGNAL_TYPE_SIGN is already defined
#endif /* (defined COM_GET_TX_SIGNAL_SIGNAL_TYPE_SIGN) */
/** \brief COM_GET_TX_SIGNAL_SIGNAL_TYPE_SIGN
 * returns the signedness of the signal
 * \param[in] signalPtr - pointer to configuration of a signal
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_TX_SIGNAL_SIGNAL_TYPE_SIGN(signalPtr) COM_GET_TX_SIGNAL((signalPtr), SIGNAL_TYPE_SIGN)

#if (defined COM_GET_TX_SIGNAL_ENDIANNESS)        /* To prevent double declaration */
#error COM_GET_TX_SIGNAL_ENDIANNESS is already defined
#endif /* (defined COM_GET_TX_SIGNAL_ENDIANNESS) */
/** \brief COM_GET_TX_SIGNAL_ENDIANNESS
 * returns the endianness of the signal
 * \param[in] signalPtr - pointer to configuration of a signal
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_TX_SIGNAL_ENDIANNESS(signalPtr) COM_GET_TX_SIGNAL((signalPtr), ENDIANNESS)

#if (defined COM_GET_TX_SIGNAL_TRANSFER_PROPERTY)        /* To prevent double declaration */
#error COM_GET_TX_SIGNAL_TRANSFER_PROPERTY is already defined
#endif /* (defined COM_GET_TX_SIGNAL_TRANSFER_PROPERTY) */
/** \brief COM_GET_TX_SIGNAL_TRANSFER_PROPERTY
 * returns the transfer property of the signal (triggered / pending only)
 * \param[in] signalPtr - pointer to configuration of a signal
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_TX_SIGNAL_TRANSFER_PROPERTY(signalPtr) COM_GET_TX_SIGNAL((signalPtr), TRANSFER_PROP)

#if (defined COM_GET_TX_SIGNAL_TOC)        /* To prevent double declaration */
#error COM_GET_TX_SIGNAL_TOC is already defined
#endif /* (defined COM_GET_TX_SIGNAL_TOC) */
/** \brief COM_GET_TX_SIGNAL_TOC
 * returns if the transfer property is triggeredOnChange
 * \param[in] signalPtr - pointer to configuration of a signal
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_TX_SIGNAL_TOC(signalPtr) COM_GET_TX_SIGNAL((signalPtr), TOC)


#if (defined COM_GET_TX_SIGNAL_TRANSFER_PROPERTY_WO_REP)        /* To prevent double declaration */
#error COM_GET_TX_SIGNAL_TRANSFER_PROPERTY_WO_REP is already defined
#endif /* (defined COM_GET_TX_SIGNAL_TRANSFER_PROPERTY_WO_REP) */
/** \brief COM_GET_TX_SIGNAL_REPS
 * returns if the transfer property is with or without repetitions
 * \param[in] signalPtr - pointer to configuration of a signal
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_TX_SIGNAL_TRANSFER_PROPERTY_WO_REP(signalPtr) COM_GET_TX_SIGNAL((signalPtr), TRANSFER_PROP_WO_REP)

#if (COM_UPDATE_BIT_TX_CONFIG == COM_UPDATE_BIT_INDIVIDUAL)

#if (defined COM_GET_TX_SIGNAL_UPDATE_BIT)        /* To prevent double declaration */
#error COM_GET_TX_SIGNAL_UPDATE_BIT is already defined
#endif /* (defined COM_GET_TX_SIGNAL_UPDATE_BIT) */
/** \brief COM_GET_TX_SIGNAL_UPDATE_BIT
 * returns if signal has an update bit configured
 * \param[in] signalPtr - pointer to configuration of a signal
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_TX_SIGNAL_UPDATE_BIT(signalPtr) COM_GET_TX_SIGNAL((signalPtr), UPDATE_BIT)
#endif

#if (defined COM_GET_TX_SIGNAL_SIGNAL_TYPE)        /* To prevent double declaration */
#error COM_GET_TX_SIGNAL_SIGNAL_TYPE is already defined
#endif /* (defined COM_GET_TX_SIGNAL_SIGNAL_TYPE) */
/** \brief COM_GET_TX_SIGNAL_SIGNAL_TYPE
 * returns if the configured type of the signal
 * \param[in] signalPtr - pointer to configuration of a signal
 * \return extracted flag
 * \retval extracted flag
 */
#define COM_GET_TX_SIGNAL_SIGNAL_TYPE(signalPtr) COM_GET_TX_SIGNAL((signalPtr), SIGNAL_TYPE_CONF)

#if (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON)
#if (defined COM_IS_TX_DUMMY_SIGNAL)        /* To prevent double declaration */
#error COM_IS_TX_DUMMY_SIGNAL is already defined
#endif /* (defined COM_IS_TX_DUMMY_SIGNAL) */
/** \brief COM_IS_TX_DUMMY_SIGNAL
 * returns if the configured signal is a dummy signal
 * \param[in] signalPtr - pointer to configuration of a signal
 * \return TRUE if dummy signal, FALSE if non dummy signal
 * \retval TRUE if dummy signal, FALSE if non dummy signal
 */
#define COM_IS_TX_DUMMY_SIGNAL(signalPtr) ( (uint16)((signalPtr)->ComTxSignalFlags & COM_CORE_WRITE_DUMMY_MASK) == COM_CORE_WRITE_DUMMY_MASK )
#endif /* (COM_PBCFG_VARIANT_SUPPORT_ENABLED == STD_ON) */

#if (COM_EVALUATE_TRIGGERED_OC == STD_ON)

#if (defined COM_TRIGGERED_OC_VALUE_CHANGED)        /* To prevent double declaration */
#error COM_TRIGGERED_OC_VALUE_CHANGED is already defined
#endif /* (defined COM_TRIGGERED_OC_VALUE_CHANGED) */
/** \brief COM_TRIGGERED_OC_VALUE_CHANGED
 * if trigger on change is enabled COM_EVALUATE_TRIGGERED_OC == STD_ON oldVal
 * and newVal are compared.
 * If COM_EVALUATE_TRIGGERED_OC == STD_OFF macro is empty.
 * \param[in] oldVal value to be compared
 * \param[in] newVal value to be compared
 * \param[out] retVal TRUE if oldVal and newVal differs, else unchanged
 */
#define COM_TRIGGERED_OC_VALUE_CHANGED(oldVal, newVal, retVal) \
   do {\
      if ((oldVal) != (newVal)) \
      { \
         (retVal) = TRUE; \
      } \
   }while(0)

#if (defined COM_TRIGGERED_OC_BYTE_VALUE_CHANGED_TOCMASK)        /* To prevent double declaration */
#error COM_TRIGGERED_OC_BYTE_VALUE_CHANGED_TOCMASK is already defined
#endif /* (defined COM_TRIGGERED_OC_BYTE_VALUE_CHANGED_TOCMASK) */
/** \brief COM_TRIGGERED_OC_BYTE_VALUE_CHANGED_TOCMASK
 * if trigger on change is enabled COM_EVALUATE_TRIGGERED_OC == STD_ON oldVal (Byte)
 * and newVal (Byte) are masked and compared.
 * If COM_EVALUATE_TRIGGERED_OC == STD_OFF macro is empty.
 * \param[in] oldVal value to be compared
 * \param[in] newVal value to be compared
 * \param[in] mask mask which is applied for comparison
 * \param[out] retVal TRUE if oldVal and newVal differs, else unchanged
 */
#define COM_TRIGGERED_OC_BYTE_VALUE_CHANGED_TOCMASK(oldVal, newVal, mask, retVal) \
   COM_TRIGGERED_OC_VALUE_CHANGED((uint8)((oldVal) & (mask)), (uint8)((newVal) & (mask)), (retVal))

#if (defined COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU)        /* To prevent double declaration */
#error COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU is already defined
#endif /* (defined COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU) */
/** \brief COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU
 * copies (one byte) from newVal to pduVal.
 * If trigger on change is enabled COM_EVALUATE_TRIGGERED_OC == STD_ON old pduVal
 * and newVal are compared.
 * If COM_EVALUATE_TRIGGERED_OC == STD_OFF just copies.
 * \param[in] pduVal value to be compared
 * \param[in] newVal value to be compared
 * \param[out] retVal TRUE if oldVal and newVal differs, else unchanged
 */
#define COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(pduVal, newVal, retVal) \
   do { \
      COM_TRIGGERED_OC_VALUE_CHANGED((pduVal), (newVal), (retVal)); \
      (pduVal) = (newVal); \
   } while (0)

#if (defined COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU_TOCMASK)        /* To prevent double declaration */
#error COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU_TOCMASK is already defined
#endif /* (defined COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU_TOCMASK) */
/** \brief COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU_TOCMASK
 * copies (one byte) from newVal to pduVal.
 * If trigger on change is enabled COM_EVALUATE_TRIGGERED_OC == STD_ON old pduVal
 * and newVal are masked and compared.
 * If COM_EVALUATE_TRIGGERED_OC == STD_OFF just copies.
 * \param[in] pduVal value to be compared
 * \param[in] newVal value to be compared
 * \param[in] mask mask which is applied for comparison
 * \param[out] retVal TRUE if oldVal and newVal differs, else unchanged
 */
#define COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU_TOCMASK(pduVal, newVal, mask, retVal) \
   do { \
      COM_TRIGGERED_OC_BYTE_VALUE_CHANGED_TOCMASK((pduVal), (newVal), (mask), (retVal)); \
      (pduVal) = (newVal); \
   } while (0)

#if (defined COM_TRIGGERED_OC_WRITE_BYTEARRAY_TO_PDU)        /* To prevent double declaration */
#error COM_TRIGGERED_OC_WRITE_BYTEARRAY_TO_PDU is already defined
#endif /* (defined COM_TRIGGERED_OC_WRITE_BYTEARRAY_TO_PDU) */
/** \brief COM_TRIGGERED_OC_WRITE_BYTEARRAY_TO_PDU
 * copies a byte array from newValPtr to pduVal.
 * If trigger on change is enabled COM_EVALUATE_TRIGGERED_OC == STD_ON old pduVal
 * and newVal are compared. If equal retVal is true otherwise false.
 * If COM_EVALUATE_TRIGGERED_OC == STD_OFF just copies.
 * \param[in] pduValuint8Ptr - reference to destination array to be copied/compared
 * \param[in] newValuint8Ptr - reference to source array to be copied/compared
 * \param[in] num - number of bytes which shall be copied/compared
 * \param[out] retVal TRUE if oldVal and newVal is equal, else FALSE
 */
#define COM_TRIGGERED_OC_WRITE_BYTEARRAY_TO_PDU(pduValuint8Ptr, newValuint8Ptr, num, retVal) \
   do { \
      uint16 i; \
      (retVal) = FALSE;          \
      for (i = 0U; i < (num); i++)              \
      { \
        COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU((pduValuint8Ptr)[i], (newValuint8Ptr)[i], (retVal)); \
      } \
   } while (0)

#if (defined COM_TRIGGERED_OC_WRITE_BYTEARRAY_TO_PDU_TOCMASK)        /* To prevent double declaration */
#error COM_TRIGGERED_OC_WRITE_BYTEARRAY_TO_PDU_TOCMASK is already defined
#endif /* (defined COM_TRIGGERED_OC_WRITE_BYTEARRAY_TO_PDU_TOCMASK) */
/** \brief COM_TRIGGERED_OC_WRITE_BYTEARRAY_TO_PDU_TOCMASK
 * copies a byte array from newValPtr to pduVal.
 * If trigger on change is enabled COM_EVALUATE_TRIGGERED_OC == STD_ON old pduVal
 * and newVal are masked and compared.
 * If COM_EVALUATE_TRIGGERED_OC == STD_OFF just copies.
  * Preconditions:
 * retVal shall be TRUE or FALSE.
 * \param[in] pduValuint8Ptr - reference to destination array to be copied/compared
 * \param[in] newValuint8Ptr - reference to source array to be copied/compared
 * \param[in] maskPtr - reference to mask array which shall be applied for comparison
 * \param[in] num - number of bytes which shall be copied/compared
 * \param[out] retVal TRUE if oldVal and newVal is differs, else unchanged
 */
#define COM_TRIGGERED_OC_WRITE_BYTEARRAY_TO_PDU_TOCMASK(pduValuint8Ptr, newValuint8Ptr, maskPtr, num, retVal) \
   do { \
      uint16 i; \
      for (i = 0U; i < (num); i++)              \
      { \
        COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU_TOCMASK((pduValuint8Ptr)[i], (newValuint8Ptr)[i], (maskPtr)[i], (retVal)); \
      } \
   } while (0)


#else

#if (defined COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU)        /* To prevent double declaration */
#error COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU is already defined
#endif /* (defined COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU) */
#define COM_TRIGGERED_OC_WRITE_BYTE_TO_PDU(pduVal, newVal, retVal) \
   (pduVal) = (newVal)

#if (defined COM_TRIGGERED_OC_WRITE_BYTEARRAY_TO_PDU)        /* To prevent double declaration */
#error COM_TRIGGERED_OC_WRITE_BYTEARRAY_TO_PDU is already defined
#endif /* (defined COM_TRIGGERED_OC_WRITE_BYTEARRAY_TO_PDU) */
#define COM_TRIGGERED_OC_WRITE_BYTEARRAY_TO_PDU(pduValuint8Ptr, newValuint8Ptr, num, retVal) \
      TS_MemCpy(pduValuint8Ptr, newValuint8Ptr, num)

#if (defined COM_TRIGGERED_OC_WRITE_BYTEARRAY_TO_PDU_TOCMASK)        /* To prevent double declaration */
#error COM_TRIGGERED_OC_WRITE_BYTEARRAY_TO_PDU_TOCMASK is already defined
#endif /* (defined COM_TRIGGERED_OC_WRITE_BYTEARRAY_TO_PDU_TOCMASK) */
#define COM_TRIGGERED_OC_WRITE_BYTEARRAY_TO_PDU_TOCMASK(pduValuint8Ptr, newValuint8Ptr, maskPtr, num, retVal) \
   COM_TRIGGERED_OC_WRITE_BYTEARRAY_TO_PDU((pduValuint8Ptr), (newValuint8Ptr), (num), (retVal))
#endif /* (COM_EVALUATE_TRIGGERED_OC == STD_ON) */


#if (defined COM_GET_TX_SIGNAL_POS_ALIG)        /* To prevent double declaration */
#error COM_GET_TX_SIGNAL_POS_ALIG is already defined
#endif /* (defined COM_GET_TX_SIGNAL_POS_ALIG) */
#define COM_GET_TX_SIGNAL_POS_ALIG(signalPtr, byte) \
do {\
   (byte) = (ComSignalDescBitType)((signalPtr)->TxBasicParam.ComSignalPositionInBit); \
} while(0)

#if (defined COM_GET_GROUP_SIGNAL_POS_ALIG)        /* To prevent double declaration */
#error COM_GET_GROUP_SIGNAL_POS_ALIG is already defined
#endif /* (defined COM_GET_GROUP_SIGNAL_POS_ALIG) */
#define COM_GET_GROUP_SIGNAL_POS_ALIG(signalPtr, byte) \
do {\
   (byte) = (ComSignalDescByteType)((signalPtr)->ComSignalPositionInBit); \
} while(0)

#if (defined COM_GET_CONFIG_ELEMENT)        /* To prevent double declaration */
#error COM_GET_CONFIG_ELEMENT is already defined
#endif /* (defined COM_GET_CONFIG_ELEMENT) */
/* there are MISRA deviations in COM_GET_CONFIG_ADDR_NOCHK (from TS_GETCONSTCFGNOCHK) */
#define COM_GET_CONFIG_ELEMENT(type, ref, id) \
   (&((P2CONST(type, AUTOMATIC, COM_APPL_CONST))(COM_GET_CONFIG_ADDR_NOCHK(type, ref)))[(id)])

#if (defined COM_GET_CONFIG_ELEMENT_GWTYPE)        /* To prevent double declaration */
#error COM_GET_CONFIG_ELEMENT_GWTYPE is already defined
#endif /* (defined COM_GET_CONFIG_ELEMENT_GWTYPE) */
/* Cast via void pointer is necessary when casting pointer pointing to a
   ComGW[Source|Destination]Type to a pointer pointing to a Com[Tx|Rx]Signal[Group]Type */
#define COM_GET_CONFIG_ELEMENT_GWTYPE(type, ref, id) \
   (&((P2CONST(type, AUTOMATIC, COM_APPL_CONST)) \
      (P2CONST(void, AUTOMATIC, COM_APPL_CONST)) \
      (COM_GET_CONFIG_ADDR_NOCHK(type, ref)))[(id)])

#if (defined COM_GET_CONFIG_ELEMENT_CHECK)        /* To prevent double declaration */
#error COM_GET_CONFIG_ELEMENT_CHECK is already defined
#endif /* (defined COM_GET_CONFIG_ELEMENT_CHECK) */
#define COM_GET_CONFIG_ELEMENT_CHECK(type, ref, id) \
   (&((P2CONST(type, AUTOMATIC, COM_APPL_CONST))(COM_GET_CONFIG_ADDR_CHK(type, ref)))[(id)])

#if (defined COM_GET_CONFIG_ELEMENT_TEMP_CHECK)        /* To prevent double declaration */
#error COM_GET_CONFIG_ELEMENT_TEMP_CHECK is already defined
#endif /* (defined COM_GET_CONFIG_ELEMENT_TEMP_CHECK) */
#define COM_GET_CONFIG_ELEMENT_TEMP_CHECK(tempCfgPtr, type, ref, id) \
   (&((P2CONST(type, AUTOMATIC, COM_APPL_CONST))(COM_GET_CONFIG_ADDR_TEMP_CHK(tempCfgPtr, type, ref)))[(id)])

#if (defined COM_GET_CONFIG_ELEMENT_TEMP_CHECK_GWTYPE)        /* To prevent double declaration */
#error COM_GET_CONFIG_ELEMENT_TEMP_CHECK_GWTYPE is already defined
#endif /* (defined COM_GET_CONFIG_ELEMENT_TEMP_CHECK_GWTYPE) */
/* Cast via void pointer is necessary when casting pointer pointing to a
   ComGW[Source|Destination]Type to a pointer pointing to a Com[Tx|Rx]Signal[Group]Type */
#define COM_GET_CONFIG_ELEMENT_TEMP_CHECK_GWTYPE(tempCfgPtr, type, ref, id) \
   (&((P2CONST(type, AUTOMATIC, COM_APPL_CONST)) \
      (P2CONST(void, AUTOMATIC, COM_APPL_CONST)) \
      (COM_GET_CONFIG_ADDR_TEMP_CHK(tempCfgPtr, type, ref)))[(id)])


#if (COM_SIGNAL_GW_ENABLE == STD_ON)

#if (defined COM_RX_IPDU_SIG_GW_FLAG_ADDRESS)        /* To prevent double declaration */
#error COM_RX_IPDU_SIG_GW_FLAG_ADDRESS is already defined
#endif /* (defined COM_RX_IPDU_SIG_GW_FLAG_ADDRESS) */
/** \brief COM_RX_IPDU_SIG_GW_FLAG_ADDRESS
 * return the address of the I-Pdu SigGWFlag
 * \param[in] ipduPtr - reference to the I-Pdu configuration
 * \return address of the SigGWFlag of the I-Pdu
 * \retval address of the SigGWFlag of the I-Pdu
 */
#define COM_RX_IPDU_SIG_GW_FLAG_ADDRESS(ipduPtr) \
   (&Com_gDataMemPtr[(Com_gConfigPtr->ComSigGWRxPduFlagBase + \
         ((ComRAMIdxType)((ipduPtr)->ComIPduSigGWFlagBit) / 8U))])

#if (defined COM_RX_IPDU_SIG_GW_FLAG_BIT)        /* To prevent double declaration */
#error COM_RX_IPDU_SIG_GW_FLAG_BIT is already defined
#endif /* (defined COM_RX_IPDU_SIG_GW_FLAG_BIT) */
/** \brief COM_RX_IPDU_SIG_GW_FLAG_BIT
 * return the bit of the I-Pdu SigGWFlag relative to COM_RX_IPDU_SIG_GW_FLAG_ADDRESS
 * \param[in] ipduPtr - reference to the I-Pdu configuration
 * \return bit of the SigGWFlag of the I-Pdu
 * \retval bit of the SigGWFlag of the I-Pdu
 */
#define COM_RX_IPDU_SIG_GW_FLAG_BIT(ipduPtr) (((ipduPtr)->ComIPduSigGWFlagBit) % 8U)


#if (COM_SIGGW_RX_FILTER_ENABLE == STD_ON)

#if (defined COM_RX_SIGNAL_SIG_GW_FLAG_ADDRESS)        /* To prevent double declaration */
#error COM_RX_SIGNAL_SIG_GW_FLAG_ADDRESS is already defined
#endif /* (defined COM_RX_SIGNAL_SIG_GW_FLAG_ADDRESS) */
/** \brief COM_RX_SIGNAL_SIG_GW_FLAG_ADDRESS
 * return the address of the signal SigGWFlag
 * \param[in] signalPtr - reference to the signal configuration
 * \return address of the SigGWFlag of the signal
 * \retval address of the SigGWFlag of the signal
 */
#define COM_RX_SIGNAL_SIG_GW_FLAG_ADDRESS(signalPtr) \
   (&Com_gDataMemPtr[(Com_gConfigPtr->ComSigGWRxSigFlagBase + \
         (((signalPtr)->ComSignalSigGWFlagBit) / 8U))])

#if (defined COM_RX_SIGNAL_SIG_GW_FLAG_BIT)        /* To prevent double declaration */
#error COM_RX_SIGNAL_SIG_GW_FLAG_BIT is already defined
#endif /* (defined COM_RX_SIGNAL_SIG_GW_FLAG_BIT) */
/** \brief COM_RX_SIGNAL_SIG_GW_FLAG_BIT
 * return bit position of the signal SigGWFlag relative to
 * COM_RX_SIGNAL_SIG_GW_FLAG_ADDRESS
 * \param[in] signalPtr - reference to the signal configuration
 * \return position of the SigGWFlag of the signal
 * \retval position of the SigGWFlag of the signal
 */
#define COM_RX_SIGNAL_SIG_GW_FLAG_BIT(signalPtr) (((signalPtr)->ComSignalSigGWFlagBit) % 8U)

#endif /* (COM_SIGGW_RX_FILTER_ENABLE == STD_ON) */


#endif /* (COM_SIGNAL_GW_ENABLE == STD_ON) */



#if (COM_TM_AVAILABLE == STD_ON)

#if (defined COM_TXSG_TMSSTATE) /* To prevent double declaration */
#error COM_TXSG_TMSSTATE already defined
#endif /* if (defined COM_TXSG_TMSSTATE) */

/** \brief Define COM_TXSG_TMSSTATE */
#define COM_TXSG_TMSSTATE 0U

#if (defined COM_TXSG_TMSMIGHTCHANGE)   /* To prevent double declaration */
#error COM_TXSG_TMSMIGHTCHANGE already defined
#endif /* if (defined COM_TXSG_TMSMIGHTCHANGE) */

/** \brief Define COM_TXSG_TMSMIGHTCHANGE */
#define COM_TXSG_TMSMIGHTCHANGE 1U


#if (defined COM_TXSG_TMS_MIGHT_CHANGE_FLAG_ADDRESS)   /* To prevent double declaration */
#error COM_TXSG_TMS_MIGHT_CHANGE_FLAG_ADDRESS already defined
#endif /* if (defined COM_TXSG_TMS_MIGHT_CHANGE_FLAG_ADDRESS) */

/** \brief COM_TXSG_TMS_MIGHT_CHANGE_FLAG_ADDRESS
 * returns the address of the TMS Flags of a signal group
 * \param[in] signalGId - id of the signal group
 * \return address of the TMS flags of the signal group
 * \retval address of the TMS flags of the signal group
 */
#define COM_TXSG_TMS_MIGHT_CHANGE_FLAG_ADDRESS(signalGId) \
   &Com_gDataMemPtr[(Com_gConfigPtr->ComTxSignalGroupStatusBase + (signalGId))]


#if (defined COM_GET_TX_SG_ID)        /* To prevent double declaration */
#error COM_GET_TX_SG_ID is already defined
#endif /* (defined COM_GET_TX_SG_ID) */
/** \brief COM_GET_TX_SG_ID
 * calculates the ID of the Signal Group
 * \param[in] result - variable where the ID of the signal group is written to
 * \param[in] signalGPtr - reference to the signal group configuration
 * \return ID of the signal group
 * \retval ID of the signal group
 */
#define COM_GET_TX_SG_ID(result, signalGPtr) \
    do \
    { \
        P2CONST(ComTxSignalGroupType, AUTOMATIC, COM_APPL_CONST) firstSignalGPtr =\
          COM_GET_CONFIG_ELEMENT(ComTxSignalGroupType, Com_gConfigPtr->ComTxSignalGroupRef, 0);\
        ptrdiff_t offset = (signalGPtr) - firstSignalGPtr; \
        (result) = (ComSignalGroupRefType) offset; \
    } while (0)

#endif /* (COM_TM_AVAILABLE == STD_ON) */


#if (COM_CBK_TX_T_OUT_ARRAY_SIZE_MAX != COM_INDEX_NONE)

#if (defined COM_GET_CBK_TX_T_OUT_IDX)        /* To prevent double declaration */
#error COM_GET_CBK_TX_T_OUT_IDX is already defined
#endif /* (defined COM_GET_CBK_TX_T_OUT_IDX) */
#define COM_GET_CBK_TX_T_OUT_IDX(listnum) \
   ((Com_CbkTxTOutIdxType) *COM_GET_CONFIG_ELEMENT(Com_CbkTxTOutIdxType, \
      Com_gConfigPtr->ComTxTimeoutNotificationRef, (listnum)))

#if (defined COM_GET_CBK_TX_ERROR_IDX)        /* To prevent double declaration */
#error COM_GET_CBK_TX_ERROR_IDX is already defined
#endif /* (defined COM_GET_CBK_TX_ERROR_IDX) */
#define COM_GET_CBK_TX_ERROR_IDX(listnum) \
   ((Com_CbkTxTOutIdxType) *COM_GET_CONFIG_ELEMENT(Com_CbkTxTOutIdxType, \
      Com_gConfigPtr->ComTxErrorNotificationRef, (listnum)))

#endif /* (COM_CBK_TX_T_OUT_ARRAY_SIZE_MAX != COM_INDEX_NONE) */


#if (COM_CBK_RX_T_OUT_ARRAY_SIZE_MAX != COM_INDEX_NONE)

#if (defined COM_GET_CBK_RX_T_OUT_IDX)        /* To prevent double declaration */
#error COM_GET_CBK_RX_T_OUT_IDX is already defined
#endif /* (defined COM_GET_CBK_RX_T_OUT_IDX) */
#define COM_GET_CBK_RX_T_OUT_IDX(listnum) \
   ((Com_CbkRxTOutIdxType) *COM_GET_CONFIG_ELEMENT(Com_CbkRxTOutIdxType, \
         Com_gConfigPtr->ComRxTimeoutNotificationRef, (listnum)))
#endif /* (COM_CBK_RX_T_OUT_ARRAY_SIZE_MAX != COM_INDEX_NONE) */


#if (COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE)

#if (defined COM_GET_CBK_TX_ACK_PTR_IDX)        /* To prevent double declaration */
#error COM_GET_CBK_TX_ACK_PTR_IDX is already defined
#endif /* (defined COM_GET_CBK_TX_ACK_PTR_IDX) */
#define COM_GET_CBK_TX_ACK_PTR_IDX(listnum) \
   ((Com_CbkTxAckIdxType) *COM_GET_CONFIG_ELEMENT(Com_CbkTxAckIdxType, \
         (Com_gConfigPtr->ComTxNotificationRef), (listnum)))
#endif /* (COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) */


#if (defined Com_ClearBitUInt8)        /* To prevent double declaration */
#error Com_ClearBitUInt8 is already defined
#endif /* (defined Com_ClearBitUInt8) */
/** \brief Com_ClearBitUInt8
 * clears a bit from a byte at the given address and position
 * \param[in] Address pointer to the variable of type uint8
 * \param[in] Bit number of the bit to clear (0..7)
 */
#define Com_ClearBitUInt8(Address, Bit)                         \
   *(Address) = (uint8)(*(Address) & (uint8)(~(uint8)(1U << (Bit))))


#if (defined Com_SetBitUInt8)        /* To prevent double declaration */
#error Com_SetBitUInt8 is already defined
#endif /* (defined Com_SetBitUInt8) */
/** \brief Com_SetBitUInt8
 * sets a bit from a byte at the given address and position
 * \param[in] Address pointer to the variable of type uint8
 * \param[in] Bit number of the bit to set (0..7)
 */
#define Com_SetBitUInt8(Address, Bit)                           \
      *(Address) = (uint8)(*(Address) | (uint8)(1U << (Bit)))

#if (defined Com_ClearBitUInt8Var)        /* To prevent double declaration */
#error Com_ClearBitUInt8Var is already defined
#endif /* (defined Com_ClearBitUInt8Var) */
/** \brief Com_ClearBitUInt8Var
 * clears a bit from a byte at the given variable and position
 * \param[in] Var variable of type uint8
 * \param[in] Bit number of the bit to clear (0..7)
 */
#define Com_ClearBitUInt8Var(Var, Bit)                          \
   (Var) = (uint8)((Var) & (uint8)(~(uint8)(1U << (Bit))))


#if (defined Com_SetBitUInt8Var)        /* To prevent double declaration */
#error Com_SetBitUInt8Var is already defined
#endif /* (defined Com_SetBitUInt8Var) */
/** \brief Com_SetBitUInt8Var
 * sets a bit from a byte at the given variable and position
 * \param[in] Var variable of type uint8
 * \param[in] Bit number of the bit to set (0..7)
 */
#define Com_SetBitUInt8Var(Var, Bit)                            \
   (Var) = (uint8)((Var) | (uint8)(1U << (Bit)))

#if (defined Com_IsBitSet_8)          /* To prevent double declaration */
#error Com_IsBitSet_8 already defined
#endif
/* Check if a bit is set in a variable of type uint8 */
#define Com_IsBitSet_8(Address,Bit) \
      TS_IsBitSet((Address), (Bit), uint8)

#if (COM_TX_DYN_LENGTH_IPDU_ENABLE == STD_ON)

#if (defined COM_TX_IPDU_SIZE_VAR)        /* To prevent double declaration */
#error COM_TX_IPDU_SIZE_VAR is already defined
#endif /* (defined COM_TX_IPDU_SIZE_VAR) */
#define COM_TX_IPDU_SIZE_VAR(PduId) (COM_GET_MEMPTR(ComIPduSizeType, Com_gConfigPtr->ComTxIPduLengthBase))[(PduId)]

#if (defined COM_GET_TX_IPDU_SIZE)        /* To prevent double declaration */
#error COM_GET_TX_IPDU_SIZE is already defined
#endif /* (defined COM_GET_TX_IPDU_SIZE) */
#define COM_GET_TX_IPDU_SIZE(PduId, IPduPtr) COM_TX_IPDU_SIZE_VAR(PduId)

#else /* (COM_TX_DYN_LENGTH_IPDU_ENABLE == STD_ON) */

#if (defined COM_GET_TX_IPDU_SIZE)        /* To prevent double declaration */
#error COM_GET_TX_IPDU_SIZE is already defined
#endif /* (defined COM_GET_TX_IPDU_SIZE) */
#define COM_GET_TX_IPDU_SIZE(PduId, IPduPtr) IPduPtr->ComIPduSizeMax

#endif /* (COM_TX_DYN_LENGTH_IPDU_ENABLE == STD_ON) */


#if ((COM_RX_DYN_LENGTH_IPDU_ENABLE == STD_ON) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON))

#if (defined COM_RX_IPDU_SIZE_VAR)        /* To prevent double declaration */
#error COM_RX_IPDU_SIZE_VAR is already defined
#endif /* (defined COM_RX_IPDU_SIZE_VAR) */
#define COM_RX_IPDU_SIZE_VAR(PduId) (COM_GET_MEMPTR(ComIPduSizeType, Com_gConfigPtr->ComRxIPduLengthBase))[(PduId)]

#if (defined COM_GET_RX_IPDU_SIZE)        /* To prevent double declaration */
#error COM_GET_RX_IPDU_SIZE is already defined
#endif /* (defined COM_GET_RX_IPDU_SIZE) */
#define COM_GET_RX_IPDU_SIZE(PduId, IPduPtr) COM_RX_IPDU_SIZE_VAR(PduId)

#if (defined COM_SET_RX_IPDU_SIZE)        /* To prevent double declaration */
#error COM_SET_RX_IPDU_SIZE is already defined
#endif /* (defined COM_SET_RX_IPDU_SIZE) */
#define COM_SET_RX_IPDU_SIZE(PduId, length) (COM_RX_IPDU_SIZE_VAR(PduId) = (length))

#else /* ((COM_RX_DYN_LENGTH_IPDU_ENABLE == STD_ON) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON)) */

#if (defined COM_GET_RX_IPDU_SIZE)        /* To prevent double declaration */
#error COM_GET_RX_IPDU_SIZE is already defined
#endif /* (defined COM_GET_RX_IPDU_SIZE) */
#define COM_GET_RX_IPDU_SIZE(PduId, IPduPtr) (IPduPtr)->ComIPduSizeMax; TS_PARAM_UNUSED((PduId))

#endif /* ((COM_RX_DYN_LENGTH_IPDU_ENABLE == STD_ON) || (COM_HANDLESMALLERRXPDUS_NOTIFY == STD_ON)) */

#if (defined COM_GET_RX_PDU_ID)        /* To prevent double declaration */
#error COM_GET_RX_PDU_ID is already defined
#endif /* (defined COM_GET_RX_PDU_ID) */
/** \brief COM_GET_RX_PDU_ID
 * calculates the ID of an RxIpdu
 * \param[in] result - variable where the ID shall be stored
 * \param[in] IPduPtr - reference to the RxIpdu configuration
 */
#define COM_GET_RX_PDU_ID(result, IPduPtr) \
    do \
    { \
        P2CONST(ComRxIPduType, AUTOMATIC, COM_APPL_CONST) firstCfgPtr =\
          COM_GET_CONFIG_ELEMENT(ComRxIPduType, Com_gConfigPtr->ComRxIPduRef, 0);\
        ptrdiff_t offset = (IPduPtr) - firstCfgPtr; \
        (result) = (ComIPduRefType) offset; \
    } while (0)

/* Interim solution to fix Misra Violations from depricated TS_Atomic functions */
#if (defined COM_AtomicSetBit_8)        /* To prevent double declaration */
#error COM_AtomicSetBit_8 is already defined
#endif /* (defined COM_AtomicSetBit_8) */
/** \brief COM_AtomicSetBit_8
 * atomically sets bit
 * \param[in] addr - pointer to object where bit shall be set
 * \param[in] bitIdx - index where bit shall be set
 */
#define COM_AtomicSetBit_8(addr, bitIdx) \
TS_AtomicSetBit_8((addr), ((uint32)((uint16)((0xFFFFU) & (bitIdx)))))

#if (defined COM_AtomicClearBit_8)        /* To prevent double declaration */
#error COM_AtomicClearBit_8 is already defined
#endif /* (defined COM_AtomicClearBit_8) */
/** \brief COM_AtomicClearBit_8
 * atomically clears bit
 * \param[in] addr - pointer to object where bit shall be cleared
 * \param[in] bitIdx - index where bit shall be cleared
 */
#define COM_AtomicClearBit_8(addr, bitIdx) \
TS_AtomicClearBit_8((addr), ((uint32)((uint16)((0xFFFFU) & (bitIdx)))))
/* End of interim solution to fix Misra Violations from depricated TS_Atomic functions */

#if (defined COM_AtomicAssign_PduLengthType)        /* To prevent double declaration */
#error COM_AtomicAssign_PduLengthType is already defined
#endif /* (defined COM_AtomicAssign_PduLengthType) */
/** \brief COM_AtomicAssign_PduLengthType
 * calls AtomicAssign16 or AtomicAssign32 based on PduLengthType type
 * \param[in] to - PduLengthType entity which is the destination of the assignment
 * \param[in] from - entity which is the source of the assignment
 */
#if (TS_SIZE_PduLengthType == 4U)
#define COM_AtomicAssign_PduLengthType(to, from) \
TS_AtomicAssign32((to), (uint32) (from))
#else
#define COM_AtomicAssign_PduLengthType(to, from) \
TS_AtomicAssign16((to), (from))
#endif /* (TS_SIZE_PduLengthType == 4U) */

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef COM_EXTENSION_H */
/*==================[end of file]===========================================*/
