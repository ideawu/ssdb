include ../../build_config.mk

LIBSSDB_PATH = ../../api/cpp
CFLAGS += -I${LIBSSDB_PATH} -I../ssdb
CLIBS += ${LIBSSDB_PATH}/libssdb-client.a ../util/libutil.a ../net/libnet.a ../ssdb/libssdb.a
OBJS = spliter.o cluster.o cluster_store.o node.o key_range.o serv.o

all: ${OBJS} ssdb-cluster.o
	${CXX} -o ssdb-cluster ssdb-cluster.o ${OBJS} ${LIBS} ${CLIBS}

#${CXX} ${CFLAGS} -o test-cluster test-cluster.cpp ${OBJS} ${CLIBS}

ssdb-cluster.o: ssdb-cluster.cpp
	${CXX} ${CFLAGS} -c ssdb-cluster.cpp
serv.o: serv.h serv.cpp
	${CXX} ${CFLAGS} -c serv.cpp

cluster.o: cluster.h cluster.cpp
	${CXX} ${CFLAGS} -c cluster.cpp

cluster_store.o: cluster_store.h cluster_store.cpp
	${CXX} ${CFLAGS} -c cluster_store.cpp

node.o: node.h node.cpp
	${CXX} ${CFLAGS} -c node.cpp

key_range.o: key_range.h key_range.cpp
	${CXX} ${CFLAGS} -c key_range.cpp

spliter.o: spliter.h spliter.cpp
	${CXX} ${CFLAGS} -c spliter.cpp
	
test:
	${CXX} ${CFLAGS} -o test-spliter test-spliter.cpp ${OBJS} ${CLIBS}

clean:
	rm -f ssdb-split *.a *.o

