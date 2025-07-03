# *******************************************************************************************
# *******************************************************************************************
#
#       Name :      pico.make
#       Purpose :   Pic makefile
#       Date :      22nd June 2025
#       Author :    Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

#
#		Serial debugging port.
#
PICO_SERIAL_PORT = /dev/ttyUSB0
PICO_SERIAL_BAUD_RATE = 115200
#
#		Location of toolchain.
#
export PICO_SDK_PATH=/usr/share/pico-sdk
export PICO_TOOLCHAIN_PATH=/aux/builds/corev-openhw-gcc-ubuntu2204-20240530
export PICO_RISCV_TOOLCHAIN_PATH=/aux/builds/corev-openhw-gcc-ubuntu2204-20240530
#
# 		Platform selection
#
# PLATFORM = rp2350
PLATFORM = rp2350-riscv
#
#		Debug upload
#
UPLOADER = openocd 
UPCONFIG = -f interface/cmsis-dap.cfg -f target/$(PLATFORM).cfg 
UPCOMMANDS = -c "adapter speed 5000" -c "program build/$(APPNAME).elf verify reset exit"

cmake:
	cp $(PICO_SDK_PATH)/external/pico_sdk_import.cmake .
	mkdir -p build
	rm -f build/CMakeCache.txt build/risc_test*
	rm -rf build/CMakeFiles build/generated build/pico-sdk build/pioasm build/pioasm-install
	rm -f build/CMakeDoxy* build/pico_flash*
	cd build ; cmake -DPICO_PLATFORM=$(PLATFORM) -DMODULEDIR=$(MODULEDIR) ..

rbuild: 
	cd build ; make -j10

upload: rbuild
	$(UPLOADER) $(UPCONFIG) $(UPCOMMANDS)

monitor:
	picocom -b 115200 /dev/ttyACM0

all: rbuild upload monitor
