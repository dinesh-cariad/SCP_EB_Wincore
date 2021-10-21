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
#ifndef COM_INT_TYPES_H_
#define COM_INT_TYPES_H_
/*==================[inclusions]============================================*/

#include <TSAutosar.h>          /* EB specific standard types */

#include <Com_Rules.h>
#include <ComStack_Types.h>     /* AUTOSAR Standard types */
#include <Com_Types.h>          /* Generated data types of the Com module */

/*==================[macros]================================================*/

#if (defined COM_REF2CONFIG)        /* To prevent double declaration */
#error COM_REF2CONFIG is already defined
#endif /* (defined COM_REF2CONFIG) */

/** \brief definition COM_REF2CONFIG(type)
 * macro used to define define a configuration structure element as
 * a relative offset
 */
#if (COM_RELOCATABLE_CFG_ENABLE == STD_ON)
#define COM_REF2CONFIG(type) \
    CONST(Com_ConfigOffsetType, AUTOMATIC)
#elif (COM_RELOCATABLE_CFG_ENABLE == STD_OFF)
#define COM_REF2CONFIG(type) \
    P2CONST(type, AUTOMATIC, COM_APPL_CONST)
#endif /* (COM_RELOCATABLE_CFG_ENABLE == STD_ON) */



#if (defined COM_CBK_TX_T_OUT_IDX_FUNC_PTR_NULL)        /* To prevent double declaration */
#error COM_CBK_TX_T_OUT_IDX_FUNC_PTR_NULL already defined
#endif /* if (defined COM_CBK_TX_T_OUT_IDX_FUNC_PTR_NULL) */

/** \brief definition COM_CBK_TX_T_OUT_IDX_FUNC_PTR_NULL */
#define COM_CBK_TX_T_OUT_IDX_FUNC_PTR_NULL COM_CBK_TX_T_OUT_ARRAY_SIZE_MAX


#if (defined COM_CBK_RX_T_OUT_IDX_FUNC_PTR_NULL)        /* To prevent double declaration */
#error COM_CBK_RX_T_OUT_IDX_FUNC_PTR_NULL already defined
#endif /* if (defined COM_CBK_RX_T_OUT_IDX_FUNC_PTR_NULL) */

/** \brief definition COM_CBK_RX_T_OUT_IDX_FUNC_PTR_NULL */
#define COM_CBK_RX_T_OUT_IDX_FUNC_PTR_NULL COM_CBK_RX_T_OUT_ARRAY_SIZE_MAX


#if (defined COM_CALL_OUT_FUNC_PTR_NULL)        /* To prevent double declaration */
#error COM_CALL_OUT_FUNC_PTR_NULL already defined
#endif /* if (defined COM_CALL_OUT_FUNC_PTR_NULL) */

/** \brief definition COM_CALL_OUT_FUNC_PTR_NULL */
#define COM_CALL_OUT_FUNC_PTR_NULL COM_CALL_OUT_FUNC_PTR_ARRAY_SIZE_MAX


#if (defined COM_CBK_TX_ACK_FUNC_PTR_NULL)        /* To prevent double declaration */
#error COM_CBK_TX_ACK_FUNC_PTR_NULL already defined
#endif /* if (defined COM_CBK_TX_ACK_FUNC_PTR_NULL) */

/** \brief definition of the COM_CBK_TX_ACK_FUNC_PTR_NULL */
#define COM_CBK_TX_ACK_FUNC_PTR_NULL COM_CBK_TX_ACK_PTR_ARRAY_SIZE_MAX


#if (defined COM_CBK_RX_ACK_FUNC_PTR_NULL)        /* To prevent double declaration */
#error COM_CBK_RX_ACK_FUNC_PTR_NULL already defined
#endif /* if (defined COM_CBK_RX_ACK_FUNC_PTR_NULL) */

/** \brief definition of the COM_CBK_RX_ACK_FUNC_PTR_NULL */
#define COM_CBK_RX_ACK_FUNC_PTR_NULL COM_CBK_RX_ACK_PTR_ARRAY_SIZE_MAX


#if (defined COM_IPDU_INVALID)        /* To prevent double declaration */
#error COM_IPDU_INVALID already defined
#endif /* if (defined COM_IPDU_INVALID) */

/** \brief definition of the COM_IPDU_INVALID */
#define COM_IPDU_INVALID COM_INDEX_UINT16

#if (defined COM_SIGNAL_BUFFER_REF_INVALID)        /* To prevent double declaration */
#error COM_SIGNAL_BUFFER_REF_INVALID already defined
#endif /* if (defined COM_SIGNAL_BUFFER_REF_INVALID) */

/** \brief definition of the COM_SIGNAL_BUFFER_REF_INVALID */
#define COM_SIGNAL_BUFFER_REF_INVALID COM_INDEX_UINT16


#if (defined COM_SIGNAL_INVALID)        /* To prevent double declaration */
#error COM_SIGNAL_INVALID already defined
#endif /* if (defined COM_SIGNAL_INVALID) */

/** \brief definition of the COM_SIGNAL_INVALID */
#define COM_SIGNAL_INVALID COM_INDEX_UINT16


#if (defined COM_TX_MODE_INVALID)        /* To prevent double declaration */
#error COM_TX_MODE_INVALID already defined
#endif /* if (defined COM_TX_MODE_INVALID) */

#if (COM_TM_AVAILABLE == STD_ON)
/** \brief definition of the COM_TX_MODE_INVALID */
#define COM_TX_MODE_INVALID COM_INDEX_UINT16
#endif /* #if (COM_TM_AVAILABLE == STD_ON) */


#if (defined COM_SIGNAL_GROUP_INVALID)        /* To prevent double declaration */
#error COM_SIGNAL_GROUP_INVALID already defined
#endif /* if (defined COM_SIGNAL_GROUP_INVALID) */

/** \brief definition of the COM_SIGNAL_GROUP_INVALID */
#define COM_SIGNAL_GROUP_INVALID COM_INDEX_UINT16

#if (defined COM_RX_DM_QUEUE_SIZE)        /* To prevent double declaration */
#error COM_RX_DM_QUEUE_SIZE is already defined
#endif /* (defined COM_RX_DM_QUEUE_SIZE) */

#if (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE > COM_RX_TIMEOUT_FACTOR_SIZE)
#define COM_RX_DM_QUEUE_SIZE COM_RX_FIRST_TIMEOUT_FACTOR_SIZE
#else
#define COM_RX_DM_QUEUE_SIZE COM_RX_TIMEOUT_FACTOR_SIZE
#endif /* (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE > COM_RX_TIMEOUT_FACTOR_SIZE) */

#if (defined COM_RX_DM_SIGNAL_QUEUE_SIZE)        /* To prevent double declaration */
#error COM_RX_DM_SIGNAL_QUEUE_SIZE is already defined
#endif /* (defined COM_RX_DM_SIGNAL_QUEUE_SIZE) */

#if (COM_RX_SIGNAL_BASED_DM == STD_ON)
#define COM_RX_DM_SIGNAL_QUEUE_SIZE COM_RX_DM_QUEUE_SIZE
#else
#define COM_RX_DM_SIGNAL_QUEUE_SIZE 0U
#endif /* (COM_RX_SIGNAL_BASED_DM == STD_ON) */


#if (defined COM_GLOBAL_TIME)   /* To prevent double declaration */
#error COM_GLOBAL_TIME already defined
#endif /* (defined COM_GLOBAL_TIME) */

/** \brief definition of COM_GLOBAL_TIME */
#if ((COM_TX_MODE_TIME_PERIOD_FACTOR_SIZE == COM_SIZE_32_BIT) || \
      (COM_TX_MODE_TIME_OFFSET_FACTOR_SIZE == COM_SIZE_32_BIT) || \
      (COM_TX_MODE_REPETITION_PERIOD_FACTOR_SIZE == COM_SIZE_32_BIT) || \
      ((COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) && (COM_RX_DM_QUEUE_SIZE == COM_SIZE_32_BIT)) || \
      ((COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) && (COM_TX_TIMEOUT_FACTOR_SIZE == COM_SIZE_32_BIT)) || \
      ((COM_MDT_QUEUE_AVAILABLE == STD_ON) && (COM_TX_IPDU_MINIMUM_DELAY_TIME_FACTOR_SIZE == COM_SIZE_32_BIT)))
#define COM_GLOBAL_TIME COM_SIZE_32_BIT
#elif ((COM_TX_MODE_TIME_PERIOD_FACTOR_SIZE == COM_SIZE_16_BIT) || \
      (COM_TX_MODE_TIME_OFFSET_FACTOR_SIZE == COM_SIZE_16_BIT) || \
      (COM_TX_MODE_REPETITION_PERIOD_FACTOR_SIZE == COM_SIZE_16_BIT) || \
      ((COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) && (COM_RX_DM_QUEUE_SIZE == COM_SIZE_16_BIT)) || \
      ((COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) && (COM_TX_TIMEOUT_FACTOR_SIZE == COM_SIZE_16_BIT)) || \
      ((COM_MDT_QUEUE_AVAILABLE == STD_ON) && (COM_TX_IPDU_MINIMUM_DELAY_TIME_FACTOR_SIZE == COM_SIZE_16_BIT)))
#define COM_GLOBAL_TIME COM_SIZE_16_BIT
#elif ((COM_TX_MODE_TIME_PERIOD_FACTOR_SIZE == COM_SIZE_8_BIT) || \
      (COM_TX_MODE_TIME_OFFSET_FACTOR_SIZE == COM_SIZE_8_BIT) || \
      (COM_TX_MODE_REPETITION_PERIOD_FACTOR_SIZE == COM_SIZE_8_BIT) || \
      ((COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) && (COM_RX_DM_QUEUE_SIZE == COM_SIZE_8_BIT)) || \
      ((COM_TX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) && (COM_TX_TIMEOUT_FACTOR_SIZE == COM_SIZE_8_BIT)) || \
      ((COM_MDT_QUEUE_AVAILABLE == STD_ON) && (COM_TX_IPDU_MINIMUM_DELAY_TIME_FACTOR_SIZE == COM_SIZE_8_BIT)))
#define COM_GLOBAL_TIME COM_SIZE_8_BIT
#else
#define COM_GLOBAL_TIME COM_SIZE_0_BIT
#endif



#if (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON)

#if (defined COM_PERIODIC_QUEUE_SIZE)   /* To prevent double declaration */
#error COM_PERIODIC_QUEUE_SIZE already defined
#endif /* (defined COM_PERIODIC_QUEUE_SIZE) */

/** \brief definition of the Com_PeriodicSendNextDueTimeType */
#if (COM_TX_MODE_TIME_PERIOD_FACTOR_SIZE != COM_SIZE_0_BIT)
#if (COM_TX_MODE_TIME_PERIOD_FACTOR_SIZE > COM_TX_MODE_TIME_OFFSET_FACTOR_SIZE)
typedef ComTxModeTimePeriodFactorType Com_PeriodicSendNextDueTimeType;
#define COM_PERIODIC_QUEUE_SIZE COM_TX_MODE_TIME_PERIOD_FACTOR_SIZE
#else
typedef ComTxModeTimeOffsetFactorType Com_PeriodicSendNextDueTimeType;
#define COM_PERIODIC_QUEUE_SIZE COM_TX_MODE_TIME_OFFSET_FACTOR_SIZE
#endif /* (COM_TX_MODE_TIME_PERIOD_FACTOR_SIZE > COM_TX_MODE_TIME_OFFSET_FACTOR_SIZE) */
#else
#define COM_PERIODIC_QUEUE_SIZE COM_SIZE_0_BIT
#endif /* (COM_TX_MODE_TIME_PERIOD_FACTOR_SIZE == COM_SIZE_0_BIT) */

#endif /* (COM_PERIODIC_QUEUE_AVAILABLE == STD_ON) */


/** \brief Definition of constant COM_EB_UNINIT */
#if (defined COM_EB_UNINIT)        /* To prevent double declaration */
#error COM_EB_UNINIT already defined
#endif /* if (defined COM_EB_UNINIT) */

/** \brief Define COM_EB_UNINIT */
#define COM_EB_UNINIT 0U

/** \brief Definition of constant COM_EB_INIT */
#if (defined COM_EB_INIT)          /* To prevent double declaration */
#error COM_EB_INIT already defined
#endif /* if (defined COM_EB_INIT) */

/** \brief Define COM_EB_INIT */
#define COM_EB_INIT 1U


/*==================[type definitions]======================================*/

/** \brief definition of the Com_ConfigDataRefType, compiler-abstracted */
typedef P2CONST(void, AUTOMATIC, COM_APPL_CONST) Com_ConfigDataRefType;

/** \brief definition of the Com_TS_SignalTypeType */
typedef uint8 Com_TS_SignalTypeType;

/** \brief definition of the Com_TS_SignalEndiannessType */
typedef uint8 Com_TS_SignalEndiannessType;

/** \brief definition of the Com_IpduGroupModeType */
typedef uint8 Com_IpduGroupModeType;

#if (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON)

#if (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE > COM_RX_TIMEOUT_FACTOR_SIZE)
typedef ComRxFirstTimeoutFactorType Com_RxDMNextDueTimeType;
#else
typedef ComRxTimeoutFactorType Com_RxDMNextDueTimeType;
#endif /* (COM_RX_FIRST_TIMEOUT_FACTOR_SIZE > COM_RX_TIMEOUT_FACTOR_SIZE) */

#endif /* (COM_RX_TIMEOUT_FACTOR_AVAILABLE == STD_ON) */

/* CHECK: RULE 505 OFF */
/* Check_C disabled. ComGlobalTimeType_Tx and ComGlobalTimeType_Rx are not renamed because of readability. */
/** \brief definition of the ComGlobalTimeType */
#if (COM_GLOBAL_TIME == COM_SIZE_0_BIT)
#elif (COM_GLOBAL_TIME == COM_SIZE_8_BIT)
#if (COM_TIMEBASE == STD_ON)
typedef uint8 ComGlobalTimeType;
#else
typedef uint8 ComGlobalTimeType_Tx[COM_MAIN_FUNCS_TX];
typedef uint8 ComGlobalTimeType_Rx[COM_MAIN_FUNCS_RX];
#endif /* (COM_TIMEBASE == STD_ON) */
#elif (COM_GLOBAL_TIME == COM_SIZE_16_BIT)
#if (COM_TIMEBASE == STD_ON)
typedef uint16 ComGlobalTimeType;
#else
typedef uint16 ComGlobalTimeType_Tx[COM_MAIN_FUNCS_TX];
typedef uint16 ComGlobalTimeType_Rx[COM_MAIN_FUNCS_RX];
#endif /* (COM_TIMEBASE == STD_ON) */
#elif (COM_GLOBAL_TIME == COM_SIZE_32_BIT)
#if (COM_TIMEBASE == STD_ON)
typedef uint32 ComGlobalTimeType;
#else
typedef uint32 ComGlobalTimeType_Tx[COM_MAIN_FUNCS_TX];
typedef uint32 ComGlobalTimeType_Rx[COM_MAIN_FUNCS_RX];
#endif /* (COM_TIMEBASE == STD_ON) */
#endif
/* CHECK: RULE 505 ON */

/** \brief definition of the Com_EB_StatusType */
typedef uint8 Com_EB_StatusType;


/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /*COM_INT_TYPES_H_ */
/*==================[end of file]===========================================*/

