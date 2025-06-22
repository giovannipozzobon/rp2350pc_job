# *******************************************************************************************
# *******************************************************************************************
#
#      Name :     	Makefile
#      Purpose :   	Build & Run RP2350 code
#      Date :      	15th June 2025
#      Author :    	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

include ../../environment/common.make

#
#	For RISCV, target in UPCONFIG is rp2350-riscv and PLATFORM is rp2350-riscv
#
PICO_SERIAL_PORT = /dev/ttyUSB0
PICO_SERIAL_BAUD_RATE = 115200
PLATFORM = rp2350

UPLOADER = openocd 
UPCONFIG = -f interface/cmsis-dap.cfg -f target/rp2350.cfg 
UPCOMMANDS = -c "adapter speed 5000" -c "program build/risc_test.elf verify reset exit"

cmake:
	cp $(PICO_SDK_PATH)/external/pico_sdk_import.cmake .
	mkdir -p build
	rm -f build/CMakeCache.txt build/risc_test*
	rm -rf build/CMakeFiles build/generated build/pico-sdk build/pioasm build/pioasm-install
	rm -f build/CMakeDoxy* build/pico_flash*
	cd build ; cmake -DPICO_PLATFORM=$(PLATFORM) ..

rbuild: 
	cd build ; make -j4

upload: rbuild
	$(UPLOADER) $(UPCONFIG) $(UPCOMMANDS)

monitor:
	picocom -b 115200 /dev/ttyACM0

all: rbuild upload monitor

