# Ioc_filelists.make - lists of files that go into the Ioc build
#
# This file just contains lists of files that go into various parts of the
# build.
#
# (c) Elektrobit Automotive GmbH
#
# $Id: Ioc_filelists.mak 1923 2017-02-28 16:22:14Z mist9353 $

#===============================================================================
# Lists of base filenames for the kernel library
#===============================================================================
# IOC_LIBSRCKERN_KLIB_BASELIST is the list of all files in the plugin/lib_src/kernel directory that
# go into the kernel library.
# The files are listed without prefix or extension.
IOC_LIBSRCKERN_KLIB_BASELIST +=	init
IOC_LIBSRCKERN_KLIB_BASELIST +=	confighandler_common
IOC_LIBSRCKERN_KLIB_BASELIST +=	confighandler_queued
IOC_LIBSRCKERN_KLIB_BASELIST +=	confighandler_unqueued
ifeq ($(IOC_LCFG_KERNEL_TYPE),IOC_MICROKERNEL)
IOC_LIBSRCKERN_KLIB_BASELIST +=	mk_addoncontrol
IOC_LIBSRCKERN_KLIB_BASELIST +=	mk_addoninit
IOC_LIBSRCKERN_KLIB_BASELIST +=	mk_getapplicationid_kernel
IOC_LIBSRCKERN_MULTICORELIB_BASELIST += mk_interlock_osspinlock
IOC_LIBSRCKERN_KLIB_BASELIST +=	mk_getcoreid_kernel
IOC_LIBSRCKERN_KLIB_BASELIST += mk_getappcoreid
else
IOC_LIBSRCKERN_MULTICORELIB_BASELIST += ac_interlock_osspinlock
IOC_LIBSRCKERN_KLIB_BASELIST +=	ac_sysextdemux
IOC_LIBSRCKERN_KLIB_BASELIST +=	ac_intralock_allint
IOC_LIBSRCKERN_KLIB_BASELIST +=	ac_getapplicationid_kernel
endif

#===============================================================================
# Lists of base filenames for the user library
#===============================================================================
# IOC_LIBSRCUSER_ULIB_BASELIST is the list of all files in the plugin/lib_src/user directory
# that go into the User library.
# The files are listed without prefix or extension.
IOC_LIBSRCUSER_ULIB_BASELIST += confighandler_queued
IOC_LIBSRCUSER_ULIB_BASELIST += confighandler_unqueued

#===============================================================================
# Lists of base filenames for the mixed library
#===============================================================================
# IOC_LIBSRCUSER_ULIB_BASELIST is the list of all files in the plugin/lib_src/mixed directory
# that go into the Mixed library.
# The files are listed without prefix or extension.
IOC_LIBSRCMIXED_MLIB_BASELIST += bufferhandler_basic
IOC_LIBSRCMIXED_MLIB_BASELIST += bufferhandler_extended
IOC_LIBSRCMIXED_MLIB_BASELIST += confighandler_common
IOC_LIBSRCMIXED_MLIB_BASELIST += lockhandler_lock
IOC_LIBSRCMIXED_MLIB_BASELIST += queuehandler_basic
IOC_LIBSRCMIXED_MLIB_BASELIST += queuehandler_extended
IOC_LIBSRCMIXED_MLIB_BASELIST += extentry_common
IOC_LIBSRCMIXED_MLIB_BASELIST += memcpy

#===============================================================================
# Lists of architecture base filenames for the user library
#===============================================================================
# IOC_LIBSRCCPUFAMILY_ULIB_BASELIST_x is the list of all files in the plugin/lib_src/$CPUFAMILY directory
# that go into the User library.
# The files are listed without prefix or extension. These files go into the user library.
#
# The basenames of these files are the same for all architectures because they are the stub
# functions that make the system calls.
# It is not known here whether these are C or assembly-language files, so the
# architecture file list must make these assignments with _x replaced by _S or _C as appropriate
#IOC_LIBSRCCPUFAMILY_ULIB_BASELIST_x	+=	$(addprefix usr, $(IOC_SYSTEMCALL_BASELIST))
#IOC_LIBSRCCPUFAMILY_ULIB_BASELIST_x	+=	$(addprefix usr, $(IOC_OSCALL_BASELIST))

#===============================================================================
# Lists of base filenames for the error library
#===============================================================================

#===============================================================================
# Lists of base filenames for the binary build
#===============================================================================
# IOC_SRC_CFG_BASELIST is the list of all configuration-dependent files in the plugin/src directory,
# without prefix or extension. These files go straight into the system build
IOC_SRC_CFG_BASELIST +=	srcconfiguration
ifeq ($(IOC_LCFG_KERNEL_TYPE),IOC_MICROKERNEL)
IOC_SRC_CFG_BASELIST +=	mk_configuration
endif

# IOC_SRC_USER_BASELIST is the list of all configuration-dependent files in the plugin/src/user directory.
IOC_SRC_USER_BASELIST += intralock_allint
IOC_SRC_USER_BASELIST += getapplicationid_user
ifeq ($(IOC_LCFG_KERNEL_TYPE),IOC_AUTOCOREOS)
IOC_SRC_USER_BASELIST += ac_confighandler_syscall
endif

# IOC_SRC_KERNEL_BASELIST is the list of all configuration-dependent files in the plugin/src/kernel directory.
ifeq ($(IOC_LCFG_KERNEL_TYPE),IOC_AUTOCOREOS)
IOC_SRC_KERNEL_BASELIST += ac_getcoreid_kernel
IOC_SRC_KERNEL_BASELIST += ac_getappcoreid
endif

# IOC_GEN_CFG_BASELIST is the list of all generated files
# without prefix or extension. These files go straight into the system build
IOC_GEN_CFG_BASELIST +=	configuration
