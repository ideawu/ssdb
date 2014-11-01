include ../build_config.mk

OBJS = ssdb.o iterator.o \
	t_kv.o t_hash.o t_zset.o t_queue.o \
	proc_kv.o proc_hash.o proc_zset.o proc_queue.o \
	backend_dump.o backend_sync.o slave.o binlog.o \
	serv.o ttl.o
LIBS = ./util/libutil.a ./net/libnet.a
EXES = ../ssdb-server


all: ssdb.h ${OBJS} ssdb-server.o
	${CXX} -o ../ssdb-server ssdb-server.o ${OBJS} ${LIBS} ${CLIBS}

ssdb-server.o: ssdb-server.cpp
	${CXX} ${CFLAGS} -c ssdb-server.cpp

objs: ssdb.h ${OBJS}

ssdb.o: ssdb.h ssdb.cpp
	${CXX} ${CFLAGS} -c ssdb.cpp

iterator.o: ssdb.h iterator.h iterator.cpp
	${CXX} ${CFLAGS} -c iterator.cpp

t_kv.o: ssdb.h t_kv.h t_kv.cpp
	${CXX} ${CFLAGS} -c t_kv.cpp

t_hash.o: ssdb.h t_hash.h t_hash.cpp
	${CXX} ${CFLAGS} -c t_hash.cpp

t_zset.o: ssdb.h t_zset.h t_zset.cpp
	${CXX} ${CFLAGS} -c t_zset.cpp

t_queue.o: ssdb.h t_queue.h t_queue.cpp
	${CXX} ${CFLAGS} -c t_queue.cpp

binlog.o: ssdb.h binlog.h binlog.cpp
	${CXX} ${CFLAGS} -c binlog.cpp

slave.o: ssdb.h slave.h slave.cpp
	${CXX} ${CFLAGS} -c slave.cpp

backend_dump.o: ssdb.h backend_dump.h backend_dump.cpp
	${CXX} ${CFLAGS} -c backend_dump.cpp

backend_sync.o: ssdb.h backend_sync.h backend_sync.cpp
	${CXX} ${CFLAGS} -c backend_sync.cpp

ttl.o: ssdb.h ttl.h ttl.cpp
	${CXX} ${CFLAGS} -c ttl.cpp

proc.o: ssdb.h serv.h proc.cpp
	${CXX} ${CFLAGS} -c proc.cpp
proc_kv.o: ssdb.h serv.h proc_kv.cpp
	${CXX} ${CFLAGS} -c proc_kv.cpp
proc_hash.o: ssdb.h serv.h proc_hash.cpp
	${CXX} ${CFLAGS} -c proc_hash.cpp
proc_zset.o: ssdb.h serv.h proc_zset.cpp
	${CXX} ${CFLAGS} -c proc_zset.cpp
proc_queue.o: ssdb.h serv.h proc_queue.cpp
	${CXX} ${CFLAGS} -c proc_queue.cpp

serv.o: ssdb.h serv.h serv.cpp
	${CXX} ${CFLAGS} -c serv.cpp

clean:
	rm -f ${EXES} *.o *.exe

