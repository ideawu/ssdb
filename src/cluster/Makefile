include ../../build_config.mk

LIBSSDB_PATH = ../../api/cpp
CFLAGS = -g -D__STDC_FORMAT_MACROS -Wall -Wno-sign-compare
CFLAGS += -I${LIBSSDB_PATH} -I../ssdb
CLIBS += ${LIBSSDB_PATH}/libssdb-client.a ../util/libutil.a ../ssdb/libssdb.a

all: spliter.o cluster.o node.o
	g++ ${CFLAGS} -o test-spliter test-spliter.cpp spliter.o node.o ${CLIBS}
	g++ ${CFLAGS} -o test-cluster test-cluster.cpp cluster.o node.o ${CLIBS}

cluster.o: cluster.h cluster.cpp
	g++ ${CFLAGS} -c cluster.cpp

node.o: node.h node.cpp
	g++ ${CFLAGS} -c node.cpp

spliter.o: spliter.h spliter.cpp
	g++ ${CFLAGS} -c spliter.cpp

clean:
	rm -f ssdb-split *.a *.o

