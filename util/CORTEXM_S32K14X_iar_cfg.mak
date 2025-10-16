# \file
#
# \brief AUTOSAR Os
#
# This file contains the implementation of the AUTOSAR
# module Os.
#
# \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
#
# Copyright 1998 - 2020 Elektrobit Automotive GmbH
# All rights exclusively reserved for Elektrobit Automotive GmbH,
# unless expressly agreed to otherwise.

#################################################################
# Release Notes relevant Infos:
#
REL_NOTES_COMPILER_VERSION_NAME = IAR ANSI C/C++ Compiler V7.50.1.10123/W32 for ARM
REL_NOTES_MCAL_VERSION = 
REL_NOTES_MCAL_VENDOR = NXP
REL_NOTES_TESTED_CHIP = S32K144 Cortex-M4
REL_NOTES_TESTED_EVALBOARD = FRDMPK144_Q100

#################################################################
# TOOLPATH_COMPILER:
# The root directory (prefix) of the GNU C Compiler for ARM chain must
# be set by the help of the variable TOOLPATH_COMPILER.
# If an environment is defined, the value of this variable will be
# used.
#
TOOLPATH_COMPILER ?= C:\compiler\IAR_7.50

#################################################################
# CC_OPT:
# The variable CC_OPT contains all default compiler switches.

# Keep long lines in diagnostic messages
CC_OPT += --no_wrap_diagnostics

# Enable language extensions such as extended keywords and anonymous structs and unions
CC_OPT += -e

# Specify a specific processor variant
CC_OPT += --cpu Cortex-M4

# Disable hardware floating point unit; use software floating-point library
CC_OPT += --fpu None

# include information in the object modules required by debuggers
CC_OPT += --debug

# Specify a DLIB configuration header: the normal library configuration will be used
CC_OPT += --dlib_config "$(TOOLPATH_COMPILER)\arm\inc\c\DLib_Config_Normal.h"

# Specify little-endian as the default byte order
CC_OPT += --endian little

# Select the 'thumb' processor mode
CC_OPT += --cpu_mode thumb

# Set the compiler optimization level: l = low
CC_OPT += -Ol

# Disable common subexpression elimination
CC_OPT += --no_cse

# Disable loop unrolling
CC_OPT += --no_unroll

# Disable function inlining
CC_OPT += --no_inline

# Disable code motion optimizations
CC_OPT += --no_code_motion

# Disable type-based alias analysis
CC_OPT += --no_tbaa

# Disable static clustering optimizations
CC_OPT += --no_clustering

# Disable the instruction scheduler
CC_OPT += --no_scheduling

# Preprocessor symbol definitions
CC_OPT += -D_ICCARM_ -DOSIARARM -DTS_ARCH_FAMILY=ARM -DTS_ARCH_DERIVATE=S32K144 -D_IAR_C_S32K14x_ -DCANSM_NO_SYMBOLS_WITHOUT_PREFIX -DCOMM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES -DNM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES


#################################################################
# ASM_OPT:
# The variable ASM_OPT contains the switches for the assembler call.

# Specify a specific processor variant
ASM_OPT += --cpu Cortex-M4

# Disable hardware floating point unit; use software floating-point library
ASM_OPT += --fpu None

# Tell the assembler to be is sensitive to the case of user symbols: '+' = case-sensitive
ASM_OPT += -s+

# Tell the assembler to generate debug information
ASM_OPT += -r

#################################################################
# CPP_OPT:
# The variable CPP_OPT contains the switches for the cpp compiler call.
#
CPP_OPT += 

#################################################################
# LINK_OPT:
# The variable LINK_OPT contains the switches for the linker call.

# Keep long lines in diagnostic messages
LINK_OPT += --no_wrap_diagnostics

# Include the debug interface—breakpoint mechanism in the output image.
# If no parameter is specified the BKPT mechanism is included for Cortex-M.
LINK_OPT += --semihosting


# Specify entry symbol
LINK_OPT += --entry boardResetStart

#################################################################
# LINK_LIBS:
# The variable LINK_LIBS tells the linker which libraries it
# should link against. The order is significant. Providers must
# be specified after the consumer(s).
#
LINK_LIBS += 

#################################################################
# The Application Makefile (Makefile.mak) contains the variable
# COMPILER_MODE. This variable defines, how the include paths and
# the flags (parameters) will be passed to the compiler. Normally,
# all parameters are passed by the windows command line but
# Windows 2000 has a restriction of 2048 characters
# (Windows XP - 8192). Therefore the Customer Build Environment
# supports up to three different compiler parameter modes.
#
# GLOBAL_OPTION_FILE:
# Before calling the compiler, a global parameter file is
# created by the Compiler Makefile Plugin. This file contains
# all include paths and the compiler flags. It is used
# for all source files. This means that the options
# <source_file>_CC_OPT and <source_file>_INCLUDE have no effect.
# This is the default compiler mode.
#
# NO_OPTION_FILE mode:
# All parameters are passed using the command line. If the include
# paths are longer than the limitations of the command line,
# the make run will abort with an error message.
#
# LOCAL_OPTION_FILE:
# If the value LOCAL_OPTION_FILE is set, a parameter file with
# the include paths and compiler flags will be created for each
# source file. The make process will be VERY slow when this option
# is used. The advantage is, that different include path and
# parameters can be defined for every source file. If you do not
# need this feature, you should not use this option.

# COMPILER_MODE := GLOBAL_OPTION_FILE

################################################################
# AR_OPT:
# The variable AR_OPT contains the options for the library call.
#
# Default: -r (added elsewhere).

# create archive if it doesn't exist
# AR_OPT += -c

# replace only newer files
# AR_OPT += -u

# write an object file index
# AR_OPT += -s

#################################################################
# EXT_LOCATOR_FILE:
# This variable specifies the name for an external locator/linker file.
# If the variable is empty, the board makefile will generate a default
# locator/linker file.
#
EXT_LOCATOR_FILE +=

#################################################################
# CREATE_PREPROCESSOR_FILE:
# This variable allows to define if a secial preprocessor file
# (output/obj/*.pre) shall be created or not.
CREATE_PREPROCESSOR_FILE = NO
