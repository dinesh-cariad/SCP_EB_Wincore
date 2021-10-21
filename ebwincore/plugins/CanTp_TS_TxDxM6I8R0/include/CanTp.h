/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef CANTP_H
#define CANTP_H
/*==================[inclusions]============================================*/

#include <CanTp_Version.h>        /* CanTp version declarations */
#include <CanTp_Cfg.h>            /* CanTp configuration */

#if(CANTP_JUMPTABLE_SUPPORT == STD_ON)
#include <CanTp_EntryTableApi.h>  /* CanTp entry jumptable */
#else
#include <CanTp_Api.h>            /* CanTp public API */
#endif

/* Exclude post-build-time config include file if requested to do so */
#if(!defined CANTP_NO_PBCFG_REQUIRED) && (!defined CANTP_NO_CFG_REQUIRED)
#if (CANTP_PBCFGM_SUPPORT_ENABLED == STD_OFF)
#include <CanTp_PBcfg.h>          /* get AUTOSAR CanTp post build configuration */
#endif /* CANTP_PBCFGM_SUPPORT_ENABLED == STD_OFF */
#endif /* CANTP_NO_PBCFG_REQUIRED, CANTP_NO_CFG_REQUIRED */

/* !LINKSTO CanTp.ASR40.CANTP264,1 */
#include <ComStack_Types.h>    /* typedefs for AUTOSAR com stack */

/* !LINKSTO CanTp.ASR40.CANTP264,1 */
#if(CANTP_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>
#endif

#undef CANTP_NO_PBCFG_REQUIRED

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

#endif /* ifndef CANTP_H */
/*==================[end of file]===========================================*/
