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
#ifndef COM_DEFS_H_
#define COM_DEFS_H_
/*==================[inclusions]============================================*/

#include <TSAutosar.h>         /* EB specific standard types */
#include <Com_DefProg_Cfg.h>   /* Module macros of defensive programming configuration */

/*==================[macros]================================================*/

#if (defined COM_IPDUGROUP_CONTROL)   /* To prevent double declaration */
#error COM_IPDUGROUP_CONTROL already defined
#endif /* #if (defined COM_IPDUGROUP_CONTROL) */

/** \brief Define COM_START_GROUP */
#define COM_IPDUGROUP_CONTROL 0U


#if (defined COM_IPDUGROUP_RXDM_CONTROL)    /* To prevent double declaration */
#error COM_IPDUGROUP_RXDM_CONTROL already defined
#endif /* #if (defined COM_IPDUGROUP_RXDM_CONTROL) */

/** \brief Define COM_IPDUGROUP_RXDM_CONTROL */
#define COM_IPDUGROUP_RXDM_CONTROL 1U


#if (defined COM_SERVICE_NOT_AVAILABLE) /* To prevent double declaration */
#error COM_SERVICE_NOT_AVAILABLE already defined
#endif /* #if (defined COM_SERVICE_NOT_AVAILABLE) */

/** \brief Define COM_SERVICE_NOT_AVAILABLE */
#define COM_SERVICE_NOT_AVAILABLE 0x80U


#if (defined COM_BUSY)       /* To prevent double declaration */
#error COM_BUSY already defined
#endif /* #if (defined COM_BUSY) */

/** \brief Define COM_BUSY */
#define COM_BUSY 0x81U

#if (defined COM_TIMEOUT)       /* To prevent double declaration */
#error COM_TIMEOUT already defined
#endif /* #if (defined COM_TIMEOUT) */

/** \brief Define COM_TIMEOUT */
#define COM_TIMEOUT 0x81U


#if (defined COM_GW_UPDATE_BIT)     /* To prevent double declaration */
#error COM_GW_UPDATE_BIT already defined
#endif /* #if (defined COM_GW_UPDATE_BIT) */

/** \brief Define COM_GW_UPDATE_BIT */
#define COM_GW_UPDATE_BIT 4U

#if (defined COM_GW_SIGNAL)     /* To prevent double declaration */
#error COM_GW_SIGNAL already defined
#endif /* #if (defined COM_GW_SIGNAL) */

/** \brief Define COM_GW_SIGNAL */
#define COM_GW_SIGNAL 0U

#if (defined COM_GW_SIGNAL_GROUP)       /* To prevent double declaration */
#error COM_GW_SIGNAL_GROUP already defined
#endif /* #if (defined COM_GW_SIGNAL_GROUP) */

/** \brief Define COM_GW_SIGNAL_GROUP */
#define COM_GW_SIGNAL_GROUP 1U

#if (defined COM_GW_GROUP_SIGNAL)       /* To prevent double declaration */
#error COM_GW_GROUP_SIGNAL already defined
#endif /* #if (defined COM_GW_GROUP_SIGNAL) */

/** \brief Define COM_GW_GROUP_SIGNAL */
#define COM_GW_GROUP_SIGNAL 2U

#if (defined COM_GW_DESCRIPTION) /* To prevent double declaration */
#error COM_GW_DESCRIPTION already defined
#endif /* #if (defined COM_GW_DESCRIPTION) */

/** \brief Define COM_GW_DESCRIPTION */
#define COM_GW_DESCRIPTION 3U


#if (defined COM_SIG_TYPE_08)       /* To prevent double declaration */
#error COM_SIG_TYPE_08 already defined
#endif /* #if (defined COM_SIG_TYPE_08) */

#define COM_SIG_TYPE_08 0U


#if (defined COM_SIG_TYPE_16)       /* To prevent double declaration */
#error COM_SIG_TYPE_16 already defined
#endif /* #if (defined COM_SIG_TYPE_16) */

#define COM_SIG_TYPE_16 1U


#if (defined COM_SIG_TYPE_32)       /* To prevent double declaration */
#error COM_SIG_TYPE_32 already defined
#endif /* #if (defined COM_SIG_TYPE_32) */

#define COM_SIG_TYPE_32 2U


#if (defined COM_SIG_TYPE_AY)       /* To prevent double declaration */
#error COM_SIG_TYPE_AY already defined
#endif /* #if (defined COM_SIG_TYPE_AY) */

#define COM_SIG_TYPE_AY 3U

#if (defined COM_SIGNED)       /* To prevent double declaration */
#error COM_SIGNED already defined
#endif /* #if (defined COM_SIGNED) */


/** \brief Define COM_SIGNED */
#define COM_SIGNED 1U

#if (defined COM_UNSIGNED)       /* To prevent double declaration */
#error COM_UNSIGNED already defined
#endif /* #if (defined COM_UNSIGNED) */


/** \brief Define COM_UNSIGNED */
#define COM_UNSIGNED 0U

#if (defined COM_BOOLEAN)       /* To prevent double declaration */
#error COM_BOOLEAN already defined
#endif /* #if (defined COM_BOOLEAN) */


/** \brief Define COM_BOOLEAN */
#define COM_BOOLEAN 0U


#if (defined COM_SINT16)        /* To prevent double declaration */
#error COM_SINT16 already defined
#endif /* #if (defined COM_SINT16) */

/** \brief Define COM_SINT16 */
#define COM_SINT16 1U


#if (defined COM_SINT32)        /* To prevent double declaration */
#error COM_SINT32 already defined
#endif /* #if (defined COM_SINT32) */

/** \brief Define COM_SINT32 */
#define COM_SINT32 2U


#if (defined COM_SINT8)         /* To prevent double declaration */
#error COM_SINT8 already defined
#endif /* #if (defined COM_SINT8) */

/** \brief Define COM_SINT8 */
#define COM_SINT8 3U


#if (defined COM_UINT16)        /* To prevent double declaration */
#error COM_UINT16 already defined
#endif /* #if (defined COM_UINT16) */


/** \brief Define COM_UINT16 */
#define COM_UINT16 4U


#if (defined COM_UINT32)        /* To prevent double declaration */
#error COM_UINT32 already defined
#endif /* #if (defined COM_UINT32) */

/** \brief Define COM_UINT32 */
#define COM_UINT32 5U


#if (defined COM_UINT8)         /* To prevent double declaration */
#error COM_UINT8 already defined
#endif /* #if (defined COM_UINT8) */

/** \brief Define COM_UINT8 */
#define COM_UINT8 6U


#if (defined COM_UINT8_N)       /* To prevent double declaration */
#error COM_UINT8_N already defined
#endif /* #if (defined COM_UINT8_N) */

/** \brief Define COM_UINT8_N */
#define COM_UINT8_N 7U


#if (defined COM_UINT8_DYN)       /* To prevent double declaration */
#error COM_UINT8_DYN already defined
#endif /* #if (defined COM_UINT8_DYN) */

/** \brief Define COM_UINT8_N */
#define COM_UINT8_DYN 8U


#if (defined COM_UINT64)       /* To prevent double declaration */
#error COM_UINT64 already defined
#endif /* #if (defined COM_UINT64) */

/** \brief Define COM_UINT64 */
#define COM_UINT64 9U


#if (defined COM_SINT64)       /* To prevent double declaration */
#error COM_SINT64 already defined
#endif /* #if (defined COM_SINT64) */

/** \brief Define COM_SINT64 */
#define COM_SINT64 10U


#if (defined COM_BIG_ENDIAN)    /* To prevent double declaration */
#error COM_BIG_ENDIAN already defined
#endif /* #if (defined COM_BIG_ENDIAN) */

/** \brief Define COM_BIG_ENDIAN */
#define COM_BIG_ENDIAN 1U

#if (defined COM_LITTLE_ENDIAN) /* To prevent double declaration */
#error COM_LITTLE_ENDIAN already defined
#endif /* #if (defined COM_LITTLE_ENDIAN) */

/** \brief Define COM_LITTLE_ENDIAN */
#define COM_LITTLE_ENDIAN 2U

#if (defined COM_OPAQUE)        /* To prevent double declaration */
#error COM_OPAQUE already defined
#endif /* #if (defined COM_OPAQUE) */

/** \brief Define COM_OPAQUE */
#define COM_OPAQUE 3U

#if (defined COM_ENDIAN_ENABLE) /* To prevent double declaration */
#error COM_ENDIAN_ENABLE already defined
#endif /* #if (defined COM_ENDIAN_ENABLE) */

/** \brief Define COM_ENDIAN_ENABLE */
#define COM_ENDIAN_ENABLE 4U

#if (defined COM_ENDIAN_DISABLE)        /* To prevent double declaration */
#error COM_ENDIAN_DISABLE already defined
#endif /* #if (defined COM_ENDIAN_DISABLE) */

/** \brief Define COM_ENDIAN_DISABLE */
#define COM_ENDIAN_DISABLE 5U


#if (defined COM_SIZE_0_BIT)          /* To prevent double declaration */
#error COM_SIZE_0_BIT already defined
#endif /* #if (defined COM_SIZE_0_BIT) */

/** \brief Define COM_SIZE_0_BIT */
#define COM_SIZE_0_BIT 0U

#if (defined COM_SIZE_8_BIT)          /* To prevent double declaration */
#error COM_SIZE_8_BIT already defined
#endif /* #if (defined COM_SIZE_8_BIT) */

/** \brief Define COM_SIZE_8_BIT */
#define COM_SIZE_8_BIT (COM_SIZE_0_BIT+1U)

#if (defined COM_SIZE_16_BIT)         /* To prevent double declaration */
#error COM_SIZE_16_BIT already defined
#endif /* #if (defined COM_SIZE_16_BIT) */

/** \brief Define COM_SIZE_16_BIT */
#define COM_SIZE_16_BIT (COM_SIZE_8_BIT+1U)

#if (defined COM_SIZE_32_BIT)         /* To prevent double declaration */
#error COM_SIZE_32_BIT already defined
#endif /* #if (defined COM_SIZE_32_BIT) */

/** \brief Define COM_SIZE_32_BIT */
#define COM_SIZE_32_BIT (COM_SIZE_16_BIT+1U)


#if (defined COM_IPDU_GROUP_GROUP)      /* To prevent double declaration */
#error COM_IPDU_GROUP_GROUP already defined
#endif /* if (defined COM_IPDU_GROUP_GROUP) */

/** \brief Define COM_IPDU_GROUP_GROUP */
#define COM_IPDU_GROUP_GROUP 0U

#if (defined COM_IPDU_GROUP_IPDU)       /* To prevent double declaration */
#error COM_IPDU_GROUP_IPDU already defined
#endif /* if (defined COM_IPDU_GROUP_IPDU) */

/** \brief Define COM_IPDU_GROUP_IPDU */
#define COM_IPDU_GROUP_IPDU 1U


#if (defined COM_RECEIVE)       /* To prevent double declaration */
#error COM_RECEIVE already defined
#endif /* if (defined COM_RECEIVE) */

/** \brief Define COM_RECEIVE */
#define COM_RECEIVE 0U

#if (defined COM_SEND)          /* To prevent double declaration */
#error COM_SEND already defined
#endif /* if (defined COM_SEND) */

/** \brief Define COM_SEND */
#define COM_SEND 1U


#if (defined COM_SIG_GW_NONE)       /* To prevent double declaration */
#error COM_SIG_GW_NONE already defined
#endif /* if (defined COM_SIG_GW_NONE) */

/** \brief Define COM_SIG_GW_NONE */
#define COM_SIG_GW_NONE 0U

#if (defined COM_SIG_GW_NO_UPDATE_BIT)       /* To prevent double declaration */
#error COM_SIG_GW_NO_UPDATE_BIT already defined
#endif /* if (defined COM_SIG_GW_NO_UPDATE_BIT) */

/** \brief Define COM_SIG_GW_NO_UPDATE_BIT */
#define COM_SIG_GW_NO_UPDATE_BIT 1U

#if (defined COM_SIG_GW_UPDATE_BIT)       /* To prevent double declaration */
#error COM_SIG_GW_UPDATE_BIT already defined
#endif /* if (defined COM_SIG_GW_UPDATE_BIT) */

/** \brief Define COM_SIG_GW_UPDATE_BIT */
#define COM_SIG_GW_UPDATE_BIT 2U

#if (defined COM_SIG_GW_WITH_WITHOUT_UP_BIT)       /* To prevent double declaration */
#error COM_SIG_GW_WITH_WITHOUT_UP_BIT already defined
#endif /* if (defined COM_SIG_GW_WITH_WITHOUT_UP_BIT) */

/** \brief Define COM_SIG_GW_WITH_WITHOUT_UP_BIT (COM_SIG_GW_WITH_WITHOUT_UPDATE_BIT) */
#define COM_SIG_GW_WITH_WITHOUT_UP_BIT 3U


#if (defined COM_SIG_GW_SIG_NATVIE)       /* To prevent double declaration */
#error COM_SIG_GW_SIG_NATVIE already defined
#endif /* if (defined COM_SIG_GW_SIG_NATVIE) */

/** \brief Define COM_SIG_GW_SIG_DESC */
#define COM_SIG_GW_SIG_NATVIE 0U

#if (defined COM_SIG_GW_SIG_DESC)       /* To prevent double declaration */
#error COM_SIG_GW_SIG_DESC already defined
#endif /* if (defined COM_SIG_GW_SIG_DESC) */

/** \brief Define COM_SIG_GW_SIG_DESC */
#define COM_SIG_GW_SIG_DESC 2U


#if (defined COM_DEFERED)       /* To prevent double declaration */
#error COM_DEFERED already defined
#endif /* if (defined COM_DEFERED) */

/** \brief Define COM_DEFERED */
#define COM_DEFERED 0U

#if (defined COM_IMMEDIATE)     /* To prevent double declaration */
#error COM_IMMEDIATE already defined
#endif /* if (defined COM_IMMEDIATE) */

/** \brief Define COM_IMMEDIATE */
#define COM_IMMEDIATE 1U


#if (defined COM_RX_DM_ON)      /* To prevent double declaration */
#error COM_RX_DM_ON already defined
#endif /* if (defined COM_RX_DM_ON) */

/** \brief Define COM_RX_DM_ON */
#define COM_RX_DM_ON TRUE

#if (defined COM_RX_SIG_DM_ON)      /* To prevent double declaration */
#error COM_RX_SIG_DM_ON already defined
#endif /* if (defined COM_RX_SIG_DM_ON) */

/** \brief Define COM_RX_SIG_DM_ON */
#define COM_RX_SIG_DM_ON TRUE


#if (defined COM_RX_DM_OFF)     /* To prevent double declaration */
#error COM_RX_DM_OFF already defined
#endif /* if (defined COM_RX_DM_OFF) */

/** \brief Define COM_RX_DM_OFF */
#define COM_RX_DM_OFF FALSE


#if (defined COM_DYN_IPDU_ON)     /* To prevent double declaration */
#error COM_DYN_IPDU_ON already defined
#endif /* if (defined COM_DYN_IPDU_ON) */

/** \brief Define COM_DYN_IPDU_ON */
#define COM_DYN_IPDU_ON TRUE


#if (defined COM_DYN_IPDU_OFF)     /* To prevent double declaration */
#error COM_DYN_IPDU_OFF already defined
#endif /* if (defined COM_DYN_IPDU_OFF) */

/** \brief Define COM_DYN_IPDU_OFF */
#define COM_DYN_IPDU_OFF FALSE


#if (defined COM_TX_DM_ON)      /* To prevent double declaration */
#error COM_TX_DM_ON already defined
#endif /* if (defined COM_TX_DM_ON) */

/** \brief Define COM_TX_DM_ON */
#define COM_TX_DM_ON TRUE


#if (defined COM_TX_DM_OFF)     /* To prevent double declaration */
#error COM_TX_DM_OFF already defined
#endif /* if (defined COM_TX_DM_OFF) */

/** \brief Define COM_TX_DM_OFF */
#define COM_TX_DM_OFF FALSE


#if (defined TM_TRUE)           /* To prevent double declaration */
#error TM_TRUE already defined
#endif /* if (defined TM_TRUE) */

/** \brief Define TM_TRUE */
#define TM_TRUE TRUE


#if (defined TM_FALSE)          /* To prevent double declaration */
#error TM_FALSE already defined
#endif /* if (defined TM_FALSE) */

/** \brief Define TM_FALSE */
#define TM_FALSE FALSE

#if (defined COM_TX_MODE_MODE_UNDEF)     /* To prevent double declaration */
#error COM_TX_MODE_MODE_UNDEF already defined
#endif /* if (defined COM_TX_MODE_MODE_UNDEF) */

/** \brief Define COM_TX_MODE_MODE_UNDEF */
#define COM_TX_MODE_MODE_UNDEF 0U

#if (defined COM_TX_MODE_MODE_NONE)     /* To prevent double declaration */
#error COM_TX_MODE_MODE_NONE already defined
#endif /* if (defined COM_TX_MODE_MODE_NONE) */

/** \brief Define COM_TX_MODE_MODE_NONE */
#define COM_TX_MODE_MODE_NONE 0

#if (defined COM_TX_MODE_MODE_PERIODIC) /* To prevent double declaration */
#error COM_TX_MODE_MODE_PERIODIC already defined
#endif /* if (defined COM_TX_MODE_MODE_PERIODIC) */

/** \brief Define COM_TX_MODE_MODE_PERIODIC */
#define COM_TX_MODE_MODE_PERIODIC 1U


#if (defined COM_TX_MODE_MODE_DIRECT)   /* To prevent double declaration */
#error COM_TX_MODE_MODE_DIRECT already defined
#endif /* if (defined COM_TX_MODE_MODE_DIRECT) */

/** \brief Define COM_TX_MODE_MODE_DIRECT */
#define COM_TX_MODE_MODE_DIRECT 2U


#if (defined COM_TX_MODE_MODE_MIXED_DIRECT)     /* To prevent double declaration */
#error COM_TX_MODE_MODE_MIXED_DIRECT already defined
#endif /* if (defined COM_TX_MODE_MODE_MIXED_DIRECT) */

/** \brief Define COM_TX_MODE_MODE_MIXED_DIRECT */
#define COM_TX_MODE_MODE_MIXED_DIRECT 3U


#if (defined COM_TX_MODE_MODE_MIXED_N_TIMES)    /* To prevent double declaration */
#error COM_TX_MODE_MODE_MIXED_N_TIMES already defined
#endif /* if (defined COM_TX_MODE_MODE_MIXED_N_TIMES) */

/** \brief Define COM_TX_MODE_MODE_MIXED_N_TIMES */
#define COM_TX_MODE_MODE_MIXED_N_TIMES 4U


#if (defined COM_TX_MODE_MODE_N_TIMES)  /* To prevent double declaration */
#error COM_TX_MODE_MODE_N_TIMES already defined
#endif /* if (defined COM_TX_MODE_MODE_N_TIMES) */

/** \brief Define COM_TX_MODE_MODE_N_TIMES */
#define COM_TX_MODE_MODE_N_TIMES 5U


#if (defined COM_INDEX_NONE)    /* To prevent double declaration */
#error COM_INDEX_NONE already defined
#endif /* if (defined COM_INDEX_NONE) */

/** \brief Define COM_INDEX_NONE */
#define COM_INDEX_NONE 0U


#if (defined COM_INDEX_UINT8)   /* To prevent double declaration */
#error COM_INDEX_UINT8 already defined
#endif /* if (defined COM_INDEX_UINT8) */

/** \brief Define COM_INDEX_UINT8 */
#define COM_INDEX_UINT8 0xFFU

#if (defined COM_INDEX_UINT16)  /* To prevent double declaration */
#error COM_INDEX_UINT16 already defined
#endif /* if (defined COM_INDEX_UINT16) */

/** \brief Define COM_INDEX_UINT16 */
#define COM_INDEX_UINT16 0xFFFFU

#if (defined COM_INDEX_UINT32)  /* To prevent double declaration */
#error COM_INDEX_UINT32 already defined
#endif /* if (defined COM_INDEX_UINT32) */

/** \brief Define COM_INDEX_UINT32 */
#define COM_INDEX_UINT32 0xFFFFFFFFU


#if (defined COM_INDEX_UINT8_PBCFG)     /* To prevent double declaration */
#error COM_INDEX_UINT8_PBCFG already defined
#endif /* if (defined COM_INDEX_UINT8_PBCFG) */

/** \brief Define COM_INDEX_UINT8_PBCFG */
#define COM_INDEX_UINT8_PBCFG 1U

#if (defined COM_INDEX_UINT16_PBCFG)    /* To prevent double declaration */
#error COM_INDEX_UINT16_PBCFG already defined
#endif /* if (defined COM_INDEX_UINT16_PBCFG) */

/** \brief Define COM_INDEX_UINT16_PBCFG */
#define COM_INDEX_UINT16_PBCFG 2U

#if (defined COM_INDEX_UINT32_PBCFG)    /* To prevent double declaration */
#error COM_INDEX_UINT32_PBCFG already defined
#endif /* if (defined COM_INDEX_UINT32_PBCFG) */

/** \brief Define COM_INDEX_UINT32_PBCFG */
#define COM_INDEX_UINT32_PBCFG 3U


#if (defined COM_RX_DATA_TOUT_ACTION_NONE)   /* To prevent double declaration */
#error COM_RX_DATA_TOUT_ACTION_NONE already defined
#endif /* if (defined COM_RX_DATA_TOUT_ACTION_NONE) */

/** \brief Define COM_RX_DATA_TOUT_ACTION_NONE (COM_RX_DATA_TIMEOUT_ACTION_NONE) */
#define COM_RX_DATA_TOUT_ACTION_NONE 0U

#if (defined COM_RX_DATA_TOUT_ACTION_REPLACE)        /* To prevent double declaration */
#error COM_RX_DATA_TOUT_ACTION_REPLACE already defined
#endif /* if (defined COM_RX_DATA_TOUT_ACTION_REPLACE) */

/** \brief Define COM_RX_DATA_TOUT_ACTION_REPLACE (COM_RX_DATA_TIMEOUT_ACTION_REPLACE) */
#define COM_RX_DATA_TOUT_ACTION_REPLACE 1U

#if (defined COM_RX_DATA_TOUT_ACTION_CONFIG) /* To prevent double declaration */
#error COM_RX_DATA_TOUT_ACTION_CONFIG already defined
#endif /* if (defined COM_RX_DATA_TOUT_ACTION_CONFIG) */

/** \brief Define COM_RX_DATA_TOUT_ACTION_CONFIG (COM_RX_DATA_TIMEOUT_ACTION_CONFIG) */
#define COM_RX_DATA_TOUT_ACTION_CONFIG 2U


#if (defined COM_UPDATE_BIT_ABSENT_FOR_ALL)     /* To prevent double declaration */
#error COM_UPDATE_BIT_ABSENT_FOR_ALL already defined
#endif /* #if (defined COM_UPDATE_BIT_ABSENT_FOR_ALL) */

/** \brief Define COM_UPDATE_BIT_ABSENT_FOR_ALL */
#define COM_UPDATE_BIT_ABSENT_FOR_ALL 0U

#if (defined COM_UPDATE_BIT_PRESENT_FOR_ALL)    /* To prevent double declaration */
#error COM_UPDATE_BIT_PRESENT_FOR_ALL already defined
#endif /* #if (defined COM_UPDATE_BIT_PRESENT_FOR_ALL) */

/** \brief Define COM_UPDATE_BIT_PRESENT_FOR_ALL */
#define COM_UPDATE_BIT_PRESENT_FOR_ALL 1U

#if (defined COM_UPDATE_BIT_INDIVIDUAL) /* To prevent double declaration */
#error COM_UPDATE_BIT_INDIVIDUAL already defined
#endif /* #if (defined COM_UPDATE_BIT_INDIVIDUAL) */

/** \brief Define COM_UPDATE_BIT_INDIVIDUAL */
#define COM_UPDATE_BIT_INDIVIDUAL 2U


#if (defined COM_UPDATE_BIT_ABSENT)     /* To prevent double declaration */
#error COM_UPDATE_BIT_ABSENT already defined
#endif /* #if (defined COM_UPDATE_BIT_ABSENT) */

/** \brief Define COM_UPDATE_BIT_ABSENT */
#define COM_UPDATE_BIT_ABSENT FALSE


#if (defined COM_UPDATE_BIT_PRESENT)    /* To prevent double declaration */
#error COM_UPDATE_BIT_PRESENT already defined
#endif /* #if (defined COM_UPDATE_BIT_PRESENT) */

/** \brief Define COM_UPDATE_BIT_PRESENT */
#define COM_UPDATE_BIT_PRESENT TRUE


#if (defined COM_TRANSFER_PPROPERTY_PENDING)    /* To prevent double declaration */
#error COM_TRANSFER_PPROPERTY_PENDING already defined
#endif /* if (defined COM_TRANSFER_PPROPERTY_PENDING) */

/** \brief Define COM_TRANSFER_PPROPERTY_PENDING */
#define COM_TRANSFER_PPROPERTY_PENDING 0U

#if (defined COM_TRANSFER_PPROPERTY_TRIG)  /* To prevent double declaration */
#error COM_TRANSFER_PPROPERTY_TRIG already defined
#endif /* if (defined COM_TRANSFER_PPROPERTY_TRIG) */


/** \brief Define COM_TRANSFER_PPROPERTY_TRIG (COM_TRANSFER_PPROPERTY_TRIGGERED) */
#define COM_TRANSFER_PPROPERTY_TRIG 1U


#if (defined COM_TRANSFER_PPROPERTY_SIG_TRIG_OC)  /* To prevent double declaration */
#error COM_TRANSFER_PPROPERTY_TRIG_OC already defined
#endif /* if (defined COM_TRANSFER_PPROPERTY_SIG_TRIG_OC) */

/** \brief Define COM_TRANSFER_PPROPERTY_SIG_TRIG_OC (COM_TRANSFER_PPROPERTY_TRIGGER_ON_CHANGE of a signal) */
#define COM_TRANSFER_PPROPERTY_SIG_TRIG_OC 1U


#if (defined COM_TRANSFER_PPROPERTY_SIGGRP_TRIG_OC)  /* To prevent double declaration */
#error COM_TRANSFER_PPROPERTY_TRIG_OC already defined
#endif /* if (defined COM_TRANSFER_PPROPERTY_TRIG_OC) */

/** \brief Define COM_TRANSFER_PPROPERTY_SIGGRP_TRIG_OC (COM_TRANSFER_PPROPERTY_TRIGGER_ON_CHANGE of a signal group) */
#define COM_TRANSFER_PPROPERTY_SIGGRP_TRIG_OC 2U


#if (defined COM_TRANSFER_PPROPERTY_TRIG_W_REP)  /* To prevent double declaration */
#error COM_TRANSFER_PPROPERTY_TRIG_W_REP already defined
#endif /* if (defined COM_TRANSFER_PPROPERTY_TRIG_W_REP) */

/** \brief Define COM_TRANSFER_PPROPERTY_TRIG_W_REP  */
#define COM_TRANSFER_PPROPERTY_TRIG_W_REP 0U


#if (defined COM_TRANSFER_PPROPERTY_TRIG_WO_REP)  /* To prevent double declaration */
#error COM_TRANSFER_PPROPERTY_TRIG_WO_REP already defined
#endif /* if (defined COM_TRANSFER_PPROPERTY_TRIG_WO_REP) */

/** \brief Define COM_TRANSFER_PPROPERTY_TRIG_WO_REP  */
#define COM_TRANSFER_PPROPERTY_TRIG_WO_REP 1U


#if (COM_PRECONDITION_ASSERT_ENABLED == STD_ON)

#if (defined COM_CONFIG_CHECK_RX_IPDU)  /* To prevent double declaration */
#error COM_CONFIG_CHECK_RX_IPDU already defined
#endif /* if (defined COM_CONFIG_CHECK_RX_IPDU) */

/** \brief Define COM_CONFIG_CHECK_RX_IPDU */
#define COM_CONFIG_CHECK_RX_IPDU 0xAAA0U

#if (defined COM_CONFIG_CHECK_TX_IPDU)  /* To prevent double declaration */
#error COM_CONFIG_CHECK_TX_IPDU already defined
#endif /* if (defined COM_CONFIG_CHECK_TX_IPDU) */

/** \brief Define COM_CONFIG_CHECK_TX_IPDU */
#define COM_CONFIG_CHECK_TX_IPDU (COM_CONFIG_CHECK_RX_IPDU+1U)

#if (defined COM_CONFIG_CHECK_TX_SIG_GROUP)  /* To prevent double declaration */
#error COM_CONFIG_CHECK_TX_SIG_GROUP already defined
#endif /* if (defined COM_CONFIG_CHECK_TX_SIG_GROUP) */

/** \brief Define COM_CONFIG_CHECK_TX_SIG_GROUP (COM_CONFIG_CHECK_TX_SIGNAL_GROUP) */
#define COM_CONFIG_CHECK_TX_SIG_GROUP (COM_CONFIG_CHECK_TX_IPDU+1U)

#if (defined COM_CONFIG_CHECK_RX_SIG_GROUP)  /* To prevent double declaration */
#error COM_CONFIG_CHECK_RX_SIG_GROUP already defined
#endif /* if (defined COM_CONFIG_CHECK_RX_SIG_GROUP) */

/** \brief Define COM_CONFIG_CHECK_RX_SIG_GROUP (COM_CONFIG_CHECK_RX_SIGNAL_GROUP) */
#define COM_CONFIG_CHECK_RX_SIG_GROUP (COM_CONFIG_CHECK_TX_SIG_GROUP+1U)

#if (defined COM_CONFIG_CHECK_TX_GROUP_SIG)  /* To prevent double declaration */
#error COM_CONFIG_CHECK_TX_GROUP_SIG already defined
#endif /* if (defined COM_CONFIG_CHECK_TX_GROUP_SIG) */

/** \brief Define COM_CONFIG_CHECK_TX_GROUP_SIG (COM_CONFIG_CHECK_TX_GROUP_SIGNAL) */
#define COM_CONFIG_CHECK_TX_GROUP_SIG (COM_CONFIG_CHECK_RX_SIG_GROUP+1U)

#if (defined COM_CONFIG_CHECK_RX_GROUP_SIG)  /* To prevent double declaration */
#error COM_CONFIG_CHECK_RX_GROUP_SIG already defined
#endif /* if (defined COM_CONFIG_CHECK_RX_GROUP_SIG) */

/** \brief Define COM_CONFIG_CHECK_RX_GROUP_SIG (COM_CONFIG_CHECK_RX_GROUP_SIGNAL) */
#define COM_CONFIG_CHECK_RX_GROUP_SIG (COM_CONFIG_CHECK_TX_GROUP_SIG+1U)

#if (defined COM_CONFIG_CHECK_RX_SIGNAL)        /* To prevent double declaration */
#error COM_CONFIG_CHECK_RX_SIGNAL already defined
#endif /* if (defined COM_CONFIG_CHECK_RX_SIGNAL) */

/** \brief Define COM_CONFIG_CHECK_RX_SIGNAL */
#define COM_CONFIG_CHECK_RX_SIGNAL (COM_CONFIG_CHECK_RX_GROUP_SIG+1U)

#if (defined COM_CONFIG_CHECK_TX_SIGNAL)        /* To prevent double declaration */
#error COM_CONFIG_CHECK_TX_SIGNAL already defined
#endif /* if (defined COM_CONFIG_CHECK_TX_SIGNAL) */

/** \brief Define COM_CONFIG_CHECK_TX_SIGNAL */
#define COM_CONFIG_CHECK_TX_SIGNAL (COM_CONFIG_CHECK_RX_SIGNAL+1U)

#if (defined COM_CONFIG_CHECK_GW_SOURCE_DESC)    /* To prevent double declaration */
#error COM_CONFIG_CHECK_GW_SOURCE_DESC already defined
#endif /* if (defined COM_CONFIG_CHECK_GW_SOURCE_DESC) */

/** \brief Define COM_CONFIG_CHECK_GW_SOURCE_DESC (COM_CONFIG_CHECK_GW_SOURCE_DESCRIPTION) */
#define COM_CONFIG_CHECK_GW_SOURCE_DESC (COM_CONFIG_CHECK_TX_SIGNAL+1U)

#if (defined COM_CONFIG_CHECK_GW_SOURCE)        /* To prevent double declaration */
#error COM_CONFIG_CHECK_GW_SOURCE already defined
#endif /* if (defined COM_CONFIG_CHECK_GW_SOURCE) */

/** \brief Define COM_CONFIG_CHECK_GW_SOURCE */
#define COM_CONFIG_CHECK_GW_SOURCE (COM_CONFIG_CHECK_GW_SOURCE_DESC+1U)

#if (defined COM_CONFIG_CHECK_GW_DEST_DESC)       /* To prevent double declaration */
#error COM_CONFIG_CHECK_GW_DEST_DESC already defined
#endif /* if (defined COM_CONFIG_CHECK_GW_DEST_DESC) */

/** \brief Define COM_CONFIG_CHECK_GW_DEST_DESC (COM_CONFIG_CHECK_GW_DESTINATION_DESCRIPTION) */
#define COM_CONFIG_CHECK_GW_DEST_DESC (COM_CONFIG_CHECK_GW_SOURCE+1U)

#if (defined COM_CONFIG_CHECK_GW_DESTINATION)   /* To prevent double declaration */
#error COM_CONFIG_CHECK_GW_DESTINATION already defined
#endif /* if (defined COM_CONFIG_CHECK_GW_DESTINATION) */

/** \brief Define COM_CONFIG_CHECK_GW_DESTINATION */
#define COM_CONFIG_CHECK_GW_DESTINATION (COM_CONFIG_CHECK_GW_DEST_DESC+1U)

#if (defined COM_CONFIG_CHECK_RX_SIG_EXT)  /* To prevent double declaration */
#error COM_CONFIG_CHECK_RX_SIG_EXT already defined
#endif /* if (defined COM_CONFIG_CHECK_RX_SIG_EXT) */

/** \brief Define COM_CONFIG_CHECK_RX_SIG_EXT (COM_CONFIG_CHECK_RX_SIG_EXTNAL) */
#define COM_CONFIG_CHECK_RX_SIG_EXT (COM_CONFIG_CHECK_GW_DESTINATION+1U)


#endif /* (COM_PRECONDITION_ASSERT_ENABLED == STD_ON) */



#if (defined COM_F_NEVER)       /* To prevent double declaration */
#error COM_F_NEVER already defined
#endif /* if (defined COM_F_NEVER) */

/** \brief definition COM_F_NEVER */
#define COM_F_NEVER FALSE

#if (defined COM_F_ALWAYS)      /* To prevent double declaration */
#error COM_F_ALWAYS already defined
#endif /* if (defined COM_F_ALWAYS) */

/** \brief definition COM_F_ALWAYS */
#define COM_F_ALWAYS TRUE

#if (defined COM_F_MASK_NEW_DIFFERS_MASK_OLD)       /* To prevent double declaration */
#error COM_F_MASK_NEW_DIFFERS_MASK_OLD already defined
#endif /* if (defined COM_F_MASK_NEW_DIFFERS_MASK_OLD) */

/** \brief definition COM_F_MASK_NEW_DIFFERS_MASK_OLD (COM_F_MASKED_NEW_DIFFERS_MASKED_OLD) */
#define COM_F_MASK_NEW_DIFFERS_MASK_OLD ((COM_F_ALWAYS+COM_F_NEVER)+1U)

#if (defined COM_F_MASKED_NEW_DIFFERS_X)        /* To prevent double declaration */
#error COM_F_MASKED_NEW_DIFFERS_X already defined
#endif /* if (defined COM_F_MASKED_NEW_DIFFERS_X) */

/** \brief definition COM_F_MASKED_NEW_DIFFERS_X */
#define COM_F_MASKED_NEW_DIFFERS_X (COM_F_MASK_NEW_DIFFERS_MASK_OLD+1U)

#if (defined COM_F_MASKED_NEW_EQUALS_X) /* To prevent double declaration */
#error COM_F_MASKED_NEW_EQUALS_X already defined
#endif /* if (defined COM_F_MASKED_NEW_EQUALS_X) */

/** \brief definition COM_F_MASKED_NEW_EQUALS_X */
#define COM_F_MASKED_NEW_EQUALS_X (COM_F_MASKED_NEW_DIFFERS_X+1U)

#if (defined COM_F_NEW_IS_OUTSIDE)      /* To prevent double declaration */
#error COM_F_NEW_IS_OUTSIDE already defined
#endif /* if (defined COM_F_NEW_IS_OUTSIDE) */

/** \brief definition COM_F_NEW_IS_OUTSIDE */
#define COM_F_NEW_IS_OUTSIDE (COM_F_MASKED_NEW_EQUALS_X+1U)

#if (defined COM_F_NEW_IS_WITHIN)       /* To prevent double declaration */
#error COM_F_NEW_IS_WITHIN already defined
#endif /* if (defined COM_F_NEW_IS_WITHIN) */

/** \brief definition COM_F_NEW_IS_WITHIN */
#define COM_F_NEW_IS_WITHIN (COM_F_NEW_IS_OUTSIDE+1U)

#if (defined COM_F_ONE_EVERY_N) /* To prevent double declaration */
#error COM_F_ONE_EVERY_N already defined
#endif /* if (defined COM_F_ONE_EVERY_N) */

/** \brief definition COM_F_ONE_EVERY_N */
#define COM_F_ONE_EVERY_N (COM_F_NEW_IS_WITHIN+1U)


#if (defined COM_F_NON_TRIVIAL) /* To prevent double declaration */
#error COM_F_NON_TRIVIAL already defined
#endif /* if (defined COM_F_ONE_EVERY_N) */

/** \brief definition COM_F_NON_TRIVIAL */
#define COM_F_NON_TRIVIAL(filterType) \
   (((filterType) > COM_F_ALWAYS) ? (TRUE) : (FALSE))


#if (defined COM_FLAG_PRESENT)  /* To prevent double declaration */
#error COM_FLAG_PRESENT already defined
#endif /* #if (defined COM_FLAG_PRESENT) */

/** \brief definition COM_FLAG_PRESENT */
#define COM_FLAG_PRESENT 0xFFU

#if (defined COM_FLAG_ABSENT)   /* To prevent double declaration */
#error COM_FLAG_ABSENT already defined
#endif /* #if (defined COM_FLAG_ABSENT) */

/** \brief definition COM_FLAG_ABSENT */
#define COM_FLAG_ABSENT 0U


#if (defined COM_RECEIVE_SIGNAL_GENERIC)  /* To prevent double declaration */
#error COM_RECEIVE_SIGNAL_GENERIC already defined
#endif /* #if (defined COM_RECEIVE_SIGNAL_GENERIC) */

/** \brief definition COM_RECEIVE_SIGNAL_GENERIC */
#define COM_RECEIVE_SIGNAL_GENERIC 0U


#if (defined COM_RECEIVE_SIGNAL_GENERATED)  /* To prevent double declaration */
#error COM_RECEIVE_SIGNAL_GENERATED already defined
#endif /* #if (defined COM_RECEIVE_SIGNAL_GENERATED) */

/** \brief definition COM_RECEIVE_SIGNAL_GENERATED */
#define COM_RECEIVE_SIGNAL_GENERATED 1U


#if (defined COM_CL_UB_TRANSMIT)  /* To prevent double declaration */
#error COM_CL_UB_TRANSMIT already defined
#endif /* #if (defined COM_CL_UB_TRANSMIT) */

/** \brief definition COM_CL_UB_TRANSMIT */
#define COM_CL_UB_TRANSMIT 0U


#if (defined COM_CL_UB_TRIGGERTRANSMIT)  /* To prevent double declaration */
#error COM_CL_UB_TRIGGERTRANSMIT already defined
#endif /* #if (defined COM_CL_UB_TRIGGERTRANSMIT) */

/** \brief definition COM_CL_UB_TRIGGERTRANSMIT */
#define COM_CL_UB_TRIGGERTRANSMIT 1U


#if (defined COM_CL_UB_CONFIRMATION)  /* To prevent double declaration */
#error COM_CL_UB_CONFIRMATION already defined
#endif /* #if (defined COM_CL_UB_CONFIRMATION) */

/** \brief definition COM_CL_UB_CONFIRMATION */
#define COM_CL_UB_CONFIRMATION 2U


#if (defined COM_CL_UB_NEVER)  /* To prevent double declaration */
#error COM_CL_UB_NEVER already defined
#endif /* #if (defined COM_CL_UB_NEVER) */

/** \brief definition COM_CL_UB_NEVER */
#define COM_CL_UB_NEVER 3U


#if (defined COM_TRUNCATE_DATA_NOTIFY_ALL)  /* To prevent double declaration */
#error COM_TRUNCATE_DATA_NOTIFY_ALL already defined
#endif /* #if (defined COM_TRUNCATE_DATA_NOTIFY_ALL) */

/** \brief definition COM_TRUNCATE_DATA_NOTIFY_ALL */
#define COM_TRUNCATE_DATA_NOTIFY_ALL 0U


#if (defined COM_TRUNCATE_DATA_NOTIFY_ONLY_RECEIVED)  /* To prevent double declaration */
#error COM_TRUNCATE_DATA_NOTIFY_ONLY_RECEIVED already defined
#endif /* #if (defined COM_TRUNCATE_DATA_NOTIFY_ONLY_RECEIVED) */

/** \brief definition COM_TRUNCATE_DATA_NOTIFY_ONLY_RECEIVED */
#define COM_TRUNCATE_DATA_NOTIFY_ONLY_RECEIVED 1U


#if (defined COM_COPY_ONLY_RECEIVED_DATA_NOTIFY_ALL)  /* To prevent double declaration */
#error COM_COPY_ONLY_RECEIVED_DATA_NOTIFY_ALL already defined
#endif /* #if (defined COM_COPY_ONLY_RECEIVED_DATA_NOTIFY_ALL) */

/** \brief definition COM_COPY_ONLY_RECEIVED_DATA_NOTIFY_ALL */
#define COM_COPY_ONLY_RECEIVED_DATA_NOTIFY_ALL 2U


#if (defined COM_COPY_ONLY_RECEIVED_DATA_NOTIFY_ONLY_RECEIVED)  /* To prevent double declaration */
#error COM_COPY_ONLY_RECEIVED_DATA_NOTIFY_ONLY_RECEIVED already defined
#endif /* #if (defined COM_COPY_ONLY_RECEIVED_DATA_NOTIFY_ONLY_RECEIVED) */

/** \brief definition COM_COPY_ONLY_RECEIVED_DATA_NOTIFY_ONLY_RECEIVED */
#define COM_COPY_ONLY_RECEIVED_DATA_NOTIFY_ONLY_RECEIVED 3U


#if (defined COM_TEST_TRUNCATE_DATA_NOTIFY_ONLY_RECEIVED)  /* To prevent double declaration */
#error COM_TEST_TRUNCATE_DATA_NOTIFY_ONLY_RECEIVED already defined
#endif /* #if (defined COM_TEST_TRUNCATE_DATA_NOTIFY_ONLY_RECEIVED) */

/** \brief definition COM_EB_TEST_TRUNCATE_DATA_NOTIFY_ONLY_RECEIVED */
#define COM_TEST_TRUNCATE_DATA_NOTIFY_ONLY_RECEIVED 4U


#if (defined COM_TEST_COPY_ONLY_RECEIVED_DATA_NOTIFY_ALL)  /* To prevent double declaration */
#error COM_TEST_COPY_ONLY_RECEIVED_DATA_NOTIFY_ALL already defined
#endif /* #if (defined COM_TEST_COPY_ONLY_RECEIVED_DATA_NOTIFY_ALL) */

/** \brief definition COM_TEST_COPY_ONLY_RECEIVED_DATA_NOTIFY_ALL */
#define COM_TEST_COPY_ONLY_RECEIVED_DATA_NOTIFY_ALL 5U


#if (defined COM_TEST_COPY_ONLY_RECEIVED_DATA_NOTIFY_ONLY_RECEIVED)  /* To prevent double declaration */
#error COM_TEST_COPY_ONLY_RECEIVED_DATA_NOTIFY_ONLY_RECEIVED already defined
#endif /* #if (defined COM_TEST_COPY_ONLY_RECEIVED_DATA_NOTIFY_ONLY_RECEIVED) */

/** \brief definition COM_TEST_COPY_ONLY_RECEIVED_DATA_NOTIFY_ONLY_RECEIVED */
#define COM_TEST_COPY_ONLY_RECEIVED_DATA_NOTIFY_ONLY_RECEIVED 6U


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /*COM_DEFS_H_ */
/*==================[end of file]===========================================*/

