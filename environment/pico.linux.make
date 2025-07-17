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
#		Location of Pico SDK (if not set by default)
#
export PICO_SDK_PATH=/usr/share/pico-sdk

# *******************************************************************************************
#
#				Select one or the other.  Note: if you switch cores, you will have
#				to upload a working core via UF2/Boot before the debug connection 
#				will work.
#
# *******************************************************************************************
#
# 		Platform selection. (ARM)
#
PLATFORM = rp2350
#
#		Platform selection (RISCV)
#
#PLATFORM = rp2350-riscv
#export PICO_TOOLCHAIN_PATH=/aux/builds/corev-openhw-gcc-ubuntu2204-20240530
#export PICO_RISCV_TOOLCHAIN_PATH=/aux/builds/corev-openhw-gcc-ubuntu2204-20240530



#
#		Serial debugging port.
#
PICO_SERIAL_PORT = /dev/ttyACM0
PICO_SERIAL_BAUD_RATE = 115200
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
