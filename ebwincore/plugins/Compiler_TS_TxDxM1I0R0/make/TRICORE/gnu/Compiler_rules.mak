#
# \file
#
# \brief AUTOSAR Compiler
#
# This file contains the implementation of the AUTOSAR
# module Compiler.
#
# \version 1.0.8
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 2017 - 2021 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.


GET_LOC_FILE = $(subst \,/,$(if $(EXT_LOCATOR_FILE),$(EXT_LOCATOR_FILE),$(LOC_FILE)))

####################################################################################################
# build-exe-file
#---------------------------------------------------------------------------------------------------
# Target................: $(PROJECT_BINARY)
#---------------------------------------------------------------------------------------------------
# $@....................: e.g. project.elf
# $(LNK_INC)............: generated linker option file (e.g. <PROJECT_ROOT>/output/make/link.inc)
# $(GET_LOC_FILE).......: linker locator file 
#---------------------------------------------------------------------------------------------------
# Desciption............: defines the call to the linker
####################################################################################################
define build-exe-file
	$(info Linking $(notdir $@))
	$(EB_VERBOSE)$(LINK) -Map=$(MAPFILE) -L$(subst \,/,$(BOARD_PROJECT_PATH)) @$(subst \,/,$(LNK_INC)) -T $(GET_LOC_FILE) -o $(subst \,/,$@)\
	&& $(OBJCPY) -O srec $(subst \,/,$@) $(subst \,/,$(addsuffix .srec, $(basename $@)))\
	&& echo === BUILD DONE ===
endef

####################################################################################################
# build-c-file
#---------------------------------------------------------------------------------------------------
# Target................: see obj_to_cc.mak
#---------------------------------------------------------------------------------------------------
# $@....................: full path to object file e.g. <OBJ_DIR>/Appl_Main.o
# $<....................: full path to source file e.g. <SRC_DIR>/Appl_Main.c
# $(DEPEND_DIR).........: depend directory e.g. <PROJECT_ROOT>/output/depend
# $(MAKE_INC)...........: compiler option file e.g. <PROJECT_ROOT>/output/make/compiler.inc
# $(OBJ_DIR)............: object directory e.g. <PROJECT_ROOT>/output/obj
# $(CC).................: executable for building objects defined in compiler_defs.mak
#---------------------------------------------------------------------------------------------------
# Desciption............: defines the call to the compiler
####################################################################################################
define build-c-file
	$(info compiling: $(notdir $<))
	$(file >$(DEPEND_DIR)/$(notdir $(basename $@)).d, TEST=compile error in c file $(notdir $<))
	$(EB_VERBOSE)$(CC) @$(MAKE_INC) -MD -MF $(DEPEND_DIR)/$(notdir $(basename $<).d) -c -x c $< -o $(OBJ_DIR)/$(notdir $(basename $<).o)
endef

define build-cpp-file
	$(info c++ compiling: $(notdir $<))
	$(file >$(DEPEND_DIR)/$(notdir $(basename $@)).d, TEST=compile error in c file $(notdir $<))
	$(EB_VERBOSE)$(CC) -MT $(OBJ_DIR)/$(notdir $(basename $<).o) -M @$(DEPEND_INC) $< -MF $(DEPEND_DIR)/$(notdir $(basename $@)).d\
	&& $(GPP) @$(MAKE_INCPP) -c -x c++ $< -o $(OBJ_DIR)/$(notdir $(basename $<).o)    
endef

####################################################################################################
# build-s-file
#---------------------------------------------------------------------------------------------------
# Target................: see obj_to_cc.mak
#---------------------------------------------------------------------------------------------------
# $@....................: full path to object file e.g. <OBJ_DIR>/cstartup.o
# $<....................: full path to source file e.g. <SRC_DIR>/cstartup.s
# $(DEPEND_DIR).........: depend directory e.g. <PROJECT_ROOT>/output/depend
# $(DEPEND_INC).........: option file for the gcc (contains only -D and -I options)
# $(ASM_INC)............: assembler option file e.g. <PROJECT_ROOT>/output/make/assembler.inc
# $(GCC)................: executable for building dependencies defined in compiler_defs.mak
# $(OBJ_DIR)............: object directory e.g. <PROJECT_ROOT>/output/obj
# $(ASM)................: executable for building objects from assembler source (see compiler_defs.mak)
#---------------------------------------------------------------------------------------------------
# Desciption............: defines the call to the compiler
####################################################################################################
define build-s-file
	$(info assembling: $(notdir $<))
	$(EB_VERBOSE)$(GCC) -MT $(OBJ_DIR)/$(notdir $(basename $<).o) -M @$(DEPEND_INC) $< -MF $(DEPEND_DIR)/$(notdir $(basename $@)).d\
	&& $(ASM) @$(ASM_INC) -c -x assembler-with-cpp $< -o $(OBJ_DIR)/$(notdir $(basename $<).o)
endef

####################################################################################################
# build-lib-file
#---------------------------------------------------------------------------------------------------
# Target................: see lib_to_obj.mak
#---------------------------------------------------------------------------------------------------
# $@....................: full path to library file e.g. <LIB_DIR>/Base_src.a
# $+....................: list of object files e.g. <OBJ_DIR>/cstartup.o <OBJ_DIR>/Appl_Main.o
# $(CCLIB)..............: executable for building objects defined in compiler_defs.mak
#---------------------------------------------------------------------------------------------------
# Desciption............: defines the call to the archiver to build a lib from a list of objects
#                       : this is a recursive call
####################################################################################################
define build-lib-file
	$(call SC_CMD_LISTPERFORM,SC_CMD_AR_HELPER,\
		$(info building lib: $(notdir $@)) \
		$(EB_VERBOSE)$(CCLIB) -c -u -s -r $@,20,21, $+)
endef

####################################################################################################
# build-link-inc-file and build-link-inc-file-flat
#---------------------------------------------------------------------------------------------------
# Target................: see LINK_OPT
#---------------------------------------------------------------------------------------------------
# $@....................: full path to linker option file e.g. <PROJECT_ROOT>/output/make/lnk.inc
# $(LINK_ONLY_OBJ)......: objects registered for linking only
# $(LIBRARIES_LINK_ONLY): libs registered for linking only
# $(OUTPUT_PATH_LIBS)   : list of all libs being linked
#---------------------------------------------------------------------------------------------------
# Desciption............: compiler specific implementation of the linker option file generation recipe
#                       : in this case all backslashes are converted to forward slashes for TRICORE
####################################################################################################
build-link-inc-file-PROVIDED=YES

define build-link-inc-file
	$(info Building lnk.inc)
	$(file >$(subst \,/,$@), $(subst \,/,$(LINK_OPT)\
	$(\n)--start-group\
	$(addprefix $(\n),$(LINK_ONLY_OBJ) $(LIBRARIES_LINK_ONLY) $(OBJECTS_LINK_ONLY) $(OUTPUT_PATH_LIBS))\
	$(\n)--end-group))
endef

define build-link-inc-file-flat
	$(info Building lnk.inc)
	$(file >$(subst \,/,$@), $(subst \,/,$(LINK_OPT)\
	$(\n)--start-group\
	$(addprefix $(\n)obj/,$(notdir $(LINK_ONLY_OBJ)))\
	$(addprefix $(\n)lib/,$(notdir $(LIBRARIES_LINK_ONLY) $(OBJECTS_LINK_ONLY)))\
	$(addprefix $(\n)obj/,$(notdir $(FLAT_OUT_LIB)))\
	$(addprefix $(\n)obj/,$(addsuffix .a,$(notdir $(basename $(PROJECT_BINARY)))))\
	$(\n)--end-group))
endef

####################################################################################################
# copy-platform-files
#---------------------------------------------------------------------------------------------------
# Target................: flat_src (see flat_src.mak)
#---------------------------------------------------------------------------------------------------
# Desciption............: defines the platform specific files which are copied into the flat build
#                       : directory as last action of the flat_src rule
####################################################################################################
define copy-platform-files
	$(info copy linker files)
	$(EB_VERBOSE)$(CP) $(BOARD_PROJECT_PATH)/*.ldscript $(FLAT_MAK_DIR) >$(DEVNULL)
	$(EB_VERBOSE)$(CP) $(GENERATED_DIR)/*.ldscript $(FLAT_MAK_DIR) >$(DEVNULL)
endef
