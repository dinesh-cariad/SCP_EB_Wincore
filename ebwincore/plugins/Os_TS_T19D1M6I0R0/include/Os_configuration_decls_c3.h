/* Os_configuration_c3_defaults.h
 *
 * This file provides configuration dependent declarations
 * and macro definitions for core C3.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_configuration_decls_c3.h 30751 2019-08-30 10:26:47Z ache271795 $
*/

#ifndef OS_CONFIGURATION_DECLS_C3_H
#define OS_CONFIGURATION_DECLS_C3_H

#include <Os_configuration_decls.h>

#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
#include <Os_panic.h>
#endif /* OS_KERNEL_TYPE == OS_MICROKERNEL */

/* If no core properties are defined, we assume, that this core isn't used.
*/
#ifndef OS_PROPERTIES_CORE3
#define OS_PROPERTIES_CORE3 OS_U(0)
#endif


/* Define default values in case this core is not active.
*/
#if ((OS_PROPERTIES_CORE3 & OS_COREPROP_USED_BIT) != OS_COREPROP_USED_BIT)

#if (OS_KERNEL_TYPE != OS_MICROKERNEL)

#ifndef OS_NAPPS_CORE3
#define OS_NAPPS_CORE3 OS_U(0)
#endif

#ifndef OS_NTASKACCOUNTING_CORE3
#define OS_NTASKACCOUNTING_CORE3 OS_U(0)
#endif

#ifndef OS_NTASKS_CORE3
#define OS_NTASKS_CORE3 OS_U(0)
#endif

#ifndef OS_NPRIORITIES_CORE3
#define OS_NPRIORITIES_CORE3 OS_U(0)
#endif

#ifndef OS_NPRIORITYQUEUEDYNAMICS_CORE3
#define OS_NPRIORITYQUEUEDYNAMICS_CORE3 OS_U(0)
#endif

#ifndef OS_NPRIORITYSLOTS_CORE3
#define OS_NPRIORITYSLOTS_CORE3 OS_U(0)
#endif

#ifndef OS_NTASKACTIVATIONS_CORE3
#define OS_NTASKACTIVATIONS_CORE3 OS_U(0)
#endif

#ifndef OS_NRESOURCES_CORE3
#define OS_NRESOURCES_CORE3 OS_U(0)
#endif

#ifndef OS_NISRACCOUNTING_CORE3
#define OS_NISRACCOUNTING_CORE3 OS_U(0)
#endif

#ifndef OS_NINTERRUPTS_CORE3
#define OS_NINTERRUPTS_CORE3 OS_U(0)
#endif

#ifndef OS_NRATEMONS_CORE3
#define OS_NRATEMONS_CORE3 OS_U(0)
#endif

#ifndef OS_NRATETIMERS_CORE3
#define OS_NRATETIMERS_CORE3 OS_U(0)
#endif

#endif /* OS_KERNEL_TYPE != OS_MICROKERNEL */

#ifndef OS_NCOUNTERS_CORE3
#define OS_NCOUNTERS_CORE3 OS_U(0)
#endif

#ifndef OS_NALARMS_CORE3
#define OS_NALARMS_CORE3 OS_U(0)
#endif

#ifndef OS_NSCHEDULES_CORE3
#define OS_NSCHEDULES_CORE3 OS_U(0)
#endif

#ifndef OS_NHWTIMERS_CORE3
#define OS_NHWTIMERS_CORE3 OS_U(0)
#endif

#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
#define OS_QMOS_PANICCODE_PTR_CORE3 OS_NULL
#endif /* OS_KERNEL_TYPE == OS_MICROKERNEL */

#endif /* not USED */


/* Deduced configuration constants.
*/

#define OS_TOTALALARMS_CORE3 (OS_NALARMS_CORE3 + OS_NSCHEDULES_CORE3)

#if (OS_KERNEL_TYPE != OS_MICROKERNEL)

#define OS_MAXPRIO_CORE3 ((OS_NPRIORITIES_CORE3 == 0) ? 0 : (OS_NPRIORITIES_CORE3 - 1))

#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
#if (OS_NTASKS == 0)
#define OS_NSLAVEWORDS_CORE3	OS_U(0)
#else /* OS_NTASKS != 0 */
#define OS_NSLAVEWORDS_CORE3	(((OS_NPRIORITIES_CORE3 + OS_CLZWORD_NBITS) - 1) / OS_CLZWORD_NBITS)
#endif /* OS_NTASKS == 0 */
#endif /* OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE */

#if (OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST)
#if (OS_NTASKS == 0)
#define OS_TASKACT_CORE3		OS_NULL
#else /* OS_NTASKS != 0 */
#define OS_TASKACT_CORE3		OS_NULL,OS_TASKACTIVATIONS_CORE3
#endif /* OS_NTASKS == 0 */
#endif /* OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST */

#endif /* OS_KERNEL_TYPE != OS_MICROKERNEL */


/* Choose symbols for single-core optimization.
*/
#if ((OS_N_CORES == 1) && (3 == OS_INITCORE))
#define OS_KERNELDATA_OBJ_CORE3			OS_kernelData
#define OS_TASKPTRS_OBJ_CORE3			OS_taskPtrs
#define OS_TASKACTIVATIONS_OBJ_CORE3	OS_taskActivations
#define OS_PRIORITYQUEUE_OBJ_CORE3		OS_priorityQueue
#define OS_PRIORITYSLOT_OBJ_CORE3		OS_prioritySlot
#define OS_SLAVEPRIOWORD_OBJ_CORE3		OS_slavePrioWord
#else
#define OS_KERNELDATA_OBJ_CORE3			OS_kernelData_core3
#define OS_TASKPTRS_OBJ_CORE3			OS_taskPtrs_core3
#define OS_TASKACTIVATIONS_OBJ_CORE3	OS_taskActivations_core3
#define OS_PRIORITYQUEUE_OBJ_CORE3		OS_priorityQueue_core3
#define OS_PRIORITYSLOT_OBJ_CORE3		OS_prioritySlot_core3
#define OS_SLAVEPRIOWORD_OBJ_CORE3		OS_slavePrioWord_core3
#endif


/* Declare core local symbols.
*/
#ifndef OS_ASM
#if ((OS_PROPERTIES_CORE3 & OS_COREPROP_USED_BIT) != OS_COREPROP_USED_BIT)

extern os_uint8_t OS_unused_configuration_c3;
#define OS_SLAVEWORDPTR_CORE3			OS_NULL
#define OS_PRIOSLOTPTR_CORE3			OS_NULL
#define OS_PRIOTITYQUEUEPTR_CORE3		OS_NULL
#define OS_PTASKPTRS_CORE3				OS_NULL
#define OS_TASKACTIVATIONSPTR_CORE3		OS_NULL
#define OS_CPULOADBUSYBUFFERPTR_CORE3	OS_NULL
#define OS_MESSAGEQUEUEPTR_CORE3		OS_NULL
#define OS_KERNELDATAPTR_CORE3			OS_NULL

#else /* USED */

#if (OS_KERNEL_TYPE != OS_MICROKERNEL)

#if (OS_NAPPS_CORE3 != 0)
extern os_appdynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core3.appDynamic)
		OS_appDynamic_core3[OS_NAPPS_CORE3]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core3.appDynamic);
#endif
#if (OS_NTASKACCOUNTING_CORE3 != 0)
extern os_taskaccounting_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core3.taskAccounting)
		OS_taskAccounting_core3[OS_NTASKACCOUNTING_CORE3]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core3.taskAccounting);
#endif
#if (OS_NTASKS_CORE3 != 0)
extern os_taskdynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core3.taskDynamic)
		OS_taskDynamic_core3[OS_NTASKS_CORE3]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core3.taskDynamic);
#endif

#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
#if (OS_NSLAVEWORDS_CORE3 == 0)
#define OS_SLAVEWORDPTR_CORE3	OS_NULL
#else
#define OS_SLAVEWORDPTR_CORE3	(&OS_SLAVEPRIOWORD_OBJ_CORE3[0])
#if (OS_N_CORES != 1)
extern os_clzword_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core3.slavePrioWord)
		OS_slavePrioWord_core3[OS_NSLAVEWORDS_CORE3]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core3.slavePrioWord);
#endif
#endif
#if (OS_NPRIORITYSLOTS_CORE3 == 0)
#define OS_PRIOSLOTPTR_CORE3 OS_NULL
#else
#if (OS_N_CORES != 1)
extern os_priorityqueueentry_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core3.prioritySlot)
		OS_prioritySlot_core3[OS_NPRIORITYSLOTS_CORE3]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core3.prioritySlot);
#endif
#define OS_PRIOSLOTPTR_CORE3 (&OS_PRIORITYSLOT_OBJ_CORE3[0])
#endif
#if (OS_NPRIORITIES_CORE3 == 0)
#define OS_PRIOTITYQUEUEPTR_CORE3 OS_NULL
#else
#if (OS_N_CORES != 1)
extern os_priorityqueue_t const OS_priorityQueue_core3[OS_NPRIORITIES_CORE3];
#endif
#define OS_PRIOTITYQUEUEPTR_CORE3 (&OS_PRIORITYQUEUE_OBJ_CORE3[0])
#endif
#endif /* OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE */

#if (OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST)
#if (OS_NTASKACTIVATIONS_CORE3 == 0)
#define OS_PTASKPTRS_CORE3 OS_NULL
#else
#if (OS_N_CORES != 1)
extern const os_task_t * const OS_taskPtrs_core3[OS_NTASKACTIVATIONS_CORE3 + 1];
#endif
#define OS_PTASKPTRS_CORE3 (&OS_TASKPTRS_OBJ_CORE3[0])
#endif
#if (OS_NTASKACTIVATIONS_CORE3 == 0)
#define OS_TASKACTIVATIONSPTR_CORE3 OS_NULL
#else
#if (OS_N_CORES != 1)
extern os_tasklink_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core3.taskActivations)
		OS_taskActivations_core3[OS_NTASKACTIVATIONS_CORE3 + 1]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core3.taskActivations);
#endif
#define OS_TASKACTIVATIONSPTR_CORE3 (&OS_TASKACTIVATIONS_OBJ_CORE3[0])
#endif
#endif /* OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST */

#if (OS_NRESOURCES_CORE3 != 0)
extern os_resourcedynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core3.resourceDynamic)
		OS_resourceDynamic_core3[OS_NRESOURCES_CORE3]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core3.resourceDynamic);
#endif
#if (OS_NISRACCOUNTING != 0)
#if (OS_NISRACCOUNTING_CORE3 != 0)
extern os_israccounting_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core3.isrAccounting)
		OS_isrAccounting_core3[OS_NISRACCOUNTING_CORE3]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core3.isrAccounting);
#endif
#endif /* OS_NISRACCOUNTING!=0 */
#if (OS_NINTERRUPTS_CORE3 != 0)
extern os_isrdynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core3.isrDynamic)
		OS_isrDynamic_core3[OS_NINTERRUPTS_CORE3]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core3.isrDynamic);
#endif

#if (OS_NRATEMONS != 0)
#if (OS_NRATEMONS_CORE3 != 0)
extern os_rateindex_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core3.rateIndex)
		OS_rateIndex_core3[OS_NRATEMONS_CORE3]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core3.rateIndex);
#endif
#if (OS_NRATETIMERS_CORE3 != 0)
extern os_timestamp_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core3.rateTimer)
		OS_rateTimer_core3[OS_NRATETIMERS_CORE3]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core3.rateTimer);
#endif
#endif /* OS_NRATEMONS != 0 */

#ifndef OS_EXCLUDE_CPULOAD
extern os_tick_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core3.cpuLoadBusyBuffer)
		OS_cpuLoadBusyBuffer_core3[OS_CPULOADCFG_NINTERVALS]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core3.cpuLoadBusyBuffer);
#define OS_CPULOADBUSYBUFFERPTR_CORE3 (&OS_cpuLoadBusyBuffer_core3[0])
#endif

#if (OS_N_CORES > 1)
extern os_messagequeue_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.shared.core3.messageQueue)
		OS_messageQueue_core3
		OS_SECTION_ATTRIB_POST_DECL(.bss.shared.core3.messageQueue);
#define OS_MESSAGEQUEUEPTR_CORE3 (&OS_messageQueue_core3)
#endif

#if (OS_N_CORES != 1)
extern os_kerneldata_t
		OS_SECTION_ATTRIB_PRE_DECL(.data.core3.kernelData)
		OS_kernelData_core3
		OS_SECTION_ATTRIB_POST_DECL(.data.core3.kernelData);
#endif
#define OS_KERNELDATAPTR_CORE3 (&OS_KERNELDATA_OBJ_CORE3)

#endif /* OS_KERNEL_TYPE != OS_MICROKERNEL */

#if (OS_NCOUNTERS_CORE3 != 0)
extern os_counterdynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core3.counterDynamic)
		OS_counterDynamic_core3[OS_NCOUNTERS_CORE3]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core3.counterDynamic);
#endif
#if (OS_TOTALALARMS_CORE3 != 0)
extern os_alarmdynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core3.alarmDynamic)
		OS_alarmDynamic_core3[OS_TOTALALARMS_CORE3]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core3.alarmDynamic);
#endif
#if (OS_NSCHEDULES_CORE3 != 0)
extern os_scheduledynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core3.scheduleDynamic)
		OS_scheduleDynamic_core3[OS_NSCHEDULES_CORE3]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core3.scheduleDynamic);
#endif
#if (OS_NHWTIMERS_CORE3 != 0)
extern os_timervalue_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core3.hwtLastValue)
		OS_hwtLastValue_core3[OS_NHWTIMERS_CORE3]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core3.hwtLastValue);
#endif

#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
extern volatile os_panic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core3.panicCode)
		OS_panicCode_core3
		OS_SECTION_ATTRIB_POST_DECL(.bss.core3.panicCode);
#define OS_QMOS_PANICCODE_PTR_CORE3 (&OS_panicCode_core3)
#endif /* OS_KERNEL_TYPE == OS_MICROKERNEL */

#endif /* USED */
#endif /* ASM */

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
