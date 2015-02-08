LIBSSDB_PATH = ../../api/cpp
#CFLAGS = -DNDEBUG -O2 -D__STDC_FORMAT_MACROS -Wall -Wno-sign-compare
CFLAGS = -g -D__STDC_FORMAT_MACROS -Wall -Wno-sign-compare
CFLAGS += -I${LIBSSDB_PATH}
CLIBS = ${LIBSSDB_PATH}/libssdb-client.a ../util/libutil.a

all:
	g++ ${CFLAGS} -c ssdb-split.cpp
	g++ -o ssdb-split ssdb-split.o ${CLIBS}

clean:
	rm -f ssdb-split *.a *.o

