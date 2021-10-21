/**
 * \file
 *
 * \brief AUTOSAR Can
 *
 * This file contains the implementation of the AUTOSAR
 * module Can.
 *
 * \version 3.1.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef CAN_VERSION_H
#define CAN_VERSION_H
/*==[Includes]================================================================*/

/*==[Macros]==================================================================*/

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined CAN_VENDOR_ID) /* to prevent double declaration */
#error CAN_VENDOR_ID is already defined
#endif /* (defined CAN_VENDOR_ID) */

/** \brief Module Information.
 **
 ** AUTOSAR vendor identification.
 */
#define CAN_VENDOR_ID 1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined CAN_MODULE_ID) /* to prevent double declaration */
#error CAN_MODULE_ID already defined
#endif /* (defined CAN_MODULE_ID) */

/** \brief Module Information.
 **
 ** AUTOSAR module identification.
 */
#define CAN_MODULE_ID 80U

#if (defined CAN_INSTANCE_ID) /* to prevent double declaration */
#error CAN_INSTANCE_ID already defined
#endif /* (defined CAN_INSTANCE_ID) */

/** \brief Can instance id */
#define CAN_INSTANCE_ID 0U

/*------------------[AUTOSAR specification version identification]----------*/

#if (defined CAN_AR_MAJOR_VERSION) /* to prevent double declaration */
#error CAN_AR_MAJOR_VERSION already defined
#endif /* (defined CAN_AR_MAJOR_VERSION) */

/** \brief Module Information.
 **
 ** AUTOSAR specification major version.
 */
#define CAN_AR_MAJOR_VERSION  4U

#if (defined CAN_AR_MINOR_VERSION) /* to prevent double declaration */
#error CAN_AR_MINOR_VERSION already defined
#endif /* (define CAN_AR_MINOR_VERSION) */

/** \brief Module Information.
 **
 ** AUTOSAR specification minor version.
 */
#define CAN_AR_MINOR_VERSION  0U

#if (defined CAN_AR_PATCH_VERSION) /* to prevent double declaration */
#error CAN_AR_PATCH_VERSION already defined
#endif /* (defined CAN_aR_PATCH_VERSION) */

/** \brief Module Information.
 **
 ** AUTOSAR specification patch version.
 */
#define CAN_AR_PATCH_VERSION  0U

/*------------------[AUTOSAR module version identification]-----------------*/

#if (defined CAN_SW_MAJOR_VERSION) /* to prevent double declaration */
#error CAN_SW_MAJOR_VERSION already defined
#endif /* (defined CAN_SW_MAJOR_VERSION) */

/** \brief Module Information.
  **
  ** AUTOSAR module major version.
  */
#define CAN_SW_MAJOR_VERSION  3U

#if (defined CAN_SW_MINOR_VERSION) /* to prevent double declaration */
#error CAN_SW_MINOR_VERSION already defined
#endif /* (defined CAN_SW_MINOR_VERSION) */

/** \brief Module Information.
 **
 ** AUTOSAR module minor version.
 */
#define CAN_SW_MINOR_VERSION  1U

#if (defined CAN_SW_PATCH_VERSION) /* to prevent double declaration */
#error CAN_SW_PATCH_VERSION already defined
#endif /* (defined CAN_SW_PATCH_VERSION) */

/** \brief Module Information.
 **
 ** AUTOSAR module patch version.
 */
#define CAN_SW_PATCH_VERSION  4U

/*==[Types]===================================================================*/

/*==[Declaration of constants with external linkage]==========================*/

/*==[Declaration of variables with external linkage]==========================*/

/*==[Declaration of functions with external linkage]==========================*/

#endif /* CAN_VERSION_H */
