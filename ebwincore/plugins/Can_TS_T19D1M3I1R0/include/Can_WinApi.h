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
#ifndef CAN_WINAPI_H
#define CAN_WINAPI_H

/*==[Includes]================================================================*/

/*==[Macros]==================================================================*/

/*==[Types]===================================================================*/

/*==[Declaration of constants with external linkage]==========================*/

/*==[Declaration of variables with external linkage]==========================*/

/*==[Declaration of functions with external linkage]==========================*/
extern void Can_InitializeCriticalSection(void);

extern void Can_EnterCriticalSection(void);

extern void Can_LeaveCriticalSection(void);

#endif /* CAN_WINAPI_H */
