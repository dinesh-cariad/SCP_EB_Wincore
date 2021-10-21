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

/*==[Includes]================================================================*/
#include <Can_WinApi.h>
#include <windows.h>

/*==[Macros]==================================================================*/

/*==[Types]===================================================================*/

/*==[Declaration of functions with internal linkage]==========================*/

/*==[Constants with internal linkage]=========================================*/

/*==[Variables with internal linkage]=========================================*/
static CRITICAL_SECTION Can_CriticalSection;

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/
void Can_InitializeCriticalSection(void)
{
  InitializeCriticalSection(&Can_CriticalSection);
}

void Can_EnterCriticalSection(void)
{
  EnterCriticalSection(&Can_CriticalSection);
}

void Can_LeaveCriticalSection(void)
{
  LeaveCriticalSection(&Can_CriticalSection);
}

/*==[Definition of functions with internal linkage]===========================*/
