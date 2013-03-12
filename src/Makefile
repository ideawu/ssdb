LEVELDB=leveldb-1.8.0
LEVELDB_PATH = ../deps/${LEVELDB}
CFLAGS += -I./ -I${LEVELDB_PATH}/include

ifeq ($(shell uname), Darwin)
CLIBS += -L${LEVELDB_PATH} -lleveldb
else
CLIBS += -static -L${LEVELDB_PATH} -lleveldb
endif

OBJS = ssdb.o t_kv.o t_hash.o t_zset.o proc.o link.o backend_dump.o backend_sync.o repl.o slave.o
UTIL_OBJS = util/log.o util/fde.o util/config.o util/bytes.o
EXES = ../ssdb-server ../ssdb-server.exe ../tools/ssdb-dump ../tools/ssdb-dump.exe


all: ssdb.h ${OBJS}
	g++ -o ../ssdb-server ${OBJS} ${UTIL_OBJS} ${CFLAGS} ${CLIBS} -lpthread ssdb-server.cpp
	g++ -o ../tools/ssdb-dump ${OBJS} ${UTIL_OBJS} ${CFLAGS} ${CLIBS} -lpthread ssdb-dump.cpp

objs: ssdb.h ${OBJS}

ssdb.o: ssdb.h ssdb.cpp
	g++ ${CFLAGS} -c ssdb.cpp

t_kv.o: ssdb.h t_kv.h t_kv.cpp
	g++ ${CFLAGS} -c t_kv.cpp

t_hash.o: ssdb.h t_hash.h t_hash.cpp
	g++ ${CFLAGS} -c t_hash.cpp

t_zset.o: ssdb.h t_zset.h t_zset.cpp
	g++ ${CFLAGS} -c t_zset.cpp

link.o: ssdb.h link.h link.cpp
	g++ ${CFLAGS} -c link.cpp

repl.o: ssdb.h repl.h repl.cpp
	g++ ${CFLAGS} -c repl.cpp

slave.o: ssdb.h slave.h slave.cpp
	g++ ${CFLAGS} -c slave.cpp

proc.o: ssdb.h proc.h proc.cpp proc_kv.cpp proc_hash.cpp proc_zset.cpp
	g++ ${CFLAGS} -c proc.cpp

backend_dump.o: ssdb.h backend_dump.h backend_dump.cpp
	g++ ${CFLAGS} -c backend_dump.cpp

backend_sync.o: ssdb.h backend_sync.h backend_sync.cpp
	g++ ${CFLAGS} -c backend_sync.cpp

clean:
	rm -f ${EXES} ${OBJS}
	rm -f ../*.exe.stackdump

