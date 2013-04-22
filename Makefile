PREFIX=/usr/local/ssdb

$(shell sh build.sh > build_config.mk)
include build_config.mk

all:
	chmod u+x deps/${LEVELDB}/build_detect_platform
	chmod u+x deps/cpy/cpy
	chmod u+x tools/ssdb-cli tools/ssdb-benchmark
	cd deps/jemalloc-3.3.1; if [ ! -f Makefile ]; then ./configure; make; fi
	cd deps/${LEVELDB}; make
	$(eval export CFLAGS=-DNDEBUG)
	cd src/util; make
	cd src; make
	cd tools; make

install:
	mkdir -p ${PREFIX}
	mkdir -p ${PREFIX}/deps
	cp -r api ${PREFIX}
	cp -r tools/* ${PREFIX}
	cp -r deps/cpy ${PREFIX}/deps
	chmod ugo+rwx ${PREFIX}

clean:
	rm -f *.exe.stackdump
	rm -rf api/cpy/_cpy_
	rm -f api/python/SSDB.pyc
	rm -rf db_test
	cd deps/cpy; make clean
	#cd deps/${LEVELDB}; make clean
	cd src/util; make clean
	cd src; make clean
	cd tools; make clean


