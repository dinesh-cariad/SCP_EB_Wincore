/* Os_configuration_cx_defaults.h
 *
 * REMOVE-THIS-LINE This is a template file. Use the script core_gen.sh to create core-specific files.
 * This file provides configuration dependent declarations
 * and macro definitions for core CX.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_configuration_decls_cx.h 30751 2019-08-30 10:26:47Z ache271795 $
*/

#ifndef OS_CONFIGURATION_DECLS_CX_H
#define OS_CONFIGURATION_DECLS_CX_H

#include <Os_configuration_decls.h>

#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
#include <Os_panic.h>
#endif /* OS_KERNEL_TYPE == OS_MICROKERNEL */

/* If no core properties are defined, we assume, that this core isn't used.
*/
#ifndef OS_PROPERTIES_COREX
#define OS_PROPERTIES_COREX OS_U(0)
#endif


/* Define default values in case this core is not active.
*/
#if ((OS_PROPERTIES_COREX & OS_COREPROP_USED_BIT) != OS_COREPROP_USED_BIT)

#if (OS_KERNEL_TYPE != OS_MICROKERNEL)

#ifndef OS_NAPPS_COREX
#define OS_NAPPS_COREX OS_U(0)
#endif

#ifndef OS_NTASKACCOUNTING_COREX
#define OS_NTASKACCOUNTING_COREX OS_U(0)
#endif

#ifndef OS_NTASKS_COREX
#define OS_NTASKS_COREX OS_U(0)
#endif

#ifndef OS_NPRIORITIES_COREX
#define OS_NPRIORITIES_COREX OS_U(0)
#endif

#ifndef OS_NPRIORITYQUEUEDYNAMICS_COREX
#define OS_NPRIORITYQUEUEDYNAMICS_COREX OS_U(0)
#endif

#ifndef OS_NPRIORITYSLOTS_COREX
#define OS_NPRIORITYSLOTS_COREX OS_U(0)
#endif

#ifndef OS_NTASKACTIVATIONS_COREX
#define OS_NTASKACTIVATIONS_COREX OS_U(0)
#endif

#ifndef OS_NRESOURCES_COREX
#define OS_NRESOURCES_COREX OS_U(0)
#endif

#ifndef OS_NISRACCOUNTING_COREX
#define OS_NISRACCOUNTING_COREX OS_U(0)
#endif

#ifndef OS_NINTERRUPTS_COREX
#define OS_NINTERRUPTS_COREX OS_U(0)
#endif

#ifndef OS_NRATEMONS_COREX
#define OS_NRATEMONS_COREX OS_U(0)
#endif

#ifndef OS_NRATETIMERS_COREX
#define OS_NRATETIMERS_COREX OS_U(0)
#endif

#endif /* OS_KERNEL_TYPE != OS_MICROKERNEL */

#ifndef OS_NCOUNTERS_COREX
#define OS_NCOUNTERS_COREX OS_U(0)
#endif

#ifndef OS_NALARMS_COREX
#define OS_NALARMS_COREX OS_U(0)
#endif

#ifndef OS_NSCHEDULES_COREX
#define OS_NSCHEDULES_COREX OS_U(0)
#endif

#ifndef OS_NHWTIMERS_COREX
#define OS_NHWTIMERS_COREX OS_U(0)
#endif

#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
#define OS_QMOS_PANICCODE_PTR_COREX OS_NULL
#endif /* OS_KERNEL_TYPE == OS_MICROKERNEL */

#endif /* not USED */


/* Deduced configuration constants.
*/

#define OS_TOTALALARMS_COREX (OS_NALARMS_COREX + OS_NSCHEDULES_COREX)

#if (OS_KERNEL_TYPE != OS_MICROKERNEL)

#define OS_MAXPRIO_COREX ((OS_NPRIORITIES_COREX == 0) ? 0 : (OS_NPRIORITIES_COREX - 1))

#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
#if (OS_NTASKS == 0)
#define OS_NSLAVEWORDS_COREX	OS_U(0)
#else /* OS_NTASKS != 0 */
#define OS_NSLAVEWORDS_COREX	(((OS_NPRIORITIES_COREX + OS_CLZWORD_NBITS) - 1) / OS_CLZWORD_NBITS)
#endif /* OS_NTASKS == 0 */
#endif /* OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE */

#if (OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST)
#if (OS_NTASKS == 0)
#define OS_TASKACT_COREX		OS_NULL
#else /* OS_NTASKS != 0 */
#define OS_TASKACT_COREX		OS_NULL,OS_TASKACTIVATIONS_COREX
#endif /* OS_NTASKS == 0 */
#endif /* OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST */

#endif /* OS_KERNEL_TYPE != OS_MICROKERNEL */


/* Choose symbols for single-core optimization.
*/
#if ((OS_N_CORES == 1) && (THIS_CORE_INDEX == OS_INITCORE))
#define OS_KERNELDATA_OBJ_COREX			OS_kernelData
#define OS_TASKPTRS_OBJ_COREX			OS_taskPtrs
#define OS_TASKACTIVATIONS_OBJ_COREX	OS_taskActivations
#define OS_PRIORITYQUEUE_OBJ_COREX		OS_priorityQueue
#define OS_PRIORITYSLOT_OBJ_COREX		OS_prioritySlot
#define OS_SLAVEPRIOWORD_OBJ_COREX		OS_slavePrioWord
#else
#define OS_KERNELDATA_OBJ_COREX			OS_kernelData_coreX
#define OS_TASKPTRS_OBJ_COREX			OS_taskPtrs_coreX
#define OS_TASKACTIVATIONS_OBJ_COREX	OS_taskActivations_coreX
#define OS_PRIORITYQUEUE_OBJ_COREX		OS_priorityQueue_coreX
#define OS_PRIORITYSLOT_OBJ_COREX		OS_prioritySlot_coreX
#define OS_SLAVEPRIOWORD_OBJ_COREX		OS_slavePrioWord_coreX
#endif


/* Declare core local symbols.
*/
#ifndef OS_ASM
#if ((OS_PROPERTIES_COREX & OS_COREPROP_USED_BIT) != OS_COREPROP_USED_BIT)

extern os_uint8_t OS_unused_configuration_cx;
#define OS_SLAVEWORDPTR_COREX			OS_NULL
#define OS_PRIOSLOTPTR_COREX			OS_NULL
#define OS_PRIOTITYQUEUEPTR_COREX		OS_NULL
#define OS_PTASKPTRS_COREX				OS_NULL
#define OS_TASKACTIVATIONSPTR_COREX		OS_NULL
#define OS_CPULOADBUSYBUFFERPTR_COREX	OS_NULL
#define OS_MESSAGEQUEUEPTR_COREX		OS_NULL
#define OS_KERNELDATAPTR_COREX			OS_NULL

#else /* USED */

#if (OS_KERNEL_TYPE != OS_MICROKERNEL)

#if (OS_NAPPS_COREX != 0)
extern os_appdynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.coreX.appDynamic)
		OS_appDynamic_coreX[OS_NAPPS_COREX]
		OS_SECTION_ATTRIB_POST_DECL(.bss.coreX.appDynamic);
#endif
#if (OS_NTASKACCOUNTING_COREX != 0)
extern os_taskaccounting_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.coreX.taskAccounting)
		OS_taskAccounting_coreX[OS_NTASKACCOUNTING_COREX]
		OS_SECTION_ATTRIB_POST_DECL(.bss.coreX.taskAccounting);
#endif
#if (OS_NTASKS_COREX != 0)
extern os_taskdynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.coreX.taskDynamic)
		OS_taskDynamic_coreX[OS_NTASKS_COREX]
		OS_SECTION_ATTRIB_POST_DECL(.bss.coreX.taskDynamic);
#endif

#if (OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE)
#if (OS_NSLAVEWORDS_COREX == 0)
#define OS_SLAVEWORDPTR_COREX	OS_NULL
#else
#define OS_SLAVEWORDPTR_COREX	(&OS_SLAVEPRIOWORD_OBJ_COREX[0])
#if (OS_N_CORES != 1)
extern os_clzword_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.coreX.slavePrioWord)
		OS_slavePrioWord_coreX[OS_NSLAVEWORDS_COREX]
		OS_SECTION_ATTRIB_POST_DECL(.bss.coreX.slavePrioWord);
#endif
#endif
#if (OS_NPRIORITYSLOTS_COREX == 0)
#define OS_PRIOSLOTPTR_COREX OS_NULL
#else
#if (OS_N_CORES != 1)
extern os_priorityqueueentry_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.coreX.prioritySlot)
		OS_prioritySlot_coreX[OS_NPRIORITYSLOTS_COREX]
		OS_SECTION_ATTRIB_POST_DECL(.bss.coreX.prioritySlot);
#endif
#define OS_PRIOSLOTPTR_COREX (&OS_PRIORITYSLOT_OBJ_COREX[0])
#endif
#if (OS_NPRIORITIES_COREX == 0)
#define OS_PRIOTITYQUEUEPTR_COREX OS_NULL
#else
#if (OS_N_CORES != 1)
extern os_priorityqueue_t const OS_priorityQueue_coreX[OS_NPRIORITIES_COREX];
#endif
#define OS_PRIOTITYQUEUEPTR_COREX (&OS_PRIORITYQUEUE_OBJ_COREX[0])
#endif
#endif /* OS_SCHEDULING_ALGORITHM == OS_CLZ_QUEUE */

#if (OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST)
#if (OS_NTASKACTIVATIONS_COREX == 0)
#define OS_PTASKPTRS_COREX OS_NULL
#else
#if (OS_N_CORES != 1)
extern const os_task_t * const OS_taskPtrs_coreX[OS_NTASKACTIVATIONS_COREX + 1];
#endif
#define OS_PTASKPTRS_COREX (&OS_TASKPTRS_OBJ_COREX[0])
#endif
#if (OS_NTASKACTIVATIONS_COREX == 0)
#define OS_TASKACTIVATIONSPTR_COREX OS_NULL
#else
#if (OS_N_CORES != 1)
extern os_tasklink_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.coreX.taskActivations)
		OS_taskActivations_coreX[OS_NTASKACTIVATIONS_COREX + 1]
		OS_SECTION_ATTRIB_POST_DECL(.bss.coreX.taskActivations);
#endif
#define OS_TASKACTIVATIONSPTR_COREX (&OS_TASKACTIVATIONS_OBJ_COREX[0])
#endif
#endif /* OS_SCHEDULING_ALGORITHM == OS_LINKED_LIST */

#if (OS_NRESOURCES_COREX != 0)
extern os_resourcedynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.coreX.resourceDynamic)
		OS_resourceDynamic_coreX[OS_NRESOURCES_COREX]
		OS_SECTION_ATTRIB_POST_DECL(.bss.coreX.resourceDynamic);
#endif
#if (OS_NISRACCOUNTING != 0)
#if (OS_NISRACCOUNTING_COREX != 0)
extern os_israccounting_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.coreX.isrAccounting)
		OS_isrAccounting_coreX[OS_NISRACCOUNTING_COREX]
		OS_SECTION_ATTRIB_POST_DECL(.bss.coreX.isrAccounting);
#endif
#endif /* OS_NISRACCOUNTING!=0 */
#if (OS_NINTERRUPTS_COREX != 0)
extern os_isrdynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.coreX.isrDynamic)
		OS_isrDynamic_coreX[OS_NINTERRUPTS_COREX]
		OS_SECTION_ATTRIB_POST_DECL(.bss.coreX.isrDynamic);
#endif

#if (OS_NRATEMONS != 0)
#if (OS_NRATEMONS_COREX != 0)
extern os_rateindex_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.coreX.rateIndex)
		OS_rateIndex_coreX[OS_NRATEMONS_COREX]
		OS_SECTION_ATTRIB_POST_DECL(.bss.coreX.rateIndex);
#endif
#if (OS_NRATETIMERS_COREX != 0)
extern os_timestamp_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.coreX.rateTimer)
		OS_rateTimer_coreX[OS_NRATETIMERS_COREX]
		OS_SECTION_ATTRIB_POST_DECL(.bss.coreX.rateTimer);
#endif
#endif /* OS_NRATEMONS != 0 */

#ifndef OS_EXCLUDE_CPULOAD
extern os_tick_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.coreX.cpuLoadBusyBuffer)
		OS_cpuLoadBusyBuffer_coreX[OS_CPULOADCFG_NINTERVALS]
		OS_SECTION_ATTRIB_POST_DECL(.bss.coreX.cpuLoadBusyBuffer);
#define OS_CPULOADBUSYBUFFERPTR_COREX (&OS_cpuLoadBusyBuffer_coreX[0])
#endif

#if (OS_N_CORES > 1)
extern os_messagequeue_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.shared.coreX.messageQueue)
		OS_messageQueue_coreX
		OS_SECTION_ATTRIB_POST_DECL(.bss.shared.coreX.messageQueue);
#define OS_MESSAGEQUEUEPTR_COREX (&OS_messageQueue_coreX)
#endif

#if (OS_N_CORES != 1)
extern os_kerneldata_t
		OS_SECTION_ATTRIB_PRE_DECL(.data.coreX.kernelData)
		OS_kernelData_coreX
		OS_SECTION_ATTRIB_POST_DECL(.data.coreX.kernelData);
#endif
#define OS_KERNELDATAPTR_COREX (&OS_KERNELDATA_OBJ_COREX)

#endif /* OS_KERNEL_TYPE != OS_MICROKERNEL */

#if (OS_NCOUNTERS_COREX != 0)
extern os_counterdynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.coreX.counterDynamic)
		OS_counterDynamic_coreX[OS_NCOUNTERS_COREX]
		OS_SECTION_ATTRIB_POST_DECL(.bss.coreX.counterDynamic);
#endif
#if (OS_TOTALALARMS_COREX != 0)
extern os_alarmdynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.coreX.alarmDynamic)
		OS_alarmDynamic_coreX[OS_TOTALALARMS_COREX]
		OS_SECTION_ATTRIB_POST_DECL(.bss.coreX.alarmDynamic);
#endif
#if (OS_NSCHEDULES_COREX != 0)
extern os_scheduledynamic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.coreX.scheduleDynamic)
		OS_scheduleDynamic_coreX[OS_NSCHEDULES_COREX]
		OS_SECTION_ATTRIB_POST_DECL(.bss.coreX.scheduleDynamic);
#endif
#if (OS_NHWTIMERS_COREX != 0)
extern os_timervalue_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.coreX.hwtLastValue)
		OS_hwtLastValue_coreX[OS_NHWTIMERS_COREX]
		OS_SECTION_ATTRIB_POST_DECL(.bss.coreX.hwtLastValue);
#endif

#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
extern volatile os_panic_t
		OS_SECTION_ATTRIB_PRE_DECL(.bss.coreX.panicCode)
		OS_panicCode_coreX
		OS_SECTION_ATTRIB_POST_DECL(.bss.coreX.panicCode);
#define OS_QMOS_PANICCODE_PTR_COREX (&OS_panicCode_coreX)
#endif /* OS_KERNEL_TYPE == OS_MICROKERNEL */

#endif /* USED */
#endif /* ASM */

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
