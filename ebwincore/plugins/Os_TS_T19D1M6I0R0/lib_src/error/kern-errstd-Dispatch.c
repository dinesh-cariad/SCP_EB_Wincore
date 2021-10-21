/* kern-errstd-Dispatch.c - kernel error database for STATUS=STANDARD
 *
 * This is a generated file - DO NOT EDIT
 *
 * Generated by errdb-to-c.pl on 2020-03-03 16:30
 *
 * (c) 2020 Elektrobit Automotive GmbH
*/

#include <Os_kernel.h>
#include <Os_error.h>

#include <memmap/Os_mm_const_begin.h>
static const os_errorentry_t OS_errtblDispatch[2] =
{
  {
    OS_E_STACKPROT,
    OS_ERROR_KernelStackOverflow,
    OS_ACTION_SHUTDOWN|OS_ACTION_PROTECTIONHOOK
  },
  {
    OS_E_STACKPROT,
    OS_ERROR_TaskStackOverflow,
    OS_ACTION_SHUTDOWN|OS_ACTION_PROTECTIONHOOK
  }
};

const os_serviceinfo_t OS_svc_Dispatch =
{
    OS_errtblDispatch,
    OS_SID_Dispatch,
    2
};
#include <memmap/Os_mm_const_end.h>
