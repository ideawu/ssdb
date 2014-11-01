include ../../build_config.mk

OBJS = log.o config.o bytes.o sorted_set.o
EXES = 

all: ${OBJS}
	ar -cru ./libutil.a ${OBJS}

log.o: log.h log.cpp
	${CXX} ${CFLAGS} -c log.cpp

config.o: config.h config.cpp
	${CXX} ${CFLAGS} -c config.cpp

bytes.o: bytes.h bytes.cpp
	${CXX} ${CFLAGS} -c bytes.cpp

sorted_set.o: sorted_set.h sorted_set.cpp
	${CXX} ${CFLAGS} -c sorted_set.cpp

clean:
	rm -f ${EXES} ${OBJS} *.o *.exe

