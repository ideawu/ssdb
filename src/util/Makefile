include ../../build_config.mk

OBJS = log.o config.o bytes.o sorted_set.o app.o
EXES = 

all: ${OBJS}
	ar -cru ./libutil.a ${OBJS}

app.o: app.h app.cpp
	${CXX} ${CFLAGS} -c app.cpp

log.o: log.h log.cpp
	${CXX} ${CFLAGS} -c log.cpp

config.o: config.h config.cpp
	${CXX} ${CFLAGS} -c config.cpp

bytes.o: bytes.h bytes.cpp
	${CXX} ${CFLAGS} -c bytes.cpp

sorted_set.o: sorted_set.h sorted_set.cpp
	${CXX} ${CFLAGS} -c sorted_set.cpp

test:
	$(CXX) ${CFLAGS} test_sorted_set.cpp $(OBJS)

clean:
	rm -f ${EXES} ${OBJS} *.o *.exe *.a

