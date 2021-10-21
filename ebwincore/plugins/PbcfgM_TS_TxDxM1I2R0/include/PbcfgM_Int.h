/**
 * \file
 *
 * \brief AUTOSAR PbcfgM
 *
 * This file contains the implementation of the AUTOSAR
 * module PbcfgM.
 *
 * \version 1.2.22
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef PBCFGM_INT_H
#define PBCFGM_INT_H

/*==================[inclusions]============================================*/

/*==================[macros]================================================*/
/** \brief Definition of status PBCFGM_INVALID_MODULE_ID */
#if (defined PBCFGM_INVALID_MODULE_ID)        /* To prevent double declaration */
#error PBCFGM_INVALID_MODULE_ID already defined
#endif

/** \brief Define PBCFGM_INVALID_MODULE_ID   */
#define PBCFGM_INVALID_MODULE_ID 0xFFFFU

 /** \brief Definition of status PBCFGM_INVALID_INSTANCE_ID */
#if (defined PBCFGM_INVALID_INSTANCE_ID)        /* To prevent double declaration */
#error PBCFGM_INVALID_INSTANCE_ID already defined
#endif

/** \brief Define PBCFGM_INVALID_INSTANCE_ID   */
#define PBCFGM_INVALID_INSTANCE_ID 0xFFFFU

/** \brief Definition of status PBCFGM_UNINIT */
#if (defined PBCFGM_UNINIT) /* To prevent double declaration */
#error PBCFGM_UNINIT already defined
#endif

/** \brief Define PBCFGM_UNINIT   */
#define PBCFGM_UNINIT (0x0U)

/** \brief Definition of status PBCFGM_INIT */
#if (defined PBCFGM_INIT)  /* To prevent double declaration */
#error PBCFGM_INIT already defined
#endif

/** \brief Define PBCFGM_INIT */
#define PBCFGM_INIT (0x01U)

/*==================[type definitions]======================================*/

/** \brief definition of the PbcfgM_StatusType */
typedef uint8 PbcfgM_StatusType;

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef PBCFGM_INT_H */
/*==================[end of file]===========================================*/
