include ../../build_config.mk

LIBSSDB_PATH = ../../api/cpp
CFLAGS = -g -D__STDC_FORMAT_MACROS -Wall -Wno-sign-compare
CFLAGS += -I${LIBSSDB_PATH} -I../ssdb
CLIBS += ${LIBSSDB_PATH}/libssdb-client.a ../util/libutil.a ../ssdb/libssdb.a

all: split.o cluster.o
	#g++ ${CFLAGS} -c ssdb-split.cpp
	#g++ -o ssdb-split ssdb-split.o split.o ${CLIBS}
	g++ -o test test-cluster.cpp cluster.o ${CLIBS}

cluster.o: cluster.h cluster.cpp
	g++ ${CFLAGS} -c cluster.cpp

split.o: split.h split.cpp
	g++ ${CFLAGS} -c split.cpp

clean:
	rm -f ssdb-split *.a *.o

