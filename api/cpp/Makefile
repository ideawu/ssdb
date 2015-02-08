include ../../build_config.mk

all: lib
	${CXX} -o demo demo.cpp libssdb-client.a
	${CXX} -o hello-ssdb hello-ssdb.cpp libssdb-client.a

lib: SSDB.h SSDB_impl.h SSDB_impl.cpp
	${CXX} -I../../src ${CFLAGS} -c SSDB_impl.cpp
	ar -cru libssdb-client.a\
		SSDB_impl.o\
		../../src/util/bytes.o\
		../../src/net/link.o

clean:
	rm -f demo hello-ssdb *.a *.o

