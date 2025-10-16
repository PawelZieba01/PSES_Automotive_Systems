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
REL_NOTES_COMPILER_VERSION_NAME = gcc-6.3.1
#REL_NOTES_MCAL_VERSION = S32K14X MCAL4.0 EAR 0.8.1
REL_NOTES_MCAL_VENDOR = Freescale
REL_NOTES_TESTED_CHIP = S32K144 Cortex-M4
REL_NOTES_TESTED_EVALBOARD = FRDMPK144_Q100

#################################################################
# TOOLPATH_COMPILER:
# The root directory (prefix) of the GNU C Compiler for ARM chain must
# be set by the help of the variable TOOLPATH_COMPILER.
# If an environment is defined, the value of this variable will be
# used.
#
TOOLPATH_COMPILER ?= C:/worktools/compilers/Linaro/gcc-linaro-6.3.1-2017.05-i686-mingw32_arm-eabi

#################################################################
# COMMON_OPT:
# Common options used by all tools.

# Target CPU
COMMON_OPT += -mcpu=cortex-m4

# produce debug information
COMMON_OPT += -g3

# floating point in software
COMMON_OPT += -mfloat-abi=soft

# set correct FPU hardware
COMMON_OPT += -mfpu=fpv4-sp-d16

# No optimization
COMMON_OPT += -O0

# Only generate Thumb code
COMMON_OPT += -mthumb

# Produce code for little-endian mode system
COMMON_OPT += -mlittle-endian

COMMON_OPT += -c

# Sysroot for linaro libraries
COMMON_OPT += --sysroot="C:/worktools/compilers/Linaro/sysroot-newlib-linaro-2017.05-arm-eabi"

#################################################################
# CC_OPT:
# The variable CC_OPT contains all default compiler switches.

CC_OPT += $(COMMON_OPT)

# specifies the language standard to compile for.
CC_OPT += -std=c99

# enable all warnings
CC_OPT += -Wall

# Make each error message appear on a single line.
CC_OPT += -fmessage-length=0

# Place each function or data item into its own section in the output file
CC_OPT += -ffunction-sections
CC_OPT += -fdata-sections

# generate individual zero-initialized definitions
# for tentative definitions
CC_OPT += -fno-common

CC_OPT += -DCOMPILERCFG_EXTENSION_MCAL_FILE 

CC_OPT += -DDISABLE_MCAL_INTERMODULE_ASR_CHECK

#################################################################
# ASM_OPT:
# The variable ASM_OPT contains the switches for the assembler call.

ASM_OPT += $(COMMON_OPT)

ASM_OPT += -x assembler-with-cpp

#################################################################
# CPP_OPT:
# The variable CPP_OPT contains the switches for the cpp compiler call.
#
CPP_OPT  +=

#################################################################
# LINK_OPT:
# The variable LINK_OPT contains the switches for the linker call.
#

# When GCC is called for linking
LINK_OPT += -Xlinker
LINK_OPT += --gc-sections
LINK_OPT += -mcpu=cortex-m4
LINK_OPT += -mthumb
LINK_OPT += -mlittle-endian
LINK_OPT += -mfloat-abi=soft

# disable the generation of build ids
LINK_OPT += -Wl,--build-id=none

# Sysroot for linaro libraries
LINK_OPT += -Wl,--sysroot="C:/worktools/compilers/Linaro/sysroot-newlib-linaro-2017.05-arm-eabi"

#################################################################
# LINK_LIBS:
# The variable LINK_LIBS tells the linker which libraries it
# should link against. The order is significant. Providers must
# be specified after the consumer(s).
#

# link against libgcc, which contains builtins like __udiv.
#LINK_LIBS += -lgcc

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
AR_OPT += -c

# replace only newer files
AR_OPT += -u

# write an object file index
AR_OPT += -s

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
