#ifndef ETHSM_VERSION_H
#define ETHSM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR EthSM
 *
 * This file contains the implementation of the AUTOSAR
 * module EthSM.
 *
 * \version 1.6.14
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

#if (defined ETHSM_VENDOR_ID)
#error ETHSM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define ETHSM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined ETHSM_MODULE_ID)
#error ETHSM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define ETHSM_MODULE_ID         143U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined ETHSM_AR_RELEASE_MAJOR_VERSION)
#error ETHSM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define ETHSM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined ETHSM_AR_RELEASE_MINOR_VERSION)
#error ETHSM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define ETHSM_AR_RELEASE_MINOR_VERSION     3U

#if (defined ETHSM_AR_RELEASE_REVISION_VERSION)
#error ETHSM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define ETHSM_AR_RELEASE_REVISION_VERSION  0U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined ETHSM_SW_MAJOR_VERSION)
#error ETHSM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define ETHSM_SW_MAJOR_VERSION             1U

#if (defined ETHSM_SW_MINOR_VERSION)
#error ETHSM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define ETHSM_SW_MINOR_VERSION             6U

#if (defined ETHSM_SW_PATCH_VERSION)
#error ETHSM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define ETHSM_SW_PATCH_VERSION             14U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef ETHSM_VERSION_H */
/*==================[end of file]============================================*/
