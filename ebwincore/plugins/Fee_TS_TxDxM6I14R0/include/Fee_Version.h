/* !LINKSTO FEE185,1 */
#ifndef FEE_VERSION_H
#define FEE_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR Fee
 *
 * This file contains the implementation of the AUTOSAR
 * module Fee.
 *
 * \version 6.14.13
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

#if (defined FEE_VENDOR_ID)
#error FEE_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define FEE_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined FEE_MODULE_ID)
#error FEE_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define FEE_MODULE_ID         21U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined FEE_AR_RELEASE_MAJOR_VERSION)
#error FEE_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define FEE_AR_RELEASE_MAJOR_VERSION     4U

#if (defined FEE_AR_RELEASE_MINOR_VERSION)
#error FEE_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define FEE_AR_RELEASE_MINOR_VERSION     0U

#if (defined FEE_AR_RELEASE_REVISION_VERSION)
#error FEE_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define FEE_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined FEE_SW_MAJOR_VERSION)
#error FEE_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define FEE_SW_MAJOR_VERSION             6U

#if (defined FEE_SW_MINOR_VERSION)
#error FEE_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define FEE_SW_MINOR_VERSION             14U

#if (defined FEE_SW_PATCH_VERSION)
#error FEE_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define FEE_SW_PATCH_VERSION             13U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef FEE_VERSION_H */
/*==================[end of file]============================================*/
