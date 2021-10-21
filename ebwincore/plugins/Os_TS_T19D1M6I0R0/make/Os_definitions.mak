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

# Don't include this file more than once
ifndef OS_DEFINITIONS_MAK_INCLUDED
OS_DEFINITIONS_MAK_INCLUDED := 1

#################################################################
# DEFINITIONS
OS_SSC_ROOT = $(AutosarOS_CORE_PATH)

ifeq ($(AUTOSAR_BASE_OUTPUT_PATH),)
  AUTOSAR_BASE_OUTPUT_PATH         = $(PROJECT_OUTPUT_PATH)
endif

ifeq ($(AutosarOS_OUTPUT_PATH),)
  ifeq ($(AUTOSAR_BASE_OUTPUT_PATH),)
    AutosarOS_OUTPUT_PATH=$(PROJECT_ROOT)\output\generated
  else
    AutosarOS_OUTPUT_PATH=$(AUTOSAR_BASE_OUTPUT_PATH)
  endif
endif

OS_FEATURELIST_MAKEDIR := $(AutosarOS_CORE_PATH)/make
include $(AutosarOS_CORE_PATH)/make/Os_featureselection.mak

-include $(AutosarOS_OUTPUT_PATH)/make/Os_objects.make
include $(AutosarOS_CORE_PATH)/make/Os_libsuffix.mak

AutosarOS_SRC_PATH     = $(AutosarOS_CORE_PATH)\src
AutosarOS_LIB_SRC_PATH = $(AutosarOS_CORE_PATH)\lib_src

export OSEK_BASE       = $(AutosarOS_CORE_PATH)

-include output/generated/Os_objects.make

LIBRARIES_LINK_ONLY +=

CC_FILES_TO_BUILD += $(OS_GENERATED_APPL_FILES)

OS_GENERATED_FILES = $(AutosarOS_OUTPUT_PATH)\src\Os_gen.c			\
					$(OS_GENERATED_IOC_FILES)

ifeq ($(OS_KERNEL_TYPE),MICROKERNEL)
Os_src_FILES += $(AutosarOS_CORE_PATH)\src\Os_configuration_microkernel.c
else  # OS_KERNEL_TYPE == MICROKERNEL
Os_src_FILES += $(AutosarOS_CORE_PATH)\src\Os_configuration.c
ifneq ($(OS_N_CORES),1)
Os_src_FILES += $(AutosarOS_CORE_PATH)\src\Os_configuration_user.c
endif # OS_N_CORES neq 1
endif # OS_KERNEL_TYPE == MICROKERNEL
Os_src_FILES += $(AutosarOS_CORE_PATH)\src\Os_configuration_c0.c
Os_src_FILES += $(AutosarOS_CORE_PATH)\src\Os_configuration_c1.c
Os_src_FILES += $(AutosarOS_CORE_PATH)\src\Os_configuration_c2.c
Os_src_FILES += $(AutosarOS_CORE_PATH)\src\Os_configuration_c3.c
Os_src_FILES += $(AutosarOS_CORE_PATH)\src\Os_configuration_c4.c
Os_src_FILES += $(AutosarOS_CORE_PATH)\src\Os_configuration_c5.c
Os_src_FILES += $(AutosarOS_CORE_PATH)\src\Os_configuration_c6.c
Os_src_FILES += $(AutosarOS_CORE_PATH)\src\Os_configuration_c7.c

Os_src_FILES += $(OS_GENERATED_FILES)								\
				$(AutosarOS_SRC_PATH)\Os_rtewrappers.c

ifeq ($(OS_HAVE_TICKERS),TRUE)
Os_src_FILES += $(AutosarOS_SRC_PATH)\Os_starttickers.c
endif

TRESOS_GEN_FILES += $(OS_GENERATED_FILES)							\
					$(AutosarOS_OUTPUT_PATH)\make\Os_objects.make	\
					$(AutosarOS_OUTPUT_PATH)\include\Os_config.h	\
					$(AutosarOS_OUTPUT_PATH)\include\Ioc_gen.h

TRESOS_DEPEND_CFG_FILES += $(AutosarOS_Config_Filename)

PREPROCESSOR_DEFINES+=os_cpu os_tool os_arch os_relsuffix

os_ncores_KEY=OS_N_CORES
os_ncores_VALUE=$(OS_N_CORES)

os_cpu_KEY=OS_CPU
os_cpu_VALUE=OS_$(OS_CPU)

os_relsuffix_KEY=OS_RELEASE_SUFFIX
os_relsuffix_VALUE=OS_$(OS_RELEASE_SUFFIX)

os_tool_KEY=OS_TOOL

############ TOOLCHAIN mapping ##############

OS_TOOLCHAIN=$(TOOLCHAIN)

ifeq ($(TOOLCHAIN),multi)
OS_TOOLCHAIN=ghs
endif

ifeq ($(TOOLCHAIN),gcc)
OS_TOOLCHAIN=gnu
endif

ifeq ($(TOOLCHAIN),gcc4)
OS_TOOLCHAIN=gnu
endif

ifeq ($(TOOLCHAIN),bcgcc)
OS_TOOLCHAIN=gnu
endif

ifeq ($(TOOLCHAIN),mgcc45)
OS_TOOLCHAIN=gnu
endif

ifeq ($(TOOLCHAIN),mgcc62)
OS_TOOLCHAIN=gnu
endif

ifeq ($(TOOLCHAIN),dcc)
OS_TOOLCHAIN=diab
endif

ifeq ($(TOOLCHAIN),cw)
OS_TOOLCHAIN=cw
endif

ifeq ($(TOOLCHAIN),cosmic)
OS_TOOLCHAIN=cosmic
endif

ifeq ($(TOOLCHAIN),taskingvx)
OS_TOOLCHAIN=tasking
endif

ifeq ($(TOOLCHAIN),armkeil)
OS_TOOLCHAIN=armkeil
endif

ifeq ($(TOOLCHAIN),arm5)
OS_TOOLCHAIN=arm5
endif

#############################################

os_tool_VALUE=OS_$(OS_TOOLCHAIN)

os_arch_KEY=OS_ARCH
os_arch_VALUE=OS_$(OS_ARCH)

ifeq ($(OS_BUILD_OPTIMIZED_LIB_FROM_SOURCE),TRUE)
OS_KERNEL_LIB_NAME = Os_$(OS_OPTIMIZED_LIB_ID)_kern_lib_$(OS_LIB_SUFFIX)
else
OS_KERNEL_LIB_NAME = Os_Static_kern_lib_$(OS_LIB_SUFFIX)
endif

OS_USER_LIB_NAME=Os_Static_user_lib_$(OS_LIB_SUFFIX)


ifeq ($(wildcard $(AutosarOS_LIB_SRC_PATH)\kernel\$(OS_KERN_PREFIX)activatetask.c),)

ifeq ($(wildcard $(AutosarOS_LIB_SRC_PATH)\kernel\Os_k_activatetask.c),)

# No source available
OS_LIB_SRC_AVAILABLE=1
OS_HYPHEN=-
OS_KERN_PREFIX=kern-
OS_USER_PREFIX=user-
OS_LIB_PREFIX=lib-
OS_ARCH_PREFIX=$(OS_ARCH)-

else

# "Renamed" release - filenames renamed, hyphens changed to underscores, prefixed with Os_
OS_LIB_SRC_AVAILABLE=1
OS_HYPHEN=_
OS_KERN_PREFIX=Os_k_
OS_USER_PREFIX=Os_u_
OS_LIB_PREFIX=Os_l_
OS_ARCH_PREFIX=Os_$(OS_ARCH)_

endif

else

# Normal release - filenames as in repository
OS_LIB_SRC_AVAILABLE=1
OS_HYPHEN=-
OS_KERN_PREFIX=kern-
OS_USER_PREFIX=user-
OS_LIB_PREFIX=lib-
OS_ARCH_PREFIX=$(OS_ARCH)-

endif

ifeq ($(OS_BUILD_OPTIMIZED_LIB_FROM_SOURCE),TRUE)
ifneq ($(OS_LIB_SRC_AVAILABLE),1)
$(error  No $(OS_LIB_PREFIX)Source found ! Please switch off OS_BUILD_OPTIMIZED_LIB_FROM_SOURCE in OS_objects.make );
endif
endif


# The kernel library. Its name depends on the configuration


# Not all of the kernel files depend on the configuration
# Those that don't could be built into a static kernel library.
# Doing so would speed up the build process quite a lot.
$(OS_KERNEL_LIB_NAME)_FILES = \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,		\
		$(OS_KERN_PREFIX)activateautotasks.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)cancelalarm.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)localcancelalarm.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)chainscheduletable.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)localchainscheduletable.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)incrementcounter.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)setabsalarm.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)setrelalarm.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)setscheduletableasync.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)localsetscheduletableasync.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)startscheduletable.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)startscheduletablesynchron.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)stopscheduletable.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)localstopscheduletable.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)syncscheduletable.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)localsyncscheduletable.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)killalarm.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)unknownsyscall.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)enqueuealarm.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)getalarmdelta.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)advancecounter.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)localsetalarm.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)activateautoalarms.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)activateautoschedules.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)initalarms.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)initschedules.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)inittimers.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)killschedule.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)resetschedulealarm.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)runschedule.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)setschedulealarm.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)wrapactivatetask.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)wrapincrementcounter.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)wraprunschedule.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)wrapsetevent.$(CC_FILE_SUFFIX)             \
		$(OS_KERN_PREFIX)hwcounterupdate.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)synchronize.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)initcounters.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)panic.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)getcount.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)getalarm.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)localgetalarm.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)getalarmbase.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)getcountervalue.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)getscheduletablestatus.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)wrapalarmcallback.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)localstartscheduletable.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)localstartscheduletablesynchron.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)startupchecksassembleroffsets.$(CC_FILE_SUFFIX)	\
	) \
	$(OS_KLIB_OBJS_ARCH)

ifeq ($(DIRMERGE_SETTINGS_TAG_OsFeature_SharedTimestampAndCounterTimer),enabled)
$(OS_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,		\
		$(OS_KERN_PREFIX)hwcountertimestampupdate.$(CC_FILE_SUFFIX)	\
	)
endif

ifneq ($(OS_KERNEL_TYPE),MICROKERNEL)
ifeq ($(DIRMERGE_SETTINGS_TAG_OsFeature_GenericTimestamp),enabled)
$(OS_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,		\
		$(OS_KERN_PREFIX)advancetimestampgeneric.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)gettimestampgeneric.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)starttimestamptimergeneric.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)timestampupdate.$(CC_FILE_SUFFIX)			\
	)
endif
endif

$(OS_USER_LIB_NAME)_FILES = \
	$(OS_ULIB_OBJS_ARCH)

ifeq ($(OS_KERNEL_TYPE),MICROKERNEL)
$(OS_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,		\
		mka-getactiveapplicationmode.$(CC_FILE_SUFFIX)				\
		mka-getalarmbase.$(CC_FILE_SUFFIX)							\
		mka-getelapsedcountervalue.$(CC_FILE_SUFFIX)				\
		mka-getscheduletablestatus.$(CC_FILE_SUFFIX)				\
		mki-getappmode.$(CC_FILE_SUFFIX)							\
		mki-reporterror.$(CC_FILE_SUFFIX)							\
		mkw-getalarm.$(CC_FILE_SUFFIX)								\
		mkw-getcountervalue.$(CC_FILE_SUFFIX)						\
		mkw-startos.$(CC_FILE_SUFFIX)								\
		mkw-setrelalarm.$(CC_FILE_SUFFIX)							\
		mkw-setabsalarm.$(CC_FILE_SUFFIX)							\
		mkw-cancelalarm.$(CC_FILE_SUFFIX)							\
		mkw-nextscheduletable.$(CC_FILE_SUFFIX)						\
		mkw-setscheduletableasync.$(CC_FILE_SUFFIX)					\
		mkw-startscheduletable.$(CC_FILE_SUFFIX)					\
		mkw-stopscheduletable.$(CC_FILE_SUFFIX)						\
		mkw-syncscheduletable.$(CC_FILE_SUFFIX)						\
		mkw-terminateapplication.$(CC_FILE_SUFFIX)					\
		mkw-updatecounter.$(CC_FILE_SUFFIX)							\
		mkw-incrementcounter.$(CC_FILE_SUFFIX)						\
		mkw-startscheduletablesynchron.$(CC_FILE_SUFFIX)			\
	)

# dummy to avoid missing file error
$(OS_USER_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,		\
		$(OS_KERN_PREFIX)nullfunction.$(CC_FILE_SUFFIX)		\
	)

else # $(OS_KERNEL_TYPE) != MICROKERNEL

$(OS_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,		\
		$(OS_KERN_PREFIX)appisaccessible.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)terminateapplication.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)localterminateapplication.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)getapplicationstate.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)allowaccess.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)releaselocks.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)startos.$(CC_FILE_SUFFIX)					\
	)

ifeq ($(DIRMERGE_SETTINGS_TAG_OsFeature_CheckForDispatch),enabled)
$(OS_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,		\
		$(OS_KERN_PREFIX)checkfordispatch.$(CC_FILE_SUFFIX)	\
	)
endif

ifneq ($(OS_N_CORES),1)
$(OS_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,			\
		$(OS_KERN_PREFIX)crosscorenotifyisr.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)takeinternallock.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)dropinternallock.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)dropinternalspinlockonly.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)trytogetspinlock.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)dotrytogetspinlock.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)releasespinlock.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)initmessagequeue.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)receivemessage.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)sendmessage.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)sendmessagewait.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)retntaskstate.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)recvgettaskstate.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)sendgettaskstate.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)sendactivatetask.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)recvactivatetask.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)recvunknowncall.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)recvsetevent.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)sendsetevent.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)retnevent.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)recvgetevent.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)sendgetevent.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)sendgetalarm.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)retntick.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)recvgetalarm.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)sendsetrelalarm.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)sendsetabsalarm.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)recvsetrelalarm.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)recvsetabsalarm.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)retnresult.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)sendcancelalarm.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)recvcancelalarm.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)recvdisableinterruptsource.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)senddisableinterruptsource.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)recvenableinterruptsource.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)sendenableinterruptsource.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)sync.$(CC_FILE_SUFFIX)							\
		$(OS_KERN_PREFIX)synchereif.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)sendstartscheduletable.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)recvstartscheduletable.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)recvstartscheduletablesynchron.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)sendstartscheduletablesynchron.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)recvstopscheduletable.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)sendstopscheduletable.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)sendsyncscheduletable.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)recvsyncscheduletable.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)sendchainscheduletable.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)recvchainscheduletable.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)recvsetscheduletableasync.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)sendsetscheduletableasync.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)recvgetscheduletablestatus.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)sendgetscheduletablestatus.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)retnscheduletablestatus.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)sendgetcount.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)recvgetcount.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)sendadvancecounter.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)recvadvancecounter.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)sendterminateapplication.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)recvterminateapplication.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)sendgetapplicationstate.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)recvgetapplicationstate.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)retnapplicationstate.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)shutdownallcores.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)recvshutdowncore.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)retncpuload.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)sendmeasurecpuload.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)recvmeasurecpuload.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)initautosarspinlocks.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)multiactivatetask.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)multicancelalarm.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)multichainscheduletable.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)multigetalarm.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)multigetevent.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)multisetalarm.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)multisetevent.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)multisetscheduletableasync.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)multistartscheduletable.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)multistartscheduletablesynchron.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)multistopscheduletable.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)multisyncscheduletable.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)multiterminateapplication.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)releasespinlockscurcore.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)multidetermineappmode.$(CC_FILE_SUFFIX)		\
	)
endif # $(OS_N_CORES) != 1

$(OS_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,		\
		$(OS_KERN_PREFIX)actitask.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)activatetask.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)callerrorhook.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)callshutdownhook.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)callstartuphook.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)calltrustedfunction.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)canwrite.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)cat1entry.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)cat2entry.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)catkentry.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)chaintask.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)checkisrmemoryaccess.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)checkobjectaccess.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)checkobjectownership.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)checktaskmemoryaccess.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)clearevent.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)dequeueall.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)dequeue.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)disableinterruptsource.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)dispatch.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)localactivatetask.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)localsetevent.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)doterminatetask.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)enableinterruptsource.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)enqueue.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)enqueuelink.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)erroraction.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)errorminimal.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)errorminimalparam.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)error.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)exceedexectime.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)exectimerinterrupt.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)getactiveapplicationmode.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)getapplicationid.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)dogetapplicationid.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)getcpuload.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)measurecoreload.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)getcurrentstackarea.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)getelapsedcountervalue.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)getevent.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)localgetevent.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)getisrid.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)getisrmaxruntime.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)getresource.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)getresourcefromisr.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)getresourcefromtask.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)getstackinfo.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)gettaskid.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)gettaskmaxruntime.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)gettaskstate.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)gettimeused.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)initapp.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)initappdata.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)initexecutiontiming.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)initinterrupts.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)initisrstacks.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)initkernstack.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)initmeasurecpuload.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)initresources.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)inittasks.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)inittaskstacks.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)isschedulenecessary.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)isscheduleworthwhile.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)killhook.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)killisr.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)killtask.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)lowercurrenttaskspriority.$(CC_FILE_SUFFIX)\
		$(OS_KERN_PREFIX)measurecpuload.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)missingterminatetask.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)nullerrorhook.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)nullfunction.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)nullisrhook.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)nullprotectionhook.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)permittedcontextautosar.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)permittedcontextosek.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)preemptisrexectiming.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)preempttaskexectiming.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)quarantineapplication.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)raisetaskpriority.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)ratemonitor.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)releaseresource.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)requeuedown.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)requeueup.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)resetpeakcpuload.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)restartapplication.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)resumeinterrupts.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)resumeisrexectiming.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)schedule.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)setevent.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)shutdown.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)shutdownos.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)simtimer.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)startisrexectiming.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)startisrintlocktiming.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)startreslocktiming.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)starttaskexectiming.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)starttaskintlocktiming.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)startupchecks.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)startupchecksalarm.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)startupchecksapplication.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)startupcheckscounter.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)startupchecksisr.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)startupchecksresource.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)startupchecksschedule.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)startupcheckstask.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)startupcheckstimer.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)startupcheckstrustedfunction.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)stopisrexectiming.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)stopisrintlocktiming.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)stopreslocktiming.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)stoptaskexectiming.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)stoptaskintlocktiming.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)suspendinterrupts.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)taskreturn.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)taskspoutofbounds.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)terminatetask.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)unknowninterrupt.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)waitevent.$(CC_FILE_SUFFIX)				\
	)

ifeq ($(DIRMERGE_SETTINGS_TAG_OsFeature_SoftwareCLZ),enabled)
$(OS_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,		\
		$(OS_KERN_PREFIX)countleadingzeros.$(CC_FILE_SUFFIX)		\
	)
endif

ifeq ($(DIRMERGE_SETTINGS_TAG_OsFeature_SharedTimestampAndExecTimer),enabled)
$(OS_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,		\
		$(OS_KERN_PREFIX)exectimertimestampinterrupt.$(CC_FILE_SUFFIX)	\
	)
endif

ifeq ($(DIRMERGE_SETTINGS_TAG_OsFeature_GenericReturnFromCall),enabled)
$(OS_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,		\
		$(OS_KERN_PREFIX)returnfromcall.$(CC_FILE_SUFFIX)			\
	)
endif

ifeq ($(DIRMERGE_SETTINGS_TAG_OsFeature_CacheOperations),enabled)
$(OS_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,		\
		$(OS_KERN_PREFIX)cacheflushfn.$(CC_FILE_SUFFIX)								\
		$(OS_KERN_PREFIX)cacheinvalidatefn.$(CC_FILE_SUFFIX)						\
	)
endif

$(OS_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\ioc\,			\
		$(OS_KERN_PREFIX)ioccheckmemoryaccess.$(CC_FILE_SUFFIX)		\
	)

# Files for the CLZ scheduling algorithm
ifeq ($(OS_SCHEDULING_ALGORITHM),CLZ_QUEUE)
$(OS_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,					\
		$(OS_KERN_PREFIX)enqueuetask.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)dequeuetask.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)startupchecksqueue.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)findhighesttask.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)inserttask.$(CC_FILE_SUFFIX)				\
	)
endif # $(OS_SCHEDULING_ALGORITHM) == CLZ_QUEUE

# The user library - does not depend on the configuration
$(OS_USER_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\user\,		\
		$(OS_LIB_PREFIX)difftime32.$(CC_FILE_SUFFIX)				\
		$(OS_LIB_PREFIX)fastresumeallinterrupts.$(CC_FILE_SUFFIX)	\
		$(OS_LIB_PREFIX)fastresumeosinterrupts.$(CC_FILE_SUFFIX)	\
		$(OS_LIB_PREFIX)fastsuspendallinterrupts.$(CC_FILE_SUFFIX)	\
		$(OS_LIB_PREFIX)fastsuspendosinterrupts.$(CC_FILE_SUFFIX)	\
		$(OS_LIB_PREFIX)getcoreid.$(CC_FILE_SUFFIX)					\
		$(OS_LIB_PREFIX)geterrorinfo.$(CC_FILE_SUFFIX)				\
		$(OS_LIB_PREFIX)gettaskstate.$(CC_FILE_SUFFIX)				\
		$(OS_LIB_PREFIX)getunusedisrstack.$(CC_FILE_SUFFIX)			\
		$(OS_LIB_PREFIX)getunusedtaskstack.$(CC_FILE_SUFFIX)		\
		$(OS_LIB_PREFIX)getusedisrstack.$(CC_FILE_SUFFIX)			\
		$(OS_LIB_PREFIX)getusedtaskstack.$(CC_FILE_SUFFIX)			\
		$(OS_LIB_PREFIX)stackcheck.$(CC_FILE_SUFFIX)				\
		$(OS_LIB_PREFIX)getscheduletablestatus.$(CC_FILE_SUFFIX)	\
		$(OS_LIB_PREFIX)salsa-getstackusage.$(CC_FILE_SUFFIX)		\
		$(OS_LIB_PREFIX)salsa-getsystemstackusage.$(CC_FILE_SUFFIX)	\
	)

ifneq ($(OS_N_CORES),1)
$(OS_USER_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\user\,					\
		$(OS_LIB_PREFIX)getspinlock.$(CC_FILE_SUFFIX)				\
		$(OS_LIB_PREFIX)getresscheduler.$(CC_FILE_SUFFIX)			\
		$(OS_LIB_PREFIX)startcore.$(CC_FILE_SUFFIX)					\
	)
endif # $(OS_N_CORES) != 1

ifeq ($(OS_ALCI),TRUE)
$(OS_USER_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\user\,					\
		$(OS_LIB_PREFIX)getphysicalcoreid.$(CC_FILE_SUFFIX)			\
		$(OS_LIB_PREFIX)getlogicalcoreid.$(CC_FILE_SUFFIX)			\
	)
endif # $(OS_ALCI) == TRUE

endif # $(OS_KERNEL_TYPE) !=  MICROKERNEL

ifeq ($(OS_KERNEL_TYPE),SYSTEM_CALL)
ifeq ($(DIRMERGE_SETTINGS_TAG_OsFeature_GenericMMU),enabled)
$(OS_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,		\
		$(OS_KERN_PREFIX)mmupagify.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)mmusort.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)mmumerge.$(CC_FILE_SUFFIX)					\
		$(OS_KERN_PREFIX)mmutaskmapper.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)mmuisrmapper.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)mmuappmapper.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)mmuoptimise.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)mmuenterprotectedmode.$(CC_FILE_SUFFIX)	\
		$(OS_KERN_PREFIX)mmusetprotection.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)mmusetisrprotection.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)mmusethookprotection.$(CC_FILE_SUFFIX)		\
		$(OS_KERN_PREFIX)initmem.$(CC_FILE_SUFFIX)			\
	)
endif
ifeq ($(DIRMERGE_SETTINGS_TAG_OsFeature_GenericMMU_GetMem),enabled)
$(OS_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,		\
		$(OS_KERN_PREFIX)getmem.$(CC_FILE_SUFFIX)			\
	)
endif
ifeq ($(DIRMERGE_SETTINGS_TAG_OsFeature_GenericMMU_GetMemAlign),enabled)
$(OS_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,		\
		$(OS_KERN_PREFIX)getmemaligned.$(CC_FILE_SUFFIX)	\
	)
endif
ifeq ($(DIRMERGE_SETTINGS_TAG_OsFeature_GenericMMU_PageMap),enabled)
$(OS_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,		\
		$(OS_KERN_PREFIX)mmubestpagesize.$(CC_FILE_SUFFIX)			\
		$(OS_KERN_PREFIX)mmupagemap.$(CC_FILE_SUFFIX)				\
		$(OS_KERN_PREFIX)mmumappages.$(CC_FILE_SUFFIX)				\
	)
endif
ifeq ($(DIRMERGE_SETTINGS_TAG_OsFeature_GenericMMU_PadReadOnly),enabled)
$(OS_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,		\
		$(OS_KERN_PREFIX)mmupadramreadonly.$(CC_FILE_SUFFIX)		\
	)
endif

OS_ULIB_OBJS_ARCH_COMMON = \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\$(OS_ARCH)\,					\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)activatetask.$(ASM_FILE_SUFFIX)				\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)allowaccess.$(ASM_FILE_SUFFIX)				\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)incrementcounter.$(ASM_FILE_SUFFIX)			\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)calltrustedfunction.$(ASM_FILE_SUFFIX)		\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)cancelalarm.$(ASM_FILE_SUFFIX)				\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)chainscheduletable.$(ASM_FILE_SUFFIX)			\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)chaintask.$(ASM_FILE_SUFFIX)					\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)checkisrmemoryaccess.$(ASM_FILE_SUFFIX)		\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)checkobjectaccess.$(ASM_FILE_SUFFIX)			\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)checkobjectownership.$(ASM_FILE_SUFFIX)		\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)checktaskmemoryaccess.$(ASM_FILE_SUFFIX)		\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)clearevent.$(ASM_FILE_SUFFIX)					\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)disableinterruptsource.$(ASM_FILE_SUFFIX)		\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)enableinterruptsource.$(ASM_FILE_SUFFIX)		\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)getactiveapplicationmode.$(ASM_FILE_SUFFIX)	\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)getalarm.$(ASM_FILE_SUFFIX)					\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)getalarmbase.$(ASM_FILE_SUFFIX)				\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)getapplicationid.$(ASM_FILE_SUFFIX)			\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)getapplicationstate.$(ASM_FILE_SUFFIX)		\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)getcountervalue.$(ASM_FILE_SUFFIX)			\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)getcpuload.$(ASM_FILE_SUFFIX)					\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)getelapsedcountervalue.$(ASM_FILE_SUFFIX)		\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)getevent.$(ASM_FILE_SUFFIX)					\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)getisrid.$(ASM_FILE_SUFFIX)					\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)getresource.$(ASM_FILE_SUFFIX)				\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)getscheduletablestatus.$(ASM_FILE_SUFFIX)		\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)getstackinfo.$(ASM_FILE_SUFFIX)				\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)gettaskid.$(ASM_FILE_SUFFIX)					\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)gettaskstate.$(ASM_FILE_SUFFIX)				\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)iocemptyqueue.$(ASM_FILE_SUFFIX)				\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)iocsend.$(ASM_FILE_SUFFIX)					\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)iocwrite.$(ASM_FILE_SUFFIX)					\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)iocaddonext.$(ASM_FILE_SUFFIX)				\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)releaseresource.$(ASM_FILE_SUFFIX)			\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)resumeinterrupts.$(ASM_FILE_SUFFIX)			\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)resetpeakcpuload.$(ASM_FILE_SUFFIX)			\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)schedule.$(ASM_FILE_SUFFIX)					\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)setabsalarm.$(ASM_FILE_SUFFIX)				\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)setevent.$(ASM_FILE_SUFFIX)					\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)setrelalarm.$(ASM_FILE_SUFFIX)				\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)setscheduletableasync.$(ASM_FILE_SUFFIX)		\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)shutdownos.$(ASM_FILE_SUFFIX)					\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)simtimeradvance.$(ASM_FILE_SUFFIX)			\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)startos.$(ASM_FILE_SUFFIX)					\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)startscheduletable.$(ASM_FILE_SUFFIX)			\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)startscheduletablesynchron.$(ASM_FILE_SUFFIX)	\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)stopscheduletable.$(ASM_FILE_SUFFIX)			\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)suspendinterrupts.$(ASM_FILE_SUFFIX)			\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)syncscheduletable.$(ASM_FILE_SUFFIX)			\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)terminateapplication.$(ASM_FILE_SUFFIX)		\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)terminatetask.$(ASM_FILE_SUFFIX)				\
		$(OS_ARCH_PREFIX)$(OS_USER_PREFIX)waitevent.$(ASM_FILE_SUFFIX)					\
	)
endif # OS_KERNEL_TYPE == SYSTEM_CALL

ifeq ($(OS_KERNEL_TYPE),FUNCTION_CALL)
ifeq ($(DIRMERGE_SETTINGS_TAG_OsFeature_GenericLeaveKernel),enabled)
$(OS_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\kernel\,		\
		$(OS_KERN_PREFIX)leavekernel.$(CC_FILE_SUFFIX)					\
	)
endif
endif # OS_KERNEL_TYPE == FUNCTION_CALL

#################################################################
# configure library variant options
ifeq ($(BUILD_MODE),LIB)
ifeq ($(MODULE),Os)
LIB_VARIANTS         += Os_EXTENDED Os_STANDARD
endif
endif
Os_STANDARD_STATUS := FALSE
Os_EXTENDED_STATUS := TRUE

ifeq ($(LIB_VARIANT),Os_EXTENDED)
OS_CONFIG_EXTENDED_STATUS=TRUE
endif



# The error-table library - the library to use depends on the configuration,
# but each library in itself is configuration-independent.
#
# If EXTENDED status is selected we must always use the Extended library.
# If STANDARD status is selected, things get a bit more complicated:
#  -- If the build is non-optimised, we must use the Universal library.
#     (We *could* also use the Extended library; that would result in an
#      EXTENDED mode system, which seems a bit pointless)
#  -- If the build is optimised, we *could* use either the Standard
#     or Extended library, but since many of the errors in these libraries
#     will never occur (because the tests are disabled), we use the
#     Standard library, which should (when everything is working
#     properly) be smaller.
ifeq ($(OS_KERNEL_TYPE),MICROKERNEL)
OS_ERROR_LIB_NAME =
else
ifeq ($(OS_CONFIG_EXTENDED_STATUS),TRUE)
  OS_ERROR_LIB_NAME_INFIX = _Extended
  OS_ERROR_INFIX = errext
else
  ifeq ($(OS_BUILD_OPTIMIZED_LIB_FROM_SOURCE),TRUE)
    OS_ERROR_LIB_NAME_INFIX = _Standard
    OS_ERROR_INFIX = errstd
  else
    OS_ERROR_LIB_NAME_INFIX = _Universal
    OS_ERROR_INFIX = erruni
  endif
endif
OS_ERROR_LIB_NAME = Os$(OS_ERROR_LIB_NAME_INFIX)_error_lib_$(OS_LIB_SUFFIX)

OS_ERROR_LIB_SRC = $(OS_KERN_PREFIX)$(OS_ERROR_INFIX)$(OS_HYPHEN)

$(OS_ERROR_LIB_NAME)_FILES = \
	$(addprefix $(AutosarOS_LIB_SRC_PATH)\error\,					\
		$(OS_ERROR_LIB_SRC)ActivateTask.$(CC_FILE_SUFFIX)			\
		$(OS_ERROR_LIB_SRC)AllowAccess.$(CC_FILE_SUFFIX)			\
		$(OS_ERROR_LIB_SRC)IncrementCounter.$(CC_FILE_SUFFIX)		\
		$(OS_ERROR_LIB_SRC)CallTrustedFunction.$(CC_FILE_SUFFIX)	\
		$(OS_ERROR_LIB_SRC)CancelAlarm.$(CC_FILE_SUFFIX)			\
		$(OS_ERROR_LIB_SRC)ChainScheduleTable.$(CC_FILE_SUFFIX)		\
		$(OS_ERROR_LIB_SRC)ChainTask.$(CC_FILE_SUFFIX)				\
		$(OS_ERROR_LIB_SRC)CheckIsrMemoryAccess.$(CC_FILE_SUFFIX)	\
		$(OS_ERROR_LIB_SRC)CheckObjectAccess.$(CC_FILE_SUFFIX)		\
		$(OS_ERROR_LIB_SRC)CheckObjectOwnership.$(CC_FILE_SUFFIX)	\
		$(OS_ERROR_LIB_SRC)CheckTaskMemoryAccess.$(CC_FILE_SUFFIX)	\
		$(OS_ERROR_LIB_SRC)ClearEvent.$(CC_FILE_SUFFIX)				\
		$(OS_ERROR_LIB_SRC)DisableInterruptSource.$(CC_FILE_SUFFIX)	\
		$(OS_ERROR_LIB_SRC)Dispatch.$(CC_FILE_SUFFIX)				\
		$(OS_ERROR_LIB_SRC)EnableInterruptSource.$(CC_FILE_SUFFIX)	\
		$(OS_ERROR_LIB_SRC)GetActiveApplicationMode.$(CC_FILE_SUFFIX)	\
		$(OS_ERROR_LIB_SRC)GetAlarm.$(CC_FILE_SUFFIX)				\
		$(OS_ERROR_LIB_SRC)GetAlarmBase.$(CC_FILE_SUFFIX)			\
		$(OS_ERROR_LIB_SRC)GetApplicationId.$(CC_FILE_SUFFIX)		\
		$(OS_ERROR_LIB_SRC)GetApplicationState.$(CC_FILE_SUFFIX)	\
		$(OS_ERROR_LIB_SRC)GetCounterValue.$(CC_FILE_SUFFIX)		\
		$(OS_ERROR_LIB_SRC)GetElapsedCounterValue.$(CC_FILE_SUFFIX)	\
		$(OS_ERROR_LIB_SRC)GetEvent.$(CC_FILE_SUFFIX)				\
		$(OS_ERROR_LIB_SRC)GetIsrId.$(CC_FILE_SUFFIX)				\
		$(OS_ERROR_LIB_SRC)GetResource.$(CC_FILE_SUFFIX)			\
		$(OS_ERROR_LIB_SRC)GetScheduleTableStatus.$(CC_FILE_SUFFIX)	\
		$(OS_ERROR_LIB_SRC)GetStackInfo.$(CC_FILE_SUFFIX)			\
		$(OS_ERROR_LIB_SRC)GetTaskId.$(CC_FILE_SUFFIX)				\
		$(OS_ERROR_LIB_SRC)GetTaskState.$(CC_FILE_SUFFIX)			\
		$(OS_ERROR_LIB_SRC)HookHandler.$(CC_FILE_SUFFIX)			\
		$(OS_ERROR_LIB_SRC)IsrHandler.$(CC_FILE_SUFFIX)				\
		$(OS_ERROR_LIB_SRC)KillAlarm.$(CC_FILE_SUFFIX)				\
		$(OS_ERROR_LIB_SRC)ReleaseResource.$(CC_FILE_SUFFIX)		\
		$(OS_ERROR_LIB_SRC)ResumeInterrupts.$(CC_FILE_SUFFIX)		\
		$(OS_ERROR_LIB_SRC)RunSchedule.$(CC_FILE_SUFFIX)			\
		$(OS_ERROR_LIB_SRC)Schedule.$(CC_FILE_SUFFIX)				\
		$(OS_ERROR_LIB_SRC)SetAbsAlarm.$(CC_FILE_SUFFIX)			\
		$(OS_ERROR_LIB_SRC)SetEvent.$(CC_FILE_SUFFIX)				\
		$(OS_ERROR_LIB_SRC)SetRelAlarm.$(CC_FILE_SUFFIX)			\
		$(OS_ERROR_LIB_SRC)SetScheduleTableAsync.$(CC_FILE_SUFFIX)	\
		$(OS_ERROR_LIB_SRC)ShutdownAllCores.$(CC_FILE_SUFFIX)		\
		$(OS_ERROR_LIB_SRC)ShutdownOs.$(CC_FILE_SUFFIX)				\
		$(OS_ERROR_LIB_SRC)StartOs.$(CC_FILE_SUFFIX)				\
		$(OS_ERROR_LIB_SRC)StartScheduleTableAbs.$(CC_FILE_SUFFIX)	\
		$(OS_ERROR_LIB_SRC)StartScheduleTableRel.$(CC_FILE_SUFFIX)	\
		$(OS_ERROR_LIB_SRC)StartScheduleTableSynchron.$(CC_FILE_SUFFIX)	\
		$(OS_ERROR_LIB_SRC)StopScheduleTable.$(CC_FILE_SUFFIX)		\
		$(OS_ERROR_LIB_SRC)SuspendInterrupts.$(CC_FILE_SUFFIX)		\
		$(OS_ERROR_LIB_SRC)SyncScheduleTable.$(CC_FILE_SUFFIX)		\
		$(OS_ERROR_LIB_SRC)TaskReturn.$(CC_FILE_SUFFIX)				\
		$(OS_ERROR_LIB_SRC)TerminateApplication.$(CC_FILE_SUFFIX)	\
		$(OS_ERROR_LIB_SRC)TerminateTask.$(CC_FILE_SUFFIX)			\
		$(OS_ERROR_LIB_SRC)TrapHandler.$(CC_FILE_SUFFIX)			\
		$(OS_ERROR_LIB_SRC)UnknownSyscall.$(CC_FILE_SUFFIX)			\
		$(OS_ERROR_LIB_SRC)WaitEvent.$(CC_FILE_SUFFIX)				\
		$(OS_ERROR_LIB_SRC)GetCpuLoad.$(CC_FILE_SUFFIX)				\
		$(OS_ERROR_LIB_SRC)ResetPeakCpuLoad.$(CC_FILE_SUFFIX)		\
		$(OS_ERROR_LIB_SRC)ReleaseSpinlock.$(CC_FILE_SUFFIX)		\
		$(OS_ERROR_LIB_SRC)TryToGetSpinlock.$(CC_FILE_SUFFIX)		\
	)																\
	$(OS_ELIB_OBJS_ARCH)

endif

#########################


###### new Features ####
EXCLUDE_MAKE_DEPEND  += \
	$($(OS_KERNEL_LIB_NAME)_FILES) \
	$($(OS_USER_LIB_NAME)_FILES)
ifneq ($(OS_KERNEL_TYPE),MICROKERNEL)
EXCLUDE_MAKE_DEPEND  += $($(OS_ERROR_LIB_NAME)_FILES)
endif

ifeq ($(OS_LIB_OUTPUT_BASEPATH),)
# if OS_LIB_OUTPUT_BASEPATH wasn't set or if it is empty, use the default
# we use override here in case someone does a "make OS_LIB_OUTPUT_BASEPATH= depend"
override OS_LIB_OUTPUT_BASEPATH = $(AutosarOS_LIB_PATH)\$(OS_CPU)_$(OS_TOOLCHAIN)
endif

DIRECTORIES_TO_CREATE += $(OS_LIB_OUTPUT_BASEPATH)

ifneq ($(BUILD_MODE),LIB)
DIRECTORIES_TO_CREATE += \
	$(OS_LIB_OUTPUT_BASEPATH)\$(OS_KERNEL_LIB_NAME) \
	$(OS_LIB_OUTPUT_BASEPATH)\$(OS_USER_LIB_NAME)
ifneq ($(OS_KERNEL_TYPE),MICROKERNEL)
DIRECTORIES_TO_CREATE += $(OS_LIB_OUTPUT_BASEPATH)\$(OS_ERROR_LIB_NAME)
endif
endif


$(OS_KERNEL_LIB_NAME)_LIB_OUTPUT_PATH = $(OS_LIB_OUTPUT_BASEPATH)
$(OS_USER_LIB_NAME)_LIB_OUTPUT_PATH = $(OS_LIB_OUTPUT_BASEPATH)
ifneq ($(OS_KERNEL_TYPE),MICROKERNEL)
$(OS_ERROR_LIB_NAME)_LIB_OUTPUT_PATH = $(OS_LIB_OUTPUT_BASEPATH)
endif

ifneq ($(BUILD_MODE),LIB)

define defineOSKernOutputPATH
$(1)_OBJ_OUTPUT_PATH = $(OS_LIB_OUTPUT_BASEPATH)\$(OS_KERNEL_LIB_NAME)
endef

define defineOSUserOutputPATH
$(1)_OBJ_OUTPUT_PATH = $(OS_LIB_OUTPUT_BASEPATH)\$(OS_USER_LIB_NAME)
endef

ifeq ($(OS_KERNEL_TYPE),MICROKERNEL)
define defineOSErrorOutputPATH
$(1)_OBJ_OUTPUT_PATH = $(OS_LIB_OUTPUT_BASEPATH)\$(OS_USER_LIB_NAME)
endef
else
define defineOSErrorOutputPATH
$(1)_OBJ_OUTPUT_PATH = $(OS_LIB_OUTPUT_BASEPATH)\$(OS_ERROR_LIB_NAME)
endef
endif

else

define defineOSKernOutputPATH
$(1)_OBJ_OUTPUT_PATH = $(PROJECT_ROOT)\output\obj
endef

define defineOSUserOutputPATH
$(1)_OBJ_OUTPUT_PATH = $(PROJECT_ROOT)\output\obj
endef

define defineOSErrorOutputPATH
$(1)_OBJ_OUTPUT_PATH = $(PROJECT_ROOT)\output\obj
endef

endif

#################################################################
# REGISTRY
SSC_PLUGINS          	    += AutosarOS

CC_INCLUDE_PATH += \
	$(AutosarOS_OUTPUT_PATH)\include \
	$(AutosarOS_CORE_PATH)\include

CPP_INCLUDE_PATH += \
	$(AutosarOS_OUTPUT_PATH)\include \
	$(AutosarOS_CORE_PATH)\include

ASM_INCLUDE_PATH += \
	$(AutosarOS_OUTPUT_PATH)\include \
	$(AutosarOS_CORE_PATH)\include \
	$(PROJECT_ROOT)\include


include $(AutosarOS_CORE_PATH)/make/$(OS_ARCH)/Os_defs_$(OS_ARCH).mak

#########################

$(foreach SRC,$(basename $(notdir $(subst \,/,$($(OS_KERNEL_LIB_NAME)_FILES)))),$(eval $(call defineOSKernOutputPATH,$(SRC))))
$(foreach SRC,$(basename $(notdir $(subst \,/,$($(OS_USER_LIB_NAME)_FILES)))),$(eval $(call defineOSUserOutputPATH,$(SRC))))
$(foreach SRC,$(basename $(notdir $(subst \,/,$($(OS_ERROR_LIB_NAME)_FILES)))),$(eval $(call defineOSErrorOutputPATH,$(SRC))))

### For Checker
Os_lib_FILES = $($(OS_USER_LIB_NAME)_FILES) $($(OS_KERNEL_LIB_NAME)_FILES)



# End of include guard
endif
#OS_DEFINITIONS_MAK_INCLUDED

#########################
# Editor settings: DO NOT DELETE
# vi:set ts=4:
