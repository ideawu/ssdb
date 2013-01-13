#!/bin/sh

echo "#ifndef SSDB_VERSION" > src/version.h
echo "#define SSDB_VERSION \"`cat version`\"" >> src/version.h
echo "#endif" >> src/version.h
