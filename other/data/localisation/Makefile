# *******************************************************************************************
# *******************************************************************************************
#
#      Name :      Makefile
#      Purpose :   Makefile wrapper.
#      Date :      3rd July 2025
#      Author :    Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

include ../../../environment/system.make

all:
	$(PYTHON) extract.py >$(MODULEDIR)input/library/keyboard/locale_data.c
	echo "Update locale data"