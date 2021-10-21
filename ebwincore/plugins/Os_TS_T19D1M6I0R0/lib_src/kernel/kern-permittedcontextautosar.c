/* kern-permittedcontextautosar.c
 *
 * This file contains the OS_permittedContextAutosar array
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-permittedcontextautosar.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_const_begin.h>

/* OS_permittedContextAutosar
 *
 * This table implements the strict Autosar rules for calling context.
 * The rules are taken from Table 5 "Allowed Calling Context For OS
 * Service Calls" in Autosar OS 1.3. In some cases OS_ININTERNALMASK
 * has been added to permit the kernel itself to call the function,
 * for example ActivateTask and SetRelAlarm during startup.
 *
 * The table contains an entry for each system service, identified by its
 * SID (see Os_error.h).
 *
 * A system-service's entry in this table contains a bit for each possible
 * calling context. If the bit is 1, the service is permitted to be called
 * from the context.
 *
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext, 2
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.ActivateTask, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.CallTrustedFunction, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.CancelAlarm, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.ChainTask, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.CheckISRMemoryAccess, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.CheckObjectAccess, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.CheckObjectOwnership, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.CheckTaskMemoryAccess, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.ClearEvent, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.DisableAllInterrupts, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.EnableAllInterrupts, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.GetActiveApplicationMode, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.GetAlarm, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.GetAlarmBase, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.GetApplicationID, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.GetCounterValue, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.GetElapsedCounterValue, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.GetEvent, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.GetISRID, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.GetResource, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.GetScheduleTableStatus, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.GetTaskID, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.GetTaskState, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.IncrementCounter, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.NextScheduleTable, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.ReleaseResource, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.ResumeAllInterrupts, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.ResumeOSInterrupts, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.Schedule, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.SetAbsAlarm, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.SetEvent, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.SetRelAlarm, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.SetScheduleTableAsync, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.ShutdownOS, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.StartOS, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.StartScheduleTableAbs, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.StartScheduleTableRel, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.StartScheduleTableSynchron, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.StopScheduleTable, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.SuspendAllInterrupts, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.SuspendOSInterrupts, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.SyncScheduleTable, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.TerminateApplication, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.TerminateTask, 1
 * !LINKSTO Kernel.Autosar.ServiceErrors.Context.PermittedContext.WaitEvent, 1
*/
const os_callermask_t OS_permittedContextAutosar[OS_SID_XXX] =
{
	OS_INTASKMASK|						/* 0: GetApplicationId		*/
	OS_INCAT2MASK|
	OS_INERRORHOOKMASK|
	OS_INPRETASKHOOKMASK|
	OS_INPOSTTASKHOOKMASK|
	OS_INSTARTUPHOOKMASK|
	OS_INSHUTDOWNHOOKMASK|
	OS_INACBMASK|
	OS_INPROTECTIONHOOKMASK,

	OS_INTASKMASK|						/* GetIsrId					*/
	OS_INCAT2MASK|
	OS_INERRORHOOKMASK|
	OS_INPROTECTIONHOOKMASK,

	OS_INTASKMASK|						/* CallTrustedFunction		*/
	OS_INCAT2MASK,

	OS_INTASKMASK|						/* CheckIsrMemoryAccess		*/
	OS_INCAT2MASK|
	OS_INERRORHOOKMASK|
	OS_INPROTECTIONHOOKMASK,

	OS_INTASKMASK|						/* CheckTaskMemoryAccess	*/
	OS_INCAT2MASK|
	OS_INERRORHOOKMASK|
	OS_INPROTECTIONHOOKMASK,

	OS_INTASKMASK|						/* CheckObjectAccess		*/
	OS_INCAT2MASK|
	OS_INERRORHOOKMASK|
	OS_INPROTECTIONHOOKMASK,

	OS_INTASKMASK|						/* CheckObjectOwnership		*/
	OS_INCAT2MASK|
	OS_INERRORHOOKMASK|
	OS_INPROTECTIONHOOKMASK,

	OS_INTASKMASK|						/* StartScheduleTableRel	*/
	OS_INCAT2MASK|
	OS_ININTERNALMASK,

	OS_INTASKMASK|						/* StartScheduleTableAbs	*/
	OS_INCAT2MASK|
	OS_ININTERNALMASK,

	OS_INTASKMASK|						/* StopScheduleTable		*/
	OS_INCAT2MASK,

	OS_INTASKMASK|						/* ChainScheduleTable/NextScheduleTable	*/
	OS_INCAT2MASK,

	OS_INTASKMASK|						/* StartScheduleTableSynchron	*/
	OS_INCAT2MASK|
	OS_ININTERNALMASK,

	OS_INTASKMASK|						/* SyncScheduleTable		*/
	OS_INCAT2MASK,

	OS_INTASKMASK|						/* SetScheduleTableAsync	*/
	OS_INCAT2MASK,

	OS_INTASKMASK|						/* GetScheduleTableStatus	*/
	OS_INCAT2MASK,

	OS_INTASKMASK|						/* Advance/IncrementCounter	*/
	OS_INCAT2MASK|
	OS_ININTERNALMASK,

	OS_INTASKMASK|						/* GetCounterValue	*/
	OS_INCAT2MASK,

	OS_INTASKMASK|						/* GetElapsedCounterValue/GetElapsedValue	*/
	OS_INCAT2MASK,

	OS_INTASKMASK|						/* TerminateApplication		*/
	OS_INCAT2MASK|
	OS_INERRORHOOKMASK,

	OS_INTASKMASK|						/* AllowAccess				*/
	OS_INCAT2MASK,

	OS_INTASKMASK|						/* 20: GetApplicationState	*/
	OS_INCAT2MASK|
	OS_INERRORHOOKMASK|
	OS_INPRETASKHOOKMASK|
	OS_INPOSTTASKHOOKMASK|
	OS_INSTARTUPHOOKMASK|
	OS_INSHUTDOWNHOOKMASK|
	OS_INPROTECTIONHOOKMASK,

	OS_INALLMASK,						/* UnknownSyscall			*/

	OS_INTASKMASK|						/* ActivateTask				*/
	OS_INCAT2MASK|
	OS_ININTERNALMASK,

	OS_INTASKMASK,						/* TerminateTask			*/

	OS_INTASKMASK,						/* ChainTask				*/

	OS_INTASKMASK,						/* Schedule					*/

	OS_INTASKMASK|						/* GetTaskId				*/
	OS_INCAT2MASK|
	OS_INERRORHOOKMASK|
	OS_INPRETASKHOOKMASK|
	OS_INPOSTTASKHOOKMASK|
	OS_INPROTECTIONHOOKMASK|
	OS_ININTERNALMASK,

	OS_INTASKMASK|						/* GetTaskState				*/
	OS_INCAT2MASK|
	OS_INERRORHOOKMASK|
	OS_INPRETASKHOOKMASK|
	OS_INPOSTTASKHOOKMASK,

	OS_INTASKMASK|						/* SuspendOSInterrupts,SuspendAllInterrupts,DisableAllInterrupts	*/
	OS_INCAT2MASK|
	OS_INCAT1MASK|
	OS_INERRORHOOKMASK|
	OS_INPRETASKHOOKMASK|
	OS_INPOSTTASKHOOKMASK|
	OS_INSTARTUPHOOKMASK|
	OS_INSHUTDOWNHOOKMASK|
	OS_INPROTECTIONHOOKMASK|
	OS_INACBMASK|
	OS_INBOOTMASK,

	OS_INTASKMASK|						/* ResumeOSInterrupts,ResumeAllInterrupts,EnableAllInterrupts		*/
	OS_INCAT2MASK|
	OS_INCAT1MASK|
	OS_INERRORHOOKMASK|
	OS_INPRETASKHOOKMASK|
	OS_INPOSTTASKHOOKMASK|
	OS_INSTARTUPHOOKMASK|
	OS_INSHUTDOWNHOOKMASK|
	OS_INPROTECTIONHOOKMASK|
	OS_INACBMASK|
	OS_INBOOTMASK,

	OS_INTASKMASK|						/* GetResource				*/
	OS_INCAT2MASK,

	OS_INTASKMASK|						/* ReleaseResource			*/
	OS_INCAT2MASK,

	OS_INTASKMASK|						/* SetEvent					*/
	OS_INCAT2MASK|
	OS_ININTERNALMASK,

	OS_INTASKMASK,						/* ClearEvent				*/

	OS_INTASKMASK|						/* GetEvent					*/
	OS_INCAT2MASK|
	OS_INERRORHOOKMASK|
	OS_INPRETASKHOOKMASK|
	OS_INPOSTTASKHOOKMASK,

	OS_INTASKMASK,						/* WaitEvent				*/

	OS_INTASKMASK|						/* GetAlarmBase				*/
	OS_INCAT2MASK|
	OS_INERRORHOOKMASK|
	OS_INPRETASKHOOKMASK|
	OS_INPOSTTASKHOOKMASK,

	OS_INTASKMASK|						/* GetAlarm					*/
	OS_INCAT2MASK|
	OS_INERRORHOOKMASK|
	OS_INPRETASKHOOKMASK|
	OS_INPOSTTASKHOOKMASK|
	OS_ININTERNALMASK,

	OS_INTASKMASK|						/* SetRelAlarm				*/
	OS_INCAT2MASK|
	OS_ININTERNALMASK,

	OS_INTASKMASK|						/* SetAbsAlarm				*/
	OS_INCAT2MASK|
	OS_ININTERNALMASK,

	OS_INTASKMASK|						/* 40: CancelAlarm			*/
	OS_INCAT2MASK,

	OS_INTASKMASK|						/* GetActiveApplicationMode	*/
	OS_INCAT2MASK|
	OS_INERRORHOOKMASK|
	OS_INPRETASKHOOKMASK|
	OS_INPOSTTASKHOOKMASK|
	OS_INSTARTUPHOOKMASK|
	OS_INSHUTDOWNHOOKMASK,

	OS_INBOOTMASK,						/* StartOs					*/

	OS_INTASKMASK|						/* ShutdownOs				*/
	OS_INCAT2MASK|
	OS_INERRORHOOKMASK|
	OS_INSTARTUPHOOKMASK|
	OS_ININTERNALMASK,

	OS_INTASKMASK|						/* GetStackInfo				*/
	OS_INCAT2MASK,

	OS_INALLMASK,						/* DisableInterruptSource. Not used, but must be here
										   because the ID still exists. */

	OS_INALLMASK,						/* EnableInterruptSource. Not used, but must be here
										   because the ID still exists. */

	OS_INTASKMASK|						/* 47: TryToGetSpinlock */
	OS_INCAT2MASK,

	OS_INTASKMASK|						/* 48: ReleaseSpinlock */
	OS_INCAT2MASK,

	OS_INTASKMASK|						/* 49: ShutdownAllCores */
	OS_INCAT2MASK|
	OS_ININTERNALMASK,

	OS_INALLMASK,						/* 50: GetCpuLoad */
	OS_INALLMASK						/* 51: ResetPeakCpuLoad */
	/* 52: OS_SID_XXX */

};

/* Improve: ASCOS-3456 GetCoreID (SID 50 in Os 5.1) isn't a syscall at the moment.
 * This works on TRICORE, but might not work on other architectures.
*/

#if (OS_SID_XXX != 52)
#error "Incorrect number of SIDs in OS_permittedContextAutosar"
#endif

#include <memmap/Os_mm_const_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
