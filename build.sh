#!/bin/sh
BASE_DIR=`pwd`
TARGET_OS=`uname -s`
JEMALLOC_PATH="$BASE_DIR/deps/jemalloc-3.3.1"
LEVELDB_PATH="$BASE_DIR/deps/leveldb-1.14.0"
SNAPPY_PATH="$BASE_DIR/deps/snappy-1.1.0"

case "$TARGET_OS" in
    Darwin)
        #PLATFORM_CLIBS="-pthread"
        ;;
    Linux)
        PLATFORM_CLIBS="-pthread"
        ;;
    CYGWIN_*)
        PLATFORM_CLIBS="-lpthread"
        ;;
    SunOS)
        PLATFORM_CLIBS="-lpthread -lrt"
        ;;
    FreeBSD)
        PLATFORM_CLIBS="-lpthread"
        ;;
    NetBSD)
        PLATFORM_CLIBS="-lpthread -lgcc_s"
        ;;
    OpenBSD)
        PLATFORM_CLIBS="-pthread"
        ;;
    DragonFly)
        PLATFORM_CLIBS="-lpthread"
        ;;
    HP-UX)
        PLATFORM_CLIBS="-pthread"
        ;;
    *)
        echo "Unknown platform!" >&2
        exit 1
esac


DIR=`pwd`
cd $SNAPPY_PATH
if [ ! -f Makefile ]; then
	echo ""
	echo "##### building snappy... #####"
	./configure
	make
	echo "##### building snappy finished #####"
	echo ""
fi
cd "$DIR"


if [[ $TARGET_OS == CYGWIN* ]]; then
	echo "not using jemalloc on $TARGET_OS"
else
	DIR=`pwd`
	cd $JEMALLOC_PATH
	if [ ! -f Makefile ]; then
		echo ""
		echo "##### building jemalloc... #####"
		./configure
		make
		echo "##### building jemalloc finished #####"
		echo ""
	fi
	cd "$DIR"
fi


rm -f src/version.h
echo "#ifndef SSDB_DEPS_H" >> src/version.h
echo "#ifndef SSDB_VERSION" >> src/version.h
echo "#define SSDB_VERSION \"`cat version`\"" >> src/version.h
if [[ $TARGET_OS == CYGWIN* ]]; then
	:
else
	echo "#include <stdlib.h>" >> src/version.h
	echo "#include <jemalloc/jemalloc.h>" >> src/version.h
fi
echo "#endif" >> src/version.h
echo "#endif" >> src/version.h

rm -f build_config.mk
echo "LEVELDB_PATH=$LEVELDB_PATH" >> build_config.mk
echo "JEMALLOC_PATH=$JEMALLOC_PATH" >> build_config.mk

echo "CFLAGS=" >> build_config.mk
echo "CFLAGS = -DNDEBUG -D__STDC_FORMAT_MACROS -Wall -O2 -Wno-sign-compare" >> build_config.mk
echo "CFLAGS += ${PLATFORM_CFLAGS}" >> build_config.mk
echo "CFLAGS += -I \"$LEVELDB_PATH/include\"" >> build_config.mk

echo "CLIBS=" >> build_config.mk
echo "CLIBS += ${PLATFORM_CLIBS}" >> build_config.mk
echo "CLIBS += \"$LEVELDB_PATH/libleveldb.a\"" >> build_config.mk
echo "CLIBS += \"$SNAPPY_PATH/.libs/libsnappy.a\"" >> build_config.mk


if [[ $TARGET_OS == CYGWIN* ]]; then
	:
else
	echo "CLIBS += \"$JEMALLOC_PATH/lib/libjemalloc.a\"" >> build_config.mk
	echo "CFLAGS += -I \"$JEMALLOC_PATH/include\"" >> build_config.mk
fi

