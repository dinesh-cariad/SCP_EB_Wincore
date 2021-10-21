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
#ifndef LINTP_TYPES_H
#define LINTP_TYPES_H

/*==================[inclusions]=============================================*/

#include <LinIf_Cfg.h>             /* module header */

#if (LINIF_TP_SUPPORTED == STD_ON)

#include <ComStack_Types.h>        /* AUTOSAR standard types */
/* !LINKSTO dsn.LinTp.PostBuildConfigurationData,1 */
#include <LinTp_CfgTypes.h>        /* LinTp generated config types */

/*=================[macros]=================================================*/

/** \brief  Disable values for timers */
#if (defined LINTP_NCR_DISABLED)
#error LINTP_NCR_DISABLED already defined!
#endif /* #if (defined LINTP_NCR_DISABLED)*/
#define LINTP_NCR_DISABLED       0U

#if (defined LINTP_NCS_DISABLED)
#error LINTP_NCS_DISABLED already defined!
#endif /* #if (defined LINTP_NCS_DISABLED)*/
#define LINTP_NCS_DISABLED       0U

#if (defined LINTP_P2TIMING_DISABLED)
#error LINTP_P2TIMING_DISABLED already defined!
#endif /* #if (defined LINTP_P2TIMING_DISABLED)*/
#define LINTP_P2TIMING_DISABLED  0U

#if (defined LINTP_NAS_DISABLED)
#error LINTP_NAS_DISABLED already defined!
#endif /* #if (defined LINTP_NAS_DISABLED)*/
#define LINTP_NAS_DISABLED       0U

/**  \brief  Define used for the case when configured NAD is zero
 * (used in LinTp_PBcfg.c)
 **/
#if (defined LINTP_RXNSDUNAD_ZERO)
#error LINTP_RXNSDUNAD_ZERO already defined!
#endif /* #if (defined LINTP_RXNSDUNAD_ZERO)*/
#define LINTP_RXNSDUNAD_ZERO     0U

#if (defined LINTP_TXNSDUNAD_ZERO)
#error LINTP_TXNSDUNAD_ZERO already defined!
#endif /* #if (defined LINTP_TXNSDUNAD_ZERO)*/
#define LINTP_TXNSDUNAD_ZERO     0U

/* BswM is called during runtime*/
#if (defined LINTP_SCHEDULE_CHANGE_DIAG)
#error LINTP_SCHEDULE_CHANGE_DIAG already defined!
#endif /* #if (defined LINTP_SCHEDULE_CHANGE_DIAG)*/
#define LINTP_SCHEDULE_CHANGE_DIAG               0x01U

/* BswM is not called during runtime */
#if (defined LINTP_SCHEDULE_CHANGE_DIAG_FALSE)
#error LINTP_SCHEDULE_CHANGE_DIAG_FALSE already defined!
#endif /* #if (defined LINTP_SCHEDULE_CHANGE_DIAG_FALSE)*/
#define LINTP_SCHEDULE_CHANGE_DIAG_FALSE         0x00U

/* Transmission is not aborted if invalid NAD is received*/
#if (defined LINTP_DROP_NOT_REQ_NAD)
#error LINTP_DROP_NOT_REQ_NAD already defined!
#endif /* #if (defined LINTP_DROP_NOT_REQ_NAD)*/
#define LINTP_DROP_NOT_REQ_NAD                   0x02U

/* Transmission is aborted if invalid NAD is received */
#if (defined LINTP_DROP_NOT_REQ_NAD_FALSE)
#error LINTP_DROP_NOT_REQ_NAD_FALSE already defined!
#endif /* #if (defined LINTP_DROP_NOT_REQ_NAD_FALSE)*/
#define LINTP_DROP_NOT_REQ_NAD_FALSE             0x00U


/** \brief Schedule table modes
 **/
#if (defined LINTP_APPLICATIVE_SCHEDULE)
#error LINTP_APPLICATIVE_SCHEDULE already defined!
#endif /* #if (defined LINTP_APPLICATIVE_SCHEDULE)*/
#define LINTP_APPLICATIVE_SCHEDULE 0U /**< Applicative schedule is selected */

#if (defined LINTP_DIAG_REQUEST)
#error LINTP_DIAG_REQUEST already defined!
#endif /* #if (defined LINTP_DIAG_REQUEST)*/
#define LINTP_DIAG_REQUEST 1U         /**< Master request schedule table is selected */

#if (defined LINTP_DIAG_RESPONSE)
#error LINTP_DIAG_RESPONSE already defined!
#endif /* #if (defined LINTP_DIAG_RESPONSE)*/
#define LINTP_DIAG_RESPONSE 2U        /**< Slave response schedule table is selected */

/*==================[type definitions]=======================================*/


/** \brief LinTp_Mode - Schedule table mode
 **/
typedef uint8 LinTp_Mode;

/*==================[external variable declarations]=========================*/

#endif /* (LINIF_TP_SUPPORTED == STD_ON) */

#endif /* ifndef LINTP_TYPES_H */
/*==================[end of file]============================================*/
