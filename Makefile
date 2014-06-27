PREFIX=/usr/local/ssdb

$(shell sh build.sh 1>&2)
include build_config.mk

all:
	chmod u+x "${LEVELDB_PATH}/build_detect_platform"
	chmod u+x deps/cpy/cpy
	chmod u+x tools/ssdb-cli
	cd "${LEVELDB_PATH}"; ${MAKE}
	cd src/util; ${MAKE}
	cd src; ${MAKE}
	cd tools; ${MAKE}

install:
	mkdir -p ${PREFIX}
	mkdir -p ${PREFIX}/_cpy_
	mkdir -p ${PREFIX}/deps
	mkdir -p ${PREFIX}/var
	mkdir -p ${PREFIX}/var_slave
	cp ssdb-server ssdb.conf ssdb_slave.conf ${PREFIX}
	cp -r api ${PREFIX}
	cp -r \
		tools/ssdb-bench tools/ssdb-cli \
		tools/ssdb-cli.cpy tools/ssdb-dump \
		tools/ssdb-repair \
		tools/ssdb-ins.sh tools/unittest.php \
		${PREFIX}
	cp -r deps/cpy ${PREFIX}/deps
	chmod ugo+rwx ${PREFIX}
	chmod -R ugo+rw ${PREFIX}
	rm -f ${PREFIX}/Makefile

clean:
	rm -f *.exe.stackdump
	rm -rf api/cpy/_cpy_
	rm -f api/python/SSDB.pyc
	rm -rf db_test
	cd deps/cpy; ${MAKE} clean
	cd src/util; ${MAKE} clean
	cd src; ${MAKE} clean
	cd tools; ${MAKE} clean

clean_all: clean
	cd "${LEVELDB_PATH}"; ${MAKE} clean
	rm -f ${JEMALLOC_PATH}/Makefile
	rm -f ${SNAPPY_PATH}/Makefile
	