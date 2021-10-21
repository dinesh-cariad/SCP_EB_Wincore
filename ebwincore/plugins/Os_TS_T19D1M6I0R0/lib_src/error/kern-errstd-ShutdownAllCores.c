/* kern-errstd-ShutdownAllCores.c - kernel error database for STATUS=STANDARD
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
static const os_errorentry_t OS_errtblShutdownAllCores[3] =
{
  {
    OS_E_CALLLEVEL,
    OS_ERROR_WrongContext,
    OS_ACTION_QUARANTINE|OS_ACTION_ERRORHOOK_EB
  },
  {
    OS_E_INTDISABLE,
    OS_ERROR_InterruptDisabled,
    OS_ACTION_QUARANTINE|OS_ACTION_ERRORHOOK_EB
  },
  {
    OS_E_NOFUNC,
    OS_ERROR_NotTrusted,
    OS_ACTION_QUARANTINE|OS_ACTION_ERRORHOOK_EB
  }
};

const os_serviceinfo_t OS_svc_ShutdownAllCores =
{
    OS_errtblShutdownAllCores,
    OS_SID_ShutdownAllCores,
    3
};
#include <memmap/Os_mm_const_end.h>
