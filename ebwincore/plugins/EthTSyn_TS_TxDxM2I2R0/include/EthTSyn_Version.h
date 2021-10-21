#ifndef ETHTSYN_VERSION_H
#define ETHTSYN_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR EthTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module EthTSyn.
 *
 * \version 2.2.4
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

#if (defined ETHTSYN_VENDOR_ID)
#error ETHTSYN_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define ETHTSYN_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined ETHTSYN_MODULE_ID)
#error ETHTSYN_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define ETHTSYN_MODULE_ID         164U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined ETHTSYN_AR_RELEASE_MAJOR_VERSION)
#error ETHTSYN_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define ETHTSYN_AR_RELEASE_MAJOR_VERSION     4U

#if (defined ETHTSYN_AR_RELEASE_MINOR_VERSION)
#error ETHTSYN_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define ETHTSYN_AR_RELEASE_MINOR_VERSION     4U

#if (defined ETHTSYN_AR_RELEASE_REVISION_VERSION)
#error ETHTSYN_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define ETHTSYN_AR_RELEASE_REVISION_VERSION  0U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined ETHTSYN_SW_MAJOR_VERSION)
#error ETHTSYN_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define ETHTSYN_SW_MAJOR_VERSION             2U

#if (defined ETHTSYN_SW_MINOR_VERSION)
#error ETHTSYN_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define ETHTSYN_SW_MINOR_VERSION             2U

#if (defined ETHTSYN_SW_PATCH_VERSION)
#error ETHTSYN_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define ETHTSYN_SW_PATCH_VERSION             4U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef ETHTSYN_VERSION_H */
/*==================[end of file]============================================*/
