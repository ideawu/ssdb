include ../../build_config.mk

OBJS = server.o resp.o proc.o worker.o fde.o link.o
UTIL_OBJS = ../util/log.o ../util/config.o ../util/bytes.o
EXES = test

all: ${OBJS}
	ar -cru ./libnet.a ${OBJS}

fde.o: fde.h fde.cpp fde_select.cpp fde_epoll.cpp
	${CXX} ${CFLAGS} -c fde.cpp
link.o: link.h link.cpp link_redis.h link_redis.cpp
	${CXX} ${CFLAGS} -c link.cpp
resp.o: resp.h resp.cpp
	${CXX} ${CFLAGS} -c resp.cpp
proc.o: proc.h proc.cpp
	${CXX} ${CFLAGS} -c proc.cpp
worker.o: worker.h worker.cpp
	${CXX} ${CFLAGS} -c worker.cpp
server.o: server.h server.cpp
	${CXX} ${CFLAGS} -c server.cpp

test:
	${CXX} -o test.out test.cpp ${CFLAGS} ${OBJS} ${UTIL_OBJS} ${CLIBS}
	${CXX} -o test2.out test2.cpp ${CFLAGS} ${OBJS} ${UTIL_OBJS} ${CLIBS}

clean:
	rm -f ${EXES} *.a *.o *.exe
