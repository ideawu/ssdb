include ../../build_config.mk

OBJS = log.o fde.o config.o bytes.o sorted_set.o
EXES = 

all: ${OBJS}

log.o: log.h log.cpp
	${CXX} ${CFLAGS} -c log.cpp

config.o: config.h config.cpp
	${CXX} ${CFLAGS} -c config.cpp

fde.o: fde.h fde.cpp fde_select.cpp fde_epoll.cpp
	${CXX} ${CFLAGS} -c fde.cpp

bytes.o: bytes.h bytes.cpp
	${CXX} ${CFLAGS} -c bytes.cpp

sorted_set.o: sorted_set.h sorted_set.cpp
	${CXX} ${CFLAGS} -c sorted_set.cpp

clean:
	rm -f ${EXES} ${OBJS}

