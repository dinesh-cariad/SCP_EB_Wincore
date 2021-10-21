/* !LINKSTO NVM743,1 */
#ifndef NVM_VERSION_H
#define NVM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR NvM
 *
 * This file contains the implementation of the AUTOSAR
 * module NvM.
 *
 * \version 6.17.22
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

#if (defined NVM_VENDOR_ID)
#error NVM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define NVM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined NVM_MODULE_ID)
#error NVM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define NVM_MODULE_ID         20U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined NVM_AR_RELEASE_MAJOR_VERSION)
#error NVM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define NVM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined NVM_AR_RELEASE_MINOR_VERSION)
#error NVM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define NVM_AR_RELEASE_MINOR_VERSION     0U

#if (defined NVM_AR_RELEASE_REVISION_VERSION)
#error NVM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define NVM_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined NVM_SW_MAJOR_VERSION)
#error NVM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define NVM_SW_MAJOR_VERSION             6U

#if (defined NVM_SW_MINOR_VERSION)
#error NVM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define NVM_SW_MINOR_VERSION             17U

#if (defined NVM_SW_PATCH_VERSION)
#error NVM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define NVM_SW_PATCH_VERSION             22U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef NVM_VERSION_H */
/*==================[end of file]============================================*/
