all:
	g++ -o demo\
		-D__STDC_FORMAT_MACROS\
		-I../../deps/jemalloc-3.3.1/include\
		-I../../deps/leveldb-1.14.0/include\
	   	-I../../src ../../src/util/bytes.o\
	   	../../src/util/log.o ../../src/link.o\
	   	demo.cpp

