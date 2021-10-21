/* kern-erruni-TrapHandler.c - kernel error database for STATUS=UNIVERSAL
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
static const os_errorentry_t OS_errtblTrapHandler[7] =
{
  {
    OS_E_INTERNAL,
    OS_ERROR_UnknownInterrupt,
    OS_ACTION_RETURN|OS_ACTION_ERRORHOOK
  },
  {
    OS_E_TIMEPROT,
    OS_ERROR_TaskTimeBudgetExceeded,
    OS_ACTION_QUARANTINE|OS_ACTION_PROTECTIONHOOK
  },
  {
    OS_E_LOCKPROT,
    OS_ERROR_TaskResLockTimeExceeded,
    OS_ACTION_QUARANTINE|OS_ACTION_PROTECTIONHOOK
  },
  {
    OS_E_LOCKPROT,
    OS_ERROR_TaskIntLockTimeExceeded,
    OS_ACTION_QUARANTINE|OS_ACTION_PROTECTIONHOOK
  },
  {
    OS_E_TIMEPROT,
    OS_ERROR_IsrTimeBudgetExceeded,
    OS_ACTION_QUARANTINE|OS_ACTION_PROTECTIONHOOK
  },
  {
    OS_E_LOCKPROT,
    OS_ERROR_IsrResLockTimeExceeded,
    OS_ACTION_QUARANTINE|OS_ACTION_PROTECTIONHOOK
  },
  {
    OS_E_LOCKPROT,
    OS_ERROR_IsrIntLockTimeExceeded,
    OS_ACTION_QUARANTINE|OS_ACTION_PROTECTIONHOOK
  }
};

const os_serviceinfo_t OS_svc_TrapHandler =
{
    OS_errtblTrapHandler,
    OS_SID_TrapHandler,
    7
};
#include <memmap/Os_mm_const_end.h>
