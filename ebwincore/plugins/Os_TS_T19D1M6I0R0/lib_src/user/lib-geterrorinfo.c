/* lib-geterrorinfo.c
 *
 * This file contains the OS_GetErrorInfo function.
 * It retrieves the error status information for the current core.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: lib-geterrorinfo.c 22444 2015-10-08 14:07:20Z stpo8218 $
*/

#include <Os_api.h>
#include <Os_error.h>
#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

/*!
 * OS_GetErrorInfo - return error status information.
 * Comment: This function is not called OS_GetErrorStatus, since
 * this was a macro in 5.1.
*/

/*
 * !LINKSTO Kernel.Feature.ErrorStatus.API.GetErrorInfo, 1
*/
const os_errorstatus_t * OS_GetErrorInfo(void)
{
	return &(OS_GetKernelData()->errorStatus);
}

/* API entries for User's Guide
 * CHECK: NOPARSE

<api API="OS_USER">
  <name>OS_GetErrorInfo</name>
  <synopsis>Get error status information</synopsis>
  <syntax>
    const os_errorstatus_t * OS_GetErrorInfo()
  </syntax>
  <description>
    <code>OS_GetErrorInfo()</code> returns a pointer to
    the error information status structure of the current core.
    The information in this structure is valid during the
    <code>ErrorHook()</code> and <code>ProtectionHook()</code>.
    It will be overwritten, once the next error occurs.
  </description>
  <availability>
    No restrictions.
  </availability>
  <returns>=Pointer to the error information structure.</returns>
</api>

 * CHECK: PARSE
*/

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/

