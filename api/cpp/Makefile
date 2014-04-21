include ../../build_config.mk

all: lib
	${CXX} -o demo demo.cpp libssdb.a
	${CXX} -o hello-ssdb hello-ssdb.cpp libssdb.a

lib: SSDB.h SSDB_impl.h SSDB_impl.cpp
	${CXX} -I../../src ${CFLAGS} -c SSDB_impl.cpp
	ar -cru libssdb.a\
		SSDB_impl.o\
		../../src/util/bytes.o\
		../../src/util/log.o\
		../../src/link.o

clean:
	rm -f demo hello-ssdb libssdb.a SSDB_impl.o
