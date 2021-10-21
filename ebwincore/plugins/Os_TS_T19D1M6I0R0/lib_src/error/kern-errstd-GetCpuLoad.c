/* kern-errstd-GetCpuLoad.c - kernel error database for STATUS=STANDARD
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
static const os_errorentry_t OS_errtblGetCpuLoad[2] =
{
  {
    OS_E_CORE,
    OS_ERROR_CoreIsDown,
    OS_ACTION_RETURN|OS_ACTION_ERRORHOOK
  },
  {
    OS_E_ID,
    OS_ERROR_InvalidCoreId,
    OS_ACTION_RETURN|OS_ACTION_ERRORHOOK
  }
};

const os_serviceinfo_t OS_svc_GetCpuLoad =
{
    OS_errtblGetCpuLoad,
    OS_SID_GetCpuLoad,
    2
};
#include <memmap/Os_mm_const_end.h>