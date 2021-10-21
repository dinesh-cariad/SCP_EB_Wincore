/**
 * \file
 *
 * \brief AUTOSAR SCrc
 *
 * This file contains the implementation of the AUTOSAR
 * module SCrc.
 *
 * \version 2.0.11
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef SCRC_H
#define SCRC_H

/*==================[includes]===============================================*/

#include <Std_Types.h>   /* AUTOSAR standard types */
#include <SCrc_LocalSrcCfg.h>    /* SCrc configuration     */

/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined SCRC_VENDOR_ID) /* to prevent double declaration */
#error SCRC_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define SCRC_VENDOR_ID         1U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined SCRC_SW_MAJOR_VERSION) /* to prevent double declaration */
#error SCRC_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define SCRC_SW_MAJOR_VERSION  2U

#if (defined SCRC_SW_MINOR_VERSION) /* to prevent double declaration */
#error SCRC_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define SCRC_SW_MINOR_VERSION  0U

#if (defined SCRC_SW_PATCH_VERSION) /* to prevent double declaration */
#error SCRC_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define SCRC_SW_PATCH_VERSION  11U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

#endif /* ifndef SCRC_H */
/*==================[end of file]============================================*/
