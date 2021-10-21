/**
 * \file
 *
 * \brief AUTOSAR ApplTemplates
 *
 * This file contains the implementation of the AUTOSAR
 * module ApplTemplates.
 *
 * \version 6.8.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#include <os.h>
#include <Os_config.h>
#include <Os_kernel.h>

/*
#include <WINDOWS/Os_WINDOWS_WIN32X86.h>
*/

void ArchInitHardware(void);

/*
 * ArchInitHardware()
 *
 * Initialise the hardware for the demo app.
*/
void ArchInitHardware(void)
{
#if (OS_CPU == OS_WIN32X86)
   /* do something */
#else
#error "OS_CPU not properly defined. Check your makefiles!"
#endif

}

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
