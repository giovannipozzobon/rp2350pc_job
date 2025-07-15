# *******************************************************************************************
# *******************************************************************************************
#
#       Name :      pico.linux.make
#       Purpose :   Pic makefile
#       Date :      22nd June 2025
#       Author :    Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

#
#		Serial debugging port.
#
PICO_SERIAL_PORT = /dev/tty.usbmodem2102
PICO_SERIAL_BAUD_RATE = 115200
#
#		Location of toolchain.
#
export PICO_SDK_PATH=/Users/giovanni.pozzobon/Documents/pico/pico-sdk
#
# 		Platform selection. RISCV will require the downloaded compiler, see documentation.
#
PLATFORM = rp2350
#PLATFORM = rp2350-riscv
#
#		Location of downloaded RISCV compiler.
#
#export PICO_TOOLCHAIN_PATH=/Users/giovanni.pozzobon/Documents/pico/corev-openhw-gcc-macos-20240530
#export PICO_RISCV_TOOLCHAIN_PATH=/Users/giovanni.pozzobon/Documents/pico/corev-openhw-gcc-macos-20240530
#
#		Debug upload
#
UPLOADER = openocd 
UPCONFIG = -f interface/cmsis-dap.cfg -f target/$(PLATFORM).cfg 
UPCOMMANDS = -c "adapter speed 5000" -c "program build/$(APPNAME).elf verify reset exit"

compile: 
	cd build ; make -j10

cmake:
	cp $(PICO_SDK_PATH)/external/pico_sdk_import.cmake .
	mkdir -p build
	rm -f build/CMakeCache.txt 
	rm -rf build/CMakeFiles build/generated build/pico-sdk build/pioasm build/pioasm-install
	rm -f build/CMakeDoxy* build/pico_flash*
	cd build ; cmake -DPICO_PLATFORM=$(PLATFORM) -DMODULEDIR=$(MODULEDIR) ..


upload: compile
	$(UPLOADER) $(UPCONFIG) $(UPCOMMANDS)

serial:
	picocom -b $(PICO_SERIAL_BAUD_RATE) $(PICO_SERIAL_PORT)

all: compile upload serial
