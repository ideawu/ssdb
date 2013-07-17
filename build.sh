#!/bin/sh
BASE_DIR=`pwd`
TARGET_OS=`uname -s`
JEMALLOC_PATH="$BASE_DIR/deps/jemalloc-3.3.1"
LEVELDB_PATH="$BASE_DIR/deps/leveldb-1.9.0"

case "$TARGET_OS" in
    Darwin)
        PLATFORM_LDFLAGS="-pthread"
        ;;
    Linux)
        PLATFORM_LDFLAGS="-pthread"
        ;;
    CYGWIN_*)
        PLATFORM_LDFLAGS="-lpthread"
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



if [[ $TARGET_OS == CYGWIN* ]]; then
	echo "not using jemalloc on $TARGET_OS"
else
	echo ""
	echo "building jemalloc..."
	DIR=`pwd`
	cd deps/jemalloc-3.3.1
	if [ ! -f Makefile ]; then
		./configure
		make
	fi
	cd "$DIR"
	echo "building jemalloc finished"
	echo ""
fi


rm -f src/version.h
echo "#ifndef SSDB_DEPS_H" >> src/version.h
echo "#ifndef SSDB_VERSION" >> src/version.h
echo "#define SSDB_VERSION \"`cat version`\"" >> src/version.h
if [[ $TARGET_OS == CYGWIN* ]]; then
	:
else
	echo "#include <jemalloc/jemalloc.h>" >> src/version.h
fi
echo "#endif" >> src/version.h
echo "#endif" >> src/version.h

rm -f build_config.mk
echo "LEVELDB_PATH=$LEVELDB_PATH" >> build_config.mk
echo "JEMALLOC_PATH=$JEMALLOC_PATH" >> build_config.mk
echo "PLATFORM_LDFLAGS += $PLATFORM_LDFLAGS" >> build_config.mk
echo "PLATFORM_LDFLAGS += \"$LEVELDB_PATH/libleveldb.a\"" >> build_config.mk
echo "PLATFORM_CFLAGS += -I \"$LEVELDB_PATH/include\"" >> build_config.mk

if [[ $TARGET_OS == CYGWIN* ]]; then
	:
else
	echo "PLATFORM_LDFLAGS += \"$JEMALLOC_PATH/lib/libjemalloc.a\"" >> build_config.mk
	echo "PLATFORM_CFLAGS += -I \"$JEMALLOC_PATH/include\"" >> build_config.mk
fi

