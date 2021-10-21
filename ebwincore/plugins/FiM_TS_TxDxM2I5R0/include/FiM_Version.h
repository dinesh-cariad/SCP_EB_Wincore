#ifndef FIM_VERSION_H
#define FIM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR FiM
 *
 * This file contains the implementation of the AUTOSAR
 * module FiM.
 *
 * \version 2.5.3
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

#if (defined FIM_VENDOR_ID)
#error FIM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define FIM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined FIM_MODULE_ID)
#error FIM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define FIM_MODULE_ID         11U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined FIM_AR_RELEASE_MAJOR_VERSION)
#error FIM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define FIM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined FIM_AR_RELEASE_MINOR_VERSION)
#error FIM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define FIM_AR_RELEASE_MINOR_VERSION     0U

#if (defined FIM_AR_RELEASE_REVISION_VERSION)
#error FIM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define FIM_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined FIM_SW_MAJOR_VERSION)
#error FIM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define FIM_SW_MAJOR_VERSION             2U

#if (defined FIM_SW_MINOR_VERSION)
#error FIM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define FIM_SW_MINOR_VERSION             5U

#if (defined FIM_SW_PATCH_VERSION)
#error FIM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define FIM_SW_PATCH_VERSION             3U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef FIM_VERSION_H */
/*==================[end of file]============================================*/
