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
#ifndef COM_RULES_H_
#define COM_RULES_H_

#include <TSAutosar.h>          /* EB specific standard types */
#include <Com_Defs.h>           /* Definitions of macros */
#include <Com_Cfg.h>
#include <ComStack_Types.h>     /* AUTOSAR Standard types */
#include <Com_Local_Cfg.h>


/*==================[macros]================================================*/

#if (defined COM_TX_MODE_NOT_NONE_OR_PERIODIC_AVAILABLE)        /* To prevent double declaration */
#error COM_TX_MODE_NOT_NONE_OR_PERIODIC_AVAILABLE is already defined
#endif /* (defined COM_TX_MODE_NOT_NONE_OR_PERIODIC_AVAILABLE) */

#if ((COM_TX_MODE_N_TIMES_ENABLE == STD_ON) || \
      (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON) || \
      (COM_TX_MODE_DIRECT_ENABLE == STD_ON) ||\
      (COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON))
#define COM_TX_MODE_NOT_NONE_OR_PERIODIC_AVAILABLE STD_ON
#else
#define COM_TX_MODE_NOT_NONE_OR_PERIODIC_AVAILABLE STD_OFF
#endif



#if (defined COM_RX_SIGNAL_BASED_DM)     /* To prevent double declaration */
#error COM_RX_SIGNAL_BASED_DM already defined
#endif /* #if (defined COM_RX_SIGNAL_BASED_DM) */

/** \brief definition COM_RX_SIGNAL_BASED_DM */
#if ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) && \
      (COM_RX_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT))
#define COM_RX_SIGNAL_BASED_DM STD_ON
#else
#define COM_RX_SIGNAL_BASED_DM STD_OFF
#endif


#if (defined COM_FILTER_AVAILABLE)      /* To prevent double declaration */
#error COM_FILTER_AVAILABLE already defined
#endif /* #if (defined COM_FILTER_AVAILABLE) */

/** \brief definition COM_FILTER_AVAILABLE */
#if ((COM_TMS_ENABLE == STD_ON) || \
    (COM_FILTER_RECEIVER_ENABLE == STD_ON))
#define COM_FILTER_AVAILABLE STD_ON
#else
#define COM_FILTER_AVAILABLE STD_OFF
#endif /* ((COM_TMS_ENABLE == STD_ON) || \
          (COM_FILTER_RECEIVER_ENABLE == STD_ON)) */


#if (defined COM_MDT_QUEUE_AVAILABLE)   /* To prevent double declaration */
#error COM_MDT_QUEUE_AVAILABLE already defined
#endif /* #if (defined COM_MDT_QUEUE_AVAILABLE) */

/** \brief definition COM_MDT_QUEUE_AVAILABLE */
#if (COM_TX_IPDU_MINIMUM_DELAY_TIME_FACTOR_SIZE != COM_SIZE_0_BIT)
#define COM_MDT_QUEUE_AVAILABLE STD_ON
#else
#define COM_MDT_QUEUE_AVAILABLE STD_OFF
#endif


#if (defined COM_TX_TIMEOUT_FACTOR_AVAILABLE)   /* To prevent double declaration */
#error COM_TX_TIMEOUT_FACTOR_AVAILABLE already defined
#endif /* (defined COM_TX_TIMEOUT_FACTOR_AVAILABLE) */

/** \brief definition COM_TX_TIMEOUT_FACTOR_AVAILABLE */
#if (COM_TX_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT)
#define COM_TX_TIMEOUT_FACTOR_AVAILABLE STD_ON
#else
#define COM_TX_TIMEOUT_FACTOR_AVAILABLE STD_OFF
#endif


#if (defined COM_RX_TIMEOUT_FACTOR_AVAILABLE)   /* To prevent double declaration */
#error COM_RX_TIMEOUT_FACTOR_AVAILABLE already defined
#endif /* (defined COM_RX_TIMEOUT_FACTOR_AVAILABLE) */

/** \brief definition COM_RX_TIMEOUT_FACTOR_AVAILABLE */
#if (COM_RX_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT)
#define COM_RX_TIMEOUT_FACTOR_AVAILABLE STD_ON
#else
#define COM_RX_TIMEOUT_FACTOR_AVAILABLE STD_OFF
#endif


#if (defined COM_FILTER_ONE_EVERY_N_AVAILABLE)  /* To prevent double declaration */
#error COM_FILTER_ONE_EVERY_N_AVAILABLE already defined
#endif /* (defined COM_FILTER_ONE_EVERY_N_AVAILABLE) */

/** \brief definition of the COM_FILTER_ONE_EVERY_N_AVAILABLE */
#if ((COM_FILTER_ONE_EVERY_N_PERIOD_OFFSET_SIZE_MAX != COM_SIZE_0_BIT) && \
     (COM_FILTER_ONE_EVERY_N_OCCURANCE_MAX != COM_SIZE_0_BIT))
#define COM_FILTER_ONE_EVERY_N_AVAILABLE COM_FILTER_AVAILABLE
#else
#define COM_FILTER_ONE_EVERY_N_AVAILABLE STD_OFF
#endif /* ((COM_FILTER_ONE_EVERY_N_PERIOD_OFFSET_SIZE_MAX != COM_SIZE_0_BIT) && \
          (COM_FILTER_ONE_EVERY_N_OCCURANCE_MAX != COM_SIZE_0_BIT)) */


#if (defined COM_TM_AVAILABLE)  /* To prevent double declaration */
#error COM_TM_AVAILABLE already defined
#endif /* (defined COM_TM_AVAILABLE) */


/** \brief definition of the COM_TM_AVAILABLE */
#if ((COM_TX_MODE_DIRECT_ENABLE == STD_ON) || \
    (COM_TX_MODE_N_TIMES_ENABLE == STD_ON) || \
    (COM_TX_MODE_PERIODIC_ENABLE == STD_ON) || \
    (COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON) || \
    (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON))
#define COM_TM_AVAILABLE STD_ON
#else
#define COM_TM_AVAILABLE STD_OFF
#endif


#if (defined COM_PERIODIC_QUEUE_AVAILABLE)      /* To prevent double declaration */
#error COM_PERIODIC_QUEUE_AVAILABLE already defined
#endif /* (defined COM_PERIODIC_QUEUE_AVAILABLE) */

/** \brief definition of the COM_PERIODIC_QUEUE_AVAILABLE */
#if ((COM_TX_MODE_PERIODIC_ENABLE == STD_ON) || \
    (COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON) || \
    (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON))
#define COM_PERIODIC_QUEUE_AVAILABLE STD_ON
#else
#define COM_PERIODIC_QUEUE_AVAILABLE STD_OFF
#endif


#if (defined COM_N_TIMES_QUEUE_AVAILABLE)       /* To prevent double declaration */
#error COM_N_TIMES_QUEUE_AVAILABLE already defined
#endif /* (defined COM_N_TIMES_QUEUE_AVAILABLE) */

/** \brief definition of the COM_N_TIMES_QUEUE_AVAILABLE */
#if ((COM_TX_MODE_N_TIMES_ENABLE == STD_ON) || \
    (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON))
#define COM_N_TIMES_QUEUE_AVAILABLE STD_ON
#else
#define COM_N_TIMES_QUEUE_AVAILABLE STD_OFF
#endif

#if (defined COM_TM_SWITCH_AVAILABLE)        /* To prevent double declaration */
#error COM_TM_SWITCH_AVAILABLE is already defined
#endif /* (defined COM_TM_SWITCH_AVAILABLE) */

#if (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON)
#define COM_TM_SWITCH_AVAILABLE STD_ON
#elif (COM_N_TIMES_QUEUE_AVAILABLE == STD_ON)
#define COM_TM_SWITCH_AVAILABLE STD_ON
#elif (COM_TX_MODE_N_TIMES_ENABLE == STD_ON)
#define COM_TM_SWITCH_AVAILABLE STD_ON
#elif ((COM_TX_MODE_MIXED_DIRECT_ENABLE == STD_ON) || (COM_TX_MODE_PERIODIC_ENABLE == STD_ON))
#define COM_TM_SWITCH_AVAILABLE STD_ON
#elif (COM_TX_MODE_MIXED_N_TIMES_ENABLE == STD_ON)
#define COM_TM_SWITCH_AVAILABLE STD_ON
#else
#define COM_TM_SWITCH_AVAILABLE STD_OFF
#endif




#if (defined COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE)    /* To prevent double declaration */
#error COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE already defined
#endif /* (defined COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE) */

/** \brief definition of COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE */
#if ((COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) && \
     (COM_TX_SIG_CONF_DEFERRED_ENABLE == STD_ON))
#define COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE STD_ON
#else
#define COM_TX_CONFIRMATIONS_DEFERED_AVAILABLE STD_OFF
#endif /* ((COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) && \
          (COM_TX_SIG_CONF_DEFERRED_ENABLE == STD_ON)) */


#if (defined COM_TX_CONFIRMATIONS_IMMEDIATE_AVAILABLE)  /* To prevent double declaration */
#error COM_TX_CONFIRMATIONS_IMMEDIATE_AVAILABLE already defined
#endif /* (defined COM_TX_CONFIRMATIONS_IMMEDIATE_AVAILABLE) */

/** \brief definition of COM_TX_CONFIRMATIONS_IMMEDIATE_AVAILABLE */
#if ((COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) && \
     (COM_TX_SIG_CONF_IMMEDIATE_ENABLE == STD_ON))
#define COM_TX_CONFIRMATIONS_IMMEDIATE_AVAILABLE STD_ON
#else
#define COM_TX_CONFIRMATIONS_IMMEDIATE_AVAILABLE STD_OFF
#endif /* ((COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) && \
          (COM_TX_SIG_CONF_IMMEDIATE_ENABLE == STD_ON)) */


#if (defined COM_RX_CONF_DEFERED_AVAILABLE)    /* To prevent double declaration */
#error COM_RX_CONF_DEFERED_AVAILABLE already defined
#endif /* (defined COM_RX_CONF_DEFERED_AVAILABLE) */

/** \brief definition of COM_RX_CONF_DEFERED_AVAILABLE */
#if ((COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) && \
     (COM_RX_SIG_CONF_DEFERRED_ENABLE == STD_ON))
#define COM_RX_CONF_DEFERED_AVAILABLE STD_ON
#else
#define COM_RX_CONF_DEFERED_AVAILABLE STD_OFF
#endif /* ((COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) && \
          (COM_RX_SIG_CONF_DEFERRED_ENABLE == STD_ON)) */


#if (defined COM_RX_CONF_IMMEDIATE_AVAILABLE)  /* To prevent double declaration */
#error COM_RX_CONF_IMMEDIATE_AVAILABLE already defined
#endif /* (defined COM_RX_CONF_IMMEDIATE_AVAILABLE) */

/** \brief definition of COM_RX_CONF_IMMEDIATE_AVAILABLE */
#if ((COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) && \
     (COM_RX_SIG_CONF_IMMEDIATE_ENABLE == STD_ON))
#define COM_RX_CONF_IMMEDIATE_AVAILABLE STD_ON
#else
#define COM_RX_CONF_IMMEDIATE_AVAILABLE STD_OFF
#endif /* ((COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) && \
          (COM_RX_SIG_CONF_IMMEDIATE_ENABLE == STD_ON)) */

#if (defined COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_OPT_SIZE)        /* To prevent double declaration */
#error COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_OPT_SIZE is already defined
#endif /* (defined COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_OPT_SIZE) */

#define COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_OPT_SIZE 2U

#if (defined COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_OPT_OFFSET)        /* To prevent double declaration */
#error COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_OPT_OFFSET is already defined
#endif /* (defined COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_OPT_OFFSET) */

#define COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_OPT_OFFSET 0U

#if (defined COM_RX_SIGNAL_FLAG_ENDIANNESS_SIZE)        /* To prevent double declaration */
#error COM_RX_SIGNAL_FLAG_ENDIANNESS_SIZE is already defined
#endif /* (defined COM_RX_SIGNAL_FLAG_ENDIANNESS_SIZE) */

#define COM_RX_SIGNAL_FLAG_ENDIANNESS_SIZE 1U

#if (defined COM_RX_SIGNAL_FLAG_ENDIANNESS_OFFSET)        /* To prevent double declaration */
#error COM_RX_SIGNAL_FLAG_ENDIANNESS_OFFSET is already defined
#endif /* (defined COM_RX_SIGNAL_FLAG_ENDIANNESS_OFFSET) */

#define COM_RX_SIGNAL_FLAG_ENDIANNESS_OFFSET (COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_OPT_SIZE + COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_OPT_OFFSET)

#if (defined COM_RX_SIGNAL_FLAG_BUFFER_LOCK_SIZE)        /* To prevent double declaration */
#error COM_RX_SIGNAL_FLAG_BUFFER_LOCK_SIZE is already defined
#endif /* (defined COM_RX_SIGNAL_FLAG_ENDIANNESS_OFFSET) */

#define COM_RX_SIGNAL_FLAG_BUFFER_LOCK_SIZE 1U

#if (defined COM_RX_SIGNAL_FLAG_BUFFER_LOCK_OFFSET)        /* To prevent double declaration */
#error COM_RX_SIGNAL_FLAG_BUFFER_LOCK_OFFSET is already defined
#endif /* (defined COM_RX_SIGNAL_FLAG_BUFFER_LOCK_OFFSET) */

#define COM_RX_SIGNAL_FLAG_BUFFER_LOCK_OFFSET (COM_RX_SIGNAL_FLAG_ENDIANNESS_SIZE + COM_RX_SIGNAL_FLAG_ENDIANNESS_OFFSET)

#if (defined COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_SIZE)        /* To prevent double declaration */
#error COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_SIZE is already defined
#endif /* (defined COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_SIZE) */

#define COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_SIZE 1U

#if (defined COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_OFFSET)        /* To prevent double declaration */
#error COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_OFFSET is already defined
#endif /* (defined COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_OFFSET) */

#define COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_OFFSET (COM_RX_SIGNAL_FLAG_BUFFER_LOCK_SIZE + COM_RX_SIGNAL_FLAG_BUFFER_LOCK_OFFSET)

#if (defined COM_RX_SIGNAL_FLAG_OPT_EXT_SIZE)        /* To prevent double declaration */
#error COM_RX_SIGNAL_FLAG_OPT_EXT_SIZE is already defined
#endif /* (defined COM_RX_SIGNAL_FLAG_OPT_EXT_SIZE) */

#define COM_RX_SIGNAL_FLAG_OPT_EXT_SIZE 1U

#if (defined COM_RX_SIGNAL_FLAG_OPT_EXT_OFFSET)        /* To prevent double declaration */
#error COM_RX_SIGNAL_FLAG_OPT_EXT_OFFSET is already defined
#endif /* (defined COM_RX_SIGNAL_FLAG_OPT_EXT_OFFSET) */

#define COM_RX_SIGNAL_FLAG_OPT_EXT_OFFSET (COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_SIZE + COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_OFFSET)

#if (defined COM_RX_SIGNAL_FLAG_SIGNAL_EXT_SIZE)        /* To prevent double declaration */
#error COM_RX_SIGNAL_FLAG_SIGNAL_EXT_SIZE is already defined
#endif /* (defined COM_RX_SIGNAL_FLAG_SIGNAL_EXT_SIZE) */

#define COM_RX_SIGNAL_FLAG_SIGNAL_EXT_SIZE 1U

#if (defined COM_RX_SIGNAL_FLAG_SIGNAL_EXT_OFFSET)        /* To prevent double declaration */
#error COM_RX_SIGNAL_FLAG_SIGNAL_EXT_OFFSET is already defined
#endif /* (defined COM_RX_SIGNAL_FLAG_SIGNAL_EXT_OFFSET) */

#define COM_RX_SIGNAL_FLAG_SIGNAL_EXT_OFFSET (COM_RX_SIGNAL_FLAG_OPT_EXT_SIZE + COM_RX_SIGNAL_FLAG_OPT_EXT_OFFSET)

#if (defined COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_CONF_SIZE)        /* To prevent double declaration */
#error COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_CONF_SIZE is already defined
#endif /* (defined COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_CONF_SIZE) */

#define COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_CONF_SIZE 4U

#if (defined COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_CONF_OFFSET)        /* To prevent double declaration */
#error COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_CONF_OFFSET is already defined
#endif /* (defined COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_CONF_OFFSET) */

#define COM_RX_SIGNAL_FLAG_SIGNAL_TYPE_CONF_OFFSET (COM_RX_SIGNAL_FLAG_SIGNAL_EXT_SIZE + COM_RX_SIGNAL_FLAG_SIGNAL_EXT_OFFSET)

#if (defined COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_OPT_SIZE)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_OPT_SIZE is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_OPT_SIZE) */

#define COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_OPT_SIZE 2U

#if (defined COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_OPT_OFFSET)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_OPT_OFFSET is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_OPT_OFFSET) */

#define COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_OPT_OFFSET 0U

#if (defined COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_SIZE)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_SIZE is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_SIZE) */

#define COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_SIZE 1U

#if (defined COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_OFFSET)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_OFFSET is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_OFFSET) */

#define COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_OFFSET (COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_OPT_SIZE + COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_OPT_OFFSET)

#if (defined COM_TX_SIGNAL_FLAG_ENDIANNESS_SIZE)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_ENDIANNESS_SIZE is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_ENDIANNESS_SIZE) */

#define COM_TX_SIGNAL_FLAG_ENDIANNESS_SIZE 1U

#if (defined COM_TX_SIGNAL_FLAG_ENDIANNESS_OFFSET)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_ENDIANNESS_OFFSET is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_ENDIANNESS_OFFSET) */

#define COM_TX_SIGNAL_FLAG_ENDIANNESS_OFFSET (COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_SIZE + COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_OFFSET)

#if (defined COM_TX_SIGNAL_FLAG_TOC_SIZE)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_TOC_SIZE is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_TOC_SIZE) */

#define COM_TX_SIGNAL_FLAG_TOC_SIZE 1U

#if (defined COM_TX_SIGNAL_FLAG_TOC_OFFSET)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_TOC_OFFSET is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_TOC_OFFSET) */

#define COM_TX_SIGNAL_FLAG_TOC_OFFSET (COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_SIGN_SIZE + COM_TX_SIGNAL_FLAG_ENDIANNESS_OFFSET)

#if (defined COM_TX_SIGNAL_FLAG_UPDATE_BIT_SIZE)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_UPDATE_BIT_SIZE is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_UPDATE_BIT_SIZE) */

#define COM_TX_SIGNAL_FLAG_UPDATE_BIT_SIZE 1U

#if (defined COM_TX_SIGNAL_FLAG_UPDATE_BIT_OFFSET)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_UPDATE_BIT_OFFSET is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_UPDATE_BIT_OFFSET) */

#define COM_TX_SIGNAL_FLAG_UPDATE_BIT_OFFSET (COM_TX_SIGNAL_FLAG_TOC_SIZE + COM_TX_SIGNAL_FLAG_TOC_OFFSET)

#if (defined COM_TX_SIGNAL_FLAG_BUFFER_LOCK_SIZE)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_BUFFER_LOCK_SIZE is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_BUFFER_LOCK_SIZE) */

#define COM_TX_SIGNAL_FLAG_BUFFER_LOCK_SIZE 1U

#if (defined COM_TX_SIGNAL_FLAG_BUFFER_LOCK_OFFSET)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_BUFFER_LOCK_OFFSET is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_BUFFER_LOCK_OFFSET) */

#define COM_TX_SIGNAL_FLAG_BUFFER_LOCK_OFFSET (COM_TX_SIGNAL_FLAG_UPDATE_BIT_SIZE + COM_TX_SIGNAL_FLAG_UPDATE_BIT_OFFSET)

#if (defined COM_TX_SIGNAL_FLAG_SPECIAL_SIZE)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_SPECIAL_SIZE is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_SPECIAL_SIZE) */

#define COM_TX_SIGNAL_FLAG_SPECIAL_SIZE 1U

#if (defined COM_TX_SIGNAL_FLAG_SPECIAL_SIZE_OFFSET)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_SPECIAL_SIZE_OFFSET is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_SPECIAL_SIZE_OFFSET) */

#define COM_TX_SIGNAL_FLAG_SPECIAL_SIZE_OFFSET (COM_TX_SIGNAL_FLAG_BUFFER_LOCK_SIZE + COM_TX_SIGNAL_FLAG_BUFFER_LOCK_OFFSET)

#if (defined COM_TX_SIGNAL_FLAG_SIGNAL_EXT_SIZE)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_SIGNAL_EXT_SIZE is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_SIGNAL_EXT_SIZE) */

#define COM_TX_SIGNAL_FLAG_SIGNAL_EXT_SIZE 1U

#if (defined COM_TX_SIGNAL_FLAG_SIGNAL_EXT_OFFSET)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_SIGNAL_EXT_OFFSET is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_SIGNAL_EXT_OFFSET) */

#define COM_TX_SIGNAL_FLAG_SIGNAL_EXT_OFFSET (COM_TX_SIGNAL_FLAG_SPECIAL_SIZE + COM_TX_SIGNAL_FLAG_SPECIAL_SIZE_OFFSET)

#if (defined COM_TX_SIGNAL_FLAG_HAS_FILTER_SIZE)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_HAS_FILTER_SIZE is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_HAS_FILTER_SIZE) */

#define COM_TX_SIGNAL_FLAG_HAS_FILTER_SIZE 1U

#if (defined COM_TX_SIGNAL_FLAG_HAS_FILTER_OFFSET)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_HAS_FILTER_OFFSET is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_HAS_FILTER_OFFSET) */

#define COM_TX_SIGNAL_FLAG_HAS_FILTER_OFFSET (COM_TX_SIGNAL_FLAG_SIGNAL_EXT_SIZE + COM_TX_SIGNAL_FLAG_SIGNAL_EXT_OFFSET)

#if (defined COM_TX_SIGNAL_FLAG_TRANSFER_PROP_SIZE)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_TRANSFER_PROP_SIZE is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_TRANSFER_PROP_SIZE) */

#define COM_TX_SIGNAL_FLAG_TRANSFER_PROP_SIZE 1U

#if (defined COM_TX_SIGNAL_FLAG_TRANSFER_PROP_OFFSET)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_TRANSFER_PROP_OFFSET is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_TRANSFER_PROP_OFFSET) */

#define COM_TX_SIGNAL_FLAG_TRANSFER_PROP_OFFSET (COM_TX_SIGNAL_FLAG_HAS_FILTER_SIZE + COM_TX_SIGNAL_FLAG_HAS_FILTER_OFFSET)

#if (defined COM_TX_SIGNAL_FLAG_TRANSFER_PROP_WO_REP_SIZE)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_TRANSFER_PROP_WO_REP_SIZE is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_TRANSFER_PROP_WO_REP_SIZE) */

#define COM_TX_SIGNAL_FLAG_TRANSFER_PROP_WO_REP_SIZE 1U

#if (defined COM_TX_SIGNAL_FLAG_TRANSFER_PROP_WO_REP_OFFSET)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_TRANSFER_PROP_WO_REP_OFFSET is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_TRANSFER_PROP_WO_REP_OFFSET) */

#define COM_TX_SIGNAL_FLAG_TRANSFER_PROP_WO_REP_OFFSET (COM_TX_SIGNAL_FLAG_TRANSFER_PROP_SIZE + COM_TX_SIGNAL_FLAG_TRANSFER_PROP_OFFSET)

#if (defined COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_CONF_SIZE)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_CONF_SIZE is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_CONF_SIZE) */

#define COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_CONF_SIZE 4U

#if (defined COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_CONF_OFFSET)        /* To prevent double declaration */
#error COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_CONF_OFFSET is already defined
#endif /* (defined COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_CONF_OFFSET) */

#define COM_TX_SIGNAL_FLAG_SIGNAL_TYPE_CONF_OFFSET (COM_TX_SIGNAL_FLAG_TRANSFER_PROP_WO_REP_SIZE + COM_TX_SIGNAL_FLAG_TRANSFER_PROP_WO_REP_OFFSET)

#if (defined COM_RX_SIGNAL_EXT_FLAG_TIMEOUT_ACTION_SIZE)        /* To prevent double declaration */
#error COM_RX_SIGNAL_EXT_FLAG_TIMEOUT_ACTION_SIZE is already defined
#endif /* (defined COM_RX_SIGNAL_EXT_FLAG_TIMEOUT_ACTION_SIZE) */

#define COM_RX_SIGNAL_EXT_FLAG_TIMEOUT_ACTION_SIZE 1U

#if (defined COM_RX_SIGNAL_EXT_FLAG_TIMEOUT_ACTION_OFFSET)        /* To prevent double declaration */
#error COM_RX_SIGNAL_EXT_FLAG_TIMEOUT_ACTION_OFFSET is already defined
#endif /* (defined COM_RX_SIGNAL_EXT_FLAG_TIMEOUT_ACTION_OFFSET) */

#define COM_RX_SIGNAL_EXT_FLAG_TIMEOUT_ACTION_OFFSET 0U

#if (defined COM_RX_SIGNAL_EXT_FLAG_UPDATE_BIT_SIZE)        /* To prevent double declaration */
#error COM_RX_SIGNAL_EXT_FLAG_UPDATE_BIT_SIZE is already defined
#endif /* (defined COM_RX_SIGNAL_EXT_FLAG_UPDATE_BIT_SIZE) */

#define COM_RX_SIGNAL_EXT_FLAG_UPDATE_BIT_SIZE 1U

#if (defined COM_RX_SIGNAL_EXT_FLAG_UPDATE_BIT_OFFSET)        /* To prevent double declaration */
#error COM_RX_SIGNAL_EXT_FLAG_UPDATE_BIT_OFFSET is already defined
#endif /* (defined COM_RX_SIGNAL_EXT_FLAG_UPDATE_BIT_OFFSET) */

#define COM_RX_SIGNAL_EXT_FLAG_UPDATE_BIT_OFFSET (COM_RX_SIGNAL_EXT_FLAG_TIMEOUT_ACTION_SIZE + COM_RX_SIGNAL_EXT_FLAG_TIMEOUT_ACTION_OFFSET)

#if (defined COM_RX_SIGNAL_EXT_FLAG_SIGDM_SIZE)        /* To prevent double declaration */
#error COM_RX_SIGNAL_EXT_FLAG_SIGDM_SIZE is already defined
#endif /* (defined COM_RX_SIGNAL_EXT_FLAG_SIGDM_SIZE) */

#define COM_RX_SIGNAL_EXT_FLAG_SIGDM_SIZE 1U

#if (defined COM_RX_SIGNAL_EXT_FLAG_SIGDM_OFFSET)        /* To prevent double declaration */
#error COM_RX_SIGNAL_EXT_FLAG_SIGDM_OFFSET is already defined
#endif /* (defined COM_RX_SIGNAL_EXT_FLAG_SIGDM_OFFSET) */

#define COM_RX_SIGNAL_EXT_FLAG_SIGDM_OFFSET (COM_RX_SIGNAL_EXT_FLAG_UPDATE_BIT_SIZE + COM_RX_SIGNAL_EXT_FLAG_UPDATE_BIT_OFFSET)

#if (defined COM_RX_SIGNAL_EXT_FLAG_PDUSIGDM_SIZE)        /* To prevent double declaration */
#error COM_RX_SIGNAL_EXT_FLAG_PDUSIGDM_SIZE is already defined
#endif /* (defined COM_RX_SIGNAL_EXT_FLAG_PDUSIGDM_SIZE) */

#define COM_RX_SIGNAL_EXT_FLAG_PDUSIGDM_SIZE 1U

#if (defined COM_RX_SIGNAL_EXT_FLAG_PDUSIGDM_OFFSET)        /* To prevent double declaration */
#error COM_RX_SIGNAL_EXT_FLAG_PDUSIGDM_OFFSET is already defined
#endif /* (defined COM_RX_SIGNAL_EXT_FLAG_PDUSIGDM_OFFSET) */

#define COM_RX_SIGNAL_EXT_FLAG_PDUSIGDM_OFFSET (COM_RX_SIGNAL_EXT_FLAG_SIGDM_SIZE + COM_RX_SIGNAL_EXT_FLAG_SIGDM_OFFSET)

#if (defined COM_RX_SIGNAL_EXT_FLAG_SIGDUMMY_SIZE)        /* To prevent double declaration */
#error COM_RX_SIGNAL_EXT_FLAG_SIGDUMMY_SIZE is already defined
#endif /* (defined COM_RX_SIGNAL_EXT_FLAG_SIGDUMMY_SIZE) */

#define COM_RX_SIGNAL_EXT_FLAG_SIGDUMMY_SIZE 1U

#if (defined COM_RX_SIGNAL_EXT_FLAG_SIGDUMMY_OFFSET)        /* To prevent double declaration */
#error COM_RX_SIGNAL_EXT_FLAG_SIGDUMMY_OFFSET is already defined
#endif /* (defined COM_RX_SIGNAL_EXT_FLAG_SIGDUMMY_OFFSET) */

#define COM_RX_SIGNAL_EXT_FLAG_SIGDUMMY_OFFSET (COM_RX_SIGNAL_EXT_FLAG_SIGDUMMY_SIZE + COM_RX_SIGNAL_EXT_FLAG_PDUSIGDM_OFFSET)

#if (defined COM_CHECK_VALUE_SIZE)        /* To prevent double declaration */
#error COM_CHECK_VALUE_SIZE already defined
#endif /* #if (defined COM_CHECK_VALUE_SIZE) */

#if (COM_DEV_ERROR_DETECT == STD_ON)

/** \brief Define COM_CHECK_VALUE_SIZE
 * defines if it shall be check if the value of a Tx signal fits into the Tx-Pdu
 * STD_ON Check turned on
 * STD_OFF Check turned off */
#define COM_CHECK_VALUE_SIZE COM_CHECK_VALUE_SIZE_ENABLE
#else
#define COM_CHECK_VALUE_SIZE STD_OFF
#endif /* (COM_DEV_ERROR_DETECT == STD_ON) */



#if (defined COM_FILTER_ONE_EVERY_N_SIZE_TYPE_MAX)        /* To prevent double declaration */
#error COM_FILTER_ONE_EVERY_N_SIZE_TYPE_MAX already defined
#endif /* #if (defined COM_FILTER_ONE_EVERY_N_SIZE_TYPE_MAX) */

#if (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON)

#if (COM_FILTER_ONE_EVERY_N_PERIOD_OFFSET_SIZE_MAX == COM_FILTER_ONE_EVERY_N_OCCURANCE_MAX)
#define COM_FILTER_ONE_EVERY_N_SIZE_TYPE_MAX COM_FILTER_ONE_EVERY_N_PERIOD_OFFSET_SIZE_MAX
#else
#error COM_FILTER_ONE_EVERY_N_PERIOD_OFFSET_SIZE_MAX shall be equal COM_FILTER_ONE_EVERY_N_OCCURANCE_MAX
#endif /* (COM_FILTER_ONE_EVERY_N_PERIOD_OFFSET_SIZE_MAX == COM_FILTER_ONE_EVERY_N_OCCURANCE_MAX) */

#endif /* (COM_FILTER_ONE_EVERY_N_AVAILABLE == STD_ON) */



#if (defined COM_VALUE_INIT_REPLACE)        /* To prevent double declaration */
#error COM_VALUE_INIT_REPLACE already defined
#endif /* #if (defined COM_VALUE_INIT_REPLACE) */

#if ((COM_RX_DATA_TOUT_ACTION != COM_RX_DATA_TOUT_ACTION_NONE) || \
      (COM_FILTER_RECEIVER_ENABLE == STD_ON))
#define COM_VALUE_INIT_REPLACE STD_ON
#else
#define COM_VALUE_INIT_REPLACE STD_OFF
#endif /* ((COM_RX_DATA_TOUT_ACTION != COM_RX_DATA_TOUT_ACTION_NONE) || \
      (COM_FILTER_RECEIVER_ENABLE == STD_ON)) */


#if (defined COM_SIGNALS_WITH_UB_RXDM_GW_ONLY)        /* To prevent double declaration */
#error COM_SIGNALS_WITH_UB_RXDM_GW_ONLY already defined
#endif /* #if (defined COM_SIGNALS_WITH_UB_RXDM_GW_ONLY) */

#if ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) && \
     ((COM_RX_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT) || (COM_SIGNAL_GW_ENABLE == STD_ON)))
#define COM_SIGNALS_WITH_UB_RXDM_GW_ONLY STD_ON
#else
#define COM_SIGNALS_WITH_UB_RXDM_GW_ONLY STD_OFF
#endif /* ((COM_UPDATE_BIT_RX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL) && \
     ((COM_RX_TIMEOUT_FACTOR_SIZE != COM_SIZE_0_BIT) || (COM_SIGNAL_GW_ENABLE == STD_ON))) */


#if (defined COM_NOTIFY_RECEIVERS_ENABLE)        /* To prevent double declaration */
#error COM_NOTIFY_RECEIVERS_ENABLE already defined
#endif /* #if (defined COM_NOTIFY_RECEIVERS_ENABLE) */

#if ((COM_FILTER_RECEIVER_ENABLE == STD_ON) ||\
      (COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX != COM_INDEX_NONE) ||\
      (COM_SIGNALS_WITH_UB_RXDM_GW_ONLY == STD_ON))
#define COM_NOTIFY_RECEIVERS_ENABLE STD_ON
#else
#define COM_NOTIFY_RECEIVERS_ENABLE STD_OFF
#endif


#if (defined COM_RX_SIGNAL_BUFFER_AVAILABLE)        /* To prevent double declaration */
#error COM_RX_SIGNAL_BUFFER_AVAILABLE already defined
#endif /* (defined COM_RX_SIGNAL_BUFFER_AVAILABLE) */

#if (COM_FILTER_RECEIVER_ENABLE == STD_ON)
#define COM_RX_SIGNAL_BUFFER_AVAILABLE STD_ON
#else
#define COM_RX_SIGNAL_BUFFER_AVAILABLE STD_OFF
#endif

#if (defined COM_TX_SIGNAL_BUFFER_AVAILABLE)        /* To prevent double declaration */
#error COM_TX_SIGNAL_BUFFER_AVAILABLE already defined
#endif /* (defined COM_TX_SIGNAL_BUFFER_AVAILABLE) */

#if ((COM_TXF_MASKNEWDIFFERSMASKOLD_EN == STD_ON) && (COM_TMS_ENABLE == STD_ON))
#define COM_TX_SIGNAL_BUFFER_AVAILABLE STD_ON
#else
#define COM_TX_SIGNAL_BUFFER_AVAILABLE STD_OFF
#endif



/** \brief COM_RCV_RX_SIG_GENERATED_SCHMENTER */
/** \brief COM_RCV_RX_SIG_GENERATED_SCHMEXIT */
/** \brief COM_RCV_RX_SIG_GENERATED_ATOMIC_ASSIGN8 */
/** \brief COM_RCV_RX_SIG_GENERATED_ATOMIC_ASSIGN16 */
/** \brief COM_RCV_RX_SIG_GENERATED_ATOMIC_ASSIGN32 */
#if (defined COM_RCV_RX_SIG_GENERATED_SCHMENTER)        /* To prevent double declaration */
#error COM_RCV_RX_SIG_GENERATED_SCHMENTER already defined
#endif /* #if (defined COM_RCV_RX_SIG_GENERATED_SCHMENTER) */

#if (defined COM_RCV_RX_SIG_GENERATED_SCHMEXIT)        /* To prevent double declaration */
#error COM_RCV_RX_SIG_GENERATED_SCHMEXIT already defined
#endif /* #if (defined COM_RCV_RX_SIG_GENERATED_SCHMEXIT) */

#if (defined COM_RCV_RX_SIG_GENERATED_ATOMIC_ASSIGN8)        /* To prevent double declaration */
#error COM_RCV_RX_SIG_GENERATED_ATOMIC_ASSIGN8 already defined
#endif /* #if (defined COM_RCV_RX_SIG_GENERATED_ATOMIC_ASSIGN8) */

#if (defined COM_RCV_RX_SIG_GENERATED_ATOMIC_ASSIGN16)        /* To prevent double declaration */
#error COM_RCV_RX_SIG_GENERATED_ATOMIC_ASSIGN16 already defined
#endif /* #if (defined COM_RCV_RX_SIG_GENERATED_ATOMIC_ASSIGN16) */

#if (defined COM_RCV_RX_SIG_GENERATED_ATOMIC_ASSIGN32)        /* To prevent double declaration */
#error COM_RCV_RX_SIG_GENERATED_ATOMIC_ASSIGN32 already defined
#endif /* #if (defined COM_RCV_RX_SIG_GENERATED_ATOMIC_ASSIGN32) */

#if (defined COM_RCV_RX_SIG_LOCK_GENERATED)
#if (COM_RCV_RX_SIG_LOCK_GENERATED == STD_ON)
#define COM_RCV_RX_SIG_GENERATED_SCHMENTER() \
   Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0()

#define COM_RCV_RX_SIG_GENERATED_SCHMEXIT() \
   Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0()

#define COM_RCV_RX_SIG_GENERATED_ATOMIC_ASSIGN8(to, from) \
   TS_AtomicAssign8((to), (from))

#define COM_RCV_RX_SIG_GENERATED_ATOMIC_ASSIGN16(to, from) \
   TS_AtomicAssign16((to), (from))

#define COM_RCV_RX_SIG_GENERATED_ATOMIC_ASSIGN32(to, from) \
   TS_AtomicAssign32((to), (from))

#else
#define COM_RCV_RX_SIG_GENERATED_SCHMENTER()
#define COM_RCV_RX_SIG_GENERATED_SCHMEXIT()
#define COM_RCV_RX_SIG_GENERATED_ATOMIC_ASSIGN8(to, from) ((to) = (from))
#define COM_RCV_RX_SIG_GENERATED_ATOMIC_ASSIGN16(to, from) ((to) = (from))
#define COM_RCV_RX_SIG_GENERATED_ATOMIC_ASSIGN32(to, from) ((to) = (from))
#endif /* (COM_RCV_RX_SIG_LOCK_GENERATED == STD_ON) */
#endif /* (defined COM_RCV_RX_SIG_LOCK_GENERATED) */


/** \brief COM_GENERATED_RCV_SIG_ENABLE */
#ifndef COM_GENERATED_RCV_SIG_ENABLE
#define COM_GENERATED_RCV_SIG_ENABLE STD_OFF
#endif /* ifndef COM_GENERATED_RCV_SIG_ENABLE */


/** \brief COM_EVALUATE_TRIGGERED_OC */
#if (defined COM_EVALUATE_TRIGGERED_OC)        /* To prevent double declaration */
#error COM_EVALUATE_TRIGGERED_OC already defined
#endif /* #if (defined COM_EVALUATE_TRIGGERED_OC) */

#if ((COM_TM_AVAILABLE == STD_ON) && (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON))
#define COM_EVALUATE_TRIGGERED_OC STD_ON
#else
#define COM_EVALUATE_TRIGGERED_OC STD_OFF
#endif /* ((COM_TM_AVAILABLE == STD_ON) && (COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON)) */


/** \brief COM_SENDUPONAPICALL */
#if (defined COM_SENDUPONAPICALL)        /* To prevent double declaration */
#error COM_SENDUPONAPICALL already defined
#endif /* #if (defined COM_SENDUPONAPICALL) */

#if ((COM_TM_AVAILABLE == STD_ON) && \
      ((COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) || \
       (COM_TRANSF_PROP_TRIGGERED_ENABLE == STD_ON) || \
       (COM_TMS_ENABLE == STD_ON)))
#define COM_SENDUPONAPICALL STD_ON
#else
#define COM_SENDUPONAPICALL STD_OFF
#endif /* ((COM_TM_AVAILABLE == STD_ON) && \
      ((COM_TRANSF_PROP_TRIGGERED_OC_ENABLE == STD_ON) || \
       (COM_TRANSF_PROP_TRIGGERED_ENABLE == STD_ON) || \
       (COM_TMS_ENABLE == STD_ON)))) */


/** \brief COM_SENDUPONAPICALL_AND_SIGGW */
#if (defined COM_SENDUPONAPICALL_AND_SIGGW)        /* To prevent double declaration */
#error COM_SENDUPONAPICALL_AND_SIGGW already defined
#endif /* #if (defined COM_SENDUPONAPICALL_AND_SIGGW) */

#if ((COM_SENDUPONAPICALL == STD_ON) && (COM_SIGNAL_GW_ENABLE == STD_ON))
#define COM_SENDUPONAPICALL_AND_SIGGW STD_ON
#else
#define COM_SENDUPONAPICALL_AND_SIGGW STD_OFF
#endif /* ((COM_SENDUPONAPICALL == STD_ON) && (COM_SIGNAL_GW_ENABLE == STD_ON)) */


/** \brief COM_CONST_INIT_ARRAY_SIZE */
#if (defined COM_CONST_INIT_ARRAY_SIZE)        /* To prevent double declaration */
#error COM_CONST_INIT_ARRAY_SIZE already defined
#endif /* #if (defined COM_CONST_INIT_ARRAY_SIZE) */

#define COM_CONST_INIT_ARRAY_SIZE 9U


/** \brief COM_CONST_INIT_START_IDX */
#if (defined COM_CONST_INIT_START_IDX)        /* To prevent double declaration */
#error COM_CONST_INIT_START_IDX already defined
#endif /* #if (defined COM_CONST_INIT_START_IDX) */

#define COM_CONST_INIT_START_IDX ((COM_CONST_INIT_ARRAY_SIZE + 1U) / 2U)


/** \brief COM_IPDU_GROUP_VECTOR_NUM_BYTES */
#if (defined COM_IPDU_GROUP_VECTOR_NUM_BYTES)        /* To prevent double declaration */
#error COM_IPDU_GROUP_VECTOR_NUM_BYTES already defined
#endif /* #if (defined COM_IPDU_GROUP_VECTOR_NUM_BYTES) */

#define COM_IPDU_GROUP_VECTOR_NUM_BYTES (((COM_SUPPORTED_IPDU_GROUPS - 1U) / 8U) + 1U)

#if (defined Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0)        /* To prevent double declaration */
#error Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0 is already defined
#endif /* (defined Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0) */

/** \brief Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0
 * maps the entry point of the EXCLUSIVE_AREA_0 to the SchM API */
#define Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_0() \
   SchM_Enter_Com_SCHM_COM_EXCLUSIVE_AREA_0()

#if (defined Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0)        /* To prevent double declaration */
#error Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0 is already defined
#endif /* (defined Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0) */

/** \brief Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0
 * maps the exit point of the EXCLUSIVE_AREA_0 to the SchM API */
#define Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_0() \
   SchM_Exit_Com_SCHM_COM_EXCLUSIVE_AREA_0()

#if (defined Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_1)        /* To prevent double declaration */
#error Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_1 is already defined
#endif /* (defined Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_1) */

/** \brief Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_1
 * mapps the entry point of the EXCLUSIVE_AREA_1 to the SchM API */
#define Com_EB_SchM_Enter_SCHM_COM_EXCLUSIVE_AREA_1() \
   SchM_Enter_Com_SCHM_COM_EXCLUSIVE_AREA_1()

#if (defined Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_1)        /* To prevent double declaration */
#error Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_1 is already defined
#endif /* (defined Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_1) */

/** \brief Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_1
 * maps the exit point of the EXCLUSIVE_AREA_1 to the SchM API */
#define Com_EB_SchM_Exit_SCHM_COM_EXCLUSIVE_AREA_1() \
   SchM_Exit_Com_SCHM_COM_EXCLUSIVE_AREA_1()


/** \brief COM_EVALUATE_TRANSMIT_RET_VAL */
#if (defined COM_EVALUATE_TRANSMIT_RET_VAL)        /* To prevent double declaration */
#error COM_EVALUATE_TRANSMIT_RET_VAL already defined
#endif /* #if (defined COM_EVALUATE_TRANSMIT_RET_VAL) */

#if (((COM_CL_UB_TX_TRANSMIT_ENABLE == STD_ON) && \
      (COM_UPDATE_BIT_TX_CONFIG != COM_UPDATE_BIT_ABSENT_FOR_ALL)) || \
      (COM_RETRY_FAILED_TRANSMIT_REQ == STD_ON))

#define COM_EVALUATE_TRANSMIT_RET_VAL STD_ON
#else
#define COM_EVALUATE_TRANSMIT_RET_VAL STD_OFF
#endif


/** \brief COM_MULTI_ROUTE_MAIN_ENABLE */
#if (defined COM_MULTI_ROUTE_MAIN_ENABLE)        /* To prevent double declaration */
#error COM_MULTI_ROUTE_MAIN_ENABLE already defined
#endif /* #if (defined COM_MULTI_ROUTE_MAIN_ENABLE) */

#if (COM_TIMEBASE == STD_OFF)
#define COM_MULTI_ROUTE_MAIN_ENABLE STD_ON
#else
#define COM_MULTI_ROUTE_MAIN_ENABLE STD_OFF
#endif


#if (defined COM_HANDLESMALLERRXPDUS_CPY_SIG)        /* To prevent double declaration */
#error COM_HANDLESMALLERRXPDUS_CPY_SIG already defined
#endif /* #if (defined COM_HANDLESMALLERRXPDUS_CPY_SIG) */

#if (defined COM_HANDLESMALLERRXPDUS_NOTIFY)        /* To prevent double declaration */
#error COM_HANDLESMALLERRXPDUS_NOTIFY already defined
#endif /* #if (defined COM_HANDLESMALLERRXPDUS_NOTIFY) */

#if (defined COM_HANDLESMALLERRXPDUS_CPY_SIG_TEST)        /* To prevent double declaration */
#error COM_HANDLESMALLERRXPDUS_CPY_SIG_TEST already defined
#endif /* #if (defined COM_HANDLESMALLERRXPDUS_CPY_SIG_TEST) */

#if (defined COM_HANDLESMALLERRXPDUS_NOTIFY_TEST)        /* To prevent double declaration */
#error COM_HANDLESMALLERRXPDUS_NOTIFY_TEST already defined
#endif /* #if (defined COM_HANDLESMALLERRXPDUS_NOTIFY_TEST) */


#if (COM_HANDLESMALLERRXPDUS == COM_TRUNCATE_DATA_NOTIFY_ALL)
#define COM_HANDLESMALLERRXPDUS_CPY_SIG STD_OFF
#define COM_HANDLESMALLERRXPDUS_NOTIFY STD_OFF
#define COM_HANDLESMALLERRXPDUS_CPY_SIG_TEST STD_OFF
#define COM_HANDLESMALLERRXPDUS_NOTIFY_TEST STD_OFF

#elif (COM_HANDLESMALLERRXPDUS == COM_TRUNCATE_DATA_NOTIFY_ONLY_RECEIVED)
#define COM_HANDLESMALLERRXPDUS_CPY_SIG STD_OFF
#define COM_HANDLESMALLERRXPDUS_NOTIFY STD_ON
#define COM_HANDLESMALLERRXPDUS_CPY_SIG_TEST STD_OFF
#define COM_HANDLESMALLERRXPDUS_NOTIFY_TEST STD_OFF

#elif (COM_HANDLESMALLERRXPDUS == COM_COPY_ONLY_RECEIVED_DATA_NOTIFY_ALL)
#define COM_HANDLESMALLERRXPDUS_CPY_SIG STD_ON
#define COM_HANDLESMALLERRXPDUS_NOTIFY STD_OFF
#define COM_HANDLESMALLERRXPDUS_CPY_SIG_TEST STD_OFF
#define COM_HANDLESMALLERRXPDUS_NOTIFY_TEST STD_OFF

#elif (COM_HANDLESMALLERRXPDUS == COM_COPY_ONLY_RECEIVED_DATA_NOTIFY_ONLY_RECEIVED)
#define COM_HANDLESMALLERRXPDUS_CPY_SIG STD_ON
#define COM_HANDLESMALLERRXPDUS_NOTIFY STD_ON
#define COM_HANDLESMALLERRXPDUS_CPY_SIG_TEST STD_OFF
#define COM_HANDLESMALLERRXPDUS_NOTIFY_TEST STD_OFF

#elif (COM_HANDLESMALLERRXPDUS == COM_TEST_TRUNCATE_DATA_NOTIFY_ONLY_RECEIVED)
#define COM_HANDLESMALLERRXPDUS_CPY_SIG STD_OFF
#define COM_HANDLESMALLERRXPDUS_NOTIFY STD_ON
#define COM_HANDLESMALLERRXPDUS_CPY_SIG_TEST COM_HANDLESMALLERRXPDUS_CPY_SIG
#define COM_HANDLESMALLERRXPDUS_NOTIFY_TEST COM_HANDLESMALLERRXPDUS_NOTIFY

#elif (COM_HANDLESMALLERRXPDUS == COM_TEST_COPY_ONLY_RECEIVED_DATA_NOTIFY_ALL)
#define COM_HANDLESMALLERRXPDUS_CPY_SIG STD_ON
#define COM_HANDLESMALLERRXPDUS_NOTIFY STD_OFF
#define COM_HANDLESMALLERRXPDUS_CPY_SIG_TEST COM_HANDLESMALLERRXPDUS_CPY_SIG
#define COM_HANDLESMALLERRXPDUS_NOTIFY_TEST COM_HANDLESMALLERRXPDUS_NOTIFY

#elif (COM_HANDLESMALLERRXPDUS == COM_TEST_COPY_ONLY_RECEIVED_DATA_NOTIFY_ONLY_RECEIVED)
#define COM_HANDLESMALLERRXPDUS_CPY_SIG STD_ON
#define COM_HANDLESMALLERRXPDUS_NOTIFY STD_ON
#define COM_HANDLESMALLERRXPDUS_CPY_SIG_TEST COM_HANDLESMALLERRXPDUS_CPY_SIG
#define COM_HANDLESMALLERRXPDUS_NOTIFY_TEST COM_HANDLESMALLERRXPDUS_NOTIFY

#else
#error unknown value of COM_HANDLESMALLERRXPDUS
#endif


#endif /*COM_RULES_H_ */

