/**
 * \file
 *
 * \brief AUTOSAR FrIf
 *
 * This file contains the implementation of the AUTOSAR
 * module FrIf.
 *
 * \version 5.3.25
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef FRIF_EXTENSION_H
#define FRIF_EXTENSION_H

/*======================[Include File Checking Section]=======================*/

#ifndef FRIF_API_H
#error this file is not self-contained but depends on FrIf_Api.h which was not included
#endif

/*======================[Include Section]=====================================*/

#include <TSAutosar.h>          /* EB specific standard types */

/*======================[Global Macros]=======================================*/

/**
 * \brief IRQ source
 *
 * Startup completed interrupt source index.
 */
#define FRIF_IRQ_STARTUP_COMPLETED     (0x00U)

/**
 * \brief IRQ source
 *
 * Cycle start interrupt source index.
 */
#define FRIF_IRQ_CYCLE_START           (0x01U)

/**
 * \brief IRQ source
 *
 * Dynamic segment start interrupt source index.
 */
#define FRIF_IRQ_DYNAMICSEGMENT_START  (0x02U)

/**
 * \brief IRQ source
 *
 * Change of NM-Vector interrupt source index.
 */
#define FRIF_IRQ_NMVECTOR_CHANGED      (0x03U)

/* check for defined STD_ON macro value */
#ifndef STD_ON
#error STD_ON not defined
#endif  /* STD_ON */


/* check for defined STD_OFF macro value */
#ifndef STD_OFF
#error STD_OFF not defined
#endif  /* STD_OFF */

/* test for valid macro definition of FRIF_REPORT_TO_DET_ENABLE */
#ifndef FRIF_REPORT_TO_DET_ENABLE
#error FRIF_REPORT_TO_DET_ENABLE not defined
#endif  /* FRIF_REPORT_TO_DET_ENABLE */

/* test for valid value of FRIF_REPORT_TO_DET_ENABLE */
#if (FRIF_REPORT_TO_DET_ENABLE != STD_ON) && \
    (FRIF_REPORT_TO_DET_ENABLE != STD_OFF)
#error macro FRIF_REPORT_TO_DET_ENABLE has an invalid value
#endif /* FRIF_REPORT_TO_DET_ENABLE */


/* test for valid macro definition of FRIF_SINGLE_CTRL_OPT_ENABLE */
#ifndef FRIF_SINGLE_CTRL_OPT_ENABLE
#error FRIF_SINGLE_CTRL_OPT_ENABLE not defined
#endif  /* FRIF_SINGLE_CTRL_OPT_ENABLE */

/* test for valid value of FRIF_SINGLE_CTRL_OPT_ENABLE */
#if (FRIF_SINGLE_CTRL_OPT_ENABLE != STD_ON) && \
    (FRIF_SINGLE_CTRL_OPT_ENABLE != STD_OFF)
#error macro FRIF_SINGLE_CTRL_OPT_ENABLE has an invalid value
#endif /* FRIF_SINGLE_CTRL_OPT_ENABLE */


/* test for valid macro definition of FRIF_SINGLE_CLST_OPT_ENABLE */
#ifndef FRIF_SINGLE_CLST_OPT_ENABLE
#error FRIF_SINGLE_CLST_OPT_ENABLE not defined
#endif  /* FRIF_SINGLE_CLST_OPT_ENABLE */

/* test for valid value of FRIF_SINGLE_CLST_OPT_ENABLE */
#if (FRIF_SINGLE_CLST_OPT_ENABLE != STD_ON) && \
    (FRIF_SINGLE_CLST_OPT_ENABLE != STD_OFF)
#error macro FRIF_SINGLE_CLST_OPT_ENABLE has an invalid value
#endif /* FRIF_SINGLE_CLST_OPT_ENABLE */


/* test for valid macro definition of FRIF_SINGLE_FRTRCV_OPT_ENABLE */
#ifndef FRIF_SINGLE_FRTRCV_OPT_ENABLE
#error FRIF_SINGLE_FRTRCV_OPT_ENABLE not defined
#endif  /* FRIF_SINGLE_FRTRCV_OPT_ENABLE */

/* test for valid value of FRIF_SINGLE_FRTRCV_OPT_ENABLE */
#if (FRIF_SINGLE_FRTRCV_OPT_ENABLE != STD_ON) && \
    (FRIF_SINGLE_FRTRCV_OPT_ENABLE != STD_OFF)
#error macro FRIF_SINGLE_FRTRCV_OPT_ENABLE has an invalid value
#endif /* FRIF_SINGLE_FRTRCV_OPT_ENABLE */


/* test for valid macro definition of FRIF_SINGLE_FR_OPT_ENABLE */
#ifndef FRIF_SINGLE_FR_OPT_ENABLE
#error FRIF_SINGLE_FR_OPT_ENABLE not defined
#endif  /* FRIF_SINGLE_FR_OPT_ENABLE */

/* test for valid value of FRIF_SINGLE_FR_OPT_ENABLE */
#if (FRIF_SINGLE_FR_OPT_ENABLE != STD_ON) && \
    (FRIF_SINGLE_FR_OPT_ENABLE != STD_OFF)
#error macro FRIF_SINGLE_FR_OPT_ENABLE has an invalid value
#endif /* FRIF_SINGLE_FR_OPT_ENABLE */


/* test for valid macro definition of FRIF_FR_INTEGRATION_ENABLE */
#ifndef FRIF_FR_INTEGRATION_ENABLE
#error FRIF_FR_INTEGRATION_ENABLE not defined
#endif  /* FRIF_FR_INTEGRATION_ENABLE */

/* test for valid value of FRIF_FR_INTEGRATION_ENABLE */
#if (FRIF_FR_INTEGRATION_ENABLE != STD_ON) && \
    (FRIF_FR_INTEGRATION_ENABLE != STD_OFF)
#error macro FRIF_FR_INTEGRATION_ENABLE has an invalid value
#endif /* FRIF_FR_INTEGRATION_ENABLE */


/* test for valid macro definition of FRIF_JOBLIST_IRQMUX_ENABLE */
#ifndef FRIF_JOBLIST_IRQMUX_ENABLE
#error FRIF_JOBLIST_IRQMUX_ENABLE not defined
#endif  /* FRIF_JOBLIST_IRQMUX_ENABLE */

/* test for valid value of FRIF_JOBLIST_IRQMUX_ENABLE */
#if (FRIF_JOBLIST_IRQMUX_ENABLE != STD_ON) && \
    (FRIF_JOBLIST_IRQMUX_ENABLE != STD_OFF)
#error macro FRIF_JOBLIST_IRQMUX_ENABLE has an invalid value
#endif /* FRIF_JOBLIST_IRQMUX_ENABLE */


/* test for valid macro definition of FRIF_JOBLIST_IRQDELAYCHECK_ENABLE */
#ifndef FRIF_JOBLIST_IRQDELAYCHECK_ENABLE
#error FRIF_JOBLIST_IRQDELAYCHECK_ENABLE not defined
#endif  /* FRIF_JOBLIST_IRQDELAYCHECK_ENABLE */

/* test for valid value of FRIF_JOBLIST_IRQDELAYCHECK_ENABLE */
#if (FRIF_JOBLIST_IRQDELAYCHECK_ENABLE != STD_ON) && \
    (FRIF_JOBLIST_IRQDELAYCHECK_ENABLE != STD_OFF)
#error macro FRIF_JOBLIST_IRQDELAYCHECK_ENABLE has an invalid value
#endif /* FRIF_JOBLIST_IRQDELAYCHECK_ENABLE */


/* test for valid macro definition of FRIF_JOBLIST_PREPARE_LPDU_ENABLE */
#ifndef FRIF_JOBLIST_PREPARE_LPDU_ENABLE
#error FRIF_JOBLIST_PREPARE_LPDU_ENABLE not defined
#endif  /* FRIF_JOBLIST_PREPARE_LPDU_ENABLE */

/* test for valid value of FRIF_JOBLIST_PREPARE_LPDU_ENABLE */
#if (FRIF_JOBLIST_PREPARE_LPDU_ENABLE != STD_ON) && \
    (FRIF_JOBLIST_PREPARE_LPDU_ENABLE != STD_OFF)
#error macro FRIF_JOBLIST_PREPARE_LPDU_ENABLE has an invalid value
#endif /* FRIF_JOBLIST_PREPARE_LPDU_ENABLE */


/* test for valid macro definition of FRIF_GETNMVECTOR_API_ENABLE */
#ifndef FRIF_GETNMVECTOR_API_ENABLE
#error FRIF_GETNMVECTOR_API_ENABLE not defined
#endif  /* FRIF_GETNMVECTOR_API_ENABLE */

/* test for valid value of FRIF_GETNMVECTOR_API_ENABLE */
#if (FRIF_GETNMVECTOR_API_ENABLE != STD_ON) && \
    (FRIF_GETNMVECTOR_API_ENABLE != STD_OFF)
#error macro FRIF_GETNMVECTOR_API_ENABLE has an invalid value
#endif /* FRIF_GETNMVECTOR_API_ENABLE */


/* test for valid macro definition of FRIF_ALLOWCOLDSTART_API_ENABLE */
#ifndef FRIF_ALLOWCOLDSTART_API_ENABLE
#error FRIF_ALLOWCOLDSTART_API_ENABLE not defined
#endif  /* FRIF_ALLOWCOLDSTART_API_ENABLE */

/* test for valid value of FRIF_ALLOWCOLDSTART_API_ENABLE */
#if (FRIF_ALLOWCOLDSTART_API_ENABLE != STD_ON) && \
    (FRIF_ALLOWCOLDSTART_API_ENABLE != STD_OFF)
#error macro FRIF_ALLOWCOLDSTART_API_ENABLE has an invalid value
#endif /* FRIF_ALLOWCOLDSTART_API_ENABLE */

/* test for valid macro definition of FRIF_GETCONTROLLERERRORSTATUS_API_ENABLE */
#ifndef FRIF_GETCONTROLLERERRORSTATUS_API_ENABLE
#error FRIF_GETCONTROLLERERRORSTATUS_API_ENABLE not defined
#endif  /* FRIF_GETCONTROLLERERRORSTATUS_API_ENABLE */

/* test for valid value of FRIF_GETCONTROLLERERRORSTATUS_API_ENABLE */
#if (FRIF_GETCONTROLLERERRORSTATUS_API_ENABLE != STD_ON) && \
    (FRIF_GETCONTROLLERERRORSTATUS_API_ENABLE != STD_OFF)
#error macro FRIF_GETCONTROLLERERRORSTATUS_API_ENABLE has an invalid value
#endif /* FRIF_GETCONTROLLERERRORSTATUS_API_ENABLE */


/* test for valid macro definition of FRIF_ALLSLOTS_API_ENABLE */
#ifndef FRIF_ALLSLOTS_API_ENABLE
#error FRIF_ALLSLOTS_API_ENABLE not defined
#endif  /* FRIF_ALLSLOTS_API_ENABLE */

/* test for valid value of FRIF_ALLSLOTS_API_ENABLE */
#if (FRIF_ALLSLOTS_API_ENABLE != STD_ON) && \
    (FRIF_ALLSLOTS_API_ENABLE != STD_OFF)
#error macro FRIF_ALLSLOTS_API_ENABLE has an invalid value
#endif /* FRIF_ALLSLOTS_API_ENABLE */


/* test for valid macro definition of FRIF_RECONFIGLPDU_API_ENABLE */
#ifndef FRIF_RECONFIGLPDU_API_ENABLE
#error FRIF_RECONFIGLPDU_API_ENABLE not defined
#endif  /* FRIF_RECONFIGLPDU_API_ENABLE */

/* test for valid value of FRIF_RECONFIGLPDU_API_ENABLE */
#if (FRIF_RECONFIGLPDU_API_ENABLE != STD_ON) && \
    (FRIF_RECONFIGLPDU_API_ENABLE != STD_OFF)
#error macro FRIF_RECONFIGLPDU_API_ENABLE has an invalid value
#endif /* FRIF_RECONFIGLPDU_API_ENABLE */


/* test for valid macro definition of FRIF_DISABLELPDU_API_ENABLE */
#ifndef FRIF_DISABLELPDU_API_ENABLE
#error FRIF_DISABLELPDU_API_ENABLE not defined
#endif  /* FRIF_DISABLELPDU_API_ENABLE */

/* test for valid value of FRIF_DISABLELPDU_API_ENABLE */
#if (FRIF_DISABLELPDU_API_ENABLE != STD_ON) && \
    (FRIF_DISABLELPDU_API_ENABLE != STD_OFF)
#error macro FRIF_DISABLELPDU_API_ENABLE has an invalid value
#endif /* FRIF_DISABLELPDU_API_ENABLE */


/* test for valid macro definition of FRIF_GETWAKEUPRXSTATUS_API_ENABLE */
#ifndef FRIF_GETWAKEUPRXSTATUS_API_ENABLE
#error FRIF_GETWAKEUPRXSTATUS_API_ENABLE not defined
#endif  /* FRIF_GETWAKEUPRXSTATUS_API_ENABLE */

/* test for valid value of FRIF_GETWAKEUPRXSTATUS_API_ENABLE */
#if (FRIF_GETWAKEUPRXSTATUS_API_ENABLE != STD_ON) && \
    (FRIF_GETWAKEUPRXSTATUS_API_ENABLE != STD_OFF)
#error macro FRIF_GETWAKEUPRXSTATUS_API_ENABLE has an invalid value
#endif /* FRIF_GETWAKEUPRXSTATUS_API_ENABLE */


/* test for valid macro definition of FRIF_RELATIVETIMER_API_ENABLE */
#ifndef FRIF_RELATIVETIMER_API_ENABLE
#error FRIF_RELATIVETIMER_API_ENABLE not defined
#endif  /* FRIF_RELATIVETIMER_API_ENABLE */

/* test for valid value of FRIF_RELATIVETIMER_API_ENABLE */
#if (FRIF_RELATIVETIMER_API_ENABLE != STD_ON) && \
    (FRIF_RELATIVETIMER_API_ENABLE != STD_OFF)
#error macro FRIF_RELATIVETIMER_API_ENABLE has an invalid value
#endif /* FRIF_RELATIVETIMER_API_ENABLE */


/* test for valid macro definition of FRIF_SETEXTSYNC_API_ENABLE */
#ifndef FRIF_SETEXTSYNC_API_ENABLE
#error FRIF_SETEXTSYNC_API_ENABLE not defined
#endif  /* FRIF_SETEXTSYNC_API_ENABLE */

/* test for valid value of FRIF_SETEXTSYNC_API_ENABLE */
#if (FRIF_SETEXTSYNC_API_ENABLE != STD_ON) && \
    (FRIF_SETEXTSYNC_API_ENABLE != STD_OFF)
#error macro FRIF_SETEXTSYNC_API_ENABLE has an invalid value
#endif /* FRIF_SETEXTSYNC_API_ENABLE */


/* test for valid macro definition of FRIF_MTS_API_ENABLE */
#ifndef FRIF_MTS_API_ENABLE
#error FRIF_MTS_API_ENABLE not defined
#endif  /* FRIF_MTS_API_ENABLE */

/* test for valid value of FRIF_MTS_API_ENABLE */
#if (FRIF_MTS_API_ENABLE != STD_ON) && \
    (FRIF_MTS_API_ENABLE != STD_OFF)
#error macro FRIF_MTS_API_ENABLE has an invalid value
#endif /* FRIF_MTS_API_ENABLE */


/* test for valid macro definition of FRIF_WAKEUP_API_ENABLE */
#ifndef FRIF_WAKEUP_API_ENABLE
#error FRIF_WAKEUP_API_ENABLE not defined
#endif  /* FRIF_WAKEUP_API_ENABLE */

/* test for valid value of FRIF_WAKEUP_API_ENABLE */
#if (FRIF_WAKEUP_API_ENABLE != STD_ON) && \
    (FRIF_WAKEUP_API_ENABLE != STD_OFF)
#error macro FRIF_WAKEUP_API_ENABLE has an invalid value
#endif /* FRIF_WAKEUP_API_ENABLE */


/* test for valid macro definition of FRIF_GETIRQSTATUS_API_ENABLE */
#ifndef FRIF_GETIRQSTATUS_API_ENABLE
#error FRIF_GETIRQSTATUS_API_ENABLE not defined
#endif  /* FRIF_GETIRQSTATUS_API_ENABLE */

/* test for valid value of FRIF_GETIRQSTATUS_API_ENABLE */
#if (FRIF_GETIRQSTATUS_API_ENABLE != STD_ON) && \
    (FRIF_GETIRQSTATUS_API_ENABLE != STD_OFF)
#error macro FRIF_GETIRQSTATUS_API_ENABLE has an invalid value
#endif /* FRIF_GETIRQSTATUS_API_ENABLE */


/* test for valid macro definition of FRIF_DISABLEIRQ_API_ENABLE */
#ifndef FRIF_DISABLEIRQ_API_ENABLE
#error FRIF_DISABLEIRQ_API_ENABLE not defined
#endif  /* FRIF_DISABLEIRQ_API_ENABLE */

/* test for valid value of FRIF_DISABLEIRQ_API_ENABLE */
#if (FRIF_DISABLEIRQ_API_ENABLE != STD_ON) && \
    (FRIF_DISABLEIRQ_API_ENABLE != STD_OFF)
#error macro FRIF_DISABLEIRQ_API_ENABLE has an invalid value
#endif /* FRIF_DISABLEIRQ_API_ENABLE */


/* test for valid macro definition of FRIF_EXT_IRQ_SERVICES_API_ENABLE */
#ifndef FRIF_EXT_IRQ_SERVICES_API_ENABLE
#error FRIF_EXT_IRQ_SERVICES_API_ENABLE not defined
#endif  /* FRIF_EXT_IRQ_SERVICES_API_ENABLE */

/* test for valid value of FRIF_EXT_IRQ_SERVICES_API_ENABLE */
#if (FRIF_EXT_IRQ_SERVICES_API_ENABLE != STD_ON) && \
    (FRIF_EXT_IRQ_SERVICES_API_ENABLE != STD_OFF)
#error macro FRIF_EXT_IRQ_SERVICES_API_ENABLE has an invalid value
#endif /* FRIF_EXT_IRQ_SERVICES_API_ENABLE */


/* test for valid macro definition of FRIF_GETCLOCKCORRECTION_API_ENABLE */
#ifndef FRIF_GETCLOCKCORRECTION_API_ENABLE
#error FRIF_GETCLOCKCORRECTION_API_ENABLE not defined
#endif  /* FRIF_GETCLOCKCORRECTION_API_ENABLE */

/* test for valid value of FRIF_GETCLOCKCORRECTION_API_ENABLE */
#if (FRIF_GETCLOCKCORRECTION_API_ENABLE != STD_ON) && \
    (FRIF_GETCLOCKCORRECTION_API_ENABLE != STD_OFF)
#error macro FRIF_GETCLOCKCORRECTION_API_ENABLE has an invalid value
#endif /* FRIF_GETCLOCKCORRECTION_API_ENABLE */


/* test for valid macro definition of FRIF_GETSYNCFRAMELIST_API_ENABLE */
#ifndef FRIF_GETSYNCFRAMELIST_API_ENABLE
#error FRIF_GETSYNCFRAMELIST_API_ENABLE not defined
#endif  /* FRIF_GETSYNCFRAMELIST_API_ENABLE */

/* test for valid value of FRIF_GETSYNCFRAMELIST_API_ENABLE */
#if (FRIF_GETSYNCFRAMELIST_API_ENABLE != STD_ON) && \
    (FRIF_GETSYNCFRAMELIST_API_ENABLE != STD_OFF)
#error macro FRIF_GETSYNCFRAMELIST_API_ENABLE has an invalid value
#endif /* FRIF_GETSYNCFRAMELIST_API_ENABLE */


/* test for valid macro definition of FRIF_GETTRANSCEIVERERROR_API_ENABLE */
#ifndef FRIF_GETTRANSCEIVERERROR_API_ENABLE
#error FRIF_GETTRANSCEIVERERROR_API_ENABLE not defined
#endif  /* FRIF_GETTRANSCEIVERERROR_API_ENABLE */

/* test for valid value of FRIF_GETTRANSCEIVERERROR_API_ENABLE */
#if (FRIF_GETTRANSCEIVERERROR_API_ENABLE != STD_ON) && \
    (FRIF_GETTRANSCEIVERERROR_API_ENABLE != STD_OFF)
#error macro FRIF_GETTRANSCEIVERERROR_API_ENABLE has an invalid value
#endif /* FRIF_GETTRANSCEIVERERROR_API_ENABLE */


/* test for valid macro definition of FRIF_GETTRANSCEIVERMODE_API_ENABLE */
#ifndef FRIF_GETTRANSCEIVERMODE_API_ENABLE
#error FRIF_GETTRANSCEIVERMODE_API_ENABLE not defined
#endif  /* FRIF_GETTRANSCEIVERMODE_API_ENABLE */

/* test for valid value of FRIF_GETTRANSCEIVERMODE_API_ENABLE */
#if (FRIF_GETTRANSCEIVERMODE_API_ENABLE != STD_ON) && \
    (FRIF_GETTRANSCEIVERMODE_API_ENABLE != STD_OFF)
#error macro FRIF_GETTRANSCEIVERMODE_API_ENABLE has an invalid value
#endif /* FRIF_GETTRANSCEIVERMODE_API_ENABLE */


/* test for valid macro definition of FRIF_GETTRANSCEIVERWUREASON_API_ENABLE */
#ifndef FRIF_GETTRANSCEIVERWUREASON_API_ENABLE
#error FRIF_GETTRANSCEIVERWUREASON_API_ENABLE not defined
#endif  /* FRIF_GETTRANSCEIVERWUREASON_API_ENABLE */

/* test for valid value of FRIF_GETTRANSCEIVERWUREASON_API_ENABLE */
#if (FRIF_GETTRANSCEIVERWUREASON_API_ENABLE != STD_ON) && \
    (FRIF_GETTRANSCEIVERWUREASON_API_ENABLE != STD_OFF)
#error macro FRIF_GETTRANSCEIVERWUREASON_API_ENABLE has an invalid value
#endif /* FRIF_GETTRANSCEIVERWUREASON_API_ENABLE */


/* test for valid macro definition of FRIF_WAKEUPCONTROL_API_ENABLE */
#ifndef FRIF_WAKEUPCONTROL_API_ENABLE
#error FRIF_WAKEUPCONTROL_API_ENABLE not defined
#endif  /* FRIF_WAKEUPCONTROL_API_ENABLE */

/* test for valid value of FRIF_WAKEUPCONTROL_API_ENABLE */
#if (FRIF_WAKEUPCONTROL_API_ENABLE != STD_ON) && \
    (FRIF_WAKEUPCONTROL_API_ENABLE != STD_OFF)
#error macro FRIF_WAKEUPCONTROL_API_ENABLE has an invalid value
#endif /* FRIF_WAKEUPCONTROL_API_ENABLE */


/* test for valid macro definition of FRIF_CHECKWAKEUPBYTRANSCEIVER_API_ENABLE */
#ifndef FRIF_CHECKWAKEUPBYTRANSCEIVER_API_ENABLE
#error FRIF_CHECKWAKEUPBYTRANSCEIVER_API_ENABLE not defined
#endif  /* FRIF_CHECKWAKEUPBYTRANSCEIVER_API_ENABLE */

/* test for valid value of FRIF_CHECKWAKEUPBYTRANSCEIVER_API_ENABLE */
#if (FRIF_CHECKWAKEUPBYTRANSCEIVER_API_ENABLE != STD_ON) && \
    (FRIF_CHECKWAKEUPBYTRANSCEIVER_API_ENABLE != STD_OFF)
#error macro FRIF_CHECKWAKEUPBYTRANSCEIVER_API_ENABLE has an invalid value
#endif /* FRIF_CHECKWAKEUPBYTRANSCEIVER_API_ENABLE */


/* test for valid macro definition of FRIF_TX_FRAME_DEFAULT_VALUE_ENABLE */
#ifndef FRIF_TX_FRAME_DEFAULT_VALUE_ENABLE
#error FRIF_TX_FRAME_DEFAULT_VALUE_ENABLE not defined
#endif  /* FRIF_TX_FRAME_DEFAULT_VALUE_ENABLE */

/* test for valid value of FRIF_TX_FRAME_DEFAULT_VALUE_ENABLE */
#if (FRIF_TX_FRAME_DEFAULT_VALUE_ENABLE != STD_ON) && \
    (FRIF_TX_FRAME_DEFAULT_VALUE_ENABLE != STD_OFF)
#error macro FRIF_TX_FRAME_DEFAULT_VALUE_ENABLE has an invalid value
#endif /* FRIF_TX_FRAME_DEFAULT_VALUE_ENABLE */

/* test for valid macro definition of FRIF_TX_FRAME_DEFAULT_VALUE */
#ifndef FRIF_TX_FRAME_DEFAULT_VALUE
#error FRIF_TX_FRAME_DEFAULT_VALUE not defined
#endif  /* FRIF_TX_FRAME_DEFAULT_VALUE */


/* test for valid macro definition of FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE */
#ifndef FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE
#error FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE not defined
#endif  /* FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE */

/* test for valid value of FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE */
#if (FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE != STD_ON) && \
    (FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE != STD_OFF)
#error macro FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE has an invalid value
#endif /* FRIF_DYNAMIC_PAYLOAD_LENGTH_ENABLE */

/* test for valid macro definition of FRIF_IMMEDIATE_TX_ENABLE */
#ifndef FRIF_IMMEDIATE_TX_ENABLE
#error FRIF_IMMEDIATE_TX_ENABLE not defined
#endif  /* FRIF_IMMEDIATE_TX_ENABLE */

/* test for valid value of FRIF_IMMEDIATE_TX_ENABLE */
#if (FRIF_IMMEDIATE_TX_ENABLE != STD_ON) && \
    (FRIF_IMMEDIATE_TX_ENABLE != STD_OFF)
#error macro FRIF_IMMEDIATE_TX_ENABLE has an invalid value
#endif /* FRIF_IMMEDIATE_TX_ENABLE */

/* test for valid macro definition of FRIF_TRANSMIT_QUEUE_ENABLE */
#ifndef FRIF_TRANSMIT_QUEUE_ENABLE
#error FRIF_TRANSMIT_QUEUE_ENABLE not defined
#endif  /* FRIF_TRANSMIT_QUEUE_ENABLE */

/* test for valid value of FRIF_TRANSMIT_QUEUE_ENABLE */
#if (FRIF_TRANSMIT_QUEUE_ENABLE != STD_ON) && \
    (FRIF_TRANSMIT_QUEUE_ENABLE != STD_OFF)
#error macro FRIF_TRANSMIT_QUEUE_ENABLE has an invalid value
#endif /* FRIF_TRANSMIT_QUEUE_ENABLE */

/* test for valid macro definition of FRIF_DECOUPLED_RX_ENABLE */
#ifndef FRIF_DECOUPLED_RX_ENABLE
#error FRIF_DECOUPLED_RX_ENABLE not defined
#endif  /* FRIF_DECOUPLED_RX_ENABLE */

/* test for valid value of FRIF_DECOUPLED_RX_ENABLE */
#if (FRIF_DECOUPLED_RX_ENABLE != STD_ON) && \
    (FRIF_DECOUPLED_RX_ENABLE != STD_OFF)
#error macro FRIF_DECOUPLED_RX_ENABLE has an invalid value
#endif /* FRIF_DECOUPLED_RX_ENABLE */

/* test for valid macro definition of FRIF_EXTENDED_RXFIFO_ENABLE */
#ifndef FRIF_EXTENDED_RXFIFO_ENABLE
#error FRIF_EXTENDED_RXFIFO_ENABLE not defined
#endif  /* FRIF_EXTENDED_RXFIFO_ENABLE */

/* test for valid value of FRIF_EXTENDED_RXFIFO_ENABLE */
#if (FRIF_EXTENDED_RXFIFO_ENABLE != STD_ON) && \
    (FRIF_EXTENDED_RXFIFO_ENABLE != STD_OFF)
#error macro FRIF_EXTENDED_RXFIFO_ENABLE has an invalid value
#endif /* FRIF_EXTENDED_RXFIFO_ENABLE */

/* if FR integration is enabled, also single FR must be enabled */
#if (FRIF_FR_INTEGRATION_ENABLE == STD_ON) && (FRIF_SINGLE_FR_OPT_ENABLE != STD_ON)
#error Invalid of pre-compile-time configuration
#endif

/* if a single Ctrl is enabled, also single FR must be enabled */
#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_ON) && (FRIF_SINGLE_FR_OPT_ENABLE != STD_ON)
#error Invalid of pre-compile-time configuration
#endif

/* if a single Ctrl is enabled, also single Clst must be enabled */
#if (FRIF_SINGLE_CTRL_OPT_ENABLE == STD_ON) && (FRIF_SINGLE_CLST_OPT_ENABLE != STD_ON)
#error Invalid of pre-compile-time configuration
#endif

/* if Joblist IRQ multiplexing is enabled, also the get IRQ Status API must be enabled */
#if (FRIF_JOBLIST_IRQMUX_ENABLE == STD_ON) && (FRIF_GETIRQSTATUS_API_ENABLE != STD_ON)
#error Invalid of pre-compile-time configuration
#endif

/* if a single FlexRay Driver optimization is enabled, the name of the Fr must
 * published.
 */
#if (FRIF_SINGLE_FR_OPT_ENABLE == STD_ON) && (!defined FRIF_SINGLE_FR_PREFIX)
#error Invalid of pre-compile-time configuration
#endif

/* if a single FlexRay Transceiver optimization is enabled, the name of the FrTrcv must be
 * published.
 */
#if (FRIF_SINGLE_FRTRCV_OPT_ENABLE == STD_ON) && (!defined FRIF_SINGLE_FRTRCV_PREFIX)
#error Invalid of pre-compile-time configuration
#endif

/*========= macro and pre-compile-time consistency check section end =========*/

#define FRIF_INVALID_ENTRY (0xFFU)
#define FRIF_ARRAY_SIZE(arrayname) (sizeof(arrayname)/sizeof((arrayname)[0]))

/*======================[Global Data Types]===================================*/


/* hose macros contain the communication operation configuration values */
#define FRIF_TRANSMIT_FRAME             (0x00U) /* communication operation 'decoupled transmission' */
#define FRIF_ALWAYS_TRANSMIT_FRAME      (0x01U) /* communication operation 'decoupled transmission' for frames that are always transmitted */
#define FRIF_TRANSMIT_FRAME_NONE_MODE   (0x08U) /* communication operation 'decoupled transmission' for frames that are always transmitted */
#define FRIF_RECEIVE_FRAME              (0x02U) /* communication operation 'receive and store frame' */
#define FRIF_CONFIRM_IMM_FRAME          (0x03U) /* communication operation 'tx confirmation' for immediately transmitted frames */
#define FRIF_CONFIRM_DEC_FRAME          (0x04U) /* communication operation 'tx confirmation' for decoupled transmitted frames */
#define FRIF_INDICATE_FRAME             (0x05U) /* communication operation 'receive indication' */
#define FRIF_RECEIVE_INDICATE_FRAME     (0x06U) /* communication operation 'receive and indicate' */
#define FRIF_PREPARE_FRAME              (0x07U) /* communication operation 'prepare frame' */

#define FRIF_TRANSMIT_SIMPLE_FRAME             (0x09U) /* communication operation 'decoupled transmission' for frames with one pdu */
#define FRIF_ALWAYS_TRANSMIT_SIMPLE_FRAME      (0x0AU) /* communication operation 'decoupled transmission' for always transmitted frames with one pdu */
#define FRIF_TRANSMIT_SIMPLE_FRAME_NONE_MODE   (0x0BU) /* communication operation 'decoupled transmission' for always transmitted frames with one pdu */
#define FRIF_CONFIRM_IMM_SIMPLE_FRAME          FRIF_CONFIRM_IMM_FRAME /* communication operation 'tx confirmation' for immediately transmitted frames with one pdu */
#define FRIF_CONFIRM_DEC_SIMPLE_FRAME          FRIF_CONFIRM_IMM_SIMPLE_FRAME /**< communication operation 'tx confirmation' for decoupled transmitted frames with one pdu */
#define FRIF_RECEIVE_INDICATE_SIMPLE_FRAME     (0x0CU) /* communication operation 'receive and indicate' for frames with one pdu */
#define FRIF_PREPARE_SIMPLE_FRAME              FRIF_PREPARE_FRAME /* communication operation 'prepare frame' for frames with one pdu */
#define FRIF_RECEIVE_INDICATE_RXFIFO           (0x0DU) /* communication operation consumes FIFO and immediately indicates Pdus */

/* include generated types */
#include <FrIf_CfgTypes.h>

/* FlexRay driver (Fr) function pointers */
#if (FRIF_SINGLE_FR_OPT_ENABLE == STD_OFF)
#if (FRIF_FR_INTEGRATION_ENABLE == STD_OFF)

typedef struct
{
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_ControllerInit)(
        uint8 param1 );

#if (FRIF_MTS_API_ENABLE == STD_ON)
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_SendMTS) (
        uint8 param1,
        Fr_ChannelType param2);

    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_StopMTS) (
        uint8 param1,
        Fr_ChannelType param2);

    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_CheckMTS)(
        uint8 param1,
        Fr_ChannelType param2,
        P2VAR(Fr_MTSStatusType, AUTOMATIC, FRIF_APPL_DATA) param3);
#endif /* FRIF_MTS_API_ENABLE */

    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_StartCommunication)(
        uint8 param1);

    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_HaltCommunication)(
        uint8 param1);

    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_AbortCommunication)(
        uint8 param1);

#if (FRIF_WAKEUP_API_ENABLE == STD_ON)
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_SetWakeupChannel)(
        uint8 param1,
        Fr_ChannelType param2);

    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_SendWUP)(
        uint8 param1);
#endif /* FRIF_WAKEUP_API_ENABLE */

#if (FRIF_SETEXTSYNC_API_ENABLE == STD_ON)
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_SetExtSync)(
        uint8 param1,
        Fr_OffsetCorrectionType param2,
        Fr_RateCorrectionType param3);
#endif /* FRIF_SETEXTSYNC_API_ENABLE */

#if (FRIF_GETSYNCSTATE_API_ENABLE == STD_ON)
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_GetSyncState)(
        uint8 param1,
        P2VAR(Fr_SyncStateType,AUTOMATIC,FRIF_APPL_DATA) param2);
#endif

    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_GetPOCStatus)(
        uint8 param1,
        P2VAR(Fr_POCStatusType,AUTOMATIC,FRIF_APPL_DATA) param2);

#if (FRIF_FR_DRIVER_AUTOSAR_VERSION == FRIF_FR_ASR_44_SUPPORT)
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_TransmitTxLPdu)(
        uint8 param1,
        uint16 param2,
        P2CONST(uint8, AUTOMATIC, FRIF_APPL_DATA) param3,
        uint8 param4,
        P2VAR(Fr_SlotAssignmentType, AUTOMATIC, FRIF_APPL_DATA) param5);

    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_ReceiveRxLPdu)(
        uint8 param1,
        uint16 param2,
        P2VAR(uint8, AUTOMATIC, FRIF_APPL_DATA) param3,
        P2VAR(Fr_RxLPduStatusType, AUTOMATIC, FRIF_APPL_DATA) param4,
        P2VAR(uint8, AUTOMATIC, FRIF_APPL_DATA) param5,
        P2VAR(Fr_SlotAssignmentType, AUTOMATIC, FRIF_APPL_DATA) param6);

    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_CheckTxLPduStatus)(
        uint8 param1,
        uint16 param2,
        P2VAR(Fr_TxLPduStatusType, AUTOMATIC, FRIF_APPL_DATA) param3,
        P2VAR(Fr_SlotAssignmentType, AUTOMATIC, FRIF_APPL_DATA) param4);
#else
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_TransmitTxLPdu)(
        uint8 param1,
        uint16 param2,
        P2CONST(uint8, AUTOMATIC, FRIF_APPL_DATA) param3,
        uint8 param4);

    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_ReceiveRxLPdu)(
        uint8 param1,
        uint16 param2,
        P2VAR(uint8, AUTOMATIC, FRIF_APPL_DATA) param3,
        P2VAR(Fr_RxLPduStatusType, AUTOMATIC, FRIF_APPL_DATA) param4,
        P2VAR(uint8, AUTOMATIC, FRIF_APPL_DATA) param5);

    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_CheckTxLPduStatus)(
        uint8 param1,
        uint16 param2,
        P2VAR(Fr_TxLPduStatusType, AUTOMATIC, FRIF_APPL_DATA) param3);
#endif /* FRIF_FR_DRIVER_AUTOSAR_VERSION == FRIF_FR_ASR_44_SUPPORT */

#if (FRIF_JOBLIST_PREPARE_LPDU_ENABLE == STD_ON)
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_PrepareLPdu)(
        uint8 param1,
        uint16 param2);
#endif /* FRIF_JOBLIST_PREPARE_LPDU_ENABLE */

    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_GetGlobalTime)(
        uint8 param1,
        P2VAR(uint8, AUTOMATIC, FRIF_APPL_DATA) param2,
        P2VAR(uint16, AUTOMATIC, FRIF_APPL_DATA) param3);

    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_SetAbsoluteTimer)(
        uint8 param1,
        uint8 param2,
        uint8 param3,
        uint16 param4);

    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_CancelAbsoluteTimer)(
        uint8 param1,
        uint8 param2);

    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_EnableAbsoluteTimerIRQ)(
        uint8 param1,
        uint8 param2);

    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_AckAbsoluteTimerIRQ)(
        uint8 param1,
        uint8 param2);

#if (FRIF_DISABLEIRQ_API_ENABLE == STD_ON)
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_DisableAbsoluteTimerIRQ)(
        uint8 param1,
        uint8 param2);
#endif /* FRIF_DISABLEIRQ_API_ENABLE */

#if (FRIF_GETIRQSTATUS_API_ENABLE == STD_ON)
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_GetAbsoluteTimerIRQStatus)(
        uint8 param1,
        uint8 param2,
        P2VAR(boolean, AUTOMATIC, FRIF_APPL_DATA) param3);
#endif /* FRIF_GETIRQSTATUS_API_ENABLE */

#if (FRIF_RELATIVETIMER_API_ENABLE == STD_ON)
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_SetRelativeTimer)(
        uint8 param1,
        uint8 param2,
        uint16 param3);

    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_CancelRelativeTimer)(
        uint8 param1,
        uint8 param2);

    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_EnableRelativeTimerIRQ)(
        uint8 param1,
        uint8 param2);

    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_AckRelativeTimerIRQ)(
        uint8 param1,
        uint8 param2);

#if (FRIF_DISABLEIRQ_API_ENABLE == STD_ON)
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_DisableRelativeTimerIRQ)(
        uint8 param1,
        uint8 param2);
#endif /* FRIF_DISABLEIRQ_API_ENABLE */

#if (FRIF_GETIRQSTATUS_API_ENABLE == STD_ON)
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, Fr_GetRelativeTimerIRQStatus)(
        uint8 param1,
        uint8 param2,
        P2VAR(boolean, AUTOMATIC, FRIF_APPL_DATA) param3);
#endif /* FRIF_GETIRQSTATUS_API_ENABLE */
#endif /* FRIF_RELATIVETIMER_API_ENABLE */

#if (FRIF_GETNMVECTOR_API_ENABLE == STD_ON)
    P2FUNC(Std_ReturnType,FRIF_APPL_CODE, Fr_GetNmVector)(
        uint8 param1,
        P2VAR(uint8,AUTOMATIC,FRIF_APPL_DATA) param2);
#endif /* FRIF_GETNMVECTOR_API_ENABLE */

#if (FRIF_ALLOWCOLDSTART_API_ENABLE == STD_ON)
    P2FUNC(Std_ReturnType,FRIF_APPL_CODE, Fr_AllowColdstart)(
        uint8 param1);
#endif /* FRIF_ALLOWCOLDSTART_API_ENABLE */

#if (FRIF_GETCHANNELSTATUS_API_ENABLE == STD_ON)
    P2FUNC(Std_ReturnType,FRIF_APPL_CODE, Fr_GetChannelStatus)(
        uint8 param1,
        P2VAR(uint16,AUTOMATIC,FRIF_APPL_DATA) param2,
        P2VAR(uint16,AUTOMATIC,FRIF_APPL_DATA) param3);
#endif /* FRIF_GETCHANNELSTATUS_API_ENABLE */

#if (FRIF_GETCONTROLLERERRORSTATUS_API_ENABLE == STD_ON)
    P2FUNC(Std_ReturnType,FRIF_APPL_CODE, Fr_GetControllerErrorStatus)(
        uint8 param1,
        P2VAR(uint16,AUTOMATIC,FRIF_APPL_DATA) param2);
#endif /* FRIF_GETCONTROLLERERRORSTATUS_API_ENABLE */

#if (FRIF_ALLSLOTS_API_ENABLE == STD_ON)
    P2FUNC(Std_ReturnType,FRIF_APPL_CODE, Fr_AllSlots)(
        uint8 param1);
#endif /* FRIF_ALLSLOTS_API_ENABLE */

#if (FRIF_RECONFIGLPDU_API_ENABLE == STD_ON)
    P2FUNC(Std_ReturnType,FRIF_APPL_CODE, Fr_ReconfigLPdu)(
        uint8 param1,
        uint16 param2,
        uint16 param3,
        Fr_ChannelType param4,
        uint8 param5,
        uint8 param6,
        uint8 param7,
        uint16 param8);
#endif /* FRIF_RECONFIGLPDU_API_ENABLE */

#if (FRIF_DISABLELPDU_API_ENABLE == STD_ON)
    P2FUNC(Std_ReturnType,FRIF_APPL_CODE, Fr_DisableLPdu)(
        uint8 param1,
        uint16 param2);
#endif /* FRIF_DISABLELPDU_API_ENABLE */

#if (FRIF_GETWAKEUPRXSTATUS_API_ENABLE == STD_ON)
    P2FUNC(Std_ReturnType,FRIF_APPL_CODE, Fr_GetWakeupRxStatus)(
        uint8 param1,
        P2VAR(uint8,AUTOMATIC,FRIF_APPL_DATA) param2);
#endif /* FRIF_GETWAKEUPRXSTATUS_API_ENABLE */

#if (FRIF_EXT_IRQ_SERVICES_API_ENABLE == STD_ON)

    P2FUNC(Std_ReturnType,FRIF_APPL_CODE, Fr_AckIRQ)(
        uint8 param1,
        uint8 param2);

    P2FUNC(Std_ReturnType,FRIF_APPL_CODE, Fr_EnableIRQ)(
        uint8 param1,
        uint8 param2);

    P2FUNC(Std_ReturnType,FRIF_APPL_CODE, Fr_DisableIRQ)(
        uint8 param1,
        uint8 param2);

    P2FUNC(Std_ReturnType,FRIF_APPL_CODE, Fr_GetIRQStatus)(
        uint8 param1,
        uint8 param2,
        P2VAR(boolean,AUTOMATIC,FRIF_APPL_DATA) param3);

#endif /* FRIF_EXT_IRQ_SERVICES_API_ENABLE */

#if (FRIF_GETCLOCKCORRECTION_API_ENABLE == STD_ON)

    P2FUNC(Std_ReturnType,FRIF_APPL_CODE, Fr_GetClockCorrection)(
        uint8 param1,
        P2VAR(sint16,AUTOMATIC,FRIF_APPL_DATA) param2,
        P2VAR(sint32,AUTOMATIC,FRIF_APPL_DATA) param3);

#endif /* FRIF_GETCLOCKCORRECTION_API_ENABLE */

#if (FRIF_GETSYNCFRAMELIST_API_ENABLE == STD_ON)

    P2FUNC(Std_ReturnType,FRIF_APPL_CODE, Fr_GetSyncFrameList)(
        uint8 param1,
        uint8 param2,
        P2VAR(uint16,AUTOMATIC,FRIF_APPL_DATA) param3,
        P2VAR(uint16,AUTOMATIC,FRIF_APPL_DATA) param4,
        P2VAR(uint16,AUTOMATIC,FRIF_APPL_DATA) param5,
        P2VAR(uint16,AUTOMATIC,FRIF_APPL_DATA) param6);

#endif /* FRIF_GETSYNCFRAMELIST_API_ENABLE */

#if (FRIF_GETNUMOFSTARTUPFRAMES_API_ENABLE == STD_ON)

    P2FUNC(Std_ReturnType,FRIF_APPL_CODE,Fr_GetNumOfStartupFrames)(
        uint8 param1,
        P2VAR(uint8,AUTOMATIC,FRIF_APPL_DATA) param2);

#endif /* FRIF_GETNUMOFSTARTUPFRAMES_API_ENABLE */

#if (FRIF_READCCCONFIG_API_ENABLE == STD_ON)

    P2FUNC(Std_ReturnType,FRIF_APPL_CODE, Fr_ReadCCConfig)(
        uint8 param1,
        uint8 param2,
        P2VAR(uint32,AUTOMATIC,FRIF_APPL_DATA) param3);

#endif /* FRIF_READCCCONFIG_API_ENABLE */

#if (FRIF_EXTENDED_RXFIFO_ENABLE == STD_ON)
    P2FUNC(Std_ReturnType,FRIF_APPL_CODE, Fr_ReceiveRxFIFO)(
        uint8 param1,
        uint8 param2,
        P2VAR(uint16,AUTOMATIC,FRIF_APPL_DATA) param3,
        P2VAR(Fr_ChannelType,AUTOMATIC,FRIF_APPL_DATA) param4,
        P2VAR(uint8,AUTOMATIC,FRIF_APPL_DATA) param5,
        P2VAR(uint8, AUTOMATIC, FRIF_APPL_DATA) param6,
        P2VAR(Fr_RxLPduStatusType, AUTOMATIC, FRIF_APPL_DATA) param7,
        P2VAR(uint8, AUTOMATIC, FRIF_APPL_DATA) param8);

    P2FUNC(Std_ReturnType,FRIF_APPL_CODE, Fr_FlushRxFIFO)(
        uint8 param1,
        uint8 param2);
#endif /* FRIF_EXTENDED_RXFIFO_ENABLE */

}FrIf_FrFunctionType;

#endif /* FRIF_FR_INTEGRATION_ENABLE */
#endif /* FRIF_SINGLE_FR_OPT_ENABLE */



/* FlexRay transceiver driver (FrTrcv) function pointers */
#if (FRIF_SINGLE_FRTRCV_OPT_ENABLE == STD_OFF)
typedef struct
{
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, FrTrcv_SetTransceiverMode)(
        uint8 param1,
        FrTrcv_TrcvModeType param2);

#if (FRIF_GETTRANSCEIVERMODE_API_ENABLE == STD_ON)
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, FrTrcv_GetTransceiverMode)(
        uint8 param1,
        P2VAR(FrTrcv_TrcvModeType, AUTOMATIC, FRIF_APPL_DATA) param2);
#endif /* FRIF_GETTRANSCEIVERMODE_API_ENABLE */

#if (FRIF_GETTRANSCEIVERWUREASON_API_ENABLE == STD_ON)
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, FrTrcv_GetTransceiverWUReason)(
        uint8 param1,
        P2VAR(FrTrcv_TrcvWUReasonType, AUTOMATIC, FRIF_APPL_DATA) param2);
#endif /* FRIF_GETTRANSCEIVERWUREASON_API_ENABLE */

#if (FRIF_WAKEUPCONTROL_API_ENABLE == STD_ON)
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE,  FrTrcv_ClearTransceiverWakeup)(
        uint8 param1);
#endif /* FRIF_WAKEUPCONTROL_API_ENABLE */

#if (FRIF_CHECKWAKEUPBYTRANSCEIVER_API_ENABLE == STD_ON)
    P2FUNC( void, FRIF_APPL_CODE,  FrTrcv_CheckWakeupByTransceiver)(
        uint8 param1);
#endif /* FRIF_CHECKWAKEUPBYTRANSCEIVER_API_ENABLE */

#if (FRIF_GETTRANSCEIVERERROR_API_ENABLE == STD_ON)
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE,  FrTrcv_GetTransceiverError)(
        uint8 param1,
        uint8 param2,
        P2VAR(uint32,AUTOMATIC,FRIF_APPL_DATA) param3);
#endif /* FRIF_GETTRANSCEIVERERROR_API_ENABLE */

#if (FRIF_DISABLETRANSCEIVERBRANCH_API_ENABLE == STD_ON)
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, FrTrcv_DisableTransceiverBranch)(
        uint8 param1,
        uint8 param2);
#endif /* FRIF_DISABLETRANSCEIVERBRANCH_API_ENABLE */

#if (FRIF_ENABLETRANSCEIVERBRANCH_API_ENABLE == STD_ON )
    P2FUNC(Std_ReturnType,FRIF_APPL_CODE, FrTrcv_EnableTransceiverBranch)(
        uint8 param1,
        uint8 param2);
#endif /* FRIF_ENABLETRANSCEIVERBRANCH_API_ENABLE */

}FrIf_TrcvFunctionType;

#endif /* FRIF_SINGLE_FRTRCV_OPT_ENABLE */

/*
 * FrIf_PduOwnerFunctionASR40Type
 *
 * Function pointer to upper layers have to be specified in
 * FrIf_PduOwnerFunctionASR40Type array.
 */
typedef struct
{
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, FrIfTriggerTransmit)(
        PduIdType param1,
        P2VAR(PduInfoType, AUTOMATIC, FRIF_APPL_DATA) param2);

    P2FUNC( void, FRIF_APPL_CODE, FrIfTxConfirmation)(
        PduIdType param1);

    P2FUNC( void, FRIF_APPL_CODE, FrIfRxIndication)(
        PduIdType param1,
        P2VAR(PduInfoType, AUTOMATIC, FRIF_APPL_DATA) param2);

}FrIf_PduOwnerFunctionASR40Type;

/*
 * FrIf_PduOwnerFunctionASR43Type
 *
 * Function pointer to upper layers have to be specified in
 * FrIf_PduOwnerFunctionASR43Type array.
 */
typedef struct
{
    P2FUNC( Std_ReturnType, FRIF_APPL_CODE, FrIfTriggerTransmit)(
        PduIdType param1,
        P2VAR(PduInfoType, AUTOMATIC, FRIF_APPL_DATA) param2);

    P2FUNC( void, FRIF_APPL_CODE, FrIfTxConfirmation)(
        PduIdType param1,
        Std_ReturnType param2);

    P2FUNC( void, FRIF_APPL_CODE, FrIfRxIndication)(
        PduIdType param1,
        P2VAR(PduInfoType, AUTOMATIC, FRIF_APPL_DATA) param2);

}FrIf_PduOwnerFunctionASR43Type;

typedef struct
{
    uint16 NextJob;
    uint8 JobListCycleBase;
    uint8 JobListTimeoutCounter;
    uint8 FrIf_SyncState;
    uint8 FrIf_State;
#if (FRIF_BUSMIRRORINGSUPPORT_ENABLE == STD_ON)
    boolean BusmirroringEnabled;
#endif /* FRIF_BUSMIRRORINGSUPPORT_ENABLE */
}FrIf_ClstRuntimeDataType;

/*======================[Global Data]=========================================*/

#define FRIF_START_SEC_CONST_UNSPECIFIED
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */

#if (FRIF_SINGLE_FRTRCV_OPT_ENABLE == STD_OFF)

extern CONST(FrIf_TrcvFunctionType, FRIF_CONST) FrIf_TrcvFuncPtr[];

#endif /* FRIF_SINGLE_FRTRCV_OPT_ENABLE */


#if (FRIF_SINGLE_FR_OPT_ENABLE == STD_OFF)

extern CONST(FrIf_FrFunctionType, FRIF_CONST) FrIf_FrFuncPtr[];

#endif /* FRIF_SINGLE_FR_OPT_ENABLE */

extern CONST(FrIf_PduOwnerFunctionASR40Type, FRIF_CONST) FrIf_PduOwnerFuncASR40Ptr[];
extern CONST(FrIf_PduOwnerFunctionASR43Type, FRIF_CONST) FrIf_PduOwnerFuncASR43Ptr[];

#define FRIF_STOP_SEC_CONST_UNSPECIFIED
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */

/* start data section declaration */
#define FRIF_START_SEC_CONST_32
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */

/* value used to validate post build configuration against link time configuration */
extern CONST(uint32, FRIF_CONST) FrIf_LcfgSignature;

/* stop data section declaration */
#define FRIF_STOP_SEC_CONST_32
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */

/* start data section declaration */
#define FRIF_START_SEC_CONST_8
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */

#if FRIF_UL_ASR43_API_ENABLE == STD_ON
/* value used to validate post build configuration against link time configuration */
extern CONST(uint8, FRIF_CONST) FrIf_FirstASR43OwnerIdx;
#endif /* FRIF_UL_ASR43_API_ENABLE == STD_ON */

/* stop data section declaration */
#define FRIF_STOP_SEC_CONST_8
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */

#define FRIF_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */

extern VAR(FrIf_ClstRuntimeDataType, FRIF_VAR_CLEARED) FrIf_ClstRuntimeData[];

extern VAR(TS_MaxAlignedType , FRIF_VAR_CLEARED) FrIf_Mem[];

#define FRIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */

/*======================[Global Function Declarations]========================*/

/* start code section declaration */
#define FRIF_START_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */

/**
 * \brief  Cluster independent FrIf_MainFunction_# implementation
 *
 * This service performs the joblist (re-)synchronization continously monitors the correct
 * execution of the joblist.
 *
 * If this service detects that FrIf_JobListeExec is not properly executed, it calls DEM and
 * reports FRIF_E_JLE_SYNC with status DEM_EVENT_STATUS_FAILED.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET.
 *
 * \param[in] FrIf_ClstIdx            FlexRay cluster index.
 *
 ** \ProductionError{FRIF_E_ACS_CH_A, thrown\, if any of the error bits in ACS of channel A is set}
 ** \ProductionError{FRIF_E_ACS_CH_B, thrown\, if any of the error bits in ACS of channel B is set}
 ** \ProductionError{FRIF_E_NIT_CH_A, thrown\, if any of the error bits in NIT of channel A is set}
 ** \ProductionError{FRIF_E_NIT_CH_B, thrown\, if any of the error bits in NIT of channel B is set}
 ** \ProductionError{FRIF_E_SW_CH_A, thrown\, if any of the error bits in SW of channel A is set}
 ** \ProductionError{FRIF_E_SW_CH_B, thrown\, if any of the error bits in SW of channel B is set}
 ** \ProductionError{FRIF_E_JLE_SYNC, thrown\, if Job List Execution lost synchronization to the FlexRay Global Time}
 *
 * \ServiceID{0x27}
 * \Reentrancy{Non Re-entrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void,FRIF_CODE) FrIf_MainFunction
    (
        uint8 FrIf_ClstIdx
    );

/**
 * \brief  Cluster independent FrIf_JobListExec_# implementation
 *
 * This service executes the joblist and the contained communication operations.
 *
 * If this service detects that it is not properly executed (in time), it calls DEM and
 * reports FRIF_E_JLE_SYNC with status DEM_EVENT_STATUS_FAILED.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET.
 *
 * \param[in] FrIf_ClstIdx            FlexRay cluster index.
 *
 * \ServiceID{0x32}
 * \Reentrancy{Non Re-entrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void,FRIF_CODE) FrIf_JobListExec
    (
        uint8 FrIf_ClstIdx
    );

#if (FRIF_MTS_API_ENABLE == STD_ON)
/**
 * \brief  Initiates transmission of a MTS symbol.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx        FlexRay controller index.
 * \param[in] FrIf_ChnlIdx        Channel the MTS should be transmitted on.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x90}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_SendMTS
    (
        uint8 FrIf_CtrlIdx,
        Fr_ChannelType FrIf_ChnlIdx
    );
#endif /* (FRIF_MTS_API_ENABLE == STD_ON) */

#if (FRIF_MTS_API_ENABLE == STD_ON)
/**
 * \brief  Stops the transmission of a MTS symbol.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx        FlexRay controller index.
 * \param[in] FrIf_ChnlIdx        Channel the MTS transmission shall be stopped on.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x91}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_StopMTS
    (
        uint8 FrIf_CtrlIdx,
        Fr_ChannelType FrIf_ChnlIdx
    );
#endif /* (FRIF_MTS_API_ENABLE == STD_ON) */

#if (FRIF_MTS_API_ENABLE == STD_ON)
/**
 * \brief  Reads the MTS receive status information.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx        FlexRay controller index.
 * \param[in] FrIf_ChnlIdx        Channel the MTS status shall read from.
 * \param[out] FrIf_MTSStatusPtr  Address the MTS status is written to.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x92}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 *
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_CheckMTS
    (
        uint8 FrIf_CtrlIdx,
        Fr_ChannelType FrIf_ChnlIdx,
        P2VAR(Fr_MTSStatusType,AUTOMATIC,FRIF_APPL_DATA) FrIf_MTSStatusPtr
    );
#endif /* (FRIF_MTS_API_ENABLE == STD_ON) */

#if (FRIF_GETCONTROLLERERRORSTATUS_API_ENABLE == STD_ON)
/**
 * \brief   Returns a FlexRay controller error status.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx        FlexRay controller index.
 * \param[out] FrIf_ControllerErrorStatusPtr  Address to write the error status to.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x83}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_GetControllerErrorStatus
(
    uint8 FrIf_CtrlIdx,
    P2VAR(uint16,AUTOMATIC,FRIF_APPL_DATA) FrIf_ControllerErrorStatusPtr
);
#endif /* (FRIF_GETCONTROLLERERRORSTATUS_API_ENABLE == STD_ON) */

#if (FRIF_EXT_IRQ_SERVICES_API_ENABLE == STD_ON)
/**
 * \brief   Acknowledges an interrupt of the FlexRay controller.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx            FlexRay controller index.
 * \param[in] FrIf_IRQIdx             Interrupt source index.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x86}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_AckIRQ
(
    uint8 FrIf_CtrlIdx,
    uint8 FrIf_IRQIdx
);
#endif /* (FRIF_EXT_IRQ_SERVICES_API_ENABLE == STD_ON) */

#if (FRIF_EXT_IRQ_SERVICES_API_ENABLE == STD_ON)
/**
 * \brief   Enables an interrupt of the FlexRay controller.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx            FlexRay controller index.
 * \param[in] FrIf_IRQIdx             Interrupt source index.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x87}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_EnableIRQ
(
    uint8 FrIf_CtrlIdx,
    uint8 FrIf_IRQIdx
);
#endif /* (FRIF_EXT_IRQ_SERVICES_API_ENABLE == STD_ON) */

#if (FRIF_EXT_IRQ_SERVICES_API_ENABLE == STD_ON)
/**
 * \brief   Disables an interrupt of the FlexRay controller.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx            FlexRay controller index.
 * \param[in] FrIf_IRQIdx             Interrupt source index.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x88}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_DisableIRQ
(
    uint8 FrIf_CtrlIdx,
    uint8 FrIf_IRQIdx
);
#endif /* (FRIF_EXT_IRQ_SERVICES_API_ENABLE == STD_ON) */

#if (FRIF_EXT_IRQ_SERVICES_API_ENABLE == STD_ON)
/**
 * \brief   Returns the interrupt status of the FlexRay controller.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx            FlexRay controller index.
 * \param[in] FrIf_IRQIdx             Interrupt source index.
 * \param[out] FrIf_IRQStatusPtr      Address to write the IRQ interrupt status.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x89}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_GetIRQStatus
(
    uint8 FrIf_CtrlIdx,
    uint8 FrIf_IRQIdx,
    P2VAR(boolean,AUTOMATIC,FRIF_APPL_DATA) FrIf_IRQStatusPtr
);
#endif /* (FRIF_EXT_IRQ_SERVICES_API_ENABLE == STD_ON) */

#if (FRIF_GETSYNCSTATE_API_ENABLE == STD_ON)
/**
 * \brief   Reads the FlexRay CC synchronization state.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx            FlexRay controller index.
 * \param[out] FrIf_SyncStatePtr      Address the synchronization state is written to.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x99}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_GetSyncState
    (
        uint8 FrIf_CtrlIdx,
        P2VAR(Fr_SyncStateType,AUTOMATIC,FRIF_APPL_DATA) FrIf_SyncStatePtr
    );
#endif /* (FRIF_GETSYNCSTATE_API_ENABLE == STD_ON) */

#if (FRIF_SETEXTSYNC_API_ENABLE == STD_ON)
/**
 * \brief   Initiates external clock synchronization.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx            FlexRay controller index.
 * \param[in] FrIf_Offset             Offset correction application mode.
 * \param[in] FrIf_Rate               Rate correction application mode.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x9A}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_SetExtSync
    (
        uint8 FrIf_CtrlIdx,
        Fr_OffsetCorrectionType FrIf_Offset,
        Fr_RateCorrectionType FrIf_Rate
    );
#endif /* (FRIF_SETEXTSYNC_API_ENABLE == STD_ON) */

#if (FRIF_RELATIVETIMER_API_ENABLE == STD_ON)
/**
 * \brief   Sets a relative timer.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx           FlexRay controller index.
 * \param[in] FrIf_RelTimerIdx       Relative timer index.
 * \param[in] FrIf_Offset            Macrotick offset the alarm should elapse.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x93}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_SetRelativeTimer
    (
        uint8 FrIf_CtrlIdx,
        uint8 FrIf_RelTimerIdx,
        uint16 FrIf_Offset
    );
#endif /* (FRIF_RELATIVETIMER_API_ENABLE == STD_ON) */

#if (FRIF_RELATIVETIMER_API_ENABLE == STD_ON)
/**
 * \brief   Cancels the relative timer.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx           FlexRay controller index.
 * \param[in] FrIf_RelTimerIdx       Relative timer index.
 *
 * \note    This function wraps the Fr-API function Fr_CancelRelativeTimer().
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x94}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_CancelRelativeTimer
    (
        uint8 FrIf_CtrlIdx,
        uint8 FrIf_RelTimerIdx
    );
#endif /* (FRIF_RELATIVETIMER_API_ENABLE == STD_ON) */

#if (FRIF_RELATIVETIMER_API_ENABLE == STD_ON)
/**
 * \brief   Enables the relative timer IRQ.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx           FlexRay controller index.
 * \param[in] FrIf_RelTimerIdx       Relative timer index.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x95}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_EnableRelativeTimerIRQ
    (
        uint8 FrIf_CtrlIdx,
        uint8 FrIf_RelTimerIdx
    );
#endif /* (FRIF_RELATIVETIMER_API_ENABLE == STD_ON) */

#if (FRIF_GETIRQSTATUS_API_ENABLE == STD_ON)
#if (FRIF_RELATIVETIMER_API_ENABLE == STD_ON)
/**
 * \brief   Reads the relative timer IRQ status.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx        FlexRay controller index.
 * \param[in] FrIf_RelTimerIdx       Relative timer index.
 * \param[out] FrIf_IRQStatusPtr    Address the timer IRQ status is stored to
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x96}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_GetRelativeTimerIRQStatus
    (
        uint8 FrIf_CtrlIdx,
        uint8 FrIf_RelTimerIdx,
        P2VAR(boolean,AUTOMATIC,FRIF_APPL_DATA) FrIf_IRQStatusPtr
    );
#endif /* (FRIF_RELATIVETIMER_API_ENABLE == STD_ON) */
#endif /* (FRIF_GETIRQSTATUS_API_ENABLE == STD_ON) */

#if (FRIF_RELATIVETIMER_API_ENABLE == STD_ON)
/**
 * \brief   Acknowledges the relative timer IRQ.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx           FlexRay controller index.
 * \param[in] FrIf_RelTimerIdx       Relative timer index.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x97}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_AckRelativeTimerIRQ
    (
        uint8 FrIf_CtrlIdx,
        uint8 FrIf_RelTimerIdx
    );
#endif /* (FRIF_RELATIVETIMER_API_ENABLE == STD_ON) */

#if (FRIF_DISABLEIRQ_API_ENABLE == STD_ON)
#if (FRIF_RELATIVETIMER_API_ENABLE == STD_ON)
/**
 * \brief   Disables the relative timer IRQ.
 *
 * This service translates the FrIf_CtrlIdx to the configured FlexRay driver and FlexRay driver
 * controller index and calls the equivalent driver service.
 *
 * If DET is enabled and FrIf_Init() was not called before this service
 * FRIF_E_NOT_INITIALIZED is reported to DET and E_NOT_OK returned.
 *
 * If DET is enabled and FrIf_CtrlIdx contains an invalid value
 * FRIF_E_INV_CTRL_IDX is reported to DET and E_NOT_OK returned.
 *
 * \param[in] FrIf_CtrlIdx           FlexRay controller index.
 * \param[in] FrIf_RelTimerIdx       Relative timer index.
 *
 * \return     E_OK: Service execution was successful.
 *         E_NOT_OK: Service execution failed.
 *
 * \ServiceID{0x98}
 * \Reentrancy{Re-entrant for different values of FrIf_CtrlIdx only}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType,FRIF_CODE) FrIf_DisableRelativeTimerIRQ
    (
        uint8 FrIf_CtrlIdx,
        uint8 FrIf_RelTimerIdx
    );
#endif /* (FRIF_RELATIVETIMER_API_ENABLE == STD_ON) */
#endif /* (FRIF_DISABLEIRQ_API_ENABLE == STD_ON) */



/* stop code section declaration */
#define FRIF_STOP_SEC_CODE
#include <FrIf_MemMap.h>  /* !LINKSTO FrIf.ASR40.FrIf05088,1 */


#undef TS_PB_CFG_PTR_CLASS

#undef TS_RELOCATABLE_CFG_ENABLE

#endif /* FRIF_EXTENSION_H */

