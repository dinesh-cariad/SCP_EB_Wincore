/**
 * \file
 *
 * \brief AUTOSAR LinIf
 *
 * This file contains the implementation of the AUTOSAR
 * module LinIf.
 *
 * \version 5.8.27
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */


#ifndef LINIF_INT_H
#define LINIF_INT_H

/*==================[inclusions]=============================================*/

#include <TSAutosar.h>             /* EB specific standard types */
#include <ComStack_Types.h>        /* AUTOSAR standard types */
#include <LinIf_Cfg.h>             /* module static configuration */
#include <LinIf_Types.h>           /* LinIf basic types */

#define TS_RELOCATABLE_CFG_ENABLE LINIF_RELOCATABLE_CFG_ENABLE

#include <TSPBConfig_Access.h>
#include <TSPBConfig_Signature.h>  /* post-build macros */


/*==================[macros]=================================================*/

/** \brief Definition of uninit protection */
#if (defined LINIF_UNINIT_ACCESS)
#error LINIF_UNINIT_ACCESS already defined!
#endif /* #if (defined LINIF_UNINIT_ACCESS)*/
#define LINIF_UNINIT_ACCESS \
((LinIf_InitState != LINIF_INIT) || (LinIf_ConfigPtr == NULL_PTR))

#if (defined LINIF_FRAME_DEMEVENTS_NOT_AVAILABLE)
#error LINIF_FRAME_DEMEVENTS_NOT_AVAILABLE already defined!
#endif /* #if( defined LINIF_FRAME_DEMEVENTS_NOT_AVAILABLE)*/
#define LINIF_FRAME_DEMEVENTS_NOT_AVAILABLE 65535U

#if (defined LINIF_INVALID_DEM_EVENT_ID)
#error LINIF_INVALID_DEM_EVENT_ID already defined!
#endif /* #if( defined LINIF_INVALID_DEM_EVENT_ID)*/
#define LINIF_INVALID_DEM_EVENT_ID 0U

/* \brief If any DEM event is switched on */
#if (defined LINIF_DEM_EVENTS_CONFIGURED)
#error LINIF_DEM_EVENTS_CONFIGURED already defined!
#endif /* #if (defined LINIF_DEM_EVENTS_CONFIGURED)*/
#define LINIF_DEM_EVENTS_CONFIGURED \
((LINIF_PROD_ERR_HANDLING_TX_BIT == TS_PROD_ERR_REP_TO_DEM) || (LINIF_PROD_ERR_HANDLING_RX_CHECKSUM == TS_PROD_ERR_REP_TO_DEM) || (LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE == TS_PROD_ERR_REP_TO_DEM))

 /* \brief If any optional error status is switched on*/
#if (defined LINIF_REPORTTODEM_ENABLED)
#error LINIF_REPORTTODEM_ENABLED already defined!
#endif /* #if (defined LINIF_REPORTTODEM_ENABLED)*/
#define LINIF_REPORTTODEM_ENABLED \
((LINIF_PROD_ERR_HANDLING_TX_BIT != TS_PROD_ERR_DISABLE) || (LINIF_PROD_ERR_HANDLING_RX_CHECKSUM != TS_PROD_ERR_DISABLE) || (LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE != TS_PROD_ERR_DISABLE))

  /** \brief Resume position
 * LINIF_START_FROM_BEGINNING  - Start from the beginning
 * LINIF_CONTINUE_AT_IT_POINT  - Continue at IT Point
 * */
#if (defined LINIF_START_FROM_BEGINNING)
#error LINIF_START_FROM_BEGINNING already defined!
#endif /* #if (defined LINIF_START_FROM_BEGINNING)*/
#define  LINIF_START_FROM_BEGINNING     0U
#if (defined LINIF_CONTINUE_AT_IT_POINT)
#error LINIF_CONTINUE_AT_IT_POINT already defined!
#endif /* #if (defined LINIF_CONTINUE_AT_IT_POINT)*/
#define  LINIF_CONTINUE_AT_IT_POINT     1U


/* API service IDs */
#if (defined LINIF_API_INIT)
#error LINIF_API_INIT already defined!
#endif /* #if (defined LINIF_API_INIT)*/
#define LINIF_API_INIT              0x01U

#if (defined LINIF_API_GETVERSIONINFO)
#error LINIF_API_GETVERSIONINFO already defined!
#endif /* #if (defined LINIF_API_GETVERSIONINFO)*/
#define LINIF_API_GETVERSIONINFO    0x03U

#if (defined LINIF_API_TRANSMIT)
#error LINIF_API_TRANSMIT already defined!
#endif /* #if (defined LINIF_API_TRANSMIT)*/
#define LINIF_API_TRANSMIT          0x04U

#if (defined LINIF_API_SCHEDULEREQUEST)
#error LINIF_API_SCHEDULEREQUEST already defined!
#endif /* #if (defined LINIF_API_SCHEDULEREQUEST)*/
#define LINIF_API_SCHEDULEREQUEST   0x05U

#if (defined LINIF_API_GOTOSLEEP)
#error LINIF_API_GOTOSLEEP already defined!
#endif /* #if (defined LINIF_API_GOTOSLEEP)*/
#define LINIF_API_GOTOSLEEP         0x06U

#if (defined LINIF_API_WAKEUP)
#error LINIF_API_WAKEUP already defined!
#endif /* #if (defined LINIF_API_WAKEUP)*/
#define LINIF_API_WAKEUP            0x07U

#if (defined LINIF_API_SETTRCVMODE)
#error LINIF_API_SETTRCVMODE already defined!
#endif /* #if (defined LINIF_API_SETTRCVMODE)*/
#define LINIF_API_SETTRCVMODE       0x08U

#if (defined LINIF_API_GETTRCVMODE)
#error LINIF_API_GETTRCVMODE already defined!
#endif /* #if (defined LINIF_API_GETTRCVMODE)*/
#define LINIF_API_GETTRCVMODE       0x09U

#if (defined LINIF_API_GETTRCVWUREASON)
#error LINIF_API_GETTRCVWUREASON already defined!
#endif /* #if (defined LINIF_API_GETTRCVWUREASON)*/
#define LINIF_API_GETTRCVWUREASON   0x0aU

#if (defined LINIF_API_SETTRCVWUMODE)
#error LINIF_API_SETTRCVWUMODE already defined!
#endif /* #if (defined LINIF_API_SETTRCVWUMODE)*/
#define LINIF_API_SETTRCVWUMODE     0x0bU

#if (defined LINIF_API_CHECKWAKEUP)
#error LINIF_API_CHECKWAKEUP already defined!
#endif /* #if (defined LINIF_API_CHECKWAKEUP)*/
#define LINIF_API_CHECKWAKEUP       0x60U

#if (defined LINIF_API_WAKEUPCONF)
#error LINIF_API_WAKEUPCONF already defined!
#endif /* #if (defined LINIF_API_WAKEUPCONF)*/
#define LINIF_API_WAKEUPCONF        0x61U

#if (defined LINIF_API_HEADERIND)
#error LINIF_API_HEADERIND already defined!
#endif /* #if (defined LINIF_API_HEADERIND)*/
#define LINIF_API_HEADERIND         0x78U

#if (defined LINIF_API_RXIND)
#error LINIF_API_RXIND already defined!
#endif /* #if (defined LINIF_API_RXIND)*/
#define LINIF_API_RXIND             0x79U

#if (defined LINIF_API_TXCONF)
#error LINIF_API_TXCONF already defined!
#endif /* #if (defined LINIF_API_TXCONF)*/
#define LINIF_API_TXCONF            0x7aU

#if (defined LINIF_API_LINERRORIND)
#error LINIF_API_LINERRORIND already defined!
#endif /* #if (defined LINIF_API_LINERRORIND)*/
#define LINIF_API_LINERRORIND       0x7bU

#if (defined LINIF_API_GETPIDTABLE)
#error LINIF_API_GETPIDTABLE already defined!
#endif /* #if (defined LINIF_API_GETPIDTABLE)*/
#define LINIF_API_GETPIDTABLE       0x72U

#if (defined LINIF_API_SETPIDTABLE)
#error LINIF_API_SETPIDTABLE already defined!
#endif /* #if (defined LINIF_API_SETPIDTABLE)*/
#define LINIF_API_SETPIDTABLE       0x73U

#if (defined LINIF_API_GETCONFNAD)
#error LINIF_API_GETCONFNAD already defined!
#endif /* #if (defined LINIF_API_GETCONFNAD)*/
#define LINIF_API_GETCONFNAD        0x70U

#if (defined LINIF_API_SETCONFNAD)
#error LINIF_API_SETCONFNAD already defined!
#endif /* #if (defined LINIF_API_SETCONFNAD)*/
#define LINIF_API_SETCONFNAD        0x71U

#if (defined LINIF_API_ENABUSMIRRORING)
#error LINIF_API_ENABUSMIRRORING already defined!
#endif /* #if (defined LINIF_API_ENABUSMIRRORING)*/
#define LINIF_API_ENABUSMIRRORING   0x7fU

#if (defined LINIF_API_MAINFUNCTION)
#error LINIF_API_MAINFUNCTION already defined!
#endif /* #if (defined LINIF_API_MAINFUNCTION)*/
#define LINIF_API_MAINFUNCTION      0x80U

#if (defined LINIF_API_ISVALIDCONFIG)
#error LINIF_API_ISVALIDCONFIG already defined!
#endif /* #if (defined LINIF_API_ISVALIDCONFIG)*/
#define LINIF_API_ISVALIDCONFIG     0x62U

/* DET macros */
#if (defined LINIF_E_UNINIT)
#error LINIF_E_UNINIT already defined!
#endif /* #if (defined LINIF_E_UNINIT)*/
#define LINIF_E_UNINIT                  0x00U

#if (defined LINIF_E_ALREADY_INITIALIZED)
#error LINIF_E_ALREADY_INITIALIZED already defined!
#endif /* #if (defined LINIF_E_ALREADY_INITIALIZED)*/
#define LINIF_E_ALREADY_INITIALIZED     0x10U

/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_E_NONEXISTENT_CHANNEL,1 */
#if (defined LINIF_E_NONEXISTENT_CHANNEL)
#error LINIF_E_NONEXISTENT_CHANNEL already defined!
#endif /* #if (defined LINIF_E_NONEXISTENT_CHANNEL)*/
#define LINIF_E_NONEXISTENT_CHANNEL     0x20U

/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_E_PARAMETER,1 */
#if (defined LINIF_E_PARAMETER)
#error LINIF_E_PARAMETER already defined!
#endif /* #if (defined LINIF_E_PARAMETER)*/
#define LINIF_E_PARAMETER               0x30U

/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_E_PARAMETER_POINTER,1 */
#if (defined LINIF_E_PARAMETER_POINTER)
#error LINIF_E_PARAMETER_POINTER already defined!
#endif /* #if (defined LINIF_E_PARAMETER_POINTER)*/
#define LINIF_E_PARAMETER_POINTER       0x40U

#if (defined LINIF_E_PARAM_POINTER)
#error LINIF_E_PARAM_POINTER already defined!
#endif /* #if (defined LINIF_E_PARAM_POINTER)*/
#define LINIF_E_PARAM_POINTER           LINIF_E_PARAMETER_POINTER

#if (defined LINIF_E_SCHEDULE_OVERFLOW)
#error LINIF_E_SCHEDULE_OVERFLOW already defined!
#endif /* #if (defined LINIF_E_SCHEDULE_OVERFLOW)*/
#define LINIF_E_SCHEDULE_OVERFLOW       0x50U

/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_E_SCHEDULE_REQUEST_ERROR,1 */
#if (defined LINIF_E_SCHEDULE_REQUEST_ERROR)
#error LINIF_E_SCHEDULE_REQUEST_ERROR already defined!
#endif /* #if (defined LINIF_E_SCHEDULE_REQUEST_ERROR)*/
#define LINIF_E_SCHEDULE_REQUEST_ERROR  0x51U

/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_E_TRCV_INV_CHANNEL,1 */
#if (defined LINIF_E_TRCV_INV_CHANNEL)
#error LINIF_E_TRCV_INV_CHANNEL already defined!
#endif /* #if (defined LINIF_E_TRCV_INV_CHANNEL)*/
#define LINIF_E_TRCV_INV_CHANNEL  0x52U

/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_ELINIF_E_TRCV_INV_MODE,1 */
#if (defined LINIF_E_TRCV_INV_MODE)
#error LINIF_E_TRCV_INV_MODE already defined!
#endif /* #if (defined LINIF_E_TRCV_INV_MODE)*/
#define LINIF_E_TRCV_INV_MODE  0x53U

/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_E_TRCV_NOT_NORMAL,1 */
#if (defined LINIF_E_TRCV_NOT_NORMAL)
#error LINIF_E_TRCV_NOT_NORMAL already defined!
#endif /* #if (defined LINIF_E_TRCV_NOT_NORMAL)*/
#define LINIF_E_TRCV_NOT_NORMAL  0x54U

/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_E_PARAM_WAKEUPSOURCE,1 */
#if (defined LINIF_E_PARAM_WAKEUPSOURCE)
#error LINIF_E_PARAM_WAKEUPSOURCE already defined!
#endif /* #if (defined LINIF_E_PARAM_WAKEUPSOURCE)*/
#define LINIF_E_PARAM_WAKEUPSOURCE      0x55U

/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00729.LINIF_E_RESPONSE,1 */
/** \brief Definition of LINIF_E_RESPONSE  */
#if (defined LINIF_E_RESPONSE)
#error LINIF_E_RESPONSE already defined!
#endif /* #if (defined LINIF_E_RESPONSE)*/
#define LINIF_E_RESPONSE                0x60U

/** \brief Definition of LINIF_E_NC_NO_RESPONSE  */
#if (defined LINIF_E_NC_NO_RESPONSE)
#error LINIF_E_NC_NO_RESPONSE already defined!
#endif /* #if (defined LINIF_E_NC_NO_RESPONSE)*/
#define LINIF_E_NC_NO_RESPONSE          0x61U

#if (defined LINIF_E_INVALID_MIRROR_CHANNEL)
#error LINIF_E_INVALID_MIRROR_CHANNEL already defined!
#endif /* #if (defined LINIF_E_INVALID_MIRROR_CHANNEL)*/
#define LINIF_E_INVALID_MIRROR_CHANNEL     0x70U

/* !LINKSTO LinIf.ASR20-11.SWS_LinIf_00376_1.LINIF_DRIVER_CHANNEL_NOT_IN_SLEEP,1 */
/** \brief Definition of LINIF_DRIVER_CHANNEL_NOT_IN_SLEEP  */
#if (defined LINIF_DRIVER_CHANNEL_NOT_IN_SLEEP)
#error LINIF_DRIVER_CHANNEL_NOT_IN_SLEEP already defined!
#endif /* #if (defined LINIF_DRIVER_CHANNEL_NOT_IN_SLEEP)*/
#define LINIF_DRIVER_CHANNEL_NOT_IN_SLEEP       0xFFU

/** \brief Instance Id for DET 2.0 (always 0, because only one
 **        LinIf-Instance allowed)
*/
#if (defined LINIF_INSTANCE_ID)
#error LINIF_INSTANCE_ID already defined!
#endif /* #if (defined LINIF_INSTANCE_ID)*/
#define LINIF_INSTANCE_ID 0U

/** \brief mask for extracting the checksum type */
#if (defined LINIF_CS_MASK)
#error LINIF_CS_MASK already defined!
#endif /* #if (defined LINIF_CS_MASK)*/
#define LINIF_CS_MASK       0x10U

/** \brief mask for extracting the data length */
#if (defined LINIF_DL_MASK)
#error LINIF_DL_MASK already defined!
#endif /* #if (defined LINIF_DL_MASK)*/
#define LINIF_DL_MASK       0x0FU

/** \brief mask for extracting StartupState */
#if (defined LINIF_SUSTATE_MASK)
#error LINIF_SUSTATE_MASK already defined!
#endif /* #if (defined LINIF_SUSTATE_MASK)*/
#define LINIF_SUSTATE_MASK       0x0FU

/** \brief mask for extracting SerialNumber support */
#if (defined LINIF_SERNUM_MASK)
#error LINIF_SERNUM_MASK already defined!
#endif /* #if (defined LINIF_SERNUM_MASK)*/
#define LINIF_SERNUM_MASK        0x10U

/* state macros for LinIf global FSM */
#if (defined LINIF_UNINIT)
#error LINIF_UNINIT already defined!
#endif /* #if (defined LINIF_UNINIT)*/
#define LINIF_UNINIT                0U

#if (defined LINIF_INIT)
#error LINIF_INIT already defined!
#endif /* #if (defined LINIF_INIT)*/
#define LINIF_INIT                  1U

/* state macros for LinIf channel FSM */
#if (defined LINIF_CHANNEL_OPERATIONAL)
#error LINIF_CHANNEL_OPERATIONAL already defined!
#endif /* #if (defined LINIF_CHANNEL_OPERATIONAL)*/
#define LINIF_CHANNEL_OPERATIONAL   1U

#if (defined LINIF_CHANNEL_SLEEP)
#error LINIF_CHANNEL_SLEEP already defined!
#endif /* #if (defined LINIF_CHANNEL_SLEEP)*/
#define LINIF_CHANNEL_SLEEP         2U

/* values and flags for the SleepHandling variable */

/** \brief No sleep request and wakeup is pending (default value) */
#if (defined LINIF_NO_SLEEP_REQUEST)
#error LINIF_NO_SLEEP_REQUEST already defined!
#endif /* #if (defined LINIF_NO_SLEEP_REQUEST)*/
#define LINIF_NO_SLEEP_REQUEST               0x00U

/** \brief Flag to indicate a pending sleep request */
#if (defined LINIF_SLEEP_REQUESTED_FLAG)
#error LINIF_SLEEP_REQUESTED_FLAG already defined!
#endif /* #if (defined LINIF_SLEEP_REQUESTED_FLAG)*/
#define LINIF_SLEEP_REQUESTED_FLAG           0x01U

/** \brief Flag to indicate an ongoing goto sleep process */
#if (defined LINIF_SLEEP_ONGOING_FLAG)
#error LINIF_SLEEP_ONGOING_FLAG already defined!
#endif /* #if (defined LINIF_SLEEP_ONGOING_FLAG)*/
#define LINIF_SLEEP_ONGOING_FLAG             0x02U

/** \brief Flag to indicate an ongoing internal wakeup process */
#if (defined LINIF_WAKEUP_INTERNAL_ONGOING_FLAG)
#error LINIF_WAKEUP_INTERNAL_ONGOING_FLAG already defined!
#endif /* #if (defined LINIF_WAKEUP_INTERNAL_ONGOING_FLAG)*/
#define LINIF_WAKEUP_INTERNAL_ONGOING_FLAG   0x04U

/** \brief Flag to indicate an ongoing wakeup process */
#if (defined LINIF_WAKEUP_ONGOING_FLAG)
#error LINIF_WAKEUP_ONGOING_FLAG already defined!
#endif /* #if (defined LINIF_WAKEUP_ONGOING_FLAG)*/
#define LINIF_WAKEUP_ONGOING_FLAG            0x08U

/** \brief Flag to indicate a pending wakeup request during go-to-sleep command */
#if (defined LINIF_WAKEUP_REQUESTED_FLAG)
#error LINIF_WAKEUP_REQUESTED_FLAG already defined!
#endif /* #if (defined LINIF_WAKEUP_REQUESTED_FLAG)*/
#define LINIF_WAKEUP_REQUESTED_FLAG          0x10U

/** \brief Flag to indicate a succesfully confirmed wakeup request */
#if (defined LINIF_WAKEUP_CONFIRMED_FLAG)
#error LINIF_WAKEUP_CONFIRMED_FLAG already defined!
#endif /* #if (defined LINIF_WAKEUP_CONFIRMED_FLAG)*/
#define LINIF_WAKEUP_CONFIRMED_FLAG          0x20U

/* values and flags for the Slave Flags variable */

/** \brief Configured Nad is being altered flag */
#if (defined LINIF_FLG_CONF_NAD_ALTER)
#error LINIF_FLG_CONF_NAD_ALTER already defined!
#endif /* #if (defined LINIF_FLG_CONF_NAD_ALTER)*/
#define LINIF_FLG_CONF_NAD_ALTER             0x01U

/** \brief Configured Nad is being altered flag bit */
#if (defined LINIF_BIT_FLG_CONF_NAD_ALTER)
#error LINIF_BIT_FLG_CONF_NAD_ALTER already defined!
#endif /* #if (defined LINIF_BIT_FLG_CONF_NAD_ALTER)*/
#define LINIF_BIT_FLG_CONF_NAD_ALTER         0U

/** \brief Pid is being altered flag */
#if (defined LINIF_FLG_CONF_PID_ALTER)
#error LINIF_FLG_CONF_PID_ALTER already defined!
#endif /* #if (defined LINIF_FLG_CONF_PID_ALTER)*/
#define LINIF_FLG_CONF_PID_ALTER             0x02U

/** \brief Pid is being altered flag bit */
#if (defined LINIF_BIT_FLG_CONF_PID_ALTER)
#error LINIF_BIT_FLG_CONF_PID_ALTER already defined!
#endif /* #if (defined LINIF_BIT_FLG_CONF_PID_ALTER)*/
#define LINIF_BIT_FLG_CONF_PID_ALTER         1U

/** \brief response_error value set flag */
#if (defined LINIF_FLG_RESP_ERR_SET)
#error LINIF_FLG_RESP_ERR_SET already defined!
#endif /* #if (defined LINIF_FLG_RESP_ERR_SET)*/
#define LINIF_FLG_RESP_ERR_SET               0x04U

/** \brief response_error value set flag bit */
#if (defined LINIF_BIT_FLG_RESP_ERR_SET)
#error LINIF_BIT_FLG_RESP_ERR_SET already defined!
#endif /* #if (defined LINIF_BIT_FLG_RESP_ERR_SET)*/
#define LINIF_BIT_FLG_RESP_ERR_SET           2U

/** \brief response_error carrying frame is transmitted flag */
#if (defined LINIF_FLG_RESP_ERR_TX)
#error LINIF_FLG_RESP_ERR_TX already defined!
#endif /* #if (defined LINIF_FLG_RESP_ERR_TX)*/
#define LINIF_FLG_RESP_ERR_TX                0x08U

/** \brief response_error carrying frame is transmitted flag bit */
#if (defined LINIF_BIT_FLG_RESP_ERR_TX)
#error LINIF_BIT_FLG_RESP_ERR_TX already defined!
#endif /* #if (defined LINIF_BIT_FLG_RESP_ERR_TX)*/
#define LINIF_BIT_FLG_RESP_ERR_TX            3U

/** \brief ReadByIdentifier ID 1 flag */
#if (defined LINIF_FLG_READ_BY_ID_1)
#error LINIF_FLG_READ_BY_ID_1 already defined!
#endif /* #if (defined LINIF_FLG_READ_BY_ID_1)*/
#define LINIF_FLG_READ_BY_ID_1               0x10U

/** \brief ReadByIdentifier ID 1 flag bit */
#if (defined LINIF_BIT_FLG_READ_BY_ID_1)
#error LINIF_BIT_FLG_READ_BY_ID_1 already defined!
#endif /* #if (defined LINIF_BIT_FLG_READ_BY_ID_1)*/
#define LINIF_BIT_FLG_READ_BY_ID_1           4U

/** \brief PID of MRFs */
#if (defined LINIF_PID_MRF)
#error LINIF_PID_MRF already defined!
#endif /* #if (defined LINIF_PID_MRF)*/
#define LINIF_PID_MRF                        0x3CU

/** \brief PID of SRFs */
#if (defined LINIF_PID_SRF)
#error LINIF_PID_SRF already defined!
#endif /* #if (defined LINIF_PID_SRF)*/
#define LINIF_PID_SRF                        0x7DU

/** \brief Check if bit n is set in uint8[] bitfield field */
#if (defined LINIF_BIT_IS_SET)
#error LINIF_BIT_IS_SET already defined!
#endif /* #if (defined LINIF_BIT_IS_SET)*/
#define LINIF_BIT_IS_SET(field, n) \
    (((field)[(n) >> 3U] & (uint8)(1U << (uint8)((n) & 0x7U))) != 0U)

/** \brief Set bit n in uint8[] bitfield field */
#if (defined LINIF_SET_BIT)
#error LINIF_SET_BIT already defined!
#endif /* #if (defined LINIF_SET_BIT)*/
#define LINIF_SET_BIT(field,n) do { \
    (field)[(n) >> 3U] |= (uint8)(1U << (uint8)((n) & 0x7U)); } while(0)

/** \brief Set bit n in uint8[] bitfield field in an atomic way */
#if (defined LINIF_SET_BIT_ATOMIC)
#error LINIF_SET_BIT_ATOMIC already defined!
#endif /* #if (defined LINIF_SET_BIT_ATOMIC)*/
#define LINIF_SET_BIT_ATOMIC(field,n) do { \
    TS_AtomicSetBit_8(&(field)[(n) >> 3U],(uint8)((n) & 0x7U)); } while(0)

/** \brief Clear bit n in uint8[] bitfield field */
#if (defined LINIF_CLEAR_BIT)
#error LINIF_CLEAR_BIT already defined!
#endif /* #if (defined LINIF_CLEAR_BIT)*/
#define LINIF_CLEAR_BIT(field,n) do { \
    (field)[(n) >> 3U] &= (uint8)(~(uint8)(1U << (uint8)((n) & 0x7U))); } while(0)

/** \brief Clear bit n in uint8[] bitfield field in an atomic way */
#if (defined LINIF_CLEAR_BIT_ATOMIC)
#error LINIF_CLEAR_BIT_ATOMIC already defined!
#endif /* #if (defined LINIF_CLEAR_BIT_ATOMIC)*/
#define LINIF_CLEAR_BIT_ATOMIC(field,n) do { \
    TS_AtomicClearBit_8(&(field)[(n) >> 3U], (uint8)((n) & 0x7U)); } while(0)

/* PDU bytes */
#if (defined LINIF_PDU_BYTE_NAD)
#error LINIF_PDU_BYTE_NAD already defined!
#endif /* #if (defined LINIF_PDU_BYTE_NAD)*/
#define LINIF_PDU_BYTE_NAD                0U

#if (defined LINIF_PDU_BYTE_PCI)
#error LINIF_PDU_BYTE_PCI already defined!
#endif /* #if (defined LINIF_PDU_BYTE_PCI)*/
#define LINIF_PDU_BYTE_PCI                1U

#if (defined LINIF_PDU_BYTE_LEN)
#error LINIF_PDU_BYTE_LEN already defined!
#endif /* #if (defined LINIF_PDU_BYTE_LEN)*/
#define LINIF_PDU_BYTE_LEN                2U

#if (defined LINIF_PDU_BYTE_SID_FF)
#error LINIF_PDU_BYTE_SID_FF already defined!
#endif /* #if (defined LINIF_PDU_BYTE_SID_FF)*/
#define LINIF_PDU_BYTE_SID_FF             3U

#if (defined LINIF_PDU_BYTE_SID)
#error LINIF_PDU_BYTE_SID already defined!
#endif /* #if (defined LINIF_PDU_BYTE_SID)*/
#define LINIF_PDU_BYTE_SID                2U

#if (defined LINIF_PDU_BYTE_SID_ECHO)
#error LINIF_PDU_BYTE_SID_ECHO already defined!
#endif /* #if (defined LINIF_PDU_BYTE_SID_ECHO)*/
#define LINIF_PDU_BYTE_SID_ECHO           3U

/* Unused byte in Pdu*/
#if (defined LINIF_UNUSED_DATA)
#error LINIF_UNUSED_DATA already defined!
#endif /* #if (defined LINIF_UNUSED_DATA)*/
#define LINIF_UNUSED_DATA                 0xFFU

#if (defined LINIF_STANDARD_RSID_NEGATIVE_RESPONSE)
#error LINIF_STANDARD_RSID_NEGATIVE_RESPONSE already defined!
#endif /* #if (defined LINIF_STANDARD_RSID_NEGATIVE_RESPONSE)*/
#define LINIF_STANDARD_RSID_NEGATIVE_RESPONSE 0x7FU

#if (defined LINIF_STANDARD_ERRC_RESPONSE_PENDING)
#error LINIF_STANDARD_ERRC_RESPONSE_PENDING already defined!
#endif /* #if (defined LINIF_STANDARD_ERRC_RESPONSE_PENDING)*/
#define LINIF_STANDARD_ERRC_RESPONSE_PENDING  0x78U

/* Functional NAD */
#if (defined LINIF_FUNCTIONAL_NAD)
#error LINIF_FUNCTIONAL_NAD already defined!
#endif /* #if (defined LINIF_FUNCTIONAL_NAD)*/
#define LINIF_FUNCTIONAL_NAD              0x7EU

/* Broadcast NAD */
#if (defined LINIF_BROADCAST_NAD)
#error LINIF_BROADCAST_NAD already defined!
#endif /* #if (defined LINIF_BROADCAST_NAD)*/
#define LINIF_BROADCAST_NAD               0x7FU

/* Bit masks of PCI */
#if (defined LINIF_PCI_SF_FRAME)
#error LINIF_PCI_SF_FRAME already defined!
#endif /* #if (defined LINIF_PCI_SF_FRAME)*/
#define LINIF_PCI_SF_FRAME                0x00U

#if (defined LINIF_PCI_FF_FRAME)
#error LINIF_PCI_FF_FRAME already defined!
#endif /* #if (defined LINIF_PCI_FF_FRAME)*/
#define LINIF_PCI_FF_FRAME                0x10U

#if (defined LINIF_PCI_CF_FRAME)
#error LINIF_PCI_CF_FRAME already defined!
#endif /* #if (defined LINIF_PCI_CF_FRAME)*/
#define LINIF_PCI_CF_FRAME                0x20U

/** \brief SID for Assign NAD */
#if (defined LINIF_SID_ASSIGN_NAD)
#error LINIF_SID_ASSIGN_NAD already defined!
#endif /* #if (defined LINIF_SID_ASSIGN_NAD)*/
#define LINIF_SID_ASSIGN_NAD              0xB0U

/** \brief RSID for Assign NAD */
#if (defined LINIF_RSID_FWD_TP)
#error LINIF_RSID_FWD_TP already defined!
#endif /* #if (defined LINIF_RSID_FWD_TP)*/
#define LINIF_RSID_FWD_TP                 1U

/** \brief RSID for Assign NAD */
#if (defined LINIF_ASSIGN_NAD_RSID)
#error LINIF_ASSIGN_NAD_RSID already defined!
#endif /* #if (defined LINIF_ASSIGN_NAD_RSID)*/
#define LINIF_ASSIGN_NAD_RSID             (LINIF_SID_ASSIGN_NAD + 0x40U)

/** \brief SID for Read Identifier */
#if (defined LINIF_SID_READ_ID)
#error LINIF_SID_READ_ID already defined!
#endif /* #if (defined LINIF_SID_READ_ID)*/
#define LINIF_SID_READ_ID                 0xB2U

/** \brief RSID for Read Identifier */
#if (defined LINIF_READ_ID_RSID)
#error LINIF_READ_ID_RSID already defined!
#endif /* #if (defined LINIF_READ_ID_RSID)*/
#define LINIF_READ_ID_RSID                 (LINIF_SID_READ_ID + 0x40U)

/** \brief ID for Read Identifier product identification */
#if (defined LINIF_ID_READ_ID_PI)
#error LINIF_ID_READ_ID_PI already defined!
#endif /* #if (defined LINIF_ID_READ_ID_PI)*/
#define LINIF_ID_READ_ID_PI              0x00U

/** \brief ID for Read Identifier serial number */
#if (defined LINIF_ID_READ_ID_SN)
#error LINIF_ID_READ_ID_SN already defined!
#endif /* #if (defined LINIF_ID_READ_ID_SN)*/
#define LINIF_ID_READ_ID_SN              0x01U

/** \brief ID for Read Identifier bit timing */
#if (defined LINIF_ID_READ_ID_BT)
#error LINIF_ID_READ_ID_BT already defined!
#endif /* #if (defined LINIF_ID_READ_ID_BT)*/
#define LINIF_ID_READ_ID_BT              0x02U

/** \brief SID for Save Configuration */
#if (defined LINIF_SID_SAVECONF)
#error LINIF_SID_SAVECONF already defined!
#endif /* #if (defined LINIF_SID_SAVECONF)*/
#define LINIF_SID_SAVECONF                0xB6U

/** \brief SID for Save Configuration */
#if (defined LINIF_SAVECONF_RSID)
#error LINIF_SAVECONF_RSID already defined!
#endif /* #if (defined LINIF_SAVECONF_RSID)*/
#define LINIF_SAVECONF_RSID                (LINIF_SID_SAVECONF + 0x40U)

/** \brief SID for Assign Frame ID Range */
#if (defined LINIF_SID_ASSIGN_FRMIDRNG)
#error LINIF_SID_ASSIGN_FRMIDRNG already defined!
#endif /* #if (defined LINIF_SID_ASSIGN_FRMIDRNG)*/
#define LINIF_SID_ASSIGN_FRMIDRNG         0xB7U

/** \brief SID for Assign Frame ID Range */
#if (defined LINIF_ASSIGN_FRMIDRNG_RSID)
#error LINIF_ASSIGN_FRMIDRNG_RSID already defined!
#endif /* #if (defined LINIF_ASSIGN_FRMIDRNG_RSID)*/
#define LINIF_ASSIGN_FRMIDRNG_RSID        (LINIF_SID_ASSIGN_FRMIDRNG + 0x40U)

/** \brief Wildcard value of NAD */
#if (defined LINIF_WC_NAD)
#error LINIF_WC_NAD already defined!
#endif /* #if (defined LINIF_WC_NAD)*/
#define LINIF_WC_NAD                      0x7FU

/** \brief Wildcard value of Supplier ID (LSB) */
#if (defined LINIF_WC_SUPPLID_LSB)
#error LINIF_WC_SUPPLID_LSB already defined!
#endif /* #if (defined LINIF_WC_SUPPLID_LSB)*/
#define LINIF_WC_SUPPLID_LSB              0xFFU

/** \brief Wildcard value of Supplier ID (MSB) */
#if (defined LINIF_WC_SUPPLID_MSB)
#error LINIF_WC_SUPPLID_MSB already defined!
#endif /* #if (defined LINIF_WC_SUPPLID_MSB)*/
#define LINIF_WC_SUPPLID_MSB              0x7FU

/** \brief Wildcard value of Function ID (both LSB and MSB) */
#if (defined LINIF_WC_FUNC_ID_LSB)
#error LINIF_WC_FUNC_ID_LSB already defined!
#endif /* #if (defined LINIF_WC_FUNC_ID_LSB)*/
#define LINIF_WC_FUNC_ID_LSB              0xFFU

#if (defined LINIF_WC_FUNC_ID_MSB)
#error LINIF_WC_FUNC_ID_MSB already defined!
#endif /* #if (defined LINIF_WC_FUNC_ID_MSB)*/
#define LINIF_WC_FUNC_ID_MSB              LINIF_WC_FUNC_ID_LSB

/** \brief Node Configuration Handler return types
 */
#if (defined LINIF_NC_HANDLE_OK)
#error LINIF_NC_HANDLE_OK already defined!
#endif /* #if (defined LINIF_NC_HANDLE_OK)*/
#define LINIF_NC_HANDLE_OK                0U /**< Request matched handler and was OK */

#if (defined LINIF_NC_HANDLE_E_NOT_OK)
#error LINIF_NC_HANDLE_E_NOT_OK already defined!
#endif /* #if (defined LINIF_NC_HANDLE_E_NOT_OK)*/
#define LINIF_NC_HANDLE_E_NOT_OK          1U /**< Request matched handler but failed */

#if (defined LINIF_NC_HANDLE_FWD_TP)
#error LINIF_NC_HANDLE_FWD_TP already defined!
#endif /* #if (defined LINIF_NC_HANDLE_FWD_TP)*/
#define LINIF_NC_HANDLE_FWD_TP            2U /**< Unknown request */

#if (defined LINIF_NC_HANDLE_IGNORE)
#error LINIF_NC_HANDLE_IGNORE already defined!
#endif /* #if (defined LINIF_NC_HANDLE_IGNORE)*/
#define LINIF_NC_HANDLE_IGNORE            3U /**< Ignore functional request */

/** \brief Macro to get the LIN channel ID
 **
 ** This macro translates the LinIf channel ID into the correct channel ID for
 ** the LIN driver
 */
#if (LINIF_MAP_CHANNEL_ID_DIRECT == STD_ON)
#if (defined LINIF_GET_LIN_CHANNEL)
#error LINIF_GET_LIN_CHANNEL already defined!
#endif /* #if (defined LINIF_GET_LIN_CHANNEL)*/
#define LINIF_GET_LIN_CHANNEL( channel )   (channel)
#else
#if (defined LINIF_GET_LIN_CHANNEL)
#error LINIF_GET_LIN_CHANNEL already defined!
#endif /* #if (defined LINIF_GET_LIN_CHANNEL)*/
#define LINIF_GET_LIN_CHANNEL( channel )   (LINIF_CFG_GET_CHANNEL[(channel)].LinChannelId)
#endif /* LINIF_MAP_CHANNEL_ID_DIRECT != STD_ON */

#if (LINIF_MULTIPLE_DRIVER_SUPPORT == STD_ON)
/** \brief Macro to get the LIN Driver ID
 **
 ** This macro translates the LinIf channel ID into the correct Driver index for
 ** the LIN driver
 */
#if (defined LINIF_GET_LIN_DRIVER_INDEX)
#error LINIF_GET_LIN_DRIVER_INDEX already defined!
#endif /* #if (defined LINIF_GET_LIN_DRIVER_INDEX)*/
#define LINIF_GET_LIN_DRIVER_INDEX( channel )   (LINIF_CFG_GET_CHANNEL[(channel)].LinIndex)
#else
#define LINIF_GET_LIN_DRIVER_INDEX( channel )   (0U)
#endif /* LINIF_MULTIPLE_DRIVER_SUPPORT != STD_ON */

#if (LINIF_DEM_EVENTS_CONFIGURED)
/** \brief Macro to get the frame DemEventId for LIN_TX_BIT_ERROR.
 **
 ** This macro translates the LinIf DemEvent Index into the correct DemEventId
 ** corresponding to the LIN_TX_BIT_ERROR error status.
 */
#if (defined LINIF_GET_TXBIT_FRAME_DEMEVENT_ID)
#error LINIF_GET_TXBIT_FRAME_DEMEVENT_ID already defined!
#endif /* #if (defined LINIF_GET_TXBIT_FRAME_DEMEVENT_ID)*/
#define LINIF_GET_TXBIT_FRAME_DEMEVENT_ID( index )   (LINIF_CFG_GET_FRAMEDEMEVENTS[(index)].LinIfTxBitErrorDemEventId)

/** \brief Macro to get the frame DemEventId for LIN_RX_CHECKSUM_ERROR.
 **
 ** This macro translates the LinIf DemEvent Index into the correct DemEventId
 ** corresponding to the LIN_RX_CHECKSUM_ERROR error status.
 */
#if (defined LINIF_GET_CHECKSUMERR_FRAME_DEMEVENT_ID)
#error LINIF_GET_CHECKSUMERR_FRAME_DEMEVENT_ID already defined!
#endif /* #if (defined LINIF_GET_CHECKSUMERR_FRAME_DEMEVENT_ID)*/
#define LINIF_GET_CHECKSUMERR_FRAME_DEMEVENT_ID( index )   (LINIF_CFG_GET_FRAMEDEMEVENTS[(index)].LinIfRxChecksumErrorDemEventId)

/** \brief Macro to get the frame DemEventId for LIN_RX_NO_RESPONSE_ERROR.
 **
 ** This macro translates the LinIf DemEvent Index into the correct DemEventId
 ** corresponding to the LIN_RX_NO_RESPONSE_ERROR error status.
 */
#if (defined LINIF_GET_NORESP_FRAME_DEMEVENT_ID)
#error LINIF_GET_NORESP_FRAME_DEMEVENT_ID already defined!
#endif /* #if (defined LINIF_GET_NORESP_FRAME_DEMEVENT_ID)*/
#define LINIF_GET_NORESP_FRAME_DEMEVENT_ID( index )   (LINIF_CFG_GET_FRAMEDEMEVENTS[(index)].LinIfRxNoResponseErrorDemEventId)

#endif /* LINIF_DEM_EVENTS_CONFIGURED */

/** \brief Macro to translate the LinIf channel ID to ComM channel ID
 **
 ** This macro translates the given channel ID to the correct ComM channel ID.
 */
#if (LINIF_MAP_COMM_CHANNEL_ID_DIRECT == STD_ON)
#if (defined LINIF_GET_COMM_CHANNEL)
#error LINIF_GET_COMM_CHANNEL already defined!
#endif /* #if (defined LINIF_GET_COMM_CHANNEL)*/
#define LINIF_GET_COMM_CHANNEL( channel )   (channel)
#else
#if (defined LINIF_GET_COMM_CHANNEL)
#error LINIF_GET_COMM_CHANNEL already defined!
#endif /* #if (defined LINIF_GET_COMM_CHANNEL)*/
#define LINIF_GET_COMM_CHANNEL( channel )   (LINIF_CFG_GET_CHANNEL[(channel)].ComMChannelId)
#endif /* (LINIF_MAP_COMM_CHANNEL_ID_DIRECT != STD_ON) */

/** \brief Macro to translate the ComM channel ID to LinIf channel ID
 **
 ** This macro translates the ComM channel ID into the correct channel ID for
 ** internal use.
 */
#if (LINIF_MAP_COMM_CHANNEL_ID_DIRECT != STD_ON)
#if (defined LINIF_TRANSLATE_COMM_CHANNEL)
#error LINIF_TRANSLATE_COMM_CHANNEL already defined!
#endif /* #if (defined LINIF_TRANSLATE_COMM_CHANNEL)*/
#define LINIF_TRANSLATE_COMM_CHANNEL( channel )   (LINIF_CFG_GET_COMMCHANNEL[(channel)])
#else
#if (defined LINIF_TRANSLATE_COMM_CHANNEL)
#error LINIF_TRANSLATE_COMM_CHANNEL already defined!
#endif /* #if (defined LINIF_TRANSLATE_COMM_CHANNEL)*/
#define LINIF_TRANSLATE_COMM_CHANNEL( channel )   (channel)
#endif /* (LINIF_MAP_COMM_CHANNEL_ID_DIRECT != STD_ON) */

/** \brief Starts the Bus Idle timer, which starts to run after entering
 *         CHANNEL_OPERATIONAL until CHANNEL_SLEEP
 *         Reloaded on any activity
 */
#if (defined LinIf_StartTimerBI)
#error LinIf_StartTimerBI already defined!
#endif /* #if( defined LinIf_StartTimerBI)*/
#define LinIf_StartTimerBI(pIfChConfig, pIfData)                  \
        TS_AtomicAssign16((pIfData)->TimerBusIdle, (pIfChConfig)->BusIdleTimeout) \

/** \brief Stops the Bus Idle timer
 */
#if (defined LinIf_StopTimerBI)
#error LinIf_StopTimerBI already defined!
#endif /* #if( defined LinIf_StopTimerBI)*/
#define LinIf_StopTimerBI(pIfData)     \
        ((pIfData)->TimerBusIdle) = 0U \

/** \brief Starts the Nas timer, which starts to run after receiving
 *         a node configuration message until a succesfully
 *         transmitted response
 */
#if (defined LinIf_StartTimerNas)
#error LinIf_StartTimerNas already defined!
#endif /* #if( defined LinIf_StartTimerNas)*/
#define LinIf_StartTimerNas(pIfChConfig, pIfData)           \
        ((pIfData)->TimerNas = (pIfChConfig)->NCNasTimeout) \

/** \brief Stops the Nas timer
 */
#if (defined LinIf_StopTimerNas)
#error LinIf_StopTimerNas already defined!
#endif /* #if( defined LinIf_StopTimerNas)*/
#define LinIf_StopTimerNas(pIfData) \
        ((pIfData)->TimerNas) = 0U  \

/*---------------[Relocatable post build configuration macros]----------------------*/
/** \brief Get a const reference to a post-build RAM element of the LinIf
 **
 ** This macro retrieves a const pointer to a post-build RAM element of the LinIf
 */
#if (defined LINIF_PBCFG_ACCESS)
#error LINIF_PBCFG_ACCESS already defined!
#endif /* #if (defined LINIF_PBCFG_ACCESS)*/
#define LINIF_PBCFG_ACCESS(type, element)\
TS_UNCHECKEDGETCFG(LinIf_ConfigPtr, type, LINIF, LinIf_ConfigPtr->element)


#if (defined LINIF_GET_TRCV_FUNCIDX)
#error LINIF_GET_TRCV_FUNCIDX already defined!
#endif /* #if (defined LINIF_GET_TRCV_FUNCIDX)*/

#if (defined LINIF_GET_TRCV_CHID)
#error LINIF_GET_TRCV_CHID already defined!
#endif /* #if (defined LINIF_GET_TRCV_CHID)*/


#if (LINIF_TRCV_SUPPORTED == STD_ON)
#if (LINIF_TRCV_MULTIPLE_SUPPORTED == STD_ON)
/** \brief Macro to get function pointer index
 **/
#define LINIF_GET_TRCV_FUNCIDX( channel )   (LINIF_PBCFG_ACCESS(LinIfChannelType, LinIfChannelRef)[(channel)].FuncPtrIdx)
#else
#define LINIF_GET_TRCV_FUNCIDX( channel )   0U
#endif
/** \brief Macro to get transceiver channel id
 **/
#define LINIF_GET_TRCV_CHID( channel )   (LINIF_PBCFG_ACCESS(LinIfChannelType, LinIfChannelRef)[(channel)].LinTrcvChannelId)
/** \brief Macro to get WakeupSource id
 **/
#define LINIF_GET_TRCV_WUSID( channel )   (LINIF_PBCFG_ACCESS(LinIfChannelType, LinIfChannelRef)[(channel)].LinTrcvWakeupSourceId)
#endif /* LINIF_TRCV_SUPPORTED == STD_ON */

/** \brief Macro to get LinIfSchTableRef
 **
 ** This macro retrieves LinIfSchTableRef to the post-build RAM element
 */
#if (defined LINIF_CFG_GET_SCHTABLE)
#error LINIF_CFG_GET_SCHTABLE already defined!
#endif /* #if (defined LINIF_CFG_GET_SCHTABLE)*/
#define LINIF_CFG_GET_SCHTABLE \
(&LINIF_PBCFG_ACCESS(LinIfScheduleTableType, LinIfSchTableRef)[0])

/** \brief Macro to get LinIfEntryRef
 **
 ** This macro retrieves LinIfEntryRef to the post-build RAM element
 */
#if (defined LINIF_CFG_GET_ENTRY)
#error LINIF_CFG_GET_ENTRY already defined!
#endif /* #if (defined LINIF_CFG_GET_ENTRY)*/
#define LINIF_CFG_GET_ENTRY \
(&LINIF_PBCFG_ACCESS(LinIfEntryType, LinIfEntryRef)[0])

/** \brief Macro to get LinIfRxFrameRef
 **
 ** This macro retrieves LinIfRxFrameRef to the post-build RAM element
 */
#if (defined LINIF_CFG_GET_RXFRAME)
#error LINIF_CFG_GET_RXFRAME already defined!
#endif /* #if (defined LINIF_CFG_GET_RXFRAME)*/
#define LINIF_CFG_GET_RXFRAME \
(&LINIF_PBCFG_ACCESS(LinIfFrameInfoType, LinIfRxFrameRef)[0])

/** \brief Macro to get LinIfTxFrameRef
 **
 ** This macro retrieves LinIfTxFrameRef to the post-build RAM element
 */
#if (defined LINIF_CFG_GET_TXFRAME)
#error LINIF_CFG_GET_TXFRAME already defined!
#endif /* #if (defined LINIF_CFG_GET_TXFRAME)*/
#define LINIF_CFG_GET_TXFRAME \
(&LINIF_PBCFG_ACCESS(LinIfFrameInfoType, LinIfTxFrameRef)[0])

/** \brief Macro to get LinIf_FrameListsRef
 **
 ** This macro retrieves LinIf_FrameListsRef to the post-build RAM element
 */
#if (defined LINIF_CFG_GET_FRAMELIST)
#error LINIF_CFG_GET_FRAMELIST already defined!
#endif /* #if (defined LINIF_CFG_GET_FRAMELIST)*/
#define LINIF_CFG_GET_FRAMELIST \
(&LINIF_PBCFG_ACCESS(uint16, LinIf_FrameListsRef)[0])

#if (LINIF_MASTER_SUPPORTED == STD_ON)
/** \brief Macro to get LinIf_FixedFramesRef
 **
 ** This macro retrieves LinIf_FixedFramesRef to the post-build RAM element
 */
#if (defined LINIF_CFG_GET_FIXEDFRAMES)
#error LINIF_CFG_GET_FIXEDFRAMES already defined!
#endif /* #if (defined LINIF_CFG_GET_FIXEDFRAMES)*/
#define LINIF_CFG_GET_FIXEDFRAMES \
(&LINIF_PBCFG_ACCESS(LinIfFixedFrameSduBytesType, LinIf_FixedFramesRef)[0])
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

/** \brief Macro to get LinIfChannelRef
 **
 ** This macro retrieves LinIfChannelRef to the post-build RAM element
 */
#if (defined LINIF_CFG_GET_CHANNEL)
#error LINIF_CFG_GET_CHANNEL already defined!
#endif /* #if (defined LINIF_CFG_GET_CHANNEL)*/
#define LINIF_CFG_GET_CHANNEL \
(&LINIF_PBCFG_ACCESS(LinIfChannelType, LinIfChannelRef)[0])

/** \brief Macro to get LinIfMasterRef
 **
 ** This macro retrieves LinIfMasterRef to the post-build RAM element
 */
#if (defined LINIF_CFG_GET_CHANNEL_MASTER)
#error LINIF_CFG_GET_CHANNEL_MASTER already defined!
#endif /* #if (defined LINIF_CFG_GET_CHANNEL_MASTER)*/
#define LINIF_CFG_GET_CHANNEL_MASTER \
(&LINIF_PBCFG_ACCESS(LinIfMasterChannelType, LinIfMasterRef)[0])

#if (LINIF_SLAVE_SUPPORTED == STD_ON)
/** \brief Macro to get LinIfSlaveRef
 **
 ** This macro retrieves LinIfSlaveRef to the post-build config element
 */
#if (defined LINIF_CFG_GET_CHANNEL_SLAVE)
#error LINIF_CFG_GET_CHANNEL_SLAVE already defined!
#endif /* #if (defined LINIF_CFG_GET_CHANNEL_SLAVE)*/
#define LINIF_CFG_GET_CHANNEL_SLAVE(Channel) \
(&LINIF_PBCFG_ACCESS(LinIfSlaveChannelType, LinIfSlaveRef)[((Channel) - LinIf_ConfigPtr->NumChannelsMaster)])

/** \brief Macro to get LinIfSlaveRef
 **
 ** This macro retrieves LinIfSlave element from array in RAM
 */
#if (defined LINIF_GET_CHANNEL_SLAVE)
#error LINIF_GET_CHANNEL_SLAVE already defined!
#endif /* #if (defined LINIF_GET_CHANNEL_SLAVE)*/
#define LINIF_GET_CHANNEL_SLAVE(Channel) \
&LinIf_SlaveChannelInfo[((Channel) - LinIf_ConfigPtr->NumChannelsMaster)]

/** \brief Macro to get LinIf_SlaveFrameTypesRef
 **
 ** This macro retrieves LinIf_SlaveFrameTypesRef to the post-build config element
 */
#if (defined LINIF_CFG_GET_SLAVEFRTYPE)
#error LINIF_CFG_GET_SLAVEFRTYPE already defined!
#endif /* #if (defined LINIF_CFG_GET_SLAVEFRTYPE)*/
#define LINIF_CFG_GET_SLAVEFRTYPE \
(&LINIF_PBCFG_ACCESS(LinIf_SlaveFrameTypesType, LinIf_SlaveFrameTypesRef)[0])

/** \brief Macro to get LinIf_SlaveETFrameListsRef
 **
 ** This macro retrieves LinIf_SlaveETFrameListsRef to the post-build config element
 */
#if (defined LINIF_CFG_GET_SLAVE_SUBSTFRM)
#error LINIF_CFG_GET_SLAVE_SUBSTFRM already defined!
#endif /* #if (defined LINIF_CFG_GET_SLAVE_SUBSTFRM)*/
#define LINIF_CFG_GET_SLAVE_SUBSTFRM \
(&LINIF_PBCFG_ACCESS(LinIf_SlaveETFrameListType, LinIf_SlaveETFrameListsRef)[0])

/** \brief Macro to get LinIf_SlaveETFrmListSepRef
 **
 ** This macro retrieves LinIf_SlaveETFrmListSepRef to the post-build config element
 */
#if (defined LINIF_CFG_GET_SLAVE_SUBSTSEP)
#error LINIF_CFG_GET_SLAVE_SUBSTSEP already defined!
#endif /* #if (defined LINIF_CFG_GET_SLAVE_SUBSTSEP)*/
#define LINIF_CFG_GET_SLAVE_SUBSTSEP(Channel) \
(&LINIF_PBCFG_ACCESS(LinIf_SlaveETFrmListSepType, LinIf_SlaveETFrmListSepRef)[((Channel) - LinIf_ConfigPtr->NumChannelsMaster)])

#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */

/** \brief Macro to get LinIf_ComMChannelIdMapRef
 **
 ** This macro retrieves LinIf_ComMChannelIdMapRef to the post-build RAM element
 */
#if (LINIF_MAP_COMM_CHANNEL_ID_DIRECT != STD_ON)
#if (defined LINIF_CFG_GET_COMMCHANNEL)
#error LINIF_CFG_GET_COMMCHANNEL already defined!
#endif /* #if (defined LINIF_CFG_GET_COMMCHANNEL)*/
#define LINIF_CFG_GET_COMMCHANNEL \
(&LINIF_PBCFG_ACCESS(uint8, LinIf_ComMChannelIdMapRef)[0])
#endif /* (LINIF_MAP_COMM_CHANNEL_ID_DIRECT != STD_ON) */

/** \brief Macro to get LinIf_Slave2SlaveFramesRef
 **
 ** This macro retrieves LinIf_Slave2SlaveFramesRef to the post-build RAM element
 */
#if (defined LINIF_CFG_GET_SLAVEFRAME)
#error LINIF_CFG_GET_SLAVEFRAME already defined!
#endif /* #if (defined LINIF_CFG_GET_SLAVEFRAME)*/
#define LINIF_CFG_GET_SLAVEFRAME \
(&LINIF_PBCFG_ACCESS(uint8, LinIf_Slave2SlaveFramesRef)[0])

/** \brief Macro to get LinIf_EventTriggeredFramesRef
 **
 ** This macro retrieves LinIf_EventTriggeredFramesRef to the post-build RAM element
 */
#if (defined LINIF_CFG_GET_EVTTRIGGERED)
#error LINIF_CFG_GET_EVTTRIGGERED already defined!
#endif /* #if (defined LINIF_CFG_GET_EVTTRIGGERED)*/
#define LINIF_CFG_GET_EVTTRIGGERED \
(&LINIF_PBCFG_ACCESS(LinIfFrameInfoType, LinIf_EventTriggeredFramesRef)[0])

#if (LINIF_MASTER_SUPPORTED == STD_ON)

/** \brief Macro to get LinIf_FrameListSepRef
 **
 ** This macro retrieves LinIf_FrameListSepRef to the post-build RAM element
 */
#if (defined LINIF_CFG_GET_FRAMELISTSEP)
#error LINIF_CFG_GET_FRAMELISTSEP already defined!
#endif /* #if (defined LINIF_CFG_GET_FRAMELISTSEP)*/
#define LINIF_CFG_GET_FRAMELISTSEP \
(&LINIF_PBCFG_ACCESS(uint16, LinIf_FrameListSepRef)[0])

/** \brief Macro to get LinIf_RxFrameListSepRef
 **
 ** This macro retrieves LinIf_RxFrameListSepRef to the post-build RAM element
 */
#if (defined LINIF_CFG_GET_RXFRAMELISTSEP)
#error LINIF_CFG_GET_RXFRAMELISTSEP already defined!
#endif /* #if (defined LINIF_CFG_GET_RXFRAMELISTSEP)*/
#define LINIF_CFG_GET_RXFRAMELISTSEP \
(&LINIF_PBCFG_ACCESS(uint16, LinIf_RxFrameListSepRef)[0])
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

/** \brief Macro to get LinIf_FrameDemEventsRef
 **
 ** This macro retrieves LinIf_FrameDemEventsRef to the post-build RAM element
 */
#if (defined LINIF_CFG_GET_FRAMEDEMEVENTS)
#error LINIF_CFG_GET_FRAMEDEMEVENTS already defined!
#endif /* #if (defined LINIF_CFG_GET_FRAMEDEMEVENTS)*/
#define LINIF_CFG_GET_FRAMEDEMEVENTS \
(&LINIF_PBCFG_ACCESS(LinIfFrameDemEventsType, LinIf_FrameDemEventsRef)[0])

#if (LINIF_EOS_NOTIF_CALLOUT_SUPPORTED == STD_ON)
/** \brief Macro to get end-of-schedule notification function
 **
 ** This macro retrieves the end-of-schedule notification function
 ** pointer.
 */
#if (defined LINIF_CFG_GET_EOS_NFPTR)
#error LINIF_CFG_GET_EOS_NFPTR already defined !
#endif /* #if (defined LINIF_CFG_GET_EOS_NFPTR) */
#define LINIF_CFG_GET_EOS_NFPTR(schTable) \
(LinIf_EOSNotifCallouts[LINIF_CFG_GET_SCHTABLE[(schTable)].SchedEndFuncInd])
#endif /* LINIF_EOS_NOTIF_CALLOUT_SUPPORTED == STD_ON */

/** \brief Macro to get UL GotoSleepConfirmation
 **
 ** This macro retrieves the UL GotoSleepConfirmation function
 ** pointer
 */
#if (defined LINIF_CFG_GET_UL_SLEEPCONF_FPTR)
#error LINIF_CFG_GET_UL_SLEEPCONF_FPTR already defined !
#endif /* #if (defined LINIF_CFG_GET_UL_SLEEPCONF_FPTR) */

#if (LINIF_UL_CDD_SUPPORTED == STD_ON)
#define LINIF_CFG_GET_UL_SLEEPCONF_FPTR(channel) \
(LinIf_SleepConfFuncs[((&LINIF_PBCFG_ACCESS(LinIf_StateHandFuncAssocType, LinIf_StateHandFuncAssocRef)[0])[LINIF_CFG_GET_CHANNEL[(channel)].StateHandFuncIdx].GotoSleepFuncIdx)])
#else
#define LINIF_CFG_GET_UL_SLEEPCONF_FPTR(channel) \
(LinSM_GotoSleepConfirmation)
#endif /* LINIF_UL_CDD_SUPPORTED == STD_ON */

/** \brief Macro to get UL ScheduleRequestConfirmation
 **
 ** This macro retrieves the UL ScheduleRequestConfirmation function
 ** pointer
 */
#if (defined LINIF_CFG_GET_UL_SCHEDREQCONF_FPTR)
#error LINIF_CFG_GET_UL_SCHEDREQCONF_FPTR already defined !
#endif /* #if (defined LINIF_CFG_GET_UL_SCHEDREQCONF_FPTR) */

#if (LINIF_UL_CDD_SUPPORTED == STD_ON)
#define LINIF_CFG_GET_UL_SCHEDREQCONF_FPTR(channel) \
(LinIf_SchedReqConfFuncs[((&LINIF_PBCFG_ACCESS(LinIf_StateHandFuncAssocType, LinIf_StateHandFuncAssocRef)[0])[LINIF_CFG_GET_CHANNEL[(channel)].StateHandFuncIdx].SchedReqConfFuncIdx)])
#else
#define LINIF_CFG_GET_UL_SCHEDREQCONF_FPTR(channel) \
(LinSM_ScheduleRequestConfirmation)
#endif /* LINIF_UL_CDD_SUPPORTED == STD_ON */

/** \brief Macro to get UL WakeupConfirmation
 **
 ** This macro retrieves the UL WakeupConfirmation function
 ** pointer
 */
#if (defined LINIF_CFG_GET_UL_WUCONF_FPTR)
#error LINIF_CFG_GET_UL_WUCONF_FPTR already defined !
#endif /* #if (defined LINIF_CFG_GET_UL_WUCONF_FPTR) */

#if (LINIF_UL_CDD_SUPPORTED == STD_ON)
#define LINIF_CFG_GET_UL_WUCONF_FPTR(channel) \
(LinIf_WuConfFuncs[((&LINIF_PBCFG_ACCESS(LinIf_StateHandFuncAssocType, LinIf_StateHandFuncAssocRef)[0])[LINIF_CFG_GET_CHANNEL[(channel)].StateHandFuncIdx].WuConfFuncIdx)])
#else
#define LINIF_CFG_GET_UL_WUCONF_FPTR(channel) \
(LinSM_WakeupConfirmation)
#endif /* LINIF_UL_CDD_SUPPORTED == STD_ON */

#if (LINIF_SLAVE_SUPPORTED == STD_ON)

/** \brief Macro to get UL GotoSleepIndication
 **
 ** This macro retrieves the UL GotoSleepIndication function
 ** pointer
 */
#if (defined LINIF_CFG_GET_UL_SLEEPIND_FPTR)
#error LINIF_CFG_GET_UL_SLEEPIND_FPTR already defined !
#endif /* #if (defined LINIF_CFG_GET_UL_SLEEPIND_FPTR) */

#if (LINIF_UL_CDD_SUPPORTED == STD_ON)
#define LINIF_CFG_GET_UL_SLEEPIND_FPTR(channel) \
(LinIf_SleepIndFuncs[((&LINIF_PBCFG_ACCESS(LinIf_StateHandFuncAssocType, LinIf_StateHandFuncAssocRef)[0])[LINIF_CFG_GET_CHANNEL[(channel)].StateHandFuncIdx].SleepIndFuncIdx)])
#else
#define LINIF_CFG_GET_UL_SLEEPIND_FPTR(channel) \
(LinSM_GotoSleepIndication)
#endif /* LINIF_UL_CDD_SUPPORTED == STD_ON */

#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */

/** \brief Macro to get UL RxIndication
 **
 ** This macro retrieves the UL RxIndication function
 ** pointer
 */
#if (defined LINIF_CFG_GET_UL_RXIND_FPTR)
#error LINIF_CFG_GET_UL_RXIND_FPTR already defined !
#endif /* #if (defined LINIF_CFG_GET_UL_RXIND_FPTR) */

#if (LINIF_UL_CDD_SUPPORTED == STD_ON)
#define LINIF_CFG_GET_UL_RXIND_FPTR(frame) \
(LinIf_ULRxIndFuncs[(LINIF_CFG_GET_RXFRAME[(frame)].ULFuncIdx)])
#else
#define LINIF_CFG_GET_UL_RXIND_FPTR(frame) \
(PduR_LinIfRxIndication)
#endif /* LINIF_UL_CDD_SUPPORTED == STD_ON */

/** \brief Macro to get UL TriggerTransmit
 **
 ** This macro retrieves the UL TriggerTransmit function
 ** pointer
 */
#if (defined LINIF_CFG_GET_UL_TRGTX_FPTR)
#error LINIF_CFG_GET_UL_TRGTX_FPTR already defined !
#endif /* #if (defined LINIF_CFG_GET_UL_TRGTX_FPTR) */

#if (LINIF_UL_CDD_SUPPORTED == STD_ON)
#define LINIF_CFG_GET_UL_TRGTX_FPTR(frame) \
(LinIf_ULTrgTxFuncs[((&LINIF_PBCFG_ACCESS(LinIf_PduHandFuncAssocType, LinIf_PduHandFuncAssocRef)[0])[LINIF_CFG_GET_TXFRAME[(frame)].ULFuncIdx].TrgTxFuncIdx)])
#else
#define LINIF_CFG_GET_UL_TRGTX_FPTR(frame) \
(PduR_LinIfTriggerTransmit)
#endif /* LINIF_UL_CDD_SUPPORTED == STD_ON */

/** \brief Macro to get UL TxConfirmation
 **
 ** This macro retrieves the UL TxConfirmation function
 ** pointer
 */
#if (defined LINIF_CFG_GET_UL_TXCONF_FPTR)
#error LINIF_CFG_GET_UL_TXCONF_FPTR already defined !
#endif /* #if (defined LINIF_CFG_GET_UL_TXCONF_FPTR) */

#if (LINIF_UL_CDD_SUPPORTED == STD_ON)
#define LINIF_CFG_GET_UL_TXCONF_FPTR(frame) \
(LinIf_ULTxConfFuncs[((&LINIF_PBCFG_ACCESS(LinIf_PduHandFuncAssocType, LinIf_PduHandFuncAssocRef)[0])[LINIF_CFG_GET_TXFRAME[(frame)].ULFuncIdx].TxConfFuncIdx)])
#else
#define LINIF_CFG_GET_UL_TXCONF_FPTR(frame) \
(PduR_LinIfTxConfirmation)
#endif /* LINIF_UL_CDD_SUPPORTED == STD_ON */
#if (LINIF_DEV_ERROR_DETECT == STD_ON)

#if (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)

#if (defined LINIF_MAX_MIRROR_CHANNELS)
#error LINIF_MAX_MIRROR_CHANNELS already defined!
#endif /* #if( defined LINIF_MAX_MIRROR_CHANNELS)*/
#define LINIF_MAX_MIRROR_CHANNELS 16U

#if (defined LINIF_MIRROR_CHANNEL_ENABLED)
#error LINIF_MIRROR_CHANNEL_ENABLED already defined!
#endif /* #if( defined LINIF_MIRROR_CHANNEL_ENABLED)*/
#define LINIF_MIRROR_CHANNEL_ENABLED(var, channel) (((var) >> (uint16)(channel)) & (uint16)0x1U)

#if (defined LINIF_CHANNEL_ENABLED)
#error LINIF_CHANNEL_ENABLED already defined!
#endif /* #if( defined LINIF_CHANNEL_ENABLED)*/
#define LINIF_CHANNEL_ENABLED 0x1U

#endif /* LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON */





/*------------------------[Defensive programming]----------------------------*/

#if (defined LINIF_PRECONDITION_ASSERT)
#error LINIF_PRECONDITION_ASSERT is already defined
#endif

#if (defined LINIF_PRECONDITION_ASSERT_NO_EVAL)
#error LINIF_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (LINIF_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define LINIF_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), LINIF_MODULE_ID, LINIF_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define LINIF_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), LINIF_MODULE_ID, LINIF_INSTANCE_ID, (ApiId))
#else
#define LINIF_PRECONDITION_ASSERT(Condition, ApiId)
#define LINIF_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined LINIF_POSTCONDITION_ASSERT)
#error LINIF_POSTCONDITION_ASSERT is already defined
#endif

#if (defined LINIF_POSTCONDITION_ASSERT_NO_EVAL)
#error LINIF_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (LINIF_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define LINIF_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), LINIF_MODULE_ID, LINIF_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define LINIF_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), LINIF_MODULE_ID, LINIF_INSTANCE_ID, (ApiId))
#else
#define LINIF_POSTCONDITION_ASSERT(Condition, ApiId)
#define LINIF_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined LINIF_INVARIANT_ASSERT)
#error LINIF_INVARIANT_ASSERT is already defined
#endif

#if (defined LINIF_INVARIANT_ASSERT_NO_EVAL)
#error LINIF_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (LINIF_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define LINIF_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), LINIF_MODULE_ID, LINIF_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define LINIF_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), LINIF_MODULE_ID, LINIF_INSTANCE_ID, (ApiId))
#else
#define LINIF_INVARIANT_ASSERT(Condition, ApiId)
#define LINIF_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined LINIF_STATIC_ASSERT)
# error LINIF_STATIC_ASSERT is already defined
#endif
#if (LINIF_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define LINIF_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define LINIF_STATIC_ASSERT(expr)
#endif

#if (defined LINIF_UNREACHABLE_CODE_ASSERT)
#error LINIF_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (LINIF_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define LINIF_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(LINIF_MODULE_ID, LINIF_INSTANCE_ID, (ApiId))
#else
#define LINIF_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined LINIF_INTERNAL_API_ID)
#error LINIF_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define LINIF_INTERNAL_API_ID DET_INTERNAL_API_ID


#endif /* LINIF_DEV_ERROR_DETECT == STD_ON */

/*==================[type definitions]=======================================*/
#if (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)
/** \brief Frame run-time information type
 **
 ** This struct gathers information for bus mirroring for one frame required
 ** during run-time */
typedef struct
{
  uint8 FrameData[8U];              /**< data of current frame */
  uint8 Pid;                        /**< PID of current frame */
  uint8 FrameLength;                /**< length of current frame */
} LinIf_FrameInfoType;
#endif /* LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON */

/** \brief Channel common run-time information type
 **
 ** This struct gathers common information for one channel required during
 ** run-time */
typedef struct
{
    PduIdType LastId;                 /**< PDU Id of the previous frame */
    PduIdType ThisId;                 /**< PDU Id of the current frame  */
#if (LINIF_DEM_EVENTS_CONFIGURED)
    uint16 ThisDemEventsIndex;        /**< Current Id of the DemEvents structure index if the
                                           frame has one configured */
    uint16 LastDemEventsIndex;        /**< Previous Id of the DemEvents structure index if the
                                           frame has oneconfigured */
#endif /* LINIF_DEM_EVENTS_CONFIGURED */
    uint8 cFSM;                       /**< represents state of channel state machine */
    uint8 SleepHandling;              /**< indicates if a goto sleep was request or is ongoing,
                                           or wake up was requested */
    uint8 LastDl;                     /**< Data length of the last frame */
    uint8 LastHeaderType;             /**< Header type of previous frame */
    uint8 FrameType;                  /**< type of current frame */
#if (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)
    LinIf_FrameInfoType FrameInfo;    /**< content of current frame */
#endif /* LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON */
} LinIf_ChannelInfoType;

/** \brief Channel Master run-time information type
 **
 ** This struct gathers Master node information for one channel required during
 ** run-time */
typedef struct
{
    uint16 SchContTable;              /**< Latest RUN_CONTINUOUS schedule table */
    uint16 SchROTable;                /**< current RUN ONCE table request is pending */
    uint16 SchCurrentTable;           /**< currently active schedule table */
    uint16 ScheduleDelay;             /**< schedule timer */
    uint16 SchEntry;                  /**< currently selected schedule table entry */
    uint16 ResumeSchEntry;            /**< schedule table entry for table resume */
    uint16 SchCollRef;                /**< reference to collision resolution schedule table */
    uint16 StatusDelay;               /**< status check timer */
    boolean ForceReschedule;          /**< flag for forcing table rescheduling */
    boolean ResumeSch;                /**< resumes the last schedule */
#if (LINIF_EOS_NOTIF_CALLOUT_SUPPORTED == STD_ON)
    boolean CallEOSNotif;             /**< call End of Schedule Notification function */
#endif /* LINIF_EOS_NOTIF_CALLOUT_SUPPORTED == STD_ON */
    boolean RestartCurrentTable;      /**< flag for requesting a restart of the same schedule table */
} LinIf_MasterChannelInfoType;

/** \brief Channel Master run-time information type
 **
 ** This struct gathers Master node information for one channel required during
 ** run-time */
typedef struct
{
    uint16 TimerBusIdle;              /**< bus idle timeout */
    uint16 TimerNas;                  /**< Nas timeout */
    uint8  Flags;                     /**< Bit0: 1 - Nad is being altered
                                                 0 - No change to Nad ongoing
                                           Bit1: 1 - Pid is being altered
                                                 0 - No change to Pid ongoing
                                           Bit2: 1 - response_error value set
                                                 0 - response_error value not set
                                           Bit3: 1 - response_error carrying frame
                                                     is transmitted
                                                 0 - other than reponse_error carrying
                                                     frame is transmitted
                                           Bit4: 1 - ReadByIdentifier ID 1
                                                 0 - ReadByIdentifier ID 0 */
    uint8  RSID;                      /**< RSID used for SRF */
    uint8  ConfNad;                   /**< configured Nad  */
} LinIf_SlaveChannelInfoType;
/*==================[external data]=========================*/

#define LINIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <LinIf_MemMap.h>

/** \brief Configuration of Lin Driver function pointers
 **
 ** This array contains the Lin driver configurations meaning the
 ** function pointers for accessing the driver functionality.
 */
extern CONST( LinIf_LinDriverConfigType, LINIF_APPL_CONST )
  LinIf_LinDriverConfig[];

#if (LINIF_EOS_NOTIF_CALLOUT_SUPPORTED == STD_ON)
/** \brief End-of-schedule function pointers
 **
 ** This array contains the LinkTime end-of-schedule notification
 ** function pointers.
 */
extern CONST(LinIf_EndOfSchedNotifFctPtrType, LINIF_CONST) LinIf_EOSNotifCallouts[];
#endif /* LINIF_EOS_NOTIF_CALLOUT_SUPPORTED == STD_ON */

#if (LINIF_UL_CDD_SUPPORTED == STD_ON)
/** \brief UL GotoSleepConfirmation function pointers
 **
 ** This array contains the LinkTime LinSM / UL Cdd GotoSleepConfirmation
 ** function pointers.
 */
extern CONST(LinIf_SlpWuConfFuncPtrType, LINIF_CONST) LinIf_SleepConfFuncs[];

/** \brief UL WakeupConfirmation function pointers
 **
 ** This array contains the LinkTime LinSM / UL Cdd WakeupConfirmation
 ** function pointers.
 */
extern CONST(LinIf_SlpWuConfFuncPtrType, LINIF_CONST) LinIf_WuConfFuncs[];

/** \brief UL ScheduleRequestConfirmation function pointers
 **
 ** This array contains the LinkTime LinSM / UL Cdd ScheduleRequestConfirmation
 ** function pointers.
 */
extern CONST(LinIf_SchedReqConfFuncPtrType, LINIF_CONST) LinIf_SchedReqConfFuncs[];

/** \brief UL RxIndication function pointers
 **
 ** This array contains the LinkTime PduR / UL Cdd RxIndication
 ** function pointers.
 */
extern CONST(LinIf_RxIndFuncPtrType, LINIF_CONST) LinIf_ULRxIndFuncs[];

/** \brief UL TriggerTransmit function pointers
 **
 ** This array contains the LinkTime PduR / UL Cdd TriggerTransmit
 ** function pointers.
 */
extern CONST(LinIf_TrgTxFuncPtrType, LINIF_CONST) LinIf_ULTrgTxFuncs[];

/** \brief UL TxConfirmation function pointers
 **
 ** This array contains the LinkTime PduR / UL Cdd TxConfirmation
 ** function pointers.
 */
extern CONST(LinIf_TxConfFuncPtrType, LINIF_CONST) LinIf_ULTxConfFuncs[];

#endif /* LINIF_UL_CDD_SUPPORTED == STD_ON */

#define LINIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <LinIf_MemMap.h>

/*==================[external variable declarations]=========================*/

#if (LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON)

#define LINIF_START_SEC_VAR_INIT_16
#include <LinIf_MemMap.h>

/** \brief Global state of the LIN Interface */
extern VAR(uint16, LINIF_VAR) LinIf_BusMiroringChannelEnabled;

#define LINIF_STOP_SEC_VAR_INIT_16
#include <LinIf_MemMap.h>

#endif /* LINIF_BUS_MIRRORING_SUPPORT_ENABLE == STD_ON */

#define LINIF_START_SEC_VAR_INIT_8
/* !LINKSTO LinIf.ASR40.LINIF590,1 */
#include <LinIf_MemMap.h>

/** \brief Global state of the LIN Interface */
extern VAR(uint8, LINIF_VAR) LinIf_InitState;

/** \brief This bitfield holds the event flags for all unconditional frames. */
extern VAR(uint8, LINIF_VAR) LinIf_FrameEventFlags[LINIF_MAX_PDUS_FLAGS];

#if (LINIF_MASTER_SUPPORTED == STD_ON)

/** \brief This bitfield holds the information if there is collision resolution
 **        in progress for event triggered frames. */
extern VAR(uint8, LINIF_VAR) LinIf_EventTriggeredCollision[LINIF_MAX_EV_TRGD];

#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

#define LINIF_STOP_SEC_VAR_INIT_8
/* !LINKSTO LinIf.ASR40.LINIF590,1 */
#include <LinIf_MemMap.h>

#if (LINIF_SLAVE_SUPPORTED == STD_ON)

#define LINIF_START_SEC_VAR_CLEARED_8
#include <LinIf_MemMap.h>

/** \brief This array holds the assigned PIDs to the slave frames */
extern VAR(uint8, LINIF_VAR) LinIf_PidTable[LINIF_MAX_SLAVE_FRAMES];

#define LINIF_STOP_SEC_VAR_CLEARED_8
#include <LinIf_MemMap.h>

#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */

#define LINIF_START_SEC_VAR_CLEARED_UNSPECIFIED
/* !LINKSTO LinIf.ASR40.LINIF590,1 */
#include <LinIf_MemMap.h>

/** \brief This pointer holds the root of the Configuration for the linIf. */
extern P2CONST(LinIf_ConfigType, LINIF_VAR, LINIF_APPL_CONST) LinIf_ConfigPtr;

/** \brief Runtime information for all channels */
extern VAR(LinIf_ChannelInfoType, LINIF_VAR) LinIf_ChannelInfo[LINIF_MAX_CHANNELS];

#if (LINIF_MASTER_SUPPORTED == STD_ON)

/** \brief Runtime information for Master node channels */
extern VAR(LinIf_MasterChannelInfoType, LINIF_VAR) LinIf_MasterChannelInfo[LINIF_MAX_CHANNELS];

#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

#if (LINIF_SLAVE_SUPPORTED == STD_ON)

/** \brief Runtime information for Slave node channels */
extern VAR(LinIf_SlaveChannelInfoType, LINIF_VAR) LinIf_SlaveChannelInfo[LINIF_MAX_CHANNELS];

#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */

#define LINIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
/* !LINKSTO LinIf.ASR40.LINIF590,1 */
#include <LinIf_MemMap.h>

/*==================[external function declarations]=========================*/

#define LINIF_START_SEC_CODE
/* !LINKSTO LinIf.ASR40.LINIF590,1 */
#include <LinIf_MemMap.h>

/** \brief This function handles new schedule request from internal
 *  or upper layers
 *
 *  \param internalChannel  The LIN channel where the schedule request should
 *                          be operated
 *  \param TableIndex       Schedule table index, indicates which schedule
 *                          should be selected
 */
extern FUNC(void, LINIF_CODE) LinIf_InternalScheduleRequest
(
    uint8 internalChannel,
    uint16 TableIndex
);

/** \brief This function confirms a successful transmission of
 *  an unconditional TX frame to upper layers
 *
 *  \param currentChannel  Channel which is currently processed
 *  \param pPduInfo        received data
 */
extern FUNC(void, LINIF_CODE) LinIf_Status_PduRTxConf
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
);

/** \brief This function indicates a successful reception of data
 *  to upper layers
 *
 *  \param currentChannel  Channel which is currently processed
 *  \param pPduInfo        received data
 */
extern FUNC(void, LINIF_CODE) LinIf_Status_PduRRxInd
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
);


#if (LINIF_PROD_ERR_HANDLING_TX_BIT != TS_PROD_ERR_DISABLE)

/** \brief This function calls the Det or the Dem module, which
 *  indicates that LIN_TX_BIT_ERROR response was received
 *  from a slave
 *
 *  \param currentChannel  Channel which is currently processed
 *  \param status          Lin Driver Status
 *  \param pPduInfo        Frame data
 */
extern FUNC(void, LINIF_CODE) LinIf_Status_HandleTxBitError
(
    uint8 currentChannel,
    Lin_StatusType status,
    P2VAR(PduInfoType,AUTOMATIC,AUTOMATIC) pPduInfo
);

#endif /* (LINIF_PROD_ERR_HANDLING_TX_BIT != TS_PROD_ERR_DISABLE) */

#if (LINIF_PROD_ERR_HANDLING_RX_CHECKSUM != TS_PROD_ERR_DISABLE)

/** \brief This function calls a Det or the Dem module, which
 *  indicates that LIN_RX_CHECKSUM_ERROR response was received
 *  from a slave
 *
 *  \param currentChannel  Channel which is currently processed
 *  \param status          Lin Driver Status
 */
extern FUNC(void, LINIF_CODE) LinIf_Status_HandleRxChecksumError
(
    uint8 currentChannel,
    Lin_StatusType status
);

#endif /* (LINIF_PROD_ERR_HANDLING_RX_CHECKSUM != TS_PROD_ERR_DISABLE) */

#if (LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE != TS_PROD_ERR_DISABLE)

/** \brief This function calls a Det or the Dem module, which
 *  indicates that LIN_RX_NO_RESPONSE_ERROR response was received
 *  from a slave
 *
 *  \param currentChannel  Channel which is currently processed
 *  \param status          Lin Driver Status
 */
extern FUNC(void, LINIF_CODE) LinIf_Status_HandleRxNoResponseError
(
    uint8 currentChannel,
    Lin_StatusType status
);

#endif /* (LINIF_PROD_ERR_HANDLING_RX_NO_RESPONSE != TS_PROD_ERR_DISABLE) */

#if (LINIF_SLAVE_SUPPORTED == STD_ON)
/** \brief This function handles sleep/wakeup after Lin_GetStatus returns.
 *
 *  \param currentChannel  Channel which is currently processed
 */
extern FUNC(void, LINIF_CODE) LinIf_SlaveHandleSleep
(
  uint8 currentChannel
);

#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */

#define LINIF_STOP_SEC_CODE
/* !LINKSTO LinIf.ASR40.LINIF590,1 */
#include <LinIf_MemMap.h>

#undef TS_RELOCATABLE_CFG_ENABLE
#endif /* ifndef LINIF_INT_H */
/*==================[end of file]============================================*/
