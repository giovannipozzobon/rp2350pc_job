# *******************************************************************************************
# *******************************************************************************************
#
#       Name :      common.make
#       Purpose :   Common make
#       Date :      22nd June 2025
#       Author :    Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

# *******************************************************************************************
#
#								  Directory of test to run
#
# *******************************************************************************************

TESTAPP = expr

# *******************************************************************************************
#
#                                       Configuration
#
# *******************************************************************************************

CC = gcc
PYTHON = python3

# *******************************************************************************************
#
#                                       Directories
#
# *******************************************************************************************
#
#       Root directory of repo
#
ROOTDIR =  $(dir $(realpath $(lastword $(MAKEFILE_LIST))))../
#
#       Working directories
#
BINDIR = $(ROOTDIR)bin/
BUILDDIR = $(ROOTDIR)build/
#
#       Where the build environment files are (e.g. like this)
#
BUILDENVDIR = $(ROOTDIR)environment/
#
#       Source related files.
#
SOURCEDIR = $(ROOTDIR)source/
CSOURCEDIR = $(SOURCEDIR)/c/
SCRIPTDIR = $(SOURCEDIR)/scripts/
GENINCDIR = $(SOURCEDIR)/scripts/generated/

PICO_SDK = /aux/build/pico-sdk

# *******************************************************************************************
#
#                   Uncommenting .SILENT will shut the whole build up.
#
# *******************************************************************************************

ifndef VERBOSE
.SILENT:
endif

default: build 

# *******************************************************************************************
#
#                                       C Building 
#
# *******************************************************************************************

CORESOURCE = $(shell find -L $(CSOURCEDIR) -name "*.c")
SOURCES = $(CORESOURCE) 
OBJECTS = $(subst .c,.o,$(SOURCES))
INCLUDES = -I $(SOURCEDIR)include -I $(GENINCDIR)
MAKEFLAGS = --no-print-directory
CFLAGS = -O2 -Wall -D_FORTIFY_SOURCE=2 -fstack-clash-protection -fstack-protector-strong -pipe -fmax-errors=5 -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable
LDFLAGS = -fmax-errors=5
LINKER = $(CC) $^ $(LDFLAGS) -o $@
BINARY = $(BINDIR)__run
ONEBINARY = $(BINDIR)__onerun

%.o:%.c
	$(CC)  $(CFLAGS) $(INCLUDES) -c -o $@ $<

run: 
	$(BINARY)

onerun: 
	$(ONEBINARY)

# *******************************************************************************************
#
#                               Rerun the build scripts
#
# *******************************************************************************************

scripts:
	make $(MAKEFLAGS) -B -C $(SCRIPTDIR) build

clean:
	rm -f $(OBJECTS) $(BUILDDIR)*.o $(BINDIR)__* $(BUILDDIR)basic.c


