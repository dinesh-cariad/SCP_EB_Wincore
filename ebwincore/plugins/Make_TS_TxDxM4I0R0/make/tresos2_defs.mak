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

.PHONY: early_generate generate generate_all generate_all_run
#################################################################
# DEFINITIONS

# set tresos batch file
TRESOS2_GENERATOR     := $(TRESOS_BIN)/tresos_cmd.bat

#################################################################
# Generate the default SWCDs (via mode generate_swcd)
# If set to ASR31: The AUTOSAR 3.1 SWCDs are generated instead (mode=generate_asr31_swcd)
# If set to ASR32: The AUTOSAR 3.2 SWCDs are generated instead (mode=generate_asr32_swcd)
# If set to ASR40: The AUTOSAR 4.0 SWCDs are generated instead (mode=generate_asr32_swcd)
TRESOS2_GENERATE_SWCD_MODE ?= DEFAULT

#################################################################
# If set to TRUE, all System Description files are first converted
# to one temporary existing arxml file with latest provided AUTOSAR version.
# This temporary file is then used as input for the generation of
# source code in EB tresos Studio.
# Note: This parameter does not affect code generation in project mode
TRESOS2_CONVERT_TO_SYSTEM_MODEL40 ?= FALSE
TRESOS2_SYSTEM_MODEL40_FILENAME   ?= SystemModel2.tdb@tdb:4.1

#################################################################
# If set to TRUE, merge all module configuration files into a
# temporary one before generation in legacy mode
TRESOS2_MERGE_XDM           ?= FALSE
TRESOS2_MERGED_XDM_FILENAME ?= $(TRESOS2_OUTPUT_PATH)/config/$(PROJECT_NAME).xdm

#################################################################
# If set to TRUE, call tresos Studio to generate the contract
# in legacy mode
TRESOS2_GENERATE_CONTRACT ?= FALSE

#################################################################
# Create directories
DIRECTORIES_TO_CREATE += $(TRESOS2_OUTPUT_PATH) $(TRESOS2_OUTPUT_PATH)/make

################################################################
# generate a list of config files for generator call
#
# tresos needs configuration files for both, the modules which shall
# be generated and for those whose configuration is just read
TRESOS2_CONF_FILES_TEMP = \
 $(subst \,/,$(foreach plugin,$(sort $(TRESOS2_GEN_PLUGINS) $(TRESOS2_NOGEN_PLUGINS) $(TRESOS2_STUB_PLUGINS)),$($(plugin)_CONF_FILE)))

# list of configuration files shall be sorted alphabetically by file names
# create lookup table from file name to path of configuration file
$(eval $(foreach conffile,$(TRESOS2_CONF_FILES_TEMP),TRESOS2_$(notdir $(conffile))_CONF_FILE += $(conffile)$(\n)))
# create list of unique configuration file paths sorted by file name using lookup table
TRESOS2_CONF_FILES = $(foreach conffile,$(sort $(notdir $(TRESOS2_CONF_FILES_TEMP))),$(TRESOS2_$(conffile)_CONF_FILE))

TRESOS2_ALL_PLUGINS = $(sort $(TRESOS2_GEN_PLUGINS) $(TRESOS2_NOGEN_PLUGINS) $(TRESOS2_STUB_PLUGINS))

# get config files of each plugin
$(foreach plugin,$(TRESOS2_ALL_PLUGINS),$(eval TRESOS2_$(plugin)_CONF_FILES = $(filter $(addprefix %/,$(filter $(plugin).xdm $(plugin)_%.xdm $(plugin).epc $(plugin)_%.epc,$(notdir $(TRESOS2_CONF_FILES)))),$(TRESOS2_CONF_FILES))))

################################################################
# assemble the whole plugin name including the variant string.
# tresos will just generate these plugins
ifeq (,$(MODULE))
  PLUGINS_TO_GENERATE     := $(foreach plugin,$(TRESOS2_GEN_PLUGINS),$(plugin)_$($(plugin)_VARIANT))
  PLUGINS_TO_GENERATE_PRJ :=
else
  PLUGINS_TO_GENERATE     := $(MODULE)_$($(MODULE)_VARIANT)
  PLUGINS_TO_GENERATE_PRJ := $(PLUGINS_TO_GENERATE)
endif

################################################################
# generate a list of modules having a config file
#
TRESOS2_CONF_MODULE_IDS   = $(sort $(PLUGINS_TO_GENERATE) $(foreach MOD,Resource $(TRESOS2_ADDITIONAL_RSC_MODULES),$(if $($(MOD)_VARIANT),$(MOD)_$($(MOD)_VARIANT))))
TRESOS2_CONF_STUBS        = $(filter-out $(TRESOS2_GEN_PLUGINS),$(basename $(notdir $(TRESOS2_CONF_FILES))))
TRESOS2_CONF_STUBS_IDS    = $(foreach MOD,$(patsubst %Stub,%,$(TRESOS2_CONF_STUBS)),$(if $($(MOD)Stub_VARIANT),$(MOD)_$($(MOD)Stub_VARIANT)))
TRESOS2_CONF_STUBS_40_IDS = $(foreach MOD,$(patsubst %Stub,%,$(TRESOS2_CONF_STUBS)),$(if $($(MOD)Stub_40_VARIANT),$(MOD)_$($(MOD)Stub_40_VARIANT)))
TRESOS2_CONF_STUBS_41_IDS = $(foreach MOD,$(patsubst %Stub,%,$(TRESOS2_CONF_STUBS)),$(if $($(MOD)Stub_41_VARIANT),$(MOD)_$($(MOD)Stub_41_VARIANT)))
TRESOS2_CONF_STUBS_42_IDS = $(foreach MOD,$(patsubst %Stub,%,$(TRESOS2_CONF_STUBS)),$(if $($(MOD)Stub_42_VARIANT),$(MOD)_$($(MOD)Stub_42_VARIANT)))
TRESOS2_CONF_STUBS_43_IDS = $(foreach MOD,$(patsubst %Stub,%,$(TRESOS2_CONF_STUBS)),$(if $($(MOD)Stub_43_VARIANT),$(MOD)_$($(MOD)Stub_43_VARIANT)))

# create a semicolon-separated list. Value for -DEcuResourceModuleIds (generator call, legacy mode only)
TRESOS2_CONF_ECU_RESOURCEMODULE_IDS = $(subst $(BLANK),;,$(strip $(TRESOS2_CONF_MODULE_IDS) $(TRESOS2_CONF_STUBS_IDS) $(TRESOS2_CONF_STUBS_40_IDS) $(TRESOS2_CONF_STUBS_41_IDS) $(TRESOS2_CONF_STUBS_42_IDS) $(TRESOS2_CONF_STUBS_43_IDS)))

################################################################
# default location for files holding the system description
# when generating in the legacy mode
#
# TRESOS2_SYSD_GENSWCD_FILES:
#    Files passed in generation mode 'generate_swcd'
#
ifeq (,$(TRESOS2_PROJECT_NAME))
  # input sysd files for the "generate_swcd" step
  TRESOS2_SYSD_GENSWCD_FILES ?= $(addsuffix @sysd,$(sort $(wildcard $(PROJECT_ROOT)/config/generate_swcd/*.arxml)))

  # generated sysd files
  TRESOS2_SYSD_GENERATED_FILES ?= $(PROJECT_ROOT)/output/generated/swcd/*.arxml

  # static sysd files
  TRESOS2_SYSD_ALL_FILES ?= $(addsuffix @sysd,$(sort $(wildcard $(PROJECT_ROOT)/config/*.arxml)))
  TRESOS2_SYSD_ALL_FILES +=\
    $(addsuffix @sysd,$(TRESOS2_SYSD_GENERATED_FILES))\
    $(TRESOS2_SYSD_GENSWCD_FILES)\
    $(TRESOS2_SYSD_FILES)
endif

################################################################
# check for clean targets; for a clean target, the
# generator should not be called

# if no target is given, call the generator
ifeq (,$(strip $(MAKECMDGOALS)))
  TRESOS2_GENERATOR_MODE := CALL_GEN
else
  # If a a target es given and the target is not clean or
  # clean_depend, call the generator.
  ifneq (,$(strip $(filter-out $(strip $(MAKE_CLEAN_RULES) clean clean_depend $(MAKE_CONFIG_RULES) $(MAKE_ADD_RULES)),$(strip $(MAKECMDGOALS)))))
    TRESOS2_GENERATOR_MODE := CALL_GEN
  endif
endif

# if no config files are given in legacy mode, the generator should not be invoked
ifeq (,$(TRESOS2_PROJECT_NAME))
  ifeq ($(strip $(TRESOS2_CONF_FILES)),)
    TRESOS2_GENERATOR_MODE := NO_ACTION
  endif
endif

ifeq ($(TRESOS2_GENERATOR_MODE),CALL_GEN)

  ifeq (,$(TRESOS2_WORKSPACE_DIR))
    # Use temporary workspace by default, if the test does not already use a
    # self-defined workspace.
    # Default can be switched-off by setting TRESOS2_USE_TMP_WORKSPACE to be
    # empty.
    TRESOS2_USE_TMP_WORKSPACE ?= 1
  endif

  # If TRESOS2_USE_TMP_WORKSPACE is set, then tresos Studio will always
  # use a temporary Eclipse workspace
  ifneq (,$(TRESOS2_USE_TMP_WORKSPACE))
    # temporary workspace directory
    ifeq (,$(TRESOS2_PROJECT_NAME))
      TRESOS2_WORKSPACE_DIR := $(TRESOS2_OUTPUT_PATH)/.ws_legacy
    else
      TRESOS2_WORKSPACE_DIR := $(abspath $(PROJECT_ROOT)/../.ws_$(TRESOS2_PROJECT_NAME))
      TRESOS2_WORKSPACE_OUT := $(TRESOS2_OUTPUT_PATH)/$(notdir $(TRESOS2_WORKSPACE_DIR))
    endif
  endif
  ifneq (,$(TRESOS2_WORKSPACE_DIR))
    TRESOS2_WORKSPACE_OPTION := -data $(TRESOS2_WORKSPACE_DIR)
  endif

  ################################################################
  # Provide required generation options for SWCD generation
  # equivalent: TRESOS2_GENERATE_SWCD_MODE_OPTION := generate$(addprefix _,$(call lc,$(TRESOS2_GENERATE_SWCD_MODE)))_swcd
  #
  ifeq ($(TRESOS2_GENERATE_SWCD_MODE), ASR31)
    TRESOS2_GENERATE_SWCD_MODE_OPTION := generate_asr31_swcd
  else ifeq ($(TRESOS2_GENERATE_SWCD_MODE), ASR32)
    TRESOS2_GENERATE_SWCD_MODE_OPTION := generate_asr32_swcd
  else ifeq ($(TRESOS2_GENERATE_SWCD_MODE), ASR40)
    TRESOS2_GENERATE_SWCD_MODE_OPTION := generate_asr40_swcd
  else
    TRESOS2_GENERATE_SWCD_MODE_OPTION := generate_swcd
  endif

endif #ifeq ($(TRESOS2_GENERATOR_MODE),CALL_GEN)

################################################################
# Provide system model input files for source code generation
# TRESOS2_SYSTEM_MODEL_INPUT_FILES can be overwritten
# Example use-case:
#   a pre-defined patched tdb file is provided, then
#   this variable can be set to this tdb file
#   (e.g. due to a bug in the EcuExtract creator)
ifeq ($(TRESOS2_CONVERT_TO_SYSTEM_MODEL40), TRUE)
  TRESOS2_SYSTEM_MODEL_INPUT_FILES ?= $(TRESOS2_SYSTEM_MODEL40_FILENAME)
else
  TRESOS2_SYSTEM_MODEL_INPUT_FILES ?= $(TRESOS2_SYSD_ALL_FILES)
endif

# TRESOS2_AUTOCONFIGURE_TARGETS contains all targets for which
# tresos will be called with the autconfigure wizard
#autoconfigure steps are set from the project makefiles
TRESOS2_AUTOCONFIGURE_TARGETS ?= 

# TRESOS2_CONF_INPUT_FILES resolves either to the merged
# configuration file if set, or all configruation files
ifeq ($(TRESOS2_MERGE_XDM), TRUE)
  TRESOS2_CONF_INPUT_FILES ?= $(TRESOS2_MERGED_XDM_FILENAME)
  DIRECTORIES_TO_CREATE    += $(TRESOS2_OUTPUT_PATH)/config
else
  TRESOS2_CONF_INPUT_FILES ?= $(TRESOS2_CONF_FILES)
endif

# TRESOS2_BATCH_FILE is the path of the batch file
# that contains the commands to be executed by
# tresos Studio batch processing, no matter if project or legacy mode
# This cannot be changed, or the batch-of-batches generation mode won't work
override TRESOS2_BATCH_FILE  := $(TRESOS2_OUTPUT_PATH)/tresos_cmd.txt
