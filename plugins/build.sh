#!/bin/bash

g++ -I ../src -I ../deps/jemalloc-3.3.1/include -I ../deps/leveldb-1.14.0/include -I ../deps/snappy-1.1.0 -shared -fPIC simple_plugin.cpp -o /dev/shm/simple_plugin.so
