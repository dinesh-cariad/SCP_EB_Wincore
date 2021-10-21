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


GET_LOC_FILE = $(if $(EXT_LOCATOR_FILE),$(EXT_LOCATOR_FILE),$(LOC_FILE))

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
	$(EB_VERBOSE)$(LINK) -@$(LNK_INC)  --output_file=$@ $(GET_LOC_FILE)\
	&& echo === BUILD DONE ===
endef

####################################################################################################
# build-c-file
#---------------------------------------------------------------------------------------------------
# Target................: see obj_to_cc.mak
#---------------------------------------------------------------------------------------------------
# $@....................: full path to object file e.g. <OBJ_DIR>/Appl_Main.obj
# $<....................: full path to source file e.g. <SRC_DIR>/Appl_Main.c
# $(DEPEND_DIR).........: depend directory e.g. <PROJECT_ROOT>/output/depend
# $(MAKE_INC)...........: compiler option file e.g. <PROJECT_ROOT>/output/make/compiler.inc
# $(OBJ_DIR)............: object directory e.g. <PROJECT_ROOT>/output/obj
# $(CC).................: executable for building objects defined in compiler_defs.mak
# $(MV).................: executable to move files (e.g. plugins/Make_TS_VARIANT/tools/mv.exe)
#---------------------------------------------------------------------------------------------------
# Desciption............: defines the call to the compiler
####################################################################################################
define build-c-file
	$(info compiling: $(notdir $<) $(if $(filter $(MAKE_INC),$(MAKE_INC_DEFAULT)),,(using $(notdir $(MAKE_INC)))))
	$(file > $(DEPEND_DIR)/$(notdir $(basename $@)).d, TEST=compile error in c file $(notdir $<))
	$(EB_VERBOSE)$(CC) -@$(MAKE_INC) -c --output_file=$(OBJ_DIR)/$(notdir $(basename $@)).$(OBJ_FILE_SUFFIX) $<
	$(EB_VERBOSE)if exist $(notdir $(basename $@)).asm $(MV) $(notdir $(basename $@)).asm $(OBJ_DIR)/$(notdir $(basename $@)).asm
endef

####################################################################################################
# build-s-file
#---------------------------------------------------------------------------------------------------
# Target................: see obj_to_cc.mak
#---------------------------------------------------------------------------------------------------
# $@....................: full path to object file e.g. <OBJ_DIR>/cstartup.obj
# $<....................: full path to source file e.g. <SRC_DIR>/cstartup.s
# $(DEPEND_DIR).........: depend directory e.g. <PROJECT_ROOT>/output/depend
# $(DEPEND_INC).........: option file for the gcc (contains only -D and -I options)
# $(ASM_OPT)............: assembler options
# $(GCC)................: executable for building dependencies defined in compiler_defs.mak
# $(OBJ_DIR)............: object directory e.g. <PROJECT_ROOT>/output/obj
# $(CC).................: executable for building objects from assembler source (see compiler_defs.mak)
#---------------------------------------------------------------------------------------------------
# Desciption............: defines the call to the compiler
####################################################################################################
define build-s-file
	$(info assembling: $(notdir $<))
	$(EB_VERBOSE)$(GCC) -MM @$(DEPEND_INC) $<  > $(DEPEND_DIR)/$(notdir $(basename $@)).d
	$(EB_VERBOSE)$(CC) -@$(ASM_INC) --c_extension=.$(ASM_FILE_SUFFIX) --asm_extension=.app --preproc_only --output_file=$(OBJ_DIR)/$(notdir $(basename $@)).app $<
	$(EB_VERBOSE)$(CC) --asm_extension=.app $(OBJ_DIR)/$(notdir $(basename $@)).app -@$(ASM_INC) --output_file=$(OBJ_DIR)/$(notdir $(basename $@)).$(OBJ_FILE_SUFFIX)
	-$(EB_VERBOSE)$(MV) $(notdir $(basename $@)).lst $(OBJ_OUTPUT_PATH)
endef




####################################################################################################
# build-lib-file
#---------------------------------------------------------------------------------------------------
# Target................: see lib_to_obj.mak
#---------------------------------------------------------------------------------------------------
# $@....................: full path to library file e.g. <LIB_DIR>/Base_src.a
# $+....................: list of object files e.g. <OBJ_DIR>/cstartup.obj <OBJ_DIR>/Appl_Main.obj
# $(CCLIB)..............: executable for building objects defined in compiler_defs.mak
#---------------------------------------------------------------------------------------------------
# Desciption............: defines the call to the archiver to build a lib from a list of objects
#                       : this is a recursive call
####################################################################################################
define build-lib-file
	$(call SC_CMD_LISTPERFORM,SC_CMD_AR_HELPER,\
		$(info building lib: $(notdir $@)) \
		$(EB_VERBOSE)$(CCLIB) -r $@,20,21, $+)
endef


####################################################################################################
# build-lib-file          (OPTIONAL)
#---------------------------------------------------------------------------------------------------
# Target................: see lnk.inc
#---------------------------------------------------------------------------------------------------
# $@....................: full path to linker option file e.g. <MK_TMP_DIR>/lnk.inc
# $(LINK_ONLY_OBJ)......: all objects which are linked directly
# $(LIBRARIES_LINK_ONLY): all static libraries not build from known object files
# $(OUTPUT_PATH_LIBS)...: the libraries build from known object files
#---------------------------------------------------------------------------------------------------
# Desciption............: defines how the linker options are build. 
#                         if you do not define it here, a standard rule is used in global.mak
####################################################################################################
define build-link-inc-file
	$(info $(LINK_OPT))
	$(file >$@, $(subst /,\,$(LINK_OPT)))
	$(foreach i,$(LINK_ONLY_OBJ) , $(file >>$@,$(subst /,\,$i)))
	$(foreach i,$(LIBRARIES_LINK_ONLY) , $(file >>$@,$(subst /,\,$i)))
	$(foreach i,$(OUTPUT_PATH_LIBS) , $(file >>$@,$(subst /,\,$i)))	
endef

# we can not check if build-link-inc-file is defined without expanding it
# this would cause a file open error. therefore set a seperate value here. 
build-link-inc-file-PROVIDED=TRUE
