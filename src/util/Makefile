include ../../build_config.mk

OBJS = log.o fde.o config.o bytes.o sorted_set.o
EXES = 

all: ${OBJS}

log.o: log.h log.cpp
	g++ ${CFLAGS} -c log.cpp

config.o: config.h config.cpp
	g++ ${CFLAGS} -c config.cpp

fde.o: fde.h fde.cpp fde_select.cpp fde_epoll.cpp
	g++ ${CFLAGS} -c fde.cpp

bytes.o: bytes.h bytes.cpp
	g++ ${CFLAGS} -c bytes.cpp

sorted_set.o: sorted_set.h sorted_set.cpp
	g++ ${CFLAGS} -c sorted_set.cpp


test: sorted_set.o log.o
	g++ -o test_sorted_set ${CFLAGS} sorted_set.o log.o test_sorted_set.cpp
	

clean:
	rm -f ${EXES} ${OBJS}

