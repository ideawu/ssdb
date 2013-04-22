#!/bin/sh

echo ""
echo "building jemalloc..."
DIR=`pwd`
cd deps/jemalloc-3.3.1
if [ ! -f Makefile ]; then
	./configure
   	make
fi
cd $DIR
echo "building jemalloc finished"
echo ""


echo "#ifndef SSDB_VERSION" > src/version.h
echo "#define SSDB_VERSION \"`cat version`\"" >> src/version.h
echo "#endif" >> src/version.h


TARGET_OS=`uname -s`
case "$TARGET_OS" in
    Darwin)
        PLATFORM_LDFLAGS="-pthread"
        ;;
    Linux)
        PLATFORM_LDFLAGS="-pthread"
        ;;
    CYGWIN_*)
        PLATFORM_LDFLAGS="-pthread"
        ;;
    SunOS)
        PLATFORM_LIBS="-lpthread -lrt"
        ;;
    FreeBSD)
        PLATFORM_LIBS="-lpthread"
        ;;
    NetBSD)
        PLATFORM_LIBS="-lpthread -lgcc_s"
        ;;
    OpenBSD)
        PLATFORM_LDFLAGS="-pthread"
        ;;
    DragonFly)
        PLATFORM_LIBS="-lpthread"
        ;;
    OS_ANDROID_CROSSCOMPILE)
        PLATFORM_LDFLAGS=""  # All pthread features are in the Android C library
        ;;
    HP-UX)
        PLATFORM_LDFLAGS="-pthread"
        ;;
    *)
        echo "Unknown platform!" >&2
        exit 1
esac

rm -f build_config.mk
echo "LEVELDB=leveldb-1.8.0" >> build_config.mk
echo "PLATFORM_LDFLAGS=$PLATFORM_LDFLAGS" >> build_config.mk

