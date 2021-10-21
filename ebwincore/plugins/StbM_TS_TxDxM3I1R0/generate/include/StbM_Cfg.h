[!AUTOSPACING!]
#ifndef STBM_CFG_H
#define STBM_CFG_H

/**
 * \file
 *
 * \brief AUTOSAR StbM
 *
 * This file contains the implementation of the AUTOSAR
 * module StbM.
 *
 * \version 3.1.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/** \addtogroup Synchronized Time Base Manager
 ** @{ */

/*==[Includes]================================================================*/

#include <StbM_Gen_Common.h>

/*==[Macros]==================================================================*/

#if (defined STBM_VERSION_INFO_API)
#error STBM_VERSION_INFO_API is already defined.
#endif
/** \brief Enables/Disables the StbM_GetVersionInfo API. */
#define STBM_VERSION_INFO_API                     STD_[!IF "StbMGeneral/StbMVersionInfoApi = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

/** \brief Definitions of symbolic names for all the synchronized (time base id < 16) and offset
 ** (time base id >= 16) time base identifiers */
[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*, './StbMSynchronizedTimeBaseIdentifier')"!]
  [!VAR "prefix" = "string('StbMConf_StbMSynchronizedTimeBase_')"!]
  [!VAR "timeBaseId" = "num:i(node:value(StbMSynchronizedTimeBaseIdentifier))"!]
  [!VAR "timeBaseName" = "node:name(.)"!]
  [!VAR "define1" = "concat($prefix, $timeBaseName)"!]
  [!VAR "define2" = "concat($define1, string('  '))"!]
  [!VAR "define3" = "concat($define2, $timeBaseId)"!]
#define [!"$define3"!]U
[!ENDLOOP!]

/** \brief Definitions of symbolic names for all the Notification Customers */
[!LOOP "as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*/StbMNotificationCustomer/*"!]
  [!VAR "prefix" = "string('StbMConf_StbMNotificationCustomer_')"!]
  [!VAR "customerId" = "num:i(node:value(StbMNotificationCustomerId))"!]
  [!VAR "customerName" = "node:name(.)"!]
  [!VAR "define1" = "concat($prefix, $customerName)"!]
  [!VAR "define2" = "concat($define1, string('  '))"!]
  [!VAR "define3" = "concat($define2, $customerId)"!]
#define [!"$define3"!]U
[!ENDLOOP!]

/*==[Types]===================================================================*/

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/

/** @} doxygen end group definition */
/*==================[end of file]===========================================*/


#endif /* ifndef STBM_CFG_H */
