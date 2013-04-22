#!/bin/sh

prefix=/usr/local
exec_prefix=/usr/local
libdir=${exec_prefix}/lib

DYLD_INSERT_LIBRARIES=${libdir}/libjemalloc.1.dylib
export DYLD_INSERT_LIBRARIES
exec "$@"
