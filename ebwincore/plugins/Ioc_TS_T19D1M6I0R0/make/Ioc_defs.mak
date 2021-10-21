# Ioc_defs.mak
#
# This makefile includes definitions used by other modules.
#
# (c) Elektrobit Automotive GmbH
#
# $Id: Ioc_defs.mak 2169 2017-06-30 13:06:58Z mist9353 $

#################################################################
# DEFINITIONS

Ioc_CORE_PATH       := $(SSC_ROOT)\Ioc_$(Ioc_VARIANT)

include $(Ioc_CORE_PATH)\make\Ioc_make_translation_layer.mak

Ioc_INC_PATH        := $(Ioc_CORE_PATH)\include
Ioc_SRC_PATH        := $(Ioc_CORE_PATH)\src
Ioc_SRC_USER_PATH   := $(Ioc_SRC_PATH)\user
Ioc_SRC_KERNEL_PATH := $(Ioc_SRC_PATH)\kernel

Ioc_LIBSRC_PATH     := $(Ioc_CORE_PATH)\lib_src
Ioc_LIBSRCKERN_PATH := $(Ioc_LIBSRC_PATH)\kernel
Ioc_LIBSRCUSER_PATH := $(Ioc_LIBSRC_PATH)\user
Ioc_LIBSRCMIXED_PATH := $(Ioc_LIBSRC_PATH)\mixed

Ioc_MAKE_PATH       := $(Ioc_CORE_PATH)\make
Ioc_MAKECPUFAMILY_PATH   := $(Ioc_MAKE_PATH)\$(IOC_CPUFAMILY)
Ioc_MAKECPU_PATH    := $(Ioc_MAKECPUFAMILY_PATH)\$(IOC_CPU)

Ioc_LIB_BASE        := $(Ioc_CORE_PATH)\lib
Ioc_LIBCPUFAMILY_PATH    := $(Ioc_LIB_BASE)\$(IOC_CPUFAMILY)
Ioc_LIBCPU_PATH     := $(Ioc_LIBCPUFAMILY_PATH)\$(IOC_CPU)
Ioc_LIB_PATH        := $(Ioc_LIBCPU_PATH)\$(IOC_TOOL)

Ioc_OUTPUT_PATH     ?= $(AUTOSAR_BASE_OUTPUT_PATH)

Ioc_OUTPUT_INC      := $(Ioc_OUTPUT_PATH)\include
Ioc_OUTPUT_SRC      := $(Ioc_OUTPUT_PATH)\src
Ioc_OUTPUT_MAKE     := $(Ioc_OUTPUT_PATH)\make

-include $(Ioc_OUTPUT_MAKE)\Ioc_gen_filelist.mak

Ioc_SrcFile_prefix     = Ioc_
Ioc_SrcUserFile_prefix = Ioc_u_
Ioc_SrcKernelFile_prefix = Ioc_k_
Ioc_File_prefix        = Ioc_
Ioc_Arch_prefix        = $(Ioc_File_prefix)$(IOC_CPUFAMILY)_

TRESOS_GEN_FILES    += $(Ioc_GEN_FILES)

#################################################################
# Library suffix (IOC_LIB_SUFFIX)
include $(Ioc_CORE_PATH)\make\Ioc_libsuffix.mak

#################################################################
# REGISTRY
SSC_PLUGINS               += Ioc

# Include the feature selection file, to know which files are required.
IOC_FEATURELIST_MAKEDIR := $(Ioc_MAKE_PATH)
include $(IOC_FEATURELIST_MAKEDIR)/Ioc_featureselection.mak

# OS_N_CORES is involved in selecting multi-core files.
# On single-core processors it will always be one, so set it, if it isn't set, yet.
ifeq ($(IOC_FEATURE_Multicore),disabled)
OS_N_CORES ?= 1
endif

# Include the file lists. Derivative list is only included if present.
-include $(Ioc_MAKECPU_PATH)\Ioc_filelists_$(IOC_CPU).mak
include $(Ioc_MAKECPUFAMILY_PATH)\Ioc_filelists_$(IOC_CPUFAMILY).mak
include $(Ioc_MAKE_PATH)\Ioc_filelists.mak

# Now suck in the architecture- and derivative-specific make definitions
# needed at this early position for possible library build flags
-include $(Ioc_MAKECPUFAMILY_PATH)\Ioc_defs_$(IOC_CPUFAMILY).mak
-include $(Ioc_MAKECPU_PATH)\Ioc_defs_$(IOC_CPUFAMILY)_$(IOC_CPU).mak

ifneq ($(IOC_CPUFAMILY_LIB_FLAGS),)
IOC_CPUFAMILY_LIB_FLAG_SUFFIX=_$(IOC_CPUFAMILY_LIB_FLAGS)
endif

# Names of the libraries
IOC_KERNEL_LIB_NAME    = Ioc_libIocKern_n$(IOC_CPUFAMILY_LIB_FLAG_SUFFIX)$(IOC_LIB_SUFFIX)
IOC_USER_LIB_NAME      = Ioc_libIocUser_n$(IOC_CPUFAMILY_LIB_FLAG_SUFFIX)$(IOC_LIB_SUFFIX)
IOC_MIXED_LIB_NAME     = Ioc_libIocMixed_n$(IOC_CPUFAMILY_LIB_FLAG_SUFFIX)$(IOC_LIB_SUFFIX)
IOC_MULTICORE_LIB_NAME = Ioc_libIocMultiCore_n$(IOC_CPUFAMILY_LIB_FLAG_SUFFIX)$(IOC_LIB_SUFFIX)

# Now we can construct all the lists of files that asc_Make needs.
$(IOC_KERNEL_LIB_NAME)_FILES += \
	$(addprefix $(Ioc_LIBSRCKERN_PATH)\$(Ioc_File_prefix)k_, \
		$(addsuffix .$(CC_FILE_SUFFIX), \
			$(IOC_LIBSRCKERN_KLIB_BASELIST)))

$(IOC_USER_LIB_NAME)_FILES += \
	$(addprefix $(Ioc_LIBSRCUSER_PATH)\$(Ioc_File_prefix)u_, \
		$(addsuffix .$(CC_FILE_SUFFIX), \
			$(IOC_LIBSRCUSER_ULIB_BASELIST)))

$(IOC_MIXED_LIB_NAME)_FILES += \
	$(addprefix $(Ioc_LIBSRCMIXED_PATH)\$(Ioc_File_prefix)m_, \
		$(addsuffix .$(CC_FILE_SUFFIX), \
			$(IOC_LIBSRCMIXED_MLIB_BASELIST)))

ifneq ($(OS_N_CORES),1)
$(IOC_MULTICORE_LIB_NAME)_FILES += \
	$(addprefix $(Ioc_LIBSRCKERN_PATH)\$(Ioc_File_prefix)k_, \
		$(addsuffix .$(CC_FILE_SUFFIX), \
			$(IOC_LIBSRCKERN_MULTICORELIB_BASELIST)))

$(IOC_MULTICORE_LIB_NAME)_FILES += \
	$(addprefix $(Ioc_LIBSRCUSER_PATH)\$(Ioc_File_prefix)u_, \
		$(addsuffix .$(CC_FILE_SUFFIX), \
			$(IOC_LIBSRCUSER_MULTICORELIB_BASELIST)))
endif

# generated files
IOC_SRC_FILES += $(Ioc_GEN_FILES)

IOC_SRC_FILES += \
	$(addprefix $(Ioc_SRC_PATH)\$(Ioc_SrcFile_prefix), \
		$(addsuffix .$(CC_FILE_SUFFIX), \
			$(IOC_SRC_CFG_BASELIST)))

IOC_SRC_FILES += \
	$(addprefix $(Ioc_SRC_USER_PATH)\$(Ioc_SrcUserFile_prefix), \
		$(addsuffix .$(CC_FILE_SUFFIX), \
			$(IOC_SRC_USER_BASELIST)))

IOC_SRC_FILES += \
	$(addprefix $(Ioc_SRC_KERNEL_PATH)\$(Ioc_SrcKernelFile_prefix), \
		$(addsuffix .$(CC_FILE_SUFFIX), \
			$(IOC_SRC_KERNEL_BASELIST)))

IOC_SRC_FILES += \
	$(addprefix $(Ioc_OUTPUT_SRC)\$(Ioc_SrcFile_prefix), \
		$(addsuffix .$(CC_FILE_SUFFIX), \
			$(IOC_GEN_CFG_BASELIST)))
			
DIRECTORIES_TO_CREATE += $(Ioc_LIB_PATH)

ifneq ($(BUILD_MODE),LIB)
DIRECTORIES_TO_CREATE += $(Ioc_LIB_PATH)\$(IOC_KERNEL_LIB_NAME)
DIRECTORIES_TO_CREATE += $(Ioc_LIB_PATH)\$(IOC_USER_LIB_NAME)
DIRECTORIES_TO_CREATE += $(Ioc_LIB_PATH)\$(IOC_MIXED_LIB_NAME)
DIRECTORIES_TO_CREATE += $(Ioc_LIB_PATH)\$(IOC_MULTICORE_LIB_NAME)
endif

$(IOC_KERNEL_LIB_NAME)_LIB_OUTPUT_PATH = $(Ioc_LIB_PATH)
$(IOC_USER_LIB_NAME)_LIB_OUTPUT_PATH = $(Ioc_LIB_PATH)
$(IOC_MIXED_LIB_NAME)_LIB_OUTPUT_PATH = $(Ioc_LIB_PATH)
$(IOC_MULTICORE_LIB_NAME)_LIB_OUTPUT_PATH = $(Ioc_LIB_PATH)

# set output path of object and library files
ifneq ($(BUILD_MODE),LIB)
define defineIocKernOutputPATH
$(1)_OBJ_OUTPUT_PATH = $(Ioc_LIB_PATH)\$(IOC_KERNEL_LIB_NAME)
endef
define defineIocUserOutputPATH
$(1)_OBJ_OUTPUT_PATH = $(Ioc_LIB_PATH)\$(IOC_USER_LIB_NAME)
endef
define defineIocMixedOutputPATH
$(1)_OBJ_OUTPUT_PATH = $(Ioc_LIB_PATH)\$(IOC_MIXED_LIB_NAME)
endef
define defineIocMultiCoreOutputPATH
$(1)_OBJ_OUTPUT_PATH = $(Ioc_LIB_PATH)\$(IOC_MULTICORE_LIB_NAME)
endef

else # $(BUILD_MODE) == LIB

define defineIocKernOutputPATH
$(1)_OBJ_OUTPUT_PATH = $(PROJECT_ROOT)\output\obj
endef
define defineIocUserOutputPATH
$(1)_OBJ_OUTPUT_PATH = $(PROJECT_ROOT)\output\obj
endef
define defineIocMixedOutputPATH
$(1)_OBJ_OUTPUT_PATH = $(PROJECT_ROOT)\output\obj
endef
define defineIocMultiCoreOutputPATH
$(1)_OBJ_OUTPUT_PATH = $(PROJECT_ROOT)\output\obj
endef
endif # $(BUILD_MODE) != LIB

$(foreach SRC,$(basename $(notdir $(subst \,/,$($(IOC_KERNEL_LIB_NAME)_FILES)))),$(eval $(call defineIocKernOutputPATH,$(SRC))))
$(foreach SRC,$(basename $(notdir $(subst \,/,$($(IOC_USER_LIB_NAME)_FILES)))),$(eval $(call defineIocUserOutputPATH,$(SRC))))
$(foreach SRC,$(basename $(notdir $(subst \,/,$($(IOC_MIXED_LIB_NAME)_FILES)))),$(eval $(call defineIocMixedOutputPATH,$(SRC))))
$(foreach SRC,$(basename $(notdir $(subst \,/,$($(IOC_MULTICORE_LIB_NAME)_FILES)))),$(eval $(call defineIocMultiCoreOutputPATH,$(SRC))))

# Standard include paths
CC_INCLUDE_PATH += $(Ioc_OUTPUT_INC)
CC_INCLUDE_PATH += $(Ioc_INC_PATH)

CPP_INCLUDE_PATH += $(Ioc_OUTPUT_INC)
CPP_INCLUDE_PATH += $(Ioc_INC_PATH)

ASM_INCLUDE_PATH += $(Ioc_OUTPUT_INC)
ASM_INCLUDE_PATH += $(Ioc_INC_PATH)
ASM_INCLUDE_PATH += $(PROJECT_ROOT)\include
