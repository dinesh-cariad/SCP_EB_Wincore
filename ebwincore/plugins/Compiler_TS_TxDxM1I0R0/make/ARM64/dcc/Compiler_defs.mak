# --------{ EB Automotive Makefile }--------

ABS_FILE_SUFFIX=elf
MAP_FILE_SUFFIX=map
ASM_FILE_SUFFIX=s
HEX_FILE_SUFFIX=mot

PROJECT_BINARY  = $(BIN_OUTPUT_PATH)/$(PROJECT).$(ABS_FILE_SUFFIX)
MAPFILE         = $(BIN_OUTPUT_PATH)/$(PROJECT).$(MAP_FILE_SUFFIX)
HEXFILE         = $(BIN_OUTPUT_PATH)/$(PROJECT).$(HEX_FILE_SUFFIX)


# ------ general files (used in common rules)
COMPILER_DIR    = $(TOOLPATH_COMPILER)/WIN64/bin
CC              = $(COMPILER_DIR)/dcc.exe
CCLIB           = $(COMPILER_DIR)/dar.exe
ASM             = $(COMPILER_DIR)/dcc.exe
LINK            = $(COMPILER_DIR)/dld.exe
HEX             = $(COMPILER_DIR)/ddump.exe
GCC             = $(PLUGINS_BASE)/Make_$(Make_VARIANT)/tools/gcc/bin/cpp.exe
