# \file
#
# \brief AUTOSAR Os
#
# This file contains the implementation of the AUTOSAR
# module Os.
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 1998 - 2020 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.
#
# $Id: Os_defs_WINDOWS.mak 25258 2016-11-25 11:09:21Z olme8414 $

ifneq ($(OS_ARCH),WINDOWS)
$(error Wrong value in variable CPU, expecting WINDOWS.)
endif

#################################################################
# On WINDOWS/WIN32X86 threads are only controlled by Windows.
# This is done with the functions SuspendThread() and ResumeThread().
#
PREPROCESSOR_DEFINES += os_use_posix_signals_for_thread_control
os_use_posix_signals_for_thread_control_KEY=OS_USE_POSIX_SIGNALS_FOR_THREAD_CONTROL
os_use_posix_signals_for_thread_control_VALUE=0

#################################################################
# Set the minimum Windows version to be used. Without this macro, 
# some Windows API functions are not available. Must be set before 
# including windows.h.
# See also http://msdn.microsoft.com/en-us/library/6sehtctf.aspx .
#
# The value 0x0601 means Windows 7 and later.
#
PREPROCESSOR_DEFINES += os_win32_winnt
os_win32_winnt_KEY=_WIN32_WINNT
os_win32_winnt_VALUE=0x0601

PREPROCESSOR_DEFINES += os_win32_winver
os_win32_winver_KEY=WINVER
os_win32_winver_VALUE=0x0601

#################################################################
# Switch for the 64-bit support of the OS.
# Attention: The parameter PRIO_WORD_SIZE in the file
# data/configuration/WINDOWS/WIN32X86.conf must be changed accordingly.
#
PREPROCESSOR_DEFINES += compile_for_64bit
compile_for_64bit_KEY = OS_ARCH_HAS_64BIT
compile_for_64bit_VALUE = 0

#################################################################
#
OS_WINDOWS_PREFIX=$(OS_ARCH_PREFIX)
OS_WINLIN_PREFIX=WINLIN$(OS_HYPHEN)

$(OS_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,					\
		$(OS_KERN_PREFIX)leavekernel.$(CC_FILE_SUFFIX)				\
	)

OS_KLIB_OBJS_ARCH = \
    $(addprefix $(AutosarOS_LIB_SRC_PATH)/WINLIN/,                 \
        $(OS_WINLIN_PREFIX)clearinterrupt.$(CC_FILE_SUFFIX)        \
        $(OS_WINLIN_PREFIX)finishedirq.$(CC_FILE_SUFFIX)           \
        $(OS_WINLIN_PREFIX)finishedusinghostosservice.$(CC_FILE_SUFFIX)\
        $(OS_WINLIN_PREFIX)getstartoffilename.$(CC_FILE_SUFFIX)    \
        $(OS_WINLIN_PREFIX)globaldata.$(CC_FILE_SUFFIX)            \
        $(OS_WINLIN_PREFIX)goingtousehostosservice.$(CC_FILE_SUFFIX)\
        $(OS_WINLIN_PREFIX)initarchwinlin.$(CC_FILE_SUFFIX)        \
        $(OS_WINLIN_PREFIX)isrthread.$(CC_FILE_SUFFIX)             \
        $(OS_WINLIN_PREFIX)isthisasystemthread.$(CC_FILE_SUFFIX)   \
        $(OS_WINLIN_PREFIX)kernclearinterrupt.$(CC_FILE_SUFFIX)    \
        $(OS_WINLIN_PREFIX)kernfinishedirq.$(CC_FILE_SUFFIX)       \
        $(OS_WINLIN_PREFIX)kerntriggerinterruptviasyscall.$(CC_FILE_SUFFIX)\
        $(OS_WINLIN_PREFIX)killcalledcontext.$(CC_FILE_SUFFIX)     \
        $(OS_WINLIN_PREFIX)logging.$(CC_FILE_SUFFIX)               \
        $(OS_WINLIN_PREFIX)longjmp.$(CC_FILE_SUFFIX)               \
        $(OS_WINLIN_PREFIX)panic.$(CC_FILE_SUFFIX)                 \
        $(OS_WINLIN_PREFIX)predynamicstop.$(CC_FILE_SUFFIX)        \
        $(OS_WINLIN_PREFIX)prestartos.$(CC_FILE_SUFFIX)            \
        $(OS_WINLIN_PREFIX)setinterruptdisablelevel.$(CC_FILE_SUFFIX)\
        $(OS_WINLIN_PREFIX)setupisr.$(CC_FILE_SUFFIX)              \
        $(OS_WINLIN_PREFIX)startos.$(CC_FILE_SUFFIX)               \
        $(OS_WINLIN_PREFIX)syscall.$(CC_FILE_SUFFIX)               \
        $(OS_WINLIN_PREFIX)taskreturn.$(CC_FILE_SUFFIX)            \
        $(OS_WINLIN_PREFIX)taskthread.$(CC_FILE_SUFFIX)            \
        $(OS_WINLIN_PREFIX)timer$(OS_HYPHEN)frc.$(CC_FILE_SUFFIX)  \
    )
    
OS_KLIB_OBJS_ARCH += \
    $(addprefix $(AutosarOS_LIB_SRC_PATH)/WINDOWS/,                \
        $(OS_WINDOWS_PREFIX)kernusehostosservice.$(CC_FILE_SUFFIX) \
        $(OS_WINDOWS_PREFIX)usehostosservice.$(CC_FILE_SUFFIX)     \
        $(OS_WINDOWS_PREFIX)win32api.$(CC_FILE_SUFFIX)             \
    )                                                              \
    $(OS_KLIB_OBJS_DERIVATE)

OS_ULIB_OBJS_ARCH = \
    $(patsubst %.$(ASM_FILE_SUFFIX),%.$(CC_FILE_SUFFIX),$(OS_ULIB_OBJS_ARCH_COMMON)) \
    $(addprefix $(AutosarOS_LIB_SRC_PATH)/WINDOWS/,                                  \
        $(OS_WINDOWS_PREFIX)$(OS_USER_PREFIX)taskreturn.$(CC_FILE_SUFFIX)            \
    )
    
ifneq ($(OS_N_CORES),1)
OS_KLIB_OBJS_ARCH += \
    $(addprefix $(AutosarOS_LIB_SRC_PATH)/WINLIN/,        \
        $(OS_WINLIN_PREFIX)startcore.$(CC_FILE_SUFFIX)    \
        $(OS_WINLIN_PREFIX)mycorehelper.$(CC_FILE_SUFFIX) \
    )
    
OS_ULIB_OBJS_ARCH += \
    $(addprefix $(AutosarOS_LIB_SRC_PATH)/WINDOWS/,                             \
        $(OS_WINDOWS_PREFIX)$(OS_USER_PREFIX)shutdownallcores.$(CC_FILE_SUFFIX) \
        $(OS_WINDOWS_PREFIX)$(OS_USER_PREFIX)releasespinlock.$(CC_FILE_SUFFIX)  \
        $(OS_WINDOWS_PREFIX)$(OS_USER_PREFIX)trytogetspinlock.$(CC_FILE_SUFFIX) \
    )
endif # $(OS_N_CORES) != 1

Os_src_FILES += \
    $(addprefix $(AutosarOS_SRC_PATH)/WINLIN/,                             \
        Os_configuration_WINLIN.$(CC_FILE_SUFFIX)                          \
        $(OS_WINLIN_PREFIX)arethereanypendingirqstoserve.$(CC_FILE_SUFFIX) \
        $(OS_WINLIN_PREFIX)assembleisrevents.$(CC_FILE_SUFFIX)             \
        $(OS_WINLIN_PREFIX)coreeventloop.$(CC_FILE_SUFFIX)                 \
        $(OS_WINLIN_PREFIX)executesyscall.$(CC_FILE_SUFFIX)                \
        $(OS_WINLIN_PREFIX)isthisacorethread.$(CC_FILE_SUFFIX)             \
        $(OS_WINLIN_PREFIX)setlongjmp.$(CC_FILE_SUFFIX)                    \
        $(OS_WINLIN_PREFIX)timestamp.$(CC_FILE_SUFFIX)                     \
        $(OS_WINLIN_PREFIX)triggerinterrupt.$(CC_FILE_SUFFIX)              \
        $(OS_WINLIN_PREFIX)triggerinterruptviasyscall.$(CC_FILE_SUFFIX)    \
    )

include $(AutosarOS_CORE_PATH)/make/$(OS_ARCH)/$(OS_CPU)/Os_defs_$(OS_ARCH)_$(OS_CPU).mak

ifeq ($(strip $(TOOLCHAIN)),vc)

  # Note: the switch COMPILE_FOR_DEBUG is defined in WINDOWS_WIN32X86_$(TOOLCHAIN)_cfg.mak.
  ifndef TOOLPATH_WINSDK
    $(error The variable TOOLPATH_WINSDK is not defined but must point to the path containing the Windows SDK. See WINDOWS_WIN32X86_vc_cfg.mak.)
  endif
  ifndef TOOLPATH_COMPILER
    $(error The variable TOOLPATH_COMPILER is not defined but must point to the path containing Visual Studio. See WINDOWS_WIN32X86_vc_cfg.mak.)
  endif

  # Define the library search paths relative to the paths of compiler and SDK.
  LINK_OPT                += /LIBPATH:"$(TOOLPATH_WINSDK)\Lib"
  LINK_OPT                += /LIBPATH:"$(TOOLPATH_COMPILER)\lib"

  # Specify the target machine.
  # See http://msdn.microsoft.com/en-us/library/vstudio/5wy54dk2.aspx
  ifeq "$(compile_for_64bit_VALUE)" "0"
    LINK_OPT                += /MACHINE:X86
  else
    LINK_OPT                += /MACHINE:X64
  endif

  # Compatible with Data Execution Prevention
  # See http://msdn.microsoft.com/en-us/library/vstudio/ms235442%28v=vs.100%29.aspx
  LINK_OPT                += /NXCOMPAT

  # Use address space layout randomization
  # See http://msdn.microsoft.com/en-us/library/vstudio/bb384887%28v=vs.100%29.aspx
  LINK_OPT                += /DYNAMICBASE

  # The WINDOWS/WIN32X86 port uses multimedia timers for its
  # timer driver: WINDOWS-timer-frc.
  # See http://msdn.microsoft.com/en-us/library/windows/desktop/dd743609%28v=vs.85%29.aspx
  LINK_OPT                += winmm.lib

  # Note the default settings for the application's manifest
  # (http://msdn.microsoft.com/en-us/library/vstudio/aa375632%28v=vs.100%29.aspx)
  # are sufficient. Hence, no need to specify options.

  # The bunch of default libraries to get all the nice Windows goodies.
  # Probably not all of them are always needed, but they also do no harm.
  # See http://msdn.microsoft.com/en-us/library/vstudio/229a6ysd%28v=vs.100%29.aspx
  LINK_OPT                += /DEFAULTLIB:kernel32.lib
  LINK_OPT                += /DEFAULTLIB:user32.lib
  LINK_OPT                += /DEFAULTLIB:gdi32.lib
  LINK_OPT                += /DEFAULTLIB:winspool.lib
  LINK_OPT                += /DEFAULTLIB:comdlg32.lib
  LINK_OPT                += /DEFAULTLIB:advapi32.lib
  LINK_OPT                += /DEFAULTLIB:shell32.lib
  LINK_OPT                += /DEFAULTLIB:ole32.lib
  LINK_OPT                += /DEFAULTLIB:oleaut32.lib
  LINK_OPT                += /DEFAULTLIB:uuid.lib
  LINK_OPT                += /DEFAULTLIB:odbc32.lib
  LINK_OPT                += /DEFAULTLIB:odbccp32.lib
  ifeq ($(strip $(COMPILE_FOR_DEBUG)),YES)
    LINK_OPT                += /DEFAULTLIB:MSVCRTD.lib
  else
    LINK_OPT                += /DEFAULTLIB:MSVCRT.lib
  endif

else # $(TOOLCHAIN) != vc

  ifndef WIN32_LIB_PATH
    $(error The variable WIN32_LIB_PATH is not defined but must point to the path containing the Windows libraries. See WINDOWS_WIN32X86_$(TOOLCHAIN)_cfg.mak.)
  endif

  # Search path for Windows-specific libraries.
  LINK_OPT += -L$(WIN32_LIB_PATH)

  # WINDOWS/WIN32X86 uses multimedia timers
  # (http://msdn.microsoft.com/en-us/library/windows/desktop/dd743609%28v=vs.85%29.aspx).
  # These timer form the basis of the timer driver: WINDOWS-timer-frc.c.
  # They must be listed at last on the command-line, so that references from
  # within the OS can be resolved.
  # See also asc_Platforms/WINDOWS/trunk/src/Autosar/make/WINDOWS/<compiler>/compiler_rules.mak .
  LINK_OPT_POST += -lwinmm
  
endif # $(TOOLCHAIN) == vc

# Editor settings: DO NOT DELETE
# vi:set ts=4:

