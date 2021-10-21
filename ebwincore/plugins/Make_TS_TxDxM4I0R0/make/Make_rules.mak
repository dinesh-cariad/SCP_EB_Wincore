#
# \file
#
# \brief AUTOSAR Make
#
# This file contains the implementation of the AUTOSAR
# module Make.
#
# \version 4.0.27
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2005 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


# ------------  rule order -----------------------------------------------------------
.PHONY: mak postbuild preprocess depend check_dups show_rules single_file single_lib single_lib_clean clean_all clean_global clean
ifeq (all run,$(filter all run,$(MAKECMDGOALS)))
run: all

endif

# ------------  input-sanitation & preprocessing  ---------------------------------------
PFLAGS			:= $(subst \,/,$(PFLAGS))
CC_OPT			:= $(subst \,/,$(CC_OPT))
CPP_OPT			:= $(subst \,/,$(CPP_OPT))
ASM_OPT			:= $(subst \,/,$(ASM_OPT))
LINK_OPT		:= $(subst \,/,$(LINK_OPT))

# sanitize user & plugin input paths - as late as possible:
# this evaluation fixes wildcards and prevents 'generate' and 'all' from running in the same instance, so must not be moved out of here
ASM_FILES_TO_BUILD	:= $(call uniq,$(subst \,/,$(ASM_FILES_TO_BUILD)))
CC_FILES_TO_BUILD	:= $(call uniq,$(subst \,/,$(CC_FILES_TO_BUILD)))
CC_INCLUDE_PATH		:= $(call uniq,$(subst \,/,$(CC_INCLUDE_PATH)))
LIB_INCLUDE_PATH	:= $(call uniq,$(subst \,/,$(LIB_INCLUDE_PATH)))
USER_INCLUDE_PATH	:= $(call uniq,$(subst \,/,$(USER_INCLUDE_PATH)))
LOCAL_INC_DIR		:= $(call uniq,$(subst \,/,$(LOCAL_INC_DIR)))
PROJECT_BINARY		:= $(subst \,/,$(PROJECT_BINARY))
# empty for WIN32X86 & redefined by WIN32X86/Compiler_defs.mak
ifneq (,$(findstring \,$(TOOLPATH_COMPILER)))
  TOOLPATH_COMPILER	:= $(subst \,/,$(TOOLPATH_COMPILER))
endif

$(eval $(foreach l,$(LIBRARIES_TO_BUILD),$(l)_FILES:=$(call uniq,$(subst \,/,$($(l)_FILES)))$(\n)))
# includes Testing's build rules if present:- may add paths / rewrite variables, but is trusted to be sanitized
-include $(PLUGINS_BASE)/Testing_$(Testing_VARIANT)/make/Testing_build.mak

# derived variables - computed from the abov input variables
BASENAMES		 = $(call uniq,$(basename $(notdir $(realpath $(CC_FILES_TO_BUILD) $(ASM_FILES_TO_BUILD)))))
OBJECTS			 = $(patsubst %,$(OBJ_DIR)/%.$(OBJ_FILE_SUFFIX),$(BASENAMES))
DEPENDENCIES		 = $(patsubst %,$(DEPEND_DIR)/%.$(DEP_FILE_SUFFIX),$(BASENAMES))
PREP_FILES		 = $(patsubst %,$(LIST_DIR)/%.$(PREP_FILE_SUFFIX),$(BASENAMES))
GEN_INC_DIR		 = $(addsuffix /include,$(GEN_OUTPUT_PATHS))
ALL_INC_DIR		 = $(call uniq,$(addprefix -I,$(realpath $(USER_INCLUDE_PATH) $(GEN_INC_DIR) $(filter-out $(GEN_INC_DIR), $(LOCAL_INC_DIR) $(CC_INCLUDE_PATH)))))
HDR_PATH		 = $(USER_INCLUDE_PATH) $(filter-out $(USER_INCLUDE_PATH),$(CC_INCLUDE_PATH))

# filter out libs where files are empty
LIBRARIES_TO_BUILD	:= $(sort $(foreach l, $(LIBRARIES_TO_BUILD),$(if $($(l)_FILES),$(l))))

# ------------  list of lib only objects  ----------------------------------------------------------------
# list all objects in lib files & add output dir to all lib files
LIB_OBJECTS		 = $(error use of DEPRECATED internal variable LIB_OBJECTS)
LIB_OBJECTS_OUT		 = $(patsubst %,$(OBJ_DIR)/%.$(OBJ_FILE_SUFFIX),$(sort $(foreach l,$(LIBRARIES_TO_BUILD),$(basename $(notdir $($(l)_FILES))))))
# get objects which are not in libraries
LINK_ONLY_OBJ		 = $(filter-out $(LIB_OBJECTS_OUT),$(OBJECTS))
# create list of all libs with output path
OUTPUT_PATH_LIBS	 = $(patsubst %,$(LIB_DIR)/%.$(LIB_FILE_SUFFIX),$(LIBRARIES_TO_BUILD))

# list all objects in postbuild libraries & add output dir to all lib files
POSTBUILD_OBJECTS	 = $(error use of DEPRECATED internal variable POSTBUILD_OBJECTS)
POSTBUILD_OBJECTS_OUT	 = $(patsubst %,$(OBJ_DIR)/%.$(OBJ_FILE_SUFFIX),$(foreach LIB_ITEM, $(LIBRARIES_PBCFG_TO_BUILD),$(basename $(notdir $($(LIB_ITEM)_FILES)))))

# define postbuild output file 
POSTBUILD_BINARY	 = $(basename $(PROJECT_BINARY)).bin

# ------------  main rules  ------------------------------------------------------------

# ALL_PREREQUISITES set from Make_defs.mak
all: $(ALL_PREREQUISITES)

# include the optional board make file here!
-include $(BOARD_PROJECT_PATH)/$(BOARD).mak
ifeq ($(MAKECMDGOALS),flat_src)
  ifneq (,$(IS_DOS_SHELL))
    include $(MAKE_ROOT)/make/flat_src.mak
  else
    $(error flat_src only supported for Windows)
  endif
endif

# check if post build files exist 
ifneq (,$(POSTBUILD_OBJECTS_OUT))
#$(POSTBUILD_OBJECTS_OUT): $(MAKE_INC_DEFAULT)

postbuild: $(POSTBUILD_BINARY)
	$(info postbuild: $(POSTBUILD_BINARY): $(POSTBUILD_OBJECTS_OUT))

else
postbuild:
	$(info WARNING: no postbuild files defined!)

endif

# ------------  rules for final binaries  ----------------------------------------------

# rule for invocation of the linker to build the executable
$(PROJECT_BINARY): $(OUTPUT_PATH_LIBS) $(LNK_INC) $(LINK_ONLY_OBJ) $(MAKE_CFG) $(LOC_FILE) # $(LING_PREP) $(LIB_TO_OBJ) $(OBJ_TO_CC)
	$(build-exe-file)

# rule for generating the postbuild binary
$(POSTBUILD_BINARY): $(POSTBUILD_OBJECTS_OUT)
	$(build-postbuild-file)

# ------------  list of lib only sources  ----------------------------------------------------------------

# list of all source and postbuild config libs which are project specific
SRC_LIBS_TO_BUILD		:= $(filter %_src,$(LIBRARIES_TO_BUILD))
PBCONFIG_LIBS_TO_BUILD		:= $(filter %_pbconfig,$(LIBRARIES_TO_BUILD))

# list of all static lib which can be shared between projects
STATIC_LIBS			:= $(filter-out $(SRC_LIBS_TO_BUILD) $(PBCONFIG_LIBS_TO_BUILD),$(LIBRARIES_TO_BUILD))

ifneq (,$(GLOBAL_LIB_ENABLE))
  # limit to static libs which do not yet exist in the shared directory
  STATIC_LIBS_TO_BUILD		:= $(filter-out $(basename $(notdir $(realpath $(patsubst %,$(GLOBAL_LIB_DIR)/%.$(LIB_FILE_SUFFIX),$(STATIC_LIBS))))),$(STATIC_LIBS))
else
  STATIC_LIBS_TO_BUILD		:= $(STATIC_LIBS)
endif
# list of all libs which needs to be build
TOTAL_LIBS_TO_BUILD		:= $(SRC_LIBS_TO_BUILD) $(PBCONFIG_LIBS_TO_BUILD) $(STATIC_LIBS_TO_BUILD)
OS_LIBS		 		:= $(OS_KERNEL_LIB_NAME) $(OS_USER_LIB_NAME) $(OS_ERROR_LIB_NAME) Os_src
NON_OS_LIBS			:= $(filter-out $(OS_LIBS), $(LIBRARIES_TO_BUILD))

# list of all source files which need to be built
LIB_SOURCES			:= $(foreach l,$(TOTAL_LIBS_TO_BUILD),$($(l)_FILES))
CC_FILES_TO_BUILD		+= $(LIB_SOURCES)
# create a list of unique paths and filter all duplicates from it
UNIQUE_SOURCES			:= $(call uniq,$(realpath $(CC_FILES_TO_BUILD) $(ASM_FILES_TO_BUILD)))
# At least for make files, we can keep paths relative to $(PROJECT_ROOT)  - lazy evaluation!
UNIQUE_SOURCES_REL= $(subst $(PROJECT_ROOT),$$(PROJECT_ROOT),$(subst $(GEN_OUTPUT_PATH),$$(GEN_OUTPUT_PATH),$(subst $(PLUGINS_BASE),$$(PLUGINS_BASE),$(UNIQUE_SOURCES))))

# avoid make restarts from generated make(-like) files
GEN_MAKEFILES			 = $(OBJ_TO_CC) $(LIB_TO_OBJ) $(OPT_MAP)\
				   $(MAKE_INC_DEFAULT) $(MAKE_INCPP) $(DEPEND_INC) $(ASM_INC) $(LNK_INC) $(LOC_FILE)\
				   $(MAKE_INFO_FILE)
#$(MAKE_CFG)#$(LING_PREP)
SOURCE_MAKEFILES		 = $(filter-out %.$(DEP_FILE_SUFFIX) $(GEN_MAKEFILES),$(MAKEFILE_LIST))

# shorten & abstract paths for *readability *file-size *movability purposes
compress_paths			 = $(subst $(PLUGINS_BASE),$$(PLUGINS_BASE),$(subst $(PROJECT_ROOT),$$(PROJECT_ROOT),$(1)))

# ------------  rules  ------------------------------------------------------

$(OUTPUT_PATH_LIBS):
ifeq (,$(GLOBAL_LIB_ENABLE))
	$(build-lib-file)
else
	$(if $(findstring $(basename $(@F)), $(SRC_LIBS_TO_BUILD) $(PBCONFIG_LIBS_TO_BUILD)),\
          $(build-lib-file),\
          $(if $(realpath $(GLOBAL_LIB_DIR)/$(@F)),\
            $(COPY) $(call fixCmdPaths,$(GLOBAL_LIB_DIR)/$(@F) $@) >$(DEVNULL),\
            $(build-lib-file)))
	$(if $(findstring $(basename $(@F)), $(STATIC_LIBS)),\
          $(if $(realpath $(GLOBAL_LIB_DIR)/$(@F)),\
            ,\
            $(COPY) $(call fixCmdPaths,$@ $(GLOBAL_LIB_DIR)/$(@F).tmp.$(PROJECT_NAME)) >$(DEVNULL) &&\
            $(call ifnotexist,$(GLOBAL_LIB_DIR)/$(@F),$(MOVE) $(call fixCmdPaths,$(GLOBAL_LIB_DIR)/$(@F).tmp.$(PROJECT_NAME) $(GLOBAL_LIB_DIR)/$(@F)) >$(DEVNULL))))
endif

# create all the files created by the make environment, same as the dependencies for the target PROJECT_BINARY - but that doesn't need to write/reread that information
mak: $(GEN_MAKEFILES)
	@echo === Makefiles created ===
# to avoid 'nothing to be done'

MAKE_INFO += \
	PROJECT=$(PROJECT) \
	TARGET=$(TARGET) \
	DERIVATE=$(DERIVATE)

$(MAKE_INFO_FILE): $(SOURCE_MAKEFILES)
	$(info Building $(@F))
	$(file >$@,$(addprefix $(\n),$(MAKE_INFO)))


# this should go to the resp. toolchain support in asc_Compiler:
.$(CC_FILE_SUFFIX).$(PREP_FILE_SUFFIX): #$(MAKE_INC_DEFAULT)
	$(build-prep-file)

.$(ASM_FILE_SUFFIX).$(OBJ_FILE_SUFFIX): #$(ASM_INC)
	$(build-s-file)

.$(CC_FILE_SUFFIX).$(OBJ_FILE_SUFFIX): #$(MAKE_INC_DEFAULT) $(OBJ_TO_CC)
	$(build-c-file)

.$(CPP_FILE_SUFFIX).$(OBJ_FILE_SUFFIX): #$(MAKE_INCPP)
	$(build-cpp-file)

#.$(PREP_FILE_SUFFIX).$(CPP_FILE_SUFFIX): $(build-prep-file)
#.$(PREP_FILE_SUFFIX).$(ASM_FILE_SUFFIX): $(build-prep-file)

_OBJ_TO_CC ?=\
	$(foreach SRC, $(filter $(CC_FILTER_LIST), $(UNIQUE_SOURCES_REL))\
		,$(\n)$$(OBJ_DIR)/$(notdir $(basename $(SRC)).$(OBJ_FILE_SUFFIX)) $$(DEPEND_DIR)/$(notdir $(basename $(SRC)).$(DEP_FILE_SUFFIX)): $(SRC) $(DEPEND_INC) $(MAKE_INC_DEFAULT); $$(build-c-file))\
	$(foreach SRC, $(filter $(ASM_FILTER_LIST), $(UNIQUE_SOURCES_REL))\
		,$(\n)$$(OBJ_DIR)/$(notdir $(basename $(SRC)).$(OBJ_FILE_SUFFIX)) $$(DEPEND_DIR)/$(notdir $(basename $(SRC)).$(DEP_FILE_SUFFIX)): $(SRC) $(DEPEND_INC) $(ASM_INC); $$(build-s-file))\
	$(foreach SRC, $(filter $(CPP_FILTER_LIST), $(UNIQUE_SOURCES_REL))\
		,$(\n)$$(OBJ_DIR)/$(notdir $(basename $(SRC)).$(OBJ_FILE_SUFFIX)) $$(DEPEND_DIR)/$(notdir $(basename $(SRC)).$(DEP_FILE_SUFFIX)): $(SRC) $(DEPEND_INC) $(MAKE_INCPP); $$(build-cpp-file))

$(OBJ_TO_CC): $(SOURCE_MAKEFILES) #$(LIB_TO_OBJ) $(MAKE_INC_DEFAULT) $(MAKE_INCPP) $(DEPEND_INC) $(ASM_INC) $(OPT_MAP)
	$(info Building $(@F) $(OS_GENERATED_APPL_FILES))
	$(file > $(OBJ_TO_CC),$(call compress_paths,$(_OBJ_TO_CC)))

_OPT_MAP ?= $(foreach LIB, $(LIBRARIES_TO_BUILD)\
		,$(if $($(LIB)_OPTMAP)\
			,$(foreach OBJ, $(notdir $(basename $($(LIB)_FILES)))\
				,$(\n)$(OBJ)_OPTMAP=$($(LIB)_OPTMAP)))) 

$(OPT_MAP): $(SOURCE_MAKEFILES)
	$(info Building $(@F))
	$(file > $(OPT_MAP),$(subst $(PLUGINS_BASE),$$(PLUGINS_BASE),$(subst $(PROJECT_ROOT),$$(PROJECT_ROOT),$(_OPT_MAP))))

_PREP_TO_CC ?= $(foreach SRC,$(filter $(CC_FILTER_LIST), $(UNIQUE_SOURCES_REL))\
			$(filter %.s $(ASM_FILTER_LIST), $(UNIQUE_SOURCES_REL))\
			$(filter $(CPP_FILTER_LIST), $(UNIQUE_SOURCES_REL))\
		,$(\n)$(LIST_DIR)/$(notdir $(basename $(SRC)).$(PREP_FILE_SUFFIX)): $(SRC) $(DEPEND_INC); $$(build-prep-file))

$(PREP_TO_CC): $(SOURCE_MAKEFILES) #$(DEPEND_INC)
	$(info Building $(@F))
	$(file > $(PREP_TO_CC),$(subst $(PLUGINS_BASE),$$(PLUGINS_BASE),$(subst $(LIST_DIR),$$(LIST_DIR),$(_PREP_TO_CC))))

$(SRC_LIST): $(SOURCE_MAKEFILES)
	$(info Building $(@F))
	$(file > $(SRC_LIST),$(addprefix $(\n),$(UNIQUE_SOURCES)))

$(HDR_LIST): $(SOURCE_MAKEFILES)
	$(info Building $(@F))
	$(file > $(HDR_LIST),$(addprefix $(\n),$(HDR_PATH)))

check_dups: $(SRC_LIST) $(HDR_LIST)
	$(EB_VERBOSE)$(PERL_EXE) $(CHECK_DUPS) --hdr=$(HDR_LIST) --src=$(SRC_LIST) && $(ECHO) === Check finished ===

ifeq (,$(build-prep-file-PROVIDED))
define build-prep-file
	$(info Preprocessing: $(notdir $<))
	$(EB_VERBOSE)$(CPP) -E @$(DEPEND_INC) $< > $(LIST_DIR)/$(notdir $(basename $@).$(PREP_FILE_SUFFIX))
endef
endif

preprocess: $(PREP_TO_CC) $(PREP_FILES)

# ------------  make temp and options files  -------------------------------------------

# Note: as the options specified in CC_OPT and CPP_OPT might contain blanks,
# the make function foreach cannot be used as it would split an option with blank in between
# resulting in an invalid options file.
# As writing all options into a single line might create a line too large for processing,
# line breaks are inserted before the options -D, -W, -X, -K, and -- using make function subst
_DEPEND_INC ?=\
  $(subst $(BLANK)-D,$(\n)-D,$(filter -D%, $(CC_OPT)) $(cppGetPreProcessorDefines))\
  $(subst $(BLANK)-I,$(\n)-I, $(ALL_INC_DIR))

$(DEPEND_INC): $(SOURCE_MAKEFILES)
	$(info Building $(@F))
	$(file >$@,-D_EMPTY$(\n)$(_DEPEND_INC))

_MAKE_INC_DEFAULT ?=\
  $(subst $(BLANK)-D,$(\n)-D,$(subst $(BLANK)-W,$(\n)-W,$(subst $(BLANK)-X,$(\n)-X,$(subst $(BLANK)-K,$(\n)-K,$(subst $(BLANK)--,$(\n)--,$(CC_OPT))))) $(call cppGetPreProcessorDefines))\
  $(subst $(BLANK)-I,$(\n)-I, $(ALL_INC_DIR))

$(MAKE_INC_DEFAULT): $(SOURCE_MAKEFILES)
	$(info Building $(@F))
	$(file >$@,-D_EMPTY$(\n)$(_MAKE_INC_DEFAULT))

_MAKE_INCPP ?=\
  $(subst $(BLANK)-D,$(\n)-D,$(subst $(BLANK)-W,$(\n)-W,$(subst $(BLANK)-X,$(\n)-X,$(subst $(BLANK)-K,$(\n)-K,$(subst $(BLANK)--,$(\n)--,$(CPP_OPT))))) $(call cppGetPreProcessorDefines))\
  $(subst $(BLANK)-I,$(\n)-I, $(ALL_INC_DIR))

$(MAKE_INCPP): $(SOURCE_MAKEFILES)
	$(info Building $(@F))
	$(file >$@,-D_EMPTY$(\n)$(_MAKE_INCPP))

_ASM_INC ?=\
  $(ASM_OPT) $(PFLAGS)\
  $(subst $(BLANK)-D,$(\n)-D, $(call cppGetPreProcessorDefines))\
  $(subst $(BLANK)-I,$(\n)-I, $(ALL_INC_DIR))

$(ASM_INC): $(SOURCE_MAKEFILES)
	$(info Building $(@F))
	$(file >$@,-D_EMPTY$(\n)$(_ASM_INC))

_LNK_INC ?=\
  $(subst $(BLANK)-L,$(\n)-L,$(LINK_OPT) $(addprefix -L,$(LIB_INCLUDE_PATH)))\
  $(addprefix $(\n),$(LINK_ONLY_OBJ) $(OBJECTS_LINK_ONLY) $(OUTPUT_PATH_LIBS))

$(LNK_INC): $(SOURCE_MAKEFILES)
ifeq (,$(build-link-inc-file-PROVIDED))
	$(info Writing $@)
	$(file >$@,$(_LNK_INC))
else
	$(build-link-inc-file)
endif

_LIB_TO_OBJ ?= $(foreach LIB, $(TOTAL_LIBS_TO_BUILD)\
		,$(\n)$$(LIB_DIR)/$(LIB).$(LIB_FILE_SUFFIX):\
		$(patsubst %,\$(\n)  $$(OBJ_DIR)/%.$(OBJ_FILE_SUFFIX),$(basename $(notdir $($(LIB)_FILES))))$(\n))

$(LIB_TO_OBJ):
	$(info Building $(@F))
	$(file >$@,$(call compress_paths,$(_LIB_TO_OBJ)))

# debug output support
$(call log_verbose,variables dump:$(foreach v,\
 MODULE PLUGINS_BASE \
 CC_INCLUDE_PATH CC_FILES_TO_BUILD \
 OBJECTS_LINK_ONLY OUTPUT_PATH_LIBS \
 LINK_ONLY_OBJ LIBRARIES_LINK_ONLY LIB_INCLUDE_PATH \
 GEN_INC_DIR LOCAL_INC_DIR ALL_INC_DIR \
 _MAKE_INC_DEFAULT _LNK_INC \
 ,$(\n)# $(v)=$(\t)$($(v)))$(\n))

# include the automatically generated prerequisites if target is not clean
# here we decide whether it is necessary to build the .d files (DEPENDENCIES)
# the all target is also allowed to work here
ifneq (,$(filter all%,$(MAKECMDGOALS)))
  # the postbuild target does not need the .d files
  ifneq ($(TS_BUILD_POST_BUILD_BINARY),TRUE)
    # needs 2 newlines as broken/old preprocessors could end with a line-continuation-backslash
    $(eval $(foreach d,$(wildcard $(DEPENDENCIES)),$(file <$(d))$(\n)$(\n)))
  endif
endif
ifneq (,$(filter all% postbuild,$(MAKECMDGOALS)))
  # we don't need to write & reread the makefiles; the include files are prerequisites
  # $(OBJ_TO_CC) $(LIB_TO_OBJ) $(MAKE_INC_DEFAULT) $(MAKE_INCPP) $(DEPEND_INC) $(ASM_INC) $(OPT_MAP)
  $(eval $(_OBJ_TO_CC) $(_LIB_TO_OBJ) $(_OPT_MAP))
endif
# the preprocess target does not need the .d or .o files
ifeq ($(MAKECMDGOALS),preprocess)
  $(eval  $(_PREP_TO_CC))
endif

depend:
	$(info obsolete target: depend)

ifneq (,$(filter single_file,$(MAKECMDGOALS)))
  SF_BASENAME = $(basename $(notdir $(SF)))
  ifeq (,$(findstring $(SF_BASENAME),$(BASENAMES)))
    $(error unkown filename in SF: "$(SF)")
  endif
  SF_OBJECT = $(patsubst %,$(OBJ_DIR)/%.$(OBJ_FILE_SUFFIX),$(SF_BASENAME))
  SF_DEPENDENCIES = $(patsubst %,$(DEPEND_DIR)/%.$(DEP_FILE_SUFFIX),$(SF_BASENAME))
  # needs 2 newlines as broken/old preprocessors could end with a line-continuation-backslash
  $(eval $(foreach d,$(wildcard $(SF_DEPENDENCIES)),$(file <$(d))$(\n)$(\n))$(_OBJ_TO_CC))

# rule for building the file
single_file: $(SF_OBJECT)
# to avoid 'nothing to be done'
	@echo building obj: $(SF_BASENAME).$(OBJ_FILE_SUFFIX)

endif

# ------------  if single lib is built include only lib prerequisites  ---------------------

# common definitions
ifneq (,$(filter single_lib%,$(MAKECMDGOALS)))

  SINGLE_LIB_FILE_NAME		 = $(LIB_DIR)/$(SL).$(LIB_FILE_SUFFIX)
  SINGLE_LIB_BASENAMES		 = $(basename $(notdir $($(SL)_FILES)))
  SINGLE_LIB_OBJECTS		 = $(patsubst %,$(OBJ_DIR)/%.$(OBJ_FILE_SUFFIX),$(SINGLE_LIB_BASENAMES))
  SINGLE_LIB_DEPENDENCIES	 = $(patsubst %,$(DEPEND_DIR)/%.$(DEP_FILE_SUFFIX),$(SINGLE_LIB_BASENAMES))

  ifeq ($(MAKECMDGOALS),single_lib)

    ifeq (,$(findstring $(SL),$(LIBRARIES_TO_BUILD)))
      $(error Library SL "$(SL)" not defined in LIBRARIES_TO_BUILD: $(LIBRARIES_TO_BUILD))
    endif
  # needs 2 newlines as broken/old preprocessors could end with a line-continuation-backslash
  $(eval $(foreach d,$(wildcard $(SINGLE_LIB_DEPENDENCIES)),$(file <$(d))$(\n)$(\n))$(_OBJ_TO_CC)$(\n)$(\n)$(_LIB_TO_OBJ))

single_lib: $(SINGLE_LIB_FILE_NAME)

  endif

  ifeq ($(MAKECMDGOALS),single_lib_clean)
    ifeq (,$(findstring $(SL),$(LIBRARIES_TO_BUILD)))
      $(error Library SL "$(SL)" not defined in LIBRARIES_TO_BUILD: $(LIBRARIES_TO_BUILD))
    endif

single_lib_clean: $(SINGLE_LIB_FILE_NAME)
	$(call SC_CMD_LISTPERFORM,SC_CMD_AR_HELPER,@cd $(PROJECT_ROOT) && $(RM) ,\
		10,11,$(subst $(PROJECT_ROOT)/,,$(SINGLE_LIB_OBJECTS) $(SINGLE_LIB_FILE_NAME) $(SINGLE_LIB_DEPENDENCIES)))
	$(info ========================================)
	$(info . $(notdir $(SINGLE_LIB_FILE_NAME)) clean SUCCESSFUL)
	$(info ========================================)

  endif
endif
