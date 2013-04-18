#!/bin/sh
echo LEVELDB=leveldb-1.8.0


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

echo PLATFORM_LDFLAGS=$PLATFORM_LDFLAGS
