include ../build_config.mk

OBJS = ssdb.o t_kv.o t_hash.o t_zset.o link.o \
	backend_dump.o backend_sync.o slave.o binlog.o serv.o \
	iterator.o
UTIL_OBJS = util/log.o util/fde.o util/config.o util/bytes.o
EXES = ../ssdb-server \
	../tools/leveldb-import \
	../tools/ssdb-repair \
	../tools/ssdb-dump


all: ssdb.h ${OBJS} ssdb-server.o ssdb-dump.o ssdb-repair.o leveldb-import.o
	g++ -o ../ssdb-server ssdb-server.o ${OBJS} ${UTIL_OBJS} ${CLIBS}
	g++ -o ../tools/ssdb-dump ssdb-dump.o ${OBJS} ${UTIL_OBJS} ${CLIBS}
	g++ -o ../tools/ssdb-repair ssdb-repair.o ${OBJS} ${UTIL_OBJS} ${CLIBS}
	g++ -o ../tools/leveldb-import leveldb-import.o ${OBJS} ${UTIL_OBJS} ${CLIBS}

ssdb-server.o: ssdb-server.cpp
	g++ ${CFLAGS} -c ssdb-server.cpp
ssdb-dump.o: ssdb-dump.cpp
	g++ ${CFLAGS} -c ssdb-dump.cpp
ssdb-repair.o: ssdb-repair.cpp
	g++ ${CFLAGS} -c ssdb-repair.cpp
leveldb-import.o: leveldb-import.cpp
	g++ ${CFLAGS} -c leveldb-import.cpp

objs: ssdb.h ${OBJS}

ssdb.o: ssdb.h ssdb.cpp
	g++ ${CFLAGS} -c ssdb.cpp

iterator.o: ssdb.h iterator.h iterator.cpp
	g++ ${CFLAGS} -c iterator.cpp

t_kv.o: ssdb.h t_kv.h t_kv.cpp
	g++ ${CFLAGS} -c t_kv.cpp

t_hash.o: ssdb.h t_hash.h t_hash.cpp
	g++ ${CFLAGS} -c t_hash.cpp

t_zset.o: ssdb.h t_zset.h t_zset.cpp
	g++ ${CFLAGS} -c t_zset.cpp

link.o: ssdb.h link.h link.cpp
	g++ ${CFLAGS} -c link.cpp

binlog.o: ssdb.h binlog.h binlog.cpp
	g++ ${CFLAGS} -c binlog.cpp

slave.o: ssdb.h slave.h slave.cpp
	g++ ${CFLAGS} -c slave.cpp

serv.o: ssdb.h serv.h serv.cpp proc_kv.cpp proc_hash.cpp proc_zset.cpp
	g++ ${CFLAGS} -c serv.cpp

backend_dump.o: ssdb.h backend_dump.h backend_dump.cpp
	g++ ${CFLAGS} -c backend_dump.cpp

backend_sync.o: ssdb.h backend_sync.h backend_sync.cpp
	g++ ${CFLAGS} -c backend_sync.cpp

clean:
	rm -f ${EXES} *.o *.exe

