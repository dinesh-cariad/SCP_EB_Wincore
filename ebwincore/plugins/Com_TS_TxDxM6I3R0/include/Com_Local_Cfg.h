/**
 * \file
 *
 * \brief AUTOSAR Com
 *
 * This file contains the implementation of the AUTOSAR
 * module Com.
 *
 * \version 6.3.50
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef COM_LOCAL_CFG_H
#define COM_LOCAL_CFG_H

/* MISRA-C:2012 Deviation List
 *
 *
 * MISRAC2012-1) Deviated Rule: 21.1 (required)
 * #define and #undef shall not be used on a reserved
 * identifier or reserved macro name.
 *
 * Reason:
 * The COM module prefix within the macro as well as
 * the macro guards which prevents double declaration
 * ensures uniqueness of the COM macros starting with a
 * underline.
 *
 */

/*==================[inclusions]============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR Standard types */

/*==================[macros]================================================*/

#if (defined _COM_INLINE_)        /* To prevent double declaration */
#error _COM_INLINE_ is already defined
#endif /* (defined _COM_INLINE_) */

/* Deviation MISRAC2012-1 */
#define _COM_INLINE_ INLINE

#if (defined _COM_STATIC_)        /* To prevent double declaration */
#error _COM_STATIC_ is already defined
#endif /* (defined _COM_STATIC_) */

/* Deviation MISRAC2012-1 */
#define _COM_STATIC_ STATIC


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef COM_LOCAL_CFG_H */
/*==================[end of file]===========================================*/
