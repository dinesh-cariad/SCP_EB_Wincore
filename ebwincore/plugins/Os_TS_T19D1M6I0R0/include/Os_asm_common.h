/* Os_asm_common.h
 *
 * This file performs the architecture-independent stuff that happens at the top
 * of every assembler file. This file is included by $(ARCH)/Os_asm_$(ARCH).h
 * and should not be included anywhere else. In particular, it must not be included
 * by high-level language files.
 *
 * "Stuff" that is done here:
 *
 *  - define OS_ASM, if not already defined
 *	- include Os_defs.h
 *  - include the assembler version of MemMap
 *
 * NOTE: Os_asm*.h doesn't follow the normal patter for architecture-selecting header files.
 * There is no generic Os_asm.h that includes the appropriate Os_asm_$(ARCH).h. This is because
 * assembler files are by definition architecture-specific, and therefore already know which
 * file to include. Thus the additional porting step of adding the architecture to the selection
 * in a hypothetical Os_asm.h is avoided. This file simply implements the part of the Os_asm_$(ARCH).h
 * files that is architecture-independent, thus avoiding duplicating it in every architecture.
 * Furthermore, it may happen occasionally that a toolchain's assembler cannot work with the
 * normal" mechanism, and must implement the contents of this file in a special way.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_asm_common.h 29543 2018-11-28 09:47:00Z olme8414 $
*/
#ifndef OS_ASM_COMMON_H
#define OS_ASM_COMMON_H

#ifndef	OS_ASM
#define OS_ASM	1
#endif

#include <Os_defs.h>

#if OS_KERNEL_TYPE == OS_MICROKERNEL
#ifndef MK_ASM
#define MK_ASM	1
#endif /* MK_ASM not defined */
#endif /* OS_KERNEL_TYPE == OS_MICROKERNEL */

/* Include the user's MemMapAsm.h file if needed
*/
#if defined(OS_MEMMAP)
#if OS_MEMMAP==1

#include <MemMapAsm.h>

#ifndef OS_ASM
#error "MemMapAsm.h has undefined OS_ASM! It shouldn't do that!"
#define OS_ASM	1
#endif

#endif
#endif

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
