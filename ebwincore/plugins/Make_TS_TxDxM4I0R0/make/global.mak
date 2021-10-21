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


ifndef (_MAKE_DEFS)
  include $(subst global.mak,Make_defs.mak,$(filter %/global.mak %\global.mak,$(MAKEFILE_LIST)))
endif

# just a little top-level magic to simplify things downstream
# simulation-of-simplicity (SOS): fold identical cases 'make' & 'make all'
# also do the right thing for TS_BUILD_POST_BUILD_BINARY
ifeq (,$(MAKECMDGOALS))
  override MAKECMDGOALS:=$(if $(filter TRUE,$(TS_BUILD_POST_BUILD_BINARY)),postbuild,all)
endif

$(call log_verbose,$(SHELL) $(SHELLFLAGS) MAKE$(MAKE_VERSION) -$(MAKEFLAGS) $(MAKECMDGOALS) {$(MAKEFILE_LIST)})

# ------------ directory structure ------------------------------------------------------
# don't create directories for cleaning & depend, but consider multiple make targets like "make depend all"
ifneq (,$(filter-out depend clean% $(MAKE_CLEAN_RULES),$(MAKECMDGOALS)))
  # check which directories need creation
  # we use gnu make -p (even on DOS), so concatenate them to one path that's efficiently creating the whole tree
  dirlist := $(addprefix $(OUTPUT_DIR)/,$(subst $(BLANK),/../,$(filter-out $(notdir $(wildcard $(OUTPUT_DIR)/*)),bin depend generated lib list make obj temp)))
  ifneq (,$(GLOBAL_LIB_ENABLE))
    ifeq (,$(wildcard $(GLOBAL_LIB_DIR)))
        dirlist += $(GLOBAL_LIB_DIR)
    endif
  endif
  ifneq (,$(dirlist))
    # at most 2 full paths in the list
    $(call log_verbose,creating directories $(dirlist))
    SHELLCOMMAND:=$(shell $(MKDIR) $(dirlist))
  endif
endif # all targets, except clean* & depend

# ------------  first rule as default, before we include any other makefiles  -----------
.PHONY: postbuild all run test debug 
# declare 'all' as the default rule, if no make target was specified on the command line
.DEFAULT: all
all:

# ------------  plugins  ----------------------------------------------------------------
# include all generated makefiles if they exist
GEN_MAKE_FILES = $(wildcard $(addsuffix /make/*.$(MAK_FILE_SUFFIX),$(GEN_OUTPUT_PATHS)))

# only do the heavy lifting, if there are make targets left after ignoring all clean & help targets
ifneq (,$(filter-out show_rules clean% $(MAKE_CLEAN_RULES),$(MAKECMDGOALS)))
  ifneq (,$(filter-out generate% %generate depend,$(MAKECMDGOALS)))
    ifeq (,$(strip $(GEN_OUTPUT_PATH)))
      $(error No GEN_OUTPUT_PATH defined! Run make generate first)
    endif
    ifeq (,$(findstring Make$(CFG_FILE_SUFFIX),$(GEN_MAKE_FILES)))
      ifeq (,$(findstring tresos2_call_generator,$(MAKECMDGOALS)))
        # if 'false' or empty
        ifeq (,$(filter-out false,$(USE_LEGACY_MAKECFG)))
          $(error $(MAKECMDGOALS) : Make$(CFG_FILE_SUFFIX) not found! Run make generate first)
        endif
      endif
    endif
  endif # not depend or *generate*

  ifneq (,$(GEN_MAKE_FILES))
    include $(GEN_MAKE_FILES)
  endif
  # filter out Make & Tresos2, they are special infrastructure
  PROJECT_MODULES_FILTERED := $(sort $(filter-out Make tresos2, $(SOFTWARE_MODULES)))
  ifneq (,$(PROJECT_MODULES_FILTERED))

    # concatenate Module names (e.g. CRC) and TS-Strings (e.g. TS_TxDxM5I1R0) to a single string (e.g. CRC_TS_TxDxM5I1R0)
    FULL_TS_STRINGS = $(foreach m, $(PROJECT_MODULES_FILTERED), $(m)_$($(m)_VARIANT))
    # concatenate FULL_TS_STRINGS (e.g. CRC_TS_TxDxM5I1R0) and PLUGINS_BASE (e.g. S:/PROJECTSPACE/eclipse/plugins) to S:/PROJECTSPACE/eclipse/plugins/CRC_TS_TxDxM5I1R0
    PLUGIN_DIRS = $(subst \,/,$(foreach m, $(PROJECT_MODULES_FILTERED), $(if $($(m)_CORE_PATH), $($(m)_CORE_PATH), $(PLUGINS_BASE)/$(m)_$($(m)_VARIANT))))
    # generate list of make dirs (e.g. S:/PROJECTSPACE/eclipse/plugins/Resource_TS_T2D17M0I9R0/make)
    # use wildcard function to generate a list of all plugin make files
    PLUGIN_MAKES = $(wildcard	$(addsuffix /make/*$(DEFS_FILE_SUFFIX), $(PLUGIN_DIRS)) \
				$(addsuffix /make/*$(RULES_FILE_SUFFIX), $(PLUGIN_DIRS)) \
				$(addsuffix /make/*$(CHECK_FILE_SUFFIX), $(PLUGIN_DIRS)))
    $(call log_verbose,including plugin makefiles: $(subst $(PLUGINS_BASE),$${PLUGIN_BASE},$(PLUGIN_MAKES))$(\n))
    $(eval $(foreach f,$(PLUGIN_MAKES),$(file <$(f))$(\n)))
  endif # ifneq (,$(PROJECT_MODULES_FILTERED))
endif # ifneq (,$(filter-out show_rules clean% $(MAKE_CLEAN_RULES),$(MAKECMDGOALS)))

GENERATE:=$(filter generate% %generate,$(MAKECMDGOALS))
BUILD:=$(strip $(filter-out $(GENERATE) show_rules clean% %clean $(MAKE_CLEAN_RULES),$(MAKECMDGOALS)) $(filter single_lib%,$(MAKECMDGOALS)))
RUN:=$(filter run% debug%,$(BUILD))
BUILD:=$(filter-out $(RUN),$(BUILD))
$(call log_verbose,global.mak GENERATE=$(GENERATE)     BUILD=$(BUILD)     RUN=$(RUN))

ifneq (,$(GENERATE))
  # tresos studio dispatch
  # handle generate & tresos studio targets, consuming MAKE_GENERATE_RULES
  # includes Testing_generate to allow MAKE_GENERATE_RULES updates & other hooks for testing
  include $(MAKE_ROOT)/make/tresos2_rules.mak
endif
GENERATE:=
ifneq (,$(BUILD)$(RUN))
  # handle all target
  # includes Testing_build to allow all target updates & other hooks for testing
  include $(subst global.mak,Make_rules.mak,$(filter %/global.mak %\global.mak,$(MAKEFILE_LIST)))
else
  # allow hooks for testing
  -include $(PLUGINS_BASE)/Testing_$(Testing_VARIANT)/make/Testing_build.mak
endif

# list of directories for clean (all except generated and flat_src)
clean_dirs := depend lib list make obj temp
ifeq (,$(CLEAN_KEEP_LOGS))
  clean_dirs += test
endif
ifeq (,$(CLEAN_KEEP_BIN))
  clean_dirs += bin
endif

clean:
	-$(EB_VERBOSE)cd $(call fixCmdPaths,$(OUTPUT_DIR)) && $(RMTREE) $(clean_dirs)

# clean_all just deletes the generated and output folder
clean_all:
	-$(EB_VERBOSE)$(RMTREE) $(GENERATED_DIR) $(OUTPUT_DIR)

# delete the output folder for the global libs
clean_global:
	-$(EB_VERBOSE)$(RMTREE) $(GLOBAL_LIB_DIR)

show_rules:
# to avoid 'nothing to be done for show_rules' - no need to do a shell the sfk then echo a single dot.
	$(EB_VERBOSE)echo .
ifeq (,$(SILENT_HELP))
	$(info . -----------------------------------------------------------------)
	$(info .  Environment variables overview)
	$(info . -----------------------------------------------------------------)
  ifneq (,$(MISRA_MODULE))
	$(info .  MODULE            = $(MISRA_MODULE))
  endif
	$(info .  TARGET            = $(TARGET))
	$(info .  DERIVATE          = $(DERIVATE))
  ifneq (,$(SUBDERIVATE))
	$(info .  SUBDERIVATE       = $(SUBDERIVATE))
  endif
	$(info .  TOOLCHAIN         = $(TOOLCHAIN))
  ifeq (,$(realpath $(TOOLPATH_COMPILER)))
    ifneq ($(TARGET),WINDOWS)
	$(info .  TOOLPATH_COMPILER needs to be set to the used compiler directory)
    endif
  else
	$(info .  TOOLPATH_COMPILER = $(TOOLPATH_COMPILER))
  endif
	$(info .  TRESOS_BASE       = $(TRESOS_BASE))
	$(info .  PLUGINS_BASE      = $(PLUGINS_BASE))
	$(info .)
	$(info .  project directory  $(PROJECT_ROOT))
	$(info .  project name       $(PROJECT_NAME))
	$(info .  output directory   $(OUTPUT_DIR))
	$(info .  binaries           $(BIN_OUTPUT_PATH))
	$(info .  libraries          $(LIB_DIR))
	$(info .  generated files    $(GENERATED_DIR))
	$(info .  dependency files   $(DEPEND_DIR))
	$(info .  object files       $(OBJ_DIR))
	$(info .  preprocessed files $(LIST_DIR))
	$(info .)
	$(info . -----------------------------------------------------------------)
	$(info .  EB build rules overview)
	$(info . -----------------------------------------------------------------)
	$(info .  Full Scale Targets:)
	$(info .        make clean         ( remove all except generate folder        ))
	$(info .        make clean_all     ( remove output folder                     ))
	$(info .        make clean_global  ( remove output folder for the global libs ))
	$(info .        make generate      ( run tresos generator                     ))
	$(info .        make -j            ( generate depend files and executable     ))
	$(info .        make preprocess    ( generate preprocessed files              ))
	$(info .        make check_dups    ( check for duplicated file names          ))
	$(info .        make flat_src      ( copy all src files in flattened dir      ))
	$(info .        make mak           ( create all temporary files               ))
	$(info .)
	$(info .  Partial Build Options:)
	$(info .        make single_file SF=Filename      ( rebuild single file       ))
	$(info .        make single_lib  SL=libname       ( rebuild single library    ))
	$(info .        make single_lib_clean SL=libname  ( clean lib .d and .o files ))
	$(info .)
  ifneq (,$(MAKE_CLEAN_RULES)$(MAKE_GLOBAL_RULES))
	$(info .  Additional Targets:)
	$(foreach rule, $(MAKE_CLEAN_RULES),$(info .        make $(rule)		( $($(rule)_DESCRIPTION) )))
	$(foreach rule, $(MAKE_GLOBAL_RULES),$(info .        make $(rule)		( $($(rule)_DESCRIPTION) )))
  endif
endif # SILENT_HELP
