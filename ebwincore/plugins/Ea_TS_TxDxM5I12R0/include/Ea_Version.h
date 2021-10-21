/* !LINKSTO EA177,1 */
#ifndef EA_VERSION_H
#define EA_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR Ea
 *
 * This file contains the implementation of the AUTOSAR
 * module Ea.
 *
 * \version 5.12.14
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined EA_VENDOR_ID)
#error EA_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define EA_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined EA_MODULE_ID)
#error EA_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define EA_MODULE_ID         40U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined EA_AR_RELEASE_MAJOR_VERSION)
#error EA_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define EA_AR_RELEASE_MAJOR_VERSION     4U

#if (defined EA_AR_RELEASE_MINOR_VERSION)
#error EA_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define EA_AR_RELEASE_MINOR_VERSION     0U

#if (defined EA_AR_RELEASE_REVISION_VERSION)
#error EA_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define EA_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined EA_SW_MAJOR_VERSION)
#error EA_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define EA_SW_MAJOR_VERSION             5U

#if (defined EA_SW_MINOR_VERSION)
#error EA_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define EA_SW_MINOR_VERSION             12U

#if (defined EA_SW_PATCH_VERSION)
#error EA_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define EA_SW_PATCH_VERSION             14U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef EA_VERSION_H */
/*==================[end of file]============================================*/
