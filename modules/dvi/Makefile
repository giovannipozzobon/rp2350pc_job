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

include ../../environment/system.make
APPNAME = dvi_out_hstx_encoder
include $(BUILDENVDIR)pico.$(OS).make

riscv:
	cp $(ROOTDIR)other/prebuilt/blinky_riscv.uf2 /run/media/paulr/RP2350

arm:
	cp $(ROOTDIR)other/prebuilt/blinky_arm.uf2 /run/media/paulr/RP2350