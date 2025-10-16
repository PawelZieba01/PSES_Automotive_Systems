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
REL_NOTES_COMPILER_VERSION_NAME = ghs-2017.1.4
REL_NOTES_MCAL_VENDOR = Freescale
REL_NOTES_TESTED_CHIP = S32K144
REL_NOTES_TESTED_EVALBOARD = FRDMPK144_Q100

#################################################################
# TOOLPATH_COMPILER:
# The root directory (prefix) of the GNU C Compiler for ARM chain must
# be set by the help of the variable TOOLPATH_COMPILER.
# If an environment is defined, the value of this variable will be
# used.
#
TOOLPATH_COMPILER ?= C:\work\workspace\ACP-8.8.3_S32K14x_25907\tools\toolchain

# GreenHills Driver Options:
#
DRV_OPT =

DRV_OPT += -cpu=cortexm4f
DRV_OPT += -fsingle
DRV_OPT += -ansi
DRV_OPT += -Osize
DRV_OPT += -dual_debug
DRV_OPT += -G
DRV_OPT += --no_exceptions
DRV_OPT += -Wundef
DRV_OPT += -Wimplicit-int
DRV_OPT += -Wshadow
DRV_OPT += -Wtrigraphs
DRV_OPT += -Wall
DRV_OPT += --prototype_errors
DRV_OPT += --incorrect_pragma_warnings
DRV_OPT += -noslashcomment
DRV_OPT += -nostartfile
DRV_OPT += --short_enum
DRV_OPT += --no_commons
DRV_OPT += -keeptempfiles
DRV_OPT += -list
DRV_OPT += -DGHS
DRV_OPT += -DDISABLE_MCAL_INTERMODULE_ASR_CHECK
DRV_OPT += -asm=-list
DRV_OPT += -Mn
DRV_OPT += -delete
DRV_OPT += -ignore_debug_references
DRV_OPT += -map
DRV_OPT += -keepmap
DRV_OPT += -preprocess_assembly_files

#################################################################
# CC_OPT:
# The variable CC_OPT contains all default compiler switches.
CC_OPT = $(DRV_OPT) -c

#################################################################
# ASM_OPT:
# The variable ASM_OPT contains the switches for the assembler call.
ASM_OPT = $(DRV_OPT) -c

#################################################################
# CPP_OPT:
# The variable CPP_OPT contains the switches for the cpp compiler call.
CPP_OPT  +=

#################################################################
# LINK_OPT:
# The variable LINK_OPT contains the switches for the linker call.
LINK_OPT += $(DRV_OPT) -v

# Let the interrupt table be linked, even though none of its symbols are referenced
# LINK_OPT += -u CORTEXMInterruptTable

#################################################################
# LINK_LIBS:
# The variable LINK_LIBS tells the linker which libraries it
# should link against. The order is significant. Providers must
# be specified after the consumer(s).
#

# link against libgcc, which contains builtins like __udiv.
# LINK_LIBS += -lgcc

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

COMPILER_MODE := GLOBAL_OPTION_FILE

################################################################
# AR_OPT:
# The variable AR_OPT contains the options for the library call.
#
# Default: -r (added elsewhere).

# create archive if it doesn't exist
AR_OPT += -archive

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
