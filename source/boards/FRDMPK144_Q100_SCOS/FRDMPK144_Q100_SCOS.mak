# #############################################################################
#
# $Id: FRDMPK144_Q100.mak 32031 2020-06-23 16:01:24Z miwi261726 $
#
# This makefile contains board-specific definitions and rules.
#
# Copyright Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.
#
# #############################################################################

# TOOLCHAIN mapping ###########################################################
ifeq ($(TOOLCHAIN),multi)
OS_TOOLCHAIN=ghs
else
OS_TOOLCHAIN=$(call lc,$(TOOLCHAIN))
endif

LIB_INCLUDE_PATH+=$(TOOLPATH_COMPILER)\lib\

# DEFINITIONS #################################################################
ifeq ($(OS_BOARD_DIR), )
OS_BOARD_DIR=$(BOARD_PROJECT_PATH)
endif

CC_INCLUDE_PATH      += $(OS_BOARD_DIR)
CPP_INCLUDE_PATH     += $(OS_BOARD_DIR)
ASM_INCLUDE_PATH     += $(OS_BOARD_DIR)

CC_FILES_TO_BUILD    += $(OS_BOARD_DIR)\board.c
CC_FILES_TO_BUILD    += $(OS_BOARD_DIR)\board_reset_vector.c
ASM_FILES_TO_BUILD   += $(OS_BOARD_DIR)\boardStartup.s

# defines #####################################################################
LINK_CONST_CODE_TARGET ?= FLASH

# setup config parameter ######################################################
# boot from ROM or RAM ?
ifeq ($(LINK_CONST_CODE_TARGET),RAM)
LINK_VARIANT := ram
OS_GENLD_OPT += -disableRomInit
else
LINK_VARIANT := flash
endif

# Minimum region size for the NXP MPU on S32K14X is 32 byte
OS_GENLD_OPT += -mpuMinRegionSize 32

# toolchain specific configs
ifeq ($(OS_TOOLCHAIN),armkeil)
OS_GENLD = $(AutosarOS_CORE_PATH)/bin/genld$(OS_HYPHEN)$(CPU)$(OS_HYPHEN)$(TOOLCHAIN).pl
LOC_FILE_SUFFIX=scat
OS_GENLD_OPT += -prepend '\#! armclang --target=arm-arm-none-eabi -mcpu=cortex-r7 -E -x c'
LINK_OPT += --undefined=CORTEXMExceptionTable
LINK_OPT += --undefined=CORTEXMInterruptTable
LINK_OPT += --undefined=reset_vector
LINK_OPT += --entry=boardResetStart
else
ifeq ($(OS_TOOLCHAIN),gnu)
OS_GENLD = $(AutosarOS_CORE_PATH)/bin/genld$(OS_HYPHEN)$(CPU).pl
LOC_FILE_SUFFIX=ldscript
LINK_OPT += -Wl,--undefined=CORTEXMExceptionTable
LINK_OPT += -Wl,--undefined=CORTEXMInterruptTable
LINK_OPT += -Wl,--undefined=reset_vector
LINK_OPT += -Wl,--entry=boardResetStart
else
ifeq ($(OS_TOOLCHAIN),ghs)
OS_GENLD = $(AutosarOS_CORE_PATH)/bin/genld$(OS_HYPHEN)$(CPU).pl
LOC_FILE_SUFFIX=ldscript
else
ifeq ($(OS_TOOLCHAIN),iar)
OS_GENLD = $(AutosarOS_CORE_PATH)/bin/genld$(OS_HYPHEN)$(CPU).pl
LOC_FILE_SUFFIX=ldscript
else
$(error "Invalid / unsupported toolchain.")
endif
endif
endif
endif

# generate linker script ######################################################
ifneq ($(OS_KERNEL_TYPE),FUNCTION_CALL)

# check if text region size was provided, if not then assume PROTECTION is
# turned off and we set the size to 0 to generate a default alignment
ifeq ($(OS_CORTEXM_GBL_TEXT_REGION_SIZE),)
OS_CORTEXM_GBL_TEXT_REGION_SIZE=0
endif

# check if rodata region size was provided, if not then assume PROTECTION is
# turned off and we set the size to 0 to generate a default alignment
ifeq ($(OS_CORTEXM_GBL_CONST_REGION_SIZE),)
OS_CORTEXM_GBL_CONST_REGION_SIZE=0
endif

# check if data region size was provided, if not then assume PROTECTION is
# turned off and we set the size to 0 to generate a default alignment
ifeq ($(OS_CORTEXM_GBL_DATA_REGION_SIZE),)
OS_CORTEXM_GBL_DATA_REGION_SIZE=0
endif

# gather all OS applications specific information
# Specify 'size' to overwrite 'startAlign' default value in GenldGnu.pm
OS_APP_SPECS = $(foreach app, $(APPLICATIONS), \
		-app $(app) -size 0x20 -core $(OS_CORE_$(app)) $(app)_gen.$(OBJ_FILE_SUFFIX) $(OBJS_$(app)) \
		$(foreach task,  $(TASKS_$(app)), -task $(task) -size 0x20 $(OBJS_$(task))) \
		$(foreach isr, $(ISRS_$(app)), -isr $(isr) -size 0x20 $(OBJS_$(isr))) \
	)	$(foreach stack, $(OS_STACKS), -stack $(stack) -size 0x20)

# setup needed files
LOC_FILE = $(PROJECT_ROOT)/output/generated/$(OS_ARCH)$(OS_HYPHEN)$(OS_TOOLCHAIN).$(LOC_FILE_SUFFIX)
LINK_ADDR_FILE = $(OS_BOARD_DIR)\board_$(LINK_VARIANT)$(OS_HYPHEN)$(OS_TOOLCHAIN).$(LOC_FILE_SUFFIX)

# call linker script generator
$(LOC_FILE): $(OS_GENLD) $(wildcard $(AutosarOS_CORE_PATH)/bin/Genld*.pm)
	$(AutosarOS_CORE_PATH)\bin\cygtools\perl $(OS_GENLD) $(OS_GENLD_OPT) \
		-backend $(OS_TOOLCHAIN) -o $@                                   \
		-include $(LINK_ADDR_FILE)                                       \
		-numcores $(OS_N_CORES_MAX)                                      \
		-text-region-size $(OS_CORTEXM_GBL_TEXT_REGION_SIZE)             \
		-const-region-size $(OS_CORTEXM_GBL_CONST_REGION_SIZE)           \
		-data-region-size $(OS_CORTEXM_GBL_DATA_REGION_SIZE)             \
		$(OS_APP_SPECS)

else

$(error "Invalid / unsupported toolchain.")
## use a static linker script for function call kernel
#LOC_FILE = $(OS_BOARD_DIR)/MFC520_B0$(OS_HYPHEN)$(OS_TOOLCHAIN).$(LOC_FILE_SUFFIX)

endif

# Editor settings - DO NOT DELETE
# vi:set ts=4:

#################################################################
# TRACE32_PATH
# Path to the Trace32 Debugger
TRACE32_PATH ?= C:\tools\T32_arm
TRACE32_APP_PATH ?= $(TRACE32_PATH)\bin\windows64