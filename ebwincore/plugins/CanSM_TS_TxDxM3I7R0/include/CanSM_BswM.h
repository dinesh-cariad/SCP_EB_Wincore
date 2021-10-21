/**
 * \file
 *
 * \brief AUTOSAR CanSM
 *
 * This file contains the implementation of the AUTOSAR
 * module CanSM.
 *
 * \version 3.7.5
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CANSM_BSWM_H
#define CANSM_BSWM_H
/*==================[inclusions]============================================*/

/*==================[macros]================================================*/

#if (defined CANSM_BSWM_NO_COMMUNICATION)      /* To prevent double declaration */
#error CANSM_BSWM_NO_COMMUNICATION already defined!
#endif /* #if (defined CANSM_BSWM_NO_COMMUNICATION) */

#if (defined CANSM_BSWM_SILENT_COMMUNICATION)      /* To prevent double declaration */
#error CANSM_BSWM_SILENT_COMMUNICATION already defined!
#endif /* #if (defined CANSM_BSWM_SILENT_COMMUNICATION) */

#if (defined CANSM_BSWM_FULL_COMMUNICATION)      /* To prevent double declaration */
#error CANSM_BSWM_FULL_COMMUNICATION already defined!
#endif /* #if (defined CANSM_BSWM_FULL_COMMUNICATION) */

#if (defined CANSM_BSWM_BUS_OFF)      /* To prevent double declaration */
#error CANSM_BSWM_BUS_OFF already defined!
#endif /* #if (defined CANSM_BSWM_BUS_OFF) */

#if (defined CANSM_BSWM_CHANGE_BAUDRATE)      /* To prevent double declaration */
#error CANSM_BSWM_CHANGE_BAUDRATE already defined!
#endif /* #if (defined CANSM_BSWM_CHANGE_BAUDRATE) */

#if (defined CANSM_BSWM_BUS_OFF_L1)      /* To prevent double declaration */
#error CANSM_BSWM_BUS_OFF_L1 already defined!
#endif /* #if (defined CANSM_BSWM_BUS_OFF_L1) */

#if (defined CANSM_BSWM_BUS_OFF_L2)      /* To prevent double declaration */
#error CANSM_BSWM_BUS_OFF_L2 already defined!
#endif /* #if (defined CANSM_BSWM_BUS_OFF_L2) */

/** \brief This macro defines the value for reporting no communication to BswM */
#define CANSM_BSWM_NO_COMMUNICATION      0U
/** \brief This macro defines the value for reporting silent communication to BswM */
#define CANSM_BSWM_SILENT_COMMUNICATION  1U
/** \brief This macro defines the value for reporting full communication to BswM */
#define CANSM_BSWM_FULL_COMMUNICATION    2U
/** \brief This macro defines the value for reporting bus-off to BswM */
#define CANSM_BSWM_BUS_OFF               3U
/** \brief This macro defines the value for reporting baudrate change to BswM */
#define CANSM_BSWM_CHANGE_BAUDRATE       4U

/* Provide new literals, in addition to the one specified in the CanSM SWS
 * Refer to configuration parameter CanSMEnhancedBusOffReporting for more details
 */

/** \brief This macro defines the value for reporting bus-off level 1 to BswM */
#define CANSM_BSWM_BUS_OFF_L1            5U
/** \brief This macro defines the value for reporting bus-off level 2 to BswM */
#define CANSM_BSWM_BUS_OFF_L2            6U

/*==================[type definitions]======================================*/

/** \brief Type to report current state to BswM */
typedef uint8 CanSM_BswMCurrentStateType;

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[external function declarations]========================*/

#endif /* ifndef CANSM_BSWM_H */
/*==================[end of file]===========================================*/

