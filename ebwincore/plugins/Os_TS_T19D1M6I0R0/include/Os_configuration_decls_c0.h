/* Os_configuration_c0_defaults.h
 *
 * This file provides configuration dependent declarations
 * and macro definitions for core C0.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_configuration_decls_c0.h 30751 2019-08-30 10:26:47Z ache271795 $
*/

#ifndef OS_CONFIGURATION_DECLS_C0_H
#define OS_CONFIGURATION_DECLS_C0_H

#include <Os_configuration_decls.h>

#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
#include <Os_panic.h>
#endif /* OS_KERNEL_TYPE == OS_MICROKERNEL */

/* If no core properties are defined, we assume, that this core isn't used.
*/
#ifndef OS_PROPERTIES_CORE0
#define OS_PROPERTIES_CORE0 OS_U(0)
#endif


/* Define default values in case this core is not active.
*/
#if ((OS_PROPERTIES_CORE0 & OS_COREPROP_USED_BIT) != OS_COREPROP_USED_BIT)

#if (OS_KERNEL_TYPE != OS_MICROKERNEL)

#ifndef OS_NAPPS_CORE0
#define OS_NAPPS_CORE0 OS_U(0)
#endif

#ifndef OS_NTASKACCOUNTING_CORE0
#define OS_NTASKACCOUNTING_CORE0 OS_U(0)
#endif

#ifndef OS_NTASKS_CORE0
#define OS_NTASKS_CORE0 OS_U(0)
#endif

#ifndef OS_NPRIORITIES_CORE0
#define OS_NPRIORITIES_CORE0 OS_U(0)
#endif

#ifndef OS_NPRIORITYQUEUEDYNAMICS_CORE0
#define OS_NPRIORITYQUEUEDYNAMICS_CORE0 OS_U(0)
#endif

#ifndef OS_NPRIORITYSLOTS_CORE0
#define OS_NPRIORITYSLOTS_CORE0 OS_U(0)
#endif

#ifndef OS_NTASKACTIVATIONS_CORE0
#define OS_NTASKACTIVATIONS_CORE0 OS_U(0)
#endif

#ifndef OS_NRESOURCES_CORE0
#define OS_NRESOURCES_CORE0 OS_U(0)
#endif

#ifndef OS_NISRACCOUNTING_CORE0
#define OS_NISRACCOUNTING_CORE0 OS_U(0)
#endif

#ifndef OS_NINTERRUPTS_CORE0
#define OS_NINTERRUPTS_CORE0 OS_U(0)
#endif

#ifndef OS_NRATEMONS_CORE0
#define OS_NRATEMONS_CORE0 OS_U(0)
#endif

#ifndef OS_NRATETIMERS_CORE0
#define OS_NRATETIMERS_CORE0 OS_U(0)
#endif

#endif /* OS_KERNEL_TYPE != OS_MICROKERNEL */

#ifndef OS_NCOUNTERS_CORE0
#define OS_NCOUNTERS_CORE0 OS_U(0)
#endif

#ifndef OS_NALARMS_CORE0
#define OS_NALARMS_CORE0 OS_U(0)
#endif

#ifndef OS_NSCHEDULES_CORE0
#define OS_NSCHEDULES_CORE0 OS_U(0)
#endif

#ifndef OS_NHWTIMERS_CORE0
#define OS_NHWTIMERS_CORE0 OS_U(0)
#endif

#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
#define OS_QMOS_PANICCODE_PTR_CORE0 OS_NULL
#endif /* OS_KERNEL_TYPE == OS_MICROKERNEL */

#endif /* not USED */


/* Deduced configuration constants.
*/

#define OS_TOTALALARMS_CORE0 (OS_NALARMS_CORE0 + OS_NSCHEDULES_CORE0)

#if (OS_KERNEL_TYPE != OS_MICROKERNEL)

#define OS_MAXPRIO_CORE0 ((OS_NPRIORITIES_CORE0 == 0) ? 0 : (OS_NPRIORITIES_CORE0 - 1))

#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
#if (OS_NTASKS == 0)
#define OS_NSLAVEWORDS_CORE0	OS_U(0)
#else /* OS_NTASKS != 0 */
#define OS_NSLAVEWORDS_CORE0	(((OS_NPRIORITIES_CORE0 + OS_CLZWORD_NBITS) - 1) / OS_CLZWORD_NBITS)
#endif /* OS_NTASKS == 0 */
#endif /* OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE */

#if (OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST)
#if (OS_NTASKS == 0)
#define OS_TASKACT_CORE0		OS_NULL
#else /* OS_NTASKS != 0 */
#define OS_TASKACT_CORE0		OS_NULL,OS_TASKACTIVATIONS_CORE0
#endif /* OS_NTASKS == 0 */
#endif /* OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST */

#endif /* OS_KERNEL_TYPE != OS_MICROKERNEL */


/* Choose symbols for single-core optimization.
*/
#if ((OS_N_CORES == 1) && (0 == OS_INITCORE))
#define OS_KERNELDATA_OBJ_CORE0			OS_kernelData
#define OS_TASKPTRS_OBJ_CORE0			OS_taskPtrs
#define OS_TASKACTIVATIONS_OBJ_CORE0	OS_taskActivations
#define OS_PRIORITYQUEUE_OBJ_CORE0		OS_priorityQueue
#define OS_PRIORITYSLOT_OBJ_CORE0		OS_prioritySlot
#define OS_SLAVEPRIOWORD_OBJ_CORE0		OS_slavePrioWord
#else
#define OS_KERNELDATA_OBJ_CORE0			OS_kernelData_core0
#define OS_TASKPTRS_OBJ_CORE0			OS_taskPtrs_core0
#define OS_TASKACTIVATIONS_OBJ_CORE0	OS_taskActivations_core0
#define OS_PRIORITYQUEUE_OBJ_CORE0		OS_priorityQueue_core0
#define OS_PRIORITYSLOT_OBJ_CORE0		OS_prioritySlot_core0
#define OS_SLAVEPRIOWORD_OBJ_CORE0		OS_slavePrioWord_core0
#endif


/* Declare core local symbols.
*/
#ifndef OS_ASM
#if ((OS_PROPERTIES_CORE0 & OS_COREPROP_USED_BIT) != OS_COREPROP_USED_BIT)

extern os_uint8_t OS_unused_configuration_c0;
#define OS_SLAVEWORDPTR_CORE0			OS_NULL
#define OS_PRIOSLOTPTR_CORE0			OS_NULL
#define OS_PRIOTITYQUEUEPTR_CORE0		OS_NULL
#define OS_PTASKPTRS_CORE0				OS_NULL
#define OS_TASKACTIVATIONSPTR_CORE0		OS_NULL
#define OS_CPULOADBUSYBUFFERPTR_CORE0	OS_NULL
#define OS_MESSAGEQUEUEPTR_CORE0		OS_NULL
#define OS_KERNELDATAPTR_CORE0			OS_NULL

#else /* USED */

#if (OS_KERNEL_TYPE != OS_MICROKERNEL)

#if (OS_NAPPS_CORE0 != 0)
extern os_appdynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core0.appDynamic)
		OS_appDynamic_core0[OS_NAPPS_CORE0]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core0.appDynamic);
#endif
#if (OS_NTASKACCOUNTING_CORE0 != 0)
extern os_taskaccounting_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core0.taskAccounting)
		OS_taskAccounting_core0[OS_NTASKACCOUNTING_CORE0]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core0.taskAccounting);
#endif
#if (OS_NTASKS_CORE0 != 0)
extern os_taskdynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core0.taskDynamic)
		OS_taskDynamic_core0[OS_NTASKS_CORE0]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core0.taskDynamic);
#endif

#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
#if (OS_NSLAVEWORDS_CORE0 == 0)
#define OS_SLAVEWORDPTR_CORE0	OS_NULL
#else
#define OS_SLAVEWORDPTR_CORE0	(&OS_SLAVEPRIOWORD_OBJ_CORE0[0])
#if (OS_N_CORES != 1)
extern os_clzword_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core0.slavePrioWord)
		OS_slavePrioWord_core0[OS_NSLAVEWORDS_CORE0]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core0.slavePrioWord);
#endif
#endif
#if (OS_NPRIORITYSLOTS_CORE0 == 0)
#define OS_PRIOSLOTPTR_CORE0 OS_NULL
#else
#if (OS_N_CORES != 1)
extern os_priorityqueueentry_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core0.prioritySlot)
		OS_prioritySlot_core0[OS_NPRIORITYSLOTS_CORE0]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core0.prioritySlot);
#endif
#define OS_PRIOSLOTPTR_CORE0 (&OS_PRIORITYSLOT_OBJ_CORE0[0])
#endif
#if (OS_NPRIORITIES_CORE0 == 0)
#define OS_PRIOTITYQUEUEPTR_CORE0 OS_NULL
#else
#if (OS_N_CORES != 1)
extern os_priorityqueue_t const OS_priorityQueue_core0[OS_NPRIORITIES_CORE0];
#endif
#define OS_PRIOTITYQUEUEPTR_CORE0 (&OS_PRIORITYQUEUE_OBJ_CORE0[0])
#endif
#endif /* OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE */

#if (OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST)
#if (OS_NTASKACTIVATIONS_CORE0 == 0)
#define OS_PTASKPTRS_CORE0 OS_NULL
#else
#if (OS_N_CORES != 1)
extern const os_task_t * const OS_taskPtrs_core0[OS_NTASKACTIVATIONS_CORE0 + 1];
#endif
#define OS_PTASKPTRS_CORE0 (&OS_TASKPTRS_OBJ_CORE0[0])
#endif
#if (OS_NTASKACTIVATIONS_CORE0 == 0)
#define OS_TASKACTIVATIONSPTR_CORE0 OS_NULL
#else
#if (OS_N_CORES != 1)
extern os_tasklink_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core0.taskActivations)
		OS_taskActivations_core0[OS_NTASKACTIVATIONS_CORE0 + 1]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core0.taskActivations);
#endif
#define OS_TASKACTIVATIONSPTR_CORE0 (&OS_TASKACTIVATIONS_OBJ_CORE0[0])
#endif
#endif /* OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST */

#if (OS_NRESOURCES_CORE0 != 0)
extern os_resourcedynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core0.resourceDynamic)
		OS_resourceDynamic_core0[OS_NRESOURCES_CORE0]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core0.resourceDynamic);
#endif
#if (OS_NISRACCOUNTING != 0)
#if (OS_NISRACCOUNTING_CORE0 != 0)
extern os_israccounting_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core0.isrAccounting)
		OS_isrAccounting_core0[OS_NISRACCOUNTING_CORE0]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core0.isrAccounting);
#endif
#endif /* OS_NISRACCOUNTING!=0 */
#if (OS_NINTERRUPTS_CORE0 != 0)
extern os_isrdynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core0.isrDynamic)
		OS_isrDynamic_core0[OS_NINTERRUPTS_CORE0]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core0.isrDynamic);
#endif

#if (OS_NRATEMONS != 0)
#if (OS_NRATEMONS_CORE0 != 0)
extern os_rateindex_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core0.rateIndex)
		OS_rateIndex_core0[OS_NRATEMONS_CORE0]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core0.rateIndex);
#endif
#if (OS_NRATETIMERS_CORE0 != 0)
extern os_timestamp_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core0.rateTimer)
		OS_rateTimer_core0[OS_NRATETIMERS_CORE0]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core0.rateTimer);
#endif
#endif /* OS_NRATEMONS != 0 */

#ifndef OS_EXCLUDE_CPULOAD
extern os_tick_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core0.cpuLoadBusyBuffer)
		OS_cpuLoadBusyBuffer_core0[OS_CPULOADCFG_NINTERVALS]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core0.cpuLoadBusyBuffer);
#define OS_CPULOADBUSYBUFFERPTR_CORE0 (&OS_cpuLoadBusyBuffer_core0[0])
#endif

#if (OS_N_CORES > 1)
extern os_messagequeue_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.shared.core0.messageQueue)
		OS_messageQueue_core0
		OS_SECTION_ATTRIB_POST_DECL(.bss.shared.core0.messageQueue);
#define OS_MESSAGEQUEUEPTR_CORE0 (&OS_messageQueue_core0)
#endif

#if (OS_N_CORES != 1)
extern os_kerneldata_t
		OS_SECTION_ATTRIB_PRE_DECL(.data.core0.kernelData)
		OS_kernelData_core0
		OS_SECTION_ATTRIB_POST_DECL(.data.core0.kernelData);
#endif
#define OS_KERNELDATAPTR_CORE0 (&OS_KERNELDATA_OBJ_CORE0)

#endif /* OS_KERNEL_TYPE != OS_MICROKERNEL */

#if (OS_NCOUNTERS_CORE0 != 0)
extern os_counterdynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core0.counterDynamic)
		OS_counterDynamic_core0[OS_NCOUNTERS_CORE0]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core0.counterDynamic);
#endif
#if (OS_TOTALALARMS_CORE0 != 0)
extern os_alarmdynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core0.alarmDynamic)
		OS_alarmDynamic_core0[OS_TOTALALARMS_CORE0]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core0.alarmDynamic);
#endif
#if (OS_NSCHEDULES_CORE0 != 0)
extern os_scheduledynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core0.scheduleDynamic)
		OS_scheduleDynamic_core0[OS_NSCHEDULES_CORE0]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core0.scheduleDynamic);
#endif
#if (OS_NHWTIMERS_CORE0 != 0)
extern os_timervalue_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core0.hwtLastValue)
		OS_hwtLastValue_core0[OS_NHWTIMERS_CORE0]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core0.hwtLastValue);
#endif

#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
extern volatile os_panic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core0.panicCode)
		OS_panicCode_core0
		OS_SECTION_ATTRIB_POST_DECL(.bss.core0.panicCode);
#define OS_QMOS_PANICCODE_PTR_CORE0 (&OS_panicCode_core0)
#endif /* OS_KERNEL_TYPE == OS_MICROKERNEL */

#endif /* USED */
#endif /* ASM */

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
