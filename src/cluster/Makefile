include ../../build_config.mk

LIBSSDB_PATH = ../../api/cpp
CFLAGS = -g -D__STDC_FORMAT_MACROS -Wall -Wno-sign-compare
CFLAGS += -I${LIBSSDB_PATH} -I../ssdb
CLIBS += ${LIBSSDB_PATH}/libssdb-client.a ../util/libutil.a ../ssdb/libssdb.a
OBJS = spliter.o cluster.o cluster_store.o node.o key_range.o

all: ${OBJS}
	g++ ${CFLAGS} -o test-cluster test-cluster.cpp ${OBJS} ${CLIBS}

cluster.o: cluster.h cluster.cpp
	g++ ${CFLAGS} -c cluster.cpp

cluster_store.o: cluster_store.h cluster_store.cpp
	g++ ${CFLAGS} -c cluster_store.cpp

node.o: node.h node.cpp
	g++ ${CFLAGS} -c node.cpp

key_range.o: key_range.h key_range.cpp
	g++ ${CFLAGS} -c key_range.cpp

spliter.o: spliter.h spliter.cpp
	g++ ${CFLAGS} -c spliter.cpp
	
test:
	g++ ${CFLAGS} -o test-spliter test-spliter.cpp ${OBJS} ${CLIBS}

clean:
	rm -f ssdb-split *.a *.o

