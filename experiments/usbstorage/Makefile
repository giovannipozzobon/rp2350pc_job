# *******************************************************************************************
# *******************************************************************************************
#
#      Name :     	Makefile
#      Purpose :   	Build & Run RP2350 code
#      Date :      	23rd June 2025
#      Author :    	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

include ../../environment/common.make
APPNAME = filetest
include $(BUILDENVDIR)pico.make

update:
		cp $(PICO_SDK_PATH)/lib/tinyusb/lib/fatfs/source/ff.c usbstorage
		cp $(PICO_SDK_PATH)/lib/tinyusb/lib/fatfs/source/ffsystem.c usbstorage
		cp $(PICO_SDK_PATH)/lib/tinyusb/lib/fatfs/source/ffunicode.c usbstorage
		cp $(PICO_SDK_PATH)/lib/tinyusb/lib/fatfs/source/ff.h include
		cp $(PICO_SDK_PATH)/lib/tinyusb/lib/fatfs/source/ffconf.h include
		cp $(PICO_SDK_PATH)/lib/tinyusb/lib/fatfs/source/diskio.h include		