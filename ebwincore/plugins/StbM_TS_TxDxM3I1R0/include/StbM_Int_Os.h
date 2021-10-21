#ifndef STBM_OS_INT_H
#define STBM_OS_INT_H

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

/*==[Includes]================================================================*/

/* !LINKSTO StbM.dsn.OsInclusion,1 */
#include <Os.h>

/*==[Macros]==================================================================*/

/* redirect symbols in BSW to appropriate Os functions */
#if (defined StbM_Os_GetCounterValue)
#error StbM_Os_GetCounterValue is already defined.
#endif
#define StbM_Os_GetCounterValue GetCounterValue

#if (defined StbM_Os_GetScheduleTableStatus)
#error StbM_Os_GetScheduleTableStatus is already defined.
#endif
#define StbM_Os_GetScheduleTableStatus GetScheduleTableStatus

#if (defined StbM_Os_SyncScheduleTable)
#error StbM_Os_SyncScheduleTable is already defined.
#endif
#define StbM_Os_SyncScheduleTable SyncScheduleTable

/*==[Types]===================================================================*/

/*==[Declaration of functions with internal linkage]==========================*/

/*==[Constants with internal linkage]=========================================*/

/*==[Variables with internal linkage]=========================================*/

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/

/*==[Definition of functions with internal linkage]===========================*/

/*==================[end of file]===========================================*/


#endif /* ifndef STBM_OS_INT_H */
