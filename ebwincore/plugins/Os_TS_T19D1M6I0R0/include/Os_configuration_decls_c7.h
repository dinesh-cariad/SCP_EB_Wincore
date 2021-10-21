/* Os_configuration_c7_defaults.h
 *
 * This file provides configuration dependent declarations
 * and macro definitions for core C7.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_configuration_decls_c7.h 30751 2019-08-30 10:26:47Z ache271795 $
*/

#ifndef OS_CONFIGURATION_DECLS_C7_H
#define OS_CONFIGURATION_DECLS_C7_H

#include <Os_configuration_decls.h>

#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
#include <Os_panic.h>
#endif /* OS_KERNEL_TYPE == OS_MICROKERNEL */

/* If no core properties are defined, we assume, that this core isn't used.
*/
#ifndef OS_PROPERTIES_CORE7
#define OS_PROPERTIES_CORE7 OS_U(0)
#endif


/* Define default values in case this core is not active.
*/
#if ((OS_PROPERTIES_CORE7 & OS_COREPROP_USED_BIT) != OS_COREPROP_USED_BIT)

#if (OS_KERNEL_TYPE != OS_MICROKERNEL)

#ifndef OS_NAPPS_CORE7
#define OS_NAPPS_CORE7 OS_U(0)
#endif

#ifndef OS_NTASKACCOUNTING_CORE7
#define OS_NTASKACCOUNTING_CORE7 OS_U(0)
#endif

#ifndef OS_NTASKS_CORE7
#define OS_NTASKS_CORE7 OS_U(0)
#endif

#ifndef OS_NPRIORITIES_CORE7
#define OS_NPRIORITIES_CORE7 OS_U(0)
#endif

#ifndef OS_NPRIORITYQUEUEDYNAMICS_CORE7
#define OS_NPRIORITYQUEUEDYNAMICS_CORE7 OS_U(0)
#endif

#ifndef OS_NPRIORITYSLOTS_CORE7
#define OS_NPRIORITYSLOTS_CORE7 OS_U(0)
#endif

#ifndef OS_NTASKACTIVATIONS_CORE7
#define OS_NTASKACTIVATIONS_CORE7 OS_U(0)
#endif

#ifndef OS_NRESOURCES_CORE7
#define OS_NRESOURCES_CORE7 OS_U(0)
#endif

#ifndef OS_NISRACCOUNTING_CORE7
#define OS_NISRACCOUNTING_CORE7 OS_U(0)
#endif

#ifndef OS_NINTERRUPTS_CORE7
#define OS_NINTERRUPTS_CORE7 OS_U(0)
#endif

#ifndef OS_NRATEMONS_CORE7
#define OS_NRATEMONS_CORE7 OS_U(0)
#endif

#ifndef OS_NRATETIMERS_CORE7
#define OS_NRATETIMERS_CORE7 OS_U(0)
#endif

#endif /* OS_KERNEL_TYPE != OS_MICROKERNEL */

#ifndef OS_NCOUNTERS_CORE7
#define OS_NCOUNTERS_CORE7 OS_U(0)
#endif

#ifndef OS_NALARMS_CORE7
#define OS_NALARMS_CORE7 OS_U(0)
#endif

#ifndef OS_NSCHEDULES_CORE7
#define OS_NSCHEDULES_CORE7 OS_U(0)
#endif

#ifndef OS_NHWTIMERS_CORE7
#define OS_NHWTIMERS_CORE7 OS_U(0)
#endif

#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
#define OS_QMOS_PANICCODE_PTR_CORE7 OS_NULL
#endif /* OS_KERNEL_TYPE == OS_MICROKERNEL */

#endif /* not USED */


/* Deduced configuration constants.
*/

#define OS_TOTALALARMS_CORE7 (OS_NALARMS_CORE7 + OS_NSCHEDULES_CORE7)

#if (OS_KERNEL_TYPE != OS_MICROKERNEL)

#define OS_MAXPRIO_CORE7 ((OS_NPRIORITIES_CORE7 == 0) ? 0 : (OS_NPRIORITIES_CORE7 - 1))

#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
#if (OS_NTASKS == 0)
#define OS_NSLAVEWORDS_CORE7	OS_U(0)
#else /* OS_NTASKS != 0 */
#define OS_NSLAVEWORDS_CORE7	(((OS_NPRIORITIES_CORE7 + OS_CLZWORD_NBITS) - 1) / OS_CLZWORD_NBITS)
#endif /* OS_NTASKS == 0 */
#endif /* OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE */

#if (OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST)
#if (OS_NTASKS == 0)
#define OS_TASKACT_CORE7		OS_NULL
#else /* OS_NTASKS != 0 */
#define OS_TASKACT_CORE7		OS_NULL,OS_TASKACTIVATIONS_CORE7
#endif /* OS_NTASKS == 0 */
#endif /* OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST */

#endif /* OS_KERNEL_TYPE != OS_MICROKERNEL */


/* Choose symbols for single-core optimization.
*/
#if ((OS_N_CORES == 1) && (7 == OS_INITCORE))
#define OS_KERNELDATA_OBJ_CORE7			OS_kernelData
#define OS_TASKPTRS_OBJ_CORE7			OS_taskPtrs
#define OS_TASKACTIVATIONS_OBJ_CORE7	OS_taskActivations
#define OS_PRIORITYQUEUE_OBJ_CORE7		OS_priorityQueue
#define OS_PRIORITYSLOT_OBJ_CORE7		OS_prioritySlot
#define OS_SLAVEPRIOWORD_OBJ_CORE7		OS_slavePrioWord
#else
#define OS_KERNELDATA_OBJ_CORE7			OS_kernelData_core7
#define OS_TASKPTRS_OBJ_CORE7			OS_taskPtrs_core7
#define OS_TASKACTIVATIONS_OBJ_CORE7	OS_taskActivations_core7
#define OS_PRIORITYQUEUE_OBJ_CORE7		OS_priorityQueue_core7
#define OS_PRIORITYSLOT_OBJ_CORE7		OS_prioritySlot_core7
#define OS_SLAVEPRIOWORD_OBJ_CORE7		OS_slavePrioWord_core7
#endif


/* Declare core local symbols.
*/
#ifndef OS_ASM
#if ((OS_PROPERTIES_CORE7 & OS_COREPROP_USED_BIT) != OS_COREPROP_USED_BIT)

extern os_uint8_t OS_unused_configuration_c7;
#define OS_SLAVEWORDPTR_CORE7			OS_NULL
#define OS_PRIOSLOTPTR_CORE7			OS_NULL
#define OS_PRIOTITYQUEUEPTR_CORE7		OS_NULL
#define OS_PTASKPTRS_CORE7				OS_NULL
#define OS_TASKACTIVATIONSPTR_CORE7		OS_NULL
#define OS_CPULOADBUSYBUFFERPTR_CORE7	OS_NULL
#define OS_MESSAGEQUEUEPTR_CORE7		OS_NULL
#define OS_KERNELDATAPTR_CORE7			OS_NULL

#else /* USED */

#if (OS_KERNEL_TYPE != OS_MICROKERNEL)

#if (OS_NAPPS_CORE7 != 0)
extern os_appdynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core7.appDynamic)
		OS_appDynamic_core7[OS_NAPPS_CORE7]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core7.appDynamic);
#endif
#if (OS_NTASKACCOUNTING_CORE7 != 0)
extern os_taskaccounting_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core7.taskAccounting)
		OS_taskAccounting_core7[OS_NTASKACCOUNTING_CORE7]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core7.taskAccounting);
#endif
#if (OS_NTASKS_CORE7 != 0)
extern os_taskdynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core7.taskDynamic)
		OS_taskDynamic_core7[OS_NTASKS_CORE7]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core7.taskDynamic);
#endif

#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
#if (OS_NSLAVEWORDS_CORE7 == 0)
#define OS_SLAVEWORDPTR_CORE7	OS_NULL
#else
#define OS_SLAVEWORDPTR_CORE7	(&OS_SLAVEPRIOWORD_OBJ_CORE7[0])
#if (OS_N_CORES != 1)
extern os_clzword_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core7.slavePrioWord)
		OS_slavePrioWord_core7[OS_NSLAVEWORDS_CORE7]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core7.slavePrioWord);
#endif
#endif
#if (OS_NPRIORITYSLOTS_CORE7 == 0)
#define OS_PRIOSLOTPTR_CORE7 OS_NULL
#else
#if (OS_N_CORES != 1)
extern os_priorityqueueentry_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core7.prioritySlot)
		OS_prioritySlot_core7[OS_NPRIORITYSLOTS_CORE7]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core7.prioritySlot);
#endif
#define OS_PRIOSLOTPTR_CORE7 (&OS_PRIORITYSLOT_OBJ_CORE7[0])
#endif
#if (OS_NPRIORITIES_CORE7 == 0)
#define OS_PRIOTITYQUEUEPTR_CORE7 OS_NULL
#else
#if (OS_N_CORES != 1)
extern os_priorityqueue_t const OS_priorityQueue_core7[OS_NPRIORITIES_CORE7];
#endif
#define OS_PRIOTITYQUEUEPTR_CORE7 (&OS_PRIORITYQUEUE_OBJ_CORE7[0])
#endif
#endif /* OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE */

#if (OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST)
#if (OS_NTASKACTIVATIONS_CORE7 == 0)
#define OS_PTASKPTRS_CORE7 OS_NULL
#else
#if (OS_N_CORES != 1)
extern const os_task_t * const OS_taskPtrs_core7[OS_NTASKACTIVATIONS_CORE7 + 1];
#endif
#define OS_PTASKPTRS_CORE7 (&OS_TASKPTRS_OBJ_CORE7[0])
#endif
#if (OS_NTASKACTIVATIONS_CORE7 == 0)
#define OS_TASKACTIVATIONSPTR_CORE7 OS_NULL
#else
#if (OS_N_CORES != 1)
extern os_tasklink_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core7.taskActivations)
		OS_taskActivations_core7[OS_NTASKACTIVATIONS_CORE7 + 1]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core7.taskActivations);
#endif
#define OS_TASKACTIVATIONSPTR_CORE7 (&OS_TASKACTIVATIONS_OBJ_CORE7[0])
#endif
#endif /* OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST */

#if (OS_NRESOURCES_CORE7 != 0)
extern os_resourcedynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core7.resourceDynamic)
		OS_resourceDynamic_core7[OS_NRESOURCES_CORE7]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core7.resourceDynamic);
#endif
#if (OS_NISRACCOUNTING != 0)
#if (OS_NISRACCOUNTING_CORE7 != 0)
extern os_israccounting_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core7.isrAccounting)
		OS_isrAccounting_core7[OS_NISRACCOUNTING_CORE7]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core7.isrAccounting);
#endif
#endif /* OS_NISRACCOUNTING!=0 */
#if (OS_NINTERRUPTS_CORE7 != 0)
extern os_isrdynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core7.isrDynamic)
		OS_isrDynamic_core7[OS_NINTERRUPTS_CORE7]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core7.isrDynamic);
#endif

#if (OS_NRATEMONS != 0)
#if (OS_NRATEMONS_CORE7 != 0)
extern os_rateindex_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core7.rateIndex)
		OS_rateIndex_core7[OS_NRATEMONS_CORE7]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core7.rateIndex);
#endif
#if (OS_NRATETIMERS_CORE7 != 0)
extern os_timestamp_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core7.rateTimer)
		OS_rateTimer_core7[OS_NRATETIMERS_CORE7]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core7.rateTimer);
#endif
#endif /* OS_NRATEMONS != 0 */

#ifndef OS_EXCLUDE_CPULOAD
extern os_tick_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core7.cpuLoadBusyBuffer)
		OS_cpuLoadBusyBuffer_core7[OS_CPULOADCFG_NINTERVALS]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core7.cpuLoadBusyBuffer);
#define OS_CPULOADBUSYBUFFERPTR_CORE7 (&OS_cpuLoadBusyBuffer_core7[0])
#endif

#if (OS_N_CORES > 1)
extern os_messagequeue_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.shared.core7.messageQueue)
		OS_messageQueue_core7
		OS_SECTION_ATTRIB_POST_DECL(.bss.shared.core7.messageQueue);
#define OS_MESSAGEQUEUEPTR_CORE7 (&OS_messageQueue_core7)
#endif

#if (OS_N_CORES != 1)
extern os_kerneldata_t
		OS_SECTION_ATTRIB_PRE_DECL(.data.core7.kernelData)
		OS_kernelData_core7
		OS_SECTION_ATTRIB_POST_DECL(.data.core7.kernelData);
#endif
#define OS_KERNELDATAPTR_CORE7 (&OS_KERNELDATA_OBJ_CORE7)

#endif /* OS_KERNEL_TYPE != OS_MICROKERNEL */

#if (OS_NCOUNTERS_CORE7 != 0)
extern os_counterdynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core7.counterDynamic)
		OS_counterDynamic_core7[OS_NCOUNTERS_CORE7]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core7.counterDynamic);
#endif
#if (OS_TOTALALARMS_CORE7 != 0)
extern os_alarmdynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core7.alarmDynamic)
		OS_alarmDynamic_core7[OS_TOTALALARMS_CORE7]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core7.alarmDynamic);
#endif
#if (OS_NSCHEDULES_CORE7 != 0)
extern os_scheduledynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core7.scheduleDynamic)
		OS_scheduleDynamic_core7[OS_NSCHEDULES_CORE7]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core7.scheduleDynamic);
#endif
#if (OS_NHWTIMERS_CORE7 != 0)
extern os_timervalue_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core7.hwtLastValue)
		OS_hwtLastValue_core7[OS_NHWTIMERS_CORE7]
		OS_SECTION_ATTRIB_POST_DECL(.bss.core7.hwtLastValue);
#endif

#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
extern volatile os_panic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.core7.panicCode)
		OS_panicCode_core7
		OS_SECTION_ATTRIB_POST_DECL(.bss.core7.panicCode);
#define OS_QMOS_PANICCODE_PTR_CORE7 (&OS_panicCode_core7)
#endif /* OS_KERNEL_TYPE == OS_MICROKERNEL */

#endif /* USED */
#endif /* ASM */

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
