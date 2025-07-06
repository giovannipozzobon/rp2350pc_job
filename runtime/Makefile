include ../environment/system.make

cmake:
	mkdir -p build
	rm -f build/CMakeCache.txt 
	cd build ; cmake -DMODULEDIR=$(MODULEDIR) ..

compile: 
	cd build ; make -j10

run: compile
	build/runtime

rebuilddvi:
	make -C ../modules/dvi compile upload