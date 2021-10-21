#ifndef CANTSYN_VERSION_H
#define CANTSYN_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR CanTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTSyn.
 *
 * \version 2.0.8
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

#if (defined CANTSYN_VENDOR_ID)
#error CANTSYN_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define CANTSYN_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined CANTSYN_MODULE_ID)
#error CANTSYN_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define CANTSYN_MODULE_ID         161U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined CANTSYN_AR_RELEASE_MAJOR_VERSION)
#error CANTSYN_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define CANTSYN_AR_RELEASE_MAJOR_VERSION     4U

#if (defined CANTSYN_AR_RELEASE_MINOR_VERSION)
#error CANTSYN_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define CANTSYN_AR_RELEASE_MINOR_VERSION     4U

#if (defined CANTSYN_AR_RELEASE_REVISION_VERSION)
#error CANTSYN_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define CANTSYN_AR_RELEASE_REVISION_VERSION  0U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined CANTSYN_SW_MAJOR_VERSION)
#error CANTSYN_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define CANTSYN_SW_MAJOR_VERSION             2U

#if (defined CANTSYN_SW_MINOR_VERSION)
#error CANTSYN_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define CANTSYN_SW_MINOR_VERSION             0U

#if (defined CANTSYN_SW_PATCH_VERSION)
#error CANTSYN_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define CANTSYN_SW_PATCH_VERSION             8U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef CANTSYN_VERSION_H */
/*==================[end of file]============================================*/
