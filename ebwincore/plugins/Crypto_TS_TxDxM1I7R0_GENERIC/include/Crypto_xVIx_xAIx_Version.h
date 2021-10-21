/**
 * \file
 *
 * \brief AUTOSAR Crypto
 *
 * This file contains the implementation of the AUTOSAR
 * module Crypto.
 *
 * \version 1.7.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef CRYPTO_XVIX_XAIX_VERSION_H
#define CRYPTO_XVIX_XAIX_VERSION_H

/*================================================================================================*/

/*==================[misra deviations]============================================================*/

/*==================[inclusions]==================================================================*/

/*==================[macros]======================================================================*/

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined CRYPTO_XVIX_XAIX_VENDOR_ID) /* to prevent double declaration */
#error CRYPTO_XVIX_XAIX_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define CRYPTO_XVIX_XAIX_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined CRYPTO_XVIX_XAIX_MODULE_ID) /* to prevent double declaration */
#error CRYPTO_XVIX_XAIX_MODULE_ID is already defined
#endif
/** \brief AUTOSAR module identification */
#define CRYPTO_XVIX_XAIX_MODULE_ID         114U

/*------------------[AUTOSAR specification version identification]----------*/

#if (defined CRYPTO_XVIX_XAIX_AR_MAJOR_VERSION) /* to prevent double declaration */
#error CRYPTO_XVIX_XAIX_AR_MAJOR_VERSION is already defined
#endif
/** \brief AUTOSAR specification major version */
#define CRYPTO_XVIX_XAIX_AR_MAJOR_VERSION  4U

#if (defined CRYPTO_XVIX_XAIX_AR_MINOR_VERSION) /* to prevent double declaration */
#error CRYPTO_XVIX_XAIX_AR_MINOR_VERSION is already defined
#endif
/** \brief AUTOSAR specification minor version */
#define CRYPTO_XVIX_XAIX_AR_MINOR_VERSION  3U

#if (defined CRYPTO_XVIX_XAIX_AR_PATCH_VERSION) /* to prevent double declaration */
#error CRYPTO_XVIX_XAIX_AR_PATCH_VERSION is already defined
#endif
/** \brief AUTOSAR specification patch version */
#define CRYPTO_XVIX_XAIX_AR_PATCH_VERSION  0U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined CRYPTO_XVIX_XAIX_SW_MAJOR_VERSION) /* to prevent double declaration */
#error CRYPTO_XVIX_XAIX_SW_MAJOR_VERSION is already defined
#endif
/** \brief AUTOSAR module major version */
#define CRYPTO_XVIX_XAIX_SW_MAJOR_VERSION  1U

#if (defined CRYPTO_XVIX_XAIX_SW_MINOR_VERSION) /* to prevent double declaration */
#error CRYPTO_XVIX_XAIX_SW_MINOR_VERSION is already defined
#endif
/** \brief AUTOSAR module minor version */
#define CRYPTO_XVIX_XAIX_SW_MINOR_VERSION  7U

#if (defined CRYPTO_XVIX_XAIX_SW_PATCH_VERSION) /* to prevent double declaration */
#error CRYPTO_XVIX_XAIX_SW_PATCH_VERSION is already defined
#endif
/** \brief AUTOSAR module patch version */
#define CRYPTO_XVIX_XAIX_SW_PATCH_VERSION  42U

/*==================[declaration of types]========================================================*/

/*==================[declaration of external constants]===========================================*/

/*==================[declaration of external variables]===========================================*/

/*==================[declaration of external functions]===========================================*/

#endif /* #ifndef CRYPTO_XVIX_XAIX_VERSION_H */

/*==================[end of file]=================================================================*/

