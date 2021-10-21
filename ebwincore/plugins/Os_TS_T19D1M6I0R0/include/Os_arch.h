/* Os_arch.h
 *
 * This file includes the appropriate architecture include file depending
 * on the chosen architecture.
 *
 * See also: Os_defs.h
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_arch.h 30593 2019-07-30 14:10:36Z beoe851 $
*/
#ifndef OS_ARCH_H
#define OS_ARCH_H

#include <Os_defs.h>

#if (OS_ARCH==OS_PA)
#include <PA/Os_kernel_PA.h>
#elif (OS_ARCH==OS_TRICORE)
#include <TRICORE/Os_kernel_TRICORE.h>
#elif (OS_ARCH==OS_RH850)
#include <RH850/Os_kernel_RH850.h>
#elif (OS_ARCH==OS_NEWARCH)
#include <NEWARCH/Os_kernel_NEWARCH.h>
#elif (OS_ARCH==OS_PIKEOS)
#include <PIKEOS/Os_kernel_PIKEOS.h>
#elif (OS_ARCH==OS_WINDOWS)
#include <WINDOWS/Os_kernel_WINDOWS.h>
#elif (OS_ARCH==OS_ARM)
#include <ARM/Os_kernel_ARM.h>
#elif (OS_ARCH==OS_ARM64)
#include <ARM64/Os_kernel_ARM64.h>
#elif (OS_ARCH==OS_LINUX)
#include <LINUX/Os_kernel_LINUX.h>
#elif (OS_ARCH==OS_CORTEXM)
#include <CORTEXM/Os_kernel_CORTEXM.h>
#elif (OS_ARCH==OS_ARC)
#include <ARC/Os_kernel_ARC.h>
#else
#error "Unsupported OS_ARCH defined. Check your Makefiles!"
#endif

#if (OS_KERNEL_TYPE == OS_MICROKERNEL)
#include <Os_microkernel.h>
#endif

#include <Os_tool.h>

/* Some default values for macros that might not be defined for all architectures
*/
#ifndef OS_HAVE_ARCHTASK_T
#define OS_HAVE_ARCHTASK_T 0
#endif

#ifndef OS_ARCHTASK_INIT
#define OS_ARCHTASK_INIT(x)
#endif

#ifndef OS_HAVE_ARCHAPP_T
#define OS_HAVE_ARCHAPP_T 0
#endif

#ifndef OS_ARCHAPP_INIT
#define OS_ARCHAPP_INIT(x)
#endif

#ifndef OS_HAVE_ARCHISRDYN_T
#define OS_HAVE_ARCHISRDYN_T	0
#endif

#ifndef OS_IntRestoreAll
#define OS_IntRestoreAll(is)	OS_IntRestore(is)
#endif

#ifndef OS_IntRestoreMax
#define OS_IntRestoreMax(is)	OS_IntRestore(is)
#endif

#ifndef OS_HAVE_ARCHHWT_T
#define OS_HAVE_ARCHHWT_T 0
#endif

/* OS_USE_IRQ_ATOMIC_INKERNEL: Interrupt vector functions set the inKernel
 * flag before enabling interrupts again. This is needed, in case the
 * the interrupt masking mechanism of an interrupt controller may allow an
 * interrupt that occured between the IRQ entry and setting the lock
 * level to occur right after enabling interrupts.
 */
#ifndef OS_USE_IRQ_ATOMIC_INKERNEL
#define OS_USE_IRQ_ATOMIC_INKERNEL 0
#endif

/* OS_CPU_CAN_ATOMIC_READ_32: Set to 1, if the CPU reads a correctly aligned
 * 32-bit value in a consistent (typically atomic) way by hardware without
 * special precautions (or need for special assembly instructions).
 *
 * Note: This is one for all 32-bit CPUs and should be 0 for all 16-bit CPUs.
 */
#ifndef OS_CPU_CAN_ATOMIC_READ_32
#define OS_CPU_CAN_ATOMIC_READ_32	1
#endif

/*!
 * OS_GetTrustedCallerSp
 *
 * This macro returns the caller's stack pointer to be used in the stack check
 * when calling a trusted function. The provided stack pointer is expected to
 * lie between the bounds returned by the OS_GetTrustedFunctionStackBounds
 * macro.
 *
 * This is the common implementation for architectures where trusted functions
 * are always executed on the kernel stack. Since this macro is invoked in
 * kernel context just before calling the trusted function, the current stack
 * pointer is returned.
 *
 * Param spp: type os_address_t*, to store the caller's stack pointer
 */
#ifndef OS_GetTrustedCallerSp
#define OS_GetTrustedCallerSp(spp) \
	do { \
		*(spp) = (os_address_t) OS_GetCurrentSp(); \
	} while (0)
#endif /* OS_GetTrustedCallerSp */

/*!
 * OS_GetTrustedFunctionStackBounds
 *
 * This macro determines the lower and upper bounds for the stack of
 * a trusted function to be executed.
 *
 * There is two standard implementations depending on whether the trusted
 * functions are executed on the stack of the calling task/ISR or on the kernel
 * stack. The former is commonly the case for function call kernels, the latter
 * if the system call interface is used, but some architectures such as TRICORE
 * (always on task/ISR stack) differ.
 *
 * The standard implementation for OS_GetTFStackBounds_Syscall for syscall
 * kernels returns the bounds of the kernel stack.
 * The standard implementation OS_GetTFStackBounds_Fctcall for function call
 * kernels returns the bounds of the stack of the calling task/ISR.
 *
 * If the architecture does not define a custom mapping, a default mapping is
 * established to the standard implementations depending on whether the kernel
 * is a function call or syscall kernel below. Architectures for which this is
 * not appropriate may define OS_GetTrustedFunctionStackBounds to a custom
 * implementation.
 *
 * Param lbp: type os_address_t*, to store the lower bound
 * Param ubp: type os_address_t*, to store the upper bound
*/

#ifndef OS_GetTrustedFunctionStackBounds
#if (OS_KERNEL_TYPE==OS_FUNCTION_CALL)
#define OS_GetTrustedFunctionStackBounds(lbp,ubp) OS_GetTFStackBounds_Fctcall(lbp,ubp)
#define OS_NEEDS_GetTFStackBounds_Fctcall 1
#else
#define OS_GetTrustedFunctionStackBounds(lbp,ubp) OS_GetTFStackBounds_Syscall(lbp,ubp)
#endif
#endif

#ifndef OS_NEEDS_GetTFStackBounds_Fctcall
#define OS_NEEDS_GetTFStackBounds_Fctcall 0
#endif

#define OS_GetTFStackBounds_Syscall(lbp,ubp) \
	do {															\
		os_kerneldata_t * const kernel_data = OS_GetKernelData();	\
		*(lbp) = (os_address_t) kernel_data->iStackBase;			\
		*(ubp) = *(lbp) + kernel_data->iStackLen;					\
	} while (0)

/*!
 * OS_IsTrustedFunctionStackValid
 *
 * This macros checks, if the stack bounded by lowerBound and upperBound is large enough,
 * so that the requested stack to execute a trusted function fits into it.
 * The trusted function's stack is bounded by lowerSp and upperSp.
 */
#ifndef OS_IsTFuncStackSizeInsufficient
#define OS_IsTFuncStackSizeInsufficient(lowerSp, upperSp, lowerBound, upperBound)	\
	(((lowerSp) < (lowerBound)) || ((upperSp) > (upperBound)) || ((lowerSp) > (upperSp)))
#endif

/* OS_ATOMIC_ASSIGN_32(dest, src): Assign the 32-bit value src to dest so that
 * the value is consistent.
 *
 * Param dest: destination. Must be 32-bit (integer type) and aligned correctly.
 * Param src: source. Must be 32-bit (integer type) and aligned correctly.
*/
#if (OS_CPU_CAN_ATOMIC_READ_32 == 0)

/* On 16-bit CPUs, the assignment typically requires at least two
 * instructions for each 16-bit portion. If we get interrupted in
 * between these, the result is inconsistent.
 * Therefore, we check if the result is identical after the assignment,
 * which means that we didn't get interrupted in between (or at least
 * that the value wasn't updated in between).
 */
#define OS_ATOMIC_ASSIGN_32(dest, src)								\
	do																\
	{																\
		(dest) = (volatile os_uint32_t)(src);						\
	} while ((os_uint32_t)(dest) != (volatile os_uint32_t)(src));

#elif (OS_CPU_CAN_ATOMIC_READ_32 == 1)

/* On 32-bit CPUs (or larger), a 32-bit value can be loaded atomically,
 * at least if it is aligned correctly.
 */
#define OS_ATOMIC_ASSIGN_32(dest, src)						\
	do														\
	{														\
		(dest) = (src); /* always consistent on 32-bit */	\
	} while (0)

#else
#error "Incorrect value of OS_CPU_CAN_ATOMIC_READ_32."
#endif

/*!
 * OS_ArchPreDynamicStop
 *
 * Architecture-dependent actions, before the dynamic waiting loop is started.
 *
 * In OS_ShutdownNoHooks() there is a waiting loop at its end, which never ends,
 * unless the variable OS_dynamicStop is set to zero again.
 *
 * This function is called right before this loop starts.
 */
#ifndef OS_ArchPreDynamicStop
#define OS_ArchPreDynamicStop()
#endif

#if (OS_KERNEL_TYPE != OS_MICROKERNEL)
#define OS_ForceIntDisable()	OS_IntDisable()
#define OS_ForceIntRestore(p)	OS_IntRestore(p)
#endif

#ifndef OS_ARCH_DEVICE_ENDIANESS_MISMATCH
#define OS_ARCH_DEVICE_ENDIANESS_MISMATCH 0
#endif

/* Memory-mapped device registers have a different endianess that the CPU.
 * Provide macros for accessing these memory-mapped registers and do byte swapping if necessary.
 */
#if (OS_ARCH_DEVICE_ENDIANESS_MISMATCH == 1)

#define OS_READ_REGISTER_32(src_reg, dest)				\
	do {												\
		os_uint32_t src_val = (os_uint32_t) (src_reg);	\
		OS_SWITCH_ENDIANESS_32(src_val, (dest));		\
	} while(0)

#define OS_WRITE_REGISTER_32(dest_reg, value)				\
	do {													\
		os_uint32_t switched_val = (os_uint32_t) (value);	\
		OS_SWITCH_ENDIANESS_32(switched_val, switched_val);	\
		(dest_reg) = (os_reg32_t) switched_val;				\
	} while (0)

#else

#define OS_READ_REGISTER_32(src_reg, dest)		\
	do {										\
		(dest) = ((os_uint32_t) (src_reg));		\
	} while(0)

#define OS_WRITE_REGISTER_32(dest_reg, value)	\
	do {										\
		(dest_reg) = (os_reg32_t)(value);		\
	} while (0);

#endif

#endif	/* !OS_ARCH_H */


/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
