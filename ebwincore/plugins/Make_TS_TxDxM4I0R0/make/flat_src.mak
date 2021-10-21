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

#================================================================
# Contains rules for building a flat directory structure
#================================================================

FLAT_OUT_DIR = $(subst \,/,$(OUTPUT_DIR))

# ------------  Project Directories  ---------------------------------------------------
FLAT_DIR         = $(FLAT_OUT_DIR)/flat_src
FLAT_SRC_DIR     = $(FLAT_DIR)/src
FLAT_OBJ_DIR     = $(FLAT_DIR)/obj
FLAT_BIN_DIR     = $(FLAT_DIR)/bin
FLAT_MAK_DIR     = $(FLAT_DIR)/mak
FLAT_DEP_DIR     = $(FLAT_DIR)/dep
FLAT_LIB_DIR     = $(FLAT_DIR)/lib
FLAT_DEP_INC     = $(FLAT_MAK_DIR)/dep.inc
FLAT_C_INC       = $(FLAT_MAK_DIR)/c.inc
FLAT_CPP_INC     = $(FLAT_MAK_DIR)/cpp.inc
FLAT_ASM_INC     = $(FLAT_MAK_DIR)/asm.inc
FLAT_LNK_INC     = $(FLAT_MAK_DIR)/lnk.inc
FLAT_SRC_INC     = $(FLAT_MAK_DIR)/src.inc
COPY_LIB_RULES   = $(FLAT_OUT_DIR)/make/copy_lib_rules.mak
FLAT_OUT_LIB     = $(FLAT_OBJ_DIR)/$(basename $(PROJECT_BINARY)).$(LIB_FILE_SUFFIX)
FLAT_LIB         = $(FLAT_MAK_DIR)/lib.inc
FLAT_MAK         = $(FLAT_MAK_DIR)/make.inc

FLAT_LIBS        = $(foreach LIB, $(LIBRARIES_LINK_ONLY) $(OBJECTS_LINK_ONLY),$(FLAT_LIB_DIR)/$(notdir $(LIB)))

ifneq ($(MAKECMDGOALS),depend)
  ifneq ($(MAKECMDGOALS),clean_all)
    ifneq ($(MAKECMDGOALS),clean)
      # create directories
      ifneq (bin depend generated lib flat_src list make obj temp,$(notdir $(sort $(wildcard $(OUTPUT_DIR)/*))))
        SHELLCOMMAND:=$(shell $(call ifnotexist,$(FLAT_DIR), $(MKDIR) $(FLAT_DIR) ))
        SHELLCOMMAND:=$(shell $(call ifnotexist,$(FLAT_SRC_DIR), $(MKDIR) $(FLAT_SRC_DIR) ))
        SHELLCOMMAND:=$(shell $(call ifnotexist,$(FLAT_OBJ_DIR), $(MKDIR) $(FLAT_OBJ_DIR) ))
        SHELLCOMMAND:=$(shell $(call ifnotexist,$(FLAT_BIN_DIR), $(MKDIR) $(FLAT_BIN_DIR) ))
        SHELLCOMMAND:=$(shell $(call ifnotexist,$(FLAT_DEP_DIR), $(MKDIR) $(FLAT_DEP_DIR) ))
        SHELLCOMMAND:=$(shell $(call ifnotexist,$(FLAT_MAK_DIR), $(MKDIR) $(FLAT_MAK_DIR) ))
        SHELLCOMMAND:=$(shell $(call ifnotexist,$(FLAT_LIB_DIR), $(MKDIR) $(FLAT_LIB_DIR) ))
      endif
    endif
  endif
endif
# ------------  tools  --------------------------------------------------------------
FLAT_SRC_SCRIPT     := $(MAKE_ROOT)/tools/scripts/createFlatSrc.pl

# ------------  rules  --------------------------------------------------------------

# $(copy-platform-files) --> see optional definition in compiler_rules.mak of Compiler plugin

flat_src: flat_copy_src\
          $(FLAT_DEP_INC) $(FLAT_C_INC) $(FLAT_CPP_INC) $(FLAT_ASM_INC) $(FLAT_SRC_INC) $(FLAT_LNK_INC)\
          $(FLAT_LIB) $(FLAT_MAK)$(DLL_RULES) $(FLAT_LIBS) $(LOC_FILE)
	$(EB_VERBOSE)$(CP) $(PLUGINS_BASE)/Compiler_$(Compiler_VARIANT)/make/$(CPU)/$(TOOLCHAIN)/Compiler$(RULES_FILE_SUFFIX) $(FLAT_DIR)/Compiler$(RULES_FILE_SUFFIX)
	$(EB_VERBOSE)$(call ifexist,$(PLUGINS_BASE)/Compiler_$(Compiler_VARIANT)/make/$(CPU)/$(TOOLCHAIN)/linux_Makefile, $(CP) $(PLUGINS_BASE)/Compiler_$(Compiler_VARIANT)/make/$(CPU)/$(TOOLCHAIN)/linux_Makefile $(FLAT_DIR)/Makefile)
	$(EB_VERBOSE)$(call ifnotexist,$(PLUGINS_BASE)/Compiler_$(Compiler_VARIANT)/make/$(CPU)/$(TOOLCHAIN)/linux_Makefile, echo WARNING: linux_Makefile not found for toolchain $(TOOLCHAIN) in Compiler_$(Compiler_VARIANT))
	$(EB_VERBOSE)$(if $(call optional,LOC_FILE),$(CP) $(LOC_FILE) $(FLAT_DIR)/$(notdir $(LOC_FILE)))
	$(EB_VERBOSE)$(copy-platform-files)
	@$(ECHO) Created flat source in $(FLAT_DIR)

flat_copy_src: $(SRC_LIST) $(HDR_LIST)
	$(EB_VERBOSE)$(PERL_EXE) $(FLAT_SRC_SCRIPT) --src=$(SRC_LIST) --hdr=$(HDR_LIST) --depDir=$(DEPEND_DIR) --destDir=$(FLAT_DIR)

define	copy-lib-file
	$(EB_VERBOSE)$(info copying $(notdir $<))\
	$(CP) $< $@
endef

$(COPY_LIB_RULES):
	$(info Building $(notdir $(COPY_LIB_RULES)))
	$(file > $@, Dempty_dummy=0)
	$(foreach LIB, $(LIBRARIES_LINK_ONLY) $(OBJECTS_LINK_ONLY),\
          $(file >> $(COPY_LIB_RULES),\
            $(FLAT_LIB_DIR)/$(notdir $(LIB)) : $(subst \,/,$(LIB)) ; $(DOLLAR)(copy-lib-file) \
          )\
        )

$(FLAT_SRC_INC): $(MAKEFILE_LIST)
	$(info Building $(notdir $(FLAT_SRC_INC)))
	$(file > $(FLAT_SRC_INC), Dempty_dummy=0)
	$(foreach SRC, $(sort $(CC_FILES_TO_BUILD) $(ASM_FILES_TO_BUILD)),\
          $(file >> $(FLAT_SRC_INC),\
             SRC_FILE_TO_BUILD+=src/$(notdir $(subst -,_,$(SRC)))\
          )\
        )
	$(foreach OBJ, $(sort $(LINK_ONLY_OBJ) $(OBJECTS_LINK_ONLY)),\
          $(file >> $(FLAT_SRC_INC),\
             LINK_ONLY_OBJ+=obj/$(notdir $(subst -,_,$(OBJ)))\
          )\
        )
	$(file >> $(FLAT_SRC_INC),\
        LOC_FILE=$(notdir $(LOC_FILE))\
     )\

define write-inc-file
	$(EB_VERBOSE)$(info Building $(@F))\
	$(file > $@,-Dempty_dummy$(addprefix $(\n)-I,src $(patsubst $(FLAT_DIR)/%/.,%,$(wildcard $(FLAT_SRC_DIR)/*/.)))$(\n)$1 $(cppGetPreProcessorDefines))\
	$(shell $(DOS2UNIX) $@)
endef

flat_filter_opt ?= $1

$(FLAT_DEP_INC): flat_copy_src
	$(call write-inc-file,$(filter -D%,$(CC_OPT)))

$(FLAT_C_INC): flat_copy_src
	$(call write-inc-file,$(call flat_filter_opt,$(CC_OPT)))

$(FLAT_CPP_INC): flat_copy_src
	$(call write-inc-file,$(call flat_filter_opt,$(CPP_OPT)))

$(FLAT_ASM_INC): flat_copy_src
	$(call write-inc-file,$(ASM_OPT) $(filter -D%,$(CC_OPT)))

ifeq ($(build-link-inc-file-PROVIDED),)
$(FLAT_LNK_INC):
	$(file >$@, $(subst \,/,$(LINK_OPT)))
	$(foreach i,$(subst -,_,$(LINK_ONLY_OBJ)), $(file >>$@,obj/$(notdir $i)))
	$(foreach i,$(subst -,_,$(LIBRARIES_LINK_ONLY)), $(file >>$@,lib/$(notdir $i)))
	$(foreach i,$(subst -,_,$(OBJECTS_LINK_ONLY)), $(file >>$@,lib/$(notdir $i)))
	$(file >>$@,$(addprefix lib/,$(notdir $(FLAT_OUT_LIB))))
else
$(FLAT_LNK_INC):
	$(build-link-inc-file-flat)
endif

$(FLAT_MAK):
	$(file > $@,#generated file $(notdir $@))
	$(file >>$@,PROJECT_BINARY=$(notdir $(PROJECT_BINARY)))
	$(file >>$@,CC_OPT=$(CC_OPT))
	$(file >>$@,ASM_OPT=$(ASM_OPT))
	$(file >>$@,CPP_OPT=$(CPP_OPT))
	$(file >>$@,LINK_OPT=$(subst \,/,$(LINK_OPT)))
	$(file >>$@,BOARD=$(BOARD))
	$(file >>$@,TARGET=$(TARGET))
	$(file >>$@,DERIVATE=$(DERIVATE))
	$(file >>$@,TOOLCHAIN=$(TOOLCHAIN))

$(FLAT_LIB):
	$(info Building $(notdir $(FLAT_LIB)))
	$(file > $(FLAT_LIB),# generated file $(notdir $(FLAT_LIB)))
	$(foreach OBJ, \
	  $(addprefix obj/,\
	    $(addsuffix .$(OBJ_FILE_SUFFIX), \
	      $(notdir \
	        $(basename $(LIB_OBJECTS_OUT)) \
	      ) \
	    )\
	  ),\
	  $(file >>$(FLAT_LIB), \
	    $(addprefix lib/,$(notdir $(FLAT_OUT_LIB))): $(subst -,_,$(OBJ)) \
	  ) \
	)

ifneq ($(MAKECMDGOALS),clean)
-include $(COPY_LIB_RULES)
endif